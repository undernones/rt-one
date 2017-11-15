//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef VIEWER_SETTINGS_H
#define VIEWER_SETTINGS_H

#include <mutex>
#include <string>
#include <unordered_map>

class Settings
{
public:
    Settings(Settings const&) = delete;
    Settings(Settings&&) = delete;
    Settings& operator=(Settings const&) = delete;
    Settings& operator=(Settings&&) = delete;

    /// The directory in which to store image dumps.
    /// Environment variable ONE_SAVE_DIR
    /// Default to ~/Desktop
    static std::string saveDir();

private:
    Settings() = default;
    static Settings& instance();

    std::string stringSetting(const std::string& key, const std::function<std::string(void)>& defaultValueFunc);

    std::mutex mMutex;
    std::unordered_map<std::string, std::string> mStringSettings;
};

#endif // VIEWER_SETTINGS_H
