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

/**
 * @file apr_json_escape.h
 * @brief APR-UTIL JSON Escaping
 */
#ifndef APR_JSON_ESCAPE_H
#define APR_JSON_ESCAPE_H
#include "apu.h"
#include "apr_general.h"
#include "apr_escape.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup APR_Util_JSON_Escaping Escape functions
 * @ingroup APR
 * @{
 */

/**
 * Apply JSON escaping to a UTF string. Invalid UTF8 character sequences
 * are replaced by the U+FFFD replacement character.
 * @param dest The destination buffer, can be NULL
 * @param src The original buffer
 * @param srclen The length of the original buffer. Pass APR_ESCAPE_STRING
 * for a NUL terminated string.
 * @param quote If non zero, surround the string with quotes, and if the
 * string is NULL, return the string "NULL".
 * @param len If present, returns the length of the string
 * @return APR_SUCCESS, or APR_NOTFOUND if the string resulted in no
 * modification, APR_EINVAL if bad UTF8 is detected. In all cases valid
 * UTF8 is returned.
 */
APR_DECLARE(apr_status_t) apr_escape_json(char *dest, const char *src,
        apr_ssize_t srclen, int quote, apr_size_t *len);

/**
 * Apply JSON escaping to a UTF string. Invalid UTF8 character sequences
 * are replaced by the U+FFFD replacement character.
 * @param p Pool to allocate from
 * @param src The original buffer
 * @param srclen The length of the original buffer. Pass APR_ESCAPE_STRING
 * for a NUL terminated string.
 * @param quote If non zero, surround the string with quotes, and if the 
 * string is NULL, return the string "NULL". 
 * @return A zero padded buffer allocated from the pool on success, or
 * NULL if src was NULL.
 */
APR_DECLARE(const char *) apr_pescape_json(apr_pool_t *p, const char *src,
        apr_ssize_t srclen, int quote);

/** @} */
#ifdef __cplusplus
}
#endif

#endif	/* !APR_JSON_ESCAPE_H */
