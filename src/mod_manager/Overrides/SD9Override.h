#include <iostream>
#include "audio/SD9File.h"

#include "mixins/Serialisable.h"
#include "mod_manager/Mod.h"

constexpr auto SD9_TYPE = "SD9";

struct SD9Replacement {
    fs::path path = "";
    SoundFile* audio = nullptr;
    SD9Info info{};
};

class SD9Override : public Override {
   public:
    explicit SD9Override(fs::path in);
    SD9Override(fs::path in, SD9Info info);

    std::string serialiseData() override;
    std::string getType() override { return SD9_TYPE; };

    [[nodiscard]] fs::path getReplacementFilepath() const {
        return _replacementData.path;
    };

    void setReplacementFilepath(const fs::path& path) {
        if (!fs::exists(path)) {
            std::cerr << "Replacement file " + path.string() +
                             " does not exist on filesystem, or is otherwise "
                             "invalid.";
            return;
        }

        try {
            auto* new_replacement = new SoundFile(path);

            delete _replacementData.audio;

            _replacementData.path = path;
            _replacementData.audio = new_replacement;
        } catch (std::exception& e) {
            std::cerr << "Unable to set replacement file.\nError details: "
                      << e.what() << std::endl;
        }
    }

    void process(fs::path out_path) override;

   private:
    SD9Replacement _replacementData{};
};
