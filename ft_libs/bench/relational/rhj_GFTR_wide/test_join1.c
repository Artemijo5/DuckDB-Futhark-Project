
// We need to define _GNU_SOURCE before
// _any_ headers files are imported to get
// the usage statistics of a thread (i.e. have RUSAGE_THREAD) on GNU/Linux
// https://manpages.courier-mta.org/htmlman2/getrusage.2.html
#ifndef _GNU_SOURCE // Avoid possible double-definition warning.
#define _GNU_SOURCE
#endif

#ifdef __clang__
#pragma clang diagnostic ignored "-Wunused-function"
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-const-variable"
#pragma clang diagnostic ignored "-Wparentheses"
#pragma clang diagnostic ignored "-Wunused-label"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#elif __GNUC__
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-const-variable"
#pragma GCC diagnostic ignored "-Wparentheses"
#pragma GCC diagnostic ignored "-Wunused-label"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

// Headers
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <float.h>

#ifdef __cplusplus
extern "C" {
#endif

// Initialisation
struct futhark_context_config;
struct futhark_context_config *futhark_context_config_new(void);
void futhark_context_config_free(struct futhark_context_config *cfg);
int futhark_context_config_set_tuning_param(struct futhark_context_config *cfg, const char *param_name, size_t new_value);
struct futhark_context;
struct futhark_context *futhark_context_new(struct futhark_context_config *cfg);
void futhark_context_free(struct futhark_context *cfg);
void futhark_context_config_set_debugging(struct futhark_context_config *cfg, int flag);
void futhark_context_config_set_profiling(struct futhark_context_config *cfg, int flag);
void futhark_context_config_set_logging(struct futhark_context_config *cfg, int flag);
int futhark_get_tuning_param_count(void);
const char *futhark_get_tuning_param_name(int);
const char *futhark_get_tuning_param_class(int);

// Arrays
struct futhark_i32_1d;
struct futhark_i32_1d *futhark_new_i32_1d(struct futhark_context *ctx, const int32_t *data, int64_t dim0);
struct futhark_i32_1d *futhark_new_raw_i32_1d(struct futhark_context *ctx, unsigned char *data, int64_t dim0);
int futhark_free_i32_1d(struct futhark_context *ctx, struct futhark_i32_1d *arr);
int futhark_values_i32_1d(struct futhark_context *ctx, struct futhark_i32_1d *arr, int32_t *data);
int futhark_index_i32_1d(struct futhark_context *ctx, int32_t *out, struct futhark_i32_1d *arr, int64_t i0);
unsigned char *futhark_values_raw_i32_1d(struct futhark_context *ctx, struct futhark_i32_1d *arr);
const int64_t *futhark_shape_i32_1d(struct futhark_context *ctx, struct futhark_i32_1d *arr);
struct futhark_i64_1d;
struct futhark_i64_1d *futhark_new_i64_1d(struct futhark_context *ctx, const int64_t *data, int64_t dim0);
struct futhark_i64_1d *futhark_new_raw_i64_1d(struct futhark_context *ctx, unsigned char *data, int64_t dim0);
int futhark_free_i64_1d(struct futhark_context *ctx, struct futhark_i64_1d *arr);
int futhark_values_i64_1d(struct futhark_context *ctx, struct futhark_i64_1d *arr, int64_t *data);
int futhark_index_i64_1d(struct futhark_context *ctx, int64_t *out, struct futhark_i64_1d *arr, int64_t i0);
unsigned char *futhark_values_raw_i64_1d(struct futhark_context *ctx, struct futhark_i64_1d *arr);
const int64_t *futhark_shape_i64_1d(struct futhark_context *ctx, struct futhark_i64_1d *arr);
struct futhark_u8_2d;
struct futhark_u8_2d *futhark_new_u8_2d(struct futhark_context *ctx, const uint8_t *data, int64_t dim0, int64_t dim1);
struct futhark_u8_2d *futhark_new_raw_u8_2d(struct futhark_context *ctx, unsigned char *data, int64_t dim0, int64_t dim1);
int futhark_free_u8_2d(struct futhark_context *ctx, struct futhark_u8_2d *arr);
int futhark_values_u8_2d(struct futhark_context *ctx, struct futhark_u8_2d *arr, uint8_t *data);
int futhark_index_u8_2d(struct futhark_context *ctx, uint8_t *out, struct futhark_u8_2d *arr, int64_t i0, int64_t i1);
unsigned char *futhark_values_raw_u8_2d(struct futhark_context *ctx, struct futhark_u8_2d *arr);
const int64_t *futhark_shape_u8_2d(struct futhark_context *ctx, struct futhark_u8_2d *arr);

// Opaque values



// Entry points
int futhark_entry_rhj(struct futhark_context *ctx, struct futhark_u8_2d **out0, struct futhark_i64_1d **out1, struct futhark_i64_1d **out2, const struct futhark_u8_2d *in0, const struct futhark_u8_2d *in1, const struct futhark_u8_2d *in2, const struct futhark_u8_2d *in3, const int32_t in4, const struct futhark_i64_1d *in5, const struct futhark_i32_1d *in6, const struct futhark_i64_1d *in7, const struct futhark_i64_1d *in8);

// Miscellaneous
int futhark_context_sync(struct futhark_context *ctx);
void futhark_context_config_set_cache_file(struct futhark_context_config *cfg, const char *f);
char *futhark_context_get_error(struct futhark_context *ctx);
void futhark_context_set_logging_file(struct futhark_context *ctx, FILE *f);
void futhark_context_pause_profiling(struct futhark_context *ctx);
void futhark_context_unpause_profiling(struct futhark_context *ctx);
char *futhark_context_report(struct futhark_context *ctx);
int futhark_context_clear_caches(struct futhark_context *ctx);
#define FUTHARK_BACKEND_c
#define FUTHARK_SUCCESS 0
#define FUTHARK_PROGRAM_ERROR 2
#define FUTHARK_OUT_OF_MEMORY 3

#ifdef __cplusplus
}
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>
// If NDEBUG is set, the assert() macro will do nothing. Since Futhark
// (unfortunately) makes use of assert() for error detection (and even some
// side effects), we want to avoid that.
#undef NDEBUG
#include <assert.h>
#include <stdarg.h>
#define SCALAR_FUN_ATTR static inline
// Start of util.h.
//
// Various helper functions that are useful in all generated C code.

#include <errno.h>
#include <string.h>

static const char *fut_progname = "(embedded Futhark)";

static void futhark_panic(int eval, const char *fmt, ...) __attribute__((noreturn));
static char* msgprintf(const char *s, ...);
static void* slurp_file(const char *filename, size_t *size);
static int dump_file(const char *file, const void *buf, size_t n);
struct str_builder;
static void str_builder_init(struct str_builder *b);
static void str_builder(struct str_builder *b, const char *s, ...);
static char *strclone(const char *str);

static void futhark_panic(int eval, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  fprintf(stderr, "%s: ", fut_progname);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  exit(eval);
}

// For generating arbitrary-sized error messages.  It is the callers
// responsibility to free the buffer at some point.
static char* msgprintf(const char *s, ...) {
  va_list vl;
  va_start(vl, s);
  size_t needed = 1 + (size_t)vsnprintf(NULL, 0, s, vl);
  char *buffer = (char*) malloc(needed);
  va_start(vl, s); // Must re-init.
  vsnprintf(buffer, needed, s, vl);
  return buffer;
}

static inline void check_err(int errval, int sets_errno, const char *fun, int line,
                             const char *msg, ...) {
  if (errval) {
    char errnum[10];

    va_list vl;
    va_start(vl, msg);

    fprintf(stderr, "ERROR: ");
    vfprintf(stderr, msg, vl);
    fprintf(stderr, " in %s() at line %d with error code %s\n",
            fun, line,
            sets_errno ? strerror(errno) : errnum);
    exit(errval);
  }
}

#define CHECK_ERR(err, ...) check_err(err, 0, __func__, __LINE__, __VA_ARGS__)
#define CHECK_ERRNO(err, ...) check_err(err, 1, __func__, __LINE__, __VA_ARGS__)

// Read the rest of an open file into a NUL-terminated string; returns
// NULL on error.
static void* fslurp_file(FILE *f, size_t *size) {
  long start = ftell(f);
  fseek(f, 0, SEEK_END);
  long src_size = ftell(f)-start;
  fseek(f, start, SEEK_SET);
  unsigned char *s = (unsigned char*) malloc((size_t)src_size + 1);
  if (fread(s, 1, (size_t)src_size, f) != (size_t)src_size) {
    free(s);
    s = NULL;
  } else {
    s[src_size] = '\0';
  }

  if (size) {
    *size = (size_t)src_size;
  }

  return s;
}

// Read a file into a NUL-terminated string; returns NULL on error.
static void* slurp_file(const char *filename, size_t *size) {
  FILE *f = fopen(filename, "rb"); // To avoid Windows messing with linebreaks.
  if (f == NULL) return NULL;
  unsigned char *s = fslurp_file(f, size);
  fclose(f);
  return s;
}

// Dump 'n' bytes from 'buf' into the file at the designated location.
// Returns 0 on success.
static int dump_file(const char *file, const void *buf, size_t n) {
  FILE *f = fopen(file, "w");

  if (f == NULL) {
    return 1;
  }

  if (fwrite(buf, sizeof(char), n, f) != n) {
    return 1;
  }

  if (fclose(f) != 0) {
    return 1;
  }

  return 0;
}

struct str_builder {
  char *str;
  size_t capacity; // Size of buffer.
  size_t used; // Bytes used, *not* including final zero.
};

static void str_builder_init(struct str_builder *b) {
  b->capacity = 10;
  b->used = 0;
  b->str = malloc(b->capacity);
  b->str[0] = 0;
}

static void str_builder(struct str_builder *b, const char *s, ...) {
  va_list vl;
  va_start(vl, s);
  size_t needed = (size_t)vsnprintf(NULL, 0, s, vl);

  while (b->capacity < b->used + needed + 1) {
    b->capacity *= 2;
    b->str = realloc(b->str, b->capacity);
  }

  va_start(vl, s); // Must re-init.
  vsnprintf(b->str+b->used, b->capacity-b->used, s, vl);
  b->used += needed;
}

static void str_builder_str(struct str_builder *b, const char *s) {
  size_t needed = strlen(s);
  if (b->capacity < b->used + needed + 1) {
    b->capacity *= 2;
    b->str = realloc(b->str, b->capacity);
  }
  strcpy(b->str+b->used, s);
  b->used += needed;
}

static void str_builder_char(struct str_builder *b, char c) {
  size_t needed = 1;
  if (b->capacity < b->used + needed + 1) {
    b->capacity *= 2;
    b->str = realloc(b->str, b->capacity);
  }
  b->str[b->used] = c;
  b->str[b->used+1] = 0;
  b->used += needed;
}

static void str_builder_json_str(struct str_builder* sb, const char* s) {
  str_builder_char(sb, '"');
  for (int j = 0; s[j]; j++) {
    char c = s[j];
    switch (c) {
    case '\n':
      str_builder_str(sb, "\\n");
      break;
    case '"':
      str_builder_str(sb, "\\\"");
      break;
    default:
      str_builder_char(sb, c);
    }
  }
  str_builder_char(sb, '"');
}

static char *strclone(const char *str) {
  size_t size = strlen(str) + 1;
  char *copy = (char*) malloc(size);
  if (copy == NULL) {
    return NULL;
  }

  memcpy(copy, str, size);
  return copy;
}

// Assumes NULL-terminated.
static char *strconcat(const char *src_fragments[]) {
  size_t src_len = 0;
  const char **p;

  for (p = src_fragments; *p; p++) {
    src_len += strlen(*p);
  }

  char *src = (char*) malloc(src_len + 1);
  size_t n = 0;
  for (p = src_fragments; *p; p++) {
    strcpy(src + n, *p);
    n += strlen(*p);
  }

  return src;
}

// End of util.h.
// Start of cache.h

#define CACHE_HASH_SIZE 8 // In 32-bit words.

struct cache_hash {
  uint32_t hash[CACHE_HASH_SIZE];
};

// Initialise a blank cache.
static void cache_hash_init(struct cache_hash *c);

// Hash some bytes and add them to the accumulated hash.
static void cache_hash(struct cache_hash *out, const char *in, size_t n);

// Try to restore cache contents from a file with the given name.
// Assumes the cache is invalid if it contains the given hash.
// Allocates memory and reads the cache conents, which is returned in
// *buf with size *buflen.  If the cache is successfully loaded, this
// function returns 0.  Otherwise it returns nonzero.  Errno is set if
// the failure to load the cache is due to anything except invalid
// cache conents.  Note that failing to restore the cache is not
// necessarily a problem: it might just be invalid or not created yet.
static int cache_restore(const char *fname, const struct cache_hash *hash,
                         unsigned char **buf, size_t *buflen);

// Store cache contents in the given file, with the given hash.
static int cache_store(const char *fname, const struct cache_hash *hash,
                       const unsigned char *buf, size_t buflen);

// Now for the implementation.

static void cache_hash_init(struct cache_hash *c) {
  memset(c->hash, 0, CACHE_HASH_SIZE * sizeof(uint32_t));
}

static void cache_hash(struct cache_hash *out, const char *in, size_t n) {
  // Adaptation of djb2 for larger output size by storing intermediate
  // states.
  uint32_t hash = 5381;
  for (size_t i = 0; i < n; i++) {
    hash = ((hash << 5) + hash) + in[i];
    out->hash[i % CACHE_HASH_SIZE] ^= hash;
  }
}

#define CACHE_HEADER_SIZE 8
static const char cache_header[CACHE_HEADER_SIZE] = "FUTHARK\0";

static int cache_restore(const char *fname, const struct cache_hash *hash,
                         unsigned char **buf, size_t *buflen) {
  FILE *f = fopen(fname, "rb");

  if (f == NULL) {
    return 1;
  }

  char f_header[CACHE_HEADER_SIZE];

  if (fread(f_header, sizeof(char), CACHE_HEADER_SIZE, f) != CACHE_HEADER_SIZE) {
    goto error;
  }

  if (memcmp(f_header, cache_header, CACHE_HEADER_SIZE) != 0) {
    goto error;
  }

  if (fseek(f, 0, SEEK_END) != 0) {
    goto error;
  }
  int64_t f_size = (int64_t)ftell(f);
  if (fseek(f, CACHE_HEADER_SIZE, SEEK_SET) != 0) {
    goto error;
  }

  int64_t expected_size;

  if (fread(&expected_size, sizeof(int64_t), 1, f) != 1) {
    goto error;
  }

  if (f_size != expected_size) {
    errno = 0;
    goto error;
  }

  int32_t f_hash[CACHE_HASH_SIZE];

  if (fread(f_hash, sizeof(int32_t), CACHE_HASH_SIZE, f) != CACHE_HASH_SIZE) {
    goto error;
  }

  if (memcmp(f_hash, hash->hash, CACHE_HASH_SIZE) != 0) {
    errno = 0;
    goto error;
  }

  *buflen = f_size - CACHE_HEADER_SIZE - sizeof(int64_t) - CACHE_HASH_SIZE*sizeof(int32_t);
  *buf = malloc(*buflen);
  if (fread(*buf, sizeof(char), *buflen, f) != *buflen) {
    free(*buf);
    goto error;
  }

  fclose(f);

  return 0;

 error:
  fclose(f);
  return 1;
}

static int cache_store(const char *fname, const struct cache_hash *hash,
                       const unsigned char *buf, size_t buflen) {
  FILE *f = fopen(fname, "wb");

  if (f == NULL) {
    return 1;
  }

  if (fwrite(cache_header, CACHE_HEADER_SIZE, 1, f) != 1) {
    goto error;
  }

  int64_t size = CACHE_HEADER_SIZE + sizeof(int64_t) + CACHE_HASH_SIZE*sizeof(int32_t) + buflen;

  if (fwrite(&size, sizeof(size), 1, f) != 1) {
    goto error;
  }

  if (fwrite(hash->hash, sizeof(int32_t), CACHE_HASH_SIZE, f) != CACHE_HASH_SIZE) {
    goto error;
  }

  if (fwrite(buf, sizeof(unsigned char), buflen, f) != buflen) {
    goto error;
  }

  fclose(f);

  return 0;

 error:
  fclose(f);
  return 1;
}

// End of cache.h
// Start of half.h.

// Conversion functions are from http://half.sourceforge.net/, but
// translated to C.
//
// Copyright (c) 2012-2021 Christian Rau
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef __OPENCL_VERSION__
#define __constant
#endif

__constant static const uint16_t base_table[512] = {
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100,
  0x0200, 0x0400, 0x0800, 0x0C00, 0x1000, 0x1400, 0x1800, 0x1C00, 0x2000, 0x2400, 0x2800, 0x2C00, 0x3000, 0x3400, 0x3800, 0x3C00,
  0x4000, 0x4400, 0x4800, 0x4C00, 0x5000, 0x5400, 0x5800, 0x5C00, 0x6000, 0x6400, 0x6800, 0x6C00, 0x7000, 0x7400, 0x7800, 0x7C00,
  0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00,
  0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00,
  0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00,
  0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00,
  0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00,
  0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00,
  0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00, 0x7C00,
  0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
  0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
  0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
  0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
  0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
  0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
  0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8001, 0x8002, 0x8004, 0x8008, 0x8010, 0x8020, 0x8040, 0x8080, 0x8100,
  0x8200, 0x8400, 0x8800, 0x8C00, 0x9000, 0x9400, 0x9800, 0x9C00, 0xA000, 0xA400, 0xA800, 0xAC00, 0xB000, 0xB400, 0xB800, 0xBC00,
  0xC000, 0xC400, 0xC800, 0xCC00, 0xD000, 0xD400, 0xD800, 0xDC00, 0xE000, 0xE400, 0xE800, 0xEC00, 0xF000, 0xF400, 0xF800, 0xFC00,
  0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00,
  0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00,
  0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00,
  0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00,
  0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00,
  0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00,
  0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00, 0xFC00 };

__constant static const unsigned char shift_table[512] = {
  24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
  24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
  24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
  24, 24, 24, 24, 24, 24, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
  13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
  24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
  24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
  24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 13,
  24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
  24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
  24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
  24, 24, 24, 24, 24, 24, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
  13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
  24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
  24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
  24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 13 };

__constant static const uint32_t mantissa_table[2048] = {
  0x00000000, 0x33800000, 0x34000000, 0x34400000, 0x34800000, 0x34A00000, 0x34C00000, 0x34E00000, 0x35000000, 0x35100000, 0x35200000, 0x35300000, 0x35400000, 0x35500000, 0x35600000, 0x35700000,
  0x35800000, 0x35880000, 0x35900000, 0x35980000, 0x35A00000, 0x35A80000, 0x35B00000, 0x35B80000, 0x35C00000, 0x35C80000, 0x35D00000, 0x35D80000, 0x35E00000, 0x35E80000, 0x35F00000, 0x35F80000,
  0x36000000, 0x36040000, 0x36080000, 0x360C0000, 0x36100000, 0x36140000, 0x36180000, 0x361C0000, 0x36200000, 0x36240000, 0x36280000, 0x362C0000, 0x36300000, 0x36340000, 0x36380000, 0x363C0000,
  0x36400000, 0x36440000, 0x36480000, 0x364C0000, 0x36500000, 0x36540000, 0x36580000, 0x365C0000, 0x36600000, 0x36640000, 0x36680000, 0x366C0000, 0x36700000, 0x36740000, 0x36780000, 0x367C0000,
  0x36800000, 0x36820000, 0x36840000, 0x36860000, 0x36880000, 0x368A0000, 0x368C0000, 0x368E0000, 0x36900000, 0x36920000, 0x36940000, 0x36960000, 0x36980000, 0x369A0000, 0x369C0000, 0x369E0000,
  0x36A00000, 0x36A20000, 0x36A40000, 0x36A60000, 0x36A80000, 0x36AA0000, 0x36AC0000, 0x36AE0000, 0x36B00000, 0x36B20000, 0x36B40000, 0x36B60000, 0x36B80000, 0x36BA0000, 0x36BC0000, 0x36BE0000,
  0x36C00000, 0x36C20000, 0x36C40000, 0x36C60000, 0x36C80000, 0x36CA0000, 0x36CC0000, 0x36CE0000, 0x36D00000, 0x36D20000, 0x36D40000, 0x36D60000, 0x36D80000, 0x36DA0000, 0x36DC0000, 0x36DE0000,
  0x36E00000, 0x36E20000, 0x36E40000, 0x36E60000, 0x36E80000, 0x36EA0000, 0x36EC0000, 0x36EE0000, 0x36F00000, 0x36F20000, 0x36F40000, 0x36F60000, 0x36F80000, 0x36FA0000, 0x36FC0000, 0x36FE0000,
  0x37000000, 0x37010000, 0x37020000, 0x37030000, 0x37040000, 0x37050000, 0x37060000, 0x37070000, 0x37080000, 0x37090000, 0x370A0000, 0x370B0000, 0x370C0000, 0x370D0000, 0x370E0000, 0x370F0000,
  0x37100000, 0x37110000, 0x37120000, 0x37130000, 0x37140000, 0x37150000, 0x37160000, 0x37170000, 0x37180000, 0x37190000, 0x371A0000, 0x371B0000, 0x371C0000, 0x371D0000, 0x371E0000, 0x371F0000,
  0x37200000, 0x37210000, 0x37220000, 0x37230000, 0x37240000, 0x37250000, 0x37260000, 0x37270000, 0x37280000, 0x37290000, 0x372A0000, 0x372B0000, 0x372C0000, 0x372D0000, 0x372E0000, 0x372F0000,
  0x37300000, 0x37310000, 0x37320000, 0x37330000, 0x37340000, 0x37350000, 0x37360000, 0x37370000, 0x37380000, 0x37390000, 0x373A0000, 0x373B0000, 0x373C0000, 0x373D0000, 0x373E0000, 0x373F0000,
  0x37400000, 0x37410000, 0x37420000, 0x37430000, 0x37440000, 0x37450000, 0x37460000, 0x37470000, 0x37480000, 0x37490000, 0x374A0000, 0x374B0000, 0x374C0000, 0x374D0000, 0x374E0000, 0x374F0000,
  0x37500000, 0x37510000, 0x37520000, 0x37530000, 0x37540000, 0x37550000, 0x37560000, 0x37570000, 0x37580000, 0x37590000, 0x375A0000, 0x375B0000, 0x375C0000, 0x375D0000, 0x375E0000, 0x375F0000,
  0x37600000, 0x37610000, 0x37620000, 0x37630000, 0x37640000, 0x37650000, 0x37660000, 0x37670000, 0x37680000, 0x37690000, 0x376A0000, 0x376B0000, 0x376C0000, 0x376D0000, 0x376E0000, 0x376F0000,
  0x37700000, 0x37710000, 0x37720000, 0x37730000, 0x37740000, 0x37750000, 0x37760000, 0x37770000, 0x37780000, 0x37790000, 0x377A0000, 0x377B0000, 0x377C0000, 0x377D0000, 0x377E0000, 0x377F0000,
  0x37800000, 0x37808000, 0x37810000, 0x37818000, 0x37820000, 0x37828000, 0x37830000, 0x37838000, 0x37840000, 0x37848000, 0x37850000, 0x37858000, 0x37860000, 0x37868000, 0x37870000, 0x37878000,
  0x37880000, 0x37888000, 0x37890000, 0x37898000, 0x378A0000, 0x378A8000, 0x378B0000, 0x378B8000, 0x378C0000, 0x378C8000, 0x378D0000, 0x378D8000, 0x378E0000, 0x378E8000, 0x378F0000, 0x378F8000,
  0x37900000, 0x37908000, 0x37910000, 0x37918000, 0x37920000, 0x37928000, 0x37930000, 0x37938000, 0x37940000, 0x37948000, 0x37950000, 0x37958000, 0x37960000, 0x37968000, 0x37970000, 0x37978000,
  0x37980000, 0x37988000, 0x37990000, 0x37998000, 0x379A0000, 0x379A8000, 0x379B0000, 0x379B8000, 0x379C0000, 0x379C8000, 0x379D0000, 0x379D8000, 0x379E0000, 0x379E8000, 0x379F0000, 0x379F8000,
  0x37A00000, 0x37A08000, 0x37A10000, 0x37A18000, 0x37A20000, 0x37A28000, 0x37A30000, 0x37A38000, 0x37A40000, 0x37A48000, 0x37A50000, 0x37A58000, 0x37A60000, 0x37A68000, 0x37A70000, 0x37A78000,
  0x37A80000, 0x37A88000, 0x37A90000, 0x37A98000, 0x37AA0000, 0x37AA8000, 0x37AB0000, 0x37AB8000, 0x37AC0000, 0x37AC8000, 0x37AD0000, 0x37AD8000, 0x37AE0000, 0x37AE8000, 0x37AF0000, 0x37AF8000,
  0x37B00000, 0x37B08000, 0x37B10000, 0x37B18000, 0x37B20000, 0x37B28000, 0x37B30000, 0x37B38000, 0x37B40000, 0x37B48000, 0x37B50000, 0x37B58000, 0x37B60000, 0x37B68000, 0x37B70000, 0x37B78000,
  0x37B80000, 0x37B88000, 0x37B90000, 0x37B98000, 0x37BA0000, 0x37BA8000, 0x37BB0000, 0x37BB8000, 0x37BC0000, 0x37BC8000, 0x37BD0000, 0x37BD8000, 0x37BE0000, 0x37BE8000, 0x37BF0000, 0x37BF8000,
  0x37C00000, 0x37C08000, 0x37C10000, 0x37C18000, 0x37C20000, 0x37C28000, 0x37C30000, 0x37C38000, 0x37C40000, 0x37C48000, 0x37C50000, 0x37C58000, 0x37C60000, 0x37C68000, 0x37C70000, 0x37C78000,
  0x37C80000, 0x37C88000, 0x37C90000, 0x37C98000, 0x37CA0000, 0x37CA8000, 0x37CB0000, 0x37CB8000, 0x37CC0000, 0x37CC8000, 0x37CD0000, 0x37CD8000, 0x37CE0000, 0x37CE8000, 0x37CF0000, 0x37CF8000,
  0x37D00000, 0x37D08000, 0x37D10000, 0x37D18000, 0x37D20000, 0x37D28000, 0x37D30000, 0x37D38000, 0x37D40000, 0x37D48000, 0x37D50000, 0x37D58000, 0x37D60000, 0x37D68000, 0x37D70000, 0x37D78000,
  0x37D80000, 0x37D88000, 0x37D90000, 0x37D98000, 0x37DA0000, 0x37DA8000, 0x37DB0000, 0x37DB8000, 0x37DC0000, 0x37DC8000, 0x37DD0000, 0x37DD8000, 0x37DE0000, 0x37DE8000, 0x37DF0000, 0x37DF8000,
  0x37E00000, 0x37E08000, 0x37E10000, 0x37E18000, 0x37E20000, 0x37E28000, 0x37E30000, 0x37E38000, 0x37E40000, 0x37E48000, 0x37E50000, 0x37E58000, 0x37E60000, 0x37E68000, 0x37E70000, 0x37E78000,
  0x37E80000, 0x37E88000, 0x37E90000, 0x37E98000, 0x37EA0000, 0x37EA8000, 0x37EB0000, 0x37EB8000, 0x37EC0000, 0x37EC8000, 0x37ED0000, 0x37ED8000, 0x37EE0000, 0x37EE8000, 0x37EF0000, 0x37EF8000,
  0x37F00000, 0x37F08000, 0x37F10000, 0x37F18000, 0x37F20000, 0x37F28000, 0x37F30000, 0x37F38000, 0x37F40000, 0x37F48000, 0x37F50000, 0x37F58000, 0x37F60000, 0x37F68000, 0x37F70000, 0x37F78000,
  0x37F80000, 0x37F88000, 0x37F90000, 0x37F98000, 0x37FA0000, 0x37FA8000, 0x37FB0000, 0x37FB8000, 0x37FC0000, 0x37FC8000, 0x37FD0000, 0x37FD8000, 0x37FE0000, 0x37FE8000, 0x37FF0000, 0x37FF8000,
  0x38000000, 0x38004000, 0x38008000, 0x3800C000, 0x38010000, 0x38014000, 0x38018000, 0x3801C000, 0x38020000, 0x38024000, 0x38028000, 0x3802C000, 0x38030000, 0x38034000, 0x38038000, 0x3803C000,
  0x38040000, 0x38044000, 0x38048000, 0x3804C000, 0x38050000, 0x38054000, 0x38058000, 0x3805C000, 0x38060000, 0x38064000, 0x38068000, 0x3806C000, 0x38070000, 0x38074000, 0x38078000, 0x3807C000,
  0x38080000, 0x38084000, 0x38088000, 0x3808C000, 0x38090000, 0x38094000, 0x38098000, 0x3809C000, 0x380A0000, 0x380A4000, 0x380A8000, 0x380AC000, 0x380B0000, 0x380B4000, 0x380B8000, 0x380BC000,
  0x380C0000, 0x380C4000, 0x380C8000, 0x380CC000, 0x380D0000, 0x380D4000, 0x380D8000, 0x380DC000, 0x380E0000, 0x380E4000, 0x380E8000, 0x380EC000, 0x380F0000, 0x380F4000, 0x380F8000, 0x380FC000,
  0x38100000, 0x38104000, 0x38108000, 0x3810C000, 0x38110000, 0x38114000, 0x38118000, 0x3811C000, 0x38120000, 0x38124000, 0x38128000, 0x3812C000, 0x38130000, 0x38134000, 0x38138000, 0x3813C000,
  0x38140000, 0x38144000, 0x38148000, 0x3814C000, 0x38150000, 0x38154000, 0x38158000, 0x3815C000, 0x38160000, 0x38164000, 0x38168000, 0x3816C000, 0x38170000, 0x38174000, 0x38178000, 0x3817C000,
  0x38180000, 0x38184000, 0x38188000, 0x3818C000, 0x38190000, 0x38194000, 0x38198000, 0x3819C000, 0x381A0000, 0x381A4000, 0x381A8000, 0x381AC000, 0x381B0000, 0x381B4000, 0x381B8000, 0x381BC000,
  0x381C0000, 0x381C4000, 0x381C8000, 0x381CC000, 0x381D0000, 0x381D4000, 0x381D8000, 0x381DC000, 0x381E0000, 0x381E4000, 0x381E8000, 0x381EC000, 0x381F0000, 0x381F4000, 0x381F8000, 0x381FC000,
  0x38200000, 0x38204000, 0x38208000, 0x3820C000, 0x38210000, 0x38214000, 0x38218000, 0x3821C000, 0x38220000, 0x38224000, 0x38228000, 0x3822C000, 0x38230000, 0x38234000, 0x38238000, 0x3823C000,
  0x38240000, 0x38244000, 0x38248000, 0x3824C000, 0x38250000, 0x38254000, 0x38258000, 0x3825C000, 0x38260000, 0x38264000, 0x38268000, 0x3826C000, 0x38270000, 0x38274000, 0x38278000, 0x3827C000,
  0x38280000, 0x38284000, 0x38288000, 0x3828C000, 0x38290000, 0x38294000, 0x38298000, 0x3829C000, 0x382A0000, 0x382A4000, 0x382A8000, 0x382AC000, 0x382B0000, 0x382B4000, 0x382B8000, 0x382BC000,
  0x382C0000, 0x382C4000, 0x382C8000, 0x382CC000, 0x382D0000, 0x382D4000, 0x382D8000, 0x382DC000, 0x382E0000, 0x382E4000, 0x382E8000, 0x382EC000, 0x382F0000, 0x382F4000, 0x382F8000, 0x382FC000,
  0x38300000, 0x38304000, 0x38308000, 0x3830C000, 0x38310000, 0x38314000, 0x38318000, 0x3831C000, 0x38320000, 0x38324000, 0x38328000, 0x3832C000, 0x38330000, 0x38334000, 0x38338000, 0x3833C000,
  0x38340000, 0x38344000, 0x38348000, 0x3834C000, 0x38350000, 0x38354000, 0x38358000, 0x3835C000, 0x38360000, 0x38364000, 0x38368000, 0x3836C000, 0x38370000, 0x38374000, 0x38378000, 0x3837C000,
  0x38380000, 0x38384000, 0x38388000, 0x3838C000, 0x38390000, 0x38394000, 0x38398000, 0x3839C000, 0x383A0000, 0x383A4000, 0x383A8000, 0x383AC000, 0x383B0000, 0x383B4000, 0x383B8000, 0x383BC000,
  0x383C0000, 0x383C4000, 0x383C8000, 0x383CC000, 0x383D0000, 0x383D4000, 0x383D8000, 0x383DC000, 0x383E0000, 0x383E4000, 0x383E8000, 0x383EC000, 0x383F0000, 0x383F4000, 0x383F8000, 0x383FC000,
  0x38400000, 0x38404000, 0x38408000, 0x3840C000, 0x38410000, 0x38414000, 0x38418000, 0x3841C000, 0x38420000, 0x38424000, 0x38428000, 0x3842C000, 0x38430000, 0x38434000, 0x38438000, 0x3843C000,
  0x38440000, 0x38444000, 0x38448000, 0x3844C000, 0x38450000, 0x38454000, 0x38458000, 0x3845C000, 0x38460000, 0x38464000, 0x38468000, 0x3846C000, 0x38470000, 0x38474000, 0x38478000, 0x3847C000,
  0x38480000, 0x38484000, 0x38488000, 0x3848C000, 0x38490000, 0x38494000, 0x38498000, 0x3849C000, 0x384A0000, 0x384A4000, 0x384A8000, 0x384AC000, 0x384B0000, 0x384B4000, 0x384B8000, 0x384BC000,
  0x384C0000, 0x384C4000, 0x384C8000, 0x384CC000, 0x384D0000, 0x384D4000, 0x384D8000, 0x384DC000, 0x384E0000, 0x384E4000, 0x384E8000, 0x384EC000, 0x384F0000, 0x384F4000, 0x384F8000, 0x384FC000,
  0x38500000, 0x38504000, 0x38508000, 0x3850C000, 0x38510000, 0x38514000, 0x38518000, 0x3851C000, 0x38520000, 0x38524000, 0x38528000, 0x3852C000, 0x38530000, 0x38534000, 0x38538000, 0x3853C000,
  0x38540000, 0x38544000, 0x38548000, 0x3854C000, 0x38550000, 0x38554000, 0x38558000, 0x3855C000, 0x38560000, 0x38564000, 0x38568000, 0x3856C000, 0x38570000, 0x38574000, 0x38578000, 0x3857C000,
  0x38580000, 0x38584000, 0x38588000, 0x3858C000, 0x38590000, 0x38594000, 0x38598000, 0x3859C000, 0x385A0000, 0x385A4000, 0x385A8000, 0x385AC000, 0x385B0000, 0x385B4000, 0x385B8000, 0x385BC000,
  0x385C0000, 0x385C4000, 0x385C8000, 0x385CC000, 0x385D0000, 0x385D4000, 0x385D8000, 0x385DC000, 0x385E0000, 0x385E4000, 0x385E8000, 0x385EC000, 0x385F0000, 0x385F4000, 0x385F8000, 0x385FC000,
  0x38600000, 0x38604000, 0x38608000, 0x3860C000, 0x38610000, 0x38614000, 0x38618000, 0x3861C000, 0x38620000, 0x38624000, 0x38628000, 0x3862C000, 0x38630000, 0x38634000, 0x38638000, 0x3863C000,
  0x38640000, 0x38644000, 0x38648000, 0x3864C000, 0x38650000, 0x38654000, 0x38658000, 0x3865C000, 0x38660000, 0x38664000, 0x38668000, 0x3866C000, 0x38670000, 0x38674000, 0x38678000, 0x3867C000,
  0x38680000, 0x38684000, 0x38688000, 0x3868C000, 0x38690000, 0x38694000, 0x38698000, 0x3869C000, 0x386A0000, 0x386A4000, 0x386A8000, 0x386AC000, 0x386B0000, 0x386B4000, 0x386B8000, 0x386BC000,
  0x386C0000, 0x386C4000, 0x386C8000, 0x386CC000, 0x386D0000, 0x386D4000, 0x386D8000, 0x386DC000, 0x386E0000, 0x386E4000, 0x386E8000, 0x386EC000, 0x386F0000, 0x386F4000, 0x386F8000, 0x386FC000,
  0x38700000, 0x38704000, 0x38708000, 0x3870C000, 0x38710000, 0x38714000, 0x38718000, 0x3871C000, 0x38720000, 0x38724000, 0x38728000, 0x3872C000, 0x38730000, 0x38734000, 0x38738000, 0x3873C000,
  0x38740000, 0x38744000, 0x38748000, 0x3874C000, 0x38750000, 0x38754000, 0x38758000, 0x3875C000, 0x38760000, 0x38764000, 0x38768000, 0x3876C000, 0x38770000, 0x38774000, 0x38778000, 0x3877C000,
  0x38780000, 0x38784000, 0x38788000, 0x3878C000, 0x38790000, 0x38794000, 0x38798000, 0x3879C000, 0x387A0000, 0x387A4000, 0x387A8000, 0x387AC000, 0x387B0000, 0x387B4000, 0x387B8000, 0x387BC000,
  0x387C0000, 0x387C4000, 0x387C8000, 0x387CC000, 0x387D0000, 0x387D4000, 0x387D8000, 0x387DC000, 0x387E0000, 0x387E4000, 0x387E8000, 0x387EC000, 0x387F0000, 0x387F4000, 0x387F8000, 0x387FC000,
  0x38000000, 0x38002000, 0x38004000, 0x38006000, 0x38008000, 0x3800A000, 0x3800C000, 0x3800E000, 0x38010000, 0x38012000, 0x38014000, 0x38016000, 0x38018000, 0x3801A000, 0x3801C000, 0x3801E000,
  0x38020000, 0x38022000, 0x38024000, 0x38026000, 0x38028000, 0x3802A000, 0x3802C000, 0x3802E000, 0x38030000, 0x38032000, 0x38034000, 0x38036000, 0x38038000, 0x3803A000, 0x3803C000, 0x3803E000,
  0x38040000, 0x38042000, 0x38044000, 0x38046000, 0x38048000, 0x3804A000, 0x3804C000, 0x3804E000, 0x38050000, 0x38052000, 0x38054000, 0x38056000, 0x38058000, 0x3805A000, 0x3805C000, 0x3805E000,
  0x38060000, 0x38062000, 0x38064000, 0x38066000, 0x38068000, 0x3806A000, 0x3806C000, 0x3806E000, 0x38070000, 0x38072000, 0x38074000, 0x38076000, 0x38078000, 0x3807A000, 0x3807C000, 0x3807E000,
  0x38080000, 0x38082000, 0x38084000, 0x38086000, 0x38088000, 0x3808A000, 0x3808C000, 0x3808E000, 0x38090000, 0x38092000, 0x38094000, 0x38096000, 0x38098000, 0x3809A000, 0x3809C000, 0x3809E000,
  0x380A0000, 0x380A2000, 0x380A4000, 0x380A6000, 0x380A8000, 0x380AA000, 0x380AC000, 0x380AE000, 0x380B0000, 0x380B2000, 0x380B4000, 0x380B6000, 0x380B8000, 0x380BA000, 0x380BC000, 0x380BE000,
  0x380C0000, 0x380C2000, 0x380C4000, 0x380C6000, 0x380C8000, 0x380CA000, 0x380CC000, 0x380CE000, 0x380D0000, 0x380D2000, 0x380D4000, 0x380D6000, 0x380D8000, 0x380DA000, 0x380DC000, 0x380DE000,
  0x380E0000, 0x380E2000, 0x380E4000, 0x380E6000, 0x380E8000, 0x380EA000, 0x380EC000, 0x380EE000, 0x380F0000, 0x380F2000, 0x380F4000, 0x380F6000, 0x380F8000, 0x380FA000, 0x380FC000, 0x380FE000,
  0x38100000, 0x38102000, 0x38104000, 0x38106000, 0x38108000, 0x3810A000, 0x3810C000, 0x3810E000, 0x38110000, 0x38112000, 0x38114000, 0x38116000, 0x38118000, 0x3811A000, 0x3811C000, 0x3811E000,
  0x38120000, 0x38122000, 0x38124000, 0x38126000, 0x38128000, 0x3812A000, 0x3812C000, 0x3812E000, 0x38130000, 0x38132000, 0x38134000, 0x38136000, 0x38138000, 0x3813A000, 0x3813C000, 0x3813E000,
  0x38140000, 0x38142000, 0x38144000, 0x38146000, 0x38148000, 0x3814A000, 0x3814C000, 0x3814E000, 0x38150000, 0x38152000, 0x38154000, 0x38156000, 0x38158000, 0x3815A000, 0x3815C000, 0x3815E000,
  0x38160000, 0x38162000, 0x38164000, 0x38166000, 0x38168000, 0x3816A000, 0x3816C000, 0x3816E000, 0x38170000, 0x38172000, 0x38174000, 0x38176000, 0x38178000, 0x3817A000, 0x3817C000, 0x3817E000,
  0x38180000, 0x38182000, 0x38184000, 0x38186000, 0x38188000, 0x3818A000, 0x3818C000, 0x3818E000, 0x38190000, 0x38192000, 0x38194000, 0x38196000, 0x38198000, 0x3819A000, 0x3819C000, 0x3819E000,
  0x381A0000, 0x381A2000, 0x381A4000, 0x381A6000, 0x381A8000, 0x381AA000, 0x381AC000, 0x381AE000, 0x381B0000, 0x381B2000, 0x381B4000, 0x381B6000, 0x381B8000, 0x381BA000, 0x381BC000, 0x381BE000,
  0x381C0000, 0x381C2000, 0x381C4000, 0x381C6000, 0x381C8000, 0x381CA000, 0x381CC000, 0x381CE000, 0x381D0000, 0x381D2000, 0x381D4000, 0x381D6000, 0x381D8000, 0x381DA000, 0x381DC000, 0x381DE000,
  0x381E0000, 0x381E2000, 0x381E4000, 0x381E6000, 0x381E8000, 0x381EA000, 0x381EC000, 0x381EE000, 0x381F0000, 0x381F2000, 0x381F4000, 0x381F6000, 0x381F8000, 0x381FA000, 0x381FC000, 0x381FE000,
  0x38200000, 0x38202000, 0x38204000, 0x38206000, 0x38208000, 0x3820A000, 0x3820C000, 0x3820E000, 0x38210000, 0x38212000, 0x38214000, 0x38216000, 0x38218000, 0x3821A000, 0x3821C000, 0x3821E000,
  0x38220000, 0x38222000, 0x38224000, 0x38226000, 0x38228000, 0x3822A000, 0x3822C000, 0x3822E000, 0x38230000, 0x38232000, 0x38234000, 0x38236000, 0x38238000, 0x3823A000, 0x3823C000, 0x3823E000,
  0x38240000, 0x38242000, 0x38244000, 0x38246000, 0x38248000, 0x3824A000, 0x3824C000, 0x3824E000, 0x38250000, 0x38252000, 0x38254000, 0x38256000, 0x38258000, 0x3825A000, 0x3825C000, 0x3825E000,
  0x38260000, 0x38262000, 0x38264000, 0x38266000, 0x38268000, 0x3826A000, 0x3826C000, 0x3826E000, 0x38270000, 0x38272000, 0x38274000, 0x38276000, 0x38278000, 0x3827A000, 0x3827C000, 0x3827E000,
  0x38280000, 0x38282000, 0x38284000, 0x38286000, 0x38288000, 0x3828A000, 0x3828C000, 0x3828E000, 0x38290000, 0x38292000, 0x38294000, 0x38296000, 0x38298000, 0x3829A000, 0x3829C000, 0x3829E000,
  0x382A0000, 0x382A2000, 0x382A4000, 0x382A6000, 0x382A8000, 0x382AA000, 0x382AC000, 0x382AE000, 0x382B0000, 0x382B2000, 0x382B4000, 0x382B6000, 0x382B8000, 0x382BA000, 0x382BC000, 0x382BE000,
  0x382C0000, 0x382C2000, 0x382C4000, 0x382C6000, 0x382C8000, 0x382CA000, 0x382CC000, 0x382CE000, 0x382D0000, 0x382D2000, 0x382D4000, 0x382D6000, 0x382D8000, 0x382DA000, 0x382DC000, 0x382DE000,
  0x382E0000, 0x382E2000, 0x382E4000, 0x382E6000, 0x382E8000, 0x382EA000, 0x382EC000, 0x382EE000, 0x382F0000, 0x382F2000, 0x382F4000, 0x382F6000, 0x382F8000, 0x382FA000, 0x382FC000, 0x382FE000,
  0x38300000, 0x38302000, 0x38304000, 0x38306000, 0x38308000, 0x3830A000, 0x3830C000, 0x3830E000, 0x38310000, 0x38312000, 0x38314000, 0x38316000, 0x38318000, 0x3831A000, 0x3831C000, 0x3831E000,
  0x38320000, 0x38322000, 0x38324000, 0x38326000, 0x38328000, 0x3832A000, 0x3832C000, 0x3832E000, 0x38330000, 0x38332000, 0x38334000, 0x38336000, 0x38338000, 0x3833A000, 0x3833C000, 0x3833E000,
  0x38340000, 0x38342000, 0x38344000, 0x38346000, 0x38348000, 0x3834A000, 0x3834C000, 0x3834E000, 0x38350000, 0x38352000, 0x38354000, 0x38356000, 0x38358000, 0x3835A000, 0x3835C000, 0x3835E000,
  0x38360000, 0x38362000, 0x38364000, 0x38366000, 0x38368000, 0x3836A000, 0x3836C000, 0x3836E000, 0x38370000, 0x38372000, 0x38374000, 0x38376000, 0x38378000, 0x3837A000, 0x3837C000, 0x3837E000,
  0x38380000, 0x38382000, 0x38384000, 0x38386000, 0x38388000, 0x3838A000, 0x3838C000, 0x3838E000, 0x38390000, 0x38392000, 0x38394000, 0x38396000, 0x38398000, 0x3839A000, 0x3839C000, 0x3839E000,
  0x383A0000, 0x383A2000, 0x383A4000, 0x383A6000, 0x383A8000, 0x383AA000, 0x383AC000, 0x383AE000, 0x383B0000, 0x383B2000, 0x383B4000, 0x383B6000, 0x383B8000, 0x383BA000, 0x383BC000, 0x383BE000,
  0x383C0000, 0x383C2000, 0x383C4000, 0x383C6000, 0x383C8000, 0x383CA000, 0x383CC000, 0x383CE000, 0x383D0000, 0x383D2000, 0x383D4000, 0x383D6000, 0x383D8000, 0x383DA000, 0x383DC000, 0x383DE000,
  0x383E0000, 0x383E2000, 0x383E4000, 0x383E6000, 0x383E8000, 0x383EA000, 0x383EC000, 0x383EE000, 0x383F0000, 0x383F2000, 0x383F4000, 0x383F6000, 0x383F8000, 0x383FA000, 0x383FC000, 0x383FE000,
  0x38400000, 0x38402000, 0x38404000, 0x38406000, 0x38408000, 0x3840A000, 0x3840C000, 0x3840E000, 0x38410000, 0x38412000, 0x38414000, 0x38416000, 0x38418000, 0x3841A000, 0x3841C000, 0x3841E000,
  0x38420000, 0x38422000, 0x38424000, 0x38426000, 0x38428000, 0x3842A000, 0x3842C000, 0x3842E000, 0x38430000, 0x38432000, 0x38434000, 0x38436000, 0x38438000, 0x3843A000, 0x3843C000, 0x3843E000,
  0x38440000, 0x38442000, 0x38444000, 0x38446000, 0x38448000, 0x3844A000, 0x3844C000, 0x3844E000, 0x38450000, 0x38452000, 0x38454000, 0x38456000, 0x38458000, 0x3845A000, 0x3845C000, 0x3845E000,
  0x38460000, 0x38462000, 0x38464000, 0x38466000, 0x38468000, 0x3846A000, 0x3846C000, 0x3846E000, 0x38470000, 0x38472000, 0x38474000, 0x38476000, 0x38478000, 0x3847A000, 0x3847C000, 0x3847E000,
  0x38480000, 0x38482000, 0x38484000, 0x38486000, 0x38488000, 0x3848A000, 0x3848C000, 0x3848E000, 0x38490000, 0x38492000, 0x38494000, 0x38496000, 0x38498000, 0x3849A000, 0x3849C000, 0x3849E000,
  0x384A0000, 0x384A2000, 0x384A4000, 0x384A6000, 0x384A8000, 0x384AA000, 0x384AC000, 0x384AE000, 0x384B0000, 0x384B2000, 0x384B4000, 0x384B6000, 0x384B8000, 0x384BA000, 0x384BC000, 0x384BE000,
  0x384C0000, 0x384C2000, 0x384C4000, 0x384C6000, 0x384C8000, 0x384CA000, 0x384CC000, 0x384CE000, 0x384D0000, 0x384D2000, 0x384D4000, 0x384D6000, 0x384D8000, 0x384DA000, 0x384DC000, 0x384DE000,
  0x384E0000, 0x384E2000, 0x384E4000, 0x384E6000, 0x384E8000, 0x384EA000, 0x384EC000, 0x384EE000, 0x384F0000, 0x384F2000, 0x384F4000, 0x384F6000, 0x384F8000, 0x384FA000, 0x384FC000, 0x384FE000,
  0x38500000, 0x38502000, 0x38504000, 0x38506000, 0x38508000, 0x3850A000, 0x3850C000, 0x3850E000, 0x38510000, 0x38512000, 0x38514000, 0x38516000, 0x38518000, 0x3851A000, 0x3851C000, 0x3851E000,
  0x38520000, 0x38522000, 0x38524000, 0x38526000, 0x38528000, 0x3852A000, 0x3852C000, 0x3852E000, 0x38530000, 0x38532000, 0x38534000, 0x38536000, 0x38538000, 0x3853A000, 0x3853C000, 0x3853E000,
  0x38540000, 0x38542000, 0x38544000, 0x38546000, 0x38548000, 0x3854A000, 0x3854C000, 0x3854E000, 0x38550000, 0x38552000, 0x38554000, 0x38556000, 0x38558000, 0x3855A000, 0x3855C000, 0x3855E000,
  0x38560000, 0x38562000, 0x38564000, 0x38566000, 0x38568000, 0x3856A000, 0x3856C000, 0x3856E000, 0x38570000, 0x38572000, 0x38574000, 0x38576000, 0x38578000, 0x3857A000, 0x3857C000, 0x3857E000,
  0x38580000, 0x38582000, 0x38584000, 0x38586000, 0x38588000, 0x3858A000, 0x3858C000, 0x3858E000, 0x38590000, 0x38592000, 0x38594000, 0x38596000, 0x38598000, 0x3859A000, 0x3859C000, 0x3859E000,
  0x385A0000, 0x385A2000, 0x385A4000, 0x385A6000, 0x385A8000, 0x385AA000, 0x385AC000, 0x385AE000, 0x385B0000, 0x385B2000, 0x385B4000, 0x385B6000, 0x385B8000, 0x385BA000, 0x385BC000, 0x385BE000,
  0x385C0000, 0x385C2000, 0x385C4000, 0x385C6000, 0x385C8000, 0x385CA000, 0x385CC000, 0x385CE000, 0x385D0000, 0x385D2000, 0x385D4000, 0x385D6000, 0x385D8000, 0x385DA000, 0x385DC000, 0x385DE000,
  0x385E0000, 0x385E2000, 0x385E4000, 0x385E6000, 0x385E8000, 0x385EA000, 0x385EC000, 0x385EE000, 0x385F0000, 0x385F2000, 0x385F4000, 0x385F6000, 0x385F8000, 0x385FA000, 0x385FC000, 0x385FE000,
  0x38600000, 0x38602000, 0x38604000, 0x38606000, 0x38608000, 0x3860A000, 0x3860C000, 0x3860E000, 0x38610000, 0x38612000, 0x38614000, 0x38616000, 0x38618000, 0x3861A000, 0x3861C000, 0x3861E000,
  0x38620000, 0x38622000, 0x38624000, 0x38626000, 0x38628000, 0x3862A000, 0x3862C000, 0x3862E000, 0x38630000, 0x38632000, 0x38634000, 0x38636000, 0x38638000, 0x3863A000, 0x3863C000, 0x3863E000,
  0x38640000, 0x38642000, 0x38644000, 0x38646000, 0x38648000, 0x3864A000, 0x3864C000, 0x3864E000, 0x38650000, 0x38652000, 0x38654000, 0x38656000, 0x38658000, 0x3865A000, 0x3865C000, 0x3865E000,
  0x38660000, 0x38662000, 0x38664000, 0x38666000, 0x38668000, 0x3866A000, 0x3866C000, 0x3866E000, 0x38670000, 0x38672000, 0x38674000, 0x38676000, 0x38678000, 0x3867A000, 0x3867C000, 0x3867E000,
  0x38680000, 0x38682000, 0x38684000, 0x38686000, 0x38688000, 0x3868A000, 0x3868C000, 0x3868E000, 0x38690000, 0x38692000, 0x38694000, 0x38696000, 0x38698000, 0x3869A000, 0x3869C000, 0x3869E000,
  0x386A0000, 0x386A2000, 0x386A4000, 0x386A6000, 0x386A8000, 0x386AA000, 0x386AC000, 0x386AE000, 0x386B0000, 0x386B2000, 0x386B4000, 0x386B6000, 0x386B8000, 0x386BA000, 0x386BC000, 0x386BE000,
  0x386C0000, 0x386C2000, 0x386C4000, 0x386C6000, 0x386C8000, 0x386CA000, 0x386CC000, 0x386CE000, 0x386D0000, 0x386D2000, 0x386D4000, 0x386D6000, 0x386D8000, 0x386DA000, 0x386DC000, 0x386DE000,
  0x386E0000, 0x386E2000, 0x386E4000, 0x386E6000, 0x386E8000, 0x386EA000, 0x386EC000, 0x386EE000, 0x386F0000, 0x386F2000, 0x386F4000, 0x386F6000, 0x386F8000, 0x386FA000, 0x386FC000, 0x386FE000,
  0x38700000, 0x38702000, 0x38704000, 0x38706000, 0x38708000, 0x3870A000, 0x3870C000, 0x3870E000, 0x38710000, 0x38712000, 0x38714000, 0x38716000, 0x38718000, 0x3871A000, 0x3871C000, 0x3871E000,
  0x38720000, 0x38722000, 0x38724000, 0x38726000, 0x38728000, 0x3872A000, 0x3872C000, 0x3872E000, 0x38730000, 0x38732000, 0x38734000, 0x38736000, 0x38738000, 0x3873A000, 0x3873C000, 0x3873E000,
  0x38740000, 0x38742000, 0x38744000, 0x38746000, 0x38748000, 0x3874A000, 0x3874C000, 0x3874E000, 0x38750000, 0x38752000, 0x38754000, 0x38756000, 0x38758000, 0x3875A000, 0x3875C000, 0x3875E000,
  0x38760000, 0x38762000, 0x38764000, 0x38766000, 0x38768000, 0x3876A000, 0x3876C000, 0x3876E000, 0x38770000, 0x38772000, 0x38774000, 0x38776000, 0x38778000, 0x3877A000, 0x3877C000, 0x3877E000,
  0x38780000, 0x38782000, 0x38784000, 0x38786000, 0x38788000, 0x3878A000, 0x3878C000, 0x3878E000, 0x38790000, 0x38792000, 0x38794000, 0x38796000, 0x38798000, 0x3879A000, 0x3879C000, 0x3879E000,
  0x387A0000, 0x387A2000, 0x387A4000, 0x387A6000, 0x387A8000, 0x387AA000, 0x387AC000, 0x387AE000, 0x387B0000, 0x387B2000, 0x387B4000, 0x387B6000, 0x387B8000, 0x387BA000, 0x387BC000, 0x387BE000,
  0x387C0000, 0x387C2000, 0x387C4000, 0x387C6000, 0x387C8000, 0x387CA000, 0x387CC000, 0x387CE000, 0x387D0000, 0x387D2000, 0x387D4000, 0x387D6000, 0x387D8000, 0x387DA000, 0x387DC000, 0x387DE000,
  0x387E0000, 0x387E2000, 0x387E4000, 0x387E6000, 0x387E8000, 0x387EA000, 0x387EC000, 0x387EE000, 0x387F0000, 0x387F2000, 0x387F4000, 0x387F6000, 0x387F8000, 0x387FA000, 0x387FC000, 0x387FE000 };
__constant static const uint32_t exponent_table[64] = {
  0x00000000, 0x00800000, 0x01000000, 0x01800000, 0x02000000, 0x02800000, 0x03000000, 0x03800000, 0x04000000, 0x04800000, 0x05000000, 0x05800000, 0x06000000, 0x06800000, 0x07000000, 0x07800000,
  0x08000000, 0x08800000, 0x09000000, 0x09800000, 0x0A000000, 0x0A800000, 0x0B000000, 0x0B800000, 0x0C000000, 0x0C800000, 0x0D000000, 0x0D800000, 0x0E000000, 0x0E800000, 0x0F000000, 0x47800000,
  0x80000000, 0x80800000, 0x81000000, 0x81800000, 0x82000000, 0x82800000, 0x83000000, 0x83800000, 0x84000000, 0x84800000, 0x85000000, 0x85800000, 0x86000000, 0x86800000, 0x87000000, 0x87800000,
  0x88000000, 0x88800000, 0x89000000, 0x89800000, 0x8A000000, 0x8A800000, 0x8B000000, 0x8B800000, 0x8C000000, 0x8C800000, 0x8D000000, 0x8D800000, 0x8E000000, 0x8E800000, 0x8F000000, 0xC7800000 };
__constant static const unsigned short offset_table[64] = {
  0, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
  0, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024 };

SCALAR_FUN_ATTR uint16_t float2halfbits(float value) {
  union { float x; uint32_t y; } u;
  u.x = value;
  uint32_t bits = u.y;

  uint16_t hbits = base_table[bits>>23] + (uint16_t)((bits&0x7FFFFF)>>shift_table[bits>>23]);;

  return hbits;
}

SCALAR_FUN_ATTR float halfbits2float(uint16_t value) {
  uint32_t bits = mantissa_table[offset_table[value>>10]+(value&0x3FF)] + exponent_table[value>>10];

  union { uint32_t x; float y; } u;
  u.x = bits;
  return u.y;
}

SCALAR_FUN_ATTR uint16_t halfbitsnextafter(uint16_t from, uint16_t to) {
  int fabs = from & 0x7FFF, tabs = to & 0x7FFF;
  if(fabs > 0x7C00 || tabs > 0x7C00) {
    return ((from&0x7FFF)>0x7C00) ? (from|0x200) : (to|0x200);
  }
  if(from == to || !(fabs|tabs)) {
    return to;
  }
  if(!fabs) {
    return (to&0x8000)+1;
  }
  unsigned int out =
    from +
    (((from>>15)^(unsigned int)((from^(0x8000|(0x8000-(from>>15))))<(to^(0x8000|(0x8000-(to>>15))))))<<1)
    - 1;
  return out;
}

// End of half.h.
// Start of timing.h.

// The function get_wall_time() returns the wall time in microseconds
// (with an unspecified offset).

#ifdef _WIN32

#include <windows.h>

static int64_t get_wall_time(void) {
  LARGE_INTEGER time,freq;
  assert(QueryPerformanceFrequency(&freq));
  assert(QueryPerformanceCounter(&time));
  return ((double)time.QuadPart / freq.QuadPart) * 1000000;
}

static int64_t get_wall_time_ns(void) {
  return get_wall_time() * 1000;
}

#else
// Assuming POSIX

#include <time.h>
#include <sys/time.h>

static int64_t get_wall_time(void) {
  struct timeval time;
  assert(gettimeofday(&time,NULL) == 0);
  return time.tv_sec * 1000000 + time.tv_usec;
}

static int64_t get_wall_time_ns(void) {
  struct timespec time;
  assert(clock_gettime(CLOCK_REALTIME, &time) == 0);
  return time.tv_sec * 1000000000 + time.tv_nsec;
}

#endif

// End of timing.h.
// Start of lock.h.

// A very simple cross-platform implementation of locks.  Uses
// pthreads on Unix and some Windows thing there.  Futhark's
// host-level code is not multithreaded, but user code may be, so we
// need some mechanism for ensuring atomic access to API functions.
// This is that mechanism.  It is not exposed to user code at all, so
// we do not have to worry about name collisions.

#ifdef _WIN32

typedef HANDLE lock_t;

static void create_lock(lock_t *lock) {
  *lock = CreateMutex(NULL,  // Default security attributes.
                      FALSE, // Initially unlocked.
                      NULL); // Unnamed.
}

static void lock_lock(lock_t *lock) {
  assert(WaitForSingleObject(*lock, INFINITE) == WAIT_OBJECT_0);
}

static void lock_unlock(lock_t *lock) {
  assert(ReleaseMutex(*lock));
}

static void free_lock(lock_t *lock) {
  CloseHandle(*lock);
}

#else
// Assuming POSIX

#include <pthread.h>

typedef pthread_mutex_t lock_t;

static void create_lock(lock_t *lock) {
  int r = pthread_mutex_init(lock, NULL);
  assert(r == 0);
}

static void lock_lock(lock_t *lock) {
  int r = pthread_mutex_lock(lock);
  assert(r == 0);
}

static void lock_unlock(lock_t *lock) {
  int r = pthread_mutex_unlock(lock);
  assert(r == 0);
}

static void free_lock(lock_t *lock) {
  // Nothing to do for pthreads.
  (void)lock;
}

#endif

// End of lock.h.
// Start of free_list.h.

typedef uintptr_t fl_mem;

// An entry in the free list.  May be invalid, to avoid having to
// deallocate entries as soon as they are removed.  There is also a
// tag, to help with memory reuse.
struct free_list_entry {
  size_t size;
  fl_mem mem;
  const char *tag;
  unsigned char valid;
};

struct free_list {
  struct free_list_entry *entries; // Pointer to entries.
  int capacity;                    // Number of entries.
  int used;                        // Number of valid entries.
  lock_t lock;                     // Thread safety.
};

static void free_list_init(struct free_list *l) {
  l->capacity = 30; // Picked arbitrarily.
  l->used = 0;
  l->entries = (struct free_list_entry*) malloc(sizeof(struct free_list_entry) * l->capacity);
  for (int i = 0; i < l->capacity; i++) {
    l->entries[i].valid = 0;
  }
  create_lock(&l->lock);
}

// Remove invalid entries from the free list.
static void free_list_pack(struct free_list *l) {
  lock_lock(&l->lock);
  int p = 0;
  for (int i = 0; i < l->capacity; i++) {
    if (l->entries[i].valid) {
      l->entries[p] = l->entries[i];
      if (i > p) {
        l->entries[i].valid = 0;
      }
      p++;
    }
  }

  // Now p is the number of used elements.  We don't want it to go
  // less than the default capacity (although in practice it's OK as
  // long as it doesn't become 1).
  if (p < 30) {
    p = 30;
  }
  l->entries = realloc(l->entries, p * sizeof(struct free_list_entry));
  l->capacity = p;
  lock_unlock(&l->lock);
}

static void free_list_destroy(struct free_list *l) {
  assert(l->used == 0);
  free(l->entries);
  free_lock(&l->lock);
}

// Not part of the interface, so no locking.
static int free_list_find_invalid(struct free_list *l) {
  int i;
  for (i = 0; i < l->capacity; i++) {
    if (!l->entries[i].valid) {
      break;
    }
  }
  return i;
}

static void free_list_insert(struct free_list *l, size_t size, fl_mem mem, const char *tag) {
  lock_lock(&l->lock);
  int i = free_list_find_invalid(l);

  if (i == l->capacity) {
    // List is full; so we have to grow it.
    int new_capacity = l->capacity * 2 * sizeof(struct free_list_entry);
    l->entries = realloc(l->entries, new_capacity);
    for (int j = 0; j < l->capacity; j++) {
      l->entries[j+l->capacity].valid = 0;
    }
    l->capacity *= 2;
  }

  // Now 'i' points to the first invalid entry.
  l->entries[i].valid = 1;
  l->entries[i].size = size;
  l->entries[i].mem = mem;
  l->entries[i].tag = tag;

  l->used++;
  lock_unlock(&l->lock);
}

// Determine whether this entry in the free list is acceptable for
// satisfying the request.  Not public, so no locking.
static bool free_list_acceptable(size_t size, const char* tag, struct free_list_entry *entry) {
  // We check not just the hard requirement (is the entry acceptable
  // and big enough?) but also put a cap on how much wasted space
  // (internal fragmentation) we allow.  This is necessarily a
  // heuristic, and a crude one.

  if (!entry->valid) {
    return false;
  }

  if (size > entry->size) {
    return false;
  }

  // We know the block fits.  Now the question is whether it is too
  // big.  Our policy is as follows:
  //
  // 1) We don't care about wasted space below 4096 bytes (to avoid
  // churn in tiny allocations).
  //
  // 2) If the tag matches, we allow _any_ amount of wasted space.
  //
  // 3) Otherwise we allow up to 50% wasted space.

  if (entry->size < 4096) {
    return true;
  }

  if (entry->tag == tag) {
    return true;
  }

  if (entry->size < size * 2) {
    return true;
  }

  return false;
}

// Find and remove a memory block of the indicated tag, or if that
// does not exist, another memory block with exactly the desired size.
// Returns 0 on success.
static int free_list_find(struct free_list *l, size_t size, const char *tag,
                          size_t *size_out, fl_mem *mem_out) {
  lock_lock(&l->lock);
  int size_match = -1;
  int i;
  int ret = 1;
  for (i = 0; i < l->capacity; i++) {
    if (free_list_acceptable(size, tag, &l->entries[i]) &&
        (size_match < 0 || l->entries[i].size < l->entries[size_match].size)) {
      // If this entry is valid, has sufficient size, and is smaller than the
      // best entry found so far, use this entry.
      size_match = i;
    }
  }

  if (size_match >= 0) {
    l->entries[size_match].valid = 0;
    *size_out = l->entries[size_match].size;
    *mem_out = l->entries[size_match].mem;
    l->used--;
    ret = 0;
  }
  lock_unlock(&l->lock);
  return ret;
}

// Remove the first block in the free list.  Returns 0 if a block was
// removed, and nonzero if the free list was already empty.
static int free_list_first(struct free_list *l, fl_mem *mem_out) {
  lock_lock(&l->lock);
  int ret = 1;
  for (int i = 0; i < l->capacity; i++) {
    if (l->entries[i].valid) {
      l->entries[i].valid = 0;
      *mem_out = l->entries[i].mem;
      l->used--;
      ret = 0;
      break;
    }
  }
  lock_unlock(&l->lock);
  return ret;
}

// End of free_list.h.
// Start of event_list.h

typedef int (*event_report_fn)(struct str_builder*, void*);

struct event {
  void* data;
  event_report_fn f;
  const char* name;
  char *description;
};

struct event_list {
  struct event *events;
  int num_events;
  int capacity;
};

static void event_list_init(struct event_list *l) {
  l->capacity = 100;
  l->num_events = 0;
  l->events = calloc(l->capacity, sizeof(struct event));
}

static void event_list_free(struct event_list *l) {
  free(l->events);
}

static void add_event_to_list(struct event_list *l,
                              const char* name,
                              char* description,
                              void* data,
                              event_report_fn f) {
  if (l->num_events == l->capacity) {
    l->capacity *= 2;
    l->events = realloc(l->events, l->capacity * sizeof(struct event));
  }
  l->events[l->num_events].name = name;
  l->events[l->num_events].description = description;
  l->events[l->num_events].data = data;
  l->events[l->num_events].f = f;
  l->num_events++;
}

static int report_events_in_list(struct event_list *l,
                                 struct str_builder* sb) {
  int ret = 0;
  for (int i = 0; i < l->num_events; i++) {
    if (i != 0) {
      str_builder_str(sb, ",");
    }
    str_builder_str(sb, "{\"name\":");
    str_builder_json_str(sb, l->events[i].name);
    str_builder_str(sb, ",\"description\":");
    str_builder_json_str(sb, l->events[i].description);
    free(l->events[i].description);
    if (l->events[i].f(sb, l->events[i].data) != 0) {
      ret = 1;
      break;
    }
    str_builder(sb, "}");
  }
  event_list_free(l);
  event_list_init(l);
  return ret;
}

// End of event_list.h
#include <getopt.h>
#include <ctype.h>
#include <inttypes.h>
static const char *entry_point = "main";
// Start of values.h.

//// Text I/O

typedef int (*writer)(FILE*, const void*);
typedef int (*bin_reader)(void*);
typedef int (*str_reader)(const char *, void*);

struct array_reader {
  char* elems;
  int64_t n_elems_space;
  int64_t elem_size;
  int64_t n_elems_used;
  int64_t *shape;
  str_reader elem_reader;
};

static void skipspaces(FILE *f) {
  int c;
  do {
    c = getc(f);
  } while (isspace(c));

  if (c != EOF) {
    ungetc(c, f);
  }
}

static int constituent(char c) {
  return isalnum(c) || c == '.' || c == '-' || c == '+' || c == '_';
}

// Produces an empty token only on EOF.
static void next_token(FILE *f, char *buf, int bufsize) {
 start:
  skipspaces(f);

  int i = 0;
  while (i < bufsize) {
    int c = getc(f);
    buf[i] = (char)c;

    if (c == EOF) {
      buf[i] = 0;
      return;
    } else if (c == '-' && i == 1 && buf[0] == '-') {
      // Line comment, so skip to end of line and start over.
      for (; c != '\n' && c != EOF; c = getc(f));
      goto start;
    } else if (!constituent((char)c)) {
      if (i == 0) {
        // We permit single-character tokens that are not
        // constituents; this lets things like ']' and ',' be
        // tokens.
        buf[i+1] = 0;
        return;
      } else {
        ungetc(c, f);
        buf[i] = 0;
        return;
      }
    }

    i++;
  }

  buf[bufsize-1] = 0;
}

static int next_token_is(FILE *f, char *buf, int bufsize, const char* expected) {
  next_token(f, buf, bufsize);
  return strcmp(buf, expected) == 0;
}

static void remove_underscores(char *buf) {
  char *w = buf;

  for (char *r = buf; *r; r++) {
    if (*r != '_') {
      *w++ = *r;
    }
  }

  *w++ = 0;
}

static int read_str_elem(char *buf, struct array_reader *reader) {
  int ret;
  if (reader->n_elems_used == reader->n_elems_space) {
    reader->n_elems_space *= 2;
    reader->elems = (char*) realloc(reader->elems,
                                    (size_t)(reader->n_elems_space * reader->elem_size));
  }

  ret = reader->elem_reader(buf, reader->elems + reader->n_elems_used * reader->elem_size);

  if (ret == 0) {
    reader->n_elems_used++;
  }

  return ret;
}

static int read_str_array_elems(FILE *f,
                                char *buf, int bufsize,
                                struct array_reader *reader, int64_t dims) {
  int ret = 1;
  int expect_elem = 1;
  char *knows_dimsize = (char*) calloc((size_t)dims, sizeof(char));
  int cur_dim = (int)dims-1;
  int64_t *elems_read_in_dim = (int64_t*) calloc((size_t)dims, sizeof(int64_t));

  while (1) {
    next_token(f, buf, bufsize);
    if (strcmp(buf, "]") == 0) {
      expect_elem = 0;
      if (knows_dimsize[cur_dim]) {
        if (reader->shape[cur_dim] != elems_read_in_dim[cur_dim]) {
          ret = 1;
          break;
        }
      } else {
        knows_dimsize[cur_dim] = 1;
        reader->shape[cur_dim] = elems_read_in_dim[cur_dim];
      }
      if (cur_dim == 0) {
        ret = 0;
        break;
      } else {
        cur_dim--;
        elems_read_in_dim[cur_dim]++;
      }
    } else if (!expect_elem && strcmp(buf, ",") == 0) {
      expect_elem = 1;
    } else if (expect_elem) {
      if (strcmp(buf, "[") == 0) {
        if (cur_dim == dims - 1) {
          ret = 1;
          break;
        }
        cur_dim++;
        elems_read_in_dim[cur_dim] = 0;
      } else if (cur_dim == dims - 1) {
        ret = read_str_elem(buf, reader);
        if (ret != 0) {
          break;
        }
        expect_elem = 0;
        elems_read_in_dim[cur_dim]++;
      } else {
        ret = 1;
        break;
      }
    } else {
      ret = 1;
      break;
    }
  }

  free(knows_dimsize);
  free(elems_read_in_dim);
  return ret;
}

static int read_str_empty_array(FILE *f, char *buf, int bufsize,
                                const char *type_name, int64_t *shape, int64_t dims) {
  if (strlen(buf) == 0) {
    // EOF
    return 1;
  }

  if (strcmp(buf, "empty") != 0) {
    return 1;
  }

  if (!next_token_is(f, buf, bufsize, "(")) {
    return 1;
  }

  for (int i = 0; i < dims; i++) {
    if (!next_token_is(f, buf, bufsize, "[")) {
      return 1;
    }

    next_token(f, buf, bufsize);

    if (sscanf(buf, "%"SCNu64, (uint64_t*)&shape[i]) != 1) {
      return 1;
    }

    if (!next_token_is(f, buf, bufsize, "]")) {
      return 1;
    }
  }

  if (!next_token_is(f, buf, bufsize, type_name)) {
    return 1;
  }


  if (!next_token_is(f, buf, bufsize, ")")) {
    return 1;
  }

  // Check whether the array really is empty.
  for (int i = 0; i < dims; i++) {
    if (shape[i] == 0) {
      return 0;
    }
  }

  // Not an empty array!
  return 1;
}

static int read_str_array(FILE *f,
                          int64_t elem_size, str_reader elem_reader,
                          const char *type_name,
                          void **data, int64_t *shape, int64_t dims) {
  int ret;
  struct array_reader reader;
  char buf[100];

  int dims_seen;
  for (dims_seen = 0; dims_seen < dims; dims_seen++) {
    if (!next_token_is(f, buf, sizeof(buf), "[")) {
      break;
    }
  }

  if (dims_seen == 0) {
    return read_str_empty_array(f, buf, sizeof(buf), type_name, shape, dims);
  }

  if (dims_seen != dims) {
    return 1;
  }

  reader.shape = shape;
  reader.n_elems_used = 0;
  reader.elem_size = elem_size;
  reader.n_elems_space = 16;
  reader.elems = (char*) realloc(*data, (size_t)(elem_size*reader.n_elems_space));
  reader.elem_reader = elem_reader;

  ret = read_str_array_elems(f, buf, sizeof(buf), &reader, dims);

  *data = reader.elems;

  return ret;
}

#define READ_STR(MACRO, PTR, SUFFIX)                                   \
  remove_underscores(buf);                                              \
  int j;                                                                \
  if (sscanf(buf, "%"MACRO"%n", (PTR*)dest, &j) == 1) {                 \
    return !(strcmp(buf+j, "") == 0 || strcmp(buf+j, SUFFIX) == 0);     \
  } else {                                                              \
    return 1;                                                           \
  }

static int read_str_i8(char *buf, void* dest) {
  // Some platforms (WINDOWS) does not support scanf %hhd or its
  // cousin, %SCNi8.  Read into int first to avoid corrupting
  // memory.
  //
  // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=63417
  remove_underscores(buf);
  int j, x;
  if (sscanf(buf, "%i%n", &x, &j) == 1) {
    *(int8_t*)dest = (int8_t)x;
    return !(strcmp(buf+j, "") == 0 || strcmp(buf+j, "i8") == 0);
  } else {
    return 1;
  }
}

static int read_str_u8(char *buf, void* dest) {
  // Some platforms (WINDOWS) does not support scanf %hhd or its
  // cousin, %SCNu8.  Read into int first to avoid corrupting
  // memory.
  //
  // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=63417
  remove_underscores(buf);
  int j, x;
  if (sscanf(buf, "%i%n", &x, &j) == 1) {
    *(uint8_t*)dest = (uint8_t)x;
    return !(strcmp(buf+j, "") == 0 || strcmp(buf+j, "u8") == 0);
  } else {
    return 1;
  }
}

static int read_str_i16(char *buf, void* dest) {
  READ_STR(SCNi16, int16_t, "i16");
}

static int read_str_u16(char *buf, void* dest) {
  READ_STR(SCNi16, int16_t, "u16");
}

static int read_str_i32(char *buf, void* dest) {
  READ_STR(SCNi32, int32_t, "i32");
}

static int read_str_u32(char *buf, void* dest) {
  READ_STR(SCNi32, int32_t, "u32");
}

static int read_str_i64(char *buf, void* dest) {
  READ_STR(SCNi64, int64_t, "i64");
}

static int read_str_u64(char *buf, void* dest) {
  // FIXME: This is not correct, as SCNu64 only permits decimal
  // literals.  However, SCNi64 does not handle very large numbers
  // correctly (it's really for signed numbers, so that's fair).
  READ_STR(SCNu64, uint64_t, "u64");
}

static int read_str_f16(char *buf, void* dest) {
  remove_underscores(buf);
  if (strcmp(buf, "f16.nan") == 0) {
    *(uint16_t*)dest = float2halfbits(NAN);
    return 0;
  } else if (strcmp(buf, "f16.inf") == 0) {
    *(uint16_t*)dest = float2halfbits(INFINITY);
    return 0;
  } else if (strcmp(buf, "-f16.inf") == 0) {
    *(uint16_t*)dest = float2halfbits(-INFINITY);
    return 0;
  } else {
    int j;
    float x;
    if (sscanf(buf, "%f%n", &x, &j) == 1) {
      if (strcmp(buf+j, "") == 0 || strcmp(buf+j, "f16") == 0) {
        *(uint16_t*)dest = float2halfbits(x);
        return 0;
      }
    }
    return 1;
  }
}

static int read_str_f32(char *buf, void* dest) {
  remove_underscores(buf);
  if (strcmp(buf, "f32.nan") == 0) {
    *(float*)dest = (float)NAN;
    return 0;
  } else if (strcmp(buf, "f32.inf") == 0) {
    *(float*)dest = (float)INFINITY;
    return 0;
  } else if (strcmp(buf, "-f32.inf") == 0) {
    *(float*)dest = (float)-INFINITY;
    return 0;
  } else {
    READ_STR("f", float, "f32");
  }
}

static int read_str_f64(char *buf, void* dest) {
  remove_underscores(buf);
  if (strcmp(buf, "f64.nan") == 0) {
    *(double*)dest = (double)NAN;
    return 0;
  } else if (strcmp(buf, "f64.inf") == 0) {
    *(double*)dest = (double)INFINITY;
    return 0;
  } else if (strcmp(buf, "-f64.inf") == 0) {
    *(double*)dest = (double)-INFINITY;
    return 0;
  } else {
    READ_STR("lf", double, "f64");
  }
}

static int read_str_bool(char *buf, void* dest) {
  if (strcmp(buf, "true") == 0) {
    *(char*)dest = 1;
    return 0;
  } else if (strcmp(buf, "false") == 0) {
    *(char*)dest = 0;
    return 0;
  } else {
    return 1;
  }
}

static int write_str_i8(FILE *out, int8_t *src) {
  return fprintf(out, "%hhdi8", *src);
}

static int write_str_u8(FILE *out, uint8_t *src) {
  return fprintf(out, "%hhuu8", *src);
}

static int write_str_i16(FILE *out, int16_t *src) {
  return fprintf(out, "%hdi16", *src);
}

static int write_str_u16(FILE *out, uint16_t *src) {
  return fprintf(out, "%huu16", *src);
}

static int write_str_i32(FILE *out, int32_t *src) {
  return fprintf(out, "%di32", *src);
}

static int write_str_u32(FILE *out, uint32_t *src) {
  return fprintf(out, "%uu32", *src);
}

static int write_str_i64(FILE *out, int64_t *src) {
  return fprintf(out, "%"PRIi64"i64", *src);
}

static int write_str_u64(FILE *out, uint64_t *src) {
  return fprintf(out, "%"PRIu64"u64", *src);
}

static int write_str_f16(FILE *out, uint16_t *src) {
  float x = halfbits2float(*src);
  if (isnan(x)) {
    return fprintf(out, "f16.nan");
  } else if (isinf(x) && x >= 0) {
    return fprintf(out, "f16.inf");
  } else if (isinf(x)) {
    return fprintf(out, "-f16.inf");
  } else {
    return fprintf(out, "%.*ff16", FLT_DIG, x);
  }
}

static int write_str_f32(FILE *out, float *src) {
  float x = *src;
  if (isnan(x)) {
    return fprintf(out, "f32.nan");
  } else if (isinf(x) && x >= 0) {
    return fprintf(out, "f32.inf");
  } else if (isinf(x)) {
    return fprintf(out, "-f32.inf");
  } else {
    return fprintf(out, "%.*ff32", FLT_DIG, x);
  }
}

static int write_str_f64(FILE *out, double *src) {
  double x = *src;
  if (isnan(x)) {
    return fprintf(out, "f64.nan");
  } else if (isinf(x) && x >= 0) {
    return fprintf(out, "f64.inf");
  } else if (isinf(x)) {
    return fprintf(out, "-f64.inf");
  } else {
    return fprintf(out, "%.*ff64", DBL_DIG, x);
  }
}

static int write_str_bool(FILE *out, void *src) {
  return fprintf(out, *(char*)src ? "true" : "false");
}

//// Binary I/O

#define BINARY_FORMAT_VERSION 2
#define IS_BIG_ENDIAN (!*(unsigned char *)&(uint16_t){1})

static void flip_bytes(size_t elem_size, unsigned char *elem) {
  for (size_t j=0; j<elem_size/2; j++) {
    unsigned char head = elem[j];
    size_t tail_index = elem_size-1-j;
    elem[j] = elem[tail_index];
    elem[tail_index] = head;
  }
}

// On Windows we need to explicitly set the file mode to not mangle
// newline characters.  On *nix there is no difference.
#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
static void set_binary_mode(FILE *f) {
  setmode(fileno(f), O_BINARY);
}
#else
static void set_binary_mode(FILE *f) {
  (void)f;
}
#endif

static int read_byte(FILE *f, void* dest) {
  size_t num_elems_read = fread(dest, 1, 1, f);
  return num_elems_read == 1 ? 0 : 1;
}

//// Types

struct primtype_info_t {
  const char binname[4]; // Used for parsing binary data.
  const char* type_name; // Same name as in Futhark.
  const int64_t size; // in bytes
  const writer write_str; // Write in text format.
  const str_reader read_str; // Read in text format.
};

static const struct primtype_info_t i8_info =
  {.binname = "  i8", .type_name = "i8",   .size = 1,
   .write_str = (writer)write_str_i8, .read_str = (str_reader)read_str_i8};
static const struct primtype_info_t i16_info =
  {.binname = " i16", .type_name = "i16",  .size = 2,
   .write_str = (writer)write_str_i16, .read_str = (str_reader)read_str_i16};
static const struct primtype_info_t i32_info =
  {.binname = " i32", .type_name = "i32",  .size = 4,
   .write_str = (writer)write_str_i32, .read_str = (str_reader)read_str_i32};
static const struct primtype_info_t i64_info =
  {.binname = " i64", .type_name = "i64",  .size = 8,
   .write_str = (writer)write_str_i64, .read_str = (str_reader)read_str_i64};
static const struct primtype_info_t u8_info =
  {.binname = "  u8", .type_name = "u8",   .size = 1,
   .write_str = (writer)write_str_u8, .read_str = (str_reader)read_str_u8};
static const struct primtype_info_t u16_info =
  {.binname = " u16", .type_name = "u16",  .size = 2,
   .write_str = (writer)write_str_u16, .read_str = (str_reader)read_str_u16};
static const struct primtype_info_t u32_info =
  {.binname = " u32", .type_name = "u32",  .size = 4,
   .write_str = (writer)write_str_u32, .read_str = (str_reader)read_str_u32};
static const struct primtype_info_t u64_info =
  {.binname = " u64", .type_name = "u64",  .size = 8,
   .write_str = (writer)write_str_u64, .read_str = (str_reader)read_str_u64};
static const struct primtype_info_t f16_info =
  {.binname = " f16", .type_name = "f16",  .size = 2,
   .write_str = (writer)write_str_f16, .read_str = (str_reader)read_str_f16};
static const struct primtype_info_t f32_info =
  {.binname = " f32", .type_name = "f32",  .size = 4,
   .write_str = (writer)write_str_f32, .read_str = (str_reader)read_str_f32};
static const struct primtype_info_t f64_info =
  {.binname = " f64", .type_name = "f64",  .size = 8,
   .write_str = (writer)write_str_f64, .read_str = (str_reader)read_str_f64};
static const struct primtype_info_t bool_info =
  {.binname = "bool", .type_name = "bool", .size = 1,
   .write_str = (writer)write_str_bool, .read_str = (str_reader)read_str_bool};

static const struct primtype_info_t* primtypes[] = {
  &i8_info, &i16_info, &i32_info, &i64_info,
  &u8_info, &u16_info, &u32_info, &u64_info,
  &f16_info, &f32_info, &f64_info,
  &bool_info,
  NULL // NULL-terminated
};

// General value interface.  All endian business taken care of at
// lower layers.

static int read_is_binary(FILE *f) {
  skipspaces(f);
  int c = getc(f);
  if (c == 'b') {
    int8_t bin_version;
    int ret = read_byte(f, &bin_version);

    if (ret != 0) { futhark_panic(1, "binary-input: could not read version.\n"); }

    if (bin_version != BINARY_FORMAT_VERSION) {
      futhark_panic(1, "binary-input: File uses version %i, but I only understand version %i.\n",
            bin_version, BINARY_FORMAT_VERSION);
    }

    return 1;
  }
  ungetc(c, f);
  return 0;
}

static const struct primtype_info_t* read_bin_read_type_enum(FILE *f) {
  char read_binname[4];

  int num_matched = fscanf(f, "%4c", read_binname);
  if (num_matched != 1) { futhark_panic(1, "binary-input: Couldn't read element type.\n"); }

  const struct primtype_info_t **type = primtypes;

  for (; *type != NULL; type++) {
    // I compare the 4 characters manually instead of using strncmp because
    // this allows any value to be used, also NULL bytes
    if (memcmp(read_binname, (*type)->binname, 4) == 0) {
      return *type;
    }
  }
  futhark_panic(1, "binary-input: Did not recognize the type '%s'.\n", read_binname);
  return NULL;
}

static void read_bin_ensure_scalar(FILE *f, const struct primtype_info_t *expected_type) {
  int8_t bin_dims;
  int ret = read_byte(f, &bin_dims);
  if (ret != 0) { futhark_panic(1, "binary-input: Couldn't get dims.\n"); }

  if (bin_dims != 0) {
    futhark_panic(1, "binary-input: Expected scalar (0 dimensions), but got array with %i dimensions.\n",
          bin_dims);
  }

  const struct primtype_info_t *bin_type = read_bin_read_type_enum(f);
  if (bin_type != expected_type) {
    futhark_panic(1, "binary-input: Expected scalar of type %s but got scalar of type %s.\n",
          expected_type->type_name,
          bin_type->type_name);
  }
}

//// High-level interface

static int read_bin_array(FILE *f,
                          const struct primtype_info_t *expected_type, void **data, int64_t *shape, int64_t dims) {
  int ret;

  int8_t bin_dims;
  ret = read_byte(f, &bin_dims);
  if (ret != 0) { futhark_panic(1, "binary-input: Couldn't get dims.\n"); }

  if (bin_dims != dims) {
    futhark_panic(1, "binary-input: Expected %i dimensions, but got array with %i dimensions.\n",
          dims, bin_dims);
  }

  const struct primtype_info_t *bin_primtype = read_bin_read_type_enum(f);
  if (expected_type != bin_primtype) {
    futhark_panic(1, "binary-input: Expected %iD-array with element type '%s' but got %iD-array with element type '%s'.\n",
          dims, expected_type->type_name, dims, bin_primtype->type_name);
  }

  int64_t elem_count = 1;
  for (int i=0; i<dims; i++) {
    int64_t bin_shape;
    ret = (int)fread(&bin_shape, sizeof(bin_shape), 1, f);
    if (ret != 1) {
      futhark_panic(1, "binary-input: Couldn't read size for dimension %i of array.\n", i);
    }
    if (IS_BIG_ENDIAN) {
      flip_bytes(sizeof(bin_shape), (unsigned char*) &bin_shape);
    }
    elem_count *= bin_shape;
    shape[i] = bin_shape;
  }

  int64_t elem_size = expected_type->size;
  void* tmp = realloc(*data, (size_t)(elem_count * elem_size));
  if (tmp == NULL) {
    futhark_panic(1, "binary-input: Failed to allocate array of size %i.\n",
          elem_count * elem_size);
  }
  *data = tmp;

  int64_t num_elems_read = (int64_t)fread(*data, (size_t)elem_size, (size_t)elem_count, f);
  if (num_elems_read != elem_count) {
    futhark_panic(1, "binary-input: tried to read %i elements of an array, but only got %i elements.\n",
          elem_count, num_elems_read);
  }

  // If we're on big endian platform we must change all multibyte elements
  // from using little endian to big endian
  if (IS_BIG_ENDIAN && elem_size != 1) {
    flip_bytes((size_t)elem_size, (unsigned char*) *data);
  }

  return 0;
}

static int read_array(FILE *f, const struct primtype_info_t *expected_type, void **data, int64_t *shape, int64_t dims) {
  if (!read_is_binary(f)) {
    return read_str_array(f, expected_type->size, (str_reader)expected_type->read_str, expected_type->type_name, data, shape, dims);
  } else {
    return read_bin_array(f, expected_type, data, shape, dims);
  }
}

static int end_of_input(FILE *f) {
  skipspaces(f);
  char token[2];
  next_token(f, token, sizeof(token));
  if (strcmp(token, "") == 0) {
    return 0;
  } else {
    return 1;
  }
}

static int write_str_array(FILE *out,
                           const struct primtype_info_t *elem_type,
                           const unsigned char *data,
                           const int64_t *shape,
                           int8_t rank) {
  if (rank==0) {
    elem_type->write_str(out, (const void*)data);
  } else {
    int64_t len = (int64_t)shape[0];
    int64_t slice_size = 1;

    int64_t elem_size = elem_type->size;
    for (int8_t i = 1; i < rank; i++) {
      slice_size *= shape[i];
    }

    if (len*slice_size == 0) {
      fprintf(out, "empty(");
      for (int64_t i = 0; i < rank; i++) {
        fprintf(out, "[%"PRIi64"]", shape[i]);
      }
      fprintf(out, "%s", elem_type->type_name);
      fprintf(out, ")");
    } else if (rank==1) {
      fputc('[', out);
      for (int64_t i = 0; i < len; i++) {
        elem_type->write_str(out, (const void*) (data + i * elem_size));
        if (i != len-1) {
          fprintf(out, ", ");
        }
      }
      fputc(']', out);
    } else {
      fputc('[', out);
      for (int64_t i = 0; i < len; i++) {
        write_str_array(out, elem_type, data + i * slice_size * elem_size, shape+1, rank-1);
        if (i != len-1) {
          fprintf(out, ", ");
        }
      }
      fputc(']', out);
    }
  }
  return 0;
}

static int write_bin_array(FILE *out,
                           const struct primtype_info_t *elem_type,
                           const unsigned char *data,
                           const int64_t *shape,
                           int8_t rank) {
  int64_t num_elems = 1;
  for (int64_t i = 0; i < rank; i++) {
    num_elems *= shape[i];
  }

  fputc('b', out);
  fputc((char)BINARY_FORMAT_VERSION, out);
  fwrite(&rank, sizeof(int8_t), 1, out);
  fwrite(elem_type->binname, 4, 1, out);
  if (shape != NULL) {
    fwrite(shape, sizeof(int64_t), (size_t)rank, out);
  }

  if (IS_BIG_ENDIAN) {
    for (int64_t i = 0; i < num_elems; i++) {
      const unsigned char *elem = data+i*elem_type->size;
      for (int64_t j = 0; j < elem_type->size; j++) {
        fwrite(&elem[elem_type->size-j], 1, 1, out);
      }
    }
  } else {
    fwrite(data, (size_t)elem_type->size, (size_t)num_elems, out);
  }

  return 0;
}

static int write_array(FILE *out, int write_binary,
                       const struct primtype_info_t *elem_type,
                       const void *data,
                       const int64_t *shape,
                       const int8_t rank) {
  if (write_binary) {
    return write_bin_array(out, elem_type, data, shape, rank);
  } else {
    return write_str_array(out, elem_type, data, shape, rank);
  }
}

static int read_scalar(FILE *f,
                       const struct primtype_info_t *expected_type, void *dest) {
  if (!read_is_binary(f)) {
    char buf[100];
    next_token(f, buf, sizeof(buf));
    return expected_type->read_str(buf, dest);
  } else {
    read_bin_ensure_scalar(f, expected_type);
    size_t elem_size = (size_t)expected_type->size;
    size_t num_elems_read = fread(dest, elem_size, 1, f);
    if (IS_BIG_ENDIAN) {
      flip_bytes(elem_size, (unsigned char*) dest);
    }
    return num_elems_read == 1 ? 0 : 1;
  }
}

static int write_scalar(FILE *out, int write_binary, const struct primtype_info_t *type, void *src) {
  if (write_binary) {
    return write_bin_array(out, type, src, NULL, 0);
  } else {
    return type->write_str(out, src);
  }
}

// End of values.h.

// Start of server.h.

// Forward declarations of things that we technically don't know until
// the application header file is included, but which we need.
struct futhark_context_config;
struct futhark_context;
char *futhark_context_get_error(struct futhark_context *ctx);
int futhark_context_sync(struct futhark_context *ctx);
int futhark_context_clear_caches(struct futhark_context *ctx);
int futhark_context_config_set_tuning_param(struct futhark_context_config *cfg,
                                            const char *param_name,
                                            size_t new_value);
int futhark_get_tuning_param_count(void);
const char* futhark_get_tuning_param_name(int i);
const char* futhark_get_tuning_param_class(int i);

typedef int (*restore_fn)(const void*, FILE *, struct futhark_context*, void*);
typedef void (*store_fn)(const void*, FILE *, struct futhark_context*, void*);
typedef int (*free_fn)(const void*, struct futhark_context*, void*);
typedef int (*project_fn)(struct futhark_context*, void*, const void*);
typedef int (*new_fn)(struct futhark_context*, void**, const void*[]);

struct field {
  const char *name;
  const struct type *type;
  project_fn project;
};

struct record {
  int num_fields;
  const struct field* fields;
  new_fn new;
};

struct type {
  const char *name;
  restore_fn restore;
  store_fn store;
  free_fn free;
  const void *aux;
  const struct record *record;
};

int free_scalar(const void *aux, struct futhark_context *ctx, void *p) {
  (void)aux;
  (void)ctx;
  (void)p;
  // Nothing to do.
  return 0;
}

#define DEF_SCALAR_TYPE(T)                                      \
  int restore_##T(const void *aux, FILE *f,                     \
                  struct futhark_context *ctx, void *p) {       \
    (void)aux;                                                  \
    (void)ctx;                                                  \
    return read_scalar(f, &T##_info, p);                        \
  }                                                             \
                                                                \
  void store_##T(const void *aux, FILE *f,                      \
                 struct futhark_context *ctx, void *p) {        \
    (void)aux;                                                  \
    (void)ctx;                                                  \
    write_scalar(f, 1, &T##_info, p);                           \
  }                                                             \
                                                                \
  struct type type_##T =                                        \
    { .name = #T,                                               \
      .restore = restore_##T,                                   \
      .store = store_##T,                                       \
      .free = free_scalar                                       \
    }                                                           \

DEF_SCALAR_TYPE(i8);
DEF_SCALAR_TYPE(i16);
DEF_SCALAR_TYPE(i32);
DEF_SCALAR_TYPE(i64);
DEF_SCALAR_TYPE(u8);
DEF_SCALAR_TYPE(u16);
DEF_SCALAR_TYPE(u32);
DEF_SCALAR_TYPE(u64);
DEF_SCALAR_TYPE(f16);
DEF_SCALAR_TYPE(f32);
DEF_SCALAR_TYPE(f64);
DEF_SCALAR_TYPE(bool);

struct value {
  const struct type *type;
  union {
    void *v_ptr;
    int8_t  v_i8;
    int16_t v_i16;
    int32_t v_i32;
    int64_t v_i64;

    uint8_t  v_u8;
    uint16_t v_u16;
    uint32_t v_u32;
    uint64_t v_u64;

    uint16_t v_f16;
    float v_f32;
    double v_f64;

    bool v_bool;
  } value;
};

void* value_ptr(struct value *v) {
  if (v->type == &type_i8) {
    return &v->value.v_i8;
  }
  if (v->type == &type_i16) {
    return &v->value.v_i16;
  }
  if (v->type == &type_i32) {
    return &v->value.v_i32;
  }
  if (v->type == &type_i64) {
    return &v->value.v_i64;
  }
  if (v->type == &type_u8) {
    return &v->value.v_u8;
  }
  if (v->type == &type_u16) {
    return &v->value.v_u16;
  }
  if (v->type == &type_u32) {
    return &v->value.v_u32;
  }
  if (v->type == &type_u64) {
    return &v->value.v_u64;
  }
  if (v->type == &type_f16) {
    return &v->value.v_f16;
  }
  if (v->type == &type_f32) {
    return &v->value.v_f32;
  }
  if (v->type == &type_f64) {
    return &v->value.v_f64;
  }
  if (v->type == &type_bool) {
    return &v->value.v_bool;
  }
  return &v->value.v_ptr;
}

struct variable {
  // NULL name indicates free slot.  Name is owned by this struct.
  char *name;
  struct value value;
};

typedef int (*entry_point_fn)(struct futhark_context*, void**, void**);

struct entry_point {
  const char *name;
  entry_point_fn f;
  const char** tuning_params;
  const struct type **out_types;
  bool *out_unique;
  const struct type **in_types;
  bool *in_unique;
};

int entry_num_ins(struct entry_point *e) {
  int count = 0;
  while (e->in_types[count]) {
    count++;
  }
  return count;
}

int entry_num_outs(struct entry_point *e) {
  int count = 0;
  while (e->out_types[count]) {
    count++;
  }
  return count;
}

struct futhark_prog {
  // Last entry point identified by NULL name.
  struct entry_point *entry_points;
  // Last type identified by NULL name.
  const struct type **types;
};

struct server_state {
  struct futhark_prog prog;
  struct futhark_context_config *cfg;
  struct futhark_context *ctx;
  int variables_capacity;
  struct variable *variables;
};

struct variable* get_variable(struct server_state *s,
                              const char *name) {
  for (int i = 0; i < s->variables_capacity; i++) {
    if (s->variables[i].name != NULL &&
        strcmp(s->variables[i].name, name) == 0) {
      return &s->variables[i];
    }
  }

  return NULL;
}

struct variable* create_variable(struct server_state *s,
                                 const char *name,
                                 const struct type *type) {
  int found = -1;
  for (int i = 0; i < s->variables_capacity; i++) {
    if (found == -1 && s->variables[i].name == NULL) {
      found = i;
    } else if (s->variables[i].name != NULL &&
               strcmp(s->variables[i].name, name) == 0) {
      return NULL;
    }
  }

  if (found != -1) {
    // Found a free spot.
    s->variables[found].name = strdup(name);
    s->variables[found].value.type = type;
    return &s->variables[found];
  }

  // Need to grow the buffer.
  found = s->variables_capacity;
  s->variables_capacity *= 2;
  s->variables = realloc(s->variables,
                         s->variables_capacity * sizeof(struct variable));

  s->variables[found].name = strdup(name);
  s->variables[found].value.type = type;

  for (int i = found+1; i < s->variables_capacity; i++) {
    s->variables[i].name = NULL;
  }

  return &s->variables[found];
}

void drop_variable(struct variable *v) {
  free(v->name);
  v->name = NULL;
}

int arg_exists(const char *args[], int i) {
  return args[i] != NULL;
}

const char* get_arg(const char *args[], int i) {
  if (!arg_exists(args, i)) {
    futhark_panic(1, "Insufficient command args.\n");
  }
  return args[i];
}

const struct type* get_type(struct server_state *s, const char *name) {
  for (int i = 0; s->prog.types[i]; i++) {
    if (strcmp(s->prog.types[i]->name, name) == 0) {
      return s->prog.types[i];
    }
  }

  futhark_panic(1, "Unknown type %s\n", name);
  return NULL;
}

struct entry_point* get_entry_point(struct server_state *s, const char *name) {
  for (int i = 0; s->prog.entry_points[i].name; i++) {
    if (strcmp(s->prog.entry_points[i].name, name) == 0) {
      return &s->prog.entry_points[i];
    }
  }

  return NULL;
}

// Print the command-done marker, indicating that we are ready for
// more input.
void ok(void) {
  printf("%%%%%% OK\n");
  fflush(stdout);
}

// Print the failure marker.  Output is now an error message until the
// next ok().
void failure(void) {
  printf("%%%%%% FAILURE\n");
}

void error_check(struct server_state *s, int err) {
  if (err != 0) {
    failure();
    char *error = futhark_context_get_error(s->ctx);
    if (error != NULL) {
      puts(error);
    }
    free(error);
  }
}

void cmd_call(struct server_state *s, const char *args[]) {
  const char *name = get_arg(args, 0);

  struct entry_point *e = get_entry_point(s, name);

  if (e == NULL) {
    failure();
    printf("Unknown entry point: %s\n", name);
    return;
  }

  int num_outs = entry_num_outs(e);
  int num_ins = entry_num_ins(e);
  // +1 to avoid zero-size arrays, which is UB.
  void* outs[num_outs+1];
  void* ins[num_ins+1];

  for (int i = 0; i < num_ins; i++) {
    const char *in_name = get_arg(args, 1+num_outs+i);
    struct variable *v = get_variable(s, in_name);
    if (v == NULL) {
      failure();
      printf("Unknown variable: %s\n", in_name);
      return;
    }
    if (v->value.type != e->in_types[i]) {
      failure();
      printf("Wrong input type.  Expected %s, got %s.\n",
             e->in_types[i]->name, v->value.type->name);
      return;
    }
    ins[i] = value_ptr(&v->value);
  }

  for (int i = 0; i < num_outs; i++) {
    const char *out_name = get_arg(args, 1+i);
    struct variable *v = create_variable(s, out_name, e->out_types[i]);
    if (v == NULL) {
      failure();
      printf("Variable already exists: %s\n", out_name);
      return;
    }
    outs[i] = value_ptr(&v->value);
  }

  int64_t t_start = get_wall_time();
  int err = e->f(s->ctx, outs, ins);
  err |= futhark_context_sync(s->ctx);
  int64_t t_end = get_wall_time();
  long long int elapsed_usec = t_end - t_start;
  printf("runtime: %lld\n", elapsed_usec);

  error_check(s, err);
  if (err != 0) {
    // Need to uncreate the output variables, which would otherwise be left
    // in an uninitialised state.
    for (int i = 0; i < num_outs; i++) {
      const char *out_name = get_arg(args, 1+i);
      struct variable *v = get_variable(s, out_name);
      if (v) {
        drop_variable(v);
      }
    }
  }
}

void cmd_restore(struct server_state *s, const char *args[]) {
  const char *fname = get_arg(args, 0);

  FILE *f = fopen(fname, "rb");
  if (f == NULL) {
    failure();
    printf("Failed to open %s: %s\n", fname, strerror(errno));
    return;
  }

  int bad = 0;
  int values = 0;
  for (int i = 1; arg_exists(args, i); i+=2, values++) {
    const char *vname = get_arg(args, i);
    const char *type = get_arg(args, i+1);

    const struct type *t = get_type(s, type);
    struct variable *v = create_variable(s, vname, t);

    if (v == NULL) {
      bad = 1;
      failure();
      printf("Variable already exists: %s\n", vname);
      break;
    }

    errno = 0;
    if (t->restore(t->aux, f, s->ctx, value_ptr(&v->value)) != 0) {
      bad = 1;
      failure();
      printf("Failed to restore variable %s.\n"
             "Possibly malformed data in %s (errno: %s)\n",
             vname, fname, strerror(errno));
      drop_variable(v);
      break;
    }
  }

  if (!bad && end_of_input(f) != 0) {
    failure();
    printf("Expected EOF after reading %d values from %s\n",
           values, fname);
  }

  fclose(f);

  if (!bad) {
    int err = futhark_context_sync(s->ctx);
    error_check(s, err);
  }
}

void cmd_store(struct server_state *s, const char *args[]) {
  const char *fname = get_arg(args, 0);

  FILE *f = fopen(fname, "wb");
  if (f == NULL) {
    failure();
    printf("Failed to open %s: %s\n", fname, strerror(errno));
  } else {
    for (int i = 1; arg_exists(args, i); i++) {
      const char *vname = get_arg(args, i);
      struct variable *v = get_variable(s, vname);

      if (v == NULL) {
        failure();
        printf("Unknown variable: %s\n", vname);
        return;
      }

      const struct type *t = v->value.type;
      t->store(t->aux, f, s->ctx, value_ptr(&v->value));
    }
    fclose(f);
  }
}

void cmd_free(struct server_state *s, const char *args[]) {
  for (int i = 0; arg_exists(args, i); i++) {
    const char *name = get_arg(args, i);
    struct variable *v = get_variable(s, name);

    if (v == NULL) {
      failure();
      printf("Unknown variable: %s\n", name);
      return;
    }

    const struct type *t = v->value.type;

    int err = t->free(t->aux, s->ctx, value_ptr(&v->value));
    error_check(s, err);
    drop_variable(v);
  }
}

void cmd_rename(struct server_state *s, const char *args[]) {
  const char *oldname = get_arg(args, 0);
  const char *newname = get_arg(args, 1);
  struct variable *old = get_variable(s, oldname);
  struct variable *new = get_variable(s, newname);

  if (old == NULL) {
    failure();
    printf("Unknown variable: %s\n", oldname);
    return;
  }

  if (new != NULL) {
    failure();
    printf("Variable already exists: %s\n", newname);
    return;
  }

  free(old->name);
  old->name = strdup(newname);
}

void cmd_inputs(struct server_state *s, const char *args[]) {
  const char *name = get_arg(args, 0);
  struct entry_point *e = get_entry_point(s, name);

  if (e == NULL) {
    failure();
    printf("Unknown entry point: %s\n", name);
    return;
  }

  int num_ins = entry_num_ins(e);
  for (int i = 0; i < num_ins; i++) {
    if (e->in_unique[i]) {
      putchar('*');
    }
    puts(e->in_types[i]->name);
  }
}

void cmd_outputs(struct server_state *s, const char *args[]) {
  const char *name = get_arg(args, 0);
  struct entry_point *e = get_entry_point(s, name);

  if (e == NULL) {
    failure();
    printf("Unknown entry point: %s\n", name);
    return;
  }

  int num_outs = entry_num_outs(e);
  for (int i = 0; i < num_outs; i++) {
    if (e->out_unique[i]) {
      putchar('*');
    }
    puts(e->out_types[i]->name);
  }
}

void cmd_clear(struct server_state *s, const char *args[]) {
  (void)args;
  int err = 0;
  for (int i = 0; i < s->variables_capacity; i++) {
    struct variable *v = &s->variables[i];
    if (v->name != NULL) {
      err |= v->value.type->free(v->value.type->aux, s->ctx, value_ptr(&v->value));
      drop_variable(v);
    }
  }
  err |= futhark_context_clear_caches(s->ctx);
  error_check(s, err);
}

void cmd_pause_profiling(struct server_state *s, const char *args[]) {
  (void)args;
  futhark_context_pause_profiling(s->ctx);
}

void cmd_unpause_profiling(struct server_state *s, const char *args[]) {
  (void)args;
  futhark_context_unpause_profiling(s->ctx);
}

void cmd_report(struct server_state *s, const char *args[]) {
  (void)args;
  char *report = futhark_context_report(s->ctx);
  if (report) {
    puts(report);
  } else {
    failure();
    report = futhark_context_get_error(s->ctx);
    if (report) {
      puts(report);
    } else {
      puts("Failed to produce profiling report.\n");
    }
  }
  free(report);
}

void cmd_set_tuning_param(struct server_state *s, const char *args[]) {
  const char *param = get_arg(args, 0);
  const char *val_s = get_arg(args, 1);
  size_t val = atol(val_s);
  int err = futhark_context_config_set_tuning_param(s->cfg, param, val);

  error_check(s, err);

  if (err != 0) {
    printf("Failed to set tuning parameter %s to %ld\n", param, (long)val);
  }
}

void cmd_tuning_params(struct server_state *s, const char *args[]) {
  const char *name = get_arg(args, 0);
  struct entry_point *e = get_entry_point(s, name);

  if (e == NULL) {
    failure();
    printf("Unknown entry point: %s\n", name);
    return;
  }

  const char **params = e->tuning_params;
  for (int i = 0; params[i] != NULL; i++) {
    printf("%s\n", params[i]);
  }
}

void cmd_tuning_param_class(struct server_state *s, const char *args[]) {
  (void)s;
  const char *param = get_arg(args, 0);

  int n = futhark_get_tuning_param_count();

  for (int i = 0; i < n; i++) {
    if (strcmp(futhark_get_tuning_param_name(i), param) == 0) {
      printf("%s\n", futhark_get_tuning_param_class(i));
      return;
    }
  }

  failure();
  printf("Unknown tuning parameter: %s\n", param);
}

void cmd_fields(struct server_state *s, const char *args[]) {
  const char *type = get_arg(args, 0);
  const struct type *t = get_type(s, type);
  const struct record *r = t->record;

  if (r == NULL) {
    failure();
    printf("Not a record type\n");
    return;
  }

  for (int i = 0; i < r->num_fields; i++) {
    const struct field f = r->fields[i];
    printf("%s %s\n", f.name, f.type->name);
  }
}

void cmd_project(struct server_state *s, const char *args[]) {
  const char *to_name = get_arg(args, 0);
  const char *from_name = get_arg(args, 1);
  const char *field_name = get_arg(args, 2);

  struct variable *from = get_variable(s, from_name);

  if (from == NULL) {
    failure();
    printf("Unknown variable: %s\n", from_name);
    return;
  }

  const struct type *from_type = from->value.type;
  const struct record *r = from_type->record;

  if (r == NULL) {
    failure();
    printf("Not a record type\n");
    return;
  }

  const struct field *field = NULL;
  for (int i = 0; i < r->num_fields; i++) {
    if (strcmp(r->fields[i].name, field_name) == 0) {
      field = &r->fields[i];
      break;
    }
  }

  if (field == NULL) {
    failure();
    printf("No such field\n");
  }

  struct variable *to = create_variable(s, to_name, field->type);

  if (to == NULL) {
    failure();
    printf("Variable already exists: %s\n", to_name);
    return;
  }

  field->project(s->ctx, value_ptr(&to->value), from->value.value.v_ptr);
}

void cmd_new(struct server_state *s, const char *args[]) {
  const char *to_name = get_arg(args, 0);
  const char *type_name = get_arg(args, 1);
  const struct type *type = get_type(s, type_name);
  struct variable *to = create_variable(s, to_name, type);

  if (to == NULL) {
    failure();
    printf("Variable already exists: %s\n", to_name);
    return;
  }

  const struct record* r = type->record;

  if (r == NULL) {
    failure();
    printf("Not a record type\n");
    return;
  }

  int num_args = 0;
  for (int i = 2; arg_exists(args, i); i++) {
    num_args++;
  }

  if (num_args != r->num_fields) {
    failure();
    printf("%d fields expected but %d values provided.\n", num_args, r->num_fields);
    return;
  }

  const void** value_ptrs = alloca(num_args * sizeof(void*));

  for (int i = 0; i < num_args; i++) {
    struct variable* v = get_variable(s, args[2+i]);

    if (v == NULL) {
      failure();
      printf("Unknown variable: %s\n", args[2+i]);
      return;
    }

    if (strcmp(v->value.type->name, r->fields[i].type->name) != 0) {
      failure();
      printf("Field %s mismatch: expected type %s, got %s\n",
             r->fields[i].name, r->fields[i].type->name, v->value.type->name);
      return;
    }

    value_ptrs[i] = value_ptr(&v->value);
  }

  r->new(s->ctx, value_ptr(&to->value), value_ptrs);
}

void cmd_entry_points(struct server_state *s, const char *args[]) {
  (void)args;
  for (int i = 0; s->prog.entry_points[i].name; i++) {
    puts(s->prog.entry_points[i].name);
  }
}

void cmd_types(struct server_state *s, const char *args[]) {
  (void)args;
  for (int i = 0; s->prog.types[i] != NULL; i++) {
    puts(s->prog.types[i]->name);
  }
}

char *next_word(char **line) {
  char *p = *line;

  while (isspace(*p)) {
    p++;
  }

  if (*p == 0) {
    return NULL;
  }

  if (*p == '"') {
    char *save = p+1;
    // Skip ahead till closing quote.
    p++;

    while (*p && *p != '"') {
      p++;
    }

    if (*p == '"') {
      *p = 0;
      *line = p+1;
      return save;
    } else {
      return NULL;
    }
  } else {
    char *save = p;
    // Skip ahead till next whitespace.

    while (*p && !isspace(*p)) {
      p++;
    }

    if (*p) {
      *p = 0;
      *line = p+1;
    } else {
      *line = p;
    }
    return save;
  }
}

void process_line(struct server_state *s, char *line) {
  int max_num_tokens = 1000;
  const char* tokens[max_num_tokens];
  int num_tokens = 0;

  while ((tokens[num_tokens] = next_word(&line)) != NULL) {
    num_tokens++;
    if (num_tokens == max_num_tokens) {
      futhark_panic(1, "Line too long.\n");
    }
  }

  const char *command = tokens[0];

  if (command == NULL) {
    failure();
    printf("Empty line\n");
  } else if (strcmp(command, "call") == 0) {
    cmd_call(s, tokens+1);
  } else if (strcmp(command, "restore") == 0) {
    cmd_restore(s, tokens+1);
  } else if (strcmp(command, "store") == 0) {
    cmd_store(s, tokens+1);
  } else if (strcmp(command, "free") == 0) {
    cmd_free(s, tokens+1);
  } else if (strcmp(command, "rename") == 0) {
    cmd_rename(s, tokens+1);
  } else if (strcmp(command, "inputs") == 0) {
    cmd_inputs(s, tokens+1);
  } else if (strcmp(command, "outputs") == 0) {
    cmd_outputs(s, tokens+1);
  } else if (strcmp(command, "clear") == 0) {
    cmd_clear(s, tokens+1);
  } else if (strcmp(command, "pause_profiling") == 0) {
    cmd_pause_profiling(s, tokens+1);
  } else if (strcmp(command, "unpause_profiling") == 0) {
    cmd_unpause_profiling(s, tokens+1);
  } else if (strcmp(command, "report") == 0) {
    cmd_report(s, tokens+1);
  } else if (strcmp(command, "set_tuning_param") == 0) {
    cmd_set_tuning_param(s, tokens+1);
  } else if (strcmp(command, "tuning_params") == 0) {
    cmd_tuning_params(s, tokens+1);
  } else if (strcmp(command, "tuning_param_class") == 0) {
    cmd_tuning_param_class(s, tokens+1);
  } else if (strcmp(command, "fields") == 0) {
    cmd_fields(s, tokens+1);
  } else if (strcmp(command, "new") == 0) {
    cmd_new(s, tokens+1);
  } else if (strcmp(command, "project") == 0) {
    cmd_project(s, tokens+1);
  } else if (strcmp(command, "entry_points") == 0) {
    cmd_entry_points(s, tokens+1);
  } else if (strcmp(command, "types") == 0) {
    cmd_types(s, tokens+1);
  } else {
    futhark_panic(1, "Unknown command: %s\n", command);
  }
}

void run_server(struct futhark_prog *prog,
                struct futhark_context_config *cfg,
                struct futhark_context *ctx) {
  char *line = NULL;
  size_t buflen = 0;
  ssize_t linelen;

  struct server_state s = {
    .cfg = cfg,
    .ctx = ctx,
    .variables_capacity = 100,
    .prog = *prog
  };

  s.variables = malloc(s.variables_capacity * sizeof(struct variable));

  for (int i = 0; i < s.variables_capacity; i++) {
    s.variables[i].name = NULL;
  }

  ok();
  while ((linelen = getline(&line, &buflen, stdin)) > 0) {
    process_line(&s, line);
    ok();
  }

  free(s.variables);
  free(line);
}

// The aux struct lets us write generic method implementations without
// code duplication.

typedef void* (*array_new_fn)(struct futhark_context *, const void*, const int64_t*);
typedef const int64_t* (*array_shape_fn)(struct futhark_context*, void*);
typedef int (*array_values_fn)(struct futhark_context*, void*, void*);
typedef int (*array_free_fn)(struct futhark_context*, void*);

struct array_aux {
  int rank;
  const struct primtype_info_t* info;
  const char *name;
  array_new_fn new;
  array_shape_fn shape;
  array_values_fn values;
  array_free_fn free;
};

int restore_array(const struct array_aux *aux, FILE *f,
                  struct futhark_context *ctx, void *p) {
  void *data = NULL;
  int64_t shape[aux->rank];
  if (read_array(f, aux->info, &data, shape, aux->rank) != 0) {
    return 1;
  }

  void *arr = aux->new(ctx, data, shape);
  if (arr == NULL) {
    return 1;
  }
  int err = futhark_context_sync(ctx);
  *(void**)p = arr;
  free(data);
  return err;
}

void store_array(const struct array_aux *aux, FILE *f,
                 struct futhark_context *ctx, void *p) {
  void *arr = *(void**)p;
  const int64_t *shape = aux->shape(ctx, arr);
  int64_t size = sizeof(aux->info->size);
  for (int i = 0; i < aux->rank; i++) {
    size *= shape[i];
  }
  int32_t *data = malloc(size);
  assert(aux->values(ctx, arr, data) == 0);
  assert(futhark_context_sync(ctx) == 0);
  assert(write_array(f, 1, aux->info, data, shape, aux->rank) == 0);
  free(data);
}

int free_array(const struct array_aux *aux,
               struct futhark_context *ctx, void *p) {
  void *arr = *(void**)p;
  return aux->free(ctx, arr);
}

typedef void* (*opaque_restore_fn)(struct futhark_context*, void*);
typedef int (*opaque_store_fn)(struct futhark_context*, const void*, void **, size_t *);
typedef int (*opaque_free_fn)(struct futhark_context*, void*);

struct opaque_aux {
  opaque_restore_fn restore;
  opaque_store_fn store;
  opaque_free_fn free;
};

int restore_opaque(const struct opaque_aux *aux, FILE *f,
                   struct futhark_context *ctx, void *p) {
  // We have a problem: we need to load data from 'f', since the
  // restore function takes a pointer, but we don't know how much we
  // need (and cannot possibly).  So we do something hacky: we read
  // *all* of the file, pass all of the data to the restore function
  // (which doesn't care if there's extra at the end), then we compute
  // how much space the the object actually takes in serialised form
  // and rewind the file to that position.  The only downside is more IO.
  size_t start = ftell(f);
  size_t size;
  char *bytes = fslurp_file(f, &size);
  void *obj = aux->restore(ctx, bytes);
  free(bytes);
  if (obj != NULL) {
    *(void**)p = obj;
    size_t obj_size;
    (void)aux->store(ctx, obj, NULL, &obj_size);
    fseek(f, start+obj_size, SEEK_SET);
    return 0;
  } else {
    fseek(f, start, SEEK_SET);
    return 1;
  }
}

void store_opaque(const struct opaque_aux *aux, FILE *f,
                  struct futhark_context *ctx, void *p) {
  void *obj = *(void**)p;
  size_t obj_size;
  void *data = NULL;
  (void)aux->store(ctx, obj, &data, &obj_size);
  assert(futhark_context_sync(ctx) == 0);
  fwrite(data, sizeof(char), obj_size, f);
  free(data);
}

int free_opaque(const struct opaque_aux *aux,
                struct futhark_context *ctx, void *p) {
  void *obj = *(void**)p;
  return aux->free(ctx, obj);
}

// End of server.h.

// Start of tuning.h.


int is_blank_line_or_comment(const char *s) {
  size_t i = strspn(s, " \t\n");
  return s[i] == '\0' || // Line is blank.
         strncmp(s + i, "--", 2) == 0; // Line is comment.
}

static char* load_tuning_file(const char *fname,
                              void *cfg,
                              int (*set_tuning_param)(void*, const char*, size_t)) {
  const int max_line_len = 1024;
  char* line = (char*) malloc(max_line_len);

  FILE *f = fopen(fname, "r");

  if (f == NULL) {
    snprintf(line, max_line_len, "Cannot open file: %s", strerror(errno));
    return line;
  }

  int lineno = 0;
  while (fgets(line, max_line_len, f) != NULL) {
    lineno++;
    if (is_blank_line_or_comment(line)) {
      continue;
    }
    char *eql = strstr(line, "=");
    if (eql) {
      *eql = 0;
      char *endptr;
      int value = strtol(eql+1, &endptr, 10);
      if (*endptr && *endptr != '\n') {
        snprintf(line, max_line_len, "Invalid line %d (must be of form 'name=int').",
                 lineno);
        return line;
      }
      if (set_tuning_param(cfg, line, (size_t)value) != 0) {
        char* err = (char*) malloc(max_line_len + 50);
        snprintf(err, max_line_len + 50, "Unknown name '%s' on line %d.", line, lineno);
        free(line);
        return err;
      }
    } else {
      snprintf(line, max_line_len, "Invalid line %d (must be of form 'name=int').",
               lineno);
      return line;
    }
  }

  free(line);

  return NULL;
}

// End of tuning.h.

const struct type type_ZMZNZMZNu8;
const struct type type_ZMZNi32;
const struct type type_ZMZNi64;
void *futhark_new_u8_2d_wrap(struct futhark_context *ctx, const void *p, const int64_t *shape)
{
    return futhark_new_u8_2d(ctx, p, shape[0], shape[1]);
}
const struct array_aux type_ZMZNZMZNu8_aux = {.name ="[][]u8", .rank =2, .info =&u8_info, .new =(array_new_fn) futhark_new_u8_2d_wrap, .free =(array_free_fn) futhark_free_u8_2d, .shape =(array_shape_fn) futhark_shape_u8_2d, .values =(array_values_fn) futhark_values_u8_2d};
const struct type type_ZMZNZMZNu8 = {.name ="[][]u8", .restore =(restore_fn) restore_array, .store =(store_fn) store_array, .free =(free_fn) free_array, .aux =&type_ZMZNZMZNu8_aux};
void *futhark_new_i32_1d_wrap(struct futhark_context *ctx, const void *p, const int64_t *shape)
{
    return futhark_new_i32_1d(ctx, p, shape[0]);
}
const struct array_aux type_ZMZNi32_aux = {.name ="[]i32", .rank =1, .info =&i32_info, .new =(array_new_fn) futhark_new_i32_1d_wrap, .free =(array_free_fn) futhark_free_i32_1d, .shape =(array_shape_fn) futhark_shape_i32_1d, .values =(array_values_fn) futhark_values_i32_1d};
const struct type type_ZMZNi32 = {.name ="[]i32", .restore =(restore_fn) restore_array, .store =(store_fn) store_array, .free =(free_fn) free_array, .aux =&type_ZMZNi32_aux};
void *futhark_new_i64_1d_wrap(struct futhark_context *ctx, const void *p, const int64_t *shape)
{
    return futhark_new_i64_1d(ctx, p, shape[0]);
}
const struct array_aux type_ZMZNi64_aux = {.name ="[]i64", .rank =1, .info =&i64_info, .new =(array_new_fn) futhark_new_i64_1d_wrap, .free =(array_free_fn) futhark_free_i64_1d, .shape =(array_shape_fn) futhark_shape_i64_1d, .values =(array_values_fn) futhark_values_i64_1d};
const struct type type_ZMZNi64 = {.name ="[]i64", .restore =(restore_fn) restore_array, .store =(store_fn) store_array, .free =(free_fn) free_array, .aux =&type_ZMZNi64_aux};
const struct type *rhj_out_types[] = {&type_ZMZNZMZNu8, &type_ZMZNi64, &type_ZMZNi64, NULL};
bool rhj_out_unique[] = {false, false, false};
const struct type *rhj_in_types[] = {&type_ZMZNZMZNu8, &type_ZMZNZMZNu8, &type_ZMZNZMZNu8, &type_ZMZNZMZNu8, &type_i32, &type_ZMZNi64, &type_ZMZNi32, &type_ZMZNi64, &type_ZMZNi64, NULL};
bool rhj_in_unique[] = {false, false, false, false, false, false, false, false, false};
const char *rhj_tuning_params[] = {NULL};
int call_rhj(struct futhark_context *ctx, void **outs, void **ins)
{
    struct futhark_u8_2d * *out0 = outs[0];
    struct futhark_i64_1d * *out1 = outs[1];
    struct futhark_i64_1d * *out2 = outs[2];
    struct futhark_u8_2d * in0 = *(struct futhark_u8_2d * *) ins[0];
    struct futhark_u8_2d * in1 = *(struct futhark_u8_2d * *) ins[1];
    struct futhark_u8_2d * in2 = *(struct futhark_u8_2d * *) ins[2];
    struct futhark_u8_2d * in3 = *(struct futhark_u8_2d * *) ins[3];
    int32_t in4 = *(int32_t *) ins[4];
    struct futhark_i64_1d * in5 = *(struct futhark_i64_1d * *) ins[5];
    struct futhark_i32_1d * in6 = *(struct futhark_i32_1d * *) ins[6];
    struct futhark_i64_1d * in7 = *(struct futhark_i64_1d * *) ins[7];
    struct futhark_i64_1d * in8 = *(struct futhark_i64_1d * *) ins[8];
    
    return futhark_entry_rhj(ctx, out0, out1, out2, in0, in1, in2, in3, in4, in5, in6, in7, in8);
}
const struct type *types[] = {&type_i8, &type_i16, &type_i32, &type_i64, &type_u8, &type_u16, &type_u32, &type_u64, &type_f16, &type_f32, &type_f64, &type_bool, &type_ZMZNZMZNu8, &type_ZMZNi32, &type_ZMZNi64, NULL};
struct entry_point entry_points[] = {{.name ="rhj", .f =call_rhj, .tuning_params =rhj_tuning_params, .in_types =rhj_in_types, .out_types =rhj_out_types, .in_unique =rhj_in_unique, .out_unique =rhj_out_unique}, {.name =NULL}};
struct futhark_prog prog = {.types =types, .entry_points =entry_points};
int parse_options(struct futhark_context_config *cfg, int argc, char *const argv[])
{
    int ch;
    static struct option long_options[] = {{"debugging", no_argument, NULL, 1}, {"log", no_argument, NULL, 2}, {"profile", no_argument, NULL, 3}, {"help", no_argument, NULL, 4}, {"print-params", no_argument, NULL, 5}, {"param", required_argument, NULL, 6}, {"tuning", required_argument, NULL, 7}, {"cache-file", required_argument, NULL, 8}, {0, 0, 0, 0}};
    static char *option_descriptions = "  -D/--debugging     Perform possibly expensive internal correctness checks and verbose logging.\n  -L/--log           Print various low-overhead logging information while running.\n  -P/--profile       Enable the collection of profiling information.\n  -h/--help          Print help information and exit.\n  --print-params     Print all tuning parameters that can be set with --param or --tuning.\n  --param ASSIGNMENT Set a tuning parameter to the given value.\n  --tuning FILE      Read size=value assignments from the given file.\n  --cache-file FILE  Store program cache here.\n";
    
    while ((ch = getopt_long(argc, argv, ":DLPh", long_options, NULL)) != -1) {
        if (ch == 1 || ch == 'D')
            futhark_context_config_set_debugging(cfg, 1);
        if (ch == 2 || ch == 'L')
            futhark_context_config_set_logging(cfg, 1);
        if (ch == 3 || ch == 'P')
            futhark_context_config_set_profiling(cfg, 1);
        if (ch == 4 || ch == 'h') {
            printf("Usage: %s [OPTIONS]...\nOptions:\n\n%s\nFor more information, consult the Futhark User's Guide or the man pages.\n", fut_progname, option_descriptions);
            exit(0);
        }
        if (ch == 5) {
            int n = futhark_get_tuning_param_count();
            
            for (int i = 0; i < n; i++)
                printf("%s (%s)\n", futhark_get_tuning_param_name(i), futhark_get_tuning_param_class(i));
            exit(0);
        }
        if (ch == 6) {
            char *name = optarg;
            char *equals = strstr(optarg, "=");
            char *value_str = equals != NULL ? equals + 1 : optarg;
            int value = atoi(value_str);
            
            if (equals != NULL) {
                *equals = 0;
                if (futhark_context_config_set_tuning_param(cfg, name, value) != 0)
                    futhark_panic(1, "Unknown size: %s\n", name);
            } else
                futhark_panic(1, "Invalid argument for size option: %s\n", optarg);
        }
        if (ch == 7) {
            char *ret = load_tuning_file(optarg, cfg, (int (*)(void *, const char *, size_t)) futhark_context_config_set_tuning_param);
            
            if (ret != NULL)
                futhark_panic(1, "When loading tuning file '%s': %s\n", optarg, ret);
        }
        if (ch == 8)
            futhark_context_config_set_cache_file(cfg, optarg);
        if (ch == ':')
            futhark_panic(-1, "Missing argument for option %s\n", argv[optind - 1]);
        if (ch == '?') {
            fprintf(stderr, "Usage: %s [OPTIONS]...\nOptions:\n\n%s\n", fut_progname, "  -D/--debugging     Perform possibly expensive internal correctness checks and verbose logging.\n  -L/--log           Print various low-overhead logging information while running.\n  -P/--profile       Enable the collection of profiling information.\n  -h/--help          Print help information and exit.\n  --print-params     Print all tuning parameters that can be set with --param or --tuning.\n  --param ASSIGNMENT Set a tuning parameter to the given value.\n  --tuning FILE      Read size=value assignments from the given file.\n  --cache-file FILE  Store program cache here.\n");
            futhark_panic(1, "Unknown option: %s\n", argv[optind - 1]);
        }
    }
    return optind;
}
int main(int argc, char **argv)
{
    fut_progname = argv[0];
    
    struct futhark_context_config *cfg = futhark_context_config_new();
    
    assert(cfg != NULL);
    
    int parsed_options = parse_options(cfg, argc, argv);
    
    argc -= parsed_options;
    argv += parsed_options;
    if (argc != 0)
        futhark_panic(1, "Excess non-option: %s\n", argv[0]);
    
    struct futhark_context *ctx = futhark_context_new(cfg);
    
    assert(ctx != NULL);
    futhark_context_set_logging_file(ctx, stdout);
    
    char *error = futhark_context_get_error(ctx);
    
    if (error != NULL)
        futhark_panic(1, "Error during context initialisation:\n%s", error);
    if (entry_point != NULL)
        run_server(&prog, cfg, ctx);
    futhark_context_free(ctx);
    futhark_context_config_free(cfg);
}

#ifdef _MSC_VER
#define inline __inline
#endif
#include <string.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <ctype.h>



#define FUTHARK_F64_ENABLED

// Start of scalar.h.

// Implementation of the primitive scalar operations.  Very
// repetitive.  This code is inserted directly into both CUDA and
// OpenCL programs, as well as the CPU code, so it has some #ifdefs to
// work everywhere.  Some operations are defined as macros because
// this allows us to use them as constant expressions in things like
// array sizes and static initialisers.

// Some of the #ifdefs are because OpenCL uses type-generic functions
// for some operations (e.g. sqrt), while C and CUDA sensibly use
// distinct functions for different precisions (e.g. sqrtf() and
// sqrt()).  This is quite annoying.  Due to C's unfortunate casting
// rules, it is also really easy to accidentally implement
// floating-point functions in the wrong precision, so be careful.

// Double-precision definitions are only included if the preprocessor
// macro FUTHARK_F64_ENABLED is set.

SCALAR_FUN_ATTR int32_t futrts_to_bits32(float x);
SCALAR_FUN_ATTR float futrts_from_bits32(int32_t x);

SCALAR_FUN_ATTR uint8_t add8(uint8_t x, uint8_t y) {
  return x + y;
}

SCALAR_FUN_ATTR uint16_t add16(uint16_t x, uint16_t y) {
  return x + y;
}

SCALAR_FUN_ATTR uint32_t add32(uint32_t x, uint32_t y) {
  return x + y;
}

SCALAR_FUN_ATTR uint64_t add64(uint64_t x, uint64_t y) {
  return x + y;
}

SCALAR_FUN_ATTR uint8_t sub8(uint8_t x, uint8_t y) {
  return x - y;
}

SCALAR_FUN_ATTR uint16_t sub16(uint16_t x, uint16_t y) {
  return x - y;
}

SCALAR_FUN_ATTR uint32_t sub32(uint32_t x, uint32_t y) {
  return x - y;
}

SCALAR_FUN_ATTR uint64_t sub64(uint64_t x, uint64_t y) {
  return x - y;
}

SCALAR_FUN_ATTR uint8_t mul8(uint8_t x, uint8_t y) {
  return x * y;
}

SCALAR_FUN_ATTR uint16_t mul16(uint16_t x, uint16_t y) {
  return x * y;
}

SCALAR_FUN_ATTR uint32_t mul32(uint32_t x, uint32_t y) {
  return x * y;
}

SCALAR_FUN_ATTR uint64_t mul64(uint64_t x, uint64_t y) {
  return x * y;
}

#if ISPC

SCALAR_FUN_ATTR uint8_t udiv8(uint8_t x, uint8_t y) {
  // This strange pattern is used to prevent the ISPC compiler from
  // causing SIGFPEs and bogus results on divisions where inactive lanes
  // have 0-valued divisors. It ensures that any inactive lane instead
  // has a divisor of 1. https://github.com/ispc/ispc/issues/2292
  uint8_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return x / ys;
}

SCALAR_FUN_ATTR uint16_t udiv16(uint16_t x, uint16_t y) {
  uint16_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return x / ys;
}

SCALAR_FUN_ATTR uint32_t udiv32(uint32_t x, uint32_t y) {
  uint32_t ys = 1;
  foreach_active(i){
    ys = y;
  }


  return x / ys;
}

SCALAR_FUN_ATTR uint64_t udiv64(uint64_t x, uint64_t y) {
  uint64_t ys = 1;
  foreach_active(i){
    ys = y;
  }


  return x / ys;
}

SCALAR_FUN_ATTR uint8_t udiv_up8(uint8_t x, uint8_t y) {
  uint8_t ys = 1;
  foreach_active(i){
    ys = y;
  }


  return (x + y - 1) / ys;
}

SCALAR_FUN_ATTR uint16_t udiv_up16(uint16_t x, uint16_t y) {
  uint16_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return (x + y - 1) / ys;
}

SCALAR_FUN_ATTR uint32_t udiv_up32(uint32_t x, uint32_t y) {
  uint32_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return (x + y - 1) / ys;
}

SCALAR_FUN_ATTR uint64_t udiv_up64(uint64_t x, uint64_t y) {
  uint64_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return (x + y - 1) / ys;
}

SCALAR_FUN_ATTR uint8_t umod8(uint8_t x, uint8_t y) {
  uint8_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return x % ys;
}

SCALAR_FUN_ATTR uint16_t umod16(uint16_t x, uint16_t y) {
  uint16_t ys = 1;
  foreach_active(i){
    ys = y;
  }


  return x % ys;
}

SCALAR_FUN_ATTR uint32_t umod32(uint32_t x, uint32_t y) {
  uint32_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return x % ys;
}

SCALAR_FUN_ATTR uint64_t umod64(uint64_t x, uint64_t y) {
  uint64_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return x % ys;
}

SCALAR_FUN_ATTR uint8_t udiv_safe8(uint8_t x, uint8_t y) {
  uint8_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return y == 0 ? 0 : x / ys;
}

SCALAR_FUN_ATTR uint16_t udiv_safe16(uint16_t x, uint16_t y) {
  uint16_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return y == 0 ? 0 : x / ys;
}

SCALAR_FUN_ATTR uint32_t udiv_safe32(uint32_t x, uint32_t y) {
  uint32_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return y == 0 ? 0 : x / ys;
}

SCALAR_FUN_ATTR uint64_t udiv_safe64(uint64_t x, uint64_t y) {
  uint64_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return y == 0 ? 0 : x / ys;
}

SCALAR_FUN_ATTR uint8_t udiv_up_safe8(uint8_t x, uint8_t y) {
  uint8_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return y == 0 ? 0 : (x + y - 1) / ys;
}

SCALAR_FUN_ATTR uint16_t udiv_up_safe16(uint16_t x, uint16_t y) {
  uint16_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return y == 0 ? 0 : (x + y - 1) / ys;
}

SCALAR_FUN_ATTR uint32_t udiv_up_safe32(uint32_t x, uint32_t y) {
  uint32_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return y == 0 ? 0 : (x + y - 1) / ys;
}

SCALAR_FUN_ATTR uint64_t udiv_up_safe64(uint64_t x, uint64_t y) {
  uint64_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return y == 0 ? 0 : (x + y - 1) / ys;
}

SCALAR_FUN_ATTR uint8_t umod_safe8(uint8_t x, uint8_t y) {
  uint8_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return y == 0 ? 0 : x % ys;
}

SCALAR_FUN_ATTR uint16_t umod_safe16(uint16_t x, uint16_t y) {
  uint16_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return y == 0 ? 0 : x % ys;
}

SCALAR_FUN_ATTR uint32_t umod_safe32(uint32_t x, uint32_t y) {
  uint32_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return y == 0 ? 0 : x % ys;
}

SCALAR_FUN_ATTR uint64_t umod_safe64(uint64_t x, uint64_t y) {
  uint64_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return y == 0 ? 0 : x % ys;
}

SCALAR_FUN_ATTR int8_t sdiv8(int8_t x, int8_t y) {
  int8_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  int8_t q = x / ys;
  int8_t r = x % ys;

  return q - ((r != 0 && r < 0 != y < 0) ? 1 : 0);
}

SCALAR_FUN_ATTR int16_t sdiv16(int16_t x, int16_t y) {
  int16_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  int16_t q = x / ys;
  int16_t r = x % ys;

  return q - ((r != 0 && r < 0 != y < 0) ? 1 : 0);
}

SCALAR_FUN_ATTR int32_t sdiv32(int32_t x, int32_t y) {
  int32_t ys = 1;
  foreach_active(i){
    ys = y;
  }
  int32_t q = x / ys;
  int32_t r = x % ys;

  return q - ((r != 0 && r < 0 != y < 0) ? 1 : 0);
}

SCALAR_FUN_ATTR int64_t sdiv64(int64_t x, int64_t y) {
  int64_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  int64_t q = x / ys;
  int64_t r = x % ys;

  return q - ((r != 0 && r < 0 != y < 0) ? 1 : 0);
}

SCALAR_FUN_ATTR int8_t sdiv_up8(int8_t x, int8_t y) {
  return sdiv8(x + y - 1, y);
}

SCALAR_FUN_ATTR int16_t sdiv_up16(int16_t x, int16_t y) {
  return sdiv16(x + y - 1, y);
}

SCALAR_FUN_ATTR int32_t sdiv_up32(int32_t x, int32_t y) {
  return sdiv32(x + y - 1, y);
}

SCALAR_FUN_ATTR int64_t sdiv_up64(int64_t x, int64_t y) {
  return sdiv64(x + y - 1, y);
}

SCALAR_FUN_ATTR int8_t smod8(int8_t x, int8_t y) {
  int8_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  int8_t r = x % ys;

  return r + (r == 0 || (x > 0 && y > 0) || (x < 0 && y < 0) ? 0 : y);
}

SCALAR_FUN_ATTR int16_t smod16(int16_t x, int16_t y) {
  int16_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  int16_t r = x % ys;

  return r + (r == 0 || (x > 0 && y > 0) || (x < 0 && y < 0) ? 0 : y);
}

SCALAR_FUN_ATTR int32_t smod32(int32_t x, int32_t y) {
  int32_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  int32_t r = x % ys;

  return r + (r == 0 || (x > 0 && y > 0) || (x < 0 && y < 0) ? 0 : y);
}

SCALAR_FUN_ATTR int64_t smod64(int64_t x, int64_t y) {
  int64_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  int64_t r = x % ys;

  return r + (r == 0 || (x > 0 && y > 0) || (x < 0 && y < 0) ? 0 : y);
}

SCALAR_FUN_ATTR int8_t sdiv_safe8(int8_t x, int8_t y) {
  return y == 0 ? 0 : sdiv8(x, y);
}

SCALAR_FUN_ATTR int16_t sdiv_safe16(int16_t x, int16_t y) {
  return y == 0 ? 0 : sdiv16(x, y);
}

SCALAR_FUN_ATTR int32_t sdiv_safe32(int32_t x, int32_t y) {
  return y == 0 ? 0 : sdiv32(x, y);
}

SCALAR_FUN_ATTR int64_t sdiv_safe64(int64_t x, int64_t y) {
  return y == 0 ? 0 : sdiv64(x, y);
}

SCALAR_FUN_ATTR int8_t sdiv_up_safe8(int8_t x, int8_t y) {
  return sdiv_safe8(x + y - 1, y);
}

SCALAR_FUN_ATTR int16_t sdiv_up_safe16(int16_t x, int16_t y) {
  return sdiv_safe16(x + y - 1, y);
}

SCALAR_FUN_ATTR int32_t sdiv_up_safe32(int32_t x, int32_t y) {
  return sdiv_safe32(x + y - 1, y);
}

SCALAR_FUN_ATTR int64_t sdiv_up_safe64(int64_t x, int64_t y) {
  return sdiv_safe64(x + y - 1, y);
}

SCALAR_FUN_ATTR int8_t smod_safe8(int8_t x, int8_t y) {
  return y == 0 ? 0 : smod8(x, y);
}

SCALAR_FUN_ATTR int16_t smod_safe16(int16_t x, int16_t y) {
  return y == 0 ? 0 : smod16(x, y);
}

SCALAR_FUN_ATTR int32_t smod_safe32(int32_t x, int32_t y) {
  return y == 0 ? 0 : smod32(x, y);
}

SCALAR_FUN_ATTR int64_t smod_safe64(int64_t x, int64_t y) {
  return y == 0 ? 0 : smod64(x, y);
}

SCALAR_FUN_ATTR int8_t squot8(int8_t x, int8_t y) {
  int8_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return x / ys;
}

SCALAR_FUN_ATTR int16_t squot16(int16_t x, int16_t y) {
  int16_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return x / ys;
}

SCALAR_FUN_ATTR int32_t squot32(int32_t x, int32_t y) {
  int32_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return x / ys;
}

SCALAR_FUN_ATTR int64_t squot64(int64_t x, int64_t y) {
  int64_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return x / ys;
}

SCALAR_FUN_ATTR int8_t srem8(int8_t x, int8_t y) {
  int8_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return x % ys;
}

SCALAR_FUN_ATTR int16_t srem16(int16_t x, int16_t y) {
  int16_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return x % ys;
}

SCALAR_FUN_ATTR int32_t srem32(int32_t x, int32_t y) {
  int32_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return x % ys;
}

SCALAR_FUN_ATTR int64_t srem64(int64_t x, int64_t y) {
  int8_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return x % ys;
}

SCALAR_FUN_ATTR int8_t squot_safe8(int8_t x, int8_t y) {
  int8_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return y == 0 ? 0 : x / ys;
}

SCALAR_FUN_ATTR int16_t squot_safe16(int16_t x, int16_t y) {
  int16_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return y == 0 ? 0 : x / ys;
}

SCALAR_FUN_ATTR int32_t squot_safe32(int32_t x, int32_t y) {
  int32_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return y == 0 ? 0 : x / ys;
}

SCALAR_FUN_ATTR int64_t squot_safe64(int64_t x, int64_t y) {
  int64_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return y == 0 ? 0 : x / ys;
}

SCALAR_FUN_ATTR int8_t srem_safe8(int8_t x, int8_t y) {
  int8_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return y == 0 ? 0 : x % ys;
}

SCALAR_FUN_ATTR int16_t srem_safe16(int16_t x, int16_t y) {
  int16_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return y == 0 ? 0 : x % ys;
}

SCALAR_FUN_ATTR int32_t srem_safe32(int32_t x, int32_t y) {
  int32_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return y == 0 ? 0 : x % ys;
}

SCALAR_FUN_ATTR int64_t srem_safe64(int64_t x, int64_t y) {
  int64_t ys = 1;
  foreach_active(i){
    ys = y;
  }

  return y == 0 ? 0 : x % ys;
}

#else

SCALAR_FUN_ATTR uint8_t udiv8(uint8_t x, uint8_t y) {
  return x / y;
}

SCALAR_FUN_ATTR uint16_t udiv16(uint16_t x, uint16_t y) {
  return x / y;
}

SCALAR_FUN_ATTR uint32_t udiv32(uint32_t x, uint32_t y) {
  return x / y;
}

SCALAR_FUN_ATTR uint64_t udiv64(uint64_t x, uint64_t y) {
  return x / y;
}

SCALAR_FUN_ATTR uint8_t udiv_up8(uint8_t x, uint8_t y) {
  return (x + y - 1) / y;
}

SCALAR_FUN_ATTR uint16_t udiv_up16(uint16_t x, uint16_t y) {
  return (x + y - 1) / y;
}

SCALAR_FUN_ATTR uint32_t udiv_up32(uint32_t x, uint32_t y) {
  return (x + y - 1) / y;
}

SCALAR_FUN_ATTR uint64_t udiv_up64(uint64_t x, uint64_t y) {
  return (x + y - 1) / y;
}

SCALAR_FUN_ATTR uint8_t umod8(uint8_t x, uint8_t y) {
  return x % y;
}

SCALAR_FUN_ATTR uint16_t umod16(uint16_t x, uint16_t y) {
  return x % y;
}

SCALAR_FUN_ATTR uint32_t umod32(uint32_t x, uint32_t y) {
  return x % y;
}

SCALAR_FUN_ATTR uint64_t umod64(uint64_t x, uint64_t y) {
  return x % y;
}

SCALAR_FUN_ATTR uint8_t udiv_safe8(uint8_t x, uint8_t y) {
  return y == 0 ? 0 : x / y;
}

SCALAR_FUN_ATTR uint16_t udiv_safe16(uint16_t x, uint16_t y) {
  return y == 0 ? 0 : x / y;
}

SCALAR_FUN_ATTR uint32_t udiv_safe32(uint32_t x, uint32_t y) {
  return y == 0 ? 0 : x / y;
}

SCALAR_FUN_ATTR uint64_t udiv_safe64(uint64_t x, uint64_t y) {
  return y == 0 ? 0 : x / y;
}

SCALAR_FUN_ATTR uint8_t udiv_up_safe8(uint8_t x, uint8_t y) {
  return y == 0 ? 0 : (x + y - 1) / y;
}

SCALAR_FUN_ATTR uint16_t udiv_up_safe16(uint16_t x, uint16_t y) {
  return y == 0 ? 0 : (x + y - 1) / y;
}

SCALAR_FUN_ATTR uint32_t udiv_up_safe32(uint32_t x, uint32_t y) {
  return y == 0 ? 0 : (x + y - 1) / y;
}

SCALAR_FUN_ATTR uint64_t udiv_up_safe64(uint64_t x, uint64_t y) {
  return y == 0 ? 0 : (x + y - 1) / y;
}

SCALAR_FUN_ATTR uint8_t umod_safe8(uint8_t x, uint8_t y) {
  return y == 0 ? 0 : x % y;
}

SCALAR_FUN_ATTR uint16_t umod_safe16(uint16_t x, uint16_t y) {
  return y == 0 ? 0 : x % y;
}

SCALAR_FUN_ATTR uint32_t umod_safe32(uint32_t x, uint32_t y) {
  return y == 0 ? 0 : x % y;
}

SCALAR_FUN_ATTR uint64_t umod_safe64(uint64_t x, uint64_t y) {
  return y == 0 ? 0 : x % y;
}

SCALAR_FUN_ATTR int8_t sdiv8(int8_t x, int8_t y) {
  int8_t q = x / y;
  int8_t r = x % y;

  return q - ((r != 0 && r < 0 != y < 0) ? 1 : 0);
}

SCALAR_FUN_ATTR int16_t sdiv16(int16_t x, int16_t y) {
  int16_t q = x / y;
  int16_t r = x % y;

  return q - ((r != 0 && r < 0 != y < 0) ? 1 : 0);
}

SCALAR_FUN_ATTR int32_t sdiv32(int32_t x, int32_t y) {
  int32_t q = x / y;
  int32_t r = x % y;

  return q - ((r != 0 && r < 0 != y < 0) ? 1 : 0);
}

SCALAR_FUN_ATTR int64_t sdiv64(int64_t x, int64_t y) {
  int64_t q = x / y;
  int64_t r = x % y;

  return q - ((r != 0 && r < 0 != y < 0) ? 1 : 0);
}

SCALAR_FUN_ATTR int8_t sdiv_up8(int8_t x, int8_t y) {
  return sdiv8(x + y - 1, y);
}

SCALAR_FUN_ATTR int16_t sdiv_up16(int16_t x, int16_t y) {
  return sdiv16(x + y - 1, y);
}

SCALAR_FUN_ATTR int32_t sdiv_up32(int32_t x, int32_t y) {
  return sdiv32(x + y - 1, y);
}

SCALAR_FUN_ATTR int64_t sdiv_up64(int64_t x, int64_t y) {
  return sdiv64(x + y - 1, y);
}

SCALAR_FUN_ATTR int8_t smod8(int8_t x, int8_t y) {
  int8_t r = x % y;

  return r + (r == 0 || (x > 0 && y > 0) || (x < 0 && y < 0) ? 0 : y);
}

SCALAR_FUN_ATTR int16_t smod16(int16_t x, int16_t y) {
  int16_t r = x % y;

  return r + (r == 0 || (x > 0 && y > 0) || (x < 0 && y < 0) ? 0 : y);
}

SCALAR_FUN_ATTR int32_t smod32(int32_t x, int32_t y) {
  int32_t r = x % y;

  return r + (r == 0 || (x > 0 && y > 0) || (x < 0 && y < 0) ? 0 : y);
}

SCALAR_FUN_ATTR int64_t smod64(int64_t x, int64_t y) {
  int64_t r = x % y;

  return r + (r == 0 || (x > 0 && y > 0) || (x < 0 && y < 0) ? 0 : y);
}

SCALAR_FUN_ATTR int8_t sdiv_safe8(int8_t x, int8_t y) {
  return y == 0 ? 0 : sdiv8(x, y);
}

SCALAR_FUN_ATTR int16_t sdiv_safe16(int16_t x, int16_t y) {
  return y == 0 ? 0 : sdiv16(x, y);
}

SCALAR_FUN_ATTR int32_t sdiv_safe32(int32_t x, int32_t y) {
  return y == 0 ? 0 : sdiv32(x, y);
}

SCALAR_FUN_ATTR int64_t sdiv_safe64(int64_t x, int64_t y) {
  return y == 0 ? 0 : sdiv64(x, y);
}

SCALAR_FUN_ATTR int8_t sdiv_up_safe8(int8_t x, int8_t y) {
  return sdiv_safe8(x + y - 1, y);
}

SCALAR_FUN_ATTR int16_t sdiv_up_safe16(int16_t x, int16_t y) {
  return sdiv_safe16(x + y - 1, y);
}

SCALAR_FUN_ATTR int32_t sdiv_up_safe32(int32_t x, int32_t y) {
  return sdiv_safe32(x + y - 1, y);
}

SCALAR_FUN_ATTR int64_t sdiv_up_safe64(int64_t x, int64_t y) {
  return sdiv_safe64(x + y - 1, y);
}

SCALAR_FUN_ATTR int8_t smod_safe8(int8_t x, int8_t y) {
  return y == 0 ? 0 : smod8(x, y);
}

SCALAR_FUN_ATTR int16_t smod_safe16(int16_t x, int16_t y) {
  return y == 0 ? 0 : smod16(x, y);
}

SCALAR_FUN_ATTR int32_t smod_safe32(int32_t x, int32_t y) {
  return y == 0 ? 0 : smod32(x, y);
}

SCALAR_FUN_ATTR int64_t smod_safe64(int64_t x, int64_t y) {
  return y == 0 ? 0 : smod64(x, y);
}

SCALAR_FUN_ATTR int8_t squot8(int8_t x, int8_t y) {
  return x / y;
}

SCALAR_FUN_ATTR int16_t squot16(int16_t x, int16_t y) {
  return x / y;
}

SCALAR_FUN_ATTR int32_t squot32(int32_t x, int32_t y) {
  return x / y;
}

SCALAR_FUN_ATTR int64_t squot64(int64_t x, int64_t y) {
  return x / y;
}

SCALAR_FUN_ATTR int8_t srem8(int8_t x, int8_t y) {
  return x % y;
}

SCALAR_FUN_ATTR int16_t srem16(int16_t x, int16_t y) {
  return x % y;
}

SCALAR_FUN_ATTR int32_t srem32(int32_t x, int32_t y) {
  return x % y;
}

SCALAR_FUN_ATTR int64_t srem64(int64_t x, int64_t y) {
  return x % y;
}

SCALAR_FUN_ATTR int8_t squot_safe8(int8_t x, int8_t y) {
  return y == 0 ? 0 : x / y;
}

SCALAR_FUN_ATTR int16_t squot_safe16(int16_t x, int16_t y) {
  return y == 0 ? 0 : x / y;
}

SCALAR_FUN_ATTR int32_t squot_safe32(int32_t x, int32_t y) {
  return y == 0 ? 0 : x / y;
}

SCALAR_FUN_ATTR int64_t squot_safe64(int64_t x, int64_t y) {
  return y == 0 ? 0 : x / y;
}

SCALAR_FUN_ATTR int8_t srem_safe8(int8_t x, int8_t y) {
  return y == 0 ? 0 : x % y;
}

SCALAR_FUN_ATTR int16_t srem_safe16(int16_t x, int16_t y) {
  return y == 0 ? 0 : x % y;
}

SCALAR_FUN_ATTR int32_t srem_safe32(int32_t x, int32_t y) {
  return y == 0 ? 0 : x % y;
}

SCALAR_FUN_ATTR int64_t srem_safe64(int64_t x, int64_t y) {
  return y == 0 ? 0 : x % y;
}

#endif

SCALAR_FUN_ATTR int8_t smin8(int8_t x, int8_t y) {
  return x < y ? x : y;
}

SCALAR_FUN_ATTR int16_t smin16(int16_t x, int16_t y) {
  return x < y ? x : y;
}

SCALAR_FUN_ATTR int32_t smin32(int32_t x, int32_t y) {
  return x < y ? x : y;
}

SCALAR_FUN_ATTR int64_t smin64(int64_t x, int64_t y) {
  return x < y ? x : y;
}

SCALAR_FUN_ATTR uint8_t umin8(uint8_t x, uint8_t y) {
  return x < y ? x : y;
}

SCALAR_FUN_ATTR uint16_t umin16(uint16_t x, uint16_t y) {
  return x < y ? x : y;
}

SCALAR_FUN_ATTR uint32_t umin32(uint32_t x, uint32_t y) {
  return x < y ? x : y;
}

SCALAR_FUN_ATTR uint64_t umin64(uint64_t x, uint64_t y) {
  return x < y ? x : y;
}

SCALAR_FUN_ATTR int8_t smax8(int8_t x, int8_t y) {
  return x < y ? y : x;
}

SCALAR_FUN_ATTR int16_t smax16(int16_t x, int16_t y) {
  return x < y ? y : x;
}

SCALAR_FUN_ATTR int32_t smax32(int32_t x, int32_t y) {
  return x < y ? y : x;
}

SCALAR_FUN_ATTR int64_t smax64(int64_t x, int64_t y) {
  return x < y ? y : x;
}

SCALAR_FUN_ATTR uint8_t umax8(uint8_t x, uint8_t y) {
  return x < y ? y : x;
}

SCALAR_FUN_ATTR uint16_t umax16(uint16_t x, uint16_t y) {
  return x < y ? y : x;
}

SCALAR_FUN_ATTR uint32_t umax32(uint32_t x, uint32_t y) {
  return x < y ? y : x;
}

SCALAR_FUN_ATTR uint64_t umax64(uint64_t x, uint64_t y) {
  return x < y ? y : x;
}

SCALAR_FUN_ATTR uint8_t shl8(uint8_t x, uint8_t y) {
  return (uint8_t)(x << y);
}

SCALAR_FUN_ATTR uint16_t shl16(uint16_t x, uint16_t y) {
  return (uint16_t)(x << y);
}

SCALAR_FUN_ATTR uint32_t shl32(uint32_t x, uint32_t y) {
  return x << y;
}

SCALAR_FUN_ATTR uint64_t shl64(uint64_t x, uint64_t y) {
  return x << y;
}

SCALAR_FUN_ATTR uint8_t lshr8(uint8_t x, uint8_t y) {
  return x >> y;
}

SCALAR_FUN_ATTR uint16_t lshr16(uint16_t x, uint16_t y) {
  return x >> y;
}

SCALAR_FUN_ATTR uint32_t lshr32(uint32_t x, uint32_t y) {
  return x >> y;
}

SCALAR_FUN_ATTR uint64_t lshr64(uint64_t x, uint64_t y) {
  return x >> y;
}

SCALAR_FUN_ATTR int8_t ashr8(int8_t x, int8_t y) {
  return x >> y;
}

SCALAR_FUN_ATTR int16_t ashr16(int16_t x, int16_t y) {
  return x >> y;
}

SCALAR_FUN_ATTR int32_t ashr32(int32_t x, int32_t y) {
  return x >> y;
}

SCALAR_FUN_ATTR int64_t ashr64(int64_t x, int64_t y) {
  return x >> y;
}

SCALAR_FUN_ATTR uint8_t and8(uint8_t x, uint8_t y) {
  return x & y;
}

SCALAR_FUN_ATTR uint16_t and16(uint16_t x, uint16_t y) {
  return x & y;
}

SCALAR_FUN_ATTR uint32_t and32(uint32_t x, uint32_t y) {
  return x & y;
}

SCALAR_FUN_ATTR uint64_t and64(uint64_t x, uint64_t y) {
  return x & y;
}

SCALAR_FUN_ATTR uint8_t or8(uint8_t x, uint8_t y) {
  return x | y;
}

SCALAR_FUN_ATTR uint16_t or16(uint16_t x, uint16_t y) {
  return x | y;
}

SCALAR_FUN_ATTR uint32_t or32(uint32_t x, uint32_t y) {
  return x | y;
}

SCALAR_FUN_ATTR uint64_t or64(uint64_t x, uint64_t y) {
  return x | y;
}

SCALAR_FUN_ATTR uint8_t xor8(uint8_t x, uint8_t y) {
  return x ^ y;
}

SCALAR_FUN_ATTR uint16_t xor16(uint16_t x, uint16_t y) {
  return x ^ y;
}

SCALAR_FUN_ATTR uint32_t xor32(uint32_t x, uint32_t y) {
  return x ^ y;
}

SCALAR_FUN_ATTR uint64_t xor64(uint64_t x, uint64_t y) {
  return x ^ y;
}

SCALAR_FUN_ATTR bool ult8(uint8_t x, uint8_t y) {
  return x < y;
}

SCALAR_FUN_ATTR bool ult16(uint16_t x, uint16_t y) {
  return x < y;
}

SCALAR_FUN_ATTR bool ult32(uint32_t x, uint32_t y) {
  return x < y;
}

SCALAR_FUN_ATTR bool ult64(uint64_t x, uint64_t y) {
  return x < y;
}

SCALAR_FUN_ATTR bool ule8(uint8_t x, uint8_t y) {
  return x <= y;
}

SCALAR_FUN_ATTR bool ule16(uint16_t x, uint16_t y) {
  return x <= y;
}

SCALAR_FUN_ATTR bool ule32(uint32_t x, uint32_t y) {
  return x <= y;
}

SCALAR_FUN_ATTR bool ule64(uint64_t x, uint64_t y) {
  return x <= y;
}

SCALAR_FUN_ATTR bool slt8(int8_t x, int8_t y) {
  return x < y;
}

SCALAR_FUN_ATTR bool slt16(int16_t x, int16_t y) {
  return x < y;
}

SCALAR_FUN_ATTR bool slt32(int32_t x, int32_t y) {
  return x < y;
}

SCALAR_FUN_ATTR bool slt64(int64_t x, int64_t y) {
  return x < y;
}

SCALAR_FUN_ATTR bool sle8(int8_t x, int8_t y) {
  return x <= y;
}

SCALAR_FUN_ATTR bool sle16(int16_t x, int16_t y) {
  return x <= y;
}

SCALAR_FUN_ATTR bool sle32(int32_t x, int32_t y) {
  return x <= y;
}

SCALAR_FUN_ATTR bool sle64(int64_t x, int64_t y) {
  return x <= y;
}

SCALAR_FUN_ATTR uint8_t pow8(uint8_t x, uint8_t y) {
  uint8_t res = 1, rem = y;

  while (rem != 0) {
    if (rem & 1)
      res *= x;
    rem >>= 1;
    x *= x;
  }
  return res;
}

SCALAR_FUN_ATTR uint16_t pow16(uint16_t x, uint16_t y) {
  uint16_t res = 1, rem = y;

  while (rem != 0) {
    if (rem & 1)
      res *= x;
    rem >>= 1;
    x *= x;
  }
  return res;
}

SCALAR_FUN_ATTR uint32_t pow32(uint32_t x, uint32_t y) {
  uint32_t res = 1, rem = y;

  while (rem != 0) {
    if (rem & 1)
      res *= x;
    rem >>= 1;
    x *= x;
  }
  return res;
}

SCALAR_FUN_ATTR uint64_t pow64(uint64_t x, uint64_t y) {
  uint64_t res = 1, rem = y;

  while (rem != 0) {
    if (rem & 1)
      res *= x;
    rem >>= 1;
    x *= x;
  }
  return res;
}

SCALAR_FUN_ATTR bool itob_i8_bool(int8_t x) {
  return x != 0;
}

SCALAR_FUN_ATTR bool itob_i16_bool(int16_t x) {
  return x != 0;
}

SCALAR_FUN_ATTR bool itob_i32_bool(int32_t x) {
  return x != 0;
}

SCALAR_FUN_ATTR bool itob_i64_bool(int64_t x) {
  return x != 0;
}

SCALAR_FUN_ATTR int8_t btoi_bool_i8(bool x) {
  return x;
}

SCALAR_FUN_ATTR int16_t btoi_bool_i16(bool x) {
  return x;
}

SCALAR_FUN_ATTR int32_t btoi_bool_i32(bool x) {
  return x;
}

SCALAR_FUN_ATTR int64_t btoi_bool_i64(bool x) {
  return x;
}

#define sext_i8_i8(x) ((int8_t) (int8_t) (x))
#define sext_i8_i16(x) ((int16_t) (int8_t) (x))
#define sext_i8_i32(x) ((int32_t) (int8_t) (x))
#define sext_i8_i64(x) ((int64_t) (int8_t) (x))
#define sext_i16_i8(x) ((int8_t) (int16_t) (x))
#define sext_i16_i16(x) ((int16_t) (int16_t) (x))
#define sext_i16_i32(x) ((int32_t) (int16_t) (x))
#define sext_i16_i64(x) ((int64_t) (int16_t) (x))
#define sext_i32_i8(x) ((int8_t) (int32_t) (x))
#define sext_i32_i16(x) ((int16_t) (int32_t) (x))
#define sext_i32_i32(x) ((int32_t) (int32_t) (x))
#define sext_i32_i64(x) ((int64_t) (int32_t) (x))
#define sext_i64_i8(x) ((int8_t) (int64_t) (x))
#define sext_i64_i16(x) ((int16_t) (int64_t) (x))
#define sext_i64_i32(x) ((int32_t) (int64_t) (x))
#define sext_i64_i64(x) ((int64_t) (int64_t) (x))
#define zext_i8_i8(x) ((int8_t) (uint8_t) (x))
#define zext_i8_i16(x) ((int16_t) (uint8_t) (x))
#define zext_i8_i32(x) ((int32_t) (uint8_t) (x))
#define zext_i8_i64(x) ((int64_t) (uint8_t) (x))
#define zext_i16_i8(x) ((int8_t) (uint16_t) (x))
#define zext_i16_i16(x) ((int16_t) (uint16_t) (x))
#define zext_i16_i32(x) ((int32_t) (uint16_t) (x))
#define zext_i16_i64(x) ((int64_t) (uint16_t) (x))
#define zext_i32_i8(x) ((int8_t) (uint32_t) (x))
#define zext_i32_i16(x) ((int16_t) (uint32_t) (x))
#define zext_i32_i32(x) ((int32_t) (uint32_t) (x))
#define zext_i32_i64(x) ((int64_t) (uint32_t) (x))
#define zext_i64_i8(x) ((int8_t) (uint64_t) (x))
#define zext_i64_i16(x) ((int16_t) (uint64_t) (x))
#define zext_i64_i32(x) ((int32_t) (uint64_t) (x))
#define zext_i64_i64(x) ((int64_t) (uint64_t) (x))

SCALAR_FUN_ATTR int8_t abs8(int8_t x) {
  return (int8_t)abs(x);
}

SCALAR_FUN_ATTR int16_t abs16(int16_t x) {
  return (int16_t)abs(x);
}

SCALAR_FUN_ATTR int32_t abs32(int32_t x) {
  return abs(x);
}

SCALAR_FUN_ATTR int64_t abs64(int64_t x) {
#if defined(__OPENCL_VERSION__) || defined(ISPC)
  return abs(x);
#else
  return llabs(x);
#endif
}

#if defined(__OPENCL_VERSION__)
SCALAR_FUN_ATTR int32_t futrts_popc8(int8_t x) {
  return popcount(x);
}

SCALAR_FUN_ATTR int32_t futrts_popc16(int16_t x) {
  return popcount(x);
}

SCALAR_FUN_ATTR int32_t futrts_popc32(int32_t x) {
  return popcount(x);
}

SCALAR_FUN_ATTR int32_t futrts_popc64(int64_t x) {
  return popcount(x);
}
#elif defined(__CUDA_ARCH__)

SCALAR_FUN_ATTR int32_t futrts_popc8(int8_t x) {
  return __popc(zext_i8_i32(x));
}

SCALAR_FUN_ATTR int32_t futrts_popc16(int16_t x) {
  return __popc(zext_i16_i32(x));
}

SCALAR_FUN_ATTR int32_t futrts_popc32(int32_t x) {
  return __popc(x);
}

SCALAR_FUN_ATTR int32_t futrts_popc64(int64_t x) {
  return __popcll(x);
}

#else // Not OpenCL or CUDA, but plain C.

SCALAR_FUN_ATTR int32_t futrts_popc8(uint8_t x) {
  int c = 0;
  for (; x; ++c) { x &= x - 1; }
  return c;
}

SCALAR_FUN_ATTR int32_t futrts_popc16(uint16_t x) {
  int c = 0;
  for (; x; ++c) { x &= x - 1; }
  return c;
}

SCALAR_FUN_ATTR int32_t futrts_popc32(uint32_t x) {
  int c = 0;
  for (; x; ++c) { x &= x - 1; }
  return c;
}

SCALAR_FUN_ATTR int32_t futrts_popc64(uint64_t x) {
  int c = 0;
  for (; x; ++c) { x &= x - 1; }
  return c;
}
#endif

#if defined(__OPENCL_VERSION__)
SCALAR_FUN_ATTR uint8_t  futrts_umul_hi8 ( uint8_t a,  uint8_t b) { return mul_hi(a, b); }
SCALAR_FUN_ATTR uint16_t futrts_umul_hi16(uint16_t a, uint16_t b) { return mul_hi(a, b); }
SCALAR_FUN_ATTR uint32_t futrts_umul_hi32(uint32_t a, uint32_t b) { return mul_hi(a, b); }
SCALAR_FUN_ATTR uint64_t futrts_umul_hi64(uint64_t a, uint64_t b) { return mul_hi(a, b); }
SCALAR_FUN_ATTR uint8_t  futrts_smul_hi8 ( int8_t a,  int8_t b) { return mul_hi(a, b); }
SCALAR_FUN_ATTR uint16_t futrts_smul_hi16(int16_t a, int16_t b) { return mul_hi(a, b); }
SCALAR_FUN_ATTR uint32_t futrts_smul_hi32(int32_t a, int32_t b) { return mul_hi(a, b); }
SCALAR_FUN_ATTR uint64_t futrts_smul_hi64(int64_t a, int64_t b) { return mul_hi(a, b); }
#elif defined(__CUDA_ARCH__)
SCALAR_FUN_ATTR  uint8_t futrts_umul_hi8(uint8_t a, uint8_t b) { return ((uint16_t)a) * ((uint16_t)b) >> 8; }
SCALAR_FUN_ATTR uint16_t futrts_umul_hi16(uint16_t a, uint16_t b) { return ((uint32_t)a) * ((uint32_t)b) >> 16; }
SCALAR_FUN_ATTR uint32_t futrts_umul_hi32(uint32_t a, uint32_t b) { return __umulhi(a, b); }
SCALAR_FUN_ATTR uint64_t futrts_umul_hi64(uint64_t a, uint64_t b) { return __umul64hi(a, b); }
SCALAR_FUN_ATTR  uint8_t futrts_smul_hi8 ( int8_t a, int8_t b) { return ((int16_t)a) * ((int16_t)b) >> 8; }
SCALAR_FUN_ATTR uint16_t futrts_smul_hi16(int16_t a, int16_t b) { return ((int32_t)a) * ((int32_t)b) >> 16; }
SCALAR_FUN_ATTR uint32_t futrts_smul_hi32(int32_t a, int32_t b) { return __mulhi(a, b); }
SCALAR_FUN_ATTR uint64_t futrts_smul_hi64(int64_t a, int64_t b) { return __mul64hi(a, b); }
#elif ISPC
SCALAR_FUN_ATTR uint8_t futrts_umul_hi8(uint8_t a, uint8_t b) { return ((uint16_t)a) * ((uint16_t)b) >> 8; }
SCALAR_FUN_ATTR uint16_t futrts_umul_hi16(uint16_t a, uint16_t b) { return ((uint32_t)a) * ((uint32_t)b) >> 16; }
SCALAR_FUN_ATTR uint32_t futrts_umul_hi32(uint32_t a, uint32_t b) { return ((uint64_t)a) * ((uint64_t)b) >> 32; }
SCALAR_FUN_ATTR uint64_t futrts_umul_hi64(uint64_t a, uint64_t b) {
  uint64_t ah = a >> 32;
  uint64_t al = a & 0xffffffff;
  uint64_t bh = b >> 32;
  uint64_t bl = b & 0xffffffff;

  uint64_t p1 = al * bl;
  uint64_t p2 = al * bh;
  uint64_t p3 = ah * bl;
  uint64_t p4 = ah * bh;

  uint64_t p1h = p1 >> 32;
  uint64_t p2h = p2 >> 32;
  uint64_t p3h = p3 >> 32;
  uint64_t p2l = p2 & 0xffffffff;
  uint64_t p3l = p3 & 0xffffffff;

  uint64_t l = p1h + p2l + p3l;
  uint64_t m = (p2 >> 32) + (p3 >> 32);
  uint64_t h = (l >> 32) + m + p4;

  return h;
}
SCALAR_FUN_ATTR  int8_t futrts_smul_hi8 ( int8_t a,  int8_t b) { return ((uint16_t)a) * ((uint16_t)b) >> 8; }
SCALAR_FUN_ATTR int16_t futrts_smul_hi16(int16_t a, int16_t b) { return ((uint32_t)a) * ((uint32_t)b) >> 16; }
SCALAR_FUN_ATTR int32_t futrts_smul_hi32(int32_t a, int32_t b) { return ((uint64_t)a) * ((uint64_t)b) >> 32; }
SCALAR_FUN_ATTR int64_t futrts_smul_hi64(int64_t a, int64_t b) {
  uint64_t ah = a >> 32;
  uint64_t al = a & 0xffffffff;
  uint64_t bh = b >> 32;
  uint64_t bl = b & 0xffffffff;

  uint64_t p1 =  al * bl;
  int64_t  p2 = al * bh;
  int64_t  p3 = ah * bl;
  uint64_t p4 =  ah * bh;

  uint64_t p1h = p1 >> 32;
  uint64_t p2h = p2 >> 32;
  uint64_t p3h = p3 >> 32;
  uint64_t p2l = p2 & 0xffffffff;
  uint64_t p3l = p3 & 0xffffffff;

  uint64_t l = p1h + p2l + p3l;
  uint64_t m = (p2 >> 32) + (p3 >> 32);
  uint64_t h = (l >> 32) + m + p4;

  return h;
}

#else // Not OpenCL, ISPC, or CUDA, but plain C.
SCALAR_FUN_ATTR uint8_t futrts_umul_hi8(uint8_t a, uint8_t b) { return ((uint16_t)a) * ((uint16_t)b) >> 8; }
SCALAR_FUN_ATTR uint16_t futrts_umul_hi16(uint16_t a, uint16_t b) { return ((uint32_t)a) * ((uint32_t)b) >> 16; }
SCALAR_FUN_ATTR uint32_t futrts_umul_hi32(uint32_t a, uint32_t b) { return ((uint64_t)a) * ((uint64_t)b) >> 32; }
SCALAR_FUN_ATTR uint64_t futrts_umul_hi64(uint64_t a, uint64_t b) { return ((__uint128_t)a) * ((__uint128_t)b) >> 64; }
SCALAR_FUN_ATTR int8_t futrts_smul_hi8(int8_t a, int8_t b) { return ((int16_t)a) * ((int16_t)b) >> 8; }
SCALAR_FUN_ATTR int16_t futrts_smul_hi16(int16_t a, int16_t b) { return ((int32_t)a) * ((int32_t)b) >> 16; }
SCALAR_FUN_ATTR int32_t futrts_smul_hi32(int32_t a, int32_t b) { return ((int64_t)a) * ((int64_t)b) >> 32; }
SCALAR_FUN_ATTR int64_t futrts_smul_hi64(int64_t a, int64_t b) { return ((__int128_t)a) * ((__int128_t)b) >> 64; }
#endif

#if defined(__OPENCL_VERSION__)
SCALAR_FUN_ATTR  uint8_t futrts_umad_hi8 ( uint8_t a,  uint8_t b,  uint8_t c) { return mad_hi(a, b, c); }
SCALAR_FUN_ATTR uint16_t futrts_umad_hi16(uint16_t a, uint16_t b, uint16_t c) { return mad_hi(a, b, c); }
SCALAR_FUN_ATTR uint32_t futrts_umad_hi32(uint32_t a, uint32_t b, uint32_t c) { return mad_hi(a, b, c); }
SCALAR_FUN_ATTR uint64_t futrts_umad_hi64(uint64_t a, uint64_t b, uint64_t c) { return mad_hi(a, b, c); }
SCALAR_FUN_ATTR  uint8_t futrts_smad_hi8( int8_t a,  int8_t b,   int8_t c) { return mad_hi(a, b, c); }
SCALAR_FUN_ATTR uint16_t futrts_smad_hi16(int16_t a, int16_t b, int16_t c) { return mad_hi(a, b, c); }
SCALAR_FUN_ATTR uint32_t futrts_smad_hi32(int32_t a, int32_t b, int32_t c) { return mad_hi(a, b, c); }
SCALAR_FUN_ATTR uint64_t futrts_smad_hi64(int64_t a, int64_t b, int64_t c) { return mad_hi(a, b, c); }
#else // Not OpenCL

SCALAR_FUN_ATTR  uint8_t futrts_umad_hi8( uint8_t a,  uint8_t b,  uint8_t c) { return futrts_umul_hi8(a, b) + c; }
SCALAR_FUN_ATTR uint16_t futrts_umad_hi16(uint16_t a, uint16_t b, uint16_t c) { return futrts_umul_hi16(a, b) + c; }
SCALAR_FUN_ATTR uint32_t futrts_umad_hi32(uint32_t a, uint32_t b, uint32_t c) { return futrts_umul_hi32(a, b) + c; }
SCALAR_FUN_ATTR uint64_t futrts_umad_hi64(uint64_t a, uint64_t b, uint64_t c) { return futrts_umul_hi64(a, b) + c; }
SCALAR_FUN_ATTR  uint8_t futrts_smad_hi8 ( int8_t a,  int8_t b,  int8_t c) { return futrts_smul_hi8(a, b) + c; }
SCALAR_FUN_ATTR uint16_t futrts_smad_hi16(int16_t a, int16_t b, int16_t c) { return futrts_smul_hi16(a, b) + c; }
SCALAR_FUN_ATTR uint32_t futrts_smad_hi32(int32_t a, int32_t b, int32_t c) { return futrts_smul_hi32(a, b) + c; }
SCALAR_FUN_ATTR uint64_t futrts_smad_hi64(int64_t a, int64_t b, int64_t c) { return futrts_smul_hi64(a, b) + c; }
#endif

#if defined(__OPENCL_VERSION__)
SCALAR_FUN_ATTR int32_t futrts_clzz8(int8_t x) {
  return clz(x);
}

SCALAR_FUN_ATTR int32_t futrts_clzz16(int16_t x) {
  return clz(x);
}

SCALAR_FUN_ATTR int32_t futrts_clzz32(int32_t x) {
  return clz(x);
}

SCALAR_FUN_ATTR int32_t futrts_clzz64(int64_t x) {
  return clz(x);
}

#elif defined(__CUDA_ARCH__)

SCALAR_FUN_ATTR int32_t futrts_clzz8(int8_t x) {
  return __clz(zext_i8_i32(x)) - 24;
}

SCALAR_FUN_ATTR int32_t futrts_clzz16(int16_t x) {
  return __clz(zext_i16_i32(x)) - 16;
}

SCALAR_FUN_ATTR int32_t futrts_clzz32(int32_t x) {
  return __clz(x);
}

SCALAR_FUN_ATTR int32_t futrts_clzz64(int64_t x) {
  return __clzll(x);
}

#elif ISPC

SCALAR_FUN_ATTR int32_t futrts_clzz8(int8_t x) {
  return count_leading_zeros((int32_t)(uint8_t)x)-24;
}

SCALAR_FUN_ATTR int32_t futrts_clzz16(int16_t x) {
  return count_leading_zeros((int32_t)(uint16_t)x)-16;
}

SCALAR_FUN_ATTR int32_t futrts_clzz32(int32_t x) {
  return count_leading_zeros(x);
}

SCALAR_FUN_ATTR int32_t futrts_clzz64(int64_t x) {
  return count_leading_zeros(x);
}

#else // Not OpenCL, ISPC or CUDA, but plain C.

SCALAR_FUN_ATTR int32_t futrts_clzz8(int8_t x) {
  return x == 0 ? 8 : __builtin_clz((uint32_t)zext_i8_i32(x)) - 24;
}

SCALAR_FUN_ATTR int32_t futrts_clzz16(int16_t x) {
  return x == 0 ? 16 : __builtin_clz((uint32_t)zext_i16_i32(x)) - 16;
}

SCALAR_FUN_ATTR int32_t futrts_clzz32(int32_t x) {
  return x == 0 ? 32 : __builtin_clz((uint32_t)x);
}

SCALAR_FUN_ATTR int32_t futrts_clzz64(int64_t x) {
  return x == 0 ? 64 : __builtin_clzll((uint64_t)x);
}
#endif

#if defined(__OPENCL_VERSION__)
SCALAR_FUN_ATTR int32_t futrts_ctzz8(int8_t x) {
  int i = 0;
  for (; i < 8 && (x & 1) == 0; i++, x >>= 1)
    ;
  return i;
}

SCALAR_FUN_ATTR int32_t futrts_ctzz16(int16_t x) {
  int i = 0;
  for (; i < 16 && (x & 1) == 0; i++, x >>= 1)
    ;
  return i;
}

SCALAR_FUN_ATTR int32_t futrts_ctzz32(int32_t x) {
  int i = 0;
  for (; i < 32 && (x & 1) == 0; i++, x >>= 1)
    ;
  return i;
}

SCALAR_FUN_ATTR int32_t futrts_ctzz64(int64_t x) {
  int i = 0;
  for (; i < 64 && (x & 1) == 0; i++, x >>= 1)
    ;
  return i;
}

#elif defined(__CUDA_ARCH__)

SCALAR_FUN_ATTR int32_t futrts_ctzz8(int8_t x) {
  int y = __ffs(x);
  return y == 0 ? 8 : y - 1;
}

SCALAR_FUN_ATTR int32_t futrts_ctzz16(int16_t x) {
  int y = __ffs(x);
  return y == 0 ? 16 : y - 1;
}

SCALAR_FUN_ATTR int32_t futrts_ctzz32(int32_t x) {
  int y = __ffs(x);
  return y == 0 ? 32 : y - 1;
}

SCALAR_FUN_ATTR int32_t futrts_ctzz64(int64_t x) {
  int y = __ffsll(x);
  return y == 0 ? 64 : y - 1;
}

#elif ISPC

SCALAR_FUN_ATTR int32_t futrts_ctzz8(int8_t x) {
  return x == 0 ? 8 : count_trailing_zeros((int32_t)x);
}

SCALAR_FUN_ATTR int32_t futrts_ctzz16(int16_t x) {
  return x == 0 ? 16 : count_trailing_zeros((int32_t)x);
}

SCALAR_FUN_ATTR int32_t futrts_ctzz32(int32_t x) {
  return count_trailing_zeros(x);
}

SCALAR_FUN_ATTR int32_t futrts_ctzz64(int64_t x) {
  return count_trailing_zeros(x);
}

#else // Not OpenCL or CUDA, but plain C.

SCALAR_FUN_ATTR int32_t futrts_ctzz8(int8_t x) {
  return x == 0 ? 8 : __builtin_ctz((uint32_t)x);
}

SCALAR_FUN_ATTR int32_t futrts_ctzz16(int16_t x) {
  return x == 0 ? 16 : __builtin_ctz((uint32_t)x);
}

SCALAR_FUN_ATTR int32_t futrts_ctzz32(int32_t x) {
  return x == 0 ? 32 : __builtin_ctz((uint32_t)x);
}

SCALAR_FUN_ATTR int32_t futrts_ctzz64(int64_t x) {
  return x == 0 ? 64 : __builtin_ctzll((uint64_t)x);
}
#endif

SCALAR_FUN_ATTR float fdiv32(float x, float y) {
  return x / y;
}

SCALAR_FUN_ATTR float fadd32(float x, float y) {
  return x + y;
}

SCALAR_FUN_ATTR float fsub32(float x, float y) {
  return x - y;
}

SCALAR_FUN_ATTR float fmul32(float x, float y) {
  return x * y;
}

SCALAR_FUN_ATTR bool cmplt32(float x, float y) {
  return x < y;
}

SCALAR_FUN_ATTR bool cmple32(float x, float y) {
  return x <= y;
}

SCALAR_FUN_ATTR float sitofp_i8_f32(int8_t x) {
  return (float) x;
}

SCALAR_FUN_ATTR float sitofp_i16_f32(int16_t x) {
  return (float) x;
}

SCALAR_FUN_ATTR float sitofp_i32_f32(int32_t x) {
  return (float) x;
}

SCALAR_FUN_ATTR float sitofp_i64_f32(int64_t x) {
  return (float) x;
}

SCALAR_FUN_ATTR float uitofp_i8_f32(uint8_t x) {
  return (float) x;
}

SCALAR_FUN_ATTR float uitofp_i16_f32(uint16_t x) {
  return (float) x;
}

SCALAR_FUN_ATTR float uitofp_i32_f32(uint32_t x) {
  return (float) x;
}

SCALAR_FUN_ATTR float uitofp_i64_f32(uint64_t x) {
  return (float) x;
}

#ifdef __OPENCL_VERSION__
SCALAR_FUN_ATTR float fabs32(float x) {
  return fabs(x);
}

SCALAR_FUN_ATTR float fmax32(float x, float y) {
  return fmax(x, y);
}

SCALAR_FUN_ATTR float fmin32(float x, float y) {
  return fmin(x, y);
}

SCALAR_FUN_ATTR float fpow32(float x, float y) {
  return pow(x, y);
}

#elif ISPC

SCALAR_FUN_ATTR float fabs32(float x) {
  return abs(x);
}

SCALAR_FUN_ATTR float fmax32(float x, float y) {
  return isnan(x) ? y : isnan(y) ? x : max(x, y);
}

SCALAR_FUN_ATTR float fmin32(float x, float y) {
  return isnan(x) ? y : isnan(y) ? x : min(x, y);
}

SCALAR_FUN_ATTR float fpow32(float a, float b) {
  float ret;
  foreach_active (i) {
      uniform float r = __stdlib_powf(extract(a, i), extract(b, i));
      ret = insert(ret, i, r);
  }
  return ret;
}

#else // Not OpenCL, but CUDA or plain C.

SCALAR_FUN_ATTR float fabs32(float x) {
  return fabsf(x);
}

SCALAR_FUN_ATTR float fmax32(float x, float y) {
  return fmaxf(x, y);
}

SCALAR_FUN_ATTR float fmin32(float x, float y) {
  return fminf(x, y);
}

SCALAR_FUN_ATTR float fpow32(float x, float y) {
  return powf(x, y);
}
#endif

SCALAR_FUN_ATTR bool futrts_isnan32(float x) {
  return isnan(x);
}

#if ISPC

SCALAR_FUN_ATTR bool futrts_isinf32(float x) {
  return !isnan(x) && isnan(x - x);
}

SCALAR_FUN_ATTR bool futrts_isfinite32(float x) {
  return !isnan(x) && !futrts_isinf32(x);
}

#else

SCALAR_FUN_ATTR bool futrts_isinf32(float x) {
  return isinf(x);
}

#endif

SCALAR_FUN_ATTR int8_t fptosi_f32_i8(float x) {
  if (futrts_isnan32(x) || futrts_isinf32(x)) {
    return 0;
  } else {
    return (int8_t) x;
  }
}

SCALAR_FUN_ATTR int16_t fptosi_f32_i16(float x) {
  if (futrts_isnan32(x) || futrts_isinf32(x)) {
    return 0;
  } else {
    return (int16_t) x;
  }
}

SCALAR_FUN_ATTR int32_t fptosi_f32_i32(float x) {
  if (futrts_isnan32(x) || futrts_isinf32(x)) {
    return 0;
  } else {
    return (int32_t) x;
  }
}

SCALAR_FUN_ATTR int64_t fptosi_f32_i64(float x) {
  if (futrts_isnan32(x) || futrts_isinf32(x)) {
    return 0;
  } else {
    return (int64_t) x;
  };
}

SCALAR_FUN_ATTR uint8_t fptoui_f32_i8(float x) {
  if (futrts_isnan32(x) || futrts_isinf32(x)) {
    return 0;
  } else {
    return (uint8_t) (int8_t) x;
  }
}

SCALAR_FUN_ATTR uint16_t fptoui_f32_i16(float x) {
  if (futrts_isnan32(x) || futrts_isinf32(x)) {
    return 0;
  } else {
    return (uint16_t) (int16_t) x;
  }
}

SCALAR_FUN_ATTR uint32_t fptoui_f32_i32(float x) {
  if (futrts_isnan32(x) || futrts_isinf32(x)) {
    return 0;
  } else {
    return (uint32_t) (int32_t) x;
  }
}

SCALAR_FUN_ATTR uint64_t fptoui_f32_i64(float x) {
  if (futrts_isnan32(x) || futrts_isinf32(x)) {
    return 0;
  } else {
    return (uint64_t) (int64_t) x;
  }
}

SCALAR_FUN_ATTR bool ftob_f32_bool(float x) {
  return x != 0;
}

SCALAR_FUN_ATTR float btof_bool_f32(bool x) {
  return x ? 1 : 0;
}

#ifdef __OPENCL_VERSION__
SCALAR_FUN_ATTR float futrts_log32(float x) {
  return log(x);
}

SCALAR_FUN_ATTR float futrts_log2_32(float x) {
  return log2(x);
}

SCALAR_FUN_ATTR float futrts_log10_32(float x) {
  return log10(x);
}

SCALAR_FUN_ATTR float futrts_log1p_32(float x) {
  return log1p(x);
}

SCALAR_FUN_ATTR float futrts_sqrt32(float x) {
  return sqrt(x);
}

SCALAR_FUN_ATTR float futrts_cbrt32(float x) {
  return cbrt(x);
}

SCALAR_FUN_ATTR float futrts_exp32(float x) {
  return exp(x);
}

SCALAR_FUN_ATTR float futrts_cos32(float x) {
  return cos(x);
}

SCALAR_FUN_ATTR float futrts_sin32(float x) {
  return sin(x);
}

SCALAR_FUN_ATTR float futrts_tan32(float x) {
  return tan(x);
}

SCALAR_FUN_ATTR float futrts_acos32(float x) {
  return acos(x);
}

SCALAR_FUN_ATTR float futrts_asin32(float x) {
  return asin(x);
}

SCALAR_FUN_ATTR float futrts_atan32(float x) {
  return atan(x);
}

SCALAR_FUN_ATTR float futrts_cosh32(float x) {
  return cosh(x);
}

SCALAR_FUN_ATTR float futrts_sinh32(float x) {
  return sinh(x);
}

SCALAR_FUN_ATTR float futrts_tanh32(float x) {
  return tanh(x);
}

SCALAR_FUN_ATTR float futrts_acosh32(float x) {
  return acosh(x);
}

SCALAR_FUN_ATTR float futrts_asinh32(float x) {
  return asinh(x);
}

SCALAR_FUN_ATTR float futrts_atanh32(float x) {
  return atanh(x);
}

SCALAR_FUN_ATTR float futrts_atan2_32(float x, float y) {
  return atan2(x, y);
}

SCALAR_FUN_ATTR float futrts_hypot32(float x, float y) {
  return hypot(x, y);
}

SCALAR_FUN_ATTR float futrts_gamma32(float x) {
  return tgamma(x);
}

SCALAR_FUN_ATTR float futrts_lgamma32(float x) {
  return lgamma(x);
}

SCALAR_FUN_ATTR float futrts_erf32(float x) {
  return erf(x);
}

SCALAR_FUN_ATTR float futrts_erfc32(float x) {
  return erfc(x);
}

SCALAR_FUN_ATTR float fmod32(float x, float y) {
  return fmod(x, y);
}

SCALAR_FUN_ATTR float futrts_round32(float x) {
  return rint(x);
}

SCALAR_FUN_ATTR float futrts_floor32(float x) {
  return floor(x);
}

SCALAR_FUN_ATTR float futrts_ceil32(float x) {
  return ceil(x);
}

SCALAR_FUN_ATTR float futrts_nextafter32(float x, float y) {
  return nextafter(x, y);
}

SCALAR_FUN_ATTR float futrts_lerp32(float v0, float v1, float t) {
  return mix(v0, v1, t);
}

SCALAR_FUN_ATTR float futrts_ldexp32(float x, int32_t y) {
  return ldexp(x, y);
}

SCALAR_FUN_ATTR float futrts_copysign32(float x, float y) {
  return copysign(x, y);
}

SCALAR_FUN_ATTR float futrts_mad32(float a, float b, float c) {
  return mad(a, b, c);
}

SCALAR_FUN_ATTR float futrts_fma32(float a, float b, float c) {
  return fma(a, b, c);
}

#elif ISPC

SCALAR_FUN_ATTR float futrts_log32(float x) {
  return futrts_isfinite32(x) || (futrts_isinf32(x) && x < 0)? log(x) : x;
}

SCALAR_FUN_ATTR float futrts_log2_32(float x) {
  return futrts_log32(x) / log(2.0f);
}

SCALAR_FUN_ATTR float futrts_log10_32(float x) {
  return futrts_log32(x) / log(10.0f);
}

SCALAR_FUN_ATTR float futrts_log1p_32(float x) {
  if(x == -1.0f || (futrts_isinf32(x) && x > 0.0f)) return x / 0.0f;
  float y = 1.0f + x;
  float z = y - 1.0f;
  return log(y) - (z-x)/y;
}

SCALAR_FUN_ATTR float futrts_sqrt32(float x) {
  return sqrt(x);
}

extern "C" unmasked uniform float cbrtf(uniform float);
SCALAR_FUN_ATTR float futrts_cbrt32(float x) {
  float res;
  foreach_active (i) {
    uniform float r = cbrtf(extract(x, i));
    res = insert(res, i, r);
  }
  return res;
}

SCALAR_FUN_ATTR float futrts_exp32(float x) {
  return exp(x);
}

SCALAR_FUN_ATTR float futrts_cos32(float x) {
  return cos(x);
}

SCALAR_FUN_ATTR float futrts_sin32(float x) {
  return sin(x);
}

SCALAR_FUN_ATTR float futrts_tan32(float x) {
  return tan(x);
}

SCALAR_FUN_ATTR float futrts_acos32(float x) {
  return acos(x);
}

SCALAR_FUN_ATTR float futrts_asin32(float x) {
  return asin(x);
}

SCALAR_FUN_ATTR float futrts_atan32(float x) {
  return atan(x);
}

SCALAR_FUN_ATTR float futrts_cosh32(float x) {
  return (exp(x)+exp(-x)) / 2.0f;
}

SCALAR_FUN_ATTR float futrts_sinh32(float x) {
  return (exp(x)-exp(-x)) / 2.0f;
}

SCALAR_FUN_ATTR float futrts_tanh32(float x) {
  return futrts_sinh32(x)/futrts_cosh32(x);
}

SCALAR_FUN_ATTR float futrts_acosh32(float x) {
  float f = x+sqrt(x*x-1);
  if(futrts_isfinite32(f)) return log(f);
  return f;
}

SCALAR_FUN_ATTR float futrts_asinh32(float x) {
  float f = x+sqrt(x*x+1);
  if(futrts_isfinite32(f)) return log(f);
  return f;

}

SCALAR_FUN_ATTR float futrts_atanh32(float x) {
  float f = (1+x)/(1-x);
  if(futrts_isfinite32(f)) return log(f)/2.0f;
  return f;

}

SCALAR_FUN_ATTR float futrts_atan2_32(float x, float y) {
  return (x == 0.0f && y == 0.0f) ? 0.0f : atan2(x, y);
}

SCALAR_FUN_ATTR float futrts_hypot32(float x, float y) {
  if (futrts_isfinite32(x) && futrts_isfinite32(y)) {
    x = abs(x);
    y = abs(y);
    float a;
    float b;
    if (x >= y){
        a = x;
        b = y;
    } else {
        a = y;
        b = x;
    }
    if(b == 0){
      return a;
    }

    int e;
    float an;
    float bn;
    an = frexp (a, &e);
    bn = ldexp (b, - e);
    float cn;
    cn = sqrt (an * an + bn * bn);
    return ldexp (cn, e);
  } else {
    if (futrts_isinf32(x) || futrts_isinf32(y)) return INFINITY;
    else return x + y;
  }

}

extern "C" unmasked uniform float tgammaf(uniform float x);
SCALAR_FUN_ATTR float futrts_gamma32(float x) {
  float res;
  foreach_active (i) {
    uniform float r = tgammaf(extract(x, i));
    res = insert(res, i, r);
  }
  return res;
}

extern "C" unmasked uniform float lgammaf(uniform float x);
SCALAR_FUN_ATTR float futrts_lgamma32(float x) {
  float res;
  foreach_active (i) {
    uniform float r = lgammaf(extract(x, i));
    res = insert(res, i, r);
  }
  return res;
}

extern "C" unmasked uniform float erff(uniform float x);
SCALAR_FUN_ATTR float futrts_erf32(float x) {
  float res;
  foreach_active (i) {
    uniform float r = erff(extract(x, i));
    res = insert(res, i, r);
  }
  return res;
}

extern "C" unmasked uniform float erfcf(uniform float x);
SCALAR_FUN_ATTR float futrts_erfc32(float x) {
  float res;
  foreach_active (i) {
    uniform float r = erfcf(extract(x, i));
    res = insert(res, i, r);
  }
  return res;
}

SCALAR_FUN_ATTR float fmod32(float x, float y) {
  return x - y * trunc(x/y);
}

SCALAR_FUN_ATTR float futrts_round32(float x) {
  return round(x);
}

SCALAR_FUN_ATTR float futrts_floor32(float x) {
  return floor(x);
}

SCALAR_FUN_ATTR float futrts_ceil32(float x) {
  return ceil(x);
}

extern "C" unmasked uniform float nextafterf(uniform float x, uniform float y);
SCALAR_FUN_ATTR float futrts_nextafter32(float x, float y) {
  float res;
  foreach_active (i) {
    uniform float r = nextafterf(extract(x, i), extract(y, i));
    res = insert(res, i, r);
  }
  return res;
}

SCALAR_FUN_ATTR float futrts_lerp32(float v0, float v1, float t) {
  return v0 + (v1 - v0) * t;
}

SCALAR_FUN_ATTR float futrts_ldexp32(float x, int32_t y) {
  return x * pow((double)2.0, (double)y);
}

SCALAR_FUN_ATTR float futrts_copysign32(float x, float y) {
  int32_t xb = futrts_to_bits32(x);
  int32_t yb = futrts_to_bits32(y);
  return futrts_from_bits32((xb & ~(1<<31)) | (yb & (1<<31)));
}

SCALAR_FUN_ATTR float futrts_mad32(float a, float b, float c) {
  return a * b + c;
}

SCALAR_FUN_ATTR float futrts_fma32(float a, float b, float c) {
  return a * b + c;
}

#else // Not OpenCL or ISPC, but CUDA or plain C.

SCALAR_FUN_ATTR float futrts_log32(float x) {
  return logf(x);
}

SCALAR_FUN_ATTR float futrts_log2_32(float x) {
  return log2f(x);
}

SCALAR_FUN_ATTR float futrts_log10_32(float x) {
  return log10f(x);
}

SCALAR_FUN_ATTR float futrts_log1p_32(float x) {
  return log1pf(x);
}

SCALAR_FUN_ATTR float futrts_sqrt32(float x) {
  return sqrtf(x);
}

SCALAR_FUN_ATTR float futrts_cbrt32(float x) {
  return cbrtf(x);
}

SCALAR_FUN_ATTR float futrts_exp32(float x) {
  return expf(x);
}

SCALAR_FUN_ATTR float futrts_cos32(float x) {
  return cosf(x);
}

SCALAR_FUN_ATTR float futrts_sin32(float x) {
  return sinf(x);
}

SCALAR_FUN_ATTR float futrts_tan32(float x) {
  return tanf(x);
}

SCALAR_FUN_ATTR float futrts_acos32(float x) {
  return acosf(x);
}

SCALAR_FUN_ATTR float futrts_asin32(float x) {
  return asinf(x);
}

SCALAR_FUN_ATTR float futrts_atan32(float x) {
  return atanf(x);
}

SCALAR_FUN_ATTR float futrts_cosh32(float x) {
  return coshf(x);
}

SCALAR_FUN_ATTR float futrts_sinh32(float x) {
  return sinhf(x);
}

SCALAR_FUN_ATTR float futrts_tanh32(float x) {
  return tanhf(x);
}

SCALAR_FUN_ATTR float futrts_acosh32(float x) {
  return acoshf(x);
}

SCALAR_FUN_ATTR float futrts_asinh32(float x) {
  return asinhf(x);
}

SCALAR_FUN_ATTR float futrts_atanh32(float x) {
  return atanhf(x);
}

SCALAR_FUN_ATTR float futrts_atan2_32(float x, float y) {
  return atan2f(x, y);
}

SCALAR_FUN_ATTR float futrts_hypot32(float x, float y) {
  return hypotf(x, y);
}

SCALAR_FUN_ATTR float futrts_gamma32(float x) {
  return tgammaf(x);
}

SCALAR_FUN_ATTR float futrts_lgamma32(float x) {
  return lgammaf(x);
}

SCALAR_FUN_ATTR float futrts_erf32(float x) {
  return erff(x);
}

SCALAR_FUN_ATTR float futrts_erfc32(float x) {
  return erfcf(x);
}

SCALAR_FUN_ATTR float fmod32(float x, float y) {
  return fmodf(x, y);
}

SCALAR_FUN_ATTR float futrts_round32(float x) {
  return rintf(x);
}

SCALAR_FUN_ATTR float futrts_floor32(float x) {
  return floorf(x);
}

SCALAR_FUN_ATTR float futrts_ceil32(float x) {
  return ceilf(x);
}

SCALAR_FUN_ATTR float futrts_nextafter32(float x, float y) {
  return nextafterf(x, y);
}

SCALAR_FUN_ATTR float futrts_lerp32(float v0, float v1, float t) {
  return v0 + (v1 - v0) * t;
}

SCALAR_FUN_ATTR float futrts_ldexp32(float x, int32_t y) {
  return ldexpf(x, y);
}

SCALAR_FUN_ATTR float futrts_copysign32(float x, float y) {
  return copysignf(x, y);
}

SCALAR_FUN_ATTR float futrts_mad32(float a, float b, float c) {
  return a * b + c;
}

SCALAR_FUN_ATTR float futrts_fma32(float a, float b, float c) {
  return fmaf(a, b, c);
}
#endif

#if ISPC
SCALAR_FUN_ATTR int32_t futrts_to_bits32(float x) {
  return intbits(x);
}

SCALAR_FUN_ATTR float futrts_from_bits32(int32_t x) {
  return floatbits(x);
}
#else
SCALAR_FUN_ATTR int32_t futrts_to_bits32(float x) {
  union {
    float f;
    int32_t t;
  } p;

  p.f = x;
  return p.t;
}

SCALAR_FUN_ATTR float futrts_from_bits32(int32_t x) {
  union {
    int32_t f;
    float t;
  } p;

  p.f = x;
  return p.t;
}
#endif

SCALAR_FUN_ATTR float fsignum32(float x) {
  return futrts_isnan32(x) ? x : (x > 0 ? 1 : 0) - (x < 0 ? 1 : 0);
}

#ifdef FUTHARK_F64_ENABLED

SCALAR_FUN_ATTR double futrts_from_bits64(int64_t x);
SCALAR_FUN_ATTR int64_t futrts_to_bits64(double x);

#if ISPC
SCALAR_FUN_ATTR bool futrts_isinf64(float x) {
  return !isnan(x) && isnan(x - x);
}

SCALAR_FUN_ATTR bool futrts_isfinite64(float x) {
  return !isnan(x) && !futrts_isinf64(x);
}

SCALAR_FUN_ATTR double fdiv64(double x, double y) {
  return x / y;
}

SCALAR_FUN_ATTR double fadd64(double x, double y) {
  return x + y;
}

SCALAR_FUN_ATTR double fsub64(double x, double y) {
  return x - y;
}

SCALAR_FUN_ATTR double fmul64(double x, double y) {
  return x * y;
}

SCALAR_FUN_ATTR bool cmplt64(double x, double y) {
  return x < y;
}

SCALAR_FUN_ATTR bool cmple64(double x, double y) {
  return x <= y;
}

SCALAR_FUN_ATTR double sitofp_i8_f64(int8_t x) {
  return (double) x;
}

SCALAR_FUN_ATTR double sitofp_i16_f64(int16_t x) {
  return (double) x;
}

SCALAR_FUN_ATTR double sitofp_i32_f64(int32_t x) {
  return (double) x;
}

SCALAR_FUN_ATTR double sitofp_i64_f64(int64_t x) {
  return (double) x;
}

SCALAR_FUN_ATTR double uitofp_i8_f64(uint8_t x) {
  return (double) x;
}

SCALAR_FUN_ATTR double uitofp_i16_f64(uint16_t x) {
  return (double) x;
}

SCALAR_FUN_ATTR double uitofp_i32_f64(uint32_t x) {
  return (double) x;
}

SCALAR_FUN_ATTR double uitofp_i64_f64(uint64_t x) {
  return (double) x;
}

SCALAR_FUN_ATTR double fabs64(double x) {
  return abs(x);
}

SCALAR_FUN_ATTR double fmax64(double x, double y) {
  return isnan(x) ? y : isnan(y) ? x : max(x, y);
}

SCALAR_FUN_ATTR double fmin64(double x, double y) {
  return isnan(x) ? y : isnan(y) ? x : min(x, y);
}

SCALAR_FUN_ATTR double fpow64(double a, double b) {
  float ret;
  foreach_active (i) {
      uniform float r = __stdlib_powf(extract(a, i), extract(b, i));
      ret = insert(ret, i, r);
  }
  return ret;
}

SCALAR_FUN_ATTR double futrts_log64(double x) {
  return futrts_isfinite64(x) || (futrts_isinf64(x) && x < 0)? log(x) : x;
}

SCALAR_FUN_ATTR double futrts_log2_64(double x) {
  return futrts_log64(x)/log(2.0d);
}

SCALAR_FUN_ATTR double futrts_log10_64(double x) {
  return futrts_log64(x)/log(10.0d);
}

SCALAR_FUN_ATTR double futrts_log1p_64(double x) {
  if(x == -1.0d || (futrts_isinf64(x) && x > 0.0d)) return x / 0.0d;
  double y = 1.0d + x;
  double z = y - 1.0d;
  return log(y) - (z-x)/y;
}

SCALAR_FUN_ATTR double futrts_sqrt64(double x) {
  return sqrt(x);
}

extern "C" unmasked uniform double cbrt(uniform double);
SCALAR_FUN_ATTR double futrts_cbrt64(double x) {
  double res;
  foreach_active (i) {
    uniform double r = cbrtf(extract(x, i));
    res = insert(res, i, r);
  }
  return res;
}

SCALAR_FUN_ATTR double futrts_exp64(double x) {
  return exp(x);
}

SCALAR_FUN_ATTR double futrts_cos64(double x) {
  return cos(x);
}

SCALAR_FUN_ATTR double futrts_sin64(double x) {
  return sin(x);
}

SCALAR_FUN_ATTR double futrts_tan64(double x) {
  return tan(x);
}

SCALAR_FUN_ATTR double futrts_acos64(double x) {
  return acos(x);
}

SCALAR_FUN_ATTR double futrts_asin64(double x) {
  return asin(x);
}

SCALAR_FUN_ATTR double futrts_atan64(double x) {
  return atan(x);
}

SCALAR_FUN_ATTR double futrts_cosh64(double x) {
  return (exp(x)+exp(-x)) / 2.0d;
}

SCALAR_FUN_ATTR double futrts_sinh64(double x) {
  return (exp(x)-exp(-x)) / 2.0d;
}

SCALAR_FUN_ATTR double futrts_tanh64(double x) {
  return futrts_sinh64(x)/futrts_cosh64(x);
}

SCALAR_FUN_ATTR double futrts_acosh64(double x) {
  double f = x+sqrt(x*x-1.0d);
  if(futrts_isfinite64(f)) return log(f);
  return f;
}

SCALAR_FUN_ATTR double futrts_asinh64(double x) {
  double f = x+sqrt(x*x+1.0d);
  if(futrts_isfinite64(f)) return log(f);
  return f;
}

SCALAR_FUN_ATTR double futrts_atanh64(double x) {
  double f = (1.0d+x)/(1.0d-x);
  if(futrts_isfinite64(f)) return log(f)/2.0d;
  return f;

}

SCALAR_FUN_ATTR double futrts_atan2_64(double x, double y) {
  return atan2(x, y);
}

extern "C" unmasked uniform double hypot(uniform double x, uniform double y);
SCALAR_FUN_ATTR double futrts_hypot64(double x, double y) {
  double res;
  foreach_active (i) {
    uniform double r = hypot(extract(x, i), extract(y, i));
    res = insert(res, i, r);
  }
  return res;
}

extern "C" unmasked uniform double tgamma(uniform double x);
SCALAR_FUN_ATTR double futrts_gamma64(double x) {
  double res;
  foreach_active (i) {
    uniform double r = tgamma(extract(x, i));
    res = insert(res, i, r);
  }
  return res;
}

extern "C" unmasked uniform double lgamma(uniform double x);
SCALAR_FUN_ATTR double futrts_lgamma64(double x) {
  double res;
  foreach_active (i) {
    uniform double r = lgamma(extract(x, i));
    res = insert(res, i, r);
  }
  return res;
}

extern "C" unmasked uniform double erf(uniform double x);
SCALAR_FUN_ATTR double futrts_erf64(double x) {
  double res;
  foreach_active (i) {
    uniform double r = erf(extract(x, i));
    res = insert(res, i, r);
  }
  return res;
}

extern "C" unmasked uniform double erfc(uniform double x);
SCALAR_FUN_ATTR double futrts_erfc64(double x) {
  double res;
  foreach_active (i) {
    uniform double r = erfc(extract(x, i));
    res = insert(res, i, r);
  }
  return res;
}

SCALAR_FUN_ATTR double futrts_fma64(double a, double b, double c) {
  return a * b + c;
}

SCALAR_FUN_ATTR double futrts_round64(double x) {
  return round(x);
}

SCALAR_FUN_ATTR double futrts_ceil64(double x) {
  return ceil(x);
}

extern "C" unmasked uniform double nextafter(uniform float x, uniform double y);
SCALAR_FUN_ATTR float futrts_nextafter64(double x, double y) {
  double res;
  foreach_active (i) {
    uniform double r = nextafter(extract(x, i), extract(y, i));
    res = insert(res, i, r);
  }
  return res;
}

SCALAR_FUN_ATTR double futrts_floor64(double x) {
  return floor(x);
}

SCALAR_FUN_ATTR bool futrts_isnan64(double x) {
  return isnan(x);
}

SCALAR_FUN_ATTR int8_t fptosi_f64_i8(double x) {
  if (futrts_isnan64(x) || futrts_isinf64(x)) {
    return 0;
  } else {
    return (int8_t) x;
  }
}

SCALAR_FUN_ATTR int16_t fptosi_f64_i16(double x) {
  if (futrts_isnan64(x) || futrts_isinf64(x)) {
    return 0;
  } else {
    return (int16_t) x;
  }
}

SCALAR_FUN_ATTR int32_t fptosi_f64_i32(double x) {
  if (futrts_isnan64(x) || futrts_isinf64(x)) {
    return 0;
  } else {
    return (int32_t) x;
  }
}

SCALAR_FUN_ATTR int64_t fptosi_f64_i64(double x) {
  if (futrts_isnan64(x) || futrts_isinf64(x)) {
    return 0;
  } else {
    return (int64_t) x;
  }
}

SCALAR_FUN_ATTR uint8_t fptoui_f64_i8(double x) {
  if (futrts_isnan64(x) || futrts_isinf64(x)) {
    return 0;
  } else {
    return (uint8_t) (int8_t) x;
  }
}

SCALAR_FUN_ATTR uint16_t fptoui_f64_i16(double x) {
  if (futrts_isnan64(x) || futrts_isinf64(x)) {
    return 0;
  } else {
    return (uint16_t) (int16_t) x;
  }
}

SCALAR_FUN_ATTR uint32_t fptoui_f64_i32(double x) {
  if (futrts_isnan64(x) || futrts_isinf64(x)) {
    return 0;
  } else {
    return (uint32_t) (int32_t) x;
  }
}

SCALAR_FUN_ATTR uint64_t fptoui_f64_i64(double x) {
  if (futrts_isnan64(x) || futrts_isinf64(x)) {
    return 0;
  } else {
    return (uint64_t) (int64_t) x;
  }
}

SCALAR_FUN_ATTR bool ftob_f64_bool(double x) {
  return x != 0.0;
}

SCALAR_FUN_ATTR double btof_bool_f64(bool x) {
  return x ? 1.0 : 0.0;
}

SCALAR_FUN_ATTR int64_t futrts_to_bits64(double x) {
  int64_t res;
  foreach_active (i) {
    uniform double tmp = extract(x, i);
    uniform int64_t r = *((uniform int64_t* uniform)&tmp);
    res = insert(res, i, r);
  }
  return res;
}

SCALAR_FUN_ATTR double futrts_from_bits64(int64_t x) {
  double res;
  foreach_active (i) {
    uniform int64_t tmp = extract(x, i);
    uniform double r = *((uniform double* uniform)&tmp);
    res = insert(res, i, r);
  }
  return res;
}

SCALAR_FUN_ATTR double fmod64(double x, double y) {
  return x - y * trunc(x/y);
}

SCALAR_FUN_ATTR double fsignum64(double x) {
  return futrts_isnan64(x) ? x : (x > 0 ? 1.0d : 0.0d) - (x < 0 ? 1.0d : 0.0d);
}

SCALAR_FUN_ATTR double futrts_lerp64(double v0, double v1, double t) {
  return v0 + (v1 - v0) * t;
}

SCALAR_FUN_ATTR double futrts_ldexp64(double x, int32_t y) {
  return x * pow((double)2.0, (double)y);
}

SCALAR_FUN_ATTR double futrts_copysign64(double x, double y) {
  int64_t xb = futrts_to_bits64(x);
  int64_t yb = futrts_to_bits64(y);
  return futrts_from_bits64((xb & ~(((int64_t)1)<<63)) | (yb & (((int64_t)1)<<63)));
}

SCALAR_FUN_ATTR double futrts_mad64(double a, double b, double c) {
  return a * b + c;
}

SCALAR_FUN_ATTR float fpconv_f32_f32(float x) {
  return (float) x;
}

SCALAR_FUN_ATTR double fpconv_f32_f64(float x) {
  return (double) x;
}

SCALAR_FUN_ATTR float fpconv_f64_f32(double x) {
  return (float) x;
}

SCALAR_FUN_ATTR double fpconv_f64_f64(double x) {
  return (double) x;
}

#else

SCALAR_FUN_ATTR double fdiv64(double x, double y) {
  return x / y;
}

SCALAR_FUN_ATTR double fadd64(double x, double y) {
  return x + y;
}

SCALAR_FUN_ATTR double fsub64(double x, double y) {
  return x - y;
}

SCALAR_FUN_ATTR double fmul64(double x, double y) {
  return x * y;
}

SCALAR_FUN_ATTR bool cmplt64(double x, double y) {
  return x < y;
}

SCALAR_FUN_ATTR bool cmple64(double x, double y) {
  return x <= y;
}

SCALAR_FUN_ATTR double sitofp_i8_f64(int8_t x) {
  return (double) x;
}

SCALAR_FUN_ATTR double sitofp_i16_f64(int16_t x) {
  return (double) x;
}

SCALAR_FUN_ATTR double sitofp_i32_f64(int32_t x) {
  return (double) x;
}

SCALAR_FUN_ATTR double sitofp_i64_f64(int64_t x) {
  return (double) x;
}

SCALAR_FUN_ATTR double uitofp_i8_f64(uint8_t x) {
  return (double) x;
}

SCALAR_FUN_ATTR double uitofp_i16_f64(uint16_t x) {
  return (double) x;
}

SCALAR_FUN_ATTR double uitofp_i32_f64(uint32_t x) {
  return (double) x;
}

SCALAR_FUN_ATTR double uitofp_i64_f64(uint64_t x) {
  return (double) x;
}

SCALAR_FUN_ATTR double fabs64(double x) {
  return fabs(x);
}

SCALAR_FUN_ATTR double fmax64(double x, double y) {
  return fmax(x, y);
}

SCALAR_FUN_ATTR double fmin64(double x, double y) {
  return fmin(x, y);
}

SCALAR_FUN_ATTR double fpow64(double x, double y) {
  return pow(x, y);
}

SCALAR_FUN_ATTR double futrts_log64(double x) {
  return log(x);
}

SCALAR_FUN_ATTR double futrts_log2_64(double x) {
  return log2(x);
}

SCALAR_FUN_ATTR double futrts_log10_64(double x) {
  return log10(x);
}

SCALAR_FUN_ATTR double futrts_log1p_64(double x) {
  return log1p(x);
}

SCALAR_FUN_ATTR double futrts_sqrt64(double x) {
  return sqrt(x);
}

SCALAR_FUN_ATTR double futrts_cbrt64(double x) {
  return cbrt(x);
}

SCALAR_FUN_ATTR double futrts_exp64(double x) {
  return exp(x);
}

SCALAR_FUN_ATTR double futrts_cos64(double x) {
  return cos(x);
}

SCALAR_FUN_ATTR double futrts_sin64(double x) {
  return sin(x);
}

SCALAR_FUN_ATTR double futrts_tan64(double x) {
  return tan(x);
}

SCALAR_FUN_ATTR double futrts_acos64(double x) {
  return acos(x);
}

SCALAR_FUN_ATTR double futrts_asin64(double x) {
  return asin(x);
}

SCALAR_FUN_ATTR double futrts_atan64(double x) {
  return atan(x);
}

SCALAR_FUN_ATTR double futrts_cosh64(double x) {
  return cosh(x);
}

SCALAR_FUN_ATTR double futrts_sinh64(double x) {
  return sinh(x);
}

SCALAR_FUN_ATTR double futrts_tanh64(double x) {
  return tanh(x);
}

SCALAR_FUN_ATTR double futrts_acosh64(double x) {
  return acosh(x);
}

SCALAR_FUN_ATTR double futrts_asinh64(double x) {
  return asinh(x);
}

SCALAR_FUN_ATTR double futrts_atanh64(double x) {
  return atanh(x);
}

SCALAR_FUN_ATTR double futrts_atan2_64(double x, double y) {
  return atan2(x, y);
}

SCALAR_FUN_ATTR double futrts_hypot64(double x, double y) {
  return hypot(x, y);
}

SCALAR_FUN_ATTR double futrts_gamma64(double x) {
  return tgamma(x);
}

SCALAR_FUN_ATTR double futrts_lgamma64(double x) {
  return lgamma(x);
}

SCALAR_FUN_ATTR double futrts_erf64(double x) {
  return erf(x);
}

SCALAR_FUN_ATTR double futrts_erfc64(double x) {
  return erfc(x);
}

SCALAR_FUN_ATTR double futrts_fma64(double a, double b, double c) {
  return fma(a, b, c);
}

SCALAR_FUN_ATTR double futrts_round64(double x) {
  return rint(x);
}

SCALAR_FUN_ATTR double futrts_ceil64(double x) {
  return ceil(x);
}

SCALAR_FUN_ATTR float futrts_nextafter64(float x, float y) {
  return nextafter(x, y);
}

SCALAR_FUN_ATTR double futrts_floor64(double x) {
  return floor(x);
}

SCALAR_FUN_ATTR bool futrts_isnan64(double x) {
  return isnan(x);
}

SCALAR_FUN_ATTR bool futrts_isinf64(double x) {
  return isinf(x);
}

SCALAR_FUN_ATTR int8_t fptosi_f64_i8(double x) {
  if (futrts_isnan64(x) || futrts_isinf64(x)) {
    return 0;
  } else {
    return (int8_t) x;
  }
}

SCALAR_FUN_ATTR int16_t fptosi_f64_i16(double x) {
  if (futrts_isnan64(x) || futrts_isinf64(x)) {
    return 0;
  } else {
    return (int16_t) x;
  }
}

SCALAR_FUN_ATTR int32_t fptosi_f64_i32(double x) {
  if (futrts_isnan64(x) || futrts_isinf64(x)) {
    return 0;
  } else {
    return (int32_t) x;
  }
}

SCALAR_FUN_ATTR int64_t fptosi_f64_i64(double x) {
  if (futrts_isnan64(x) || futrts_isinf64(x)) {
    return 0;
  } else {
    return (int64_t) x;
  }
}

SCALAR_FUN_ATTR uint8_t fptoui_f64_i8(double x) {
  if (futrts_isnan64(x) || futrts_isinf64(x)) {
    return 0;
  } else {
    return (uint8_t) (int8_t) x;
  }
}

SCALAR_FUN_ATTR uint16_t fptoui_f64_i16(double x) {
  if (futrts_isnan64(x) || futrts_isinf64(x)) {
    return 0;
  } else {
    return (uint16_t) (int16_t) x;
  }
}

SCALAR_FUN_ATTR uint32_t fptoui_f64_i32(double x) {
  if (futrts_isnan64(x) || futrts_isinf64(x)) {
    return 0;
  } else {
    return (uint32_t) (int32_t) x;
  }
}

SCALAR_FUN_ATTR uint64_t fptoui_f64_i64(double x) {
  if (futrts_isnan64(x) || futrts_isinf64(x)) {
    return 0;
  } else {
    return (uint64_t) (int64_t) x;
  }
}

SCALAR_FUN_ATTR bool ftob_f64_bool(double x) {
  return x != 0;
}

SCALAR_FUN_ATTR double btof_bool_f64(bool x) {
  return x ? 1 : 0;
}

SCALAR_FUN_ATTR int64_t futrts_to_bits64(double x) {
  union {
    double f;
    int64_t t;
  } p;

  p.f = x;
  return p.t;
}

SCALAR_FUN_ATTR double futrts_from_bits64(int64_t x) {
  union {
    int64_t f;
    double t;
  } p;

  p.f = x;
  return p.t;
}

SCALAR_FUN_ATTR double fmod64(double x, double y) {
  return fmod(x, y);
}

SCALAR_FUN_ATTR double fsignum64(double x) {
  return futrts_isnan64(x) ? x : (x > 0) - (x < 0);
}

SCALAR_FUN_ATTR double futrts_lerp64(double v0, double v1, double t) {
#ifdef __OPENCL_VERSION__
  return mix(v0, v1, t);
#else
  return v0 + (v1 - v0) * t;
#endif
}

SCALAR_FUN_ATTR double futrts_ldexp64(double x, int32_t y) {
  return ldexp(x, y);
}

SCALAR_FUN_ATTR float futrts_copysign64(double x, double y) {
  return copysign(x, y);
}

SCALAR_FUN_ATTR double futrts_mad64(double a, double b, double c) {
#ifdef __OPENCL_VERSION__
  return mad(a, b, c);
#else
  return a * b + c;
#endif
}

SCALAR_FUN_ATTR float fpconv_f32_f32(float x) {
  return (float) x;
}

SCALAR_FUN_ATTR double fpconv_f32_f64(float x) {
  return (double) x;
}

SCALAR_FUN_ATTR float fpconv_f64_f32(double x) {
  return (float) x;
}

SCALAR_FUN_ATTR double fpconv_f64_f64(double x) {
  return (double) x;
}

#endif

#endif

// End of scalar.h.
// Start of scalar_f16.h.

// Half-precision is emulated if needed (e.g. in straight C) with the
// native type used if possible.  The emulation works by typedef'ing
// 'float' to 'f16', and then implementing all operations on single
// precision.  To cut down on duplication, we use the same code for
// those Futhark functions that require just operators or casts.  The
// in-memory representation for arrays will still be 16 bits even
// under emulation, so the compiler will have to be careful when
// generating reads or writes.

#if !defined(cl_khr_fp16) && !(defined(__CUDA_ARCH__) && __CUDA_ARCH__ >= 600) && !(defined(ISPC))
#define EMULATE_F16
#endif

#if !defined(EMULATE_F16) && defined(__OPENCL_VERSION__)
#pragma OPENCL EXTENSION cl_khr_fp16 : enable
#endif

#ifdef EMULATE_F16

// Note that the half-precision storage format is still 16 bits - the
// compiler will have to be real careful!
typedef float f16;

#elif ISPC
typedef float16 f16;

#else

#ifdef __CUDA_ARCH__
#include <cuda_fp16.h>
#endif

typedef half f16;

#endif

// Some of these functions convert to single precision because half
// precision versions are not available.

SCALAR_FUN_ATTR f16 fadd16(f16 x, f16 y) {
  return x + y;
}

SCALAR_FUN_ATTR f16 fsub16(f16 x, f16 y) {
  return x - y;
}

SCALAR_FUN_ATTR f16 fmul16(f16 x, f16 y) {
  return x * y;
}

SCALAR_FUN_ATTR bool cmplt16(f16 x, f16 y) {
  return x < y;
}

SCALAR_FUN_ATTR bool cmple16(f16 x, f16 y) {
  return x <= y;
}

SCALAR_FUN_ATTR f16 sitofp_i8_f16(int8_t x) {
  return (f16) x;
}

SCALAR_FUN_ATTR f16 sitofp_i16_f16(int16_t x) {
  return (f16) x;
}

SCALAR_FUN_ATTR f16 sitofp_i32_f16(int32_t x) {
  return (f16) x;
}

SCALAR_FUN_ATTR f16 sitofp_i64_f16(int64_t x) {
  return (f16) x;
}

SCALAR_FUN_ATTR f16 uitofp_i8_f16(uint8_t x) {
  return (f16) x;
}

SCALAR_FUN_ATTR f16 uitofp_i16_f16(uint16_t x) {
  return (f16) x;
}

SCALAR_FUN_ATTR f16 uitofp_i32_f16(uint32_t x) {
  return (f16) x;
}

SCALAR_FUN_ATTR f16 uitofp_i64_f16(uint64_t x) {
  return (f16) x;
}

SCALAR_FUN_ATTR int8_t fptosi_f16_i8(f16 x) {
  return (int8_t) (float) x;
}

SCALAR_FUN_ATTR int16_t fptosi_f16_i16(f16 x) {
  return (int16_t) x;
}

SCALAR_FUN_ATTR int32_t fptosi_f16_i32(f16 x) {
  return (int32_t) x;
}

SCALAR_FUN_ATTR int64_t fptosi_f16_i64(f16 x) {
  return (int64_t) x;
}

SCALAR_FUN_ATTR uint8_t fptoui_f16_i8(f16 x) {
  return (uint8_t) (float) x;
}

SCALAR_FUN_ATTR uint16_t fptoui_f16_i16(f16 x) {
  return (uint16_t) x;
}

SCALAR_FUN_ATTR uint32_t fptoui_f16_i32(f16 x) {
  return (uint32_t) x;
}

SCALAR_FUN_ATTR uint64_t fptoui_f16_i64(f16 x) {
  return (uint64_t) x;
}

SCALAR_FUN_ATTR bool ftob_f16_bool(f16 x) {
  return x != (f16)0;
}

SCALAR_FUN_ATTR f16 btof_bool_f16(bool x) {
  return x ? 1 : 0;
}

#ifndef EMULATE_F16
SCALAR_FUN_ATTR bool futrts_isnan16(f16 x) {
  return isnan((float)x);
}

#ifdef __OPENCL_VERSION__

SCALAR_FUN_ATTR f16 fabs16(f16 x) {
  return fabs(x);
}

SCALAR_FUN_ATTR f16 fmax16(f16 x, f16 y) {
  return fmax(x, y);
}

SCALAR_FUN_ATTR f16 fmin16(f16 x, f16 y) {
  return fmin(x, y);
}

SCALAR_FUN_ATTR f16 fpow16(f16 x, f16 y) {
  return pow(x, y);
}

#elif ISPC
SCALAR_FUN_ATTR f16 fabs16(f16 x) {
  return abs(x);
}

SCALAR_FUN_ATTR f16 fmax16(f16 x, f16 y) {
  return futrts_isnan16(x) ? y : futrts_isnan16(y) ? x : max(x, y);
}

SCALAR_FUN_ATTR f16 fmin16(f16 x, f16 y) {
  return futrts_isnan16(x) ? y : futrts_isnan16(y) ? x : min(x, y);
}

SCALAR_FUN_ATTR f16 fpow16(f16 x, f16 y) {
  return pow(x, y);
}

#else // Assuming CUDA.

SCALAR_FUN_ATTR f16 fabs16(f16 x) {
  return fabsf(x);
}

SCALAR_FUN_ATTR f16 fmax16(f16 x, f16 y) {
  return fmaxf(x, y);
}

SCALAR_FUN_ATTR f16 fmin16(f16 x, f16 y) {
  return fminf(x, y);
}

SCALAR_FUN_ATTR f16 fpow16(f16 x, f16 y) {
  return powf(x, y);
}
#endif

#if ISPC
SCALAR_FUN_ATTR bool futrts_isinf16(float x) {
  return !futrts_isnan16(x) && futrts_isnan16(x - x);
}
SCALAR_FUN_ATTR bool futrts_isfinite16(float x) {
  return !futrts_isnan16(x) && !futrts_isinf16(x);
}

#else

SCALAR_FUN_ATTR bool futrts_isinf16(f16 x) {
  return isinf((float)x);
}
#endif

#ifdef __OPENCL_VERSION__
SCALAR_FUN_ATTR f16 futrts_log16(f16 x) {
  return log(x);
}

SCALAR_FUN_ATTR f16 futrts_log2_16(f16 x) {
  return log2(x);
}

SCALAR_FUN_ATTR f16 futrts_log10_16(f16 x) {
  return log10(x);
}

SCALAR_FUN_ATTR f16 futrts_log1p_16(f16 x) {
  return log1p(x);
}

SCALAR_FUN_ATTR f16 futrts_sqrt16(f16 x) {
  return sqrt(x);
}

SCALAR_FUN_ATTR f16 futrts_cbrt16(f16 x) {
  return cbrt(x);
}

SCALAR_FUN_ATTR f16 futrts_exp16(f16 x) {
  return exp(x);
}

SCALAR_FUN_ATTR f16 futrts_cos16(f16 x) {
  return cos(x);
}

SCALAR_FUN_ATTR f16 futrts_sin16(f16 x) {
  return sin(x);
}

SCALAR_FUN_ATTR f16 futrts_tan16(f16 x) {
  return tan(x);
}

SCALAR_FUN_ATTR f16 futrts_acos16(f16 x) {
  return acos(x);
}

SCALAR_FUN_ATTR f16 futrts_asin16(f16 x) {
  return asin(x);
}

SCALAR_FUN_ATTR f16 futrts_atan16(f16 x) {
  return atan(x);
}

SCALAR_FUN_ATTR f16 futrts_cosh16(f16 x) {
  return cosh(x);
}

SCALAR_FUN_ATTR f16 futrts_sinh16(f16 x) {
  return sinh(x);
}

SCALAR_FUN_ATTR f16 futrts_tanh16(f16 x) {
  return tanh(x);
}

SCALAR_FUN_ATTR f16 futrts_acosh16(f16 x) {
  return acosh(x);
}

SCALAR_FUN_ATTR f16 futrts_asinh16(f16 x) {
  return asinh(x);
}

SCALAR_FUN_ATTR f16 futrts_atanh16(f16 x) {
  return atanh(x);
}

SCALAR_FUN_ATTR f16 futrts_atan2_16(f16 x, f16 y) {
  return atan2(x, y);
}

SCALAR_FUN_ATTR f16 futrts_hypot16(f16 x, f16 y) {
  return hypot(x, y);
}

SCALAR_FUN_ATTR f16 futrts_gamma16(f16 x) {
  return tgamma(x);
}

SCALAR_FUN_ATTR f16 futrts_lgamma16(f16 x) {
  return lgamma(x);
}

SCALAR_FUN_ATTR f16 futrts_erf16(f16 x) {
  return erf(x);
}

SCALAR_FUN_ATTR f16 futrts_erfc16(f16 x) {
  return erfc(x);
}

SCALAR_FUN_ATTR f16 fmod16(f16 x, f16 y) {
  return fmod(x, y);
}

SCALAR_FUN_ATTR f16 futrts_round16(f16 x) {
  return rint(x);
}

SCALAR_FUN_ATTR f16 futrts_floor16(f16 x) {
  return floor(x);
}

SCALAR_FUN_ATTR f16 futrts_ceil16(f16 x) {
  return ceil(x);
}

SCALAR_FUN_ATTR f16 futrts_nextafter16(f16 x, f16 y) {
  return nextafter(x, y);
}

SCALAR_FUN_ATTR f16 futrts_lerp16(f16 v0, f16 v1, f16 t) {
  return mix(v0, v1, t);
}

SCALAR_FUN_ATTR f16 futrts_ldexp16(f16 x, int32_t y) {
  return ldexp(x, y);
}

SCALAR_FUN_ATTR f16 futrts_copysign16(f16 x, f16 y) {
  return copysign(x, y);
}

SCALAR_FUN_ATTR f16 futrts_mad16(f16 a, f16 b, f16 c) {
  return mad(a, b, c);
}

SCALAR_FUN_ATTR f16 futrts_fma16(f16 a, f16 b, f16 c) {
  return fma(a, b, c);
}
#elif ISPC

SCALAR_FUN_ATTR f16 futrts_log16(f16 x) {
  return futrts_isfinite16(x) || (futrts_isinf16(x) && x < 0) ? log(x) : x;
}

SCALAR_FUN_ATTR f16 futrts_log2_16(f16 x) {
  return futrts_log16(x) / log(2.0f16);
}

SCALAR_FUN_ATTR f16 futrts_log10_16(f16 x) {
  return futrts_log16(x) / log(10.0f16);
}

SCALAR_FUN_ATTR f16 futrts_log1p_16(f16 x) {
  if(x == -1.0f16 || (futrts_isinf16(x) && x > 0.0f16)) return x / 0.0f16;
  f16 y = 1.0f16 + x;
  f16 z = y - 1.0f16;
  return log(y) - (z-x)/y;
}

SCALAR_FUN_ATTR f16 futrts_sqrt16(f16 x) {
  return (float16)sqrt((float)x);
}

SCALAR_FUN_ATTR f16 futrts_exp16(f16 x) {
  return exp(x);
}

SCALAR_FUN_ATTR f16 futrts_cos16(f16 x) {
  return (float16)cos((float)x);
}

SCALAR_FUN_ATTR f16 futrts_sin16(f16 x) {
  return (float16)sin((float)x);
}

SCALAR_FUN_ATTR f16 futrts_tan16(f16 x) {
  return (float16)tan((float)x);
}

SCALAR_FUN_ATTR f16 futrts_acos16(f16 x) {
  return (float16)acos((float)x);
}

SCALAR_FUN_ATTR f16 futrts_asin16(f16 x) {
  return (float16)asin((float)x);
}

SCALAR_FUN_ATTR f16 futrts_atan16(f16 x) {
  return (float16)atan((float)x);
}

SCALAR_FUN_ATTR f16 futrts_cosh16(f16 x) {
  return (exp(x)+exp(-x)) / 2.0f16;
}

SCALAR_FUN_ATTR f16 futrts_sinh16(f16 x) {
  return (exp(x)-exp(-x)) / 2.0f16;
}

SCALAR_FUN_ATTR f16 futrts_tanh16(f16 x) {
  return futrts_sinh16(x)/futrts_cosh16(x);
}

SCALAR_FUN_ATTR f16 futrts_acosh16(f16 x) {
  float16 f = x+(float16)sqrt((float)(x*x-1));
  if(futrts_isfinite16(f)) return log(f);
  return f;
}

SCALAR_FUN_ATTR f16 futrts_asinh16(f16 x) {
  float16 f = x+(float16)sqrt((float)(x*x+1));
  if(futrts_isfinite16(f)) return log(f);
  return f;
}

SCALAR_FUN_ATTR f16 futrts_atanh16(f16 x) {
  float16 f = (1+x)/(1-x);
  if(futrts_isfinite16(f)) return log(f)/2.0f16;
  return f;
}

SCALAR_FUN_ATTR f16 futrts_atan2_16(f16 x, f16 y) {
  return (float16)atan2((float)x, (float)y);
}

SCALAR_FUN_ATTR f16 futrts_hypot16(f16 x, f16 y) {
  return (float16)futrts_hypot32((float)x, (float)y);
}

extern "C" unmasked uniform float tgammaf(uniform float x);
SCALAR_FUN_ATTR f16 futrts_gamma16(f16 x) {
  f16 res;
  foreach_active (i) {
    uniform f16 r = (f16)tgammaf(extract((float)x, i));
    res = insert(res, i, r);
  }
  return res;
}

extern "C" unmasked uniform float lgammaf(uniform float x);
SCALAR_FUN_ATTR f16 futrts_lgamma16(f16 x) {
  f16 res;
  foreach_active (i) {
    uniform f16 r = (f16)lgammaf(extract((float)x, i));
    res = insert(res, i, r);
  }
  return res;
}

SCALAR_FUN_ATTR f16 futrts_cbrt16(f16 x) {
  f16 res = (f16)futrts_cbrt32((float)x);
  return res;
}

SCALAR_FUN_ATTR f16 futrts_erf16(f16 x) {
  f16 res = (f16)futrts_erf32((float)x);
  return res;
}

SCALAR_FUN_ATTR f16 futrts_erfc16(f16 x) {
  f16 res = (f16)futrts_erfc32((float)x);
  return res;
}

SCALAR_FUN_ATTR f16 fmod16(f16 x, f16 y) {
  return x - y * (float16)trunc((float) (x/y));
}

SCALAR_FUN_ATTR f16 futrts_round16(f16 x) {
  return (float16)round((float)x);
}

SCALAR_FUN_ATTR f16 futrts_floor16(f16 x) {
  return (float16)floor((float)x);
}

SCALAR_FUN_ATTR f16 futrts_ceil16(f16 x) {
  return (float16)ceil((float)x);
}

SCALAR_FUN_ATTR f16 futrts_nextafter16(f16 x, f16 y) {
  return (float16)futrts_nextafter32((float)x, (float) y);
}

SCALAR_FUN_ATTR f16 futrts_lerp16(f16 v0, f16 v1, f16 t) {
  return v0 + (v1 - v0) * t;
}

SCALAR_FUN_ATTR f16 futrts_ldexp16(f16 x, int32_t y) {
  return futrts_ldexp32((float)x, y);
}

SCALAR_FUN_ATTR f16 futrts_copysign16(f16 x, f16 y) {
  return futrts_copysign32((float)x, y);
}

SCALAR_FUN_ATTR f16 futrts_mad16(f16 a, f16 b, f16 c) {
  return a * b + c;
}

SCALAR_FUN_ATTR f16 futrts_fma16(f16 a, f16 b, f16 c) {
  return a * b + c;
}

#else // Assume CUDA.

SCALAR_FUN_ATTR f16 futrts_log16(f16 x) {
  return hlog(x);
}

SCALAR_FUN_ATTR f16 futrts_log2_16(f16 x) {
  return hlog2(x);
}

SCALAR_FUN_ATTR f16 futrts_log10_16(f16 x) {
  return hlog10(x);
}

SCALAR_FUN_ATTR f16 futrts_log1p_16(f16 x) {
  return (f16)log1pf((float)x);
}

SCALAR_FUN_ATTR f16 futrts_sqrt16(f16 x) {
  return hsqrt(x);
}

SCALAR_FUN_ATTR f16 futrts_cbrt16(f16 x) {
  return cbrtf(x);
}

SCALAR_FUN_ATTR f16 futrts_exp16(f16 x) {
  return hexp(x);
}

SCALAR_FUN_ATTR f16 futrts_cos16(f16 x) {
  return hcos(x);
}

SCALAR_FUN_ATTR f16 futrts_sin16(f16 x) {
  return hsin(x);
}

SCALAR_FUN_ATTR f16 futrts_tan16(f16 x) {
  return tanf(x);
}

SCALAR_FUN_ATTR f16 futrts_acos16(f16 x) {
  return acosf(x);
}

SCALAR_FUN_ATTR f16 futrts_asin16(f16 x) {
  return asinf(x);
}

SCALAR_FUN_ATTR f16 futrts_atan16(f16 x) {
  return atanf(x);
}

SCALAR_FUN_ATTR f16 futrts_cosh16(f16 x) {
  return coshf(x);
}

SCALAR_FUN_ATTR f16 futrts_sinh16(f16 x) {
  return sinhf(x);
}

SCALAR_FUN_ATTR f16 futrts_tanh16(f16 x) {
  return tanhf(x);
}

SCALAR_FUN_ATTR f16 futrts_acosh16(f16 x) {
  return acoshf(x);
}

SCALAR_FUN_ATTR f16 futrts_asinh16(f16 x) {
  return asinhf(x);
}

SCALAR_FUN_ATTR f16 futrts_atanh16(f16 x) {
  return atanhf(x);
}

SCALAR_FUN_ATTR f16 futrts_atan2_16(f16 x, f16 y) {
  return atan2f(x, y);
}

SCALAR_FUN_ATTR f16 futrts_hypot16(f16 x, f16 y) {
  return hypotf(x, y);
}

SCALAR_FUN_ATTR f16 futrts_gamma16(f16 x) {
  return tgammaf(x);
}

SCALAR_FUN_ATTR f16 futrts_lgamma16(f16 x) {
  return lgammaf(x);
}

SCALAR_FUN_ATTR f16 futrts_erf16(f16 x) {
  return erff(x);
}

SCALAR_FUN_ATTR f16 futrts_erfc16(f16 x) {
  return erfcf(x);
}

SCALAR_FUN_ATTR f16 fmod16(f16 x, f16 y) {
  return fmodf(x, y);
}

SCALAR_FUN_ATTR f16 futrts_round16(f16 x) {
  return rintf(x);
}

SCALAR_FUN_ATTR f16 futrts_floor16(f16 x) {
  return hfloor(x);
}

SCALAR_FUN_ATTR f16 futrts_ceil16(f16 x) {
  return hceil(x);
}

SCALAR_FUN_ATTR f16 futrts_nextafter16(f16 x, f16 y) {
  return __ushort_as_half(halfbitsnextafter(__half_as_ushort(x), __half_as_ushort(y)));
}

SCALAR_FUN_ATTR f16 futrts_lerp16(f16 v0, f16 v1, f16 t) {
  return v0 + (v1 - v0) * t;
}

SCALAR_FUN_ATTR f16 futrts_ldexp16(f16 x, int32_t y) {
  return futrts_ldexp32((float)x, y);
}

SCALAR_FUN_ATTR f16 futrts_copysign16(f16 x, f16 y) {
  return futrts_copysign32((float)x, y);
}

SCALAR_FUN_ATTR f16 futrts_mad16(f16 a, f16 b, f16 c) {
  return a * b + c;
}

SCALAR_FUN_ATTR f16 futrts_fma16(f16 a, f16 b, f16 c) {
  return fmaf(a, b, c);
}

#endif

// The CUDA __half type cannot be put in unions for some reason, so we
// use bespoke conversion functions instead.
#ifdef __CUDA_ARCH__
SCALAR_FUN_ATTR int16_t futrts_to_bits16(f16 x) {
  return __half_as_ushort(x);
}
SCALAR_FUN_ATTR f16 futrts_from_bits16(int16_t x) {
  return __ushort_as_half(x);
}
#elif ISPC

SCALAR_FUN_ATTR int16_t futrts_to_bits16(f16 x) {
  varying int16_t y = *((varying int16_t * uniform)&x);
  return y;
}

SCALAR_FUN_ATTR f16 futrts_from_bits16(int16_t x) {
  varying f16 y = *((varying f16 * uniform)&x);
  return y;
}
#else
SCALAR_FUN_ATTR int16_t futrts_to_bits16(f16 x) {
  union {
    f16 f;
    int16_t t;
  } p;

  p.f = x;
  return p.t;
}

SCALAR_FUN_ATTR f16 futrts_from_bits16(int16_t x) {
  union {
    int16_t f;
    f16 t;
  } p;

  p.f = x;
  return p.t;
}
#endif

#else // No native f16 - emulate.

SCALAR_FUN_ATTR f16 fabs16(f16 x) {
  return fabs32(x);
}

SCALAR_FUN_ATTR f16 fmax16(f16 x, f16 y) {
  return fmax32(x, y);
}

SCALAR_FUN_ATTR f16 fmin16(f16 x, f16 y) {
  return fmin32(x, y);
}

SCALAR_FUN_ATTR f16 fpow16(f16 x, f16 y) {
  return fpow32(x, y);
}

SCALAR_FUN_ATTR bool futrts_isnan16(f16 x) {
  return futrts_isnan32(x);
}

SCALAR_FUN_ATTR bool futrts_isinf16(f16 x) {
  return futrts_isinf32(x);
}

SCALAR_FUN_ATTR f16 futrts_log16(f16 x) {
  return futrts_log32(x);
}

SCALAR_FUN_ATTR f16 futrts_log2_16(f16 x) {
  return futrts_log2_32(x);
}

SCALAR_FUN_ATTR f16 futrts_log10_16(f16 x) {
  return futrts_log10_32(x);
}

SCALAR_FUN_ATTR f16 futrts_log1p_16(f16 x) {
  return futrts_log1p_32(x);
}

SCALAR_FUN_ATTR f16 futrts_sqrt16(f16 x) {
  return futrts_sqrt32(x);
}

SCALAR_FUN_ATTR f16 futrts_cbrt16(f16 x) {
  return futrts_cbrt32(x);
}

SCALAR_FUN_ATTR f16 futrts_exp16(f16 x) {
  return futrts_exp32(x);
}

SCALAR_FUN_ATTR f16 futrts_cos16(f16 x) {
  return futrts_cos32(x);
}

SCALAR_FUN_ATTR f16 futrts_sin16(f16 x) {
  return futrts_sin32(x);
}

SCALAR_FUN_ATTR f16 futrts_tan16(f16 x) {
  return futrts_tan32(x);
}

SCALAR_FUN_ATTR f16 futrts_acos16(f16 x) {
  return futrts_acos32(x);
}

SCALAR_FUN_ATTR f16 futrts_asin16(f16 x) {
  return futrts_asin32(x);
}

SCALAR_FUN_ATTR f16 futrts_atan16(f16 x) {
  return futrts_atan32(x);
}

SCALAR_FUN_ATTR f16 futrts_cosh16(f16 x) {
  return futrts_cosh32(x);
}

SCALAR_FUN_ATTR f16 futrts_sinh16(f16 x) {
  return futrts_sinh32(x);
}

SCALAR_FUN_ATTR f16 futrts_tanh16(f16 x) {
  return futrts_tanh32(x);
}

SCALAR_FUN_ATTR f16 futrts_acosh16(f16 x) {
  return futrts_acosh32(x);
}

SCALAR_FUN_ATTR f16 futrts_asinh16(f16 x) {
  return futrts_asinh32(x);
}

SCALAR_FUN_ATTR f16 futrts_atanh16(f16 x) {
  return futrts_atanh32(x);
}

SCALAR_FUN_ATTR f16 futrts_atan2_16(f16 x, f16 y) {
  return futrts_atan2_32(x, y);
}

SCALAR_FUN_ATTR f16 futrts_hypot16(f16 x, f16 y) {
  return futrts_hypot32(x, y);
}

SCALAR_FUN_ATTR f16 futrts_gamma16(f16 x) {
  return futrts_gamma32(x);
}

SCALAR_FUN_ATTR f16 futrts_lgamma16(f16 x) {
  return futrts_lgamma32(x);
}

SCALAR_FUN_ATTR f16 futrts_erf16(f16 x) {
  return futrts_erf32(x);
}

SCALAR_FUN_ATTR f16 futrts_erfc16(f16 x) {
  return futrts_erfc32(x);
}

SCALAR_FUN_ATTR f16 fmod16(f16 x, f16 y) {
  return fmod32(x, y);
}

SCALAR_FUN_ATTR f16 futrts_round16(f16 x) {
  return futrts_round32(x);
}

SCALAR_FUN_ATTR f16 futrts_floor16(f16 x) {
  return futrts_floor32(x);
}

SCALAR_FUN_ATTR f16 futrts_ceil16(f16 x) {
  return futrts_ceil32(x);
}

SCALAR_FUN_ATTR f16 futrts_nextafter16(f16 x, f16 y) {
  return halfbits2float(halfbitsnextafter(float2halfbits(x), float2halfbits(y)));
}

SCALAR_FUN_ATTR f16 futrts_lerp16(f16 v0, f16 v1, f16 t) {
  return futrts_lerp32(v0, v1, t);
}

SCALAR_FUN_ATTR f16 futrts_ldexp16(f16 x, int32_t y) {
  return futrts_ldexp32(x, y);
}

SCALAR_FUN_ATTR f16 futrts_copysign16(f16 x, f16 y) {
  return futrts_copysign32((float)x, y);
}

SCALAR_FUN_ATTR f16 futrts_mad16(f16 a, f16 b, f16 c) {
  return futrts_mad32(a, b, c);
}

SCALAR_FUN_ATTR f16 futrts_fma16(f16 a, f16 b, f16 c) {
  return futrts_fma32(a, b, c);
}

// Even when we are using an OpenCL that does not support cl_khr_fp16,
// it must still support vload_half for actually creating a
// half-precision number, which can then be efficiently converted to a
// float.  Similarly for vstore_half.
#ifdef __OPENCL_VERSION__

SCALAR_FUN_ATTR int16_t futrts_to_bits16(f16 x) {
  int16_t y;
  // Violating strict aliasing here.
  vstore_half((float)x, 0, (half*)&y);
  return y;
}

SCALAR_FUN_ATTR f16 futrts_from_bits16(int16_t x) {
  return (f16)vload_half(0, (half*)&x);
}

#else

SCALAR_FUN_ATTR int16_t futrts_to_bits16(f16 x) {
  return (int16_t)float2halfbits(x);
}

SCALAR_FUN_ATTR f16 futrts_from_bits16(int16_t x) {
  return halfbits2float((uint16_t)x);
}

SCALAR_FUN_ATTR f16 fsignum16(f16 x) {
  return futrts_isnan16(x) ? x : (x > 0 ? 1 : 0) - (x < 0 ? 1 : 0);
}

#endif

#endif

SCALAR_FUN_ATTR float fpconv_f16_f16(f16 x) {
  return x;
}

SCALAR_FUN_ATTR float fpconv_f16_f32(f16 x) {
  return x;
}

SCALAR_FUN_ATTR f16 fpconv_f32_f16(float x) {
  return (f16) x;
}

#ifdef FUTHARK_F64_ENABLED

SCALAR_FUN_ATTR double fpconv_f16_f64(f16 x) {
  return (double) x;
}

#if ISPC
SCALAR_FUN_ATTR f16 fpconv_f64_f16(double x) {
  return (f16) ((float)x);
}
#else
SCALAR_FUN_ATTR f16 fpconv_f64_f16(double x) {
  return (f16) x;
}
#endif
#endif


// End of scalar_f16.h.

// Start of context_prototypes.h
//
// Prototypes for the functions in context.h, or that will be called
// from those functions, that need to be available very early.

struct futhark_context_config;
struct futhark_context;

static void set_error(struct futhark_context* ctx, char *error);

// These are called in context/config new/free functions and contain
// shared setup.  They are generated by the compiler itself.
static int init_constants(struct futhark_context*);
static int free_constants(struct futhark_context*);
static void setup_program(struct futhark_context* ctx);
static void teardown_program(struct futhark_context *ctx);

// Allocate host memory.  Must be freed with host_free().
static void host_alloc(struct futhark_context* ctx, size_t size, const char* tag, size_t* size_out, void** mem_out);
// Allocate memory allocated with host_alloc().
static void host_free(struct futhark_context* ctx, size_t size, const char* tag, void* mem);

// Log that a copy has occurred.
static void log_copy(struct futhark_context* ctx,
                     const char *kind, int r,
                     int64_t dst_offset, int64_t dst_strides[r],
                     int64_t src_offset, int64_t src_strides[r],
                     int64_t shape[r]);

static void log_transpose(struct futhark_context* ctx,
                          int64_t k, int64_t m, int64_t n);

static bool lmad_map_tr(int64_t *num_arrays_out, int64_t *n_out, int64_t *m_out,
                        int r,
                        const int64_t dst_strides[r],
                        const int64_t src_strides[r],
                        const int64_t shape[r]);

static bool lmad_contiguous(int r, int64_t strides[r], int64_t shape[r]);

static bool lmad_memcpyable(int r,
                            int64_t dst_strides[r], int64_t src_strides[r], int64_t shape[r]);

static void add_event(struct futhark_context* ctx,
                      const char* name,
                      char* description,
                      void* data,
                      event_report_fn f);

// Functions that must be defined by the backend.
static void backend_context_config_setup(struct futhark_context_config* cfg);
static void backend_context_config_teardown(struct futhark_context_config* cfg);
static int backend_context_setup(struct futhark_context *ctx);
static void backend_context_teardown(struct futhark_context *ctx);

// End of of context_prototypes.h

struct memblock {
    int *references;
    unsigned char *mem;
    int64_t size;
    const char *desc;
};
struct constants {
    int dummy;
};
struct tuning_params {
    int dummy;
};
static const int num_tuning_params = 0;
static const char *tuning_param_names[] = {NULL};
static const char *tuning_param_vars[] = {NULL};
static const char *tuning_param_classes[] = {NULL};
static int64_t tuning_param_defaults[] = {0};
// Start of backends/c.h

struct futhark_context_config {
  int in_use;
  int debugging;
  int profiling;
  int logging;
  char *cache_fname;
  int num_tuning_params;
  int64_t *tuning_params;
  const char** tuning_param_names;
  const char** tuning_param_vars;
  const char** tuning_param_classes;
};

static void backend_context_config_setup(struct futhark_context_config* cfg) {
  (void)cfg;
}

static void backend_context_config_teardown(struct futhark_context_config* cfg) {
  (void)cfg;
}

int futhark_context_config_set_tuning_param(struct futhark_context_config* cfg, const char *param_name, size_t param_value) {
  (void)cfg; (void)param_name; (void)param_value;
  return 1;
}

struct futhark_context {
  struct futhark_context_config* cfg;
  int detail_memory;
  int debugging;
  int profiling;
  int profiling_paused;
  int logging;
  lock_t lock;
  char *error;
  lock_t error_lock;
  FILE *log;
  struct constants *constants;
  struct free_list free_list;
  struct event_list event_list;
  int64_t peak_mem_usage_default;
  int64_t cur_mem_usage_default;
  struct program* program;
  bool program_initialised;
};

int backend_context_setup(struct futhark_context* ctx) {
  (void)ctx;
  return 0;
}

void backend_context_teardown(struct futhark_context* ctx) {
  (void)ctx;
}

int futhark_context_sync(struct futhark_context* ctx) {
  (void)ctx;
  return 0;
}

// End of backends/c.h

struct program {
    int dummy;
};
static void setup_program(struct futhark_context *ctx)
{
    (void) ctx;
    
    int error = 0;
    
    (void) error;
    ctx->program = malloc(sizeof(struct program));
}
static void teardown_program(struct futhark_context *ctx)
{
    (void) ctx;
    
    int error = 0;
    
    (void) error;
    free(ctx->program);
}
static void set_tuning_params(struct futhark_context *ctx)
{
    (void) ctx;
}
int memblock_unref(struct futhark_context *ctx, struct memblock *block, const char *desc)
{
    if (block->references != NULL) {
        *block->references -= 1;
        if (ctx->detail_memory)
            fprintf(ctx->log, "Unreferencing block %s (allocated as %s) in %s: %d references remaining.\n", desc, block->desc, "default space", *block->references);
        if (*block->references == 0) {
            ctx->cur_mem_usage_default -= block->size;
            host_free(ctx, (size_t) block->size, desc, (void *) block->mem);
            free(block->references);
            if (ctx->detail_memory)
                fprintf(ctx->log, "%lld bytes freed (now allocated: %lld bytes)\n", (long long) block->size, (long long) ctx->cur_mem_usage_default);
        }
        block->references = NULL;
    }
    return 0;
}
int memblock_alloc(struct futhark_context *ctx, struct memblock *block, int64_t size, const char *desc)
{
    if (size < 0)
        futhark_panic(1, "Negative allocation of %lld bytes attempted for %s in %s.\n", (long long) size, desc, "default space", ctx->cur_mem_usage_default);
    
    int ret = memblock_unref(ctx, block, desc);
    
    if (ret != FUTHARK_SUCCESS)
        return ret;
    if (ctx->detail_memory)
        fprintf(ctx->log, "Allocating %lld bytes for %s in %s (currently allocated: %lld bytes).\n", (long long) size, desc, "default space", (long long) ctx->cur_mem_usage_default);
    host_alloc(ctx, (size_t) size, desc, (size_t *) &size, (void *) &block->mem);
    if (ctx->error == NULL) {
        block->references = (int *) malloc(sizeof(int));
        *block->references = 1;
        block->size = size;
        block->desc = desc;
        
        long long new_usage = ctx->cur_mem_usage_default + size;
        
        if (ctx->detail_memory)
            fprintf(ctx->log, "Received block of %lld bytes; now allocated: %lld bytes", (long long) block->size, new_usage);
        ctx->cur_mem_usage_default = new_usage;
        if (new_usage > ctx->peak_mem_usage_default) {
            ctx->peak_mem_usage_default = new_usage;
            if (ctx->detail_memory)
                fprintf(ctx->log, " (new peak).\n");
        } else if (ctx->detail_memory)
            fprintf(ctx->log, ".\n");
        return FUTHARK_SUCCESS;
    } else {
        // We are naively assuming that any memory allocation error is due to OOM.
        lock_lock(&ctx->error_lock);
        
        char *old_error = ctx->error;
        
        ctx->error = msgprintf("Failed to allocate memory in %s.\nAttempted allocation: %12lld bytes\nCurrently allocated:  %12lld bytes\n%s", "default space", (long long) size, (long long) ctx->cur_mem_usage_default, old_error);
        free(old_error);
        lock_unlock(&ctx->error_lock);
        return FUTHARK_OUT_OF_MEMORY;
    }
}
int memblock_set(struct futhark_context *ctx, struct memblock *lhs, struct memblock *rhs, const char *lhs_desc)
{
    int ret = memblock_unref(ctx, lhs, lhs_desc);
    
    if (rhs->references != NULL)
        (*rhs->references)++;
    *lhs = *rhs;
    return ret;
}
char *futhark_context_report(struct futhark_context *ctx)
{
    if (futhark_context_sync(ctx) != 0)
        return NULL;
    
    struct str_builder builder;
    
    str_builder_init(&builder);
    str_builder_char(&builder, '{');
    str_builder_str(&builder, "\"memory\":{");
    str_builder(&builder, "\"default space\": %lld", (long long) ctx->peak_mem_usage_default);
    str_builder_str(&builder, "},\"events\":[");
    if (report_events_in_list(&ctx->event_list, &builder) != 0) {
        free(builder.str);
        return NULL;
    } else {
        str_builder_str(&builder, "]}");
        return builder.str;
    }
}
int futhark_context_clear_caches(struct futhark_context *ctx)
{
    lock_lock(&ctx->lock);
    ctx->peak_mem_usage_default = 0;
    lock_unlock(&ctx->lock);
    return ctx->error != NULL;
}

// Start of context.h

// Internal functions.

static void set_error(struct futhark_context* ctx, char *error) {
  lock_lock(&ctx->error_lock);
  if (ctx->error == NULL) {
    ctx->error = error;
  } else {
    free(error);
  }
  lock_unlock(&ctx->error_lock);
}

// XXX: should be static, but used in ispc_util.h
void lexical_realloc_error(struct futhark_context* ctx, size_t new_size) {
  set_error(ctx,
            msgprintf("Failed to allocate memory.\nAttempted allocation: %12lld bytes\n",
                      (long long) new_size));
}

static int lexical_realloc(struct futhark_context *ctx,
                           unsigned char **ptr,
                           int64_t *old_size,
                           int64_t new_size) {
  unsigned char *new = realloc(*ptr, (size_t)new_size);
  if (new == NULL) {
    lexical_realloc_error(ctx, new_size);
    return FUTHARK_OUT_OF_MEMORY;
  } else {
    *ptr = new;
    *old_size = new_size;
    return FUTHARK_SUCCESS;
  }
}

static void free_all_in_free_list(struct futhark_context* ctx) {
  fl_mem mem;
  free_list_pack(&ctx->free_list);
  while (free_list_first(&ctx->free_list, (fl_mem*)&mem) == 0) {
    free((void*)mem);
  }
}

static int is_small_alloc(size_t size) {
  return size < 1024*1024;
}

static void host_alloc(struct futhark_context* ctx,
                       size_t size, const char* tag, size_t* size_out, void** mem_out) {
  if (is_small_alloc(size) || free_list_find(&ctx->free_list, size, tag, size_out, (fl_mem*)mem_out) != 0) {
    *size_out = size;
    *mem_out = malloc(size);
  }
}

static void host_free(struct futhark_context* ctx,
                      size_t size, const char* tag, void* mem) {
  // Small allocations are handled by malloc()s own free list.  The
  // threshold here is kind of arbitrary, but seems to work OK.
  // Larger allocations are mmap()ed/munmapped() every time, which is
  // very slow, and Futhark programs tend to use a few very large
  // allocations.
  if (is_small_alloc(size)) {
    free(mem);
  } else {
    free_list_insert(&ctx->free_list, size, (fl_mem)mem, tag);
  }
}

static void add_event(struct futhark_context* ctx,
                      const char* name,
                      char* description,
                      void* data,
                      event_report_fn f) {
  if (ctx->logging) {
    fprintf(ctx->log, "Event: %s\n%s\n", name, description);
  }
  add_event_to_list(&ctx->event_list, name, description, data, f);
}

char *futhark_context_get_error(struct futhark_context *ctx) {
  char *error = ctx->error;
  ctx->error = NULL;
  return error;
}

void futhark_context_config_set_debugging(struct futhark_context_config *cfg, int flag) {
    cfg->profiling = cfg->logging = cfg->debugging = flag;
}

void futhark_context_config_set_profiling(struct futhark_context_config *cfg, int flag) {
    cfg->profiling = flag;
}

void futhark_context_config_set_logging(struct futhark_context_config *cfg, int flag) {
    cfg->logging = flag;
}

void futhark_context_config_set_cache_file(struct futhark_context_config *cfg, const char *f) {
  cfg->cache_fname = strdup(f);
}

int futhark_get_tuning_param_count(void) {
  return num_tuning_params;
}

const char *futhark_get_tuning_param_name(int i) {
  return tuning_param_names[i];
}

const char *futhark_get_tuning_param_class(int i) {
    return tuning_param_classes[i];
}

void futhark_context_set_logging_file(struct futhark_context *ctx, FILE *f){
  ctx->log = f;
}

void futhark_context_pause_profiling(struct futhark_context *ctx) {
  ctx->profiling_paused = 1;
}

void futhark_context_unpause_profiling(struct futhark_context *ctx) {
  ctx->profiling_paused = 0;
}

struct futhark_context_config* futhark_context_config_new(void) {
  struct futhark_context_config* cfg = malloc(sizeof(struct futhark_context_config));
  if (cfg == NULL) {
    return NULL;
  }
  cfg->in_use = 0;
  cfg->debugging = 0;
  cfg->profiling = 0;
  cfg->logging = 0;
  cfg->cache_fname = NULL;
  cfg->num_tuning_params = num_tuning_params;
  cfg->tuning_params = malloc(cfg->num_tuning_params * sizeof(int64_t));
  memcpy(cfg->tuning_params, tuning_param_defaults,
         cfg->num_tuning_params * sizeof(int64_t));
  cfg->tuning_param_names = tuning_param_names;
  cfg->tuning_param_vars = tuning_param_vars;
  cfg->tuning_param_classes = tuning_param_classes;
  backend_context_config_setup(cfg);
  return cfg;
}

void futhark_context_config_free(struct futhark_context_config* cfg) {
  assert(!cfg->in_use);
  backend_context_config_teardown(cfg);
  free(cfg->cache_fname);
  free(cfg->tuning_params);
  free(cfg);
}

struct futhark_context* futhark_context_new(struct futhark_context_config* cfg) {
  struct futhark_context* ctx = malloc(sizeof(struct futhark_context));
  if (ctx == NULL) {
    return NULL;
  }
  assert(!cfg->in_use);
  ctx->cfg = cfg;
  ctx->cfg->in_use = 1;
  ctx->program_initialised = false;
  create_lock(&ctx->error_lock);
  create_lock(&ctx->lock);
  free_list_init(&ctx->free_list);
  event_list_init(&ctx->event_list);
  ctx->peak_mem_usage_default = 0;
  ctx->cur_mem_usage_default = 0;
  ctx->constants = malloc(sizeof(struct constants));
  ctx->debugging = cfg->debugging;
  ctx->logging = cfg->logging;
  ctx->detail_memory = cfg->logging;
  ctx->profiling = cfg->profiling;
  ctx->profiling_paused = 0;
  ctx->error = NULL;
  ctx->log = stderr;
  set_tuning_params(ctx);
  if (backend_context_setup(ctx) == 0) {
    setup_program(ctx);
    init_constants(ctx);
    ctx->program_initialised = true;
    (void)futhark_context_clear_caches(ctx);
    (void)futhark_context_sync(ctx);
  }
  return ctx;
}

void futhark_context_free(struct futhark_context* ctx) {
  if (ctx->program_initialised) {
    free_constants(ctx);
    teardown_program(ctx);
  }
  backend_context_teardown(ctx);
  free_all_in_free_list(ctx);
  free_list_destroy(&ctx->free_list);
  event_list_free(&ctx->event_list);
  free(ctx->constants);
  free(ctx->error);
  free_lock(&ctx->lock);
  free_lock(&ctx->error_lock);
  ctx->cfg->in_use = 0;
  free(ctx);
}

// End of context.h

// Start of copy.h

// Cache-oblivious map-transpose function.
#define GEN_MAP_TRANSPOSE(NAME, ELEM_TYPE)                              \
  static void map_transpose_##NAME                                      \
  (ELEM_TYPE* dst, ELEM_TYPE* src,                                      \
   int64_t k, int64_t m, int64_t n,                                     \
   int64_t cb, int64_t ce, int64_t rb, int64_t re)                      \
  {                                                                     \
  int32_t r = re - rb;                                                  \
  int32_t c = ce - cb;                                                  \
  if (k == 1) {                                                         \
    if (r <= 64 && c <= 64) {                                           \
      for (int64_t j = 0; j < c; j++) {                                 \
        for (int64_t i = 0; i < r; i++) {                               \
          dst[(j + cb) * n + (i + rb)] = src[(i + rb) * m + (j + cb)];  \
        }                                                               \
      }                                                                 \
    } else if (c <= r) {                                                \
      map_transpose_##NAME(dst, src, k, m, n, cb, ce, rb, rb + r/2);    \
      map_transpose_##NAME(dst, src, k, m, n, cb, ce, rb + r/2, re);    \
    } else {                                                            \
      map_transpose_##NAME(dst, src, k, m, n, cb, cb + c/2, rb, re);    \
      map_transpose_##NAME(dst, src, k, m, n, cb + c/2, ce, rb, re);    \
    }                                                                   \
  } else {                                                              \
  for (int64_t i = 0; i < k; i++) {                                     \
    map_transpose_##NAME(dst + i * m * n, src + i * m * n, 1, m, n, cb, ce, rb, re); \
  }\
} \
}

// Straightforward LMAD copy function.
#define GEN_LMAD_COPY_ELEMENTS(NAME, ELEM_TYPE)                         \
  static void lmad_copy_elements_##NAME(int r,                          \
                                        ELEM_TYPE* dst, int64_t dst_strides[r], \
                                        ELEM_TYPE *src, int64_t src_strides[r], \
                                        int64_t shape[r]) {             \
    if (r == 1) {                                                       \
      for (int i = 0; i < shape[0]; i++) {                              \
        dst[i*dst_strides[0]] = src[i*src_strides[0]];                  \
      }                                                                 \
    } else if (r > 1) {                                                 \
      for (int i = 0; i < shape[0]; i++) {                              \
        lmad_copy_elements_##NAME(r-1,                                  \
                                  dst+i*dst_strides[0], dst_strides+1,  \
                                  src+i*src_strides[0], src_strides+1,  \
                                  shape+1);                             \
      }                                                                 \
    }                                                                   \
  }                                                                     \

// Check whether this LMAD can be seen as a transposed 2D array.  This
// is done by checking every possible splitting point.
static bool lmad_is_tr(int64_t *n_out, int64_t *m_out,
                       int r,
                       const int64_t strides[r],
                       const int64_t shape[r]) {
  for (int i = 1; i < r; i++) {
    int n = 1, m = 1;
    bool ok = true;
    int64_t expected = 1;
    // Check strides before 'i'.
    for (int j = i-1; j >= 0; j--) {
      ok = ok && strides[j] == expected;
      expected *= shape[j];
      n *= shape[j];
    }
    // Check strides after 'i'.
    for (int j = r-1; j >= i; j--) {
      ok = ok && strides[j] == expected;
      expected *= shape[j];
      m *= shape[j];
    }
    if (ok) {
      *n_out = n;
      *m_out = m;
      return true;
    }
  }
  return false;
}

// This function determines whether the a 'dst' LMAD is row-major and
// 'src' LMAD is column-major.  Both LMADs are for arrays of the same
// shape.  Both LMADs are allowed to have additional dimensions "on
// top".  Essentially, this function determines whether a copy from
// 'src' to 'dst' is a "map(transpose)" that we know how to implement
// efficiently.  The LMADs can have arbitrary rank, and the main
// challenge here is checking whether the src LMAD actually
// corresponds to a 2D column-major layout by morally collapsing
// dimensions.  There is a lot of looping here, but the actual trip
// count is going to be very low in practice.
//
// Returns true if this is indeed a map(transpose), and writes the
// number of arrays, and moral array size to appropriate output
// parameters.
static bool lmad_map_tr(int64_t *num_arrays_out, int64_t *n_out, int64_t *m_out,
                        int r,
                        const int64_t dst_strides[r],
                        const int64_t src_strides[r],
                        const int64_t shape[r]) {
  int64_t rowmajor_strides[r];
  rowmajor_strides[r-1] = 1;

  for (int i = r-2; i >= 0; i--) {
    rowmajor_strides[i] = rowmajor_strides[i+1] * shape[i+1];
  }

  // map_r will be the number of mapped dimensions on top.
  int map_r = 0;
  int64_t num_arrays = 1;
  for (int i = 0; i < r; i++) {
    if (dst_strides[i] != rowmajor_strides[i] ||
        src_strides[i] != rowmajor_strides[i]) {
      break;
    } else {
      num_arrays *= shape[i];
      map_r++;
    }
  }

  *num_arrays_out = num_arrays;

  if (r==map_r) {
    return false;
  }

  if (memcmp(&rowmajor_strides[map_r],
             &dst_strides[map_r],
             sizeof(int64_t)*(r-map_r)) == 0) {
    return lmad_is_tr(n_out, m_out, r-map_r, src_strides+map_r, shape+map_r);
  } else if (memcmp(&rowmajor_strides[map_r],
                    &src_strides[map_r],
                    sizeof(int64_t)*(r-map_r)) == 0) {
    return lmad_is_tr(m_out, n_out, r-map_r, dst_strides+map_r, shape+map_r);
  }
  return false;
}

// Check if the strides correspond to row-major strides of *any*
// permutation of the shape.  This is done by recursive search with
// backtracking.  This is worst-case exponential, but hopefully the
// arrays we encounter do not have that many dimensions.
static bool lmad_contiguous_search(int checked, int64_t expected,
                                   int r,
                                   int64_t strides[r], int64_t shape[r], bool used[r]) {
  for (int i = 0; i < r; i++) {
    for (int j = 0; j < r; j++) {
      if (!used[j] && strides[j] == expected && strides[j] >= 0) {
        used[j] = true;
        if (checked+1 == r ||
            lmad_contiguous_search(checked+1, expected * shape[j], r, strides, shape, used)) {
          return true;
        }
        used[j] = false;
      }
    }
  }
  return false;
}

// Does this LMAD correspond to an array with positive strides and no
// holes?
static bool lmad_contiguous(int r, int64_t strides[r], int64_t shape[r]) {
  bool used[r];
  for (int i = 0; i < r; i++) {
    used[i] = false;
  }
  return lmad_contiguous_search(0, 1, r, strides, shape, used);
}

// Does this copy correspond to something that could be done with a
// memcpy()-like operation?  I.e. do the LMADs actually represent the
// same in-memory layout and are they contiguous?
static bool lmad_memcpyable(int r,
                            int64_t dst_strides[r], int64_t src_strides[r], int64_t shape[r]) {
  if (!lmad_contiguous(r, dst_strides, shape)) {
    return false;
  }
  for (int i = 0; i < r; i++) {
    if (dst_strides[i] != src_strides[i] && shape[i] != 1) {
      return false;
    }
  }
  return true;
}


static void log_copy(struct futhark_context* ctx,
                     const char *kind, int r,
                     int64_t dst_offset, int64_t dst_strides[r],
                     int64_t src_offset, int64_t src_strides[r],
                     int64_t shape[r]) {
  if (ctx->logging) {
    fprintf(ctx->log, "\n# Copy %s\n", kind);
    fprintf(ctx->log, "Shape: ");
    for (int i = 0; i < r; i++) { fprintf(ctx->log, "[%ld]", (long int)shape[i]); }
    fprintf(ctx->log, "\n");
    fprintf(ctx->log, "Dst offset: %ld\n", (long int)dst_offset);
    fprintf(ctx->log, "Dst strides:");
    for (int i = 0; i < r; i++) { fprintf(ctx->log, " %ld", (long int)dst_strides[i]); }
    fprintf(ctx->log, "\n");
    fprintf(ctx->log, "Src offset: %ld\n", (long int)src_offset);
    fprintf(ctx->log, "Src strides:");
    for (int i = 0; i < r; i++) { fprintf(ctx->log, " %ld", (long int)src_strides[i]); }
    fprintf(ctx->log, "\n");
  }
}

static void log_transpose(struct futhark_context* ctx,
                          int64_t k, int64_t n, int64_t m) {
  if (ctx->logging) {
    fprintf(ctx->log, "## Transpose\n");
    fprintf(ctx->log, "Arrays     : %ld\n", (long int)k);
    fprintf(ctx->log, "X elements : %ld\n", (long int)m);
    fprintf(ctx->log, "Y elements : %ld\n", (long int)n);
    fprintf(ctx->log, "\n");
  }
}

#define GEN_LMAD_COPY(NAME, ELEM_TYPE)                                  \
  static void lmad_copy_##NAME                                          \
  (struct futhark_context *ctx, int r,                                  \
   ELEM_TYPE* dst, int64_t dst_offset, int64_t dst_strides[r],          \
   ELEM_TYPE *src, int64_t src_offset, int64_t src_strides[r],          \
   int64_t shape[r]) {                                                  \
    log_copy(ctx, "CPU to CPU", r, dst_offset, dst_strides,             \
             src_offset, src_strides, shape);                           \
    int64_t size = 1;                                                   \
    for (int i = 0; i < r; i++) { size *= shape[i]; }                   \
    if (size == 0) { return; }                                          \
    int64_t k, n, m;                                                    \
    if (lmad_map_tr(&k, &n, &m,                                         \
                    r, dst_strides, src_strides, shape)) {              \
      log_transpose(ctx, k, n, m);                                      \
      map_transpose_##NAME                                              \
        (dst+dst_offset, src+src_offset, k, n, m, 0, n, 0, m);          \
    } else if (lmad_memcpyable(r, dst_strides, src_strides, shape)) {   \
      if (ctx->logging) {fprintf(ctx->log, "## Flat copy\n\n");}          \
      memcpy(dst+dst_offset, src+src_offset, size*sizeof(*dst));        \
    } else {                                                            \
      if (ctx->logging) {fprintf(ctx->log, "## General copy\n\n");}       \
      lmad_copy_elements_##NAME                                         \
        (r,                                                             \
         dst+dst_offset, dst_strides,                                   \
         src+src_offset, src_strides, shape);                           \
    }                                                                   \
  }

GEN_MAP_TRANSPOSE(1b, uint8_t)
GEN_MAP_TRANSPOSE(2b, uint16_t)
GEN_MAP_TRANSPOSE(4b, uint32_t)
GEN_MAP_TRANSPOSE(8b, uint64_t)

GEN_LMAD_COPY_ELEMENTS(1b, uint8_t)
GEN_LMAD_COPY_ELEMENTS(2b, uint16_t)
GEN_LMAD_COPY_ELEMENTS(4b, uint32_t)
GEN_LMAD_COPY_ELEMENTS(8b, uint64_t)

GEN_LMAD_COPY(1b, uint8_t)
GEN_LMAD_COPY(2b, uint16_t)
GEN_LMAD_COPY(4b, uint32_t)
GEN_LMAD_COPY(8b, uint64_t)

// End of copy.h

#define FUTHARK_FUN_ATTR static

FUTHARK_FUN_ATTR int futrts_entry_rhj(struct futhark_context *ctx, struct memblock *mem_out_p_24020, struct memblock *mem_out_p_24021, struct memblock *mem_out_p_24022, int64_t *out_prim_out_24023, struct memblock ks1_mem_23557, struct memblock nameless_mem_23558, struct memblock ks2_mem_23559, struct memblock nameless_mem_23560, struct memblock bounds2_mem_23561, struct memblock depths2_mem_23562, struct memblock first_idx2_mem_23563, struct memblock last_idx2_mem_23564, int64_t n1_16484, int64_t b_16485, int64_t dz2080U_16486, int64_t n2_16487, int64_t dz2081U_16488, int64_t dz2082U_16489, int64_t dz2083U_16490, int64_t coercez2081Uz2088U_16491, int64_t coercez2082Uz2081U_16492, int32_t maxDepth2_16497);

static int init_constants(struct futhark_context *ctx)
{
    (void) ctx;
    
    int err = 0;
    
    
  cleanup:
    return err;
}
static int free_constants(struct futhark_context *ctx)
{
    (void) ctx;
    return 0;
}
struct futhark_u8_2d {
    struct memblock mem;
    int64_t shape[2];
};
struct futhark_u8_2d *futhark_new_u8_2d(struct futhark_context *ctx, const uint8_t *data, int64_t dim0, int64_t dim1)
{
    int err = 0;
    struct futhark_u8_2d *bad = NULL;
    struct futhark_u8_2d *arr = (struct futhark_u8_2d *) malloc(sizeof(struct futhark_u8_2d));
    
    if (arr == NULL)
        return bad;
    lock_lock(&ctx->lock);
    arr->mem.references = NULL;
    if (memblock_alloc(ctx, &arr->mem, dim0 * dim1 * 1, "arr->mem"))
        err = 1;
    arr->shape[0] = dim0;
    arr->shape[1] = dim1;
    if ((size_t) (dim0 * dim1) * 1 > 0)
        memmove(arr->mem.mem + 0, (const unsigned char *) data + 0, (size_t) (dim0 * dim1) * 1);
    lock_unlock(&ctx->lock);
    if (err != 0) {
        free(arr);
        return bad;
    }
    return arr;
}
struct futhark_u8_2d *futhark_new_raw_u8_2d(struct futhark_context *ctx, unsigned char *data, int64_t dim0, int64_t dim1)
{
    int err = 0;
    struct futhark_u8_2d *bad = NULL;
    struct futhark_u8_2d *arr = (struct futhark_u8_2d *) malloc(sizeof(struct futhark_u8_2d));
    
    if (arr == NULL)
        return bad;
    lock_lock(&ctx->lock);
    arr->mem.references = NULL;
    arr->mem.mem = data;
    arr->shape[0] = dim0;
    arr->shape[1] = dim1;
    lock_unlock(&ctx->lock);
    return arr;
}
int futhark_free_u8_2d(struct futhark_context *ctx, struct futhark_u8_2d *arr)
{
    lock_lock(&ctx->lock);
    if (memblock_unref(ctx, &arr->mem, "arr->mem") != 0)
        return 1;
    lock_unlock(&ctx->lock);
    free(arr);
    return 0;
}
int futhark_values_u8_2d(struct futhark_context *ctx, struct futhark_u8_2d *arr, uint8_t *data)
{
    int err = 0;
    
    lock_lock(&ctx->lock);
    if ((size_t) (arr->shape[0] * arr->shape[1]) * 1 > 0)
        memmove((unsigned char *) data + 0, arr->mem.mem + 0, (size_t) (arr->shape[0] * arr->shape[1]) * 1);
    lock_unlock(&ctx->lock);
    return err;
}
int futhark_index_u8_2d(struct futhark_context *ctx, uint8_t *out, struct futhark_u8_2d *arr, int64_t i0, int64_t i1)
{
    int err = 0;
    
    if ((i0 >= 0 && i0 < arr->shape[0]) && (i1 >= 0 && i1 < arr->shape[1])) {
        lock_lock(&ctx->lock);
        if (1 > 0)
            memmove((unsigned char *) out + 0, arr->mem.mem + 1 * (i0 * arr->shape[1] + i1 * 1), 1);
        lock_unlock(&ctx->lock);
    } else {
        err = 1;
        set_error(ctx, strdup("Index out of bounds."));
    }
    return err;
}
unsigned char *futhark_values_raw_u8_2d(struct futhark_context *ctx, struct futhark_u8_2d *arr)
{
    (void) ctx;
    return arr->mem.mem;
}
const int64_t *futhark_shape_u8_2d(struct futhark_context *ctx, struct futhark_u8_2d *arr)
{
    (void) ctx;
    return arr->shape;
}
struct futhark_i32_1d {
    struct memblock mem;
    int64_t shape[1];
};
struct futhark_i32_1d *futhark_new_i32_1d(struct futhark_context *ctx, const int32_t *data, int64_t dim0)
{
    int err = 0;
    struct futhark_i32_1d *bad = NULL;
    struct futhark_i32_1d *arr = (struct futhark_i32_1d *) malloc(sizeof(struct futhark_i32_1d));
    
    if (arr == NULL)
        return bad;
    lock_lock(&ctx->lock);
    arr->mem.references = NULL;
    if (memblock_alloc(ctx, &arr->mem, dim0 * 4, "arr->mem"))
        err = 1;
    arr->shape[0] = dim0;
    if ((size_t) dim0 * 4 > 0)
        memmove(arr->mem.mem + 0, (const unsigned char *) data + 0, (size_t) dim0 * 4);
    lock_unlock(&ctx->lock);
    if (err != 0) {
        free(arr);
        return bad;
    }
    return arr;
}
struct futhark_i32_1d *futhark_new_raw_i32_1d(struct futhark_context *ctx, unsigned char *data, int64_t dim0)
{
    int err = 0;
    struct futhark_i32_1d *bad = NULL;
    struct futhark_i32_1d *arr = (struct futhark_i32_1d *) malloc(sizeof(struct futhark_i32_1d));
    
    if (arr == NULL)
        return bad;
    lock_lock(&ctx->lock);
    arr->mem.references = NULL;
    arr->mem.mem = data;
    arr->shape[0] = dim0;
    lock_unlock(&ctx->lock);
    return arr;
}
int futhark_free_i32_1d(struct futhark_context *ctx, struct futhark_i32_1d *arr)
{
    lock_lock(&ctx->lock);
    if (memblock_unref(ctx, &arr->mem, "arr->mem") != 0)
        return 1;
    lock_unlock(&ctx->lock);
    free(arr);
    return 0;
}
int futhark_values_i32_1d(struct futhark_context *ctx, struct futhark_i32_1d *arr, int32_t *data)
{
    int err = 0;
    
    lock_lock(&ctx->lock);
    if ((size_t) arr->shape[0] * 4 > 0)
        memmove((unsigned char *) data + 0, arr->mem.mem + 0, (size_t) arr->shape[0] * 4);
    lock_unlock(&ctx->lock);
    return err;
}
int futhark_index_i32_1d(struct futhark_context *ctx, int32_t *out, struct futhark_i32_1d *arr, int64_t i0)
{
    int err = 0;
    
    if (i0 >= 0 && i0 < arr->shape[0]) {
        lock_lock(&ctx->lock);
        if (4 > 0)
            memmove((unsigned char *) out + 0, arr->mem.mem + 4 * (i0 * 1), 4);
        lock_unlock(&ctx->lock);
    } else {
        err = 1;
        set_error(ctx, strdup("Index out of bounds."));
    }
    return err;
}
unsigned char *futhark_values_raw_i32_1d(struct futhark_context *ctx, struct futhark_i32_1d *arr)
{
    (void) ctx;
    return arr->mem.mem;
}
const int64_t *futhark_shape_i32_1d(struct futhark_context *ctx, struct futhark_i32_1d *arr)
{
    (void) ctx;
    return arr->shape;
}
struct futhark_i64_1d {
    struct memblock mem;
    int64_t shape[1];
};
struct futhark_i64_1d *futhark_new_i64_1d(struct futhark_context *ctx, const int64_t *data, int64_t dim0)
{
    int err = 0;
    struct futhark_i64_1d *bad = NULL;
    struct futhark_i64_1d *arr = (struct futhark_i64_1d *) malloc(sizeof(struct futhark_i64_1d));
    
    if (arr == NULL)
        return bad;
    lock_lock(&ctx->lock);
    arr->mem.references = NULL;
    if (memblock_alloc(ctx, &arr->mem, dim0 * 8, "arr->mem"))
        err = 1;
    arr->shape[0] = dim0;
    if ((size_t) dim0 * 8 > 0)
        memmove(arr->mem.mem + 0, (const unsigned char *) data + 0, (size_t) dim0 * 8);
    lock_unlock(&ctx->lock);
    if (err != 0) {
        free(arr);
        return bad;
    }
    return arr;
}
struct futhark_i64_1d *futhark_new_raw_i64_1d(struct futhark_context *ctx, unsigned char *data, int64_t dim0)
{
    int err = 0;
    struct futhark_i64_1d *bad = NULL;
    struct futhark_i64_1d *arr = (struct futhark_i64_1d *) malloc(sizeof(struct futhark_i64_1d));
    
    if (arr == NULL)
        return bad;
    lock_lock(&ctx->lock);
    arr->mem.references = NULL;
    arr->mem.mem = data;
    arr->shape[0] = dim0;
    lock_unlock(&ctx->lock);
    return arr;
}
int futhark_free_i64_1d(struct futhark_context *ctx, struct futhark_i64_1d *arr)
{
    lock_lock(&ctx->lock);
    if (memblock_unref(ctx, &arr->mem, "arr->mem") != 0)
        return 1;
    lock_unlock(&ctx->lock);
    free(arr);
    return 0;
}
int futhark_values_i64_1d(struct futhark_context *ctx, struct futhark_i64_1d *arr, int64_t *data)
{
    int err = 0;
    
    lock_lock(&ctx->lock);
    if ((size_t) arr->shape[0] * 8 > 0)
        memmove((unsigned char *) data + 0, arr->mem.mem + 0, (size_t) arr->shape[0] * 8);
    lock_unlock(&ctx->lock);
    return err;
}
int futhark_index_i64_1d(struct futhark_context *ctx, int64_t *out, struct futhark_i64_1d *arr, int64_t i0)
{
    int err = 0;
    
    if (i0 >= 0 && i0 < arr->shape[0]) {
        lock_lock(&ctx->lock);
        if (8 > 0)
            memmove((unsigned char *) out + 0, arr->mem.mem + 8 * (i0 * 1), 8);
        lock_unlock(&ctx->lock);
    } else {
        err = 1;
        set_error(ctx, strdup("Index out of bounds."));
    }
    return err;
}
unsigned char *futhark_values_raw_i64_1d(struct futhark_context *ctx, struct futhark_i64_1d *arr)
{
    (void) ctx;
    return arr->mem.mem;
}
const int64_t *futhark_shape_i64_1d(struct futhark_context *ctx, struct futhark_i64_1d *arr)
{
    (void) ctx;
    return arr->shape;
}

FUTHARK_FUN_ATTR int futrts_entry_rhj(struct futhark_context *ctx, struct memblock *mem_out_p_24020, struct memblock *mem_out_p_24021, struct memblock *mem_out_p_24022, int64_t *out_prim_out_24023, struct memblock ks1_mem_23557, struct memblock nameless_mem_23558, struct memblock ks2_mem_23559, struct memblock nameless_mem_23560, struct memblock bounds2_mem_23561, struct memblock depths2_mem_23562, struct memblock first_idx2_mem_23563, struct memblock last_idx2_mem_23564, int64_t n1_16484, int64_t b_16485, int64_t dz2080U_16486, int64_t n2_16487, int64_t dz2081U_16488, int64_t dz2082U_16489, int64_t dz2083U_16490, int64_t coercez2081Uz2088U_16491, int64_t coercez2082Uz2081U_16492, int32_t maxDepth2_16497)
{
    (void) ctx;
    
    int err = 0;
    int64_t mem_23568_cached_sizze_24024 = 0;
    unsigned char *mem_23568 = NULL;
    int64_t mem_23575_cached_sizze_24025 = 0;
    unsigned char *mem_23575 = NULL;
    int64_t mem_23620_cached_sizze_24026 = 0;
    unsigned char *mem_23620 = NULL;
    int64_t mem_23621_cached_sizze_24027 = 0;
    unsigned char *mem_23621 = NULL;
    int64_t mem_23634_cached_sizze_24028 = 0;
    unsigned char *mem_23634 = NULL;
    int64_t mem_23641_cached_sizze_24029 = 0;
    unsigned char *mem_23641 = NULL;
    int64_t mem_23642_cached_sizze_24030 = 0;
    unsigned char *mem_23642 = NULL;
    int64_t mem_23655_cached_sizze_24031 = 0;
    unsigned char *mem_23655 = NULL;
    int64_t mem_23656_cached_sizze_24032 = 0;
    unsigned char *mem_23656 = NULL;
    int64_t mem_23669_cached_sizze_24033 = 0;
    unsigned char *mem_23669 = NULL;
    int64_t mem_23670_cached_sizze_24034 = 0;
    unsigned char *mem_23670 = NULL;
    int64_t mem_23683_cached_sizze_24035 = 0;
    unsigned char *mem_23683 = NULL;
    int64_t mem_23706_cached_sizze_24036 = 0;
    unsigned char *mem_23706 = NULL;
    int64_t mem_23723_cached_sizze_24037 = 0;
    unsigned char *mem_23723 = NULL;
    int64_t mem_23724_cached_sizze_24038 = 0;
    unsigned char *mem_23724 = NULL;
    int64_t mem_23737_cached_sizze_24039 = 0;
    unsigned char *mem_23737 = NULL;
    int64_t mem_23751_cached_sizze_24040 = 0;
    unsigned char *mem_23751 = NULL;
    int64_t mem_23753_cached_sizze_24041 = 0;
    unsigned char *mem_23753 = NULL;
    int64_t mem_23755_cached_sizze_24042 = 0;
    unsigned char *mem_23755 = NULL;
    int64_t mem_23775_cached_sizze_24043 = 0;
    unsigned char *mem_23775 = NULL;
    int64_t mem_23783_cached_sizze_24044 = 0;
    unsigned char *mem_23783 = NULL;
    int64_t mem_23785_cached_sizze_24045 = 0;
    unsigned char *mem_23785 = NULL;
    int64_t mem_23870_cached_sizze_24046 = 0;
    unsigned char *mem_23870 = NULL;
    int64_t mem_23871_cached_sizze_24047 = 0;
    unsigned char *mem_23871 = NULL;
    int64_t mem_23884_cached_sizze_24048 = 0;
    unsigned char *mem_23884 = NULL;
    struct memblock mem_23904;
    
    mem_23904.references = NULL;
    
    struct memblock mem_param_tmp_24011;
    
    mem_param_tmp_24011.references = NULL;
    
    struct memblock mem_param_tmp_24010;
    
    mem_param_tmp_24010.references = NULL;
    
    struct memblock mem_23863;
    
    mem_23863.references = NULL;
    
    struct memblock mem_23861;
    
    mem_23861.references = NULL;
    
    struct memblock mem_param_23859;
    
    mem_param_23859.references = NULL;
    
    struct memblock mem_param_23856;
    
    mem_param_23856.references = NULL;
    
    struct memblock ext_mem_23901;
    
    ext_mem_23901.references = NULL;
    
    struct memblock ext_mem_23902;
    
    ext_mem_23902.references = NULL;
    
    struct memblock mem_23839;
    
    mem_23839.references = NULL;
    
    struct memblock mem_23837;
    
    mem_23837.references = NULL;
    
    struct memblock mem_param_tmp_24003;
    
    mem_param_tmp_24003.references = NULL;
    
    struct memblock mem_param_tmp_24002;
    
    mem_param_tmp_24002.references = NULL;
    
    struct memblock mem_23817;
    
    mem_23817.references = NULL;
    
    struct memblock mem_23815;
    
    mem_23815.references = NULL;
    
    struct memblock mem_param_23813;
    
    mem_param_23813.references = NULL;
    
    struct memblock mem_param_23810;
    
    mem_param_23810.references = NULL;
    
    struct memblock ext_mem_23834;
    
    ext_mem_23834.references = NULL;
    
    struct memblock ext_mem_23835;
    
    ext_mem_23835.references = NULL;
    
    struct memblock mem_23807;
    
    mem_23807.references = NULL;
    
    struct memblock mem_23799;
    
    mem_23799.references = NULL;
    
    struct memblock mem_param_tmp_23982;
    
    mem_param_tmp_23982.references = NULL;
    
    struct memblock mem_23719;
    
    mem_23719.references = NULL;
    
    struct memblock mem_param_23717;
    
    mem_param_23717.references = NULL;
    
    struct memblock ext_mem_23749;
    
    ext_mem_23749.references = NULL;
    
    struct memblock mem_23714;
    
    mem_23714.references = NULL;
    
    struct memblock mem_param_tmp_23944;
    
    mem_param_tmp_23944.references = NULL;
    
    struct memblock mem_param_tmp_23943;
    
    mem_param_tmp_23943.references = NULL;
    
    struct memblock mem_23607;
    
    mem_23607.references = NULL;
    
    struct memblock mem_23605;
    
    mem_23605.references = NULL;
    
    struct memblock mem_param_23603;
    
    mem_param_23603.references = NULL;
    
    struct memblock mem_param_23600;
    
    mem_param_23600.references = NULL;
    
    struct memblock ext_mem_23700;
    
    ext_mem_23700.references = NULL;
    
    struct memblock ext_mem_23701;
    
    ext_mem_23701.references = NULL;
    
    struct memblock mem_23597;
    
    mem_23597.references = NULL;
    
    struct memblock mem_23589;
    
    mem_23589.references = NULL;
    
    struct memblock ext_mem_23704;
    
    ext_mem_23704.references = NULL;
    
    struct memblock mem_23566;
    
    mem_23566.references = NULL;
    
    struct memblock mem_out_23934;
    
    mem_out_23934.references = NULL;
    
    struct memblock mem_out_23933;
    
    mem_out_23933.references = NULL;
    
    struct memblock mem_out_23932;
    
    mem_out_23932.references = NULL;
    
    int64_t prim_out_23935;
    double i64_res_19771 = sitofp_i64_f64(coercez2081Uz2088U_16491);
    double log2_res_19772 = futrts_log2_64(i64_res_19771);
    int32_t f64_res_19773 = fptosi_f64_i32(log2_res_19772);
    int64_t i32_res_19774 = sext_i32_i64(f64_res_19773);
    bool nonnegative_19775 = sle64((int64_t) 0, i32_res_19774);
    bool nonzzero_cert_19776;
    
    if (!nonnegative_19775) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "negative exponent", "-> #0  test_join1.fut:25:49-72\n   #1  test_join1.fut:16:1-29:36\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t dzlz7bUZLztztZRz20U2z20UZLi32z20Uradix_sizzeZRz7dUzg_19777 = shl64((int64_t) 1, i32_res_19774);
    bool dim_match_19778 = dzlz7bUZLztztZRz20U2z20UZLi32z20Uradix_sizzeZRz7dUzg_19777 == coercez2081Uz2088U_16491;
    bool empty_or_match_cert_19779;
    
    if (!dim_match_19778) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Value of (desugared) shape [", (long long) coercez2081Uz2088U_16491, "] cannot match shape of type `[", (long long) dzlz7bUZLztztZRz20U2z20UZLi32z20Uradix_sizzeZRz7dUzg_19777, "]i64`.", "-> #0  test_join1.fut:25:34-76\n   #1  test_join1.fut:16:1-29:36\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool dim_match_19780 = dzlz7bUZLztztZRz20U2z20UZLi32z20Uradix_sizzeZRz7dUzg_19777 == coercez2082Uz2081U_16492;
    bool empty_or_match_cert_19781;
    
    if (!dim_match_19780) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Value of (desugared) shape [", (long long) coercez2082Uz2081U_16492, "] cannot match shape of type `[", (long long) dzlz7bUZLztztZRz20U2z20UZLi32z20Uradix_sizzeZRz7dUzg_19777, "]i64`.", "-> #0  test_join1.fut:26:33-74\n   #1  test_join1.fut:16:1-29:36\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int32_t getRadix_arg1_19782 = sub32(f64_res_19773, 1);
    int32_t zm_lhs_19783 = add32(8, f64_res_19773);
    int32_t zs_lhs_19784 = sub32(zm_lhs_19783, 1);
    int32_t i32_arg0_19785 = sdiv32(zs_lhs_19784, 8);
    int64_t i32_res_19786 = sext_i32_i64(i32_arg0_19785);
    bool bounds_invalid_upwards_19787 = slt64(i32_res_19786, (int64_t) 0);
    bool valid_19788 = !bounds_invalid_upwards_19787;
    bool range_valid_c_19789;
    
    if (!valid_19788) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..<", (long long) i32_res_19786, " is invalid.", "-> #0  ../../../joins/ftHashJoin.fut:41:17-32\n   #1  ../../../joins/ftHashJoin.fut:422:21-46\n   #2  /prelude/functional.fut:9:44-45\n   #3  test_join1.fut:16:1-29:36\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int32_t i64_res_20350 = sext_i64_i32(b_16485);
    int32_t zm_lhs_20351 = mul32(8, i64_res_20350);
    int32_t min_arg1_20352 = sub32(zm_lhs_20351, 1);
    int32_t min_res_20353 = smin32(getRadix_arg1_19782, min_arg1_20352);
    int32_t i32_arg0_20357 = sub32(i64_res_20350, 1);
    int64_t i32_res_20358 = sext_i32_i64(i32_arg0_20357);
    int32_t zm_rhs_20360 = sdiv32(min_res_20353, 8);
    int32_t zm_lhs_20361 = sub32(i64_res_20350, zm_rhs_20360);
    int32_t i32_arg0_20362 = sub32(zm_lhs_20361, 1);
    int64_t i32_res_20363 = sext_i32_i64(i32_arg0_20362);
    int64_t zm_lhs_20364 = mul64((int64_t) 8, b_16485);
    int64_t min_arg1_20365 = sub64(zm_lhs_20364, (int64_t) 1);
    int64_t min_res_20366 = smin64(i32_res_20363, min_arg1_20365);
    int32_t lastBit_20368 = smod32(min_res_20353, 8);
    int32_t zm_lhs_20371 = sub32(8, lastBit_20368);
    int32_t i32_arg0_20372 = sub32(zm_lhs_20371, 1);
    int8_t unsign_arg0_20373 = zext_i32_i8(i32_arg0_20372);
    int8_t unsign_arg0_20374 = lshr8((int8_t) -1, unsign_arg0_20373);
    bool x_20383 = sle64((int64_t) 0, i32_res_20358);
    bool y_20384 = slt64(i32_res_20358, b_16485);
    bool bounds_check_20385 = x_20383 && y_20384;
    bool index_certs_20386;
    
    if (!bounds_check_20385) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i32_res_20358, "] out of bounds for array of shape [", (long long) b_16485, "].", "-> #0  ../../../joins/ftHashJoin.fut:32:50-66\n   #1  test_join1.fut:16:1-29:36\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool x_20394 = sle64((int64_t) 0, min_res_20366);
    bool y_20395 = slt64(min_res_20366, b_16485);
    bool bounds_check_20396 = x_20394 && y_20395;
    bool index_certs_20397;
    
    if (!bounds_check_20396) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) min_res_20366, "] out of bounds for array of shape [", (long long) b_16485, "].", "-> #0  ../../../joins/ftHashJoin.fut:33:40-56\n   #1  test_join1.fut:16:1-29:36\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t bytes_23565 = (int64_t) 8 * n1_16484;
    
    if (memblock_alloc(ctx, &mem_23566, bytes_23565, "mem_23566")) {
        err = 1;
        goto cleanup;
    }
    if (mem_23568_cached_sizze_24024 < bytes_23565) {
        err = lexical_realloc(ctx, &mem_23568, &mem_23568_cached_sizze_24024, bytes_23565);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_23575_cached_sizze_24025 < b_16485) {
        err = lexical_realloc(ctx, &mem_23575, &mem_23575_cached_sizze_24025, b_16485);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    
    bool defunc_0_reduce_res_23370;
    bool redout_23389 = 0;
    
    for (int64_t i_23392 = 0; i_23392 < n1_16484; i_23392++) {
        for (int64_t i_23385 = 0; i_23385 < b_16485; i_23385++) {
            int8_t eta_p_21286 = ((int8_t *) ks1_mem_23557.mem)[i_23392 * b_16485 + i_23385];
            bool cond_21287 = sle64(i_23385, i32_res_20358);
            bool cond_t_res_21288 = sle64(min_res_20366, i_23385);
            bool x_21289 = cond_21287 && cond_t_res_21288;
            int8_t lifted_lambda_res_21290;
            
            if (x_21289) {
                lifted_lambda_res_21290 = eta_p_21286;
            } else {
                lifted_lambda_res_21290 = (int8_t) 0;
            }
            ((int8_t *) mem_23575)[i_23385] = lifted_lambda_res_21290;
        }
        
        int8_t za_lhs_21291 = ((int8_t *) mem_23575)[i32_res_20358];
        int8_t mod1_x_21292 = (int8_t) -1 & za_lhs_21291;
        
        ((int8_t *) mem_23575)[i32_res_20358] = mod1_x_21292;
        
        int8_t za_lhs_21294 = ((int8_t *) mem_23575)[min_res_20366];
        int8_t tmp_21295 = unsign_arg0_20374 & za_lhs_21294;
        
        ((int8_t *) mem_23575)[min_res_20366] = tmp_21295;
        
        int64_t radix_to_idx_res_21297;
        int64_t y_21299 = (int64_t) 0;
        
        for (int64_t i_21298 = 0; i_21298 < i32_res_19786; i_21298++) {
            int64_t zm_lhs_21300 = sub64(b_16485, i_21298);
            int64_t u8_arg0_21301 = sub64(zm_lhs_21300, (int64_t) 1);
            bool x_21302 = sle64((int64_t) 0, u8_arg0_21301);
            bool y_21303 = slt64(u8_arg0_21301, b_16485);
            bool bounds_check_21304 = x_21302 && y_21303;
            bool index_certs_21305;
            
            if (!bounds_check_21304) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) u8_arg0_21301, "] out of bounds for array of shape [", (long long) b_16485, "].", "-> #0  ../../../joins/ftHashJoin.fut:42:25-35\n   #1  ../../../joins/ftHashJoin.fut:422:21-46\n   #2  /prelude/functional.fut:9:44-45\n   #3  test_join1.fut:16:1-29:36\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int8_t u8_arg0_21306 = ((int8_t *) mem_23575)[u8_arg0_21301];
            int64_t u8_res_21307 = zext_i8_i64(u8_arg0_21306);
            int64_t zlzl_rhs_21308 = mul64((int64_t) 8, i_21298);
            int64_t r_21309 = shl64(u8_res_21307, zlzl_rhs_21308);
            int64_t loopres_21310 = y_21299 | r_21309;
            int64_t y_tmp_23940 = loopres_21310;
            
            y_21299 = y_tmp_23940;
        }
        radix_to_idx_res_21297 = y_21299;
        
        bool x_21312 = sle64((int64_t) 0, radix_to_idx_res_21297);
        bool y_21313 = slt64(radix_to_idx_res_21297, dzlz7bUZLztztZRz20U2z20UZLi32z20Uradix_sizzeZRz7dUzg_19777);
        bool bounds_check_21314 = x_21312 && y_21313;
        bool index_certs_21315;
        
        if (!bounds_check_21314) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) radix_to_idx_res_21297, "] out of bounds for array of shape [", (long long) dzlz7bUZLztztZRz20U2z20UZLi32z20Uradix_sizzeZRz7dUzg_19777, "].", "-> #0  ../../../joins/ftHashJoin.fut:423:23-46\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin.fut:423:10-72\n   #3  test_join1.fut:16:1-29:36\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t tmp_21316 = ((int64_t *) first_idx2_mem_23563.mem)[radix_to_idx_res_21297];
        int64_t tmp_21317 = ((int64_t *) last_idx2_mem_23564.mem)[radix_to_idx_res_21297];
        bool lifted_lambda_res_21320 = tmp_21316 == tmp_21317;
        bool lifted_lambda_res_21321 = !lifted_lambda_res_21320;
        bool defunc_0_op_res_19825 = lifted_lambda_res_21321 || redout_23389;
        
        ((int64_t *) mem_23566.mem)[i_23392] = tmp_21316;
        ((int64_t *) mem_23568)[i_23392] = tmp_21317;
        
        bool redout_tmp_23936 = defunc_0_op_res_19825;
        
        redout_23389 = redout_tmp_23936;
    }
    defunc_0_reduce_res_23370 = redout_23389;
    
    bool cond_19826 = !defunc_0_reduce_res_23370;
    int64_t map2_arg2_20883 = add64((int64_t) 1, i32_res_20358);
    
    if (cond_19826) {
        if (memblock_set(ctx, &ext_mem_23704, &mem_23566, "mem_23566") != 0)
            return 1;
    } else {
        if (memblock_alloc(ctx, &mem_23589, bytes_23565, "mem_23589")) {
            err = 1;
            goto cleanup;
        }
        
        int64_t defunc_0_reduce_res_23312;
        int64_t redout_23396 = (int64_t) -9223372036854775808;
        
        for (int64_t i_23398 = 0; i_23398 < n1_16484; i_23398++) {
            int64_t eta_p_21381 = ((int64_t *) mem_23566.mem)[i_23398];
            int64_t eta_p_21382 = ((int64_t *) mem_23568)[i_23398];
            int64_t zp_lhs_21383 = sub64(eta_p_21382, eta_p_21381);
            int64_t lifted_lambda_res_21384 = add64((int64_t) 1, zp_lhs_21383);
            int64_t max_res_19837 = smax64(lifted_lambda_res_21384, redout_23396);
            
            ((int64_t *) mem_23589.mem)[i_23398] = lifted_lambda_res_21384;
            
            int64_t redout_tmp_23941 = max_res_19837;
            
            redout_23396 = redout_tmp_23941;
        }
        defunc_0_reduce_res_23312 = redout_23396;
        
        double i64_res_19838 = sitofp_i64_f64(defunc_0_reduce_res_23312);
        double log2_res_19839 = futrts_log2_64(i64_res_19838);
        double ceil_res_19840 = futrts_ceil64(log2_res_19839);
        int64_t f64_res_19841 = fptosi_f64_i64(ceil_res_19840);
        int64_t num_iter_19842 = add64((int64_t) 1, f64_res_19841);
        
        if (memblock_alloc(ctx, &mem_23597, bytes_23565, "mem_23597")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_23597.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_23566.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {n1_16484});
        if (memblock_set(ctx, &mem_param_23600, &mem_23597, "mem_23597") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_23603, &mem_23589, "mem_23589") != 0)
            return 1;
        for (int64_t i_19846 = 0; i_19846 < num_iter_19842; i_19846++) {
            if (memblock_alloc(ctx, &mem_23605, bytes_23565, "mem_23605")) {
                err = 1;
                goto cleanup;
            }
            if (memblock_alloc(ctx, &mem_23607, bytes_23565, "mem_23607")) {
                err = 1;
                goto cleanup;
            }
            for (int64_t i_23440 = 0; i_23440 < n1_16484; i_23440++) {
                int64_t eta_p_21861 = ((int64_t *) mem_param_23600.mem)[i_23440];
                int64_t eta_p_21862 = ((int64_t *) mem_23566.mem)[i_23440];
                bool cond_21869 = slt64(eta_p_21861, (int64_t) 0);
                bool cond_21878 = eta_p_21861 == eta_p_21862;
                int64_t tmp_21885 = sub64(eta_p_21861, (int64_t) 1);
                bool x_21886 = sle64((int64_t) 0, tmp_21885);
                bool y_21887 = slt64(tmp_21885, dz2082U_16489);
                bool bounds_check_21888 = x_21886 && y_21887;
                bool protect_assert_disj_23918 = cond_21878 || bounds_check_21888;
                bool protect_assert_disj_23924 = cond_21869 || protect_assert_disj_23918;
                bool index_certs_21889;
                
                if (!protect_assert_disj_23924) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_21885, "] out of bounds for array of shape [", (long long) dz2082U_16489, "].", "-> #0  ../../../joins/ftHashJoin.fut:438:66-80\n   #1  /prelude/soacs.fut:67:23-24\n   #2  /prelude/soacs.fut:67:27-37\n   #3  test_join1.fut:16:1-29:36\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                bool x_21903 = sle64((int64_t) 0, eta_p_21861);
                bool y_21904 = slt64(eta_p_21861, dz2082U_16489);
                bool bounds_check_21905 = x_21903 && y_21904;
                bool protect_assert_disj_23927 = cond_21869 || bounds_check_21905;
                bool index_certs_21906;
                
                if (!protect_assert_disj_23927) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_21861, "] out of bounds for array of shape [", (long long) dz2082U_16489, "].", "-> #0  ../../../joins/ftHashJoin.fut:440:33-45\n   #1  /prelude/soacs.fut:67:23-24\n   #2  /prelude/soacs.fut:67:27-37\n   #3  test_join1.fut:16:1-29:36\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                int64_t eta_p_21860 = ((int64_t *) mem_param_23603.mem)[i_23440];
                int64_t eta_p_21864 = ((int64_t *) mem_23568)[i_23440];
                int64_t max_arg1_21865 = sdiv64(eta_p_21860, (int64_t) 2);
                int64_t max_res_21866 = smax64((int64_t) 1, max_arg1_21865);
                int64_t lifted_lambda_res_21870;
                
                if (cond_21869) {
                    lifted_lambda_res_21870 = (int64_t) -1;
                } else {
                    lifted_lambda_res_21870 = eta_p_21861;
                }
                
                bool x_23921 = !cond_21869;
                bool x_23915 = !cond_21878;
                bool protect_cond_conj_23925 = x_23915 && x_23921;
                int64_t tmp_21890;
                
                if (protect_cond_conj_23925) {
                    int64_t x_23919 = ((int64_t *) bounds2_mem_23561.mem)[tmp_21885];
                    
                    tmp_21890 = x_23919;
                } else {
                    tmp_21890 = (int64_t) 0;
                }
                
                int64_t lmad_ext_23614 = b_16485 * tmp_21890;
                int64_t lmad_ext_23615 = b_16485 * eta_p_21862;
                int64_t cur_elem_21907;
                
                if (x_23921) {
                    int64_t x_23928 = ((int64_t *) bounds2_mem_23561.mem)[eta_p_21861];
                    
                    cur_elem_21907 = x_23928;
                } else {
                    cur_elem_21907 = (int64_t) 0;
                }
                
                int64_t lmad_ext_23617 = b_16485 * cur_elem_21907;
                int64_t ext_23618;
                
                if (cond_21869) {
                    ext_23618 = (int64_t) 0;
                } else {
                    ext_23618 = lmad_ext_23617;
                }
                
                int64_t ext_23616;
                
                if (cond_21878) {
                    ext_23616 = lmad_ext_23615;
                } else {
                    ext_23616 = lmad_ext_23614;
                }
                
                int64_t ext_23619;
                
                if (cond_21869) {
                    ext_23619 = (int64_t) 0;
                } else {
                    ext_23619 = ext_23616;
                }
                
                int32_t lifted_lambda_res_21873;
                int32_t lifted_lambda_res_21874;
                
                if (cond_21869) {
                    bool y_23191 = slt64((int64_t) 0, n2_16487);
                    bool index_certs_23192;
                    
                    if (!y_23191) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) n2_16487, "].", "-> #0  ../../../joins/ftHashJoin.fut:437:33-38\n   #1  /prelude/soacs.fut:67:23-24\n   #2  /prelude/soacs.fut:67:27-37\n   #3  test_join1.fut:16:1-29:36\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    lifted_lambda_res_21873 = 0;
                    lifted_lambda_res_21874 = 0;
                } else {
                    bool x_21908 = sle64((int64_t) 0, cur_elem_21907);
                    bool y_21909 = slt64(cur_elem_21907, n2_16487);
                    bool bounds_check_21910 = x_21908 && y_21909;
                    bool index_certs_21911;
                    
                    if (!bounds_check_21910) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) cur_elem_21907, "] out of bounds for array of shape [", (long long) n2_16487, "].", "-> #0  ../../../joins/ftHashJoin.fut:440:30-46\n   #1  /prelude/soacs.fut:67:23-24\n   #2  /prelude/soacs.fut:67:27-37\n   #3  test_join1.fut:16:1-29:36\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    bool y_21913 = slt64(eta_p_21861, dz2083U_16490);
                    bool bounds_check_21914 = x_21903 && y_21913;
                    bool index_certs_21915;
                    
                    if (!bounds_check_21914) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_21861, "] out of bounds for array of shape [", (long long) dz2083U_16490, "].", "-> #0  ../../../joins/ftHashJoin.fut:441:31-43\n   #1  /prelude/soacs.fut:67:23-24\n   #2  /prelude/soacs.fut:67:27-37\n   #3  test_join1.fut:16:1-29:36\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    if (cond_21878) {
                        bool x_23194 = sle64((int64_t) 0, eta_p_21862);
                        bool y_23195 = slt64(eta_p_21862, n2_16487);
                        bool bounds_check_23196 = x_23194 && y_23195;
                        bool index_certs_23197;
                        
                        if (!bounds_check_23196) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_21862, "] out of bounds for array of shape [", (long long) n2_16487, "].", "-> #0  ../../../joins/ftHashJoin.fut:438:48-57\n   #1  /prelude/soacs.fut:67:23-24\n   #2  /prelude/soacs.fut:67:27-37\n   #3  test_join1.fut:16:1-29:36\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                    } else {
                        bool x_21891 = sle64((int64_t) 0, tmp_21890);
                        bool y_21892 = slt64(tmp_21890, n2_16487);
                        bool bounds_check_21893 = x_21891 && y_21892;
                        bool index_certs_21894;
                        
                        if (!bounds_check_21893) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_21890, "] out of bounds for array of shape [", (long long) n2_16487, "].", "-> #0  ../../../joins/ftHashJoin.fut:438:63-81\n   #1  /prelude/soacs.fut:67:23-24\n   #2  /prelude/soacs.fut:67:27-37\n   #3  test_join1.fut:16:1-29:36\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                    }
                    
                    int32_t prev_depth_21896;
                    
                    if (cond_21878) {
                        prev_depth_21896 = 0;
                    } else {
                        bool y_21899 = slt64(tmp_21885, dz2083U_16490);
                        bool bounds_check_21900 = x_21886 && y_21899;
                        bool index_certs_21901;
                        
                        if (!bounds_check_21900) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_21885, "] out of bounds for array of shape [", (long long) dz2083U_16490, "].", "-> #0  ../../../joins/ftHashJoin.fut:439:56-70\n   #1  /prelude/soacs.fut:67:23-24\n   #2  /prelude/soacs.fut:67:27-37\n   #3  test_join1.fut:16:1-29:36\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int32_t prev_depth_f_res_21902 = ((int32_t *) depths2_mem_23562.mem)[tmp_21885];
                        
                        prev_depth_21896 = prev_depth_f_res_21902;
                    }
                    
                    int32_t cur_depth_21916 = ((int32_t *) depths2_mem_23562.mem)[eta_p_21861];
                    
                    lifted_lambda_res_21873 = prev_depth_21896;
                    lifted_lambda_res_21874 = cur_depth_21916;
                }
                
                bool cond_21922 = slt64(lifted_lambda_res_21870, (int64_t) 0);
                int32_t zm_lhs_21923 = mul32(f64_res_19773, lifted_lambda_res_21874);
                int32_t byteSeq_eq_arg1_21924 = sub32(zm_lhs_21923, 1);
                int32_t zm_rhs_21925 = sdiv32(byteSeq_eq_arg1_21924, 8);
                int32_t zm_lhs_21926 = sub32(i64_res_20350, zm_rhs_21925);
                int32_t i32_arg0_21927 = sub32(zm_lhs_21926, 1);
                int64_t i32_res_21928 = sext_i32_i64(i32_arg0_21927);
                int64_t j_m_i_21929 = sub64(map2_arg2_20883, i32_res_21928);
                int64_t lifted_lambda_res_21930;
                
                if (cond_21922) {
                    lifted_lambda_res_21930 = (int64_t) -1;
                } else {
                    int32_t min_res_21934 = smin32(min_arg1_20352, byteSeq_eq_arg1_21924);
                    int32_t zm_rhs_21937 = sdiv32(min_res_21934, 8);
                    int32_t zm_lhs_21938 = sub32(i64_res_20350, zm_rhs_21937);
                    int32_t i32_arg0_21939 = sub32(zm_lhs_21938, 1);
                    int64_t i32_res_21940 = sext_i32_i64(i32_arg0_21939);
                    int64_t min_res_21943 = smin64(min_arg1_20365, i32_res_21940);
                    bool x_21964 = sle64((int64_t) 0, min_res_21943);
                    bool y_21965 = slt64(min_res_21943, b_16485);
                    bool bounds_check_21966 = x_21964 && y_21965;
                    bool index_certs_21967;
                    
                    if (!bounds_check_21966) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) min_res_21943, "] out of bounds for array of shape [", (long long) b_16485, "].", "-> #0  ../../../joins/ftHashJoin.fut:33:40-56\n   #1  test_join1.fut:16:1-29:36\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    bool empty_slice_21984 = j_m_i_21929 == (int64_t) 0;
                    int64_t m_21985 = sub64(j_m_i_21929, (int64_t) 1);
                    int64_t i_p_m_t_s_21986 = add64(i32_res_21928, m_21985);
                    bool zzero_leq_i_p_m_t_s_21987 = sle64((int64_t) 0, i_p_m_t_s_21986);
                    bool i_p_m_t_s_leq_w_21988 = slt64(i_p_m_t_s_21986, b_16485);
                    bool zzero_lte_i_21989 = sle64((int64_t) 0, i32_res_21928);
                    bool i_lte_j_21990 = sle64(i32_res_21928, map2_arg2_20883);
                    bool y_21991 = i_p_m_t_s_leq_w_21988 && zzero_lte_i_21989;
                    bool y_21992 = zzero_leq_i_p_m_t_s_21987 && y_21991;
                    bool forwards_ok_21993 = i_lte_j_21990 && y_21992;
                    bool ok_or_empty_21994 = empty_slice_21984 || forwards_ok_21993;
                    bool index_certs_21995;
                    
                    if (!ok_or_empty_21994) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i32_res_21928, ":", (long long) map2_arg2_20883, "] out of bounds for array of shape [", (long long) b_16485, "].", "-> #0  ../../../joins/ftHashJoin.fut:51:49-60\n   #1  test_join1.fut:16:1-29:36\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int32_t lastBit_21944 = smod32(min_res_21934, 8);
                    int32_t zm_lhs_21945 = sub32(8, lastBit_21944);
                    int32_t i32_arg0_21946 = sub32(zm_lhs_21945, 1);
                    int8_t unsign_arg0_21947 = zext_i32_i8(i32_arg0_21946);
                    int8_t unsign_arg0_21948 = lshr8((int8_t) -1, unsign_arg0_21947);
                    
                    if (mem_23620_cached_sizze_24026 < b_16485) {
                        err = lexical_realloc(ctx, &mem_23620, &mem_23620_cached_sizze_24026, b_16485);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    if (mem_23621_cached_sizze_24027 < b_16485) {
                        err = lexical_realloc(ctx, &mem_23621, &mem_23621_cached_sizze_24027, b_16485);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    for (int64_t i_23404 = 0; i_23404 < b_16485; i_23404++) {
                        int8_t eta_p_22356 = ((int8_t *) ks1_mem_23557.mem)[i_23440 * b_16485 + i_23404];
                        int8_t eta_p_22357 = ((int8_t *) ks2_mem_23559.mem)[ext_23618 + i_23404];
                        bool cond_22359 = sle64(i_23404, i32_res_20358);
                        bool cond_t_res_22360 = sle64(min_res_21943, i_23404);
                        bool x_22361 = cond_22359 && cond_t_res_22360;
                        int8_t lifted_lambda_res_22362;
                        
                        if (x_22361) {
                            lifted_lambda_res_22362 = eta_p_22356;
                        } else {
                            lifted_lambda_res_22362 = (int8_t) 0;
                        }
                        
                        int8_t lifted_lambda_res_22367;
                        
                        if (x_22361) {
                            lifted_lambda_res_22367 = eta_p_22357;
                        } else {
                            lifted_lambda_res_22367 = (int8_t) 0;
                        }
                        ((int8_t *) mem_23620)[i_23404] = lifted_lambda_res_22367;
                        ((int8_t *) mem_23621)[i_23404] = lifted_lambda_res_22362;
                    }
                    
                    int8_t za_lhs_21961 = ((int8_t *) mem_23621)[i32_res_20358];
                    int8_t mod1_x_21962 = (int8_t) -1 & za_lhs_21961;
                    
                    ((int8_t *) mem_23621)[i32_res_20358] = mod1_x_21962;
                    
                    int8_t za_lhs_21968 = ((int8_t *) mem_23621)[min_res_21943];
                    int8_t tmp_21969 = unsign_arg0_21948 & za_lhs_21968;
                    
                    ((int8_t *) mem_23621)[min_res_21943] = tmp_21969;
                    
                    int8_t za_lhs_21978 = ((int8_t *) mem_23620)[i32_res_20358];
                    int8_t mod1_x_21979 = (int8_t) -1 & za_lhs_21978;
                    
                    ((int8_t *) mem_23620)[i32_res_20358] = mod1_x_21979;
                    
                    int8_t za_lhs_21981 = ((int8_t *) mem_23620)[min_res_21943];
                    int8_t tmp_21982 = unsign_arg0_21948 & za_lhs_21981;
                    
                    ((int8_t *) mem_23620)[min_res_21943] = tmp_21982;
                    if (mem_23634_cached_sizze_24028 < j_m_i_21929) {
                        err = lexical_realloc(ctx, &mem_23634, &mem_23634_cached_sizze_24028, j_m_i_21929);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    for (int64_t i_23409 = 0; i_23409 < j_m_i_21929; i_23409++) {
                        int64_t slice_23552 = i32_res_21928 + i_23409;
                        int8_t eta_p_21999 = ((int8_t *) mem_23621)[slice_23552];
                        int8_t eta_p_22000 = ((int8_t *) mem_23620)[slice_23552];
                        bool defunc_0_f_res_22001 = eta_p_21999 == eta_p_22000;
                        
                        ((bool *) mem_23634)[i_23409] = defunc_0_f_res_22001;
                    }
                    
                    bool defunc_0_foldl_res_22002;
                    bool acc_22004 = 1;
                    
                    for (int64_t i_22003 = 0; i_22003 < j_m_i_21929; i_22003++) {
                        bool b_22005 = ((bool *) mem_23634)[i_22003];
                        bool x_22006 = acc_22004 && b_22005;
                        bool acc_tmp_23952 = x_22006;
                        
                        acc_22004 = acc_tmp_23952;
                    }
                    defunc_0_foldl_res_22002 = acc_22004;
                    
                    bool loop_cond_t_res_22085 = slt32(0, lifted_lambda_res_21874);
                    int64_t lifted_lambda_res_f_res_22007;
                    
                    if (defunc_0_foldl_res_22002) {
                        bool eq_x_y_23204 = eta_p_21862 == (int64_t) -1;
                        bool eq_x_zz_23205 = eta_p_21862 == eta_p_21861;
                        bool p_and_eq_x_y_23206 = cond_21869 && eq_x_y_23204;
                        bool p_and_eq_x_y_23208 = eq_x_zz_23205 && x_23921;
                        bool cond_23209 = p_and_eq_x_y_23206 || p_and_eq_x_y_23208;
                        int32_t zm_lhs_23210 = mul32(f64_res_19773, lifted_lambda_res_21873);
                        int32_t byteSeq_neq_arg1_23211 = sub32(zm_lhs_23210, 1);
                        int32_t zm_rhs_23212 = sdiv32(byteSeq_neq_arg1_23211, 8);
                        int32_t zm_lhs_23213 = sub32(i64_res_20350, zm_rhs_23212);
                        int32_t i32_arg0_23214 = sub32(zm_lhs_23213, 1);
                        int64_t i32_res_23215 = sext_i32_i64(i32_arg0_23214);
                        int64_t j_m_i_23217 = sub64(map2_arg2_20883, i32_res_23215);
                        bool cond_23218;
                        
                        if (cond_23209) {
                            cond_23218 = 1;
                        } else {
                            int32_t min_res_23219 = smin32(min_arg1_20352, byteSeq_neq_arg1_23211);
                            int32_t zm_rhs_23220 = sdiv32(min_res_23219, 8);
                            int32_t zm_lhs_23221 = sub32(i64_res_20350, zm_rhs_23220);
                            int32_t i32_arg0_23222 = sub32(zm_lhs_23221, 1);
                            int64_t i32_res_23223 = sext_i32_i64(i32_arg0_23222);
                            int64_t min_res_23224 = smin64(min_arg1_20365, i32_res_23223);
                            bool x_23248 = sle64((int64_t) 0, min_res_23224);
                            bool y_23249 = slt64(min_res_23224, b_16485);
                            bool bounds_check_23250 = x_23248 && y_23249;
                            bool index_certs_23251;
                            
                            if (!bounds_check_23250) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) min_res_23224, "] out of bounds for array of shape [", (long long) b_16485, "].", "-> #0  ../../../joins/ftHashJoin.fut:33:40-56\n   #1  test_join1.fut:16:1-29:36\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            bool empty_slice_23261 = j_m_i_23217 == (int64_t) 0;
                            int64_t m_23262 = sub64(j_m_i_23217, (int64_t) 1);
                            int64_t i_p_m_t_s_23263 = add64(i32_res_23215, m_23262);
                            bool zzero_leq_i_p_m_t_s_23264 = sle64((int64_t) 0, i_p_m_t_s_23263);
                            bool i_p_m_t_s_leq_w_23265 = slt64(i_p_m_t_s_23263, b_16485);
                            bool zzero_lte_i_23266 = sle64((int64_t) 0, i32_res_23215);
                            bool i_lte_j_23267 = sle64(i32_res_23215, map2_arg2_20883);
                            bool y_23268 = i_p_m_t_s_leq_w_23265 && zzero_lte_i_23266;
                            bool y_23269 = zzero_leq_i_p_m_t_s_23264 && y_23268;
                            bool forwards_ok_23270 = i_lte_j_23267 && y_23269;
                            bool ok_or_empty_23271 = empty_slice_23261 || forwards_ok_23270;
                            bool index_certs_23272;
                            
                            if (!ok_or_empty_23271) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i32_res_23215, ":", (long long) map2_arg2_20883, "] out of bounds for array of shape [", (long long) b_16485, "].", "-> #0  ../../../joins/ftHashJoin.fut:59:50-61\n   #1  ../../../joins/ftHashJoin.fut:141:5-43\n   #2  ../../../joins/ftHashJoin.fut:449:31-73\n   #3  /prelude/soacs.fut:83:33-34\n   #4  /prelude/soacs.fut:83:37-53\n   #5  test_join1.fut:16:1-29:36\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int32_t lastBit_23225 = smod32(min_res_23219, 8);
                            int32_t zm_lhs_23226 = sub32(8, lastBit_23225);
                            int32_t i32_arg0_23227 = sub32(zm_lhs_23226, 1);
                            int8_t unsign_arg0_23228 = zext_i32_i8(i32_arg0_23227);
                            int8_t unsign_arg0_23229 = lshr8((int8_t) -1, unsign_arg0_23228);
                            
                            if (mem_23669_cached_sizze_24033 < b_16485) {
                                err = lexical_realloc(ctx, &mem_23669, &mem_23669_cached_sizze_24033, b_16485);
                                if (err != FUTHARK_SUCCESS)
                                    goto cleanup;
                            }
                            if (mem_23670_cached_sizze_24034 < b_16485) {
                                err = lexical_realloc(ctx, &mem_23670, &mem_23670_cached_sizze_24034, b_16485);
                                if (err != FUTHARK_SUCCESS)
                                    goto cleanup;
                            }
                            for (int64_t i_23415 = 0; i_23415 < b_16485; i_23415++) {
                                int8_t eta_p_23233 = ((int8_t *) ks1_mem_23557.mem)[i_23440 * b_16485 + i_23415];
                                int8_t eta_p_23234 = ((int8_t *) ks2_mem_23559.mem)[ext_23619 + i_23415];
                                bool cond_23235 = sle64(i_23415, i32_res_20358);
                                bool cond_t_res_23236 = sle64(min_res_23224, i_23415);
                                bool x_23237 = cond_23235 && cond_t_res_23236;
                                int8_t lifted_lambda_res_23238;
                                
                                if (x_23237) {
                                    lifted_lambda_res_23238 = eta_p_23233;
                                } else {
                                    lifted_lambda_res_23238 = (int8_t) 0;
                                }
                                
                                int8_t lifted_lambda_res_23242;
                                
                                if (x_23237) {
                                    lifted_lambda_res_23242 = eta_p_23234;
                                } else {
                                    lifted_lambda_res_23242 = (int8_t) 0;
                                }
                                ((int8_t *) mem_23669)[i_23415] = lifted_lambda_res_23242;
                                ((int8_t *) mem_23670)[i_23415] = lifted_lambda_res_23238;
                            }
                            
                            int8_t za_lhs_23245 = ((int8_t *) mem_23670)[i32_res_20358];
                            int8_t mod1_x_23246 = (int8_t) -1 & za_lhs_23245;
                            
                            ((int8_t *) mem_23670)[i32_res_20358] = mod1_x_23246;
                            
                            int8_t za_lhs_23252 = ((int8_t *) mem_23670)[min_res_23224];
                            int8_t tmp_23253 = unsign_arg0_23229 & za_lhs_23252;
                            
                            ((int8_t *) mem_23670)[min_res_23224] = tmp_23253;
                            
                            int8_t za_lhs_23255 = ((int8_t *) mem_23669)[i32_res_20358];
                            int8_t mod1_x_23256 = (int8_t) -1 & za_lhs_23255;
                            
                            ((int8_t *) mem_23669)[i32_res_20358] = mod1_x_23256;
                            
                            int8_t za_lhs_23258 = ((int8_t *) mem_23669)[min_res_23224];
                            int8_t tmp_23259 = unsign_arg0_23229 & za_lhs_23258;
                            
                            ((int8_t *) mem_23669)[min_res_23224] = tmp_23259;
                            if (mem_23683_cached_sizze_24035 < j_m_i_23217) {
                                err = lexical_realloc(ctx, &mem_23683, &mem_23683_cached_sizze_24035, j_m_i_23217);
                                if (err != FUTHARK_SUCCESS)
                                    goto cleanup;
                            }
                            for (int64_t i_23420 = 0; i_23420 < j_m_i_23217; i_23420++) {
                                int64_t slice_23544 = i32_res_23215 + i_23420;
                                int8_t eta_p_23276 = ((int8_t *) mem_23670)[slice_23544];
                                int8_t eta_p_23277 = ((int8_t *) mem_23669)[slice_23544];
                                bool defunc_0_f_res_23278 = eta_p_23276 == eta_p_23277;
                                bool defunc_0_f_res_23279 = !defunc_0_f_res_23278;
                                
                                ((bool *) mem_23683)[i_23420] = defunc_0_f_res_23279;
                            }
                            
                            bool defunc_0_foldl_res_23281;
                            bool acc_23283 = 0;
                            
                            for (int64_t i_23282 = 0; i_23282 < j_m_i_23217; i_23282++) {
                                bool b_23284 = ((bool *) mem_23683)[i_23282];
                                bool defunc_0_f_res_23285 = acc_23283 || b_23284;
                                bool acc_tmp_23956 = defunc_0_f_res_23285;
                                
                                acc_23283 = acc_tmp_23956;
                            }
                            defunc_0_foldl_res_23281 = acc_23283;
                            cond_23218 = defunc_0_foldl_res_23281;
                        }
                        
                        int64_t lifted_lambda_res_f_res_t_res_23286;
                        
                        if (cond_23218) {
                            lifted_lambda_res_f_res_t_res_23286 = lifted_lambda_res_21870;
                        } else {
                            int64_t lifted_lambda_res_f_res_t_res_f_res_23287 = sub64(lifted_lambda_res_21870, max_res_21866);
                            
                            lifted_lambda_res_f_res_t_res_23286 = lifted_lambda_res_f_res_t_res_f_res_23287;
                        }
                        lifted_lambda_res_f_res_22007 = lifted_lambda_res_f_res_t_res_23286;
                    } else {
                        if (mem_23641_cached_sizze_24029 < b_16485) {
                            err = lexical_realloc(ctx, &mem_23641, &mem_23641_cached_sizze_24029, b_16485);
                            if (err != FUTHARK_SUCCESS)
                                goto cleanup;
                        }
                        if (mem_23642_cached_sizze_24030 < b_16485) {
                            err = lexical_realloc(ctx, &mem_23642, &mem_23642_cached_sizze_24030, b_16485);
                            if (err != FUTHARK_SUCCESS)
                                goto cleanup;
                        }
                        
                        bool radix_gt_res_22086;
                        int32_t radix_gt_res_22087;
                        bool radix_gt_res_22088;
                        bool loop_while_22089;
                        int32_t p_22090;
                        bool p_22091;
                        
                        loop_while_22089 = loop_cond_t_res_22085;
                        p_22090 = 0;
                        p_22091 = 0;
                        while (loop_while_22089) {
                            int32_t zt_rhs_22092 = add32(1, p_22090);
                            int32_t zm_lhs_22093 = mul32(f64_res_19773, zt_rhs_22092);
                            int32_t byteSeq_lt_arg1_22094 = sub32(zm_lhs_22093, 1);
                            int32_t byteSeq_lt_arg0_22095 = mul32(f64_res_19773, p_22090);
                            int32_t min_res_22096 = smin32(min_arg1_20352, byteSeq_lt_arg1_22094);
                            int32_t zm_rhs_22097 = sdiv32(byteSeq_lt_arg0_22095, 8);
                            int32_t zm_lhs_22098 = sub32(i64_res_20350, zm_rhs_22097);
                            int32_t i32_arg0_22099 = sub32(zm_lhs_22098, 1);
                            int64_t i32_res_22100 = sext_i32_i64(i32_arg0_22099);
                            int32_t zm_rhs_22101 = sdiv32(min_res_22096, 8);
                            int32_t zm_lhs_22102 = sub32(i64_res_20350, zm_rhs_22101);
                            int32_t i32_arg0_22103 = sub32(zm_lhs_22102, 1);
                            int64_t i32_res_22104 = sext_i32_i64(i32_arg0_22103);
                            int64_t min_res_22105 = smin64(min_arg1_20365, i32_res_22104);
                            bool x_22121 = sle64((int64_t) 0, i32_res_22100);
                            bool y_22122 = slt64(i32_res_22100, b_16485);
                            bool bounds_check_22123 = x_22121 && y_22122;
                            bool index_certs_22124;
                            
                            if (!bounds_check_22123) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i32_res_22100, "] out of bounds for array of shape [", (long long) b_16485, "].", "-> #0  ../../../joins/ftHashJoin.fut:32:50-66\n   #1  test_join1.fut:16:1-29:36\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            bool x_22128 = sle64((int64_t) 0, min_res_22105);
                            bool y_22129 = slt64(min_res_22105, b_16485);
                            bool bounds_check_22130 = x_22128 && y_22129;
                            bool index_certs_22131;
                            
                            if (!bounds_check_22130) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) min_res_22105, "] out of bounds for array of shape [", (long long) b_16485, "].", "-> #0  ../../../joins/ftHashJoin.fut:33:40-56\n   #1  test_join1.fut:16:1-29:36\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int32_t firstBit_22106 = smod32(byteSeq_lt_arg0_22095, 8);
                            int32_t lastBit_22107 = smod32(min_res_22096, 8);
                            int8_t unsign_arg0_22108 = zext_i32_i8(firstBit_22106);
                            int8_t first_bitMask_22109 = shl8((int8_t) -1, unsign_arg0_22108);
                            int32_t zm_lhs_22110 = sub32(8, lastBit_22107);
                            int32_t i32_arg0_22111 = sub32(zm_lhs_22110, 1);
                            int8_t unsign_arg0_22112 = zext_i32_i8(i32_arg0_22111);
                            int8_t unsign_arg0_22113 = lshr8((int8_t) -1, unsign_arg0_22112);
                            
                            for (int64_t i_23426 = 0; i_23426 < b_16485; i_23426++) {
                                int8_t eta_p_22481 = ((int8_t *) ks1_mem_23557.mem)[i_23440 * b_16485 + i_23426];
                                int8_t eta_p_22482 = ((int8_t *) ks2_mem_23559.mem)[ext_23618 + i_23426];
                                bool cond_22484 = sle64(i_23426, i32_res_22100);
                                bool cond_t_res_22485 = sle64(min_res_22105, i_23426);
                                bool x_22486 = cond_22484 && cond_t_res_22485;
                                int8_t lifted_lambda_res_22487;
                                
                                if (x_22486) {
                                    lifted_lambda_res_22487 = eta_p_22481;
                                } else {
                                    lifted_lambda_res_22487 = (int8_t) 0;
                                }
                                
                                int8_t lifted_lambda_res_22492;
                                
                                if (x_22486) {
                                    lifted_lambda_res_22492 = eta_p_22482;
                                } else {
                                    lifted_lambda_res_22492 = (int8_t) 0;
                                }
                                ((int8_t *) mem_23641)[i_23426] = lifted_lambda_res_22492;
                                ((int8_t *) mem_23642)[i_23426] = lifted_lambda_res_22487;
                            }
                            
                            int8_t za_lhs_22125 = ((int8_t *) mem_23642)[i32_res_22100];
                            int8_t mod1_x_22126 = first_bitMask_22109 & za_lhs_22125;
                            
                            ((int8_t *) mem_23642)[i32_res_22100] = mod1_x_22126;
                            
                            int8_t za_lhs_22132 = ((int8_t *) mem_23642)[min_res_22105];
                            int8_t tmp_22133 = unsign_arg0_22113 & za_lhs_22132;
                            
                            ((int8_t *) mem_23642)[min_res_22105] = tmp_22133;
                            
                            int8_t za_lhs_22142 = ((int8_t *) mem_23641)[i32_res_22100];
                            int8_t mod1_x_22143 = first_bitMask_22109 & za_lhs_22142;
                            
                            ((int8_t *) mem_23641)[i32_res_22100] = mod1_x_22143;
                            
                            int8_t za_lhs_22145 = ((int8_t *) mem_23641)[min_res_22105];
                            int8_t tmp_22146 = unsign_arg0_22113 & za_lhs_22145;
                            
                            ((int8_t *) mem_23641)[min_res_22105] = tmp_22146;
                            
                            int32_t zm_rhs_22148 = sdiv32(byteSeq_lt_arg1_22094, 8);
                            int32_t zm_lhs_22149 = sub32(i64_res_20350, zm_rhs_22148);
                            int32_t i32_arg0_22150 = sub32(zm_lhs_22149, 1);
                            int64_t i32_res_22151 = sext_i32_i64(i32_arg0_22150);
                            bool loop_cond_t_res_22152 = sle64(i32_res_22151, i32_res_22100);
                            bool byteSeq_lt_res_22153;
                            int64_t byteSeq_lt_res_22154;
                            bool byteSeq_lt_res_22155;
                            bool loop_while_22156;
                            int64_t p_22157;
                            bool p_22158;
                            
                            loop_while_22156 = loop_cond_t_res_22152;
                            p_22157 = i32_res_22151;
                            p_22158 = 0;
                            while (loop_while_22156) {
                                bool x_22159 = sle64((int64_t) 0, p_22157);
                                bool y_22160 = slt64(p_22157, b_16485);
                                bool bounds_check_22161 = x_22159 && y_22160;
                                bool index_certs_22162;
                                
                                if (!bounds_check_22161) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) p_22157, "] out of bounds for array of shape [", (long long) b_16485, "].", "-> #0  ../../../joins/ftHashJoin.fut:89:19-29\n   #1  ../../../joins/ftHashJoin.fut:192:19-85\n   #2  ../../../joins/ftHashJoin.fut:452:22-61\n   #3  /prelude/soacs.fut:83:33-34\n   #4  /prelude/soacs.fut:83:37-53\n   #5  test_join1.fut:16:1-29:36\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int8_t zl_lhs_22163 = ((int8_t *) mem_23642)[p_22157];
                                int8_t zl_rhs_22164 = ((int8_t *) mem_23641)[p_22157];
                                bool dlt_22165 = ult8(zl_lhs_22163, zl_rhs_22164);
                                bool dgt_22166 = ult8(zl_rhs_22164, zl_lhs_22163);
                                bool tmp_22167 = dlt_22165 || dgt_22166;
                                bool cond_22168 = !tmp_22167;
                                int64_t tmp_22169;
                                
                                if (cond_22168) {
                                    int64_t tmp_t_res_23288 = add64((int64_t) 1, p_22157);
                                    
                                    tmp_22169 = tmp_t_res_23288;
                                } else {
                                    int64_t tmp_f_res_22171 = add64((int64_t) 1, i32_res_22100);
                                    
                                    tmp_22169 = tmp_f_res_22171;
                                }
                                
                                bool cond_22172 = !dlt_22165;
                                bool loop_cond_t_res_22173 = sle64(tmp_22169, i32_res_22100);
                                bool x_22174 = cond_22172 && loop_cond_t_res_22173;
                                bool loop_while_tmp_23962 = x_22174;
                                int64_t p_tmp_23963 = tmp_22169;
                                bool p_tmp_23964 = dlt_22165;
                                
                                loop_while_22156 = loop_while_tmp_23962;
                                p_22157 = p_tmp_23963;
                                p_22158 = p_tmp_23964;
                            }
                            byteSeq_lt_res_22153 = loop_while_22156;
                            byteSeq_lt_res_22154 = p_22157;
                            byteSeq_lt_res_22155 = p_22158;
                            
                            bool byteSeq_gt_res_22175;
                            int64_t byteSeq_gt_res_22176;
                            bool byteSeq_gt_res_22177;
                            bool loop_while_22178;
                            int64_t p_22179;
                            bool p_22180;
                            
                            loop_while_22178 = loop_cond_t_res_22152;
                            p_22179 = i32_res_22151;
                            p_22180 = 0;
                            while (loop_while_22178) {
                                bool x_22181 = sle64((int64_t) 0, p_22179);
                                bool y_22182 = slt64(p_22179, b_16485);
                                bool bounds_check_22183 = x_22181 && y_22182;
                                bool index_certs_22184;
                                
                                if (!bounds_check_22183) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) p_22179, "] out of bounds for array of shape [", (long long) b_16485, "].", "-> #0  ../../../joins/ftHashJoin.fut:126:19-29\n   #1  ../../../joins/ftHashJoin.fut:193:19-85\n   #2  ../../../joins/ftHashJoin.fut:452:22-61\n   #3  /prelude/soacs.fut:83:33-34\n   #4  /prelude/soacs.fut:83:37-53\n   #5  test_join1.fut:16:1-29:36\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int8_t zl_lhs_22185 = ((int8_t *) mem_23642)[p_22179];
                                int8_t zl_rhs_22186 = ((int8_t *) mem_23641)[p_22179];
                                bool dlt_22187 = ult8(zl_lhs_22185, zl_rhs_22186);
                                bool dgt_22188 = ult8(zl_rhs_22186, zl_lhs_22185);
                                bool tmp_22189 = dlt_22187 || dgt_22188;
                                bool cond_22190 = !tmp_22189;
                                int64_t tmp_22191;
                                
                                if (cond_22190) {
                                    int64_t tmp_t_res_23289 = add64((int64_t) 1, p_22179);
                                    
                                    tmp_22191 = tmp_t_res_23289;
                                } else {
                                    int64_t tmp_f_res_22193 = add64((int64_t) 1, i32_res_22100);
                                    
                                    tmp_22191 = tmp_f_res_22193;
                                }
                                
                                bool cond_22194 = !dgt_22188;
                                bool loop_cond_t_res_22195 = sle64(tmp_22191, i32_res_22100);
                                bool x_22196 = cond_22194 && loop_cond_t_res_22195;
                                bool loop_while_tmp_23965 = x_22196;
                                int64_t p_tmp_23966 = tmp_22191;
                                bool p_tmp_23967 = dgt_22188;
                                
                                loop_while_22178 = loop_while_tmp_23965;
                                p_22179 = p_tmp_23966;
                                p_22180 = p_tmp_23967;
                            }
                            byteSeq_gt_res_22175 = loop_while_22178;
                            byteSeq_gt_res_22176 = p_22179;
                            byteSeq_gt_res_22177 = p_22180;
                            
                            bool tmp_22197 = byteSeq_lt_res_22155 || byteSeq_gt_res_22177;
                            bool cond_22198 = !tmp_22197;
                            int32_t tmp_22199;
                            
                            if (cond_22198) {
                                tmp_22199 = zt_rhs_22092;
                            } else {
                                tmp_22199 = lifted_lambda_res_21874;
                            }
                            
                            bool cond_22200 = !byteSeq_gt_res_22177;
                            bool loop_cond_t_res_22201 = slt32(tmp_22199, lifted_lambda_res_21874);
                            bool x_22202 = cond_22200 && loop_cond_t_res_22201;
                            bool loop_while_tmp_23957 = x_22202;
                            int32_t p_tmp_23958 = tmp_22199;
                            bool p_tmp_23959 = byteSeq_gt_res_22177;
                            
                            loop_while_22089 = loop_while_tmp_23957;
                            p_22090 = p_tmp_23958;
                            p_22091 = p_tmp_23959;
                        }
                        radix_gt_res_22086 = loop_while_22089;
                        radix_gt_res_22087 = p_22090;
                        radix_gt_res_22088 = p_22091;
                        
                        int64_t lifted_lambda_res_f_res_f_res_22203;
                        
                        if (radix_gt_res_22088) {
                            bool eq_x_y_23292 = eta_p_21864 == (int64_t) -1;
                            bool eq_x_zz_23293 = eta_p_21864 == eta_p_21861;
                            bool p_and_eq_x_y_23294 = cond_21869 && eq_x_y_23292;
                            bool p_and_eq_x_y_23296 = eq_x_zz_23293 && x_23921;
                            bool cond_23297 = p_and_eq_x_y_23294 || p_and_eq_x_y_23296;
                            int64_t lifted_lambda_res_f_res_f_res_t_res_23298;
                            
                            if (cond_23297) {
                                lifted_lambda_res_f_res_f_res_t_res_23298 = (int64_t) -1;
                            } else {
                                int64_t min_arg1_23299 = add64(max_res_21866, lifted_lambda_res_21870);
                                int64_t min_res_23300 = smin64(eta_p_21864, min_arg1_23299);
                                
                                lifted_lambda_res_f_res_f_res_t_res_23298 = min_res_23300;
                            }
                            lifted_lambda_res_f_res_f_res_22203 = lifted_lambda_res_f_res_f_res_t_res_23298;
                        } else {
                            bool eq_x_zz_22342 = (int64_t) 0 == eta_p_21861;
                            bool p_and_eq_x_y_22345 = eq_x_zz_22342 && x_23921;
                            bool loop_cond_t_res_22210 = slt32(0, lifted_lambda_res_21873);
                            bool cond_22209;
                            
                            if (p_and_eq_x_y_22345) {
                                cond_22209 = 1;
                            } else {
                                if (mem_23655_cached_sizze_24031 < b_16485) {
                                    err = lexical_realloc(ctx, &mem_23655, &mem_23655_cached_sizze_24031, b_16485);
                                    if (err != FUTHARK_SUCCESS)
                                        goto cleanup;
                                }
                                if (mem_23656_cached_sizze_24032 < b_16485) {
                                    err = lexical_realloc(ctx, &mem_23656, &mem_23656_cached_sizze_24032, b_16485);
                                    if (err != FUTHARK_SUCCESS)
                                        goto cleanup;
                                }
                                
                                bool radix_gt_res_22211;
                                int32_t radix_gt_res_22212;
                                bool radix_gt_res_22213;
                                bool loop_while_22214;
                                int32_t p_22215;
                                bool p_22216;
                                
                                loop_while_22214 = loop_cond_t_res_22210;
                                p_22215 = 0;
                                p_22216 = 0;
                                while (loop_while_22214) {
                                    int32_t zt_rhs_22217 = add32(1, p_22215);
                                    int32_t zm_lhs_22218 = mul32(f64_res_19773, zt_rhs_22217);
                                    int32_t byteSeq_lt_arg1_22219 = sub32(zm_lhs_22218, 1);
                                    int32_t byteSeq_lt_arg0_22220 = mul32(f64_res_19773, p_22215);
                                    int32_t min_res_22221 = smin32(min_arg1_20352, byteSeq_lt_arg1_22219);
                                    int32_t zm_rhs_22222 = sdiv32(byteSeq_lt_arg0_22220, 8);
                                    int32_t zm_lhs_22223 = sub32(i64_res_20350, zm_rhs_22222);
                                    int32_t i32_arg0_22224 = sub32(zm_lhs_22223, 1);
                                    int64_t i32_res_22225 = sext_i32_i64(i32_arg0_22224);
                                    int32_t zm_rhs_22226 = sdiv32(min_res_22221, 8);
                                    int32_t zm_lhs_22227 = sub32(i64_res_20350, zm_rhs_22226);
                                    int32_t i32_arg0_22228 = sub32(zm_lhs_22227, 1);
                                    int64_t i32_res_22229 = sext_i32_i64(i32_arg0_22228);
                                    int64_t min_res_22230 = smin64(min_arg1_20365, i32_res_22229);
                                    bool x_22246 = sle64((int64_t) 0, i32_res_22225);
                                    bool y_22247 = slt64(i32_res_22225, b_16485);
                                    bool bounds_check_22248 = x_22246 && y_22247;
                                    bool index_certs_22249;
                                    
                                    if (!bounds_check_22248) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i32_res_22225, "] out of bounds for array of shape [", (long long) b_16485, "].", "-> #0  ../../../joins/ftHashJoin.fut:32:50-66\n   #1  test_join1.fut:16:1-29:36\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    bool x_22253 = sle64((int64_t) 0, min_res_22230);
                                    bool y_22254 = slt64(min_res_22230, b_16485);
                                    bool bounds_check_22255 = x_22253 && y_22254;
                                    bool index_certs_22256;
                                    
                                    if (!bounds_check_22255) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) min_res_22230, "] out of bounds for array of shape [", (long long) b_16485, "].", "-> #0  ../../../joins/ftHashJoin.fut:33:40-56\n   #1  test_join1.fut:16:1-29:36\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    int32_t firstBit_22231 = smod32(byteSeq_lt_arg0_22220, 8);
                                    int32_t lastBit_22232 = smod32(min_res_22221, 8);
                                    int8_t unsign_arg0_22233 = zext_i32_i8(firstBit_22231);
                                    int8_t first_bitMask_22234 = shl8((int8_t) -1, unsign_arg0_22233);
                                    int32_t zm_lhs_22235 = sub32(8, lastBit_22232);
                                    int32_t i32_arg0_22236 = sub32(zm_lhs_22235, 1);
                                    int8_t unsign_arg0_22237 = zext_i32_i8(i32_arg0_22236);
                                    int8_t unsign_arg0_22238 = lshr8((int8_t) -1, unsign_arg0_22237);
                                    
                                    for (int64_t i_23433 = 0; i_23433 < b_16485; i_23433++) {
                                        int8_t eta_p_22510 = ((int8_t *) ks1_mem_23557.mem)[i_23440 * b_16485 + i_23433];
                                        int8_t eta_p_22511 = ((int8_t *) ks2_mem_23559.mem)[ext_23619 + i_23433];
                                        bool cond_22513 = sle64(i_23433, i32_res_22225);
                                        bool cond_t_res_22514 = sle64(min_res_22230, i_23433);
                                        bool x_22515 = cond_22513 && cond_t_res_22514;
                                        int8_t lifted_lambda_res_22516;
                                        
                                        if (x_22515) {
                                            lifted_lambda_res_22516 = eta_p_22510;
                                        } else {
                                            lifted_lambda_res_22516 = (int8_t) 0;
                                        }
                                        
                                        int8_t lifted_lambda_res_22521;
                                        
                                        if (x_22515) {
                                            lifted_lambda_res_22521 = eta_p_22511;
                                        } else {
                                            lifted_lambda_res_22521 = (int8_t) 0;
                                        }
                                        ((int8_t *) mem_23655)[i_23433] = lifted_lambda_res_22521;
                                        ((int8_t *) mem_23656)[i_23433] = lifted_lambda_res_22516;
                                    }
                                    
                                    int8_t za_lhs_22250 = ((int8_t *) mem_23656)[i32_res_22225];
                                    int8_t mod1_x_22251 = first_bitMask_22234 & za_lhs_22250;
                                    
                                    ((int8_t *) mem_23656)[i32_res_22225] = mod1_x_22251;
                                    
                                    int8_t za_lhs_22257 = ((int8_t *) mem_23656)[min_res_22230];
                                    int8_t tmp_22258 = unsign_arg0_22238 & za_lhs_22257;
                                    
                                    ((int8_t *) mem_23656)[min_res_22230] = tmp_22258;
                                    
                                    int8_t za_lhs_22267 = ((int8_t *) mem_23655)[i32_res_22225];
                                    int8_t mod1_x_22268 = first_bitMask_22234 & za_lhs_22267;
                                    
                                    ((int8_t *) mem_23655)[i32_res_22225] = mod1_x_22268;
                                    
                                    int8_t za_lhs_22270 = ((int8_t *) mem_23655)[min_res_22230];
                                    int8_t tmp_22271 = unsign_arg0_22238 & za_lhs_22270;
                                    
                                    ((int8_t *) mem_23655)[min_res_22230] = tmp_22271;
                                    
                                    int32_t zm_rhs_22273 = sdiv32(byteSeq_lt_arg1_22219, 8);
                                    int32_t zm_lhs_22274 = sub32(i64_res_20350, zm_rhs_22273);
                                    int32_t i32_arg0_22275 = sub32(zm_lhs_22274, 1);
                                    int64_t i32_res_22276 = sext_i32_i64(i32_arg0_22275);
                                    bool loop_cond_t_res_22277 = sle64(i32_res_22276, i32_res_22225);
                                    bool byteSeq_lt_res_22278;
                                    int64_t byteSeq_lt_res_22279;
                                    bool byteSeq_lt_res_22280;
                                    bool loop_while_22281;
                                    int64_t p_22282;
                                    bool p_22283;
                                    
                                    loop_while_22281 = loop_cond_t_res_22277;
                                    p_22282 = i32_res_22276;
                                    p_22283 = 0;
                                    while (loop_while_22281) {
                                        bool x_22284 = sle64((int64_t) 0, p_22282);
                                        bool y_22285 = slt64(p_22282, b_16485);
                                        bool bounds_check_22286 = x_22284 && y_22285;
                                        bool index_certs_22287;
                                        
                                        if (!bounds_check_22286) {
                                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) p_22282, "] out of bounds for array of shape [", (long long) b_16485, "].", "-> #0  ../../../joins/ftHashJoin.fut:89:19-29\n   #1  ../../../joins/ftHashJoin.fut:192:19-85\n   #2  ../../../joins/ftHashJoin.fut:457:27-68\n   #3  /prelude/soacs.fut:83:33-34\n   #4  /prelude/soacs.fut:83:37-53\n   #5  test_join1.fut:16:1-29:36\n"));
                                            err = FUTHARK_PROGRAM_ERROR;
                                            goto cleanup;
                                        }
                                        
                                        int8_t zl_lhs_22288 = ((int8_t *) mem_23656)[p_22282];
                                        int8_t zl_rhs_22289 = ((int8_t *) mem_23655)[p_22282];
                                        bool dlt_22290 = ult8(zl_lhs_22288, zl_rhs_22289);
                                        bool dgt_22291 = ult8(zl_rhs_22289, zl_lhs_22288);
                                        bool tmp_22292 = dlt_22290 || dgt_22291;
                                        bool cond_22293 = !tmp_22292;
                                        int64_t tmp_22294;
                                        
                                        if (cond_22293) {
                                            int64_t tmp_t_res_23301 = add64((int64_t) 1, p_22282);
                                            
                                            tmp_22294 = tmp_t_res_23301;
                                        } else {
                                            int64_t tmp_f_res_22296 = add64((int64_t) 1, i32_res_22225);
                                            
                                            tmp_22294 = tmp_f_res_22296;
                                        }
                                        
                                        bool cond_22297 = !dlt_22290;
                                        bool loop_cond_t_res_22298 = sle64(tmp_22294, i32_res_22225);
                                        bool x_22299 = cond_22297 && loop_cond_t_res_22298;
                                        bool loop_while_tmp_23973 = x_22299;
                                        int64_t p_tmp_23974 = tmp_22294;
                                        bool p_tmp_23975 = dlt_22290;
                                        
                                        loop_while_22281 = loop_while_tmp_23973;
                                        p_22282 = p_tmp_23974;
                                        p_22283 = p_tmp_23975;
                                    }
                                    byteSeq_lt_res_22278 = loop_while_22281;
                                    byteSeq_lt_res_22279 = p_22282;
                                    byteSeq_lt_res_22280 = p_22283;
                                    
                                    bool byteSeq_gt_res_22300;
                                    int64_t byteSeq_gt_res_22301;
                                    bool byteSeq_gt_res_22302;
                                    bool loop_while_22303;
                                    int64_t p_22304;
                                    bool p_22305;
                                    
                                    loop_while_22303 = loop_cond_t_res_22277;
                                    p_22304 = i32_res_22276;
                                    p_22305 = 0;
                                    while (loop_while_22303) {
                                        bool x_22306 = sle64((int64_t) 0, p_22304);
                                        bool y_22307 = slt64(p_22304, b_16485);
                                        bool bounds_check_22308 = x_22306 && y_22307;
                                        bool index_certs_22309;
                                        
                                        if (!bounds_check_22308) {
                                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) p_22304, "] out of bounds for array of shape [", (long long) b_16485, "].", "-> #0  ../../../joins/ftHashJoin.fut:126:19-29\n   #1  ../../../joins/ftHashJoin.fut:193:19-85\n   #2  ../../../joins/ftHashJoin.fut:457:27-68\n   #3  /prelude/soacs.fut:83:33-34\n   #4  /prelude/soacs.fut:83:37-53\n   #5  test_join1.fut:16:1-29:36\n"));
                                            err = FUTHARK_PROGRAM_ERROR;
                                            goto cleanup;
                                        }
                                        
                                        int8_t zl_lhs_22310 = ((int8_t *) mem_23656)[p_22304];
                                        int8_t zl_rhs_22311 = ((int8_t *) mem_23655)[p_22304];
                                        bool dlt_22312 = ult8(zl_lhs_22310, zl_rhs_22311);
                                        bool dgt_22313 = ult8(zl_rhs_22311, zl_lhs_22310);
                                        bool tmp_22314 = dlt_22312 || dgt_22313;
                                        bool cond_22315 = !tmp_22314;
                                        int64_t tmp_22316;
                                        
                                        if (cond_22315) {
                                            int64_t tmp_t_res_23302 = add64((int64_t) 1, p_22304);
                                            
                                            tmp_22316 = tmp_t_res_23302;
                                        } else {
                                            int64_t tmp_f_res_22318 = add64((int64_t) 1, i32_res_22225);
                                            
                                            tmp_22316 = tmp_f_res_22318;
                                        }
                                        
                                        bool cond_22319 = !dgt_22313;
                                        bool loop_cond_t_res_22320 = sle64(tmp_22316, i32_res_22225);
                                        bool x_22321 = cond_22319 && loop_cond_t_res_22320;
                                        bool loop_while_tmp_23976 = x_22321;
                                        int64_t p_tmp_23977 = tmp_22316;
                                        bool p_tmp_23978 = dgt_22313;
                                        
                                        loop_while_22303 = loop_while_tmp_23976;
                                        p_22304 = p_tmp_23977;
                                        p_22305 = p_tmp_23978;
                                    }
                                    byteSeq_gt_res_22300 = loop_while_22303;
                                    byteSeq_gt_res_22301 = p_22304;
                                    byteSeq_gt_res_22302 = p_22305;
                                    
                                    bool tmp_22322 = byteSeq_lt_res_22280 || byteSeq_gt_res_22302;
                                    bool cond_22323 = !tmp_22322;
                                    int32_t tmp_22324;
                                    
                                    if (cond_22323) {
                                        tmp_22324 = zt_rhs_22217;
                                    } else {
                                        tmp_22324 = lifted_lambda_res_21873;
                                    }
                                    
                                    bool cond_22325 = !byteSeq_gt_res_22302;
                                    bool loop_cond_t_res_22326 = slt32(tmp_22324, lifted_lambda_res_21873);
                                    bool x_22327 = cond_22325 && loop_cond_t_res_22326;
                                    bool loop_while_tmp_23968 = x_22327;
                                    int32_t p_tmp_23969 = tmp_22324;
                                    bool p_tmp_23970 = byteSeq_gt_res_22302;
                                    
                                    loop_while_22214 = loop_while_tmp_23968;
                                    p_22215 = p_tmp_23969;
                                    p_22216 = p_tmp_23970;
                                }
                                radix_gt_res_22211 = loop_while_22214;
                                radix_gt_res_22212 = p_22215;
                                radix_gt_res_22213 = p_22216;
                                cond_22209 = radix_gt_res_22213;
                            }
                            
                            int64_t lifted_lambda_res_f_res_f_res_f_res_22328;
                            
                            if (cond_22209) {
                                lifted_lambda_res_f_res_f_res_f_res_22328 = (int64_t) -1;
                            } else {
                                int64_t max_arg1_22329 = sub64(lifted_lambda_res_21870, max_res_21866);
                                int64_t max_res_22330 = smax64(eta_p_21862, max_arg1_22329);
                                
                                lifted_lambda_res_f_res_f_res_f_res_22328 = max_res_22330;
                            }
                            lifted_lambda_res_f_res_f_res_22203 = lifted_lambda_res_f_res_f_res_f_res_22328;
                        }
                        lifted_lambda_res_f_res_22007 = lifted_lambda_res_f_res_f_res_22203;
                    }
                    lifted_lambda_res_21930 = lifted_lambda_res_f_res_22007;
                }
                ((int64_t *) mem_23605.mem)[i_23440] = lifted_lambda_res_21930;
                ((int64_t *) mem_23607.mem)[i_23440] = max_res_21866;
            }
            if (memblock_set(ctx, &mem_param_tmp_23943, &mem_23605, "mem_23605") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_tmp_23944, &mem_23607, "mem_23607") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_23600, &mem_param_tmp_23943, "mem_param_tmp_23943") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_23603, &mem_param_tmp_23944, "mem_param_tmp_23944") != 0)
                return 1;
        }
        if (memblock_set(ctx, &ext_mem_23701, &mem_param_23600, "mem_param_23600") != 0)
            return 1;
        if (memblock_set(ctx, &ext_mem_23700, &mem_param_23603, "mem_param_23603") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_23589, "mem_23589") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_23597, "mem_23597") != 0)
            return 1;
        if (memblock_set(ctx, &ext_mem_23704, &ext_mem_23701, "ext_mem_23701") != 0)
            return 1;
    }
    if (memblock_unref(ctx, &mem_23566, "mem_23566") != 0)
        return 1;
    if (mem_23706_cached_sizze_24036 < bytes_23565) {
        err = lexical_realloc(ctx, &mem_23706, &mem_23706_cached_sizze_24036, bytes_23565);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    
    int64_t defunc_0_reduce_res_23373;
    int64_t redout_23444 = (int64_t) -9223372036854775808;
    
    for (int64_t i_23446 = 0; i_23446 < n1_16484; i_23446++) {
        int64_t eta_p_21324 = ((int64_t *) ext_mem_23704.mem)[i_23446];
        bool cond_21326 = slt64(eta_p_21324, (int64_t) 0);
        int64_t lifted_lambda_res_21327;
        int64_t lifted_lambda_res_21328;
        
        if (cond_21326) {
            lifted_lambda_res_21327 = (int64_t) -1;
            lifted_lambda_res_21328 = (int64_t) -1;
        } else {
            bool x_21329 = sle64((int64_t) 0, eta_p_21324);
            bool y_21330 = slt64(eta_p_21324, dz2082U_16489);
            bool bounds_check_21331 = x_21329 && y_21330;
            bool index_certs_21332;
            
            if (!bounds_check_21331) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_21324, "] out of bounds for array of shape [", (long long) dz2082U_16489, "].", "-> #0  ../../../joins/ftHashJoin.fut:465:47-59\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin.fut:465:10-103\n   #3  test_join1.fut:16:1-29:36\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t tmp_21333 = ((int64_t *) bounds2_mem_23561.mem)[eta_p_21324];
            int64_t zeze_rhs_21334 = sub64(dz2082U_16489, (int64_t) 1);
            bool cond_21335 = eta_p_21324 == zeze_rhs_21334;
            int64_t tmp_21336;
            
            if (cond_21335) {
                tmp_21336 = n2_16487;
            } else {
                int64_t tmp_21337 = add64((int64_t) 1, eta_p_21324);
                bool x_21338 = sle64((int64_t) 0, tmp_21337);
                bool y_21339 = slt64(tmp_21337, dz2082U_16489);
                bool bounds_check_21340 = x_21338 && y_21339;
                bool index_certs_21341;
                
                if (!bounds_check_21340) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_21337, "] out of bounds for array of shape [", (long long) dz2082U_16489, "].", "-> #0  ../../../joins/ftHashJoin.fut:465:87-101\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin.fut:465:10-103\n   #3  test_join1.fut:16:1-29:36\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                int64_t tmp_f_res_21342 = ((int64_t *) bounds2_mem_23561.mem)[tmp_21337];
                
                tmp_21336 = tmp_f_res_21342;
            }
            lifted_lambda_res_21327 = tmp_21333;
            lifted_lambda_res_21328 = tmp_21336;
        }
        
        int64_t lifted_lambda_res_21345 = sub64(lifted_lambda_res_21328, lifted_lambda_res_21327);
        int64_t lifted_lambda_res_21348;
        
        if (cond_21326) {
            lifted_lambda_res_21348 = (int64_t) -1;
        } else {
            bool x_21349 = sle64((int64_t) 0, eta_p_21324);
            bool y_21350 = slt64(eta_p_21324, dz2082U_16489);
            bool bounds_check_21351 = x_21349 && y_21350;
            bool index_certs_21352;
            
            if (!bounds_check_21351) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_21324, "] out of bounds for array of shape [", (long long) dz2082U_16489, "].", "-> #0  ../../../joins/ftHashJoin.fut:468:79-91\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin.fut:468:46-92\n   #3  test_join1.fut:16:1-29:36\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t lifted_lambda_res_f_res_21353 = ((int64_t *) bounds2_mem_23561.mem)[eta_p_21324];
            
            lifted_lambda_res_21348 = lifted_lambda_res_f_res_21353;
        }
        
        int64_t max_res_20148 = smax64(lifted_lambda_res_21345, redout_23444);
        
        ((int64_t *) mem_23706)[i_23446] = lifted_lambda_res_21348;
        
        int64_t redout_tmp_23979 = max_res_20148;
        
        redout_23444 = redout_tmp_23979;
    }
    defunc_0_reduce_res_23373 = redout_23444;
    if (memblock_unref(ctx, &ext_mem_23704, "ext_mem_23704") != 0)
        return 1;
    if (memblock_alloc(ctx, &mem_23714, bytes_23565, "mem_23714")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t nest_i_23981 = 0; nest_i_23981 < n1_16484; nest_i_23981++) {
        ((int64_t *) mem_23714.mem)[nest_i_23981] = (int64_t) 0;
    }
    
    bool bounds_invalid_upwards_20159 = slt64(defunc_0_reduce_res_23373, (int64_t) 0);
    bool valid_20160 = !bounds_invalid_upwards_20159;
    bool range_valid_c_20161;
    
    if (!valid_20160) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) defunc_0_reduce_res_23373, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  ../../../joins/ftHashJoin.fut:472:17-40\n   #2  test_join1.fut:16:1-29:36\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int32_t zm_rhs_21024 = sdiv32(min_arg1_20352, 8);
    int32_t zm_lhs_21025 = sub32(i64_res_20350, zm_rhs_21024);
    int32_t i32_arg0_21026 = sub32(zm_lhs_21025, 1);
    int64_t i32_res_21027 = sext_i32_i64(i32_arg0_21026);
    int64_t j_m_i_21033 = sub64(map2_arg2_20883, i32_res_21027);
    
    if (memblock_set(ctx, &mem_param_23717, &mem_23714, "mem_23714") != 0)
        return 1;
    for (int64_t i_20163 = 0; i_20163 < defunc_0_reduce_res_23373; i_20163++) {
        if (memblock_alloc(ctx, &mem_23719, bytes_23565, "mem_23719")) {
            err = 1;
            goto cleanup;
        }
        for (int64_t i_23461 = 0; i_23461 < n1_16484; i_23461++) {
            int64_t eta_p_20166 = ((int64_t *) mem_23706)[i_23461];
            int64_t eta_p_20167 = ((int64_t *) mem_param_23717.mem)[i_23461];
            bool cond_20169 = slt64(eta_p_20166, (int64_t) 0);
            int64_t zgze_lhs_20170 = add64(i_20163, eta_p_20166);
            bool cond_f_res_20171 = sle64(n2_16487, zgze_lhs_20170);
            bool x_20172 = !cond_20169;
            bool y_20173 = cond_f_res_20171 && x_20172;
            bool cond_20174 = cond_20169 || y_20173;
            int64_t lifted_lambda_res_20175;
            
            if (cond_20174) {
                lifted_lambda_res_20175 = eta_p_20167;
            } else {
                bool x_20176 = sle64((int64_t) 0, zgze_lhs_20170);
                bool y_20177 = slt64(zgze_lhs_20170, n2_16487);
                bool bounds_check_20178 = x_20176 && y_20177;
                bool index_certs_20179;
                
                if (!bounds_check_20178) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) zgze_lhs_20170, "] out of bounds for array of shape [", (long long) n2_16487, "].", "-> #0  ../../../joins/ftHashJoin.fut:475:20-27\n   #1  /prelude/soacs.fut:75:28-29\n   #2  test_join1.fut:16:1-29:36\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                int32_t min_arg1_20915 = sub32(zm_lhs_20351, 1);
                int32_t min_res_20916 = smin32(min_arg1_20352, min_arg1_20915);
                int32_t zm_rhs_20923 = sdiv32(min_res_20916, 8);
                int32_t zm_lhs_20924 = sub32(i64_res_20350, zm_rhs_20923);
                int32_t i32_arg0_20925 = sub32(zm_lhs_20924, 1);
                int64_t i32_res_20926 = sext_i32_i64(i32_arg0_20925);
                int64_t min_res_20929 = smin64(min_arg1_20365, i32_res_20926);
                int64_t i32_res_20921 = sext_i32_i64(i32_arg0_20357);
                bool x_20946 = sle64((int64_t) 0, i32_res_20921);
                bool y_20947 = slt64(i32_res_20921, b_16485);
                bool bounds_check_20948 = x_20946 && y_20947;
                bool index_certs_20949;
                
                if (!bounds_check_20948) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i32_res_20921, "] out of bounds for array of shape [", (long long) b_16485, "].", "-> #0  ../../../joins/ftHashJoin.fut:32:50-66\n   #1  test_join1.fut:16:1-29:36\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                bool x_20957 = sle64((int64_t) 0, min_res_20929);
                bool y_20958 = slt64(min_res_20929, b_16485);
                bool bounds_check_20959 = x_20957 && y_20958;
                bool index_certs_20960;
                
                if (!bounds_check_20959) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) min_res_20929, "] out of bounds for array of shape [", (long long) b_16485, "].", "-> #0  ../../../joins/ftHashJoin.fut:33:40-56\n   #1  test_join1.fut:16:1-29:36\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                bool empty_slice_21034 = j_m_i_21033 == (int64_t) 0;
                int64_t m_21035 = sub64(j_m_i_21033, (int64_t) 1);
                int64_t i_p_m_t_s_21036 = add64(i32_res_21027, m_21035);
                bool zzero_leq_i_p_m_t_s_21037 = sle64((int64_t) 0, i_p_m_t_s_21036);
                bool i_p_m_t_s_leq_w_21038 = slt64(i_p_m_t_s_21036, b_16485);
                bool zzero_lte_i_21039 = sle64((int64_t) 0, i32_res_21027);
                bool i_lte_j_21040 = sle64(i32_res_21027, map2_arg2_20883);
                bool y_21041 = i_p_m_t_s_leq_w_21038 && zzero_lte_i_21039;
                bool y_21042 = zzero_leq_i_p_m_t_s_21037 && y_21041;
                bool forwards_ok_21043 = i_lte_j_21040 && y_21042;
                bool ok_or_empty_21044 = empty_slice_21034 || forwards_ok_21043;
                bool index_certs_21045;
                
                if (!ok_or_empty_21044) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i32_res_21027, ":", (long long) map2_arg2_20883, "] out of bounds for array of shape [", (long long) b_16485, "].", "-> #0  ../../../joins/ftHashJoin.fut:51:49-60\n   #1  test_join1.fut:16:1-29:36\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                if (mem_23723_cached_sizze_24037 < b_16485) {
                    err = lexical_realloc(ctx, &mem_23723, &mem_23723_cached_sizze_24037, b_16485);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                if (mem_23724_cached_sizze_24038 < b_16485) {
                    err = lexical_realloc(ctx, &mem_23724, &mem_23724_cached_sizze_24038, b_16485);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                for (int64_t i_23452 = 0; i_23452 < b_16485; i_23452++) {
                    int8_t eta_p_23044 = ((int8_t *) ks1_mem_23557.mem)[i_23461 * b_16485 + i_23452];
                    int8_t eta_p_23045 = ((int8_t *) ks2_mem_23559.mem)[zgze_lhs_20170 * b_16485 + i_23452];
                    bool cond_23047 = sle64(i_23452, i32_res_20921);
                    bool cond_t_res_23048 = sle64(min_res_20929, i_23452);
                    bool x_23049 = cond_23047 && cond_t_res_23048;
                    int8_t lifted_lambda_res_23050;
                    
                    if (x_23049) {
                        lifted_lambda_res_23050 = eta_p_23045;
                    } else {
                        lifted_lambda_res_23050 = (int8_t) 0;
                    }
                    
                    int8_t lifted_lambda_res_23055;
                    
                    if (x_23049) {
                        lifted_lambda_res_23055 = eta_p_23044;
                    } else {
                        lifted_lambda_res_23055 = (int8_t) 0;
                    }
                    ((int8_t *) mem_23723)[i_23452] = lifted_lambda_res_23055;
                    ((int8_t *) mem_23724)[i_23452] = lifted_lambda_res_23050;
                }
                
                int32_t lastBit_20931 = smod32(min_res_20916, 8);
                int32_t zm_lhs_20934 = sub32(8, lastBit_20931);
                int32_t i32_arg0_20935 = sub32(zm_lhs_20934, 1);
                int8_t unsign_arg0_20936 = zext_i32_i8(i32_arg0_20935);
                int8_t unsign_arg0_20937 = lshr8((int8_t) -1, unsign_arg0_20936);
                int8_t za_lhs_20950 = ((int8_t *) mem_23723)[i32_res_20921];
                int8_t mod1_x_20951 = (int8_t) -1 & za_lhs_20950;
                
                ((int8_t *) mem_23723)[i32_res_20921] = mod1_x_20951;
                
                int8_t za_lhs_20961 = ((int8_t *) mem_23723)[min_res_20929];
                int8_t tmp_20962 = unsign_arg0_20937 & za_lhs_20961;
                
                ((int8_t *) mem_23723)[min_res_20929] = tmp_20962;
                
                int8_t za_lhs_21005 = ((int8_t *) mem_23724)[i32_res_20921];
                int8_t mod1_x_21006 = (int8_t) -1 & za_lhs_21005;
                
                ((int8_t *) mem_23724)[i32_res_20921] = mod1_x_21006;
                
                int8_t za_lhs_21016 = ((int8_t *) mem_23724)[min_res_20929];
                int8_t tmp_21017 = unsign_arg0_20937 & za_lhs_21016;
                
                ((int8_t *) mem_23724)[min_res_20929] = tmp_21017;
                if (mem_23737_cached_sizze_24039 < j_m_i_21033) {
                    err = lexical_realloc(ctx, &mem_23737, &mem_23737_cached_sizze_24039, j_m_i_21033);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                for (int64_t i_23457 = 0; i_23457 < j_m_i_21033; i_23457++) {
                    int64_t slice_23540 = i32_res_21027 + i_23457;
                    int8_t eta_p_21049 = ((int8_t *) mem_23723)[slice_23540];
                    int8_t eta_p_21050 = ((int8_t *) mem_23724)[slice_23540];
                    bool defunc_0_f_res_21051 = eta_p_21049 == eta_p_21050;
                    
                    ((bool *) mem_23737)[i_23457] = defunc_0_f_res_21051;
                }
                
                bool defunc_0_foldl_res_21052;
                bool acc_21054 = 1;
                
                for (int64_t i_21053 = 0; i_21053 < j_m_i_21033; i_21053++) {
                    bool b_21055 = ((bool *) mem_23737)[i_21053];
                    bool x_21056 = acc_21054 && b_21055;
                    bool acc_tmp_23988 = x_21056;
                    
                    acc_21054 = acc_tmp_23988;
                }
                defunc_0_foldl_res_21052 = acc_21054;
                
                int64_t lifted_lambda_res_f_res_20185;
                
                if (defunc_0_foldl_res_21052) {
                    int64_t lifted_lambda_res_f_res_t_res_23314 = add64((int64_t) 1, eta_p_20167);
                    
                    lifted_lambda_res_f_res_20185 = lifted_lambda_res_f_res_t_res_23314;
                } else {
                    lifted_lambda_res_f_res_20185 = eta_p_20167;
                }
                lifted_lambda_res_20175 = lifted_lambda_res_f_res_20185;
            }
            ((int64_t *) mem_23719.mem)[i_23461] = lifted_lambda_res_20175;
        }
        if (memblock_set(ctx, &mem_param_tmp_23982, &mem_23719, "mem_23719") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_23717, &mem_param_tmp_23982, "mem_param_tmp_23982") != 0)
            return 1;
    }
    if (memblock_set(ctx, &ext_mem_23749, &mem_param_23717, "mem_param_23717") != 0)
        return 1;
    if (memblock_unref(ctx, &mem_23714, "mem_23714") != 0)
        return 1;
    if (mem_23751_cached_sizze_24040 < bytes_23565) {
        err = lexical_realloc(ctx, &mem_23751, &mem_23751_cached_sizze_24040, bytes_23565);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_23753_cached_sizze_24041 < bytes_23565) {
        err = lexical_realloc(ctx, &mem_23753, &mem_23753_cached_sizze_24041, bytes_23565);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_23755_cached_sizze_24042 < bytes_23565) {
        err = lexical_realloc(ctx, &mem_23755, &mem_23755_cached_sizze_24042, bytes_23565);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    
    int64_t discard_23476;
    int64_t discard_23477;
    int64_t defunc_res_23377;
    int64_t scanacc_23466;
    int64_t scanacc_23467;
    int64_t redout_23470;
    
    scanacc_23466 = (int64_t) 0;
    scanacc_23467 = (int64_t) 0;
    redout_23470 = (int64_t) 0;
    for (int64_t i_23472 = 0; i_23472 < n1_16484; i_23472++) {
        int64_t eta_p_21359 = ((int64_t *) ext_mem_23749.mem)[i_23472];
        bool lifted_lambda_res_21362 = slt64((int64_t) 1, eta_p_21359);
        int64_t defunc_0_f_res_21363 = btoi_bool_i64(lifted_lambda_res_21362);
        int64_t defunc_0_op_res_20191 = add64(eta_p_21359, scanacc_23466);
        int64_t defunc_0_op_res_20221 = add64(defunc_0_f_res_21363, scanacc_23467);
        int64_t zp_res_20212 = add64(eta_p_21359, redout_23470);
        
        ((int64_t *) mem_23751)[i_23472] = defunc_0_op_res_20191;
        ((int64_t *) mem_23753)[i_23472] = defunc_0_op_res_20221;
        ((int64_t *) mem_23755)[i_23472] = defunc_0_f_res_21363;
        
        int64_t scanacc_tmp_23989 = defunc_0_op_res_20191;
        int64_t scanacc_tmp_23990 = defunc_0_op_res_20221;
        int64_t redout_tmp_23993 = zp_res_20212;
        
        scanacc_23466 = scanacc_tmp_23989;
        scanacc_23467 = scanacc_tmp_23990;
        redout_23470 = redout_tmp_23993;
    }
    discard_23476 = scanacc_23466;
    discard_23477 = scanacc_23467;
    defunc_res_23377 = redout_23470;
    
    bool cond_20222 = n1_16484 == (int64_t) 0;
    bool x_20223 = !cond_20222;
    int64_t tmp_20224 = sub64(n1_16484, (int64_t) 1);
    bool x_20225 = sle64((int64_t) 0, tmp_20224);
    bool y_20226 = slt64(tmp_20224, n1_16484);
    bool bounds_check_20227 = x_20225 && y_20226;
    bool protect_assert_disj_20228 = cond_20222 || bounds_check_20227;
    bool index_certs_20229;
    
    if (!protect_assert_disj_20228) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_20224, "] out of bounds for array of shape [", (long long) n1_16484, "].", "-> #0  /prelude/soacs.fut:257:33-47\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_join1.fut:16:1-29:36\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t m_f_res_20230;
    
    if (x_20223) {
        int64_t x_23322 = ((int64_t *) mem_23753)[tmp_20224];
        
        m_f_res_20230 = x_23322;
    } else {
        m_f_res_20230 = (int64_t) 0;
    }
    
    int64_t m_20232;
    
    if (cond_20222) {
        m_20232 = (int64_t) 0;
    } else {
        m_20232 = m_f_res_20230;
    }
    
    bool cond_20255 = slt64((int64_t) 0, m_20232);
    int64_t m_20242 = sub64(m_20232, (int64_t) 1);
    bool i_p_m_t_s_leq_w_20244 = slt64(m_20242, n1_16484);
    bool zzero_leq_i_p_m_t_s_20243 = sle64((int64_t) 0, m_20242);
    bool y_20246 = zzero_leq_i_p_m_t_s_20243 && i_p_m_t_s_leq_w_20244;
    bool i_lte_j_20245 = sle64((int64_t) 0, m_20232);
    bool forwards_ok_20247 = i_lte_j_20245 && y_20246;
    bool eq_x_zz_20239 = (int64_t) 0 == m_f_res_20230;
    bool p_and_eq_x_y_20240 = x_20223 && eq_x_zz_20239;
    bool empty_slice_20241 = cond_20222 || p_and_eq_x_y_20240;
    bool ok_or_empty_20248 = empty_slice_20241 || forwards_ok_20247;
    bool index_certs_20249;
    
    if (!ok_or_empty_20248) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_20232, "] out of bounds for array of shape [", (long long) n1_16484, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_join1.fut:16:1-29:36\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t bytes_23782 = (int64_t) 8 * m_20232;
    
    if (mem_23783_cached_sizze_24044 < bytes_23782) {
        err = lexical_realloc(ctx, &mem_23783, &mem_23783_cached_sizze_24044, bytes_23782);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    lmad_copy_8b(ctx, 1, (uint64_t *) mem_23783, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_23749.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_20232});
    
    int64_t max_mult_t_res_20256;
    
    if (cond_20255) {
        if (mem_23785_cached_sizze_24045 < bytes_23565) {
            err = lexical_realloc(ctx, &mem_23785, &mem_23785_cached_sizze_24045, bytes_23565);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t i_23484 = 0; i_23484 < n1_16484; i_23484++) {
            int64_t eta_p_23328 = ((int64_t *) mem_23755)[i_23484];
            int64_t eta_p_23329 = ((int64_t *) mem_23753)[i_23484];
            bool cond_23330 = eta_p_23328 == (int64_t) 1;
            int64_t lifted_lambda_res_23331;
            
            if (cond_23330) {
                int64_t lifted_lambda_res_t_res_23332 = sub64(eta_p_23329, (int64_t) 1);
                
                lifted_lambda_res_23331 = lifted_lambda_res_t_res_23332;
            } else {
                lifted_lambda_res_23331 = (int64_t) -1;
            }
            ((int64_t *) mem_23785)[i_23484] = lifted_lambda_res_23331;
        }
        for (int64_t write_iter_23486 = 0; write_iter_23486 < n1_16484; write_iter_23486++) {
            int64_t write_iv_23488 = ((int64_t *) mem_23785)[write_iter_23486];
            int64_t write_iv_23489 = ((int64_t *) ext_mem_23749.mem)[write_iter_23486];
            
            if (sle64((int64_t) 0, write_iv_23488) && slt64(write_iv_23488, m_20232)) {
                ((int64_t *) mem_23783)[write_iv_23488] = write_iv_23489;
            }
        }
        
        int64_t defunc_0_reduce_res_23342;
        int64_t redout_23491 = (int64_t) -9223372036854775808;
        
        for (int64_t i_23492 = 0; i_23492 < m_20232; i_23492++) {
            int64_t x_23343 = ((int64_t *) mem_23783)[i_23492];
            int64_t max_res_23346 = smax64(x_23343, redout_23491);
            int64_t redout_tmp_23997 = max_res_23346;
            
            redout_23491 = redout_tmp_23997;
        }
        defunc_0_reduce_res_23342 = redout_23491;
        max_mult_t_res_20256 = defunc_0_reduce_res_23342;
    } else {
        max_mult_t_res_20256 = (int64_t) 0;
    }
    
    int64_t max_mult_20262;
    
    if (cond_20255) {
        max_mult_20262 = max_mult_t_res_20256;
    } else {
        max_mult_20262 = (int64_t) 1;
    }
    
    int64_t bytes_23798 = (int64_t) 8 * defunc_res_23377;
    bool bounds_invalid_upwards_20268 = slt64(max_mult_20262, (int64_t) 1);
    bool valid_20270 = !bounds_invalid_upwards_20268;
    bool range_valid_c_20271;
    
    if (!valid_20270) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 1, "..<", (long long) max_mult_20262, " is invalid.", "-> #0  ../../../joins/ftHashJoin.fut:493:20-32\n   #1  test_join1.fut:16:1-29:36\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t bytes_23836 = b_16485 * defunc_res_23377;
    
    if (mem_23775_cached_sizze_24043 < bytes_23565) {
        err = lexical_realloc(ctx, &mem_23775, &mem_23775_cached_sizze_24043, bytes_23565);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    for (int64_t i_23480 = 0; i_23480 < n1_16484; i_23480++) {
        int64_t eta_p_21222 = ((int64_t *) ext_mem_23749.mem)[i_23480];
        int64_t zv_lhs_21224 = add64((int64_t) -1, i_23480);
        int64_t tmp_21225 = smod64(zv_lhs_21224, n1_16484);
        int64_t lifted_lambda_res_21226 = ((int64_t *) mem_23751)[tmp_21225];
        bool cond_21228 = i_23480 == (int64_t) 0;
        int64_t lifted_lambda_res_21229;
        
        if (cond_21228) {
            lifted_lambda_res_21229 = (int64_t) 0;
        } else {
            lifted_lambda_res_21229 = lifted_lambda_res_21226;
        }
        
        bool cond_21231 = slt64((int64_t) 0, eta_p_21222);
        int64_t lifted_lambda_res_21232;
        
        if (cond_21231) {
            lifted_lambda_res_21232 = lifted_lambda_res_21229;
        } else {
            lifted_lambda_res_21232 = (int64_t) -1;
        }
        ((int64_t *) mem_23775)[i_23480] = lifted_lambda_res_21232;
    }
    if (memblock_alloc(ctx, &mem_23799, bytes_23798, "mem_23799")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t nest_i_23999 = 0; nest_i_23999 < defunc_res_23377; nest_i_23999++) {
        ((int64_t *) mem_23799.mem)[nest_i_23999] = (int64_t) 0;
    }
    for (int64_t write_iter_23493 = 0; write_iter_23493 < n1_16484; write_iter_23493++) {
        int64_t write_iv_23495 = ((int64_t *) mem_23775)[write_iter_23493];
        
        if (sle64((int64_t) 0, write_iv_23495) && slt64(write_iv_23495, defunc_res_23377)) {
            ((int64_t *) mem_23799.mem)[write_iv_23495] = write_iter_23493;
        }
    }
    if (memblock_alloc(ctx, &mem_23807, bytes_23798, "mem_23807")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t nest_i_24001 = 0; nest_i_24001 < defunc_res_23377; nest_i_24001++) {
        ((int64_t *) mem_23807.mem)[nest_i_24001] = (int64_t) 1;
    }
    
    int64_t distance_20269 = sub64(max_mult_20262, (int64_t) 1);
    
    if (memblock_set(ctx, &mem_param_23810, &mem_23807, "mem_23807") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_23813, &mem_23799, "mem_23799") != 0)
        return 1;
    for (int64_t i_20274 = 0; i_20274 < distance_20269; i_20274++) {
        int64_t index_primexp_20277 = add64((int64_t) 1, i_20274);
        
        if (memblock_alloc(ctx, &mem_23815, bytes_23798, "mem_23815")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_23815.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_23813.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {defunc_res_23377});
        if (memblock_alloc(ctx, &mem_23817, bytes_23798, "mem_23817")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_23817.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_23810.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {defunc_res_23377});
        
        int64_t replicate_arg1_20289 = add64((int64_t) 2, i_20274);
        
        for (int64_t write_iter_23498 = 0; write_iter_23498 < n1_16484; write_iter_23498++) {
            int64_t zlze_lhs_23117 = ((int64_t *) ext_mem_23749.mem)[write_iter_23498];
            bool cond_23118 = sle64(zlze_lhs_23117, index_primexp_20277);
            int64_t lifted_lambda_res_23119;
            
            if (cond_23118) {
                lifted_lambda_res_23119 = (int64_t) -1;
            } else {
                int64_t zp_lhs_23120 = ((int64_t *) mem_23775)[write_iter_23498];
                int64_t lifted_lambda_res_f_res_23121 = add64(index_primexp_20277, zp_lhs_23120);
                
                lifted_lambda_res_23119 = lifted_lambda_res_f_res_23121;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_23119) && slt64(lifted_lambda_res_23119, defunc_res_23377)) {
                ((int64_t *) mem_23817.mem)[lifted_lambda_res_23119] = replicate_arg1_20289;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_23119) && slt64(lifted_lambda_res_23119, defunc_res_23377)) {
                ((int64_t *) mem_23815.mem)[lifted_lambda_res_23119] = write_iter_23498;
            }
        }
        if (memblock_set(ctx, &mem_param_tmp_24002, &mem_23817, "mem_23817") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_24003, &mem_23815, "mem_23815") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_23810, &mem_param_tmp_24002, "mem_param_tmp_24002") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_23813, &mem_param_tmp_24003, "mem_param_tmp_24003") != 0)
            return 1;
    }
    if (memblock_set(ctx, &ext_mem_23835, &mem_param_23810, "mem_param_23810") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_23834, &mem_param_23813, "mem_param_23813") != 0)
        return 1;
    if (memblock_unref(ctx, &ext_mem_23749, "ext_mem_23749") != 0)
        return 1;
    if (memblock_unref(ctx, &mem_23799, "mem_23799") != 0)
        return 1;
    if (memblock_unref(ctx, &mem_23807, "mem_23807") != 0)
        return 1;
    if (memblock_alloc(ctx, &mem_23837, bytes_23836, "mem_23837")) {
        err = 1;
        goto cleanup;
    }
    if (memblock_alloc(ctx, &mem_23839, bytes_23798, "mem_23839")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t i_23508 = 0; i_23508 < defunc_res_23377; i_23508++) {
        int64_t eta_p_21366 = ((int64_t *) ext_mem_23834.mem)[i_23508];
        bool x_21368 = sle64((int64_t) 0, eta_p_21366);
        bool y_21369 = slt64(eta_p_21366, n1_16484);
        bool bounds_check_21370 = x_21368 && y_21369;
        bool index_certs_21371;
        
        if (!bounds_check_21370) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_21366, "] out of bounds for array of shape [", (long long) n1_16484, "].", "-> #0  ../../../joins/ftHashJoin.fut:502:58-70\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin.fut:502:41-74\n   #3  test_join1.fut:16:1-29:36\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t tmp_21372 = ((int64_t *) mem_23706)[eta_p_21366];
        
        lmad_copy_1b(ctx, 1, (uint8_t *) mem_23837.mem, i_23508 * b_16485, (int64_t []) {(int64_t) 1}, (uint8_t *) ks1_mem_23557.mem, eta_p_21366 * b_16485, (int64_t []) {(int64_t) 1}, (int64_t []) {b_16485});
        ((int64_t *) mem_23839.mem)[i_23508] = tmp_21372;
    }
    
    int32_t zm_rhs_21173 = sdiv32(i32_arg0_20357, 8);
    int32_t zm_lhs_21174 = sub32(i64_res_20350, zm_rhs_21173);
    int32_t i32_arg0_21175 = sub32(zm_lhs_21174, 1);
    int64_t i32_res_21176 = sext_i32_i64(i32_arg0_21175);
    int64_t j_m_i_21182 = sub64(map2_arg2_20883, i32_res_21176);
    
    if (memblock_set(ctx, &mem_param_23856, &mem_23839, "mem_23839") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_23859, &ext_mem_23835, "ext_mem_23835") != 0)
        return 1;
    for (int64_t i_20305 = 0; i_20305 < defunc_0_reduce_res_23373; i_20305++) {
        if (memblock_alloc(ctx, &mem_23861, bytes_23798, "mem_23861")) {
            err = 1;
            goto cleanup;
        }
        if (memblock_alloc(ctx, &mem_23863, bytes_23798, "mem_23863")) {
            err = 1;
            goto cleanup;
        }
        for (int64_t i_23526 = 0; i_23526 < defunc_res_23377; i_23526++) {
            int64_t eta_p_20310 = ((int64_t *) mem_param_23856.mem)[i_23526];
            int64_t eta_p_20311 = ((int64_t *) ext_mem_23834.mem)[i_23526];
            int64_t eta_p_20312 = ((int64_t *) mem_param_23859.mem)[i_23526];
            bool cond_20313 = sle64(eta_p_20312, (int64_t) 0);
            int64_t lifted_lambda_res_20314;
            int64_t lifted_lambda_res_20315;
            
            if (cond_20313) {
                lifted_lambda_res_20314 = eta_p_20310;
                lifted_lambda_res_20315 = eta_p_20312;
            } else {
                bool x_20316 = sle64((int64_t) 0, eta_p_20311);
                bool y_20317 = slt64(eta_p_20311, n1_16484);
                bool bounds_check_20318 = x_20316 && y_20317;
                bool index_certs_20319;
                
                if (!bounds_check_20318) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_20311, "] out of bounds for array of shape [", (long long) n1_16484, "].", "-> #0  ../../../joins/ftHashJoin.fut:506:20-26\n   #1  /prelude/soacs.fut:75:28-29\n   #2  test_join1.fut:16:1-29:36\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                int32_t min_res_21065 = smin32(min_arg1_20352, i32_arg0_20357);
                int32_t zm_rhs_21072 = sdiv32(min_res_21065, 8);
                int32_t zm_lhs_21073 = sub32(i64_res_20350, zm_rhs_21072);
                int32_t i32_arg0_21074 = sub32(zm_lhs_21073, 1);
                int64_t i32_res_21075 = sext_i32_i64(i32_arg0_21074);
                int64_t min_res_21078 = smin64(min_arg1_20365, i32_res_21075);
                bool y_20322 = slt64(eta_p_20310, n2_16487);
                bool x_20321 = sle64((int64_t) 0, eta_p_20310);
                bool bounds_check_20323 = x_20321 && y_20322;
                bool index_certs_20324;
                
                if (!bounds_check_20323) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_20310, "] out of bounds for array of shape [", (long long) n2_16487, "].", "-> #0  ../../../joins/ftHashJoin.fut:507:20-27\n   #1  /prelude/soacs.fut:75:28-29\n   #2  test_join1.fut:16:1-29:36\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                bool x_21106 = sle64((int64_t) 0, min_res_21078);
                bool y_21107 = slt64(min_res_21078, b_16485);
                bool bounds_check_21108 = x_21106 && y_21107;
                bool index_certs_21109;
                
                if (!bounds_check_21108) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) min_res_21078, "] out of bounds for array of shape [", (long long) b_16485, "].", "-> #0  ../../../joins/ftHashJoin.fut:33:40-56\n   #1  test_join1.fut:16:1-29:36\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                bool empty_slice_21183 = j_m_i_21182 == (int64_t) 0;
                int64_t m_21184 = sub64(j_m_i_21182, (int64_t) 1);
                int64_t i_p_m_t_s_21185 = add64(i32_res_21176, m_21184);
                bool zzero_leq_i_p_m_t_s_21186 = sle64((int64_t) 0, i_p_m_t_s_21185);
                bool i_p_m_t_s_leq_w_21187 = slt64(i_p_m_t_s_21185, b_16485);
                bool zzero_lte_i_21188 = sle64((int64_t) 0, i32_res_21176);
                bool i_lte_j_21189 = sle64(i32_res_21176, map2_arg2_20883);
                bool y_21190 = i_p_m_t_s_leq_w_21187 && zzero_lte_i_21188;
                bool y_21191 = zzero_leq_i_p_m_t_s_21186 && y_21190;
                bool forwards_ok_21192 = i_lte_j_21189 && y_21191;
                bool ok_or_empty_21193 = empty_slice_21183 || forwards_ok_21192;
                bool index_certs_21194;
                
                if (!ok_or_empty_21193) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i32_res_21176, ":", (long long) map2_arg2_20883, "] out of bounds for array of shape [", (long long) b_16485, "].", "-> #0  ../../../joins/ftHashJoin.fut:51:49-60\n   #1  test_join1.fut:16:1-29:36\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                if (mem_23870_cached_sizze_24046 < b_16485) {
                    err = lexical_realloc(ctx, &mem_23870, &mem_23870_cached_sizze_24046, b_16485);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                if (mem_23871_cached_sizze_24047 < b_16485) {
                    err = lexical_realloc(ctx, &mem_23871, &mem_23871_cached_sizze_24047, b_16485);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                for (int64_t i_23515 = 0; i_23515 < b_16485; i_23515++) {
                    int8_t eta_p_23127 = ((int8_t *) ks1_mem_23557.mem)[eta_p_20311 * b_16485 + i_23515];
                    int8_t eta_p_23128 = ((int8_t *) ks2_mem_23559.mem)[eta_p_20310 * b_16485 + i_23515];
                    bool cond_23130 = sle64(i_23515, i32_res_20358);
                    bool cond_t_res_23131 = sle64(min_res_21078, i_23515);
                    bool x_23132 = cond_23130 && cond_t_res_23131;
                    int8_t lifted_lambda_res_23133;
                    
                    if (x_23132) {
                        lifted_lambda_res_23133 = eta_p_23127;
                    } else {
                        lifted_lambda_res_23133 = (int8_t) 0;
                    }
                    
                    int8_t lifted_lambda_res_23138;
                    
                    if (x_23132) {
                        lifted_lambda_res_23138 = eta_p_23128;
                    } else {
                        lifted_lambda_res_23138 = (int8_t) 0;
                    }
                    ((int8_t *) mem_23870)[i_23515] = lifted_lambda_res_23138;
                    ((int8_t *) mem_23871)[i_23515] = lifted_lambda_res_23133;
                }
                
                int32_t lastBit_21080 = smod32(min_res_21065, 8);
                int32_t zm_lhs_21083 = sub32(8, lastBit_21080);
                int32_t i32_arg0_21084 = sub32(zm_lhs_21083, 1);
                int8_t unsign_arg0_21085 = zext_i32_i8(i32_arg0_21084);
                int8_t unsign_arg0_21086 = lshr8((int8_t) -1, unsign_arg0_21085);
                int8_t za_lhs_21099 = ((int8_t *) mem_23871)[i32_res_20358];
                int8_t mod1_x_21100 = (int8_t) -1 & za_lhs_21099;
                
                ((int8_t *) mem_23871)[i32_res_20358] = mod1_x_21100;
                
                int8_t za_lhs_21110 = ((int8_t *) mem_23871)[min_res_21078];
                int8_t tmp_21111 = unsign_arg0_21086 & za_lhs_21110;
                
                ((int8_t *) mem_23871)[min_res_21078] = tmp_21111;
                
                int8_t za_lhs_21154 = ((int8_t *) mem_23870)[i32_res_20358];
                int8_t mod1_x_21155 = (int8_t) -1 & za_lhs_21154;
                
                ((int8_t *) mem_23870)[i32_res_20358] = mod1_x_21155;
                
                int8_t za_lhs_21165 = ((int8_t *) mem_23870)[min_res_21078];
                int8_t tmp_21166 = unsign_arg0_21086 & za_lhs_21165;
                
                ((int8_t *) mem_23870)[min_res_21078] = tmp_21166;
                if (mem_23884_cached_sizze_24048 < j_m_i_21182) {
                    err = lexical_realloc(ctx, &mem_23884, &mem_23884_cached_sizze_24048, j_m_i_21182);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                for (int64_t i_23520 = 0; i_23520 < j_m_i_21182; i_23520++) {
                    int64_t slice_23530 = i32_res_21176 + i_23520;
                    int8_t eta_p_21198 = ((int8_t *) mem_23871)[slice_23530];
                    int8_t eta_p_21199 = ((int8_t *) mem_23870)[slice_23530];
                    bool defunc_0_f_res_21200 = eta_p_21198 == eta_p_21199;
                    
                    ((bool *) mem_23884)[i_23520] = defunc_0_f_res_21200;
                }
                
                bool defunc_0_foldl_res_21201;
                bool acc_21203 = 1;
                
                for (int64_t i_21202 = 0; i_21202 < j_m_i_21182; i_21202++) {
                    bool b_21204 = ((bool *) mem_23884)[i_21202];
                    bool x_21205 = acc_21203 && b_21204;
                    bool acc_tmp_24019 = x_21205;
                    
                    acc_21203 = acc_tmp_24019;
                }
                defunc_0_foldl_res_21201 = acc_21203;
                
                int64_t lifted_lambda_res_f_res_20329;
                int64_t lifted_lambda_res_f_res_20330;
                
                if (defunc_0_foldl_res_21201) {
                    bool cond_23355 = eta_p_20312 == (int64_t) 1;
                    int64_t lifted_lambda_res_f_res_t_res_23356;
                    int64_t lifted_lambda_res_f_res_t_res_23357;
                    
                    if (cond_23355) {
                        int64_t tmp_23358 = sub64(eta_p_20312, (int64_t) 1);
                        
                        lifted_lambda_res_f_res_t_res_23356 = eta_p_20310;
                        lifted_lambda_res_f_res_t_res_23357 = tmp_23358;
                    } else {
                        int64_t tmp_23359 = add64((int64_t) 1, eta_p_20310);
                        int64_t tmp_23360 = sub64(eta_p_20312, (int64_t) 1);
                        
                        lifted_lambda_res_f_res_t_res_23356 = tmp_23359;
                        lifted_lambda_res_f_res_t_res_23357 = tmp_23360;
                    }
                    lifted_lambda_res_f_res_20329 = lifted_lambda_res_f_res_t_res_23356;
                    lifted_lambda_res_f_res_20330 = lifted_lambda_res_f_res_t_res_23357;
                } else {
                    int64_t tmp_20337 = add64((int64_t) 1, eta_p_20310);
                    
                    lifted_lambda_res_f_res_20329 = tmp_20337;
                    lifted_lambda_res_f_res_20330 = eta_p_20312;
                }
                lifted_lambda_res_20314 = lifted_lambda_res_f_res_20329;
                lifted_lambda_res_20315 = lifted_lambda_res_f_res_20330;
            }
            ((int64_t *) mem_23861.mem)[i_23526] = lifted_lambda_res_20314;
            ((int64_t *) mem_23863.mem)[i_23526] = lifted_lambda_res_20315;
        }
        if (memblock_set(ctx, &mem_param_tmp_24010, &mem_23861, "mem_23861") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_24011, &mem_23863, "mem_23863") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_23856, &mem_param_tmp_24010, "mem_param_tmp_24010") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_23859, &mem_param_tmp_24011, "mem_param_tmp_24011") != 0)
            return 1;
    }
    if (memblock_set(ctx, &ext_mem_23902, &mem_param_23856, "mem_param_23856") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_23901, &mem_param_23859, "mem_param_23859") != 0)
        return 1;
    if (memblock_unref(ctx, &ext_mem_23835, "ext_mem_23835") != 0)
        return 1;
    if (memblock_unref(ctx, &mem_23839, "mem_23839") != 0)
        return 1;
    if (memblock_alloc(ctx, &mem_23904, bytes_23798, "mem_23904")) {
        err = 1;
        goto cleanup;
    }
    lmad_copy_8b(ctx, 1, (uint64_t *) mem_23904.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_23834.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {defunc_res_23377});
    if (memblock_unref(ctx, &ext_mem_23834, "ext_mem_23834") != 0)
        return 1;
    if (memblock_set(ctx, &mem_out_23932, &mem_23837, "mem_23837") != 0)
        return 1;
    if (memblock_set(ctx, &mem_out_23933, &mem_23904, "mem_23904") != 0)
        return 1;
    if (memblock_set(ctx, &mem_out_23934, &ext_mem_23902, "ext_mem_23902") != 0)
        return 1;
    prim_out_23935 = defunc_res_23377;
    if (memblock_set(ctx, &*mem_out_p_24020, &mem_out_23932, "mem_out_23932") != 0)
        return 1;
    if (memblock_set(ctx, &*mem_out_p_24021, &mem_out_23933, "mem_out_23933") != 0)
        return 1;
    if (memblock_set(ctx, &*mem_out_p_24022, &mem_out_23934, "mem_out_23934") != 0)
        return 1;
    *out_prim_out_24023 = prim_out_23935;
    
  cleanup:
    {
        free(mem_23568);
        free(mem_23575);
        free(mem_23620);
        free(mem_23621);
        free(mem_23634);
        free(mem_23641);
        free(mem_23642);
        free(mem_23655);
        free(mem_23656);
        free(mem_23669);
        free(mem_23670);
        free(mem_23683);
        free(mem_23706);
        free(mem_23723);
        free(mem_23724);
        free(mem_23737);
        free(mem_23751);
        free(mem_23753);
        free(mem_23755);
        free(mem_23775);
        free(mem_23783);
        free(mem_23785);
        free(mem_23870);
        free(mem_23871);
        free(mem_23884);
        if (memblock_unref(ctx, &mem_23904, "mem_23904") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_24011, "mem_param_tmp_24011") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_24010, "mem_param_tmp_24010") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_23863, "mem_23863") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_23861, "mem_23861") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_23859, "mem_param_23859") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_23856, "mem_param_23856") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_23901, "ext_mem_23901") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_23902, "ext_mem_23902") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_23839, "mem_23839") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_23837, "mem_23837") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_24003, "mem_param_tmp_24003") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_24002, "mem_param_tmp_24002") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_23817, "mem_23817") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_23815, "mem_23815") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_23813, "mem_param_23813") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_23810, "mem_param_23810") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_23834, "ext_mem_23834") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_23835, "ext_mem_23835") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_23807, "mem_23807") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_23799, "mem_23799") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_23982, "mem_param_tmp_23982") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_23719, "mem_23719") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_23717, "mem_param_23717") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_23749, "ext_mem_23749") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_23714, "mem_23714") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_23944, "mem_param_tmp_23944") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_23943, "mem_param_tmp_23943") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_23607, "mem_23607") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_23605, "mem_23605") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_23603, "mem_param_23603") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_23600, "mem_param_23600") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_23700, "ext_mem_23700") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_23701, "ext_mem_23701") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_23597, "mem_23597") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_23589, "mem_23589") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_23704, "ext_mem_23704") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_23566, "mem_23566") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_23934, "mem_out_23934") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_23933, "mem_out_23933") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_23932, "mem_out_23932") != 0)
            return 1;
    }
    return err;
}

int futhark_entry_rhj(struct futhark_context *ctx, struct futhark_u8_2d **out0, struct futhark_i64_1d **out1, struct futhark_i64_1d **out2, const struct futhark_u8_2d *in0, const struct futhark_u8_2d *in1, const struct futhark_u8_2d *in2, const struct futhark_u8_2d *in3, const int32_t in4, const struct futhark_i64_1d *in5, const struct futhark_i32_1d *in6, const struct futhark_i64_1d *in7, const struct futhark_i64_1d *in8)
{
    int64_t n1_16484 = (int64_t) 0;
    int64_t b_16485 = (int64_t) 0;
    int64_t dz2080U_16486 = (int64_t) 0;
    int64_t n2_16487 = (int64_t) 0;
    int64_t dz2081U_16488 = (int64_t) 0;
    int64_t dz2082U_16489 = (int64_t) 0;
    int64_t dz2083U_16490 = (int64_t) 0;
    int64_t coercez2081Uz2088U_16491 = (int64_t) 0;
    int64_t coercez2082Uz2081U_16492 = (int64_t) 0;
    int32_t maxDepth2_16497 = 0;
    int64_t prim_out_23935 = (int64_t) 0;
    int ret = 0;
    
    lock_lock(&ctx->lock);
    
    struct memblock mem_out_23934;
    
    mem_out_23934.references = NULL;
    
    struct memblock mem_out_23933;
    
    mem_out_23933.references = NULL;
    
    struct memblock mem_out_23932;
    
    mem_out_23932.references = NULL;
    
    struct memblock last_idx2_mem_23564;
    
    last_idx2_mem_23564.references = NULL;
    
    struct memblock first_idx2_mem_23563;
    
    first_idx2_mem_23563.references = NULL;
    
    struct memblock depths2_mem_23562;
    
    depths2_mem_23562.references = NULL;
    
    struct memblock bounds2_mem_23561;
    
    bounds2_mem_23561.references = NULL;
    
    struct memblock nameless_mem_23560;
    
    nameless_mem_23560.references = NULL;
    
    struct memblock ks2_mem_23559;
    
    ks2_mem_23559.references = NULL;
    
    struct memblock nameless_mem_23558;
    
    nameless_mem_23558.references = NULL;
    
    struct memblock ks1_mem_23557;
    
    ks1_mem_23557.references = NULL;
    ks1_mem_23557 = in0->mem;
    n1_16484 = in0->shape[0];
    b_16485 = in0->shape[1];
    nameless_mem_23558 = in1->mem;
    n1_16484 = in1->shape[0];
    dz2080U_16486 = in1->shape[1];
    ks2_mem_23559 = in2->mem;
    n2_16487 = in2->shape[0];
    b_16485 = in2->shape[1];
    nameless_mem_23560 = in3->mem;
    n2_16487 = in3->shape[0];
    dz2081U_16488 = in3->shape[1];
    maxDepth2_16497 = in4;
    bounds2_mem_23561 = in5->mem;
    dz2082U_16489 = in5->shape[0];
    depths2_mem_23562 = in6->mem;
    dz2083U_16490 = in6->shape[0];
    first_idx2_mem_23563 = in7->mem;
    coercez2081Uz2088U_16491 = in7->shape[0];
    last_idx2_mem_23564 = in8->mem;
    coercez2082Uz2081U_16492 = in8->shape[0];
    if (!((n1_16484 == in0->shape[0] && b_16485 == in0->shape[1]) && ((n1_16484 == in1->shape[0] && dz2080U_16486 == in1->shape[1]) && ((n2_16487 == in2->shape[0] && b_16485 == in2->shape[1]) && ((n2_16487 == in3->shape[0] && dz2081U_16488 == in3->shape[1]) && (dz2082U_16489 == in5->shape[0] && (dz2083U_16490 == in6->shape[0] && (coercez2081Uz2088U_16491 == in7->shape[0] && coercez2082Uz2081U_16492 == in8->shape[0])))))))) {
        ret = 1;
        set_error(ctx, msgprintf("Error: entry point arguments have invalid sizes.\n"));
    }
    if (ret == 0) {
        ret = futrts_entry_rhj(ctx, &mem_out_23932, &mem_out_23933, &mem_out_23934, &prim_out_23935, ks1_mem_23557, nameless_mem_23558, ks2_mem_23559, nameless_mem_23560, bounds2_mem_23561, depths2_mem_23562, first_idx2_mem_23563, last_idx2_mem_23564, n1_16484, b_16485, dz2080U_16486, n2_16487, dz2081U_16488, dz2082U_16489, dz2083U_16490, coercez2081Uz2088U_16491, coercez2082Uz2081U_16492, maxDepth2_16497);
        if (ret == 0) {
            assert((*out0 = (struct futhark_u8_2d *) malloc(sizeof(struct futhark_u8_2d))) != NULL);
            (*out0)->mem = mem_out_23932;
            (*out0)->shape[0] = prim_out_23935;
            (*out0)->shape[1] = b_16485;
            assert((*out1 = (struct futhark_i64_1d *) malloc(sizeof(struct futhark_i64_1d))) != NULL);
            (*out1)->mem = mem_out_23933;
            (*out1)->shape[0] = prim_out_23935;
            assert((*out2 = (struct futhark_i64_1d *) malloc(sizeof(struct futhark_i64_1d))) != NULL);
            (*out2)->mem = mem_out_23934;
            (*out2)->shape[0] = prim_out_23935;
        }
    }
    lock_unlock(&ctx->lock);
    return ret;
}
  
