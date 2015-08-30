#ifndef MAIN_H
#define MAIN_H

#include <pebble.h>

/** Functions implemented in clicks.c */

/// Config click provider for the window
void click_config_provider( );

/// Get timestamp on last click ;
time_t click_last_time();


/** Functions implemented in main_window.c */

/// Update the window to display elapsed time since start
void main_window_update_elapsed( time_t time_now  );

/// Set text to given string
void main_window_text( unsigned int id, const char* text, unsigned int len );
/// Set the current status
void main_window_set_status( bool connected );

/// Window loading function
void main_window_unload(Window *window) ;
void main_window_load(Window *window) ;

typedef enum
{
  CLICKTYPE_SINGLE = 0,
  CLICKTYPE_LONG = 1,
  CLICKTYPE_MULTI = 2,
  CLICKTYPE_LAST = 3
} ClickType;

/** Functions implemented in config.c */
/// @return is the action enabled
bool config_enabled( ButtonId button_id, ClickType type );
/// Write config to storage
void config_write( ButtonId button_id, ClickType type, bool enabled );


/** Functions implemented in communication.c */

#define COMMUNICATION_KEY_PING   100
#define COMMUNICATION_KEY_CONFIG 200
#define COMMUNICATION_KEY_TYPE   1
#define COMMUNICATION_KEY_BUTTON 2

// Get timestamp on the last ping message
time_t communication_last_ping();
/// Init
void communication_init();
/// Send click event 
void communication_send_click( char key, char type );

#define CLICK_TIMER_DELAY_S      5

#define THRESHOLD_COMMUNICATION_TIMEOUT_SEC 15
#endif