#include <pebble.h>
#include "logging.h"
#include "layers/tick_layer.h"
#include "layers/time_layer.h"
#include "layers/radial_layer.h"

static Window *s_window;
static TickLayer *s_tick_layer;
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

static void window_load(Window *window) {
    logd("%s", __func__);
    Layer *root_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(root_layer);

    s_tick_layer = tick_layer_create(bounds);
    s_time_layer = time_layer_create(bounds);

    s_batt_layer = radial_layer_create(grect_inset(bounds, GEdgeInsets(PBL_IF_ROUND_ELSE(75, 60))));
    radial_layer_set_max(s_batt_layer, 100);
    radial_layer_set_inset(s_batt_layer, 30);

    layer_add_child(root_layer, s_tick_layer);
    layer_add_child(root_layer, s_batt_layer);
    layer_add_child(root_layer, s_time_layer);

    tick_timer_service_subscribe(SECOND_UNIT, tick_handler);

    batt_handler(battery_state_service_peek());
    battery_state_service_subscribe(batt_handler);
}

static void window_unload(Window *window) {
    logd("%s", __func__);
    battery_state_service_unsubscribe();
    tick_timer_service_unsubscribe();

    radial_layer_destroy(s_batt_layer);
    time_layer_destroy(s_time_layer);
    tick_layer_destroy(s_tick_layer);
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
