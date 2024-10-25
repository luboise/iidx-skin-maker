namespace FileHandler {
    fs::path Backup(const fs::path& from);
    bool Write(const std::string&, const fs::path&);
    bool Delete(const fs::path&);
    std::string Read(const fs::path&);

};  // namespace FileHandler
