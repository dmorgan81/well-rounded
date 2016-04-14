#pragma once
#include <pebble.h>

typedef Layer CircleLayer;

CircleLayer *circle_layer_create();
void circle_layer_destroy(CircleLayer *this);
void circle_layer_set_radius(CircleLayer *this, uint16_t radius);
void circle_layer_set_color(CircleLayer *this, GColor color);
