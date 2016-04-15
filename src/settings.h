#pragma once
#include <pebble.h>

#define SETTINGS_VERSION_KEY 0
#define SETTINGS_VERSION 1
#define SETTINGS_DATA_KEY 1

typedef enum {
    AppKeyColorBackground = 0,
    AppKeyColorTicks,
    AppKeyColorHandHour,
    AppKeyColorHandMinute,
    AppKeyColorHandSecond,
    AppKeyColorBattery,
    AppKeyShowTicks,
    AppKeyShowSecondHand,
    AppKeyShowBattery,
} SettingAppKeys;

/*
The order of the struct is important and cannot change between versions.
Settings cannot be removed, only marked deprecated.
New settings must go at the end.
*/

typedef struct {
    /* BEGIN SETTINGS_VERSION 1 */
    GColor color_background;
    GColor color_ticks;
    GColor color_hour_hand;
    GColor color_minute_hand;
    GColor color_second_hand;
    GColor color_battery;

    bool show_ticks;
    bool show_second_hand;
    bool show_battery;
    /* END SETTINGS_VERSION 1 */
} Settings;

Settings *settings_load();
void settings_save(Settings *this);
void settings_free(Settings *this);
