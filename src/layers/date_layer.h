#pragma once
#include <pebble.h>
#include "../settings.h"

typedef Layer DateLayer;

DateLayer *date_layer_create(GRect frame);
void date_layer_destroy(DateLayer *this);
void date_layer_set_settings(DateLayer *this, Settings *settings);
