

#include <pebble.h>
#include "main.h"

static time_t LOCAL_time_last_click   = 0;

static void local_click_handler( const char* type, const char* button )
{
  communication_send_click( button[0], type[0] );
  

  // Update the text
  main_window_text( 0, "Button", 7 );
  main_window_text( 1, button, strlen(button) + 1 );
  main_window_text( 2, "Click", 6  );
  main_window_text( 3, type, strlen(type) + 1 );
  
  APP_LOG( APP_LOG_LEVEL_DEBUG, "Clicked .. ");
  LOCAL_time_last_click  = time( NULL );
}




static void local_click_handler_single( ClickRecognizerRef recognizer, void *context) 
{
  local_click_handler( "SINGLE" , (const char*)context );
}

static void local_click_handler_long( ClickRecognizerRef recognizer, void *context) 
{
  local_click_handler( "LONG" , (const char*)context );
}

static void local_click_handler_multi( ClickRecognizerRef recognizer, void *context) 
{
  local_click_handler( "MULTI" , (const char*)context );
}


static void local_click_config_provider_wrapper(  ButtonId button_id, void* context ) 
{ 
  if ( config_enabled( button_id, CLICKTYPE_SINGLE ) )
     window_single_click_subscribe( button_id, local_click_handler_single ); // single click
  if ( config_enabled( button_id, CLICKTYPE_LONG ) )
     window_long_click_subscribe( button_id, 0, NULL, local_click_handler_long ); // long click, call on up
  if ( config_enabled( button_id, CLICKTYPE_MULTI ) )
     window_multi_click_subscribe( button_id, 2, 2, 0, true, local_click_handler_multi ); // double click, call on last
     
  window_set_click_context( button_id, context);
}


void click_config_provider( )
{
  local_click_config_provider_wrapper( BUTTON_ID_UP, "UP" );
  local_click_config_provider_wrapper( BUTTON_ID_SELECT, "SEL" );
  local_click_config_provider_wrapper( BUTTON_ID_DOWN, "DOWN" );
}

time_t click_last_time()
{
  return LOCAL_time_last_click;
}