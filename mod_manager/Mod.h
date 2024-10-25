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

    int32_t game_version = 31;

    int32_t version_major = 1;
    int32_t version_minor = 0;

    fs::path root_dir = "";

    std::vector<Override> overrides{};

    [[nodiscard]] std::string serialise() const;
    static Mod deserialise(std::string);
};
