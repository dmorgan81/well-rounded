#pragma once
#include <pebble.h>
#include "../settings.h"

typedef Layer TimeLayer;

TimeLayer *time_layer_create(GRect frame);
void time_layer_destroy(TimeLayer *this);
void time_layer_set_settings(TimeLayer *this, Settings *settings);
