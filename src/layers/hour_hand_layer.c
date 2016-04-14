#include "../logging.h"
#include "hour_hand_layer.h"

static void hour_hand_layer_update_proc(Layer *layer, GContext *ctx) {
    logd("%s", __func__);
    GRect bounds = layer_get_bounds(layer);
    GPoint center = grect_center_point(&bounds);

    graphics_context_set_fill_color(ctx, GColorWhite);
    graphics_fill_circle(ctx, center, 5);

    const int16_t hand_len = (bounds.size.w / 2) - 30;
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    int32_t angle = TRIG_MAX_ANGLE * ((((t->tm_hour % 12) * 6) + (t->tm_min / 10))) / (12 * 6);
    GPoint point = {
        .x = (int16_t)(sin_lookup(angle) * (int32_t)hand_len / TRIG_MAX_RATIO) + center.x,
        .y = (int16_t)(-cos_lookup(angle) * (int32_t)hand_len / TRIG_MAX_RATIO) + center.y
    };

    graphics_context_set_stroke_color(ctx, GColorWhite);
    graphics_context_set_stroke_width(ctx, 6);
    graphics_draw_line(ctx, point, center);
}

HourHandLayer *hour_hand_layer_create(GRect frame) {
    logd("%s", __func__);
    HourHandLayer *this = layer_create(frame);
    layer_set_update_proc(this, hour_hand_layer_update_proc);
    return this;
}

void hour_hand_layer_destroy(HourHandLayer *this) {
    logd("%s", __func__);
    layer_destroy(this);
}
