#include <string.h> // memcpy
#include <malloc.h>
#include <zlib.h>

#include "spdy_session.h"
#include "spdy_frame.h"

int spdy_session_create(spdy_session_t *session)
{
  memset(session, 0, sizeof(spdy_session_t));

  session->inflate_zstrm.zalloc = Z_NULL;
  session->inflate_zstrm.zfree = Z_NULL;
  session->inflate_zstrm.opaque = Z_NULL;
  session->inflate_zstrm.avail_in = 0;
  session->inflate_zstrm.next_in = Z_NULL;
  inflateInit(&session->inflate_zstrm);
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
    // discard old headers
    spdy_headers_destroy(&session->last_frame_headers);

    // res contains the length of the parsed frame
    session->received_frame_count++;

    size_t remaining = session->avail_to_parse - res;
    session->avail_to_parse -= res;

    memcpy(session->parse_buffer, session->parse_buffer+res, remaining);

    // if(compressed_headers_at_offset >= 0)
    // {
    //   frame->headers = malloc(sizeof(spdy_headers_t));
    //   int res = spdy_headers_inflate(frame->headers, &source[8+compressed_headers_at_offset], frame->data_length-compressed_headers_at_offset);
      
    //   DEBUG2("spdy_headers_inflate result: %d\n", res);
    //   if(res) {
    //     free(frame->headers);
    //     return SPDY_FRAME_ERROR_CORRUPT_DATA;
    //   }
    // }
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
  spdy_headers_destroy(&session->last_frame_headers);
}