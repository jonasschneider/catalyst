#include <stdio.h> // puts
#include <string.h> // memcpy
#include <malloc.h>

#include "spdy_frame.h"
#include "spdy_headers.h"
#include "catalyst.h"

int spdy_frame_create(spdy_frame_t *frame)
{
  memset(frame, 0, sizeof(spdy_frame_t));
}

int spdy_frame_parse(spdy_frame_t *frame, uint8_t *source, uint32_t source_len)
{
  if(frame->parsed > 0)
    return SPDY_FRAME_ERROR_ALREADY_PARSED;

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

  // FIXME: check length for correctness using soruce_len
  frame->data_length = (source[5] << 16) + (source[6] << 8) + source[7];

  // raw data
  //frame->data = malloc(frame->data_length);
  //printf("first char: %d\n", source[8+offs]);
  //printf("second char: %d\n", source[9+offs]);
  //memcpy(frame->data, &source[8+offs], frame->data_length);

  int compressed_headers_at_offset = 0;

  switch(frame->control_frame_type)
  {
    case SPDY_CONTROL_SYN_STREAM:
      frame->control_header.syn_stream.stream_id = ((source[8] & (0xff >> 1)) << 24) + (source[9] << 16) + (source[10] << 8) + source[11];
      frame->control_header.syn_stream.associated_stream_id = ((source[12] & (0xff >> 1)) << 24) + (source[13] << 16) + (source[14] << 8) + source[15];
      frame->control_header.syn_stream.priority = (source[16] & 0xfff00000) >> 5;
      frame->control_header.syn_stream.slot = source[17];

      compressed_headers_at_offset = 10;
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
  }

  if(compressed_headers_at_offset > 0)
  {
    frame->headers = malloc(sizeof(spdy_headers_t));
    int res = spdy_headers_inflate(frame->headers, &source[8+compressed_headers_at_offset], frame->data_length-compressed_headers_at_offset);

    DEBUG2("spdy_headers_inflate result: %d\n", res);
    DEBUG2("second byte of header data: %x\n", frame->headers->data[1]);
  }

  frame->parsed = 1; // track that we have allocated memory for headers etc

  return 0;
}


int spdy_frame_dump(spdy_frame_t *frame)
{
  printf("Frame type: %u\n", frame->frame_type);
  printf("SPDY frame v%d\n", frame->protocol_version);
  printf("Frame type: %u\n", frame->control_frame_type);
  printf("Flags: %u\n", frame->flags);
  printf("Frame data length: %d\n", frame->data_length);

  spdy_headers_dump(frame->headers);
}


int spdy_frame_destroy(spdy_frame_t *frame)
{
  // free data pointers of spdy_frame_parse
}