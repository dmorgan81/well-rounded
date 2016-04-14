#pragma once
#include <pebble.h>

typedef Layer TickLayer;

TickLayer *tick_layer_create(GRect frame);
void tick_layer_destroy(TickLayer *this);
