#include "AudioHandler.h"

#include <fstream>
#include <stdexcept>

using std::ifstream;

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

template <typename T>
T &swap16(T &block) {
	block = block << 8 | block >> 8;
	return block;
}

ADPCMData::ADPCMData(fs::path filepath) {
	size_t numBytes;

	ifstream ifs(filepath, std::ios::binary);

	// Get the sd9 chunk
	ifs.read(this->sd9bytes, SD9_HEADER_SIZE);

	// Get the RIFF chunk
	ifs.read(reinterpret_cast<char *>(&this->riffChunk),
			 sizeof(this->riffChunk));

	// Get the first subchunk
	ifs.read(reinterpret_cast<char *>(&this->fmtChunk), sizeof(this->fmtChunk));

	swap16<u16>(this->fmtChunk.blockAlign);

	auto x = this->fmtChunk.numCoefficients;

	short **coefs = new short *[2];
	for (int i = 0; i < 2; ++i) {
		coefs[i] = new short[x];
	}

	for (auto i = 0; i < x; i++) {
		ifs.read(reinterpret_cast<char *>(&coefs[0][i]), sizeof(short));
		swap16<short>(coefs[0][i]);
		ifs.read((char *)&coefs[1][i], sizeof(short));
		swap16(coefs[1][i]);
	}

	bool ni = true;
}
