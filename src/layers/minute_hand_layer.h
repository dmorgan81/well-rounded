#pragma once
#include <pebble.h>

typedef Layer MinuteHandLayer;

MinuteHandLayer *minute_hand_layer_create(GRect frame);
void minute_hand_layer_destroy(MinuteHandLayer *this);
