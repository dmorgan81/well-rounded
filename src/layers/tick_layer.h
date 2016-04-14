#pragma once
#include <pebble.h>
#include "../settings.h"

typedef Layer TickLayer;

TickLayer *tick_layer_create(GRect frame);
void tick_layer_destroy(TickLayer *this);
void tick_layer_set_color(TickLayer *this, GColor color);
