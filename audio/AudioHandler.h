#pragma once

#include <portaudio.h>

#include <string>
#include <vector>

#include "../files/FileHandler.h"

#define SAMPLE_RATE (44100)
#define N_CHANNELS (2)
#define SAMPLES_PER_BUFFER (512)
#define FRAMES_PER_BUFFER (SAMPLES_PER_BUFFER / N_CHANNELS)
#define BITS_PER_SAMPLE 4

using std::string;

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef char i8;

union AudioFrame {
	struct {
		unsigned int left : 4;	 // 4 bits for the left half
		unsigned int right : 4;	 // 4 bits for the right half
	} half;
	char full;	// 8-bit representation
};

class AudioHandler {
   public:
	static bool Init();
	static void Terminate();

	static void PlaySound(char *wavFile, unsigned long dataSize);
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

class BufferManager {
   public:
	BufferManager(char *chars, unsigned long charCount);
	AudioFrame &GetFrame();

   private:
	unsigned long frameCounter;
	unsigned long bufferPosition;
	std::vector<AudioFrame *> buffers;
};

class ADPCMData {
   public:
	ADPCMData(fs::path filepath);

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
		short **coefficients;

	} fmtChunk;

	struct {
		u8 dataString[4] = {'d', 'a', 't', 'a'};
		u32 chunkSize;
	} dataChunk;
};