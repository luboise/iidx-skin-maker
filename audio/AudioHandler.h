#pragma once

#include <portaudio.h>

#include <string>
#include <vector>

#include "../files/FileHandler.h"

#define STEREO_PREAMBLE_SIZE 14

using std::string;

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef char i8;
typedef short i16;

union AudioFrame {
	struct {
		int right : 4;	// 4 bits for the right half
		int left : 4;	// 4 bits for the left half
	} half;
	char full;	// 8-bit representation
};

struct PCMFrame {
	i16 leftSample;
	i16 rightSample;
};

struct StereoPreamble {
	i8 leftBlockPredictor;
	i8 rightBlockPredictor;
	i16 leftInitialDelta;
	i16 rightInitialDelta;
	i16 leftSample1;
	i16 rightSample1;
	i16 leftSample2;
	i16 rightSample2;
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

class AudioHandler {
   public:
	static bool Init();
	static void Terminate();

	static void PlaySound(char *wavFile, unsigned long dataSize);
	static void PlayPCM(PCMBufferManager &bm);
	static void TestSound();

   private:
	static PaStream *stream;

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
