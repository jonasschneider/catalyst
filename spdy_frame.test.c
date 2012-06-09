#include <assert.h>
#include <stdio.h>
#include "spdy_frame.h"

const uint8_t test_packet_syn_stream[] = {0x80, 0x02, 0x00, 0x01, 0x01, 0x00, 0x01, 0x45, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0xea, 0xdf, 0xa2, 0x51, 0xb2, 0x62, 0xe0, 0x66, 0x60, 0x83, 0xa4, 0x17, 0x86, 0x28, 0x34, 0xdb, 0x75, 0x30, 0xac, 0xd7, 0x06, 0x89, 0xc2, 0xfd, 0x61, 0x5d, 0x68, 0x6b, 0xa0, 0x67, 0xa9, 0x83, 0x70, 0x13, 0x58, 0xc0, 0x42, 0x07, 0xee, 0x3f, 0x1d, 0x2d, 0x7d, 0x2d, 0xb0, 0x98, 0x29, 0x03, 0x1f, 0x6a, 0x66, 0x66, 0x90, 0xf3, 0x0c, 0xf6, 0x87, 0x3a, 0x55, 0x07, 0xec, 0x56, 0xb0, 0x3a, 0x73, 0x1d, 0x88, 0x7a, 0x63, 0x06, 0x7e, 0xb4, 0xec, 0xce, 0x20, 0x08, 0xf2, 0x8c, 0x0e, 0xd4, 0x37, 0x3a, 0xc5, 0x29, 0xc9, 0x19, 0x70, 0x35, 0xb0, 0x14, 0xc2, 0xc0, 0x97, 0x9a, 0xa7, 0x1b, 0x1a, 0xac, 0x93, 0x0a, 0x75, 0x08, 0x03, 0x2f, 0x4a, 0x3a, 0x64, 0xe0, 0x84, 0x86, 0x96, 0xad, 0x01, 0x03, 0x0b, 0xa8, 0x60, 0x60, 0xe0, 0x33, 0x34, 0x32, 0xd7, 0x33, 0x00, 0x42, 0x43, 0x2b, 0x4b, 0x03, 0x03, 0x03, 0x06, 0xb6, 0x5c, 0x60, 0x81, 0x94, 0x9f, 0xc2, 0xc0, 0xec, 0xee, 0x1a, 0xc2, 0xc0, 0x56, 0x0c, 0xd4, 0x9b, 0x9b, 0xca, 0xc0, 0x9a, 0x51, 0x52, 0x52, 0x50, 0xcc, 0xc0, 0x0c, 0x0a, 0x2c, 0x11, 0x7d, 0x7b, 0x80, 0x80, 0x61, 0x9d, 0x6f, 0x9b, 0xa8, 0x06, 0x2c, 0x10, 0x80, 0xc5, 0x86, 0x6d, 0x56, 0x71, 0x7e, 0x1e, 0x03, 0x17, 0x22, 0xd3, 0x33, 0x94, 0xf9, 0xe6, 0x57, 0x65, 0xe6, 0xe4, 0x24, 0xea, 0x9b, 0xea, 0x19, 0x28, 0x68, 0xf8, 0x26, 0x26, 0x67, 0xe6, 0x95, 0xe4, 0x17, 0x67, 0x58, 0x2b, 0x78, 0x02, 0xd3, 0x61, 0x8e, 0x02, 0x50, 0x40, 0xc1, 0x3f, 0x58, 0x21, 0x42, 0xc1, 0xd0, 0x20, 0xde, 0x2c, 0xde, 0x5c, 0x53, 0xc1, 0x11, 0x18, 0xc4, 0xa9, 0xe1, 0xa9, 0x49, 0xde, 0x99, 0x25, 0xfa, 0xa6, 0xc6, 0x26, 0x7a, 0x46, 0x26, 0x0a, 0x1a, 0xde, 0x1e, 0x21, 0xbe, 0x3e, 0x3a, 0x0a, 0x39, 0x99, 0xd9, 0xa9, 0x0a, 0xee, 0xa9, 0xc9, 0xd9, 0xf9, 0x9a, 0x0a, 0xce, 0x19, 0xc0, 0xc2, 0x2c, 0x55, 0xdf, 0xd0, 0x10, 0xe8, 0x58, 0x33, 0x4b, 0x33, 0x3d, 0x23, 0x0b, 0x85, 0xe0, 0xc4, 0xb4, 0xc4, 0xa2, 0x4c, 0xa8, 0x26, 0x06, 0x76, 0x68, 0x94, 0x32, 0x70, 0xc0, 0x62, 0x1a, 0x00, 0x00, 0x00, 0xff, 0xff};
void test_syn_stream()
{
  spdy_frame_t the_frame;
  spdy_frame_t *frame = &the_frame;
  spdy_frame_create(frame);

  int res = spdy_frame_parse(frame, (uint8_t*)test_packet_syn_stream, sizeof(test_packet_syn_stream));
  assert(res == 0);

  assert(frame->frame_type == SPDY_CONTROL_FRAME);
  assert(frame->protocol_version == 2);
  assert(frame->control_frame_type == SPDY_CONTROL_SYN_STREAM);
  assert(frame->flags == 1);

  assert(frame->control_header.syn_stream.stream_id == 1);
  assert(frame->control_header.syn_stream.associated_stream_id == 0);
  assert(frame->control_header.syn_stream.priority == 0);
  assert(frame->control_header.syn_stream.slot == 0);

  // FIXME: test headers
  
  spdy_frame_destroy(frame);
}

