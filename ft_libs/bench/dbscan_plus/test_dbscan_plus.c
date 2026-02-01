
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
struct futhark_f64_1d;
struct futhark_f64_1d *futhark_new_f64_1d(struct futhark_context *ctx, const double *data, int64_t dim0);
struct futhark_f64_1d *futhark_new_raw_f64_1d(struct futhark_context *ctx, unsigned char *data, int64_t dim0);
int futhark_free_f64_1d(struct futhark_context *ctx, struct futhark_f64_1d *arr);
int futhark_values_f64_1d(struct futhark_context *ctx, struct futhark_f64_1d *arr, double *data);
int futhark_index_f64_1d(struct futhark_context *ctx, double *out, struct futhark_f64_1d *arr, int64_t i0);
unsigned char *futhark_values_raw_f64_1d(struct futhark_context *ctx, struct futhark_f64_1d *arr);
const int64_t *futhark_shape_f64_1d(struct futhark_context *ctx, struct futhark_f64_1d *arr);
struct futhark_f64_2d;
struct futhark_f64_2d *futhark_new_f64_2d(struct futhark_context *ctx, const double *data, int64_t dim0, int64_t dim1);
struct futhark_f64_2d *futhark_new_raw_f64_2d(struct futhark_context *ctx, unsigned char *data, int64_t dim0, int64_t dim1);
int futhark_free_f64_2d(struct futhark_context *ctx, struct futhark_f64_2d *arr);
int futhark_values_f64_2d(struct futhark_context *ctx, struct futhark_f64_2d *arr, double *data);
int futhark_index_f64_2d(struct futhark_context *ctx, double *out, struct futhark_f64_2d *arr, int64_t i0, int64_t i1);
unsigned char *futhark_values_raw_f64_2d(struct futhark_context *ctx, struct futhark_f64_2d *arr);
const int64_t *futhark_shape_f64_2d(struct futhark_context *ctx, struct futhark_f64_2d *arr);
struct futhark_i64_1d;
struct futhark_i64_1d *futhark_new_i64_1d(struct futhark_context *ctx, const int64_t *data, int64_t dim0);
struct futhark_i64_1d *futhark_new_raw_i64_1d(struct futhark_context *ctx, unsigned char *data, int64_t dim0);
int futhark_free_i64_1d(struct futhark_context *ctx, struct futhark_i64_1d *arr);
int futhark_values_i64_1d(struct futhark_context *ctx, struct futhark_i64_1d *arr, int64_t *data);
int futhark_index_i64_1d(struct futhark_context *ctx, int64_t *out, struct futhark_i64_1d *arr, int64_t i0);
unsigned char *futhark_values_raw_i64_1d(struct futhark_context *ctx, struct futhark_i64_1d *arr);
const int64_t *futhark_shape_i64_1d(struct futhark_context *ctx, struct futhark_i64_1d *arr);

// Opaque values



// Entry points
int futhark_entry_test_dbscan_plus(struct futhark_context *ctx, struct futhark_f64_1d **out0, struct futhark_f64_1d **out1, struct futhark_i64_1d **out2, const struct futhark_f64_2d *in0);

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

const struct type type_ZMZNZMZNf64;
const struct type type_ZMZNf64;
const struct type type_ZMZNi64;
void *futhark_new_f64_2d_wrap(struct futhark_context *ctx, const void *p, const int64_t *shape)
{
    return futhark_new_f64_2d(ctx, p, shape[0], shape[1]);
}
const struct array_aux type_ZMZNZMZNf64_aux = {.name ="[][]f64", .rank =2, .info =&f64_info, .new =(array_new_fn) futhark_new_f64_2d_wrap, .free =(array_free_fn) futhark_free_f64_2d, .shape =(array_shape_fn) futhark_shape_f64_2d, .values =(array_values_fn) futhark_values_f64_2d};
const struct type type_ZMZNZMZNf64 = {.name ="[][]f64", .restore =(restore_fn) restore_array, .store =(store_fn) store_array, .free =(free_fn) free_array, .aux =&type_ZMZNZMZNf64_aux};
void *futhark_new_f64_1d_wrap(struct futhark_context *ctx, const void *p, const int64_t *shape)
{
    return futhark_new_f64_1d(ctx, p, shape[0]);
}
const struct array_aux type_ZMZNf64_aux = {.name ="[]f64", .rank =1, .info =&f64_info, .new =(array_new_fn) futhark_new_f64_1d_wrap, .free =(array_free_fn) futhark_free_f64_1d, .shape =(array_shape_fn) futhark_shape_f64_1d, .values =(array_values_fn) futhark_values_f64_1d};
const struct type type_ZMZNf64 = {.name ="[]f64", .restore =(restore_fn) restore_array, .store =(store_fn) store_array, .free =(free_fn) free_array, .aux =&type_ZMZNf64_aux};
void *futhark_new_i64_1d_wrap(struct futhark_context *ctx, const void *p, const int64_t *shape)
{
    return futhark_new_i64_1d(ctx, p, shape[0]);
}
const struct array_aux type_ZMZNi64_aux = {.name ="[]i64", .rank =1, .info =&i64_info, .new =(array_new_fn) futhark_new_i64_1d_wrap, .free =(array_free_fn) futhark_free_i64_1d, .shape =(array_shape_fn) futhark_shape_i64_1d, .values =(array_values_fn) futhark_values_i64_1d};
const struct type type_ZMZNi64 = {.name ="[]i64", .restore =(restore_fn) restore_array, .store =(store_fn) store_array, .free =(free_fn) free_array, .aux =&type_ZMZNi64_aux};
const struct type *test_dbscan_plus_out_types[] = {&type_ZMZNf64, &type_ZMZNf64, &type_ZMZNi64, NULL};
bool test_dbscan_plus_out_unique[] = {false, false, false};
const struct type *test_dbscan_plus_in_types[] = {&type_ZMZNZMZNf64, NULL};
bool test_dbscan_plus_in_unique[] = {false};
const char *test_dbscan_plus_tuning_params[] = {NULL};
int call_test_dbscan_plus(struct futhark_context *ctx, void **outs, void **ins)
{
    struct futhark_f64_1d * *out0 = outs[0];
    struct futhark_f64_1d * *out1 = outs[1];
    struct futhark_i64_1d * *out2 = outs[2];
    struct futhark_f64_2d * in0 = *(struct futhark_f64_2d * *) ins[0];
    
    return futhark_entry_test_dbscan_plus(ctx, out0, out1, out2, in0);
}
const struct type *types[] = {&type_i8, &type_i16, &type_i32, &type_i64, &type_u8, &type_u16, &type_u32, &type_u64, &type_f16, &type_f32, &type_f64, &type_bool, &type_ZMZNZMZNf64, &type_ZMZNf64, &type_ZMZNi64, NULL};
struct entry_point entry_points[] = {{.name ="test_dbscan_plus", .f =call_test_dbscan_plus, .tuning_params =test_dbscan_plus_tuning_params, .in_types =test_dbscan_plus_in_types, .out_types =test_dbscan_plus_out_types, .in_unique =test_dbscan_plus_in_unique, .out_unique =test_dbscan_plus_out_unique}, {.name =NULL}};
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

FUTHARK_FUN_ATTR int futrts_entry_test_dbscan_plus(struct futhark_context *ctx, struct memblock *mem_out_p_84812, struct memblock *mem_out_p_84813, struct memblock *mem_out_p_84814, struct memblock pts_mem_82613, int64_t dz2081U_57317);
FUTHARK_FUN_ATTR int futrts_get_num_neighbours_against_12423(struct futhark_context *ctx, struct memblock *mem_out_p_84960, struct memblock dat1_mem_82613, struct memblock dat1_mem_82614, struct memblock dat2_mem_82615, struct memblock dat2_mem_82616, int64_t n1_40266, int64_t n2_40267, double eps_40272, int8_t dist_t_40273, int8_t angle_t_40274, double radius_40275, int64_t m_sizze_40276);
FUTHARK_FUN_ATTR int futrts_get_num_neighbours_against_12426(struct futhark_context *ctx, struct memblock *mem_out_p_84961, struct memblock dat1_mem_82613, struct memblock dat1_mem_82614, struct memblock dat2_mem_82615, struct memblock dat2_mem_82616, int64_t n1_40529, int64_t n2_40530, double eps_40535, int8_t dist_t_40536, int8_t angle_t_40537, double radius_40538, int64_t m_sizze_40539);
FUTHARK_FUN_ATTR int futrts_greatest_divisor_leq_than_5592(struct futhark_context *ctx, int64_t *out_prim_out_84962, int64_t upper_bound_33645, int64_t n_33646);
FUTHARK_FUN_ATTR int futrts_lifted_get_grid_id_12077(struct futhark_context *ctx, int64_t *out_prim_out_84963, double subdv_x_33978, double step_x_33979, double step_y_33980, double x_33981, double y_33982);
FUTHARK_FUN_ATTR int futrts_lifted_lambda_12071(struct futhark_context *ctx, bool *out_prim_out_84964, int64_t c1_51373, int64_t p1_51374, int64_t c2_51375, int64_t p2_51376);
FUTHARK_FUN_ATTR int futrts_lifted_normalizze_11972(struct futhark_context *ctx, int64_t *out_prim_out_84965, int64_t *out_prim_out_84966, int64_t *out_prim_out_84967, int64_t *out_prim_out_84968, int64_t *out_prim_out_84969, int64_t slo_33878, int64_t shi_33879, int64_t tlo_33880, int64_t thi_33881, int64_t count_33882);

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
struct futhark_f64_1d {
    struct memblock mem;
    int64_t shape[1];
};
struct futhark_f64_1d *futhark_new_f64_1d(struct futhark_context *ctx, const double *data, int64_t dim0)
{
    int err = 0;
    struct futhark_f64_1d *bad = NULL;
    struct futhark_f64_1d *arr = (struct futhark_f64_1d *) malloc(sizeof(struct futhark_f64_1d));
    
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
struct futhark_f64_1d *futhark_new_raw_f64_1d(struct futhark_context *ctx, unsigned char *data, int64_t dim0)
{
    int err = 0;
    struct futhark_f64_1d *bad = NULL;
    struct futhark_f64_1d *arr = (struct futhark_f64_1d *) malloc(sizeof(struct futhark_f64_1d));
    
    if (arr == NULL)
        return bad;
    lock_lock(&ctx->lock);
    arr->mem.references = NULL;
    arr->mem.mem = data;
    arr->shape[0] = dim0;
    lock_unlock(&ctx->lock);
    return arr;
}
int futhark_free_f64_1d(struct futhark_context *ctx, struct futhark_f64_1d *arr)
{
    lock_lock(&ctx->lock);
    if (memblock_unref(ctx, &arr->mem, "arr->mem") != 0)
        return 1;
    lock_unlock(&ctx->lock);
    free(arr);
    return 0;
}
int futhark_values_f64_1d(struct futhark_context *ctx, struct futhark_f64_1d *arr, double *data)
{
    int err = 0;
    
    lock_lock(&ctx->lock);
    if ((size_t) arr->shape[0] * 8 > 0)
        memmove((unsigned char *) data + 0, arr->mem.mem + 0, (size_t) arr->shape[0] * 8);
    lock_unlock(&ctx->lock);
    return err;
}
int futhark_index_f64_1d(struct futhark_context *ctx, double *out, struct futhark_f64_1d *arr, int64_t i0)
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
unsigned char *futhark_values_raw_f64_1d(struct futhark_context *ctx, struct futhark_f64_1d *arr)
{
    (void) ctx;
    return arr->mem.mem;
}
const int64_t *futhark_shape_f64_1d(struct futhark_context *ctx, struct futhark_f64_1d *arr)
{
    (void) ctx;
    return arr->shape;
}
struct futhark_f64_2d {
    struct memblock mem;
    int64_t shape[2];
};
struct futhark_f64_2d *futhark_new_f64_2d(struct futhark_context *ctx, const double *data, int64_t dim0, int64_t dim1)
{
    int err = 0;
    struct futhark_f64_2d *bad = NULL;
    struct futhark_f64_2d *arr = (struct futhark_f64_2d *) malloc(sizeof(struct futhark_f64_2d));
    
    if (arr == NULL)
        return bad;
    lock_lock(&ctx->lock);
    arr->mem.references = NULL;
    if (memblock_alloc(ctx, &arr->mem, dim0 * dim1 * 8, "arr->mem"))
        err = 1;
    arr->shape[0] = dim0;
    arr->shape[1] = dim1;
    if ((size_t) (dim0 * dim1) * 8 > 0)
        memmove(arr->mem.mem + 0, (const unsigned char *) data + 0, (size_t) (dim0 * dim1) * 8);
    lock_unlock(&ctx->lock);
    if (err != 0) {
        free(arr);
        return bad;
    }
    return arr;
}
struct futhark_f64_2d *futhark_new_raw_f64_2d(struct futhark_context *ctx, unsigned char *data, int64_t dim0, int64_t dim1)
{
    int err = 0;
    struct futhark_f64_2d *bad = NULL;
    struct futhark_f64_2d *arr = (struct futhark_f64_2d *) malloc(sizeof(struct futhark_f64_2d));
    
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
int futhark_free_f64_2d(struct futhark_context *ctx, struct futhark_f64_2d *arr)
{
    lock_lock(&ctx->lock);
    if (memblock_unref(ctx, &arr->mem, "arr->mem") != 0)
        return 1;
    lock_unlock(&ctx->lock);
    free(arr);
    return 0;
}
int futhark_values_f64_2d(struct futhark_context *ctx, struct futhark_f64_2d *arr, double *data)
{
    int err = 0;
    
    lock_lock(&ctx->lock);
    if ((size_t) (arr->shape[0] * arr->shape[1]) * 8 > 0)
        memmove((unsigned char *) data + 0, arr->mem.mem + 0, (size_t) (arr->shape[0] * arr->shape[1]) * 8);
    lock_unlock(&ctx->lock);
    return err;
}
int futhark_index_f64_2d(struct futhark_context *ctx, double *out, struct futhark_f64_2d *arr, int64_t i0, int64_t i1)
{
    int err = 0;
    
    if ((i0 >= 0 && i0 < arr->shape[0]) && (i1 >= 0 && i1 < arr->shape[1])) {
        lock_lock(&ctx->lock);
        if (8 > 0)
            memmove((unsigned char *) out + 0, arr->mem.mem + 8 * (i0 * arr->shape[1] + i1 * 1), 8);
        lock_unlock(&ctx->lock);
    } else {
        err = 1;
        set_error(ctx, strdup("Index out of bounds."));
    }
    return err;
}
unsigned char *futhark_values_raw_f64_2d(struct futhark_context *ctx, struct futhark_f64_2d *arr)
{
    (void) ctx;
    return arr->mem.mem;
}
const int64_t *futhark_shape_f64_2d(struct futhark_context *ctx, struct futhark_f64_2d *arr)
{
    (void) ctx;
    return arr->shape;
}

FUTHARK_FUN_ATTR int futrts_entry_test_dbscan_plus(struct futhark_context *ctx, struct memblock *mem_out_p_84812, struct memblock *mem_out_p_84813, struct memblock *mem_out_p_84814, struct memblock pts_mem_82613, int64_t dz2081U_57317)
{
    (void) ctx;
    
    int err = 0;
    int64_t mem_82615_cached_sizze_84815 = 0;
    unsigned char *mem_82615 = NULL;
    int64_t mem_82626_cached_sizze_84816 = 0;
    unsigned char *mem_82626 = NULL;
    int64_t mem_82629_cached_sizze_84817 = 0;
    unsigned char *mem_82629 = NULL;
    int64_t mem_82639_cached_sizze_84818 = 0;
    unsigned char *mem_82639 = NULL;
    int64_t mem_82641_cached_sizze_84819 = 0;
    unsigned char *mem_82641 = NULL;
    int64_t mem_82708_cached_sizze_84820 = 0;
    unsigned char *mem_82708 = NULL;
    int64_t mem_82711_cached_sizze_84821 = 0;
    unsigned char *mem_82711 = NULL;
    int64_t mem_82721_cached_sizze_84822 = 0;
    unsigned char *mem_82721 = NULL;
    int64_t mem_82723_cached_sizze_84823 = 0;
    unsigned char *mem_82723 = NULL;
    int64_t mem_82738_cached_sizze_84824 = 0;
    unsigned char *mem_82738 = NULL;
    int64_t mem_82741_cached_sizze_84825 = 0;
    unsigned char *mem_82741 = NULL;
    int64_t mem_82830_cached_sizze_84826 = 0;
    unsigned char *mem_82830 = NULL;
    int64_t mem_82832_cached_sizze_84827 = 0;
    unsigned char *mem_82832 = NULL;
    int64_t mem_82845_cached_sizze_84828 = 0;
    unsigned char *mem_82845 = NULL;
    int64_t mem_82922_cached_sizze_84829 = 0;
    unsigned char *mem_82922 = NULL;
    int64_t mem_82928_cached_sizze_84830 = 0;
    unsigned char *mem_82928 = NULL;
    int64_t mem_82929_cached_sizze_84831 = 0;
    unsigned char *mem_82929 = NULL;
    int64_t mem_82930_cached_sizze_84832 = 0;
    unsigned char *mem_82930 = NULL;
    int64_t mem_82950_cached_sizze_84833 = 0;
    unsigned char *mem_82950 = NULL;
    int64_t mem_82958_cached_sizze_84834 = 0;
    unsigned char *mem_82958 = NULL;
    int64_t mem_82960_cached_sizze_84835 = 0;
    unsigned char *mem_82960 = NULL;
    int64_t mem_82962_cached_sizze_84836 = 0;
    unsigned char *mem_82962 = NULL;
    int64_t mem_82984_cached_sizze_84837 = 0;
    unsigned char *mem_82984 = NULL;
    int64_t mem_82986_cached_sizze_84838 = 0;
    unsigned char *mem_82986 = NULL;
    int64_t mem_83007_cached_sizze_84839 = 0;
    unsigned char *mem_83007 = NULL;
    int64_t mem_83009_cached_sizze_84840 = 0;
    unsigned char *mem_83009 = NULL;
    int64_t mem_83011_cached_sizze_84841 = 0;
    unsigned char *mem_83011 = NULL;
    int64_t mem_83031_cached_sizze_84842 = 0;
    unsigned char *mem_83031 = NULL;
    int64_t mem_83033_cached_sizze_84843 = 0;
    unsigned char *mem_83033 = NULL;
    int64_t mem_83034_cached_sizze_84844 = 0;
    unsigned char *mem_83034 = NULL;
    int64_t mem_83036_cached_sizze_84845 = 0;
    unsigned char *mem_83036 = NULL;
    int64_t mem_83062_cached_sizze_84846 = 0;
    unsigned char *mem_83062 = NULL;
    int64_t mem_83064_cached_sizze_84847 = 0;
    unsigned char *mem_83064 = NULL;
    int64_t mem_83066_cached_sizze_84848 = 0;
    unsigned char *mem_83066 = NULL;
    int64_t mem_83086_cached_sizze_84849 = 0;
    unsigned char *mem_83086 = NULL;
    int64_t mem_83094_cached_sizze_84850 = 0;
    unsigned char *mem_83094 = NULL;
    int64_t mem_83096_cached_sizze_84851 = 0;
    unsigned char *mem_83096 = NULL;
    int64_t mem_83125_cached_sizze_84852 = 0;
    unsigned char *mem_83125 = NULL;
    int64_t mem_83127_cached_sizze_84853 = 0;
    unsigned char *mem_83127 = NULL;
    int64_t mem_83129_cached_sizze_84854 = 0;
    unsigned char *mem_83129 = NULL;
    int64_t mem_83153_cached_sizze_84855 = 0;
    unsigned char *mem_83153 = NULL;
    int64_t mem_83155_cached_sizze_84856 = 0;
    unsigned char *mem_83155 = NULL;
    int64_t mem_83157_cached_sizze_84857 = 0;
    unsigned char *mem_83157 = NULL;
    int64_t mem_83177_cached_sizze_84858 = 0;
    unsigned char *mem_83177 = NULL;
    int64_t mem_83179_cached_sizze_84859 = 0;
    unsigned char *mem_83179 = NULL;
    int64_t mem_83181_cached_sizze_84860 = 0;
    unsigned char *mem_83181 = NULL;
    int64_t mem_83183_cached_sizze_84861 = 0;
    unsigned char *mem_83183 = NULL;
    int64_t mem_83184_cached_sizze_84862 = 0;
    unsigned char *mem_83184 = NULL;
    int64_t mem_83186_cached_sizze_84863 = 0;
    unsigned char *mem_83186 = NULL;
    int64_t mem_83224_cached_sizze_84864 = 0;
    unsigned char *mem_83224 = NULL;
    int64_t mem_83226_cached_sizze_84865 = 0;
    unsigned char *mem_83226 = NULL;
    int64_t mem_83227_cached_sizze_84866 = 0;
    unsigned char *mem_83227 = NULL;
    int64_t mem_83229_cached_sizze_84867 = 0;
    unsigned char *mem_83229 = NULL;
    int64_t mem_83231_cached_sizze_84868 = 0;
    unsigned char *mem_83231 = NULL;
    int64_t mem_83263_cached_sizze_84869 = 0;
    unsigned char *mem_83263 = NULL;
    int64_t mem_83265_cached_sizze_84870 = 0;
    unsigned char *mem_83265 = NULL;
    int64_t mem_83267_cached_sizze_84871 = 0;
    unsigned char *mem_83267 = NULL;
    int64_t mem_83287_cached_sizze_84872 = 0;
    unsigned char *mem_83287 = NULL;
    int64_t mem_83289_cached_sizze_84873 = 0;
    unsigned char *mem_83289 = NULL;
    int64_t mem_83290_cached_sizze_84874 = 0;
    unsigned char *mem_83290 = NULL;
    int64_t mem_83292_cached_sizze_84875 = 0;
    unsigned char *mem_83292 = NULL;
    int64_t mem_83294_cached_sizze_84876 = 0;
    unsigned char *mem_83294 = NULL;
    int64_t mem_83365_cached_sizze_84877 = 0;
    unsigned char *mem_83365 = NULL;
    int64_t mem_83367_cached_sizze_84878 = 0;
    unsigned char *mem_83367 = NULL;
    int64_t mem_83368_cached_sizze_84879 = 0;
    unsigned char *mem_83368 = NULL;
    int64_t mem_83388_cached_sizze_84880 = 0;
    unsigned char *mem_83388 = NULL;
    int64_t mem_83390_cached_sizze_84881 = 0;
    unsigned char *mem_83390 = NULL;
    int64_t mem_83392_cached_sizze_84882 = 0;
    unsigned char *mem_83392 = NULL;
    int64_t mem_83412_cached_sizze_84883 = 0;
    unsigned char *mem_83412 = NULL;
    int64_t mem_83414_cached_sizze_84884 = 0;
    unsigned char *mem_83414 = NULL;
    int64_t mem_83428_cached_sizze_84885 = 0;
    unsigned char *mem_83428 = NULL;
    int64_t mem_83430_cached_sizze_84886 = 0;
    unsigned char *mem_83430 = NULL;
    int64_t mem_83432_cached_sizze_84887 = 0;
    unsigned char *mem_83432 = NULL;
    int64_t mem_83453_cached_sizze_84888 = 0;
    unsigned char *mem_83453 = NULL;
    int64_t mem_83474_cached_sizze_84889 = 0;
    unsigned char *mem_83474 = NULL;
    int64_t mem_83476_cached_sizze_84890 = 0;
    unsigned char *mem_83476 = NULL;
    int64_t mem_83497_cached_sizze_84891 = 0;
    unsigned char *mem_83497 = NULL;
    int64_t mem_83499_cached_sizze_84892 = 0;
    unsigned char *mem_83499 = NULL;
    int64_t mem_83501_cached_sizze_84893 = 0;
    unsigned char *mem_83501 = NULL;
    int64_t mem_83521_cached_sizze_84894 = 0;
    unsigned char *mem_83521 = NULL;
    int64_t mem_83523_cached_sizze_84895 = 0;
    unsigned char *mem_83523 = NULL;
    int64_t mem_83525_cached_sizze_84896 = 0;
    unsigned char *mem_83525 = NULL;
    int64_t mem_83534_cached_sizze_84897 = 0;
    unsigned char *mem_83534 = NULL;
    int64_t mem_83536_cached_sizze_84898 = 0;
    unsigned char *mem_83536 = NULL;
    int64_t mem_83564_cached_sizze_84899 = 0;
    unsigned char *mem_83564 = NULL;
    int64_t mem_83567_cached_sizze_84900 = 0;
    unsigned char *mem_83567 = NULL;
    int64_t mem_83570_cached_sizze_84901 = 0;
    unsigned char *mem_83570 = NULL;
    int64_t mem_83572_cached_sizze_84902 = 0;
    unsigned char *mem_83572 = NULL;
    int64_t mem_83588_cached_sizze_84903 = 0;
    unsigned char *mem_83588 = NULL;
    int64_t mem_83618_cached_sizze_84904 = 0;
    unsigned char *mem_83618 = NULL;
    int64_t mem_83625_cached_sizze_84905 = 0;
    unsigned char *mem_83625 = NULL;
    int64_t mem_83636_cached_sizze_84906 = 0;
    unsigned char *mem_83636 = NULL;
    int64_t mem_83644_cached_sizze_84907 = 0;
    unsigned char *mem_83644 = NULL;
    int64_t mem_83645_cached_sizze_84908 = 0;
    unsigned char *mem_83645 = NULL;
    int64_t mem_83659_cached_sizze_84909 = 0;
    unsigned char *mem_83659 = NULL;
    int64_t mem_83678_cached_sizze_84910 = 0;
    unsigned char *mem_83678 = NULL;
    int64_t mem_83686_cached_sizze_84911 = 0;
    unsigned char *mem_83686 = NULL;
    int64_t mem_83687_cached_sizze_84912 = 0;
    unsigned char *mem_83687 = NULL;
    int64_t mem_83701_cached_sizze_84913 = 0;
    unsigned char *mem_83701 = NULL;
    int64_t mem_83709_cached_sizze_84914 = 0;
    unsigned char *mem_83709 = NULL;
    int64_t mem_83717_cached_sizze_84915 = 0;
    unsigned char *mem_83717 = NULL;
    int64_t mem_83725_cached_sizze_84916 = 0;
    unsigned char *mem_83725 = NULL;
    int64_t mem_83733_cached_sizze_84917 = 0;
    unsigned char *mem_83733 = NULL;
    int64_t mem_83735_cached_sizze_84918 = 0;
    unsigned char *mem_83735 = NULL;
    int64_t mem_83749_cached_sizze_84919 = 0;
    unsigned char *mem_83749 = NULL;
    int64_t mem_83751_cached_sizze_84920 = 0;
    unsigned char *mem_83751 = NULL;
    int64_t mem_83765_cached_sizze_84921 = 0;
    unsigned char *mem_83765 = NULL;
    int64_t mem_83767_cached_sizze_84922 = 0;
    unsigned char *mem_83767 = NULL;
    int64_t mem_83781_cached_sizze_84923 = 0;
    unsigned char *mem_83781 = NULL;
    int64_t mem_83783_cached_sizze_84924 = 0;
    unsigned char *mem_83783 = NULL;
    int64_t mem_83797_cached_sizze_84925 = 0;
    unsigned char *mem_83797 = NULL;
    int64_t mem_83799_cached_sizze_84926 = 0;
    unsigned char *mem_83799 = NULL;
    int64_t mem_83813_cached_sizze_84927 = 0;
    unsigned char *mem_83813 = NULL;
    int64_t mem_83815_cached_sizze_84928 = 0;
    unsigned char *mem_83815 = NULL;
    int64_t mem_83817_cached_sizze_84929 = 0;
    unsigned char *mem_83817 = NULL;
    int64_t mem_83837_cached_sizze_84930 = 0;
    unsigned char *mem_83837 = NULL;
    int64_t mem_83839_cached_sizze_84931 = 0;
    unsigned char *mem_83839 = NULL;
    int64_t mem_83853_cached_sizze_84932 = 0;
    unsigned char *mem_83853 = NULL;
    int64_t mem_83855_cached_sizze_84933 = 0;
    unsigned char *mem_83855 = NULL;
    int64_t mem_83869_cached_sizze_84934 = 0;
    unsigned char *mem_83869 = NULL;
    int64_t mem_83871_cached_sizze_84935 = 0;
    unsigned char *mem_83871 = NULL;
    int64_t mem_83885_cached_sizze_84936 = 0;
    unsigned char *mem_83885 = NULL;
    int64_t mem_83887_cached_sizze_84937 = 0;
    unsigned char *mem_83887 = NULL;
    int64_t mem_83889_cached_sizze_84938 = 0;
    unsigned char *mem_83889 = NULL;
    int64_t mem_83891_cached_sizze_84939 = 0;
    unsigned char *mem_83891 = NULL;
    int64_t mem_83893_cached_sizze_84940 = 0;
    unsigned char *mem_83893 = NULL;
    int64_t mem_83925_cached_sizze_84941 = 0;
    unsigned char *mem_83925 = NULL;
    int64_t mem_83927_cached_sizze_84942 = 0;
    unsigned char *mem_83927 = NULL;
    int64_t mem_83929_cached_sizze_84943 = 0;
    unsigned char *mem_83929 = NULL;
    int64_t mem_83949_cached_sizze_84944 = 0;
    unsigned char *mem_83949 = NULL;
    int64_t mem_83996_cached_sizze_84945 = 0;
    unsigned char *mem_83996 = NULL;
    int64_t mem_83997_cached_sizze_84946 = 0;
    unsigned char *mem_83997 = NULL;
    int64_t mem_84011_cached_sizze_84947 = 0;
    unsigned char *mem_84011 = NULL;
    int64_t mem_84035_cached_sizze_84948 = 0;
    unsigned char *mem_84035 = NULL;
    int64_t mem_84038_cached_sizze_84949 = 0;
    unsigned char *mem_84038 = NULL;
    int64_t mem_84048_cached_sizze_84950 = 0;
    unsigned char *mem_84048 = NULL;
    int64_t mem_84050_cached_sizze_84951 = 0;
    unsigned char *mem_84050 = NULL;
    int64_t mem_84117_cached_sizze_84952 = 0;
    unsigned char *mem_84117 = NULL;
    int64_t mem_84120_cached_sizze_84953 = 0;
    unsigned char *mem_84120 = NULL;
    int64_t mem_84130_cached_sizze_84954 = 0;
    unsigned char *mem_84130 = NULL;
    int64_t mem_84132_cached_sizze_84955 = 0;
    unsigned char *mem_84132 = NULL;
    int64_t mem_84147_cached_sizze_84956 = 0;
    unsigned char *mem_84147 = NULL;
    int64_t mem_84150_cached_sizze_84957 = 0;
    unsigned char *mem_84150 = NULL;
    int64_t mem_84242_cached_sizze_84958 = 0;
    unsigned char *mem_84242 = NULL;
    int64_t mem_84244_cached_sizze_84959 = 0;
    unsigned char *mem_84244 = NULL;
    struct memblock mem_84369;
    
    mem_84369.references = NULL;
    
    struct memblock mem_84367;
    
    mem_84367.references = NULL;
    
    struct memblock mem_84356;
    
    mem_84356.references = NULL;
    
    struct memblock mem_param_tmp_84807;
    
    mem_param_tmp_84807.references = NULL;
    
    struct memblock mem_84345;
    
    mem_84345.references = NULL;
    
    struct memblock mem_param_84343;
    
    mem_param_84343.references = NULL;
    
    struct memblock ext_mem_84354;
    
    ext_mem_84354.references = NULL;
    
    struct memblock mem_84340;
    
    mem_84340.references = NULL;
    
    struct memblock ext_mem_84365;
    
    ext_mem_84365.references = NULL;
    
    struct memblock mem_param_tmp_84510;
    
    mem_param_tmp_84510.references = NULL;
    
    struct memblock mem_param_tmp_84509;
    
    mem_param_tmp_84509.references = NULL;
    
    struct memblock mem_param_tmp_84508;
    
    mem_param_tmp_84508.references = NULL;
    
    struct memblock mem_param_tmp_84507;
    
    mem_param_tmp_84507.references = NULL;
    
    struct memblock mem_param_tmp_84506;
    
    mem_param_tmp_84506.references = NULL;
    
    struct memblock mem_param_tmp_84505;
    
    mem_param_tmp_84505.references = NULL;
    
    struct memblock mem_param_tmp_84504;
    
    mem_param_tmp_84504.references = NULL;
    
    struct memblock mem_param_tmp_84503;
    
    mem_param_tmp_84503.references = NULL;
    
    struct memblock mem_param_tmp_84502;
    
    mem_param_tmp_84502.references = NULL;
    
    struct memblock mem_param_tmp_84501;
    
    mem_param_tmp_84501.references = NULL;
    
    struct memblock mem_param_tmp_84500;
    
    mem_param_tmp_84500.references = NULL;
    
    struct memblock mem_param_tmp_84499;
    
    mem_param_tmp_84499.references = NULL;
    
    struct memblock mem_param_tmp_84498;
    
    mem_param_tmp_84498.references = NULL;
    
    struct memblock mem_param_tmp_84497;
    
    mem_param_tmp_84497.references = NULL;
    
    struct memblock mem_param_tmp_84496;
    
    mem_param_tmp_84496.references = NULL;
    
    struct memblock mem_param_tmp_84495;
    
    mem_param_tmp_84495.references = NULL;
    
    struct memblock mem_param_tmp_84494;
    
    mem_param_tmp_84494.references = NULL;
    
    struct memblock mem_param_tmp_84493;
    
    mem_param_tmp_84493.references = NULL;
    
    struct memblock mem_84284;
    
    mem_84284.references = NULL;
    
    struct memblock mem_84282;
    
    mem_84282.references = NULL;
    
    struct memblock mem_84280;
    
    mem_84280.references = NULL;
    
    struct memblock mem_84260;
    
    mem_84260.references = NULL;
    
    struct memblock mem_84258;
    
    mem_84258.references = NULL;
    
    struct memblock mem_param_tmp_84773;
    
    mem_param_tmp_84773.references = NULL;
    
    struct memblock mem_param_tmp_84772;
    
    mem_param_tmp_84772.references = NULL;
    
    struct memblock mem_84227;
    
    mem_84227.references = NULL;
    
    struct memblock mem_84223;
    
    mem_84223.references = NULL;
    
    struct memblock mem_84189;
    
    mem_84189.references = NULL;
    
    struct memblock mem_84185;
    
    mem_84185.references = NULL;
    
    struct memblock mem_84194;
    
    mem_84194.references = NULL;
    
    struct memblock mem_84192;
    
    mem_84192.references = NULL;
    
    struct memblock ext_mem_84209;
    
    ext_mem_84209.references = NULL;
    
    struct memblock ext_mem_84212;
    
    ext_mem_84212.references = NULL;
    
    struct memblock mem_param_84114;
    
    mem_param_84114.references = NULL;
    
    struct memblock mem_param_84111;
    
    mem_param_84111.references = NULL;
    
    struct memblock ext_mem_84233;
    
    ext_mem_84233.references = NULL;
    
    struct memblock ext_mem_84234;
    
    ext_mem_84234.references = NULL;
    
    struct memblock mem_84107;
    
    mem_84107.references = NULL;
    
    struct memblock mem_84103;
    
    mem_84103.references = NULL;
    
    struct memblock ext_mem_84237;
    
    ext_mem_84237.references = NULL;
    
    struct memblock ext_mem_84240;
    
    ext_mem_84240.references = NULL;
    
    struct memblock ext_mem_84275;
    
    ext_mem_84275.references = NULL;
    
    struct memblock ext_mem_84278;
    
    ext_mem_84278.references = NULL;
    
    struct memblock mem_84029;
    
    mem_84029.references = NULL;
    
    struct memblock mem_84027;
    
    mem_84027.references = NULL;
    
    struct memblock mem_84025;
    
    mem_84025.references = NULL;
    
    struct memblock ext_mem_84030;
    
    ext_mem_84030.references = NULL;
    
    struct memblock ext_mem_84031;
    
    ext_mem_84031.references = NULL;
    
    struct memblock ext_mem_84032;
    
    ext_mem_84032.references = NULL;
    
    struct memblock mem_84023;
    
    mem_84023.references = NULL;
    
    struct memblock mem_84021;
    
    mem_84021.references = NULL;
    
    struct memblock mem_84019;
    
    mem_84019.references = NULL;
    
    struct memblock mem_83995;
    
    mem_83995.references = NULL;
    
    struct memblock mem_83993;
    
    mem_83993.references = NULL;
    
    struct memblock mem_83965;
    
    mem_83965.references = NULL;
    
    struct memblock mem_83151;
    
    mem_83151.references = NULL;
    
    struct memblock mem_83149;
    
    mem_83149.references = NULL;
    
    struct memblock mem_param_tmp_84663;
    
    mem_param_tmp_84663.references = NULL;
    
    struct memblock mem_83667;
    
    mem_83667.references = NULL;
    
    struct memblock mem_param_83634;
    
    mem_param_83634.references = NULL;
    
    struct memblock ext_mem_83676;
    
    ext_mem_83676.references = NULL;
    
    struct memblock mem_param_tmp_84641;
    
    mem_param_tmp_84641.references = NULL;
    
    struct memblock mem_83610;
    
    mem_83610.references = NULL;
    
    struct memblock mem_param_83531;
    
    mem_param_83531.references = NULL;
    
    struct memblock ext_mem_83615;
    
    ext_mem_83615.references = NULL;
    
    struct memblock mem_83477;
    
    mem_83477.references = NULL;
    
    struct memblock ext_mem_83466;
    
    ext_mem_83466.references = NULL;
    
    struct memblock mem_83464;
    
    mem_83464.references = NULL;
    
    struct memblock mem_83461;
    
    mem_83461.references = NULL;
    
    struct memblock mem_83458;
    
    mem_83458.references = NULL;
    
    struct memblock mem_83455;
    
    mem_83455.references = NULL;
    
    struct memblock ext_mem_83451;
    
    ext_mem_83451.references = NULL;
    
    struct memblock ext_mem_83363;
    
    ext_mem_83363.references = NULL;
    
    struct memblock mem_83361;
    
    mem_83361.references = NULL;
    
    struct memblock mem_83358;
    
    mem_83358.references = NULL;
    
    struct memblock mem_83355;
    
    mem_83355.references = NULL;
    
    struct memblock mem_83352;
    
    mem_83352.references = NULL;
    
    struct memblock ext_mem_83350;
    
    ext_mem_83350.references = NULL;
    
    struct memblock mem_83348;
    
    mem_83348.references = NULL;
    
    struct memblock mem_83345;
    
    mem_83345.references = NULL;
    
    struct memblock mem_83342;
    
    mem_83342.references = NULL;
    
    struct memblock mem_83339;
    
    mem_83339.references = NULL;
    
    struct memblock ext_mem_83337;
    
    ext_mem_83337.references = NULL;
    
    struct memblock mem_83335;
    
    mem_83335.references = NULL;
    
    struct memblock mem_83332;
    
    mem_83332.references = NULL;
    
    struct memblock mem_83329;
    
    mem_83329.references = NULL;
    
    struct memblock mem_83326;
    
    mem_83326.references = NULL;
    
    struct memblock mem_83099;
    
    mem_83099.references = NULL;
    
    struct memblock mem_83097;
    
    mem_83097.references = NULL;
    
    struct memblock mem_82987;
    
    mem_82987.references = NULL;
    
    struct memblock mem_82976;
    
    mem_82976.references = NULL;
    
    struct memblock mem_82927;
    
    mem_82927.references = NULL;
    
    struct memblock mem_82925;
    
    mem_82925.references = NULL;
    
    struct memblock mem_82923;
    
    mem_82923.references = NULL;
    
    struct memblock mem_82917;
    
    mem_82917.references = NULL;
    
    struct memblock mem_82915;
    
    mem_82915.references = NULL;
    
    struct memblock mem_82919;
    
    mem_82919.references = NULL;
    
    struct memblock ext_mem_82920;
    
    ext_mem_82920.references = NULL;
    
    struct memblock ext_mem_82921;
    
    ext_mem_82921.references = NULL;
    
    struct memblock mem_param_82913;
    
    mem_param_82913.references = NULL;
    
    struct memblock mem_param_82910;
    
    mem_param_82910.references = NULL;
    
    struct memblock mem_param_82907;
    
    mem_param_82907.references = NULL;
    
    struct memblock mem_param_82904;
    
    mem_param_82904.references = NULL;
    
    struct memblock mem_param_82901;
    
    mem_param_82901.references = NULL;
    
    struct memblock mem_param_82898;
    
    mem_param_82898.references = NULL;
    
    struct memblock mem_param_82895;
    
    mem_param_82895.references = NULL;
    
    struct memblock mem_param_82892;
    
    mem_param_82892.references = NULL;
    
    struct memblock mem_param_82889;
    
    mem_param_82889.references = NULL;
    
    struct memblock mem_param_82886;
    
    mem_param_82886.references = NULL;
    
    struct memblock mem_param_82883;
    
    mem_param_82883.references = NULL;
    
    struct memblock mem_param_82880;
    
    mem_param_82880.references = NULL;
    
    struct memblock mem_param_82877;
    
    mem_param_82877.references = NULL;
    
    struct memblock mem_param_82874;
    
    mem_param_82874.references = NULL;
    
    struct memblock mem_param_82871;
    
    mem_param_82871.references = NULL;
    
    struct memblock mem_param_82868;
    
    mem_param_82868.references = NULL;
    
    struct memblock mem_param_82865;
    
    mem_param_82865.references = NULL;
    
    struct memblock mem_param_82862;
    
    mem_param_82862.references = NULL;
    
    struct memblock ext_mem_84321;
    
    ext_mem_84321.references = NULL;
    
    struct memblock ext_mem_84322;
    
    ext_mem_84322.references = NULL;
    
    struct memblock ext_mem_84323;
    
    ext_mem_84323.references = NULL;
    
    struct memblock ext_mem_84324;
    
    ext_mem_84324.references = NULL;
    
    struct memblock ext_mem_84325;
    
    ext_mem_84325.references = NULL;
    
    struct memblock ext_mem_84326;
    
    ext_mem_84326.references = NULL;
    
    struct memblock ext_mem_84327;
    
    ext_mem_84327.references = NULL;
    
    struct memblock ext_mem_84328;
    
    ext_mem_84328.references = NULL;
    
    struct memblock ext_mem_84329;
    
    ext_mem_84329.references = NULL;
    
    struct memblock ext_mem_84330;
    
    ext_mem_84330.references = NULL;
    
    struct memblock ext_mem_84331;
    
    ext_mem_84331.references = NULL;
    
    struct memblock ext_mem_84332;
    
    ext_mem_84332.references = NULL;
    
    struct memblock ext_mem_84333;
    
    ext_mem_84333.references = NULL;
    
    struct memblock ext_mem_84334;
    
    ext_mem_84334.references = NULL;
    
    struct memblock ext_mem_84335;
    
    ext_mem_84335.references = NULL;
    
    struct memblock ext_mem_84336;
    
    ext_mem_84336.references = NULL;
    
    struct memblock ext_mem_84337;
    
    ext_mem_84337.references = NULL;
    
    struct memblock ext_mem_84338;
    
    ext_mem_84338.references = NULL;
    
    struct memblock mem_82859;
    
    mem_82859.references = NULL;
    
    struct memblock mem_82857;
    
    mem_82857.references = NULL;
    
    struct memblock mem_82855;
    
    mem_82855.references = NULL;
    
    struct memblock mem_82854;
    
    mem_82854.references = NULL;
    
    struct memblock mem_82853;
    
    mem_82853.references = NULL;
    
    struct memblock mem_82852;
    
    mem_82852.references = NULL;
    
    struct memblock mem_param_tmp_84458;
    
    mem_param_tmp_84458.references = NULL;
    
    struct memblock mem_param_tmp_84457;
    
    mem_param_tmp_84457.references = NULL;
    
    struct memblock mem_82818;
    
    mem_82818.references = NULL;
    
    struct memblock mem_82814;
    
    mem_82814.references = NULL;
    
    struct memblock mem_82780;
    
    mem_82780.references = NULL;
    
    struct memblock mem_82776;
    
    mem_82776.references = NULL;
    
    struct memblock mem_82785;
    
    mem_82785.references = NULL;
    
    struct memblock mem_82783;
    
    mem_82783.references = NULL;
    
    struct memblock ext_mem_82800;
    
    ext_mem_82800.references = NULL;
    
    struct memblock ext_mem_82803;
    
    ext_mem_82803.references = NULL;
    
    struct memblock mem_param_82705;
    
    mem_param_82705.references = NULL;
    
    struct memblock mem_param_82702;
    
    mem_param_82702.references = NULL;
    
    struct memblock ext_mem_82824;
    
    ext_mem_82824.references = NULL;
    
    struct memblock ext_mem_82825;
    
    ext_mem_82825.references = NULL;
    
    struct memblock mem_82698;
    
    mem_82698.references = NULL;
    
    struct memblock mem_82694;
    
    mem_82694.references = NULL;
    
    struct memblock ext_mem_82828;
    
    ext_mem_82828.references = NULL;
    
    struct memblock mem_82623;
    
    mem_82623.references = NULL;
    
    struct memblock mem_out_84435;
    
    mem_out_84435.references = NULL;
    
    struct memblock mem_out_84434;
    
    mem_out_84434.references = NULL;
    
    struct memblock mem_out_84433;
    
    mem_out_84433.references = NULL;
    
    int64_t bytes_82614 = (int64_t) 8 * dz2081U_57317;
    bool cond_73357 = sle64(dz2081U_57317, (int64_t) 1);
    int64_t zs_lhs_73358 = sub64(dz2081U_57317, (int64_t) 1);
    int64_t zp_rhs_73359 = sdiv_safe64(zs_lhs_73358, (int64_t) 20);
    int64_t min_num_blocks_73360 = add64((int64_t) 1, zp_rhs_73359);
    bool loop_cond_73361 = slt64((int64_t) 1, min_num_blocks_73360);
    bool smallest_pow_2_geq_than_res_73362;
    int64_t smallest_pow_2_geq_than_res_73363;
    bool loop_while_73364;
    int64_t x_73365;
    
    loop_while_73364 = loop_cond_73361;
    x_73365 = (int64_t) 1;
    while (loop_while_73364) {
        int64_t loopres_73366 = mul64((int64_t) 2, x_73365);
        bool loop_cond_73367 = slt64(loopres_73366, min_num_blocks_73360);
        bool loop_while_tmp_84436 = loop_cond_73367;
        int64_t x_tmp_84437 = loopres_73366;
        
        loop_while_73364 = loop_while_tmp_84436;
        x_73365 = x_tmp_84437;
    }
    smallest_pow_2_geq_than_res_73362 = loop_while_73364;
    smallest_pow_2_geq_than_res_73363 = x_73365;
    
    bool zzero_73368 = smallest_pow_2_geq_than_res_73363 == (int64_t) 0;
    bool nonzzero_73369 = !zzero_73368;
    bool protect_assert_disj_73370 = cond_73357 || nonzzero_73369;
    bool nonzzero_cert_73371;
    
    if (!protect_assert_disj_73370) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:234:30-42\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #2  /prelude/functional.fut:9:44-45\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t zp_rhs_73372 = sdiv_safe64(zs_lhs_73358, smallest_pow_2_geq_than_res_73363);
    int64_t block_sizze_73373 = add64((int64_t) 1, zp_rhs_73372);
    bool y_73378 = slt64((int64_t) 0, dz2081U_57317);
    bool protect_assert_disj_84396 = cond_73357 || y_73378;
    bool index_certs_73379;
    
    if (!protect_assert_disj_84396) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) dz2081U_57317, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:240:60-64\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #2  /prelude/functional.fut:9:44-45\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool bounds_invalid_upwards_73392 = slt64(smallest_pow_2_geq_than_res_73363, (int64_t) 0);
    bool valid_73393 = !bounds_invalid_upwards_73392;
    bool protect_assert_disj_84404 = cond_73357 || valid_73393;
    bool range_valid_c_73394;
    
    if (!protect_assert_disj_84404) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) smallest_pow_2_geq_than_res_73363, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #3  /prelude/functional.fut:9:44-45\n   #4  test_dbscan_plus.fut:14:23-19:30\n   #5  test_dbscan_plus.fut:13:1-20:55\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool bounds_invalid_upwards_73396 = slt64(block_sizze_73373, (int64_t) 0);
    bool valid_73397 = !bounds_invalid_upwards_73396;
    bool protect_assert_disj_84406 = cond_73357 || valid_73397;
    bool range_valid_c_73398;
    
    if (!protect_assert_disj_84406) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) block_sizze_73373, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  test_dbscan_plus.fut:14:23-19:30\n   #7  test_dbscan_plus.fut:13:1-20:55\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool cond_73400 = sle64(block_sizze_73373, (int64_t) 1);
    int64_t tmp_73401 = sub64(block_sizze_73373, (int64_t) 2);
    bool bounds_invalid_upwards_73402 = slt64(tmp_73401, (int64_t) 0);
    bool valid_73404 = !bounds_invalid_upwards_73402;
    bool protect_assert_disj_73405 = cond_73400 || valid_73404;
    bool protect_assert_disj_84408 = cond_73357 || protect_assert_disj_73405;
    bool range_valid_c_73406;
    
    if (!protect_assert_disj_84408) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "...", (long long) tmp_73401, " is invalid.", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:166:19-28\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  test_dbscan_plus.fut:14:23-19:30\n   #7  test_dbscan_plus.fut:13:1-20:55\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    double defunc_0_reduce_res_81671;
    double defunc_0_reduce_res_81672;
    double redout_81685;
    double redout_81686;
    
    redout_81685 = -INFINITY;
    redout_81686 = INFINITY;
    for (int64_t i_81687 = 0; i_81687 < dz2081U_57317; i_81687++) {
        double x_77015 = ((double *) pts_mem_82613.mem)[i_81687];
        double max_res_73332 = fmax64(x_77015, redout_81685);
        double min_res_73342 = fmin64(x_77015, redout_81686);
        double redout_tmp_84438 = max_res_73332;
        double redout_tmp_84439 = min_res_73342;
        
        redout_81685 = redout_tmp_84438;
        redout_81686 = redout_tmp_84439;
    }
    defunc_0_reduce_res_81671 = redout_81685;
    defunc_0_reduce_res_81672 = redout_81686;
    
    double defunc_0_reduce_res_81673;
    double defunc_0_reduce_res_81674;
    double redout_81688;
    double redout_81689;
    
    redout_81688 = -INFINITY;
    redout_81689 = INFINITY;
    for (int64_t i_81690 = 0; i_81690 < dz2081U_57317; i_81690++) {
        double x_77018 = ((double *) pts_mem_82613.mem)[dz2081U_57317 + i_81690];
        double max_res_73337 = fmax64(x_77018, redout_81688);
        double min_res_73347 = fmin64(x_77018, redout_81689);
        double redout_tmp_84440 = max_res_73337;
        double redout_tmp_84441 = min_res_73347;
        
        redout_81688 = redout_tmp_84440;
        redout_81689 = redout_tmp_84441;
    }
    defunc_0_reduce_res_81673 = redout_81688;
    defunc_0_reduce_res_81674 = redout_81689;
    
    double zs_lhs_73348 = defunc_0_reduce_res_81671 - defunc_0_reduce_res_81672;
    double step_x_73349 = zs_lhs_73348 / 5.0;
    double zs_lhs_73350 = defunc_0_reduce_res_81673 - defunc_0_reduce_res_81674;
    double step_y_73351 = zs_lhs_73350 / 5.0;
    
    if (mem_82615_cached_sizze_84815 < bytes_82614) {
        err = lexical_realloc(ctx, &mem_82615, &mem_82615_cached_sizze_84815, bytes_82614);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    for (int64_t i_81693 = 0; i_81693 < dz2081U_57317; i_81693++) {
        double eta_p_73353 = ((double *) pts_mem_82613.mem)[i_81693];
        double eta_p_73354 = ((double *) pts_mem_82613.mem)[dz2081U_57317 + i_81693];
        int64_t defunc_0_f_res_73355;
        
        if (futrts_lifted_get_grid_id_12077(ctx, &defunc_0_f_res_73355, 5.0, step_x_73349, step_y_73351, eta_p_73353, eta_p_73354) != 0) {
            err = 1;
            goto cleanup;
        }
        ((int64_t *) mem_82615)[i_81693] = defunc_0_f_res_73355;
    }
    if (memblock_alloc(ctx, &mem_82623, bytes_82614, "mem_82623")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t i_84443 = 0; i_84443 < dz2081U_57317; i_84443++) {
        int64_t x_84444 = (int64_t) 0 + i_84443 * (int64_t) 1;
        
        ((int64_t *) mem_82623.mem)[i_84443] = x_84444;
    }
    
    int64_t flat_dim_73374 = smallest_pow_2_geq_than_res_73363 * block_sizze_73373;
    bool x_84391 = !cond_73357;
    int64_t greatest_divisor_leq_than_arg1_73376 = mul64((int64_t) 2, block_sizze_73373);
    int64_t merge_block_sizze_73377;
    
    if (x_84391) {
        int64_t x_84393;
        
        if (futrts_greatest_divisor_leq_than_5592(ctx, &x_84393, (int64_t) 8, greatest_divisor_leq_than_arg1_73376) != 0) {
            err = 1;
            goto cleanup;
        }
        merge_block_sizze_73377 = x_84393;
    } else {
        merge_block_sizze_73377 = (int64_t) 0;
    }
    
    int64_t reduce_arg1_73380;
    
    if (x_84391) {
        int64_t x_84397 = ((int64_t *) mem_82615)[(int64_t) 0];
        
        reduce_arg1_73380 = x_84397;
    } else {
        reduce_arg1_73380 = (int64_t) 0;
    }
    
    int64_t defunc_0_reduce_res_80957;
    int64_t defunc_0_reduce_res_80958;
    
    if (x_84391) {
        int64_t x_84399;
        int64_t x_84400;
        int64_t redout_81695;
        int64_t redout_81696;
        
        redout_81695 = reduce_arg1_73380;
        redout_81696 = (int64_t) 0;
        for (int64_t i_81697 = 0; i_81697 < dz2081U_57317; i_81697++) {
            int64_t x_73383 = ((int64_t *) mem_82615)[i_81697];
            bool defunc_0_leq_res_73389 = sle64(redout_81695, x_73383);
            int64_t defunc_0_op_res_73390;
            
            if (defunc_0_leq_res_73389) {
                defunc_0_op_res_73390 = x_73383;
            } else {
                defunc_0_op_res_73390 = redout_81695;
            }
            
            int64_t defunc_0_op_res_73391;
            
            if (defunc_0_leq_res_73389) {
                defunc_0_op_res_73391 = i_81697;
            } else {
                defunc_0_op_res_73391 = redout_81696;
            }
            
            int64_t redout_tmp_84445 = defunc_0_op_res_73390;
            int64_t redout_tmp_84446 = defunc_0_op_res_73391;
            
            redout_81695 = redout_tmp_84445;
            redout_81696 = redout_tmp_84446;
        }
        x_84399 = redout_81695;
        x_84400 = redout_81696;
        defunc_0_reduce_res_80957 = x_84399;
        defunc_0_reduce_res_80958 = x_84400;
    } else {
        defunc_0_reduce_res_80957 = (int64_t) 0;
        defunc_0_reduce_res_80958 = (int64_t) 0;
    }
    
    int64_t distance_73403 = add64((int64_t) 1, tmp_73401);
    int64_t binop_x_82624 = (int64_t) 8 * smallest_pow_2_geq_than_res_73363;
    int64_t bytes_82625 = block_sizze_73373 * binop_x_82624;
    int64_t bytes_82638 = (int64_t) 8 * block_sizze_73373;
    bool loop_cond_73477 = slt64(block_sizze_73373, flat_dim_73374);
    bool zzero_73478 = merge_block_sizze_73377 == (int64_t) 0;
    bool nonzzero_73479 = !zzero_73478;
    
    if (cond_73357) {
        if (memblock_set(ctx, &ext_mem_82828, &mem_82623, "mem_82623") != 0)
            return 1;
    } else {
        if (mem_82626_cached_sizze_84816 < bytes_82625) {
            err = lexical_realloc(ctx, &mem_82626, &mem_82626_cached_sizze_84816, bytes_82625);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_82629_cached_sizze_84817 < bytes_82625) {
            err = lexical_realloc(ctx, &mem_82629, &mem_82629_cached_sizze_84817, bytes_82625);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_82639_cached_sizze_84818 < bytes_82638) {
            err = lexical_realloc(ctx, &mem_82639, &mem_82639_cached_sizze_84818, bytes_82638);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_82641_cached_sizze_84819 < bytes_82638) {
            err = lexical_realloc(ctx, &mem_82641, &mem_82641_cached_sizze_84819, bytes_82638);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t i_81709 = 0; i_81709 < smallest_pow_2_geq_than_res_73363; i_81709++) {
            int64_t zp_lhs_73410 = mul64(block_sizze_73373, i_81709);
            
            for (int64_t i_81702 = 0; i_81702 < block_sizze_73373; i_81702++) {
                int64_t k_73414 = add64(zp_lhs_73410, i_81702);
                bool cond_73415 = slt64(k_73414, dz2081U_57317);
                int64_t lifted_lambda_res_73416;
                int64_t lifted_lambda_res_73417;
                
                if (cond_73415) {
                    bool x_80810 = sle64((int64_t) 0, k_73414);
                    bool bounds_check_80811 = cond_73415 && x_80810;
                    bool index_certs_80812;
                    
                    if (!bounds_check_80811) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) k_73414, "] out of bounds for array of shape [", (long long) dz2081U_57317, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:246:26-30\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t lifted_lambda_res_t_res_80813 = ((int64_t *) mem_82615)[k_73414];
                    
                    lifted_lambda_res_73416 = lifted_lambda_res_t_res_80813;
                    lifted_lambda_res_73417 = k_73414;
                } else {
                    lifted_lambda_res_73416 = defunc_0_reduce_res_80957;
                    lifted_lambda_res_73417 = defunc_0_reduce_res_80958;
                }
                ((int64_t *) mem_82639)[i_81702] = lifted_lambda_res_73416;
                ((int64_t *) mem_82641)[i_81702] = lifted_lambda_res_73417;
            }
            if (!cond_73400) {
                for (int64_t i_73426 = 0; i_73426 < distance_73403; i_73426++) {
                    int64_t gt_arg1_73429 = add64((int64_t) 1, i_73426);
                    bool x_73430 = sle64((int64_t) 0, gt_arg1_73429);
                    bool y_73431 = slt64(gt_arg1_73429, block_sizze_73373);
                    bool bounds_check_73432 = x_73430 && y_73431;
                    bool index_certs_73433;
                    
                    if (!bounds_check_73432) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) gt_arg1_73429, "] out of bounds for array of shape [", (long long) block_sizze_73373, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:169:31-37\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  test_dbscan_plus.fut:14:23-19:30\n   #7  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    bool y_73435 = slt64(i_73426, block_sizze_73373);
                    bool index_certs_73436;
                    
                    if (!y_73435) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_73426, "] out of bounds for array of shape [", (long long) block_sizze_73373, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:169:26-30\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  test_dbscan_plus.fut:14:23-19:30\n   #7  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t gt_arg1_73434 = ((int64_t *) mem_82639)[gt_arg1_73429];
                    int64_t gt_arg0_73437 = ((int64_t *) mem_82639)[i_73426];
                    bool defunc_0_leq_res_73438 = sle64(gt_arg0_73437, gt_arg1_73434);
                    bool defunc_0_lifted_gt_res_73439 = !defunc_0_leq_res_73438;
                    bool s_73440;
                    int64_t s_73443;
                    bool loop_while_73444;
                    int64_t j_73447;
                    
                    loop_while_73444 = defunc_0_lifted_gt_res_73439;
                    j_73447 = i_73426;
                    while (loop_while_73444) {
                        bool x_73448 = sle64((int64_t) 0, j_73447);
                        bool y_73449 = slt64(j_73447, block_sizze_73373);
                        bool bounds_check_73450 = x_73448 && y_73449;
                        bool index_certs_73451;
                        
                        if (!bounds_check_73450) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_73447, "] out of bounds for array of shape [", (long long) block_sizze_73373, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:170:24-28\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  test_dbscan_plus.fut:14:23-19:30\n   #7  test_dbscan_plus.fut:13:1-20:55\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t copy_arg0_73454 = add64((int64_t) 1, j_73447);
                        bool x_73455 = sle64((int64_t) 0, copy_arg0_73454);
                        bool y_73456 = slt64(copy_arg0_73454, block_sizze_73373);
                        bool bounds_check_73457 = x_73455 && y_73456;
                        bool index_certs_73458;
                        
                        if (!bounds_check_73457) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) copy_arg0_73454, "] out of bounds for array of shape [", (long long) block_sizze_73373, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:171:35-41\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  test_dbscan_plus.fut:14:23-19:30\n   #7  test_dbscan_plus.fut:13:1-20:55\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t copy_arg0_73452 = ((int64_t *) mem_82639)[j_73447];
                        int64_t copy_arg0_73453 = ((int64_t *) mem_82641)[j_73447];
                        int64_t copy_arg0_73459 = ((int64_t *) mem_82639)[copy_arg0_73454];
                        int64_t copy_arg0_73460 = ((int64_t *) mem_82641)[copy_arg0_73454];
                        
                        ((int64_t *) mem_82639)[j_73447] = copy_arg0_73459;
                        ((int64_t *) mem_82641)[j_73447] = copy_arg0_73460;
                        ((int64_t *) mem_82639)[copy_arg0_73454] = copy_arg0_73452;
                        ((int64_t *) mem_82641)[copy_arg0_73454] = copy_arg0_73453;
                        
                        int64_t tmp_73465 = sub64(j_73447, (int64_t) 1);
                        bool cond_73466 = sle64((int64_t) 0, tmp_73465);
                        bool loop_cond_73467;
                        
                        if (cond_73466) {
                            bool y_80815 = slt64(tmp_73465, block_sizze_73373);
                            bool bounds_check_80816 = cond_73466 && y_80815;
                            bool index_certs_80817;
                            
                            if (!bounds_check_80816) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_73465, "] out of bounds for array of shape [", (long long) block_sizze_73373, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:169:26-30\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  test_dbscan_plus.fut:14:23-19:30\n   #7  test_dbscan_plus.fut:13:1-20:55\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t gt_arg1_80814 = ((int64_t *) mem_82639)[j_73447];
                            int64_t gt_arg0_80818 = ((int64_t *) mem_82639)[tmp_73465];
                            bool defunc_0_leq_res_80819 = sle64(gt_arg0_80818, gt_arg1_80814);
                            bool defunc_0_lifted_gt_res_80820 = !defunc_0_leq_res_80819;
                            
                            loop_cond_73467 = defunc_0_lifted_gt_res_80820;
                        } else {
                            loop_cond_73467 = 0;
                        }
                        
                        bool loop_while_tmp_84453 = loop_cond_73467;
                        int64_t j_tmp_84456 = tmp_73465;
                        
                        loop_while_73444 = loop_while_tmp_84453;
                        j_73447 = j_tmp_84456;
                    }
                    s_73440 = loop_while_73444;
                    s_73443 = j_73447;
                }
            }
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_82626, i_81709 * block_sizze_73373, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82639, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {block_sizze_73373});
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_82629, i_81709 * block_sizze_73373, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82641, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {block_sizze_73373});
        }
        if (memblock_alloc(ctx, &mem_82694, bytes_82625, "mem_82694")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 2, (uint64_t *) mem_82694.mem, (int64_t) 0, (int64_t []) {block_sizze_73373, (int64_t) 1}, (uint64_t *) mem_82626, (int64_t) 0, (int64_t []) {block_sizze_73373, (int64_t) 1}, (int64_t []) {smallest_pow_2_geq_than_res_73363, block_sizze_73373});
        if (memblock_alloc(ctx, &mem_82698, bytes_82625, "mem_82698")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 2, (uint64_t *) mem_82698.mem, (int64_t) 0, (int64_t []) {block_sizze_73373, (int64_t) 1}, (uint64_t *) mem_82629, (int64_t) 0, (int64_t []) {block_sizze_73373, (int64_t) 1}, (int64_t []) {smallest_pow_2_geq_than_res_73363, block_sizze_73373});
        
        int64_t data_73480;
        bool data_73481;
        int64_t data_73484;
        int64_t loop_dz2084Uz2082U_73485;
        bool loop_while_73486;
        int64_t stride_73489;
        
        if (memblock_set(ctx, &mem_param_82702, &mem_82694, "mem_82694") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82705, &mem_82698, "mem_82698") != 0)
            return 1;
        loop_dz2084Uz2082U_73485 = flat_dim_73374;
        loop_while_73486 = loop_cond_73477;
        stride_73489 = block_sizze_73373;
        while (loop_while_73486) {
            int64_t next_stride_73490 = mul64((int64_t) 2, stride_73489);
            bool zzero_73491 = next_stride_73490 == (int64_t) 0;
            bool nonzzero_73492 = !zzero_73491;
            bool nonzzero_cert_73493;
            
            if (!nonzzero_73492) {
                set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:254:33-46\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #2  /prelude/functional.fut:9:44-45\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t num_merges_73494 = sdiv64(flat_dim_73374, next_stride_73490);
            bool bounds_invalid_upwards_73495 = slt64(num_merges_73494, (int64_t) 0);
            bool valid_73496 = !bounds_invalid_upwards_73495;
            bool range_valid_c_73497;
            
            if (!valid_73496) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) num_merges_73494, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #3  /prelude/functional.fut:9:44-45\n   #4  test_dbscan_plus.fut:14:23-19:30\n   #5  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t m_73499 = sub64(next_stride_73490, (int64_t) 1);
            bool zzero_leq_i_p_m_t_s_73500 = sle64((int64_t) 0, m_73499);
            bool cond_73502 = slt64(next_stride_73490, (int64_t) 10);
            bool protect_assert_disj_73503 = nonzzero_73479 || cond_73502;
            bool nonzzero_cert_73504;
            
            if (!protect_assert_disj_73503) {
                set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:147:30-42\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  test_dbscan_plus.fut:14:23-19:30\n   #7  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t zeze_lhs_73505 = smod_safe64(next_stride_73490, merge_block_sizze_73377);
            bool num_blocks_73506 = zeze_lhs_73505 == (int64_t) 0;
            bool protect_assert_disj_73507 = cond_73502 || num_blocks_73506;
            bool assert_c_73508;
            
            if (!protect_assert_disj_73507) {
                set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "Assertion is false: (n % block_size == 0)", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:147:20-65\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  test_dbscan_plus.fut:14:23-19:30\n   #7  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t num_blocks_73509 = sdiv_safe64(next_stride_73490, merge_block_sizze_73377);
            int64_t dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_73510 = add64((int64_t) 1, num_blocks_73509);
            int64_t j_m_i_73511 = sub64(next_stride_73490, stride_73489);
            int64_t flat_dim_73512 = merge_block_sizze_73377 * num_blocks_73509;
            bool bounds_invalid_upwards_73513 = slt64(dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_73510, (int64_t) 0);
            bool empty_slice_73514 = j_m_i_73511 == (int64_t) 0;
            int64_t m_73515 = sub64(j_m_i_73511, (int64_t) 1);
            bool zzero_lte_i_73516 = sle64((int64_t) 0, stride_73489);
            bool i_lte_j_73517 = sle64(stride_73489, next_stride_73490);
            bool empty_slice_73518 = stride_73489 == (int64_t) 0;
            int64_t m_73519 = sub64(stride_73489, (int64_t) 1);
            bool bounds_invalid_upwards_73520 = slt64(num_blocks_73509, (int64_t) 0);
            bool cond_73521 = slt64((int64_t) 0, stride_73489);
            bool valid_73522 = !bounds_invalid_upwards_73513;
            int64_t i_p_m_t_s_73523 = add64(stride_73489, m_73515);
            bool zzero_leq_i_p_m_t_s_73524 = sle64((int64_t) 0, m_73519);
            bool i_p_m_t_s_leq_w_73525 = slt64(m_73519, next_stride_73490);
            bool valid_73526 = !bounds_invalid_upwards_73520;
            bool i_p_m_t_s_leq_w_73527 = slt64(m_73499, flat_dim_73512);
            bool y_73528 = slt64((int64_t) 0, j_m_i_73511);
            bool protect_assert_disj_73529 = cond_73502 || valid_73522;
            bool range_valid_c_73530;
            
            if (!protect_assert_disj_73529) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_73510, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  test_dbscan_plus.fut:14:23-19:30\n   #9  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool zzero_leq_i_p_m_t_s_73531 = sle64((int64_t) 0, i_p_m_t_s_73523);
            bool i_p_m_t_s_leq_w_73532 = slt64(i_p_m_t_s_73523, next_stride_73490);
            bool y_73533 = zzero_leq_i_p_m_t_s_73524 && i_p_m_t_s_leq_w_73525;
            bool protect_assert_disj_73534 = cond_73502 || valid_73526;
            bool range_valid_c_73535;
            
            if (!protect_assert_disj_73534) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) num_blocks_73509, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool y_73536 = zzero_leq_i_p_m_t_s_73500 && i_p_m_t_s_leq_w_73527;
            bool y_73537 = zzero_lte_i_73516 && i_p_m_t_s_leq_w_73532;
            bool forwards_ok_73538 = zzero_lte_i_73516 && y_73533;
            bool ok_or_empty_73539 = zzero_73491 || y_73536;
            bool protect_assert_disj_73540 = cond_73521 || y_73528;
            bool y_73541 = zzero_leq_i_p_m_t_s_73531 && y_73537;
            bool ok_or_empty_73542 = empty_slice_73518 || forwards_ok_73538;
            bool protect_assert_disj_73543 = cond_73502 || ok_or_empty_73539;
            bool index_certs_73544;
            
            if (!protect_assert_disj_73543) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) next_stride_73490, "] out of bounds for array of shape [", (long long) flat_dim_73512, "].", "-> #0  /prelude/array.fut:46:45-51\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:155:3-46\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool loop_not_taken_73545 = !cond_73502;
            bool protect_assert_disj_73546 = protect_assert_disj_73540 || loop_not_taken_73545;
            bool index_certs_73547;
            
            if (!protect_assert_disj_73546) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) j_m_i_73511, "].", "-> #0  /prelude/array.fut:22:29-33\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:100:48-54\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  test_dbscan_plus.fut:14:23-19:30\n   #9  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool forwards_ok_73548 = i_lte_j_73517 && y_73541;
            bool protect_assert_disj_73549 = cond_73502 || ok_or_empty_73542;
            bool index_certs_73550;
            
            if (!protect_assert_disj_73549) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) stride_73489, "] out of bounds for array of shape [", (long long) next_stride_73490, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:52-59\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  test_dbscan_plus.fut:14:23-19:30\n   #10 test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool protect_assert_disj_73551 = ok_or_empty_73542 || loop_not_taken_73545;
            bool index_certs_73552;
            
            if (!protect_assert_disj_73551) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) stride_73489, "] out of bounds for array of shape [", (long long) next_stride_73490, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:26-33\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  test_dbscan_plus.fut:14:23-19:30\n   #7  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool ok_or_empty_73553 = empty_slice_73514 || forwards_ok_73548;
            bool protect_assert_disj_73554 = cond_73502 || ok_or_empty_73553;
            bool index_certs_73555;
            
            if (!protect_assert_disj_73554) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) stride_73489, ":] out of bounds for array of shape [", (long long) next_stride_73490, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:60-67\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  test_dbscan_plus.fut:14:23-19:30\n   #10 test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool protect_assert_disj_73556 = loop_not_taken_73545 || ok_or_empty_73553;
            bool index_certs_73557;
            
            if (!protect_assert_disj_73556) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) stride_73489, ":] out of bounds for array of shape [", (long long) next_stride_73490, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:34-41\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  test_dbscan_plus.fut:14:23-19:30\n   #7  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t binop_x_82706 = (int64_t) 8 * num_merges_73494;
            int64_t bytes_82707 = next_stride_73490 * binop_x_82706;
            bool i_lte_j_73501 = sle64((int64_t) 0, next_stride_73490);
            
            if (mem_82708_cached_sizze_84820 < bytes_82707) {
                err = lexical_realloc(ctx, &mem_82708, &mem_82708_cached_sizze_84820, bytes_82707);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            if (mem_82711_cached_sizze_84821 < bytes_82707) {
                err = lexical_realloc(ctx, &mem_82711, &mem_82711_cached_sizze_84821, bytes_82707);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            
            int64_t bytes_82720 = (int64_t) 8 * dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_73510;
            int64_t binop_x_82736 = (int64_t) 8 * num_blocks_73509;
            int64_t bytes_82737 = merge_block_sizze_73377 * binop_x_82736;
            int64_t bytes_82782 = (int64_t) 16 * stride_73489;
            
            for (int64_t i_81730 = 0; i_81730 < num_merges_73494; i_81730++) {
                int64_t start_73561 = mul64(next_stride_73490, i_81730);
                int64_t j_m_i_73562 = sub64(loop_dz2084Uz2082U_73485, start_73561);
                bool empty_slice_73563 = j_m_i_73562 == (int64_t) 0;
                int64_t m_73564 = sub64(j_m_i_73562, (int64_t) 1);
                int64_t i_p_m_t_s_73565 = add64(start_73561, m_73564);
                bool zzero_leq_i_p_m_t_s_73566 = sle64((int64_t) 0, i_p_m_t_s_73565);
                bool i_p_m_t_s_leq_w_73567 = slt64(i_p_m_t_s_73565, loop_dz2084Uz2082U_73485);
                bool zzero_lte_i_73568 = sle64((int64_t) 0, start_73561);
                bool i_lte_j_73569 = sle64(start_73561, loop_dz2084Uz2082U_73485);
                bool y_73570 = i_p_m_t_s_leq_w_73567 && zzero_lte_i_73568;
                bool y_73571 = zzero_leq_i_p_m_t_s_73566 && y_73570;
                bool forwards_ok_73572 = i_lte_j_73569 && y_73571;
                bool ok_or_empty_73573 = empty_slice_73563 || forwards_ok_73572;
                bool index_certs_73574;
                
                if (!ok_or_empty_73573) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) start_73561, ":] out of bounds for array of shape [", (long long) loop_dz2084Uz2082U_73485, "].", "-> #0  /prelude/array.fut:52:49-54\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:258:41-56\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  test_dbscan_plus.fut:14:23-19:30\n   #7  test_dbscan_plus.fut:13:1-20:55\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                bool i_p_m_t_s_leq_w_73575 = slt64(m_73499, j_m_i_73562);
                bool y_73576 = zzero_leq_i_p_m_t_s_73500 && i_p_m_t_s_leq_w_73575;
                bool forwards_ok_73577 = i_lte_j_73501 && y_73576;
                bool ok_or_empty_73578 = zzero_73491 || forwards_ok_73577;
                bool index_certs_73579;
                
                if (!ok_or_empty_73578) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) next_stride_73490, "] out of bounds for array of shape [", (long long) j_m_i_73562, "].", "-> #0  /prelude/array.fut:46:45-51\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:258:23-56\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  test_dbscan_plus.fut:14:23-19:30\n   #7  test_dbscan_plus.fut:13:1-20:55\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                if (cond_73502) {
                    int64_t dummy_80846;
                    int64_t dummy_80847;
                    
                    if (cond_73521) {
                        int64_t head_res_80848 = ((int64_t *) mem_param_82702.mem)[start_73561];
                        int64_t head_res_80849 = ((int64_t *) mem_param_82705.mem)[start_73561];
                        
                        dummy_80846 = head_res_80848;
                        dummy_80847 = head_res_80849;
                    } else {
                        int64_t slice_80850 = stride_73489 + start_73561;
                        int64_t head_res_80851 = ((int64_t *) mem_param_82702.mem)[slice_80850];
                        int64_t head_res_80852 = ((int64_t *) mem_param_82705.mem)[slice_80850];
                        
                        dummy_80846 = head_res_80851;
                        dummy_80847 = head_res_80852;
                    }
                    if (memblock_alloc(ctx, &mem_82783, bytes_82782, "mem_82783")) {
                        err = 1;
                        goto cleanup;
                    }
                    for (int64_t nest_i_84466 = 0; nest_i_84466 < next_stride_73490; nest_i_84466++) {
                        ((int64_t *) mem_82783.mem)[nest_i_84466] = dummy_80846;
                    }
                    if (memblock_alloc(ctx, &mem_82785, bytes_82782, "mem_82785")) {
                        err = 1;
                        goto cleanup;
                    }
                    for (int64_t nest_i_84467 = 0; nest_i_84467 < next_stride_73490; nest_i_84467++) {
                        ((int64_t *) mem_82785.mem)[nest_i_84467] = dummy_80847;
                    }
                    
                    int64_t data_80855;
                    int64_t i_80859 = (int64_t) 0;
                    
                    for (int64_t k_80858 = 0; k_80858 < next_stride_73490; k_80858++) {
                        int64_t j_80862 = sub64(k_80858, i_80859);
                        bool cond_80863 = j_80862 == j_m_i_73511;
                        bool cond_80864;
                        
                        if (cond_80863) {
                            cond_80864 = 1;
                        } else {
                            bool cond_80865 = slt64(i_80859, stride_73489);
                            bool cond_f_res_80866;
                            
                            if (cond_80865) {
                                bool x_80867 = sle64((int64_t) 0, j_80862);
                                bool y_80868 = slt64(j_80862, j_m_i_73511);
                                bool bounds_check_80869 = x_80867 && y_80868;
                                bool index_certs_80870;
                                
                                if (!bounds_check_80869) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_80862, "] out of bounds for array of shape [", (long long) j_m_i_73511, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:55-59\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                bool x_80874 = sle64((int64_t) 0, i_80859);
                                bool bounds_check_80875 = cond_80865 && x_80874;
                                bool index_certs_80876;
                                
                                if (!bounds_check_80875) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_80859, "] out of bounds for array of shape [", (long long) stride_73489, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:50-54\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t slice_80871 = stride_73489 + j_80862;
                                int64_t slice_80872 = start_73561 + slice_80871;
                                int64_t leq_arg1_80873 = ((int64_t *) mem_param_82702.mem)[slice_80872];
                                int64_t slice_80877 = start_73561 + i_80859;
                                int64_t leq_arg0_80878 = ((int64_t *) mem_param_82702.mem)[slice_80877];
                                bool defunc_0_leq_res_80879 = sle64(leq_arg0_80878, leq_arg1_80873);
                                
                                cond_f_res_80866 = defunc_0_leq_res_80879;
                            } else {
                                cond_f_res_80866 = 0;
                            }
                            cond_80864 = cond_f_res_80866;
                        }
                        
                        int64_t loopres_80880;
                        int64_t loopres_80881;
                        int64_t loopres_80882;
                        
                        if (cond_80864) {
                            bool x_80884 = sle64((int64_t) 0, i_80859);
                            bool y_80885 = slt64(i_80859, stride_73489);
                            bool bounds_check_80886 = x_80884 && y_80885;
                            bool index_certs_80887;
                            
                            if (!bounds_check_80886) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_80859, "] out of bounds for array of shape [", (long long) stride_73489, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:106:17-21\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t tmp_80883 = add64((int64_t) 1, i_80859);
                            int64_t slice_80888 = start_73561 + i_80859;
                            int64_t tmp_80889 = ((int64_t *) mem_param_82702.mem)[slice_80888];
                            int64_t tmp_80890 = ((int64_t *) mem_param_82705.mem)[slice_80888];
                            
                            loopres_80880 = tmp_80883;
                            loopres_80881 = tmp_80889;
                            loopres_80882 = tmp_80890;
                        } else {
                            bool x_80891 = sle64((int64_t) 0, j_80862);
                            bool y_80892 = slt64(j_80862, j_m_i_73511);
                            bool bounds_check_80893 = x_80891 && y_80892;
                            bool index_certs_80894;
                            
                            if (!bounds_check_80893) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_80862, "] out of bounds for array of shape [", (long long) j_m_i_73511, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:108:15-19\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t slice_80895 = stride_73489 + j_80862;
                            int64_t slice_80896 = start_73561 + slice_80895;
                            int64_t tmp_80897 = ((int64_t *) mem_param_82702.mem)[slice_80896];
                            int64_t tmp_80898 = ((int64_t *) mem_param_82705.mem)[slice_80896];
                            
                            loopres_80880 = i_80859;
                            loopres_80881 = tmp_80897;
                            loopres_80882 = tmp_80898;
                        }
                        ((int64_t *) mem_82783.mem)[k_80858] = loopres_80881;
                        ((int64_t *) mem_82785.mem)[k_80858] = loopres_80882;
                        
                        int64_t i_tmp_84468 = loopres_80880;
                        
                        i_80859 = i_tmp_84468;
                    }
                    data_80855 = i_80859;
                    if (memblock_set(ctx, &ext_mem_82803, &mem_82783, "mem_82783") != 0)
                        return 1;
                    if (memblock_set(ctx, &ext_mem_82800, &mem_82785, "mem_82785") != 0)
                        return 1;
                } else {
                    if (mem_82721_cached_sizze_84822 < bytes_82720) {
                        err = lexical_realloc(ctx, &mem_82721, &mem_82721_cached_sizze_84822, bytes_82720);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    if (mem_82723_cached_sizze_84823 < bytes_82720) {
                        err = lexical_realloc(ctx, &mem_82723, &mem_82723_cached_sizze_84823, bytes_82720);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    for (int64_t i_81716 = 0; i_81716 < dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_73510; i_81716++) {
                        int64_t split_count_arg3_73641 = mul64(merge_block_sizze_73377, i_81716);
                        int64_t defunc_0_split_count_res_73642;
                        int64_t defunc_0_split_count_res_73643;
                        int64_t defunc_0_split_count_res_73644;
                        int64_t defunc_0_split_count_res_73645;
                        int64_t defunc_0_split_count_res_73646;
                        
                        if (futrts_lifted_normalizze_11972(ctx, &defunc_0_split_count_res_73642, &defunc_0_split_count_res_73643, &defunc_0_split_count_res_73644, &defunc_0_split_count_res_73645, &defunc_0_split_count_res_73646, (int64_t) 0, stride_73489, (int64_t) 0, j_m_i_73511, split_count_arg3_73641) != 0) {
                            err = 1;
                            goto cleanup;
                        }
                        
                        bool loop_cond_73647 = slt64((int64_t) 0, defunc_0_split_count_res_73646);
                        bool defunc_0_split_count_res_73648;
                        int64_t defunc_0_split_count_res_73649;
                        int64_t defunc_0_split_count_res_73650;
                        int64_t defunc_0_split_count_res_73651;
                        int64_t defunc_0_split_count_res_73652;
                        int64_t defunc_0_split_count_res_73653;
                        bool loop_while_73654;
                        int64_t ss_73655;
                        int64_t ss_73656;
                        int64_t tt_73657;
                        int64_t tt_73658;
                        int64_t count_73659;
                        
                        loop_while_73654 = loop_cond_73647;
                        ss_73655 = defunc_0_split_count_res_73642;
                        ss_73656 = defunc_0_split_count_res_73643;
                        tt_73657 = defunc_0_split_count_res_73644;
                        tt_73658 = defunc_0_split_count_res_73645;
                        count_73659 = defunc_0_split_count_res_73646;
                        while (loop_while_73654) {
                            int64_t zlze_lhs_73660 = sub64(ss_73656, ss_73655);
                            bool cond_73661 = sle64(zlze_lhs_73660, (int64_t) 0);
                            int64_t defunc_0_lifted_step_res_73662;
                            int64_t defunc_0_lifted_step_res_73663;
                            int64_t defunc_0_lifted_step_res_73664;
                            int64_t defunc_0_lifted_step_res_73665;
                            
                            if (cond_73661) {
                                int64_t tmp_80901 = add64(tt_73657, count_73659);
                                
                                defunc_0_lifted_step_res_73662 = ss_73655;
                                defunc_0_lifted_step_res_73663 = tmp_80901;
                                defunc_0_lifted_step_res_73664 = tt_73658;
                                defunc_0_lifted_step_res_73665 = (int64_t) 0;
                            } else {
                                int64_t zlze_lhs_73667 = sub64(tt_73658, tt_73657);
                                bool cond_73668 = sle64(zlze_lhs_73667, (int64_t) 0);
                                int64_t defunc_0_lifted_step_res_f_res_73669;
                                int64_t defunc_0_lifted_step_res_f_res_73670;
                                int64_t defunc_0_lifted_step_res_f_res_73671;
                                int64_t defunc_0_lifted_step_res_f_res_73672;
                                
                                if (cond_73668) {
                                    int64_t tmp_80902 = add64(ss_73655, count_73659);
                                    
                                    defunc_0_lifted_step_res_f_res_73669 = tmp_80902;
                                    defunc_0_lifted_step_res_f_res_73670 = tt_73657;
                                    defunc_0_lifted_step_res_f_res_73671 = tt_73658;
                                    defunc_0_lifted_step_res_f_res_73672 = (int64_t) 0;
                                } else {
                                    bool cond_73674 = count_73659 == (int64_t) 1;
                                    int64_t defunc_0_lifted_step_res_f_res_f_res_73675;
                                    int64_t defunc_0_lifted_step_res_f_res_f_res_73676;
                                    int64_t defunc_0_lifted_step_res_f_res_f_res_73677;
                                    int64_t defunc_0_lifted_step_res_f_res_f_res_73678;
                                    
                                    if (cond_73674) {
                                        bool x_80904 = sle64((int64_t) 0, ss_73655);
                                        bool y_80905 = slt64(ss_73655, stride_73489);
                                        bool bounds_check_80906 = x_80904 && y_80905;
                                        bool index_certs_80907;
                                        
                                        if (!bounds_check_80906) {
                                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) ss_73655, "] out of bounds for array of shape [", (long long) stride_73489, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:53:21-27\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 test_dbscan_plus.fut:14:23-19:30\n   #12 test_dbscan_plus.fut:13:1-20:55\n"));
                                            err = FUTHARK_PROGRAM_ERROR;
                                            goto cleanup;
                                        }
                                        
                                        bool x_80910 = sle64((int64_t) 0, tt_73657);
                                        bool y_80911 = slt64(tt_73657, j_m_i_73511);
                                        bool bounds_check_80912 = x_80910 && y_80911;
                                        bool index_certs_80913;
                                        
                                        if (!bounds_check_80912) {
                                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tt_73657, "] out of bounds for array of shape [", (long long) j_m_i_73511, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:53:14-20\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 test_dbscan_plus.fut:14:23-19:30\n   #12 test_dbscan_plus.fut:13:1-20:55\n"));
                                            err = FUTHARK_PROGRAM_ERROR;
                                            goto cleanup;
                                        }
                                        
                                        int64_t slice_80908 = start_73561 + ss_73655;
                                        int64_t leq_arg1_80909 = ((int64_t *) mem_param_82702.mem)[slice_80908];
                                        int64_t slice_80914 = stride_73489 + tt_73657;
                                        int64_t slice_80915 = start_73561 + slice_80914;
                                        int64_t leq_arg0_80916 = ((int64_t *) mem_param_82702.mem)[slice_80915];
                                        bool defunc_0_leq_res_80917 = sle64(leq_arg0_80916, leq_arg1_80909);
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_t_res_80918;
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_t_res_80919;
                                        
                                        if (defunc_0_leq_res_80917) {
                                            int64_t tmp_80920 = add64((int64_t) 1, tt_73657);
                                            
                                            defunc_0_lifted_step_res_f_res_f_res_t_res_80918 = ss_73655;
                                            defunc_0_lifted_step_res_f_res_f_res_t_res_80919 = tmp_80920;
                                        } else {
                                            int64_t tmp_80921 = add64((int64_t) 1, ss_73655);
                                            
                                            defunc_0_lifted_step_res_f_res_f_res_t_res_80918 = tmp_80921;
                                            defunc_0_lifted_step_res_f_res_f_res_t_res_80919 = tt_73657;
                                        }
                                        defunc_0_lifted_step_res_f_res_f_res_73675 = defunc_0_lifted_step_res_f_res_f_res_t_res_80918;
                                        defunc_0_lifted_step_res_f_res_f_res_73676 = defunc_0_lifted_step_res_f_res_f_res_t_res_80919;
                                        defunc_0_lifted_step_res_f_res_f_res_73677 = tt_73658;
                                        defunc_0_lifted_step_res_f_res_f_res_73678 = (int64_t) 0;
                                    } else {
                                        int64_t m_73697 = sdiv64(count_73659, (int64_t) 2);
                                        int64_t n_73698 = sub64(count_73659, m_73697);
                                        bool cond_73699 = n_73698 == (int64_t) 0;
                                        int64_t zgze_lhs_73700 = add64(ss_73655, m_73697);
                                        bool cond_f_res_73701 = sle64(ss_73656, zgze_lhs_73700);
                                        bool x_73702 = !cond_73699;
                                        bool y_73703 = cond_f_res_73701 && x_73702;
                                        bool cond_73704 = cond_73699 || y_73703;
                                        bool leq_y_x_73705;
                                        
                                        if (cond_73704) {
                                            leq_y_x_73705 = 1;
                                        } else {
                                            bool x_73706 = sle64((int64_t) 0, zgze_lhs_73700);
                                            bool y_73707 = slt64(zgze_lhs_73700, stride_73489);
                                            bool bounds_check_73708 = x_73706 && y_73707;
                                            bool index_certs_73709;
                                            
                                            if (!bounds_check_73708) {
                                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) zgze_lhs_73700, "] out of bounds for array of shape [", (long long) stride_73489, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:73:22-30\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 test_dbscan_plus.fut:14:23-19:30\n   #12 test_dbscan_plus.fut:13:1-20:55\n"));
                                                err = FUTHARK_PROGRAM_ERROR;
                                                goto cleanup;
                                            }
                                            
                                            int64_t zm_lhs_73712 = add64(tt_73657, n_73698);
                                            int64_t leq_arg0_73713 = sub64(zm_lhs_73712, (int64_t) 1);
                                            bool x_73714 = sle64((int64_t) 0, leq_arg0_73713);
                                            bool y_73715 = slt64(leq_arg0_73713, j_m_i_73511);
                                            bool bounds_check_73716 = x_73714 && y_73715;
                                            bool index_certs_73717;
                                            
                                            if (!bounds_check_73716) {
                                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) leq_arg0_73713, "] out of bounds for array of shape [", (long long) j_m_i_73511, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:73:11-21\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 test_dbscan_plus.fut:14:23-19:30\n   #12 test_dbscan_plus.fut:13:1-20:55\n"));
                                                err = FUTHARK_PROGRAM_ERROR;
                                                goto cleanup;
                                            }
                                            
                                            int64_t slice_73710 = start_73561 + zgze_lhs_73700;
                                            int64_t leq_arg1_73711 = ((int64_t *) mem_param_82702.mem)[slice_73710];
                                            int64_t slice_73718 = stride_73489 + leq_arg0_73713;
                                            int64_t slice_73719 = start_73561 + slice_73718;
                                            int64_t leq_arg0_73720 = ((int64_t *) mem_param_82702.mem)[slice_73719];
                                            bool defunc_0_leq_res_73721 = sle64(leq_arg0_73720, leq_arg1_73711);
                                            
                                            leq_y_x_73705 = defunc_0_leq_res_73721;
                                        }
                                        
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_f_res_73722;
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_f_res_73723;
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_f_res_73724;
                                        
                                        if (leq_y_x_73705) {
                                            int64_t tmp_80922 = add64(tt_73657, n_73698);
                                            int64_t tmp_80923 = sub64(count_73659, n_73698);
                                            
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_73722 = tmp_80922;
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_73723 = tt_73658;
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_73724 = tmp_80923;
                                        } else {
                                            int64_t tmp_73727 = add64(tt_73657, n_73698);
                                            
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_73722 = tt_73657;
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_73723 = tmp_73727;
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_73724 = count_73659;
                                        }
                                        defunc_0_lifted_step_res_f_res_f_res_73675 = ss_73655;
                                        defunc_0_lifted_step_res_f_res_f_res_73676 = defunc_0_lifted_step_res_f_res_f_res_f_res_73722;
                                        defunc_0_lifted_step_res_f_res_f_res_73677 = defunc_0_lifted_step_res_f_res_f_res_f_res_73723;
                                        defunc_0_lifted_step_res_f_res_f_res_73678 = defunc_0_lifted_step_res_f_res_f_res_f_res_73724;
                                    }
                                    defunc_0_lifted_step_res_f_res_73669 = defunc_0_lifted_step_res_f_res_f_res_73675;
                                    defunc_0_lifted_step_res_f_res_73670 = defunc_0_lifted_step_res_f_res_f_res_73676;
                                    defunc_0_lifted_step_res_f_res_73671 = defunc_0_lifted_step_res_f_res_f_res_73677;
                                    defunc_0_lifted_step_res_f_res_73672 = defunc_0_lifted_step_res_f_res_f_res_73678;
                                }
                                defunc_0_lifted_step_res_73662 = defunc_0_lifted_step_res_f_res_73669;
                                defunc_0_lifted_step_res_73663 = defunc_0_lifted_step_res_f_res_73670;
                                defunc_0_lifted_step_res_73664 = defunc_0_lifted_step_res_f_res_73671;
                                defunc_0_lifted_step_res_73665 = defunc_0_lifted_step_res_f_res_73672;
                            }
                            
                            int64_t loopres_73728;
                            int64_t loopres_73729;
                            int64_t loopres_73730;
                            int64_t loopres_73731;
                            int64_t loopres_73732;
                            
                            if (futrts_lifted_normalizze_11972(ctx, &loopres_73728, &loopres_73729, &loopres_73730, &loopres_73731, &loopres_73732, defunc_0_lifted_step_res_73662, ss_73656, defunc_0_lifted_step_res_73663, defunc_0_lifted_step_res_73664, defunc_0_lifted_step_res_73665) != 0) {
                                err = 1;
                                goto cleanup;
                            }
                            
                            bool loop_cond_73733 = slt64((int64_t) 0, loopres_73732);
                            bool loop_while_tmp_84473 = loop_cond_73733;
                            int64_t ss_tmp_84474 = loopres_73728;
                            int64_t ss_tmp_84475 = loopres_73729;
                            int64_t tt_tmp_84476 = loopres_73730;
                            int64_t tt_tmp_84477 = loopres_73731;
                            int64_t count_tmp_84478 = loopres_73732;
                            
                            loop_while_73654 = loop_while_tmp_84473;
                            ss_73655 = ss_tmp_84474;
                            ss_73656 = ss_tmp_84475;
                            tt_73657 = tt_tmp_84476;
                            tt_73658 = tt_tmp_84477;
                            count_73659 = count_tmp_84478;
                        }
                        defunc_0_split_count_res_73648 = loop_while_73654;
                        defunc_0_split_count_res_73649 = ss_73655;
                        defunc_0_split_count_res_73650 = ss_73656;
                        defunc_0_split_count_res_73651 = tt_73657;
                        defunc_0_split_count_res_73652 = tt_73658;
                        defunc_0_split_count_res_73653 = count_73659;
                        ((int64_t *) mem_82721)[i_81716] = defunc_0_split_count_res_73649;
                        ((int64_t *) mem_82723)[i_81716] = defunc_0_split_count_res_73651;
                    }
                    if (mem_82738_cached_sizze_84824 < bytes_82737) {
                        err = lexical_realloc(ctx, &mem_82738, &mem_82738_cached_sizze_84824, bytes_82737);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    if (mem_82741_cached_sizze_84825 < bytes_82737) {
                        err = lexical_realloc(ctx, &mem_82741, &mem_82741_cached_sizze_84825, bytes_82737);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    for (int64_t i_81723 = 0; i_81723 < num_blocks_73509; i_81723++) {
                        bool y_73739 = slt64(i_81723, dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_73510);
                        bool index_certs_73741;
                        
                        if (!y_73739) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_81723, "] out of bounds for array of shape [", (long long) dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_73510, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:151:22-34\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  test_dbscan_plus.fut:14:23-19:30\n   #9  test_dbscan_plus.fut:13:1-20:55\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t defunc_0_f_res_73742 = ((int64_t *) mem_82721)[i_81723];
                        int64_t defunc_0_f_res_73743 = ((int64_t *) mem_82723)[i_81723];
                        int64_t tmp_73744 = add64((int64_t) 1, i_81723);
                        bool x_73745 = sle64((int64_t) 0, tmp_73744);
                        bool y_73746 = slt64(tmp_73744, dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_73510);
                        bool bounds_check_73747 = x_73745 && y_73746;
                        bool index_certs_73748;
                        
                        if (!bounds_check_73747) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_73744, "] out of bounds for array of shape [", (long long) dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_73510, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:152:22-36\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  test_dbscan_plus.fut:14:23-19:30\n   #9  test_dbscan_plus.fut:13:1-20:55\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t defunc_0_f_res_73749 = ((int64_t *) mem_82721)[tmp_73744];
                        int64_t defunc_0_f_res_73750 = ((int64_t *) mem_82723)[tmp_73744];
                        int64_t merge_sequential_arg2_73751 = add64(stride_73489, defunc_0_f_res_73743);
                        int64_t merge_sequential_arg2_73752 = add64(stride_73489, defunc_0_f_res_73750);
                        int64_t j_m_i_73753 = sub64(merge_sequential_arg2_73752, merge_sequential_arg2_73751);
                        bool empty_slice_73754 = j_m_i_73753 == (int64_t) 0;
                        int64_t m_73755 = sub64(j_m_i_73753, (int64_t) 1);
                        int64_t i_p_m_t_s_73756 = add64(merge_sequential_arg2_73751, m_73755);
                        bool zzero_leq_i_p_m_t_s_73757 = sle64((int64_t) 0, i_p_m_t_s_73756);
                        bool i_p_m_t_s_leq_w_73758 = slt64(i_p_m_t_s_73756, next_stride_73490);
                        bool zzero_lte_i_73759 = sle64((int64_t) 0, merge_sequential_arg2_73751);
                        bool i_lte_j_73760 = sle64(merge_sequential_arg2_73751, merge_sequential_arg2_73752);
                        bool y_73761 = i_p_m_t_s_leq_w_73758 && zzero_lte_i_73759;
                        bool y_73762 = zzero_leq_i_p_m_t_s_73757 && y_73761;
                        bool forwards_ok_73763 = i_lte_j_73760 && y_73762;
                        bool ok_or_empty_73764 = empty_slice_73754 || forwards_ok_73763;
                        bool index_certs_73765;
                        
                        if (!ok_or_empty_73764) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) merge_sequential_arg2_73751, ":", (long long) merge_sequential_arg2_73752, "] out of bounds for array of shape [", (long long) next_stride_73490, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:40-58\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  test_dbscan_plus.fut:14:23-19:30\n   #9  test_dbscan_plus.fut:13:1-20:55\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t j_m_i_73766 = sub64(defunc_0_f_res_73749, defunc_0_f_res_73742);
                        bool empty_slice_73767 = j_m_i_73766 == (int64_t) 0;
                        int64_t m_73768 = sub64(j_m_i_73766, (int64_t) 1);
                        int64_t i_p_m_t_s_73769 = add64(defunc_0_f_res_73742, m_73768);
                        bool zzero_leq_i_p_m_t_s_73770 = sle64((int64_t) 0, i_p_m_t_s_73769);
                        bool i_p_m_t_s_leq_w_73771 = slt64(i_p_m_t_s_73769, next_stride_73490);
                        bool zzero_lte_i_73772 = sle64((int64_t) 0, defunc_0_f_res_73742);
                        bool i_lte_j_73773 = sle64(defunc_0_f_res_73742, defunc_0_f_res_73749);
                        bool y_73774 = i_p_m_t_s_leq_w_73771 && zzero_lte_i_73772;
                        bool y_73775 = zzero_leq_i_p_m_t_s_73770 && y_73774;
                        bool forwards_ok_73776 = i_lte_j_73773 && y_73775;
                        bool ok_or_empty_73777 = empty_slice_73767 || forwards_ok_73776;
                        bool index_certs_73778;
                        
                        if (!ok_or_empty_73777) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) defunc_0_f_res_73742, ":", (long long) defunc_0_f_res_73749, "] out of bounds for array of shape [", (long long) next_stride_73490, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:29-39\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  test_dbscan_plus.fut:14:23-19:30\n   #9  test_dbscan_plus.fut:13:1-20:55\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        bool cond_73779 = slt64((int64_t) 0, j_m_i_73766);
                        int64_t dummy_73780;
                        int64_t dummy_73781;
                        
                        if (cond_73779) {
                            bool index_certs_80924;
                            
                            if (!cond_73779) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) j_m_i_73766, "].", "-> #0  /prelude/array.fut:22:29-33\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:100:36-42\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #6  /prelude/soacs.fut:59:9-11\n   #7  /prelude/array.fut:241:11-17\n   #8  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #9  /prelude/functional.fut:9:44-45\n   #10 test_dbscan_plus.fut:14:23-19:30\n   #11 test_dbscan_plus.fut:13:1-20:55\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t slice_80925 = start_73561 + defunc_0_f_res_73742;
                            int64_t head_res_80926 = ((int64_t *) mem_param_82702.mem)[slice_80925];
                            int64_t head_res_80927 = ((int64_t *) mem_param_82705.mem)[slice_80925];
                            
                            dummy_73780 = head_res_80926;
                            dummy_73781 = head_res_80927;
                        } else {
                            bool y_73786 = slt64((int64_t) 0, j_m_i_73753);
                            bool index_certs_73787;
                            
                            if (!y_73786) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) j_m_i_73753, "].", "-> #0  /prelude/array.fut:22:29-33\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:100:48-54\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #6  /prelude/soacs.fut:59:9-11\n   #7  /prelude/array.fut:241:11-17\n   #8  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #9  /prelude/functional.fut:9:44-45\n   #10 test_dbscan_plus.fut:14:23-19:30\n   #11 test_dbscan_plus.fut:13:1-20:55\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t slice_73788 = start_73561 + merge_sequential_arg2_73751;
                            int64_t head_res_73789 = ((int64_t *) mem_param_82702.mem)[slice_73788];
                            int64_t head_res_73790 = ((int64_t *) mem_param_82705.mem)[slice_73788];
                            
                            dummy_73780 = head_res_73789;
                            dummy_73781 = head_res_73790;
                        }
                        for (int64_t nest_i_84481 = 0; nest_i_84481 < merge_block_sizze_73377; nest_i_84481++) {
                            ((int64_t *) mem_82738)[i_81723 * merge_block_sizze_73377 + nest_i_84481] = dummy_73780;
                        }
                        for (int64_t nest_i_84482 = 0; nest_i_84482 < merge_block_sizze_73377; nest_i_84482++) {
                            ((int64_t *) mem_82741)[i_81723 * merge_block_sizze_73377 + nest_i_84482] = dummy_73781;
                        }
                        
                        int64_t data_73793;
                        int64_t i_73797 = (int64_t) 0;
                        
                        for (int64_t k_73796 = 0; k_73796 < merge_block_sizze_73377; k_73796++) {
                            int64_t j_73800 = sub64(k_73796, i_73797);
                            bool cond_73801 = j_73800 == j_m_i_73753;
                            bool cond_73802;
                            
                            if (cond_73801) {
                                cond_73802 = 1;
                            } else {
                                bool cond_73803 = slt64(i_73797, j_m_i_73766);
                                bool cond_f_res_73804;
                                
                                if (cond_73803) {
                                    bool x_80928 = sle64((int64_t) 0, j_73800);
                                    bool y_80929 = slt64(j_73800, j_m_i_73753);
                                    bool bounds_check_80930 = x_80928 && y_80929;
                                    bool index_certs_80931;
                                    
                                    if (!bounds_check_80930) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_73800, "] out of bounds for array of shape [", (long long) j_m_i_73753, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:55-59\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  test_dbscan_plus.fut:14:23-19:30\n   #10 test_dbscan_plus.fut:13:1-20:55\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    bool x_80935 = sle64((int64_t) 0, i_73797);
                                    bool bounds_check_80936 = cond_73803 && x_80935;
                                    bool index_certs_80937;
                                    
                                    if (!bounds_check_80936) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_73797, "] out of bounds for array of shape [", (long long) j_m_i_73766, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:50-54\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  test_dbscan_plus.fut:14:23-19:30\n   #10 test_dbscan_plus.fut:13:1-20:55\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    int64_t slice_80932 = merge_sequential_arg2_73751 + j_73800;
                                    int64_t slice_80933 = start_73561 + slice_80932;
                                    int64_t leq_arg1_80934 = ((int64_t *) mem_param_82702.mem)[slice_80933];
                                    int64_t slice_80938 = defunc_0_f_res_73742 + i_73797;
                                    int64_t slice_80939 = start_73561 + slice_80938;
                                    int64_t leq_arg0_80940 = ((int64_t *) mem_param_82702.mem)[slice_80939];
                                    bool defunc_0_leq_res_80941 = sle64(leq_arg0_80940, leq_arg1_80934);
                                    
                                    cond_f_res_73804 = defunc_0_leq_res_80941;
                                } else {
                                    cond_f_res_73804 = 0;
                                }
                                cond_73802 = cond_f_res_73804;
                            }
                            
                            int64_t loopres_73819;
                            int64_t loopres_73820;
                            int64_t loopres_73821;
                            
                            if (cond_73802) {
                                bool x_80943 = sle64((int64_t) 0, i_73797);
                                bool y_80944 = slt64(i_73797, j_m_i_73766);
                                bool bounds_check_80945 = x_80943 && y_80944;
                                bool index_certs_80946;
                                
                                if (!bounds_check_80945) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_73797, "] out of bounds for array of shape [", (long long) j_m_i_73766, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:106:17-21\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  test_dbscan_plus.fut:14:23-19:30\n   #10 test_dbscan_plus.fut:13:1-20:55\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t tmp_80942 = add64((int64_t) 1, i_73797);
                                int64_t slice_80947 = defunc_0_f_res_73742 + i_73797;
                                int64_t slice_80948 = start_73561 + slice_80947;
                                int64_t tmp_80949 = ((int64_t *) mem_param_82702.mem)[slice_80948];
                                int64_t tmp_80950 = ((int64_t *) mem_param_82705.mem)[slice_80948];
                                
                                loopres_73819 = tmp_80942;
                                loopres_73820 = tmp_80949;
                                loopres_73821 = tmp_80950;
                            } else {
                                bool x_73831 = sle64((int64_t) 0, j_73800);
                                bool y_73832 = slt64(j_73800, j_m_i_73753);
                                bool bounds_check_73833 = x_73831 && y_73832;
                                bool index_certs_73834;
                                
                                if (!bounds_check_73833) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_73800, "] out of bounds for array of shape [", (long long) j_m_i_73753, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:108:15-19\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  test_dbscan_plus.fut:14:23-19:30\n   #10 test_dbscan_plus.fut:13:1-20:55\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t slice_73835 = merge_sequential_arg2_73751 + j_73800;
                                int64_t slice_73836 = start_73561 + slice_73835;
                                int64_t tmp_73837 = ((int64_t *) mem_param_82702.mem)[slice_73836];
                                int64_t tmp_73838 = ((int64_t *) mem_param_82705.mem)[slice_73836];
                                
                                loopres_73819 = i_73797;
                                loopres_73820 = tmp_73837;
                                loopres_73821 = tmp_73838;
                            }
                            ((int64_t *) mem_82738)[i_81723 * merge_block_sizze_73377 + k_73796] = loopres_73820;
                            ((int64_t *) mem_82741)[i_81723 * merge_block_sizze_73377 + k_73796] = loopres_73821;
                            
                            int64_t i_tmp_84483 = loopres_73819;
                            
                            i_73797 = i_tmp_84483;
                        }
                        data_73793 = i_73797;
                    }
                    if (memblock_alloc(ctx, &mem_82776, bytes_82737, "mem_82776")) {
                        err = 1;
                        goto cleanup;
                    }
                    lmad_copy_8b(ctx, 2, (uint64_t *) mem_82776.mem, (int64_t) 0, (int64_t []) {merge_block_sizze_73377, (int64_t) 1}, (uint64_t *) mem_82738, (int64_t) 0, (int64_t []) {merge_block_sizze_73377, (int64_t) 1}, (int64_t []) {num_blocks_73509, merge_block_sizze_73377});
                    if (memblock_alloc(ctx, &mem_82780, bytes_82737, "mem_82780")) {
                        err = 1;
                        goto cleanup;
                    }
                    lmad_copy_8b(ctx, 2, (uint64_t *) mem_82780.mem, (int64_t) 0, (int64_t []) {merge_block_sizze_73377, (int64_t) 1}, (uint64_t *) mem_82741, (int64_t) 0, (int64_t []) {merge_block_sizze_73377, (int64_t) 1}, (int64_t []) {num_blocks_73509, merge_block_sizze_73377});
                    if (memblock_set(ctx, &ext_mem_82803, &mem_82776, "mem_82776") != 0)
                        return 1;
                    if (memblock_set(ctx, &ext_mem_82800, &mem_82780, "mem_82780") != 0)
                        return 1;
                }
                lmad_copy_8b(ctx, 1, (uint64_t *) mem_82708, i_81730 * next_stride_73490, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_82803.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {next_stride_73490});
                if (memblock_unref(ctx, &ext_mem_82803, "ext_mem_82803") != 0)
                    return 1;
                lmad_copy_8b(ctx, 1, (uint64_t *) mem_82711, i_81730 * next_stride_73490, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_82800.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {next_stride_73490});
                if (memblock_unref(ctx, &ext_mem_82800, "ext_mem_82800") != 0)
                    return 1;
            }
            
            int64_t flat_dim_73845 = next_stride_73490 * num_merges_73494;
            
            if (memblock_alloc(ctx, &mem_82814, bytes_82707, "mem_82814")) {
                err = 1;
                goto cleanup;
            }
            lmad_copy_8b(ctx, 2, (uint64_t *) mem_82814.mem, (int64_t) 0, (int64_t []) {next_stride_73490, (int64_t) 1}, (uint64_t *) mem_82708, (int64_t) 0, (int64_t []) {next_stride_73490, (int64_t) 1}, (int64_t []) {num_merges_73494, next_stride_73490});
            if (memblock_alloc(ctx, &mem_82818, bytes_82707, "mem_82818")) {
                err = 1;
                goto cleanup;
            }
            lmad_copy_8b(ctx, 2, (uint64_t *) mem_82818.mem, (int64_t) 0, (int64_t []) {next_stride_73490, (int64_t) 1}, (uint64_t *) mem_82711, (int64_t) 0, (int64_t []) {next_stride_73490, (int64_t) 1}, (int64_t []) {num_merges_73494, next_stride_73490});
            
            bool loop_cond_73848 = slt64(next_stride_73490, flat_dim_73374);
            
            if (memblock_set(ctx, &mem_param_tmp_84457, &mem_82814, "mem_82814") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_tmp_84458, &mem_82818, "mem_82818") != 0)
                return 1;
            
            int64_t loop_dz2084Uz2082U_tmp_84459 = flat_dim_73845;
            bool loop_while_tmp_84460 = loop_cond_73848;
            int64_t stride_tmp_84463 = next_stride_73490;
            
            if (memblock_set(ctx, &mem_param_82702, &mem_param_tmp_84457, "mem_param_tmp_84457") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_82705, &mem_param_tmp_84458, "mem_param_tmp_84458") != 0)
                return 1;
            loop_dz2084Uz2082U_73485 = loop_dz2084Uz2082U_tmp_84459;
            loop_while_73486 = loop_while_tmp_84460;
            stride_73489 = stride_tmp_84463;
        }
        if (memblock_set(ctx, &ext_mem_82825, &mem_param_82702, "mem_param_82702") != 0)
            return 1;
        if (memblock_set(ctx, &ext_mem_82824, &mem_param_82705, "mem_param_82705") != 0)
            return 1;
        data_73480 = loop_dz2084Uz2082U_73485;
        data_73481 = loop_while_73486;
        data_73484 = stride_73489;
        if (memblock_unref(ctx, &mem_82694, "mem_82694") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82698, "mem_82698") != 0)
            return 1;
        
        bool empty_slice_73849 = dz2081U_57317 == (int64_t) 0;
        bool zzero_leq_i_p_m_t_s_73850 = sle64((int64_t) 0, zs_lhs_73358);
        bool i_p_m_t_s_leq_w_73851 = slt64(zs_lhs_73358, data_73480);
        bool y_73852 = zzero_leq_i_p_m_t_s_73850 && i_p_m_t_s_leq_w_73851;
        bool ok_or_empty_73853 = empty_slice_73849 || y_73852;
        bool index_certs_73854;
        
        if (!ok_or_empty_73853) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) dz2081U_57317, "] out of bounds for array of shape [", (long long) data_73480, "].", "-> #0  /prelude/array.fut:46:45-51\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:265:3-14\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #3  /prelude/functional.fut:9:44-45\n   #4  test_dbscan_plus.fut:14:23-19:30\n   #5  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        if (memblock_set(ctx, &ext_mem_82828, &ext_mem_82824, "ext_mem_82824") != 0)
            return 1;
    }
    if (memblock_unref(ctx, &mem_82623, "mem_82623") != 0)
        return 1;
    if (mem_82830_cached_sizze_84826 < bytes_82614) {
        err = lexical_realloc(ctx, &mem_82830, &mem_82830_cached_sizze_84826, bytes_82614);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_82832_cached_sizze_84827 < bytes_82614) {
        err = lexical_realloc(ctx, &mem_82832, &mem_82832_cached_sizze_84827, bytes_82614);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    for (int64_t i_81737 = 0; i_81737 < dz2081U_57317; i_81737++) {
        int64_t eta_p_73858 = ((int64_t *) ext_mem_82828.mem)[i_81737];
        bool x_73859 = sle64((int64_t) 0, eta_p_73858);
        bool y_73860 = slt64(eta_p_73858, dz2081U_57317);
        bool bounds_check_73861 = x_73859 && y_73860;
        bool index_certs_73862;
        
        if (!bounds_check_73861) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_73858, "] out of bounds for array of shape [", (long long) dz2081U_57317, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:277:24-28\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:277:8-29\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        double lifted_lambda_res_73863 = ((double *) pts_mem_82613.mem)[eta_p_73858];
        double lifted_lambda_res_73864 = ((double *) pts_mem_82613.mem)[dz2081U_57317 + eta_p_73858];
        
        ((double *) mem_82830)[i_81737] = lifted_lambda_res_73863;
        ((double *) mem_82832)[i_81737] = lifted_lambda_res_73864;
    }
    if (memblock_unref(ctx, &ext_mem_82828, "ext_mem_82828") != 0)
        return 1;
    if (mem_82845_cached_sizze_84828 < (int64_t) 200) {
        err = lexical_realloc(ctx, &mem_82845, &mem_82845_cached_sizze_84828, (int64_t) 200);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    for (int64_t nest_i_84488 = 0; nest_i_84488 < (int64_t) 25; nest_i_84488++) {
        ((int64_t *) mem_82845)[nest_i_84488] = dz2081U_57317;
    }
    for (int64_t iter_81740 = 0; iter_81740 < dz2081U_57317; iter_81740++) {
        double pixel_81742 = ((double *) mem_82830)[iter_81740];
        double pixel_81743 = ((double *) mem_82832)[iter_81740];
        int64_t lifted_lambda_res_77012;
        
        if (futrts_lifted_get_grid_id_12077(ctx, &lifted_lambda_res_77012, 5.0, step_x_73349, step_y_73351, pixel_81742, pixel_81743) != 0) {
            err = 1;
            goto cleanup;
        }
        
        bool less_than_zzero_81745 = slt64(lifted_lambda_res_77012, (int64_t) 0);
        bool greater_than_sizze_81746 = sle64((int64_t) 25, lifted_lambda_res_77012);
        bool outside_bounds_dim_81747 = less_than_zzero_81745 || greater_than_sizze_81746;
        
        if (!outside_bounds_dim_81747) {
            int64_t read_hist_81749 = ((int64_t *) mem_82845)[lifted_lambda_res_77012];
            bool cond_73873 = slt64(read_hist_81749, iter_81740);
            int64_t lifted_lambda_res_73874;
            
            if (cond_73873) {
                lifted_lambda_res_73874 = read_hist_81749;
            } else {
                lifted_lambda_res_73874 = iter_81740;
            }
            ((int64_t *) mem_82845)[lifted_lambda_res_77012] = lifted_lambda_res_73874;
        }
    }
    if (memblock_alloc(ctx, &mem_82852, (int64_t) 25, "mem_82852")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t nest_i_84490 = 0; nest_i_84490 < (int64_t) 25; nest_i_84490++) {
        ((bool *) mem_82852.mem)[nest_i_84490] = 0;
    }
    if (memblock_alloc(ctx, &mem_82853, (int64_t) 0, "mem_82853")) {
        err = 1;
        goto cleanup;
    }
    if (memblock_alloc(ctx, &mem_82854, (int64_t) 0, "mem_82854")) {
        err = 1;
        goto cleanup;
    }
    if (memblock_alloc(ctx, &mem_82855, (int64_t) 0, "mem_82855")) {
        err = 1;
        goto cleanup;
    }
    if (memblock_alloc(ctx, &mem_82857, bytes_82614, "mem_82857")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t nest_i_84491 = 0; nest_i_84491 < dz2081U_57317; nest_i_84491++) {
        ((double *) mem_82857.mem)[nest_i_84491] = 0.0;
    }
    if (memblock_alloc(ctx, &mem_82859, bytes_82614, "mem_82859")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t nest_i_84492 = 0; nest_i_84492 < dz2081U_57317; nest_i_84492++) {
        ((int64_t *) mem_82859.mem)[nest_i_84492] = (int64_t) -1;
    }
    if (mem_82922_cached_sizze_84829 < (int64_t) 64) {
        err = lexical_realloc(ctx, &mem_82922, &mem_82922_cached_sizze_84829, (int64_t) 64);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_82928_cached_sizze_84830 < (int64_t) 64) {
        err = lexical_realloc(ctx, &mem_82928, &mem_82928_cached_sizze_84830, (int64_t) 64);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_82929_cached_sizze_84831 < (int64_t) 64) {
        err = lexical_realloc(ctx, &mem_82929, &mem_82929_cached_sizze_84831, (int64_t) 64);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_82930_cached_sizze_84832 < (int64_t) 64) {
        err = lexical_realloc(ctx, &mem_82930, &mem_82930_cached_sizze_84832, (int64_t) 64);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    
    int64_t ext_84286;
    int64_t ext_84285;
    int64_t internal_DBSCAN_double_res_73884;
    int64_t internal_DBSCAN_double_res_73885;
    int64_t internal_DBSCAN_double_res_73886;
    int64_t internal_DBSCAN_double_res_73887;
    int64_t internal_DBSCAN_double_res_73888;
    int64_t internal_DBSCAN_double_res_73889;
    int64_t internal_DBSCAN_double_res_73890;
    int64_t internal_DBSCAN_double_res_73891;
    bool internal_DBSCAN_double_res_73892;
    int64_t internal_DBSCAN_double_res_73893;
    int64_t internal_DBSCAN_double_res_73898;
    int64_t internal_DBSCAN_double_res_73906;
    int64_t internal_DBSCAN_double_res_73907;
    int64_t internal_DBSCAN_double_res_73915;
    int64_t loop_dz2085Uz2083U_73916;
    int64_t loop_dz2085Uz2084U_73917;
    int64_t loop_dz2085Uz2085U_73918;
    int64_t loop_dz2086Uz2080U_73919;
    int64_t loop_dz2086Uz2081U_73920;
    int64_t loop_dz2086Uz2082U_73921;
    int64_t loop_dz2086Uz2084U_73922;
    int64_t loop_dz2087Uz2080U_73923;
    bool loop_while_73924;
    int64_t num_flushed_73925;
    int64_t collisions_73930;
    int64_t old_clHandler_73938;
    int64_t old_clHandler_73939;
    int64_t next_part_73947;
    int64_t ctx_param_ext_82911;
    int64_t ctx_param_ext_82912;
    
    if (memblock_set(ctx, &mem_param_82862, &mem_82857, "mem_82857") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82865, &mem_82857, "mem_82857") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82868, &mem_82859, "mem_82859") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82871, &mem_82853, "mem_82853") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82874, &mem_82853, "mem_82853") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82877, &mem_82853, "mem_82853") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82880, &mem_82853, "mem_82853") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82883, &mem_82854, "mem_82854") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82886, &mem_82853, "mem_82853") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82889, &mem_82855, "mem_82855") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82892, &mem_82855, "mem_82855") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82895, &mem_82852, "mem_82852") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82898, &mem_82853, "mem_82853") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82901, &mem_82853, "mem_82853") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82904, &mem_82855, "mem_82855") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82907, &mem_82855, "mem_82855") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82910, &mem_82854, "mem_82854") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82913, &mem_82854, "mem_82854") != 0)
        return 1;
    ctx_param_ext_82911 = (int64_t) 0;
    ctx_param_ext_82912 = (int64_t) 1;
    loop_dz2085Uz2083U_73916 = (int64_t) 0;
    loop_dz2085Uz2084U_73917 = (int64_t) 0;
    loop_dz2085Uz2085U_73918 = (int64_t) 0;
    loop_dz2086Uz2080U_73919 = (int64_t) 0;
    loop_dz2086Uz2081U_73920 = (int64_t) 0;
    loop_dz2086Uz2082U_73921 = (int64_t) 0;
    loop_dz2086Uz2084U_73922 = (int64_t) 0;
    loop_dz2087Uz2080U_73923 = (int64_t) 0;
    loop_while_73924 = 1;
    num_flushed_73925 = (int64_t) 0;
    collisions_73930 = (int64_t) 0;
    old_clHandler_73938 = (int64_t) 0;
    old_clHandler_73939 = (int64_t) -1;
    next_part_73947 = (int64_t) 0;
    while (loop_while_73924) {
        bool x_73948 = sle64((int64_t) 0, next_part_73947);
        bool y_73949 = slt64(next_part_73947, (int64_t) 25);
        bool bounds_check_73950 = x_73948 && y_73949;
        bool index_certs_73951;
        
        if (!bounds_check_73950) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) next_part_73947, "] out of bounds for array of shape [", (long long) (int64_t) 25, "].", "-> #0  ../../ftDBSCAN_plus.fut:1114:35-56\n   #1  test_dbscan_plus.fut:14:23-19:30\n   #2  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t inf_73952 = ((int64_t *) mem_82845)[next_part_73947];
        bool cond_73953 = next_part_73947 == (int64_t) 24;
        bool x_73954 = !cond_73953;
        int64_t tmp_73955 = add64((int64_t) 1, next_part_73947);
        bool x_73956 = sle64((int64_t) 0, tmp_73955);
        bool y_73957 = slt64(tmp_73955, (int64_t) 25);
        bool bounds_check_73958 = x_73956 && y_73957;
        bool protect_assert_disj_73959 = cond_73953 || bounds_check_73958;
        bool index_certs_73960;
        
        if (!protect_assert_disj_73959) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_73955, "] out of bounds for array of shape [", (long long) (int64_t) 25, "].", "-> #0  ../../ftDBSCAN_plus.fut:1115:71-94\n   #1  test_dbscan_plus.fut:14:23-19:30\n   #2  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t sup_f_res_73961;
        
        if (x_73954) {
            int64_t x_80961 = ((int64_t *) mem_82845)[tmp_73955];
            
            sup_f_res_73961 = x_80961;
        } else {
            sup_f_res_73961 = (int64_t) 0;
        }
        
        int64_t sup_73963;
        
        if (cond_73953) {
            sup_73963 = dz2081U_57317;
        } else {
            sup_73963 = sup_f_res_73961;
        }
        
        int64_t dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73964 = sub64(sup_73963, inf_73952);
        bool cond_73965 = inf_73952 == dz2081U_57317;
        bool empty_slice_73966 = dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73964 == (int64_t) 0;
        int64_t m_73967 = sub64(dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73964, (int64_t) 1);
        int64_t i_p_m_t_s_73968 = add64(inf_73952, m_73967);
        bool zzero_leq_i_p_m_t_s_73969 = sle64((int64_t) 0, i_p_m_t_s_73968);
        bool i_p_m_t_s_leq_w_73970 = slt64(i_p_m_t_s_73968, dz2081U_57317);
        bool zzero_lte_i_73971 = sle64((int64_t) 0, inf_73952);
        bool i_lte_j_73972 = sle64(inf_73952, sup_73963);
        bool y_73973 = i_p_m_t_s_leq_w_73970 && zzero_lte_i_73971;
        bool y_73974 = zzero_leq_i_p_m_t_s_73969 && y_73973;
        bool forwards_ok_73975 = i_lte_j_73972 && y_73974;
        bool ok_or_empty_73976 = empty_slice_73966 || forwards_ok_73975;
        bool protect_assert_disj_73977 = cond_73965 || ok_or_empty_73976;
        bool index_certs_73978;
        
        if (!protect_assert_disj_73977) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) inf_73952, ":", (long long) sup_73963, "] out of bounds for array of shape [", (long long) dz2081U_57317, "].", "-> #0  ../../ftDBSCAN_plus.fut:1118:76-95\n   #1  test_dbscan_plus.fut:14:23-19:30\n   #2  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t loopres_73979;
        
        if (cond_73965) {
            loopres_73979 = (int64_t) 0;
        } else {
            loopres_73979 = dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73964;
        }
        
        int64_t bytes_82914 = (int64_t) 8 * dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73964;
        int64_t bytes_82918 = (int64_t) 8 * loopres_73979;
        
        if (cond_73965) {
            if (memblock_alloc(ctx, &mem_82919, bytes_82918, "mem_82919")) {
                err = 1;
                goto cleanup;
            }
            if (memblock_set(ctx, &ext_mem_82921, &mem_82919, "mem_82919") != 0)
                return 1;
            if (memblock_set(ctx, &ext_mem_82920, &mem_82919, "mem_82919") != 0)
                return 1;
        } else {
            if (memblock_alloc(ctx, &mem_82915, bytes_82914, "mem_82915")) {
                err = 1;
                goto cleanup;
            }
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_82915.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82830, (int64_t) 0 + (int64_t) 1 * inf_73952, (int64_t []) {(int64_t) 1}, (int64_t []) {dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73964});
            if (memblock_alloc(ctx, &mem_82917, bytes_82914, "mem_82917")) {
                err = 1;
                goto cleanup;
            }
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_82917.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82832, (int64_t) 0 + (int64_t) 1 * inf_73952, (int64_t []) {(int64_t) 1}, (int64_t []) {dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73964});
            if (memblock_set(ctx, &ext_mem_82921, &mem_82915, "mem_82915") != 0)
                return 1;
            if (memblock_set(ctx, &ext_mem_82920, &mem_82917, "mem_82917") != 0)
                return 1;
        }
        
        bool eq_x_zz_73989 = (int64_t) 0 == dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73964;
        bool not_p_73990 = !cond_73965;
        bool p_and_eq_x_y_73991 = eq_x_zz_73989 && not_p_73990;
        bool eq_x_y_73992 = cond_73965 || p_and_eq_x_y_73991;
        bool p_and_eq_x_y_73993 = cond_73965 && empty_slice_73966;
        bool eq_x_zz_73994 = not_p_73990 || p_and_eq_x_y_73993;
        bool p_and_eq_x_y_73995 = cond_73965 && eq_x_y_73992;
        bool p_and_eq_x_y_73996 = not_p_73990 && eq_x_zz_73994;
        bool dim_match_73997 = p_and_eq_x_y_73995 || p_and_eq_x_y_73996;
        bool empty_or_match_cert_73998;
        
        if (!dim_match_73997) {
            set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "function arguments of wrong shape", "-> #0  ../../ftDBSCAN_plus.fut:1119:41-115\n   #1  test_dbscan_plus.fut:14:23-19:30\n   #2  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t x_id_73999 = smod64(next_part_73947, (int64_t) 5);
        int64_t y_id_74000 = sdiv64(next_part_73947, (int64_t) 5);
        bool cond_74001 = slt64((int64_t) 0, x_id_73999);
        int64_t zp_lhs_74002 = mul64((int64_t) 5, y_id_74000);
        int64_t zm_lhs_74003 = add64(x_id_73999, zp_lhs_74002);
        int64_t nleft_t_res_74004 = sub64(zm_lhs_74003, (int64_t) 1);
        int64_t nleft_74005;
        
        if (cond_74001) {
            nleft_74005 = nleft_t_res_74004;
        } else {
            nleft_74005 = (int64_t) -1;
        }
        
        bool cond_t_res_74006 = slt64((int64_t) 0, y_id_74000);
        bool x_74007 = cond_74001 && cond_t_res_74006;
        int64_t zt_lhs_74008 = sub64(y_id_74000, (int64_t) 1);
        int64_t zp_lhs_74009 = mul64((int64_t) 5, zt_lhs_74008);
        int64_t zm_lhs_74010 = add64(x_id_73999, zp_lhs_74009);
        int64_t nld_t_res_74011 = sub64(zm_lhs_74010, (int64_t) 1);
        int64_t nld_74012;
        
        if (x_74007) {
            nld_74012 = nld_t_res_74011;
        } else {
            nld_74012 = (int64_t) -1;
        }
        
        int64_t ndown_74013;
        
        if (cond_t_res_74006) {
            ndown_74013 = zm_lhs_74010;
        } else {
            ndown_74013 = (int64_t) -1;
        }
        
        bool cond_74014 = slt64(x_id_73999, (int64_t) 4);
        bool x_74015 = cond_t_res_74006 && cond_74014;
        int64_t nrd_t_res_74016 = add64((int64_t) 1, zm_lhs_74010);
        int64_t nrd_74017;
        
        if (x_74015) {
            nrd_74017 = nrd_t_res_74016;
        } else {
            nrd_74017 = (int64_t) -1;
        }
        
        int64_t nright_t_res_74018 = add64((int64_t) 1, zm_lhs_74003);
        int64_t nright_74019;
        
        if (cond_74014) {
            nright_74019 = nright_t_res_74018;
        } else {
            nright_74019 = (int64_t) -1;
        }
        
        bool cond_t_res_74020 = slt64(y_id_74000, (int64_t) 4);
        bool x_74021 = cond_74014 && cond_t_res_74020;
        int64_t zt_lhs_74022 = add64((int64_t) 1, y_id_74000);
        int64_t zp_lhs_74023 = mul64((int64_t) 5, zt_lhs_74022);
        int64_t zp_lhs_74024 = add64(x_id_73999, zp_lhs_74023);
        int64_t nru_t_res_74025 = add64((int64_t) 1, zp_lhs_74024);
        int64_t nru_74026;
        
        if (x_74021) {
            nru_74026 = nru_t_res_74025;
        } else {
            nru_74026 = (int64_t) -1;
        }
        
        int64_t nup_74027;
        
        if (cond_t_res_74020) {
            nup_74027 = zp_lhs_74024;
        } else {
            nup_74027 = (int64_t) -1;
        }
        
        bool x_74028 = cond_74001 && cond_t_res_74020;
        int64_t nlu_t_res_74029 = sub64(zp_lhs_74024, (int64_t) 1);
        int64_t nlu_74030;
        
        if (x_74028) {
            nlu_74030 = nlu_t_res_74029;
        } else {
            nlu_74030 = (int64_t) -1;
        }
        ((int64_t *) mem_82922)[(int64_t) 0] = nleft_74005;
        ((int64_t *) mem_82922)[(int64_t) 1] = nld_74012;
        ((int64_t *) mem_82922)[(int64_t) 2] = ndown_74013;
        ((int64_t *) mem_82922)[(int64_t) 3] = nrd_74017;
        ((int64_t *) mem_82922)[(int64_t) 4] = nright_74019;
        ((int64_t *) mem_82922)[(int64_t) 5] = nru_74026;
        ((int64_t *) mem_82922)[(int64_t) 6] = nup_74027;
        ((int64_t *) mem_82922)[(int64_t) 7] = nlu_74030;
        if (memblock_alloc(ctx, &mem_82923, (int64_t) 25, "mem_82923")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_1b(ctx, 1, (uint8_t *) mem_82923.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint8_t *) mem_param_82895.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {(int64_t) 25});
        ((bool *) mem_82923.mem)[next_part_73947] = 1;
        
        bool cond_74034 = x_id_73999 == (int64_t) 0;
        double i64_res_74035 = sitofp_i64_f64(x_id_73999);
        double zt_res_74036 = step_x_73349 * i64_res_74035;
        double zp_res_74037 = zt_res_74036 + defunc_0_reduce_res_81672;
        double minC_74038;
        
        if (cond_74034) {
            minC_74038 = -INFINITY;
        } else {
            minC_74038 = zp_res_74037;
        }
        
        bool cond_74039 = y_id_74000 == (int64_t) 0;
        double i64_res_74040 = sitofp_i64_f64(y_id_74000);
        double zt_res_74041 = step_y_73351 * i64_res_74040;
        double zp_res_74042 = zt_res_74041 + defunc_0_reduce_res_81674;
        double minC_74043;
        
        if (cond_74039) {
            minC_74043 = -INFINITY;
        } else {
            minC_74043 = zp_res_74042;
        }
        
        bool cond_74044 = x_id_73999 == (int64_t) 4;
        int64_t from_i64_arg0_74045 = add64((int64_t) 1, x_id_73999);
        double i64_res_74046 = sitofp_i64_f64(from_i64_arg0_74045);
        double zt_res_74047 = step_x_73349 * i64_res_74046;
        double zp_res_74048 = zt_res_74047 + defunc_0_reduce_res_81672;
        double maxC_74049;
        
        if (cond_74044) {
            maxC_74049 = INFINITY;
        } else {
            maxC_74049 = zp_res_74048;
        }
        
        bool cond_74050 = y_id_74000 == (int64_t) 4;
        double i64_res_74051 = sitofp_i64_f64(zt_lhs_74022);
        double zt_res_74052 = step_y_73351 * i64_res_74051;
        double zp_res_74053 = zt_res_74052 + defunc_0_reduce_res_81674;
        double maxC_74054;
        
        if (cond_74050) {
            maxC_74054 = INFINITY;
        } else {
            maxC_74054 = zp_res_74053;
        }
        if (memblock_alloc(ctx, &mem_82925, bytes_82918, "mem_82925")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_82925.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_82921.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {loopres_73979});
        if (memblock_alloc(ctx, &mem_82927, bytes_82918, "mem_82927")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_82927.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_82920.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {loopres_73979});
        
        int64_t discard_81767;
        int64_t scanacc_81759 = (int64_t) 0;
        
        for (int64_t i_81763 = 0; i_81763 < (int64_t) 8; i_81763++) {
            int64_t eta_p_78587 = ((int64_t *) mem_82922)[i_81763];
            bool defunc_0_reduce_res_80972;
            bool redout_81752 = 0;
            
            for (int64_t i_81753 = 0; i_81753 < loop_dz2086Uz2084U_73922; i_81753++) {
                int64_t eta_p_78690 = ((int64_t *) mem_param_82898.mem)[i_81753];
                bool lifted_lambda_res_78691 = eta_p_78690 == eta_p_78587;
                bool defunc_0_op_res_78595 = lifted_lambda_res_78691 || redout_81752;
                bool redout_tmp_84549 = defunc_0_op_res_78595;
                
                redout_81752 = redout_tmp_84549;
            }
            defunc_0_reduce_res_80972 = redout_81752;
            
            int64_t lifted_lambda_res_78596;
            
            if (defunc_0_reduce_res_80972) {
                lifted_lambda_res_78596 = (int64_t) -1;
            } else {
                lifted_lambda_res_78596 = eta_p_78587;
            }
            
            bool cond_78598 = slt64(lifted_lambda_res_78596, (int64_t) 0);
            int64_t defunc_0_f_res_78599;
            
            if (cond_78598) {
                defunc_0_f_res_78599 = (int64_t) -1;
            } else {
                defunc_0_f_res_78599 = lifted_lambda_res_78596;
            }
            
            bool defunc_0_f_res_78600;
            
            if (cond_78598) {
                defunc_0_f_res_78600 = 0;
            } else {
                int64_t nxid_78601 = smod64(lifted_lambda_res_78596, (int64_t) 5);
                int64_t nyid_78602 = sdiv64(lifted_lambda_res_78596, (int64_t) 5);
                double i64_res_78603 = sitofp_i64_f64(nxid_78601);
                double zt_res_78604 = step_x_73349 * i64_res_78603;
                double zp_res_78605 = zt_res_78604 + defunc_0_reduce_res_81672;
                double i64_res_78606 = sitofp_i64_f64(nyid_78602);
                double zt_res_78607 = step_y_73351 * i64_res_78606;
                double zp_res_78608 = zt_res_78607 + defunc_0_reduce_res_81674;
                double zp_res_78609 = step_x_73349 + zp_res_78605;
                double zp_res_78610 = step_y_73351 + zp_res_78608;
                bool defunc_0_reduce_res_80971;
                bool redout_81754 = 0;
                
                for (int64_t i_81755 = 0; i_81755 < loopres_73979; i_81755++) {
                    double eta_p_78694 = ((double *) ext_mem_82921.mem)[i_81755];
                    double eta_p_78695 = ((double *) ext_mem_82920.mem)[i_81755];
                    bool zgze_res_78696 = zp_res_78608 <= eta_p_78695;
                    bool zlze_res_78697 = eta_p_78695 <= zp_res_78610;
                    bool x_78698 = zgze_res_78696 && zlze_res_78697;
                    bool zeze_res_78699 = eta_p_78694 == zp_res_78605;
                    double d_euclidean_res_78700;
                    
                    if (zeze_res_78699) {
                        double zm_res_80963 = eta_p_78695 - eta_p_78695;
                        double abs_res_80964 = fabs64(zm_res_80963);
                        
                        d_euclidean_res_78700 = abs_res_80964;
                    } else {
                        double zm_res_78703 = eta_p_78694 - zp_res_78605;
                        double abs_res_78704 = fabs64(zm_res_78703);
                        
                        d_euclidean_res_78700 = abs_res_78704;
                    }
                    
                    bool zlze_res_78705 = d_euclidean_res_78700 <= 2.0;
                    bool zeze_res_78706 = eta_p_78694 == zp_res_78609;
                    double d_euclidean_res_78707;
                    
                    if (zeze_res_78706) {
                        double zm_res_80965 = eta_p_78695 - eta_p_78695;
                        double abs_res_80966 = fabs64(zm_res_80965);
                        
                        d_euclidean_res_78707 = abs_res_80966;
                    } else {
                        double zm_res_78710 = eta_p_78694 - zp_res_78609;
                        double abs_res_78711 = fabs64(zm_res_78710);
                        
                        d_euclidean_res_78707 = abs_res_78711;
                    }
                    
                    bool zlze_res_78712 = d_euclidean_res_78707 <= 2.0;
                    bool x_78713 = !zlze_res_78705;
                    bool y_78714 = zlze_res_78712 && x_78713;
                    bool cond_t_res_78715 = zlze_res_78705 || y_78714;
                    bool x_78716 = x_78698 && cond_t_res_78715;
                    bool zgze_res_78717 = zp_res_78605 <= eta_p_78694;
                    bool zlze_res_78718 = eta_p_78694 <= zp_res_78609;
                    bool x_78719 = zgze_res_78717 && zlze_res_78718;
                    double zm_res_78720 = eta_p_78695 - zp_res_78608;
                    double abs_res_78721 = fabs64(zm_res_78720);
                    bool zlze_res_78722 = abs_res_78721 <= 2.0;
                    double zm_res_78723 = eta_p_78695 - zp_res_78610;
                    double abs_res_78724 = fabs64(zm_res_78723);
                    bool zlze_res_78725 = abs_res_78724 <= 2.0;
                    bool x_78726 = !zlze_res_78722;
                    bool y_78727 = zlze_res_78725 && x_78726;
                    bool cond_f_res_t_res_78728 = zlze_res_78722 || y_78727;
                    bool x_78729 = x_78719 && cond_f_res_t_res_78728;
                    bool x_78730 = !x_78716;
                    bool y_78731 = x_78729 && x_78730;
                    bool cond_78732 = x_78716 || y_78731;
                    bool zlze_res_78733 = eta_p_78694 <= zp_res_78605;
                    double corn_x_78734;
                    
                    if (zlze_res_78733) {
                        corn_x_78734 = zp_res_78605;
                    } else {
                        corn_x_78734 = zp_res_78609;
                    }
                    
                    bool zlze_res_78735 = eta_p_78695 <= zp_res_78608;
                    double corn_y_78736;
                    
                    if (zlze_res_78735) {
                        corn_y_78736 = zp_res_78608;
                    } else {
                        corn_y_78736 = zp_res_78610;
                    }
                    
                    bool p_and_eq_x_y_78737 = zeze_res_78699 && zlze_res_78733;
                    bool not_p_78738 = !zlze_res_78733;
                    bool p_and_eq_x_y_78739 = zeze_res_78706 && not_p_78738;
                    bool zeze_res_78740 = p_and_eq_x_y_78737 || p_and_eq_x_y_78739;
                    double d_euclidean_res_78741;
                    
                    if (zeze_res_78740) {
                        double zm_res_80967 = eta_p_78695 - corn_y_78736;
                        double abs_res_80968 = fabs64(zm_res_80967);
                        
                        d_euclidean_res_78741 = abs_res_80968;
                    } else {
                        bool eq_x_y_78744 = eta_p_78695 == zp_res_78608;
                        bool eq_x_zz_78745 = eta_p_78695 == zp_res_78610;
                        bool p_and_eq_x_y_78746 = zlze_res_78735 && eq_x_y_78744;
                        bool not_p_78747 = !zlze_res_78735;
                        bool p_and_eq_x_y_78748 = eq_x_zz_78745 && not_p_78747;
                        bool zeze_res_78749 = p_and_eq_x_y_78746 || p_and_eq_x_y_78748;
                        double d_euclidean_res_f_res_78750;
                        
                        if (zeze_res_78749) {
                            double zm_res_80969 = eta_p_78694 - corn_x_78734;
                            double abs_res_80970 = fabs64(zm_res_80969);
                            
                            d_euclidean_res_f_res_78750 = abs_res_80970;
                        } else {
                            double zm_res_78753 = eta_p_78695 - corn_y_78736;
                            double zm_res_78754 = eta_p_78694 - corn_x_78734;
                            double hypot_res_78755 = futrts_hypot64(zm_res_78754, zm_res_78753);
                            
                            d_euclidean_res_f_res_78750 = hypot_res_78755;
                        }
                        d_euclidean_res_78741 = d_euclidean_res_f_res_78750;
                    }
                    
                    bool zlze_res_78756 = d_euclidean_res_78741 <= 2.0;
                    bool x_78757 = !cond_78732;
                    bool y_78758 = zlze_res_78756 && x_78757;
                    bool is_pt_marginal_res_78759 = cond_78732 || y_78758;
                    bool defunc_0_op_res_78682 = is_pt_marginal_res_78759 || redout_81754;
                    bool redout_tmp_84550 = defunc_0_op_res_78682;
                    
                    redout_81754 = redout_tmp_84550;
                }
                defunc_0_reduce_res_80971 = redout_81754;
                defunc_0_f_res_78600 = defunc_0_reduce_res_80971;
            }
            
            int64_t defunc_0_f_res_78685 = btoi_bool_i64(defunc_0_f_res_78600);
            int64_t defunc_0_op_res_74163 = add64(defunc_0_f_res_78685, scanacc_81759);
            
            ((int64_t *) mem_82928)[i_81763] = defunc_0_op_res_74163;
            ((int64_t *) mem_82929)[i_81763] = defunc_0_f_res_78685;
            ((int64_t *) mem_82930)[i_81763] = defunc_0_f_res_78599;
            
            int64_t scanacc_tmp_84545 = defunc_0_op_res_74163;
            
            scanacc_81759 = scanacc_tmp_84545;
        }
        discard_81767 = scanacc_81759;
        
        int64_t m_f_res_74164 = ((int64_t *) mem_82928)[(int64_t) 7];
        int64_t m_74172 = sub64(m_f_res_74164, (int64_t) 1);
        bool i_p_m_t_s_leq_w_74174 = slt64(m_74172, (int64_t) 8);
        bool zzero_leq_i_p_m_t_s_74173 = sle64((int64_t) 0, m_74172);
        bool y_74176 = zzero_leq_i_p_m_t_s_74173 && i_p_m_t_s_leq_w_74174;
        bool i_lte_j_74175 = sle64((int64_t) 0, m_f_res_74164);
        bool forwards_ok_74177 = i_lte_j_74175 && y_74176;
        bool empty_slice_74171 = m_f_res_74164 == (int64_t) 0;
        bool ok_or_empty_74178 = empty_slice_74171 || forwards_ok_74177;
        bool index_certs_74179;
        
        if (!ok_or_empty_74178) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_f_res_74164, "] out of bounds for array of shape [", (long long) (int64_t) 8, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_82949 = (int64_t) 8 * m_f_res_74164;
        
        if (mem_82950_cached_sizze_84833 < bytes_82949) {
            err = lexical_realloc(ctx, &mem_82950, &mem_82950_cached_sizze_84833, bytes_82949);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_82950, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82930, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_f_res_74164});
        for (int64_t write_iter_81768 = 0; write_iter_81768 < (int64_t) 8; write_iter_81768++) {
            int64_t write_iv_81770 = ((int64_t *) mem_82929)[write_iter_81768];
            int64_t write_iv_81771 = ((int64_t *) mem_82928)[write_iter_81768];
            int64_t write_iv_81772 = ((int64_t *) mem_82930)[write_iter_81768];
            bool cond_78486 = write_iv_81770 == (int64_t) 1;
            int64_t lifted_lambda_res_78487;
            
            if (cond_78486) {
                int64_t lifted_lambda_res_t_res_80973 = sub64(write_iv_81771, (int64_t) 1);
                
                lifted_lambda_res_78487 = lifted_lambda_res_t_res_80973;
            } else {
                lifted_lambda_res_78487 = (int64_t) -1;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_78487) && slt64(lifted_lambda_res_78487, m_f_res_74164)) {
                ((int64_t *) mem_82950)[lifted_lambda_res_78487] = write_iv_81772;
            }
        }
        
        int64_t conc_tmp_74185 = loop_dz2086Uz2084U_73922 + m_f_res_74164;
        int64_t bytes_82957 = (int64_t) 8 * conc_tmp_74185;
        
        if (mem_82958_cached_sizze_84834 < bytes_82957) {
            err = lexical_realloc(ctx, &mem_82958, &mem_82958_cached_sizze_84834, bytes_82957);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t tmp_offs_84552 = (int64_t) 0;
        
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_82958, tmp_offs_84552, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_82898.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {loop_dz2086Uz2084U_73922});
        tmp_offs_84552 += loop_dz2086Uz2084U_73922;
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_82958, tmp_offs_84552, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82950, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_f_res_74164});
        tmp_offs_84552 += m_f_res_74164;
        if (mem_82960_cached_sizze_84835 < bytes_82957) {
            err = lexical_realloc(ctx, &mem_82960, &mem_82960_cached_sizze_84835, bytes_82957);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_82962_cached_sizze_84836 < bytes_82957) {
            err = lexical_realloc(ctx, &mem_82962, &mem_82962_cached_sizze_84836, bytes_82957);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_81782;
        int64_t scanacc_81776 = (int64_t) 0;
        
        for (int64_t i_81779 = 0; i_81779 < conc_tmp_74185; i_81779++) {
            bool index_concat_cmp_82591 = sle64(loop_dz2086Uz2084U_73922, i_81779);
            int64_t index_concat_branch_82595;
            
            if (index_concat_cmp_82591) {
                int64_t index_concat_i_82592 = sub64(i_81779, loop_dz2086Uz2084U_73922);
                int64_t index_concat_82593 = ((int64_t *) mem_82950)[index_concat_i_82592];
                
                index_concat_branch_82595 = index_concat_82593;
            } else {
                int64_t index_concat_82594 = ((int64_t *) mem_param_82898.mem)[i_81779];
                
                index_concat_branch_82595 = index_concat_82594;
            }
            
            bool x_78474 = sle64((int64_t) 0, index_concat_branch_82595);
            bool y_78475 = slt64(index_concat_branch_82595, (int64_t) 25);
            bool bounds_check_78476 = x_78474 && y_78475;
            bool index_certs_78477;
            
            if (!bounds_check_78476) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) index_concat_branch_82595, "] out of bounds for array of shape [", (long long) (int64_t) 25, "].", "-> #0  ../../ftDBSCAN_plus.fut:352:53-75\n   #1  /prelude/soacs.fut:255:31-32\n   #2  /prelude/soacs.fut:255:48-50\n   #3  /prelude/functional.fut:9:44-45\n   #4  test_dbscan_plus.fut:14:23-19:30\n   #5  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool tmp_78478 = ((bool *) mem_82923.mem)[index_concat_branch_82595];
            bool lifted_lambda_res_78479 = !tmp_78478;
            int64_t defunc_0_f_res_78480 = btoi_bool_i64(lifted_lambda_res_78479);
            int64_t defunc_0_op_res_74200 = add64(defunc_0_f_res_78480, scanacc_81776);
            
            ((int64_t *) mem_82960)[i_81779] = defunc_0_op_res_74200;
            ((int64_t *) mem_82962)[i_81779] = defunc_0_f_res_78480;
            
            int64_t scanacc_tmp_84553 = defunc_0_op_res_74200;
            
            scanacc_81776 = scanacc_tmp_84553;
        }
        discard_81782 = scanacc_81776;
        
        bool cond_74201 = conc_tmp_74185 == (int64_t) 0;
        bool x_74202 = !cond_74201;
        int64_t tmp_74203 = sub64(conc_tmp_74185, (int64_t) 1);
        bool x_74204 = sle64((int64_t) 0, tmp_74203);
        bool y_74205 = slt64(tmp_74203, conc_tmp_74185);
        bool bounds_check_74206 = x_74204 && y_74205;
        bool protect_assert_disj_74207 = cond_74201 || bounds_check_74206;
        bool index_certs_74208;
        
        if (!protect_assert_disj_74207) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_74203, "] out of bounds for array of shape [", (long long) conc_tmp_74185, "].", "-> #0  /prelude/soacs.fut:257:33-47\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t m_f_res_74209;
        
        if (x_74202) {
            int64_t x_80974 = ((int64_t *) mem_82960)[tmp_74203];
            
            m_f_res_74209 = x_80974;
        } else {
            m_f_res_74209 = (int64_t) 0;
        }
        
        int64_t m_74211;
        
        if (cond_74201) {
            m_74211 = (int64_t) 0;
        } else {
            m_74211 = m_f_res_74209;
        }
        
        int64_t m_74221 = sub64(m_74211, (int64_t) 1);
        bool i_p_m_t_s_leq_w_74223 = slt64(m_74221, conc_tmp_74185);
        bool zzero_leq_i_p_m_t_s_74222 = sle64((int64_t) 0, m_74221);
        bool y_74225 = zzero_leq_i_p_m_t_s_74222 && i_p_m_t_s_leq_w_74223;
        bool i_lte_j_74224 = sle64((int64_t) 0, m_74211);
        bool forwards_ok_74226 = i_lte_j_74224 && y_74225;
        bool eq_x_zz_74218 = (int64_t) 0 == m_f_res_74209;
        bool p_and_eq_x_y_74219 = x_74202 && eq_x_zz_74218;
        bool empty_slice_74220 = cond_74201 || p_and_eq_x_y_74219;
        bool ok_or_empty_74227 = empty_slice_74220 || forwards_ok_74226;
        bool index_certs_74228;
        
        if (!ok_or_empty_74227) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_74211, "] out of bounds for array of shape [", (long long) conc_tmp_74185, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_82975 = (int64_t) 8 * m_74211;
        
        if (memblock_alloc(ctx, &mem_82976, bytes_82975, "mem_82976")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_82976.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82958, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74211});
        for (int64_t write_iter_81783 = 0; write_iter_81783 < conc_tmp_74185; write_iter_81783++) {
            int64_t write_iv_81785 = ((int64_t *) mem_82962)[write_iter_81783];
            int64_t write_iv_81786 = ((int64_t *) mem_82960)[write_iter_81783];
            bool index_concat_cmp_82585 = sle64(loop_dz2086Uz2084U_73922, write_iter_81783);
            int64_t index_concat_branch_82589;
            
            if (index_concat_cmp_82585) {
                int64_t index_concat_i_82586 = sub64(write_iter_81783, loop_dz2086Uz2084U_73922);
                int64_t index_concat_82587 = ((int64_t *) mem_82950)[index_concat_i_82586];
                
                index_concat_branch_82589 = index_concat_82587;
            } else {
                int64_t index_concat_82588 = ((int64_t *) mem_param_82898.mem)[write_iter_81783];
                
                index_concat_branch_82589 = index_concat_82588;
            }
            
            bool cond_78468 = write_iv_81785 == (int64_t) 1;
            int64_t lifted_lambda_res_78469;
            
            if (cond_78468) {
                int64_t lifted_lambda_res_t_res_80975 = sub64(write_iv_81786, (int64_t) 1);
                
                lifted_lambda_res_78469 = lifted_lambda_res_t_res_80975;
            } else {
                lifted_lambda_res_78469 = (int64_t) -1;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_78469) && slt64(lifted_lambda_res_78469, m_74211)) {
                ((int64_t *) mem_82976.mem)[lifted_lambda_res_78469] = index_concat_branch_82589;
            }
        }
        if (mem_82984_cached_sizze_84837 < bytes_82918) {
            err = lexical_realloc(ctx, &mem_82984, &mem_82984_cached_sizze_84837, bytes_82918);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_82986_cached_sizze_84838 < bytes_82918) {
            err = lexical_realloc(ctx, &mem_82986, &mem_82986_cached_sizze_84838, bytes_82918);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (memblock_alloc(ctx, &mem_82987, loopres_73979, "mem_82987")) {
            err = 1;
            goto cleanup;
        }
        
        int64_t discard_81804;
        int64_t scanacc_81796 = (int64_t) 0;
        
        for (int64_t i_81800 = 0; i_81800 < loopres_73979; i_81800++) {
            double eta_p_78287 = ((double *) ext_mem_82921.mem)[i_81800];
            double eta_p_78288 = ((double *) ext_mem_82920.mem)[i_81800];
            bool defunc_0_reduce_res_81076;
            bool redout_81789 = 0;
            
            for (int64_t i_81790 = 0; i_81790 < (int64_t) 8; i_81790++) {
                int64_t eta_p_78792 = ((int64_t *) mem_82922)[i_81790];
                bool cond_78793 = slt64(eta_p_78792, (int64_t) 0);
                int64_t nxid_78794 = smod64(eta_p_78792, (int64_t) 5);
                int64_t nyid_78795 = sdiv64(eta_p_78792, (int64_t) 5);
                double i64_res_78796 = sitofp_i64_f64(nxid_78794);
                double zt_res_78797 = step_x_73349 * i64_res_78796;
                double zp_res_78798 = zt_res_78797 + defunc_0_reduce_res_81672;
                double i64_res_78799 = sitofp_i64_f64(nyid_78795);
                double zt_res_78800 = step_y_73351 * i64_res_78799;
                double zp_res_78801 = zt_res_78800 + defunc_0_reduce_res_81674;
                double zp_res_78802 = step_x_73349 + zp_res_78798;
                double zp_res_78803 = step_y_73351 + zp_res_78801;
                bool zgze_res_78804 = zp_res_78801 <= eta_p_78288;
                bool zlze_res_78805 = eta_p_78288 <= zp_res_78803;
                bool x_78806 = zgze_res_78804 && zlze_res_78805;
                bool zeze_res_78807 = eta_p_78287 == zp_res_78798;
                double d_euclidean_res_78808;
                
                if (zeze_res_78807) {
                    double zm_res_80976 = eta_p_78288 - eta_p_78288;
                    double abs_res_80977 = fabs64(zm_res_80976);
                    
                    d_euclidean_res_78808 = abs_res_80977;
                } else {
                    double zm_res_78811 = eta_p_78287 - zp_res_78798;
                    double abs_res_78812 = fabs64(zm_res_78811);
                    
                    d_euclidean_res_78808 = abs_res_78812;
                }
                
                bool zlze_res_78813 = d_euclidean_res_78808 <= 4.0;
                bool zeze_res_78814 = eta_p_78287 == zp_res_78802;
                double d_euclidean_res_78815;
                
                if (zeze_res_78814) {
                    double zm_res_80978 = eta_p_78288 - eta_p_78288;
                    double abs_res_80979 = fabs64(zm_res_80978);
                    
                    d_euclidean_res_78815 = abs_res_80979;
                } else {
                    double zm_res_78818 = eta_p_78287 - zp_res_78802;
                    double abs_res_78819 = fabs64(zm_res_78818);
                    
                    d_euclidean_res_78815 = abs_res_78819;
                }
                
                bool zlze_res_78820 = d_euclidean_res_78815 <= 4.0;
                bool x_78821 = !zlze_res_78813;
                bool y_78822 = zlze_res_78820 && x_78821;
                bool cond_t_res_78823 = zlze_res_78813 || y_78822;
                bool x_78824 = x_78806 && cond_t_res_78823;
                bool zgze_res_78825 = zp_res_78798 <= eta_p_78287;
                bool zlze_res_78826 = eta_p_78287 <= zp_res_78802;
                bool x_78827 = zgze_res_78825 && zlze_res_78826;
                double zm_res_78828 = eta_p_78288 - zp_res_78801;
                double abs_res_78829 = fabs64(zm_res_78828);
                bool zlze_res_78830 = abs_res_78829 <= 4.0;
                double zm_res_78831 = eta_p_78288 - zp_res_78803;
                double abs_res_78832 = fabs64(zm_res_78831);
                bool zlze_res_78833 = abs_res_78832 <= 4.0;
                bool x_78834 = !zlze_res_78830;
                bool y_78835 = zlze_res_78833 && x_78834;
                bool cond_f_res_t_res_78836 = zlze_res_78830 || y_78835;
                bool x_78837 = x_78827 && cond_f_res_t_res_78836;
                bool x_78838 = !x_78824;
                bool y_78839 = x_78837 && x_78838;
                bool cond_78840 = x_78824 || y_78839;
                bool zlze_res_78841 = eta_p_78287 <= zp_res_78798;
                double corn_x_78842;
                
                if (zlze_res_78841) {
                    corn_x_78842 = zp_res_78798;
                } else {
                    corn_x_78842 = zp_res_78802;
                }
                
                bool zlze_res_78843 = eta_p_78288 <= zp_res_78801;
                double corn_y_78844;
                
                if (zlze_res_78843) {
                    corn_y_78844 = zp_res_78801;
                } else {
                    corn_y_78844 = zp_res_78803;
                }
                
                bool p_and_eq_x_y_78845 = zeze_res_78807 && zlze_res_78841;
                bool not_p_78846 = !zlze_res_78841;
                bool p_and_eq_x_y_78847 = zeze_res_78814 && not_p_78846;
                bool zeze_res_78848 = p_and_eq_x_y_78845 || p_and_eq_x_y_78847;
                double d_euclidean_res_78849;
                
                if (zeze_res_78848) {
                    double zm_res_80980 = eta_p_78288 - corn_y_78844;
                    double abs_res_80981 = fabs64(zm_res_80980);
                    
                    d_euclidean_res_78849 = abs_res_80981;
                } else {
                    bool eq_x_y_78852 = eta_p_78288 == zp_res_78801;
                    bool eq_x_zz_78853 = eta_p_78288 == zp_res_78803;
                    bool p_and_eq_x_y_78854 = zlze_res_78843 && eq_x_y_78852;
                    bool not_p_78855 = !zlze_res_78843;
                    bool p_and_eq_x_y_78856 = eq_x_zz_78853 && not_p_78855;
                    bool zeze_res_78857 = p_and_eq_x_y_78854 || p_and_eq_x_y_78856;
                    double d_euclidean_res_f_res_78858;
                    
                    if (zeze_res_78857) {
                        double zm_res_80982 = eta_p_78287 - corn_x_78842;
                        double abs_res_80983 = fabs64(zm_res_80982);
                        
                        d_euclidean_res_f_res_78858 = abs_res_80983;
                    } else {
                        double zm_res_78861 = eta_p_78288 - corn_y_78844;
                        double zm_res_78862 = eta_p_78287 - corn_x_78842;
                        double hypot_res_78863 = futrts_hypot64(zm_res_78862, zm_res_78861);
                        
                        d_euclidean_res_f_res_78858 = hypot_res_78863;
                    }
                    d_euclidean_res_78849 = d_euclidean_res_f_res_78858;
                }
                
                bool zlze_res_78864 = d_euclidean_res_78849 <= 4.0;
                bool x_78865 = !cond_78840;
                bool y_78866 = zlze_res_78864 && x_78865;
                bool is_pt_marginal_res_78867 = cond_78840 || y_78866;
                bool x_78868 = !cond_78793;
                bool y_78869 = is_pt_marginal_res_78867 && x_78868;
                bool defunc_0_op_res_78374 = y_78869 || redout_81789;
                bool redout_tmp_84561 = defunc_0_op_res_78374;
                
                redout_81789 = redout_tmp_84561;
            }
            defunc_0_reduce_res_81076 = redout_81789;
            
            bool defunc_0_f_res_78376;
            
            if (defunc_0_reduce_res_81076) {
                bool defunc_0_reduce_res_80993;
                bool redout_81791 = 0;
                
                for (int64_t i_81792 = 0; i_81792 < (int64_t) 8; i_81792++) {
                    int64_t eta_p_80994 = ((int64_t *) mem_82922)[i_81792];
                    bool cond_80995 = slt64(eta_p_80994, (int64_t) 0);
                    int64_t nxid_80996 = smod64(eta_p_80994, (int64_t) 5);
                    int64_t nyid_80997 = sdiv64(eta_p_80994, (int64_t) 5);
                    double i64_res_80998 = sitofp_i64_f64(nxid_80996);
                    double zt_res_80999 = step_x_73349 * i64_res_80998;
                    double zp_res_81000 = zt_res_80999 + defunc_0_reduce_res_81672;
                    double i64_res_81001 = sitofp_i64_f64(nyid_80997);
                    double zt_res_81002 = step_y_73351 * i64_res_81001;
                    double zp_res_81003 = zt_res_81002 + defunc_0_reduce_res_81674;
                    double zp_res_81004 = step_x_73349 + zp_res_81000;
                    double zp_res_81005 = step_y_73351 + zp_res_81003;
                    bool zgze_res_81006 = zp_res_81003 <= eta_p_78288;
                    bool zlze_res_81007 = eta_p_78288 <= zp_res_81005;
                    bool x_81008 = zgze_res_81006 && zlze_res_81007;
                    bool zeze_res_81009 = eta_p_78287 == zp_res_81000;
                    double d_euclidean_res_81010;
                    
                    if (zeze_res_81009) {
                        double zm_res_81011 = eta_p_78288 - eta_p_78288;
                        double abs_res_81012 = fabs64(zm_res_81011);
                        
                        d_euclidean_res_81010 = abs_res_81012;
                    } else {
                        double zm_res_81013 = eta_p_78287 - zp_res_81000;
                        double abs_res_81014 = fabs64(zm_res_81013);
                        
                        d_euclidean_res_81010 = abs_res_81014;
                    }
                    
                    bool zlze_res_81015 = d_euclidean_res_81010 <= 2.0;
                    bool zeze_res_81016 = eta_p_78287 == zp_res_81004;
                    double d_euclidean_res_81017;
                    
                    if (zeze_res_81016) {
                        double zm_res_81018 = eta_p_78288 - eta_p_78288;
                        double abs_res_81019 = fabs64(zm_res_81018);
                        
                        d_euclidean_res_81017 = abs_res_81019;
                    } else {
                        double zm_res_81020 = eta_p_78287 - zp_res_81004;
                        double abs_res_81021 = fabs64(zm_res_81020);
                        
                        d_euclidean_res_81017 = abs_res_81021;
                    }
                    
                    bool zlze_res_81022 = d_euclidean_res_81017 <= 2.0;
                    bool x_81023 = !zlze_res_81015;
                    bool y_81024 = zlze_res_81022 && x_81023;
                    bool cond_t_res_81025 = zlze_res_81015 || y_81024;
                    bool x_81026 = x_81008 && cond_t_res_81025;
                    bool zgze_res_81027 = zp_res_81000 <= eta_p_78287;
                    bool zlze_res_81028 = eta_p_78287 <= zp_res_81004;
                    bool x_81029 = zgze_res_81027 && zlze_res_81028;
                    double zm_res_81030 = eta_p_78288 - zp_res_81003;
                    double abs_res_81031 = fabs64(zm_res_81030);
                    bool zlze_res_81032 = abs_res_81031 <= 2.0;
                    double zm_res_81033 = eta_p_78288 - zp_res_81005;
                    double abs_res_81034 = fabs64(zm_res_81033);
                    bool zlze_res_81035 = abs_res_81034 <= 2.0;
                    bool x_81036 = !zlze_res_81032;
                    bool y_81037 = zlze_res_81035 && x_81036;
                    bool cond_f_res_t_res_81038 = zlze_res_81032 || y_81037;
                    bool x_81039 = x_81029 && cond_f_res_t_res_81038;
                    bool x_81040 = !x_81026;
                    bool y_81041 = x_81039 && x_81040;
                    bool cond_81042 = x_81026 || y_81041;
                    bool zlze_res_81043 = eta_p_78287 <= zp_res_81000;
                    double corn_x_81044;
                    
                    if (zlze_res_81043) {
                        corn_x_81044 = zp_res_81000;
                    } else {
                        corn_x_81044 = zp_res_81004;
                    }
                    
                    bool zlze_res_81045 = eta_p_78288 <= zp_res_81003;
                    double corn_y_81046;
                    
                    if (zlze_res_81045) {
                        corn_y_81046 = zp_res_81003;
                    } else {
                        corn_y_81046 = zp_res_81005;
                    }
                    
                    bool p_and_eq_x_y_81047 = zeze_res_81009 && zlze_res_81043;
                    bool not_p_81048 = !zlze_res_81043;
                    bool p_and_eq_x_y_81049 = zeze_res_81016 && not_p_81048;
                    bool zeze_res_81050 = p_and_eq_x_y_81047 || p_and_eq_x_y_81049;
                    double d_euclidean_res_81051;
                    
                    if (zeze_res_81050) {
                        double zm_res_81052 = eta_p_78288 - corn_y_81046;
                        double abs_res_81053 = fabs64(zm_res_81052);
                        
                        d_euclidean_res_81051 = abs_res_81053;
                    } else {
                        bool eq_x_y_81054 = eta_p_78288 == zp_res_81003;
                        bool eq_x_zz_81055 = eta_p_78288 == zp_res_81005;
                        bool p_and_eq_x_y_81056 = zlze_res_81045 && eq_x_y_81054;
                        bool not_p_81057 = !zlze_res_81045;
                        bool p_and_eq_x_y_81058 = eq_x_zz_81055 && not_p_81057;
                        bool zeze_res_81059 = p_and_eq_x_y_81056 || p_and_eq_x_y_81058;
                        double d_euclidean_res_f_res_81060;
                        
                        if (zeze_res_81059) {
                            double zm_res_81061 = eta_p_78287 - corn_x_81044;
                            double abs_res_81062 = fabs64(zm_res_81061);
                            
                            d_euclidean_res_f_res_81060 = abs_res_81062;
                        } else {
                            double zm_res_81063 = eta_p_78288 - corn_y_81046;
                            double zm_res_81064 = eta_p_78287 - corn_x_81044;
                            double hypot_res_81065 = futrts_hypot64(zm_res_81064, zm_res_81063);
                            
                            d_euclidean_res_f_res_81060 = hypot_res_81065;
                        }
                        d_euclidean_res_81051 = d_euclidean_res_f_res_81060;
                    }
                    
                    bool zlze_res_81066 = d_euclidean_res_81051 <= 2.0;
                    bool x_81067 = !cond_81042;
                    bool y_81068 = zlze_res_81066 && x_81067;
                    bool is_pt_marginal_res_81069 = cond_81042 || y_81068;
                    bool x_81070 = !cond_80995;
                    bool y_81071 = is_pt_marginal_res_81069 && x_81070;
                    bool defunc_0_op_res_81074 = y_81071 || redout_81791;
                    bool redout_tmp_84562 = defunc_0_op_res_81074;
                    
                    redout_81791 = redout_tmp_84562;
                }
                defunc_0_reduce_res_80993 = redout_81791;
                defunc_0_f_res_78376 = defunc_0_reduce_res_80993;
            } else {
                defunc_0_f_res_78376 = 0;
            }
            
            int64_t defunc_0_f_res_78462 = btoi_bool_i64(defunc_0_f_res_78376);
            int64_t defunc_0_op_res_74732 = add64(defunc_0_f_res_78462, scanacc_81796);
            
            ((int64_t *) mem_82984)[i_81800] = defunc_0_op_res_74732;
            ((int64_t *) mem_82986)[i_81800] = defunc_0_f_res_78462;
            ((bool *) mem_82987.mem)[i_81800] = defunc_0_reduce_res_81076;
            
            int64_t scanacc_tmp_84557 = defunc_0_op_res_74732;
            
            scanacc_81796 = scanacc_tmp_84557;
        }
        discard_81804 = scanacc_81796;
        
        int64_t bytes_83006 = (int64_t) 8 * loop_dz2087Uz2080U_73923;
        
        if (mem_83007_cached_sizze_84839 < bytes_83006) {
            err = lexical_realloc(ctx, &mem_83007, &mem_83007_cached_sizze_84839, bytes_83006);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83009_cached_sizze_84840 < bytes_83006) {
            err = lexical_realloc(ctx, &mem_83009, &mem_83009_cached_sizze_84840, bytes_83006);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83011_cached_sizze_84841 < bytes_83006) {
            err = lexical_realloc(ctx, &mem_83011, &mem_83011_cached_sizze_84841, bytes_83006);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_81817;
        int64_t discard_81818;
        int64_t scanacc_81808;
        int64_t scanacc_81809;
        
        scanacc_81808 = (int64_t) 0;
        scanacc_81809 = (int64_t) 0;
        for (int64_t i_81813 = 0; i_81813 < loop_dz2087Uz2080U_73923; i_81813++) {
            bool x_78181 = ((bool *) mem_param_82913.mem)[ctx_param_ext_82911 + i_81813 * ctx_param_ext_82912];
            bool cond_neg_78182 = !x_78181;
            int64_t defunc_0_lifted_pzq_res_78183 = btoi_bool_i64(cond_neg_78182);
            bool is_i_78184 = defunc_0_lifted_pzq_res_78183 == (int64_t) 0;
            int64_t part_res_78185 = btoi_bool_i64(is_i_78184);
            int64_t part_res_78186;
            int64_t part_res_78187;
            
            if (is_i_78184) {
                part_res_78186 = (int64_t) 0;
                part_res_78187 = (int64_t) 0;
            } else {
                bool is_i_78188 = defunc_0_lifted_pzq_res_78183 == (int64_t) 1;
                int64_t part_res_78189;
                
                if (is_i_78188) {
                    part_res_78189 = (int64_t) 1;
                } else {
                    part_res_78189 = (int64_t) 2;
                }
                
                int64_t part_res_78190 = btoi_bool_i64(is_i_78188);
                
                part_res_78186 = part_res_78189;
                part_res_78187 = part_res_78190;
            }
            
            int64_t zz_74431 = part_res_78185 + scanacc_81808;
            int64_t zz_74432 = part_res_78187 + scanacc_81809;
            
            ((int64_t *) mem_83007)[i_81813] = zz_74431;
            ((int64_t *) mem_83009)[i_81813] = zz_74432;
            ((int64_t *) mem_83011)[i_81813] = part_res_78186;
            
            int64_t scanacc_tmp_84563 = zz_74431;
            int64_t scanacc_tmp_84564 = zz_74432;
            
            scanacc_81808 = scanacc_tmp_84563;
            scanacc_81809 = scanacc_tmp_84564;
        }
        discard_81817 = scanacc_81808;
        discard_81818 = scanacc_81809;
        
        int64_t last_index_74433 = loop_dz2087Uz2080U_73923 - (int64_t) 1;
        bool is_empty_74434 = loop_dz2087Uz2080U_73923 == (int64_t) 0;
        bool x_74435 = !is_empty_74434;
        int64_t last_offset_74436;
        
        if (x_74435) {
            int64_t x_81077 = ((int64_t *) mem_83007)[last_index_74433];
            
            last_offset_74436 = x_81077;
        } else {
            last_offset_74436 = (int64_t) 0;
        }
        
        int64_t partition_sizze_74438;
        
        if (is_empty_74434) {
            partition_sizze_74438 = (int64_t) 0;
        } else {
            partition_sizze_74438 = last_offset_74436;
        }
        if (mem_83031_cached_sizze_84842 < bytes_83006) {
            err = lexical_realloc(ctx, &mem_83031, &mem_83031_cached_sizze_84842, bytes_83006);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83033_cached_sizze_84843 < bytes_83006) {
            err = lexical_realloc(ctx, &mem_83033, &mem_83033_cached_sizze_84843, bytes_83006);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83034_cached_sizze_84844 < loop_dz2087Uz2080U_73923) {
            err = lexical_realloc(ctx, &mem_83034, &mem_83034_cached_sizze_84844, loop_dz2087Uz2080U_73923);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83036_cached_sizze_84845 < bytes_83006) {
            err = lexical_realloc(ctx, &mem_83036, &mem_83036_cached_sizze_84845, bytes_83006);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t y_74443 = (int64_t) -1 + partition_sizze_74438;
        
        for (int64_t write_iter_81819 = 0; write_iter_81819 < loop_dz2087Uz2080U_73923; write_iter_81819++) {
            int64_t write_iv_81824 = ((int64_t *) mem_83011)[write_iter_81819];
            int64_t write_iv_81825 = ((int64_t *) mem_83007)[write_iter_81819];
            int64_t write_iv_81826 = ((int64_t *) mem_83009)[write_iter_81819];
            double write_iv_81827 = ((double *) mem_param_82904.mem)[write_iter_81819];
            double write_iv_81828 = ((double *) mem_param_82907.mem)[write_iter_81819];
            bool write_iv_81829 = ((bool *) mem_param_82910.mem)[write_iter_81819];
            int64_t write_iv_81830 = ((int64_t *) mem_param_82901.mem)[write_iter_81819];
            bool is_this_one_74455 = write_iv_81824 == (int64_t) 0;
            bool is_this_one_74456 = write_iv_81824 == (int64_t) 1;
            int64_t this_offset_74457 = y_74443 + write_iv_81826;
            int64_t total_res_74458;
            
            if (is_this_one_74456) {
                total_res_74458 = this_offset_74457;
            } else {
                total_res_74458 = (int64_t) -1;
            }
            
            int64_t this_offset_74459 = (int64_t) -1 + write_iv_81825;
            int64_t total_res_74460;
            
            if (is_this_one_74455) {
                total_res_74460 = this_offset_74459;
            } else {
                total_res_74460 = total_res_74458;
            }
            if (sle64((int64_t) 0, total_res_74460) && slt64(total_res_74460, loop_dz2087Uz2080U_73923)) {
                ((double *) mem_83031)[total_res_74460] = write_iv_81827;
            }
            if (sle64((int64_t) 0, total_res_74460) && slt64(total_res_74460, loop_dz2087Uz2080U_73923)) {
                ((double *) mem_83033)[total_res_74460] = write_iv_81828;
            }
            if (sle64((int64_t) 0, total_res_74460) && slt64(total_res_74460, loop_dz2087Uz2080U_73923)) {
                ((bool *) mem_83034)[total_res_74460] = write_iv_81829;
            }
            if (sle64((int64_t) 0, total_res_74460) && slt64(total_res_74460, loop_dz2087Uz2080U_73923)) {
                ((int64_t *) mem_83036)[total_res_74460] = write_iv_81830;
            }
        }
        
        bool eq_x_zz_74461 = (int64_t) 0 == last_offset_74436;
        bool p_and_eq_x_y_74462 = x_74435 && eq_x_zz_74461;
        bool empty_slice_74463 = is_empty_74434 || p_and_eq_x_y_74462;
        int64_t m_74464 = sub64(partition_sizze_74438, (int64_t) 1);
        bool zzero_leq_i_p_m_t_s_74465 = sle64((int64_t) 0, m_74464);
        bool i_p_m_t_s_leq_w_74466 = slt64(m_74464, loop_dz2087Uz2080U_73923);
        bool i_lte_j_74467 = sle64((int64_t) 0, partition_sizze_74438);
        bool y_74468 = zzero_leq_i_p_m_t_s_74465 && i_p_m_t_s_leq_w_74466;
        bool forwards_ok_74469 = i_lte_j_74467 && y_74468;
        bool ok_or_empty_74470 = empty_slice_74463 || forwards_ok_74469;
        bool index_certs_74471;
        
        if (!ok_or_empty_74470) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) partition_sizze_74438, "] out of bounds for array of shape [", (long long) loop_dz2087Uz2080U_73923, "].", "-> #0  /prelude/soacs.fut:174:7-19\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t j_m_i_74472 = sub64(loop_dz2087Uz2080U_73923, partition_sizze_74438);
        bool empty_slice_74473 = j_m_i_74472 == (int64_t) 0;
        int64_t m_74474 = sub64(j_m_i_74472, (int64_t) 1);
        int64_t i_p_m_t_s_74475 = add64(partition_sizze_74438, m_74474);
        bool zzero_leq_i_p_m_t_s_74476 = sle64((int64_t) 0, i_p_m_t_s_74475);
        bool i_p_m_t_s_leq_w_74477 = slt64(i_p_m_t_s_74475, loop_dz2087Uz2080U_73923);
        bool i_lte_j_74478 = sle64(partition_sizze_74438, loop_dz2087Uz2080U_73923);
        bool y_74479 = zzero_leq_i_p_m_t_s_74476 && i_p_m_t_s_leq_w_74477;
        bool forwards_ok_74480 = i_lte_j_74478 && y_74479;
        bool ok_or_empty_74481 = empty_slice_74473 || forwards_ok_74480;
        bool index_certs_74482;
        
        if (!ok_or_empty_74481) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) partition_sizze_74438, ":", (long long) loop_dz2087Uz2080U_73923, "] out of bounds for array of shape [", (long long) loop_dz2087Uz2080U_73923, "].", "-> #0  /prelude/soacs.fut:174:21-33\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74486 = add64(loop_dz2086Uz2082U_73921, partition_sizze_74438);
        int64_t conc_tmp_74487 = loop_dz2086Uz2080U_73919 + partition_sizze_74438;
        int64_t conc_tmp_74488 = loop_dz2086Uz2081U_73920 + partition_sizze_74438;
        bool cond_74490 = slt64((int64_t) 0, loop_dz2086Uz2084U_73922);
        int64_t bytes_83061 = (int64_t) 8 * dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74486;
        
        if (mem_83062_cached_sizze_84846 < bytes_83061) {
            err = lexical_realloc(ctx, &mem_83062, &mem_83062_cached_sizze_84846, bytes_83061);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83064_cached_sizze_84847 < bytes_83061) {
            err = lexical_realloc(ctx, &mem_83064, &mem_83064_cached_sizze_84847, bytes_83061);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83066_cached_sizze_84848 < bytes_83061) {
            err = lexical_realloc(ctx, &mem_83066, &mem_83066_cached_sizze_84848, bytes_83061);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_81847;
        int64_t discard_81848;
        int64_t scanacc_81838;
        int64_t scanacc_81839;
        
        scanacc_81838 = (int64_t) 0;
        scanacc_81839 = (int64_t) 0;
        for (int64_t i_81843 = 0; i_81843 < dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74486; i_81843++) {
            bool index_concat_cmp_78067 = sle64(loop_dz2086Uz2082U_73921, i_81843);
            double index_concat_branch_78068;
            
            if (index_concat_cmp_78067) {
                int64_t index_concat_i_81078 = sub64(i_81843, loop_dz2086Uz2082U_73921);
                double index_concat_81079 = ((double *) mem_83031)[index_concat_i_81078];
                
                index_concat_branch_78068 = index_concat_81079;
            } else {
                double index_concat_78071 = ((double *) mem_param_82889.mem)[i_81843];
                
                index_concat_branch_78068 = index_concat_78071;
            }
            
            double index_concat_branch_78072;
            
            if (index_concat_cmp_78067) {
                int64_t index_concat_i_81080 = sub64(i_81843, loop_dz2086Uz2082U_73921);
                double index_concat_81081 = ((double *) mem_83033)[index_concat_i_81080];
                
                index_concat_branch_78072 = index_concat_81081;
            } else {
                double index_concat_78075 = ((double *) mem_param_82892.mem)[i_81843];
                
                index_concat_branch_78072 = index_concat_78075;
            }
            
            bool isNearAnyRelevant_78076;
            int64_t isNearAnyRelevant_78077;
            bool isNearAnyRelevant_78078;
            bool loop_while_78079;
            int64_t curPart_i_78080;
            bool isNear_78081;
            
            loop_while_78079 = cond_74490;
            curPart_i_78080 = (int64_t) 0;
            isNear_78081 = 0;
            while (loop_while_78079) {
                bool x_78082 = sle64((int64_t) 0, curPart_i_78080);
                bool y_78083 = slt64(curPart_i_78080, loop_dz2086Uz2084U_73922);
                bool bounds_check_78084 = x_78082 && y_78083;
                bool index_certs_78085;
                
                if (!bounds_check_78084) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) curPart_i_78080, "] out of bounds for array of shape [", (long long) loop_dz2086Uz2084U_73922, "].", "-> #0  ../../ftDBSCAN_plus.fut:418:79-110\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                int64_t curPart_78086 = ((int64_t *) mem_param_82898.mem)[curPart_i_78080];
                int64_t cur_xid_78087 = smod64(curPart_78086, (int64_t) 5);
                int64_t cur_yid_78088 = sdiv64(curPart_78086, (int64_t) 5);
                double i64_res_78089 = sitofp_i64_f64(cur_xid_78087);
                double zt_res_78090 = step_x_73349 * i64_res_78089;
                double zp_res_78091 = zt_res_78090 + defunc_0_reduce_res_81672;
                double i64_res_78092 = sitofp_i64_f64(cur_yid_78088);
                double zt_res_78093 = step_y_73351 * i64_res_78092;
                double zp_res_78094 = zt_res_78093 + defunc_0_reduce_res_81674;
                double zp_res_78095 = step_x_73349 + zp_res_78091;
                double zp_res_78096 = step_y_73351 + zp_res_78094;
                bool zgze_res_78097 = zp_res_78094 <= index_concat_branch_78072;
                bool zlze_res_78098 = index_concat_branch_78072 <= zp_res_78096;
                bool x_78099 = zgze_res_78097 && zlze_res_78098;
                bool zeze_res_78100 = index_concat_branch_78068 == zp_res_78091;
                double d_euclidean_res_78101;
                
                if (zeze_res_78100) {
                    double zm_res_81082 = index_concat_branch_78072 - index_concat_branch_78072;
                    double abs_res_81083 = fabs64(zm_res_81082);
                    
                    d_euclidean_res_78101 = abs_res_81083;
                } else {
                    double zm_res_78104 = index_concat_branch_78068 - zp_res_78091;
                    double abs_res_78105 = fabs64(zm_res_78104);
                    
                    d_euclidean_res_78101 = abs_res_78105;
                }
                
                bool zlze_res_78106 = d_euclidean_res_78101 <= 4.0;
                bool zeze_res_78107 = index_concat_branch_78068 == zp_res_78095;
                double d_euclidean_res_78108;
                
                if (zeze_res_78107) {
                    double zm_res_81084 = index_concat_branch_78072 - index_concat_branch_78072;
                    double abs_res_81085 = fabs64(zm_res_81084);
                    
                    d_euclidean_res_78108 = abs_res_81085;
                } else {
                    double zm_res_78111 = index_concat_branch_78068 - zp_res_78095;
                    double abs_res_78112 = fabs64(zm_res_78111);
                    
                    d_euclidean_res_78108 = abs_res_78112;
                }
                
                bool zlze_res_78113 = d_euclidean_res_78108 <= 4.0;
                bool x_78114 = !zlze_res_78106;
                bool y_78115 = zlze_res_78113 && x_78114;
                bool cond_t_res_78116 = zlze_res_78106 || y_78115;
                bool x_78117 = x_78099 && cond_t_res_78116;
                bool zgze_res_78118 = zp_res_78091 <= index_concat_branch_78068;
                bool zlze_res_78119 = index_concat_branch_78068 <= zp_res_78095;
                bool x_78120 = zgze_res_78118 && zlze_res_78119;
                double zm_res_78121 = index_concat_branch_78072 - zp_res_78094;
                double abs_res_78122 = fabs64(zm_res_78121);
                bool zlze_res_78123 = abs_res_78122 <= 4.0;
                double zm_res_78124 = index_concat_branch_78072 - zp_res_78096;
                double abs_res_78125 = fabs64(zm_res_78124);
                bool zlze_res_78126 = abs_res_78125 <= 4.0;
                bool x_78127 = !zlze_res_78123;
                bool y_78128 = zlze_res_78126 && x_78127;
                bool cond_f_res_t_res_78129 = zlze_res_78123 || y_78128;
                bool x_78130 = x_78120 && cond_f_res_t_res_78129;
                bool x_78131 = !x_78117;
                bool y_78132 = x_78130 && x_78131;
                bool cond_78133 = x_78117 || y_78132;
                bool zlze_res_78134 = index_concat_branch_78068 <= zp_res_78091;
                double corn_x_78135;
                
                if (zlze_res_78134) {
                    corn_x_78135 = zp_res_78091;
                } else {
                    corn_x_78135 = zp_res_78095;
                }
                
                bool zlze_res_78136 = index_concat_branch_78072 <= zp_res_78094;
                double corn_y_78137;
                
                if (zlze_res_78136) {
                    corn_y_78137 = zp_res_78094;
                } else {
                    corn_y_78137 = zp_res_78096;
                }
                
                bool p_and_eq_x_y_78138 = zeze_res_78100 && zlze_res_78134;
                bool not_p_78139 = !zlze_res_78134;
                bool p_and_eq_x_y_78140 = zeze_res_78107 && not_p_78139;
                bool zeze_res_78141 = p_and_eq_x_y_78138 || p_and_eq_x_y_78140;
                double d_euclidean_res_78142;
                
                if (zeze_res_78141) {
                    double zm_res_81086 = index_concat_branch_78072 - corn_y_78137;
                    double abs_res_81087 = fabs64(zm_res_81086);
                    
                    d_euclidean_res_78142 = abs_res_81087;
                } else {
                    bool eq_x_y_78145 = index_concat_branch_78072 == zp_res_78094;
                    bool eq_x_zz_78146 = index_concat_branch_78072 == zp_res_78096;
                    bool p_and_eq_x_y_78147 = zlze_res_78136 && eq_x_y_78145;
                    bool not_p_78148 = !zlze_res_78136;
                    bool p_and_eq_x_y_78149 = eq_x_zz_78146 && not_p_78148;
                    bool zeze_res_78150 = p_and_eq_x_y_78147 || p_and_eq_x_y_78149;
                    double d_euclidean_res_f_res_78151;
                    
                    if (zeze_res_78150) {
                        double zm_res_81088 = index_concat_branch_78068 - corn_x_78135;
                        double abs_res_81089 = fabs64(zm_res_81088);
                        
                        d_euclidean_res_f_res_78151 = abs_res_81089;
                    } else {
                        double zm_res_78154 = index_concat_branch_78072 - corn_y_78137;
                        double zm_res_78155 = index_concat_branch_78068 - corn_x_78135;
                        double hypot_res_78156 = futrts_hypot64(zm_res_78155, zm_res_78154);
                        
                        d_euclidean_res_f_res_78151 = hypot_res_78156;
                    }
                    d_euclidean_res_78142 = d_euclidean_res_f_res_78151;
                }
                
                bool zlze_res_78157 = d_euclidean_res_78142 <= 4.0;
                bool x_78158 = !cond_78133;
                bool y_78159 = zlze_res_78157 && x_78158;
                bool is_pt_marginal_res_78160 = cond_78133 || y_78159;
                int64_t tmp_78161 = add64((int64_t) 1, curPart_i_78080);
                bool cond_78162 = slt64(tmp_78161, loop_dz2086Uz2084U_73922);
                bool loop_cond_t_res_78163 = !is_pt_marginal_res_78160;
                bool x_78164 = cond_78162 && loop_cond_t_res_78163;
                bool loop_while_tmp_84577 = x_78164;
                int64_t curPart_i_tmp_84578 = tmp_78161;
                bool isNear_tmp_84579 = is_pt_marginal_res_78160;
                
                loop_while_78079 = loop_while_tmp_84577;
                curPart_i_78080 = curPart_i_tmp_84578;
                isNear_78081 = isNear_tmp_84579;
            }
            isNearAnyRelevant_78076 = loop_while_78079;
            isNearAnyRelevant_78077 = curPart_i_78080;
            isNearAnyRelevant_78078 = isNear_78081;
            
            bool cond_neg_78166 = !isNearAnyRelevant_78078;
            int64_t defunc_0_lifted_pzq_res_78167 = btoi_bool_i64(cond_neg_78166);
            bool is_i_78168 = defunc_0_lifted_pzq_res_78167 == (int64_t) 0;
            int64_t part_res_78169 = btoi_bool_i64(is_i_78168);
            int64_t part_res_78170;
            int64_t part_res_78171;
            
            if (is_i_78168) {
                part_res_78170 = (int64_t) 0;
                part_res_78171 = (int64_t) 0;
            } else {
                bool is_i_78172 = defunc_0_lifted_pzq_res_78167 == (int64_t) 1;
                int64_t part_res_78173;
                
                if (is_i_78172) {
                    part_res_78173 = (int64_t) 1;
                } else {
                    part_res_78173 = (int64_t) 2;
                }
                
                int64_t part_res_78174 = btoi_bool_i64(is_i_78172);
                
                part_res_78170 = part_res_78173;
                part_res_78171 = part_res_78174;
            }
            
            int64_t zz_74616 = part_res_78169 + scanacc_81838;
            int64_t zz_74617 = part_res_78171 + scanacc_81839;
            
            ((int64_t *) mem_83062)[i_81843] = zz_74616;
            ((int64_t *) mem_83064)[i_81843] = zz_74617;
            ((int64_t *) mem_83066)[i_81843] = part_res_78170;
            
            int64_t scanacc_tmp_84572 = zz_74616;
            int64_t scanacc_tmp_84573 = zz_74617;
            
            scanacc_81838 = scanacc_tmp_84572;
            scanacc_81839 = scanacc_tmp_84573;
        }
        discard_81847 = scanacc_81838;
        discard_81848 = scanacc_81839;
        
        int64_t last_index_74618 = dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74486 - (int64_t) 1;
        bool is_empty_74619 = dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74486 == (int64_t) 0;
        bool x_74620 = !is_empty_74619;
        int64_t last_offset_74621;
        
        if (x_74620) {
            int64_t x_81090 = ((int64_t *) mem_83062)[last_index_74618];
            
            last_offset_74621 = x_81090;
        } else {
            last_offset_74621 = (int64_t) 0;
        }
        
        int64_t partition_sizze_74623;
        
        if (is_empty_74619) {
            partition_sizze_74623 = (int64_t) 0;
        } else {
            partition_sizze_74623 = last_offset_74621;
        }
        if (mem_83086_cached_sizze_84849 < bytes_83061) {
            err = lexical_realloc(ctx, &mem_83086, &mem_83086_cached_sizze_84849, bytes_83061);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t y_74625 = (int64_t) -1 + partition_sizze_74623;
        
        for (int64_t write_iter_81849 = 0; write_iter_81849 < dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74486; write_iter_81849++) {
            int64_t write_iv_81851 = ((int64_t *) mem_83066)[write_iter_81849];
            int64_t write_iv_81852 = ((int64_t *) mem_83062)[write_iter_81849];
            int64_t write_iv_81853 = ((int64_t *) mem_83064)[write_iter_81849];
            bool is_this_one_74631 = write_iv_81851 == (int64_t) 0;
            bool is_this_one_74632 = write_iv_81851 == (int64_t) 1;
            int64_t this_offset_74633 = y_74625 + write_iv_81853;
            int64_t total_res_74634;
            
            if (is_this_one_74632) {
                total_res_74634 = this_offset_74633;
            } else {
                total_res_74634 = (int64_t) -1;
            }
            
            int64_t this_offset_74635 = (int64_t) -1 + write_iv_81852;
            int64_t total_res_74636;
            
            if (is_this_one_74631) {
                total_res_74636 = this_offset_74635;
            } else {
                total_res_74636 = total_res_74634;
            }
            if (sle64((int64_t) 0, total_res_74636) && slt64(total_res_74636, dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74486)) {
                ((int64_t *) mem_83086)[total_res_74636] = write_iter_81849;
            }
        }
        
        bool eq_x_zz_74637 = (int64_t) 0 == last_offset_74621;
        bool p_and_eq_x_y_74638 = x_74620 && eq_x_zz_74637;
        bool empty_slice_74639 = is_empty_74619 || p_and_eq_x_y_74638;
        int64_t m_74640 = sub64(partition_sizze_74623, (int64_t) 1);
        bool zzero_leq_i_p_m_t_s_74641 = sle64((int64_t) 0, m_74640);
        bool i_p_m_t_s_leq_w_74642 = slt64(m_74640, dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74486);
        bool i_lte_j_74643 = sle64((int64_t) 0, partition_sizze_74623);
        bool y_74644 = zzero_leq_i_p_m_t_s_74641 && i_p_m_t_s_leq_w_74642;
        bool forwards_ok_74645 = i_lte_j_74643 && y_74644;
        bool ok_or_empty_74646 = empty_slice_74639 || forwards_ok_74645;
        bool index_certs_74647;
        
        if (!ok_or_empty_74646) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) partition_sizze_74623, "] out of bounds for array of shape [", (long long) dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74486, "].", "-> #0  /prelude/soacs.fut:174:7-19\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_83093 = (int64_t) 8 * partition_sizze_74623;
        
        if (mem_83094_cached_sizze_84850 < bytes_83093) {
            err = lexical_realloc(ctx, &mem_83094, &mem_83094_cached_sizze_84850, bytes_83093);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83096_cached_sizze_84851 < bytes_83093) {
            err = lexical_realloc(ctx, &mem_83096, &mem_83096_cached_sizze_84851, bytes_83093);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (memblock_alloc(ctx, &mem_83097, partition_sizze_74623, "mem_83097")) {
            err = 1;
            goto cleanup;
        }
        if (memblock_alloc(ctx, &mem_83099, bytes_83093, "mem_83099")) {
            err = 1;
            goto cleanup;
        }
        for (int64_t i_81864 = 0; i_81864 < partition_sizze_74623; i_81864++) {
            int64_t eta_p_74665 = ((int64_t *) mem_83086)[i_81864];
            bool x_74666 = sle64((int64_t) 0, eta_p_74665);
            bool y_74667 = slt64(eta_p_74665, dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74486);
            bool bounds_check_74668 = x_74666 && y_74667;
            bool index_certs_74669;
            
            if (!bounds_check_74668) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_74665, "] out of bounds for array of shape [", (long long) dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74486, "].", "-> #0  ../../ftDBSCAN_plus.fut:435:60-72\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../ftDBSCAN_plus.fut:435:44-102\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool y_74679 = slt64(eta_p_74665, conc_tmp_74487);
            bool bounds_check_74680 = x_74666 && y_74679;
            bool index_certs_74681;
            
            if (!bounds_check_74680) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_74665, "] out of bounds for array of shape [", (long long) conc_tmp_74487, "].", "-> #0  ../../ftDBSCAN_plus.fut:435:74-86\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../ftDBSCAN_plus.fut:435:44-102\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool y_74687 = slt64(eta_p_74665, conc_tmp_74488);
            bool bounds_check_74688 = x_74666 && y_74687;
            bool index_certs_74689;
            
            if (!bounds_check_74688) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_74665, "] out of bounds for array of shape [", (long long) conc_tmp_74488, "].", "-> #0  ../../ftDBSCAN_plus.fut:435:88-100\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../ftDBSCAN_plus.fut:435:44-102\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool index_concat_cmp_74670 = sle64(loop_dz2086Uz2082U_73921, eta_p_74665);
            double index_concat_branch_74671;
            
            if (index_concat_cmp_74670) {
                int64_t index_concat_i_81091 = sub64(eta_p_74665, loop_dz2086Uz2082U_73921);
                double index_concat_81092 = ((double *) mem_83031)[index_concat_i_81091];
                
                index_concat_branch_74671 = index_concat_81092;
            } else {
                double index_concat_74674 = ((double *) mem_param_82889.mem)[eta_p_74665];
                
                index_concat_branch_74671 = index_concat_74674;
            }
            
            double index_concat_branch_74675;
            
            if (index_concat_cmp_74670) {
                int64_t index_concat_i_81093 = sub64(eta_p_74665, loop_dz2086Uz2082U_73921);
                double index_concat_81094 = ((double *) mem_83033)[index_concat_i_81093];
                
                index_concat_branch_74675 = index_concat_81094;
            } else {
                double index_concat_74678 = ((double *) mem_param_82892.mem)[eta_p_74665];
                
                index_concat_branch_74675 = index_concat_74678;
            }
            
            bool index_concat_cmp_74682 = sle64(loop_dz2086Uz2080U_73919, eta_p_74665);
            bool index_concat_branch_74683;
            
            if (index_concat_cmp_74682) {
                int64_t index_concat_i_81095 = sub64(eta_p_74665, loop_dz2086Uz2080U_73919);
                bool index_concat_81096 = ((bool *) mem_83034)[index_concat_i_81095];
                
                index_concat_branch_74683 = index_concat_81096;
            } else {
                bool index_concat_74686 = ((bool *) mem_param_82883.mem)[eta_p_74665];
                
                index_concat_branch_74683 = index_concat_74686;
            }
            
            bool index_concat_cmp_74690 = sle64(loop_dz2086Uz2081U_73920, eta_p_74665);
            int64_t index_concat_branch_74691;
            
            if (index_concat_cmp_74690) {
                int64_t index_concat_i_81097 = sub64(eta_p_74665, loop_dz2086Uz2081U_73920);
                int64_t index_concat_81098 = ((int64_t *) mem_83036)[index_concat_i_81097];
                
                index_concat_branch_74691 = index_concat_81098;
            } else {
                int64_t index_concat_74694 = ((int64_t *) mem_param_82886.mem)[eta_p_74665];
                
                index_concat_branch_74691 = index_concat_74694;
            }
            ((double *) mem_83094)[i_81864] = index_concat_branch_74671;
            ((double *) mem_83096)[i_81864] = index_concat_branch_74675;
            ((bool *) mem_83097.mem)[i_81864] = index_concat_branch_74683;
            ((int64_t *) mem_83099.mem)[i_81864] = index_concat_branch_74691;
        }
        
        int64_t j_m_i_74649 = sub64(dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74486, partition_sizze_74623);
        bool empty_slice_74650 = j_m_i_74649 == (int64_t) 0;
        int64_t m_74651 = sub64(j_m_i_74649, (int64_t) 1);
        int64_t i_p_m_t_s_74652 = add64(partition_sizze_74623, m_74651);
        bool zzero_leq_i_p_m_t_s_74653 = sle64((int64_t) 0, i_p_m_t_s_74652);
        bool i_p_m_t_s_leq_w_74654 = slt64(i_p_m_t_s_74652, dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74486);
        bool i_lte_j_74655 = sle64(partition_sizze_74623, dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74486);
        bool y_74656 = zzero_leq_i_p_m_t_s_74653 && i_p_m_t_s_leq_w_74654;
        bool forwards_ok_74657 = i_lte_j_74655 && y_74656;
        bool ok_or_empty_74658 = empty_slice_74650 || forwards_ok_74657;
        bool index_certs_74659;
        
        if (!ok_or_empty_74658) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) partition_sizze_74623, ":", (long long) dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74486, "] out of bounds for array of shape [", (long long) dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74486, "].", "-> #0  /prelude/soacs.fut:174:21-33\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_83124 = (int64_t) 8 * j_m_i_74649;
        int64_t conc_tmp_74722 = j_m_i_74472 + j_m_i_74649;
        int64_t new_proc_74723 = add64((int64_t) 1, old_clHandler_73939);
        bool x_74733 = !eq_x_y_73992;
        int64_t tmp_74734 = sub64(loopres_73979, (int64_t) 1);
        bool x_74735 = sle64((int64_t) 0, tmp_74734);
        bool y_74736 = slt64(tmp_74734, loopres_73979);
        bool bounds_check_74737 = x_74735 && y_74736;
        bool protect_assert_disj_74738 = eq_x_y_73992 || bounds_check_74737;
        bool index_certs_74739;
        
        if (!protect_assert_disj_74738) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_74734, "] out of bounds for array of shape [", (long long) loopres_73979, "].", "-> #0  /prelude/soacs.fut:257:33-47\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t m_f_res_74740;
        
        if (x_74733) {
            int64_t x_81105 = ((int64_t *) mem_82984)[tmp_74734];
            
            m_f_res_74740 = x_81105;
        } else {
            m_f_res_74740 = (int64_t) 0;
        }
        
        int64_t m_74742;
        
        if (eq_x_y_73992) {
            m_74742 = (int64_t) 0;
        } else {
            m_74742 = m_f_res_74740;
        }
        
        int64_t m_74752 = sub64(m_74742, (int64_t) 1);
        bool i_p_m_t_s_leq_w_74754 = slt64(m_74752, loopres_73979);
        bool zzero_leq_i_p_m_t_s_74753 = sle64((int64_t) 0, m_74752);
        bool y_74756 = zzero_leq_i_p_m_t_s_74753 && i_p_m_t_s_leq_w_74754;
        bool i_lte_j_74755 = sle64((int64_t) 0, m_74742);
        bool forwards_ok_74757 = i_lte_j_74755 && y_74756;
        bool eq_x_zz_74749 = (int64_t) 0 == m_f_res_74740;
        bool p_and_eq_x_y_74750 = x_74733 && eq_x_zz_74749;
        bool empty_slice_74751 = eq_x_y_73992 || p_and_eq_x_y_74750;
        bool ok_or_empty_74758 = empty_slice_74751 || forwards_ok_74757;
        bool index_certs_74759;
        
        if (!ok_or_empty_74758) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_74742, "] out of bounds for array of shape [", (long long) loopres_73979, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_83152 = (int64_t) 8 * m_74742;
        
        if (mem_83153_cached_sizze_84855 < bytes_83152) {
            err = lexical_realloc(ctx, &mem_83153, &mem_83153_cached_sizze_84855, bytes_83152);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83153, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_82920.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74742});
        if (mem_83155_cached_sizze_84856 < bytes_83152) {
            err = lexical_realloc(ctx, &mem_83155, &mem_83155_cached_sizze_84856, bytes_83152);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83155, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_82921.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74742});
        if (mem_83157_cached_sizze_84857 < bytes_83152) {
            err = lexical_realloc(ctx, &mem_83157, &mem_83157_cached_sizze_84857, bytes_83152);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t i_84585 = 0; i_84585 < m_74742; i_84585++) {
            int64_t x_84586 = (int64_t) 0 + i_84585 * (int64_t) 1;
            
            ((int64_t *) mem_83157)[i_84585] = x_84586;
        }
        for (int64_t write_iter_81879 = 0; write_iter_81879 < loopres_73979; write_iter_81879++) {
            int64_t write_iv_81883 = ((int64_t *) mem_82986)[write_iter_81879];
            int64_t write_iv_81884 = ((int64_t *) mem_82984)[write_iter_81879];
            double write_iv_81886 = ((double *) ext_mem_82921.mem)[write_iter_81879];
            double write_iv_81887 = ((double *) ext_mem_82920.mem)[write_iter_81879];
            bool cond_78037 = write_iv_81883 == (int64_t) 1;
            int64_t lifted_lambda_res_78038;
            
            if (cond_78037) {
                int64_t lifted_lambda_res_t_res_81106 = sub64(write_iv_81884, (int64_t) 1);
                
                lifted_lambda_res_78038 = lifted_lambda_res_t_res_81106;
            } else {
                lifted_lambda_res_78038 = (int64_t) -1;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_78038) && slt64(lifted_lambda_res_78038, m_74742)) {
                ((int64_t *) mem_83157)[lifted_lambda_res_78038] = write_iter_81879;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_78038) && slt64(lifted_lambda_res_78038, m_74742)) {
                ((double *) mem_83155)[lifted_lambda_res_78038] = write_iv_81886;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_78038) && slt64(lifted_lambda_res_78038, m_74742)) {
                ((double *) mem_83153)[lifted_lambda_res_78038] = write_iv_81887;
            }
        }
        
        bool not_p_74787 = !cond_74044;
        bool not_p_74786 = !cond_74034;
        
        if (mem_83177_cached_sizze_84858 < bytes_83093) {
            err = lexical_realloc(ctx, &mem_83177, &mem_83177_cached_sizze_84858, bytes_83093);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83179_cached_sizze_84859 < bytes_83093) {
            err = lexical_realloc(ctx, &mem_83179, &mem_83179_cached_sizze_84859, bytes_83093);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83181_cached_sizze_84860 < bytes_83093) {
            err = lexical_realloc(ctx, &mem_83181, &mem_83181_cached_sizze_84860, bytes_83093);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83183_cached_sizze_84861 < bytes_83093) {
            err = lexical_realloc(ctx, &mem_83183, &mem_83183_cached_sizze_84861, bytes_83093);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83184_cached_sizze_84862 < partition_sizze_74623) {
            err = lexical_realloc(ctx, &mem_83184, &mem_83184_cached_sizze_84862, partition_sizze_74623);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83186_cached_sizze_84863 < bytes_83093) {
            err = lexical_realloc(ctx, &mem_83186, &mem_83186_cached_sizze_84863, bytes_83093);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_81911;
        int64_t scanacc_81897 = (int64_t) 0;
        
        for (int64_t i_81904 = 0; i_81904 < partition_sizze_74623; i_81904++) {
            double lifted_lambda_res_77939 = ((double *) mem_83094)[i_81904];
            double lifted_lambda_res_77940 = ((double *) mem_83096)[i_81904];
            bool tmp_77941 = ((bool *) mem_83097.mem)[i_81904];
            int64_t tmp_77942 = ((int64_t *) mem_83099.mem)[i_81904];
            bool zgze_res_77947 = minC_74043 <= lifted_lambda_res_77940;
            bool zlze_res_77948 = lifted_lambda_res_77940 <= maxC_74054;
            bool x_77949 = zgze_res_77947 && zlze_res_77948;
            bool eq_x_y_77950 = lifted_lambda_res_77939 == -INFINITY;
            bool eq_x_zz_77951 = lifted_lambda_res_77939 == zp_res_74037;
            bool p_and_eq_x_y_77952 = cond_74034 && eq_x_y_77950;
            bool p_and_eq_x_y_77953 = not_p_74786 && eq_x_zz_77951;
            bool zeze_res_77954 = p_and_eq_x_y_77952 || p_and_eq_x_y_77953;
            double d_euclidean_res_77955;
            
            if (zeze_res_77954) {
                double zm_res_81107 = lifted_lambda_res_77940 - lifted_lambda_res_77940;
                double abs_res_81108 = fabs64(zm_res_81107);
                
                d_euclidean_res_77955 = abs_res_81108;
            } else {
                double zm_res_77958 = lifted_lambda_res_77939 - minC_74038;
                double abs_res_77959 = fabs64(zm_res_77958);
                
                d_euclidean_res_77955 = abs_res_77959;
            }
            
            bool zlze_res_77960 = d_euclidean_res_77955 <= 4.0;
            bool eq_x_y_77961 = lifted_lambda_res_77939 == INFINITY;
            bool eq_x_zz_77962 = lifted_lambda_res_77939 == zp_res_74048;
            bool p_and_eq_x_y_77963 = cond_74044 && eq_x_y_77961;
            bool p_and_eq_x_y_77964 = not_p_74787 && eq_x_zz_77962;
            bool zeze_res_77965 = p_and_eq_x_y_77963 || p_and_eq_x_y_77964;
            double d_euclidean_res_77966;
            
            if (zeze_res_77965) {
                double zm_res_81109 = lifted_lambda_res_77940 - lifted_lambda_res_77940;
                double abs_res_81110 = fabs64(zm_res_81109);
                
                d_euclidean_res_77966 = abs_res_81110;
            } else {
                double zm_res_77969 = lifted_lambda_res_77939 - maxC_74049;
                double abs_res_77970 = fabs64(zm_res_77969);
                
                d_euclidean_res_77966 = abs_res_77970;
            }
            
            bool zlze_res_77971 = d_euclidean_res_77966 <= 4.0;
            bool x_77972 = !zlze_res_77960;
            bool y_77973 = zlze_res_77971 && x_77972;
            bool cond_t_res_77974 = zlze_res_77960 || y_77973;
            bool x_77975 = x_77949 && cond_t_res_77974;
            bool zgze_res_77976 = minC_74038 <= lifted_lambda_res_77939;
            bool zlze_res_77977 = lifted_lambda_res_77939 <= maxC_74049;
            bool x_77978 = zgze_res_77976 && zlze_res_77977;
            double zm_res_77979 = lifted_lambda_res_77940 - minC_74043;
            double abs_res_77980 = fabs64(zm_res_77979);
            bool zlze_res_77981 = abs_res_77980 <= 4.0;
            double zm_res_77982 = lifted_lambda_res_77940 - maxC_74054;
            double abs_res_77983 = fabs64(zm_res_77982);
            bool zlze_res_77984 = abs_res_77983 <= 4.0;
            bool x_77985 = !zlze_res_77981;
            bool y_77986 = zlze_res_77984 && x_77985;
            bool cond_f_res_t_res_77987 = zlze_res_77981 || y_77986;
            bool x_77988 = x_77978 && cond_f_res_t_res_77987;
            bool x_77989 = !x_77975;
            bool y_77990 = x_77988 && x_77989;
            bool cond_77991 = x_77975 || y_77990;
            bool zlze_res_77992 = lifted_lambda_res_77939 <= minC_74038;
            double corn_x_77993;
            
            if (zlze_res_77992) {
                corn_x_77993 = minC_74038;
            } else {
                corn_x_77993 = maxC_74049;
            }
            
            bool zlze_res_77994 = lifted_lambda_res_77940 <= minC_74043;
            double corn_y_77995;
            
            if (zlze_res_77994) {
                corn_y_77995 = minC_74043;
            } else {
                corn_y_77995 = maxC_74054;
            }
            
            bool p_and_eq_x_y_77996 = zeze_res_77954 && zlze_res_77992;
            bool not_p_77997 = !zlze_res_77992;
            bool p_and_eq_x_y_77998 = zeze_res_77965 && not_p_77997;
            bool zeze_res_77999 = p_and_eq_x_y_77996 || p_and_eq_x_y_77998;
            double d_euclidean_res_78000;
            
            if (zeze_res_77999) {
                double zm_res_81111 = lifted_lambda_res_77940 - corn_y_77995;
                double abs_res_81112 = fabs64(zm_res_81111);
                
                d_euclidean_res_78000 = abs_res_81112;
            } else {
                bool eq_x_y_78003 = lifted_lambda_res_77940 == -INFINITY;
                bool eq_x_zz_78004 = lifted_lambda_res_77940 == zp_res_74042;
                bool p_and_eq_x_y_78005 = cond_74039 && eq_x_y_78003;
                bool not_p_78006 = !cond_74039;
                bool p_and_eq_x_y_78007 = eq_x_zz_78004 && not_p_78006;
                bool eq_x_y_78008 = p_and_eq_x_y_78005 || p_and_eq_x_y_78007;
                bool eq_x_y_78009 = lifted_lambda_res_77940 == INFINITY;
                bool eq_x_zz_78010 = lifted_lambda_res_77940 == zp_res_74053;
                bool p_and_eq_x_y_78011 = cond_74050 && eq_x_y_78009;
                bool not_p_78012 = !cond_74050;
                bool p_and_eq_x_y_78013 = eq_x_zz_78010 && not_p_78012;
                bool eq_x_zz_78014 = p_and_eq_x_y_78011 || p_and_eq_x_y_78013;
                bool p_and_eq_x_y_78015 = zlze_res_77994 && eq_x_y_78008;
                bool not_p_78016 = !zlze_res_77994;
                bool p_and_eq_x_y_78017 = eq_x_zz_78014 && not_p_78016;
                bool zeze_res_78018 = p_and_eq_x_y_78015 || p_and_eq_x_y_78017;
                double d_euclidean_res_f_res_78019;
                
                if (zeze_res_78018) {
                    double zm_res_81113 = lifted_lambda_res_77939 - corn_x_77993;
                    double abs_res_81114 = fabs64(zm_res_81113);
                    
                    d_euclidean_res_f_res_78019 = abs_res_81114;
                } else {
                    double zm_res_78022 = lifted_lambda_res_77940 - corn_y_77995;
                    double zm_res_78023 = lifted_lambda_res_77939 - corn_x_77993;
                    double hypot_res_78024 = futrts_hypot64(zm_res_78023, zm_res_78022);
                    
                    d_euclidean_res_f_res_78019 = hypot_res_78024;
                }
                d_euclidean_res_78000 = d_euclidean_res_f_res_78019;
            }
            
            bool zlze_res_78025 = d_euclidean_res_78000 <= 4.0;
            bool x_78026 = !cond_77991;
            bool y_78027 = zlze_res_78025 && x_78026;
            bool is_pt_marginal_res_78028 = cond_77991 || y_78027;
            int64_t defunc_0_f_res_78029 = btoi_bool_i64(is_pt_marginal_res_78028);
            int64_t defunc_0_op_res_74878 = add64(defunc_0_f_res_78029, scanacc_81897);
            
            ((int64_t *) mem_83177)[i_81904] = defunc_0_op_res_74878;
            ((int64_t *) mem_83179)[i_81904] = defunc_0_f_res_78029;
            ((double *) mem_83181)[i_81904] = lifted_lambda_res_77939;
            ((double *) mem_83183)[i_81904] = lifted_lambda_res_77940;
            ((bool *) mem_83184)[i_81904] = tmp_77941;
            ((int64_t *) mem_83186)[i_81904] = tmp_77942;
            
            int64_t scanacc_tmp_84590 = defunc_0_op_res_74878;
            
            scanacc_81897 = scanacc_tmp_84590;
        }
        discard_81911 = scanacc_81897;
        
        bool x_74881 = !empty_slice_74639;
        bool y_74882 = slt64(m_74640, partition_sizze_74623);
        bool bounds_check_74883 = zzero_leq_i_p_m_t_s_74641 && y_74882;
        bool protect_assert_disj_74884 = empty_slice_74639 || bounds_check_74883;
        bool index_certs_74885;
        
        if (!protect_assert_disj_74884) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) m_74640, "] out of bounds for array of shape [", (long long) partition_sizze_74623, "].", "-> #0  /prelude/soacs.fut:257:33-47\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t m_f_res_74886;
        
        if (x_74881) {
            int64_t x_81115 = ((int64_t *) mem_83177)[m_74640];
            
            m_f_res_74886 = x_81115;
        } else {
            m_f_res_74886 = (int64_t) 0;
        }
        
        int64_t m_74888;
        
        if (empty_slice_74639) {
            m_74888 = (int64_t) 0;
        } else {
            m_74888 = m_f_res_74886;
        }
        
        int64_t m_74898 = sub64(m_74888, (int64_t) 1);
        bool i_p_m_t_s_leq_w_74900 = slt64(m_74898, partition_sizze_74623);
        bool zzero_leq_i_p_m_t_s_74899 = sle64((int64_t) 0, m_74898);
        bool y_74902 = zzero_leq_i_p_m_t_s_74899 && i_p_m_t_s_leq_w_74900;
        bool i_lte_j_74901 = sle64((int64_t) 0, m_74888);
        bool forwards_ok_74903 = i_lte_j_74901 && y_74902;
        bool eq_x_zz_74895 = (int64_t) 0 == m_f_res_74886;
        bool p_and_eq_x_y_74896 = x_74881 && eq_x_zz_74895;
        bool empty_slice_74897 = empty_slice_74639 || p_and_eq_x_y_74896;
        bool ok_or_empty_74904 = empty_slice_74897 || forwards_ok_74903;
        bool index_certs_74905;
        
        if (!ok_or_empty_74904) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_74888, "] out of bounds for array of shape [", (long long) partition_sizze_74623, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_83223 = (int64_t) 8 * m_74888;
        
        if (mem_83224_cached_sizze_84864 < bytes_83223) {
            err = lexical_realloc(ctx, &mem_83224, &mem_83224_cached_sizze_84864, bytes_83223);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t i_84597 = 0; i_84597 < m_74888; i_84597++) {
            int64_t x_84598 = (int64_t) 0 + i_84597 * (int64_t) 1;
            
            ((int64_t *) mem_83224)[i_84597] = x_84598;
        }
        if (mem_83226_cached_sizze_84865 < bytes_83223) {
            err = lexical_realloc(ctx, &mem_83226, &mem_83226_cached_sizze_84865, bytes_83223);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83226, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83186, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74888});
        if (mem_83227_cached_sizze_84866 < m_74888) {
            err = lexical_realloc(ctx, &mem_83227, &mem_83227_cached_sizze_84866, m_74888);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_1b(ctx, 1, (uint8_t *) mem_83227, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint8_t *) mem_83184, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74888});
        if (mem_83229_cached_sizze_84867 < bytes_83223) {
            err = lexical_realloc(ctx, &mem_83229, &mem_83229_cached_sizze_84867, bytes_83223);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83229, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83183, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74888});
        if (mem_83231_cached_sizze_84868 < bytes_83223) {
            err = lexical_realloc(ctx, &mem_83231, &mem_83231_cached_sizze_84868, bytes_83223);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83231, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83181, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74888});
        for (int64_t write_iter_81912 = 0; write_iter_81912 < partition_sizze_74623; write_iter_81912++) {
            int64_t write_iv_81918 = ((int64_t *) mem_83179)[write_iter_81912];
            int64_t write_iv_81919 = ((int64_t *) mem_83177)[write_iter_81912];
            double write_iv_81920 = ((double *) mem_83181)[write_iter_81912];
            double write_iv_81921 = ((double *) mem_83183)[write_iter_81912];
            bool write_iv_81922 = ((bool *) mem_83184)[write_iter_81912];
            int64_t write_iv_81923 = ((int64_t *) mem_83186)[write_iter_81912];
            bool cond_77839 = write_iv_81918 == (int64_t) 1;
            int64_t lifted_lambda_res_77840;
            
            if (cond_77839) {
                int64_t lifted_lambda_res_t_res_81116 = sub64(write_iv_81919, (int64_t) 1);
                
                lifted_lambda_res_77840 = lifted_lambda_res_t_res_81116;
            } else {
                lifted_lambda_res_77840 = (int64_t) -1;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77840) && slt64(lifted_lambda_res_77840, m_74888)) {
                ((double *) mem_83231)[lifted_lambda_res_77840] = write_iv_81920;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77840) && slt64(lifted_lambda_res_77840, m_74888)) {
                ((double *) mem_83229)[lifted_lambda_res_77840] = write_iv_81921;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77840) && slt64(lifted_lambda_res_77840, m_74888)) {
                ((bool *) mem_83227)[lifted_lambda_res_77840] = write_iv_81922;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77840) && slt64(lifted_lambda_res_77840, m_74888)) {
                ((int64_t *) mem_83226)[lifted_lambda_res_77840] = write_iv_81923;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77840) && slt64(lifted_lambda_res_77840, m_74888)) {
                ((int64_t *) mem_83224)[lifted_lambda_res_77840] = write_iter_81912;
            }
        }
        if (mem_83263_cached_sizze_84869 < bytes_83223) {
            err = lexical_realloc(ctx, &mem_83263, &mem_83263_cached_sizze_84869, bytes_83223);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83265_cached_sizze_84870 < bytes_83223) {
            err = lexical_realloc(ctx, &mem_83265, &mem_83265_cached_sizze_84870, bytes_83223);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83267_cached_sizze_84871 < bytes_83223) {
            err = lexical_realloc(ctx, &mem_83267, &mem_83267_cached_sizze_84871, bytes_83223);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_81942;
        int64_t discard_81943;
        int64_t scanacc_81933;
        int64_t scanacc_81934;
        
        scanacc_81933 = (int64_t) 0;
        scanacc_81934 = (int64_t) 0;
        for (int64_t i_81938 = 0; i_81938 < m_74888; i_81938++) {
            double x_77735 = ((double *) mem_83231)[i_81938];
            double x_77736 = ((double *) mem_83229)[i_81938];
            bool zgze_res_77737 = minC_74043 <= x_77736;
            bool zlze_res_77738 = x_77736 <= maxC_74054;
            bool x_77739 = zgze_res_77737 && zlze_res_77738;
            bool eq_x_y_77740 = x_77735 == -INFINITY;
            bool eq_x_zz_77741 = x_77735 == zp_res_74037;
            bool p_and_eq_x_y_77742 = cond_74034 && eq_x_y_77740;
            bool p_and_eq_x_y_77743 = not_p_74786 && eq_x_zz_77741;
            bool zeze_res_77744 = p_and_eq_x_y_77742 || p_and_eq_x_y_77743;
            double d_euclidean_res_77745;
            
            if (zeze_res_77744) {
                double zm_res_81117 = x_77736 - x_77736;
                double abs_res_81118 = fabs64(zm_res_81117);
                
                d_euclidean_res_77745 = abs_res_81118;
            } else {
                double zm_res_77748 = x_77735 - minC_74038;
                double abs_res_77749 = fabs64(zm_res_77748);
                
                d_euclidean_res_77745 = abs_res_77749;
            }
            
            bool zlze_res_77750 = d_euclidean_res_77745 <= 2.0;
            bool eq_x_y_77751 = x_77735 == INFINITY;
            bool eq_x_zz_77752 = x_77735 == zp_res_74048;
            bool p_and_eq_x_y_77753 = cond_74044 && eq_x_y_77751;
            bool p_and_eq_x_y_77754 = not_p_74787 && eq_x_zz_77752;
            bool zeze_res_77755 = p_and_eq_x_y_77753 || p_and_eq_x_y_77754;
            double d_euclidean_res_77756;
            
            if (zeze_res_77755) {
                double zm_res_81119 = x_77736 - x_77736;
                double abs_res_81120 = fabs64(zm_res_81119);
                
                d_euclidean_res_77756 = abs_res_81120;
            } else {
                double zm_res_77759 = x_77735 - maxC_74049;
                double abs_res_77760 = fabs64(zm_res_77759);
                
                d_euclidean_res_77756 = abs_res_77760;
            }
            
            bool zlze_res_77761 = d_euclidean_res_77756 <= 2.0;
            bool x_77762 = !zlze_res_77750;
            bool y_77763 = zlze_res_77761 && x_77762;
            bool cond_t_res_77764 = zlze_res_77750 || y_77763;
            bool x_77765 = x_77739 && cond_t_res_77764;
            bool zgze_res_77766 = minC_74038 <= x_77735;
            bool zlze_res_77767 = x_77735 <= maxC_74049;
            bool x_77768 = zgze_res_77766 && zlze_res_77767;
            double zm_res_77769 = x_77736 - minC_74043;
            double abs_res_77770 = fabs64(zm_res_77769);
            bool zlze_res_77771 = abs_res_77770 <= 2.0;
            double zm_res_77772 = x_77736 - maxC_74054;
            double abs_res_77773 = fabs64(zm_res_77772);
            bool zlze_res_77774 = abs_res_77773 <= 2.0;
            bool x_77775 = !zlze_res_77771;
            bool y_77776 = zlze_res_77774 && x_77775;
            bool cond_f_res_t_res_77777 = zlze_res_77771 || y_77776;
            bool x_77778 = x_77768 && cond_f_res_t_res_77777;
            bool x_77779 = !x_77765;
            bool y_77780 = x_77778 && x_77779;
            bool cond_77781 = x_77765 || y_77780;
            bool zlze_res_77782 = x_77735 <= minC_74038;
            double corn_x_77783;
            
            if (zlze_res_77782) {
                corn_x_77783 = minC_74038;
            } else {
                corn_x_77783 = maxC_74049;
            }
            
            bool zlze_res_77784 = x_77736 <= minC_74043;
            double corn_y_77785;
            
            if (zlze_res_77784) {
                corn_y_77785 = minC_74043;
            } else {
                corn_y_77785 = maxC_74054;
            }
            
            bool p_and_eq_x_y_77786 = zeze_res_77744 && zlze_res_77782;
            bool not_p_77787 = !zlze_res_77782;
            bool p_and_eq_x_y_77788 = zeze_res_77755 && not_p_77787;
            bool zeze_res_77789 = p_and_eq_x_y_77786 || p_and_eq_x_y_77788;
            double d_euclidean_res_77790;
            
            if (zeze_res_77789) {
                double zm_res_81121 = x_77736 - corn_y_77785;
                double abs_res_81122 = fabs64(zm_res_81121);
                
                d_euclidean_res_77790 = abs_res_81122;
            } else {
                bool eq_x_y_77793 = x_77736 == -INFINITY;
                bool eq_x_zz_77794 = x_77736 == zp_res_74042;
                bool p_and_eq_x_y_77795 = cond_74039 && eq_x_y_77793;
                bool not_p_77796 = !cond_74039;
                bool p_and_eq_x_y_77797 = eq_x_zz_77794 && not_p_77796;
                bool eq_x_y_77798 = p_and_eq_x_y_77795 || p_and_eq_x_y_77797;
                bool eq_x_y_77799 = x_77736 == INFINITY;
                bool eq_x_zz_77800 = x_77736 == zp_res_74053;
                bool p_and_eq_x_y_77801 = cond_74050 && eq_x_y_77799;
                bool not_p_77802 = !cond_74050;
                bool p_and_eq_x_y_77803 = eq_x_zz_77800 && not_p_77802;
                bool eq_x_zz_77804 = p_and_eq_x_y_77801 || p_and_eq_x_y_77803;
                bool p_and_eq_x_y_77805 = zlze_res_77784 && eq_x_y_77798;
                bool not_p_77806 = !zlze_res_77784;
                bool p_and_eq_x_y_77807 = eq_x_zz_77804 && not_p_77806;
                bool zeze_res_77808 = p_and_eq_x_y_77805 || p_and_eq_x_y_77807;
                double d_euclidean_res_f_res_77809;
                
                if (zeze_res_77808) {
                    double zm_res_81123 = x_77735 - corn_x_77783;
                    double abs_res_81124 = fabs64(zm_res_81123);
                    
                    d_euclidean_res_f_res_77809 = abs_res_81124;
                } else {
                    double zm_res_77812 = x_77736 - corn_y_77785;
                    double zm_res_77813 = x_77735 - corn_x_77783;
                    double hypot_res_77814 = futrts_hypot64(zm_res_77813, zm_res_77812);
                    
                    d_euclidean_res_f_res_77809 = hypot_res_77814;
                }
                d_euclidean_res_77790 = d_euclidean_res_f_res_77809;
            }
            
            bool zlze_res_77815 = d_euclidean_res_77790 <= 2.0;
            bool x_77816 = !cond_77781;
            bool y_77817 = zlze_res_77815 && x_77816;
            bool is_pt_marginal_res_77818 = cond_77781 || y_77817;
            bool cond_neg_77819 = !is_pt_marginal_res_77818;
            int64_t defunc_0_lifted_pzq_res_77820 = btoi_bool_i64(cond_neg_77819);
            bool is_i_77821 = defunc_0_lifted_pzq_res_77820 == (int64_t) 0;
            int64_t part_res_77822 = btoi_bool_i64(is_i_77821);
            int64_t part_res_77823;
            int64_t part_res_77824;
            
            if (is_i_77821) {
                part_res_77823 = (int64_t) 0;
                part_res_77824 = (int64_t) 0;
            } else {
                bool is_i_77825 = defunc_0_lifted_pzq_res_77820 == (int64_t) 1;
                int64_t part_res_77826;
                
                if (is_i_77825) {
                    part_res_77826 = (int64_t) 1;
                } else {
                    part_res_77826 = (int64_t) 2;
                }
                
                int64_t part_res_77827 = btoi_bool_i64(is_i_77825);
                
                part_res_77823 = part_res_77826;
                part_res_77824 = part_res_77827;
            }
            
            int64_t zz_75030 = part_res_77822 + scanacc_81933;
            int64_t zz_75031 = part_res_77824 + scanacc_81934;
            
            ((int64_t *) mem_83263)[i_81938] = zz_75030;
            ((int64_t *) mem_83265)[i_81938] = zz_75031;
            ((int64_t *) mem_83267)[i_81938] = part_res_77823;
            
            int64_t scanacc_tmp_84604 = zz_75030;
            int64_t scanacc_tmp_84605 = zz_75031;
            
            scanacc_81933 = scanacc_tmp_84604;
            scanacc_81934 = scanacc_tmp_84605;
        }
        discard_81942 = scanacc_81933;
        discard_81943 = scanacc_81934;
        
        bool x_75032 = !empty_slice_74897;
        int64_t last_offset_75033;
        
        if (x_75032) {
            int64_t x_81125 = ((int64_t *) mem_83263)[m_74898];
            
            last_offset_75033 = x_81125;
        } else {
            last_offset_75033 = (int64_t) 0;
        }
        
        int64_t partition_sizze_75035;
        
        if (empty_slice_74897) {
            partition_sizze_75035 = (int64_t) 0;
        } else {
            partition_sizze_75035 = last_offset_75033;
        }
        if (mem_83287_cached_sizze_84872 < bytes_83223) {
            err = lexical_realloc(ctx, &mem_83287, &mem_83287_cached_sizze_84872, bytes_83223);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83289_cached_sizze_84873 < bytes_83223) {
            err = lexical_realloc(ctx, &mem_83289, &mem_83289_cached_sizze_84873, bytes_83223);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83290_cached_sizze_84874 < m_74888) {
            err = lexical_realloc(ctx, &mem_83290, &mem_83290_cached_sizze_84874, m_74888);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83292_cached_sizze_84875 < bytes_83223) {
            err = lexical_realloc(ctx, &mem_83292, &mem_83292_cached_sizze_84875, bytes_83223);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83294_cached_sizze_84876 < bytes_83223) {
            err = lexical_realloc(ctx, &mem_83294, &mem_83294_cached_sizze_84876, bytes_83223);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t y_75041 = (int64_t) -1 + partition_sizze_75035;
        
        for (int64_t write_iter_81944 = 0; write_iter_81944 < m_74888; write_iter_81944++) {
            int64_t write_iv_81950 = ((int64_t *) mem_83267)[write_iter_81944];
            int64_t write_iv_81951 = ((int64_t *) mem_83263)[write_iter_81944];
            int64_t write_iv_81952 = ((int64_t *) mem_83265)[write_iter_81944];
            double write_iv_81953 = ((double *) mem_83231)[write_iter_81944];
            double write_iv_81954 = ((double *) mem_83229)[write_iter_81944];
            bool write_iv_81955 = ((bool *) mem_83227)[write_iter_81944];
            int64_t write_iv_81956 = ((int64_t *) mem_83226)[write_iter_81944];
            int64_t write_iv_81957 = ((int64_t *) mem_83224)[write_iter_81944];
            bool is_this_one_75055 = write_iv_81950 == (int64_t) 0;
            bool is_this_one_75056 = write_iv_81950 == (int64_t) 1;
            int64_t this_offset_75057 = y_75041 + write_iv_81952;
            int64_t total_res_75058;
            
            if (is_this_one_75056) {
                total_res_75058 = this_offset_75057;
            } else {
                total_res_75058 = (int64_t) -1;
            }
            
            int64_t this_offset_75059 = (int64_t) -1 + write_iv_81951;
            int64_t total_res_75060;
            
            if (is_this_one_75055) {
                total_res_75060 = this_offset_75059;
            } else {
                total_res_75060 = total_res_75058;
            }
            if (sle64((int64_t) 0, total_res_75060) && slt64(total_res_75060, m_74888)) {
                ((double *) mem_83287)[total_res_75060] = write_iv_81953;
            }
            if (sle64((int64_t) 0, total_res_75060) && slt64(total_res_75060, m_74888)) {
                ((double *) mem_83289)[total_res_75060] = write_iv_81954;
            }
            if (sle64((int64_t) 0, total_res_75060) && slt64(total_res_75060, m_74888)) {
                ((bool *) mem_83290)[total_res_75060] = write_iv_81955;
            }
            if (sle64((int64_t) 0, total_res_75060) && slt64(total_res_75060, m_74888)) {
                ((int64_t *) mem_83292)[total_res_75060] = write_iv_81956;
            }
            if (sle64((int64_t) 0, total_res_75060) && slt64(total_res_75060, m_74888)) {
                ((int64_t *) mem_83294)[total_res_75060] = write_iv_81957;
            }
        }
        
        bool eq_x_zz_75061 = (int64_t) 0 == last_offset_75033;
        bool p_and_eq_x_y_75062 = x_75032 && eq_x_zz_75061;
        bool empty_slice_75063 = empty_slice_74897 || p_and_eq_x_y_75062;
        int64_t m_75064 = sub64(partition_sizze_75035, (int64_t) 1);
        bool zzero_leq_i_p_m_t_s_75065 = sle64((int64_t) 0, m_75064);
        bool i_p_m_t_s_leq_w_75066 = slt64(m_75064, m_74888);
        bool i_lte_j_75067 = sle64((int64_t) 0, partition_sizze_75035);
        bool y_75068 = zzero_leq_i_p_m_t_s_75065 && i_p_m_t_s_leq_w_75066;
        bool forwards_ok_75069 = i_lte_j_75067 && y_75068;
        bool ok_or_empty_75070 = empty_slice_75063 || forwards_ok_75069;
        bool index_certs_75071;
        
        if (!ok_or_empty_75070) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) partition_sizze_75035, "] out of bounds for array of shape [", (long long) m_74888, "].", "-> #0  /prelude/soacs.fut:174:7-19\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        bool y_75132 = slt64(m_75064, partition_sizze_75035);
        bool bounds_check_75133 = zzero_leq_i_p_m_t_s_75065 && y_75132;
        bool protect_assert_disj_75134 = empty_slice_75063 || bounds_check_75133;
        bool index_certs_75135;
        
        if (!protect_assert_disj_75134) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) m_75064, "] out of bounds for array of shape [", (long long) partition_sizze_75035, "].", "-> #0  /prelude/soacs.fut:257:33-47\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        bool x_75131 = !empty_slice_75063;
        int64_t bytes_83325 = (int64_t) 8 * partition_sizze_75035;
        
        if (memblock_alloc(ctx, &mem_83326, bytes_83325, "mem_83326")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83326.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83287, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {partition_sizze_75035});
        if (memblock_alloc(ctx, &mem_83329, bytes_83325, "mem_83329")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83329.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83289, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {partition_sizze_75035});
        if (memblock_alloc(ctx, &mem_83332, bytes_83152, "mem_83332")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83332.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83155, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74742});
        if (memblock_alloc(ctx, &mem_83335, bytes_83152, "mem_83335")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83335.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83153, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74742});
        if (futrts_get_num_neighbours_against_12426(ctx, &ext_mem_83337, mem_83326, mem_83329, mem_83332, mem_83335, partition_sizze_75035, m_74742, 2.0, (int8_t) 0, (int8_t) 1, 0.0, (int64_t) 9223372036854775807) != 0) {
            err = 1;
            goto cleanup;
        }
        if (memblock_unref(ctx, &mem_83326, "mem_83326") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83329, "mem_83329") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83332, "mem_83332") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83335, "mem_83335") != 0)
            return 1;
        
        int64_t j_m_i_75076 = sub64(m_74888, partition_sizze_75035);
        int64_t m_75078 = sub64(j_m_i_75076, (int64_t) 1);
        int64_t i_p_m_t_s_75079 = add64(partition_sizze_75035, m_75078);
        bool i_p_m_t_s_leq_w_75081 = slt64(i_p_m_t_s_75079, m_74888);
        bool zzero_leq_i_p_m_t_s_75080 = sle64((int64_t) 0, i_p_m_t_s_75079);
        bool y_75083 = zzero_leq_i_p_m_t_s_75080 && i_p_m_t_s_leq_w_75081;
        bool i_lte_j_75082 = sle64(partition_sizze_75035, m_74888);
        bool forwards_ok_75084 = i_lte_j_75082 && y_75083;
        bool empty_slice_75077 = j_m_i_75076 == (int64_t) 0;
        bool ok_or_empty_75085 = empty_slice_75077 || forwards_ok_75084;
        bool index_certs_75086;
        
        if (!ok_or_empty_75085) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) partition_sizze_75035, ":", (long long) m_74888, "] out of bounds for array of shape [", (long long) m_74888, "].", "-> #0  /prelude/soacs.fut:174:21-33\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        if (memblock_alloc(ctx, &mem_83339, bytes_83325, "mem_83339")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83339.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83287, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {partition_sizze_75035});
        if (memblock_alloc(ctx, &mem_83342, bytes_83325, "mem_83342")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83342.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83289, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {partition_sizze_75035});
        
        int64_t bytes_83344 = (int64_t) 8 * j_m_i_75076;
        
        if (memblock_alloc(ctx, &mem_83345, bytes_83344, "mem_83345")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83345.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83287, (int64_t) 0 + (int64_t) 1 * partition_sizze_75035, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_75076});
        if (memblock_alloc(ctx, &mem_83348, bytes_83344, "mem_83348")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83348.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83289, (int64_t) 0 + (int64_t) 1 * partition_sizze_75035, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_75076});
        if (futrts_get_num_neighbours_against_12426(ctx, &ext_mem_83350, mem_83339, mem_83342, mem_83345, mem_83348, partition_sizze_75035, j_m_i_75076, 2.0, (int8_t) 0, (int8_t) 1, 0.0, (int64_t) 9223372036854775807) != 0) {
            err = 1;
            goto cleanup;
        }
        if (memblock_unref(ctx, &mem_83339, "mem_83339") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83342, "mem_83342") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83345, "mem_83345") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83348, "mem_83348") != 0)
            return 1;
        if (memblock_alloc(ctx, &mem_83352, bytes_83325, "mem_83352")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83352.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83287, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {partition_sizze_75035});
        if (memblock_alloc(ctx, &mem_83355, bytes_83325, "mem_83355")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83355.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83289, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {partition_sizze_75035});
        if (memblock_alloc(ctx, &mem_83358, bytes_83325, "mem_83358")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83358.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83287, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {partition_sizze_75035});
        if (memblock_alloc(ctx, &mem_83361, bytes_83325, "mem_83361")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83361.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83289, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {partition_sizze_75035});
        if (futrts_get_num_neighbours_against_12423(ctx, &ext_mem_83363, mem_83352, mem_83355, mem_83358, mem_83361, partition_sizze_75035, partition_sizze_75035, 2.0, (int8_t) 0, (int8_t) 1, 0.0, (int64_t) 9223372036854775807) != 0) {
            err = 1;
            goto cleanup;
        }
        if (memblock_unref(ctx, &mem_83352, "mem_83352") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83355, "mem_83355") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83358, "mem_83358") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83361, "mem_83361") != 0)
            return 1;
        if (mem_83365_cached_sizze_84877 < bytes_83325) {
            err = lexical_realloc(ctx, &mem_83365, &mem_83365_cached_sizze_84877, bytes_83325);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83367_cached_sizze_84878 < bytes_83325) {
            err = lexical_realloc(ctx, &mem_83367, &mem_83367_cached_sizze_84878, bytes_83325);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83368_cached_sizze_84879 < partition_sizze_75035) {
            err = lexical_realloc(ctx, &mem_83368, &mem_83368_cached_sizze_84879, partition_sizze_75035);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_81974;
        int64_t scanacc_81966 = (int64_t) 0;
        
        for (int64_t i_81970 = 0; i_81970 < partition_sizze_75035; i_81970++) {
            int64_t eta_p_77720 = ((int64_t *) ext_mem_83363.mem)[i_81970];
            int64_t eta_p_77721 = ((int64_t *) ext_mem_83350.mem)[i_81970];
            int64_t eta_p_77722 = ((int64_t *) ext_mem_83337.mem)[i_81970];
            int64_t lifted_lambda_res_77723 = sub64(eta_p_77720, (int64_t) 1);
            int64_t zp_lhs_77725 = add64(eta_p_77721, lifted_lambda_res_77723);
            int64_t lifted_lambda_res_77726 = add64(eta_p_77722, zp_lhs_77725);
            bool lifted_lambda_res_77728 = sle64((int64_t) 4, lifted_lambda_res_77726);
            int64_t defunc_0_f_res_77730 = btoi_bool_i64(lifted_lambda_res_77728);
            int64_t defunc_0_op_res_75130 = add64(defunc_0_f_res_77730, scanacc_81966);
            
            ((int64_t *) mem_83365)[i_81970] = defunc_0_op_res_75130;
            ((int64_t *) mem_83367)[i_81970] = defunc_0_f_res_77730;
            ((bool *) mem_83368)[i_81970] = lifted_lambda_res_77728;
            
            int64_t scanacc_tmp_84614 = defunc_0_op_res_75130;
            
            scanacc_81966 = scanacc_tmp_84614;
        }
        discard_81974 = scanacc_81966;
        if (memblock_unref(ctx, &ext_mem_83337, "ext_mem_83337") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_83350, "ext_mem_83350") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_83363, "ext_mem_83363") != 0)
            return 1;
        
        int64_t m_f_res_75136;
        
        if (x_75131) {
            int64_t x_81126 = ((int64_t *) mem_83365)[m_75064];
            
            m_f_res_75136 = x_81126;
        } else {
            m_f_res_75136 = (int64_t) 0;
        }
        
        int64_t m_75138;
        
        if (empty_slice_75063) {
            m_75138 = (int64_t) 0;
        } else {
            m_75138 = m_f_res_75136;
        }
        
        int64_t m_75148 = sub64(m_75138, (int64_t) 1);
        bool i_p_m_t_s_leq_w_75150 = slt64(m_75148, partition_sizze_75035);
        bool zzero_leq_i_p_m_t_s_75149 = sle64((int64_t) 0, m_75148);
        bool y_75152 = zzero_leq_i_p_m_t_s_75149 && i_p_m_t_s_leq_w_75150;
        bool i_lte_j_75151 = sle64((int64_t) 0, m_75138);
        bool forwards_ok_75153 = i_lte_j_75151 && y_75152;
        bool eq_x_zz_75145 = (int64_t) 0 == m_f_res_75136;
        bool p_and_eq_x_y_75146 = x_75131 && eq_x_zz_75145;
        bool empty_slice_75147 = empty_slice_75063 || p_and_eq_x_y_75146;
        bool ok_or_empty_75154 = empty_slice_75147 || forwards_ok_75153;
        bool index_certs_75155;
        
        if (!ok_or_empty_75154) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_75138, "] out of bounds for array of shape [", (long long) partition_sizze_75035, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_83387 = (int64_t) 8 * m_75138;
        
        if (mem_83388_cached_sizze_84880 < bytes_83387) {
            err = lexical_realloc(ctx, &mem_83388, &mem_83388_cached_sizze_84880, bytes_83387);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83388, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83292, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75138});
        if (mem_83390_cached_sizze_84881 < bytes_83387) {
            err = lexical_realloc(ctx, &mem_83390, &mem_83390_cached_sizze_84881, bytes_83387);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83390, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83289, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75138});
        if (mem_83392_cached_sizze_84882 < bytes_83387) {
            err = lexical_realloc(ctx, &mem_83392, &mem_83392_cached_sizze_84882, bytes_83387);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83392, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83287, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75138});
        for (int64_t write_iter_81975 = 0; write_iter_81975 < partition_sizze_75035; write_iter_81975++) {
            int64_t write_iv_81979 = ((int64_t *) mem_83367)[write_iter_81975];
            int64_t write_iv_81980 = ((int64_t *) mem_83365)[write_iter_81975];
            double write_iv_81981 = ((double *) mem_83287)[write_iter_81975];
            double write_iv_81982 = ((double *) mem_83289)[write_iter_81975];
            int64_t write_iv_81983 = ((int64_t *) mem_83292)[write_iter_81975];
            bool cond_77668 = write_iv_81979 == (int64_t) 1;
            int64_t lifted_lambda_res_77669;
            
            if (cond_77668) {
                int64_t lifted_lambda_res_t_res_81127 = sub64(write_iv_81980, (int64_t) 1);
                
                lifted_lambda_res_77669 = lifted_lambda_res_t_res_81127;
            } else {
                lifted_lambda_res_77669 = (int64_t) -1;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77669) && slt64(lifted_lambda_res_77669, m_75138)) {
                ((double *) mem_83392)[lifted_lambda_res_77669] = write_iv_81981;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77669) && slt64(lifted_lambda_res_77669, m_75138)) {
                ((double *) mem_83390)[lifted_lambda_res_77669] = write_iv_81982;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77669) && slt64(lifted_lambda_res_77669, m_75138)) {
                ((int64_t *) mem_83388)[lifted_lambda_res_77669] = write_iv_81983;
            }
        }
        if (mem_83412_cached_sizze_84883 < bytes_83344) {
            err = lexical_realloc(ctx, &mem_83412, &mem_83412_cached_sizze_84883, bytes_83344);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83414_cached_sizze_84884 < bytes_83344) {
            err = lexical_realloc(ctx, &mem_83414, &mem_83414_cached_sizze_84884, bytes_83344);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_81995;
        int64_t scanacc_81989 = (int64_t) 0;
        
        for (int64_t i_81992 = 0; i_81992 < j_m_i_75076; i_81992++) {
            int64_t slice_82572 = partition_sizze_75035 + i_81992;
            bool eta_p_77646 = ((bool *) mem_83290)[slice_82572];
            int64_t defunc_0_f_res_77647 = btoi_bool_i64(eta_p_77646);
            int64_t defunc_0_op_res_75221 = add64(defunc_0_f_res_77647, scanacc_81989);
            
            ((int64_t *) mem_83412)[i_81992] = defunc_0_op_res_75221;
            ((int64_t *) mem_83414)[i_81992] = defunc_0_f_res_77647;
            
            int64_t scanacc_tmp_84621 = defunc_0_op_res_75221;
            
            scanacc_81989 = scanacc_tmp_84621;
        }
        discard_81995 = scanacc_81989;
        
        bool y_75210 = slt64(m_75078, j_m_i_75076);
        bool x_75209 = sle64((int64_t) 0, m_75078);
        bool bounds_check_75211 = x_75209 && y_75210;
        bool protect_assert_disj_75212 = empty_slice_75077 || bounds_check_75211;
        bool index_certs_75213;
        
        if (!protect_assert_disj_75212) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) m_75078, "] out of bounds for array of shape [", (long long) j_m_i_75076, "].", "-> #0  /prelude/soacs.fut:257:33-47\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        bool x_75208 = !empty_slice_75077;
        int64_t m_f_res_75222;
        
        if (x_75208) {
            int64_t x_81128 = ((int64_t *) mem_83412)[m_75078];
            
            m_f_res_75222 = x_81128;
        } else {
            m_f_res_75222 = (int64_t) 0;
        }
        
        int64_t m_75224;
        
        if (empty_slice_75077) {
            m_75224 = (int64_t) 0;
        } else {
            m_75224 = m_f_res_75222;
        }
        
        int64_t m_75234 = sub64(m_75224, (int64_t) 1);
        bool i_p_m_t_s_leq_w_75236 = slt64(m_75234, j_m_i_75076);
        bool zzero_leq_i_p_m_t_s_75235 = sle64((int64_t) 0, m_75234);
        bool y_75238 = zzero_leq_i_p_m_t_s_75235 && i_p_m_t_s_leq_w_75236;
        bool i_lte_j_75237 = sle64((int64_t) 0, m_75224);
        bool forwards_ok_75239 = i_lte_j_75237 && y_75238;
        bool eq_x_zz_75231 = (int64_t) 0 == m_f_res_75222;
        bool p_and_eq_x_y_75232 = x_75208 && eq_x_zz_75231;
        bool empty_slice_75233 = empty_slice_75077 || p_and_eq_x_y_75232;
        bool ok_or_empty_75240 = empty_slice_75233 || forwards_ok_75239;
        bool index_certs_75241;
        
        if (!ok_or_empty_75240) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_75224, "] out of bounds for array of shape [", (long long) j_m_i_75076, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_83427 = (int64_t) 8 * m_75224;
        
        if (mem_83428_cached_sizze_84885 < bytes_83427) {
            err = lexical_realloc(ctx, &mem_83428, &mem_83428_cached_sizze_84885, bytes_83427);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83428, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83292, (int64_t) 0 + (int64_t) 1 * partition_sizze_75035, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75224});
        if (mem_83430_cached_sizze_84886 < bytes_83427) {
            err = lexical_realloc(ctx, &mem_83430, &mem_83430_cached_sizze_84886, bytes_83427);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83430, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83289, (int64_t) 0 + (int64_t) 1 * partition_sizze_75035, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75224});
        if (mem_83432_cached_sizze_84887 < bytes_83427) {
            err = lexical_realloc(ctx, &mem_83432, &mem_83432_cached_sizze_84887, bytes_83427);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83432, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83287, (int64_t) 0 + (int64_t) 1 * partition_sizze_75035, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75224});
        for (int64_t write_iter_81996 = 0; write_iter_81996 < j_m_i_75076; write_iter_81996++) {
            int64_t write_iv_82000 = ((int64_t *) mem_83414)[write_iter_81996];
            int64_t write_iv_82001 = ((int64_t *) mem_83412)[write_iter_81996];
            int64_t slice_82571 = partition_sizze_75035 + write_iter_81996;
            double write_iv_82002 = ((double *) mem_83287)[slice_82571];
            double write_iv_82003 = ((double *) mem_83289)[slice_82571];
            int64_t write_iv_82004 = ((int64_t *) mem_83292)[slice_82571];
            bool cond_77641 = write_iv_82000 == (int64_t) 1;
            int64_t lifted_lambda_res_77642;
            
            if (cond_77641) {
                int64_t lifted_lambda_res_t_res_81129 = sub64(write_iv_82001, (int64_t) 1);
                
                lifted_lambda_res_77642 = lifted_lambda_res_t_res_81129;
            } else {
                lifted_lambda_res_77642 = (int64_t) -1;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77642) && slt64(lifted_lambda_res_77642, m_75224)) {
                ((double *) mem_83432)[lifted_lambda_res_77642] = write_iv_82002;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77642) && slt64(lifted_lambda_res_77642, m_75224)) {
                ((double *) mem_83430)[lifted_lambda_res_77642] = write_iv_82003;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77642) && slt64(lifted_lambda_res_77642, m_75224)) {
                ((int64_t *) mem_83428)[lifted_lambda_res_77642] = write_iv_82004;
            }
        }
        if (futrts_get_num_neighbours_against_12423(ctx, &ext_mem_83451, mem_82925, mem_82927, mem_82925, mem_82927, loopres_73979, loopres_73979, 2.0, (int8_t) 0, (int8_t) 1, 0.0, (int64_t) 9223372036854775807) != 0) {
            err = 1;
            goto cleanup;
        }
        if (mem_83453_cached_sizze_84888 < bytes_82918) {
            err = lexical_realloc(ctx, &mem_83453, &mem_83453_cached_sizze_84888, bytes_82918);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t nest_i_84627 = 0; nest_i_84627 < loopres_73979; nest_i_84627++) {
            ((int64_t *) mem_83453)[nest_i_84627] = (int64_t) 0;
        }
        if (memblock_alloc(ctx, &mem_83455, bytes_83152, "mem_83455")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83455.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83155, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74742});
        if (memblock_alloc(ctx, &mem_83458, bytes_83152, "mem_83458")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83458.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83153, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74742});
        if (memblock_alloc(ctx, &mem_83461, bytes_83325, "mem_83461")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83461.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83287, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {partition_sizze_75035});
        if (memblock_alloc(ctx, &mem_83464, bytes_83325, "mem_83464")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83464.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83289, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {partition_sizze_75035});
        if (futrts_get_num_neighbours_against_12426(ctx, &ext_mem_83466, mem_83455, mem_83458, mem_83461, mem_83464, m_74742, partition_sizze_75035, 2.0, (int8_t) 0, (int8_t) 1, 0.0, (int64_t) 9223372036854775807) != 0) {
            err = 1;
            goto cleanup;
        }
        if (memblock_unref(ctx, &mem_83455, "mem_83455") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83458, "mem_83458") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83461, "mem_83461") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83464, "mem_83464") != 0)
            return 1;
        for (int64_t write_iter_82008 = 0; write_iter_82008 < m_74742; write_iter_82008++) {
            int64_t write_iv_82010 = ((int64_t *) mem_83157)[write_iter_82008];
            int64_t write_iv_82011 = ((int64_t *) ext_mem_83466.mem)[write_iter_82008];
            
            if (sle64((int64_t) 0, write_iv_82010) && slt64(write_iv_82010, loopres_73979)) {
                ((int64_t *) mem_83453)[write_iv_82010] = write_iv_82011;
            }
        }
        if (memblock_unref(ctx, &ext_mem_83466, "ext_mem_83466") != 0)
            return 1;
        if (mem_83474_cached_sizze_84889 < bytes_82918) {
            err = lexical_realloc(ctx, &mem_83474, &mem_83474_cached_sizze_84889, bytes_82918);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83476_cached_sizze_84890 < bytes_82918) {
            err = lexical_realloc(ctx, &mem_83476, &mem_83476_cached_sizze_84890, bytes_82918);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (memblock_alloc(ctx, &mem_83477, loopres_73979, "mem_83477")) {
            err = 1;
            goto cleanup;
        }
        
        int64_t discard_82024;
        int64_t scanacc_82016 = (int64_t) 0;
        
        for (int64_t i_82020 = 0; i_82020 < loopres_73979; i_82020++) {
            int64_t eta_p_77692 = ((int64_t *) ext_mem_83451.mem)[i_82020];
            int64_t eta_p_77693 = ((int64_t *) mem_83453)[i_82020];
            int64_t lifted_lambda_res_77694 = sub64(eta_p_77692, (int64_t) 1);
            int64_t defunc_0_f_res_77696 = add64(eta_p_77693, lifted_lambda_res_77694);
            bool lifted_lambda_res_77698 = sle64((int64_t) 4, defunc_0_f_res_77696);
            int64_t defunc_0_f_res_77700 = btoi_bool_i64(lifted_lambda_res_77698);
            int64_t defunc_0_op_res_75176 = add64(defunc_0_f_res_77700, scanacc_82016);
            
            ((int64_t *) mem_83474)[i_82020] = defunc_0_op_res_75176;
            ((int64_t *) mem_83476)[i_82020] = defunc_0_f_res_77700;
            ((bool *) mem_83477.mem)[i_82020] = lifted_lambda_res_77698;
            
            int64_t scanacc_tmp_84629 = defunc_0_op_res_75176;
            
            scanacc_82016 = scanacc_tmp_84629;
        }
        discard_82024 = scanacc_82016;
        if (memblock_unref(ctx, &ext_mem_83451, "ext_mem_83451") != 0)
            return 1;
        
        int64_t m_f_res_75177;
        
        if (x_74733) {
            int64_t x_81130 = ((int64_t *) mem_83474)[tmp_74734];
            
            m_f_res_75177 = x_81130;
        } else {
            m_f_res_75177 = (int64_t) 0;
        }
        
        int64_t m_75179;
        
        if (eq_x_y_73992) {
            m_75179 = (int64_t) 0;
        } else {
            m_75179 = m_f_res_75177;
        }
        
        int64_t m_75189 = sub64(m_75179, (int64_t) 1);
        bool i_p_m_t_s_leq_w_75191 = slt64(m_75189, loopres_73979);
        bool zzero_leq_i_p_m_t_s_75190 = sle64((int64_t) 0, m_75189);
        bool y_75193 = zzero_leq_i_p_m_t_s_75190 && i_p_m_t_s_leq_w_75191;
        bool i_lte_j_75192 = sle64((int64_t) 0, m_75179);
        bool forwards_ok_75194 = i_lte_j_75192 && y_75193;
        bool eq_x_zz_75186 = (int64_t) 0 == m_f_res_75177;
        bool p_and_eq_x_y_75187 = x_74733 && eq_x_zz_75186;
        bool empty_slice_75188 = eq_x_y_73992 || p_and_eq_x_y_75187;
        bool ok_or_empty_75195 = empty_slice_75188 || forwards_ok_75194;
        bool index_certs_75196;
        
        if (!ok_or_empty_75195) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_75179, "] out of bounds for array of shape [", (long long) loopres_73979, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_83496 = (int64_t) 8 * m_75179;
        
        if (mem_83497_cached_sizze_84891 < bytes_83496) {
            err = lexical_realloc(ctx, &mem_83497, &mem_83497_cached_sizze_84891, bytes_83496);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t nest_i_84633 = 0; nest_i_84633 < m_75179; nest_i_84633++) {
            ((int64_t *) mem_83497)[nest_i_84633] = (int64_t) -1;
        }
        if (mem_83499_cached_sizze_84892 < bytes_83496) {
            err = lexical_realloc(ctx, &mem_83499, &mem_83499_cached_sizze_84892, bytes_83496);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83499, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_82920.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75179});
        if (mem_83501_cached_sizze_84893 < bytes_83496) {
            err = lexical_realloc(ctx, &mem_83501, &mem_83501_cached_sizze_84893, bytes_83496);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83501, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_82921.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75179});
        for (int64_t write_iter_82025 = 0; write_iter_82025 < loopres_73979; write_iter_82025++) {
            int64_t write_iv_82029 = ((int64_t *) mem_83476)[write_iter_82025];
            int64_t write_iv_82030 = ((int64_t *) mem_83474)[write_iter_82025];
            double write_iv_82031 = ((double *) ext_mem_82921.mem)[write_iter_82025];
            double write_iv_82032 = ((double *) ext_mem_82920.mem)[write_iter_82025];
            bool cond_77654 = write_iv_82029 == (int64_t) 1;
            int64_t lifted_lambda_res_77655;
            
            if (cond_77654) {
                int64_t lifted_lambda_res_t_res_81131 = sub64(write_iv_82030, (int64_t) 1);
                
                lifted_lambda_res_77655 = lifted_lambda_res_t_res_81131;
            } else {
                lifted_lambda_res_77655 = (int64_t) -1;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77655) && slt64(lifted_lambda_res_77655, m_75179)) {
                ((double *) mem_83501)[lifted_lambda_res_77655] = write_iv_82031;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77655) && slt64(lifted_lambda_res_77655, m_75179)) {
                ((double *) mem_83499)[lifted_lambda_res_77655] = write_iv_82032;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77655) && slt64(lifted_lambda_res_77655, m_75179)) {
                ((int64_t *) mem_83497)[lifted_lambda_res_77655] = (int64_t) -1;
            }
        }
        
        int64_t conc_tmp_75255 = m_75138 + m_75179;
        int64_t conc_tmp_75256 = m_75224 + conc_tmp_75255;
        int64_t bytes_83520 = (int64_t) 8 * conc_tmp_75256;
        
        if (mem_83521_cached_sizze_84894 < bytes_83520) {
            err = lexical_realloc(ctx, &mem_83521, &mem_83521_cached_sizze_84894, bytes_83520);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t tmp_offs_84637 = (int64_t) 0;
        
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83521, tmp_offs_84637, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83497, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75179});
        tmp_offs_84637 += m_75179;
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83521, tmp_offs_84637, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83388, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75138});
        tmp_offs_84637 += m_75138;
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83521, tmp_offs_84637, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83428, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75224});
        tmp_offs_84637 += m_75224;
        
        int64_t max_res_75262 = smax64((int64_t) 1, conc_tmp_75256);
        bool zzero_75263 = max_res_75262 == (int64_t) 0;
        bool nonzzero_75264 = !zzero_75263;
        bool nonzzero_cert_75265;
        
        if (!nonzzero_75264) {
            set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../ftDBSCAN_plus.fut:636:63-77\n   #1  test_dbscan_plus.fut:14:23-19:30\n   #2  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t max_arg1_75266 = sdiv64((int64_t) 9223372036854775807, max_res_75262);
        int64_t max_res_75267 = smax64((int64_t) 1, max_arg1_75266);
        int64_t zm_lhs_75268 = add64(conc_tmp_75256, max_res_75267);
        int64_t zs_lhs_75269 = sub64(zm_lhs_75268, (int64_t) 1);
        bool zzero_75270 = max_res_75267 == (int64_t) 0;
        bool nonzzero_75271 = !zzero_75270;
        bool nonzzero_cert_75272;
        
        if (!nonzzero_75271) {
            set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../ftDBSCAN_plus.fut:558:58-65\n   #1  test_dbscan_plus.fut:14:23-19:30\n   #2  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t inner_iter_75273 = sdiv64(zs_lhs_75269, max_res_75267);
        
        if (mem_83523_cached_sizze_84895 < bytes_83520) {
            err = lexical_realloc(ctx, &mem_83523, &mem_83523_cached_sizze_84895, bytes_83520);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t nest_i_84638 = 0; nest_i_84638 < conc_tmp_75256; nest_i_84638++) {
            ((int64_t *) mem_83523)[nest_i_84638] = (int64_t) 0;
        }
        
        bool bounds_invalid_upwards_75275 = slt64(inner_iter_75273, (int64_t) 0);
        bool valid_75276 = !bounds_invalid_upwards_75275;
        bool range_valid_c_75277;
        
        if (!valid_75276) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) inner_iter_75273, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  ../../ftDBSCAN_plus.fut:561:42-57\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        if (mem_83525_cached_sizze_84896 < bytes_83520) {
            err = lexical_realloc(ctx, &mem_83525, &mem_83525_cached_sizze_84896, bytes_83520);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t i_84639 = 0; i_84639 < conc_tmp_75256; i_84639++) {
            int64_t x_84640 = (int64_t) 0 + i_84639 * (int64_t) 1;
            
            ((int64_t *) mem_83525)[i_84639] = x_84640;
        }
        
        int64_t mk_adjacency_list_res_75279;
        int64_t loop_dz2089U_75283;
        
        if (memblock_set(ctx, &mem_param_83531, &mem_82853, "mem_82853") != 0)
            return 1;
        loop_dz2089U_75283 = (int64_t) 0;
        for (int64_t i_75282 = 0; i_75282 < inner_iter_75273; i_75282++) {
            int64_t inf_75286 = mul64(max_res_75267, i_75282);
            int64_t min_arg1_75287 = add64(max_res_75267, inf_75286);
            int64_t min_res_75288 = smin64(conc_tmp_75256, min_arg1_75287);
            int64_t dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_75289 = sub64(min_res_75288, inf_75286);
            int64_t dzlz7bUZLztZRz20UZLZLzmZRz20Usupz20UinfZRz20Unz7dUzg_75290 = mul64(conc_tmp_75256, dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_75289);
            bool empty_slice_75291 = dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_75289 == (int64_t) 0;
            int64_t m_75292 = sub64(dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_75289, (int64_t) 1);
            int64_t i_p_m_t_s_75293 = add64(inf_75286, m_75292);
            bool zzero_leq_i_p_m_t_s_75294 = sle64((int64_t) 0, i_p_m_t_s_75293);
            bool i_p_m_t_s_leq_w_75295 = slt64(i_p_m_t_s_75293, conc_tmp_75256);
            bool zzero_lte_i_75296 = sle64((int64_t) 0, inf_75286);
            bool i_lte_j_75297 = sle64(inf_75286, min_res_75288);
            bool y_75298 = i_p_m_t_s_leq_w_75295 && zzero_lte_i_75296;
            bool y_75299 = zzero_leq_i_p_m_t_s_75294 && y_75298;
            bool forwards_ok_75300 = i_lte_j_75297 && y_75299;
            bool ok_or_empty_75301 = empty_slice_75291 || forwards_ok_75300;
            bool index_certs_75302;
            
            if (!ok_or_empty_75301) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) inf_75286, ":", (long long) min_res_75288, "] out of bounds for array of shape [", (long long) conc_tmp_75256, "].", "-> #0  ../../ftDBSCAN_plus.fut:564:56-73\n   #1  test_dbscan_plus.fut:14:23-19:30\n   #2  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool bounds_invalid_upwards_75303 = slt64(min_res_75288, inf_75286);
            bool valid_75304 = !bounds_invalid_upwards_75303;
            bool range_valid_c_75305;
            
            if (!valid_75304) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) inf_75286, "..<", (long long) min_res_75288, " is invalid.", "-> #0  ../../ftDBSCAN_plus.fut:566:57-66\n   #1  test_dbscan_plus.fut:14:23-19:30\n   #2  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t binop_x_83532 = (int64_t) 8 * dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_75289;
            int64_t bytes_83533 = conc_tmp_75256 * binop_x_83532;
            
            if (mem_83534_cached_sizze_84897 < bytes_83533) {
                err = lexical_realloc(ctx, &mem_83534, &mem_83534_cached_sizze_84897, bytes_83533);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            if (mem_83536_cached_sizze_84898 < dzlz7bUZLztZRz20UZLZLzmZRz20Usupz20UinfZRz20Unz7dUzg_75290) {
                err = lexical_realloc(ctx, &mem_83536, &mem_83536_cached_sizze_84898, dzlz7bUZLztZRz20UZLZLzmZRz20Usupz20UinfZRz20Unz7dUzg_75290);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            for (int64_t i_82044 = 0; i_82044 < dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_75289; i_82044++) {
                int64_t index_primexp_82568 = add64(inf_75286, i_82044);
                bool index_concat_cmp_82558 = sle64(conc_tmp_75255, index_primexp_82568);
                double index_concat_branch_82566;
                
                if (index_concat_cmp_82558) {
                    int64_t index_concat_i_82559 = sub64(index_primexp_82568, conc_tmp_75255);
                    double index_concat_82560 = ((double *) mem_83432)[index_concat_i_82559];
                    
                    index_concat_branch_82566 = index_concat_82560;
                } else {
                    bool index_concat_cmp_82561 = sle64(m_75179, index_primexp_82568);
                    double index_concat_branch_82565;
                    
                    if (index_concat_cmp_82561) {
                        int64_t index_concat_i_82562 = sub64(index_primexp_82568, m_75179);
                        double index_concat_82563 = ((double *) mem_83392)[index_concat_i_82562];
                        
                        index_concat_branch_82565 = index_concat_82563;
                    } else {
                        double index_concat_82564 = ((double *) mem_83501)[index_primexp_82568];
                        
                        index_concat_branch_82565 = index_concat_82564;
                    }
                    index_concat_branch_82566 = index_concat_branch_82565;
                }
                
                double index_concat_branch_82554;
                
                if (index_concat_cmp_82558) {
                    int64_t index_concat_i_82547 = sub64(index_primexp_82568, conc_tmp_75255);
                    double index_concat_82548 = ((double *) mem_83430)[index_concat_i_82547];
                    
                    index_concat_branch_82554 = index_concat_82548;
                } else {
                    bool index_concat_cmp_82549 = sle64(m_75179, index_primexp_82568);
                    double index_concat_branch_82553;
                    
                    if (index_concat_cmp_82549) {
                        int64_t index_concat_i_82550 = sub64(index_primexp_82568, m_75179);
                        double index_concat_82551 = ((double *) mem_83390)[index_concat_i_82550];
                        
                        index_concat_branch_82553 = index_concat_82551;
                    } else {
                        double index_concat_82552 = ((double *) mem_83499)[index_primexp_82568];
                        
                        index_concat_branch_82553 = index_concat_82552;
                    }
                    index_concat_branch_82554 = index_concat_branch_82553;
                }
                for (int64_t nest_i_84647 = 0; nest_i_84647 < conc_tmp_75256; nest_i_84647++) {
                    ((int64_t *) mem_83534)[i_82044 * conc_tmp_75256 + nest_i_84647] = index_primexp_82568;
                }
                for (int64_t i_82038 = 0; i_82038 < conc_tmp_75256; i_82038++) {
                    bool index_concat_cmp_82532 = sle64(conc_tmp_75255, i_82038);
                    double index_concat_branch_82540;
                    
                    if (index_concat_cmp_82532) {
                        int64_t index_concat_i_82533 = sub64(i_82038, conc_tmp_75255);
                        double index_concat_82534 = ((double *) mem_83432)[index_concat_i_82533];
                        
                        index_concat_branch_82540 = index_concat_82534;
                    } else {
                        bool index_concat_cmp_82535 = sle64(m_75179, i_82038);
                        double index_concat_branch_82539;
                        
                        if (index_concat_cmp_82535) {
                            int64_t index_concat_i_82536 = sub64(i_82038, m_75179);
                            double index_concat_82537 = ((double *) mem_83392)[index_concat_i_82536];
                            
                            index_concat_branch_82539 = index_concat_82537;
                        } else {
                            double index_concat_82538 = ((double *) mem_83501)[i_82038];
                            
                            index_concat_branch_82539 = index_concat_82538;
                        }
                        index_concat_branch_82540 = index_concat_branch_82539;
                    }
                    
                    double index_concat_branch_82529;
                    
                    if (index_concat_cmp_82532) {
                        int64_t index_concat_i_82522 = sub64(i_82038, conc_tmp_75255);
                        double index_concat_82523 = ((double *) mem_83430)[index_concat_i_82522];
                        
                        index_concat_branch_82529 = index_concat_82523;
                    } else {
                        bool index_concat_cmp_82524 = sle64(m_75179, i_82038);
                        double index_concat_branch_82528;
                        
                        if (index_concat_cmp_82524) {
                            int64_t index_concat_i_82525 = sub64(i_82038, m_75179);
                            double index_concat_82526 = ((double *) mem_83390)[index_concat_i_82525];
                            
                            index_concat_branch_82528 = index_concat_82526;
                        } else {
                            double index_concat_82527 = ((double *) mem_83499)[i_82038];
                            
                            index_concat_branch_82528 = index_concat_82527;
                        }
                        index_concat_branch_82529 = index_concat_branch_82528;
                    }
                    
                    bool cond_75320 = index_primexp_82568 == i_82038;
                    bool zeze_res_75321 = index_concat_branch_82566 == index_concat_branch_82540;
                    double d_euclidean_res_75322;
                    
                    if (zeze_res_75321) {
                        double zm_res_81132 = index_concat_branch_82554 - index_concat_branch_82529;
                        double abs_res_81133 = fabs64(zm_res_81132);
                        
                        d_euclidean_res_75322 = abs_res_81133;
                    } else {
                        bool zeze_res_75325 = index_concat_branch_82554 == index_concat_branch_82529;
                        double d_euclidean_res_f_res_75326;
                        
                        if (zeze_res_75325) {
                            double zm_res_81134 = index_concat_branch_82566 - index_concat_branch_82540;
                            double abs_res_81135 = fabs64(zm_res_81134);
                            
                            d_euclidean_res_f_res_75326 = abs_res_81135;
                        } else {
                            double zm_res_75329 = index_concat_branch_82554 - index_concat_branch_82529;
                            double zm_res_75330 = index_concat_branch_82566 - index_concat_branch_82540;
                            double hypot_res_75331 = futrts_hypot64(zm_res_75330, zm_res_75329);
                            
                            d_euclidean_res_f_res_75326 = hypot_res_75331;
                        }
                        d_euclidean_res_75322 = d_euclidean_res_f_res_75326;
                    }
                    
                    bool zlze_res_75332 = d_euclidean_res_75322 <= 2.0;
                    bool x_75333 = !cond_75320;
                    bool y_75334 = zlze_res_75332 && x_75333;
                    bool lifted_lambda_res_75335 = cond_75320 || y_75334;
                    
                    ((bool *) mem_83536)[i_82044 * conc_tmp_75256 + i_82038] = lifted_lambda_res_75335;
                }
            }
            
            int64_t bytes_83569 = (int64_t) 8 * dzlz7bUZLztZRz20UZLZLzmZRz20Usupz20UinfZRz20Unz7dUzg_75290;
            
            if (mem_83570_cached_sizze_84901 < bytes_83569) {
                err = lexical_realloc(ctx, &mem_83570, &mem_83570_cached_sizze_84901, bytes_83569);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            if (mem_83572_cached_sizze_84902 < bytes_83569) {
                err = lexical_realloc(ctx, &mem_83572, &mem_83572_cached_sizze_84902, bytes_83569);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            
            int64_t discard_82055;
            int64_t scanacc_82049 = (int64_t) 0;
            
            for (int64_t i_82052 = 0; i_82052 < dzlz7bUZLztZRz20UZLZLzmZRz20Usupz20UinfZRz20Unz7dUzg_75290; i_82052++) {
                int64_t new_index_82515 = squot64(i_82052, conc_tmp_75256);
                int64_t binop_y_82517 = conc_tmp_75256 * new_index_82515;
                int64_t new_index_82518 = i_82052 - binop_y_82517;
                bool eta_p_79231 = ((bool *) mem_83536)[new_index_82515 * conc_tmp_75256 + new_index_82518];
                int64_t defunc_0_f_res_79232 = btoi_bool_i64(eta_p_79231);
                int64_t defunc_0_op_res_75346 = add64(defunc_0_f_res_79232, scanacc_82049);
                
                ((int64_t *) mem_83570)[i_82052] = defunc_0_op_res_75346;
                ((int64_t *) mem_83572)[i_82052] = defunc_0_f_res_79232;
                
                int64_t scanacc_tmp_84649 = defunc_0_op_res_75346;
                
                scanacc_82049 = scanacc_tmp_84649;
            }
            discard_82055 = scanacc_82049;
            
            bool cond_75347 = dzlz7bUZLztZRz20UZLZLzmZRz20Usupz20UinfZRz20Unz7dUzg_75290 == (int64_t) 0;
            bool x_75348 = !cond_75347;
            int64_t tmp_75349 = sub64(dzlz7bUZLztZRz20UZLZLzmZRz20Usupz20UinfZRz20Unz7dUzg_75290, (int64_t) 1);
            bool x_75350 = sle64((int64_t) 0, tmp_75349);
            bool y_75351 = slt64(tmp_75349, dzlz7bUZLztZRz20UZLZLzmZRz20Usupz20UinfZRz20Unz7dUzg_75290);
            bool bounds_check_75352 = x_75350 && y_75351;
            bool protect_assert_disj_75353 = cond_75347 || bounds_check_75352;
            bool index_certs_75354;
            
            if (!protect_assert_disj_75353) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_75349, "] out of bounds for array of shape [", (long long) dzlz7bUZLztZRz20UZLZLzmZRz20Usupz20UinfZRz20Unz7dUzg_75290, "].", "-> #0  /prelude/soacs.fut:257:33-47\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t m_f_res_75355;
            
            if (x_75348) {
                int64_t x_81137 = ((int64_t *) mem_83570)[tmp_75349];
                
                m_f_res_75355 = x_81137;
            } else {
                m_f_res_75355 = (int64_t) 0;
            }
            
            int64_t m_75357;
            
            if (cond_75347) {
                m_75357 = (int64_t) 0;
            } else {
                m_75357 = m_f_res_75355;
            }
            
            int64_t m_75367 = sub64(m_75357, (int64_t) 1);
            bool i_p_m_t_s_leq_w_75369 = slt64(m_75367, dzlz7bUZLztZRz20UZLZLzmZRz20Usupz20UinfZRz20Unz7dUzg_75290);
            bool zzero_leq_i_p_m_t_s_75368 = sle64((int64_t) 0, m_75367);
            bool y_75371 = zzero_leq_i_p_m_t_s_75368 && i_p_m_t_s_leq_w_75369;
            bool i_lte_j_75370 = sle64((int64_t) 0, m_75357);
            bool forwards_ok_75372 = i_lte_j_75370 && y_75371;
            bool eq_x_zz_75364 = (int64_t) 0 == m_f_res_75355;
            bool p_and_eq_x_y_75365 = x_75348 && eq_x_zz_75364;
            bool empty_slice_75366 = cond_75347 || p_and_eq_x_y_75365;
            bool ok_or_empty_75373 = empty_slice_75366 || forwards_ok_75372;
            bool index_certs_75374;
            
            if (!ok_or_empty_75373) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_75357, "] out of bounds for array of shape [", (long long) dzlz7bUZLztZRz20UZLZLzmZRz20Usupz20UinfZRz20Unz7dUzg_75290, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t bytes_83585 = (int64_t) 8 * m_75357;
            int64_t conc_tmp_75396 = loop_dz2089U_75283 + m_75357;
            int64_t bytes_83609 = (int64_t) 8 * conc_tmp_75396;
            
            if (mem_83564_cached_sizze_84899 < bytes_83533) {
                err = lexical_realloc(ctx, &mem_83564, &mem_83564_cached_sizze_84899, bytes_83533);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            for (int64_t nest_i_84652 = 0; nest_i_84652 < dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_75289; nest_i_84652++) {
                lmad_copy_8b(ctx, 1, (uint64_t *) mem_83564, nest_i_84652 * conc_tmp_75256, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83525, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {conc_tmp_75256});
            }
            if (mem_83567_cached_sizze_84900 < bytes_83533) {
                err = lexical_realloc(ctx, &mem_83567, &mem_83567_cached_sizze_84900, bytes_83533);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            lmad_copy_8b(ctx, 2, (uint64_t *) mem_83567, (int64_t) 0, (int64_t []) {conc_tmp_75256, (int64_t) 1}, (uint64_t *) mem_83534, (int64_t) 0, (int64_t []) {conc_tmp_75256, (int64_t) 1}, (int64_t []) {dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_75289, conc_tmp_75256});
            if (memblock_alloc(ctx, &mem_83610, bytes_83609, "mem_83610")) {
                err = 1;
                goto cleanup;
            }
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_83610.mem, loop_dz2089U_75283, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83564, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75357});
            if (mem_83588_cached_sizze_84903 < bytes_83585) {
                err = lexical_realloc(ctx, &mem_83588, &mem_83588_cached_sizze_84903, bytes_83585);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_83588, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83567, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75357});
            for (int64_t write_iter_82056 = 0; write_iter_82056 < dzlz7bUZLztZRz20UZLZLzmZRz20Usupz20UinfZRz20Unz7dUzg_75290; write_iter_82056++) {
                int64_t write_iv_82059 = ((int64_t *) mem_83572)[write_iter_82056];
                int64_t write_iv_82060 = ((int64_t *) mem_83570)[write_iter_82056];
                int64_t new_index_82505 = squot64(write_iter_82056, conc_tmp_75256);
                int64_t binop_y_82507 = conc_tmp_75256 * new_index_82505;
                int64_t new_index_82508 = write_iter_82056 - binop_y_82507;
                int64_t write_iv_82061 = ((int64_t *) mem_83534)[new_index_82505 * conc_tmp_75256 + new_index_82508];
                bool cond_79226 = write_iv_82059 == (int64_t) 1;
                int64_t lifted_lambda_res_79227;
                
                if (cond_79226) {
                    int64_t lifted_lambda_res_t_res_81138 = sub64(write_iv_82060, (int64_t) 1);
                    
                    lifted_lambda_res_79227 = lifted_lambda_res_t_res_81138;
                } else {
                    lifted_lambda_res_79227 = (int64_t) -1;
                }
                if (sle64((int64_t) 0, lifted_lambda_res_79227) && slt64(lifted_lambda_res_79227, m_75357)) {
                    ((int64_t *) mem_83588)[lifted_lambda_res_79227] = write_iv_82061;
                }
                if (sle64((int64_t) 0, lifted_lambda_res_79227) && slt64(lifted_lambda_res_79227, m_75357)) {
                    ((int64_t *) mem_83610.mem)[loop_dz2089U_75283 + lifted_lambda_res_79227] = new_index_82508;
                }
            }
            for (int64_t nest_i_84655 = 0; nest_i_84655 < dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_75289; nest_i_84655++) {
                ((int64_t *) mem_83523)[inf_75286 + nest_i_84655] = (int64_t) 0;
            }
            for (int64_t iter_82065 = 0; iter_82065 < m_75357; iter_82065++) {
                int64_t pixel_82067 = ((int64_t *) mem_83588)[iter_82065];
                int64_t tmp_79219 = sub64(pixel_82067, inf_75286);
                bool less_than_zzero_82069 = slt64(tmp_79219, (int64_t) 0);
                bool greater_than_sizze_82070 = sle64(dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_75289, tmp_79219);
                bool outside_bounds_dim_82071 = less_than_zzero_82069 || greater_than_sizze_82070;
                
                if (!outside_bounds_dim_82071) {
                    int64_t read_hist_82073 = ((int64_t *) mem_83523)[inf_75286 + tmp_79219];
                    int64_t defunc_0_op_res_75392 = add64((int64_t) 1, read_hist_82073);
                    
                    ((int64_t *) mem_83523)[inf_75286 + tmp_79219] = defunc_0_op_res_75392;
                }
            }
            
            int64_t tmp_offs_84657 = (int64_t) 0;
            
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_83610.mem, tmp_offs_84657, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_83531.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {loop_dz2089U_75283});
            tmp_offs_84657 += loop_dz2089U_75283;
            if (!(tmp_offs_84657 == loop_dz2089U_75283)) {
                lmad_copy_8b(ctx, 1, (uint64_t *) mem_83610.mem, tmp_offs_84657, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83610.mem, loop_dz2089U_75283, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75357});
            }
            tmp_offs_84657 += m_75357;
            if (memblock_set(ctx, &mem_param_tmp_84641, &mem_83610, "mem_83610") != 0)
                return 1;
            
            int64_t loop_dz2089U_tmp_84642 = conc_tmp_75396;
            
            if (memblock_set(ctx, &mem_param_83531, &mem_param_tmp_84641, "mem_param_tmp_84641") != 0)
                return 1;
            loop_dz2089U_75283 = loop_dz2089U_tmp_84642;
        }
        if (memblock_set(ctx, &ext_mem_83615, &mem_param_83531, "mem_param_83531") != 0)
            return 1;
        mk_adjacency_list_res_75279 = loop_dz2089U_75283;
        if (mem_83618_cached_sizze_84904 < bytes_83520) {
            err = lexical_realloc(ctx, &mem_83618, &mem_83618_cached_sizze_84904, bytes_83520);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_82081;
        int64_t scanacc_82077 = (int64_t) 0;
        
        for (int64_t i_82079 = 0; i_82079 < conc_tmp_75256; i_82079++) {
            int64_t x_75399 = ((int64_t *) mem_83523)[i_82079];
            int64_t defunc_0_op_res_75402 = add64(x_75399, scanacc_82077);
            
            ((int64_t *) mem_83618)[i_82079] = defunc_0_op_res_75402;
            
            int64_t scanacc_tmp_84658 = defunc_0_op_res_75402;
            
            scanacc_82077 = scanacc_tmp_84658;
        }
        discard_82081 = scanacc_82077;
        if (mem_83625_cached_sizze_84905 < mk_adjacency_list_res_75279) {
            err = lexical_realloc(ctx, &mem_83625, &mem_83625_cached_sizze_84905, mk_adjacency_list_res_75279);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t nest_i_84660 = 0; nest_i_84660 < mk_adjacency_list_res_75279; nest_i_84660++) {
            ((bool *) mem_83625)[nest_i_84660] = 0;
        }
        for (int64_t write_iter_82082 = 0; write_iter_82082 < conc_tmp_75256; write_iter_82082++) {
            int64_t zv_lhs_77628 = add64((int64_t) -1, write_iter_82082);
            int64_t tmp_77629 = smod64(zv_lhs_77628, conc_tmp_75256);
            int64_t lifted_lambda_res_77630 = ((int64_t *) mem_83618)[tmp_77629];
            bool cond_77632 = write_iter_82082 == (int64_t) 0;
            int64_t lifted_lambda_res_77633;
            
            if (cond_77632) {
                lifted_lambda_res_77633 = (int64_t) 0;
            } else {
                lifted_lambda_res_77633 = lifted_lambda_res_77630;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77633) && slt64(lifted_lambda_res_77633, mk_adjacency_list_res_75279)) {
                ((bool *) mem_83625)[lifted_lambda_res_77633] = 1;
            }
        }
        
        bool defunc_0_reduce_res_81616;
        bool redout_82086 = 0;
        
        for (int64_t i_82087 = 0; i_82087 < mk_adjacency_list_res_75279; i_82087++) {
            int64_t eta_p_77615 = ((int64_t *) ext_mem_83615.mem)[i_82087];
            bool lifted_lambda_res_77616 = (int64_t) -1 == eta_p_77615;
            bool lifted_lambda_res_77617 = !lifted_lambda_res_77616;
            bool defunc_0_op_res_76954 = lifted_lambda_res_77617 || redout_82086;
            bool redout_tmp_84662 = defunc_0_op_res_76954;
            
            redout_82086 = redout_tmp_84662;
        }
        defunc_0_reduce_res_81616 = redout_82086;
        
        bool cond_75422 = slt64((int64_t) 0, mk_adjacency_list_res_75279);
        int64_t bytes_83635 = (int64_t) 8 * mk_adjacency_list_res_75279;
        
        if (mem_83636_cached_sizze_84906 < bytes_83635) {
            err = lexical_realloc(ctx, &mem_83636, &mem_83636_cached_sizze_84906, bytes_83635);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83644_cached_sizze_84907 < bytes_83635) {
            err = lexical_realloc(ctx, &mem_83644, &mem_83644_cached_sizze_84907, bytes_83635);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83645_cached_sizze_84908 < mk_adjacency_list_res_75279) {
            err = lexical_realloc(ctx, &mem_83645, &mem_83645_cached_sizze_84908, mk_adjacency_list_res_75279);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        bool itered_neighlist_75423;
        bool loop_while_75425;
        
        if (memblock_set(ctx, &mem_param_83634, &ext_mem_83615, "ext_mem_83615") != 0)
            return 1;
        loop_while_75425 = defunc_0_reduce_res_81616;
        while (loop_while_75425) {
            int64_t discard_82107;
            int64_t scanacc_82101 = (int64_t) 0;
            
            for (int64_t i_82104 = 0; i_82104 < mk_adjacency_list_res_75279; i_82104++) {
                int64_t zv_lhs_79289 = add64((int64_t) 1, i_82104);
                int64_t tmp_79290 = smod64(zv_lhs_79289, mk_adjacency_list_res_75279);
                bool lifted_lambda_res_79291 = ((bool *) mem_83625)[tmp_79290];
                int64_t bool_res_79293 = btoi_bool_i64(lifted_lambda_res_79291);
                int64_t defunc_0_op_res_75451 = add64(bool_res_79293, scanacc_82101);
                
                ((int64_t *) mem_83644)[i_82104] = defunc_0_op_res_75451;
                ((bool *) mem_83645)[i_82104] = lifted_lambda_res_79291;
                
                int64_t scanacc_tmp_84666 = defunc_0_op_res_75451;
                
                scanacc_82101 = scanacc_tmp_84666;
            }
            discard_82107 = scanacc_82101;
            
            int64_t num_segments_t_res_75452;
            
            if (cond_75422) {
                int64_t tmp_81149 = sub64(mk_adjacency_list_res_75279, (int64_t) 1);
                bool x_81150 = sle64((int64_t) 0, tmp_81149);
                bool y_81151 = slt64(tmp_81149, mk_adjacency_list_res_75279);
                bool bounds_check_81152 = x_81150 && y_81151;
                bool index_certs_81153;
                
                if (!bounds_check_81152) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_81149, "] out of bounds for array of shape [", (long long) mk_adjacency_list_res_75279, "].", "-> #0  /prelude/array.fut:28:29-37\n   #1  ../../lib/github.com/diku-dk/segmented/segmented.fut:29:36-60\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                int64_t last_res_81154 = ((int64_t *) mem_83644)[tmp_81149];
                
                num_segments_t_res_75452 = last_res_81154;
            } else {
                num_segments_t_res_75452 = (int64_t) 0;
            }
            
            int64_t num_segments_75459;
            
            if (cond_75422) {
                num_segments_75459 = num_segments_t_res_75452;
            } else {
                num_segments_75459 = (int64_t) 0;
            }
            
            int64_t bytes_83658 = (int64_t) 8 * num_segments_75459;
            int64_t discard_82098;
            int64_t scanacc_82091 = (int64_t) 9223372036854775807;
            
            for (int64_t i_82094 = 0; i_82094 < mk_adjacency_list_res_75279; i_82094++) {
                bool x_75429 = ((bool *) mem_83625)[i_82094];
                int64_t x_75430 = ((int64_t *) mem_param_83634.mem)[i_82094];
                int64_t tmp_75436;
                
                if (x_75429) {
                    tmp_75436 = x_75430;
                } else {
                    bool cond_75437 = slt64(scanacc_82091, x_75430);
                    int64_t lifted_lambda_res_75438;
                    
                    if (cond_75437) {
                        lifted_lambda_res_75438 = scanacc_82091;
                    } else {
                        lifted_lambda_res_75438 = x_75430;
                    }
                    tmp_75436 = lifted_lambda_res_75438;
                }
                ((int64_t *) mem_83636)[i_82094] = tmp_75436;
                
                int64_t scanacc_tmp_84669 = tmp_75436;
                
                scanacc_82091 = scanacc_tmp_84669;
            }
            discard_82098 = scanacc_82091;
            if (mem_83659_cached_sizze_84909 < bytes_83658) {
                err = lexical_realloc(ctx, &mem_83659, &mem_83659_cached_sizze_84909, bytes_83658);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            for (int64_t nest_i_84671 = 0; nest_i_84671 < num_segments_75459; nest_i_84671++) {
                ((int64_t *) mem_83659)[nest_i_84671] = (int64_t) 9223372036854775807;
            }
            for (int64_t write_iter_82108 = 0; write_iter_82108 < mk_adjacency_list_res_75279; write_iter_82108++) {
                int64_t write_iv_82110 = ((int64_t *) mem_83644)[write_iter_82108];
                bool write_iv_82111 = ((bool *) mem_83645)[write_iter_82108];
                int64_t write_iv_82112 = ((int64_t *) mem_83636)[write_iter_82108];
                int64_t lifted_index_res_79280;
                
                if (write_iv_82111) {
                    int64_t lifted_index_res_t_res_81155 = sub64(write_iv_82110, (int64_t) 1);
                    
                    lifted_index_res_79280 = lifted_index_res_t_res_81155;
                } else {
                    lifted_index_res_79280 = (int64_t) -1;
                }
                if (sle64((int64_t) 0, lifted_index_res_79280) && slt64(lifted_index_res_79280, num_segments_75459)) {
                    ((int64_t *) mem_83659)[lifted_index_res_79280] = write_iv_82112;
                }
            }
            if (memblock_alloc(ctx, &mem_83667, bytes_83635, "mem_83667")) {
                err = 1;
                goto cleanup;
            }
            
            bool defunc_0_reduce_res_81161;
            bool redout_82115 = 0;
            
            for (int64_t i_82117 = 0; i_82117 < mk_adjacency_list_res_75279; i_82117++) {
                int64_t eta_p_79265 = ((int64_t *) mem_param_83634.mem)[i_82117];
                bool x_79267 = sle64((int64_t) 0, eta_p_79265);
                bool y_79268 = slt64(eta_p_79265, num_segments_75459);
                bool bounds_check_79269 = x_79267 && y_79268;
                bool index_certs_79270;
                
                if (!bounds_check_79269) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_79265, "] out of bounds for array of shape [", (long long) num_segments_75459, "].", "-> #0  ../../ftDBSCAN_plus.fut:607:71-96\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../ftDBSCAN_plus.fut:607:60-97\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                int64_t lifted_lambda_res_79271 = ((int64_t *) mem_83659)[eta_p_79265];
                bool lifted_lambda_res_79273 = eta_p_79265 == lifted_lambda_res_79271;
                bool lifted_lambda_res_79274 = !lifted_lambda_res_79273;
                bool defunc_0_op_res_76962 = lifted_lambda_res_79274 || redout_82115;
                
                ((int64_t *) mem_83667.mem)[i_82117] = lifted_lambda_res_79271;
                
                bool redout_tmp_84673 = defunc_0_op_res_76962;
                
                redout_82115 = redout_tmp_84673;
            }
            defunc_0_reduce_res_81161 = redout_82115;
            if (memblock_set(ctx, &mem_param_tmp_84663, &mem_83667, "mem_83667") != 0)
                return 1;
            
            bool loop_while_tmp_84664 = defunc_0_reduce_res_81161;
            
            if (memblock_set(ctx, &mem_param_83634, &mem_param_tmp_84663, "mem_param_tmp_84663") != 0)
                return 1;
            loop_while_75425 = loop_while_tmp_84664;
        }
        if (memblock_set(ctx, &ext_mem_83676, &mem_param_83634, "mem_param_83634") != 0)
            return 1;
        itered_neighlist_75423 = loop_while_75425;
        if (memblock_unref(ctx, &ext_mem_83615, "ext_mem_83615") != 0)
            return 1;
        if (mem_83678_cached_sizze_84910 < bytes_83635) {
            err = lexical_realloc(ctx, &mem_83678, &mem_83678_cached_sizze_84910, bytes_83635);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_82129;
        int64_t scanacc_82122 = (int64_t) 9223372036854775807;
        
        for (int64_t i_82125 = 0; i_82125 < mk_adjacency_list_res_75279; i_82125++) {
            bool x_75484 = ((bool *) mem_83625)[i_82125];
            int64_t x_75485 = ((int64_t *) ext_mem_83676.mem)[i_82125];
            int64_t tmp_75491;
            
            if (x_75484) {
                tmp_75491 = x_75485;
            } else {
                bool cond_75492 = slt64(scanacc_82122, x_75485);
                int64_t lifted_lambda_res_75493;
                
                if (cond_75492) {
                    lifted_lambda_res_75493 = scanacc_82122;
                } else {
                    lifted_lambda_res_75493 = x_75485;
                }
                tmp_75491 = lifted_lambda_res_75493;
            }
            ((int64_t *) mem_83678)[i_82125] = tmp_75491;
            
            int64_t scanacc_tmp_84675 = tmp_75491;
            
            scanacc_82122 = scanacc_tmp_84675;
        }
        discard_82129 = scanacc_82122;
        if (memblock_unref(ctx, &ext_mem_83676, "ext_mem_83676") != 0)
            return 1;
        if (mem_83686_cached_sizze_84911 < bytes_83635) {
            err = lexical_realloc(ctx, &mem_83686, &mem_83686_cached_sizze_84911, bytes_83635);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83687_cached_sizze_84912 < mk_adjacency_list_res_75279) {
            err = lexical_realloc(ctx, &mem_83687, &mem_83687_cached_sizze_84912, mk_adjacency_list_res_75279);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_82138;
        int64_t scanacc_82132 = (int64_t) 0;
        
        for (int64_t i_82135 = 0; i_82135 < mk_adjacency_list_res_75279; i_82135++) {
            int64_t zv_lhs_77608 = add64((int64_t) 1, i_82135);
            int64_t tmp_77609 = smod64(zv_lhs_77608, mk_adjacency_list_res_75279);
            bool lifted_lambda_res_77610 = ((bool *) mem_83625)[tmp_77609];
            int64_t bool_res_77612 = btoi_bool_i64(lifted_lambda_res_77610);
            int64_t defunc_0_op_res_75506 = add64(bool_res_77612, scanacc_82132);
            
            ((int64_t *) mem_83686)[i_82135] = defunc_0_op_res_75506;
            ((bool *) mem_83687)[i_82135] = lifted_lambda_res_77610;
            
            int64_t scanacc_tmp_84677 = defunc_0_op_res_75506;
            
            scanacc_82132 = scanacc_tmp_84677;
        }
        discard_82138 = scanacc_82132;
        
        int64_t num_segments_t_res_75507;
        
        if (cond_75422) {
            int64_t tmp_81163 = sub64(mk_adjacency_list_res_75279, (int64_t) 1);
            bool x_81164 = sle64((int64_t) 0, tmp_81163);
            bool y_81165 = slt64(tmp_81163, mk_adjacency_list_res_75279);
            bool bounds_check_81166 = x_81164 && y_81165;
            bool index_certs_81167;
            
            if (!bounds_check_81166) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_81163, "] out of bounds for array of shape [", (long long) mk_adjacency_list_res_75279, "].", "-> #0  /prelude/array.fut:28:29-37\n   #1  ../../lib/github.com/diku-dk/segmented/segmented.fut:29:36-60\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t last_res_81168 = ((int64_t *) mem_83686)[tmp_81163];
            
            num_segments_t_res_75507 = last_res_81168;
        } else {
            num_segments_t_res_75507 = (int64_t) 0;
        }
        
        int64_t num_segments_75514;
        
        if (cond_75422) {
            num_segments_75514 = num_segments_t_res_75507;
        } else {
            num_segments_75514 = (int64_t) 0;
        }
        
        int64_t bytes_83700 = (int64_t) 8 * num_segments_75514;
        
        if (mem_83701_cached_sizze_84913 < bytes_83700) {
            err = lexical_realloc(ctx, &mem_83701, &mem_83701_cached_sizze_84913, bytes_83700);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t nest_i_84680 = 0; nest_i_84680 < num_segments_75514; nest_i_84680++) {
            ((int64_t *) mem_83701)[nest_i_84680] = (int64_t) 9223372036854775807;
        }
        for (int64_t write_iter_82139 = 0; write_iter_82139 < mk_adjacency_list_res_75279; write_iter_82139++) {
            int64_t write_iv_82141 = ((int64_t *) mem_83686)[write_iter_82139];
            bool write_iv_82142 = ((bool *) mem_83687)[write_iter_82139];
            int64_t write_iv_82143 = ((int64_t *) mem_83678)[write_iter_82139];
            int64_t lifted_index_res_77599;
            
            if (write_iv_82142) {
                int64_t lifted_index_res_t_res_81169 = sub64(write_iv_82141, (int64_t) 1);
                
                lifted_index_res_77599 = lifted_index_res_t_res_81169;
            } else {
                lifted_index_res_77599 = (int64_t) -1;
            }
            if (sle64((int64_t) 0, lifted_index_res_77599) && slt64(lifted_index_res_77599, num_segments_75514)) {
                ((int64_t *) mem_83701)[lifted_index_res_77599] = write_iv_82143;
            }
        }
        
        bool eq_x_y_75524 = conc_tmp_75256 == num_segments_t_res_75507;
        bool eq_x_zz_75525 = conc_tmp_75256 == (int64_t) 0;
        bool p_and_eq_x_y_75526 = cond_75422 && eq_x_y_75524;
        bool not_p_75527 = !cond_75422;
        bool p_and_eq_x_y_75528 = eq_x_zz_75525 && not_p_75527;
        bool dim_match_75529 = p_and_eq_x_y_75526 || p_and_eq_x_y_75528;
        bool empty_or_match_cert_75530;
        
        if (!dim_match_75529) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Value of (desugared) shape [", (long long) num_segments_75514, "] cannot match shape of type `[", (long long) conc_tmp_75256, "]i64`.", "-> #0  ../../ftDBSCAN_plus.fut:616:28-51\n   #1  test_dbscan_plus.fut:14:23-19:30\n   #2  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        if (mem_83709_cached_sizze_84914 < bytes_83520) {
            err = lexical_realloc(ctx, &mem_83709, &mem_83709_cached_sizze_84914, bytes_83520);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_82150;
        int64_t scanacc_82146 = (int64_t) 0;
        
        for (int64_t i_82148 = 0; i_82148 < conc_tmp_75256; i_82148++) {
            int64_t eta_p_77590 = ((int64_t *) mem_83701)[i_82148];
            bool defunc_0_f_res_77591 = i_82148 == eta_p_77590;
            int64_t lifted_lambda_res_77593 = btoi_bool_i64(defunc_0_f_res_77591);
            int64_t defunc_0_op_res_75543 = add64(lifted_lambda_res_77593, scanacc_82146);
            
            ((int64_t *) mem_83709)[i_82148] = defunc_0_op_res_75543;
            
            int64_t scanacc_tmp_84682 = defunc_0_op_res_75543;
            
            scanacc_82146 = scanacc_tmp_84682;
        }
        discard_82150 = scanacc_82146;
        if (mem_83717_cached_sizze_84915 < bytes_83520) {
            err = lexical_realloc(ctx, &mem_83717, &mem_83717_cached_sizze_84915, bytes_83520);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t i_82153 = 0; i_82153 < conc_tmp_75256; i_82153++) {
            int64_t zv_lhs_77578 = add64((int64_t) -1, i_82153);
            int64_t tmp_77579 = smod64(zv_lhs_77578, conc_tmp_75256);
            int64_t lifted_lambda_res_77580 = ((int64_t *) mem_83709)[tmp_77579];
            bool cond_77582 = i_82153 == (int64_t) 0;
            int64_t lifted_lambda_res_77583;
            
            if (cond_77582) {
                lifted_lambda_res_77583 = (int64_t) 0;
            } else {
                lifted_lambda_res_77583 = lifted_lambda_res_77580;
            }
            ((int64_t *) mem_83717)[i_82153] = lifted_lambda_res_77583;
        }
        if (mem_83725_cached_sizze_84916 < bytes_83520) {
            err = lexical_realloc(ctx, &mem_83725, &mem_83725_cached_sizze_84916, bytes_83520);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t i_82157 = 0; i_82157 < conc_tmp_75256; i_82157++) {
            int64_t eta_p_77561 = ((int64_t *) mem_83701)[i_82157];
            bool cond_77562 = slt64(eta_p_77561, (int64_t) 0);
            bool cond_f_res_77563 = sle64(conc_tmp_75256, eta_p_77561);
            bool x_77564 = !cond_77562;
            bool y_77565 = cond_f_res_77563 && x_77564;
            bool cond_77566 = cond_77562 || y_77565;
            int64_t lifted_lambda_res_77567;
            
            if (cond_77566) {
                lifted_lambda_res_77567 = (int64_t) -1;
            } else {
                bool x_77568 = sle64((int64_t) 0, eta_p_77561);
                bool y_77569 = slt64(eta_p_77561, conc_tmp_75256);
                bool bounds_check_77570 = x_77568 && y_77569;
                bool index_certs_77571;
                
                if (!bounds_check_77570) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_77561, "] out of bounds for array of shape [", (long long) conc_tmp_75256, "].", "-> #0  ../../ftbasics.fut:27:66-71\n   #1  /prelude/soacs.fut:67:23-24\n   #2  /prelude/soacs.fut:67:27-37\n   #3  /prelude/functional.fut:9:44-45\n   #4  ../../ftbasics.fut:27:23-77\n   #5  test_dbscan_plus.fut:13:1-20:55\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                int64_t lifted_lambda_res_f_res_77572 = ((int64_t *) mem_83717)[eta_p_77561];
                
                lifted_lambda_res_77567 = lifted_lambda_res_f_res_77572;
            }
            
            int64_t lifted_lambda_res_77574 = add64(old_clHandler_73938, lifted_lambda_res_77567);
            
            ((int64_t *) mem_83725)[i_82157] = lifted_lambda_res_77574;
        }
        if (mem_83733_cached_sizze_84917 < bytes_83520) {
            err = lexical_realloc(ctx, &mem_83733, &mem_83733_cached_sizze_84917, bytes_83520);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83735_cached_sizze_84918 < bytes_83520) {
            err = lexical_realloc(ctx, &mem_83735, &mem_83735_cached_sizze_84918, bytes_83520);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_82167;
        int64_t scanacc_82161 = (int64_t) 0;
        
        for (int64_t i_82164 = 0; i_82164 < conc_tmp_75256; i_82164++) {
            bool index_concat_cmp_82486 = sle64(conc_tmp_75255, i_82164);
            int64_t index_concat_branch_82494;
            
            if (index_concat_cmp_82486) {
                int64_t index_concat_i_82487 = sub64(i_82164, conc_tmp_75255);
                int64_t index_concat_82488 = ((int64_t *) mem_83428)[index_concat_i_82487];
                
                index_concat_branch_82494 = index_concat_82488;
            } else {
                bool index_concat_cmp_82489 = sle64(m_75179, i_82164);
                int64_t index_concat_branch_82493;
                
                if (index_concat_cmp_82489) {
                    int64_t index_concat_i_82490 = sub64(i_82164, m_75179);
                    int64_t index_concat_82491 = ((int64_t *) mem_83388)[index_concat_i_82490];
                    
                    index_concat_branch_82493 = index_concat_82491;
                } else {
                    int64_t index_concat_82492 = ((int64_t *) mem_83497)[i_82164];
                    
                    index_concat_branch_82493 = index_concat_82492;
                }
                index_concat_branch_82494 = index_concat_branch_82493;
            }
            
            bool lifted_lambda_res_77557 = sle64((int64_t) 0, index_concat_branch_82494);
            int64_t defunc_0_f_res_77558 = btoi_bool_i64(lifted_lambda_res_77557);
            int64_t defunc_0_op_res_75567 = add64(defunc_0_f_res_77558, scanacc_82161);
            
            ((int64_t *) mem_83733)[i_82164] = defunc_0_op_res_75567;
            ((int64_t *) mem_83735)[i_82164] = defunc_0_f_res_77558;
            
            int64_t scanacc_tmp_84686 = defunc_0_op_res_75567;
            
            scanacc_82161 = scanacc_tmp_84686;
        }
        discard_82167 = scanacc_82161;
        
        bool x_75568 = !eq_x_zz_75525;
        int64_t tmp_75569 = sub64(conc_tmp_75256, (int64_t) 1);
        bool x_75570 = sle64((int64_t) 0, tmp_75569);
        bool y_75571 = slt64(tmp_75569, conc_tmp_75256);
        bool bounds_check_75572 = x_75570 && y_75571;
        bool protect_assert_disj_75573 = eq_x_zz_75525 || bounds_check_75572;
        bool index_certs_75574;
        
        if (!protect_assert_disj_75573) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_75569, "] out of bounds for array of shape [", (long long) conc_tmp_75256, "].", "-> #0  /prelude/soacs.fut:257:33-47\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t m_f_res_75575;
        
        if (x_75568) {
            int64_t x_81170 = ((int64_t *) mem_83733)[tmp_75569];
            
            m_f_res_75575 = x_81170;
        } else {
            m_f_res_75575 = (int64_t) 0;
        }
        
        int64_t m_75577;
        
        if (eq_x_zz_75525) {
            m_75577 = (int64_t) 0;
        } else {
            m_75577 = m_f_res_75575;
        }
        
        int64_t m_75587 = sub64(m_75577, (int64_t) 1);
        bool i_p_m_t_s_leq_w_75589 = slt64(m_75587, conc_tmp_75256);
        bool zzero_leq_i_p_m_t_s_75588 = sle64((int64_t) 0, m_75587);
        bool y_75591 = zzero_leq_i_p_m_t_s_75588 && i_p_m_t_s_leq_w_75589;
        bool i_lte_j_75590 = sle64((int64_t) 0, m_75577);
        bool forwards_ok_75592 = i_lte_j_75590 && y_75591;
        bool eq_x_zz_75584 = (int64_t) 0 == m_f_res_75575;
        bool p_and_eq_x_y_75585 = x_75568 && eq_x_zz_75584;
        bool empty_slice_75586 = eq_x_zz_75525 || p_and_eq_x_y_75585;
        bool ok_or_empty_75593 = empty_slice_75586 || forwards_ok_75592;
        bool index_certs_75594;
        
        if (!ok_or_empty_75593) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_75577, "] out of bounds for array of shape [", (long long) conc_tmp_75256, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_83748 = (int64_t) 8 * m_75577;
        
        if (mem_83749_cached_sizze_84919 < bytes_83748) {
            err = lexical_realloc(ctx, &mem_83749, &mem_83749_cached_sizze_84919, bytes_83748);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83749, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83521, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75577});
        if (mem_83751_cached_sizze_84920 < bytes_83748) {
            err = lexical_realloc(ctx, &mem_83751, &mem_83751_cached_sizze_84920, bytes_83748);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83751, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83725, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75577});
        for (int64_t write_iter_82168 = 0; write_iter_82168 < conc_tmp_75256; write_iter_82168++) {
            int64_t write_iv_82171 = ((int64_t *) mem_83735)[write_iter_82168];
            int64_t write_iv_82172 = ((int64_t *) mem_83733)[write_iter_82168];
            int64_t write_iv_82173 = ((int64_t *) mem_83725)[write_iter_82168];
            bool index_concat_cmp_82475 = sle64(conc_tmp_75255, write_iter_82168);
            int64_t index_concat_branch_82483;
            
            if (index_concat_cmp_82475) {
                int64_t index_concat_i_82476 = sub64(write_iter_82168, conc_tmp_75255);
                int64_t index_concat_82477 = ((int64_t *) mem_83428)[index_concat_i_82476];
                
                index_concat_branch_82483 = index_concat_82477;
            } else {
                bool index_concat_cmp_82478 = sle64(m_75179, write_iter_82168);
                int64_t index_concat_branch_82482;
                
                if (index_concat_cmp_82478) {
                    int64_t index_concat_i_82479 = sub64(write_iter_82168, m_75179);
                    int64_t index_concat_82480 = ((int64_t *) mem_83388)[index_concat_i_82479];
                    
                    index_concat_branch_82482 = index_concat_82480;
                } else {
                    int64_t index_concat_82481 = ((int64_t *) mem_83497)[write_iter_82168];
                    
                    index_concat_branch_82482 = index_concat_82481;
                }
                index_concat_branch_82483 = index_concat_branch_82482;
            }
            
            bool cond_77551 = write_iv_82171 == (int64_t) 1;
            int64_t lifted_lambda_res_77552;
            
            if (cond_77551) {
                int64_t lifted_lambda_res_t_res_81171 = sub64(write_iv_82172, (int64_t) 1);
                
                lifted_lambda_res_77552 = lifted_lambda_res_t_res_81171;
            } else {
                lifted_lambda_res_77552 = (int64_t) -1;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77552) && slt64(lifted_lambda_res_77552, m_75577)) {
                ((int64_t *) mem_83751)[lifted_lambda_res_77552] = write_iv_82173;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77552) && slt64(lifted_lambda_res_77552, m_75577)) {
                ((int64_t *) mem_83749)[lifted_lambda_res_77552] = index_concat_branch_82483;
            }
        }
        if (mem_83765_cached_sizze_84921 < bytes_83748) {
            err = lexical_realloc(ctx, &mem_83765, &mem_83765_cached_sizze_84921, bytes_83748);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83767_cached_sizze_84922 < bytes_83748) {
            err = lexical_realloc(ctx, &mem_83767, &mem_83767_cached_sizze_84922, bytes_83748);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t i_82181 = 0; i_82181 < m_75577; i_82181++) {
            int64_t lifted_lambda_res_75612 = ((int64_t *) mem_83751)[i_82181];
            int64_t lifted_lambda_res_75613 = ((int64_t *) mem_83749)[i_82181];
            
            ((int64_t *) mem_83765)[i_82181] = lifted_lambda_res_75612;
            ((int64_t *) mem_83767)[i_82181] = lifted_lambda_res_75613;
        }
        if (mem_83781_cached_sizze_84923 < bytes_83748) {
            err = lexical_realloc(ctx, &mem_83781, &mem_83781_cached_sizze_84923, bytes_83748);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83783_cached_sizze_84924 < bytes_83748) {
            err = lexical_realloc(ctx, &mem_83783, &mem_83783_cached_sizze_84924, bytes_83748);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_82196;
        int64_t scanacc_82190 = (int64_t) 0;
        
        for (int64_t i_82193 = 0; i_82193 < m_75577; i_82193++) {
            int64_t lifted_lambda_res_77511 = ((int64_t *) mem_83751)[i_82193];
            int64_t lifted_lambda_res_77512 = ((int64_t *) mem_83749)[i_82193];
            int64_t defunc_0_f_res_81172;
            int64_t defunc_0_f_res_81173;
            int64_t defunc_0_f_res_81174;
            int64_t redout_82184;
            int64_t redout_82185;
            int64_t redout_82186;
            
            redout_82184 = i_82193;
            redout_82185 = lifted_lambda_res_77511;
            redout_82186 = lifted_lambda_res_77512;
            for (int64_t i_82187 = 0; i_82187 < m_75577; i_82187++) {
                int64_t x_77517 = ((int64_t *) mem_83765)[i_82187];
                int64_t x_77518 = ((int64_t *) mem_83767)[i_82187];
                bool cond_77525 = redout_82185 == lifted_lambda_res_77511;
                bool cond_77526 = !cond_77525;
                bool cond_f_res_77527 = redout_82186 == lifted_lambda_res_77512;
                bool cond_f_res_77528 = !cond_f_res_77527;
                bool y_77529 = cond_77525 && cond_f_res_77528;
                bool cond_77530 = cond_77526 || y_77529;
                bool cond_77531 = slt64(i_82187, redout_82184);
                bool cond_t_res_77532 = x_77517 == lifted_lambda_res_77511;
                bool x_77533 = cond_77531 && cond_t_res_77532;
                bool cond_f_res_t_res_77534 = x_77518 == lifted_lambda_res_77512;
                bool x_77535 = x_77533 && cond_f_res_t_res_77534;
                bool x_77536 = !cond_77530;
                bool y_77537 = x_77535 && x_77536;
                bool cond_77538 = cond_77530 || y_77537;
                int64_t lifted_lambda_res_77539;
                
                if (cond_77538) {
                    lifted_lambda_res_77539 = i_82187;
                } else {
                    lifted_lambda_res_77539 = redout_82184;
                }
                
                int64_t lifted_lambda_res_77540;
                
                if (cond_77538) {
                    lifted_lambda_res_77540 = x_77517;
                } else {
                    lifted_lambda_res_77540 = redout_82185;
                }
                
                int64_t lifted_lambda_res_77541;
                
                if (cond_77538) {
                    lifted_lambda_res_77541 = x_77518;
                } else {
                    lifted_lambda_res_77541 = redout_82186;
                }
                
                int64_t redout_tmp_84696 = lifted_lambda_res_77539;
                int64_t redout_tmp_84697 = lifted_lambda_res_77540;
                int64_t redout_tmp_84698 = lifted_lambda_res_77541;
                
                redout_82184 = redout_tmp_84696;
                redout_82185 = redout_tmp_84697;
                redout_82186 = redout_tmp_84698;
            }
            defunc_0_f_res_81172 = redout_82184;
            defunc_0_f_res_81173 = redout_82185;
            defunc_0_f_res_81174 = redout_82186;
            
            bool lifted_lambda_res_77542 = defunc_0_f_res_81172 == i_82193;
            int64_t defunc_0_f_res_77544 = btoi_bool_i64(lifted_lambda_res_77542);
            int64_t defunc_0_op_res_75659 = add64(defunc_0_f_res_77544, scanacc_82190);
            
            ((int64_t *) mem_83781)[i_82193] = defunc_0_op_res_75659;
            ((int64_t *) mem_83783)[i_82193] = defunc_0_f_res_77544;
            
            int64_t scanacc_tmp_84693 = defunc_0_op_res_75659;
            
            scanacc_82190 = scanacc_tmp_84693;
        }
        discard_82196 = scanacc_82190;
        
        bool x_75660 = !empty_slice_75586;
        bool y_75661 = slt64(m_75587, m_75577);
        bool bounds_check_75662 = zzero_leq_i_p_m_t_s_75588 && y_75661;
        bool protect_assert_disj_75663 = empty_slice_75586 || bounds_check_75662;
        bool index_certs_75664;
        
        if (!protect_assert_disj_75663) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) m_75587, "] out of bounds for array of shape [", (long long) m_75577, "].", "-> #0  /prelude/soacs.fut:257:33-47\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t m_f_res_75665;
        
        if (x_75660) {
            int64_t x_81175 = ((int64_t *) mem_83781)[m_75587];
            
            m_f_res_75665 = x_81175;
        } else {
            m_f_res_75665 = (int64_t) 0;
        }
        
        int64_t m_75667;
        
        if (empty_slice_75586) {
            m_75667 = (int64_t) 0;
        } else {
            m_75667 = m_f_res_75665;
        }
        
        int64_t m_75677 = sub64(m_75667, (int64_t) 1);
        bool i_p_m_t_s_leq_w_75679 = slt64(m_75677, m_75577);
        bool zzero_leq_i_p_m_t_s_75678 = sle64((int64_t) 0, m_75677);
        bool y_75681 = zzero_leq_i_p_m_t_s_75678 && i_p_m_t_s_leq_w_75679;
        bool i_lte_j_75680 = sle64((int64_t) 0, m_75667);
        bool forwards_ok_75682 = i_lte_j_75680 && y_75681;
        bool eq_x_zz_75674 = (int64_t) 0 == m_f_res_75665;
        bool p_and_eq_x_y_75675 = x_75660 && eq_x_zz_75674;
        bool empty_slice_75676 = empty_slice_75586 || p_and_eq_x_y_75675;
        bool ok_or_empty_75683 = empty_slice_75676 || forwards_ok_75682;
        bool index_certs_75684;
        
        if (!ok_or_empty_75683) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_75667, "] out of bounds for array of shape [", (long long) m_75577, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_83796 = (int64_t) 8 * m_75667;
        
        if (mem_83797_cached_sizze_84925 < bytes_83796) {
            err = lexical_realloc(ctx, &mem_83797, &mem_83797_cached_sizze_84925, bytes_83796);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83797, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83749, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75667});
        if (mem_83799_cached_sizze_84926 < bytes_83796) {
            err = lexical_realloc(ctx, &mem_83799, &mem_83799_cached_sizze_84926, bytes_83796);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83799, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83751, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75667});
        for (int64_t write_iter_82197 = 0; write_iter_82197 < m_75577; write_iter_82197++) {
            int64_t write_iv_82200 = ((int64_t *) mem_83783)[write_iter_82197];
            int64_t write_iv_82201 = ((int64_t *) mem_83781)[write_iter_82197];
            int64_t write_iv_82202 = ((int64_t *) mem_83751)[write_iter_82197];
            int64_t write_iv_82203 = ((int64_t *) mem_83749)[write_iter_82197];
            bool cond_77497 = write_iv_82200 == (int64_t) 1;
            int64_t lifted_lambda_res_77498;
            
            if (cond_77497) {
                int64_t lifted_lambda_res_t_res_81176 = sub64(write_iv_82201, (int64_t) 1);
                
                lifted_lambda_res_77498 = lifted_lambda_res_t_res_81176;
            } else {
                lifted_lambda_res_77498 = (int64_t) -1;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77498) && slt64(lifted_lambda_res_77498, m_75667)) {
                ((int64_t *) mem_83799)[lifted_lambda_res_77498] = write_iv_82202;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77498) && slt64(lifted_lambda_res_77498, m_75667)) {
                ((int64_t *) mem_83797)[lifted_lambda_res_77498] = write_iv_82203;
            }
        }
        if (mem_83813_cached_sizze_84927 < bytes_83520) {
            err = lexical_realloc(ctx, &mem_83813, &mem_83813_cached_sizze_84927, bytes_83520);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83815_cached_sizze_84928 < bytes_83520) {
            err = lexical_realloc(ctx, &mem_83815, &mem_83815_cached_sizze_84928, bytes_83520);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83817_cached_sizze_84929 < bytes_83520) {
            err = lexical_realloc(ctx, &mem_83817, &mem_83817_cached_sizze_84929, bytes_83520);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_82220;
        int64_t scanacc_82212 = (int64_t) 0;
        
        for (int64_t i_82216 = 0; i_82216 < conc_tmp_75256; i_82216++) {
            int64_t eta_p_77463 = ((int64_t *) mem_83725)[i_82216];
            bool index_concat_cmp_82458 = sle64(conc_tmp_75255, i_82216);
            int64_t index_concat_branch_82466;
            
            if (index_concat_cmp_82458) {
                int64_t index_concat_i_82459 = sub64(i_82216, conc_tmp_75255);
                int64_t index_concat_82460 = ((int64_t *) mem_83428)[index_concat_i_82459];
                
                index_concat_branch_82466 = index_concat_82460;
            } else {
                bool index_concat_cmp_82461 = sle64(m_75179, i_82216);
                int64_t index_concat_branch_82465;
                
                if (index_concat_cmp_82461) {
                    int64_t index_concat_i_82462 = sub64(i_82216, m_75179);
                    int64_t index_concat_82463 = ((int64_t *) mem_83388)[index_concat_i_82462];
                    
                    index_concat_branch_82465 = index_concat_82463;
                } else {
                    int64_t index_concat_82464 = ((int64_t *) mem_83497)[i_82216];
                    
                    index_concat_branch_82465 = index_concat_82464;
                }
                index_concat_branch_82466 = index_concat_branch_82465;
            }
            
            int64_t defunc_0_f_res_81177;
            int64_t defunc_0_f_res_81178;
            int64_t redout_82206;
            int64_t redout_82207;
            
            redout_82206 = eta_p_77463;
            redout_82207 = eta_p_77463;
            for (int64_t i_82208 = 0; i_82208 < m_75667; i_82208++) {
                int64_t x_77467 = ((int64_t *) mem_83799)[i_82208];
                int64_t x_77468 = ((int64_t *) mem_83797)[i_82208];
                bool cond_77473 = redout_82206 == eta_p_77463;
                bool cond_77474 = !cond_77473;
                bool cond_f_res_77475 = slt64(redout_82207, (int64_t) 0);
                bool y_77476 = cond_77473 && cond_f_res_77475;
                bool cond_77477 = cond_77474 || y_77476;
                bool cond_77478 = slt64(x_77468, redout_82207);
                bool cond_f_res_t_res_77479 = x_77467 == eta_p_77463;
                bool x_77480 = cond_77478 && cond_f_res_t_res_77479;
                bool x_77481 = !cond_77477;
                bool y_77482 = x_77480 && x_77481;
                bool cond_77483 = cond_77477 || y_77482;
                int64_t lifted_lambda_res_77484;
                
                if (cond_77483) {
                    lifted_lambda_res_77484 = x_77467;
                } else {
                    lifted_lambda_res_77484 = redout_82206;
                }
                
                int64_t lifted_lambda_res_77485;
                
                if (cond_77483) {
                    lifted_lambda_res_77485 = x_77468;
                } else {
                    lifted_lambda_res_77485 = redout_82207;
                }
                
                int64_t redout_tmp_84705 = lifted_lambda_res_77484;
                int64_t redout_tmp_84706 = lifted_lambda_res_77485;
                
                redout_82206 = redout_tmp_84705;
                redout_82207 = redout_tmp_84706;
            }
            defunc_0_f_res_81177 = redout_82206;
            defunc_0_f_res_81178 = redout_82207;
            
            bool cond_77487 = slt64(defunc_0_f_res_81178, old_clHandler_73938);
            bool lifted_lambda_res_t_res_77488 = sle64((int64_t) 0, index_concat_branch_82466);
            bool x_77489 = cond_77487 && lifted_lambda_res_t_res_77488;
            int64_t defunc_0_f_res_77490 = btoi_bool_i64(x_77489);
            int64_t defunc_0_op_res_75728 = add64(defunc_0_f_res_77490, scanacc_82212);
            
            ((int64_t *) mem_83813)[i_82216] = defunc_0_op_res_75728;
            ((int64_t *) mem_83815)[i_82216] = defunc_0_f_res_77490;
            ((int64_t *) mem_83817)[i_82216] = defunc_0_f_res_81178;
            
            int64_t scanacc_tmp_84701 = defunc_0_op_res_75728;
            
            scanacc_82212 = scanacc_tmp_84701;
        }
        discard_82220 = scanacc_82212;
        
        int64_t m_f_res_75729;
        
        if (x_75568) {
            int64_t x_81179 = ((int64_t *) mem_83813)[tmp_75569];
            
            m_f_res_75729 = x_81179;
        } else {
            m_f_res_75729 = (int64_t) 0;
        }
        
        int64_t m_75731;
        
        if (eq_x_zz_75525) {
            m_75731 = (int64_t) 0;
        } else {
            m_75731 = m_f_res_75729;
        }
        
        int64_t m_75741 = sub64(m_75731, (int64_t) 1);
        bool i_p_m_t_s_leq_w_75743 = slt64(m_75741, conc_tmp_75256);
        bool zzero_leq_i_p_m_t_s_75742 = sle64((int64_t) 0, m_75741);
        bool y_75745 = zzero_leq_i_p_m_t_s_75742 && i_p_m_t_s_leq_w_75743;
        bool i_lte_j_75744 = sle64((int64_t) 0, m_75731);
        bool forwards_ok_75746 = i_lte_j_75744 && y_75745;
        bool eq_x_zz_75738 = (int64_t) 0 == m_f_res_75729;
        bool p_and_eq_x_y_75739 = x_75568 && eq_x_zz_75738;
        bool empty_slice_75740 = eq_x_zz_75525 || p_and_eq_x_y_75739;
        bool ok_or_empty_75747 = empty_slice_75740 || forwards_ok_75746;
        bool index_certs_75748;
        
        if (!ok_or_empty_75747) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_75731, "] out of bounds for array of shape [", (long long) conc_tmp_75256, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_83836 = (int64_t) 8 * m_75731;
        
        if (mem_83837_cached_sizze_84930 < bytes_83836) {
            err = lexical_realloc(ctx, &mem_83837, &mem_83837_cached_sizze_84930, bytes_83836);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83837, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83817, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75731});
        if (mem_83839_cached_sizze_84931 < bytes_83836) {
            err = lexical_realloc(ctx, &mem_83839, &mem_83839_cached_sizze_84931, bytes_83836);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83839, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83521, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75731});
        for (int64_t write_iter_82221 = 0; write_iter_82221 < conc_tmp_75256; write_iter_82221++) {
            int64_t write_iv_82224 = ((int64_t *) mem_83815)[write_iter_82221];
            int64_t write_iv_82225 = ((int64_t *) mem_83813)[write_iter_82221];
            bool index_concat_cmp_82447 = sle64(conc_tmp_75255, write_iter_82221);
            int64_t index_concat_branch_82455;
            
            if (index_concat_cmp_82447) {
                int64_t index_concat_i_82448 = sub64(write_iter_82221, conc_tmp_75255);
                int64_t index_concat_82449 = ((int64_t *) mem_83428)[index_concat_i_82448];
                
                index_concat_branch_82455 = index_concat_82449;
            } else {
                bool index_concat_cmp_82450 = sle64(m_75179, write_iter_82221);
                int64_t index_concat_branch_82454;
                
                if (index_concat_cmp_82450) {
                    int64_t index_concat_i_82451 = sub64(write_iter_82221, m_75179);
                    int64_t index_concat_82452 = ((int64_t *) mem_83388)[index_concat_i_82451];
                    
                    index_concat_branch_82454 = index_concat_82452;
                } else {
                    int64_t index_concat_82453 = ((int64_t *) mem_83497)[write_iter_82221];
                    
                    index_concat_branch_82454 = index_concat_82453;
                }
                index_concat_branch_82455 = index_concat_branch_82454;
            }
            
            int64_t write_iv_82227 = ((int64_t *) mem_83817)[write_iter_82221];
            bool cond_77450 = write_iv_82224 == (int64_t) 1;
            int64_t lifted_lambda_res_77451;
            
            if (cond_77450) {
                int64_t lifted_lambda_res_t_res_81180 = sub64(write_iv_82225, (int64_t) 1);
                
                lifted_lambda_res_77451 = lifted_lambda_res_t_res_81180;
            } else {
                lifted_lambda_res_77451 = (int64_t) -1;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77451) && slt64(lifted_lambda_res_77451, m_75731)) {
                ((int64_t *) mem_83839)[lifted_lambda_res_77451] = index_concat_branch_82455;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77451) && slt64(lifted_lambda_res_77451, m_75731)) {
                ((int64_t *) mem_83837)[lifted_lambda_res_77451] = write_iv_82227;
            }
        }
        if (mem_83853_cached_sizze_84932 < bytes_83836) {
            err = lexical_realloc(ctx, &mem_83853, &mem_83853_cached_sizze_84932, bytes_83836);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83855_cached_sizze_84933 < bytes_83836) {
            err = lexical_realloc(ctx, &mem_83855, &mem_83855_cached_sizze_84933, bytes_83836);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_82242;
        int64_t scanacc_82236 = (int64_t) 0;
        
        for (int64_t i_82239 = 0; i_82239 < m_75731; i_82239++) {
            int64_t lifted_lambda_res_77401 = ((int64_t *) mem_83839)[i_82239];
            int64_t lifted_lambda_res_77402 = ((int64_t *) mem_83837)[i_82239];
            int64_t defunc_0_f_res_81181;
            int64_t defunc_0_f_res_81182;
            int64_t defunc_0_f_res_81183;
            int64_t redout_82230;
            int64_t redout_82231;
            int64_t redout_82232;
            
            redout_82230 = i_82239;
            redout_82231 = lifted_lambda_res_77401;
            redout_82232 = lifted_lambda_res_77402;
            for (int64_t i_82233 = 0; i_82233 < m_75731; i_82233++) {
                int64_t x_77407 = ((int64_t *) mem_83839)[i_82233];
                int64_t x_77408 = ((int64_t *) mem_83837)[i_82233];
                bool cond_77415 = redout_82232 == lifted_lambda_res_77402;
                bool cond_77416 = !cond_77415;
                bool cond_f_res_77417 = redout_82232 == redout_82231;
                bool y_77418 = cond_77415 && cond_f_res_77417;
                bool cond_77419 = cond_77416 || y_77418;
                bool cond_77420 = x_77408 == lifted_lambda_res_77402;
                bool cond_77421 = slt64(x_77407, redout_82231);
                bool cond_t_res_77422 = x_77407 == x_77408;
                bool cond_t_res_77423 = !cond_t_res_77422;
                bool x_77424 = cond_77421 && cond_t_res_77423;
                bool cond_77425 = x_77407 == redout_82231;
                bool cond_f_res_t_res_f_res_t_res_77426 = slt64(i_82233, redout_82230);
                bool x_77427 = cond_77425 && cond_f_res_t_res_f_res_t_res_77426;
                bool x_77428 = !x_77424;
                bool y_77429 = x_77427 && x_77428;
                bool cond_f_res_t_res_77430 = x_77424 || y_77429;
                bool x_77431 = cond_77420 && cond_f_res_t_res_77430;
                bool x_77432 = !cond_77419;
                bool y_77433 = x_77431 && x_77432;
                bool cond_77434 = cond_77419 || y_77433;
                int64_t lifted_lambda_res_77435;
                
                if (cond_77434) {
                    lifted_lambda_res_77435 = i_82233;
                } else {
                    lifted_lambda_res_77435 = redout_82230;
                }
                
                int64_t lifted_lambda_res_77436;
                
                if (cond_77434) {
                    lifted_lambda_res_77436 = x_77408;
                } else {
                    lifted_lambda_res_77436 = redout_82232;
                }
                
                int64_t lifted_lambda_res_77437;
                
                if (cond_77434) {
                    lifted_lambda_res_77437 = x_77407;
                } else {
                    lifted_lambda_res_77437 = redout_82231;
                }
                
                int64_t redout_tmp_84712 = lifted_lambda_res_77435;
                int64_t redout_tmp_84713 = lifted_lambda_res_77436;
                int64_t redout_tmp_84714 = lifted_lambda_res_77437;
                
                redout_82230 = redout_tmp_84712;
                redout_82231 = redout_tmp_84713;
                redout_82232 = redout_tmp_84714;
            }
            defunc_0_f_res_81181 = redout_82230;
            defunc_0_f_res_81182 = redout_82231;
            defunc_0_f_res_81183 = redout_82232;
            
            bool cond_77438 = i_82239 == defunc_0_f_res_81181;
            bool lifted_lambda_res_t_res_77439 = defunc_0_f_res_81182 == defunc_0_f_res_81183;
            bool lifted_lambda_res_t_res_77440 = !lifted_lambda_res_t_res_77439;
            bool x_77441 = cond_77438 && lifted_lambda_res_t_res_77440;
            int64_t defunc_0_f_res_77443 = btoi_bool_i64(x_77441);
            int64_t defunc_0_op_res_75813 = add64(defunc_0_f_res_77443, scanacc_82236);
            
            ((int64_t *) mem_83853)[i_82239] = defunc_0_op_res_75813;
            ((int64_t *) mem_83855)[i_82239] = defunc_0_f_res_77443;
            
            int64_t scanacc_tmp_84709 = defunc_0_op_res_75813;
            
            scanacc_82236 = scanacc_tmp_84709;
        }
        discard_82242 = scanacc_82236;
        
        bool x_75814 = !empty_slice_75740;
        bool y_75815 = slt64(m_75741, m_75731);
        bool bounds_check_75816 = zzero_leq_i_p_m_t_s_75742 && y_75815;
        bool protect_assert_disj_75817 = empty_slice_75740 || bounds_check_75816;
        bool index_certs_75818;
        
        if (!protect_assert_disj_75817) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) m_75741, "] out of bounds for array of shape [", (long long) m_75731, "].", "-> #0  /prelude/soacs.fut:257:33-47\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t m_f_res_75819;
        
        if (x_75814) {
            int64_t x_81184 = ((int64_t *) mem_83853)[m_75741];
            
            m_f_res_75819 = x_81184;
        } else {
            m_f_res_75819 = (int64_t) 0;
        }
        
        int64_t m_75821;
        
        if (empty_slice_75740) {
            m_75821 = (int64_t) 0;
        } else {
            m_75821 = m_f_res_75819;
        }
        
        int64_t m_75831 = sub64(m_75821, (int64_t) 1);
        bool i_p_m_t_s_leq_w_75833 = slt64(m_75831, m_75731);
        bool zzero_leq_i_p_m_t_s_75832 = sle64((int64_t) 0, m_75831);
        bool y_75835 = zzero_leq_i_p_m_t_s_75832 && i_p_m_t_s_leq_w_75833;
        bool i_lte_j_75834 = sle64((int64_t) 0, m_75821);
        bool forwards_ok_75836 = i_lte_j_75834 && y_75835;
        bool eq_x_zz_75828 = (int64_t) 0 == m_f_res_75819;
        bool p_and_eq_x_y_75829 = x_75814 && eq_x_zz_75828;
        bool empty_slice_75830 = empty_slice_75740 || p_and_eq_x_y_75829;
        bool ok_or_empty_75837 = empty_slice_75830 || forwards_ok_75836;
        bool index_certs_75838;
        
        if (!ok_or_empty_75837) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_75821, "] out of bounds for array of shape [", (long long) m_75731, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_83868 = (int64_t) 8 * m_75821;
        
        if (mem_83885_cached_sizze_84936 < bytes_83520) {
            err = lexical_realloc(ctx, &mem_83885, &mem_83885_cached_sizze_84936, bytes_83520);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83887_cached_sizze_84937 < bytes_83520) {
            err = lexical_realloc(ctx, &mem_83887, &mem_83887_cached_sizze_84937, bytes_83520);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83889_cached_sizze_84938 < bytes_83520) {
            err = lexical_realloc(ctx, &mem_83889, &mem_83889_cached_sizze_84938, bytes_83520);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83891_cached_sizze_84939 < bytes_83520) {
            err = lexical_realloc(ctx, &mem_83891, &mem_83891_cached_sizze_84939, bytes_83520);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83893_cached_sizze_84940 < bytes_83520) {
            err = lexical_realloc(ctx, &mem_83893, &mem_83893_cached_sizze_84940, bytes_83520);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_82269;
        int64_t scanacc_82257 = (int64_t) 0;
        
        for (int64_t i_82263 = 0; i_82263 < conc_tmp_75256; i_82263++) {
            int64_t tmp_77276 = ((int64_t *) mem_83817)[i_82263];
            bool index_concat_cmp_77278 = sle64(conc_tmp_75255, i_82263);
            double index_concat_branch_77279;
            
            if (index_concat_cmp_77278) {
                int64_t index_concat_i_81186 = sub64(i_82263, conc_tmp_75255);
                double index_concat_81187 = ((double *) mem_83432)[index_concat_i_81186];
                
                index_concat_branch_77279 = index_concat_81187;
            } else {
                bool index_concat_cmp_77282 = sle64(m_75179, i_82263);
                double index_concat_branch_77283;
                
                if (index_concat_cmp_77282) {
                    int64_t index_concat_i_81188 = sub64(i_82263, m_75179);
                    double index_concat_81189 = ((double *) mem_83392)[index_concat_i_81188];
                    
                    index_concat_branch_77283 = index_concat_81189;
                } else {
                    double index_concat_77286 = ((double *) mem_83501)[i_82263];
                    
                    index_concat_branch_77283 = index_concat_77286;
                }
                index_concat_branch_77279 = index_concat_branch_77283;
            }
            
            double index_concat_branch_77287;
            
            if (index_concat_cmp_77278) {
                int64_t index_concat_i_81190 = sub64(i_82263, conc_tmp_75255);
                double index_concat_81191 = ((double *) mem_83430)[index_concat_i_81190];
                
                index_concat_branch_77287 = index_concat_81191;
            } else {
                bool index_concat_cmp_77290 = sle64(m_75179, i_82263);
                double index_concat_branch_77291;
                
                if (index_concat_cmp_77290) {
                    int64_t index_concat_i_81192 = sub64(i_82263, m_75179);
                    double index_concat_81193 = ((double *) mem_83390)[index_concat_i_81192];
                    
                    index_concat_branch_77291 = index_concat_81193;
                } else {
                    double index_concat_77294 = ((double *) mem_83499)[i_82263];
                    
                    index_concat_branch_77291 = index_concat_77294;
                }
                index_concat_branch_77287 = index_concat_branch_77291;
            }
            
            bool zgze_res_77298 = minC_74043 <= index_concat_branch_77287;
            bool zlze_res_77299 = index_concat_branch_77287 <= maxC_74054;
            bool x_77300 = zgze_res_77298 && zlze_res_77299;
            bool eq_x_y_77301 = index_concat_branch_77279 == -INFINITY;
            bool eq_x_zz_77302 = index_concat_branch_77279 == zp_res_74037;
            bool p_and_eq_x_y_77303 = cond_74034 && eq_x_y_77301;
            bool p_and_eq_x_y_77304 = not_p_74786 && eq_x_zz_77302;
            bool zeze_res_77305 = p_and_eq_x_y_77303 || p_and_eq_x_y_77304;
            double d_euclidean_res_77306;
            
            if (zeze_res_77305) {
                double zm_res_81194 = index_concat_branch_77287 - index_concat_branch_77287;
                double abs_res_81195 = fabs64(zm_res_81194);
                
                d_euclidean_res_77306 = abs_res_81195;
            } else {
                double zm_res_77309 = index_concat_branch_77279 - minC_74038;
                double abs_res_77310 = fabs64(zm_res_77309);
                
                d_euclidean_res_77306 = abs_res_77310;
            }
            
            bool zlze_res_77311 = d_euclidean_res_77306 <= 2.0;
            bool eq_x_y_77312 = index_concat_branch_77279 == INFINITY;
            bool eq_x_zz_77313 = index_concat_branch_77279 == zp_res_74048;
            bool p_and_eq_x_y_77314 = cond_74044 && eq_x_y_77312;
            bool p_and_eq_x_y_77315 = not_p_74787 && eq_x_zz_77313;
            bool zeze_res_77316 = p_and_eq_x_y_77314 || p_and_eq_x_y_77315;
            double d_euclidean_res_77317;
            
            if (zeze_res_77316) {
                double zm_res_81196 = index_concat_branch_77287 - index_concat_branch_77287;
                double abs_res_81197 = fabs64(zm_res_81196);
                
                d_euclidean_res_77317 = abs_res_81197;
            } else {
                double zm_res_77320 = index_concat_branch_77279 - maxC_74049;
                double abs_res_77321 = fabs64(zm_res_77320);
                
                d_euclidean_res_77317 = abs_res_77321;
            }
            
            bool zlze_res_77322 = d_euclidean_res_77317 <= 2.0;
            bool x_77323 = !zlze_res_77311;
            bool y_77324 = zlze_res_77322 && x_77323;
            bool cond_t_res_77325 = zlze_res_77311 || y_77324;
            bool x_77326 = x_77300 && cond_t_res_77325;
            bool zgze_res_77327 = minC_74038 <= index_concat_branch_77279;
            bool zlze_res_77328 = index_concat_branch_77279 <= maxC_74049;
            bool x_77329 = zgze_res_77327 && zlze_res_77328;
            double zm_res_77330 = index_concat_branch_77287 - minC_74043;
            double abs_res_77331 = fabs64(zm_res_77330);
            bool zlze_res_77332 = abs_res_77331 <= 2.0;
            double zm_res_77333 = index_concat_branch_77287 - maxC_74054;
            double abs_res_77334 = fabs64(zm_res_77333);
            bool zlze_res_77335 = abs_res_77334 <= 2.0;
            bool x_77336 = !zlze_res_77332;
            bool y_77337 = zlze_res_77335 && x_77336;
            bool cond_f_res_t_res_77338 = zlze_res_77332 || y_77337;
            bool x_77339 = x_77329 && cond_f_res_t_res_77338;
            bool x_77340 = !x_77326;
            bool y_77341 = x_77339 && x_77340;
            bool cond_77342 = x_77326 || y_77341;
            bool zlze_res_77343 = index_concat_branch_77279 <= minC_74038;
            double corn_x_77344;
            
            if (zlze_res_77343) {
                corn_x_77344 = minC_74038;
            } else {
                corn_x_77344 = maxC_74049;
            }
            
            bool zlze_res_77345 = index_concat_branch_77287 <= minC_74043;
            double corn_y_77346;
            
            if (zlze_res_77345) {
                corn_y_77346 = minC_74043;
            } else {
                corn_y_77346 = maxC_74054;
            }
            
            bool p_and_eq_x_y_77347 = zeze_res_77305 && zlze_res_77343;
            bool not_p_77348 = !zlze_res_77343;
            bool p_and_eq_x_y_77349 = zeze_res_77316 && not_p_77348;
            bool zeze_res_77350 = p_and_eq_x_y_77347 || p_and_eq_x_y_77349;
            double d_euclidean_res_77351;
            
            if (zeze_res_77350) {
                double zm_res_81198 = index_concat_branch_77287 - corn_y_77346;
                double abs_res_81199 = fabs64(zm_res_81198);
                
                d_euclidean_res_77351 = abs_res_81199;
            } else {
                bool eq_x_y_77354 = index_concat_branch_77287 == -INFINITY;
                bool eq_x_zz_77355 = index_concat_branch_77287 == zp_res_74042;
                bool p_and_eq_x_y_77356 = cond_74039 && eq_x_y_77354;
                bool not_p_77357 = !cond_74039;
                bool p_and_eq_x_y_77358 = eq_x_zz_77355 && not_p_77357;
                bool eq_x_y_77359 = p_and_eq_x_y_77356 || p_and_eq_x_y_77358;
                bool eq_x_y_77360 = index_concat_branch_77287 == INFINITY;
                bool eq_x_zz_77361 = index_concat_branch_77287 == zp_res_74053;
                bool p_and_eq_x_y_77362 = cond_74050 && eq_x_y_77360;
                bool not_p_77363 = !cond_74050;
                bool p_and_eq_x_y_77364 = eq_x_zz_77361 && not_p_77363;
                bool eq_x_zz_77365 = p_and_eq_x_y_77362 || p_and_eq_x_y_77364;
                bool p_and_eq_x_y_77366 = zlze_res_77345 && eq_x_y_77359;
                bool not_p_77367 = !zlze_res_77345;
                bool p_and_eq_x_y_77368 = eq_x_zz_77365 && not_p_77367;
                bool zeze_res_77369 = p_and_eq_x_y_77366 || p_and_eq_x_y_77368;
                double d_euclidean_res_f_res_77370;
                
                if (zeze_res_77369) {
                    double zm_res_81200 = index_concat_branch_77279 - corn_x_77344;
                    double abs_res_81201 = fabs64(zm_res_81200);
                    
                    d_euclidean_res_f_res_77370 = abs_res_81201;
                } else {
                    double zm_res_77373 = index_concat_branch_77287 - corn_y_77346;
                    double zm_res_77374 = index_concat_branch_77279 - corn_x_77344;
                    double hypot_res_77375 = futrts_hypot64(zm_res_77374, zm_res_77373);
                    
                    d_euclidean_res_f_res_77370 = hypot_res_77375;
                }
                d_euclidean_res_77351 = d_euclidean_res_f_res_77370;
            }
            
            bool zlze_res_77376 = d_euclidean_res_77351 <= 2.0;
            bool x_77377 = !cond_77342;
            bool y_77378 = zlze_res_77376 && x_77377;
            bool is_pt_marginal_res_77379 = cond_77342 || y_77378;
            int64_t defunc_0_f_res_77380 = btoi_bool_i64(is_pt_marginal_res_77379);
            int64_t defunc_0_op_res_75969 = add64(defunc_0_f_res_77380, scanacc_82257);
            
            ((int64_t *) mem_83885)[i_82263] = defunc_0_op_res_75969;
            ((int64_t *) mem_83887)[i_82263] = defunc_0_f_res_77380;
            ((int64_t *) mem_83889)[i_82263] = tmp_77276;
            ((double *) mem_83891)[i_82263] = index_concat_branch_77279;
            ((double *) mem_83893)[i_82263] = index_concat_branch_77287;
            
            int64_t scanacc_tmp_84715 = defunc_0_op_res_75969;
            
            scanacc_82257 = scanacc_tmp_84715;
        }
        discard_82269 = scanacc_82257;
        
        int64_t m_f_res_75970;
        
        if (x_75568) {
            int64_t x_81202 = ((int64_t *) mem_83885)[tmp_75569];
            
            m_f_res_75970 = x_81202;
        } else {
            m_f_res_75970 = (int64_t) 0;
        }
        
        int64_t m_75972;
        
        if (eq_x_zz_75525) {
            m_75972 = (int64_t) 0;
        } else {
            m_75972 = m_f_res_75970;
        }
        
        int64_t m_75982 = sub64(m_75972, (int64_t) 1);
        bool i_p_m_t_s_leq_w_75984 = slt64(m_75982, conc_tmp_75256);
        bool zzero_leq_i_p_m_t_s_75983 = sle64((int64_t) 0, m_75982);
        bool y_75986 = zzero_leq_i_p_m_t_s_75983 && i_p_m_t_s_leq_w_75984;
        bool i_lte_j_75985 = sle64((int64_t) 0, m_75972);
        bool forwards_ok_75987 = i_lte_j_75985 && y_75986;
        bool eq_x_zz_75979 = (int64_t) 0 == m_f_res_75970;
        bool p_and_eq_x_y_75980 = x_75568 && eq_x_zz_75979;
        bool empty_slice_75981 = eq_x_zz_75525 || p_and_eq_x_y_75980;
        bool ok_or_empty_75988 = empty_slice_75981 || forwards_ok_75987;
        bool index_certs_75989;
        
        if (!ok_or_empty_75988) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_75972, "] out of bounds for array of shape [", (long long) conc_tmp_75256, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_83924 = (int64_t) 8 * m_75972;
        int64_t max_res_76003 = smax64((int64_t) 1, m_75972);
        bool zzero_76004 = max_res_76003 == (int64_t) 0;
        bool nonzzero_76005 = !zzero_76004;
        bool nonzzero_cert_76006;
        
        if (!nonzzero_76005) {
            set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../ftDBSCAN_plus.fut:733:58-94\n   #1  test_dbscan_plus.fut:14:23-19:30\n   #2  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t max_arg1_76007 = sdiv64((int64_t) 9223372036854775807, max_res_76003);
        int64_t max_res_76008 = smax64((int64_t) 1, max_arg1_76007);
        int64_t zm_lhs_76009 = add64(m_74888, max_res_76008);
        int64_t zs_lhs_76010 = sub64(zm_lhs_76009, (int64_t) 1);
        bool zzero_76011 = max_res_76008 == (int64_t) 0;
        bool nonzzero_76012 = !zzero_76011;
        bool nonzzero_cert_76013;
        
        if (!nonzzero_76012) {
            set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../ftDBSCAN_plus.fut:734:71-81\n   #1  test_dbscan_plus.fut:14:23-19:30\n   #2  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t max_arg1_76014 = sdiv64(zs_lhs_76010, max_res_76008);
        int64_t max_res_76015 = smax64((int64_t) 0, max_arg1_76014);
        bool bounds_invalid_upwards_76017 = slt64(max_res_76015, (int64_t) 0);
        bool valid_76018 = !bounds_invalid_upwards_76017;
        bool range_valid_c_76019;
        
        if (!valid_76018) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) max_res_76015, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  ../../ftDBSCAN_plus.fut:736:75-88\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t zm_lhs_76075 = add64(loopres_73979, max_res_75267);
        int64_t zs_lhs_76076 = sub64(zm_lhs_76075, (int64_t) 1);
        int64_t max_arg1_76080 = sdiv64(zs_lhs_76076, max_res_75267);
        int64_t max_res_76081 = smax64((int64_t) 0, max_arg1_76080);
        bool bounds_invalid_upwards_76083 = slt64(max_res_76081, (int64_t) 0);
        bool valid_76084 = !bounds_invalid_upwards_76083;
        bool range_valid_c_76085;
        
        if (!valid_76084) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) max_res_76081, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  ../../ftDBSCAN_plus.fut:753:75-88\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t conc_tmp_76160 = loop_dz2085Uz2085U_73918 + m_75821;
        int64_t bytes_83992 = (int64_t) 8 * conc_tmp_76160;
        bool cond_76163 = new_proc_74723 == (int64_t) 24;
        bool x_76164 = !cond_76163;
        bool cond_76165 = slt64((int64_t) 0, m_74211);
        bool protect_cond_conj_76166 = x_76164 && cond_76165;
        int64_t next_partition_to_read_res_f_res_t_res_76167;
        
        if (protect_cond_conj_76166) {
            int64_t x_81222 = ((int64_t *) mem_82976.mem)[(int64_t) 0];
            
            next_partition_to_read_res_f_res_t_res_76167 = x_81222;
        } else {
            next_partition_to_read_res_f_res_t_res_76167 = (int64_t) 0;
        }
        
        int64_t next_partition_to_read_res_f_res_76169;
        
        if (x_76164) {
            bool x_81679 = !cond_76165;
            int64_t defunc_res_81274;
            
            if (x_81679) {
                int64_t x_81681;
                int64_t redout_82310 = (int64_t) 0;
                
                for (int64_t i_82311 = 0; i_82311 < (int64_t) 25; i_82311++) {
                    bool eta_p_81275 = ((bool *) mem_82923.mem)[i_82311];
                    bool lifted_lambda_res_81276 = !eta_p_81275;
                    int64_t bool_res_81277 = btoi_bool_i64(lifted_lambda_res_81276);
                    int64_t zp_res_81280 = add64(bool_res_81277, redout_82310);
                    int64_t redout_tmp_84721 = zp_res_81280;
                    
                    redout_82310 = redout_tmp_84721;
                }
                x_81681 = redout_82310;
                defunc_res_81274 = x_81681;
            } else {
                defunc_res_81274 = (int64_t) 0;
            }
            
            bool cond_81285 = slt64((int64_t) 0, defunc_res_81274);
            int64_t next_partition_to_read_res_f_res_f_res_81286;
            
            if (x_81679) {
                int64_t x_81683;
                
                if (cond_81285) {
                    if (mem_83996_cached_sizze_84945 < (int64_t) 200) {
                        err = lexical_realloc(ctx, &mem_83996, &mem_83996_cached_sizze_84945, (int64_t) 200);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    if (mem_83997_cached_sizze_84946 < (int64_t) 200) {
                        err = lexical_realloc(ctx, &mem_83997, &mem_83997_cached_sizze_84946, (int64_t) 200);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    
                    int64_t discard_82320;
                    int64_t scanacc_82314 = (int64_t) 0;
                    
                    for (int64_t i_82317 = 0; i_82317 < (int64_t) 25; i_82317++) {
                        bool eta_p_81290 = ((bool *) mem_82923.mem)[i_82317];
                        bool lifted_lambda_res_81291 = !eta_p_81290;
                        int64_t defunc_0_f_res_81292 = btoi_bool_i64(lifted_lambda_res_81291);
                        int64_t defunc_0_op_res_81295 = add64(defunc_0_f_res_81292, scanacc_82314);
                        
                        ((int64_t *) mem_83996)[i_82317] = defunc_0_op_res_81295;
                        ((int64_t *) mem_83997)[i_82317] = defunc_0_f_res_81292;
                        
                        int64_t scanacc_tmp_84722 = defunc_0_op_res_81295;
                        
                        scanacc_82314 = scanacc_tmp_84722;
                    }
                    discard_82320 = scanacc_82314;
                    
                    int64_t x_81306 = ((int64_t *) mem_83996)[(int64_t) 24];
                    int64_t m_81307 = sub64(x_81306, (int64_t) 1);
                    bool i_p_m_t_s_leq_w_81308 = slt64(m_81307, (int64_t) 25);
                    bool zzero_leq_i_p_m_t_s_81309 = sle64((int64_t) 0, m_81307);
                    bool y_81310 = i_p_m_t_s_leq_w_81308 && zzero_leq_i_p_m_t_s_81309;
                    bool i_lte_j_81311 = sle64((int64_t) 0, x_81306);
                    bool forwards_ok_81312 = y_81310 && i_lte_j_81311;
                    bool eq_x_zz_81313 = (int64_t) 0 == x_81306;
                    bool ok_or_empty_81314 = forwards_ok_81312 || eq_x_zz_81313;
                    bool index_certs_81315;
                    
                    if (!ok_or_empty_81314) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) x_81306, "] out of bounds for array of shape [", (long long) (int64_t) 25, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t bytes_84010 = (int64_t) 8 * x_81306;
                    bool y_81329 = slt64((int64_t) 0, x_81306);
                    bool index_certs_81330;
                    
                    if (!y_81329) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) x_81306, "].", "-> #0  ../../ftDBSCAN_plus.fut:267:44-56\n   #1  test_dbscan_plus.fut:14:23-19:30\n   #2  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    if (mem_84011_cached_sizze_84947 < bytes_84010) {
                        err = lexical_realloc(ctx, &mem_84011, &mem_84011_cached_sizze_84947, bytes_84010);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    for (int64_t i_84725 = 0; i_84725 < x_81306; i_84725++) {
                        int64_t x_84726 = (int64_t) 0 + i_84725 * (int64_t) 1;
                        
                        ((int64_t *) mem_84011)[i_84725] = x_84726;
                    }
                    for (int64_t write_iter_82321 = 0; write_iter_82321 < (int64_t) 25; write_iter_82321++) {
                        int64_t write_iv_82323 = ((int64_t *) mem_83997)[write_iter_82321];
                        int64_t write_iv_82324 = ((int64_t *) mem_83996)[write_iter_82321];
                        bool cond_81321 = write_iv_82323 == (int64_t) 1;
                        int64_t lifted_lambda_res_81322;
                        
                        if (cond_81321) {
                            int64_t lifted_lambda_res_t_res_81323 = sub64(write_iv_82324, (int64_t) 1);
                            
                            lifted_lambda_res_81322 = lifted_lambda_res_t_res_81323;
                        } else {
                            lifted_lambda_res_81322 = (int64_t) -1;
                        }
                        if (sle64((int64_t) 0, lifted_lambda_res_81322) && slt64(lifted_lambda_res_81322, x_81306)) {
                            ((int64_t *) mem_84011)[lifted_lambda_res_81322] = write_iter_82321;
                        }
                    }
                    
                    int64_t next_partition_to_read_res_f_res_f_res_t_res_81331 = ((int64_t *) mem_84011)[(int64_t) 0];
                    
                    x_81683 = next_partition_to_read_res_f_res_f_res_t_res_81331;
                } else {
                    x_81683 = (int64_t) -1;
                }
                next_partition_to_read_res_f_res_f_res_81286 = x_81683;
            } else {
                next_partition_to_read_res_f_res_f_res_81286 = (int64_t) 0;
            }
            
            int64_t x_81273;
            
            if (cond_76165) {
                x_81273 = next_partition_to_read_res_f_res_t_res_76167;
            } else {
                x_81273 = next_partition_to_read_res_f_res_f_res_81286;
            }
            next_partition_to_read_res_f_res_76169 = x_81273;
        } else {
            next_partition_to_read_res_f_res_76169 = (int64_t) 0;
        }
        
        int64_t next_partition_to_read_res_76215;
        
        if (cond_76163) {
            next_partition_to_read_res_76215 = (int64_t) -1;
        } else {
            next_partition_to_read_res_76215 = next_partition_to_read_res_f_res_76169;
        }
        
        bool cond_76216 = sle64((int64_t) 0, next_partition_to_read_res_76215);
        int64_t dzlz7bUZLzpZRz20UZLZLzpZRz20Udz2085Uz20Udz2082UZRz20Unz7dUzg_76217 = add64(loopres_73979, partition_sizze_74623);
        int64_t conc_tmp_76218 = conc_tmp_74722 + dzlz7bUZLzpZRz20UZLZLzpZRz20Udz2085Uz20Udz2082UZRz20Unz7dUzg_76217;
        int64_t loopres_76219;
        
        if (cond_76216) {
            loopres_76219 = conc_tmp_74722;
        } else {
            loopres_76219 = conc_tmp_76218;
        }
        
        int64_t bytes_84018 = (int64_t) 8 * loopres_76219;
        bool x_76229 = !cond_76216;
        bool cond_76230 = sle64(conc_tmp_76160, (int64_t) 1);
        int64_t zs_lhs_76231 = sub64(conc_tmp_76160, (int64_t) 1);
        int64_t zp_rhs_76232 = sdiv_safe64(zs_lhs_76231, (int64_t) 20);
        int64_t min_num_blocks_76233 = add64((int64_t) 1, zp_rhs_76232);
        bool loop_cond_76234 = slt64((int64_t) 1, min_num_blocks_76233);
        bool x_76235;
        int64_t x_76236;
        bool loop_while_76237;
        int64_t x_76238;
        
        loop_while_76237 = loop_cond_76234;
        x_76238 = (int64_t) 1;
        while (loop_while_76237) {
            int64_t loopres_76239 = mul64((int64_t) 2, x_76238);
            bool loop_cond_76240 = slt64(loopres_76239, min_num_blocks_76233);
            bool loop_while_tmp_84728 = loop_cond_76240;
            int64_t x_tmp_84729 = loopres_76239;
            
            loop_while_76237 = loop_while_tmp_84728;
            x_76238 = x_tmp_84729;
        }
        x_76235 = loop_while_76237;
        x_76236 = x_76238;
        
        bool zzero_76241 = x_76236 == (int64_t) 0;
        bool nonzzero_76242 = !zzero_76241;
        bool protect_assert_disj_76243 = cond_76230 || nonzzero_76242;
        bool protect_assert_disj_76244 = cond_76216 || protect_assert_disj_76243;
        bool nonzzero_cert_76245;
        
        if (!protect_assert_disj_76244) {
            set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:234:30-42\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #2  /prelude/functional.fut:9:44-45\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t zp_rhs_76246 = sdiv_safe64(zs_lhs_76231, x_76236);
        int64_t block_sizze_76247 = add64((int64_t) 1, zp_rhs_76246);
        bool y_76254 = slt64((int64_t) 0, conc_tmp_76160);
        bool protect_assert_disj_76255 = cond_76230 || y_76254;
        bool protect_assert_disj_76256 = cond_76216 || protect_assert_disj_76255;
        bool index_certs_76257;
        
        if (!protect_assert_disj_76256) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) conc_tmp_76160, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:240:60-64\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #2  /prelude/functional.fut:9:44-45\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        bool bounds_invalid_upwards_76282 = slt64(x_76236, (int64_t) 0);
        bool valid_76283 = !bounds_invalid_upwards_76282;
        bool protect_assert_disj_76284 = cond_76230 || valid_76283;
        bool protect_assert_disj_76285 = cond_76216 || protect_assert_disj_76284;
        bool range_valid_c_76286;
        
        if (!protect_assert_disj_76285) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) x_76236, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:243:35-248:37\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #4  /prelude/functional.fut:9:44-45\n   #5  test_dbscan_plus.fut:14:23-19:30\n   #6  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        bool bounds_invalid_upwards_76287 = slt64(block_sizze_76247, (int64_t) 0);
        bool valid_76288 = !bounds_invalid_upwards_76287;
        bool protect_assert_disj_76289 = cond_76230 || valid_76288;
        bool protect_assert_disj_76290 = cond_76216 || protect_assert_disj_76289;
        bool range_valid_c_76291;
        
        if (!protect_assert_disj_76290) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) block_sizze_76247, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:244:40-246:44\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:243:35-248:37\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  test_dbscan_plus.fut:14:23-19:30\n   #9  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        bool cond_76292 = sle64(block_sizze_76247, (int64_t) 1);
        int64_t tmp_76293 = sub64(block_sizze_76247, (int64_t) 2);
        bool bounds_invalid_upwards_76294 = slt64(tmp_76293, (int64_t) 0);
        bool valid_76296 = !bounds_invalid_upwards_76294;
        bool protect_assert_disj_76297 = cond_76292 || valid_76296;
        bool protect_assert_disj_76298 = cond_76230 || protect_assert_disj_76297;
        bool protect_assert_disj_76299 = cond_76216 || protect_assert_disj_76298;
        bool range_valid_c_76300;
        
        if (!protect_assert_disj_76299) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "...", (long long) tmp_76293, " is invalid.", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:166:19-28\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:243:35-248:37\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        bool empty_slice_76304 = conc_tmp_76160 == (int64_t) 0;
        bool zzero_leq_i_p_m_t_s_76305 = sle64((int64_t) 0, zs_lhs_76231);
        bool y_76307 = slt64(zs_lhs_76231, conc_tmp_76160);
        bool bounds_check_76308 = zzero_leq_i_p_m_t_s_76305 && y_76307;
        bool protect_assert_disj_76309 = empty_slice_76304 || bounds_check_76308;
        bool protect_assert_disj_76310 = cond_76216 || protect_assert_disj_76309;
        bool index_certs_76311;
        
        if (!protect_assert_disj_76310) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) zs_lhs_76231, "] out of bounds for array of shape [", (long long) conc_tmp_76160, "].", "-> #0  /prelude/soacs.fut:257:33-47\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t new_numFlushed_76842 = add64(num_flushed_73925, loopres_76219);
        bool eq_x_y_76845 = (int64_t) 0 == conc_tmp_74722;
        bool eq_x_zz_76846 = (int64_t) 0 == conc_tmp_76218;
        bool p_and_eq_x_y_76847 = cond_76216 && eq_x_y_76845;
        bool p_and_eq_x_y_76848 = x_76229 && eq_x_zz_76846;
        bool empty_slice_76849 = p_and_eq_x_y_76847 || p_and_eq_x_y_76848;
        int64_t m_76850 = sub64(loopres_76219, (int64_t) 1);
        int64_t i_p_m_t_s_76851 = add64(num_flushed_73925, m_76850);
        bool zzero_leq_i_p_m_t_s_76852 = sle64((int64_t) 0, i_p_m_t_s_76851);
        bool i_p_m_t_s_leq_w_76853 = slt64(i_p_m_t_s_76851, dz2081U_57317);
        bool zzero_lte_i_76854 = sle64((int64_t) 0, num_flushed_73925);
        bool i_lte_j_76855 = sle64(num_flushed_73925, new_numFlushed_76842);
        bool y_76856 = i_p_m_t_s_leq_w_76853 && zzero_lte_i_76854;
        bool y_76857 = zzero_leq_i_p_m_t_s_76852 && y_76856;
        bool forwards_ok_76858 = i_lte_j_76855 && y_76857;
        bool ok_or_empty_76859 = empty_slice_76849 || forwards_ok_76858;
        bool index_certs_76860;
        
        if (!ok_or_empty_76859) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) num_flushed_73925, ":", (long long) new_numFlushed_76842, "] out of bounds for array of shape [", (long long) dz2081U_57317, "].", "-> #0  ../../ftDBSCAN_plus.fut:1140:33-97\n   #1  test_dbscan_plus.fut:14:23-19:30\n   #2  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        bool eq_x_zz_76862 = conc_tmp_74722 == conc_tmp_76218;
        bool p_and_eq_x_y_76863 = x_76229 && eq_x_zz_76862;
        bool eq_x_y_76864 = cond_76216 || p_and_eq_x_y_76863;
        bool eq_x_y_76865 = conc_tmp_76218 == conc_tmp_74722;
        bool p_and_eq_x_y_76866 = cond_76216 && eq_x_y_76865;
        bool eq_x_zz_76867 = x_76229 || p_and_eq_x_y_76866;
        bool p_and_eq_x_y_76868 = cond_76216 && eq_x_y_76864;
        bool p_and_eq_x_y_76869 = x_76229 && eq_x_zz_76867;
        bool dim_match_76870 = p_and_eq_x_y_76868 || p_and_eq_x_y_76869;
        bool empty_or_match_cert_76871;
        
        if (!dim_match_76870) {
            set_error(ctx, msgprintf("Error: %s%s%lld%s%s%s%lld%s%s\n\nBacktrace:\n%s", "Shape ", "[", (long long) loopres_76219, "]", " of slice does not match shape ", "[", (long long) loopres_76219, "]", " of value.", "-> #0  ../../ftDBSCAN_plus.fut:1140:33-97\n   #1  test_dbscan_plus.fut:14:23-19:30\n   #2  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        if (mem_83125_cached_sizze_84852 < bytes_83124) {
            err = lexical_realloc(ctx, &mem_83125, &mem_83125_cached_sizze_84852, bytes_83124);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83127_cached_sizze_84853 < bytes_83124) {
            err = lexical_realloc(ctx, &mem_83127, &mem_83127_cached_sizze_84853, bytes_83124);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83129_cached_sizze_84854 < bytes_83124) {
            err = lexical_realloc(ctx, &mem_83129, &mem_83129_cached_sizze_84854, bytes_83124);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t i_81875 = 0; i_81875 < j_m_i_74649; i_81875++) {
            int64_t slice_82579 = partition_sizze_74623 + i_81875;
            int64_t eta_p_74700 = ((int64_t *) mem_83086)[slice_82579];
            bool x_74701 = sle64((int64_t) 0, eta_p_74700);
            bool y_74702 = slt64(eta_p_74700, dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74486);
            bool bounds_check_74703 = x_74701 && y_74702;
            bool index_certs_74704;
            
            if (!bounds_check_74703) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_74700, "] out of bounds for array of shape [", (long long) dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74486, "].", "-> #0  ../../ftDBSCAN_plus.fut:439:60-72\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../ftDBSCAN_plus.fut:439:44-88\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool y_74714 = slt64(eta_p_74700, conc_tmp_74488);
            bool bounds_check_74715 = x_74701 && y_74714;
            bool index_certs_74716;
            
            if (!bounds_check_74715) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_74700, "] out of bounds for array of shape [", (long long) conc_tmp_74488, "].", "-> #0  ../../ftDBSCAN_plus.fut:439:74-86\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../ftDBSCAN_plus.fut:439:44-88\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool index_concat_cmp_74705 = sle64(loop_dz2086Uz2082U_73921, eta_p_74700);
            double index_concat_branch_74706;
            
            if (index_concat_cmp_74705) {
                int64_t index_concat_i_81099 = sub64(eta_p_74700, loop_dz2086Uz2082U_73921);
                double index_concat_81100 = ((double *) mem_83031)[index_concat_i_81099];
                
                index_concat_branch_74706 = index_concat_81100;
            } else {
                double index_concat_74709 = ((double *) mem_param_82889.mem)[eta_p_74700];
                
                index_concat_branch_74706 = index_concat_74709;
            }
            
            double index_concat_branch_74710;
            
            if (index_concat_cmp_74705) {
                int64_t index_concat_i_81101 = sub64(eta_p_74700, loop_dz2086Uz2082U_73921);
                double index_concat_81102 = ((double *) mem_83033)[index_concat_i_81101];
                
                index_concat_branch_74710 = index_concat_81102;
            } else {
                double index_concat_74713 = ((double *) mem_param_82892.mem)[eta_p_74700];
                
                index_concat_branch_74710 = index_concat_74713;
            }
            
            bool index_concat_cmp_74717 = sle64(loop_dz2086Uz2081U_73920, eta_p_74700);
            int64_t index_concat_branch_74718;
            
            if (index_concat_cmp_74717) {
                int64_t index_concat_i_81103 = sub64(eta_p_74700, loop_dz2086Uz2081U_73920);
                int64_t index_concat_81104 = ((int64_t *) mem_83036)[index_concat_i_81103];
                
                index_concat_branch_74718 = index_concat_81104;
            } else {
                int64_t index_concat_74721 = ((int64_t *) mem_param_82886.mem)[eta_p_74700];
                
                index_concat_branch_74718 = index_concat_74721;
            }
            ((double *) mem_83125)[i_81875] = index_concat_branch_74706;
            ((double *) mem_83127)[i_81875] = index_concat_branch_74710;
            ((int64_t *) mem_83129)[i_81875] = index_concat_branch_74718;
        }
        if (memblock_alloc(ctx, &mem_83149, bytes_83093, "mem_83149")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83149.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83094, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {partition_sizze_74623});
        if (memblock_alloc(ctx, &mem_83151, bytes_83093, "mem_83151")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83151.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83096, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {partition_sizze_74623});
        if (mem_83869_cached_sizze_84934 < bytes_83868) {
            err = lexical_realloc(ctx, &mem_83869, &mem_83869_cached_sizze_84934, bytes_83868);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83869, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83837, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75821});
        if (mem_83871_cached_sizze_84935 < bytes_83868) {
            err = lexical_realloc(ctx, &mem_83871, &mem_83871_cached_sizze_84935, bytes_83868);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83871, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83839, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75821});
        for (int64_t write_iter_82243 = 0; write_iter_82243 < m_75731; write_iter_82243++) {
            int64_t write_iv_82246 = ((int64_t *) mem_83855)[write_iter_82243];
            int64_t write_iv_82247 = ((int64_t *) mem_83853)[write_iter_82243];
            int64_t write_iv_82248 = ((int64_t *) mem_83839)[write_iter_82243];
            int64_t write_iv_82249 = ((int64_t *) mem_83837)[write_iter_82243];
            bool cond_77387 = write_iv_82246 == (int64_t) 1;
            int64_t lifted_lambda_res_77388;
            
            if (cond_77387) {
                int64_t lifted_lambda_res_t_res_81185 = sub64(write_iv_82247, (int64_t) 1);
                
                lifted_lambda_res_77388 = lifted_lambda_res_t_res_81185;
            } else {
                lifted_lambda_res_77388 = (int64_t) -1;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77388) && slt64(lifted_lambda_res_77388, m_75821)) {
                ((int64_t *) mem_83871)[lifted_lambda_res_77388] = write_iv_82248;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77388) && slt64(lifted_lambda_res_77388, m_75821)) {
                ((int64_t *) mem_83869)[lifted_lambda_res_77388] = write_iv_82249;
            }
        }
        if (mem_83925_cached_sizze_84941 < bytes_83924) {
            err = lexical_realloc(ctx, &mem_83925, &mem_83925_cached_sizze_84941, bytes_83924);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83925, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83893, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75972});
        if (mem_83927_cached_sizze_84942 < bytes_83924) {
            err = lexical_realloc(ctx, &mem_83927, &mem_83927_cached_sizze_84942, bytes_83924);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83927, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83891, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75972});
        if (mem_83929_cached_sizze_84943 < bytes_83924) {
            err = lexical_realloc(ctx, &mem_83929, &mem_83929_cached_sizze_84943, bytes_83924);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83929, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83889, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75972});
        for (int64_t write_iter_82270 = 0; write_iter_82270 < conc_tmp_75256; write_iter_82270++) {
            int64_t write_iv_82274 = ((int64_t *) mem_83887)[write_iter_82270];
            int64_t write_iv_82275 = ((int64_t *) mem_83885)[write_iter_82270];
            int64_t write_iv_82276 = ((int64_t *) mem_83889)[write_iter_82270];
            double write_iv_82277 = ((double *) mem_83891)[write_iter_82270];
            double write_iv_82278 = ((double *) mem_83893)[write_iter_82270];
            bool cond_77177 = write_iv_82274 == (int64_t) 1;
            int64_t lifted_lambda_res_77178;
            
            if (cond_77177) {
                int64_t lifted_lambda_res_t_res_81203 = sub64(write_iv_82275, (int64_t) 1);
                
                lifted_lambda_res_77178 = lifted_lambda_res_t_res_81203;
            } else {
                lifted_lambda_res_77178 = (int64_t) -1;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77178) && slt64(lifted_lambda_res_77178, m_75972)) {
                ((int64_t *) mem_83929)[lifted_lambda_res_77178] = write_iv_82276;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77178) && slt64(lifted_lambda_res_77178, m_75972)) {
                ((double *) mem_83927)[lifted_lambda_res_77178] = write_iv_82277;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77178) && slt64(lifted_lambda_res_77178, m_75972)) {
                ((double *) mem_83925)[lifted_lambda_res_77178] = write_iv_82278;
            }
        }
        if (mem_83949_cached_sizze_84944 < bytes_83223) {
            err = lexical_realloc(ctx, &mem_83949, &mem_83949_cached_sizze_84944, bytes_83223);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t nest_i_84738 = 0; nest_i_84738 < m_74888; nest_i_84738++) {
            ((int64_t *) mem_83949)[nest_i_84738] = (int64_t) -1;
        }
        for (int64_t i_76021 = 0; i_76021 < max_res_76015; i_76021++) {
            int64_t inf_76023 = mul64(max_res_76008, i_76021);
            int64_t min_arg1_76024 = add64(max_res_76008, inf_76023);
            int64_t min_res_76025 = smin64(m_74888, min_arg1_76024);
            int64_t dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_76026 = sub64(min_res_76025, inf_76023);
            bool empty_slice_76027 = dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_76026 == (int64_t) 0;
            int64_t m_76028 = sub64(dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_76026, (int64_t) 1);
            int64_t i_p_m_t_s_76029 = add64(inf_76023, m_76028);
            bool zzero_leq_i_p_m_t_s_76030 = sle64((int64_t) 0, i_p_m_t_s_76029);
            bool i_p_m_t_s_leq_w_76031 = slt64(i_p_m_t_s_76029, m_74888);
            bool zzero_lte_i_76032 = sle64((int64_t) 0, inf_76023);
            bool i_lte_j_76033 = sle64(inf_76023, min_res_76025);
            bool y_76034 = i_p_m_t_s_leq_w_76031 && zzero_lte_i_76032;
            bool y_76035 = zzero_leq_i_p_m_t_s_76030 && y_76034;
            bool forwards_ok_76036 = i_lte_j_76033 && y_76035;
            bool ok_or_empty_76037 = empty_slice_76027 || forwards_ok_76036;
            bool index_certs_76038;
            
            if (!ok_or_empty_76037) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) inf_76023, ":", (long long) min_res_76025, "] out of bounds for array of shape [", (long long) m_74888, "].", "-> #0  ../../ftDBSCAN_plus.fut:739:48-69\n   #1  test_dbscan_plus.fut:14:23-19:30\n   #2  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            for (int64_t i_82287 = 0; i_82287 < dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_76026; i_82287++) {
                int64_t slice_82432 = inf_76023 + i_82287;
                bool index_concat_cmp_82434 = sle64(partition_sizze_75035, slice_82432);
                double index_concat_branch_82438;
                
                if (index_concat_cmp_82434) {
                    double index_concat_82436 = ((double *) mem_83287)[slice_82432];
                    
                    index_concat_branch_82438 = index_concat_82436;
                } else {
                    double index_concat_82437 = ((double *) mem_83287)[slice_82432];
                    
                    index_concat_branch_82438 = index_concat_82437;
                }
                
                double index_concat_branch_82431;
                
                if (index_concat_cmp_82434) {
                    double index_concat_82429 = ((double *) mem_83289)[slice_82432];
                    
                    index_concat_branch_82431 = index_concat_82429;
                } else {
                    double index_concat_82430 = ((double *) mem_83289)[slice_82432];
                    
                    index_concat_branch_82431 = index_concat_82430;
                }
                
                int64_t defunc_0_f_res_81208;
                double defunc_0_f_res_81209;
                int64_t redout_82282;
                double redout_82283;
                
                redout_82282 = (int64_t) -1;
                redout_82283 = INFINITY;
                for (int64_t i_82284 = 0; i_82284 < m_75972; i_82284++) {
                    double eta_p_79358 = ((double *) mem_83927)[i_82284];
                    double eta_p_79359 = ((double *) mem_83925)[i_82284];
                    int64_t x_79360 = ((int64_t *) mem_83929)[i_82284];
                    bool zeze_res_79361 = index_concat_branch_82438 == eta_p_79358;
                    double d_euclidean_res_79362;
                    
                    if (zeze_res_79361) {
                        double zm_res_81204 = index_concat_branch_82431 - eta_p_79359;
                        double abs_res_81205 = fabs64(zm_res_81204);
                        
                        d_euclidean_res_79362 = abs_res_81205;
                    } else {
                        bool zeze_res_79365 = index_concat_branch_82431 == eta_p_79359;
                        double d_euclidean_res_f_res_79366;
                        
                        if (zeze_res_79365) {
                            double zm_res_81206 = index_concat_branch_82438 - eta_p_79358;
                            double abs_res_81207 = fabs64(zm_res_81206);
                            
                            d_euclidean_res_f_res_79366 = abs_res_81207;
                        } else {
                            double zm_res_79369 = index_concat_branch_82431 - eta_p_79359;
                            double zm_res_79370 = index_concat_branch_82438 - eta_p_79358;
                            double hypot_res_79371 = futrts_hypot64(zm_res_79370, zm_res_79369);
                            
                            d_euclidean_res_f_res_79366 = hypot_res_79371;
                        }
                        d_euclidean_res_79362 = d_euclidean_res_f_res_79366;
                    }
                    
                    bool zg_res_76066 = 2.0 < redout_82283;
                    bool zg_res_76067 = 2.0 < d_euclidean_res_79362;
                    bool x_76068 = zg_res_76066 && zg_res_76067;
                    int64_t lifted_lambda_res_76069;
                    double lifted_lambda_res_76070;
                    
                    if (x_76068) {
                        lifted_lambda_res_76069 = (int64_t) -1;
                        lifted_lambda_res_76070 = INFINITY;
                    } else {
                        bool zl_res_76071 = redout_82283 < d_euclidean_res_79362;
                        int64_t lifted_lambda_res_f_res_76072;
                        
                        if (zl_res_76071) {
                            lifted_lambda_res_f_res_76072 = redout_82282;
                        } else {
                            lifted_lambda_res_f_res_76072 = x_79360;
                        }
                        
                        double lifted_lambda_res_f_res_76073;
                        
                        if (zl_res_76071) {
                            lifted_lambda_res_f_res_76073 = redout_82283;
                        } else {
                            lifted_lambda_res_f_res_76073 = d_euclidean_res_79362;
                        }
                        lifted_lambda_res_76069 = lifted_lambda_res_f_res_76072;
                        lifted_lambda_res_76070 = lifted_lambda_res_f_res_76073;
                    }
                    
                    int64_t redout_tmp_84741 = lifted_lambda_res_76069;
                    double redout_tmp_84742 = lifted_lambda_res_76070;
                    
                    redout_82282 = redout_tmp_84741;
                    redout_82283 = redout_tmp_84742;
                }
                defunc_0_f_res_81208 = redout_82282;
                defunc_0_f_res_81209 = redout_82283;
                ((int64_t *) mem_83949)[inf_76023 + i_82287] = defunc_0_f_res_81208;
            }
        }
        if (memblock_alloc(ctx, &mem_83965, bytes_82918, "mem_83965")) {
            err = 1;
            goto cleanup;
        }
        for (int64_t nest_i_84743 = 0; nest_i_84743 < loopres_73979; nest_i_84743++) {
            ((int64_t *) mem_83965.mem)[nest_i_84743] = (int64_t) -1;
        }
        for (int64_t i_76087 = 0; i_76087 < max_res_76081; i_76087++) {
            int64_t inf_76089 = mul64(max_res_75267, i_76087);
            int64_t min_arg1_76090 = add64(max_res_75267, inf_76089);
            int64_t min_res_76091 = smin64(loopres_73979, min_arg1_76090);
            int64_t dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_76092 = sub64(min_res_76091, inf_76089);
            bool empty_slice_76093 = dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_76092 == (int64_t) 0;
            int64_t m_76094 = sub64(dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_76092, (int64_t) 1);
            int64_t i_p_m_t_s_76095 = add64(inf_76089, m_76094);
            bool zzero_leq_i_p_m_t_s_76096 = sle64((int64_t) 0, i_p_m_t_s_76095);
            bool i_p_m_t_s_leq_w_76097 = slt64(i_p_m_t_s_76095, loopres_73979);
            bool zzero_lte_i_76098 = sle64((int64_t) 0, inf_76089);
            bool i_lte_j_76099 = sle64(inf_76089, min_res_76091);
            bool y_76100 = i_p_m_t_s_leq_w_76097 && zzero_lte_i_76098;
            bool y_76101 = zzero_leq_i_p_m_t_s_76096 && y_76100;
            bool forwards_ok_76102 = i_lte_j_76099 && y_76101;
            bool ok_or_empty_76103 = empty_slice_76093 || forwards_ok_76102;
            bool index_certs_76104;
            
            if (!ok_or_empty_76103) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) inf_76089, ":", (long long) min_res_76091, "] out of bounds for array of shape [", (long long) loopres_73979, "].", "-> #0  ../../ftDBSCAN_plus.fut:756:48-70\n   #1  test_dbscan_plus.fut:14:23-19:30\n   #2  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            for (int64_t i_82294 = 0; i_82294 < dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_76092; i_82294++) {
                int64_t slice_82424 = inf_76089 + i_82294;
                double eta_p_76108 = ((double *) ext_mem_82921.mem)[slice_82424];
                double eta_p_76109 = ((double *) ext_mem_82920.mem)[slice_82424];
                int64_t defunc_0_f_res_81217;
                double defunc_0_f_res_81218;
                int64_t redout_82289;
                double redout_82290;
                
                redout_82289 = (int64_t) -1;
                redout_82290 = INFINITY;
                for (int64_t i_82291 = 0; i_82291 < conc_tmp_75256; i_82291++) {
                    double eta_p_79393 = ((double *) mem_83891)[i_82291];
                    double eta_p_79394 = ((double *) mem_83893)[i_82291];
                    int64_t x_79395 = ((int64_t *) mem_83889)[i_82291];
                    bool zeze_res_79396 = eta_p_76108 == eta_p_79393;
                    double d_euclidean_res_79397;
                    
                    if (zeze_res_79396) {
                        double zm_res_81213 = eta_p_76109 - eta_p_79394;
                        double abs_res_81214 = fabs64(zm_res_81213);
                        
                        d_euclidean_res_79397 = abs_res_81214;
                    } else {
                        bool zeze_res_79400 = eta_p_76109 == eta_p_79394;
                        double d_euclidean_res_f_res_79401;
                        
                        if (zeze_res_79400) {
                            double zm_res_81215 = eta_p_76108 - eta_p_79393;
                            double abs_res_81216 = fabs64(zm_res_81215);
                            
                            d_euclidean_res_f_res_79401 = abs_res_81216;
                        } else {
                            double zm_res_79404 = eta_p_76109 - eta_p_79394;
                            double zm_res_79405 = eta_p_76108 - eta_p_79393;
                            double hypot_res_79406 = futrts_hypot64(zm_res_79405, zm_res_79404);
                            
                            d_euclidean_res_f_res_79401 = hypot_res_79406;
                        }
                        d_euclidean_res_79397 = d_euclidean_res_f_res_79401;
                    }
                    
                    bool zg_res_76132 = 2.0 < redout_82290;
                    bool zg_res_76133 = 2.0 < d_euclidean_res_79397;
                    bool x_76134 = zg_res_76132 && zg_res_76133;
                    int64_t lifted_lambda_res_76135;
                    double lifted_lambda_res_76136;
                    
                    if (x_76134) {
                        lifted_lambda_res_76135 = (int64_t) -1;
                        lifted_lambda_res_76136 = INFINITY;
                    } else {
                        bool zl_res_76137 = redout_82290 < d_euclidean_res_79397;
                        int64_t lifted_lambda_res_f_res_76138;
                        
                        if (zl_res_76137) {
                            lifted_lambda_res_f_res_76138 = redout_82289;
                        } else {
                            lifted_lambda_res_f_res_76138 = x_79395;
                        }
                        
                        double lifted_lambda_res_f_res_76139;
                        
                        if (zl_res_76137) {
                            lifted_lambda_res_f_res_76139 = redout_82290;
                        } else {
                            lifted_lambda_res_f_res_76139 = d_euclidean_res_79397;
                        }
                        lifted_lambda_res_76135 = lifted_lambda_res_f_res_76138;
                        lifted_lambda_res_76136 = lifted_lambda_res_f_res_76139;
                    }
                    
                    int64_t redout_tmp_84746 = lifted_lambda_res_76135;
                    double redout_tmp_84747 = lifted_lambda_res_76136;
                    
                    redout_82289 = redout_tmp_84746;
                    redout_82290 = redout_tmp_84747;
                }
                defunc_0_f_res_81217 = redout_82289;
                defunc_0_f_res_81218 = redout_82290;
                ((int64_t *) mem_83965.mem)[inf_76089 + i_82294] = defunc_0_f_res_81217;
            }
        }
        if (memblock_unref(ctx, &ext_mem_82920, "ext_mem_82920") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_82921, "ext_mem_82921") != 0)
            return 1;
        for (int64_t write_iter_82296 = 0; write_iter_82296 < m_74888; write_iter_82296++) {
            bool index_concat_cmp_82418 = sle64(partition_sizze_75035, write_iter_82296);
            int64_t index_concat_branch_82422;
            
            if (index_concat_cmp_82418) {
                int64_t index_concat_82420 = ((int64_t *) mem_83294)[write_iter_82296];
                
                index_concat_branch_82422 = index_concat_82420;
            } else {
                int64_t index_concat_82421 = ((int64_t *) mem_83294)[write_iter_82296];
                
                index_concat_branch_82422 = index_concat_82421;
            }
            
            int64_t write_iv_82299 = ((int64_t *) mem_83949)[write_iter_82296];
            
            if (sle64((int64_t) 0, index_concat_branch_82422) && slt64(index_concat_branch_82422, partition_sizze_74623)) {
                ((int64_t *) mem_83099.mem)[index_concat_branch_82422] = write_iv_82299;
            }
        }
        for (int64_t write_iter_82301 = 0; write_iter_82301 < partition_sizze_75035; write_iter_82301++) {
            int64_t write_iv_82303 = ((int64_t *) mem_83294)[write_iter_82301];
            bool write_iv_82304 = ((bool *) mem_83368)[write_iter_82301];
            
            if (sle64((int64_t) 0, write_iv_82303) && slt64(write_iv_82303, partition_sizze_74623)) {
                ((bool *) mem_83097.mem)[write_iv_82303] = write_iv_82304;
            }
        }
        
        int64_t defunc_0_reduce_res_81654;
        int64_t redout_82306 = (int64_t) -9223372036854775808;
        
        for (int64_t i_82307 = 0; i_82307 < loopres_73979; i_82307++) {
            int64_t x_76148 = ((int64_t *) mem_83965.mem)[i_82307];
            int64_t max_res_76151 = smax64(x_76148, redout_82306);
            int64_t redout_tmp_84750 = max_res_76151;
            
            redout_82306 = redout_tmp_84750;
        }
        defunc_0_reduce_res_81654 = redout_82306;
        
        int64_t defunc_0_reduce_res_81655;
        int64_t redout_82308 = (int64_t) -9223372036854775808;
        
        for (int64_t i_82309 = 0; i_82309 < m_74888; i_82309++) {
            int64_t x_76153 = ((int64_t *) mem_83949)[i_82309];
            int64_t max_res_76156 = smax64(x_76153, redout_82308);
            int64_t redout_tmp_84751 = max_res_76156;
            
            redout_82308 = redout_tmp_84751;
        }
        defunc_0_reduce_res_81655 = redout_82308;
        
        int64_t max_res_76157 = smax64(defunc_0_reduce_res_81654, defunc_0_reduce_res_81655);
        int64_t max_arg1_76158 = add64((int64_t) 1, max_res_76157);
        int64_t max_res_76159 = smax64(old_clHandler_73938, max_arg1_76158);
        
        if (memblock_alloc(ctx, &mem_83993, bytes_83992, "mem_83993")) {
            err = 1;
            goto cleanup;
        }
        
        int64_t tmp_offs_84752 = (int64_t) 0;
        
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83993.mem, tmp_offs_84752, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_82877.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {loop_dz2085Uz2085U_73918});
        tmp_offs_84752 += loop_dz2085Uz2085U_73918;
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83993.mem, tmp_offs_84752, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83871, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75821});
        tmp_offs_84752 += m_75821;
        if (memblock_alloc(ctx, &mem_83995, bytes_83992, "mem_83995")) {
            err = 1;
            goto cleanup;
        }
        
        int64_t tmp_offs_84753 = (int64_t) 0;
        
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83995.mem, tmp_offs_84753, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_82880.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {loop_dz2085Uz2085U_73918});
        tmp_offs_84753 += loop_dz2085Uz2085U_73918;
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83995.mem, tmp_offs_84753, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83869, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75821});
        tmp_offs_84753 += m_75821;
        if (memblock_alloc(ctx, &mem_84019, bytes_84018, "mem_84019")) {
            err = 1;
            goto cleanup;
        }
        
        int64_t tmp_offs_84754 = (int64_t) 0;
        
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_84019.mem, tmp_offs_84754, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83031, (int64_t) 0 + (int64_t) 1 * partition_sizze_74438, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_74472});
        tmp_offs_84754 += j_m_i_74472;
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_84019.mem, tmp_offs_84754, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83125, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_74649});
        tmp_offs_84754 += j_m_i_74649;
        if (memblock_alloc(ctx, &mem_84021, bytes_84018, "mem_84021")) {
            err = 1;
            goto cleanup;
        }
        
        int64_t tmp_offs_84755 = (int64_t) 0;
        
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_84021.mem, tmp_offs_84755, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83033, (int64_t) 0 + (int64_t) 1 * partition_sizze_74438, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_74472});
        tmp_offs_84755 += j_m_i_74472;
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_84021.mem, tmp_offs_84755, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83127, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_74649});
        tmp_offs_84755 += j_m_i_74649;
        if (memblock_alloc(ctx, &mem_84023, bytes_84018, "mem_84023")) {
            err = 1;
            goto cleanup;
        }
        
        int64_t tmp_offs_84756 = (int64_t) 0;
        
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_84023.mem, tmp_offs_84756, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83036, (int64_t) 0 + (int64_t) 1 * partition_sizze_74438, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_74472});
        tmp_offs_84756 += j_m_i_74472;
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_84023.mem, tmp_offs_84756, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83129, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_74649});
        tmp_offs_84756 += j_m_i_74649;
        if (cond_76216) {
            if (memblock_set(ctx, &ext_mem_84032, &mem_84019, "mem_84019") != 0)
                return 1;
            if (memblock_set(ctx, &ext_mem_84031, &mem_84021, "mem_84021") != 0)
                return 1;
            if (memblock_set(ctx, &ext_mem_84030, &mem_84023, "mem_84023") != 0)
                return 1;
        } else {
            if (memblock_alloc(ctx, &mem_84025, bytes_84018, "mem_84025")) {
                err = 1;
                goto cleanup;
            }
            
            int64_t tmp_offs_84757 = (int64_t) 0;
            
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_84025.mem, tmp_offs_84757, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83031, (int64_t) 0 + (int64_t) 1 * partition_sizze_74438, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_74472});
            tmp_offs_84757 += j_m_i_74472;
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_84025.mem, tmp_offs_84757, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83125, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_74649});
            tmp_offs_84757 += j_m_i_74649;
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_84025.mem, tmp_offs_84757, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83149.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {partition_sizze_74623});
            tmp_offs_84757 += partition_sizze_74623;
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_84025.mem, tmp_offs_84757, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82925.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {loopres_73979});
            tmp_offs_84757 += loopres_73979;
            if (memblock_alloc(ctx, &mem_84027, bytes_84018, "mem_84027")) {
                err = 1;
                goto cleanup;
            }
            
            int64_t tmp_offs_84758 = (int64_t) 0;
            
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_84027.mem, tmp_offs_84758, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83033, (int64_t) 0 + (int64_t) 1 * partition_sizze_74438, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_74472});
            tmp_offs_84758 += j_m_i_74472;
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_84027.mem, tmp_offs_84758, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83127, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_74649});
            tmp_offs_84758 += j_m_i_74649;
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_84027.mem, tmp_offs_84758, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83151.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {partition_sizze_74623});
            tmp_offs_84758 += partition_sizze_74623;
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_84027.mem, tmp_offs_84758, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82927.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {loopres_73979});
            tmp_offs_84758 += loopres_73979;
            if (memblock_alloc(ctx, &mem_84029, bytes_84018, "mem_84029")) {
                err = 1;
                goto cleanup;
            }
            
            int64_t tmp_offs_84759 = (int64_t) 0;
            
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_84029.mem, tmp_offs_84759, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83036, (int64_t) 0 + (int64_t) 1 * partition_sizze_74438, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_74472});
            tmp_offs_84759 += j_m_i_74472;
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_84029.mem, tmp_offs_84759, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83129, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_74649});
            tmp_offs_84759 += j_m_i_74649;
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_84029.mem, tmp_offs_84759, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83099.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {partition_sizze_74623});
            tmp_offs_84759 += partition_sizze_74623;
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_84029.mem, tmp_offs_84759, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83965.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {loopres_73979});
            tmp_offs_84759 += loopres_73979;
            if (memblock_set(ctx, &ext_mem_84032, &mem_84025, "mem_84025") != 0)
                return 1;
            if (memblock_set(ctx, &ext_mem_84031, &mem_84027, "mem_84027") != 0)
                return 1;
            if (memblock_set(ctx, &ext_mem_84030, &mem_84029, "mem_84029") != 0)
                return 1;
        }
        if (memblock_unref(ctx, &mem_84019, "mem_84019") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84021, "mem_84021") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84023, "mem_84023") != 0)
            return 1;
        
        int64_t flat_dim_76248 = x_76236 * block_sizze_76247;
        bool x_76249 = !cond_76230;
        int64_t greatest_divisor_leq_than_arg1_76250 = mul64((int64_t) 2, block_sizze_76247);
        bool protect_cond_conj_76251 = x_76229 && x_76249;
        int64_t merge_block_sizze_76252;
        
        if (protect_cond_conj_76251) {
            int64_t x_81332;
            
            if (futrts_greatest_divisor_leq_than_5592(ctx, &x_81332, (int64_t) 8, greatest_divisor_leq_than_arg1_76250) != 0) {
                err = 1;
                goto cleanup;
            }
            merge_block_sizze_76252 = x_81332;
        } else {
            merge_block_sizze_76252 = (int64_t) 0;
        }
        
        bool index_concat_cmp_76258 = sle64(loop_dz2085Uz2085U_73918, (int64_t) 0);
        int64_t index_concat_branch_76259;
        
        if (protect_cond_conj_76251) {
            int64_t x_81335;
            
            if (index_concat_cmp_76258) {
                int64_t index_concat_i_81336 = sub64((int64_t) 0, loop_dz2085Uz2085U_73918);
                int64_t index_concat_81337 = ((int64_t *) mem_83871)[index_concat_i_81336];
                
                x_81335 = index_concat_81337;
            } else {
                int64_t index_concat_81338 = ((int64_t *) mem_param_82877.mem)[(int64_t) 0];
                
                x_81335 = index_concat_81338;
            }
            index_concat_branch_76259 = x_81335;
        } else {
            index_concat_branch_76259 = (int64_t) 0;
        }
        
        int64_t index_concat_branch_76264;
        
        if (protect_cond_conj_76251) {
            int64_t x_81341;
            
            if (index_concat_cmp_76258) {
                int64_t index_concat_i_81342 = sub64((int64_t) 0, loop_dz2085Uz2085U_73918);
                int64_t index_concat_81343 = ((int64_t *) mem_83869)[index_concat_i_81342];
                
                x_81341 = index_concat_81343;
            } else {
                int64_t index_concat_81344 = ((int64_t *) mem_param_82880.mem)[(int64_t) 0];
                
                x_81341 = index_concat_81344;
            }
            index_concat_branch_76264 = x_81341;
        } else {
            index_concat_branch_76264 = (int64_t) 0;
        }
        
        int64_t defunc_0_reduce_res_76269;
        int64_t defunc_0_reduce_res_76270;
        
        if (protect_cond_conj_76251) {
            int64_t x_81347;
            int64_t x_81348;
            int64_t redout_82327;
            int64_t redout_82328;
            
            redout_82327 = index_concat_branch_76259;
            redout_82328 = index_concat_branch_76264;
            for (int64_t i_82329 = 0; i_82329 < conc_tmp_76160; i_82329++) {
                bool index_concat_cmp_82410 = sle64(loop_dz2085Uz2085U_73918, i_82329);
                int64_t index_concat_branch_82414;
                
                if (index_concat_cmp_82410) {
                    int64_t index_concat_i_82411 = sub64(i_82329, loop_dz2085Uz2085U_73918);
                    int64_t index_concat_82412 = ((int64_t *) mem_83871)[index_concat_i_82411];
                    
                    index_concat_branch_82414 = index_concat_82412;
                } else {
                    int64_t index_concat_82413 = ((int64_t *) mem_param_82877.mem)[i_82329];
                    
                    index_concat_branch_82414 = index_concat_82413;
                }
                
                int64_t index_concat_branch_82408;
                
                if (index_concat_cmp_82410) {
                    int64_t index_concat_i_82405 = sub64(i_82329, loop_dz2085Uz2085U_73918);
                    int64_t index_concat_82406 = ((int64_t *) mem_83869)[index_concat_i_82405];
                    
                    index_concat_branch_82408 = index_concat_82406;
                } else {
                    int64_t index_concat_82407 = ((int64_t *) mem_param_82880.mem)[i_82329];
                    
                    index_concat_branch_82408 = index_concat_82407;
                }
                
                bool defunc_0_leq_res_81355;
                
                if (futrts_lifted_lambda_12071(ctx, &defunc_0_leq_res_81355, redout_82327, redout_82328, index_concat_branch_82414, index_concat_branch_82408) != 0) {
                    err = 1;
                    goto cleanup;
                }
                
                int64_t defunc_0_op_res_81356;
                
                if (defunc_0_leq_res_81355) {
                    defunc_0_op_res_81356 = index_concat_branch_82414;
                } else {
                    defunc_0_op_res_81356 = redout_82327;
                }
                
                int64_t defunc_0_op_res_81357;
                
                if (defunc_0_leq_res_81355) {
                    defunc_0_op_res_81357 = index_concat_branch_82408;
                } else {
                    defunc_0_op_res_81357 = redout_82328;
                }
                
                int64_t redout_tmp_84760 = defunc_0_op_res_81356;
                int64_t redout_tmp_84761 = defunc_0_op_res_81357;
                
                redout_82327 = redout_tmp_84760;
                redout_82328 = redout_tmp_84761;
            }
            x_81347 = redout_82327;
            x_81348 = redout_82328;
            defunc_0_reduce_res_76269 = x_81347;
            defunc_0_reduce_res_76270 = x_81348;
        } else {
            defunc_0_reduce_res_76269 = (int64_t) 0;
            defunc_0_reduce_res_76270 = (int64_t) 0;
        }
        
        int64_t distance_76295 = add64((int64_t) 1, tmp_76293);
        bool loop_cond_76301 = slt64(block_sizze_76247, flat_dim_76248);
        bool zzero_76302 = merge_block_sizze_76252 == (int64_t) 0;
        bool nonzzero_76303 = !zzero_76302;
        bool x_76306 = !empty_slice_76304;
        int64_t binop_x_84033 = (int64_t) 8 * x_76236;
        int64_t bytes_84034 = block_sizze_76247 * binop_x_84033;
        int64_t bytes_84047 = (int64_t) 8 * block_sizze_76247;
        int64_t loopres_76312;
        int64_t loopres_76313;
        int64_t loopres_76315;
        
        if (cond_76216) {
            if (memblock_set(ctx, &ext_mem_84278, &mem_param_82871, "mem_param_82871") != 0)
                return 1;
            if (memblock_set(ctx, &ext_mem_84275, &mem_param_82874, "mem_param_82874") != 0)
                return 1;
            loopres_76312 = loop_dz2085Uz2083U_73916;
            loopres_76313 = loop_dz2085Uz2084U_73917;
            loopres_76315 = collisions_73930;
        } else {
            if (cond_76230) {
                if (memblock_set(ctx, &ext_mem_84240, &mem_83993, "mem_83993") != 0)
                    return 1;
                if (memblock_set(ctx, &ext_mem_84237, &mem_83995, "mem_83995") != 0)
                    return 1;
            } else {
                if (mem_84035_cached_sizze_84948 < bytes_84034) {
                    err = lexical_realloc(ctx, &mem_84035, &mem_84035_cached_sizze_84948, bytes_84034);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                if (mem_84038_cached_sizze_84949 < bytes_84034) {
                    err = lexical_realloc(ctx, &mem_84038, &mem_84038_cached_sizze_84949, bytes_84034);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                if (mem_84048_cached_sizze_84950 < bytes_84047) {
                    err = lexical_realloc(ctx, &mem_84048, &mem_84048_cached_sizze_84950, bytes_84047);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                if (mem_84050_cached_sizze_84951 < bytes_84047) {
                    err = lexical_realloc(ctx, &mem_84050, &mem_84050_cached_sizze_84951, bytes_84047);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                for (int64_t i_82341 = 0; i_82341 < x_76236; i_82341++) {
                    int64_t zp_lhs_76324 = mul64(block_sizze_76247, i_82341);
                    
                    for (int64_t i_82334 = 0; i_82334 < block_sizze_76247; i_82334++) {
                        int64_t k_76328 = add64(zp_lhs_76324, i_82334);
                        bool cond_76329 = slt64(k_76328, conc_tmp_76160);
                        int64_t lifted_lambda_res_76330;
                        int64_t lifted_lambda_res_76331;
                        
                        if (cond_76329) {
                            bool x_81368 = sle64((int64_t) 0, k_76328);
                            bool bounds_check_81369 = cond_76329 && x_81368;
                            bool index_certs_81370;
                            
                            if (!bounds_check_81369) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) k_76328, "] out of bounds for array of shape [", (long long) conc_tmp_76160, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:246:26-30\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:244:40-246:44\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:243:35-248:37\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  test_dbscan_plus.fut:14:23-19:30\n   #10 test_dbscan_plus.fut:13:1-20:55\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            bool index_concat_cmp_81371 = sle64(loop_dz2085Uz2085U_73918, k_76328);
                            int64_t index_concat_branch_81372;
                            
                            if (index_concat_cmp_81371) {
                                int64_t index_concat_i_81373 = sub64(k_76328, loop_dz2085Uz2085U_73918);
                                int64_t index_concat_81374 = ((int64_t *) mem_83871)[index_concat_i_81373];
                                
                                index_concat_branch_81372 = index_concat_81374;
                            } else {
                                int64_t index_concat_81375 = ((int64_t *) mem_param_82877.mem)[k_76328];
                                
                                index_concat_branch_81372 = index_concat_81375;
                            }
                            
                            int64_t index_concat_branch_81376;
                            
                            if (index_concat_cmp_81371) {
                                int64_t index_concat_i_81377 = sub64(k_76328, loop_dz2085Uz2085U_73918);
                                int64_t index_concat_81378 = ((int64_t *) mem_83869)[index_concat_i_81377];
                                
                                index_concat_branch_81376 = index_concat_81378;
                            } else {
                                int64_t index_concat_81379 = ((int64_t *) mem_param_82880.mem)[k_76328];
                                
                                index_concat_branch_81376 = index_concat_81379;
                            }
                            lifted_lambda_res_76330 = index_concat_branch_81372;
                            lifted_lambda_res_76331 = index_concat_branch_81376;
                        } else {
                            lifted_lambda_res_76330 = defunc_0_reduce_res_76269;
                            lifted_lambda_res_76331 = defunc_0_reduce_res_76270;
                        }
                        ((int64_t *) mem_84048)[i_82334] = lifted_lambda_res_76330;
                        ((int64_t *) mem_84050)[i_82334] = lifted_lambda_res_76331;
                    }
                    if (!cond_76292) {
                        for (int64_t i_76348 = 0; i_76348 < distance_76295; i_76348++) {
                            int64_t gt_arg1_76351 = add64((int64_t) 1, i_76348);
                            bool x_76352 = sle64((int64_t) 0, gt_arg1_76351);
                            bool y_76353 = slt64(gt_arg1_76351, block_sizze_76247);
                            bool bounds_check_76354 = x_76352 && y_76353;
                            bool index_certs_76355;
                            
                            if (!bounds_check_76354) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) gt_arg1_76351, "] out of bounds for array of shape [", (long long) block_sizze_76247, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:169:31-37\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:243:35-248:37\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            bool y_76358 = slt64(i_76348, block_sizze_76247);
                            bool index_certs_76359;
                            
                            if (!y_76358) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_76348, "] out of bounds for array of shape [", (long long) block_sizze_76247, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:169:26-30\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:243:35-248:37\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t gt_arg1_76356 = ((int64_t *) mem_84048)[gt_arg1_76351];
                            int64_t gt_arg1_76357 = ((int64_t *) mem_84050)[gt_arg1_76351];
                            int64_t gt_arg0_76360 = ((int64_t *) mem_84048)[i_76348];
                            int64_t gt_arg0_76361 = ((int64_t *) mem_84050)[i_76348];
                            bool defunc_0_leq_res_76362;
                            
                            if (futrts_lifted_lambda_12071(ctx, &defunc_0_leq_res_76362, gt_arg0_76360, gt_arg0_76361, gt_arg1_76356, gt_arg1_76357) != 0) {
                                err = 1;
                                goto cleanup;
                            }
                            
                            bool defunc_0_lifted_gt_res_76363 = !defunc_0_leq_res_76362;
                            bool s_76364;
                            int64_t s_76367;
                            bool loop_while_76368;
                            int64_t j_76371;
                            
                            loop_while_76368 = defunc_0_lifted_gt_res_76363;
                            j_76371 = i_76348;
                            while (loop_while_76368) {
                                bool x_76372 = sle64((int64_t) 0, j_76371);
                                bool y_76373 = slt64(j_76371, block_sizze_76247);
                                bool bounds_check_76374 = x_76372 && y_76373;
                                bool index_certs_76375;
                                
                                if (!bounds_check_76374) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_76371, "] out of bounds for array of shape [", (long long) block_sizze_76247, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:170:24-28\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:243:35-248:37\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t copy_arg0_76378 = add64((int64_t) 1, j_76371);
                                bool x_76379 = sle64((int64_t) 0, copy_arg0_76378);
                                bool y_76380 = slt64(copy_arg0_76378, block_sizze_76247);
                                bool bounds_check_76381 = x_76379 && y_76380;
                                bool index_certs_76382;
                                
                                if (!bounds_check_76381) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) copy_arg0_76378, "] out of bounds for array of shape [", (long long) block_sizze_76247, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:171:35-41\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:243:35-248:37\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t copy_arg0_76376 = ((int64_t *) mem_84048)[j_76371];
                                int64_t copy_arg0_76377 = ((int64_t *) mem_84050)[j_76371];
                                int64_t copy_arg0_76383 = ((int64_t *) mem_84048)[copy_arg0_76378];
                                int64_t copy_arg0_76384 = ((int64_t *) mem_84050)[copy_arg0_76378];
                                
                                ((int64_t *) mem_84048)[j_76371] = copy_arg0_76383;
                                ((int64_t *) mem_84050)[j_76371] = copy_arg0_76384;
                                ((int64_t *) mem_84048)[copy_arg0_76378] = copy_arg0_76376;
                                ((int64_t *) mem_84050)[copy_arg0_76378] = copy_arg0_76377;
                                
                                int64_t tmp_76389 = sub64(j_76371, (int64_t) 1);
                                bool cond_76390 = sle64((int64_t) 0, tmp_76389);
                                bool loop_cond_76391;
                                
                                if (cond_76390) {
                                    bool y_81382 = slt64(tmp_76389, block_sizze_76247);
                                    bool bounds_check_81383 = cond_76390 && y_81382;
                                    bool index_certs_81384;
                                    
                                    if (!bounds_check_81383) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_76389, "] out of bounds for array of shape [", (long long) block_sizze_76247, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:169:26-30\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:243:35-248:37\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    int64_t gt_arg1_81380 = ((int64_t *) mem_84048)[j_76371];
                                    int64_t gt_arg1_81381 = ((int64_t *) mem_84050)[j_76371];
                                    int64_t gt_arg0_81385 = ((int64_t *) mem_84048)[tmp_76389];
                                    int64_t gt_arg0_81386 = ((int64_t *) mem_84050)[tmp_76389];
                                    bool defunc_0_leq_res_81387;
                                    
                                    if (futrts_lifted_lambda_12071(ctx, &defunc_0_leq_res_81387, gt_arg0_81385, gt_arg0_81386, gt_arg1_81380, gt_arg1_81381) != 0) {
                                        err = 1;
                                        goto cleanup;
                                    }
                                    
                                    bool defunc_0_lifted_gt_res_81388 = !defunc_0_leq_res_81387;
                                    
                                    loop_cond_76391 = defunc_0_lifted_gt_res_81388;
                                } else {
                                    loop_cond_76391 = 0;
                                }
                                
                                bool loop_while_tmp_84768 = loop_cond_76391;
                                int64_t j_tmp_84771 = tmp_76389;
                                
                                loop_while_76368 = loop_while_tmp_84768;
                                j_76371 = j_tmp_84771;
                            }
                            s_76364 = loop_while_76368;
                            s_76367 = j_76371;
                        }
                    }
                    lmad_copy_8b(ctx, 1, (uint64_t *) mem_84035, i_82341 * block_sizze_76247, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_84048, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {block_sizze_76247});
                    lmad_copy_8b(ctx, 1, (uint64_t *) mem_84038, i_82341 * block_sizze_76247, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_84050, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {block_sizze_76247});
                }
                if (memblock_alloc(ctx, &mem_84103, bytes_84034, "mem_84103")) {
                    err = 1;
                    goto cleanup;
                }
                lmad_copy_8b(ctx, 2, (uint64_t *) mem_84103.mem, (int64_t) 0, (int64_t []) {block_sizze_76247, (int64_t) 1}, (uint64_t *) mem_84035, (int64_t) 0, (int64_t []) {block_sizze_76247, (int64_t) 1}, (int64_t []) {x_76236, block_sizze_76247});
                if (memblock_alloc(ctx, &mem_84107, bytes_84034, "mem_84107")) {
                    err = 1;
                    goto cleanup;
                }
                lmad_copy_8b(ctx, 2, (uint64_t *) mem_84107.mem, (int64_t) 0, (int64_t []) {block_sizze_76247, (int64_t) 1}, (uint64_t *) mem_84038, (int64_t) 0, (int64_t []) {block_sizze_76247, (int64_t) 1}, (int64_t []) {x_76236, block_sizze_76247});
                
                int64_t data_76403;
                bool data_76404;
                int64_t data_76407;
                int64_t loop_dz2084Uz2082U_76408;
                bool loop_while_76409;
                int64_t stride_76412;
                
                if (memblock_set(ctx, &mem_param_84111, &mem_84103, "mem_84103") != 0)
                    return 1;
                if (memblock_set(ctx, &mem_param_84114, &mem_84107, "mem_84107") != 0)
                    return 1;
                loop_dz2084Uz2082U_76408 = flat_dim_76248;
                loop_while_76409 = loop_cond_76301;
                stride_76412 = block_sizze_76247;
                while (loop_while_76409) {
                    int64_t next_stride_76413 = mul64((int64_t) 2, stride_76412);
                    bool zzero_76414 = next_stride_76413 == (int64_t) 0;
                    bool nonzzero_76415 = !zzero_76414;
                    bool nonzzero_cert_76416;
                    
                    if (!nonzzero_76415) {
                        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:254:33-46\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #2  /prelude/functional.fut:9:44-45\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t num_merges_76417 = sdiv64(flat_dim_76248, next_stride_76413);
                    bool bounds_invalid_upwards_76418 = slt64(num_merges_76417, (int64_t) 0);
                    bool valid_76419 = !bounds_invalid_upwards_76418;
                    bool range_valid_c_76420;
                    
                    if (!valid_76419) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) num_merges_76417, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #4  /prelude/functional.fut:9:44-45\n   #5  test_dbscan_plus.fut:14:23-19:30\n   #6  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t m_76422 = sub64(next_stride_76413, (int64_t) 1);
                    bool zzero_leq_i_p_m_t_s_76423 = sle64((int64_t) 0, m_76422);
                    bool cond_76425 = slt64(next_stride_76413, (int64_t) 10);
                    bool protect_assert_disj_76426 = nonzzero_76303 || cond_76425;
                    bool nonzzero_cert_76427;
                    
                    if (!protect_assert_disj_76426) {
                        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:147:30-42\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t zeze_lhs_76428 = smod_safe64(next_stride_76413, merge_block_sizze_76252);
                    bool num_blocks_76429 = zeze_lhs_76428 == (int64_t) 0;
                    bool protect_assert_disj_76430 = cond_76425 || num_blocks_76429;
                    bool assert_c_76431;
                    
                    if (!protect_assert_disj_76430) {
                        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "Assertion is false: (n % block_size == 0)", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:147:20-65\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t num_blocks_76433 = sdiv_safe64(next_stride_76413, merge_block_sizze_76252);
                    int64_t dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_76434 = add64((int64_t) 1, num_blocks_76433);
                    int64_t j_m_i_76435 = sub64(next_stride_76413, stride_76412);
                    int64_t flat_dim_76436 = merge_block_sizze_76252 * num_blocks_76433;
                    bool bounds_invalid_upwards_76437 = slt64(dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_76434, (int64_t) 0);
                    bool zzero_lte_i_76438 = sle64((int64_t) 0, stride_76412);
                    bool i_lte_j_76439 = sle64(stride_76412, next_stride_76413);
                    bool empty_slice_76440 = stride_76412 == (int64_t) 0;
                    int64_t m_76441 = sub64(stride_76412, (int64_t) 1);
                    bool bounds_invalid_upwards_76442 = slt64(num_blocks_76433, (int64_t) 0);
                    bool empty_slice_76443 = j_m_i_76435 == (int64_t) 0;
                    int64_t m_76444 = sub64(j_m_i_76435, (int64_t) 1);
                    bool cond_76445 = slt64((int64_t) 0, stride_76412);
                    bool valid_76446 = !bounds_invalid_upwards_76437;
                    bool zzero_leq_i_p_m_t_s_76447 = sle64((int64_t) 0, m_76441);
                    bool i_p_m_t_s_leq_w_76448 = slt64(m_76441, next_stride_76413);
                    bool valid_76449 = !bounds_invalid_upwards_76442;
                    bool i_p_m_t_s_leq_w_76450 = slt64(m_76422, flat_dim_76436);
                    int64_t i_p_m_t_s_76451 = add64(stride_76412, m_76444);
                    bool y_76452 = slt64((int64_t) 0, j_m_i_76435);
                    bool protect_assert_disj_76453 = cond_76425 || valid_76446;
                    bool range_valid_c_76454;
                    
                    if (!protect_assert_disj_76453) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_76434, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  test_dbscan_plus.fut:14:23-19:30\n   #10 test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    bool y_76455 = zzero_leq_i_p_m_t_s_76447 && i_p_m_t_s_leq_w_76448;
                    bool protect_assert_disj_76456 = cond_76425 || valid_76449;
                    bool range_valid_c_76457;
                    
                    if (!protect_assert_disj_76456) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) num_blocks_76433, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  test_dbscan_plus.fut:14:23-19:30\n   #9  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    bool y_76458 = zzero_leq_i_p_m_t_s_76423 && i_p_m_t_s_leq_w_76450;
                    bool zzero_leq_i_p_m_t_s_76459 = sle64((int64_t) 0, i_p_m_t_s_76451);
                    bool i_p_m_t_s_leq_w_76460 = slt64(i_p_m_t_s_76451, next_stride_76413);
                    bool forwards_ok_76461 = zzero_lte_i_76438 && y_76455;
                    bool ok_or_empty_76462 = zzero_76414 || y_76458;
                    bool y_76463 = zzero_lte_i_76438 && i_p_m_t_s_leq_w_76460;
                    bool protect_assert_disj_76464 = cond_76445 || y_76452;
                    bool ok_or_empty_76465 = empty_slice_76440 || forwards_ok_76461;
                    bool protect_assert_disj_76466 = cond_76425 || ok_or_empty_76462;
                    bool index_certs_76467;
                    
                    if (!protect_assert_disj_76466) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) next_stride_76413, "] out of bounds for array of shape [", (long long) flat_dim_76436, "].", "-> #0  /prelude/array.fut:46:45-51\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:155:3-46\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  test_dbscan_plus.fut:14:23-19:30\n   #9  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    bool y_76468 = zzero_leq_i_p_m_t_s_76459 && y_76463;
                    bool loop_not_taken_76469 = !cond_76425;
                    bool protect_assert_disj_76470 = protect_assert_disj_76464 || loop_not_taken_76469;
                    bool index_certs_76471;
                    
                    if (!protect_assert_disj_76470) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) j_m_i_76435, "].", "-> #0  /prelude/array.fut:22:29-33\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:100:48-54\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  test_dbscan_plus.fut:14:23-19:30\n   #10 test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    bool protect_assert_disj_76472 = cond_76425 || ok_or_empty_76465;
                    bool index_certs_76473;
                    
                    if (!protect_assert_disj_76472) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) stride_76412, "] out of bounds for array of shape [", (long long) next_stride_76413, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:52-59\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #8  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #9  /prelude/functional.fut:9:44-45\n   #10 test_dbscan_plus.fut:14:23-19:30\n   #11 test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    bool forwards_ok_76474 = i_lte_j_76439 && y_76468;
                    bool protect_assert_disj_76475 = ok_or_empty_76465 || loop_not_taken_76469;
                    bool index_certs_76476;
                    
                    if (!protect_assert_disj_76475) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) stride_76412, "] out of bounds for array of shape [", (long long) next_stride_76413, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:26-33\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    bool ok_or_empty_76477 = empty_slice_76443 || forwards_ok_76474;
                    bool protect_assert_disj_76478 = loop_not_taken_76469 || ok_or_empty_76477;
                    bool index_certs_76479;
                    
                    if (!protect_assert_disj_76478) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) stride_76412, ":] out of bounds for array of shape [", (long long) next_stride_76413, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:34-41\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    bool protect_assert_disj_76480 = cond_76425 || ok_or_empty_76477;
                    bool index_certs_76481;
                    
                    if (!protect_assert_disj_76480) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) stride_76412, ":] out of bounds for array of shape [", (long long) next_stride_76413, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:60-67\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #8  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #9  /prelude/functional.fut:9:44-45\n   #10 test_dbscan_plus.fut:14:23-19:30\n   #11 test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t binop_x_84115 = (int64_t) 8 * num_merges_76417;
                    int64_t bytes_84116 = next_stride_76413 * binop_x_84115;
                    bool i_lte_j_76424 = sle64((int64_t) 0, next_stride_76413);
                    
                    if (mem_84117_cached_sizze_84952 < bytes_84116) {
                        err = lexical_realloc(ctx, &mem_84117, &mem_84117_cached_sizze_84952, bytes_84116);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    if (mem_84120_cached_sizze_84953 < bytes_84116) {
                        err = lexical_realloc(ctx, &mem_84120, &mem_84120_cached_sizze_84953, bytes_84116);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    
                    int64_t bytes_84129 = (int64_t) 8 * dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_76434;
                    int64_t binop_x_84145 = (int64_t) 8 * num_blocks_76433;
                    int64_t bytes_84146 = merge_block_sizze_76252 * binop_x_84145;
                    int64_t bytes_84191 = (int64_t) 16 * stride_76412;
                    
                    for (int64_t i_82362 = 0; i_82362 < num_merges_76417; i_82362++) {
                        int64_t start_76485 = mul64(next_stride_76413, i_82362);
                        int64_t j_m_i_76486 = sub64(loop_dz2084Uz2082U_76408, start_76485);
                        bool empty_slice_76487 = j_m_i_76486 == (int64_t) 0;
                        int64_t m_76488 = sub64(j_m_i_76486, (int64_t) 1);
                        int64_t i_p_m_t_s_76489 = add64(start_76485, m_76488);
                        bool zzero_leq_i_p_m_t_s_76490 = sle64((int64_t) 0, i_p_m_t_s_76489);
                        bool i_p_m_t_s_leq_w_76491 = slt64(i_p_m_t_s_76489, loop_dz2084Uz2082U_76408);
                        bool zzero_lte_i_76492 = sle64((int64_t) 0, start_76485);
                        bool i_lte_j_76493 = sle64(start_76485, loop_dz2084Uz2082U_76408);
                        bool y_76494 = i_p_m_t_s_leq_w_76491 && zzero_lte_i_76492;
                        bool y_76495 = zzero_leq_i_p_m_t_s_76490 && y_76494;
                        bool forwards_ok_76496 = i_lte_j_76493 && y_76495;
                        bool ok_or_empty_76497 = empty_slice_76487 || forwards_ok_76496;
                        bool index_certs_76498;
                        
                        if (!ok_or_empty_76497) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) start_76485, ":] out of bounds for array of shape [", (long long) loop_dz2084Uz2082U_76408, "].", "-> #0  /prelude/array.fut:52:49-54\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:258:41-56\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        bool i_p_m_t_s_leq_w_76499 = slt64(m_76422, j_m_i_76486);
                        bool y_76500 = zzero_leq_i_p_m_t_s_76423 && i_p_m_t_s_leq_w_76499;
                        bool forwards_ok_76501 = i_lte_j_76424 && y_76500;
                        bool ok_or_empty_76502 = zzero_76414 || forwards_ok_76501;
                        bool index_certs_76503;
                        
                        if (!ok_or_empty_76502) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) next_stride_76413, "] out of bounds for array of shape [", (long long) j_m_i_76486, "].", "-> #0  /prelude/array.fut:46:45-51\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:258:23-56\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        if (cond_76425) {
                            int64_t dummy_81416;
                            int64_t dummy_81417;
                            
                            if (cond_76445) {
                                int64_t head_res_81418 = ((int64_t *) mem_param_84111.mem)[start_76485];
                                int64_t head_res_81419 = ((int64_t *) mem_param_84114.mem)[start_76485];
                                
                                dummy_81416 = head_res_81418;
                                dummy_81417 = head_res_81419;
                            } else {
                                int64_t slice_81420 = stride_76412 + start_76485;
                                int64_t head_res_81421 = ((int64_t *) mem_param_84111.mem)[slice_81420];
                                int64_t head_res_81422 = ((int64_t *) mem_param_84114.mem)[slice_81420];
                                
                                dummy_81416 = head_res_81421;
                                dummy_81417 = head_res_81422;
                            }
                            if (memblock_alloc(ctx, &mem_84192, bytes_84191, "mem_84192")) {
                                err = 1;
                                goto cleanup;
                            }
                            for (int64_t nest_i_84781 = 0; nest_i_84781 < next_stride_76413; nest_i_84781++) {
                                ((int64_t *) mem_84192.mem)[nest_i_84781] = dummy_81416;
                            }
                            if (memblock_alloc(ctx, &mem_84194, bytes_84191, "mem_84194")) {
                                err = 1;
                                goto cleanup;
                            }
                            for (int64_t nest_i_84782 = 0; nest_i_84782 < next_stride_76413; nest_i_84782++) {
                                ((int64_t *) mem_84194.mem)[nest_i_84782] = dummy_81417;
                            }
                            
                            int64_t data_81425;
                            int64_t i_81429 = (int64_t) 0;
                            
                            for (int64_t k_81428 = 0; k_81428 < next_stride_76413; k_81428++) {
                                int64_t j_81432 = sub64(k_81428, i_81429);
                                bool cond_81433 = j_81432 == j_m_i_76435;
                                bool cond_81434;
                                
                                if (cond_81433) {
                                    cond_81434 = 1;
                                } else {
                                    bool cond_81435 = slt64(i_81429, stride_76412);
                                    bool cond_f_res_81436;
                                    
                                    if (cond_81435) {
                                        bool x_81437 = sle64((int64_t) 0, j_81432);
                                        bool y_81438 = slt64(j_81432, j_m_i_76435);
                                        bool bounds_check_81439 = x_81437 && y_81438;
                                        bool index_certs_81440;
                                        
                                        if (!bounds_check_81439) {
                                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_81432, "] out of bounds for array of shape [", (long long) j_m_i_76435, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:55-59\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  test_dbscan_plus.fut:14:23-19:30\n   #9  test_dbscan_plus.fut:13:1-20:55\n"));
                                            err = FUTHARK_PROGRAM_ERROR;
                                            goto cleanup;
                                        }
                                        
                                        bool x_81445 = sle64((int64_t) 0, i_81429);
                                        bool bounds_check_81446 = cond_81435 && x_81445;
                                        bool index_certs_81447;
                                        
                                        if (!bounds_check_81446) {
                                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_81429, "] out of bounds for array of shape [", (long long) stride_76412, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:50-54\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  test_dbscan_plus.fut:14:23-19:30\n   #9  test_dbscan_plus.fut:13:1-20:55\n"));
                                            err = FUTHARK_PROGRAM_ERROR;
                                            goto cleanup;
                                        }
                                        
                                        int64_t slice_81441 = stride_76412 + j_81432;
                                        int64_t slice_81442 = start_76485 + slice_81441;
                                        int64_t leq_arg1_81443 = ((int64_t *) mem_param_84111.mem)[slice_81442];
                                        int64_t leq_arg1_81444 = ((int64_t *) mem_param_84114.mem)[slice_81442];
                                        int64_t slice_81448 = start_76485 + i_81429;
                                        int64_t leq_arg0_81449 = ((int64_t *) mem_param_84111.mem)[slice_81448];
                                        int64_t leq_arg0_81450 = ((int64_t *) mem_param_84114.mem)[slice_81448];
                                        bool defunc_0_leq_res_81451;
                                        
                                        if (futrts_lifted_lambda_12071(ctx, &defunc_0_leq_res_81451, leq_arg0_81449, leq_arg0_81450, leq_arg1_81443, leq_arg1_81444) != 0) {
                                            err = 1;
                                            goto cleanup;
                                        }
                                        cond_f_res_81436 = defunc_0_leq_res_81451;
                                    } else {
                                        cond_f_res_81436 = 0;
                                    }
                                    cond_81434 = cond_f_res_81436;
                                }
                                
                                int64_t loopres_81452;
                                int64_t loopres_81453;
                                int64_t loopres_81454;
                                
                                if (cond_81434) {
                                    bool x_81456 = sle64((int64_t) 0, i_81429);
                                    bool y_81457 = slt64(i_81429, stride_76412);
                                    bool bounds_check_81458 = x_81456 && y_81457;
                                    bool index_certs_81459;
                                    
                                    if (!bounds_check_81458) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_81429, "] out of bounds for array of shape [", (long long) stride_76412, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:106:17-21\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  test_dbscan_plus.fut:14:23-19:30\n   #9  test_dbscan_plus.fut:13:1-20:55\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    int64_t tmp_81455 = add64((int64_t) 1, i_81429);
                                    int64_t slice_81460 = start_76485 + i_81429;
                                    int64_t tmp_81461 = ((int64_t *) mem_param_84111.mem)[slice_81460];
                                    int64_t tmp_81462 = ((int64_t *) mem_param_84114.mem)[slice_81460];
                                    
                                    loopres_81452 = tmp_81455;
                                    loopres_81453 = tmp_81461;
                                    loopres_81454 = tmp_81462;
                                } else {
                                    bool x_81463 = sle64((int64_t) 0, j_81432);
                                    bool y_81464 = slt64(j_81432, j_m_i_76435);
                                    bool bounds_check_81465 = x_81463 && y_81464;
                                    bool index_certs_81466;
                                    
                                    if (!bounds_check_81465) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_81432, "] out of bounds for array of shape [", (long long) j_m_i_76435, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:108:15-19\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  test_dbscan_plus.fut:14:23-19:30\n   #9  test_dbscan_plus.fut:13:1-20:55\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    int64_t slice_81467 = stride_76412 + j_81432;
                                    int64_t slice_81468 = start_76485 + slice_81467;
                                    int64_t tmp_81469 = ((int64_t *) mem_param_84111.mem)[slice_81468];
                                    int64_t tmp_81470 = ((int64_t *) mem_param_84114.mem)[slice_81468];
                                    
                                    loopres_81452 = i_81429;
                                    loopres_81453 = tmp_81469;
                                    loopres_81454 = tmp_81470;
                                }
                                ((int64_t *) mem_84192.mem)[k_81428] = loopres_81453;
                                ((int64_t *) mem_84194.mem)[k_81428] = loopres_81454;
                                
                                int64_t i_tmp_84783 = loopres_81452;
                                
                                i_81429 = i_tmp_84783;
                            }
                            data_81425 = i_81429;
                            if (memblock_set(ctx, &ext_mem_84212, &mem_84192, "mem_84192") != 0)
                                return 1;
                            if (memblock_set(ctx, &ext_mem_84209, &mem_84194, "mem_84194") != 0)
                                return 1;
                        } else {
                            if (mem_84130_cached_sizze_84954 < bytes_84129) {
                                err = lexical_realloc(ctx, &mem_84130, &mem_84130_cached_sizze_84954, bytes_84129);
                                if (err != FUTHARK_SUCCESS)
                                    goto cleanup;
                            }
                            if (mem_84132_cached_sizze_84955 < bytes_84129) {
                                err = lexical_realloc(ctx, &mem_84132, &mem_84132_cached_sizze_84955, bytes_84129);
                                if (err != FUTHARK_SUCCESS)
                                    goto cleanup;
                            }
                            for (int64_t i_82348 = 0; i_82348 < dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_76434; i_82348++) {
                                int64_t split_count_arg3_76567 = mul64(merge_block_sizze_76252, i_82348);
                                int64_t defunc_0_split_count_res_76568;
                                int64_t defunc_0_split_count_res_76569;
                                int64_t defunc_0_split_count_res_76570;
                                int64_t defunc_0_split_count_res_76571;
                                int64_t defunc_0_split_count_res_76572;
                                
                                if (futrts_lifted_normalizze_11972(ctx, &defunc_0_split_count_res_76568, &defunc_0_split_count_res_76569, &defunc_0_split_count_res_76570, &defunc_0_split_count_res_76571, &defunc_0_split_count_res_76572, (int64_t) 0, stride_76412, (int64_t) 0, j_m_i_76435, split_count_arg3_76567) != 0) {
                                    err = 1;
                                    goto cleanup;
                                }
                                
                                bool loop_cond_76573 = slt64((int64_t) 0, defunc_0_split_count_res_76572);
                                bool defunc_0_split_count_res_76574;
                                int64_t defunc_0_split_count_res_76575;
                                int64_t defunc_0_split_count_res_76576;
                                int64_t defunc_0_split_count_res_76577;
                                int64_t defunc_0_split_count_res_76578;
                                int64_t defunc_0_split_count_res_76579;
                                bool loop_while_76580;
                                int64_t ss_76581;
                                int64_t ss_76582;
                                int64_t tt_76583;
                                int64_t tt_76584;
                                int64_t count_76585;
                                
                                loop_while_76580 = loop_cond_76573;
                                ss_76581 = defunc_0_split_count_res_76568;
                                ss_76582 = defunc_0_split_count_res_76569;
                                tt_76583 = defunc_0_split_count_res_76570;
                                tt_76584 = defunc_0_split_count_res_76571;
                                count_76585 = defunc_0_split_count_res_76572;
                                while (loop_while_76580) {
                                    int64_t zlze_lhs_76586 = sub64(ss_76582, ss_76581);
                                    bool cond_76587 = sle64(zlze_lhs_76586, (int64_t) 0);
                                    int64_t defunc_0_lifted_step_res_76588;
                                    int64_t defunc_0_lifted_step_res_76589;
                                    int64_t defunc_0_lifted_step_res_76590;
                                    int64_t defunc_0_lifted_step_res_76591;
                                    
                                    if (cond_76587) {
                                        int64_t tmp_81473 = add64(tt_76583, count_76585);
                                        
                                        defunc_0_lifted_step_res_76588 = ss_76581;
                                        defunc_0_lifted_step_res_76589 = tmp_81473;
                                        defunc_0_lifted_step_res_76590 = tt_76584;
                                        defunc_0_lifted_step_res_76591 = (int64_t) 0;
                                    } else {
                                        int64_t zlze_lhs_76593 = sub64(tt_76584, tt_76583);
                                        bool cond_76594 = sle64(zlze_lhs_76593, (int64_t) 0);
                                        int64_t defunc_0_lifted_step_res_f_res_76595;
                                        int64_t defunc_0_lifted_step_res_f_res_76596;
                                        int64_t defunc_0_lifted_step_res_f_res_76597;
                                        int64_t defunc_0_lifted_step_res_f_res_76598;
                                        
                                        if (cond_76594) {
                                            int64_t tmp_81474 = add64(ss_76581, count_76585);
                                            
                                            defunc_0_lifted_step_res_f_res_76595 = tmp_81474;
                                            defunc_0_lifted_step_res_f_res_76596 = tt_76583;
                                            defunc_0_lifted_step_res_f_res_76597 = tt_76584;
                                            defunc_0_lifted_step_res_f_res_76598 = (int64_t) 0;
                                        } else {
                                            bool cond_76600 = count_76585 == (int64_t) 1;
                                            int64_t defunc_0_lifted_step_res_f_res_f_res_76601;
                                            int64_t defunc_0_lifted_step_res_f_res_f_res_76602;
                                            int64_t defunc_0_lifted_step_res_f_res_f_res_76603;
                                            int64_t defunc_0_lifted_step_res_f_res_f_res_76604;
                                            
                                            if (cond_76600) {
                                                bool x_81476 = sle64((int64_t) 0, ss_76581);
                                                bool y_81477 = slt64(ss_76581, stride_76412);
                                                bool bounds_check_81478 = x_81476 && y_81477;
                                                bool index_certs_81479;
                                                
                                                if (!bounds_check_81478) {
                                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) ss_76581, "] out of bounds for array of shape [", (long long) stride_76412, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:53:21-27\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #10 ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #11 /prelude/functional.fut:9:44-45\n   #12 test_dbscan_plus.fut:14:23-19:30\n   #13 test_dbscan_plus.fut:13:1-20:55\n"));
                                                    err = FUTHARK_PROGRAM_ERROR;
                                                    goto cleanup;
                                                }
                                                
                                                bool x_81483 = sle64((int64_t) 0, tt_76583);
                                                bool y_81484 = slt64(tt_76583, j_m_i_76435);
                                                bool bounds_check_81485 = x_81483 && y_81484;
                                                bool index_certs_81486;
                                                
                                                if (!bounds_check_81485) {
                                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tt_76583, "] out of bounds for array of shape [", (long long) j_m_i_76435, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:53:14-20\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #10 ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #11 /prelude/functional.fut:9:44-45\n   #12 test_dbscan_plus.fut:14:23-19:30\n   #13 test_dbscan_plus.fut:13:1-20:55\n"));
                                                    err = FUTHARK_PROGRAM_ERROR;
                                                    goto cleanup;
                                                }
                                                
                                                int64_t slice_81480 = start_76485 + ss_76581;
                                                int64_t leq_arg1_81481 = ((int64_t *) mem_param_84111.mem)[slice_81480];
                                                int64_t leq_arg1_81482 = ((int64_t *) mem_param_84114.mem)[slice_81480];
                                                int64_t slice_81487 = stride_76412 + tt_76583;
                                                int64_t slice_81488 = start_76485 + slice_81487;
                                                int64_t leq_arg0_81489 = ((int64_t *) mem_param_84111.mem)[slice_81488];
                                                int64_t leq_arg0_81490 = ((int64_t *) mem_param_84114.mem)[slice_81488];
                                                bool defunc_0_leq_res_81491;
                                                
                                                if (futrts_lifted_lambda_12071(ctx, &defunc_0_leq_res_81491, leq_arg0_81489, leq_arg0_81490, leq_arg1_81481, leq_arg1_81482) != 0) {
                                                    err = 1;
                                                    goto cleanup;
                                                }
                                                
                                                int64_t defunc_0_lifted_step_res_f_res_f_res_t_res_81492;
                                                int64_t defunc_0_lifted_step_res_f_res_f_res_t_res_81493;
                                                
                                                if (defunc_0_leq_res_81491) {
                                                    int64_t tmp_81494 = add64((int64_t) 1, tt_76583);
                                                    
                                                    defunc_0_lifted_step_res_f_res_f_res_t_res_81492 = ss_76581;
                                                    defunc_0_lifted_step_res_f_res_f_res_t_res_81493 = tmp_81494;
                                                } else {
                                                    int64_t tmp_81495 = add64((int64_t) 1, ss_76581);
                                                    
                                                    defunc_0_lifted_step_res_f_res_f_res_t_res_81492 = tmp_81495;
                                                    defunc_0_lifted_step_res_f_res_f_res_t_res_81493 = tt_76583;
                                                }
                                                defunc_0_lifted_step_res_f_res_f_res_76601 = defunc_0_lifted_step_res_f_res_f_res_t_res_81492;
                                                defunc_0_lifted_step_res_f_res_f_res_76602 = defunc_0_lifted_step_res_f_res_f_res_t_res_81493;
                                                defunc_0_lifted_step_res_f_res_f_res_76603 = tt_76584;
                                                defunc_0_lifted_step_res_f_res_f_res_76604 = (int64_t) 0;
                                            } else {
                                                int64_t m_76625 = sdiv64(count_76585, (int64_t) 2);
                                                int64_t n_76626 = sub64(count_76585, m_76625);
                                                bool cond_76627 = n_76626 == (int64_t) 0;
                                                int64_t zgze_lhs_76628 = add64(ss_76581, m_76625);
                                                bool cond_f_res_76629 = sle64(ss_76582, zgze_lhs_76628);
                                                bool x_76630 = !cond_76627;
                                                bool y_76631 = cond_f_res_76629 && x_76630;
                                                bool cond_76632 = cond_76627 || y_76631;
                                                bool leq_y_x_76633;
                                                
                                                if (cond_76632) {
                                                    leq_y_x_76633 = 1;
                                                } else {
                                                    bool x_76634 = sle64((int64_t) 0, zgze_lhs_76628);
                                                    bool y_76635 = slt64(zgze_lhs_76628, stride_76412);
                                                    bool bounds_check_76636 = x_76634 && y_76635;
                                                    bool index_certs_76637;
                                                    
                                                    if (!bounds_check_76636) {
                                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) zgze_lhs_76628, "] out of bounds for array of shape [", (long long) stride_76412, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:73:22-30\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #10 ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #11 /prelude/functional.fut:9:44-45\n   #12 test_dbscan_plus.fut:14:23-19:30\n   #13 test_dbscan_plus.fut:13:1-20:55\n"));
                                                        err = FUTHARK_PROGRAM_ERROR;
                                                        goto cleanup;
                                                    }
                                                    
                                                    int64_t zm_lhs_76641 = add64(tt_76583, n_76626);
                                                    int64_t leq_arg0_76642 = sub64(zm_lhs_76641, (int64_t) 1);
                                                    bool x_76643 = sle64((int64_t) 0, leq_arg0_76642);
                                                    bool y_76644 = slt64(leq_arg0_76642, j_m_i_76435);
                                                    bool bounds_check_76645 = x_76643 && y_76644;
                                                    bool index_certs_76646;
                                                    
                                                    if (!bounds_check_76645) {
                                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) leq_arg0_76642, "] out of bounds for array of shape [", (long long) j_m_i_76435, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:73:11-21\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #10 ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #11 /prelude/functional.fut:9:44-45\n   #12 test_dbscan_plus.fut:14:23-19:30\n   #13 test_dbscan_plus.fut:13:1-20:55\n"));
                                                        err = FUTHARK_PROGRAM_ERROR;
                                                        goto cleanup;
                                                    }
                                                    
                                                    int64_t slice_76638 = start_76485 + zgze_lhs_76628;
                                                    int64_t leq_arg1_76639 = ((int64_t *) mem_param_84111.mem)[slice_76638];
                                                    int64_t leq_arg1_76640 = ((int64_t *) mem_param_84114.mem)[slice_76638];
                                                    int64_t slice_76647 = stride_76412 + leq_arg0_76642;
                                                    int64_t slice_76648 = start_76485 + slice_76647;
                                                    int64_t leq_arg0_76649 = ((int64_t *) mem_param_84111.mem)[slice_76648];
                                                    int64_t leq_arg0_76650 = ((int64_t *) mem_param_84114.mem)[slice_76648];
                                                    bool defunc_0_leq_res_76651;
                                                    
                                                    if (futrts_lifted_lambda_12071(ctx, &defunc_0_leq_res_76651, leq_arg0_76649, leq_arg0_76650, leq_arg1_76639, leq_arg1_76640) != 0) {
                                                        err = 1;
                                                        goto cleanup;
                                                    }
                                                    leq_y_x_76633 = defunc_0_leq_res_76651;
                                                }
                                                
                                                int64_t defunc_0_lifted_step_res_f_res_f_res_f_res_76652;
                                                int64_t defunc_0_lifted_step_res_f_res_f_res_f_res_76653;
                                                int64_t defunc_0_lifted_step_res_f_res_f_res_f_res_76654;
                                                
                                                if (leq_y_x_76633) {
                                                    int64_t tmp_81496 = add64(tt_76583, n_76626);
                                                    int64_t tmp_81497 = sub64(count_76585, n_76626);
                                                    
                                                    defunc_0_lifted_step_res_f_res_f_res_f_res_76652 = tmp_81496;
                                                    defunc_0_lifted_step_res_f_res_f_res_f_res_76653 = tt_76584;
                                                    defunc_0_lifted_step_res_f_res_f_res_f_res_76654 = tmp_81497;
                                                } else {
                                                    int64_t tmp_76657 = add64(tt_76583, n_76626);
                                                    
                                                    defunc_0_lifted_step_res_f_res_f_res_f_res_76652 = tt_76583;
                                                    defunc_0_lifted_step_res_f_res_f_res_f_res_76653 = tmp_76657;
                                                    defunc_0_lifted_step_res_f_res_f_res_f_res_76654 = count_76585;
                                                }
                                                defunc_0_lifted_step_res_f_res_f_res_76601 = ss_76581;
                                                defunc_0_lifted_step_res_f_res_f_res_76602 = defunc_0_lifted_step_res_f_res_f_res_f_res_76652;
                                                defunc_0_lifted_step_res_f_res_f_res_76603 = defunc_0_lifted_step_res_f_res_f_res_f_res_76653;
                                                defunc_0_lifted_step_res_f_res_f_res_76604 = defunc_0_lifted_step_res_f_res_f_res_f_res_76654;
                                            }
                                            defunc_0_lifted_step_res_f_res_76595 = defunc_0_lifted_step_res_f_res_f_res_76601;
                                            defunc_0_lifted_step_res_f_res_76596 = defunc_0_lifted_step_res_f_res_f_res_76602;
                                            defunc_0_lifted_step_res_f_res_76597 = defunc_0_lifted_step_res_f_res_f_res_76603;
                                            defunc_0_lifted_step_res_f_res_76598 = defunc_0_lifted_step_res_f_res_f_res_76604;
                                        }
                                        defunc_0_lifted_step_res_76588 = defunc_0_lifted_step_res_f_res_76595;
                                        defunc_0_lifted_step_res_76589 = defunc_0_lifted_step_res_f_res_76596;
                                        defunc_0_lifted_step_res_76590 = defunc_0_lifted_step_res_f_res_76597;
                                        defunc_0_lifted_step_res_76591 = defunc_0_lifted_step_res_f_res_76598;
                                    }
                                    
                                    int64_t loopres_76658;
                                    int64_t loopres_76659;
                                    int64_t loopres_76660;
                                    int64_t loopres_76661;
                                    int64_t loopres_76662;
                                    
                                    if (futrts_lifted_normalizze_11972(ctx, &loopres_76658, &loopres_76659, &loopres_76660, &loopres_76661, &loopres_76662, defunc_0_lifted_step_res_76588, ss_76582, defunc_0_lifted_step_res_76589, defunc_0_lifted_step_res_76590, defunc_0_lifted_step_res_76591) != 0) {
                                        err = 1;
                                        goto cleanup;
                                    }
                                    
                                    bool loop_cond_76663 = slt64((int64_t) 0, loopres_76662);
                                    bool loop_while_tmp_84788 = loop_cond_76663;
                                    int64_t ss_tmp_84789 = loopres_76658;
                                    int64_t ss_tmp_84790 = loopres_76659;
                                    int64_t tt_tmp_84791 = loopres_76660;
                                    int64_t tt_tmp_84792 = loopres_76661;
                                    int64_t count_tmp_84793 = loopres_76662;
                                    
                                    loop_while_76580 = loop_while_tmp_84788;
                                    ss_76581 = ss_tmp_84789;
                                    ss_76582 = ss_tmp_84790;
                                    tt_76583 = tt_tmp_84791;
                                    tt_76584 = tt_tmp_84792;
                                    count_76585 = count_tmp_84793;
                                }
                                defunc_0_split_count_res_76574 = loop_while_76580;
                                defunc_0_split_count_res_76575 = ss_76581;
                                defunc_0_split_count_res_76576 = ss_76582;
                                defunc_0_split_count_res_76577 = tt_76583;
                                defunc_0_split_count_res_76578 = tt_76584;
                                defunc_0_split_count_res_76579 = count_76585;
                                ((int64_t *) mem_84130)[i_82348] = defunc_0_split_count_res_76575;
                                ((int64_t *) mem_84132)[i_82348] = defunc_0_split_count_res_76577;
                            }
                            if (mem_84147_cached_sizze_84956 < bytes_84146) {
                                err = lexical_realloc(ctx, &mem_84147, &mem_84147_cached_sizze_84956, bytes_84146);
                                if (err != FUTHARK_SUCCESS)
                                    goto cleanup;
                            }
                            if (mem_84150_cached_sizze_84957 < bytes_84146) {
                                err = lexical_realloc(ctx, &mem_84150, &mem_84150_cached_sizze_84957, bytes_84146);
                                if (err != FUTHARK_SUCCESS)
                                    goto cleanup;
                            }
                            for (int64_t i_82355 = 0; i_82355 < num_blocks_76433; i_82355++) {
                                bool y_76669 = slt64(i_82355, dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_76434);
                                bool index_certs_76671;
                                
                                if (!y_76669) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_82355, "] out of bounds for array of shape [", (long long) dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_76434, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:151:22-34\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  test_dbscan_plus.fut:14:23-19:30\n   #10 test_dbscan_plus.fut:13:1-20:55\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t defunc_0_f_res_76672 = ((int64_t *) mem_84130)[i_82355];
                                int64_t defunc_0_f_res_76673 = ((int64_t *) mem_84132)[i_82355];
                                int64_t tmp_76674 = add64((int64_t) 1, i_82355);
                                bool x_76675 = sle64((int64_t) 0, tmp_76674);
                                bool y_76676 = slt64(tmp_76674, dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_76434);
                                bool bounds_check_76677 = x_76675 && y_76676;
                                bool index_certs_76678;
                                
                                if (!bounds_check_76677) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_76674, "] out of bounds for array of shape [", (long long) dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_76434, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:152:22-36\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  test_dbscan_plus.fut:14:23-19:30\n   #10 test_dbscan_plus.fut:13:1-20:55\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t defunc_0_f_res_76679 = ((int64_t *) mem_84130)[tmp_76674];
                                int64_t defunc_0_f_res_76680 = ((int64_t *) mem_84132)[tmp_76674];
                                int64_t merge_sequential_arg2_76681 = add64(stride_76412, defunc_0_f_res_76673);
                                int64_t merge_sequential_arg2_76682 = add64(stride_76412, defunc_0_f_res_76680);
                                int64_t j_m_i_76683 = sub64(merge_sequential_arg2_76682, merge_sequential_arg2_76681);
                                bool empty_slice_76684 = j_m_i_76683 == (int64_t) 0;
                                int64_t m_76685 = sub64(j_m_i_76683, (int64_t) 1);
                                int64_t i_p_m_t_s_76686 = add64(merge_sequential_arg2_76681, m_76685);
                                bool zzero_leq_i_p_m_t_s_76687 = sle64((int64_t) 0, i_p_m_t_s_76686);
                                bool i_p_m_t_s_leq_w_76688 = slt64(i_p_m_t_s_76686, next_stride_76413);
                                bool zzero_lte_i_76689 = sle64((int64_t) 0, merge_sequential_arg2_76681);
                                bool i_lte_j_76690 = sle64(merge_sequential_arg2_76681, merge_sequential_arg2_76682);
                                bool y_76691 = i_p_m_t_s_leq_w_76688 && zzero_lte_i_76689;
                                bool y_76692 = zzero_leq_i_p_m_t_s_76687 && y_76691;
                                bool forwards_ok_76693 = i_lte_j_76690 && y_76692;
                                bool ok_or_empty_76694 = empty_slice_76684 || forwards_ok_76693;
                                bool index_certs_76695;
                                
                                if (!ok_or_empty_76694) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) merge_sequential_arg2_76681, ":", (long long) merge_sequential_arg2_76682, "] out of bounds for array of shape [", (long long) next_stride_76413, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:40-58\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  test_dbscan_plus.fut:14:23-19:30\n   #10 test_dbscan_plus.fut:13:1-20:55\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t j_m_i_76696 = sub64(defunc_0_f_res_76679, defunc_0_f_res_76672);
                                bool empty_slice_76697 = j_m_i_76696 == (int64_t) 0;
                                int64_t m_76698 = sub64(j_m_i_76696, (int64_t) 1);
                                int64_t i_p_m_t_s_76699 = add64(defunc_0_f_res_76672, m_76698);
                                bool zzero_leq_i_p_m_t_s_76700 = sle64((int64_t) 0, i_p_m_t_s_76699);
                                bool i_p_m_t_s_leq_w_76701 = slt64(i_p_m_t_s_76699, next_stride_76413);
                                bool zzero_lte_i_76702 = sle64((int64_t) 0, defunc_0_f_res_76672);
                                bool i_lte_j_76703 = sle64(defunc_0_f_res_76672, defunc_0_f_res_76679);
                                bool y_76704 = i_p_m_t_s_leq_w_76701 && zzero_lte_i_76702;
                                bool y_76705 = zzero_leq_i_p_m_t_s_76700 && y_76704;
                                bool forwards_ok_76706 = i_lte_j_76703 && y_76705;
                                bool ok_or_empty_76707 = empty_slice_76697 || forwards_ok_76706;
                                bool index_certs_76708;
                                
                                if (!ok_or_empty_76707) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) defunc_0_f_res_76672, ":", (long long) defunc_0_f_res_76679, "] out of bounds for array of shape [", (long long) next_stride_76413, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:29-39\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  test_dbscan_plus.fut:14:23-19:30\n   #10 test_dbscan_plus.fut:13:1-20:55\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                bool cond_76709 = slt64((int64_t) 0, j_m_i_76696);
                                int64_t dummy_76710;
                                int64_t dummy_76711;
                                
                                if (cond_76709) {
                                    bool index_certs_81498;
                                    
                                    if (!cond_76709) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) j_m_i_76696, "].", "-> #0  /prelude/array.fut:22:29-33\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:100:36-42\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #6  /prelude/soacs.fut:59:9-11\n   #7  /prelude/array.fut:241:11-17\n   #8  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #9  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 test_dbscan_plus.fut:14:23-19:30\n   #12 test_dbscan_plus.fut:13:1-20:55\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    int64_t slice_81499 = start_76485 + defunc_0_f_res_76672;
                                    int64_t head_res_81500 = ((int64_t *) mem_param_84111.mem)[slice_81499];
                                    int64_t head_res_81501 = ((int64_t *) mem_param_84114.mem)[slice_81499];
                                    
                                    dummy_76710 = head_res_81500;
                                    dummy_76711 = head_res_81501;
                                } else {
                                    bool y_76716 = slt64((int64_t) 0, j_m_i_76683);
                                    bool index_certs_76717;
                                    
                                    if (!y_76716) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) j_m_i_76683, "].", "-> #0  /prelude/array.fut:22:29-33\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:100:48-54\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #6  /prelude/soacs.fut:59:9-11\n   #7  /prelude/array.fut:241:11-17\n   #8  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #9  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 test_dbscan_plus.fut:14:23-19:30\n   #12 test_dbscan_plus.fut:13:1-20:55\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    int64_t slice_76718 = start_76485 + merge_sequential_arg2_76681;
                                    int64_t head_res_76719 = ((int64_t *) mem_param_84111.mem)[slice_76718];
                                    int64_t head_res_76720 = ((int64_t *) mem_param_84114.mem)[slice_76718];
                                    
                                    dummy_76710 = head_res_76719;
                                    dummy_76711 = head_res_76720;
                                }
                                for (int64_t nest_i_84796 = 0; nest_i_84796 < merge_block_sizze_76252; nest_i_84796++) {
                                    ((int64_t *) mem_84147)[i_82355 * merge_block_sizze_76252 + nest_i_84796] = dummy_76710;
                                }
                                for (int64_t nest_i_84797 = 0; nest_i_84797 < merge_block_sizze_76252; nest_i_84797++) {
                                    ((int64_t *) mem_84150)[i_82355 * merge_block_sizze_76252 + nest_i_84797] = dummy_76711;
                                }
                                
                                int64_t data_76723;
                                int64_t i_76727 = (int64_t) 0;
                                
                                for (int64_t k_76726 = 0; k_76726 < merge_block_sizze_76252; k_76726++) {
                                    int64_t j_76730 = sub64(k_76726, i_76727);
                                    bool cond_76731 = j_76730 == j_m_i_76683;
                                    bool cond_76732;
                                    
                                    if (cond_76731) {
                                        cond_76732 = 1;
                                    } else {
                                        bool cond_76733 = slt64(i_76727, j_m_i_76696);
                                        bool cond_f_res_76734;
                                        
                                        if (cond_76733) {
                                            bool x_81502 = sle64((int64_t) 0, j_76730);
                                            bool y_81503 = slt64(j_76730, j_m_i_76683);
                                            bool bounds_check_81504 = x_81502 && y_81503;
                                            bool index_certs_81505;
                                            
                                            if (!bounds_check_81504) {
                                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_76730, "] out of bounds for array of shape [", (long long) j_m_i_76683, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:55-59\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #8  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #9  /prelude/functional.fut:9:44-45\n   #10 test_dbscan_plus.fut:14:23-19:30\n   #11 test_dbscan_plus.fut:13:1-20:55\n"));
                                                err = FUTHARK_PROGRAM_ERROR;
                                                goto cleanup;
                                            }
                                            
                                            bool x_81510 = sle64((int64_t) 0, i_76727);
                                            bool bounds_check_81511 = cond_76733 && x_81510;
                                            bool index_certs_81512;
                                            
                                            if (!bounds_check_81511) {
                                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_76727, "] out of bounds for array of shape [", (long long) j_m_i_76696, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:50-54\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #8  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #9  /prelude/functional.fut:9:44-45\n   #10 test_dbscan_plus.fut:14:23-19:30\n   #11 test_dbscan_plus.fut:13:1-20:55\n"));
                                                err = FUTHARK_PROGRAM_ERROR;
                                                goto cleanup;
                                            }
                                            
                                            int64_t slice_81506 = merge_sequential_arg2_76681 + j_76730;
                                            int64_t slice_81507 = start_76485 + slice_81506;
                                            int64_t leq_arg1_81508 = ((int64_t *) mem_param_84111.mem)[slice_81507];
                                            int64_t leq_arg1_81509 = ((int64_t *) mem_param_84114.mem)[slice_81507];
                                            int64_t slice_81513 = defunc_0_f_res_76672 + i_76727;
                                            int64_t slice_81514 = start_76485 + slice_81513;
                                            int64_t leq_arg0_81515 = ((int64_t *) mem_param_84111.mem)[slice_81514];
                                            int64_t leq_arg0_81516 = ((int64_t *) mem_param_84114.mem)[slice_81514];
                                            bool defunc_0_leq_res_81517;
                                            
                                            if (futrts_lifted_lambda_12071(ctx, &defunc_0_leq_res_81517, leq_arg0_81515, leq_arg0_81516, leq_arg1_81508, leq_arg1_81509) != 0) {
                                                err = 1;
                                                goto cleanup;
                                            }
                                            cond_f_res_76734 = defunc_0_leq_res_81517;
                                        } else {
                                            cond_f_res_76734 = 0;
                                        }
                                        cond_76732 = cond_f_res_76734;
                                    }
                                    
                                    int64_t loopres_76751;
                                    int64_t loopres_76752;
                                    int64_t loopres_76753;
                                    
                                    if (cond_76732) {
                                        bool x_81519 = sle64((int64_t) 0, i_76727);
                                        bool y_81520 = slt64(i_76727, j_m_i_76696);
                                        bool bounds_check_81521 = x_81519 && y_81520;
                                        bool index_certs_81522;
                                        
                                        if (!bounds_check_81521) {
                                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_76727, "] out of bounds for array of shape [", (long long) j_m_i_76696, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:106:17-21\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #8  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #9  /prelude/functional.fut:9:44-45\n   #10 test_dbscan_plus.fut:14:23-19:30\n   #11 test_dbscan_plus.fut:13:1-20:55\n"));
                                            err = FUTHARK_PROGRAM_ERROR;
                                            goto cleanup;
                                        }
                                        
                                        int64_t tmp_81518 = add64((int64_t) 1, i_76727);
                                        int64_t slice_81523 = defunc_0_f_res_76672 + i_76727;
                                        int64_t slice_81524 = start_76485 + slice_81523;
                                        int64_t tmp_81525 = ((int64_t *) mem_param_84111.mem)[slice_81524];
                                        int64_t tmp_81526 = ((int64_t *) mem_param_84114.mem)[slice_81524];
                                        
                                        loopres_76751 = tmp_81518;
                                        loopres_76752 = tmp_81525;
                                        loopres_76753 = tmp_81526;
                                    } else {
                                        bool x_76763 = sle64((int64_t) 0, j_76730);
                                        bool y_76764 = slt64(j_76730, j_m_i_76683);
                                        bool bounds_check_76765 = x_76763 && y_76764;
                                        bool index_certs_76766;
                                        
                                        if (!bounds_check_76765) {
                                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_76730, "] out of bounds for array of shape [", (long long) j_m_i_76683, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:108:15-19\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #8  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #9  /prelude/functional.fut:9:44-45\n   #10 test_dbscan_plus.fut:14:23-19:30\n   #11 test_dbscan_plus.fut:13:1-20:55\n"));
                                            err = FUTHARK_PROGRAM_ERROR;
                                            goto cleanup;
                                        }
                                        
                                        int64_t slice_76767 = merge_sequential_arg2_76681 + j_76730;
                                        int64_t slice_76768 = start_76485 + slice_76767;
                                        int64_t tmp_76769 = ((int64_t *) mem_param_84111.mem)[slice_76768];
                                        int64_t tmp_76770 = ((int64_t *) mem_param_84114.mem)[slice_76768];
                                        
                                        loopres_76751 = i_76727;
                                        loopres_76752 = tmp_76769;
                                        loopres_76753 = tmp_76770;
                                    }
                                    ((int64_t *) mem_84147)[i_82355 * merge_block_sizze_76252 + k_76726] = loopres_76752;
                                    ((int64_t *) mem_84150)[i_82355 * merge_block_sizze_76252 + k_76726] = loopres_76753;
                                    
                                    int64_t i_tmp_84798 = loopres_76751;
                                    
                                    i_76727 = i_tmp_84798;
                                }
                                data_76723 = i_76727;
                            }
                            if (memblock_alloc(ctx, &mem_84185, bytes_84146, "mem_84185")) {
                                err = 1;
                                goto cleanup;
                            }
                            lmad_copy_8b(ctx, 2, (uint64_t *) mem_84185.mem, (int64_t) 0, (int64_t []) {merge_block_sizze_76252, (int64_t) 1}, (uint64_t *) mem_84147, (int64_t) 0, (int64_t []) {merge_block_sizze_76252, (int64_t) 1}, (int64_t []) {num_blocks_76433, merge_block_sizze_76252});
                            if (memblock_alloc(ctx, &mem_84189, bytes_84146, "mem_84189")) {
                                err = 1;
                                goto cleanup;
                            }
                            lmad_copy_8b(ctx, 2, (uint64_t *) mem_84189.mem, (int64_t) 0, (int64_t []) {merge_block_sizze_76252, (int64_t) 1}, (uint64_t *) mem_84150, (int64_t) 0, (int64_t []) {merge_block_sizze_76252, (int64_t) 1}, (int64_t []) {num_blocks_76433, merge_block_sizze_76252});
                            if (memblock_set(ctx, &ext_mem_84212, &mem_84185, "mem_84185") != 0)
                                return 1;
                            if (memblock_set(ctx, &ext_mem_84209, &mem_84189, "mem_84189") != 0)
                                return 1;
                        }
                        lmad_copy_8b(ctx, 1, (uint64_t *) mem_84117, i_82362 * next_stride_76413, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_84212.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {next_stride_76413});
                        if (memblock_unref(ctx, &ext_mem_84212, "ext_mem_84212") != 0)
                            return 1;
                        lmad_copy_8b(ctx, 1, (uint64_t *) mem_84120, i_82362 * next_stride_76413, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_84209.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {next_stride_76413});
                        if (memblock_unref(ctx, &ext_mem_84209, "ext_mem_84209") != 0)
                            return 1;
                    }
                    
                    int64_t flat_dim_76777 = next_stride_76413 * num_merges_76417;
                    
                    if (memblock_alloc(ctx, &mem_84223, bytes_84116, "mem_84223")) {
                        err = 1;
                        goto cleanup;
                    }
                    lmad_copy_8b(ctx, 2, (uint64_t *) mem_84223.mem, (int64_t) 0, (int64_t []) {next_stride_76413, (int64_t) 1}, (uint64_t *) mem_84117, (int64_t) 0, (int64_t []) {next_stride_76413, (int64_t) 1}, (int64_t []) {num_merges_76417, next_stride_76413});
                    if (memblock_alloc(ctx, &mem_84227, bytes_84116, "mem_84227")) {
                        err = 1;
                        goto cleanup;
                    }
                    lmad_copy_8b(ctx, 2, (uint64_t *) mem_84227.mem, (int64_t) 0, (int64_t []) {next_stride_76413, (int64_t) 1}, (uint64_t *) mem_84120, (int64_t) 0, (int64_t []) {next_stride_76413, (int64_t) 1}, (int64_t []) {num_merges_76417, next_stride_76413});
                    
                    bool loop_cond_76780 = slt64(next_stride_76413, flat_dim_76248);
                    
                    if (memblock_set(ctx, &mem_param_tmp_84772, &mem_84223, "mem_84223") != 0)
                        return 1;
                    if (memblock_set(ctx, &mem_param_tmp_84773, &mem_84227, "mem_84227") != 0)
                        return 1;
                    
                    int64_t loop_dz2084Uz2082U_tmp_84774 = flat_dim_76777;
                    bool loop_while_tmp_84775 = loop_cond_76780;
                    int64_t stride_tmp_84778 = next_stride_76413;
                    
                    if (memblock_set(ctx, &mem_param_84111, &mem_param_tmp_84772, "mem_param_tmp_84772") != 0)
                        return 1;
                    if (memblock_set(ctx, &mem_param_84114, &mem_param_tmp_84773, "mem_param_tmp_84773") != 0)
                        return 1;
                    loop_dz2084Uz2082U_76408 = loop_dz2084Uz2082U_tmp_84774;
                    loop_while_76409 = loop_while_tmp_84775;
                    stride_76412 = stride_tmp_84778;
                }
                if (memblock_set(ctx, &ext_mem_84234, &mem_param_84111, "mem_param_84111") != 0)
                    return 1;
                if (memblock_set(ctx, &ext_mem_84233, &mem_param_84114, "mem_param_84114") != 0)
                    return 1;
                data_76403 = loop_dz2084Uz2082U_76408;
                data_76404 = loop_while_76409;
                data_76407 = stride_76412;
                if (memblock_unref(ctx, &mem_84103, "mem_84103") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_84107, "mem_84107") != 0)
                    return 1;
                
                bool i_p_m_t_s_leq_w_76781 = slt64(zs_lhs_76231, data_76403);
                bool y_76782 = zzero_leq_i_p_m_t_s_76305 && i_p_m_t_s_leq_w_76781;
                bool ok_or_empty_76783 = empty_slice_76304 || y_76782;
                bool index_certs_76784;
                
                if (!ok_or_empty_76783) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) conc_tmp_76160, "] out of bounds for array of shape [", (long long) data_76403, "].", "-> #0  /prelude/array.fut:46:45-51\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:265:3-14\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #3  /prelude/functional.fut:9:44-45\n   #4  test_dbscan_plus.fut:14:23-19:30\n   #5  test_dbscan_plus.fut:13:1-20:55\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                if (memblock_set(ctx, &ext_mem_84240, &ext_mem_84234, "ext_mem_84234") != 0)
                    return 1;
                if (memblock_set(ctx, &ext_mem_84237, &ext_mem_84233, "ext_mem_84233") != 0)
                    return 1;
            }
            if (mem_84242_cached_sizze_84958 < bytes_83992) {
                err = lexical_realloc(ctx, &mem_84242, &mem_84242_cached_sizze_84958, bytes_83992);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            if (mem_84244_cached_sizze_84959 < bytes_83992) {
                err = lexical_realloc(ctx, &mem_84244, &mem_84244_cached_sizze_84959, bytes_83992);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            
            int64_t discard_82373;
            int64_t scanacc_82367 = (int64_t) 0;
            
            for (int64_t i_82370 = 0; i_82370 < conc_tmp_76160; i_82370++) {
                bool cond_79663 = i_82370 == (int64_t) 0;
                bool lifted_lambda_res_79664;
                
                if (cond_79663) {
                    lifted_lambda_res_79664 = 1;
                } else {
                    int64_t znze_rhs_79670 = sub64(i_82370, (int64_t) 1);
                    bool x_79671 = sle64((int64_t) 0, znze_rhs_79670);
                    bool y_79672 = slt64(znze_rhs_79670, conc_tmp_76160);
                    bool bounds_check_79673 = x_79671 && y_79672;
                    bool index_certs_79674;
                    
                    if (!bounds_check_79673) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) znze_rhs_79670, "] out of bounds for array of shape [", (long long) conc_tmp_76160, "].", "-> #0  ../../ftDBSCAN_plus.fut:961:89-103\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../ftDBSCAN_plus.fut:961:36-107\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t lifted_lambda_res_f_res_79669 = ((int64_t *) ext_mem_84240.mem)[i_82370];
                    int64_t lifted_lambda_res_f_res_79675 = ((int64_t *) ext_mem_84240.mem)[znze_rhs_79670];
                    bool lifted_lambda_res_f_res_79676 = lifted_lambda_res_f_res_79669 == lifted_lambda_res_f_res_79675;
                    bool lifted_lambda_res_f_res_79677 = !lifted_lambda_res_f_res_79676;
                    
                    lifted_lambda_res_79664 = lifted_lambda_res_f_res_79677;
                }
                
                int64_t defunc_0_f_res_79679 = btoi_bool_i64(lifted_lambda_res_79664);
                int64_t defunc_0_op_res_76812 = add64(defunc_0_f_res_79679, scanacc_82367);
                
                ((int64_t *) mem_84242)[i_82370] = defunc_0_op_res_76812;
                ((int64_t *) mem_84244)[i_82370] = defunc_0_f_res_79679;
                
                int64_t scanacc_tmp_84801 = defunc_0_op_res_76812;
                
                scanacc_82367 = scanacc_tmp_84801;
            }
            discard_82373 = scanacc_82367;
            
            int64_t m_f_res_76813;
            
            if (x_76306) {
                int64_t x_81535 = ((int64_t *) mem_84242)[zs_lhs_76231];
                
                m_f_res_76813 = x_81535;
            } else {
                m_f_res_76813 = (int64_t) 0;
            }
            
            int64_t m_76815;
            
            if (empty_slice_76304) {
                m_76815 = (int64_t) 0;
            } else {
                m_76815 = m_f_res_76813;
            }
            
            int64_t m_76825 = sub64(m_76815, (int64_t) 1);
            bool i_p_m_t_s_leq_w_76827 = slt64(m_76825, conc_tmp_76160);
            bool zzero_leq_i_p_m_t_s_76826 = sle64((int64_t) 0, m_76825);
            bool y_76829 = zzero_leq_i_p_m_t_s_76826 && i_p_m_t_s_leq_w_76827;
            bool i_lte_j_76828 = sle64((int64_t) 0, m_76815);
            bool forwards_ok_76830 = i_lte_j_76828 && y_76829;
            bool eq_x_zz_76822 = (int64_t) 0 == m_f_res_76813;
            bool p_and_eq_x_y_76823 = x_76306 && eq_x_zz_76822;
            bool empty_slice_76824 = empty_slice_76304 || p_and_eq_x_y_76823;
            bool ok_or_empty_76831 = empty_slice_76824 || forwards_ok_76830;
            bool index_certs_76832;
            
            if (!ok_or_empty_76831) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_76815, "] out of bounds for array of shape [", (long long) conc_tmp_76160, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t bytes_84257 = (int64_t) 8 * m_76815;
            
            if (memblock_alloc(ctx, &mem_84258, bytes_84257, "mem_84258")) {
                err = 1;
                goto cleanup;
            }
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_84258.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_84237.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_76815});
            if (memblock_alloc(ctx, &mem_84260, bytes_84257, "mem_84260")) {
                err = 1;
                goto cleanup;
            }
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_84260.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_84240.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_76815});
            for (int64_t write_iter_82374 = 0; write_iter_82374 < conc_tmp_76160; write_iter_82374++) {
                int64_t write_iv_82377 = ((int64_t *) mem_84244)[write_iter_82374];
                int64_t write_iv_82378 = ((int64_t *) mem_84242)[write_iter_82374];
                int64_t write_iv_82379 = ((int64_t *) ext_mem_84240.mem)[write_iter_82374];
                int64_t write_iv_82380 = ((int64_t *) ext_mem_84237.mem)[write_iter_82374];
                bool cond_79653 = write_iv_82377 == (int64_t) 1;
                int64_t lifted_lambda_res_79654;
                
                if (cond_79653) {
                    int64_t lifted_lambda_res_t_res_81536 = sub64(write_iv_82378, (int64_t) 1);
                    
                    lifted_lambda_res_79654 = lifted_lambda_res_t_res_81536;
                } else {
                    lifted_lambda_res_79654 = (int64_t) -1;
                }
                if (sle64((int64_t) 0, lifted_lambda_res_79654) && slt64(lifted_lambda_res_79654, m_76815)) {
                    ((int64_t *) mem_84260.mem)[lifted_lambda_res_79654] = write_iv_82379;
                }
                if (sle64((int64_t) 0, lifted_lambda_res_79654) && slt64(lifted_lambda_res_79654, m_76815)) {
                    ((int64_t *) mem_84258.mem)[lifted_lambda_res_79654] = write_iv_82380;
                }
            }
            if (memblock_unref(ctx, &ext_mem_84237, "ext_mem_84237") != 0)
                return 1;
            if (memblock_unref(ctx, &ext_mem_84240, "ext_mem_84240") != 0)
                return 1;
            if (memblock_set(ctx, &ext_mem_84278, &mem_84260, "mem_84260") != 0)
                return 1;
            if (memblock_set(ctx, &ext_mem_84275, &mem_84258, "mem_84258") != 0)
                return 1;
            loopres_76312 = m_76815;
            loopres_76313 = m_76815;
            loopres_76315 = m_76815;
        }
        if (memblock_alloc(ctx, &mem_84280, bytes_82614, "mem_84280")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_84280.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_82862.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {dz2081U_57317});
        if (memblock_alloc(ctx, &mem_84282, bytes_82614, "mem_84282")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_84282.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_82865.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {dz2081U_57317});
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_84280.mem, num_flushed_73925, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_84032.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {loopres_76219});
        if (memblock_unref(ctx, &ext_mem_84032, "ext_mem_84032") != 0)
            return 1;
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_84282.mem, num_flushed_73925, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_84031.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {loopres_76219});
        if (memblock_unref(ctx, &ext_mem_84031, "ext_mem_84031") != 0)
            return 1;
        if (memblock_alloc(ctx, &mem_84284, bytes_82614, "mem_84284")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_84284.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_82868.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {dz2081U_57317});
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_84284.mem, num_flushed_73925, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_84030.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {loopres_76219});
        if (memblock_unref(ctx, &ext_mem_84030, "ext_mem_84030") != 0)
            return 1;
        
        bool loop_cond_t_res_76875 = slt64(next_partition_to_read_res_76215, (int64_t) 25);
        bool x_76876 = cond_76216 && loop_cond_t_res_76875;
        
        if (memblock_set(ctx, &mem_param_tmp_84493, &mem_84280, "mem_84280") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84494, &mem_84282, "mem_84282") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84495, &mem_84284, "mem_84284") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84496, &ext_mem_84278, "ext_mem_84278") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84497, &ext_mem_84275, "ext_mem_84275") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84498, &mem_83993, "mem_83993") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84499, &mem_83995, "mem_83995") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84500, &mem_83097, "mem_83097") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84501, &mem_83099, "mem_83099") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84502, &mem_83149, "mem_83149") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84503, &mem_83151, "mem_83151") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84504, &mem_82923, "mem_82923") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84505, &mem_82976, "mem_82976") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84506, &mem_83965, "mem_83965") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84507, &mem_82925, "mem_82925") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84508, &mem_82927, "mem_82927") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84509, &mem_83477, "mem_83477") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84510, &mem_82987, "mem_82987") != 0)
            return 1;
        
        int64_t ctx_param_ext_tmp_84511 = (int64_t) 0;
        int64_t ctx_param_ext_tmp_84512 = (int64_t) 1;
        int64_t loop_dz2085Uz2083U_tmp_84513 = loopres_76312;
        int64_t loop_dz2085Uz2084U_tmp_84514 = loopres_76313;
        int64_t loop_dz2085Uz2085U_tmp_84515 = conc_tmp_76160;
        int64_t loop_dz2086Uz2080U_tmp_84516 = partition_sizze_74623;
        int64_t loop_dz2086Uz2081U_tmp_84517 = partition_sizze_74623;
        int64_t loop_dz2086Uz2082U_tmp_84518 = partition_sizze_74623;
        int64_t loop_dz2086Uz2084U_tmp_84519 = m_74211;
        int64_t loop_dz2087Uz2080U_tmp_84520 = loopres_73979;
        bool loop_while_tmp_84521 = x_76876;
        int64_t num_flushed_tmp_84522 = new_numFlushed_76842;
        int64_t collisions_tmp_84527 = loopres_76315;
        int64_t old_clHandler_tmp_84535 = max_res_76159;
        int64_t old_clHandler_tmp_84536 = new_proc_74723;
        int64_t next_part_tmp_84544 = next_partition_to_read_res_76215;
        
        if (memblock_set(ctx, &mem_param_82862, &mem_param_tmp_84493, "mem_param_tmp_84493") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82865, &mem_param_tmp_84494, "mem_param_tmp_84494") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82868, &mem_param_tmp_84495, "mem_param_tmp_84495") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82871, &mem_param_tmp_84496, "mem_param_tmp_84496") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82874, &mem_param_tmp_84497, "mem_param_tmp_84497") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82877, &mem_param_tmp_84498, "mem_param_tmp_84498") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82880, &mem_param_tmp_84499, "mem_param_tmp_84499") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82883, &mem_param_tmp_84500, "mem_param_tmp_84500") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82886, &mem_param_tmp_84501, "mem_param_tmp_84501") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82889, &mem_param_tmp_84502, "mem_param_tmp_84502") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82892, &mem_param_tmp_84503, "mem_param_tmp_84503") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82895, &mem_param_tmp_84504, "mem_param_tmp_84504") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82898, &mem_param_tmp_84505, "mem_param_tmp_84505") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82901, &mem_param_tmp_84506, "mem_param_tmp_84506") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82904, &mem_param_tmp_84507, "mem_param_tmp_84507") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82907, &mem_param_tmp_84508, "mem_param_tmp_84508") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82910, &mem_param_tmp_84509, "mem_param_tmp_84509") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82913, &mem_param_tmp_84510, "mem_param_tmp_84510") != 0)
            return 1;
        ctx_param_ext_82911 = ctx_param_ext_tmp_84511;
        ctx_param_ext_82912 = ctx_param_ext_tmp_84512;
        loop_dz2085Uz2083U_73916 = loop_dz2085Uz2083U_tmp_84513;
        loop_dz2085Uz2084U_73917 = loop_dz2085Uz2084U_tmp_84514;
        loop_dz2085Uz2085U_73918 = loop_dz2085Uz2085U_tmp_84515;
        loop_dz2086Uz2080U_73919 = loop_dz2086Uz2080U_tmp_84516;
        loop_dz2086Uz2081U_73920 = loop_dz2086Uz2081U_tmp_84517;
        loop_dz2086Uz2082U_73921 = loop_dz2086Uz2082U_tmp_84518;
        loop_dz2086Uz2084U_73922 = loop_dz2086Uz2084U_tmp_84519;
        loop_dz2087Uz2080U_73923 = loop_dz2087Uz2080U_tmp_84520;
        loop_while_73924 = loop_while_tmp_84521;
        num_flushed_73925 = num_flushed_tmp_84522;
        collisions_73930 = collisions_tmp_84527;
        old_clHandler_73938 = old_clHandler_tmp_84535;
        old_clHandler_73939 = old_clHandler_tmp_84536;
        next_part_73947 = next_part_tmp_84544;
    }
    if (memblock_set(ctx, &ext_mem_84338, &mem_param_82862, "mem_param_82862") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84337, &mem_param_82865, "mem_param_82865") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84336, &mem_param_82868, "mem_param_82868") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84335, &mem_param_82871, "mem_param_82871") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84334, &mem_param_82874, "mem_param_82874") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84333, &mem_param_82877, "mem_param_82877") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84332, &mem_param_82880, "mem_param_82880") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84331, &mem_param_82883, "mem_param_82883") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84330, &mem_param_82886, "mem_param_82886") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84329, &mem_param_82889, "mem_param_82889") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84328, &mem_param_82892, "mem_param_82892") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84327, &mem_param_82895, "mem_param_82895") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84326, &mem_param_82898, "mem_param_82898") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84325, &mem_param_82901, "mem_param_82901") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84324, &mem_param_82904, "mem_param_82904") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84323, &mem_param_82907, "mem_param_82907") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84322, &mem_param_82910, "mem_param_82910") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84321, &mem_param_82913, "mem_param_82913") != 0)
        return 1;
    ext_84286 = ctx_param_ext_82911;
    ext_84285 = ctx_param_ext_82912;
    internal_DBSCAN_double_res_73884 = loop_dz2085Uz2083U_73916;
    internal_DBSCAN_double_res_73885 = loop_dz2085Uz2084U_73917;
    internal_DBSCAN_double_res_73886 = loop_dz2085Uz2085U_73918;
    internal_DBSCAN_double_res_73887 = loop_dz2086Uz2080U_73919;
    internal_DBSCAN_double_res_73888 = loop_dz2086Uz2081U_73920;
    internal_DBSCAN_double_res_73889 = loop_dz2086Uz2082U_73921;
    internal_DBSCAN_double_res_73890 = loop_dz2086Uz2084U_73922;
    internal_DBSCAN_double_res_73891 = loop_dz2087Uz2080U_73923;
    internal_DBSCAN_double_res_73892 = loop_while_73924;
    internal_DBSCAN_double_res_73893 = num_flushed_73925;
    internal_DBSCAN_double_res_73898 = collisions_73930;
    internal_DBSCAN_double_res_73906 = old_clHandler_73938;
    internal_DBSCAN_double_res_73907 = old_clHandler_73939;
    internal_DBSCAN_double_res_73915 = next_part_73947;
    if (memblock_unref(ctx, &mem_82852, "mem_82852") != 0)
        return 1;
    if (memblock_unref(ctx, &mem_82853, "mem_82853") != 0)
        return 1;
    if (memblock_unref(ctx, &mem_82854, "mem_82854") != 0)
        return 1;
    if (memblock_unref(ctx, &mem_82855, "mem_82855") != 0)
        return 1;
    if (memblock_unref(ctx, &mem_82857, "mem_82857") != 0)
        return 1;
    if (memblock_unref(ctx, &mem_82859, "mem_82859") != 0)
        return 1;
    
    bool cond_76877 = internal_DBSCAN_double_res_73898 == (int64_t) 0;
    
    if (cond_76877) {
        if (memblock_set(ctx, &ext_mem_84365, &ext_mem_84336, "ext_mem_84336") != 0)
            return 1;
    } else {
        double i64_res_76879 = sitofp_i64_f64(internal_DBSCAN_double_res_73898);
        double log2_res_76880 = futrts_log2_64(i64_res_76879);
        double ceil_res_76881 = futrts_ceil64(log2_res_76880);
        int64_t f64_res_76882 = fptosi_f64_i64(ceil_res_76881);
        int64_t num_iter_76883 = add64((int64_t) 1, f64_res_76882);
        
        if (memblock_alloc(ctx, &mem_84340, bytes_82614, "mem_84340")) {
            err = 1;
            goto cleanup;
        }
        for (int64_t nest_i_84806 = 0; nest_i_84806 < dz2081U_57317; nest_i_84806++) {
            ((int64_t *) mem_84340.mem)[nest_i_84806] = (int64_t) 0;
        }
        
        int64_t bsearch_76886;
        int64_t last_step_76889;
        
        if (memblock_set(ctx, &mem_param_84343, &mem_84340, "mem_84340") != 0)
            return 1;
        last_step_76889 = internal_DBSCAN_double_res_73898;
        for (int64_t i_76887 = 0; i_76887 < num_iter_76883; i_76887++) {
            int64_t zs_lhs_76890 = add64((int64_t) 1, last_step_76889);
            int64_t this_step_76891 = sdiv64(zs_lhs_76890, (int64_t) 2);
            
            if (memblock_alloc(ctx, &mem_84345, bytes_82614, "mem_84345")) {
                err = 1;
                goto cleanup;
            }
            for (int64_t i_82385 = 0; i_82385 < dz2081U_57317; i_82385++) {
                int64_t eta_p_80719 = ((int64_t *) mem_param_84343.mem)[i_82385];
                int64_t eta_p_80720 = ((int64_t *) ext_mem_84336.mem)[i_82385];
                bool cond_80722 = sle64(eta_p_80719, (int64_t) 0);
                int64_t prev_elem_80723;
                
                if (cond_80722) {
                    bool y_81656 = slt64((int64_t) 0, internal_DBSCAN_double_res_73884);
                    bool index_certs_81657;
                    
                    if (!y_81656) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) internal_DBSCAN_double_res_73884, "].", "-> #0  ../../ftDBSCAN_plus.fut:983:66-72\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../ftDBSCAN_plus.fut:982:38-986:36\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t prev_elem_t_res_81658 = ((int64_t *) ext_mem_84335.mem)[(int64_t) 0];
                    
                    prev_elem_80723 = prev_elem_t_res_81658;
                } else {
                    int64_t tmp_80727 = sub64(eta_p_80719, (int64_t) 1);
                    bool x_80728 = sle64((int64_t) 0, tmp_80727);
                    bool y_80729 = slt64(tmp_80727, internal_DBSCAN_double_res_73884);
                    bool bounds_check_80730 = x_80728 && y_80729;
                    bool index_certs_80731;
                    
                    if (!bounds_check_80730) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_80727, "] out of bounds for array of shape [", (long long) internal_DBSCAN_double_res_73884, "].", "-> #0  ../../ftDBSCAN_plus.fut:983:78-86\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../ftDBSCAN_plus.fut:982:38-986:36\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t prev_elem_f_res_80732 = ((int64_t *) ext_mem_84335.mem)[tmp_80727];
                    
                    prev_elem_80723 = prev_elem_f_res_80732;
                }
                
                bool cond_80733 = slt64(eta_p_80719, (int64_t) 0);
                int64_t cur_elem_80734;
                
                if (cond_80733) {
                    bool y_81659 = slt64((int64_t) 0, internal_DBSCAN_double_res_73884);
                    bool index_certs_81660;
                    
                    if (!y_81659) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) internal_DBSCAN_double_res_73884, "].", "-> #0  ../../ftDBSCAN_plus.fut:984:64-70\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../ftDBSCAN_plus.fut:982:38-986:36\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t cur_elem_t_res_81661 = ((int64_t *) ext_mem_84335.mem)[(int64_t) 0];
                    
                    cur_elem_80734 = cur_elem_t_res_81661;
                } else {
                    bool x_80738 = sle64((int64_t) 0, eta_p_80719);
                    bool y_80739 = slt64(eta_p_80719, internal_DBSCAN_double_res_73884);
                    bool bounds_check_80740 = x_80738 && y_80739;
                    bool index_certs_80741;
                    
                    if (!bounds_check_80740) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_80719, "] out of bounds for array of shape [", (long long) internal_DBSCAN_double_res_73884, "].", "-> #0  ../../ftDBSCAN_plus.fut:984:76-82\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../ftDBSCAN_plus.fut:982:38-986:36\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t cur_elem_f_res_80742 = ((int64_t *) ext_mem_84335.mem)[eta_p_80719];
                    
                    cur_elem_80734 = cur_elem_f_res_80742;
                }
                
                int64_t lifted_lambda_res_80746;
                
                if (cond_80733) {
                    lifted_lambda_res_80746 = (int64_t) -1;
                } else {
                    bool cond_80747 = eta_p_80720 == cur_elem_80734;
                    bool cond_80748 = eta_p_80719 == (int64_t) 0;
                    bool cond_t_res_f_res_80749 = slt64(prev_elem_80723, eta_p_80720);
                    bool x_80750 = !cond_80748;
                    bool y_80751 = cond_t_res_f_res_80749 && x_80750;
                    bool cond_t_res_80752 = cond_80748 || y_80751;
                    bool x_80753 = cond_80747 && cond_t_res_80752;
                    int64_t lifted_lambda_res_f_res_80754;
                    
                    if (x_80753) {
                        lifted_lambda_res_f_res_80754 = eta_p_80719;
                    } else {
                        int64_t lifted_lambda_res_f_res_f_res_80755;
                        
                        if (cond_80747) {
                            int64_t max_arg1_81662 = sub64(eta_p_80719, this_step_76891);
                            int64_t max_res_81663 = smax64((int64_t) 0, max_arg1_81662);
                            
                            lifted_lambda_res_f_res_f_res_80755 = max_res_81663;
                        } else {
                            bool cond_80758 = slt64(cur_elem_80734, eta_p_80720);
                            int64_t lifted_lambda_res_f_res_f_res_f_res_80759;
                            
                            if (cond_80758) {
                                int64_t zeze_rhs_81664 = sub64(internal_DBSCAN_double_res_73898, (int64_t) 1);
                                bool cond_81665 = eta_p_80719 == zeze_rhs_81664;
                                int64_t lifted_lambda_res_f_res_f_res_f_res_t_res_81666;
                                
                                if (cond_81665) {
                                    lifted_lambda_res_f_res_f_res_f_res_t_res_81666 = (int64_t) -1;
                                } else {
                                    int64_t min_arg1_81667 = add64(this_step_76891, eta_p_80719);
                                    int64_t min_res_81668 = smin64(zeze_rhs_81664, min_arg1_81667);
                                    
                                    lifted_lambda_res_f_res_f_res_f_res_t_res_81666 = min_res_81668;
                                }
                                lifted_lambda_res_f_res_f_res_f_res_80759 = lifted_lambda_res_f_res_f_res_f_res_t_res_81666;
                            } else {
                                int64_t lifted_lambda_res_f_res_f_res_f_res_f_res_80765;
                                
                                if (cond_t_res_80752) {
                                    lifted_lambda_res_f_res_f_res_f_res_f_res_80765 = (int64_t) -1;
                                } else {
                                    int64_t max_arg1_80766 = sub64(eta_p_80719, this_step_76891);
                                    int64_t max_res_80767 = smax64((int64_t) 0, max_arg1_80766);
                                    
                                    lifted_lambda_res_f_res_f_res_f_res_f_res_80765 = max_res_80767;
                                }
                                lifted_lambda_res_f_res_f_res_f_res_80759 = lifted_lambda_res_f_res_f_res_f_res_f_res_80765;
                            }
                            lifted_lambda_res_f_res_f_res_80755 = lifted_lambda_res_f_res_f_res_f_res_80759;
                        }
                        lifted_lambda_res_f_res_80754 = lifted_lambda_res_f_res_f_res_80755;
                    }
                    lifted_lambda_res_80746 = lifted_lambda_res_f_res_80754;
                }
                ((int64_t *) mem_84345.mem)[i_82385] = lifted_lambda_res_80746;
            }
            if (memblock_set(ctx, &mem_param_tmp_84807, &mem_84345, "mem_84345") != 0)
                return 1;
            
            int64_t last_step_tmp_84809 = this_step_76891;
            
            if (memblock_set(ctx, &mem_param_84343, &mem_param_tmp_84807, "mem_param_tmp_84807") != 0)
                return 1;
            last_step_76889 = last_step_tmp_84809;
        }
        if (memblock_set(ctx, &ext_mem_84354, &mem_param_84343, "mem_param_84343") != 0)
            return 1;
        bsearch_76886 = last_step_76889;
        if (memblock_unref(ctx, &mem_84340, "mem_84340") != 0)
            return 1;
        if (memblock_alloc(ctx, &mem_84356, bytes_82614, "mem_84356")) {
            err = 1;
            goto cleanup;
        }
        for (int64_t i_82389 = 0; i_82389 < dz2081U_57317; i_82389++) {
            int64_t eta_p_76992 = ((int64_t *) ext_mem_84336.mem)[i_82389];
            int64_t eta_p_76993 = ((int64_t *) ext_mem_84354.mem)[i_82389];
            bool cond_76994 = slt64(eta_p_76993, (int64_t) 0);
            bool cond_f_res_76995 = sle64(internal_DBSCAN_double_res_73885, eta_p_76993);
            bool x_76996 = !cond_76994;
            bool y_76997 = cond_f_res_76995 && x_76996;
            bool cond_76998 = cond_76994 || y_76997;
            int64_t lifted_lambda_res_76999;
            
            if (cond_76998) {
                lifted_lambda_res_76999 = eta_p_76992;
            } else {
                bool x_77000 = sle64((int64_t) 0, eta_p_76993);
                bool y_77001 = slt64(eta_p_76993, internal_DBSCAN_double_res_73885);
                bool bounds_check_77002 = x_77000 && y_77001;
                bool index_certs_77003;
                
                if (!bounds_check_77002) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_76993, "] out of bounds for array of shape [", (long long) internal_DBSCAN_double_res_73885, "].", "-> #0  ../../ftbasics.fut:27:66-71\n   #1  /prelude/soacs.fut:67:23-24\n   #2  /prelude/soacs.fut:67:27-37\n   #3  /prelude/functional.fut:9:44-45\n   #4  ../../ftbasics.fut:27:23-77\n   #5  test_dbscan_plus.fut:13:1-20:55\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                int64_t lifted_lambda_res_f_res_77004 = ((int64_t *) ext_mem_84334.mem)[eta_p_76993];
                
                lifted_lambda_res_76999 = lifted_lambda_res_f_res_77004;
            }
            ((int64_t *) mem_84356.mem)[i_82389] = lifted_lambda_res_76999;
        }
        if (memblock_unref(ctx, &ext_mem_84354, "ext_mem_84354") != 0)
            return 1;
        if (memblock_set(ctx, &ext_mem_84365, &mem_84356, "mem_84356") != 0)
            return 1;
    }
    if (memblock_unref(ctx, &ext_mem_84334, "ext_mem_84334") != 0)
        return 1;
    if (memblock_unref(ctx, &ext_mem_84335, "ext_mem_84335") != 0)
        return 1;
    if (memblock_unref(ctx, &ext_mem_84336, "ext_mem_84336") != 0)
        return 1;
    if (memblock_alloc(ctx, &mem_84367, bytes_82614, "mem_84367")) {
        err = 1;
        goto cleanup;
    }
    lmad_copy_8b(ctx, 1, (uint64_t *) mem_84367.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_84338.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {dz2081U_57317});
    if (memblock_unref(ctx, &ext_mem_84338, "ext_mem_84338") != 0)
        return 1;
    if (memblock_alloc(ctx, &mem_84369, bytes_82614, "mem_84369")) {
        err = 1;
        goto cleanup;
    }
    lmad_copy_8b(ctx, 1, (uint64_t *) mem_84369.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_84337.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {dz2081U_57317});
    if (memblock_unref(ctx, &ext_mem_84337, "ext_mem_84337") != 0)
        return 1;
    if (memblock_set(ctx, &mem_out_84433, &mem_84367, "mem_84367") != 0)
        return 1;
    if (memblock_set(ctx, &mem_out_84434, &mem_84369, "mem_84369") != 0)
        return 1;
    if (memblock_set(ctx, &mem_out_84435, &ext_mem_84365, "ext_mem_84365") != 0)
        return 1;
    if (memblock_set(ctx, &*mem_out_p_84812, &mem_out_84433, "mem_out_84433") != 0)
        return 1;
    if (memblock_set(ctx, &*mem_out_p_84813, &mem_out_84434, "mem_out_84434") != 0)
        return 1;
    if (memblock_set(ctx, &*mem_out_p_84814, &mem_out_84435, "mem_out_84435") != 0)
        return 1;
    
  cleanup:
    {
        free(mem_82615);
        free(mem_82626);
        free(mem_82629);
        free(mem_82639);
        free(mem_82641);
        free(mem_82708);
        free(mem_82711);
        free(mem_82721);
        free(mem_82723);
        free(mem_82738);
        free(mem_82741);
        free(mem_82830);
        free(mem_82832);
        free(mem_82845);
        free(mem_82922);
        free(mem_82928);
        free(mem_82929);
        free(mem_82930);
        free(mem_82950);
        free(mem_82958);
        free(mem_82960);
        free(mem_82962);
        free(mem_82984);
        free(mem_82986);
        free(mem_83007);
        free(mem_83009);
        free(mem_83011);
        free(mem_83031);
        free(mem_83033);
        free(mem_83034);
        free(mem_83036);
        free(mem_83062);
        free(mem_83064);
        free(mem_83066);
        free(mem_83086);
        free(mem_83094);
        free(mem_83096);
        free(mem_83125);
        free(mem_83127);
        free(mem_83129);
        free(mem_83153);
        free(mem_83155);
        free(mem_83157);
        free(mem_83177);
        free(mem_83179);
        free(mem_83181);
        free(mem_83183);
        free(mem_83184);
        free(mem_83186);
        free(mem_83224);
        free(mem_83226);
        free(mem_83227);
        free(mem_83229);
        free(mem_83231);
        free(mem_83263);
        free(mem_83265);
        free(mem_83267);
        free(mem_83287);
        free(mem_83289);
        free(mem_83290);
        free(mem_83292);
        free(mem_83294);
        free(mem_83365);
        free(mem_83367);
        free(mem_83368);
        free(mem_83388);
        free(mem_83390);
        free(mem_83392);
        free(mem_83412);
        free(mem_83414);
        free(mem_83428);
        free(mem_83430);
        free(mem_83432);
        free(mem_83453);
        free(mem_83474);
        free(mem_83476);
        free(mem_83497);
        free(mem_83499);
        free(mem_83501);
        free(mem_83521);
        free(mem_83523);
        free(mem_83525);
        free(mem_83534);
        free(mem_83536);
        free(mem_83564);
        free(mem_83567);
        free(mem_83570);
        free(mem_83572);
        free(mem_83588);
        free(mem_83618);
        free(mem_83625);
        free(mem_83636);
        free(mem_83644);
        free(mem_83645);
        free(mem_83659);
        free(mem_83678);
        free(mem_83686);
        free(mem_83687);
        free(mem_83701);
        free(mem_83709);
        free(mem_83717);
        free(mem_83725);
        free(mem_83733);
        free(mem_83735);
        free(mem_83749);
        free(mem_83751);
        free(mem_83765);
        free(mem_83767);
        free(mem_83781);
        free(mem_83783);
        free(mem_83797);
        free(mem_83799);
        free(mem_83813);
        free(mem_83815);
        free(mem_83817);
        free(mem_83837);
        free(mem_83839);
        free(mem_83853);
        free(mem_83855);
        free(mem_83869);
        free(mem_83871);
        free(mem_83885);
        free(mem_83887);
        free(mem_83889);
        free(mem_83891);
        free(mem_83893);
        free(mem_83925);
        free(mem_83927);
        free(mem_83929);
        free(mem_83949);
        free(mem_83996);
        free(mem_83997);
        free(mem_84011);
        free(mem_84035);
        free(mem_84038);
        free(mem_84048);
        free(mem_84050);
        free(mem_84117);
        free(mem_84120);
        free(mem_84130);
        free(mem_84132);
        free(mem_84147);
        free(mem_84150);
        free(mem_84242);
        free(mem_84244);
        if (memblock_unref(ctx, &mem_84369, "mem_84369") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84367, "mem_84367") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84356, "mem_84356") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84807, "mem_param_tmp_84807") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84345, "mem_84345") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_84343, "mem_param_84343") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84354, "ext_mem_84354") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84340, "mem_84340") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84365, "ext_mem_84365") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84510, "mem_param_tmp_84510") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84509, "mem_param_tmp_84509") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84508, "mem_param_tmp_84508") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84507, "mem_param_tmp_84507") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84506, "mem_param_tmp_84506") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84505, "mem_param_tmp_84505") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84504, "mem_param_tmp_84504") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84503, "mem_param_tmp_84503") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84502, "mem_param_tmp_84502") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84501, "mem_param_tmp_84501") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84500, "mem_param_tmp_84500") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84499, "mem_param_tmp_84499") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84498, "mem_param_tmp_84498") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84497, "mem_param_tmp_84497") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84496, "mem_param_tmp_84496") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84495, "mem_param_tmp_84495") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84494, "mem_param_tmp_84494") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84493, "mem_param_tmp_84493") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84284, "mem_84284") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84282, "mem_84282") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84280, "mem_84280") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84260, "mem_84260") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84258, "mem_84258") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84773, "mem_param_tmp_84773") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84772, "mem_param_tmp_84772") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84227, "mem_84227") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84223, "mem_84223") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84189, "mem_84189") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84185, "mem_84185") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84194, "mem_84194") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84192, "mem_84192") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84209, "ext_mem_84209") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84212, "ext_mem_84212") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_84114, "mem_param_84114") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_84111, "mem_param_84111") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84233, "ext_mem_84233") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84234, "ext_mem_84234") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84107, "mem_84107") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84103, "mem_84103") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84237, "ext_mem_84237") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84240, "ext_mem_84240") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84275, "ext_mem_84275") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84278, "ext_mem_84278") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84029, "mem_84029") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84027, "mem_84027") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84025, "mem_84025") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84030, "ext_mem_84030") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84031, "ext_mem_84031") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84032, "ext_mem_84032") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84023, "mem_84023") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84021, "mem_84021") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84019, "mem_84019") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83995, "mem_83995") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83993, "mem_83993") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83965, "mem_83965") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83151, "mem_83151") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83149, "mem_83149") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84663, "mem_param_tmp_84663") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83667, "mem_83667") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_83634, "mem_param_83634") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_83676, "ext_mem_83676") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84641, "mem_param_tmp_84641") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83610, "mem_83610") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_83531, "mem_param_83531") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_83615, "ext_mem_83615") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83477, "mem_83477") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_83466, "ext_mem_83466") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83464, "mem_83464") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83461, "mem_83461") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83458, "mem_83458") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83455, "mem_83455") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_83451, "ext_mem_83451") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_83363, "ext_mem_83363") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83361, "mem_83361") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83358, "mem_83358") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83355, "mem_83355") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83352, "mem_83352") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_83350, "ext_mem_83350") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83348, "mem_83348") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83345, "mem_83345") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83342, "mem_83342") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83339, "mem_83339") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_83337, "ext_mem_83337") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83335, "mem_83335") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83332, "mem_83332") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83329, "mem_83329") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83326, "mem_83326") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83099, "mem_83099") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83097, "mem_83097") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82987, "mem_82987") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82976, "mem_82976") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82927, "mem_82927") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82925, "mem_82925") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82923, "mem_82923") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82917, "mem_82917") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82915, "mem_82915") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82919, "mem_82919") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_82920, "ext_mem_82920") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_82921, "ext_mem_82921") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82913, "mem_param_82913") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82910, "mem_param_82910") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82907, "mem_param_82907") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82904, "mem_param_82904") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82901, "mem_param_82901") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82898, "mem_param_82898") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82895, "mem_param_82895") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82892, "mem_param_82892") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82889, "mem_param_82889") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82886, "mem_param_82886") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82883, "mem_param_82883") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82880, "mem_param_82880") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82877, "mem_param_82877") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82874, "mem_param_82874") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82871, "mem_param_82871") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82868, "mem_param_82868") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82865, "mem_param_82865") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82862, "mem_param_82862") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84321, "ext_mem_84321") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84322, "ext_mem_84322") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84323, "ext_mem_84323") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84324, "ext_mem_84324") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84325, "ext_mem_84325") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84326, "ext_mem_84326") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84327, "ext_mem_84327") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84328, "ext_mem_84328") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84329, "ext_mem_84329") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84330, "ext_mem_84330") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84331, "ext_mem_84331") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84332, "ext_mem_84332") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84333, "ext_mem_84333") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84334, "ext_mem_84334") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84335, "ext_mem_84335") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84336, "ext_mem_84336") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84337, "ext_mem_84337") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84338, "ext_mem_84338") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82859, "mem_82859") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82857, "mem_82857") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82855, "mem_82855") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82854, "mem_82854") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82853, "mem_82853") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82852, "mem_82852") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84458, "mem_param_tmp_84458") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84457, "mem_param_tmp_84457") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82818, "mem_82818") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82814, "mem_82814") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82780, "mem_82780") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82776, "mem_82776") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82785, "mem_82785") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82783, "mem_82783") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_82800, "ext_mem_82800") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_82803, "ext_mem_82803") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82705, "mem_param_82705") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82702, "mem_param_82702") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_82824, "ext_mem_82824") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_82825, "ext_mem_82825") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82698, "mem_82698") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82694, "mem_82694") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_82828, "ext_mem_82828") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82623, "mem_82623") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_84435, "mem_out_84435") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_84434, "mem_out_84434") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_84433, "mem_out_84433") != 0)
            return 1;
    }
    return err;
}
FUTHARK_FUN_ATTR int futrts_get_num_neighbours_against_12423(struct futhark_context *ctx, struct memblock *mem_out_p_84960, struct memblock dat1_mem_82613, struct memblock dat1_mem_82614, struct memblock dat2_mem_82615, struct memblock dat2_mem_82616, int64_t n1_40266, int64_t n2_40267, double eps_40272, int8_t dist_t_40273, int8_t angle_t_40274, double radius_40275, int64_t m_sizze_40276)
{
    (void) ctx;
    
    int err = 0;
    struct memblock mem_82618;
    
    mem_82618.references = NULL;
    
    struct memblock mem_out_84433;
    
    mem_out_84433.references = NULL;
    
    int64_t max_res_57408 = smax64((int64_t) 1, n2_40267);
    bool zzero_40279 = max_res_57408 == (int64_t) 0;
    bool nonzzero_40280 = !zzero_40279;
    bool nonzzero_cert_40281;
    
    if (!nonzzero_40280) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../ftDBSCAN_plus.fut:505:55-69\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t max_arg1_40282 = sdiv64(m_sizze_40276, max_res_57408);
    int64_t max_res_57411 = smax64((int64_t) 1, max_arg1_40282);
    int64_t zm_lhs_40286 = add64(n1_40266, max_res_57411);
    int64_t zs_lhs_40288 = sub64(zm_lhs_40286, (int64_t) 1);
    bool zzero_40290 = max_res_57411 == (int64_t) 0;
    bool nonzzero_40291 = !zzero_40290;
    bool nonzzero_cert_40292;
    
    if (!nonzzero_40291) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../ftDBSCAN_plus.fut:506:68-75\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t max_arg1_40293 = sdiv64(zs_lhs_40288, max_res_57411);
    int64_t max_res_57414 = smax64((int64_t) 0, max_arg1_40293);
    int64_t bytes_82617 = (int64_t) 8 * n1_40266;
    bool bounds_invalid_upwards_57416 = slt64(max_res_57414, (int64_t) 0);
    bool valid_57417 = !bounds_invalid_upwards_57416;
    bool range_valid_c_57418;
    
    if (!valid_57417) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) max_res_57414, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  ../../ftDBSCAN_plus.fut:507:69-82\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    if (memblock_alloc(ctx, &mem_82618, bytes_82617, "mem_82618")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t nest_i_84434 = 0; nest_i_84434 < n1_40266; nest_i_84434++) {
        ((int64_t *) mem_82618.mem)[nest_i_84434] = (int64_t) 0;
    }
    for (int64_t i_40301 = 0; i_40301 < max_res_57414; i_40301++) {
        int64_t inf_40304 = mul64(i_40301, max_res_57411);
        int64_t min_arg1_40306 = add64(inf_40304, max_res_57411);
        int64_t min_res_57426 = smin64(n1_40266, min_arg1_40306);
        int64_t dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_40310 = sub64(min_res_57426, inf_40304);
        bool empty_slice_40322 = dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_40310 == (int64_t) 0;
        int64_t m_40323 = sub64(dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_40310, (int64_t) 1);
        int64_t i_p_m_t_s_40325 = add64(inf_40304, m_40323);
        bool zzero_leq_i_p_m_t_s_40326 = sle64((int64_t) 0, i_p_m_t_s_40325);
        bool i_p_m_t_s_leq_w_40328 = slt64(i_p_m_t_s_40325, n1_40266);
        bool zzero_lte_i_40329 = sle64((int64_t) 0, inf_40304);
        bool i_lte_j_40330 = sle64(inf_40304, min_res_57426);
        bool y_40331 = i_p_m_t_s_leq_w_40328 && zzero_lte_i_40329;
        bool y_40332 = zzero_leq_i_p_m_t_s_40326 && y_40331;
        bool forwards_ok_40333 = i_lte_j_40330 && y_40332;
        bool ok_or_empty_40340 = empty_slice_40322 || forwards_ok_40333;
        bool index_certs_40342;
        
        if (!ok_or_empty_40340) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) inf_40304, ":", (long long) min_res_57426, "] out of bounds for array of shape [", (long long) n1_40266, "].", "-> #0  ../../ftDBSCAN_plus.fut:510:47-60\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        for (int64_t i_81689 = 0; i_81689 < dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_40310; i_81689++) {
            int64_t slice_82392 = inf_40304 + i_81689;
            double eta_p_58012 = ((double *) dat1_mem_82613.mem)[slice_82392];
            double eta_p_58013 = ((double *) dat1_mem_82614.mem)[slice_82392];
            double zt_res_76956 = 3.141592653589793 * eta_p_58012;
            double zt_res_76957 = 3.141592653589793 * eta_p_58013;
            double zs_res_76958 = zt_res_76956 / 180.0;
            double zs_res_76959 = zt_res_76957 / 180.0;
            double d_haversine_res_76960;
            
            if (angle_t_40274 == (int8_t) 1) {
                d_haversine_res_76960 = eta_p_58012;
            } else {
                d_haversine_res_76960 = zs_res_76958;
            }
            
            double d_haversine_res_76961;
            
            if (angle_t_40274 == (int8_t) 1) {
                d_haversine_res_76961 = eta_p_58013;
            } else {
                d_haversine_res_76961 = zs_res_76959;
            }
            
            double cos_res_76962 = futrts_cos64(d_haversine_res_76960);
            int64_t defunc_res_77118;
            int64_t redout_81685 = (int64_t) 0;
            
            for (int64_t i_81686 = 0; i_81686 < n2_40267; i_81686++) {
                double eta_p_77014 = ((double *) dat2_mem_82615.mem)[i_81686];
                double eta_p_77015 = ((double *) dat2_mem_82616.mem)[i_81686];
                double dist_res_77016;
                
                if (dist_t_40273 == (int8_t) 0) {
                    bool zeze_res_77107 = eta_p_58012 == eta_p_77014;
                    double d_euclidean_res_77108;
                    
                    if (zeze_res_77107) {
                        double zm_res_77109 = eta_p_58013 - eta_p_77015;
                        double abs_res_77110 = fabs64(zm_res_77109);
                        
                        d_euclidean_res_77108 = abs_res_77110;
                    } else {
                        bool zeze_res_77111 = eta_p_58013 == eta_p_77015;
                        double d_euclidean_res_f_res_77112;
                        
                        if (zeze_res_77111) {
                            double zm_res_77113 = eta_p_58012 - eta_p_77014;
                            double abs_res_77114 = fabs64(zm_res_77113);
                            
                            d_euclidean_res_f_res_77112 = abs_res_77114;
                        } else {
                            double zm_res_77115 = eta_p_58013 - eta_p_77015;
                            double zm_res_77116 = eta_p_58012 - eta_p_77014;
                            double hypot_res_77117 = futrts_hypot64(zm_res_77116, zm_res_77115);
                            
                            d_euclidean_res_f_res_77112 = hypot_res_77117;
                        }
                        d_euclidean_res_77108 = d_euclidean_res_f_res_77112;
                    }
                    dist_res_77016 = d_euclidean_res_77108;
                } else {
                    double d_haversine_res_77028;
                    double d_haversine_res_77029;
                    
                    if (angle_t_40274 == (int8_t) 1) {
                        d_haversine_res_77028 = eta_p_77014;
                        d_haversine_res_77029 = eta_p_77015;
                    } else {
                        double zt_res_77030 = 3.141592653589793 * eta_p_77014;
                        double zs_res_77031 = zt_res_77030 / 180.0;
                        double zt_res_77032 = 3.141592653589793 * eta_p_77015;
                        double zs_res_77033 = zt_res_77032 / 180.0;
                        
                        d_haversine_res_77028 = zs_res_77031;
                        d_haversine_res_77029 = zs_res_77033;
                    }
                    
                    double zm_res_77034 = d_haversine_res_76960 - d_haversine_res_77028;
                    double cos_res_77035 = futrts_cos64(zm_res_77034);
                    double zm_res_77036 = 1.0 - cos_res_77035;
                    double cos_res_77037 = futrts_cos64(d_haversine_res_77028);
                    double zt_res_77038 = cos_res_76962 * cos_res_77037;
                    double zm_res_77039 = d_haversine_res_76961 - d_haversine_res_77029;
                    double zm_res_77040 = 1.0 - zm_res_77039;
                    double zt_res_77041 = zt_res_77038 * zm_res_77040;
                    double zp_res_77042 = zm_res_77036 + zt_res_77041;
                    double zs_res_77043 = zp_res_77042 / 2.0;
                    double min_res_77044 = fmin64(1.0, zs_res_77043);
                    double zt_res_77045 = 2.0 * min_res_77044;
                    double zm_res_77046 = 1.0 - zt_res_77045;
                    double acos_res_77047 = futrts_acos64(zm_res_77046);
                    double zt_res_77048 = radius_40275 * acos_res_77047;
                    
                    dist_res_77016 = zt_res_77048;
                }
                
                bool zlze_res_77050 = dist_res_77016 <= eps_40272;
                int64_t bool_res_77051 = btoi_bool_i64(zlze_res_77050);
                int64_t zp_res_77007 = add64(bool_res_77051, redout_81685);
                int64_t redout_tmp_84437 = zp_res_77007;
                
                redout_81685 = redout_tmp_84437;
            }
            defunc_res_77118 = redout_81685;
            ((int64_t *) mem_82618.mem)[inf_40304 + i_81689] = defunc_res_77118;
        }
    }
    if (memblock_set(ctx, &mem_out_84433, &mem_82618, "mem_82618") != 0)
        return 1;
    if (memblock_set(ctx, &*mem_out_p_84960, &mem_out_84433, "mem_out_84433") != 0)
        return 1;
    
  cleanup:
    {
        if (memblock_unref(ctx, &mem_82618, "mem_82618") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_84433, "mem_out_84433") != 0)
            return 1;
    }
    return err;
}
FUTHARK_FUN_ATTR int futrts_get_num_neighbours_against_12426(struct futhark_context *ctx, struct memblock *mem_out_p_84961, struct memblock dat1_mem_82613, struct memblock dat1_mem_82614, struct memblock dat2_mem_82615, struct memblock dat2_mem_82616, int64_t n1_40529, int64_t n2_40530, double eps_40535, int8_t dist_t_40536, int8_t angle_t_40537, double radius_40538, int64_t m_sizze_40539)
{
    (void) ctx;
    
    int err = 0;
    struct memblock mem_82618;
    
    mem_82618.references = NULL;
    
    struct memblock mem_out_84433;
    
    mem_out_84433.references = NULL;
    
    int64_t max_res_57408 = smax64((int64_t) 1, n2_40530);
    bool zzero_40542 = max_res_57408 == (int64_t) 0;
    bool nonzzero_40543 = !zzero_40542;
    bool nonzzero_cert_40544;
    
    if (!nonzzero_40543) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../ftDBSCAN_plus.fut:505:55-69\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t max_arg1_40545 = sdiv64(m_sizze_40539, max_res_57408);
    int64_t max_res_57411 = smax64((int64_t) 1, max_arg1_40545);
    int64_t zm_lhs_40549 = add64(n1_40529, max_res_57411);
    int64_t zs_lhs_40551 = sub64(zm_lhs_40549, (int64_t) 1);
    bool zzero_40553 = max_res_57411 == (int64_t) 0;
    bool nonzzero_40554 = !zzero_40553;
    bool nonzzero_cert_40555;
    
    if (!nonzzero_40554) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../ftDBSCAN_plus.fut:506:68-75\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t max_arg1_40556 = sdiv64(zs_lhs_40551, max_res_57411);
    int64_t max_res_57414 = smax64((int64_t) 0, max_arg1_40556);
    int64_t bytes_82617 = (int64_t) 8 * n1_40529;
    bool bounds_invalid_upwards_57416 = slt64(max_res_57414, (int64_t) 0);
    bool valid_57417 = !bounds_invalid_upwards_57416;
    bool range_valid_c_57418;
    
    if (!valid_57417) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) max_res_57414, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  ../../ftDBSCAN_plus.fut:507:69-82\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    if (memblock_alloc(ctx, &mem_82618, bytes_82617, "mem_82618")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t nest_i_84434 = 0; nest_i_84434 < n1_40529; nest_i_84434++) {
        ((int64_t *) mem_82618.mem)[nest_i_84434] = (int64_t) 0;
    }
    for (int64_t i_40564 = 0; i_40564 < max_res_57414; i_40564++) {
        int64_t inf_40567 = mul64(i_40564, max_res_57411);
        int64_t min_arg1_40569 = add64(inf_40567, max_res_57411);
        int64_t min_res_57426 = smin64(n1_40529, min_arg1_40569);
        int64_t dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_40573 = sub64(min_res_57426, inf_40567);
        bool empty_slice_40585 = dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_40573 == (int64_t) 0;
        int64_t m_40586 = sub64(dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_40573, (int64_t) 1);
        int64_t i_p_m_t_s_40588 = add64(inf_40567, m_40586);
        bool zzero_leq_i_p_m_t_s_40589 = sle64((int64_t) 0, i_p_m_t_s_40588);
        bool i_p_m_t_s_leq_w_40591 = slt64(i_p_m_t_s_40588, n1_40529);
        bool zzero_lte_i_40592 = sle64((int64_t) 0, inf_40567);
        bool i_lte_j_40593 = sle64(inf_40567, min_res_57426);
        bool y_40594 = i_p_m_t_s_leq_w_40591 && zzero_lte_i_40592;
        bool y_40595 = zzero_leq_i_p_m_t_s_40589 && y_40594;
        bool forwards_ok_40596 = i_lte_j_40593 && y_40595;
        bool ok_or_empty_40603 = empty_slice_40585 || forwards_ok_40596;
        bool index_certs_40605;
        
        if (!ok_or_empty_40603) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) inf_40567, ":", (long long) min_res_57426, "] out of bounds for array of shape [", (long long) n1_40529, "].", "-> #0  ../../ftDBSCAN_plus.fut:510:47-60\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        for (int64_t i_81689 = 0; i_81689 < dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_40573; i_81689++) {
            int64_t slice_82392 = inf_40567 + i_81689;
            double eta_p_58012 = ((double *) dat1_mem_82613.mem)[slice_82392];
            double eta_p_58013 = ((double *) dat1_mem_82614.mem)[slice_82392];
            double zt_res_76956 = 3.141592653589793 * eta_p_58012;
            double zt_res_76957 = 3.141592653589793 * eta_p_58013;
            double zs_res_76958 = zt_res_76956 / 180.0;
            double zs_res_76959 = zt_res_76957 / 180.0;
            double d_haversine_res_76960;
            
            if (angle_t_40537 == (int8_t) 1) {
                d_haversine_res_76960 = eta_p_58012;
            } else {
                d_haversine_res_76960 = zs_res_76958;
            }
            
            double d_haversine_res_76961;
            
            if (angle_t_40537 == (int8_t) 1) {
                d_haversine_res_76961 = eta_p_58013;
            } else {
                d_haversine_res_76961 = zs_res_76959;
            }
            
            double cos_res_76962 = futrts_cos64(d_haversine_res_76960);
            int64_t defunc_res_77118;
            int64_t redout_81685 = (int64_t) 0;
            
            for (int64_t i_81686 = 0; i_81686 < n2_40530; i_81686++) {
                double eta_p_77014 = ((double *) dat2_mem_82615.mem)[i_81686];
                double eta_p_77015 = ((double *) dat2_mem_82616.mem)[i_81686];
                double dist_res_77016;
                
                if (dist_t_40536 == (int8_t) 0) {
                    bool zeze_res_77107 = eta_p_58012 == eta_p_77014;
                    double d_euclidean_res_77108;
                    
                    if (zeze_res_77107) {
                        double zm_res_77109 = eta_p_58013 - eta_p_77015;
                        double abs_res_77110 = fabs64(zm_res_77109);
                        
                        d_euclidean_res_77108 = abs_res_77110;
                    } else {
                        bool zeze_res_77111 = eta_p_58013 == eta_p_77015;
                        double d_euclidean_res_f_res_77112;
                        
                        if (zeze_res_77111) {
                            double zm_res_77113 = eta_p_58012 - eta_p_77014;
                            double abs_res_77114 = fabs64(zm_res_77113);
                            
                            d_euclidean_res_f_res_77112 = abs_res_77114;
                        } else {
                            double zm_res_77115 = eta_p_58013 - eta_p_77015;
                            double zm_res_77116 = eta_p_58012 - eta_p_77014;
                            double hypot_res_77117 = futrts_hypot64(zm_res_77116, zm_res_77115);
                            
                            d_euclidean_res_f_res_77112 = hypot_res_77117;
                        }
                        d_euclidean_res_77108 = d_euclidean_res_f_res_77112;
                    }
                    dist_res_77016 = d_euclidean_res_77108;
                } else {
                    double d_haversine_res_77028;
                    double d_haversine_res_77029;
                    
                    if (angle_t_40537 == (int8_t) 1) {
                        d_haversine_res_77028 = eta_p_77014;
                        d_haversine_res_77029 = eta_p_77015;
                    } else {
                        double zt_res_77030 = 3.141592653589793 * eta_p_77014;
                        double zs_res_77031 = zt_res_77030 / 180.0;
                        double zt_res_77032 = 3.141592653589793 * eta_p_77015;
                        double zs_res_77033 = zt_res_77032 / 180.0;
                        
                        d_haversine_res_77028 = zs_res_77031;
                        d_haversine_res_77029 = zs_res_77033;
                    }
                    
                    double zm_res_77034 = d_haversine_res_76960 - d_haversine_res_77028;
                    double cos_res_77035 = futrts_cos64(zm_res_77034);
                    double zm_res_77036 = 1.0 - cos_res_77035;
                    double cos_res_77037 = futrts_cos64(d_haversine_res_77028);
                    double zt_res_77038 = cos_res_76962 * cos_res_77037;
                    double zm_res_77039 = d_haversine_res_76961 - d_haversine_res_77029;
                    double zm_res_77040 = 1.0 - zm_res_77039;
                    double zt_res_77041 = zt_res_77038 * zm_res_77040;
                    double zp_res_77042 = zm_res_77036 + zt_res_77041;
                    double zs_res_77043 = zp_res_77042 / 2.0;
                    double min_res_77044 = fmin64(1.0, zs_res_77043);
                    double zt_res_77045 = 2.0 * min_res_77044;
                    double zm_res_77046 = 1.0 - zt_res_77045;
                    double acos_res_77047 = futrts_acos64(zm_res_77046);
                    double zt_res_77048 = radius_40538 * acos_res_77047;
                    
                    dist_res_77016 = zt_res_77048;
                }
                
                bool zlze_res_77050 = dist_res_77016 <= eps_40535;
                int64_t bool_res_77051 = btoi_bool_i64(zlze_res_77050);
                int64_t zp_res_77007 = add64(bool_res_77051, redout_81685);
                int64_t redout_tmp_84437 = zp_res_77007;
                
                redout_81685 = redout_tmp_84437;
            }
            defunc_res_77118 = redout_81685;
            ((int64_t *) mem_82618.mem)[inf_40567 + i_81689] = defunc_res_77118;
        }
    }
    if (memblock_set(ctx, &mem_out_84433, &mem_82618, "mem_82618") != 0)
        return 1;
    if (memblock_set(ctx, &*mem_out_p_84961, &mem_out_84433, "mem_out_84433") != 0)
        return 1;
    
  cleanup:
    {
        if (memblock_unref(ctx, &mem_82618, "mem_82618") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_84433, "mem_out_84433") != 0)
            return 1;
    }
    return err;
}
FUTHARK_FUN_ATTR int futrts_greatest_divisor_leq_than_5592(struct futhark_context *ctx, int64_t *out_prim_out_84962, int64_t upper_bound_33645, int64_t n_33646)
{
    (void) ctx;
    
    int err = 0;
    int64_t prim_out_84433;
    bool upper_bound_33647 = sle64((int64_t) 1, upper_bound_33645);
    bool assert_c_33649;
    
    if (!upper_bound_33647) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "Assertion is false: (upper_bound >= 1)", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:189:21-58\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool cond_33658 = slt64(upper_bound_33645, n_33646);
    bool d_33667;
    int64_t d_33668;
    bool loop_while_33669;
    int64_t d_33670;
    
    loop_while_33669 = cond_33658;
    d_33670 = (int64_t) 1;
    while (loop_while_33669) {
        int64_t loopres_33671 = add64((int64_t) 1, d_33670);
        bool zzero_33673 = loopres_33671 == (int64_t) 0;
        bool nonzzero_33674 = !zzero_33673;
        bool nonzzero_cert_33675;
        
        if (!nonzzero_33674) {
            set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:190:36-38\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t zg_lhs_33676 = sdiv64(n_33646, loopres_33671);
        bool cond_33678 = slt64(upper_bound_33645, zg_lhs_33676);
        bool loop_cond_33679;
        
        if (cond_33678) {
            loop_cond_33679 = 1;
        } else {
            int64_t znze_lhs_33683 = smod64(n_33646, loopres_33671);
            bool loop_cond_f_res_33685 = znze_lhs_33683 == (int64_t) 0;
            bool loop_cond_f_res_33686 = !loop_cond_f_res_33685;
            
            loop_cond_33679 = loop_cond_f_res_33686;
        }
        
        bool loop_while_tmp_84434 = loop_cond_33679;
        int64_t d_tmp_84435 = loopres_33671;
        
        loop_while_33669 = loop_while_tmp_84434;
        d_33670 = d_tmp_84435;
    }
    d_33667 = loop_while_33669;
    d_33668 = d_33670;
    
    bool zzero_33688 = d_33668 == (int64_t) 0;
    bool nonzzero_33689 = !zzero_33688;
    bool nonzzero_cert_33690;
    
    if (!nonzzero_33689) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:191:7-9\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t greatest_divisor_leq_than_res_33691 = sdiv64(n_33646, d_33668);
    
    prim_out_84433 = greatest_divisor_leq_than_res_33691;
    *out_prim_out_84962 = prim_out_84433;
    
  cleanup:
    { }
    return err;
}
FUTHARK_FUN_ATTR int futrts_lifted_get_grid_id_12077(struct futhark_context *ctx, int64_t *out_prim_out_84963, double subdv_x_33978, double step_x_33979, double step_y_33980, double x_33981, double y_33982)
{
    (void) ctx;
    
    int err = 0;
    int64_t prim_out_84433;
    double zbzg_lhs_33983 = x_33981 / step_x_33979;
    double floor_res_58101 = futrts_floor64(zbzg_lhs_33983);
    double zt_lhs_33988 = y_33982 / step_y_33980;
    double zbzg_lhs_33990 = subdv_x_33978 * zt_lhs_33988;
    double floor_res_58104 = futrts_floor64(zbzg_lhs_33990);
    double zbzg_lhs_33995 = floor_res_58101 + floor_res_58104;
    int64_t f64_res_58002 = fptosi_f64_i64(zbzg_lhs_33995);
    
    prim_out_84433 = f64_res_58002;
    *out_prim_out_84963 = prim_out_84433;
    
  cleanup:
    { }
    return err;
}
FUTHARK_FUN_ATTR int futrts_lifted_lambda_12071(struct futhark_context *ctx, bool *out_prim_out_84964, int64_t c1_51373, int64_t p1_51374, int64_t c2_51375, int64_t p2_51376)
{
    (void) ctx;
    
    int err = 0;
    bool prim_out_84433;
    bool cond_51377 = slt64(c1_51373, c2_51375);
    bool cond_51379 = c1_51373 == c2_51375;
    bool lifted_lambda_res_f_res_t_res_51381 = sle64(p1_51374, p2_51376);
    bool x_57325 = cond_51379 && lifted_lambda_res_f_res_t_res_51381;
    bool x_57323 = !cond_51377;
    bool y_57324 = x_57323 && x_57325;
    bool lifted_lambda_res_51378 = cond_51377 || y_57324;
    
    prim_out_84433 = lifted_lambda_res_51378;
    *out_prim_out_84964 = prim_out_84433;
    
  cleanup:
    { }
    return err;
}
FUTHARK_FUN_ATTR int futrts_lifted_normalizze_11972(struct futhark_context *ctx, int64_t *out_prim_out_84965, int64_t *out_prim_out_84966, int64_t *out_prim_out_84967, int64_t *out_prim_out_84968, int64_t *out_prim_out_84969, int64_t slo_33878, int64_t shi_33879, int64_t tlo_33880, int64_t thi_33881, int64_t count_33882)
{
    (void) ctx;
    
    int err = 0;
    int64_t prim_out_84433;
    int64_t prim_out_84434;
    int64_t prim_out_84435;
    int64_t prim_out_84436;
    int64_t prim_out_84437;
    int64_t min_arg1_33885 = add64(slo_33878, count_33882);
    int64_t min_res_57408 = smin64(shi_33879, min_arg1_33885);
    int64_t min_arg1_33889 = add64(tlo_33880, count_33882);
    int64_t min_res_57411 = smin64(thi_33881, min_arg1_33889);
    int64_t zp_lhs_33893 = sub64(min_res_57408, slo_33878);
    int64_t zp_rhs_33895 = sub64(min_res_57411, tlo_33880);
    int64_t zm_lhs_33897 = add64(zp_lhs_33893, zp_rhs_33895);
    int64_t slack_33899 = sub64(zm_lhs_33897, count_33882);
    int64_t min_res_57414 = smin64(zp_lhs_33893, slack_33899);
    int64_t min_res_57417 = smin64(zp_rhs_33895, min_res_57414);
    int64_t max_arg1_33909 = sub64(min_res_57408, min_res_57417);
    int64_t max_res_57420 = smax64(slo_33878, max_arg1_33909);
    int64_t max_arg1_33913 = sub64(min_res_57411, min_res_57417);
    int64_t max_res_57423 = smax64(tlo_33880, max_arg1_33913);
    int64_t zm_rhs_33917 = sub64(max_res_57420, slo_33878);
    int64_t zm_lhs_33919 = sub64(count_33882, zm_rhs_33917);
    int64_t zm_rhs_33921 = sub64(max_res_57423, tlo_33880);
    int64_t count_33923 = sub64(zm_lhs_33919, zm_rhs_33921);
    
    prim_out_84433 = max_res_57420;
    prim_out_84434 = min_res_57408;
    prim_out_84435 = max_res_57423;
    prim_out_84436 = min_res_57411;
    prim_out_84437 = count_33923;
    *out_prim_out_84965 = prim_out_84433;
    *out_prim_out_84966 = prim_out_84434;
    *out_prim_out_84967 = prim_out_84435;
    *out_prim_out_84968 = prim_out_84436;
    *out_prim_out_84969 = prim_out_84437;
    
  cleanup:
    { }
    return err;
}

int futhark_entry_test_dbscan_plus(struct futhark_context *ctx, struct futhark_f64_1d **out0, struct futhark_f64_1d **out1, struct futhark_i64_1d **out2, const struct futhark_f64_2d *in0)
{
    int64_t dz2081U_57317 = (int64_t) 0;
    int ret = 0;
    
    lock_lock(&ctx->lock);
    
    struct memblock mem_out_84435;
    
    mem_out_84435.references = NULL;
    
    struct memblock mem_out_84434;
    
    mem_out_84434.references = NULL;
    
    struct memblock mem_out_84433;
    
    mem_out_84433.references = NULL;
    
    struct memblock pts_mem_82613;
    
    pts_mem_82613.references = NULL;
    pts_mem_82613 = in0->mem;
    dz2081U_57317 = in0->shape[1];
    if (!((int64_t) 2 == in0->shape[0] && dz2081U_57317 == in0->shape[1])) {
        ret = 1;
        set_error(ctx, msgprintf("Error: entry point arguments have invalid sizes.\n"));
    }
    if (ret == 0) {
        ret = futrts_entry_test_dbscan_plus(ctx, &mem_out_84433, &mem_out_84434, &mem_out_84435, pts_mem_82613, dz2081U_57317);
        if (ret == 0) {
            assert((*out0 = (struct futhark_f64_1d *) malloc(sizeof(struct futhark_f64_1d))) != NULL);
            (*out0)->mem = mem_out_84433;
            (*out0)->shape[0] = dz2081U_57317;
            assert((*out1 = (struct futhark_f64_1d *) malloc(sizeof(struct futhark_f64_1d))) != NULL);
            (*out1)->mem = mem_out_84434;
            (*out1)->shape[0] = dz2081U_57317;
            assert((*out2 = (struct futhark_i64_1d *) malloc(sizeof(struct futhark_i64_1d))) != NULL);
            (*out2)->mem = mem_out_84435;
            (*out2)->shape[0] = dz2081U_57317;
        }
    }
    lock_unlock(&ctx->lock);
    return ret;
}
  
