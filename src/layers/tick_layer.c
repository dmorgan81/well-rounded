#include "tick_layer.h"

static void tick_layer_update_proc(Layer *layer, GContext *ctx) {
    GRect bounds = layer_get_bounds(layer);
    GRect inset = grect_inset(bounds, GEdgeInsets(5));

    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_fill_rect(ctx, bounds, 0, GCornerNone);

    graphics_context_set_fill_color(ctx, GColorWhite);
    for (int i = 0; i < 12; i++) {
        int32_t angle = TRIG_MAX_ANGLE * i / 12;
        GPoint point = gpoint_from_polar(inset, GOvalScaleModeFitCircle, angle);
        graphics_fill_circle(ctx, point, (i % 3 == 0 ? 2 : 1));
    }
}

TickLayer *tick_layer_create(GRect frame) {
    TickLayer *this = layer_create(frame);
    layer_set_update_proc(this, tick_layer_update_proc);
    return this;
}

void tick_layer_destroy(TickLayer *this) {
    layer_destroy(this);
}
