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

int spdy_headers_create(spdy_headers_t *headers)
{
  memset(headers, 0, sizeof(spdy_headers_t));
}


int spdy_headers_inflate(spdy_headers_t *headers, z_stream *zstrm, uint8_t *source, uint32_t source_len)
{
    int ret;
    unsigned have = 0;

    const int initial_output_size = 256;
    if(headers->data)
    {
      free(headers->data);
      headers->data = 0;
    }
    headers->data = (uint8_t*) malloc(initial_output_size);

    DEBUG2("starting inflate, z_stream.total_in=%u\n", zstrm->total_in);
    DEBUG2("dumping compressed data, len=%u\n", source_len);

    int n;
    for(n=0; n < source_len; n++) {
      DEBUG2("%02x ",(uint8_t)source[n]);
      if((n+1) % 32 == 0)
        DEBUG1("\n");
    }
    DEBUG1("\n");

    uint32_t output_size = initial_output_size;
    headers->data_length = 0;

    zstrm->next_in = source;
    zstrm->avail_in = source_len;

    zstrm->next_out = headers->data;
    zstrm->avail_out = initial_output_size;

    do {
        DEBUG4("in avail: %u, out avail=%u, total_in=%u\n",zstrm->avail_in,zstrm->avail_out, zstrm->total_in);
        DEBUG3("next_in=%x, next_out=%x\n",zstrm->next_in,zstrm->next_out);
        ret = inflate(zstrm, Z_SYNC_FLUSH);
        if(ret == Z_NEED_DICT)
        {
          DEBUG1("zlib needs dict, retrying\n");
          inflateSetDictionary(zstrm, spdy_dictionary, sizeof(spdy_dictionary));
          ret = inflate(zstrm, Z_SYNC_FLUSH);
        }
        DEBUG3("inflate returned %d, msg: %s\n", ret, zstrm->msg);

        if(ret) { // fixme: handle mem error etc.
            return SPDY_HEADERS_ERROR_CORRUPT_DATA;
        }

        headers->data_length += initial_output_size -zstrm->avail_out;

        DEBUG4("in avail: %u, out avail=%u, total_in=%u\n",zstrm->avail_in,zstrm->avail_out, zstrm->total_in);

        if(zstrm->avail_in > 0) {
          output_size = output_size + initial_output_size;
          DEBUG2("need bigger inflate output buffer, reallocing to %u\n", output_size);

          uint8_t *oldptr = headers->data;
          headers->data = realloc(headers->data, output_size);

         zstrm->avail_out += initial_output_size;
         zstrm->next_out = headers->data + headers->data_length;
        }
    } while(zstrm->avail_in > 0);

    headers->entry_count = (headers->data[0] << 8) + headers->data[1];
    // fixme: check if count is correct

    DEBUG1("dumping uncompressed data:\n");
    for(n=0; n < headers->data_length; n++) {
      DEBUG2("%02x ",(uint8_t)headers->data[n]);
      if((n+1) % 32 == 0)
        DEBUG1("\n");
    }
    DEBUG1("\n");

    DEBUG1("inflate done.\n");
    return 0;
}




uint8_t *spdy_headers_iterate(spdy_headers_t *headers, uint8_t *position)
{
  if(position == 0)
  {
    if(headers->data == 0)
    {
      return 0;
    }
    else
    {
      return headers->data+2;
    }
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
  printf("Headers (%u entries):\n", headers->entry_count);

  uint8_t nbuf[SPDY_HEADERS_NAME_SIZE];
  uint8_t vbuf[SPDY_HEADERS_VALUE_SIZE];
  uint8_t *p=0;

  while((p = spdy_headers_iterate(headers, p)) && p > 0)
  {
    assert(spdy_headers_get(p, nbuf, vbuf) == 0);
    printf("  %s => %s\n", nbuf, vbuf);
  }
}

void spdy_headers_destroy(spdy_headers_t *headers)
{
  if(headers->data)
  {
    free(headers->data);
    headers->data = 0;
  }
}