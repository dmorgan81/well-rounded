#include "../logging.h"
#include "radial_layer.h"

typedef struct {
    int32_t max;
    int32_t value;
    uint16_t inset;
    GColor color;
} Internal;

void radial_layer_update_proc(Layer *layer, GContext *ctx) {
    logd("%s", __func__);
    GRect bounds = layer_get_bounds(layer);
    Internal *internal = (Internal *) layer_get_data(layer);

    int32_t angle = TRIG_MAX_ANGLE * internal->value / internal->max;
    graphics_context_set_fill_color(ctx, internal->color);
    graphics_fill_radial(ctx, bounds, GOvalScaleModeFitCircle, internal->inset, 0, angle);
}

RadialLayer *radial_layer_create(GRect frame) {
    logd("%s", __func__);
    RadialLayer *this = layer_create_with_data(frame, sizeof(Internal));
    layer_set_update_proc(this, radial_layer_update_proc);
    Internal *internal = (Internal *) layer_get_data(this);

    internal->max = 0;
    internal->value = 0;
    internal->inset = 0;
    internal->color = GColorWhite;

    return this;
}

void radial_layer_destroy(RadialLayer *this) {
    logd("%s", __func__);
    layer_destroy(this);
}

void radial_layer_set_value(RadialLayer *this, int32_t value) {
    logd("%s", __func__);
    ((Internal *) layer_get_data(this))->value = value;
    layer_mark_dirty(this);
}

void radial_layer_set_max(RadialLayer *this, int32_t max) {
    logd("%s", __func__);
    ((Internal *) layer_get_data(this))->max = max;
    layer_mark_dirty(this);
}

void radial_layer_set_inset(RadialLayer *this, uint16_t inset) {
    logd("%s", __func__);
    ((Internal *) layer_get_data(this))->inset = inset;
    layer_mark_dirty(this);
}

void radial_layer_set_color(RadialLayer *this, GColor color) {
    logd("%s", __func__);
    ((Internal *) layer_get_data(this))->color = color;
    layer_mark_dirty(this);
}
