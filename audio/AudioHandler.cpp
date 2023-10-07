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
	// PCMBufferManager *bm = new PCMBufferManager(wavData, dataSize);

	return;

	// Create the stream, and put the wav in it (last arg)
	// err = Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, 44100, 512,
	// 						   &AudioHandler::audioCallback, bm);

	// if (err != paNoError) {
	// 	return;
	// }

	// err = Pa_StartStream(stream);
	// if (err != paNoError) {
	// 	throw std::logic_error("Bad stream received.");
	// }
}

void AudioHandler::TestSound() {}

int AudioHandler::audioCallback(const void *inputBuffer, void *outputBuffer,
								unsigned long framesPerBuffer,
								const PaStreamCallbackTimeInfo *timeInfo,
								PaStreamCallbackFlags statusFlags,
								void *userData) {
	// Get the buffer manager
	PCMBufferManager &bm = *(PCMBufferManager *)userData;

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

// PCMBufferManager::PCMBufferManager(char *chars, unsigned long charCount) {
// 	// TODO: REMOVE
// 	auto FRAMES_PER_BUFFER = 256;

// 	this->bufferPosition = 0;
// 	this->frameCounter = 0;

// 	unsigned long charTracker = 0;

// 	while (charTracker < charCount) {
// 		// Create a new buffer
// 		AudioFrame *newBuffer = new AudioFrame[FRAMES_PER_BUFFER];

// 		// Copy the data across from the old buffer
// 		for (auto i = 0; i < FRAMES_PER_BUFFER; i++) {
// 			newBuffer[i].full = chars[charTracker++];

// 			if ((charTracker + i) >= charCount) {
// 				break;
// 			}

// 			buffers.push_back(newBuffer);
// 		}
// 	}
// }

AudioFrame &PCMBufferManager::GetFrame() {
	// TODO: REMOVE
	auto FRAMES_PER_BUFFER = 256;

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
	ifs.read(reinterpret_cast<char *>(&this->fmtChunk),
			 sizeof(this->fmtChunk) - sizeof(this->fmtChunk.coefficients) - 2);

	// Swap the blockAlign to big endian
	swap16<u16>(this->fmtChunk.blockAlign);

	// Initialise the coefficients
	auto x = this->fmtChunk.numCoefficients;
	short **coefs = new short *[2];
	for (int i = 0; i < 2; ++i) {
		coefs[i] = new short[x];
	}

	// Read the coefficients from the file
	for (auto i = 0; i < x; i++) {
		ifs.read(reinterpret_cast<char *>(&coefs[0][i]), sizeof(short));
		ifs.read((char *)&coefs[1][i], sizeof(short));
	}

	// Write them to the struct
	this->fmtChunk.coefficients = coefs;

	// Get the fact subchunk
	ifs.read(reinterpret_cast<char *>(&this->factChunk),
			 sizeof(this->factChunk));

	// Get data section header
	ifs.read(reinterpret_cast<char *>(&dataChunk),
			 sizeof(dataChunk.dataString) + sizeof(dataChunk.chunkSize));

	u32 blockSize = sizeof(StereoPreamble) +
					((fmtChunk.samplesPerBlock * fmtChunk.bitsPerSample)) / 8;

	for (auto i = 0; i < dataChunk.chunkSize; i += blockSize) {
		StereoPreamble sp;
		ifs.read(reinterpret_cast<char *>(&sp), sizeof(sp));
		Block *b = new Block(sp);

		u16 j;

		for (j = 0; j < fmtChunk.samplesPerBlock; j += 2) {
			AudioFrame *af = new AudioFrame;
			ifs.read(&af->full, sizeof(af->full));
			b->insertAudioFrame(af);
		}

		this->dataChunk.blocks.push_back(b);

		if (j > fmtChunk.samplesPerBlock) {
			throw std::logic_error(
				"A sample has been read that doesn't exist (j too high)");
		};
	}

	if (!ifs.eof()) {
		throw std::invalid_argument(
			"End of file not reached after ADPCM blocks imported.");
	}
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

void Block::insertAudioFrame(AudioFrame *af) {
	this->m_audioframes.push_back(af);
}

const vector<AudioFrame *> &Block::getAudioFrames() const {
	return this->m_audioframes;
}

PCMData::PCMData(const ADPCMData &adpcm) {
	this->riffChunk.dataSize = adpcm.riffChunk.dataSize;

	this->fmtChunk.chunkSize = adpcm.fmtChunk.chunkSize;
	this->fmtChunk.formatTag = 0x0001;
	this->fmtChunk.channels = adpcm.fmtChunk.channels;
	this->fmtChunk.samplesPerSecond = adpcm.fmtChunk.samplesPerBlock;
	this->fmtChunk.avrgBytesPerSecond = adpcm.fmtChunk.avrgBytesPerSecond;
	this->fmtChunk.blockAlign = adpcm.fmtChunk.blockAlign;

	this->bm = new PCMBufferManager(adpcm.dataChunk.blocks,
									adpcm.fmtChunk.samplesPerBlock, 1536);
}

PCMBufferManager::PCMBufferManager(const vector<Block *> &adpcmBlocks,
								   unsigned long samplesPerBlock,
								   unsigned long BufferSize) {
	for (const Block *bPtr : adpcmBlocks) {
		// Get
	}
}