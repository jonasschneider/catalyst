#include <stdio.h> // puts
#include <string.h> // memcpy
#include <malloc.h>

#include "spdy_frame.h"
#include "spdy_headers.h"
#include "catalyst.h"

int spdy_frame_parse(spdy_frame_t *frame, uint8_t *source, uint32_t source_len)
{
  if(frame->parsed)
    return -1;

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

  int offs = 10;

  // FIXME: check length for correctness using soruce_len
  frame->data_length = (source[5] << 16) + (source[6] << 8) + source[7];

  // raw data
  frame->data = malloc(frame->data_length);
  
  printf("first char: %d\n", source[8+offs]);
  printf("second char: %d\n", source[9+offs]);
  memcpy(frame->data, &source[8+offs], frame->data_length);

  //compressed data

  frame->headers = malloc(sizeof(spdy_headers_t));
  int res = spdy_headers_inflate(frame->headers, &source[8+offs], frame->data_length-offs);

  printf("inflate result: %d\n", res);
  //printf("buffer contents: %s\n", buffer);
  printf("first byte of buffer: %x\n", frame->headers->data[0]);
  printf("second byte of buffer: %x\n", frame->headers->data[1]);


  frame->parsed = 1; // track that we have allocated memory, FIXME: track whether stuff has been uncompressed

  return 0;
}


int spdy_frame_dump(spdy_frame_t *frame)
{
  printf("Frame type: %u\n", frame->frame_type);
  printf("SPDY frame v%d\n", frame->protocol_version);
  printf("Frame type: %u\n", frame->control_frame_type);
  printf("Flags: %u\n", frame->flags);
  printf("Frame data length: %d\n", frame->data_length);
}


int spdy_frame_cleanup(spdy_frame_t *frame)
{
  // free data pointers of spdy_frame_parse
}