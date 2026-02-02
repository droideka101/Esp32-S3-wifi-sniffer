//
// Created by ben on 01/02/2026.
//

#pragma once

#include <Arduino.h>

enum ButtonEvent {
    BTN_NONE,
    BTN_UP,
    BTN_DOWN,
    BTN_SELECT,
    BTN_BACK
};

struct Screen {
    void (*onEnter)();
    void (*onEvent)(ButtonEvent);
    void (*onDraw)();
    void (*onExit)();
};
