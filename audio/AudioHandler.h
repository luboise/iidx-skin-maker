#include <portaudio.h>

#include <vector>

#define SAMPLE_RATE (44100)
#define N_CHANNELS (2)
#define SAMPLES_PER_BUFFER (512)
#define FRAMES_PER_BUFFER (SAMPLES_PER_BUFFER / N_CHANNELS)
#define BITS_PER_SAMPLE 4

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