const uint8_t test_packet_syn_reply[] = {0x80, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x35, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x78, 0xbb, 0xdf, 0xa2, 0x51, 0xb2, 0x62, 0x60, 0x66, 0xe0, 0x71, 0x86, 0x06, 0x52, 0x08, 0x30, 0x90, 0x18, 0xb8, 0x10, 0x76, 0x30, 0xb0, 0x41, 0x94, 0x33, 0xb0, 0x01, 0x93, 0xb1, 0x82, 0xbf, 0x37, 0x03, 0x3b, 0x54, 0x23, 0x03, 0x07, 0xcc, 0x3c, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff};
void test_syn_reply()
{
  spdy_frame_t the_frame;
  spdy_frame_t *frame = &the_frame;
  spdy_frame_create(frame);

  int res = spdy_frame_parse(frame, (uint8_t*)test_packet_syn_reply, sizeof(test_packet_syn_reply));
  assert(res == 0);

  assert(frame->frame_type == SPDY_CONTROL_FRAME);
  assert(frame->protocol_version == 2);
  assert(frame->control_frame_type == SPDY_CONTROL_SYN_REPLY);
  assert(frame->flags == 0);

  assert(frame->control_header.syn_reply.stream_id == 1);

  // FIXME: test headers
  
  spdy_frame_destroy(frame);
}

const uint8_t test_packet_rst_stream[] = {0x80, 0x02, 0x00, 0x03, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01};
void test_rst_stream()
{
  spdy_frame_t the_frame;
  spdy_frame_t *frame = &the_frame;
  spdy_frame_create(frame);

  int res = spdy_frame_parse(frame, (uint8_t*)test_packet_rst_stream, sizeof(test_packet_rst_stream));
  assert(res == 0);

  assert(frame->frame_type == SPDY_CONTROL_FRAME);
  assert(frame->protocol_version == 2);
  assert(frame->control_frame_type == SPDY_CONTROL_RST_STREAM);
  assert(frame->flags == 0);
  assert(frame->data_length == 8);

  assert(frame->control_header.rst_stream.stream_id == 1);
  assert(frame->control_header.rst_stream.status == 1);

  spdy_frame_destroy(frame);
}

