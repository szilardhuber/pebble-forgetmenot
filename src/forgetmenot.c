#include <pebble.h>
#include <time.h>

#define MINUTE 60
#define HOUR 60 * MINUTE
#define DAY 24 * HOUR

static Window *window;
static TextLayer *text_layer;
static size_t notification_interval = 1 * DAY;
// static size_t notification_interval = 5;

// forward declarations
static void schedule(int32_t id, const char* message);

// handlers

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Select");
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Up");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Down button pressed");
    text_layer_set_text(text_layer, "Down");
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create((GRect) { .origin = { 0, 12 }, .size = { bounds.size.w / 3 * 2, bounds.size.h } });
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_overflow_mode(text_layer, GTextOverflowModeTrailingEllipsis);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void notify(const char* message) {
    light_enable_interaction();
    vibes_short_pulse();
    text_layer_set_text(text_layer, message);
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

void set_message(int32_t cookie, const char * message)
{
    persist_write_string( cookie, message);
}


void get_message(int32_t cookie, char * message, const size_t buffer_size)
{
    persist_read_string(cookie, message, buffer_size);
}

static void on_wakeup(int32_t wakeupid, int32_t cookie)
{
    APP_LOG(APP_LOG_LEVEL_DEBUG, "App woke up, wakeupid: %ld, cookie: %ld", wakeupid, cookie);
    char * message = (char *)malloc(128);
    get_message(cookie, message, 128);
    schedule(wakeupid, message);
    notify(message);
    free(message);
}

static void schedule(int32_t id, const char* message)
{
    time_t now;
    time(&now);
    int32_t wakeup_id = wakeup_schedule(now + notification_interval, id, true);
    if (wakeup_id < 0)
    {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Failed to schedule wakeup. Reason: %ld", id);
    }
    else
    {
        set_message(id, message);
    }
}

static void init(void)
{
    wakeup_service_subscribe(on_wakeup);
    window = window_create();
    window_set_click_config_provider(window, click_config_provider);
    window_set_window_handlers(window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
    const bool animated = true;
    window_stack_push(window, animated);
    if (launch_reason() == APP_LAUNCH_WAKEUP)
    {
        int32_t wakeup_id;
        int32_t cookie;
        APP_LOG(APP_LOG_LEVEL_DEBUG, "App started due to wakeup event");
        wakeup_get_launch_event(&wakeup_id, &cookie);
        on_wakeup(wakeup_id, cookie);
    }
    else
    {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Scheduling wakeup");
        schedule(42, "Did you take your pills?");
    }
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
