#include "audio/SD9File.h"

#include "mixins/Serialisable.h"
#include "mod_manager/Mod.h"

class SD9Override : public Override {
   public:
    explicit SD9Override(fs::path in, fs::path replacement_file, SD9Info info);

    std::string serialiseData() override;
    static std::string getType() { return "SD9"; };

    void process(fs::path out_path) override {}

   private:
    fs::path _replacement_file;
    SD9Info _info;
};
