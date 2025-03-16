#include <Arduino.h>
#include "WiFiManager.h"
#include "WebServer.h"

WebServer* webServer = nullptr;

void setup() {
    Serial.begin(115200);
    
    // Initialize WiFi Manager
    WiFiManager::getInstance().begin();
    
    // Initialize Web Server
    webServer = new WebServer();
    webServer->begin();
}

void loop() {
    if (webServer) {
        webServer->handleDNS();
    }
    delay(1);
}