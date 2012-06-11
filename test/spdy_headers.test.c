#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include "spdy_headers.h"

const uint8_t test_data_compressed_header[] = {0x38, 0xea, 0xdf, 0xa2, 0x51, 0xb2, 0x62, 0xe0, 0x66, 0x60, 0x83, 0xa4, 0x17, 0x86, 0x28, 0x34, 0xdb, 0x75, 0x30, 0xac, 0xd7, 0x06, 0x89, 0xc2, 0xfd, 0x61, 0x5d, 0x68, 0x6b, 0xa0, 0x67, 0xa9, 0x83, 0x70, 0x13, 0x58, 0xc0, 0x42, 0x07, 0xee, 0x3f, 0x1d, 0x2d, 0x7d, 0x2d, 0xb0, 0x98, 0x29, 0x03, 0x1f, 0x6a, 0x66, 0x66, 0x90, 0xf3, 0x0c, 0xf6, 0x87, 0x3a, 0x55, 0x07, 0xec, 0x56, 0xb0, 0x3a, 0x73, 0x1d, 0x88, 0x7a, 0x63, 0x06, 0x7e, 0xb4, 0xec, 0xce, 0x20, 0x08, 0xf2, 0x8c, 0x0e, 0xd4, 0x37, 0x3a, 0xc5, 0x29, 0xc9, 0x19, 0x70, 0x35, 0xb0, 0x14, 0xc2, 0xc0, 0x97, 0x9a, 0xa7, 0x1b, 0x1a, 0xac, 0x93, 0x0a, 0x75, 0x08, 0x03, 0x1b, 0x24, 0x56, 0x19, 0x5a, 0x8a, 0x8d, 0x8c, 0xd3, 0xd2, 0x75, 0x8d, 0x0b, 0x8c, 0xcc, 0x8d, 0x92, 0x0b, 0x2c, 0xcb, 0x0d, 0xe2, 0x8b, 0x6c, 0xcd, 0xad, 0x15, 0x0c, 0xb3, 0xcd, 0x0b, 0xcd, 0x75, 0x33, 0x0d, 0x52, 0xcc, 0xd3, 0x8b, 0x0b, 0x0d, 0x2d, 0xcc, 0x81, 0xa2, 0x06, 0xd6, 0x0a, 0xf1, 0x20, 0x27, 0x96, 0x24, 0xa5, 0x26, 0x96, 0x18, 0x01, 0x64, 0x5b, 0x9c, 0x6a, 0x9c, 0x55, 0x51, 0x92, 0x64, 0x54, 0x99, 0x65, 0x9c, 0x9a, 0x93, 0x93, 0x6a, 0x0c, 0x94, 0x8d, 0x2f, 0x2f, 0x29, 0x8f, 0x2f, 0xcb, 0x2c, 0x8e, 0x07, 0x66, 0x79, 0x5b, 0x6b, 0x85, 0xb2, 0xc4, 0xbc, 0xcc, 0x92, 0xca, 0xf8, 0xcc, 0x14, 0xdb, 0x34, 0x63, 0x23, 0x8b, 0xe4, 0x24, 0x33, 0x13, 0x63, 0x93, 0x64, 0x60, 0xfe, 0x4d, 0x31, 0x32, 0x4e, 0x31, 0x35, 0x34, 0x4f, 0x49, 0x33, 0x32, 0x4f, 0x03, 0x16, 0x04, 0xa6, 0x96, 0x49, 0x0c, 0x2c, 0xa0, 0xc2, 0x88, 0x81, 0x0f, 0x94, 0x62, 0x73, 0x40, 0x4c, 0x2b, 0x4b, 0x03, 0x03, 0x03, 0x06, 0xb6, 0x5c, 0x60, 0x21, 0x98, 0x9f, 0xc2, 0xc0, 0xec, 0xee, 0x1a, 0xc2, 0xc0, 0x56, 0x0c, 0xcc, 0x37, 0xb9, 0xa9, 0x0c, 0xac, 0x19, 0x25, 0x25, 0x05, 0xc5, 0x0c, 0xcc, 0xa0, 0x08, 0x62, 0xd4, 0x67, 0xe0, 0x42, 0x94, 0x2a, 0x0c, 0x65, 0xbe, 0xf9, 0x55, 0x99, 0x39, 0x39, 0x89, 0xfa, 0xa6, 0x7a, 0x06, 0x00, 0x29, 0x68, 0xf8, 0x26, 0x26, 0x67, 0xe6, 0x95, 0xe4, 0x17, 0x67, 0x58, 0x2b, 0x78, 0x02, 0x13, 0x7a, 0x8e, 0x02, 0x50, 0x40, 0xc1, 0x3f, 0x58, 0x21, 0x42, 0xc1, 0xd0, 0x20, 0xde, 0x2c, 0xde, 0x5c, 0x53, 0xc1, 0x11, 0x18, 0x87, 0xa9, 0xe1, 0xa9, 0x49, 0xde, 0x99, 0x25, 0xfa, 0xa6, 0xc6, 0x26, 0x7a, 0x46, 0x26, 0x0a, 0x1a, 0xde, 0x1e, 0x21, 0xbe, 0x3e, 0x3a, 0x0a, 0x39, 0x99, 0xd9, 0xa9, 0x0a, 0xee, 0xa9, 0xc9, 0xd9, 0xf9, 0x9a, 0x0a, 0xce, 0x19, 0xc0, 0xd2, 0x32, 0x55, 0xdf, 0xd0, 0x50, 0xcf, 0x40, 0xcf, 0xcc, 0xd2, 0x4c, 0xcf, 0xc8, 0x42, 0x21, 0x38, 0x31, 0x2d, 0xb1, 0x28, 0x13, 0xaa, 0x89, 0x81, 0x1d, 0x9a, 0x66, 0x18, 0x38, 0x60, 0x49, 0x09, 0x00, 0x00, 0x00, 0xff, 0xff};
const uint8_t test_data_corrupt_header[] = {0x38, 0x97, 0x9a, 0xa7, 0x1b, 0x1a, 0xac, 0x93, 0x0a, 0x75, 0x08, 0x03, 0x1b, 0x24, 0x56, 0x19, 0x5a, 0x8a, 0x8d, 0x8c, 0xd3, 0xd2, 0x75, 0x8d, 0x0b, 0x8c, 0xcc, 0x8d, 0x92, 0x0b, 0x2c, 0xcb, 0x0d, 0xe2, 0x8b, 0x6c, 0xcd, 0xad, 0x15, 0x0c, 0xb3, 0xcd, 0x0b, 0xcd, 0x75, 0x33, 0x0d, 0x52, 0xcc, 0xd3, 0x8b, 0x0b, 0x0d, 0x2d, 0xcc, 0x81, 0xa2, 0x06, 0xd6, 0x0a, 0xf1, 0x20, 0x27, 0x96, 0x24, 0xa5, 0x26, 0x96, 0x18, 0x01, 0x64, 0x5b, 0x9c, 0x6a, 0x9c, 0x55, 0x51, 0x92, 0x64, 0x54, 0x99, 0x65, 0x9c, 0x9a, 0x93, 0x93, 0x6a, 0x0c, 0x94, 0x8d, 0x2f, 0x2f, 0x29, 0x8f, 0x2f, 0xcb, 0x2c, 0x8e, 0x07, 0x66, 0x79, 0x5b, 0x6b, 0x85, 0xb2, 0xc4, 0xbc, 0xcc, 0x92, 0xca, 0xf8, 0xcc, 0x14, 0xdb, 0x34, 0x63, 0x23, 0x8b, 0xe4, 0x24, 0x33, 0x13, 0x63, 0x93, 0x64, 0x60, 0xfe, 0x4d, 0x31, 0x32, 0x4e, 0x31, 0x35, 0x34, 0x4f, 0x49, 0x33, 0x32, 0x4f, 0x03, 0x16, 0x04, 0xa6, 0x96, 0x49, 0x0c, 0x2c, 0xa0, 0xc2, 0x88, 0x81, 0x0f, 0x94, 0x62, 0x73, 0x40, 0x4c, 0x2b, 0x4b, 0x03, 0x03, 0x03, 0x06, 0xb6, 0x5c, 0x60, 0x21, 0x98, 0x9f, 0xc2, 0xc0, 0xec, 0xee, 0x1a, 0xc2, 0xc0, 0x56, 0x0c, 0xcc, 0x37, 0xb9, 0xa9, 0x0c, 0xac, 0x19, 0x25, 0x25, 0x05, 0xc5, 0x0c, 0xcc, 0xa0, 0x08, 0x62, 0xd4, 0x67, 0xe0, 0x42, 0x94, 0x2a, 0x0c, 0x65, 0xbe, 0xf9, 0x55, 0x99, 0x39, 0x39, 0x89, 0xfa, 0xa6, 0x7a, 0x06, 0x00, 0x29, 0x68, 0xf8, 0x26, 0x26, 0x67, 0xe6, 0x95, 0xe4, 0x17, 0x67, 0x58, 0x2b, 0x78, 0x02, 0x13, 0x7a, 0x8e, 0x02, 0x50, 0x40, 0xc1, 0x3f, 0x58, 0x21, 0x42, 0xc1, 0xd0, 0x20, 0xde, 0x2c, 0xde, 0x5c, 0x53, 0xc1, 0x11, 0x18, 0x87, 0xa9, 0xe1, 0xa9, 0x49, 0xde, 0x99, 0x25, 0xfa, 0xa6, 0xc6, 0x26, 0x7a, 0x46, 0x26, 0x0a, 0x1a, 0xde, 0x1e, 0x21, 0xbe, 0x3e, 0x3a, 0x0a, 0x39, 0x99, 0xd9, 0xa9, 0x0a, 0xee, 0xa9, 0xc9, 0xd9, 0xf9, 0x9a, 0x0a, 0xce, 0x19, 0xc0, 0xd2, 0x32, 0x55, 0xdf, 0xd0, 0x50, 0xcf, 0x40, 0xcf, 0xcc, 0xd2, 0x4c, 0xcf, 0xc8, 0x42, 0x21, 0x38, 0x31, 0x2d, 0xb1, 0x28, 0x13, 0xaa, 0x89, 0x81, 0x1d, 0x9a, 0x66, 0x18, 0x38, 0x60, 0x49, 0x09, 0x00, 0x00, 0x00, 0xff, 0xff};


