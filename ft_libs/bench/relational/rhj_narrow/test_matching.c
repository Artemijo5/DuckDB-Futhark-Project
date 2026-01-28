
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
int futhark_entry_smj1_i32(struct futhark_context *ctx, struct futhark_i32_1d **out0, struct futhark_i64_1d **out1, struct futhark_i64_1d **out2, struct futhark_i64_1d **out3, const struct futhark_i32_1d *in0, const struct futhark_u8_2d *in1, const struct futhark_i32_1d *in2, const struct futhark_u8_2d *in3);
int futhark_entry_smj1_i64(struct futhark_context *ctx, struct futhark_i64_1d **out0, struct futhark_i64_1d **out1, struct futhark_i64_1d **out2, struct futhark_i64_1d **out3, const struct futhark_i64_1d *in0, const struct futhark_u8_2d *in1, const struct futhark_i64_1d *in2, const struct futhark_u8_2d *in3);

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
const struct type *smj1_i32_out_types[] = {&type_ZMZNi32, &type_ZMZNi64, &type_ZMZNi64, &type_ZMZNi64, NULL};
bool smj1_i32_out_unique[] = {false, false, false, false};
const struct type *smj1_i32_in_types[] = {&type_ZMZNi32, &type_ZMZNZMZNu8, &type_ZMZNi32, &type_ZMZNZMZNu8, NULL};
bool smj1_i32_in_unique[] = {false, false, false, false};
const char *smj1_i32_tuning_params[] = {NULL};
int call_smj1_i32(struct futhark_context *ctx, void **outs, void **ins)
{
    struct futhark_i32_1d * *out0 = outs[0];
    struct futhark_i64_1d * *out1 = outs[1];
    struct futhark_i64_1d * *out2 = outs[2];
    struct futhark_i64_1d * *out3 = outs[3];
    struct futhark_i32_1d * in0 = *(struct futhark_i32_1d * *) ins[0];
    struct futhark_u8_2d * in1 = *(struct futhark_u8_2d * *) ins[1];
    struct futhark_i32_1d * in2 = *(struct futhark_i32_1d * *) ins[2];
    struct futhark_u8_2d * in3 = *(struct futhark_u8_2d * *) ins[3];
    
    return futhark_entry_smj1_i32(ctx, out0, out1, out2, out3, in0, in1, in2, in3);
}
const struct type *smj1_i64_out_types[] = {&type_ZMZNi64, &type_ZMZNi64, &type_ZMZNi64, &type_ZMZNi64, NULL};
bool smj1_i64_out_unique[] = {false, false, false, false};
const struct type *smj1_i64_in_types[] = {&type_ZMZNi64, &type_ZMZNZMZNu8, &type_ZMZNi64, &type_ZMZNZMZNu8, NULL};
bool smj1_i64_in_unique[] = {false, false, false, false};
const char *smj1_i64_tuning_params[] = {NULL};
int call_smj1_i64(struct futhark_context *ctx, void **outs, void **ins)
{
    struct futhark_i64_1d * *out0 = outs[0];
    struct futhark_i64_1d * *out1 = outs[1];
    struct futhark_i64_1d * *out2 = outs[2];
    struct futhark_i64_1d * *out3 = outs[3];
    struct futhark_i64_1d * in0 = *(struct futhark_i64_1d * *) ins[0];
    struct futhark_u8_2d * in1 = *(struct futhark_u8_2d * *) ins[1];
    struct futhark_i64_1d * in2 = *(struct futhark_i64_1d * *) ins[2];
    struct futhark_u8_2d * in3 = *(struct futhark_u8_2d * *) ins[3];
    
    return futhark_entry_smj1_i64(ctx, out0, out1, out2, out3, in0, in1, in2, in3);
}
const struct type *types[] = {&type_i8, &type_i16, &type_i32, &type_i64, &type_u8, &type_u16, &type_u32, &type_u64, &type_f16, &type_f32, &type_f64, &type_bool, &type_ZMZNZMZNu8, &type_ZMZNi32, &type_ZMZNi64, NULL};
struct entry_point entry_points[] = {{.name ="smj1_i32", .f =call_smj1_i32, .tuning_params =smj1_i32_tuning_params, .in_types =smj1_i32_in_types, .out_types =smj1_i32_out_types, .in_unique =smj1_i32_in_unique, .out_unique =smj1_i32_out_unique}, {.name ="smj1_i64", .f =call_smj1_i64, .tuning_params =smj1_i64_tuning_params, .in_types =smj1_i64_in_types, .out_types =smj1_i64_out_types, .in_unique =smj1_i64_in_unique, .out_unique =smj1_i64_out_unique}, {.name =NULL}};
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

FUTHARK_FUN_ATTR int futrts_entry_smj1_i32(struct futhark_context *ctx, struct memblock *mem_out_p_40333, struct memblock *mem_out_p_40334, struct memblock *mem_out_p_40335, struct memblock *mem_out_p_40336, struct memblock ks1_mem_39592, struct memblock pL1_mem_39593, struct memblock ks2_mem_39594, struct memblock pL2_mem_39595, int64_t n1_24948, int64_t n2_24949);
FUTHARK_FUN_ATTR int futrts_entry_smj1_i64(struct futhark_context *ctx, struct memblock *mem_out_p_40359, struct memblock *mem_out_p_40360, struct memblock *mem_out_p_40361, struct memblock *mem_out_p_40362, struct memblock ks1_mem_39592, struct memblock pL1_mem_39593, struct memblock ks2_mem_39594, struct memblock pL2_mem_39595, int64_t n1_30936, int64_t n2_30937);
FUTHARK_FUN_ATTR int futrts_greatest_divisor_leq_than_5346(struct futhark_context *ctx, int64_t *out_prim_out_40385, int64_t upper_bound_18796, int64_t n_18797);
FUTHARK_FUN_ATTR int futrts_indicesWithIncrement_7869(struct futhark_context *ctx, struct memblock *mem_out_p_40386, struct memblock nameless_mem_39592, int64_t n_19155, int64_t incr_19156);
FUTHARK_FUN_ATTR int futrts_indicesWithIncrement_7982(struct futhark_context *ctx, struct memblock *mem_out_p_40387, struct memblock nameless_mem_39592, int64_t n_25215, int64_t incr_25216);
FUTHARK_FUN_ATTR int futrts_lifted_normalizze_7743(struct futhark_context *ctx, int64_t *out_prim_out_40388, int64_t *out_prim_out_40389, int64_t *out_prim_out_40390, int64_t *out_prim_out_40391, int64_t *out_prim_out_40392, int64_t slo_19029, int64_t shi_19030, int64_t tlo_19031, int64_t thi_19032, int64_t count_19033);

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

FUTHARK_FUN_ATTR int futrts_entry_smj1_i32(struct futhark_context *ctx, struct memblock *mem_out_p_40333, struct memblock *mem_out_p_40334, struct memblock *mem_out_p_40335, struct memblock *mem_out_p_40336, struct memblock ks1_mem_39592, struct memblock pL1_mem_39593, struct memblock ks2_mem_39594, struct memblock pL2_mem_39595, int64_t n1_24948, int64_t n2_24949)
{
    (void) ctx;
    
    int err = 0;
    int64_t mem_39600_cached_sizze_40337 = 0;
    unsigned char *mem_39600 = NULL;
    int64_t mem_39603_cached_sizze_40338 = 0;
    unsigned char *mem_39603 = NULL;
    int64_t mem_39613_cached_sizze_40339 = 0;
    unsigned char *mem_39613 = NULL;
    int64_t mem_39615_cached_sizze_40340 = 0;
    unsigned char *mem_39615 = NULL;
    int64_t mem_39682_cached_sizze_40341 = 0;
    unsigned char *mem_39682 = NULL;
    int64_t mem_39685_cached_sizze_40342 = 0;
    unsigned char *mem_39685 = NULL;
    int64_t mem_39695_cached_sizze_40343 = 0;
    unsigned char *mem_39695 = NULL;
    int64_t mem_39697_cached_sizze_40344 = 0;
    unsigned char *mem_39697 = NULL;
    int64_t mem_39712_cached_sizze_40345 = 0;
    unsigned char *mem_39712 = NULL;
    int64_t mem_39715_cached_sizze_40346 = 0;
    unsigned char *mem_39715 = NULL;
    int64_t mem_39804_cached_sizze_40347 = 0;
    unsigned char *mem_39804 = NULL;
    int64_t mem_39817_cached_sizze_40348 = 0;
    unsigned char *mem_39817 = NULL;
    int64_t mem_39820_cached_sizze_40349 = 0;
    unsigned char *mem_39820 = NULL;
    int64_t mem_39830_cached_sizze_40350 = 0;
    unsigned char *mem_39830 = NULL;
    int64_t mem_39832_cached_sizze_40351 = 0;
    unsigned char *mem_39832 = NULL;
    int64_t mem_39899_cached_sizze_40352 = 0;
    unsigned char *mem_39899 = NULL;
    int64_t mem_39902_cached_sizze_40353 = 0;
    unsigned char *mem_39902 = NULL;
    int64_t mem_39912_cached_sizze_40354 = 0;
    unsigned char *mem_39912 = NULL;
    int64_t mem_39914_cached_sizze_40355 = 0;
    unsigned char *mem_39914 = NULL;
    int64_t mem_39929_cached_sizze_40356 = 0;
    unsigned char *mem_39929 = NULL;
    int64_t mem_39932_cached_sizze_40357 = 0;
    unsigned char *mem_39932 = NULL;
    int64_t mem_40021_cached_sizze_40358 = 0;
    unsigned char *mem_40021 = NULL;
    struct memblock mem_param_tmp_40304;
    
    mem_param_tmp_40304.references = NULL;
    
    struct memblock mem_param_tmp_40303;
    
    mem_param_tmp_40303.references = NULL;
    
    struct memblock mem_40116;
    
    mem_40116.references = NULL;
    
    struct memblock mem_40114;
    
    mem_40114.references = NULL;
    
    struct memblock mem_param_tmp_40313;
    
    mem_param_tmp_40313.references = NULL;
    
    struct memblock mem_param_tmp_40312;
    
    mem_param_tmp_40312.references = NULL;
    
    struct memblock mem_40082;
    
    mem_40082.references = NULL;
    
    struct memblock mem_40080;
    
    mem_40080.references = NULL;
    
    struct memblock mem_param_tmp_40327;
    
    mem_param_tmp_40327.references = NULL;
    
    struct memblock mem_40069;
    
    mem_40069.references = NULL;
    
    struct memblock mem_param_40067;
    
    mem_param_40067.references = NULL;
    
    struct memblock ext_mem_40078;
    
    ext_mem_40078.references = NULL;
    
    struct memblock mem_param_tmp_40323;
    
    mem_param_tmp_40323.references = NULL;
    
    struct memblock mem_40055;
    
    mem_40055.references = NULL;
    
    struct memblock mem_param_40053;
    
    mem_param_40053.references = NULL;
    
    struct memblock ext_mem_40064;
    
    ext_mem_40064.references = NULL;
    
    struct memblock ext_mem_40097;
    
    ext_mem_40097.references = NULL;
    
    struct memblock ext_mem_40100;
    
    ext_mem_40100.references = NULL;
    
    struct memblock ext_mem_40103;
    
    ext_mem_40103.references = NULL;
    
    struct memblock ext_mem_40106;
    
    ext_mem_40106.references = NULL;
    
    struct memblock mem_param_40050;
    
    mem_param_40050.references = NULL;
    
    struct memblock mem_param_40047;
    
    mem_param_40047.references = NULL;
    
    struct memblock ext_mem_40111;
    
    ext_mem_40111.references = NULL;
    
    struct memblock ext_mem_40112;
    
    ext_mem_40112.references = NULL;
    
    struct memblock mem_40044;
    
    mem_40044.references = NULL;
    
    struct memblock mem_40042;
    
    mem_40042.references = NULL;
    
    struct memblock mem_param_40040;
    
    mem_param_40040.references = NULL;
    
    struct memblock mem_param_40037;
    
    mem_param_40037.references = NULL;
    
    struct memblock ext_mem_40121;
    
    ext_mem_40121.references = NULL;
    
    struct memblock ext_mem_40122;
    
    ext_mem_40122.references = NULL;
    
    struct memblock mem_40034;
    
    mem_40034.references = NULL;
    
    struct memblock mem_40032;
    
    mem_40032.references = NULL;
    
    struct memblock ext_mem_40030;
    
    ext_mem_40030.references = NULL;
    
    struct memblock mem_40029;
    
    mem_40029.references = NULL;
    
    struct memblock mem_param_tmp_40272;
    
    mem_param_tmp_40272.references = NULL;
    
    struct memblock mem_param_tmp_40271;
    
    mem_param_tmp_40271.references = NULL;
    
    struct memblock mem_40009;
    
    mem_40009.references = NULL;
    
    struct memblock mem_40005;
    
    mem_40005.references = NULL;
    
    struct memblock mem_39971;
    
    mem_39971.references = NULL;
    
    struct memblock mem_39967;
    
    mem_39967.references = NULL;
    
    struct memblock mem_39976;
    
    mem_39976.references = NULL;
    
    struct memblock mem_39974;
    
    mem_39974.references = NULL;
    
    struct memblock ext_mem_39991;
    
    ext_mem_39991.references = NULL;
    
    struct memblock ext_mem_39994;
    
    ext_mem_39994.references = NULL;
    
    struct memblock mem_param_39896;
    
    mem_param_39896.references = NULL;
    
    struct memblock mem_param_39893;
    
    mem_param_39893.references = NULL;
    
    struct memblock ext_mem_40015;
    
    ext_mem_40015.references = NULL;
    
    struct memblock ext_mem_40016;
    
    ext_mem_40016.references = NULL;
    
    struct memblock mem_39889;
    
    mem_39889.references = NULL;
    
    struct memblock mem_39885;
    
    mem_39885.references = NULL;
    
    struct memblock ext_mem_40019;
    
    ext_mem_40019.references = NULL;
    
    struct memblock mem_39814;
    
    mem_39814.references = NULL;
    
    struct memblock mem_39812;
    
    mem_39812.references = NULL;
    
    struct memblock mem_param_tmp_40228;
    
    mem_param_tmp_40228.references = NULL;
    
    struct memblock mem_param_tmp_40227;
    
    mem_param_tmp_40227.references = NULL;
    
    struct memblock mem_39792;
    
    mem_39792.references = NULL;
    
    struct memblock mem_39788;
    
    mem_39788.references = NULL;
    
    struct memblock mem_39754;
    
    mem_39754.references = NULL;
    
    struct memblock mem_39750;
    
    mem_39750.references = NULL;
    
    struct memblock mem_39759;
    
    mem_39759.references = NULL;
    
    struct memblock mem_39757;
    
    mem_39757.references = NULL;
    
    struct memblock ext_mem_39774;
    
    ext_mem_39774.references = NULL;
    
    struct memblock ext_mem_39777;
    
    ext_mem_39777.references = NULL;
    
    struct memblock mem_param_39679;
    
    mem_param_39679.references = NULL;
    
    struct memblock mem_param_39676;
    
    mem_param_39676.references = NULL;
    
    struct memblock ext_mem_39798;
    
    ext_mem_39798.references = NULL;
    
    struct memblock ext_mem_39799;
    
    ext_mem_39799.references = NULL;
    
    struct memblock mem_39672;
    
    mem_39672.references = NULL;
    
    struct memblock mem_39668;
    
    mem_39668.references = NULL;
    
    struct memblock ext_mem_39802;
    
    ext_mem_39802.references = NULL;
    
    struct memblock mem_39597;
    
    mem_39597.references = NULL;
    
    struct memblock mem_out_40208;
    
    mem_out_40208.references = NULL;
    
    struct memblock mem_out_40207;
    
    mem_out_40207.references = NULL;
    
    struct memblock mem_out_40206;
    
    mem_out_40206.references = NULL;
    
    struct memblock mem_out_40205;
    
    mem_out_40205.references = NULL;
    
    int64_t bytes_39596 = (int64_t) 8 * n1_24948;
    bool cond_37162 = sle64(n1_24948, (int64_t) 1);
    int64_t zs_lhs_37163 = sub64(n1_24948, (int64_t) 1);
    int64_t zp_rhs_37164 = sdiv_safe64(zs_lhs_37163, (int64_t) 20);
    int64_t min_num_blocks_37165 = add64((int64_t) 1, zp_rhs_37164);
    bool loop_cond_37166 = slt64((int64_t) 1, min_num_blocks_37165);
    bool smallest_pow_2_geq_than_res_37167;
    int64_t smallest_pow_2_geq_than_res_37168;
    bool loop_while_37169;
    int64_t x_37170;
    
    loop_while_37169 = loop_cond_37166;
    x_37170 = (int64_t) 1;
    while (loop_while_37169) {
        int64_t loopres_37171 = mul64((int64_t) 2, x_37170);
        bool loop_cond_37172 = slt64(loopres_37171, min_num_blocks_37165);
        bool loop_while_tmp_40209 = loop_cond_37172;
        int64_t x_tmp_40210 = loopres_37171;
        
        loop_while_37169 = loop_while_tmp_40209;
        x_37170 = x_tmp_40210;
    }
    smallest_pow_2_geq_than_res_37167 = loop_while_37169;
    smallest_pow_2_geq_than_res_37168 = x_37170;
    
    bool zzero_37173 = smallest_pow_2_geq_than_res_37168 == (int64_t) 0;
    bool nonzzero_37174 = !zzero_37173;
    bool protect_assert_disj_37175 = cond_37162 || nonzzero_37174;
    bool nonzzero_cert_37176;
    
    if (!protect_assert_disj_37175) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:234:30-42\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftSMJ.fut:104:60-63\n   #4  test_matching.fut:25:43-47\n   #5  test_matching.fut:19:1-28:44\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t zp_rhs_37177 = sdiv_safe64(zs_lhs_37163, smallest_pow_2_geq_than_res_37168);
    int64_t block_sizze_37178 = add64((int64_t) 1, zp_rhs_37177);
    bool y_37184 = slt64((int64_t) 0, n1_24948);
    bool protect_assert_disj_40166 = cond_37162 || y_37184;
    bool index_certs_37185;
    
    if (!protect_assert_disj_40166) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) n1_24948, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:240:60-64\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftSMJ.fut:104:60-63\n   #4  test_matching.fut:25:43-47\n   #5  test_matching.fut:19:1-28:44\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool bounds_invalid_upwards_37199 = slt64(smallest_pow_2_geq_than_res_37168, (int64_t) 0);
    bool valid_37200 = !bounds_invalid_upwards_37199;
    bool protect_assert_disj_40174 = cond_37162 || valid_37200;
    bool range_valid_c_37201;
    
    if (!protect_assert_disj_40174) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) smallest_pow_2_geq_than_res_37168, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #3  /prelude/functional.fut:9:44-45\n   #4  ../../../joins/ftSMJ.fut:104:60-63\n   #5  test_matching.fut:25:43-47\n   #6  test_matching.fut:19:1-28:44\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool bounds_invalid_upwards_37203 = slt64(block_sizze_37178, (int64_t) 0);
    bool valid_37204 = !bounds_invalid_upwards_37203;
    bool protect_assert_disj_40176 = cond_37162 || valid_37204;
    bool range_valid_c_37205;
    
    if (!protect_assert_disj_40176) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) block_sizze_37178, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:25:43-47\n   #8  test_matching.fut:19:1-28:44\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool cond_37207 = sle64(block_sizze_37178, (int64_t) 1);
    int64_t tmp_37208 = sub64(block_sizze_37178, (int64_t) 2);
    bool bounds_invalid_upwards_37209 = slt64(tmp_37208, (int64_t) 0);
    bool valid_37211 = !bounds_invalid_upwards_37209;
    bool protect_assert_disj_37212 = cond_37207 || valid_37211;
    bool protect_assert_disj_40178 = cond_37162 || protect_assert_disj_37212;
    bool range_valid_c_37213;
    
    if (!protect_assert_disj_40178) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "...", (long long) tmp_37208, " is invalid.", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:166:19-28\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:25:43-47\n   #8  test_matching.fut:19:1-28:44\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t bytes_39803 = (int64_t) 4 * n1_24948;
    int64_t bytes_39813 = (int64_t) 8 * n2_24949;
    bool cond_37682 = sle64(n2_24949, (int64_t) 1);
    int64_t zs_lhs_37683 = sub64(n2_24949, (int64_t) 1);
    int64_t zp_rhs_37684 = sdiv_safe64(zs_lhs_37683, (int64_t) 20);
    int64_t min_num_blocks_37685 = add64((int64_t) 1, zp_rhs_37684);
    bool loop_cond_37686 = slt64((int64_t) 1, min_num_blocks_37685);
    bool smallest_pow_2_geq_than_res_37687;
    int64_t smallest_pow_2_geq_than_res_37688;
    bool loop_while_37689;
    int64_t x_37690;
    
    loop_while_37689 = loop_cond_37686;
    x_37690 = (int64_t) 1;
    while (loop_while_37689) {
        int64_t loopres_37691 = mul64((int64_t) 2, x_37690);
        bool loop_cond_37692 = slt64(loopres_37691, min_num_blocks_37685);
        bool loop_while_tmp_40211 = loop_cond_37692;
        int64_t x_tmp_40212 = loopres_37691;
        
        loop_while_37689 = loop_while_tmp_40211;
        x_37690 = x_tmp_40212;
    }
    smallest_pow_2_geq_than_res_37687 = loop_while_37689;
    smallest_pow_2_geq_than_res_37688 = x_37690;
    
    bool zzero_37693 = smallest_pow_2_geq_than_res_37688 == (int64_t) 0;
    bool nonzzero_37694 = !zzero_37693;
    bool protect_assert_disj_37695 = cond_37682 || nonzzero_37694;
    bool nonzzero_cert_37696;
    
    if (!protect_assert_disj_37695) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:234:30-42\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftSMJ.fut:104:60-63\n   #4  test_matching.fut:26:43-47\n   #5  test_matching.fut:19:1-28:44\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t zp_rhs_37697 = sdiv_safe64(zs_lhs_37683, smallest_pow_2_geq_than_res_37688);
    int64_t block_sizze_37698 = add64((int64_t) 1, zp_rhs_37697);
    bool y_37704 = slt64((int64_t) 0, n2_24949);
    bool protect_assert_disj_40140 = cond_37682 || y_37704;
    bool index_certs_37705;
    
    if (!protect_assert_disj_40140) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) n2_24949, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:240:60-64\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftSMJ.fut:104:60-63\n   #4  test_matching.fut:26:43-47\n   #5  test_matching.fut:19:1-28:44\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool bounds_invalid_upwards_37719 = slt64(smallest_pow_2_geq_than_res_37688, (int64_t) 0);
    bool valid_37720 = !bounds_invalid_upwards_37719;
    bool protect_assert_disj_40148 = cond_37682 || valid_37720;
    bool range_valid_c_37721;
    
    if (!protect_assert_disj_40148) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) smallest_pow_2_geq_than_res_37688, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #3  /prelude/functional.fut:9:44-45\n   #4  ../../../joins/ftSMJ.fut:104:60-63\n   #5  test_matching.fut:26:43-47\n   #6  test_matching.fut:19:1-28:44\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool bounds_invalid_upwards_37723 = slt64(block_sizze_37698, (int64_t) 0);
    bool valid_37724 = !bounds_invalid_upwards_37723;
    bool protect_assert_disj_40150 = cond_37682 || valid_37724;
    bool range_valid_c_37725;
    
    if (!protect_assert_disj_40150) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) block_sizze_37698, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:26:43-47\n   #8  test_matching.fut:19:1-28:44\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool cond_37727 = sle64(block_sizze_37698, (int64_t) 1);
    int64_t tmp_37728 = sub64(block_sizze_37698, (int64_t) 2);
    bool bounds_invalid_upwards_37729 = slt64(tmp_37728, (int64_t) 0);
    bool valid_37731 = !bounds_invalid_upwards_37729;
    bool protect_assert_disj_37732 = cond_37727 || valid_37731;
    bool protect_assert_disj_40152 = cond_37682 || protect_assert_disj_37732;
    bool range_valid_c_37733;
    
    if (!protect_assert_disj_40152) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "...", (long long) tmp_37728, " is invalid.", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:166:19-28\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:26:43-47\n   #8  test_matching.fut:19:1-28:44\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t bytes_40020 = (int64_t) 4 * n2_24949;
    
    if (memblock_alloc(ctx, &mem_39597, bytes_39596, "mem_39597")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t i_40213 = 0; i_40213 < n1_24948; i_40213++) {
        int64_t x_40214 = (int64_t) 0 + i_40213 * (int64_t) 1;
        
        ((int64_t *) mem_39597.mem)[i_40213] = x_40214;
    }
    
    int64_t flat_dim_37179 = smallest_pow_2_geq_than_res_37168 * block_sizze_37178;
    bool x_40161 = !cond_37162;
    int64_t greatest_divisor_leq_than_arg1_37182 = mul64((int64_t) 2, block_sizze_37178);
    int64_t merge_block_sizze_37183;
    
    if (x_40161) {
        int64_t x_40163;
        
        if (futrts_greatest_divisor_leq_than_5346(ctx, &x_40163, (int64_t) 8, greatest_divisor_leq_than_arg1_37182) != 0) {
            err = 1;
            goto cleanup;
        }
        merge_block_sizze_37183 = x_40163;
    } else {
        merge_block_sizze_37183 = (int64_t) 0;
    }
    
    int32_t reduce_arg1_37186;
    
    if (x_40161) {
        int32_t x_40167 = ((int32_t *) ks1_mem_39592.mem)[(int64_t) 0];
        
        reduce_arg1_37186 = x_40167;
    } else {
        reduce_arg1_37186 = 0;
    }
    
    int32_t defunc_0_reduce_res_39276;
    int64_t defunc_0_reduce_res_39277;
    
    if (x_40161) {
        int32_t x_40169;
        int64_t x_40170;
        int32_t redout_39467;
        int64_t redout_39468;
        
        redout_39467 = reduce_arg1_37186;
        redout_39468 = (int64_t) 0;
        for (int64_t i_39469 = 0; i_39469 < n1_24948; i_39469++) {
            int32_t x_37190 = ((int32_t *) ks1_mem_39592.mem)[i_39469];
            bool defunc_0_leq_res_37196 = sle32(redout_39467, x_37190);
            int32_t defunc_0_op_res_37197;
            
            if (defunc_0_leq_res_37196) {
                defunc_0_op_res_37197 = x_37190;
            } else {
                defunc_0_op_res_37197 = redout_39467;
            }
            
            int64_t defunc_0_op_res_37198;
            
            if (defunc_0_leq_res_37196) {
                defunc_0_op_res_37198 = i_39469;
            } else {
                defunc_0_op_res_37198 = redout_39468;
            }
            
            int32_t redout_tmp_40215 = defunc_0_op_res_37197;
            int64_t redout_tmp_40216 = defunc_0_op_res_37198;
            
            redout_39467 = redout_tmp_40215;
            redout_39468 = redout_tmp_40216;
        }
        x_40169 = redout_39467;
        x_40170 = redout_39468;
        defunc_0_reduce_res_39276 = x_40169;
        defunc_0_reduce_res_39277 = x_40170;
    } else {
        defunc_0_reduce_res_39276 = 0;
        defunc_0_reduce_res_39277 = (int64_t) 0;
    }
    
    int64_t distance_37210 = add64((int64_t) 1, tmp_37208);
    int64_t binop_x_39598 = (int64_t) 4 * smallest_pow_2_geq_than_res_37168;
    int64_t bytes_39599 = block_sizze_37178 * binop_x_39598;
    int64_t binop_x_39601 = (int64_t) 8 * smallest_pow_2_geq_than_res_37168;
    int64_t bytes_39602 = block_sizze_37178 * binop_x_39601;
    int64_t bytes_39612 = (int64_t) 4 * block_sizze_37178;
    int64_t bytes_39614 = (int64_t) 8 * block_sizze_37178;
    bool loop_cond_37285 = slt64(block_sizze_37178, flat_dim_37179);
    bool zzero_37286 = merge_block_sizze_37183 == (int64_t) 0;
    bool nonzzero_37287 = !zzero_37286;
    
    if (cond_37162) {
        if (memblock_set(ctx, &ext_mem_39802, &mem_39597, "mem_39597") != 0)
            return 1;
    } else {
        if (mem_39600_cached_sizze_40337 < bytes_39599) {
            err = lexical_realloc(ctx, &mem_39600, &mem_39600_cached_sizze_40337, bytes_39599);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_39603_cached_sizze_40338 < bytes_39602) {
            err = lexical_realloc(ctx, &mem_39603, &mem_39603_cached_sizze_40338, bytes_39602);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_39613_cached_sizze_40339 < bytes_39612) {
            err = lexical_realloc(ctx, &mem_39613, &mem_39613_cached_sizze_40339, bytes_39612);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_39615_cached_sizze_40340 < bytes_39614) {
            err = lexical_realloc(ctx, &mem_39615, &mem_39615_cached_sizze_40340, bytes_39614);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t i_39481 = 0; i_39481 < smallest_pow_2_geq_than_res_37168; i_39481++) {
            int64_t zp_lhs_37217 = mul64(block_sizze_37178, i_39481);
            
            for (int64_t i_39474 = 0; i_39474 < block_sizze_37178; i_39474++) {
                int64_t k_37221 = add64(zp_lhs_37217, i_39474);
                bool cond_37222 = slt64(k_37221, n1_24948);
                int32_t lifted_lambda_res_37223;
                int64_t lifted_lambda_res_37224;
                
                if (cond_37222) {
                    bool x_39129 = sle64((int64_t) 0, k_37221);
                    bool bounds_check_39130 = cond_37222 && x_39129;
                    bool index_certs_39131;
                    
                    if (!bounds_check_39130) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) k_37221, "] out of bounds for array of shape [", (long long) n1_24948, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:246:26-30\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ../../../joins/ftSMJ.fut:104:60-63\n   #8  test_matching.fut:25:43-47\n   #9  test_matching.fut:19:1-28:44\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int32_t lifted_lambda_res_t_res_39132 = ((int32_t *) ks1_mem_39592.mem)[k_37221];
                    
                    lifted_lambda_res_37223 = lifted_lambda_res_t_res_39132;
                    lifted_lambda_res_37224 = k_37221;
                } else {
                    lifted_lambda_res_37223 = defunc_0_reduce_res_39276;
                    lifted_lambda_res_37224 = defunc_0_reduce_res_39277;
                }
                ((int32_t *) mem_39613)[i_39474] = lifted_lambda_res_37223;
                ((int64_t *) mem_39615)[i_39474] = lifted_lambda_res_37224;
            }
            if (!cond_37207) {
                for (int64_t i_37234 = 0; i_37234 < distance_37210; i_37234++) {
                    int64_t gt_arg1_37237 = add64((int64_t) 1, i_37234);
                    bool x_37238 = sle64((int64_t) 0, gt_arg1_37237);
                    bool y_37239 = slt64(gt_arg1_37237, block_sizze_37178);
                    bool bounds_check_37240 = x_37238 && y_37239;
                    bool index_certs_37241;
                    
                    if (!bounds_check_37240) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) gt_arg1_37237, "] out of bounds for array of shape [", (long long) block_sizze_37178, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:169:31-37\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:25:43-47\n   #8  test_matching.fut:19:1-28:44\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    bool y_37243 = slt64(i_37234, block_sizze_37178);
                    bool index_certs_37244;
                    
                    if (!y_37243) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_37234, "] out of bounds for array of shape [", (long long) block_sizze_37178, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:169:26-30\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:25:43-47\n   #8  test_matching.fut:19:1-28:44\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int32_t gt_arg1_37242 = ((int32_t *) mem_39613)[gt_arg1_37237];
                    int32_t gt_arg0_37245 = ((int32_t *) mem_39613)[i_37234];
                    bool defunc_0_leq_res_37246 = sle32(gt_arg0_37245, gt_arg1_37242);
                    bool defunc_0_lifted_gt_res_37247 = !defunc_0_leq_res_37246;
                    bool s_37248;
                    int64_t s_37251;
                    bool loop_while_37252;
                    int64_t j_37255;
                    
                    loop_while_37252 = defunc_0_lifted_gt_res_37247;
                    j_37255 = i_37234;
                    while (loop_while_37252) {
                        bool x_37256 = sle64((int64_t) 0, j_37255);
                        bool y_37257 = slt64(j_37255, block_sizze_37178);
                        bool bounds_check_37258 = x_37256 && y_37257;
                        bool index_certs_37259;
                        
                        if (!bounds_check_37258) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_37255, "] out of bounds for array of shape [", (long long) block_sizze_37178, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:170:24-28\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:25:43-47\n   #8  test_matching.fut:19:1-28:44\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t copy_arg0_37262 = add64((int64_t) 1, j_37255);
                        bool x_37263 = sle64((int64_t) 0, copy_arg0_37262);
                        bool y_37264 = slt64(copy_arg0_37262, block_sizze_37178);
                        bool bounds_check_37265 = x_37263 && y_37264;
                        bool index_certs_37266;
                        
                        if (!bounds_check_37265) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) copy_arg0_37262, "] out of bounds for array of shape [", (long long) block_sizze_37178, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:171:35-41\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:25:43-47\n   #8  test_matching.fut:19:1-28:44\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int32_t copy_arg0_37260 = ((int32_t *) mem_39613)[j_37255];
                        int64_t copy_arg0_37261 = ((int64_t *) mem_39615)[j_37255];
                        int32_t copy_arg0_37267 = ((int32_t *) mem_39613)[copy_arg0_37262];
                        int64_t copy_arg0_37268 = ((int64_t *) mem_39615)[copy_arg0_37262];
                        
                        ((int32_t *) mem_39613)[j_37255] = copy_arg0_37267;
                        ((int64_t *) mem_39615)[j_37255] = copy_arg0_37268;
                        ((int32_t *) mem_39613)[copy_arg0_37262] = copy_arg0_37260;
                        ((int64_t *) mem_39615)[copy_arg0_37262] = copy_arg0_37261;
                        
                        int64_t tmp_37273 = sub64(j_37255, (int64_t) 1);
                        bool cond_37274 = sle64((int64_t) 0, tmp_37273);
                        bool loop_cond_37275;
                        
                        if (cond_37274) {
                            bool y_39134 = slt64(tmp_37273, block_sizze_37178);
                            bool bounds_check_39135 = cond_37274 && y_39134;
                            bool index_certs_39136;
                            
                            if (!bounds_check_39135) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_37273, "] out of bounds for array of shape [", (long long) block_sizze_37178, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:169:26-30\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:25:43-47\n   #8  test_matching.fut:19:1-28:44\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int32_t gt_arg1_39133 = ((int32_t *) mem_39613)[j_37255];
                            int32_t gt_arg0_39137 = ((int32_t *) mem_39613)[tmp_37273];
                            bool defunc_0_leq_res_39138 = sle32(gt_arg0_39137, gt_arg1_39133);
                            bool defunc_0_lifted_gt_res_39139 = !defunc_0_leq_res_39138;
                            
                            loop_cond_37275 = defunc_0_lifted_gt_res_39139;
                        } else {
                            loop_cond_37275 = 0;
                        }
                        
                        bool loop_while_tmp_40223 = loop_cond_37275;
                        int64_t j_tmp_40226 = tmp_37273;
                        
                        loop_while_37252 = loop_while_tmp_40223;
                        j_37255 = j_tmp_40226;
                    }
                    s_37248 = loop_while_37252;
                    s_37251 = j_37255;
                }
            }
            lmad_copy_4b(ctx, 1, (uint32_t *) mem_39600, i_39481 * block_sizze_37178, (int64_t []) {(int64_t) 1}, (uint32_t *) mem_39613, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {block_sizze_37178});
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_39603, i_39481 * block_sizze_37178, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_39615, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {block_sizze_37178});
        }
        if (memblock_alloc(ctx, &mem_39668, bytes_39599, "mem_39668")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_4b(ctx, 2, (uint32_t *) mem_39668.mem, (int64_t) 0, (int64_t []) {block_sizze_37178, (int64_t) 1}, (uint32_t *) mem_39600, (int64_t) 0, (int64_t []) {block_sizze_37178, (int64_t) 1}, (int64_t []) {smallest_pow_2_geq_than_res_37168, block_sizze_37178});
        if (memblock_alloc(ctx, &mem_39672, bytes_39602, "mem_39672")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 2, (uint64_t *) mem_39672.mem, (int64_t) 0, (int64_t []) {block_sizze_37178, (int64_t) 1}, (uint64_t *) mem_39603, (int64_t) 0, (int64_t []) {block_sizze_37178, (int64_t) 1}, (int64_t []) {smallest_pow_2_geq_than_res_37168, block_sizze_37178});
        
        int64_t data_37288;
        bool data_37289;
        int64_t data_37292;
        int64_t loop_dz2084Uz2082U_37293;
        bool loop_while_37294;
        int64_t stride_37297;
        
        if (memblock_set(ctx, &mem_param_39676, &mem_39668, "mem_39668") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_39679, &mem_39672, "mem_39672") != 0)
            return 1;
        loop_dz2084Uz2082U_37293 = flat_dim_37179;
        loop_while_37294 = loop_cond_37285;
        stride_37297 = block_sizze_37178;
        while (loop_while_37294) {
            int64_t next_stride_37298 = mul64((int64_t) 2, stride_37297);
            bool zzero_37299 = next_stride_37298 == (int64_t) 0;
            bool nonzzero_37300 = !zzero_37299;
            bool nonzzero_cert_37301;
            
            if (!nonzzero_37300) {
                set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:254:33-46\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftSMJ.fut:104:60-63\n   #4  test_matching.fut:25:43-47\n   #5  test_matching.fut:19:1-28:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t num_merges_37302 = sdiv64(flat_dim_37179, next_stride_37298);
            bool bounds_invalid_upwards_37303 = slt64(num_merges_37302, (int64_t) 0);
            bool valid_37304 = !bounds_invalid_upwards_37303;
            bool range_valid_c_37305;
            
            if (!valid_37304) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) num_merges_37302, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #3  /prelude/functional.fut:9:44-45\n   #4  ../../../joins/ftSMJ.fut:104:60-63\n   #5  test_matching.fut:25:43-47\n   #6  test_matching.fut:19:1-28:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t m_37307 = sub64(next_stride_37298, (int64_t) 1);
            bool zzero_leq_i_p_m_t_s_37308 = sle64((int64_t) 0, m_37307);
            bool cond_37310 = slt64(next_stride_37298, (int64_t) 10);
            bool protect_assert_disj_37311 = nonzzero_37287 || cond_37310;
            bool nonzzero_cert_37312;
            
            if (!protect_assert_disj_37311) {
                set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:147:30-42\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:25:43-47\n   #8  test_matching.fut:19:1-28:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t zeze_lhs_37313 = smod_safe64(next_stride_37298, merge_block_sizze_37183);
            bool num_blocks_37314 = zeze_lhs_37313 == (int64_t) 0;
            bool protect_assert_disj_37315 = cond_37310 || num_blocks_37314;
            bool assert_c_37316;
            
            if (!protect_assert_disj_37315) {
                set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "Assertion is false: (n % block_size == 0)", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:147:20-65\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:25:43-47\n   #8  test_matching.fut:19:1-28:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t num_blocks_37318 = sdiv_safe64(next_stride_37298, merge_block_sizze_37183);
            int64_t dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37319 = add64((int64_t) 1, num_blocks_37318);
            int64_t j_m_i_37320 = sub64(next_stride_37298, stride_37297);
            int64_t flat_dim_37321 = merge_block_sizze_37183 * num_blocks_37318;
            bool bounds_invalid_upwards_37322 = slt64(dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37319, (int64_t) 0);
            bool empty_slice_37323 = j_m_i_37320 == (int64_t) 0;
            int64_t m_37324 = sub64(j_m_i_37320, (int64_t) 1);
            bool zzero_lte_i_37325 = sle64((int64_t) 0, stride_37297);
            bool i_lte_j_37326 = sle64(stride_37297, next_stride_37298);
            bool empty_slice_37327 = stride_37297 == (int64_t) 0;
            int64_t m_37328 = sub64(stride_37297, (int64_t) 1);
            bool bounds_invalid_upwards_37329 = slt64(num_blocks_37318, (int64_t) 0);
            bool cond_37330 = slt64((int64_t) 0, stride_37297);
            bool valid_37331 = !bounds_invalid_upwards_37322;
            int64_t i_p_m_t_s_37332 = add64(stride_37297, m_37324);
            bool zzero_leq_i_p_m_t_s_37333 = sle64((int64_t) 0, m_37328);
            bool i_p_m_t_s_leq_w_37334 = slt64(m_37328, next_stride_37298);
            bool valid_37335 = !bounds_invalid_upwards_37329;
            bool i_p_m_t_s_leq_w_37336 = slt64(m_37307, flat_dim_37321);
            bool y_37337 = slt64((int64_t) 0, j_m_i_37320);
            bool protect_assert_disj_37338 = cond_37310 || valid_37331;
            bool range_valid_c_37339;
            
            if (!protect_assert_disj_37338) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37319, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  ../../../joins/ftSMJ.fut:104:60-63\n   #9  test_matching.fut:25:43-47\n   #10 test_matching.fut:19:1-28:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool zzero_leq_i_p_m_t_s_37340 = sle64((int64_t) 0, i_p_m_t_s_37332);
            bool i_p_m_t_s_leq_w_37341 = slt64(i_p_m_t_s_37332, next_stride_37298);
            bool y_37342 = zzero_leq_i_p_m_t_s_37333 && i_p_m_t_s_leq_w_37334;
            bool protect_assert_disj_37343 = cond_37310 || valid_37335;
            bool range_valid_c_37344;
            
            if (!protect_assert_disj_37343) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) num_blocks_37318, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ../../../joins/ftSMJ.fut:104:60-63\n   #8  test_matching.fut:25:43-47\n   #9  test_matching.fut:19:1-28:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool y_37345 = zzero_leq_i_p_m_t_s_37308 && i_p_m_t_s_leq_w_37336;
            bool y_37346 = zzero_lte_i_37325 && i_p_m_t_s_leq_w_37341;
            bool forwards_ok_37347 = zzero_lte_i_37325 && y_37342;
            bool ok_or_empty_37348 = zzero_37299 || y_37345;
            bool protect_assert_disj_37349 = cond_37330 || y_37337;
            bool y_37350 = zzero_leq_i_p_m_t_s_37340 && y_37346;
            bool ok_or_empty_37351 = empty_slice_37327 || forwards_ok_37347;
            bool protect_assert_disj_37352 = cond_37310 || ok_or_empty_37348;
            bool index_certs_37353;
            
            if (!protect_assert_disj_37352) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) next_stride_37298, "] out of bounds for array of shape [", (long long) flat_dim_37321, "].", "-> #0  /prelude/array.fut:46:45-51\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:155:3-46\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ../../../joins/ftSMJ.fut:104:60-63\n   #8  test_matching.fut:25:43-47\n   #9  test_matching.fut:19:1-28:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool loop_not_taken_37354 = !cond_37310;
            bool protect_assert_disj_37355 = protect_assert_disj_37349 || loop_not_taken_37354;
            bool index_certs_37356;
            
            if (!protect_assert_disj_37355) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) j_m_i_37320, "].", "-> #0  /prelude/array.fut:22:29-33\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:100:48-54\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  ../../../joins/ftSMJ.fut:104:60-63\n   #9  test_matching.fut:25:43-47\n   #10 test_matching.fut:19:1-28:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool forwards_ok_37357 = i_lte_j_37326 && y_37350;
            bool protect_assert_disj_37358 = cond_37310 || ok_or_empty_37351;
            bool index_certs_37359;
            
            if (!protect_assert_disj_37358) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) stride_37297, "] out of bounds for array of shape [", (long long) next_stride_37298, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:52-59\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  ../../../joins/ftSMJ.fut:104:60-63\n   #10 test_matching.fut:25:43-47\n   #11 test_matching.fut:19:1-28:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool protect_assert_disj_37360 = ok_or_empty_37351 || loop_not_taken_37354;
            bool index_certs_37361;
            
            if (!protect_assert_disj_37360) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) stride_37297, "] out of bounds for array of shape [", (long long) next_stride_37298, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:26-33\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:25:43-47\n   #8  test_matching.fut:19:1-28:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool ok_or_empty_37362 = empty_slice_37323 || forwards_ok_37357;
            bool protect_assert_disj_37363 = cond_37310 || ok_or_empty_37362;
            bool index_certs_37364;
            
            if (!protect_assert_disj_37363) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) stride_37297, ":] out of bounds for array of shape [", (long long) next_stride_37298, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:60-67\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  ../../../joins/ftSMJ.fut:104:60-63\n   #10 test_matching.fut:25:43-47\n   #11 test_matching.fut:19:1-28:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool protect_assert_disj_37365 = loop_not_taken_37354 || ok_or_empty_37362;
            bool index_certs_37366;
            
            if (!protect_assert_disj_37365) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) stride_37297, ":] out of bounds for array of shape [", (long long) next_stride_37298, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:34-41\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:25:43-47\n   #8  test_matching.fut:19:1-28:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t binop_x_39680 = (int64_t) 4 * num_merges_37302;
            int64_t bytes_39681 = next_stride_37298 * binop_x_39680;
            int64_t binop_x_39683 = (int64_t) 8 * num_merges_37302;
            int64_t bytes_39684 = next_stride_37298 * binop_x_39683;
            bool i_lte_j_37309 = sle64((int64_t) 0, next_stride_37298);
            
            if (mem_39682_cached_sizze_40341 < bytes_39681) {
                err = lexical_realloc(ctx, &mem_39682, &mem_39682_cached_sizze_40341, bytes_39681);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            if (mem_39685_cached_sizze_40342 < bytes_39684) {
                err = lexical_realloc(ctx, &mem_39685, &mem_39685_cached_sizze_40342, bytes_39684);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            
            int64_t bytes_39694 = (int64_t) 8 * dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37319;
            int64_t binop_x_39710 = (int64_t) 4 * num_blocks_37318;
            int64_t bytes_39711 = merge_block_sizze_37183 * binop_x_39710;
            int64_t binop_x_39713 = (int64_t) 8 * num_blocks_37318;
            int64_t bytes_39714 = merge_block_sizze_37183 * binop_x_39713;
            int64_t bytes_39756 = (int64_t) 8 * stride_37297;
            int64_t bytes_39758 = (int64_t) 16 * stride_37297;
            
            for (int64_t i_39502 = 0; i_39502 < num_merges_37302; i_39502++) {
                int64_t start_37370 = mul64(next_stride_37298, i_39502);
                int64_t j_m_i_37371 = sub64(loop_dz2084Uz2082U_37293, start_37370);
                bool empty_slice_37372 = j_m_i_37371 == (int64_t) 0;
                int64_t m_37373 = sub64(j_m_i_37371, (int64_t) 1);
                int64_t i_p_m_t_s_37374 = add64(start_37370, m_37373);
                bool zzero_leq_i_p_m_t_s_37375 = sle64((int64_t) 0, i_p_m_t_s_37374);
                bool i_p_m_t_s_leq_w_37376 = slt64(i_p_m_t_s_37374, loop_dz2084Uz2082U_37293);
                bool zzero_lte_i_37377 = sle64((int64_t) 0, start_37370);
                bool i_lte_j_37378 = sle64(start_37370, loop_dz2084Uz2082U_37293);
                bool y_37379 = i_p_m_t_s_leq_w_37376 && zzero_lte_i_37377;
                bool y_37380 = zzero_leq_i_p_m_t_s_37375 && y_37379;
                bool forwards_ok_37381 = i_lte_j_37378 && y_37380;
                bool ok_or_empty_37382 = empty_slice_37372 || forwards_ok_37381;
                bool index_certs_37383;
                
                if (!ok_or_empty_37382) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) start_37370, ":] out of bounds for array of shape [", (long long) loop_dz2084Uz2082U_37293, "].", "-> #0  /prelude/array.fut:52:49-54\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:258:41-56\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:25:43-47\n   #8  test_matching.fut:19:1-28:44\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                bool i_p_m_t_s_leq_w_37384 = slt64(m_37307, j_m_i_37371);
                bool y_37385 = zzero_leq_i_p_m_t_s_37308 && i_p_m_t_s_leq_w_37384;
                bool forwards_ok_37386 = i_lte_j_37309 && y_37385;
                bool ok_or_empty_37387 = zzero_37299 || forwards_ok_37386;
                bool index_certs_37388;
                
                if (!ok_or_empty_37387) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) next_stride_37298, "] out of bounds for array of shape [", (long long) j_m_i_37371, "].", "-> #0  /prelude/array.fut:46:45-51\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:258:23-56\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:25:43-47\n   #8  test_matching.fut:19:1-28:44\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                if (cond_37310) {
                    int32_t dummy_39165;
                    int64_t dummy_39166;
                    
                    if (cond_37330) {
                        int32_t head_res_39167 = ((int32_t *) mem_param_39676.mem)[start_37370];
                        int64_t head_res_39168 = ((int64_t *) mem_param_39679.mem)[start_37370];
                        
                        dummy_39165 = head_res_39167;
                        dummy_39166 = head_res_39168;
                    } else {
                        int64_t slice_39169 = stride_37297 + start_37370;
                        int32_t head_res_39170 = ((int32_t *) mem_param_39676.mem)[slice_39169];
                        int64_t head_res_39171 = ((int64_t *) mem_param_39679.mem)[slice_39169];
                        
                        dummy_39165 = head_res_39170;
                        dummy_39166 = head_res_39171;
                    }
                    if (memblock_alloc(ctx, &mem_39757, bytes_39756, "mem_39757")) {
                        err = 1;
                        goto cleanup;
                    }
                    for (int64_t nest_i_40236 = 0; nest_i_40236 < next_stride_37298; nest_i_40236++) {
                        ((int32_t *) mem_39757.mem)[nest_i_40236] = dummy_39165;
                    }
                    if (memblock_alloc(ctx, &mem_39759, bytes_39758, "mem_39759")) {
                        err = 1;
                        goto cleanup;
                    }
                    for (int64_t nest_i_40237 = 0; nest_i_40237 < next_stride_37298; nest_i_40237++) {
                        ((int64_t *) mem_39759.mem)[nest_i_40237] = dummy_39166;
                    }
                    
                    int64_t data_39174;
                    int64_t i_39178 = (int64_t) 0;
                    
                    for (int64_t k_39177 = 0; k_39177 < next_stride_37298; k_39177++) {
                        int64_t j_39181 = sub64(k_39177, i_39178);
                        bool cond_39182 = j_39181 == j_m_i_37320;
                        bool cond_39183;
                        
                        if (cond_39182) {
                            cond_39183 = 1;
                        } else {
                            bool cond_39184 = slt64(i_39178, stride_37297);
                            bool cond_f_res_39185;
                            
                            if (cond_39184) {
                                bool x_39186 = sle64((int64_t) 0, j_39181);
                                bool y_39187 = slt64(j_39181, j_m_i_37320);
                                bool bounds_check_39188 = x_39186 && y_39187;
                                bool index_certs_39189;
                                
                                if (!bounds_check_39188) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_39181, "] out of bounds for array of shape [", (long long) j_m_i_37320, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:55-59\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ../../../joins/ftSMJ.fut:104:60-63\n   #8  test_matching.fut:25:43-47\n   #9  test_matching.fut:19:1-28:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                bool x_39193 = sle64((int64_t) 0, i_39178);
                                bool bounds_check_39194 = cond_39184 && x_39193;
                                bool index_certs_39195;
                                
                                if (!bounds_check_39194) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_39178, "] out of bounds for array of shape [", (long long) stride_37297, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:50-54\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ../../../joins/ftSMJ.fut:104:60-63\n   #8  test_matching.fut:25:43-47\n   #9  test_matching.fut:19:1-28:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t slice_39190 = stride_37297 + j_39181;
                                int64_t slice_39191 = start_37370 + slice_39190;
                                int32_t leq_arg1_39192 = ((int32_t *) mem_param_39676.mem)[slice_39191];
                                int64_t slice_39196 = start_37370 + i_39178;
                                int32_t leq_arg0_39197 = ((int32_t *) mem_param_39676.mem)[slice_39196];
                                bool defunc_0_leq_res_39198 = sle32(leq_arg0_39197, leq_arg1_39192);
                                
                                cond_f_res_39185 = defunc_0_leq_res_39198;
                            } else {
                                cond_f_res_39185 = 0;
                            }
                            cond_39183 = cond_f_res_39185;
                        }
                        
                        int64_t loopres_39199;
                        int32_t loopres_39200;
                        int64_t loopres_39201;
                        
                        if (cond_39183) {
                            bool x_39203 = sle64((int64_t) 0, i_39178);
                            bool y_39204 = slt64(i_39178, stride_37297);
                            bool bounds_check_39205 = x_39203 && y_39204;
                            bool index_certs_39206;
                            
                            if (!bounds_check_39205) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_39178, "] out of bounds for array of shape [", (long long) stride_37297, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:106:17-21\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ../../../joins/ftSMJ.fut:104:60-63\n   #8  test_matching.fut:25:43-47\n   #9  test_matching.fut:19:1-28:44\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t tmp_39202 = add64((int64_t) 1, i_39178);
                            int64_t slice_39207 = start_37370 + i_39178;
                            int32_t tmp_39208 = ((int32_t *) mem_param_39676.mem)[slice_39207];
                            int64_t tmp_39209 = ((int64_t *) mem_param_39679.mem)[slice_39207];
                            
                            loopres_39199 = tmp_39202;
                            loopres_39200 = tmp_39208;
                            loopres_39201 = tmp_39209;
                        } else {
                            bool x_39210 = sle64((int64_t) 0, j_39181);
                            bool y_39211 = slt64(j_39181, j_m_i_37320);
                            bool bounds_check_39212 = x_39210 && y_39211;
                            bool index_certs_39213;
                            
                            if (!bounds_check_39212) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_39181, "] out of bounds for array of shape [", (long long) j_m_i_37320, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:108:15-19\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ../../../joins/ftSMJ.fut:104:60-63\n   #8  test_matching.fut:25:43-47\n   #9  test_matching.fut:19:1-28:44\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t slice_39214 = stride_37297 + j_39181;
                            int64_t slice_39215 = start_37370 + slice_39214;
                            int32_t tmp_39216 = ((int32_t *) mem_param_39676.mem)[slice_39215];
                            int64_t tmp_39217 = ((int64_t *) mem_param_39679.mem)[slice_39215];
                            
                            loopres_39199 = i_39178;
                            loopres_39200 = tmp_39216;
                            loopres_39201 = tmp_39217;
                        }
                        ((int32_t *) mem_39757.mem)[k_39177] = loopres_39200;
                        ((int64_t *) mem_39759.mem)[k_39177] = loopres_39201;
                        
                        int64_t i_tmp_40238 = loopres_39199;
                        
                        i_39178 = i_tmp_40238;
                    }
                    data_39174 = i_39178;
                    if (memblock_set(ctx, &ext_mem_39777, &mem_39757, "mem_39757") != 0)
                        return 1;
                    if (memblock_set(ctx, &ext_mem_39774, &mem_39759, "mem_39759") != 0)
                        return 1;
                } else {
                    if (mem_39695_cached_sizze_40343 < bytes_39694) {
                        err = lexical_realloc(ctx, &mem_39695, &mem_39695_cached_sizze_40343, bytes_39694);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    if (mem_39697_cached_sizze_40344 < bytes_39694) {
                        err = lexical_realloc(ctx, &mem_39697, &mem_39697_cached_sizze_40344, bytes_39694);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    for (int64_t i_39488 = 0; i_39488 < dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37319; i_39488++) {
                        int64_t split_count_arg3_37450 = mul64(merge_block_sizze_37183, i_39488);
                        int64_t defunc_0_split_count_res_37451;
                        int64_t defunc_0_split_count_res_37452;
                        int64_t defunc_0_split_count_res_37453;
                        int64_t defunc_0_split_count_res_37454;
                        int64_t defunc_0_split_count_res_37455;
                        
                        if (futrts_lifted_normalizze_7743(ctx, &defunc_0_split_count_res_37451, &defunc_0_split_count_res_37452, &defunc_0_split_count_res_37453, &defunc_0_split_count_res_37454, &defunc_0_split_count_res_37455, (int64_t) 0, stride_37297, (int64_t) 0, j_m_i_37320, split_count_arg3_37450) != 0) {
                            err = 1;
                            goto cleanup;
                        }
                        
                        bool loop_cond_37456 = slt64((int64_t) 0, defunc_0_split_count_res_37455);
                        bool defunc_0_split_count_res_37457;
                        int64_t defunc_0_split_count_res_37458;
                        int64_t defunc_0_split_count_res_37459;
                        int64_t defunc_0_split_count_res_37460;
                        int64_t defunc_0_split_count_res_37461;
                        int64_t defunc_0_split_count_res_37462;
                        bool loop_while_37463;
                        int64_t ss_37464;
                        int64_t ss_37465;
                        int64_t tt_37466;
                        int64_t tt_37467;
                        int64_t count_37468;
                        
                        loop_while_37463 = loop_cond_37456;
                        ss_37464 = defunc_0_split_count_res_37451;
                        ss_37465 = defunc_0_split_count_res_37452;
                        tt_37466 = defunc_0_split_count_res_37453;
                        tt_37467 = defunc_0_split_count_res_37454;
                        count_37468 = defunc_0_split_count_res_37455;
                        while (loop_while_37463) {
                            int64_t zlze_lhs_37469 = sub64(ss_37465, ss_37464);
                            bool cond_37470 = sle64(zlze_lhs_37469, (int64_t) 0);
                            int64_t defunc_0_lifted_step_res_37471;
                            int64_t defunc_0_lifted_step_res_37472;
                            int64_t defunc_0_lifted_step_res_37473;
                            int64_t defunc_0_lifted_step_res_37474;
                            
                            if (cond_37470) {
                                int64_t tmp_39220 = add64(tt_37466, count_37468);
                                
                                defunc_0_lifted_step_res_37471 = ss_37464;
                                defunc_0_lifted_step_res_37472 = tmp_39220;
                                defunc_0_lifted_step_res_37473 = tt_37467;
                                defunc_0_lifted_step_res_37474 = (int64_t) 0;
                            } else {
                                int64_t zlze_lhs_37476 = sub64(tt_37467, tt_37466);
                                bool cond_37477 = sle64(zlze_lhs_37476, (int64_t) 0);
                                int64_t defunc_0_lifted_step_res_f_res_37478;
                                int64_t defunc_0_lifted_step_res_f_res_37479;
                                int64_t defunc_0_lifted_step_res_f_res_37480;
                                int64_t defunc_0_lifted_step_res_f_res_37481;
                                
                                if (cond_37477) {
                                    int64_t tmp_39221 = add64(ss_37464, count_37468);
                                    
                                    defunc_0_lifted_step_res_f_res_37478 = tmp_39221;
                                    defunc_0_lifted_step_res_f_res_37479 = tt_37466;
                                    defunc_0_lifted_step_res_f_res_37480 = tt_37467;
                                    defunc_0_lifted_step_res_f_res_37481 = (int64_t) 0;
                                } else {
                                    bool cond_37483 = count_37468 == (int64_t) 1;
                                    int64_t defunc_0_lifted_step_res_f_res_f_res_37484;
                                    int64_t defunc_0_lifted_step_res_f_res_f_res_37485;
                                    int64_t defunc_0_lifted_step_res_f_res_f_res_37486;
                                    int64_t defunc_0_lifted_step_res_f_res_f_res_37487;
                                    
                                    if (cond_37483) {
                                        bool x_39223 = sle64((int64_t) 0, ss_37464);
                                        bool y_39224 = slt64(ss_37464, stride_37297);
                                        bool bounds_check_39225 = x_39223 && y_39224;
                                        bool index_certs_39226;
                                        
                                        if (!bounds_check_39225) {
                                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) ss_37464, "] out of bounds for array of shape [", (long long) stride_37297, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:53:21-27\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 ../../../joins/ftSMJ.fut:104:60-63\n   #12 test_matching.fut:25:43-47\n   #13 test_matching.fut:19:1-28:44\n"));
                                            err = FUTHARK_PROGRAM_ERROR;
                                            goto cleanup;
                                        }
                                        
                                        bool x_39229 = sle64((int64_t) 0, tt_37466);
                                        bool y_39230 = slt64(tt_37466, j_m_i_37320);
                                        bool bounds_check_39231 = x_39229 && y_39230;
                                        bool index_certs_39232;
                                        
                                        if (!bounds_check_39231) {
                                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tt_37466, "] out of bounds for array of shape [", (long long) j_m_i_37320, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:53:14-20\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 ../../../joins/ftSMJ.fut:104:60-63\n   #12 test_matching.fut:25:43-47\n   #13 test_matching.fut:19:1-28:44\n"));
                                            err = FUTHARK_PROGRAM_ERROR;
                                            goto cleanup;
                                        }
                                        
                                        int64_t slice_39227 = start_37370 + ss_37464;
                                        int32_t leq_arg1_39228 = ((int32_t *) mem_param_39676.mem)[slice_39227];
                                        int64_t slice_39233 = stride_37297 + tt_37466;
                                        int64_t slice_39234 = start_37370 + slice_39233;
                                        int32_t leq_arg0_39235 = ((int32_t *) mem_param_39676.mem)[slice_39234];
                                        bool defunc_0_leq_res_39236 = sle32(leq_arg0_39235, leq_arg1_39228);
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_t_res_39237;
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_t_res_39238;
                                        
                                        if (defunc_0_leq_res_39236) {
                                            int64_t tmp_39239 = add64((int64_t) 1, tt_37466);
                                            
                                            defunc_0_lifted_step_res_f_res_f_res_t_res_39237 = ss_37464;
                                            defunc_0_lifted_step_res_f_res_f_res_t_res_39238 = tmp_39239;
                                        } else {
                                            int64_t tmp_39240 = add64((int64_t) 1, ss_37464);
                                            
                                            defunc_0_lifted_step_res_f_res_f_res_t_res_39237 = tmp_39240;
                                            defunc_0_lifted_step_res_f_res_f_res_t_res_39238 = tt_37466;
                                        }
                                        defunc_0_lifted_step_res_f_res_f_res_37484 = defunc_0_lifted_step_res_f_res_f_res_t_res_39237;
                                        defunc_0_lifted_step_res_f_res_f_res_37485 = defunc_0_lifted_step_res_f_res_f_res_t_res_39238;
                                        defunc_0_lifted_step_res_f_res_f_res_37486 = tt_37467;
                                        defunc_0_lifted_step_res_f_res_f_res_37487 = (int64_t) 0;
                                    } else {
                                        int64_t m_37506 = sdiv64(count_37468, (int64_t) 2);
                                        int64_t n_37507 = sub64(count_37468, m_37506);
                                        bool cond_37508 = n_37507 == (int64_t) 0;
                                        int64_t zgze_lhs_37509 = add64(ss_37464, m_37506);
                                        bool cond_f_res_37510 = sle64(ss_37465, zgze_lhs_37509);
                                        bool x_37511 = !cond_37508;
                                        bool y_37512 = cond_f_res_37510 && x_37511;
                                        bool cond_37513 = cond_37508 || y_37512;
                                        bool leq_y_x_37514;
                                        
                                        if (cond_37513) {
                                            leq_y_x_37514 = 1;
                                        } else {
                                            bool x_37515 = sle64((int64_t) 0, zgze_lhs_37509);
                                            bool y_37516 = slt64(zgze_lhs_37509, stride_37297);
                                            bool bounds_check_37517 = x_37515 && y_37516;
                                            bool index_certs_37518;
                                            
                                            if (!bounds_check_37517) {
                                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) zgze_lhs_37509, "] out of bounds for array of shape [", (long long) stride_37297, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:73:22-30\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 ../../../joins/ftSMJ.fut:104:60-63\n   #12 test_matching.fut:25:43-47\n   #13 test_matching.fut:19:1-28:44\n"));
                                                err = FUTHARK_PROGRAM_ERROR;
                                                goto cleanup;
                                            }
                                            
                                            int64_t zm_lhs_37521 = add64(tt_37466, n_37507);
                                            int64_t leq_arg0_37522 = sub64(zm_lhs_37521, (int64_t) 1);
                                            bool x_37523 = sle64((int64_t) 0, leq_arg0_37522);
                                            bool y_37524 = slt64(leq_arg0_37522, j_m_i_37320);
                                            bool bounds_check_37525 = x_37523 && y_37524;
                                            bool index_certs_37526;
                                            
                                            if (!bounds_check_37525) {
                                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) leq_arg0_37522, "] out of bounds for array of shape [", (long long) j_m_i_37320, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:73:11-21\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 ../../../joins/ftSMJ.fut:104:60-63\n   #12 test_matching.fut:25:43-47\n   #13 test_matching.fut:19:1-28:44\n"));
                                                err = FUTHARK_PROGRAM_ERROR;
                                                goto cleanup;
                                            }
                                            
                                            int64_t slice_37519 = start_37370 + zgze_lhs_37509;
                                            int32_t leq_arg1_37520 = ((int32_t *) mem_param_39676.mem)[slice_37519];
                                            int64_t slice_37527 = stride_37297 + leq_arg0_37522;
                                            int64_t slice_37528 = start_37370 + slice_37527;
                                            int32_t leq_arg0_37529 = ((int32_t *) mem_param_39676.mem)[slice_37528];
                                            bool defunc_0_leq_res_37530 = sle32(leq_arg0_37529, leq_arg1_37520);
                                            
                                            leq_y_x_37514 = defunc_0_leq_res_37530;
                                        }
                                        
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_f_res_37531;
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_f_res_37532;
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_f_res_37533;
                                        
                                        if (leq_y_x_37514) {
                                            int64_t tmp_39241 = add64(tt_37466, n_37507);
                                            int64_t tmp_39242 = sub64(count_37468, n_37507);
                                            
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_37531 = tmp_39241;
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_37532 = tt_37467;
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_37533 = tmp_39242;
                                        } else {
                                            int64_t tmp_37536 = add64(tt_37466, n_37507);
                                            
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_37531 = tt_37466;
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_37532 = tmp_37536;
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_37533 = count_37468;
                                        }
                                        defunc_0_lifted_step_res_f_res_f_res_37484 = ss_37464;
                                        defunc_0_lifted_step_res_f_res_f_res_37485 = defunc_0_lifted_step_res_f_res_f_res_f_res_37531;
                                        defunc_0_lifted_step_res_f_res_f_res_37486 = defunc_0_lifted_step_res_f_res_f_res_f_res_37532;
                                        defunc_0_lifted_step_res_f_res_f_res_37487 = defunc_0_lifted_step_res_f_res_f_res_f_res_37533;
                                    }
                                    defunc_0_lifted_step_res_f_res_37478 = defunc_0_lifted_step_res_f_res_f_res_37484;
                                    defunc_0_lifted_step_res_f_res_37479 = defunc_0_lifted_step_res_f_res_f_res_37485;
                                    defunc_0_lifted_step_res_f_res_37480 = defunc_0_lifted_step_res_f_res_f_res_37486;
                                    defunc_0_lifted_step_res_f_res_37481 = defunc_0_lifted_step_res_f_res_f_res_37487;
                                }
                                defunc_0_lifted_step_res_37471 = defunc_0_lifted_step_res_f_res_37478;
                                defunc_0_lifted_step_res_37472 = defunc_0_lifted_step_res_f_res_37479;
                                defunc_0_lifted_step_res_37473 = defunc_0_lifted_step_res_f_res_37480;
                                defunc_0_lifted_step_res_37474 = defunc_0_lifted_step_res_f_res_37481;
                            }
                            
                            int64_t loopres_37537;
                            int64_t loopres_37538;
                            int64_t loopres_37539;
                            int64_t loopres_37540;
                            int64_t loopres_37541;
                            
                            if (futrts_lifted_normalizze_7743(ctx, &loopres_37537, &loopres_37538, &loopres_37539, &loopres_37540, &loopres_37541, defunc_0_lifted_step_res_37471, ss_37465, defunc_0_lifted_step_res_37472, defunc_0_lifted_step_res_37473, defunc_0_lifted_step_res_37474) != 0) {
                                err = 1;
                                goto cleanup;
                            }
                            
                            bool loop_cond_37542 = slt64((int64_t) 0, loopres_37541);
                            bool loop_while_tmp_40243 = loop_cond_37542;
                            int64_t ss_tmp_40244 = loopres_37537;
                            int64_t ss_tmp_40245 = loopres_37538;
                            int64_t tt_tmp_40246 = loopres_37539;
                            int64_t tt_tmp_40247 = loopres_37540;
                            int64_t count_tmp_40248 = loopres_37541;
                            
                            loop_while_37463 = loop_while_tmp_40243;
                            ss_37464 = ss_tmp_40244;
                            ss_37465 = ss_tmp_40245;
                            tt_37466 = tt_tmp_40246;
                            tt_37467 = tt_tmp_40247;
                            count_37468 = count_tmp_40248;
                        }
                        defunc_0_split_count_res_37457 = loop_while_37463;
                        defunc_0_split_count_res_37458 = ss_37464;
                        defunc_0_split_count_res_37459 = ss_37465;
                        defunc_0_split_count_res_37460 = tt_37466;
                        defunc_0_split_count_res_37461 = tt_37467;
                        defunc_0_split_count_res_37462 = count_37468;
                        ((int64_t *) mem_39695)[i_39488] = defunc_0_split_count_res_37458;
                        ((int64_t *) mem_39697)[i_39488] = defunc_0_split_count_res_37460;
                    }
                    if (mem_39712_cached_sizze_40345 < bytes_39711) {
                        err = lexical_realloc(ctx, &mem_39712, &mem_39712_cached_sizze_40345, bytes_39711);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    if (mem_39715_cached_sizze_40346 < bytes_39714) {
                        err = lexical_realloc(ctx, &mem_39715, &mem_39715_cached_sizze_40346, bytes_39714);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    for (int64_t i_39495 = 0; i_39495 < num_blocks_37318; i_39495++) {
                        bool y_37548 = slt64(i_39495, dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37319);
                        bool index_certs_37550;
                        
                        if (!y_37548) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_39495, "] out of bounds for array of shape [", (long long) dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37319, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:151:22-34\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  ../../../joins/ftSMJ.fut:104:60-63\n   #9  test_matching.fut:25:43-47\n   #10 test_matching.fut:19:1-28:44\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t defunc_0_f_res_37551 = ((int64_t *) mem_39695)[i_39495];
                        int64_t defunc_0_f_res_37552 = ((int64_t *) mem_39697)[i_39495];
                        int64_t tmp_37553 = add64((int64_t) 1, i_39495);
                        bool x_37554 = sle64((int64_t) 0, tmp_37553);
                        bool y_37555 = slt64(tmp_37553, dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37319);
                        bool bounds_check_37556 = x_37554 && y_37555;
                        bool index_certs_37557;
                        
                        if (!bounds_check_37556) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_37553, "] out of bounds for array of shape [", (long long) dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37319, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:152:22-36\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  ../../../joins/ftSMJ.fut:104:60-63\n   #9  test_matching.fut:25:43-47\n   #10 test_matching.fut:19:1-28:44\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t defunc_0_f_res_37558 = ((int64_t *) mem_39695)[tmp_37553];
                        int64_t defunc_0_f_res_37559 = ((int64_t *) mem_39697)[tmp_37553];
                        int64_t merge_sequential_arg2_37560 = add64(stride_37297, defunc_0_f_res_37552);
                        int64_t merge_sequential_arg2_37561 = add64(stride_37297, defunc_0_f_res_37559);
                        int64_t j_m_i_37562 = sub64(merge_sequential_arg2_37561, merge_sequential_arg2_37560);
                        bool empty_slice_37563 = j_m_i_37562 == (int64_t) 0;
                        int64_t m_37564 = sub64(j_m_i_37562, (int64_t) 1);
                        int64_t i_p_m_t_s_37565 = add64(merge_sequential_arg2_37560, m_37564);
                        bool zzero_leq_i_p_m_t_s_37566 = sle64((int64_t) 0, i_p_m_t_s_37565);
                        bool i_p_m_t_s_leq_w_37567 = slt64(i_p_m_t_s_37565, next_stride_37298);
                        bool zzero_lte_i_37568 = sle64((int64_t) 0, merge_sequential_arg2_37560);
                        bool i_lte_j_37569 = sle64(merge_sequential_arg2_37560, merge_sequential_arg2_37561);
                        bool y_37570 = i_p_m_t_s_leq_w_37567 && zzero_lte_i_37568;
                        bool y_37571 = zzero_leq_i_p_m_t_s_37566 && y_37570;
                        bool forwards_ok_37572 = i_lte_j_37569 && y_37571;
                        bool ok_or_empty_37573 = empty_slice_37563 || forwards_ok_37572;
                        bool index_certs_37574;
                        
                        if (!ok_or_empty_37573) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) merge_sequential_arg2_37560, ":", (long long) merge_sequential_arg2_37561, "] out of bounds for array of shape [", (long long) next_stride_37298, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:40-58\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  ../../../joins/ftSMJ.fut:104:60-63\n   #9  test_matching.fut:25:43-47\n   #10 test_matching.fut:19:1-28:44\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t j_m_i_37575 = sub64(defunc_0_f_res_37558, defunc_0_f_res_37551);
                        bool empty_slice_37576 = j_m_i_37575 == (int64_t) 0;
                        int64_t m_37577 = sub64(j_m_i_37575, (int64_t) 1);
                        int64_t i_p_m_t_s_37578 = add64(defunc_0_f_res_37551, m_37577);
                        bool zzero_leq_i_p_m_t_s_37579 = sle64((int64_t) 0, i_p_m_t_s_37578);
                        bool i_p_m_t_s_leq_w_37580 = slt64(i_p_m_t_s_37578, next_stride_37298);
                        bool zzero_lte_i_37581 = sle64((int64_t) 0, defunc_0_f_res_37551);
                        bool i_lte_j_37582 = sle64(defunc_0_f_res_37551, defunc_0_f_res_37558);
                        bool y_37583 = i_p_m_t_s_leq_w_37580 && zzero_lte_i_37581;
                        bool y_37584 = zzero_leq_i_p_m_t_s_37579 && y_37583;
                        bool forwards_ok_37585 = i_lte_j_37582 && y_37584;
                        bool ok_or_empty_37586 = empty_slice_37576 || forwards_ok_37585;
                        bool index_certs_37587;
                        
                        if (!ok_or_empty_37586) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) defunc_0_f_res_37551, ":", (long long) defunc_0_f_res_37558, "] out of bounds for array of shape [", (long long) next_stride_37298, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:29-39\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  ../../../joins/ftSMJ.fut:104:60-63\n   #9  test_matching.fut:25:43-47\n   #10 test_matching.fut:19:1-28:44\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        bool cond_37588 = slt64((int64_t) 0, j_m_i_37575);
                        int32_t dummy_37589;
                        int64_t dummy_37590;
                        
                        if (cond_37588) {
                            bool index_certs_39243;
                            
                            if (!cond_37588) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) j_m_i_37575, "].", "-> #0  /prelude/array.fut:22:29-33\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:100:36-42\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #6  /prelude/soacs.fut:59:9-11\n   #7  /prelude/array.fut:241:11-17\n   #8  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #9  /prelude/functional.fut:9:44-45\n   #10 ../../../joins/ftSMJ.fut:104:60-63\n   #11 test_matching.fut:25:43-47\n   #12 test_matching.fut:19:1-28:44\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t slice_39244 = start_37370 + defunc_0_f_res_37551;
                            int32_t head_res_39245 = ((int32_t *) mem_param_39676.mem)[slice_39244];
                            int64_t head_res_39246 = ((int64_t *) mem_param_39679.mem)[slice_39244];
                            
                            dummy_37589 = head_res_39245;
                            dummy_37590 = head_res_39246;
                        } else {
                            bool y_37595 = slt64((int64_t) 0, j_m_i_37562);
                            bool index_certs_37596;
                            
                            if (!y_37595) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) j_m_i_37562, "].", "-> #0  /prelude/array.fut:22:29-33\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:100:48-54\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #6  /prelude/soacs.fut:59:9-11\n   #7  /prelude/array.fut:241:11-17\n   #8  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #9  /prelude/functional.fut:9:44-45\n   #10 ../../../joins/ftSMJ.fut:104:60-63\n   #11 test_matching.fut:25:43-47\n   #12 test_matching.fut:19:1-28:44\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t slice_37597 = start_37370 + merge_sequential_arg2_37560;
                            int32_t head_res_37598 = ((int32_t *) mem_param_39676.mem)[slice_37597];
                            int64_t head_res_37599 = ((int64_t *) mem_param_39679.mem)[slice_37597];
                            
                            dummy_37589 = head_res_37598;
                            dummy_37590 = head_res_37599;
                        }
                        for (int64_t nest_i_40251 = 0; nest_i_40251 < merge_block_sizze_37183; nest_i_40251++) {
                            ((int32_t *) mem_39712)[i_39495 * merge_block_sizze_37183 + nest_i_40251] = dummy_37589;
                        }
                        for (int64_t nest_i_40252 = 0; nest_i_40252 < merge_block_sizze_37183; nest_i_40252++) {
                            ((int64_t *) mem_39715)[i_39495 * merge_block_sizze_37183 + nest_i_40252] = dummy_37590;
                        }
                        
                        int64_t data_37602;
                        int64_t i_37606 = (int64_t) 0;
                        
                        for (int64_t k_37605 = 0; k_37605 < merge_block_sizze_37183; k_37605++) {
                            int64_t j_37609 = sub64(k_37605, i_37606);
                            bool cond_37610 = j_37609 == j_m_i_37562;
                            bool cond_37611;
                            
                            if (cond_37610) {
                                cond_37611 = 1;
                            } else {
                                bool cond_37612 = slt64(i_37606, j_m_i_37575);
                                bool cond_f_res_37613;
                                
                                if (cond_37612) {
                                    bool x_39247 = sle64((int64_t) 0, j_37609);
                                    bool y_39248 = slt64(j_37609, j_m_i_37562);
                                    bool bounds_check_39249 = x_39247 && y_39248;
                                    bool index_certs_39250;
                                    
                                    if (!bounds_check_39249) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_37609, "] out of bounds for array of shape [", (long long) j_m_i_37562, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:55-59\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  ../../../joins/ftSMJ.fut:104:60-63\n   #10 test_matching.fut:25:43-47\n   #11 test_matching.fut:19:1-28:44\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    bool x_39254 = sle64((int64_t) 0, i_37606);
                                    bool bounds_check_39255 = cond_37612 && x_39254;
                                    bool index_certs_39256;
                                    
                                    if (!bounds_check_39255) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_37606, "] out of bounds for array of shape [", (long long) j_m_i_37575, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:50-54\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  ../../../joins/ftSMJ.fut:104:60-63\n   #10 test_matching.fut:25:43-47\n   #11 test_matching.fut:19:1-28:44\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    int64_t slice_39251 = merge_sequential_arg2_37560 + j_37609;
                                    int64_t slice_39252 = start_37370 + slice_39251;
                                    int32_t leq_arg1_39253 = ((int32_t *) mem_param_39676.mem)[slice_39252];
                                    int64_t slice_39257 = defunc_0_f_res_37551 + i_37606;
                                    int64_t slice_39258 = start_37370 + slice_39257;
                                    int32_t leq_arg0_39259 = ((int32_t *) mem_param_39676.mem)[slice_39258];
                                    bool defunc_0_leq_res_39260 = sle32(leq_arg0_39259, leq_arg1_39253);
                                    
                                    cond_f_res_37613 = defunc_0_leq_res_39260;
                                } else {
                                    cond_f_res_37613 = 0;
                                }
                                cond_37611 = cond_f_res_37613;
                            }
                            
                            int64_t loopres_37628;
                            int32_t loopres_37629;
                            int64_t loopres_37630;
                            
                            if (cond_37611) {
                                bool x_39262 = sle64((int64_t) 0, i_37606);
                                bool y_39263 = slt64(i_37606, j_m_i_37575);
                                bool bounds_check_39264 = x_39262 && y_39263;
                                bool index_certs_39265;
                                
                                if (!bounds_check_39264) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_37606, "] out of bounds for array of shape [", (long long) j_m_i_37575, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:106:17-21\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  ../../../joins/ftSMJ.fut:104:60-63\n   #10 test_matching.fut:25:43-47\n   #11 test_matching.fut:19:1-28:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t tmp_39261 = add64((int64_t) 1, i_37606);
                                int64_t slice_39266 = defunc_0_f_res_37551 + i_37606;
                                int64_t slice_39267 = start_37370 + slice_39266;
                                int32_t tmp_39268 = ((int32_t *) mem_param_39676.mem)[slice_39267];
                                int64_t tmp_39269 = ((int64_t *) mem_param_39679.mem)[slice_39267];
                                
                                loopres_37628 = tmp_39261;
                                loopres_37629 = tmp_39268;
                                loopres_37630 = tmp_39269;
                            } else {
                                bool x_37640 = sle64((int64_t) 0, j_37609);
                                bool y_37641 = slt64(j_37609, j_m_i_37562);
                                bool bounds_check_37642 = x_37640 && y_37641;
                                bool index_certs_37643;
                                
                                if (!bounds_check_37642) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_37609, "] out of bounds for array of shape [", (long long) j_m_i_37562, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:108:15-19\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  ../../../joins/ftSMJ.fut:104:60-63\n   #10 test_matching.fut:25:43-47\n   #11 test_matching.fut:19:1-28:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t slice_37644 = merge_sequential_arg2_37560 + j_37609;
                                int64_t slice_37645 = start_37370 + slice_37644;
                                int32_t tmp_37646 = ((int32_t *) mem_param_39676.mem)[slice_37645];
                                int64_t tmp_37647 = ((int64_t *) mem_param_39679.mem)[slice_37645];
                                
                                loopres_37628 = i_37606;
                                loopres_37629 = tmp_37646;
                                loopres_37630 = tmp_37647;
                            }
                            ((int32_t *) mem_39712)[i_39495 * merge_block_sizze_37183 + k_37605] = loopres_37629;
                            ((int64_t *) mem_39715)[i_39495 * merge_block_sizze_37183 + k_37605] = loopres_37630;
                            
                            int64_t i_tmp_40253 = loopres_37628;
                            
                            i_37606 = i_tmp_40253;
                        }
                        data_37602 = i_37606;
                    }
                    if (memblock_alloc(ctx, &mem_39750, bytes_39711, "mem_39750")) {
                        err = 1;
                        goto cleanup;
                    }
                    lmad_copy_4b(ctx, 2, (uint32_t *) mem_39750.mem, (int64_t) 0, (int64_t []) {merge_block_sizze_37183, (int64_t) 1}, (uint32_t *) mem_39712, (int64_t) 0, (int64_t []) {merge_block_sizze_37183, (int64_t) 1}, (int64_t []) {num_blocks_37318, merge_block_sizze_37183});
                    if (memblock_alloc(ctx, &mem_39754, bytes_39714, "mem_39754")) {
                        err = 1;
                        goto cleanup;
                    }
                    lmad_copy_8b(ctx, 2, (uint64_t *) mem_39754.mem, (int64_t) 0, (int64_t []) {merge_block_sizze_37183, (int64_t) 1}, (uint64_t *) mem_39715, (int64_t) 0, (int64_t []) {merge_block_sizze_37183, (int64_t) 1}, (int64_t []) {num_blocks_37318, merge_block_sizze_37183});
                    if (memblock_set(ctx, &ext_mem_39777, &mem_39750, "mem_39750") != 0)
                        return 1;
                    if (memblock_set(ctx, &ext_mem_39774, &mem_39754, "mem_39754") != 0)
                        return 1;
                }
                lmad_copy_4b(ctx, 1, (uint32_t *) mem_39682, i_39502 * next_stride_37298, (int64_t []) {(int64_t) 1}, (uint32_t *) ext_mem_39777.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {next_stride_37298});
                if (memblock_unref(ctx, &ext_mem_39777, "ext_mem_39777") != 0)
                    return 1;
                lmad_copy_8b(ctx, 1, (uint64_t *) mem_39685, i_39502 * next_stride_37298, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_39774.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {next_stride_37298});
                if (memblock_unref(ctx, &ext_mem_39774, "ext_mem_39774") != 0)
                    return 1;
            }
            
            int64_t flat_dim_37654 = next_stride_37298 * num_merges_37302;
            
            if (memblock_alloc(ctx, &mem_39788, bytes_39681, "mem_39788")) {
                err = 1;
                goto cleanup;
            }
            lmad_copy_4b(ctx, 2, (uint32_t *) mem_39788.mem, (int64_t) 0, (int64_t []) {next_stride_37298, (int64_t) 1}, (uint32_t *) mem_39682, (int64_t) 0, (int64_t []) {next_stride_37298, (int64_t) 1}, (int64_t []) {num_merges_37302, next_stride_37298});
            if (memblock_alloc(ctx, &mem_39792, bytes_39684, "mem_39792")) {
                err = 1;
                goto cleanup;
            }
            lmad_copy_8b(ctx, 2, (uint64_t *) mem_39792.mem, (int64_t) 0, (int64_t []) {next_stride_37298, (int64_t) 1}, (uint64_t *) mem_39685, (int64_t) 0, (int64_t []) {next_stride_37298, (int64_t) 1}, (int64_t []) {num_merges_37302, next_stride_37298});
            
            bool loop_cond_37657 = slt64(next_stride_37298, flat_dim_37179);
            
            if (memblock_set(ctx, &mem_param_tmp_40227, &mem_39788, "mem_39788") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_tmp_40228, &mem_39792, "mem_39792") != 0)
                return 1;
            
            int64_t loop_dz2084Uz2082U_tmp_40229 = flat_dim_37654;
            bool loop_while_tmp_40230 = loop_cond_37657;
            int64_t stride_tmp_40233 = next_stride_37298;
            
            if (memblock_set(ctx, &mem_param_39676, &mem_param_tmp_40227, "mem_param_tmp_40227") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_39679, &mem_param_tmp_40228, "mem_param_tmp_40228") != 0)
                return 1;
            loop_dz2084Uz2082U_37293 = loop_dz2084Uz2082U_tmp_40229;
            loop_while_37294 = loop_while_tmp_40230;
            stride_37297 = stride_tmp_40233;
        }
        if (memblock_set(ctx, &ext_mem_39799, &mem_param_39676, "mem_param_39676") != 0)
            return 1;
        if (memblock_set(ctx, &ext_mem_39798, &mem_param_39679, "mem_param_39679") != 0)
            return 1;
        data_37288 = loop_dz2084Uz2082U_37293;
        data_37289 = loop_while_37294;
        data_37292 = stride_37297;
        if (memblock_unref(ctx, &mem_39668, "mem_39668") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39672, "mem_39672") != 0)
            return 1;
        
        bool empty_slice_37658 = n1_24948 == (int64_t) 0;
        bool zzero_leq_i_p_m_t_s_37659 = sle64((int64_t) 0, zs_lhs_37163);
        bool i_p_m_t_s_leq_w_37660 = slt64(zs_lhs_37163, data_37288);
        bool y_37661 = zzero_leq_i_p_m_t_s_37659 && i_p_m_t_s_leq_w_37660;
        bool ok_or_empty_37662 = empty_slice_37658 || y_37661;
        bool index_certs_37663;
        
        if (!ok_or_empty_37662) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) n1_24948, "] out of bounds for array of shape [", (long long) data_37288, "].", "-> #0  /prelude/array.fut:46:45-51\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:265:3-14\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #3  /prelude/functional.fut:9:44-45\n   #4  ../../../joins/ftSMJ.fut:104:60-63\n   #5  test_matching.fut:25:43-47\n   #6  test_matching.fut:19:1-28:44\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        if (memblock_set(ctx, &ext_mem_39802, &ext_mem_39798, "ext_mem_39798") != 0)
            return 1;
    }
    if (memblock_unref(ctx, &mem_39597, "mem_39597") != 0)
        return 1;
    if (mem_39804_cached_sizze_40347 < bytes_39803) {
        err = lexical_realloc(ctx, &mem_39804, &mem_39804_cached_sizze_40347, bytes_39803);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    for (int64_t i_39507 = 0; i_39507 < n1_24948; i_39507++) {
        int64_t eta_p_37671 = ((int64_t *) ext_mem_39802.mem)[i_39507];
        bool x_37672 = sle64((int64_t) 0, eta_p_37671);
        bool y_37673 = slt64(eta_p_37671, n1_24948);
        bool bounds_check_37674 = x_37672 && y_37673;
        bool index_certs_37675;
        
        if (!bounds_check_37674) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_37671, "] out of bounds for array of shape [", (long long) n1_24948, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:277:24-28\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:277:8-29\n   #3  ../../../joins/ftSMJ.fut:104:60-63\n   #4  test_matching.fut:25:43-47\n   #5  test_matching.fut:19:1-28:44\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int32_t lifted_lambda_res_37676 = ((int32_t *) ks1_mem_39592.mem)[eta_p_37671];
        
        ((int32_t *) mem_39804)[i_39507] = lifted_lambda_res_37676;
    }
    if (memblock_unref(ctx, &ext_mem_39802, "ext_mem_39802") != 0)
        return 1;
    if (memblock_alloc(ctx, &mem_39812, bytes_39803, "mem_39812")) {
        err = 1;
        goto cleanup;
    }
    lmad_copy_4b(ctx, 1, (uint32_t *) mem_39812.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint32_t *) mem_39804, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {n1_24948});
    if (memblock_alloc(ctx, &mem_39814, bytes_39813, "mem_39814")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t i_40257 = 0; i_40257 < n2_24949; i_40257++) {
        int64_t x_40258 = (int64_t) 0 + i_40257 * (int64_t) 1;
        
        ((int64_t *) mem_39814.mem)[i_40257] = x_40258;
    }
    
    int64_t flat_dim_37699 = smallest_pow_2_geq_than_res_37688 * block_sizze_37698;
    bool x_40135 = !cond_37682;
    int64_t greatest_divisor_leq_than_arg1_37702 = mul64((int64_t) 2, block_sizze_37698);
    int64_t merge_block_sizze_37703;
    
    if (x_40135) {
        int64_t x_40137;
        
        if (futrts_greatest_divisor_leq_than_5346(ctx, &x_40137, (int64_t) 8, greatest_divisor_leq_than_arg1_37702) != 0) {
            err = 1;
            goto cleanup;
        }
        merge_block_sizze_37703 = x_40137;
    } else {
        merge_block_sizze_37703 = (int64_t) 0;
    }
    
    int32_t reduce_arg1_37706;
    
    if (x_40135) {
        int32_t x_40141 = ((int32_t *) ks2_mem_39594.mem)[(int64_t) 0];
        
        reduce_arg1_37706 = x_40141;
    } else {
        reduce_arg1_37706 = 0;
    }
    
    int32_t defunc_0_reduce_res_39427;
    int64_t defunc_0_reduce_res_39428;
    
    if (x_40135) {
        int32_t x_40143;
        int64_t x_40144;
        int32_t redout_39509;
        int64_t redout_39510;
        
        redout_39509 = reduce_arg1_37706;
        redout_39510 = (int64_t) 0;
        for (int64_t i_39511 = 0; i_39511 < n2_24949; i_39511++) {
            int32_t x_37710 = ((int32_t *) ks2_mem_39594.mem)[i_39511];
            bool defunc_0_leq_res_37716 = sle32(redout_39509, x_37710);
            int32_t defunc_0_op_res_37717;
            
            if (defunc_0_leq_res_37716) {
                defunc_0_op_res_37717 = x_37710;
            } else {
                defunc_0_op_res_37717 = redout_39509;
            }
            
            int64_t defunc_0_op_res_37718;
            
            if (defunc_0_leq_res_37716) {
                defunc_0_op_res_37718 = i_39511;
            } else {
                defunc_0_op_res_37718 = redout_39510;
            }
            
            int32_t redout_tmp_40259 = defunc_0_op_res_37717;
            int64_t redout_tmp_40260 = defunc_0_op_res_37718;
            
            redout_39509 = redout_tmp_40259;
            redout_39510 = redout_tmp_40260;
        }
        x_40143 = redout_39509;
        x_40144 = redout_39510;
        defunc_0_reduce_res_39427 = x_40143;
        defunc_0_reduce_res_39428 = x_40144;
    } else {
        defunc_0_reduce_res_39427 = 0;
        defunc_0_reduce_res_39428 = (int64_t) 0;
    }
    
    int64_t distance_37730 = add64((int64_t) 1, tmp_37728);
    int64_t binop_x_39815 = (int64_t) 4 * smallest_pow_2_geq_than_res_37688;
    int64_t bytes_39816 = block_sizze_37698 * binop_x_39815;
    int64_t binop_x_39818 = (int64_t) 8 * smallest_pow_2_geq_than_res_37688;
    int64_t bytes_39819 = block_sizze_37698 * binop_x_39818;
    int64_t bytes_39829 = (int64_t) 4 * block_sizze_37698;
    int64_t bytes_39831 = (int64_t) 8 * block_sizze_37698;
    bool loop_cond_37805 = slt64(block_sizze_37698, flat_dim_37699);
    bool zzero_37806 = merge_block_sizze_37703 == (int64_t) 0;
    bool nonzzero_37807 = !zzero_37806;
    
    if (cond_37682) {
        if (memblock_set(ctx, &ext_mem_40019, &mem_39814, "mem_39814") != 0)
            return 1;
    } else {
        if (mem_39817_cached_sizze_40348 < bytes_39816) {
            err = lexical_realloc(ctx, &mem_39817, &mem_39817_cached_sizze_40348, bytes_39816);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_39820_cached_sizze_40349 < bytes_39819) {
            err = lexical_realloc(ctx, &mem_39820, &mem_39820_cached_sizze_40349, bytes_39819);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_39830_cached_sizze_40350 < bytes_39829) {
            err = lexical_realloc(ctx, &mem_39830, &mem_39830_cached_sizze_40350, bytes_39829);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_39832_cached_sizze_40351 < bytes_39831) {
            err = lexical_realloc(ctx, &mem_39832, &mem_39832_cached_sizze_40351, bytes_39831);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t i_39523 = 0; i_39523 < smallest_pow_2_geq_than_res_37688; i_39523++) {
            int64_t zp_lhs_37737 = mul64(block_sizze_37698, i_39523);
            
            for (int64_t i_39516 = 0; i_39516 < block_sizze_37698; i_39516++) {
                int64_t k_37741 = add64(zp_lhs_37737, i_39516);
                bool cond_37742 = slt64(k_37741, n2_24949);
                int32_t lifted_lambda_res_37743;
                int64_t lifted_lambda_res_37744;
                
                if (cond_37742) {
                    bool x_39280 = sle64((int64_t) 0, k_37741);
                    bool bounds_check_39281 = cond_37742 && x_39280;
                    bool index_certs_39282;
                    
                    if (!bounds_check_39281) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) k_37741, "] out of bounds for array of shape [", (long long) n2_24949, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:246:26-30\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ../../../joins/ftSMJ.fut:104:60-63\n   #8  test_matching.fut:26:43-47\n   #9  test_matching.fut:19:1-28:44\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int32_t lifted_lambda_res_t_res_39283 = ((int32_t *) ks2_mem_39594.mem)[k_37741];
                    
                    lifted_lambda_res_37743 = lifted_lambda_res_t_res_39283;
                    lifted_lambda_res_37744 = k_37741;
                } else {
                    lifted_lambda_res_37743 = defunc_0_reduce_res_39427;
                    lifted_lambda_res_37744 = defunc_0_reduce_res_39428;
                }
                ((int32_t *) mem_39830)[i_39516] = lifted_lambda_res_37743;
                ((int64_t *) mem_39832)[i_39516] = lifted_lambda_res_37744;
            }
            if (!cond_37727) {
                for (int64_t i_37754 = 0; i_37754 < distance_37730; i_37754++) {
                    int64_t gt_arg1_37757 = add64((int64_t) 1, i_37754);
                    bool x_37758 = sle64((int64_t) 0, gt_arg1_37757);
                    bool y_37759 = slt64(gt_arg1_37757, block_sizze_37698);
                    bool bounds_check_37760 = x_37758 && y_37759;
                    bool index_certs_37761;
                    
                    if (!bounds_check_37760) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) gt_arg1_37757, "] out of bounds for array of shape [", (long long) block_sizze_37698, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:169:31-37\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:26:43-47\n   #8  test_matching.fut:19:1-28:44\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    bool y_37763 = slt64(i_37754, block_sizze_37698);
                    bool index_certs_37764;
                    
                    if (!y_37763) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_37754, "] out of bounds for array of shape [", (long long) block_sizze_37698, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:169:26-30\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:26:43-47\n   #8  test_matching.fut:19:1-28:44\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int32_t gt_arg1_37762 = ((int32_t *) mem_39830)[gt_arg1_37757];
                    int32_t gt_arg0_37765 = ((int32_t *) mem_39830)[i_37754];
                    bool defunc_0_leq_res_37766 = sle32(gt_arg0_37765, gt_arg1_37762);
                    bool defunc_0_lifted_gt_res_37767 = !defunc_0_leq_res_37766;
                    bool s_37768;
                    int64_t s_37771;
                    bool loop_while_37772;
                    int64_t j_37775;
                    
                    loop_while_37772 = defunc_0_lifted_gt_res_37767;
                    j_37775 = i_37754;
                    while (loop_while_37772) {
                        bool x_37776 = sle64((int64_t) 0, j_37775);
                        bool y_37777 = slt64(j_37775, block_sizze_37698);
                        bool bounds_check_37778 = x_37776 && y_37777;
                        bool index_certs_37779;
                        
                        if (!bounds_check_37778) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_37775, "] out of bounds for array of shape [", (long long) block_sizze_37698, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:170:24-28\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:26:43-47\n   #8  test_matching.fut:19:1-28:44\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t copy_arg0_37782 = add64((int64_t) 1, j_37775);
                        bool x_37783 = sle64((int64_t) 0, copy_arg0_37782);
                        bool y_37784 = slt64(copy_arg0_37782, block_sizze_37698);
                        bool bounds_check_37785 = x_37783 && y_37784;
                        bool index_certs_37786;
                        
                        if (!bounds_check_37785) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) copy_arg0_37782, "] out of bounds for array of shape [", (long long) block_sizze_37698, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:171:35-41\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:26:43-47\n   #8  test_matching.fut:19:1-28:44\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int32_t copy_arg0_37780 = ((int32_t *) mem_39830)[j_37775];
                        int64_t copy_arg0_37781 = ((int64_t *) mem_39832)[j_37775];
                        int32_t copy_arg0_37787 = ((int32_t *) mem_39830)[copy_arg0_37782];
                        int64_t copy_arg0_37788 = ((int64_t *) mem_39832)[copy_arg0_37782];
                        
                        ((int32_t *) mem_39830)[j_37775] = copy_arg0_37787;
                        ((int64_t *) mem_39832)[j_37775] = copy_arg0_37788;
                        ((int32_t *) mem_39830)[copy_arg0_37782] = copy_arg0_37780;
                        ((int64_t *) mem_39832)[copy_arg0_37782] = copy_arg0_37781;
                        
                        int64_t tmp_37793 = sub64(j_37775, (int64_t) 1);
                        bool cond_37794 = sle64((int64_t) 0, tmp_37793);
                        bool loop_cond_37795;
                        
                        if (cond_37794) {
                            bool y_39285 = slt64(tmp_37793, block_sizze_37698);
                            bool bounds_check_39286 = cond_37794 && y_39285;
                            bool index_certs_39287;
                            
                            if (!bounds_check_39286) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_37793, "] out of bounds for array of shape [", (long long) block_sizze_37698, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:169:26-30\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:26:43-47\n   #8  test_matching.fut:19:1-28:44\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int32_t gt_arg1_39284 = ((int32_t *) mem_39830)[j_37775];
                            int32_t gt_arg0_39288 = ((int32_t *) mem_39830)[tmp_37793];
                            bool defunc_0_leq_res_39289 = sle32(gt_arg0_39288, gt_arg1_39284);
                            bool defunc_0_lifted_gt_res_39290 = !defunc_0_leq_res_39289;
                            
                            loop_cond_37795 = defunc_0_lifted_gt_res_39290;
                        } else {
                            loop_cond_37795 = 0;
                        }
                        
                        bool loop_while_tmp_40267 = loop_cond_37795;
                        int64_t j_tmp_40270 = tmp_37793;
                        
                        loop_while_37772 = loop_while_tmp_40267;
                        j_37775 = j_tmp_40270;
                    }
                    s_37768 = loop_while_37772;
                    s_37771 = j_37775;
                }
            }
            lmad_copy_4b(ctx, 1, (uint32_t *) mem_39817, i_39523 * block_sizze_37698, (int64_t []) {(int64_t) 1}, (uint32_t *) mem_39830, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {block_sizze_37698});
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_39820, i_39523 * block_sizze_37698, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_39832, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {block_sizze_37698});
        }
        if (memblock_alloc(ctx, &mem_39885, bytes_39816, "mem_39885")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_4b(ctx, 2, (uint32_t *) mem_39885.mem, (int64_t) 0, (int64_t []) {block_sizze_37698, (int64_t) 1}, (uint32_t *) mem_39817, (int64_t) 0, (int64_t []) {block_sizze_37698, (int64_t) 1}, (int64_t []) {smallest_pow_2_geq_than_res_37688, block_sizze_37698});
        if (memblock_alloc(ctx, &mem_39889, bytes_39819, "mem_39889")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 2, (uint64_t *) mem_39889.mem, (int64_t) 0, (int64_t []) {block_sizze_37698, (int64_t) 1}, (uint64_t *) mem_39820, (int64_t) 0, (int64_t []) {block_sizze_37698, (int64_t) 1}, (int64_t []) {smallest_pow_2_geq_than_res_37688, block_sizze_37698});
        
        int64_t data_37808;
        bool data_37809;
        int64_t data_37812;
        int64_t loop_dz2084Uz2082U_37813;
        bool loop_while_37814;
        int64_t stride_37817;
        
        if (memblock_set(ctx, &mem_param_39893, &mem_39885, "mem_39885") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_39896, &mem_39889, "mem_39889") != 0)
            return 1;
        loop_dz2084Uz2082U_37813 = flat_dim_37699;
        loop_while_37814 = loop_cond_37805;
        stride_37817 = block_sizze_37698;
        while (loop_while_37814) {
            int64_t next_stride_37818 = mul64((int64_t) 2, stride_37817);
            bool zzero_37819 = next_stride_37818 == (int64_t) 0;
            bool nonzzero_37820 = !zzero_37819;
            bool nonzzero_cert_37821;
            
            if (!nonzzero_37820) {
                set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:254:33-46\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftSMJ.fut:104:60-63\n   #4  test_matching.fut:26:43-47\n   #5  test_matching.fut:19:1-28:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t num_merges_37822 = sdiv64(flat_dim_37699, next_stride_37818);
            bool bounds_invalid_upwards_37823 = slt64(num_merges_37822, (int64_t) 0);
            bool valid_37824 = !bounds_invalid_upwards_37823;
            bool range_valid_c_37825;
            
            if (!valid_37824) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) num_merges_37822, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #3  /prelude/functional.fut:9:44-45\n   #4  ../../../joins/ftSMJ.fut:104:60-63\n   #5  test_matching.fut:26:43-47\n   #6  test_matching.fut:19:1-28:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t m_37827 = sub64(next_stride_37818, (int64_t) 1);
            bool zzero_leq_i_p_m_t_s_37828 = sle64((int64_t) 0, m_37827);
            bool cond_37830 = slt64(next_stride_37818, (int64_t) 10);
            bool protect_assert_disj_37831 = nonzzero_37807 || cond_37830;
            bool nonzzero_cert_37832;
            
            if (!protect_assert_disj_37831) {
                set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:147:30-42\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:26:43-47\n   #8  test_matching.fut:19:1-28:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t zeze_lhs_37833 = smod_safe64(next_stride_37818, merge_block_sizze_37703);
            bool num_blocks_37834 = zeze_lhs_37833 == (int64_t) 0;
            bool protect_assert_disj_37835 = cond_37830 || num_blocks_37834;
            bool assert_c_37836;
            
            if (!protect_assert_disj_37835) {
                set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "Assertion is false: (n % block_size == 0)", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:147:20-65\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:26:43-47\n   #8  test_matching.fut:19:1-28:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t num_blocks_37838 = sdiv_safe64(next_stride_37818, merge_block_sizze_37703);
            int64_t dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37839 = add64((int64_t) 1, num_blocks_37838);
            int64_t j_m_i_37840 = sub64(next_stride_37818, stride_37817);
            int64_t flat_dim_37841 = merge_block_sizze_37703 * num_blocks_37838;
            bool bounds_invalid_upwards_37842 = slt64(dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37839, (int64_t) 0);
            bool empty_slice_37843 = j_m_i_37840 == (int64_t) 0;
            int64_t m_37844 = sub64(j_m_i_37840, (int64_t) 1);
            bool zzero_lte_i_37845 = sle64((int64_t) 0, stride_37817);
            bool i_lte_j_37846 = sle64(stride_37817, next_stride_37818);
            bool empty_slice_37847 = stride_37817 == (int64_t) 0;
            int64_t m_37848 = sub64(stride_37817, (int64_t) 1);
            bool bounds_invalid_upwards_37849 = slt64(num_blocks_37838, (int64_t) 0);
            bool cond_37850 = slt64((int64_t) 0, stride_37817);
            bool valid_37851 = !bounds_invalid_upwards_37842;
            int64_t i_p_m_t_s_37852 = add64(stride_37817, m_37844);
            bool zzero_leq_i_p_m_t_s_37853 = sle64((int64_t) 0, m_37848);
            bool i_p_m_t_s_leq_w_37854 = slt64(m_37848, next_stride_37818);
            bool valid_37855 = !bounds_invalid_upwards_37849;
            bool i_p_m_t_s_leq_w_37856 = slt64(m_37827, flat_dim_37841);
            bool y_37857 = slt64((int64_t) 0, j_m_i_37840);
            bool protect_assert_disj_37858 = cond_37830 || valid_37851;
            bool range_valid_c_37859;
            
            if (!protect_assert_disj_37858) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37839, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  ../../../joins/ftSMJ.fut:104:60-63\n   #9  test_matching.fut:26:43-47\n   #10 test_matching.fut:19:1-28:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool zzero_leq_i_p_m_t_s_37860 = sle64((int64_t) 0, i_p_m_t_s_37852);
            bool i_p_m_t_s_leq_w_37861 = slt64(i_p_m_t_s_37852, next_stride_37818);
            bool y_37862 = zzero_leq_i_p_m_t_s_37853 && i_p_m_t_s_leq_w_37854;
            bool protect_assert_disj_37863 = cond_37830 || valid_37855;
            bool range_valid_c_37864;
            
            if (!protect_assert_disj_37863) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) num_blocks_37838, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ../../../joins/ftSMJ.fut:104:60-63\n   #8  test_matching.fut:26:43-47\n   #9  test_matching.fut:19:1-28:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool y_37865 = zzero_leq_i_p_m_t_s_37828 && i_p_m_t_s_leq_w_37856;
            bool y_37866 = zzero_lte_i_37845 && i_p_m_t_s_leq_w_37861;
            bool forwards_ok_37867 = zzero_lte_i_37845 && y_37862;
            bool ok_or_empty_37868 = zzero_37819 || y_37865;
            bool protect_assert_disj_37869 = cond_37850 || y_37857;
            bool y_37870 = zzero_leq_i_p_m_t_s_37860 && y_37866;
            bool ok_or_empty_37871 = empty_slice_37847 || forwards_ok_37867;
            bool protect_assert_disj_37872 = cond_37830 || ok_or_empty_37868;
            bool index_certs_37873;
            
            if (!protect_assert_disj_37872) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) next_stride_37818, "] out of bounds for array of shape [", (long long) flat_dim_37841, "].", "-> #0  /prelude/array.fut:46:45-51\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:155:3-46\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ../../../joins/ftSMJ.fut:104:60-63\n   #8  test_matching.fut:26:43-47\n   #9  test_matching.fut:19:1-28:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool loop_not_taken_37874 = !cond_37830;
            bool protect_assert_disj_37875 = protect_assert_disj_37869 || loop_not_taken_37874;
            bool index_certs_37876;
            
            if (!protect_assert_disj_37875) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) j_m_i_37840, "].", "-> #0  /prelude/array.fut:22:29-33\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:100:48-54\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  ../../../joins/ftSMJ.fut:104:60-63\n   #9  test_matching.fut:26:43-47\n   #10 test_matching.fut:19:1-28:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool forwards_ok_37877 = i_lte_j_37846 && y_37870;
            bool protect_assert_disj_37878 = cond_37830 || ok_or_empty_37871;
            bool index_certs_37879;
            
            if (!protect_assert_disj_37878) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) stride_37817, "] out of bounds for array of shape [", (long long) next_stride_37818, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:52-59\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  ../../../joins/ftSMJ.fut:104:60-63\n   #10 test_matching.fut:26:43-47\n   #11 test_matching.fut:19:1-28:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool protect_assert_disj_37880 = ok_or_empty_37871 || loop_not_taken_37874;
            bool index_certs_37881;
            
            if (!protect_assert_disj_37880) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) stride_37817, "] out of bounds for array of shape [", (long long) next_stride_37818, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:26-33\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:26:43-47\n   #8  test_matching.fut:19:1-28:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool ok_or_empty_37882 = empty_slice_37843 || forwards_ok_37877;
            bool protect_assert_disj_37883 = cond_37830 || ok_or_empty_37882;
            bool index_certs_37884;
            
            if (!protect_assert_disj_37883) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) stride_37817, ":] out of bounds for array of shape [", (long long) next_stride_37818, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:60-67\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  ../../../joins/ftSMJ.fut:104:60-63\n   #10 test_matching.fut:26:43-47\n   #11 test_matching.fut:19:1-28:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool protect_assert_disj_37885 = loop_not_taken_37874 || ok_or_empty_37882;
            bool index_certs_37886;
            
            if (!protect_assert_disj_37885) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) stride_37817, ":] out of bounds for array of shape [", (long long) next_stride_37818, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:34-41\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:26:43-47\n   #8  test_matching.fut:19:1-28:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t binop_x_39897 = (int64_t) 4 * num_merges_37822;
            int64_t bytes_39898 = next_stride_37818 * binop_x_39897;
            int64_t binop_x_39900 = (int64_t) 8 * num_merges_37822;
            int64_t bytes_39901 = next_stride_37818 * binop_x_39900;
            bool i_lte_j_37829 = sle64((int64_t) 0, next_stride_37818);
            
            if (mem_39899_cached_sizze_40352 < bytes_39898) {
                err = lexical_realloc(ctx, &mem_39899, &mem_39899_cached_sizze_40352, bytes_39898);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            if (mem_39902_cached_sizze_40353 < bytes_39901) {
                err = lexical_realloc(ctx, &mem_39902, &mem_39902_cached_sizze_40353, bytes_39901);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            
            int64_t bytes_39911 = (int64_t) 8 * dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37839;
            int64_t binop_x_39927 = (int64_t) 4 * num_blocks_37838;
            int64_t bytes_39928 = merge_block_sizze_37703 * binop_x_39927;
            int64_t binop_x_39930 = (int64_t) 8 * num_blocks_37838;
            int64_t bytes_39931 = merge_block_sizze_37703 * binop_x_39930;
            int64_t bytes_39973 = (int64_t) 8 * stride_37817;
            int64_t bytes_39975 = (int64_t) 16 * stride_37817;
            
            for (int64_t i_39544 = 0; i_39544 < num_merges_37822; i_39544++) {
                int64_t start_37890 = mul64(next_stride_37818, i_39544);
                int64_t j_m_i_37891 = sub64(loop_dz2084Uz2082U_37813, start_37890);
                bool empty_slice_37892 = j_m_i_37891 == (int64_t) 0;
                int64_t m_37893 = sub64(j_m_i_37891, (int64_t) 1);
                int64_t i_p_m_t_s_37894 = add64(start_37890, m_37893);
                bool zzero_leq_i_p_m_t_s_37895 = sle64((int64_t) 0, i_p_m_t_s_37894);
                bool i_p_m_t_s_leq_w_37896 = slt64(i_p_m_t_s_37894, loop_dz2084Uz2082U_37813);
                bool zzero_lte_i_37897 = sle64((int64_t) 0, start_37890);
                bool i_lte_j_37898 = sle64(start_37890, loop_dz2084Uz2082U_37813);
                bool y_37899 = i_p_m_t_s_leq_w_37896 && zzero_lte_i_37897;
                bool y_37900 = zzero_leq_i_p_m_t_s_37895 && y_37899;
                bool forwards_ok_37901 = i_lte_j_37898 && y_37900;
                bool ok_or_empty_37902 = empty_slice_37892 || forwards_ok_37901;
                bool index_certs_37903;
                
                if (!ok_or_empty_37902) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) start_37890, ":] out of bounds for array of shape [", (long long) loop_dz2084Uz2082U_37813, "].", "-> #0  /prelude/array.fut:52:49-54\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:258:41-56\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:26:43-47\n   #8  test_matching.fut:19:1-28:44\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                bool i_p_m_t_s_leq_w_37904 = slt64(m_37827, j_m_i_37891);
                bool y_37905 = zzero_leq_i_p_m_t_s_37828 && i_p_m_t_s_leq_w_37904;
                bool forwards_ok_37906 = i_lte_j_37829 && y_37905;
                bool ok_or_empty_37907 = zzero_37819 || forwards_ok_37906;
                bool index_certs_37908;
                
                if (!ok_or_empty_37907) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) next_stride_37818, "] out of bounds for array of shape [", (long long) j_m_i_37891, "].", "-> #0  /prelude/array.fut:46:45-51\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:258:23-56\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:26:43-47\n   #8  test_matching.fut:19:1-28:44\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                if (cond_37830) {
                    int32_t dummy_39316;
                    int64_t dummy_39317;
                    
                    if (cond_37850) {
                        int32_t head_res_39318 = ((int32_t *) mem_param_39893.mem)[start_37890];
                        int64_t head_res_39319 = ((int64_t *) mem_param_39896.mem)[start_37890];
                        
                        dummy_39316 = head_res_39318;
                        dummy_39317 = head_res_39319;
                    } else {
                        int64_t slice_39320 = stride_37817 + start_37890;
                        int32_t head_res_39321 = ((int32_t *) mem_param_39893.mem)[slice_39320];
                        int64_t head_res_39322 = ((int64_t *) mem_param_39896.mem)[slice_39320];
                        
                        dummy_39316 = head_res_39321;
                        dummy_39317 = head_res_39322;
                    }
                    if (memblock_alloc(ctx, &mem_39974, bytes_39973, "mem_39974")) {
                        err = 1;
                        goto cleanup;
                    }
                    for (int64_t nest_i_40280 = 0; nest_i_40280 < next_stride_37818; nest_i_40280++) {
                        ((int32_t *) mem_39974.mem)[nest_i_40280] = dummy_39316;
                    }
                    if (memblock_alloc(ctx, &mem_39976, bytes_39975, "mem_39976")) {
                        err = 1;
                        goto cleanup;
                    }
                    for (int64_t nest_i_40281 = 0; nest_i_40281 < next_stride_37818; nest_i_40281++) {
                        ((int64_t *) mem_39976.mem)[nest_i_40281] = dummy_39317;
                    }
                    
                    int64_t data_39325;
                    int64_t i_39329 = (int64_t) 0;
                    
                    for (int64_t k_39328 = 0; k_39328 < next_stride_37818; k_39328++) {
                        int64_t j_39332 = sub64(k_39328, i_39329);
                        bool cond_39333 = j_39332 == j_m_i_37840;
                        bool cond_39334;
                        
                        if (cond_39333) {
                            cond_39334 = 1;
                        } else {
                            bool cond_39335 = slt64(i_39329, stride_37817);
                            bool cond_f_res_39336;
                            
                            if (cond_39335) {
                                bool x_39337 = sle64((int64_t) 0, j_39332);
                                bool y_39338 = slt64(j_39332, j_m_i_37840);
                                bool bounds_check_39339 = x_39337 && y_39338;
                                bool index_certs_39340;
                                
                                if (!bounds_check_39339) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_39332, "] out of bounds for array of shape [", (long long) j_m_i_37840, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:55-59\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ../../../joins/ftSMJ.fut:104:60-63\n   #8  test_matching.fut:26:43-47\n   #9  test_matching.fut:19:1-28:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                bool x_39344 = sle64((int64_t) 0, i_39329);
                                bool bounds_check_39345 = cond_39335 && x_39344;
                                bool index_certs_39346;
                                
                                if (!bounds_check_39345) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_39329, "] out of bounds for array of shape [", (long long) stride_37817, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:50-54\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ../../../joins/ftSMJ.fut:104:60-63\n   #8  test_matching.fut:26:43-47\n   #9  test_matching.fut:19:1-28:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t slice_39341 = stride_37817 + j_39332;
                                int64_t slice_39342 = start_37890 + slice_39341;
                                int32_t leq_arg1_39343 = ((int32_t *) mem_param_39893.mem)[slice_39342];
                                int64_t slice_39347 = start_37890 + i_39329;
                                int32_t leq_arg0_39348 = ((int32_t *) mem_param_39893.mem)[slice_39347];
                                bool defunc_0_leq_res_39349 = sle32(leq_arg0_39348, leq_arg1_39343);
                                
                                cond_f_res_39336 = defunc_0_leq_res_39349;
                            } else {
                                cond_f_res_39336 = 0;
                            }
                            cond_39334 = cond_f_res_39336;
                        }
                        
                        int64_t loopres_39350;
                        int32_t loopres_39351;
                        int64_t loopres_39352;
                        
                        if (cond_39334) {
                            bool x_39354 = sle64((int64_t) 0, i_39329);
                            bool y_39355 = slt64(i_39329, stride_37817);
                            bool bounds_check_39356 = x_39354 && y_39355;
                            bool index_certs_39357;
                            
                            if (!bounds_check_39356) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_39329, "] out of bounds for array of shape [", (long long) stride_37817, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:106:17-21\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ../../../joins/ftSMJ.fut:104:60-63\n   #8  test_matching.fut:26:43-47\n   #9  test_matching.fut:19:1-28:44\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t tmp_39353 = add64((int64_t) 1, i_39329);
                            int64_t slice_39358 = start_37890 + i_39329;
                            int32_t tmp_39359 = ((int32_t *) mem_param_39893.mem)[slice_39358];
                            int64_t tmp_39360 = ((int64_t *) mem_param_39896.mem)[slice_39358];
                            
                            loopres_39350 = tmp_39353;
                            loopres_39351 = tmp_39359;
                            loopres_39352 = tmp_39360;
                        } else {
                            bool x_39361 = sle64((int64_t) 0, j_39332);
                            bool y_39362 = slt64(j_39332, j_m_i_37840);
                            bool bounds_check_39363 = x_39361 && y_39362;
                            bool index_certs_39364;
                            
                            if (!bounds_check_39363) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_39332, "] out of bounds for array of shape [", (long long) j_m_i_37840, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:108:15-19\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ../../../joins/ftSMJ.fut:104:60-63\n   #8  test_matching.fut:26:43-47\n   #9  test_matching.fut:19:1-28:44\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t slice_39365 = stride_37817 + j_39332;
                            int64_t slice_39366 = start_37890 + slice_39365;
                            int32_t tmp_39367 = ((int32_t *) mem_param_39893.mem)[slice_39366];
                            int64_t tmp_39368 = ((int64_t *) mem_param_39896.mem)[slice_39366];
                            
                            loopres_39350 = i_39329;
                            loopres_39351 = tmp_39367;
                            loopres_39352 = tmp_39368;
                        }
                        ((int32_t *) mem_39974.mem)[k_39328] = loopres_39351;
                        ((int64_t *) mem_39976.mem)[k_39328] = loopres_39352;
                        
                        int64_t i_tmp_40282 = loopres_39350;
                        
                        i_39329 = i_tmp_40282;
                    }
                    data_39325 = i_39329;
                    if (memblock_set(ctx, &ext_mem_39994, &mem_39974, "mem_39974") != 0)
                        return 1;
                    if (memblock_set(ctx, &ext_mem_39991, &mem_39976, "mem_39976") != 0)
                        return 1;
                } else {
                    if (mem_39912_cached_sizze_40354 < bytes_39911) {
                        err = lexical_realloc(ctx, &mem_39912, &mem_39912_cached_sizze_40354, bytes_39911);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    if (mem_39914_cached_sizze_40355 < bytes_39911) {
                        err = lexical_realloc(ctx, &mem_39914, &mem_39914_cached_sizze_40355, bytes_39911);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    for (int64_t i_39530 = 0; i_39530 < dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37839; i_39530++) {
                        int64_t split_count_arg3_37970 = mul64(merge_block_sizze_37703, i_39530);
                        int64_t defunc_0_split_count_res_37971;
                        int64_t defunc_0_split_count_res_37972;
                        int64_t defunc_0_split_count_res_37973;
                        int64_t defunc_0_split_count_res_37974;
                        int64_t defunc_0_split_count_res_37975;
                        
                        if (futrts_lifted_normalizze_7743(ctx, &defunc_0_split_count_res_37971, &defunc_0_split_count_res_37972, &defunc_0_split_count_res_37973, &defunc_0_split_count_res_37974, &defunc_0_split_count_res_37975, (int64_t) 0, stride_37817, (int64_t) 0, j_m_i_37840, split_count_arg3_37970) != 0) {
                            err = 1;
                            goto cleanup;
                        }
                        
                        bool loop_cond_37976 = slt64((int64_t) 0, defunc_0_split_count_res_37975);
                        bool defunc_0_split_count_res_37977;
                        int64_t defunc_0_split_count_res_37978;
                        int64_t defunc_0_split_count_res_37979;
                        int64_t defunc_0_split_count_res_37980;
                        int64_t defunc_0_split_count_res_37981;
                        int64_t defunc_0_split_count_res_37982;
                        bool loop_while_37983;
                        int64_t ss_37984;
                        int64_t ss_37985;
                        int64_t tt_37986;
                        int64_t tt_37987;
                        int64_t count_37988;
                        
                        loop_while_37983 = loop_cond_37976;
                        ss_37984 = defunc_0_split_count_res_37971;
                        ss_37985 = defunc_0_split_count_res_37972;
                        tt_37986 = defunc_0_split_count_res_37973;
                        tt_37987 = defunc_0_split_count_res_37974;
                        count_37988 = defunc_0_split_count_res_37975;
                        while (loop_while_37983) {
                            int64_t zlze_lhs_37989 = sub64(ss_37985, ss_37984);
                            bool cond_37990 = sle64(zlze_lhs_37989, (int64_t) 0);
                            int64_t defunc_0_lifted_step_res_37991;
                            int64_t defunc_0_lifted_step_res_37992;
                            int64_t defunc_0_lifted_step_res_37993;
                            int64_t defunc_0_lifted_step_res_37994;
                            
                            if (cond_37990) {
                                int64_t tmp_39371 = add64(tt_37986, count_37988);
                                
                                defunc_0_lifted_step_res_37991 = ss_37984;
                                defunc_0_lifted_step_res_37992 = tmp_39371;
                                defunc_0_lifted_step_res_37993 = tt_37987;
                                defunc_0_lifted_step_res_37994 = (int64_t) 0;
                            } else {
                                int64_t zlze_lhs_37996 = sub64(tt_37987, tt_37986);
                                bool cond_37997 = sle64(zlze_lhs_37996, (int64_t) 0);
                                int64_t defunc_0_lifted_step_res_f_res_37998;
                                int64_t defunc_0_lifted_step_res_f_res_37999;
                                int64_t defunc_0_lifted_step_res_f_res_38000;
                                int64_t defunc_0_lifted_step_res_f_res_38001;
                                
                                if (cond_37997) {
                                    int64_t tmp_39372 = add64(ss_37984, count_37988);
                                    
                                    defunc_0_lifted_step_res_f_res_37998 = tmp_39372;
                                    defunc_0_lifted_step_res_f_res_37999 = tt_37986;
                                    defunc_0_lifted_step_res_f_res_38000 = tt_37987;
                                    defunc_0_lifted_step_res_f_res_38001 = (int64_t) 0;
                                } else {
                                    bool cond_38003 = count_37988 == (int64_t) 1;
                                    int64_t defunc_0_lifted_step_res_f_res_f_res_38004;
                                    int64_t defunc_0_lifted_step_res_f_res_f_res_38005;
                                    int64_t defunc_0_lifted_step_res_f_res_f_res_38006;
                                    int64_t defunc_0_lifted_step_res_f_res_f_res_38007;
                                    
                                    if (cond_38003) {
                                        bool x_39374 = sle64((int64_t) 0, ss_37984);
                                        bool y_39375 = slt64(ss_37984, stride_37817);
                                        bool bounds_check_39376 = x_39374 && y_39375;
                                        bool index_certs_39377;
                                        
                                        if (!bounds_check_39376) {
                                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) ss_37984, "] out of bounds for array of shape [", (long long) stride_37817, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:53:21-27\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 ../../../joins/ftSMJ.fut:104:60-63\n   #12 test_matching.fut:26:43-47\n   #13 test_matching.fut:19:1-28:44\n"));
                                            err = FUTHARK_PROGRAM_ERROR;
                                            goto cleanup;
                                        }
                                        
                                        bool x_39380 = sle64((int64_t) 0, tt_37986);
                                        bool y_39381 = slt64(tt_37986, j_m_i_37840);
                                        bool bounds_check_39382 = x_39380 && y_39381;
                                        bool index_certs_39383;
                                        
                                        if (!bounds_check_39382) {
                                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tt_37986, "] out of bounds for array of shape [", (long long) j_m_i_37840, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:53:14-20\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 ../../../joins/ftSMJ.fut:104:60-63\n   #12 test_matching.fut:26:43-47\n   #13 test_matching.fut:19:1-28:44\n"));
                                            err = FUTHARK_PROGRAM_ERROR;
                                            goto cleanup;
                                        }
                                        
                                        int64_t slice_39378 = start_37890 + ss_37984;
                                        int32_t leq_arg1_39379 = ((int32_t *) mem_param_39893.mem)[slice_39378];
                                        int64_t slice_39384 = stride_37817 + tt_37986;
                                        int64_t slice_39385 = start_37890 + slice_39384;
                                        int32_t leq_arg0_39386 = ((int32_t *) mem_param_39893.mem)[slice_39385];
                                        bool defunc_0_leq_res_39387 = sle32(leq_arg0_39386, leq_arg1_39379);
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_t_res_39388;
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_t_res_39389;
                                        
                                        if (defunc_0_leq_res_39387) {
                                            int64_t tmp_39390 = add64((int64_t) 1, tt_37986);
                                            
                                            defunc_0_lifted_step_res_f_res_f_res_t_res_39388 = ss_37984;
                                            defunc_0_lifted_step_res_f_res_f_res_t_res_39389 = tmp_39390;
                                        } else {
                                            int64_t tmp_39391 = add64((int64_t) 1, ss_37984);
                                            
                                            defunc_0_lifted_step_res_f_res_f_res_t_res_39388 = tmp_39391;
                                            defunc_0_lifted_step_res_f_res_f_res_t_res_39389 = tt_37986;
                                        }
                                        defunc_0_lifted_step_res_f_res_f_res_38004 = defunc_0_lifted_step_res_f_res_f_res_t_res_39388;
                                        defunc_0_lifted_step_res_f_res_f_res_38005 = defunc_0_lifted_step_res_f_res_f_res_t_res_39389;
                                        defunc_0_lifted_step_res_f_res_f_res_38006 = tt_37987;
                                        defunc_0_lifted_step_res_f_res_f_res_38007 = (int64_t) 0;
                                    } else {
                                        int64_t m_38026 = sdiv64(count_37988, (int64_t) 2);
                                        int64_t n_38027 = sub64(count_37988, m_38026);
                                        bool cond_38028 = n_38027 == (int64_t) 0;
                                        int64_t zgze_lhs_38029 = add64(ss_37984, m_38026);
                                        bool cond_f_res_38030 = sle64(ss_37985, zgze_lhs_38029);
                                        bool x_38031 = !cond_38028;
                                        bool y_38032 = cond_f_res_38030 && x_38031;
                                        bool cond_38033 = cond_38028 || y_38032;
                                        bool leq_y_x_38034;
                                        
                                        if (cond_38033) {
                                            leq_y_x_38034 = 1;
                                        } else {
                                            bool x_38035 = sle64((int64_t) 0, zgze_lhs_38029);
                                            bool y_38036 = slt64(zgze_lhs_38029, stride_37817);
                                            bool bounds_check_38037 = x_38035 && y_38036;
                                            bool index_certs_38038;
                                            
                                            if (!bounds_check_38037) {
                                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) zgze_lhs_38029, "] out of bounds for array of shape [", (long long) stride_37817, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:73:22-30\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 ../../../joins/ftSMJ.fut:104:60-63\n   #12 test_matching.fut:26:43-47\n   #13 test_matching.fut:19:1-28:44\n"));
                                                err = FUTHARK_PROGRAM_ERROR;
                                                goto cleanup;
                                            }
                                            
                                            int64_t zm_lhs_38041 = add64(tt_37986, n_38027);
                                            int64_t leq_arg0_38042 = sub64(zm_lhs_38041, (int64_t) 1);
                                            bool x_38043 = sle64((int64_t) 0, leq_arg0_38042);
                                            bool y_38044 = slt64(leq_arg0_38042, j_m_i_37840);
                                            bool bounds_check_38045 = x_38043 && y_38044;
                                            bool index_certs_38046;
                                            
                                            if (!bounds_check_38045) {
                                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) leq_arg0_38042, "] out of bounds for array of shape [", (long long) j_m_i_37840, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:73:11-21\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 ../../../joins/ftSMJ.fut:104:60-63\n   #12 test_matching.fut:26:43-47\n   #13 test_matching.fut:19:1-28:44\n"));
                                                err = FUTHARK_PROGRAM_ERROR;
                                                goto cleanup;
                                            }
                                            
                                            int64_t slice_38039 = start_37890 + zgze_lhs_38029;
                                            int32_t leq_arg1_38040 = ((int32_t *) mem_param_39893.mem)[slice_38039];
                                            int64_t slice_38047 = stride_37817 + leq_arg0_38042;
                                            int64_t slice_38048 = start_37890 + slice_38047;
                                            int32_t leq_arg0_38049 = ((int32_t *) mem_param_39893.mem)[slice_38048];
                                            bool defunc_0_leq_res_38050 = sle32(leq_arg0_38049, leq_arg1_38040);
                                            
                                            leq_y_x_38034 = defunc_0_leq_res_38050;
                                        }
                                        
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_f_res_38051;
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_f_res_38052;
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_f_res_38053;
                                        
                                        if (leq_y_x_38034) {
                                            int64_t tmp_39392 = add64(tt_37986, n_38027);
                                            int64_t tmp_39393 = sub64(count_37988, n_38027);
                                            
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_38051 = tmp_39392;
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_38052 = tt_37987;
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_38053 = tmp_39393;
                                        } else {
                                            int64_t tmp_38056 = add64(tt_37986, n_38027);
                                            
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_38051 = tt_37986;
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_38052 = tmp_38056;
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_38053 = count_37988;
                                        }
                                        defunc_0_lifted_step_res_f_res_f_res_38004 = ss_37984;
                                        defunc_0_lifted_step_res_f_res_f_res_38005 = defunc_0_lifted_step_res_f_res_f_res_f_res_38051;
                                        defunc_0_lifted_step_res_f_res_f_res_38006 = defunc_0_lifted_step_res_f_res_f_res_f_res_38052;
                                        defunc_0_lifted_step_res_f_res_f_res_38007 = defunc_0_lifted_step_res_f_res_f_res_f_res_38053;
                                    }
                                    defunc_0_lifted_step_res_f_res_37998 = defunc_0_lifted_step_res_f_res_f_res_38004;
                                    defunc_0_lifted_step_res_f_res_37999 = defunc_0_lifted_step_res_f_res_f_res_38005;
                                    defunc_0_lifted_step_res_f_res_38000 = defunc_0_lifted_step_res_f_res_f_res_38006;
                                    defunc_0_lifted_step_res_f_res_38001 = defunc_0_lifted_step_res_f_res_f_res_38007;
                                }
                                defunc_0_lifted_step_res_37991 = defunc_0_lifted_step_res_f_res_37998;
                                defunc_0_lifted_step_res_37992 = defunc_0_lifted_step_res_f_res_37999;
                                defunc_0_lifted_step_res_37993 = defunc_0_lifted_step_res_f_res_38000;
                                defunc_0_lifted_step_res_37994 = defunc_0_lifted_step_res_f_res_38001;
                            }
                            
                            int64_t loopres_38057;
                            int64_t loopres_38058;
                            int64_t loopres_38059;
                            int64_t loopres_38060;
                            int64_t loopres_38061;
                            
                            if (futrts_lifted_normalizze_7743(ctx, &loopres_38057, &loopres_38058, &loopres_38059, &loopres_38060, &loopres_38061, defunc_0_lifted_step_res_37991, ss_37985, defunc_0_lifted_step_res_37992, defunc_0_lifted_step_res_37993, defunc_0_lifted_step_res_37994) != 0) {
                                err = 1;
                                goto cleanup;
                            }
                            
                            bool loop_cond_38062 = slt64((int64_t) 0, loopres_38061);
                            bool loop_while_tmp_40287 = loop_cond_38062;
                            int64_t ss_tmp_40288 = loopres_38057;
                            int64_t ss_tmp_40289 = loopres_38058;
                            int64_t tt_tmp_40290 = loopres_38059;
                            int64_t tt_tmp_40291 = loopres_38060;
                            int64_t count_tmp_40292 = loopres_38061;
                            
                            loop_while_37983 = loop_while_tmp_40287;
                            ss_37984 = ss_tmp_40288;
                            ss_37985 = ss_tmp_40289;
                            tt_37986 = tt_tmp_40290;
                            tt_37987 = tt_tmp_40291;
                            count_37988 = count_tmp_40292;
                        }
                        defunc_0_split_count_res_37977 = loop_while_37983;
                        defunc_0_split_count_res_37978 = ss_37984;
                        defunc_0_split_count_res_37979 = ss_37985;
                        defunc_0_split_count_res_37980 = tt_37986;
                        defunc_0_split_count_res_37981 = tt_37987;
                        defunc_0_split_count_res_37982 = count_37988;
                        ((int64_t *) mem_39912)[i_39530] = defunc_0_split_count_res_37978;
                        ((int64_t *) mem_39914)[i_39530] = defunc_0_split_count_res_37980;
                    }
                    if (mem_39929_cached_sizze_40356 < bytes_39928) {
                        err = lexical_realloc(ctx, &mem_39929, &mem_39929_cached_sizze_40356, bytes_39928);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    if (mem_39932_cached_sizze_40357 < bytes_39931) {
                        err = lexical_realloc(ctx, &mem_39932, &mem_39932_cached_sizze_40357, bytes_39931);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    for (int64_t i_39537 = 0; i_39537 < num_blocks_37838; i_39537++) {
                        bool y_38068 = slt64(i_39537, dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37839);
                        bool index_certs_38070;
                        
                        if (!y_38068) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_39537, "] out of bounds for array of shape [", (long long) dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37839, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:151:22-34\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  ../../../joins/ftSMJ.fut:104:60-63\n   #9  test_matching.fut:26:43-47\n   #10 test_matching.fut:19:1-28:44\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t defunc_0_f_res_38071 = ((int64_t *) mem_39912)[i_39537];
                        int64_t defunc_0_f_res_38072 = ((int64_t *) mem_39914)[i_39537];
                        int64_t tmp_38073 = add64((int64_t) 1, i_39537);
                        bool x_38074 = sle64((int64_t) 0, tmp_38073);
                        bool y_38075 = slt64(tmp_38073, dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37839);
                        bool bounds_check_38076 = x_38074 && y_38075;
                        bool index_certs_38077;
                        
                        if (!bounds_check_38076) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_38073, "] out of bounds for array of shape [", (long long) dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37839, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:152:22-36\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  ../../../joins/ftSMJ.fut:104:60-63\n   #9  test_matching.fut:26:43-47\n   #10 test_matching.fut:19:1-28:44\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t defunc_0_f_res_38078 = ((int64_t *) mem_39912)[tmp_38073];
                        int64_t defunc_0_f_res_38079 = ((int64_t *) mem_39914)[tmp_38073];
                        int64_t merge_sequential_arg2_38080 = add64(stride_37817, defunc_0_f_res_38072);
                        int64_t merge_sequential_arg2_38081 = add64(stride_37817, defunc_0_f_res_38079);
                        int64_t j_m_i_38082 = sub64(merge_sequential_arg2_38081, merge_sequential_arg2_38080);
                        bool empty_slice_38083 = j_m_i_38082 == (int64_t) 0;
                        int64_t m_38084 = sub64(j_m_i_38082, (int64_t) 1);
                        int64_t i_p_m_t_s_38085 = add64(merge_sequential_arg2_38080, m_38084);
                        bool zzero_leq_i_p_m_t_s_38086 = sle64((int64_t) 0, i_p_m_t_s_38085);
                        bool i_p_m_t_s_leq_w_38087 = slt64(i_p_m_t_s_38085, next_stride_37818);
                        bool zzero_lte_i_38088 = sle64((int64_t) 0, merge_sequential_arg2_38080);
                        bool i_lte_j_38089 = sle64(merge_sequential_arg2_38080, merge_sequential_arg2_38081);
                        bool y_38090 = i_p_m_t_s_leq_w_38087 && zzero_lte_i_38088;
                        bool y_38091 = zzero_leq_i_p_m_t_s_38086 && y_38090;
                        bool forwards_ok_38092 = i_lte_j_38089 && y_38091;
                        bool ok_or_empty_38093 = empty_slice_38083 || forwards_ok_38092;
                        bool index_certs_38094;
                        
                        if (!ok_or_empty_38093) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) merge_sequential_arg2_38080, ":", (long long) merge_sequential_arg2_38081, "] out of bounds for array of shape [", (long long) next_stride_37818, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:40-58\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  ../../../joins/ftSMJ.fut:104:60-63\n   #9  test_matching.fut:26:43-47\n   #10 test_matching.fut:19:1-28:44\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t j_m_i_38095 = sub64(defunc_0_f_res_38078, defunc_0_f_res_38071);
                        bool empty_slice_38096 = j_m_i_38095 == (int64_t) 0;
                        int64_t m_38097 = sub64(j_m_i_38095, (int64_t) 1);
                        int64_t i_p_m_t_s_38098 = add64(defunc_0_f_res_38071, m_38097);
                        bool zzero_leq_i_p_m_t_s_38099 = sle64((int64_t) 0, i_p_m_t_s_38098);
                        bool i_p_m_t_s_leq_w_38100 = slt64(i_p_m_t_s_38098, next_stride_37818);
                        bool zzero_lte_i_38101 = sle64((int64_t) 0, defunc_0_f_res_38071);
                        bool i_lte_j_38102 = sle64(defunc_0_f_res_38071, defunc_0_f_res_38078);
                        bool y_38103 = i_p_m_t_s_leq_w_38100 && zzero_lte_i_38101;
                        bool y_38104 = zzero_leq_i_p_m_t_s_38099 && y_38103;
                        bool forwards_ok_38105 = i_lte_j_38102 && y_38104;
                        bool ok_or_empty_38106 = empty_slice_38096 || forwards_ok_38105;
                        bool index_certs_38107;
                        
                        if (!ok_or_empty_38106) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) defunc_0_f_res_38071, ":", (long long) defunc_0_f_res_38078, "] out of bounds for array of shape [", (long long) next_stride_37818, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:29-39\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  ../../../joins/ftSMJ.fut:104:60-63\n   #9  test_matching.fut:26:43-47\n   #10 test_matching.fut:19:1-28:44\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        bool cond_38108 = slt64((int64_t) 0, j_m_i_38095);
                        int32_t dummy_38109;
                        int64_t dummy_38110;
                        
                        if (cond_38108) {
                            bool index_certs_39394;
                            
                            if (!cond_38108) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) j_m_i_38095, "].", "-> #0  /prelude/array.fut:22:29-33\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:100:36-42\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #6  /prelude/soacs.fut:59:9-11\n   #7  /prelude/array.fut:241:11-17\n   #8  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #9  /prelude/functional.fut:9:44-45\n   #10 ../../../joins/ftSMJ.fut:104:60-63\n   #11 test_matching.fut:26:43-47\n   #12 test_matching.fut:19:1-28:44\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t slice_39395 = start_37890 + defunc_0_f_res_38071;
                            int32_t head_res_39396 = ((int32_t *) mem_param_39893.mem)[slice_39395];
                            int64_t head_res_39397 = ((int64_t *) mem_param_39896.mem)[slice_39395];
                            
                            dummy_38109 = head_res_39396;
                            dummy_38110 = head_res_39397;
                        } else {
                            bool y_38115 = slt64((int64_t) 0, j_m_i_38082);
                            bool index_certs_38116;
                            
                            if (!y_38115) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) j_m_i_38082, "].", "-> #0  /prelude/array.fut:22:29-33\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:100:48-54\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #6  /prelude/soacs.fut:59:9-11\n   #7  /prelude/array.fut:241:11-17\n   #8  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #9  /prelude/functional.fut:9:44-45\n   #10 ../../../joins/ftSMJ.fut:104:60-63\n   #11 test_matching.fut:26:43-47\n   #12 test_matching.fut:19:1-28:44\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t slice_38117 = start_37890 + merge_sequential_arg2_38080;
                            int32_t head_res_38118 = ((int32_t *) mem_param_39893.mem)[slice_38117];
                            int64_t head_res_38119 = ((int64_t *) mem_param_39896.mem)[slice_38117];
                            
                            dummy_38109 = head_res_38118;
                            dummy_38110 = head_res_38119;
                        }
                        for (int64_t nest_i_40295 = 0; nest_i_40295 < merge_block_sizze_37703; nest_i_40295++) {
                            ((int32_t *) mem_39929)[i_39537 * merge_block_sizze_37703 + nest_i_40295] = dummy_38109;
                        }
                        for (int64_t nest_i_40296 = 0; nest_i_40296 < merge_block_sizze_37703; nest_i_40296++) {
                            ((int64_t *) mem_39932)[i_39537 * merge_block_sizze_37703 + nest_i_40296] = dummy_38110;
                        }
                        
                        int64_t data_38122;
                        int64_t i_38126 = (int64_t) 0;
                        
                        for (int64_t k_38125 = 0; k_38125 < merge_block_sizze_37703; k_38125++) {
                            int64_t j_38129 = sub64(k_38125, i_38126);
                            bool cond_38130 = j_38129 == j_m_i_38082;
                            bool cond_38131;
                            
                            if (cond_38130) {
                                cond_38131 = 1;
                            } else {
                                bool cond_38132 = slt64(i_38126, j_m_i_38095);
                                bool cond_f_res_38133;
                                
                                if (cond_38132) {
                                    bool x_39398 = sle64((int64_t) 0, j_38129);
                                    bool y_39399 = slt64(j_38129, j_m_i_38082);
                                    bool bounds_check_39400 = x_39398 && y_39399;
                                    bool index_certs_39401;
                                    
                                    if (!bounds_check_39400) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_38129, "] out of bounds for array of shape [", (long long) j_m_i_38082, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:55-59\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  ../../../joins/ftSMJ.fut:104:60-63\n   #10 test_matching.fut:26:43-47\n   #11 test_matching.fut:19:1-28:44\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    bool x_39405 = sle64((int64_t) 0, i_38126);
                                    bool bounds_check_39406 = cond_38132 && x_39405;
                                    bool index_certs_39407;
                                    
                                    if (!bounds_check_39406) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_38126, "] out of bounds for array of shape [", (long long) j_m_i_38095, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:50-54\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  ../../../joins/ftSMJ.fut:104:60-63\n   #10 test_matching.fut:26:43-47\n   #11 test_matching.fut:19:1-28:44\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    int64_t slice_39402 = merge_sequential_arg2_38080 + j_38129;
                                    int64_t slice_39403 = start_37890 + slice_39402;
                                    int32_t leq_arg1_39404 = ((int32_t *) mem_param_39893.mem)[slice_39403];
                                    int64_t slice_39408 = defunc_0_f_res_38071 + i_38126;
                                    int64_t slice_39409 = start_37890 + slice_39408;
                                    int32_t leq_arg0_39410 = ((int32_t *) mem_param_39893.mem)[slice_39409];
                                    bool defunc_0_leq_res_39411 = sle32(leq_arg0_39410, leq_arg1_39404);
                                    
                                    cond_f_res_38133 = defunc_0_leq_res_39411;
                                } else {
                                    cond_f_res_38133 = 0;
                                }
                                cond_38131 = cond_f_res_38133;
                            }
                            
                            int64_t loopres_38148;
                            int32_t loopres_38149;
                            int64_t loopres_38150;
                            
                            if (cond_38131) {
                                bool x_39413 = sle64((int64_t) 0, i_38126);
                                bool y_39414 = slt64(i_38126, j_m_i_38095);
                                bool bounds_check_39415 = x_39413 && y_39414;
                                bool index_certs_39416;
                                
                                if (!bounds_check_39415) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_38126, "] out of bounds for array of shape [", (long long) j_m_i_38095, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:106:17-21\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  ../../../joins/ftSMJ.fut:104:60-63\n   #10 test_matching.fut:26:43-47\n   #11 test_matching.fut:19:1-28:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t tmp_39412 = add64((int64_t) 1, i_38126);
                                int64_t slice_39417 = defunc_0_f_res_38071 + i_38126;
                                int64_t slice_39418 = start_37890 + slice_39417;
                                int32_t tmp_39419 = ((int32_t *) mem_param_39893.mem)[slice_39418];
                                int64_t tmp_39420 = ((int64_t *) mem_param_39896.mem)[slice_39418];
                                
                                loopres_38148 = tmp_39412;
                                loopres_38149 = tmp_39419;
                                loopres_38150 = tmp_39420;
                            } else {
                                bool x_38160 = sle64((int64_t) 0, j_38129);
                                bool y_38161 = slt64(j_38129, j_m_i_38082);
                                bool bounds_check_38162 = x_38160 && y_38161;
                                bool index_certs_38163;
                                
                                if (!bounds_check_38162) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_38129, "] out of bounds for array of shape [", (long long) j_m_i_38082, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:108:15-19\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  ../../../joins/ftSMJ.fut:104:60-63\n   #10 test_matching.fut:26:43-47\n   #11 test_matching.fut:19:1-28:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t slice_38164 = merge_sequential_arg2_38080 + j_38129;
                                int64_t slice_38165 = start_37890 + slice_38164;
                                int32_t tmp_38166 = ((int32_t *) mem_param_39893.mem)[slice_38165];
                                int64_t tmp_38167 = ((int64_t *) mem_param_39896.mem)[slice_38165];
                                
                                loopres_38148 = i_38126;
                                loopres_38149 = tmp_38166;
                                loopres_38150 = tmp_38167;
                            }
                            ((int32_t *) mem_39929)[i_39537 * merge_block_sizze_37703 + k_38125] = loopres_38149;
                            ((int64_t *) mem_39932)[i_39537 * merge_block_sizze_37703 + k_38125] = loopres_38150;
                            
                            int64_t i_tmp_40297 = loopres_38148;
                            
                            i_38126 = i_tmp_40297;
                        }
                        data_38122 = i_38126;
                    }
                    if (memblock_alloc(ctx, &mem_39967, bytes_39928, "mem_39967")) {
                        err = 1;
                        goto cleanup;
                    }
                    lmad_copy_4b(ctx, 2, (uint32_t *) mem_39967.mem, (int64_t) 0, (int64_t []) {merge_block_sizze_37703, (int64_t) 1}, (uint32_t *) mem_39929, (int64_t) 0, (int64_t []) {merge_block_sizze_37703, (int64_t) 1}, (int64_t []) {num_blocks_37838, merge_block_sizze_37703});
                    if (memblock_alloc(ctx, &mem_39971, bytes_39931, "mem_39971")) {
                        err = 1;
                        goto cleanup;
                    }
                    lmad_copy_8b(ctx, 2, (uint64_t *) mem_39971.mem, (int64_t) 0, (int64_t []) {merge_block_sizze_37703, (int64_t) 1}, (uint64_t *) mem_39932, (int64_t) 0, (int64_t []) {merge_block_sizze_37703, (int64_t) 1}, (int64_t []) {num_blocks_37838, merge_block_sizze_37703});
                    if (memblock_set(ctx, &ext_mem_39994, &mem_39967, "mem_39967") != 0)
                        return 1;
                    if (memblock_set(ctx, &ext_mem_39991, &mem_39971, "mem_39971") != 0)
                        return 1;
                }
                lmad_copy_4b(ctx, 1, (uint32_t *) mem_39899, i_39544 * next_stride_37818, (int64_t []) {(int64_t) 1}, (uint32_t *) ext_mem_39994.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {next_stride_37818});
                if (memblock_unref(ctx, &ext_mem_39994, "ext_mem_39994") != 0)
                    return 1;
                lmad_copy_8b(ctx, 1, (uint64_t *) mem_39902, i_39544 * next_stride_37818, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_39991.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {next_stride_37818});
                if (memblock_unref(ctx, &ext_mem_39991, "ext_mem_39991") != 0)
                    return 1;
            }
            
            int64_t flat_dim_38174 = next_stride_37818 * num_merges_37822;
            
            if (memblock_alloc(ctx, &mem_40005, bytes_39898, "mem_40005")) {
                err = 1;
                goto cleanup;
            }
            lmad_copy_4b(ctx, 2, (uint32_t *) mem_40005.mem, (int64_t) 0, (int64_t []) {next_stride_37818, (int64_t) 1}, (uint32_t *) mem_39899, (int64_t) 0, (int64_t []) {next_stride_37818, (int64_t) 1}, (int64_t []) {num_merges_37822, next_stride_37818});
            if (memblock_alloc(ctx, &mem_40009, bytes_39901, "mem_40009")) {
                err = 1;
                goto cleanup;
            }
            lmad_copy_8b(ctx, 2, (uint64_t *) mem_40009.mem, (int64_t) 0, (int64_t []) {next_stride_37818, (int64_t) 1}, (uint64_t *) mem_39902, (int64_t) 0, (int64_t []) {next_stride_37818, (int64_t) 1}, (int64_t []) {num_merges_37822, next_stride_37818});
            
            bool loop_cond_38177 = slt64(next_stride_37818, flat_dim_37699);
            
            if (memblock_set(ctx, &mem_param_tmp_40271, &mem_40005, "mem_40005") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_tmp_40272, &mem_40009, "mem_40009") != 0)
                return 1;
            
            int64_t loop_dz2084Uz2082U_tmp_40273 = flat_dim_38174;
            bool loop_while_tmp_40274 = loop_cond_38177;
            int64_t stride_tmp_40277 = next_stride_37818;
            
            if (memblock_set(ctx, &mem_param_39893, &mem_param_tmp_40271, "mem_param_tmp_40271") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_39896, &mem_param_tmp_40272, "mem_param_tmp_40272") != 0)
                return 1;
            loop_dz2084Uz2082U_37813 = loop_dz2084Uz2082U_tmp_40273;
            loop_while_37814 = loop_while_tmp_40274;
            stride_37817 = stride_tmp_40277;
        }
        if (memblock_set(ctx, &ext_mem_40016, &mem_param_39893, "mem_param_39893") != 0)
            return 1;
        if (memblock_set(ctx, &ext_mem_40015, &mem_param_39896, "mem_param_39896") != 0)
            return 1;
        data_37808 = loop_dz2084Uz2082U_37813;
        data_37809 = loop_while_37814;
        data_37812 = stride_37817;
        if (memblock_unref(ctx, &mem_39885, "mem_39885") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39889, "mem_39889") != 0)
            return 1;
        
        bool empty_slice_38178 = n2_24949 == (int64_t) 0;
        bool zzero_leq_i_p_m_t_s_38179 = sle64((int64_t) 0, zs_lhs_37683);
        bool i_p_m_t_s_leq_w_38180 = slt64(zs_lhs_37683, data_37808);
        bool y_38181 = zzero_leq_i_p_m_t_s_38179 && i_p_m_t_s_leq_w_38180;
        bool ok_or_empty_38182 = empty_slice_38178 || y_38181;
        bool index_certs_38183;
        
        if (!ok_or_empty_38182) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) n2_24949, "] out of bounds for array of shape [", (long long) data_37808, "].", "-> #0  /prelude/array.fut:46:45-51\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:265:3-14\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #3  /prelude/functional.fut:9:44-45\n   #4  ../../../joins/ftSMJ.fut:104:60-63\n   #5  test_matching.fut:26:43-47\n   #6  test_matching.fut:19:1-28:44\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        if (memblock_set(ctx, &ext_mem_40019, &ext_mem_40015, "ext_mem_40015") != 0)
            return 1;
    }
    if (memblock_unref(ctx, &mem_39814, "mem_39814") != 0)
        return 1;
    if (mem_40021_cached_sizze_40358 < bytes_40020) {
        err = lexical_realloc(ctx, &mem_40021, &mem_40021_cached_sizze_40358, bytes_40020);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    for (int64_t i_39549 = 0; i_39549 < n2_24949; i_39549++) {
        int64_t eta_p_38191 = ((int64_t *) ext_mem_40019.mem)[i_39549];
        bool x_38192 = sle64((int64_t) 0, eta_p_38191);
        bool y_38193 = slt64(eta_p_38191, n2_24949);
        bool bounds_check_38194 = x_38192 && y_38193;
        bool index_certs_38195;
        
        if (!bounds_check_38194) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_38191, "] out of bounds for array of shape [", (long long) n2_24949, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:277:24-28\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:277:8-29\n   #3  ../../../joins/ftSMJ.fut:104:60-63\n   #4  test_matching.fut:26:43-47\n   #5  test_matching.fut:19:1-28:44\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int32_t lifted_lambda_res_38196 = ((int32_t *) ks2_mem_39594.mem)[eta_p_38191];
        
        ((int32_t *) mem_40021)[i_39549] = lifted_lambda_res_38196;
    }
    if (memblock_unref(ctx, &ext_mem_40019, "ext_mem_40019") != 0)
        return 1;
    
    int64_t zm_lhs_38207 = add64((int64_t) 2305843009213693951, n1_24948);
    int64_t zs_lhs_38208 = sub64(zm_lhs_38207, (int64_t) 1);
    int64_t numIter_R_38212 = sdiv64(zs_lhs_38208, (int64_t) 2305843009213693951);
    int64_t zm_lhs_38213 = add64((int64_t) 2305843009213693951, n2_24949);
    int64_t zs_lhs_38214 = sub64(zm_lhs_38213, (int64_t) 1);
    int64_t numIter_S_38215 = sdiv64(zs_lhs_38214, (int64_t) 2305843009213693951);
    
    if (memblock_alloc(ctx, &mem_40029, bytes_39803, "mem_40029")) {
        err = 1;
        goto cleanup;
    }
    lmad_copy_4b(ctx, 1, (uint32_t *) mem_40029.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint32_t *) mem_39804, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {n1_24948});
    if (futrts_indicesWithIncrement_7869(ctx, &ext_mem_40030, mem_39812, n1_24948, (int64_t) 0) != 0) {
        err = 1;
        goto cleanup;
    }
    if (memblock_unref(ctx, &mem_39812, "mem_39812") != 0)
        return 1;
    if (memblock_alloc(ctx, &mem_40032, bytes_39596, "mem_40032")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t nest_i_40301 = 0; nest_i_40301 < n1_24948; nest_i_40301++) {
        ((int64_t *) mem_40032.mem)[nest_i_40301] = (int64_t) -1;
    }
    if (memblock_alloc(ctx, &mem_40034, bytes_39596, "mem_40034")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t nest_i_40302 = 0; nest_i_40302 < n1_24948; nest_i_40302++) {
        ((int64_t *) mem_40034.mem)[nest_i_40302] = (int64_t) 0;
    }
    
    bool loop_cond_38220 = slt64((int64_t) 0, numIter_R_38212);
    bool defunc_0_mergeJoin_res_38221;
    int64_t defunc_0_mergeJoin_res_38222;
    int64_t defunc_0_mergeJoin_res_38225;
    bool loop_while_38226;
    int64_t iter_38227;
    int64_t first_relevant_in_S_38230;
    
    if (memblock_set(ctx, &mem_param_40037, &mem_40034, "mem_40034") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_40040, &mem_40032, "mem_40032") != 0)
        return 1;
    loop_while_38226 = loop_cond_38220;
    iter_38227 = (int64_t) 0;
    first_relevant_in_S_38230 = (int64_t) 0;
    while (loop_while_38226) {
        int64_t tR_start_38231 = mul64((int64_t) 2305843009213693951, iter_38227);
        int64_t min_arg1_38232 = add64((int64_t) 2305843009213693951, tR_start_38231);
        int64_t min_res_38233 = smin64(n1_24948, min_arg1_38232);
        int64_t dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234 = sub64(min_res_38233, tR_start_38231);
        bool empty_slice_38235 = dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234 == (int64_t) 0;
        int64_t m_38236 = sub64(dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234, (int64_t) 1);
        int64_t i_p_m_t_s_38237 = add64(tR_start_38231, m_38236);
        bool zzero_leq_i_p_m_t_s_38238 = sle64((int64_t) 0, i_p_m_t_s_38237);
        bool i_p_m_t_s_leq_w_38239 = slt64(i_p_m_t_s_38237, n1_24948);
        bool zzero_lte_i_38240 = sle64((int64_t) 0, tR_start_38231);
        bool i_lte_j_38241 = sle64(tR_start_38231, min_res_38233);
        bool y_38242 = i_p_m_t_s_leq_w_38239 && zzero_lte_i_38240;
        bool y_38243 = zzero_leq_i_p_m_t_s_38238 && y_38242;
        bool forwards_ok_38244 = i_lte_j_38241 && y_38243;
        bool ok_or_empty_38245 = empty_slice_38235 || forwards_ok_38244;
        bool index_certs_38246;
        
        if (!ok_or_empty_38245) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tR_start_38231, ":", (long long) min_res_38233, "] out of bounds for array of shape [", (long long) n1_24948, "].", "-> #0  ../../../joins/ftSMJ.fut:288:19-38\n   #1  test_matching.fut:27:79-82\n   #2  test_matching.fut:19:1-28:44\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_40041 = (int64_t) 8 * dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234;
        bool cond_38250 = slt64(first_relevant_in_S_38230, numIter_S_38215);
        bool y_38255 = slt64((int64_t) 0, dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234);
        bool loop_not_taken_38256 = !cond_38250;
        bool protect_assert_disj_38257 = y_38255 || loop_not_taken_38256;
        bool index_certs_38258;
        
        if (!protect_assert_disj_38257) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234, "].", "-> #0  ../../../joins/ftSMJ.fut:303:28-36\n   #1  test_matching.fut:27:79-82\n   #2  test_matching.fut:19:1-28:44\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        bool x_38261 = sle64((int64_t) 0, m_38236);
        bool y_38262 = slt64(m_38236, dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234);
        bool bounds_check_38263 = x_38261 && y_38262;
        bool protect_assert_disj_38264 = loop_not_taken_38256 || bounds_check_38263;
        bool index_certs_38265;
        
        if (!protect_assert_disj_38264) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) m_38236, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234, "].", "-> #0  ../../../joins/ftSMJ.fut:304:30-49\n   #1  test_matching.fut:27:79-82\n   #2  test_matching.fut:19:1-28:44\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        bool index_certs_38476;
        
        if (!ok_or_empty_38245) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tR_start_38231, ":", (long long) min_res_38233, "] out of bounds for array of shape [", (long long) n1_24948, "].", "-> #0  ../../../joins/ftSMJ.fut:198:17-65\n   #1  ../../../joins/ftSMJ.fut:314:22-66\n   #2  test_matching.fut:27:79-82\n   #3  test_matching.fut:19:1-28:44\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        if (memblock_alloc(ctx, &mem_40042, bytes_40041, "mem_40042")) {
            err = 1;
            goto cleanup;
        }
        for (int64_t nest_i_40310 = 0; nest_i_40310 < dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234; nest_i_40310++) {
            ((int64_t *) mem_40042.mem)[nest_i_40310] = (int64_t) -1;
        }
        if (memblock_alloc(ctx, &mem_40044, bytes_40041, "mem_40044")) {
            err = 1;
            goto cleanup;
        }
        for (int64_t nest_i_40311 = 0; nest_i_40311 < dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234; nest_i_40311++) {
            ((int64_t *) mem_40044.mem)[nest_i_40311] = (int64_t) 0;
        }
        
        int32_t gt_lhs_38259;
        
        if (cond_38250) {
            int32_t x_39431 = ((int32_t *) mem_39804)[tR_start_38231];
            
            gt_lhs_38259 = x_39431;
        } else {
            gt_lhs_38259 = 0;
        }
        
        int32_t lt_lhs_38266;
        
        if (cond_38250) {
            int32_t x_39432 = ((int32_t *) mem_39804)[i_p_m_t_s_38237];
            
            lt_lhs_38266 = x_39432;
        } else {
            lt_lhs_38266 = 0;
        }
        
        int64_t ext_40110;
        int64_t ext_40109;
        int64_t ext_40108;
        int64_t ext_40107;
        bool loopres_38268;
        int64_t loopres_38269;
        int64_t loopres_38272;
        bool loop_while_38273;
        int64_t s_iter_38274;
        int64_t minRelevant_38277;
        int64_t ctx_param_ext_40045;
        int64_t ctx_param_ext_40046;
        int64_t ctx_param_ext_40048;
        int64_t ctx_param_ext_40049;
        
        if (memblock_set(ctx, &mem_param_40047, &mem_40044, "mem_40044") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_40050, &mem_40042, "mem_40042") != 0)
            return 1;
        ctx_param_ext_40045 = (int64_t) 0;
        ctx_param_ext_40046 = (int64_t) 1;
        ctx_param_ext_40048 = (int64_t) 0;
        ctx_param_ext_40049 = (int64_t) 1;
        loop_while_38273 = cond_38250;
        s_iter_38274 = first_relevant_in_S_38230;
        minRelevant_38277 = first_relevant_in_S_38230;
        while (loop_while_38273) {
            int64_t tS_start_38278 = mul64((int64_t) 2305843009213693951, s_iter_38274);
            int64_t min_arg1_38279 = add64((int64_t) 2305843009213693951, tS_start_38278);
            int64_t min_res_38280 = smin64(n2_24949, min_arg1_38279);
            int64_t dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281 = sub64(min_res_38280, tS_start_38278);
            bool empty_slice_38282 = dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281 == (int64_t) 0;
            int64_t m_38283 = sub64(dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281, (int64_t) 1);
            int64_t i_p_m_t_s_38284 = add64(tS_start_38278, m_38283);
            bool zzero_leq_i_p_m_t_s_38285 = sle64((int64_t) 0, i_p_m_t_s_38284);
            bool i_p_m_t_s_leq_w_38286 = slt64(i_p_m_t_s_38284, n2_24949);
            bool zzero_lte_i_38287 = sle64((int64_t) 0, tS_start_38278);
            bool i_lte_j_38288 = sle64(tS_start_38278, min_res_38280);
            bool y_38289 = i_p_m_t_s_leq_w_38286 && zzero_lte_i_38287;
            bool y_38290 = zzero_leq_i_p_m_t_s_38285 && y_38289;
            bool forwards_ok_38291 = i_lte_j_38288 && y_38290;
            bool ok_or_empty_38292 = empty_slice_38282 || forwards_ok_38291;
            bool index_certs_38293;
            
            if (!ok_or_empty_38292) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tS_start_38278, ":", (long long) min_res_38280, "] out of bounds for array of shape [", (long long) n2_24949, "].", "-> #0  ../../../joins/ftSMJ.fut:301:21-40\n   #1  test_matching.fut:27:79-82\n   #2  test_matching.fut:19:1-28:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool x_38295 = sle64((int64_t) 0, m_38283);
            bool y_38296 = slt64(m_38283, dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281);
            bool bounds_check_38297 = x_38295 && y_38296;
            bool index_certs_38298;
            
            if (!bounds_check_38297) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) m_38283, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281, "].", "-> #0  ../../../joins/ftSMJ.fut:303:42-61\n   #1  test_matching.fut:27:79-82\n   #2  test_matching.fut:19:1-28:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool y_38301 = slt64((int64_t) 0, dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281);
            bool index_certs_38302;
            
            if (!y_38301) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281, "].", "-> #0  ../../../joins/ftSMJ.fut:304:55-63\n   #1  test_matching.fut:27:79-82\n   #2  test_matching.fut:19:1-28:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int32_t gt_rhs_38299 = ((int32_t *) mem_40021)[i_p_m_t_s_38284];
            bool defunc_0_gt_res_38300 = slt32(gt_rhs_38299, gt_lhs_38259);
            int32_t lt_rhs_38303 = ((int32_t *) mem_40021)[tS_start_38278];
            bool defunc_0_lt_res_38304 = slt32(lt_lhs_38266, lt_rhs_38303);
            bool thisStillRel_38305 = !defunc_0_lt_res_38304;
            int64_t ext_40099;
            
            if (defunc_0_lt_res_38304) {
                ext_40099 = ctx_param_ext_40045;
            } else {
                ext_40099 = (int64_t) 0;
            }
            
            int64_t ext_40098;
            
            if (defunc_0_lt_res_38304) {
                ext_40098 = ctx_param_ext_40046;
            } else {
                ext_40098 = (int64_t) 1;
            }
            
            int64_t ext_40096;
            
            if (defunc_0_lt_res_38304) {
                ext_40096 = ctx_param_ext_40048;
            } else {
                ext_40096 = (int64_t) 0;
            }
            
            int64_t ext_40095;
            
            if (defunc_0_lt_res_38304) {
                ext_40095 = ctx_param_ext_40049;
            } else {
                ext_40095 = (int64_t) 1;
            }
            
            int64_t ext_40105;
            
            if (defunc_0_gt_res_38300) {
                ext_40105 = ctx_param_ext_40045;
            } else {
                ext_40105 = ext_40099;
            }
            
            int64_t ext_40104;
            
            if (defunc_0_gt_res_38300) {
                ext_40104 = ctx_param_ext_40046;
            } else {
                ext_40104 = ext_40098;
            }
            
            int64_t ext_40102;
            
            if (defunc_0_gt_res_38300) {
                ext_40102 = ctx_param_ext_40048;
            } else {
                ext_40102 = ext_40096;
            }
            
            int64_t ext_40101;
            
            if (defunc_0_gt_res_38300) {
                ext_40101 = ctx_param_ext_40049;
            } else {
                ext_40101 = ext_40095;
            }
            
            int64_t loopres_38306;
            bool loopres_38309;
            int64_t loopres_38310;
            
            if (defunc_0_gt_res_38300) {
                int64_t tmp_39433 = add64((int64_t) 1, s_iter_38274);
                
                if (memblock_set(ctx, &ext_mem_40106, &mem_param_40047, "mem_param_40047") != 0)
                    return 1;
                if (memblock_set(ctx, &ext_mem_40103, &mem_param_40050, "mem_param_40050") != 0)
                    return 1;
                loopres_38306 = tmp_39433;
                loopres_38309 = 1;
                loopres_38310 = tmp_39433;
            } else {
                int64_t loopres_f_res_38312;
                bool loopres_f_res_38315;
                
                if (defunc_0_lt_res_38304) {
                    if (memblock_set(ctx, &ext_mem_40100, &mem_param_40047, "mem_param_40047") != 0)
                        return 1;
                    if (memblock_set(ctx, &ext_mem_40097, &mem_param_40050, "mem_param_40050") != 0)
                        return 1;
                    loopres_f_res_38312 = s_iter_38274;
                    loopres_f_res_38315 = thisStillRel_38305;
                } else {
                    bool index_certs_38316;
                    
                    if (!bounds_check_38263) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) m_38236, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234, "].", "-> #0  ../../../joins/ftSMJ.fut:308:27-46\n   #1  test_matching.fut:27:79-82\n   #2  test_matching.fut:19:1-28:44\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int32_t lt_lhs_38317 = ((int32_t *) mem_39804)[i_p_m_t_s_38237];
                    bool defunc_0_lt_res_38319 = slt32(lt_lhs_38317, gt_rhs_38299);
                    bool nextRel_38320 = !defunc_0_lt_res_38319;
                    double i64_res_38322 = sitofp_i64_f64(dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281);
                    double log2_res_38323 = futrts_log2_64(i64_res_38322);
                    double ceil_res_38324 = futrts_ceil64(log2_res_38323);
                    int64_t f64_res_38325 = fptosi_f64_i64(ceil_res_38324);
                    int64_t num_iter_38326 = add64((int64_t) 1, f64_res_38325);
                    int64_t bsearch_first_38329;
                    int64_t last_step_38332;
                    
                    if (memblock_set(ctx, &mem_param_40053, &mem_40044, "mem_40044") != 0)
                        return 1;
                    last_step_38332 = dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281;
                    for (int64_t i_38330 = 0; i_38330 < num_iter_38326; i_38330++) {
                        int64_t zs_lhs_38333 = add64((int64_t) 1, last_step_38332);
                        int64_t this_step_38334 = sdiv64(zs_lhs_38333, (int64_t) 2);
                        
                        if (memblock_alloc(ctx, &mem_40055, bytes_40041, "mem_40055")) {
                            err = 1;
                            goto cleanup;
                        }
                        for (int64_t i_39553 = 0; i_39553 < dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234; i_39553++) {
                            int64_t eta_p_38840 = ((int64_t *) mem_param_40053.mem)[i_39553];
                            int64_t slice_39567 = tR_start_38231 + i_39553;
                            int32_t eta_p_38841 = ((int32_t *) mem_39804)[slice_39567];
                            bool cond_38843 = sle64(eta_p_38840, (int64_t) 0);
                            int32_t prev_elem_38844;
                            
                            if (cond_38843) {
                                bool index_certs_39435;
                                
                                if (!y_38301) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281, "].", "-> #0  ../../../joins/ftSMJ.fut:219:42-47\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftSMJ.fut:218:14-222:12\n   #3  test_matching.fut:27:79-82\n   #4  test_matching.fut:19:1-28:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                prev_elem_38844 = lt_rhs_38303;
                            } else {
                                int64_t tmp_38848 = sub64(eta_p_38840, (int64_t) 1);
                                bool x_38849 = sle64((int64_t) 0, tmp_38848);
                                bool y_38850 = slt64(tmp_38848, dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281);
                                bool bounds_check_38851 = x_38849 && y_38850;
                                bool index_certs_38852;
                                
                                if (!bounds_check_38851) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_38848, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281, "].", "-> #0  ../../../joins/ftSMJ.fut:219:53-60\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftSMJ.fut:218:14-222:12\n   #3  test_matching.fut:27:79-82\n   #4  test_matching.fut:19:1-28:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t slice_38853 = tS_start_38278 + tmp_38848;
                                int32_t prev_elem_f_res_38854 = ((int32_t *) mem_40021)[slice_38853];
                                
                                prev_elem_38844 = prev_elem_f_res_38854;
                            }
                            
                            bool cond_38855 = slt64(eta_p_38840, (int64_t) 0);
                            int32_t cur_elem_38856;
                            
                            if (cond_38855) {
                                bool index_certs_39438;
                                
                                if (!y_38301) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281, "].", "-> #0  ../../../joins/ftSMJ.fut:220:40-45\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftSMJ.fut:218:14-222:12\n   #3  test_matching.fut:27:79-82\n   #4  test_matching.fut:19:1-28:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                cur_elem_38856 = lt_rhs_38303;
                            } else {
                                bool x_38860 = sle64((int64_t) 0, eta_p_38840);
                                bool y_38861 = slt64(eta_p_38840, dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281);
                                bool bounds_check_38862 = x_38860 && y_38861;
                                bool index_certs_38863;
                                
                                if (!bounds_check_38862) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_38840, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281, "].", "-> #0  ../../../joins/ftSMJ.fut:220:51-56\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftSMJ.fut:218:14-222:12\n   #3  test_matching.fut:27:79-82\n   #4  test_matching.fut:19:1-28:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t slice_38864 = tS_start_38278 + eta_p_38840;
                                int32_t cur_elem_f_res_38865 = ((int32_t *) mem_40021)[slice_38864];
                                
                                cur_elem_38856 = cur_elem_f_res_38865;
                            }
                            
                            int64_t defunc_0_f_res_38869;
                            
                            if (cond_38855) {
                                defunc_0_f_res_38869 = (int64_t) -1;
                            } else {
                                bool defunc_0_eq_res_38870 = eta_p_38841 == cur_elem_38856;
                                bool cond_38871 = eta_p_38840 == (int64_t) 0;
                                bool defunc_0_gt_res_38872 = slt32(prev_elem_38844, eta_p_38841);
                                bool x_38873 = !cond_38871;
                                bool y_38874 = defunc_0_gt_res_38872 && x_38873;
                                bool cond_t_res_38875 = cond_38871 || y_38874;
                                bool x_38876 = defunc_0_eq_res_38870 && cond_t_res_38875;
                                int64_t defunc_0_f_res_f_res_38877;
                                
                                if (x_38876) {
                                    defunc_0_f_res_f_res_38877 = eta_p_38840;
                                } else {
                                    int64_t defunc_0_f_res_f_res_f_res_38878;
                                    
                                    if (defunc_0_eq_res_38870) {
                                        int64_t max_arg1_39440 = sub64(eta_p_38840, this_step_38334);
                                        int64_t max_res_39441 = smax64((int64_t) 0, max_arg1_39440);
                                        
                                        defunc_0_f_res_f_res_f_res_38878 = max_res_39441;
                                    } else {
                                        bool defunc_0_gt_res_38881 = slt32(cur_elem_38856, eta_p_38841);
                                        int64_t defunc_0_f_res_f_res_f_res_f_res_38882;
                                        
                                        if (defunc_0_gt_res_38881) {
                                            bool cond_39443 = eta_p_38840 == m_38283;
                                            int64_t defunc_0_f_res_f_res_f_res_f_res_t_res_39444;
                                            
                                            if (cond_39443) {
                                                defunc_0_f_res_f_res_f_res_f_res_t_res_39444 = (int64_t) -1;
                                            } else {
                                                int64_t min_arg1_39445 = add64(this_step_38334, eta_p_38840);
                                                int64_t min_res_39446 = smin64(m_38283, min_arg1_39445);
                                                
                                                defunc_0_f_res_f_res_f_res_f_res_t_res_39444 = min_res_39446;
                                            }
                                            defunc_0_f_res_f_res_f_res_f_res_38882 = defunc_0_f_res_f_res_f_res_f_res_t_res_39444;
                                        } else {
                                            int64_t defunc_0_f_res_f_res_f_res_f_res_f_res_38888;
                                            
                                            if (cond_t_res_38875) {
                                                defunc_0_f_res_f_res_f_res_f_res_f_res_38888 = (int64_t) -1;
                                            } else {
                                                int64_t max_arg1_38889 = sub64(eta_p_38840, this_step_38334);
                                                int64_t max_res_38890 = smax64((int64_t) 0, max_arg1_38889);
                                                
                                                defunc_0_f_res_f_res_f_res_f_res_f_res_38888 = max_res_38890;
                                            }
                                            defunc_0_f_res_f_res_f_res_f_res_38882 = defunc_0_f_res_f_res_f_res_f_res_f_res_38888;
                                        }
                                        defunc_0_f_res_f_res_f_res_38878 = defunc_0_f_res_f_res_f_res_f_res_38882;
                                    }
                                    defunc_0_f_res_f_res_38877 = defunc_0_f_res_f_res_f_res_38878;
                                }
                                defunc_0_f_res_38869 = defunc_0_f_res_f_res_38877;
                            }
                            ((int64_t *) mem_40055.mem)[i_39553] = defunc_0_f_res_38869;
                        }
                        if (memblock_set(ctx, &mem_param_tmp_40323, &mem_40055, "mem_40055") != 0)
                            return 1;
                        
                        int64_t last_step_tmp_40325 = this_step_38334;
                        
                        if (memblock_set(ctx, &mem_param_40053, &mem_param_tmp_40323, "mem_param_tmp_40323") != 0)
                            return 1;
                        last_step_38332 = last_step_tmp_40325;
                    }
                    if (memblock_set(ctx, &ext_mem_40064, &mem_param_40053, "mem_param_40053") != 0)
                        return 1;
                    bsearch_first_38329 = last_step_38332;
                    
                    int64_t bsearch_last_38389;
                    int64_t last_step_38392;
                    
                    if (memblock_set(ctx, &mem_param_40067, &ext_mem_40064, "ext_mem_40064") != 0)
                        return 1;
                    last_step_38392 = dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281;
                    for (int64_t i_38390 = 0; i_38390 < num_iter_38326; i_38390++) {
                        int64_t zs_lhs_38393 = add64((int64_t) 1, last_step_38392);
                        int64_t max_arg1_38394 = sdiv64(zs_lhs_38393, (int64_t) 2);
                        int64_t max_res_38395 = smax64((int64_t) 1, max_arg1_38394);
                        
                        if (memblock_alloc(ctx, &mem_40069, bytes_40041, "mem_40069")) {
                            err = 1;
                            goto cleanup;
                        }
                        for (int64_t i_39557 = 0; i_39557 < dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234; i_39557++) {
                            int64_t eta_p_38920 = ((int64_t *) mem_param_40067.mem)[i_39557];
                            int64_t slice_39566 = tR_start_38231 + i_39557;
                            int32_t eta_p_38921 = ((int32_t *) mem_39804)[slice_39566];
                            bool cond_38923 = slt64(eta_p_38920, (int64_t) 0);
                            int32_t cur_elem_38924;
                            
                            if (cond_38923) {
                                bool index_certs_39449;
                                
                                if (!y_38301) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281, "].", "-> #0  ../../../joins/ftSMJ.fut:244:40-45\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftSMJ.fut:243:14-247:12\n   #3  test_matching.fut:27:79-82\n   #4  test_matching.fut:19:1-28:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                cur_elem_38924 = lt_rhs_38303;
                            } else {
                                bool x_38928 = sle64((int64_t) 0, eta_p_38920);
                                bool y_38929 = slt64(eta_p_38920, dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281);
                                bool bounds_check_38930 = x_38928 && y_38929;
                                bool index_certs_38931;
                                
                                if (!bounds_check_38930) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_38920, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281, "].", "-> #0  ../../../joins/ftSMJ.fut:244:51-56\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftSMJ.fut:243:14-247:12\n   #3  test_matching.fut:27:79-82\n   #4  test_matching.fut:19:1-28:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t slice_38932 = tS_start_38278 + eta_p_38920;
                                int32_t cur_elem_f_res_38933 = ((int32_t *) mem_40021)[slice_38932];
                                
                                cur_elem_38924 = cur_elem_f_res_38933;
                            }
                            
                            bool cond_38934 = sle64(m_38283, eta_p_38920);
                            int32_t next_elem_38935;
                            
                            if (cond_38934) {
                                bool index_certs_39455;
                                
                                if (!bounds_check_38297) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) m_38283, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281, "].", "-> #0  ../../../joins/ftSMJ.fut:245:47-55\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftSMJ.fut:243:14-247:12\n   #3  test_matching.fut:27:79-82\n   #4  test_matching.fut:19:1-28:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                next_elem_38935 = gt_rhs_38299;
                            } else {
                                int64_t tmp_38943 = add64((int64_t) 1, eta_p_38920);
                                bool x_38944 = sle64((int64_t) 0, tmp_38943);
                                bool y_38945 = slt64(tmp_38943, dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281);
                                bool bounds_check_38946 = x_38944 && y_38945;
                                bool index_certs_38947;
                                
                                if (!bounds_check_38946) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_38943, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281, "].", "-> #0  ../../../joins/ftSMJ.fut:245:61-68\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftSMJ.fut:243:14-247:12\n   #3  test_matching.fut:27:79-82\n   #4  test_matching.fut:19:1-28:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t slice_38948 = tS_start_38278 + tmp_38943;
                                int32_t next_elem_f_res_38949 = ((int32_t *) mem_40021)[slice_38948];
                                
                                next_elem_38935 = next_elem_f_res_38949;
                            }
                            
                            int64_t defunc_0_f_res_38953;
                            
                            if (cond_38923) {
                                defunc_0_f_res_38953 = (int64_t) -1;
                            } else {
                                bool defunc_0_eq_res_38954 = eta_p_38921 == cur_elem_38924;
                                bool cond_38956 = eta_p_38920 == m_38283;
                                bool defunc_0_gt_res_38957 = slt32(eta_p_38921, next_elem_38935);
                                bool x_38958 = !cond_38956;
                                bool y_38959 = defunc_0_gt_res_38957 && x_38958;
                                bool cond_t_res_38960 = cond_38956 || y_38959;
                                bool x_38961 = defunc_0_eq_res_38954 && cond_t_res_38960;
                                int64_t defunc_0_f_res_f_res_38962;
                                
                                if (x_38961) {
                                    defunc_0_f_res_f_res_38962 = eta_p_38920;
                                } else {
                                    int64_t defunc_0_f_res_f_res_f_res_38963;
                                    
                                    if (defunc_0_eq_res_38954) {
                                        int64_t min_arg1_39458 = add64(max_res_38395, eta_p_38920);
                                        int64_t min_res_39459 = smin64(m_38283, min_arg1_39458);
                                        
                                        defunc_0_f_res_f_res_f_res_38963 = min_res_39459;
                                    } else {
                                        bool defunc_0_gt_res_38966 = slt32(cur_elem_38924, eta_p_38921);
                                        int64_t defunc_0_f_res_f_res_f_res_f_res_38967;
                                        
                                        if (defunc_0_gt_res_38966) {
                                            int64_t min_arg1_39460 = add64(max_res_38395, eta_p_38920);
                                            int64_t min_res_39461 = smin64(m_38283, min_arg1_39460);
                                            
                                            defunc_0_f_res_f_res_f_res_f_res_38967 = min_res_39461;
                                        } else {
                                            int64_t max_arg1_38970 = sub64(eta_p_38920, max_res_38395);
                                            int64_t max_res_38971 = smax64((int64_t) 0, max_arg1_38970);
                                            
                                            defunc_0_f_res_f_res_f_res_f_res_38967 = max_res_38971;
                                        }
                                        defunc_0_f_res_f_res_f_res_38963 = defunc_0_f_res_f_res_f_res_f_res_38967;
                                    }
                                    defunc_0_f_res_f_res_38962 = defunc_0_f_res_f_res_f_res_38963;
                                }
                                defunc_0_f_res_38953 = defunc_0_f_res_f_res_38962;
                            }
                            ((int64_t *) mem_40069.mem)[i_39557] = defunc_0_f_res_38953;
                        }
                        if (memblock_set(ctx, &mem_param_tmp_40327, &mem_40069, "mem_40069") != 0)
                            return 1;
                        
                        int64_t last_step_tmp_40329 = max_res_38395;
                        
                        if (memblock_set(ctx, &mem_param_40067, &mem_param_tmp_40327, "mem_param_tmp_40327") != 0)
                            return 1;
                        last_step_38392 = last_step_tmp_40329;
                    }
                    if (memblock_set(ctx, &ext_mem_40078, &mem_param_40067, "mem_param_40067") != 0)
                        return 1;
                    bsearch_last_38389 = last_step_38392;
                    if (memblock_alloc(ctx, &mem_40080, bytes_40041, "mem_40080")) {
                        err = 1;
                        goto cleanup;
                    }
                    if (memblock_alloc(ctx, &mem_40082, bytes_40041, "mem_40082")) {
                        err = 1;
                        goto cleanup;
                    }
                    for (int64_t i_39563 = 0; i_39563 < dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234; i_39563++) {
                        int64_t eta_p_38820 = ((int64_t *) ext_mem_40064.mem)[i_39563];
                        int64_t eta_p_38821 = ((int64_t *) mem_param_40050.mem)[ctx_param_ext_40048 + i_39563 * ctx_param_ext_40049];
                        int64_t eta_p_38822 = ((int64_t *) ext_mem_40078.mem)[i_39563];
                        int64_t eta_p_38823 = ((int64_t *) mem_param_40047.mem)[ctx_param_ext_40045 + i_39563 * ctx_param_ext_40046];
                        bool cond_38825 = slt64(eta_p_38820, (int64_t) 0);
                        int64_t lifted_lambda_res_38826;
                        
                        if (cond_38825) {
                            lifted_lambda_res_38826 = (int64_t) 0;
                        } else {
                            int64_t zp_lhs_38827 = sub64(eta_p_38822, eta_p_38820);
                            int64_t lifted_lambda_res_f_res_38828 = add64((int64_t) 1, zp_lhs_38827);
                            
                            lifted_lambda_res_38826 = lifted_lambda_res_f_res_38828;
                        }
                        
                        int64_t defunc_0_f_res_38830 = add64(eta_p_38823, lifted_lambda_res_38826);
                        int64_t lifted_lambda_res_38833;
                        
                        if (cond_38825) {
                            lifted_lambda_res_38833 = eta_p_38820;
                        } else {
                            int64_t lifted_lambda_res_f_res_38834 = add64(tS_start_38278, eta_p_38820);
                            
                            lifted_lambda_res_38833 = lifted_lambda_res_f_res_38834;
                        }
                        
                        bool cond_38836 = slt64(eta_p_38821, (int64_t) 0);
                        int64_t lifted_lambda_res_38837;
                        
                        if (cond_38836) {
                            lifted_lambda_res_38837 = lifted_lambda_res_38833;
                        } else {
                            lifted_lambda_res_38837 = eta_p_38821;
                        }
                        ((int64_t *) mem_40080.mem)[i_39563] = lifted_lambda_res_38837;
                        ((int64_t *) mem_40082.mem)[i_39563] = defunc_0_f_res_38830;
                    }
                    if (memblock_unref(ctx, &ext_mem_40064, "ext_mem_40064") != 0)
                        return 1;
                    if (memblock_unref(ctx, &ext_mem_40078, "ext_mem_40078") != 0)
                        return 1;
                    
                    int64_t tmp_38472 = add64((int64_t) 1, s_iter_38274);
                    
                    if (memblock_set(ctx, &ext_mem_40100, &mem_40082, "mem_40082") != 0)
                        return 1;
                    if (memblock_set(ctx, &ext_mem_40097, &mem_40080, "mem_40080") != 0)
                        return 1;
                    loopres_f_res_38312 = tmp_38472;
                    loopres_f_res_38315 = nextRel_38320;
                }
                if (memblock_set(ctx, &ext_mem_40106, &ext_mem_40100, "ext_mem_40100") != 0)
                    return 1;
                if (memblock_set(ctx, &ext_mem_40103, &ext_mem_40097, "ext_mem_40097") != 0)
                    return 1;
                loopres_38306 = loopres_f_res_38312;
                loopres_38309 = loopres_f_res_38315;
                loopres_38310 = minRelevant_38277;
            }
            
            bool cond_38473 = slt64(loopres_38306, numIter_S_38215);
            bool x_38474 = loopres_38309 && cond_38473;
            
            if (memblock_set(ctx, &mem_param_tmp_40312, &ext_mem_40106, "ext_mem_40106") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_tmp_40313, &ext_mem_40103, "ext_mem_40103") != 0)
                return 1;
            
            int64_t ctx_param_ext_tmp_40314 = ext_40105;
            int64_t ctx_param_ext_tmp_40315 = ext_40104;
            int64_t ctx_param_ext_tmp_40316 = ext_40102;
            int64_t ctx_param_ext_tmp_40317 = ext_40101;
            bool loop_while_tmp_40318 = x_38474;
            int64_t s_iter_tmp_40319 = loopres_38306;
            int64_t minRelevant_tmp_40322 = loopres_38310;
            
            if (memblock_set(ctx, &mem_param_40047, &mem_param_tmp_40312, "mem_param_tmp_40312") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_40050, &mem_param_tmp_40313, "mem_param_tmp_40313") != 0)
                return 1;
            ctx_param_ext_40045 = ctx_param_ext_tmp_40314;
            ctx_param_ext_40046 = ctx_param_ext_tmp_40315;
            ctx_param_ext_40048 = ctx_param_ext_tmp_40316;
            ctx_param_ext_40049 = ctx_param_ext_tmp_40317;
            loop_while_38273 = loop_while_tmp_40318;
            s_iter_38274 = s_iter_tmp_40319;
            minRelevant_38277 = minRelevant_tmp_40322;
        }
        if (memblock_set(ctx, &ext_mem_40112, &mem_param_40047, "mem_param_40047") != 0)
            return 1;
        if (memblock_set(ctx, &ext_mem_40111, &mem_param_40050, "mem_param_40050") != 0)
            return 1;
        ext_40110 = ctx_param_ext_40045;
        ext_40109 = ctx_param_ext_40046;
        ext_40108 = ctx_param_ext_40048;
        ext_40107 = ctx_param_ext_40049;
        loopres_38268 = loop_while_38273;
        loopres_38269 = s_iter_38274;
        loopres_38272 = minRelevant_38277;
        if (memblock_unref(ctx, &mem_40042, "mem_40042") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_40044, "mem_40044") != 0)
            return 1;
        if (memblock_alloc(ctx, &mem_40114, bytes_39596, "mem_40114")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_40114.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_40040.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {n1_24948});
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_40114.mem, tR_start_38231, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_40111.mem, ext_40108, (int64_t []) {ext_40107}, (int64_t []) {dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234});
        if (memblock_unref(ctx, &ext_mem_40111, "ext_mem_40111") != 0)
            return 1;
        if (memblock_alloc(ctx, &mem_40116, bytes_39596, "mem_40116")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_40116.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_40037.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {n1_24948});
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_40116.mem, tR_start_38231, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_40112.mem, ext_40110, (int64_t []) {ext_40109}, (int64_t []) {dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234});
        if (memblock_unref(ctx, &ext_mem_40112, "ext_mem_40112") != 0)
            return 1;
        
        int64_t tmp_38480 = add64((int64_t) 1, iter_38227);
        bool loop_cond_38481 = slt64(tmp_38480, numIter_R_38212);
        
        if (memblock_set(ctx, &mem_param_tmp_40303, &mem_40116, "mem_40116") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_40304, &mem_40114, "mem_40114") != 0)
            return 1;
        
        bool loop_while_tmp_40305 = loop_cond_38481;
        int64_t iter_tmp_40306 = tmp_38480;
        int64_t first_relevant_in_S_tmp_40309 = loopres_38272;
        
        if (memblock_set(ctx, &mem_param_40037, &mem_param_tmp_40303, "mem_param_tmp_40303") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_40040, &mem_param_tmp_40304, "mem_param_tmp_40304") != 0)
            return 1;
        loop_while_38226 = loop_while_tmp_40305;
        iter_38227 = iter_tmp_40306;
        first_relevant_in_S_38230 = first_relevant_in_S_tmp_40309;
    }
    if (memblock_set(ctx, &ext_mem_40122, &mem_param_40037, "mem_param_40037") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_40121, &mem_param_40040, "mem_param_40040") != 0)
        return 1;
    defunc_0_mergeJoin_res_38221 = loop_while_38226;
    defunc_0_mergeJoin_res_38222 = iter_38227;
    defunc_0_mergeJoin_res_38225 = first_relevant_in_S_38230;
    if (memblock_unref(ctx, &mem_40032, "mem_40032") != 0)
        return 1;
    if (memblock_unref(ctx, &mem_40034, "mem_40034") != 0)
        return 1;
    if (memblock_set(ctx, &mem_out_40205, &mem_40029, "mem_40029") != 0)
        return 1;
    if (memblock_set(ctx, &mem_out_40206, &ext_mem_40030, "ext_mem_40030") != 0)
        return 1;
    if (memblock_set(ctx, &mem_out_40207, &ext_mem_40121, "ext_mem_40121") != 0)
        return 1;
    if (memblock_set(ctx, &mem_out_40208, &ext_mem_40122, "ext_mem_40122") != 0)
        return 1;
    if (memblock_set(ctx, &*mem_out_p_40333, &mem_out_40205, "mem_out_40205") != 0)
        return 1;
    if (memblock_set(ctx, &*mem_out_p_40334, &mem_out_40206, "mem_out_40206") != 0)
        return 1;
    if (memblock_set(ctx, &*mem_out_p_40335, &mem_out_40207, "mem_out_40207") != 0)
        return 1;
    if (memblock_set(ctx, &*mem_out_p_40336, &mem_out_40208, "mem_out_40208") != 0)
        return 1;
    
  cleanup:
    {
        free(mem_39600);
        free(mem_39603);
        free(mem_39613);
        free(mem_39615);
        free(mem_39682);
        free(mem_39685);
        free(mem_39695);
        free(mem_39697);
        free(mem_39712);
        free(mem_39715);
        free(mem_39804);
        free(mem_39817);
        free(mem_39820);
        free(mem_39830);
        free(mem_39832);
        free(mem_39899);
        free(mem_39902);
        free(mem_39912);
        free(mem_39914);
        free(mem_39929);
        free(mem_39932);
        free(mem_40021);
        if (memblock_unref(ctx, &mem_param_tmp_40304, "mem_param_tmp_40304") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_40303, "mem_param_tmp_40303") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_40116, "mem_40116") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_40114, "mem_40114") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_40313, "mem_param_tmp_40313") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_40312, "mem_param_tmp_40312") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_40082, "mem_40082") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_40080, "mem_40080") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_40327, "mem_param_tmp_40327") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_40069, "mem_40069") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_40067, "mem_param_40067") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_40078, "ext_mem_40078") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_40323, "mem_param_tmp_40323") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_40055, "mem_40055") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_40053, "mem_param_40053") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_40064, "ext_mem_40064") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_40097, "ext_mem_40097") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_40100, "ext_mem_40100") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_40103, "ext_mem_40103") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_40106, "ext_mem_40106") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_40050, "mem_param_40050") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_40047, "mem_param_40047") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_40111, "ext_mem_40111") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_40112, "ext_mem_40112") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_40044, "mem_40044") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_40042, "mem_40042") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_40040, "mem_param_40040") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_40037, "mem_param_40037") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_40121, "ext_mem_40121") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_40122, "ext_mem_40122") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_40034, "mem_40034") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_40032, "mem_40032") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_40030, "ext_mem_40030") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_40029, "mem_40029") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_40272, "mem_param_tmp_40272") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_40271, "mem_param_tmp_40271") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_40009, "mem_40009") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_40005, "mem_40005") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39971, "mem_39971") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39967, "mem_39967") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39976, "mem_39976") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39974, "mem_39974") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_39991, "ext_mem_39991") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_39994, "ext_mem_39994") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_39896, "mem_param_39896") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_39893, "mem_param_39893") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_40015, "ext_mem_40015") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_40016, "ext_mem_40016") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39889, "mem_39889") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39885, "mem_39885") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_40019, "ext_mem_40019") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39814, "mem_39814") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39812, "mem_39812") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_40228, "mem_param_tmp_40228") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_40227, "mem_param_tmp_40227") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39792, "mem_39792") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39788, "mem_39788") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39754, "mem_39754") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39750, "mem_39750") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39759, "mem_39759") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39757, "mem_39757") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_39774, "ext_mem_39774") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_39777, "ext_mem_39777") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_39679, "mem_param_39679") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_39676, "mem_param_39676") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_39798, "ext_mem_39798") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_39799, "ext_mem_39799") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39672, "mem_39672") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39668, "mem_39668") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_39802, "ext_mem_39802") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39597, "mem_39597") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_40208, "mem_out_40208") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_40207, "mem_out_40207") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_40206, "mem_out_40206") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_40205, "mem_out_40205") != 0)
            return 1;
    }
    return err;
}
FUTHARK_FUN_ATTR int futrts_entry_smj1_i64(struct futhark_context *ctx, struct memblock *mem_out_p_40359, struct memblock *mem_out_p_40360, struct memblock *mem_out_p_40361, struct memblock *mem_out_p_40362, struct memblock ks1_mem_39592, struct memblock pL1_mem_39593, struct memblock ks2_mem_39594, struct memblock pL2_mem_39595, int64_t n1_30936, int64_t n2_30937)
{
    (void) ctx;
    
    int err = 0;
    int64_t mem_39600_cached_sizze_40363 = 0;
    unsigned char *mem_39600 = NULL;
    int64_t mem_39603_cached_sizze_40364 = 0;
    unsigned char *mem_39603 = NULL;
    int64_t mem_39613_cached_sizze_40365 = 0;
    unsigned char *mem_39613 = NULL;
    int64_t mem_39615_cached_sizze_40366 = 0;
    unsigned char *mem_39615 = NULL;
    int64_t mem_39682_cached_sizze_40367 = 0;
    unsigned char *mem_39682 = NULL;
    int64_t mem_39685_cached_sizze_40368 = 0;
    unsigned char *mem_39685 = NULL;
    int64_t mem_39695_cached_sizze_40369 = 0;
    unsigned char *mem_39695 = NULL;
    int64_t mem_39697_cached_sizze_40370 = 0;
    unsigned char *mem_39697 = NULL;
    int64_t mem_39712_cached_sizze_40371 = 0;
    unsigned char *mem_39712 = NULL;
    int64_t mem_39715_cached_sizze_40372 = 0;
    unsigned char *mem_39715 = NULL;
    int64_t mem_39804_cached_sizze_40373 = 0;
    unsigned char *mem_39804 = NULL;
    int64_t mem_39817_cached_sizze_40374 = 0;
    unsigned char *mem_39817 = NULL;
    int64_t mem_39820_cached_sizze_40375 = 0;
    unsigned char *mem_39820 = NULL;
    int64_t mem_39830_cached_sizze_40376 = 0;
    unsigned char *mem_39830 = NULL;
    int64_t mem_39832_cached_sizze_40377 = 0;
    unsigned char *mem_39832 = NULL;
    int64_t mem_39899_cached_sizze_40378 = 0;
    unsigned char *mem_39899 = NULL;
    int64_t mem_39902_cached_sizze_40379 = 0;
    unsigned char *mem_39902 = NULL;
    int64_t mem_39912_cached_sizze_40380 = 0;
    unsigned char *mem_39912 = NULL;
    int64_t mem_39914_cached_sizze_40381 = 0;
    unsigned char *mem_39914 = NULL;
    int64_t mem_39929_cached_sizze_40382 = 0;
    unsigned char *mem_39929 = NULL;
    int64_t mem_39932_cached_sizze_40383 = 0;
    unsigned char *mem_39932 = NULL;
    int64_t mem_40021_cached_sizze_40384 = 0;
    unsigned char *mem_40021 = NULL;
    struct memblock mem_param_tmp_40304;
    
    mem_param_tmp_40304.references = NULL;
    
    struct memblock mem_param_tmp_40303;
    
    mem_param_tmp_40303.references = NULL;
    
    struct memblock mem_40116;
    
    mem_40116.references = NULL;
    
    struct memblock mem_40114;
    
    mem_40114.references = NULL;
    
    struct memblock mem_param_tmp_40313;
    
    mem_param_tmp_40313.references = NULL;
    
    struct memblock mem_param_tmp_40312;
    
    mem_param_tmp_40312.references = NULL;
    
    struct memblock mem_40082;
    
    mem_40082.references = NULL;
    
    struct memblock mem_40080;
    
    mem_40080.references = NULL;
    
    struct memblock mem_param_tmp_40327;
    
    mem_param_tmp_40327.references = NULL;
    
    struct memblock mem_40069;
    
    mem_40069.references = NULL;
    
    struct memblock mem_param_40067;
    
    mem_param_40067.references = NULL;
    
    struct memblock ext_mem_40078;
    
    ext_mem_40078.references = NULL;
    
    struct memblock mem_param_tmp_40323;
    
    mem_param_tmp_40323.references = NULL;
    
    struct memblock mem_40055;
    
    mem_40055.references = NULL;
    
    struct memblock mem_param_40053;
    
    mem_param_40053.references = NULL;
    
    struct memblock ext_mem_40064;
    
    ext_mem_40064.references = NULL;
    
    struct memblock ext_mem_40097;
    
    ext_mem_40097.references = NULL;
    
    struct memblock ext_mem_40100;
    
    ext_mem_40100.references = NULL;
    
    struct memblock ext_mem_40103;
    
    ext_mem_40103.references = NULL;
    
    struct memblock ext_mem_40106;
    
    ext_mem_40106.references = NULL;
    
    struct memblock mem_param_40050;
    
    mem_param_40050.references = NULL;
    
    struct memblock mem_param_40047;
    
    mem_param_40047.references = NULL;
    
    struct memblock ext_mem_40111;
    
    ext_mem_40111.references = NULL;
    
    struct memblock ext_mem_40112;
    
    ext_mem_40112.references = NULL;
    
    struct memblock mem_40044;
    
    mem_40044.references = NULL;
    
    struct memblock mem_40042;
    
    mem_40042.references = NULL;
    
    struct memblock mem_param_40040;
    
    mem_param_40040.references = NULL;
    
    struct memblock mem_param_40037;
    
    mem_param_40037.references = NULL;
    
    struct memblock ext_mem_40121;
    
    ext_mem_40121.references = NULL;
    
    struct memblock ext_mem_40122;
    
    ext_mem_40122.references = NULL;
    
    struct memblock mem_40034;
    
    mem_40034.references = NULL;
    
    struct memblock mem_40032;
    
    mem_40032.references = NULL;
    
    struct memblock ext_mem_40030;
    
    ext_mem_40030.references = NULL;
    
    struct memblock mem_40029;
    
    mem_40029.references = NULL;
    
    struct memblock mem_param_tmp_40272;
    
    mem_param_tmp_40272.references = NULL;
    
    struct memblock mem_param_tmp_40271;
    
    mem_param_tmp_40271.references = NULL;
    
    struct memblock mem_40009;
    
    mem_40009.references = NULL;
    
    struct memblock mem_40005;
    
    mem_40005.references = NULL;
    
    struct memblock mem_39971;
    
    mem_39971.references = NULL;
    
    struct memblock mem_39967;
    
    mem_39967.references = NULL;
    
    struct memblock mem_39976;
    
    mem_39976.references = NULL;
    
    struct memblock mem_39974;
    
    mem_39974.references = NULL;
    
    struct memblock ext_mem_39991;
    
    ext_mem_39991.references = NULL;
    
    struct memblock ext_mem_39994;
    
    ext_mem_39994.references = NULL;
    
    struct memblock mem_param_39896;
    
    mem_param_39896.references = NULL;
    
    struct memblock mem_param_39893;
    
    mem_param_39893.references = NULL;
    
    struct memblock ext_mem_40015;
    
    ext_mem_40015.references = NULL;
    
    struct memblock ext_mem_40016;
    
    ext_mem_40016.references = NULL;
    
    struct memblock mem_39889;
    
    mem_39889.references = NULL;
    
    struct memblock mem_39885;
    
    mem_39885.references = NULL;
    
    struct memblock ext_mem_40019;
    
    ext_mem_40019.references = NULL;
    
    struct memblock mem_39814;
    
    mem_39814.references = NULL;
    
    struct memblock mem_39812;
    
    mem_39812.references = NULL;
    
    struct memblock mem_param_tmp_40228;
    
    mem_param_tmp_40228.references = NULL;
    
    struct memblock mem_param_tmp_40227;
    
    mem_param_tmp_40227.references = NULL;
    
    struct memblock mem_39792;
    
    mem_39792.references = NULL;
    
    struct memblock mem_39788;
    
    mem_39788.references = NULL;
    
    struct memblock mem_39754;
    
    mem_39754.references = NULL;
    
    struct memblock mem_39750;
    
    mem_39750.references = NULL;
    
    struct memblock mem_39759;
    
    mem_39759.references = NULL;
    
    struct memblock mem_39757;
    
    mem_39757.references = NULL;
    
    struct memblock ext_mem_39774;
    
    ext_mem_39774.references = NULL;
    
    struct memblock ext_mem_39777;
    
    ext_mem_39777.references = NULL;
    
    struct memblock mem_param_39679;
    
    mem_param_39679.references = NULL;
    
    struct memblock mem_param_39676;
    
    mem_param_39676.references = NULL;
    
    struct memblock ext_mem_39798;
    
    ext_mem_39798.references = NULL;
    
    struct memblock ext_mem_39799;
    
    ext_mem_39799.references = NULL;
    
    struct memblock mem_39672;
    
    mem_39672.references = NULL;
    
    struct memblock mem_39668;
    
    mem_39668.references = NULL;
    
    struct memblock ext_mem_39802;
    
    ext_mem_39802.references = NULL;
    
    struct memblock mem_39597;
    
    mem_39597.references = NULL;
    
    struct memblock mem_out_40208;
    
    mem_out_40208.references = NULL;
    
    struct memblock mem_out_40207;
    
    mem_out_40207.references = NULL;
    
    struct memblock mem_out_40206;
    
    mem_out_40206.references = NULL;
    
    struct memblock mem_out_40205;
    
    mem_out_40205.references = NULL;
    
    int64_t bytes_39596 = (int64_t) 8 * n1_30936;
    bool cond_37162 = sle64(n1_30936, (int64_t) 1);
    int64_t zs_lhs_37163 = sub64(n1_30936, (int64_t) 1);
    int64_t zp_rhs_37164 = sdiv_safe64(zs_lhs_37163, (int64_t) 20);
    int64_t min_num_blocks_37165 = add64((int64_t) 1, zp_rhs_37164);
    bool loop_cond_37166 = slt64((int64_t) 1, min_num_blocks_37165);
    bool smallest_pow_2_geq_than_res_37167;
    int64_t smallest_pow_2_geq_than_res_37168;
    bool loop_while_37169;
    int64_t x_37170;
    
    loop_while_37169 = loop_cond_37166;
    x_37170 = (int64_t) 1;
    while (loop_while_37169) {
        int64_t loopres_37171 = mul64((int64_t) 2, x_37170);
        bool loop_cond_37172 = slt64(loopres_37171, min_num_blocks_37165);
        bool loop_while_tmp_40209 = loop_cond_37172;
        int64_t x_tmp_40210 = loopres_37171;
        
        loop_while_37169 = loop_while_tmp_40209;
        x_37170 = x_tmp_40210;
    }
    smallest_pow_2_geq_than_res_37167 = loop_while_37169;
    smallest_pow_2_geq_than_res_37168 = x_37170;
    
    bool zzero_37173 = smallest_pow_2_geq_than_res_37168 == (int64_t) 0;
    bool nonzzero_37174 = !zzero_37173;
    bool protect_assert_disj_37175 = cond_37162 || nonzzero_37174;
    bool nonzzero_cert_37176;
    
    if (!protect_assert_disj_37175) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:234:30-42\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftSMJ.fut:104:60-63\n   #4  test_matching.fut:51:43-47\n   #5  test_matching.fut:45:1-54:44\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t zp_rhs_37177 = sdiv_safe64(zs_lhs_37163, smallest_pow_2_geq_than_res_37168);
    int64_t block_sizze_37178 = add64((int64_t) 1, zp_rhs_37177);
    bool y_37184 = slt64((int64_t) 0, n1_30936);
    bool protect_assert_disj_40166 = cond_37162 || y_37184;
    bool index_certs_37185;
    
    if (!protect_assert_disj_40166) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) n1_30936, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:240:60-64\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftSMJ.fut:104:60-63\n   #4  test_matching.fut:51:43-47\n   #5  test_matching.fut:45:1-54:44\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool bounds_invalid_upwards_37199 = slt64(smallest_pow_2_geq_than_res_37168, (int64_t) 0);
    bool valid_37200 = !bounds_invalid_upwards_37199;
    bool protect_assert_disj_40174 = cond_37162 || valid_37200;
    bool range_valid_c_37201;
    
    if (!protect_assert_disj_40174) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) smallest_pow_2_geq_than_res_37168, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #3  /prelude/functional.fut:9:44-45\n   #4  ../../../joins/ftSMJ.fut:104:60-63\n   #5  test_matching.fut:51:43-47\n   #6  test_matching.fut:45:1-54:44\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool bounds_invalid_upwards_37203 = slt64(block_sizze_37178, (int64_t) 0);
    bool valid_37204 = !bounds_invalid_upwards_37203;
    bool protect_assert_disj_40176 = cond_37162 || valid_37204;
    bool range_valid_c_37205;
    
    if (!protect_assert_disj_40176) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) block_sizze_37178, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:51:43-47\n   #8  test_matching.fut:45:1-54:44\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool cond_37207 = sle64(block_sizze_37178, (int64_t) 1);
    int64_t tmp_37208 = sub64(block_sizze_37178, (int64_t) 2);
    bool bounds_invalid_upwards_37209 = slt64(tmp_37208, (int64_t) 0);
    bool valid_37211 = !bounds_invalid_upwards_37209;
    bool protect_assert_disj_37212 = cond_37207 || valid_37211;
    bool protect_assert_disj_40178 = cond_37162 || protect_assert_disj_37212;
    bool range_valid_c_37213;
    
    if (!protect_assert_disj_40178) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "...", (long long) tmp_37208, " is invalid.", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:166:19-28\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:51:43-47\n   #8  test_matching.fut:45:1-54:44\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t bytes_39813 = (int64_t) 8 * n2_30937;
    bool cond_37682 = sle64(n2_30937, (int64_t) 1);
    int64_t zs_lhs_37683 = sub64(n2_30937, (int64_t) 1);
    int64_t zp_rhs_37684 = sdiv_safe64(zs_lhs_37683, (int64_t) 20);
    int64_t min_num_blocks_37685 = add64((int64_t) 1, zp_rhs_37684);
    bool loop_cond_37686 = slt64((int64_t) 1, min_num_blocks_37685);
    bool smallest_pow_2_geq_than_res_37687;
    int64_t smallest_pow_2_geq_than_res_37688;
    bool loop_while_37689;
    int64_t x_37690;
    
    loop_while_37689 = loop_cond_37686;
    x_37690 = (int64_t) 1;
    while (loop_while_37689) {
        int64_t loopres_37691 = mul64((int64_t) 2, x_37690);
        bool loop_cond_37692 = slt64(loopres_37691, min_num_blocks_37685);
        bool loop_while_tmp_40211 = loop_cond_37692;
        int64_t x_tmp_40212 = loopres_37691;
        
        loop_while_37689 = loop_while_tmp_40211;
        x_37690 = x_tmp_40212;
    }
    smallest_pow_2_geq_than_res_37687 = loop_while_37689;
    smallest_pow_2_geq_than_res_37688 = x_37690;
    
    bool zzero_37693 = smallest_pow_2_geq_than_res_37688 == (int64_t) 0;
    bool nonzzero_37694 = !zzero_37693;
    bool protect_assert_disj_37695 = cond_37682 || nonzzero_37694;
    bool nonzzero_cert_37696;
    
    if (!protect_assert_disj_37695) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:234:30-42\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftSMJ.fut:104:60-63\n   #4  test_matching.fut:52:43-47\n   #5  test_matching.fut:45:1-54:44\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t zp_rhs_37697 = sdiv_safe64(zs_lhs_37683, smallest_pow_2_geq_than_res_37688);
    int64_t block_sizze_37698 = add64((int64_t) 1, zp_rhs_37697);
    bool y_37704 = slt64((int64_t) 0, n2_30937);
    bool protect_assert_disj_40140 = cond_37682 || y_37704;
    bool index_certs_37705;
    
    if (!protect_assert_disj_40140) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) n2_30937, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:240:60-64\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftSMJ.fut:104:60-63\n   #4  test_matching.fut:52:43-47\n   #5  test_matching.fut:45:1-54:44\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool bounds_invalid_upwards_37719 = slt64(smallest_pow_2_geq_than_res_37688, (int64_t) 0);
    bool valid_37720 = !bounds_invalid_upwards_37719;
    bool protect_assert_disj_40148 = cond_37682 || valid_37720;
    bool range_valid_c_37721;
    
    if (!protect_assert_disj_40148) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) smallest_pow_2_geq_than_res_37688, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #3  /prelude/functional.fut:9:44-45\n   #4  ../../../joins/ftSMJ.fut:104:60-63\n   #5  test_matching.fut:52:43-47\n   #6  test_matching.fut:45:1-54:44\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool bounds_invalid_upwards_37723 = slt64(block_sizze_37698, (int64_t) 0);
    bool valid_37724 = !bounds_invalid_upwards_37723;
    bool protect_assert_disj_40150 = cond_37682 || valid_37724;
    bool range_valid_c_37725;
    
    if (!protect_assert_disj_40150) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) block_sizze_37698, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:52:43-47\n   #8  test_matching.fut:45:1-54:44\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool cond_37727 = sle64(block_sizze_37698, (int64_t) 1);
    int64_t tmp_37728 = sub64(block_sizze_37698, (int64_t) 2);
    bool bounds_invalid_upwards_37729 = slt64(tmp_37728, (int64_t) 0);
    bool valid_37731 = !bounds_invalid_upwards_37729;
    bool protect_assert_disj_37732 = cond_37727 || valid_37731;
    bool protect_assert_disj_40152 = cond_37682 || protect_assert_disj_37732;
    bool range_valid_c_37733;
    
    if (!protect_assert_disj_40152) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "...", (long long) tmp_37728, " is invalid.", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:166:19-28\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:52:43-47\n   #8  test_matching.fut:45:1-54:44\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    if (memblock_alloc(ctx, &mem_39597, bytes_39596, "mem_39597")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t i_40213 = 0; i_40213 < n1_30936; i_40213++) {
        int64_t x_40214 = (int64_t) 0 + i_40213 * (int64_t) 1;
        
        ((int64_t *) mem_39597.mem)[i_40213] = x_40214;
    }
    
    int64_t flat_dim_37179 = smallest_pow_2_geq_than_res_37168 * block_sizze_37178;
    bool x_40161 = !cond_37162;
    int64_t greatest_divisor_leq_than_arg1_37182 = mul64((int64_t) 2, block_sizze_37178);
    int64_t merge_block_sizze_37183;
    
    if (x_40161) {
        int64_t x_40163;
        
        if (futrts_greatest_divisor_leq_than_5346(ctx, &x_40163, (int64_t) 8, greatest_divisor_leq_than_arg1_37182) != 0) {
            err = 1;
            goto cleanup;
        }
        merge_block_sizze_37183 = x_40163;
    } else {
        merge_block_sizze_37183 = (int64_t) 0;
    }
    
    int64_t reduce_arg1_37186;
    
    if (x_40161) {
        int64_t x_40167 = ((int64_t *) ks1_mem_39592.mem)[(int64_t) 0];
        
        reduce_arg1_37186 = x_40167;
    } else {
        reduce_arg1_37186 = (int64_t) 0;
    }
    
    int64_t defunc_0_reduce_res_39276;
    int64_t defunc_0_reduce_res_39277;
    
    if (x_40161) {
        int64_t x_40169;
        int64_t x_40170;
        int64_t redout_39467;
        int64_t redout_39468;
        
        redout_39467 = reduce_arg1_37186;
        redout_39468 = (int64_t) 0;
        for (int64_t i_39469 = 0; i_39469 < n1_30936; i_39469++) {
            int64_t x_37190 = ((int64_t *) ks1_mem_39592.mem)[i_39469];
            bool defunc_0_leq_res_37196 = sle64(redout_39467, x_37190);
            int64_t defunc_0_op_res_37197;
            
            if (defunc_0_leq_res_37196) {
                defunc_0_op_res_37197 = x_37190;
            } else {
                defunc_0_op_res_37197 = redout_39467;
            }
            
            int64_t defunc_0_op_res_37198;
            
            if (defunc_0_leq_res_37196) {
                defunc_0_op_res_37198 = i_39469;
            } else {
                defunc_0_op_res_37198 = redout_39468;
            }
            
            int64_t redout_tmp_40215 = defunc_0_op_res_37197;
            int64_t redout_tmp_40216 = defunc_0_op_res_37198;
            
            redout_39467 = redout_tmp_40215;
            redout_39468 = redout_tmp_40216;
        }
        x_40169 = redout_39467;
        x_40170 = redout_39468;
        defunc_0_reduce_res_39276 = x_40169;
        defunc_0_reduce_res_39277 = x_40170;
    } else {
        defunc_0_reduce_res_39276 = (int64_t) 0;
        defunc_0_reduce_res_39277 = (int64_t) 0;
    }
    
    int64_t distance_37210 = add64((int64_t) 1, tmp_37208);
    int64_t binop_x_39598 = (int64_t) 8 * smallest_pow_2_geq_than_res_37168;
    int64_t bytes_39599 = block_sizze_37178 * binop_x_39598;
    int64_t bytes_39612 = (int64_t) 8 * block_sizze_37178;
    bool loop_cond_37285 = slt64(block_sizze_37178, flat_dim_37179);
    bool zzero_37286 = merge_block_sizze_37183 == (int64_t) 0;
    bool nonzzero_37287 = !zzero_37286;
    
    if (cond_37162) {
        if (memblock_set(ctx, &ext_mem_39802, &mem_39597, "mem_39597") != 0)
            return 1;
    } else {
        if (mem_39600_cached_sizze_40363 < bytes_39599) {
            err = lexical_realloc(ctx, &mem_39600, &mem_39600_cached_sizze_40363, bytes_39599);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_39603_cached_sizze_40364 < bytes_39599) {
            err = lexical_realloc(ctx, &mem_39603, &mem_39603_cached_sizze_40364, bytes_39599);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_39613_cached_sizze_40365 < bytes_39612) {
            err = lexical_realloc(ctx, &mem_39613, &mem_39613_cached_sizze_40365, bytes_39612);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_39615_cached_sizze_40366 < bytes_39612) {
            err = lexical_realloc(ctx, &mem_39615, &mem_39615_cached_sizze_40366, bytes_39612);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t i_39481 = 0; i_39481 < smallest_pow_2_geq_than_res_37168; i_39481++) {
            int64_t zp_lhs_37217 = mul64(block_sizze_37178, i_39481);
            
            for (int64_t i_39474 = 0; i_39474 < block_sizze_37178; i_39474++) {
                int64_t k_37221 = add64(zp_lhs_37217, i_39474);
                bool cond_37222 = slt64(k_37221, n1_30936);
                int64_t lifted_lambda_res_37223;
                int64_t lifted_lambda_res_37224;
                
                if (cond_37222) {
                    bool x_39129 = sle64((int64_t) 0, k_37221);
                    bool bounds_check_39130 = cond_37222 && x_39129;
                    bool index_certs_39131;
                    
                    if (!bounds_check_39130) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) k_37221, "] out of bounds for array of shape [", (long long) n1_30936, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:246:26-30\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ../../../joins/ftSMJ.fut:104:60-63\n   #8  test_matching.fut:51:43-47\n   #9  test_matching.fut:45:1-54:44\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t lifted_lambda_res_t_res_39132 = ((int64_t *) ks1_mem_39592.mem)[k_37221];
                    
                    lifted_lambda_res_37223 = lifted_lambda_res_t_res_39132;
                    lifted_lambda_res_37224 = k_37221;
                } else {
                    lifted_lambda_res_37223 = defunc_0_reduce_res_39276;
                    lifted_lambda_res_37224 = defunc_0_reduce_res_39277;
                }
                ((int64_t *) mem_39613)[i_39474] = lifted_lambda_res_37223;
                ((int64_t *) mem_39615)[i_39474] = lifted_lambda_res_37224;
            }
            if (!cond_37207) {
                for (int64_t i_37234 = 0; i_37234 < distance_37210; i_37234++) {
                    int64_t gt_arg1_37237 = add64((int64_t) 1, i_37234);
                    bool x_37238 = sle64((int64_t) 0, gt_arg1_37237);
                    bool y_37239 = slt64(gt_arg1_37237, block_sizze_37178);
                    bool bounds_check_37240 = x_37238 && y_37239;
                    bool index_certs_37241;
                    
                    if (!bounds_check_37240) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) gt_arg1_37237, "] out of bounds for array of shape [", (long long) block_sizze_37178, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:169:31-37\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:51:43-47\n   #8  test_matching.fut:45:1-54:44\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    bool y_37243 = slt64(i_37234, block_sizze_37178);
                    bool index_certs_37244;
                    
                    if (!y_37243) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_37234, "] out of bounds for array of shape [", (long long) block_sizze_37178, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:169:26-30\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:51:43-47\n   #8  test_matching.fut:45:1-54:44\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t gt_arg1_37242 = ((int64_t *) mem_39613)[gt_arg1_37237];
                    int64_t gt_arg0_37245 = ((int64_t *) mem_39613)[i_37234];
                    bool defunc_0_leq_res_37246 = sle64(gt_arg0_37245, gt_arg1_37242);
                    bool defunc_0_lifted_gt_res_37247 = !defunc_0_leq_res_37246;
                    bool s_37248;
                    int64_t s_37251;
                    bool loop_while_37252;
                    int64_t j_37255;
                    
                    loop_while_37252 = defunc_0_lifted_gt_res_37247;
                    j_37255 = i_37234;
                    while (loop_while_37252) {
                        bool x_37256 = sle64((int64_t) 0, j_37255);
                        bool y_37257 = slt64(j_37255, block_sizze_37178);
                        bool bounds_check_37258 = x_37256 && y_37257;
                        bool index_certs_37259;
                        
                        if (!bounds_check_37258) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_37255, "] out of bounds for array of shape [", (long long) block_sizze_37178, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:170:24-28\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:51:43-47\n   #8  test_matching.fut:45:1-54:44\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t copy_arg0_37262 = add64((int64_t) 1, j_37255);
                        bool x_37263 = sle64((int64_t) 0, copy_arg0_37262);
                        bool y_37264 = slt64(copy_arg0_37262, block_sizze_37178);
                        bool bounds_check_37265 = x_37263 && y_37264;
                        bool index_certs_37266;
                        
                        if (!bounds_check_37265) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) copy_arg0_37262, "] out of bounds for array of shape [", (long long) block_sizze_37178, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:171:35-41\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:51:43-47\n   #8  test_matching.fut:45:1-54:44\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t copy_arg0_37260 = ((int64_t *) mem_39613)[j_37255];
                        int64_t copy_arg0_37261 = ((int64_t *) mem_39615)[j_37255];
                        int64_t copy_arg0_37267 = ((int64_t *) mem_39613)[copy_arg0_37262];
                        int64_t copy_arg0_37268 = ((int64_t *) mem_39615)[copy_arg0_37262];
                        
                        ((int64_t *) mem_39613)[j_37255] = copy_arg0_37267;
                        ((int64_t *) mem_39615)[j_37255] = copy_arg0_37268;
                        ((int64_t *) mem_39613)[copy_arg0_37262] = copy_arg0_37260;
                        ((int64_t *) mem_39615)[copy_arg0_37262] = copy_arg0_37261;
                        
                        int64_t tmp_37273 = sub64(j_37255, (int64_t) 1);
                        bool cond_37274 = sle64((int64_t) 0, tmp_37273);
                        bool loop_cond_37275;
                        
                        if (cond_37274) {
                            bool y_39134 = slt64(tmp_37273, block_sizze_37178);
                            bool bounds_check_39135 = cond_37274 && y_39134;
                            bool index_certs_39136;
                            
                            if (!bounds_check_39135) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_37273, "] out of bounds for array of shape [", (long long) block_sizze_37178, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:169:26-30\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:51:43-47\n   #8  test_matching.fut:45:1-54:44\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t gt_arg1_39133 = ((int64_t *) mem_39613)[j_37255];
                            int64_t gt_arg0_39137 = ((int64_t *) mem_39613)[tmp_37273];
                            bool defunc_0_leq_res_39138 = sle64(gt_arg0_39137, gt_arg1_39133);
                            bool defunc_0_lifted_gt_res_39139 = !defunc_0_leq_res_39138;
                            
                            loop_cond_37275 = defunc_0_lifted_gt_res_39139;
                        } else {
                            loop_cond_37275 = 0;
                        }
                        
                        bool loop_while_tmp_40223 = loop_cond_37275;
                        int64_t j_tmp_40226 = tmp_37273;
                        
                        loop_while_37252 = loop_while_tmp_40223;
                        j_37255 = j_tmp_40226;
                    }
                    s_37248 = loop_while_37252;
                    s_37251 = j_37255;
                }
            }
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_39600, i_39481 * block_sizze_37178, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_39613, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {block_sizze_37178});
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_39603, i_39481 * block_sizze_37178, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_39615, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {block_sizze_37178});
        }
        if (memblock_alloc(ctx, &mem_39668, bytes_39599, "mem_39668")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 2, (uint64_t *) mem_39668.mem, (int64_t) 0, (int64_t []) {block_sizze_37178, (int64_t) 1}, (uint64_t *) mem_39600, (int64_t) 0, (int64_t []) {block_sizze_37178, (int64_t) 1}, (int64_t []) {smallest_pow_2_geq_than_res_37168, block_sizze_37178});
        if (memblock_alloc(ctx, &mem_39672, bytes_39599, "mem_39672")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 2, (uint64_t *) mem_39672.mem, (int64_t) 0, (int64_t []) {block_sizze_37178, (int64_t) 1}, (uint64_t *) mem_39603, (int64_t) 0, (int64_t []) {block_sizze_37178, (int64_t) 1}, (int64_t []) {smallest_pow_2_geq_than_res_37168, block_sizze_37178});
        
        int64_t data_37288;
        bool data_37289;
        int64_t data_37292;
        int64_t loop_dz2084Uz2082U_37293;
        bool loop_while_37294;
        int64_t stride_37297;
        
        if (memblock_set(ctx, &mem_param_39676, &mem_39668, "mem_39668") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_39679, &mem_39672, "mem_39672") != 0)
            return 1;
        loop_dz2084Uz2082U_37293 = flat_dim_37179;
        loop_while_37294 = loop_cond_37285;
        stride_37297 = block_sizze_37178;
        while (loop_while_37294) {
            int64_t next_stride_37298 = mul64((int64_t) 2, stride_37297);
            bool zzero_37299 = next_stride_37298 == (int64_t) 0;
            bool nonzzero_37300 = !zzero_37299;
            bool nonzzero_cert_37301;
            
            if (!nonzzero_37300) {
                set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:254:33-46\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftSMJ.fut:104:60-63\n   #4  test_matching.fut:51:43-47\n   #5  test_matching.fut:45:1-54:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t num_merges_37302 = sdiv64(flat_dim_37179, next_stride_37298);
            bool bounds_invalid_upwards_37303 = slt64(num_merges_37302, (int64_t) 0);
            bool valid_37304 = !bounds_invalid_upwards_37303;
            bool range_valid_c_37305;
            
            if (!valid_37304) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) num_merges_37302, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #3  /prelude/functional.fut:9:44-45\n   #4  ../../../joins/ftSMJ.fut:104:60-63\n   #5  test_matching.fut:51:43-47\n   #6  test_matching.fut:45:1-54:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t m_37307 = sub64(next_stride_37298, (int64_t) 1);
            bool zzero_leq_i_p_m_t_s_37308 = sle64((int64_t) 0, m_37307);
            bool cond_37310 = slt64(next_stride_37298, (int64_t) 10);
            bool protect_assert_disj_37311 = nonzzero_37287 || cond_37310;
            bool nonzzero_cert_37312;
            
            if (!protect_assert_disj_37311) {
                set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:147:30-42\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:51:43-47\n   #8  test_matching.fut:45:1-54:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t zeze_lhs_37313 = smod_safe64(next_stride_37298, merge_block_sizze_37183);
            bool num_blocks_37314 = zeze_lhs_37313 == (int64_t) 0;
            bool protect_assert_disj_37315 = cond_37310 || num_blocks_37314;
            bool assert_c_37316;
            
            if (!protect_assert_disj_37315) {
                set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "Assertion is false: (n % block_size == 0)", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:147:20-65\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:51:43-47\n   #8  test_matching.fut:45:1-54:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t num_blocks_37318 = sdiv_safe64(next_stride_37298, merge_block_sizze_37183);
            int64_t dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37319 = add64((int64_t) 1, num_blocks_37318);
            int64_t j_m_i_37320 = sub64(next_stride_37298, stride_37297);
            int64_t flat_dim_37321 = merge_block_sizze_37183 * num_blocks_37318;
            bool bounds_invalid_upwards_37322 = slt64(dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37319, (int64_t) 0);
            bool empty_slice_37323 = j_m_i_37320 == (int64_t) 0;
            int64_t m_37324 = sub64(j_m_i_37320, (int64_t) 1);
            bool zzero_lte_i_37325 = sle64((int64_t) 0, stride_37297);
            bool i_lte_j_37326 = sle64(stride_37297, next_stride_37298);
            bool empty_slice_37327 = stride_37297 == (int64_t) 0;
            int64_t m_37328 = sub64(stride_37297, (int64_t) 1);
            bool bounds_invalid_upwards_37329 = slt64(num_blocks_37318, (int64_t) 0);
            bool cond_37330 = slt64((int64_t) 0, stride_37297);
            bool valid_37331 = !bounds_invalid_upwards_37322;
            int64_t i_p_m_t_s_37332 = add64(stride_37297, m_37324);
            bool zzero_leq_i_p_m_t_s_37333 = sle64((int64_t) 0, m_37328);
            bool i_p_m_t_s_leq_w_37334 = slt64(m_37328, next_stride_37298);
            bool valid_37335 = !bounds_invalid_upwards_37329;
            bool i_p_m_t_s_leq_w_37336 = slt64(m_37307, flat_dim_37321);
            bool y_37337 = slt64((int64_t) 0, j_m_i_37320);
            bool protect_assert_disj_37338 = cond_37310 || valid_37331;
            bool range_valid_c_37339;
            
            if (!protect_assert_disj_37338) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37319, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  ../../../joins/ftSMJ.fut:104:60-63\n   #9  test_matching.fut:51:43-47\n   #10 test_matching.fut:45:1-54:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool zzero_leq_i_p_m_t_s_37340 = sle64((int64_t) 0, i_p_m_t_s_37332);
            bool i_p_m_t_s_leq_w_37341 = slt64(i_p_m_t_s_37332, next_stride_37298);
            bool y_37342 = zzero_leq_i_p_m_t_s_37333 && i_p_m_t_s_leq_w_37334;
            bool protect_assert_disj_37343 = cond_37310 || valid_37335;
            bool range_valid_c_37344;
            
            if (!protect_assert_disj_37343) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) num_blocks_37318, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ../../../joins/ftSMJ.fut:104:60-63\n   #8  test_matching.fut:51:43-47\n   #9  test_matching.fut:45:1-54:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool y_37345 = zzero_leq_i_p_m_t_s_37308 && i_p_m_t_s_leq_w_37336;
            bool y_37346 = zzero_lte_i_37325 && i_p_m_t_s_leq_w_37341;
            bool forwards_ok_37347 = zzero_lte_i_37325 && y_37342;
            bool ok_or_empty_37348 = zzero_37299 || y_37345;
            bool protect_assert_disj_37349 = cond_37330 || y_37337;
            bool y_37350 = zzero_leq_i_p_m_t_s_37340 && y_37346;
            bool ok_or_empty_37351 = empty_slice_37327 || forwards_ok_37347;
            bool protect_assert_disj_37352 = cond_37310 || ok_or_empty_37348;
            bool index_certs_37353;
            
            if (!protect_assert_disj_37352) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) next_stride_37298, "] out of bounds for array of shape [", (long long) flat_dim_37321, "].", "-> #0  /prelude/array.fut:46:45-51\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:155:3-46\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ../../../joins/ftSMJ.fut:104:60-63\n   #8  test_matching.fut:51:43-47\n   #9  test_matching.fut:45:1-54:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool loop_not_taken_37354 = !cond_37310;
            bool protect_assert_disj_37355 = protect_assert_disj_37349 || loop_not_taken_37354;
            bool index_certs_37356;
            
            if (!protect_assert_disj_37355) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) j_m_i_37320, "].", "-> #0  /prelude/array.fut:22:29-33\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:100:48-54\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  ../../../joins/ftSMJ.fut:104:60-63\n   #9  test_matching.fut:51:43-47\n   #10 test_matching.fut:45:1-54:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool forwards_ok_37357 = i_lte_j_37326 && y_37350;
            bool protect_assert_disj_37358 = cond_37310 || ok_or_empty_37351;
            bool index_certs_37359;
            
            if (!protect_assert_disj_37358) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) stride_37297, "] out of bounds for array of shape [", (long long) next_stride_37298, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:52-59\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  ../../../joins/ftSMJ.fut:104:60-63\n   #10 test_matching.fut:51:43-47\n   #11 test_matching.fut:45:1-54:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool protect_assert_disj_37360 = ok_or_empty_37351 || loop_not_taken_37354;
            bool index_certs_37361;
            
            if (!protect_assert_disj_37360) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) stride_37297, "] out of bounds for array of shape [", (long long) next_stride_37298, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:26-33\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:51:43-47\n   #8  test_matching.fut:45:1-54:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool ok_or_empty_37362 = empty_slice_37323 || forwards_ok_37357;
            bool protect_assert_disj_37363 = cond_37310 || ok_or_empty_37362;
            bool index_certs_37364;
            
            if (!protect_assert_disj_37363) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) stride_37297, ":] out of bounds for array of shape [", (long long) next_stride_37298, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:60-67\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  ../../../joins/ftSMJ.fut:104:60-63\n   #10 test_matching.fut:51:43-47\n   #11 test_matching.fut:45:1-54:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool protect_assert_disj_37365 = loop_not_taken_37354 || ok_or_empty_37362;
            bool index_certs_37366;
            
            if (!protect_assert_disj_37365) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) stride_37297, ":] out of bounds for array of shape [", (long long) next_stride_37298, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:34-41\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:51:43-47\n   #8  test_matching.fut:45:1-54:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t binop_x_39680 = (int64_t) 8 * num_merges_37302;
            int64_t bytes_39681 = next_stride_37298 * binop_x_39680;
            bool i_lte_j_37309 = sle64((int64_t) 0, next_stride_37298);
            
            if (mem_39682_cached_sizze_40367 < bytes_39681) {
                err = lexical_realloc(ctx, &mem_39682, &mem_39682_cached_sizze_40367, bytes_39681);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            if (mem_39685_cached_sizze_40368 < bytes_39681) {
                err = lexical_realloc(ctx, &mem_39685, &mem_39685_cached_sizze_40368, bytes_39681);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            
            int64_t bytes_39694 = (int64_t) 8 * dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37319;
            int64_t binop_x_39710 = (int64_t) 8 * num_blocks_37318;
            int64_t bytes_39711 = merge_block_sizze_37183 * binop_x_39710;
            int64_t bytes_39756 = (int64_t) 16 * stride_37297;
            
            for (int64_t i_39502 = 0; i_39502 < num_merges_37302; i_39502++) {
                int64_t start_37370 = mul64(next_stride_37298, i_39502);
                int64_t j_m_i_37371 = sub64(loop_dz2084Uz2082U_37293, start_37370);
                bool empty_slice_37372 = j_m_i_37371 == (int64_t) 0;
                int64_t m_37373 = sub64(j_m_i_37371, (int64_t) 1);
                int64_t i_p_m_t_s_37374 = add64(start_37370, m_37373);
                bool zzero_leq_i_p_m_t_s_37375 = sle64((int64_t) 0, i_p_m_t_s_37374);
                bool i_p_m_t_s_leq_w_37376 = slt64(i_p_m_t_s_37374, loop_dz2084Uz2082U_37293);
                bool zzero_lte_i_37377 = sle64((int64_t) 0, start_37370);
                bool i_lte_j_37378 = sle64(start_37370, loop_dz2084Uz2082U_37293);
                bool y_37379 = i_p_m_t_s_leq_w_37376 && zzero_lte_i_37377;
                bool y_37380 = zzero_leq_i_p_m_t_s_37375 && y_37379;
                bool forwards_ok_37381 = i_lte_j_37378 && y_37380;
                bool ok_or_empty_37382 = empty_slice_37372 || forwards_ok_37381;
                bool index_certs_37383;
                
                if (!ok_or_empty_37382) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) start_37370, ":] out of bounds for array of shape [", (long long) loop_dz2084Uz2082U_37293, "].", "-> #0  /prelude/array.fut:52:49-54\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:258:41-56\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:51:43-47\n   #8  test_matching.fut:45:1-54:44\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                bool i_p_m_t_s_leq_w_37384 = slt64(m_37307, j_m_i_37371);
                bool y_37385 = zzero_leq_i_p_m_t_s_37308 && i_p_m_t_s_leq_w_37384;
                bool forwards_ok_37386 = i_lte_j_37309 && y_37385;
                bool ok_or_empty_37387 = zzero_37299 || forwards_ok_37386;
                bool index_certs_37388;
                
                if (!ok_or_empty_37387) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) next_stride_37298, "] out of bounds for array of shape [", (long long) j_m_i_37371, "].", "-> #0  /prelude/array.fut:46:45-51\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:258:23-56\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:51:43-47\n   #8  test_matching.fut:45:1-54:44\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                if (cond_37310) {
                    int64_t dummy_39165;
                    int64_t dummy_39166;
                    
                    if (cond_37330) {
                        int64_t head_res_39167 = ((int64_t *) mem_param_39676.mem)[start_37370];
                        int64_t head_res_39168 = ((int64_t *) mem_param_39679.mem)[start_37370];
                        
                        dummy_39165 = head_res_39167;
                        dummy_39166 = head_res_39168;
                    } else {
                        int64_t slice_39169 = stride_37297 + start_37370;
                        int64_t head_res_39170 = ((int64_t *) mem_param_39676.mem)[slice_39169];
                        int64_t head_res_39171 = ((int64_t *) mem_param_39679.mem)[slice_39169];
                        
                        dummy_39165 = head_res_39170;
                        dummy_39166 = head_res_39171;
                    }
                    if (memblock_alloc(ctx, &mem_39757, bytes_39756, "mem_39757")) {
                        err = 1;
                        goto cleanup;
                    }
                    for (int64_t nest_i_40236 = 0; nest_i_40236 < next_stride_37298; nest_i_40236++) {
                        ((int64_t *) mem_39757.mem)[nest_i_40236] = dummy_39165;
                    }
                    if (memblock_alloc(ctx, &mem_39759, bytes_39756, "mem_39759")) {
                        err = 1;
                        goto cleanup;
                    }
                    for (int64_t nest_i_40237 = 0; nest_i_40237 < next_stride_37298; nest_i_40237++) {
                        ((int64_t *) mem_39759.mem)[nest_i_40237] = dummy_39166;
                    }
                    
                    int64_t data_39174;
                    int64_t i_39178 = (int64_t) 0;
                    
                    for (int64_t k_39177 = 0; k_39177 < next_stride_37298; k_39177++) {
                        int64_t j_39181 = sub64(k_39177, i_39178);
                        bool cond_39182 = j_39181 == j_m_i_37320;
                        bool cond_39183;
                        
                        if (cond_39182) {
                            cond_39183 = 1;
                        } else {
                            bool cond_39184 = slt64(i_39178, stride_37297);
                            bool cond_f_res_39185;
                            
                            if (cond_39184) {
                                bool x_39186 = sle64((int64_t) 0, j_39181);
                                bool y_39187 = slt64(j_39181, j_m_i_37320);
                                bool bounds_check_39188 = x_39186 && y_39187;
                                bool index_certs_39189;
                                
                                if (!bounds_check_39188) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_39181, "] out of bounds for array of shape [", (long long) j_m_i_37320, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:55-59\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ../../../joins/ftSMJ.fut:104:60-63\n   #8  test_matching.fut:51:43-47\n   #9  test_matching.fut:45:1-54:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                bool x_39193 = sle64((int64_t) 0, i_39178);
                                bool bounds_check_39194 = cond_39184 && x_39193;
                                bool index_certs_39195;
                                
                                if (!bounds_check_39194) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_39178, "] out of bounds for array of shape [", (long long) stride_37297, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:50-54\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ../../../joins/ftSMJ.fut:104:60-63\n   #8  test_matching.fut:51:43-47\n   #9  test_matching.fut:45:1-54:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t slice_39190 = stride_37297 + j_39181;
                                int64_t slice_39191 = start_37370 + slice_39190;
                                int64_t leq_arg1_39192 = ((int64_t *) mem_param_39676.mem)[slice_39191];
                                int64_t slice_39196 = start_37370 + i_39178;
                                int64_t leq_arg0_39197 = ((int64_t *) mem_param_39676.mem)[slice_39196];
                                bool defunc_0_leq_res_39198 = sle64(leq_arg0_39197, leq_arg1_39192);
                                
                                cond_f_res_39185 = defunc_0_leq_res_39198;
                            } else {
                                cond_f_res_39185 = 0;
                            }
                            cond_39183 = cond_f_res_39185;
                        }
                        
                        int64_t loopres_39199;
                        int64_t loopres_39200;
                        int64_t loopres_39201;
                        
                        if (cond_39183) {
                            bool x_39203 = sle64((int64_t) 0, i_39178);
                            bool y_39204 = slt64(i_39178, stride_37297);
                            bool bounds_check_39205 = x_39203 && y_39204;
                            bool index_certs_39206;
                            
                            if (!bounds_check_39205) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_39178, "] out of bounds for array of shape [", (long long) stride_37297, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:106:17-21\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ../../../joins/ftSMJ.fut:104:60-63\n   #8  test_matching.fut:51:43-47\n   #9  test_matching.fut:45:1-54:44\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t tmp_39202 = add64((int64_t) 1, i_39178);
                            int64_t slice_39207 = start_37370 + i_39178;
                            int64_t tmp_39208 = ((int64_t *) mem_param_39676.mem)[slice_39207];
                            int64_t tmp_39209 = ((int64_t *) mem_param_39679.mem)[slice_39207];
                            
                            loopres_39199 = tmp_39202;
                            loopres_39200 = tmp_39208;
                            loopres_39201 = tmp_39209;
                        } else {
                            bool x_39210 = sle64((int64_t) 0, j_39181);
                            bool y_39211 = slt64(j_39181, j_m_i_37320);
                            bool bounds_check_39212 = x_39210 && y_39211;
                            bool index_certs_39213;
                            
                            if (!bounds_check_39212) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_39181, "] out of bounds for array of shape [", (long long) j_m_i_37320, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:108:15-19\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ../../../joins/ftSMJ.fut:104:60-63\n   #8  test_matching.fut:51:43-47\n   #9  test_matching.fut:45:1-54:44\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t slice_39214 = stride_37297 + j_39181;
                            int64_t slice_39215 = start_37370 + slice_39214;
                            int64_t tmp_39216 = ((int64_t *) mem_param_39676.mem)[slice_39215];
                            int64_t tmp_39217 = ((int64_t *) mem_param_39679.mem)[slice_39215];
                            
                            loopres_39199 = i_39178;
                            loopres_39200 = tmp_39216;
                            loopres_39201 = tmp_39217;
                        }
                        ((int64_t *) mem_39757.mem)[k_39177] = loopres_39200;
                        ((int64_t *) mem_39759.mem)[k_39177] = loopres_39201;
                        
                        int64_t i_tmp_40238 = loopres_39199;
                        
                        i_39178 = i_tmp_40238;
                    }
                    data_39174 = i_39178;
                    if (memblock_set(ctx, &ext_mem_39777, &mem_39757, "mem_39757") != 0)
                        return 1;
                    if (memblock_set(ctx, &ext_mem_39774, &mem_39759, "mem_39759") != 0)
                        return 1;
                } else {
                    if (mem_39695_cached_sizze_40369 < bytes_39694) {
                        err = lexical_realloc(ctx, &mem_39695, &mem_39695_cached_sizze_40369, bytes_39694);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    if (mem_39697_cached_sizze_40370 < bytes_39694) {
                        err = lexical_realloc(ctx, &mem_39697, &mem_39697_cached_sizze_40370, bytes_39694);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    for (int64_t i_39488 = 0; i_39488 < dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37319; i_39488++) {
                        int64_t split_count_arg3_37450 = mul64(merge_block_sizze_37183, i_39488);
                        int64_t defunc_0_split_count_res_37451;
                        int64_t defunc_0_split_count_res_37452;
                        int64_t defunc_0_split_count_res_37453;
                        int64_t defunc_0_split_count_res_37454;
                        int64_t defunc_0_split_count_res_37455;
                        
                        if (futrts_lifted_normalizze_7743(ctx, &defunc_0_split_count_res_37451, &defunc_0_split_count_res_37452, &defunc_0_split_count_res_37453, &defunc_0_split_count_res_37454, &defunc_0_split_count_res_37455, (int64_t) 0, stride_37297, (int64_t) 0, j_m_i_37320, split_count_arg3_37450) != 0) {
                            err = 1;
                            goto cleanup;
                        }
                        
                        bool loop_cond_37456 = slt64((int64_t) 0, defunc_0_split_count_res_37455);
                        bool defunc_0_split_count_res_37457;
                        int64_t defunc_0_split_count_res_37458;
                        int64_t defunc_0_split_count_res_37459;
                        int64_t defunc_0_split_count_res_37460;
                        int64_t defunc_0_split_count_res_37461;
                        int64_t defunc_0_split_count_res_37462;
                        bool loop_while_37463;
                        int64_t ss_37464;
                        int64_t ss_37465;
                        int64_t tt_37466;
                        int64_t tt_37467;
                        int64_t count_37468;
                        
                        loop_while_37463 = loop_cond_37456;
                        ss_37464 = defunc_0_split_count_res_37451;
                        ss_37465 = defunc_0_split_count_res_37452;
                        tt_37466 = defunc_0_split_count_res_37453;
                        tt_37467 = defunc_0_split_count_res_37454;
                        count_37468 = defunc_0_split_count_res_37455;
                        while (loop_while_37463) {
                            int64_t zlze_lhs_37469 = sub64(ss_37465, ss_37464);
                            bool cond_37470 = sle64(zlze_lhs_37469, (int64_t) 0);
                            int64_t defunc_0_lifted_step_res_37471;
                            int64_t defunc_0_lifted_step_res_37472;
                            int64_t defunc_0_lifted_step_res_37473;
                            int64_t defunc_0_lifted_step_res_37474;
                            
                            if (cond_37470) {
                                int64_t tmp_39220 = add64(tt_37466, count_37468);
                                
                                defunc_0_lifted_step_res_37471 = ss_37464;
                                defunc_0_lifted_step_res_37472 = tmp_39220;
                                defunc_0_lifted_step_res_37473 = tt_37467;
                                defunc_0_lifted_step_res_37474 = (int64_t) 0;
                            } else {
                                int64_t zlze_lhs_37476 = sub64(tt_37467, tt_37466);
                                bool cond_37477 = sle64(zlze_lhs_37476, (int64_t) 0);
                                int64_t defunc_0_lifted_step_res_f_res_37478;
                                int64_t defunc_0_lifted_step_res_f_res_37479;
                                int64_t defunc_0_lifted_step_res_f_res_37480;
                                int64_t defunc_0_lifted_step_res_f_res_37481;
                                
                                if (cond_37477) {
                                    int64_t tmp_39221 = add64(ss_37464, count_37468);
                                    
                                    defunc_0_lifted_step_res_f_res_37478 = tmp_39221;
                                    defunc_0_lifted_step_res_f_res_37479 = tt_37466;
                                    defunc_0_lifted_step_res_f_res_37480 = tt_37467;
                                    defunc_0_lifted_step_res_f_res_37481 = (int64_t) 0;
                                } else {
                                    bool cond_37483 = count_37468 == (int64_t) 1;
                                    int64_t defunc_0_lifted_step_res_f_res_f_res_37484;
                                    int64_t defunc_0_lifted_step_res_f_res_f_res_37485;
                                    int64_t defunc_0_lifted_step_res_f_res_f_res_37486;
                                    int64_t defunc_0_lifted_step_res_f_res_f_res_37487;
                                    
                                    if (cond_37483) {
                                        bool x_39223 = sle64((int64_t) 0, ss_37464);
                                        bool y_39224 = slt64(ss_37464, stride_37297);
                                        bool bounds_check_39225 = x_39223 && y_39224;
                                        bool index_certs_39226;
                                        
                                        if (!bounds_check_39225) {
                                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) ss_37464, "] out of bounds for array of shape [", (long long) stride_37297, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:53:21-27\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 ../../../joins/ftSMJ.fut:104:60-63\n   #12 test_matching.fut:51:43-47\n   #13 test_matching.fut:45:1-54:44\n"));
                                            err = FUTHARK_PROGRAM_ERROR;
                                            goto cleanup;
                                        }
                                        
                                        bool x_39229 = sle64((int64_t) 0, tt_37466);
                                        bool y_39230 = slt64(tt_37466, j_m_i_37320);
                                        bool bounds_check_39231 = x_39229 && y_39230;
                                        bool index_certs_39232;
                                        
                                        if (!bounds_check_39231) {
                                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tt_37466, "] out of bounds for array of shape [", (long long) j_m_i_37320, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:53:14-20\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 ../../../joins/ftSMJ.fut:104:60-63\n   #12 test_matching.fut:51:43-47\n   #13 test_matching.fut:45:1-54:44\n"));
                                            err = FUTHARK_PROGRAM_ERROR;
                                            goto cleanup;
                                        }
                                        
                                        int64_t slice_39227 = start_37370 + ss_37464;
                                        int64_t leq_arg1_39228 = ((int64_t *) mem_param_39676.mem)[slice_39227];
                                        int64_t slice_39233 = stride_37297 + tt_37466;
                                        int64_t slice_39234 = start_37370 + slice_39233;
                                        int64_t leq_arg0_39235 = ((int64_t *) mem_param_39676.mem)[slice_39234];
                                        bool defunc_0_leq_res_39236 = sle64(leq_arg0_39235, leq_arg1_39228);
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_t_res_39237;
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_t_res_39238;
                                        
                                        if (defunc_0_leq_res_39236) {
                                            int64_t tmp_39239 = add64((int64_t) 1, tt_37466);
                                            
                                            defunc_0_lifted_step_res_f_res_f_res_t_res_39237 = ss_37464;
                                            defunc_0_lifted_step_res_f_res_f_res_t_res_39238 = tmp_39239;
                                        } else {
                                            int64_t tmp_39240 = add64((int64_t) 1, ss_37464);
                                            
                                            defunc_0_lifted_step_res_f_res_f_res_t_res_39237 = tmp_39240;
                                            defunc_0_lifted_step_res_f_res_f_res_t_res_39238 = tt_37466;
                                        }
                                        defunc_0_lifted_step_res_f_res_f_res_37484 = defunc_0_lifted_step_res_f_res_f_res_t_res_39237;
                                        defunc_0_lifted_step_res_f_res_f_res_37485 = defunc_0_lifted_step_res_f_res_f_res_t_res_39238;
                                        defunc_0_lifted_step_res_f_res_f_res_37486 = tt_37467;
                                        defunc_0_lifted_step_res_f_res_f_res_37487 = (int64_t) 0;
                                    } else {
                                        int64_t m_37506 = sdiv64(count_37468, (int64_t) 2);
                                        int64_t n_37507 = sub64(count_37468, m_37506);
                                        bool cond_37508 = n_37507 == (int64_t) 0;
                                        int64_t zgze_lhs_37509 = add64(ss_37464, m_37506);
                                        bool cond_f_res_37510 = sle64(ss_37465, zgze_lhs_37509);
                                        bool x_37511 = !cond_37508;
                                        bool y_37512 = cond_f_res_37510 && x_37511;
                                        bool cond_37513 = cond_37508 || y_37512;
                                        bool leq_y_x_37514;
                                        
                                        if (cond_37513) {
                                            leq_y_x_37514 = 1;
                                        } else {
                                            bool x_37515 = sle64((int64_t) 0, zgze_lhs_37509);
                                            bool y_37516 = slt64(zgze_lhs_37509, stride_37297);
                                            bool bounds_check_37517 = x_37515 && y_37516;
                                            bool index_certs_37518;
                                            
                                            if (!bounds_check_37517) {
                                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) zgze_lhs_37509, "] out of bounds for array of shape [", (long long) stride_37297, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:73:22-30\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 ../../../joins/ftSMJ.fut:104:60-63\n   #12 test_matching.fut:51:43-47\n   #13 test_matching.fut:45:1-54:44\n"));
                                                err = FUTHARK_PROGRAM_ERROR;
                                                goto cleanup;
                                            }
                                            
                                            int64_t zm_lhs_37521 = add64(tt_37466, n_37507);
                                            int64_t leq_arg0_37522 = sub64(zm_lhs_37521, (int64_t) 1);
                                            bool x_37523 = sle64((int64_t) 0, leq_arg0_37522);
                                            bool y_37524 = slt64(leq_arg0_37522, j_m_i_37320);
                                            bool bounds_check_37525 = x_37523 && y_37524;
                                            bool index_certs_37526;
                                            
                                            if (!bounds_check_37525) {
                                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) leq_arg0_37522, "] out of bounds for array of shape [", (long long) j_m_i_37320, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:73:11-21\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 ../../../joins/ftSMJ.fut:104:60-63\n   #12 test_matching.fut:51:43-47\n   #13 test_matching.fut:45:1-54:44\n"));
                                                err = FUTHARK_PROGRAM_ERROR;
                                                goto cleanup;
                                            }
                                            
                                            int64_t slice_37519 = start_37370 + zgze_lhs_37509;
                                            int64_t leq_arg1_37520 = ((int64_t *) mem_param_39676.mem)[slice_37519];
                                            int64_t slice_37527 = stride_37297 + leq_arg0_37522;
                                            int64_t slice_37528 = start_37370 + slice_37527;
                                            int64_t leq_arg0_37529 = ((int64_t *) mem_param_39676.mem)[slice_37528];
                                            bool defunc_0_leq_res_37530 = sle64(leq_arg0_37529, leq_arg1_37520);
                                            
                                            leq_y_x_37514 = defunc_0_leq_res_37530;
                                        }
                                        
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_f_res_37531;
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_f_res_37532;
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_f_res_37533;
                                        
                                        if (leq_y_x_37514) {
                                            int64_t tmp_39241 = add64(tt_37466, n_37507);
                                            int64_t tmp_39242 = sub64(count_37468, n_37507);
                                            
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_37531 = tmp_39241;
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_37532 = tt_37467;
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_37533 = tmp_39242;
                                        } else {
                                            int64_t tmp_37536 = add64(tt_37466, n_37507);
                                            
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_37531 = tt_37466;
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_37532 = tmp_37536;
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_37533 = count_37468;
                                        }
                                        defunc_0_lifted_step_res_f_res_f_res_37484 = ss_37464;
                                        defunc_0_lifted_step_res_f_res_f_res_37485 = defunc_0_lifted_step_res_f_res_f_res_f_res_37531;
                                        defunc_0_lifted_step_res_f_res_f_res_37486 = defunc_0_lifted_step_res_f_res_f_res_f_res_37532;
                                        defunc_0_lifted_step_res_f_res_f_res_37487 = defunc_0_lifted_step_res_f_res_f_res_f_res_37533;
                                    }
                                    defunc_0_lifted_step_res_f_res_37478 = defunc_0_lifted_step_res_f_res_f_res_37484;
                                    defunc_0_lifted_step_res_f_res_37479 = defunc_0_lifted_step_res_f_res_f_res_37485;
                                    defunc_0_lifted_step_res_f_res_37480 = defunc_0_lifted_step_res_f_res_f_res_37486;
                                    defunc_0_lifted_step_res_f_res_37481 = defunc_0_lifted_step_res_f_res_f_res_37487;
                                }
                                defunc_0_lifted_step_res_37471 = defunc_0_lifted_step_res_f_res_37478;
                                defunc_0_lifted_step_res_37472 = defunc_0_lifted_step_res_f_res_37479;
                                defunc_0_lifted_step_res_37473 = defunc_0_lifted_step_res_f_res_37480;
                                defunc_0_lifted_step_res_37474 = defunc_0_lifted_step_res_f_res_37481;
                            }
                            
                            int64_t loopres_37537;
                            int64_t loopres_37538;
                            int64_t loopres_37539;
                            int64_t loopres_37540;
                            int64_t loopres_37541;
                            
                            if (futrts_lifted_normalizze_7743(ctx, &loopres_37537, &loopres_37538, &loopres_37539, &loopres_37540, &loopres_37541, defunc_0_lifted_step_res_37471, ss_37465, defunc_0_lifted_step_res_37472, defunc_0_lifted_step_res_37473, defunc_0_lifted_step_res_37474) != 0) {
                                err = 1;
                                goto cleanup;
                            }
                            
                            bool loop_cond_37542 = slt64((int64_t) 0, loopres_37541);
                            bool loop_while_tmp_40243 = loop_cond_37542;
                            int64_t ss_tmp_40244 = loopres_37537;
                            int64_t ss_tmp_40245 = loopres_37538;
                            int64_t tt_tmp_40246 = loopres_37539;
                            int64_t tt_tmp_40247 = loopres_37540;
                            int64_t count_tmp_40248 = loopres_37541;
                            
                            loop_while_37463 = loop_while_tmp_40243;
                            ss_37464 = ss_tmp_40244;
                            ss_37465 = ss_tmp_40245;
                            tt_37466 = tt_tmp_40246;
                            tt_37467 = tt_tmp_40247;
                            count_37468 = count_tmp_40248;
                        }
                        defunc_0_split_count_res_37457 = loop_while_37463;
                        defunc_0_split_count_res_37458 = ss_37464;
                        defunc_0_split_count_res_37459 = ss_37465;
                        defunc_0_split_count_res_37460 = tt_37466;
                        defunc_0_split_count_res_37461 = tt_37467;
                        defunc_0_split_count_res_37462 = count_37468;
                        ((int64_t *) mem_39695)[i_39488] = defunc_0_split_count_res_37458;
                        ((int64_t *) mem_39697)[i_39488] = defunc_0_split_count_res_37460;
                    }
                    if (mem_39712_cached_sizze_40371 < bytes_39711) {
                        err = lexical_realloc(ctx, &mem_39712, &mem_39712_cached_sizze_40371, bytes_39711);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    if (mem_39715_cached_sizze_40372 < bytes_39711) {
                        err = lexical_realloc(ctx, &mem_39715, &mem_39715_cached_sizze_40372, bytes_39711);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    for (int64_t i_39495 = 0; i_39495 < num_blocks_37318; i_39495++) {
                        bool y_37548 = slt64(i_39495, dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37319);
                        bool index_certs_37550;
                        
                        if (!y_37548) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_39495, "] out of bounds for array of shape [", (long long) dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37319, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:151:22-34\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  ../../../joins/ftSMJ.fut:104:60-63\n   #9  test_matching.fut:51:43-47\n   #10 test_matching.fut:45:1-54:44\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t defunc_0_f_res_37551 = ((int64_t *) mem_39695)[i_39495];
                        int64_t defunc_0_f_res_37552 = ((int64_t *) mem_39697)[i_39495];
                        int64_t tmp_37553 = add64((int64_t) 1, i_39495);
                        bool x_37554 = sle64((int64_t) 0, tmp_37553);
                        bool y_37555 = slt64(tmp_37553, dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37319);
                        bool bounds_check_37556 = x_37554 && y_37555;
                        bool index_certs_37557;
                        
                        if (!bounds_check_37556) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_37553, "] out of bounds for array of shape [", (long long) dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37319, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:152:22-36\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  ../../../joins/ftSMJ.fut:104:60-63\n   #9  test_matching.fut:51:43-47\n   #10 test_matching.fut:45:1-54:44\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t defunc_0_f_res_37558 = ((int64_t *) mem_39695)[tmp_37553];
                        int64_t defunc_0_f_res_37559 = ((int64_t *) mem_39697)[tmp_37553];
                        int64_t merge_sequential_arg2_37560 = add64(stride_37297, defunc_0_f_res_37552);
                        int64_t merge_sequential_arg2_37561 = add64(stride_37297, defunc_0_f_res_37559);
                        int64_t j_m_i_37562 = sub64(merge_sequential_arg2_37561, merge_sequential_arg2_37560);
                        bool empty_slice_37563 = j_m_i_37562 == (int64_t) 0;
                        int64_t m_37564 = sub64(j_m_i_37562, (int64_t) 1);
                        int64_t i_p_m_t_s_37565 = add64(merge_sequential_arg2_37560, m_37564);
                        bool zzero_leq_i_p_m_t_s_37566 = sle64((int64_t) 0, i_p_m_t_s_37565);
                        bool i_p_m_t_s_leq_w_37567 = slt64(i_p_m_t_s_37565, next_stride_37298);
                        bool zzero_lte_i_37568 = sle64((int64_t) 0, merge_sequential_arg2_37560);
                        bool i_lte_j_37569 = sle64(merge_sequential_arg2_37560, merge_sequential_arg2_37561);
                        bool y_37570 = i_p_m_t_s_leq_w_37567 && zzero_lte_i_37568;
                        bool y_37571 = zzero_leq_i_p_m_t_s_37566 && y_37570;
                        bool forwards_ok_37572 = i_lte_j_37569 && y_37571;
                        bool ok_or_empty_37573 = empty_slice_37563 || forwards_ok_37572;
                        bool index_certs_37574;
                        
                        if (!ok_or_empty_37573) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) merge_sequential_arg2_37560, ":", (long long) merge_sequential_arg2_37561, "] out of bounds for array of shape [", (long long) next_stride_37298, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:40-58\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  ../../../joins/ftSMJ.fut:104:60-63\n   #9  test_matching.fut:51:43-47\n   #10 test_matching.fut:45:1-54:44\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t j_m_i_37575 = sub64(defunc_0_f_res_37558, defunc_0_f_res_37551);
                        bool empty_slice_37576 = j_m_i_37575 == (int64_t) 0;
                        int64_t m_37577 = sub64(j_m_i_37575, (int64_t) 1);
                        int64_t i_p_m_t_s_37578 = add64(defunc_0_f_res_37551, m_37577);
                        bool zzero_leq_i_p_m_t_s_37579 = sle64((int64_t) 0, i_p_m_t_s_37578);
                        bool i_p_m_t_s_leq_w_37580 = slt64(i_p_m_t_s_37578, next_stride_37298);
                        bool zzero_lte_i_37581 = sle64((int64_t) 0, defunc_0_f_res_37551);
                        bool i_lte_j_37582 = sle64(defunc_0_f_res_37551, defunc_0_f_res_37558);
                        bool y_37583 = i_p_m_t_s_leq_w_37580 && zzero_lte_i_37581;
                        bool y_37584 = zzero_leq_i_p_m_t_s_37579 && y_37583;
                        bool forwards_ok_37585 = i_lte_j_37582 && y_37584;
                        bool ok_or_empty_37586 = empty_slice_37576 || forwards_ok_37585;
                        bool index_certs_37587;
                        
                        if (!ok_or_empty_37586) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) defunc_0_f_res_37551, ":", (long long) defunc_0_f_res_37558, "] out of bounds for array of shape [", (long long) next_stride_37298, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:29-39\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  ../../../joins/ftSMJ.fut:104:60-63\n   #9  test_matching.fut:51:43-47\n   #10 test_matching.fut:45:1-54:44\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        bool cond_37588 = slt64((int64_t) 0, j_m_i_37575);
                        int64_t dummy_37589;
                        int64_t dummy_37590;
                        
                        if (cond_37588) {
                            bool index_certs_39243;
                            
                            if (!cond_37588) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) j_m_i_37575, "].", "-> #0  /prelude/array.fut:22:29-33\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:100:36-42\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #6  /prelude/soacs.fut:59:9-11\n   #7  /prelude/array.fut:241:11-17\n   #8  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #9  /prelude/functional.fut:9:44-45\n   #10 ../../../joins/ftSMJ.fut:104:60-63\n   #11 test_matching.fut:51:43-47\n   #12 test_matching.fut:45:1-54:44\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t slice_39244 = start_37370 + defunc_0_f_res_37551;
                            int64_t head_res_39245 = ((int64_t *) mem_param_39676.mem)[slice_39244];
                            int64_t head_res_39246 = ((int64_t *) mem_param_39679.mem)[slice_39244];
                            
                            dummy_37589 = head_res_39245;
                            dummy_37590 = head_res_39246;
                        } else {
                            bool y_37595 = slt64((int64_t) 0, j_m_i_37562);
                            bool index_certs_37596;
                            
                            if (!y_37595) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) j_m_i_37562, "].", "-> #0  /prelude/array.fut:22:29-33\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:100:48-54\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #6  /prelude/soacs.fut:59:9-11\n   #7  /prelude/array.fut:241:11-17\n   #8  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #9  /prelude/functional.fut:9:44-45\n   #10 ../../../joins/ftSMJ.fut:104:60-63\n   #11 test_matching.fut:51:43-47\n   #12 test_matching.fut:45:1-54:44\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t slice_37597 = start_37370 + merge_sequential_arg2_37560;
                            int64_t head_res_37598 = ((int64_t *) mem_param_39676.mem)[slice_37597];
                            int64_t head_res_37599 = ((int64_t *) mem_param_39679.mem)[slice_37597];
                            
                            dummy_37589 = head_res_37598;
                            dummy_37590 = head_res_37599;
                        }
                        for (int64_t nest_i_40251 = 0; nest_i_40251 < merge_block_sizze_37183; nest_i_40251++) {
                            ((int64_t *) mem_39712)[i_39495 * merge_block_sizze_37183 + nest_i_40251] = dummy_37589;
                        }
                        for (int64_t nest_i_40252 = 0; nest_i_40252 < merge_block_sizze_37183; nest_i_40252++) {
                            ((int64_t *) mem_39715)[i_39495 * merge_block_sizze_37183 + nest_i_40252] = dummy_37590;
                        }
                        
                        int64_t data_37602;
                        int64_t i_37606 = (int64_t) 0;
                        
                        for (int64_t k_37605 = 0; k_37605 < merge_block_sizze_37183; k_37605++) {
                            int64_t j_37609 = sub64(k_37605, i_37606);
                            bool cond_37610 = j_37609 == j_m_i_37562;
                            bool cond_37611;
                            
                            if (cond_37610) {
                                cond_37611 = 1;
                            } else {
                                bool cond_37612 = slt64(i_37606, j_m_i_37575);
                                bool cond_f_res_37613;
                                
                                if (cond_37612) {
                                    bool x_39247 = sle64((int64_t) 0, j_37609);
                                    bool y_39248 = slt64(j_37609, j_m_i_37562);
                                    bool bounds_check_39249 = x_39247 && y_39248;
                                    bool index_certs_39250;
                                    
                                    if (!bounds_check_39249) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_37609, "] out of bounds for array of shape [", (long long) j_m_i_37562, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:55-59\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  ../../../joins/ftSMJ.fut:104:60-63\n   #10 test_matching.fut:51:43-47\n   #11 test_matching.fut:45:1-54:44\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    bool x_39254 = sle64((int64_t) 0, i_37606);
                                    bool bounds_check_39255 = cond_37612 && x_39254;
                                    bool index_certs_39256;
                                    
                                    if (!bounds_check_39255) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_37606, "] out of bounds for array of shape [", (long long) j_m_i_37575, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:50-54\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  ../../../joins/ftSMJ.fut:104:60-63\n   #10 test_matching.fut:51:43-47\n   #11 test_matching.fut:45:1-54:44\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    int64_t slice_39251 = merge_sequential_arg2_37560 + j_37609;
                                    int64_t slice_39252 = start_37370 + slice_39251;
                                    int64_t leq_arg1_39253 = ((int64_t *) mem_param_39676.mem)[slice_39252];
                                    int64_t slice_39257 = defunc_0_f_res_37551 + i_37606;
                                    int64_t slice_39258 = start_37370 + slice_39257;
                                    int64_t leq_arg0_39259 = ((int64_t *) mem_param_39676.mem)[slice_39258];
                                    bool defunc_0_leq_res_39260 = sle64(leq_arg0_39259, leq_arg1_39253);
                                    
                                    cond_f_res_37613 = defunc_0_leq_res_39260;
                                } else {
                                    cond_f_res_37613 = 0;
                                }
                                cond_37611 = cond_f_res_37613;
                            }
                            
                            int64_t loopres_37628;
                            int64_t loopres_37629;
                            int64_t loopres_37630;
                            
                            if (cond_37611) {
                                bool x_39262 = sle64((int64_t) 0, i_37606);
                                bool y_39263 = slt64(i_37606, j_m_i_37575);
                                bool bounds_check_39264 = x_39262 && y_39263;
                                bool index_certs_39265;
                                
                                if (!bounds_check_39264) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_37606, "] out of bounds for array of shape [", (long long) j_m_i_37575, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:106:17-21\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  ../../../joins/ftSMJ.fut:104:60-63\n   #10 test_matching.fut:51:43-47\n   #11 test_matching.fut:45:1-54:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t tmp_39261 = add64((int64_t) 1, i_37606);
                                int64_t slice_39266 = defunc_0_f_res_37551 + i_37606;
                                int64_t slice_39267 = start_37370 + slice_39266;
                                int64_t tmp_39268 = ((int64_t *) mem_param_39676.mem)[slice_39267];
                                int64_t tmp_39269 = ((int64_t *) mem_param_39679.mem)[slice_39267];
                                
                                loopres_37628 = tmp_39261;
                                loopres_37629 = tmp_39268;
                                loopres_37630 = tmp_39269;
                            } else {
                                bool x_37640 = sle64((int64_t) 0, j_37609);
                                bool y_37641 = slt64(j_37609, j_m_i_37562);
                                bool bounds_check_37642 = x_37640 && y_37641;
                                bool index_certs_37643;
                                
                                if (!bounds_check_37642) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_37609, "] out of bounds for array of shape [", (long long) j_m_i_37562, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:108:15-19\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  ../../../joins/ftSMJ.fut:104:60-63\n   #10 test_matching.fut:51:43-47\n   #11 test_matching.fut:45:1-54:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t slice_37644 = merge_sequential_arg2_37560 + j_37609;
                                int64_t slice_37645 = start_37370 + slice_37644;
                                int64_t tmp_37646 = ((int64_t *) mem_param_39676.mem)[slice_37645];
                                int64_t tmp_37647 = ((int64_t *) mem_param_39679.mem)[slice_37645];
                                
                                loopres_37628 = i_37606;
                                loopres_37629 = tmp_37646;
                                loopres_37630 = tmp_37647;
                            }
                            ((int64_t *) mem_39712)[i_39495 * merge_block_sizze_37183 + k_37605] = loopres_37629;
                            ((int64_t *) mem_39715)[i_39495 * merge_block_sizze_37183 + k_37605] = loopres_37630;
                            
                            int64_t i_tmp_40253 = loopres_37628;
                            
                            i_37606 = i_tmp_40253;
                        }
                        data_37602 = i_37606;
                    }
                    if (memblock_alloc(ctx, &mem_39750, bytes_39711, "mem_39750")) {
                        err = 1;
                        goto cleanup;
                    }
                    lmad_copy_8b(ctx, 2, (uint64_t *) mem_39750.mem, (int64_t) 0, (int64_t []) {merge_block_sizze_37183, (int64_t) 1}, (uint64_t *) mem_39712, (int64_t) 0, (int64_t []) {merge_block_sizze_37183, (int64_t) 1}, (int64_t []) {num_blocks_37318, merge_block_sizze_37183});
                    if (memblock_alloc(ctx, &mem_39754, bytes_39711, "mem_39754")) {
                        err = 1;
                        goto cleanup;
                    }
                    lmad_copy_8b(ctx, 2, (uint64_t *) mem_39754.mem, (int64_t) 0, (int64_t []) {merge_block_sizze_37183, (int64_t) 1}, (uint64_t *) mem_39715, (int64_t) 0, (int64_t []) {merge_block_sizze_37183, (int64_t) 1}, (int64_t []) {num_blocks_37318, merge_block_sizze_37183});
                    if (memblock_set(ctx, &ext_mem_39777, &mem_39750, "mem_39750") != 0)
                        return 1;
                    if (memblock_set(ctx, &ext_mem_39774, &mem_39754, "mem_39754") != 0)
                        return 1;
                }
                lmad_copy_8b(ctx, 1, (uint64_t *) mem_39682, i_39502 * next_stride_37298, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_39777.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {next_stride_37298});
                if (memblock_unref(ctx, &ext_mem_39777, "ext_mem_39777") != 0)
                    return 1;
                lmad_copy_8b(ctx, 1, (uint64_t *) mem_39685, i_39502 * next_stride_37298, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_39774.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {next_stride_37298});
                if (memblock_unref(ctx, &ext_mem_39774, "ext_mem_39774") != 0)
                    return 1;
            }
            
            int64_t flat_dim_37654 = next_stride_37298 * num_merges_37302;
            
            if (memblock_alloc(ctx, &mem_39788, bytes_39681, "mem_39788")) {
                err = 1;
                goto cleanup;
            }
            lmad_copy_8b(ctx, 2, (uint64_t *) mem_39788.mem, (int64_t) 0, (int64_t []) {next_stride_37298, (int64_t) 1}, (uint64_t *) mem_39682, (int64_t) 0, (int64_t []) {next_stride_37298, (int64_t) 1}, (int64_t []) {num_merges_37302, next_stride_37298});
            if (memblock_alloc(ctx, &mem_39792, bytes_39681, "mem_39792")) {
                err = 1;
                goto cleanup;
            }
            lmad_copy_8b(ctx, 2, (uint64_t *) mem_39792.mem, (int64_t) 0, (int64_t []) {next_stride_37298, (int64_t) 1}, (uint64_t *) mem_39685, (int64_t) 0, (int64_t []) {next_stride_37298, (int64_t) 1}, (int64_t []) {num_merges_37302, next_stride_37298});
            
            bool loop_cond_37657 = slt64(next_stride_37298, flat_dim_37179);
            
            if (memblock_set(ctx, &mem_param_tmp_40227, &mem_39788, "mem_39788") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_tmp_40228, &mem_39792, "mem_39792") != 0)
                return 1;
            
            int64_t loop_dz2084Uz2082U_tmp_40229 = flat_dim_37654;
            bool loop_while_tmp_40230 = loop_cond_37657;
            int64_t stride_tmp_40233 = next_stride_37298;
            
            if (memblock_set(ctx, &mem_param_39676, &mem_param_tmp_40227, "mem_param_tmp_40227") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_39679, &mem_param_tmp_40228, "mem_param_tmp_40228") != 0)
                return 1;
            loop_dz2084Uz2082U_37293 = loop_dz2084Uz2082U_tmp_40229;
            loop_while_37294 = loop_while_tmp_40230;
            stride_37297 = stride_tmp_40233;
        }
        if (memblock_set(ctx, &ext_mem_39799, &mem_param_39676, "mem_param_39676") != 0)
            return 1;
        if (memblock_set(ctx, &ext_mem_39798, &mem_param_39679, "mem_param_39679") != 0)
            return 1;
        data_37288 = loop_dz2084Uz2082U_37293;
        data_37289 = loop_while_37294;
        data_37292 = stride_37297;
        if (memblock_unref(ctx, &mem_39668, "mem_39668") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39672, "mem_39672") != 0)
            return 1;
        
        bool empty_slice_37658 = n1_30936 == (int64_t) 0;
        bool zzero_leq_i_p_m_t_s_37659 = sle64((int64_t) 0, zs_lhs_37163);
        bool i_p_m_t_s_leq_w_37660 = slt64(zs_lhs_37163, data_37288);
        bool y_37661 = zzero_leq_i_p_m_t_s_37659 && i_p_m_t_s_leq_w_37660;
        bool ok_or_empty_37662 = empty_slice_37658 || y_37661;
        bool index_certs_37663;
        
        if (!ok_or_empty_37662) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) n1_30936, "] out of bounds for array of shape [", (long long) data_37288, "].", "-> #0  /prelude/array.fut:46:45-51\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:265:3-14\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #3  /prelude/functional.fut:9:44-45\n   #4  ../../../joins/ftSMJ.fut:104:60-63\n   #5  test_matching.fut:51:43-47\n   #6  test_matching.fut:45:1-54:44\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        if (memblock_set(ctx, &ext_mem_39802, &ext_mem_39798, "ext_mem_39798") != 0)
            return 1;
    }
    if (memblock_unref(ctx, &mem_39597, "mem_39597") != 0)
        return 1;
    if (mem_39804_cached_sizze_40373 < bytes_39596) {
        err = lexical_realloc(ctx, &mem_39804, &mem_39804_cached_sizze_40373, bytes_39596);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    for (int64_t i_39507 = 0; i_39507 < n1_30936; i_39507++) {
        int64_t eta_p_37671 = ((int64_t *) ext_mem_39802.mem)[i_39507];
        bool x_37672 = sle64((int64_t) 0, eta_p_37671);
        bool y_37673 = slt64(eta_p_37671, n1_30936);
        bool bounds_check_37674 = x_37672 && y_37673;
        bool index_certs_37675;
        
        if (!bounds_check_37674) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_37671, "] out of bounds for array of shape [", (long long) n1_30936, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:277:24-28\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:277:8-29\n   #3  ../../../joins/ftSMJ.fut:104:60-63\n   #4  test_matching.fut:51:43-47\n   #5  test_matching.fut:45:1-54:44\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t lifted_lambda_res_37676 = ((int64_t *) ks1_mem_39592.mem)[eta_p_37671];
        
        ((int64_t *) mem_39804)[i_39507] = lifted_lambda_res_37676;
    }
    if (memblock_unref(ctx, &ext_mem_39802, "ext_mem_39802") != 0)
        return 1;
    if (memblock_alloc(ctx, &mem_39812, bytes_39596, "mem_39812")) {
        err = 1;
        goto cleanup;
    }
    lmad_copy_8b(ctx, 1, (uint64_t *) mem_39812.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_39804, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {n1_30936});
    if (memblock_alloc(ctx, &mem_39814, bytes_39813, "mem_39814")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t i_40257 = 0; i_40257 < n2_30937; i_40257++) {
        int64_t x_40258 = (int64_t) 0 + i_40257 * (int64_t) 1;
        
        ((int64_t *) mem_39814.mem)[i_40257] = x_40258;
    }
    
    int64_t flat_dim_37699 = smallest_pow_2_geq_than_res_37688 * block_sizze_37698;
    bool x_40135 = !cond_37682;
    int64_t greatest_divisor_leq_than_arg1_37702 = mul64((int64_t) 2, block_sizze_37698);
    int64_t merge_block_sizze_37703;
    
    if (x_40135) {
        int64_t x_40137;
        
        if (futrts_greatest_divisor_leq_than_5346(ctx, &x_40137, (int64_t) 8, greatest_divisor_leq_than_arg1_37702) != 0) {
            err = 1;
            goto cleanup;
        }
        merge_block_sizze_37703 = x_40137;
    } else {
        merge_block_sizze_37703 = (int64_t) 0;
    }
    
    int64_t reduce_arg1_37706;
    
    if (x_40135) {
        int64_t x_40141 = ((int64_t *) ks2_mem_39594.mem)[(int64_t) 0];
        
        reduce_arg1_37706 = x_40141;
    } else {
        reduce_arg1_37706 = (int64_t) 0;
    }
    
    int64_t defunc_0_reduce_res_39427;
    int64_t defunc_0_reduce_res_39428;
    
    if (x_40135) {
        int64_t x_40143;
        int64_t x_40144;
        int64_t redout_39509;
        int64_t redout_39510;
        
        redout_39509 = reduce_arg1_37706;
        redout_39510 = (int64_t) 0;
        for (int64_t i_39511 = 0; i_39511 < n2_30937; i_39511++) {
            int64_t x_37710 = ((int64_t *) ks2_mem_39594.mem)[i_39511];
            bool defunc_0_leq_res_37716 = sle64(redout_39509, x_37710);
            int64_t defunc_0_op_res_37717;
            
            if (defunc_0_leq_res_37716) {
                defunc_0_op_res_37717 = x_37710;
            } else {
                defunc_0_op_res_37717 = redout_39509;
            }
            
            int64_t defunc_0_op_res_37718;
            
            if (defunc_0_leq_res_37716) {
                defunc_0_op_res_37718 = i_39511;
            } else {
                defunc_0_op_res_37718 = redout_39510;
            }
            
            int64_t redout_tmp_40259 = defunc_0_op_res_37717;
            int64_t redout_tmp_40260 = defunc_0_op_res_37718;
            
            redout_39509 = redout_tmp_40259;
            redout_39510 = redout_tmp_40260;
        }
        x_40143 = redout_39509;
        x_40144 = redout_39510;
        defunc_0_reduce_res_39427 = x_40143;
        defunc_0_reduce_res_39428 = x_40144;
    } else {
        defunc_0_reduce_res_39427 = (int64_t) 0;
        defunc_0_reduce_res_39428 = (int64_t) 0;
    }
    
    int64_t distance_37730 = add64((int64_t) 1, tmp_37728);
    int64_t binop_x_39815 = (int64_t) 8 * smallest_pow_2_geq_than_res_37688;
    int64_t bytes_39816 = block_sizze_37698 * binop_x_39815;
    int64_t bytes_39829 = (int64_t) 8 * block_sizze_37698;
    bool loop_cond_37805 = slt64(block_sizze_37698, flat_dim_37699);
    bool zzero_37806 = merge_block_sizze_37703 == (int64_t) 0;
    bool nonzzero_37807 = !zzero_37806;
    
    if (cond_37682) {
        if (memblock_set(ctx, &ext_mem_40019, &mem_39814, "mem_39814") != 0)
            return 1;
    } else {
        if (mem_39817_cached_sizze_40374 < bytes_39816) {
            err = lexical_realloc(ctx, &mem_39817, &mem_39817_cached_sizze_40374, bytes_39816);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_39820_cached_sizze_40375 < bytes_39816) {
            err = lexical_realloc(ctx, &mem_39820, &mem_39820_cached_sizze_40375, bytes_39816);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_39830_cached_sizze_40376 < bytes_39829) {
            err = lexical_realloc(ctx, &mem_39830, &mem_39830_cached_sizze_40376, bytes_39829);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_39832_cached_sizze_40377 < bytes_39829) {
            err = lexical_realloc(ctx, &mem_39832, &mem_39832_cached_sizze_40377, bytes_39829);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t i_39523 = 0; i_39523 < smallest_pow_2_geq_than_res_37688; i_39523++) {
            int64_t zp_lhs_37737 = mul64(block_sizze_37698, i_39523);
            
            for (int64_t i_39516 = 0; i_39516 < block_sizze_37698; i_39516++) {
                int64_t k_37741 = add64(zp_lhs_37737, i_39516);
                bool cond_37742 = slt64(k_37741, n2_30937);
                int64_t lifted_lambda_res_37743;
                int64_t lifted_lambda_res_37744;
                
                if (cond_37742) {
                    bool x_39280 = sle64((int64_t) 0, k_37741);
                    bool bounds_check_39281 = cond_37742 && x_39280;
                    bool index_certs_39282;
                    
                    if (!bounds_check_39281) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) k_37741, "] out of bounds for array of shape [", (long long) n2_30937, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:246:26-30\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ../../../joins/ftSMJ.fut:104:60-63\n   #8  test_matching.fut:52:43-47\n   #9  test_matching.fut:45:1-54:44\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t lifted_lambda_res_t_res_39283 = ((int64_t *) ks2_mem_39594.mem)[k_37741];
                    
                    lifted_lambda_res_37743 = lifted_lambda_res_t_res_39283;
                    lifted_lambda_res_37744 = k_37741;
                } else {
                    lifted_lambda_res_37743 = defunc_0_reduce_res_39427;
                    lifted_lambda_res_37744 = defunc_0_reduce_res_39428;
                }
                ((int64_t *) mem_39830)[i_39516] = lifted_lambda_res_37743;
                ((int64_t *) mem_39832)[i_39516] = lifted_lambda_res_37744;
            }
            if (!cond_37727) {
                for (int64_t i_37754 = 0; i_37754 < distance_37730; i_37754++) {
                    int64_t gt_arg1_37757 = add64((int64_t) 1, i_37754);
                    bool x_37758 = sle64((int64_t) 0, gt_arg1_37757);
                    bool y_37759 = slt64(gt_arg1_37757, block_sizze_37698);
                    bool bounds_check_37760 = x_37758 && y_37759;
                    bool index_certs_37761;
                    
                    if (!bounds_check_37760) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) gt_arg1_37757, "] out of bounds for array of shape [", (long long) block_sizze_37698, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:169:31-37\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:52:43-47\n   #8  test_matching.fut:45:1-54:44\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    bool y_37763 = slt64(i_37754, block_sizze_37698);
                    bool index_certs_37764;
                    
                    if (!y_37763) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_37754, "] out of bounds for array of shape [", (long long) block_sizze_37698, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:169:26-30\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:52:43-47\n   #8  test_matching.fut:45:1-54:44\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t gt_arg1_37762 = ((int64_t *) mem_39830)[gt_arg1_37757];
                    int64_t gt_arg0_37765 = ((int64_t *) mem_39830)[i_37754];
                    bool defunc_0_leq_res_37766 = sle64(gt_arg0_37765, gt_arg1_37762);
                    bool defunc_0_lifted_gt_res_37767 = !defunc_0_leq_res_37766;
                    bool s_37768;
                    int64_t s_37771;
                    bool loop_while_37772;
                    int64_t j_37775;
                    
                    loop_while_37772 = defunc_0_lifted_gt_res_37767;
                    j_37775 = i_37754;
                    while (loop_while_37772) {
                        bool x_37776 = sle64((int64_t) 0, j_37775);
                        bool y_37777 = slt64(j_37775, block_sizze_37698);
                        bool bounds_check_37778 = x_37776 && y_37777;
                        bool index_certs_37779;
                        
                        if (!bounds_check_37778) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_37775, "] out of bounds for array of shape [", (long long) block_sizze_37698, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:170:24-28\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:52:43-47\n   #8  test_matching.fut:45:1-54:44\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t copy_arg0_37782 = add64((int64_t) 1, j_37775);
                        bool x_37783 = sle64((int64_t) 0, copy_arg0_37782);
                        bool y_37784 = slt64(copy_arg0_37782, block_sizze_37698);
                        bool bounds_check_37785 = x_37783 && y_37784;
                        bool index_certs_37786;
                        
                        if (!bounds_check_37785) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) copy_arg0_37782, "] out of bounds for array of shape [", (long long) block_sizze_37698, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:171:35-41\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:52:43-47\n   #8  test_matching.fut:45:1-54:44\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t copy_arg0_37780 = ((int64_t *) mem_39830)[j_37775];
                        int64_t copy_arg0_37781 = ((int64_t *) mem_39832)[j_37775];
                        int64_t copy_arg0_37787 = ((int64_t *) mem_39830)[copy_arg0_37782];
                        int64_t copy_arg0_37788 = ((int64_t *) mem_39832)[copy_arg0_37782];
                        
                        ((int64_t *) mem_39830)[j_37775] = copy_arg0_37787;
                        ((int64_t *) mem_39832)[j_37775] = copy_arg0_37788;
                        ((int64_t *) mem_39830)[copy_arg0_37782] = copy_arg0_37780;
                        ((int64_t *) mem_39832)[copy_arg0_37782] = copy_arg0_37781;
                        
                        int64_t tmp_37793 = sub64(j_37775, (int64_t) 1);
                        bool cond_37794 = sle64((int64_t) 0, tmp_37793);
                        bool loop_cond_37795;
                        
                        if (cond_37794) {
                            bool y_39285 = slt64(tmp_37793, block_sizze_37698);
                            bool bounds_check_39286 = cond_37794 && y_39285;
                            bool index_certs_39287;
                            
                            if (!bounds_check_39286) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_37793, "] out of bounds for array of shape [", (long long) block_sizze_37698, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:169:26-30\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:52:43-47\n   #8  test_matching.fut:45:1-54:44\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t gt_arg1_39284 = ((int64_t *) mem_39830)[j_37775];
                            int64_t gt_arg0_39288 = ((int64_t *) mem_39830)[tmp_37793];
                            bool defunc_0_leq_res_39289 = sle64(gt_arg0_39288, gt_arg1_39284);
                            bool defunc_0_lifted_gt_res_39290 = !defunc_0_leq_res_39289;
                            
                            loop_cond_37795 = defunc_0_lifted_gt_res_39290;
                        } else {
                            loop_cond_37795 = 0;
                        }
                        
                        bool loop_while_tmp_40267 = loop_cond_37795;
                        int64_t j_tmp_40270 = tmp_37793;
                        
                        loop_while_37772 = loop_while_tmp_40267;
                        j_37775 = j_tmp_40270;
                    }
                    s_37768 = loop_while_37772;
                    s_37771 = j_37775;
                }
            }
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_39817, i_39523 * block_sizze_37698, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_39830, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {block_sizze_37698});
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_39820, i_39523 * block_sizze_37698, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_39832, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {block_sizze_37698});
        }
        if (memblock_alloc(ctx, &mem_39885, bytes_39816, "mem_39885")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 2, (uint64_t *) mem_39885.mem, (int64_t) 0, (int64_t []) {block_sizze_37698, (int64_t) 1}, (uint64_t *) mem_39817, (int64_t) 0, (int64_t []) {block_sizze_37698, (int64_t) 1}, (int64_t []) {smallest_pow_2_geq_than_res_37688, block_sizze_37698});
        if (memblock_alloc(ctx, &mem_39889, bytes_39816, "mem_39889")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 2, (uint64_t *) mem_39889.mem, (int64_t) 0, (int64_t []) {block_sizze_37698, (int64_t) 1}, (uint64_t *) mem_39820, (int64_t) 0, (int64_t []) {block_sizze_37698, (int64_t) 1}, (int64_t []) {smallest_pow_2_geq_than_res_37688, block_sizze_37698});
        
        int64_t data_37808;
        bool data_37809;
        int64_t data_37812;
        int64_t loop_dz2084Uz2082U_37813;
        bool loop_while_37814;
        int64_t stride_37817;
        
        if (memblock_set(ctx, &mem_param_39893, &mem_39885, "mem_39885") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_39896, &mem_39889, "mem_39889") != 0)
            return 1;
        loop_dz2084Uz2082U_37813 = flat_dim_37699;
        loop_while_37814 = loop_cond_37805;
        stride_37817 = block_sizze_37698;
        while (loop_while_37814) {
            int64_t next_stride_37818 = mul64((int64_t) 2, stride_37817);
            bool zzero_37819 = next_stride_37818 == (int64_t) 0;
            bool nonzzero_37820 = !zzero_37819;
            bool nonzzero_cert_37821;
            
            if (!nonzzero_37820) {
                set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:254:33-46\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #2  /prelude/functional.fut:9:44-45\n   #3  ../../../joins/ftSMJ.fut:104:60-63\n   #4  test_matching.fut:52:43-47\n   #5  test_matching.fut:45:1-54:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t num_merges_37822 = sdiv64(flat_dim_37699, next_stride_37818);
            bool bounds_invalid_upwards_37823 = slt64(num_merges_37822, (int64_t) 0);
            bool valid_37824 = !bounds_invalid_upwards_37823;
            bool range_valid_c_37825;
            
            if (!valid_37824) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) num_merges_37822, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #3  /prelude/functional.fut:9:44-45\n   #4  ../../../joins/ftSMJ.fut:104:60-63\n   #5  test_matching.fut:52:43-47\n   #6  test_matching.fut:45:1-54:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t m_37827 = sub64(next_stride_37818, (int64_t) 1);
            bool zzero_leq_i_p_m_t_s_37828 = sle64((int64_t) 0, m_37827);
            bool cond_37830 = slt64(next_stride_37818, (int64_t) 10);
            bool protect_assert_disj_37831 = nonzzero_37807 || cond_37830;
            bool nonzzero_cert_37832;
            
            if (!protect_assert_disj_37831) {
                set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:147:30-42\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:52:43-47\n   #8  test_matching.fut:45:1-54:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t zeze_lhs_37833 = smod_safe64(next_stride_37818, merge_block_sizze_37703);
            bool num_blocks_37834 = zeze_lhs_37833 == (int64_t) 0;
            bool protect_assert_disj_37835 = cond_37830 || num_blocks_37834;
            bool assert_c_37836;
            
            if (!protect_assert_disj_37835) {
                set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "Assertion is false: (n % block_size == 0)", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:147:20-65\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:52:43-47\n   #8  test_matching.fut:45:1-54:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t num_blocks_37838 = sdiv_safe64(next_stride_37818, merge_block_sizze_37703);
            int64_t dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37839 = add64((int64_t) 1, num_blocks_37838);
            int64_t j_m_i_37840 = sub64(next_stride_37818, stride_37817);
            int64_t flat_dim_37841 = merge_block_sizze_37703 * num_blocks_37838;
            bool bounds_invalid_upwards_37842 = slt64(dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37839, (int64_t) 0);
            bool empty_slice_37843 = j_m_i_37840 == (int64_t) 0;
            int64_t m_37844 = sub64(j_m_i_37840, (int64_t) 1);
            bool zzero_lte_i_37845 = sle64((int64_t) 0, stride_37817);
            bool i_lte_j_37846 = sle64(stride_37817, next_stride_37818);
            bool empty_slice_37847 = stride_37817 == (int64_t) 0;
            int64_t m_37848 = sub64(stride_37817, (int64_t) 1);
            bool bounds_invalid_upwards_37849 = slt64(num_blocks_37838, (int64_t) 0);
            bool cond_37850 = slt64((int64_t) 0, stride_37817);
            bool valid_37851 = !bounds_invalid_upwards_37842;
            int64_t i_p_m_t_s_37852 = add64(stride_37817, m_37844);
            bool zzero_leq_i_p_m_t_s_37853 = sle64((int64_t) 0, m_37848);
            bool i_p_m_t_s_leq_w_37854 = slt64(m_37848, next_stride_37818);
            bool valid_37855 = !bounds_invalid_upwards_37849;
            bool i_p_m_t_s_leq_w_37856 = slt64(m_37827, flat_dim_37841);
            bool y_37857 = slt64((int64_t) 0, j_m_i_37840);
            bool protect_assert_disj_37858 = cond_37830 || valid_37851;
            bool range_valid_c_37859;
            
            if (!protect_assert_disj_37858) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37839, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  ../../../joins/ftSMJ.fut:104:60-63\n   #9  test_matching.fut:52:43-47\n   #10 test_matching.fut:45:1-54:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool zzero_leq_i_p_m_t_s_37860 = sle64((int64_t) 0, i_p_m_t_s_37852);
            bool i_p_m_t_s_leq_w_37861 = slt64(i_p_m_t_s_37852, next_stride_37818);
            bool y_37862 = zzero_leq_i_p_m_t_s_37853 && i_p_m_t_s_leq_w_37854;
            bool protect_assert_disj_37863 = cond_37830 || valid_37855;
            bool range_valid_c_37864;
            
            if (!protect_assert_disj_37863) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) num_blocks_37838, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ../../../joins/ftSMJ.fut:104:60-63\n   #8  test_matching.fut:52:43-47\n   #9  test_matching.fut:45:1-54:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool y_37865 = zzero_leq_i_p_m_t_s_37828 && i_p_m_t_s_leq_w_37856;
            bool y_37866 = zzero_lte_i_37845 && i_p_m_t_s_leq_w_37861;
            bool forwards_ok_37867 = zzero_lte_i_37845 && y_37862;
            bool ok_or_empty_37868 = zzero_37819 || y_37865;
            bool protect_assert_disj_37869 = cond_37850 || y_37857;
            bool y_37870 = zzero_leq_i_p_m_t_s_37860 && y_37866;
            bool ok_or_empty_37871 = empty_slice_37847 || forwards_ok_37867;
            bool protect_assert_disj_37872 = cond_37830 || ok_or_empty_37868;
            bool index_certs_37873;
            
            if (!protect_assert_disj_37872) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) next_stride_37818, "] out of bounds for array of shape [", (long long) flat_dim_37841, "].", "-> #0  /prelude/array.fut:46:45-51\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:155:3-46\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ../../../joins/ftSMJ.fut:104:60-63\n   #8  test_matching.fut:52:43-47\n   #9  test_matching.fut:45:1-54:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool loop_not_taken_37874 = !cond_37830;
            bool protect_assert_disj_37875 = protect_assert_disj_37869 || loop_not_taken_37874;
            bool index_certs_37876;
            
            if (!protect_assert_disj_37875) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) j_m_i_37840, "].", "-> #0  /prelude/array.fut:22:29-33\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:100:48-54\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  ../../../joins/ftSMJ.fut:104:60-63\n   #9  test_matching.fut:52:43-47\n   #10 test_matching.fut:45:1-54:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool forwards_ok_37877 = i_lte_j_37846 && y_37870;
            bool protect_assert_disj_37878 = cond_37830 || ok_or_empty_37871;
            bool index_certs_37879;
            
            if (!protect_assert_disj_37878) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) stride_37817, "] out of bounds for array of shape [", (long long) next_stride_37818, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:52-59\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  ../../../joins/ftSMJ.fut:104:60-63\n   #10 test_matching.fut:52:43-47\n   #11 test_matching.fut:45:1-54:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool protect_assert_disj_37880 = ok_or_empty_37871 || loop_not_taken_37874;
            bool index_certs_37881;
            
            if (!protect_assert_disj_37880) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) stride_37817, "] out of bounds for array of shape [", (long long) next_stride_37818, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:26-33\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:52:43-47\n   #8  test_matching.fut:45:1-54:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool ok_or_empty_37882 = empty_slice_37843 || forwards_ok_37877;
            bool protect_assert_disj_37883 = cond_37830 || ok_or_empty_37882;
            bool index_certs_37884;
            
            if (!protect_assert_disj_37883) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) stride_37817, ":] out of bounds for array of shape [", (long long) next_stride_37818, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:60-67\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  ../../../joins/ftSMJ.fut:104:60-63\n   #10 test_matching.fut:52:43-47\n   #11 test_matching.fut:45:1-54:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool protect_assert_disj_37885 = loop_not_taken_37874 || ok_or_empty_37882;
            bool index_certs_37886;
            
            if (!protect_assert_disj_37885) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) stride_37817, ":] out of bounds for array of shape [", (long long) next_stride_37818, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:34-41\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:52:43-47\n   #8  test_matching.fut:45:1-54:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t binop_x_39897 = (int64_t) 8 * num_merges_37822;
            int64_t bytes_39898 = next_stride_37818 * binop_x_39897;
            bool i_lte_j_37829 = sle64((int64_t) 0, next_stride_37818);
            
            if (mem_39899_cached_sizze_40378 < bytes_39898) {
                err = lexical_realloc(ctx, &mem_39899, &mem_39899_cached_sizze_40378, bytes_39898);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            if (mem_39902_cached_sizze_40379 < bytes_39898) {
                err = lexical_realloc(ctx, &mem_39902, &mem_39902_cached_sizze_40379, bytes_39898);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            
            int64_t bytes_39911 = (int64_t) 8 * dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37839;
            int64_t binop_x_39927 = (int64_t) 8 * num_blocks_37838;
            int64_t bytes_39928 = merge_block_sizze_37703 * binop_x_39927;
            int64_t bytes_39973 = (int64_t) 16 * stride_37817;
            
            for (int64_t i_39544 = 0; i_39544 < num_merges_37822; i_39544++) {
                int64_t start_37890 = mul64(next_stride_37818, i_39544);
                int64_t j_m_i_37891 = sub64(loop_dz2084Uz2082U_37813, start_37890);
                bool empty_slice_37892 = j_m_i_37891 == (int64_t) 0;
                int64_t m_37893 = sub64(j_m_i_37891, (int64_t) 1);
                int64_t i_p_m_t_s_37894 = add64(start_37890, m_37893);
                bool zzero_leq_i_p_m_t_s_37895 = sle64((int64_t) 0, i_p_m_t_s_37894);
                bool i_p_m_t_s_leq_w_37896 = slt64(i_p_m_t_s_37894, loop_dz2084Uz2082U_37813);
                bool zzero_lte_i_37897 = sle64((int64_t) 0, start_37890);
                bool i_lte_j_37898 = sle64(start_37890, loop_dz2084Uz2082U_37813);
                bool y_37899 = i_p_m_t_s_leq_w_37896 && zzero_lte_i_37897;
                bool y_37900 = zzero_leq_i_p_m_t_s_37895 && y_37899;
                bool forwards_ok_37901 = i_lte_j_37898 && y_37900;
                bool ok_or_empty_37902 = empty_slice_37892 || forwards_ok_37901;
                bool index_certs_37903;
                
                if (!ok_or_empty_37902) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) start_37890, ":] out of bounds for array of shape [", (long long) loop_dz2084Uz2082U_37813, "].", "-> #0  /prelude/array.fut:52:49-54\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:258:41-56\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:52:43-47\n   #8  test_matching.fut:45:1-54:44\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                bool i_p_m_t_s_leq_w_37904 = slt64(m_37827, j_m_i_37891);
                bool y_37905 = zzero_leq_i_p_m_t_s_37828 && i_p_m_t_s_leq_w_37904;
                bool forwards_ok_37906 = i_lte_j_37829 && y_37905;
                bool ok_or_empty_37907 = zzero_37819 || forwards_ok_37906;
                bool index_certs_37908;
                
                if (!ok_or_empty_37907) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) next_stride_37818, "] out of bounds for array of shape [", (long long) j_m_i_37891, "].", "-> #0  /prelude/array.fut:46:45-51\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:258:23-56\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ../../../joins/ftSMJ.fut:104:60-63\n   #7  test_matching.fut:52:43-47\n   #8  test_matching.fut:45:1-54:44\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                if (cond_37830) {
                    int64_t dummy_39316;
                    int64_t dummy_39317;
                    
                    if (cond_37850) {
                        int64_t head_res_39318 = ((int64_t *) mem_param_39893.mem)[start_37890];
                        int64_t head_res_39319 = ((int64_t *) mem_param_39896.mem)[start_37890];
                        
                        dummy_39316 = head_res_39318;
                        dummy_39317 = head_res_39319;
                    } else {
                        int64_t slice_39320 = stride_37817 + start_37890;
                        int64_t head_res_39321 = ((int64_t *) mem_param_39893.mem)[slice_39320];
                        int64_t head_res_39322 = ((int64_t *) mem_param_39896.mem)[slice_39320];
                        
                        dummy_39316 = head_res_39321;
                        dummy_39317 = head_res_39322;
                    }
                    if (memblock_alloc(ctx, &mem_39974, bytes_39973, "mem_39974")) {
                        err = 1;
                        goto cleanup;
                    }
                    for (int64_t nest_i_40280 = 0; nest_i_40280 < next_stride_37818; nest_i_40280++) {
                        ((int64_t *) mem_39974.mem)[nest_i_40280] = dummy_39316;
                    }
                    if (memblock_alloc(ctx, &mem_39976, bytes_39973, "mem_39976")) {
                        err = 1;
                        goto cleanup;
                    }
                    for (int64_t nest_i_40281 = 0; nest_i_40281 < next_stride_37818; nest_i_40281++) {
                        ((int64_t *) mem_39976.mem)[nest_i_40281] = dummy_39317;
                    }
                    
                    int64_t data_39325;
                    int64_t i_39329 = (int64_t) 0;
                    
                    for (int64_t k_39328 = 0; k_39328 < next_stride_37818; k_39328++) {
                        int64_t j_39332 = sub64(k_39328, i_39329);
                        bool cond_39333 = j_39332 == j_m_i_37840;
                        bool cond_39334;
                        
                        if (cond_39333) {
                            cond_39334 = 1;
                        } else {
                            bool cond_39335 = slt64(i_39329, stride_37817);
                            bool cond_f_res_39336;
                            
                            if (cond_39335) {
                                bool x_39337 = sle64((int64_t) 0, j_39332);
                                bool y_39338 = slt64(j_39332, j_m_i_37840);
                                bool bounds_check_39339 = x_39337 && y_39338;
                                bool index_certs_39340;
                                
                                if (!bounds_check_39339) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_39332, "] out of bounds for array of shape [", (long long) j_m_i_37840, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:55-59\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ../../../joins/ftSMJ.fut:104:60-63\n   #8  test_matching.fut:52:43-47\n   #9  test_matching.fut:45:1-54:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                bool x_39344 = sle64((int64_t) 0, i_39329);
                                bool bounds_check_39345 = cond_39335 && x_39344;
                                bool index_certs_39346;
                                
                                if (!bounds_check_39345) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_39329, "] out of bounds for array of shape [", (long long) stride_37817, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:50-54\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ../../../joins/ftSMJ.fut:104:60-63\n   #8  test_matching.fut:52:43-47\n   #9  test_matching.fut:45:1-54:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t slice_39341 = stride_37817 + j_39332;
                                int64_t slice_39342 = start_37890 + slice_39341;
                                int64_t leq_arg1_39343 = ((int64_t *) mem_param_39893.mem)[slice_39342];
                                int64_t slice_39347 = start_37890 + i_39329;
                                int64_t leq_arg0_39348 = ((int64_t *) mem_param_39893.mem)[slice_39347];
                                bool defunc_0_leq_res_39349 = sle64(leq_arg0_39348, leq_arg1_39343);
                                
                                cond_f_res_39336 = defunc_0_leq_res_39349;
                            } else {
                                cond_f_res_39336 = 0;
                            }
                            cond_39334 = cond_f_res_39336;
                        }
                        
                        int64_t loopres_39350;
                        int64_t loopres_39351;
                        int64_t loopres_39352;
                        
                        if (cond_39334) {
                            bool x_39354 = sle64((int64_t) 0, i_39329);
                            bool y_39355 = slt64(i_39329, stride_37817);
                            bool bounds_check_39356 = x_39354 && y_39355;
                            bool index_certs_39357;
                            
                            if (!bounds_check_39356) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_39329, "] out of bounds for array of shape [", (long long) stride_37817, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:106:17-21\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ../../../joins/ftSMJ.fut:104:60-63\n   #8  test_matching.fut:52:43-47\n   #9  test_matching.fut:45:1-54:44\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t tmp_39353 = add64((int64_t) 1, i_39329);
                            int64_t slice_39358 = start_37890 + i_39329;
                            int64_t tmp_39359 = ((int64_t *) mem_param_39893.mem)[slice_39358];
                            int64_t tmp_39360 = ((int64_t *) mem_param_39896.mem)[slice_39358];
                            
                            loopres_39350 = tmp_39353;
                            loopres_39351 = tmp_39359;
                            loopres_39352 = tmp_39360;
                        } else {
                            bool x_39361 = sle64((int64_t) 0, j_39332);
                            bool y_39362 = slt64(j_39332, j_m_i_37840);
                            bool bounds_check_39363 = x_39361 && y_39362;
                            bool index_certs_39364;
                            
                            if (!bounds_check_39363) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_39332, "] out of bounds for array of shape [", (long long) j_m_i_37840, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:108:15-19\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ../../../joins/ftSMJ.fut:104:60-63\n   #8  test_matching.fut:52:43-47\n   #9  test_matching.fut:45:1-54:44\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t slice_39365 = stride_37817 + j_39332;
                            int64_t slice_39366 = start_37890 + slice_39365;
                            int64_t tmp_39367 = ((int64_t *) mem_param_39893.mem)[slice_39366];
                            int64_t tmp_39368 = ((int64_t *) mem_param_39896.mem)[slice_39366];
                            
                            loopres_39350 = i_39329;
                            loopres_39351 = tmp_39367;
                            loopres_39352 = tmp_39368;
                        }
                        ((int64_t *) mem_39974.mem)[k_39328] = loopres_39351;
                        ((int64_t *) mem_39976.mem)[k_39328] = loopres_39352;
                        
                        int64_t i_tmp_40282 = loopres_39350;
                        
                        i_39329 = i_tmp_40282;
                    }
                    data_39325 = i_39329;
                    if (memblock_set(ctx, &ext_mem_39994, &mem_39974, "mem_39974") != 0)
                        return 1;
                    if (memblock_set(ctx, &ext_mem_39991, &mem_39976, "mem_39976") != 0)
                        return 1;
                } else {
                    if (mem_39912_cached_sizze_40380 < bytes_39911) {
                        err = lexical_realloc(ctx, &mem_39912, &mem_39912_cached_sizze_40380, bytes_39911);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    if (mem_39914_cached_sizze_40381 < bytes_39911) {
                        err = lexical_realloc(ctx, &mem_39914, &mem_39914_cached_sizze_40381, bytes_39911);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    for (int64_t i_39530 = 0; i_39530 < dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37839; i_39530++) {
                        int64_t split_count_arg3_37970 = mul64(merge_block_sizze_37703, i_39530);
                        int64_t defunc_0_split_count_res_37971;
                        int64_t defunc_0_split_count_res_37972;
                        int64_t defunc_0_split_count_res_37973;
                        int64_t defunc_0_split_count_res_37974;
                        int64_t defunc_0_split_count_res_37975;
                        
                        if (futrts_lifted_normalizze_7743(ctx, &defunc_0_split_count_res_37971, &defunc_0_split_count_res_37972, &defunc_0_split_count_res_37973, &defunc_0_split_count_res_37974, &defunc_0_split_count_res_37975, (int64_t) 0, stride_37817, (int64_t) 0, j_m_i_37840, split_count_arg3_37970) != 0) {
                            err = 1;
                            goto cleanup;
                        }
                        
                        bool loop_cond_37976 = slt64((int64_t) 0, defunc_0_split_count_res_37975);
                        bool defunc_0_split_count_res_37977;
                        int64_t defunc_0_split_count_res_37978;
                        int64_t defunc_0_split_count_res_37979;
                        int64_t defunc_0_split_count_res_37980;
                        int64_t defunc_0_split_count_res_37981;
                        int64_t defunc_0_split_count_res_37982;
                        bool loop_while_37983;
                        int64_t ss_37984;
                        int64_t ss_37985;
                        int64_t tt_37986;
                        int64_t tt_37987;
                        int64_t count_37988;
                        
                        loop_while_37983 = loop_cond_37976;
                        ss_37984 = defunc_0_split_count_res_37971;
                        ss_37985 = defunc_0_split_count_res_37972;
                        tt_37986 = defunc_0_split_count_res_37973;
                        tt_37987 = defunc_0_split_count_res_37974;
                        count_37988 = defunc_0_split_count_res_37975;
                        while (loop_while_37983) {
                            int64_t zlze_lhs_37989 = sub64(ss_37985, ss_37984);
                            bool cond_37990 = sle64(zlze_lhs_37989, (int64_t) 0);
                            int64_t defunc_0_lifted_step_res_37991;
                            int64_t defunc_0_lifted_step_res_37992;
                            int64_t defunc_0_lifted_step_res_37993;
                            int64_t defunc_0_lifted_step_res_37994;
                            
                            if (cond_37990) {
                                int64_t tmp_39371 = add64(tt_37986, count_37988);
                                
                                defunc_0_lifted_step_res_37991 = ss_37984;
                                defunc_0_lifted_step_res_37992 = tmp_39371;
                                defunc_0_lifted_step_res_37993 = tt_37987;
                                defunc_0_lifted_step_res_37994 = (int64_t) 0;
                            } else {
                                int64_t zlze_lhs_37996 = sub64(tt_37987, tt_37986);
                                bool cond_37997 = sle64(zlze_lhs_37996, (int64_t) 0);
                                int64_t defunc_0_lifted_step_res_f_res_37998;
                                int64_t defunc_0_lifted_step_res_f_res_37999;
                                int64_t defunc_0_lifted_step_res_f_res_38000;
                                int64_t defunc_0_lifted_step_res_f_res_38001;
                                
                                if (cond_37997) {
                                    int64_t tmp_39372 = add64(ss_37984, count_37988);
                                    
                                    defunc_0_lifted_step_res_f_res_37998 = tmp_39372;
                                    defunc_0_lifted_step_res_f_res_37999 = tt_37986;
                                    defunc_0_lifted_step_res_f_res_38000 = tt_37987;
                                    defunc_0_lifted_step_res_f_res_38001 = (int64_t) 0;
                                } else {
                                    bool cond_38003 = count_37988 == (int64_t) 1;
                                    int64_t defunc_0_lifted_step_res_f_res_f_res_38004;
                                    int64_t defunc_0_lifted_step_res_f_res_f_res_38005;
                                    int64_t defunc_0_lifted_step_res_f_res_f_res_38006;
                                    int64_t defunc_0_lifted_step_res_f_res_f_res_38007;
                                    
                                    if (cond_38003) {
                                        bool x_39374 = sle64((int64_t) 0, ss_37984);
                                        bool y_39375 = slt64(ss_37984, stride_37817);
                                        bool bounds_check_39376 = x_39374 && y_39375;
                                        bool index_certs_39377;
                                        
                                        if (!bounds_check_39376) {
                                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) ss_37984, "] out of bounds for array of shape [", (long long) stride_37817, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:53:21-27\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 ../../../joins/ftSMJ.fut:104:60-63\n   #12 test_matching.fut:52:43-47\n   #13 test_matching.fut:45:1-54:44\n"));
                                            err = FUTHARK_PROGRAM_ERROR;
                                            goto cleanup;
                                        }
                                        
                                        bool x_39380 = sle64((int64_t) 0, tt_37986);
                                        bool y_39381 = slt64(tt_37986, j_m_i_37840);
                                        bool bounds_check_39382 = x_39380 && y_39381;
                                        bool index_certs_39383;
                                        
                                        if (!bounds_check_39382) {
                                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tt_37986, "] out of bounds for array of shape [", (long long) j_m_i_37840, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:53:14-20\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 ../../../joins/ftSMJ.fut:104:60-63\n   #12 test_matching.fut:52:43-47\n   #13 test_matching.fut:45:1-54:44\n"));
                                            err = FUTHARK_PROGRAM_ERROR;
                                            goto cleanup;
                                        }
                                        
                                        int64_t slice_39378 = start_37890 + ss_37984;
                                        int64_t leq_arg1_39379 = ((int64_t *) mem_param_39893.mem)[slice_39378];
                                        int64_t slice_39384 = stride_37817 + tt_37986;
                                        int64_t slice_39385 = start_37890 + slice_39384;
                                        int64_t leq_arg0_39386 = ((int64_t *) mem_param_39893.mem)[slice_39385];
                                        bool defunc_0_leq_res_39387 = sle64(leq_arg0_39386, leq_arg1_39379);
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_t_res_39388;
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_t_res_39389;
                                        
                                        if (defunc_0_leq_res_39387) {
                                            int64_t tmp_39390 = add64((int64_t) 1, tt_37986);
                                            
                                            defunc_0_lifted_step_res_f_res_f_res_t_res_39388 = ss_37984;
                                            defunc_0_lifted_step_res_f_res_f_res_t_res_39389 = tmp_39390;
                                        } else {
                                            int64_t tmp_39391 = add64((int64_t) 1, ss_37984);
                                            
                                            defunc_0_lifted_step_res_f_res_f_res_t_res_39388 = tmp_39391;
                                            defunc_0_lifted_step_res_f_res_f_res_t_res_39389 = tt_37986;
                                        }
                                        defunc_0_lifted_step_res_f_res_f_res_38004 = defunc_0_lifted_step_res_f_res_f_res_t_res_39388;
                                        defunc_0_lifted_step_res_f_res_f_res_38005 = defunc_0_lifted_step_res_f_res_f_res_t_res_39389;
                                        defunc_0_lifted_step_res_f_res_f_res_38006 = tt_37987;
                                        defunc_0_lifted_step_res_f_res_f_res_38007 = (int64_t) 0;
                                    } else {
                                        int64_t m_38026 = sdiv64(count_37988, (int64_t) 2);
                                        int64_t n_38027 = sub64(count_37988, m_38026);
                                        bool cond_38028 = n_38027 == (int64_t) 0;
                                        int64_t zgze_lhs_38029 = add64(ss_37984, m_38026);
                                        bool cond_f_res_38030 = sle64(ss_37985, zgze_lhs_38029);
                                        bool x_38031 = !cond_38028;
                                        bool y_38032 = cond_f_res_38030 && x_38031;
                                        bool cond_38033 = cond_38028 || y_38032;
                                        bool leq_y_x_38034;
                                        
                                        if (cond_38033) {
                                            leq_y_x_38034 = 1;
                                        } else {
                                            bool x_38035 = sle64((int64_t) 0, zgze_lhs_38029);
                                            bool y_38036 = slt64(zgze_lhs_38029, stride_37817);
                                            bool bounds_check_38037 = x_38035 && y_38036;
                                            bool index_certs_38038;
                                            
                                            if (!bounds_check_38037) {
                                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) zgze_lhs_38029, "] out of bounds for array of shape [", (long long) stride_37817, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:73:22-30\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 ../../../joins/ftSMJ.fut:104:60-63\n   #12 test_matching.fut:52:43-47\n   #13 test_matching.fut:45:1-54:44\n"));
                                                err = FUTHARK_PROGRAM_ERROR;
                                                goto cleanup;
                                            }
                                            
                                            int64_t zm_lhs_38041 = add64(tt_37986, n_38027);
                                            int64_t leq_arg0_38042 = sub64(zm_lhs_38041, (int64_t) 1);
                                            bool x_38043 = sle64((int64_t) 0, leq_arg0_38042);
                                            bool y_38044 = slt64(leq_arg0_38042, j_m_i_37840);
                                            bool bounds_check_38045 = x_38043 && y_38044;
                                            bool index_certs_38046;
                                            
                                            if (!bounds_check_38045) {
                                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) leq_arg0_38042, "] out of bounds for array of shape [", (long long) j_m_i_37840, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:73:11-21\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 ../../../joins/ftSMJ.fut:104:60-63\n   #12 test_matching.fut:52:43-47\n   #13 test_matching.fut:45:1-54:44\n"));
                                                err = FUTHARK_PROGRAM_ERROR;
                                                goto cleanup;
                                            }
                                            
                                            int64_t slice_38039 = start_37890 + zgze_lhs_38029;
                                            int64_t leq_arg1_38040 = ((int64_t *) mem_param_39893.mem)[slice_38039];
                                            int64_t slice_38047 = stride_37817 + leq_arg0_38042;
                                            int64_t slice_38048 = start_37890 + slice_38047;
                                            int64_t leq_arg0_38049 = ((int64_t *) mem_param_39893.mem)[slice_38048];
                                            bool defunc_0_leq_res_38050 = sle64(leq_arg0_38049, leq_arg1_38040);
                                            
                                            leq_y_x_38034 = defunc_0_leq_res_38050;
                                        }
                                        
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_f_res_38051;
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_f_res_38052;
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_f_res_38053;
                                        
                                        if (leq_y_x_38034) {
                                            int64_t tmp_39392 = add64(tt_37986, n_38027);
                                            int64_t tmp_39393 = sub64(count_37988, n_38027);
                                            
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_38051 = tmp_39392;
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_38052 = tt_37987;
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_38053 = tmp_39393;
                                        } else {
                                            int64_t tmp_38056 = add64(tt_37986, n_38027);
                                            
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_38051 = tt_37986;
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_38052 = tmp_38056;
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_38053 = count_37988;
                                        }
                                        defunc_0_lifted_step_res_f_res_f_res_38004 = ss_37984;
                                        defunc_0_lifted_step_res_f_res_f_res_38005 = defunc_0_lifted_step_res_f_res_f_res_f_res_38051;
                                        defunc_0_lifted_step_res_f_res_f_res_38006 = defunc_0_lifted_step_res_f_res_f_res_f_res_38052;
                                        defunc_0_lifted_step_res_f_res_f_res_38007 = defunc_0_lifted_step_res_f_res_f_res_f_res_38053;
                                    }
                                    defunc_0_lifted_step_res_f_res_37998 = defunc_0_lifted_step_res_f_res_f_res_38004;
                                    defunc_0_lifted_step_res_f_res_37999 = defunc_0_lifted_step_res_f_res_f_res_38005;
                                    defunc_0_lifted_step_res_f_res_38000 = defunc_0_lifted_step_res_f_res_f_res_38006;
                                    defunc_0_lifted_step_res_f_res_38001 = defunc_0_lifted_step_res_f_res_f_res_38007;
                                }
                                defunc_0_lifted_step_res_37991 = defunc_0_lifted_step_res_f_res_37998;
                                defunc_0_lifted_step_res_37992 = defunc_0_lifted_step_res_f_res_37999;
                                defunc_0_lifted_step_res_37993 = defunc_0_lifted_step_res_f_res_38000;
                                defunc_0_lifted_step_res_37994 = defunc_0_lifted_step_res_f_res_38001;
                            }
                            
                            int64_t loopres_38057;
                            int64_t loopres_38058;
                            int64_t loopres_38059;
                            int64_t loopres_38060;
                            int64_t loopres_38061;
                            
                            if (futrts_lifted_normalizze_7743(ctx, &loopres_38057, &loopres_38058, &loopres_38059, &loopres_38060, &loopres_38061, defunc_0_lifted_step_res_37991, ss_37985, defunc_0_lifted_step_res_37992, defunc_0_lifted_step_res_37993, defunc_0_lifted_step_res_37994) != 0) {
                                err = 1;
                                goto cleanup;
                            }
                            
                            bool loop_cond_38062 = slt64((int64_t) 0, loopres_38061);
                            bool loop_while_tmp_40287 = loop_cond_38062;
                            int64_t ss_tmp_40288 = loopres_38057;
                            int64_t ss_tmp_40289 = loopres_38058;
                            int64_t tt_tmp_40290 = loopres_38059;
                            int64_t tt_tmp_40291 = loopres_38060;
                            int64_t count_tmp_40292 = loopres_38061;
                            
                            loop_while_37983 = loop_while_tmp_40287;
                            ss_37984 = ss_tmp_40288;
                            ss_37985 = ss_tmp_40289;
                            tt_37986 = tt_tmp_40290;
                            tt_37987 = tt_tmp_40291;
                            count_37988 = count_tmp_40292;
                        }
                        defunc_0_split_count_res_37977 = loop_while_37983;
                        defunc_0_split_count_res_37978 = ss_37984;
                        defunc_0_split_count_res_37979 = ss_37985;
                        defunc_0_split_count_res_37980 = tt_37986;
                        defunc_0_split_count_res_37981 = tt_37987;
                        defunc_0_split_count_res_37982 = count_37988;
                        ((int64_t *) mem_39912)[i_39530] = defunc_0_split_count_res_37978;
                        ((int64_t *) mem_39914)[i_39530] = defunc_0_split_count_res_37980;
                    }
                    if (mem_39929_cached_sizze_40382 < bytes_39928) {
                        err = lexical_realloc(ctx, &mem_39929, &mem_39929_cached_sizze_40382, bytes_39928);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    if (mem_39932_cached_sizze_40383 < bytes_39928) {
                        err = lexical_realloc(ctx, &mem_39932, &mem_39932_cached_sizze_40383, bytes_39928);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    for (int64_t i_39537 = 0; i_39537 < num_blocks_37838; i_39537++) {
                        bool y_38068 = slt64(i_39537, dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37839);
                        bool index_certs_38070;
                        
                        if (!y_38068) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_39537, "] out of bounds for array of shape [", (long long) dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37839, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:151:22-34\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  ../../../joins/ftSMJ.fut:104:60-63\n   #9  test_matching.fut:52:43-47\n   #10 test_matching.fut:45:1-54:44\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t defunc_0_f_res_38071 = ((int64_t *) mem_39912)[i_39537];
                        int64_t defunc_0_f_res_38072 = ((int64_t *) mem_39914)[i_39537];
                        int64_t tmp_38073 = add64((int64_t) 1, i_39537);
                        bool x_38074 = sle64((int64_t) 0, tmp_38073);
                        bool y_38075 = slt64(tmp_38073, dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37839);
                        bool bounds_check_38076 = x_38074 && y_38075;
                        bool index_certs_38077;
                        
                        if (!bounds_check_38076) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_38073, "] out of bounds for array of shape [", (long long) dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_37839, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:152:22-36\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  ../../../joins/ftSMJ.fut:104:60-63\n   #9  test_matching.fut:52:43-47\n   #10 test_matching.fut:45:1-54:44\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t defunc_0_f_res_38078 = ((int64_t *) mem_39912)[tmp_38073];
                        int64_t defunc_0_f_res_38079 = ((int64_t *) mem_39914)[tmp_38073];
                        int64_t merge_sequential_arg2_38080 = add64(stride_37817, defunc_0_f_res_38072);
                        int64_t merge_sequential_arg2_38081 = add64(stride_37817, defunc_0_f_res_38079);
                        int64_t j_m_i_38082 = sub64(merge_sequential_arg2_38081, merge_sequential_arg2_38080);
                        bool empty_slice_38083 = j_m_i_38082 == (int64_t) 0;
                        int64_t m_38084 = sub64(j_m_i_38082, (int64_t) 1);
                        int64_t i_p_m_t_s_38085 = add64(merge_sequential_arg2_38080, m_38084);
                        bool zzero_leq_i_p_m_t_s_38086 = sle64((int64_t) 0, i_p_m_t_s_38085);
                        bool i_p_m_t_s_leq_w_38087 = slt64(i_p_m_t_s_38085, next_stride_37818);
                        bool zzero_lte_i_38088 = sle64((int64_t) 0, merge_sequential_arg2_38080);
                        bool i_lte_j_38089 = sle64(merge_sequential_arg2_38080, merge_sequential_arg2_38081);
                        bool y_38090 = i_p_m_t_s_leq_w_38087 && zzero_lte_i_38088;
                        bool y_38091 = zzero_leq_i_p_m_t_s_38086 && y_38090;
                        bool forwards_ok_38092 = i_lte_j_38089 && y_38091;
                        bool ok_or_empty_38093 = empty_slice_38083 || forwards_ok_38092;
                        bool index_certs_38094;
                        
                        if (!ok_or_empty_38093) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) merge_sequential_arg2_38080, ":", (long long) merge_sequential_arg2_38081, "] out of bounds for array of shape [", (long long) next_stride_37818, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:40-58\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  ../../../joins/ftSMJ.fut:104:60-63\n   #9  test_matching.fut:52:43-47\n   #10 test_matching.fut:45:1-54:44\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t j_m_i_38095 = sub64(defunc_0_f_res_38078, defunc_0_f_res_38071);
                        bool empty_slice_38096 = j_m_i_38095 == (int64_t) 0;
                        int64_t m_38097 = sub64(j_m_i_38095, (int64_t) 1);
                        int64_t i_p_m_t_s_38098 = add64(defunc_0_f_res_38071, m_38097);
                        bool zzero_leq_i_p_m_t_s_38099 = sle64((int64_t) 0, i_p_m_t_s_38098);
                        bool i_p_m_t_s_leq_w_38100 = slt64(i_p_m_t_s_38098, next_stride_37818);
                        bool zzero_lte_i_38101 = sle64((int64_t) 0, defunc_0_f_res_38071);
                        bool i_lte_j_38102 = sle64(defunc_0_f_res_38071, defunc_0_f_res_38078);
                        bool y_38103 = i_p_m_t_s_leq_w_38100 && zzero_lte_i_38101;
                        bool y_38104 = zzero_leq_i_p_m_t_s_38099 && y_38103;
                        bool forwards_ok_38105 = i_lte_j_38102 && y_38104;
                        bool ok_or_empty_38106 = empty_slice_38096 || forwards_ok_38105;
                        bool index_certs_38107;
                        
                        if (!ok_or_empty_38106) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) defunc_0_f_res_38071, ":", (long long) defunc_0_f_res_38078, "] out of bounds for array of shape [", (long long) next_stride_37818, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:29-39\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  ../../../joins/ftSMJ.fut:104:60-63\n   #9  test_matching.fut:52:43-47\n   #10 test_matching.fut:45:1-54:44\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        bool cond_38108 = slt64((int64_t) 0, j_m_i_38095);
                        int64_t dummy_38109;
                        int64_t dummy_38110;
                        
                        if (cond_38108) {
                            bool index_certs_39394;
                            
                            if (!cond_38108) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) j_m_i_38095, "].", "-> #0  /prelude/array.fut:22:29-33\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:100:36-42\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #6  /prelude/soacs.fut:59:9-11\n   #7  /prelude/array.fut:241:11-17\n   #8  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #9  /prelude/functional.fut:9:44-45\n   #10 ../../../joins/ftSMJ.fut:104:60-63\n   #11 test_matching.fut:52:43-47\n   #12 test_matching.fut:45:1-54:44\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t slice_39395 = start_37890 + defunc_0_f_res_38071;
                            int64_t head_res_39396 = ((int64_t *) mem_param_39893.mem)[slice_39395];
                            int64_t head_res_39397 = ((int64_t *) mem_param_39896.mem)[slice_39395];
                            
                            dummy_38109 = head_res_39396;
                            dummy_38110 = head_res_39397;
                        } else {
                            bool y_38115 = slt64((int64_t) 0, j_m_i_38082);
                            bool index_certs_38116;
                            
                            if (!y_38115) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) j_m_i_38082, "].", "-> #0  /prelude/array.fut:22:29-33\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:100:48-54\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #6  /prelude/soacs.fut:59:9-11\n   #7  /prelude/array.fut:241:11-17\n   #8  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #9  /prelude/functional.fut:9:44-45\n   #10 ../../../joins/ftSMJ.fut:104:60-63\n   #11 test_matching.fut:52:43-47\n   #12 test_matching.fut:45:1-54:44\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t slice_38117 = start_37890 + merge_sequential_arg2_38080;
                            int64_t head_res_38118 = ((int64_t *) mem_param_39893.mem)[slice_38117];
                            int64_t head_res_38119 = ((int64_t *) mem_param_39896.mem)[slice_38117];
                            
                            dummy_38109 = head_res_38118;
                            dummy_38110 = head_res_38119;
                        }
                        for (int64_t nest_i_40295 = 0; nest_i_40295 < merge_block_sizze_37703; nest_i_40295++) {
                            ((int64_t *) mem_39929)[i_39537 * merge_block_sizze_37703 + nest_i_40295] = dummy_38109;
                        }
                        for (int64_t nest_i_40296 = 0; nest_i_40296 < merge_block_sizze_37703; nest_i_40296++) {
                            ((int64_t *) mem_39932)[i_39537 * merge_block_sizze_37703 + nest_i_40296] = dummy_38110;
                        }
                        
                        int64_t data_38122;
                        int64_t i_38126 = (int64_t) 0;
                        
                        for (int64_t k_38125 = 0; k_38125 < merge_block_sizze_37703; k_38125++) {
                            int64_t j_38129 = sub64(k_38125, i_38126);
                            bool cond_38130 = j_38129 == j_m_i_38082;
                            bool cond_38131;
                            
                            if (cond_38130) {
                                cond_38131 = 1;
                            } else {
                                bool cond_38132 = slt64(i_38126, j_m_i_38095);
                                bool cond_f_res_38133;
                                
                                if (cond_38132) {
                                    bool x_39398 = sle64((int64_t) 0, j_38129);
                                    bool y_39399 = slt64(j_38129, j_m_i_38082);
                                    bool bounds_check_39400 = x_39398 && y_39399;
                                    bool index_certs_39401;
                                    
                                    if (!bounds_check_39400) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_38129, "] out of bounds for array of shape [", (long long) j_m_i_38082, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:55-59\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  ../../../joins/ftSMJ.fut:104:60-63\n   #10 test_matching.fut:52:43-47\n   #11 test_matching.fut:45:1-54:44\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    bool x_39405 = sle64((int64_t) 0, i_38126);
                                    bool bounds_check_39406 = cond_38132 && x_39405;
                                    bool index_certs_39407;
                                    
                                    if (!bounds_check_39406) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_38126, "] out of bounds for array of shape [", (long long) j_m_i_38095, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:50-54\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  ../../../joins/ftSMJ.fut:104:60-63\n   #10 test_matching.fut:52:43-47\n   #11 test_matching.fut:45:1-54:44\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    int64_t slice_39402 = merge_sequential_arg2_38080 + j_38129;
                                    int64_t slice_39403 = start_37890 + slice_39402;
                                    int64_t leq_arg1_39404 = ((int64_t *) mem_param_39893.mem)[slice_39403];
                                    int64_t slice_39408 = defunc_0_f_res_38071 + i_38126;
                                    int64_t slice_39409 = start_37890 + slice_39408;
                                    int64_t leq_arg0_39410 = ((int64_t *) mem_param_39893.mem)[slice_39409];
                                    bool defunc_0_leq_res_39411 = sle64(leq_arg0_39410, leq_arg1_39404);
                                    
                                    cond_f_res_38133 = defunc_0_leq_res_39411;
                                } else {
                                    cond_f_res_38133 = 0;
                                }
                                cond_38131 = cond_f_res_38133;
                            }
                            
                            int64_t loopres_38148;
                            int64_t loopres_38149;
                            int64_t loopres_38150;
                            
                            if (cond_38131) {
                                bool x_39413 = sle64((int64_t) 0, i_38126);
                                bool y_39414 = slt64(i_38126, j_m_i_38095);
                                bool bounds_check_39415 = x_39413 && y_39414;
                                bool index_certs_39416;
                                
                                if (!bounds_check_39415) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_38126, "] out of bounds for array of shape [", (long long) j_m_i_38095, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:106:17-21\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  ../../../joins/ftSMJ.fut:104:60-63\n   #10 test_matching.fut:52:43-47\n   #11 test_matching.fut:45:1-54:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t tmp_39412 = add64((int64_t) 1, i_38126);
                                int64_t slice_39417 = defunc_0_f_res_38071 + i_38126;
                                int64_t slice_39418 = start_37890 + slice_39417;
                                int64_t tmp_39419 = ((int64_t *) mem_param_39893.mem)[slice_39418];
                                int64_t tmp_39420 = ((int64_t *) mem_param_39896.mem)[slice_39418];
                                
                                loopres_38148 = tmp_39412;
                                loopres_38149 = tmp_39419;
                                loopres_38150 = tmp_39420;
                            } else {
                                bool x_38160 = sle64((int64_t) 0, j_38129);
                                bool y_38161 = slt64(j_38129, j_m_i_38082);
                                bool bounds_check_38162 = x_38160 && y_38161;
                                bool index_certs_38163;
                                
                                if (!bounds_check_38162) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_38129, "] out of bounds for array of shape [", (long long) j_m_i_38082, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:108:15-19\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  ../../../joins/ftSMJ.fut:104:60-63\n   #10 test_matching.fut:52:43-47\n   #11 test_matching.fut:45:1-54:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t slice_38164 = merge_sequential_arg2_38080 + j_38129;
                                int64_t slice_38165 = start_37890 + slice_38164;
                                int64_t tmp_38166 = ((int64_t *) mem_param_39893.mem)[slice_38165];
                                int64_t tmp_38167 = ((int64_t *) mem_param_39896.mem)[slice_38165];
                                
                                loopres_38148 = i_38126;
                                loopres_38149 = tmp_38166;
                                loopres_38150 = tmp_38167;
                            }
                            ((int64_t *) mem_39929)[i_39537 * merge_block_sizze_37703 + k_38125] = loopres_38149;
                            ((int64_t *) mem_39932)[i_39537 * merge_block_sizze_37703 + k_38125] = loopres_38150;
                            
                            int64_t i_tmp_40297 = loopres_38148;
                            
                            i_38126 = i_tmp_40297;
                        }
                        data_38122 = i_38126;
                    }
                    if (memblock_alloc(ctx, &mem_39967, bytes_39928, "mem_39967")) {
                        err = 1;
                        goto cleanup;
                    }
                    lmad_copy_8b(ctx, 2, (uint64_t *) mem_39967.mem, (int64_t) 0, (int64_t []) {merge_block_sizze_37703, (int64_t) 1}, (uint64_t *) mem_39929, (int64_t) 0, (int64_t []) {merge_block_sizze_37703, (int64_t) 1}, (int64_t []) {num_blocks_37838, merge_block_sizze_37703});
                    if (memblock_alloc(ctx, &mem_39971, bytes_39928, "mem_39971")) {
                        err = 1;
                        goto cleanup;
                    }
                    lmad_copy_8b(ctx, 2, (uint64_t *) mem_39971.mem, (int64_t) 0, (int64_t []) {merge_block_sizze_37703, (int64_t) 1}, (uint64_t *) mem_39932, (int64_t) 0, (int64_t []) {merge_block_sizze_37703, (int64_t) 1}, (int64_t []) {num_blocks_37838, merge_block_sizze_37703});
                    if (memblock_set(ctx, &ext_mem_39994, &mem_39967, "mem_39967") != 0)
                        return 1;
                    if (memblock_set(ctx, &ext_mem_39991, &mem_39971, "mem_39971") != 0)
                        return 1;
                }
                lmad_copy_8b(ctx, 1, (uint64_t *) mem_39899, i_39544 * next_stride_37818, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_39994.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {next_stride_37818});
                if (memblock_unref(ctx, &ext_mem_39994, "ext_mem_39994") != 0)
                    return 1;
                lmad_copy_8b(ctx, 1, (uint64_t *) mem_39902, i_39544 * next_stride_37818, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_39991.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {next_stride_37818});
                if (memblock_unref(ctx, &ext_mem_39991, "ext_mem_39991") != 0)
                    return 1;
            }
            
            int64_t flat_dim_38174 = next_stride_37818 * num_merges_37822;
            
            if (memblock_alloc(ctx, &mem_40005, bytes_39898, "mem_40005")) {
                err = 1;
                goto cleanup;
            }
            lmad_copy_8b(ctx, 2, (uint64_t *) mem_40005.mem, (int64_t) 0, (int64_t []) {next_stride_37818, (int64_t) 1}, (uint64_t *) mem_39899, (int64_t) 0, (int64_t []) {next_stride_37818, (int64_t) 1}, (int64_t []) {num_merges_37822, next_stride_37818});
            if (memblock_alloc(ctx, &mem_40009, bytes_39898, "mem_40009")) {
                err = 1;
                goto cleanup;
            }
            lmad_copy_8b(ctx, 2, (uint64_t *) mem_40009.mem, (int64_t) 0, (int64_t []) {next_stride_37818, (int64_t) 1}, (uint64_t *) mem_39902, (int64_t) 0, (int64_t []) {next_stride_37818, (int64_t) 1}, (int64_t []) {num_merges_37822, next_stride_37818});
            
            bool loop_cond_38177 = slt64(next_stride_37818, flat_dim_37699);
            
            if (memblock_set(ctx, &mem_param_tmp_40271, &mem_40005, "mem_40005") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_tmp_40272, &mem_40009, "mem_40009") != 0)
                return 1;
            
            int64_t loop_dz2084Uz2082U_tmp_40273 = flat_dim_38174;
            bool loop_while_tmp_40274 = loop_cond_38177;
            int64_t stride_tmp_40277 = next_stride_37818;
            
            if (memblock_set(ctx, &mem_param_39893, &mem_param_tmp_40271, "mem_param_tmp_40271") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_39896, &mem_param_tmp_40272, "mem_param_tmp_40272") != 0)
                return 1;
            loop_dz2084Uz2082U_37813 = loop_dz2084Uz2082U_tmp_40273;
            loop_while_37814 = loop_while_tmp_40274;
            stride_37817 = stride_tmp_40277;
        }
        if (memblock_set(ctx, &ext_mem_40016, &mem_param_39893, "mem_param_39893") != 0)
            return 1;
        if (memblock_set(ctx, &ext_mem_40015, &mem_param_39896, "mem_param_39896") != 0)
            return 1;
        data_37808 = loop_dz2084Uz2082U_37813;
        data_37809 = loop_while_37814;
        data_37812 = stride_37817;
        if (memblock_unref(ctx, &mem_39885, "mem_39885") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39889, "mem_39889") != 0)
            return 1;
        
        bool empty_slice_38178 = n2_30937 == (int64_t) 0;
        bool zzero_leq_i_p_m_t_s_38179 = sle64((int64_t) 0, zs_lhs_37683);
        bool i_p_m_t_s_leq_w_38180 = slt64(zs_lhs_37683, data_37808);
        bool y_38181 = zzero_leq_i_p_m_t_s_38179 && i_p_m_t_s_leq_w_38180;
        bool ok_or_empty_38182 = empty_slice_38178 || y_38181;
        bool index_certs_38183;
        
        if (!ok_or_empty_38182) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) n2_30937, "] out of bounds for array of shape [", (long long) data_37808, "].", "-> #0  /prelude/array.fut:46:45-51\n   #1  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:265:3-14\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #3  /prelude/functional.fut:9:44-45\n   #4  ../../../joins/ftSMJ.fut:104:60-63\n   #5  test_matching.fut:52:43-47\n   #6  test_matching.fut:45:1-54:44\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        if (memblock_set(ctx, &ext_mem_40019, &ext_mem_40015, "ext_mem_40015") != 0)
            return 1;
    }
    if (memblock_unref(ctx, &mem_39814, "mem_39814") != 0)
        return 1;
    if (mem_40021_cached_sizze_40384 < bytes_39813) {
        err = lexical_realloc(ctx, &mem_40021, &mem_40021_cached_sizze_40384, bytes_39813);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    for (int64_t i_39549 = 0; i_39549 < n2_30937; i_39549++) {
        int64_t eta_p_38191 = ((int64_t *) ext_mem_40019.mem)[i_39549];
        bool x_38192 = sle64((int64_t) 0, eta_p_38191);
        bool y_38193 = slt64(eta_p_38191, n2_30937);
        bool bounds_check_38194 = x_38192 && y_38193;
        bool index_certs_38195;
        
        if (!bounds_check_38194) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_38191, "] out of bounds for array of shape [", (long long) n2_30937, "].", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:277:24-28\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:277:8-29\n   #3  ../../../joins/ftSMJ.fut:104:60-63\n   #4  test_matching.fut:52:43-47\n   #5  test_matching.fut:45:1-54:44\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t lifted_lambda_res_38196 = ((int64_t *) ks2_mem_39594.mem)[eta_p_38191];
        
        ((int64_t *) mem_40021)[i_39549] = lifted_lambda_res_38196;
    }
    if (memblock_unref(ctx, &ext_mem_40019, "ext_mem_40019") != 0)
        return 1;
    
    int64_t zm_lhs_38207 = add64((int64_t) 2305843009213693951, n1_30936);
    int64_t zs_lhs_38208 = sub64(zm_lhs_38207, (int64_t) 1);
    int64_t numIter_R_38212 = sdiv64(zs_lhs_38208, (int64_t) 2305843009213693951);
    int64_t zm_lhs_38213 = add64((int64_t) 2305843009213693951, n2_30937);
    int64_t zs_lhs_38214 = sub64(zm_lhs_38213, (int64_t) 1);
    int64_t numIter_S_38215 = sdiv64(zs_lhs_38214, (int64_t) 2305843009213693951);
    
    if (memblock_alloc(ctx, &mem_40029, bytes_39596, "mem_40029")) {
        err = 1;
        goto cleanup;
    }
    lmad_copy_8b(ctx, 1, (uint64_t *) mem_40029.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_39804, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {n1_30936});
    if (futrts_indicesWithIncrement_7982(ctx, &ext_mem_40030, mem_39812, n1_30936, (int64_t) 0) != 0) {
        err = 1;
        goto cleanup;
    }
    if (memblock_unref(ctx, &mem_39812, "mem_39812") != 0)
        return 1;
    if (memblock_alloc(ctx, &mem_40032, bytes_39596, "mem_40032")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t nest_i_40301 = 0; nest_i_40301 < n1_30936; nest_i_40301++) {
        ((int64_t *) mem_40032.mem)[nest_i_40301] = (int64_t) -1;
    }
    if (memblock_alloc(ctx, &mem_40034, bytes_39596, "mem_40034")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t nest_i_40302 = 0; nest_i_40302 < n1_30936; nest_i_40302++) {
        ((int64_t *) mem_40034.mem)[nest_i_40302] = (int64_t) 0;
    }
    
    bool loop_cond_38220 = slt64((int64_t) 0, numIter_R_38212);
    bool defunc_0_mergeJoin_res_38221;
    int64_t defunc_0_mergeJoin_res_38222;
    int64_t defunc_0_mergeJoin_res_38225;
    bool loop_while_38226;
    int64_t iter_38227;
    int64_t first_relevant_in_S_38230;
    
    if (memblock_set(ctx, &mem_param_40037, &mem_40034, "mem_40034") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_40040, &mem_40032, "mem_40032") != 0)
        return 1;
    loop_while_38226 = loop_cond_38220;
    iter_38227 = (int64_t) 0;
    first_relevant_in_S_38230 = (int64_t) 0;
    while (loop_while_38226) {
        int64_t tR_start_38231 = mul64((int64_t) 2305843009213693951, iter_38227);
        int64_t min_arg1_38232 = add64((int64_t) 2305843009213693951, tR_start_38231);
        int64_t min_res_38233 = smin64(n1_30936, min_arg1_38232);
        int64_t dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234 = sub64(min_res_38233, tR_start_38231);
        bool empty_slice_38235 = dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234 == (int64_t) 0;
        int64_t m_38236 = sub64(dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234, (int64_t) 1);
        int64_t i_p_m_t_s_38237 = add64(tR_start_38231, m_38236);
        bool zzero_leq_i_p_m_t_s_38238 = sle64((int64_t) 0, i_p_m_t_s_38237);
        bool i_p_m_t_s_leq_w_38239 = slt64(i_p_m_t_s_38237, n1_30936);
        bool zzero_lte_i_38240 = sle64((int64_t) 0, tR_start_38231);
        bool i_lte_j_38241 = sle64(tR_start_38231, min_res_38233);
        bool y_38242 = i_p_m_t_s_leq_w_38239 && zzero_lte_i_38240;
        bool y_38243 = zzero_leq_i_p_m_t_s_38238 && y_38242;
        bool forwards_ok_38244 = i_lte_j_38241 && y_38243;
        bool ok_or_empty_38245 = empty_slice_38235 || forwards_ok_38244;
        bool index_certs_38246;
        
        if (!ok_or_empty_38245) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tR_start_38231, ":", (long long) min_res_38233, "] out of bounds for array of shape [", (long long) n1_30936, "].", "-> #0  ../../../joins/ftSMJ.fut:288:19-38\n   #1  test_matching.fut:53:79-82\n   #2  test_matching.fut:45:1-54:44\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_40041 = (int64_t) 8 * dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234;
        bool cond_38250 = slt64(first_relevant_in_S_38230, numIter_S_38215);
        bool y_38255 = slt64((int64_t) 0, dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234);
        bool loop_not_taken_38256 = !cond_38250;
        bool protect_assert_disj_38257 = y_38255 || loop_not_taken_38256;
        bool index_certs_38258;
        
        if (!protect_assert_disj_38257) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234, "].", "-> #0  ../../../joins/ftSMJ.fut:303:28-36\n   #1  test_matching.fut:53:79-82\n   #2  test_matching.fut:45:1-54:44\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        bool x_38261 = sle64((int64_t) 0, m_38236);
        bool y_38262 = slt64(m_38236, dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234);
        bool bounds_check_38263 = x_38261 && y_38262;
        bool protect_assert_disj_38264 = loop_not_taken_38256 || bounds_check_38263;
        bool index_certs_38265;
        
        if (!protect_assert_disj_38264) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) m_38236, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234, "].", "-> #0  ../../../joins/ftSMJ.fut:304:30-49\n   #1  test_matching.fut:53:79-82\n   #2  test_matching.fut:45:1-54:44\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        bool index_certs_38476;
        
        if (!ok_or_empty_38245) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tR_start_38231, ":", (long long) min_res_38233, "] out of bounds for array of shape [", (long long) n1_30936, "].", "-> #0  ../../../joins/ftSMJ.fut:198:17-65\n   #1  ../../../joins/ftSMJ.fut:314:22-66\n   #2  test_matching.fut:53:79-82\n   #3  test_matching.fut:45:1-54:44\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        if (memblock_alloc(ctx, &mem_40042, bytes_40041, "mem_40042")) {
            err = 1;
            goto cleanup;
        }
        for (int64_t nest_i_40310 = 0; nest_i_40310 < dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234; nest_i_40310++) {
            ((int64_t *) mem_40042.mem)[nest_i_40310] = (int64_t) -1;
        }
        if (memblock_alloc(ctx, &mem_40044, bytes_40041, "mem_40044")) {
            err = 1;
            goto cleanup;
        }
        for (int64_t nest_i_40311 = 0; nest_i_40311 < dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234; nest_i_40311++) {
            ((int64_t *) mem_40044.mem)[nest_i_40311] = (int64_t) 0;
        }
        
        int64_t gt_lhs_38259;
        
        if (cond_38250) {
            int64_t x_39431 = ((int64_t *) mem_39804)[tR_start_38231];
            
            gt_lhs_38259 = x_39431;
        } else {
            gt_lhs_38259 = (int64_t) 0;
        }
        
        int64_t lt_lhs_38266;
        
        if (cond_38250) {
            int64_t x_39432 = ((int64_t *) mem_39804)[i_p_m_t_s_38237];
            
            lt_lhs_38266 = x_39432;
        } else {
            lt_lhs_38266 = (int64_t) 0;
        }
        
        int64_t ext_40110;
        int64_t ext_40109;
        int64_t ext_40108;
        int64_t ext_40107;
        bool loopres_38268;
        int64_t loopres_38269;
        int64_t loopres_38272;
        bool loop_while_38273;
        int64_t s_iter_38274;
        int64_t minRelevant_38277;
        int64_t ctx_param_ext_40045;
        int64_t ctx_param_ext_40046;
        int64_t ctx_param_ext_40048;
        int64_t ctx_param_ext_40049;
        
        if (memblock_set(ctx, &mem_param_40047, &mem_40044, "mem_40044") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_40050, &mem_40042, "mem_40042") != 0)
            return 1;
        ctx_param_ext_40045 = (int64_t) 0;
        ctx_param_ext_40046 = (int64_t) 1;
        ctx_param_ext_40048 = (int64_t) 0;
        ctx_param_ext_40049 = (int64_t) 1;
        loop_while_38273 = cond_38250;
        s_iter_38274 = first_relevant_in_S_38230;
        minRelevant_38277 = first_relevant_in_S_38230;
        while (loop_while_38273) {
            int64_t tS_start_38278 = mul64((int64_t) 2305843009213693951, s_iter_38274);
            int64_t min_arg1_38279 = add64((int64_t) 2305843009213693951, tS_start_38278);
            int64_t min_res_38280 = smin64(n2_30937, min_arg1_38279);
            int64_t dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281 = sub64(min_res_38280, tS_start_38278);
            bool empty_slice_38282 = dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281 == (int64_t) 0;
            int64_t m_38283 = sub64(dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281, (int64_t) 1);
            int64_t i_p_m_t_s_38284 = add64(tS_start_38278, m_38283);
            bool zzero_leq_i_p_m_t_s_38285 = sle64((int64_t) 0, i_p_m_t_s_38284);
            bool i_p_m_t_s_leq_w_38286 = slt64(i_p_m_t_s_38284, n2_30937);
            bool zzero_lte_i_38287 = sle64((int64_t) 0, tS_start_38278);
            bool i_lte_j_38288 = sle64(tS_start_38278, min_res_38280);
            bool y_38289 = i_p_m_t_s_leq_w_38286 && zzero_lte_i_38287;
            bool y_38290 = zzero_leq_i_p_m_t_s_38285 && y_38289;
            bool forwards_ok_38291 = i_lte_j_38288 && y_38290;
            bool ok_or_empty_38292 = empty_slice_38282 || forwards_ok_38291;
            bool index_certs_38293;
            
            if (!ok_or_empty_38292) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tS_start_38278, ":", (long long) min_res_38280, "] out of bounds for array of shape [", (long long) n2_30937, "].", "-> #0  ../../../joins/ftSMJ.fut:301:21-40\n   #1  test_matching.fut:53:79-82\n   #2  test_matching.fut:45:1-54:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool x_38295 = sle64((int64_t) 0, m_38283);
            bool y_38296 = slt64(m_38283, dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281);
            bool bounds_check_38297 = x_38295 && y_38296;
            bool index_certs_38298;
            
            if (!bounds_check_38297) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) m_38283, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281, "].", "-> #0  ../../../joins/ftSMJ.fut:303:42-61\n   #1  test_matching.fut:53:79-82\n   #2  test_matching.fut:45:1-54:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool y_38301 = slt64((int64_t) 0, dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281);
            bool index_certs_38302;
            
            if (!y_38301) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281, "].", "-> #0  ../../../joins/ftSMJ.fut:304:55-63\n   #1  test_matching.fut:53:79-82\n   #2  test_matching.fut:45:1-54:44\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t gt_rhs_38299 = ((int64_t *) mem_40021)[i_p_m_t_s_38284];
            bool defunc_0_gt_res_38300 = slt64(gt_rhs_38299, gt_lhs_38259);
            int64_t lt_rhs_38303 = ((int64_t *) mem_40021)[tS_start_38278];
            bool defunc_0_lt_res_38304 = slt64(lt_lhs_38266, lt_rhs_38303);
            bool thisStillRel_38305 = !defunc_0_lt_res_38304;
            int64_t ext_40099;
            
            if (defunc_0_lt_res_38304) {
                ext_40099 = ctx_param_ext_40045;
            } else {
                ext_40099 = (int64_t) 0;
            }
            
            int64_t ext_40098;
            
            if (defunc_0_lt_res_38304) {
                ext_40098 = ctx_param_ext_40046;
            } else {
                ext_40098 = (int64_t) 1;
            }
            
            int64_t ext_40096;
            
            if (defunc_0_lt_res_38304) {
                ext_40096 = ctx_param_ext_40048;
            } else {
                ext_40096 = (int64_t) 0;
            }
            
            int64_t ext_40095;
            
            if (defunc_0_lt_res_38304) {
                ext_40095 = ctx_param_ext_40049;
            } else {
                ext_40095 = (int64_t) 1;
            }
            
            int64_t ext_40105;
            
            if (defunc_0_gt_res_38300) {
                ext_40105 = ctx_param_ext_40045;
            } else {
                ext_40105 = ext_40099;
            }
            
            int64_t ext_40104;
            
            if (defunc_0_gt_res_38300) {
                ext_40104 = ctx_param_ext_40046;
            } else {
                ext_40104 = ext_40098;
            }
            
            int64_t ext_40102;
            
            if (defunc_0_gt_res_38300) {
                ext_40102 = ctx_param_ext_40048;
            } else {
                ext_40102 = ext_40096;
            }
            
            int64_t ext_40101;
            
            if (defunc_0_gt_res_38300) {
                ext_40101 = ctx_param_ext_40049;
            } else {
                ext_40101 = ext_40095;
            }
            
            int64_t loopres_38306;
            bool loopres_38309;
            int64_t loopres_38310;
            
            if (defunc_0_gt_res_38300) {
                int64_t tmp_39433 = add64((int64_t) 1, s_iter_38274);
                
                if (memblock_set(ctx, &ext_mem_40106, &mem_param_40047, "mem_param_40047") != 0)
                    return 1;
                if (memblock_set(ctx, &ext_mem_40103, &mem_param_40050, "mem_param_40050") != 0)
                    return 1;
                loopres_38306 = tmp_39433;
                loopres_38309 = 1;
                loopres_38310 = tmp_39433;
            } else {
                int64_t loopres_f_res_38312;
                bool loopres_f_res_38315;
                
                if (defunc_0_lt_res_38304) {
                    if (memblock_set(ctx, &ext_mem_40100, &mem_param_40047, "mem_param_40047") != 0)
                        return 1;
                    if (memblock_set(ctx, &ext_mem_40097, &mem_param_40050, "mem_param_40050") != 0)
                        return 1;
                    loopres_f_res_38312 = s_iter_38274;
                    loopres_f_res_38315 = thisStillRel_38305;
                } else {
                    bool index_certs_38316;
                    
                    if (!bounds_check_38263) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) m_38236, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234, "].", "-> #0  ../../../joins/ftSMJ.fut:308:27-46\n   #1  test_matching.fut:53:79-82\n   #2  test_matching.fut:45:1-54:44\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t lt_lhs_38317 = ((int64_t *) mem_39804)[i_p_m_t_s_38237];
                    bool defunc_0_lt_res_38319 = slt64(lt_lhs_38317, gt_rhs_38299);
                    bool nextRel_38320 = !defunc_0_lt_res_38319;
                    double i64_res_38322 = sitofp_i64_f64(dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281);
                    double log2_res_38323 = futrts_log2_64(i64_res_38322);
                    double ceil_res_38324 = futrts_ceil64(log2_res_38323);
                    int64_t f64_res_38325 = fptosi_f64_i64(ceil_res_38324);
                    int64_t num_iter_38326 = add64((int64_t) 1, f64_res_38325);
                    int64_t bsearch_first_38329;
                    int64_t last_step_38332;
                    
                    if (memblock_set(ctx, &mem_param_40053, &mem_40044, "mem_40044") != 0)
                        return 1;
                    last_step_38332 = dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281;
                    for (int64_t i_38330 = 0; i_38330 < num_iter_38326; i_38330++) {
                        int64_t zs_lhs_38333 = add64((int64_t) 1, last_step_38332);
                        int64_t this_step_38334 = sdiv64(zs_lhs_38333, (int64_t) 2);
                        
                        if (memblock_alloc(ctx, &mem_40055, bytes_40041, "mem_40055")) {
                            err = 1;
                            goto cleanup;
                        }
                        for (int64_t i_39553 = 0; i_39553 < dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234; i_39553++) {
                            int64_t eta_p_38840 = ((int64_t *) mem_param_40053.mem)[i_39553];
                            int64_t slice_39567 = tR_start_38231 + i_39553;
                            int64_t eta_p_38841 = ((int64_t *) mem_39804)[slice_39567];
                            bool cond_38843 = sle64(eta_p_38840, (int64_t) 0);
                            int64_t prev_elem_38844;
                            
                            if (cond_38843) {
                                bool index_certs_39435;
                                
                                if (!y_38301) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281, "].", "-> #0  ../../../joins/ftSMJ.fut:219:42-47\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftSMJ.fut:218:14-222:12\n   #3  test_matching.fut:53:79-82\n   #4  test_matching.fut:45:1-54:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                prev_elem_38844 = lt_rhs_38303;
                            } else {
                                int64_t tmp_38848 = sub64(eta_p_38840, (int64_t) 1);
                                bool x_38849 = sle64((int64_t) 0, tmp_38848);
                                bool y_38850 = slt64(tmp_38848, dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281);
                                bool bounds_check_38851 = x_38849 && y_38850;
                                bool index_certs_38852;
                                
                                if (!bounds_check_38851) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_38848, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281, "].", "-> #0  ../../../joins/ftSMJ.fut:219:53-60\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftSMJ.fut:218:14-222:12\n   #3  test_matching.fut:53:79-82\n   #4  test_matching.fut:45:1-54:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t slice_38853 = tS_start_38278 + tmp_38848;
                                int64_t prev_elem_f_res_38854 = ((int64_t *) mem_40021)[slice_38853];
                                
                                prev_elem_38844 = prev_elem_f_res_38854;
                            }
                            
                            bool cond_38855 = slt64(eta_p_38840, (int64_t) 0);
                            int64_t cur_elem_38856;
                            
                            if (cond_38855) {
                                bool index_certs_39438;
                                
                                if (!y_38301) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281, "].", "-> #0  ../../../joins/ftSMJ.fut:220:40-45\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftSMJ.fut:218:14-222:12\n   #3  test_matching.fut:53:79-82\n   #4  test_matching.fut:45:1-54:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                cur_elem_38856 = lt_rhs_38303;
                            } else {
                                bool x_38860 = sle64((int64_t) 0, eta_p_38840);
                                bool y_38861 = slt64(eta_p_38840, dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281);
                                bool bounds_check_38862 = x_38860 && y_38861;
                                bool index_certs_38863;
                                
                                if (!bounds_check_38862) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_38840, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281, "].", "-> #0  ../../../joins/ftSMJ.fut:220:51-56\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftSMJ.fut:218:14-222:12\n   #3  test_matching.fut:53:79-82\n   #4  test_matching.fut:45:1-54:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t slice_38864 = tS_start_38278 + eta_p_38840;
                                int64_t cur_elem_f_res_38865 = ((int64_t *) mem_40021)[slice_38864];
                                
                                cur_elem_38856 = cur_elem_f_res_38865;
                            }
                            
                            int64_t defunc_0_f_res_38869;
                            
                            if (cond_38855) {
                                defunc_0_f_res_38869 = (int64_t) -1;
                            } else {
                                bool defunc_0_eq_res_38870 = eta_p_38841 == cur_elem_38856;
                                bool cond_38871 = eta_p_38840 == (int64_t) 0;
                                bool defunc_0_gt_res_38872 = slt64(prev_elem_38844, eta_p_38841);
                                bool x_38873 = !cond_38871;
                                bool y_38874 = defunc_0_gt_res_38872 && x_38873;
                                bool cond_t_res_38875 = cond_38871 || y_38874;
                                bool x_38876 = defunc_0_eq_res_38870 && cond_t_res_38875;
                                int64_t defunc_0_f_res_f_res_38877;
                                
                                if (x_38876) {
                                    defunc_0_f_res_f_res_38877 = eta_p_38840;
                                } else {
                                    int64_t defunc_0_f_res_f_res_f_res_38878;
                                    
                                    if (defunc_0_eq_res_38870) {
                                        int64_t max_arg1_39440 = sub64(eta_p_38840, this_step_38334);
                                        int64_t max_res_39441 = smax64((int64_t) 0, max_arg1_39440);
                                        
                                        defunc_0_f_res_f_res_f_res_38878 = max_res_39441;
                                    } else {
                                        bool defunc_0_gt_res_38881 = slt64(cur_elem_38856, eta_p_38841);
                                        int64_t defunc_0_f_res_f_res_f_res_f_res_38882;
                                        
                                        if (defunc_0_gt_res_38881) {
                                            bool cond_39443 = eta_p_38840 == m_38283;
                                            int64_t defunc_0_f_res_f_res_f_res_f_res_t_res_39444;
                                            
                                            if (cond_39443) {
                                                defunc_0_f_res_f_res_f_res_f_res_t_res_39444 = (int64_t) -1;
                                            } else {
                                                int64_t min_arg1_39445 = add64(this_step_38334, eta_p_38840);
                                                int64_t min_res_39446 = smin64(m_38283, min_arg1_39445);
                                                
                                                defunc_0_f_res_f_res_f_res_f_res_t_res_39444 = min_res_39446;
                                            }
                                            defunc_0_f_res_f_res_f_res_f_res_38882 = defunc_0_f_res_f_res_f_res_f_res_t_res_39444;
                                        } else {
                                            int64_t defunc_0_f_res_f_res_f_res_f_res_f_res_38888;
                                            
                                            if (cond_t_res_38875) {
                                                defunc_0_f_res_f_res_f_res_f_res_f_res_38888 = (int64_t) -1;
                                            } else {
                                                int64_t max_arg1_38889 = sub64(eta_p_38840, this_step_38334);
                                                int64_t max_res_38890 = smax64((int64_t) 0, max_arg1_38889);
                                                
                                                defunc_0_f_res_f_res_f_res_f_res_f_res_38888 = max_res_38890;
                                            }
                                            defunc_0_f_res_f_res_f_res_f_res_38882 = defunc_0_f_res_f_res_f_res_f_res_f_res_38888;
                                        }
                                        defunc_0_f_res_f_res_f_res_38878 = defunc_0_f_res_f_res_f_res_f_res_38882;
                                    }
                                    defunc_0_f_res_f_res_38877 = defunc_0_f_res_f_res_f_res_38878;
                                }
                                defunc_0_f_res_38869 = defunc_0_f_res_f_res_38877;
                            }
                            ((int64_t *) mem_40055.mem)[i_39553] = defunc_0_f_res_38869;
                        }
                        if (memblock_set(ctx, &mem_param_tmp_40323, &mem_40055, "mem_40055") != 0)
                            return 1;
                        
                        int64_t last_step_tmp_40325 = this_step_38334;
                        
                        if (memblock_set(ctx, &mem_param_40053, &mem_param_tmp_40323, "mem_param_tmp_40323") != 0)
                            return 1;
                        last_step_38332 = last_step_tmp_40325;
                    }
                    if (memblock_set(ctx, &ext_mem_40064, &mem_param_40053, "mem_param_40053") != 0)
                        return 1;
                    bsearch_first_38329 = last_step_38332;
                    
                    int64_t bsearch_last_38389;
                    int64_t last_step_38392;
                    
                    if (memblock_set(ctx, &mem_param_40067, &ext_mem_40064, "ext_mem_40064") != 0)
                        return 1;
                    last_step_38392 = dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281;
                    for (int64_t i_38390 = 0; i_38390 < num_iter_38326; i_38390++) {
                        int64_t zs_lhs_38393 = add64((int64_t) 1, last_step_38392);
                        int64_t max_arg1_38394 = sdiv64(zs_lhs_38393, (int64_t) 2);
                        int64_t max_res_38395 = smax64((int64_t) 1, max_arg1_38394);
                        
                        if (memblock_alloc(ctx, &mem_40069, bytes_40041, "mem_40069")) {
                            err = 1;
                            goto cleanup;
                        }
                        for (int64_t i_39557 = 0; i_39557 < dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234; i_39557++) {
                            int64_t eta_p_38920 = ((int64_t *) mem_param_40067.mem)[i_39557];
                            int64_t slice_39566 = tR_start_38231 + i_39557;
                            int64_t eta_p_38921 = ((int64_t *) mem_39804)[slice_39566];
                            bool cond_38923 = slt64(eta_p_38920, (int64_t) 0);
                            int64_t cur_elem_38924;
                            
                            if (cond_38923) {
                                bool index_certs_39449;
                                
                                if (!y_38301) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281, "].", "-> #0  ../../../joins/ftSMJ.fut:244:40-45\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftSMJ.fut:243:14-247:12\n   #3  test_matching.fut:53:79-82\n   #4  test_matching.fut:45:1-54:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                cur_elem_38924 = lt_rhs_38303;
                            } else {
                                bool x_38928 = sle64((int64_t) 0, eta_p_38920);
                                bool y_38929 = slt64(eta_p_38920, dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281);
                                bool bounds_check_38930 = x_38928 && y_38929;
                                bool index_certs_38931;
                                
                                if (!bounds_check_38930) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_38920, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281, "].", "-> #0  ../../../joins/ftSMJ.fut:244:51-56\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftSMJ.fut:243:14-247:12\n   #3  test_matching.fut:53:79-82\n   #4  test_matching.fut:45:1-54:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t slice_38932 = tS_start_38278 + eta_p_38920;
                                int64_t cur_elem_f_res_38933 = ((int64_t *) mem_40021)[slice_38932];
                                
                                cur_elem_38924 = cur_elem_f_res_38933;
                            }
                            
                            bool cond_38934 = sle64(m_38283, eta_p_38920);
                            int64_t next_elem_38935;
                            
                            if (cond_38934) {
                                bool index_certs_39455;
                                
                                if (!bounds_check_38297) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) m_38283, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281, "].", "-> #0  ../../../joins/ftSMJ.fut:245:47-55\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftSMJ.fut:243:14-247:12\n   #3  test_matching.fut:53:79-82\n   #4  test_matching.fut:45:1-54:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                next_elem_38935 = gt_rhs_38299;
                            } else {
                                int64_t tmp_38943 = add64((int64_t) 1, eta_p_38920);
                                bool x_38944 = sle64((int64_t) 0, tmp_38943);
                                bool y_38945 = slt64(tmp_38943, dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281);
                                bool bounds_check_38946 = x_38944 && y_38945;
                                bool index_certs_38947;
                                
                                if (!bounds_check_38946) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_38943, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20UtS_endz20UtS_startz7dUzg_38281, "].", "-> #0  ../../../joins/ftSMJ.fut:245:61-68\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../../joins/ftSMJ.fut:243:14-247:12\n   #3  test_matching.fut:53:79-82\n   #4  test_matching.fut:45:1-54:44\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t slice_38948 = tS_start_38278 + tmp_38943;
                                int64_t next_elem_f_res_38949 = ((int64_t *) mem_40021)[slice_38948];
                                
                                next_elem_38935 = next_elem_f_res_38949;
                            }
                            
                            int64_t defunc_0_f_res_38953;
                            
                            if (cond_38923) {
                                defunc_0_f_res_38953 = (int64_t) -1;
                            } else {
                                bool defunc_0_eq_res_38954 = eta_p_38921 == cur_elem_38924;
                                bool cond_38956 = eta_p_38920 == m_38283;
                                bool defunc_0_gt_res_38957 = slt64(eta_p_38921, next_elem_38935);
                                bool x_38958 = !cond_38956;
                                bool y_38959 = defunc_0_gt_res_38957 && x_38958;
                                bool cond_t_res_38960 = cond_38956 || y_38959;
                                bool x_38961 = defunc_0_eq_res_38954 && cond_t_res_38960;
                                int64_t defunc_0_f_res_f_res_38962;
                                
                                if (x_38961) {
                                    defunc_0_f_res_f_res_38962 = eta_p_38920;
                                } else {
                                    int64_t defunc_0_f_res_f_res_f_res_38963;
                                    
                                    if (defunc_0_eq_res_38954) {
                                        int64_t min_arg1_39458 = add64(max_res_38395, eta_p_38920);
                                        int64_t min_res_39459 = smin64(m_38283, min_arg1_39458);
                                        
                                        defunc_0_f_res_f_res_f_res_38963 = min_res_39459;
                                    } else {
                                        bool defunc_0_gt_res_38966 = slt64(cur_elem_38924, eta_p_38921);
                                        int64_t defunc_0_f_res_f_res_f_res_f_res_38967;
                                        
                                        if (defunc_0_gt_res_38966) {
                                            int64_t min_arg1_39460 = add64(max_res_38395, eta_p_38920);
                                            int64_t min_res_39461 = smin64(m_38283, min_arg1_39460);
                                            
                                            defunc_0_f_res_f_res_f_res_f_res_38967 = min_res_39461;
                                        } else {
                                            int64_t max_arg1_38970 = sub64(eta_p_38920, max_res_38395);
                                            int64_t max_res_38971 = smax64((int64_t) 0, max_arg1_38970);
                                            
                                            defunc_0_f_res_f_res_f_res_f_res_38967 = max_res_38971;
                                        }
                                        defunc_0_f_res_f_res_f_res_38963 = defunc_0_f_res_f_res_f_res_f_res_38967;
                                    }
                                    defunc_0_f_res_f_res_38962 = defunc_0_f_res_f_res_f_res_38963;
                                }
                                defunc_0_f_res_38953 = defunc_0_f_res_f_res_38962;
                            }
                            ((int64_t *) mem_40069.mem)[i_39557] = defunc_0_f_res_38953;
                        }
                        if (memblock_set(ctx, &mem_param_tmp_40327, &mem_40069, "mem_40069") != 0)
                            return 1;
                        
                        int64_t last_step_tmp_40329 = max_res_38395;
                        
                        if (memblock_set(ctx, &mem_param_40067, &mem_param_tmp_40327, "mem_param_tmp_40327") != 0)
                            return 1;
                        last_step_38392 = last_step_tmp_40329;
                    }
                    if (memblock_set(ctx, &ext_mem_40078, &mem_param_40067, "mem_param_40067") != 0)
                        return 1;
                    bsearch_last_38389 = last_step_38392;
                    if (memblock_alloc(ctx, &mem_40080, bytes_40041, "mem_40080")) {
                        err = 1;
                        goto cleanup;
                    }
                    if (memblock_alloc(ctx, &mem_40082, bytes_40041, "mem_40082")) {
                        err = 1;
                        goto cleanup;
                    }
                    for (int64_t i_39563 = 0; i_39563 < dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234; i_39563++) {
                        int64_t eta_p_38820 = ((int64_t *) ext_mem_40064.mem)[i_39563];
                        int64_t eta_p_38821 = ((int64_t *) mem_param_40050.mem)[ctx_param_ext_40048 + i_39563 * ctx_param_ext_40049];
                        int64_t eta_p_38822 = ((int64_t *) ext_mem_40078.mem)[i_39563];
                        int64_t eta_p_38823 = ((int64_t *) mem_param_40047.mem)[ctx_param_ext_40045 + i_39563 * ctx_param_ext_40046];
                        bool cond_38825 = slt64(eta_p_38820, (int64_t) 0);
                        int64_t lifted_lambda_res_38826;
                        
                        if (cond_38825) {
                            lifted_lambda_res_38826 = (int64_t) 0;
                        } else {
                            int64_t zp_lhs_38827 = sub64(eta_p_38822, eta_p_38820);
                            int64_t lifted_lambda_res_f_res_38828 = add64((int64_t) 1, zp_lhs_38827);
                            
                            lifted_lambda_res_38826 = lifted_lambda_res_f_res_38828;
                        }
                        
                        int64_t defunc_0_f_res_38830 = add64(eta_p_38823, lifted_lambda_res_38826);
                        int64_t lifted_lambda_res_38833;
                        
                        if (cond_38825) {
                            lifted_lambda_res_38833 = eta_p_38820;
                        } else {
                            int64_t lifted_lambda_res_f_res_38834 = add64(tS_start_38278, eta_p_38820);
                            
                            lifted_lambda_res_38833 = lifted_lambda_res_f_res_38834;
                        }
                        
                        bool cond_38836 = slt64(eta_p_38821, (int64_t) 0);
                        int64_t lifted_lambda_res_38837;
                        
                        if (cond_38836) {
                            lifted_lambda_res_38837 = lifted_lambda_res_38833;
                        } else {
                            lifted_lambda_res_38837 = eta_p_38821;
                        }
                        ((int64_t *) mem_40080.mem)[i_39563] = lifted_lambda_res_38837;
                        ((int64_t *) mem_40082.mem)[i_39563] = defunc_0_f_res_38830;
                    }
                    if (memblock_unref(ctx, &ext_mem_40064, "ext_mem_40064") != 0)
                        return 1;
                    if (memblock_unref(ctx, &ext_mem_40078, "ext_mem_40078") != 0)
                        return 1;
                    
                    int64_t tmp_38472 = add64((int64_t) 1, s_iter_38274);
                    
                    if (memblock_set(ctx, &ext_mem_40100, &mem_40082, "mem_40082") != 0)
                        return 1;
                    if (memblock_set(ctx, &ext_mem_40097, &mem_40080, "mem_40080") != 0)
                        return 1;
                    loopres_f_res_38312 = tmp_38472;
                    loopres_f_res_38315 = nextRel_38320;
                }
                if (memblock_set(ctx, &ext_mem_40106, &ext_mem_40100, "ext_mem_40100") != 0)
                    return 1;
                if (memblock_set(ctx, &ext_mem_40103, &ext_mem_40097, "ext_mem_40097") != 0)
                    return 1;
                loopres_38306 = loopres_f_res_38312;
                loopres_38309 = loopres_f_res_38315;
                loopres_38310 = minRelevant_38277;
            }
            
            bool cond_38473 = slt64(loopres_38306, numIter_S_38215);
            bool x_38474 = loopres_38309 && cond_38473;
            
            if (memblock_set(ctx, &mem_param_tmp_40312, &ext_mem_40106, "ext_mem_40106") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_tmp_40313, &ext_mem_40103, "ext_mem_40103") != 0)
                return 1;
            
            int64_t ctx_param_ext_tmp_40314 = ext_40105;
            int64_t ctx_param_ext_tmp_40315 = ext_40104;
            int64_t ctx_param_ext_tmp_40316 = ext_40102;
            int64_t ctx_param_ext_tmp_40317 = ext_40101;
            bool loop_while_tmp_40318 = x_38474;
            int64_t s_iter_tmp_40319 = loopres_38306;
            int64_t minRelevant_tmp_40322 = loopres_38310;
            
            if (memblock_set(ctx, &mem_param_40047, &mem_param_tmp_40312, "mem_param_tmp_40312") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_40050, &mem_param_tmp_40313, "mem_param_tmp_40313") != 0)
                return 1;
            ctx_param_ext_40045 = ctx_param_ext_tmp_40314;
            ctx_param_ext_40046 = ctx_param_ext_tmp_40315;
            ctx_param_ext_40048 = ctx_param_ext_tmp_40316;
            ctx_param_ext_40049 = ctx_param_ext_tmp_40317;
            loop_while_38273 = loop_while_tmp_40318;
            s_iter_38274 = s_iter_tmp_40319;
            minRelevant_38277 = minRelevant_tmp_40322;
        }
        if (memblock_set(ctx, &ext_mem_40112, &mem_param_40047, "mem_param_40047") != 0)
            return 1;
        if (memblock_set(ctx, &ext_mem_40111, &mem_param_40050, "mem_param_40050") != 0)
            return 1;
        ext_40110 = ctx_param_ext_40045;
        ext_40109 = ctx_param_ext_40046;
        ext_40108 = ctx_param_ext_40048;
        ext_40107 = ctx_param_ext_40049;
        loopres_38268 = loop_while_38273;
        loopres_38269 = s_iter_38274;
        loopres_38272 = minRelevant_38277;
        if (memblock_unref(ctx, &mem_40042, "mem_40042") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_40044, "mem_40044") != 0)
            return 1;
        if (memblock_alloc(ctx, &mem_40114, bytes_39596, "mem_40114")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_40114.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_40040.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {n1_30936});
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_40114.mem, tR_start_38231, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_40111.mem, ext_40108, (int64_t []) {ext_40107}, (int64_t []) {dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234});
        if (memblock_unref(ctx, &ext_mem_40111, "ext_mem_40111") != 0)
            return 1;
        if (memblock_alloc(ctx, &mem_40116, bytes_39596, "mem_40116")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_40116.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_40037.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {n1_30936});
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_40116.mem, tR_start_38231, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_40112.mem, ext_40110, (int64_t []) {ext_40109}, (int64_t []) {dzlz7bUZLzmZRz20UtR_endz20UtR_startz7dUzg_38234});
        if (memblock_unref(ctx, &ext_mem_40112, "ext_mem_40112") != 0)
            return 1;
        
        int64_t tmp_38480 = add64((int64_t) 1, iter_38227);
        bool loop_cond_38481 = slt64(tmp_38480, numIter_R_38212);
        
        if (memblock_set(ctx, &mem_param_tmp_40303, &mem_40116, "mem_40116") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_40304, &mem_40114, "mem_40114") != 0)
            return 1;
        
        bool loop_while_tmp_40305 = loop_cond_38481;
        int64_t iter_tmp_40306 = tmp_38480;
        int64_t first_relevant_in_S_tmp_40309 = loopres_38272;
        
        if (memblock_set(ctx, &mem_param_40037, &mem_param_tmp_40303, "mem_param_tmp_40303") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_40040, &mem_param_tmp_40304, "mem_param_tmp_40304") != 0)
            return 1;
        loop_while_38226 = loop_while_tmp_40305;
        iter_38227 = iter_tmp_40306;
        first_relevant_in_S_38230 = first_relevant_in_S_tmp_40309;
    }
    if (memblock_set(ctx, &ext_mem_40122, &mem_param_40037, "mem_param_40037") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_40121, &mem_param_40040, "mem_param_40040") != 0)
        return 1;
    defunc_0_mergeJoin_res_38221 = loop_while_38226;
    defunc_0_mergeJoin_res_38222 = iter_38227;
    defunc_0_mergeJoin_res_38225 = first_relevant_in_S_38230;
    if (memblock_unref(ctx, &mem_40032, "mem_40032") != 0)
        return 1;
    if (memblock_unref(ctx, &mem_40034, "mem_40034") != 0)
        return 1;
    if (memblock_set(ctx, &mem_out_40205, &mem_40029, "mem_40029") != 0)
        return 1;
    if (memblock_set(ctx, &mem_out_40206, &ext_mem_40030, "ext_mem_40030") != 0)
        return 1;
    if (memblock_set(ctx, &mem_out_40207, &ext_mem_40121, "ext_mem_40121") != 0)
        return 1;
    if (memblock_set(ctx, &mem_out_40208, &ext_mem_40122, "ext_mem_40122") != 0)
        return 1;
    if (memblock_set(ctx, &*mem_out_p_40359, &mem_out_40205, "mem_out_40205") != 0)
        return 1;
    if (memblock_set(ctx, &*mem_out_p_40360, &mem_out_40206, "mem_out_40206") != 0)
        return 1;
    if (memblock_set(ctx, &*mem_out_p_40361, &mem_out_40207, "mem_out_40207") != 0)
        return 1;
    if (memblock_set(ctx, &*mem_out_p_40362, &mem_out_40208, "mem_out_40208") != 0)
        return 1;
    
  cleanup:
    {
        free(mem_39600);
        free(mem_39603);
        free(mem_39613);
        free(mem_39615);
        free(mem_39682);
        free(mem_39685);
        free(mem_39695);
        free(mem_39697);
        free(mem_39712);
        free(mem_39715);
        free(mem_39804);
        free(mem_39817);
        free(mem_39820);
        free(mem_39830);
        free(mem_39832);
        free(mem_39899);
        free(mem_39902);
        free(mem_39912);
        free(mem_39914);
        free(mem_39929);
        free(mem_39932);
        free(mem_40021);
        if (memblock_unref(ctx, &mem_param_tmp_40304, "mem_param_tmp_40304") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_40303, "mem_param_tmp_40303") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_40116, "mem_40116") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_40114, "mem_40114") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_40313, "mem_param_tmp_40313") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_40312, "mem_param_tmp_40312") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_40082, "mem_40082") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_40080, "mem_40080") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_40327, "mem_param_tmp_40327") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_40069, "mem_40069") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_40067, "mem_param_40067") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_40078, "ext_mem_40078") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_40323, "mem_param_tmp_40323") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_40055, "mem_40055") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_40053, "mem_param_40053") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_40064, "ext_mem_40064") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_40097, "ext_mem_40097") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_40100, "ext_mem_40100") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_40103, "ext_mem_40103") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_40106, "ext_mem_40106") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_40050, "mem_param_40050") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_40047, "mem_param_40047") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_40111, "ext_mem_40111") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_40112, "ext_mem_40112") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_40044, "mem_40044") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_40042, "mem_40042") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_40040, "mem_param_40040") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_40037, "mem_param_40037") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_40121, "ext_mem_40121") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_40122, "ext_mem_40122") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_40034, "mem_40034") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_40032, "mem_40032") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_40030, "ext_mem_40030") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_40029, "mem_40029") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_40272, "mem_param_tmp_40272") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_40271, "mem_param_tmp_40271") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_40009, "mem_40009") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_40005, "mem_40005") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39971, "mem_39971") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39967, "mem_39967") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39976, "mem_39976") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39974, "mem_39974") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_39991, "ext_mem_39991") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_39994, "ext_mem_39994") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_39896, "mem_param_39896") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_39893, "mem_param_39893") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_40015, "ext_mem_40015") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_40016, "ext_mem_40016") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39889, "mem_39889") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39885, "mem_39885") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_40019, "ext_mem_40019") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39814, "mem_39814") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39812, "mem_39812") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_40228, "mem_param_tmp_40228") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_40227, "mem_param_tmp_40227") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39792, "mem_39792") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39788, "mem_39788") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39754, "mem_39754") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39750, "mem_39750") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39759, "mem_39759") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39757, "mem_39757") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_39774, "ext_mem_39774") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_39777, "ext_mem_39777") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_39679, "mem_param_39679") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_39676, "mem_param_39676") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_39798, "ext_mem_39798") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_39799, "ext_mem_39799") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39672, "mem_39672") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39668, "mem_39668") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_39802, "ext_mem_39802") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_39597, "mem_39597") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_40208, "mem_out_40208") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_40207, "mem_out_40207") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_40206, "mem_out_40206") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_40205, "mem_out_40205") != 0)
            return 1;
    }
    return err;
}
FUTHARK_FUN_ATTR int futrts_greatest_divisor_leq_than_5346(struct futhark_context *ctx, int64_t *out_prim_out_40385, int64_t upper_bound_18796, int64_t n_18797)
{
    (void) ctx;
    
    int err = 0;
    int64_t prim_out_40205;
    bool upper_bound_18798 = sle64((int64_t) 1, upper_bound_18796);
    bool assert_c_18800;
    
    if (!upper_bound_18798) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "Assertion is false: (upper_bound >= 1)", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:189:21-58\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool cond_18809 = slt64(upper_bound_18796, n_18797);
    bool d_18818;
    int64_t d_18819;
    bool loop_while_18820;
    int64_t d_18821;
    
    loop_while_18820 = cond_18809;
    d_18821 = (int64_t) 1;
    while (loop_while_18820) {
        int64_t loopres_18822 = add64((int64_t) 1, d_18821);
        bool zzero_18824 = loopres_18822 == (int64_t) 0;
        bool nonzzero_18825 = !zzero_18824;
        bool nonzzero_cert_18826;
        
        if (!nonzzero_18825) {
            set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:190:36-38\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t zg_lhs_18827 = sdiv64(n_18797, loopres_18822);
        bool cond_18829 = slt64(upper_bound_18796, zg_lhs_18827);
        bool loop_cond_18830;
        
        if (cond_18829) {
            loop_cond_18830 = 1;
        } else {
            int64_t znze_lhs_18834 = smod64(n_18797, loopres_18822);
            bool loop_cond_f_res_18836 = znze_lhs_18834 == (int64_t) 0;
            bool loop_cond_f_res_18837 = !loop_cond_f_res_18836;
            
            loop_cond_18830 = loop_cond_f_res_18837;
        }
        
        bool loop_while_tmp_40206 = loop_cond_18830;
        int64_t d_tmp_40207 = loopres_18822;
        
        loop_while_18820 = loop_while_tmp_40206;
        d_18821 = d_tmp_40207;
    }
    d_18818 = loop_while_18820;
    d_18819 = d_18821;
    
    bool zzero_18839 = d_18819 == (int64_t) 0;
    bool nonzzero_18840 = !zzero_18839;
    bool nonzzero_cert_18841;
    
    if (!nonzzero_18840) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../../lib/github.com/diku-dk/sorts/merge_sort.fut:191:7-9\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t greatest_divisor_leq_than_res_18842 = sdiv64(n_18797, d_18819);
    
    prim_out_40205 = greatest_divisor_leq_than_res_18842;
    *out_prim_out_40385 = prim_out_40205;
    
  cleanup:
    { }
    return err;
}
FUTHARK_FUN_ATTR int futrts_indicesWithIncrement_7869(struct futhark_context *ctx, struct memblock *mem_out_p_40386, struct memblock nameless_mem_39592, int64_t n_19155, int64_t incr_19156)
{
    (void) ctx;
    
    int err = 0;
    struct memblock mem_39594;
    
    mem_39594.references = NULL;
    
    struct memblock mem_out_40205;
    
    mem_out_40205.references = NULL;
    
    int64_t rng_19161 = add64(n_19155, incr_19156);
    bool bounds_invalid_upwards_19165 = slt64(rng_19161, incr_19156);
    bool valid_19170 = !bounds_invalid_upwards_19165;
    bool range_valid_c_19171;
    
    if (!valid_19170) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) incr_19156, "..<", (long long) rng_19161, " is invalid.", "-> #0  ../../../ftbasics.fut:11:25-40\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t bytes_39593 = (int64_t) 8 * n_19155;
    
    if (memblock_alloc(ctx, &mem_39594, bytes_39593, "mem_39594")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t i_40206 = 0; i_40206 < n_19155; i_40206++) {
        int64_t x_40207 = incr_19156 + i_40206 * (int64_t) 1;
        
        ((int64_t *) mem_39594.mem)[i_40206] = x_40207;
    }
    if (memblock_set(ctx, &mem_out_40205, &mem_39594, "mem_39594") != 0)
        return 1;
    if (memblock_set(ctx, &*mem_out_p_40386, &mem_out_40205, "mem_out_40205") != 0)
        return 1;
    
  cleanup:
    {
        if (memblock_unref(ctx, &mem_39594, "mem_39594") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_40205, "mem_out_40205") != 0)
            return 1;
    }
    return err;
}
FUTHARK_FUN_ATTR int futrts_indicesWithIncrement_7982(struct futhark_context *ctx, struct memblock *mem_out_p_40387, struct memblock nameless_mem_39592, int64_t n_25215, int64_t incr_25216)
{
    (void) ctx;
    
    int err = 0;
    struct memblock mem_39594;
    
    mem_39594.references = NULL;
    
    struct memblock mem_out_40205;
    
    mem_out_40205.references = NULL;
    
    int64_t rng_25221 = add64(n_25215, incr_25216);
    bool bounds_invalid_upwards_25225 = slt64(rng_25221, incr_25216);
    bool valid_25230 = !bounds_invalid_upwards_25225;
    bool range_valid_c_25231;
    
    if (!valid_25230) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) incr_25216, "..<", (long long) rng_25221, " is invalid.", "-> #0  ../../../ftbasics.fut:11:25-40\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t bytes_39593 = (int64_t) 8 * n_25215;
    
    if (memblock_alloc(ctx, &mem_39594, bytes_39593, "mem_39594")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t i_40206 = 0; i_40206 < n_25215; i_40206++) {
        int64_t x_40207 = incr_25216 + i_40206 * (int64_t) 1;
        
        ((int64_t *) mem_39594.mem)[i_40206] = x_40207;
    }
    if (memblock_set(ctx, &mem_out_40205, &mem_39594, "mem_39594") != 0)
        return 1;
    if (memblock_set(ctx, &*mem_out_p_40387, &mem_out_40205, "mem_out_40205") != 0)
        return 1;
    
  cleanup:
    {
        if (memblock_unref(ctx, &mem_39594, "mem_39594") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_40205, "mem_out_40205") != 0)
            return 1;
    }
    return err;
}
FUTHARK_FUN_ATTR int futrts_lifted_normalizze_7743(struct futhark_context *ctx, int64_t *out_prim_out_40388, int64_t *out_prim_out_40389, int64_t *out_prim_out_40390, int64_t *out_prim_out_40391, int64_t *out_prim_out_40392, int64_t slo_19029, int64_t shi_19030, int64_t tlo_19031, int64_t thi_19032, int64_t count_19033)
{
    (void) ctx;
    
    int err = 0;
    int64_t prim_out_40205;
    int64_t prim_out_40206;
    int64_t prim_out_40207;
    int64_t prim_out_40208;
    int64_t prim_out_40209;
    int64_t min_arg1_19036 = add64(slo_19029, count_19033);
    int64_t min_res_30978 = smin64(shi_19030, min_arg1_19036);
    int64_t min_arg1_19040 = add64(tlo_19031, count_19033);
    int64_t min_res_30981 = smin64(thi_19032, min_arg1_19040);
    int64_t zp_lhs_19044 = sub64(min_res_30978, slo_19029);
    int64_t zp_rhs_19046 = sub64(min_res_30981, tlo_19031);
    int64_t zm_lhs_19048 = add64(zp_lhs_19044, zp_rhs_19046);
    int64_t slack_19050 = sub64(zm_lhs_19048, count_19033);
    int64_t min_res_30984 = smin64(zp_lhs_19044, slack_19050);
    int64_t min_res_30987 = smin64(zp_rhs_19046, min_res_30984);
    int64_t max_arg1_19060 = sub64(min_res_30978, min_res_30987);
    int64_t max_res_30990 = smax64(slo_19029, max_arg1_19060);
    int64_t max_arg1_19064 = sub64(min_res_30981, min_res_30987);
    int64_t max_res_30993 = smax64(tlo_19031, max_arg1_19064);
    int64_t zm_rhs_19068 = sub64(max_res_30990, slo_19029);
    int64_t zm_lhs_19070 = sub64(count_19033, zm_rhs_19068);
    int64_t zm_rhs_19072 = sub64(max_res_30993, tlo_19031);
    int64_t count_19074 = sub64(zm_lhs_19070, zm_rhs_19072);
    
    prim_out_40205 = max_res_30990;
    prim_out_40206 = min_res_30978;
    prim_out_40207 = max_res_30993;
    prim_out_40208 = min_res_30981;
    prim_out_40209 = count_19074;
    *out_prim_out_40388 = prim_out_40205;
    *out_prim_out_40389 = prim_out_40206;
    *out_prim_out_40390 = prim_out_40207;
    *out_prim_out_40391 = prim_out_40208;
    *out_prim_out_40392 = prim_out_40209;
    
  cleanup:
    { }
    return err;
}

int futhark_entry_smj1_i32(struct futhark_context *ctx, struct futhark_i32_1d **out0, struct futhark_i64_1d **out1, struct futhark_i64_1d **out2, struct futhark_i64_1d **out3, const struct futhark_i32_1d *in0, const struct futhark_u8_2d *in1, const struct futhark_i32_1d *in2, const struct futhark_u8_2d *in3)
{
    int64_t n1_24948 = (int64_t) 0;
    int64_t n2_24949 = (int64_t) 0;
    int ret = 0;
    
    lock_lock(&ctx->lock);
    
    struct memblock mem_out_40208;
    
    mem_out_40208.references = NULL;
    
    struct memblock mem_out_40207;
    
    mem_out_40207.references = NULL;
    
    struct memblock mem_out_40206;
    
    mem_out_40206.references = NULL;
    
    struct memblock mem_out_40205;
    
    mem_out_40205.references = NULL;
    
    struct memblock pL2_mem_39595;
    
    pL2_mem_39595.references = NULL;
    
    struct memblock ks2_mem_39594;
    
    ks2_mem_39594.references = NULL;
    
    struct memblock pL1_mem_39593;
    
    pL1_mem_39593.references = NULL;
    
    struct memblock ks1_mem_39592;
    
    ks1_mem_39592.references = NULL;
    ks1_mem_39592 = in0->mem;
    n1_24948 = in0->shape[0];
    pL1_mem_39593 = in1->mem;
    n1_24948 = in1->shape[0];
    ks2_mem_39594 = in2->mem;
    n2_24949 = in2->shape[0];
    pL2_mem_39595 = in3->mem;
    n2_24949 = in3->shape[0];
    if (!(n1_24948 == in0->shape[0] && ((n1_24948 == in1->shape[0] && (int64_t) 0 == in1->shape[1]) && (n2_24949 == in2->shape[0] && (n2_24949 == in3->shape[0] && (int64_t) 0 == in3->shape[1]))))) {
        ret = 1;
        set_error(ctx, msgprintf("Error: entry point arguments have invalid sizes.\n"));
    }
    if (ret == 0) {
        ret = futrts_entry_smj1_i32(ctx, &mem_out_40205, &mem_out_40206, &mem_out_40207, &mem_out_40208, ks1_mem_39592, pL1_mem_39593, ks2_mem_39594, pL2_mem_39595, n1_24948, n2_24949);
        if (ret == 0) {
            assert((*out0 = (struct futhark_i32_1d *) malloc(sizeof(struct futhark_i32_1d))) != NULL);
            (*out0)->mem = mem_out_40205;
            (*out0)->shape[0] = n1_24948;
            assert((*out1 = (struct futhark_i64_1d *) malloc(sizeof(struct futhark_i64_1d))) != NULL);
            (*out1)->mem = mem_out_40206;
            (*out1)->shape[0] = n1_24948;
            assert((*out2 = (struct futhark_i64_1d *) malloc(sizeof(struct futhark_i64_1d))) != NULL);
            (*out2)->mem = mem_out_40207;
            (*out2)->shape[0] = n1_24948;
            assert((*out3 = (struct futhark_i64_1d *) malloc(sizeof(struct futhark_i64_1d))) != NULL);
            (*out3)->mem = mem_out_40208;
            (*out3)->shape[0] = n1_24948;
        }
    }
    lock_unlock(&ctx->lock);
    return ret;
}
int futhark_entry_smj1_i64(struct futhark_context *ctx, struct futhark_i64_1d **out0, struct futhark_i64_1d **out1, struct futhark_i64_1d **out2, struct futhark_i64_1d **out3, const struct futhark_i64_1d *in0, const struct futhark_u8_2d *in1, const struct futhark_i64_1d *in2, const struct futhark_u8_2d *in3)
{
    int64_t n1_30936 = (int64_t) 0;
    int64_t n2_30937 = (int64_t) 0;
    int ret = 0;
    
    lock_lock(&ctx->lock);
    
    struct memblock mem_out_40208;
    
    mem_out_40208.references = NULL;
    
    struct memblock mem_out_40207;
    
    mem_out_40207.references = NULL;
    
    struct memblock mem_out_40206;
    
    mem_out_40206.references = NULL;
    
    struct memblock mem_out_40205;
    
    mem_out_40205.references = NULL;
    
    struct memblock pL2_mem_39595;
    
    pL2_mem_39595.references = NULL;
    
    struct memblock ks2_mem_39594;
    
    ks2_mem_39594.references = NULL;
    
    struct memblock pL1_mem_39593;
    
    pL1_mem_39593.references = NULL;
    
    struct memblock ks1_mem_39592;
    
    ks1_mem_39592.references = NULL;
    ks1_mem_39592 = in0->mem;
    n1_30936 = in0->shape[0];
    pL1_mem_39593 = in1->mem;
    n1_30936 = in1->shape[0];
    ks2_mem_39594 = in2->mem;
    n2_30937 = in2->shape[0];
    pL2_mem_39595 = in3->mem;
    n2_30937 = in3->shape[0];
    if (!(n1_30936 == in0->shape[0] && ((n1_30936 == in1->shape[0] && (int64_t) 0 == in1->shape[1]) && (n2_30937 == in2->shape[0] && (n2_30937 == in3->shape[0] && (int64_t) 0 == in3->shape[1]))))) {
        ret = 1;
        set_error(ctx, msgprintf("Error: entry point arguments have invalid sizes.\n"));
    }
    if (ret == 0) {
        ret = futrts_entry_smj1_i64(ctx, &mem_out_40205, &mem_out_40206, &mem_out_40207, &mem_out_40208, ks1_mem_39592, pL1_mem_39593, ks2_mem_39594, pL2_mem_39595, n1_30936, n2_30937);
        if (ret == 0) {
            assert((*out0 = (struct futhark_i64_1d *) malloc(sizeof(struct futhark_i64_1d))) != NULL);
            (*out0)->mem = mem_out_40205;
            (*out0)->shape[0] = n1_30936;
            assert((*out1 = (struct futhark_i64_1d *) malloc(sizeof(struct futhark_i64_1d))) != NULL);
            (*out1)->mem = mem_out_40206;
            (*out1)->shape[0] = n1_30936;
            assert((*out2 = (struct futhark_i64_1d *) malloc(sizeof(struct futhark_i64_1d))) != NULL);
            (*out2)->mem = mem_out_40207;
            (*out2)->shape[0] = n1_30936;
            assert((*out3 = (struct futhark_i64_1d *) malloc(sizeof(struct futhark_i64_1d))) != NULL);
            (*out3)->mem = mem_out_40208;
            (*out3)->shape[0] = n1_30936;
        }
    }
    lock_unlock(&ctx->lock);
    return ret;
}
  
