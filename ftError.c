
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
struct futhark_u8_1d;
struct futhark_u8_1d *futhark_new_u8_1d(struct futhark_context *ctx, const uint8_t *data, int64_t dim0);
struct futhark_u8_1d *futhark_new_raw_u8_1d(struct futhark_context *ctx, unsigned char *data, int64_t dim0);
int futhark_free_u8_1d(struct futhark_context *ctx, struct futhark_u8_1d *arr);
int futhark_values_u8_1d(struct futhark_context *ctx, struct futhark_u8_1d *arr, uint8_t *data);
int futhark_index_u8_1d(struct futhark_context *ctx, uint8_t *out, struct futhark_u8_1d *arr, int64_t i0);
unsigned char *futhark_values_raw_u8_1d(struct futhark_context *ctx, struct futhark_u8_1d *arr);
const int64_t *futhark_shape_u8_1d(struct futhark_context *ctx, struct futhark_u8_1d *arr);

// Opaque values
struct futhark_opaque_sortStruct_int;
int futhark_free_opaque_sortStruct_int(struct futhark_context *ctx, struct futhark_opaque_sortStruct_int *obj);
int futhark_store_opaque_sortStruct_int(struct futhark_context *ctx, const struct futhark_opaque_sortStruct_int *obj, void **p, size_t *n);
struct futhark_opaque_sortStruct_int *futhark_restore_opaque_sortStruct_int(struct futhark_context *ctx, const void *p);
int futhark_project_opaque_sortStruct_int_k(struct futhark_context *ctx, struct futhark_i32_1d **out, const struct futhark_opaque_sortStruct_int *obj);
int futhark_project_opaque_sortStruct_int_pL(struct futhark_context *ctx, struct futhark_u8_1d **out, const struct futhark_opaque_sortStruct_int *obj);
int futhark_new_opaque_sortStruct_int(struct futhark_context *ctx, struct futhark_opaque_sortStruct_int **out, const struct futhark_i32_1d *f_k, const struct futhark_u8_1d *f_pL);

// Entry points
int futhark_entry_mergeSortRelation_int(struct futhark_context *ctx, struct futhark_opaque_sortStruct_int **out0, const int64_t in0, const struct futhark_opaque_sortStruct_int *in1);
int futhark_entry_radixSortRelation_int(struct futhark_context *ctx, struct futhark_opaque_sortStruct_int **out0, const int16_t in0, const int64_t in1, const struct futhark_opaque_sortStruct_int *in2);

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
#include <unistd.h>
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

static int binary_output = 0;
static int print_result = 1;
static FILE *runtime_file;
static int perform_warmup = 0;
static int num_runs = 1;
static const char *entry_point = "main";
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

