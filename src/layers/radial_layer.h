#pragma once
#include <pebble.h>

typedef Layer RadialLayer;

RadialLayer *radial_layer_create(GRect frame);
void radial_layer_destroy(RadialLayer *this);
void radial_layer_set_value(RadialLayer *this, int32_t value);
void radial_layer_set_max(RadialLayer *this, int32_t max);
void radial_layer_set_inset(RadialLayer *this, uint16_t inset);
void radial_layer_set_color(RadialLayer *this, GColor color);
