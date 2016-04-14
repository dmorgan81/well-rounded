#pragma once
#include <pebble.h>

typedef Layer SecondHandLayer;

SecondHandLayer *second_hand_layer_create(GRect frame);
void second_hand_layer_destroy(SecondHandLayer *this);
