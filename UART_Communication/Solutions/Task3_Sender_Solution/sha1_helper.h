#ifndef SHA1_HELPER_H
#define SHA1_HELPER_H

// ------------------------------------------------------------
// Minimal SHA-1 for Arduino (classic Nano and Nano Every)
//
// Usage:
//   char hash[41];
//   sha1_hex("Testing", hash);   // hash = "0820b32b...319acdfd"
//
// RAM-friendly: the message schedule uses a 16-word rolling
// buffer (64 bytes) instead of the textbook 80-word array
// (320 bytes), which matters on a 2 KB ATmega328P.
// ------------------------------------------------------------

#include <Arduino.h>
#include <stdint.h>
#include <string.h>

class SimpleSHA1 {
public:
  SimpleSHA1() { reset(); }

  void reset() {
    _dlen = 0; _bits = 0;
    _st[0] = 0x67452301UL; _st[1] = 0xEFCDAB89UL; _st[2] = 0x98BADCFEUL;
    _st[3] = 0x10325476UL; _st[4] = 0xC3D2E1F0UL;
  }

  void update(const uint8_t* data, size_t len) {
    for (size_t i = 0; i < len; i++) {
      _blk[_dlen++] = data[i];
      if (_dlen == 64) { transform(); _bits += 512; _dlen = 0; }
    }
  }

  void finalize(uint8_t out[20]) {
    _bits += (uint64_t)_dlen * 8ULL;
    uint8_t i = _dlen;
    _blk[i++] = 0x80;
    if (i > 56) {                        // no room for the length:
                                         // pad this block, start another
      while (i < 64) _blk[i++] = 0x00;
      transform();
      i = 0;
    }
    while (i < 56) _blk[i++] = 0x00;
    for (uint8_t j = 0; j < 8; j++) {    // 64-bit length, big-endian
      _blk[63 - j] = (uint8_t)(_bits >> (8 * j));
    }
    transform();
    for (uint8_t j = 0; j < 5; j++) {
      out[j * 4 + 0] = (uint8_t)(_st[j] >> 24);
      out[j * 4 + 1] = (uint8_t)(_st[j] >> 16);
      out[j * 4 + 2] = (uint8_t)(_st[j] >> 8);
      out[j * 4 + 3] = (uint8_t)(_st[j]);
    }
  }

private:
  uint8_t  _blk[64];
  uint8_t  _dlen;
  uint64_t _bits;
  uint32_t _st[5];

  static inline uint32_t rol(uint32_t x, uint8_t n) { return (x << n) | (x >> (32 - n)); }

  void transform() {
    uint32_t w[16];                      // rolling schedule: w[t & 15]
    for (uint8_t i = 0; i < 16; i++) {
      w[i] = ((uint32_t)_blk[i * 4] << 24) | ((uint32_t)_blk[i * 4 + 1] << 16) |
             ((uint32_t)_blk[i * 4 + 2] << 8) | _blk[i * 4 + 3];
    }
    uint32_t a = _st[0], b = _st[1], c = _st[2], d = _st[3], e = _st[4];
    for (uint8_t t = 0; t < 80; t++) {
      if (t >= 16) {
        w[t & 15] = rol(w[(t + 13) & 15] ^ w[(t + 8) & 15] ^
                        w[(t + 2) & 15] ^ w[t & 15], 1);
      }
      uint32_t f, k;
      if (t < 20)      { f = (b & c) | (~b & d);          k = 0x5A827999UL; }
      else if (t < 40) { f = b ^ c ^ d;                   k = 0x6ED9EBA1UL; }
      else if (t < 60) { f = (b & c) | (b & d) | (c & d); k = 0x8F1BBCDCUL; }
      else             { f = b ^ c ^ d;                   k = 0xCA62C1D6UL; }
      uint32_t tmp = rol(a, 5) + f + e + k + w[t & 15];
      e = d; d = c; c = rol(b, 30); b = a; a = tmp;
    }
    _st[0] += a; _st[1] += b; _st[2] += c; _st[3] += d; _st[4] += e;
  }
};

// Lowercase hex SHA-1 of a C string into outHex[41] (40 chars + '\0')
inline void sha1_hex(const char* msg, char outHex[41]) {
  static const char HEXCHARS[] = "0123456789abcdef";
  SimpleSHA1 s;
  uint8_t d[20];
  s.update((const uint8_t*)msg, strlen(msg));
  s.finalize(d);
  for (uint8_t i = 0; i < 20; i++) {
    outHex[i * 2]     = HEXCHARS[d[i] >> 4];
    outHex[i * 2 + 1] = HEXCHARS[d[i] & 0x0F];
  }
  outHex[40] = '\0';
}

#endif // SHA1_HELPER_H
