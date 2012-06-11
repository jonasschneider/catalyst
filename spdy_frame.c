#include <stdio.h> // puts
#include <string.h> // memcpy
#include <malloc.h>

#include "spdy_frame.h"
#include "catalyst.h"

int spdy_frame_create(spdy_frame_t *frame)
{
  memset(frame, 0, sizeof(spdy_frame_t));
}

int spdy_frame_parse(spdy_frame_t *frame, uint8_t *source, uint32_t source_len)
{
  if(frame->parsed > 0)
    return SPDY_FRAME_ERROR_ALREADY_PARSED;

  if(source_len < 8)
    return SPDY_FRAME_ERROR_INCOMPLETE;

  uint32_t data_length = (source[5] << 16) + (source[6] << 8) + source[7];

  DEBUG2("parsing frame, source_len=%u. first 32 bytes:\n", source_len);
  int n;
  for(n=0; n < (32 > source_len ? source_len : 32); n++) {
    DEBUG2("%02x ",(uint8_t)source[n]);
  }
  DEBUG2("\nlast 32 bytes:\n", source_len);
  for(n=(31 > source_len ? source_len : 31); n>0; n--) {
    DEBUG2("%02x ",(uint8_t)source[source_len-n]);
  }
  DEBUG1("\n");

  if(source_len < 8 + data_length) // for the moment we only parse complete frames
    return SPDY_FRAME_ERROR_INCOMPLETE;

  frame->data_length = data_length;
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

  switch(frame->control_frame_type)
  {
    case SPDY_CONTROL_SYN_STREAM:
      frame->control_header.syn_stream.stream_id = ((source[8] & (0xff >> 1)) << 24) + (source[9] << 16) + (source[10] << 8) + source[11];
      frame->control_header.syn_stream.associated_stream_id = ((source[12] & (0xff >> 1)) << 24) + (source[13] << 16) + (source[14] << 8) + source[15];
      frame->control_header.syn_stream.priority = (source[16] & 0xfff00000) >> 5;
      frame->control_header.syn_stream.slot = source[17];
      break;
    case SPDY_CONTROL_SYN_REPLY:
      frame->control_header.syn_reply.stream_id = ((source[8] & (0xff >> 1)) << 24) + (source[9] << 16) + (source[10] << 8) + source[11];
      break;
    case SPDY_CONTROL_RST_STREAM:
      frame->control_header.rst_stream.stream_id = ((source[8] & (0xff >> 1)) << 24) + (source[9] << 16) + (source[10] << 8) + source[11];
      frame->control_header.rst_stream.status = (source[12] << 24) + (source[13] << 16) + (source[14] << 8) + source[15];
      break;
    case SPDY_CONTROL_PING:
      frame->control_header.ping.ping_id = (source[8] << 24) + (source[9] << 16) + (source[10] << 8) + source[11];
      break;
    case SPDY_CONTROL_GOAWAY:
      frame->control_header.goaway.last_good_stream_id = (source[8] << 24) + (source[9] << 16) + (source[10] << 8) + source[11];
      break;
    // fixme: fail for other control frame types
  }

  frame->data = malloc(frame->data_length);
  DEBUG2("parsing succeeded, first data byte: %x\n", source[8]);
  memcpy(frame->data, &source[8], frame->data_length);

  frame->parsed = 1; // track that we have allocated memory for headers etc

  return frame->data_length + 8;
}


int spdy_frame_dump(spdy_frame_t *frame)
{
  printf("Frame type: %u\n", frame->frame_type);
  printf("SPDY frame v%d\n", frame->protocol_version);
  printf("Frame type: %u\n", frame->control_frame_type);
  printf("Flags: %u\n", frame->flags);
  printf("Frame data length: %d\n", frame->data_length);
}


int spdy_frame_destroy(spdy_frame_t *frame)
{
  if(frame->data)
  {
    free(frame->data);
  }
  
}