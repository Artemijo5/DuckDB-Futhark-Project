
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

FUTHARK_FUN_ATTR int futrts_entry_test_dbscan_plus(struct futhark_context *ctx, struct memblock *mem_out_p_84576, struct memblock *mem_out_p_84577, struct memblock *mem_out_p_84578, int64_t *out_prim_out_84579, int64_t *out_prim_out_84580, struct memblock pts_mem_82400, int64_t dz2081U_57123);
FUTHARK_FUN_ATTR int futrts_get_num_neighbours_against_12426(struct futhark_context *ctx, struct memblock *mem_out_p_84726, struct memblock dat1_mem_82400, struct memblock dat1_mem_82401, struct memblock dat2_mem_82402, struct memblock dat2_mem_82403, int64_t n1_40130, int64_t n2_40131, double eps_40136, int8_t dist_t_40137, int8_t angle_t_40138, double radius_40139, int64_t m_sizze_40140);
FUTHARK_FUN_ATTR int futrts_get_num_neighbours_against_12429(struct futhark_context *ctx, struct memblock *mem_out_p_84727, struct memblock dat1_mem_82400, struct memblock dat1_mem_82401, struct memblock dat2_mem_82402, struct memblock dat2_mem_82403, int64_t n1_40393, int64_t n2_40394, double eps_40399, int8_t dist_t_40400, int8_t angle_t_40401, double radius_40402, int64_t m_sizze_40403);
FUTHARK_FUN_ATTR int futrts_greatest_divisor_leq_than_5592(struct futhark_context *ctx, int64_t *out_prim_out_84728, int64_t upper_bound_33545, int64_t n_33546);
FUTHARK_FUN_ATTR int futrts_lifted_get_grid_id_12083(struct futhark_context *ctx, int64_t *out_prim_out_84729, double subdv_x_33878, double step_x_33879, double step_y_33880, double x_33881, double y_33882);
FUTHARK_FUN_ATTR int futrts_lifted_lambda_12077(struct futhark_context *ctx, bool *out_prim_out_84730, int64_t c1_51237, int64_t p1_51238, int64_t c2_51239, int64_t p2_51240);
FUTHARK_FUN_ATTR int futrts_lifted_normalizze_11978(struct futhark_context *ctx, int64_t *out_prim_out_84731, int64_t *out_prim_out_84732, int64_t *out_prim_out_84733, int64_t *out_prim_out_84734, int64_t *out_prim_out_84735, int64_t slo_33778, int64_t shi_33779, int64_t tlo_33780, int64_t thi_33781, int64_t count_33782);

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

FUTHARK_FUN_ATTR int futrts_entry_test_dbscan_plus(struct futhark_context *ctx, struct memblock *mem_out_p_84576, struct memblock *mem_out_p_84577, struct memblock *mem_out_p_84578, int64_t *out_prim_out_84579, int64_t *out_prim_out_84580, struct memblock pts_mem_82400, int64_t dz2081U_57123)
{
    (void) ctx;
    
    int err = 0;
    int64_t mem_82402_cached_sizze_84581 = 0;
    unsigned char *mem_82402 = NULL;
    int64_t mem_82413_cached_sizze_84582 = 0;
    unsigned char *mem_82413 = NULL;
    int64_t mem_82416_cached_sizze_84583 = 0;
    unsigned char *mem_82416 = NULL;
    int64_t mem_82426_cached_sizze_84584 = 0;
    unsigned char *mem_82426 = NULL;
    int64_t mem_82428_cached_sizze_84585 = 0;
    unsigned char *mem_82428 = NULL;
    int64_t mem_82495_cached_sizze_84586 = 0;
    unsigned char *mem_82495 = NULL;
    int64_t mem_82498_cached_sizze_84587 = 0;
    unsigned char *mem_82498 = NULL;
    int64_t mem_82508_cached_sizze_84588 = 0;
    unsigned char *mem_82508 = NULL;
    int64_t mem_82510_cached_sizze_84589 = 0;
    unsigned char *mem_82510 = NULL;
    int64_t mem_82525_cached_sizze_84590 = 0;
    unsigned char *mem_82525 = NULL;
    int64_t mem_82528_cached_sizze_84591 = 0;
    unsigned char *mem_82528 = NULL;
    int64_t mem_82617_cached_sizze_84592 = 0;
    unsigned char *mem_82617 = NULL;
    int64_t mem_82619_cached_sizze_84593 = 0;
    unsigned char *mem_82619 = NULL;
    int64_t mem_82632_cached_sizze_84594 = 0;
    unsigned char *mem_82632 = NULL;
    int64_t mem_82698_cached_sizze_84595 = 0;
    unsigned char *mem_82698 = NULL;
    int64_t mem_82704_cached_sizze_84596 = 0;
    unsigned char *mem_82704 = NULL;
    int64_t mem_82705_cached_sizze_84597 = 0;
    unsigned char *mem_82705 = NULL;
    int64_t mem_82706_cached_sizze_84598 = 0;
    unsigned char *mem_82706 = NULL;
    int64_t mem_82726_cached_sizze_84599 = 0;
    unsigned char *mem_82726 = NULL;
    int64_t mem_82734_cached_sizze_84600 = 0;
    unsigned char *mem_82734 = NULL;
    int64_t mem_82736_cached_sizze_84601 = 0;
    unsigned char *mem_82736 = NULL;
    int64_t mem_82738_cached_sizze_84602 = 0;
    unsigned char *mem_82738 = NULL;
    int64_t mem_82760_cached_sizze_84603 = 0;
    unsigned char *mem_82760 = NULL;
    int64_t mem_82762_cached_sizze_84604 = 0;
    unsigned char *mem_82762 = NULL;
    int64_t mem_82783_cached_sizze_84605 = 0;
    unsigned char *mem_82783 = NULL;
    int64_t mem_82785_cached_sizze_84606 = 0;
    unsigned char *mem_82785 = NULL;
    int64_t mem_82787_cached_sizze_84607 = 0;
    unsigned char *mem_82787 = NULL;
    int64_t mem_82807_cached_sizze_84608 = 0;
    unsigned char *mem_82807 = NULL;
    int64_t mem_82809_cached_sizze_84609 = 0;
    unsigned char *mem_82809 = NULL;
    int64_t mem_82810_cached_sizze_84610 = 0;
    unsigned char *mem_82810 = NULL;
    int64_t mem_82812_cached_sizze_84611 = 0;
    unsigned char *mem_82812 = NULL;
    int64_t mem_82838_cached_sizze_84612 = 0;
    unsigned char *mem_82838 = NULL;
    int64_t mem_82840_cached_sizze_84613 = 0;
    unsigned char *mem_82840 = NULL;
    int64_t mem_82842_cached_sizze_84614 = 0;
    unsigned char *mem_82842 = NULL;
    int64_t mem_82862_cached_sizze_84615 = 0;
    unsigned char *mem_82862 = NULL;
    int64_t mem_82870_cached_sizze_84616 = 0;
    unsigned char *mem_82870 = NULL;
    int64_t mem_82872_cached_sizze_84617 = 0;
    unsigned char *mem_82872 = NULL;
    int64_t mem_82901_cached_sizze_84618 = 0;
    unsigned char *mem_82901 = NULL;
    int64_t mem_82903_cached_sizze_84619 = 0;
    unsigned char *mem_82903 = NULL;
    int64_t mem_82905_cached_sizze_84620 = 0;
    unsigned char *mem_82905 = NULL;
    int64_t mem_82929_cached_sizze_84621 = 0;
    unsigned char *mem_82929 = NULL;
    int64_t mem_82931_cached_sizze_84622 = 0;
    unsigned char *mem_82931 = NULL;
    int64_t mem_82933_cached_sizze_84623 = 0;
    unsigned char *mem_82933 = NULL;
    int64_t mem_82953_cached_sizze_84624 = 0;
    unsigned char *mem_82953 = NULL;
    int64_t mem_82955_cached_sizze_84625 = 0;
    unsigned char *mem_82955 = NULL;
    int64_t mem_82957_cached_sizze_84626 = 0;
    unsigned char *mem_82957 = NULL;
    int64_t mem_82959_cached_sizze_84627 = 0;
    unsigned char *mem_82959 = NULL;
    int64_t mem_82960_cached_sizze_84628 = 0;
    unsigned char *mem_82960 = NULL;
    int64_t mem_82962_cached_sizze_84629 = 0;
    unsigned char *mem_82962 = NULL;
    int64_t mem_83000_cached_sizze_84630 = 0;
    unsigned char *mem_83000 = NULL;
    int64_t mem_83002_cached_sizze_84631 = 0;
    unsigned char *mem_83002 = NULL;
    int64_t mem_83003_cached_sizze_84632 = 0;
    unsigned char *mem_83003 = NULL;
    int64_t mem_83005_cached_sizze_84633 = 0;
    unsigned char *mem_83005 = NULL;
    int64_t mem_83007_cached_sizze_84634 = 0;
    unsigned char *mem_83007 = NULL;
    int64_t mem_83039_cached_sizze_84635 = 0;
    unsigned char *mem_83039 = NULL;
    int64_t mem_83041_cached_sizze_84636 = 0;
    unsigned char *mem_83041 = NULL;
    int64_t mem_83043_cached_sizze_84637 = 0;
    unsigned char *mem_83043 = NULL;
    int64_t mem_83063_cached_sizze_84638 = 0;
    unsigned char *mem_83063 = NULL;
    int64_t mem_83065_cached_sizze_84639 = 0;
    unsigned char *mem_83065 = NULL;
    int64_t mem_83066_cached_sizze_84640 = 0;
    unsigned char *mem_83066 = NULL;
    int64_t mem_83068_cached_sizze_84641 = 0;
    unsigned char *mem_83068 = NULL;
    int64_t mem_83070_cached_sizze_84642 = 0;
    unsigned char *mem_83070 = NULL;
    int64_t mem_83141_cached_sizze_84643 = 0;
    unsigned char *mem_83141 = NULL;
    int64_t mem_83143_cached_sizze_84644 = 0;
    unsigned char *mem_83143 = NULL;
    int64_t mem_83144_cached_sizze_84645 = 0;
    unsigned char *mem_83144 = NULL;
    int64_t mem_83164_cached_sizze_84646 = 0;
    unsigned char *mem_83164 = NULL;
    int64_t mem_83166_cached_sizze_84647 = 0;
    unsigned char *mem_83166 = NULL;
    int64_t mem_83168_cached_sizze_84648 = 0;
    unsigned char *mem_83168 = NULL;
    int64_t mem_83188_cached_sizze_84649 = 0;
    unsigned char *mem_83188 = NULL;
    int64_t mem_83190_cached_sizze_84650 = 0;
    unsigned char *mem_83190 = NULL;
    int64_t mem_83204_cached_sizze_84651 = 0;
    unsigned char *mem_83204 = NULL;
    int64_t mem_83206_cached_sizze_84652 = 0;
    unsigned char *mem_83206 = NULL;
    int64_t mem_83208_cached_sizze_84653 = 0;
    unsigned char *mem_83208 = NULL;
    int64_t mem_83229_cached_sizze_84654 = 0;
    unsigned char *mem_83229 = NULL;
    int64_t mem_83250_cached_sizze_84655 = 0;
    unsigned char *mem_83250 = NULL;
    int64_t mem_83252_cached_sizze_84656 = 0;
    unsigned char *mem_83252 = NULL;
    int64_t mem_83273_cached_sizze_84657 = 0;
    unsigned char *mem_83273 = NULL;
    int64_t mem_83275_cached_sizze_84658 = 0;
    unsigned char *mem_83275 = NULL;
    int64_t mem_83277_cached_sizze_84659 = 0;
    unsigned char *mem_83277 = NULL;
    int64_t mem_83297_cached_sizze_84660 = 0;
    unsigned char *mem_83297 = NULL;
    int64_t mem_83299_cached_sizze_84661 = 0;
    unsigned char *mem_83299 = NULL;
    int64_t mem_83301_cached_sizze_84662 = 0;
    unsigned char *mem_83301 = NULL;
    int64_t mem_83310_cached_sizze_84663 = 0;
    unsigned char *mem_83310 = NULL;
    int64_t mem_83312_cached_sizze_84664 = 0;
    unsigned char *mem_83312 = NULL;
    int64_t mem_83340_cached_sizze_84665 = 0;
    unsigned char *mem_83340 = NULL;
    int64_t mem_83343_cached_sizze_84666 = 0;
    unsigned char *mem_83343 = NULL;
    int64_t mem_83346_cached_sizze_84667 = 0;
    unsigned char *mem_83346 = NULL;
    int64_t mem_83348_cached_sizze_84668 = 0;
    unsigned char *mem_83348 = NULL;
    int64_t mem_83364_cached_sizze_84669 = 0;
    unsigned char *mem_83364 = NULL;
    int64_t mem_83394_cached_sizze_84670 = 0;
    unsigned char *mem_83394 = NULL;
    int64_t mem_83401_cached_sizze_84671 = 0;
    unsigned char *mem_83401 = NULL;
    int64_t mem_83412_cached_sizze_84672 = 0;
    unsigned char *mem_83412 = NULL;
    int64_t mem_83420_cached_sizze_84673 = 0;
    unsigned char *mem_83420 = NULL;
    int64_t mem_83421_cached_sizze_84674 = 0;
    unsigned char *mem_83421 = NULL;
    int64_t mem_83435_cached_sizze_84675 = 0;
    unsigned char *mem_83435 = NULL;
    int64_t mem_83454_cached_sizze_84676 = 0;
    unsigned char *mem_83454 = NULL;
    int64_t mem_83462_cached_sizze_84677 = 0;
    unsigned char *mem_83462 = NULL;
    int64_t mem_83463_cached_sizze_84678 = 0;
    unsigned char *mem_83463 = NULL;
    int64_t mem_83477_cached_sizze_84679 = 0;
    unsigned char *mem_83477 = NULL;
    int64_t mem_83485_cached_sizze_84680 = 0;
    unsigned char *mem_83485 = NULL;
    int64_t mem_83493_cached_sizze_84681 = 0;
    unsigned char *mem_83493 = NULL;
    int64_t mem_83501_cached_sizze_84682 = 0;
    unsigned char *mem_83501 = NULL;
    int64_t mem_83509_cached_sizze_84683 = 0;
    unsigned char *mem_83509 = NULL;
    int64_t mem_83511_cached_sizze_84684 = 0;
    unsigned char *mem_83511 = NULL;
    int64_t mem_83525_cached_sizze_84685 = 0;
    unsigned char *mem_83525 = NULL;
    int64_t mem_83527_cached_sizze_84686 = 0;
    unsigned char *mem_83527 = NULL;
    int64_t mem_83541_cached_sizze_84687 = 0;
    unsigned char *mem_83541 = NULL;
    int64_t mem_83543_cached_sizze_84688 = 0;
    unsigned char *mem_83543 = NULL;
    int64_t mem_83557_cached_sizze_84689 = 0;
    unsigned char *mem_83557 = NULL;
    int64_t mem_83559_cached_sizze_84690 = 0;
    unsigned char *mem_83559 = NULL;
    int64_t mem_83573_cached_sizze_84691 = 0;
    unsigned char *mem_83573 = NULL;
    int64_t mem_83575_cached_sizze_84692 = 0;
    unsigned char *mem_83575 = NULL;
    int64_t mem_83589_cached_sizze_84693 = 0;
    unsigned char *mem_83589 = NULL;
    int64_t mem_83591_cached_sizze_84694 = 0;
    unsigned char *mem_83591 = NULL;
    int64_t mem_83593_cached_sizze_84695 = 0;
    unsigned char *mem_83593 = NULL;
    int64_t mem_83613_cached_sizze_84696 = 0;
    unsigned char *mem_83613 = NULL;
    int64_t mem_83615_cached_sizze_84697 = 0;
    unsigned char *mem_83615 = NULL;
    int64_t mem_83629_cached_sizze_84698 = 0;
    unsigned char *mem_83629 = NULL;
    int64_t mem_83631_cached_sizze_84699 = 0;
    unsigned char *mem_83631 = NULL;
    int64_t mem_83645_cached_sizze_84700 = 0;
    unsigned char *mem_83645 = NULL;
    int64_t mem_83647_cached_sizze_84701 = 0;
    unsigned char *mem_83647 = NULL;
    int64_t mem_83661_cached_sizze_84702 = 0;
    unsigned char *mem_83661 = NULL;
    int64_t mem_83663_cached_sizze_84703 = 0;
    unsigned char *mem_83663 = NULL;
    int64_t mem_83665_cached_sizze_84704 = 0;
    unsigned char *mem_83665 = NULL;
    int64_t mem_83667_cached_sizze_84705 = 0;
    unsigned char *mem_83667 = NULL;
    int64_t mem_83669_cached_sizze_84706 = 0;
    unsigned char *mem_83669 = NULL;
    int64_t mem_83701_cached_sizze_84707 = 0;
    unsigned char *mem_83701 = NULL;
    int64_t mem_83703_cached_sizze_84708 = 0;
    unsigned char *mem_83703 = NULL;
    int64_t mem_83705_cached_sizze_84709 = 0;
    unsigned char *mem_83705 = NULL;
    int64_t mem_83725_cached_sizze_84710 = 0;
    unsigned char *mem_83725 = NULL;
    int64_t mem_83772_cached_sizze_84711 = 0;
    unsigned char *mem_83772 = NULL;
    int64_t mem_83773_cached_sizze_84712 = 0;
    unsigned char *mem_83773 = NULL;
    int64_t mem_83787_cached_sizze_84713 = 0;
    unsigned char *mem_83787 = NULL;
    int64_t mem_83811_cached_sizze_84714 = 0;
    unsigned char *mem_83811 = NULL;
    int64_t mem_83814_cached_sizze_84715 = 0;
    unsigned char *mem_83814 = NULL;
    int64_t mem_83824_cached_sizze_84716 = 0;
    unsigned char *mem_83824 = NULL;
    int64_t mem_83826_cached_sizze_84717 = 0;
    unsigned char *mem_83826 = NULL;
    int64_t mem_83893_cached_sizze_84718 = 0;
    unsigned char *mem_83893 = NULL;
    int64_t mem_83896_cached_sizze_84719 = 0;
    unsigned char *mem_83896 = NULL;
    int64_t mem_83906_cached_sizze_84720 = 0;
    unsigned char *mem_83906 = NULL;
    int64_t mem_83908_cached_sizze_84721 = 0;
    unsigned char *mem_83908 = NULL;
    int64_t mem_83923_cached_sizze_84722 = 0;
    unsigned char *mem_83923 = NULL;
    int64_t mem_83926_cached_sizze_84723 = 0;
    unsigned char *mem_83926 = NULL;
    int64_t mem_84018_cached_sizze_84724 = 0;
    unsigned char *mem_84018 = NULL;
    int64_t mem_84020_cached_sizze_84725 = 0;
    unsigned char *mem_84020 = NULL;
    struct memblock mem_84149;
    
    mem_84149.references = NULL;
    
    struct memblock mem_84147;
    
    mem_84147.references = NULL;
    
    struct memblock mem_84136;
    
    mem_84136.references = NULL;
    
    struct memblock mem_param_tmp_84571;
    
    mem_param_tmp_84571.references = NULL;
    
    struct memblock mem_84125;
    
    mem_84125.references = NULL;
    
    struct memblock mem_param_84123;
    
    mem_param_84123.references = NULL;
    
    struct memblock ext_mem_84134;
    
    ext_mem_84134.references = NULL;
    
    struct memblock mem_84120;
    
    mem_84120.references = NULL;
    
    struct memblock ext_mem_84145;
    
    ext_mem_84145.references = NULL;
    
    struct memblock mem_param_tmp_84270;
    
    mem_param_tmp_84270.references = NULL;
    
    struct memblock mem_param_tmp_84269;
    
    mem_param_tmp_84269.references = NULL;
    
    struct memblock mem_param_tmp_84268;
    
    mem_param_tmp_84268.references = NULL;
    
    struct memblock mem_param_tmp_84267;
    
    mem_param_tmp_84267.references = NULL;
    
    struct memblock mem_param_tmp_84266;
    
    mem_param_tmp_84266.references = NULL;
    
    struct memblock mem_param_tmp_84265;
    
    mem_param_tmp_84265.references = NULL;
    
    struct memblock mem_param_tmp_84264;
    
    mem_param_tmp_84264.references = NULL;
    
    struct memblock mem_param_tmp_84263;
    
    mem_param_tmp_84263.references = NULL;
    
    struct memblock mem_param_tmp_84262;
    
    mem_param_tmp_84262.references = NULL;
    
    struct memblock mem_param_tmp_84261;
    
    mem_param_tmp_84261.references = NULL;
    
    struct memblock mem_param_tmp_84260;
    
    mem_param_tmp_84260.references = NULL;
    
    struct memblock mem_param_tmp_84259;
    
    mem_param_tmp_84259.references = NULL;
    
    struct memblock mem_param_tmp_84258;
    
    mem_param_tmp_84258.references = NULL;
    
    struct memblock mem_param_tmp_84257;
    
    mem_param_tmp_84257.references = NULL;
    
    struct memblock mem_param_tmp_84256;
    
    mem_param_tmp_84256.references = NULL;
    
    struct memblock mem_param_tmp_84255;
    
    mem_param_tmp_84255.references = NULL;
    
    struct memblock mem_param_tmp_84254;
    
    mem_param_tmp_84254.references = NULL;
    
    struct memblock mem_param_tmp_84253;
    
    mem_param_tmp_84253.references = NULL;
    
    struct memblock mem_84064;
    
    mem_84064.references = NULL;
    
    struct memblock mem_84062;
    
    mem_84062.references = NULL;
    
    struct memblock mem_84060;
    
    mem_84060.references = NULL;
    
    struct memblock mem_84050;
    
    mem_84050.references = NULL;
    
    struct memblock mem_84052;
    
    mem_84052.references = NULL;
    
    struct memblock mem_param_tmp_84532;
    
    mem_param_tmp_84532.references = NULL;
    
    struct memblock mem_param_tmp_84531;
    
    mem_param_tmp_84531.references = NULL;
    
    struct memblock mem_84003;
    
    mem_84003.references = NULL;
    
    struct memblock mem_83999;
    
    mem_83999.references = NULL;
    
    struct memblock mem_83965;
    
    mem_83965.references = NULL;
    
    struct memblock mem_83961;
    
    mem_83961.references = NULL;
    
    struct memblock mem_83970;
    
    mem_83970.references = NULL;
    
    struct memblock mem_83968;
    
    mem_83968.references = NULL;
    
    struct memblock ext_mem_83985;
    
    ext_mem_83985.references = NULL;
    
    struct memblock ext_mem_83988;
    
    ext_mem_83988.references = NULL;
    
    struct memblock mem_param_83890;
    
    mem_param_83890.references = NULL;
    
    struct memblock mem_param_83887;
    
    mem_param_83887.references = NULL;
    
    struct memblock ext_mem_84009;
    
    ext_mem_84009.references = NULL;
    
    struct memblock ext_mem_84010;
    
    ext_mem_84010.references = NULL;
    
    struct memblock mem_83883;
    
    mem_83883.references = NULL;
    
    struct memblock mem_83879;
    
    mem_83879.references = NULL;
    
    struct memblock ext_mem_84013;
    
    ext_mem_84013.references = NULL;
    
    struct memblock ext_mem_84016;
    
    ext_mem_84016.references = NULL;
    
    struct memblock ext_mem_84055;
    
    ext_mem_84055.references = NULL;
    
    struct memblock ext_mem_84058;
    
    ext_mem_84058.references = NULL;
    
    struct memblock mem_83805;
    
    mem_83805.references = NULL;
    
    struct memblock mem_83803;
    
    mem_83803.references = NULL;
    
    struct memblock mem_83801;
    
    mem_83801.references = NULL;
    
    struct memblock ext_mem_83806;
    
    ext_mem_83806.references = NULL;
    
    struct memblock ext_mem_83807;
    
    ext_mem_83807.references = NULL;
    
    struct memblock ext_mem_83808;
    
    ext_mem_83808.references = NULL;
    
    struct memblock mem_83799;
    
    mem_83799.references = NULL;
    
    struct memblock mem_83797;
    
    mem_83797.references = NULL;
    
    struct memblock mem_83795;
    
    mem_83795.references = NULL;
    
    struct memblock mem_83771;
    
    mem_83771.references = NULL;
    
    struct memblock mem_83769;
    
    mem_83769.references = NULL;
    
    struct memblock mem_83741;
    
    mem_83741.references = NULL;
    
    struct memblock mem_82927;
    
    mem_82927.references = NULL;
    
    struct memblock mem_82925;
    
    mem_82925.references = NULL;
    
    struct memblock mem_param_tmp_84422;
    
    mem_param_tmp_84422.references = NULL;
    
    struct memblock mem_83443;
    
    mem_83443.references = NULL;
    
    struct memblock mem_param_83410;
    
    mem_param_83410.references = NULL;
    
    struct memblock ext_mem_83452;
    
    ext_mem_83452.references = NULL;
    
    struct memblock mem_param_tmp_84400;
    
    mem_param_tmp_84400.references = NULL;
    
    struct memblock mem_83386;
    
    mem_83386.references = NULL;
    
    struct memblock mem_param_83307;
    
    mem_param_83307.references = NULL;
    
    struct memblock ext_mem_83391;
    
    ext_mem_83391.references = NULL;
    
    struct memblock mem_83253;
    
    mem_83253.references = NULL;
    
    struct memblock ext_mem_83242;
    
    ext_mem_83242.references = NULL;
    
    struct memblock mem_83240;
    
    mem_83240.references = NULL;
    
    struct memblock mem_83237;
    
    mem_83237.references = NULL;
    
    struct memblock mem_83234;
    
    mem_83234.references = NULL;
    
    struct memblock mem_83231;
    
    mem_83231.references = NULL;
    
    struct memblock ext_mem_83227;
    
    ext_mem_83227.references = NULL;
    
    struct memblock ext_mem_83139;
    
    ext_mem_83139.references = NULL;
    
    struct memblock mem_83137;
    
    mem_83137.references = NULL;
    
    struct memblock mem_83134;
    
    mem_83134.references = NULL;
    
    struct memblock mem_83131;
    
    mem_83131.references = NULL;
    
    struct memblock mem_83128;
    
    mem_83128.references = NULL;
    
    struct memblock ext_mem_83126;
    
    ext_mem_83126.references = NULL;
    
    struct memblock mem_83124;
    
    mem_83124.references = NULL;
    
    struct memblock mem_83121;
    
    mem_83121.references = NULL;
    
    struct memblock mem_83118;
    
    mem_83118.references = NULL;
    
    struct memblock mem_83115;
    
    mem_83115.references = NULL;
    
    struct memblock ext_mem_83113;
    
    ext_mem_83113.references = NULL;
    
    struct memblock mem_83111;
    
    mem_83111.references = NULL;
    
    struct memblock mem_83108;
    
    mem_83108.references = NULL;
    
    struct memblock mem_83105;
    
    mem_83105.references = NULL;
    
    struct memblock mem_83102;
    
    mem_83102.references = NULL;
    
    struct memblock mem_82875;
    
    mem_82875.references = NULL;
    
    struct memblock mem_82873;
    
    mem_82873.references = NULL;
    
    struct memblock mem_82763;
    
    mem_82763.references = NULL;
    
    struct memblock mem_82752;
    
    mem_82752.references = NULL;
    
    struct memblock mem_82703;
    
    mem_82703.references = NULL;
    
    struct memblock mem_82701;
    
    mem_82701.references = NULL;
    
    struct memblock mem_82699;
    
    mem_82699.references = NULL;
    
    struct memblock mem_param_82697;
    
    mem_param_82697.references = NULL;
    
    struct memblock mem_param_82694;
    
    mem_param_82694.references = NULL;
    
    struct memblock mem_param_82691;
    
    mem_param_82691.references = NULL;
    
    struct memblock mem_param_82688;
    
    mem_param_82688.references = NULL;
    
    struct memblock mem_param_82685;
    
    mem_param_82685.references = NULL;
    
    struct memblock mem_param_82682;
    
    mem_param_82682.references = NULL;
    
    struct memblock mem_param_82679;
    
    mem_param_82679.references = NULL;
    
    struct memblock mem_param_82676;
    
    mem_param_82676.references = NULL;
    
    struct memblock mem_param_82673;
    
    mem_param_82673.references = NULL;
    
    struct memblock mem_param_82670;
    
    mem_param_82670.references = NULL;
    
    struct memblock mem_param_82667;
    
    mem_param_82667.references = NULL;
    
    struct memblock mem_param_82664;
    
    mem_param_82664.references = NULL;
    
    struct memblock mem_param_82661;
    
    mem_param_82661.references = NULL;
    
    struct memblock mem_param_82658;
    
    mem_param_82658.references = NULL;
    
    struct memblock mem_param_82655;
    
    mem_param_82655.references = NULL;
    
    struct memblock mem_param_82652;
    
    mem_param_82652.references = NULL;
    
    struct memblock mem_param_82649;
    
    mem_param_82649.references = NULL;
    
    struct memblock mem_param_82646;
    
    mem_param_82646.references = NULL;
    
    struct memblock ext_mem_84101;
    
    ext_mem_84101.references = NULL;
    
    struct memblock ext_mem_84102;
    
    ext_mem_84102.references = NULL;
    
    struct memblock ext_mem_84103;
    
    ext_mem_84103.references = NULL;
    
    struct memblock ext_mem_84104;
    
    ext_mem_84104.references = NULL;
    
    struct memblock ext_mem_84105;
    
    ext_mem_84105.references = NULL;
    
    struct memblock ext_mem_84106;
    
    ext_mem_84106.references = NULL;
    
    struct memblock ext_mem_84107;
    
    ext_mem_84107.references = NULL;
    
    struct memblock ext_mem_84108;
    
    ext_mem_84108.references = NULL;
    
    struct memblock ext_mem_84109;
    
    ext_mem_84109.references = NULL;
    
    struct memblock ext_mem_84110;
    
    ext_mem_84110.references = NULL;
    
    struct memblock ext_mem_84111;
    
    ext_mem_84111.references = NULL;
    
    struct memblock ext_mem_84112;
    
    ext_mem_84112.references = NULL;
    
    struct memblock ext_mem_84113;
    
    ext_mem_84113.references = NULL;
    
    struct memblock ext_mem_84114;
    
    ext_mem_84114.references = NULL;
    
    struct memblock ext_mem_84115;
    
    ext_mem_84115.references = NULL;
    
    struct memblock ext_mem_84116;
    
    ext_mem_84116.references = NULL;
    
    struct memblock ext_mem_84117;
    
    ext_mem_84117.references = NULL;
    
    struct memblock ext_mem_84118;
    
    ext_mem_84118.references = NULL;
    
    struct memblock mem_82643;
    
    mem_82643.references = NULL;
    
    struct memblock mem_82642;
    
    mem_82642.references = NULL;
    
    struct memblock mem_82641;
    
    mem_82641.references = NULL;
    
    struct memblock mem_82640;
    
    mem_82640.references = NULL;
    
    struct memblock mem_82639;
    
    mem_82639.references = NULL;
    
    struct memblock mem_param_tmp_84220;
    
    mem_param_tmp_84220.references = NULL;
    
    struct memblock mem_param_tmp_84219;
    
    mem_param_tmp_84219.references = NULL;
    
    struct memblock mem_82605;
    
    mem_82605.references = NULL;
    
    struct memblock mem_82601;
    
    mem_82601.references = NULL;
    
    struct memblock mem_82567;
    
    mem_82567.references = NULL;
    
    struct memblock mem_82563;
    
    mem_82563.references = NULL;
    
    struct memblock mem_82572;
    
    mem_82572.references = NULL;
    
    struct memblock mem_82570;
    
    mem_82570.references = NULL;
    
    struct memblock ext_mem_82587;
    
    ext_mem_82587.references = NULL;
    
    struct memblock ext_mem_82590;
    
    ext_mem_82590.references = NULL;
    
    struct memblock mem_param_82492;
    
    mem_param_82492.references = NULL;
    
    struct memblock mem_param_82489;
    
    mem_param_82489.references = NULL;
    
    struct memblock ext_mem_82611;
    
    ext_mem_82611.references = NULL;
    
    struct memblock ext_mem_82612;
    
    ext_mem_82612.references = NULL;
    
    struct memblock mem_82485;
    
    mem_82485.references = NULL;
    
    struct memblock mem_82481;
    
    mem_82481.references = NULL;
    
    struct memblock ext_mem_82615;
    
    ext_mem_82615.references = NULL;
    
    struct memblock mem_82410;
    
    mem_82410.references = NULL;
    
    struct memblock mem_out_84195;
    
    mem_out_84195.references = NULL;
    
    struct memblock mem_out_84194;
    
    mem_out_84194.references = NULL;
    
    struct memblock mem_out_84193;
    
    mem_out_84193.references = NULL;
    
    int64_t prim_out_84196;
    int64_t prim_out_84197;
    double defunc_0_reduce_res_81448;
    double defunc_0_reduce_res_81449;
    double redout_81462;
    double redout_81463;
    
    redout_81462 = -INFINITY;
    redout_81463 = INFINITY;
    for (int64_t i_81464 = 0; i_81464 < dz2081U_57123; i_81464++) {
        double x_76763 = ((double *) pts_mem_82400.mem)[i_81464];
        double max_res_73105 = fmax64(x_76763, redout_81462);
        double min_res_73115 = fmin64(x_76763, redout_81463);
        double redout_tmp_84198 = max_res_73105;
        double redout_tmp_84199 = min_res_73115;
        
        redout_81462 = redout_tmp_84198;
        redout_81463 = redout_tmp_84199;
    }
    defunc_0_reduce_res_81448 = redout_81462;
    defunc_0_reduce_res_81449 = redout_81463;
    
    double defunc_0_reduce_res_81450;
    double defunc_0_reduce_res_81451;
    double redout_81465;
    double redout_81466;
    
    redout_81465 = -INFINITY;
    redout_81466 = INFINITY;
    for (int64_t i_81467 = 0; i_81467 < dz2081U_57123; i_81467++) {
        double x_76766 = ((double *) pts_mem_82400.mem)[dz2081U_57123 + i_81467];
        double max_res_73110 = fmax64(x_76766, redout_81465);
        double min_res_73120 = fmin64(x_76766, redout_81466);
        double redout_tmp_84200 = max_res_73110;
        double redout_tmp_84201 = min_res_73120;
        
        redout_81465 = redout_tmp_84200;
        redout_81466 = redout_tmp_84201;
    }
    defunc_0_reduce_res_81450 = redout_81465;
    defunc_0_reduce_res_81451 = redout_81466;
    
    double zs_lhs_73121 = defunc_0_reduce_res_81448 - defunc_0_reduce_res_81449;
    double step_x_73122 = zs_lhs_73121 / 5.0;
    double zs_lhs_73123 = defunc_0_reduce_res_81450 - defunc_0_reduce_res_81451;
    double step_y_73124 = zs_lhs_73123 / 5.0;
    int64_t bytes_82401 = (int64_t) 8 * dz2081U_57123;
    
    if (mem_82402_cached_sizze_84581 < bytes_82401) {
        err = lexical_realloc(ctx, &mem_82402, &mem_82402_cached_sizze_84581, bytes_82401);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    for (int64_t i_81470 = 0; i_81470 < dz2081U_57123; i_81470++) {
        double eta_p_73126 = ((double *) pts_mem_82400.mem)[i_81470];
        double eta_p_73127 = ((double *) pts_mem_82400.mem)[dz2081U_57123 + i_81470];
        int64_t defunc_0_f_res_73128;
        
        if (futrts_lifted_get_grid_id_12083(ctx, &defunc_0_f_res_73128, 5.0, step_x_73122, step_y_73124, eta_p_73126, eta_p_73127) != 0) {
            err = 1;
            goto cleanup;
        }
        ((int64_t *) mem_82402)[i_81470] = defunc_0_f_res_73128;
    }
    if (memblock_alloc(ctx, &mem_82410, bytes_82401, "mem_82410")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t i_84203 = 0; i_84203 < dz2081U_57123; i_84203++) {
        int64_t x_84204 = (int64_t) 0 + i_84203 * (int64_t) 1;
        
        ((int64_t *) mem_82410.mem)[i_84203] = x_84204;
    }
    
    bool cond_73130 = sle64(dz2081U_57123, (int64_t) 1);
    int64_t zs_lhs_73131 = sub64(dz2081U_57123, (int64_t) 1);
    int64_t zp_rhs_73132 = sdiv_safe64(zs_lhs_73131, (int64_t) 20);
    int64_t min_num_blocks_73133 = add64((int64_t) 1, zp_rhs_73132);
    bool loop_cond_73134 = slt64((int64_t) 1, min_num_blocks_73133);
    bool smallest_pow_2_geq_than_res_73135;
    int64_t smallest_pow_2_geq_than_res_73136;
    bool loop_while_73137;
    int64_t x_73138;
    
    loop_while_73137 = loop_cond_73134;
    x_73138 = (int64_t) 1;
    while (loop_while_73137) {
        int64_t loopres_73139 = mul64((int64_t) 2, x_73138);
        bool loop_cond_73140 = slt64(loopres_73139, min_num_blocks_73133);
        bool loop_while_tmp_84205 = loop_cond_73140;
        int64_t x_tmp_84206 = loopres_73139;
        
        loop_while_73137 = loop_while_tmp_84205;
        x_73138 = x_tmp_84206;
    }
    smallest_pow_2_geq_than_res_73135 = loop_while_73137;
    smallest_pow_2_geq_than_res_73136 = x_73138;
    
    bool zzero_73141 = smallest_pow_2_geq_than_res_73136 == (int64_t) 0;
    bool nonzzero_73142 = !zzero_73141;
    bool protect_assert_disj_73143 = cond_73130 || nonzzero_73142;
    bool nonzzero_cert_73144;
    
    if (!protect_assert_disj_73143) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:234:30-42\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #2  /prelude/functional.fut:9:44-45\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t zp_rhs_73145 = sdiv_safe64(zs_lhs_73131, smallest_pow_2_geq_than_res_73136);
    int64_t block_sizze_73146 = add64((int64_t) 1, zp_rhs_73145);
    int64_t flat_dim_73147 = smallest_pow_2_geq_than_res_73136 * block_sizze_73146;
    bool x_73148 = !cond_73130;
    int64_t greatest_divisor_leq_than_arg1_73149 = mul64((int64_t) 2, block_sizze_73146);
    int64_t merge_block_sizze_73150;
    
    if (x_73148) {
        int64_t x_80571;
        
        if (futrts_greatest_divisor_leq_than_5592(ctx, &x_80571, (int64_t) 8, greatest_divisor_leq_than_arg1_73149) != 0) {
            err = 1;
            goto cleanup;
        }
        merge_block_sizze_73150 = x_80571;
    } else {
        merge_block_sizze_73150 = (int64_t) 0;
    }
    
    bool y_73152 = slt64((int64_t) 0, dz2081U_57123);
    bool protect_assert_disj_73153 = cond_73130 || y_73152;
    bool index_certs_73154;
    
    if (!protect_assert_disj_73153) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) dz2081U_57123, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:240:60-64\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #2  /prelude/functional.fut:9:44-45\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t reduce_arg1_73155;
    
    if (x_73148) {
        int64_t x_80572 = ((int64_t *) mem_82402)[(int64_t) 0];
        
        reduce_arg1_73155 = x_80572;
    } else {
        reduce_arg1_73155 = (int64_t) 0;
    }
    
    int64_t defunc_0_reduce_res_73157;
    int64_t defunc_0_reduce_res_73158;
    
    if (x_73148) {
        int64_t x_80575;
        int64_t x_80576;
        int64_t redout_81472;
        int64_t redout_81473;
        
        redout_81472 = reduce_arg1_73155;
        redout_81473 = (int64_t) 0;
        for (int64_t i_81474 = 0; i_81474 < dz2081U_57123; i_81474++) {
            int64_t x_80577 = ((int64_t *) mem_82402)[i_81474];
            bool defunc_0_leq_res_80583 = sle64(redout_81472, x_80577);
            int64_t defunc_0_op_res_80584;
            
            if (defunc_0_leq_res_80583) {
                defunc_0_op_res_80584 = x_80577;
            } else {
                defunc_0_op_res_80584 = redout_81472;
            }
            
            int64_t defunc_0_op_res_80585;
            
            if (defunc_0_leq_res_80583) {
                defunc_0_op_res_80585 = i_81474;
            } else {
                defunc_0_op_res_80585 = redout_81473;
            }
            
            int64_t redout_tmp_84207 = defunc_0_op_res_80584;
            int64_t redout_tmp_84208 = defunc_0_op_res_80585;
            
            redout_81472 = redout_tmp_84207;
            redout_81473 = redout_tmp_84208;
        }
        x_80575 = redout_81472;
        x_80576 = redout_81473;
        defunc_0_reduce_res_73157 = x_80575;
        defunc_0_reduce_res_73158 = x_80576;
    } else {
        defunc_0_reduce_res_73157 = (int64_t) 0;
        defunc_0_reduce_res_73158 = (int64_t) 0;
    }
    
    bool bounds_invalid_upwards_73170 = slt64(smallest_pow_2_geq_than_res_73136, (int64_t) 0);
    bool valid_73171 = !bounds_invalid_upwards_73170;
    bool protect_assert_disj_73172 = cond_73130 || valid_73171;
    bool range_valid_c_73173;
    
    if (!protect_assert_disj_73172) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) smallest_pow_2_geq_than_res_73136, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #3  /prelude/functional.fut:9:44-45\n   #4  test_dbscan_plus.fut:14:23-19:30\n   #5  test_dbscan_plus.fut:13:1-20:55\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool bounds_invalid_upwards_73174 = slt64(block_sizze_73146, (int64_t) 0);
    bool valid_73175 = !bounds_invalid_upwards_73174;
    bool protect_assert_disj_73176 = cond_73130 || valid_73175;
    bool range_valid_c_73177;
    
    if (!protect_assert_disj_73176) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) block_sizze_73146, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  test_dbscan_plus.fut:14:23-19:30\n   #7  test_dbscan_plus.fut:13:1-20:55\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool cond_73178 = sle64(block_sizze_73146, (int64_t) 1);
    int64_t tmp_73179 = sub64(block_sizze_73146, (int64_t) 2);
    bool bounds_invalid_upwards_73180 = slt64(tmp_73179, (int64_t) 0);
    int64_t distance_73181 = add64((int64_t) 1, tmp_73179);
    bool valid_73182 = !bounds_invalid_upwards_73180;
    bool protect_assert_disj_73183 = cond_73178 || valid_73182;
    bool protect_assert_disj_73184 = cond_73130 || protect_assert_disj_73183;
    bool range_valid_c_73185;
    
    if (!protect_assert_disj_73184) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "...", (long long) tmp_73179, " is invalid.", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:166:19-28\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  test_dbscan_plus.fut:14:23-19:30\n   #7  test_dbscan_plus.fut:13:1-20:55\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool loop_cond_73186 = slt64(block_sizze_73146, flat_dim_73147);
    bool zzero_73187 = merge_block_sizze_73150 == (int64_t) 0;
    bool nonzzero_73188 = !zzero_73187;
    bool empty_slice_73189 = dz2081U_57123 == (int64_t) 0;
    bool zzero_leq_i_p_m_t_s_73190 = sle64((int64_t) 0, zs_lhs_73131);
    int64_t binop_x_82411 = (int64_t) 8 * smallest_pow_2_geq_than_res_73136;
    int64_t bytes_82412 = block_sizze_73146 * binop_x_82411;
    int64_t bytes_82425 = (int64_t) 8 * block_sizze_73146;
    
    if (cond_73130) {
        if (memblock_set(ctx, &ext_mem_82615, &mem_82410, "mem_82410") != 0)
            return 1;
    } else {
        if (mem_82413_cached_sizze_84582 < bytes_82412) {
            err = lexical_realloc(ctx, &mem_82413, &mem_82413_cached_sizze_84582, bytes_82412);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_82416_cached_sizze_84583 < bytes_82412) {
            err = lexical_realloc(ctx, &mem_82416, &mem_82416_cached_sizze_84583, bytes_82412);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_82426_cached_sizze_84584 < bytes_82425) {
            err = lexical_realloc(ctx, &mem_82426, &mem_82426_cached_sizze_84584, bytes_82425);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_82428_cached_sizze_84585 < bytes_82425) {
            err = lexical_realloc(ctx, &mem_82428, &mem_82428_cached_sizze_84585, bytes_82425);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t i_81486 = 0; i_81486 < smallest_pow_2_geq_than_res_73136; i_81486++) {
            int64_t zp_lhs_73197 = mul64(block_sizze_73146, i_81486);
            
            for (int64_t i_81479 = 0; i_81479 < block_sizze_73146; i_81479++) {
                int64_t k_73201 = add64(zp_lhs_73197, i_81479);
                bool cond_73202 = slt64(k_73201, dz2081U_57123);
                int64_t lifted_lambda_res_73203;
                int64_t lifted_lambda_res_73204;
                
                if (cond_73202) {
                    bool x_80590 = sle64((int64_t) 0, k_73201);
                    bool bounds_check_80591 = cond_73202 && x_80590;
                    bool index_certs_80592;
                    
                    if (!bounds_check_80591) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) k_73201, "] out of bounds for array of shape [", (long long) dz2081U_57123, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:246:26-30\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t lifted_lambda_res_t_res_80593 = ((int64_t *) mem_82402)[k_73201];
                    
                    lifted_lambda_res_73203 = lifted_lambda_res_t_res_80593;
                    lifted_lambda_res_73204 = k_73201;
                } else {
                    lifted_lambda_res_73203 = defunc_0_reduce_res_73157;
                    lifted_lambda_res_73204 = defunc_0_reduce_res_73158;
                }
                ((int64_t *) mem_82426)[i_81479] = lifted_lambda_res_73203;
                ((int64_t *) mem_82428)[i_81479] = lifted_lambda_res_73204;
            }
            if (!cond_73178) {
                for (int64_t i_73213 = 0; i_73213 < distance_73181; i_73213++) {
                    int64_t gt_arg1_73216 = add64((int64_t) 1, i_73213);
                    bool x_73217 = sle64((int64_t) 0, gt_arg1_73216);
                    bool y_73218 = slt64(gt_arg1_73216, block_sizze_73146);
                    bool bounds_check_73219 = x_73217 && y_73218;
                    bool index_certs_73220;
                    
                    if (!bounds_check_73219) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) gt_arg1_73216, "] out of bounds for array of shape [", (long long) block_sizze_73146, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:169:31-37\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  test_dbscan_plus.fut:14:23-19:30\n   #7  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    bool y_73222 = slt64(i_73213, block_sizze_73146);
                    bool index_certs_73223;
                    
                    if (!y_73222) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_73213, "] out of bounds for array of shape [", (long long) block_sizze_73146, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:169:26-30\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  test_dbscan_plus.fut:14:23-19:30\n   #7  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t gt_arg1_73221 = ((int64_t *) mem_82426)[gt_arg1_73216];
                    int64_t gt_arg0_73224 = ((int64_t *) mem_82426)[i_73213];
                    bool defunc_0_leq_res_73225 = sle64(gt_arg0_73224, gt_arg1_73221);
                    bool defunc_0_lifted_gt_res_73226 = !defunc_0_leq_res_73225;
                    bool s_73227;
                    int64_t s_73230;
                    bool loop_while_73231;
                    int64_t j_73234;
                    
                    loop_while_73231 = defunc_0_lifted_gt_res_73226;
                    j_73234 = i_73213;
                    while (loop_while_73231) {
                        bool x_73235 = sle64((int64_t) 0, j_73234);
                        bool y_73236 = slt64(j_73234, block_sizze_73146);
                        bool bounds_check_73237 = x_73235 && y_73236;
                        bool index_certs_73238;
                        
                        if (!bounds_check_73237) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_73234, "] out of bounds for array of shape [", (long long) block_sizze_73146, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:170:24-28\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  test_dbscan_plus.fut:14:23-19:30\n   #7  test_dbscan_plus.fut:13:1-20:55\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t copy_arg0_73241 = add64((int64_t) 1, j_73234);
                        bool x_73242 = sle64((int64_t) 0, copy_arg0_73241);
                        bool y_73243 = slt64(copy_arg0_73241, block_sizze_73146);
                        bool bounds_check_73244 = x_73242 && y_73243;
                        bool index_certs_73245;
                        
                        if (!bounds_check_73244) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) copy_arg0_73241, "] out of bounds for array of shape [", (long long) block_sizze_73146, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:171:35-41\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  test_dbscan_plus.fut:14:23-19:30\n   #7  test_dbscan_plus.fut:13:1-20:55\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t copy_arg0_73239 = ((int64_t *) mem_82426)[j_73234];
                        int64_t copy_arg0_73240 = ((int64_t *) mem_82428)[j_73234];
                        int64_t copy_arg0_73246 = ((int64_t *) mem_82426)[copy_arg0_73241];
                        int64_t copy_arg0_73247 = ((int64_t *) mem_82428)[copy_arg0_73241];
                        
                        ((int64_t *) mem_82426)[j_73234] = copy_arg0_73246;
                        ((int64_t *) mem_82428)[j_73234] = copy_arg0_73247;
                        ((int64_t *) mem_82426)[copy_arg0_73241] = copy_arg0_73239;
                        ((int64_t *) mem_82428)[copy_arg0_73241] = copy_arg0_73240;
                        
                        int64_t tmp_73252 = sub64(j_73234, (int64_t) 1);
                        bool cond_73253 = sle64((int64_t) 0, tmp_73252);
                        bool loop_cond_73254;
                        
                        if (cond_73253) {
                            bool y_80595 = slt64(tmp_73252, block_sizze_73146);
                            bool bounds_check_80596 = cond_73253 && y_80595;
                            bool index_certs_80597;
                            
                            if (!bounds_check_80596) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_73252, "] out of bounds for array of shape [", (long long) block_sizze_73146, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:169:26-30\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  test_dbscan_plus.fut:14:23-19:30\n   #7  test_dbscan_plus.fut:13:1-20:55\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t gt_arg1_80594 = ((int64_t *) mem_82426)[j_73234];
                            int64_t gt_arg0_80598 = ((int64_t *) mem_82426)[tmp_73252];
                            bool defunc_0_leq_res_80599 = sle64(gt_arg0_80598, gt_arg1_80594);
                            bool defunc_0_lifted_gt_res_80600 = !defunc_0_leq_res_80599;
                            
                            loop_cond_73254 = defunc_0_lifted_gt_res_80600;
                        } else {
                            loop_cond_73254 = 0;
                        }
                        
                        bool loop_while_tmp_84215 = loop_cond_73254;
                        int64_t j_tmp_84218 = tmp_73252;
                        
                        loop_while_73231 = loop_while_tmp_84215;
                        j_73234 = j_tmp_84218;
                    }
                    s_73227 = loop_while_73231;
                    s_73230 = j_73234;
                }
            }
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_82413, i_81486 * block_sizze_73146, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82426, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {block_sizze_73146});
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_82416, i_81486 * block_sizze_73146, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82428, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {block_sizze_73146});
        }
        if (memblock_alloc(ctx, &mem_82481, bytes_82412, "mem_82481")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 2, (uint64_t *) mem_82481.mem, (int64_t) 0, (int64_t []) {block_sizze_73146, (int64_t) 1}, (uint64_t *) mem_82413, (int64_t) 0, (int64_t []) {block_sizze_73146, (int64_t) 1}, (int64_t []) {smallest_pow_2_geq_than_res_73136, block_sizze_73146});
        if (memblock_alloc(ctx, &mem_82485, bytes_82412, "mem_82485")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 2, (uint64_t *) mem_82485.mem, (int64_t) 0, (int64_t []) {block_sizze_73146, (int64_t) 1}, (uint64_t *) mem_82416, (int64_t) 0, (int64_t []) {block_sizze_73146, (int64_t) 1}, (int64_t []) {smallest_pow_2_geq_than_res_73136, block_sizze_73146});
        
        int64_t data_73264;
        bool data_73265;
        int64_t data_73268;
        int64_t loop_dz2084Uz2082U_73269;
        bool loop_while_73270;
        int64_t stride_73273;
        
        if (memblock_set(ctx, &mem_param_82489, &mem_82481, "mem_82481") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82492, &mem_82485, "mem_82485") != 0)
            return 1;
        loop_dz2084Uz2082U_73269 = flat_dim_73147;
        loop_while_73270 = loop_cond_73186;
        stride_73273 = block_sizze_73146;
        while (loop_while_73270) {
            int64_t next_stride_73274 = mul64((int64_t) 2, stride_73273);
            bool zzero_73275 = next_stride_73274 == (int64_t) 0;
            bool nonzzero_73276 = !zzero_73275;
            bool nonzzero_cert_73277;
            
            if (!nonzzero_73276) {
                set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:254:33-46\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #2  /prelude/functional.fut:9:44-45\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t num_merges_73278 = sdiv64(flat_dim_73147, next_stride_73274);
            bool bounds_invalid_upwards_73279 = slt64(num_merges_73278, (int64_t) 0);
            bool valid_73280 = !bounds_invalid_upwards_73279;
            bool range_valid_c_73281;
            
            if (!valid_73280) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) num_merges_73278, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #3  /prelude/functional.fut:9:44-45\n   #4  test_dbscan_plus.fut:14:23-19:30\n   #5  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t m_73283 = sub64(next_stride_73274, (int64_t) 1);
            bool zzero_leq_i_p_m_t_s_73284 = sle64((int64_t) 0, m_73283);
            bool cond_73286 = slt64(next_stride_73274, (int64_t) 10);
            bool protect_assert_disj_73287 = nonzzero_73188 || cond_73286;
            bool nonzzero_cert_73288;
            
            if (!protect_assert_disj_73287) {
                set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:147:30-42\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  test_dbscan_plus.fut:14:23-19:30\n   #7  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t zeze_lhs_73289 = smod_safe64(next_stride_73274, merge_block_sizze_73150);
            bool num_blocks_73290 = zeze_lhs_73289 == (int64_t) 0;
            bool protect_assert_disj_73291 = cond_73286 || num_blocks_73290;
            bool assert_c_73292;
            
            if (!protect_assert_disj_73291) {
                set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "Assertion is false: (n % block_size == 0)", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:147:20-65\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  test_dbscan_plus.fut:14:23-19:30\n   #7  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t num_blocks_73293 = sdiv_safe64(next_stride_73274, merge_block_sizze_73150);
            int64_t dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_73294 = add64((int64_t) 1, num_blocks_73293);
            int64_t j_m_i_73295 = sub64(next_stride_73274, stride_73273);
            int64_t flat_dim_73296 = merge_block_sizze_73150 * num_blocks_73293;
            bool bounds_invalid_upwards_73297 = slt64(dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_73294, (int64_t) 0);
            bool empty_slice_73298 = j_m_i_73295 == (int64_t) 0;
            int64_t m_73299 = sub64(j_m_i_73295, (int64_t) 1);
            bool zzero_lte_i_73300 = sle64((int64_t) 0, stride_73273);
            bool i_lte_j_73301 = sle64(stride_73273, next_stride_73274);
            bool empty_slice_73302 = stride_73273 == (int64_t) 0;
            int64_t m_73303 = sub64(stride_73273, (int64_t) 1);
            bool bounds_invalid_upwards_73304 = slt64(num_blocks_73293, (int64_t) 0);
            bool cond_73305 = slt64((int64_t) 0, stride_73273);
            bool valid_73306 = !bounds_invalid_upwards_73297;
            int64_t i_p_m_t_s_73307 = add64(stride_73273, m_73299);
            bool zzero_leq_i_p_m_t_s_73308 = sle64((int64_t) 0, m_73303);
            bool i_p_m_t_s_leq_w_73309 = slt64(m_73303, next_stride_73274);
            bool valid_73310 = !bounds_invalid_upwards_73304;
            bool i_p_m_t_s_leq_w_73311 = slt64(m_73283, flat_dim_73296);
            bool y_73312 = slt64((int64_t) 0, j_m_i_73295);
            bool protect_assert_disj_73313 = cond_73286 || valid_73306;
            bool range_valid_c_73314;
            
            if (!protect_assert_disj_73313) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_73294, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  test_dbscan_plus.fut:14:23-19:30\n   #9  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool zzero_leq_i_p_m_t_s_73315 = sle64((int64_t) 0, i_p_m_t_s_73307);
            bool i_p_m_t_s_leq_w_73316 = slt64(i_p_m_t_s_73307, next_stride_73274);
            bool y_73317 = zzero_leq_i_p_m_t_s_73308 && i_p_m_t_s_leq_w_73309;
            bool protect_assert_disj_73318 = cond_73286 || valid_73310;
            bool range_valid_c_73319;
            
            if (!protect_assert_disj_73318) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) num_blocks_73293, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool y_73320 = zzero_leq_i_p_m_t_s_73284 && i_p_m_t_s_leq_w_73311;
            bool y_73321 = zzero_lte_i_73300 && i_p_m_t_s_leq_w_73316;
            bool forwards_ok_73322 = zzero_lte_i_73300 && y_73317;
            bool ok_or_empty_73323 = zzero_73275 || y_73320;
            bool protect_assert_disj_73324 = cond_73305 || y_73312;
            bool y_73325 = zzero_leq_i_p_m_t_s_73315 && y_73321;
            bool ok_or_empty_73326 = empty_slice_73302 || forwards_ok_73322;
            bool protect_assert_disj_73327 = cond_73286 || ok_or_empty_73323;
            bool index_certs_73328;
            
            if (!protect_assert_disj_73327) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) next_stride_73274, "] out of bounds for array of shape [", (long long) flat_dim_73296, "].", "-> #0  /prelude/array.fut:46:45-51\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:155:3-46\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool loop_not_taken_73329 = !cond_73286;
            bool protect_assert_disj_73330 = protect_assert_disj_73324 || loop_not_taken_73329;
            bool index_certs_73331;
            
            if (!protect_assert_disj_73330) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) j_m_i_73295, "].", "-> #0  /prelude/array.fut:22:29-33\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:100:48-54\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  test_dbscan_plus.fut:14:23-19:30\n   #9  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool forwards_ok_73332 = i_lte_j_73301 && y_73325;
            bool protect_assert_disj_73333 = cond_73286 || ok_or_empty_73326;
            bool index_certs_73334;
            
            if (!protect_assert_disj_73333) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) stride_73273, "] out of bounds for array of shape [", (long long) next_stride_73274, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:52-59\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  test_dbscan_plus.fut:14:23-19:30\n   #10 test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool protect_assert_disj_73335 = ok_or_empty_73326 || loop_not_taken_73329;
            bool index_certs_73336;
            
            if (!protect_assert_disj_73335) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) stride_73273, "] out of bounds for array of shape [", (long long) next_stride_73274, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:26-33\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  test_dbscan_plus.fut:14:23-19:30\n   #7  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool ok_or_empty_73337 = empty_slice_73298 || forwards_ok_73332;
            bool protect_assert_disj_73338 = cond_73286 || ok_or_empty_73337;
            bool index_certs_73339;
            
            if (!protect_assert_disj_73338) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) stride_73273, ":] out of bounds for array of shape [", (long long) next_stride_73274, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:60-67\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  test_dbscan_plus.fut:14:23-19:30\n   #10 test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool protect_assert_disj_73340 = loop_not_taken_73329 || ok_or_empty_73337;
            bool index_certs_73341;
            
            if (!protect_assert_disj_73340) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) stride_73273, ":] out of bounds for array of shape [", (long long) next_stride_73274, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:34-41\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  test_dbscan_plus.fut:14:23-19:30\n   #7  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t binop_x_82493 = (int64_t) 8 * num_merges_73278;
            int64_t bytes_82494 = next_stride_73274 * binop_x_82493;
            bool i_lte_j_73285 = sle64((int64_t) 0, next_stride_73274);
            
            if (mem_82495_cached_sizze_84586 < bytes_82494) {
                err = lexical_realloc(ctx, &mem_82495, &mem_82495_cached_sizze_84586, bytes_82494);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            if (mem_82498_cached_sizze_84587 < bytes_82494) {
                err = lexical_realloc(ctx, &mem_82498, &mem_82498_cached_sizze_84587, bytes_82494);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            
            int64_t bytes_82507 = (int64_t) 8 * dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_73294;
            int64_t binop_x_82523 = (int64_t) 8 * num_blocks_73293;
            int64_t bytes_82524 = merge_block_sizze_73150 * binop_x_82523;
            int64_t bytes_82569 = (int64_t) 16 * stride_73273;
            
            for (int64_t i_81507 = 0; i_81507 < num_merges_73278; i_81507++) {
                int64_t start_73345 = mul64(next_stride_73274, i_81507);
                int64_t j_m_i_73346 = sub64(loop_dz2084Uz2082U_73269, start_73345);
                bool empty_slice_73347 = j_m_i_73346 == (int64_t) 0;
                int64_t m_73348 = sub64(j_m_i_73346, (int64_t) 1);
                int64_t i_p_m_t_s_73349 = add64(start_73345, m_73348);
                bool zzero_leq_i_p_m_t_s_73350 = sle64((int64_t) 0, i_p_m_t_s_73349);
                bool i_p_m_t_s_leq_w_73351 = slt64(i_p_m_t_s_73349, loop_dz2084Uz2082U_73269);
                bool zzero_lte_i_73352 = sle64((int64_t) 0, start_73345);
                bool i_lte_j_73353 = sle64(start_73345, loop_dz2084Uz2082U_73269);
                bool y_73354 = i_p_m_t_s_leq_w_73351 && zzero_lte_i_73352;
                bool y_73355 = zzero_leq_i_p_m_t_s_73350 && y_73354;
                bool forwards_ok_73356 = i_lte_j_73353 && y_73355;
                bool ok_or_empty_73357 = empty_slice_73347 || forwards_ok_73356;
                bool index_certs_73358;
                
                if (!ok_or_empty_73357) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) start_73345, ":] out of bounds for array of shape [", (long long) loop_dz2084Uz2082U_73269, "].", "-> #0  /prelude/array.fut:52:49-54\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:258:41-56\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  test_dbscan_plus.fut:14:23-19:30\n   #7  test_dbscan_plus.fut:13:1-20:55\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                bool i_p_m_t_s_leq_w_73359 = slt64(m_73283, j_m_i_73346);
                bool y_73360 = zzero_leq_i_p_m_t_s_73284 && i_p_m_t_s_leq_w_73359;
                bool forwards_ok_73361 = i_lte_j_73285 && y_73360;
                bool ok_or_empty_73362 = zzero_73275 || forwards_ok_73361;
                bool index_certs_73363;
                
                if (!ok_or_empty_73362) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) next_stride_73274, "] out of bounds for array of shape [", (long long) j_m_i_73346, "].", "-> #0  /prelude/array.fut:46:45-51\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:258:23-56\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  test_dbscan_plus.fut:14:23-19:30\n   #7  test_dbscan_plus.fut:13:1-20:55\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                if (cond_73286) {
                    int64_t dummy_80626;
                    int64_t dummy_80627;
                    
                    if (cond_73305) {
                        int64_t head_res_80628 = ((int64_t *) mem_param_82489.mem)[start_73345];
                        int64_t head_res_80629 = ((int64_t *) mem_param_82492.mem)[start_73345];
                        
                        dummy_80626 = head_res_80628;
                        dummy_80627 = head_res_80629;
                    } else {
                        int64_t slice_80630 = stride_73273 + start_73345;
                        int64_t head_res_80631 = ((int64_t *) mem_param_82489.mem)[slice_80630];
                        int64_t head_res_80632 = ((int64_t *) mem_param_82492.mem)[slice_80630];
                        
                        dummy_80626 = head_res_80631;
                        dummy_80627 = head_res_80632;
                    }
                    if (memblock_alloc(ctx, &mem_82570, bytes_82569, "mem_82570")) {
                        err = 1;
                        goto cleanup;
                    }
                    for (int64_t nest_i_84228 = 0; nest_i_84228 < next_stride_73274; nest_i_84228++) {
                        ((int64_t *) mem_82570.mem)[nest_i_84228] = dummy_80626;
                    }
                    if (memblock_alloc(ctx, &mem_82572, bytes_82569, "mem_82572")) {
                        err = 1;
                        goto cleanup;
                    }
                    for (int64_t nest_i_84229 = 0; nest_i_84229 < next_stride_73274; nest_i_84229++) {
                        ((int64_t *) mem_82572.mem)[nest_i_84229] = dummy_80627;
                    }
                    
                    int64_t data_80635;
                    int64_t i_80639 = (int64_t) 0;
                    
                    for (int64_t k_80638 = 0; k_80638 < next_stride_73274; k_80638++) {
                        int64_t j_80642 = sub64(k_80638, i_80639);
                        bool cond_80643 = j_80642 == j_m_i_73295;
                        bool cond_80644;
                        
                        if (cond_80643) {
                            cond_80644 = 1;
                        } else {
                            bool cond_80645 = slt64(i_80639, stride_73273);
                            bool cond_f_res_80646;
                            
                            if (cond_80645) {
                                bool x_80647 = sle64((int64_t) 0, j_80642);
                                bool y_80648 = slt64(j_80642, j_m_i_73295);
                                bool bounds_check_80649 = x_80647 && y_80648;
                                bool index_certs_80650;
                                
                                if (!bounds_check_80649) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_80642, "] out of bounds for array of shape [", (long long) j_m_i_73295, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:55-59\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                bool x_80654 = sle64((int64_t) 0, i_80639);
                                bool bounds_check_80655 = cond_80645 && x_80654;
                                bool index_certs_80656;
                                
                                if (!bounds_check_80655) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_80639, "] out of bounds for array of shape [", (long long) stride_73273, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:50-54\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t slice_80651 = stride_73273 + j_80642;
                                int64_t slice_80652 = start_73345 + slice_80651;
                                int64_t leq_arg1_80653 = ((int64_t *) mem_param_82489.mem)[slice_80652];
                                int64_t slice_80657 = start_73345 + i_80639;
                                int64_t leq_arg0_80658 = ((int64_t *) mem_param_82489.mem)[slice_80657];
                                bool defunc_0_leq_res_80659 = sle64(leq_arg0_80658, leq_arg1_80653);
                                
                                cond_f_res_80646 = defunc_0_leq_res_80659;
                            } else {
                                cond_f_res_80646 = 0;
                            }
                            cond_80644 = cond_f_res_80646;
                        }
                        
                        int64_t loopres_80660;
                        int64_t loopres_80661;
                        int64_t loopres_80662;
                        
                        if (cond_80644) {
                            bool x_80664 = sle64((int64_t) 0, i_80639);
                            bool y_80665 = slt64(i_80639, stride_73273);
                            bool bounds_check_80666 = x_80664 && y_80665;
                            bool index_certs_80667;
                            
                            if (!bounds_check_80666) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_80639, "] out of bounds for array of shape [", (long long) stride_73273, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:106:17-21\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t tmp_80663 = add64((int64_t) 1, i_80639);
                            int64_t slice_80668 = start_73345 + i_80639;
                            int64_t tmp_80669 = ((int64_t *) mem_param_82489.mem)[slice_80668];
                            int64_t tmp_80670 = ((int64_t *) mem_param_82492.mem)[slice_80668];
                            
                            loopres_80660 = tmp_80663;
                            loopres_80661 = tmp_80669;
                            loopres_80662 = tmp_80670;
                        } else {
                            bool x_80671 = sle64((int64_t) 0, j_80642);
                            bool y_80672 = slt64(j_80642, j_m_i_73295);
                            bool bounds_check_80673 = x_80671 && y_80672;
                            bool index_certs_80674;
                            
                            if (!bounds_check_80673) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_80642, "] out of bounds for array of shape [", (long long) j_m_i_73295, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:108:15-19\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t slice_80675 = stride_73273 + j_80642;
                            int64_t slice_80676 = start_73345 + slice_80675;
                            int64_t tmp_80677 = ((int64_t *) mem_param_82489.mem)[slice_80676];
                            int64_t tmp_80678 = ((int64_t *) mem_param_82492.mem)[slice_80676];
                            
                            loopres_80660 = i_80639;
                            loopres_80661 = tmp_80677;
                            loopres_80662 = tmp_80678;
                        }
                        ((int64_t *) mem_82570.mem)[k_80638] = loopres_80661;
                        ((int64_t *) mem_82572.mem)[k_80638] = loopres_80662;
                        
                        int64_t i_tmp_84230 = loopres_80660;
                        
                        i_80639 = i_tmp_84230;
                    }
                    data_80635 = i_80639;
                    if (memblock_set(ctx, &ext_mem_82590, &mem_82570, "mem_82570") != 0)
                        return 1;
                    if (memblock_set(ctx, &ext_mem_82587, &mem_82572, "mem_82572") != 0)
                        return 1;
                } else {
                    if (mem_82508_cached_sizze_84588 < bytes_82507) {
                        err = lexical_realloc(ctx, &mem_82508, &mem_82508_cached_sizze_84588, bytes_82507);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    if (mem_82510_cached_sizze_84589 < bytes_82507) {
                        err = lexical_realloc(ctx, &mem_82510, &mem_82510_cached_sizze_84589, bytes_82507);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    for (int64_t i_81493 = 0; i_81493 < dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_73294; i_81493++) {
                        int64_t split_count_arg3_73425 = mul64(merge_block_sizze_73150, i_81493);
                        int64_t defunc_0_split_count_res_73426;
                        int64_t defunc_0_split_count_res_73427;
                        int64_t defunc_0_split_count_res_73428;
                        int64_t defunc_0_split_count_res_73429;
                        int64_t defunc_0_split_count_res_73430;
                        
                        if (futrts_lifted_normalizze_11978(ctx, &defunc_0_split_count_res_73426, &defunc_0_split_count_res_73427, &defunc_0_split_count_res_73428, &defunc_0_split_count_res_73429, &defunc_0_split_count_res_73430, (int64_t) 0, stride_73273, (int64_t) 0, j_m_i_73295, split_count_arg3_73425) != 0) {
                            err = 1;
                            goto cleanup;
                        }
                        
                        bool loop_cond_73431 = slt64((int64_t) 0, defunc_0_split_count_res_73430);
                        bool defunc_0_split_count_res_73432;
                        int64_t defunc_0_split_count_res_73433;
                        int64_t defunc_0_split_count_res_73434;
                        int64_t defunc_0_split_count_res_73435;
                        int64_t defunc_0_split_count_res_73436;
                        int64_t defunc_0_split_count_res_73437;
                        bool loop_while_73438;
                        int64_t ss_73439;
                        int64_t ss_73440;
                        int64_t tt_73441;
                        int64_t tt_73442;
                        int64_t count_73443;
                        
                        loop_while_73438 = loop_cond_73431;
                        ss_73439 = defunc_0_split_count_res_73426;
                        ss_73440 = defunc_0_split_count_res_73427;
                        tt_73441 = defunc_0_split_count_res_73428;
                        tt_73442 = defunc_0_split_count_res_73429;
                        count_73443 = defunc_0_split_count_res_73430;
                        while (loop_while_73438) {
                            int64_t zlze_lhs_73444 = sub64(ss_73440, ss_73439);
                            bool cond_73445 = sle64(zlze_lhs_73444, (int64_t) 0);
                            int64_t defunc_0_lifted_step_res_73446;
                            int64_t defunc_0_lifted_step_res_73447;
                            int64_t defunc_0_lifted_step_res_73448;
                            int64_t defunc_0_lifted_step_res_73449;
                            
                            if (cond_73445) {
                                int64_t tmp_80681 = add64(tt_73441, count_73443);
                                
                                defunc_0_lifted_step_res_73446 = ss_73439;
                                defunc_0_lifted_step_res_73447 = tmp_80681;
                                defunc_0_lifted_step_res_73448 = tt_73442;
                                defunc_0_lifted_step_res_73449 = (int64_t) 0;
                            } else {
                                int64_t zlze_lhs_73451 = sub64(tt_73442, tt_73441);
                                bool cond_73452 = sle64(zlze_lhs_73451, (int64_t) 0);
                                int64_t defunc_0_lifted_step_res_f_res_73453;
                                int64_t defunc_0_lifted_step_res_f_res_73454;
                                int64_t defunc_0_lifted_step_res_f_res_73455;
                                int64_t defunc_0_lifted_step_res_f_res_73456;
                                
                                if (cond_73452) {
                                    int64_t tmp_80682 = add64(ss_73439, count_73443);
                                    
                                    defunc_0_lifted_step_res_f_res_73453 = tmp_80682;
                                    defunc_0_lifted_step_res_f_res_73454 = tt_73441;
                                    defunc_0_lifted_step_res_f_res_73455 = tt_73442;
                                    defunc_0_lifted_step_res_f_res_73456 = (int64_t) 0;
                                } else {
                                    bool cond_73458 = count_73443 == (int64_t) 1;
                                    int64_t defunc_0_lifted_step_res_f_res_f_res_73459;
                                    int64_t defunc_0_lifted_step_res_f_res_f_res_73460;
                                    int64_t defunc_0_lifted_step_res_f_res_f_res_73461;
                                    int64_t defunc_0_lifted_step_res_f_res_f_res_73462;
                                    
                                    if (cond_73458) {
                                        bool x_80684 = sle64((int64_t) 0, ss_73439);
                                        bool y_80685 = slt64(ss_73439, stride_73273);
                                        bool bounds_check_80686 = x_80684 && y_80685;
                                        bool index_certs_80687;
                                        
                                        if (!bounds_check_80686) {
                                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) ss_73439, "] out of bounds for array of shape [", (long long) stride_73273, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:53:21-27\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 test_dbscan_plus.fut:14:23-19:30\n   #12 test_dbscan_plus.fut:13:1-20:55\n"));
                                            err = FUTHARK_PROGRAM_ERROR;
                                            goto cleanup;
                                        }
                                        
                                        bool x_80690 = sle64((int64_t) 0, tt_73441);
                                        bool y_80691 = slt64(tt_73441, j_m_i_73295);
                                        bool bounds_check_80692 = x_80690 && y_80691;
                                        bool index_certs_80693;
                                        
                                        if (!bounds_check_80692) {
                                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tt_73441, "] out of bounds for array of shape [", (long long) j_m_i_73295, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:53:14-20\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 test_dbscan_plus.fut:14:23-19:30\n   #12 test_dbscan_plus.fut:13:1-20:55\n"));
                                            err = FUTHARK_PROGRAM_ERROR;
                                            goto cleanup;
                                        }
                                        
                                        int64_t slice_80688 = start_73345 + ss_73439;
                                        int64_t leq_arg1_80689 = ((int64_t *) mem_param_82489.mem)[slice_80688];
                                        int64_t slice_80694 = stride_73273 + tt_73441;
                                        int64_t slice_80695 = start_73345 + slice_80694;
                                        int64_t leq_arg0_80696 = ((int64_t *) mem_param_82489.mem)[slice_80695];
                                        bool defunc_0_leq_res_80697 = sle64(leq_arg0_80696, leq_arg1_80689);
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_t_res_80698;
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_t_res_80699;
                                        
                                        if (defunc_0_leq_res_80697) {
                                            int64_t tmp_80700 = add64((int64_t) 1, tt_73441);
                                            
                                            defunc_0_lifted_step_res_f_res_f_res_t_res_80698 = ss_73439;
                                            defunc_0_lifted_step_res_f_res_f_res_t_res_80699 = tmp_80700;
                                        } else {
                                            int64_t tmp_80701 = add64((int64_t) 1, ss_73439);
                                            
                                            defunc_0_lifted_step_res_f_res_f_res_t_res_80698 = tmp_80701;
                                            defunc_0_lifted_step_res_f_res_f_res_t_res_80699 = tt_73441;
                                        }
                                        defunc_0_lifted_step_res_f_res_f_res_73459 = defunc_0_lifted_step_res_f_res_f_res_t_res_80698;
                                        defunc_0_lifted_step_res_f_res_f_res_73460 = defunc_0_lifted_step_res_f_res_f_res_t_res_80699;
                                        defunc_0_lifted_step_res_f_res_f_res_73461 = tt_73442;
                                        defunc_0_lifted_step_res_f_res_f_res_73462 = (int64_t) 0;
                                    } else {
                                        int64_t m_73481 = sdiv64(count_73443, (int64_t) 2);
                                        int64_t n_73482 = sub64(count_73443, m_73481);
                                        bool cond_73483 = n_73482 == (int64_t) 0;
                                        int64_t zgze_lhs_73484 = add64(ss_73439, m_73481);
                                        bool cond_f_res_73485 = sle64(ss_73440, zgze_lhs_73484);
                                        bool x_73486 = !cond_73483;
                                        bool y_73487 = cond_f_res_73485 && x_73486;
                                        bool cond_73488 = cond_73483 || y_73487;
                                        bool leq_y_x_73489;
                                        
                                        if (cond_73488) {
                                            leq_y_x_73489 = 1;
                                        } else {
                                            bool x_73490 = sle64((int64_t) 0, zgze_lhs_73484);
                                            bool y_73491 = slt64(zgze_lhs_73484, stride_73273);
                                            bool bounds_check_73492 = x_73490 && y_73491;
                                            bool index_certs_73493;
                                            
                                            if (!bounds_check_73492) {
                                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) zgze_lhs_73484, "] out of bounds for array of shape [", (long long) stride_73273, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:73:22-30\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 test_dbscan_plus.fut:14:23-19:30\n   #12 test_dbscan_plus.fut:13:1-20:55\n"));
                                                err = FUTHARK_PROGRAM_ERROR;
                                                goto cleanup;
                                            }
                                            
                                            int64_t zm_lhs_73496 = add64(tt_73441, n_73482);
                                            int64_t leq_arg0_73497 = sub64(zm_lhs_73496, (int64_t) 1);
                                            bool x_73498 = sle64((int64_t) 0, leq_arg0_73497);
                                            bool y_73499 = slt64(leq_arg0_73497, j_m_i_73295);
                                            bool bounds_check_73500 = x_73498 && y_73499;
                                            bool index_certs_73501;
                                            
                                            if (!bounds_check_73500) {
                                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) leq_arg0_73497, "] out of bounds for array of shape [", (long long) j_m_i_73295, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:73:11-21\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 test_dbscan_plus.fut:14:23-19:30\n   #12 test_dbscan_plus.fut:13:1-20:55\n"));
                                                err = FUTHARK_PROGRAM_ERROR;
                                                goto cleanup;
                                            }
                                            
                                            int64_t slice_73494 = start_73345 + zgze_lhs_73484;
                                            int64_t leq_arg1_73495 = ((int64_t *) mem_param_82489.mem)[slice_73494];
                                            int64_t slice_73502 = stride_73273 + leq_arg0_73497;
                                            int64_t slice_73503 = start_73345 + slice_73502;
                                            int64_t leq_arg0_73504 = ((int64_t *) mem_param_82489.mem)[slice_73503];
                                            bool defunc_0_leq_res_73505 = sle64(leq_arg0_73504, leq_arg1_73495);
                                            
                                            leq_y_x_73489 = defunc_0_leq_res_73505;
                                        }
                                        
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_f_res_73506;
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_f_res_73507;
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_f_res_73508;
                                        
                                        if (leq_y_x_73489) {
                                            int64_t tmp_80702 = add64(tt_73441, n_73482);
                                            int64_t tmp_80703 = sub64(count_73443, n_73482);
                                            
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_73506 = tmp_80702;
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_73507 = tt_73442;
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_73508 = tmp_80703;
                                        } else {
                                            int64_t tmp_73511 = add64(tt_73441, n_73482);
                                            
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_73506 = tt_73441;
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_73507 = tmp_73511;
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_73508 = count_73443;
                                        }
                                        defunc_0_lifted_step_res_f_res_f_res_73459 = ss_73439;
                                        defunc_0_lifted_step_res_f_res_f_res_73460 = defunc_0_lifted_step_res_f_res_f_res_f_res_73506;
                                        defunc_0_lifted_step_res_f_res_f_res_73461 = defunc_0_lifted_step_res_f_res_f_res_f_res_73507;
                                        defunc_0_lifted_step_res_f_res_f_res_73462 = defunc_0_lifted_step_res_f_res_f_res_f_res_73508;
                                    }
                                    defunc_0_lifted_step_res_f_res_73453 = defunc_0_lifted_step_res_f_res_f_res_73459;
                                    defunc_0_lifted_step_res_f_res_73454 = defunc_0_lifted_step_res_f_res_f_res_73460;
                                    defunc_0_lifted_step_res_f_res_73455 = defunc_0_lifted_step_res_f_res_f_res_73461;
                                    defunc_0_lifted_step_res_f_res_73456 = defunc_0_lifted_step_res_f_res_f_res_73462;
                                }
                                defunc_0_lifted_step_res_73446 = defunc_0_lifted_step_res_f_res_73453;
                                defunc_0_lifted_step_res_73447 = defunc_0_lifted_step_res_f_res_73454;
                                defunc_0_lifted_step_res_73448 = defunc_0_lifted_step_res_f_res_73455;
                                defunc_0_lifted_step_res_73449 = defunc_0_lifted_step_res_f_res_73456;
                            }
                            
                            int64_t loopres_73512;
                            int64_t loopres_73513;
                            int64_t loopres_73514;
                            int64_t loopres_73515;
                            int64_t loopres_73516;
                            
                            if (futrts_lifted_normalizze_11978(ctx, &loopres_73512, &loopres_73513, &loopres_73514, &loopres_73515, &loopres_73516, defunc_0_lifted_step_res_73446, ss_73440, defunc_0_lifted_step_res_73447, defunc_0_lifted_step_res_73448, defunc_0_lifted_step_res_73449) != 0) {
                                err = 1;
                                goto cleanup;
                            }
                            
                            bool loop_cond_73517 = slt64((int64_t) 0, loopres_73516);
                            bool loop_while_tmp_84235 = loop_cond_73517;
                            int64_t ss_tmp_84236 = loopres_73512;
                            int64_t ss_tmp_84237 = loopres_73513;
                            int64_t tt_tmp_84238 = loopres_73514;
                            int64_t tt_tmp_84239 = loopres_73515;
                            int64_t count_tmp_84240 = loopres_73516;
                            
                            loop_while_73438 = loop_while_tmp_84235;
                            ss_73439 = ss_tmp_84236;
                            ss_73440 = ss_tmp_84237;
                            tt_73441 = tt_tmp_84238;
                            tt_73442 = tt_tmp_84239;
                            count_73443 = count_tmp_84240;
                        }
                        defunc_0_split_count_res_73432 = loop_while_73438;
                        defunc_0_split_count_res_73433 = ss_73439;
                        defunc_0_split_count_res_73434 = ss_73440;
                        defunc_0_split_count_res_73435 = tt_73441;
                        defunc_0_split_count_res_73436 = tt_73442;
                        defunc_0_split_count_res_73437 = count_73443;
                        ((int64_t *) mem_82508)[i_81493] = defunc_0_split_count_res_73433;
                        ((int64_t *) mem_82510)[i_81493] = defunc_0_split_count_res_73435;
                    }
                    if (mem_82525_cached_sizze_84590 < bytes_82524) {
                        err = lexical_realloc(ctx, &mem_82525, &mem_82525_cached_sizze_84590, bytes_82524);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    if (mem_82528_cached_sizze_84591 < bytes_82524) {
                        err = lexical_realloc(ctx, &mem_82528, &mem_82528_cached_sizze_84591, bytes_82524);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    for (int64_t i_81500 = 0; i_81500 < num_blocks_73293; i_81500++) {
                        bool y_73523 = slt64(i_81500, dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_73294);
                        bool index_certs_73525;
                        
                        if (!y_73523) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_81500, "] out of bounds for array of shape [", (long long) dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_73294, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:151:22-34\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  test_dbscan_plus.fut:14:23-19:30\n   #9  test_dbscan_plus.fut:13:1-20:55\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t defunc_0_f_res_73526 = ((int64_t *) mem_82508)[i_81500];
                        int64_t defunc_0_f_res_73527 = ((int64_t *) mem_82510)[i_81500];
                        int64_t tmp_73528 = add64((int64_t) 1, i_81500);
                        bool x_73529 = sle64((int64_t) 0, tmp_73528);
                        bool y_73530 = slt64(tmp_73528, dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_73294);
                        bool bounds_check_73531 = x_73529 && y_73530;
                        bool index_certs_73532;
                        
                        if (!bounds_check_73531) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_73528, "] out of bounds for array of shape [", (long long) dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_73294, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:152:22-36\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  test_dbscan_plus.fut:14:23-19:30\n   #9  test_dbscan_plus.fut:13:1-20:55\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t defunc_0_f_res_73533 = ((int64_t *) mem_82508)[tmp_73528];
                        int64_t defunc_0_f_res_73534 = ((int64_t *) mem_82510)[tmp_73528];
                        int64_t merge_sequential_arg2_73535 = add64(stride_73273, defunc_0_f_res_73527);
                        int64_t merge_sequential_arg2_73536 = add64(stride_73273, defunc_0_f_res_73534);
                        int64_t j_m_i_73537 = sub64(merge_sequential_arg2_73536, merge_sequential_arg2_73535);
                        bool empty_slice_73538 = j_m_i_73537 == (int64_t) 0;
                        int64_t m_73539 = sub64(j_m_i_73537, (int64_t) 1);
                        int64_t i_p_m_t_s_73540 = add64(merge_sequential_arg2_73535, m_73539);
                        bool zzero_leq_i_p_m_t_s_73541 = sle64((int64_t) 0, i_p_m_t_s_73540);
                        bool i_p_m_t_s_leq_w_73542 = slt64(i_p_m_t_s_73540, next_stride_73274);
                        bool zzero_lte_i_73543 = sle64((int64_t) 0, merge_sequential_arg2_73535);
                        bool i_lte_j_73544 = sle64(merge_sequential_arg2_73535, merge_sequential_arg2_73536);
                        bool y_73545 = i_p_m_t_s_leq_w_73542 && zzero_lte_i_73543;
                        bool y_73546 = zzero_leq_i_p_m_t_s_73541 && y_73545;
                        bool forwards_ok_73547 = i_lte_j_73544 && y_73546;
                        bool ok_or_empty_73548 = empty_slice_73538 || forwards_ok_73547;
                        bool index_certs_73549;
                        
                        if (!ok_or_empty_73548) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) merge_sequential_arg2_73535, ":", (long long) merge_sequential_arg2_73536, "] out of bounds for array of shape [", (long long) next_stride_73274, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:40-58\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  test_dbscan_plus.fut:14:23-19:30\n   #9  test_dbscan_plus.fut:13:1-20:55\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t j_m_i_73550 = sub64(defunc_0_f_res_73533, defunc_0_f_res_73526);
                        bool empty_slice_73551 = j_m_i_73550 == (int64_t) 0;
                        int64_t m_73552 = sub64(j_m_i_73550, (int64_t) 1);
                        int64_t i_p_m_t_s_73553 = add64(defunc_0_f_res_73526, m_73552);
                        bool zzero_leq_i_p_m_t_s_73554 = sle64((int64_t) 0, i_p_m_t_s_73553);
                        bool i_p_m_t_s_leq_w_73555 = slt64(i_p_m_t_s_73553, next_stride_73274);
                        bool zzero_lte_i_73556 = sle64((int64_t) 0, defunc_0_f_res_73526);
                        bool i_lte_j_73557 = sle64(defunc_0_f_res_73526, defunc_0_f_res_73533);
                        bool y_73558 = i_p_m_t_s_leq_w_73555 && zzero_lte_i_73556;
                        bool y_73559 = zzero_leq_i_p_m_t_s_73554 && y_73558;
                        bool forwards_ok_73560 = i_lte_j_73557 && y_73559;
                        bool ok_or_empty_73561 = empty_slice_73551 || forwards_ok_73560;
                        bool index_certs_73562;
                        
                        if (!ok_or_empty_73561) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) defunc_0_f_res_73526, ":", (long long) defunc_0_f_res_73533, "] out of bounds for array of shape [", (long long) next_stride_73274, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:29-39\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  test_dbscan_plus.fut:14:23-19:30\n   #9  test_dbscan_plus.fut:13:1-20:55\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        bool cond_73563 = slt64((int64_t) 0, j_m_i_73550);
                        int64_t dummy_73564;
                        int64_t dummy_73565;
                        
                        if (cond_73563) {
                            bool index_certs_80704;
                            
                            if (!cond_73563) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) j_m_i_73550, "].", "-> #0  /prelude/array.fut:22:29-33\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:100:36-42\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #6  /prelude/soacs.fut:59:9-11\n   #7  /prelude/array.fut:241:11-17\n   #8  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #9  /prelude/functional.fut:9:44-45\n   #10 test_dbscan_plus.fut:14:23-19:30\n   #11 test_dbscan_plus.fut:13:1-20:55\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t slice_80705 = start_73345 + defunc_0_f_res_73526;
                            int64_t head_res_80706 = ((int64_t *) mem_param_82489.mem)[slice_80705];
                            int64_t head_res_80707 = ((int64_t *) mem_param_82492.mem)[slice_80705];
                            
                            dummy_73564 = head_res_80706;
                            dummy_73565 = head_res_80707;
                        } else {
                            bool y_73570 = slt64((int64_t) 0, j_m_i_73537);
                            bool index_certs_73571;
                            
                            if (!y_73570) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) j_m_i_73537, "].", "-> #0  /prelude/array.fut:22:29-33\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:100:48-54\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #6  /prelude/soacs.fut:59:9-11\n   #7  /prelude/array.fut:241:11-17\n   #8  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #9  /prelude/functional.fut:9:44-45\n   #10 test_dbscan_plus.fut:14:23-19:30\n   #11 test_dbscan_plus.fut:13:1-20:55\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t slice_73572 = start_73345 + merge_sequential_arg2_73535;
                            int64_t head_res_73573 = ((int64_t *) mem_param_82489.mem)[slice_73572];
                            int64_t head_res_73574 = ((int64_t *) mem_param_82492.mem)[slice_73572];
                            
                            dummy_73564 = head_res_73573;
                            dummy_73565 = head_res_73574;
                        }
                        for (int64_t nest_i_84243 = 0; nest_i_84243 < merge_block_sizze_73150; nest_i_84243++) {
                            ((int64_t *) mem_82525)[i_81500 * merge_block_sizze_73150 + nest_i_84243] = dummy_73564;
                        }
                        for (int64_t nest_i_84244 = 0; nest_i_84244 < merge_block_sizze_73150; nest_i_84244++) {
                            ((int64_t *) mem_82528)[i_81500 * merge_block_sizze_73150 + nest_i_84244] = dummy_73565;
                        }
                        
                        int64_t data_73577;
                        int64_t i_73581 = (int64_t) 0;
                        
                        for (int64_t k_73580 = 0; k_73580 < merge_block_sizze_73150; k_73580++) {
                            int64_t j_73584 = sub64(k_73580, i_73581);
                            bool cond_73585 = j_73584 == j_m_i_73537;
                            bool cond_73586;
                            
                            if (cond_73585) {
                                cond_73586 = 1;
                            } else {
                                bool cond_73587 = slt64(i_73581, j_m_i_73550);
                                bool cond_f_res_73588;
                                
                                if (cond_73587) {
                                    bool x_80708 = sle64((int64_t) 0, j_73584);
                                    bool y_80709 = slt64(j_73584, j_m_i_73537);
                                    bool bounds_check_80710 = x_80708 && y_80709;
                                    bool index_certs_80711;
                                    
                                    if (!bounds_check_80710) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_73584, "] out of bounds for array of shape [", (long long) j_m_i_73537, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:55-59\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  test_dbscan_plus.fut:14:23-19:30\n   #10 test_dbscan_plus.fut:13:1-20:55\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    bool x_80715 = sle64((int64_t) 0, i_73581);
                                    bool bounds_check_80716 = cond_73587 && x_80715;
                                    bool index_certs_80717;
                                    
                                    if (!bounds_check_80716) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_73581, "] out of bounds for array of shape [", (long long) j_m_i_73550, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:50-54\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  test_dbscan_plus.fut:14:23-19:30\n   #10 test_dbscan_plus.fut:13:1-20:55\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    int64_t slice_80712 = merge_sequential_arg2_73535 + j_73584;
                                    int64_t slice_80713 = start_73345 + slice_80712;
                                    int64_t leq_arg1_80714 = ((int64_t *) mem_param_82489.mem)[slice_80713];
                                    int64_t slice_80718 = defunc_0_f_res_73526 + i_73581;
                                    int64_t slice_80719 = start_73345 + slice_80718;
                                    int64_t leq_arg0_80720 = ((int64_t *) mem_param_82489.mem)[slice_80719];
                                    bool defunc_0_leq_res_80721 = sle64(leq_arg0_80720, leq_arg1_80714);
                                    
                                    cond_f_res_73588 = defunc_0_leq_res_80721;
                                } else {
                                    cond_f_res_73588 = 0;
                                }
                                cond_73586 = cond_f_res_73588;
                            }
                            
                            int64_t loopres_73603;
                            int64_t loopres_73604;
                            int64_t loopres_73605;
                            
                            if (cond_73586) {
                                bool x_80723 = sle64((int64_t) 0, i_73581);
                                bool y_80724 = slt64(i_73581, j_m_i_73550);
                                bool bounds_check_80725 = x_80723 && y_80724;
                                bool index_certs_80726;
                                
                                if (!bounds_check_80725) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_73581, "] out of bounds for array of shape [", (long long) j_m_i_73550, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:106:17-21\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  test_dbscan_plus.fut:14:23-19:30\n   #10 test_dbscan_plus.fut:13:1-20:55\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t tmp_80722 = add64((int64_t) 1, i_73581);
                                int64_t slice_80727 = defunc_0_f_res_73526 + i_73581;
                                int64_t slice_80728 = start_73345 + slice_80727;
                                int64_t tmp_80729 = ((int64_t *) mem_param_82489.mem)[slice_80728];
                                int64_t tmp_80730 = ((int64_t *) mem_param_82492.mem)[slice_80728];
                                
                                loopres_73603 = tmp_80722;
                                loopres_73604 = tmp_80729;
                                loopres_73605 = tmp_80730;
                            } else {
                                bool x_73615 = sle64((int64_t) 0, j_73584);
                                bool y_73616 = slt64(j_73584, j_m_i_73537);
                                bool bounds_check_73617 = x_73615 && y_73616;
                                bool index_certs_73618;
                                
                                if (!bounds_check_73617) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_73584, "] out of bounds for array of shape [", (long long) j_m_i_73537, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:108:15-19\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  test_dbscan_plus.fut:14:23-19:30\n   #10 test_dbscan_plus.fut:13:1-20:55\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t slice_73619 = merge_sequential_arg2_73535 + j_73584;
                                int64_t slice_73620 = start_73345 + slice_73619;
                                int64_t tmp_73621 = ((int64_t *) mem_param_82489.mem)[slice_73620];
                                int64_t tmp_73622 = ((int64_t *) mem_param_82492.mem)[slice_73620];
                                
                                loopres_73603 = i_73581;
                                loopres_73604 = tmp_73621;
                                loopres_73605 = tmp_73622;
                            }
                            ((int64_t *) mem_82525)[i_81500 * merge_block_sizze_73150 + k_73580] = loopres_73604;
                            ((int64_t *) mem_82528)[i_81500 * merge_block_sizze_73150 + k_73580] = loopres_73605;
                            
                            int64_t i_tmp_84245 = loopres_73603;
                            
                            i_73581 = i_tmp_84245;
                        }
                        data_73577 = i_73581;
                    }
                    if (memblock_alloc(ctx, &mem_82563, bytes_82524, "mem_82563")) {
                        err = 1;
                        goto cleanup;
                    }
                    lmad_copy_8b(ctx, 2, (uint64_t *) mem_82563.mem, (int64_t) 0, (int64_t []) {merge_block_sizze_73150, (int64_t) 1}, (uint64_t *) mem_82525, (int64_t) 0, (int64_t []) {merge_block_sizze_73150, (int64_t) 1}, (int64_t []) {num_blocks_73293, merge_block_sizze_73150});
                    if (memblock_alloc(ctx, &mem_82567, bytes_82524, "mem_82567")) {
                        err = 1;
                        goto cleanup;
                    }
                    lmad_copy_8b(ctx, 2, (uint64_t *) mem_82567.mem, (int64_t) 0, (int64_t []) {merge_block_sizze_73150, (int64_t) 1}, (uint64_t *) mem_82528, (int64_t) 0, (int64_t []) {merge_block_sizze_73150, (int64_t) 1}, (int64_t []) {num_blocks_73293, merge_block_sizze_73150});
                    if (memblock_set(ctx, &ext_mem_82590, &mem_82563, "mem_82563") != 0)
                        return 1;
                    if (memblock_set(ctx, &ext_mem_82587, &mem_82567, "mem_82567") != 0)
                        return 1;
                }
                lmad_copy_8b(ctx, 1, (uint64_t *) mem_82495, i_81507 * next_stride_73274, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_82590.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {next_stride_73274});
                if (memblock_unref(ctx, &ext_mem_82590, "ext_mem_82590") != 0)
                    return 1;
                lmad_copy_8b(ctx, 1, (uint64_t *) mem_82498, i_81507 * next_stride_73274, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_82587.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {next_stride_73274});
                if (memblock_unref(ctx, &ext_mem_82587, "ext_mem_82587") != 0)
                    return 1;
            }
            
            int64_t flat_dim_73629 = next_stride_73274 * num_merges_73278;
            
            if (memblock_alloc(ctx, &mem_82601, bytes_82494, "mem_82601")) {
                err = 1;
                goto cleanup;
            }
            lmad_copy_8b(ctx, 2, (uint64_t *) mem_82601.mem, (int64_t) 0, (int64_t []) {next_stride_73274, (int64_t) 1}, (uint64_t *) mem_82495, (int64_t) 0, (int64_t []) {next_stride_73274, (int64_t) 1}, (int64_t []) {num_merges_73278, next_stride_73274});
            if (memblock_alloc(ctx, &mem_82605, bytes_82494, "mem_82605")) {
                err = 1;
                goto cleanup;
            }
            lmad_copy_8b(ctx, 2, (uint64_t *) mem_82605.mem, (int64_t) 0, (int64_t []) {next_stride_73274, (int64_t) 1}, (uint64_t *) mem_82498, (int64_t) 0, (int64_t []) {next_stride_73274, (int64_t) 1}, (int64_t []) {num_merges_73278, next_stride_73274});
            
            bool loop_cond_73632 = slt64(next_stride_73274, flat_dim_73147);
            
            if (memblock_set(ctx, &mem_param_tmp_84219, &mem_82601, "mem_82601") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_tmp_84220, &mem_82605, "mem_82605") != 0)
                return 1;
            
            int64_t loop_dz2084Uz2082U_tmp_84221 = flat_dim_73629;
            bool loop_while_tmp_84222 = loop_cond_73632;
            int64_t stride_tmp_84225 = next_stride_73274;
            
            if (memblock_set(ctx, &mem_param_82489, &mem_param_tmp_84219, "mem_param_tmp_84219") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_82492, &mem_param_tmp_84220, "mem_param_tmp_84220") != 0)
                return 1;
            loop_dz2084Uz2082U_73269 = loop_dz2084Uz2082U_tmp_84221;
            loop_while_73270 = loop_while_tmp_84222;
            stride_73273 = stride_tmp_84225;
        }
        if (memblock_set(ctx, &ext_mem_82612, &mem_param_82489, "mem_param_82489") != 0)
            return 1;
        if (memblock_set(ctx, &ext_mem_82611, &mem_param_82492, "mem_param_82492") != 0)
            return 1;
        data_73264 = loop_dz2084Uz2082U_73269;
        data_73265 = loop_while_73270;
        data_73268 = stride_73273;
        if (memblock_unref(ctx, &mem_82481, "mem_82481") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82485, "mem_82485") != 0)
            return 1;
        
        bool i_p_m_t_s_leq_w_73633 = slt64(zs_lhs_73131, data_73264);
        bool y_73634 = zzero_leq_i_p_m_t_s_73190 && i_p_m_t_s_leq_w_73633;
        bool ok_or_empty_73635 = empty_slice_73189 || y_73634;
        bool index_certs_73636;
        
        if (!ok_or_empty_73635) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) dz2081U_57123, "] out of bounds for array of shape [", (long long) data_73264, "].", "-> #0  /prelude/array.fut:46:45-51\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:265:3-14\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #3  /prelude/functional.fut:9:44-45\n   #4  test_dbscan_plus.fut:14:23-19:30\n   #5  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        if (memblock_set(ctx, &ext_mem_82615, &ext_mem_82611, "ext_mem_82611") != 0)
            return 1;
    }
    if (memblock_unref(ctx, &mem_82410, "mem_82410") != 0)
        return 1;
    if (mem_82617_cached_sizze_84592 < bytes_82401) {
        err = lexical_realloc(ctx, &mem_82617, &mem_82617_cached_sizze_84592, bytes_82401);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_82619_cached_sizze_84593 < bytes_82401) {
        err = lexical_realloc(ctx, &mem_82619, &mem_82619_cached_sizze_84593, bytes_82401);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    for (int64_t i_81514 = 0; i_81514 < dz2081U_57123; i_81514++) {
        int64_t eta_p_73640 = ((int64_t *) ext_mem_82615.mem)[i_81514];
        bool x_73641 = sle64((int64_t) 0, eta_p_73640);
        bool y_73642 = slt64(eta_p_73640, dz2081U_57123);
        bool bounds_check_73643 = x_73641 && y_73642;
        bool index_certs_73644;
        
        if (!bounds_check_73643) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_73640, "] out of bounds for array of shape [", (long long) dz2081U_57123, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:277:24-28\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:277:8-29\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        double lifted_lambda_res_73645 = ((double *) pts_mem_82400.mem)[eta_p_73640];
        double lifted_lambda_res_73646 = ((double *) pts_mem_82400.mem)[dz2081U_57123 + eta_p_73640];
        
        ((double *) mem_82617)[i_81514] = lifted_lambda_res_73645;
        ((double *) mem_82619)[i_81514] = lifted_lambda_res_73646;
    }
    if (memblock_unref(ctx, &ext_mem_82615, "ext_mem_82615") != 0)
        return 1;
    if (mem_82632_cached_sizze_84594 < (int64_t) 200) {
        err = lexical_realloc(ctx, &mem_82632, &mem_82632_cached_sizze_84594, (int64_t) 200);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    for (int64_t nest_i_84250 = 0; nest_i_84250 < (int64_t) 25; nest_i_84250++) {
        ((int64_t *) mem_82632)[nest_i_84250] = dz2081U_57123;
    }
    for (int64_t iter_81517 = 0; iter_81517 < dz2081U_57123; iter_81517++) {
        double pixel_81519 = ((double *) mem_82617)[iter_81517];
        double pixel_81520 = ((double *) mem_82619)[iter_81517];
        int64_t lifted_lambda_res_76760;
        
        if (futrts_lifted_get_grid_id_12083(ctx, &lifted_lambda_res_76760, 5.0, step_x_73122, step_y_73124, pixel_81519, pixel_81520) != 0) {
            err = 1;
            goto cleanup;
        }
        
        bool less_than_zzero_81522 = slt64(lifted_lambda_res_76760, (int64_t) 0);
        bool greater_than_sizze_81523 = sle64((int64_t) 25, lifted_lambda_res_76760);
        bool outside_bounds_dim_81524 = less_than_zzero_81522 || greater_than_sizze_81523;
        
        if (!outside_bounds_dim_81524) {
            int64_t read_hist_81526 = ((int64_t *) mem_82632)[lifted_lambda_res_76760];
            bool cond_73655 = slt64(read_hist_81526, iter_81517);
            int64_t lifted_lambda_res_73656;
            
            if (cond_73655) {
                lifted_lambda_res_73656 = read_hist_81526;
            } else {
                lifted_lambda_res_73656 = iter_81517;
            }
            ((int64_t *) mem_82632)[lifted_lambda_res_76760] = lifted_lambda_res_73656;
        }
    }
    if (memblock_alloc(ctx, &mem_82639, (int64_t) 25, "mem_82639")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t nest_i_84252 = 0; nest_i_84252 < (int64_t) 25; nest_i_84252++) {
        ((bool *) mem_82639.mem)[nest_i_84252] = 0;
    }
    if (memblock_alloc(ctx, &mem_82640, (int64_t) 0, "mem_82640")) {
        err = 1;
        goto cleanup;
    }
    if (memblock_alloc(ctx, &mem_82641, (int64_t) 0, "mem_82641")) {
        err = 1;
        goto cleanup;
    }
    if (memblock_alloc(ctx, &mem_82642, (int64_t) 0, "mem_82642")) {
        err = 1;
        goto cleanup;
    }
    if (memblock_alloc(ctx, &mem_82643, (int64_t) 0, "mem_82643")) {
        err = 1;
        goto cleanup;
    }
    if (mem_82698_cached_sizze_84595 < (int64_t) 64) {
        err = lexical_realloc(ctx, &mem_82698, &mem_82698_cached_sizze_84595, (int64_t) 64);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_82704_cached_sizze_84596 < (int64_t) 64) {
        err = lexical_realloc(ctx, &mem_82704, &mem_82704_cached_sizze_84596, (int64_t) 64);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_82705_cached_sizze_84597 < (int64_t) 64) {
        err = lexical_realloc(ctx, &mem_82705, &mem_82705_cached_sizze_84597, (int64_t) 64);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_82706_cached_sizze_84598 < (int64_t) 64) {
        err = lexical_realloc(ctx, &mem_82706, &mem_82706_cached_sizze_84598, (int64_t) 64);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    
    int64_t internal_DBSCAN_double_res_73665;
    int64_t internal_DBSCAN_double_res_73666;
    int64_t internal_DBSCAN_double_res_73667;
    int64_t internal_DBSCAN_double_res_73668;
    int64_t internal_DBSCAN_double_res_73669;
    int64_t internal_DBSCAN_double_res_73670;
    int64_t internal_DBSCAN_double_res_73671;
    int64_t internal_DBSCAN_double_res_73672;
    int64_t internal_DBSCAN_double_res_73673;
    int64_t internal_DBSCAN_double_res_73674;
    bool internal_DBSCAN_double_res_73675;
    int64_t internal_DBSCAN_double_res_73680;
    int64_t internal_DBSCAN_double_res_73688;
    int64_t internal_DBSCAN_double_res_73689;
    int64_t internal_DBSCAN_double_res_73697;
    int64_t loop_dz2084Uz2087U_73698;
    int64_t loop_dz2084Uz2088U_73699;
    int64_t loop_dz2084Uz2089U_73700;
    int64_t loop_dz2085Uz2080U_73701;
    int64_t loop_dz2085Uz2081U_73702;
    int64_t loop_dz2085Uz2086U_73703;
    int64_t loop_dz2085Uz2087U_73704;
    int64_t loop_dz2085Uz2088U_73705;
    int64_t loop_dz2086Uz2080U_73706;
    int64_t loop_dz2086Uz2086U_73707;
    bool loop_while_73708;
    int64_t collisions_73713;
    int64_t old_clHandler_73721;
    int64_t old_clHandler_73722;
    int64_t next_part_73730;
    
    if (memblock_set(ctx, &mem_param_82646, &mem_82642, "mem_82642") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82649, &mem_82642, "mem_82642") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82652, &mem_82643, "mem_82643") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82655, &mem_82640, "mem_82640") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82658, &mem_82640, "mem_82640") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82661, &mem_82640, "mem_82640") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82664, &mem_82640, "mem_82640") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82667, &mem_82641, "mem_82641") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82670, &mem_82640, "mem_82640") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82673, &mem_82642, "mem_82642") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82676, &mem_82642, "mem_82642") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82679, &mem_82639, "mem_82639") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82682, &mem_82640, "mem_82640") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82685, &mem_82640, "mem_82640") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82688, &mem_82642, "mem_82642") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82691, &mem_82642, "mem_82642") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82694, &mem_82641, "mem_82641") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_82697, &mem_82641, "mem_82641") != 0)
        return 1;
    loop_dz2084Uz2087U_73698 = (int64_t) 0;
    loop_dz2084Uz2088U_73699 = (int64_t) 0;
    loop_dz2084Uz2089U_73700 = (int64_t) 0;
    loop_dz2085Uz2080U_73701 = (int64_t) 0;
    loop_dz2085Uz2081U_73702 = (int64_t) 0;
    loop_dz2085Uz2086U_73703 = (int64_t) 0;
    loop_dz2085Uz2087U_73704 = (int64_t) 0;
    loop_dz2085Uz2088U_73705 = (int64_t) 0;
    loop_dz2086Uz2080U_73706 = (int64_t) 0;
    loop_dz2086Uz2086U_73707 = (int64_t) 0;
    loop_while_73708 = 1;
    collisions_73713 = (int64_t) 0;
    old_clHandler_73721 = (int64_t) 0;
    old_clHandler_73722 = (int64_t) -1;
    next_part_73730 = (int64_t) 0;
    while (loop_while_73708) {
        bool x_73731 = sle64((int64_t) 0, next_part_73730);
        bool y_73732 = slt64(next_part_73730, (int64_t) 25);
        bool bounds_check_73733 = x_73731 && y_73732;
        bool index_certs_73734;
        
        if (!bounds_check_73733) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) next_part_73730, "] out of bounds for array of shape [", (long long) (int64_t) 25, "].", "-> #0  ../../ftDBSCAN_plus.fut:1111:35-56\n   #1  test_dbscan_plus.fut:14:23-19:30\n   #2  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t inf_73735 = ((int64_t *) mem_82632)[next_part_73730];
        bool cond_73736 = next_part_73730 == (int64_t) 24;
        bool x_73737 = !cond_73736;
        int64_t tmp_73738 = add64((int64_t) 1, next_part_73730);
        bool x_73739 = sle64((int64_t) 0, tmp_73738);
        bool y_73740 = slt64(tmp_73738, (int64_t) 25);
        bool bounds_check_73741 = x_73739 && y_73740;
        bool protect_assert_disj_73742 = cond_73736 || bounds_check_73741;
        bool index_certs_73743;
        
        if (!protect_assert_disj_73742) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_73738, "] out of bounds for array of shape [", (long long) (int64_t) 25, "].", "-> #0  ../../ftDBSCAN_plus.fut:1112:71-94\n   #1  test_dbscan_plus.fut:14:23-19:30\n   #2  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t sup_f_res_73744;
        
        if (x_73737) {
            int64_t x_80739 = ((int64_t *) mem_82632)[tmp_73738];
            
            sup_f_res_73744 = x_80739;
        } else {
            sup_f_res_73744 = (int64_t) 0;
        }
        
        int64_t sup_73746;
        
        if (cond_73736) {
            sup_73746 = dz2081U_57123;
        } else {
            sup_73746 = sup_f_res_73744;
        }
        
        int64_t dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747 = sub64(sup_73746, inf_73735);
        bool empty_slice_73748 = dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747 == (int64_t) 0;
        int64_t m_73749 = sub64(dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747, (int64_t) 1);
        int64_t i_p_m_t_s_73750 = add64(inf_73735, m_73749);
        bool zzero_leq_i_p_m_t_s_73751 = sle64((int64_t) 0, i_p_m_t_s_73750);
        bool i_p_m_t_s_leq_w_73752 = slt64(i_p_m_t_s_73750, dz2081U_57123);
        bool zzero_lte_i_73753 = sle64((int64_t) 0, inf_73735);
        bool i_lte_j_73754 = sle64(inf_73735, sup_73746);
        bool y_73755 = i_p_m_t_s_leq_w_73752 && zzero_lte_i_73753;
        bool y_73756 = zzero_leq_i_p_m_t_s_73751 && y_73755;
        bool forwards_ok_73757 = i_lte_j_73754 && y_73756;
        bool ok_or_empty_73758 = empty_slice_73748 || forwards_ok_73757;
        bool index_certs_73759;
        
        if (!ok_or_empty_73758) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) inf_73735, ":", (long long) sup_73746, "] out of bounds for array of shape [", (long long) dz2081U_57123, "].", "-> #0  ../../ftDBSCAN_plus.fut:1113:48-67\n   #1  test_dbscan_plus.fut:14:23-19:30\n   #2  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t x_id_73762 = smod64(next_part_73730, (int64_t) 5);
        int64_t y_id_73763 = sdiv64(next_part_73730, (int64_t) 5);
        bool cond_73764 = slt64((int64_t) 0, x_id_73762);
        int64_t zp_lhs_73765 = mul64((int64_t) 5, y_id_73763);
        int64_t zm_lhs_73766 = add64(x_id_73762, zp_lhs_73765);
        int64_t nleft_t_res_73767 = sub64(zm_lhs_73766, (int64_t) 1);
        int64_t nleft_73768;
        
        if (cond_73764) {
            nleft_73768 = nleft_t_res_73767;
        } else {
            nleft_73768 = (int64_t) -1;
        }
        
        bool cond_t_res_73769 = slt64((int64_t) 0, y_id_73763);
        bool x_73770 = cond_73764 && cond_t_res_73769;
        int64_t zt_lhs_73771 = sub64(y_id_73763, (int64_t) 1);
        int64_t zp_lhs_73772 = mul64((int64_t) 5, zt_lhs_73771);
        int64_t zm_lhs_73773 = add64(x_id_73762, zp_lhs_73772);
        int64_t nld_t_res_73774 = sub64(zm_lhs_73773, (int64_t) 1);
        int64_t nld_73775;
        
        if (x_73770) {
            nld_73775 = nld_t_res_73774;
        } else {
            nld_73775 = (int64_t) -1;
        }
        
        int64_t ndown_73776;
        
        if (cond_t_res_73769) {
            ndown_73776 = zm_lhs_73773;
        } else {
            ndown_73776 = (int64_t) -1;
        }
        
        bool cond_73777 = slt64(x_id_73762, (int64_t) 4);
        bool x_73778 = cond_t_res_73769 && cond_73777;
        int64_t nrd_t_res_73779 = add64((int64_t) 1, zm_lhs_73773);
        int64_t nrd_73780;
        
        if (x_73778) {
            nrd_73780 = nrd_t_res_73779;
        } else {
            nrd_73780 = (int64_t) -1;
        }
        
        int64_t nright_t_res_73781 = add64((int64_t) 1, zm_lhs_73766);
        int64_t nright_73782;
        
        if (cond_73777) {
            nright_73782 = nright_t_res_73781;
        } else {
            nright_73782 = (int64_t) -1;
        }
        
        bool cond_t_res_73783 = slt64(y_id_73763, (int64_t) 4);
        bool x_73784 = cond_73777 && cond_t_res_73783;
        int64_t zt_lhs_73785 = add64((int64_t) 1, y_id_73763);
        int64_t zp_lhs_73786 = mul64((int64_t) 5, zt_lhs_73785);
        int64_t zp_lhs_73787 = add64(x_id_73762, zp_lhs_73786);
        int64_t nru_t_res_73788 = add64((int64_t) 1, zp_lhs_73787);
        int64_t nru_73789;
        
        if (x_73784) {
            nru_73789 = nru_t_res_73788;
        } else {
            nru_73789 = (int64_t) -1;
        }
        
        int64_t nup_73790;
        
        if (cond_t_res_73783) {
            nup_73790 = zp_lhs_73787;
        } else {
            nup_73790 = (int64_t) -1;
        }
        
        bool x_73791 = cond_73764 && cond_t_res_73783;
        int64_t nlu_t_res_73792 = sub64(zp_lhs_73787, (int64_t) 1);
        int64_t nlu_73793;
        
        if (x_73791) {
            nlu_73793 = nlu_t_res_73792;
        } else {
            nlu_73793 = (int64_t) -1;
        }
        ((int64_t *) mem_82698)[(int64_t) 0] = nleft_73768;
        ((int64_t *) mem_82698)[(int64_t) 1] = nld_73775;
        ((int64_t *) mem_82698)[(int64_t) 2] = ndown_73776;
        ((int64_t *) mem_82698)[(int64_t) 3] = nrd_73780;
        ((int64_t *) mem_82698)[(int64_t) 4] = nright_73782;
        ((int64_t *) mem_82698)[(int64_t) 5] = nru_73789;
        ((int64_t *) mem_82698)[(int64_t) 6] = nup_73790;
        ((int64_t *) mem_82698)[(int64_t) 7] = nlu_73793;
        if (memblock_alloc(ctx, &mem_82699, (int64_t) 25, "mem_82699")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_1b(ctx, 1, (uint8_t *) mem_82699.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint8_t *) mem_param_82679.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {(int64_t) 25});
        ((bool *) mem_82699.mem)[next_part_73730] = 1;
        
        bool cond_73797 = x_id_73762 == (int64_t) 0;
        double i64_res_73798 = sitofp_i64_f64(x_id_73762);
        double zt_res_73799 = step_x_73122 * i64_res_73798;
        double zp_res_73800 = zt_res_73799 + defunc_0_reduce_res_81449;
        double minC_73801;
        
        if (cond_73797) {
            minC_73801 = -INFINITY;
        } else {
            minC_73801 = zp_res_73800;
        }
        
        bool cond_73802 = y_id_73763 == (int64_t) 0;
        double i64_res_73803 = sitofp_i64_f64(y_id_73763);
        double zt_res_73804 = step_y_73124 * i64_res_73803;
        double zp_res_73805 = zt_res_73804 + defunc_0_reduce_res_81451;
        double minC_73806;
        
        if (cond_73802) {
            minC_73806 = -INFINITY;
        } else {
            minC_73806 = zp_res_73805;
        }
        
        bool cond_73807 = x_id_73762 == (int64_t) 4;
        int64_t from_i64_arg0_73808 = add64((int64_t) 1, x_id_73762);
        double i64_res_73809 = sitofp_i64_f64(from_i64_arg0_73808);
        double zt_res_73810 = step_x_73122 * i64_res_73809;
        double zp_res_73811 = zt_res_73810 + defunc_0_reduce_res_81449;
        double maxC_73812;
        
        if (cond_73807) {
            maxC_73812 = INFINITY;
        } else {
            maxC_73812 = zp_res_73811;
        }
        
        bool cond_73813 = y_id_73763 == (int64_t) 4;
        double i64_res_73814 = sitofp_i64_f64(zt_lhs_73785);
        double zt_res_73815 = step_y_73124 * i64_res_73814;
        double zp_res_73816 = zt_res_73815 + defunc_0_reduce_res_81451;
        double maxC_73817;
        
        if (cond_73813) {
            maxC_73817 = INFINITY;
        } else {
            maxC_73817 = zp_res_73816;
        }
        
        int64_t bytes_82700 = (int64_t) 8 * dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747;
        
        if (memblock_alloc(ctx, &mem_82701, bytes_82700, "mem_82701")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_82701.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82617, (int64_t) 0 + (int64_t) 1 * inf_73735, (int64_t []) {(int64_t) 1}, (int64_t []) {dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747});
        if (memblock_alloc(ctx, &mem_82703, bytes_82700, "mem_82703")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_82703.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82619, (int64_t) 0 + (int64_t) 1 * inf_73735, (int64_t []) {(int64_t) 1}, (int64_t []) {dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747});
        
        int64_t discard_81544;
        int64_t scanacc_81536 = (int64_t) 0;
        
        for (int64_t i_81540 = 0; i_81540 < (int64_t) 8; i_81540++) {
            int64_t eta_p_78350 = ((int64_t *) mem_82698)[i_81540];
            bool defunc_0_reduce_res_80749;
            bool redout_81529 = 0;
            
            for (int64_t i_81530 = 0; i_81530 < loop_dz2086Uz2080U_73706; i_81530++) {
                int64_t eta_p_78452 = ((int64_t *) mem_param_82682.mem)[i_81530];
                bool lifted_lambda_res_78453 = eta_p_78452 == eta_p_78350;
                bool defunc_0_op_res_78358 = lifted_lambda_res_78453 || redout_81529;
                bool redout_tmp_84308 = defunc_0_op_res_78358;
                
                redout_81529 = redout_tmp_84308;
            }
            defunc_0_reduce_res_80749 = redout_81529;
            
            int64_t lifted_lambda_res_78359;
            
            if (defunc_0_reduce_res_80749) {
                lifted_lambda_res_78359 = (int64_t) -1;
            } else {
                lifted_lambda_res_78359 = eta_p_78350;
            }
            
            bool cond_78361 = slt64(lifted_lambda_res_78359, (int64_t) 0);
            int64_t defunc_0_f_res_78362;
            
            if (cond_78361) {
                defunc_0_f_res_78362 = (int64_t) -1;
            } else {
                defunc_0_f_res_78362 = lifted_lambda_res_78359;
            }
            
            bool defunc_0_f_res_78363;
            
            if (cond_78361) {
                defunc_0_f_res_78363 = 0;
            } else {
                int64_t nxid_78364 = smod64(lifted_lambda_res_78359, (int64_t) 5);
                int64_t nyid_78365 = sdiv64(lifted_lambda_res_78359, (int64_t) 5);
                double i64_res_78366 = sitofp_i64_f64(nxid_78364);
                double zt_res_78367 = step_x_73122 * i64_res_78366;
                double zp_res_78368 = zt_res_78367 + defunc_0_reduce_res_81449;
                double i64_res_78369 = sitofp_i64_f64(nyid_78365);
                double zt_res_78370 = step_y_73124 * i64_res_78369;
                double zp_res_78371 = zt_res_78370 + defunc_0_reduce_res_81451;
                double zp_res_78372 = step_x_73122 + zp_res_78368;
                double zp_res_78373 = step_y_73124 + zp_res_78371;
                bool defunc_0_reduce_res_80748;
                bool redout_81531 = 0;
                
                for (int64_t i_81532 = 0; i_81532 < dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747; i_81532++) {
                    int64_t slice_82382 = inf_73735 + i_81532;
                    double eta_p_78456 = ((double *) mem_82617)[slice_82382];
                    double eta_p_78457 = ((double *) mem_82619)[slice_82382];
                    bool zgze_res_78458 = zp_res_78371 <= eta_p_78457;
                    bool zlze_res_78459 = eta_p_78457 <= zp_res_78373;
                    bool x_78460 = zgze_res_78458 && zlze_res_78459;
                    bool zeze_res_78461 = eta_p_78456 == zp_res_78368;
                    double d_euclidean_res_78462;
                    
                    if (zeze_res_78461) {
                        double zm_res_80740 = eta_p_78457 - eta_p_78457;
                        double abs_res_80741 = fabs64(zm_res_80740);
                        
                        d_euclidean_res_78462 = abs_res_80741;
                    } else {
                        double zm_res_78465 = eta_p_78456 - zp_res_78368;
                        double abs_res_78466 = fabs64(zm_res_78465);
                        
                        d_euclidean_res_78462 = abs_res_78466;
                    }
                    
                    bool zlze_res_78467 = d_euclidean_res_78462 <= 2.0;
                    bool zeze_res_78468 = eta_p_78456 == zp_res_78372;
                    double d_euclidean_res_78469;
                    
                    if (zeze_res_78468) {
                        double zm_res_80742 = eta_p_78457 - eta_p_78457;
                        double abs_res_80743 = fabs64(zm_res_80742);
                        
                        d_euclidean_res_78469 = abs_res_80743;
                    } else {
                        double zm_res_78472 = eta_p_78456 - zp_res_78372;
                        double abs_res_78473 = fabs64(zm_res_78472);
                        
                        d_euclidean_res_78469 = abs_res_78473;
                    }
                    
                    bool zlze_res_78474 = d_euclidean_res_78469 <= 2.0;
                    bool x_78475 = !zlze_res_78467;
                    bool y_78476 = zlze_res_78474 && x_78475;
                    bool cond_t_res_78477 = zlze_res_78467 || y_78476;
                    bool x_78478 = x_78460 && cond_t_res_78477;
                    bool zgze_res_78479 = zp_res_78368 <= eta_p_78456;
                    bool zlze_res_78480 = eta_p_78456 <= zp_res_78372;
                    bool x_78481 = zgze_res_78479 && zlze_res_78480;
                    double zm_res_78482 = eta_p_78457 - zp_res_78371;
                    double abs_res_78483 = fabs64(zm_res_78482);
                    bool zlze_res_78484 = abs_res_78483 <= 2.0;
                    double zm_res_78485 = eta_p_78457 - zp_res_78373;
                    double abs_res_78486 = fabs64(zm_res_78485);
                    bool zlze_res_78487 = abs_res_78486 <= 2.0;
                    bool x_78488 = !zlze_res_78484;
                    bool y_78489 = zlze_res_78487 && x_78488;
                    bool cond_f_res_t_res_78490 = zlze_res_78484 || y_78489;
                    bool x_78491 = x_78481 && cond_f_res_t_res_78490;
                    bool x_78492 = !x_78478;
                    bool y_78493 = x_78491 && x_78492;
                    bool cond_78494 = x_78478 || y_78493;
                    bool zlze_res_78495 = eta_p_78456 <= zp_res_78368;
                    double corn_x_78496;
                    
                    if (zlze_res_78495) {
                        corn_x_78496 = zp_res_78368;
                    } else {
                        corn_x_78496 = zp_res_78372;
                    }
                    
                    bool zlze_res_78497 = eta_p_78457 <= zp_res_78371;
                    double corn_y_78498;
                    
                    if (zlze_res_78497) {
                        corn_y_78498 = zp_res_78371;
                    } else {
                        corn_y_78498 = zp_res_78373;
                    }
                    
                    bool p_and_eq_x_y_78499 = zeze_res_78461 && zlze_res_78495;
                    bool not_p_78500 = !zlze_res_78495;
                    bool p_and_eq_x_y_78501 = zeze_res_78468 && not_p_78500;
                    bool zeze_res_78502 = p_and_eq_x_y_78499 || p_and_eq_x_y_78501;
                    double d_euclidean_res_78503;
                    
                    if (zeze_res_78502) {
                        double zm_res_80744 = eta_p_78457 - corn_y_78498;
                        double abs_res_80745 = fabs64(zm_res_80744);
                        
                        d_euclidean_res_78503 = abs_res_80745;
                    } else {
                        bool eq_x_y_78506 = eta_p_78457 == zp_res_78371;
                        bool eq_x_zz_78507 = eta_p_78457 == zp_res_78373;
                        bool p_and_eq_x_y_78508 = zlze_res_78497 && eq_x_y_78506;
                        bool not_p_78509 = !zlze_res_78497;
                        bool p_and_eq_x_y_78510 = eq_x_zz_78507 && not_p_78509;
                        bool zeze_res_78511 = p_and_eq_x_y_78508 || p_and_eq_x_y_78510;
                        double d_euclidean_res_f_res_78512;
                        
                        if (zeze_res_78511) {
                            double zm_res_80746 = eta_p_78456 - corn_x_78496;
                            double abs_res_80747 = fabs64(zm_res_80746);
                            
                            d_euclidean_res_f_res_78512 = abs_res_80747;
                        } else {
                            double zm_res_78515 = eta_p_78457 - corn_y_78498;
                            double zm_res_78516 = eta_p_78456 - corn_x_78496;
                            double hypot_res_78517 = futrts_hypot64(zm_res_78516, zm_res_78515);
                            
                            d_euclidean_res_f_res_78512 = hypot_res_78517;
                        }
                        d_euclidean_res_78503 = d_euclidean_res_f_res_78512;
                    }
                    
                    bool zlze_res_78518 = d_euclidean_res_78503 <= 2.0;
                    bool x_78519 = !cond_78494;
                    bool y_78520 = zlze_res_78518 && x_78519;
                    bool is_pt_marginal_res_78521 = cond_78494 || y_78520;
                    bool defunc_0_op_res_78445 = is_pt_marginal_res_78521 || redout_81531;
                    bool redout_tmp_84309 = defunc_0_op_res_78445;
                    
                    redout_81531 = redout_tmp_84309;
                }
                defunc_0_reduce_res_80748 = redout_81531;
                defunc_0_f_res_78363 = defunc_0_reduce_res_80748;
            }
            
            int64_t defunc_0_f_res_78448 = btoi_bool_i64(defunc_0_f_res_78363);
            int64_t defunc_0_op_res_73926 = add64(defunc_0_f_res_78448, scanacc_81536);
            
            ((int64_t *) mem_82704)[i_81540] = defunc_0_op_res_73926;
            ((int64_t *) mem_82705)[i_81540] = defunc_0_f_res_78448;
            ((int64_t *) mem_82706)[i_81540] = defunc_0_f_res_78362;
            
            int64_t scanacc_tmp_84304 = defunc_0_op_res_73926;
            
            scanacc_81536 = scanacc_tmp_84304;
        }
        discard_81544 = scanacc_81536;
        
        int64_t m_f_res_73927 = ((int64_t *) mem_82704)[(int64_t) 7];
        int64_t m_73935 = sub64(m_f_res_73927, (int64_t) 1);
        bool i_p_m_t_s_leq_w_73937 = slt64(m_73935, (int64_t) 8);
        bool zzero_leq_i_p_m_t_s_73936 = sle64((int64_t) 0, m_73935);
        bool y_73939 = zzero_leq_i_p_m_t_s_73936 && i_p_m_t_s_leq_w_73937;
        bool i_lte_j_73938 = sle64((int64_t) 0, m_f_res_73927);
        bool forwards_ok_73940 = i_lte_j_73938 && y_73939;
        bool empty_slice_73934 = m_f_res_73927 == (int64_t) 0;
        bool ok_or_empty_73941 = empty_slice_73934 || forwards_ok_73940;
        bool index_certs_73942;
        
        if (!ok_or_empty_73941) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_f_res_73927, "] out of bounds for array of shape [", (long long) (int64_t) 8, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_82725 = (int64_t) 8 * m_f_res_73927;
        
        if (mem_82726_cached_sizze_84599 < bytes_82725) {
            err = lexical_realloc(ctx, &mem_82726, &mem_82726_cached_sizze_84599, bytes_82725);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_82726, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82706, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_f_res_73927});
        for (int64_t write_iter_81545 = 0; write_iter_81545 < (int64_t) 8; write_iter_81545++) {
            int64_t write_iv_81547 = ((int64_t *) mem_82705)[write_iter_81545];
            int64_t write_iv_81548 = ((int64_t *) mem_82704)[write_iter_81545];
            int64_t write_iv_81549 = ((int64_t *) mem_82706)[write_iter_81545];
            bool cond_78249 = write_iv_81547 == (int64_t) 1;
            int64_t lifted_lambda_res_78250;
            
            if (cond_78249) {
                int64_t lifted_lambda_res_t_res_80750 = sub64(write_iv_81548, (int64_t) 1);
                
                lifted_lambda_res_78250 = lifted_lambda_res_t_res_80750;
            } else {
                lifted_lambda_res_78250 = (int64_t) -1;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_78250) && slt64(lifted_lambda_res_78250, m_f_res_73927)) {
                ((int64_t *) mem_82726)[lifted_lambda_res_78250] = write_iv_81549;
            }
        }
        
        int64_t conc_tmp_73948 = loop_dz2086Uz2080U_73706 + m_f_res_73927;
        int64_t bytes_82733 = (int64_t) 8 * conc_tmp_73948;
        
        if (mem_82734_cached_sizze_84600 < bytes_82733) {
            err = lexical_realloc(ctx, &mem_82734, &mem_82734_cached_sizze_84600, bytes_82733);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t tmp_offs_84311 = (int64_t) 0;
        
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_82734, tmp_offs_84311, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_82682.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {loop_dz2086Uz2080U_73706});
        tmp_offs_84311 += loop_dz2086Uz2080U_73706;
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_82734, tmp_offs_84311, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82726, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_f_res_73927});
        tmp_offs_84311 += m_f_res_73927;
        if (mem_82736_cached_sizze_84601 < bytes_82733) {
            err = lexical_realloc(ctx, &mem_82736, &mem_82736_cached_sizze_84601, bytes_82733);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_82738_cached_sizze_84602 < bytes_82733) {
            err = lexical_realloc(ctx, &mem_82738, &mem_82738_cached_sizze_84602, bytes_82733);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_81559;
        int64_t scanacc_81553 = (int64_t) 0;
        
        for (int64_t i_81556 = 0; i_81556 < conc_tmp_73948; i_81556++) {
            bool index_concat_cmp_82376 = sle64(loop_dz2086Uz2080U_73706, i_81556);
            int64_t index_concat_branch_82380;
            
            if (index_concat_cmp_82376) {
                int64_t index_concat_i_82377 = sub64(i_81556, loop_dz2086Uz2080U_73706);
                int64_t index_concat_82378 = ((int64_t *) mem_82726)[index_concat_i_82377];
                
                index_concat_branch_82380 = index_concat_82378;
            } else {
                int64_t index_concat_82379 = ((int64_t *) mem_param_82682.mem)[i_81556];
                
                index_concat_branch_82380 = index_concat_82379;
            }
            
            bool x_78237 = sle64((int64_t) 0, index_concat_branch_82380);
            bool y_78238 = slt64(index_concat_branch_82380, (int64_t) 25);
            bool bounds_check_78239 = x_78237 && y_78238;
            bool index_certs_78240;
            
            if (!bounds_check_78239) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) index_concat_branch_82380, "] out of bounds for array of shape [", (long long) (int64_t) 25, "].", "-> #0  ../../ftDBSCAN_plus.fut:352:53-75\n   #1  /prelude/soacs.fut:255:31-32\n   #2  /prelude/soacs.fut:255:48-50\n   #3  /prelude/functional.fut:9:44-45\n   #4  test_dbscan_plus.fut:14:23-19:30\n   #5  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool tmp_78241 = ((bool *) mem_82699.mem)[index_concat_branch_82380];
            bool lifted_lambda_res_78242 = !tmp_78241;
            int64_t defunc_0_f_res_78243 = btoi_bool_i64(lifted_lambda_res_78242);
            int64_t defunc_0_op_res_73963 = add64(defunc_0_f_res_78243, scanacc_81553);
            
            ((int64_t *) mem_82736)[i_81556] = defunc_0_op_res_73963;
            ((int64_t *) mem_82738)[i_81556] = defunc_0_f_res_78243;
            
            int64_t scanacc_tmp_84312 = defunc_0_op_res_73963;
            
            scanacc_81553 = scanacc_tmp_84312;
        }
        discard_81559 = scanacc_81553;
        
        bool cond_73964 = conc_tmp_73948 == (int64_t) 0;
        bool x_73965 = !cond_73964;
        int64_t tmp_73966 = sub64(conc_tmp_73948, (int64_t) 1);
        bool x_73967 = sle64((int64_t) 0, tmp_73966);
        bool y_73968 = slt64(tmp_73966, conc_tmp_73948);
        bool bounds_check_73969 = x_73967 && y_73968;
        bool protect_assert_disj_73970 = cond_73964 || bounds_check_73969;
        bool index_certs_73971;
        
        if (!protect_assert_disj_73970) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_73966, "] out of bounds for array of shape [", (long long) conc_tmp_73948, "].", "-> #0  /prelude/soacs.fut:257:33-47\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t m_f_res_73972;
        
        if (x_73965) {
            int64_t x_80751 = ((int64_t *) mem_82736)[tmp_73966];
            
            m_f_res_73972 = x_80751;
        } else {
            m_f_res_73972 = (int64_t) 0;
        }
        
        int64_t m_73974;
        
        if (cond_73964) {
            m_73974 = (int64_t) 0;
        } else {
            m_73974 = m_f_res_73972;
        }
        
        int64_t m_73984 = sub64(m_73974, (int64_t) 1);
        bool i_p_m_t_s_leq_w_73986 = slt64(m_73984, conc_tmp_73948);
        bool zzero_leq_i_p_m_t_s_73985 = sle64((int64_t) 0, m_73984);
        bool y_73988 = zzero_leq_i_p_m_t_s_73985 && i_p_m_t_s_leq_w_73986;
        bool i_lte_j_73987 = sle64((int64_t) 0, m_73974);
        bool forwards_ok_73989 = i_lte_j_73987 && y_73988;
        bool eq_x_zz_73981 = (int64_t) 0 == m_f_res_73972;
        bool p_and_eq_x_y_73982 = x_73965 && eq_x_zz_73981;
        bool empty_slice_73983 = cond_73964 || p_and_eq_x_y_73982;
        bool ok_or_empty_73990 = empty_slice_73983 || forwards_ok_73989;
        bool index_certs_73991;
        
        if (!ok_or_empty_73990) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_73974, "] out of bounds for array of shape [", (long long) conc_tmp_73948, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_82751 = (int64_t) 8 * m_73974;
        
        if (memblock_alloc(ctx, &mem_82752, bytes_82751, "mem_82752")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_82752.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82734, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_73974});
        for (int64_t write_iter_81560 = 0; write_iter_81560 < conc_tmp_73948; write_iter_81560++) {
            int64_t write_iv_81562 = ((int64_t *) mem_82738)[write_iter_81560];
            int64_t write_iv_81563 = ((int64_t *) mem_82736)[write_iter_81560];
            bool index_concat_cmp_82370 = sle64(loop_dz2086Uz2080U_73706, write_iter_81560);
            int64_t index_concat_branch_82374;
            
            if (index_concat_cmp_82370) {
                int64_t index_concat_i_82371 = sub64(write_iter_81560, loop_dz2086Uz2080U_73706);
                int64_t index_concat_82372 = ((int64_t *) mem_82726)[index_concat_i_82371];
                
                index_concat_branch_82374 = index_concat_82372;
            } else {
                int64_t index_concat_82373 = ((int64_t *) mem_param_82682.mem)[write_iter_81560];
                
                index_concat_branch_82374 = index_concat_82373;
            }
            
            bool cond_78231 = write_iv_81562 == (int64_t) 1;
            int64_t lifted_lambda_res_78232;
            
            if (cond_78231) {
                int64_t lifted_lambda_res_t_res_80752 = sub64(write_iv_81563, (int64_t) 1);
                
                lifted_lambda_res_78232 = lifted_lambda_res_t_res_80752;
            } else {
                lifted_lambda_res_78232 = (int64_t) -1;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_78232) && slt64(lifted_lambda_res_78232, m_73974)) {
                ((int64_t *) mem_82752.mem)[lifted_lambda_res_78232] = index_concat_branch_82374;
            }
        }
        if (mem_82760_cached_sizze_84603 < bytes_82700) {
            err = lexical_realloc(ctx, &mem_82760, &mem_82760_cached_sizze_84603, bytes_82700);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_82762_cached_sizze_84604 < bytes_82700) {
            err = lexical_realloc(ctx, &mem_82762, &mem_82762_cached_sizze_84604, bytes_82700);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (memblock_alloc(ctx, &mem_82763, dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747, "mem_82763")) {
            err = 1;
            goto cleanup;
        }
        
        int64_t discard_81581;
        int64_t scanacc_81573 = (int64_t) 0;
        
        for (int64_t i_81577 = 0; i_81577 < dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747; i_81577++) {
            int64_t slice_82368 = inf_73735 + i_81577;
            double eta_p_78050 = ((double *) mem_82617)[slice_82368];
            double eta_p_78051 = ((double *) mem_82619)[slice_82368];
            bool defunc_0_reduce_res_80853;
            bool redout_81566 = 0;
            
            for (int64_t i_81567 = 0; i_81567 < (int64_t) 8; i_81567++) {
                int64_t eta_p_78554 = ((int64_t *) mem_82698)[i_81567];
                bool cond_78555 = slt64(eta_p_78554, (int64_t) 0);
                int64_t nxid_78556 = smod64(eta_p_78554, (int64_t) 5);
                int64_t nyid_78557 = sdiv64(eta_p_78554, (int64_t) 5);
                double i64_res_78558 = sitofp_i64_f64(nxid_78556);
                double zt_res_78559 = step_x_73122 * i64_res_78558;
                double zp_res_78560 = zt_res_78559 + defunc_0_reduce_res_81449;
                double i64_res_78561 = sitofp_i64_f64(nyid_78557);
                double zt_res_78562 = step_y_73124 * i64_res_78561;
                double zp_res_78563 = zt_res_78562 + defunc_0_reduce_res_81451;
                double zp_res_78564 = step_x_73122 + zp_res_78560;
                double zp_res_78565 = step_y_73124 + zp_res_78563;
                bool zgze_res_78566 = zp_res_78563 <= eta_p_78051;
                bool zlze_res_78567 = eta_p_78051 <= zp_res_78565;
                bool x_78568 = zgze_res_78566 && zlze_res_78567;
                bool zeze_res_78569 = eta_p_78050 == zp_res_78560;
                double d_euclidean_res_78570;
                
                if (zeze_res_78569) {
                    double zm_res_80753 = eta_p_78051 - eta_p_78051;
                    double abs_res_80754 = fabs64(zm_res_80753);
                    
                    d_euclidean_res_78570 = abs_res_80754;
                } else {
                    double zm_res_78573 = eta_p_78050 - zp_res_78560;
                    double abs_res_78574 = fabs64(zm_res_78573);
                    
                    d_euclidean_res_78570 = abs_res_78574;
                }
                
                bool zlze_res_78575 = d_euclidean_res_78570 <= 4.0;
                bool zeze_res_78576 = eta_p_78050 == zp_res_78564;
                double d_euclidean_res_78577;
                
                if (zeze_res_78576) {
                    double zm_res_80755 = eta_p_78051 - eta_p_78051;
                    double abs_res_80756 = fabs64(zm_res_80755);
                    
                    d_euclidean_res_78577 = abs_res_80756;
                } else {
                    double zm_res_78580 = eta_p_78050 - zp_res_78564;
                    double abs_res_78581 = fabs64(zm_res_78580);
                    
                    d_euclidean_res_78577 = abs_res_78581;
                }
                
                bool zlze_res_78582 = d_euclidean_res_78577 <= 4.0;
                bool x_78583 = !zlze_res_78575;
                bool y_78584 = zlze_res_78582 && x_78583;
                bool cond_t_res_78585 = zlze_res_78575 || y_78584;
                bool x_78586 = x_78568 && cond_t_res_78585;
                bool zgze_res_78587 = zp_res_78560 <= eta_p_78050;
                bool zlze_res_78588 = eta_p_78050 <= zp_res_78564;
                bool x_78589 = zgze_res_78587 && zlze_res_78588;
                double zm_res_78590 = eta_p_78051 - zp_res_78563;
                double abs_res_78591 = fabs64(zm_res_78590);
                bool zlze_res_78592 = abs_res_78591 <= 4.0;
                double zm_res_78593 = eta_p_78051 - zp_res_78565;
                double abs_res_78594 = fabs64(zm_res_78593);
                bool zlze_res_78595 = abs_res_78594 <= 4.0;
                bool x_78596 = !zlze_res_78592;
                bool y_78597 = zlze_res_78595 && x_78596;
                bool cond_f_res_t_res_78598 = zlze_res_78592 || y_78597;
                bool x_78599 = x_78589 && cond_f_res_t_res_78598;
                bool x_78600 = !x_78586;
                bool y_78601 = x_78599 && x_78600;
                bool cond_78602 = x_78586 || y_78601;
                bool zlze_res_78603 = eta_p_78050 <= zp_res_78560;
                double corn_x_78604;
                
                if (zlze_res_78603) {
                    corn_x_78604 = zp_res_78560;
                } else {
                    corn_x_78604 = zp_res_78564;
                }
                
                bool zlze_res_78605 = eta_p_78051 <= zp_res_78563;
                double corn_y_78606;
                
                if (zlze_res_78605) {
                    corn_y_78606 = zp_res_78563;
                } else {
                    corn_y_78606 = zp_res_78565;
                }
                
                bool p_and_eq_x_y_78607 = zeze_res_78569 && zlze_res_78603;
                bool not_p_78608 = !zlze_res_78603;
                bool p_and_eq_x_y_78609 = zeze_res_78576 && not_p_78608;
                bool zeze_res_78610 = p_and_eq_x_y_78607 || p_and_eq_x_y_78609;
                double d_euclidean_res_78611;
                
                if (zeze_res_78610) {
                    double zm_res_80757 = eta_p_78051 - corn_y_78606;
                    double abs_res_80758 = fabs64(zm_res_80757);
                    
                    d_euclidean_res_78611 = abs_res_80758;
                } else {
                    bool eq_x_y_78614 = eta_p_78051 == zp_res_78563;
                    bool eq_x_zz_78615 = eta_p_78051 == zp_res_78565;
                    bool p_and_eq_x_y_78616 = zlze_res_78605 && eq_x_y_78614;
                    bool not_p_78617 = !zlze_res_78605;
                    bool p_and_eq_x_y_78618 = eq_x_zz_78615 && not_p_78617;
                    bool zeze_res_78619 = p_and_eq_x_y_78616 || p_and_eq_x_y_78618;
                    double d_euclidean_res_f_res_78620;
                    
                    if (zeze_res_78619) {
                        double zm_res_80759 = eta_p_78050 - corn_x_78604;
                        double abs_res_80760 = fabs64(zm_res_80759);
                        
                        d_euclidean_res_f_res_78620 = abs_res_80760;
                    } else {
                        double zm_res_78623 = eta_p_78051 - corn_y_78606;
                        double zm_res_78624 = eta_p_78050 - corn_x_78604;
                        double hypot_res_78625 = futrts_hypot64(zm_res_78624, zm_res_78623);
                        
                        d_euclidean_res_f_res_78620 = hypot_res_78625;
                    }
                    d_euclidean_res_78611 = d_euclidean_res_f_res_78620;
                }
                
                bool zlze_res_78626 = d_euclidean_res_78611 <= 4.0;
                bool x_78627 = !cond_78602;
                bool y_78628 = zlze_res_78626 && x_78627;
                bool is_pt_marginal_res_78629 = cond_78602 || y_78628;
                bool x_78630 = !cond_78555;
                bool y_78631 = is_pt_marginal_res_78629 && x_78630;
                bool defunc_0_op_res_78137 = y_78631 || redout_81566;
                bool redout_tmp_84320 = defunc_0_op_res_78137;
                
                redout_81566 = redout_tmp_84320;
            }
            defunc_0_reduce_res_80853 = redout_81566;
            
            bool defunc_0_f_res_78139;
            
            if (defunc_0_reduce_res_80853) {
                bool defunc_0_reduce_res_80770;
                bool redout_81568 = 0;
                
                for (int64_t i_81569 = 0; i_81569 < (int64_t) 8; i_81569++) {
                    int64_t eta_p_80771 = ((int64_t *) mem_82698)[i_81569];
                    bool cond_80772 = slt64(eta_p_80771, (int64_t) 0);
                    int64_t nxid_80773 = smod64(eta_p_80771, (int64_t) 5);
                    int64_t nyid_80774 = sdiv64(eta_p_80771, (int64_t) 5);
                    double i64_res_80775 = sitofp_i64_f64(nxid_80773);
                    double zt_res_80776 = step_x_73122 * i64_res_80775;
                    double zp_res_80777 = zt_res_80776 + defunc_0_reduce_res_81449;
                    double i64_res_80778 = sitofp_i64_f64(nyid_80774);
                    double zt_res_80779 = step_y_73124 * i64_res_80778;
                    double zp_res_80780 = zt_res_80779 + defunc_0_reduce_res_81451;
                    double zp_res_80781 = step_x_73122 + zp_res_80777;
                    double zp_res_80782 = step_y_73124 + zp_res_80780;
                    bool zgze_res_80783 = zp_res_80780 <= eta_p_78051;
                    bool zlze_res_80784 = eta_p_78051 <= zp_res_80782;
                    bool x_80785 = zgze_res_80783 && zlze_res_80784;
                    bool zeze_res_80786 = eta_p_78050 == zp_res_80777;
                    double d_euclidean_res_80787;
                    
                    if (zeze_res_80786) {
                        double zm_res_80788 = eta_p_78051 - eta_p_78051;
                        double abs_res_80789 = fabs64(zm_res_80788);
                        
                        d_euclidean_res_80787 = abs_res_80789;
                    } else {
                        double zm_res_80790 = eta_p_78050 - zp_res_80777;
                        double abs_res_80791 = fabs64(zm_res_80790);
                        
                        d_euclidean_res_80787 = abs_res_80791;
                    }
                    
                    bool zlze_res_80792 = d_euclidean_res_80787 <= 2.0;
                    bool zeze_res_80793 = eta_p_78050 == zp_res_80781;
                    double d_euclidean_res_80794;
                    
                    if (zeze_res_80793) {
                        double zm_res_80795 = eta_p_78051 - eta_p_78051;
                        double abs_res_80796 = fabs64(zm_res_80795);
                        
                        d_euclidean_res_80794 = abs_res_80796;
                    } else {
                        double zm_res_80797 = eta_p_78050 - zp_res_80781;
                        double abs_res_80798 = fabs64(zm_res_80797);
                        
                        d_euclidean_res_80794 = abs_res_80798;
                    }
                    
                    bool zlze_res_80799 = d_euclidean_res_80794 <= 2.0;
                    bool x_80800 = !zlze_res_80792;
                    bool y_80801 = zlze_res_80799 && x_80800;
                    bool cond_t_res_80802 = zlze_res_80792 || y_80801;
                    bool x_80803 = x_80785 && cond_t_res_80802;
                    bool zgze_res_80804 = zp_res_80777 <= eta_p_78050;
                    bool zlze_res_80805 = eta_p_78050 <= zp_res_80781;
                    bool x_80806 = zgze_res_80804 && zlze_res_80805;
                    double zm_res_80807 = eta_p_78051 - zp_res_80780;
                    double abs_res_80808 = fabs64(zm_res_80807);
                    bool zlze_res_80809 = abs_res_80808 <= 2.0;
                    double zm_res_80810 = eta_p_78051 - zp_res_80782;
                    double abs_res_80811 = fabs64(zm_res_80810);
                    bool zlze_res_80812 = abs_res_80811 <= 2.0;
                    bool x_80813 = !zlze_res_80809;
                    bool y_80814 = zlze_res_80812 && x_80813;
                    bool cond_f_res_t_res_80815 = zlze_res_80809 || y_80814;
                    bool x_80816 = x_80806 && cond_f_res_t_res_80815;
                    bool x_80817 = !x_80803;
                    bool y_80818 = x_80816 && x_80817;
                    bool cond_80819 = x_80803 || y_80818;
                    bool zlze_res_80820 = eta_p_78050 <= zp_res_80777;
                    double corn_x_80821;
                    
                    if (zlze_res_80820) {
                        corn_x_80821 = zp_res_80777;
                    } else {
                        corn_x_80821 = zp_res_80781;
                    }
                    
                    bool zlze_res_80822 = eta_p_78051 <= zp_res_80780;
                    double corn_y_80823;
                    
                    if (zlze_res_80822) {
                        corn_y_80823 = zp_res_80780;
                    } else {
                        corn_y_80823 = zp_res_80782;
                    }
                    
                    bool p_and_eq_x_y_80824 = zeze_res_80786 && zlze_res_80820;
                    bool not_p_80825 = !zlze_res_80820;
                    bool p_and_eq_x_y_80826 = zeze_res_80793 && not_p_80825;
                    bool zeze_res_80827 = p_and_eq_x_y_80824 || p_and_eq_x_y_80826;
                    double d_euclidean_res_80828;
                    
                    if (zeze_res_80827) {
                        double zm_res_80829 = eta_p_78051 - corn_y_80823;
                        double abs_res_80830 = fabs64(zm_res_80829);
                        
                        d_euclidean_res_80828 = abs_res_80830;
                    } else {
                        bool eq_x_y_80831 = eta_p_78051 == zp_res_80780;
                        bool eq_x_zz_80832 = eta_p_78051 == zp_res_80782;
                        bool p_and_eq_x_y_80833 = zlze_res_80822 && eq_x_y_80831;
                        bool not_p_80834 = !zlze_res_80822;
                        bool p_and_eq_x_y_80835 = eq_x_zz_80832 && not_p_80834;
                        bool zeze_res_80836 = p_and_eq_x_y_80833 || p_and_eq_x_y_80835;
                        double d_euclidean_res_f_res_80837;
                        
                        if (zeze_res_80836) {
                            double zm_res_80838 = eta_p_78050 - corn_x_80821;
                            double abs_res_80839 = fabs64(zm_res_80838);
                            
                            d_euclidean_res_f_res_80837 = abs_res_80839;
                        } else {
                            double zm_res_80840 = eta_p_78051 - corn_y_80823;
                            double zm_res_80841 = eta_p_78050 - corn_x_80821;
                            double hypot_res_80842 = futrts_hypot64(zm_res_80841, zm_res_80840);
                            
                            d_euclidean_res_f_res_80837 = hypot_res_80842;
                        }
                        d_euclidean_res_80828 = d_euclidean_res_f_res_80837;
                    }
                    
                    bool zlze_res_80843 = d_euclidean_res_80828 <= 2.0;
                    bool x_80844 = !cond_80819;
                    bool y_80845 = zlze_res_80843 && x_80844;
                    bool is_pt_marginal_res_80846 = cond_80819 || y_80845;
                    bool x_80847 = !cond_80772;
                    bool y_80848 = is_pt_marginal_res_80846 && x_80847;
                    bool defunc_0_op_res_80851 = y_80848 || redout_81568;
                    bool redout_tmp_84321 = defunc_0_op_res_80851;
                    
                    redout_81568 = redout_tmp_84321;
                }
                defunc_0_reduce_res_80770 = redout_81568;
                defunc_0_f_res_78139 = defunc_0_reduce_res_80770;
            } else {
                defunc_0_f_res_78139 = 0;
            }
            
            int64_t defunc_0_f_res_78225 = btoi_bool_i64(defunc_0_f_res_78139);
            int64_t defunc_0_op_res_74495 = add64(defunc_0_f_res_78225, scanacc_81573);
            
            ((int64_t *) mem_82760)[i_81577] = defunc_0_op_res_74495;
            ((int64_t *) mem_82762)[i_81577] = defunc_0_f_res_78225;
            ((bool *) mem_82763.mem)[i_81577] = defunc_0_reduce_res_80853;
            
            int64_t scanacc_tmp_84316 = defunc_0_op_res_74495;
            
            scanacc_81573 = scanacc_tmp_84316;
        }
        discard_81581 = scanacc_81573;
        
        int64_t bytes_82782 = (int64_t) 8 * loop_dz2086Uz2086U_73707;
        
        if (mem_82783_cached_sizze_84605 < bytes_82782) {
            err = lexical_realloc(ctx, &mem_82783, &mem_82783_cached_sizze_84605, bytes_82782);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_82785_cached_sizze_84606 < bytes_82782) {
            err = lexical_realloc(ctx, &mem_82785, &mem_82785_cached_sizze_84606, bytes_82782);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_82787_cached_sizze_84607 < bytes_82782) {
            err = lexical_realloc(ctx, &mem_82787, &mem_82787_cached_sizze_84607, bytes_82782);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_81594;
        int64_t discard_81595;
        int64_t scanacc_81585;
        int64_t scanacc_81586;
        
        scanacc_81585 = (int64_t) 0;
        scanacc_81586 = (int64_t) 0;
        for (int64_t i_81590 = 0; i_81590 < loop_dz2086Uz2086U_73707; i_81590++) {
            bool x_77944 = ((bool *) mem_param_82697.mem)[i_81590];
            bool cond_neg_77945 = !x_77944;
            int64_t defunc_0_lifted_pzq_res_77946 = btoi_bool_i64(cond_neg_77945);
            bool is_i_77947 = defunc_0_lifted_pzq_res_77946 == (int64_t) 0;
            int64_t part_res_77948 = btoi_bool_i64(is_i_77947);
            int64_t part_res_77949;
            int64_t part_res_77950;
            
            if (is_i_77947) {
                part_res_77949 = (int64_t) 0;
                part_res_77950 = (int64_t) 0;
            } else {
                bool is_i_77951 = defunc_0_lifted_pzq_res_77946 == (int64_t) 1;
                int64_t part_res_77952;
                
                if (is_i_77951) {
                    part_res_77952 = (int64_t) 1;
                } else {
                    part_res_77952 = (int64_t) 2;
                }
                
                int64_t part_res_77953 = btoi_bool_i64(is_i_77951);
                
                part_res_77949 = part_res_77952;
                part_res_77950 = part_res_77953;
            }
            
            int64_t zz_74194 = part_res_77948 + scanacc_81585;
            int64_t zz_74195 = part_res_77950 + scanacc_81586;
            
            ((int64_t *) mem_82783)[i_81590] = zz_74194;
            ((int64_t *) mem_82785)[i_81590] = zz_74195;
            ((int64_t *) mem_82787)[i_81590] = part_res_77949;
            
            int64_t scanacc_tmp_84322 = zz_74194;
            int64_t scanacc_tmp_84323 = zz_74195;
            
            scanacc_81585 = scanacc_tmp_84322;
            scanacc_81586 = scanacc_tmp_84323;
        }
        discard_81594 = scanacc_81585;
        discard_81595 = scanacc_81586;
        
        int64_t last_index_74196 = loop_dz2086Uz2086U_73707 - (int64_t) 1;
        bool is_empty_74197 = loop_dz2086Uz2086U_73707 == (int64_t) 0;
        bool x_74198 = !is_empty_74197;
        int64_t last_offset_74199;
        
        if (x_74198) {
            int64_t x_80854 = ((int64_t *) mem_82783)[last_index_74196];
            
            last_offset_74199 = x_80854;
        } else {
            last_offset_74199 = (int64_t) 0;
        }
        
        int64_t partition_sizze_74201;
        
        if (is_empty_74197) {
            partition_sizze_74201 = (int64_t) 0;
        } else {
            partition_sizze_74201 = last_offset_74199;
        }
        if (mem_82807_cached_sizze_84608 < bytes_82782) {
            err = lexical_realloc(ctx, &mem_82807, &mem_82807_cached_sizze_84608, bytes_82782);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_82809_cached_sizze_84609 < bytes_82782) {
            err = lexical_realloc(ctx, &mem_82809, &mem_82809_cached_sizze_84609, bytes_82782);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_82810_cached_sizze_84610 < loop_dz2086Uz2086U_73707) {
            err = lexical_realloc(ctx, &mem_82810, &mem_82810_cached_sizze_84610, loop_dz2086Uz2086U_73707);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_82812_cached_sizze_84611 < bytes_82782) {
            err = lexical_realloc(ctx, &mem_82812, &mem_82812_cached_sizze_84611, bytes_82782);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t y_74206 = (int64_t) -1 + partition_sizze_74201;
        
        for (int64_t write_iter_81596 = 0; write_iter_81596 < loop_dz2086Uz2086U_73707; write_iter_81596++) {
            int64_t write_iv_81601 = ((int64_t *) mem_82787)[write_iter_81596];
            int64_t write_iv_81602 = ((int64_t *) mem_82783)[write_iter_81596];
            int64_t write_iv_81603 = ((int64_t *) mem_82785)[write_iter_81596];
            double write_iv_81604 = ((double *) mem_param_82688.mem)[write_iter_81596];
            double write_iv_81605 = ((double *) mem_param_82691.mem)[write_iter_81596];
            bool write_iv_81606 = ((bool *) mem_param_82694.mem)[write_iter_81596];
            int64_t write_iv_81607 = ((int64_t *) mem_param_82685.mem)[write_iter_81596];
            bool is_this_one_74218 = write_iv_81601 == (int64_t) 0;
            bool is_this_one_74219 = write_iv_81601 == (int64_t) 1;
            int64_t this_offset_74220 = y_74206 + write_iv_81603;
            int64_t total_res_74221;
            
            if (is_this_one_74219) {
                total_res_74221 = this_offset_74220;
            } else {
                total_res_74221 = (int64_t) -1;
            }
            
            int64_t this_offset_74222 = (int64_t) -1 + write_iv_81602;
            int64_t total_res_74223;
            
            if (is_this_one_74218) {
                total_res_74223 = this_offset_74222;
            } else {
                total_res_74223 = total_res_74221;
            }
            if (sle64((int64_t) 0, total_res_74223) && slt64(total_res_74223, loop_dz2086Uz2086U_73707)) {
                ((double *) mem_82807)[total_res_74223] = write_iv_81604;
            }
            if (sle64((int64_t) 0, total_res_74223) && slt64(total_res_74223, loop_dz2086Uz2086U_73707)) {
                ((double *) mem_82809)[total_res_74223] = write_iv_81605;
            }
            if (sle64((int64_t) 0, total_res_74223) && slt64(total_res_74223, loop_dz2086Uz2086U_73707)) {
                ((bool *) mem_82810)[total_res_74223] = write_iv_81606;
            }
            if (sle64((int64_t) 0, total_res_74223) && slt64(total_res_74223, loop_dz2086Uz2086U_73707)) {
                ((int64_t *) mem_82812)[total_res_74223] = write_iv_81607;
            }
        }
        
        bool eq_x_zz_74224 = (int64_t) 0 == last_offset_74199;
        bool p_and_eq_x_y_74225 = x_74198 && eq_x_zz_74224;
        bool empty_slice_74226 = is_empty_74197 || p_and_eq_x_y_74225;
        int64_t m_74227 = sub64(partition_sizze_74201, (int64_t) 1);
        bool zzero_leq_i_p_m_t_s_74228 = sle64((int64_t) 0, m_74227);
        bool i_p_m_t_s_leq_w_74229 = slt64(m_74227, loop_dz2086Uz2086U_73707);
        bool i_lte_j_74230 = sle64((int64_t) 0, partition_sizze_74201);
        bool y_74231 = zzero_leq_i_p_m_t_s_74228 && i_p_m_t_s_leq_w_74229;
        bool forwards_ok_74232 = i_lte_j_74230 && y_74231;
        bool ok_or_empty_74233 = empty_slice_74226 || forwards_ok_74232;
        bool index_certs_74234;
        
        if (!ok_or_empty_74233) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) partition_sizze_74201, "] out of bounds for array of shape [", (long long) loop_dz2086Uz2086U_73707, "].", "-> #0  /prelude/soacs.fut:174:7-19\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t j_m_i_74235 = sub64(loop_dz2086Uz2086U_73707, partition_sizze_74201);
        bool empty_slice_74236 = j_m_i_74235 == (int64_t) 0;
        int64_t m_74237 = sub64(j_m_i_74235, (int64_t) 1);
        int64_t i_p_m_t_s_74238 = add64(partition_sizze_74201, m_74237);
        bool zzero_leq_i_p_m_t_s_74239 = sle64((int64_t) 0, i_p_m_t_s_74238);
        bool i_p_m_t_s_leq_w_74240 = slt64(i_p_m_t_s_74238, loop_dz2086Uz2086U_73707);
        bool i_lte_j_74241 = sle64(partition_sizze_74201, loop_dz2086Uz2086U_73707);
        bool y_74242 = zzero_leq_i_p_m_t_s_74239 && i_p_m_t_s_leq_w_74240;
        bool forwards_ok_74243 = i_lte_j_74241 && y_74242;
        bool ok_or_empty_74244 = empty_slice_74236 || forwards_ok_74243;
        bool index_certs_74245;
        
        if (!ok_or_empty_74244) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) partition_sizze_74201, ":", (long long) loop_dz2086Uz2086U_73707, "] out of bounds for array of shape [", (long long) loop_dz2086Uz2086U_73707, "].", "-> #0  /prelude/soacs.fut:174:21-33\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74249 = add64(loop_dz2085Uz2088U_73705, partition_sizze_74201);
        int64_t conc_tmp_74250 = loop_dz2085Uz2086U_73703 + partition_sizze_74201;
        int64_t conc_tmp_74251 = loop_dz2085Uz2087U_73704 + partition_sizze_74201;
        bool cond_74253 = slt64((int64_t) 0, loop_dz2086Uz2080U_73706);
        int64_t bytes_82837 = (int64_t) 8 * dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74249;
        
        if (mem_82838_cached_sizze_84612 < bytes_82837) {
            err = lexical_realloc(ctx, &mem_82838, &mem_82838_cached_sizze_84612, bytes_82837);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_82840_cached_sizze_84613 < bytes_82837) {
            err = lexical_realloc(ctx, &mem_82840, &mem_82840_cached_sizze_84613, bytes_82837);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_82842_cached_sizze_84614 < bytes_82837) {
            err = lexical_realloc(ctx, &mem_82842, &mem_82842_cached_sizze_84614, bytes_82837);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_81624;
        int64_t discard_81625;
        int64_t scanacc_81615;
        int64_t scanacc_81616;
        
        scanacc_81615 = (int64_t) 0;
        scanacc_81616 = (int64_t) 0;
        for (int64_t i_81620 = 0; i_81620 < dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74249; i_81620++) {
            bool index_concat_cmp_77830 = sle64(loop_dz2085Uz2088U_73705, i_81620);
            double index_concat_branch_77831;
            
            if (index_concat_cmp_77830) {
                int64_t index_concat_i_80855 = sub64(i_81620, loop_dz2085Uz2088U_73705);
                double index_concat_80856 = ((double *) mem_82807)[index_concat_i_80855];
                
                index_concat_branch_77831 = index_concat_80856;
            } else {
                double index_concat_77834 = ((double *) mem_param_82673.mem)[i_81620];
                
                index_concat_branch_77831 = index_concat_77834;
            }
            
            double index_concat_branch_77835;
            
            if (index_concat_cmp_77830) {
                int64_t index_concat_i_80857 = sub64(i_81620, loop_dz2085Uz2088U_73705);
                double index_concat_80858 = ((double *) mem_82809)[index_concat_i_80857];
                
                index_concat_branch_77835 = index_concat_80858;
            } else {
                double index_concat_77838 = ((double *) mem_param_82676.mem)[i_81620];
                
                index_concat_branch_77835 = index_concat_77838;
            }
            
            bool isNearAnyRelevant_77839;
            int64_t isNearAnyRelevant_77840;
            bool isNearAnyRelevant_77841;
            bool loop_while_77842;
            int64_t curPart_i_77843;
            bool isNear_77844;
            
            loop_while_77842 = cond_74253;
            curPart_i_77843 = (int64_t) 0;
            isNear_77844 = 0;
            while (loop_while_77842) {
                bool x_77845 = sle64((int64_t) 0, curPart_i_77843);
                bool y_77846 = slt64(curPart_i_77843, loop_dz2086Uz2080U_73706);
                bool bounds_check_77847 = x_77845 && y_77846;
                bool index_certs_77848;
                
                if (!bounds_check_77847) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) curPart_i_77843, "] out of bounds for array of shape [", (long long) loop_dz2086Uz2080U_73706, "].", "-> #0  ../../ftDBSCAN_plus.fut:418:79-110\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                int64_t curPart_77849 = ((int64_t *) mem_param_82682.mem)[curPart_i_77843];
                int64_t cur_xid_77850 = smod64(curPart_77849, (int64_t) 5);
                int64_t cur_yid_77851 = sdiv64(curPart_77849, (int64_t) 5);
                double i64_res_77852 = sitofp_i64_f64(cur_xid_77850);
                double zt_res_77853 = step_x_73122 * i64_res_77852;
                double zp_res_77854 = zt_res_77853 + defunc_0_reduce_res_81449;
                double i64_res_77855 = sitofp_i64_f64(cur_yid_77851);
                double zt_res_77856 = step_y_73124 * i64_res_77855;
                double zp_res_77857 = zt_res_77856 + defunc_0_reduce_res_81451;
                double zp_res_77858 = step_x_73122 + zp_res_77854;
                double zp_res_77859 = step_y_73124 + zp_res_77857;
                bool zgze_res_77860 = zp_res_77857 <= index_concat_branch_77835;
                bool zlze_res_77861 = index_concat_branch_77835 <= zp_res_77859;
                bool x_77862 = zgze_res_77860 && zlze_res_77861;
                bool zeze_res_77863 = index_concat_branch_77831 == zp_res_77854;
                double d_euclidean_res_77864;
                
                if (zeze_res_77863) {
                    double zm_res_80859 = index_concat_branch_77835 - index_concat_branch_77835;
                    double abs_res_80860 = fabs64(zm_res_80859);
                    
                    d_euclidean_res_77864 = abs_res_80860;
                } else {
                    double zm_res_77867 = index_concat_branch_77831 - zp_res_77854;
                    double abs_res_77868 = fabs64(zm_res_77867);
                    
                    d_euclidean_res_77864 = abs_res_77868;
                }
                
                bool zlze_res_77869 = d_euclidean_res_77864 <= 4.0;
                bool zeze_res_77870 = index_concat_branch_77831 == zp_res_77858;
                double d_euclidean_res_77871;
                
                if (zeze_res_77870) {
                    double zm_res_80861 = index_concat_branch_77835 - index_concat_branch_77835;
                    double abs_res_80862 = fabs64(zm_res_80861);
                    
                    d_euclidean_res_77871 = abs_res_80862;
                } else {
                    double zm_res_77874 = index_concat_branch_77831 - zp_res_77858;
                    double abs_res_77875 = fabs64(zm_res_77874);
                    
                    d_euclidean_res_77871 = abs_res_77875;
                }
                
                bool zlze_res_77876 = d_euclidean_res_77871 <= 4.0;
                bool x_77877 = !zlze_res_77869;
                bool y_77878 = zlze_res_77876 && x_77877;
                bool cond_t_res_77879 = zlze_res_77869 || y_77878;
                bool x_77880 = x_77862 && cond_t_res_77879;
                bool zgze_res_77881 = zp_res_77854 <= index_concat_branch_77831;
                bool zlze_res_77882 = index_concat_branch_77831 <= zp_res_77858;
                bool x_77883 = zgze_res_77881 && zlze_res_77882;
                double zm_res_77884 = index_concat_branch_77835 - zp_res_77857;
                double abs_res_77885 = fabs64(zm_res_77884);
                bool zlze_res_77886 = abs_res_77885 <= 4.0;
                double zm_res_77887 = index_concat_branch_77835 - zp_res_77859;
                double abs_res_77888 = fabs64(zm_res_77887);
                bool zlze_res_77889 = abs_res_77888 <= 4.0;
                bool x_77890 = !zlze_res_77886;
                bool y_77891 = zlze_res_77889 && x_77890;
                bool cond_f_res_t_res_77892 = zlze_res_77886 || y_77891;
                bool x_77893 = x_77883 && cond_f_res_t_res_77892;
                bool x_77894 = !x_77880;
                bool y_77895 = x_77893 && x_77894;
                bool cond_77896 = x_77880 || y_77895;
                bool zlze_res_77897 = index_concat_branch_77831 <= zp_res_77854;
                double corn_x_77898;
                
                if (zlze_res_77897) {
                    corn_x_77898 = zp_res_77854;
                } else {
                    corn_x_77898 = zp_res_77858;
                }
                
                bool zlze_res_77899 = index_concat_branch_77835 <= zp_res_77857;
                double corn_y_77900;
                
                if (zlze_res_77899) {
                    corn_y_77900 = zp_res_77857;
                } else {
                    corn_y_77900 = zp_res_77859;
                }
                
                bool p_and_eq_x_y_77901 = zeze_res_77863 && zlze_res_77897;
                bool not_p_77902 = !zlze_res_77897;
                bool p_and_eq_x_y_77903 = zeze_res_77870 && not_p_77902;
                bool zeze_res_77904 = p_and_eq_x_y_77901 || p_and_eq_x_y_77903;
                double d_euclidean_res_77905;
                
                if (zeze_res_77904) {
                    double zm_res_80863 = index_concat_branch_77835 - corn_y_77900;
                    double abs_res_80864 = fabs64(zm_res_80863);
                    
                    d_euclidean_res_77905 = abs_res_80864;
                } else {
                    bool eq_x_y_77908 = index_concat_branch_77835 == zp_res_77857;
                    bool eq_x_zz_77909 = index_concat_branch_77835 == zp_res_77859;
                    bool p_and_eq_x_y_77910 = zlze_res_77899 && eq_x_y_77908;
                    bool not_p_77911 = !zlze_res_77899;
                    bool p_and_eq_x_y_77912 = eq_x_zz_77909 && not_p_77911;
                    bool zeze_res_77913 = p_and_eq_x_y_77910 || p_and_eq_x_y_77912;
                    double d_euclidean_res_f_res_77914;
                    
                    if (zeze_res_77913) {
                        double zm_res_80865 = index_concat_branch_77831 - corn_x_77898;
                        double abs_res_80866 = fabs64(zm_res_80865);
                        
                        d_euclidean_res_f_res_77914 = abs_res_80866;
                    } else {
                        double zm_res_77917 = index_concat_branch_77835 - corn_y_77900;
                        double zm_res_77918 = index_concat_branch_77831 - corn_x_77898;
                        double hypot_res_77919 = futrts_hypot64(zm_res_77918, zm_res_77917);
                        
                        d_euclidean_res_f_res_77914 = hypot_res_77919;
                    }
                    d_euclidean_res_77905 = d_euclidean_res_f_res_77914;
                }
                
                bool zlze_res_77920 = d_euclidean_res_77905 <= 4.0;
                bool x_77921 = !cond_77896;
                bool y_77922 = zlze_res_77920 && x_77921;
                bool is_pt_marginal_res_77923 = cond_77896 || y_77922;
                int64_t tmp_77924 = add64((int64_t) 1, curPart_i_77843);
                bool cond_77925 = slt64(tmp_77924, loop_dz2086Uz2080U_73706);
                bool loop_cond_t_res_77926 = !is_pt_marginal_res_77923;
                bool x_77927 = cond_77925 && loop_cond_t_res_77926;
                bool loop_while_tmp_84336 = x_77927;
                int64_t curPart_i_tmp_84337 = tmp_77924;
                bool isNear_tmp_84338 = is_pt_marginal_res_77923;
                
                loop_while_77842 = loop_while_tmp_84336;
                curPart_i_77843 = curPart_i_tmp_84337;
                isNear_77844 = isNear_tmp_84338;
            }
            isNearAnyRelevant_77839 = loop_while_77842;
            isNearAnyRelevant_77840 = curPart_i_77843;
            isNearAnyRelevant_77841 = isNear_77844;
            
            bool cond_neg_77929 = !isNearAnyRelevant_77841;
            int64_t defunc_0_lifted_pzq_res_77930 = btoi_bool_i64(cond_neg_77929);
            bool is_i_77931 = defunc_0_lifted_pzq_res_77930 == (int64_t) 0;
            int64_t part_res_77932 = btoi_bool_i64(is_i_77931);
            int64_t part_res_77933;
            int64_t part_res_77934;
            
            if (is_i_77931) {
                part_res_77933 = (int64_t) 0;
                part_res_77934 = (int64_t) 0;
            } else {
                bool is_i_77935 = defunc_0_lifted_pzq_res_77930 == (int64_t) 1;
                int64_t part_res_77936;
                
                if (is_i_77935) {
                    part_res_77936 = (int64_t) 1;
                } else {
                    part_res_77936 = (int64_t) 2;
                }
                
                int64_t part_res_77937 = btoi_bool_i64(is_i_77935);
                
                part_res_77933 = part_res_77936;
                part_res_77934 = part_res_77937;
            }
            
            int64_t zz_74379 = part_res_77932 + scanacc_81615;
            int64_t zz_74380 = part_res_77934 + scanacc_81616;
            
            ((int64_t *) mem_82838)[i_81620] = zz_74379;
            ((int64_t *) mem_82840)[i_81620] = zz_74380;
            ((int64_t *) mem_82842)[i_81620] = part_res_77933;
            
            int64_t scanacc_tmp_84331 = zz_74379;
            int64_t scanacc_tmp_84332 = zz_74380;
            
            scanacc_81615 = scanacc_tmp_84331;
            scanacc_81616 = scanacc_tmp_84332;
        }
        discard_81624 = scanacc_81615;
        discard_81625 = scanacc_81616;
        
        int64_t last_index_74381 = dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74249 - (int64_t) 1;
        bool is_empty_74382 = dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74249 == (int64_t) 0;
        bool x_74383 = !is_empty_74382;
        int64_t last_offset_74384;
        
        if (x_74383) {
            int64_t x_80867 = ((int64_t *) mem_82838)[last_index_74381];
            
            last_offset_74384 = x_80867;
        } else {
            last_offset_74384 = (int64_t) 0;
        }
        
        int64_t partition_sizze_74386;
        
        if (is_empty_74382) {
            partition_sizze_74386 = (int64_t) 0;
        } else {
            partition_sizze_74386 = last_offset_74384;
        }
        if (mem_82862_cached_sizze_84615 < bytes_82837) {
            err = lexical_realloc(ctx, &mem_82862, &mem_82862_cached_sizze_84615, bytes_82837);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t y_74388 = (int64_t) -1 + partition_sizze_74386;
        
        for (int64_t write_iter_81626 = 0; write_iter_81626 < dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74249; write_iter_81626++) {
            int64_t write_iv_81628 = ((int64_t *) mem_82842)[write_iter_81626];
            int64_t write_iv_81629 = ((int64_t *) mem_82838)[write_iter_81626];
            int64_t write_iv_81630 = ((int64_t *) mem_82840)[write_iter_81626];
            bool is_this_one_74394 = write_iv_81628 == (int64_t) 0;
            bool is_this_one_74395 = write_iv_81628 == (int64_t) 1;
            int64_t this_offset_74396 = y_74388 + write_iv_81630;
            int64_t total_res_74397;
            
            if (is_this_one_74395) {
                total_res_74397 = this_offset_74396;
            } else {
                total_res_74397 = (int64_t) -1;
            }
            
            int64_t this_offset_74398 = (int64_t) -1 + write_iv_81629;
            int64_t total_res_74399;
            
            if (is_this_one_74394) {
                total_res_74399 = this_offset_74398;
            } else {
                total_res_74399 = total_res_74397;
            }
            if (sle64((int64_t) 0, total_res_74399) && slt64(total_res_74399, dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74249)) {
                ((int64_t *) mem_82862)[total_res_74399] = write_iter_81626;
            }
        }
        
        bool eq_x_zz_74400 = (int64_t) 0 == last_offset_74384;
        bool p_and_eq_x_y_74401 = x_74383 && eq_x_zz_74400;
        bool empty_slice_74402 = is_empty_74382 || p_and_eq_x_y_74401;
        int64_t m_74403 = sub64(partition_sizze_74386, (int64_t) 1);
        bool zzero_leq_i_p_m_t_s_74404 = sle64((int64_t) 0, m_74403);
        bool i_p_m_t_s_leq_w_74405 = slt64(m_74403, dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74249);
        bool i_lte_j_74406 = sle64((int64_t) 0, partition_sizze_74386);
        bool y_74407 = zzero_leq_i_p_m_t_s_74404 && i_p_m_t_s_leq_w_74405;
        bool forwards_ok_74408 = i_lte_j_74406 && y_74407;
        bool ok_or_empty_74409 = empty_slice_74402 || forwards_ok_74408;
        bool index_certs_74410;
        
        if (!ok_or_empty_74409) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) partition_sizze_74386, "] out of bounds for array of shape [", (long long) dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74249, "].", "-> #0  /prelude/soacs.fut:174:7-19\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_82869 = (int64_t) 8 * partition_sizze_74386;
        
        if (mem_82870_cached_sizze_84616 < bytes_82869) {
            err = lexical_realloc(ctx, &mem_82870, &mem_82870_cached_sizze_84616, bytes_82869);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_82872_cached_sizze_84617 < bytes_82869) {
            err = lexical_realloc(ctx, &mem_82872, &mem_82872_cached_sizze_84617, bytes_82869);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (memblock_alloc(ctx, &mem_82873, partition_sizze_74386, "mem_82873")) {
            err = 1;
            goto cleanup;
        }
        if (memblock_alloc(ctx, &mem_82875, bytes_82869, "mem_82875")) {
            err = 1;
            goto cleanup;
        }
        for (int64_t i_81641 = 0; i_81641 < partition_sizze_74386; i_81641++) {
            int64_t eta_p_74428 = ((int64_t *) mem_82862)[i_81641];
            bool x_74429 = sle64((int64_t) 0, eta_p_74428);
            bool y_74430 = slt64(eta_p_74428, dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74249);
            bool bounds_check_74431 = x_74429 && y_74430;
            bool index_certs_74432;
            
            if (!bounds_check_74431) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_74428, "] out of bounds for array of shape [", (long long) dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74249, "].", "-> #0  ../../ftDBSCAN_plus.fut:435:60-72\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../ftDBSCAN_plus.fut:435:44-102\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool y_74442 = slt64(eta_p_74428, conc_tmp_74250);
            bool bounds_check_74443 = x_74429 && y_74442;
            bool index_certs_74444;
            
            if (!bounds_check_74443) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_74428, "] out of bounds for array of shape [", (long long) conc_tmp_74250, "].", "-> #0  ../../ftDBSCAN_plus.fut:435:74-86\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../ftDBSCAN_plus.fut:435:44-102\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool y_74450 = slt64(eta_p_74428, conc_tmp_74251);
            bool bounds_check_74451 = x_74429 && y_74450;
            bool index_certs_74452;
            
            if (!bounds_check_74451) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_74428, "] out of bounds for array of shape [", (long long) conc_tmp_74251, "].", "-> #0  ../../ftDBSCAN_plus.fut:435:88-100\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../ftDBSCAN_plus.fut:435:44-102\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool index_concat_cmp_74433 = sle64(loop_dz2085Uz2088U_73705, eta_p_74428);
            double index_concat_branch_74434;
            
            if (index_concat_cmp_74433) {
                int64_t index_concat_i_80868 = sub64(eta_p_74428, loop_dz2085Uz2088U_73705);
                double index_concat_80869 = ((double *) mem_82807)[index_concat_i_80868];
                
                index_concat_branch_74434 = index_concat_80869;
            } else {
                double index_concat_74437 = ((double *) mem_param_82673.mem)[eta_p_74428];
                
                index_concat_branch_74434 = index_concat_74437;
            }
            
            double index_concat_branch_74438;
            
            if (index_concat_cmp_74433) {
                int64_t index_concat_i_80870 = sub64(eta_p_74428, loop_dz2085Uz2088U_73705);
                double index_concat_80871 = ((double *) mem_82809)[index_concat_i_80870];
                
                index_concat_branch_74438 = index_concat_80871;
            } else {
                double index_concat_74441 = ((double *) mem_param_82676.mem)[eta_p_74428];
                
                index_concat_branch_74438 = index_concat_74441;
            }
            
            bool index_concat_cmp_74445 = sle64(loop_dz2085Uz2086U_73703, eta_p_74428);
            bool index_concat_branch_74446;
            
            if (index_concat_cmp_74445) {
                int64_t index_concat_i_80872 = sub64(eta_p_74428, loop_dz2085Uz2086U_73703);
                bool index_concat_80873 = ((bool *) mem_82810)[index_concat_i_80872];
                
                index_concat_branch_74446 = index_concat_80873;
            } else {
                bool index_concat_74449 = ((bool *) mem_param_82667.mem)[eta_p_74428];
                
                index_concat_branch_74446 = index_concat_74449;
            }
            
            bool index_concat_cmp_74453 = sle64(loop_dz2085Uz2087U_73704, eta_p_74428);
            int64_t index_concat_branch_74454;
            
            if (index_concat_cmp_74453) {
                int64_t index_concat_i_80874 = sub64(eta_p_74428, loop_dz2085Uz2087U_73704);
                int64_t index_concat_80875 = ((int64_t *) mem_82812)[index_concat_i_80874];
                
                index_concat_branch_74454 = index_concat_80875;
            } else {
                int64_t index_concat_74457 = ((int64_t *) mem_param_82670.mem)[eta_p_74428];
                
                index_concat_branch_74454 = index_concat_74457;
            }
            ((double *) mem_82870)[i_81641] = index_concat_branch_74434;
            ((double *) mem_82872)[i_81641] = index_concat_branch_74438;
            ((bool *) mem_82873.mem)[i_81641] = index_concat_branch_74446;
            ((int64_t *) mem_82875.mem)[i_81641] = index_concat_branch_74454;
        }
        
        int64_t j_m_i_74412 = sub64(dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74249, partition_sizze_74386);
        bool empty_slice_74413 = j_m_i_74412 == (int64_t) 0;
        int64_t m_74414 = sub64(j_m_i_74412, (int64_t) 1);
        int64_t i_p_m_t_s_74415 = add64(partition_sizze_74386, m_74414);
        bool zzero_leq_i_p_m_t_s_74416 = sle64((int64_t) 0, i_p_m_t_s_74415);
        bool i_p_m_t_s_leq_w_74417 = slt64(i_p_m_t_s_74415, dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74249);
        bool i_lte_j_74418 = sle64(partition_sizze_74386, dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74249);
        bool y_74419 = zzero_leq_i_p_m_t_s_74416 && i_p_m_t_s_leq_w_74417;
        bool forwards_ok_74420 = i_lte_j_74418 && y_74419;
        bool ok_or_empty_74421 = empty_slice_74413 || forwards_ok_74420;
        bool index_certs_74422;
        
        if (!ok_or_empty_74421) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) partition_sizze_74386, ":", (long long) dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74249, "] out of bounds for array of shape [", (long long) dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74249, "].", "-> #0  /prelude/soacs.fut:174:21-33\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_82900 = (int64_t) 8 * j_m_i_74412;
        int64_t conc_tmp_74485 = j_m_i_74235 + j_m_i_74412;
        int64_t new_proc_74486 = add64((int64_t) 1, old_clHandler_73722);
        bool x_74496 = !empty_slice_73748;
        bool x_74497 = sle64((int64_t) 0, m_73749);
        bool y_74498 = slt64(m_73749, dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747);
        bool bounds_check_74499 = x_74497 && y_74498;
        bool protect_assert_disj_74500 = empty_slice_73748 || bounds_check_74499;
        bool index_certs_74501;
        
        if (!protect_assert_disj_74500) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) m_73749, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747, "].", "-> #0  /prelude/soacs.fut:257:33-47\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t m_f_res_74502;
        
        if (x_74496) {
            int64_t x_80882 = ((int64_t *) mem_82760)[m_73749];
            
            m_f_res_74502 = x_80882;
        } else {
            m_f_res_74502 = (int64_t) 0;
        }
        
        int64_t m_74504;
        
        if (empty_slice_73748) {
            m_74504 = (int64_t) 0;
        } else {
            m_74504 = m_f_res_74502;
        }
        
        int64_t m_74514 = sub64(m_74504, (int64_t) 1);
        bool i_p_m_t_s_leq_w_74516 = slt64(m_74514, dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747);
        bool zzero_leq_i_p_m_t_s_74515 = sle64((int64_t) 0, m_74514);
        bool y_74518 = zzero_leq_i_p_m_t_s_74515 && i_p_m_t_s_leq_w_74516;
        bool i_lte_j_74517 = sle64((int64_t) 0, m_74504);
        bool forwards_ok_74519 = i_lte_j_74517 && y_74518;
        bool eq_x_zz_74511 = (int64_t) 0 == m_f_res_74502;
        bool p_and_eq_x_y_74512 = x_74496 && eq_x_zz_74511;
        bool empty_slice_74513 = empty_slice_73748 || p_and_eq_x_y_74512;
        bool ok_or_empty_74520 = empty_slice_74513 || forwards_ok_74519;
        bool index_certs_74521;
        
        if (!ok_or_empty_74520) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_74504, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_82928 = (int64_t) 8 * m_74504;
        
        if (mem_82929_cached_sizze_84621 < bytes_82928) {
            err = lexical_realloc(ctx, &mem_82929, &mem_82929_cached_sizze_84621, bytes_82928);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_82929, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82619, (int64_t) 0 + (int64_t) 1 * inf_73735, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74504});
        if (mem_82931_cached_sizze_84622 < bytes_82928) {
            err = lexical_realloc(ctx, &mem_82931, &mem_82931_cached_sizze_84622, bytes_82928);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_82931, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82617, (int64_t) 0 + (int64_t) 1 * inf_73735, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74504});
        if (mem_82933_cached_sizze_84623 < bytes_82928) {
            err = lexical_realloc(ctx, &mem_82933, &mem_82933_cached_sizze_84623, bytes_82928);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t i_84344 = 0; i_84344 < m_74504; i_84344++) {
            int64_t x_84345 = (int64_t) 0 + i_84344 * (int64_t) 1;
            
            ((int64_t *) mem_82933)[i_84344] = x_84345;
        }
        for (int64_t write_iter_81656 = 0; write_iter_81656 < dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747; write_iter_81656++) {
            int64_t write_iv_81660 = ((int64_t *) mem_82762)[write_iter_81656];
            int64_t write_iv_81661 = ((int64_t *) mem_82760)[write_iter_81656];
            int64_t slice_82357 = inf_73735 + write_iter_81656;
            double write_iv_81663 = ((double *) mem_82617)[slice_82357];
            double write_iv_81664 = ((double *) mem_82619)[slice_82357];
            bool cond_77800 = write_iv_81660 == (int64_t) 1;
            int64_t lifted_lambda_res_77801;
            
            if (cond_77800) {
                int64_t lifted_lambda_res_t_res_80883 = sub64(write_iv_81661, (int64_t) 1);
                
                lifted_lambda_res_77801 = lifted_lambda_res_t_res_80883;
            } else {
                lifted_lambda_res_77801 = (int64_t) -1;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77801) && slt64(lifted_lambda_res_77801, m_74504)) {
                ((int64_t *) mem_82933)[lifted_lambda_res_77801] = write_iter_81656;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77801) && slt64(lifted_lambda_res_77801, m_74504)) {
                ((double *) mem_82931)[lifted_lambda_res_77801] = write_iv_81663;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77801) && slt64(lifted_lambda_res_77801, m_74504)) {
                ((double *) mem_82929)[lifted_lambda_res_77801] = write_iv_81664;
            }
        }
        
        bool not_p_74549 = !cond_73807;
        bool not_p_74548 = !cond_73797;
        
        if (mem_82953_cached_sizze_84624 < bytes_82869) {
            err = lexical_realloc(ctx, &mem_82953, &mem_82953_cached_sizze_84624, bytes_82869);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_82955_cached_sizze_84625 < bytes_82869) {
            err = lexical_realloc(ctx, &mem_82955, &mem_82955_cached_sizze_84625, bytes_82869);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_82957_cached_sizze_84626 < bytes_82869) {
            err = lexical_realloc(ctx, &mem_82957, &mem_82957_cached_sizze_84626, bytes_82869);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_82959_cached_sizze_84627 < bytes_82869) {
            err = lexical_realloc(ctx, &mem_82959, &mem_82959_cached_sizze_84627, bytes_82869);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_82960_cached_sizze_84628 < partition_sizze_74386) {
            err = lexical_realloc(ctx, &mem_82960, &mem_82960_cached_sizze_84628, partition_sizze_74386);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_82962_cached_sizze_84629 < bytes_82869) {
            err = lexical_realloc(ctx, &mem_82962, &mem_82962_cached_sizze_84629, bytes_82869);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_81688;
        int64_t scanacc_81674 = (int64_t) 0;
        
        for (int64_t i_81681 = 0; i_81681 < partition_sizze_74386; i_81681++) {
            double lifted_lambda_res_77702 = ((double *) mem_82870)[i_81681];
            double lifted_lambda_res_77703 = ((double *) mem_82872)[i_81681];
            bool tmp_77704 = ((bool *) mem_82873.mem)[i_81681];
            int64_t tmp_77705 = ((int64_t *) mem_82875.mem)[i_81681];
            bool zgze_res_77710 = minC_73806 <= lifted_lambda_res_77703;
            bool zlze_res_77711 = lifted_lambda_res_77703 <= maxC_73817;
            bool x_77712 = zgze_res_77710 && zlze_res_77711;
            bool eq_x_y_77713 = lifted_lambda_res_77702 == -INFINITY;
            bool eq_x_zz_77714 = lifted_lambda_res_77702 == zp_res_73800;
            bool p_and_eq_x_y_77715 = cond_73797 && eq_x_y_77713;
            bool p_and_eq_x_y_77716 = not_p_74548 && eq_x_zz_77714;
            bool zeze_res_77717 = p_and_eq_x_y_77715 || p_and_eq_x_y_77716;
            double d_euclidean_res_77718;
            
            if (zeze_res_77717) {
                double zm_res_80884 = lifted_lambda_res_77703 - lifted_lambda_res_77703;
                double abs_res_80885 = fabs64(zm_res_80884);
                
                d_euclidean_res_77718 = abs_res_80885;
            } else {
                double zm_res_77721 = lifted_lambda_res_77702 - minC_73801;
                double abs_res_77722 = fabs64(zm_res_77721);
                
                d_euclidean_res_77718 = abs_res_77722;
            }
            
            bool zlze_res_77723 = d_euclidean_res_77718 <= 4.0;
            bool eq_x_y_77724 = lifted_lambda_res_77702 == INFINITY;
            bool eq_x_zz_77725 = lifted_lambda_res_77702 == zp_res_73811;
            bool p_and_eq_x_y_77726 = cond_73807 && eq_x_y_77724;
            bool p_and_eq_x_y_77727 = not_p_74549 && eq_x_zz_77725;
            bool zeze_res_77728 = p_and_eq_x_y_77726 || p_and_eq_x_y_77727;
            double d_euclidean_res_77729;
            
            if (zeze_res_77728) {
                double zm_res_80886 = lifted_lambda_res_77703 - lifted_lambda_res_77703;
                double abs_res_80887 = fabs64(zm_res_80886);
                
                d_euclidean_res_77729 = abs_res_80887;
            } else {
                double zm_res_77732 = lifted_lambda_res_77702 - maxC_73812;
                double abs_res_77733 = fabs64(zm_res_77732);
                
                d_euclidean_res_77729 = abs_res_77733;
            }
            
            bool zlze_res_77734 = d_euclidean_res_77729 <= 4.0;
            bool x_77735 = !zlze_res_77723;
            bool y_77736 = zlze_res_77734 && x_77735;
            bool cond_t_res_77737 = zlze_res_77723 || y_77736;
            bool x_77738 = x_77712 && cond_t_res_77737;
            bool zgze_res_77739 = minC_73801 <= lifted_lambda_res_77702;
            bool zlze_res_77740 = lifted_lambda_res_77702 <= maxC_73812;
            bool x_77741 = zgze_res_77739 && zlze_res_77740;
            double zm_res_77742 = lifted_lambda_res_77703 - minC_73806;
            double abs_res_77743 = fabs64(zm_res_77742);
            bool zlze_res_77744 = abs_res_77743 <= 4.0;
            double zm_res_77745 = lifted_lambda_res_77703 - maxC_73817;
            double abs_res_77746 = fabs64(zm_res_77745);
            bool zlze_res_77747 = abs_res_77746 <= 4.0;
            bool x_77748 = !zlze_res_77744;
            bool y_77749 = zlze_res_77747 && x_77748;
            bool cond_f_res_t_res_77750 = zlze_res_77744 || y_77749;
            bool x_77751 = x_77741 && cond_f_res_t_res_77750;
            bool x_77752 = !x_77738;
            bool y_77753 = x_77751 && x_77752;
            bool cond_77754 = x_77738 || y_77753;
            bool zlze_res_77755 = lifted_lambda_res_77702 <= minC_73801;
            double corn_x_77756;
            
            if (zlze_res_77755) {
                corn_x_77756 = minC_73801;
            } else {
                corn_x_77756 = maxC_73812;
            }
            
            bool zlze_res_77757 = lifted_lambda_res_77703 <= minC_73806;
            double corn_y_77758;
            
            if (zlze_res_77757) {
                corn_y_77758 = minC_73806;
            } else {
                corn_y_77758 = maxC_73817;
            }
            
            bool p_and_eq_x_y_77759 = zeze_res_77717 && zlze_res_77755;
            bool not_p_77760 = !zlze_res_77755;
            bool p_and_eq_x_y_77761 = zeze_res_77728 && not_p_77760;
            bool zeze_res_77762 = p_and_eq_x_y_77759 || p_and_eq_x_y_77761;
            double d_euclidean_res_77763;
            
            if (zeze_res_77762) {
                double zm_res_80888 = lifted_lambda_res_77703 - corn_y_77758;
                double abs_res_80889 = fabs64(zm_res_80888);
                
                d_euclidean_res_77763 = abs_res_80889;
            } else {
                bool eq_x_y_77766 = lifted_lambda_res_77703 == -INFINITY;
                bool eq_x_zz_77767 = lifted_lambda_res_77703 == zp_res_73805;
                bool p_and_eq_x_y_77768 = cond_73802 && eq_x_y_77766;
                bool not_p_77769 = !cond_73802;
                bool p_and_eq_x_y_77770 = eq_x_zz_77767 && not_p_77769;
                bool eq_x_y_77771 = p_and_eq_x_y_77768 || p_and_eq_x_y_77770;
                bool eq_x_y_77772 = lifted_lambda_res_77703 == INFINITY;
                bool eq_x_zz_77773 = lifted_lambda_res_77703 == zp_res_73816;
                bool p_and_eq_x_y_77774 = cond_73813 && eq_x_y_77772;
                bool not_p_77775 = !cond_73813;
                bool p_and_eq_x_y_77776 = eq_x_zz_77773 && not_p_77775;
                bool eq_x_zz_77777 = p_and_eq_x_y_77774 || p_and_eq_x_y_77776;
                bool p_and_eq_x_y_77778 = zlze_res_77757 && eq_x_y_77771;
                bool not_p_77779 = !zlze_res_77757;
                bool p_and_eq_x_y_77780 = eq_x_zz_77777 && not_p_77779;
                bool zeze_res_77781 = p_and_eq_x_y_77778 || p_and_eq_x_y_77780;
                double d_euclidean_res_f_res_77782;
                
                if (zeze_res_77781) {
                    double zm_res_80890 = lifted_lambda_res_77702 - corn_x_77756;
                    double abs_res_80891 = fabs64(zm_res_80890);
                    
                    d_euclidean_res_f_res_77782 = abs_res_80891;
                } else {
                    double zm_res_77785 = lifted_lambda_res_77703 - corn_y_77758;
                    double zm_res_77786 = lifted_lambda_res_77702 - corn_x_77756;
                    double hypot_res_77787 = futrts_hypot64(zm_res_77786, zm_res_77785);
                    
                    d_euclidean_res_f_res_77782 = hypot_res_77787;
                }
                d_euclidean_res_77763 = d_euclidean_res_f_res_77782;
            }
            
            bool zlze_res_77788 = d_euclidean_res_77763 <= 4.0;
            bool x_77789 = !cond_77754;
            bool y_77790 = zlze_res_77788 && x_77789;
            bool is_pt_marginal_res_77791 = cond_77754 || y_77790;
            int64_t defunc_0_f_res_77792 = btoi_bool_i64(is_pt_marginal_res_77791);
            int64_t defunc_0_op_res_74640 = add64(defunc_0_f_res_77792, scanacc_81674);
            
            ((int64_t *) mem_82953)[i_81681] = defunc_0_op_res_74640;
            ((int64_t *) mem_82955)[i_81681] = defunc_0_f_res_77792;
            ((double *) mem_82957)[i_81681] = lifted_lambda_res_77702;
            ((double *) mem_82959)[i_81681] = lifted_lambda_res_77703;
            ((bool *) mem_82960)[i_81681] = tmp_77704;
            ((int64_t *) mem_82962)[i_81681] = tmp_77705;
            
            int64_t scanacc_tmp_84349 = defunc_0_op_res_74640;
            
            scanacc_81674 = scanacc_tmp_84349;
        }
        discard_81688 = scanacc_81674;
        
        bool x_74643 = !empty_slice_74402;
        bool y_74644 = slt64(m_74403, partition_sizze_74386);
        bool bounds_check_74645 = zzero_leq_i_p_m_t_s_74404 && y_74644;
        bool protect_assert_disj_74646 = empty_slice_74402 || bounds_check_74645;
        bool index_certs_74647;
        
        if (!protect_assert_disj_74646) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) m_74403, "] out of bounds for array of shape [", (long long) partition_sizze_74386, "].", "-> #0  /prelude/soacs.fut:257:33-47\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t m_f_res_74648;
        
        if (x_74643) {
            int64_t x_80892 = ((int64_t *) mem_82953)[m_74403];
            
            m_f_res_74648 = x_80892;
        } else {
            m_f_res_74648 = (int64_t) 0;
        }
        
        int64_t m_74650;
        
        if (empty_slice_74402) {
            m_74650 = (int64_t) 0;
        } else {
            m_74650 = m_f_res_74648;
        }
        
        int64_t m_74660 = sub64(m_74650, (int64_t) 1);
        bool i_p_m_t_s_leq_w_74662 = slt64(m_74660, partition_sizze_74386);
        bool zzero_leq_i_p_m_t_s_74661 = sle64((int64_t) 0, m_74660);
        bool y_74664 = zzero_leq_i_p_m_t_s_74661 && i_p_m_t_s_leq_w_74662;
        bool i_lte_j_74663 = sle64((int64_t) 0, m_74650);
        bool forwards_ok_74665 = i_lte_j_74663 && y_74664;
        bool eq_x_zz_74657 = (int64_t) 0 == m_f_res_74648;
        bool p_and_eq_x_y_74658 = x_74643 && eq_x_zz_74657;
        bool empty_slice_74659 = empty_slice_74402 || p_and_eq_x_y_74658;
        bool ok_or_empty_74666 = empty_slice_74659 || forwards_ok_74665;
        bool index_certs_74667;
        
        if (!ok_or_empty_74666) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_74650, "] out of bounds for array of shape [", (long long) partition_sizze_74386, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_82999 = (int64_t) 8 * m_74650;
        
        if (mem_83000_cached_sizze_84630 < bytes_82999) {
            err = lexical_realloc(ctx, &mem_83000, &mem_83000_cached_sizze_84630, bytes_82999);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t i_84356 = 0; i_84356 < m_74650; i_84356++) {
            int64_t x_84357 = (int64_t) 0 + i_84356 * (int64_t) 1;
            
            ((int64_t *) mem_83000)[i_84356] = x_84357;
        }
        if (mem_83002_cached_sizze_84631 < bytes_82999) {
            err = lexical_realloc(ctx, &mem_83002, &mem_83002_cached_sizze_84631, bytes_82999);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83002, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82962, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74650});
        if (mem_83003_cached_sizze_84632 < m_74650) {
            err = lexical_realloc(ctx, &mem_83003, &mem_83003_cached_sizze_84632, m_74650);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_1b(ctx, 1, (uint8_t *) mem_83003, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint8_t *) mem_82960, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74650});
        if (mem_83005_cached_sizze_84633 < bytes_82999) {
            err = lexical_realloc(ctx, &mem_83005, &mem_83005_cached_sizze_84633, bytes_82999);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83005, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82959, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74650});
        if (mem_83007_cached_sizze_84634 < bytes_82999) {
            err = lexical_realloc(ctx, &mem_83007, &mem_83007_cached_sizze_84634, bytes_82999);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83007, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82957, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74650});
        for (int64_t write_iter_81689 = 0; write_iter_81689 < partition_sizze_74386; write_iter_81689++) {
            int64_t write_iv_81695 = ((int64_t *) mem_82955)[write_iter_81689];
            int64_t write_iv_81696 = ((int64_t *) mem_82953)[write_iter_81689];
            double write_iv_81697 = ((double *) mem_82957)[write_iter_81689];
            double write_iv_81698 = ((double *) mem_82959)[write_iter_81689];
            bool write_iv_81699 = ((bool *) mem_82960)[write_iter_81689];
            int64_t write_iv_81700 = ((int64_t *) mem_82962)[write_iter_81689];
            bool cond_77602 = write_iv_81695 == (int64_t) 1;
            int64_t lifted_lambda_res_77603;
            
            if (cond_77602) {
                int64_t lifted_lambda_res_t_res_80893 = sub64(write_iv_81696, (int64_t) 1);
                
                lifted_lambda_res_77603 = lifted_lambda_res_t_res_80893;
            } else {
                lifted_lambda_res_77603 = (int64_t) -1;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77603) && slt64(lifted_lambda_res_77603, m_74650)) {
                ((double *) mem_83007)[lifted_lambda_res_77603] = write_iv_81697;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77603) && slt64(lifted_lambda_res_77603, m_74650)) {
                ((double *) mem_83005)[lifted_lambda_res_77603] = write_iv_81698;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77603) && slt64(lifted_lambda_res_77603, m_74650)) {
                ((bool *) mem_83003)[lifted_lambda_res_77603] = write_iv_81699;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77603) && slt64(lifted_lambda_res_77603, m_74650)) {
                ((int64_t *) mem_83002)[lifted_lambda_res_77603] = write_iv_81700;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77603) && slt64(lifted_lambda_res_77603, m_74650)) {
                ((int64_t *) mem_83000)[lifted_lambda_res_77603] = write_iter_81689;
            }
        }
        if (mem_83039_cached_sizze_84635 < bytes_82999) {
            err = lexical_realloc(ctx, &mem_83039, &mem_83039_cached_sizze_84635, bytes_82999);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83041_cached_sizze_84636 < bytes_82999) {
            err = lexical_realloc(ctx, &mem_83041, &mem_83041_cached_sizze_84636, bytes_82999);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83043_cached_sizze_84637 < bytes_82999) {
            err = lexical_realloc(ctx, &mem_83043, &mem_83043_cached_sizze_84637, bytes_82999);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_81719;
        int64_t discard_81720;
        int64_t scanacc_81710;
        int64_t scanacc_81711;
        
        scanacc_81710 = (int64_t) 0;
        scanacc_81711 = (int64_t) 0;
        for (int64_t i_81715 = 0; i_81715 < m_74650; i_81715++) {
            double x_77498 = ((double *) mem_83007)[i_81715];
            double x_77499 = ((double *) mem_83005)[i_81715];
            bool zgze_res_77500 = minC_73806 <= x_77499;
            bool zlze_res_77501 = x_77499 <= maxC_73817;
            bool x_77502 = zgze_res_77500 && zlze_res_77501;
            bool eq_x_y_77503 = x_77498 == -INFINITY;
            bool eq_x_zz_77504 = x_77498 == zp_res_73800;
            bool p_and_eq_x_y_77505 = cond_73797 && eq_x_y_77503;
            bool p_and_eq_x_y_77506 = not_p_74548 && eq_x_zz_77504;
            bool zeze_res_77507 = p_and_eq_x_y_77505 || p_and_eq_x_y_77506;
            double d_euclidean_res_77508;
            
            if (zeze_res_77507) {
                double zm_res_80894 = x_77499 - x_77499;
                double abs_res_80895 = fabs64(zm_res_80894);
                
                d_euclidean_res_77508 = abs_res_80895;
            } else {
                double zm_res_77511 = x_77498 - minC_73801;
                double abs_res_77512 = fabs64(zm_res_77511);
                
                d_euclidean_res_77508 = abs_res_77512;
            }
            
            bool zlze_res_77513 = d_euclidean_res_77508 <= 2.0;
            bool eq_x_y_77514 = x_77498 == INFINITY;
            bool eq_x_zz_77515 = x_77498 == zp_res_73811;
            bool p_and_eq_x_y_77516 = cond_73807 && eq_x_y_77514;
            bool p_and_eq_x_y_77517 = not_p_74549 && eq_x_zz_77515;
            bool zeze_res_77518 = p_and_eq_x_y_77516 || p_and_eq_x_y_77517;
            double d_euclidean_res_77519;
            
            if (zeze_res_77518) {
                double zm_res_80896 = x_77499 - x_77499;
                double abs_res_80897 = fabs64(zm_res_80896);
                
                d_euclidean_res_77519 = abs_res_80897;
            } else {
                double zm_res_77522 = x_77498 - maxC_73812;
                double abs_res_77523 = fabs64(zm_res_77522);
                
                d_euclidean_res_77519 = abs_res_77523;
            }
            
            bool zlze_res_77524 = d_euclidean_res_77519 <= 2.0;
            bool x_77525 = !zlze_res_77513;
            bool y_77526 = zlze_res_77524 && x_77525;
            bool cond_t_res_77527 = zlze_res_77513 || y_77526;
            bool x_77528 = x_77502 && cond_t_res_77527;
            bool zgze_res_77529 = minC_73801 <= x_77498;
            bool zlze_res_77530 = x_77498 <= maxC_73812;
            bool x_77531 = zgze_res_77529 && zlze_res_77530;
            double zm_res_77532 = x_77499 - minC_73806;
            double abs_res_77533 = fabs64(zm_res_77532);
            bool zlze_res_77534 = abs_res_77533 <= 2.0;
            double zm_res_77535 = x_77499 - maxC_73817;
            double abs_res_77536 = fabs64(zm_res_77535);
            bool zlze_res_77537 = abs_res_77536 <= 2.0;
            bool x_77538 = !zlze_res_77534;
            bool y_77539 = zlze_res_77537 && x_77538;
            bool cond_f_res_t_res_77540 = zlze_res_77534 || y_77539;
            bool x_77541 = x_77531 && cond_f_res_t_res_77540;
            bool x_77542 = !x_77528;
            bool y_77543 = x_77541 && x_77542;
            bool cond_77544 = x_77528 || y_77543;
            bool zlze_res_77545 = x_77498 <= minC_73801;
            double corn_x_77546;
            
            if (zlze_res_77545) {
                corn_x_77546 = minC_73801;
            } else {
                corn_x_77546 = maxC_73812;
            }
            
            bool zlze_res_77547 = x_77499 <= minC_73806;
            double corn_y_77548;
            
            if (zlze_res_77547) {
                corn_y_77548 = minC_73806;
            } else {
                corn_y_77548 = maxC_73817;
            }
            
            bool p_and_eq_x_y_77549 = zeze_res_77507 && zlze_res_77545;
            bool not_p_77550 = !zlze_res_77545;
            bool p_and_eq_x_y_77551 = zeze_res_77518 && not_p_77550;
            bool zeze_res_77552 = p_and_eq_x_y_77549 || p_and_eq_x_y_77551;
            double d_euclidean_res_77553;
            
            if (zeze_res_77552) {
                double zm_res_80898 = x_77499 - corn_y_77548;
                double abs_res_80899 = fabs64(zm_res_80898);
                
                d_euclidean_res_77553 = abs_res_80899;
            } else {
                bool eq_x_y_77556 = x_77499 == -INFINITY;
                bool eq_x_zz_77557 = x_77499 == zp_res_73805;
                bool p_and_eq_x_y_77558 = cond_73802 && eq_x_y_77556;
                bool not_p_77559 = !cond_73802;
                bool p_and_eq_x_y_77560 = eq_x_zz_77557 && not_p_77559;
                bool eq_x_y_77561 = p_and_eq_x_y_77558 || p_and_eq_x_y_77560;
                bool eq_x_y_77562 = x_77499 == INFINITY;
                bool eq_x_zz_77563 = x_77499 == zp_res_73816;
                bool p_and_eq_x_y_77564 = cond_73813 && eq_x_y_77562;
                bool not_p_77565 = !cond_73813;
                bool p_and_eq_x_y_77566 = eq_x_zz_77563 && not_p_77565;
                bool eq_x_zz_77567 = p_and_eq_x_y_77564 || p_and_eq_x_y_77566;
                bool p_and_eq_x_y_77568 = zlze_res_77547 && eq_x_y_77561;
                bool not_p_77569 = !zlze_res_77547;
                bool p_and_eq_x_y_77570 = eq_x_zz_77567 && not_p_77569;
                bool zeze_res_77571 = p_and_eq_x_y_77568 || p_and_eq_x_y_77570;
                double d_euclidean_res_f_res_77572;
                
                if (zeze_res_77571) {
                    double zm_res_80900 = x_77498 - corn_x_77546;
                    double abs_res_80901 = fabs64(zm_res_80900);
                    
                    d_euclidean_res_f_res_77572 = abs_res_80901;
                } else {
                    double zm_res_77575 = x_77499 - corn_y_77548;
                    double zm_res_77576 = x_77498 - corn_x_77546;
                    double hypot_res_77577 = futrts_hypot64(zm_res_77576, zm_res_77575);
                    
                    d_euclidean_res_f_res_77572 = hypot_res_77577;
                }
                d_euclidean_res_77553 = d_euclidean_res_f_res_77572;
            }
            
            bool zlze_res_77578 = d_euclidean_res_77553 <= 2.0;
            bool x_77579 = !cond_77544;
            bool y_77580 = zlze_res_77578 && x_77579;
            bool is_pt_marginal_res_77581 = cond_77544 || y_77580;
            bool cond_neg_77582 = !is_pt_marginal_res_77581;
            int64_t defunc_0_lifted_pzq_res_77583 = btoi_bool_i64(cond_neg_77582);
            bool is_i_77584 = defunc_0_lifted_pzq_res_77583 == (int64_t) 0;
            int64_t part_res_77585 = btoi_bool_i64(is_i_77584);
            int64_t part_res_77586;
            int64_t part_res_77587;
            
            if (is_i_77584) {
                part_res_77586 = (int64_t) 0;
                part_res_77587 = (int64_t) 0;
            } else {
                bool is_i_77588 = defunc_0_lifted_pzq_res_77583 == (int64_t) 1;
                int64_t part_res_77589;
                
                if (is_i_77588) {
                    part_res_77589 = (int64_t) 1;
                } else {
                    part_res_77589 = (int64_t) 2;
                }
                
                int64_t part_res_77590 = btoi_bool_i64(is_i_77588);
                
                part_res_77586 = part_res_77589;
                part_res_77587 = part_res_77590;
            }
            
            int64_t zz_74792 = part_res_77585 + scanacc_81710;
            int64_t zz_74793 = part_res_77587 + scanacc_81711;
            
            ((int64_t *) mem_83039)[i_81715] = zz_74792;
            ((int64_t *) mem_83041)[i_81715] = zz_74793;
            ((int64_t *) mem_83043)[i_81715] = part_res_77586;
            
            int64_t scanacc_tmp_84363 = zz_74792;
            int64_t scanacc_tmp_84364 = zz_74793;
            
            scanacc_81710 = scanacc_tmp_84363;
            scanacc_81711 = scanacc_tmp_84364;
        }
        discard_81719 = scanacc_81710;
        discard_81720 = scanacc_81711;
        
        bool x_74794 = !empty_slice_74659;
        int64_t last_offset_74795;
        
        if (x_74794) {
            int64_t x_80902 = ((int64_t *) mem_83039)[m_74660];
            
            last_offset_74795 = x_80902;
        } else {
            last_offset_74795 = (int64_t) 0;
        }
        
        int64_t partition_sizze_74797;
        
        if (empty_slice_74659) {
            partition_sizze_74797 = (int64_t) 0;
        } else {
            partition_sizze_74797 = last_offset_74795;
        }
        if (mem_83063_cached_sizze_84638 < bytes_82999) {
            err = lexical_realloc(ctx, &mem_83063, &mem_83063_cached_sizze_84638, bytes_82999);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83065_cached_sizze_84639 < bytes_82999) {
            err = lexical_realloc(ctx, &mem_83065, &mem_83065_cached_sizze_84639, bytes_82999);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83066_cached_sizze_84640 < m_74650) {
            err = lexical_realloc(ctx, &mem_83066, &mem_83066_cached_sizze_84640, m_74650);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83068_cached_sizze_84641 < bytes_82999) {
            err = lexical_realloc(ctx, &mem_83068, &mem_83068_cached_sizze_84641, bytes_82999);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83070_cached_sizze_84642 < bytes_82999) {
            err = lexical_realloc(ctx, &mem_83070, &mem_83070_cached_sizze_84642, bytes_82999);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t y_74803 = (int64_t) -1 + partition_sizze_74797;
        
        for (int64_t write_iter_81721 = 0; write_iter_81721 < m_74650; write_iter_81721++) {
            int64_t write_iv_81727 = ((int64_t *) mem_83043)[write_iter_81721];
            int64_t write_iv_81728 = ((int64_t *) mem_83039)[write_iter_81721];
            int64_t write_iv_81729 = ((int64_t *) mem_83041)[write_iter_81721];
            double write_iv_81730 = ((double *) mem_83007)[write_iter_81721];
            double write_iv_81731 = ((double *) mem_83005)[write_iter_81721];
            bool write_iv_81732 = ((bool *) mem_83003)[write_iter_81721];
            int64_t write_iv_81733 = ((int64_t *) mem_83002)[write_iter_81721];
            int64_t write_iv_81734 = ((int64_t *) mem_83000)[write_iter_81721];
            bool is_this_one_74817 = write_iv_81727 == (int64_t) 0;
            bool is_this_one_74818 = write_iv_81727 == (int64_t) 1;
            int64_t this_offset_74819 = y_74803 + write_iv_81729;
            int64_t total_res_74820;
            
            if (is_this_one_74818) {
                total_res_74820 = this_offset_74819;
            } else {
                total_res_74820 = (int64_t) -1;
            }
            
            int64_t this_offset_74821 = (int64_t) -1 + write_iv_81728;
            int64_t total_res_74822;
            
            if (is_this_one_74817) {
                total_res_74822 = this_offset_74821;
            } else {
                total_res_74822 = total_res_74820;
            }
            if (sle64((int64_t) 0, total_res_74822) && slt64(total_res_74822, m_74650)) {
                ((double *) mem_83063)[total_res_74822] = write_iv_81730;
            }
            if (sle64((int64_t) 0, total_res_74822) && slt64(total_res_74822, m_74650)) {
                ((double *) mem_83065)[total_res_74822] = write_iv_81731;
            }
            if (sle64((int64_t) 0, total_res_74822) && slt64(total_res_74822, m_74650)) {
                ((bool *) mem_83066)[total_res_74822] = write_iv_81732;
            }
            if (sle64((int64_t) 0, total_res_74822) && slt64(total_res_74822, m_74650)) {
                ((int64_t *) mem_83068)[total_res_74822] = write_iv_81733;
            }
            if (sle64((int64_t) 0, total_res_74822) && slt64(total_res_74822, m_74650)) {
                ((int64_t *) mem_83070)[total_res_74822] = write_iv_81734;
            }
        }
        
        bool eq_x_zz_74823 = (int64_t) 0 == last_offset_74795;
        bool p_and_eq_x_y_74824 = x_74794 && eq_x_zz_74823;
        bool empty_slice_74825 = empty_slice_74659 || p_and_eq_x_y_74824;
        int64_t m_74826 = sub64(partition_sizze_74797, (int64_t) 1);
        bool zzero_leq_i_p_m_t_s_74827 = sle64((int64_t) 0, m_74826);
        bool i_p_m_t_s_leq_w_74828 = slt64(m_74826, m_74650);
        bool i_lte_j_74829 = sle64((int64_t) 0, partition_sizze_74797);
        bool y_74830 = zzero_leq_i_p_m_t_s_74827 && i_p_m_t_s_leq_w_74828;
        bool forwards_ok_74831 = i_lte_j_74829 && y_74830;
        bool ok_or_empty_74832 = empty_slice_74825 || forwards_ok_74831;
        bool index_certs_74833;
        
        if (!ok_or_empty_74832) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) partition_sizze_74797, "] out of bounds for array of shape [", (long long) m_74650, "].", "-> #0  /prelude/soacs.fut:174:7-19\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        bool y_74894 = slt64(m_74826, partition_sizze_74797);
        bool bounds_check_74895 = zzero_leq_i_p_m_t_s_74827 && y_74894;
        bool protect_assert_disj_74896 = empty_slice_74825 || bounds_check_74895;
        bool index_certs_74897;
        
        if (!protect_assert_disj_74896) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) m_74826, "] out of bounds for array of shape [", (long long) partition_sizze_74797, "].", "-> #0  /prelude/soacs.fut:257:33-47\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        bool x_74893 = !empty_slice_74825;
        int64_t bytes_83101 = (int64_t) 8 * partition_sizze_74797;
        
        if (memblock_alloc(ctx, &mem_83102, bytes_83101, "mem_83102")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83102.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83063, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {partition_sizze_74797});
        if (memblock_alloc(ctx, &mem_83105, bytes_83101, "mem_83105")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83105.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83065, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {partition_sizze_74797});
        if (memblock_alloc(ctx, &mem_83108, bytes_82928, "mem_83108")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83108.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82931, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74504});
        if (memblock_alloc(ctx, &mem_83111, bytes_82928, "mem_83111")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83111.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82929, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74504});
        if (futrts_get_num_neighbours_against_12429(ctx, &ext_mem_83113, mem_83102, mem_83105, mem_83108, mem_83111, partition_sizze_74797, m_74504, 2.0, (int8_t) 0, (int8_t) 1, 0.0, (int64_t) 9223372036854775807) != 0) {
            err = 1;
            goto cleanup;
        }
        if (memblock_unref(ctx, &mem_83102, "mem_83102") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83105, "mem_83105") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83108, "mem_83108") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83111, "mem_83111") != 0)
            return 1;
        
        int64_t j_m_i_74838 = sub64(m_74650, partition_sizze_74797);
        int64_t m_74840 = sub64(j_m_i_74838, (int64_t) 1);
        int64_t i_p_m_t_s_74841 = add64(partition_sizze_74797, m_74840);
        bool i_p_m_t_s_leq_w_74843 = slt64(i_p_m_t_s_74841, m_74650);
        bool zzero_leq_i_p_m_t_s_74842 = sle64((int64_t) 0, i_p_m_t_s_74841);
        bool y_74845 = zzero_leq_i_p_m_t_s_74842 && i_p_m_t_s_leq_w_74843;
        bool i_lte_j_74844 = sle64(partition_sizze_74797, m_74650);
        bool forwards_ok_74846 = i_lte_j_74844 && y_74845;
        bool empty_slice_74839 = j_m_i_74838 == (int64_t) 0;
        bool ok_or_empty_74847 = empty_slice_74839 || forwards_ok_74846;
        bool index_certs_74848;
        
        if (!ok_or_empty_74847) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) partition_sizze_74797, ":", (long long) m_74650, "] out of bounds for array of shape [", (long long) m_74650, "].", "-> #0  /prelude/soacs.fut:174:21-33\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        if (memblock_alloc(ctx, &mem_83115, bytes_83101, "mem_83115")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83115.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83063, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {partition_sizze_74797});
        if (memblock_alloc(ctx, &mem_83118, bytes_83101, "mem_83118")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83118.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83065, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {partition_sizze_74797});
        
        int64_t bytes_83120 = (int64_t) 8 * j_m_i_74838;
        
        if (memblock_alloc(ctx, &mem_83121, bytes_83120, "mem_83121")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83121.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83063, (int64_t) 0 + (int64_t) 1 * partition_sizze_74797, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_74838});
        if (memblock_alloc(ctx, &mem_83124, bytes_83120, "mem_83124")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83124.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83065, (int64_t) 0 + (int64_t) 1 * partition_sizze_74797, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_74838});
        if (futrts_get_num_neighbours_against_12429(ctx, &ext_mem_83126, mem_83115, mem_83118, mem_83121, mem_83124, partition_sizze_74797, j_m_i_74838, 2.0, (int8_t) 0, (int8_t) 1, 0.0, (int64_t) 9223372036854775807) != 0) {
            err = 1;
            goto cleanup;
        }
        if (memblock_unref(ctx, &mem_83115, "mem_83115") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83118, "mem_83118") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83121, "mem_83121") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83124, "mem_83124") != 0)
            return 1;
        if (memblock_alloc(ctx, &mem_83128, bytes_83101, "mem_83128")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83128.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83063, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {partition_sizze_74797});
        if (memblock_alloc(ctx, &mem_83131, bytes_83101, "mem_83131")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83131.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83065, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {partition_sizze_74797});
        if (memblock_alloc(ctx, &mem_83134, bytes_83101, "mem_83134")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83134.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83063, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {partition_sizze_74797});
        if (memblock_alloc(ctx, &mem_83137, bytes_83101, "mem_83137")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83137.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83065, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {partition_sizze_74797});
        if (futrts_get_num_neighbours_against_12426(ctx, &ext_mem_83139, mem_83128, mem_83131, mem_83134, mem_83137, partition_sizze_74797, partition_sizze_74797, 2.0, (int8_t) 0, (int8_t) 1, 0.0, (int64_t) 9223372036854775807) != 0) {
            err = 1;
            goto cleanup;
        }
        if (memblock_unref(ctx, &mem_83128, "mem_83128") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83131, "mem_83131") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83134, "mem_83134") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83137, "mem_83137") != 0)
            return 1;
        if (mem_83141_cached_sizze_84643 < bytes_83101) {
            err = lexical_realloc(ctx, &mem_83141, &mem_83141_cached_sizze_84643, bytes_83101);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83143_cached_sizze_84644 < bytes_83101) {
            err = lexical_realloc(ctx, &mem_83143, &mem_83143_cached_sizze_84644, bytes_83101);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83144_cached_sizze_84645 < partition_sizze_74797) {
            err = lexical_realloc(ctx, &mem_83144, &mem_83144_cached_sizze_84645, partition_sizze_74797);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_81751;
        int64_t scanacc_81743 = (int64_t) 0;
        
        for (int64_t i_81747 = 0; i_81747 < partition_sizze_74797; i_81747++) {
            int64_t eta_p_77483 = ((int64_t *) ext_mem_83139.mem)[i_81747];
            int64_t eta_p_77484 = ((int64_t *) ext_mem_83126.mem)[i_81747];
            int64_t eta_p_77485 = ((int64_t *) ext_mem_83113.mem)[i_81747];
            int64_t lifted_lambda_res_77486 = sub64(eta_p_77483, (int64_t) 1);
            int64_t zp_lhs_77488 = add64(eta_p_77484, lifted_lambda_res_77486);
            int64_t lifted_lambda_res_77489 = add64(eta_p_77485, zp_lhs_77488);
            bool lifted_lambda_res_77491 = sle64((int64_t) 4, lifted_lambda_res_77489);
            int64_t defunc_0_f_res_77493 = btoi_bool_i64(lifted_lambda_res_77491);
            int64_t defunc_0_op_res_74892 = add64(defunc_0_f_res_77493, scanacc_81743);
            
            ((int64_t *) mem_83141)[i_81747] = defunc_0_op_res_74892;
            ((int64_t *) mem_83143)[i_81747] = defunc_0_f_res_77493;
            ((bool *) mem_83144)[i_81747] = lifted_lambda_res_77491;
            
            int64_t scanacc_tmp_84373 = defunc_0_op_res_74892;
            
            scanacc_81743 = scanacc_tmp_84373;
        }
        discard_81751 = scanacc_81743;
        if (memblock_unref(ctx, &ext_mem_83113, "ext_mem_83113") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_83126, "ext_mem_83126") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_83139, "ext_mem_83139") != 0)
            return 1;
        
        int64_t m_f_res_74898;
        
        if (x_74893) {
            int64_t x_80903 = ((int64_t *) mem_83141)[m_74826];
            
            m_f_res_74898 = x_80903;
        } else {
            m_f_res_74898 = (int64_t) 0;
        }
        
        int64_t m_74900;
        
        if (empty_slice_74825) {
            m_74900 = (int64_t) 0;
        } else {
            m_74900 = m_f_res_74898;
        }
        
        int64_t m_74910 = sub64(m_74900, (int64_t) 1);
        bool i_p_m_t_s_leq_w_74912 = slt64(m_74910, partition_sizze_74797);
        bool zzero_leq_i_p_m_t_s_74911 = sle64((int64_t) 0, m_74910);
        bool y_74914 = zzero_leq_i_p_m_t_s_74911 && i_p_m_t_s_leq_w_74912;
        bool i_lte_j_74913 = sle64((int64_t) 0, m_74900);
        bool forwards_ok_74915 = i_lte_j_74913 && y_74914;
        bool eq_x_zz_74907 = (int64_t) 0 == m_f_res_74898;
        bool p_and_eq_x_y_74908 = x_74893 && eq_x_zz_74907;
        bool empty_slice_74909 = empty_slice_74825 || p_and_eq_x_y_74908;
        bool ok_or_empty_74916 = empty_slice_74909 || forwards_ok_74915;
        bool index_certs_74917;
        
        if (!ok_or_empty_74916) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_74900, "] out of bounds for array of shape [", (long long) partition_sizze_74797, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_83163 = (int64_t) 8 * m_74900;
        
        if (mem_83164_cached_sizze_84646 < bytes_83163) {
            err = lexical_realloc(ctx, &mem_83164, &mem_83164_cached_sizze_84646, bytes_83163);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83164, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83068, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74900});
        if (mem_83166_cached_sizze_84647 < bytes_83163) {
            err = lexical_realloc(ctx, &mem_83166, &mem_83166_cached_sizze_84647, bytes_83163);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83166, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83065, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74900});
        if (mem_83168_cached_sizze_84648 < bytes_83163) {
            err = lexical_realloc(ctx, &mem_83168, &mem_83168_cached_sizze_84648, bytes_83163);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83168, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83063, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74900});
        for (int64_t write_iter_81752 = 0; write_iter_81752 < partition_sizze_74797; write_iter_81752++) {
            int64_t write_iv_81756 = ((int64_t *) mem_83143)[write_iter_81752];
            int64_t write_iv_81757 = ((int64_t *) mem_83141)[write_iter_81752];
            double write_iv_81758 = ((double *) mem_83063)[write_iter_81752];
            double write_iv_81759 = ((double *) mem_83065)[write_iter_81752];
            int64_t write_iv_81760 = ((int64_t *) mem_83068)[write_iter_81752];
            bool cond_77431 = write_iv_81756 == (int64_t) 1;
            int64_t lifted_lambda_res_77432;
            
            if (cond_77431) {
                int64_t lifted_lambda_res_t_res_80904 = sub64(write_iv_81757, (int64_t) 1);
                
                lifted_lambda_res_77432 = lifted_lambda_res_t_res_80904;
            } else {
                lifted_lambda_res_77432 = (int64_t) -1;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77432) && slt64(lifted_lambda_res_77432, m_74900)) {
                ((double *) mem_83168)[lifted_lambda_res_77432] = write_iv_81758;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77432) && slt64(lifted_lambda_res_77432, m_74900)) {
                ((double *) mem_83166)[lifted_lambda_res_77432] = write_iv_81759;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77432) && slt64(lifted_lambda_res_77432, m_74900)) {
                ((int64_t *) mem_83164)[lifted_lambda_res_77432] = write_iv_81760;
            }
        }
        if (mem_83188_cached_sizze_84649 < bytes_83120) {
            err = lexical_realloc(ctx, &mem_83188, &mem_83188_cached_sizze_84649, bytes_83120);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83190_cached_sizze_84650 < bytes_83120) {
            err = lexical_realloc(ctx, &mem_83190, &mem_83190_cached_sizze_84650, bytes_83120);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_81772;
        int64_t scanacc_81766 = (int64_t) 0;
        
        for (int64_t i_81769 = 0; i_81769 < j_m_i_74838; i_81769++) {
            int64_t slice_82351 = partition_sizze_74797 + i_81769;
            bool eta_p_77409 = ((bool *) mem_83066)[slice_82351];
            int64_t defunc_0_f_res_77410 = btoi_bool_i64(eta_p_77409);
            int64_t defunc_0_op_res_74983 = add64(defunc_0_f_res_77410, scanacc_81766);
            
            ((int64_t *) mem_83188)[i_81769] = defunc_0_op_res_74983;
            ((int64_t *) mem_83190)[i_81769] = defunc_0_f_res_77410;
            
            int64_t scanacc_tmp_84380 = defunc_0_op_res_74983;
            
            scanacc_81766 = scanacc_tmp_84380;
        }
        discard_81772 = scanacc_81766;
        
        bool y_74972 = slt64(m_74840, j_m_i_74838);
        bool x_74971 = sle64((int64_t) 0, m_74840);
        bool bounds_check_74973 = x_74971 && y_74972;
        bool protect_assert_disj_74974 = empty_slice_74839 || bounds_check_74973;
        bool index_certs_74975;
        
        if (!protect_assert_disj_74974) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) m_74840, "] out of bounds for array of shape [", (long long) j_m_i_74838, "].", "-> #0  /prelude/soacs.fut:257:33-47\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        bool x_74970 = !empty_slice_74839;
        int64_t m_f_res_74984;
        
        if (x_74970) {
            int64_t x_80905 = ((int64_t *) mem_83188)[m_74840];
            
            m_f_res_74984 = x_80905;
        } else {
            m_f_res_74984 = (int64_t) 0;
        }
        
        int64_t m_74986;
        
        if (empty_slice_74839) {
            m_74986 = (int64_t) 0;
        } else {
            m_74986 = m_f_res_74984;
        }
        
        int64_t m_74996 = sub64(m_74986, (int64_t) 1);
        bool i_p_m_t_s_leq_w_74998 = slt64(m_74996, j_m_i_74838);
        bool zzero_leq_i_p_m_t_s_74997 = sle64((int64_t) 0, m_74996);
        bool y_75000 = zzero_leq_i_p_m_t_s_74997 && i_p_m_t_s_leq_w_74998;
        bool i_lte_j_74999 = sle64((int64_t) 0, m_74986);
        bool forwards_ok_75001 = i_lte_j_74999 && y_75000;
        bool eq_x_zz_74993 = (int64_t) 0 == m_f_res_74984;
        bool p_and_eq_x_y_74994 = x_74970 && eq_x_zz_74993;
        bool empty_slice_74995 = empty_slice_74839 || p_and_eq_x_y_74994;
        bool ok_or_empty_75002 = empty_slice_74995 || forwards_ok_75001;
        bool index_certs_75003;
        
        if (!ok_or_empty_75002) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_74986, "] out of bounds for array of shape [", (long long) j_m_i_74838, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_83203 = (int64_t) 8 * m_74986;
        
        if (mem_83204_cached_sizze_84651 < bytes_83203) {
            err = lexical_realloc(ctx, &mem_83204, &mem_83204_cached_sizze_84651, bytes_83203);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83204, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83068, (int64_t) 0 + (int64_t) 1 * partition_sizze_74797, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74986});
        if (mem_83206_cached_sizze_84652 < bytes_83203) {
            err = lexical_realloc(ctx, &mem_83206, &mem_83206_cached_sizze_84652, bytes_83203);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83206, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83065, (int64_t) 0 + (int64_t) 1 * partition_sizze_74797, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74986});
        if (mem_83208_cached_sizze_84653 < bytes_83203) {
            err = lexical_realloc(ctx, &mem_83208, &mem_83208_cached_sizze_84653, bytes_83203);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83208, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83063, (int64_t) 0 + (int64_t) 1 * partition_sizze_74797, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74986});
        for (int64_t write_iter_81773 = 0; write_iter_81773 < j_m_i_74838; write_iter_81773++) {
            int64_t write_iv_81777 = ((int64_t *) mem_83190)[write_iter_81773];
            int64_t write_iv_81778 = ((int64_t *) mem_83188)[write_iter_81773];
            int64_t slice_82350 = partition_sizze_74797 + write_iter_81773;
            double write_iv_81779 = ((double *) mem_83063)[slice_82350];
            double write_iv_81780 = ((double *) mem_83065)[slice_82350];
            int64_t write_iv_81781 = ((int64_t *) mem_83068)[slice_82350];
            bool cond_77404 = write_iv_81777 == (int64_t) 1;
            int64_t lifted_lambda_res_77405;
            
            if (cond_77404) {
                int64_t lifted_lambda_res_t_res_80906 = sub64(write_iv_81778, (int64_t) 1);
                
                lifted_lambda_res_77405 = lifted_lambda_res_t_res_80906;
            } else {
                lifted_lambda_res_77405 = (int64_t) -1;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77405) && slt64(lifted_lambda_res_77405, m_74986)) {
                ((double *) mem_83208)[lifted_lambda_res_77405] = write_iv_81779;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77405) && slt64(lifted_lambda_res_77405, m_74986)) {
                ((double *) mem_83206)[lifted_lambda_res_77405] = write_iv_81780;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77405) && slt64(lifted_lambda_res_77405, m_74986)) {
                ((int64_t *) mem_83204)[lifted_lambda_res_77405] = write_iv_81781;
            }
        }
        if (futrts_get_num_neighbours_against_12426(ctx, &ext_mem_83227, mem_82701, mem_82703, mem_82701, mem_82703, dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747, dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747, 2.0, (int8_t) 0, (int8_t) 1, 0.0, (int64_t) 9223372036854775807) != 0) {
            err = 1;
            goto cleanup;
        }
        if (mem_83229_cached_sizze_84654 < bytes_82700) {
            err = lexical_realloc(ctx, &mem_83229, &mem_83229_cached_sizze_84654, bytes_82700);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t nest_i_84386 = 0; nest_i_84386 < dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747; nest_i_84386++) {
            ((int64_t *) mem_83229)[nest_i_84386] = (int64_t) 0;
        }
        if (memblock_alloc(ctx, &mem_83231, bytes_82928, "mem_83231")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83231.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82931, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74504});
        if (memblock_alloc(ctx, &mem_83234, bytes_82928, "mem_83234")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83234.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82929, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74504});
        if (memblock_alloc(ctx, &mem_83237, bytes_83101, "mem_83237")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83237.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83063, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {partition_sizze_74797});
        if (memblock_alloc(ctx, &mem_83240, bytes_83101, "mem_83240")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83240.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83065, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {partition_sizze_74797});
        if (futrts_get_num_neighbours_against_12429(ctx, &ext_mem_83242, mem_83231, mem_83234, mem_83237, mem_83240, m_74504, partition_sizze_74797, 2.0, (int8_t) 0, (int8_t) 1, 0.0, (int64_t) 9223372036854775807) != 0) {
            err = 1;
            goto cleanup;
        }
        if (memblock_unref(ctx, &mem_83231, "mem_83231") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83234, "mem_83234") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83237, "mem_83237") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83240, "mem_83240") != 0)
            return 1;
        for (int64_t write_iter_81785 = 0; write_iter_81785 < m_74504; write_iter_81785++) {
            int64_t write_iv_81787 = ((int64_t *) mem_82933)[write_iter_81785];
            int64_t write_iv_81788 = ((int64_t *) ext_mem_83242.mem)[write_iter_81785];
            
            if (sle64((int64_t) 0, write_iv_81787) && slt64(write_iv_81787, dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747)) {
                ((int64_t *) mem_83229)[write_iv_81787] = write_iv_81788;
            }
        }
        if (memblock_unref(ctx, &ext_mem_83242, "ext_mem_83242") != 0)
            return 1;
        if (mem_83250_cached_sizze_84655 < bytes_82700) {
            err = lexical_realloc(ctx, &mem_83250, &mem_83250_cached_sizze_84655, bytes_82700);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83252_cached_sizze_84656 < bytes_82700) {
            err = lexical_realloc(ctx, &mem_83252, &mem_83252_cached_sizze_84656, bytes_82700);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (memblock_alloc(ctx, &mem_83253, dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747, "mem_83253")) {
            err = 1;
            goto cleanup;
        }
        
        int64_t discard_81801;
        int64_t scanacc_81793 = (int64_t) 0;
        
        for (int64_t i_81797 = 0; i_81797 < dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747; i_81797++) {
            int64_t eta_p_77455 = ((int64_t *) ext_mem_83227.mem)[i_81797];
            int64_t eta_p_77456 = ((int64_t *) mem_83229)[i_81797];
            int64_t lifted_lambda_res_77457 = sub64(eta_p_77455, (int64_t) 1);
            int64_t defunc_0_f_res_77459 = add64(eta_p_77456, lifted_lambda_res_77457);
            bool lifted_lambda_res_77461 = sle64((int64_t) 4, defunc_0_f_res_77459);
            int64_t defunc_0_f_res_77463 = btoi_bool_i64(lifted_lambda_res_77461);
            int64_t defunc_0_op_res_74938 = add64(defunc_0_f_res_77463, scanacc_81793);
            
            ((int64_t *) mem_83250)[i_81797] = defunc_0_op_res_74938;
            ((int64_t *) mem_83252)[i_81797] = defunc_0_f_res_77463;
            ((bool *) mem_83253.mem)[i_81797] = lifted_lambda_res_77461;
            
            int64_t scanacc_tmp_84388 = defunc_0_op_res_74938;
            
            scanacc_81793 = scanacc_tmp_84388;
        }
        discard_81801 = scanacc_81793;
        if (memblock_unref(ctx, &ext_mem_83227, "ext_mem_83227") != 0)
            return 1;
        
        int64_t m_f_res_74939;
        
        if (x_74496) {
            int64_t x_80907 = ((int64_t *) mem_83250)[m_73749];
            
            m_f_res_74939 = x_80907;
        } else {
            m_f_res_74939 = (int64_t) 0;
        }
        
        int64_t m_74941;
        
        if (empty_slice_73748) {
            m_74941 = (int64_t) 0;
        } else {
            m_74941 = m_f_res_74939;
        }
        
        int64_t m_74951 = sub64(m_74941, (int64_t) 1);
        bool i_p_m_t_s_leq_w_74953 = slt64(m_74951, dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747);
        bool zzero_leq_i_p_m_t_s_74952 = sle64((int64_t) 0, m_74951);
        bool y_74955 = zzero_leq_i_p_m_t_s_74952 && i_p_m_t_s_leq_w_74953;
        bool i_lte_j_74954 = sle64((int64_t) 0, m_74941);
        bool forwards_ok_74956 = i_lte_j_74954 && y_74955;
        bool eq_x_zz_74948 = (int64_t) 0 == m_f_res_74939;
        bool p_and_eq_x_y_74949 = x_74496 && eq_x_zz_74948;
        bool empty_slice_74950 = empty_slice_73748 || p_and_eq_x_y_74949;
        bool ok_or_empty_74957 = empty_slice_74950 || forwards_ok_74956;
        bool index_certs_74958;
        
        if (!ok_or_empty_74957) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_74941, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_83272 = (int64_t) 8 * m_74941;
        
        if (mem_83273_cached_sizze_84657 < bytes_83272) {
            err = lexical_realloc(ctx, &mem_83273, &mem_83273_cached_sizze_84657, bytes_83272);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t nest_i_84392 = 0; nest_i_84392 < m_74941; nest_i_84392++) {
            ((int64_t *) mem_83273)[nest_i_84392] = (int64_t) -1;
        }
        if (mem_83275_cached_sizze_84658 < bytes_83272) {
            err = lexical_realloc(ctx, &mem_83275, &mem_83275_cached_sizze_84658, bytes_83272);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83275, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82619, (int64_t) 0 + (int64_t) 1 * inf_73735, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74941});
        if (mem_83277_cached_sizze_84659 < bytes_83272) {
            err = lexical_realloc(ctx, &mem_83277, &mem_83277_cached_sizze_84659, bytes_83272);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83277, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82617, (int64_t) 0 + (int64_t) 1 * inf_73735, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74941});
        for (int64_t write_iter_81802 = 0; write_iter_81802 < dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747; write_iter_81802++) {
            int64_t write_iv_81806 = ((int64_t *) mem_83252)[write_iter_81802];
            int64_t write_iv_81807 = ((int64_t *) mem_83250)[write_iter_81802];
            int64_t slice_82347 = inf_73735 + write_iter_81802;
            double write_iv_81808 = ((double *) mem_82617)[slice_82347];
            double write_iv_81809 = ((double *) mem_82619)[slice_82347];
            bool cond_77417 = write_iv_81806 == (int64_t) 1;
            int64_t lifted_lambda_res_77418;
            
            if (cond_77417) {
                int64_t lifted_lambda_res_t_res_80908 = sub64(write_iv_81807, (int64_t) 1);
                
                lifted_lambda_res_77418 = lifted_lambda_res_t_res_80908;
            } else {
                lifted_lambda_res_77418 = (int64_t) -1;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77418) && slt64(lifted_lambda_res_77418, m_74941)) {
                ((double *) mem_83277)[lifted_lambda_res_77418] = write_iv_81808;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77418) && slt64(lifted_lambda_res_77418, m_74941)) {
                ((double *) mem_83275)[lifted_lambda_res_77418] = write_iv_81809;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77418) && slt64(lifted_lambda_res_77418, m_74941)) {
                ((int64_t *) mem_83273)[lifted_lambda_res_77418] = (int64_t) -1;
            }
        }
        
        int64_t conc_tmp_75017 = m_74900 + m_74941;
        int64_t conc_tmp_75018 = m_74986 + conc_tmp_75017;
        int64_t bytes_83296 = (int64_t) 8 * conc_tmp_75018;
        
        if (mem_83297_cached_sizze_84660 < bytes_83296) {
            err = lexical_realloc(ctx, &mem_83297, &mem_83297_cached_sizze_84660, bytes_83296);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t tmp_offs_84396 = (int64_t) 0;
        
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83297, tmp_offs_84396, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83273, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74941});
        tmp_offs_84396 += m_74941;
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83297, tmp_offs_84396, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83164, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74900});
        tmp_offs_84396 += m_74900;
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83297, tmp_offs_84396, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83204, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_74986});
        tmp_offs_84396 += m_74986;
        
        int64_t max_res_75024 = smax64((int64_t) 1, conc_tmp_75018);
        bool zzero_75025 = max_res_75024 == (int64_t) 0;
        bool nonzzero_75026 = !zzero_75025;
        bool nonzzero_cert_75027;
        
        if (!nonzzero_75026) {
            set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../ftDBSCAN_plus.fut:636:63-77\n   #1  test_dbscan_plus.fut:14:23-19:30\n   #2  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t max_arg1_75028 = sdiv64((int64_t) 9223372036854775807, max_res_75024);
        int64_t max_res_75029 = smax64((int64_t) 1, max_arg1_75028);
        int64_t zm_lhs_75030 = add64(conc_tmp_75018, max_res_75029);
        int64_t zs_lhs_75031 = sub64(zm_lhs_75030, (int64_t) 1);
        bool zzero_75032 = max_res_75029 == (int64_t) 0;
        bool nonzzero_75033 = !zzero_75032;
        bool nonzzero_cert_75034;
        
        if (!nonzzero_75033) {
            set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../ftDBSCAN_plus.fut:558:58-65\n   #1  test_dbscan_plus.fut:14:23-19:30\n   #2  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t inner_iter_75035 = sdiv64(zs_lhs_75031, max_res_75029);
        
        if (mem_83299_cached_sizze_84661 < bytes_83296) {
            err = lexical_realloc(ctx, &mem_83299, &mem_83299_cached_sizze_84661, bytes_83296);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t nest_i_84397 = 0; nest_i_84397 < conc_tmp_75018; nest_i_84397++) {
            ((int64_t *) mem_83299)[nest_i_84397] = (int64_t) 0;
        }
        
        bool bounds_invalid_upwards_75037 = slt64(inner_iter_75035, (int64_t) 0);
        bool valid_75038 = !bounds_invalid_upwards_75037;
        bool range_valid_c_75039;
        
        if (!valid_75038) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) inner_iter_75035, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  ../../ftDBSCAN_plus.fut:561:42-57\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        if (mem_83301_cached_sizze_84662 < bytes_83296) {
            err = lexical_realloc(ctx, &mem_83301, &mem_83301_cached_sizze_84662, bytes_83296);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t i_84398 = 0; i_84398 < conc_tmp_75018; i_84398++) {
            int64_t x_84399 = (int64_t) 0 + i_84398 * (int64_t) 1;
            
            ((int64_t *) mem_83301)[i_84398] = x_84399;
        }
        
        int64_t mk_adjacency_list_res_75041;
        int64_t loop_dz2089U_75045;
        
        if (memblock_set(ctx, &mem_param_83307, &mem_82640, "mem_82640") != 0)
            return 1;
        loop_dz2089U_75045 = (int64_t) 0;
        for (int64_t i_75044 = 0; i_75044 < inner_iter_75035; i_75044++) {
            int64_t inf_75048 = mul64(max_res_75029, i_75044);
            int64_t min_arg1_75049 = add64(max_res_75029, inf_75048);
            int64_t min_res_75050 = smin64(conc_tmp_75018, min_arg1_75049);
            int64_t dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_75051 = sub64(min_res_75050, inf_75048);
            int64_t dzlz7bUZLztZRz20UZLZLzmZRz20Usupz20UinfZRz20Unz7dUzg_75052 = mul64(conc_tmp_75018, dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_75051);
            bool empty_slice_75053 = dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_75051 == (int64_t) 0;
            int64_t m_75054 = sub64(dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_75051, (int64_t) 1);
            int64_t i_p_m_t_s_75055 = add64(inf_75048, m_75054);
            bool zzero_leq_i_p_m_t_s_75056 = sle64((int64_t) 0, i_p_m_t_s_75055);
            bool i_p_m_t_s_leq_w_75057 = slt64(i_p_m_t_s_75055, conc_tmp_75018);
            bool zzero_lte_i_75058 = sle64((int64_t) 0, inf_75048);
            bool i_lte_j_75059 = sle64(inf_75048, min_res_75050);
            bool y_75060 = i_p_m_t_s_leq_w_75057 && zzero_lte_i_75058;
            bool y_75061 = zzero_leq_i_p_m_t_s_75056 && y_75060;
            bool forwards_ok_75062 = i_lte_j_75059 && y_75061;
            bool ok_or_empty_75063 = empty_slice_75053 || forwards_ok_75062;
            bool index_certs_75064;
            
            if (!ok_or_empty_75063) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) inf_75048, ":", (long long) min_res_75050, "] out of bounds for array of shape [", (long long) conc_tmp_75018, "].", "-> #0  ../../ftDBSCAN_plus.fut:564:56-73\n   #1  test_dbscan_plus.fut:14:23-19:30\n   #2  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool bounds_invalid_upwards_75065 = slt64(min_res_75050, inf_75048);
            bool valid_75066 = !bounds_invalid_upwards_75065;
            bool range_valid_c_75067;
            
            if (!valid_75066) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) inf_75048, "..<", (long long) min_res_75050, " is invalid.", "-> #0  ../../ftDBSCAN_plus.fut:566:57-66\n   #1  test_dbscan_plus.fut:14:23-19:30\n   #2  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t binop_x_83308 = (int64_t) 8 * dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_75051;
            int64_t bytes_83309 = conc_tmp_75018 * binop_x_83308;
            
            if (mem_83310_cached_sizze_84663 < bytes_83309) {
                err = lexical_realloc(ctx, &mem_83310, &mem_83310_cached_sizze_84663, bytes_83309);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            if (mem_83312_cached_sizze_84664 < dzlz7bUZLztZRz20UZLZLzmZRz20Usupz20UinfZRz20Unz7dUzg_75052) {
                err = lexical_realloc(ctx, &mem_83312, &mem_83312_cached_sizze_84664, dzlz7bUZLztZRz20UZLZLzmZRz20Usupz20UinfZRz20Unz7dUzg_75052);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            for (int64_t i_81821 = 0; i_81821 < dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_75051; i_81821++) {
                int64_t index_primexp_82345 = add64(inf_75048, i_81821);
                bool index_concat_cmp_82335 = sle64(conc_tmp_75017, index_primexp_82345);
                double index_concat_branch_82343;
                
                if (index_concat_cmp_82335) {
                    int64_t index_concat_i_82336 = sub64(index_primexp_82345, conc_tmp_75017);
                    double index_concat_82337 = ((double *) mem_83208)[index_concat_i_82336];
                    
                    index_concat_branch_82343 = index_concat_82337;
                } else {
                    bool index_concat_cmp_82338 = sle64(m_74941, index_primexp_82345);
                    double index_concat_branch_82342;
                    
                    if (index_concat_cmp_82338) {
                        int64_t index_concat_i_82339 = sub64(index_primexp_82345, m_74941);
                        double index_concat_82340 = ((double *) mem_83168)[index_concat_i_82339];
                        
                        index_concat_branch_82342 = index_concat_82340;
                    } else {
                        double index_concat_82341 = ((double *) mem_83277)[index_primexp_82345];
                        
                        index_concat_branch_82342 = index_concat_82341;
                    }
                    index_concat_branch_82343 = index_concat_branch_82342;
                }
                
                double index_concat_branch_82331;
                
                if (index_concat_cmp_82335) {
                    int64_t index_concat_i_82324 = sub64(index_primexp_82345, conc_tmp_75017);
                    double index_concat_82325 = ((double *) mem_83206)[index_concat_i_82324];
                    
                    index_concat_branch_82331 = index_concat_82325;
                } else {
                    bool index_concat_cmp_82326 = sle64(m_74941, index_primexp_82345);
                    double index_concat_branch_82330;
                    
                    if (index_concat_cmp_82326) {
                        int64_t index_concat_i_82327 = sub64(index_primexp_82345, m_74941);
                        double index_concat_82328 = ((double *) mem_83166)[index_concat_i_82327];
                        
                        index_concat_branch_82330 = index_concat_82328;
                    } else {
                        double index_concat_82329 = ((double *) mem_83275)[index_primexp_82345];
                        
                        index_concat_branch_82330 = index_concat_82329;
                    }
                    index_concat_branch_82331 = index_concat_branch_82330;
                }
                for (int64_t nest_i_84406 = 0; nest_i_84406 < conc_tmp_75018; nest_i_84406++) {
                    ((int64_t *) mem_83310)[i_81821 * conc_tmp_75018 + nest_i_84406] = index_primexp_82345;
                }
                for (int64_t i_81815 = 0; i_81815 < conc_tmp_75018; i_81815++) {
                    bool index_concat_cmp_82309 = sle64(conc_tmp_75017, i_81815);
                    double index_concat_branch_82317;
                    
                    if (index_concat_cmp_82309) {
                        int64_t index_concat_i_82310 = sub64(i_81815, conc_tmp_75017);
                        double index_concat_82311 = ((double *) mem_83208)[index_concat_i_82310];
                        
                        index_concat_branch_82317 = index_concat_82311;
                    } else {
                        bool index_concat_cmp_82312 = sle64(m_74941, i_81815);
                        double index_concat_branch_82316;
                        
                        if (index_concat_cmp_82312) {
                            int64_t index_concat_i_82313 = sub64(i_81815, m_74941);
                            double index_concat_82314 = ((double *) mem_83168)[index_concat_i_82313];
                            
                            index_concat_branch_82316 = index_concat_82314;
                        } else {
                            double index_concat_82315 = ((double *) mem_83277)[i_81815];
                            
                            index_concat_branch_82316 = index_concat_82315;
                        }
                        index_concat_branch_82317 = index_concat_branch_82316;
                    }
                    
                    double index_concat_branch_82306;
                    
                    if (index_concat_cmp_82309) {
                        int64_t index_concat_i_82299 = sub64(i_81815, conc_tmp_75017);
                        double index_concat_82300 = ((double *) mem_83206)[index_concat_i_82299];
                        
                        index_concat_branch_82306 = index_concat_82300;
                    } else {
                        bool index_concat_cmp_82301 = sle64(m_74941, i_81815);
                        double index_concat_branch_82305;
                        
                        if (index_concat_cmp_82301) {
                            int64_t index_concat_i_82302 = sub64(i_81815, m_74941);
                            double index_concat_82303 = ((double *) mem_83166)[index_concat_i_82302];
                            
                            index_concat_branch_82305 = index_concat_82303;
                        } else {
                            double index_concat_82304 = ((double *) mem_83275)[i_81815];
                            
                            index_concat_branch_82305 = index_concat_82304;
                        }
                        index_concat_branch_82306 = index_concat_branch_82305;
                    }
                    
                    bool cond_75082 = index_primexp_82345 == i_81815;
                    bool zeze_res_75083 = index_concat_branch_82343 == index_concat_branch_82317;
                    double d_euclidean_res_75084;
                    
                    if (zeze_res_75083) {
                        double zm_res_80909 = index_concat_branch_82331 - index_concat_branch_82306;
                        double abs_res_80910 = fabs64(zm_res_80909);
                        
                        d_euclidean_res_75084 = abs_res_80910;
                    } else {
                        bool zeze_res_75087 = index_concat_branch_82331 == index_concat_branch_82306;
                        double d_euclidean_res_f_res_75088;
                        
                        if (zeze_res_75087) {
                            double zm_res_80911 = index_concat_branch_82343 - index_concat_branch_82317;
                            double abs_res_80912 = fabs64(zm_res_80911);
                            
                            d_euclidean_res_f_res_75088 = abs_res_80912;
                        } else {
                            double zm_res_75091 = index_concat_branch_82331 - index_concat_branch_82306;
                            double zm_res_75092 = index_concat_branch_82343 - index_concat_branch_82317;
                            double hypot_res_75093 = futrts_hypot64(zm_res_75092, zm_res_75091);
                            
                            d_euclidean_res_f_res_75088 = hypot_res_75093;
                        }
                        d_euclidean_res_75084 = d_euclidean_res_f_res_75088;
                    }
                    
                    bool zlze_res_75094 = d_euclidean_res_75084 <= 2.0;
                    bool x_75095 = !cond_75082;
                    bool y_75096 = zlze_res_75094 && x_75095;
                    bool lifted_lambda_res_75097 = cond_75082 || y_75096;
                    
                    ((bool *) mem_83312)[i_81821 * conc_tmp_75018 + i_81815] = lifted_lambda_res_75097;
                }
            }
            
            int64_t bytes_83345 = (int64_t) 8 * dzlz7bUZLztZRz20UZLZLzmZRz20Usupz20UinfZRz20Unz7dUzg_75052;
            
            if (mem_83346_cached_sizze_84667 < bytes_83345) {
                err = lexical_realloc(ctx, &mem_83346, &mem_83346_cached_sizze_84667, bytes_83345);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            if (mem_83348_cached_sizze_84668 < bytes_83345) {
                err = lexical_realloc(ctx, &mem_83348, &mem_83348_cached_sizze_84668, bytes_83345);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            
            int64_t discard_81832;
            int64_t scanacc_81826 = (int64_t) 0;
            
            for (int64_t i_81829 = 0; i_81829 < dzlz7bUZLztZRz20UZLZLzmZRz20Usupz20UinfZRz20Unz7dUzg_75052; i_81829++) {
                int64_t new_index_82292 = squot64(i_81829, conc_tmp_75018);
                int64_t binop_y_82294 = conc_tmp_75018 * new_index_82292;
                int64_t new_index_82295 = i_81829 - binop_y_82294;
                bool eta_p_78993 = ((bool *) mem_83312)[new_index_82292 * conc_tmp_75018 + new_index_82295];
                int64_t defunc_0_f_res_78994 = btoi_bool_i64(eta_p_78993);
                int64_t defunc_0_op_res_75108 = add64(defunc_0_f_res_78994, scanacc_81826);
                
                ((int64_t *) mem_83346)[i_81829] = defunc_0_op_res_75108;
                ((int64_t *) mem_83348)[i_81829] = defunc_0_f_res_78994;
                
                int64_t scanacc_tmp_84408 = defunc_0_op_res_75108;
                
                scanacc_81826 = scanacc_tmp_84408;
            }
            discard_81832 = scanacc_81826;
            
            bool cond_75109 = dzlz7bUZLztZRz20UZLZLzmZRz20Usupz20UinfZRz20Unz7dUzg_75052 == (int64_t) 0;
            bool x_75110 = !cond_75109;
            int64_t tmp_75111 = sub64(dzlz7bUZLztZRz20UZLZLzmZRz20Usupz20UinfZRz20Unz7dUzg_75052, (int64_t) 1);
            bool x_75112 = sle64((int64_t) 0, tmp_75111);
            bool y_75113 = slt64(tmp_75111, dzlz7bUZLztZRz20UZLZLzmZRz20Usupz20UinfZRz20Unz7dUzg_75052);
            bool bounds_check_75114 = x_75112 && y_75113;
            bool protect_assert_disj_75115 = cond_75109 || bounds_check_75114;
            bool index_certs_75116;
            
            if (!protect_assert_disj_75115) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_75111, "] out of bounds for array of shape [", (long long) dzlz7bUZLztZRz20UZLZLzmZRz20Usupz20UinfZRz20Unz7dUzg_75052, "].", "-> #0  /prelude/soacs.fut:257:33-47\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t m_f_res_75117;
            
            if (x_75110) {
                int64_t x_80914 = ((int64_t *) mem_83346)[tmp_75111];
                
                m_f_res_75117 = x_80914;
            } else {
                m_f_res_75117 = (int64_t) 0;
            }
            
            int64_t m_75119;
            
            if (cond_75109) {
                m_75119 = (int64_t) 0;
            } else {
                m_75119 = m_f_res_75117;
            }
            
            int64_t m_75129 = sub64(m_75119, (int64_t) 1);
            bool i_p_m_t_s_leq_w_75131 = slt64(m_75129, dzlz7bUZLztZRz20UZLZLzmZRz20Usupz20UinfZRz20Unz7dUzg_75052);
            bool zzero_leq_i_p_m_t_s_75130 = sle64((int64_t) 0, m_75129);
            bool y_75133 = zzero_leq_i_p_m_t_s_75130 && i_p_m_t_s_leq_w_75131;
            bool i_lte_j_75132 = sle64((int64_t) 0, m_75119);
            bool forwards_ok_75134 = i_lte_j_75132 && y_75133;
            bool eq_x_zz_75126 = (int64_t) 0 == m_f_res_75117;
            bool p_and_eq_x_y_75127 = x_75110 && eq_x_zz_75126;
            bool empty_slice_75128 = cond_75109 || p_and_eq_x_y_75127;
            bool ok_or_empty_75135 = empty_slice_75128 || forwards_ok_75134;
            bool index_certs_75136;
            
            if (!ok_or_empty_75135) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_75119, "] out of bounds for array of shape [", (long long) dzlz7bUZLztZRz20UZLZLzmZRz20Usupz20UinfZRz20Unz7dUzg_75052, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t bytes_83361 = (int64_t) 8 * m_75119;
            int64_t conc_tmp_75158 = loop_dz2089U_75045 + m_75119;
            int64_t bytes_83385 = (int64_t) 8 * conc_tmp_75158;
            
            if (mem_83340_cached_sizze_84665 < bytes_83309) {
                err = lexical_realloc(ctx, &mem_83340, &mem_83340_cached_sizze_84665, bytes_83309);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            for (int64_t nest_i_84411 = 0; nest_i_84411 < dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_75051; nest_i_84411++) {
                lmad_copy_8b(ctx, 1, (uint64_t *) mem_83340, nest_i_84411 * conc_tmp_75018, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83301, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {conc_tmp_75018});
            }
            if (mem_83343_cached_sizze_84666 < bytes_83309) {
                err = lexical_realloc(ctx, &mem_83343, &mem_83343_cached_sizze_84666, bytes_83309);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            lmad_copy_8b(ctx, 2, (uint64_t *) mem_83343, (int64_t) 0, (int64_t []) {conc_tmp_75018, (int64_t) 1}, (uint64_t *) mem_83310, (int64_t) 0, (int64_t []) {conc_tmp_75018, (int64_t) 1}, (int64_t []) {dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_75051, conc_tmp_75018});
            if (memblock_alloc(ctx, &mem_83386, bytes_83385, "mem_83386")) {
                err = 1;
                goto cleanup;
            }
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_83386.mem, loop_dz2089U_75045, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83340, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75119});
            if (mem_83364_cached_sizze_84669 < bytes_83361) {
                err = lexical_realloc(ctx, &mem_83364, &mem_83364_cached_sizze_84669, bytes_83361);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_83364, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83343, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75119});
            for (int64_t write_iter_81833 = 0; write_iter_81833 < dzlz7bUZLztZRz20UZLZLzmZRz20Usupz20UinfZRz20Unz7dUzg_75052; write_iter_81833++) {
                int64_t write_iv_81836 = ((int64_t *) mem_83348)[write_iter_81833];
                int64_t write_iv_81837 = ((int64_t *) mem_83346)[write_iter_81833];
                int64_t new_index_82282 = squot64(write_iter_81833, conc_tmp_75018);
                int64_t binop_y_82284 = conc_tmp_75018 * new_index_82282;
                int64_t new_index_82285 = write_iter_81833 - binop_y_82284;
                int64_t write_iv_81838 = ((int64_t *) mem_83310)[new_index_82282 * conc_tmp_75018 + new_index_82285];
                bool cond_78988 = write_iv_81836 == (int64_t) 1;
                int64_t lifted_lambda_res_78989;
                
                if (cond_78988) {
                    int64_t lifted_lambda_res_t_res_80915 = sub64(write_iv_81837, (int64_t) 1);
                    
                    lifted_lambda_res_78989 = lifted_lambda_res_t_res_80915;
                } else {
                    lifted_lambda_res_78989 = (int64_t) -1;
                }
                if (sle64((int64_t) 0, lifted_lambda_res_78989) && slt64(lifted_lambda_res_78989, m_75119)) {
                    ((int64_t *) mem_83364)[lifted_lambda_res_78989] = write_iv_81838;
                }
                if (sle64((int64_t) 0, lifted_lambda_res_78989) && slt64(lifted_lambda_res_78989, m_75119)) {
                    ((int64_t *) mem_83386.mem)[loop_dz2089U_75045 + lifted_lambda_res_78989] = new_index_82285;
                }
            }
            for (int64_t nest_i_84414 = 0; nest_i_84414 < dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_75051; nest_i_84414++) {
                ((int64_t *) mem_83299)[inf_75048 + nest_i_84414] = (int64_t) 0;
            }
            for (int64_t iter_81842 = 0; iter_81842 < m_75119; iter_81842++) {
                int64_t pixel_81844 = ((int64_t *) mem_83364)[iter_81842];
                int64_t tmp_78981 = sub64(pixel_81844, inf_75048);
                bool less_than_zzero_81846 = slt64(tmp_78981, (int64_t) 0);
                bool greater_than_sizze_81847 = sle64(dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_75051, tmp_78981);
                bool outside_bounds_dim_81848 = less_than_zzero_81846 || greater_than_sizze_81847;
                
                if (!outside_bounds_dim_81848) {
                    int64_t read_hist_81850 = ((int64_t *) mem_83299)[inf_75048 + tmp_78981];
                    int64_t defunc_0_op_res_75154 = add64((int64_t) 1, read_hist_81850);
                    
                    ((int64_t *) mem_83299)[inf_75048 + tmp_78981] = defunc_0_op_res_75154;
                }
            }
            
            int64_t tmp_offs_84416 = (int64_t) 0;
            
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_83386.mem, tmp_offs_84416, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_83307.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {loop_dz2089U_75045});
            tmp_offs_84416 += loop_dz2089U_75045;
            if (!(tmp_offs_84416 == loop_dz2089U_75045)) {
                lmad_copy_8b(ctx, 1, (uint64_t *) mem_83386.mem, tmp_offs_84416, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83386.mem, loop_dz2089U_75045, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75119});
            }
            tmp_offs_84416 += m_75119;
            if (memblock_set(ctx, &mem_param_tmp_84400, &mem_83386, "mem_83386") != 0)
                return 1;
            
            int64_t loop_dz2089U_tmp_84401 = conc_tmp_75158;
            
            if (memblock_set(ctx, &mem_param_83307, &mem_param_tmp_84400, "mem_param_tmp_84400") != 0)
                return 1;
            loop_dz2089U_75045 = loop_dz2089U_tmp_84401;
        }
        if (memblock_set(ctx, &ext_mem_83391, &mem_param_83307, "mem_param_83307") != 0)
            return 1;
        mk_adjacency_list_res_75041 = loop_dz2089U_75045;
        if (mem_83394_cached_sizze_84670 < bytes_83296) {
            err = lexical_realloc(ctx, &mem_83394, &mem_83394_cached_sizze_84670, bytes_83296);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_81858;
        int64_t scanacc_81854 = (int64_t) 0;
        
        for (int64_t i_81856 = 0; i_81856 < conc_tmp_75018; i_81856++) {
            int64_t x_75161 = ((int64_t *) mem_83299)[i_81856];
            int64_t defunc_0_op_res_75164 = add64(x_75161, scanacc_81854);
            
            ((int64_t *) mem_83394)[i_81856] = defunc_0_op_res_75164;
            
            int64_t scanacc_tmp_84417 = defunc_0_op_res_75164;
            
            scanacc_81854 = scanacc_tmp_84417;
        }
        discard_81858 = scanacc_81854;
        if (mem_83401_cached_sizze_84671 < mk_adjacency_list_res_75041) {
            err = lexical_realloc(ctx, &mem_83401, &mem_83401_cached_sizze_84671, mk_adjacency_list_res_75041);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t nest_i_84419 = 0; nest_i_84419 < mk_adjacency_list_res_75041; nest_i_84419++) {
            ((bool *) mem_83401)[nest_i_84419] = 0;
        }
        for (int64_t write_iter_81859 = 0; write_iter_81859 < conc_tmp_75018; write_iter_81859++) {
            int64_t zv_lhs_77391 = add64((int64_t) -1, write_iter_81859);
            int64_t tmp_77392 = smod64(zv_lhs_77391, conc_tmp_75018);
            int64_t lifted_lambda_res_77393 = ((int64_t *) mem_83394)[tmp_77392];
            bool cond_77395 = write_iter_81859 == (int64_t) 0;
            int64_t lifted_lambda_res_77396;
            
            if (cond_77395) {
                lifted_lambda_res_77396 = (int64_t) 0;
            } else {
                lifted_lambda_res_77396 = lifted_lambda_res_77393;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77396) && slt64(lifted_lambda_res_77396, mk_adjacency_list_res_75041)) {
                ((bool *) mem_83401)[lifted_lambda_res_77396] = 1;
            }
        }
        
        bool defunc_0_reduce_res_81393;
        bool redout_81863 = 0;
        
        for (int64_t i_81864 = 0; i_81864 < mk_adjacency_list_res_75041; i_81864++) {
            int64_t eta_p_77378 = ((int64_t *) ext_mem_83391.mem)[i_81864];
            bool lifted_lambda_res_77379 = (int64_t) -1 == eta_p_77378;
            bool lifted_lambda_res_77380 = !lifted_lambda_res_77379;
            bool defunc_0_op_res_76702 = lifted_lambda_res_77380 || redout_81863;
            bool redout_tmp_84421 = defunc_0_op_res_76702;
            
            redout_81863 = redout_tmp_84421;
        }
        defunc_0_reduce_res_81393 = redout_81863;
        
        bool cond_75184 = slt64((int64_t) 0, mk_adjacency_list_res_75041);
        int64_t bytes_83411 = (int64_t) 8 * mk_adjacency_list_res_75041;
        
        if (mem_83412_cached_sizze_84672 < bytes_83411) {
            err = lexical_realloc(ctx, &mem_83412, &mem_83412_cached_sizze_84672, bytes_83411);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83420_cached_sizze_84673 < bytes_83411) {
            err = lexical_realloc(ctx, &mem_83420, &mem_83420_cached_sizze_84673, bytes_83411);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83421_cached_sizze_84674 < mk_adjacency_list_res_75041) {
            err = lexical_realloc(ctx, &mem_83421, &mem_83421_cached_sizze_84674, mk_adjacency_list_res_75041);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        bool itered_neighlist_75185;
        bool loop_while_75187;
        
        if (memblock_set(ctx, &mem_param_83410, &ext_mem_83391, "ext_mem_83391") != 0)
            return 1;
        loop_while_75187 = defunc_0_reduce_res_81393;
        while (loop_while_75187) {
            int64_t discard_81884;
            int64_t scanacc_81878 = (int64_t) 0;
            
            for (int64_t i_81881 = 0; i_81881 < mk_adjacency_list_res_75041; i_81881++) {
                int64_t zv_lhs_79051 = add64((int64_t) 1, i_81881);
                int64_t tmp_79052 = smod64(zv_lhs_79051, mk_adjacency_list_res_75041);
                bool lifted_lambda_res_79053 = ((bool *) mem_83401)[tmp_79052];
                int64_t bool_res_79055 = btoi_bool_i64(lifted_lambda_res_79053);
                int64_t defunc_0_op_res_75213 = add64(bool_res_79055, scanacc_81878);
                
                ((int64_t *) mem_83420)[i_81881] = defunc_0_op_res_75213;
                ((bool *) mem_83421)[i_81881] = lifted_lambda_res_79053;
                
                int64_t scanacc_tmp_84425 = defunc_0_op_res_75213;
                
                scanacc_81878 = scanacc_tmp_84425;
            }
            discard_81884 = scanacc_81878;
            
            int64_t num_segments_t_res_75214;
            
            if (cond_75184) {
                int64_t tmp_80926 = sub64(mk_adjacency_list_res_75041, (int64_t) 1);
                bool x_80927 = sle64((int64_t) 0, tmp_80926);
                bool y_80928 = slt64(tmp_80926, mk_adjacency_list_res_75041);
                bool bounds_check_80929 = x_80927 && y_80928;
                bool index_certs_80930;
                
                if (!bounds_check_80929) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_80926, "] out of bounds for array of shape [", (long long) mk_adjacency_list_res_75041, "].", "-> #0  /prelude/array.fut:28:29-37\n   #1  ../../lib/github.com/diku-dk/segmented/segmented.fut:29:36-60\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                int64_t last_res_80931 = ((int64_t *) mem_83420)[tmp_80926];
                
                num_segments_t_res_75214 = last_res_80931;
            } else {
                num_segments_t_res_75214 = (int64_t) 0;
            }
            
            int64_t num_segments_75221;
            
            if (cond_75184) {
                num_segments_75221 = num_segments_t_res_75214;
            } else {
                num_segments_75221 = (int64_t) 0;
            }
            
            int64_t bytes_83434 = (int64_t) 8 * num_segments_75221;
            int64_t discard_81875;
            int64_t scanacc_81868 = (int64_t) 9223372036854775807;
            
            for (int64_t i_81871 = 0; i_81871 < mk_adjacency_list_res_75041; i_81871++) {
                bool x_75191 = ((bool *) mem_83401)[i_81871];
                int64_t x_75192 = ((int64_t *) mem_param_83410.mem)[i_81871];
                int64_t tmp_75198;
                
                if (x_75191) {
                    tmp_75198 = x_75192;
                } else {
                    bool cond_75199 = slt64(scanacc_81868, x_75192);
                    int64_t lifted_lambda_res_75200;
                    
                    if (cond_75199) {
                        lifted_lambda_res_75200 = scanacc_81868;
                    } else {
                        lifted_lambda_res_75200 = x_75192;
                    }
                    tmp_75198 = lifted_lambda_res_75200;
                }
                ((int64_t *) mem_83412)[i_81871] = tmp_75198;
                
                int64_t scanacc_tmp_84428 = tmp_75198;
                
                scanacc_81868 = scanacc_tmp_84428;
            }
            discard_81875 = scanacc_81868;
            if (mem_83435_cached_sizze_84675 < bytes_83434) {
                err = lexical_realloc(ctx, &mem_83435, &mem_83435_cached_sizze_84675, bytes_83434);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            for (int64_t nest_i_84430 = 0; nest_i_84430 < num_segments_75221; nest_i_84430++) {
                ((int64_t *) mem_83435)[nest_i_84430] = (int64_t) 9223372036854775807;
            }
            for (int64_t write_iter_81885 = 0; write_iter_81885 < mk_adjacency_list_res_75041; write_iter_81885++) {
                int64_t write_iv_81887 = ((int64_t *) mem_83420)[write_iter_81885];
                bool write_iv_81888 = ((bool *) mem_83421)[write_iter_81885];
                int64_t write_iv_81889 = ((int64_t *) mem_83412)[write_iter_81885];
                int64_t lifted_index_res_79042;
                
                if (write_iv_81888) {
                    int64_t lifted_index_res_t_res_80932 = sub64(write_iv_81887, (int64_t) 1);
                    
                    lifted_index_res_79042 = lifted_index_res_t_res_80932;
                } else {
                    lifted_index_res_79042 = (int64_t) -1;
                }
                if (sle64((int64_t) 0, lifted_index_res_79042) && slt64(lifted_index_res_79042, num_segments_75221)) {
                    ((int64_t *) mem_83435)[lifted_index_res_79042] = write_iv_81889;
                }
            }
            if (memblock_alloc(ctx, &mem_83443, bytes_83411, "mem_83443")) {
                err = 1;
                goto cleanup;
            }
            
            bool defunc_0_reduce_res_80938;
            bool redout_81892 = 0;
            
            for (int64_t i_81894 = 0; i_81894 < mk_adjacency_list_res_75041; i_81894++) {
                int64_t eta_p_79027 = ((int64_t *) mem_param_83410.mem)[i_81894];
                bool x_79029 = sle64((int64_t) 0, eta_p_79027);
                bool y_79030 = slt64(eta_p_79027, num_segments_75221);
                bool bounds_check_79031 = x_79029 && y_79030;
                bool index_certs_79032;
                
                if (!bounds_check_79031) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_79027, "] out of bounds for array of shape [", (long long) num_segments_75221, "].", "-> #0  ../../ftDBSCAN_plus.fut:607:71-96\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../ftDBSCAN_plus.fut:607:60-97\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                int64_t lifted_lambda_res_79033 = ((int64_t *) mem_83435)[eta_p_79027];
                bool lifted_lambda_res_79035 = eta_p_79027 == lifted_lambda_res_79033;
                bool lifted_lambda_res_79036 = !lifted_lambda_res_79035;
                bool defunc_0_op_res_76710 = lifted_lambda_res_79036 || redout_81892;
                
                ((int64_t *) mem_83443.mem)[i_81894] = lifted_lambda_res_79033;
                
                bool redout_tmp_84432 = defunc_0_op_res_76710;
                
                redout_81892 = redout_tmp_84432;
            }
            defunc_0_reduce_res_80938 = redout_81892;
            if (memblock_set(ctx, &mem_param_tmp_84422, &mem_83443, "mem_83443") != 0)
                return 1;
            
            bool loop_while_tmp_84423 = defunc_0_reduce_res_80938;
            
            if (memblock_set(ctx, &mem_param_83410, &mem_param_tmp_84422, "mem_param_tmp_84422") != 0)
                return 1;
            loop_while_75187 = loop_while_tmp_84423;
        }
        if (memblock_set(ctx, &ext_mem_83452, &mem_param_83410, "mem_param_83410") != 0)
            return 1;
        itered_neighlist_75185 = loop_while_75187;
        if (memblock_unref(ctx, &ext_mem_83391, "ext_mem_83391") != 0)
            return 1;
        if (mem_83454_cached_sizze_84676 < bytes_83411) {
            err = lexical_realloc(ctx, &mem_83454, &mem_83454_cached_sizze_84676, bytes_83411);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_81906;
        int64_t scanacc_81899 = (int64_t) 9223372036854775807;
        
        for (int64_t i_81902 = 0; i_81902 < mk_adjacency_list_res_75041; i_81902++) {
            bool x_75246 = ((bool *) mem_83401)[i_81902];
            int64_t x_75247 = ((int64_t *) ext_mem_83452.mem)[i_81902];
            int64_t tmp_75253;
            
            if (x_75246) {
                tmp_75253 = x_75247;
            } else {
                bool cond_75254 = slt64(scanacc_81899, x_75247);
                int64_t lifted_lambda_res_75255;
                
                if (cond_75254) {
                    lifted_lambda_res_75255 = scanacc_81899;
                } else {
                    lifted_lambda_res_75255 = x_75247;
                }
                tmp_75253 = lifted_lambda_res_75255;
            }
            ((int64_t *) mem_83454)[i_81902] = tmp_75253;
            
            int64_t scanacc_tmp_84434 = tmp_75253;
            
            scanacc_81899 = scanacc_tmp_84434;
        }
        discard_81906 = scanacc_81899;
        if (memblock_unref(ctx, &ext_mem_83452, "ext_mem_83452") != 0)
            return 1;
        if (mem_83462_cached_sizze_84677 < bytes_83411) {
            err = lexical_realloc(ctx, &mem_83462, &mem_83462_cached_sizze_84677, bytes_83411);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83463_cached_sizze_84678 < mk_adjacency_list_res_75041) {
            err = lexical_realloc(ctx, &mem_83463, &mem_83463_cached_sizze_84678, mk_adjacency_list_res_75041);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_81915;
        int64_t scanacc_81909 = (int64_t) 0;
        
        for (int64_t i_81912 = 0; i_81912 < mk_adjacency_list_res_75041; i_81912++) {
            int64_t zv_lhs_77371 = add64((int64_t) 1, i_81912);
            int64_t tmp_77372 = smod64(zv_lhs_77371, mk_adjacency_list_res_75041);
            bool lifted_lambda_res_77373 = ((bool *) mem_83401)[tmp_77372];
            int64_t bool_res_77375 = btoi_bool_i64(lifted_lambda_res_77373);
            int64_t defunc_0_op_res_75268 = add64(bool_res_77375, scanacc_81909);
            
            ((int64_t *) mem_83462)[i_81912] = defunc_0_op_res_75268;
            ((bool *) mem_83463)[i_81912] = lifted_lambda_res_77373;
            
            int64_t scanacc_tmp_84436 = defunc_0_op_res_75268;
            
            scanacc_81909 = scanacc_tmp_84436;
        }
        discard_81915 = scanacc_81909;
        
        int64_t num_segments_t_res_75269;
        
        if (cond_75184) {
            int64_t tmp_80940 = sub64(mk_adjacency_list_res_75041, (int64_t) 1);
            bool x_80941 = sle64((int64_t) 0, tmp_80940);
            bool y_80942 = slt64(tmp_80940, mk_adjacency_list_res_75041);
            bool bounds_check_80943 = x_80941 && y_80942;
            bool index_certs_80944;
            
            if (!bounds_check_80943) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_80940, "] out of bounds for array of shape [", (long long) mk_adjacency_list_res_75041, "].", "-> #0  /prelude/array.fut:28:29-37\n   #1  ../../lib/github.com/diku-dk/segmented/segmented.fut:29:36-60\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t last_res_80945 = ((int64_t *) mem_83462)[tmp_80940];
            
            num_segments_t_res_75269 = last_res_80945;
        } else {
            num_segments_t_res_75269 = (int64_t) 0;
        }
        
        int64_t num_segments_75276;
        
        if (cond_75184) {
            num_segments_75276 = num_segments_t_res_75269;
        } else {
            num_segments_75276 = (int64_t) 0;
        }
        
        int64_t bytes_83476 = (int64_t) 8 * num_segments_75276;
        
        if (mem_83477_cached_sizze_84679 < bytes_83476) {
            err = lexical_realloc(ctx, &mem_83477, &mem_83477_cached_sizze_84679, bytes_83476);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t nest_i_84439 = 0; nest_i_84439 < num_segments_75276; nest_i_84439++) {
            ((int64_t *) mem_83477)[nest_i_84439] = (int64_t) 9223372036854775807;
        }
        for (int64_t write_iter_81916 = 0; write_iter_81916 < mk_adjacency_list_res_75041; write_iter_81916++) {
            int64_t write_iv_81918 = ((int64_t *) mem_83462)[write_iter_81916];
            bool write_iv_81919 = ((bool *) mem_83463)[write_iter_81916];
            int64_t write_iv_81920 = ((int64_t *) mem_83454)[write_iter_81916];
            int64_t lifted_index_res_77362;
            
            if (write_iv_81919) {
                int64_t lifted_index_res_t_res_80946 = sub64(write_iv_81918, (int64_t) 1);
                
                lifted_index_res_77362 = lifted_index_res_t_res_80946;
            } else {
                lifted_index_res_77362 = (int64_t) -1;
            }
            if (sle64((int64_t) 0, lifted_index_res_77362) && slt64(lifted_index_res_77362, num_segments_75276)) {
                ((int64_t *) mem_83477)[lifted_index_res_77362] = write_iv_81920;
            }
        }
        
        bool eq_x_y_75286 = conc_tmp_75018 == num_segments_t_res_75269;
        bool eq_x_zz_75287 = conc_tmp_75018 == (int64_t) 0;
        bool p_and_eq_x_y_75288 = cond_75184 && eq_x_y_75286;
        bool not_p_75289 = !cond_75184;
        bool p_and_eq_x_y_75290 = eq_x_zz_75287 && not_p_75289;
        bool dim_match_75291 = p_and_eq_x_y_75288 || p_and_eq_x_y_75290;
        bool empty_or_match_cert_75292;
        
        if (!dim_match_75291) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Value of (desugared) shape [", (long long) num_segments_75276, "] cannot match shape of type `[", (long long) conc_tmp_75018, "]i64`.", "-> #0  ../../ftDBSCAN_plus.fut:616:28-51\n   #1  test_dbscan_plus.fut:14:23-19:30\n   #2  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        if (mem_83485_cached_sizze_84680 < bytes_83296) {
            err = lexical_realloc(ctx, &mem_83485, &mem_83485_cached_sizze_84680, bytes_83296);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_81927;
        int64_t scanacc_81923 = (int64_t) 0;
        
        for (int64_t i_81925 = 0; i_81925 < conc_tmp_75018; i_81925++) {
            int64_t eta_p_77353 = ((int64_t *) mem_83477)[i_81925];
            bool defunc_0_f_res_77354 = i_81925 == eta_p_77353;
            int64_t lifted_lambda_res_77356 = btoi_bool_i64(defunc_0_f_res_77354);
            int64_t defunc_0_op_res_75305 = add64(lifted_lambda_res_77356, scanacc_81923);
            
            ((int64_t *) mem_83485)[i_81925] = defunc_0_op_res_75305;
            
            int64_t scanacc_tmp_84441 = defunc_0_op_res_75305;
            
            scanacc_81923 = scanacc_tmp_84441;
        }
        discard_81927 = scanacc_81923;
        if (mem_83493_cached_sizze_84681 < bytes_83296) {
            err = lexical_realloc(ctx, &mem_83493, &mem_83493_cached_sizze_84681, bytes_83296);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t i_81930 = 0; i_81930 < conc_tmp_75018; i_81930++) {
            int64_t zv_lhs_77341 = add64((int64_t) -1, i_81930);
            int64_t tmp_77342 = smod64(zv_lhs_77341, conc_tmp_75018);
            int64_t lifted_lambda_res_77343 = ((int64_t *) mem_83485)[tmp_77342];
            bool cond_77345 = i_81930 == (int64_t) 0;
            int64_t lifted_lambda_res_77346;
            
            if (cond_77345) {
                lifted_lambda_res_77346 = (int64_t) 0;
            } else {
                lifted_lambda_res_77346 = lifted_lambda_res_77343;
            }
            ((int64_t *) mem_83493)[i_81930] = lifted_lambda_res_77346;
        }
        if (mem_83501_cached_sizze_84682 < bytes_83296) {
            err = lexical_realloc(ctx, &mem_83501, &mem_83501_cached_sizze_84682, bytes_83296);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t i_81934 = 0; i_81934 < conc_tmp_75018; i_81934++) {
            int64_t eta_p_77324 = ((int64_t *) mem_83477)[i_81934];
            bool cond_77325 = slt64(eta_p_77324, (int64_t) 0);
            bool cond_f_res_77326 = sle64(conc_tmp_75018, eta_p_77324);
            bool x_77327 = !cond_77325;
            bool y_77328 = cond_f_res_77326 && x_77327;
            bool cond_77329 = cond_77325 || y_77328;
            int64_t lifted_lambda_res_77330;
            
            if (cond_77329) {
                lifted_lambda_res_77330 = (int64_t) -1;
            } else {
                bool x_77331 = sle64((int64_t) 0, eta_p_77324);
                bool y_77332 = slt64(eta_p_77324, conc_tmp_75018);
                bool bounds_check_77333 = x_77331 && y_77332;
                bool index_certs_77334;
                
                if (!bounds_check_77333) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_77324, "] out of bounds for array of shape [", (long long) conc_tmp_75018, "].", "-> #0  ../../ftbasics.fut:27:66-71\n   #1  /prelude/soacs.fut:67:23-24\n   #2  /prelude/soacs.fut:67:27-37\n   #3  /prelude/functional.fut:9:44-45\n   #4  ../../ftbasics.fut:27:23-77\n   #5  test_dbscan_plus.fut:13:1-20:55\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                int64_t lifted_lambda_res_f_res_77335 = ((int64_t *) mem_83493)[eta_p_77324];
                
                lifted_lambda_res_77330 = lifted_lambda_res_f_res_77335;
            }
            
            int64_t lifted_lambda_res_77337 = add64(old_clHandler_73721, lifted_lambda_res_77330);
            
            ((int64_t *) mem_83501)[i_81934] = lifted_lambda_res_77337;
        }
        if (mem_83509_cached_sizze_84683 < bytes_83296) {
            err = lexical_realloc(ctx, &mem_83509, &mem_83509_cached_sizze_84683, bytes_83296);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83511_cached_sizze_84684 < bytes_83296) {
            err = lexical_realloc(ctx, &mem_83511, &mem_83511_cached_sizze_84684, bytes_83296);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_81944;
        int64_t scanacc_81938 = (int64_t) 0;
        
        for (int64_t i_81941 = 0; i_81941 < conc_tmp_75018; i_81941++) {
            bool index_concat_cmp_82263 = sle64(conc_tmp_75017, i_81941);
            int64_t index_concat_branch_82271;
            
            if (index_concat_cmp_82263) {
                int64_t index_concat_i_82264 = sub64(i_81941, conc_tmp_75017);
                int64_t index_concat_82265 = ((int64_t *) mem_83204)[index_concat_i_82264];
                
                index_concat_branch_82271 = index_concat_82265;
            } else {
                bool index_concat_cmp_82266 = sle64(m_74941, i_81941);
                int64_t index_concat_branch_82270;
                
                if (index_concat_cmp_82266) {
                    int64_t index_concat_i_82267 = sub64(i_81941, m_74941);
                    int64_t index_concat_82268 = ((int64_t *) mem_83164)[index_concat_i_82267];
                    
                    index_concat_branch_82270 = index_concat_82268;
                } else {
                    int64_t index_concat_82269 = ((int64_t *) mem_83273)[i_81941];
                    
                    index_concat_branch_82270 = index_concat_82269;
                }
                index_concat_branch_82271 = index_concat_branch_82270;
            }
            
            bool lifted_lambda_res_77320 = sle64((int64_t) 0, index_concat_branch_82271);
            int64_t defunc_0_f_res_77321 = btoi_bool_i64(lifted_lambda_res_77320);
            int64_t defunc_0_op_res_75329 = add64(defunc_0_f_res_77321, scanacc_81938);
            
            ((int64_t *) mem_83509)[i_81941] = defunc_0_op_res_75329;
            ((int64_t *) mem_83511)[i_81941] = defunc_0_f_res_77321;
            
            int64_t scanacc_tmp_84445 = defunc_0_op_res_75329;
            
            scanacc_81938 = scanacc_tmp_84445;
        }
        discard_81944 = scanacc_81938;
        
        bool x_75330 = !eq_x_zz_75287;
        int64_t tmp_75331 = sub64(conc_tmp_75018, (int64_t) 1);
        bool x_75332 = sle64((int64_t) 0, tmp_75331);
        bool y_75333 = slt64(tmp_75331, conc_tmp_75018);
        bool bounds_check_75334 = x_75332 && y_75333;
        bool protect_assert_disj_75335 = eq_x_zz_75287 || bounds_check_75334;
        bool index_certs_75336;
        
        if (!protect_assert_disj_75335) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_75331, "] out of bounds for array of shape [", (long long) conc_tmp_75018, "].", "-> #0  /prelude/soacs.fut:257:33-47\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t m_f_res_75337;
        
        if (x_75330) {
            int64_t x_80947 = ((int64_t *) mem_83509)[tmp_75331];
            
            m_f_res_75337 = x_80947;
        } else {
            m_f_res_75337 = (int64_t) 0;
        }
        
        int64_t m_75339;
        
        if (eq_x_zz_75287) {
            m_75339 = (int64_t) 0;
        } else {
            m_75339 = m_f_res_75337;
        }
        
        int64_t m_75349 = sub64(m_75339, (int64_t) 1);
        bool i_p_m_t_s_leq_w_75351 = slt64(m_75349, conc_tmp_75018);
        bool zzero_leq_i_p_m_t_s_75350 = sle64((int64_t) 0, m_75349);
        bool y_75353 = zzero_leq_i_p_m_t_s_75350 && i_p_m_t_s_leq_w_75351;
        bool i_lte_j_75352 = sle64((int64_t) 0, m_75339);
        bool forwards_ok_75354 = i_lte_j_75352 && y_75353;
        bool eq_x_zz_75346 = (int64_t) 0 == m_f_res_75337;
        bool p_and_eq_x_y_75347 = x_75330 && eq_x_zz_75346;
        bool empty_slice_75348 = eq_x_zz_75287 || p_and_eq_x_y_75347;
        bool ok_or_empty_75355 = empty_slice_75348 || forwards_ok_75354;
        bool index_certs_75356;
        
        if (!ok_or_empty_75355) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_75339, "] out of bounds for array of shape [", (long long) conc_tmp_75018, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_83524 = (int64_t) 8 * m_75339;
        
        if (mem_83525_cached_sizze_84685 < bytes_83524) {
            err = lexical_realloc(ctx, &mem_83525, &mem_83525_cached_sizze_84685, bytes_83524);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83525, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83297, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75339});
        if (mem_83527_cached_sizze_84686 < bytes_83524) {
            err = lexical_realloc(ctx, &mem_83527, &mem_83527_cached_sizze_84686, bytes_83524);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83527, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83501, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75339});
        for (int64_t write_iter_81945 = 0; write_iter_81945 < conc_tmp_75018; write_iter_81945++) {
            int64_t write_iv_81948 = ((int64_t *) mem_83511)[write_iter_81945];
            int64_t write_iv_81949 = ((int64_t *) mem_83509)[write_iter_81945];
            int64_t write_iv_81950 = ((int64_t *) mem_83501)[write_iter_81945];
            bool index_concat_cmp_82252 = sle64(conc_tmp_75017, write_iter_81945);
            int64_t index_concat_branch_82260;
            
            if (index_concat_cmp_82252) {
                int64_t index_concat_i_82253 = sub64(write_iter_81945, conc_tmp_75017);
                int64_t index_concat_82254 = ((int64_t *) mem_83204)[index_concat_i_82253];
                
                index_concat_branch_82260 = index_concat_82254;
            } else {
                bool index_concat_cmp_82255 = sle64(m_74941, write_iter_81945);
                int64_t index_concat_branch_82259;
                
                if (index_concat_cmp_82255) {
                    int64_t index_concat_i_82256 = sub64(write_iter_81945, m_74941);
                    int64_t index_concat_82257 = ((int64_t *) mem_83164)[index_concat_i_82256];
                    
                    index_concat_branch_82259 = index_concat_82257;
                } else {
                    int64_t index_concat_82258 = ((int64_t *) mem_83273)[write_iter_81945];
                    
                    index_concat_branch_82259 = index_concat_82258;
                }
                index_concat_branch_82260 = index_concat_branch_82259;
            }
            
            bool cond_77314 = write_iv_81948 == (int64_t) 1;
            int64_t lifted_lambda_res_77315;
            
            if (cond_77314) {
                int64_t lifted_lambda_res_t_res_80948 = sub64(write_iv_81949, (int64_t) 1);
                
                lifted_lambda_res_77315 = lifted_lambda_res_t_res_80948;
            } else {
                lifted_lambda_res_77315 = (int64_t) -1;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77315) && slt64(lifted_lambda_res_77315, m_75339)) {
                ((int64_t *) mem_83527)[lifted_lambda_res_77315] = write_iv_81950;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77315) && slt64(lifted_lambda_res_77315, m_75339)) {
                ((int64_t *) mem_83525)[lifted_lambda_res_77315] = index_concat_branch_82260;
            }
        }
        if (mem_83541_cached_sizze_84687 < bytes_83524) {
            err = lexical_realloc(ctx, &mem_83541, &mem_83541_cached_sizze_84687, bytes_83524);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83543_cached_sizze_84688 < bytes_83524) {
            err = lexical_realloc(ctx, &mem_83543, &mem_83543_cached_sizze_84688, bytes_83524);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t i_81958 = 0; i_81958 < m_75339; i_81958++) {
            int64_t lifted_lambda_res_75374 = ((int64_t *) mem_83527)[i_81958];
            int64_t lifted_lambda_res_75375 = ((int64_t *) mem_83525)[i_81958];
            
            ((int64_t *) mem_83541)[i_81958] = lifted_lambda_res_75374;
            ((int64_t *) mem_83543)[i_81958] = lifted_lambda_res_75375;
        }
        if (mem_83557_cached_sizze_84689 < bytes_83524) {
            err = lexical_realloc(ctx, &mem_83557, &mem_83557_cached_sizze_84689, bytes_83524);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83559_cached_sizze_84690 < bytes_83524) {
            err = lexical_realloc(ctx, &mem_83559, &mem_83559_cached_sizze_84690, bytes_83524);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_81973;
        int64_t scanacc_81967 = (int64_t) 0;
        
        for (int64_t i_81970 = 0; i_81970 < m_75339; i_81970++) {
            int64_t lifted_lambda_res_77274 = ((int64_t *) mem_83527)[i_81970];
            int64_t lifted_lambda_res_77275 = ((int64_t *) mem_83525)[i_81970];
            int64_t defunc_0_f_res_80949;
            int64_t defunc_0_f_res_80950;
            int64_t defunc_0_f_res_80951;
            int64_t redout_81961;
            int64_t redout_81962;
            int64_t redout_81963;
            
            redout_81961 = i_81970;
            redout_81962 = lifted_lambda_res_77274;
            redout_81963 = lifted_lambda_res_77275;
            for (int64_t i_81964 = 0; i_81964 < m_75339; i_81964++) {
                int64_t x_77280 = ((int64_t *) mem_83541)[i_81964];
                int64_t x_77281 = ((int64_t *) mem_83543)[i_81964];
                bool cond_77288 = redout_81962 == lifted_lambda_res_77274;
                bool cond_77289 = !cond_77288;
                bool cond_f_res_77290 = redout_81963 == lifted_lambda_res_77275;
                bool cond_f_res_77291 = !cond_f_res_77290;
                bool y_77292 = cond_77288 && cond_f_res_77291;
                bool cond_77293 = cond_77289 || y_77292;
                bool cond_77294 = slt64(i_81964, redout_81961);
                bool cond_t_res_77295 = x_77280 == lifted_lambda_res_77274;
                bool x_77296 = cond_77294 && cond_t_res_77295;
                bool cond_f_res_t_res_77297 = x_77281 == lifted_lambda_res_77275;
                bool x_77298 = x_77296 && cond_f_res_t_res_77297;
                bool x_77299 = !cond_77293;
                bool y_77300 = x_77298 && x_77299;
                bool cond_77301 = cond_77293 || y_77300;
                int64_t lifted_lambda_res_77302;
                
                if (cond_77301) {
                    lifted_lambda_res_77302 = i_81964;
                } else {
                    lifted_lambda_res_77302 = redout_81961;
                }
                
                int64_t lifted_lambda_res_77303;
                
                if (cond_77301) {
                    lifted_lambda_res_77303 = x_77280;
                } else {
                    lifted_lambda_res_77303 = redout_81962;
                }
                
                int64_t lifted_lambda_res_77304;
                
                if (cond_77301) {
                    lifted_lambda_res_77304 = x_77281;
                } else {
                    lifted_lambda_res_77304 = redout_81963;
                }
                
                int64_t redout_tmp_84455 = lifted_lambda_res_77302;
                int64_t redout_tmp_84456 = lifted_lambda_res_77303;
                int64_t redout_tmp_84457 = lifted_lambda_res_77304;
                
                redout_81961 = redout_tmp_84455;
                redout_81962 = redout_tmp_84456;
                redout_81963 = redout_tmp_84457;
            }
            defunc_0_f_res_80949 = redout_81961;
            defunc_0_f_res_80950 = redout_81962;
            defunc_0_f_res_80951 = redout_81963;
            
            bool lifted_lambda_res_77305 = defunc_0_f_res_80949 == i_81970;
            int64_t defunc_0_f_res_77307 = btoi_bool_i64(lifted_lambda_res_77305);
            int64_t defunc_0_op_res_75421 = add64(defunc_0_f_res_77307, scanacc_81967);
            
            ((int64_t *) mem_83557)[i_81970] = defunc_0_op_res_75421;
            ((int64_t *) mem_83559)[i_81970] = defunc_0_f_res_77307;
            
            int64_t scanacc_tmp_84452 = defunc_0_op_res_75421;
            
            scanacc_81967 = scanacc_tmp_84452;
        }
        discard_81973 = scanacc_81967;
        
        bool x_75422 = !empty_slice_75348;
        bool y_75423 = slt64(m_75349, m_75339);
        bool bounds_check_75424 = zzero_leq_i_p_m_t_s_75350 && y_75423;
        bool protect_assert_disj_75425 = empty_slice_75348 || bounds_check_75424;
        bool index_certs_75426;
        
        if (!protect_assert_disj_75425) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) m_75349, "] out of bounds for array of shape [", (long long) m_75339, "].", "-> #0  /prelude/soacs.fut:257:33-47\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t m_f_res_75427;
        
        if (x_75422) {
            int64_t x_80952 = ((int64_t *) mem_83557)[m_75349];
            
            m_f_res_75427 = x_80952;
        } else {
            m_f_res_75427 = (int64_t) 0;
        }
        
        int64_t m_75429;
        
        if (empty_slice_75348) {
            m_75429 = (int64_t) 0;
        } else {
            m_75429 = m_f_res_75427;
        }
        
        int64_t m_75439 = sub64(m_75429, (int64_t) 1);
        bool i_p_m_t_s_leq_w_75441 = slt64(m_75439, m_75339);
        bool zzero_leq_i_p_m_t_s_75440 = sle64((int64_t) 0, m_75439);
        bool y_75443 = zzero_leq_i_p_m_t_s_75440 && i_p_m_t_s_leq_w_75441;
        bool i_lte_j_75442 = sle64((int64_t) 0, m_75429);
        bool forwards_ok_75444 = i_lte_j_75442 && y_75443;
        bool eq_x_zz_75436 = (int64_t) 0 == m_f_res_75427;
        bool p_and_eq_x_y_75437 = x_75422 && eq_x_zz_75436;
        bool empty_slice_75438 = empty_slice_75348 || p_and_eq_x_y_75437;
        bool ok_or_empty_75445 = empty_slice_75438 || forwards_ok_75444;
        bool index_certs_75446;
        
        if (!ok_or_empty_75445) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_75429, "] out of bounds for array of shape [", (long long) m_75339, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_83572 = (int64_t) 8 * m_75429;
        
        if (mem_83573_cached_sizze_84691 < bytes_83572) {
            err = lexical_realloc(ctx, &mem_83573, &mem_83573_cached_sizze_84691, bytes_83572);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83573, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83525, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75429});
        if (mem_83575_cached_sizze_84692 < bytes_83572) {
            err = lexical_realloc(ctx, &mem_83575, &mem_83575_cached_sizze_84692, bytes_83572);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83575, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83527, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75429});
        for (int64_t write_iter_81974 = 0; write_iter_81974 < m_75339; write_iter_81974++) {
            int64_t write_iv_81977 = ((int64_t *) mem_83559)[write_iter_81974];
            int64_t write_iv_81978 = ((int64_t *) mem_83557)[write_iter_81974];
            int64_t write_iv_81979 = ((int64_t *) mem_83527)[write_iter_81974];
            int64_t write_iv_81980 = ((int64_t *) mem_83525)[write_iter_81974];
            bool cond_77260 = write_iv_81977 == (int64_t) 1;
            int64_t lifted_lambda_res_77261;
            
            if (cond_77260) {
                int64_t lifted_lambda_res_t_res_80953 = sub64(write_iv_81978, (int64_t) 1);
                
                lifted_lambda_res_77261 = lifted_lambda_res_t_res_80953;
            } else {
                lifted_lambda_res_77261 = (int64_t) -1;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77261) && slt64(lifted_lambda_res_77261, m_75429)) {
                ((int64_t *) mem_83575)[lifted_lambda_res_77261] = write_iv_81979;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77261) && slt64(lifted_lambda_res_77261, m_75429)) {
                ((int64_t *) mem_83573)[lifted_lambda_res_77261] = write_iv_81980;
            }
        }
        if (mem_83589_cached_sizze_84693 < bytes_83296) {
            err = lexical_realloc(ctx, &mem_83589, &mem_83589_cached_sizze_84693, bytes_83296);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83591_cached_sizze_84694 < bytes_83296) {
            err = lexical_realloc(ctx, &mem_83591, &mem_83591_cached_sizze_84694, bytes_83296);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83593_cached_sizze_84695 < bytes_83296) {
            err = lexical_realloc(ctx, &mem_83593, &mem_83593_cached_sizze_84695, bytes_83296);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_81997;
        int64_t scanacc_81989 = (int64_t) 0;
        
        for (int64_t i_81993 = 0; i_81993 < conc_tmp_75018; i_81993++) {
            int64_t eta_p_77226 = ((int64_t *) mem_83501)[i_81993];
            bool index_concat_cmp_82235 = sle64(conc_tmp_75017, i_81993);
            int64_t index_concat_branch_82243;
            
            if (index_concat_cmp_82235) {
                int64_t index_concat_i_82236 = sub64(i_81993, conc_tmp_75017);
                int64_t index_concat_82237 = ((int64_t *) mem_83204)[index_concat_i_82236];
                
                index_concat_branch_82243 = index_concat_82237;
            } else {
                bool index_concat_cmp_82238 = sle64(m_74941, i_81993);
                int64_t index_concat_branch_82242;
                
                if (index_concat_cmp_82238) {
                    int64_t index_concat_i_82239 = sub64(i_81993, m_74941);
                    int64_t index_concat_82240 = ((int64_t *) mem_83164)[index_concat_i_82239];
                    
                    index_concat_branch_82242 = index_concat_82240;
                } else {
                    int64_t index_concat_82241 = ((int64_t *) mem_83273)[i_81993];
                    
                    index_concat_branch_82242 = index_concat_82241;
                }
                index_concat_branch_82243 = index_concat_branch_82242;
            }
            
            int64_t defunc_0_f_res_80954;
            int64_t defunc_0_f_res_80955;
            int64_t redout_81983;
            int64_t redout_81984;
            
            redout_81983 = eta_p_77226;
            redout_81984 = eta_p_77226;
            for (int64_t i_81985 = 0; i_81985 < m_75429; i_81985++) {
                int64_t x_77230 = ((int64_t *) mem_83575)[i_81985];
                int64_t x_77231 = ((int64_t *) mem_83573)[i_81985];
                bool cond_77236 = redout_81983 == eta_p_77226;
                bool cond_77237 = !cond_77236;
                bool cond_f_res_77238 = slt64(redout_81984, (int64_t) 0);
                bool y_77239 = cond_77236 && cond_f_res_77238;
                bool cond_77240 = cond_77237 || y_77239;
                bool cond_77241 = slt64(x_77231, redout_81984);
                bool cond_f_res_t_res_77242 = x_77230 == eta_p_77226;
                bool x_77243 = cond_77241 && cond_f_res_t_res_77242;
                bool x_77244 = !cond_77240;
                bool y_77245 = x_77243 && x_77244;
                bool cond_77246 = cond_77240 || y_77245;
                int64_t lifted_lambda_res_77247;
                
                if (cond_77246) {
                    lifted_lambda_res_77247 = x_77230;
                } else {
                    lifted_lambda_res_77247 = redout_81983;
                }
                
                int64_t lifted_lambda_res_77248;
                
                if (cond_77246) {
                    lifted_lambda_res_77248 = x_77231;
                } else {
                    lifted_lambda_res_77248 = redout_81984;
                }
                
                int64_t redout_tmp_84464 = lifted_lambda_res_77247;
                int64_t redout_tmp_84465 = lifted_lambda_res_77248;
                
                redout_81983 = redout_tmp_84464;
                redout_81984 = redout_tmp_84465;
            }
            defunc_0_f_res_80954 = redout_81983;
            defunc_0_f_res_80955 = redout_81984;
            
            bool cond_77250 = slt64(defunc_0_f_res_80955, old_clHandler_73721);
            bool lifted_lambda_res_t_res_77251 = sle64((int64_t) 0, index_concat_branch_82243);
            bool x_77252 = cond_77250 && lifted_lambda_res_t_res_77251;
            int64_t defunc_0_f_res_77253 = btoi_bool_i64(x_77252);
            int64_t defunc_0_op_res_75490 = add64(defunc_0_f_res_77253, scanacc_81989);
            
            ((int64_t *) mem_83589)[i_81993] = defunc_0_op_res_75490;
            ((int64_t *) mem_83591)[i_81993] = defunc_0_f_res_77253;
            ((int64_t *) mem_83593)[i_81993] = defunc_0_f_res_80955;
            
            int64_t scanacc_tmp_84460 = defunc_0_op_res_75490;
            
            scanacc_81989 = scanacc_tmp_84460;
        }
        discard_81997 = scanacc_81989;
        
        int64_t m_f_res_75491;
        
        if (x_75330) {
            int64_t x_80956 = ((int64_t *) mem_83589)[tmp_75331];
            
            m_f_res_75491 = x_80956;
        } else {
            m_f_res_75491 = (int64_t) 0;
        }
        
        int64_t m_75493;
        
        if (eq_x_zz_75287) {
            m_75493 = (int64_t) 0;
        } else {
            m_75493 = m_f_res_75491;
        }
        
        int64_t m_75503 = sub64(m_75493, (int64_t) 1);
        bool i_p_m_t_s_leq_w_75505 = slt64(m_75503, conc_tmp_75018);
        bool zzero_leq_i_p_m_t_s_75504 = sle64((int64_t) 0, m_75503);
        bool y_75507 = zzero_leq_i_p_m_t_s_75504 && i_p_m_t_s_leq_w_75505;
        bool i_lte_j_75506 = sle64((int64_t) 0, m_75493);
        bool forwards_ok_75508 = i_lte_j_75506 && y_75507;
        bool eq_x_zz_75500 = (int64_t) 0 == m_f_res_75491;
        bool p_and_eq_x_y_75501 = x_75330 && eq_x_zz_75500;
        bool empty_slice_75502 = eq_x_zz_75287 || p_and_eq_x_y_75501;
        bool ok_or_empty_75509 = empty_slice_75502 || forwards_ok_75508;
        bool index_certs_75510;
        
        if (!ok_or_empty_75509) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_75493, "] out of bounds for array of shape [", (long long) conc_tmp_75018, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_83612 = (int64_t) 8 * m_75493;
        
        if (mem_83613_cached_sizze_84696 < bytes_83612) {
            err = lexical_realloc(ctx, &mem_83613, &mem_83613_cached_sizze_84696, bytes_83612);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83613, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83593, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75493});
        if (mem_83615_cached_sizze_84697 < bytes_83612) {
            err = lexical_realloc(ctx, &mem_83615, &mem_83615_cached_sizze_84697, bytes_83612);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83615, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83297, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75493});
        for (int64_t write_iter_81998 = 0; write_iter_81998 < conc_tmp_75018; write_iter_81998++) {
            int64_t write_iv_82001 = ((int64_t *) mem_83591)[write_iter_81998];
            int64_t write_iv_82002 = ((int64_t *) mem_83589)[write_iter_81998];
            bool index_concat_cmp_82224 = sle64(conc_tmp_75017, write_iter_81998);
            int64_t index_concat_branch_82232;
            
            if (index_concat_cmp_82224) {
                int64_t index_concat_i_82225 = sub64(write_iter_81998, conc_tmp_75017);
                int64_t index_concat_82226 = ((int64_t *) mem_83204)[index_concat_i_82225];
                
                index_concat_branch_82232 = index_concat_82226;
            } else {
                bool index_concat_cmp_82227 = sle64(m_74941, write_iter_81998);
                int64_t index_concat_branch_82231;
                
                if (index_concat_cmp_82227) {
                    int64_t index_concat_i_82228 = sub64(write_iter_81998, m_74941);
                    int64_t index_concat_82229 = ((int64_t *) mem_83164)[index_concat_i_82228];
                    
                    index_concat_branch_82231 = index_concat_82229;
                } else {
                    int64_t index_concat_82230 = ((int64_t *) mem_83273)[write_iter_81998];
                    
                    index_concat_branch_82231 = index_concat_82230;
                }
                index_concat_branch_82232 = index_concat_branch_82231;
            }
            
            int64_t write_iv_82004 = ((int64_t *) mem_83593)[write_iter_81998];
            bool cond_77213 = write_iv_82001 == (int64_t) 1;
            int64_t lifted_lambda_res_77214;
            
            if (cond_77213) {
                int64_t lifted_lambda_res_t_res_80957 = sub64(write_iv_82002, (int64_t) 1);
                
                lifted_lambda_res_77214 = lifted_lambda_res_t_res_80957;
            } else {
                lifted_lambda_res_77214 = (int64_t) -1;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77214) && slt64(lifted_lambda_res_77214, m_75493)) {
                ((int64_t *) mem_83615)[lifted_lambda_res_77214] = index_concat_branch_82232;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77214) && slt64(lifted_lambda_res_77214, m_75493)) {
                ((int64_t *) mem_83613)[lifted_lambda_res_77214] = write_iv_82004;
            }
        }
        if (mem_83629_cached_sizze_84698 < bytes_83612) {
            err = lexical_realloc(ctx, &mem_83629, &mem_83629_cached_sizze_84698, bytes_83612);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83631_cached_sizze_84699 < bytes_83612) {
            err = lexical_realloc(ctx, &mem_83631, &mem_83631_cached_sizze_84699, bytes_83612);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_82019;
        int64_t scanacc_82013 = (int64_t) 0;
        
        for (int64_t i_82016 = 0; i_82016 < m_75493; i_82016++) {
            int64_t lifted_lambda_res_77164 = ((int64_t *) mem_83615)[i_82016];
            int64_t lifted_lambda_res_77165 = ((int64_t *) mem_83613)[i_82016];
            int64_t defunc_0_f_res_80958;
            int64_t defunc_0_f_res_80959;
            int64_t defunc_0_f_res_80960;
            int64_t redout_82007;
            int64_t redout_82008;
            int64_t redout_82009;
            
            redout_82007 = i_82016;
            redout_82008 = lifted_lambda_res_77164;
            redout_82009 = lifted_lambda_res_77165;
            for (int64_t i_82010 = 0; i_82010 < m_75493; i_82010++) {
                int64_t x_77170 = ((int64_t *) mem_83615)[i_82010];
                int64_t x_77171 = ((int64_t *) mem_83613)[i_82010];
                bool cond_77178 = redout_82009 == lifted_lambda_res_77165;
                bool cond_77179 = !cond_77178;
                bool cond_f_res_77180 = redout_82009 == redout_82008;
                bool y_77181 = cond_77178 && cond_f_res_77180;
                bool cond_77182 = cond_77179 || y_77181;
                bool cond_77183 = x_77171 == lifted_lambda_res_77165;
                bool cond_77184 = slt64(x_77170, redout_82008);
                bool cond_t_res_77185 = x_77170 == x_77171;
                bool cond_t_res_77186 = !cond_t_res_77185;
                bool x_77187 = cond_77184 && cond_t_res_77186;
                bool cond_77188 = x_77170 == redout_82008;
                bool cond_f_res_t_res_f_res_t_res_77189 = slt64(i_82010, redout_82007);
                bool x_77190 = cond_77188 && cond_f_res_t_res_f_res_t_res_77189;
                bool x_77191 = !x_77187;
                bool y_77192 = x_77190 && x_77191;
                bool cond_f_res_t_res_77193 = x_77187 || y_77192;
                bool x_77194 = cond_77183 && cond_f_res_t_res_77193;
                bool x_77195 = !cond_77182;
                bool y_77196 = x_77194 && x_77195;
                bool cond_77197 = cond_77182 || y_77196;
                int64_t lifted_lambda_res_77198;
                
                if (cond_77197) {
                    lifted_lambda_res_77198 = i_82010;
                } else {
                    lifted_lambda_res_77198 = redout_82007;
                }
                
                int64_t lifted_lambda_res_77199;
                
                if (cond_77197) {
                    lifted_lambda_res_77199 = x_77171;
                } else {
                    lifted_lambda_res_77199 = redout_82009;
                }
                
                int64_t lifted_lambda_res_77200;
                
                if (cond_77197) {
                    lifted_lambda_res_77200 = x_77170;
                } else {
                    lifted_lambda_res_77200 = redout_82008;
                }
                
                int64_t redout_tmp_84471 = lifted_lambda_res_77198;
                int64_t redout_tmp_84472 = lifted_lambda_res_77199;
                int64_t redout_tmp_84473 = lifted_lambda_res_77200;
                
                redout_82007 = redout_tmp_84471;
                redout_82008 = redout_tmp_84472;
                redout_82009 = redout_tmp_84473;
            }
            defunc_0_f_res_80958 = redout_82007;
            defunc_0_f_res_80959 = redout_82008;
            defunc_0_f_res_80960 = redout_82009;
            
            bool cond_77201 = i_82016 == defunc_0_f_res_80958;
            bool lifted_lambda_res_t_res_77202 = defunc_0_f_res_80959 == defunc_0_f_res_80960;
            bool lifted_lambda_res_t_res_77203 = !lifted_lambda_res_t_res_77202;
            bool x_77204 = cond_77201 && lifted_lambda_res_t_res_77203;
            int64_t defunc_0_f_res_77206 = btoi_bool_i64(x_77204);
            int64_t defunc_0_op_res_75575 = add64(defunc_0_f_res_77206, scanacc_82013);
            
            ((int64_t *) mem_83629)[i_82016] = defunc_0_op_res_75575;
            ((int64_t *) mem_83631)[i_82016] = defunc_0_f_res_77206;
            
            int64_t scanacc_tmp_84468 = defunc_0_op_res_75575;
            
            scanacc_82013 = scanacc_tmp_84468;
        }
        discard_82019 = scanacc_82013;
        
        bool x_75576 = !empty_slice_75502;
        bool y_75577 = slt64(m_75503, m_75493);
        bool bounds_check_75578 = zzero_leq_i_p_m_t_s_75504 && y_75577;
        bool protect_assert_disj_75579 = empty_slice_75502 || bounds_check_75578;
        bool index_certs_75580;
        
        if (!protect_assert_disj_75579) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) m_75503, "] out of bounds for array of shape [", (long long) m_75493, "].", "-> #0  /prelude/soacs.fut:257:33-47\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t m_f_res_75581;
        
        if (x_75576) {
            int64_t x_80961 = ((int64_t *) mem_83629)[m_75503];
            
            m_f_res_75581 = x_80961;
        } else {
            m_f_res_75581 = (int64_t) 0;
        }
        
        int64_t m_75583;
        
        if (empty_slice_75502) {
            m_75583 = (int64_t) 0;
        } else {
            m_75583 = m_f_res_75581;
        }
        
        int64_t m_75593 = sub64(m_75583, (int64_t) 1);
        bool i_p_m_t_s_leq_w_75595 = slt64(m_75593, m_75493);
        bool zzero_leq_i_p_m_t_s_75594 = sle64((int64_t) 0, m_75593);
        bool y_75597 = zzero_leq_i_p_m_t_s_75594 && i_p_m_t_s_leq_w_75595;
        bool i_lte_j_75596 = sle64((int64_t) 0, m_75583);
        bool forwards_ok_75598 = i_lte_j_75596 && y_75597;
        bool eq_x_zz_75590 = (int64_t) 0 == m_f_res_75581;
        bool p_and_eq_x_y_75591 = x_75576 && eq_x_zz_75590;
        bool empty_slice_75592 = empty_slice_75502 || p_and_eq_x_y_75591;
        bool ok_or_empty_75599 = empty_slice_75592 || forwards_ok_75598;
        bool index_certs_75600;
        
        if (!ok_or_empty_75599) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_75583, "] out of bounds for array of shape [", (long long) m_75493, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_83644 = (int64_t) 8 * m_75583;
        
        if (mem_83661_cached_sizze_84702 < bytes_83296) {
            err = lexical_realloc(ctx, &mem_83661, &mem_83661_cached_sizze_84702, bytes_83296);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83663_cached_sizze_84703 < bytes_83296) {
            err = lexical_realloc(ctx, &mem_83663, &mem_83663_cached_sizze_84703, bytes_83296);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83665_cached_sizze_84704 < bytes_83296) {
            err = lexical_realloc(ctx, &mem_83665, &mem_83665_cached_sizze_84704, bytes_83296);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83667_cached_sizze_84705 < bytes_83296) {
            err = lexical_realloc(ctx, &mem_83667, &mem_83667_cached_sizze_84705, bytes_83296);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_83669_cached_sizze_84706 < bytes_83296) {
            err = lexical_realloc(ctx, &mem_83669, &mem_83669_cached_sizze_84706, bytes_83296);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_82046;
        int64_t scanacc_82034 = (int64_t) 0;
        
        for (int64_t i_82040 = 0; i_82040 < conc_tmp_75018; i_82040++) {
            int64_t tmp_77039 = ((int64_t *) mem_83593)[i_82040];
            bool index_concat_cmp_77041 = sle64(conc_tmp_75017, i_82040);
            double index_concat_branch_77042;
            
            if (index_concat_cmp_77041) {
                int64_t index_concat_i_80963 = sub64(i_82040, conc_tmp_75017);
                double index_concat_80964 = ((double *) mem_83208)[index_concat_i_80963];
                
                index_concat_branch_77042 = index_concat_80964;
            } else {
                bool index_concat_cmp_77045 = sle64(m_74941, i_82040);
                double index_concat_branch_77046;
                
                if (index_concat_cmp_77045) {
                    int64_t index_concat_i_80965 = sub64(i_82040, m_74941);
                    double index_concat_80966 = ((double *) mem_83168)[index_concat_i_80965];
                    
                    index_concat_branch_77046 = index_concat_80966;
                } else {
                    double index_concat_77049 = ((double *) mem_83277)[i_82040];
                    
                    index_concat_branch_77046 = index_concat_77049;
                }
                index_concat_branch_77042 = index_concat_branch_77046;
            }
            
            double index_concat_branch_77050;
            
            if (index_concat_cmp_77041) {
                int64_t index_concat_i_80967 = sub64(i_82040, conc_tmp_75017);
                double index_concat_80968 = ((double *) mem_83206)[index_concat_i_80967];
                
                index_concat_branch_77050 = index_concat_80968;
            } else {
                bool index_concat_cmp_77053 = sle64(m_74941, i_82040);
                double index_concat_branch_77054;
                
                if (index_concat_cmp_77053) {
                    int64_t index_concat_i_80969 = sub64(i_82040, m_74941);
                    double index_concat_80970 = ((double *) mem_83166)[index_concat_i_80969];
                    
                    index_concat_branch_77054 = index_concat_80970;
                } else {
                    double index_concat_77057 = ((double *) mem_83275)[i_82040];
                    
                    index_concat_branch_77054 = index_concat_77057;
                }
                index_concat_branch_77050 = index_concat_branch_77054;
            }
            
            bool zgze_res_77061 = minC_73806 <= index_concat_branch_77050;
            bool zlze_res_77062 = index_concat_branch_77050 <= maxC_73817;
            bool x_77063 = zgze_res_77061 && zlze_res_77062;
            bool eq_x_y_77064 = index_concat_branch_77042 == -INFINITY;
            bool eq_x_zz_77065 = index_concat_branch_77042 == zp_res_73800;
            bool p_and_eq_x_y_77066 = cond_73797 && eq_x_y_77064;
            bool p_and_eq_x_y_77067 = not_p_74548 && eq_x_zz_77065;
            bool zeze_res_77068 = p_and_eq_x_y_77066 || p_and_eq_x_y_77067;
            double d_euclidean_res_77069;
            
            if (zeze_res_77068) {
                double zm_res_80971 = index_concat_branch_77050 - index_concat_branch_77050;
                double abs_res_80972 = fabs64(zm_res_80971);
                
                d_euclidean_res_77069 = abs_res_80972;
            } else {
                double zm_res_77072 = index_concat_branch_77042 - minC_73801;
                double abs_res_77073 = fabs64(zm_res_77072);
                
                d_euclidean_res_77069 = abs_res_77073;
            }
            
            bool zlze_res_77074 = d_euclidean_res_77069 <= 2.0;
            bool eq_x_y_77075 = index_concat_branch_77042 == INFINITY;
            bool eq_x_zz_77076 = index_concat_branch_77042 == zp_res_73811;
            bool p_and_eq_x_y_77077 = cond_73807 && eq_x_y_77075;
            bool p_and_eq_x_y_77078 = not_p_74549 && eq_x_zz_77076;
            bool zeze_res_77079 = p_and_eq_x_y_77077 || p_and_eq_x_y_77078;
            double d_euclidean_res_77080;
            
            if (zeze_res_77079) {
                double zm_res_80973 = index_concat_branch_77050 - index_concat_branch_77050;
                double abs_res_80974 = fabs64(zm_res_80973);
                
                d_euclidean_res_77080 = abs_res_80974;
            } else {
                double zm_res_77083 = index_concat_branch_77042 - maxC_73812;
                double abs_res_77084 = fabs64(zm_res_77083);
                
                d_euclidean_res_77080 = abs_res_77084;
            }
            
            bool zlze_res_77085 = d_euclidean_res_77080 <= 2.0;
            bool x_77086 = !zlze_res_77074;
            bool y_77087 = zlze_res_77085 && x_77086;
            bool cond_t_res_77088 = zlze_res_77074 || y_77087;
            bool x_77089 = x_77063 && cond_t_res_77088;
            bool zgze_res_77090 = minC_73801 <= index_concat_branch_77042;
            bool zlze_res_77091 = index_concat_branch_77042 <= maxC_73812;
            bool x_77092 = zgze_res_77090 && zlze_res_77091;
            double zm_res_77093 = index_concat_branch_77050 - minC_73806;
            double abs_res_77094 = fabs64(zm_res_77093);
            bool zlze_res_77095 = abs_res_77094 <= 2.0;
            double zm_res_77096 = index_concat_branch_77050 - maxC_73817;
            double abs_res_77097 = fabs64(zm_res_77096);
            bool zlze_res_77098 = abs_res_77097 <= 2.0;
            bool x_77099 = !zlze_res_77095;
            bool y_77100 = zlze_res_77098 && x_77099;
            bool cond_f_res_t_res_77101 = zlze_res_77095 || y_77100;
            bool x_77102 = x_77092 && cond_f_res_t_res_77101;
            bool x_77103 = !x_77089;
            bool y_77104 = x_77102 && x_77103;
            bool cond_77105 = x_77089 || y_77104;
            bool zlze_res_77106 = index_concat_branch_77042 <= minC_73801;
            double corn_x_77107;
            
            if (zlze_res_77106) {
                corn_x_77107 = minC_73801;
            } else {
                corn_x_77107 = maxC_73812;
            }
            
            bool zlze_res_77108 = index_concat_branch_77050 <= minC_73806;
            double corn_y_77109;
            
            if (zlze_res_77108) {
                corn_y_77109 = minC_73806;
            } else {
                corn_y_77109 = maxC_73817;
            }
            
            bool p_and_eq_x_y_77110 = zeze_res_77068 && zlze_res_77106;
            bool not_p_77111 = !zlze_res_77106;
            bool p_and_eq_x_y_77112 = zeze_res_77079 && not_p_77111;
            bool zeze_res_77113 = p_and_eq_x_y_77110 || p_and_eq_x_y_77112;
            double d_euclidean_res_77114;
            
            if (zeze_res_77113) {
                double zm_res_80975 = index_concat_branch_77050 - corn_y_77109;
                double abs_res_80976 = fabs64(zm_res_80975);
                
                d_euclidean_res_77114 = abs_res_80976;
            } else {
                bool eq_x_y_77117 = index_concat_branch_77050 == -INFINITY;
                bool eq_x_zz_77118 = index_concat_branch_77050 == zp_res_73805;
                bool p_and_eq_x_y_77119 = cond_73802 && eq_x_y_77117;
                bool not_p_77120 = !cond_73802;
                bool p_and_eq_x_y_77121 = eq_x_zz_77118 && not_p_77120;
                bool eq_x_y_77122 = p_and_eq_x_y_77119 || p_and_eq_x_y_77121;
                bool eq_x_y_77123 = index_concat_branch_77050 == INFINITY;
                bool eq_x_zz_77124 = index_concat_branch_77050 == zp_res_73816;
                bool p_and_eq_x_y_77125 = cond_73813 && eq_x_y_77123;
                bool not_p_77126 = !cond_73813;
                bool p_and_eq_x_y_77127 = eq_x_zz_77124 && not_p_77126;
                bool eq_x_zz_77128 = p_and_eq_x_y_77125 || p_and_eq_x_y_77127;
                bool p_and_eq_x_y_77129 = zlze_res_77108 && eq_x_y_77122;
                bool not_p_77130 = !zlze_res_77108;
                bool p_and_eq_x_y_77131 = eq_x_zz_77128 && not_p_77130;
                bool zeze_res_77132 = p_and_eq_x_y_77129 || p_and_eq_x_y_77131;
                double d_euclidean_res_f_res_77133;
                
                if (zeze_res_77132) {
                    double zm_res_80977 = index_concat_branch_77042 - corn_x_77107;
                    double abs_res_80978 = fabs64(zm_res_80977);
                    
                    d_euclidean_res_f_res_77133 = abs_res_80978;
                } else {
                    double zm_res_77136 = index_concat_branch_77050 - corn_y_77109;
                    double zm_res_77137 = index_concat_branch_77042 - corn_x_77107;
                    double hypot_res_77138 = futrts_hypot64(zm_res_77137, zm_res_77136);
                    
                    d_euclidean_res_f_res_77133 = hypot_res_77138;
                }
                d_euclidean_res_77114 = d_euclidean_res_f_res_77133;
            }
            
            bool zlze_res_77139 = d_euclidean_res_77114 <= 2.0;
            bool x_77140 = !cond_77105;
            bool y_77141 = zlze_res_77139 && x_77140;
            bool is_pt_marginal_res_77142 = cond_77105 || y_77141;
            int64_t defunc_0_f_res_77143 = btoi_bool_i64(is_pt_marginal_res_77142);
            int64_t defunc_0_op_res_75731 = add64(defunc_0_f_res_77143, scanacc_82034);
            
            ((int64_t *) mem_83661)[i_82040] = defunc_0_op_res_75731;
            ((int64_t *) mem_83663)[i_82040] = defunc_0_f_res_77143;
            ((int64_t *) mem_83665)[i_82040] = tmp_77039;
            ((double *) mem_83667)[i_82040] = index_concat_branch_77042;
            ((double *) mem_83669)[i_82040] = index_concat_branch_77050;
            
            int64_t scanacc_tmp_84474 = defunc_0_op_res_75731;
            
            scanacc_82034 = scanacc_tmp_84474;
        }
        discard_82046 = scanacc_82034;
        
        int64_t m_f_res_75732;
        
        if (x_75330) {
            int64_t x_80979 = ((int64_t *) mem_83661)[tmp_75331];
            
            m_f_res_75732 = x_80979;
        } else {
            m_f_res_75732 = (int64_t) 0;
        }
        
        int64_t m_75734;
        
        if (eq_x_zz_75287) {
            m_75734 = (int64_t) 0;
        } else {
            m_75734 = m_f_res_75732;
        }
        
        int64_t m_75744 = sub64(m_75734, (int64_t) 1);
        bool i_p_m_t_s_leq_w_75746 = slt64(m_75744, conc_tmp_75018);
        bool zzero_leq_i_p_m_t_s_75745 = sle64((int64_t) 0, m_75744);
        bool y_75748 = zzero_leq_i_p_m_t_s_75745 && i_p_m_t_s_leq_w_75746;
        bool i_lte_j_75747 = sle64((int64_t) 0, m_75734);
        bool forwards_ok_75749 = i_lte_j_75747 && y_75748;
        bool eq_x_zz_75741 = (int64_t) 0 == m_f_res_75732;
        bool p_and_eq_x_y_75742 = x_75330 && eq_x_zz_75741;
        bool empty_slice_75743 = eq_x_zz_75287 || p_and_eq_x_y_75742;
        bool ok_or_empty_75750 = empty_slice_75743 || forwards_ok_75749;
        bool index_certs_75751;
        
        if (!ok_or_empty_75750) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_75734, "] out of bounds for array of shape [", (long long) conc_tmp_75018, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_83700 = (int64_t) 8 * m_75734;
        int64_t max_res_75765 = smax64((int64_t) 1, m_75734);
        bool zzero_75766 = max_res_75765 == (int64_t) 0;
        bool nonzzero_75767 = !zzero_75766;
        bool nonzzero_cert_75768;
        
        if (!nonzzero_75767) {
            set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../ftDBSCAN_plus.fut:733:58-94\n   #1  test_dbscan_plus.fut:14:23-19:30\n   #2  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t max_arg1_75769 = sdiv64((int64_t) 9223372036854775807, max_res_75765);
        int64_t max_res_75770 = smax64((int64_t) 1, max_arg1_75769);
        int64_t zm_lhs_75771 = add64(m_74650, max_res_75770);
        int64_t zs_lhs_75772 = sub64(zm_lhs_75771, (int64_t) 1);
        bool zzero_75773 = max_res_75770 == (int64_t) 0;
        bool nonzzero_75774 = !zzero_75773;
        bool nonzzero_cert_75775;
        
        if (!nonzzero_75774) {
            set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../ftDBSCAN_plus.fut:734:71-81\n   #1  test_dbscan_plus.fut:14:23-19:30\n   #2  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t max_arg1_75776 = sdiv64(zs_lhs_75772, max_res_75770);
        int64_t max_res_75777 = smax64((int64_t) 0, max_arg1_75776);
        bool bounds_invalid_upwards_75779 = slt64(max_res_75777, (int64_t) 0);
        bool valid_75780 = !bounds_invalid_upwards_75779;
        bool range_valid_c_75781;
        
        if (!valid_75780) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) max_res_75777, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  ../../ftDBSCAN_plus.fut:736:75-88\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t zm_lhs_75837 = add64(dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747, max_res_75029);
        int64_t zs_lhs_75838 = sub64(zm_lhs_75837, (int64_t) 1);
        int64_t max_arg1_75842 = sdiv64(zs_lhs_75838, max_res_75029);
        int64_t max_res_75843 = smax64((int64_t) 0, max_arg1_75842);
        bool bounds_invalid_upwards_75845 = slt64(max_res_75843, (int64_t) 0);
        bool valid_75846 = !bounds_invalid_upwards_75845;
        bool range_valid_c_75847;
        
        if (!valid_75846) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) max_res_75843, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  ../../ftDBSCAN_plus.fut:753:75-88\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t conc_tmp_75923 = loop_dz2085Uz2081U_73702 + m_75583;
        int64_t bytes_83768 = (int64_t) 8 * conc_tmp_75923;
        bool cond_75926 = new_proc_74486 == (int64_t) 24;
        bool x_75927 = !cond_75926;
        bool cond_75928 = slt64((int64_t) 0, m_73974);
        bool protect_cond_conj_75929 = x_75927 && cond_75928;
        int64_t next_partition_to_read_res_f_res_t_res_75930;
        
        if (protect_cond_conj_75929) {
            int64_t x_80999 = ((int64_t *) mem_82752.mem)[(int64_t) 0];
            
            next_partition_to_read_res_f_res_t_res_75930 = x_80999;
        } else {
            next_partition_to_read_res_f_res_t_res_75930 = (int64_t) 0;
        }
        
        int64_t next_partition_to_read_res_f_res_75932;
        
        if (x_75927) {
            bool x_81456 = !cond_75928;
            int64_t defunc_res_81051;
            
            if (x_81456) {
                int64_t x_81458;
                int64_t redout_82087 = (int64_t) 0;
                
                for (int64_t i_82088 = 0; i_82088 < (int64_t) 25; i_82088++) {
                    bool eta_p_81052 = ((bool *) mem_82699.mem)[i_82088];
                    bool lifted_lambda_res_81053 = !eta_p_81052;
                    int64_t bool_res_81054 = btoi_bool_i64(lifted_lambda_res_81053);
                    int64_t zp_res_81057 = add64(bool_res_81054, redout_82087);
                    int64_t redout_tmp_84480 = zp_res_81057;
                    
                    redout_82087 = redout_tmp_84480;
                }
                x_81458 = redout_82087;
                defunc_res_81051 = x_81458;
            } else {
                defunc_res_81051 = (int64_t) 0;
            }
            
            bool cond_81062 = slt64((int64_t) 0, defunc_res_81051);
            int64_t next_partition_to_read_res_f_res_f_res_81063;
            
            if (x_81456) {
                int64_t x_81460;
                
                if (cond_81062) {
                    if (mem_83772_cached_sizze_84711 < (int64_t) 200) {
                        err = lexical_realloc(ctx, &mem_83772, &mem_83772_cached_sizze_84711, (int64_t) 200);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    if (mem_83773_cached_sizze_84712 < (int64_t) 200) {
                        err = lexical_realloc(ctx, &mem_83773, &mem_83773_cached_sizze_84712, (int64_t) 200);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    
                    int64_t discard_82097;
                    int64_t scanacc_82091 = (int64_t) 0;
                    
                    for (int64_t i_82094 = 0; i_82094 < (int64_t) 25; i_82094++) {
                        bool eta_p_81067 = ((bool *) mem_82699.mem)[i_82094];
                        bool lifted_lambda_res_81068 = !eta_p_81067;
                        int64_t defunc_0_f_res_81069 = btoi_bool_i64(lifted_lambda_res_81068);
                        int64_t defunc_0_op_res_81072 = add64(defunc_0_f_res_81069, scanacc_82091);
                        
                        ((int64_t *) mem_83772)[i_82094] = defunc_0_op_res_81072;
                        ((int64_t *) mem_83773)[i_82094] = defunc_0_f_res_81069;
                        
                        int64_t scanacc_tmp_84481 = defunc_0_op_res_81072;
                        
                        scanacc_82091 = scanacc_tmp_84481;
                    }
                    discard_82097 = scanacc_82091;
                    
                    int64_t x_81083 = ((int64_t *) mem_83772)[(int64_t) 24];
                    int64_t m_81084 = sub64(x_81083, (int64_t) 1);
                    bool i_p_m_t_s_leq_w_81085 = slt64(m_81084, (int64_t) 25);
                    bool zzero_leq_i_p_m_t_s_81086 = sle64((int64_t) 0, m_81084);
                    bool y_81087 = i_p_m_t_s_leq_w_81085 && zzero_leq_i_p_m_t_s_81086;
                    bool i_lte_j_81088 = sle64((int64_t) 0, x_81083);
                    bool forwards_ok_81089 = y_81087 && i_lte_j_81088;
                    bool eq_x_zz_81090 = (int64_t) 0 == x_81083;
                    bool ok_or_empty_81091 = forwards_ok_81089 || eq_x_zz_81090;
                    bool index_certs_81092;
                    
                    if (!ok_or_empty_81091) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) x_81083, "] out of bounds for array of shape [", (long long) (int64_t) 25, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t bytes_83786 = (int64_t) 8 * x_81083;
                    bool y_81106 = slt64((int64_t) 0, x_81083);
                    bool index_certs_81107;
                    
                    if (!y_81106) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) x_81083, "].", "-> #0  ../../ftDBSCAN_plus.fut:267:44-56\n   #1  test_dbscan_plus.fut:14:23-19:30\n   #2  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    if (mem_83787_cached_sizze_84713 < bytes_83786) {
                        err = lexical_realloc(ctx, &mem_83787, &mem_83787_cached_sizze_84713, bytes_83786);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    for (int64_t i_84484 = 0; i_84484 < x_81083; i_84484++) {
                        int64_t x_84485 = (int64_t) 0 + i_84484 * (int64_t) 1;
                        
                        ((int64_t *) mem_83787)[i_84484] = x_84485;
                    }
                    for (int64_t write_iter_82098 = 0; write_iter_82098 < (int64_t) 25; write_iter_82098++) {
                        int64_t write_iv_82100 = ((int64_t *) mem_83773)[write_iter_82098];
                        int64_t write_iv_82101 = ((int64_t *) mem_83772)[write_iter_82098];
                        bool cond_81098 = write_iv_82100 == (int64_t) 1;
                        int64_t lifted_lambda_res_81099;
                        
                        if (cond_81098) {
                            int64_t lifted_lambda_res_t_res_81100 = sub64(write_iv_82101, (int64_t) 1);
                            
                            lifted_lambda_res_81099 = lifted_lambda_res_t_res_81100;
                        } else {
                            lifted_lambda_res_81099 = (int64_t) -1;
                        }
                        if (sle64((int64_t) 0, lifted_lambda_res_81099) && slt64(lifted_lambda_res_81099, x_81083)) {
                            ((int64_t *) mem_83787)[lifted_lambda_res_81099] = write_iter_82098;
                        }
                    }
                    
                    int64_t next_partition_to_read_res_f_res_f_res_t_res_81108 = ((int64_t *) mem_83787)[(int64_t) 0];
                    
                    x_81460 = next_partition_to_read_res_f_res_f_res_t_res_81108;
                } else {
                    x_81460 = (int64_t) -1;
                }
                next_partition_to_read_res_f_res_f_res_81063 = x_81460;
            } else {
                next_partition_to_read_res_f_res_f_res_81063 = (int64_t) 0;
            }
            
            int64_t x_81050;
            
            if (cond_75928) {
                x_81050 = next_partition_to_read_res_f_res_t_res_75930;
            } else {
                x_81050 = next_partition_to_read_res_f_res_f_res_81063;
            }
            next_partition_to_read_res_f_res_75932 = x_81050;
        } else {
            next_partition_to_read_res_f_res_75932 = (int64_t) 0;
        }
        
        int64_t next_partition_to_read_res_75978;
        
        if (cond_75926) {
            next_partition_to_read_res_75978 = (int64_t) -1;
        } else {
            next_partition_to_read_res_75978 = next_partition_to_read_res_f_res_75932;
        }
        
        bool cond_75979 = sle64((int64_t) 0, next_partition_to_read_res_75978);
        int64_t dzlz7bUZLzpZRz20UZLZLzpZRz20Udz2085Uz20Udz2082UZRz20Unz7dUzg_75980 = add64(dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747, partition_sizze_74386);
        int64_t conc_tmp_75981 = conc_tmp_74485 + dzlz7bUZLzpZRz20UZLZLzpZRz20Udz2085Uz20Udz2082UZRz20Unz7dUzg_75980;
        int64_t loopres_75982;
        
        if (cond_75979) {
            loopres_75982 = conc_tmp_74485;
        } else {
            loopres_75982 = conc_tmp_75981;
        }
        
        int64_t bytes_83794 = (int64_t) 8 * loopres_75982;
        bool x_75992 = !cond_75979;
        bool cond_75993 = sle64(conc_tmp_75923, (int64_t) 1);
        int64_t zs_lhs_75994 = sub64(conc_tmp_75923, (int64_t) 1);
        int64_t zp_rhs_75995 = sdiv_safe64(zs_lhs_75994, (int64_t) 20);
        int64_t min_num_blocks_75996 = add64((int64_t) 1, zp_rhs_75995);
        bool loop_cond_75997 = slt64((int64_t) 1, min_num_blocks_75996);
        bool x_75998;
        int64_t x_75999;
        bool loop_while_76000;
        int64_t x_76001;
        
        loop_while_76000 = loop_cond_75997;
        x_76001 = (int64_t) 1;
        while (loop_while_76000) {
            int64_t loopres_76002 = mul64((int64_t) 2, x_76001);
            bool loop_cond_76003 = slt64(loopres_76002, min_num_blocks_75996);
            bool loop_while_tmp_84487 = loop_cond_76003;
            int64_t x_tmp_84488 = loopres_76002;
            
            loop_while_76000 = loop_while_tmp_84487;
            x_76001 = x_tmp_84488;
        }
        x_75998 = loop_while_76000;
        x_75999 = x_76001;
        
        bool zzero_76004 = x_75999 == (int64_t) 0;
        bool nonzzero_76005 = !zzero_76004;
        bool protect_assert_disj_76006 = cond_75993 || nonzzero_76005;
        bool protect_assert_disj_76007 = cond_75979 || protect_assert_disj_76006;
        bool nonzzero_cert_76008;
        
        if (!protect_assert_disj_76007) {
            set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:234:30-42\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #2  /prelude/functional.fut:9:44-45\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t zp_rhs_76009 = sdiv_safe64(zs_lhs_75994, x_75999);
        int64_t block_sizze_76010 = add64((int64_t) 1, zp_rhs_76009);
        bool y_76017 = slt64((int64_t) 0, conc_tmp_75923);
        bool protect_assert_disj_76018 = cond_75993 || y_76017;
        bool protect_assert_disj_76019 = cond_75979 || protect_assert_disj_76018;
        bool index_certs_76020;
        
        if (!protect_assert_disj_76019) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) conc_tmp_75923, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:240:60-64\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #2  /prelude/functional.fut:9:44-45\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        bool bounds_invalid_upwards_76045 = slt64(x_75999, (int64_t) 0);
        bool valid_76046 = !bounds_invalid_upwards_76045;
        bool protect_assert_disj_76047 = cond_75993 || valid_76046;
        bool protect_assert_disj_76048 = cond_75979 || protect_assert_disj_76047;
        bool range_valid_c_76049;
        
        if (!protect_assert_disj_76048) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) x_75999, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:243:35-248:37\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #4  /prelude/functional.fut:9:44-45\n   #5  test_dbscan_plus.fut:14:23-19:30\n   #6  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        bool bounds_invalid_upwards_76050 = slt64(block_sizze_76010, (int64_t) 0);
        bool valid_76051 = !bounds_invalid_upwards_76050;
        bool protect_assert_disj_76052 = cond_75993 || valid_76051;
        bool protect_assert_disj_76053 = cond_75979 || protect_assert_disj_76052;
        bool range_valid_c_76054;
        
        if (!protect_assert_disj_76053) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) block_sizze_76010, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:244:40-246:44\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:243:35-248:37\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  test_dbscan_plus.fut:14:23-19:30\n   #9  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        bool cond_76055 = sle64(block_sizze_76010, (int64_t) 1);
        int64_t tmp_76056 = sub64(block_sizze_76010, (int64_t) 2);
        bool bounds_invalid_upwards_76057 = slt64(tmp_76056, (int64_t) 0);
        bool valid_76059 = !bounds_invalid_upwards_76057;
        bool protect_assert_disj_76060 = cond_76055 || valid_76059;
        bool protect_assert_disj_76061 = cond_75993 || protect_assert_disj_76060;
        bool protect_assert_disj_76062 = cond_75979 || protect_assert_disj_76061;
        bool range_valid_c_76063;
        
        if (!protect_assert_disj_76062) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "...", (long long) tmp_76056, " is invalid.", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:166:19-28\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:243:35-248:37\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        bool empty_slice_76067 = conc_tmp_75923 == (int64_t) 0;
        bool zzero_leq_i_p_m_t_s_76068 = sle64((int64_t) 0, zs_lhs_75994);
        bool y_76070 = slt64(zs_lhs_75994, conc_tmp_75923);
        bool bounds_check_76071 = zzero_leq_i_p_m_t_s_76068 && y_76070;
        bool protect_assert_disj_76072 = empty_slice_76067 || bounds_check_76071;
        bool protect_assert_disj_76073 = cond_75979 || protect_assert_disj_76072;
        bool index_certs_76074;
        
        if (!protect_assert_disj_76073) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) zs_lhs_75994, "] out of bounds for array of shape [", (long long) conc_tmp_75923, "].", "-> #0  /prelude/soacs.fut:257:33-47\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        bool eq_x_zz_76610 = conc_tmp_74485 == conc_tmp_75981;
        bool p_and_eq_x_y_76611 = x_75992 && eq_x_zz_76610;
        bool eq_x_y_76612 = cond_75979 || p_and_eq_x_y_76611;
        bool eq_x_y_76613 = conc_tmp_75981 == conc_tmp_74485;
        bool p_and_eq_x_y_76614 = cond_75979 && eq_x_y_76613;
        bool eq_x_zz_76615 = x_75992 || p_and_eq_x_y_76614;
        bool p_and_eq_x_y_76616 = cond_75979 && eq_x_y_76612;
        bool p_and_eq_x_y_76617 = x_75992 && eq_x_zz_76615;
        bool dim_match_76618 = p_and_eq_x_y_76616 || p_and_eq_x_y_76617;
        bool empty_or_match_cert_76619;
        
        if (!dim_match_76618) {
            set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "function arguments of wrong shape", "-> #0  ../../ftDBSCAN_plus.fut:1137:29-53\n   #1  test_dbscan_plus.fut:14:23-19:30\n   #2  test_dbscan_plus.fut:13:1-20:55\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t conc_tmp_76620 = loop_dz2084Uz2087U_73698 + loopres_75982;
        int64_t bytes_84059 = (int64_t) 8 * conc_tmp_76620;
        int64_t conc_tmp_76623 = loop_dz2084Uz2088U_73699 + loopres_75982;
        int64_t bytes_84063 = (int64_t) 8 * conc_tmp_76623;
        
        if (mem_82901_cached_sizze_84618 < bytes_82900) {
            err = lexical_realloc(ctx, &mem_82901, &mem_82901_cached_sizze_84618, bytes_82900);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_82903_cached_sizze_84619 < bytes_82900) {
            err = lexical_realloc(ctx, &mem_82903, &mem_82903_cached_sizze_84619, bytes_82900);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_82905_cached_sizze_84620 < bytes_82900) {
            err = lexical_realloc(ctx, &mem_82905, &mem_82905_cached_sizze_84620, bytes_82900);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t i_81652 = 0; i_81652 < j_m_i_74412; i_81652++) {
            int64_t slice_82360 = partition_sizze_74386 + i_81652;
            int64_t eta_p_74463 = ((int64_t *) mem_82862)[slice_82360];
            bool x_74464 = sle64((int64_t) 0, eta_p_74463);
            bool y_74465 = slt64(eta_p_74463, dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74249);
            bool bounds_check_74466 = x_74464 && y_74465;
            bool index_certs_74467;
            
            if (!bounds_check_74466) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_74463, "] out of bounds for array of shape [", (long long) dzlz7bUZLzpZRz20Udz2082Uz20Udz2084Uz2083Uz2082Uz7dUzg_74249, "].", "-> #0  ../../ftDBSCAN_plus.fut:439:60-72\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../ftDBSCAN_plus.fut:439:44-88\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool y_74477 = slt64(eta_p_74463, conc_tmp_74251);
            bool bounds_check_74478 = x_74464 && y_74477;
            bool index_certs_74479;
            
            if (!bounds_check_74478) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_74463, "] out of bounds for array of shape [", (long long) conc_tmp_74251, "].", "-> #0  ../../ftDBSCAN_plus.fut:439:74-86\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../ftDBSCAN_plus.fut:439:44-88\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool index_concat_cmp_74468 = sle64(loop_dz2085Uz2088U_73705, eta_p_74463);
            double index_concat_branch_74469;
            
            if (index_concat_cmp_74468) {
                int64_t index_concat_i_80876 = sub64(eta_p_74463, loop_dz2085Uz2088U_73705);
                double index_concat_80877 = ((double *) mem_82807)[index_concat_i_80876];
                
                index_concat_branch_74469 = index_concat_80877;
            } else {
                double index_concat_74472 = ((double *) mem_param_82673.mem)[eta_p_74463];
                
                index_concat_branch_74469 = index_concat_74472;
            }
            
            double index_concat_branch_74473;
            
            if (index_concat_cmp_74468) {
                int64_t index_concat_i_80878 = sub64(eta_p_74463, loop_dz2085Uz2088U_73705);
                double index_concat_80879 = ((double *) mem_82809)[index_concat_i_80878];
                
                index_concat_branch_74473 = index_concat_80879;
            } else {
                double index_concat_74476 = ((double *) mem_param_82676.mem)[eta_p_74463];
                
                index_concat_branch_74473 = index_concat_74476;
            }
            
            bool index_concat_cmp_74480 = sle64(loop_dz2085Uz2087U_73704, eta_p_74463);
            int64_t index_concat_branch_74481;
            
            if (index_concat_cmp_74480) {
                int64_t index_concat_i_80880 = sub64(eta_p_74463, loop_dz2085Uz2087U_73704);
                int64_t index_concat_80881 = ((int64_t *) mem_82812)[index_concat_i_80880];
                
                index_concat_branch_74481 = index_concat_80881;
            } else {
                int64_t index_concat_74484 = ((int64_t *) mem_param_82670.mem)[eta_p_74463];
                
                index_concat_branch_74481 = index_concat_74484;
            }
            ((double *) mem_82901)[i_81652] = index_concat_branch_74469;
            ((double *) mem_82903)[i_81652] = index_concat_branch_74473;
            ((int64_t *) mem_82905)[i_81652] = index_concat_branch_74481;
        }
        if (memblock_alloc(ctx, &mem_82925, bytes_82869, "mem_82925")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_82925.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82870, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {partition_sizze_74386});
        if (memblock_alloc(ctx, &mem_82927, bytes_82869, "mem_82927")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_82927.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82872, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {partition_sizze_74386});
        if (mem_83645_cached_sizze_84700 < bytes_83644) {
            err = lexical_realloc(ctx, &mem_83645, &mem_83645_cached_sizze_84700, bytes_83644);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83645, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83613, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75583});
        if (mem_83647_cached_sizze_84701 < bytes_83644) {
            err = lexical_realloc(ctx, &mem_83647, &mem_83647_cached_sizze_84701, bytes_83644);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83647, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83615, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75583});
        for (int64_t write_iter_82020 = 0; write_iter_82020 < m_75493; write_iter_82020++) {
            int64_t write_iv_82023 = ((int64_t *) mem_83631)[write_iter_82020];
            int64_t write_iv_82024 = ((int64_t *) mem_83629)[write_iter_82020];
            int64_t write_iv_82025 = ((int64_t *) mem_83615)[write_iter_82020];
            int64_t write_iv_82026 = ((int64_t *) mem_83613)[write_iter_82020];
            bool cond_77150 = write_iv_82023 == (int64_t) 1;
            int64_t lifted_lambda_res_77151;
            
            if (cond_77150) {
                int64_t lifted_lambda_res_t_res_80962 = sub64(write_iv_82024, (int64_t) 1);
                
                lifted_lambda_res_77151 = lifted_lambda_res_t_res_80962;
            } else {
                lifted_lambda_res_77151 = (int64_t) -1;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77151) && slt64(lifted_lambda_res_77151, m_75583)) {
                ((int64_t *) mem_83647)[lifted_lambda_res_77151] = write_iv_82025;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_77151) && slt64(lifted_lambda_res_77151, m_75583)) {
                ((int64_t *) mem_83645)[lifted_lambda_res_77151] = write_iv_82026;
            }
        }
        if (mem_83701_cached_sizze_84707 < bytes_83700) {
            err = lexical_realloc(ctx, &mem_83701, &mem_83701_cached_sizze_84707, bytes_83700);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83701, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83669, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75734});
        if (mem_83703_cached_sizze_84708 < bytes_83700) {
            err = lexical_realloc(ctx, &mem_83703, &mem_83703_cached_sizze_84708, bytes_83700);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83703, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83667, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75734});
        if (mem_83705_cached_sizze_84709 < bytes_83700) {
            err = lexical_realloc(ctx, &mem_83705, &mem_83705_cached_sizze_84709, bytes_83700);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83705, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83665, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75734});
        for (int64_t write_iter_82047 = 0; write_iter_82047 < conc_tmp_75018; write_iter_82047++) {
            int64_t write_iv_82051 = ((int64_t *) mem_83663)[write_iter_82047];
            int64_t write_iv_82052 = ((int64_t *) mem_83661)[write_iter_82047];
            int64_t write_iv_82053 = ((int64_t *) mem_83665)[write_iter_82047];
            double write_iv_82054 = ((double *) mem_83667)[write_iter_82047];
            double write_iv_82055 = ((double *) mem_83669)[write_iter_82047];
            bool cond_76940 = write_iv_82051 == (int64_t) 1;
            int64_t lifted_lambda_res_76941;
            
            if (cond_76940) {
                int64_t lifted_lambda_res_t_res_80980 = sub64(write_iv_82052, (int64_t) 1);
                
                lifted_lambda_res_76941 = lifted_lambda_res_t_res_80980;
            } else {
                lifted_lambda_res_76941 = (int64_t) -1;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_76941) && slt64(lifted_lambda_res_76941, m_75734)) {
                ((int64_t *) mem_83705)[lifted_lambda_res_76941] = write_iv_82053;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_76941) && slt64(lifted_lambda_res_76941, m_75734)) {
                ((double *) mem_83703)[lifted_lambda_res_76941] = write_iv_82054;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_76941) && slt64(lifted_lambda_res_76941, m_75734)) {
                ((double *) mem_83701)[lifted_lambda_res_76941] = write_iv_82055;
            }
        }
        if (mem_83725_cached_sizze_84710 < bytes_82999) {
            err = lexical_realloc(ctx, &mem_83725, &mem_83725_cached_sizze_84710, bytes_82999);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t nest_i_84497 = 0; nest_i_84497 < m_74650; nest_i_84497++) {
            ((int64_t *) mem_83725)[nest_i_84497] = (int64_t) -1;
        }
        for (int64_t i_75783 = 0; i_75783 < max_res_75777; i_75783++) {
            int64_t inf_75785 = mul64(max_res_75770, i_75783);
            int64_t min_arg1_75786 = add64(max_res_75770, inf_75785);
            int64_t min_res_75787 = smin64(m_74650, min_arg1_75786);
            int64_t dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_75788 = sub64(min_res_75787, inf_75785);
            bool empty_slice_75789 = dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_75788 == (int64_t) 0;
            int64_t m_75790 = sub64(dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_75788, (int64_t) 1);
            int64_t i_p_m_t_s_75791 = add64(inf_75785, m_75790);
            bool zzero_leq_i_p_m_t_s_75792 = sle64((int64_t) 0, i_p_m_t_s_75791);
            bool i_p_m_t_s_leq_w_75793 = slt64(i_p_m_t_s_75791, m_74650);
            bool zzero_lte_i_75794 = sle64((int64_t) 0, inf_75785);
            bool i_lte_j_75795 = sle64(inf_75785, min_res_75787);
            bool y_75796 = i_p_m_t_s_leq_w_75793 && zzero_lte_i_75794;
            bool y_75797 = zzero_leq_i_p_m_t_s_75792 && y_75796;
            bool forwards_ok_75798 = i_lte_j_75795 && y_75797;
            bool ok_or_empty_75799 = empty_slice_75789 || forwards_ok_75798;
            bool index_certs_75800;
            
            if (!ok_or_empty_75799) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) inf_75785, ":", (long long) min_res_75787, "] out of bounds for array of shape [", (long long) m_74650, "].", "-> #0  ../../ftDBSCAN_plus.fut:739:48-69\n   #1  test_dbscan_plus.fut:14:23-19:30\n   #2  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            for (int64_t i_82064 = 0; i_82064 < dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_75788; i_82064++) {
                int64_t slice_82209 = inf_75785 + i_82064;
                bool index_concat_cmp_82211 = sle64(partition_sizze_74797, slice_82209);
                double index_concat_branch_82215;
                
                if (index_concat_cmp_82211) {
                    double index_concat_82213 = ((double *) mem_83063)[slice_82209];
                    
                    index_concat_branch_82215 = index_concat_82213;
                } else {
                    double index_concat_82214 = ((double *) mem_83063)[slice_82209];
                    
                    index_concat_branch_82215 = index_concat_82214;
                }
                
                double index_concat_branch_82208;
                
                if (index_concat_cmp_82211) {
                    double index_concat_82206 = ((double *) mem_83065)[slice_82209];
                    
                    index_concat_branch_82208 = index_concat_82206;
                } else {
                    double index_concat_82207 = ((double *) mem_83065)[slice_82209];
                    
                    index_concat_branch_82208 = index_concat_82207;
                }
                
                int64_t defunc_0_f_res_80985;
                double defunc_0_f_res_80986;
                int64_t redout_82059;
                double redout_82060;
                
                redout_82059 = (int64_t) -1;
                redout_82060 = INFINITY;
                for (int64_t i_82061 = 0; i_82061 < m_75734; i_82061++) {
                    double eta_p_79120 = ((double *) mem_83703)[i_82061];
                    double eta_p_79121 = ((double *) mem_83701)[i_82061];
                    int64_t x_79122 = ((int64_t *) mem_83705)[i_82061];
                    bool zeze_res_79123 = index_concat_branch_82215 == eta_p_79120;
                    double d_euclidean_res_79124;
                    
                    if (zeze_res_79123) {
                        double zm_res_80981 = index_concat_branch_82208 - eta_p_79121;
                        double abs_res_80982 = fabs64(zm_res_80981);
                        
                        d_euclidean_res_79124 = abs_res_80982;
                    } else {
                        bool zeze_res_79127 = index_concat_branch_82208 == eta_p_79121;
                        double d_euclidean_res_f_res_79128;
                        
                        if (zeze_res_79127) {
                            double zm_res_80983 = index_concat_branch_82215 - eta_p_79120;
                            double abs_res_80984 = fabs64(zm_res_80983);
                            
                            d_euclidean_res_f_res_79128 = abs_res_80984;
                        } else {
                            double zm_res_79131 = index_concat_branch_82208 - eta_p_79121;
                            double zm_res_79132 = index_concat_branch_82215 - eta_p_79120;
                            double hypot_res_79133 = futrts_hypot64(zm_res_79132, zm_res_79131);
                            
                            d_euclidean_res_f_res_79128 = hypot_res_79133;
                        }
                        d_euclidean_res_79124 = d_euclidean_res_f_res_79128;
                    }
                    
                    bool zg_res_75828 = 2.0 < redout_82060;
                    bool zg_res_75829 = 2.0 < d_euclidean_res_79124;
                    bool x_75830 = zg_res_75828 && zg_res_75829;
                    int64_t lifted_lambda_res_75831;
                    double lifted_lambda_res_75832;
                    
                    if (x_75830) {
                        lifted_lambda_res_75831 = (int64_t) -1;
                        lifted_lambda_res_75832 = INFINITY;
                    } else {
                        bool zl_res_75833 = redout_82060 < d_euclidean_res_79124;
                        int64_t lifted_lambda_res_f_res_75834;
                        
                        if (zl_res_75833) {
                            lifted_lambda_res_f_res_75834 = redout_82059;
                        } else {
                            lifted_lambda_res_f_res_75834 = x_79122;
                        }
                        
                        double lifted_lambda_res_f_res_75835;
                        
                        if (zl_res_75833) {
                            lifted_lambda_res_f_res_75835 = redout_82060;
                        } else {
                            lifted_lambda_res_f_res_75835 = d_euclidean_res_79124;
                        }
                        lifted_lambda_res_75831 = lifted_lambda_res_f_res_75834;
                        lifted_lambda_res_75832 = lifted_lambda_res_f_res_75835;
                    }
                    
                    int64_t redout_tmp_84500 = lifted_lambda_res_75831;
                    double redout_tmp_84501 = lifted_lambda_res_75832;
                    
                    redout_82059 = redout_tmp_84500;
                    redout_82060 = redout_tmp_84501;
                }
                defunc_0_f_res_80985 = redout_82059;
                defunc_0_f_res_80986 = redout_82060;
                ((int64_t *) mem_83725)[inf_75785 + i_82064] = defunc_0_f_res_80985;
            }
        }
        if (memblock_alloc(ctx, &mem_83741, bytes_82700, "mem_83741")) {
            err = 1;
            goto cleanup;
        }
        for (int64_t nest_i_84502 = 0; nest_i_84502 < dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747; nest_i_84502++) {
            ((int64_t *) mem_83741.mem)[nest_i_84502] = (int64_t) -1;
        }
        for (int64_t i_75849 = 0; i_75849 < max_res_75843; i_75849++) {
            int64_t inf_75851 = mul64(max_res_75029, i_75849);
            int64_t min_arg1_75852 = add64(max_res_75029, inf_75851);
            int64_t min_res_75853 = smin64(dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747, min_arg1_75852);
            int64_t dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_75854 = sub64(min_res_75853, inf_75851);
            bool empty_slice_75855 = dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_75854 == (int64_t) 0;
            int64_t m_75856 = sub64(dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_75854, (int64_t) 1);
            int64_t i_p_m_t_s_75857 = add64(inf_75851, m_75856);
            bool zzero_leq_i_p_m_t_s_75858 = sle64((int64_t) 0, i_p_m_t_s_75857);
            bool i_p_m_t_s_leq_w_75859 = slt64(i_p_m_t_s_75857, dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747);
            bool zzero_lte_i_75860 = sle64((int64_t) 0, inf_75851);
            bool i_lte_j_75861 = sle64(inf_75851, min_res_75853);
            bool y_75862 = i_p_m_t_s_leq_w_75859 && zzero_lte_i_75860;
            bool y_75863 = zzero_leq_i_p_m_t_s_75858 && y_75862;
            bool forwards_ok_75864 = i_lte_j_75861 && y_75863;
            bool ok_or_empty_75865 = empty_slice_75855 || forwards_ok_75864;
            bool index_certs_75866;
            
            if (!ok_or_empty_75865) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) inf_75851, ":", (long long) min_res_75853, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747, "].", "-> #0  ../../ftDBSCAN_plus.fut:756:48-70\n   #1  test_dbscan_plus.fut:14:23-19:30\n   #2  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t slice_75867 = inf_73735 + inf_75851;
            
            for (int64_t i_82071 = 0; i_82071 < dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_75854; i_82071++) {
                int64_t slice_82201 = slice_75867 + i_82071;
                double eta_p_75871 = ((double *) mem_82617)[slice_82201];
                double eta_p_75872 = ((double *) mem_82619)[slice_82201];
                int64_t defunc_0_f_res_80994;
                double defunc_0_f_res_80995;
                int64_t redout_82066;
                double redout_82067;
                
                redout_82066 = (int64_t) -1;
                redout_82067 = INFINITY;
                for (int64_t i_82068 = 0; i_82068 < conc_tmp_75018; i_82068++) {
                    double eta_p_79155 = ((double *) mem_83667)[i_82068];
                    double eta_p_79156 = ((double *) mem_83669)[i_82068];
                    int64_t x_79157 = ((int64_t *) mem_83665)[i_82068];
                    bool zeze_res_79158 = eta_p_75871 == eta_p_79155;
                    double d_euclidean_res_79159;
                    
                    if (zeze_res_79158) {
                        double zm_res_80990 = eta_p_75872 - eta_p_79156;
                        double abs_res_80991 = fabs64(zm_res_80990);
                        
                        d_euclidean_res_79159 = abs_res_80991;
                    } else {
                        bool zeze_res_79162 = eta_p_75872 == eta_p_79156;
                        double d_euclidean_res_f_res_79163;
                        
                        if (zeze_res_79162) {
                            double zm_res_80992 = eta_p_75871 - eta_p_79155;
                            double abs_res_80993 = fabs64(zm_res_80992);
                            
                            d_euclidean_res_f_res_79163 = abs_res_80993;
                        } else {
                            double zm_res_79166 = eta_p_75872 - eta_p_79156;
                            double zm_res_79167 = eta_p_75871 - eta_p_79155;
                            double hypot_res_79168 = futrts_hypot64(zm_res_79167, zm_res_79166);
                            
                            d_euclidean_res_f_res_79163 = hypot_res_79168;
                        }
                        d_euclidean_res_79159 = d_euclidean_res_f_res_79163;
                    }
                    
                    bool zg_res_75895 = 2.0 < redout_82067;
                    bool zg_res_75896 = 2.0 < d_euclidean_res_79159;
                    bool x_75897 = zg_res_75895 && zg_res_75896;
                    int64_t lifted_lambda_res_75898;
                    double lifted_lambda_res_75899;
                    
                    if (x_75897) {
                        lifted_lambda_res_75898 = (int64_t) -1;
                        lifted_lambda_res_75899 = INFINITY;
                    } else {
                        bool zl_res_75900 = redout_82067 < d_euclidean_res_79159;
                        int64_t lifted_lambda_res_f_res_75901;
                        
                        if (zl_res_75900) {
                            lifted_lambda_res_f_res_75901 = redout_82066;
                        } else {
                            lifted_lambda_res_f_res_75901 = x_79157;
                        }
                        
                        double lifted_lambda_res_f_res_75902;
                        
                        if (zl_res_75900) {
                            lifted_lambda_res_f_res_75902 = redout_82067;
                        } else {
                            lifted_lambda_res_f_res_75902 = d_euclidean_res_79159;
                        }
                        lifted_lambda_res_75898 = lifted_lambda_res_f_res_75901;
                        lifted_lambda_res_75899 = lifted_lambda_res_f_res_75902;
                    }
                    
                    int64_t redout_tmp_84505 = lifted_lambda_res_75898;
                    double redout_tmp_84506 = lifted_lambda_res_75899;
                    
                    redout_82066 = redout_tmp_84505;
                    redout_82067 = redout_tmp_84506;
                }
                defunc_0_f_res_80994 = redout_82066;
                defunc_0_f_res_80995 = redout_82067;
                ((int64_t *) mem_83741.mem)[inf_75851 + i_82071] = defunc_0_f_res_80994;
            }
        }
        for (int64_t write_iter_82073 = 0; write_iter_82073 < m_74650; write_iter_82073++) {
            bool index_concat_cmp_82195 = sle64(partition_sizze_74797, write_iter_82073);
            int64_t index_concat_branch_82199;
            
            if (index_concat_cmp_82195) {
                int64_t index_concat_82197 = ((int64_t *) mem_83070)[write_iter_82073];
                
                index_concat_branch_82199 = index_concat_82197;
            } else {
                int64_t index_concat_82198 = ((int64_t *) mem_83070)[write_iter_82073];
                
                index_concat_branch_82199 = index_concat_82198;
            }
            
            int64_t write_iv_82076 = ((int64_t *) mem_83725)[write_iter_82073];
            
            if (sle64((int64_t) 0, index_concat_branch_82199) && slt64(index_concat_branch_82199, partition_sizze_74386)) {
                ((int64_t *) mem_82875.mem)[index_concat_branch_82199] = write_iv_82076;
            }
        }
        for (int64_t write_iter_82078 = 0; write_iter_82078 < partition_sizze_74797; write_iter_82078++) {
            int64_t write_iv_82080 = ((int64_t *) mem_83070)[write_iter_82078];
            bool write_iv_82081 = ((bool *) mem_83144)[write_iter_82078];
            
            if (sle64((int64_t) 0, write_iv_82080) && slt64(write_iv_82080, partition_sizze_74386)) {
                ((bool *) mem_82873.mem)[write_iv_82080] = write_iv_82081;
            }
        }
        
        int64_t defunc_0_reduce_res_81431;
        int64_t redout_82083 = (int64_t) -9223372036854775808;
        
        for (int64_t i_82084 = 0; i_82084 < dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747; i_82084++) {
            int64_t x_75911 = ((int64_t *) mem_83741.mem)[i_82084];
            int64_t max_res_75914 = smax64(x_75911, redout_82083);
            int64_t redout_tmp_84509 = max_res_75914;
            
            redout_82083 = redout_tmp_84509;
        }
        defunc_0_reduce_res_81431 = redout_82083;
        
        int64_t defunc_0_reduce_res_81432;
        int64_t redout_82085 = (int64_t) -9223372036854775808;
        
        for (int64_t i_82086 = 0; i_82086 < m_74650; i_82086++) {
            int64_t x_75916 = ((int64_t *) mem_83725)[i_82086];
            int64_t max_res_75919 = smax64(x_75916, redout_82085);
            int64_t redout_tmp_84510 = max_res_75919;
            
            redout_82085 = redout_tmp_84510;
        }
        defunc_0_reduce_res_81432 = redout_82085;
        
        int64_t max_res_75920 = smax64(defunc_0_reduce_res_81431, defunc_0_reduce_res_81432);
        int64_t max_arg1_75921 = add64((int64_t) 1, max_res_75920);
        int64_t max_res_75922 = smax64(old_clHandler_73721, max_arg1_75921);
        
        if (memblock_alloc(ctx, &mem_83769, bytes_83768, "mem_83769")) {
            err = 1;
            goto cleanup;
        }
        
        int64_t tmp_offs_84511 = (int64_t) 0;
        
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83769.mem, tmp_offs_84511, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_82661.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {loop_dz2085Uz2081U_73702});
        tmp_offs_84511 += loop_dz2085Uz2081U_73702;
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83769.mem, tmp_offs_84511, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83647, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75583});
        tmp_offs_84511 += m_75583;
        if (memblock_alloc(ctx, &mem_83771, bytes_83768, "mem_83771")) {
            err = 1;
            goto cleanup;
        }
        
        int64_t tmp_offs_84512 = (int64_t) 0;
        
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83771.mem, tmp_offs_84512, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_82664.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {loop_dz2085Uz2081U_73702});
        tmp_offs_84512 += loop_dz2085Uz2081U_73702;
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83771.mem, tmp_offs_84512, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83645, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_75583});
        tmp_offs_84512 += m_75583;
        if (memblock_alloc(ctx, &mem_83795, bytes_83794, "mem_83795")) {
            err = 1;
            goto cleanup;
        }
        
        int64_t tmp_offs_84513 = (int64_t) 0;
        
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83795.mem, tmp_offs_84513, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82807, (int64_t) 0 + (int64_t) 1 * partition_sizze_74201, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_74235});
        tmp_offs_84513 += j_m_i_74235;
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83795.mem, tmp_offs_84513, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82901, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_74412});
        tmp_offs_84513 += j_m_i_74412;
        if (memblock_alloc(ctx, &mem_83797, bytes_83794, "mem_83797")) {
            err = 1;
            goto cleanup;
        }
        
        int64_t tmp_offs_84514 = (int64_t) 0;
        
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83797.mem, tmp_offs_84514, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82809, (int64_t) 0 + (int64_t) 1 * partition_sizze_74201, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_74235});
        tmp_offs_84514 += j_m_i_74235;
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83797.mem, tmp_offs_84514, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82903, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_74412});
        tmp_offs_84514 += j_m_i_74412;
        if (memblock_alloc(ctx, &mem_83799, bytes_83794, "mem_83799")) {
            err = 1;
            goto cleanup;
        }
        
        int64_t tmp_offs_84515 = (int64_t) 0;
        
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83799.mem, tmp_offs_84515, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82812, (int64_t) 0 + (int64_t) 1 * partition_sizze_74201, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_74235});
        tmp_offs_84515 += j_m_i_74235;
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83799.mem, tmp_offs_84515, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82905, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_74412});
        tmp_offs_84515 += j_m_i_74412;
        if (cond_75979) {
            if (memblock_set(ctx, &ext_mem_83808, &mem_83795, "mem_83795") != 0)
                return 1;
            if (memblock_set(ctx, &ext_mem_83807, &mem_83797, "mem_83797") != 0)
                return 1;
            if (memblock_set(ctx, &ext_mem_83806, &mem_83799, "mem_83799") != 0)
                return 1;
        } else {
            if (memblock_alloc(ctx, &mem_83801, bytes_83794, "mem_83801")) {
                err = 1;
                goto cleanup;
            }
            
            int64_t tmp_offs_84516 = (int64_t) 0;
            
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_83801.mem, tmp_offs_84516, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82807, (int64_t) 0 + (int64_t) 1 * partition_sizze_74201, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_74235});
            tmp_offs_84516 += j_m_i_74235;
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_83801.mem, tmp_offs_84516, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82901, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_74412});
            tmp_offs_84516 += j_m_i_74412;
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_83801.mem, tmp_offs_84516, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82925.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {partition_sizze_74386});
            tmp_offs_84516 += partition_sizze_74386;
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_83801.mem, tmp_offs_84516, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82701.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747});
            tmp_offs_84516 += dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747;
            if (memblock_alloc(ctx, &mem_83803, bytes_83794, "mem_83803")) {
                err = 1;
                goto cleanup;
            }
            
            int64_t tmp_offs_84517 = (int64_t) 0;
            
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_83803.mem, tmp_offs_84517, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82809, (int64_t) 0 + (int64_t) 1 * partition_sizze_74201, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_74235});
            tmp_offs_84517 += j_m_i_74235;
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_83803.mem, tmp_offs_84517, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82903, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_74412});
            tmp_offs_84517 += j_m_i_74412;
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_83803.mem, tmp_offs_84517, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82927.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {partition_sizze_74386});
            tmp_offs_84517 += partition_sizze_74386;
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_83803.mem, tmp_offs_84517, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82703.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747});
            tmp_offs_84517 += dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747;
            if (memblock_alloc(ctx, &mem_83805, bytes_83794, "mem_83805")) {
                err = 1;
                goto cleanup;
            }
            
            int64_t tmp_offs_84518 = (int64_t) 0;
            
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_83805.mem, tmp_offs_84518, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82812, (int64_t) 0 + (int64_t) 1 * partition_sizze_74201, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_74235});
            tmp_offs_84518 += j_m_i_74235;
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_83805.mem, tmp_offs_84518, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82905, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_74412});
            tmp_offs_84518 += j_m_i_74412;
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_83805.mem, tmp_offs_84518, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_82875.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {partition_sizze_74386});
            tmp_offs_84518 += partition_sizze_74386;
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_83805.mem, tmp_offs_84518, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83741.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747});
            tmp_offs_84518 += dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747;
            if (memblock_set(ctx, &ext_mem_83808, &mem_83801, "mem_83801") != 0)
                return 1;
            if (memblock_set(ctx, &ext_mem_83807, &mem_83803, "mem_83803") != 0)
                return 1;
            if (memblock_set(ctx, &ext_mem_83806, &mem_83805, "mem_83805") != 0)
                return 1;
        }
        if (memblock_unref(ctx, &mem_83795, "mem_83795") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83797, "mem_83797") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83799, "mem_83799") != 0)
            return 1;
        
        int64_t flat_dim_76011 = x_75999 * block_sizze_76010;
        bool x_76012 = !cond_75993;
        int64_t greatest_divisor_leq_than_arg1_76013 = mul64((int64_t) 2, block_sizze_76010);
        bool protect_cond_conj_76014 = x_75992 && x_76012;
        int64_t merge_block_sizze_76015;
        
        if (protect_cond_conj_76014) {
            int64_t x_81109;
            
            if (futrts_greatest_divisor_leq_than_5592(ctx, &x_81109, (int64_t) 8, greatest_divisor_leq_than_arg1_76013) != 0) {
                err = 1;
                goto cleanup;
            }
            merge_block_sizze_76015 = x_81109;
        } else {
            merge_block_sizze_76015 = (int64_t) 0;
        }
        
        bool index_concat_cmp_76021 = sle64(loop_dz2085Uz2081U_73702, (int64_t) 0);
        int64_t index_concat_branch_76022;
        
        if (protect_cond_conj_76014) {
            int64_t x_81112;
            
            if (index_concat_cmp_76021) {
                int64_t index_concat_i_81113 = sub64((int64_t) 0, loop_dz2085Uz2081U_73702);
                int64_t index_concat_81114 = ((int64_t *) mem_83647)[index_concat_i_81113];
                
                x_81112 = index_concat_81114;
            } else {
                int64_t index_concat_81115 = ((int64_t *) mem_param_82661.mem)[(int64_t) 0];
                
                x_81112 = index_concat_81115;
            }
            index_concat_branch_76022 = x_81112;
        } else {
            index_concat_branch_76022 = (int64_t) 0;
        }
        
        int64_t index_concat_branch_76027;
        
        if (protect_cond_conj_76014) {
            int64_t x_81118;
            
            if (index_concat_cmp_76021) {
                int64_t index_concat_i_81119 = sub64((int64_t) 0, loop_dz2085Uz2081U_73702);
                int64_t index_concat_81120 = ((int64_t *) mem_83645)[index_concat_i_81119];
                
                x_81118 = index_concat_81120;
            } else {
                int64_t index_concat_81121 = ((int64_t *) mem_param_82664.mem)[(int64_t) 0];
                
                x_81118 = index_concat_81121;
            }
            index_concat_branch_76027 = x_81118;
        } else {
            index_concat_branch_76027 = (int64_t) 0;
        }
        
        int64_t defunc_0_reduce_res_76032;
        int64_t defunc_0_reduce_res_76033;
        
        if (protect_cond_conj_76014) {
            int64_t x_81124;
            int64_t x_81125;
            int64_t redout_82104;
            int64_t redout_82105;
            
            redout_82104 = index_concat_branch_76022;
            redout_82105 = index_concat_branch_76027;
            for (int64_t i_82106 = 0; i_82106 < conc_tmp_75923; i_82106++) {
                bool index_concat_cmp_82187 = sle64(loop_dz2085Uz2081U_73702, i_82106);
                int64_t index_concat_branch_82191;
                
                if (index_concat_cmp_82187) {
                    int64_t index_concat_i_82188 = sub64(i_82106, loop_dz2085Uz2081U_73702);
                    int64_t index_concat_82189 = ((int64_t *) mem_83647)[index_concat_i_82188];
                    
                    index_concat_branch_82191 = index_concat_82189;
                } else {
                    int64_t index_concat_82190 = ((int64_t *) mem_param_82661.mem)[i_82106];
                    
                    index_concat_branch_82191 = index_concat_82190;
                }
                
                int64_t index_concat_branch_82185;
                
                if (index_concat_cmp_82187) {
                    int64_t index_concat_i_82182 = sub64(i_82106, loop_dz2085Uz2081U_73702);
                    int64_t index_concat_82183 = ((int64_t *) mem_83645)[index_concat_i_82182];
                    
                    index_concat_branch_82185 = index_concat_82183;
                } else {
                    int64_t index_concat_82184 = ((int64_t *) mem_param_82664.mem)[i_82106];
                    
                    index_concat_branch_82185 = index_concat_82184;
                }
                
                bool defunc_0_leq_res_81132;
                
                if (futrts_lifted_lambda_12077(ctx, &defunc_0_leq_res_81132, redout_82104, redout_82105, index_concat_branch_82191, index_concat_branch_82185) != 0) {
                    err = 1;
                    goto cleanup;
                }
                
                int64_t defunc_0_op_res_81133;
                
                if (defunc_0_leq_res_81132) {
                    defunc_0_op_res_81133 = index_concat_branch_82191;
                } else {
                    defunc_0_op_res_81133 = redout_82104;
                }
                
                int64_t defunc_0_op_res_81134;
                
                if (defunc_0_leq_res_81132) {
                    defunc_0_op_res_81134 = index_concat_branch_82185;
                } else {
                    defunc_0_op_res_81134 = redout_82105;
                }
                
                int64_t redout_tmp_84519 = defunc_0_op_res_81133;
                int64_t redout_tmp_84520 = defunc_0_op_res_81134;
                
                redout_82104 = redout_tmp_84519;
                redout_82105 = redout_tmp_84520;
            }
            x_81124 = redout_82104;
            x_81125 = redout_82105;
            defunc_0_reduce_res_76032 = x_81124;
            defunc_0_reduce_res_76033 = x_81125;
        } else {
            defunc_0_reduce_res_76032 = (int64_t) 0;
            defunc_0_reduce_res_76033 = (int64_t) 0;
        }
        
        int64_t distance_76058 = add64((int64_t) 1, tmp_76056);
        bool loop_cond_76064 = slt64(block_sizze_76010, flat_dim_76011);
        bool zzero_76065 = merge_block_sizze_76015 == (int64_t) 0;
        bool nonzzero_76066 = !zzero_76065;
        bool x_76069 = !empty_slice_76067;
        int64_t binop_x_83809 = (int64_t) 8 * x_75999;
        int64_t bytes_83810 = block_sizze_76010 * binop_x_83809;
        int64_t bytes_83823 = (int64_t) 8 * block_sizze_76010;
        int64_t loopres_76075;
        int64_t loopres_76076;
        int64_t loopres_76078;
        
        if (cond_75979) {
            if (memblock_set(ctx, &ext_mem_84058, &mem_param_82655, "mem_param_82655") != 0)
                return 1;
            if (memblock_set(ctx, &ext_mem_84055, &mem_param_82658, "mem_param_82658") != 0)
                return 1;
            loopres_76075 = loop_dz2084Uz2089U_73700;
            loopres_76076 = loop_dz2085Uz2080U_73701;
            loopres_76078 = collisions_73713;
        } else {
            if (cond_75993) {
                if (memblock_set(ctx, &ext_mem_84016, &mem_83769, "mem_83769") != 0)
                    return 1;
                if (memblock_set(ctx, &ext_mem_84013, &mem_83771, "mem_83771") != 0)
                    return 1;
            } else {
                if (mem_83811_cached_sizze_84714 < bytes_83810) {
                    err = lexical_realloc(ctx, &mem_83811, &mem_83811_cached_sizze_84714, bytes_83810);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                if (mem_83814_cached_sizze_84715 < bytes_83810) {
                    err = lexical_realloc(ctx, &mem_83814, &mem_83814_cached_sizze_84715, bytes_83810);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                if (mem_83824_cached_sizze_84716 < bytes_83823) {
                    err = lexical_realloc(ctx, &mem_83824, &mem_83824_cached_sizze_84716, bytes_83823);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                if (mem_83826_cached_sizze_84717 < bytes_83823) {
                    err = lexical_realloc(ctx, &mem_83826, &mem_83826_cached_sizze_84717, bytes_83823);
                    if (err != FUTHARK_SUCCESS)
                        goto cleanup;
                }
                for (int64_t i_82118 = 0; i_82118 < x_75999; i_82118++) {
                    int64_t zp_lhs_76087 = mul64(block_sizze_76010, i_82118);
                    
                    for (int64_t i_82111 = 0; i_82111 < block_sizze_76010; i_82111++) {
                        int64_t k_76091 = add64(zp_lhs_76087, i_82111);
                        bool cond_76092 = slt64(k_76091, conc_tmp_75923);
                        int64_t lifted_lambda_res_76093;
                        int64_t lifted_lambda_res_76094;
                        
                        if (cond_76092) {
                            bool x_81145 = sle64((int64_t) 0, k_76091);
                            bool bounds_check_81146 = cond_76092 && x_81145;
                            bool index_certs_81147;
                            
                            if (!bounds_check_81146) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) k_76091, "] out of bounds for array of shape [", (long long) conc_tmp_75923, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:246:26-30\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:244:40-246:44\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:243:35-248:37\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  test_dbscan_plus.fut:14:23-19:30\n   #10 test_dbscan_plus.fut:13:1-20:55\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            bool index_concat_cmp_81148 = sle64(loop_dz2085Uz2081U_73702, k_76091);
                            int64_t index_concat_branch_81149;
                            
                            if (index_concat_cmp_81148) {
                                int64_t index_concat_i_81150 = sub64(k_76091, loop_dz2085Uz2081U_73702);
                                int64_t index_concat_81151 = ((int64_t *) mem_83647)[index_concat_i_81150];
                                
                                index_concat_branch_81149 = index_concat_81151;
                            } else {
                                int64_t index_concat_81152 = ((int64_t *) mem_param_82661.mem)[k_76091];
                                
                                index_concat_branch_81149 = index_concat_81152;
                            }
                            
                            int64_t index_concat_branch_81153;
                            
                            if (index_concat_cmp_81148) {
                                int64_t index_concat_i_81154 = sub64(k_76091, loop_dz2085Uz2081U_73702);
                                int64_t index_concat_81155 = ((int64_t *) mem_83645)[index_concat_i_81154];
                                
                                index_concat_branch_81153 = index_concat_81155;
                            } else {
                                int64_t index_concat_81156 = ((int64_t *) mem_param_82664.mem)[k_76091];
                                
                                index_concat_branch_81153 = index_concat_81156;
                            }
                            lifted_lambda_res_76093 = index_concat_branch_81149;
                            lifted_lambda_res_76094 = index_concat_branch_81153;
                        } else {
                            lifted_lambda_res_76093 = defunc_0_reduce_res_76032;
                            lifted_lambda_res_76094 = defunc_0_reduce_res_76033;
                        }
                        ((int64_t *) mem_83824)[i_82111] = lifted_lambda_res_76093;
                        ((int64_t *) mem_83826)[i_82111] = lifted_lambda_res_76094;
                    }
                    if (!cond_76055) {
                        for (int64_t i_76111 = 0; i_76111 < distance_76058; i_76111++) {
                            int64_t gt_arg1_76114 = add64((int64_t) 1, i_76111);
                            bool x_76115 = sle64((int64_t) 0, gt_arg1_76114);
                            bool y_76116 = slt64(gt_arg1_76114, block_sizze_76010);
                            bool bounds_check_76117 = x_76115 && y_76116;
                            bool index_certs_76118;
                            
                            if (!bounds_check_76117) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) gt_arg1_76114, "] out of bounds for array of shape [", (long long) block_sizze_76010, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:169:31-37\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:243:35-248:37\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            bool y_76121 = slt64(i_76111, block_sizze_76010);
                            bool index_certs_76122;
                            
                            if (!y_76121) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_76111, "] out of bounds for array of shape [", (long long) block_sizze_76010, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:169:26-30\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:243:35-248:37\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t gt_arg1_76119 = ((int64_t *) mem_83824)[gt_arg1_76114];
                            int64_t gt_arg1_76120 = ((int64_t *) mem_83826)[gt_arg1_76114];
                            int64_t gt_arg0_76123 = ((int64_t *) mem_83824)[i_76111];
                            int64_t gt_arg0_76124 = ((int64_t *) mem_83826)[i_76111];
                            bool defunc_0_leq_res_76125;
                            
                            if (futrts_lifted_lambda_12077(ctx, &defunc_0_leq_res_76125, gt_arg0_76123, gt_arg0_76124, gt_arg1_76119, gt_arg1_76120) != 0) {
                                err = 1;
                                goto cleanup;
                            }
                            
                            bool defunc_0_lifted_gt_res_76126 = !defunc_0_leq_res_76125;
                            bool s_76127;
                            int64_t s_76130;
                            bool loop_while_76131;
                            int64_t j_76134;
                            
                            loop_while_76131 = defunc_0_lifted_gt_res_76126;
                            j_76134 = i_76111;
                            while (loop_while_76131) {
                                bool x_76135 = sle64((int64_t) 0, j_76134);
                                bool y_76136 = slt64(j_76134, block_sizze_76010);
                                bool bounds_check_76137 = x_76135 && y_76136;
                                bool index_certs_76138;
                                
                                if (!bounds_check_76137) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_76134, "] out of bounds for array of shape [", (long long) block_sizze_76010, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:170:24-28\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:243:35-248:37\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t copy_arg0_76141 = add64((int64_t) 1, j_76134);
                                bool x_76142 = sle64((int64_t) 0, copy_arg0_76141);
                                bool y_76143 = slt64(copy_arg0_76141, block_sizze_76010);
                                bool bounds_check_76144 = x_76142 && y_76143;
                                bool index_certs_76145;
                                
                                if (!bounds_check_76144) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) copy_arg0_76141, "] out of bounds for array of shape [", (long long) block_sizze_76010, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:171:35-41\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:243:35-248:37\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t copy_arg0_76139 = ((int64_t *) mem_83824)[j_76134];
                                int64_t copy_arg0_76140 = ((int64_t *) mem_83826)[j_76134];
                                int64_t copy_arg0_76146 = ((int64_t *) mem_83824)[copy_arg0_76141];
                                int64_t copy_arg0_76147 = ((int64_t *) mem_83826)[copy_arg0_76141];
                                
                                ((int64_t *) mem_83824)[j_76134] = copy_arg0_76146;
                                ((int64_t *) mem_83826)[j_76134] = copy_arg0_76147;
                                ((int64_t *) mem_83824)[copy_arg0_76141] = copy_arg0_76139;
                                ((int64_t *) mem_83826)[copy_arg0_76141] = copy_arg0_76140;
                                
                                int64_t tmp_76152 = sub64(j_76134, (int64_t) 1);
                                bool cond_76153 = sle64((int64_t) 0, tmp_76152);
                                bool loop_cond_76154;
                                
                                if (cond_76153) {
                                    bool y_81159 = slt64(tmp_76152, block_sizze_76010);
                                    bool bounds_check_81160 = cond_76153 && y_81159;
                                    bool index_certs_81161;
                                    
                                    if (!bounds_check_81160) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_76152, "] out of bounds for array of shape [", (long long) block_sizze_76010, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:169:26-30\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:243:35-248:37\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    int64_t gt_arg1_81157 = ((int64_t *) mem_83824)[j_76134];
                                    int64_t gt_arg1_81158 = ((int64_t *) mem_83826)[j_76134];
                                    int64_t gt_arg0_81162 = ((int64_t *) mem_83824)[tmp_76152];
                                    int64_t gt_arg0_81163 = ((int64_t *) mem_83826)[tmp_76152];
                                    bool defunc_0_leq_res_81164;
                                    
                                    if (futrts_lifted_lambda_12077(ctx, &defunc_0_leq_res_81164, gt_arg0_81162, gt_arg0_81163, gt_arg1_81157, gt_arg1_81158) != 0) {
                                        err = 1;
                                        goto cleanup;
                                    }
                                    
                                    bool defunc_0_lifted_gt_res_81165 = !defunc_0_leq_res_81164;
                                    
                                    loop_cond_76154 = defunc_0_lifted_gt_res_81165;
                                } else {
                                    loop_cond_76154 = 0;
                                }
                                
                                bool loop_while_tmp_84527 = loop_cond_76154;
                                int64_t j_tmp_84530 = tmp_76152;
                                
                                loop_while_76131 = loop_while_tmp_84527;
                                j_76134 = j_tmp_84530;
                            }
                            s_76127 = loop_while_76131;
                            s_76130 = j_76134;
                        }
                    }
                    lmad_copy_8b(ctx, 1, (uint64_t *) mem_83811, i_82118 * block_sizze_76010, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83824, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {block_sizze_76010});
                    lmad_copy_8b(ctx, 1, (uint64_t *) mem_83814, i_82118 * block_sizze_76010, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_83826, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {block_sizze_76010});
                }
                if (memblock_alloc(ctx, &mem_83879, bytes_83810, "mem_83879")) {
                    err = 1;
                    goto cleanup;
                }
                lmad_copy_8b(ctx, 2, (uint64_t *) mem_83879.mem, (int64_t) 0, (int64_t []) {block_sizze_76010, (int64_t) 1}, (uint64_t *) mem_83811, (int64_t) 0, (int64_t []) {block_sizze_76010, (int64_t) 1}, (int64_t []) {x_75999, block_sizze_76010});
                if (memblock_alloc(ctx, &mem_83883, bytes_83810, "mem_83883")) {
                    err = 1;
                    goto cleanup;
                }
                lmad_copy_8b(ctx, 2, (uint64_t *) mem_83883.mem, (int64_t) 0, (int64_t []) {block_sizze_76010, (int64_t) 1}, (uint64_t *) mem_83814, (int64_t) 0, (int64_t []) {block_sizze_76010, (int64_t) 1}, (int64_t []) {x_75999, block_sizze_76010});
                
                int64_t data_76166;
                bool data_76167;
                int64_t data_76170;
                int64_t loop_dz2084Uz2082U_76171;
                bool loop_while_76172;
                int64_t stride_76175;
                
                if (memblock_set(ctx, &mem_param_83887, &mem_83879, "mem_83879") != 0)
                    return 1;
                if (memblock_set(ctx, &mem_param_83890, &mem_83883, "mem_83883") != 0)
                    return 1;
                loop_dz2084Uz2082U_76171 = flat_dim_76011;
                loop_while_76172 = loop_cond_76064;
                stride_76175 = block_sizze_76010;
                while (loop_while_76172) {
                    int64_t next_stride_76176 = mul64((int64_t) 2, stride_76175);
                    bool zzero_76177 = next_stride_76176 == (int64_t) 0;
                    bool nonzzero_76178 = !zzero_76177;
                    bool nonzzero_cert_76179;
                    
                    if (!nonzzero_76178) {
                        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:254:33-46\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #2  /prelude/functional.fut:9:44-45\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t num_merges_76180 = sdiv64(flat_dim_76011, next_stride_76176);
                    bool bounds_invalid_upwards_76181 = slt64(num_merges_76180, (int64_t) 0);
                    bool valid_76182 = !bounds_invalid_upwards_76181;
                    bool range_valid_c_76183;
                    
                    if (!valid_76182) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) num_merges_76180, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #4  /prelude/functional.fut:9:44-45\n   #5  test_dbscan_plus.fut:14:23-19:30\n   #6  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t m_76185 = sub64(next_stride_76176, (int64_t) 1);
                    bool zzero_leq_i_p_m_t_s_76186 = sle64((int64_t) 0, m_76185);
                    bool cond_76188 = slt64(next_stride_76176, (int64_t) 10);
                    bool protect_assert_disj_76189 = nonzzero_76066 || cond_76188;
                    bool nonzzero_cert_76190;
                    
                    if (!protect_assert_disj_76189) {
                        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:147:30-42\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t zeze_lhs_76191 = smod_safe64(next_stride_76176, merge_block_sizze_76015);
                    bool num_blocks_76192 = zeze_lhs_76191 == (int64_t) 0;
                    bool protect_assert_disj_76193 = cond_76188 || num_blocks_76192;
                    bool assert_c_76194;
                    
                    if (!protect_assert_disj_76193) {
                        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "Assertion is false: (n % block_size == 0)", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:147:20-65\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t num_blocks_76196 = sdiv_safe64(next_stride_76176, merge_block_sizze_76015);
                    int64_t dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_76197 = add64((int64_t) 1, num_blocks_76196);
                    int64_t j_m_i_76198 = sub64(next_stride_76176, stride_76175);
                    int64_t flat_dim_76199 = merge_block_sizze_76015 * num_blocks_76196;
                    bool bounds_invalid_upwards_76200 = slt64(dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_76197, (int64_t) 0);
                    bool zzero_lte_i_76201 = sle64((int64_t) 0, stride_76175);
                    bool i_lte_j_76202 = sle64(stride_76175, next_stride_76176);
                    bool empty_slice_76203 = stride_76175 == (int64_t) 0;
                    int64_t m_76204 = sub64(stride_76175, (int64_t) 1);
                    bool bounds_invalid_upwards_76205 = slt64(num_blocks_76196, (int64_t) 0);
                    bool empty_slice_76206 = j_m_i_76198 == (int64_t) 0;
                    int64_t m_76207 = sub64(j_m_i_76198, (int64_t) 1);
                    bool cond_76208 = slt64((int64_t) 0, stride_76175);
                    bool valid_76209 = !bounds_invalid_upwards_76200;
                    bool zzero_leq_i_p_m_t_s_76210 = sle64((int64_t) 0, m_76204);
                    bool i_p_m_t_s_leq_w_76211 = slt64(m_76204, next_stride_76176);
                    bool valid_76212 = !bounds_invalid_upwards_76205;
                    bool i_p_m_t_s_leq_w_76213 = slt64(m_76185, flat_dim_76199);
                    int64_t i_p_m_t_s_76214 = add64(stride_76175, m_76207);
                    bool y_76215 = slt64((int64_t) 0, j_m_i_76198);
                    bool protect_assert_disj_76216 = cond_76188 || valid_76209;
                    bool range_valid_c_76217;
                    
                    if (!protect_assert_disj_76216) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_76197, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  test_dbscan_plus.fut:14:23-19:30\n   #10 test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    bool y_76218 = zzero_leq_i_p_m_t_s_76210 && i_p_m_t_s_leq_w_76211;
                    bool protect_assert_disj_76219 = cond_76188 || valid_76212;
                    bool range_valid_c_76220;
                    
                    if (!protect_assert_disj_76219) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) num_blocks_76196, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  test_dbscan_plus.fut:14:23-19:30\n   #9  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    bool y_76221 = zzero_leq_i_p_m_t_s_76186 && i_p_m_t_s_leq_w_76213;
                    bool zzero_leq_i_p_m_t_s_76222 = sle64((int64_t) 0, i_p_m_t_s_76214);
                    bool i_p_m_t_s_leq_w_76223 = slt64(i_p_m_t_s_76214, next_stride_76176);
                    bool forwards_ok_76224 = zzero_lte_i_76201 && y_76218;
                    bool ok_or_empty_76225 = zzero_76177 || y_76221;
                    bool y_76226 = zzero_lte_i_76201 && i_p_m_t_s_leq_w_76223;
                    bool protect_assert_disj_76227 = cond_76208 || y_76215;
                    bool ok_or_empty_76228 = empty_slice_76203 || forwards_ok_76224;
                    bool protect_assert_disj_76229 = cond_76188 || ok_or_empty_76225;
                    bool index_certs_76230;
                    
                    if (!protect_assert_disj_76229) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) next_stride_76176, "] out of bounds for array of shape [", (long long) flat_dim_76199, "].", "-> #0  /prelude/array.fut:46:45-51\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:155:3-46\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  test_dbscan_plus.fut:14:23-19:30\n   #9  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    bool y_76231 = zzero_leq_i_p_m_t_s_76222 && y_76226;
                    bool loop_not_taken_76232 = !cond_76188;
                    bool protect_assert_disj_76233 = protect_assert_disj_76227 || loop_not_taken_76232;
                    bool index_certs_76234;
                    
                    if (!protect_assert_disj_76233) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) j_m_i_76198, "].", "-> #0  /prelude/array.fut:22:29-33\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:100:48-54\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  test_dbscan_plus.fut:14:23-19:30\n   #10 test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    bool protect_assert_disj_76235 = cond_76188 || ok_or_empty_76228;
                    bool index_certs_76236;
                    
                    if (!protect_assert_disj_76235) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) stride_76175, "] out of bounds for array of shape [", (long long) next_stride_76176, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:52-59\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #8  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #9  /prelude/functional.fut:9:44-45\n   #10 test_dbscan_plus.fut:14:23-19:30\n   #11 test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    bool forwards_ok_76237 = i_lte_j_76202 && y_76231;
                    bool protect_assert_disj_76238 = ok_or_empty_76228 || loop_not_taken_76232;
                    bool index_certs_76239;
                    
                    if (!protect_assert_disj_76238) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) stride_76175, "] out of bounds for array of shape [", (long long) next_stride_76176, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:26-33\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    bool ok_or_empty_76240 = empty_slice_76206 || forwards_ok_76237;
                    bool protect_assert_disj_76241 = loop_not_taken_76232 || ok_or_empty_76240;
                    bool index_certs_76242;
                    
                    if (!protect_assert_disj_76241) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) stride_76175, ":] out of bounds for array of shape [", (long long) next_stride_76176, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:34-41\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    bool protect_assert_disj_76243 = cond_76188 || ok_or_empty_76240;
                    bool index_certs_76244;
                    
                    if (!protect_assert_disj_76243) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) stride_76175, ":] out of bounds for array of shape [", (long long) next_stride_76176, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:60-67\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #8  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #9  /prelude/functional.fut:9:44-45\n   #10 test_dbscan_plus.fut:14:23-19:30\n   #11 test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t binop_x_83891 = (int64_t) 8 * num_merges_76180;
                    int64_t bytes_83892 = next_stride_76176 * binop_x_83891;
                    bool i_lte_j_76187 = sle64((int64_t) 0, next_stride_76176);
                    
                    if (mem_83893_cached_sizze_84718 < bytes_83892) {
                        err = lexical_realloc(ctx, &mem_83893, &mem_83893_cached_sizze_84718, bytes_83892);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    if (mem_83896_cached_sizze_84719 < bytes_83892) {
                        err = lexical_realloc(ctx, &mem_83896, &mem_83896_cached_sizze_84719, bytes_83892);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    
                    int64_t bytes_83905 = (int64_t) 8 * dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_76197;
                    int64_t binop_x_83921 = (int64_t) 8 * num_blocks_76196;
                    int64_t bytes_83922 = merge_block_sizze_76015 * binop_x_83921;
                    int64_t bytes_83967 = (int64_t) 16 * stride_76175;
                    
                    for (int64_t i_82139 = 0; i_82139 < num_merges_76180; i_82139++) {
                        int64_t start_76248 = mul64(next_stride_76176, i_82139);
                        int64_t j_m_i_76249 = sub64(loop_dz2084Uz2082U_76171, start_76248);
                        bool empty_slice_76250 = j_m_i_76249 == (int64_t) 0;
                        int64_t m_76251 = sub64(j_m_i_76249, (int64_t) 1);
                        int64_t i_p_m_t_s_76252 = add64(start_76248, m_76251);
                        bool zzero_leq_i_p_m_t_s_76253 = sle64((int64_t) 0, i_p_m_t_s_76252);
                        bool i_p_m_t_s_leq_w_76254 = slt64(i_p_m_t_s_76252, loop_dz2084Uz2082U_76171);
                        bool zzero_lte_i_76255 = sle64((int64_t) 0, start_76248);
                        bool i_lte_j_76256 = sle64(start_76248, loop_dz2084Uz2082U_76171);
                        bool y_76257 = i_p_m_t_s_leq_w_76254 && zzero_lte_i_76255;
                        bool y_76258 = zzero_leq_i_p_m_t_s_76253 && y_76257;
                        bool forwards_ok_76259 = i_lte_j_76256 && y_76258;
                        bool ok_or_empty_76260 = empty_slice_76250 || forwards_ok_76259;
                        bool index_certs_76261;
                        
                        if (!ok_or_empty_76260) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) start_76248, ":] out of bounds for array of shape [", (long long) loop_dz2084Uz2082U_76171, "].", "-> #0  /prelude/array.fut:52:49-54\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:258:41-56\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        bool i_p_m_t_s_leq_w_76262 = slt64(m_76185, j_m_i_76249);
                        bool y_76263 = zzero_leq_i_p_m_t_s_76186 && i_p_m_t_s_leq_w_76262;
                        bool forwards_ok_76264 = i_lte_j_76187 && y_76263;
                        bool ok_or_empty_76265 = zzero_76177 || forwards_ok_76264;
                        bool index_certs_76266;
                        
                        if (!ok_or_empty_76265) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) next_stride_76176, "] out of bounds for array of shape [", (long long) j_m_i_76249, "].", "-> #0  /prelude/array.fut:46:45-51\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:258:23-56\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  test_dbscan_plus.fut:14:23-19:30\n   #8  test_dbscan_plus.fut:13:1-20:55\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        if (cond_76188) {
                            int64_t dummy_81193;
                            int64_t dummy_81194;
                            
                            if (cond_76208) {
                                int64_t head_res_81195 = ((int64_t *) mem_param_83887.mem)[start_76248];
                                int64_t head_res_81196 = ((int64_t *) mem_param_83890.mem)[start_76248];
                                
                                dummy_81193 = head_res_81195;
                                dummy_81194 = head_res_81196;
                            } else {
                                int64_t slice_81197 = stride_76175 + start_76248;
                                int64_t head_res_81198 = ((int64_t *) mem_param_83887.mem)[slice_81197];
                                int64_t head_res_81199 = ((int64_t *) mem_param_83890.mem)[slice_81197];
                                
                                dummy_81193 = head_res_81198;
                                dummy_81194 = head_res_81199;
                            }
                            if (memblock_alloc(ctx, &mem_83968, bytes_83967, "mem_83968")) {
                                err = 1;
                                goto cleanup;
                            }
                            for (int64_t nest_i_84540 = 0; nest_i_84540 < next_stride_76176; nest_i_84540++) {
                                ((int64_t *) mem_83968.mem)[nest_i_84540] = dummy_81193;
                            }
                            if (memblock_alloc(ctx, &mem_83970, bytes_83967, "mem_83970")) {
                                err = 1;
                                goto cleanup;
                            }
                            for (int64_t nest_i_84541 = 0; nest_i_84541 < next_stride_76176; nest_i_84541++) {
                                ((int64_t *) mem_83970.mem)[nest_i_84541] = dummy_81194;
                            }
                            
                            int64_t data_81202;
                            int64_t i_81206 = (int64_t) 0;
                            
                            for (int64_t k_81205 = 0; k_81205 < next_stride_76176; k_81205++) {
                                int64_t j_81209 = sub64(k_81205, i_81206);
                                bool cond_81210 = j_81209 == j_m_i_76198;
                                bool cond_81211;
                                
                                if (cond_81210) {
                                    cond_81211 = 1;
                                } else {
                                    bool cond_81212 = slt64(i_81206, stride_76175);
                                    bool cond_f_res_81213;
                                    
                                    if (cond_81212) {
                                        bool x_81214 = sle64((int64_t) 0, j_81209);
                                        bool y_81215 = slt64(j_81209, j_m_i_76198);
                                        bool bounds_check_81216 = x_81214 && y_81215;
                                        bool index_certs_81217;
                                        
                                        if (!bounds_check_81216) {
                                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_81209, "] out of bounds for array of shape [", (long long) j_m_i_76198, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:55-59\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  test_dbscan_plus.fut:14:23-19:30\n   #9  test_dbscan_plus.fut:13:1-20:55\n"));
                                            err = FUTHARK_PROGRAM_ERROR;
                                            goto cleanup;
                                        }
                                        
                                        bool x_81222 = sle64((int64_t) 0, i_81206);
                                        bool bounds_check_81223 = cond_81212 && x_81222;
                                        bool index_certs_81224;
                                        
                                        if (!bounds_check_81223) {
                                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_81206, "] out of bounds for array of shape [", (long long) stride_76175, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:50-54\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  test_dbscan_plus.fut:14:23-19:30\n   #9  test_dbscan_plus.fut:13:1-20:55\n"));
                                            err = FUTHARK_PROGRAM_ERROR;
                                            goto cleanup;
                                        }
                                        
                                        int64_t slice_81218 = stride_76175 + j_81209;
                                        int64_t slice_81219 = start_76248 + slice_81218;
                                        int64_t leq_arg1_81220 = ((int64_t *) mem_param_83887.mem)[slice_81219];
                                        int64_t leq_arg1_81221 = ((int64_t *) mem_param_83890.mem)[slice_81219];
                                        int64_t slice_81225 = start_76248 + i_81206;
                                        int64_t leq_arg0_81226 = ((int64_t *) mem_param_83887.mem)[slice_81225];
                                        int64_t leq_arg0_81227 = ((int64_t *) mem_param_83890.mem)[slice_81225];
                                        bool defunc_0_leq_res_81228;
                                        
                                        if (futrts_lifted_lambda_12077(ctx, &defunc_0_leq_res_81228, leq_arg0_81226, leq_arg0_81227, leq_arg1_81220, leq_arg1_81221) != 0) {
                                            err = 1;
                                            goto cleanup;
                                        }
                                        cond_f_res_81213 = defunc_0_leq_res_81228;
                                    } else {
                                        cond_f_res_81213 = 0;
                                    }
                                    cond_81211 = cond_f_res_81213;
                                }
                                
                                int64_t loopres_81229;
                                int64_t loopres_81230;
                                int64_t loopres_81231;
                                
                                if (cond_81211) {
                                    bool x_81233 = sle64((int64_t) 0, i_81206);
                                    bool y_81234 = slt64(i_81206, stride_76175);
                                    bool bounds_check_81235 = x_81233 && y_81234;
                                    bool index_certs_81236;
                                    
                                    if (!bounds_check_81235) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_81206, "] out of bounds for array of shape [", (long long) stride_76175, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:106:17-21\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  test_dbscan_plus.fut:14:23-19:30\n   #9  test_dbscan_plus.fut:13:1-20:55\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    int64_t tmp_81232 = add64((int64_t) 1, i_81206);
                                    int64_t slice_81237 = start_76248 + i_81206;
                                    int64_t tmp_81238 = ((int64_t *) mem_param_83887.mem)[slice_81237];
                                    int64_t tmp_81239 = ((int64_t *) mem_param_83890.mem)[slice_81237];
                                    
                                    loopres_81229 = tmp_81232;
                                    loopres_81230 = tmp_81238;
                                    loopres_81231 = tmp_81239;
                                } else {
                                    bool x_81240 = sle64((int64_t) 0, j_81209);
                                    bool y_81241 = slt64(j_81209, j_m_i_76198);
                                    bool bounds_check_81242 = x_81240 && y_81241;
                                    bool index_certs_81243;
                                    
                                    if (!bounds_check_81242) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_81209, "] out of bounds for array of shape [", (long long) j_m_i_76198, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:108:15-19\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  test_dbscan_plus.fut:14:23-19:30\n   #9  test_dbscan_plus.fut:13:1-20:55\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    int64_t slice_81244 = stride_76175 + j_81209;
                                    int64_t slice_81245 = start_76248 + slice_81244;
                                    int64_t tmp_81246 = ((int64_t *) mem_param_83887.mem)[slice_81245];
                                    int64_t tmp_81247 = ((int64_t *) mem_param_83890.mem)[slice_81245];
                                    
                                    loopres_81229 = i_81206;
                                    loopres_81230 = tmp_81246;
                                    loopres_81231 = tmp_81247;
                                }
                                ((int64_t *) mem_83968.mem)[k_81205] = loopres_81230;
                                ((int64_t *) mem_83970.mem)[k_81205] = loopres_81231;
                                
                                int64_t i_tmp_84542 = loopres_81229;
                                
                                i_81206 = i_tmp_84542;
                            }
                            data_81202 = i_81206;
                            if (memblock_set(ctx, &ext_mem_83988, &mem_83968, "mem_83968") != 0)
                                return 1;
                            if (memblock_set(ctx, &ext_mem_83985, &mem_83970, "mem_83970") != 0)
                                return 1;
                        } else {
                            if (mem_83906_cached_sizze_84720 < bytes_83905) {
                                err = lexical_realloc(ctx, &mem_83906, &mem_83906_cached_sizze_84720, bytes_83905);
                                if (err != FUTHARK_SUCCESS)
                                    goto cleanup;
                            }
                            if (mem_83908_cached_sizze_84721 < bytes_83905) {
                                err = lexical_realloc(ctx, &mem_83908, &mem_83908_cached_sizze_84721, bytes_83905);
                                if (err != FUTHARK_SUCCESS)
                                    goto cleanup;
                            }
                            for (int64_t i_82125 = 0; i_82125 < dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_76197; i_82125++) {
                                int64_t split_count_arg3_76330 = mul64(merge_block_sizze_76015, i_82125);
                                int64_t defunc_0_split_count_res_76331;
                                int64_t defunc_0_split_count_res_76332;
                                int64_t defunc_0_split_count_res_76333;
                                int64_t defunc_0_split_count_res_76334;
                                int64_t defunc_0_split_count_res_76335;
                                
                                if (futrts_lifted_normalizze_11978(ctx, &defunc_0_split_count_res_76331, &defunc_0_split_count_res_76332, &defunc_0_split_count_res_76333, &defunc_0_split_count_res_76334, &defunc_0_split_count_res_76335, (int64_t) 0, stride_76175, (int64_t) 0, j_m_i_76198, split_count_arg3_76330) != 0) {
                                    err = 1;
                                    goto cleanup;
                                }
                                
                                bool loop_cond_76336 = slt64((int64_t) 0, defunc_0_split_count_res_76335);
                                bool defunc_0_split_count_res_76337;
                                int64_t defunc_0_split_count_res_76338;
                                int64_t defunc_0_split_count_res_76339;
                                int64_t defunc_0_split_count_res_76340;
                                int64_t defunc_0_split_count_res_76341;
                                int64_t defunc_0_split_count_res_76342;
                                bool loop_while_76343;
                                int64_t ss_76344;
                                int64_t ss_76345;
                                int64_t tt_76346;
                                int64_t tt_76347;
                                int64_t count_76348;
                                
                                loop_while_76343 = loop_cond_76336;
                                ss_76344 = defunc_0_split_count_res_76331;
                                ss_76345 = defunc_0_split_count_res_76332;
                                tt_76346 = defunc_0_split_count_res_76333;
                                tt_76347 = defunc_0_split_count_res_76334;
                                count_76348 = defunc_0_split_count_res_76335;
                                while (loop_while_76343) {
                                    int64_t zlze_lhs_76349 = sub64(ss_76345, ss_76344);
                                    bool cond_76350 = sle64(zlze_lhs_76349, (int64_t) 0);
                                    int64_t defunc_0_lifted_step_res_76351;
                                    int64_t defunc_0_lifted_step_res_76352;
                                    int64_t defunc_0_lifted_step_res_76353;
                                    int64_t defunc_0_lifted_step_res_76354;
                                    
                                    if (cond_76350) {
                                        int64_t tmp_81250 = add64(tt_76346, count_76348);
                                        
                                        defunc_0_lifted_step_res_76351 = ss_76344;
                                        defunc_0_lifted_step_res_76352 = tmp_81250;
                                        defunc_0_lifted_step_res_76353 = tt_76347;
                                        defunc_0_lifted_step_res_76354 = (int64_t) 0;
                                    } else {
                                        int64_t zlze_lhs_76356 = sub64(tt_76347, tt_76346);
                                        bool cond_76357 = sle64(zlze_lhs_76356, (int64_t) 0);
                                        int64_t defunc_0_lifted_step_res_f_res_76358;
                                        int64_t defunc_0_lifted_step_res_f_res_76359;
                                        int64_t defunc_0_lifted_step_res_f_res_76360;
                                        int64_t defunc_0_lifted_step_res_f_res_76361;
                                        
                                        if (cond_76357) {
                                            int64_t tmp_81251 = add64(ss_76344, count_76348);
                                            
                                            defunc_0_lifted_step_res_f_res_76358 = tmp_81251;
                                            defunc_0_lifted_step_res_f_res_76359 = tt_76346;
                                            defunc_0_lifted_step_res_f_res_76360 = tt_76347;
                                            defunc_0_lifted_step_res_f_res_76361 = (int64_t) 0;
                                        } else {
                                            bool cond_76363 = count_76348 == (int64_t) 1;
                                            int64_t defunc_0_lifted_step_res_f_res_f_res_76364;
                                            int64_t defunc_0_lifted_step_res_f_res_f_res_76365;
                                            int64_t defunc_0_lifted_step_res_f_res_f_res_76366;
                                            int64_t defunc_0_lifted_step_res_f_res_f_res_76367;
                                            
                                            if (cond_76363) {
                                                bool x_81253 = sle64((int64_t) 0, ss_76344);
                                                bool y_81254 = slt64(ss_76344, stride_76175);
                                                bool bounds_check_81255 = x_81253 && y_81254;
                                                bool index_certs_81256;
                                                
                                                if (!bounds_check_81255) {
                                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) ss_76344, "] out of bounds for array of shape [", (long long) stride_76175, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:53:21-27\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #10 ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #11 /prelude/functional.fut:9:44-45\n   #12 test_dbscan_plus.fut:14:23-19:30\n   #13 test_dbscan_plus.fut:13:1-20:55\n"));
                                                    err = FUTHARK_PROGRAM_ERROR;
                                                    goto cleanup;
                                                }
                                                
                                                bool x_81260 = sle64((int64_t) 0, tt_76346);
                                                bool y_81261 = slt64(tt_76346, j_m_i_76198);
                                                bool bounds_check_81262 = x_81260 && y_81261;
                                                bool index_certs_81263;
                                                
                                                if (!bounds_check_81262) {
                                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tt_76346, "] out of bounds for array of shape [", (long long) j_m_i_76198, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:53:14-20\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #10 ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #11 /prelude/functional.fut:9:44-45\n   #12 test_dbscan_plus.fut:14:23-19:30\n   #13 test_dbscan_plus.fut:13:1-20:55\n"));
                                                    err = FUTHARK_PROGRAM_ERROR;
                                                    goto cleanup;
                                                }
                                                
                                                int64_t slice_81257 = start_76248 + ss_76344;
                                                int64_t leq_arg1_81258 = ((int64_t *) mem_param_83887.mem)[slice_81257];
                                                int64_t leq_arg1_81259 = ((int64_t *) mem_param_83890.mem)[slice_81257];
                                                int64_t slice_81264 = stride_76175 + tt_76346;
                                                int64_t slice_81265 = start_76248 + slice_81264;
                                                int64_t leq_arg0_81266 = ((int64_t *) mem_param_83887.mem)[slice_81265];
                                                int64_t leq_arg0_81267 = ((int64_t *) mem_param_83890.mem)[slice_81265];
                                                bool defunc_0_leq_res_81268;
                                                
                                                if (futrts_lifted_lambda_12077(ctx, &defunc_0_leq_res_81268, leq_arg0_81266, leq_arg0_81267, leq_arg1_81258, leq_arg1_81259) != 0) {
                                                    err = 1;
                                                    goto cleanup;
                                                }
                                                
                                                int64_t defunc_0_lifted_step_res_f_res_f_res_t_res_81269;
                                                int64_t defunc_0_lifted_step_res_f_res_f_res_t_res_81270;
                                                
                                                if (defunc_0_leq_res_81268) {
                                                    int64_t tmp_81271 = add64((int64_t) 1, tt_76346);
                                                    
                                                    defunc_0_lifted_step_res_f_res_f_res_t_res_81269 = ss_76344;
                                                    defunc_0_lifted_step_res_f_res_f_res_t_res_81270 = tmp_81271;
                                                } else {
                                                    int64_t tmp_81272 = add64((int64_t) 1, ss_76344);
                                                    
                                                    defunc_0_lifted_step_res_f_res_f_res_t_res_81269 = tmp_81272;
                                                    defunc_0_lifted_step_res_f_res_f_res_t_res_81270 = tt_76346;
                                                }
                                                defunc_0_lifted_step_res_f_res_f_res_76364 = defunc_0_lifted_step_res_f_res_f_res_t_res_81269;
                                                defunc_0_lifted_step_res_f_res_f_res_76365 = defunc_0_lifted_step_res_f_res_f_res_t_res_81270;
                                                defunc_0_lifted_step_res_f_res_f_res_76366 = tt_76347;
                                                defunc_0_lifted_step_res_f_res_f_res_76367 = (int64_t) 0;
                                            } else {
                                                int64_t m_76388 = sdiv64(count_76348, (int64_t) 2);
                                                int64_t n_76389 = sub64(count_76348, m_76388);
                                                bool cond_76390 = n_76389 == (int64_t) 0;
                                                int64_t zgze_lhs_76391 = add64(ss_76344, m_76388);
                                                bool cond_f_res_76392 = sle64(ss_76345, zgze_lhs_76391);
                                                bool x_76393 = !cond_76390;
                                                bool y_76394 = cond_f_res_76392 && x_76393;
                                                bool cond_76395 = cond_76390 || y_76394;
                                                bool leq_y_x_76396;
                                                
                                                if (cond_76395) {
                                                    leq_y_x_76396 = 1;
                                                } else {
                                                    bool x_76397 = sle64((int64_t) 0, zgze_lhs_76391);
                                                    bool y_76398 = slt64(zgze_lhs_76391, stride_76175);
                                                    bool bounds_check_76399 = x_76397 && y_76398;
                                                    bool index_certs_76400;
                                                    
                                                    if (!bounds_check_76399) {
                                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) zgze_lhs_76391, "] out of bounds for array of shape [", (long long) stride_76175, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:73:22-30\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #10 ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #11 /prelude/functional.fut:9:44-45\n   #12 test_dbscan_plus.fut:14:23-19:30\n   #13 test_dbscan_plus.fut:13:1-20:55\n"));
                                                        err = FUTHARK_PROGRAM_ERROR;
                                                        goto cleanup;
                                                    }
                                                    
                                                    int64_t zm_lhs_76404 = add64(tt_76346, n_76389);
                                                    int64_t leq_arg0_76405 = sub64(zm_lhs_76404, (int64_t) 1);
                                                    bool x_76406 = sle64((int64_t) 0, leq_arg0_76405);
                                                    bool y_76407 = slt64(leq_arg0_76405, j_m_i_76198);
                                                    bool bounds_check_76408 = x_76406 && y_76407;
                                                    bool index_certs_76409;
                                                    
                                                    if (!bounds_check_76408) {
                                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) leq_arg0_76405, "] out of bounds for array of shape [", (long long) j_m_i_76198, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:73:11-21\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #10 ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #11 /prelude/functional.fut:9:44-45\n   #12 test_dbscan_plus.fut:14:23-19:30\n   #13 test_dbscan_plus.fut:13:1-20:55\n"));
                                                        err = FUTHARK_PROGRAM_ERROR;
                                                        goto cleanup;
                                                    }
                                                    
                                                    int64_t slice_76401 = start_76248 + zgze_lhs_76391;
                                                    int64_t leq_arg1_76402 = ((int64_t *) mem_param_83887.mem)[slice_76401];
                                                    int64_t leq_arg1_76403 = ((int64_t *) mem_param_83890.mem)[slice_76401];
                                                    int64_t slice_76410 = stride_76175 + leq_arg0_76405;
                                                    int64_t slice_76411 = start_76248 + slice_76410;
                                                    int64_t leq_arg0_76412 = ((int64_t *) mem_param_83887.mem)[slice_76411];
                                                    int64_t leq_arg0_76413 = ((int64_t *) mem_param_83890.mem)[slice_76411];
                                                    bool defunc_0_leq_res_76414;
                                                    
                                                    if (futrts_lifted_lambda_12077(ctx, &defunc_0_leq_res_76414, leq_arg0_76412, leq_arg0_76413, leq_arg1_76402, leq_arg1_76403) != 0) {
                                                        err = 1;
                                                        goto cleanup;
                                                    }
                                                    leq_y_x_76396 = defunc_0_leq_res_76414;
                                                }
                                                
                                                int64_t defunc_0_lifted_step_res_f_res_f_res_f_res_76415;
                                                int64_t defunc_0_lifted_step_res_f_res_f_res_f_res_76416;
                                                int64_t defunc_0_lifted_step_res_f_res_f_res_f_res_76417;
                                                
                                                if (leq_y_x_76396) {
                                                    int64_t tmp_81273 = add64(tt_76346, n_76389);
                                                    int64_t tmp_81274 = sub64(count_76348, n_76389);
                                                    
                                                    defunc_0_lifted_step_res_f_res_f_res_f_res_76415 = tmp_81273;
                                                    defunc_0_lifted_step_res_f_res_f_res_f_res_76416 = tt_76347;
                                                    defunc_0_lifted_step_res_f_res_f_res_f_res_76417 = tmp_81274;
                                                } else {
                                                    int64_t tmp_76420 = add64(tt_76346, n_76389);
                                                    
                                                    defunc_0_lifted_step_res_f_res_f_res_f_res_76415 = tt_76346;
                                                    defunc_0_lifted_step_res_f_res_f_res_f_res_76416 = tmp_76420;
                                                    defunc_0_lifted_step_res_f_res_f_res_f_res_76417 = count_76348;
                                                }
                                                defunc_0_lifted_step_res_f_res_f_res_76364 = ss_76344;
                                                defunc_0_lifted_step_res_f_res_f_res_76365 = defunc_0_lifted_step_res_f_res_f_res_f_res_76415;
                                                defunc_0_lifted_step_res_f_res_f_res_76366 = defunc_0_lifted_step_res_f_res_f_res_f_res_76416;
                                                defunc_0_lifted_step_res_f_res_f_res_76367 = defunc_0_lifted_step_res_f_res_f_res_f_res_76417;
                                            }
                                            defunc_0_lifted_step_res_f_res_76358 = defunc_0_lifted_step_res_f_res_f_res_76364;
                                            defunc_0_lifted_step_res_f_res_76359 = defunc_0_lifted_step_res_f_res_f_res_76365;
                                            defunc_0_lifted_step_res_f_res_76360 = defunc_0_lifted_step_res_f_res_f_res_76366;
                                            defunc_0_lifted_step_res_f_res_76361 = defunc_0_lifted_step_res_f_res_f_res_76367;
                                        }
                                        defunc_0_lifted_step_res_76351 = defunc_0_lifted_step_res_f_res_76358;
                                        defunc_0_lifted_step_res_76352 = defunc_0_lifted_step_res_f_res_76359;
                                        defunc_0_lifted_step_res_76353 = defunc_0_lifted_step_res_f_res_76360;
                                        defunc_0_lifted_step_res_76354 = defunc_0_lifted_step_res_f_res_76361;
                                    }
                                    
                                    int64_t loopres_76421;
                                    int64_t loopres_76422;
                                    int64_t loopres_76423;
                                    int64_t loopres_76424;
                                    int64_t loopres_76425;
                                    
                                    if (futrts_lifted_normalizze_11978(ctx, &loopres_76421, &loopres_76422, &loopres_76423, &loopres_76424, &loopres_76425, defunc_0_lifted_step_res_76351, ss_76345, defunc_0_lifted_step_res_76352, defunc_0_lifted_step_res_76353, defunc_0_lifted_step_res_76354) != 0) {
                                        err = 1;
                                        goto cleanup;
                                    }
                                    
                                    bool loop_cond_76426 = slt64((int64_t) 0, loopres_76425);
                                    bool loop_while_tmp_84547 = loop_cond_76426;
                                    int64_t ss_tmp_84548 = loopres_76421;
                                    int64_t ss_tmp_84549 = loopres_76422;
                                    int64_t tt_tmp_84550 = loopres_76423;
                                    int64_t tt_tmp_84551 = loopres_76424;
                                    int64_t count_tmp_84552 = loopres_76425;
                                    
                                    loop_while_76343 = loop_while_tmp_84547;
                                    ss_76344 = ss_tmp_84548;
                                    ss_76345 = ss_tmp_84549;
                                    tt_76346 = tt_tmp_84550;
                                    tt_76347 = tt_tmp_84551;
                                    count_76348 = count_tmp_84552;
                                }
                                defunc_0_split_count_res_76337 = loop_while_76343;
                                defunc_0_split_count_res_76338 = ss_76344;
                                defunc_0_split_count_res_76339 = ss_76345;
                                defunc_0_split_count_res_76340 = tt_76346;
                                defunc_0_split_count_res_76341 = tt_76347;
                                defunc_0_split_count_res_76342 = count_76348;
                                ((int64_t *) mem_83906)[i_82125] = defunc_0_split_count_res_76338;
                                ((int64_t *) mem_83908)[i_82125] = defunc_0_split_count_res_76340;
                            }
                            if (mem_83923_cached_sizze_84722 < bytes_83922) {
                                err = lexical_realloc(ctx, &mem_83923, &mem_83923_cached_sizze_84722, bytes_83922);
                                if (err != FUTHARK_SUCCESS)
                                    goto cleanup;
                            }
                            if (mem_83926_cached_sizze_84723 < bytes_83922) {
                                err = lexical_realloc(ctx, &mem_83926, &mem_83926_cached_sizze_84723, bytes_83922);
                                if (err != FUTHARK_SUCCESS)
                                    goto cleanup;
                            }
                            for (int64_t i_82132 = 0; i_82132 < num_blocks_76196; i_82132++) {
                                bool y_76432 = slt64(i_82132, dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_76197);
                                bool index_certs_76434;
                                
                                if (!y_76432) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_82132, "] out of bounds for array of shape [", (long long) dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_76197, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:151:22-34\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  test_dbscan_plus.fut:14:23-19:30\n   #10 test_dbscan_plus.fut:13:1-20:55\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t defunc_0_f_res_76435 = ((int64_t *) mem_83906)[i_82132];
                                int64_t defunc_0_f_res_76436 = ((int64_t *) mem_83908)[i_82132];
                                int64_t tmp_76437 = add64((int64_t) 1, i_82132);
                                bool x_76438 = sle64((int64_t) 0, tmp_76437);
                                bool y_76439 = slt64(tmp_76437, dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_76197);
                                bool bounds_check_76440 = x_76438 && y_76439;
                                bool index_certs_76441;
                                
                                if (!bounds_check_76440) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_76437, "] out of bounds for array of shape [", (long long) dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_76197, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:152:22-36\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  test_dbscan_plus.fut:14:23-19:30\n   #10 test_dbscan_plus.fut:13:1-20:55\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t defunc_0_f_res_76442 = ((int64_t *) mem_83906)[tmp_76437];
                                int64_t defunc_0_f_res_76443 = ((int64_t *) mem_83908)[tmp_76437];
                                int64_t merge_sequential_arg2_76444 = add64(stride_76175, defunc_0_f_res_76436);
                                int64_t merge_sequential_arg2_76445 = add64(stride_76175, defunc_0_f_res_76443);
                                int64_t j_m_i_76446 = sub64(merge_sequential_arg2_76445, merge_sequential_arg2_76444);
                                bool empty_slice_76447 = j_m_i_76446 == (int64_t) 0;
                                int64_t m_76448 = sub64(j_m_i_76446, (int64_t) 1);
                                int64_t i_p_m_t_s_76449 = add64(merge_sequential_arg2_76444, m_76448);
                                bool zzero_leq_i_p_m_t_s_76450 = sle64((int64_t) 0, i_p_m_t_s_76449);
                                bool i_p_m_t_s_leq_w_76451 = slt64(i_p_m_t_s_76449, next_stride_76176);
                                bool zzero_lte_i_76452 = sle64((int64_t) 0, merge_sequential_arg2_76444);
                                bool i_lte_j_76453 = sle64(merge_sequential_arg2_76444, merge_sequential_arg2_76445);
                                bool y_76454 = i_p_m_t_s_leq_w_76451 && zzero_lte_i_76452;
                                bool y_76455 = zzero_leq_i_p_m_t_s_76450 && y_76454;
                                bool forwards_ok_76456 = i_lte_j_76453 && y_76455;
                                bool ok_or_empty_76457 = empty_slice_76447 || forwards_ok_76456;
                                bool index_certs_76458;
                                
                                if (!ok_or_empty_76457) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) merge_sequential_arg2_76444, ":", (long long) merge_sequential_arg2_76445, "] out of bounds for array of shape [", (long long) next_stride_76176, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:40-58\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  test_dbscan_plus.fut:14:23-19:30\n   #10 test_dbscan_plus.fut:13:1-20:55\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t j_m_i_76459 = sub64(defunc_0_f_res_76442, defunc_0_f_res_76435);
                                bool empty_slice_76460 = j_m_i_76459 == (int64_t) 0;
                                int64_t m_76461 = sub64(j_m_i_76459, (int64_t) 1);
                                int64_t i_p_m_t_s_76462 = add64(defunc_0_f_res_76435, m_76461);
                                bool zzero_leq_i_p_m_t_s_76463 = sle64((int64_t) 0, i_p_m_t_s_76462);
                                bool i_p_m_t_s_leq_w_76464 = slt64(i_p_m_t_s_76462, next_stride_76176);
                                bool zzero_lte_i_76465 = sle64((int64_t) 0, defunc_0_f_res_76435);
                                bool i_lte_j_76466 = sle64(defunc_0_f_res_76435, defunc_0_f_res_76442);
                                bool y_76467 = i_p_m_t_s_leq_w_76464 && zzero_lte_i_76465;
                                bool y_76468 = zzero_leq_i_p_m_t_s_76463 && y_76467;
                                bool forwards_ok_76469 = i_lte_j_76466 && y_76468;
                                bool ok_or_empty_76470 = empty_slice_76460 || forwards_ok_76469;
                                bool index_certs_76471;
                                
                                if (!ok_or_empty_76470) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) defunc_0_f_res_76435, ":", (long long) defunc_0_f_res_76442, "] out of bounds for array of shape [", (long long) next_stride_76176, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:29-39\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  test_dbscan_plus.fut:14:23-19:30\n   #10 test_dbscan_plus.fut:13:1-20:55\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                bool cond_76472 = slt64((int64_t) 0, j_m_i_76459);
                                int64_t dummy_76473;
                                int64_t dummy_76474;
                                
                                if (cond_76472) {
                                    bool index_certs_81275;
                                    
                                    if (!cond_76472) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) j_m_i_76459, "].", "-> #0  /prelude/array.fut:22:29-33\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:100:36-42\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #6  /prelude/soacs.fut:59:9-11\n   #7  /prelude/array.fut:241:11-17\n   #8  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #9  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 test_dbscan_plus.fut:14:23-19:30\n   #12 test_dbscan_plus.fut:13:1-20:55\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    int64_t slice_81276 = start_76248 + defunc_0_f_res_76435;
                                    int64_t head_res_81277 = ((int64_t *) mem_param_83887.mem)[slice_81276];
                                    int64_t head_res_81278 = ((int64_t *) mem_param_83890.mem)[slice_81276];
                                    
                                    dummy_76473 = head_res_81277;
                                    dummy_76474 = head_res_81278;
                                } else {
                                    bool y_76479 = slt64((int64_t) 0, j_m_i_76446);
                                    bool index_certs_76480;
                                    
                                    if (!y_76479) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) j_m_i_76446, "].", "-> #0  /prelude/array.fut:22:29-33\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:100:48-54\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #6  /prelude/soacs.fut:59:9-11\n   #7  /prelude/array.fut:241:11-17\n   #8  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #9  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 test_dbscan_plus.fut:14:23-19:30\n   #12 test_dbscan_plus.fut:13:1-20:55\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    int64_t slice_76481 = start_76248 + merge_sequential_arg2_76444;
                                    int64_t head_res_76482 = ((int64_t *) mem_param_83887.mem)[slice_76481];
                                    int64_t head_res_76483 = ((int64_t *) mem_param_83890.mem)[slice_76481];
                                    
                                    dummy_76473 = head_res_76482;
                                    dummy_76474 = head_res_76483;
                                }
                                for (int64_t nest_i_84555 = 0; nest_i_84555 < merge_block_sizze_76015; nest_i_84555++) {
                                    ((int64_t *) mem_83923)[i_82132 * merge_block_sizze_76015 + nest_i_84555] = dummy_76473;
                                }
                                for (int64_t nest_i_84556 = 0; nest_i_84556 < merge_block_sizze_76015; nest_i_84556++) {
                                    ((int64_t *) mem_83926)[i_82132 * merge_block_sizze_76015 + nest_i_84556] = dummy_76474;
                                }
                                
                                int64_t data_76486;
                                int64_t i_76490 = (int64_t) 0;
                                
                                for (int64_t k_76489 = 0; k_76489 < merge_block_sizze_76015; k_76489++) {
                                    int64_t j_76493 = sub64(k_76489, i_76490);
                                    bool cond_76494 = j_76493 == j_m_i_76446;
                                    bool cond_76495;
                                    
                                    if (cond_76494) {
                                        cond_76495 = 1;
                                    } else {
                                        bool cond_76496 = slt64(i_76490, j_m_i_76459);
                                        bool cond_f_res_76497;
                                        
                                        if (cond_76496) {
                                            bool x_81279 = sle64((int64_t) 0, j_76493);
                                            bool y_81280 = slt64(j_76493, j_m_i_76446);
                                            bool bounds_check_81281 = x_81279 && y_81280;
                                            bool index_certs_81282;
                                            
                                            if (!bounds_check_81281) {
                                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_76493, "] out of bounds for array of shape [", (long long) j_m_i_76446, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:55-59\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #8  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #9  /prelude/functional.fut:9:44-45\n   #10 test_dbscan_plus.fut:14:23-19:30\n   #11 test_dbscan_plus.fut:13:1-20:55\n"));
                                                err = FUTHARK_PROGRAM_ERROR;
                                                goto cleanup;
                                            }
                                            
                                            bool x_81287 = sle64((int64_t) 0, i_76490);
                                            bool bounds_check_81288 = cond_76496 && x_81287;
                                            bool index_certs_81289;
                                            
                                            if (!bounds_check_81288) {
                                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_76490, "] out of bounds for array of shape [", (long long) j_m_i_76459, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:105:50-54\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #8  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #9  /prelude/functional.fut:9:44-45\n   #10 test_dbscan_plus.fut:14:23-19:30\n   #11 test_dbscan_plus.fut:13:1-20:55\n"));
                                                err = FUTHARK_PROGRAM_ERROR;
                                                goto cleanup;
                                            }
                                            
                                            int64_t slice_81283 = merge_sequential_arg2_76444 + j_76493;
                                            int64_t slice_81284 = start_76248 + slice_81283;
                                            int64_t leq_arg1_81285 = ((int64_t *) mem_param_83887.mem)[slice_81284];
                                            int64_t leq_arg1_81286 = ((int64_t *) mem_param_83890.mem)[slice_81284];
                                            int64_t slice_81290 = defunc_0_f_res_76435 + i_76490;
                                            int64_t slice_81291 = start_76248 + slice_81290;
                                            int64_t leq_arg0_81292 = ((int64_t *) mem_param_83887.mem)[slice_81291];
                                            int64_t leq_arg0_81293 = ((int64_t *) mem_param_83890.mem)[slice_81291];
                                            bool defunc_0_leq_res_81294;
                                            
                                            if (futrts_lifted_lambda_12077(ctx, &defunc_0_leq_res_81294, leq_arg0_81292, leq_arg0_81293, leq_arg1_81285, leq_arg1_81286) != 0) {
                                                err = 1;
                                                goto cleanup;
                                            }
                                            cond_f_res_76497 = defunc_0_leq_res_81294;
                                        } else {
                                            cond_f_res_76497 = 0;
                                        }
                                        cond_76495 = cond_f_res_76497;
                                    }
                                    
                                    int64_t loopres_76514;
                                    int64_t loopres_76515;
                                    int64_t loopres_76516;
                                    
                                    if (cond_76495) {
                                        bool x_81296 = sle64((int64_t) 0, i_76490);
                                        bool y_81297 = slt64(i_76490, j_m_i_76459);
                                        bool bounds_check_81298 = x_81296 && y_81297;
                                        bool index_certs_81299;
                                        
                                        if (!bounds_check_81298) {
                                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_76490, "] out of bounds for array of shape [", (long long) j_m_i_76459, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:106:17-21\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #8  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #9  /prelude/functional.fut:9:44-45\n   #10 test_dbscan_plus.fut:14:23-19:30\n   #11 test_dbscan_plus.fut:13:1-20:55\n"));
                                            err = FUTHARK_PROGRAM_ERROR;
                                            goto cleanup;
                                        }
                                        
                                        int64_t tmp_81295 = add64((int64_t) 1, i_76490);
                                        int64_t slice_81300 = defunc_0_f_res_76435 + i_76490;
                                        int64_t slice_81301 = start_76248 + slice_81300;
                                        int64_t tmp_81302 = ((int64_t *) mem_param_83887.mem)[slice_81301];
                                        int64_t tmp_81303 = ((int64_t *) mem_param_83890.mem)[slice_81301];
                                        
                                        loopres_76514 = tmp_81295;
                                        loopres_76515 = tmp_81302;
                                        loopres_76516 = tmp_81303;
                                    } else {
                                        bool x_76526 = sle64((int64_t) 0, j_76493);
                                        bool y_76527 = slt64(j_76493, j_m_i_76446);
                                        bool bounds_check_76528 = x_76526 && y_76527;
                                        bool index_certs_76529;
                                        
                                        if (!bounds_check_76528) {
                                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_76493, "] out of bounds for array of shape [", (long long) j_m_i_76446, "].", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:108:15-19\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:256:39-260:59\n   #8  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #9  /prelude/functional.fut:9:44-45\n   #10 test_dbscan_plus.fut:14:23-19:30\n   #11 test_dbscan_plus.fut:13:1-20:55\n"));
                                            err = FUTHARK_PROGRAM_ERROR;
                                            goto cleanup;
                                        }
                                        
                                        int64_t slice_76530 = merge_sequential_arg2_76444 + j_76493;
                                        int64_t slice_76531 = start_76248 + slice_76530;
                                        int64_t tmp_76532 = ((int64_t *) mem_param_83887.mem)[slice_76531];
                                        int64_t tmp_76533 = ((int64_t *) mem_param_83890.mem)[slice_76531];
                                        
                                        loopres_76514 = i_76490;
                                        loopres_76515 = tmp_76532;
                                        loopres_76516 = tmp_76533;
                                    }
                                    ((int64_t *) mem_83923)[i_82132 * merge_block_sizze_76015 + k_76489] = loopres_76515;
                                    ((int64_t *) mem_83926)[i_82132 * merge_block_sizze_76015 + k_76489] = loopres_76516;
                                    
                                    int64_t i_tmp_84557 = loopres_76514;
                                    
                                    i_76490 = i_tmp_84557;
                                }
                                data_76486 = i_76490;
                            }
                            if (memblock_alloc(ctx, &mem_83961, bytes_83922, "mem_83961")) {
                                err = 1;
                                goto cleanup;
                            }
                            lmad_copy_8b(ctx, 2, (uint64_t *) mem_83961.mem, (int64_t) 0, (int64_t []) {merge_block_sizze_76015, (int64_t) 1}, (uint64_t *) mem_83923, (int64_t) 0, (int64_t []) {merge_block_sizze_76015, (int64_t) 1}, (int64_t []) {num_blocks_76196, merge_block_sizze_76015});
                            if (memblock_alloc(ctx, &mem_83965, bytes_83922, "mem_83965")) {
                                err = 1;
                                goto cleanup;
                            }
                            lmad_copy_8b(ctx, 2, (uint64_t *) mem_83965.mem, (int64_t) 0, (int64_t []) {merge_block_sizze_76015, (int64_t) 1}, (uint64_t *) mem_83926, (int64_t) 0, (int64_t []) {merge_block_sizze_76015, (int64_t) 1}, (int64_t []) {num_blocks_76196, merge_block_sizze_76015});
                            if (memblock_set(ctx, &ext_mem_83988, &mem_83961, "mem_83961") != 0)
                                return 1;
                            if (memblock_set(ctx, &ext_mem_83985, &mem_83965, "mem_83965") != 0)
                                return 1;
                        }
                        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83893, i_82139 * next_stride_76176, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_83988.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {next_stride_76176});
                        if (memblock_unref(ctx, &ext_mem_83988, "ext_mem_83988") != 0)
                            return 1;
                        lmad_copy_8b(ctx, 1, (uint64_t *) mem_83896, i_82139 * next_stride_76176, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_83985.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {next_stride_76176});
                        if (memblock_unref(ctx, &ext_mem_83985, "ext_mem_83985") != 0)
                            return 1;
                    }
                    
                    int64_t flat_dim_76540 = next_stride_76176 * num_merges_76180;
                    
                    if (memblock_alloc(ctx, &mem_83999, bytes_83892, "mem_83999")) {
                        err = 1;
                        goto cleanup;
                    }
                    lmad_copy_8b(ctx, 2, (uint64_t *) mem_83999.mem, (int64_t) 0, (int64_t []) {next_stride_76176, (int64_t) 1}, (uint64_t *) mem_83893, (int64_t) 0, (int64_t []) {next_stride_76176, (int64_t) 1}, (int64_t []) {num_merges_76180, next_stride_76176});
                    if (memblock_alloc(ctx, &mem_84003, bytes_83892, "mem_84003")) {
                        err = 1;
                        goto cleanup;
                    }
                    lmad_copy_8b(ctx, 2, (uint64_t *) mem_84003.mem, (int64_t) 0, (int64_t []) {next_stride_76176, (int64_t) 1}, (uint64_t *) mem_83896, (int64_t) 0, (int64_t []) {next_stride_76176, (int64_t) 1}, (int64_t []) {num_merges_76180, next_stride_76176});
                    
                    bool loop_cond_76543 = slt64(next_stride_76176, flat_dim_76011);
                    
                    if (memblock_set(ctx, &mem_param_tmp_84531, &mem_83999, "mem_83999") != 0)
                        return 1;
                    if (memblock_set(ctx, &mem_param_tmp_84532, &mem_84003, "mem_84003") != 0)
                        return 1;
                    
                    int64_t loop_dz2084Uz2082U_tmp_84533 = flat_dim_76540;
                    bool loop_while_tmp_84534 = loop_cond_76543;
                    int64_t stride_tmp_84537 = next_stride_76176;
                    
                    if (memblock_set(ctx, &mem_param_83887, &mem_param_tmp_84531, "mem_param_tmp_84531") != 0)
                        return 1;
                    if (memblock_set(ctx, &mem_param_83890, &mem_param_tmp_84532, "mem_param_tmp_84532") != 0)
                        return 1;
                    loop_dz2084Uz2082U_76171 = loop_dz2084Uz2082U_tmp_84533;
                    loop_while_76172 = loop_while_tmp_84534;
                    stride_76175 = stride_tmp_84537;
                }
                if (memblock_set(ctx, &ext_mem_84010, &mem_param_83887, "mem_param_83887") != 0)
                    return 1;
                if (memblock_set(ctx, &ext_mem_84009, &mem_param_83890, "mem_param_83890") != 0)
                    return 1;
                data_76166 = loop_dz2084Uz2082U_76171;
                data_76167 = loop_while_76172;
                data_76170 = stride_76175;
                if (memblock_unref(ctx, &mem_83879, "mem_83879") != 0)
                    return 1;
                if (memblock_unref(ctx, &mem_83883, "mem_83883") != 0)
                    return 1;
                
                bool i_p_m_t_s_leq_w_76544 = slt64(zs_lhs_75994, data_76166);
                bool y_76545 = zzero_leq_i_p_m_t_s_76068 && i_p_m_t_s_leq_w_76544;
                bool ok_or_empty_76546 = empty_slice_76067 || y_76545;
                bool index_certs_76547;
                
                if (!ok_or_empty_76546) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) conc_tmp_75923, "] out of bounds for array of shape [", (long long) data_76166, "].", "-> #0  /prelude/array.fut:46:45-51\n   #1  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:265:3-14\n   #2  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #3  /prelude/functional.fut:9:44-45\n   #4  test_dbscan_plus.fut:14:23-19:30\n   #5  test_dbscan_plus.fut:13:1-20:55\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                if (memblock_set(ctx, &ext_mem_84016, &ext_mem_84010, "ext_mem_84010") != 0)
                    return 1;
                if (memblock_set(ctx, &ext_mem_84013, &ext_mem_84009, "ext_mem_84009") != 0)
                    return 1;
            }
            if (mem_84018_cached_sizze_84724 < bytes_83768) {
                err = lexical_realloc(ctx, &mem_84018, &mem_84018_cached_sizze_84724, bytes_83768);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            if (mem_84020_cached_sizze_84725 < bytes_83768) {
                err = lexical_realloc(ctx, &mem_84020, &mem_84020_cached_sizze_84725, bytes_83768);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            
            int64_t discard_82150;
            int64_t scanacc_82144 = (int64_t) 0;
            
            for (int64_t i_82147 = 0; i_82147 < conc_tmp_75923; i_82147++) {
                bool cond_79425 = i_82147 == (int64_t) 0;
                bool lifted_lambda_res_79426;
                
                if (cond_79425) {
                    lifted_lambda_res_79426 = 1;
                } else {
                    int64_t znze_rhs_79432 = sub64(i_82147, (int64_t) 1);
                    bool x_79433 = sle64((int64_t) 0, znze_rhs_79432);
                    bool y_79434 = slt64(znze_rhs_79432, conc_tmp_75923);
                    bool bounds_check_79435 = x_79433 && y_79434;
                    bool index_certs_79436;
                    
                    if (!bounds_check_79435) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) znze_rhs_79432, "] out of bounds for array of shape [", (long long) conc_tmp_75923, "].", "-> #0  ../../ftDBSCAN_plus.fut:961:89-103\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../ftDBSCAN_plus.fut:961:36-107\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t lifted_lambda_res_f_res_79431 = ((int64_t *) ext_mem_84016.mem)[i_82147];
                    int64_t lifted_lambda_res_f_res_79437 = ((int64_t *) ext_mem_84016.mem)[znze_rhs_79432];
                    bool lifted_lambda_res_f_res_79438 = lifted_lambda_res_f_res_79431 == lifted_lambda_res_f_res_79437;
                    bool lifted_lambda_res_f_res_79439 = !lifted_lambda_res_f_res_79438;
                    
                    lifted_lambda_res_79426 = lifted_lambda_res_f_res_79439;
                }
                
                int64_t defunc_0_f_res_79441 = btoi_bool_i64(lifted_lambda_res_79426);
                int64_t defunc_0_op_res_76575 = add64(defunc_0_f_res_79441, scanacc_82144);
                
                ((int64_t *) mem_84018)[i_82147] = defunc_0_op_res_76575;
                ((int64_t *) mem_84020)[i_82147] = defunc_0_f_res_79441;
                
                int64_t scanacc_tmp_84560 = defunc_0_op_res_76575;
                
                scanacc_82144 = scanacc_tmp_84560;
            }
            discard_82150 = scanacc_82144;
            
            int64_t m_f_res_76576;
            
            if (x_76069) {
                int64_t x_81312 = ((int64_t *) mem_84018)[zs_lhs_75994];
                
                m_f_res_76576 = x_81312;
            } else {
                m_f_res_76576 = (int64_t) 0;
            }
            
            int64_t m_76578;
            
            if (empty_slice_76067) {
                m_76578 = (int64_t) 0;
            } else {
                m_76578 = m_f_res_76576;
            }
            
            int64_t m_76588 = sub64(m_76578, (int64_t) 1);
            bool i_p_m_t_s_leq_w_76590 = slt64(m_76588, conc_tmp_75923);
            bool zzero_leq_i_p_m_t_s_76589 = sle64((int64_t) 0, m_76588);
            bool y_76592 = zzero_leq_i_p_m_t_s_76589 && i_p_m_t_s_leq_w_76590;
            bool i_lte_j_76591 = sle64((int64_t) 0, m_76578);
            bool forwards_ok_76593 = i_lte_j_76591 && y_76592;
            bool eq_x_zz_76585 = (int64_t) 0 == m_f_res_76576;
            bool p_and_eq_x_y_76586 = x_76069 && eq_x_zz_76585;
            bool empty_slice_76587 = empty_slice_76067 || p_and_eq_x_y_76586;
            bool ok_or_empty_76594 = empty_slice_76587 || forwards_ok_76593;
            bool index_certs_76595;
            
            if (!ok_or_empty_76594) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_76578, "] out of bounds for array of shape [", (long long) conc_tmp_75923, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  test_dbscan_plus.fut:14:23-19:30\n   #3  test_dbscan_plus.fut:13:1-20:55\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t conc_tmp_76606 = loop_dz2084Uz2089U_73700 + m_76578;
            int64_t bytes_84049 = (int64_t) 8 * conc_tmp_76606;
            int64_t conc_tmp_76608 = loop_dz2085Uz2080U_73701 + m_76578;
            int64_t bytes_84051 = (int64_t) 8 * conc_tmp_76608;
            
            if (memblock_alloc(ctx, &mem_84052, bytes_84051, "mem_84052")) {
                err = 1;
                goto cleanup;
            }
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_84052.mem, loop_dz2085Uz2080U_73701, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_84013.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_76578});
            if (memblock_alloc(ctx, &mem_84050, bytes_84049, "mem_84050")) {
                err = 1;
                goto cleanup;
            }
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_84050.mem, loop_dz2084Uz2089U_73700, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_84016.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_76578});
            for (int64_t write_iter_82151 = 0; write_iter_82151 < conc_tmp_75923; write_iter_82151++) {
                int64_t write_iv_82154 = ((int64_t *) mem_84020)[write_iter_82151];
                int64_t write_iv_82155 = ((int64_t *) mem_84018)[write_iter_82151];
                int64_t write_iv_82156 = ((int64_t *) ext_mem_84016.mem)[write_iter_82151];
                int64_t write_iv_82157 = ((int64_t *) ext_mem_84013.mem)[write_iter_82151];
                bool cond_79415 = write_iv_82154 == (int64_t) 1;
                int64_t lifted_lambda_res_79416;
                
                if (cond_79415) {
                    int64_t lifted_lambda_res_t_res_81313 = sub64(write_iv_82155, (int64_t) 1);
                    
                    lifted_lambda_res_79416 = lifted_lambda_res_t_res_81313;
                } else {
                    lifted_lambda_res_79416 = (int64_t) -1;
                }
                if (sle64((int64_t) 0, lifted_lambda_res_79416) && slt64(lifted_lambda_res_79416, m_76578)) {
                    ((int64_t *) mem_84050.mem)[loop_dz2084Uz2089U_73700 + lifted_lambda_res_79416] = write_iv_82156;
                }
                if (sle64((int64_t) 0, lifted_lambda_res_79416) && slt64(lifted_lambda_res_79416, m_76578)) {
                    ((int64_t *) mem_84052.mem)[loop_dz2085Uz2080U_73701 + lifted_lambda_res_79416] = write_iv_82157;
                }
            }
            if (memblock_unref(ctx, &ext_mem_84013, "ext_mem_84013") != 0)
                return 1;
            if (memblock_unref(ctx, &ext_mem_84016, "ext_mem_84016") != 0)
                return 1;
            
            int64_t tmp_76605 = add64(collisions_73713, m_76578);
            int64_t tmp_offs_84565 = (int64_t) 0;
            
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_84050.mem, tmp_offs_84565, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_82655.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {loop_dz2084Uz2089U_73700});
            tmp_offs_84565 += loop_dz2084Uz2089U_73700;
            if (!(tmp_offs_84565 == loop_dz2084Uz2089U_73700)) {
                lmad_copy_8b(ctx, 1, (uint64_t *) mem_84050.mem, tmp_offs_84565, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_84050.mem, loop_dz2084Uz2089U_73700, (int64_t []) {(int64_t) 1}, (int64_t []) {m_76578});
            }
            tmp_offs_84565 += m_76578;
            
            int64_t tmp_offs_84566 = (int64_t) 0;
            
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_84052.mem, tmp_offs_84566, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_82658.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {loop_dz2085Uz2080U_73701});
            tmp_offs_84566 += loop_dz2085Uz2080U_73701;
            if (!(tmp_offs_84566 == loop_dz2085Uz2080U_73701)) {
                lmad_copy_8b(ctx, 1, (uint64_t *) mem_84052.mem, tmp_offs_84566, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_84052.mem, loop_dz2085Uz2080U_73701, (int64_t []) {(int64_t) 1}, (int64_t []) {m_76578});
            }
            tmp_offs_84566 += m_76578;
            if (memblock_set(ctx, &ext_mem_84058, &mem_84050, "mem_84050") != 0)
                return 1;
            if (memblock_set(ctx, &ext_mem_84055, &mem_84052, "mem_84052") != 0)
                return 1;
            loopres_76075 = conc_tmp_76606;
            loopres_76076 = conc_tmp_76608;
            loopres_76078 = tmp_76605;
        }
        if (memblock_alloc(ctx, &mem_84060, bytes_84059, "mem_84060")) {
            err = 1;
            goto cleanup;
        }
        
        int64_t tmp_offs_84567 = (int64_t) 0;
        
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_84060.mem, tmp_offs_84567, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_82646.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {loop_dz2084Uz2087U_73698});
        tmp_offs_84567 += loop_dz2084Uz2087U_73698;
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_84060.mem, tmp_offs_84567, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_83808.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {loopres_75982});
        tmp_offs_84567 += loopres_75982;
        if (memblock_unref(ctx, &ext_mem_83808, "ext_mem_83808") != 0)
            return 1;
        if (memblock_alloc(ctx, &mem_84062, bytes_84059, "mem_84062")) {
            err = 1;
            goto cleanup;
        }
        
        int64_t tmp_offs_84568 = (int64_t) 0;
        
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_84062.mem, tmp_offs_84568, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_82649.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {loop_dz2084Uz2087U_73698});
        tmp_offs_84568 += loop_dz2084Uz2087U_73698;
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_84062.mem, tmp_offs_84568, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_83807.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {loopres_75982});
        tmp_offs_84568 += loopres_75982;
        if (memblock_unref(ctx, &ext_mem_83807, "ext_mem_83807") != 0)
            return 1;
        if (memblock_alloc(ctx, &mem_84064, bytes_84063, "mem_84064")) {
            err = 1;
            goto cleanup;
        }
        
        int64_t tmp_offs_84569 = (int64_t) 0;
        
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_84064.mem, tmp_offs_84569, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_82652.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {loop_dz2084Uz2088U_73699});
        tmp_offs_84569 += loop_dz2084Uz2088U_73699;
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_84064.mem, tmp_offs_84569, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_83806.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {loopres_75982});
        tmp_offs_84569 += loopres_75982;
        if (memblock_unref(ctx, &ext_mem_83806, "ext_mem_83806") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84253, &mem_84060, "mem_84060") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84254, &mem_84062, "mem_84062") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84255, &mem_84064, "mem_84064") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84256, &ext_mem_84058, "ext_mem_84058") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84257, &ext_mem_84055, "ext_mem_84055") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84258, &mem_83769, "mem_83769") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84259, &mem_83771, "mem_83771") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84260, &mem_82873, "mem_82873") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84261, &mem_82875, "mem_82875") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84262, &mem_82925, "mem_82925") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84263, &mem_82927, "mem_82927") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84264, &mem_82699, "mem_82699") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84265, &mem_82752, "mem_82752") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84266, &mem_83741, "mem_83741") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84267, &mem_82701, "mem_82701") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84268, &mem_82703, "mem_82703") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84269, &mem_83253, "mem_83253") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_84270, &mem_82763, "mem_82763") != 0)
            return 1;
        
        int64_t loop_dz2084Uz2087U_tmp_84271 = conc_tmp_76620;
        int64_t loop_dz2084Uz2088U_tmp_84272 = conc_tmp_76623;
        int64_t loop_dz2084Uz2089U_tmp_84273 = loopres_76075;
        int64_t loop_dz2085Uz2080U_tmp_84274 = loopres_76076;
        int64_t loop_dz2085Uz2081U_tmp_84275 = conc_tmp_75923;
        int64_t loop_dz2085Uz2086U_tmp_84276 = partition_sizze_74386;
        int64_t loop_dz2085Uz2087U_tmp_84277 = partition_sizze_74386;
        int64_t loop_dz2085Uz2088U_tmp_84278 = partition_sizze_74386;
        int64_t loop_dz2086Uz2080U_tmp_84279 = m_73974;
        int64_t loop_dz2086Uz2086U_tmp_84280 = dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_73747;
        bool loop_while_tmp_84281 = cond_75979;
        int64_t collisions_tmp_84286 = loopres_76078;
        int64_t old_clHandler_tmp_84294 = max_res_75922;
        int64_t old_clHandler_tmp_84295 = new_proc_74486;
        int64_t next_part_tmp_84303 = next_partition_to_read_res_75978;
        
        if (memblock_set(ctx, &mem_param_82646, &mem_param_tmp_84253, "mem_param_tmp_84253") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82649, &mem_param_tmp_84254, "mem_param_tmp_84254") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82652, &mem_param_tmp_84255, "mem_param_tmp_84255") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82655, &mem_param_tmp_84256, "mem_param_tmp_84256") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82658, &mem_param_tmp_84257, "mem_param_tmp_84257") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82661, &mem_param_tmp_84258, "mem_param_tmp_84258") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82664, &mem_param_tmp_84259, "mem_param_tmp_84259") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82667, &mem_param_tmp_84260, "mem_param_tmp_84260") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82670, &mem_param_tmp_84261, "mem_param_tmp_84261") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82673, &mem_param_tmp_84262, "mem_param_tmp_84262") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82676, &mem_param_tmp_84263, "mem_param_tmp_84263") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82679, &mem_param_tmp_84264, "mem_param_tmp_84264") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82682, &mem_param_tmp_84265, "mem_param_tmp_84265") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82685, &mem_param_tmp_84266, "mem_param_tmp_84266") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82688, &mem_param_tmp_84267, "mem_param_tmp_84267") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82691, &mem_param_tmp_84268, "mem_param_tmp_84268") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82694, &mem_param_tmp_84269, "mem_param_tmp_84269") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_82697, &mem_param_tmp_84270, "mem_param_tmp_84270") != 0)
            return 1;
        loop_dz2084Uz2087U_73698 = loop_dz2084Uz2087U_tmp_84271;
        loop_dz2084Uz2088U_73699 = loop_dz2084Uz2088U_tmp_84272;
        loop_dz2084Uz2089U_73700 = loop_dz2084Uz2089U_tmp_84273;
        loop_dz2085Uz2080U_73701 = loop_dz2085Uz2080U_tmp_84274;
        loop_dz2085Uz2081U_73702 = loop_dz2085Uz2081U_tmp_84275;
        loop_dz2085Uz2086U_73703 = loop_dz2085Uz2086U_tmp_84276;
        loop_dz2085Uz2087U_73704 = loop_dz2085Uz2087U_tmp_84277;
        loop_dz2085Uz2088U_73705 = loop_dz2085Uz2088U_tmp_84278;
        loop_dz2086Uz2080U_73706 = loop_dz2086Uz2080U_tmp_84279;
        loop_dz2086Uz2086U_73707 = loop_dz2086Uz2086U_tmp_84280;
        loop_while_73708 = loop_while_tmp_84281;
        collisions_73713 = collisions_tmp_84286;
        old_clHandler_73721 = old_clHandler_tmp_84294;
        old_clHandler_73722 = old_clHandler_tmp_84295;
        next_part_73730 = next_part_tmp_84303;
    }
    if (memblock_set(ctx, &ext_mem_84118, &mem_param_82646, "mem_param_82646") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84117, &mem_param_82649, "mem_param_82649") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84116, &mem_param_82652, "mem_param_82652") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84115, &mem_param_82655, "mem_param_82655") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84114, &mem_param_82658, "mem_param_82658") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84113, &mem_param_82661, "mem_param_82661") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84112, &mem_param_82664, "mem_param_82664") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84111, &mem_param_82667, "mem_param_82667") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84110, &mem_param_82670, "mem_param_82670") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84109, &mem_param_82673, "mem_param_82673") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84108, &mem_param_82676, "mem_param_82676") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84107, &mem_param_82679, "mem_param_82679") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84106, &mem_param_82682, "mem_param_82682") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84105, &mem_param_82685, "mem_param_82685") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84104, &mem_param_82688, "mem_param_82688") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84103, &mem_param_82691, "mem_param_82691") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84102, &mem_param_82694, "mem_param_82694") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_84101, &mem_param_82697, "mem_param_82697") != 0)
        return 1;
    internal_DBSCAN_double_res_73665 = loop_dz2084Uz2087U_73698;
    internal_DBSCAN_double_res_73666 = loop_dz2084Uz2088U_73699;
    internal_DBSCAN_double_res_73667 = loop_dz2084Uz2089U_73700;
    internal_DBSCAN_double_res_73668 = loop_dz2085Uz2080U_73701;
    internal_DBSCAN_double_res_73669 = loop_dz2085Uz2081U_73702;
    internal_DBSCAN_double_res_73670 = loop_dz2085Uz2086U_73703;
    internal_DBSCAN_double_res_73671 = loop_dz2085Uz2087U_73704;
    internal_DBSCAN_double_res_73672 = loop_dz2085Uz2088U_73705;
    internal_DBSCAN_double_res_73673 = loop_dz2086Uz2080U_73706;
    internal_DBSCAN_double_res_73674 = loop_dz2086Uz2086U_73707;
    internal_DBSCAN_double_res_73675 = loop_while_73708;
    internal_DBSCAN_double_res_73680 = collisions_73713;
    internal_DBSCAN_double_res_73688 = old_clHandler_73721;
    internal_DBSCAN_double_res_73689 = old_clHandler_73722;
    internal_DBSCAN_double_res_73697 = next_part_73730;
    if (memblock_unref(ctx, &mem_82639, "mem_82639") != 0)
        return 1;
    if (memblock_unref(ctx, &mem_82640, "mem_82640") != 0)
        return 1;
    if (memblock_unref(ctx, &mem_82641, "mem_82641") != 0)
        return 1;
    if (memblock_unref(ctx, &mem_82642, "mem_82642") != 0)
        return 1;
    if (memblock_unref(ctx, &mem_82643, "mem_82643") != 0)
        return 1;
    
    int64_t bytes_84146 = (int64_t) 8 * internal_DBSCAN_double_res_73665;
    bool cond_76625 = internal_DBSCAN_double_res_73680 == (int64_t) 0;
    int64_t bytes_84119 = (int64_t) 8 * internal_DBSCAN_double_res_73666;
    
    if (cond_76625) {
        if (memblock_set(ctx, &ext_mem_84145, &ext_mem_84116, "ext_mem_84116") != 0)
            return 1;
    } else {
        double i64_res_76627 = sitofp_i64_f64(internal_DBSCAN_double_res_73680);
        double log2_res_76628 = futrts_log2_64(i64_res_76627);
        double ceil_res_76629 = futrts_ceil64(log2_res_76628);
        int64_t f64_res_76630 = fptosi_f64_i64(ceil_res_76629);
        int64_t num_iter_76631 = add64((int64_t) 1, f64_res_76630);
        
        if (memblock_alloc(ctx, &mem_84120, bytes_84119, "mem_84120")) {
            err = 1;
            goto cleanup;
        }
        for (int64_t nest_i_84570 = 0; nest_i_84570 < internal_DBSCAN_double_res_73666; nest_i_84570++) {
            ((int64_t *) mem_84120.mem)[nest_i_84570] = (int64_t) 0;
        }
        
        int64_t bsearch_76634;
        int64_t last_step_76637;
        
        if (memblock_set(ctx, &mem_param_84123, &mem_84120, "mem_84120") != 0)
            return 1;
        last_step_76637 = internal_DBSCAN_double_res_73680;
        for (int64_t i_76635 = 0; i_76635 < num_iter_76631; i_76635++) {
            int64_t zs_lhs_76638 = add64((int64_t) 1, last_step_76637);
            int64_t this_step_76639 = sdiv64(zs_lhs_76638, (int64_t) 2);
            
            if (memblock_alloc(ctx, &mem_84125, bytes_84119, "mem_84125")) {
                err = 1;
                goto cleanup;
            }
            for (int64_t i_82162 = 0; i_82162 < internal_DBSCAN_double_res_73666; i_82162++) {
                int64_t eta_p_80480 = ((int64_t *) mem_param_84123.mem)[i_82162];
                int64_t eta_p_80481 = ((int64_t *) ext_mem_84116.mem)[i_82162];
                bool cond_80483 = sle64(eta_p_80480, (int64_t) 0);
                int64_t prev_elem_80484;
                
                if (cond_80483) {
                    bool y_81433 = slt64((int64_t) 0, internal_DBSCAN_double_res_73667);
                    bool index_certs_81434;
                    
                    if (!y_81433) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) internal_DBSCAN_double_res_73667, "].", "-> #0  ../../ftDBSCAN_plus.fut:983:66-72\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../ftDBSCAN_plus.fut:982:38-986:36\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t prev_elem_t_res_81435 = ((int64_t *) ext_mem_84115.mem)[(int64_t) 0];
                    
                    prev_elem_80484 = prev_elem_t_res_81435;
                } else {
                    int64_t tmp_80488 = sub64(eta_p_80480, (int64_t) 1);
                    bool x_80489 = sle64((int64_t) 0, tmp_80488);
                    bool y_80490 = slt64(tmp_80488, internal_DBSCAN_double_res_73667);
                    bool bounds_check_80491 = x_80489 && y_80490;
                    bool index_certs_80492;
                    
                    if (!bounds_check_80491) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_80488, "] out of bounds for array of shape [", (long long) internal_DBSCAN_double_res_73667, "].", "-> #0  ../../ftDBSCAN_plus.fut:983:78-86\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../ftDBSCAN_plus.fut:982:38-986:36\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t prev_elem_f_res_80493 = ((int64_t *) ext_mem_84115.mem)[tmp_80488];
                    
                    prev_elem_80484 = prev_elem_f_res_80493;
                }
                
                bool cond_80494 = slt64(eta_p_80480, (int64_t) 0);
                int64_t cur_elem_80495;
                
                if (cond_80494) {
                    bool y_81436 = slt64((int64_t) 0, internal_DBSCAN_double_res_73667);
                    bool index_certs_81437;
                    
                    if (!y_81436) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) internal_DBSCAN_double_res_73667, "].", "-> #0  ../../ftDBSCAN_plus.fut:984:64-70\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../ftDBSCAN_plus.fut:982:38-986:36\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t cur_elem_t_res_81438 = ((int64_t *) ext_mem_84115.mem)[(int64_t) 0];
                    
                    cur_elem_80495 = cur_elem_t_res_81438;
                } else {
                    bool x_80499 = sle64((int64_t) 0, eta_p_80480);
                    bool y_80500 = slt64(eta_p_80480, internal_DBSCAN_double_res_73667);
                    bool bounds_check_80501 = x_80499 && y_80500;
                    bool index_certs_80502;
                    
                    if (!bounds_check_80501) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_80480, "] out of bounds for array of shape [", (long long) internal_DBSCAN_double_res_73667, "].", "-> #0  ../../ftDBSCAN_plus.fut:984:76-82\n   #1  /prelude/functional.fut:9:44-45\n   #2  ../../ftDBSCAN_plus.fut:982:38-986:36\n   #3  test_dbscan_plus.fut:14:23-19:30\n   #4  test_dbscan_plus.fut:13:1-20:55\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t cur_elem_f_res_80503 = ((int64_t *) ext_mem_84115.mem)[eta_p_80480];
                    
                    cur_elem_80495 = cur_elem_f_res_80503;
                }
                
                int64_t lifted_lambda_res_80507;
                
                if (cond_80494) {
                    lifted_lambda_res_80507 = (int64_t) -1;
                } else {
                    bool cond_80508 = eta_p_80481 == cur_elem_80495;
                    bool cond_80509 = eta_p_80480 == (int64_t) 0;
                    bool cond_t_res_f_res_80510 = slt64(prev_elem_80484, eta_p_80481);
                    bool x_80511 = !cond_80509;
                    bool y_80512 = cond_t_res_f_res_80510 && x_80511;
                    bool cond_t_res_80513 = cond_80509 || y_80512;
                    bool x_80514 = cond_80508 && cond_t_res_80513;
                    int64_t lifted_lambda_res_f_res_80515;
                    
                    if (x_80514) {
                        lifted_lambda_res_f_res_80515 = eta_p_80480;
                    } else {
                        int64_t lifted_lambda_res_f_res_f_res_80516;
                        
                        if (cond_80508) {
                            int64_t max_arg1_81439 = sub64(eta_p_80480, this_step_76639);
                            int64_t max_res_81440 = smax64((int64_t) 0, max_arg1_81439);
                            
                            lifted_lambda_res_f_res_f_res_80516 = max_res_81440;
                        } else {
                            bool cond_80519 = slt64(cur_elem_80495, eta_p_80481);
                            int64_t lifted_lambda_res_f_res_f_res_f_res_80520;
                            
                            if (cond_80519) {
                                int64_t zeze_rhs_81441 = sub64(internal_DBSCAN_double_res_73680, (int64_t) 1);
                                bool cond_81442 = eta_p_80480 == zeze_rhs_81441;
                                int64_t lifted_lambda_res_f_res_f_res_f_res_t_res_81443;
                                
                                if (cond_81442) {
                                    lifted_lambda_res_f_res_f_res_f_res_t_res_81443 = (int64_t) -1;
                                } else {
                                    int64_t min_arg1_81444 = add64(this_step_76639, eta_p_80480);
                                    int64_t min_res_81445 = smin64(zeze_rhs_81441, min_arg1_81444);
                                    
                                    lifted_lambda_res_f_res_f_res_f_res_t_res_81443 = min_res_81445;
                                }
                                lifted_lambda_res_f_res_f_res_f_res_80520 = lifted_lambda_res_f_res_f_res_f_res_t_res_81443;
                            } else {
                                int64_t lifted_lambda_res_f_res_f_res_f_res_f_res_80526;
                                
                                if (cond_t_res_80513) {
                                    lifted_lambda_res_f_res_f_res_f_res_f_res_80526 = (int64_t) -1;
                                } else {
                                    int64_t max_arg1_80527 = sub64(eta_p_80480, this_step_76639);
                                    int64_t max_res_80528 = smax64((int64_t) 0, max_arg1_80527);
                                    
                                    lifted_lambda_res_f_res_f_res_f_res_f_res_80526 = max_res_80528;
                                }
                                lifted_lambda_res_f_res_f_res_f_res_80520 = lifted_lambda_res_f_res_f_res_f_res_f_res_80526;
                            }
                            lifted_lambda_res_f_res_f_res_80516 = lifted_lambda_res_f_res_f_res_f_res_80520;
                        }
                        lifted_lambda_res_f_res_80515 = lifted_lambda_res_f_res_f_res_80516;
                    }
                    lifted_lambda_res_80507 = lifted_lambda_res_f_res_80515;
                }
                ((int64_t *) mem_84125.mem)[i_82162] = lifted_lambda_res_80507;
            }
            if (memblock_set(ctx, &mem_param_tmp_84571, &mem_84125, "mem_84125") != 0)
                return 1;
            
            int64_t last_step_tmp_84573 = this_step_76639;
            
            if (memblock_set(ctx, &mem_param_84123, &mem_param_tmp_84571, "mem_param_tmp_84571") != 0)
                return 1;
            last_step_76637 = last_step_tmp_84573;
        }
        if (memblock_set(ctx, &ext_mem_84134, &mem_param_84123, "mem_param_84123") != 0)
            return 1;
        bsearch_76634 = last_step_76637;
        if (memblock_unref(ctx, &mem_84120, "mem_84120") != 0)
            return 1;
        if (memblock_alloc(ctx, &mem_84136, bytes_84119, "mem_84136")) {
            err = 1;
            goto cleanup;
        }
        for (int64_t i_82166 = 0; i_82166 < internal_DBSCAN_double_res_73666; i_82166++) {
            int64_t eta_p_76740 = ((int64_t *) ext_mem_84116.mem)[i_82166];
            int64_t eta_p_76741 = ((int64_t *) ext_mem_84134.mem)[i_82166];
            bool cond_76742 = slt64(eta_p_76741, (int64_t) 0);
            bool cond_f_res_76743 = sle64(internal_DBSCAN_double_res_73668, eta_p_76741);
            bool x_76744 = !cond_76742;
            bool y_76745 = cond_f_res_76743 && x_76744;
            bool cond_76746 = cond_76742 || y_76745;
            int64_t lifted_lambda_res_76747;
            
            if (cond_76746) {
                lifted_lambda_res_76747 = eta_p_76740;
            } else {
                bool x_76748 = sle64((int64_t) 0, eta_p_76741);
                bool y_76749 = slt64(eta_p_76741, internal_DBSCAN_double_res_73668);
                bool bounds_check_76750 = x_76748 && y_76749;
                bool index_certs_76751;
                
                if (!bounds_check_76750) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_76741, "] out of bounds for array of shape [", (long long) internal_DBSCAN_double_res_73668, "].", "-> #0  ../../ftbasics.fut:27:66-71\n   #1  /prelude/soacs.fut:67:23-24\n   #2  /prelude/soacs.fut:67:27-37\n   #3  /prelude/functional.fut:9:44-45\n   #4  ../../ftbasics.fut:27:23-77\n   #5  test_dbscan_plus.fut:13:1-20:55\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                int64_t lifted_lambda_res_f_res_76752 = ((int64_t *) ext_mem_84114.mem)[eta_p_76741];
                
                lifted_lambda_res_76747 = lifted_lambda_res_f_res_76752;
            }
            ((int64_t *) mem_84136.mem)[i_82166] = lifted_lambda_res_76747;
        }
        if (memblock_unref(ctx, &ext_mem_84134, "ext_mem_84134") != 0)
            return 1;
        if (memblock_set(ctx, &ext_mem_84145, &mem_84136, "mem_84136") != 0)
            return 1;
    }
    if (memblock_unref(ctx, &ext_mem_84114, "ext_mem_84114") != 0)
        return 1;
    if (memblock_unref(ctx, &ext_mem_84115, "ext_mem_84115") != 0)
        return 1;
    if (memblock_unref(ctx, &ext_mem_84116, "ext_mem_84116") != 0)
        return 1;
    if (memblock_alloc(ctx, &mem_84147, bytes_84146, "mem_84147")) {
        err = 1;
        goto cleanup;
    }
    lmad_copy_8b(ctx, 1, (uint64_t *) mem_84147.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_84118.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {internal_DBSCAN_double_res_73665});
    if (memblock_unref(ctx, &ext_mem_84118, "ext_mem_84118") != 0)
        return 1;
    if (memblock_alloc(ctx, &mem_84149, bytes_84146, "mem_84149")) {
        err = 1;
        goto cleanup;
    }
    lmad_copy_8b(ctx, 1, (uint64_t *) mem_84149.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_84117.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {internal_DBSCAN_double_res_73665});
    if (memblock_unref(ctx, &ext_mem_84117, "ext_mem_84117") != 0)
        return 1;
    if (memblock_set(ctx, &mem_out_84193, &mem_84147, "mem_84147") != 0)
        return 1;
    if (memblock_set(ctx, &mem_out_84194, &mem_84149, "mem_84149") != 0)
        return 1;
    if (memblock_set(ctx, &mem_out_84195, &ext_mem_84145, "ext_mem_84145") != 0)
        return 1;
    prim_out_84196 = internal_DBSCAN_double_res_73665;
    prim_out_84197 = internal_DBSCAN_double_res_73666;
    if (memblock_set(ctx, &*mem_out_p_84576, &mem_out_84193, "mem_out_84193") != 0)
        return 1;
    if (memblock_set(ctx, &*mem_out_p_84577, &mem_out_84194, "mem_out_84194") != 0)
        return 1;
    if (memblock_set(ctx, &*mem_out_p_84578, &mem_out_84195, "mem_out_84195") != 0)
        return 1;
    *out_prim_out_84579 = prim_out_84196;
    *out_prim_out_84580 = prim_out_84197;
    
  cleanup:
    {
        free(mem_82402);
        free(mem_82413);
        free(mem_82416);
        free(mem_82426);
        free(mem_82428);
        free(mem_82495);
        free(mem_82498);
        free(mem_82508);
        free(mem_82510);
        free(mem_82525);
        free(mem_82528);
        free(mem_82617);
        free(mem_82619);
        free(mem_82632);
        free(mem_82698);
        free(mem_82704);
        free(mem_82705);
        free(mem_82706);
        free(mem_82726);
        free(mem_82734);
        free(mem_82736);
        free(mem_82738);
        free(mem_82760);
        free(mem_82762);
        free(mem_82783);
        free(mem_82785);
        free(mem_82787);
        free(mem_82807);
        free(mem_82809);
        free(mem_82810);
        free(mem_82812);
        free(mem_82838);
        free(mem_82840);
        free(mem_82842);
        free(mem_82862);
        free(mem_82870);
        free(mem_82872);
        free(mem_82901);
        free(mem_82903);
        free(mem_82905);
        free(mem_82929);
        free(mem_82931);
        free(mem_82933);
        free(mem_82953);
        free(mem_82955);
        free(mem_82957);
        free(mem_82959);
        free(mem_82960);
        free(mem_82962);
        free(mem_83000);
        free(mem_83002);
        free(mem_83003);
        free(mem_83005);
        free(mem_83007);
        free(mem_83039);
        free(mem_83041);
        free(mem_83043);
        free(mem_83063);
        free(mem_83065);
        free(mem_83066);
        free(mem_83068);
        free(mem_83070);
        free(mem_83141);
        free(mem_83143);
        free(mem_83144);
        free(mem_83164);
        free(mem_83166);
        free(mem_83168);
        free(mem_83188);
        free(mem_83190);
        free(mem_83204);
        free(mem_83206);
        free(mem_83208);
        free(mem_83229);
        free(mem_83250);
        free(mem_83252);
        free(mem_83273);
        free(mem_83275);
        free(mem_83277);
        free(mem_83297);
        free(mem_83299);
        free(mem_83301);
        free(mem_83310);
        free(mem_83312);
        free(mem_83340);
        free(mem_83343);
        free(mem_83346);
        free(mem_83348);
        free(mem_83364);
        free(mem_83394);
        free(mem_83401);
        free(mem_83412);
        free(mem_83420);
        free(mem_83421);
        free(mem_83435);
        free(mem_83454);
        free(mem_83462);
        free(mem_83463);
        free(mem_83477);
        free(mem_83485);
        free(mem_83493);
        free(mem_83501);
        free(mem_83509);
        free(mem_83511);
        free(mem_83525);
        free(mem_83527);
        free(mem_83541);
        free(mem_83543);
        free(mem_83557);
        free(mem_83559);
        free(mem_83573);
        free(mem_83575);
        free(mem_83589);
        free(mem_83591);
        free(mem_83593);
        free(mem_83613);
        free(mem_83615);
        free(mem_83629);
        free(mem_83631);
        free(mem_83645);
        free(mem_83647);
        free(mem_83661);
        free(mem_83663);
        free(mem_83665);
        free(mem_83667);
        free(mem_83669);
        free(mem_83701);
        free(mem_83703);
        free(mem_83705);
        free(mem_83725);
        free(mem_83772);
        free(mem_83773);
        free(mem_83787);
        free(mem_83811);
        free(mem_83814);
        free(mem_83824);
        free(mem_83826);
        free(mem_83893);
        free(mem_83896);
        free(mem_83906);
        free(mem_83908);
        free(mem_83923);
        free(mem_83926);
        free(mem_84018);
        free(mem_84020);
        if (memblock_unref(ctx, &mem_84149, "mem_84149") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84147, "mem_84147") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84136, "mem_84136") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84571, "mem_param_tmp_84571") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84125, "mem_84125") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_84123, "mem_param_84123") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84134, "ext_mem_84134") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84120, "mem_84120") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84145, "ext_mem_84145") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84270, "mem_param_tmp_84270") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84269, "mem_param_tmp_84269") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84268, "mem_param_tmp_84268") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84267, "mem_param_tmp_84267") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84266, "mem_param_tmp_84266") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84265, "mem_param_tmp_84265") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84264, "mem_param_tmp_84264") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84263, "mem_param_tmp_84263") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84262, "mem_param_tmp_84262") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84261, "mem_param_tmp_84261") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84260, "mem_param_tmp_84260") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84259, "mem_param_tmp_84259") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84258, "mem_param_tmp_84258") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84257, "mem_param_tmp_84257") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84256, "mem_param_tmp_84256") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84255, "mem_param_tmp_84255") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84254, "mem_param_tmp_84254") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84253, "mem_param_tmp_84253") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84064, "mem_84064") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84062, "mem_84062") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84060, "mem_84060") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84050, "mem_84050") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84052, "mem_84052") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84532, "mem_param_tmp_84532") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84531, "mem_param_tmp_84531") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_84003, "mem_84003") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83999, "mem_83999") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83965, "mem_83965") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83961, "mem_83961") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83970, "mem_83970") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83968, "mem_83968") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_83985, "ext_mem_83985") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_83988, "ext_mem_83988") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_83890, "mem_param_83890") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_83887, "mem_param_83887") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84009, "ext_mem_84009") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84010, "ext_mem_84010") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83883, "mem_83883") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83879, "mem_83879") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84013, "ext_mem_84013") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84016, "ext_mem_84016") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84055, "ext_mem_84055") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84058, "ext_mem_84058") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83805, "mem_83805") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83803, "mem_83803") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83801, "mem_83801") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_83806, "ext_mem_83806") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_83807, "ext_mem_83807") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_83808, "ext_mem_83808") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83799, "mem_83799") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83797, "mem_83797") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83795, "mem_83795") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83771, "mem_83771") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83769, "mem_83769") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83741, "mem_83741") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82927, "mem_82927") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82925, "mem_82925") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84422, "mem_param_tmp_84422") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83443, "mem_83443") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_83410, "mem_param_83410") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_83452, "ext_mem_83452") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84400, "mem_param_tmp_84400") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83386, "mem_83386") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_83307, "mem_param_83307") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_83391, "ext_mem_83391") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83253, "mem_83253") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_83242, "ext_mem_83242") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83240, "mem_83240") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83237, "mem_83237") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83234, "mem_83234") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83231, "mem_83231") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_83227, "ext_mem_83227") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_83139, "ext_mem_83139") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83137, "mem_83137") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83134, "mem_83134") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83131, "mem_83131") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83128, "mem_83128") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_83126, "ext_mem_83126") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83124, "mem_83124") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83121, "mem_83121") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83118, "mem_83118") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83115, "mem_83115") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_83113, "ext_mem_83113") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83111, "mem_83111") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83108, "mem_83108") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83105, "mem_83105") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_83102, "mem_83102") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82875, "mem_82875") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82873, "mem_82873") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82763, "mem_82763") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82752, "mem_82752") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82703, "mem_82703") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82701, "mem_82701") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82699, "mem_82699") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82697, "mem_param_82697") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82694, "mem_param_82694") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82691, "mem_param_82691") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82688, "mem_param_82688") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82685, "mem_param_82685") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82682, "mem_param_82682") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82679, "mem_param_82679") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82676, "mem_param_82676") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82673, "mem_param_82673") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82670, "mem_param_82670") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82667, "mem_param_82667") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82664, "mem_param_82664") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82661, "mem_param_82661") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82658, "mem_param_82658") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82655, "mem_param_82655") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82652, "mem_param_82652") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82649, "mem_param_82649") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82646, "mem_param_82646") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84101, "ext_mem_84101") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84102, "ext_mem_84102") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84103, "ext_mem_84103") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84104, "ext_mem_84104") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84105, "ext_mem_84105") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84106, "ext_mem_84106") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84107, "ext_mem_84107") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84108, "ext_mem_84108") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84109, "ext_mem_84109") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84110, "ext_mem_84110") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84111, "ext_mem_84111") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84112, "ext_mem_84112") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84113, "ext_mem_84113") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84114, "ext_mem_84114") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84115, "ext_mem_84115") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84116, "ext_mem_84116") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84117, "ext_mem_84117") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_84118, "ext_mem_84118") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82643, "mem_82643") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82642, "mem_82642") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82641, "mem_82641") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82640, "mem_82640") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82639, "mem_82639") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84220, "mem_param_tmp_84220") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_84219, "mem_param_tmp_84219") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82605, "mem_82605") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82601, "mem_82601") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82567, "mem_82567") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82563, "mem_82563") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82572, "mem_82572") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82570, "mem_82570") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_82587, "ext_mem_82587") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_82590, "ext_mem_82590") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82492, "mem_param_82492") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_82489, "mem_param_82489") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_82611, "ext_mem_82611") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_82612, "ext_mem_82612") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82485, "mem_82485") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82481, "mem_82481") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_82615, "ext_mem_82615") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_82410, "mem_82410") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_84195, "mem_out_84195") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_84194, "mem_out_84194") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_84193, "mem_out_84193") != 0)
            return 1;
    }
    return err;
}
FUTHARK_FUN_ATTR int futrts_get_num_neighbours_against_12426(struct futhark_context *ctx, struct memblock *mem_out_p_84726, struct memblock dat1_mem_82400, struct memblock dat1_mem_82401, struct memblock dat2_mem_82402, struct memblock dat2_mem_82403, int64_t n1_40130, int64_t n2_40131, double eps_40136, int8_t dist_t_40137, int8_t angle_t_40138, double radius_40139, int64_t m_sizze_40140)
{
    (void) ctx;
    
    int err = 0;
    struct memblock mem_82405;
    
    mem_82405.references = NULL;
    
    struct memblock mem_out_84193;
    
    mem_out_84193.references = NULL;
    
    int64_t max_res_57216 = smax64((int64_t) 1, n2_40131);
    bool zzero_40143 = max_res_57216 == (int64_t) 0;
    bool nonzzero_40144 = !zzero_40143;
    bool nonzzero_cert_40145;
    
    if (!nonzzero_40144) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../ftDBSCAN_plus.fut:505:55-69\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t max_arg1_40146 = sdiv64(m_sizze_40140, max_res_57216);
    int64_t max_res_57219 = smax64((int64_t) 1, max_arg1_40146);
    int64_t zm_lhs_40150 = add64(n1_40130, max_res_57219);
    int64_t zs_lhs_40152 = sub64(zm_lhs_40150, (int64_t) 1);
    bool zzero_40154 = max_res_57219 == (int64_t) 0;
    bool nonzzero_40155 = !zzero_40154;
    bool nonzzero_cert_40156;
    
    if (!nonzzero_40155) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../ftDBSCAN_plus.fut:506:68-75\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t max_arg1_40157 = sdiv64(zs_lhs_40152, max_res_57219);
    int64_t max_res_57222 = smax64((int64_t) 0, max_arg1_40157);
    int64_t bytes_82404 = (int64_t) 8 * n1_40130;
    bool bounds_invalid_upwards_57224 = slt64(max_res_57222, (int64_t) 0);
    bool valid_57225 = !bounds_invalid_upwards_57224;
    bool range_valid_c_57226;
    
    if (!valid_57225) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) max_res_57222, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  ../../ftDBSCAN_plus.fut:507:69-82\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    if (memblock_alloc(ctx, &mem_82405, bytes_82404, "mem_82405")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t nest_i_84194 = 0; nest_i_84194 < n1_40130; nest_i_84194++) {
        ((int64_t *) mem_82405.mem)[nest_i_84194] = (int64_t) 0;
    }
    for (int64_t i_40165 = 0; i_40165 < max_res_57222; i_40165++) {
        int64_t inf_40168 = mul64(i_40165, max_res_57219);
        int64_t min_arg1_40170 = add64(inf_40168, max_res_57219);
        int64_t min_res_57234 = smin64(n1_40130, min_arg1_40170);
        int64_t dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_40174 = sub64(min_res_57234, inf_40168);
        bool empty_slice_40186 = dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_40174 == (int64_t) 0;
        int64_t m_40187 = sub64(dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_40174, (int64_t) 1);
        int64_t i_p_m_t_s_40189 = add64(inf_40168, m_40187);
        bool zzero_leq_i_p_m_t_s_40190 = sle64((int64_t) 0, i_p_m_t_s_40189);
        bool i_p_m_t_s_leq_w_40192 = slt64(i_p_m_t_s_40189, n1_40130);
        bool zzero_lte_i_40193 = sle64((int64_t) 0, inf_40168);
        bool i_lte_j_40194 = sle64(inf_40168, min_res_57234);
        bool y_40195 = i_p_m_t_s_leq_w_40192 && zzero_lte_i_40193;
        bool y_40196 = zzero_leq_i_p_m_t_s_40190 && y_40195;
        bool forwards_ok_40197 = i_lte_j_40194 && y_40196;
        bool ok_or_empty_40204 = empty_slice_40186 || forwards_ok_40197;
        bool index_certs_40206;
        
        if (!ok_or_empty_40204) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) inf_40168, ":", (long long) min_res_57234, "] out of bounds for array of shape [", (long long) n1_40130, "].", "-> #0  ../../ftDBSCAN_plus.fut:510:47-60\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        for (int64_t i_81466 = 0; i_81466 < dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_40174; i_81466++) {
            int64_t slice_82169 = inf_40168 + i_81466;
            double eta_p_57820 = ((double *) dat1_mem_82400.mem)[slice_82169];
            double eta_p_57821 = ((double *) dat1_mem_82401.mem)[slice_82169];
            double zt_res_76704 = 3.141592653589793 * eta_p_57820;
            double zt_res_76705 = 3.141592653589793 * eta_p_57821;
            double zs_res_76706 = zt_res_76704 / 180.0;
            double zs_res_76707 = zt_res_76705 / 180.0;
            double d_haversine_res_76708;
            
            if (angle_t_40138 == (int8_t) 1) {
                d_haversine_res_76708 = eta_p_57820;
            } else {
                d_haversine_res_76708 = zs_res_76706;
            }
            
            double d_haversine_res_76709;
            
            if (angle_t_40138 == (int8_t) 1) {
                d_haversine_res_76709 = eta_p_57821;
            } else {
                d_haversine_res_76709 = zs_res_76707;
            }
            
            double cos_res_76710 = futrts_cos64(d_haversine_res_76708);
            int64_t defunc_res_76866;
            int64_t redout_81462 = (int64_t) 0;
            
            for (int64_t i_81463 = 0; i_81463 < n2_40131; i_81463++) {
                double eta_p_76762 = ((double *) dat2_mem_82402.mem)[i_81463];
                double eta_p_76763 = ((double *) dat2_mem_82403.mem)[i_81463];
                double dist_res_76764;
                
                if (dist_t_40137 == (int8_t) 0) {
                    bool zeze_res_76855 = eta_p_57820 == eta_p_76762;
                    double d_euclidean_res_76856;
                    
                    if (zeze_res_76855) {
                        double zm_res_76857 = eta_p_57821 - eta_p_76763;
                        double abs_res_76858 = fabs64(zm_res_76857);
                        
                        d_euclidean_res_76856 = abs_res_76858;
                    } else {
                        bool zeze_res_76859 = eta_p_57821 == eta_p_76763;
                        double d_euclidean_res_f_res_76860;
                        
                        if (zeze_res_76859) {
                            double zm_res_76861 = eta_p_57820 - eta_p_76762;
                            double abs_res_76862 = fabs64(zm_res_76861);
                            
                            d_euclidean_res_f_res_76860 = abs_res_76862;
                        } else {
                            double zm_res_76863 = eta_p_57821 - eta_p_76763;
                            double zm_res_76864 = eta_p_57820 - eta_p_76762;
                            double hypot_res_76865 = futrts_hypot64(zm_res_76864, zm_res_76863);
                            
                            d_euclidean_res_f_res_76860 = hypot_res_76865;
                        }
                        d_euclidean_res_76856 = d_euclidean_res_f_res_76860;
                    }
                    dist_res_76764 = d_euclidean_res_76856;
                } else {
                    double d_haversine_res_76776;
                    double d_haversine_res_76777;
                    
                    if (angle_t_40138 == (int8_t) 1) {
                        d_haversine_res_76776 = eta_p_76762;
                        d_haversine_res_76777 = eta_p_76763;
                    } else {
                        double zt_res_76778 = 3.141592653589793 * eta_p_76762;
                        double zs_res_76779 = zt_res_76778 / 180.0;
                        double zt_res_76780 = 3.141592653589793 * eta_p_76763;
                        double zs_res_76781 = zt_res_76780 / 180.0;
                        
                        d_haversine_res_76776 = zs_res_76779;
                        d_haversine_res_76777 = zs_res_76781;
                    }
                    
                    double zm_res_76782 = d_haversine_res_76708 - d_haversine_res_76776;
                    double cos_res_76783 = futrts_cos64(zm_res_76782);
                    double zm_res_76784 = 1.0 - cos_res_76783;
                    double cos_res_76785 = futrts_cos64(d_haversine_res_76776);
                    double zt_res_76786 = cos_res_76710 * cos_res_76785;
                    double zm_res_76787 = d_haversine_res_76709 - d_haversine_res_76777;
                    double zm_res_76788 = 1.0 - zm_res_76787;
                    double zt_res_76789 = zt_res_76786 * zm_res_76788;
                    double zp_res_76790 = zm_res_76784 + zt_res_76789;
                    double zs_res_76791 = zp_res_76790 / 2.0;
                    double min_res_76792 = fmin64(1.0, zs_res_76791);
                    double zt_res_76793 = 2.0 * min_res_76792;
                    double zm_res_76794 = 1.0 - zt_res_76793;
                    double acos_res_76795 = futrts_acos64(zm_res_76794);
                    double zt_res_76796 = radius_40139 * acos_res_76795;
                    
                    dist_res_76764 = zt_res_76796;
                }
                
                bool zlze_res_76798 = dist_res_76764 <= eps_40136;
                int64_t bool_res_76799 = btoi_bool_i64(zlze_res_76798);
                int64_t zp_res_76755 = add64(bool_res_76799, redout_81462);
                int64_t redout_tmp_84197 = zp_res_76755;
                
                redout_81462 = redout_tmp_84197;
            }
            defunc_res_76866 = redout_81462;
            ((int64_t *) mem_82405.mem)[inf_40168 + i_81466] = defunc_res_76866;
        }
    }
    if (memblock_set(ctx, &mem_out_84193, &mem_82405, "mem_82405") != 0)
        return 1;
    if (memblock_set(ctx, &*mem_out_p_84726, &mem_out_84193, "mem_out_84193") != 0)
        return 1;
    
  cleanup:
    {
        if (memblock_unref(ctx, &mem_82405, "mem_82405") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_84193, "mem_out_84193") != 0)
            return 1;
    }
    return err;
}
FUTHARK_FUN_ATTR int futrts_get_num_neighbours_against_12429(struct futhark_context *ctx, struct memblock *mem_out_p_84727, struct memblock dat1_mem_82400, struct memblock dat1_mem_82401, struct memblock dat2_mem_82402, struct memblock dat2_mem_82403, int64_t n1_40393, int64_t n2_40394, double eps_40399, int8_t dist_t_40400, int8_t angle_t_40401, double radius_40402, int64_t m_sizze_40403)
{
    (void) ctx;
    
    int err = 0;
    struct memblock mem_82405;
    
    mem_82405.references = NULL;
    
    struct memblock mem_out_84193;
    
    mem_out_84193.references = NULL;
    
    int64_t max_res_57216 = smax64((int64_t) 1, n2_40394);
    bool zzero_40406 = max_res_57216 == (int64_t) 0;
    bool nonzzero_40407 = !zzero_40406;
    bool nonzzero_cert_40408;
    
    if (!nonzzero_40407) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../ftDBSCAN_plus.fut:505:55-69\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t max_arg1_40409 = sdiv64(m_sizze_40403, max_res_57216);
    int64_t max_res_57219 = smax64((int64_t) 1, max_arg1_40409);
    int64_t zm_lhs_40413 = add64(n1_40393, max_res_57219);
    int64_t zs_lhs_40415 = sub64(zm_lhs_40413, (int64_t) 1);
    bool zzero_40417 = max_res_57219 == (int64_t) 0;
    bool nonzzero_40418 = !zzero_40417;
    bool nonzzero_cert_40419;
    
    if (!nonzzero_40418) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../ftDBSCAN_plus.fut:506:68-75\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t max_arg1_40420 = sdiv64(zs_lhs_40415, max_res_57219);
    int64_t max_res_57222 = smax64((int64_t) 0, max_arg1_40420);
    int64_t bytes_82404 = (int64_t) 8 * n1_40393;
    bool bounds_invalid_upwards_57224 = slt64(max_res_57222, (int64_t) 0);
    bool valid_57225 = !bounds_invalid_upwards_57224;
    bool range_valid_c_57226;
    
    if (!valid_57225) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) max_res_57222, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  ../../ftDBSCAN_plus.fut:507:69-82\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    if (memblock_alloc(ctx, &mem_82405, bytes_82404, "mem_82405")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t nest_i_84194 = 0; nest_i_84194 < n1_40393; nest_i_84194++) {
        ((int64_t *) mem_82405.mem)[nest_i_84194] = (int64_t) 0;
    }
    for (int64_t i_40428 = 0; i_40428 < max_res_57222; i_40428++) {
        int64_t inf_40431 = mul64(i_40428, max_res_57219);
        int64_t min_arg1_40433 = add64(inf_40431, max_res_57219);
        int64_t min_res_57234 = smin64(n1_40393, min_arg1_40433);
        int64_t dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_40437 = sub64(min_res_57234, inf_40431);
        bool empty_slice_40449 = dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_40437 == (int64_t) 0;
        int64_t m_40450 = sub64(dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_40437, (int64_t) 1);
        int64_t i_p_m_t_s_40452 = add64(inf_40431, m_40450);
        bool zzero_leq_i_p_m_t_s_40453 = sle64((int64_t) 0, i_p_m_t_s_40452);
        bool i_p_m_t_s_leq_w_40455 = slt64(i_p_m_t_s_40452, n1_40393);
        bool zzero_lte_i_40456 = sle64((int64_t) 0, inf_40431);
        bool i_lte_j_40457 = sle64(inf_40431, min_res_57234);
        bool y_40458 = i_p_m_t_s_leq_w_40455 && zzero_lte_i_40456;
        bool y_40459 = zzero_leq_i_p_m_t_s_40453 && y_40458;
        bool forwards_ok_40460 = i_lte_j_40457 && y_40459;
        bool ok_or_empty_40467 = empty_slice_40449 || forwards_ok_40460;
        bool index_certs_40469;
        
        if (!ok_or_empty_40467) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) inf_40431, ":", (long long) min_res_57234, "] out of bounds for array of shape [", (long long) n1_40393, "].", "-> #0  ../../ftDBSCAN_plus.fut:510:47-60\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        for (int64_t i_81466 = 0; i_81466 < dzlz7bUZLzmZRz20Usupz20Uinfz7dUzg_40437; i_81466++) {
            int64_t slice_82169 = inf_40431 + i_81466;
            double eta_p_57820 = ((double *) dat1_mem_82400.mem)[slice_82169];
            double eta_p_57821 = ((double *) dat1_mem_82401.mem)[slice_82169];
            double zt_res_76704 = 3.141592653589793 * eta_p_57820;
            double zt_res_76705 = 3.141592653589793 * eta_p_57821;
            double zs_res_76706 = zt_res_76704 / 180.0;
            double zs_res_76707 = zt_res_76705 / 180.0;
            double d_haversine_res_76708;
            
            if (angle_t_40401 == (int8_t) 1) {
                d_haversine_res_76708 = eta_p_57820;
            } else {
                d_haversine_res_76708 = zs_res_76706;
            }
            
            double d_haversine_res_76709;
            
            if (angle_t_40401 == (int8_t) 1) {
                d_haversine_res_76709 = eta_p_57821;
            } else {
                d_haversine_res_76709 = zs_res_76707;
            }
            
            double cos_res_76710 = futrts_cos64(d_haversine_res_76708);
            int64_t defunc_res_76866;
            int64_t redout_81462 = (int64_t) 0;
            
            for (int64_t i_81463 = 0; i_81463 < n2_40394; i_81463++) {
                double eta_p_76762 = ((double *) dat2_mem_82402.mem)[i_81463];
                double eta_p_76763 = ((double *) dat2_mem_82403.mem)[i_81463];
                double dist_res_76764;
                
                if (dist_t_40400 == (int8_t) 0) {
                    bool zeze_res_76855 = eta_p_57820 == eta_p_76762;
                    double d_euclidean_res_76856;
                    
                    if (zeze_res_76855) {
                        double zm_res_76857 = eta_p_57821 - eta_p_76763;
                        double abs_res_76858 = fabs64(zm_res_76857);
                        
                        d_euclidean_res_76856 = abs_res_76858;
                    } else {
                        bool zeze_res_76859 = eta_p_57821 == eta_p_76763;
                        double d_euclidean_res_f_res_76860;
                        
                        if (zeze_res_76859) {
                            double zm_res_76861 = eta_p_57820 - eta_p_76762;
                            double abs_res_76862 = fabs64(zm_res_76861);
                            
                            d_euclidean_res_f_res_76860 = abs_res_76862;
                        } else {
                            double zm_res_76863 = eta_p_57821 - eta_p_76763;
                            double zm_res_76864 = eta_p_57820 - eta_p_76762;
                            double hypot_res_76865 = futrts_hypot64(zm_res_76864, zm_res_76863);
                            
                            d_euclidean_res_f_res_76860 = hypot_res_76865;
                        }
                        d_euclidean_res_76856 = d_euclidean_res_f_res_76860;
                    }
                    dist_res_76764 = d_euclidean_res_76856;
                } else {
                    double d_haversine_res_76776;
                    double d_haversine_res_76777;
                    
                    if (angle_t_40401 == (int8_t) 1) {
                        d_haversine_res_76776 = eta_p_76762;
                        d_haversine_res_76777 = eta_p_76763;
                    } else {
                        double zt_res_76778 = 3.141592653589793 * eta_p_76762;
                        double zs_res_76779 = zt_res_76778 / 180.0;
                        double zt_res_76780 = 3.141592653589793 * eta_p_76763;
                        double zs_res_76781 = zt_res_76780 / 180.0;
                        
                        d_haversine_res_76776 = zs_res_76779;
                        d_haversine_res_76777 = zs_res_76781;
                    }
                    
                    double zm_res_76782 = d_haversine_res_76708 - d_haversine_res_76776;
                    double cos_res_76783 = futrts_cos64(zm_res_76782);
                    double zm_res_76784 = 1.0 - cos_res_76783;
                    double cos_res_76785 = futrts_cos64(d_haversine_res_76776);
                    double zt_res_76786 = cos_res_76710 * cos_res_76785;
                    double zm_res_76787 = d_haversine_res_76709 - d_haversine_res_76777;
                    double zm_res_76788 = 1.0 - zm_res_76787;
                    double zt_res_76789 = zt_res_76786 * zm_res_76788;
                    double zp_res_76790 = zm_res_76784 + zt_res_76789;
                    double zs_res_76791 = zp_res_76790 / 2.0;
                    double min_res_76792 = fmin64(1.0, zs_res_76791);
                    double zt_res_76793 = 2.0 * min_res_76792;
                    double zm_res_76794 = 1.0 - zt_res_76793;
                    double acos_res_76795 = futrts_acos64(zm_res_76794);
                    double zt_res_76796 = radius_40402 * acos_res_76795;
                    
                    dist_res_76764 = zt_res_76796;
                }
                
                bool zlze_res_76798 = dist_res_76764 <= eps_40399;
                int64_t bool_res_76799 = btoi_bool_i64(zlze_res_76798);
                int64_t zp_res_76755 = add64(bool_res_76799, redout_81462);
                int64_t redout_tmp_84197 = zp_res_76755;
                
                redout_81462 = redout_tmp_84197;
            }
            defunc_res_76866 = redout_81462;
            ((int64_t *) mem_82405.mem)[inf_40431 + i_81466] = defunc_res_76866;
        }
    }
    if (memblock_set(ctx, &mem_out_84193, &mem_82405, "mem_82405") != 0)
        return 1;
    if (memblock_set(ctx, &*mem_out_p_84727, &mem_out_84193, "mem_out_84193") != 0)
        return 1;
    
  cleanup:
    {
        if (memblock_unref(ctx, &mem_82405, "mem_82405") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_84193, "mem_out_84193") != 0)
            return 1;
    }
    return err;
}
FUTHARK_FUN_ATTR int futrts_greatest_divisor_leq_than_5592(struct futhark_context *ctx, int64_t *out_prim_out_84728, int64_t upper_bound_33545, int64_t n_33546)
{
    (void) ctx;
    
    int err = 0;
    int64_t prim_out_84193;
    bool upper_bound_33547 = sle64((int64_t) 1, upper_bound_33545);
    bool assert_c_33549;
    
    if (!upper_bound_33547) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "Assertion is false: (upper_bound >= 1)", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:189:21-58\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool cond_33558 = slt64(upper_bound_33545, n_33546);
    bool d_33567;
    int64_t d_33568;
    bool loop_while_33569;
    int64_t d_33570;
    
    loop_while_33569 = cond_33558;
    d_33570 = (int64_t) 1;
    while (loop_while_33569) {
        int64_t loopres_33571 = add64((int64_t) 1, d_33570);
        bool zzero_33573 = loopres_33571 == (int64_t) 0;
        bool nonzzero_33574 = !zzero_33573;
        bool nonzzero_cert_33575;
        
        if (!nonzzero_33574) {
            set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:190:36-38\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t zg_lhs_33576 = sdiv64(n_33546, loopres_33571);
        bool cond_33578 = slt64(upper_bound_33545, zg_lhs_33576);
        bool loop_cond_33579;
        
        if (cond_33578) {
            loop_cond_33579 = 1;
        } else {
            int64_t znze_lhs_33583 = smod64(n_33546, loopres_33571);
            bool loop_cond_f_res_33585 = znze_lhs_33583 == (int64_t) 0;
            bool loop_cond_f_res_33586 = !loop_cond_f_res_33585;
            
            loop_cond_33579 = loop_cond_f_res_33586;
        }
        
        bool loop_while_tmp_84194 = loop_cond_33579;
        int64_t d_tmp_84195 = loopres_33571;
        
        loop_while_33569 = loop_while_tmp_84194;
        d_33570 = d_tmp_84195;
    }
    d_33567 = loop_while_33569;
    d_33568 = d_33570;
    
    bool zzero_33588 = d_33568 == (int64_t) 0;
    bool nonzzero_33589 = !zzero_33588;
    bool nonzzero_cert_33590;
    
    if (!nonzzero_33589) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  ../../lib/github.com/diku-dk/sorts/merge_sort.fut:191:7-9\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t greatest_divisor_leq_than_res_33591 = sdiv64(n_33546, d_33568);
    
    prim_out_84193 = greatest_divisor_leq_than_res_33591;
    *out_prim_out_84728 = prim_out_84193;
    
  cleanup:
    { }
    return err;
}
FUTHARK_FUN_ATTR int futrts_lifted_get_grid_id_12083(struct futhark_context *ctx, int64_t *out_prim_out_84729, double subdv_x_33878, double step_x_33879, double step_y_33880, double x_33881, double y_33882)
{
    (void) ctx;
    
    int err = 0;
    int64_t prim_out_84193;
    double zbzg_lhs_33883 = x_33881 / step_x_33879;
    double floor_res_57909 = futrts_floor64(zbzg_lhs_33883);
    double zt_lhs_33888 = y_33882 / step_y_33880;
    double zbzg_lhs_33890 = subdv_x_33878 * zt_lhs_33888;
    double floor_res_57912 = futrts_floor64(zbzg_lhs_33890);
    double zbzg_lhs_33895 = floor_res_57909 + floor_res_57912;
    int64_t f64_res_57810 = fptosi_f64_i64(zbzg_lhs_33895);
    
    prim_out_84193 = f64_res_57810;
    *out_prim_out_84729 = prim_out_84193;
    
  cleanup:
    { }
    return err;
}
FUTHARK_FUN_ATTR int futrts_lifted_lambda_12077(struct futhark_context *ctx, bool *out_prim_out_84730, int64_t c1_51237, int64_t p1_51238, int64_t c2_51239, int64_t p2_51240)
{
    (void) ctx;
    
    int err = 0;
    bool prim_out_84193;
    bool cond_51241 = slt64(c1_51237, c2_51239);
    bool cond_51243 = c1_51237 == c2_51239;
    bool lifted_lambda_res_f_res_t_res_51245 = sle64(p1_51238, p2_51240);
    bool x_57133 = cond_51243 && lifted_lambda_res_f_res_t_res_51245;
    bool x_57131 = !cond_51241;
    bool y_57132 = x_57131 && x_57133;
    bool lifted_lambda_res_51242 = cond_51241 || y_57132;
    
    prim_out_84193 = lifted_lambda_res_51242;
    *out_prim_out_84730 = prim_out_84193;
    
  cleanup:
    { }
    return err;
}
FUTHARK_FUN_ATTR int futrts_lifted_normalizze_11978(struct futhark_context *ctx, int64_t *out_prim_out_84731, int64_t *out_prim_out_84732, int64_t *out_prim_out_84733, int64_t *out_prim_out_84734, int64_t *out_prim_out_84735, int64_t slo_33778, int64_t shi_33779, int64_t tlo_33780, int64_t thi_33781, int64_t count_33782)
{
    (void) ctx;
    
    int err = 0;
    int64_t prim_out_84193;
    int64_t prim_out_84194;
    int64_t prim_out_84195;
    int64_t prim_out_84196;
    int64_t prim_out_84197;
    int64_t min_arg1_33785 = add64(slo_33778, count_33782);
    int64_t min_res_57216 = smin64(shi_33779, min_arg1_33785);
    int64_t min_arg1_33789 = add64(tlo_33780, count_33782);
    int64_t min_res_57219 = smin64(thi_33781, min_arg1_33789);
    int64_t zp_lhs_33793 = sub64(min_res_57216, slo_33778);
    int64_t zp_rhs_33795 = sub64(min_res_57219, tlo_33780);
    int64_t zm_lhs_33797 = add64(zp_lhs_33793, zp_rhs_33795);
    int64_t slack_33799 = sub64(zm_lhs_33797, count_33782);
    int64_t min_res_57222 = smin64(zp_lhs_33793, slack_33799);
    int64_t min_res_57225 = smin64(zp_rhs_33795, min_res_57222);
    int64_t max_arg1_33809 = sub64(min_res_57216, min_res_57225);
    int64_t max_res_57228 = smax64(slo_33778, max_arg1_33809);
    int64_t max_arg1_33813 = sub64(min_res_57219, min_res_57225);
    int64_t max_res_57231 = smax64(tlo_33780, max_arg1_33813);
    int64_t zm_rhs_33817 = sub64(max_res_57228, slo_33778);
    int64_t zm_lhs_33819 = sub64(count_33782, zm_rhs_33817);
    int64_t zm_rhs_33821 = sub64(max_res_57231, tlo_33780);
    int64_t count_33823 = sub64(zm_lhs_33819, zm_rhs_33821);
    
    prim_out_84193 = max_res_57228;
    prim_out_84194 = min_res_57216;
    prim_out_84195 = max_res_57231;
    prim_out_84196 = min_res_57219;
    prim_out_84197 = count_33823;
    *out_prim_out_84731 = prim_out_84193;
    *out_prim_out_84732 = prim_out_84194;
    *out_prim_out_84733 = prim_out_84195;
    *out_prim_out_84734 = prim_out_84196;
    *out_prim_out_84735 = prim_out_84197;
    
  cleanup:
    { }
    return err;
}

int futhark_entry_test_dbscan_plus(struct futhark_context *ctx, struct futhark_f64_1d **out0, struct futhark_f64_1d **out1, struct futhark_i64_1d **out2, const struct futhark_f64_2d *in0)
{
    int64_t dz2081U_57123 = (int64_t) 0;
    int64_t prim_out_84196 = (int64_t) 0;
    int64_t prim_out_84197 = (int64_t) 0;
    int ret = 0;
    
    lock_lock(&ctx->lock);
    
    struct memblock mem_out_84195;
    
    mem_out_84195.references = NULL;
    
    struct memblock mem_out_84194;
    
    mem_out_84194.references = NULL;
    
    struct memblock mem_out_84193;
    
    mem_out_84193.references = NULL;
    
    struct memblock pts_mem_82400;
    
    pts_mem_82400.references = NULL;
    pts_mem_82400 = in0->mem;
    dz2081U_57123 = in0->shape[1];
    if (!((int64_t) 2 == in0->shape[0] && dz2081U_57123 == in0->shape[1])) {
        ret = 1;
        set_error(ctx, msgprintf("Error: entry point arguments have invalid sizes.\n"));
    }
    if (ret == 0) {
        ret = futrts_entry_test_dbscan_plus(ctx, &mem_out_84193, &mem_out_84194, &mem_out_84195, &prim_out_84196, &prim_out_84197, pts_mem_82400, dz2081U_57123);
        if (ret == 0) {
            assert((*out0 = (struct futhark_f64_1d *) malloc(sizeof(struct futhark_f64_1d))) != NULL);
            (*out0)->mem = mem_out_84193;
            (*out0)->shape[0] = prim_out_84196;
            assert((*out1 = (struct futhark_f64_1d *) malloc(sizeof(struct futhark_f64_1d))) != NULL);
            (*out1)->mem = mem_out_84194;
            (*out1)->shape[0] = prim_out_84196;
            assert((*out2 = (struct futhark_i64_1d *) malloc(sizeof(struct futhark_i64_1d))) != NULL);
            (*out2)->mem = mem_out_84195;
            (*out2)->shape[0] = prim_out_84197;
        }
    }
    lock_unlock(&ctx->lock);
    return ret;
}
  
