
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

FUTHARK_FUN_ATTR int futrts_entry_rhj(struct futhark_context *ctx, struct memblock *mem_out_p_42753, struct memblock *mem_out_p_42754, struct memblock *mem_out_p_42755, int64_t *out_prim_out_42756, struct memblock ks1_mem_41977, struct memblock nameless_mem_41978, struct memblock ks2_mem_41979, struct memblock nameless_mem_41980, struct memblock bounds2_mem_41981, struct memblock depths2_mem_41982, struct memblock first_idx2_mem_41983, struct memblock last_idx2_mem_41984, int64_t n1_25949, int64_t b_25950, int64_t dz2080U_25951, int64_t n2_25952, int64_t dz2081U_25953, int64_t dz2083U_25954, int64_t coercez2081Uz2088U_25955, int64_t coercez2082Uz2081U_25956, int32_t maxDepth2_25961);

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

FUTHARK_FUN_ATTR int futrts_entry_rhj(struct futhark_context *ctx, struct memblock *mem_out_p_42753, struct memblock *mem_out_p_42754, struct memblock *mem_out_p_42755, int64_t *out_prim_out_42756, struct memblock ks1_mem_41977, struct memblock nameless_mem_41978, struct memblock ks2_mem_41979, struct memblock nameless_mem_41980, struct memblock bounds2_mem_41981, struct memblock depths2_mem_41982, struct memblock first_idx2_mem_41983, struct memblock last_idx2_mem_41984, int64_t n1_25949, int64_t b_25950, int64_t dz2080U_25951, int64_t n2_25952, int64_t dz2081U_25953, int64_t dz2083U_25954, int64_t coercez2081Uz2088U_25955, int64_t coercez2082Uz2081U_25956, int32_t maxDepth2_25961)
{
    (void) ctx;
    
    int err = 0;
    int64_t mem_41986_cached_sizze_42757 = 0;
    unsigned char *mem_41986 = NULL;
    int64_t mem_41994_cached_sizze_42758 = 0;
    unsigned char *mem_41994 = NULL;
    int64_t mem_41996_cached_sizze_42759 = 0;
    unsigned char *mem_41996 = NULL;
    int64_t mem_41998_cached_sizze_42760 = 0;
    unsigned char *mem_41998 = NULL;
    int64_t mem_42000_cached_sizze_42761 = 0;
    unsigned char *mem_42000 = NULL;
    int64_t mem_42002_cached_sizze_42762 = 0;
    unsigned char *mem_42002 = NULL;
    int64_t mem_42004_cached_sizze_42763 = 0;
    unsigned char *mem_42004 = NULL;
    int64_t mem_42042_cached_sizze_42764 = 0;
    unsigned char *mem_42042 = NULL;
    int64_t mem_42050_cached_sizze_42765 = 0;
    unsigned char *mem_42050 = NULL;
    int64_t mem_42052_cached_sizze_42766 = 0;
    unsigned char *mem_42052 = NULL;
    int64_t mem_42124_cached_sizze_42767 = 0;
    unsigned char *mem_42124 = NULL;
    int64_t mem_42132_cached_sizze_42768 = 0;
    unsigned char *mem_42132 = NULL;
    int64_t mem_42134_cached_sizze_42769 = 0;
    unsigned char *mem_42134 = NULL;
    int64_t mem_42136_cached_sizze_42770 = 0;
    unsigned char *mem_42136 = NULL;
    int64_t mem_42138_cached_sizze_42771 = 0;
    unsigned char *mem_42138 = NULL;
    int64_t mem_42140_cached_sizze_42772 = 0;
    unsigned char *mem_42140 = NULL;
    int64_t mem_42142_cached_sizze_42773 = 0;
    unsigned char *mem_42142 = NULL;
    int64_t mem_42180_cached_sizze_42774 = 0;
    unsigned char *mem_42180 = NULL;
    int64_t mem_42188_cached_sizze_42775 = 0;
    unsigned char *mem_42188 = NULL;
    int64_t mem_42190_cached_sizze_42776 = 0;
    unsigned char *mem_42190 = NULL;
    int64_t mem_42270_cached_sizze_42777 = 0;
    unsigned char *mem_42270 = NULL;
    int64_t mem_42278_cached_sizze_42778 = 0;
    unsigned char *mem_42278 = NULL;
    int64_t mem_42280_cached_sizze_42779 = 0;
    unsigned char *mem_42280 = NULL;
    int64_t mem_42282_cached_sizze_42780 = 0;
    unsigned char *mem_42282 = NULL;
    int64_t mem_42284_cached_sizze_42781 = 0;
    unsigned char *mem_42284 = NULL;
    int64_t mem_42286_cached_sizze_42782 = 0;
    unsigned char *mem_42286 = NULL;
    int64_t mem_42288_cached_sizze_42783 = 0;
    unsigned char *mem_42288 = NULL;
    int64_t mem_42326_cached_sizze_42784 = 0;
    unsigned char *mem_42326 = NULL;
    int64_t mem_42334_cached_sizze_42785 = 0;
    unsigned char *mem_42334 = NULL;
    int64_t mem_42336_cached_sizze_42786 = 0;
    unsigned char *mem_42336 = NULL;
    int64_t mem_42415_cached_sizze_42787 = 0;
    unsigned char *mem_42415 = NULL;
    int64_t mem_42423_cached_sizze_42788 = 0;
    unsigned char *mem_42423 = NULL;
    int64_t mem_42425_cached_sizze_42789 = 0;
    unsigned char *mem_42425 = NULL;
    int64_t mem_42427_cached_sizze_42790 = 0;
    unsigned char *mem_42427 = NULL;
    int64_t mem_42429_cached_sizze_42791 = 0;
    unsigned char *mem_42429 = NULL;
    int64_t mem_42431_cached_sizze_42792 = 0;
    unsigned char *mem_42431 = NULL;
    int64_t mem_42432_cached_sizze_42793 = 0;
    unsigned char *mem_42432 = NULL;
    int64_t mem_42470_cached_sizze_42794 = 0;
    unsigned char *mem_42470 = NULL;
    int64_t mem_42478_cached_sizze_42795 = 0;
    unsigned char *mem_42478 = NULL;
    int64_t mem_42480_cached_sizze_42796 = 0;
    unsigned char *mem_42480 = NULL;
    struct memblock mem_42122;
    
    mem_42122.references = NULL;
    
    struct memblock mem_42106;
    
    mem_42106.references = NULL;
    
    struct memblock mem_42104;
    
    mem_42104.references = NULL;
    
    struct memblock mem_param_tmp_42743;
    
    mem_param_tmp_42743.references = NULL;
    
    struct memblock mem_param_tmp_42742;
    
    mem_param_tmp_42742.references = NULL;
    
    struct memblock mem_42084;
    
    mem_42084.references = NULL;
    
    struct memblock mem_42082;
    
    mem_42082.references = NULL;
    
    struct memblock mem_param_42080;
    
    mem_param_42080.references = NULL;
    
    struct memblock mem_param_42077;
    
    mem_param_42077.references = NULL;
    
    struct memblock ext_mem_42101;
    
    ext_mem_42101.references = NULL;
    
    struct memblock ext_mem_42102;
    
    ext_mem_42102.references = NULL;
    
    struct memblock mem_42074;
    
    mem_42074.references = NULL;
    
    struct memblock mem_42066;
    
    mem_42066.references = NULL;
    
    struct memblock mem_42260;
    
    mem_42260.references = NULL;
    
    struct memblock mem_42244;
    
    mem_42244.references = NULL;
    
    struct memblock mem_42242;
    
    mem_42242.references = NULL;
    
    struct memblock mem_param_tmp_42697;
    
    mem_param_tmp_42697.references = NULL;
    
    struct memblock mem_param_tmp_42696;
    
    mem_param_tmp_42696.references = NULL;
    
    struct memblock mem_42222;
    
    mem_42222.references = NULL;
    
    struct memblock mem_42220;
    
    mem_42220.references = NULL;
    
    struct memblock mem_param_42218;
    
    mem_param_42218.references = NULL;
    
    struct memblock mem_param_42215;
    
    mem_param_42215.references = NULL;
    
    struct memblock ext_mem_42239;
    
    ext_mem_42239.references = NULL;
    
    struct memblock ext_mem_42240;
    
    ext_mem_42240.references = NULL;
    
    struct memblock mem_42212;
    
    mem_42212.references = NULL;
    
    struct memblock mem_42204;
    
    mem_42204.references = NULL;
    
    struct memblock ext_mem_42264;
    
    ext_mem_42264.references = NULL;
    
    struct memblock ext_mem_42267;
    
    ext_mem_42267.references = NULL;
    
    struct memblock ext_mem_42268;
    
    ext_mem_42268.references = NULL;
    
    struct memblock mem_42406;
    
    mem_42406.references = NULL;
    
    struct memblock mem_42390;
    
    mem_42390.references = NULL;
    
    struct memblock mem_42388;
    
    mem_42388.references = NULL;
    
    struct memblock mem_param_tmp_42651;
    
    mem_param_tmp_42651.references = NULL;
    
    struct memblock mem_param_tmp_42650;
    
    mem_param_tmp_42650.references = NULL;
    
    struct memblock mem_42368;
    
    mem_42368.references = NULL;
    
    struct memblock mem_42366;
    
    mem_42366.references = NULL;
    
    struct memblock mem_param_42364;
    
    mem_param_42364.references = NULL;
    
    struct memblock mem_param_42361;
    
    mem_param_42361.references = NULL;
    
    struct memblock ext_mem_42385;
    
    ext_mem_42385.references = NULL;
    
    struct memblock ext_mem_42386;
    
    ext_mem_42386.references = NULL;
    
    struct memblock mem_42358;
    
    mem_42358.references = NULL;
    
    struct memblock mem_42350;
    
    mem_42350.references = NULL;
    
    struct memblock ext_mem_42410;
    
    ext_mem_42410.references = NULL;
    
    struct memblock ext_mem_42413;
    
    ext_mem_42413.references = NULL;
    
    struct memblock ext_mem_42414;
    
    ext_mem_42414.references = NULL;
    
    struct memblock mem_42550;
    
    mem_42550.references = NULL;
    
    struct memblock mem_42534;
    
    mem_42534.references = NULL;
    
    struct memblock mem_42532;
    
    mem_42532.references = NULL;
    
    struct memblock mem_param_tmp_42605;
    
    mem_param_tmp_42605.references = NULL;
    
    struct memblock mem_param_tmp_42604;
    
    mem_param_tmp_42604.references = NULL;
    
    struct memblock mem_42512;
    
    mem_42512.references = NULL;
    
    struct memblock mem_42510;
    
    mem_42510.references = NULL;
    
    struct memblock mem_param_42508;
    
    mem_param_42508.references = NULL;
    
    struct memblock mem_param_42505;
    
    mem_param_42505.references = NULL;
    
    struct memblock ext_mem_42529;
    
    ext_mem_42529.references = NULL;
    
    struct memblock ext_mem_42530;
    
    ext_mem_42530.references = NULL;
    
    struct memblock mem_42502;
    
    mem_42502.references = NULL;
    
    struct memblock mem_42494;
    
    mem_42494.references = NULL;
    
    struct memblock ext_mem_42554;
    
    ext_mem_42554.references = NULL;
    
    struct memblock ext_mem_42557;
    
    ext_mem_42557.references = NULL;
    
    struct memblock ext_mem_42558;
    
    ext_mem_42558.references = NULL;
    
    struct memblock mem_out_42567;
    
    mem_out_42567.references = NULL;
    
    struct memblock mem_out_42566;
    
    mem_out_42566.references = NULL;
    
    struct memblock mem_out_42565;
    
    mem_out_42565.references = NULL;
    
    int64_t prim_out_42568;
    double i64_res_31064 = sitofp_i64_f64(coercez2081Uz2088U_25955);
    double log2_res_31065 = futrts_log2_64(i64_res_31064);
    int32_t f64_res_31066 = fptosi_f64_i32(log2_res_31065);
    int64_t i32_res_31067 = sext_i32_i64(f64_res_31066);
    bool nonnegative_31068 = sle64((int64_t) 0, i32_res_31067);
    bool nonzzero_cert_31069;
    
    if (!nonnegative_31068) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "negative exponent", "-> #0  test_join2.fut:25:49-72\n   #1  test_join2.fut:16:1-29:36\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t dzlz7bUZLztztZRz20U2z20UZLi32z20Uradix_sizzeZRz7dUzg_31070 = shl64((int64_t) 1, i32_res_31067);
    bool dim_match_31071 = dzlz7bUZLztztZRz20U2z20UZLi32z20Uradix_sizzeZRz7dUzg_31070 == coercez2081Uz2088U_25955;
    bool empty_or_match_cert_31072;
    
    if (!dim_match_31071) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Value of (desugared) shape [", (long long) coercez2081Uz2088U_25955, "] cannot match shape of type `[", (long long) dzlz7bUZLztztZRz20U2z20UZLi32z20Uradix_sizzeZRz7dUzg_31070, "]i64`.", "-> #0  test_join2.fut:25:34-76\n   #1  test_join2.fut:16:1-29:36\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool dim_match_31073 = dzlz7bUZLztztZRz20U2z20UZLi32z20Uradix_sizzeZRz7dUzg_31070 == coercez2082Uz2081U_25956;
    bool empty_or_match_cert_31074;
    
    if (!dim_match_31073) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Value of (desugared) shape [", (long long) coercez2082Uz2081U_25956, "] cannot match shape of type `[", (long long) dzlz7bUZLztztZRz20U2z20UZLi32z20Uradix_sizzeZRz7dUzg_31070, "]i64`.", "-> #0  test_join2.fut:26:33-74\n   #1  test_join2.fut:16:1-29:36\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool cond_31075 = b_25950 == (int64_t) 1;
    bool cond_31076 = b_25950 == (int64_t) 2;
    bool cond_31077 = sle64(b_25950, (int64_t) 4);
    int64_t unsign_arg0_31078 = zext_i32_i64(f64_res_31066);
    int64_t asPow_31079 = shl64((int64_t) 1, unsign_arg0_31078);
    int64_t u64_arg0_31080 = sub64(asPow_31079, (int64_t) 1);
    int64_t zeze_rhs_31081 = sub64(dz2083U_25954, (int64_t) 1);
    bool cond_31082 = n1_25949 == (int64_t) 0;
    bool x_31083 = !cond_31082;
    int64_t tmp_31084 = sub64(n1_25949, (int64_t) 1);
    bool x_31085 = sle64((int64_t) 0, tmp_31084);
    bool y_31086 = slt64(tmp_31084, n1_25949);
    bool bounds_check_31087 = x_31085 && y_31086;
    bool protect_assert_disj_31088 = cond_31082 || bounds_check_31087;
    bool protect_assert_disj_31089 = cond_31077 || protect_assert_disj_31088;
    bool protect_assert_disj_31090 = cond_31076 || protect_assert_disj_31089;
    bool protect_assert_disj_31091 = cond_31075 || protect_assert_disj_31090;
    bool index_certs_31092;
    
    if (!protect_assert_disj_31091) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_31084, "] out of bounds for array of shape [", (long long) n1_25949, "].", "-> #0  /prelude/soacs.fut:257:33-47\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin_old.fut:848:4-854:44\n   #3  test_join2.fut:16:1-29:36\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int32_t u64_res_31093 = zext_i64_i32(asPow_31079);
    int32_t za_rhs_31094 = sub32(u64_res_31093, 1);
    bool loop_not_taken_31095 = !cond_31077;
    bool protect_assert_disj_31096 = protect_assert_disj_31088 || loop_not_taken_31095;
    bool protect_assert_disj_31097 = cond_31076 || protect_assert_disj_31096;
    bool protect_assert_disj_31098 = cond_31075 || protect_assert_disj_31097;
    bool index_certs_31099;
    
    if (!protect_assert_disj_31098) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_31084, "] out of bounds for array of shape [", (long long) n1_25949, "].", "-> #0  /prelude/soacs.fut:257:33-47\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin_old.fut:848:4-853:57\n   #3  test_join2.fut:16:1-29:36\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int16_t u64_res_31100 = zext_i64_i16(u64_arg0_31080);
    bool loop_not_taken_31101 = !cond_31076;
    bool protect_assert_disj_31102 = protect_assert_disj_31088 || loop_not_taken_31101;
    bool protect_assert_disj_31103 = cond_31075 || protect_assert_disj_31102;
    bool index_certs_31104;
    
    if (!protect_assert_disj_31103) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_31084, "] out of bounds for array of shape [", (long long) n1_25949, "].", "-> #0  /prelude/soacs.fut:257:33-47\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin_old.fut:848:4-852:57\n   #3  test_join2.fut:16:1-29:36\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int8_t u64_res_31105 = zext_i64_i8(u64_arg0_31080);
    bool loop_not_taken_31106 = !cond_31075;
    bool protect_assert_disj_31107 = protect_assert_disj_31088 || loop_not_taken_31106;
    bool index_certs_31108;
    
    if (!protect_assert_disj_31107) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_31084, "] out of bounds for array of shape [", (long long) n1_25949, "].", "-> #0  /prelude/soacs.fut:257:33-47\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin_old.fut:848:4-851:51\n   #3  test_join2.fut:16:1-29:36\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t bytes_41985 = (int64_t) 8 * n2_25952;
    int64_t bytes_41993 = (int64_t) 8 * n1_25949;
    int64_t bytes_42123 = (int64_t) 4 * n2_25952;
    int64_t bytes_42141 = (int64_t) 4 * n1_25949;
    int64_t bytes_42269 = (int64_t) 2 * n2_25952;
    int64_t bytes_42287 = (int64_t) 2 * n1_25949;
    int64_t radix_hash_join_res_31109;
    int64_t radix_hash_join_res_31110;
    int64_t radix_hash_join_res_31111;
    
    if (cond_31075) {
        if (mem_42415_cached_sizze_42787 < n2_25952) {
            err = lexical_realloc(ctx, &mem_42415, &mem_42415_cached_sizze_42787, n2_25952);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t i_41711 = 0; i_41711 < n2_25952; i_41711++) {
            int8_t defunc_0_byteSeq_to_prim_res_40126;
            int8_t y_40128 = (int8_t) 0;
            
            for (int64_t i_40127 = 0; i_40127 < b_25950; i_40127++) {
                int64_t zm_lhs_40129 = sub64(b_25950, i_40127);
                int64_t from_u8_arg0_40130 = sub64(zm_lhs_40129, (int64_t) 1);
                bool x_40131 = sle64((int64_t) 0, from_u8_arg0_40130);
                bool y_40132 = slt64(from_u8_arg0_40130, b_25950);
                bool bounds_check_40133 = x_40131 && y_40132;
                bool index_certs_40134;
                
                if (!bounds_check_40133) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) from_u8_arg0_40130, "] out of bounds for array of shape [", (long long) b_25950, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:417:22-30\n   #1  ../../../joins/ftHashJoin_old.fut:421:70-76\n   #2  test_join2.fut:16:1-29:36\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                int8_t from_u8_arg0_40135 = ((int8_t *) ks2_mem_41979.mem)[i_41711 * b_25950 + from_u8_arg0_40130];
                int32_t i64_res_40136 = sext_i64_i32(i_40127);
                int32_t shift_left_rhs_40137 = mul32(8, i64_res_40136);
                int8_t unsign_arg0_40138 = zext_i32_i8(shift_left_rhs_40137);
                int8_t lifted_lambda_res_40139 = shl8(from_u8_arg0_40135, unsign_arg0_40138);
                int8_t unsign_arg0_40140 = y_40128 | lifted_lambda_res_40139;
                int8_t y_tmp_42570 = unsign_arg0_40140;
                
                y_40128 = y_tmp_42570;
            }
            defunc_0_byteSeq_to_prim_res_40126 = y_40128;
            ((int8_t *) mem_42415)[i_41711] = defunc_0_byteSeq_to_prim_res_40126;
        }
        if (mem_42423_cached_sizze_42788 < bytes_41993) {
            err = lexical_realloc(ctx, &mem_42423, &mem_42423_cached_sizze_42788, bytes_41993);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_42425_cached_sizze_42789 < bytes_41993) {
            err = lexical_realloc(ctx, &mem_42425, &mem_42425_cached_sizze_42789, bytes_41993);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_42427_cached_sizze_42790 < bytes_41993) {
            err = lexical_realloc(ctx, &mem_42427, &mem_42427_cached_sizze_42790, bytes_41993);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_42429_cached_sizze_42791 < bytes_41993) {
            err = lexical_realloc(ctx, &mem_42429, &mem_42429_cached_sizze_42791, bytes_41993);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_42431_cached_sizze_42792 < bytes_41993) {
            err = lexical_realloc(ctx, &mem_42431, &mem_42431_cached_sizze_42792, bytes_41993);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_42432_cached_sizze_42793 < n1_25949) {
            err = lexical_realloc(ctx, &mem_42432, &mem_42432_cached_sizze_42793, n1_25949);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_41735;
        int64_t discard_41736;
        int64_t defunc_res_40145;
        int64_t scanacc_41719;
        int64_t scanacc_41720;
        int64_t redout_41723;
        
        scanacc_41719 = (int64_t) 0;
        scanacc_41720 = (int64_t) 0;
        redout_41723 = (int64_t) 0;
        for (int64_t i_41728 = 0; i_41728 < n1_25949; i_41728++) {
            int8_t defunc_0_byteSeq_to_prim_res_40151;
            int8_t y_40153 = (int8_t) 0;
            
            for (int64_t i_40152 = 0; i_40152 < b_25950; i_40152++) {
                int64_t zm_lhs_40154 = sub64(b_25950, i_40152);
                int64_t from_u8_arg0_40155 = sub64(zm_lhs_40154, (int64_t) 1);
                bool x_40156 = sle64((int64_t) 0, from_u8_arg0_40155);
                bool y_40157 = slt64(from_u8_arg0_40155, b_25950);
                bool bounds_check_40158 = x_40156 && y_40157;
                bool index_certs_40159;
                
                if (!bounds_check_40158) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) from_u8_arg0_40155, "] out of bounds for array of shape [", (long long) b_25950, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:417:22-30\n   #1  ../../../joins/ftHashJoin_old.fut:421:70-76\n   #2  test_join2.fut:16:1-29:36\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                int8_t from_u8_arg0_40160 = ((int8_t *) ks1_mem_41977.mem)[i_41728 * b_25950 + from_u8_arg0_40155];
                int32_t i64_res_40161 = sext_i64_i32(i_40152);
                int32_t shift_left_rhs_40162 = mul32(8, i64_res_40161);
                int8_t unsign_arg0_40163 = zext_i32_i8(shift_left_rhs_40162);
                int8_t lifted_lambda_res_40164 = shl8(from_u8_arg0_40160, unsign_arg0_40163);
                int8_t unsign_arg0_40165 = y_40153 | lifted_lambda_res_40164;
                int8_t y_tmp_42580 = unsign_arg0_40165;
                
                y_40153 = y_tmp_42580;
            }
            defunc_0_byteSeq_to_prim_res_40151 = y_40153;
            
            int8_t lifted_lambda_res_40166 = u64_res_31105 & defunc_0_byteSeq_to_prim_res_40151;
            int64_t u8_res_40167 = zext_i8_i64(lifted_lambda_res_40166);
            bool x_40168 = sle64((int64_t) 0, u8_res_40167);
            bool y_40169 = slt64(u8_res_40167, dzlz7bUZLztztZRz20U2z20UZLi32z20Uradix_sizzeZRz7dUzg_31070);
            bool bounds_check_40170 = x_40168 && y_40169;
            bool index_certs_40171;
            
            if (!bounds_check_40170) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) u8_res_40167, "] out of bounds for array of shape [", (long long) dzlz7bUZLztztZRz20U2z20UZLi32z20Uradix_sizzeZRz7dUzg_31070, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:530:13-35\n   #1  ../../../joins/ftHashJoin_old.fut:592:3-599:86\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-851:51\n   #4  test_join2.fut:16:1-29:36\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t spi_40172 = ((int64_t *) first_idx2_mem_41983.mem)[u8_res_40167];
            bool cond_40173 = slt64(spi_40172, (int64_t) 0);
            int64_t defunc_res_40174;
            
            if (cond_40173) {
                defunc_res_40174 = (int64_t) -1;
            } else {
                int64_t zeze_rhs_40175 = ((int64_t *) last_idx2_mem_41984.mem)[u8_res_40167];
                bool cond_40176 = spi_40172 == zeze_rhs_40175;
                int64_t defunc_res_f_res_40177;
                
                if (cond_40176) {
                    defunc_res_f_res_40177 = spi_40172;
                } else {
                    int64_t zs_lhs_40178 = sub64(zeze_rhs_40175, spi_40172);
                    int64_t max_arg1_40179 = sdiv64(zs_lhs_40178, (int64_t) 2);
                    int64_t max_res_40180 = smax64((int64_t) 1, max_arg1_40179);
                    bool loop_cond_40181 = slt64((int64_t) 0, max_res_40180);
                    bool bsearch_40182;
                    int64_t bsearch_40183;
                    int64_t bsearch_40184;
                    bool loop_while_40185;
                    int64_t heshi_40186;
                    int64_t step_40187;
                    
                    loop_while_40185 = loop_cond_40181;
                    heshi_40186 = spi_40172;
                    step_40187 = max_res_40180;
                    while (loop_while_40185) {
                        bool x_40188 = sle64((int64_t) 0, heshi_40186);
                        bool y_40189 = slt64(heshi_40186, dz2083U_25954);
                        bool bounds_check_40190 = x_40188 && y_40189;
                        bool index_certs_40191;
                        
                        if (!bounds_check_40190) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) heshi_40186, "] out of bounds for array of shape [", (long long) dz2083U_25954, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:539:25-40\n   #1  ../../../joins/ftHashJoin_old.fut:592:3-599:86\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-851:51\n   #4  test_join2.fut:16:1-29:36\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t cur_S_40209 = ((int64_t *) bounds2_mem_41981.mem)[heshi_40186];
                        bool x_40210 = sle64((int64_t) 0, cur_S_40209);
                        bool y_40211 = slt64(cur_S_40209, n2_25952);
                        bool bounds_check_40212 = x_40210 && y_40211;
                        bool index_certs_40213;
                        
                        if (!bounds_check_40212) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) cur_S_40209, "] out of bounds for array of shape [", (long long) n2_25952, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:542:21-40\n   #1  ../../../joins/ftHashJoin_old.fut:592:3-599:86\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-851:51\n   #4  test_join2.fut:16:1-29:36\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int32_t cur_depth_40192 = ((int32_t *) depths2_mem_41982.mem)[heshi_40186];
                        bool cond_40193 = heshi_40186 == spi_40172;
                        int32_t prev_depth_40194;
                        
                        if (cond_40193) {
                            prev_depth_40194 = cur_depth_40192;
                        } else {
                            int64_t tmp_40195 = sub64(heshi_40186, (int64_t) 1);
                            bool x_40196 = sle64((int64_t) 0, tmp_40195);
                            bool y_40197 = slt64(tmp_40195, dz2083U_25954);
                            bool bounds_check_40198 = x_40196 && y_40197;
                            bool index_certs_40199;
                            
                            if (!bounds_check_40198) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_40195, "] out of bounds for array of shape [", (long long) dz2083U_25954, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:540:60-77\n   #1  ../../../joins/ftHashJoin_old.fut:592:3-599:86\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-851:51\n   #4  test_join2.fut:16:1-29:36\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int32_t prev_depth_f_res_40200 = ((int32_t *) depths2_mem_41982.mem)[tmp_40195];
                            
                            prev_depth_40194 = prev_depth_f_res_40200;
                        }
                        
                        bool cond_40201 = heshi_40186 == zeze_rhs_40175;
                        int32_t next_depth_40202;
                        
                        if (cond_40201) {
                            next_depth_40202 = cur_depth_40192;
                        } else {
                            int64_t tmp_40203 = add64((int64_t) 1, heshi_40186);
                            bool x_40204 = sle64((int64_t) 0, tmp_40203);
                            bool y_40205 = slt64(tmp_40203, dz2083U_25954);
                            bool bounds_check_40206 = x_40204 && y_40205;
                            bool index_certs_40207;
                            
                            if (!bounds_check_40206) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_40203, "] out of bounds for array of shape [", (long long) dz2083U_25954, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:541:64-81\n   #1  ../../../joins/ftHashJoin_old.fut:592:3-599:86\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-851:51\n   #4  test_join2.fut:16:1-29:36\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int32_t next_depth_f_res_40208 = ((int32_t *) depths2_mem_41982.mem)[tmp_40203];
                            
                            next_depth_40202 = next_depth_f_res_40208;
                        }
                        
                        int8_t cur_S_40214 = ((int8_t *) mem_42415)[cur_S_40209];
                        int8_t prev_S_40215;
                        
                        if (cond_40193) {
                            prev_S_40215 = cur_S_40214;
                        } else {
                            int64_t tmp_40216 = sub64(heshi_40186, (int64_t) 1);
                            bool x_40217 = sle64((int64_t) 0, tmp_40216);
                            bool y_40218 = slt64(tmp_40216, dz2083U_25954);
                            bool bounds_check_40219 = x_40217 && y_40218;
                            bool index_certs_40220;
                            
                            if (!bounds_check_40219) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_40216, "] out of bounds for array of shape [", (long long) dz2083U_25954, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:543:55-72\n   #1  ../../../joins/ftHashJoin_old.fut:592:3-599:86\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-851:51\n   #4  test_join2.fut:16:1-29:36\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t tmp_40221 = ((int64_t *) bounds2_mem_41981.mem)[tmp_40216];
                            bool x_40222 = sle64((int64_t) 0, tmp_40221);
                            bool y_40223 = slt64(tmp_40221, n2_25952);
                            bool bounds_check_40224 = x_40222 && y_40223;
                            bool index_certs_40225;
                            
                            if (!bounds_check_40224) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_40221, "] out of bounds for array of shape [", (long long) n2_25952, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:543:52-73\n   #1  ../../../joins/ftHashJoin_old.fut:592:3-599:86\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-851:51\n   #4  test_join2.fut:16:1-29:36\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int8_t prev_S_f_res_40226 = ((int8_t *) mem_42415)[tmp_40221];
                            
                            prev_S_40215 = prev_S_f_res_40226;
                        }
                        
                        int8_t next_S_40227;
                        
                        if (cond_40201) {
                            next_S_40227 = cur_S_40214;
                        } else {
                            int64_t tmp_40228 = add64((int64_t) 1, heshi_40186);
                            bool x_40229 = sle64((int64_t) 0, tmp_40228);
                            bool y_40230 = slt64(tmp_40228, dz2083U_25954);
                            bool bounds_check_40231 = x_40229 && y_40230;
                            bool index_certs_40232;
                            
                            if (!bounds_check_40231) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_40228, "] out of bounds for array of shape [", (long long) dz2083U_25954, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:544:59-76\n   #1  ../../../joins/ftHashJoin_old.fut:592:3-599:86\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-851:51\n   #4  test_join2.fut:16:1-29:36\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t tmp_40233 = ((int64_t *) bounds2_mem_41981.mem)[tmp_40228];
                            bool x_40234 = sle64((int64_t) 0, tmp_40233);
                            bool y_40235 = slt64(tmp_40233, n2_25952);
                            bool bounds_check_40236 = x_40234 && y_40235;
                            bool index_certs_40237;
                            
                            if (!bounds_check_40236) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_40233, "] out of bounds for array of shape [", (long long) n2_25952, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:544:56-77\n   #1  ../../../joins/ftHashJoin_old.fut:592:3-599:86\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-851:51\n   #4  test_join2.fut:16:1-29:36\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int8_t next_S_f_res_40238 = ((int8_t *) mem_42415)[tmp_40233];
                            
                            next_S_40227 = next_S_f_res_40238;
                        }
                        
                        int32_t prim_get_radix_arg0_40239 = mul32(f64_res_31066, cur_depth_40192);
                        int64_t unsign_arg0_40240 = zext_i32_i64(prim_get_radix_arg0_40239);
                        int64_t asPow_40241 = shl64((int64_t) 1, unsign_arg0_40240);
                        int64_t u64_arg0_40242 = sub64(asPow_40241, (int64_t) 1);
                        int8_t u64_res_40243 = zext_i64_i8(u64_arg0_40242);
                        int8_t lifted_lambda_res_40244 = defunc_0_byteSeq_to_prim_res_40151 & u64_res_40243;
                        int8_t lifted_lambda_res_40245 = cur_S_40214 & u64_res_40243;
                        bool defunc_0_eq_res_40246 = lifted_lambda_res_40244 == lifted_lambda_res_40245;
                        int64_t loopres_40247;
                        int64_t loopres_40248;
                        
                        if (defunc_0_eq_res_40246) {
                            loopres_40247 = heshi_40186;
                            loopres_40248 = (int64_t) 0;
                        } else {
                            bool loop_cond_t_res_40249 = slt32(0, cur_depth_40192);
                            bool is_lt_40250;
                            bool is_lt_40251;
                            int32_t is_lt_40252;
                            bool loop_while_40253;
                            bool def_lt_40254;
                            int32_t bi_40255;
                            
                            loop_while_40253 = loop_cond_t_res_40249;
                            def_lt_40254 = 0;
                            bi_40255 = 0;
                            while (loop_while_40253) {
                                int32_t zt_rhs_40256 = add32(1, bi_40255);
                                int32_t prim_get_radix_arg0_40257 = mul32(f64_res_31066, zt_rhs_40256);
                                int64_t unsign_arg0_40258 = zext_i32_i64(prim_get_radix_arg0_40257);
                                int64_t asPow_40259 = shl64((int64_t) 1, unsign_arg0_40258);
                                int64_t u64_arg0_40260 = sub64(asPow_40259, (int64_t) 1);
                                int8_t u64_res_40261 = zext_i64_i8(u64_arg0_40260);
                                int8_t lifted_lambda_res_40262 = defunc_0_byteSeq_to_prim_res_40151 & u64_res_40261;
                                int8_t lifted_lambda_res_40263 = cur_S_40214 & u64_res_40261;
                                bool defunc_0_lt_res_40264 = ult8(lifted_lambda_res_40262, lifted_lambda_res_40263);
                                bool defunc_0_lt_res_40265 = ult8(lifted_lambda_res_40263, lifted_lambda_res_40262);
                                bool cond_40266 = !defunc_0_lt_res_40265;
                                bool cond_t_res_40267 = !defunc_0_lt_res_40264;
                                bool x_40268 = cond_40266 && cond_t_res_40267;
                                bool loop_cond_t_res_40269 = slt32(zt_rhs_40256, cur_depth_40192);
                                bool x_40270 = x_40268 && loop_cond_t_res_40269;
                                bool loop_while_tmp_42584 = x_40270;
                                bool def_lt_tmp_42585 = defunc_0_lt_res_40264;
                                int32_t bi_tmp_42586 = zt_rhs_40256;
                                
                                loop_while_40253 = loop_while_tmp_42584;
                                def_lt_40254 = def_lt_tmp_42585;
                                bi_40255 = bi_tmp_42586;
                            }
                            is_lt_40250 = loop_while_40253;
                            is_lt_40251 = def_lt_40254;
                            is_lt_40252 = bi_40255;
                            
                            int64_t loopres_f_res_40271;
                            int64_t loopres_f_res_40272;
                            
                            if (is_lt_40251) {
                                bool loop_cond_t_res_40273 = slt32(0, prev_depth_40194);
                                bool is_gt_40274;
                                bool is_gt_40275;
                                int32_t is_gt_40276;
                                bool loop_while_40277;
                                bool def_gt_40278;
                                int32_t bi_40279;
                                
                                loop_while_40277 = loop_cond_t_res_40273;
                                def_gt_40278 = 0;
                                bi_40279 = 0;
                                while (loop_while_40277) {
                                    int32_t zt_rhs_40280 = add32(1, bi_40279);
                                    int32_t prim_get_radix_arg0_40281 = mul32(f64_res_31066, zt_rhs_40280);
                                    int64_t unsign_arg0_40282 = zext_i32_i64(prim_get_radix_arg0_40281);
                                    int64_t asPow_40283 = shl64((int64_t) 1, unsign_arg0_40282);
                                    int64_t u64_arg0_40284 = sub64(asPow_40283, (int64_t) 1);
                                    int8_t u64_res_40285 = zext_i64_i8(u64_arg0_40284);
                                    int8_t lifted_lambda_res_40286 = defunc_0_byteSeq_to_prim_res_40151 & u64_res_40285;
                                    int8_t lifted_lambda_res_40287 = prev_S_40215 & u64_res_40285;
                                    bool defunc_0_gt_res_40288 = ult8(lifted_lambda_res_40287, lifted_lambda_res_40286);
                                    bool defunc_0_gt_res_40289 = ult8(lifted_lambda_res_40286, lifted_lambda_res_40287);
                                    bool cond_40290 = !defunc_0_gt_res_40288;
                                    bool cond_t_res_40291 = !defunc_0_gt_res_40289;
                                    bool x_40292 = cond_40290 && cond_t_res_40291;
                                    bool loop_cond_t_res_40293 = slt32(zt_rhs_40280, prev_depth_40194);
                                    bool x_40294 = x_40292 && loop_cond_t_res_40293;
                                    bool loop_while_tmp_42587 = x_40294;
                                    bool def_gt_tmp_42588 = defunc_0_gt_res_40288;
                                    int32_t bi_tmp_42589 = zt_rhs_40280;
                                    
                                    loop_while_40277 = loop_while_tmp_42587;
                                    def_gt_40278 = def_gt_tmp_42588;
                                    bi_40279 = bi_tmp_42589;
                                }
                                is_gt_40274 = loop_while_40277;
                                is_gt_40275 = def_gt_40278;
                                is_gt_40276 = bi_40279;
                                
                                bool x_40295 = !cond_40193;
                                bool y_40296 = is_gt_40275 && x_40295;
                                bool cond_40297 = cond_40193 || y_40296;
                                int64_t loopres_f_res_t_res_40298;
                                int64_t loopres_f_res_t_res_40299;
                                
                                if (cond_40297) {
                                    loopres_f_res_t_res_40298 = (int64_t) -1;
                                    loopres_f_res_t_res_40299 = (int64_t) 0;
                                } else {
                                    int64_t tmp_40300 = sub64(heshi_40186, step_40187);
                                    int64_t max_arg1_40301 = sdiv64(step_40187, (int64_t) 2);
                                    int64_t max_res_40302 = smax64((int64_t) 1, max_arg1_40301);
                                    
                                    loopres_f_res_t_res_40298 = tmp_40300;
                                    loopres_f_res_t_res_40299 = max_res_40302;
                                }
                                loopres_f_res_40271 = loopres_f_res_t_res_40298;
                                loopres_f_res_40272 = loopres_f_res_t_res_40299;
                            } else {
                                bool loop_cond_t_res_40303 = slt32(0, next_depth_40202);
                                bool is_lt_40304;
                                bool is_lt_40305;
                                int32_t is_lt_40306;
                                bool loop_while_40307;
                                bool def_lt_40308;
                                int32_t bi_40309;
                                
                                loop_while_40307 = loop_cond_t_res_40303;
                                def_lt_40308 = 0;
                                bi_40309 = 0;
                                while (loop_while_40307) {
                                    int32_t zt_rhs_40310 = add32(1, bi_40309);
                                    int32_t prim_get_radix_arg0_40311 = mul32(f64_res_31066, zt_rhs_40310);
                                    int64_t unsign_arg0_40312 = zext_i32_i64(prim_get_radix_arg0_40311);
                                    int64_t asPow_40313 = shl64((int64_t) 1, unsign_arg0_40312);
                                    int64_t u64_arg0_40314 = sub64(asPow_40313, (int64_t) 1);
                                    int8_t u64_res_40315 = zext_i64_i8(u64_arg0_40314);
                                    int8_t lifted_lambda_res_40316 = defunc_0_byteSeq_to_prim_res_40151 & u64_res_40315;
                                    int8_t lifted_lambda_res_40317 = next_S_40227 & u64_res_40315;
                                    bool defunc_0_lt_res_40318 = ult8(lifted_lambda_res_40316, lifted_lambda_res_40317);
                                    bool defunc_0_lt_res_40319 = ult8(lifted_lambda_res_40317, lifted_lambda_res_40316);
                                    bool cond_40320 = !defunc_0_lt_res_40319;
                                    bool cond_t_res_40321 = !defunc_0_lt_res_40318;
                                    bool x_40322 = cond_40320 && cond_t_res_40321;
                                    bool loop_cond_t_res_40323 = slt32(zt_rhs_40310, next_depth_40202);
                                    bool x_40324 = x_40322 && loop_cond_t_res_40323;
                                    bool loop_while_tmp_42590 = x_40324;
                                    bool def_lt_tmp_42591 = defunc_0_lt_res_40318;
                                    int32_t bi_tmp_42592 = zt_rhs_40310;
                                    
                                    loop_while_40307 = loop_while_tmp_42590;
                                    def_lt_40308 = def_lt_tmp_42591;
                                    bi_40309 = bi_tmp_42592;
                                }
                                is_lt_40304 = loop_while_40307;
                                is_lt_40305 = def_lt_40308;
                                is_lt_40306 = bi_40309;
                                
                                bool x_40325 = !cond_40201;
                                bool y_40326 = is_lt_40305 && x_40325;
                                bool cond_40327 = cond_40201 || y_40326;
                                int64_t loopres_f_res_f_res_40328;
                                int64_t loopres_f_res_f_res_40329;
                                
                                if (cond_40327) {
                                    loopres_f_res_f_res_40328 = (int64_t) -1;
                                    loopres_f_res_f_res_40329 = (int64_t) 0;
                                } else {
                                    int64_t tmp_40330 = add64(heshi_40186, step_40187);
                                    int64_t max_arg1_40331 = sdiv64(step_40187, (int64_t) 2);
                                    int64_t max_res_40332 = smax64((int64_t) 1, max_arg1_40331);
                                    
                                    loopres_f_res_f_res_40328 = tmp_40330;
                                    loopres_f_res_f_res_40329 = max_res_40332;
                                }
                                loopres_f_res_40271 = loopres_f_res_f_res_40328;
                                loopres_f_res_40272 = loopres_f_res_f_res_40329;
                            }
                            loopres_40247 = loopres_f_res_40271;
                            loopres_40248 = loopres_f_res_40272;
                        }
                        
                        bool loop_cond_40333 = slt64((int64_t) 0, loopres_40248);
                        bool loop_while_tmp_42581 = loop_cond_40333;
                        int64_t heshi_tmp_42582 = loopres_40247;
                        int64_t step_tmp_42583 = loopres_40248;
                        
                        loop_while_40185 = loop_while_tmp_42581;
                        heshi_40186 = heshi_tmp_42582;
                        step_40187 = step_tmp_42583;
                    }
                    bsearch_40182 = loop_while_40185;
                    bsearch_40183 = heshi_40186;
                    bsearch_40184 = step_40187;
                    defunc_res_f_res_40177 = bsearch_40183;
                }
                defunc_res_40174 = defunc_res_f_res_40177;
            }
            
            bool cond_40334 = sle64(defunc_res_40174, (int64_t) 0);
            int64_t inf_40335;
            
            if (cond_40334) {
                inf_40335 = (int64_t) 0;
            } else {
                bool x_40336 = sle64((int64_t) 0, defunc_res_40174);
                bool y_40337 = slt64(defunc_res_40174, dz2083U_25954);
                bool bounds_check_40338 = x_40336 && y_40337;
                bool index_certs_40339;
                
                if (!bounds_check_40338) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) defunc_res_40174, "] out of bounds for array of shape [", (long long) dz2083U_25954, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:600:41-57\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin_old.fut:848:4-851:51\n   #3  test_join2.fut:16:1-29:36\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                int64_t inf_f_res_40340 = ((int64_t *) bounds2_mem_41981.mem)[defunc_res_40174];
                
                inf_40335 = inf_f_res_40340;
            }
            
            bool cond_40341 = defunc_res_40174 == zeze_rhs_31081;
            int64_t sup_40342;
            
            if (cond_40341) {
                sup_40342 = n2_25952;
            } else {
                int64_t tmp_40343 = add64((int64_t) 1, defunc_res_40174);
                bool x_40344 = sle64((int64_t) 0, tmp_40343);
                bool y_40345 = slt64(tmp_40343, dz2083U_25954);
                bool bounds_check_40346 = x_40344 && y_40345;
                bool index_certs_40347;
                
                if (!bounds_check_40346) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_40343, "] out of bounds for array of shape [", (long long) dz2083U_25954, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:601:47-65\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin_old.fut:848:4-851:51\n   #3  test_join2.fut:16:1-29:36\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                int64_t sup_f_res_40348 = ((int64_t *) bounds2_mem_41981.mem)[tmp_40343];
                
                sup_40342 = sup_f_res_40348;
            }
            
            bool loop_cond_40349 = slt64(inf_40335, sup_40342);
            bool lifted_lambda_res_40350;
            int64_t lifted_lambda_res_40351;
            int64_t lifted_lambda_res_40352;
            int64_t lifted_lambda_res_40353;
            bool loop_while_40354;
            int64_t j_40355;
            int64_t duome_40356;
            int64_t i_40357;
            
            loop_while_40354 = loop_cond_40349;
            j_40355 = inf_40335;
            duome_40356 = (int64_t) 0;
            i_40357 = inf_40335;
            while (loop_while_40354) {
                bool x_40358 = sle64((int64_t) 0, i_40357);
                bool y_40359 = slt64(i_40357, n2_25952);
                bool bounds_check_40360 = x_40358 && y_40359;
                bool index_certs_40361;
                
                if (!bounds_check_40360) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_40357, "] out of bounds for array of shape [", (long long) n2_25952, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:604:29-38\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin_old.fut:848:4-851:51\n   #3  test_join2.fut:16:1-29:36\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                int8_t zeze_rhs_40362 = ((int8_t *) mem_42415)[i_40357];
                bool is_eq_40363 = defunc_0_byteSeq_to_prim_res_40151 == zeze_rhs_40362;
                bool cond_40364 = slt64((int64_t) 0, duome_40356);
                bool cond_40365 = is_eq_40363 || cond_40364;
                int64_t next_j_40366;
                
                if (cond_40365) {
                    next_j_40366 = j_40355;
                } else {
                    int64_t next_j_f_res_40367 = add64((int64_t) 1, j_40355);
                    
                    next_j_40366 = next_j_f_res_40367;
                }
                
                int64_t next_duome_40368;
                
                if (is_eq_40363) {
                    int64_t next_duome_t_res_40369 = add64((int64_t) 1, duome_40356);
                    
                    next_duome_40368 = next_duome_t_res_40369;
                } else {
                    next_duome_40368 = duome_40356;
                }
                
                int64_t tmp_40370 = add64((int64_t) 1, i_40357);
                bool loop_cond_40371 = slt64(tmp_40370, sup_40342);
                bool loop_while_tmp_42593 = loop_cond_40371;
                int64_t j_tmp_42594 = next_j_40366;
                int64_t duome_tmp_42595 = next_duome_40368;
                int64_t i_tmp_42596 = tmp_40370;
                
                loop_while_40354 = loop_while_tmp_42593;
                j_40355 = j_tmp_42594;
                duome_40356 = duome_tmp_42595;
                i_40357 = i_tmp_42596;
            }
            lifted_lambda_res_40350 = loop_while_40354;
            lifted_lambda_res_40351 = j_40355;
            lifted_lambda_res_40352 = duome_40356;
            lifted_lambda_res_40353 = i_40357;
            
            bool cond_40372 = slt64(lifted_lambda_res_40351, sup_40342);
            int64_t lifted_lambda_res_40373;
            
            if (cond_40372) {
                lifted_lambda_res_40373 = lifted_lambda_res_40352;
            } else {
                lifted_lambda_res_40373 = (int64_t) 0;
            }
            
            int64_t lifted_lambda_res_40374;
            
            if (cond_40372) {
                lifted_lambda_res_40374 = lifted_lambda_res_40351;
            } else {
                lifted_lambda_res_40374 = (int64_t) -1;
            }
            
            bool lifted_lambda_res_40375 = slt64((int64_t) 1, lifted_lambda_res_40373);
            int64_t defunc_0_f_res_40376 = btoi_bool_i64(lifted_lambda_res_40375);
            int64_t defunc_0_op_res_40379 = add64(defunc_0_f_res_40376, scanacc_41719);
            int64_t defunc_0_op_res_40382 = add64(lifted_lambda_res_40373, scanacc_41720);
            int64_t zp_res_40385 = add64(lifted_lambda_res_40373, redout_41723);
            
            ((int64_t *) mem_42423)[i_41728] = defunc_0_op_res_40379;
            ((int64_t *) mem_42425)[i_41728] = defunc_0_op_res_40382;
            ((int64_t *) mem_42427)[i_41728] = defunc_0_f_res_40376;
            ((int64_t *) mem_42429)[i_41728] = lifted_lambda_res_40374;
            ((int64_t *) mem_42431)[i_41728] = lifted_lambda_res_40373;
            ((int8_t *) mem_42432)[i_41728] = defunc_0_byteSeq_to_prim_res_40151;
            
            int64_t scanacc_tmp_42571 = defunc_0_op_res_40379;
            int64_t scanacc_tmp_42572 = defunc_0_op_res_40382;
            int64_t redout_tmp_42575 = zp_res_40385;
            
            scanacc_41719 = scanacc_tmp_42571;
            scanacc_41720 = scanacc_tmp_42572;
            redout_41723 = redout_tmp_42575;
        }
        discard_41735 = scanacc_41719;
        discard_41736 = scanacc_41720;
        defunc_res_40145 = redout_41723;
        
        int64_t m_f_res_40413;
        
        if (x_31083) {
            int64_t x_40414 = ((int64_t *) mem_42423)[tmp_31084];
            
            m_f_res_40413 = x_40414;
        } else {
            m_f_res_40413 = (int64_t) 0;
        }
        
        int64_t m_40415;
        
        if (cond_31082) {
            m_40415 = (int64_t) 0;
        } else {
            m_40415 = m_f_res_40413;
        }
        
        bool cond_40416 = slt64((int64_t) 0, m_40415);
        int64_t m_40417 = sub64(m_40415, (int64_t) 1);
        bool i_p_m_t_s_leq_w_40418 = slt64(m_40417, n1_25949);
        bool zzero_leq_i_p_m_t_s_40419 = sle64((int64_t) 0, m_40417);
        bool y_40420 = i_p_m_t_s_leq_w_40418 && zzero_leq_i_p_m_t_s_40419;
        bool i_lte_j_40421 = sle64((int64_t) 0, m_40415);
        bool forwards_ok_40422 = y_40420 && i_lte_j_40421;
        bool eq_x_zz_40423 = (int64_t) 0 == m_f_res_40413;
        bool p_and_eq_x_y_40424 = x_31083 && eq_x_zz_40423;
        bool empty_slice_40425 = cond_31082 || p_and_eq_x_y_40424;
        bool ok_or_empty_40426 = forwards_ok_40422 || empty_slice_40425;
        bool index_certs_40427;
        
        if (!ok_or_empty_40426) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_40415, "] out of bounds for array of shape [", (long long) n1_25949, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin_old.fut:848:4-851:51\n   #3  test_join2.fut:16:1-29:36\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_42477 = (int64_t) 8 * m_40415;
        
        if (mem_42478_cached_sizze_42795 < bytes_42477) {
            err = lexical_realloc(ctx, &mem_42478, &mem_42478_cached_sizze_42795, bytes_42477);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_42478, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_42431, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_40415});
        
        int64_t max_mult_40430;
        
        if (cond_40416) {
            if (mem_42480_cached_sizze_42796 < bytes_41993) {
                err = lexical_realloc(ctx, &mem_42480, &mem_42480_cached_sizze_42796, bytes_41993);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            for (int64_t i_41743 = 0; i_41743 < n1_25949; i_41743++) {
                int64_t eta_p_40432 = ((int64_t *) mem_42427)[i_41743];
                int64_t eta_p_40433 = ((int64_t *) mem_42423)[i_41743];
                bool cond_40434 = eta_p_40432 == (int64_t) 1;
                int64_t lifted_lambda_res_40435;
                
                if (cond_40434) {
                    int64_t lifted_lambda_res_t_res_40436 = sub64(eta_p_40433, (int64_t) 1);
                    
                    lifted_lambda_res_40435 = lifted_lambda_res_t_res_40436;
                } else {
                    lifted_lambda_res_40435 = (int64_t) -1;
                }
                ((int64_t *) mem_42480)[i_41743] = lifted_lambda_res_40435;
            }
            for (int64_t write_iter_41745 = 0; write_iter_41745 < n1_25949; write_iter_41745++) {
                int64_t write_iv_41747 = ((int64_t *) mem_42480)[write_iter_41745];
                int64_t write_iv_41748 = ((int64_t *) mem_42431)[write_iter_41745];
                
                if (sle64((int64_t) 0, write_iv_41747) && slt64(write_iv_41747, m_40415)) {
                    ((int64_t *) mem_42478)[write_iv_41747] = write_iv_41748;
                }
            }
            
            int64_t defunc_0_reduce_res_40448;
            int64_t redout_41750 = (int64_t) -9223372036854775808;
            
            for (int64_t i_41751 = 0; i_41751 < m_40415; i_41751++) {
                int64_t x_40449 = ((int64_t *) mem_42478)[i_41751];
                int64_t max_res_40452 = smax64(x_40449, redout_41750);
                int64_t redout_tmp_42599 = max_res_40452;
                
                redout_41750 = redout_tmp_42599;
            }
            defunc_0_reduce_res_40448 = redout_41750;
            max_mult_40430 = defunc_0_reduce_res_40448;
        } else {
            max_mult_40430 = (int64_t) 1;
        }
        
        int64_t bytes_42493 = (int64_t) 8 * defunc_res_40145;
        bool bounds_invalid_upwards_40480 = slt64(max_mult_40430, (int64_t) 1);
        bool valid_40482 = !bounds_invalid_upwards_40480;
        bool range_valid_c_40483;
        
        if (!valid_40482) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 1, "..<", (long long) max_mult_40430, " is invalid.", "-> #0  ../../../joins/ftHashJoin_old.fut:627:18-30\n   #1  ../../../joins/ftHashJoin_old.fut:848:4-851:51\n   #2  test_join2.fut:16:1-29:36\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_42531 = b_25950 * defunc_res_40145;
        
        if (mem_42470_cached_sizze_42794 < bytes_41993) {
            err = lexical_realloc(ctx, &mem_42470, &mem_42470_cached_sizze_42794, bytes_41993);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t i_41739 = 0; i_41739 < n1_25949; i_41739++) {
            int64_t eta_p_40403 = ((int64_t *) mem_42431)[i_41739];
            int64_t zv_lhs_40404 = add64((int64_t) -1, i_41739);
            int64_t tmp_40405 = smod64(zv_lhs_40404, n1_25949);
            int64_t lifted_lambda_res_40406 = ((int64_t *) mem_42425)[tmp_40405];
            bool cond_40407 = i_41739 == (int64_t) 0;
            int64_t lifted_lambda_res_40408;
            
            if (cond_40407) {
                lifted_lambda_res_40408 = (int64_t) 0;
            } else {
                lifted_lambda_res_40408 = lifted_lambda_res_40406;
            }
            
            bool cond_40409 = slt64((int64_t) 0, eta_p_40403);
            int64_t lifted_lambda_res_40410;
            
            if (cond_40409) {
                lifted_lambda_res_40410 = lifted_lambda_res_40408;
            } else {
                lifted_lambda_res_40410 = (int64_t) -1;
            }
            ((int64_t *) mem_42470)[i_41739] = lifted_lambda_res_40410;
        }
        if (memblock_alloc(ctx, &mem_42494, bytes_42493, "mem_42494")) {
            err = 1;
            goto cleanup;
        }
        for (int64_t nest_i_42601 = 0; nest_i_42601 < defunc_res_40145; nest_i_42601++) {
            ((int64_t *) mem_42494.mem)[nest_i_42601] = (int64_t) 0;
        }
        for (int64_t write_iter_41752 = 0; write_iter_41752 < n1_25949; write_iter_41752++) {
            int64_t write_iv_41754 = ((int64_t *) mem_42470)[write_iter_41752];
            
            if (sle64((int64_t) 0, write_iv_41754) && slt64(write_iv_41754, defunc_res_40145)) {
                ((int64_t *) mem_42494.mem)[write_iv_41754] = write_iter_41752;
            }
        }
        if (memblock_alloc(ctx, &mem_42502, bytes_42493, "mem_42502")) {
            err = 1;
            goto cleanup;
        }
        for (int64_t nest_i_42603 = 0; nest_i_42603 < defunc_res_40145; nest_i_42603++) {
            ((int64_t *) mem_42502.mem)[nest_i_42603] = (int64_t) 1;
        }
        
        int64_t distance_40481 = sub64(max_mult_40430, (int64_t) 1);
        
        if (memblock_set(ctx, &mem_param_42505, &mem_42502, "mem_42502") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_42508, &mem_42494, "mem_42494") != 0)
            return 1;
        for (int64_t i_40486 = 0; i_40486 < distance_40481; i_40486++) {
            int64_t index_primexp_40489 = add64((int64_t) 1, i_40486);
            
            if (memblock_alloc(ctx, &mem_42510, bytes_42493, "mem_42510")) {
                err = 1;
                goto cleanup;
            }
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_42510.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_42508.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {defunc_res_40145});
            if (memblock_alloc(ctx, &mem_42512, bytes_42493, "mem_42512")) {
                err = 1;
                goto cleanup;
            }
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_42512.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_42505.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {defunc_res_40145});
            
            int64_t replicate_arg1_40492 = add64((int64_t) 2, i_40486);
            
            for (int64_t write_iter_41757 = 0; write_iter_41757 < n1_25949; write_iter_41757++) {
                int64_t zlze_lhs_40500 = ((int64_t *) mem_42431)[write_iter_41757];
                bool cond_40501 = sle64(zlze_lhs_40500, index_primexp_40489);
                int64_t lifted_lambda_res_40502;
                
                if (cond_40501) {
                    lifted_lambda_res_40502 = (int64_t) -1;
                } else {
                    int64_t zp_lhs_40503 = ((int64_t *) mem_42470)[write_iter_41757];
                    int64_t lifted_lambda_res_f_res_40504 = add64(index_primexp_40489, zp_lhs_40503);
                    
                    lifted_lambda_res_40502 = lifted_lambda_res_f_res_40504;
                }
                if (sle64((int64_t) 0, lifted_lambda_res_40502) && slt64(lifted_lambda_res_40502, defunc_res_40145)) {
                    ((int64_t *) mem_42512.mem)[lifted_lambda_res_40502] = replicate_arg1_40492;
                }
                if (sle64((int64_t) 0, lifted_lambda_res_40502) && slt64(lifted_lambda_res_40502, defunc_res_40145)) {
                    ((int64_t *) mem_42510.mem)[lifted_lambda_res_40502] = write_iter_41757;
                }
            }
            if (memblock_set(ctx, &mem_param_tmp_42604, &mem_42512, "mem_42512") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_tmp_42605, &mem_42510, "mem_42510") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_42505, &mem_param_tmp_42604, "mem_param_tmp_42604") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_42508, &mem_param_tmp_42605, "mem_param_tmp_42605") != 0)
                return 1;
        }
        if (memblock_set(ctx, &ext_mem_42530, &mem_param_42505, "mem_param_42505") != 0)
            return 1;
        if (memblock_set(ctx, &ext_mem_42529, &mem_param_42508, "mem_param_42508") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_42494, "mem_42494") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_42502, "mem_42502") != 0)
            return 1;
        if (memblock_alloc(ctx, &mem_42532, bytes_42531, "mem_42532")) {
            err = 1;
            goto cleanup;
        }
        if (memblock_alloc(ctx, &mem_42534, bytes_42493, "mem_42534")) {
            err = 1;
            goto cleanup;
        }
        for (int64_t i_41767 = 0; i_41767 < defunc_res_40145; i_41767++) {
            int64_t eta_p_40514 = ((int64_t *) ext_mem_42529.mem)[i_41767];
            bool x_40515 = sle64((int64_t) 0, eta_p_40514);
            bool y_40516 = slt64(eta_p_40514, n1_25949);
            bool bounds_check_40517 = x_40515 && y_40516;
            bool index_certs_40518;
            
            if (!bounds_check_40517) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_40514, "] out of bounds for array of shape [", (long long) n1_25949, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:637:16-26\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin_old.fut:636:8-644:6\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-851:51\n   #4  test_join2.fut:16:1-29:36\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t eta_p_40513 = ((int64_t *) ext_mem_42530.mem)[i_41767];
            int8_t rv_40519 = ((int8_t *) mem_42432)[eta_p_40514];
            int64_t inf_s_idx_40520 = ((int64_t *) mem_42429)[eta_p_40514];
            int64_t tmp_40521 = sub64(inf_s_idx_40520, (int64_t) 1);
            bool loop_cond_40522 = slt64((int64_t) 0, eta_p_40513);
            bool kth_match_40523;
            int64_t kth_match_40524;
            int64_t kth_match_40525;
            bool loop_while_40526;
            int64_t j_40527;
            int64_t m_40528;
            
            loop_while_40526 = loop_cond_40522;
            j_40527 = tmp_40521;
            m_40528 = (int64_t) 0;
            while (loop_while_40526) {
                int64_t zeze_rhs_40529 = add64((int64_t) 1, j_40527);
                bool x_40530 = sle64((int64_t) 0, zeze_rhs_40529);
                bool y_40531 = slt64(zeze_rhs_40529, n2_25952);
                bool bounds_check_40532 = x_40530 && y_40531;
                bool index_certs_40533;
                
                if (!bounds_check_40532) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) zeze_rhs_40529, "] out of bounds for array of shape [", (long long) n2_25952, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:641:29-40\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin_old.fut:636:8-644:6\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-851:51\n   #4  test_join2.fut:16:1-29:36\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                int8_t zeze_rhs_40534 = ((int8_t *) mem_42415)[zeze_rhs_40529];
                bool is_eq_40535 = rv_40519 == zeze_rhs_40534;
                int64_t loopres_40536;
                
                if (is_eq_40535) {
                    int64_t tmp_40537 = add64((int64_t) 1, m_40528);
                    
                    loopres_40536 = tmp_40537;
                } else {
                    loopres_40536 = m_40528;
                }
                
                bool loop_cond_40538 = slt64(loopres_40536, eta_p_40513);
                bool loop_while_tmp_42612 = loop_cond_40538;
                int64_t j_tmp_42613 = zeze_rhs_40529;
                int64_t m_tmp_42614 = loopres_40536;
                
                loop_while_40526 = loop_while_tmp_42612;
                j_40527 = j_tmp_42613;
                m_40528 = m_tmp_42614;
            }
            kth_match_40523 = loop_while_40526;
            kth_match_40524 = j_40527;
            kth_match_40525 = m_40528;
            lmad_copy_1b(ctx, 1, (uint8_t *) mem_42532.mem, i_41767 * b_25950, (int64_t []) {(int64_t) 1}, (uint8_t *) ks1_mem_41977.mem, eta_p_40514 * b_25950, (int64_t []) {(int64_t) 1}, (int64_t []) {b_25950});
            ((int64_t *) mem_42534.mem)[i_41767] = kth_match_40524;
        }
        if (memblock_unref(ctx, &ext_mem_42530, "ext_mem_42530") != 0)
            return 1;
        if (memblock_alloc(ctx, &mem_42550, bytes_42493, "mem_42550")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_42550.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_42529.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {defunc_res_40145});
        if (memblock_unref(ctx, &ext_mem_42529, "ext_mem_42529") != 0)
            return 1;
        if (memblock_set(ctx, &ext_mem_42558, &mem_42550, "mem_42550") != 0)
            return 1;
        if (memblock_set(ctx, &ext_mem_42557, &mem_42534, "mem_42534") != 0)
            return 1;
        if (memblock_set(ctx, &ext_mem_42554, &mem_42532, "mem_42532") != 0)
            return 1;
        radix_hash_join_res_31109 = defunc_res_40145;
        radix_hash_join_res_31110 = defunc_res_40145;
        radix_hash_join_res_31111 = defunc_res_40145;
    } else {
        int64_t radix_hash_join_res_f_res_31468;
        int64_t radix_hash_join_res_f_res_31469;
        int64_t radix_hash_join_res_f_res_31470;
        
        if (cond_31076) {
            if (mem_42270_cached_sizze_42777 < bytes_42269) {
                err = lexical_realloc(ctx, &mem_42270, &mem_42270_cached_sizze_42777, bytes_42269);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            for (int64_t i_41772 = 0; i_41772 < n2_25952; i_41772++) {
                int16_t defunc_0_byteSeq_to_prim_res_40633;
                int16_t y_40635 = (int16_t) 0;
                
                for (int64_t i_40634 = 0; i_40634 < b_25950; i_40634++) {
                    int64_t zm_lhs_40636 = sub64(b_25950, i_40634);
                    int64_t from_u8_arg0_40637 = sub64(zm_lhs_40636, (int64_t) 1);
                    bool x_40638 = sle64((int64_t) 0, from_u8_arg0_40637);
                    bool y_40639 = slt64(from_u8_arg0_40637, b_25950);
                    bool bounds_check_40640 = x_40638 && y_40639;
                    bool index_certs_40641;
                    
                    if (!bounds_check_40640) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) from_u8_arg0_40637, "] out of bounds for array of shape [", (long long) b_25950, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:417:22-30\n   #1  ../../../joins/ftHashJoin_old.fut:423:75-82\n   #2  test_join2.fut:16:1-29:36\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int8_t from_u8_arg0_40642 = ((int8_t *) ks2_mem_41979.mem)[i_41772 * b_25950 + from_u8_arg0_40637];
                    int16_t u8_res_40643 = zext_i8_i16(from_u8_arg0_40642);
                    int32_t i64_res_40644 = sext_i64_i32(i_40634);
                    int32_t shift_left_rhs_40645 = mul32(8, i64_res_40644);
                    int16_t unsign_arg0_40646 = zext_i32_i16(shift_left_rhs_40645);
                    int16_t lifted_lambda_res_40647 = shl16(u8_res_40643, unsign_arg0_40646);
                    int16_t unsign_arg0_40648 = y_40635 | lifted_lambda_res_40647;
                    int16_t y_tmp_42616 = unsign_arg0_40648;
                    
                    y_40635 = y_tmp_42616;
                }
                defunc_0_byteSeq_to_prim_res_40633 = y_40635;
                ((int16_t *) mem_42270)[i_41772] = defunc_0_byteSeq_to_prim_res_40633;
            }
            if (mem_42278_cached_sizze_42778 < bytes_41993) {
                err = lexical_realloc(ctx, &mem_42278, &mem_42278_cached_sizze_42778, bytes_41993);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            if (mem_42280_cached_sizze_42779 < bytes_41993) {
                err = lexical_realloc(ctx, &mem_42280, &mem_42280_cached_sizze_42779, bytes_41993);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            if (mem_42282_cached_sizze_42780 < bytes_41993) {
                err = lexical_realloc(ctx, &mem_42282, &mem_42282_cached_sizze_42780, bytes_41993);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            if (mem_42284_cached_sizze_42781 < bytes_41993) {
                err = lexical_realloc(ctx, &mem_42284, &mem_42284_cached_sizze_42781, bytes_41993);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            if (mem_42286_cached_sizze_42782 < bytes_41993) {
                err = lexical_realloc(ctx, &mem_42286, &mem_42286_cached_sizze_42782, bytes_41993);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            if (mem_42288_cached_sizze_42783 < bytes_42287) {
                err = lexical_realloc(ctx, &mem_42288, &mem_42288_cached_sizze_42783, bytes_42287);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            
            int64_t discard_41796;
            int64_t discard_41797;
            int64_t defunc_res_40654;
            int64_t scanacc_41780;
            int64_t scanacc_41781;
            int64_t redout_41784;
            
            scanacc_41780 = (int64_t) 0;
            scanacc_41781 = (int64_t) 0;
            redout_41784 = (int64_t) 0;
            for (int64_t i_41789 = 0; i_41789 < n1_25949; i_41789++) {
                int16_t defunc_0_byteSeq_to_prim_res_40660;
                int16_t y_40662 = (int16_t) 0;
                
                for (int64_t i_40661 = 0; i_40661 < b_25950; i_40661++) {
                    int64_t zm_lhs_40663 = sub64(b_25950, i_40661);
                    int64_t from_u8_arg0_40664 = sub64(zm_lhs_40663, (int64_t) 1);
                    bool x_40665 = sle64((int64_t) 0, from_u8_arg0_40664);
                    bool y_40666 = slt64(from_u8_arg0_40664, b_25950);
                    bool bounds_check_40667 = x_40665 && y_40666;
                    bool index_certs_40668;
                    
                    if (!bounds_check_40667) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) from_u8_arg0_40664, "] out of bounds for array of shape [", (long long) b_25950, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:417:22-30\n   #1  ../../../joins/ftHashJoin_old.fut:423:75-82\n   #2  test_join2.fut:16:1-29:36\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int8_t from_u8_arg0_40669 = ((int8_t *) ks1_mem_41977.mem)[i_41789 * b_25950 + from_u8_arg0_40664];
                    int16_t u8_res_40670 = zext_i8_i16(from_u8_arg0_40669);
                    int32_t i64_res_40671 = sext_i64_i32(i_40661);
                    int32_t shift_left_rhs_40672 = mul32(8, i64_res_40671);
                    int16_t unsign_arg0_40673 = zext_i32_i16(shift_left_rhs_40672);
                    int16_t lifted_lambda_res_40674 = shl16(u8_res_40670, unsign_arg0_40673);
                    int16_t unsign_arg0_40675 = y_40662 | lifted_lambda_res_40674;
                    int16_t y_tmp_42626 = unsign_arg0_40675;
                    
                    y_40662 = y_tmp_42626;
                }
                defunc_0_byteSeq_to_prim_res_40660 = y_40662;
                
                int16_t lifted_lambda_res_40676 = u64_res_31100 & defunc_0_byteSeq_to_prim_res_40660;
                int64_t u16_res_40677 = zext_i16_i64(lifted_lambda_res_40676);
                bool x_40678 = sle64((int64_t) 0, u16_res_40677);
                bool y_40679 = slt64(u16_res_40677, dzlz7bUZLztztZRz20U2z20UZLi32z20Uradix_sizzeZRz7dUzg_31070);
                bool bounds_check_40680 = x_40678 && y_40679;
                bool index_certs_40681;
                
                if (!bounds_check_40680) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) u16_res_40677, "] out of bounds for array of shape [", (long long) dzlz7bUZLztztZRz20U2z20UZLi32z20Uradix_sizzeZRz7dUzg_31070, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:530:13-35\n   #1  ../../../joins/ftHashJoin_old.fut:656:3-663:87\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-852:57\n   #4  test_join2.fut:16:1-29:36\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                int64_t spi_40682 = ((int64_t *) first_idx2_mem_41983.mem)[u16_res_40677];
                bool cond_40683 = slt64(spi_40682, (int64_t) 0);
                int64_t defunc_res_40684;
                
                if (cond_40683) {
                    defunc_res_40684 = (int64_t) -1;
                } else {
                    int64_t zeze_rhs_40685 = ((int64_t *) last_idx2_mem_41984.mem)[u16_res_40677];
                    bool cond_40686 = spi_40682 == zeze_rhs_40685;
                    int64_t defunc_res_f_res_40687;
                    
                    if (cond_40686) {
                        defunc_res_f_res_40687 = spi_40682;
                    } else {
                        int64_t zs_lhs_40688 = sub64(zeze_rhs_40685, spi_40682);
                        int64_t max_arg1_40689 = sdiv64(zs_lhs_40688, (int64_t) 2);
                        int64_t max_res_40690 = smax64((int64_t) 1, max_arg1_40689);
                        bool loop_cond_40691 = slt64((int64_t) 0, max_res_40690);
                        bool bsearch_40692;
                        int64_t bsearch_40693;
                        int64_t bsearch_40694;
                        bool loop_while_40695;
                        int64_t heshi_40696;
                        int64_t step_40697;
                        
                        loop_while_40695 = loop_cond_40691;
                        heshi_40696 = spi_40682;
                        step_40697 = max_res_40690;
                        while (loop_while_40695) {
                            bool x_40698 = sle64((int64_t) 0, heshi_40696);
                            bool y_40699 = slt64(heshi_40696, dz2083U_25954);
                            bool bounds_check_40700 = x_40698 && y_40699;
                            bool index_certs_40701;
                            
                            if (!bounds_check_40700) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) heshi_40696, "] out of bounds for array of shape [", (long long) dz2083U_25954, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:539:25-40\n   #1  ../../../joins/ftHashJoin_old.fut:656:3-663:87\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-852:57\n   #4  test_join2.fut:16:1-29:36\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t cur_S_40719 = ((int64_t *) bounds2_mem_41981.mem)[heshi_40696];
                            bool x_40720 = sle64((int64_t) 0, cur_S_40719);
                            bool y_40721 = slt64(cur_S_40719, n2_25952);
                            bool bounds_check_40722 = x_40720 && y_40721;
                            bool index_certs_40723;
                            
                            if (!bounds_check_40722) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) cur_S_40719, "] out of bounds for array of shape [", (long long) n2_25952, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:542:21-40\n   #1  ../../../joins/ftHashJoin_old.fut:656:3-663:87\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-852:57\n   #4  test_join2.fut:16:1-29:36\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int32_t cur_depth_40702 = ((int32_t *) depths2_mem_41982.mem)[heshi_40696];
                            bool cond_40703 = heshi_40696 == spi_40682;
                            int32_t prev_depth_40704;
                            
                            if (cond_40703) {
                                prev_depth_40704 = cur_depth_40702;
                            } else {
                                int64_t tmp_40705 = sub64(heshi_40696, (int64_t) 1);
                                bool x_40706 = sle64((int64_t) 0, tmp_40705);
                                bool y_40707 = slt64(tmp_40705, dz2083U_25954);
                                bool bounds_check_40708 = x_40706 && y_40707;
                                bool index_certs_40709;
                                
                                if (!bounds_check_40708) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_40705, "] out of bounds for array of shape [", (long long) dz2083U_25954, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:540:60-77\n   #1  ../../../joins/ftHashJoin_old.fut:656:3-663:87\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-852:57\n   #4  test_join2.fut:16:1-29:36\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int32_t prev_depth_f_res_40710 = ((int32_t *) depths2_mem_41982.mem)[tmp_40705];
                                
                                prev_depth_40704 = prev_depth_f_res_40710;
                            }
                            
                            bool cond_40711 = heshi_40696 == zeze_rhs_40685;
                            int32_t next_depth_40712;
                            
                            if (cond_40711) {
                                next_depth_40712 = cur_depth_40702;
                            } else {
                                int64_t tmp_40713 = add64((int64_t) 1, heshi_40696);
                                bool x_40714 = sle64((int64_t) 0, tmp_40713);
                                bool y_40715 = slt64(tmp_40713, dz2083U_25954);
                                bool bounds_check_40716 = x_40714 && y_40715;
                                bool index_certs_40717;
                                
                                if (!bounds_check_40716) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_40713, "] out of bounds for array of shape [", (long long) dz2083U_25954, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:541:64-81\n   #1  ../../../joins/ftHashJoin_old.fut:656:3-663:87\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-852:57\n   #4  test_join2.fut:16:1-29:36\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int32_t next_depth_f_res_40718 = ((int32_t *) depths2_mem_41982.mem)[tmp_40713];
                                
                                next_depth_40712 = next_depth_f_res_40718;
                            }
                            
                            int16_t cur_S_40724 = ((int16_t *) mem_42270)[cur_S_40719];
                            int16_t prev_S_40725;
                            
                            if (cond_40703) {
                                prev_S_40725 = cur_S_40724;
                            } else {
                                int64_t tmp_40726 = sub64(heshi_40696, (int64_t) 1);
                                bool x_40727 = sle64((int64_t) 0, tmp_40726);
                                bool y_40728 = slt64(tmp_40726, dz2083U_25954);
                                bool bounds_check_40729 = x_40727 && y_40728;
                                bool index_certs_40730;
                                
                                if (!bounds_check_40729) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_40726, "] out of bounds for array of shape [", (long long) dz2083U_25954, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:543:55-72\n   #1  ../../../joins/ftHashJoin_old.fut:656:3-663:87\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-852:57\n   #4  test_join2.fut:16:1-29:36\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t tmp_40731 = ((int64_t *) bounds2_mem_41981.mem)[tmp_40726];
                                bool x_40732 = sle64((int64_t) 0, tmp_40731);
                                bool y_40733 = slt64(tmp_40731, n2_25952);
                                bool bounds_check_40734 = x_40732 && y_40733;
                                bool index_certs_40735;
                                
                                if (!bounds_check_40734) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_40731, "] out of bounds for array of shape [", (long long) n2_25952, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:543:52-73\n   #1  ../../../joins/ftHashJoin_old.fut:656:3-663:87\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-852:57\n   #4  test_join2.fut:16:1-29:36\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int16_t prev_S_f_res_40736 = ((int16_t *) mem_42270)[tmp_40731];
                                
                                prev_S_40725 = prev_S_f_res_40736;
                            }
                            
                            int16_t next_S_40737;
                            
                            if (cond_40711) {
                                next_S_40737 = cur_S_40724;
                            } else {
                                int64_t tmp_40738 = add64((int64_t) 1, heshi_40696);
                                bool x_40739 = sle64((int64_t) 0, tmp_40738);
                                bool y_40740 = slt64(tmp_40738, dz2083U_25954);
                                bool bounds_check_40741 = x_40739 && y_40740;
                                bool index_certs_40742;
                                
                                if (!bounds_check_40741) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_40738, "] out of bounds for array of shape [", (long long) dz2083U_25954, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:544:59-76\n   #1  ../../../joins/ftHashJoin_old.fut:656:3-663:87\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-852:57\n   #4  test_join2.fut:16:1-29:36\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t tmp_40743 = ((int64_t *) bounds2_mem_41981.mem)[tmp_40738];
                                bool x_40744 = sle64((int64_t) 0, tmp_40743);
                                bool y_40745 = slt64(tmp_40743, n2_25952);
                                bool bounds_check_40746 = x_40744 && y_40745;
                                bool index_certs_40747;
                                
                                if (!bounds_check_40746) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_40743, "] out of bounds for array of shape [", (long long) n2_25952, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:544:56-77\n   #1  ../../../joins/ftHashJoin_old.fut:656:3-663:87\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-852:57\n   #4  test_join2.fut:16:1-29:36\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int16_t next_S_f_res_40748 = ((int16_t *) mem_42270)[tmp_40743];
                                
                                next_S_40737 = next_S_f_res_40748;
                            }
                            
                            int32_t prim_get_radix_arg0_40749 = mul32(f64_res_31066, cur_depth_40702);
                            int64_t unsign_arg0_40750 = zext_i32_i64(prim_get_radix_arg0_40749);
                            int64_t asPow_40751 = shl64((int64_t) 1, unsign_arg0_40750);
                            int64_t u64_arg0_40752 = sub64(asPow_40751, (int64_t) 1);
                            int16_t u64_res_40753 = zext_i64_i16(u64_arg0_40752);
                            int16_t lifted_lambda_res_40754 = defunc_0_byteSeq_to_prim_res_40660 & u64_res_40753;
                            int16_t lifted_lambda_res_40755 = cur_S_40724 & u64_res_40753;
                            bool defunc_0_eq_res_40756 = lifted_lambda_res_40754 == lifted_lambda_res_40755;
                            int64_t loopres_40757;
                            int64_t loopres_40758;
                            
                            if (defunc_0_eq_res_40756) {
                                loopres_40757 = heshi_40696;
                                loopres_40758 = (int64_t) 0;
                            } else {
                                bool loop_cond_t_res_40759 = slt32(0, cur_depth_40702);
                                bool is_lt_40760;
                                bool is_lt_40761;
                                int32_t is_lt_40762;
                                bool loop_while_40763;
                                bool def_lt_40764;
                                int32_t bi_40765;
                                
                                loop_while_40763 = loop_cond_t_res_40759;
                                def_lt_40764 = 0;
                                bi_40765 = 0;
                                while (loop_while_40763) {
                                    int32_t zt_rhs_40766 = add32(1, bi_40765);
                                    int32_t prim_get_radix_arg0_40767 = mul32(f64_res_31066, zt_rhs_40766);
                                    int64_t unsign_arg0_40768 = zext_i32_i64(prim_get_radix_arg0_40767);
                                    int64_t asPow_40769 = shl64((int64_t) 1, unsign_arg0_40768);
                                    int64_t u64_arg0_40770 = sub64(asPow_40769, (int64_t) 1);
                                    int16_t u64_res_40771 = zext_i64_i16(u64_arg0_40770);
                                    int16_t lifted_lambda_res_40772 = defunc_0_byteSeq_to_prim_res_40660 & u64_res_40771;
                                    int16_t lifted_lambda_res_40773 = cur_S_40724 & u64_res_40771;
                                    bool defunc_0_lt_res_40774 = ult16(lifted_lambda_res_40772, lifted_lambda_res_40773);
                                    bool defunc_0_lt_res_40775 = ult16(lifted_lambda_res_40773, lifted_lambda_res_40772);
                                    bool cond_40776 = !defunc_0_lt_res_40775;
                                    bool cond_t_res_40777 = !defunc_0_lt_res_40774;
                                    bool x_40778 = cond_40776 && cond_t_res_40777;
                                    bool loop_cond_t_res_40779 = slt32(zt_rhs_40766, cur_depth_40702);
                                    bool x_40780 = x_40778 && loop_cond_t_res_40779;
                                    bool loop_while_tmp_42630 = x_40780;
                                    bool def_lt_tmp_42631 = defunc_0_lt_res_40774;
                                    int32_t bi_tmp_42632 = zt_rhs_40766;
                                    
                                    loop_while_40763 = loop_while_tmp_42630;
                                    def_lt_40764 = def_lt_tmp_42631;
                                    bi_40765 = bi_tmp_42632;
                                }
                                is_lt_40760 = loop_while_40763;
                                is_lt_40761 = def_lt_40764;
                                is_lt_40762 = bi_40765;
                                
                                int64_t loopres_f_res_40781;
                                int64_t loopres_f_res_40782;
                                
                                if (is_lt_40761) {
                                    bool loop_cond_t_res_40783 = slt32(0, prev_depth_40704);
                                    bool is_gt_40784;
                                    bool is_gt_40785;
                                    int32_t is_gt_40786;
                                    bool loop_while_40787;
                                    bool def_gt_40788;
                                    int32_t bi_40789;
                                    
                                    loop_while_40787 = loop_cond_t_res_40783;
                                    def_gt_40788 = 0;
                                    bi_40789 = 0;
                                    while (loop_while_40787) {
                                        int32_t zt_rhs_40790 = add32(1, bi_40789);
                                        int32_t prim_get_radix_arg0_40791 = mul32(f64_res_31066, zt_rhs_40790);
                                        int64_t unsign_arg0_40792 = zext_i32_i64(prim_get_radix_arg0_40791);
                                        int64_t asPow_40793 = shl64((int64_t) 1, unsign_arg0_40792);
                                        int64_t u64_arg0_40794 = sub64(asPow_40793, (int64_t) 1);
                                        int16_t u64_res_40795 = zext_i64_i16(u64_arg0_40794);
                                        int16_t lifted_lambda_res_40796 = defunc_0_byteSeq_to_prim_res_40660 & u64_res_40795;
                                        int16_t lifted_lambda_res_40797 = prev_S_40725 & u64_res_40795;
                                        bool defunc_0_gt_res_40798 = ult16(lifted_lambda_res_40797, lifted_lambda_res_40796);
                                        bool defunc_0_gt_res_40799 = ult16(lifted_lambda_res_40796, lifted_lambda_res_40797);
                                        bool cond_40800 = !defunc_0_gt_res_40798;
                                        bool cond_t_res_40801 = !defunc_0_gt_res_40799;
                                        bool x_40802 = cond_40800 && cond_t_res_40801;
                                        bool loop_cond_t_res_40803 = slt32(zt_rhs_40790, prev_depth_40704);
                                        bool x_40804 = x_40802 && loop_cond_t_res_40803;
                                        bool loop_while_tmp_42633 = x_40804;
                                        bool def_gt_tmp_42634 = defunc_0_gt_res_40798;
                                        int32_t bi_tmp_42635 = zt_rhs_40790;
                                        
                                        loop_while_40787 = loop_while_tmp_42633;
                                        def_gt_40788 = def_gt_tmp_42634;
                                        bi_40789 = bi_tmp_42635;
                                    }
                                    is_gt_40784 = loop_while_40787;
                                    is_gt_40785 = def_gt_40788;
                                    is_gt_40786 = bi_40789;
                                    
                                    bool x_40805 = !cond_40703;
                                    bool y_40806 = is_gt_40785 && x_40805;
                                    bool cond_40807 = cond_40703 || y_40806;
                                    int64_t loopres_f_res_t_res_40808;
                                    int64_t loopres_f_res_t_res_40809;
                                    
                                    if (cond_40807) {
                                        loopres_f_res_t_res_40808 = (int64_t) -1;
                                        loopres_f_res_t_res_40809 = (int64_t) 0;
                                    } else {
                                        int64_t tmp_40810 = sub64(heshi_40696, step_40697);
                                        int64_t max_arg1_40811 = sdiv64(step_40697, (int64_t) 2);
                                        int64_t max_res_40812 = smax64((int64_t) 1, max_arg1_40811);
                                        
                                        loopres_f_res_t_res_40808 = tmp_40810;
                                        loopres_f_res_t_res_40809 = max_res_40812;
                                    }
                                    loopres_f_res_40781 = loopres_f_res_t_res_40808;
                                    loopres_f_res_40782 = loopres_f_res_t_res_40809;
                                } else {
                                    bool loop_cond_t_res_40813 = slt32(0, next_depth_40712);
                                    bool is_lt_40814;
                                    bool is_lt_40815;
                                    int32_t is_lt_40816;
                                    bool loop_while_40817;
                                    bool def_lt_40818;
                                    int32_t bi_40819;
                                    
                                    loop_while_40817 = loop_cond_t_res_40813;
                                    def_lt_40818 = 0;
                                    bi_40819 = 0;
                                    while (loop_while_40817) {
                                        int32_t zt_rhs_40820 = add32(1, bi_40819);
                                        int32_t prim_get_radix_arg0_40821 = mul32(f64_res_31066, zt_rhs_40820);
                                        int64_t unsign_arg0_40822 = zext_i32_i64(prim_get_radix_arg0_40821);
                                        int64_t asPow_40823 = shl64((int64_t) 1, unsign_arg0_40822);
                                        int64_t u64_arg0_40824 = sub64(asPow_40823, (int64_t) 1);
                                        int16_t u64_res_40825 = zext_i64_i16(u64_arg0_40824);
                                        int16_t lifted_lambda_res_40826 = defunc_0_byteSeq_to_prim_res_40660 & u64_res_40825;
                                        int16_t lifted_lambda_res_40827 = next_S_40737 & u64_res_40825;
                                        bool defunc_0_lt_res_40828 = ult16(lifted_lambda_res_40826, lifted_lambda_res_40827);
                                        bool defunc_0_lt_res_40829 = ult16(lifted_lambda_res_40827, lifted_lambda_res_40826);
                                        bool cond_40830 = !defunc_0_lt_res_40829;
                                        bool cond_t_res_40831 = !defunc_0_lt_res_40828;
                                        bool x_40832 = cond_40830 && cond_t_res_40831;
                                        bool loop_cond_t_res_40833 = slt32(zt_rhs_40820, next_depth_40712);
                                        bool x_40834 = x_40832 && loop_cond_t_res_40833;
                                        bool loop_while_tmp_42636 = x_40834;
                                        bool def_lt_tmp_42637 = defunc_0_lt_res_40828;
                                        int32_t bi_tmp_42638 = zt_rhs_40820;
                                        
                                        loop_while_40817 = loop_while_tmp_42636;
                                        def_lt_40818 = def_lt_tmp_42637;
                                        bi_40819 = bi_tmp_42638;
                                    }
                                    is_lt_40814 = loop_while_40817;
                                    is_lt_40815 = def_lt_40818;
                                    is_lt_40816 = bi_40819;
                                    
                                    bool x_40835 = !cond_40711;
                                    bool y_40836 = is_lt_40815 && x_40835;
                                    bool cond_40837 = cond_40711 || y_40836;
                                    int64_t loopres_f_res_f_res_40838;
                                    int64_t loopres_f_res_f_res_40839;
                                    
                                    if (cond_40837) {
                                        loopres_f_res_f_res_40838 = (int64_t) -1;
                                        loopres_f_res_f_res_40839 = (int64_t) 0;
                                    } else {
                                        int64_t tmp_40840 = add64(heshi_40696, step_40697);
                                        int64_t max_arg1_40841 = sdiv64(step_40697, (int64_t) 2);
                                        int64_t max_res_40842 = smax64((int64_t) 1, max_arg1_40841);
                                        
                                        loopres_f_res_f_res_40838 = tmp_40840;
                                        loopres_f_res_f_res_40839 = max_res_40842;
                                    }
                                    loopres_f_res_40781 = loopres_f_res_f_res_40838;
                                    loopres_f_res_40782 = loopres_f_res_f_res_40839;
                                }
                                loopres_40757 = loopres_f_res_40781;
                                loopres_40758 = loopres_f_res_40782;
                            }
                            
                            bool loop_cond_40843 = slt64((int64_t) 0, loopres_40758);
                            bool loop_while_tmp_42627 = loop_cond_40843;
                            int64_t heshi_tmp_42628 = loopres_40757;
                            int64_t step_tmp_42629 = loopres_40758;
                            
                            loop_while_40695 = loop_while_tmp_42627;
                            heshi_40696 = heshi_tmp_42628;
                            step_40697 = step_tmp_42629;
                        }
                        bsearch_40692 = loop_while_40695;
                        bsearch_40693 = heshi_40696;
                        bsearch_40694 = step_40697;
                        defunc_res_f_res_40687 = bsearch_40693;
                    }
                    defunc_res_40684 = defunc_res_f_res_40687;
                }
                
                bool cond_40844 = sle64(defunc_res_40684, (int64_t) 0);
                int64_t inf_40845;
                
                if (cond_40844) {
                    inf_40845 = (int64_t) 0;
                } else {
                    bool x_40846 = sle64((int64_t) 0, defunc_res_40684);
                    bool y_40847 = slt64(defunc_res_40684, dz2083U_25954);
                    bool bounds_check_40848 = x_40846 && y_40847;
                    bool index_certs_40849;
                    
                    if (!bounds_check_40848) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) defunc_res_40684, "] out of bounds for array of shape [", (long long) dz2083U_25954, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:664:41-57\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin_old.fut:848:4-852:57\n   #3  test_join2.fut:16:1-29:36\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t inf_f_res_40850 = ((int64_t *) bounds2_mem_41981.mem)[defunc_res_40684];
                    
                    inf_40845 = inf_f_res_40850;
                }
                
                bool cond_40851 = defunc_res_40684 == zeze_rhs_31081;
                int64_t sup_40852;
                
                if (cond_40851) {
                    sup_40852 = n2_25952;
                } else {
                    int64_t tmp_40853 = add64((int64_t) 1, defunc_res_40684);
                    bool x_40854 = sle64((int64_t) 0, tmp_40853);
                    bool y_40855 = slt64(tmp_40853, dz2083U_25954);
                    bool bounds_check_40856 = x_40854 && y_40855;
                    bool index_certs_40857;
                    
                    if (!bounds_check_40856) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_40853, "] out of bounds for array of shape [", (long long) dz2083U_25954, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:665:47-65\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin_old.fut:848:4-852:57\n   #3  test_join2.fut:16:1-29:36\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t sup_f_res_40858 = ((int64_t *) bounds2_mem_41981.mem)[tmp_40853];
                    
                    sup_40852 = sup_f_res_40858;
                }
                
                bool loop_cond_40859 = slt64(inf_40845, sup_40852);
                bool lifted_lambda_res_40860;
                int64_t lifted_lambda_res_40861;
                int64_t lifted_lambda_res_40862;
                int64_t lifted_lambda_res_40863;
                bool loop_while_40864;
                int64_t j_40865;
                int64_t duome_40866;
                int64_t i_40867;
                
                loop_while_40864 = loop_cond_40859;
                j_40865 = inf_40845;
                duome_40866 = (int64_t) 0;
                i_40867 = inf_40845;
                while (loop_while_40864) {
                    bool x_40868 = sle64((int64_t) 0, i_40867);
                    bool y_40869 = slt64(i_40867, n2_25952);
                    bool bounds_check_40870 = x_40868 && y_40869;
                    bool index_certs_40871;
                    
                    if (!bounds_check_40870) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_40867, "] out of bounds for array of shape [", (long long) n2_25952, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:668:29-38\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin_old.fut:848:4-852:57\n   #3  test_join2.fut:16:1-29:36\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int16_t zeze_rhs_40872 = ((int16_t *) mem_42270)[i_40867];
                    bool is_eq_40873 = defunc_0_byteSeq_to_prim_res_40660 == zeze_rhs_40872;
                    bool cond_40874 = slt64((int64_t) 0, duome_40866);
                    bool cond_40875 = is_eq_40873 || cond_40874;
                    int64_t next_j_40876;
                    
                    if (cond_40875) {
                        next_j_40876 = j_40865;
                    } else {
                        int64_t next_j_f_res_40877 = add64((int64_t) 1, j_40865);
                        
                        next_j_40876 = next_j_f_res_40877;
                    }
                    
                    int64_t next_duome_40878;
                    
                    if (is_eq_40873) {
                        int64_t next_duome_t_res_40879 = add64((int64_t) 1, duome_40866);
                        
                        next_duome_40878 = next_duome_t_res_40879;
                    } else {
                        next_duome_40878 = duome_40866;
                    }
                    
                    int64_t tmp_40880 = add64((int64_t) 1, i_40867);
                    bool loop_cond_40881 = slt64(tmp_40880, sup_40852);
                    bool loop_while_tmp_42639 = loop_cond_40881;
                    int64_t j_tmp_42640 = next_j_40876;
                    int64_t duome_tmp_42641 = next_duome_40878;
                    int64_t i_tmp_42642 = tmp_40880;
                    
                    loop_while_40864 = loop_while_tmp_42639;
                    j_40865 = j_tmp_42640;
                    duome_40866 = duome_tmp_42641;
                    i_40867 = i_tmp_42642;
                }
                lifted_lambda_res_40860 = loop_while_40864;
                lifted_lambda_res_40861 = j_40865;
                lifted_lambda_res_40862 = duome_40866;
                lifted_lambda_res_40863 = i_40867;
                
                bool cond_40882 = slt64(lifted_lambda_res_40861, sup_40852);
                int64_t lifted_lambda_res_40883;
                
                if (cond_40882) {
                    lifted_lambda_res_40883 = lifted_lambda_res_40862;
                } else {
                    lifted_lambda_res_40883 = (int64_t) 0;
                }
                
                int64_t lifted_lambda_res_40884;
                
                if (cond_40882) {
                    lifted_lambda_res_40884 = lifted_lambda_res_40861;
                } else {
                    lifted_lambda_res_40884 = (int64_t) -1;
                }
                
                bool lifted_lambda_res_40885 = slt64((int64_t) 1, lifted_lambda_res_40883);
                int64_t defunc_0_f_res_40886 = btoi_bool_i64(lifted_lambda_res_40885);
                int64_t defunc_0_op_res_40889 = add64(defunc_0_f_res_40886, scanacc_41780);
                int64_t defunc_0_op_res_40892 = add64(lifted_lambda_res_40883, scanacc_41781);
                int64_t zp_res_40895 = add64(lifted_lambda_res_40883, redout_41784);
                
                ((int64_t *) mem_42278)[i_41789] = defunc_0_op_res_40889;
                ((int64_t *) mem_42280)[i_41789] = defunc_0_op_res_40892;
                ((int64_t *) mem_42282)[i_41789] = defunc_0_f_res_40886;
                ((int64_t *) mem_42284)[i_41789] = lifted_lambda_res_40884;
                ((int64_t *) mem_42286)[i_41789] = lifted_lambda_res_40883;
                ((int16_t *) mem_42288)[i_41789] = defunc_0_byteSeq_to_prim_res_40660;
                
                int64_t scanacc_tmp_42617 = defunc_0_op_res_40889;
                int64_t scanacc_tmp_42618 = defunc_0_op_res_40892;
                int64_t redout_tmp_42621 = zp_res_40895;
                
                scanacc_41780 = scanacc_tmp_42617;
                scanacc_41781 = scanacc_tmp_42618;
                redout_41784 = redout_tmp_42621;
            }
            discard_41796 = scanacc_41780;
            discard_41797 = scanacc_41781;
            defunc_res_40654 = redout_41784;
            
            int64_t m_f_res_40931;
            
            if (x_31083) {
                int64_t x_40932 = ((int64_t *) mem_42278)[tmp_31084];
                
                m_f_res_40931 = x_40932;
            } else {
                m_f_res_40931 = (int64_t) 0;
            }
            
            int64_t m_40933;
            
            if (cond_31082) {
                m_40933 = (int64_t) 0;
            } else {
                m_40933 = m_f_res_40931;
            }
            
            bool cond_40934 = slt64((int64_t) 0, m_40933);
            int64_t m_40935 = sub64(m_40933, (int64_t) 1);
            bool i_p_m_t_s_leq_w_40936 = slt64(m_40935, n1_25949);
            bool zzero_leq_i_p_m_t_s_40937 = sle64((int64_t) 0, m_40935);
            bool y_40938 = i_p_m_t_s_leq_w_40936 && zzero_leq_i_p_m_t_s_40937;
            bool i_lte_j_40939 = sle64((int64_t) 0, m_40933);
            bool forwards_ok_40940 = y_40938 && i_lte_j_40939;
            bool eq_x_zz_40941 = (int64_t) 0 == m_f_res_40931;
            bool p_and_eq_x_y_40942 = x_31083 && eq_x_zz_40941;
            bool empty_slice_40943 = cond_31082 || p_and_eq_x_y_40942;
            bool ok_or_empty_40944 = forwards_ok_40940 || empty_slice_40943;
            bool index_certs_40945;
            
            if (!ok_or_empty_40944) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_40933, "] out of bounds for array of shape [", (long long) n1_25949, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin_old.fut:848:4-852:57\n   #3  test_join2.fut:16:1-29:36\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t bytes_42333 = (int64_t) 8 * m_40933;
            
            if (mem_42334_cached_sizze_42785 < bytes_42333) {
                err = lexical_realloc(ctx, &mem_42334, &mem_42334_cached_sizze_42785, bytes_42333);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_42334, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_42286, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_40933});
            
            int64_t max_mult_40948;
            
            if (cond_40934) {
                if (mem_42336_cached_sizze_42786 < bytes_41993) {
                    err = lexical_realloc(ctx, &mem_42336, &mem_42336_cached_sizze_42786, bytes_41993);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                for (int64_t i_41804 = 0; i_41804 < n1_25949; i_41804++) {
                    int64_t eta_p_40950 = ((int64_t *) mem_42282)[i_41804];
                    int64_t eta_p_40951 = ((int64_t *) mem_42278)[i_41804];
                    bool cond_40952 = eta_p_40950 == (int64_t) 1;
                    int64_t lifted_lambda_res_40953;
                    
                    if (cond_40952) {
                        int64_t lifted_lambda_res_t_res_40954 = sub64(eta_p_40951, (int64_t) 1);
                        
                        lifted_lambda_res_40953 = lifted_lambda_res_t_res_40954;
                    } else {
                        lifted_lambda_res_40953 = (int64_t) -1;
                    }
                    ((int64_t *) mem_42336)[i_41804] = lifted_lambda_res_40953;
                }
                for (int64_t write_iter_41806 = 0; write_iter_41806 < n1_25949; write_iter_41806++) {
                    int64_t write_iv_41808 = ((int64_t *) mem_42336)[write_iter_41806];
                    int64_t write_iv_41809 = ((int64_t *) mem_42286)[write_iter_41806];
                    
                    if (sle64((int64_t) 0, write_iv_41808) && slt64(write_iv_41808, m_40933)) {
                        ((int64_t *) mem_42334)[write_iv_41808] = write_iv_41809;
                    }
                }
                
                int64_t defunc_0_reduce_res_40968;
                int64_t redout_41811 = (int64_t) -9223372036854775808;
                
                for (int64_t i_41812 = 0; i_41812 < m_40933; i_41812++) {
                    int64_t x_40969 = ((int64_t *) mem_42334)[i_41812];
                    int64_t max_res_40972 = smax64(x_40969, redout_41811);
                    int64_t redout_tmp_42645 = max_res_40972;
                    
                    redout_41811 = redout_tmp_42645;
                }
                defunc_0_reduce_res_40968 = redout_41811;
                max_mult_40948 = defunc_0_reduce_res_40968;
            } else {
                max_mult_40948 = (int64_t) 1;
            }
            
            int64_t bytes_42349 = (int64_t) 8 * defunc_res_40654;
            bool bounds_invalid_upwards_41004 = slt64(max_mult_40948, (int64_t) 1);
            bool valid_41006 = !bounds_invalid_upwards_41004;
            bool range_valid_c_41007;
            
            if (!valid_41006) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 1, "..<", (long long) max_mult_40948, " is invalid.", "-> #0  ../../../joins/ftHashJoin_old.fut:691:18-30\n   #1  ../../../joins/ftHashJoin_old.fut:848:4-852:57\n   #2  test_join2.fut:16:1-29:36\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t bytes_42387 = b_25950 * defunc_res_40654;
            
            if (mem_42326_cached_sizze_42784 < bytes_41993) {
                err = lexical_realloc(ctx, &mem_42326, &mem_42326_cached_sizze_42784, bytes_41993);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            for (int64_t i_41800 = 0; i_41800 < n1_25949; i_41800++) {
                int64_t eta_p_40920 = ((int64_t *) mem_42286)[i_41800];
                int64_t zv_lhs_40921 = add64((int64_t) -1, i_41800);
                int64_t tmp_40922 = smod64(zv_lhs_40921, n1_25949);
                int64_t lifted_lambda_res_40923 = ((int64_t *) mem_42280)[tmp_40922];
                bool cond_40924 = i_41800 == (int64_t) 0;
                int64_t lifted_lambda_res_40925;
                
                if (cond_40924) {
                    lifted_lambda_res_40925 = (int64_t) 0;
                } else {
                    lifted_lambda_res_40925 = lifted_lambda_res_40923;
                }
                
                bool cond_40926 = slt64((int64_t) 0, eta_p_40920);
                int64_t lifted_lambda_res_40927;
                
                if (cond_40926) {
                    lifted_lambda_res_40927 = lifted_lambda_res_40925;
                } else {
                    lifted_lambda_res_40927 = (int64_t) -1;
                }
                ((int64_t *) mem_42326)[i_41800] = lifted_lambda_res_40927;
            }
            if (memblock_alloc(ctx, &mem_42350, bytes_42349, "mem_42350")) {
                err = 1;
                goto cleanup;
            }
            for (int64_t nest_i_42647 = 0; nest_i_42647 < defunc_res_40654; nest_i_42647++) {
                ((int64_t *) mem_42350.mem)[nest_i_42647] = (int64_t) 0;
            }
            for (int64_t write_iter_41813 = 0; write_iter_41813 < n1_25949; write_iter_41813++) {
                int64_t write_iv_41815 = ((int64_t *) mem_42326)[write_iter_41813];
                
                if (sle64((int64_t) 0, write_iv_41815) && slt64(write_iv_41815, defunc_res_40654)) {
                    ((int64_t *) mem_42350.mem)[write_iv_41815] = write_iter_41813;
                }
            }
            if (memblock_alloc(ctx, &mem_42358, bytes_42349, "mem_42358")) {
                err = 1;
                goto cleanup;
            }
            for (int64_t nest_i_42649 = 0; nest_i_42649 < defunc_res_40654; nest_i_42649++) {
                ((int64_t *) mem_42358.mem)[nest_i_42649] = (int64_t) 1;
            }
            
            int64_t distance_41005 = sub64(max_mult_40948, (int64_t) 1);
            
            if (memblock_set(ctx, &mem_param_42361, &mem_42358, "mem_42358") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_42364, &mem_42350, "mem_42350") != 0)
                return 1;
            for (int64_t i_41010 = 0; i_41010 < distance_41005; i_41010++) {
                int64_t index_primexp_41013 = add64((int64_t) 1, i_41010);
                
                if (memblock_alloc(ctx, &mem_42366, bytes_42349, "mem_42366")) {
                    err = 1;
                    goto cleanup;
                }
                lmad_copy_8b(ctx, 1, (uint64_t *) mem_42366.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_42364.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {defunc_res_40654});
                if (memblock_alloc(ctx, &mem_42368, bytes_42349, "mem_42368")) {
                    err = 1;
                    goto cleanup;
                }
                lmad_copy_8b(ctx, 1, (uint64_t *) mem_42368.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_42361.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {defunc_res_40654});
                
                int64_t replicate_arg1_41016 = add64((int64_t) 2, i_41010);
                
                for (int64_t write_iter_41818 = 0; write_iter_41818 < n1_25949; write_iter_41818++) {
                    int64_t zlze_lhs_41024 = ((int64_t *) mem_42286)[write_iter_41818];
                    bool cond_41025 = sle64(zlze_lhs_41024, index_primexp_41013);
                    int64_t lifted_lambda_res_41026;
                    
                    if (cond_41025) {
                        lifted_lambda_res_41026 = (int64_t) -1;
                    } else {
                        int64_t zp_lhs_41027 = ((int64_t *) mem_42326)[write_iter_41818];
                        int64_t lifted_lambda_res_f_res_41028 = add64(index_primexp_41013, zp_lhs_41027);
                        
                        lifted_lambda_res_41026 = lifted_lambda_res_f_res_41028;
                    }
                    if (sle64((int64_t) 0, lifted_lambda_res_41026) && slt64(lifted_lambda_res_41026, defunc_res_40654)) {
                        ((int64_t *) mem_42368.mem)[lifted_lambda_res_41026] = replicate_arg1_41016;
                    }
                    if (sle64((int64_t) 0, lifted_lambda_res_41026) && slt64(lifted_lambda_res_41026, defunc_res_40654)) {
                        ((int64_t *) mem_42366.mem)[lifted_lambda_res_41026] = write_iter_41818;
                    }
                }
                if (memblock_set(ctx, &mem_param_tmp_42650, &mem_42368, "mem_42368") != 0)
                    return 1;
                if (memblock_set(ctx, &mem_param_tmp_42651, &mem_42366, "mem_42366") != 0)
                    return 1;
                if (memblock_set(ctx, &mem_param_42361, &mem_param_tmp_42650, "mem_param_tmp_42650") != 0)
                    return 1;
                if (memblock_set(ctx, &mem_param_42364, &mem_param_tmp_42651, "mem_param_tmp_42651") != 0)
                    return 1;
            }
            if (memblock_set(ctx, &ext_mem_42386, &mem_param_42361, "mem_param_42361") != 0)
                return 1;
            if (memblock_set(ctx, &ext_mem_42385, &mem_param_42364, "mem_param_42364") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_42350, "mem_42350") != 0)
                return 1;
            if (memblock_unref(ctx, &mem_42358, "mem_42358") != 0)
                return 1;
            if (memblock_alloc(ctx, &mem_42388, bytes_42387, "mem_42388")) {
                err = 1;
                goto cleanup;
            }
            if (memblock_alloc(ctx, &mem_42390, bytes_42349, "mem_42390")) {
                err = 1;
                goto cleanup;
            }
            for (int64_t i_41828 = 0; i_41828 < defunc_res_40654; i_41828++) {
                int64_t eta_p_41040 = ((int64_t *) ext_mem_42385.mem)[i_41828];
                bool x_41041 = sle64((int64_t) 0, eta_p_41040);
                bool y_41042 = slt64(eta_p_41040, n1_25949);
                bool bounds_check_41043 = x_41041 && y_41042;
                bool index_certs_41044;
                
                if (!bounds_check_41043) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_41040, "] out of bounds for array of shape [", (long long) n1_25949, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:701:16-26\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin_old.fut:700:8-708:6\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-852:57\n   #4  test_join2.fut:16:1-29:36\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                int64_t eta_p_41039 = ((int64_t *) ext_mem_42386.mem)[i_41828];
                int16_t rv_41045 = ((int16_t *) mem_42288)[eta_p_41040];
                int64_t inf_s_idx_41046 = ((int64_t *) mem_42284)[eta_p_41040];
                int64_t tmp_41047 = sub64(inf_s_idx_41046, (int64_t) 1);
                bool loop_cond_41048 = slt64((int64_t) 0, eta_p_41039);
                bool kth_match_41049;
                int64_t kth_match_41050;
                int64_t kth_match_41051;
                bool loop_while_41052;
                int64_t j_41053;
                int64_t m_41054;
                
                loop_while_41052 = loop_cond_41048;
                j_41053 = tmp_41047;
                m_41054 = (int64_t) 0;
                while (loop_while_41052) {
                    int64_t zeze_rhs_41055 = add64((int64_t) 1, j_41053);
                    bool x_41056 = sle64((int64_t) 0, zeze_rhs_41055);
                    bool y_41057 = slt64(zeze_rhs_41055, n2_25952);
                    bool bounds_check_41058 = x_41056 && y_41057;
                    bool index_certs_41059;
                    
                    if (!bounds_check_41058) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) zeze_rhs_41055, "] out of bounds for array of shape [", (long long) n2_25952, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:705:29-40\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin_old.fut:700:8-708:6\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-852:57\n   #4  test_join2.fut:16:1-29:36\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int16_t zeze_rhs_41060 = ((int16_t *) mem_42270)[zeze_rhs_41055];
                    bool is_eq_41061 = rv_41045 == zeze_rhs_41060;
                    int64_t loopres_41062;
                    
                    if (is_eq_41061) {
                        int64_t tmp_41063 = add64((int64_t) 1, m_41054);
                        
                        loopres_41062 = tmp_41063;
                    } else {
                        loopres_41062 = m_41054;
                    }
                    
                    bool loop_cond_41064 = slt64(loopres_41062, eta_p_41039);
                    bool loop_while_tmp_42658 = loop_cond_41064;
                    int64_t j_tmp_42659 = zeze_rhs_41055;
                    int64_t m_tmp_42660 = loopres_41062;
                    
                    loop_while_41052 = loop_while_tmp_42658;
                    j_41053 = j_tmp_42659;
                    m_41054 = m_tmp_42660;
                }
                kth_match_41049 = loop_while_41052;
                kth_match_41050 = j_41053;
                kth_match_41051 = m_41054;
                lmad_copy_1b(ctx, 1, (uint8_t *) mem_42388.mem, i_41828 * b_25950, (int64_t []) {(int64_t) 1}, (uint8_t *) ks1_mem_41977.mem, eta_p_41040 * b_25950, (int64_t []) {(int64_t) 1}, (int64_t []) {b_25950});
                ((int64_t *) mem_42390.mem)[i_41828] = kth_match_41050;
            }
            if (memblock_unref(ctx, &ext_mem_42386, "ext_mem_42386") != 0)
                return 1;
            if (memblock_alloc(ctx, &mem_42406, bytes_42349, "mem_42406")) {
                err = 1;
                goto cleanup;
            }
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_42406.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_42385.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {defunc_res_40654});
            if (memblock_unref(ctx, &ext_mem_42385, "ext_mem_42385") != 0)
                return 1;
            if (memblock_set(ctx, &ext_mem_42414, &mem_42406, "mem_42406") != 0)
                return 1;
            if (memblock_set(ctx, &ext_mem_42413, &mem_42390, "mem_42390") != 0)
                return 1;
            if (memblock_set(ctx, &ext_mem_42410, &mem_42388, "mem_42388") != 0)
                return 1;
            radix_hash_join_res_f_res_31468 = defunc_res_40654;
            radix_hash_join_res_f_res_31469 = defunc_res_40654;
            radix_hash_join_res_f_res_31470 = defunc_res_40654;
        } else {
            int64_t radix_hash_join_res_f_res_f_res_31827;
            
            if (cond_31077) {
                if (mem_42124_cached_sizze_42767 < bytes_42123) {
                    err = lexical_realloc(ctx, &mem_42124, &mem_42124_cached_sizze_42767, bytes_42123);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                for (int64_t i_41833 = 0; i_41833 < n2_25952; i_41833++) {
                    int32_t defunc_0_byteSeq_to_prim_res_41164;
                    int32_t y_41166 = 0;
                    
                    for (int64_t i_41165 = 0; i_41165 < b_25950; i_41165++) {
                        int64_t zm_lhs_41167 = sub64(b_25950, i_41165);
                        int64_t from_u8_arg0_41168 = sub64(zm_lhs_41167, (int64_t) 1);
                        bool x_41169 = sle64((int64_t) 0, from_u8_arg0_41168);
                        bool y_41170 = slt64(from_u8_arg0_41168, b_25950);
                        bool bounds_check_41171 = x_41169 && y_41170;
                        bool index_certs_41172;
                        
                        if (!bounds_check_41171) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) from_u8_arg0_41168, "] out of bounds for array of shape [", (long long) b_25950, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:417:22-30\n   #1  ../../../joins/ftHashJoin_old.fut:425:75-82\n   #2  test_join2.fut:16:1-29:36\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int8_t from_u8_arg0_41173 = ((int8_t *) ks2_mem_41979.mem)[i_41833 * b_25950 + from_u8_arg0_41168];
                        int32_t u8_res_41174 = zext_i8_i32(from_u8_arg0_41173);
                        int32_t i64_res_41175 = sext_i64_i32(i_41165);
                        int32_t shift_left_rhs_41176 = mul32(8, i64_res_41175);
                        int32_t lifted_lambda_res_41177 = shl32(u8_res_41174, shift_left_rhs_41176);
                        int32_t unsign_arg0_41178 = y_41166 | lifted_lambda_res_41177;
                        int32_t y_tmp_42662 = unsign_arg0_41178;
                        
                        y_41166 = y_tmp_42662;
                    }
                    defunc_0_byteSeq_to_prim_res_41164 = y_41166;
                    ((int32_t *) mem_42124)[i_41833] = defunc_0_byteSeq_to_prim_res_41164;
                }
                if (mem_42132_cached_sizze_42768 < bytes_41993) {
                    err = lexical_realloc(ctx, &mem_42132, &mem_42132_cached_sizze_42768, bytes_41993);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                if (mem_42134_cached_sizze_42769 < bytes_41993) {
                    err = lexical_realloc(ctx, &mem_42134, &mem_42134_cached_sizze_42769, bytes_41993);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                if (mem_42136_cached_sizze_42770 < bytes_41993) {
                    err = lexical_realloc(ctx, &mem_42136, &mem_42136_cached_sizze_42770, bytes_41993);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                if (mem_42138_cached_sizze_42771 < bytes_41993) {
                    err = lexical_realloc(ctx, &mem_42138, &mem_42138_cached_sizze_42771, bytes_41993);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                if (mem_42140_cached_sizze_42772 < bytes_41993) {
                    err = lexical_realloc(ctx, &mem_42140, &mem_42140_cached_sizze_42772, bytes_41993);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                if (mem_42142_cached_sizze_42773 < bytes_42141) {
                    err = lexical_realloc(ctx, &mem_42142, &mem_42142_cached_sizze_42773, bytes_42141);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                
                int64_t discard_41857;
                int64_t discard_41858;
                int64_t defunc_res_41185;
                int64_t scanacc_41841;
                int64_t scanacc_41842;
                int64_t redout_41845;
                
                scanacc_41841 = (int64_t) 0;
                scanacc_41842 = (int64_t) 0;
                redout_41845 = (int64_t) 0;
                for (int64_t i_41850 = 0; i_41850 < n1_25949; i_41850++) {
                    int32_t defunc_0_byteSeq_to_prim_res_41191;
                    int32_t y_41193 = 0;
                    
                    for (int64_t i_41192 = 0; i_41192 < b_25950; i_41192++) {
                        int64_t zm_lhs_41194 = sub64(b_25950, i_41192);
                        int64_t from_u8_arg0_41195 = sub64(zm_lhs_41194, (int64_t) 1);
                        bool x_41196 = sle64((int64_t) 0, from_u8_arg0_41195);
                        bool y_41197 = slt64(from_u8_arg0_41195, b_25950);
                        bool bounds_check_41198 = x_41196 && y_41197;
                        bool index_certs_41199;
                        
                        if (!bounds_check_41198) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) from_u8_arg0_41195, "] out of bounds for array of shape [", (long long) b_25950, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:417:22-30\n   #1  ../../../joins/ftHashJoin_old.fut:425:75-82\n   #2  test_join2.fut:16:1-29:36\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int8_t from_u8_arg0_41200 = ((int8_t *) ks1_mem_41977.mem)[i_41850 * b_25950 + from_u8_arg0_41195];
                        int32_t u8_res_41201 = zext_i8_i32(from_u8_arg0_41200);
                        int32_t i64_res_41202 = sext_i64_i32(i_41192);
                        int32_t shift_left_rhs_41203 = mul32(8, i64_res_41202);
                        int32_t lifted_lambda_res_41204 = shl32(u8_res_41201, shift_left_rhs_41203);
                        int32_t unsign_arg0_41205 = y_41193 | lifted_lambda_res_41204;
                        int32_t y_tmp_42672 = unsign_arg0_41205;
                        
                        y_41193 = y_tmp_42672;
                    }
                    defunc_0_byteSeq_to_prim_res_41191 = y_41193;
                    
                    int32_t lifted_lambda_res_41206 = za_rhs_31094 & defunc_0_byteSeq_to_prim_res_41191;
                    int64_t u32_res_41207 = zext_i32_i64(lifted_lambda_res_41206);
                    bool x_41208 = sle64((int64_t) 0, u32_res_41207);
                    bool y_41209 = slt64(u32_res_41207, dzlz7bUZLztztZRz20U2z20UZLi32z20Uradix_sizzeZRz7dUzg_31070);
                    bool bounds_check_41210 = x_41208 && y_41209;
                    bool index_certs_41211;
                    
                    if (!bounds_check_41210) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) u32_res_41207, "] out of bounds for array of shape [", (long long) dzlz7bUZLztztZRz20U2z20UZLi32z20Uradix_sizzeZRz7dUzg_31070, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:530:13-35\n   #1  ../../../joins/ftHashJoin_old.fut:720:3-727:87\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-853:57\n   #4  test_join2.fut:16:1-29:36\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t spi_41212 = ((int64_t *) first_idx2_mem_41983.mem)[u32_res_41207];
                    bool cond_41213 = slt64(spi_41212, (int64_t) 0);
                    int64_t defunc_res_41214;
                    
                    if (cond_41213) {
                        defunc_res_41214 = (int64_t) -1;
                    } else {
                        int64_t zeze_rhs_41215 = ((int64_t *) last_idx2_mem_41984.mem)[u32_res_41207];
                        bool cond_41216 = spi_41212 == zeze_rhs_41215;
                        int64_t defunc_res_f_res_41217;
                        
                        if (cond_41216) {
                            defunc_res_f_res_41217 = spi_41212;
                        } else {
                            int64_t zs_lhs_41218 = sub64(zeze_rhs_41215, spi_41212);
                            int64_t max_arg1_41219 = sdiv64(zs_lhs_41218, (int64_t) 2);
                            int64_t max_res_41220 = smax64((int64_t) 1, max_arg1_41219);
                            bool loop_cond_41221 = slt64((int64_t) 0, max_res_41220);
                            bool bsearch_41222;
                            int64_t bsearch_41223;
                            int64_t bsearch_41224;
                            bool loop_while_41225;
                            int64_t heshi_41226;
                            int64_t step_41227;
                            
                            loop_while_41225 = loop_cond_41221;
                            heshi_41226 = spi_41212;
                            step_41227 = max_res_41220;
                            while (loop_while_41225) {
                                bool x_41228 = sle64((int64_t) 0, heshi_41226);
                                bool y_41229 = slt64(heshi_41226, dz2083U_25954);
                                bool bounds_check_41230 = x_41228 && y_41229;
                                bool index_certs_41231;
                                
                                if (!bounds_check_41230) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) heshi_41226, "] out of bounds for array of shape [", (long long) dz2083U_25954, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:539:25-40\n   #1  ../../../joins/ftHashJoin_old.fut:720:3-727:87\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-853:57\n   #4  test_join2.fut:16:1-29:36\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t cur_S_41249 = ((int64_t *) bounds2_mem_41981.mem)[heshi_41226];
                                bool x_41250 = sle64((int64_t) 0, cur_S_41249);
                                bool y_41251 = slt64(cur_S_41249, n2_25952);
                                bool bounds_check_41252 = x_41250 && y_41251;
                                bool index_certs_41253;
                                
                                if (!bounds_check_41252) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) cur_S_41249, "] out of bounds for array of shape [", (long long) n2_25952, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:542:21-40\n   #1  ../../../joins/ftHashJoin_old.fut:720:3-727:87\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-853:57\n   #4  test_join2.fut:16:1-29:36\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int32_t cur_depth_41232 = ((int32_t *) depths2_mem_41982.mem)[heshi_41226];
                                bool cond_41233 = heshi_41226 == spi_41212;
                                int32_t prev_depth_41234;
                                
                                if (cond_41233) {
                                    prev_depth_41234 = cur_depth_41232;
                                } else {
                                    int64_t tmp_41235 = sub64(heshi_41226, (int64_t) 1);
                                    bool x_41236 = sle64((int64_t) 0, tmp_41235);
                                    bool y_41237 = slt64(tmp_41235, dz2083U_25954);
                                    bool bounds_check_41238 = x_41236 && y_41237;
                                    bool index_certs_41239;
                                    
                                    if (!bounds_check_41238) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_41235, "] out of bounds for array of shape [", (long long) dz2083U_25954, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:540:60-77\n   #1  ../../../joins/ftHashJoin_old.fut:720:3-727:87\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-853:57\n   #4  test_join2.fut:16:1-29:36\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    int32_t prev_depth_f_res_41240 = ((int32_t *) depths2_mem_41982.mem)[tmp_41235];
                                    
                                    prev_depth_41234 = prev_depth_f_res_41240;
                                }
                                
                                bool cond_41241 = heshi_41226 == zeze_rhs_41215;
                                int32_t next_depth_41242;
                                
                                if (cond_41241) {
                                    next_depth_41242 = cur_depth_41232;
                                } else {
                                    int64_t tmp_41243 = add64((int64_t) 1, heshi_41226);
                                    bool x_41244 = sle64((int64_t) 0, tmp_41243);
                                    bool y_41245 = slt64(tmp_41243, dz2083U_25954);
                                    bool bounds_check_41246 = x_41244 && y_41245;
                                    bool index_certs_41247;
                                    
                                    if (!bounds_check_41246) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_41243, "] out of bounds for array of shape [", (long long) dz2083U_25954, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:541:64-81\n   #1  ../../../joins/ftHashJoin_old.fut:720:3-727:87\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-853:57\n   #4  test_join2.fut:16:1-29:36\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    int32_t next_depth_f_res_41248 = ((int32_t *) depths2_mem_41982.mem)[tmp_41243];
                                    
                                    next_depth_41242 = next_depth_f_res_41248;
                                }
                                
                                int32_t cur_S_41254 = ((int32_t *) mem_42124)[cur_S_41249];
                                int32_t prev_S_41255;
                                
                                if (cond_41233) {
                                    prev_S_41255 = cur_S_41254;
                                } else {
                                    int64_t tmp_41256 = sub64(heshi_41226, (int64_t) 1);
                                    bool x_41257 = sle64((int64_t) 0, tmp_41256);
                                    bool y_41258 = slt64(tmp_41256, dz2083U_25954);
                                    bool bounds_check_41259 = x_41257 && y_41258;
                                    bool index_certs_41260;
                                    
                                    if (!bounds_check_41259) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_41256, "] out of bounds for array of shape [", (long long) dz2083U_25954, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:543:55-72\n   #1  ../../../joins/ftHashJoin_old.fut:720:3-727:87\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-853:57\n   #4  test_join2.fut:16:1-29:36\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    int64_t tmp_41261 = ((int64_t *) bounds2_mem_41981.mem)[tmp_41256];
                                    bool x_41262 = sle64((int64_t) 0, tmp_41261);
                                    bool y_41263 = slt64(tmp_41261, n2_25952);
                                    bool bounds_check_41264 = x_41262 && y_41263;
                                    bool index_certs_41265;
                                    
                                    if (!bounds_check_41264) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_41261, "] out of bounds for array of shape [", (long long) n2_25952, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:543:52-73\n   #1  ../../../joins/ftHashJoin_old.fut:720:3-727:87\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-853:57\n   #4  test_join2.fut:16:1-29:36\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    int32_t prev_S_f_res_41266 = ((int32_t *) mem_42124)[tmp_41261];
                                    
                                    prev_S_41255 = prev_S_f_res_41266;
                                }
                                
                                int32_t next_S_41267;
                                
                                if (cond_41241) {
                                    next_S_41267 = cur_S_41254;
                                } else {
                                    int64_t tmp_41268 = add64((int64_t) 1, heshi_41226);
                                    bool x_41269 = sle64((int64_t) 0, tmp_41268);
                                    bool y_41270 = slt64(tmp_41268, dz2083U_25954);
                                    bool bounds_check_41271 = x_41269 && y_41270;
                                    bool index_certs_41272;
                                    
                                    if (!bounds_check_41271) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_41268, "] out of bounds for array of shape [", (long long) dz2083U_25954, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:544:59-76\n   #1  ../../../joins/ftHashJoin_old.fut:720:3-727:87\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-853:57\n   #4  test_join2.fut:16:1-29:36\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    int64_t tmp_41273 = ((int64_t *) bounds2_mem_41981.mem)[tmp_41268];
                                    bool x_41274 = sle64((int64_t) 0, tmp_41273);
                                    bool y_41275 = slt64(tmp_41273, n2_25952);
                                    bool bounds_check_41276 = x_41274 && y_41275;
                                    bool index_certs_41277;
                                    
                                    if (!bounds_check_41276) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_41273, "] out of bounds for array of shape [", (long long) n2_25952, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:544:56-77\n   #1  ../../../joins/ftHashJoin_old.fut:720:3-727:87\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-853:57\n   #4  test_join2.fut:16:1-29:36\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    int32_t next_S_f_res_41278 = ((int32_t *) mem_42124)[tmp_41273];
                                    
                                    next_S_41267 = next_S_f_res_41278;
                                }
                                
                                int32_t prim_get_radix_arg0_41279 = mul32(f64_res_31066, cur_depth_41232);
                                int64_t unsign_arg0_41280 = zext_i32_i64(prim_get_radix_arg0_41279);
                                int64_t asPow_41281 = shl64((int64_t) 1, unsign_arg0_41280);
                                int64_t u64_arg0_41282 = sub64(asPow_41281, (int64_t) 1);
                                int32_t u64_res_41283 = zext_i64_i32(u64_arg0_41282);
                                int32_t lifted_lambda_res_41284 = defunc_0_byteSeq_to_prim_res_41191 & u64_res_41283;
                                int32_t lifted_lambda_res_41285 = cur_S_41254 & u64_res_41283;
                                bool defunc_0_eq_res_41286 = lifted_lambda_res_41284 == lifted_lambda_res_41285;
                                int64_t loopres_41287;
                                int64_t loopres_41288;
                                
                                if (defunc_0_eq_res_41286) {
                                    loopres_41287 = heshi_41226;
                                    loopres_41288 = (int64_t) 0;
                                } else {
                                    bool loop_cond_t_res_41289 = slt32(0, cur_depth_41232);
                                    bool is_lt_41290;
                                    bool is_lt_41291;
                                    int32_t is_lt_41292;
                                    bool loop_while_41293;
                                    bool def_lt_41294;
                                    int32_t bi_41295;
                                    
                                    loop_while_41293 = loop_cond_t_res_41289;
                                    def_lt_41294 = 0;
                                    bi_41295 = 0;
                                    while (loop_while_41293) {
                                        int32_t zt_rhs_41296 = add32(1, bi_41295);
                                        int32_t prim_get_radix_arg0_41297 = mul32(f64_res_31066, zt_rhs_41296);
                                        int64_t unsign_arg0_41298 = zext_i32_i64(prim_get_radix_arg0_41297);
                                        int64_t asPow_41299 = shl64((int64_t) 1, unsign_arg0_41298);
                                        int64_t u64_arg0_41300 = sub64(asPow_41299, (int64_t) 1);
                                        int32_t u64_res_41301 = zext_i64_i32(u64_arg0_41300);
                                        int32_t lifted_lambda_res_41302 = defunc_0_byteSeq_to_prim_res_41191 & u64_res_41301;
                                        int32_t lifted_lambda_res_41303 = cur_S_41254 & u64_res_41301;
                                        bool defunc_0_lt_res_41304 = ult32(lifted_lambda_res_41302, lifted_lambda_res_41303);
                                        bool defunc_0_lt_res_41305 = ult32(lifted_lambda_res_41303, lifted_lambda_res_41302);
                                        bool cond_41306 = !defunc_0_lt_res_41305;
                                        bool cond_t_res_41307 = !defunc_0_lt_res_41304;
                                        bool x_41308 = cond_41306 && cond_t_res_41307;
                                        bool loop_cond_t_res_41309 = slt32(zt_rhs_41296, cur_depth_41232);
                                        bool x_41310 = x_41308 && loop_cond_t_res_41309;
                                        bool loop_while_tmp_42676 = x_41310;
                                        bool def_lt_tmp_42677 = defunc_0_lt_res_41304;
                                        int32_t bi_tmp_42678 = zt_rhs_41296;
                                        
                                        loop_while_41293 = loop_while_tmp_42676;
                                        def_lt_41294 = def_lt_tmp_42677;
                                        bi_41295 = bi_tmp_42678;
                                    }
                                    is_lt_41290 = loop_while_41293;
                                    is_lt_41291 = def_lt_41294;
                                    is_lt_41292 = bi_41295;
                                    
                                    int64_t loopres_f_res_41311;
                                    int64_t loopres_f_res_41312;
                                    
                                    if (is_lt_41291) {
                                        bool loop_cond_t_res_41313 = slt32(0, prev_depth_41234);
                                        bool is_gt_41314;
                                        bool is_gt_41315;
                                        int32_t is_gt_41316;
                                        bool loop_while_41317;
                                        bool def_gt_41318;
                                        int32_t bi_41319;
                                        
                                        loop_while_41317 = loop_cond_t_res_41313;
                                        def_gt_41318 = 0;
                                        bi_41319 = 0;
                                        while (loop_while_41317) {
                                            int32_t zt_rhs_41320 = add32(1, bi_41319);
                                            int32_t prim_get_radix_arg0_41321 = mul32(f64_res_31066, zt_rhs_41320);
                                            int64_t unsign_arg0_41322 = zext_i32_i64(prim_get_radix_arg0_41321);
                                            int64_t asPow_41323 = shl64((int64_t) 1, unsign_arg0_41322);
                                            int64_t u64_arg0_41324 = sub64(asPow_41323, (int64_t) 1);
                                            int32_t u64_res_41325 = zext_i64_i32(u64_arg0_41324);
                                            int32_t lifted_lambda_res_41326 = defunc_0_byteSeq_to_prim_res_41191 & u64_res_41325;
                                            int32_t lifted_lambda_res_41327 = prev_S_41255 & u64_res_41325;
                                            bool defunc_0_gt_res_41328 = ult32(lifted_lambda_res_41327, lifted_lambda_res_41326);
                                            bool defunc_0_gt_res_41329 = ult32(lifted_lambda_res_41326, lifted_lambda_res_41327);
                                            bool cond_41330 = !defunc_0_gt_res_41328;
                                            bool cond_t_res_41331 = !defunc_0_gt_res_41329;
                                            bool x_41332 = cond_41330 && cond_t_res_41331;
                                            bool loop_cond_t_res_41333 = slt32(zt_rhs_41320, prev_depth_41234);
                                            bool x_41334 = x_41332 && loop_cond_t_res_41333;
                                            bool loop_while_tmp_42679 = x_41334;
                                            bool def_gt_tmp_42680 = defunc_0_gt_res_41328;
                                            int32_t bi_tmp_42681 = zt_rhs_41320;
                                            
                                            loop_while_41317 = loop_while_tmp_42679;
                                            def_gt_41318 = def_gt_tmp_42680;
                                            bi_41319 = bi_tmp_42681;
                                        }
                                        is_gt_41314 = loop_while_41317;
                                        is_gt_41315 = def_gt_41318;
                                        is_gt_41316 = bi_41319;
                                        
                                        bool x_41335 = !cond_41233;
                                        bool y_41336 = is_gt_41315 && x_41335;
                                        bool cond_41337 = cond_41233 || y_41336;
                                        int64_t loopres_f_res_t_res_41338;
                                        int64_t loopres_f_res_t_res_41339;
                                        
                                        if (cond_41337) {
                                            loopres_f_res_t_res_41338 = (int64_t) -1;
                                            loopres_f_res_t_res_41339 = (int64_t) 0;
                                        } else {
                                            int64_t tmp_41340 = sub64(heshi_41226, step_41227);
                                            int64_t max_arg1_41341 = sdiv64(step_41227, (int64_t) 2);
                                            int64_t max_res_41342 = smax64((int64_t) 1, max_arg1_41341);
                                            
                                            loopres_f_res_t_res_41338 = tmp_41340;
                                            loopres_f_res_t_res_41339 = max_res_41342;
                                        }
                                        loopres_f_res_41311 = loopres_f_res_t_res_41338;
                                        loopres_f_res_41312 = loopres_f_res_t_res_41339;
                                    } else {
                                        bool loop_cond_t_res_41343 = slt32(0, next_depth_41242);
                                        bool is_lt_41344;
                                        bool is_lt_41345;
                                        int32_t is_lt_41346;
                                        bool loop_while_41347;
                                        bool def_lt_41348;
                                        int32_t bi_41349;
                                        
                                        loop_while_41347 = loop_cond_t_res_41343;
                                        def_lt_41348 = 0;
                                        bi_41349 = 0;
                                        while (loop_while_41347) {
                                            int32_t zt_rhs_41350 = add32(1, bi_41349);
                                            int32_t prim_get_radix_arg0_41351 = mul32(f64_res_31066, zt_rhs_41350);
                                            int64_t unsign_arg0_41352 = zext_i32_i64(prim_get_radix_arg0_41351);
                                            int64_t asPow_41353 = shl64((int64_t) 1, unsign_arg0_41352);
                                            int64_t u64_arg0_41354 = sub64(asPow_41353, (int64_t) 1);
                                            int32_t u64_res_41355 = zext_i64_i32(u64_arg0_41354);
                                            int32_t lifted_lambda_res_41356 = defunc_0_byteSeq_to_prim_res_41191 & u64_res_41355;
                                            int32_t lifted_lambda_res_41357 = next_S_41267 & u64_res_41355;
                                            bool defunc_0_lt_res_41358 = ult32(lifted_lambda_res_41356, lifted_lambda_res_41357);
                                            bool defunc_0_lt_res_41359 = ult32(lifted_lambda_res_41357, lifted_lambda_res_41356);
                                            bool cond_41360 = !defunc_0_lt_res_41359;
                                            bool cond_t_res_41361 = !defunc_0_lt_res_41358;
                                            bool x_41362 = cond_41360 && cond_t_res_41361;
                                            bool loop_cond_t_res_41363 = slt32(zt_rhs_41350, next_depth_41242);
                                            bool x_41364 = x_41362 && loop_cond_t_res_41363;
                                            bool loop_while_tmp_42682 = x_41364;
                                            bool def_lt_tmp_42683 = defunc_0_lt_res_41358;
                                            int32_t bi_tmp_42684 = zt_rhs_41350;
                                            
                                            loop_while_41347 = loop_while_tmp_42682;
                                            def_lt_41348 = def_lt_tmp_42683;
                                            bi_41349 = bi_tmp_42684;
                                        }
                                        is_lt_41344 = loop_while_41347;
                                        is_lt_41345 = def_lt_41348;
                                        is_lt_41346 = bi_41349;
                                        
                                        bool x_41365 = !cond_41241;
                                        bool y_41366 = is_lt_41345 && x_41365;
                                        bool cond_41367 = cond_41241 || y_41366;
                                        int64_t loopres_f_res_f_res_41368;
                                        int64_t loopres_f_res_f_res_41369;
                                        
                                        if (cond_41367) {
                                            loopres_f_res_f_res_41368 = (int64_t) -1;
                                            loopres_f_res_f_res_41369 = (int64_t) 0;
                                        } else {
                                            int64_t tmp_41370 = add64(heshi_41226, step_41227);
                                            int64_t max_arg1_41371 = sdiv64(step_41227, (int64_t) 2);
                                            int64_t max_res_41372 = smax64((int64_t) 1, max_arg1_41371);
                                            
                                            loopres_f_res_f_res_41368 = tmp_41370;
                                            loopres_f_res_f_res_41369 = max_res_41372;
                                        }
                                        loopres_f_res_41311 = loopres_f_res_f_res_41368;
                                        loopres_f_res_41312 = loopres_f_res_f_res_41369;
                                    }
                                    loopres_41287 = loopres_f_res_41311;
                                    loopres_41288 = loopres_f_res_41312;
                                }
                                
                                bool loop_cond_41373 = slt64((int64_t) 0, loopres_41288);
                                bool loop_while_tmp_42673 = loop_cond_41373;
                                int64_t heshi_tmp_42674 = loopres_41287;
                                int64_t step_tmp_42675 = loopres_41288;
                                
                                loop_while_41225 = loop_while_tmp_42673;
                                heshi_41226 = heshi_tmp_42674;
                                step_41227 = step_tmp_42675;
                            }
                            bsearch_41222 = loop_while_41225;
                            bsearch_41223 = heshi_41226;
                            bsearch_41224 = step_41227;
                            defunc_res_f_res_41217 = bsearch_41223;
                        }
                        defunc_res_41214 = defunc_res_f_res_41217;
                    }
                    
                    bool cond_41374 = sle64(defunc_res_41214, (int64_t) 0);
                    int64_t inf_41375;
                    
                    if (cond_41374) {
                        inf_41375 = (int64_t) 0;
                    } else {
                        bool x_41376 = sle64((int64_t) 0, defunc_res_41214);
                        bool y_41377 = slt64(defunc_res_41214, dz2083U_25954);
                        bool bounds_check_41378 = x_41376 && y_41377;
                        bool index_certs_41379;
                        
                        if (!bounds_check_41378) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) defunc_res_41214, "] out of bounds for array of shape [", (long long) dz2083U_25954, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:728:41-57\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin_old.fut:848:4-853:57\n   #3  test_join2.fut:16:1-29:36\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t inf_f_res_41380 = ((int64_t *) bounds2_mem_41981.mem)[defunc_res_41214];
                        
                        inf_41375 = inf_f_res_41380;
                    }
                    
                    bool cond_41381 = defunc_res_41214 == zeze_rhs_31081;
                    int64_t sup_41382;
                    
                    if (cond_41381) {
                        sup_41382 = n2_25952;
                    } else {
                        int64_t tmp_41383 = add64((int64_t) 1, defunc_res_41214);
                        bool x_41384 = sle64((int64_t) 0, tmp_41383);
                        bool y_41385 = slt64(tmp_41383, dz2083U_25954);
                        bool bounds_check_41386 = x_41384 && y_41385;
                        bool index_certs_41387;
                        
                        if (!bounds_check_41386) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_41383, "] out of bounds for array of shape [", (long long) dz2083U_25954, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:729:47-65\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin_old.fut:848:4-853:57\n   #3  test_join2.fut:16:1-29:36\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t sup_f_res_41388 = ((int64_t *) bounds2_mem_41981.mem)[tmp_41383];
                        
                        sup_41382 = sup_f_res_41388;
                    }
                    
                    bool loop_cond_41389 = slt64(inf_41375, sup_41382);
                    bool lifted_lambda_res_41390;
                    int64_t lifted_lambda_res_41391;
                    int64_t lifted_lambda_res_41392;
                    int64_t lifted_lambda_res_41393;
                    bool loop_while_41394;
                    int64_t j_41395;
                    int64_t duome_41396;
                    int64_t i_41397;
                    
                    loop_while_41394 = loop_cond_41389;
                    j_41395 = inf_41375;
                    duome_41396 = (int64_t) 0;
                    i_41397 = inf_41375;
                    while (loop_while_41394) {
                        bool x_41398 = sle64((int64_t) 0, i_41397);
                        bool y_41399 = slt64(i_41397, n2_25952);
                        bool bounds_check_41400 = x_41398 && y_41399;
                        bool index_certs_41401;
                        
                        if (!bounds_check_41400) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_41397, "] out of bounds for array of shape [", (long long) n2_25952, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:732:29-38\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin_old.fut:848:4-853:57\n   #3  test_join2.fut:16:1-29:36\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int32_t zeze_rhs_41402 = ((int32_t *) mem_42124)[i_41397];
                        bool is_eq_41403 = defunc_0_byteSeq_to_prim_res_41191 == zeze_rhs_41402;
                        bool cond_41404 = slt64((int64_t) 0, duome_41396);
                        bool cond_41405 = is_eq_41403 || cond_41404;
                        int64_t next_j_41406;
                        
                        if (cond_41405) {
                            next_j_41406 = j_41395;
                        } else {
                            int64_t next_j_f_res_41407 = add64((int64_t) 1, j_41395);
                            
                            next_j_41406 = next_j_f_res_41407;
                        }
                        
                        int64_t next_duome_41408;
                        
                        if (is_eq_41403) {
                            int64_t next_duome_t_res_41409 = add64((int64_t) 1, duome_41396);
                            
                            next_duome_41408 = next_duome_t_res_41409;
                        } else {
                            next_duome_41408 = duome_41396;
                        }
                        
                        int64_t tmp_41410 = add64((int64_t) 1, i_41397);
                        bool loop_cond_41411 = slt64(tmp_41410, sup_41382);
                        bool loop_while_tmp_42685 = loop_cond_41411;
                        int64_t j_tmp_42686 = next_j_41406;
                        int64_t duome_tmp_42687 = next_duome_41408;
                        int64_t i_tmp_42688 = tmp_41410;
                        
                        loop_while_41394 = loop_while_tmp_42685;
                        j_41395 = j_tmp_42686;
                        duome_41396 = duome_tmp_42687;
                        i_41397 = i_tmp_42688;
                    }
                    lifted_lambda_res_41390 = loop_while_41394;
                    lifted_lambda_res_41391 = j_41395;
                    lifted_lambda_res_41392 = duome_41396;
                    lifted_lambda_res_41393 = i_41397;
                    
                    bool cond_41412 = slt64(lifted_lambda_res_41391, sup_41382);
                    int64_t lifted_lambda_res_41413;
                    
                    if (cond_41412) {
                        lifted_lambda_res_41413 = lifted_lambda_res_41392;
                    } else {
                        lifted_lambda_res_41413 = (int64_t) 0;
                    }
                    
                    int64_t lifted_lambda_res_41414;
                    
                    if (cond_41412) {
                        lifted_lambda_res_41414 = lifted_lambda_res_41391;
                    } else {
                        lifted_lambda_res_41414 = (int64_t) -1;
                    }
                    
                    bool lifted_lambda_res_41415 = slt64((int64_t) 1, lifted_lambda_res_41413);
                    int64_t defunc_0_f_res_41416 = btoi_bool_i64(lifted_lambda_res_41415);
                    int64_t defunc_0_op_res_41419 = add64(defunc_0_f_res_41416, scanacc_41841);
                    int64_t defunc_0_op_res_41422 = add64(lifted_lambda_res_41413, scanacc_41842);
                    int64_t zp_res_41425 = add64(lifted_lambda_res_41413, redout_41845);
                    
                    ((int64_t *) mem_42132)[i_41850] = defunc_0_op_res_41419;
                    ((int64_t *) mem_42134)[i_41850] = defunc_0_op_res_41422;
                    ((int64_t *) mem_42136)[i_41850] = defunc_0_f_res_41416;
                    ((int64_t *) mem_42138)[i_41850] = lifted_lambda_res_41414;
                    ((int64_t *) mem_42140)[i_41850] = lifted_lambda_res_41413;
                    ((int32_t *) mem_42142)[i_41850] = defunc_0_byteSeq_to_prim_res_41191;
                    
                    int64_t scanacc_tmp_42663 = defunc_0_op_res_41419;
                    int64_t scanacc_tmp_42664 = defunc_0_op_res_41422;
                    int64_t redout_tmp_42667 = zp_res_41425;
                    
                    scanacc_41841 = scanacc_tmp_42663;
                    scanacc_41842 = scanacc_tmp_42664;
                    redout_41845 = redout_tmp_42667;
                }
                discard_41857 = scanacc_41841;
                discard_41858 = scanacc_41842;
                defunc_res_41185 = redout_41845;
                
                int64_t m_f_res_41469;
                
                if (x_31083) {
                    int64_t x_41470 = ((int64_t *) mem_42132)[tmp_31084];
                    
                    m_f_res_41469 = x_41470;
                } else {
                    m_f_res_41469 = (int64_t) 0;
                }
                
                int64_t m_41471;
                
                if (cond_31082) {
                    m_41471 = (int64_t) 0;
                } else {
                    m_41471 = m_f_res_41469;
                }
                
                bool cond_41472 = slt64((int64_t) 0, m_41471);
                int64_t m_41473 = sub64(m_41471, (int64_t) 1);
                bool i_p_m_t_s_leq_w_41474 = slt64(m_41473, n1_25949);
                bool zzero_leq_i_p_m_t_s_41475 = sle64((int64_t) 0, m_41473);
                bool y_41476 = i_p_m_t_s_leq_w_41474 && zzero_leq_i_p_m_t_s_41475;
                bool i_lte_j_41477 = sle64((int64_t) 0, m_41471);
                bool forwards_ok_41478 = y_41476 && i_lte_j_41477;
                bool eq_x_zz_41479 = (int64_t) 0 == m_f_res_41469;
                bool p_and_eq_x_y_41480 = x_31083 && eq_x_zz_41479;
                bool empty_slice_41481 = cond_31082 || p_and_eq_x_y_41480;
                bool ok_or_empty_41482 = forwards_ok_41478 || empty_slice_41481;
                bool index_certs_41483;
                
                if (!ok_or_empty_41482) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_41471, "] out of bounds for array of shape [", (long long) n1_25949, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin_old.fut:848:4-853:57\n   #3  test_join2.fut:16:1-29:36\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                int64_t bytes_42187 = (int64_t) 8 * m_41471;
                
                if (mem_42188_cached_sizze_42775 < bytes_42187) {
                    err = lexical_realloc(ctx, &mem_42188, &mem_42188_cached_sizze_42775, bytes_42187);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                lmad_copy_8b(ctx, 1, (uint64_t *) mem_42188, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_42140, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_41471});
                
                int64_t max_mult_41486;
                
                if (cond_41472) {
                    if (mem_42190_cached_sizze_42776 < bytes_41993) {
                        err = lexical_realloc(ctx, &mem_42190, &mem_42190_cached_sizze_42776, bytes_41993);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    for (int64_t i_41865 = 0; i_41865 < n1_25949; i_41865++) {
                        int64_t eta_p_41488 = ((int64_t *) mem_42136)[i_41865];
                        int64_t eta_p_41489 = ((int64_t *) mem_42132)[i_41865];
                        bool cond_41490 = eta_p_41488 == (int64_t) 1;
                        int64_t lifted_lambda_res_41491;
                        
                        if (cond_41490) {
                            int64_t lifted_lambda_res_t_res_41492 = sub64(eta_p_41489, (int64_t) 1);
                            
                            lifted_lambda_res_41491 = lifted_lambda_res_t_res_41492;
                        } else {
                            lifted_lambda_res_41491 = (int64_t) -1;
                        }
                        ((int64_t *) mem_42190)[i_41865] = lifted_lambda_res_41491;
                    }
                    for (int64_t write_iter_41867 = 0; write_iter_41867 < n1_25949; write_iter_41867++) {
                        int64_t write_iv_41869 = ((int64_t *) mem_42190)[write_iter_41867];
                        int64_t write_iv_41870 = ((int64_t *) mem_42140)[write_iter_41867];
                        
                        if (sle64((int64_t) 0, write_iv_41869) && slt64(write_iv_41869, m_41471)) {
                            ((int64_t *) mem_42188)[write_iv_41869] = write_iv_41870;
                        }
                    }
                    
                    int64_t defunc_0_reduce_res_41508;
                    int64_t redout_41872 = (int64_t) -9223372036854775808;
                    
                    for (int64_t i_41873 = 0; i_41873 < m_41471; i_41873++) {
                        int64_t x_41509 = ((int64_t *) mem_42188)[i_41873];
                        int64_t max_res_41512 = smax64(x_41509, redout_41872);
                        int64_t redout_tmp_42691 = max_res_41512;
                        
                        redout_41872 = redout_tmp_42691;
                    }
                    defunc_0_reduce_res_41508 = redout_41872;
                    max_mult_41486 = defunc_0_reduce_res_41508;
                } else {
                    max_mult_41486 = (int64_t) 1;
                }
                
                int64_t bytes_42203 = (int64_t) 8 * defunc_res_41185;
                bool bounds_invalid_upwards_41548 = slt64(max_mult_41486, (int64_t) 1);
                bool valid_41550 = !bounds_invalid_upwards_41548;
                bool range_valid_c_41551;
                
                if (!valid_41550) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 1, "..<", (long long) max_mult_41486, " is invalid.", "-> #0  ../../../joins/ftHashJoin_old.fut:755:18-30\n   #1  ../../../joins/ftHashJoin_old.fut:848:4-853:57\n   #2  test_join2.fut:16:1-29:36\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                int64_t bytes_42241 = b_25950 * defunc_res_41185;
                
                if (mem_42180_cached_sizze_42774 < bytes_41993) {
                    err = lexical_realloc(ctx, &mem_42180, &mem_42180_cached_sizze_42774, bytes_41993);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                for (int64_t i_41861 = 0; i_41861 < n1_25949; i_41861++) {
                    int64_t eta_p_41457 = ((int64_t *) mem_42140)[i_41861];
                    int64_t zv_lhs_41458 = add64((int64_t) -1, i_41861);
                    int64_t tmp_41459 = smod64(zv_lhs_41458, n1_25949);
                    int64_t lifted_lambda_res_41460 = ((int64_t *) mem_42134)[tmp_41459];
                    bool cond_41461 = i_41861 == (int64_t) 0;
                    int64_t lifted_lambda_res_41462;
                    
                    if (cond_41461) {
                        lifted_lambda_res_41462 = (int64_t) 0;
                    } else {
                        lifted_lambda_res_41462 = lifted_lambda_res_41460;
                    }
                    
                    bool cond_41463 = slt64((int64_t) 0, eta_p_41457);
                    int64_t lifted_lambda_res_41464;
                    
                    if (cond_41463) {
                        lifted_lambda_res_41464 = lifted_lambda_res_41462;
                    } else {
                        lifted_lambda_res_41464 = (int64_t) -1;
                    }
                    ((int64_t *) mem_42180)[i_41861] = lifted_lambda_res_41464;
                }
                if (memblock_alloc(ctx, &mem_42204, bytes_42203, "mem_42204")) {
                    err = 1;
                    goto cleanup;
                }
                for (int64_t nest_i_42693 = 0; nest_i_42693 < defunc_res_41185; nest_i_42693++) {
                    ((int64_t *) mem_42204.mem)[nest_i_42693] = (int64_t) 0;
                }
                for (int64_t write_iter_41874 = 0; write_iter_41874 < n1_25949; write_iter_41874++) {
                    int64_t write_iv_41876 = ((int64_t *) mem_42180)[write_iter_41874];
                    
                    if (sle64((int64_t) 0, write_iv_41876) && slt64(write_iv_41876, defunc_res_41185)) {
                        ((int64_t *) mem_42204.mem)[write_iv_41876] = write_iter_41874;
                    }
                }
                if (memblock_alloc(ctx, &mem_42212, bytes_42203, "mem_42212")) {
                    err = 1;
                    goto cleanup;
                }
                for (int64_t nest_i_42695 = 0; nest_i_42695 < defunc_res_41185; nest_i_42695++) {
                    ((int64_t *) mem_42212.mem)[nest_i_42695] = (int64_t) 1;
                }
                
                int64_t distance_41549 = sub64(max_mult_41486, (int64_t) 1);
                
                if (memblock_set(ctx, &mem_param_42215, &mem_42212, "mem_42212") != 0)
                    return 1;
                if (memblock_set(ctx, &mem_param_42218, &mem_42204, "mem_42204") != 0)
                    return 1;
                for (int64_t i_41554 = 0; i_41554 < distance_41549; i_41554++) {
                    int64_t index_primexp_41557 = add64((int64_t) 1, i_41554);
                    
                    if (memblock_alloc(ctx, &mem_42220, bytes_42203, "mem_42220")) {
                        err = 1;
                        goto cleanup;
                    }
                    lmad_copy_8b(ctx, 1, (uint64_t *) mem_42220.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_42218.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {defunc_res_41185});
                    if (memblock_alloc(ctx, &mem_42222, bytes_42203, "mem_42222")) {
                        err = 1;
                        goto cleanup;
                    }
                    lmad_copy_8b(ctx, 1, (uint64_t *) mem_42222.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_42215.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {defunc_res_41185});
                    
                    int64_t replicate_arg1_41560 = add64((int64_t) 2, i_41554);
                    
                    for (int64_t write_iter_41879 = 0; write_iter_41879 < n1_25949; write_iter_41879++) {
                        int64_t zlze_lhs_41568 = ((int64_t *) mem_42140)[write_iter_41879];
                        bool cond_41569 = sle64(zlze_lhs_41568, index_primexp_41557);
                        int64_t lifted_lambda_res_41570;
                        
                        if (cond_41569) {
                            lifted_lambda_res_41570 = (int64_t) -1;
                        } else {
                            int64_t zp_lhs_41571 = ((int64_t *) mem_42180)[write_iter_41879];
                            int64_t lifted_lambda_res_f_res_41572 = add64(index_primexp_41557, zp_lhs_41571);
                            
                            lifted_lambda_res_41570 = lifted_lambda_res_f_res_41572;
                        }
                        if (sle64((int64_t) 0, lifted_lambda_res_41570) && slt64(lifted_lambda_res_41570, defunc_res_41185)) {
                            ((int64_t *) mem_42222.mem)[lifted_lambda_res_41570] = replicate_arg1_41560;
                        }
                        if (sle64((int64_t) 0, lifted_lambda_res_41570) && slt64(lifted_lambda_res_41570, defunc_res_41185)) {
                            ((int64_t *) mem_42220.mem)[lifted_lambda_res_41570] = write_iter_41879;
                        }
                    }
                    if (memblock_set(ctx, &mem_param_tmp_42696, &mem_42222, "mem_42222") != 0)
                        return 1;
                    if (memblock_set(ctx, &mem_param_tmp_42697, &mem_42220, "mem_42220") != 0)
                        return 1;
                    if (memblock_set(ctx, &mem_param_42215, &mem_param_tmp_42696, "mem_param_tmp_42696") != 0)
                        return 1;
                    if (memblock_set(ctx, &mem_param_42218, &mem_param_tmp_42697, "mem_param_tmp_42697") != 0)
                        return 1;
                }
                if (memblock_set(ctx, &ext_mem_42240, &mem_param_42215, "mem_param_42215") != 0)
                    return 1;
                if (memblock_set(ctx, &ext_mem_42239, &mem_param_42218, "mem_param_42218") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_42204, "mem_42204") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_42212, "mem_42212") != 0)
                    return 1;
                if (memblock_alloc(ctx, &mem_42242, bytes_42241, "mem_42242")) {
                    err = 1;
                    goto cleanup;
                }
                if (memblock_alloc(ctx, &mem_42244, bytes_42203, "mem_42244")) {
                    err = 1;
                    goto cleanup;
                }
                for (int64_t i_41889 = 0; i_41889 < defunc_res_41185; i_41889++) {
                    int64_t eta_p_41586 = ((int64_t *) ext_mem_42239.mem)[i_41889];
                    bool x_41587 = sle64((int64_t) 0, eta_p_41586);
                    bool y_41588 = slt64(eta_p_41586, n1_25949);
                    bool bounds_check_41589 = x_41587 && y_41588;
                    bool index_certs_41590;
                    
                    if (!bounds_check_41589) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_41586, "] out of bounds for array of shape [", (long long) n1_25949, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:765:16-26\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin_old.fut:764:8-772:6\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-853:57\n   #4  test_join2.fut:16:1-29:36\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t eta_p_41585 = ((int64_t *) ext_mem_42240.mem)[i_41889];
                    int32_t rv_41591 = ((int32_t *) mem_42142)[eta_p_41586];
                    int64_t inf_s_idx_41592 = ((int64_t *) mem_42138)[eta_p_41586];
                    int64_t tmp_41593 = sub64(inf_s_idx_41592, (int64_t) 1);
                    bool loop_cond_41594 = slt64((int64_t) 0, eta_p_41585);
                    bool kth_match_41595;
                    int64_t kth_match_41596;
                    int64_t kth_match_41597;
                    bool loop_while_41598;
                    int64_t j_41599;
                    int64_t m_41600;
                    
                    loop_while_41598 = loop_cond_41594;
                    j_41599 = tmp_41593;
                    m_41600 = (int64_t) 0;
                    while (loop_while_41598) {
                        int64_t zeze_rhs_41601 = add64((int64_t) 1, j_41599);
                        bool x_41602 = sle64((int64_t) 0, zeze_rhs_41601);
                        bool y_41603 = slt64(zeze_rhs_41601, n2_25952);
                        bool bounds_check_41604 = x_41602 && y_41603;
                        bool index_certs_41605;
                        
                        if (!bounds_check_41604) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) zeze_rhs_41601, "] out of bounds for array of shape [", (long long) n2_25952, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:769:29-40\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin_old.fut:764:8-772:6\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-853:57\n   #4  test_join2.fut:16:1-29:36\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int32_t zeze_rhs_41606 = ((int32_t *) mem_42124)[zeze_rhs_41601];
                        bool is_eq_41607 = rv_41591 == zeze_rhs_41606;
                        int64_t loopres_41608;
                        
                        if (is_eq_41607) {
                            int64_t tmp_41609 = add64((int64_t) 1, m_41600);
                            
                            loopres_41608 = tmp_41609;
                        } else {
                            loopres_41608 = m_41600;
                        }
                        
                        bool loop_cond_41610 = slt64(loopres_41608, eta_p_41585);
                        bool loop_while_tmp_42704 = loop_cond_41610;
                        int64_t j_tmp_42705 = zeze_rhs_41601;
                        int64_t m_tmp_42706 = loopres_41608;
                        
                        loop_while_41598 = loop_while_tmp_42704;
                        j_41599 = j_tmp_42705;
                        m_41600 = m_tmp_42706;
                    }
                    kth_match_41595 = loop_while_41598;
                    kth_match_41596 = j_41599;
                    kth_match_41597 = m_41600;
                    lmad_copy_1b(ctx, 1, (uint8_t *) mem_42242.mem, i_41889 * b_25950, (int64_t []) {(int64_t) 1}, (uint8_t *) ks1_mem_41977.mem, eta_p_41586 * b_25950, (int64_t []) {(int64_t) 1}, (int64_t []) {b_25950});
                    ((int64_t *) mem_42244.mem)[i_41889] = kth_match_41596;
                }
                if (memblock_unref(ctx, &ext_mem_42240, "ext_mem_42240") != 0)
                    return 1;
                if (memblock_alloc(ctx, &mem_42260, bytes_42203, "mem_42260")) {
                    err = 1;
                    goto cleanup;
                }
                lmad_copy_8b(ctx, 1, (uint64_t *) mem_42260.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_42239.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {defunc_res_41185});
                if (memblock_unref(ctx, &ext_mem_42239, "ext_mem_42239") != 0)
                    return 1;
                if (memblock_set(ctx, &ext_mem_42268, &mem_42260, "mem_42260") != 0)
                    return 1;
                if (memblock_set(ctx, &ext_mem_42267, &mem_42244, "mem_42244") != 0)
                    return 1;
                if (memblock_set(ctx, &ext_mem_42264, &mem_42242, "mem_42242") != 0)
                    return 1;
                radix_hash_join_res_f_res_f_res_31827 = defunc_res_41185;
            } else {
                if (mem_41986_cached_sizze_42757 < bytes_41985) {
                    err = lexical_realloc(ctx, &mem_41986, &mem_41986_cached_sizze_42757, bytes_41985);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                for (int64_t i_41894 = 0; i_41894 < n2_25952; i_41894++) {
                    int64_t defunc_0_byteSeq_to_prim_res_32662;
                    int64_t y_32664 = (int64_t) 0;
                    
                    for (int64_t i_32663 = 0; i_32663 < b_25950; i_32663++) {
                        int64_t zm_lhs_32665 = sub64(b_25950, i_32663);
                        int64_t from_u8_arg0_32666 = sub64(zm_lhs_32665, (int64_t) 1);
                        bool x_32667 = sle64((int64_t) 0, from_u8_arg0_32666);
                        bool y_32668 = slt64(from_u8_arg0_32666, b_25950);
                        bool bounds_check_32669 = x_32667 && y_32668;
                        bool index_certs_32670;
                        
                        if (!bounds_check_32669) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) from_u8_arg0_32666, "] out of bounds for array of shape [", (long long) b_25950, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:417:22-30\n   #1  ../../../joins/ftHashJoin_old.fut:427:75-82\n   #2  test_join2.fut:16:1-29:36\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int8_t from_u8_arg0_32671 = ((int8_t *) ks2_mem_41979.mem)[i_41894 * b_25950 + from_u8_arg0_32666];
                        int64_t u8_res_32672 = zext_i8_i64(from_u8_arg0_32671);
                        int32_t i64_res_32673 = sext_i64_i32(i_32663);
                        int32_t shift_left_rhs_32674 = mul32(8, i64_res_32673);
                        int64_t unsign_arg0_32675 = zext_i32_i64(shift_left_rhs_32674);
                        int64_t lifted_lambda_res_32676 = shl64(u8_res_32672, unsign_arg0_32675);
                        int64_t unsign_arg0_32677 = y_32664 | lifted_lambda_res_32676;
                        int64_t y_tmp_42708 = unsign_arg0_32677;
                        
                        y_32664 = y_tmp_42708;
                    }
                    defunc_0_byteSeq_to_prim_res_32662 = y_32664;
                    ((int64_t *) mem_41986)[i_41894] = defunc_0_byteSeq_to_prim_res_32662;
                }
                if (mem_41994_cached_sizze_42758 < bytes_41993) {
                    err = lexical_realloc(ctx, &mem_41994, &mem_41994_cached_sizze_42758, bytes_41993);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                if (mem_41996_cached_sizze_42759 < bytes_41993) {
                    err = lexical_realloc(ctx, &mem_41996, &mem_41996_cached_sizze_42759, bytes_41993);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                if (mem_41998_cached_sizze_42760 < bytes_41993) {
                    err = lexical_realloc(ctx, &mem_41998, &mem_41998_cached_sizze_42760, bytes_41993);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                if (mem_42000_cached_sizze_42761 < bytes_41993) {
                    err = lexical_realloc(ctx, &mem_42000, &mem_42000_cached_sizze_42761, bytes_41993);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                if (mem_42002_cached_sizze_42762 < bytes_41993) {
                    err = lexical_realloc(ctx, &mem_42002, &mem_42002_cached_sizze_42762, bytes_41993);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                if (mem_42004_cached_sizze_42763 < bytes_41993) {
                    err = lexical_realloc(ctx, &mem_42004, &mem_42004_cached_sizze_42763, bytes_41993);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                
                int64_t discard_41918;
                int64_t discard_41919;
                int64_t defunc_res_41700;
                int64_t scanacc_41902;
                int64_t scanacc_41903;
                int64_t redout_41906;
                
                scanacc_41902 = (int64_t) 0;
                scanacc_41903 = (int64_t) 0;
                redout_41906 = (int64_t) 0;
                for (int64_t i_41911 = 0; i_41911 < n1_25949; i_41911++) {
                    int64_t defunc_0_byteSeq_to_prim_res_37932;
                    int64_t y_37934 = (int64_t) 0;
                    
                    for (int64_t i_37933 = 0; i_37933 < b_25950; i_37933++) {
                        int64_t zm_lhs_37935 = sub64(b_25950, i_37933);
                        int64_t from_u8_arg0_37936 = sub64(zm_lhs_37935, (int64_t) 1);
                        bool x_37937 = sle64((int64_t) 0, from_u8_arg0_37936);
                        bool y_37938 = slt64(from_u8_arg0_37936, b_25950);
                        bool bounds_check_37939 = x_37937 && y_37938;
                        bool index_certs_37940;
                        
                        if (!bounds_check_37939) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) from_u8_arg0_37936, "] out of bounds for array of shape [", (long long) b_25950, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:417:22-30\n   #1  ../../../joins/ftHashJoin_old.fut:427:75-82\n   #2  test_join2.fut:16:1-29:36\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int8_t from_u8_arg0_37941 = ((int8_t *) ks1_mem_41977.mem)[i_41911 * b_25950 + from_u8_arg0_37936];
                        int64_t u8_res_37942 = zext_i8_i64(from_u8_arg0_37941);
                        int32_t i64_res_37943 = sext_i64_i32(i_37933);
                        int32_t shift_left_rhs_37944 = mul32(8, i64_res_37943);
                        int64_t unsign_arg0_37945 = zext_i32_i64(shift_left_rhs_37944);
                        int64_t lifted_lambda_res_37946 = shl64(u8_res_37942, unsign_arg0_37945);
                        int64_t unsign_arg0_37947 = y_37934 | lifted_lambda_res_37946;
                        int64_t y_tmp_42718 = unsign_arg0_37947;
                        
                        y_37934 = y_tmp_42718;
                    }
                    defunc_0_byteSeq_to_prim_res_37932 = y_37934;
                    
                    int64_t lifted_lambda_res_37949 = u64_arg0_31080 & defunc_0_byteSeq_to_prim_res_37932;
                    bool x_37950 = sle64((int64_t) 0, lifted_lambda_res_37949);
                    bool y_37951 = slt64(lifted_lambda_res_37949, dzlz7bUZLztztZRz20U2z20UZLi32z20Uradix_sizzeZRz7dUzg_31070);
                    bool bounds_check_37952 = x_37950 && y_37951;
                    bool index_certs_37953;
                    
                    if (!bounds_check_37952) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) lifted_lambda_res_37949, "] out of bounds for array of shape [", (long long) dzlz7bUZLztztZRz20U2z20UZLi32z20Uradix_sizzeZRz7dUzg_31070, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:530:13-35\n   #1  ../../../joins/ftHashJoin_old.fut:784:3-791:87\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-854:44\n   #4  test_join2.fut:16:1-29:36\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t spi_37954 = ((int64_t *) first_idx2_mem_41983.mem)[lifted_lambda_res_37949];
                    bool cond_37955 = slt64(spi_37954, (int64_t) 0);
                    int64_t defunc_res_37956;
                    
                    if (cond_37955) {
                        defunc_res_37956 = (int64_t) -1;
                    } else {
                        int64_t zeze_rhs_37957 = ((int64_t *) last_idx2_mem_41984.mem)[lifted_lambda_res_37949];
                        bool cond_37958 = spi_37954 == zeze_rhs_37957;
                        int64_t defunc_res_f_res_37959;
                        
                        if (cond_37958) {
                            defunc_res_f_res_37959 = spi_37954;
                        } else {
                            int64_t zs_lhs_37960 = sub64(zeze_rhs_37957, spi_37954);
                            int64_t max_arg1_37961 = sdiv64(zs_lhs_37960, (int64_t) 2);
                            int64_t max_res_37962 = smax64((int64_t) 1, max_arg1_37961);
                            bool loop_cond_37963 = slt64((int64_t) 0, max_res_37962);
                            bool bsearch_37964;
                            int64_t bsearch_37965;
                            int64_t bsearch_37966;
                            bool loop_while_37967;
                            int64_t heshi_37968;
                            int64_t step_37969;
                            
                            loop_while_37967 = loop_cond_37963;
                            heshi_37968 = spi_37954;
                            step_37969 = max_res_37962;
                            while (loop_while_37967) {
                                bool x_37970 = sle64((int64_t) 0, heshi_37968);
                                bool y_37971 = slt64(heshi_37968, dz2083U_25954);
                                bool bounds_check_37972 = x_37970 && y_37971;
                                bool index_certs_37973;
                                
                                if (!bounds_check_37972) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) heshi_37968, "] out of bounds for array of shape [", (long long) dz2083U_25954, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:539:25-40\n   #1  ../../../joins/ftHashJoin_old.fut:784:3-791:87\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-854:44\n   #4  test_join2.fut:16:1-29:36\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t cur_S_37991 = ((int64_t *) bounds2_mem_41981.mem)[heshi_37968];
                                bool x_37992 = sle64((int64_t) 0, cur_S_37991);
                                bool y_37993 = slt64(cur_S_37991, n2_25952);
                                bool bounds_check_37994 = x_37992 && y_37993;
                                bool index_certs_37995;
                                
                                if (!bounds_check_37994) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) cur_S_37991, "] out of bounds for array of shape [", (long long) n2_25952, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:542:21-40\n   #1  ../../../joins/ftHashJoin_old.fut:784:3-791:87\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-854:44\n   #4  test_join2.fut:16:1-29:36\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int32_t cur_depth_37974 = ((int32_t *) depths2_mem_41982.mem)[heshi_37968];
                                bool cond_37975 = heshi_37968 == spi_37954;
                                int32_t prev_depth_37976;
                                
                                if (cond_37975) {
                                    prev_depth_37976 = cur_depth_37974;
                                } else {
                                    int64_t tmp_37977 = sub64(heshi_37968, (int64_t) 1);
                                    bool x_37978 = sle64((int64_t) 0, tmp_37977);
                                    bool y_37979 = slt64(tmp_37977, dz2083U_25954);
                                    bool bounds_check_37980 = x_37978 && y_37979;
                                    bool index_certs_37981;
                                    
                                    if (!bounds_check_37980) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_37977, "] out of bounds for array of shape [", (long long) dz2083U_25954, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:540:60-77\n   #1  ../../../joins/ftHashJoin_old.fut:784:3-791:87\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-854:44\n   #4  test_join2.fut:16:1-29:36\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    int32_t prev_depth_f_res_37982 = ((int32_t *) depths2_mem_41982.mem)[tmp_37977];
                                    
                                    prev_depth_37976 = prev_depth_f_res_37982;
                                }
                                
                                bool cond_37983 = heshi_37968 == zeze_rhs_37957;
                                int32_t next_depth_37984;
                                
                                if (cond_37983) {
                                    next_depth_37984 = cur_depth_37974;
                                } else {
                                    int64_t tmp_37985 = add64((int64_t) 1, heshi_37968);
                                    bool x_37986 = sle64((int64_t) 0, tmp_37985);
                                    bool y_37987 = slt64(tmp_37985, dz2083U_25954);
                                    bool bounds_check_37988 = x_37986 && y_37987;
                                    bool index_certs_37989;
                                    
                                    if (!bounds_check_37988) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_37985, "] out of bounds for array of shape [", (long long) dz2083U_25954, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:541:64-81\n   #1  ../../../joins/ftHashJoin_old.fut:784:3-791:87\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-854:44\n   #4  test_join2.fut:16:1-29:36\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    int32_t next_depth_f_res_37990 = ((int32_t *) depths2_mem_41982.mem)[tmp_37985];
                                    
                                    next_depth_37984 = next_depth_f_res_37990;
                                }
                                
                                int64_t cur_S_37996 = ((int64_t *) mem_41986)[cur_S_37991];
                                int64_t prev_S_37997;
                                
                                if (cond_37975) {
                                    prev_S_37997 = cur_S_37996;
                                } else {
                                    int64_t tmp_37998 = sub64(heshi_37968, (int64_t) 1);
                                    bool x_37999 = sle64((int64_t) 0, tmp_37998);
                                    bool y_38000 = slt64(tmp_37998, dz2083U_25954);
                                    bool bounds_check_38001 = x_37999 && y_38000;
                                    bool index_certs_38002;
                                    
                                    if (!bounds_check_38001) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_37998, "] out of bounds for array of shape [", (long long) dz2083U_25954, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:543:55-72\n   #1  ../../../joins/ftHashJoin_old.fut:784:3-791:87\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-854:44\n   #4  test_join2.fut:16:1-29:36\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    int64_t tmp_38003 = ((int64_t *) bounds2_mem_41981.mem)[tmp_37998];
                                    bool x_38004 = sle64((int64_t) 0, tmp_38003);
                                    bool y_38005 = slt64(tmp_38003, n2_25952);
                                    bool bounds_check_38006 = x_38004 && y_38005;
                                    bool index_certs_38007;
                                    
                                    if (!bounds_check_38006) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_38003, "] out of bounds for array of shape [", (long long) n2_25952, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:543:52-73\n   #1  ../../../joins/ftHashJoin_old.fut:784:3-791:87\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-854:44\n   #4  test_join2.fut:16:1-29:36\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    int64_t prev_S_f_res_38008 = ((int64_t *) mem_41986)[tmp_38003];
                                    
                                    prev_S_37997 = prev_S_f_res_38008;
                                }
                                
                                int64_t next_S_38009;
                                
                                if (cond_37983) {
                                    next_S_38009 = cur_S_37996;
                                } else {
                                    int64_t tmp_38010 = add64((int64_t) 1, heshi_37968);
                                    bool x_38011 = sle64((int64_t) 0, tmp_38010);
                                    bool y_38012 = slt64(tmp_38010, dz2083U_25954);
                                    bool bounds_check_38013 = x_38011 && y_38012;
                                    bool index_certs_38014;
                                    
                                    if (!bounds_check_38013) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_38010, "] out of bounds for array of shape [", (long long) dz2083U_25954, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:544:59-76\n   #1  ../../../joins/ftHashJoin_old.fut:784:3-791:87\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-854:44\n   #4  test_join2.fut:16:1-29:36\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    int64_t tmp_38015 = ((int64_t *) bounds2_mem_41981.mem)[tmp_38010];
                                    bool x_38016 = sle64((int64_t) 0, tmp_38015);
                                    bool y_38017 = slt64(tmp_38015, n2_25952);
                                    bool bounds_check_38018 = x_38016 && y_38017;
                                    bool index_certs_38019;
                                    
                                    if (!bounds_check_38018) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_38015, "] out of bounds for array of shape [", (long long) n2_25952, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:544:56-77\n   #1  ../../../joins/ftHashJoin_old.fut:784:3-791:87\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-854:44\n   #4  test_join2.fut:16:1-29:36\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    int64_t next_S_f_res_38020 = ((int64_t *) mem_41986)[tmp_38015];
                                    
                                    next_S_38009 = next_S_f_res_38020;
                                }
                                
                                int32_t prim_get_radix_arg0_38021 = mul32(f64_res_31066, cur_depth_37974);
                                int64_t unsign_arg0_38022 = zext_i32_i64(prim_get_radix_arg0_38021);
                                int64_t asPow_38023 = shl64((int64_t) 1, unsign_arg0_38022);
                                int64_t za_rhs_38024 = sub64(asPow_38023, (int64_t) 1);
                                int64_t lifted_lambda_res_38025 = defunc_0_byteSeq_to_prim_res_37932 & za_rhs_38024;
                                int64_t lifted_lambda_res_38026 = cur_S_37996 & za_rhs_38024;
                                bool defunc_0_eq_res_38027 = lifted_lambda_res_38025 == lifted_lambda_res_38026;
                                int64_t loopres_38028;
                                int64_t loopres_38029;
                                
                                if (defunc_0_eq_res_38027) {
                                    loopres_38028 = heshi_37968;
                                    loopres_38029 = (int64_t) 0;
                                } else {
                                    bool loop_cond_t_res_38030 = slt32(0, cur_depth_37974);
                                    bool is_lt_38031;
                                    bool is_lt_38032;
                                    int32_t is_lt_38033;
                                    bool loop_while_38034;
                                    bool def_lt_38035;
                                    int32_t bi_38036;
                                    
                                    loop_while_38034 = loop_cond_t_res_38030;
                                    def_lt_38035 = 0;
                                    bi_38036 = 0;
                                    while (loop_while_38034) {
                                        int32_t zt_rhs_38037 = add32(1, bi_38036);
                                        int32_t prim_get_radix_arg0_38038 = mul32(f64_res_31066, zt_rhs_38037);
                                        int64_t unsign_arg0_38039 = zext_i32_i64(prim_get_radix_arg0_38038);
                                        int64_t asPow_38040 = shl64((int64_t) 1, unsign_arg0_38039);
                                        int64_t za_rhs_38041 = sub64(asPow_38040, (int64_t) 1);
                                        int64_t lifted_lambda_res_38042 = defunc_0_byteSeq_to_prim_res_37932 & za_rhs_38041;
                                        int64_t lifted_lambda_res_38043 = cur_S_37996 & za_rhs_38041;
                                        bool defunc_0_lt_res_38044 = ult64(lifted_lambda_res_38042, lifted_lambda_res_38043);
                                        bool defunc_0_lt_res_38045 = ult64(lifted_lambda_res_38043, lifted_lambda_res_38042);
                                        bool cond_38046 = !defunc_0_lt_res_38045;
                                        bool cond_t_res_38047 = !defunc_0_lt_res_38044;
                                        bool x_38048 = cond_38046 && cond_t_res_38047;
                                        bool loop_cond_t_res_38049 = slt32(zt_rhs_38037, cur_depth_37974);
                                        bool x_38050 = x_38048 && loop_cond_t_res_38049;
                                        bool loop_while_tmp_42722 = x_38050;
                                        bool def_lt_tmp_42723 = defunc_0_lt_res_38044;
                                        int32_t bi_tmp_42724 = zt_rhs_38037;
                                        
                                        loop_while_38034 = loop_while_tmp_42722;
                                        def_lt_38035 = def_lt_tmp_42723;
                                        bi_38036 = bi_tmp_42724;
                                    }
                                    is_lt_38031 = loop_while_38034;
                                    is_lt_38032 = def_lt_38035;
                                    is_lt_38033 = bi_38036;
                                    
                                    int64_t loopres_f_res_38051;
                                    int64_t loopres_f_res_38052;
                                    
                                    if (is_lt_38032) {
                                        bool loop_cond_t_res_41625 = slt32(0, prev_depth_37976);
                                        bool is_gt_41626;
                                        bool is_gt_41627;
                                        int32_t is_gt_41628;
                                        bool loop_while_41629;
                                        bool def_gt_41630;
                                        int32_t bi_41631;
                                        
                                        loop_while_41629 = loop_cond_t_res_41625;
                                        def_gt_41630 = 0;
                                        bi_41631 = 0;
                                        while (loop_while_41629) {
                                            int32_t zt_rhs_41632 = add32(1, bi_41631);
                                            int32_t prim_get_radix_arg0_41633 = mul32(f64_res_31066, zt_rhs_41632);
                                            int64_t unsign_arg0_41634 = zext_i32_i64(prim_get_radix_arg0_41633);
                                            int64_t asPow_41635 = shl64((int64_t) 1, unsign_arg0_41634);
                                            int64_t za_rhs_41636 = sub64(asPow_41635, (int64_t) 1);
                                            int64_t lifted_lambda_res_41637 = defunc_0_byteSeq_to_prim_res_37932 & za_rhs_41636;
                                            int64_t lifted_lambda_res_41638 = prev_S_37997 & za_rhs_41636;
                                            bool defunc_0_gt_res_41639 = ult64(lifted_lambda_res_41638, lifted_lambda_res_41637);
                                            bool defunc_0_gt_res_41640 = ult64(lifted_lambda_res_41637, lifted_lambda_res_41638);
                                            bool cond_41641 = !defunc_0_gt_res_41639;
                                            bool cond_t_res_41642 = !defunc_0_gt_res_41640;
                                            bool x_41643 = cond_41641 && cond_t_res_41642;
                                            bool loop_cond_t_res_41644 = slt32(zt_rhs_41632, prev_depth_37976);
                                            bool x_41645 = x_41643 && loop_cond_t_res_41644;
                                            bool loop_while_tmp_42725 = x_41645;
                                            bool def_gt_tmp_42726 = defunc_0_gt_res_41639;
                                            int32_t bi_tmp_42727 = zt_rhs_41632;
                                            
                                            loop_while_41629 = loop_while_tmp_42725;
                                            def_gt_41630 = def_gt_tmp_42726;
                                            bi_41631 = bi_tmp_42727;
                                        }
                                        is_gt_41626 = loop_while_41629;
                                        is_gt_41627 = def_gt_41630;
                                        is_gt_41628 = bi_41631;
                                        
                                        bool x_41646 = !cond_37975;
                                        bool y_41647 = is_gt_41627 && x_41646;
                                        bool cond_41648 = cond_37975 || y_41647;
                                        int64_t loopres_f_res_t_res_41649;
                                        int64_t loopres_f_res_t_res_41650;
                                        
                                        if (cond_41648) {
                                            loopres_f_res_t_res_41649 = (int64_t) -1;
                                            loopres_f_res_t_res_41650 = (int64_t) 0;
                                        } else {
                                            int64_t tmp_41651 = sub64(heshi_37968, step_37969);
                                            int64_t max_arg1_41652 = sdiv64(step_37969, (int64_t) 2);
                                            int64_t max_res_41653 = smax64((int64_t) 1, max_arg1_41652);
                                            
                                            loopres_f_res_t_res_41649 = tmp_41651;
                                            loopres_f_res_t_res_41650 = max_res_41653;
                                        }
                                        loopres_f_res_38051 = loopres_f_res_t_res_41649;
                                        loopres_f_res_38052 = loopres_f_res_t_res_41650;
                                    } else {
                                        bool loop_cond_t_res_38082 = slt32(0, next_depth_37984);
                                        bool is_lt_38083;
                                        bool is_lt_38084;
                                        int32_t is_lt_38085;
                                        bool loop_while_38086;
                                        bool def_lt_38087;
                                        int32_t bi_38088;
                                        
                                        loop_while_38086 = loop_cond_t_res_38082;
                                        def_lt_38087 = 0;
                                        bi_38088 = 0;
                                        while (loop_while_38086) {
                                            int32_t zt_rhs_38089 = add32(1, bi_38088);
                                            int32_t prim_get_radix_arg0_38090 = mul32(f64_res_31066, zt_rhs_38089);
                                            int64_t unsign_arg0_38091 = zext_i32_i64(prim_get_radix_arg0_38090);
                                            int64_t asPow_38092 = shl64((int64_t) 1, unsign_arg0_38091);
                                            int64_t za_rhs_38093 = sub64(asPow_38092, (int64_t) 1);
                                            int64_t lifted_lambda_res_38094 = defunc_0_byteSeq_to_prim_res_37932 & za_rhs_38093;
                                            int64_t lifted_lambda_res_38095 = next_S_38009 & za_rhs_38093;
                                            bool defunc_0_lt_res_38096 = ult64(lifted_lambda_res_38094, lifted_lambda_res_38095);
                                            bool defunc_0_lt_res_38097 = ult64(lifted_lambda_res_38095, lifted_lambda_res_38094);
                                            bool cond_38098 = !defunc_0_lt_res_38097;
                                            bool cond_t_res_38099 = !defunc_0_lt_res_38096;
                                            bool x_38100 = cond_38098 && cond_t_res_38099;
                                            bool loop_cond_t_res_38101 = slt32(zt_rhs_38089, next_depth_37984);
                                            bool x_38102 = x_38100 && loop_cond_t_res_38101;
                                            bool loop_while_tmp_42728 = x_38102;
                                            bool def_lt_tmp_42729 = defunc_0_lt_res_38096;
                                            int32_t bi_tmp_42730 = zt_rhs_38089;
                                            
                                            loop_while_38086 = loop_while_tmp_42728;
                                            def_lt_38087 = def_lt_tmp_42729;
                                            bi_38088 = bi_tmp_42730;
                                        }
                                        is_lt_38083 = loop_while_38086;
                                        is_lt_38084 = def_lt_38087;
                                        is_lt_38085 = bi_38088;
                                        
                                        bool x_38103 = !cond_37983;
                                        bool y_38104 = is_lt_38084 && x_38103;
                                        bool cond_38105 = cond_37983 || y_38104;
                                        int64_t loopres_f_res_f_res_38106;
                                        int64_t loopres_f_res_f_res_38107;
                                        
                                        if (cond_38105) {
                                            loopres_f_res_f_res_38106 = (int64_t) -1;
                                            loopres_f_res_f_res_38107 = (int64_t) 0;
                                        } else {
                                            int64_t tmp_38108 = add64(heshi_37968, step_37969);
                                            int64_t max_arg1_38109 = sdiv64(step_37969, (int64_t) 2);
                                            int64_t max_res_38110 = smax64((int64_t) 1, max_arg1_38109);
                                            
                                            loopres_f_res_f_res_38106 = tmp_38108;
                                            loopres_f_res_f_res_38107 = max_res_38110;
                                        }
                                        loopres_f_res_38051 = loopres_f_res_f_res_38106;
                                        loopres_f_res_38052 = loopres_f_res_f_res_38107;
                                    }
                                    loopres_38028 = loopres_f_res_38051;
                                    loopres_38029 = loopres_f_res_38052;
                                }
                                
                                bool loop_cond_38111 = slt64((int64_t) 0, loopres_38029);
                                bool loop_while_tmp_42719 = loop_cond_38111;
                                int64_t heshi_tmp_42720 = loopres_38028;
                                int64_t step_tmp_42721 = loopres_38029;
                                
                                loop_while_37967 = loop_while_tmp_42719;
                                heshi_37968 = heshi_tmp_42720;
                                step_37969 = step_tmp_42721;
                            }
                            bsearch_37964 = loop_while_37967;
                            bsearch_37965 = heshi_37968;
                            bsearch_37966 = step_37969;
                            defunc_res_f_res_37959 = bsearch_37965;
                        }
                        defunc_res_37956 = defunc_res_f_res_37959;
                    }
                    
                    bool cond_38112 = sle64(defunc_res_37956, (int64_t) 0);
                    int64_t inf_38113;
                    
                    if (cond_38112) {
                        inf_38113 = (int64_t) 0;
                    } else {
                        bool x_38114 = sle64((int64_t) 0, defunc_res_37956);
                        bool y_38115 = slt64(defunc_res_37956, dz2083U_25954);
                        bool bounds_check_38116 = x_38114 && y_38115;
                        bool index_certs_38117;
                        
                        if (!bounds_check_38116) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) defunc_res_37956, "] out of bounds for array of shape [", (long long) dz2083U_25954, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:792:41-57\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin_old.fut:848:4-854:44\n   #3  test_join2.fut:16:1-29:36\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t inf_f_res_38118 = ((int64_t *) bounds2_mem_41981.mem)[defunc_res_37956];
                        
                        inf_38113 = inf_f_res_38118;
                    }
                    
                    bool cond_38119 = defunc_res_37956 == zeze_rhs_31081;
                    int64_t sup_38120;
                    
                    if (cond_38119) {
                        sup_38120 = n2_25952;
                    } else {
                        int64_t tmp_38121 = add64((int64_t) 1, defunc_res_37956);
                        bool x_38122 = sle64((int64_t) 0, tmp_38121);
                        bool y_38123 = slt64(tmp_38121, dz2083U_25954);
                        bool bounds_check_38124 = x_38122 && y_38123;
                        bool index_certs_38125;
                        
                        if (!bounds_check_38124) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_38121, "] out of bounds for array of shape [", (long long) dz2083U_25954, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:793:47-65\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin_old.fut:848:4-854:44\n   #3  test_join2.fut:16:1-29:36\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t sup_f_res_38126 = ((int64_t *) bounds2_mem_41981.mem)[tmp_38121];
                        
                        sup_38120 = sup_f_res_38126;
                    }
                    
                    bool loop_cond_38127 = slt64(inf_38113, sup_38120);
                    bool lifted_lambda_res_38128;
                    int64_t lifted_lambda_res_38129;
                    int64_t lifted_lambda_res_38130;
                    int64_t lifted_lambda_res_38131;
                    bool loop_while_38132;
                    int64_t j_38133;
                    int64_t duome_38134;
                    int64_t i_38135;
                    
                    loop_while_38132 = loop_cond_38127;
                    j_38133 = inf_38113;
                    duome_38134 = (int64_t) 0;
                    i_38135 = inf_38113;
                    while (loop_while_38132) {
                        bool x_38136 = sle64((int64_t) 0, i_38135);
                        bool y_38137 = slt64(i_38135, n2_25952);
                        bool bounds_check_38138 = x_38136 && y_38137;
                        bool index_certs_38139;
                        
                        if (!bounds_check_38138) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_38135, "] out of bounds for array of shape [", (long long) n2_25952, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:796:29-38\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin_old.fut:848:4-854:44\n   #3  test_join2.fut:16:1-29:36\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t zeze_rhs_38140 = ((int64_t *) mem_41986)[i_38135];
                        bool is_eq_38141 = defunc_0_byteSeq_to_prim_res_37932 == zeze_rhs_38140;
                        bool cond_38142 = slt64((int64_t) 0, duome_38134);
                        bool cond_38143 = is_eq_38141 || cond_38142;
                        int64_t next_j_38144;
                        
                        if (cond_38143) {
                            next_j_38144 = j_38133;
                        } else {
                            int64_t next_j_f_res_38145 = add64((int64_t) 1, j_38133);
                            
                            next_j_38144 = next_j_f_res_38145;
                        }
                        
                        int64_t next_duome_38146;
                        
                        if (is_eq_38141) {
                            int64_t next_duome_t_res_41654 = add64((int64_t) 1, duome_38134);
                            
                            next_duome_38146 = next_duome_t_res_41654;
                        } else {
                            next_duome_38146 = duome_38134;
                        }
                        
                        int64_t tmp_38148 = add64((int64_t) 1, i_38135);
                        bool loop_cond_38149 = slt64(tmp_38148, sup_38120);
                        bool loop_while_tmp_42731 = loop_cond_38149;
                        int64_t j_tmp_42732 = next_j_38144;
                        int64_t duome_tmp_42733 = next_duome_38146;
                        int64_t i_tmp_42734 = tmp_38148;
                        
                        loop_while_38132 = loop_while_tmp_42731;
                        j_38133 = j_tmp_42732;
                        duome_38134 = duome_tmp_42733;
                        i_38135 = i_tmp_42734;
                    }
                    lifted_lambda_res_38128 = loop_while_38132;
                    lifted_lambda_res_38129 = j_38133;
                    lifted_lambda_res_38130 = duome_38134;
                    lifted_lambda_res_38131 = i_38135;
                    
                    bool cond_38150 = slt64(lifted_lambda_res_38129, sup_38120);
                    int64_t lifted_lambda_res_38151;
                    
                    if (cond_38150) {
                        lifted_lambda_res_38151 = lifted_lambda_res_38130;
                    } else {
                        lifted_lambda_res_38151 = (int64_t) 0;
                    }
                    
                    int64_t lifted_lambda_res_38152;
                    
                    if (cond_38150) {
                        lifted_lambda_res_38152 = lifted_lambda_res_38129;
                    } else {
                        lifted_lambda_res_38152 = (int64_t) -1;
                    }
                    
                    bool lifted_lambda_res_38155 = slt64((int64_t) 1, lifted_lambda_res_38151);
                    int64_t defunc_0_f_res_38156 = btoi_bool_i64(lifted_lambda_res_38155);
                    int64_t defunc_0_op_res_32431 = add64(defunc_0_f_res_38156, scanacc_41902);
                    int64_t defunc_0_op_res_32401 = add64(lifted_lambda_res_38151, scanacc_41903);
                    int64_t zp_res_32422 = add64(lifted_lambda_res_38151, redout_41906);
                    
                    ((int64_t *) mem_41994)[i_41911] = defunc_0_op_res_32431;
                    ((int64_t *) mem_41996)[i_41911] = defunc_0_op_res_32401;
                    ((int64_t *) mem_41998)[i_41911] = defunc_0_f_res_38156;
                    ((int64_t *) mem_42000)[i_41911] = lifted_lambda_res_38152;
                    ((int64_t *) mem_42002)[i_41911] = lifted_lambda_res_38151;
                    ((int64_t *) mem_42004)[i_41911] = defunc_0_byteSeq_to_prim_res_37932;
                    
                    int64_t scanacc_tmp_42709 = defunc_0_op_res_32431;
                    int64_t scanacc_tmp_42710 = defunc_0_op_res_32401;
                    int64_t redout_tmp_42713 = zp_res_32422;
                    
                    scanacc_41902 = scanacc_tmp_42709;
                    scanacc_41903 = scanacc_tmp_42710;
                    redout_41906 = redout_tmp_42713;
                }
                discard_41918 = scanacc_41902;
                discard_41919 = scanacc_41903;
                defunc_res_41700 = redout_41906;
                
                int64_t m_f_res_32432;
                
                if (x_31083) {
                    int64_t x_41655 = ((int64_t *) mem_41994)[tmp_31084];
                    
                    m_f_res_32432 = x_41655;
                } else {
                    m_f_res_32432 = (int64_t) 0;
                }
                
                int64_t m_32434;
                
                if (cond_31082) {
                    m_32434 = (int64_t) 0;
                } else {
                    m_32434 = m_f_res_32432;
                }
                
                bool cond_32457 = slt64((int64_t) 0, m_32434);
                int64_t m_32444 = sub64(m_32434, (int64_t) 1);
                bool i_p_m_t_s_leq_w_32446 = slt64(m_32444, n1_25949);
                bool zzero_leq_i_p_m_t_s_32445 = sle64((int64_t) 0, m_32444);
                bool y_32448 = zzero_leq_i_p_m_t_s_32445 && i_p_m_t_s_leq_w_32446;
                bool i_lte_j_32447 = sle64((int64_t) 0, m_32434);
                bool forwards_ok_32449 = i_lte_j_32447 && y_32448;
                bool eq_x_zz_32441 = (int64_t) 0 == m_f_res_32432;
                bool p_and_eq_x_y_32442 = x_31083 && eq_x_zz_32441;
                bool empty_slice_32443 = cond_31082 || p_and_eq_x_y_32442;
                bool ok_or_empty_32450 = empty_slice_32443 || forwards_ok_32449;
                bool index_certs_32451;
                
                if (!ok_or_empty_32450) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_32434, "] out of bounds for array of shape [", (long long) n1_25949, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin_old.fut:848:4-854:44\n   #3  test_join2.fut:16:1-29:36\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                int64_t bytes_42049 = (int64_t) 8 * m_32434;
                
                if (mem_42050_cached_sizze_42765 < bytes_42049) {
                    err = lexical_realloc(ctx, &mem_42050, &mem_42050_cached_sizze_42765, bytes_42049);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                lmad_copy_8b(ctx, 1, (uint64_t *) mem_42050, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_42002, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_32434});
                
                int64_t max_mult_32458;
                
                if (cond_32457) {
                    if (mem_42052_cached_sizze_42766 < bytes_41993) {
                        err = lexical_realloc(ctx, &mem_42052, &mem_42052_cached_sizze_42766, bytes_41993);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    for (int64_t i_41926 = 0; i_41926 < n1_25949; i_41926++) {
                        int64_t eta_p_41661 = ((int64_t *) mem_41998)[i_41926];
                        int64_t eta_p_41662 = ((int64_t *) mem_41994)[i_41926];
                        bool cond_41663 = eta_p_41661 == (int64_t) 1;
                        int64_t lifted_lambda_res_41664;
                        
                        if (cond_41663) {
                            int64_t lifted_lambda_res_t_res_41665 = sub64(eta_p_41662, (int64_t) 1);
                            
                            lifted_lambda_res_41664 = lifted_lambda_res_t_res_41665;
                        } else {
                            lifted_lambda_res_41664 = (int64_t) -1;
                        }
                        ((int64_t *) mem_42052)[i_41926] = lifted_lambda_res_41664;
                    }
                    for (int64_t write_iter_41928 = 0; write_iter_41928 < n1_25949; write_iter_41928++) {
                        int64_t write_iv_41930 = ((int64_t *) mem_42052)[write_iter_41928];
                        int64_t write_iv_41931 = ((int64_t *) mem_42002)[write_iter_41928];
                        
                        if (sle64((int64_t) 0, write_iv_41930) && slt64(write_iv_41930, m_32434)) {
                            ((int64_t *) mem_42050)[write_iv_41930] = write_iv_41931;
                        }
                    }
                    
                    int64_t defunc_0_reduce_res_41681;
                    int64_t redout_41933 = (int64_t) -9223372036854775808;
                    
                    for (int64_t i_41934 = 0; i_41934 < m_32434; i_41934++) {
                        int64_t x_41682 = ((int64_t *) mem_42050)[i_41934];
                        int64_t max_res_41685 = smax64(x_41682, redout_41933);
                        int64_t redout_tmp_42737 = max_res_41685;
                        
                        redout_41933 = redout_tmp_42737;
                    }
                    defunc_0_reduce_res_41681 = redout_41933;
                    max_mult_32458 = defunc_0_reduce_res_41681;
                } else {
                    max_mult_32458 = (int64_t) 1;
                }
                
                int64_t bytes_42065 = (int64_t) 8 * defunc_res_41700;
                bool bounds_invalid_upwards_32469 = slt64(max_mult_32458, (int64_t) 1);
                bool valid_32471 = !bounds_invalid_upwards_32469;
                bool range_valid_c_32472;
                
                if (!valid_32471) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 1, "..<", (long long) max_mult_32458, " is invalid.", "-> #0  ../../../joins/ftHashJoin_old.fut:819:18-30\n   #1  ../../../joins/ftHashJoin_old.fut:848:4-854:44\n   #2  test_join2.fut:16:1-29:36\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                int64_t bytes_42103 = b_25950 * defunc_res_41700;
                
                if (mem_42042_cached_sizze_42764 < bytes_41993) {
                    err = lexical_realloc(ctx, &mem_42042, &mem_42042_cached_sizze_42764, bytes_41993);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                for (int64_t i_41922 = 0; i_41922 < n1_25949; i_41922++) {
                    int64_t eta_p_37269 = ((int64_t *) mem_42002)[i_41922];
                    int64_t zv_lhs_37271 = add64((int64_t) -1, i_41922);
                    int64_t tmp_37272 = smod64(zv_lhs_37271, n1_25949);
                    int64_t lifted_lambda_res_37273 = ((int64_t *) mem_41996)[tmp_37272];
                    bool cond_37275 = i_41922 == (int64_t) 0;
                    int64_t lifted_lambda_res_37276;
                    
                    if (cond_37275) {
                        lifted_lambda_res_37276 = (int64_t) 0;
                    } else {
                        lifted_lambda_res_37276 = lifted_lambda_res_37273;
                    }
                    
                    bool cond_37278 = slt64((int64_t) 0, eta_p_37269);
                    int64_t lifted_lambda_res_37279;
                    
                    if (cond_37278) {
                        lifted_lambda_res_37279 = lifted_lambda_res_37276;
                    } else {
                        lifted_lambda_res_37279 = (int64_t) -1;
                    }
                    ((int64_t *) mem_42042)[i_41922] = lifted_lambda_res_37279;
                }
                if (memblock_alloc(ctx, &mem_42066, bytes_42065, "mem_42066")) {
                    err = 1;
                    goto cleanup;
                }
                for (int64_t nest_i_42739 = 0; nest_i_42739 < defunc_res_41700; nest_i_42739++) {
                    ((int64_t *) mem_42066.mem)[nest_i_42739] = (int64_t) 0;
                }
                for (int64_t write_iter_41935 = 0; write_iter_41935 < n1_25949; write_iter_41935++) {
                    int64_t write_iv_41937 = ((int64_t *) mem_42042)[write_iter_41935];
                    
                    if (sle64((int64_t) 0, write_iv_41937) && slt64(write_iv_41937, defunc_res_41700)) {
                        ((int64_t *) mem_42066.mem)[write_iv_41937] = write_iter_41935;
                    }
                }
                if (memblock_alloc(ctx, &mem_42074, bytes_42065, "mem_42074")) {
                    err = 1;
                    goto cleanup;
                }
                for (int64_t nest_i_42741 = 0; nest_i_42741 < defunc_res_41700; nest_i_42741++) {
                    ((int64_t *) mem_42074.mem)[nest_i_42741] = (int64_t) 1;
                }
                
                int64_t distance_32470 = sub64(max_mult_32458, (int64_t) 1);
                
                if (memblock_set(ctx, &mem_param_42077, &mem_42074, "mem_42074") != 0)
                    return 1;
                if (memblock_set(ctx, &mem_param_42080, &mem_42066, "mem_42066") != 0)
                    return 1;
                for (int64_t i_32475 = 0; i_32475 < distance_32470; i_32475++) {
                    int64_t index_primexp_32478 = add64((int64_t) 1, i_32475);
                    
                    if (memblock_alloc(ctx, &mem_42082, bytes_42065, "mem_42082")) {
                        err = 1;
                        goto cleanup;
                    }
                    lmad_copy_8b(ctx, 1, (uint64_t *) mem_42082.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_42080.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {defunc_res_41700});
                    if (memblock_alloc(ctx, &mem_42084, bytes_42065, "mem_42084")) {
                        err = 1;
                        goto cleanup;
                    }
                    lmad_copy_8b(ctx, 1, (uint64_t *) mem_42084.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_42077.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {defunc_res_41700});
                    
                    int64_t replicate_arg1_32490 = add64((int64_t) 2, i_32475);
                    
                    for (int64_t write_iter_41940 = 0; write_iter_41940 < n1_25949; write_iter_41940++) {
                        int64_t zlze_lhs_38266 = ((int64_t *) mem_42002)[write_iter_41940];
                        bool cond_38267 = sle64(zlze_lhs_38266, index_primexp_32478);
                        int64_t lifted_lambda_res_38268;
                        
                        if (cond_38267) {
                            lifted_lambda_res_38268 = (int64_t) -1;
                        } else {
                            int64_t zp_lhs_38269 = ((int64_t *) mem_42042)[write_iter_41940];
                            int64_t lifted_lambda_res_f_res_38270 = add64(index_primexp_32478, zp_lhs_38269);
                            
                            lifted_lambda_res_38268 = lifted_lambda_res_f_res_38270;
                        }
                        if (sle64((int64_t) 0, lifted_lambda_res_38268) && slt64(lifted_lambda_res_38268, defunc_res_41700)) {
                            ((int64_t *) mem_42084.mem)[lifted_lambda_res_38268] = replicate_arg1_32490;
                        }
                        if (sle64((int64_t) 0, lifted_lambda_res_38268) && slt64(lifted_lambda_res_38268, defunc_res_41700)) {
                            ((int64_t *) mem_42082.mem)[lifted_lambda_res_38268] = write_iter_41940;
                        }
                    }
                    if (memblock_set(ctx, &mem_param_tmp_42742, &mem_42084, "mem_42084") != 0)
                        return 1;
                    if (memblock_set(ctx, &mem_param_tmp_42743, &mem_42082, "mem_42082") != 0)
                        return 1;
                    if (memblock_set(ctx, &mem_param_42077, &mem_param_tmp_42742, "mem_param_tmp_42742") != 0)
                        return 1;
                    if (memblock_set(ctx, &mem_param_42080, &mem_param_tmp_42743, "mem_param_tmp_42743") != 0)
                        return 1;
                }
                if (memblock_set(ctx, &ext_mem_42102, &mem_param_42077, "mem_param_42077") != 0)
                    return 1;
                if (memblock_set(ctx, &ext_mem_42101, &mem_param_42080, "mem_param_42080") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_42066, "mem_42066") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_42074, "mem_42074") != 0)
                    return 1;
                if (memblock_alloc(ctx, &mem_42104, bytes_42103, "mem_42104")) {
                    err = 1;
                    goto cleanup;
                }
                if (memblock_alloc(ctx, &mem_42106, bytes_42065, "mem_42106")) {
                    err = 1;
                    goto cleanup;
                }
                for (int64_t i_41950 = 0; i_41950 < defunc_res_41700; i_41950++) {
                    int64_t eta_p_38166 = ((int64_t *) ext_mem_42101.mem)[i_41950];
                    bool x_38168 = sle64((int64_t) 0, eta_p_38166);
                    bool y_38169 = slt64(eta_p_38166, n1_25949);
                    bool bounds_check_38170 = x_38168 && y_38169;
                    bool index_certs_38171;
                    
                    if (!bounds_check_38170) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_38166, "] out of bounds for array of shape [", (long long) n1_25949, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:829:16-26\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin_old.fut:828:8-836:6\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-854:44\n   #4  test_join2.fut:16:1-29:36\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t eta_p_38165 = ((int64_t *) ext_mem_42102.mem)[i_41950];
                    int64_t rv_38172 = ((int64_t *) mem_42004)[eta_p_38166];
                    int64_t inf_s_idx_38173 = ((int64_t *) mem_42000)[eta_p_38166];
                    int64_t tmp_38174 = sub64(inf_s_idx_38173, (int64_t) 1);
                    bool loop_cond_38175 = slt64((int64_t) 0, eta_p_38165);
                    bool kth_match_38176;
                    int64_t kth_match_38177;
                    int64_t kth_match_38178;
                    bool loop_while_38179;
                    int64_t j_38180;
                    int64_t m_38181;
                    
                    loop_while_38179 = loop_cond_38175;
                    j_38180 = tmp_38174;
                    m_38181 = (int64_t) 0;
                    while (loop_while_38179) {
                        int64_t zeze_rhs_38182 = add64((int64_t) 1, j_38180);
                        bool x_38183 = sle64((int64_t) 0, zeze_rhs_38182);
                        bool y_38184 = slt64(zeze_rhs_38182, n2_25952);
                        bool bounds_check_38185 = x_38183 && y_38184;
                        bool index_certs_38186;
                        
                        if (!bounds_check_38185) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) zeze_rhs_38182, "] out of bounds for array of shape [", (long long) n2_25952, "].", "-> #0  ../../../joins/ftHashJoin_old.fut:833:29-40\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftHashJoin_old.fut:828:8-836:6\n   #3  ../../../joins/ftHashJoin_old.fut:848:4-854:44\n   #4  test_join2.fut:16:1-29:36\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t zeze_rhs_38187 = ((int64_t *) mem_41986)[zeze_rhs_38182];
                        bool is_eq_38188 = rv_38172 == zeze_rhs_38187;
                        int64_t loopres_38189;
                        
                        if (is_eq_38188) {
                            int64_t tmp_41696 = add64((int64_t) 1, m_38181);
                            
                            loopres_38189 = tmp_41696;
                        } else {
                            loopres_38189 = m_38181;
                        }
                        
                        bool loop_cond_38191 = slt64(loopres_38189, eta_p_38165);
                        bool loop_while_tmp_42750 = loop_cond_38191;
                        int64_t j_tmp_42751 = zeze_rhs_38182;
                        int64_t m_tmp_42752 = loopres_38189;
                        
                        loop_while_38179 = loop_while_tmp_42750;
                        j_38180 = j_tmp_42751;
                        m_38181 = m_tmp_42752;
                    }
                    kth_match_38176 = loop_while_38179;
                    kth_match_38177 = j_38180;
                    kth_match_38178 = m_38181;
                    lmad_copy_1b(ctx, 1, (uint8_t *) mem_42104.mem, i_41950 * b_25950, (int64_t []) {(int64_t) 1}, (uint8_t *) ks1_mem_41977.mem, eta_p_38166 * b_25950, (int64_t []) {(int64_t) 1}, (int64_t []) {b_25950});
                    ((int64_t *) mem_42106.mem)[i_41950] = kth_match_38177;
                }
                if (memblock_unref(ctx, &ext_mem_42102, "ext_mem_42102") != 0)
                    return 1;
                if (memblock_alloc(ctx, &mem_42122, bytes_42065, "mem_42122")) {
                    err = 1;
                    goto cleanup;
                }
                lmad_copy_8b(ctx, 1, (uint64_t *) mem_42122.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_42101.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {defunc_res_41700});
                if (memblock_unref(ctx, &ext_mem_42101, "ext_mem_42101") != 0)
                    return 1;
                if (memblock_set(ctx, &ext_mem_42268, &mem_42122, "mem_42122") != 0)
                    return 1;
                if (memblock_set(ctx, &ext_mem_42267, &mem_42106, "mem_42106") != 0)
                    return 1;
                if (memblock_set(ctx, &ext_mem_42264, &mem_42104, "mem_42104") != 0)
                    return 1;
                radix_hash_join_res_f_res_f_res_31827 = defunc_res_41700;
            }
            if (memblock_set(ctx, &ext_mem_42414, &ext_mem_42268, "ext_mem_42268") != 0)
                return 1;
            if (memblock_set(ctx, &ext_mem_42413, &ext_mem_42267, "ext_mem_42267") != 0)
                return 1;
            if (memblock_set(ctx, &ext_mem_42410, &ext_mem_42264, "ext_mem_42264") != 0)
                return 1;
            radix_hash_join_res_f_res_31468 = radix_hash_join_res_f_res_f_res_31827;
            radix_hash_join_res_f_res_31469 = radix_hash_join_res_f_res_f_res_31827;
            radix_hash_join_res_f_res_31470 = radix_hash_join_res_f_res_f_res_31827;
        }
        if (memblock_set(ctx, &ext_mem_42558, &ext_mem_42414, "ext_mem_42414") != 0)
            return 1;
        if (memblock_set(ctx, &ext_mem_42557, &ext_mem_42413, "ext_mem_42413") != 0)
            return 1;
        if (memblock_set(ctx, &ext_mem_42554, &ext_mem_42410, "ext_mem_42410") != 0)
            return 1;
        radix_hash_join_res_31109 = radix_hash_join_res_f_res_31468;
        radix_hash_join_res_31110 = radix_hash_join_res_f_res_31469;
        radix_hash_join_res_31111 = radix_hash_join_res_f_res_31470;
    }
    
    bool dim_match_32532 = radix_hash_join_res_31109 == radix_hash_join_res_31110;
    bool empty_or_match_cert_32533;
    
    if (!dim_match_32532) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "Function return value does not match shape of declared return type.", "-> #0  ../../../joins/ftHashJoin_old.fut:844:1-854:52\n   #1  test_join2.fut:16:1-29:36\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool dim_match_32535 = radix_hash_join_res_31109 == radix_hash_join_res_31111;
    bool empty_or_match_cert_32536;
    
    if (!dim_match_32535) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "Function return value does not match shape of declared return type.", "-> #0  ../../../joins/ftHashJoin_old.fut:844:1-854:52\n   #1  test_join2.fut:16:1-29:36\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    if (memblock_set(ctx, &mem_out_42565, &ext_mem_42554, "ext_mem_42554") != 0)
        return 1;
    if (memblock_set(ctx, &mem_out_42566, &ext_mem_42558, "ext_mem_42558") != 0)
        return 1;
    if (memblock_set(ctx, &mem_out_42567, &ext_mem_42557, "ext_mem_42557") != 0)
        return 1;
    prim_out_42568 = radix_hash_join_res_31109;
    if (memblock_set(ctx, &*mem_out_p_42753, &mem_out_42565, "mem_out_42565") != 0)
        return 1;
    if (memblock_set(ctx, &*mem_out_p_42754, &mem_out_42566, "mem_out_42566") != 0)
        return 1;
    if (memblock_set(ctx, &*mem_out_p_42755, &mem_out_42567, "mem_out_42567") != 0)
        return 1;
    *out_prim_out_42756 = prim_out_42568;
    
  cleanup:
    {
        free(mem_41986);
        free(mem_41994);
        free(mem_41996);
        free(mem_41998);
        free(mem_42000);
        free(mem_42002);
        free(mem_42004);
        free(mem_42042);
        free(mem_42050);
        free(mem_42052);
        free(mem_42124);
        free(mem_42132);
        free(mem_42134);
        free(mem_42136);
        free(mem_42138);
        free(mem_42140);
        free(mem_42142);
        free(mem_42180);
        free(mem_42188);
        free(mem_42190);
        free(mem_42270);
        free(mem_42278);
        free(mem_42280);
        free(mem_42282);
        free(mem_42284);
        free(mem_42286);
        free(mem_42288);
        free(mem_42326);
        free(mem_42334);
        free(mem_42336);
        free(mem_42415);
        free(mem_42423);
        free(mem_42425);
        free(mem_42427);
        free(mem_42429);
        free(mem_42431);
        free(mem_42432);
        free(mem_42470);
        free(mem_42478);
        free(mem_42480);
        if (memblock_unref(ctx, &mem_42122, "mem_42122") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_42106, "mem_42106") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_42104, "mem_42104") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_42743, "mem_param_tmp_42743") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_42742, "mem_param_tmp_42742") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_42084, "mem_42084") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_42082, "mem_42082") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_42080, "mem_param_42080") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_42077, "mem_param_42077") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_42101, "ext_mem_42101") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_42102, "ext_mem_42102") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_42074, "mem_42074") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_42066, "mem_42066") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_42260, "mem_42260") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_42244, "mem_42244") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_42242, "mem_42242") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_42697, "mem_param_tmp_42697") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_42696, "mem_param_tmp_42696") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_42222, "mem_42222") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_42220, "mem_42220") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_42218, "mem_param_42218") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_42215, "mem_param_42215") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_42239, "ext_mem_42239") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_42240, "ext_mem_42240") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_42212, "mem_42212") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_42204, "mem_42204") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_42264, "ext_mem_42264") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_42267, "ext_mem_42267") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_42268, "ext_mem_42268") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_42406, "mem_42406") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_42390, "mem_42390") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_42388, "mem_42388") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_42651, "mem_param_tmp_42651") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_42650, "mem_param_tmp_42650") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_42368, "mem_42368") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_42366, "mem_42366") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_42364, "mem_param_42364") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_42361, "mem_param_42361") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_42385, "ext_mem_42385") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_42386, "ext_mem_42386") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_42358, "mem_42358") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_42350, "mem_42350") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_42410, "ext_mem_42410") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_42413, "ext_mem_42413") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_42414, "ext_mem_42414") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_42550, "mem_42550") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_42534, "mem_42534") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_42532, "mem_42532") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_42605, "mem_param_tmp_42605") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_42604, "mem_param_tmp_42604") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_42512, "mem_42512") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_42510, "mem_42510") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_42508, "mem_param_42508") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_42505, "mem_param_42505") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_42529, "ext_mem_42529") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_42530, "ext_mem_42530") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_42502, "mem_42502") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_42494, "mem_42494") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_42554, "ext_mem_42554") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_42557, "ext_mem_42557") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_42558, "ext_mem_42558") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_42567, "mem_out_42567") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_42566, "mem_out_42566") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_42565, "mem_out_42565") != 0)
            return 1;
    }
    return err;
}

