#include "../logging.h"
#include "time_layer.h"
#include "hand_layer.h"

typedef struct {
    HandLayer *minute_hand_layer;
    HandLayer *hour_hand_layer;
    HandLayer *second_hand_layer;
    Layer *middle_dot_layer;
} Internal;

static void middle_dot_layer_update_proc(Layer *layer, GContext *ctx) {
    logd("%s", __func__);
    GRect bounds = layer_get_bounds(layer);
    GPoint center = grect_center_point(&bounds);

    graphics_context_set_fill_color(ctx, GColorWhite);
    graphics_fill_circle(ctx, center, 5);
}

static int32_t angle_calc_minute_hand(Layer *layer, struct tm *t) {
    logd("%s", __func__);
    return TRIG_MAX_ANGLE * t->tm_min / 60;
}

static int16_t len_calc_minute_hand(Layer *layer, GRect *bounds) {
    logd("%s", __func__);
    return (bounds->size.w / 2) - 20;
}

static int32_t angle_calc_hour_hand(Layer *layer, struct tm *t) {
    logd("%s", __func__);
    return TRIG_MAX_ANGLE * ((((t->tm_hour % 12) * 6) + (t->tm_min / 10))) / (12 * 6);
}

static int16_t len_calc_hour_hand(Layer *layer, GRect *bounds) {
    logd("%s", __func__);
    return (bounds->size.w / 2) - 30;
}

static int32_t angle_calc_second_hand(Layer *layer, struct tm *t) {
    logd("%s", __func__);
    return TRIG_MAX_ANGLE * t->tm_sec / 60;
}

static int16_t len_calc_second_hand(Layer *layer, GRect *bounds) {
    logd("%s", __func__);
    return (bounds->size.w / 2) - 20;
}

TimeLayer *time_layer_create(GRect frame) {
    logd("%s", __func__);
    TimeLayer *this = layer_create_with_data(frame, sizeof(Internal));
    Internal *internal = (Internal *) layer_get_data(this);

    internal->minute_hand_layer = hand_layer_create(frame);
    hand_layer_set_angle_calc(internal->minute_hand_layer, angle_calc_minute_hand);
    hand_layer_set_len_calc(internal->minute_hand_layer, len_calc_minute_hand);
    hand_layer_set_stroke_width(internal->minute_hand_layer, 6);
    layer_add_child(this, internal->minute_hand_layer);

    internal->hour_hand_layer = hand_layer_create(frame);
    hand_layer_set_angle_calc(internal->hour_hand_layer, angle_calc_hour_hand);
    hand_layer_set_len_calc(internal->hour_hand_layer, len_calc_hour_hand);
    hand_layer_set_stroke_width(internal->hour_hand_layer, 6);
    layer_add_child(this, internal->hour_hand_layer);

    internal->second_hand_layer = hand_layer_create(frame);
    hand_layer_set_angle_calc(internal->second_hand_layer, angle_calc_second_hand);
    hand_layer_set_len_calc(internal->second_hand_layer, len_calc_second_hand);
    hand_layer_set_stroke_width(internal->second_hand_layer, 2);
    layer_add_child(this, internal->second_hand_layer);

    internal->middle_dot_layer = layer_create(frame);
    layer_set_update_proc(internal->middle_dot_layer, middle_dot_layer_update_proc);
    layer_add_child(this, internal->middle_dot_layer);

    return this;
}

void time_layer_destroy(TimeLayer *this) {
    logd("%s", __func__);
    Internal *internal = (Internal *) layer_get_data(this);

    layer_destroy(internal->middle_dot_layer);
    hand_layer_destroy(internal->second_hand_layer);
    hand_layer_destroy(internal->hour_hand_layer);
    hand_layer_destroy(internal->minute_hand_layer);

    layer_destroy(this);
}
