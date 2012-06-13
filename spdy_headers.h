#ifndef SPDY_HEADERS_H
#define SPDY_HEADERS_H
#include <inttypes.h>
#include <zlib.h>

#define SPDY_HEADERS_NAME_SIZE 256
#define SPDY_HEADERS_VALUE_SIZE 4096

#define SPDY_HEADERS_ERROR_CORRUPT_DATA -1

typedef struct
{
  uint32_t entry_count;
  size_t data_length;
  uint8_t *data;
} spdy_headers_t;

int spdy_headers_create(spdy_headers_t *headers);

int spdy_headers_inflate(spdy_headers_t *headers, z_stream *zstrm, uint8_t *source, uint32_t source_len);

int spdy_headers_add(spdy_headers_t *headers, char *n, char *v);
int spdy_headers_add_byte(spdy_headers_t *headers, uint8_t *n, uint8_t *v);

uint8_t *spdy_headers_iterate(spdy_headers_t *headers, uint8_t *position);
int spdy_headers_get(uint8_t *position, uint8_t *nbuf, uint8_t *vbuf);

// Deflate the headers into *out. *out should be at least of size headers->data_length to ensure
// the compressed headers fit in there. Returns the number of used bytes within *out.
uint8_t *spdy_headers_deflate(spdy_headers_t *headers, z_stream *zstrm, size_t *size);

void spdy_headers_dump(spdy_headers_t *headers);
void spdy_headers_destroy(spdy_headers_t *headers);

# endif