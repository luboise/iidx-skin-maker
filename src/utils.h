#pragma once

namespace Utils {

    fs::path filePopup(const wxString& text, const wxString& filetype);
    fs::path directoryPopup(const wxString& text);
    fs::path saveFilePopup(const wxString& extension);
    bool booleanPopup(const wxString& message,
                      const wxString& true_option = "Yes",
                      const wxString& false_option = "No");

    namespace File {
        bool IsSupported(const fs::path&);
    }

    namespace Dialog {
        void Error(const wxString& message);
    }

    namespace String {
        bool StartsWith(char* s1, char* s2);
        vector<std::string> Split(const std::string& in_string, char delimiter);
    }  // namespace String

}  // namespace Utils
