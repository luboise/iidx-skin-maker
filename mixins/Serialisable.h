template <typename T>

class Serialisable {
    static T deserialise(const std::string&);
    virtual std::string serialise() = 0;
};