int parse_options(struct futhark_context_config *cfg, int argc, char *const argv[])
{
    int ch;
    static struct option long_options[] = {{"write-runtime-to", required_argument, NULL, 1}, {"runs", required_argument, NULL, 2}, {"debugging", no_argument, NULL, 3}, {"log", no_argument, NULL, 4}, {"profile", no_argument, NULL, 5}, {"entry-point", required_argument, NULL, 6}, {"binary-output", no_argument, NULL, 7}, {"no-print-result", no_argument, NULL, 8}, {"help", no_argument, NULL, 9}, {"print-params", no_argument, NULL, 10}, {"param", required_argument, NULL, 11}, {"tuning", required_argument, NULL, 12}, {"cache-file", required_argument, NULL, 13}, {0, 0, 0, 0}};
    static char *option_descriptions = "  -t/--write-runtime-to FILE Print the time taken to execute the program to the indicated file, an integral number of microseconds.\n  -r/--runs INT              Perform NUM runs of the program.\n  -D/--debugging             Perform possibly expensive internal correctness checks and verbose logging.\n  -L/--log                   Print various low-overhead logging information to stderr while running.\n  -P/--profile               Enable the collection of profiling information.\n  -e/--entry-point NAME      The entry point to run. Defaults to main.\n  -b/--binary-output         Print the program result in the binary output format.\n  -n/--no-print-result       Do not print the program result.\n  -h/--help                  Print help information and exit.\n  --print-params             Print all tuning parameters that can be set with --param or --tuning.\n  --param ASSIGNMENT         Set a tuning parameter to the given value.\n  --tuning FILE              Read size=value assignments from the given file.\n  --cache-file FILE          Store program cache here.\n";
    
    while ((ch = getopt_long(argc, argv, ":t:r:DLPe:bnh", long_options, NULL)) != -1) {
        if (ch == 1 || ch == 't') {
            runtime_file = fopen(optarg, "w");
            if (runtime_file == NULL)
                futhark_panic(1, "Cannot open %s: %s\n", optarg, strerror(errno));
        }
        if (ch == 2 || ch == 'r') {
            num_runs = atoi(optarg);
            perform_warmup = 1;
            if (num_runs <= 0)
                futhark_panic(1, "Need a positive number of runs, not %s\n", optarg);
        }
        if (ch == 3 || ch == 'D')
            futhark_context_config_set_debugging(cfg, 1);
        if (ch == 4 || ch == 'L')
            futhark_context_config_set_logging(cfg, 1);
        if (ch == 5 || ch == 'P')
            futhark_context_config_set_profiling(cfg, 1);
        if (ch == 6 || ch == 'e') {
            if (entry_point != NULL)
                entry_point = optarg;
        }
        if (ch == 7 || ch == 'b')
            binary_output = 1;
        if (ch == 8 || ch == 'n')
            print_result = 0;
        if (ch == 9 || ch == 'h') {
            printf("Usage: %s [OPTION]...\nOptions:\n\n%s\nFor more information, consult the Futhark User's Guide or the man pages.\n", fut_progname, option_descriptions);
            exit(0);
        }
        if (ch == 10) {
            int n = futhark_get_tuning_param_count();
            
            for (int i = 0; i < n; i++)
                printf("%s (%s)\n", futhark_get_tuning_param_name(i), futhark_get_tuning_param_class(i));
            exit(0);
        }
        if (ch == 11) {
            char *name = optarg;
            char *equals = strstr(optarg, "=");
            char *value_str = equals != NULL ? equals + 1 : optarg;
            int value = atoi(value_str);
            
            if (equals != NULL) {
                *equals = 0;
                if (futhark_context_config_set_tuning_param(cfg, name, (size_t) value) != 0)
                    futhark_panic(1, "Unknown size: %s\n", name);
            } else
                futhark_panic(1, "Invalid argument for size option: %s\n", optarg);
        }
        if (ch == 12) {
            char *ret = load_tuning_file(optarg, cfg, (int (*)(void *, const char *, size_t)) futhark_context_config_set_tuning_param);
            
            if (ret != NULL)
                futhark_panic(1, "When loading tuning file '%s': %s\n", optarg, ret);
        }
        if (ch == 13)
            futhark_context_config_set_cache_file(cfg, optarg);
        if (ch == ':')
            futhark_panic(-1, "Missing argument for option %s\n", argv[optind - 1]);
        if (ch == '?') {
            fprintf(stderr, "Usage: %s [OPTIONS]...\nOptions:\n\n%s\n", fut_progname, "  -t/--write-runtime-to FILE Print the time taken to execute the program to the indicated file, an integral number of microseconds.\n  -r/--runs INT              Perform NUM runs of the program.\n  -D/--debugging             Perform possibly expensive internal correctness checks and verbose logging.\n  -L/--log                   Print various low-overhead logging information to stderr while running.\n  -P/--profile               Enable the collection of profiling information.\n  -e/--entry-point NAME      The entry point to run. Defaults to main.\n  -b/--binary-output         Print the program result in the binary output format.\n  -n/--no-print-result       Do not print the program result.\n  -h/--help                  Print help information and exit.\n  --print-params             Print all tuning parameters that can be set with --param or --tuning.\n  --param ASSIGNMENT         Set a tuning parameter to the given value.\n  --tuning FILE              Read size=value assignments from the given file.\n  --cache-file FILE          Store program cache here.\n");
            futhark_panic(1, "Unknown option: %s\n", argv[optind - 1]);
        }
    }
    return optind;
}
static int futrts_cli_entry_mergeSortRelation_int(struct futhark_context *ctx)
{
    int64_t t_start, t_end;
    int time_runs = 0, profile_run = 0;
    int retval = 0;
    
    // We do not want to profile all the initialisation.
    futhark_context_pause_profiling(ctx);
    // Declare and read input.
    set_binary_mode(stdin);
    
    int64_t read_value_0;
    
    if (read_scalar(stdin, &i64_info, &read_value_0) != 0)
        futhark_panic(1, "Error when reading input #%d of type %s (errno: %s).\n", 0, "i64", strerror(errno));
    ;
    futhark_panic(1, "Cannot read input #%d of type %s\n", 1, "struct futhark_opaque_sortStruct_int *");
    if (end_of_input(stdin) != 0)
        futhark_panic(1, "Expected EOF on stdin after reading input for \"%s\".\n", "mergeSortRelation_int");
    
    struct futhark_opaque_sortStruct_int * result_0;
    
    if (perform_warmup) {
        int r;
        
        ;
        ;
        if (futhark_context_sync(ctx) != 0)
            futhark_panic(1, "%s", futhark_context_get_error(ctx));
        ;
        // Only profile last run.
        if (profile_run)
            futhark_context_unpause_profiling(ctx);
        t_start = get_wall_time();
        r = futhark_entry_mergeSortRelation_int(ctx, &result_0, read_value_0, NULL);
        if (r != 0)
            futhark_panic(1, "%s", futhark_context_get_error(ctx));
        if (futhark_context_sync(ctx) != 0)
            futhark_panic(1, "%s", futhark_context_get_error(ctx));
        ;
        if (profile_run)
            futhark_context_pause_profiling(ctx);
        t_end = get_wall_time();
        
        long elapsed_usec = t_end - t_start;
        
        if (time_runs && runtime_file != NULL) {
            fprintf(runtime_file, "%lld\n", (long long) elapsed_usec);
            fflush(runtime_file);
        }
        ;
        ;
        assert(futhark_free_opaque_sortStruct_int(ctx, result_0) == 0);
    }
    time_runs = 1;
    // Proper run.
    for (int run = 0; run < num_runs; run++) {
        // Only profile last run.
        profile_run = run == num_runs - 1;
        
        int r;
        
        ;
        ;
        if (futhark_context_sync(ctx) != 0)
            futhark_panic(1, "%s", futhark_context_get_error(ctx));
        ;
        // Only profile last run.
        if (profile_run)
            futhark_context_unpause_profiling(ctx);
        t_start = get_wall_time();
        r = futhark_entry_mergeSortRelation_int(ctx, &result_0, read_value_0, NULL);
        if (r != 0)
            futhark_panic(1, "%s", futhark_context_get_error(ctx));
        if (futhark_context_sync(ctx) != 0)
            futhark_panic(1, "%s", futhark_context_get_error(ctx));
        ;
        if (profile_run)
            futhark_context_pause_profiling(ctx);
        t_end = get_wall_time();
        
        long elapsed_usec = t_end - t_start;
        
        if (time_runs && runtime_file != NULL) {
            fprintf(runtime_file, "%lld\n", (long long) elapsed_usec);
            fflush(runtime_file);
        }
        ;
        ;
        if (run < num_runs - 1) {
            assert(futhark_free_opaque_sortStruct_int(ctx, result_0) == 0);
        }
    }
    ;
    ;
    if (print_result) {
        // Print the final result.
        if (binary_output)
            set_binary_mode(stdout);
        {
            fprintf(stderr, "Values of type \"%s\" have no external representation.\n", "struct futhark_opaque_sortStruct_int *");
            retval = 1;
            goto print_end;
        }
        printf("\n");
    }
    
  print_end:
    { }
    assert(futhark_free_opaque_sortStruct_int(ctx, result_0) == 0);
    return retval;
}
static int futrts_cli_entry_radixSortRelation_int(struct futhark_context *ctx)
{
    int64_t t_start, t_end;
    int time_runs = 0, profile_run = 0;
    int retval = 0;
    
    // We do not want to profile all the initialisation.
    futhark_context_pause_profiling(ctx);
    // Declare and read input.
    set_binary_mode(stdin);
    
    int16_t read_value_0;
    
    if (read_scalar(stdin, &i16_info, &read_value_0) != 0)
        futhark_panic(1, "Error when reading input #%d of type %s (errno: %s).\n", 0, "i16", strerror(errno));
    ;
    
    int64_t read_value_1;
    
    if (read_scalar(stdin, &i64_info, &read_value_1) != 0)
        futhark_panic(1, "Error when reading input #%d of type %s (errno: %s).\n", 1, "i64", strerror(errno));
    ;
    futhark_panic(1, "Cannot read input #%d of type %s\n", 2, "struct futhark_opaque_sortStruct_int *");
    if (end_of_input(stdin) != 0)
        futhark_panic(1, "Expected EOF on stdin after reading input for \"%s\".\n", "radixSortRelation_int");
    
    struct futhark_opaque_sortStruct_int * result_0;
    
    if (perform_warmup) {
        int r;
        
        ;
        ;
        ;
        if (futhark_context_sync(ctx) != 0)
            futhark_panic(1, "%s", futhark_context_get_error(ctx));
        ;
        // Only profile last run.
        if (profile_run)
            futhark_context_unpause_profiling(ctx);
        t_start = get_wall_time();
        r = futhark_entry_radixSortRelation_int(ctx, &result_0, read_value_0, read_value_1, NULL);
        if (r != 0)
            futhark_panic(1, "%s", futhark_context_get_error(ctx));
        if (futhark_context_sync(ctx) != 0)
            futhark_panic(1, "%s", futhark_context_get_error(ctx));
        ;
        if (profile_run)
            futhark_context_pause_profiling(ctx);
        t_end = get_wall_time();
        
        long elapsed_usec = t_end - t_start;
        
        if (time_runs && runtime_file != NULL) {
            fprintf(runtime_file, "%lld\n", (long long) elapsed_usec);
            fflush(runtime_file);
        }
        ;
        ;
        ;
        assert(futhark_free_opaque_sortStruct_int(ctx, result_0) == 0);
    }
    time_runs = 1;
    // Proper run.
    for (int run = 0; run < num_runs; run++) {
        // Only profile last run.
        profile_run = run == num_runs - 1;
        
        int r;
        
        ;
        ;
        ;
        if (futhark_context_sync(ctx) != 0)
            futhark_panic(1, "%s", futhark_context_get_error(ctx));
        ;
        // Only profile last run.
        if (profile_run)
            futhark_context_unpause_profiling(ctx);
        t_start = get_wall_time();
        r = futhark_entry_radixSortRelation_int(ctx, &result_0, read_value_0, read_value_1, NULL);
        if (r != 0)
            futhark_panic(1, "%s", futhark_context_get_error(ctx));
        if (futhark_context_sync(ctx) != 0)
            futhark_panic(1, "%s", futhark_context_get_error(ctx));
        ;
        if (profile_run)
            futhark_context_pause_profiling(ctx);
        t_end = get_wall_time();
        
        long elapsed_usec = t_end - t_start;
        
        if (time_runs && runtime_file != NULL) {
            fprintf(runtime_file, "%lld\n", (long long) elapsed_usec);
            fflush(runtime_file);
        }
        ;
        ;
        ;
        if (run < num_runs - 1) {
            assert(futhark_free_opaque_sortStruct_int(ctx, result_0) == 0);
        }
    }
    ;
    ;
    ;
    if (print_result) {
        // Print the final result.
        if (binary_output)
            set_binary_mode(stdout);
        {
            fprintf(stderr, "Values of type \"%s\" have no external representation.\n", "struct futhark_opaque_sortStruct_int *");
            retval = 1;
            goto print_end;
        }
        printf("\n");
    }
    
  print_end:
    { }
    assert(futhark_free_opaque_sortStruct_int(ctx, result_0) == 0);
    return retval;
}
typedef int entry_point_fun(struct futhark_context *);
struct entry_point_entry {
    const char *name;
    entry_point_fun *fun;
};
int main(int argc, char **argv)
{
    int retval = 0;
    
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
    
    char *error = futhark_context_get_error(ctx);
    
    if (error != NULL)
        futhark_panic(1, "%s", error);
    
    struct entry_point_entry entry_points[] = {{.name ="mergeSortRelation_int", .fun =futrts_cli_entry_mergeSortRelation_int}, {.name ="radixSortRelation_int", .fun =futrts_cli_entry_radixSortRelation_int}};
    
    if (entry_point != NULL) {
        int num_entry_points = sizeof(entry_points) / sizeof(entry_points[0]);
        entry_point_fun *entry_point_fun = NULL;
        
        for (int i = 0; i < num_entry_points; i++) {
            if (strcmp(entry_points[i].name, entry_point) == 0) {
                entry_point_fun = entry_points[i].fun;
                break;
            }
        }
        if (entry_point_fun == NULL) {
            fprintf(stderr, "No entry point '%s'.  Select another with --entry-point.  Options are:\n", entry_point);
            for (int i = 0; i < num_entry_points; i++)
                fprintf(stderr, "%s\n", entry_points[i].name);
            return 1;
        }
        if (isatty(fileno(stdin))) {
            fprintf(stderr, "Reading input from TTY.\n");
            fprintf(stderr, "Send EOF (CTRL-d) after typing all input values.\n");
        }
        retval = entry_point_fun(ctx);
        if (runtime_file != NULL)
            fclose(runtime_file);
    }
    futhark_context_free(ctx);
    futhark_context_config_free(cfg);
    return retval;
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

FUTHARK_FUN_ATTR int futrts_entry_mergeSortRelation_int(struct futhark_context *ctx, struct memblock *mem_out_p_24325, struct memblock *mem_out_p_24326, struct memblock k_mem_23976, struct memblock pL_mem_23977, int64_t n_17305, int64_t dzlz7bUZLztZRz20Unz20Upayload_bytesz7dUzg_17306, int64_t payload_bytes_17307);
FUTHARK_FUN_ATTR int futrts_entry_radixSortRelation_int(struct futhark_context *ctx, struct memblock *mem_out_p_24338, struct memblock *mem_out_p_24339, struct memblock k_mem_23976, struct memblock pL_mem_23977, int64_t n_14783, int64_t dzlz7bUZLztZRz20Unz20Upayload_bytesz7dUzg_14784, int16_t block_sizze_14785, int64_t payload_bytes_14786);
FUTHARK_FUN_ATTR int futrts_lifted_f_6833(struct futhark_context *ctx, int64_t *out_prim_out_24366, int16_t na_12050, int16_t nb_12051, int16_t nc_12052, int16_t bin_12053, int16_t a_12054, int16_t b_12055, int16_t c_12056, int16_t d_12057);
FUTHARK_FUN_ATTR int futrts_lifted_lambda_6832(struct futhark_context *ctx, int16_t *out_prim_out_24367, int16_t *out_prim_out_24368, int16_t *out_prim_out_24369, int16_t *out_prim_out_24370, int16_t x_11993);
FUTHARK_FUN_ATTR int futrts_lifted_normalizze_6809(struct futhark_context *ctx, int64_t *out_prim_out_24371, int64_t *out_prim_out_24372, int64_t *out_prim_out_24373, int64_t *out_prim_out_24374, int64_t *out_prim_out_24375, int64_t slo_15029, int64_t shi_15030, int64_t tlo_15031, int64_t thi_15032, int64_t count_15033);

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
struct futhark_u8_1d {
    struct memblock mem;
    int64_t shape[1];
};
struct futhark_u8_1d *futhark_new_u8_1d(struct futhark_context *ctx, const uint8_t *data, int64_t dim0)
{
    int err = 0;
    struct futhark_u8_1d *bad = NULL;
    struct futhark_u8_1d *arr = (struct futhark_u8_1d *) malloc(sizeof(struct futhark_u8_1d));
    
    if (arr == NULL)
        return bad;
    lock_lock(&ctx->lock);
    arr->mem.references = NULL;
    if (memblock_alloc(ctx, &arr->mem, dim0 * 1, "arr->mem"))
        err = 1;
    arr->shape[0] = dim0;
    if ((size_t) dim0 * 1 > 0)
        memmove(arr->mem.mem + 0, (const unsigned char *) data + 0, (size_t) dim0 * 1);
    lock_unlock(&ctx->lock);
    if (err != 0) {
        free(arr);
        return bad;
    }
    return arr;
}
struct futhark_u8_1d *futhark_new_raw_u8_1d(struct futhark_context *ctx, unsigned char *data, int64_t dim0)
{
    int err = 0;
    struct futhark_u8_1d *bad = NULL;
    struct futhark_u8_1d *arr = (struct futhark_u8_1d *) malloc(sizeof(struct futhark_u8_1d));
    
    if (arr == NULL)
        return bad;
    lock_lock(&ctx->lock);
    arr->mem.references = NULL;
    arr->mem.mem = data;
    arr->shape[0] = dim0;
    lock_unlock(&ctx->lock);
    return arr;
}
int futhark_free_u8_1d(struct futhark_context *ctx, struct futhark_u8_1d *arr)
{
    lock_lock(&ctx->lock);
    if (memblock_unref(ctx, &arr->mem, "arr->mem") != 0)
        return 1;
    lock_unlock(&ctx->lock);
    free(arr);
    return 0;
}
int futhark_values_u8_1d(struct futhark_context *ctx, struct futhark_u8_1d *arr, uint8_t *data)
{
    int err = 0;
    
    lock_lock(&ctx->lock);
    if ((size_t) arr->shape[0] * 1 > 0)
        memmove((unsigned char *) data + 0, arr->mem.mem + 0, (size_t) arr->shape[0] * 1);
    lock_unlock(&ctx->lock);
    return err;
}
int futhark_index_u8_1d(struct futhark_context *ctx, uint8_t *out, struct futhark_u8_1d *arr, int64_t i0)
{
    int err = 0;
    
    if (i0 >= 0 && i0 < arr->shape[0]) {
        lock_lock(&ctx->lock);
        if (1 > 0)
            memmove((unsigned char *) out + 0, arr->mem.mem + 1 * (i0 * 1), 1);
        lock_unlock(&ctx->lock);
    } else {
        err = 1;
        set_error(ctx, strdup("Index out of bounds."));
    }
    return err;
}
unsigned char *futhark_values_raw_u8_1d(struct futhark_context *ctx, struct futhark_u8_1d *arr)
{
    (void) ctx;
    return arr->mem.mem;
}
const int64_t *futhark_shape_u8_1d(struct futhark_context *ctx, struct futhark_u8_1d *arr)
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
struct futhark_opaque_sortStruct_int {
    struct futhark_i32_1d *v0;
    struct futhark_u8_1d *v1;
};
int futhark_project_opaque_sortStruct_int_k(struct futhark_context *ctx, struct futhark_i32_1d **out, const struct futhark_opaque_sortStruct_int *obj)
{
    (void) ctx;
    
    struct futhark_i32_1d *v;
    
    lock_lock(&ctx->lock);
    v = malloc(sizeof(struct futhark_i32_1d));
    memcpy(v, obj->v0, sizeof(struct futhark_i32_1d));
    (void) (*v->mem.references)++;
    lock_unlock(&ctx->lock);
    *out = v;
    return 0;
}
int futhark_project_opaque_sortStruct_int_pL(struct futhark_context *ctx, struct futhark_u8_1d **out, const struct futhark_opaque_sortStruct_int *obj)
{
    (void) ctx;
    
    struct futhark_u8_1d *v;
    
    lock_lock(&ctx->lock);
    v = malloc(sizeof(struct futhark_u8_1d));
    memcpy(v, obj->v1, sizeof(struct futhark_u8_1d));
    (void) (*v->mem.references)++;
    lock_unlock(&ctx->lock);
    *out = v;
    return 0;
}
int futhark_new_opaque_sortStruct_int(struct futhark_context *ctx, struct futhark_opaque_sortStruct_int **out, const struct futhark_i32_1d *f_k, const struct futhark_u8_1d *f_pL)
{
    struct futhark_opaque_sortStruct_int *v = malloc(sizeof(struct futhark_opaque_sortStruct_int));
    
    lock_lock(&ctx->lock);
    {
        v->v0 = malloc(sizeof(struct futhark_i32_1d));
        *v->v0 = *f_k;
        (void) (*v->v0->mem.references)++;
    }
    {
        v->v1 = malloc(sizeof(struct futhark_u8_1d));
        *v->v1 = *f_pL;
        (void) (*v->v1->mem.references)++;
    }
    lock_unlock(&ctx->lock);
    *out = v;
    return FUTHARK_SUCCESS;
}
int futhark_free_opaque_sortStruct_int(struct futhark_context *ctx, struct futhark_opaque_sortStruct_int *obj)
{
    (void) ctx;
    
    int ret = 0, tmp;
    
    if (obj->v0 != NULL && (tmp = futhark_free_i32_1d(ctx, obj->v0)) != 0)
        ret = tmp;
    if (obj->v1 != NULL && (tmp = futhark_free_u8_1d(ctx, obj->v1)) != 0)
        ret = tmp;
    free(obj);
    return ret;
}
int futhark_store_opaque_sortStruct_int(struct futhark_context *ctx, const struct futhark_opaque_sortStruct_int *obj, void **p, size_t *n)
{
    (void) ctx;
    
    int ret = 0;
    int64_t size_0 = 7 + 1 * sizeof(int64_t) + futhark_shape_i32_1d(ctx, obj->v0)[0] * sizeof(int32_t);
    int64_t size_1 = 7 + 1 * sizeof(int64_t) + futhark_shape_u8_1d(ctx, obj->v1)[0] * sizeof(int8_t);
    
    *n = size_0 + size_1;
    if (p != NULL && *p == NULL)
        *p = malloc(*n);
    if (p != NULL) {
        unsigned char *out = *p;
        
        *out++ = 'b';
        *out++ = 2;
        *out++ = 1;
        memcpy(out, " i32", 4);
        out += 4;
        memcpy(out, futhark_shape_i32_1d(ctx, obj->v0), 1 * sizeof(int64_t));
        out += 1 * sizeof(int64_t);
        ret |= futhark_values_i32_1d(ctx, obj->v0, (void *) out);
        out += futhark_shape_i32_1d(ctx, obj->v0)[0] * sizeof(int32_t);
        *out++ = 'b';
        *out++ = 2;
        *out++ = 1;
        memcpy(out, "  u8", 4);
        out += 4;
        memcpy(out, futhark_shape_u8_1d(ctx, obj->v1), 1 * sizeof(int64_t));
        out += 1 * sizeof(int64_t);
        ret |= futhark_values_u8_1d(ctx, obj->v1, (void *) out);
        out += futhark_shape_u8_1d(ctx, obj->v1)[0] * sizeof(int8_t);
    }
    return ret;
}
struct futhark_opaque_sortStruct_int *futhark_restore_opaque_sortStruct_int(struct futhark_context *ctx, const void *p)
{
    (void) ctx;
    
    int err = 0;
    const unsigned char *src = p;
    struct futhark_opaque_sortStruct_int *obj = malloc(sizeof(struct futhark_opaque_sortStruct_int));
    int64_t shape_0[1] = {0};
    
    err |= *src++ != 'b';
    err |= *src++ != 2;
    err |= *src++ != 1;
    err |= memcmp(src, " i32", 4) != 0;
    src += 4;
    if (err == 0) {
        memcpy(shape_0, src, 1 * sizeof(int64_t));
        src += 1 * sizeof(int64_t);
    }
    
    const void *data_0 = src;
    
    obj->v0 = NULL;
    src += shape_0[0] * sizeof(int32_t);
    
    int64_t shape_1[1] = {0};
    
    err |= *src++ != 'b';
    err |= *src++ != 2;
    err |= *src++ != 1;
    err |= memcmp(src, "  u8", 4) != 0;
    src += 4;
    if (err == 0) {
        memcpy(shape_1, src, 1 * sizeof(int64_t));
        src += 1 * sizeof(int64_t);
    }
    
    const void *data_1 = src;
    
    obj->v1 = NULL;
    src += shape_1[0] * sizeof(int8_t);
    if (err == 0) {
        obj->v0 = futhark_new_i32_1d(ctx, data_0, shape_0[0]);
        if (obj->v0 == NULL)
            err = 1;
        obj->v1 = futhark_new_u8_1d(ctx, data_1, shape_1[0]);
        if (obj->v1 == NULL)
            err = 1;
    }
    if (err != 0) {
        int ret = 0, tmp;
        
        if (obj->v0 != NULL && (tmp = futhark_free_i32_1d(ctx, obj->v0)) != 0)
            ret = tmp;
        if (obj->v1 != NULL && (tmp = futhark_free_u8_1d(ctx, obj->v1)) != 0)
            ret = tmp;
        free(obj);
        obj = NULL;
    }
    return obj;
}

FUTHARK_FUN_ATTR int futrts_entry_mergeSortRelation_int(struct futhark_context *ctx, struct memblock *mem_out_p_24325, struct memblock *mem_out_p_24326, struct memblock k_mem_23976, struct memblock pL_mem_23977, int64_t n_17305, int64_t dzlz7bUZLztZRz20Unz20Upayload_bytesz7dUzg_17306, int64_t payload_bytes_17307)
{
    (void) ctx;
    
    int err = 0;
    int64_t mem_23982_cached_sizze_24327 = 0;
    unsigned char *mem_23982 = NULL;
    int64_t mem_23985_cached_sizze_24328 = 0;
    unsigned char *mem_23985 = NULL;
    int64_t mem_23995_cached_sizze_24329 = 0;
    unsigned char *mem_23995 = NULL;
    int64_t mem_23997_cached_sizze_24330 = 0;
    unsigned char *mem_23997 = NULL;
    int64_t mem_24064_cached_sizze_24331 = 0;
    unsigned char *mem_24064 = NULL;
    int64_t mem_24067_cached_sizze_24332 = 0;
    unsigned char *mem_24067 = NULL;
    int64_t mem_24077_cached_sizze_24333 = 0;
    unsigned char *mem_24077 = NULL;
    int64_t mem_24079_cached_sizze_24334 = 0;
    unsigned char *mem_24079 = NULL;
    int64_t mem_24094_cached_sizze_24335 = 0;
    unsigned char *mem_24094 = NULL;
    int64_t mem_24097_cached_sizze_24336 = 0;
    unsigned char *mem_24097 = NULL;
    int64_t mem_24188_cached_sizze_24337 = 0;
    unsigned char *mem_24188 = NULL;
    struct memblock mem_24204;
    
    mem_24204.references = NULL;
    
    struct memblock mem_24186;
    
    mem_24186.references = NULL;
    
    struct memblock mem_param_tmp_24295;
    
    mem_param_tmp_24295.references = NULL;
    
    struct memblock mem_param_tmp_24294;
    
    mem_param_tmp_24294.references = NULL;
    
    struct memblock mem_24174;
    
    mem_24174.references = NULL;
    
    struct memblock mem_24170;
    
    mem_24170.references = NULL;
    
    struct memblock mem_24136;
    
    mem_24136.references = NULL;
    
    struct memblock mem_24132;
    
    mem_24132.references = NULL;
    
    struct memblock mem_24141;
    
    mem_24141.references = NULL;
    
    struct memblock mem_24139;
    
    mem_24139.references = NULL;
    
    struct memblock ext_mem_24156;
    
    ext_mem_24156.references = NULL;
    
    struct memblock ext_mem_24159;
    
    ext_mem_24159.references = NULL;
    
    struct memblock mem_param_24061;
    
    mem_param_24061.references = NULL;
    
    struct memblock mem_param_24058;
    
    mem_param_24058.references = NULL;
    
    struct memblock ext_mem_24180;
    
    ext_mem_24180.references = NULL;
    
    struct memblock ext_mem_24181;
    
    ext_mem_24181.references = NULL;
    
    struct memblock mem_24054;
    
    mem_24054.references = NULL;
    
    struct memblock mem_24050;
    
    mem_24050.references = NULL;
    
    struct memblock ext_mem_24184;
    
    ext_mem_24184.references = NULL;
    
    struct memblock mem_23979;
    
    mem_23979.references = NULL;
    
    struct memblock mem_out_24275;
    
    mem_out_24275.references = NULL;
    
    struct memblock mem_out_24274;
    
    mem_out_24274.references = NULL;
    
    int64_t arg_22991 = mul64(n_17305, payload_bytes_17307);
    bool assert_cond_22992 = arg_22991 == dzlz7bUZLztZRz20Unz20Upayload_bytesz7dUzg_17306;
    bool assert_c_22993;
    
    if (!assert_cond_22992) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "Assertion is false: entry point arguments have invalid sizes.", "-> #0  ftError.fut:40:28-42:32\n   #1  ftError.fut:38:1-42:32\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool binop_x_22995 = dzlz7bUZLztZRz20Unz20Upayload_bytesz7dUzg_17306 == arg_22991;
    bool binop_y_22997 = sle64((int64_t) 0, payload_bytes_17307);
    bool dim_ok_22999 = binop_x_22995 && binop_y_22997;
    bool dim_ok_cert_23000;
    
    if (!dim_ok_22999) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Cannot unflatten array of shape [", (long long) dzlz7bUZLztZRz20Unz20Upayload_bytesz7dUzg_17306, "] to array of shape [", (long long) n_17305, "][", (long long) payload_bytes_17307, "]", "-> #0  /prelude/array.fut:184:3-30\n   #1  ftError.fut:32:41-50\n   #2  /prelude/functional.fut:9:44-45\n   #3  ftError.fut:29:20-32:50\n   #4  ftError.fut:42:28-32\n   #5  ftError.fut:38:1-42:32\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t bytes_23978 = (int64_t) 8 * n_17305;
    bool cond_23016 = sle64(n_17305, (int64_t) 1);
    int64_t zs_lhs_23017 = sub64(n_17305, (int64_t) 1);
    int64_t zp_rhs_23018 = sdiv_safe64(zs_lhs_23017, (int64_t) 20);
    int64_t min_num_blocks_23019 = add64((int64_t) 1, zp_rhs_23018);
    bool loop_cond_23020 = slt64((int64_t) 1, min_num_blocks_23019);
    bool smallest_pow_2_geq_than_res_23021;
    int64_t smallest_pow_2_geq_than_res_23022;
    bool loop_while_23023;
    int64_t x_23024;
    
    loop_while_23023 = loop_cond_23020;
    x_23024 = (int64_t) 1;
    while (loop_while_23023) {
        int64_t loopres_23025 = mul64((int64_t) 2, x_23024);
        bool loop_cond_23026 = slt64(loopres_23025, min_num_blocks_23019);
        bool loop_while_tmp_24276 = loop_cond_23026;
        int64_t x_tmp_24277 = loopres_23025;
        
        loop_while_23023 = loop_while_tmp_24276;
        x_23024 = x_tmp_24277;
    }
    smallest_pow_2_geq_than_res_23021 = loop_while_23023;
    smallest_pow_2_geq_than_res_23022 = x_23024;
    
    bool zzero_23027 = smallest_pow_2_geq_than_res_23022 == (int64_t) 0;
    bool nonzzero_23028 = !zzero_23027;
    bool protect_assert_disj_23029 = cond_23016 || nonzzero_23028;
    bool nonzzero_cert_23030;
    
    if (!protect_assert_disj_23029) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:234:30-42\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #2  /prelude/functional.fut:9:44-45\n   #3  ftError.fut:34:58-61\n   #4  ftError.fut:42:28-32\n   #5  ftError.fut:38:1-42:32\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t zp_rhs_23031 = sdiv_safe64(zs_lhs_23017, smallest_pow_2_geq_than_res_23022);
    int64_t block_sizze_23032 = add64((int64_t) 1, zp_rhs_23031);
    bool x_24226 = !cond_23016;
    int64_t greatest_divisor_leq_than_arg1_23036 = mul64((int64_t) 2, block_sizze_23032);
    bool cond_23037 = slt64((int64_t) 8, greatest_divisor_leq_than_arg1_23036);
    int64_t d_23039;
    
    if (x_24226) {
        bool x_24228;
        int64_t x_24229;
        bool loop_while_23040;
        int64_t d_23041;
        
        loop_while_23040 = cond_23037;
        d_23041 = (int64_t) 1;
        while (loop_while_23040) {
            int64_t loopres_23042 = add64((int64_t) 1, d_23041);
            bool zzero_23043 = loopres_23042 == (int64_t) 0;
            bool nonzzero_23044 = !zzero_23043;
            bool nonzzero_cert_23045;
            
            if (!nonzzero_23044) {
                set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:190:36-38\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:238:5-65\n   #2  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #3  /prelude/functional.fut:9:44-45\n   #4  ftError.fut:34:58-61\n   #5  ftError.fut:42:28-32\n   #6  ftError.fut:38:1-42:32\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t zg_lhs_23046 = sdiv64(greatest_divisor_leq_than_arg1_23036, loopres_23042);
            bool cond_23047 = slt64((int64_t) 8, zg_lhs_23046);
            bool loop_cond_23048;
            
            if (cond_23047) {
                loop_cond_23048 = 1;
            } else {
                int64_t znze_lhs_23049 = smod64(greatest_divisor_leq_than_arg1_23036, loopres_23042);
                bool loop_cond_f_res_23050 = znze_lhs_23049 == (int64_t) 0;
                bool loop_cond_f_res_23051 = !loop_cond_f_res_23050;
                
                loop_cond_23048 = loop_cond_f_res_23051;
            }
            
            bool loop_while_tmp_24278 = loop_cond_23048;
            int64_t d_tmp_24279 = loopres_23042;
            
            loop_while_23040 = loop_while_tmp_24278;
            d_23041 = d_tmp_24279;
        }
        x_24228 = loop_while_23040;
        x_24229 = d_23041;
        d_23039 = x_24229;
    } else {
        d_23039 = (int64_t) 0;
    }
    
    bool zzero_23052 = d_23039 == (int64_t) 0;
    bool nonzzero_23053 = !zzero_23052;
    bool protect_assert_disj_24233 = cond_23016 || nonzzero_23053;
    bool nonzzero_cert_23054;
    
    if (!protect_assert_disj_24233) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:191:7-9\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:238:5-65\n   #2  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #3  /prelude/functional.fut:9:44-45\n   #4  ftError.fut:34:58-61\n   #5  ftError.fut:42:28-32\n   #6  ftError.fut:38:1-42:32\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool y_23056 = slt64((int64_t) 0, n_17305);
    bool protect_assert_disj_24235 = cond_23016 || y_23056;
    bool index_certs_23057;
    
    if (!protect_assert_disj_24235) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) n_17305, "].", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:240:60-64\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #2  /prelude/functional.fut:9:44-45\n   #3  ftError.fut:34:58-61\n   #4  ftError.fut:42:28-32\n   #5  ftError.fut:38:1-42:32\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool bounds_invalid_upwards_23071 = slt64(smallest_pow_2_geq_than_res_23022, (int64_t) 0);
    bool valid_23072 = !bounds_invalid_upwards_23071;
    bool protect_assert_disj_24243 = cond_23016 || valid_23072;
    bool range_valid_c_23073;
    
    if (!protect_assert_disj_24243) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) smallest_pow_2_geq_than_res_23022, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #3  /prelude/functional.fut:9:44-45\n   #4  ftError.fut:34:58-61\n   #5  ftError.fut:42:28-32\n   #6  ftError.fut:38:1-42:32\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool bounds_invalid_upwards_23075 = slt64(block_sizze_23032, (int64_t) 0);
    bool valid_23076 = !bounds_invalid_upwards_23075;
    bool protect_assert_disj_24245 = cond_23016 || valid_23076;
    bool range_valid_c_23077;
    
    if (!protect_assert_disj_24245) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) block_sizze_23032, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ftError.fut:34:58-61\n   #7  ftError.fut:42:28-32\n   #8  ftError.fut:38:1-42:32\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool cond_23079 = sle64(block_sizze_23032, (int64_t) 1);
    int64_t tmp_23080 = sub64(block_sizze_23032, (int64_t) 2);
    bool bounds_invalid_upwards_23081 = slt64(tmp_23080, (int64_t) 0);
    bool valid_23083 = !bounds_invalid_upwards_23081;
    bool protect_assert_disj_23084 = cond_23079 || valid_23083;
    bool protect_assert_disj_24247 = cond_23016 || protect_assert_disj_23084;
    bool range_valid_c_23085;
    
    if (!protect_assert_disj_24247) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "...", (long long) tmp_23080, " is invalid.", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:166:19-28\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ftError.fut:34:58-61\n   #7  ftError.fut:42:28-32\n   #8  ftError.fut:38:1-42:32\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t bytes_24185 = (int64_t) 4 * n_17305;
    bool empty_or_match_cert_23555;
    
    if (!binop_x_22995) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "Function return value does not match shape of declared return type.", "-> #0  unknown location\n   #1  ftError.fut:36:38-45\n   #2  ftError.fut:42:28-32\n   #3  ftError.fut:38:1-42:32\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    if (memblock_alloc(ctx, &mem_23979, bytes_23978, "mem_23979")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t i_24280 = 0; i_24280 < n_17305; i_24280++) {
        int64_t x_24281 = (int64_t) 0 + i_24280 * (int64_t) 1;
        
        ((int64_t *) mem_23979.mem)[i_24280] = x_24281;
    }
    
    int64_t flat_dim_23033 = smallest_pow_2_geq_than_res_23022 * block_sizze_23032;
    int64_t greatest_divisor_leq_than_res_23055 = sdiv_safe64(greatest_divisor_leq_than_arg1_23036, d_23039);
    int32_t reduce_arg1_23058;
    
    if (x_24226) {
        int32_t x_24236 = ((int32_t *) k_mem_23976.mem)[(int64_t) 0];
        
        reduce_arg1_23058 = x_24236;
    } else {
        reduce_arg1_23058 = 0;
    }
    
    int32_t defunc_0_reduce_res_23731;
    int64_t defunc_0_reduce_res_23732;
    
    if (x_24226) {
        int32_t x_24238;
        int64_t x_24239;
        int32_t redout_23813;
        int64_t redout_23814;
        
        redout_23813 = reduce_arg1_23058;
        redout_23814 = (int64_t) 0;
        for (int64_t i_23815 = 0; i_23815 < n_17305; i_23815++) {
            int32_t x_23062 = ((int32_t *) k_mem_23976.mem)[i_23815];
            bool defunc_0_leq_res_23068 = sle32(redout_23813, x_23062);
            int32_t defunc_0_op_res_23069;
            
            if (defunc_0_leq_res_23068) {
                defunc_0_op_res_23069 = x_23062;
            } else {
                defunc_0_op_res_23069 = redout_23813;
            }
            
            int64_t defunc_0_op_res_23070;
            
            if (defunc_0_leq_res_23068) {
                defunc_0_op_res_23070 = i_23815;
            } else {
                defunc_0_op_res_23070 = redout_23814;
            }
            
            int32_t redout_tmp_24282 = defunc_0_op_res_23069;
            int64_t redout_tmp_24283 = defunc_0_op_res_23070;
            
            redout_23813 = redout_tmp_24282;
            redout_23814 = redout_tmp_24283;
        }
        x_24238 = redout_23813;
        x_24239 = redout_23814;
        defunc_0_reduce_res_23731 = x_24238;
        defunc_0_reduce_res_23732 = x_24239;
    } else {
        defunc_0_reduce_res_23731 = 0;
        defunc_0_reduce_res_23732 = (int64_t) 0;
    }
    
    int64_t distance_23082 = add64((int64_t) 1, tmp_23080);
    int64_t binop_x_23980 = (int64_t) 4 * smallest_pow_2_geq_than_res_23022;
    int64_t bytes_23981 = block_sizze_23032 * binop_x_23980;
    int64_t binop_x_23983 = (int64_t) 8 * smallest_pow_2_geq_than_res_23022;
    int64_t bytes_23984 = block_sizze_23032 * binop_x_23983;
    int64_t bytes_23994 = (int64_t) 4 * block_sizze_23032;
    int64_t bytes_23996 = (int64_t) 8 * block_sizze_23032;
    bool loop_cond_23157 = slt64(block_sizze_23032, flat_dim_23033);
    bool zzero_23158 = greatest_divisor_leq_than_res_23055 == (int64_t) 0;
    bool nonzzero_23159 = !zzero_23158;
    
    if (cond_23016) {
        if (memblock_set(ctx, &ext_mem_24184, &mem_23979, "mem_23979") != 0)
            return 1;
    } else {
        if (mem_23982_cached_sizze_24327 < bytes_23981) {
            err = lexical_realloc(ctx, &mem_23982, &mem_23982_cached_sizze_24327, bytes_23981);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_23985_cached_sizze_24328 < bytes_23984) {
            err = lexical_realloc(ctx, &mem_23985, &mem_23985_cached_sizze_24328, bytes_23984);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_23995_cached_sizze_24329 < bytes_23994) {
            err = lexical_realloc(ctx, &mem_23995, &mem_23995_cached_sizze_24329, bytes_23994);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_23997_cached_sizze_24330 < bytes_23996) {
            err = lexical_realloc(ctx, &mem_23997, &mem_23997_cached_sizze_24330, bytes_23996);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t i_23827 = 0; i_23827 < smallest_pow_2_geq_than_res_23022; i_23827++) {
            int64_t zp_lhs_23089 = mul64(block_sizze_23032, i_23827);
            
            for (int64_t i_23820 = 0; i_23820 < block_sizze_23032; i_23820++) {
                int64_t k_23093 = add64(zp_lhs_23089, i_23820);
                bool cond_23094 = slt64(k_23093, n_17305);
                int32_t lifted_lambda_res_23095;
                int64_t lifted_lambda_res_23096;
                
                if (cond_23094) {
                    bool x_23584 = sle64((int64_t) 0, k_23093);
                    bool bounds_check_23585 = cond_23094 && x_23584;
                    bool index_certs_23586;
                    
                    if (!bounds_check_23585) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) k_23093, "] out of bounds for array of shape [", (long long) n_17305, "].", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:246:26-30\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ftError.fut:34:58-61\n   #8  ftError.fut:42:28-32\n   #9  ftError.fut:38:1-42:32\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int32_t lifted_lambda_res_t_res_23587 = ((int32_t *) k_mem_23976.mem)[k_23093];
                    
                    lifted_lambda_res_23095 = lifted_lambda_res_t_res_23587;
                    lifted_lambda_res_23096 = k_23093;
                } else {
                    lifted_lambda_res_23095 = defunc_0_reduce_res_23731;
                    lifted_lambda_res_23096 = defunc_0_reduce_res_23732;
                }
                ((int32_t *) mem_23995)[i_23820] = lifted_lambda_res_23095;
                ((int64_t *) mem_23997)[i_23820] = lifted_lambda_res_23096;
            }
            if (!cond_23079) {
                for (int64_t i_23106 = 0; i_23106 < distance_23082; i_23106++) {
                    int64_t gt_arg1_23109 = add64((int64_t) 1, i_23106);
                    bool x_23110 = sle64((int64_t) 0, gt_arg1_23109);
                    bool y_23111 = slt64(gt_arg1_23109, block_sizze_23032);
                    bool bounds_check_23112 = x_23110 && y_23111;
                    bool index_certs_23113;
                    
                    if (!bounds_check_23112) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) gt_arg1_23109, "] out of bounds for array of shape [", (long long) block_sizze_23032, "].", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:169:31-37\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ftError.fut:34:58-61\n   #7  ftError.fut:42:28-32\n   #8  ftError.fut:38:1-42:32\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    bool y_23115 = slt64(i_23106, block_sizze_23032);
                    bool index_certs_23116;
                    
                    if (!y_23115) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_23106, "] out of bounds for array of shape [", (long long) block_sizze_23032, "].", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:169:26-30\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ftError.fut:34:58-61\n   #7  ftError.fut:42:28-32\n   #8  ftError.fut:38:1-42:32\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int32_t gt_arg1_23114 = ((int32_t *) mem_23995)[gt_arg1_23109];
                    int32_t gt_arg0_23117 = ((int32_t *) mem_23995)[i_23106];
                    bool defunc_0_leq_res_23118 = sle32(gt_arg0_23117, gt_arg1_23114);
                    bool defunc_0_lifted_gt_res_23119 = !defunc_0_leq_res_23118;
                    bool s_23120;
                    int64_t s_23123;
                    bool loop_while_23124;
                    int64_t j_23127;
                    
                    loop_while_23124 = defunc_0_lifted_gt_res_23119;
                    j_23127 = i_23106;
                    while (loop_while_23124) {
                        bool x_23128 = sle64((int64_t) 0, j_23127);
                        bool y_23129 = slt64(j_23127, block_sizze_23032);
                        bool bounds_check_23130 = x_23128 && y_23129;
                        bool index_certs_23131;
                        
                        if (!bounds_check_23130) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_23127, "] out of bounds for array of shape [", (long long) block_sizze_23032, "].", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:170:24-28\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ftError.fut:34:58-61\n   #7  ftError.fut:42:28-32\n   #8  ftError.fut:38:1-42:32\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t copy_arg0_23134 = add64((int64_t) 1, j_23127);
                        bool x_23135 = sle64((int64_t) 0, copy_arg0_23134);
                        bool y_23136 = slt64(copy_arg0_23134, block_sizze_23032);
                        bool bounds_check_23137 = x_23135 && y_23136;
                        bool index_certs_23138;
                        
                        if (!bounds_check_23137) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) copy_arg0_23134, "] out of bounds for array of shape [", (long long) block_sizze_23032, "].", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:171:35-41\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ftError.fut:34:58-61\n   #7  ftError.fut:42:28-32\n   #8  ftError.fut:38:1-42:32\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int32_t copy_arg0_23132 = ((int32_t *) mem_23995)[j_23127];
                        int64_t copy_arg0_23133 = ((int64_t *) mem_23997)[j_23127];
                        int32_t copy_arg0_23139 = ((int32_t *) mem_23995)[copy_arg0_23134];
                        int64_t copy_arg0_23140 = ((int64_t *) mem_23997)[copy_arg0_23134];
                        
                        ((int32_t *) mem_23995)[j_23127] = copy_arg0_23139;
                        ((int64_t *) mem_23997)[j_23127] = copy_arg0_23140;
                        ((int32_t *) mem_23995)[copy_arg0_23134] = copy_arg0_23132;
                        ((int64_t *) mem_23997)[copy_arg0_23134] = copy_arg0_23133;
                        
                        int64_t tmp_23145 = sub64(j_23127, (int64_t) 1);
                        bool cond_23146 = sle64((int64_t) 0, tmp_23145);
                        bool loop_cond_23147;
                        
                        if (cond_23146) {
                            bool y_23589 = slt64(tmp_23145, block_sizze_23032);
                            bool bounds_check_23590 = cond_23146 && y_23589;
                            bool index_certs_23591;
                            
                            if (!bounds_check_23590) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_23145, "] out of bounds for array of shape [", (long long) block_sizze_23032, "].", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:169:26-30\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:248:32-37\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ftError.fut:34:58-61\n   #7  ftError.fut:42:28-32\n   #8  ftError.fut:38:1-42:32\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int32_t gt_arg1_23588 = ((int32_t *) mem_23995)[j_23127];
                            int32_t gt_arg0_23592 = ((int32_t *) mem_23995)[tmp_23145];
                            bool defunc_0_leq_res_23593 = sle32(gt_arg0_23592, gt_arg1_23588);
                            bool defunc_0_lifted_gt_res_23594 = !defunc_0_leq_res_23593;
                            
                            loop_cond_23147 = defunc_0_lifted_gt_res_23594;
                        } else {
                            loop_cond_23147 = 0;
                        }
                        
                        bool loop_while_tmp_24290 = loop_cond_23147;
                        int64_t j_tmp_24293 = tmp_23145;
                        
                        loop_while_23124 = loop_while_tmp_24290;
                        j_23127 = j_tmp_24293;
                    }
                    s_23120 = loop_while_23124;
                    s_23123 = j_23127;
                }
            }
            lmad_copy_4b(ctx, 1, (uint32_t *) mem_23982, i_23827 * block_sizze_23032, (int64_t []) {(int64_t) 1}, (uint32_t *) mem_23995, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {block_sizze_23032});
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_23985, i_23827 * block_sizze_23032, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_23997, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {block_sizze_23032});
        }
        if (memblock_alloc(ctx, &mem_24050, bytes_23981, "mem_24050")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_4b(ctx, 2, (uint32_t *) mem_24050.mem, (int64_t) 0, (int64_t []) {block_sizze_23032, (int64_t) 1}, (uint32_t *) mem_23982, (int64_t) 0, (int64_t []) {block_sizze_23032, (int64_t) 1}, (int64_t []) {smallest_pow_2_geq_than_res_23022, block_sizze_23032});
        if (memblock_alloc(ctx, &mem_24054, bytes_23984, "mem_24054")) {
            err = 1;
            goto cleanup;
        }
        lmad_copy_8b(ctx, 2, (uint64_t *) mem_24054.mem, (int64_t) 0, (int64_t []) {block_sizze_23032, (int64_t) 1}, (uint64_t *) mem_23985, (int64_t) 0, (int64_t []) {block_sizze_23032, (int64_t) 1}, (int64_t []) {smallest_pow_2_geq_than_res_23022, block_sizze_23032});
        
        int64_t data_23160;
        bool data_23161;
        int64_t data_23164;
        int64_t loop_dz2084Uz2082U_23165;
        bool loop_while_23166;
        int64_t stride_23169;
        
        if (memblock_set(ctx, &mem_param_24058, &mem_24050, "mem_24050") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_24061, &mem_24054, "mem_24054") != 0)
            return 1;
        loop_dz2084Uz2082U_23165 = flat_dim_23033;
        loop_while_23166 = loop_cond_23157;
        stride_23169 = block_sizze_23032;
        while (loop_while_23166) {
            int64_t next_stride_23170 = mul64((int64_t) 2, stride_23169);
            bool zzero_23171 = next_stride_23170 == (int64_t) 0;
            bool nonzzero_23172 = !zzero_23171;
            bool nonzzero_cert_23173;
            
            if (!nonzzero_23172) {
                set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:254:33-46\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #2  /prelude/functional.fut:9:44-45\n   #3  ftError.fut:34:58-61\n   #4  ftError.fut:42:28-32\n   #5  ftError.fut:38:1-42:32\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t num_merges_23174 = sdiv64(flat_dim_23033, next_stride_23170);
            bool bounds_invalid_upwards_23175 = slt64(num_merges_23174, (int64_t) 0);
            bool valid_23176 = !bounds_invalid_upwards_23175;
            bool range_valid_c_23177;
            
            if (!valid_23176) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) num_merges_23174, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #3  /prelude/functional.fut:9:44-45\n   #4  ftError.fut:34:58-61\n   #5  ftError.fut:42:28-32\n   #6  ftError.fut:38:1-42:32\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t m_23179 = sub64(next_stride_23170, (int64_t) 1);
            bool zzero_leq_i_p_m_t_s_23180 = sle64((int64_t) 0, m_23179);
            bool cond_23182 = slt64(next_stride_23170, (int64_t) 10);
            bool protect_assert_disj_23183 = nonzzero_23159 || cond_23182;
            bool nonzzero_cert_23184;
            
            if (!protect_assert_disj_23183) {
                set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:147:30-42\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ftError.fut:34:58-61\n   #7  ftError.fut:42:28-32\n   #8  ftError.fut:38:1-42:32\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t zeze_lhs_23185 = smod_safe64(next_stride_23170, greatest_divisor_leq_than_res_23055);
            bool num_blocks_23186 = zeze_lhs_23185 == (int64_t) 0;
            bool protect_assert_disj_23187 = cond_23182 || num_blocks_23186;
            bool assert_c_23188;
            
            if (!protect_assert_disj_23187) {
                set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "Assertion is false: (n % block_size == 0)", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:147:20-65\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ftError.fut:34:58-61\n   #7  ftError.fut:42:28-32\n   #8  ftError.fut:38:1-42:32\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t num_blocks_23190 = sdiv_safe64(next_stride_23170, greatest_divisor_leq_than_res_23055);
            int64_t dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_23191 = add64((int64_t) 1, num_blocks_23190);
            int64_t j_m_i_23192 = sub64(next_stride_23170, stride_23169);
            int64_t flat_dim_23193 = greatest_divisor_leq_than_res_23055 * num_blocks_23190;
            bool bounds_invalid_upwards_23194 = slt64(dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_23191, (int64_t) 0);
            bool empty_slice_23195 = j_m_i_23192 == (int64_t) 0;
            int64_t m_23196 = sub64(j_m_i_23192, (int64_t) 1);
            bool zzero_lte_i_23197 = sle64((int64_t) 0, stride_23169);
            bool i_lte_j_23198 = sle64(stride_23169, next_stride_23170);
            bool empty_slice_23199 = stride_23169 == (int64_t) 0;
            int64_t m_23200 = sub64(stride_23169, (int64_t) 1);
            bool bounds_invalid_upwards_23201 = slt64(num_blocks_23190, (int64_t) 0);
            bool cond_23202 = slt64((int64_t) 0, stride_23169);
            bool valid_23203 = !bounds_invalid_upwards_23194;
            int64_t i_p_m_t_s_23204 = add64(stride_23169, m_23196);
            bool zzero_leq_i_p_m_t_s_23205 = sle64((int64_t) 0, m_23200);
            bool i_p_m_t_s_leq_w_23206 = slt64(m_23200, next_stride_23170);
            bool valid_23207 = !bounds_invalid_upwards_23201;
            bool i_p_m_t_s_leq_w_23208 = slt64(m_23179, flat_dim_23193);
            bool y_23209 = slt64((int64_t) 0, j_m_i_23192);
            bool protect_assert_disj_23210 = cond_23182 || valid_23203;
            bool range_valid_c_23211;
            
            if (!protect_assert_disj_23210) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_23191, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #3  lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  ftError.fut:34:58-61\n   #9  ftError.fut:42:28-32\n   #10 ftError.fut:38:1-42:32\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool zzero_leq_i_p_m_t_s_23212 = sle64((int64_t) 0, i_p_m_t_s_23204);
            bool i_p_m_t_s_leq_w_23213 = slt64(i_p_m_t_s_23204, next_stride_23170);
            bool y_23214 = zzero_leq_i_p_m_t_s_23205 && i_p_m_t_s_leq_w_23206;
            bool protect_assert_disj_23215 = cond_23182 || valid_23207;
            bool range_valid_c_23216;
            
            if (!protect_assert_disj_23215) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..", (long long) (int64_t) 1, "..<", (long long) num_blocks_23190, " is invalid.", "-> #0  /prelude/array.fut:94:3-11\n   #1  /prelude/array.fut:241:11-17\n   #2  lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ftError.fut:34:58-61\n   #8  ftError.fut:42:28-32\n   #9  ftError.fut:38:1-42:32\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool y_23217 = zzero_leq_i_p_m_t_s_23180 && i_p_m_t_s_leq_w_23208;
            bool y_23218 = zzero_lte_i_23197 && i_p_m_t_s_leq_w_23213;
            bool forwards_ok_23219 = zzero_lte_i_23197 && y_23214;
            bool ok_or_empty_23220 = zzero_23171 || y_23217;
            bool protect_assert_disj_23221 = cond_23202 || y_23209;
            bool y_23222 = zzero_leq_i_p_m_t_s_23212 && y_23218;
            bool ok_or_empty_23223 = empty_slice_23199 || forwards_ok_23219;
            bool protect_assert_disj_23224 = cond_23182 || ok_or_empty_23220;
            bool index_certs_23225;
            
            if (!protect_assert_disj_23224) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) next_stride_23170, "] out of bounds for array of shape [", (long long) flat_dim_23193, "].", "-> #0  /prelude/array.fut:46:45-51\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:155:3-46\n   #2  lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ftError.fut:34:58-61\n   #8  ftError.fut:42:28-32\n   #9  ftError.fut:38:1-42:32\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool loop_not_taken_23226 = !cond_23182;
            bool protect_assert_disj_23227 = protect_assert_disj_23221 || loop_not_taken_23226;
            bool index_certs_23228;
            
            if (!protect_assert_disj_23227) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) j_m_i_23192, "].", "-> #0  /prelude/array.fut:22:29-33\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:100:48-54\n   #2  lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #3  lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  ftError.fut:34:58-61\n   #9  ftError.fut:42:28-32\n   #10 ftError.fut:38:1-42:32\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool forwards_ok_23229 = i_lte_j_23198 && y_23222;
            bool protect_assert_disj_23230 = cond_23182 || ok_or_empty_23223;
            bool index_certs_23231;
            
            if (!protect_assert_disj_23230) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) stride_23169, "] out of bounds for array of shape [", (long long) next_stride_23170, "].", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:149:52-59\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #4  lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  ftError.fut:34:58-61\n   #10 ftError.fut:42:28-32\n   #11 ftError.fut:38:1-42:32\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool protect_assert_disj_23232 = ok_or_empty_23223 || loop_not_taken_23226;
            bool index_certs_23233;
            
            if (!protect_assert_disj_23232) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) stride_23169, "] out of bounds for array of shape [", (long long) next_stride_23170, "].", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:145:26-33\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ftError.fut:34:58-61\n   #7  ftError.fut:42:28-32\n   #8  ftError.fut:38:1-42:32\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool ok_or_empty_23234 = empty_slice_23195 || forwards_ok_23229;
            bool protect_assert_disj_23235 = cond_23182 || ok_or_empty_23234;
            bool index_certs_23236;
            
            if (!protect_assert_disj_23235) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) stride_23169, ":] out of bounds for array of shape [", (long long) next_stride_23170, "].", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:149:60-67\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #4  lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  ftError.fut:34:58-61\n   #10 ftError.fut:42:28-32\n   #11 ftError.fut:38:1-42:32\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool protect_assert_disj_23237 = loop_not_taken_23226 || ok_or_empty_23234;
            bool index_certs_23238;
            
            if (!protect_assert_disj_23237) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) stride_23169, ":] out of bounds for array of shape [", (long long) next_stride_23170, "].", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:145:34-41\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ftError.fut:34:58-61\n   #7  ftError.fut:42:28-32\n   #8  ftError.fut:38:1-42:32\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t binop_x_24062 = (int64_t) 4 * num_merges_23174;
            int64_t bytes_24063 = next_stride_23170 * binop_x_24062;
            int64_t binop_x_24065 = (int64_t) 8 * num_merges_23174;
            int64_t bytes_24066 = next_stride_23170 * binop_x_24065;
            bool i_lte_j_23181 = sle64((int64_t) 0, next_stride_23170);
            
            if (mem_24064_cached_sizze_24331 < bytes_24063) {
                err = lexical_realloc(ctx, &mem_24064, &mem_24064_cached_sizze_24331, bytes_24063);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            if (mem_24067_cached_sizze_24332 < bytes_24066) {
                err = lexical_realloc(ctx, &mem_24067, &mem_24067_cached_sizze_24332, bytes_24066);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            
            int64_t bytes_24076 = (int64_t) 8 * dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_23191;
            int64_t binop_x_24092 = (int64_t) 4 * num_blocks_23190;
            int64_t bytes_24093 = greatest_divisor_leq_than_res_23055 * binop_x_24092;
            int64_t binop_x_24095 = (int64_t) 8 * num_blocks_23190;
            int64_t bytes_24096 = greatest_divisor_leq_than_res_23055 * binop_x_24095;
            int64_t bytes_24138 = (int64_t) 8 * stride_23169;
            int64_t bytes_24140 = (int64_t) 16 * stride_23169;
            
            for (int64_t i_23848 = 0; i_23848 < num_merges_23174; i_23848++) {
                int64_t start_23242 = mul64(next_stride_23170, i_23848);
                int64_t j_m_i_23243 = sub64(loop_dz2084Uz2082U_23165, start_23242);
                bool empty_slice_23244 = j_m_i_23243 == (int64_t) 0;
                int64_t m_23245 = sub64(j_m_i_23243, (int64_t) 1);
                int64_t i_p_m_t_s_23246 = add64(start_23242, m_23245);
                bool zzero_leq_i_p_m_t_s_23247 = sle64((int64_t) 0, i_p_m_t_s_23246);
                bool i_p_m_t_s_leq_w_23248 = slt64(i_p_m_t_s_23246, loop_dz2084Uz2082U_23165);
                bool zzero_lte_i_23249 = sle64((int64_t) 0, start_23242);
                bool i_lte_j_23250 = sle64(start_23242, loop_dz2084Uz2082U_23165);
                bool y_23251 = i_p_m_t_s_leq_w_23248 && zzero_lte_i_23249;
                bool y_23252 = zzero_leq_i_p_m_t_s_23247 && y_23251;
                bool forwards_ok_23253 = i_lte_j_23250 && y_23252;
                bool ok_or_empty_23254 = empty_slice_23244 || forwards_ok_23253;
                bool index_certs_23255;
                
                if (!ok_or_empty_23254) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) start_23242, ":] out of bounds for array of shape [", (long long) loop_dz2084Uz2082U_23165, "].", "-> #0  /prelude/array.fut:52:49-54\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:258:41-56\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ftError.fut:34:58-61\n   #7  ftError.fut:42:28-32\n   #8  ftError.fut:38:1-42:32\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                bool i_p_m_t_s_leq_w_23256 = slt64(m_23179, j_m_i_23243);
                bool y_23257 = zzero_leq_i_p_m_t_s_23180 && i_p_m_t_s_leq_w_23256;
                bool forwards_ok_23258 = i_lte_j_23181 && y_23257;
                bool ok_or_empty_23259 = zzero_23171 || forwards_ok_23258;
                bool index_certs_23260;
                
                if (!ok_or_empty_23259) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) next_stride_23170, "] out of bounds for array of shape [", (long long) j_m_i_23243, "].", "-> #0  /prelude/array.fut:46:45-51\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:258:23-56\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #5  /prelude/functional.fut:9:44-45\n   #6  ftError.fut:34:58-61\n   #7  ftError.fut:42:28-32\n   #8  ftError.fut:38:1-42:32\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                if (cond_23182) {
                    int32_t dummy_23620;
                    int64_t dummy_23621;
                    
                    if (cond_23202) {
                        int32_t head_res_23622 = ((int32_t *) mem_param_24058.mem)[start_23242];
                        int64_t head_res_23623 = ((int64_t *) mem_param_24061.mem)[start_23242];
                        
                        dummy_23620 = head_res_23622;
                        dummy_23621 = head_res_23623;
                    } else {
                        int64_t slice_23624 = stride_23169 + start_23242;
                        int32_t head_res_23625 = ((int32_t *) mem_param_24058.mem)[slice_23624];
                        int64_t head_res_23626 = ((int64_t *) mem_param_24061.mem)[slice_23624];
                        
                        dummy_23620 = head_res_23625;
                        dummy_23621 = head_res_23626;
                    }
                    if (memblock_alloc(ctx, &mem_24139, bytes_24138, "mem_24139")) {
                        err = 1;
                        goto cleanup;
                    }
                    for (int64_t nest_i_24303 = 0; nest_i_24303 < next_stride_23170; nest_i_24303++) {
                        ((int32_t *) mem_24139.mem)[nest_i_24303] = dummy_23620;
                    }
                    if (memblock_alloc(ctx, &mem_24141, bytes_24140, "mem_24141")) {
                        err = 1;
                        goto cleanup;
                    }
                    for (int64_t nest_i_24304 = 0; nest_i_24304 < next_stride_23170; nest_i_24304++) {
                        ((int64_t *) mem_24141.mem)[nest_i_24304] = dummy_23621;
                    }
                    
                    int64_t data_23629;
                    int64_t i_23633 = (int64_t) 0;
                    
                    for (int64_t k_23632 = 0; k_23632 < next_stride_23170; k_23632++) {
                        int64_t j_23636 = sub64(k_23632, i_23633);
                        bool cond_23637 = j_23636 == j_m_i_23192;
                        bool cond_23638;
                        
                        if (cond_23637) {
                            cond_23638 = 1;
                        } else {
                            bool cond_23639 = slt64(i_23633, stride_23169);
                            bool cond_f_res_23640;
                            
                            if (cond_23639) {
                                bool x_23641 = sle64((int64_t) 0, j_23636);
                                bool y_23642 = slt64(j_23636, j_m_i_23192);
                                bool bounds_check_23643 = x_23641 && y_23642;
                                bool index_certs_23644;
                                
                                if (!bounds_check_23643) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_23636, "] out of bounds for array of shape [", (long long) j_m_i_23192, "].", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:105:55-59\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ftError.fut:34:58-61\n   #8  ftError.fut:42:28-32\n   #9  ftError.fut:38:1-42:32\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                bool x_23648 = sle64((int64_t) 0, i_23633);
                                bool bounds_check_23649 = cond_23639 && x_23648;
                                bool index_certs_23650;
                                
                                if (!bounds_check_23649) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_23633, "] out of bounds for array of shape [", (long long) stride_23169, "].", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:105:50-54\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ftError.fut:34:58-61\n   #8  ftError.fut:42:28-32\n   #9  ftError.fut:38:1-42:32\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t slice_23645 = stride_23169 + j_23636;
                                int64_t slice_23646 = start_23242 + slice_23645;
                                int32_t leq_arg1_23647 = ((int32_t *) mem_param_24058.mem)[slice_23646];
                                int64_t slice_23651 = start_23242 + i_23633;
                                int32_t leq_arg0_23652 = ((int32_t *) mem_param_24058.mem)[slice_23651];
                                bool defunc_0_leq_res_23653 = sle32(leq_arg0_23652, leq_arg1_23647);
                                
                                cond_f_res_23640 = defunc_0_leq_res_23653;
                            } else {
                                cond_f_res_23640 = 0;
                            }
                            cond_23638 = cond_f_res_23640;
                        }
                        
                        int64_t loopres_23654;
                        int32_t loopres_23655;
                        int64_t loopres_23656;
                        
                        if (cond_23638) {
                            bool x_23658 = sle64((int64_t) 0, i_23633);
                            bool y_23659 = slt64(i_23633, stride_23169);
                            bool bounds_check_23660 = x_23658 && y_23659;
                            bool index_certs_23661;
                            
                            if (!bounds_check_23660) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_23633, "] out of bounds for array of shape [", (long long) stride_23169, "].", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:106:17-21\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ftError.fut:34:58-61\n   #8  ftError.fut:42:28-32\n   #9  ftError.fut:38:1-42:32\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t tmp_23657 = add64((int64_t) 1, i_23633);
                            int64_t slice_23662 = start_23242 + i_23633;
                            int32_t tmp_23663 = ((int32_t *) mem_param_24058.mem)[slice_23662];
                            int64_t tmp_23664 = ((int64_t *) mem_param_24061.mem)[slice_23662];
                            
                            loopres_23654 = tmp_23657;
                            loopres_23655 = tmp_23663;
                            loopres_23656 = tmp_23664;
                        } else {
                            bool x_23665 = sle64((int64_t) 0, j_23636);
                            bool y_23666 = slt64(j_23636, j_m_i_23192);
                            bool bounds_check_23667 = x_23665 && y_23666;
                            bool index_certs_23668;
                            
                            if (!bounds_check_23667) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_23636, "] out of bounds for array of shape [", (long long) j_m_i_23192, "].", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:108:15-19\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:145:42-43\n   #2  lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #6  /prelude/functional.fut:9:44-45\n   #7  ftError.fut:34:58-61\n   #8  ftError.fut:42:28-32\n   #9  ftError.fut:38:1-42:32\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t slice_23669 = stride_23169 + j_23636;
                            int64_t slice_23670 = start_23242 + slice_23669;
                            int32_t tmp_23671 = ((int32_t *) mem_param_24058.mem)[slice_23670];
                            int64_t tmp_23672 = ((int64_t *) mem_param_24061.mem)[slice_23670];
                            
                            loopres_23654 = i_23633;
                            loopres_23655 = tmp_23671;
                            loopres_23656 = tmp_23672;
                        }
                        ((int32_t *) mem_24139.mem)[k_23632] = loopres_23655;
                        ((int64_t *) mem_24141.mem)[k_23632] = loopres_23656;
                        
                        int64_t i_tmp_24305 = loopres_23654;
                        
                        i_23633 = i_tmp_24305;
                    }
                    data_23629 = i_23633;
                    if (memblock_set(ctx, &ext_mem_24159, &mem_24139, "mem_24139") != 0)
                        return 1;
                    if (memblock_set(ctx, &ext_mem_24156, &mem_24141, "mem_24141") != 0)
                        return 1;
                } else {
                    if (mem_24077_cached_sizze_24333 < bytes_24076) {
                        err = lexical_realloc(ctx, &mem_24077, &mem_24077_cached_sizze_24333, bytes_24076);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    if (mem_24079_cached_sizze_24334 < bytes_24076) {
                        err = lexical_realloc(ctx, &mem_24079, &mem_24079_cached_sizze_24334, bytes_24076);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    for (int64_t i_23834 = 0; i_23834 < dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_23191; i_23834++) {
                        int64_t split_count_arg3_23322 = mul64(greatest_divisor_leq_than_res_23055, i_23834);
                        int64_t defunc_0_split_count_res_23323;
                        int64_t defunc_0_split_count_res_23324;
                        int64_t defunc_0_split_count_res_23325;
                        int64_t defunc_0_split_count_res_23326;
                        int64_t defunc_0_split_count_res_23327;
                        
                        if (futrts_lifted_normalizze_6809(ctx, &defunc_0_split_count_res_23323, &defunc_0_split_count_res_23324, &defunc_0_split_count_res_23325, &defunc_0_split_count_res_23326, &defunc_0_split_count_res_23327, (int64_t) 0, stride_23169, (int64_t) 0, j_m_i_23192, split_count_arg3_23322) != 0) {
                            err = 1;
                            goto cleanup;
                        }
                        
                        bool loop_cond_23328 = slt64((int64_t) 0, defunc_0_split_count_res_23327);
                        bool defunc_0_split_count_res_23329;
                        int64_t defunc_0_split_count_res_23330;
                        int64_t defunc_0_split_count_res_23331;
                        int64_t defunc_0_split_count_res_23332;
                        int64_t defunc_0_split_count_res_23333;
                        int64_t defunc_0_split_count_res_23334;
                        bool loop_while_23335;
                        int64_t ss_23336;
                        int64_t ss_23337;
                        int64_t tt_23338;
                        int64_t tt_23339;
                        int64_t count_23340;
                        
                        loop_while_23335 = loop_cond_23328;
                        ss_23336 = defunc_0_split_count_res_23323;
                        ss_23337 = defunc_0_split_count_res_23324;
                        tt_23338 = defunc_0_split_count_res_23325;
                        tt_23339 = defunc_0_split_count_res_23326;
                        count_23340 = defunc_0_split_count_res_23327;
                        while (loop_while_23335) {
                            int64_t zlze_lhs_23341 = sub64(ss_23337, ss_23336);
                            bool cond_23342 = sle64(zlze_lhs_23341, (int64_t) 0);
                            int64_t defunc_0_lifted_step_res_23343;
                            int64_t defunc_0_lifted_step_res_23344;
                            int64_t defunc_0_lifted_step_res_23345;
                            int64_t defunc_0_lifted_step_res_23346;
                            
                            if (cond_23342) {
                                int64_t tmp_23675 = add64(tt_23338, count_23340);
                                
                                defunc_0_lifted_step_res_23343 = ss_23336;
                                defunc_0_lifted_step_res_23344 = tmp_23675;
                                defunc_0_lifted_step_res_23345 = tt_23339;
                                defunc_0_lifted_step_res_23346 = (int64_t) 0;
                            } else {
                                int64_t zlze_lhs_23348 = sub64(tt_23339, tt_23338);
                                bool cond_23349 = sle64(zlze_lhs_23348, (int64_t) 0);
                                int64_t defunc_0_lifted_step_res_f_res_23350;
                                int64_t defunc_0_lifted_step_res_f_res_23351;
                                int64_t defunc_0_lifted_step_res_f_res_23352;
                                int64_t defunc_0_lifted_step_res_f_res_23353;
                                
                                if (cond_23349) {
                                    int64_t tmp_23676 = add64(ss_23336, count_23340);
                                    
                                    defunc_0_lifted_step_res_f_res_23350 = tmp_23676;
                                    defunc_0_lifted_step_res_f_res_23351 = tt_23338;
                                    defunc_0_lifted_step_res_f_res_23352 = tt_23339;
                                    defunc_0_lifted_step_res_f_res_23353 = (int64_t) 0;
                                } else {
                                    bool cond_23355 = count_23340 == (int64_t) 1;
                                    int64_t defunc_0_lifted_step_res_f_res_f_res_23356;
                                    int64_t defunc_0_lifted_step_res_f_res_f_res_23357;
                                    int64_t defunc_0_lifted_step_res_f_res_f_res_23358;
                                    int64_t defunc_0_lifted_step_res_f_res_f_res_23359;
                                    
                                    if (cond_23355) {
                                        bool x_23678 = sle64((int64_t) 0, ss_23336);
                                        bool y_23679 = slt64(ss_23336, stride_23169);
                                        bool bounds_check_23680 = x_23678 && y_23679;
                                        bool index_certs_23681;
                                        
                                        if (!bounds_check_23680) {
                                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) ss_23336, "] out of bounds for array of shape [", (long long) stride_23169, "].", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:53:21-27\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 ftError.fut:34:58-61\n   #12 ftError.fut:42:28-32\n   #13 ftError.fut:38:1-42:32\n"));
                                            err = FUTHARK_PROGRAM_ERROR;
                                            goto cleanup;
                                        }
                                        
                                        bool x_23684 = sle64((int64_t) 0, tt_23338);
                                        bool y_23685 = slt64(tt_23338, j_m_i_23192);
                                        bool bounds_check_23686 = x_23684 && y_23685;
                                        bool index_certs_23687;
                                        
                                        if (!bounds_check_23686) {
                                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tt_23338, "] out of bounds for array of shape [", (long long) j_m_i_23192, "].", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:53:14-20\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 ftError.fut:34:58-61\n   #12 ftError.fut:42:28-32\n   #13 ftError.fut:38:1-42:32\n"));
                                            err = FUTHARK_PROGRAM_ERROR;
                                            goto cleanup;
                                        }
                                        
                                        int64_t slice_23682 = start_23242 + ss_23336;
                                        int32_t leq_arg1_23683 = ((int32_t *) mem_param_24058.mem)[slice_23682];
                                        int64_t slice_23688 = stride_23169 + tt_23338;
                                        int64_t slice_23689 = start_23242 + slice_23688;
                                        int32_t leq_arg0_23690 = ((int32_t *) mem_param_24058.mem)[slice_23689];
                                        bool defunc_0_leq_res_23691 = sle32(leq_arg0_23690, leq_arg1_23683);
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_t_res_23692;
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_t_res_23693;
                                        
                                        if (defunc_0_leq_res_23691) {
                                            int64_t tmp_23694 = add64((int64_t) 1, tt_23338);
                                            
                                            defunc_0_lifted_step_res_f_res_f_res_t_res_23692 = ss_23336;
                                            defunc_0_lifted_step_res_f_res_f_res_t_res_23693 = tmp_23694;
                                        } else {
                                            int64_t tmp_23695 = add64((int64_t) 1, ss_23336);
                                            
                                            defunc_0_lifted_step_res_f_res_f_res_t_res_23692 = tmp_23695;
                                            defunc_0_lifted_step_res_f_res_f_res_t_res_23693 = tt_23338;
                                        }
                                        defunc_0_lifted_step_res_f_res_f_res_23356 = defunc_0_lifted_step_res_f_res_f_res_t_res_23692;
                                        defunc_0_lifted_step_res_f_res_f_res_23357 = defunc_0_lifted_step_res_f_res_f_res_t_res_23693;
                                        defunc_0_lifted_step_res_f_res_f_res_23358 = tt_23339;
                                        defunc_0_lifted_step_res_f_res_f_res_23359 = (int64_t) 0;
                                    } else {
                                        int64_t m_23378 = sdiv64(count_23340, (int64_t) 2);
                                        int64_t n_23379 = sub64(count_23340, m_23378);
                                        bool cond_23380 = n_23379 == (int64_t) 0;
                                        int64_t zgze_lhs_23381 = add64(ss_23336, m_23378);
                                        bool cond_f_res_23382 = sle64(ss_23337, zgze_lhs_23381);
                                        bool x_23383 = !cond_23380;
                                        bool y_23384 = cond_f_res_23382 && x_23383;
                                        bool cond_23385 = cond_23380 || y_23384;
                                        bool leq_y_x_23386;
                                        
                                        if (cond_23385) {
                                            leq_y_x_23386 = 1;
                                        } else {
                                            bool x_23387 = sle64((int64_t) 0, zgze_lhs_23381);
                                            bool y_23388 = slt64(zgze_lhs_23381, stride_23169);
                                            bool bounds_check_23389 = x_23387 && y_23388;
                                            bool index_certs_23390;
                                            
                                            if (!bounds_check_23389) {
                                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) zgze_lhs_23381, "] out of bounds for array of shape [", (long long) stride_23169, "].", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:73:22-30\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 ftError.fut:34:58-61\n   #12 ftError.fut:42:28-32\n   #13 ftError.fut:38:1-42:32\n"));
                                                err = FUTHARK_PROGRAM_ERROR;
                                                goto cleanup;
                                            }
                                            
                                            int64_t zm_lhs_23393 = add64(tt_23338, n_23379);
                                            int64_t leq_arg0_23394 = sub64(zm_lhs_23393, (int64_t) 1);
                                            bool x_23395 = sle64((int64_t) 0, leq_arg0_23394);
                                            bool y_23396 = slt64(leq_arg0_23394, j_m_i_23192);
                                            bool bounds_check_23397 = x_23395 && y_23396;
                                            bool index_certs_23398;
                                            
                                            if (!bounds_check_23397) {
                                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) leq_arg0_23394, "] out of bounds for array of shape [", (long long) j_m_i_23192, "].", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:73:11-21\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:83:23-38\n   #2  lib/github.com/diku-dk/sorts/merge_sort.fut:149:71-83\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  lib/github.com/diku-dk/sorts/merge_sort.fut:149:30-84\n   #6  lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #7  /prelude/soacs.fut:59:9-11\n   #8  /prelude/array.fut:241:11-17\n   #9  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #10 /prelude/functional.fut:9:44-45\n   #11 ftError.fut:34:58-61\n   #12 ftError.fut:42:28-32\n   #13 ftError.fut:38:1-42:32\n"));
                                                err = FUTHARK_PROGRAM_ERROR;
                                                goto cleanup;
                                            }
                                            
                                            int64_t slice_23391 = start_23242 + zgze_lhs_23381;
                                            int32_t leq_arg1_23392 = ((int32_t *) mem_param_24058.mem)[slice_23391];
                                            int64_t slice_23399 = stride_23169 + leq_arg0_23394;
                                            int64_t slice_23400 = start_23242 + slice_23399;
                                            int32_t leq_arg0_23401 = ((int32_t *) mem_param_24058.mem)[slice_23400];
                                            bool defunc_0_leq_res_23402 = sle32(leq_arg0_23401, leq_arg1_23392);
                                            
                                            leq_y_x_23386 = defunc_0_leq_res_23402;
                                        }
                                        
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_f_res_23403;
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_f_res_23404;
                                        int64_t defunc_0_lifted_step_res_f_res_f_res_f_res_23405;
                                        
                                        if (leq_y_x_23386) {
                                            int64_t tmp_23696 = add64(tt_23338, n_23379);
                                            int64_t tmp_23697 = sub64(count_23340, n_23379);
                                            
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_23403 = tmp_23696;
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_23404 = tt_23339;
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_23405 = tmp_23697;
                                        } else {
                                            int64_t tmp_23408 = add64(tt_23338, n_23379);
                                            
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_23403 = tt_23338;
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_23404 = tmp_23408;
                                            defunc_0_lifted_step_res_f_res_f_res_f_res_23405 = count_23340;
                                        }
                                        defunc_0_lifted_step_res_f_res_f_res_23356 = ss_23336;
                                        defunc_0_lifted_step_res_f_res_f_res_23357 = defunc_0_lifted_step_res_f_res_f_res_f_res_23403;
                                        defunc_0_lifted_step_res_f_res_f_res_23358 = defunc_0_lifted_step_res_f_res_f_res_f_res_23404;
                                        defunc_0_lifted_step_res_f_res_f_res_23359 = defunc_0_lifted_step_res_f_res_f_res_f_res_23405;
                                    }
                                    defunc_0_lifted_step_res_f_res_23350 = defunc_0_lifted_step_res_f_res_f_res_23356;
                                    defunc_0_lifted_step_res_f_res_23351 = defunc_0_lifted_step_res_f_res_f_res_23357;
                                    defunc_0_lifted_step_res_f_res_23352 = defunc_0_lifted_step_res_f_res_f_res_23358;
                                    defunc_0_lifted_step_res_f_res_23353 = defunc_0_lifted_step_res_f_res_f_res_23359;
                                }
                                defunc_0_lifted_step_res_23343 = defunc_0_lifted_step_res_f_res_23350;
                                defunc_0_lifted_step_res_23344 = defunc_0_lifted_step_res_f_res_23351;
                                defunc_0_lifted_step_res_23345 = defunc_0_lifted_step_res_f_res_23352;
                                defunc_0_lifted_step_res_23346 = defunc_0_lifted_step_res_f_res_23353;
                            }
                            
                            int64_t loopres_23409;
                            int64_t loopres_23410;
                            int64_t loopres_23411;
                            int64_t loopres_23412;
                            int64_t loopres_23413;
                            
                            if (futrts_lifted_normalizze_6809(ctx, &loopres_23409, &loopres_23410, &loopres_23411, &loopres_23412, &loopres_23413, defunc_0_lifted_step_res_23343, ss_23337, defunc_0_lifted_step_res_23344, defunc_0_lifted_step_res_23345, defunc_0_lifted_step_res_23346) != 0) {
                                err = 1;
                                goto cleanup;
                            }
                            
                            bool loop_cond_23414 = slt64((int64_t) 0, loopres_23413);
                            bool loop_while_tmp_24310 = loop_cond_23414;
                            int64_t ss_tmp_24311 = loopres_23409;
                            int64_t ss_tmp_24312 = loopres_23410;
                            int64_t tt_tmp_24313 = loopres_23411;
                            int64_t tt_tmp_24314 = loopres_23412;
                            int64_t count_tmp_24315 = loopres_23413;
                            
                            loop_while_23335 = loop_while_tmp_24310;
                            ss_23336 = ss_tmp_24311;
                            ss_23337 = ss_tmp_24312;
                            tt_23338 = tt_tmp_24313;
                            tt_23339 = tt_tmp_24314;
                            count_23340 = count_tmp_24315;
                        }
                        defunc_0_split_count_res_23329 = loop_while_23335;
                        defunc_0_split_count_res_23330 = ss_23336;
                        defunc_0_split_count_res_23331 = ss_23337;
                        defunc_0_split_count_res_23332 = tt_23338;
                        defunc_0_split_count_res_23333 = tt_23339;
                        defunc_0_split_count_res_23334 = count_23340;
                        ((int64_t *) mem_24077)[i_23834] = defunc_0_split_count_res_23330;
                        ((int64_t *) mem_24079)[i_23834] = defunc_0_split_count_res_23332;
                    }
                    if (mem_24094_cached_sizze_24335 < bytes_24093) {
                        err = lexical_realloc(ctx, &mem_24094, &mem_24094_cached_sizze_24335, bytes_24093);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    if (mem_24097_cached_sizze_24336 < bytes_24096) {
                        err = lexical_realloc(ctx, &mem_24097, &mem_24097_cached_sizze_24336, bytes_24096);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    for (int64_t i_23841 = 0; i_23841 < num_blocks_23190; i_23841++) {
                        bool y_23420 = slt64(i_23841, dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_23191);
                        bool index_certs_23422;
                        
                        if (!y_23420) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_23841, "] out of bounds for array of shape [", (long long) dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_23191, "].", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:151:22-34\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  ftError.fut:34:58-61\n   #9  ftError.fut:42:28-32\n   #10 ftError.fut:38:1-42:32\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t defunc_0_f_res_23423 = ((int64_t *) mem_24077)[i_23841];
                        int64_t defunc_0_f_res_23424 = ((int64_t *) mem_24079)[i_23841];
                        int64_t tmp_23425 = add64((int64_t) 1, i_23841);
                        bool x_23426 = sle64((int64_t) 0, tmp_23425);
                        bool y_23427 = slt64(tmp_23425, dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_23191);
                        bool bounds_check_23428 = x_23426 && y_23427;
                        bool index_certs_23429;
                        
                        if (!bounds_check_23428) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_23425, "] out of bounds for array of shape [", (long long) dzlz7bUZLzpZRz20U1z20Unum_blocksz7dUzg_23191, "].", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:152:22-36\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  ftError.fut:34:58-61\n   #9  ftError.fut:42:28-32\n   #10 ftError.fut:38:1-42:32\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t defunc_0_f_res_23430 = ((int64_t *) mem_24077)[tmp_23425];
                        int64_t defunc_0_f_res_23431 = ((int64_t *) mem_24079)[tmp_23425];
                        int64_t merge_sequential_arg2_23432 = add64(stride_23169, defunc_0_f_res_23424);
                        int64_t merge_sequential_arg2_23433 = add64(stride_23169, defunc_0_f_res_23431);
                        int64_t j_m_i_23434 = sub64(merge_sequential_arg2_23433, merge_sequential_arg2_23432);
                        bool empty_slice_23435 = j_m_i_23434 == (int64_t) 0;
                        int64_t m_23436 = sub64(j_m_i_23434, (int64_t) 1);
                        int64_t i_p_m_t_s_23437 = add64(merge_sequential_arg2_23432, m_23436);
                        bool zzero_leq_i_p_m_t_s_23438 = sle64((int64_t) 0, i_p_m_t_s_23437);
                        bool i_p_m_t_s_leq_w_23439 = slt64(i_p_m_t_s_23437, next_stride_23170);
                        bool zzero_lte_i_23440 = sle64((int64_t) 0, merge_sequential_arg2_23432);
                        bool i_lte_j_23441 = sle64(merge_sequential_arg2_23432, merge_sequential_arg2_23433);
                        bool y_23442 = i_p_m_t_s_leq_w_23439 && zzero_lte_i_23440;
                        bool y_23443 = zzero_leq_i_p_m_t_s_23438 && y_23442;
                        bool forwards_ok_23444 = i_lte_j_23441 && y_23443;
                        bool ok_or_empty_23445 = empty_slice_23435 || forwards_ok_23444;
                        bool index_certs_23446;
                        
                        if (!ok_or_empty_23445) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) merge_sequential_arg2_23432, ":", (long long) merge_sequential_arg2_23433, "] out of bounds for array of shape [", (long long) next_stride_23170, "].", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:153:40-58\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  ftError.fut:34:58-61\n   #9  ftError.fut:42:28-32\n   #10 ftError.fut:38:1-42:32\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int64_t j_m_i_23447 = sub64(defunc_0_f_res_23430, defunc_0_f_res_23423);
                        bool empty_slice_23448 = j_m_i_23447 == (int64_t) 0;
                        int64_t m_23449 = sub64(j_m_i_23447, (int64_t) 1);
                        int64_t i_p_m_t_s_23450 = add64(defunc_0_f_res_23423, m_23449);
                        bool zzero_leq_i_p_m_t_s_23451 = sle64((int64_t) 0, i_p_m_t_s_23450);
                        bool i_p_m_t_s_leq_w_23452 = slt64(i_p_m_t_s_23450, next_stride_23170);
                        bool zzero_lte_i_23453 = sle64((int64_t) 0, defunc_0_f_res_23423);
                        bool i_lte_j_23454 = sle64(defunc_0_f_res_23423, defunc_0_f_res_23430);
                        bool y_23455 = i_p_m_t_s_leq_w_23452 && zzero_lte_i_23453;
                        bool y_23456 = zzero_leq_i_p_m_t_s_23451 && y_23455;
                        bool forwards_ok_23457 = i_lte_j_23454 && y_23456;
                        bool ok_or_empty_23458 = empty_slice_23448 || forwards_ok_23457;
                        bool index_certs_23459;
                        
                        if (!ok_or_empty_23458) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) defunc_0_f_res_23423, ":", (long long) defunc_0_f_res_23430, "] out of bounds for array of shape [", (long long) next_stride_23170, "].", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:153:29-39\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #4  /prelude/soacs.fut:59:9-11\n   #5  /prelude/array.fut:241:11-17\n   #6  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #7  /prelude/functional.fut:9:44-45\n   #8  ftError.fut:34:58-61\n   #9  ftError.fut:42:28-32\n   #10 ftError.fut:38:1-42:32\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        bool cond_23460 = slt64((int64_t) 0, j_m_i_23447);
                        int32_t dummy_23461;
                        int64_t dummy_23462;
                        
                        if (cond_23460) {
                            bool index_certs_23698;
                            
                            if (!cond_23460) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) j_m_i_23447, "].", "-> #0  /prelude/array.fut:22:29-33\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:100:36-42\n   #2  lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #6  /prelude/soacs.fut:59:9-11\n   #7  /prelude/array.fut:241:11-17\n   #8  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #9  /prelude/functional.fut:9:44-45\n   #10 ftError.fut:34:58-61\n   #11 ftError.fut:42:28-32\n   #12 ftError.fut:38:1-42:32\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t slice_23699 = start_23242 + defunc_0_f_res_23423;
                            int32_t head_res_23700 = ((int32_t *) mem_param_24058.mem)[slice_23699];
                            int64_t head_res_23701 = ((int64_t *) mem_param_24061.mem)[slice_23699];
                            
                            dummy_23461 = head_res_23700;
                            dummy_23462 = head_res_23701;
                        } else {
                            bool y_23467 = slt64((int64_t) 0, j_m_i_23434);
                            bool index_certs_23468;
                            
                            if (!y_23467) {
                                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) j_m_i_23434, "].", "-> #0  /prelude/array.fut:22:29-33\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:100:48-54\n   #2  lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #3  /prelude/soacs.fut:59:9-11\n   #4  /prelude/array.fut:241:11-17\n   #5  lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #6  /prelude/soacs.fut:59:9-11\n   #7  /prelude/array.fut:241:11-17\n   #8  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #9  /prelude/functional.fut:9:44-45\n   #10 ftError.fut:34:58-61\n   #11 ftError.fut:42:28-32\n   #12 ftError.fut:38:1-42:32\n"));
                                err = FUTHARK_PROGRAM_ERROR;
                                goto cleanup;
                            }
                            
                            int64_t slice_23469 = start_23242 + merge_sequential_arg2_23432;
                            int32_t head_res_23470 = ((int32_t *) mem_param_24058.mem)[slice_23469];
                            int64_t head_res_23471 = ((int64_t *) mem_param_24061.mem)[slice_23469];
                            
                            dummy_23461 = head_res_23470;
                            dummy_23462 = head_res_23471;
                        }
                        for (int64_t nest_i_24318 = 0; nest_i_24318 < greatest_divisor_leq_than_res_23055; nest_i_24318++) {
                            ((int32_t *) mem_24094)[i_23841 * greatest_divisor_leq_than_res_23055 + nest_i_24318] = dummy_23461;
                        }
                        for (int64_t nest_i_24319 = 0; nest_i_24319 < greatest_divisor_leq_than_res_23055; nest_i_24319++) {
                            ((int64_t *) mem_24097)[i_23841 * greatest_divisor_leq_than_res_23055 + nest_i_24319] = dummy_23462;
                        }
                        
                        int64_t data_23474;
                        int64_t i_23478 = (int64_t) 0;
                        
                        for (int64_t k_23477 = 0; k_23477 < greatest_divisor_leq_than_res_23055; k_23477++) {
                            int64_t j_23481 = sub64(k_23477, i_23478);
                            bool cond_23482 = j_23481 == j_m_i_23434;
                            bool cond_23483;
                            
                            if (cond_23482) {
                                cond_23483 = 1;
                            } else {
                                bool cond_23484 = slt64(i_23478, j_m_i_23447);
                                bool cond_f_res_23485;
                                
                                if (cond_23484) {
                                    bool x_23702 = sle64((int64_t) 0, j_23481);
                                    bool y_23703 = slt64(j_23481, j_m_i_23434);
                                    bool bounds_check_23704 = x_23702 && y_23703;
                                    bool index_certs_23705;
                                    
                                    if (!bounds_check_23704) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_23481, "] out of bounds for array of shape [", (long long) j_m_i_23434, "].", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:105:55-59\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  ftError.fut:34:58-61\n   #10 ftError.fut:42:28-32\n   #11 ftError.fut:38:1-42:32\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    bool x_23709 = sle64((int64_t) 0, i_23478);
                                    bool bounds_check_23710 = cond_23484 && x_23709;
                                    bool index_certs_23711;
                                    
                                    if (!bounds_check_23710) {
                                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_23478, "] out of bounds for array of shape [", (long long) j_m_i_23447, "].", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:105:50-54\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  ftError.fut:34:58-61\n   #10 ftError.fut:42:28-32\n   #11 ftError.fut:38:1-42:32\n"));
                                        err = FUTHARK_PROGRAM_ERROR;
                                        goto cleanup;
                                    }
                                    
                                    int64_t slice_23706 = merge_sequential_arg2_23432 + j_23481;
                                    int64_t slice_23707 = start_23242 + slice_23706;
                                    int32_t leq_arg1_23708 = ((int32_t *) mem_param_24058.mem)[slice_23707];
                                    int64_t slice_23712 = defunc_0_f_res_23423 + i_23478;
                                    int64_t slice_23713 = start_23242 + slice_23712;
                                    int32_t leq_arg0_23714 = ((int32_t *) mem_param_24058.mem)[slice_23713];
                                    bool defunc_0_leq_res_23715 = sle32(leq_arg0_23714, leq_arg1_23708);
                                    
                                    cond_f_res_23485 = defunc_0_leq_res_23715;
                                } else {
                                    cond_f_res_23485 = 0;
                                }
                                cond_23483 = cond_f_res_23485;
                            }
                            
                            int64_t loopres_23500;
                            int32_t loopres_23501;
                            int64_t loopres_23502;
                            
                            if (cond_23483) {
                                bool x_23717 = sle64((int64_t) 0, i_23478);
                                bool y_23718 = slt64(i_23478, j_m_i_23447);
                                bool bounds_check_23719 = x_23717 && y_23718;
                                bool index_certs_23720;
                                
                                if (!bounds_check_23719) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_23478, "] out of bounds for array of shape [", (long long) j_m_i_23447, "].", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:106:17-21\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  ftError.fut:34:58-61\n   #10 ftError.fut:42:28-32\n   #11 ftError.fut:38:1-42:32\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t tmp_23716 = add64((int64_t) 1, i_23478);
                                int64_t slice_23721 = defunc_0_f_res_23423 + i_23478;
                                int64_t slice_23722 = start_23242 + slice_23721;
                                int32_t tmp_23723 = ((int32_t *) mem_param_24058.mem)[slice_23722];
                                int64_t tmp_23724 = ((int64_t *) mem_param_24061.mem)[slice_23722];
                                
                                loopres_23500 = tmp_23716;
                                loopres_23501 = tmp_23723;
                                loopres_23502 = tmp_23724;
                            } else {
                                bool x_23512 = sle64((int64_t) 0, j_23481);
                                bool y_23513 = slt64(j_23481, j_m_i_23434);
                                bool bounds_check_23514 = x_23512 && y_23513;
                                bool index_certs_23515;
                                
                                if (!bounds_check_23514) {
                                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) j_23481, "] out of bounds for array of shape [", (long long) j_m_i_23434, "].", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:108:15-19\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:153:59-69\n   #2  /prelude/soacs.fut:59:9-11\n   #3  /prelude/array.fut:241:11-17\n   #4  lib/github.com/diku-dk/sorts/merge_sort.fut:260:43-59\n   #5  /prelude/soacs.fut:59:9-11\n   #6  /prelude/array.fut:241:11-17\n   #7  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #8  /prelude/functional.fut:9:44-45\n   #9  ftError.fut:34:58-61\n   #10 ftError.fut:42:28-32\n   #11 ftError.fut:38:1-42:32\n"));
                                    err = FUTHARK_PROGRAM_ERROR;
                                    goto cleanup;
                                }
                                
                                int64_t slice_23516 = merge_sequential_arg2_23432 + j_23481;
                                int64_t slice_23517 = start_23242 + slice_23516;
                                int32_t tmp_23518 = ((int32_t *) mem_param_24058.mem)[slice_23517];
                                int64_t tmp_23519 = ((int64_t *) mem_param_24061.mem)[slice_23517];
                                
                                loopres_23500 = i_23478;
                                loopres_23501 = tmp_23518;
                                loopres_23502 = tmp_23519;
                            }
                            ((int32_t *) mem_24094)[i_23841 * greatest_divisor_leq_than_res_23055 + k_23477] = loopres_23501;
                            ((int64_t *) mem_24097)[i_23841 * greatest_divisor_leq_than_res_23055 + k_23477] = loopres_23502;
                            
                            int64_t i_tmp_24320 = loopres_23500;
                            
                            i_23478 = i_tmp_24320;
                        }
                        data_23474 = i_23478;
                    }
                    if (memblock_alloc(ctx, &mem_24132, bytes_24093, "mem_24132")) {
                        err = 1;
                        goto cleanup;
                    }
                    lmad_copy_4b(ctx, 2, (uint32_t *) mem_24132.mem, (int64_t) 0, (int64_t []) {greatest_divisor_leq_than_res_23055, (int64_t) 1}, (uint32_t *) mem_24094, (int64_t) 0, (int64_t []) {greatest_divisor_leq_than_res_23055, (int64_t) 1}, (int64_t []) {num_blocks_23190, greatest_divisor_leq_than_res_23055});
                    if (memblock_alloc(ctx, &mem_24136, bytes_24096, "mem_24136")) {
                        err = 1;
                        goto cleanup;
                    }
                    lmad_copy_8b(ctx, 2, (uint64_t *) mem_24136.mem, (int64_t) 0, (int64_t []) {greatest_divisor_leq_than_res_23055, (int64_t) 1}, (uint64_t *) mem_24097, (int64_t) 0, (int64_t []) {greatest_divisor_leq_than_res_23055, (int64_t) 1}, (int64_t []) {num_blocks_23190, greatest_divisor_leq_than_res_23055});
                    if (memblock_set(ctx, &ext_mem_24159, &mem_24132, "mem_24132") != 0)
                        return 1;
                    if (memblock_set(ctx, &ext_mem_24156, &mem_24136, "mem_24136") != 0)
                        return 1;
                }
                lmad_copy_4b(ctx, 1, (uint32_t *) mem_24064, i_23848 * next_stride_23170, (int64_t []) {(int64_t) 1}, (uint32_t *) ext_mem_24159.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {next_stride_23170});
                if (memblock_unref(ctx, &ext_mem_24159, "ext_mem_24159") != 0)
                    return 1;
                lmad_copy_8b(ctx, 1, (uint64_t *) mem_24067, i_23848 * next_stride_23170, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_24156.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {next_stride_23170});
                if (memblock_unref(ctx, &ext_mem_24156, "ext_mem_24156") != 0)
                    return 1;
            }
            
            int64_t flat_dim_23526 = next_stride_23170 * num_merges_23174;
            
            if (memblock_alloc(ctx, &mem_24170, bytes_24063, "mem_24170")) {
                err = 1;
                goto cleanup;
            }
            lmad_copy_4b(ctx, 2, (uint32_t *) mem_24170.mem, (int64_t) 0, (int64_t []) {next_stride_23170, (int64_t) 1}, (uint32_t *) mem_24064, (int64_t) 0, (int64_t []) {next_stride_23170, (int64_t) 1}, (int64_t []) {num_merges_23174, next_stride_23170});
            if (memblock_alloc(ctx, &mem_24174, bytes_24066, "mem_24174")) {
                err = 1;
                goto cleanup;
            }
            lmad_copy_8b(ctx, 2, (uint64_t *) mem_24174.mem, (int64_t) 0, (int64_t []) {next_stride_23170, (int64_t) 1}, (uint64_t *) mem_24067, (int64_t) 0, (int64_t []) {next_stride_23170, (int64_t) 1}, (int64_t []) {num_merges_23174, next_stride_23170});
            
            bool loop_cond_23529 = slt64(next_stride_23170, flat_dim_23033);
            
            if (memblock_set(ctx, &mem_param_tmp_24294, &mem_24170, "mem_24170") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_tmp_24295, &mem_24174, "mem_24174") != 0)
                return 1;
            
            int64_t loop_dz2084Uz2082U_tmp_24296 = flat_dim_23526;
            bool loop_while_tmp_24297 = loop_cond_23529;
            int64_t stride_tmp_24300 = next_stride_23170;
            
            if (memblock_set(ctx, &mem_param_24058, &mem_param_tmp_24294, "mem_param_tmp_24294") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_24061, &mem_param_tmp_24295, "mem_param_tmp_24295") != 0)
                return 1;
            loop_dz2084Uz2082U_23165 = loop_dz2084Uz2082U_tmp_24296;
            loop_while_23166 = loop_while_tmp_24297;
            stride_23169 = stride_tmp_24300;
        }
        if (memblock_set(ctx, &ext_mem_24181, &mem_param_24058, "mem_param_24058") != 0)
            return 1;
        if (memblock_set(ctx, &ext_mem_24180, &mem_param_24061, "mem_param_24061") != 0)
            return 1;
        data_23160 = loop_dz2084Uz2082U_23165;
        data_23161 = loop_while_23166;
        data_23164 = stride_23169;
        if (memblock_unref(ctx, &mem_24050, "mem_24050") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_24054, "mem_24054") != 0)
            return 1;
        
        bool empty_slice_23530 = n_17305 == (int64_t) 0;
        bool zzero_leq_i_p_m_t_s_23531 = sle64((int64_t) 0, zs_lhs_23017);
        bool i_p_m_t_s_leq_w_23532 = slt64(zs_lhs_23017, data_23160);
        bool y_23533 = zzero_leq_i_p_m_t_s_23531 && i_p_m_t_s_leq_w_23532;
        bool ok_or_empty_23534 = empty_slice_23530 || y_23533;
        bool index_certs_23535;
        
        if (!ok_or_empty_23534) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) n_17305, "] out of bounds for array of shape [", (long long) data_23160, "].", "-> #0  /prelude/array.fut:46:45-51\n   #1  lib/github.com/diku-dk/sorts/merge_sort.fut:265:3-14\n   #2  lib/github.com/diku-dk/sorts/merge_sort.fut:272:74-75\n   #3  /prelude/functional.fut:9:44-45\n   #4  ftError.fut:34:58-61\n   #5  ftError.fut:42:28-32\n   #6  ftError.fut:38:1-42:32\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        if (memblock_set(ctx, &ext_mem_24184, &ext_mem_24180, "ext_mem_24180") != 0)
            return 1;
    }
    if (memblock_unref(ctx, &mem_23979, "mem_23979") != 0)
        return 1;
    if (memblock_alloc(ctx, &mem_24186, bytes_24185, "mem_24186")) {
        err = 1;
        goto cleanup;
    }
    if (mem_24188_cached_sizze_24337 < arg_22991) {
        err = lexical_realloc(ctx, &mem_24188, &mem_24188_cached_sizze_24337, arg_22991);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    for (int64_t i_23855 = 0; i_23855 < n_17305; i_23855++) {
        int64_t eta_p_23543 = ((int64_t *) ext_mem_24184.mem)[i_23855];
        bool x_23544 = sle64((int64_t) 0, eta_p_23543);
        bool y_23545 = slt64(eta_p_23543, n_17305);
        bool bounds_check_23546 = x_23544 && y_23545;
        bool index_certs_23547;
        
        if (!bounds_check_23546) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_23543, "] out of bounds for array of shape [", (long long) n_17305, "].", "-> #0  lib/github.com/diku-dk/sorts/merge_sort.fut:277:24-28\n   #1  /prelude/functional.fut:9:44-45\n   #2  lib/github.com/diku-dk/sorts/merge_sort.fut:277:8-29\n   #3  ftError.fut:34:58-61\n   #4  ftError.fut:42:28-32\n   #5  ftError.fut:38:1-42:32\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int32_t lifted_lambda_res_23548 = ((int32_t *) k_mem_23976.mem)[eta_p_23543];
        
        ((int32_t *) mem_24186.mem)[i_23855] = lifted_lambda_res_23548;
        lmad_copy_1b(ctx, 1, (uint8_t *) mem_24188, i_23855 * payload_bytes_17307, (int64_t []) {(int64_t) 1}, (uint8_t *) pL_mem_23977.mem, eta_p_23543 * payload_bytes_17307, (int64_t []) {(int64_t) 1}, (int64_t []) {payload_bytes_17307});
    }
    if (memblock_unref(ctx, &ext_mem_24184, "ext_mem_24184") != 0)
        return 1;
    if (memblock_alloc(ctx, &mem_24204, arg_22991, "mem_24204")) {
        err = 1;
        goto cleanup;
    }
    lmad_copy_1b(ctx, 2, (uint8_t *) mem_24204.mem, (int64_t) 0, (int64_t []) {payload_bytes_17307, (int64_t) 1}, (uint8_t *) mem_24188, (int64_t) 0, (int64_t []) {payload_bytes_17307, (int64_t) 1}, (int64_t []) {n_17305, payload_bytes_17307});
    if (memblock_set(ctx, &mem_out_24274, &mem_24186, "mem_24186") != 0)
        return 1;
    if (memblock_set(ctx, &mem_out_24275, &mem_24204, "mem_24204") != 0)
        return 1;
    if (memblock_set(ctx, &*mem_out_p_24325, &mem_out_24274, "mem_out_24274") != 0)
        return 1;
    if (memblock_set(ctx, &*mem_out_p_24326, &mem_out_24275, "mem_out_24275") != 0)
        return 1;
    
  cleanup:
    {
        free(mem_23982);
        free(mem_23985);
        free(mem_23995);
        free(mem_23997);
        free(mem_24064);
        free(mem_24067);
        free(mem_24077);
        free(mem_24079);
        free(mem_24094);
        free(mem_24097);
        free(mem_24188);
        if (memblock_unref(ctx, &mem_24204, "mem_24204") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_24186, "mem_24186") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_24295, "mem_param_tmp_24295") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_24294, "mem_param_tmp_24294") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_24174, "mem_24174") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_24170, "mem_24170") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_24136, "mem_24136") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_24132, "mem_24132") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_24141, "mem_24141") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_24139, "mem_24139") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_24156, "ext_mem_24156") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_24159, "ext_mem_24159") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_24061, "mem_param_24061") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_24058, "mem_param_24058") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_24180, "ext_mem_24180") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_24181, "ext_mem_24181") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_24054, "mem_24054") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_24050, "mem_24050") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_24184, "ext_mem_24184") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_23979, "mem_23979") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_24275, "mem_out_24275") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_24274, "mem_out_24274") != 0)
            return 1;
    }
    return err;
}
FUTHARK_FUN_ATTR int futrts_entry_radixSortRelation_int(struct futhark_context *ctx, struct memblock *mem_out_p_24338, struct memblock *mem_out_p_24339, struct memblock k_mem_23976, struct memblock pL_mem_23977, int64_t n_14783, int64_t dzlz7bUZLztZRz20Unz20Upayload_bytesz7dUzg_14784, int16_t block_sizze_14785, int64_t payload_bytes_14786)
{
    (void) ctx;
    
    int err = 0;
    int64_t mem_23979_cached_sizze_24340 = 0;
    unsigned char *mem_23979 = NULL;
    int64_t mem_23981_cached_sizze_24341 = 0;
    unsigned char *mem_23981 = NULL;
    int64_t mem_23989_cached_sizze_24342 = 0;
    unsigned char *mem_23989 = NULL;
    int64_t mem_23991_cached_sizze_24343 = 0;
    unsigned char *mem_23991 = NULL;
    int64_t mem_23993_cached_sizze_24344 = 0;
    unsigned char *mem_23993 = NULL;
    int64_t mem_23995_cached_sizze_24345 = 0;
    unsigned char *mem_23995 = NULL;
    int64_t mem_23997_cached_sizze_24346 = 0;
    unsigned char *mem_23997 = NULL;
    int64_t mem_24029_cached_sizze_24347 = 0;
    unsigned char *mem_24029 = NULL;
    int64_t mem_24031_cached_sizze_24348 = 0;
    unsigned char *mem_24031 = NULL;
    int64_t mem_24044_cached_sizze_24349 = 0;
    unsigned char *mem_24044 = NULL;
    int64_t mem_24045_cached_sizze_24350 = 0;
    unsigned char *mem_24045 = NULL;
    int64_t mem_24052_cached_sizze_24351 = 0;
    unsigned char *mem_24052 = NULL;
    int64_t mem_24055_cached_sizze_24352 = 0;
    unsigned char *mem_24055 = NULL;
    int64_t mem_24058_cached_sizze_24353 = 0;
    unsigned char *mem_24058 = NULL;
    int64_t mem_24061_cached_sizze_24354 = 0;
    unsigned char *mem_24061 = NULL;
    int64_t mem_24064_cached_sizze_24355 = 0;
    unsigned char *mem_24064 = NULL;
    int64_t mem_24086_cached_sizze_24356 = 0;
    unsigned char *mem_24086 = NULL;
    int64_t mem_24088_cached_sizze_24357 = 0;
    unsigned char *mem_24088 = NULL;
    int64_t mem_24090_cached_sizze_24358 = 0;
    unsigned char *mem_24090 = NULL;
    int64_t mem_24092_cached_sizze_24359 = 0;
    unsigned char *mem_24092 = NULL;
    int64_t mem_24094_cached_sizze_24360 = 0;
    unsigned char *mem_24094 = NULL;
    int64_t mem_24137_cached_sizze_24361 = 0;
    unsigned char *mem_24137 = NULL;
    int64_t mem_24138_cached_sizze_24362 = 0;
    unsigned char *mem_24138 = NULL;
    int64_t mem_24163_cached_sizze_24363 = 0;
    unsigned char *mem_24163 = NULL;
    int64_t mem_24171_cached_sizze_24364 = 0;
    unsigned char *mem_24171 = NULL;
    int64_t mem_24199_cached_sizze_24365 = 0;
    unsigned char *mem_24199 = NULL;
    struct memblock mem_24215;
    
    mem_24215.references = NULL;
    
    struct memblock mem_24197;
    
    mem_24197.references = NULL;
    
    struct memblock mem_out_24275;
    
    mem_out_24275.references = NULL;
    
    struct memblock mem_out_24274;
    
    mem_out_24274.references = NULL;
    
    int64_t arg_21348 = mul64(n_14783, payload_bytes_14786);
    bool assert_cond_21349 = arg_21348 == dzlz7bUZLztZRz20Unz20Upayload_bytesz7dUzg_14784;
    bool assert_c_21350;
    
    if (!assert_cond_21349) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "Assertion is false: entry point arguments have invalid sizes.", "-> #0  ftError.fut:24:28-26:77\n   #1  ftError.fut:21:1-26:78\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool binop_x_21351 = dzlz7bUZLztZRz20Unz20Upayload_bytesz7dUzg_14784 == arg_21348;
    bool binop_y_21352 = sle64((int64_t) 0, payload_bytes_14786);
    bool dim_ok_21353 = binop_x_21351 && binop_y_21352;
    bool dim_ok_cert_21354;
    
    if (!dim_ok_21353) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Cannot unflatten array of shape [", (long long) dzlz7bUZLztZRz20Unz20Upayload_bytesz7dUzg_14784, "] to array of shape [", (long long) n_14783, "][", (long long) payload_bytes_14786, "]", "-> #0  /prelude/array.fut:184:3-30\n   #1  ftError.fut:15:41-50\n   #2  /prelude/functional.fut:9:44-45\n   #3  ftError.fut:11:20-15:50\n   #4  ftError.fut:26:66-77\n   #5  ftError.fut:21:1-26:78\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool cond_21356 = n_14783 == (int64_t) 0;
    int32_t iters_21357;
    
    if (cond_21356) {
        iters_21357 = 0;
    } else {
        iters_21357 = 16;
    }
    
    int64_t i16_res_21358 = sext_i16_i64(block_sizze_14785);
    bool zzero_21359 = i16_res_21358 == (int64_t) 0;
    bool nonzzero_21360 = !zzero_21359;
    bool nonzzero_cert_21361;
    
    if (!nonzzero_21360) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  lib/github.com/diku-dk/sorts/radix_sort.fut:222:20-27\n   #1  lib/github.com/diku-dk/sorts/radix_sort.fut:248:49-51\n   #2  /prelude/functional.fut:9:44-45\n   #3  lib/github.com/diku-dk/sorts/radix_sort.fut:258:49-51\n   #4  ftError.fut:17:92-95\n   #5  ftError.fut:26:66-77\n   #6  ftError.fut:21:1-26:78\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t n_blocks_21362 = sdiv64(n_14783, i16_res_21358);
    int64_t rest_21363 = smod64(n_14783, i16_res_21358);
    int64_t arg_21364 = mul64(i16_res_21358, n_blocks_21362);
    int64_t dzlz7bUZLzpZRz20UZLZLztZRz20Un_blocksz20UblockZRz20Urestz7dUzg_21365 = add64(rest_21363, arg_21364);
    bool dim_match_21366 = dzlz7bUZLzpZRz20UZLZLztZRz20Un_blocksz20UblockZRz20Urestz7dUzg_21365 == n_14783;
    bool empty_or_match_cert_21367;
    
    if (!dim_match_21366) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Value of (desugared) shape [", (long long) n_14783, "] cannot match shape of type `[", (long long) dzlz7bUZLzpZRz20UZLZLztZRz20Un_blocksz20UblockZRz20Urestz7dUzg_21365, "](i32, i64)`.", "-> #0  /prelude/array.fut:58:47-57\n   #1  lib/github.com/diku-dk/sorts/radix_sort.fut:224:12-46\n   #2  lib/github.com/diku-dk/sorts/radix_sort.fut:248:49-51\n   #3  /prelude/functional.fut:9:44-45\n   #4  lib/github.com/diku-dk/sorts/radix_sort.fut:258:49-51\n   #5  ftError.fut:17:92-95\n   #6  ftError.fut:26:66-77\n   #7  ftError.fut:21:1-26:78\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t bytes_23978 = (int64_t) 4 * dzlz7bUZLzpZRz20UZLZLztZRz20Un_blocksz20UblockZRz20Urestz7dUzg_21365;
    int64_t bytes_23980 = (int64_t) 8 * dzlz7bUZLzpZRz20UZLZLztZRz20Un_blocksz20UblockZRz20Urestz7dUzg_21365;
    bool loop_nonempty_21371 = slt32(0, iters_21357);
    int64_t dzlz7bUZLzpZRz20Unz20U1z7dUzg_21372 = add64((int64_t) 1, n_blocks_21362);
    int64_t dzlz7bUZLztZRz20U4z20UZLZLzpZRz20Unz20U1ZRz7dUzg_21373 = mul64((int64_t) 4, dzlz7bUZLzpZRz20Unz20U1z7dUzg_21372);
    bool empty_slice_21374 = arg_21364 == (int64_t) 0;
    int64_t m_21375 = sub64(arg_21364, (int64_t) 1);
    bool zzero_leq_i_p_m_t_s_21376 = sle64((int64_t) 0, m_21375);
    bool i_p_m_t_s_leq_w_21377 = slt64(m_21375, dzlz7bUZLzpZRz20UZLZLztZRz20Un_blocksz20UblockZRz20Urestz7dUzg_21365);
    bool i_lte_j_21378 = sle64((int64_t) 0, arg_21364);
    bool y_21379 = zzero_leq_i_p_m_t_s_21376 && i_p_m_t_s_leq_w_21377;
    bool forwards_ok_21380 = i_lte_j_21378 && y_21379;
    bool ok_or_empty_21381 = empty_slice_21374 || forwards_ok_21380;
    bool loop_not_taken_21382 = !loop_nonempty_21371;
    bool protect_assert_disj_21383 = ok_or_empty_21381 || loop_not_taken_21382;
    bool index_certs_21384;
    
    if (!protect_assert_disj_21383) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) arg_21364, "] out of bounds for array of shape [", (long long) dzlz7bUZLzpZRz20UZLZLztZRz20Un_blocksz20UblockZRz20Urestz7dUzg_21365, "].", "-> #0  /prelude/array.fut:65:4-11\n   #1  lib/github.com/diku-dk/sorts/radix_sort.fut:163:34-164:32\n   #2  lib/github.com/diku-dk/sorts/radix_sort.fut:227:48-50\n   #3  lib/github.com/diku-dk/sorts/radix_sort.fut:248:49-51\n   #4  /prelude/functional.fut:9:44-45\n   #5  lib/github.com/diku-dk/sorts/radix_sort.fut:258:49-51\n   #6  ftError.fut:17:92-95\n   #7  ftError.fut:26:66-77\n   #8  ftError.fut:21:1-26:78\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool empty_slice_21385 = rest_21363 == (int64_t) 0;
    int64_t m_21386 = sub64(rest_21363, (int64_t) 1);
    int64_t i_p_m_t_s_21387 = add64(arg_21364, m_21386);
    bool zzero_leq_i_p_m_t_s_21388 = sle64((int64_t) 0, i_p_m_t_s_21387);
    bool i_p_m_t_s_leq_w_21389 = slt64(i_p_m_t_s_21387, dzlz7bUZLzpZRz20UZLZLztZRz20Un_blocksz20UblockZRz20Urestz7dUzg_21365);
    bool i_lte_j_21390 = sle64(arg_21364, dzlz7bUZLzpZRz20UZLZLztZRz20Un_blocksz20UblockZRz20Urestz7dUzg_21365);
    bool y_21391 = zzero_leq_i_p_m_t_s_21388 && i_p_m_t_s_leq_w_21389;
    bool forwards_ok_21392 = i_lte_j_21390 && y_21391;
    bool ok_or_empty_21393 = empty_slice_21385 || forwards_ok_21392;
    bool protect_assert_disj_21394 = loop_not_taken_21382 || ok_or_empty_21393;
    bool index_certs_21395;
    
    if (!protect_assert_disj_21394) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) arg_21364, ":", (long long) dzlz7bUZLzpZRz20UZLZLztZRz20Un_blocksz20UblockZRz20Urestz7dUzg_21365, "] out of bounds for array of shape [", (long long) dzlz7bUZLzpZRz20UZLZLztZRz20Un_blocksz20UblockZRz20Urestz7dUzg_21365, "].", "-> #0  /prelude/array.fut:65:13-24\n   #1  lib/github.com/diku-dk/sorts/radix_sort.fut:163:34-164:32\n   #2  lib/github.com/diku-dk/sorts/radix_sort.fut:227:48-50\n   #3  lib/github.com/diku-dk/sorts/radix_sort.fut:248:49-51\n   #4  /prelude/functional.fut:9:44-45\n   #5  lib/github.com/diku-dk/sorts/radix_sort.fut:258:49-51\n   #6  ftError.fut:17:92-95\n   #7  ftError.fut:26:66-77\n   #8  ftError.fut:21:1-26:78\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool binop_x_21396 = sle64((int64_t) 0, n_blocks_21362);
    bool binop_y_21397 = sle64((int64_t) 0, i16_res_21358);
    bool binop_y_21398 = binop_x_21396 && binop_y_21397;
    bool protect_assert_disj_21399 = loop_not_taken_21382 || binop_y_21398;
    bool dim_ok_cert_21400;
    
    if (!protect_assert_disj_21399) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Cannot unflatten array of shape [", (long long) arg_21364, "] to array of shape [", (long long) n_blocks_21362, "][", (long long) i16_res_21358, "]", "-> #0  /prelude/array.fut:184:3-30\n   #1  lib/github.com/diku-dk/sorts/radix_sort.fut:163:34-168:21\n   #2  lib/github.com/diku-dk/sorts/radix_sort.fut:227:48-50\n   #3  lib/github.com/diku-dk/sorts/radix_sort.fut:248:49-51\n   #4  /prelude/functional.fut:9:44-45\n   #5  lib/github.com/diku-dk/sorts/radix_sort.fut:258:49-51\n   #6  ftError.fut:17:92-95\n   #7  ftError.fut:26:66-77\n   #8  ftError.fut:21:1-26:78\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t tmp_21401 = sub64(i16_res_21358, (int64_t) 1);
    bool x_21402 = sle64((int64_t) 0, tmp_21401);
    bool y_21403 = slt64(tmp_21401, i16_res_21358);
    bool bounds_check_21404 = x_21402 && y_21403;
    bool protect_assert_disj_21405 = zzero_21359 || bounds_check_21404;
    bool protect_assert_disj_21406 = loop_not_taken_21382 || protect_assert_disj_21405;
    bool index_certs_21407;
    
    if (!protect_assert_disj_21406) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_21401, "] out of bounds for array of shape [", (long long) i16_res_21358, "].", "-> #0  /prelude/array.fut:28:29-37\n   #1  lib/github.com/diku-dk/sorts/radix_sort.fut:149:53-65\n   #2  /prelude/functional.fut:9:44-45\n   #3  lib/github.com/diku-dk/sorts/radix_sort.fut:227:48-50\n   #4  lib/github.com/diku-dk/sorts/radix_sort.fut:248:49-51\n   #5  /prelude/functional.fut:9:44-45\n   #6  lib/github.com/diku-dk/sorts/radix_sort.fut:258:49-51\n   #7  ftError.fut:17:92-95\n   #8  ftError.fut:26:66-77\n   #9  ftError.fut:21:1-26:78\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool y_21409 = slt64((int64_t) 0, dzlz7bUZLztZRz20U4z20UZLZLzpZRz20Unz20U1ZRz7dUzg_21373);
    bool protect_assert_disj_21410 = loop_not_taken_21382 || y_21409;
    bool index_certs_21411;
    
    if (!protect_assert_disj_21410) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, "] out of bounds for array of shape [", (long long) dzlz7bUZLztZRz20U4z20UZLZLzpZRz20Unz20U1ZRz7dUzg_21373, "].", "-> #0  lib/github.com/diku-dk/sorts/radix_sort.fut:122:3-123:7\n   #1  /prelude/functional.fut:9:44-45\n   #2  lib/github.com/diku-dk/sorts/radix_sort.fut:178:8-20\n   #3  lib/github.com/diku-dk/sorts/radix_sort.fut:227:48-50\n   #4  lib/github.com/diku-dk/sorts/radix_sort.fut:248:49-51\n   #5  /prelude/functional.fut:9:44-45\n   #6  lib/github.com/diku-dk/sorts/radix_sort.fut:258:49-51\n   #7  ftError.fut:17:92-95\n   #8  ftError.fut:26:66-77\n   #9  ftError.fut:21:1-26:78\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool protect_assert_disj_21413 = nonzzero_21360 || loop_not_taken_21382;
    bool nonzzero_cert_21414;
    
    if (!protect_assert_disj_21413) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "division by zero", "-> #0  lib/github.com/diku-dk/sorts/radix_sort.fut:186:27-30\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  lib/github.com/diku-dk/sorts/radix_sort.fut:183:7-191:15\n   #4  lib/github.com/diku-dk/sorts/radix_sort.fut:227:48-50\n   #5  lib/github.com/diku-dk/sorts/radix_sort.fut:248:49-51\n   #6  /prelude/functional.fut:9:44-45\n   #7  lib/github.com/diku-dk/sorts/radix_sort.fut:258:49-51\n   #8  ftError.fut:17:92-95\n   #9  ftError.fut:26:66-77\n   #10 ftError.fut:21:1-26:78\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    bool x_21415 = sle64((int64_t) 0, m_21386);
    bool y_21416 = slt64(m_21386, rest_21363);
    bool bounds_check_21417 = x_21415 && y_21416;
    bool protect_assert_disj_21418 = empty_slice_21385 || bounds_check_21417;
    bool protect_assert_disj_21419 = loop_not_taken_21382 || protect_assert_disj_21418;
    bool index_certs_21420;
    
    if (!protect_assert_disj_21419) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) m_21386, "] out of bounds for array of shape [", (long long) rest_21363, "].", "-> #0  /prelude/array.fut:28:29-37\n   #1  lib/github.com/diku-dk/sorts/radix_sort.fut:149:53-65\n   #2  lib/github.com/diku-dk/sorts/radix_sort.fut:166:41-45\n   #3  lib/github.com/diku-dk/sorts/radix_sort.fut:227:48-50\n   #4  lib/github.com/diku-dk/sorts/radix_sort.fut:248:49-51\n   #5  /prelude/functional.fut:9:44-45\n   #6  lib/github.com/diku-dk/sorts/radix_sort.fut:258:49-51\n   #7  ftError.fut:17:92-95\n   #8  ftError.fut:26:66-77\n   #9  ftError.fut:21:1-26:78\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t bytes_23988 = (int64_t) 2 * rest_21363;
    int64_t bytes_24028 = (int64_t) 8 * rest_21363;
    int64_t bytes_24030 = (int64_t) 4 * rest_21363;
    int64_t binop_x_24053 = (int64_t) 4 * n_blocks_21362;
    int64_t bytes_24054 = i16_res_21358 * binop_x_24053;
    int64_t binop_x_24056 = (int64_t) 8 * n_blocks_21362;
    int64_t bytes_24057 = i16_res_21358 * binop_x_24056;
    int64_t bytes_24060 = (int64_t) 32 * n_blocks_21362;
    int64_t bytes_24085 = (int64_t) 2 * i16_res_21358;
    int64_t bytes_24162 = (int64_t) 32 * dzlz7bUZLzpZRz20Unz20U1z7dUzg_21372;
    bool dim_match_21652 = n_14783 == dzlz7bUZLzpZRz20UZLZLztZRz20Un_blocksz20UblockZRz20Urestz7dUzg_21365;
    bool empty_or_match_cert_21653;
    
    if (!dim_match_21652) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Value of (desugared) shape [", (long long) dzlz7bUZLzpZRz20UZLZLztZRz20Un_blocksz20UblockZRz20Urestz7dUzg_21365, "] cannot match shape of type `[", (long long) n_14783, "](i32, i64)`.", "-> #0  /prelude/array.fut:58:47-57\n   #1  lib/github.com/diku-dk/sorts/radix_sort.fut:225:6-11\n   #2  /prelude/functional.fut:26:41-42\n   #3  lib/github.com/diku-dk/sorts/radix_sort.fut:226:6-227:50\n   #4  lib/github.com/diku-dk/sorts/radix_sort.fut:248:49-51\n   #5  /prelude/functional.fut:9:44-45\n   #6  lib/github.com/diku-dk/sorts/radix_sort.fut:258:49-51\n   #7  ftError.fut:17:92-95\n   #8  ftError.fut:26:66-77\n   #9  ftError.fut:21:1-26:78\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t bytes_24196 = (int64_t) 4 * n_14783;
    bool empty_or_match_cert_21665;
    
    if (!binop_x_21351) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "Function return value does not match shape of declared return type.", "-> #0  unknown location\n   #1  ftError.fut:19:38-45\n   #2  ftError.fut:26:66-77\n   #3  ftError.fut:21:1-26:78\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    if (mem_23979_cached_sizze_24340 < bytes_23978) {
        err = lexical_realloc(ctx, &mem_23979, &mem_23979_cached_sizze_24340, bytes_23978);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    lmad_copy_4b(ctx, 1, (uint32_t *) mem_23979, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint32_t *) k_mem_23976.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {dzlz7bUZLzpZRz20UZLZLztZRz20Un_blocksz20UblockZRz20Urestz7dUzg_21365});
    if (mem_23981_cached_sizze_24341 < bytes_23980) {
        err = lexical_realloc(ctx, &mem_23981, &mem_23981_cached_sizze_24341, bytes_23980);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    for (int64_t i_24276 = 0; i_24276 < dzlz7bUZLzpZRz20UZLZLztZRz20Un_blocksz20UblockZRz20Urestz7dUzg_21365; i_24276++) {
        int64_t x_24277 = (int64_t) 0 + i_24276 * (int64_t) 1;
        
        ((int64_t *) mem_23981)[i_24276] = x_24277;
    }
    if (mem_23989_cached_sizze_24342 < bytes_23988) {
        err = lexical_realloc(ctx, &mem_23989, &mem_23989_cached_sizze_24342, bytes_23988);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_23991_cached_sizze_24343 < bytes_23988) {
        err = lexical_realloc(ctx, &mem_23991, &mem_23991_cached_sizze_24343, bytes_23988);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_23993_cached_sizze_24344 < bytes_23988) {
        err = lexical_realloc(ctx, &mem_23993, &mem_23993_cached_sizze_24344, bytes_23988);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_23995_cached_sizze_24345 < bytes_23988) {
        err = lexical_realloc(ctx, &mem_23995, &mem_23995_cached_sizze_24345, bytes_23988);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_23997_cached_sizze_24346 < bytes_23988) {
        err = lexical_realloc(ctx, &mem_23997, &mem_23997_cached_sizze_24346, bytes_23988);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_24029_cached_sizze_24347 < bytes_24028) {
        err = lexical_realloc(ctx, &mem_24029, &mem_24029_cached_sizze_24347, bytes_24028);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_24031_cached_sizze_24348 < bytes_24030) {
        err = lexical_realloc(ctx, &mem_24031, &mem_24031_cached_sizze_24348, bytes_24030);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_24044_cached_sizze_24349 < (int64_t) 8) {
        err = lexical_realloc(ctx, &mem_24044, &mem_24044_cached_sizze_24349, (int64_t) 8);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_24045_cached_sizze_24350 < (int64_t) 32) {
        err = lexical_realloc(ctx, &mem_24045, &mem_24045_cached_sizze_24350, (int64_t) 32);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_24052_cached_sizze_24351 < (int64_t) 8) {
        err = lexical_realloc(ctx, &mem_24052, &mem_24052_cached_sizze_24351, (int64_t) 8);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_24055_cached_sizze_24352 < bytes_24054) {
        err = lexical_realloc(ctx, &mem_24055, &mem_24055_cached_sizze_24352, bytes_24054);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_24058_cached_sizze_24353 < bytes_24057) {
        err = lexical_realloc(ctx, &mem_24058, &mem_24058_cached_sizze_24353, bytes_24057);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_24061_cached_sizze_24354 < bytes_24060) {
        err = lexical_realloc(ctx, &mem_24061, &mem_24061_cached_sizze_24354, bytes_24060);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_24064_cached_sizze_24355 < binop_x_24056) {
        err = lexical_realloc(ctx, &mem_24064, &mem_24064_cached_sizze_24355, binop_x_24056);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_24086_cached_sizze_24356 < bytes_24085) {
        err = lexical_realloc(ctx, &mem_24086, &mem_24086_cached_sizze_24356, bytes_24085);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_24088_cached_sizze_24357 < bytes_24085) {
        err = lexical_realloc(ctx, &mem_24088, &mem_24088_cached_sizze_24357, bytes_24085);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_24090_cached_sizze_24358 < bytes_24085) {
        err = lexical_realloc(ctx, &mem_24090, &mem_24090_cached_sizze_24358, bytes_24085);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_24092_cached_sizze_24359 < bytes_24085) {
        err = lexical_realloc(ctx, &mem_24092, &mem_24092_cached_sizze_24359, bytes_24085);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_24094_cached_sizze_24360 < bytes_24085) {
        err = lexical_realloc(ctx, &mem_24094, &mem_24094_cached_sizze_24360, bytes_24085);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_24137_cached_sizze_24361 < (int64_t) 8) {
        err = lexical_realloc(ctx, &mem_24137, &mem_24137_cached_sizze_24361, (int64_t) 8);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_24138_cached_sizze_24362 < (int64_t) 32) {
        err = lexical_realloc(ctx, &mem_24138, &mem_24138_cached_sizze_24362, (int64_t) 32);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_24163_cached_sizze_24363 < bytes_24162) {
        err = lexical_realloc(ctx, &mem_24163, &mem_24163_cached_sizze_24363, bytes_24162);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_24171_cached_sizze_24364 < bytes_24162) {
        err = lexical_realloc(ctx, &mem_24171, &mem_24171_cached_sizze_24364, bytes_24162);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    for (int32_t i_21423 = 0; i_21423 < iters_21357; i_21423++) {
        int32_t blocked_radix_sort_step_arg1_21426 = mul32(2, i_21423);
        int32_t get_bit_arg0_21431 = add32(1, blocked_radix_sort_step_arg1_21426);
        bool cond_21432 = get_bit_arg0_21431 == 31;
        bool cond_21433 = blocked_radix_sort_step_arg1_21426 == 31;
        int16_t discard_23833;
        int16_t discard_23834;
        int16_t discard_23835;
        int16_t discard_23836;
        int16_t scanacc_23818;
        int16_t scanacc_23819;
        int16_t scanacc_23820;
        int16_t scanacc_23821;
        
        scanacc_23818 = (int16_t) 0;
        scanacc_23819 = (int16_t) 0;
        scanacc_23820 = (int16_t) 0;
        scanacc_23821 = (int16_t) 0;
        for (int64_t i_23827 = 0; i_23827 < rest_21363; i_23827++) {
            int64_t slice_23961 = arg_21364 + i_23827;
            int32_t eta_p_23658 = ((int32_t *) mem_23979)[slice_23961];
            int32_t zgzg_res_23659 = ashr32(eta_p_23658, get_bit_arg0_21431);
            int32_t za_res_23660 = 1 & zgzg_res_23659;
            int32_t defunc_0_get_bit_res_23661;
            
            if (cond_21432) {
                int32_t defunc_0_get_bit_res_t_res_23776 = 1 ^ za_res_23660;
                
                defunc_0_get_bit_res_23661 = defunc_0_get_bit_res_t_res_23776;
            } else {
                defunc_0_get_bit_res_23661 = za_res_23660;
            }
            
            int32_t zp_lhs_23663 = mul32(2, defunc_0_get_bit_res_23661);
            int32_t zgzg_res_23664 = ashr32(eta_p_23658, blocked_radix_sort_step_arg1_21426);
            int32_t za_res_23665 = 1 & zgzg_res_23664;
            int32_t defunc_0_get_bit_res_23666;
            
            if (cond_21433) {
                int32_t defunc_0_get_bit_res_t_res_23777 = 1 ^ za_res_23665;
                
                defunc_0_get_bit_res_23666 = defunc_0_get_bit_res_t_res_23777;
            } else {
                defunc_0_get_bit_res_23666 = za_res_23665;
            }
            
            int32_t i32_arg0_23668 = add32(zp_lhs_23663, defunc_0_get_bit_res_23666);
            int16_t i32_res_23669 = sext_i32_i16(i32_arg0_23668);
            int16_t defunc_0_f_res_23671;
            int16_t defunc_0_f_res_23672;
            int16_t defunc_0_f_res_23673;
            int16_t defunc_0_f_res_23674;
            
            if (futrts_lifted_lambda_6832(ctx, &defunc_0_f_res_23671, &defunc_0_f_res_23672, &defunc_0_f_res_23673, &defunc_0_f_res_23674, i32_res_23669) != 0) {
                err = 1;
                goto cleanup;
            }
            
            int16_t defunc_0_op_res_21470 = add16(defunc_0_f_res_23671, scanacc_23818);
            int16_t defunc_0_op_res_21471 = add16(defunc_0_f_res_23672, scanacc_23819);
            int16_t defunc_0_op_res_21472 = add16(defunc_0_f_res_23673, scanacc_23820);
            int16_t defunc_0_op_res_21473 = add16(defunc_0_f_res_23674, scanacc_23821);
            
            ((int16_t *) mem_23989)[i_23827] = defunc_0_op_res_21470;
            ((int16_t *) mem_23991)[i_23827] = defunc_0_op_res_21471;
            ((int16_t *) mem_23993)[i_23827] = defunc_0_op_res_21472;
            ((int16_t *) mem_23995)[i_23827] = defunc_0_op_res_21473;
            ((int16_t *) mem_23997)[i_23827] = i32_res_23669;
            
            int16_t scanacc_tmp_24280 = defunc_0_op_res_21470;
            int16_t scanacc_tmp_24281 = defunc_0_op_res_21471;
            int16_t scanacc_tmp_24282 = defunc_0_op_res_21472;
            int16_t scanacc_tmp_24283 = defunc_0_op_res_21473;
            
            scanacc_23818 = scanacc_tmp_24280;
            scanacc_23819 = scanacc_tmp_24281;
            scanacc_23820 = scanacc_tmp_24282;
            scanacc_23821 = scanacc_tmp_24283;
        }
        discard_23833 = scanacc_23818;
        discard_23834 = scanacc_23819;
        discard_23835 = scanacc_23820;
        discard_23836 = scanacc_23821;
        
        int16_t defunc_0_radix_sort_step_i16_res_21474;
        int16_t defunc_0_radix_sort_step_i16_res_21475;
        int16_t defunc_0_radix_sort_step_i16_res_21476;
        int16_t defunc_0_radix_sort_step_i16_res_21477;
        
        if (empty_slice_21385) {
            defunc_0_radix_sort_step_i16_res_21474 = (int16_t) 0;
            defunc_0_radix_sort_step_i16_res_21475 = (int16_t) 0;
            defunc_0_radix_sort_step_i16_res_21476 = (int16_t) 0;
            defunc_0_radix_sort_step_i16_res_21477 = (int16_t) 0;
        } else {
            int16_t last_res_21478 = ((int16_t *) mem_23989)[m_21386];
            int16_t last_res_21479 = ((int16_t *) mem_23991)[m_21386];
            int16_t last_res_21480 = ((int16_t *) mem_23993)[m_21386];
            int16_t last_res_21481 = ((int16_t *) mem_23995)[m_21386];
            
            defunc_0_radix_sort_step_i16_res_21474 = last_res_21478;
            defunc_0_radix_sort_step_i16_res_21475 = last_res_21479;
            defunc_0_radix_sort_step_i16_res_21476 = last_res_21480;
            defunc_0_radix_sort_step_i16_res_21477 = last_res_21481;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_24029, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_23981, (int64_t) 0 + (int64_t) 1 * arg_21364, (int64_t []) {(int64_t) 1}, (int64_t []) {rest_21363});
        lmad_copy_4b(ctx, 1, (uint32_t *) mem_24031, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint32_t *) mem_23979, (int64_t) 0 + (int64_t) 1 * arg_21364, (int64_t []) {(int64_t) 1}, (int64_t []) {rest_21363});
        for (int64_t write_iter_23837 = 0; write_iter_23837 < rest_21363; write_iter_23837++) {
            int16_t write_iv_23840 = ((int16_t *) mem_23997)[write_iter_23837];
            int16_t write_iv_23841 = ((int16_t *) mem_23989)[write_iter_23837];
            int16_t write_iv_23842 = ((int16_t *) mem_23991)[write_iter_23837];
            int16_t write_iv_23843 = ((int16_t *) mem_23993)[write_iter_23837];
            int16_t write_iv_23844 = ((int16_t *) mem_23995)[write_iter_23837];
            int64_t slice_23960 = arg_21364 + write_iter_23837;
            int32_t write_iv_23845 = ((int32_t *) mem_23979)[slice_23960];
            int64_t write_iv_23846 = ((int64_t *) mem_23981)[slice_23960];
            int64_t defunc_0_f_res_23642;
            
            if (futrts_lifted_f_6833(ctx, &defunc_0_f_res_23642, defunc_0_radix_sort_step_i16_res_21474, defunc_0_radix_sort_step_i16_res_21475, defunc_0_radix_sort_step_i16_res_21476, write_iv_23840, write_iv_23841, write_iv_23842, write_iv_23843, write_iv_23844) != 0) {
                err = 1;
                goto cleanup;
            }
            if (sle64((int64_t) 0, defunc_0_f_res_23642) && slt64(defunc_0_f_res_23642, rest_21363)) {
                ((int32_t *) mem_24031)[defunc_0_f_res_23642] = write_iv_23845;
            }
            if (sle64((int64_t) 0, defunc_0_f_res_23642) && slt64(defunc_0_f_res_23642, rest_21363)) {
                ((int64_t *) mem_24029)[defunc_0_f_res_23642] = write_iv_23846;
            }
        }
        ((int16_t *) mem_24044)[(int64_t) 0] = defunc_0_radix_sort_step_i16_res_21474;
        ((int16_t *) mem_24044)[(int64_t) 1] = defunc_0_radix_sort_step_i16_res_21475;
        ((int16_t *) mem_24044)[(int64_t) 2] = defunc_0_radix_sort_step_i16_res_21476;
        ((int16_t *) mem_24044)[(int64_t) 3] = defunc_0_radix_sort_step_i16_res_21477;
        for (int64_t i_23851 = 0; i_23851 < (int64_t) 4; i_23851++) {
            int16_t eta_p_21498 = ((int16_t *) mem_24044)[i_23851];
            int64_t i16_res_21499 = sext_i16_i64(eta_p_21498);
            
            ((int64_t *) mem_24045)[i_23851] = i16_res_21499;
        }
        
        int16_t tmp_21500 = add16(defunc_0_radix_sort_step_i16_res_21474, defunc_0_radix_sort_step_i16_res_21475);
        int16_t tmp_21501 = add16(defunc_0_radix_sort_step_i16_res_21476, tmp_21500);
        
        ((int16_t *) mem_24052)[(int64_t) 0] = (int16_t) 0;
        ((int16_t *) mem_24052)[(int64_t) 1] = defunc_0_radix_sort_step_i16_res_21474;
        ((int16_t *) mem_24052)[(int64_t) 2] = tmp_21500;
        ((int16_t *) mem_24052)[(int64_t) 3] = tmp_21501;
        for (int64_t i_23901 = 0; i_23901 < n_blocks_21362; i_23901++) {
            lmad_copy_4b(ctx, 1, (uint32_t *) mem_24055, i_23901 * i16_res_21358, (int64_t []) {(int64_t) 1}, (uint32_t *) mem_23979, (int64_t) 0 + i_23901 * ((int64_t) 1 * i16_res_21358), (int64_t []) {(int64_t) 1}, (int64_t []) {i16_res_21358});
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_24058, i_23901 * i16_res_21358, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_23981, (int64_t) 0 + i_23901 * ((int64_t) 1 * i16_res_21358), (int64_t []) {(int64_t) 1}, (int64_t []) {i16_res_21358});
            
            int64_t binop_x_23974 = i16_res_21358 * i_23901;
            int16_t discard_23873;
            int16_t discard_23874;
            int16_t discard_23875;
            int16_t discard_23876;
            int16_t scanacc_23858;
            int16_t scanacc_23859;
            int16_t scanacc_23860;
            int16_t scanacc_23861;
            
            scanacc_23858 = (int16_t) 0;
            scanacc_23859 = (int16_t) 0;
            scanacc_23860 = (int16_t) 0;
            scanacc_23861 = (int16_t) 0;
            for (int64_t i_23867 = 0; i_23867 < i16_res_21358; i_23867++) {
                int64_t new_index_23975 = i_23867 + binop_x_23974;
                int32_t eta_p_23705 = ((int32_t *) mem_23979)[new_index_23975];
                int32_t zgzg_res_23706 = ashr32(eta_p_23705, get_bit_arg0_21431);
                int32_t za_res_23707 = 1 & zgzg_res_23706;
                int32_t defunc_0_get_bit_res_23708;
                
                if (cond_21432) {
                    int32_t defunc_0_get_bit_res_t_res_23778 = 1 ^ za_res_23707;
                    
                    defunc_0_get_bit_res_23708 = defunc_0_get_bit_res_t_res_23778;
                } else {
                    defunc_0_get_bit_res_23708 = za_res_23707;
                }
                
                int32_t zp_lhs_23710 = mul32(2, defunc_0_get_bit_res_23708);
                int32_t zgzg_res_23711 = ashr32(eta_p_23705, blocked_radix_sort_step_arg1_21426);
                int32_t za_res_23712 = 1 & zgzg_res_23711;
                int32_t defunc_0_get_bit_res_23713;
                
                if (cond_21433) {
                    int32_t defunc_0_get_bit_res_t_res_23779 = 1 ^ za_res_23712;
                    
                    defunc_0_get_bit_res_23713 = defunc_0_get_bit_res_t_res_23779;
                } else {
                    defunc_0_get_bit_res_23713 = za_res_23712;
                }
                
                int32_t i32_arg0_23715 = add32(zp_lhs_23710, defunc_0_get_bit_res_23713);
                int16_t i32_res_23716 = sext_i32_i16(i32_arg0_23715);
                int16_t defunc_0_f_res_23718;
                int16_t defunc_0_f_res_23719;
                int16_t defunc_0_f_res_23720;
                int16_t defunc_0_f_res_23721;
                
                if (futrts_lifted_lambda_6832(ctx, &defunc_0_f_res_23718, &defunc_0_f_res_23719, &defunc_0_f_res_23720, &defunc_0_f_res_23721, i32_res_23716) != 0) {
                    err = 1;
                    goto cleanup;
                }
                
                int16_t defunc_0_op_res_21551 = add16(defunc_0_f_res_23718, scanacc_23858);
                int16_t defunc_0_op_res_21552 = add16(defunc_0_f_res_23719, scanacc_23859);
                int16_t defunc_0_op_res_21553 = add16(defunc_0_f_res_23720, scanacc_23860);
                int16_t defunc_0_op_res_21554 = add16(defunc_0_f_res_23721, scanacc_23861);
                
                ((int16_t *) mem_24086)[i_23867] = defunc_0_op_res_21551;
                ((int16_t *) mem_24088)[i_23867] = defunc_0_op_res_21552;
                ((int16_t *) mem_24090)[i_23867] = defunc_0_op_res_21553;
                ((int16_t *) mem_24092)[i_23867] = defunc_0_op_res_21554;
                ((int16_t *) mem_24094)[i_23867] = i32_res_23716;
                
                int16_t scanacc_tmp_24296 = defunc_0_op_res_21551;
                int16_t scanacc_tmp_24297 = defunc_0_op_res_21552;
                int16_t scanacc_tmp_24298 = defunc_0_op_res_21553;
                int16_t scanacc_tmp_24299 = defunc_0_op_res_21554;
                
                scanacc_23858 = scanacc_tmp_24296;
                scanacc_23859 = scanacc_tmp_24297;
                scanacc_23860 = scanacc_tmp_24298;
                scanacc_23861 = scanacc_tmp_24299;
            }
            discard_23873 = scanacc_23858;
            discard_23874 = scanacc_23859;
            discard_23875 = scanacc_23860;
            discard_23876 = scanacc_23861;
            
            int16_t defunc_0_f_res_21555;
            int16_t defunc_0_f_res_21556;
            int16_t defunc_0_f_res_21557;
            int16_t defunc_0_f_res_21558;
            
            if (zzero_21359) {
                defunc_0_f_res_21555 = (int16_t) 0;
                defunc_0_f_res_21556 = (int16_t) 0;
                defunc_0_f_res_21557 = (int16_t) 0;
                defunc_0_f_res_21558 = (int16_t) 0;
            } else {
                int16_t last_res_21559 = ((int16_t *) mem_24086)[tmp_21401];
                int16_t last_res_21560 = ((int16_t *) mem_24088)[tmp_21401];
                int16_t last_res_21561 = ((int16_t *) mem_24090)[tmp_21401];
                int16_t last_res_21562 = ((int16_t *) mem_24092)[tmp_21401];
                
                defunc_0_f_res_21555 = last_res_21559;
                defunc_0_f_res_21556 = last_res_21560;
                defunc_0_f_res_21557 = last_res_21561;
                defunc_0_f_res_21558 = last_res_21562;
            }
            for (int64_t write_iter_23877 = 0; write_iter_23877 < i16_res_21358; write_iter_23877++) {
                int16_t write_iv_23880 = ((int16_t *) mem_24094)[write_iter_23877];
                int16_t write_iv_23881 = ((int16_t *) mem_24086)[write_iter_23877];
                int16_t write_iv_23882 = ((int16_t *) mem_24088)[write_iter_23877];
                int16_t write_iv_23883 = ((int16_t *) mem_24090)[write_iter_23877];
                int16_t write_iv_23884 = ((int16_t *) mem_24092)[write_iter_23877];
                int64_t new_index_23971 = write_iter_23877 + binop_x_23974;
                int32_t write_iv_23885 = ((int32_t *) mem_23979)[new_index_23971];
                int64_t write_iv_23886 = ((int64_t *) mem_23981)[new_index_23971];
                int64_t defunc_0_f_res_23689;
                
                if (futrts_lifted_f_6833(ctx, &defunc_0_f_res_23689, defunc_0_f_res_21555, defunc_0_f_res_21556, defunc_0_f_res_21557, write_iv_23880, write_iv_23881, write_iv_23882, write_iv_23883, write_iv_23884) != 0) {
                    err = 1;
                    goto cleanup;
                }
                if (sle64((int64_t) 0, defunc_0_f_res_23689) && slt64(defunc_0_f_res_23689, i16_res_21358)) {
                    ((int32_t *) mem_24055)[i_23901 * i16_res_21358 + defunc_0_f_res_23689] = write_iv_23885;
                }
                if (sle64((int64_t) 0, defunc_0_f_res_23689) && slt64(defunc_0_f_res_23689, i16_res_21358)) {
                    ((int64_t *) mem_24058)[i_23901 * i16_res_21358 + defunc_0_f_res_23689] = write_iv_23886;
                }
            }
            ((int16_t *) mem_24137)[(int64_t) 0] = defunc_0_f_res_21555;
            ((int16_t *) mem_24137)[(int64_t) 1] = defunc_0_f_res_21556;
            ((int16_t *) mem_24137)[(int64_t) 2] = defunc_0_f_res_21557;
            ((int16_t *) mem_24137)[(int64_t) 3] = defunc_0_f_res_21558;
            for (int64_t i_23891 = 0; i_23891 < (int64_t) 4; i_23891++) {
                int16_t eta_p_21577 = ((int16_t *) mem_24137)[i_23891];
                int64_t i16_res_21578 = sext_i16_i64(eta_p_21577);
                
                ((int64_t *) mem_24138)[i_23891] = i16_res_21578;
            }
            
            int16_t tmp_21579 = add16(defunc_0_f_res_21555, defunc_0_f_res_21556);
            int16_t tmp_21580 = add16(defunc_0_f_res_21557, tmp_21579);
            
            ((int16_t *) mem_24064)[i_23901 * (int64_t) 4] = (int16_t) 0;
            ((int16_t *) mem_24064)[i_23901 * (int64_t) 4 + (int64_t) 1] = defunc_0_f_res_21555;
            ((int16_t *) mem_24064)[i_23901 * (int64_t) 4 + (int64_t) 2] = tmp_21579;
            ((int16_t *) mem_24064)[i_23901 * (int64_t) 4 + (int64_t) 3] = tmp_21580;
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_24061, i_23901 * (int64_t) 4, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_24138, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {(int64_t) 4});
        }
        
        int64_t discard_23913;
        int64_t scanacc_23909 = (int64_t) 0;
        
        for (int64_t i_23911 = 0; i_23911 < dzlz7bUZLztZRz20U4z20UZLZLzpZRz20Unz20U1ZRz7dUzg_21373; i_23911++) {
            int64_t new_index_23949 = squot64(i_23911, dzlz7bUZLzpZRz20Unz20U1z7dUzg_21372);
            int64_t binop_y_23951 = dzlz7bUZLzpZRz20Unz20U1z7dUzg_21372 * new_index_23949;
            int64_t new_index_23952 = i_23911 - binop_y_23951;
            bool index_concat_cmp_23954 = sle64(n_blocks_21362, new_index_23952);
            int64_t index_concat_branch_23958;
            
            if (index_concat_cmp_23954) {
                int64_t index_concat_23956 = ((int64_t *) mem_24045)[new_index_23949];
                
                index_concat_branch_23958 = index_concat_23956;
            } else {
                int64_t index_concat_23957 = ((int64_t *) mem_24061)[new_index_23952 * (int64_t) 4 + new_index_23949];
                
                index_concat_branch_23958 = index_concat_23957;
            }
            
            int64_t defunc_0_op_res_21592 = add64(scanacc_23909, index_concat_branch_23958);
            
            ((int64_t *) mem_24163)[i_23911] = defunc_0_op_res_21592;
            
            int64_t scanacc_tmp_24308 = defunc_0_op_res_21592;
            
            scanacc_23909 = scanacc_tmp_24308;
        }
        discard_23913 = scanacc_23909;
        for (int64_t i_23916 = 0; i_23916 < dzlz7bUZLztZRz20U4z20UZLZLzpZRz20Unz20U1ZRz7dUzg_21373; i_23916++) {
            int64_t zv_lhs_21595 = add64((int64_t) -1, i_23916);
            int64_t tmp_21596 = smod64(zv_lhs_21595, dzlz7bUZLztZRz20U4z20UZLZLzpZRz20Unz20U1ZRz7dUzg_21373);
            int64_t lifted_lambda_res_21597 = ((int64_t *) mem_24163)[tmp_21596];
            
            ((int64_t *) mem_24171)[i_23916] = lifted_lambda_res_21597;
        }
        ((int64_t *) mem_24171)[(int64_t) 0] = (int64_t) 0;
        for (int64_t write_iter_23918 = 0; write_iter_23918 < dzlz7bUZLzpZRz20UZLZLztZRz20Un_blocksz20UblockZRz20Urestz7dUzg_21365; write_iter_23918++) {
            bool index_concat_cmp_23940 = sle64(arg_21364, write_iter_23918);
            int32_t index_concat_branch_23593;
            
            if (index_concat_cmp_23940) {
                int64_t index_concat_i_23788 = sub64(write_iter_23918, arg_21364);
                int32_t index_concat_23789 = ((int32_t *) mem_24031)[index_concat_i_23788];
                
                index_concat_branch_23593 = index_concat_23789;
            } else {
                int64_t new_index_23596 = squot64(write_iter_23918, i16_res_21358);
                int64_t binop_y_23597 = i16_res_21358 * new_index_23596;
                int64_t new_index_23598 = write_iter_23918 - binop_y_23597;
                int32_t index_concat_23599 = ((int32_t *) mem_24055)[new_index_23596 * i16_res_21358 + new_index_23598];
                
                index_concat_branch_23593 = index_concat_23599;
            }
            
            int32_t zlzb_rhs_23600;
            int32_t acc_23602 = 0;
            
            for (int32_t i_23601 = 0; i_23601 < 2; i_23601++) {
                int32_t get_bit_arg0_23603 = add32(blocked_radix_sort_step_arg1_21426, i_23601);
                int32_t zgzg_res_23604 = ashr32(index_concat_branch_23593, get_bit_arg0_23603);
                int32_t za_res_23605 = 1 & zgzg_res_23604;
                bool cond_23606 = get_bit_arg0_23603 == 31;
                int32_t defunc_0_get_bit_res_23607;
                
                if (cond_23606) {
                    int32_t defunc_0_get_bit_res_t_res_23790 = 1 ^ za_res_23605;
                    
                    defunc_0_get_bit_res_23607 = defunc_0_get_bit_res_t_res_23790;
                } else {
                    defunc_0_get_bit_res_23607 = za_res_23605;
                }
                
                int32_t zp_rhs_23609 = shl32(defunc_0_get_bit_res_23607, i_23601);
                int32_t loopres_23610 = add32(acc_23602, zp_rhs_23609);
                int32_t acc_tmp_24313 = loopres_23610;
                
                acc_23602 = acc_tmp_24313;
            }
            zlzb_rhs_23600 = acc_23602;
            
            int64_t i32_res_23611 = sext_i32_i64(zlzb_rhs_23600);
            int64_t block_idx_23612 = sdiv64(write_iter_23918, i16_res_21358);
            bool x_23613 = sle64((int64_t) 0, block_idx_23612);
            bool y_23614 = slt64(block_idx_23612, dzlz7bUZLzpZRz20Unz20U1z7dUzg_21372);
            bool bounds_check_23615 = x_23613 && y_23614;
            bool index_certs_23616;
            
            if (!bounds_check_23615) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) block_idx_23612, "] out of bounds for array of shape [", (long long) dzlz7bUZLzpZRz20Unz20U1z7dUzg_21372, "].", "-> #0  lib/github.com/diku-dk/sorts/radix_sort.fut:187:26-48\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  lib/github.com/diku-dk/sorts/radix_sort.fut:183:7-191:15\n   #4  lib/github.com/diku-dk/sorts/radix_sort.fut:227:48-50\n   #5  lib/github.com/diku-dk/sorts/radix_sort.fut:248:49-51\n   #6  /prelude/functional.fut:9:44-45\n   #7  lib/github.com/diku-dk/sorts/radix_sort.fut:258:49-51\n   #8  ftError.fut:17:92-95\n   #9  ftError.fut:26:66-77\n   #10 ftError.fut:21:1-26:78\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            bool x_23617 = sle64((int64_t) 0, i32_res_23611);
            bool y_23618 = slt64(i32_res_23611, (int64_t) 4);
            bool bounds_check_23619 = x_23617 && y_23618;
            bool index_certs_23620;
            
            if (!bounds_check_23619) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i32_res_23611, "] out of bounds for array of shape [", (long long) (int64_t) 4, "].", "-> #0  lib/github.com/diku-dk/sorts/radix_sort.fut:187:26-53\n   #1  /prelude/soacs.fut:59:9-11\n   #2  /prelude/array.fut:241:11-17\n   #3  lib/github.com/diku-dk/sorts/radix_sort.fut:183:7-191:15\n   #4  lib/github.com/diku-dk/sorts/radix_sort.fut:227:48-50\n   #5  lib/github.com/diku-dk/sorts/radix_sort.fut:248:49-51\n   #6  /prelude/functional.fut:9:44-45\n   #7  lib/github.com/diku-dk/sorts/radix_sort.fut:258:49-51\n   #8  ftError.fut:17:92-95\n   #9  ftError.fut:26:66-77\n   #10 ftError.fut:21:1-26:78\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int32_t index_concat_branch_23944;
            
            if (index_concat_cmp_23940) {
                int64_t index_concat_i_23941 = sub64(write_iter_23918, arg_21364);
                int32_t index_concat_23942 = ((int32_t *) mem_24031)[index_concat_i_23941];
                
                index_concat_branch_23944 = index_concat_23942;
            } else {
                int64_t new_index_23966 = squot64(write_iter_23918, i16_res_21358);
                int64_t binop_y_23968 = i16_res_21358 * new_index_23966;
                int64_t new_index_23969 = write_iter_23918 - binop_y_23968;
                int32_t index_concat_23943 = ((int32_t *) mem_24055)[new_index_23966 * i16_res_21358 + new_index_23969];
                
                index_concat_branch_23944 = index_concat_23943;
            }
            
            int64_t index_concat_branch_23938;
            
            if (index_concat_cmp_23940) {
                int64_t index_concat_i_23935 = sub64(write_iter_23918, arg_21364);
                int64_t index_concat_23936 = ((int64_t *) mem_24029)[index_concat_i_23935];
                
                index_concat_branch_23938 = index_concat_23936;
            } else {
                int64_t new_index_23962 = squot64(write_iter_23918, i16_res_21358);
                int64_t binop_y_23964 = i16_res_21358 * new_index_23962;
                int64_t new_index_23965 = write_iter_23918 - binop_y_23964;
                int64_t index_concat_23937 = ((int64_t *) mem_24058)[new_index_23962 * i16_res_21358 + new_index_23965];
                
                index_concat_branch_23938 = index_concat_23937;
            }
            
            int64_t binop_x_23621 = dzlz7bUZLzpZRz20Unz20U1z7dUzg_21372 * i32_res_23611;
            int64_t new_index_23622 = block_idx_23612 + binop_x_23621;
            int64_t new_offset_23623 = ((int64_t *) mem_24171)[new_index_23622];
            bool index_concat_cmp_23624 = sle64(n_blocks_21362, block_idx_23612);
            int16_t index_concat_branch_23625;
            
            if (index_concat_cmp_23624) {
                int16_t index_concat_23791 = ((int16_t *) mem_24052)[i32_res_23611];
                
                index_concat_branch_23625 = index_concat_23791;
            } else {
                int16_t index_concat_23627 = ((int16_t *) mem_24064)[block_idx_23612 * (int64_t) 4 + i32_res_23611];
                
                index_concat_branch_23625 = index_concat_23627;
            }
            
            int64_t i16_res_23628 = sext_i16_i64(index_concat_branch_23625);
            int64_t zp_lhs_23629 = mul64(i16_res_21358, block_idx_23612);
            int64_t old_offset_23630 = add64(i16_res_23628, zp_lhs_23629);
            int64_t zp_lhs_23631 = sub64(write_iter_23918, old_offset_23630);
            int64_t idx_23632 = add64(new_offset_23623, zp_lhs_23631);
            
            if (sle64((int64_t) 0, idx_23632) && slt64(idx_23632, dzlz7bUZLzpZRz20UZLZLztZRz20Un_blocksz20UblockZRz20Urestz7dUzg_21365)) {
                ((int32_t *) mem_23979)[idx_23632] = index_concat_branch_23944;
            }
            if (sle64((int64_t) 0, idx_23632) && slt64(idx_23632, dzlz7bUZLzpZRz20UZLZLztZRz20Un_blocksz20UblockZRz20Urestz7dUzg_21365)) {
                ((int64_t *) mem_23981)[idx_23632] = index_concat_branch_23938;
            }
        }
    }
    if (memblock_alloc(ctx, &mem_24197, bytes_24196, "mem_24197")) {
        err = 1;
        goto cleanup;
    }
    if (mem_24199_cached_sizze_24365 < arg_21348) {
        err = lexical_realloc(ctx, &mem_24199, &mem_24199_cached_sizze_24365, arg_21348);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    for (int64_t i_23930 = 0; i_23930 < n_14783; i_23930++) {
        int64_t eta_p_21657 = ((int64_t *) mem_23981)[i_23930];
        bool x_21658 = sle64((int64_t) 0, eta_p_21657);
        bool y_21659 = slt64(eta_p_21657, n_14783);
        bool bounds_check_21660 = x_21658 && y_21659;
        bool index_certs_21661;
        
        if (!bounds_check_21660) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_21657, "] out of bounds for array of shape [", (long long) n_14783, "].", "-> #0  lib/github.com/diku-dk/sorts/radix_sort.fut:68:27-32\n   #1  /prelude/functional.fut:9:44-45\n   #2  lib/github.com/diku-dk/sorts/radix_sort.fut:68:6-33\n   #3  lib/github.com/diku-dk/sorts/radix_sort.fut:258:49-51\n   #4  ftError.fut:17:92-95\n   #5  ftError.fut:26:66-77\n   #6  ftError.fut:21:1-26:78\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int32_t lifted_lambda_res_21662 = ((int32_t *) k_mem_23976.mem)[eta_p_21657];
        
        ((int32_t *) mem_24197.mem)[i_23930] = lifted_lambda_res_21662;
        lmad_copy_1b(ctx, 1, (uint8_t *) mem_24199, i_23930 * payload_bytes_14786, (int64_t []) {(int64_t) 1}, (uint8_t *) pL_mem_23977.mem, eta_p_21657 * payload_bytes_14786, (int64_t []) {(int64_t) 1}, (int64_t []) {payload_bytes_14786});
    }
    if (memblock_alloc(ctx, &mem_24215, arg_21348, "mem_24215")) {
        err = 1;
        goto cleanup;
    }
    lmad_copy_1b(ctx, 2, (uint8_t *) mem_24215.mem, (int64_t) 0, (int64_t []) {payload_bytes_14786, (int64_t) 1}, (uint8_t *) mem_24199, (int64_t) 0, (int64_t []) {payload_bytes_14786, (int64_t) 1}, (int64_t []) {n_14783, payload_bytes_14786});
    if (memblock_set(ctx, &mem_out_24274, &mem_24197, "mem_24197") != 0)
        return 1;
    if (memblock_set(ctx, &mem_out_24275, &mem_24215, "mem_24215") != 0)
        return 1;
    if (memblock_set(ctx, &*mem_out_p_24338, &mem_out_24274, "mem_out_24274") != 0)
        return 1;
    if (memblock_set(ctx, &*mem_out_p_24339, &mem_out_24275, "mem_out_24275") != 0)
        return 1;
    
  cleanup:
    {
        free(mem_23979);
        free(mem_23981);
        free(mem_23989);
        free(mem_23991);
        free(mem_23993);
        free(mem_23995);
        free(mem_23997);
        free(mem_24029);
        free(mem_24031);
        free(mem_24044);
        free(mem_24045);
        free(mem_24052);
        free(mem_24055);
        free(mem_24058);
        free(mem_24061);
        free(mem_24064);
        free(mem_24086);
        free(mem_24088);
        free(mem_24090);
        free(mem_24092);
        free(mem_24094);
        free(mem_24137);
        free(mem_24138);
        free(mem_24163);
        free(mem_24171);
        free(mem_24199);
        if (memblock_unref(ctx, &mem_24215, "mem_24215") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_24197, "mem_24197") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_24275, "mem_out_24275") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_24274, "mem_out_24274") != 0)
            return 1;
    }
    return err;
}
FUTHARK_FUN_ATTR int futrts_lifted_f_6833(struct futhark_context *ctx, int64_t *out_prim_out_24366, int16_t na_12050, int16_t nb_12051, int16_t nc_12052, int16_t bin_12053, int16_t a_12054, int16_t b_12055, int16_t c_12056, int16_t d_12057)
{
    (void) ctx;
    
    int err = 0;
    int64_t prim_out_24274;
    bool bool_arg0_12058 = bin_12053 == (int16_t) 0;
    int16_t bool_res_17330 = btoi_bool_i16(bool_arg0_12058);
    int16_t zp_rhs_12062 = mul16(a_12054, bool_res_17330);
    int16_t zp_lhs_12065 = add16((int16_t) -1, zp_rhs_12062);
    bool bool_arg0_12067 = slt16((int16_t) 0, bin_12053);
    int16_t bool_res_17332 = btoi_bool_i16(bool_arg0_12067);
    int16_t zp_rhs_12071 = mul16(na_12050, bool_res_17332);
    int16_t zp_lhs_12073 = add16(zp_lhs_12065, zp_rhs_12071);
    bool bool_arg0_12075 = bin_12053 == (int16_t) 1;
    int16_t bool_res_17334 = btoi_bool_i16(bool_arg0_12075);
    int16_t zp_rhs_12079 = mul16(b_12055, bool_res_17334);
    int16_t zp_lhs_12081 = add16(zp_lhs_12073, zp_rhs_12079);
    bool bool_arg0_12083 = slt16((int16_t) 1, bin_12053);
    int16_t bool_res_17336 = btoi_bool_i16(bool_arg0_12083);
    int16_t zp_rhs_12087 = mul16(nb_12051, bool_res_17336);
    int16_t zp_lhs_12089 = add16(zp_lhs_12081, zp_rhs_12087);
    bool bool_arg0_12091 = bin_12053 == (int16_t) 2;
    int16_t bool_res_17338 = btoi_bool_i16(bool_arg0_12091);
    int16_t zp_rhs_12095 = mul16(c_12056, bool_res_17338);
    int16_t zp_lhs_12097 = add16(zp_lhs_12089, zp_rhs_12095);
    bool bool_arg0_12099 = slt16((int16_t) 2, bin_12053);
    int16_t bool_res_17340 = btoi_bool_i16(bool_arg0_12099);
    int16_t zp_rhs_12103 = mul16(nc_12052, bool_res_17340);
    int16_t zp_lhs_12105 = add16(zp_lhs_12097, zp_rhs_12103);
    bool bool_arg0_12107 = bin_12053 == (int16_t) 3;
    int16_t bool_res_17342 = btoi_bool_i16(bool_arg0_12107);
    int16_t zp_rhs_12111 = mul16(d_12057, bool_res_17342);
    int16_t i16_arg0_12113 = add16(zp_lhs_12105, zp_rhs_12111);
    int64_t i16_res_17344 = sext_i16_i64(i16_arg0_12113);
    
    prim_out_24274 = i16_res_17344;
    *out_prim_out_24366 = prim_out_24274;
    
  cleanup:
    { }
    return err;
}
FUTHARK_FUN_ATTR int futrts_lifted_lambda_6832(struct futhark_context *ctx, int16_t *out_prim_out_24367, int16_t *out_prim_out_24368, int16_t *out_prim_out_24369, int16_t *out_prim_out_24370, int16_t x_11993)
{
    (void) ctx;
    
    int err = 0;
    int16_t prim_out_24274;
    int16_t prim_out_24275;
    int16_t prim_out_24276;
    int16_t prim_out_24277;
    bool bool_arg0_11994 = x_11993 == (int16_t) 0;
    int16_t bool_res_17330 = btoi_bool_i16(bool_arg0_11994);
    bool bool_arg0_11998 = x_11993 == (int16_t) 1;
    int16_t bool_res_17332 = btoi_bool_i16(bool_arg0_11998);
    bool bool_arg0_12002 = x_11993 == (int16_t) 2;
    int16_t bool_res_17334 = btoi_bool_i16(bool_arg0_12002);
    bool bool_arg0_12006 = x_11993 == (int16_t) 3;
    int16_t bool_res_17336 = btoi_bool_i16(bool_arg0_12006);
    
    prim_out_24274 = bool_res_17330;
    prim_out_24275 = bool_res_17332;
    prim_out_24276 = bool_res_17334;
    prim_out_24277 = bool_res_17336;
    *out_prim_out_24367 = prim_out_24274;
    *out_prim_out_24368 = prim_out_24275;
    *out_prim_out_24369 = prim_out_24276;
    *out_prim_out_24370 = prim_out_24277;
    
  cleanup:
    { }
    return err;
}
FUTHARK_FUN_ATTR int futrts_lifted_normalizze_6809(struct futhark_context *ctx, int64_t *out_prim_out_24371, int64_t *out_prim_out_24372, int64_t *out_prim_out_24373, int64_t *out_prim_out_24374, int64_t *out_prim_out_24375, int64_t slo_15029, int64_t shi_15030, int64_t tlo_15031, int64_t thi_15032, int64_t count_15033)
{
    (void) ctx;
    
    int err = 0;
    int64_t prim_out_24274;
    int64_t prim_out_24275;
    int64_t prim_out_24276;
    int64_t prim_out_24277;
    int64_t prim_out_24278;
    int64_t min_arg1_15036 = add64(slo_15029, count_15033);
    int64_t min_res_17331 = smin64(shi_15030, min_arg1_15036);
    int64_t min_arg1_15040 = add64(tlo_15031, count_15033);
    int64_t min_res_17334 = smin64(thi_15032, min_arg1_15040);
    int64_t zp_lhs_15044 = sub64(min_res_17331, slo_15029);
    int64_t zp_rhs_15046 = sub64(min_res_17334, tlo_15031);
    int64_t zm_lhs_15048 = add64(zp_lhs_15044, zp_rhs_15046);
    int64_t slack_15050 = sub64(zm_lhs_15048, count_15033);
    int64_t min_res_17337 = smin64(zp_lhs_15044, slack_15050);
    int64_t min_res_17340 = smin64(zp_rhs_15046, min_res_17337);
    int64_t max_arg1_15060 = sub64(min_res_17331, min_res_17340);
    int64_t max_res_17343 = smax64(slo_15029, max_arg1_15060);
    int64_t max_arg1_15064 = sub64(min_res_17334, min_res_17340);
    int64_t max_res_17346 = smax64(tlo_15031, max_arg1_15064);
    int64_t zm_rhs_15068 = sub64(max_res_17343, slo_15029);
    int64_t zm_lhs_15070 = sub64(count_15033, zm_rhs_15068);
    int64_t zm_rhs_15072 = sub64(max_res_17346, tlo_15031);
    int64_t count_15074 = sub64(zm_lhs_15070, zm_rhs_15072);
    
    prim_out_24274 = max_res_17343;
    prim_out_24275 = min_res_17331;
    prim_out_24276 = max_res_17346;
    prim_out_24277 = min_res_17334;
    prim_out_24278 = count_15074;
    *out_prim_out_24371 = prim_out_24274;
    *out_prim_out_24372 = prim_out_24275;
    *out_prim_out_24373 = prim_out_24276;
    *out_prim_out_24374 = prim_out_24277;
    *out_prim_out_24375 = prim_out_24278;
    
  cleanup:
    { }
    return err;
}

