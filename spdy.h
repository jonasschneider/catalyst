#include <inttypes.h>

#define SPDY_DATA_FRAME 0
#define SPDY_CONTROL_FRAME 1

typedef struct
{
  // Common attributes
  char frame_type;
  uint8_t flags;
  uint32_t data_length;
  void *data;

  // Control frame attributes
  uint16_t protocol_version;
  uint16_t control_frame_type;

  // Data frame attributes
  uint32_t stream_id;

} Spdy_Frame;

int spdy_frame_parse(Spdy_Frame *frame, uint8_t *source);
int spdy_frame_dump(Spdy_Frame *frame);