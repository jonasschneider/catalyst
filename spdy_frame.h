#include <inttypes.h>
#include "spdy_headers.h"

#define SPDY_DATA_FRAME 0
#define SPDY_CONTROL_FRAME 1

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

  // Data frame attributes
  uint32_t stream_id;

} spdy_frame_t;

int spdy_frame_parse(spdy_frame_t *frame, uint8_t *source, uint32_t source_len);
int spdy_frame_dump(spdy_frame_t *frame);
int spdy_frame_cleanup(spdy_frame_t *frame);