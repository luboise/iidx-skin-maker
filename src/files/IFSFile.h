constexpr uint8_t XML_SIGNATURE = 0xA0;

constexpr uint8_t XML_COMPRESSED = 0x42;
constexpr uint8_t XML_UNCOMPRESSED = 0x45;

enum class EncodingKeys : uint8_t {
    ASCII = 0x20,
    ISO_8859_1 = 0x40,
    EUC_JP = 0x60,
    CP932 = 0x80,
    UTF_8 = 0xA0,
};

constexpr auto DEFAULT_ENCODING_KEY = EncodingKeys::CP932;

struct IFSFile {
    uint8_t xmlSignature{XML_SIGNATURE};
    uint8_t isCompressed{0};
    uint8_t encodingKey{0};
    uint8_t inverseEncodingKey{0};
};
