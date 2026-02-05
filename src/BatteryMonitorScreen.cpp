//
// Created by ben on 05/02/2026.
//

#include <Arduino.h>
#include "BatteryMonitorScreen.h"
#include <U8g2lib.h>
#include "ScreenManager.h"
#include "UIConstants.h"
#include "MenuScreen.h"

extern U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2;

void BatteryMonitorEnter() {}

void BatteryMonitorExit() {}

void BatteryMonitorEvent(ButtonEvent evt) {
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

void BatteryMonitorDraw() {
    u8g2.drawStr(0, 12, "BatteryMonitorScreen");
}

Screen ChannelAnalyzerScreen = {
    BatteryMonitorEnter,
    BatteryMonitorrEvent,
    BatteryMonitorDraw,
    BatteryMonitorExit
};
