#include "minute_hand_layer.h"

static void minute_hand_layer_update_proc(Layer *layer, GContext *ctx) {
    GRect bounds = layer_get_bounds(layer);
    GPoint center = grect_center_point(&bounds);

    const int16_t hand_len = (bounds.size.w / 2) - 20;
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    int32_t angle = TRIG_MAX_ANGLE * t->tm_min / 60;
    GPoint point = {
        .x = (int16_t)(sin_lookup(angle) * (int32_t)hand_len / TRIG_MAX_RATIO) + center.x,
        .y = (int16_t)(-cos_lookup(angle) * (int32_t)hand_len / TRIG_MAX_RATIO) + center.y
    };

    graphics_context_set_stroke_color(ctx, GColorWhite);
    graphics_context_set_stroke_width(ctx, 6);
    graphics_draw_line(ctx, point, center);
}

MinuteHandLayer *minute_hand_layer_create(GRect frame) {
    MinuteHandLayer *this = layer_create(frame);
    layer_set_update_proc(this, minute_hand_layer_update_proc);
    return this;
}

void minute_hand_layer_destroy(MinuteHandLayer *this) {
    layer_destroy(this);
}
