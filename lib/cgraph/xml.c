#include <assert.h>
#include <cgraph/xml.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool starts_with(const char *s, const char *prefix) {
  return strncmp(s, prefix, strlen(prefix)) == 0;
}

// is this a UTF-8 continuation byte?
static bool is_continuation_byte(uint8_t byte) { return byte >> 6 == 2; }

// Decode a UTF-8 code point. Returns 0 on failure.
static uint32_t get_utf8(const char *s) {

  // is this a 1-byte char?
  if ((uint8_t)*s <= 0x7f) {
    return (uint8_t)*s;
  }

  // is this a 2-byte char?
  if ((uint8_t)*s <= 0xe0) {

    // is it malformed?
    if (!is_continuation_byte((uint8_t)s[1])) {
      return 0;
    }

    uint8_t byte1 = ((uint8_t)*s) & 0x1f;
    uint8_t byte2 = ((uint8_t)s[1]) & 0x3f;

    return (((uint32_t)byte1) << 6) | ((uint32_t)byte2);
  }

  // is this a 3-byte char?
  if ((uint8_t)*s <= 0xf0) {

    // is it malformed?
    if (!is_continuation_byte((uint8_t)s[1]) ||
        !is_continuation_byte((uint8_t)s[2])) {
      return 0;
    }

    uint8_t byte1 = ((uint8_t)*s) & 0xf;
    uint8_t byte2 = ((uint8_t)s[1]) & 0x3f;
    uint8_t byte3 = ((uint8_t)s[2]) & 0x3f;

    return (((uint32_t)byte1) << 12) | (((uint32_t)byte2) << 6) |
           ((uint32_t)byte3);
  }

  // is this a 4-byte char?
  if ((uint8_t)*s <= 0xf8) {

    // is it malformed?
    if (!is_continuation_byte((uint8_t)s[1]) ||
        !is_continuation_byte((uint8_t)s[2]) ||
        !is_continuation_byte((uint8_t)s[3])) {
      return 0;
    }

    uint8_t byte1 = ((uint8_t)*s) & 0x7;
    uint8_t byte2 = ((uint8_t)s[1]) & 0x3f;
    uint8_t byte3 = ((uint8_t)s[2]) & 0x3f;
    uint8_t byte4 = ((uint8_t)s[3]) & 0x3f;

    return (((uint32_t)byte1) << 18) | (((uint32_t)byte2) << 12) |
           (((uint32_t)byte3) << 6) | ((uint32_t)byte4);
  }

  // malformed
  return 0;
}

// how many bytes does the given code point take to encode in UTF-8?
static size_t bytes_for_utf8(uint32_t c) {
  if (c <= 0x7f) {
    return 1;
  }
  if (c <= 0x7ff) {
    return 2;
  }
  if (c <= 0xffff) {
    return 3;
  }
  return 4;
}

/** parse the starting character of an XML name
 *
 * This function recognizes something that matches NameStartChar from the XML
 * specification¹ and returns its length in bytes. If such a thing cannot be
 * recognized, 0 is returned.
 *
 * ¹ https://www.w3.org/TR/REC-xml/#NT-Name
 *
 * @param s String to scan
 * @returns Byte length of the recognized starting character or 0
 */
static size_t name_start_char_len(const char *s) {

  uint32_t c = get_utf8(s);

  // To comprehend the mapping from UTF-8 code points to byte length below:
  //
  //  * a 1-byte UTF-8 character has 7 bits of encoding space
  //  * a 2-byte UTF-8 character has 11 bits of encoding space
  //  * a 3-byte UTF-8 character has 16 bits of encoding space
  //  * a 4-byte UTF-8 character has 21 bits of encoding space
  //
  // So, e.g. the [0x37f,0x1fff] range is split into two cases, [0x37f,0x7ff]
  // that can be encoded into 2 UTF-8 bytes and [0x800,0x1fff] that takes 3
  // UTF-8 bytes to encode.

  if (c == ':') {
    return 1;
  }
  if (c >= 'A' && c <= 'Z') {
    return 1;
  }
  if (c == '_') {
    return 1;
  }
  if (c >= 'a' && c <= 'z') {
    return 1;
  }

  if (c >= 0xc0 && c <= 0xd6) {
    return 2;
  }

  if (c >= 0xd8 && c <= 0xf6) {
    return 2;
  }

  if (c >= 0xf8 && c <= 0x2ff) {
    return 2;
  }

  if (c >= 0x370 && c <= 0x37d) {
    return 2;
  }

  if (c >= 0x37f && c <= 0x7ff) {
    return 2;
  }
  if (c >= 0x800 && c <= 0x1fff) {
    return 3;
  }

  if (c >= 0x200c && c <= 0x200d) {
    return 3;
  }

  if (c >= 0x2070 && c <= 0x218f) {
    return 3;
  }

  if (c >= 0x2c00 && c <= 0x2fef) {
    return 3;
  }

  if (c >= 0x3001 && c <= 0xd7ff) {
    return 3;
  }

  if (c >= 0xf900 && c <= 0xfdcf) {
    return 3;
  }

  if (c >= 0xfdf0 && c <= 0xfffd) {
    return 3;
  }

  if (c >= 0x10000 && c <= 0xeffff) {
    return 4;
  }

  return 0;
}

