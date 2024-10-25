#pragma once

#include <sndfile.h>

#include <cmath>
#include <complex>
#include <string>
#include <vector>

using SampleType = float;
using SampleList = std::vector<SampleType>;

using FFT_T = std::complex<SampleType>;
using FFTBinList = std::vector<FFT_T>;

struct StreamDetails {
    std::istream* stream;
    long start = 0;
};

class SoundFile {
   public:
    SoundFile(const std::string& filepath);

    SoundFile(char* data, uint32_t size);

    SoundFile(std::istream&);
    ~SoundFile();

    void exportToFile(const std::string filename);

    [[nodiscard]] bool isValid() const;
    [[nodiscard]] double getSoundLength() const;

    [[nodiscard]] SampleList getStretched(double stretchFactor) const;
    [[nodiscard]] SampleList getStretched(double stretchFactor,
                                          int32_t startFrame,
                                          int32_t endFrame) const;
    std::vector<SampleType> getFrame(int32_t frameIndex) const;
    // SampleList getSamples() const;
    [[nodiscard]] SampleList getChannel(uint8_t channel) const;

    [[nodiscard]] size_t getChannelCount() const;
    [[nodiscard]] size_t getSampleCount() const;

    [[nodiscard]] size_t getFrameCount() const { return _channels[0].size(); };

    void setSamples(const SampleList frames);
    void setChannel(const size_t channel, const size_t offset,
                    const SampleList samples);

    void changeVolume(const double newVolume);
    void normalise();

    [[nodiscard]] const SF_INFO& getSoundInfo() const { return _sndinfo; }

   private:
    std::vector<SampleList> _channels;
    SF_INFO _sndinfo;

    void initialiseData(SNDFILE&, SF_INFO&);

    // void swingFrames(const int32_t leftFrame, const int32_t rightFrame);
    // void swingFrames(const int32_t leftFrame, const int32_t rightFrame,
    // bool removeSwing);

    [[nodiscard]] std::vector<SampleList> getWindows(
        const SampleList& samples, const size_t windowSize,
        const double hopSize) const;

    [[nodiscard]] std::vector<FFTBinList> getWindowBins(
        std::vector<SampleList> windows) const;

    static double getNewBeatPosition(double beat, double ratio,
                                     bool removingSwing);

    void resizeChannels();
};