int futhark_entry_rhj(struct futhark_context *ctx, struct futhark_u8_2d **out0, struct futhark_i64_1d **out1, struct futhark_i64_1d **out2, const struct futhark_u8_2d *in0, const struct futhark_u8_2d *in1, const struct futhark_u8_2d *in2, const struct futhark_u8_2d *in3, const int32_t in4, const struct futhark_i64_1d *in5, const struct futhark_i32_1d *in6, const struct futhark_i64_1d *in7, const struct futhark_i64_1d *in8)
{
    int64_t n1_25949 = (int64_t) 0;
    int64_t b_25950 = (int64_t) 0;
    int64_t dz2080U_25951 = (int64_t) 0;
    int64_t n2_25952 = (int64_t) 0;
    int64_t dz2081U_25953 = (int64_t) 0;
    int64_t dz2083U_25954 = (int64_t) 0;
    int64_t coercez2081Uz2088U_25955 = (int64_t) 0;
    int64_t coercez2082Uz2081U_25956 = (int64_t) 0;
    int32_t maxDepth2_25961 = 0;
    int64_t prim_out_42568 = (int64_t) 0;
    int ret = 0;
    
    lock_lock(&ctx->lock);
    
    struct memblock mem_out_42567;
    
    mem_out_42567.references = NULL;
    
    struct memblock mem_out_42566;
    
    mem_out_42566.references = NULL;
    
    struct memblock mem_out_42565;
    
    mem_out_42565.references = NULL;
    
    struct memblock last_idx2_mem_41984;
    
    last_idx2_mem_41984.references = NULL;
    
    struct memblock first_idx2_mem_41983;
    
    first_idx2_mem_41983.references = NULL;
    
    struct memblock depths2_mem_41982;
    
    depths2_mem_41982.references = NULL;
    
    struct memblock bounds2_mem_41981;
    
    bounds2_mem_41981.references = NULL;
    
    struct memblock nameless_mem_41980;
    
    nameless_mem_41980.references = NULL;
    
    struct memblock ks2_mem_41979;
    
    ks2_mem_41979.references = NULL;
    
    struct memblock nameless_mem_41978;
    
    nameless_mem_41978.references = NULL;
    
    struct memblock ks1_mem_41977;
    
    ks1_mem_41977.references = NULL;
    ks1_mem_41977 = in0->mem;
    n1_25949 = in0->shape[0];
    b_25950 = in0->shape[1];
    nameless_mem_41978 = in1->mem;
    n1_25949 = in1->shape[0];
    dz2080U_25951 = in1->shape[1];
    ks2_mem_41979 = in2->mem;
    n2_25952 = in2->shape[0];
    b_25950 = in2->shape[1];
    nameless_mem_41980 = in3->mem;
    n2_25952 = in3->shape[0];
    dz2081U_25953 = in3->shape[1];
    maxDepth2_25961 = in4;
    bounds2_mem_41981 = in5->mem;
    dz2083U_25954 = in5->shape[0];
    depths2_mem_41982 = in6->mem;
    dz2083U_25954 = in6->shape[0];
    first_idx2_mem_41983 = in7->mem;
    coercez2081Uz2088U_25955 = in7->shape[0];
    last_idx2_mem_41984 = in8->mem;
    coercez2082Uz2081U_25956 = in8->shape[0];
    if (!((n1_25949 == in0->shape[0] && b_25950 == in0->shape[1]) && ((n1_25949 == in1->shape[0] && dz2080U_25951 == in1->shape[1]) && ((n2_25952 == in2->shape[0] && b_25950 == in2->shape[1]) && ((n2_25952 == in3->shape[0] && dz2081U_25953 == in3->shape[1]) && (dz2083U_25954 == in5->shape[0] && (dz2083U_25954 == in6->shape[0] && (coercez2081Uz2088U_25955 == in7->shape[0] && coercez2082Uz2081U_25956 == in8->shape[0])))))))) {
        ret = 1;
        set_error(ctx, msgprintf("Error: entry point arguments have invalid sizes.\n"));
    }
    if (ret == 0) {
        ret = futrts_entry_rhj(ctx, &mem_out_42565, &mem_out_42566, &mem_out_42567, &prim_out_42568, ks1_mem_41977, nameless_mem_41978, ks2_mem_41979, nameless_mem_41980, bounds2_mem_41981, depths2_mem_41982, first_idx2_mem_41983, last_idx2_mem_41984, n1_25949, b_25950, dz2080U_25951, n2_25952, dz2081U_25953, dz2083U_25954, coercez2081Uz2088U_25955, coercez2082Uz2081U_25956, maxDepth2_25961);
        if (ret == 0) {
            assert((*out0 = (struct futhark_u8_2d *) malloc(sizeof(struct futhark_u8_2d))) != NULL);
            (*out0)->mem = mem_out_42565;
            (*out0)->shape[0] = prim_out_42568;
            (*out0)->shape[1] = b_25950;
            assert((*out1 = (struct futhark_i64_1d *) malloc(sizeof(struct futhark_i64_1d))) != NULL);
            (*out1)->mem = mem_out_42566;
            (*out1)->shape[0] = prim_out_42568;
            assert((*out2 = (struct futhark_i64_1d *) malloc(sizeof(struct futhark_i64_1d))) != NULL);
            (*out2)->mem = mem_out_42567;
            (*out2)->shape[0] = prim_out_42568;
        }
    }
    lock_unlock(&ctx->lock);
    return ret;
}
  