void test_inflate()
{
  spdy_headers_t the_headers;
  spdy_headers_t *headers = &the_headers;
  spdy_headers_create(headers);
  z_stream inflatestream;
  inflatestream.zalloc = Z_NULL;
  inflatestream.zfree = Z_NULL;
  inflatestream.opaque = Z_NULL;
  inflatestream.avail_in = 0;
  inflatestream.next_in = Z_NULL;
  inflateInit(&inflatestream);

  int res = spdy_headers_inflate(headers, &inflatestream, (uint8_t*)test_data_compressed_header, sizeof(test_data_compressed_header));
  printf("result: %d\n", res);
  assert(res == 0);

  assert(headers->entry_count == 11);
  assert(headers->data_length == 570);

  spdy_headers_destroy(headers);
}



void test_inflate_corrupt()
{
  spdy_headers_t the_headers;
  spdy_headers_t *headers = &the_headers;
  spdy_headers_create(headers);
  z_stream inflatestream;
  inflatestream.zalloc = Z_NULL;
  inflatestream.zfree = Z_NULL;
  inflatestream.opaque = Z_NULL;
  inflatestream.avail_in = 0;
  inflatestream.next_in = Z_NULL;
  inflateInit(&inflatestream);

  int res = spdy_headers_inflate(headers, &inflatestream, (uint8_t*)test_data_corrupt_header, sizeof(test_data_corrupt_header));
  assert(res == SPDY_HEADERS_ERROR_CORRUPT_DATA);

  spdy_headers_destroy(headers);
}



