#include "../logging.h"
#include "time_layer.h"
#include "hand_layer.h"
#include "circle_layer.h"

typedef struct {
    HandLayer *minute_hand_layer;
    HandLayer *hour_hand_layer;
    HandLayer *second_hand_layer;
    CircleLayer *middle_dot_layer;
} Internal;

static int32_t angle_calc_minute_hand(Layer *layer, struct tm *t) {
    logd("%s", __func__);
#ifdef DEMO
    return TRIG_MAX_ANGLE * 50 / 60;
#else
    return TRIG_MAX_ANGLE * t->tm_min / 60;
#endif
}

static int16_t len_calc_minute_hand(Layer *layer, GRect *bounds) {
    logd("%s", __func__);
    return (bounds->size.w / 2) - 20;
}

static int32_t angle_calc_hour_hand(Layer *layer, struct tm *t) {
    logd("%s", __func__);
#ifdef DEMO
    return TRIG_MAX_ANGLE * ((((1 % 12) * 6) + (50 / 10))) / (12 * 6);
#else
    return TRIG_MAX_ANGLE * ((((t->tm_hour % 12) * 6) + (t->tm_min / 10))) / (12 * 6);
#endif
}

static int16_t len_calc_hour_hand(Layer *layer, GRect *bounds) {
    logd("%s", __func__);
    return (bounds->size.w / 2) - 30;
}

static int32_t angle_calc_second_hand(Layer *layer, struct tm *t) {
    logd("%s", __func__);
#ifdef DEMO
    return TRIG_MAX_ANGLE * 30 / 60;
#else
    return TRIG_MAX_ANGLE * t->tm_sec / 60;
#endif
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

    internal->middle_dot_layer = circle_layer_create(frame);
    circle_layer_set_radius(internal->middle_dot_layer, 5);
    layer_add_child(this, internal->middle_dot_layer);

    return this;
}

void time_layer_destroy(TimeLayer *this) {
    logd("%s", __func__);
    Internal *internal = (Internal *) layer_get_data(this);

    circle_layer_destroy(internal->middle_dot_layer);
    hand_layer_destroy(internal->second_hand_layer);
    hand_layer_destroy(internal->hour_hand_layer);
    hand_layer_destroy(internal->minute_hand_layer);

    layer_destroy(this);
}

void time_layer_set_settings(TimeLayer *this, Settings *settings) {
    Internal *internal = (Internal *) layer_get_data(this);

    circle_layer_set_color(internal->middle_dot_layer, settings->color_hour_hand);
    hand_layer_set_color(internal->hour_hand_layer, settings->color_hour_hand);
    hand_layer_set_color(internal->minute_hand_layer, settings->color_minute_hand);
    hand_layer_set_color(internal->second_hand_layer, settings->color_second_hand);

    layer_set_hidden(internal->second_hand_layer, !settings->show_second_hand);

    layer_mark_dirty(this);
}
