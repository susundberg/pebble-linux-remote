#include <pebble.h>
#include "main.h"

typedef struct
{
  char key;
  uint8_t value;
} Mapping;

static time_t LOCAL_time_ping = 0;

static bool local_get_from_mapping( char id, const Mapping* mapping, int size, uint8_t* value )
{
  for ( int loop = 0; loop < size; loop ++ )
  {
    if ( mapping[loop].key == id )
    {
      *value = mapping[loop].value;
       return true;
    }
  }
  return false;
}


static bool local_get_button_id( char id, ButtonId* button_id ) 
{
  const Mapping mapping[] = { { 'U', BUTTON_ID_UP }, { 'D', BUTTON_ID_DOWN }, { 'S', BUTTON_ID_SELECT } };
  uint8_t value;
  if ( local_get_from_mapping( id, mapping, 3, &value ) == false )
    return false;
  *button_id = value;
  return true;
}


static bool local_get_click_type( char id, ClickType* type ) 
{
   uint8_t value;
   const Mapping mapping[] = { { 'S', CLICKTYPE_SINGLE }, { 'M', CLICKTYPE_MULTI }, { 'L', CLICKTYPE_LONG } };
   if ( local_get_from_mapping( id, mapping, 3, &value ) == false )
     return false;
   *type = value;
   return true;
}

static bool local_get_enabled( char id, bool* enabled ) 
{
   uint8_t value;
   const Mapping mapping[] = { { 'T', 1}, { 'F', 0 } };
   if ( local_get_from_mapping( id, mapping, 2, &value ) == false )
     return false;
   
   *enabled = (value != 0 );
   return true;
}



static void inbox_received_callback(DictionaryIterator* iter, void* context) 
{
  APP_LOG(APP_LOG_LEVEL_INFO, "Message received!");
  
  Tuple* ping_tuple = dict_find(iter, COMMUNICATION_KEY_PING );
  
  if ( ping_tuple != NULL )
  {
     LOCAL_time_ping = time( NULL );  
     return;
  }
  
  Tuple* config_tuple = dict_find(iter, COMMUNICATION_KEY_CONFIG );
  if ( config_tuple != NULL )
  {
    const char* value = config_tuple->value->cstring;
    
    ButtonId button_id;
    if (  local_get_button_id( value[0], &button_id ) == false )
      return;
    
    ClickType click_type;
    if ( local_get_click_type( value[1], &click_type ) == false )
      return ;
    
    bool enabled;
    if ( local_get_enabled( value[2], &enabled ) == false )
      return ;
    
    config_write( button_id, click_type, enabled );
    return;
  }
  
  APP_LOG(APP_LOG_LEVEL_ERROR, "Unknown message received!");
  
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) 
{
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

// static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
//   APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
// }

void communication_init()
{
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
//   app_message_register_outbox_sent(outbox_sent_callback);
  app_message_open(64, 64);
}

void communication_send_click( char key, char type )
{
  // Send app message
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  dict_write_uint8(iter, COMMUNICATION_KEY_BUTTON, key );
  dict_write_uint8(iter, COMMUNICATION_KEY_TYPE  , type );
  app_message_outbox_send();
}

time_t communication_last_ping()
{
    return LOCAL_time_ping;
}