const uint8_t test_compressed_headers_1[] = {0x38, 0xea, 0xdf, 0xa2, 0x51, 0xb2, 0x62, 0xe0, 0x62, 0x60, 0x83, 0xa4, 0x17, 0x06, 0x7b, 0xb8, 0x0b, 0x75, 0x30, 0x2c, 0xd6, 0xae, 0x40, 0x17, 0xcd, 0xcd, 0xb1, 0x2e, 0xb4, 0x35, 0xd0, 0xb3, 0xd4, 0xd1, 0xd2, 0xd7, 0x02, 0xb3, 0x2c, 0x18, 0xf8, 0x50, 0x73, 0x2c, 0x83, 0x9c, 0x67, 0xb0, 0x3f, 0xd4, 0x3d, 0x3a, 0x60, 0x07, 0x81, 0xd5, 0x99, 0xeb, 0x40, 0xd4, 0x1b, 0x33, 0xf0, 0xa3, 0xe5, 0x69, 0x06, 0x41, 0x90, 0x8b, 0x75, 0xa0, 0x4e, 0xd6, 0x29, 0x4e, 0x49, 0xce, 0x80, 0xab, 0x81, 0x25, 0x03, 0x06, 0xbe, 0xd4, 0x3c, 0xdd, 0xd0, 0x60, 0x9d, 0xd4, 0x3c, 0xa8, 0xa5, 0x2c, 0xa0, 0x3c, 0xce, 0xc0, 0x0f, 0x4a, 0x08, 0x39, 0x20, 0xa6, 0x15, 0x30, 0xe3, 0x19, 0x18, 0x30, 0xb0, 0xe5, 0x02, 0x0b, 0x97, 0xfc, 0x14, 0x06, 0x66, 0x77, 0xd7, 0x10, 0x06, 0xb6, 0x62, 0x60, 0x7a, 0xcc, 0x4d, 0x65, 0x60, 0xcd, 0x28, 0x29, 0x29, 0x28, 0x66, 0x60, 0x06, 0x79, 0x9c, 0x51, 0x9f, 0x81, 0x0b, 0x91, 0x5b, 0x19, 0xd2, 0x7d, 0xf3, 0xab, 0x32, 0x73, 0x72, 0x12, 0xf5, 0x4d, 0xf5, 0x0c, 0x14, 0x34, 0x00, 0x8a, 0x30, 0x34, 0xb4, 0x56, 0xf0, 0xc9, 0xcc, 0x2b, 0xad, 0x50, 0xa8, 0xb0, 0x30, 0x8b, 0x37, 0x33, 0xd1, 0x54, 0x70, 0x04, 0x7a, 0x3e, 0x35, 0x3c, 0x35, 0xc9, 0x3b, 0xb3, 0x44, 0xdf, 0xd4, 0xd8, 0x44, 0xcf, 0x18, 0xa8, 0xcc, 0xdb, 0x23, 0xc4, 0xd7, 0x47, 0x47, 0x21, 0x27, 0x33, 0x3b, 0x55, 0xc1, 0x3d, 0x35, 0x39, 0x3b, 0x5f, 0x53, 0xc1, 0x39, 0x03, 0x58, 0xec, 0xa4, 0xea, 0x1b, 0x1a, 0xe9, 0x01, 0x7d, 0x6a, 0x62, 0x04, 0x52, 0x16, 0x9c, 0x98, 0x96, 0x58, 0x94, 0x09, 0xd5, 0xc4, 0xc0, 0x0e, 0x0d, 0x7c, 0x06, 0x0e, 0x58, 0x9c, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff};
const uint8_t test_compressed_headers_2[] = {0x42, 0x8a, 0x02, 0x66, 0x60, 0x60, 0x0e, 0xad, 0x60, 0xe4, 0xd1, 0x4f, 0x4b, 0x2c, 0xcb, 0x04, 0x66, 0x33, 0x3d, 0x20, 0x31, 0x58, 0x42, 0x14, 0x00, 0x00, 0x00, 0xff, 0xff};

