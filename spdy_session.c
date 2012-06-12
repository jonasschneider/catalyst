#include <string.h> // memcpy
#include <malloc.h>
#include <zlib.h>

#include "spdy_session.h"
#include "spdy_frame.h"
#include "catalyst.h"

int spdy_session_create(spdy_session_t *session)
{
  memset(session, 0, sizeof(spdy_session_t));
  spdy_headers_create(&session->last_frame_headers);
  spdy_frame_create(&session->last_frame);

  session->inflate_zstrm.zalloc = Z_NULL;
  session->inflate_zstrm.zfree = Z_NULL;
  session->inflate_zstrm.opaque = Z_NULL;
  session->inflate_zstrm.avail_in = 0;
  session->inflate_zstrm.next_in = Z_NULL;
  inflateInit(&session->inflate_zstrm);

  session->deflate_zstrm.zalloc = Z_NULL;
  session->deflate_zstrm.zfree = Z_NULL;
  session->deflate_zstrm.opaque = Z_NULL;
  session->deflate_zstrm.avail_in = 0;
  session->deflate_zstrm.next_in = Z_NULL;
  deflateInit(&session->deflate_zstrm, Z_DEFAULT_COMPRESSION);
}



int spdy_session_parse_next_frame(spdy_session_t *session)
{
  printf("avail_to_parse: %u\n", session->avail_to_parse);

  int res = spdy_frame_parse(&session->last_frame, (uint8_t*)session->parse_buffer, session->avail_to_parse);
  printf("spdy_frame_parse result: %d\n", res);
  if(res > 0)
  { // res then contains the length of the parsed frame

    // shift the parse buffer
    session->avail_to_parse -= res;
    memcpy(session->parse_buffer, session->parse_buffer+res, session->avail_to_parse);

    session->received_frame_count++;

    // discard old headers
    spdy_headers_destroy(&session->last_frame_headers);

    // check if we have to uncompress stuff
    if(session->last_frame.frame_type == SPDY_CONTROL_FRAME && session->last_frame.control_frame_type == SPDY_CONTROL_SYN_STREAM)
    {
      DEBUG1("inflating header\n");
      int hdr_offset = 10;
      int res = spdy_headers_inflate(&session->last_frame_headers, &session->inflate_zstrm, session->last_frame.data+hdr_offset, session->last_frame.data_length-hdr_offset);
      
      DEBUG2("spdy_headers_inflate result: %d\n", res);
      if(res) {
        return SPDY_SESSION_ERROR_PARSE_ERROR;
      }

      spdy_headers_dump(&session->last_frame_headers);
    }
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
  inflateEnd(&session->inflate_zstrm);
  deflateEnd(&session->deflate_zstrm);
  spdy_headers_destroy(&session->last_frame_headers);
}