/** parse a character of an XML name
 *
 * This function recognizes something that matches NameChar from the XML
 * specification¹ and returns its length in bytes. If such a thing cannot be
 * recognized, 0 is returned.
 *
 * ¹ https://www.w3.org/TR/REC-xml/#NT-Name
 *
 * @param s String to scan
 * @returns Byte length of the recognized starting character or 0
 */
static size_t name_char_len(const char *s) {

  size_t len = name_start_char_len(s);
  if (len != 0) {
    return len;
  }

  uint32_t c = get_utf8(s);

  // see comment in name_start_char explaining the logic below

  if (c == '-' || c == '.' || (c >= '0' && c <= '9') || c == 0xb7) {
    return 1;
  }

  if (c >= 0x0300 && c <= 0x036f) {
    return 2;
  }

  if (c >= 0x203f && c <= 0x2040) {
    return 3;
  }

  return 0;
}

/** is the given string an XML entity?
 *
 * Specifically, this checks if s matches any of the following:
 *
 *   * "&[A-Za-z]+;"      (e.g. &Ccedil; )
 *   * "&#[0-9]+;"        (e.g. &#38; )
 *   * "&#x[0-9a-fA-F]+;" (e.g. &#x6C34; )
 *
 * @param s String to examine
 * @return True if the string is an XML entity
 */
static bool is_xml_entity(const char *s) {

  // hexadecimal character reference
  if (starts_with(s, "&#x")) {
    for (const char *p = s + strlen("&#x"); *p != '\0'; ++p) {
      if (*p == ';') {
        // only accept non-0-length number
        return p != s + strlen("&#x");
      }
      if (!isxdigit(*p)) {
        return false;
      }
    }
    return false;
  }

  // decimal character reference
  if (starts_with(s, "&#")) {
    for (const char *p = s + strlen("&#"); *p != '\0'; ++p) {
      if (*p == ';') {
        // only accept non-0-length number
        return p != s + strlen("&#");
      }
      if (!isdigit(*p)) {
        return false;
      }
    }
    return false;
  }

  // character entity reference
  size_t offset = strlen("&");
  bool start = true;
  for (const char *p = s + offset; offset > 0; p += offset) {
    if (start) {
      offset = name_start_char_len(p);
      start = false;
    } else if (*p == ';') {
      return true;
    } else {
      offset = name_char_len(p);
    }
  }

  return false;
}

// characters required to print a number in hex
static size_t chars_for_num(uint32_t n) {

  if (n == 0) {
    return 1;
  }

#ifdef _WIN32
  size_t leading_zeroes = (size_t)__lzcnt(n);
#else
  assert(sizeof(unsigned long) >= sizeof(uint32_t));
  size_t leading_zeroes = (size_t)__builtin_clzl(n);
  leading_zeroes -= (sizeof(unsigned long) - sizeof(uint32_t)) * 8;
#endif

  return 8 - leading_zeroes / 8;
}

char *gv_xml_esc(const char *s, struct gv_xml_esc_options options) {

  // compute in advance how much output space we need
  size_t len = 0;
  for (const char *p = s; *p != '\0';) {
    switch (*p) {

#define X(guard, match, replacement)                                           \
  case match:                                                                  \
    if (guard) {                                                               \
      len += strlen(replacement);                                              \
    } else {                                                                   \
      ++len;                                                                   \
    }                                                                          \
    ++p;                                                                       \
    break;
#include "xml-map.inc"
#undef X

    default: {
      uint32_t c = get_utf8(p);

      // abort on malformed UTF-8 characters
      if (c == 0) {
        return NULL;
      }

      // ASCII characters, we can output as-is
      if (c <= 0x7f) {
        ++len;
        ++p;
        break;
      }

      // extended Unicode, we will hex-escape
      len += strlen("&#x") + chars_for_num(c) + strlen(";");
      p += bytes_for_utf8(c);
    }
    }
  }

  // account for '\0'
  ++len;

  // allocate space for the output
  char *r = malloc(sizeof(char) * len);
  if (r == NULL) {
    return NULL;
  }

  // do the actual escaping work
  size_t offset = 0;
  for (const char *p = s; *p != '\0';) {
    switch (*p) {

#define X(guard, match, replacement)                                           \
  case match:                                                                  \
    if (guard) {                                                               \
      strcpy(&r[offset], (replacement));                                       \
      offset += strlen(replacement);                                           \
    } else {                                                                   \
      r[offset] = *p;                                                          \
      ++offset;                                                                \
    }                                                                          \
    ++p;                                                                       \
    break;
#include "xml-map.inc"
#undef X

    default: {
      uint32_t c = get_utf8(p);

      assert(c != 0 && "malformed UTF-8 character not rejected in first pass");

      // ASCII characters, output as-is
      if (c <= 0x7f) {
        r[offset] = *p;
        ++offset;
        ++p;
        break;
      }

      // extended Unicode, we hex-escape
      int printed = sprintf(&r[offset], "&#x%x;", c);
      assert(printed >= 0 && "sprintf failed");
      assert((size_t)printed ==
                 strlen("&#x") + chars_for_num(c) + strlen(";") &&
             "printed characters does not match previous estimate");

      offset += (size_t)printed;
      p += bytes_for_utf8(c);
    }
    }
  }

  assert(offset == len - 1 &&
         "incorrect calculation of XML-escaped string size");

  r[offset] = '\0';

  return r;
}
