#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "spdy_session.h"

const uint8_t test_packet_syn_stream[] =             {0x80, 0x02, 0x00, 0x01, 0x01, 0x00, 0x01, 0x45, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0xea, 0xdf, 0xa2, 0x51, 0xb2, 0x62, 0xe0, 0x66, 0x60, 0x83, 0xa4, 0x17, 0x86, 0x28, 0x34, 0xdb, 0x75, 0x30, 0xac, 0xd7, 0x06, 0x89, 0xc2, 0xfd, 0x61, 0x5d, 0x68, 0x6b, 0xa0, 0x67, 0xa9, 0x83, 0x70, 0x13, 0x58, 0xc0, 0x42, 0x07, 0xee, 0x3f, 0x1d, 0x2d, 0x7d, 0x2d, 0xb0, 0x98, 0x29, 0x03, 0x1f, 0x6a, 0x66, 0x66, 0x90, 0xf3, 0x0c, 0xf6, 0x87, 0x3a, 0x55, 0x07, 0xec, 0x56, 0xb0, 0x3a, 0x73, 0x1d, 0x88, 0x7a, 0x63, 0x06, 0x7e, 0xb4, 0xec, 0xce, 0x20, 0x08, 0xf2, 0x8c, 0x0e, 0xd4, 0x37, 0x3a, 0xc5, 0x29, 0xc9, 0x19, 0x70, 0x35, 0xb0, 0x14, 0xc2, 0xc0, 0x97, 0x9a, 0xa7, 0x1b, 0x1a, 0xac, 0x93, 0x0a, 0x75, 0x08, 0x03, 0x2f, 0x4a, 0x3a, 0x64, 0xe0, 0x84, 0x86, 0x96, 0xad, 0x01, 0x03, 0x0b, 0xa8, 0x60, 0x60, 0xe0, 0x33, 0x34, 0x32, 0xd7, 0x33, 0x00, 0x42, 0x43, 0x2b, 0x4b, 0x03, 0x03, 0x03, 0x06, 0xb6, 0x5c, 0x60, 0x81, 0x94, 0x9f, 0xc2, 0xc0, 0xec, 0xee, 0x1a, 0xc2, 0xc0, 0x56, 0x0c, 0xd4, 0x9b, 0x9b, 0xca, 0xc0, 0x9a, 0x51, 0x52, 0x52, 0x50, 0xcc, 0xc0, 0x0c, 0x0a, 0x2c, 0x11, 0x7d, 0x7b, 0x80, 0x80, 0x61, 0x9d, 0x6f, 0x9b, 0xa8, 0x06, 0x2c, 0x10, 0x80, 0xc5, 0x86, 0x6d, 0x56, 0x71, 0x7e, 0x1e, 0x03, 0x17, 0x22, 0xd3, 0x33, 0x94, 0xf9, 0xe6, 0x57, 0x65, 0xe6, 0xe4, 0x24, 0xea, 0x9b, 0xea, 0x19, 0x28, 0x68, 0xf8, 0x26, 0x26, 0x67, 0xe6, 0x95, 0xe4, 0x17, 0x67, 0x58, 0x2b, 0x78, 0x02, 0xd3, 0x61, 0x8e, 0x02, 0x50, 0x40, 0xc1, 0x3f, 0x58, 0x21, 0x42, 0xc1, 0xd0, 0x20, 0xde, 0x2c, 0xde, 0x5c, 0x53, 0xc1, 0x11, 0x18, 0xc4, 0xa9, 0xe1, 0xa9, 0x49, 0xde, 0x99, 0x25, 0xfa, 0xa6, 0xc6, 0x26, 0x7a, 0x46, 0x26, 0x0a, 0x1a, 0xde, 0x1e, 0x21, 0xbe, 0x3e, 0x3a, 0x0a, 0x39, 0x99, 0xd9, 0xa9, 0x0a, 0xee, 0xa9, 0xc9, 0xd9, 0xf9, 0x9a, 0x0a, 0xce, 0x19, 0xc0, 0xc2, 0x2c, 0x55, 0xdf, 0xd0, 0x10, 0xe8, 0x58, 0x33, 0x4b, 0x33, 0x3d, 0x23, 0x0b, 0x85, 0xe0, 0xc4, 0xb4, 0xc4, 0xa2, 0x4c, 0xa8, 0x26, 0x06, 0x76, 0x68, 0x94, 0x32, 0x70, 0xc0, 0x62, 0x1a, 0x00, 0x00, 0x00, 0xff, 0xff};
const uint8_t test_packet_data_frame[] =             {0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0d, 0x54, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x53, 0x50, 0x44, 0x59, 0x2e};

