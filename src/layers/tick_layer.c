#include "../logging.h"
#include "tick_layer.h"

typedef struct {
    GColor color;
} Internal;

static void tick_layer_update_proc(Layer *layer, GContext *ctx) {
    logd("%s", __func__);
    Internal *internal = (Internal *) layer_get_data(layer);

    GRect bounds = layer_get_bounds(layer);
    GRect inset = grect_inset(bounds, GEdgeInsets(5));

    graphics_context_set_fill_color(ctx, internal->color);
    for (int i = 0; i < 12; i++) {
        int32_t angle = TRIG_MAX_ANGLE * i / 12;
        GPoint point = gpoint_from_polar(inset, GOvalScaleModeFitCircle, angle);
        graphics_fill_circle(ctx, point, (i % 3 == 0 ? 2 : 1));
    }
}

TickLayer *tick_layer_create(GRect frame) {
    logd("%s", __func__);
    TickLayer *this = layer_create_with_data(frame, sizeof(Internal));
    layer_set_update_proc(this, tick_layer_update_proc);
    return this;
}

void tick_layer_destroy(TickLayer *this) {
    logd("%s", __func__);
    layer_destroy(this);
}

void tick_layer_set_color(TickLayer *this, GColor color) {
    logd("%s", __func__);
    ((Internal *) layer_get_data(this))->color = color;
    layer_mark_dirty(this);
}
