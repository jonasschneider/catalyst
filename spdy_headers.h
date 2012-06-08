#ifndef SPDY_HEADERS_H
#define SPDY_HEADERS_H

#include <inttypes.h>

typedef struct
{
  uint32_t entry_count;
  uint32_t data_length;
  uint8_t *data;
} spdy_headers_t;

int spdy_headers_inflate(spdy_headers_t *headers, uint8_t *source, uint32_t source_len);

# endif