const uint8_t test_packet_settings[] = {0x80, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x01, 0x2c};
void test_settings()
{
  spdy_frame_t the_frame;
  spdy_frame_t *frame = &the_frame;
  spdy_frame_create(frame);

  int res = spdy_frame_parse(frame, (uint8_t*)test_packet_settings, sizeof(test_packet_settings));
  assert(res == 0);

  assert(frame->frame_type == SPDY_CONTROL_FRAME);
  assert(frame->protocol_version == 2);
  assert(frame->control_frame_type == SPDY_CONTROL_SETTINGS);
  assert(frame->flags == 0);
  assert(frame->data_length == 8);

  // fixme: extract data

  spdy_frame_destroy(frame);
}

const uint8_t test_packet_ping[] = {0x80, 0x02, 0x00, 0x06, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01};
void test_ping()
{
  spdy_frame_t the_frame;
  spdy_frame_t *frame = &the_frame;
  spdy_frame_create(frame);

  int res = spdy_frame_parse(frame, (uint8_t*)test_packet_ping, sizeof(test_packet_ping));
  assert(res == 0);

  assert(frame->frame_type == SPDY_CONTROL_FRAME);
  assert(frame->protocol_version == 2);
  assert(frame->control_frame_type == SPDY_CONTROL_PING);
  assert(frame->flags == 0);
  assert(frame->data_length == 4);

  assert(frame->control_header.ping.ping_id == 1);

  spdy_frame_destroy(frame);
}

const uint8_t test_packet_goaway[] = {0x80, 0x02, 0x00, 0x07, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01};
void test_goaway()
{
  spdy_frame_t the_frame;
  spdy_frame_t *frame = &the_frame;
  spdy_frame_create(frame);

  int res = spdy_frame_parse(frame, (uint8_t*)test_packet_goaway, sizeof(test_packet_goaway));
  assert(res == 0);

  assert(frame->frame_type == SPDY_CONTROL_FRAME);
  assert(frame->protocol_version == 2);
  assert(frame->control_frame_type == SPDY_CONTROL_GOAWAY);
  assert(frame->flags == 0);
  assert(frame->data_length == 4);

  assert(frame->control_header.goaway.last_good_stream_id == 1);

  spdy_frame_destroy(frame);
}


const uint8_t test_packet_data_frame[] = {0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0d, 0x54, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x53, 0x50, 0x44, 0x59, 0x2e};
void test_data_frame()
{
  spdy_frame_t the_frame;
  spdy_frame_t *frame = &the_frame;
  spdy_frame_create(frame);
  int res = spdy_frame_parse(frame, (uint8_t*)test_packet_data_frame, sizeof(test_packet_data_frame));
  assert(res == 0);

  assert(frame->frame_type == SPDY_DATA_FRAME);
  assert(frame->stream_id == 1);  
  assert(frame->flags == 0);
  assert(frame->data_length == 13);

  spdy_frame_destroy(frame);
}


void test_double_parse()
{
  spdy_frame_t the_frame;
  spdy_frame_t *frame = &the_frame;
  spdy_frame_create(frame);

  spdy_frame_parse(frame, (uint8_t*)test_packet_syn_stream, sizeof(test_packet_syn_stream));
  int res = spdy_frame_parse(frame, (uint8_t*)test_packet_syn_stream, sizeof(test_packet_syn_stream));

  assert(res == SPDY_FRAME_ERROR_ALREADY_PARSED);
  spdy_frame_destroy(frame);
}

int main() {
  // Control frame tests
  test_syn_stream();
  test_syn_reply();
  test_rst_stream();
  test_settings();
  test_ping();
  test_goaway();

  // Data frame tests
  test_data_frame();

  // API tests
  test_double_parse();

  fprintf(stderr, "%c[%d;%d;%dmspdy_frame passed.", 0x1B, 1,32,40);
  fprintf(stderr, "%c[%dm\n", 0x1B, 0);
  return 0;
}