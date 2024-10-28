#include "AudioHandler.h"

#include <portaudio.h>
#include <iostream>
#include <memory>

#include "SD9File.h"

using std::ifstream;

PaStream* AudioHandler::_stream = nullptr;

bool AudioHandler::Init() {
    auto err = Pa_Initialize();

    if (err != paNoError) {
        return false;
    }

    return (err == paNoError);
}

void AudioHandler::Terminate() { Pa_Terminate(); }

int SD9Callback(const void* input, void* out_buffer, unsigned long frame_count,
                const PaStreamCallbackTimeInfo* timeInfo,
                PaStreamCallbackFlags statusFlags, void* userData) {
    auto* playback_set = static_cast<PlaybackSet*>(userData);
    auto& sound_file = playback_set->sd9;

    // Read the output buffer
    auto* out = static_cast<float*>(out_buffer);

    for (unsigned i = 0; i < frame_count; i++) {
        size_t frame_index = playback_set->next_frame + i;
        if (frame_index >= playback_set->total_frames) {
            SD9Info info = playback_set->sd9.getSD9Info();

            if (info.loop_enabled != 0) {
                playback_set->next_frame =
                    info.loop_start / sound_file.getChannelCount();
                return paContinue;
            }
            return paComplete;
        }

        const auto& frame = sound_file.getFrame(frame_index);

        for (const auto& sample : frame) {
            *(out++) = sample;
        }
    }

    playback_set->next_frame += frame_count;
    return paContinue;
};

void AudioHandler::PlaySD9(const fs::path& path) {
    if (!fs::exists(path)) {
        throw std::runtime_error(
            "Path " + path.string() +
            " does not exist on filesystem, or is otherwise invalid.");
    }

    ifstream ifs(path);
    try {
        auto audio_file = SD9File(ifs);
        AudioHandler::PlaySD9(audio_file);
    } catch (std::exception& e) {
        std::cerr << "Unable to play SD9 file: " << e.what() << std::endl;
    }
};

void AudioHandler::PlaySD9(SD9File& sd9) {
    PaError err = 0;

    AudioHandler::Stop();

    _playbackSet = std::make_unique<PlaybackSet>(PlaybackSet(sd9));

    err = Pa_OpenDefaultStream(
        &_stream, 0, 2, paFloat32, sd9.getSoundInfo().samplerate,
        paFramesPerBufferUnspecified, &SD9Callback, _playbackSet.get());

    if (err != paNoError) {
        throw std::logic_error("Unable to initialise audio stream.");
    }

    AudioHandler::Start();
};

void AudioHandler::TestSound() {}

void AudioHandler::Start() {
    if (_stream == nullptr || _playbackSet == nullptr) {
        std::cerr
            << "Unable to start audio. No stream is available to play from. "
            << std::endl;
        return;
    }

    PaError err = Pa_StartStream(_stream);
    if (err != paNoError) {
        throw std::logic_error("Bad stream received.");
    }
}

void AudioHandler::Stop() {
    if (AudioHandler::_stream != nullptr) {
        if (Pa_StopStream(_stream) != paNoError) {
            throw std::runtime_error("Failed to stop existing stream.");
        }

        if (Pa_CloseStream(_stream) != paNoError) {
            throw std::runtime_error("Failed to stop existing stream.");
        }
        _stream = nullptr;
    }

    _playbackSet = nullptr;
};

