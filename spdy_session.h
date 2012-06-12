#ifndef SPDY_SESSION_H
#define SPDY_SESSION_H
#include <inttypes.h>
#include "spdy_frame.h"

#define SPDY_SESSION_PARSE_BUFFER_SIZE 8192

#define SPDY_SESSION_ERROR_NEED_MORE -1
#define SPDY_SESSION_ERROR_PARSE_ERROR -2

typedef struct
{
  uint8_t parse_buffer[SPDY_SESSION_PARSE_BUFFER_SIZE];
  size_t avail_to_parse;
  z_stream inflate_zstrm;
  z_stream deflate_zstrm;

  spdy_frame_t last_frame;
  spdy_headers_t last_frame_headers;

  uint32_t received_frame_count;

} spdy_session_t;

int spdy_session_create(spdy_session_t *session);
int spdy_session_parse_next_frame(spdy_session_t *session);
void spdy_session_destroy(spdy_session_t *session);
# endif