#pragma once

#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include "WiFiManager.h"

class WebServer {
public:
    WebServer();
    void begin();
    void handleDNS();

private:
    AsyncWebServer server;
    DNSServer dnsServer;
    WiFiManager& wifiManager;

    void setupRoutes();
    String getMainPage();
    String getSettingsPage();
};