//
// Created by ben on 05/02/2026.
//

#include <Arduino.h>
#include "ChannelAnalyzerScreen.h"
#include <U8g2lib.h>
#include "ScreenManager.h"
#include "UIConstants.h"
#include "MenuScreen.h"

extern U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2;

void ChannelAnalyzerEnter() {}

void ChannelAnalyzerExit() {}

void ChannelAnalyzerEvent(ButtonEvent evt) {
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

void ChannelAnalyzerDraw() {
    u8g2.drawStr(0, 12, "ChannelAnalyzerScreen");
}

Screen ChannelAnalyzerScreen = {
    ChannelAnalyzerEnter,
    ChannelAnalyzerEvent,
    ChannelAnalyzerDraw,
    ChannelAnalyzerExit
};
