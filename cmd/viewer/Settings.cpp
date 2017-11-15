//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Settings.h"

namespace
{

std::string
getStringEnv(const std::string& name, const std::function<std::string(void)>& defaultValueFunc)
{
    const char* value = getenv(name.c_str());
    if (value != nullptr) {
        return std::string(value);
    }
    return defaultValueFunc();
}

}

Settings&
Settings::instance()
{
    static Settings inst;
    return inst;
}

std::string
Settings::stringSetting(const std::string& key, const std::function<std::string(void)>& defaultValueFunc)
{
    std::lock_guard<std::mutex> lock(mMutex);
    if (mStringSettings.find(key) == mStringSettings.end()) {
        mStringSettings[key] = getStringEnv(key, defaultValueFunc);
    }
    return mStringSettings[key];
}

std::string
Settings::saveDir()
{
    return instance().stringSetting("ONE_SAVE_DIR", []() {
        auto homeDir = std::string();
        auto homeEnv = getenv("HOME");
        if (homeEnv != nullptr) {
            homeDir = std::string(homeEnv);
        }
    
        return homeDir + "/Desktop/";
    });
}
