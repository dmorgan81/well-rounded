#include "../logging.h"
#include "circle_layer.h"

typedef struct {
    uint16_t radius;
    GColor color;
} Internal;

static void circle_layer_update_proc(Layer *layer, GContext *ctx) {
    logd("%s", __func__);
    Internal *internal = (Internal *) layer_get_data(layer);

    GRect bounds = layer_get_bounds(layer);
    GPoint center = grect_center_point(&bounds);

    graphics_context_set_fill_color(ctx, internal->color);
    graphics_fill_circle(ctx, center, internal->radius);
}

CircleLayer *circle_layer_create(GRect frame) {
    logd("%s", __func__);
    CircleLayer *this = layer_create_with_data(frame, sizeof(Internal));
    layer_set_update_proc(this, circle_layer_update_proc);
    Internal *internal = (Internal *) layer_get_data(this);

    internal->radius = 0;
    internal->color = GColorWhite;

    return this;
}

void circle_layer_destroy(CircleLayer *this) {
    logd("%s", __func__);
    layer_destroy(this);
}

void circle_layer_set_radius(CircleLayer *this, uint16_t radius) {
    logd("%s", __func__);
    ((Internal *) layer_get_data(this))->radius = radius;
    layer_mark_dirty(this);
}

void circle_layer_set_color(CircleLayer *this, GColor color) {
    logd("%s", __func__);
    ((Internal *) layer_get_data(this))->color = color;
    layer_mark_dirty(this);
}
