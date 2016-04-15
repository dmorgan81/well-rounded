#include <pebble.h>
#include "logging.h"
#include "settings.h"
#include "sync.h"
#include "layers/tick_layer.h"
#include "layers/time_layer.h"
#include "layers/radial_layer.h"

static Window *s_window;
static Settings *s_settings;
static TickLayer *s_tick_layer;
#ifdef PBL_HEALTH
static RadialLayer *s_health_layer;
#endif
static RadialLayer *s_batt_layer;
static TimeLayer *s_time_layer;

static void tick_handler(struct tm* tick_time, TimeUnits units_changed) {
    logd("%s", __func__);
    layer_mark_dirty(s_time_layer);
}

static void batt_handler(BatteryChargeState charge) {
    logd("%s", __func__);
    radial_layer_set_value(s_batt_layer, charge.charge_percent);
}

#ifdef PBL_HEALTH
static void health_handler(HealthEventType event, void *context) {
    logd("%s", __func__);
    if (event == HealthEventSignificantUpdate) {
        time_t start = time_start_of_today();
        time_t end = start + SECONDS_PER_DAY;
        HealthServiceAccessibilityMask accessible = health_service_metric_averaged_accessible(HealthMetricStepCount, start, end, HealthServiceTimeScopeDaily);
        if (accessible & HealthServiceAccessibilityMaskAvailable) {
            HealthValue average = health_service_sum_averaged(HealthMetricStepCount, start, end, HealthServiceTimeScopeDaily);
            radial_layer_set_max(s_health_layer, average);
        }
        health_handler(HealthEventMovementUpdate, context);
    }
    if (event == HealthEventMovementUpdate) {
        HealthServiceAccessibilityMask accessible = health_service_metric_accessible(HealthMetricStepCount, time_start_of_today(), time(NULL));
        if (accessible & HealthServiceAccessibilityMaskAvailable) {
            HealthValue sum = health_service_sum_today(HealthMetricStepCount);
            radial_layer_set_value(s_health_layer, sum);
        }
    }
}
#endif

static void sync_handler(const uint32_t key, const Tuple *new_tuple, const Tuple *old_tuple, void *context) {
    logd("%s", __func__);
    if (new_tuple == NULL || old_tuple == NULL) {
        return;
    }
    switch (key) {
        case AppKeyColorBackground:
            s_settings->color_background = GColorFromHEX(new_tuple->value->uint32);
            window_set_background_color(s_window, s_settings->color_background);
            break;
        case AppKeyColorTicks:
            s_settings->color_ticks = GColorFromHEX(new_tuple->value->uint32);
            tick_layer_set_color(s_tick_layer, s_settings->color_ticks);
            break;
        case AppKeyColorHandHour:
            s_settings->color_hour_hand = GColorFromHEX(new_tuple->value->uint32);
            time_layer_set_settings(s_time_layer, s_settings);
            break;
        case AppKeyColorHandMinute:
            s_settings->color_minute_hand = GColorFromHEX(new_tuple->value->uint32);
            time_layer_set_settings(s_time_layer, s_settings);
            break;
        case AppKeyColorHandSecond:
            s_settings->color_second_hand = GColorFromHEX(new_tuple->value->uint32);
            time_layer_set_settings(s_time_layer, s_settings);
            break;
        case AppKeyColorBattery:
            s_settings->color_battery = GColorFromHEX(new_tuple->value->uint32);
            radial_layer_set_color(s_batt_layer, s_settings->color_battery);
            break;
        case AppKeyShowTicks:
            s_settings->show_ticks = new_tuple->value->uint8;
            layer_set_hidden(s_tick_layer, !s_settings->show_ticks);
            break;
        case AppKeyShowSecondHand:
            s_settings->show_second_hand = new_tuple->value->uint8;
            time_layer_set_settings(s_time_layer, s_settings);
            tick_timer_service_subscribe(s_settings->show_second_hand ? SECOND_UNIT : MINUTE_UNIT, tick_handler);
            break;
        case AppKeyShowBattery:
            s_settings->show_battery = new_tuple->value->uint8;
            layer_set_hidden(s_batt_layer, !s_settings->show_battery);
            if (s_settings->show_battery) {
                batt_handler(battery_state_service_peek());
                battery_state_service_subscribe(batt_handler);
            } else {
                battery_state_service_unsubscribe();
            }
            break;
#ifdef PBL_HEALTH
        case AppKeyColorHealth:
            s_settings->color_health = GColorFromHEX(new_tuple->value->uint32);
            radial_layer_set_color(s_health_layer, s_settings->color_health);
            break;
        case AppKeyShowHealth:
            s_settings->show_health = new_tuple->value->uint8;
            if (s_settings->show_health && health_service_events_subscribe(health_handler, NULL)) {
                layer_set_hidden(s_health_layer, false);
            } else {
                layer_set_hidden(s_health_layer, true);
                health_service_events_unsubscribe();
            }
            break;
#endif
    }
    settings_save(s_settings);
}

