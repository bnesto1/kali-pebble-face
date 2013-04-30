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

BmpContainer background_image;
TextLayer text_time_layer;
TextLayer text_date_layer;
TextLayer text_day_layer;
TextLayer text_month_layer;

void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Kali Linux");
  window_stack_push(&window, true /* Animated */);
  window_set_background_color(&window, GColorWhite);

  resource_init_current_app(&APP_RESOURCES);

  bmp_init_container(RESOURCE_ID_IMAGE_BACKGROUND, &background_image);
  layer_add_child(&window.layer, &background_image.layer.layer);

  text_layer_init(&text_day_layer, window.layer.frame);
  text_layer_set_text_color(&text_day_layer, GColorBlack);
  text_layer_set_background_color(&text_day_layer, GColorClear);
  layer_set_frame(&text_day_layer.layer, GRect(100, 5, 70, 25));
  text_layer_set_font(&text_day_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_21)));
  layer_add_child(&window.layer, &text_day_layer.layer);

  text_layer_init(&text_month_layer, window.layer.frame);
  text_layer_set_text_color(&text_month_layer, GColorBlack);
  text_layer_set_background_color(&text_month_layer, GColorClear);
  layer_set_frame(&text_month_layer.layer, GRect(100, 30, 70, 25));
  text_layer_set_font(&text_month_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_21)));
  layer_add_child(&window.layer, &text_month_layer.layer);

  text_layer_init(&text_date_layer, window.layer.frame);
  text_layer_set_text_color(&text_date_layer, GColorBlack);
  text_layer_set_background_color(&text_date_layer, GColorClear);
  layer_set_frame(&text_date_layer.layer, GRect(100, 55, 70, 25));
  text_layer_set_font(&text_date_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_BOLD_SUBSET_24)));
  layer_add_child(&window.layer, &text_date_layer.layer);

  text_layer_init(&text_time_layer, window.layer.frame);
  text_layer_set_text_color(&text_time_layer, GColorBlack);
  text_layer_set_background_color(&text_time_layer, GColorClear);
  layer_set_frame(&text_time_layer.layer, GRect(35, 120, 144-35, 168-120));
  text_layer_set_font(&text_time_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_BOLD_SUBSET_36)));
  layer_add_child(&window.layer, &text_time_layer.layer);

}

void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {

  (void)ctx;

  //PblTm currentTime;
  // Need to be static because they're used by the system later.
  static char time_text[] = "00:00";
  static char date_text[] = "00";
  static char day_text[] = "Xxx";
  static char month_text[] = "Xxx";
  //static char timeTextD[] = "Sat 28 - Apr - week 16 failsafe";
  char *time_format;


  // TODO: Only update the date when it's changed.
  string_format_time(month_text, sizeof(month_text), "%b", t->tick_time);
  text_layer_set_text(&text_month_layer, month_text);

  string_format_time(date_text, sizeof(date_text), "%e", t->tick_time);
  text_layer_set_text(&text_date_layer, date_text);

  //string_format_time(timeTextD, sizeof(timeTextD), "%a %d - %h - w %V", &currentTime);
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