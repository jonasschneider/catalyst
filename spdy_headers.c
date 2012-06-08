#include <assert.h>
#include <zlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <malloc.h>
#include <stdarg.h>

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
        have += initial_output_size - strm.avail_out;

        headers->data_length += have;

        DEBUG3("in avail: %u, out avail=%u\n", strm.avail_in, strm.avail_out);
        DEBUG3("next avail byte: %x at %x\n", *strm.next_in, strm.next_in);
        

        int n;
        for(n=0; n < have; n++) {
          DEBUG2("%02x ",(uint8_t)headers->data[n]);
          if((n+1) % 32 == 0)
            DEBUG1("\n");
        }
        DEBUG1("\n");

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

    return ret == Z_OK ? 0 : -1;
}