static void window_load(Window *window) {
    logd("%s", __func__);
    s_settings = settings_load();
    sync_init(s_settings, sync_handler);

    window_set_background_color(window, s_settings->color_background);

    Layer *root_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(root_layer);

    s_tick_layer = tick_layer_create(bounds);
    tick_layer_set_color(s_tick_layer, s_settings->color_ticks);
    layer_set_hidden(s_tick_layer, !s_settings->show_ticks);

    s_time_layer = time_layer_create(bounds);
    time_layer_set_settings(s_time_layer, s_settings);

    s_batt_layer = radial_layer_create(grect_inset(bounds, GEdgeInsets(PBL_IF_ROUND_ELSE(70, 50))));
    radial_layer_set_max(s_batt_layer, 100);
    radial_layer_set_inset(s_batt_layer, 10);
    radial_layer_set_color(s_batt_layer, s_settings->color_battery);
    layer_set_hidden(s_batt_layer, !s_settings->show_battery);

#ifdef PBL_HEALTH
    s_health_layer = radial_layer_create(grect_inset(bounds, GEdgeInsets(PBL_IF_ROUND_ELSE(60, 40))));
    radial_layer_set_max(s_health_layer, 0);
    radial_layer_set_inset(s_health_layer, 10);
    radial_layer_set_color(s_health_layer, s_settings->color_health);
    layer_set_hidden(s_health_layer, !s_settings->show_health);
#endif

    layer_add_child(root_layer, s_tick_layer);
#ifdef PBL_HEALTH
    layer_add_child(root_layer, s_health_layer);
#endif
    layer_add_child(root_layer, s_batt_layer);
    layer_add_child(root_layer, s_time_layer);

    tick_timer_service_subscribe(s_settings->show_second_hand ? SECOND_UNIT : MINUTE_UNIT, tick_handler);

    if (s_settings->show_battery) {
        batt_handler(battery_state_service_peek());
        battery_state_service_subscribe(batt_handler);
    }

#ifdef PBL_HEALTH
    if (s_settings->show_health && !health_service_events_subscribe(health_handler, NULL)) {
        layer_set_hidden(s_health_layer, true);
    }
#endif
}

static void window_unload(Window *window) {
    logd("%s", __func__);
    battery_state_service_unsubscribe();
    tick_timer_service_unsubscribe();

#ifdef PBL_HEALTH
    health_service_events_unsubscribe();
    radial_layer_destroy(s_health_layer);
#endif

    radial_layer_destroy(s_batt_layer);
    time_layer_destroy(s_time_layer);
    tick_layer_destroy(s_tick_layer);

    sync_deinit();
    settings_free(s_settings);
}

static void init(void) {
    logd("%s", __func__);
    s_window = window_create();
    window_set_window_handlers(s_window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
    window_stack_push(s_window, true);
}

static void deinit(void) {
    logd("%s", __func__);
    window_destroy(s_window);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
