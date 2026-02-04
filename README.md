# WiFi Scraper

WiFi Scraper is a feature-rich ESP32-based WiFi scanning and network analysis tool. It runs on an SH1106 128x64 OLED display and provides a menu-driven interface to explore nearby networks, scan devices, monitor signal strength (RSSI), and more.

---

## Features

- **Network Scanner**: Scan nearby WiFi networks and view SSID, RSSI, channel, and encryption type.
- **Device Scanner** *(coming soon)*: Detect devices connected to the network.
- **Packet Sniffer** *(coming soon)*: Capture and analyze network packets.
- **Channel Analyzer** *(coming soon)*: Analyze channel congestion and optimize WiFi performance.
- **RSSI Meter** *(coming soon)*: Measure WiFi signal strength.
- **Wardriving Logger** *(coming soon)*: Log WiFi networks while moving.
- **Fake AP** *(coming soon)*: Create a test access point for security research.
- **Deauth Tester** *(coming soon)*: Test deauthentication attacks (for testing only).
- **Battery Monitor** *(coming soon)*: Monitor device power consumption.

---

## Hardware Requirements

- ESP32-S3 DevKitC-1 (or compatible ESP32 board)
- 1.3" 128x64 SH1106 OLED display
- Optional: Custom I2C pins (configured in `main.cpp`)

---

## Installation

1. Clone the repository:

   ```bash
   git clone https://github.com/<your-username>/wifi-scraper.git
   cd wifi-scraper
