#pragma once

#include <Arduino.h>
#include <Preferences.h>

class WiFiManager {
public:
    static WiFiManager& getInstance();
    void begin();
    void setupAP();
    void loadSettings();
    void saveSettings(const String& ssid, const String& password);
    
    String getAPSSID() const { return apSSID; }
    String getAPPass() const { return apPass; }

private:
    WiFiManager();
    static WiFiManager* instance;
    
    Preferences preferences;
    String apSSID;
    String apPass;
    String staSSID;
    String staPass;
};