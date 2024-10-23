#pragma once

#include <array>

#include "SoundFile.h"

#define SD9_HEADER_SIZE 32

class SD9File {
   public:
	SD9File(const char* filename);
	SD9File(std::ifstream&);

	const SoundFile* getSoundFile() const { return _soundFile; }

	~SD9File();

   private:
	std::array<char, 32> _sd9Header;
	SoundFile* _soundFile;
};
