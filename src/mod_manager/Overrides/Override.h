constexpr char OVERRIDE_SEP_CHAR = '|';

struct ProcessData {
    fs::path in_root = "";
    fs::path out_root = "";
};

/**
 * An interface for anything intended to override an original game file. For
 * example, this could be extended to have an AudioOverride and a VideoOverride.
 */
class Override {
   public:
    explicit Override(fs::path proximate_path)
        : _proximatePath(std::move(proximate_path)) {}
    virtual ~Override() = default;

    // Abstract function that processes the mod
    virtual void process(const ProcessData&) = 0;

    std::string serialise() {
        std::stringstream ss;
        ss << getType() << OVERRIDE_SEP_CHAR << _proximatePath.string()
           << OVERRIDE_SEP_CHAR << this->serialiseData();

        return ss.str();
    };

    virtual std::string serialiseData() = 0;

    [[nodiscard]] fs::path proximatePath() const { return _proximatePath; }

    virtual std::string getType() = 0;

   protected:
    // The path of the file to be overridden
    fs::path _proximatePath = "";

    std::string _out = "";
};
