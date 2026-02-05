//
// Created by ben on 05/02/2026.
//

#include <Arduino.h>
#include "WarDrivingLoggerScreen.h"
#include <U8g2lib.h>
#include "ScreenManager.h"
#include "UIConstants.h"
#include "MenuScreen.h"

extern U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2;

void WarDrivingLoggerEnter() {}

void WarDrivingLoggerExit() {}

void WarDrivingLoggerEvent(ButtonEvent evt) {
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

void WarDrivingLoggerDraw() {
    u8g2.drawStr(0, 12, "WarDrivingLoggerScreen");
}

Screen WarDrivingLoggerScreen = {
    WarDrivingLoggerEnter,
    WarDrivingLoggerEvent,
    WarDrivingLoggerDraw,
    WarDrivingLoggerExit
};
