#include "WiFiManager.h"
#include <WiFi.h>

#define DEFAULT_AP_SSID "P4k-Control"
#define DEFAULT_AP_PASS "esp32p4k"

WiFiManager* WiFiManager::instance = nullptr;

WiFiManager& WiFiManager::getInstance() {
    if (instance == nullptr) {
        instance = new WiFiManager();
    }
    return *instance;
}

WiFiManager::WiFiManager()
    : apSSID(DEFAULT_AP_SSID)
    , apPass(DEFAULT_AP_PASS) {
}

void WiFiManager::begin() {
    preferences.begin("wifi-config", false);
    loadSettings();
    setupAP();
}

void WiFiManager::setupAP() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(apSSID.c_str(), apPass.c_str());
}

void WiFiManager::loadSettings() {
    apSSID = preferences.getString("ap_ssid", DEFAULT_AP_SSID);
    apPass = preferences.getString("ap_pass", DEFAULT_AP_PASS);
    staSSID = preferences.getString("sta_ssid", "");
    staPass = preferences.getString("sta_pass", "");
}

void WiFiManager::saveSettings(const String& ssid, const String& password) {
    staSSID = ssid;
    staPass = password;
    preferences.putString("sta_ssid", staSSID);
    preferences.putString("sta_pass", staPass);
}