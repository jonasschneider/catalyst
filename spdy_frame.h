#ifndef SPDY_FRAME_H
#define SPDY_FRAME_H

#include <inttypes.h>
#include "spdy_headers.h"

#define SPDY_DATA_FRAME 0
#define SPDY_CONTROL_FRAME 1

#define SPDY_FRAME_ERROR_INCOMPLETE -2
#define SPDY_FRAME_ERROR_CORRUPT_DATA -3

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

  // Control frame attributes
  uint16_t protocol_version;
  uint16_t control_frame_type;

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

void spdy_frame_create(spdy_frame_t *frame);

// Tries to parse a `frame` from the buffer `source` of length `source_len`.
// Returns
// - the number of consumed bytes on success, or
// - SPDY_FRAME_ERROR_INCOMPLETE if the buffer does not contain a complete frame.
int spdy_frame_parse(spdy_frame_t *frame, uint8_t *source, uint32_t source_len);

size_t spdy_frame_pack_syn_reply(uint8_t *dest, size_t dest_size, uint32_t stream_id, uint8_t *headers, size_t headers_len, uint8_t flags);
size_t spdy_frame_pack_rst_stream(uint8_t *dest, size_t dest_size, uint32_t stream_id, uint32_t status, uint8_t flags);
size_t spdy_frame_pack_ping(uint8_t *dest, size_t dest_size, uint32_t ping_id, uint8_t flags);
size_t spdy_frame_pack_data(uint8_t *dest, size_t dest_size, uint32_t stream_id, uint8_t *data, size_t data_len, uint8_t flags);
size_t spdy_frame_pack_goaway(uint8_t *dest, size_t dest_size, uint32_t last_good_stream_id, uint8_t flags);

// Pack the `frame` into the `dest` buffer. The size of the buffer is specified
// by `dest_size`. Return the number of bytes filled within the buffer,
// or 0 if the frame is too large to fit the buffer.
uint32_t spdy_frame_pack(spdy_frame_t *frame, uint8_t *dest, uint32_t dest_size);

// printf details about the `frame`.
void spdy_frame_dump(spdy_frame_t *frame);

// Frees the parsed frame data, if any. Only to be called when spdy_frame_parse was used.
void spdy_frame_destroy(spdy_frame_t *frame);
#endif