const uint8_t test_packet_successive_synstream_1[] = {0x80, 0x02, 0x00, 0x01, 0x01, 0x00, 0x01, 0x0E, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0xea, 0xdf, 0xa2, 0x51, 0xb2, 0x62, 0xe0, 0x62, 0x60, 0x83, 0xa4, 0x17, 0x06, 0x7b, 0xb8, 0x0b, 0x75, 0x30, 0x2c, 0xd6, 0xae, 0x40, 0x17, 0xcd, 0xcd, 0xb1, 0x2e, 0xb4, 0x35, 0xd0, 0xb3, 0xd4, 0xd1, 0xd2, 0xd7, 0x02, 0xb3, 0x2c, 0x18, 0xf8, 0x50, 0x73, 0x2c, 0x83, 0x9c, 0x67, 0xb0, 0x3f, 0xd4, 0x3d, 0x3a, 0x60, 0x07, 0x81, 0xd5, 0x99, 0xeb, 0x40, 0xd4, 0x1b, 0x33, 0xf0, 0xa3, 0xe5, 0x69, 0x06, 0x41, 0x90, 0x8b, 0x75, 0xa0, 0x4e, 0xd6, 0x29, 0x4e, 0x49, 0xce, 0x80, 0xab, 0x81, 0x25, 0x03, 0x06, 0xbe, 0xd4, 0x3c, 0xdd, 0xd0, 0x60, 0x9d, 0xd4, 0x3c, 0xa8, 0xa5, 0x2c, 0xa0, 0x3c, 0xce, 0xc0, 0x0f, 0x4a, 0x08, 0x39, 0x20, 0xa6, 0x15, 0x30, 0xe3, 0x19, 0x18, 0x30, 0xb0, 0xe5, 0x02, 0x0b, 0x97, 0xfc, 0x14, 0x06, 0x66, 0x77, 0xd7, 0x10, 0x06, 0xb6, 0x62, 0x60, 0x7a, 0xcc, 0x4d, 0x65, 0x60, 0xcd, 0x28, 0x29, 0x29, 0x28, 0x66, 0x60, 0x06, 0x79, 0x9c, 0x51, 0x9f, 0x81, 0x0b, 0x91, 0x5b, 0x19, 0xd2, 0x7d, 0xf3, 0xab, 0x32, 0x73, 0x72, 0x12, 0xf5, 0x4d, 0xf5, 0x0c, 0x14, 0x34, 0x00, 0x8a, 0x30, 0x34, 0xb4, 0x56, 0xf0, 0xc9, 0xcc, 0x2b, 0xad, 0x50, 0xa8, 0xb0, 0x30, 0x8b, 0x37, 0x33, 0xd1, 0x54, 0x70, 0x04, 0x7a, 0x3e, 0x35, 0x3c, 0x35, 0xc9, 0x3b, 0xb3, 0x44, 0xdf, 0xd4, 0xd8, 0x44, 0xcf, 0x18, 0xa8, 0xcc, 0xdb, 0x23, 0xc4, 0xd7, 0x47, 0x47, 0x21, 0x27, 0x33, 0x3b, 0x55, 0xc1, 0x3d, 0x35, 0x39, 0x3b, 0x5f, 0x53, 0xc1, 0x39, 0x03, 0x58, 0xec, 0xa4, 0xea, 0x1b, 0x1a, 0xe9, 0x01, 0x7d, 0x6a, 0x62, 0x04, 0x52, 0x16, 0x9c, 0x98, 0x96, 0x58, 0x94, 0x09, 0xd5, 0xc4, 0xc0, 0x0e, 0x0d, 0x7c, 0x06, 0x0e, 0x58, 0x9c, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff};
const uint8_t test_packet_successive_synstream_2[] = {0x80, 0x02, 0x00, 0x01, 0x01, 0x00, 0x00, 0x27, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x8a, 0x02, 0x66, 0x60, 0x60, 0x0e, 0xad, 0x60, 0xe4, 0xd1, 0x4f, 0x4b, 0x2c, 0xcb, 0x04, 0x66, 0x33, 0x3d, 0x20, 0x31, 0x58, 0x42, 0x14, 0x00, 0x00, 0x00, 0xff, 0xff};

const uint8_t test_packet_settings_frame[] =         {0x80, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x01, 0x2c};
void test_parse()
{
  spdy_session_t session;
  spdy_session_create(&session);
  assert(session.received_frame_count == 0);

  assert(sizeof(test_packet_settings_frame) <= SPDY_SESSION_PARSE_BUFFER_SIZE);

  memcpy(session.parse_buffer, test_packet_settings_frame, sizeof(test_packet_settings_frame));
  session.avail_to_parse = sizeof(test_packet_settings_frame);

  int res = spdy_session_parse_next_frame(&session);
  assert(res == 0); // frame complete

  assert(session.last_frame.frame_type == SPDY_CONTROL_FRAME);
  assert(session.last_frame.control_frame_type == SPDY_CONTROL_SETTINGS);

  assert(session.received_frame_count == 1);

  spdy_session_destroy(&session);
}



