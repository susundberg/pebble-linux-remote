/**
 * Main file for the pebble app -- linux remote.
 * 
 * @author Pauli Salmenrinne, 2015
 */

#include <pebble.h>
#include "main.h"


static Window *LOCAL_window_main;


static void local_time_event_handler(struct tm *tick_time, TimeUnits units_changed)
{
   time_t time_now = time( NULL );
   

   // wait for while after click before updating the display
   if ( ( time_now - click_last_time() ) < 2 )
      return;
   
   if ( ( time_now - communication_last_ping() > THRESHOLD_COMMUNICATION_TIMEOUT_SEC ) )
      main_window_set_status( false );  
   else
      main_window_set_status( true );
   
   main_window_update_elapsed( time_now );
   
}


static void local_init() 
{
  
  tick_timer_service_subscribe( SECOND_UNIT , local_time_event_handler );
  
  LOCAL_window_main = window_create();
  
  window_set_window_handlers(LOCAL_window_main, (WindowHandlers) {
    .load   = main_window_load,
    .unload = main_window_unload
    });
  
  window_set_click_config_provider(LOCAL_window_main, click_config_provider);
  
  window_stack_push(LOCAL_window_main, true);
  communication_init();  
}

static void local_deinit() 
{
  window_destroy(LOCAL_window_main);
  tick_timer_service_unsubscribe();
}

int main(void) 
{
  local_init();
  app_event_loop();
  local_deinit();
}