/*
int AudioHandler::audioCallback(const void* inputBuffer, void* outputBuffer,
                                unsigned long framesPerBuffer,
                                const PaStreamCallbackTimeInfo* timeInfo,
                                PaStreamCallbackFlags statusFlags,
                                void* userData) {
    // Get the buffer manager
    PCMBufferManager& bm = *(PCMBufferManager*)userData;

    // Read the output buffer
    i16* out = (i16*)outputBuffer;

    (void)inputBuffer;
    // Prevent unused variable warning.

    for (unsigned i = 0; i < framesPerBuffer; i++) {
        const PCMFrame& frame = bm.GetFrame();

        *out++ = frame.leftSample;
        *out++ = frame.rightSample;

        // std::cout << "(" << frame.leftSample << "," << frame.rightSample
        // <<
        // ")"
        // 		  << std::endl;
    }
    return paContinue;
};

template <typename T>
T& swap16(T& block) {
    block = block << 8 | block >> 8;
    return block;
}

ADPCMData::ADPCMData(fs::path filepath) {
    size_t numBytes;

    ifstream ifs(filepath, std::ios::binary);

    // Get the sd9 chunk
    ifs.read(this->sd9bytes, SD9_HEADER_SIZE);

    // Get the RIFF chunk
    ifs.read(reinterpret_cast<char*>(&this->riffChunk),
             sizeof(this->riffChunk));

    // Get the first subchunk
    ifs.read(reinterpret_cast<char*>(&this->fmtChunk),
             sizeof(this->fmtChunk) - 2 * sizeof(this->fmtChunk.l_coefs) - 2);

    // Initialise the coefficients
    auto x = this->fmtChunk.numCoefficients;

    short* l_coefs = new short[x];
    short* r_coefs = new short[x];

    // Read the coefficients from the file
    for (auto i = 0; i < x; i++) {
        ifs.read(reinterpret_cast<char*>(&l_coefs[i]), sizeof(short));
        ifs.read(reinterpret_cast<char*>(&r_coefs[i]), sizeof(short));
    }

    // Write them to the struct
    this->fmtChunk.l_coefs = l_coefs;
    this->fmtChunk.r_coefs = r_coefs;

    // Get the fact subchunk
    ifs.read(reinterpret_cast<char*>(&this->factChunk),
             sizeof(this->factChunk));

    // Get data section header
    ifs.read(reinterpret_cast<char*>(&dataChunk),
             sizeof(dataChunk.dataString) + sizeof(dataChunk.chunkSize));

    // u32 blockSize = sizeof(StereoPreamble) +
    // 				((fmtChunk.samplesPerBlock*
    // fmtChunk.bitsPerSample)) / 8;
    u32 blockSize = fmtChunk.blockAlign;

    for (auto i = 0; i < dataChunk.chunkSize; i += blockSize) {
        try {
            StereoPreamble sp;
            ifs.read(reinterpret_cast<char*>(&sp), sizeof(sp));

            // swap16<i16>(sp.leftInitialDelta);
            // swap16<i16>(sp.rightInitialDelta);

            Block* b = new Block(sp);

            u16 j;

            // Exclude the 2 samples in the header
            for (j = 0; j < fmtChunk.samplesPerBlock - 2; j++) {
                AudioFrame* af = new AudioFrame;
                ifs.read(&af->full, sizeof(af->full));
                b->insertAudioFrame(af);
            }

            this->dataChunk.blocks.push_back(b);

            if (j > fmtChunk.samplesPerBlock) {
                throw std::logic_error(
                    "A sample has been read that doesn't exist (j too "
                    "high)");
            }
        } catch (std::exception e) {
            std::cout << "Exception caught: " << e.what() << std::endl;
            return;
        }
    }

    // TODO: UNCOMMENT WHEN DONE DEBUGGING
    // if (!ifs.eof()) {
    // 	throw std::invalid_argument(
    // 		"End of file not reached after ADPCM blocks imported.");
    // }
}

const vector<Block*> ADPCMData::getBlocks() const {
    return this->dataChunk.blocks;
}

short* ADPCMData::getLCoefs() const { return this->fmtChunk.l_coefs; }
short* ADPCMData::getRCoefs() const { return this->fmtChunk.r_coefs; }

size_t ADPCMData::getCoefCount() const {
    return this->fmtChunk.numCoefficients;
}

u16 ADPCMData::getSamplesPerBlock() const {
    return this->fmtChunk.samplesPerBlock;
}

Block::Block(StereoPreamble preamble) { this->m_preamble = preamble; }

Block::~Block() {
    // Delete the audioframes
    for (auto x : m_audioframes) {
        if (x != nullptr) {
            delete x;
        }
    }
}

StereoPreamble Block::getPreamble() const { return this->m_preamble; }

void Block::insertAudioFrame(AudioFrame* af) {
    this->m_audioframes.push_back(af);
}

const vector<AudioFrame*>& Block::getAudioFrames() const {
    return this->m_audioframes;
}

PCMData::PCMData(const ADPCMData& adpcm) {
    this->riffChunk.dataSize = adpcm.riffChunk.dataSize;

    this->fmtChunk.chunkSize = adpcm.fmtChunk.chunkSize;
    this->fmtChunk.formatTag = 0x0001;
    this->fmtChunk.channels = adpcm.fmtChunk.channels;
    this->fmtChunk.samplesPerSecond = adpcm.fmtChunk.samplesPerBlock;
    this->fmtChunk.avrgBytesPerSecond = adpcm.fmtChunk.avrgBytesPerSecond;
    this->fmtChunk.blockAlign = adpcm.fmtChunk.blockAlign;

    this->bm = new PCMBufferManager(adpcm, 1536);
}

PCMBufferManager& PCMData::getBufferManager() { return *this->bm; }

const int AdaptationTable[] = {230, 230, 230, 230, 307, 409, 512, 614,
                               768, 614, 512, 409, 307, 230, 230, 230};

PCMBufferManager::PCMBufferManager(const ADPCMData& adpcm,
                                   size_t PCMBufferSize) {
    this->bufferPosition = 0;

    u16 samplesPerBlock = adpcm.getSamplesPerBlock();

    const vector<Block*>& adpcmBlocks = adpcm.getBlocks();

    const size_t num_coefficients = adpcm.getCoefCount();
    const auto l_coefs = adpcm.getLCoefs();
    const auto r_coefs = adpcm.getRCoefs();

    for (const Block* bPtr : adpcmBlocks) {
        auto& b = *bPtr;

        auto preamble = b.getPreamble();

        struct {
            i16 sample1;
            i16 sample2;
        } l_samples, r_samples;

        // TODO: UNCOMMENT THIS AND FIX IT IN ADPCM INSTEAD
        // l_samples.sample1 = preamble.leftSample1;
        // l_samples.sample2 = preamble.leftSample2;

        l_samples.sample1 = preamble.leftSample2;
        l_samples.sample2 = preamble.leftSample1;

        r_samples.sample1 = 0;
        r_samples.sample2 = 0;

        PCMFrame frame;
        frame = {l_samples.sample1, r_samples.sample1};
        frames.push_back(new PCMFrame(frame));

        frame = {l_samples.sample2, r_samples.sample2};

        frames.push_back(new PCMFrame(frame));

        auto l_delta = preamble.leftInitialDelta;
        // auto r_delta = preamble.rightInitialDelta;

        if (preamble.leftBlockPredictor >= num_coefficients ||
            preamble.rightBlockPredictor >= num_coefficients) {
            std::stringstream ss;

            ss << "Invalid indices received: " << preamble.leftBlockPredictor
               << "|" << preamble.rightBlockPredictor << " (max of "
               << num_coefficients << " applies to both)";
            throw std::invalid_argument(ss.str());
        }

        int l_coef1 = l_coefs[preamble.leftBlockPredictor];
        int l_coef2 = r_coefs[preamble.leftBlockPredictor];

        // For each left and right nibble
        for (auto i = 0; i < (samplesPerBlock - 2); i++) {
            int l_nibble = b.getAudioFrames().at(i)->half.left;

            int l_predictor = ((l_samples.sample1 * l_coef1) +
                               (l_samples.sample2 * l_coef2)) /
                              256;

            l_predictor += l_nibble * l_delta;

            i16 pcm_val = l_predictor;

            std::cout << pcm_val << ",";

            frame = {pcm_val, 0};

            frames.push_back(new PCMFrame(frame));

            l_samples.sample2 = l_samples.sample1;
            l_samples.sample1 = pcm_val;

            l_delta = AdaptationTable[b.getAudioFrames().at(i)->uhalf.left] *
                      (l_delta / 256);
            if (l_delta < -16) {
                l_delta = 16;
            } else if (l_delta > 16) {
                l_delta = 16;
            }
        }

        std::cout << std::endl;
    }

    this->frameCounter = 0;
    this->bufferPosition = 0;
}

void AudioHandler::PlayPCM(PCMBufferManager& bm) {
    PaStream* stream;

    PaError err;

    err = Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, 44100, 512,
                               &AudioHandler::audioCallback, &bm);

    if (err != paNoError) {
        throw std::logic_error("Unable to initialise audio stream.");
    }

    err = Pa_StartStream(stream);
    if (err != paNoError) {
        throw std::logic_error("Bad stream received.");
    }
}

PCMFrame& PCMBufferManager::GetFrame() {
    // TODO: REMOVE
    // auto FRAMES_PER_BUFFER = 256;

    // PCMFrame &returnVal =
    // frames[this->frameCounter][this->bufferPosition++];

    // if (bufferPosition >= FRAMES_PER_BUFFER) {
    // 	bufferPosition = 0;
    // 	this->frameCounter++;
    // }
    // return returnVal;

    return *this->frames[this->frameCounter++];

    // PCMFrame &returnVal =
    // frames[this->frameCounter][this->bufferPosition++];

    // if (bufferPosition >= FRAMES_PER_BUFFER) {
    // 	bufferPosition = 0;
    // 	this->frameCounter++;
    // }
    // return returnVal;
}

*/
