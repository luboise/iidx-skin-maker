#pragma once

#include <portaudio.h>

#include <string>
#include <vector>

#include "SD9File.h"

using std::string;

using u32 = unsigned int;
using u16 = unsigned short;
using u8 = unsigned char;
using i8 = char;
using i16 = short;

using VolumeType = double;

union AudioFrame {
    struct {
        int right : 4;  // 4 bits for the right half
        int left : 4;   // 4 bits for the left half
    } half;
    struct {
        unsigned int right : 4;  // 4 bits for the right half
        unsigned left : 4;       // 4 bits for the left half
    } uhalf;
    char full;  // 8-bit representation
};

struct PCMFrame {
    i16 leftSample;
    i16 rightSample;
};

struct StereoPreamble {
    u8 leftBlockPredictor;
    u8 rightBlockPredictor;
    i16 leftInitialDelta;
    i16 rightInitialDelta;
    i16 leftSample2;
    i16 rightSample2;
    i16 leftSample1;
    i16 rightSample1;
};

class Block {
   public:
    Block(StereoPreamble preamble);
    ~Block();

    StereoPreamble getPreamble() const;

    void insertAudioFrame(AudioFrame *af);
    const vector<AudioFrame *> &getAudioFrames() const;

   private:
    StereoPreamble m_preamble;
    vector<AudioFrame *> m_audioframes;
};

class ADPCMData {
    friend class PCMData;

   public:
    ADPCMData(fs::path filepath);

    const vector<Block *> getBlocks() const;

    short *getLCoefs() const;
    short *getRCoefs() const;
    size_t getCoefCount() const;

    u16 getSamplesPerBlock() const;

   private:
    i8 sd9bytes[SD9_HEADER_SIZE];
    struct {
        i8 riffstr[4] = {'R', 'I', 'F', 'F'};
        u32 dataSize;
        i8 formatString[4] = {'W', 'A', 'V', 'E'};
    } riffChunk;

    struct {
        u8 fmtString[4] = {'f', 'm', 't', ' '};
        u32 chunkSize;
        u16 formatTag;
        u16 channels;
        u32 samplesPerSecond;
        u32 avrgBytesPerSecond;

        // (BE) Size of a single frame, equal to (channels * bits per sample) /
        // 8, 1 byte for MS ADPCM
        u16 blockAlign;

        // (BE) 4 for MS ADPCM
        u16 bitsPerSample;

        // (LE) Extra data size (32 for MS ADPCM)
        u16 cbSize;

        // Min 32, Max 512
        u16 samplesPerBlock;

        // Almost always 7
        u16 numCoefficients;

        // Signed decoding coefficients, going L1, R1, L2, R2, etc etc
        short *l_coefs;
        short *r_coefs;

    } fmtChunk;

    struct {
        char factStr[4];
        u32 strSize;
        u32 numSamples;
    } factChunk;

    struct {
        u8 dataString[4] = {'d', 'a', 't', 'a'};
        u32 chunkSize;
        vector<Block *> blocks;
    } dataChunk;
};

class PCMBufferManager {
   public:
    PCMBufferManager(const ADPCMData &adpcm, size_t PCMBufferSize);
    PCMFrame &GetFrame();

   private:
    unsigned long frameCounter;
    unsigned long bufferPosition;
    std::vector<PCMFrame *> frames;
};

struct PlaybackSet {
    SD9File sd9;
    size_t next_frame = 0;
    size_t total_frames;
    double volume{1};

    // Leave as copy
    explicit PlaybackSet(const SD9File &sd9_file)
        : sd9(sd9_file), total_frames(sd9.getFrameCount()) {}

    [[nodiscard]] size_t getFinalFrame() const {
        auto sd9_info = this->sd9.getSD9Info();

        return sd9_info.isLooping()
                   ? sd9_info.loop_end_byte_offset / SD9_SAMPLE_SIZE
                   : this->total_frames;
    }
};

class AudioHandler {
   public:
    static bool Init();
    static void Terminate();

    static void PlaySD9(SD9File &);
    static void PlaySD9(const fs::path &);

    static void TestSound();

    static void Start();
    static void Stop();

    static void SetVolume(VolumeType new_volume) {
        auto normalised_volume = new_volume / 100;
        if (normalised_volume < 0 || normalised_volume > 100) {
            std::cerr << "Unable to set volume to new value " << new_volume
                      << ". Keeping old value of " << _volume << "."
                      << std::endl;
        }

        _volume = normalised_volume;
        if (_playbackSet) {
            _playbackSet->volume = normalised_volume;
        }
    }

    // static void PlayPCM(PCMBufferManager &bm);

   private:
    static PaStream *_stream;
    inline static VolumeType _volume{0};

    inline static std::unique_ptr<PlaybackSet> _playbackSet = nullptr;

    /* This routine will be called by the PortAudio engine when audio is needed.
     * It may called at interrupt level on some machines so don't do anything
     * that could mess up the system like calling malloc() or free().
     */

    static int audioCallback(const void *inputBuffer, void *outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo *timeInfo,
                             PaStreamCallbackFlags statusFlags, void *userData);
};

class PCMData {
   public:
    PCMData(const ADPCMData &adpcm);
    PCMBufferManager &getBufferManager();

   private:
    i8 sd9bytes[SD9_HEADER_SIZE];
    struct {
        i8 riffstr[4] = {'R', 'I', 'F', 'F'};
        u32 dataSize;
        i8 formatString[4] = {'W', 'A', 'V', 'E'};
    } riffChunk;

    struct {
        u8 fmtString[4] = {'f', 'm', 't', ' '};
        u32 chunkSize;
        u16 formatTag;
        u16 channels;
        u32 samplesPerSecond;
        u32 avrgBytesPerSecond;

        // (BE) Size of a single frame, equal to (channels * bits per sample) /
        // 8, 1 byte for MS ADPCM
        u16 blockAlign;

        // (BE) 16bit for PCM
        u16 bitsPerSample = 16;

        // (LE) Extra data size (0 when converting from ADPCM)
        u16 cbSize = 0;

    } fmtChunk;

    struct {
        char factStr[4];
        u32 strSize;
        u32 numSamples;
    } factChunk;

    struct {
        u8 dataString[4] = {'d', 'a', 't', 'a'};
        u32 chunkSize;
        vector<Block *> blocks;
    } dataChunk;

    PCMBufferManager *bm;
};
