

#include "main.h"




uint32_t local_get_id( ButtonId button_id, ClickType type )
{
  uint32_t int_type = (uint32_t)type;
  
   switch( button_id )
   {
     case BUTTON_ID_UP:
       return 0 * CLICKTYPE_LAST + int_type ;
     case BUTTON_ID_DOWN:
       return 1 * CLICKTYPE_LAST + int_type;
     case BUTTON_ID_SELECT:
       return 2 * CLICKTYPE_LAST + int_type;
     default:
       APP_LOG( APP_LOG_LEVEL_ERROR, "Invalid combination ID" ); 
       return 0;
   }
}

bool config_enabled( ButtonId button_id, ClickType type )
{
  uint32_t key_id = local_get_id( button_id, type );
  
  if (persist_exists(key_id)) 
    return persist_read_bool(key_id);
  return true ;  
}

void config_write( ButtonId button_id, ClickType type, bool enabled )
{
  uint32_t key_id = local_get_id( button_id, type );
  if ( persist_write_bool(key_id, enabled) != S_TRUE )
  {
      APP_LOG( APP_LOG_LEVEL_ERROR, "Writing config id %u failed!", (unsigned int)key_id ); 
  }
  APP_LOG( APP_LOG_LEVEL_INFO, "Write %d %d %d", (int)button_id, (int)type, (int)enabled ); 
}



