#include "../logging.h"
#include "hand_layer.h"

typedef struct {
    Settings *settings;
    HandLayerAngleCalc angle_calc;
    HandLayerLenCalc len_calc;
    uint8_t stroke_width;
    GColor color;
} Internal;

static void hand_layer_update_proc(Layer *layer, GContext *ctx) {
    logd("%s", __func__);
    Internal *internal = (Internal *) layer_get_data(layer);

    GRect bounds = layer_get_bounds(layer);
    GPoint center = grect_center_point(&bounds);

    const int16_t hand_len = (*internal->len_calc)(layer, &bounds);
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    int32_t angle = (*internal->angle_calc)(layer, t);
    GPoint point = {
        .x = (int16_t)(sin_lookup(angle) * (int32_t)hand_len / TRIG_MAX_RATIO) + center.x,
        .y = (int16_t)(-cos_lookup(angle) * (int32_t)hand_len / TRIG_MAX_RATIO) + center.y
    };

    graphics_context_set_stroke_color(ctx, internal->color);
    graphics_context_set_stroke_width(ctx, internal->stroke_width);
    graphics_draw_line(ctx, point, center);
}

HandLayer *hand_layer_create(GRect frame) {
    logd("%s", __func__);
    HandLayer *this = layer_create_with_data(frame, sizeof(Internal));
    Internal *internal = (Internal *) layer_get_data(this);
    internal->stroke_width = 1;
    internal->color = GColorWhite;
    layer_set_update_proc(this, hand_layer_update_proc);
    return this;
}

void hand_layer_destroy(HandLayer *this) {
    logd("%s", __func__);
    layer_destroy(this);
}

void hand_layer_set_angle_calc(HandLayer *this, HandLayerAngleCalc angle_calc) {
    logd("%s", __func__);
    ((Internal *) layer_get_data(this))->angle_calc = angle_calc;
    layer_mark_dirty(this);
}

void hand_layer_set_len_calc(HandLayer *this, HandLayerLenCalc len_calc) {
    logd("%s", __func__);
    ((Internal *) layer_get_data(this))->len_calc = len_calc;
    layer_mark_dirty(this);
}

void hand_layer_set_stroke_width(HandLayer *this, uint8_t stroke_width) {
    logd("%s", __func__);
    ((Internal *) layer_get_data(this))->stroke_width = stroke_width;
    layer_mark_dirty(this);
}

void hand_layer_set_color(HandLayer *this, GColor color) {
    logd("%s", __func__);
    ((Internal *) layer_get_data(this))->color = color;
    layer_mark_dirty(this);
}
