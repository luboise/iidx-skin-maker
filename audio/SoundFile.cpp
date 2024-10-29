#include <sndfile.h>

#include <cstdint>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

#include <filesystem>
#include <istream>
#include <sstream>

#include "SoundFile.h"
// #include "utils.h"

#define SWING_RATIO (2.0 / 3.0)
#define PITCH_WINDOW (4096)
#define LANCZOS_WINDOW 3

#define WINDOW_SIZE (4096 * 1.6)
#define OVERLAP_RATIO_FAST (0.25)
#define OVERLAP_RATIO_SLOW (0.625)

using std::istream;

SoundFile::SoundFile(char* data, uint32_t size) {
    this->parseAudioData(data, size);
}

SoundFile::SoundFile(const fs::path& filepath) {
    SF_INFO info;

    if (!fs::exists(filepath) || fs::is_directory(filepath)) {
        std::cerr << "Unable to locate file: \"" << filepath
                  << "\" or it is a directory and not a file." << std::endl;
        return;
    }

    SNDFILE* snd = sf_open(filepath.string().c_str(), SFM_READ, &info);

    if (snd == nullptr) {
        std::cerr << "Error opening sound file \"" << filepath << "\""
                  << std::endl;
        return;
    }

    this->initialiseData(*snd, info);
    sf_close(snd);
}

SoundFile::~SoundFile() {}

sf_count_t read_callback(void* ptr, sf_count_t count, void* user_data) {
    // StreamDetails& details = *(StreamDetails*)user_data;
    auto& stream = *((istream*)user_data);

    stream.read((char*)ptr, count);

    return stream.gcount();
}

sf_count_t tell_callback(void* user_data) {
    auto& stream = *((istream*)user_data);

    return stream.tellg();
}

// Seek callback for libsndfile
sf_count_t seek_callback(sf_count_t offset, int whence, void* user_data) {
    auto& stream = *((istream*)user_data);

    if (whence == SEEK_SET) {
        stream.seekg(offset, std::ios::beg);
    } else if (whence == SEEK_CUR) {
        stream.seekg(offset);
    } else if (whence == SEEK_END) {
        stream.seekg(0, std::ios::end);
    }

    return stream.tellg();
}

sf_count_t file_length_callback(void* user_data) {
    auto& stream = *((istream*)user_data);

    // Backup cursor
    const auto curr = stream.tellg();

    // Go to the end and then get the real filesize, then restore
    stream.seekg(0, std::ios::end);
    const sf_count_t length = stream.tellg();
    stream.seekg(curr, std::ios::beg);

    return length;
}

bool SoundFile::parseAudioData(char* data, uint32_t size) {
    std::string str(data, size);
    std::istringstream iss(str);

    SF_INFO info;

    SF_VIRTUAL_IO virtual_io = {file_length_callback, seek_callback,
                                read_callback, nullptr, tell_callback};

    SNDFILE* snd = sf_open_virtual(&virtual_io, SFM_READ, &info, &iss);

    if (snd == nullptr) {
        const char* error_text = sf_strerror(nullptr);

        std::stringstream ss;
        ss << "Error opening sound file: " << error_text;

        return false;
    }

    this->initialiseData(*snd, info);
    sf_close(snd);

    return true;
};

// SoundFile(std::istringstream&&){};
/*
SoundFile::SoundFile(std::istream& stream) {
    StreamDetails details{&stream, stream.tellg()};

    SF_INFO info;

    SF_VIRTUAL_IO virtual_io = {
        file_length_callback, seek_callback, read_callback,
        NULL,  // Write callback not needed
        NULL   // Tell callback not needed
    };

    SNDFILE* snd = sf_open_virtual(&virtual_io, SFM_READ, &info, &stream);

    if (snd == nullptr) {
        const char* error_text = sf_strerror(nullptr);

        std::stringstream ss;
        ss << "Error opening sound file: " << error_text;

        throw std::runtime_error(ss.str());
    }

    this->initialiseData(*snd, info);
    sf_close(snd);
}
*/

void SoundFile::initialiseData(SNDFILE& snd, SF_INFO& info) {
    const uint32_t sample_count = info.frames * info.channels;

    std::vector<SampleType> raw_samples(sample_count);

    auto read_count = sf_readf_float(&snd, raw_samples.data(), info.frames);
    if (read_count != info.frames) {
        std::cerr << "Read failed."
                  << ", frames read: " << read_count << "/" << info.frames
                  << std::endl;
        return;
    }

    std::vector<SampleList> channels(info.channels);
    for (SampleList& channel : channels) {
        channels.reserve(info.frames);
    }

    for (size_t sample = 0; sample < raw_samples.size();
         sample += info.channels) {
        for (size_t channel = 0; channel < info.channels; channel++) {
            channels[channel].push_back(raw_samples[sample + channel]);
        }
    }

    this->_sndinfo = info;
    this->_channels = channels;
};

