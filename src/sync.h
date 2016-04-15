#pragma once
#include <pebble.h>
#include "settings.h"

void sync_init(Settings *settings, AppSyncTupleChangedCallback sync_changed_handler);
void sync_deinit();
