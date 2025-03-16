#include "WebServer.h"

WebServer::WebServer()
    : server(80)
    , wifiManager(WiFiManager::getInstance()) {
}

void WebServer::begin() {
    setupRoutes();
    server.begin();
    // Captive portal setup - redirect all requests to our IP
    dnsServer.start(53, "*", WiFi.softAPIP());
}

void WebServer::handleDNS() {
    dnsServer.processNextRequest();
}

void WebServer::setupRoutes() {
    // Captive portal - catch-all handler
    server.onNotFound([](AsyncWebServerRequest *request){
        request->redirect("/");
    });

    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request){
        request->send(200, "text/html", getMainPage());
    });

    server.on("/settings", HTTP_GET, [this](AsyncWebServerRequest *request){
        request->send(200, "text/html", getSettingsPage());
    });

    server.on("/save-settings", HTTP_POST, [this](AsyncWebServerRequest *request){
        if (request->hasParam("ssid", true) && request->hasParam("password", true)) {
            String ssid = request->getParam("ssid", true)->value();
            String password = request->getParam("password", true)->value();
            wifiManager.saveSettings(ssid, password);
            request->send(200, "text/html", "Settings saved. ESP32 will attempt to connect to the network.");
            ESP.restart();
        } else {
            request->send(400, "text/plain", "Missing parameters");
        }
    });

    // Handle captive portal detection
    server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request){
        request->redirect("/");
    });
    server.on("/fwlink", HTTP_GET, [](AsyncWebServerRequest *request){
        request->redirect("/");
    });
}

String WebServer::getMainPage() {
    return R"(
<!DOCTYPE html>
<html>
<head>
    <title>P4k Control</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link href="https://fonts.googleapis.com/icon?family=Material+Icons" rel="stylesheet">
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
            background: #121212;
            color: #ffffff;
        }
        .header {
            background: #1a1a1a;
            padding: 10px 20px;
            display: flex;
            justify-content: space-between;
            align-items: center;
            border-bottom: 1px solid #333;
        }
        .header h1 {
            margin: 0;
            font-size: 18px;
            color: #ff0000;
        }
        .settings-icon {
            color: #ff0000;
            text-decoration: none;
            cursor: pointer;
            display: flex;
            align-items: center;
        }
        .material-icons {
            font-size: 24px;
        }
        .container {
            max-width: 600px;
            margin: 40px auto;
            padding: 20px;
        }
        .content {
            background: #1e1e1e;
            padding: 20px;
            border-radius: 8px;
            box-shadow: 0 2px 4px rgba(0,0,0,0.2);
        }
    </style>
</head>
<body>
    <div class="header">
        <h1>Pocket4k Control</h1>
        <a href="/settings" class="settings-icon">
            <span class="material-icons">settings</span>
        </a>
    </div>
    <div class="container">
        <div class="content">
            <h2>Hello World</h2>
        </div>
    </div>
</body>
</html>
)";
}

String WebServer::getSettingsPage() {
    return R"(
<!DOCTYPE html>
<html>
<head>
    <title>P4k Settings</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link href="https://fonts.googleapis.com/icon?family=Material+Icons" rel="stylesheet">
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
            background: #121212;
            color: #ffffff;
        }
        .header {
            background: #1a1a1a;
            padding: 10px 20px;
            display: flex;
            justify-content: space-between;
            align-items: center;
            border-bottom: 1px solid #333;
        }
        .header h1 {
            margin: 0;
            font-size: 18px;
            color: #ff0000;
        }
        .back-icon {
            color: #ff0000;
            text-decoration: none;
            cursor: pointer;
            display: flex;
            align-items: center;
        }
        .material-icons {
            font-size: 24px;
        }
        .container {
            max-width: 600px;
            margin: 40px auto;
            padding: 20px;
        }
        .content {
            background: #1e1e1e;
            padding: 20px;
            border-radius: 8px;
            box-shadow: 0 2px 4px rgba(0,0,0,0.2);
        }
        .form-group {
            margin-bottom: 15px;
        }
        label {
            display: block;
            margin-bottom: 5px;
            color: #aaa;
        }
        input {
            width: 100%;
            padding: 8px;
            background: #2a2a2a;
            border: 1px solid #333;
            border-radius: 4px;
            box-sizing: border-box;
            color: #ffffff;
        }
        input:focus {
            outline: none;
            border-color: #ff0000;
        }
        button {
            background: #ff0000;
            color: white;
            padding: 10px 20px;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            width: 100%;
        }
        button:hover {
            background: #cc0000;
        }
    </style>
</head>
<body>
    <div class="header">
        <a href="/" class="back-icon">
            <span class="material-icons">arrow_back</span>
        </a>
        <h1>Pocket4k Control</h1>
        <div style="width: 24px;"></div>
    </div>
    <div class="container">
        <div class="content">
            <h2>WiFi Settings</h2>
            <form action="/save-settings" method="POST">
                <div class="form-group">
                    <label>Network SSID:</label>
                    <input type="text" name="ssid" required>
                </div>
                <div class="form-group">
                    <label>Network Password:</label>
                    <input type="password" name="password" required>
                </div>
                <button type="submit">Save Settings</button>
            </form>
        </div>
    </div>
</body>
</html>
)";
}