void test_inflate_successive()
{
  printf("===== SUCCESSIVE INFLATE TEST\n");
  spdy_headers_t the_headers;
  spdy_headers_t *headers = &the_headers;
  spdy_headers_create(headers);
  z_stream inflatestream;
  inflatestream.zalloc = Z_NULL;
  inflatestream.zfree = Z_NULL;
  inflatestream.opaque = Z_NULL;
  inflatestream.avail_in = 0;
  inflatestream.avail_out = 0;
  inflatestream.next_in = Z_NULL;
  inflatestream.next_out = Z_NULL;
  inflateInit(&inflatestream);
  int res;

  res = spdy_headers_inflate(headers, &inflatestream, (uint8_t*)test_compressed_headers_1, sizeof(test_compressed_headers_1));
  assert(res == 0);

  res = spdy_headers_inflate(headers, &inflatestream, (uint8_t*)test_compressed_headers_2, sizeof(test_compressed_headers_2));
  assert(res == 0);

  spdy_headers_destroy(headers);
}


void test_iterate()
{
  spdy_headers_t the_headers;
  spdy_headers_t *headers = &the_headers;
  z_stream inflatestream;
  spdy_headers_create(headers);
  inflatestream.zalloc = Z_NULL;
  inflatestream.zfree = Z_NULL;
  inflatestream.opaque = Z_NULL;
  inflatestream.avail_in = 0;
  inflatestream.next_in = Z_NULL;
  inflateInit(&inflatestream);

  spdy_headers_inflate(headers, &inflatestream, (uint8_t*)test_data_compressed_header, sizeof(test_data_compressed_header));

  spdy_headers_dump(headers);

  uint8_t nbuf[SPDY_HEADERS_NAME_SIZE];
  uint8_t vbuf[SPDY_HEADERS_VALUE_SIZE];
  uint8_t *p=0;
  uint8_t n=0;

  while(p = spdy_headers_iterate(headers, p))
  {
    n++;
    assert(spdy_headers_get(p, nbuf, vbuf) == 0);
    if(n==6)
    {
      assert(strcmp(nbuf, "host") == 0);
      assert(strcmp(vbuf, "localhost:9000") == 0);
    }
  }

  assert(n == 11);

  spdy_headers_destroy(headers);
}

int main() {
  test_inflate();
  test_inflate_corrupt();
  test_inflate_successive();
  test_iterate();

  fprintf(stderr, "%c[%d;%d;%dmspdy_headers passed.", 0x1B, 1,32,40);
  fprintf(stderr, "%c[%dm\n", 0x1B, 0);
  return 0;
}