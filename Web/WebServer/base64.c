

#include "base64.h"

/* C89 compliant way to cast 'char' to 'unsigned char'. */
unsigned char to_uchar (char ch)
{
  return ch;
}

void base64_encode (const char *in,unsigned int inlen,
               char *out, unsigned int outlen)
{
  static const char b64str[64] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  while (inlen && outlen)
    {
      *out++ = b64str[(to_uchar (in[0]) >> 2) & 0x3f];
      if (!--outlen)
        break;
      *out++ = b64str[((to_uchar (in[0]) << 4)
                       + (--inlen ? to_uchar (in[1]) >> 4 : 0))
                      & 0x3f];
      if (!--outlen)
        break;
      *out++ =
        (inlen
         ? b64str[((to_uchar (in[1]) << 2)
                   + (--inlen ? to_uchar (in[2]) >> 6 : 0))
                  & 0x3f]
         : '=');
      if (!--outlen)
        break;
      *out++ = inlen ? b64str[to_uchar (in[2]) & 0x3f] : '=';
      if (!--outlen)
        break;
      if (inlen)
        inlen--;
      if (inlen)
        in += 3;
    }

  if (outlen)
    *out = '\0';
}
