#include "logging.h"
#include "settings.h"

Settings *settings_load() {
    logd("%s", __func__);
    Settings *settings = malloc(sizeof(Settings));

    int32_t version = persist_read_int(SETTINGS_VERSION_KEY);
    if (version == 0) {
        settings->color_background = GColorBlack;
        settings->color_ticks = GColorLightGray;
        settings->color_hour_hand = GColorRed;
        settings->color_minute_hand = GColorLightGray;
        settings->color_second_hand = GColorWhite;
        settings->color_battery = GColorIslamicGreen;
        settings_save(settings);
    } else {
        persist_read_data(SETTINGS_DATA_KEY, settings, sizeof(Settings));
    }

    return settings;
}

void settings_save(Settings *this) {
    logd("%s", __func__);
    persist_write_int(SETTINGS_VERSION_KEY, SETTINGS_VERSION);
    persist_write_data(SETTINGS_DATA_KEY, this, sizeof(Settings));
}

void settings_free(Settings *this) {
    logd("%s", __func__);
    free(this);
}
