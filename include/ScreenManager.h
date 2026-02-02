//
// Created by ben on 02/02/2026.
//

#pragma once
#include "Screen.h"

// These exist in main.cpp, but other screens need to see them
extern Screen* currentScreen;
extern Screen* nextScreen;

// Function to switch screens
void requestScreen(Screen* s);
