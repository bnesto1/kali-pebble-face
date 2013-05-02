#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#define MY_UUID {0xA4, 0x1B, 0xB0, 0xE2, 0xD2, 0x62, 0x4E, 0xA6, 0xAA, 0x30, 0xED, 0xBE, 0x01, 0xE3, 0x8A, 0x02}
PBL_APP_INFO(MY_UUID,
             "Kali", "Kali Linux",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_WATCH_FACE);

Window window;

BmpContainer background_image;	// Background image container
TextLayer text_hour_layer;	// Hour layer
TextLayer text_minute_layer;	// Minute layer
Layer line_layer;		// Layer for the horizontal line

/*
TextLayer text_date_layer;	// Numerical date layer
TextLayer text_day_layer;	// Day of the week layer
TextLayer text_month_layer;	// Month layer
*/

void line_layer_update_callback(Layer *me, GContext* ctx) {
  (void)me;

  graphics_context_set_stroke_color(ctx, GColorWhite);	// Line will be white

  // Draw 2 lines so it's actually visible
  graphics_draw_line(ctx, GPoint(12, 115), GPoint(132, 115));
  graphics_draw_line(ctx, GPoint(12, 116), GPoint(132, 116));
}


void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Kali Linux");
  window_stack_push(&window, true /* Animated */);
  window_set_background_color(&window, GColorBlack);	// Set the background to black

  resource_init_current_app(&APP_RESOURCES);		// Initial our resources

  bmp_init_container(RESOURCE_ID_IMAGE_BACKGROUND_BLK, &background_image);	// Assign the background image
  layer_add_child(&window.layer, &background_image.layer.layer);	// Draw the background

/*
  // Make the layer for the day of the week.
  text_layer_init(&text_day_layer, window.layer.frame);
  text_layer_set_text_color(&text_day_layer, GColorWhite);		// Set the font colour to white
  text_layer_set_background_color(&text_day_layer, GColorClear);	// Make the layer's background transparent
  layer_set_frame(&text_day_layer.layer, GRect(5, 140, 40, 28));	// Layer at 5, 140 with size 40 x 28
  text_layer_set_font(&text_day_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_21)));	// Custom font
  layer_add_child(&window.layer, &text_day_layer.layer);
*/

/*
  // Make the layer for the month.
  text_layer_init(&text_month_layer, window.layer.frame);
  text_layer_set_text_color(&text_month_layer, GColorWhite);		// Set the font colour to white
  text_layer_set_background_color(&text_month_layer, GColorClear);	// Make the layer's background transparent
  layer_set_frame(&text_month_layer.layer, GRect(109-45, 140, 40, 28));	// Layer at 109-45,140 with size 40 x 28
  text_layer_set_font(&text_month_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_21)));	// Custom font
  layer_add_child(&window.layer, &text_month_layer.layer);
*/

/*
  // Make the layer for the date.
  text_layer_init(&text_date_layer, window.layer.frame);
  text_layer_set_text_color(&text_date_layer, GColorWhite);		// Set the font colour to white
  text_layer_set_background_color(&text_date_layer, GColorClear);	// Make the layer's background transparent
  layer_set_frame(&text_date_layer.layer, GRect(109, 140, 30, 28));	// Layer at 109, 140 with size of 30 x 28
  text_layer_set_font(&text_date_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_21)));	// Custom font
  layer_add_child(&window.layer, &text_date_layer.layer);
*/


  // Make the hour layer.
  text_layer_init(&text_hour_layer, window.layer.frame);
  text_layer_set_text_color(&text_hour_layer, GColorWhite);		// Set the font colour to white
  text_layer_set_background_color(&text_hour_layer, GColorClear);	// Make the layer's background transparent
  layer_set_frame(&text_hour_layer.layer, GRect(12, 120, 60, 40));	// Layer at 5, 120 with size of 139 x 48
  text_layer_set_font(&text_hour_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_BORIS_BLACKBLOXX_40)));	// Custom font
  layer_add_child(&window.layer, &text_hour_layer.layer);

  // Make the minute layer
  text_layer_init(&text_minute_layer, window.layer.frame);
  text_layer_set_text_color(&text_minute_layer, GColorWhite);             // Set the font colour to white
  text_layer_set_background_color(&text_minute_layer, GColorClear);       // Make the layer's background transparent
  layer_set_frame(&text_minute_layer.layer, GRect(72, 120, 60, 40));      // Layer at 5, 120 with size of 139 x 48
  text_layer_set_font(&text_minute_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_BORIS_BLACKBLOXX_40)));     // Custom font
  layer_add_child(&window.layer, &text_minute_layer.layer);

  // Make the line layer
  layer_init(&line_layer, window.layer.frame);
  line_layer.update_proc = &line_layer_update_callback;
  layer_add_child(&window.layer, &line_layer);

}

void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {

  (void)ctx;

  // Need to be static because they're used by the system later.
  static char hour_text[] = "00";
  static char minute_text[] = "00";
  static char date_text[] = "00";
  static char day_text[] = "Xxx";
  static char month_text[] = "Xxx";
  char *hour_format;
  char *minute_format;

  // Get the 3-letter abbreviated month
//  string_format_time(month_text, sizeof(month_text), "%b", t->tick_time);
//  text_layer_set_text(&text_month_layer, month_text);

  // Get just the number of the date
//  string_format_time(date_text, sizeof(date_text), "%e", t->tick_time);
//  text_layer_set_text(&text_date_layer, date_text);

  // Get the 3-letter abbreviated day of the week
//  string_format_time(day_text, sizeof(day_text), "%a", t->tick_time);
//  text_layer_set_text(&text_day_layer, day_text);

  hour_format = "%H";
  minute_format = "%M";

  string_format_time(hour_text, sizeof(hour_text), hour_format, t->tick_time);
  string_format_time(minute_text, sizeof(minute_text), minute_format, t->tick_time);

  // Kludge to handle lack of non-padded hour format string
  // for twelve hour clock.
  if (!clock_is_24h_style() && (hour_text[0] == '0')) {
    memmove(hour_text, &hour_text[1], sizeof(hour_text) - 1);
  }

  text_layer_set_text(&text_hour_layer, hour_text);
  text_layer_set_text(&text_minute_layer, minute_text);

}

/*
char *upcase(char *str)
{
    char *s = str;

    while (*s)
    {
        *s++ = toupper((int)*s);
    }

    return str;
}
*/

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,

    .tick_info = {
      .tick_handler = &handle_minute_tick,
      .tick_units = MINUTE_UNIT
    }

  };
  app_event_loop(params, &handlers);
}
