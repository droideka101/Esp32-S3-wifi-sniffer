//
// Created by ben on 04/02/2026.
//

#pragma once
#include "Screen.h"

extern Screen NetworkInfoScreen;

int getSelectedNetworkIndex();
String getSelectedSSID();
int getSelectedRSSI();
int getSelectedChannel();
int getSelectedEncryption();