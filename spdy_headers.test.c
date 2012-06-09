#include <assert.h>
#include <stdio.h>
#include "spdy_headers.h"

const uint8_t test_data_compressed_header[] = {0x38, 0xea, 0xdf, 0xa2, 0x51, 0xb2, 0x62, 0xe0, 0x66, 0x60, 0x83, 0xa4, 0x17, 0x86, 0x28, 0x34, 0xdb, 0x75, 0x30, 0xac, 0xd7, 0x06, 0x89, 0xc2, 0xfd, 0x61, 0x5d, 0x68, 0x6b, 0xa0, 0x67, 0xa9, 0x83, 0x70, 0x13, 0x58, 0xc0, 0x42, 0x07, 0xee, 0x3f, 0x1d, 0x2d, 0x7d, 0x2d, 0xb0, 0x98, 0x29, 0x03, 0x1f, 0x6a, 0x66, 0x66, 0x90, 0xf3, 0x0c, 0xf6, 0x87, 0x3a, 0x55, 0x07, 0xec, 0x56, 0xb0, 0x3a, 0x73, 0x1d, 0x88, 0x7a, 0x63, 0x06, 0x7e, 0xb4, 0xec, 0xce, 0x20, 0x08, 0xf2, 0x8c, 0x0e, 0xd4, 0x37, 0x3a, 0xc5, 0x29, 0xc9, 0x19, 0x70, 0x35, 0xb0, 0x14, 0xc2, 0xc0, 0x97, 0x9a, 0xa7, 0x1b, 0x1a, 0xac, 0x93, 0x0a, 0x75, 0x08, 0x03, 0x1b, 0x24, 0x56, 0x19, 0x5a, 0x8a, 0x8d, 0x8c, 0xd3, 0xd2, 0x75, 0x8d, 0x0b, 0x8c, 0xcc, 0x8d, 0x92, 0x0b, 0x2c, 0xcb, 0x0d, 0xe2, 0x8b, 0x6c, 0xcd, 0xad, 0x15, 0x0c, 0xb3, 0xcd, 0x0b, 0xcd, 0x75, 0x33, 0x0d, 0x52, 0xcc, 0xd3, 0x8b, 0x0b, 0x0d, 0x2d, 0xcc, 0x81, 0xa2, 0x06, 0xd6, 0x0a, 0xf1, 0x20, 0x27, 0x96, 0x24, 0xa5, 0x26, 0x96, 0x18, 0x01, 0x64, 0x5b, 0x9c, 0x6a, 0x9c, 0x55, 0x51, 0x92, 0x64, 0x54, 0x99, 0x65, 0x9c, 0x9a, 0x93, 0x93, 0x6a, 0x0c, 0x94, 0x8d, 0x2f, 0x2f, 0x29, 0x8f, 0x2f, 0xcb, 0x2c, 0x8e, 0x07, 0x66, 0x79, 0x5b, 0x6b, 0x85, 0xb2, 0xc4, 0xbc, 0xcc, 0x92, 0xca, 0xf8, 0xcc, 0x14, 0xdb, 0x34, 0x63, 0x23, 0x8b, 0xe4, 0x24, 0x33, 0x13, 0x63, 0x93, 0x64, 0x60, 0xfe, 0x4d, 0x31, 0x32, 0x4e, 0x31, 0x35, 0x34, 0x4f, 0x49, 0x33, 0x32, 0x4f, 0x03, 0x16, 0x04, 0xa6, 0x96, 0x49, 0x0c, 0x2c, 0xa0, 0xc2, 0x88, 0x81, 0x0f, 0x94, 0x62, 0x73, 0x40, 0x4c, 0x2b, 0x4b, 0x03, 0x03, 0x03, 0x06, 0xb6, 0x5c, 0x60, 0x21, 0x98, 0x9f, 0xc2, 0xc0, 0xec, 0xee, 0x1a, 0xc2, 0xc0, 0x56, 0x0c, 0xcc, 0x37, 0xb9, 0xa9, 0x0c, 0xac, 0x19, 0x25, 0x25, 0x05, 0xc5, 0x0c, 0xcc, 0xa0, 0x08, 0x62, 0xd4, 0x67, 0xe0, 0x42, 0x94, 0x2a, 0x0c, 0x65, 0xbe, 0xf9, 0x55, 0x99, 0x39, 0x39, 0x89, 0xfa, 0xa6, 0x7a, 0x06, 0x00, 0x29, 0x68, 0xf8, 0x26, 0x26, 0x67, 0xe6, 0x95, 0xe4, 0x17, 0x67, 0x58, 0x2b, 0x78, 0x02, 0x13, 0x7a, 0x8e, 0x02, 0x50, 0x40, 0xc1, 0x3f, 0x58, 0x21, 0x42, 0xc1, 0xd0, 0x20, 0xde, 0x2c, 0xde, 0x5c, 0x53, 0xc1, 0x11, 0x18, 0x87, 0xa9, 0xe1, 0xa9, 0x49, 0xde, 0x99, 0x25, 0xfa, 0xa6, 0xc6, 0x26, 0x7a, 0x46, 0x26, 0x0a, 0x1a, 0xde, 0x1e, 0x21, 0xbe, 0x3e, 0x3a, 0x0a, 0x39, 0x99, 0xd9, 0xa9, 0x0a, 0xee, 0xa9, 0xc9, 0xd9, 0xf9, 0x9a, 0x0a, 0xce, 0x19, 0xc0, 0xd2, 0x32, 0x55, 0xdf, 0xd0, 0x50, 0xcf, 0x40, 0xcf, 0xcc, 0xd2, 0x4c, 0xcf, 0xc8, 0x42, 0x21, 0x38, 0x31, 0x2d, 0xb1, 0x28, 0x13, 0xaa, 0x89, 0x81, 0x1d, 0x9a, 0x66, 0x18, 0x38, 0x60, 0x49, 0x09, 0x00, 0x00, 0x00, 0xff, 0xff};
void test_inflate()
{
  spdy_headers_t the_headers;
  spdy_headers_t *headers = &the_headers;

  int res = spdy_headers_inflate(headers, test_data_compressed_header, sizeof(test_data_compressed_header));
  assert(res == 0);

  assert(headers->entry_count == 11);

  //spdy_frame_destroy(frame);
}

int main() {
  test_inflate();

  printf("%c[%d;%d;%dmspdy_headers passed.", 0x1B, 1,32,40);
  printf("%c[%dm\n", 0x1B, 0);
  return 0;
}