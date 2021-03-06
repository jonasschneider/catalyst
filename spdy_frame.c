#include <stdio.h> // puts
#include <string.h> // memcpy
#include <malloc.h>

#include "spdy_frame.h"
#include "catalyst.h"

static void spdy_frame_reset(spdy_frame_t *frame)
{
  if(frame->data)
  {
    free(frame->data);
    frame->data = 0;
  }

  memset(frame, 0, sizeof(spdy_frame_t));
}

void spdy_frame_create(spdy_frame_t *frame)
{
  memset(frame, 0, sizeof(spdy_frame_t));
}

int spdy_frame_parse(spdy_frame_t *frame, uint8_t *source, uint32_t source_len)
{
  if(source_len < 8)
    return SPDY_FRAME_ERROR_INCOMPLETE;

  uint32_t data_length = (source[5] << 16) + (source[6] << 8) + source[7];

  DEBUG2("=== parsing frame, source_len=%u.\nfirst 32 bytes:\n", source_len);
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

  // from here on the old frame is discarded
  spdy_frame_reset(frame);

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

  return frame->data_length + 8;
}

size_t spdy_frame_pack_syn_reply(uint8_t *dest, size_t dest_size, uint32_t stream_id, uint8_t *headers, size_t headers_len, uint8_t flags)
{
  spdy_frame_t frame;
  spdy_frame_create(&frame);

  frame.frame_type = SPDY_CONTROL_FRAME;
  frame.protocol_version = 2;
  frame.control_frame_type = SPDY_CONTROL_SYN_REPLY;
  frame.flags = flags;
  frame.stream_id = stream_id;

  uint8_t data[6+headers_len];
  frame.data = data;
  frame.data_length = 6+headers_len;

  data[0] = (stream_id >> 24) & (0xff >> 1);
  data[1] = stream_id >> 16;
  data[2] = stream_id >> 8;
  data[3] = stream_id;
  data[4] = 0;
  data[5] = 0;
  memcpy(data+6, headers, headers_len);
  
  return spdy_frame_pack(&frame, dest, dest_size);
}

size_t spdy_frame_pack_rst_stream(uint8_t *dest, size_t dest_size, uint32_t stream_id, uint32_t status, uint8_t flags)
{
  spdy_frame_t frame;
  spdy_frame_create(&frame);

  frame.frame_type = SPDY_CONTROL_FRAME;
  frame.protocol_version = 2;
  frame.control_frame_type = SPDY_CONTROL_RST_STREAM;
  frame.flags = flags;
  
  uint8_t data[8];
  frame.data = data;
  frame.data_length = 8;

  data[0] = (stream_id >> 24) & (0xff >> 1);
  data[1] = stream_id >> 16;
  data[2] = stream_id >> 8;
  data[3] = stream_id;

  data[4] = status >> 24;
  data[5] = status >> 16;
  data[6] = status >> 8;
  data[7] = status;

  return spdy_frame_pack(&frame, dest, dest_size);
}

size_t spdy_frame_pack_ping(uint8_t *dest, size_t dest_size, uint32_t ping_id, uint8_t flags)
{
  spdy_frame_t frame;
  spdy_frame_create(&frame);

  frame.frame_type = SPDY_CONTROL_FRAME;
  frame.protocol_version = 2;
  frame.control_frame_type = SPDY_CONTROL_PING;
  frame.flags = flags;
  
  uint8_t data[4];
  frame.data = data;
  frame.data_length = 4;

  data[0] = ping_id >> 24;
  data[1] = ping_id >> 16;
  data[2] = ping_id >> 8;
  data[3] = ping_id;

  return spdy_frame_pack(&frame, dest, dest_size);
}


size_t spdy_frame_pack_data(uint8_t *dest, size_t dest_size, uint32_t stream_id, uint8_t *data, size_t data_len, uint8_t flags)
{
  spdy_frame_t frame;
  spdy_frame_create(&frame);

  frame.frame_type = SPDY_DATA_FRAME;
  frame.flags = flags;
  frame.stream_id = stream_id;
  
  frame.data = data;
  frame.data_length = data_len;

  return spdy_frame_pack(&frame, dest, dest_size);
}


size_t spdy_frame_pack_goaway(uint8_t *dest, size_t dest_size, uint32_t last_good_stream_id, uint8_t flags)
{
  spdy_frame_t frame;
  spdy_frame_create(&frame);

  frame.frame_type = SPDY_CONTROL_FRAME;
  frame.protocol_version = 2;
  frame.control_frame_type = SPDY_CONTROL_GOAWAY;
  frame.flags = flags;
  
  uint8_t data[4];
  frame.data = data;
  frame.data_length = 4;

  data[0] = (last_good_stream_id >> 24) & (0xff >> 1);
  data[1] = last_good_stream_id >> 16;
  data[2] = last_good_stream_id >> 8;
  data[3] = last_good_stream_id;

  return spdy_frame_pack(&frame, dest, dest_size);
}


uint32_t spdy_frame_pack(spdy_frame_t *frame, uint8_t *dest, uint32_t dest_size)
{
  if(frame->data_length + 8 > dest_size) // fixme: adjust for different control header sizes
  {
    return 0;
  }
  memset(dest, 0, 8);

  if(frame->frame_type == SPDY_CONTROL_FRAME)
  {
    dest[0] = (frame->protocol_version >> 8) | 1<<7;
    dest[1] = frame->protocol_version;
    dest[2] = frame->control_frame_type >> 8;
    dest[3] = frame->control_frame_type;
  }
  else
  {
    dest[0] = (frame->stream_id >> 24) & (0xff >> 1);
    dest[1] = frame->stream_id >> 16;
    dest[2] = frame->stream_id >> 8;
    dest[3] = frame->stream_id;
  }

  memcpy(dest+8, frame->data, frame->data_length);

  dest[4] = frame->flags;
  dest[5] = frame->data_length << 16;
  dest[6] = frame->data_length << 8;
  dest[7] = frame->data_length;
  return 8 + frame->data_length;
}


void spdy_frame_dump(spdy_frame_t *frame)
{
  printf("Frame type: %u\n", frame->frame_type);
  printf("SPDY frame v%d\n", frame->protocol_version);
  printf("Frame type: %u\n", frame->control_frame_type);
  printf("Flags: %u\n", frame->flags);
  printf("Frame data length: %d\n", frame->data_length);
}


void spdy_frame_destroy(spdy_frame_t *frame)
{
  if(frame->data)
  {
    free(frame->data);
    frame->data = 0;
  }
}