#include "../logging.h"
#include "time_layer.h"
#include "second_hand_layer.h"
#include "minute_hand_layer.h"
#include "hour_hand_layer.h"

typedef struct {
    MinuteHandLayer *minute_hand_layer;
    HourHandLayer *hour_hand_layer;
    SecondHandLayer *second_hand_layer;
} Internal;

TimeLayer *time_layer_create(GRect frame) {
    logd("%s", __func__);
    TimeLayer *this = layer_create_with_data(frame, sizeof(Internal));
    Internal *internal = (Internal *) layer_get_data(this);

    internal->minute_hand_layer = minute_hand_layer_create(frame);
    layer_add_child(this, internal->minute_hand_layer);

    internal->hour_hand_layer = hour_hand_layer_create(frame);
    layer_add_child(this, internal->hour_hand_layer);

    internal->second_hand_layer = second_hand_layer_create(frame);
    layer_add_child(this, internal->second_hand_layer);

    return this;
}

void time_layer_destroy(TimeLayer *this) {
    logd("%s", __func__);
    Internal *internal = (Internal *) layer_get_data(this);

    second_hand_layer_destroy(internal->second_hand_layer);
    hour_hand_layer_destroy(internal->hour_hand_layer);
    minute_hand_layer_destroy(internal->minute_hand_layer);

    layer_destroy(this);
}
