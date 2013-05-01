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
TextLayer text_time_layer;	// Time layer
TextLayer text_date_layer;	// Numerical date layer
TextLayer text_day_layer;	// Day of the week layer
TextLayer text_month_layer;	// Month layer

void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Kali Linux");
  window_stack_push(&window, true /* Animated */);
  window_set_background_color(&window, GColorBlack);	// Set the background to black

  resource_init_current_app(&APP_RESOURCES);		// Initial our resources

  bmp_init_container(RESOURCE_ID_IMAGE_BACKGROUND_BLK, &background_image);	// Assign the background image
  layer_add_child(&window.layer, &background_image.layer.layer);	// Draw the background

  /* Make the layer for the day of the week.
  Positioned at 5, 140 with size of 40 x 28 */
  text_layer_init(&text_day_layer, window.layer.frame);
  text_layer_set_text_color(&text_day_layer, GColorWhite);		// Set the font colour to white
  text_layer_set_background_color(&text_day_layer, GColorClear);	// Make the layer's background transparent
  layer_set_frame(&text_day_layer.layer, GRect(5, 140, 40, 28));	// Layer at 5, 140 with size 40 x 28
  text_layer_set_font(&text_day_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_21)));	// Custom font
  layer_add_child(&window.layer, &text_day_layer.layer);

  /* Make the layer for the month.
  Positioned at 109-45, 140 with size of 40 x 28 */
  text_layer_init(&text_month_layer, window.layer.frame);
  text_layer_set_text_color(&text_month_layer, GColorWhite);		// Set the font colour to white
  text_layer_set_background_color(&text_month_layer, GColorClear);	// Make the layer's background transparent
  layer_set_frame(&text_month_layer.layer, GRect(109-45, 140, 40, 28));	// Layer at 109-45,140 with size 40 x 28
  text_layer_set_font(&text_month_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_21)));	// Custom font
  layer_add_child(&window.layer, &text_month_layer.layer);

  /* Make the layer for the date.
  Positioned at 109, 140 with size of 30 x 28 */
  text_layer_init(&text_date_layer, window.layer.frame);
  text_layer_set_text_color(&text_date_layer, GColorWhite);		// Set the font colour to white
  text_layer_set_background_color(&text_date_layer, GColorClear);	// Make the layer's background transparent
  layer_set_frame(&text_date_layer.layer, GRect(109, 140, 30, 28));	// Layer at 109, 140 with size of 30 x 28
  text_layer_set_font(&text_date_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_21)));	// Custom font
  layer_add_child(&window.layer, &text_date_layer.layer);

  /* Make the time layer.
  Positioned at 5, 120 with size of 139 x 48 */
  text_layer_init(&text_time_layer, window.layer.frame);
  text_layer_set_text_color(&text_time_layer, GColorWhite);		// Set the font colour to white
  text_layer_set_background_color(&text_time_layer, GColorClear);	// Make the layer's background transparent
  text_layer_set_text_alignment(&text_time_layer, GTextAlignmentCenter);	// Centre within the frame
  layer_set_frame(&text_time_layer.layer, GRect(5, 100, 139, 48));	// Layer at 5, 120 with size of 139 x 48
  text_layer_set_font(&text_time_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_BOLD_SUBSET_36)));	// Custom font
  layer_add_child(&window.layer, &text_time_layer.layer);

}

void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {

  (void)ctx;

  // Need to be static because they're used by the system later.
  static char time_text[] = "00:00";
  static char date_text[] = "00";
  static char day_text[] = "Xxx";
  static char month_text[] = "Xxx";
  char *time_format;


  // Get the 3-letter abbreviated month
  string_format_time(month_text, sizeof(month_text), "%b", t->tick_time);
  text_layer_set_text(&text_month_layer, month_text);

  // Get just the number of the date
  string_format_time(date_text, sizeof(date_text), "%e", t->tick_time);
  text_layer_set_text(&text_date_layer, date_text);

  // Get the 3-letter abbreviated day of the week
  string_format_time(day_text, sizeof(day_text), "%a", t->tick_time);
  text_layer_set_text(&text_day_layer, day_text);

  if (clock_is_24h_style()) {
    time_format = "%R";
  } else {
    time_format = "%I:%M";
  }

  string_format_time(time_text, sizeof(time_text), time_format, t->tick_time);

  // Kludge to handle lack of non-padded hour format string
  // for twelve hour clock.
  if (!clock_is_24h_style() && (time_text[0] == '0')) {
    memmove(time_text, &time_text[1], sizeof(time_text) - 1);
  }

  text_layer_set_text(&text_time_layer, time_text);

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
