//
// Created by ben on 04/02/2026.
//
#include <WiFi.h>
#include <ESP32Ping.h>
#include <Arduino.h>
#include "MenuScreen.h"
#include <U8g2lib.h>
#include "ScreenManager.h"
#include "UIConstants.h"
#include "DeviceScannerScreen.h"

// these still live globally for now
extern U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2;

struct DeviceInfo {
    IPAddress ip;
    String hostname;
    bool alive;
};

static DeviceInfo devices[50];
static int deviceCount = 0;
static int currentDeviceIndex = 0;

static bool needsWifi = true;
static bool wifiPrompted = false;
static bool scanStarted = false;
static bool scanComplete = false;

static int nextIP = 1;
static bool scanComplete = false;


String readLineFromSerial() {
    String input = "";
    while (true) {
        if (Serial.available()) {
            char c = Serial.read();
            if (c == '\n' || c == '\r') break;
            input += c;
            Serial.print(c); // echo back as typed
        }
        delay(10);
    }

    Serial.println();

    // Flush any leftover characters in the buffer
    while (Serial.available()) Serial.read();

    return input;
}


void promptWiFiCredentials() {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x12_mr);
    u8g2.drawStr(0, 12, "Device Scanner");
    u8g2.drawStr(0, 24, "Enter WiFi via Serial");
    u8g2.sendBuffer();

    Serial.println();
    Serial.println("=== Device Scanner WiFi Setup ===");
    Serial.println("Enter SSID:");

    while (!Serial.available()) delay(10);
    String ssid = readLineFromSerial();
    ssid.trim();

    Serial.println("Enter Password:");
    while (!Serial.available()) delay(10);
    String pass = readLineFromSerial();
    pass.trim();

    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), pass.c_str());

    uint32_t start = millis();
    while (WiFi.status() != WL_CONNECTED) {
        delay(300);
        Serial.print(".");
        if (millis() - start > 15000) {
            Serial.println("\nConnection failed.");
            return;
        }
    }

    Serial.println("\nConnected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    u8g2.clearBuffer();
    u8g2.drawStr(0, 12, "Device Scanner");
    u8g2.drawStr(0, 24, "WiFi Connected!");
    u8g2.sendBuffer();
    delay(2000);
}

void scanForDevices() {
    deviceCount = 0;

    IPAddress localIP = WiFi.localIP();
    IPAddress subnet = WiFi.subnetMask();


    for (int i = 1; i < 255 && deviceCount < 50; i++) {
        IPAddress target(
            localIP[0],
            localIP[1],
            localIP[2],
            i
        );

        Serial.print("Pinging: ");
        Serial.println(target);

        if (Ping.ping(target)) {
            Serial.println(" - alive");
            devices[deviceCount].ip = target;
            devices[deviceCount].alive = true;
            devices[deviceCount].hostname = ""; // optional later
            deviceCount++;
        } else {
            Serial.println(" - failed");
        }
    }
}

void ScanForDevicesTest() {
    deviceCount = 0;


    IPAddress localIP = WiFi.localIP();
    IPAddress subnet = WiFi.subnetMask();



    for (int batch = 0; batch < 5 && nextIP < 255; batch++) {
        IPAddress target(
            localIP[0],
            localIP[1],
            localIP[2],
            nextIP
        );

        Serial.print("Pinging: ");
        Serial.println(target);

        if (Ping.ping(target)) {
            Serial.println(" - alive");
            devices[deviceCount].ip = target;
            devices[deviceCount].alive = true;
            devices[deviceCount].hostname = ""; // optional later
            deviceCount++;
        } else {
            Serial.println(" - failed");
        }
        nextIP++;
    }
    if (nextIP > 255) {
        scanComplete = true;
    }
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x12_mr);
    u8g2.drawStr(0, 12, "Scanning devices...");
    u8g2.setCursor(0, 24);
    u8g2.print("Progress: "); u8g2.print(nextIP-1); u8g2.print("/254");
    u8g2.sendBuffer();

    return;
}


void DeviceScannerEnter() {
    deviceCount = 0;
    currentDeviceIndex = 0;

    needsWifi = (WiFi.status() != WL_CONNECTED);
    wifiPrompted = false;
    scanStarted = false;
    scanComplete = false;
}

void DeviceScannerExit() {}

void DeviceScannerEvent(ButtonEvent evt) {
    if (evt == BTN_DOWN) {
    }

    if (evt == BTN_UP) {
    }

    if (evt == BTN_SELECT) {
    }

    if (evt == BTN_BACK) {
        requestScreen(&MenuScreen);
    }
}

void DeviceScannerDraw() {
    if (needsWifi) {

        if (!wifiPrompted) {
            promptWiFiCredentials();
            wifiPrompted = true;
        }

        if (WiFi.status() == WL_CONNECTED) {
            needsWifi = false;
        }

        return;
    }

    if (!scanStarted) {
        u8g2.drawStr(0, 12, "Scanning devices...");
        scanStarted = true;
        return;
    }

    if (!scanComplete) {
        Serial.println("scanning devices...");
        scanForDevices();
        scanComplete = true;
    }

    // Draw results (later)
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x12_mr);
    u8g2.drawStr(0, 12, "Devices found:");
    for (int i = 0; i < deviceCount && i < 4; i++) { // 4 lines visible
        u8g2.setCursor(0, 24 + i*12);
        u8g2.print(devices[i].ip);
    }
    u8g2.sendBuffer();

    // listing logic will go here
}

Screen DeviceScannerScreen = {
    DeviceScannerEnter,
    DeviceScannerEvent,
    DeviceScannerDraw,
    DeviceScannerExit
};