bool SoundFile::isValid() const { return this->_channels.size() > 0; }

// void SoundFile::addSwing(const double bpm, double offset) {
//     double beat_length;
//
//     beat_length = 60 / bpm;
//
//     if (offset > 0) {
//         offset = std::fmod(offset, beat_length) - beat_length;
//     }
//
//     uint32_t beat = 0;
//
//     // Trackers in seconds
//     double tracker_l = offset;
//
//     int32_t left_frame = -1;
//     int32_t right_frame = -1;
//
//     const auto length = this->getSoundLength();
//     while (tracker_l < length) {
//         // Get frame counter
//         left_frame = tracker_l * _sndinfo.samplerate;
//         right_frame = (tracker_l + beat_length) *
//         (double)_sndinfo.samplerate;
//
//         swingFrames(left_frame, right_frame);
//
//         // Update tracker
//         tracker_l = offset + beat_length * ++beat;
//     }
// }

void SoundFile::exportToFile(const fs::path& filename) {
    if (!this->isValid()) {
        std::cerr << "Attempting to write invalid file. Skipping..."
                  << std::endl;
        return;
    }

    this->resizeChannels();

    SampleList outSamples(this->getSampleCount());

    size_t channel_count = _channels.size();

    for (size_t i = 0; i < this->_channels[0].size(); i++) {
        for (size_t j = 0; j < channel_count; j++) {
            outSamples[i * channel_count + j] = _channels[j][i];
        }
    }

    // Remove const binding from const function
    SF_INFO info = _sndinfo;
    SNDFILE* snd = sf_open(filename.string().c_str(), SFM_WRITE, &info);

    if (snd == nullptr) {
        std::cerr << "Unable to open path" << filename << " for writing.";
        return;
    }

    sf_write_float(snd, outSamples.data(), outSamples.size());
    sf_close(snd);
}

double SoundFile::getSoundLength() const {
    return (double)_sndinfo.frames / _sndinfo.samplerate;
}

SampleList SoundFile::getStretched(double stretchFactor) const {
    return getStretched(stretchFactor, 0, _sndinfo.frames - 1);
}

SampleList SoundFile::getStretched(double stretchFactor, int32_t startFrame,
                                   int32_t endFrame) const {
    const int32_t startSample = startFrame * _sndinfo.channels;

    const int32_t frame_count = endFrame - startFrame;
    const int32_t sample_count = frame_count * _sndinfo.channels;

    int32_t out_frames = floor(frame_count / stretchFactor);

    SampleList stretched(out_frames * _sndinfo.channels);

    uint32_t current_sample = 0;
    for (int frame = 0; frame < out_frames; frame++) {
        // pos is [0.0, 1.0]
        double pos = (double)frame / (frame_count - 1);

        // Get the frame offset that we want the real value from
        double offset = pos * frame_count * stretchFactor;

        double weight_r = fmod(offset, 1.0);
        double weight_l = 1 - weight_r;

        int32_t wanted_frame = (startFrame + (uint32_t)offset);
        auto left_frame = this->getFrame(wanted_frame);
        auto right_frame = this->getFrame(wanted_frame + 1);

        for (size_t i = 0; i < _sndinfo.channels; i++) {
            double interpolated_val =
                weight_l * left_frame[i] + weight_r * right_frame[i];
            stretched[current_sample++] = interpolated_val;
        }
    }

    return stretched;
}

SampleList SoundFile::getFrame(const int32_t frameIndex) const {
    SampleList frame(_sndinfo.channels);

    for (size_t i = 0; i < _sndinfo.channels; i++) {
        frame[i] = _channels[i][frameIndex];
    }

    return frame;
}

void SoundFile::setChannel(size_t channel, size_t offset,
                           SampleList&& samples) {
    _channels[channel] = std::move(samples);

    /*if (samples.size() <= _channels[channel].size())
    size_t index;
    for (size_t i = 0; i < samples.size(); i++) {
            index = (offset + i) * _sndinfo.channels + channel;
            _channels[channel] = samples[i];
    }*/
}

// SampleList SoundFile::getSamples() const { return this->_samples; }

SampleList SoundFile::getChannel(uint8_t channel) const {
    if (channel >= _channels.size()) {
        std::cerr << "Attempted to get channel at invalid index " << channel
                  << "." << std::endl;
        return {};
    }

    auto new_list = SampleList(_channels[channel]);

    return new_list;
}

size_t SoundFile::getChannelCount() const { return _sndinfo.channels; }
size_t SoundFile::getSampleCount() const {
    if (_channels.size() == 0) return 0;
    return _channels.size() * _channels[0].size();
    ;
}

double lanczosWindow(double x, double a) {
    if (std::abs(x) < 1e-8) return 1.0;
    if (std::abs(x) >= a) {
        return 0.0;
    }
    return std::sin(M_PI * x) * std::sin(M_PI * x / a) / (M_PI * M_PI * x * x);
}

