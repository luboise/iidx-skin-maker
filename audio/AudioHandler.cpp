#include "AudioHandler.h"

#include <stdexcept>

bool AudioHandler::Init() {
	auto err = Pa_Initialize();

	if (err != paNoError) {
		return false;
	}

	return (err == paNoError);
}

void AudioHandler::Terminate() { Pa_Terminate(); }

void AudioHandler::PlaySound(char *wavData) {
	PaStream *stream;

	PaError err;

	// Parse the wav file into relevant information
	BufferManager *bm = new BufferManager(wavData, 149252);

	// Create the stream, and put the wav in it (last arg)
	err = Pa_OpenDefaultStream(&stream, 0, N_CHANNELS, paInt8, 44100,
							   SAMPLES_PER_BUFFER, &AudioHandler::audioCallback,
							   bm);

	if (err != paNoError) {
		return;
	}

	err = Pa_StartStream(stream);
	if (err != paNoError) {
		throw std::logic_error("Bad stream received.");
	}
}

void AudioHandler::TestSound() {}

int AudioHandler::audioCallback(const void *inputBuffer, void *outputBuffer,
								unsigned long framesPerBuffer,
								const PaStreamCallbackTimeInfo *timeInfo,
								PaStreamCallbackFlags statusFlags,
								void *userData) {
	// Get the buffer manager
	BufferManager &bm = *(BufferManager *)userData;

	// Read the output buffer
	char *out = (char *)outputBuffer;

	(void)inputBuffer; /* Prevent unused variable warning. */

	for (unsigned i = 0; i < framesPerBuffer; i++) {
		AudioFrame frame = bm.GetFrame();

		*out++ = frame.left;  /* left */
		*out++ = frame.right; /* right */
	}
	return paContinue;
};

BufferManager::BufferManager(char *chars, unsigned long charCount) {
	this->bufferPosition = 0;
	this->frameCounter = 0;

	unsigned long charTracker = 0;

	while (charTracker < charCount) {
		// Create a new buffer
		AudioFrame *newBuffer = new AudioFrame[FRAMES_PER_BUFFER];

		// Copy the data across from the old buffer
		for (auto i = 0; i < FRAMES_PER_BUFFER; i++) {
			newBuffer[i].left = chars[charTracker++];
			if (charTracker >= charCount) {
				newBuffer[i].right = 0;
			} else {
				newBuffer[i].right = chars[charTracker++];
			}

			if ((charTracker + i) >= charCount) {
				break;
			}

			buffers.push_back(newBuffer);
		}
	}
}

AudioFrame &BufferManager::GetFrame() {
	AudioFrame &returnVal = buffers[this->frameCounter][this->bufferPosition++];

	if (bufferPosition >= FRAMES_PER_BUFFER) {
		bufferPosition = 0;
		this->frameCounter++;
	}
	return returnVal;
}
