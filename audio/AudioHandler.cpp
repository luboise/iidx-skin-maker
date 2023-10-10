#include "AudioHandler.h"

#include <fstream>
#include <stdexcept>

// TODO: REMOVE LATER DEBUG ONLY
#include <iostream>
#include <sstream>

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

void AudioHandler::PlayPCM(PCMBufferManager &bm) {
	PaStream *stream;

	PaError err;

	err = Pa_OpenDefaultStream(&stream, 0, 2, paInt16, 44100, 512,
							   &AudioHandler::audioCallback, &bm);

	if (err != paNoError) {
		throw std::logic_error("Unable to initialise audio stream.");
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
	PCMBufferManager &bm = *(PCMBufferManager *)userData;

	// Read the output buffer
	i16 *out = (i16 *)outputBuffer;

	(void)inputBuffer; /* Prevent unused variable warning. */

	for (unsigned i = 0; i < framesPerBuffer; i++) {
		const PCMFrame &frame = bm.GetFrame();

		*out++ = frame.leftSample;
		*out++ = frame.rightSample; /* right */

		// std::cout << "(" << frame.leftSample << "," << frame.rightSample <<
		// ")"
		// 		  << std::endl;
	}
	return paContinue;
};

PCMFrame &PCMBufferManager::GetFrame() {
	// TODO: REMOVE
	// auto FRAMES_PER_BUFFER = 256;

	// PCMFrame &returnVal = frames[this->frameCounter][this->bufferPosition++];

	// if (bufferPosition >= FRAMES_PER_BUFFER) {
	// 	bufferPosition = 0;
	// 	this->frameCounter++;
	// }
	// return returnVal;

	return *this->frames[this->frameCounter++];

	// PCMFrame &returnVal = frames[this->frameCounter][this->bufferPosition++];

	// if (bufferPosition >= FRAMES_PER_BUFFER) {
	// 	bufferPosition = 0;
	// 	this->frameCounter++;
	// }
	// return returnVal;
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
			 sizeof(this->fmtChunk) - 2 * sizeof(this->fmtChunk.l_coefs) - 2);

	// Swap the blockAlign to big endian
	// swap16<u16>(this->fmtChunk.blockAlign);

	// Initialise the coefficients
	auto x = this->fmtChunk.numCoefficients;

	short *l_coefs = new short[x];
	short *r_coefs = new short[x];

	// Read the coefficients from the file
	for (auto i = 0; i < x; i++) {
		ifs.read(reinterpret_cast<char *>(&l_coefs[i]), sizeof(short));
		ifs.read(reinterpret_cast<char *>(&r_coefs[i]), sizeof(short));
	}

	// Write them to the struct
	this->fmtChunk.l_coefs = l_coefs;
	this->fmtChunk.r_coefs = r_coefs;

	// Get the fact subchunk
	ifs.read(reinterpret_cast<char *>(&this->factChunk),
			 sizeof(this->factChunk));

	// Get data section header
	ifs.read(reinterpret_cast<char *>(&dataChunk),
			 sizeof(dataChunk.dataString) + sizeof(dataChunk.chunkSize));

	// u32 blockSize = sizeof(StereoPreamble) +
	// 				((fmtChunk.samplesPerBlock * fmtChunk.bitsPerSample)) / 8;
	u32 blockSize = fmtChunk.blockAlign;

	for (auto i = 0; i < dataChunk.chunkSize; i += blockSize) {
		try {
			StereoPreamble sp;
			ifs.read(reinterpret_cast<char *>(&sp), sizeof(sp));

			// swap16<i16>(sp.leftInitialDelta);
			// swap16<i16>(sp.leftInitialDelta);

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

const vector<Block *> ADPCMData::getBlocks() const {
	return this->dataChunk.blocks;
}

short *ADPCMData::getLCoefs() const { return this->fmtChunk.l_coefs; }
short *ADPCMData::getRCoefs() const { return this->fmtChunk.r_coefs; }

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

	this->bm = new PCMBufferManager(adpcm, 1536);
}

PCMBufferManager &PCMData::getBufferManager() { return *this->bm; }

int AdaptationTable[] = {230, 230, 230, 230, 307, 409, 512, 614,
						 768, 614, 512, 409, 307, 230, 230, 230};

PCMBufferManager::PCMBufferManager(const ADPCMData &adpcm,
								   size_t PCMBufferSize) {
	this->bufferPosition = 0;

	u16 samplesPerBlock = adpcm.getSamplesPerBlock();

	const vector<Block *> &adpcmBlocks = adpcm.getBlocks();

	const size_t num_coefficients = adpcm.getCoefCount();
	const auto l_coefs = adpcm.getLCoefs();
	const auto r_coefs = adpcm.getRCoefs();

	for (const Block *bPtr : adpcmBlocks) {
		auto &b = *bPtr;

		auto preamble = b.getPreamble();

		struct {
			i16 sample1;
			i16 sample2;
		} l_samples, r_samples;

		l_samples.sample1 = preamble.leftSample1;
		l_samples.sample2 = preamble.leftSample2;

		r_samples.sample1 = 0;
		r_samples.sample2 = 0;

		PCMFrame frame;
		frame = {l_samples.sample1, r_samples.sample1};
		frames.push_back(new PCMFrame(frame));

		frame = {l_samples.sample2, r_samples.sample2};

		frames.push_back(new PCMFrame(frame));

		auto l_delta = preamble.leftInitialDelta;
		auto r_delta = preamble.rightInitialDelta;

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
		for (auto i = 0; i < samplesPerBlock / 2; i++) {
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

			l_delta = AdaptationTable[l_nibble] * l_delta / 256;
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