std::vector<SampleList> SoundFile::getWindows(const SampleList& samples,
                                              size_t windowSize,
                                              double hopSize) const {
    std::vector<SampleList> windows;
    uint32_t l_index = 0;
    uint32_t r_index = 0;

    size_t window_index = 0;

    const size_t size = samples.size();
    while (true) {
        l_index = window_index++ * hopSize;
        r_index = l_index + windowSize;

        if (r_index >= size) {
            SampleList window(samples.begin() + l_index, samples.end());
            windows.push_back(window);
            break;
        } else {
            SampleList window(samples.begin() + l_index,
                              samples.begin() + r_index);
            windows.push_back(window);
        }
    }

    return windows;
}

double SoundFile::getNewBeatPosition(double beat, double ratio,
                                     bool removingSwing) {
    double threshold = (removingSwing) ? ratio : 0.5;

    double subbeat = fmod(beat, 1.0);

    if (subbeat < threshold) {
        subbeat *= ratio / threshold;
    } else {
        subbeat -= threshold;
        subbeat *= (1 - ratio) / threshold;
        subbeat += threshold;
    }

    return floor(beat) + subbeat;
}

void SoundFile::resizeChannels() {
    uint32_t maxSize = 0;
    for (const auto& channel : _channels) {
        if (channel.size() > maxSize) {
            maxSize = channel.size();
        }
    }

    for (auto& channel : _channels) {
        if (channel.size() < maxSize) {
            channel.resize(maxSize);
        }
    }

    _sndinfo.frames = (_channels.size() > 0) ? _channels[0].size() : 0;
}

void SoundFile::changeVolume(const double newVolume) {
    for (auto& channel : _channels) {
        for (auto& sample : channel) {
            sample *= newVolume;
        }
    }
}

void SoundFile::normalise() {
    if (_channels.size() == 0 || _channels[0].size() == 0) {
        return;
    }
    double maxSample = abs(_channels[0][0]);
    for (auto& channel : _channels) {
        for (const auto& sample : channel) {
            auto newVal = abs(sample);
            if (newVal > maxSample) maxSample = newVal;
        }
    }

    double scalar = 1 / maxSample;
    for (auto& channel : _channels) {
        for (auto& sample : channel) {
            sample *= scalar;
        }
    }
}

// void SoundFile::swingFrames(const int32_t leftFrame, const int32_t
// rightFrame) {
//     const size_t frame_count = rightFrame - leftFrame;
//     const size_t sample_count = frame_count * _sndinfo.channels;
//     const size_t global_sample_count = _sndinfo.frames *
//     _sndinfo.channels;
//
//     const int32_t leftSample = leftFrame * _sndinfo.channels;
//
//     std::vector<double> splice(sample_count);
//
//     for (size_t i = 0; i < sample_count; i++) {
//         auto index = leftSample + i;
//
//         if (index >= 0 && index < global_sample_count) {
//             splice[i] = _samples[index];
//         } else {
//             splice[i] = 0;
//         }
//     }
//
//     for (size_t current_channel = 0; current_channel < _sndinfo.channels;
//          current_channel++) {
//         for (size_t frame_index = 1; frame_index < frame_count - 1;
//              frame_index++) {
//             double pos = (double)frame_index / (double)(frame_count - 1);
//
//             // Normalised pos
//             double transposed_pos;
//             if (pos <= SWING_RATIO) {
//                 transposed_pos = pos / SWING_RATIO * 0.5;
//             } else {
//                 transposed_pos = (pos - SWING_RATIO) / (2 * (1 -
//                 SWING_RATIO)); transposed_pos += 0.5;
//             }
//
//             // suppose relevantframe = 3.4, then we want 60% of 3 and 40%
//             of 4 double weight_r = fmod(transposed_pos, 1.0); double
//             weight_l = 1 - weight_r;
//
//             int64_t localFrame = transposed_pos * frame_count;
//             int64_t localSample =
//                 localFrame * _sndinfo.channels + current_channel;
//
//             int64_t lsampleindex = leftSample + localSample;
//             int64_t rsampleindex = lsampleindex + _sndinfo.channels;
//
//             if (lsampleindex < 0 || lsampleindex >= global_sample_count
//             ||
//                 rsampleindex >= global_sample_count) {
//                 continue;
//             }
//
//             double val = (weight_l * _samples[lsampleindex]) +
//                          (weight_r * _samples[rsampleindex]);
//
//             auto test = frame_index * _sndinfo.channels +
//             current_channel; splice[test] = val;
//         }
//     }
//
//     int32_t real_index;
//     for (size_t i = 0; i < sample_count; i++) {
//         real_index = leftSample + i;
//         if (real_index < 0)
//             continue;
//         else if (real_index >= global_sample_count)
//             break;
//
//         //_samples[real_index] = (splice[i] != -2.0) ? splice[i] : 0;
//         _samples[real_index] = splice[i];
//     }
//
//     return;
// }
