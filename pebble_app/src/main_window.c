#include "main.h"


#define BUFFER_SIZE 16 

static TextLayer* LOCAL_layers[4];
static time_t LOCAL_time_start = 0;
static int8_t LOCAL_last_status = -1;

char LOCAL_layers_text[4][BUFFER_SIZE];

static void local_create_layer( Layer* window_layer, GRect* window_bounds, unsigned int index  )
{
  unsigned int nlarge = index/2;     // 0,0,1,1
  unsigned int nsmall = (index+1)/2; // 0,1,1,2,
  
  // total of 1 + 3, 1 + 3 == 8 + 1 for borders
  
  unsigned int layer_size_small = (window_bounds->size.h)/8;
  unsigned int layer_size_big   = layer_size_small*3;
  
  unsigned int layer_size =  layer_size_small ;

  if ( (index&0x01) == 1 )
    layer_size = layer_size_big;
  
  
  unsigned int offset_h =  nsmall * layer_size_small + nlarge * layer_size_big;
  TextLayer* tlayer = text_layer_create(GRect(0, offset_h, window_bounds->size.w, layer_size ));
  
  APP_LOG( APP_LOG_LEVEL_DEBUG, "Created layer %d %d", layer_size, offset_h );  
  if (layer_size == layer_size_big )
  {
     text_layer_set_font( tlayer , fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  }
  else 
  {
     text_layer_set_font( tlayer , fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  }
  
  layer_add_child(window_layer, text_layer_get_layer(tlayer ));
  LOCAL_layers[ index ] = tlayer; 
}


void main_window_load(Window *window) 
{
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  LOCAL_time_start = time( NULL );
  
  for ( int loop = 0; loop < 4; loop ++ )
     local_create_layer( window_layer, &window_bounds, loop );
  
  main_window_set_status( false );
  main_window_text( 1, "HELLO", 5 ); 
}

void main_window_unload(Window *window) 
{
  // Destroy TextLayer
  for ( int loop = 0; loop < 4; loop ++ )
     text_layer_destroy(LOCAL_layers[loop]);
}


static void local_get_time_since( time_t passed_time, unsigned int* min, unsigned* sec )
{
  *min = passed_time/60;
  *sec = passed_time - (*min)*60;
  if ( *min > 99 )
  {
    *min = 99;
    *sec = 99;
  }
}


void main_window_set_status( bool connected )
{
  
  if ( LOCAL_last_status == connected )
    return;
  
  if ( LOCAL_last_status >= 0 )
    vibes_short_pulse();
  
  LOCAL_last_status = connected;
  
  GColor front = GColorWhite;
  GColor back  = GColorBlack;
  
  if ( connected == true )
  {
     back  = GColorWhite;
     front = GColorBlack;
  }
  
  for ( int loop = 0; loop < 4; loop ++ )
  {
    text_layer_set_background_color( LOCAL_layers[loop], back );
    text_layer_set_text_color( LOCAL_layers[loop] , front );
  }  
}


void main_window_text( unsigned int id, const char* text, unsigned int len )
{
  if ( len >= BUFFER_SIZE )
  {
     APP_LOG( APP_LOG_LEVEL_ERROR, "Invalid update" );
     return;
  }
  
  memcpy( LOCAL_layers_text[id], text, len );
  text_layer_set_text( LOCAL_layers[id], LOCAL_layers_text[id] );
}
 
void main_window_update_elapsed( time_t time_now )
{
   unsigned int min_start;
   unsigned int sec_start;
   
   char buffer[6];
   // long enough, update the display
   local_get_time_since( time_now - LOCAL_time_start, &min_start, &sec_start );
   
   if ( click_last_time() > 0 ) 
   {
      unsigned int min_click;
      unsigned int sec_click;
      
      main_window_text( 0, "Since click:", 13 );
      local_get_time_since( time_now - click_last_time(), &min_click, &sec_click );
      snprintf( buffer, 6, "%02u:%02u", min_click, sec_click );
      main_window_text( 1, buffer, 6 );
   }
   else 
   {
      main_window_text( 0, "No clicks", 14 );
      main_window_text( 1, "  -  ", 6 );
   }
   
   main_window_text( 2, "Since start:", 13);
   snprintf( buffer, 6, "%02u:%02u", min_start, sec_start  );
   main_window_text( 3, buffer, 6 );
}