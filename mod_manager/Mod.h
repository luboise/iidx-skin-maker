#pragma once

/**
 * An interface for anything intended to override an original game file. For
 * example, this could be extended to have an AudioOverride and a VideoOverride.
 */
class Override {
   public:
	// Abstract function that processes the mod
	virtual void process(std::string out_root) = 0;

	virtual ~Override();

   private:
	// The path of the file to be overridden
	std::string _in;
};

struct Mod {
	std::string name = "My Mod";

	uint32_t version_major = 1;
	uint32_t version_minor = 0;

	fs::path root_dir = "";

	std::vector<Override> overrides{};
};
