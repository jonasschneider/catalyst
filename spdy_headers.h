#ifndef SPDY_HEADERS_H
#define SPDY_HEADERS_H

#include <inttypes.h>

#define SPDY_HEADERS_NAME_SIZE 256
#define SPDY_HEADERS_VALUE_SIZE 4096

typedef struct
{
  uint32_t entry_count;
  uint32_t data_length;
  uint8_t *data;
} spdy_headers_t;

int spdy_headers_inflate(spdy_headers_t *headers, uint8_t *source, uint32_t source_len);
uint8_t *spdy_headers_iterate(spdy_headers_t *headers, uint8_t *position);
int spdy_headers_get(uint8_t *position, uint8_t *nbuf, uint8_t *vbuf);
int spdy_headers_dump(spdy_headers_t *headers);

# endif