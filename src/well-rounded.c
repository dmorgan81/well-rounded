#include <pebble.h>
#include "logging.h"
#include "layers/tick_layer.h"
#include "layers/time_layer.h"

static Window *s_window;
static TickLayer *s_tick_layer;
static TimeLayer *s_time_layer;

static void tick_handler(struct tm* tick_time, TimeUnits units_changed) {
    logd("%s", __func__);
    layer_mark_dirty(s_time_layer);
}

static void window_load(Window *window) {
    logd("%s", __func__);
    Layer *root_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(root_layer);

    s_tick_layer = tick_layer_create(bounds);
    layer_add_child(root_layer, s_tick_layer);

    s_time_layer = time_layer_create(bounds);
    layer_add_child(root_layer, s_time_layer);

    tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

static void window_unload(Window *window) {
    logd("%s", __func__);
    tick_timer_service_unsubscribe();

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
