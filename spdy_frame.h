#ifndef SPDY_FRAME_H
#define SPDY_FRAME_H

#include <inttypes.h>
#include "spdy_headers.h"

#define SPDY_DATA_FRAME 0
#define SPDY_CONTROL_FRAME 1

#define SPDY_FRAME_ERROR_ALREADY_PARSED -1
#define SPDY_FRAME_ERROR_INCOMPLETE -2

#define SPDY_CONTROL_SYN_STREAM 1
#define SPDY_CONTROL_SYN_REPLY 2
#define SPDY_CONTROL_RST_STREAM 3
#define SPDY_CONTROL_SETTINGS 4
#define SPDY_CONTROL_PING 6
#define SPDY_CONTROL_GOAWAY 7 

typedef struct
{
  // Common attributes
  char frame_type;
  uint8_t flags;
  uint32_t data_length;
  uint8_t *data;
  char parsed;

  // Control frame attributes
  uint16_t protocol_version;
  uint16_t control_frame_type;
  spdy_headers_t *headers;

  union {
    struct {
      uint32_t stream_id;
      uint32_t associated_stream_id;
      uint8_t priority;
      uint8_t slot;
    } syn_stream;

    struct {
      uint32_t stream_id;
    } syn_reply;

    struct {
      uint32_t stream_id;
      uint32_t status;
    } rst_stream;

    struct {
      uint32_t ping_id;
    } ping;

    struct {
      uint32_t last_good_stream_id;
    } goaway;
  } control_header;

  // Data frame attributes
  uint32_t stream_id;

} spdy_frame_t;

int spdy_frame_create(spdy_frame_t *frame);

int spdy_frame_parse(spdy_frame_t *frame, uint8_t *source, uint32_t source_len);
int spdy_frame_dump(spdy_frame_t *frame);

int spdy_frame_destroy(spdy_frame_t *frame);
#endif