#include <string.h> // memcpy
#include <malloc.h>

#include "spdy_session.h"
#include "spdy_frame.h"

// typedef struct
// {
//   size_t parse_buffer_size;
//   uint8_t *parse_buffer;
//   size_t avail_to_parse;
//   uint32_t received_frame_count;
// } spdy_session_t;

int spdy_session_create(spdy_session_t *session)
{
  memset(session, 0, sizeof(spdy_session_t));
}



int spdy_session_parse_next_frame(spdy_session_t *session)
{
  spdy_frame_t frame;
  spdy_frame_create(&frame);

  printf("avail_to_parse: %u\n", session->avail_to_parse);

  int res = spdy_frame_parse(&frame, (uint8_t*)session->parse_buffer, session->avail_to_parse);
  printf("spdy_frame_parse result: %d\n", res);
  if(res > 0)
  {
    session->received_frame_count = 1;
    // todo: shift parse buffer
    return 0;
  }
  else if(res == SPDY_FRAME_ERROR_INCOMPLETE)
  {
    return SPDY_SESSION_ERROR_NEED_MORE;
  }
  else
  {
    return SPDY_SESSION_ERROR_PARSE_ERROR;
  }
}



void spdy_session_destroy(spdy_session_t *session)
{
}