//
// Created by ben on 02/02/2026.
//
#include <Arduino.h>
#include <U8g2lib.h>
#include "Screen.h"
#include "ScreenManager.h"
#include "MenuScreen.h"
#include "UIConstants.h"

extern U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2;
extern Screen MenuScreen;

void startEnter() {}
void startExit() {}
void startEvent(ButtonEvent evt) {
    if (evt == BTN_SELECT) {
        requestScreen(&MenuScreen);
    }
}
void startDraw() {
    u8g2.clearBuffer();

    // --- SMALL WIFI ICON (2× size) ---
    u8g2.setFont(u8g2_font_open_iconic_www_2x_t);
    u8g2.drawGlyph(54, 24, 0x0048);

    // --- TITLE ---
    u8g2.setFont(u8g2_font_6x12_mr);
    const char* title = "WiFi Scraper";
    int tw = u8g2.getStrWidth(title);
    u8g2.setCursor((128 - tw) / 2, 40);
    u8g2.print(title);

    // --- START BUTTON FRAME ---
    const char* startText = " Start ";
    int sw = u8g2.getStrWidth(startText);
    int bx = (128 - (sw + 18)) / 2;
    int by = 46;
    u8g2.drawRBox(bx, by, sw + 18, 16, 3);

    // --- SMALL PLAY ICON (centered vertically) ---
    u8g2.setFont(u8g2_font_open_iconic_play_2x_t);
    int iconY = by + 16;
    int iconX = bx + 1;
    u8g2.setDrawColor(0);
    u8g2.drawGlyph(iconX, iconY, 0x45);
    u8g2.setDrawColor(1);

    // --- BUTTON TEXT ---
    u8g2.setFont(u8g2_font_6x12_mr);
    u8g2.setCursor(bx + 16, by + 12);
    u8g2.print(startText);

    u8g2.sendBuffer();
}

Screen StartScreen = {
    startEnter,
    startEvent,
    startDraw,
    startExit
};
