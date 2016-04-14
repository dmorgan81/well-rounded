#pragma once
#include <pebble.h>

typedef Layer HandLayer;

typedef int32_t (*HandLayerAngleCalc)(Layer *layer, struct tm *t);
typedef int16_t (*HandLayerLenCalc)(Layer *layer, GRect *bounds);

HandLayer *hand_layer_create(GRect frame);
void hand_layer_destroy(HandLayer *this);
void hand_layer_set_angle_calc(HandLayer *this, HandLayerAngleCalc angle_calc);
void hand_layer_set_len_calc(HandLayer *this, HandLayerLenCalc len_calc);
void hand_layer_set_stroke_width(HandLayer *this, uint8_t stroke_width);
