constexpr char OVERRIDE_SEP_CHAR = '|';

/**
 * An interface for anything intended to override an original game file. For
 * example, this could be extended to have an AudioOverride and a VideoOverride.
 */
class Override {
   public:
    explicit Override(fs::path in_path) : _in(std::move(in_path)) {}
    virtual ~Override() = default;

    // Abstract function that processes the mod
    virtual void process(fs::path out_path) = 0;

    std::string serialise() {
        std::stringstream ss;
        ss << getType() << OVERRIDE_SEP_CHAR << _in.string()
           << OVERRIDE_SEP_CHAR << this->serialiseData();

        return ss.str();
    };

    virtual std::string serialiseData() = 0;

    static std::string getType() { return "UNIMPLEMENTED"; };

   protected:
    // The path of the file to be overridden
    fs::path _in;

    std::string _out;
};
