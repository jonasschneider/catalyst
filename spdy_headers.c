#include <assert.h>
#include <zlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <malloc.h>
#include <stdarg.h>
#include <string.h>

#include "spdy_frame.h"
#include "spdy_headers.h"
#include "catalyst.h"

static const char spdy_dictionary[] =
  "optionsgetheadpostputdeletetraceacceptaccept-charsetaccept-encodingaccept-"
  "languageauthorizationexpectfromhostif-modified-sinceif-matchif-none-matchi"
  "f-rangeif-unmodifiedsincemax-forwardsproxy-authorizationrangerefererteuser"
  "-agent10010120020120220320420520630030130230330430530630740040140240340440"
  "5406407408409410411412413414415416417500501502503504505accept-rangesageeta"
  "glocationproxy-authenticatepublicretry-afterservervarywarningwww-authentic"
  "ateallowcontent-basecontent-encodingcache-controlconnectiondatetrailertran"
  "sfer-encodingupgradeviawarningcontent-languagecontent-lengthcontent-locati"
  "oncontent-md5content-rangecontent-typeetagexpireslast-modifiedset-cookieMo"
  "ndayTuesdayWednesdayThursdayFridaySaturdaySundayJanFebMarAprMayJunJulAugSe"
  "pOctNovDecchunkedtext/htmlimage/pngimage/jpgimage/gifapplication/xmlapplic"
  "ation/xhtmltext/plainpublicmax-agecharset=iso-8859-1utf-8gzipdeflateHTTP/1"
  ".1statusversionurl";


// FIXME: persist zlib stream for a spdy session
int spdy_headers_inflate(spdy_headers_t *headers, uint8_t *source, uint32_t source_len)
{
    int ret;
    unsigned have = 0;
    z_stream strm;

    const int initial_output_size = 256;
    headers->data = (uint8_t*) malloc(initial_output_size);

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
        return ret;

    DEBUG2("compressed data len: %u\n", source_len);

    strm.avail_in = source_len;
    strm.next_in = source;

    int n;
    for(n=0; n < source_len; n++) {
      DEBUG2("%02x ",(uint8_t)source[n]);
      if((n+1) % 32 == 0)
        DEBUG1("\n");
    }

    inflate(&strm, Z_NO_FLUSH);

    uint32_t output_size = initial_output_size;
    headers->data_length = 0;

    strm.avail_out = initial_output_size;

    strm.next_out = headers->data;

    do {
        ret = inflate(&strm, Z_NO_FLUSH);
        DEBUG2("inflate returned %d\n", ret);
        if(ret == Z_NEED_DICT)
        {
          DEBUG1("zlib needs dict\n");
          inflateSetDictionary(&strm, spdy_dictionary, sizeof(spdy_dictionary));
          ret = inflate(&strm, Z_NO_FLUSH);
          DEBUG2("retried, result=%u\n", ret);
        }
        assert(ret != Z_STREAM_ERROR);  /* state not clobbered */

        switch (ret) {
        case Z_NEED_DICT:
        case Z_DATA_ERROR:
        case Z_MEM_ERROR:
            (void)inflateEnd(&strm);
            //return ret;
        }

        headers->data_length += initial_output_size - strm.avail_out;

        DEBUG3("in avail: %u, out avail=%u\n", strm.avail_in, strm.avail_out);
        DEBUG3("next avail byte: %x at %x\n", *strm.next_in, strm.next_in);

        if(strm.avail_in > 0) {
          output_size = output_size + initial_output_size;
          DEBUG3("more to come (avail out=%u), reallocing to %u\n", strm.avail_out, output_size);

          uint8_t *oldptr = headers->data;
          headers->data = realloc(headers->data, output_size);

          strm.avail_out += initial_output_size;
          strm.next_out = headers->data + headers->data_length;
        }

        DEBUG2("avail out: %u\n", strm.avail_out);
    } while (strm.avail_in > 0);

    (void)inflateEnd(&strm);

    headers->entry_count = (headers->data[0] << 8) + headers->data[1];
    // fixme: check if count is correct

    for(n=0; n < headers->data_length; n++) {
      DEBUG2("%02x ",(uint8_t)headers->data[n]);
      if((n+1) % 32 == 0)
        DEBUG1("\n");
    }
    DEBUG1("\n");

    return ret == Z_OK ? 0 : -1;
}

uint8_t *spdy_headers_iterate(spdy_headers_t *headers, uint8_t *position)
{
  if(position == 0)
  {
    return headers->data+2;
  }

  uint16_t name_len = (*position << 8) + *(position+1);
  position += 2 + name_len;

  uint16_t value_len = (*position << 8) + *(position+1);
  position += 2 + value_len;

  if(position >= (headers->data + headers->data_length))
  {
    return 0;
  }

  return position;
}

int spdy_headers_get(uint8_t *position, uint8_t *nbuf, uint8_t *vbuf)
{
  uint16_t name_len = (*position << 8) + *(position+1);
  if(name_len > SPDY_HEADERS_NAME_SIZE)
  {
    return -1;
  }
  strncpy(nbuf, position+2, SPDY_HEADERS_NAME_SIZE);

  position += 2 + name_len;

  uint16_t value_len = (*position << 8) + *(position+1);
  if(value_len > SPDY_HEADERS_VALUE_SIZE)
  {
    return -1;
  }
  strncpy(vbuf, position+2, SPDY_HEADERS_VALUE_SIZE);

  return 0;
}

void spdy_headers_dump(spdy_headers_t *headers)
{
  int i, j;
  printf("Headers (%u entries):\n", headers->entry_count);

  int offset = 2;

  for(i=0; i < headers->entry_count * 2; i++)
  {
    if(i%2 == 0 && i != headers->entry_count * 2 - 1)
      printf("  ");
    int len = (headers->data[offset] << 8) + headers->data[offset+1];
    offset += 2;
    for(j=0; j < len; j++)
      printf("%c", headers->data[offset+j]);
    if(i%2 == 0)
    {
      printf(" => ");
    }
    else
    {
      printf("\n");
    }
    offset += j;
  }
}

void spdy_headers_destroy(spdy_headers_t *headers)
{
  free(headers->data);
}