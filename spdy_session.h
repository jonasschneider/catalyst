#ifndef SPDY_SESSION_H
#define SPDY_SESSION_H
#include <inttypes.h>
#include "spdy_frame.h"

#define SPDY_SESSION_PARSE_BUFFER_SIZE 8192
#define SPDY_SESSION_FRAME_PACK_BUFFER_SIZE 8192
#define SPDY_SESSION_SEND_QUEUE_MAXLENGTH 32

#define SPDY_SESSION_ERROR_NEED_MORE -1
#define SPDY_SESSION_ERROR_PARSE_ERROR -2

struct _spdy_send_queue_item
{
  size_t length;
  uint8_t *data;
};

typedef struct
{
  uint8_t parse_buffer[SPDY_SESSION_PARSE_BUFFER_SIZE];
  size_t avail_to_parse;
  z_stream inflate_zstrm;
  z_stream deflate_zstrm;

  uint32_t send_queue_length;
  uint32_t send_queue_front;
  struct _spdy_send_queue_item send_queue[SPDY_SESSION_SEND_QUEUE_MAXLENGTH];

  spdy_frame_t last_frame;
  spdy_headers_t last_frame_headers;

  uint32_t received_frame_count;

} spdy_session_t;

void spdy_session_create(spdy_session_t *session);
int spdy_session_parse_next_frame(spdy_session_t *session);

int spdy_session_send_syn_reply(spdy_session_t *session, uint32_t stream_id, spdy_headers_t *headers, uint8_t flags);
int spdy_session_send_rst_stream(spdy_session_t *session, uint32_t stream_id, uint32_t status, uint8_t flags);
int spdy_session_send_data(spdy_session_t *session, uint32_t stream_id, uint8_t *data, size_t data_len, uint8_t flags);

int spdy_session_queue_frame(spdy_session_t *session, uint8_t *data, size_t length);
uint8_t *spdy_session_unqueue_frame(spdy_session_t *session, size_t *length);

void spdy_session_destroy(spdy_session_t *session);
# endif