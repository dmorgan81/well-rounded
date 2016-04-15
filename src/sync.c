#include "logging.h"
#include "sync.h"

static AppSync s_sync;
static uint8_t s_sync_buffer[256];

static void sync_error_handler(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
    logd("%s", __func__);
    loge("sync error %d %d", dict_error, app_message_error);
}

void sync_init(Settings *settings, AppSyncTupleChangedCallback sync_changed_handler) {
    logd("%s", __func__);
    app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

    Tuplet initial_values[] = {
        TupletInteger(AppKeyColorBackground, settings->color_background.argb),
        TupletInteger(AppKeyColorTicks, settings->color_ticks.argb),
        TupletInteger(AppKeyColorHandHour, settings->color_hour_hand.argb),
        TupletInteger(AppKeyColorHandMinute, settings->color_minute_hand.argb),
        TupletInteger(AppKeyColorHandSecond, settings->color_second_hand.argb),
        TupletInteger(AppKeyColorBattery, settings->color_battery.argb),
        TupletInteger(AppKeyShowTicks, settings->show_ticks),
        TupletInteger(AppKeyShowSecondHand, settings->show_second_hand),
        TupletInteger(AppKeyShowBattery, settings->show_battery),
#ifdef PBL_HEALTH
        TupletInteger(AppKeyColorHealth, settings->color_health.argb),
        TupletInteger(AppKeyShowHealth, settings->show_health),
#endif
    };

    app_sync_init(&s_sync, s_sync_buffer, sizeof(s_sync_buffer), initial_values, ARRAY_LENGTH(initial_values),
        sync_changed_handler, sync_error_handler, NULL);
}

void sync_deinit() {
    logd("%s", __func__);
    app_sync_deinit(&s_sync);
}
