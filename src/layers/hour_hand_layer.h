#pragma once
#include <pebble.h>

typedef Layer HourHandLayer;

HourHandLayer *hour_hand_layer_create(GRect frame);
void hour_hand_layer_destroy(HourHandLayer *this);
