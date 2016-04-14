#pragma once
#include <pebble.h>

#define SETTINGS_VERSION_KEY 0
#define SETTINGS_VERSION 1
#define SETTINGS_DATA_KEY 1

typedef struct {
    GColor color_background;
    GColor color_ticks;
    GColor color_hour_hand;
    GColor color_minute_hand;
    GColor color_second_hand;
    GColor color_battery;
} Settings;

Settings *settings_load();
void settings_save(Settings *this);
void settings_free(Settings *this);