int futhark_entry_mergeSortRelation_int(struct futhark_context *ctx, struct futhark_opaque_sortStruct_int **out0, const int64_t in0, const struct futhark_opaque_sortStruct_int *in1)
{
    int64_t n_17305 = (int64_t) 0;
    int64_t dzlz7bUZLztZRz20Unz20Upayload_bytesz7dUzg_17306 = (int64_t) 0;
    int64_t payload_bytes_17307 = (int64_t) 0;
    int ret = 0;
    
    lock_lock(&ctx->lock);
    
    struct memblock mem_out_24275;
    
    mem_out_24275.references = NULL;
    
    struct memblock mem_out_24274;
    
    mem_out_24274.references = NULL;
    
    struct memblock pL_mem_23977;
    
    pL_mem_23977.references = NULL;
    
    struct memblock k_mem_23976;
    
    k_mem_23976.references = NULL;
    payload_bytes_17307 = in0;
    k_mem_23976 = in1->v0->mem;
    n_17305 = in1->v0->shape[0];
    pL_mem_23977 = in1->v1->mem;
    dzlz7bUZLztZRz20Unz20Upayload_bytesz7dUzg_17306 = in1->v1->shape[0];
    if (!(n_17305 == in1->v0->shape[0] && dzlz7bUZLztZRz20Unz20Upayload_bytesz7dUzg_17306 == in1->v1->shape[0])) {
        ret = 1;
        set_error(ctx, msgprintf("Error: entry point arguments have invalid sizes.\n"));
    }
    if (ret == 0) {
        ret = futrts_entry_mergeSortRelation_int(ctx, &mem_out_24274, &mem_out_24275, k_mem_23976, pL_mem_23977, n_17305, dzlz7bUZLztZRz20Unz20Upayload_bytesz7dUzg_17306, payload_bytes_17307);
        if (ret == 0) {
            assert((*out0 = (struct futhark_opaque_sortStruct_int *) malloc(sizeof(struct futhark_opaque_sortStruct_int))) != NULL);
            assert(((*out0)->v0 = (struct futhark_i32_1d *) malloc(sizeof(struct futhark_i32_1d))) != NULL);
            (*out0)->v0->mem = mem_out_24274;
            (*out0)->v0->shape[0] = n_17305;
            assert(((*out0)->v1 = (struct futhark_u8_1d *) malloc(sizeof(struct futhark_u8_1d))) != NULL);
            (*out0)->v1->mem = mem_out_24275;
            (*out0)->v1->shape[0] = dzlz7bUZLztZRz20Unz20Upayload_bytesz7dUzg_17306;
        }
    }
    lock_unlock(&ctx->lock);
    return ret;
}
int futhark_entry_radixSortRelation_int(struct futhark_context *ctx, struct futhark_opaque_sortStruct_int **out0, const int16_t in0, const int64_t in1, const struct futhark_opaque_sortStruct_int *in2)
{
    int64_t n_14783 = (int64_t) 0;
    int64_t dzlz7bUZLztZRz20Unz20Upayload_bytesz7dUzg_14784 = (int64_t) 0;
    int16_t block_sizze_14785 = (int16_t) 0;
    int64_t payload_bytes_14786 = (int64_t) 0;
    int ret = 0;
    
    lock_lock(&ctx->lock);
    
    struct memblock mem_out_24275;
    
    mem_out_24275.references = NULL;
    
    struct memblock mem_out_24274;
    
    mem_out_24274.references = NULL;
    
    struct memblock pL_mem_23977;
    
    pL_mem_23977.references = NULL;
    
    struct memblock k_mem_23976;
    
    k_mem_23976.references = NULL;
    block_sizze_14785 = in0;
    payload_bytes_14786 = in1;
    k_mem_23976 = in2->v0->mem;
    n_14783 = in2->v0->shape[0];
    pL_mem_23977 = in2->v1->mem;
    dzlz7bUZLztZRz20Unz20Upayload_bytesz7dUzg_14784 = in2->v1->shape[0];
    if (!(n_14783 == in2->v0->shape[0] && dzlz7bUZLztZRz20Unz20Upayload_bytesz7dUzg_14784 == in2->v1->shape[0])) {
        ret = 1;
        set_error(ctx, msgprintf("Error: entry point arguments have invalid sizes.\n"));
    }
    if (ret == 0) {
        ret = futrts_entry_radixSortRelation_int(ctx, &mem_out_24274, &mem_out_24275, k_mem_23976, pL_mem_23977, n_14783, dzlz7bUZLztZRz20Unz20Upayload_bytesz7dUzg_14784, block_sizze_14785, payload_bytes_14786);
        if (ret == 0) {
            assert((*out0 = (struct futhark_opaque_sortStruct_int *) malloc(sizeof(struct futhark_opaque_sortStruct_int))) != NULL);
            assert(((*out0)->v0 = (struct futhark_i32_1d *) malloc(sizeof(struct futhark_i32_1d))) != NULL);
            (*out0)->v0->mem = mem_out_24274;
            (*out0)->v0->shape[0] = n_14783;
            assert(((*out0)->v1 = (struct futhark_u8_1d *) malloc(sizeof(struct futhark_u8_1d))) != NULL);
            (*out0)->v1->mem = mem_out_24275;
            (*out0)->v1->shape[0] = dzlz7bUZLztZRz20Unz20Upayload_bytesz7dUzg_14784;
        }
    }
    lock_unlock(&ctx->lock);
    return ret;
}
  
