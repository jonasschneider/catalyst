#include <stdio.h> // puts
#include <string.h> // memcpy
#include <malloc.h>

#include "spdy.h"


int spdy_frame_parse(Spdy_Frame *frame, uint8_t *source)
{
  frame->frame_type = source[0] >> 7;
  
  if(frame->frame_type == SPDY_CONTROL_FRAME)
  {
    frame->protocol_version = ((source[0] & (0xff >> 1)) << 8) + source[1];
    frame->control_frame_type = (source[2] << 8) + source[3];
  }
  else
  {
    frame->stream_id = ((source[0] & (0xff >> 1)) << 24) + (source[1] << 16) + (source[2] << 8) + source[3];
  }

  frame->flags = source[4];

  // FIXME: check length
  frame->data_length = (source[5] << 16) + (source[6] << 8) + source[7];
  frame->data = malloc(frame->data_length);
  memcpy(frame->data, &source[8], frame->data_length);

  return 0;
}



int spdy_frame_dump(Spdy_Frame *frame)
{
  printf("Frame type: %u\n", frame->frame_type);
  printf("SPDY frame v%d\n", frame->protocol_version);
  printf("Frame type: %u\n", frame->control_frame_type);
  printf("Flags: %u\n", frame->flags);
  printf("Frame data length: %d\n", frame->data_length);
}