void test_queue_frame()
{
  spdy_session_t session;
  spdy_session_create(&session);
  assert(session.received_frame_count == 0);

  assert(session.send_queue_length == 0);
  assert(session.send_queue_front == 0);

  spdy_session_queue_frame(&session, (uint8_t*)test_packet_data_frame, sizeof(test_packet_data_frame));
  assert(session.send_queue_length == 1);

  uint8_t *queued_frame_data;
  size_t queued_frame_length;

  queued_frame_data = spdy_session_unqueue_frame(&session, &queued_frame_length);
  assert(session.send_queue_length == 0);
  assert(session.send_queue_front == 1);

  assert(queued_frame_length == sizeof(test_packet_data_frame));
  assert(queued_frame_data == test_packet_data_frame);

  assert(spdy_session_unqueue_frame(&session, &queued_frame_length) == 0);
  assert(spdy_session_unqueue_frame(&session, &queued_frame_length) == 0);

  spdy_session_destroy(&session);
}



void test_parse_multipart()
{
  spdy_session_t session;
  spdy_session_create(&session);
  assert(sizeof(test_packet_syn_stream) <= SPDY_SESSION_PARSE_BUFFER_SIZE);

  memcpy(session.parse_buffer, test_packet_syn_stream, 5);
  session.avail_to_parse = 5;

  int res = spdy_session_parse_next_frame(&session);
  assert(res == SPDY_SESSION_ERROR_NEED_MORE);
  assert(session.received_frame_count == 0);

  memcpy(session.parse_buffer+5, test_packet_syn_stream+5, sizeof(test_packet_syn_stream)-5);
  session.avail_to_parse = sizeof(test_packet_syn_stream);

  res = spdy_session_parse_next_frame(&session);
  assert(res == 0); // frame complete
  assert(session.received_frame_count == 1);

  spdy_session_destroy(&session);
}




void test_parse_multiframe()
{
  spdy_session_t session;
  int res;

  spdy_session_create(&session);
  assert((sizeof(test_packet_syn_stream) + sizeof(test_packet_data_frame)) <= SPDY_SESSION_PARSE_BUFFER_SIZE);

  memcpy(session.parse_buffer, test_packet_syn_stream, sizeof(test_packet_syn_stream));
  memcpy(session.parse_buffer+sizeof(test_packet_syn_stream), test_packet_data_frame, sizeof(test_packet_data_frame));
  
  session.avail_to_parse = sizeof(test_packet_syn_stream) + sizeof(test_packet_data_frame);

  res = spdy_session_parse_next_frame(&session);
  assert(res == 0);
  assert(session.received_frame_count == 1);
  assert(session.avail_to_parse == sizeof(test_packet_data_frame));

  res = spdy_session_parse_next_frame(&session);
  assert(res == 0);
  assert(session.received_frame_count == 2);
  assert(session.avail_to_parse == 0);

  res = spdy_session_parse_next_frame(&session);
  assert(res == SPDY_SESSION_ERROR_NEED_MORE);

  spdy_session_destroy(&session);
}

void test_inflate_synstream_headers()
{
  spdy_session_t session;
  spdy_session_create(&session);

  assert(sizeof(test_packet_syn_stream) <= SPDY_SESSION_PARSE_BUFFER_SIZE);

  memcpy(session.parse_buffer, test_packet_syn_stream, sizeof(test_packet_syn_stream));
  session.avail_to_parse = sizeof(test_packet_syn_stream);

  int res = spdy_session_parse_next_frame(&session);
  assert(res == 0); // frame complete

  spdy_headers_dump(&session.last_frame_headers);

  assert(session.last_frame_headers.entry_count == 11);

  spdy_session_destroy(&session);
}


void test_zlib_persistence()
{
  printf("==== PERSISTENCE TEST\n");
  spdy_session_t session;
  int res;

  spdy_session_create(&session);
  
  memcpy(session.parse_buffer, test_packet_successive_synstream_1, sizeof(test_packet_successive_synstream_1));
  session.avail_to_parse = sizeof(test_packet_successive_synstream_1);
  res = spdy_session_parse_next_frame(&session);
  assert(res == 0);

  printf("==== First packet succeeded\n");

  memcpy(session.parse_buffer, test_packet_successive_synstream_2, sizeof(test_packet_successive_synstream_2));
  session.avail_to_parse = sizeof(test_packet_successive_synstream_2);
  res = spdy_session_parse_next_frame(&session);
  assert(res == 0);

  assert(session.avail_to_parse == 0);

  spdy_session_destroy(&session);
}




int main() {
  test_parse();
  test_inflate_synstream_headers();
  test_parse_multipart();
  test_parse_multiframe();
  test_zlib_persistence();

  test_queue_frame();

  fprintf(stderr, "%c[%d;%d;%dmspdy_session passed.", 0x1B, 1,32,40);
  fprintf(stderr, "%c[%dm\n", 0x1B, 0);
  return 0;
}