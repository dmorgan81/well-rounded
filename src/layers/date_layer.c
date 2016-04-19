#include "../logging.h"
#include "date_layer.h"

typedef struct {
    Settings *settings;
    GFont font;
} Internal;

static void date_layer_update_proc(Layer *layer, GContext *ctx) {
    logd("%s", __func__);
    Internal *internal = (Internal *) layer_get_data(layer);
    Settings *settings = internal->settings;

    GRect bounds = layer_get_bounds(layer);
    GRect inset = grect_inset(bounds, GEdgeInsets(PBL_IF_ROUND_ELSE(10, 5)));

    graphics_context_set_text_color(ctx, settings->color_date);
    graphics_context_set_fill_color(ctx, settings->color_background);

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    int dir = settings->direction_date == DateFollowMonth ? t->tm_mon + 1 : settings->direction_date;
    int32_t angle = TRIG_MAX_ANGLE * dir / 12;
    GRect rect = grect_centered_from_polar(inset, GOvalScaleModeFitCircle, angle, GSize(20, 20));
    graphics_fill_rect(ctx, rect, 0, GCornerNone);

    char buffer[8];
    snprintf(buffer, sizeof(buffer), "%d", t->tm_mday);
    graphics_draw_text(ctx, buffer, internal->font, rect, GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
}

DateLayer *date_layer_create(GRect frame) {
    logd("%s", __func__);
    DateLayer *this = layer_create_with_data(frame, sizeof(Internal));
    layer_set_update_proc(this, date_layer_update_proc);
    Internal *internal = (Internal *) layer_get_data(this);
    internal->font = fonts_get_system_font(FONT_KEY_GOTHIC_14);
    return this;
}

void date_layer_destroy(DateLayer *this) {
    logd("%s", __func__);
    layer_destroy(this);
}

void date_layer_set_settings(DateLayer *this, Settings *settings) {
        logd("%s", __func__);
        Internal *internal = (Internal *) layer_get_data(this);
        internal->settings = settings;
        layer_set_hidden(this, !settings->show_date);
        layer_mark_dirty(this);
}
