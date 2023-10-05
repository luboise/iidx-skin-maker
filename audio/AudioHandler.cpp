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

void AudioHandler::PlaySound(char *wavData, unsigned long dataSize) {
	PaStream *stream;

	PaError err;

	// Parse the wav file into relevant information
	BufferManager *bm = new BufferManager(wavData, dataSize);

	// Create the stream, and put the wav in it (last arg)
	err = Pa_OpenDefaultStream(&stream, 0, N_CHANNELS, paFloat32, 44100,
							   FRAMES_PER_BUFFER, &AudioHandler::audioCallback,
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
	float *out = (float *)outputBuffer;

	(void)inputBuffer; /* Prevent unused variable warning. */

	for (unsigned i = 0; i < framesPerBuffer; i++) {
		AudioFrame frame = bm.GetFrame();

		*out++ = (frame.half.left / 15.0f) * 2 - 1;	 /* left */
		*out++ = (frame.half.right / 15.0f) * 2 - 1; /* right */
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
			newBuffer[i].full = chars[charTracker++];

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
