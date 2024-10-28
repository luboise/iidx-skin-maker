#include <iostream>
#include "audio/SD9File.h"

#include "mixins/Serialisable.h"
#include "mod_manager/Mod.h"

class SD9Override : public Override {
   public:
    explicit SD9Override(fs::path in);
    SD9Override(fs::path in, SD9Info info);

    std::string serialiseData() override;
    static std::string getType() { return "SD9"; };

    [[nodiscard]] fs::path getReplacementFile() const {
        return _replacement_file;
    };

    void setReplacementFile(const fs::path& path) {
        if (!fs::exists(path)) {
            std::cerr << "Replacement file " + path.string() +
                             " does not exist on filesystem, or is otherwise "
                             "invalid.";

            return;
        }
        _replacement_file = path;
    }

    void process(fs::path out_path) override {}

   private:
    fs::path _replacement_file;
    SD9Info _info;
};
