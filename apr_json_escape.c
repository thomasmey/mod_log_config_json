/* Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "apr_json_escape.h"
#include "apr_escape_test_char.h"
#include "apr_lib.h"
#include "apr_strings.h"

/* we assume the folks using this ensure 0 <= c < 256... which means
 * you need a cast to (unsigned char) first, you can't just plug a
 * char in here and get it to work, because if char is signed then it
 * will first be sign extended.
 */
#define TEST_CHAR(c, f)        (test_char_table[(apr_uint16_t)(c)] & (f))

APR_DECLARE(apr_status_t) apr_escape_json(char *escaped, const char *str,
        apr_ssize_t slen, int quote, apr_size_t *len)
{
    apr_size_t size = 1;
    int found = quote;
    int error = 0;
    const unsigned char *s = (const unsigned char *) str;
    unsigned char *d = (unsigned char *) escaped;
    unsigned c;
    const char invalid[3] = { 0xEF, 0xBF, 0xBD };

    if (s) {
        if (d) {
            if (quote) {
                *d++ = '"';
                size += 1;
            }
            while ((c = *s) && slen) {
                if (TEST_CHAR(c, T_ESCAPE_JSON)) {
                    switch (c) {
                    case '\b':
                        *d++ = '\\';
                        *d++ = 'b';
                        size += 2;
                        found = 1;
                        break;
                    case '\f':
                        *d++ = '\\';
                        *d++ = 'f';
                        size += 2;
                        found = 1;
                        break;
                    case '\n':
                        *d++ = '\\';
                        *d++ = 'n';
                        size += 2;
                        found = 1;
                        break;
                    case '\r':
                        *d++ = '\\';
                        *d++ = 'r';
                        size += 2;
                        found = 1;
                        break;
                    case '\t':
                        *d++ = '\\';
                        *d++ = 't';
                        size += 2;
                        found = 1;
                        break;
                    case '\\':
                        *d++ = '\\';
                        *d++ = '\\';
                        size += 2;
                        found = 1;
                        break;
                    case '"':
                        *d++ = '\\';
                        *d++ = '"';
                        size += 2; 
                        found = 1;
                        break;
                    default:
                        if (c < 0x20) {
                            apr_snprintf((char *)d, 7, "\\u%04x", c);
                            size += 6;
                            d += 6; 
                            found = 1;
                        }
                        else if (((c >> 7) == 0x00)) {
                            /* 1 byte */
                            memcpy(d, s, 1);
                            d += 1;
                            size += 1;
                        }
                        else if ((slen < 0 || slen > 1) && ((c >> 5) == 0x06) && s[1]) {
                            /* 2 bytes */
                            if ((slen > 0 && slen < 2) || (s[1] >> 6) != 0x02) {
                                memcpy(d, invalid, sizeof(invalid));
                                d += sizeof(invalid);
                                size += sizeof(invalid);
                                found = error = 1;
                            }
                            else {
                                memcpy(d, s, 2);
                                d += 2;
                                size += 2;
                                s += 1;
                                slen -= 1;
                            }
                        }
                        else if (((c >> 4) == 0x0E)) {
                            /* 3 bytes */
                            if ((slen > 0 && slen < 3) || (s[1] >> 6) != 0x02 || (s[2] >> 6) != 0x02) {
                                memcpy(d, invalid, sizeof(invalid));
                                size += sizeof(invalid);
                                d += sizeof(invalid);
                                found = error = 1;
                            }
                            else {
                                memcpy(d, s, 3);
                                d += 3;
                                size += 3;
                                s += 2;
                                slen -= 2;
                            }
                        }
                        else if ((c >> 3) == 0x1E) {
                            /* 4 bytes */
                            if ((slen > 0 && slen < 4) || (s[1] >> 6) != 0x02 || (s[2] >> 6) != 0x02 || (s[3] >> 6) != 0x02) {
                                memcpy(d, invalid, sizeof(invalid));
                                d += sizeof(invalid);
                                size += sizeof(invalid);
                                found = error = 1;
                            }
                            else {
                                memcpy(d, s, 4);
                                d += 4;
                                size += 4;
                                s += 3;
                                slen -= 3;
                            }
                        }
                        else {
                            memcpy(d, invalid, sizeof(invalid));
                            d += sizeof(invalid);
                            size += sizeof(invalid);
                            found = error = 1;
                        }
                        break;
                    }
                }
                else {
                    *d++ = c;
                    size++;
                }
                ++s;
                slen--;
            }
            if (quote) {
                *d++ = '"';
                size += 1;
            }
            *d = '\0';
        }
        else {
            if (quote) {
                size += 1;
            }
            while ((c = *s) && slen) {
                if (TEST_CHAR(c, T_ESCAPE_JSON)) {
                    switch (c) {
                    case '\b':
                    case '\f':
                    case '\n':
                    case '\r':
                    case '\t':
                    case '\\':
                    case '"':
                        size += 2;
                        found = 1;
                        break;
                    default:
                        if (c < 0x20) {
                            size += 6; /* \uXXXX */
                            found = 1;
                        }
                        else if (((c >> 7) == 0x00)) {
                            /* 1 byte */
                            size += 1;
                        }
                        else if ((slen < 0 || slen > 1) && ((c >> 5) == 0x06) && s[1]) {
                            /* 2 bytes */
                            if ((slen > 0 && slen < 2) || (s[1] >> 6) != 0x02) {
                                size += sizeof(invalid);
                                found = error = 1;
                            }
                            else {
                                size += 2;
                                s += 1;
                                slen -= 1;
                            }
                        }
                        else if (((c >> 4) == 0x0E)) {
                            /* 3 bytes */
                            if ((slen > 0 && slen < 3) || (s[1] >> 6) != 0x02 || (s[2] >> 6) != 0x02) {
                                size += sizeof(invalid);
                                found = error = 1;
                            }
                            else {
                                size += 3;
                                s += 2;
                                slen -= 2;
                            }
                        }
                        else if ((c >> 3) == 0x1E) {
                            /* 4 bytes */
                            if ((slen > 0 && slen < 4) || (s[1] >> 6) != 0x02 || (s[2] >> 6) != 0x02 || (s[3] >> 6) != 0x02) {
                                size += sizeof(invalid);
                                found = error = 1;
                            }
                            else {
                                size += 4;
                                s += 3;
                                slen -= 3;
                            }
                        }
                        else {
                            size += sizeof(invalid);
                            found = error = 1;
                        }
                    }
                }
                else {
                    size++;
                }
                ++s;
                slen--;
            }
            if (quote) {
                size += 1;
            }
        }
    }

    else if (quote) {
        if (d) {
            memcpy(d, "null", 5);
        }
        else {
            size += 4;
        }
    }

    if (len) {
        *len = size;
    }
    if (error) {
        return APR_EINVAL;
    }
    if (!found) {
        return APR_NOTFOUND;
    }

    return APR_SUCCESS;
}

APR_DECLARE(const char *) apr_pescape_json(apr_pool_t *p, const char *src,
        apr_ssize_t srclen, int quote)
{
    apr_size_t len;

    switch (apr_escape_json(NULL, src, srclen, quote, &len)) {
    case APR_NOTFOUND: {
        break;
    }
    default: {
        char *encoded = apr_palloc(p, len);
        apr_escape_json(encoded, src, srclen, quote, NULL);
        return encoded;
    }
    }

    return src;
}
