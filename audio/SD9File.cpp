#include "SD9File.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

#include "SoundFile.h"

using std::ifstream;

SD9File::SD9File(ifstream& ifs) {
	if (!ifs.is_open()) {
		std::stringstream ss;
		ss << "Input file stream could not be opened ";

		throw std::runtime_error(ss.str());
	}

	// Get size of file
	ifs.seekg(0, std::ios::end);
	const size_t size = ifs.tellg();

	// Allocate memory for the data excluding the SD9 header
	char* data = new char[size - 32 + 1];

	// Read SD9 header
	ifs.seekg(0, std::ios::beg);
	ifs.read(_sd9Header.data(), 32);

	ifs.read(data, size);
	ifs.close();

	_soundFile = new SoundFile(data, size);
};

SD9File::SD9File(const char* filename) {
	ifstream ifs(filename);

	if (!ifs.is_open()) {
		std::stringstream ss;
		ss << "Input file stream could not be opened for " << filename;

		throw std::runtime_error(ss.str());
	}

	// Get size of file
	ifs.seekg(0, std::ios::end);
	const size_t size = ifs.tellg();

	// Allocate memory for the data excluding the SD9 header
	char* data = new char[size - 32 + 1];

	// Read SD9 header
	ifs.seekg(size, std::ios::beg);
	ifs.read(_sd9Header.data(), 32);

	ifs.read(data, size);

	_soundFile = new SoundFile(data);
}

SD9File::~SD9File() { delete _soundFile; }
