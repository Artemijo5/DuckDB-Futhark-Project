
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
struct futhark_bool_1d;
struct futhark_bool_1d *futhark_new_bool_1d(struct futhark_context *ctx, const bool *data, int64_t dim0);
struct futhark_bool_1d *futhark_new_raw_bool_1d(struct futhark_context *ctx, unsigned char *data, int64_t dim0);
int futhark_free_bool_1d(struct futhark_context *ctx, struct futhark_bool_1d *arr);
int futhark_values_bool_1d(struct futhark_context *ctx, struct futhark_bool_1d *arr, bool *data);
int futhark_index_bool_1d(struct futhark_context *ctx, bool *out, struct futhark_bool_1d *arr, int64_t i0);
unsigned char *futhark_values_raw_bool_1d(struct futhark_context *ctx, struct futhark_bool_1d *arr);
const int64_t *futhark_shape_bool_1d(struct futhark_context *ctx, struct futhark_bool_1d *arr);

// Opaque values



// Entry points
int futhark_entry_main(struct futhark_context *ctx, struct futhark_bool_1d **out0, const int32_t in0);

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
static int futrts_cli_entry_main(struct futhark_context *ctx)
{
    int64_t t_start, t_end;
    int time_runs = 0, profile_run = 0;
    int retval = 0;
    
    // We do not want to profile all the initialisation.
    futhark_context_pause_profiling(ctx);
    // Declare and read input.
    set_binary_mode(stdin);
    
    int32_t read_value_0;
    
    if (read_scalar(stdin, &i32_info, &read_value_0) != 0)
        futhark_panic(1, "Error when reading input #%d of type %s (errno: %s).\n", 0, "i32", strerror(errno));
    ;
    if (end_of_input(stdin) != 0)
        futhark_panic(1, "Expected EOF on stdin after reading input for \"%s\".\n", "main");
    
    struct futhark_bool_1d * result_0;
    
    if (perform_warmup) {
        int r;
        
        ;
        if (futhark_context_sync(ctx) != 0)
            futhark_panic(1, "%s", futhark_context_get_error(ctx));
        ;
        // Only profile last run.
        if (profile_run)
            futhark_context_unpause_profiling(ctx);
        t_start = get_wall_time();
        r = futhark_entry_main(ctx, &result_0, read_value_0);
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
        assert(futhark_free_bool_1d(ctx, result_0) == 0);
    }
    time_runs = 1;
    // Proper run.
    for (int run = 0; run < num_runs; run++) {
        // Only profile last run.
        profile_run = run == num_runs - 1;
        
        int r;
        
        ;
        if (futhark_context_sync(ctx) != 0)
            futhark_panic(1, "%s", futhark_context_get_error(ctx));
        ;
        // Only profile last run.
        if (profile_run)
            futhark_context_unpause_profiling(ctx);
        t_start = get_wall_time();
        r = futhark_entry_main(ctx, &result_0, read_value_0);
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
        if (run < num_runs - 1) {
            assert(futhark_free_bool_1d(ctx, result_0) == 0);
        }
    }
    ;
    if (print_result) {
        // Print the final result.
        if (binary_output)
            set_binary_mode(stdout);
        {
            bool *arr = calloc(futhark_shape_bool_1d(ctx, result_0)[0], bool_info.size);
            
            assert(arr != NULL);
            assert(futhark_values_bool_1d(ctx, result_0, arr) == 0);
            assert(futhark_context_sync(ctx) == 0);
            write_array(stdout, binary_output, &bool_info, arr, futhark_shape_bool_1d(ctx, result_0), 1);
            free(arr);
        }
        printf("\n");
    }
    
  print_end:
    { }
    assert(futhark_free_bool_1d(ctx, result_0) == 0);
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
    
    struct entry_point_entry entry_points[] = {{.name ="main", .fun =futrts_cli_entry_main}};
    
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
static int8_t mainzistatic_array_realtype_20637[16] = { (int8_t) 4,(int8_t) 2,(int8_t) 2,(int8_t) 1,(int8_t) 2,(int8_t) 2,(int8_t) 3,(int8_t) 5,(int8_t) 2,(int8_t) 5,(int8_t) 3,(int8_t) 1,(int8_t) 4,(int8_t) 5,(int8_t) 3,(int8_t) 7};
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

FUTHARK_FUN_ATTR int futrts_entry_main(struct futhark_context *ctx, struct memblock *mem_out_p_20612, int64_t *out_prim_out_20613, int32_t max_depth_16888);
FUTHARK_FUN_ATTR int futrts_getPartitionBounds_8485(struct futhark_context *ctx, struct memblock *mem_out_p_20638, struct memblock *mem_out_p_20639, int64_t *out_prim_out_20640, int64_t *out_prim_out_20641, int32_t *out_prim_out_20642, int32_t *out_prim_out_20643, int32_t *out_prim_out_20644, struct memblock pXs_mem_20109, int64_t n_14231, int64_t b_14232, int32_t curDepth_14233, int32_t i_14235, int32_t j_14236);
FUTHARK_FUN_ATTR int futrts_set_bit_2464(struct futhark_context *ctx, int8_t *out_prim_out_20650, int32_t bit_12477, int8_t x_12478, int32_t b_12479);

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
struct futhark_bool_1d {
    struct memblock mem;
    int64_t shape[1];
};
struct futhark_bool_1d *futhark_new_bool_1d(struct futhark_context *ctx, const bool *data, int64_t dim0)
{
    int err = 0;
    struct futhark_bool_1d *bad = NULL;
    struct futhark_bool_1d *arr = (struct futhark_bool_1d *) malloc(sizeof(struct futhark_bool_1d));
    
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
struct futhark_bool_1d *futhark_new_raw_bool_1d(struct futhark_context *ctx, unsigned char *data, int64_t dim0)
{
    int err = 0;
    struct futhark_bool_1d *bad = NULL;
    struct futhark_bool_1d *arr = (struct futhark_bool_1d *) malloc(sizeof(struct futhark_bool_1d));
    
    if (arr == NULL)
        return bad;
    lock_lock(&ctx->lock);
    arr->mem.references = NULL;
    arr->mem.mem = data;
    arr->shape[0] = dim0;
    lock_unlock(&ctx->lock);
    return arr;
}
int futhark_free_bool_1d(struct futhark_context *ctx, struct futhark_bool_1d *arr)
{
    lock_lock(&ctx->lock);
    if (memblock_unref(ctx, &arr->mem, "arr->mem") != 0)
        return 1;
    lock_unlock(&ctx->lock);
    free(arr);
    return 0;
}
int futhark_values_bool_1d(struct futhark_context *ctx, struct futhark_bool_1d *arr, bool *data)
{
    int err = 0;
    
    lock_lock(&ctx->lock);
    if ((size_t) arr->shape[0] * 1 > 0)
        memmove((unsigned char *) data + 0, arr->mem.mem + 0, (size_t) arr->shape[0] * 1);
    lock_unlock(&ctx->lock);
    return err;
}
int futhark_index_bool_1d(struct futhark_context *ctx, bool *out, struct futhark_bool_1d *arr, int64_t i0)
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
unsigned char *futhark_values_raw_bool_1d(struct futhark_context *ctx, struct futhark_bool_1d *arr)
{
    (void) ctx;
    return arr->mem.mem;
}
const int64_t *futhark_shape_bool_1d(struct futhark_context *ctx, struct futhark_bool_1d *arr)
{
    (void) ctx;
    return arr->shape;
}

FUTHARK_FUN_ATTR int futrts_entry_main(struct futhark_context *ctx, struct memblock *mem_out_p_20612, int64_t *out_prim_out_20613, int32_t max_depth_16888)
{
    (void) ctx;
    
    int err = 0;
    int64_t mem_20153_cached_sizze_20614 = 0;
    unsigned char *mem_20153 = NULL;
    int64_t mem_20186_cached_sizze_20615 = 0;
    unsigned char *mem_20186 = NULL;
    int64_t mem_20188_cached_sizze_20616 = 0;
    unsigned char *mem_20188 = NULL;
    int64_t mem_20196_cached_sizze_20617 = 0;
    unsigned char *mem_20196 = NULL;
    int64_t mem_20224_cached_sizze_20618 = 0;
    unsigned char *mem_20224 = NULL;
    int64_t mem_20226_cached_sizze_20619 = 0;
    unsigned char *mem_20226 = NULL;
    int64_t mem_20228_cached_sizze_20620 = 0;
    unsigned char *mem_20228 = NULL;
    int64_t mem_20230_cached_sizze_20621 = 0;
    unsigned char *mem_20230 = NULL;
    int64_t mem_20256_cached_sizze_20622 = 0;
    unsigned char *mem_20256 = NULL;
    int64_t mem_20258_cached_sizze_20623 = 0;
    unsigned char *mem_20258 = NULL;
    int64_t mem_20330_cached_sizze_20624 = 0;
    unsigned char *mem_20330 = NULL;
    int64_t mem_20332_cached_sizze_20625 = 0;
    unsigned char *mem_20332 = NULL;
    int64_t mem_20334_cached_sizze_20626 = 0;
    unsigned char *mem_20334 = NULL;
    int64_t mem_20336_cached_sizze_20627 = 0;
    unsigned char *mem_20336 = NULL;
    int64_t mem_20362_cached_sizze_20628 = 0;
    unsigned char *mem_20362 = NULL;
    int64_t mem_20364_cached_sizze_20629 = 0;
    unsigned char *mem_20364 = NULL;
    int64_t mem_20366_cached_sizze_20630 = 0;
    unsigned char *mem_20366 = NULL;
    int64_t mem_20421_cached_sizze_20631 = 0;
    unsigned char *mem_20421 = NULL;
    int64_t mem_20450_cached_sizze_20632 = 0;
    unsigned char *mem_20450 = NULL;
    int64_t mem_20457_cached_sizze_20633 = 0;
    unsigned char *mem_20457 = NULL;
    int64_t mem_20458_cached_sizze_20634 = 0;
    unsigned char *mem_20458 = NULL;
    int64_t mem_20465_cached_sizze_20635 = 0;
    unsigned char *mem_20465 = NULL;
    int64_t mem_20466_cached_sizze_20636 = 0;
    unsigned char *mem_20466 = NULL;
    struct memblock mem_20485;
    
    mem_20485.references = NULL;
    
    struct memblock mem_20429;
    
    mem_20429.references = NULL;
    
    struct memblock mem_20428;
    
    mem_20428.references = NULL;
    
    struct memblock mem_20442;
    
    mem_20442.references = NULL;
    
    struct memblock ext_mem_20445;
    
    ext_mem_20445.references = NULL;
    
    struct memblock ext_mem_20448;
    
    ext_mem_20448.references = NULL;
    
    struct memblock mem_param_tmp_20566;
    
    mem_param_tmp_20566.references = NULL;
    
    struct memblock mem_param_tmp_20565;
    
    mem_param_tmp_20565.references = NULL;
    
    struct memblock mem_param_tmp_20586;
    
    mem_param_tmp_20586.references = NULL;
    
    struct memblock mem_param_tmp_20585;
    
    mem_param_tmp_20585.references = NULL;
    
    struct memblock mem_20407;
    
    mem_20407.references = NULL;
    
    struct memblock mem_20405;
    
    mem_20405.references = NULL;
    
    struct memblock ext_mem_20394;
    
    ext_mem_20394.references = NULL;
    
    struct memblock ext_mem_20395;
    
    ext_mem_20395.references = NULL;
    
    struct memblock mem_20392;
    
    mem_20392.references = NULL;
    
    struct memblock mem_param_20390;
    
    mem_param_20390.references = NULL;
    
    struct memblock mem_param_20387;
    
    mem_param_20387.references = NULL;
    
    struct memblock ext_mem_20412;
    
    ext_mem_20412.references = NULL;
    
    struct memblock ext_mem_20413;
    
    ext_mem_20413.references = NULL;
    
    struct memblock mem_param_20328;
    
    mem_param_20328.references = NULL;
    
    struct memblock mem_param_20325;
    
    mem_param_20325.references = NULL;
    
    struct memblock ext_mem_20418;
    
    ext_mem_20418.references = NULL;
    
    struct memblock ext_mem_20419;
    
    ext_mem_20419.references = NULL;
    
    struct memblock ext_mem_20321;
    
    ext_mem_20321.references = NULL;
    
    struct memblock ext_mem_20322;
    
    ext_mem_20322.references = NULL;
    
    struct memblock mem_20319;
    
    mem_20319.references = NULL;
    
    struct memblock mem_param_tmp_20516;
    
    mem_param_tmp_20516.references = NULL;
    
    struct memblock mem_param_tmp_20515;
    
    mem_param_tmp_20515.references = NULL;
    
    struct memblock mem_param_tmp_20514;
    
    mem_param_tmp_20514.references = NULL;
    
    struct memblock mem_param_tmp_20513;
    
    mem_param_tmp_20513.references = NULL;
    
    struct memblock mem_param_tmp_20527;
    
    mem_param_tmp_20527.references = NULL;
    
    struct memblock mem_param_tmp_20526;
    
    mem_param_tmp_20526.references = NULL;
    
    struct memblock mem_param_tmp_20525;
    
    mem_param_tmp_20525.references = NULL;
    
    struct memblock mem_param_tmp_20524;
    
    mem_param_tmp_20524.references = NULL;
    
    struct memblock mem_20292;
    
    mem_20292.references = NULL;
    
    struct memblock mem_20290;
    
    mem_20290.references = NULL;
    
    struct memblock mem_20272;
    
    mem_20272.references = NULL;
    
    struct memblock mem_20271;
    
    mem_20271.references = NULL;
    
    struct memblock ext_mem_20221;
    
    ext_mem_20221.references = NULL;
    
    struct memblock ext_mem_20222;
    
    ext_mem_20222.references = NULL;
    
    struct memblock mem_20219;
    
    mem_20219.references = NULL;
    
    struct memblock mem_param_tmp_20535;
    
    mem_param_tmp_20535.references = NULL;
    
    struct memblock mem_param_tmp_20534;
    
    mem_param_tmp_20534.references = NULL;
    
    struct memblock mem_20177;
    
    mem_20177.references = NULL;
    
    struct memblock mem_20175;
    
    mem_20175.references = NULL;
    
    struct memblock mem_param_tmp_20542;
    
    mem_param_tmp_20542.references = NULL;
    
    struct memblock mem_20164;
    
    mem_20164.references = NULL;
    
    struct memblock mem_param_20162;
    
    mem_param_20162.references = NULL;
    
    struct memblock ext_mem_20173;
    
    ext_mem_20173.references = NULL;
    
    struct memblock mem_param_20148;
    
    mem_param_20148.references = NULL;
    
    struct memblock mem_param_20145;
    
    mem_param_20145.references = NULL;
    
    struct memblock ext_mem_20216;
    
    ext_mem_20216.references = NULL;
    
    struct memblock ext_mem_20217;
    
    ext_mem_20217.references = NULL;
    
    struct memblock mem_20141;
    
    mem_20141.references = NULL;
    
    struct memblock mem_param_20139;
    
    mem_param_20139.references = NULL;
    
    struct memblock mem_param_20135;
    
    mem_param_20135.references = NULL;
    
    struct memblock mem_param_20132;
    
    mem_param_20132.references = NULL;
    
    struct memblock mem_param_20129;
    
    mem_param_20129.references = NULL;
    
    struct memblock ext_mem_20302;
    
    ext_mem_20302.references = NULL;
    
    struct memblock ext_mem_20303;
    
    ext_mem_20303.references = NULL;
    
    struct memblock ext_mem_20304;
    
    ext_mem_20304.references = NULL;
    
    struct memblock ext_mem_20305;
    
    ext_mem_20305.references = NULL;
    
    struct memblock mem_param_20126;
    
    mem_param_20126.references = NULL;
    
    struct memblock mem_param_20123;
    
    mem_param_20123.references = NULL;
    
    struct memblock mem_param_20120;
    
    mem_param_20120.references = NULL;
    
    struct memblock mem_param_20116;
    
    mem_param_20116.references = NULL;
    
    struct memblock ext_mem_20315;
    
    ext_mem_20315.references = NULL;
    
    struct memblock ext_mem_20316;
    
    ext_mem_20316.references = NULL;
    
    struct memblock ext_mem_20317;
    
    ext_mem_20317.references = NULL;
    
    struct memblock ext_mem_20318;
    
    ext_mem_20318.references = NULL;
    
    struct memblock mem_20113;
    
    mem_20113.references = NULL;
    
    struct memblock mem_20112;
    
    mem_20112.references = NULL;
    
    struct memblock mem_20111;
    
    mem_20111.references = NULL;
    
    struct memblock mem_20110;
    
    mem_20110.references = NULL;
    
    struct memblock mem_20109;
    
    mem_20109.references = NULL;
    
    struct memblock mem_out_20506;
    
    mem_out_20506.references = NULL;
    
    int64_t prim_out_20507;
    
    if (memblock_alloc(ctx, &mem_20109, (int64_t) 16, "mem_20109")) {
        err = 1;
        goto cleanup;
    }
    
    struct memblock mainzistatic_array_20508 = (struct memblock) {NULL, (unsigned char *) mainzistatic_array_realtype_20637, 0, "main.static_array_20508"};
    
    lmad_copy_1b(ctx, 1, (uint8_t *) mem_20109.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint8_t *) mainzistatic_array_20508.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {(int64_t) 16});
    if (memblock_alloc(ctx, &mem_20110, (int64_t) 64, "mem_20110")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t i_20509 = 0; i_20509 < (int64_t) 8; i_20509++) {
        int64_t x_20510 = (int64_t) 0 + i_20509 * (int64_t) 1;
        
        ((int64_t *) mem_20110.mem)[i_20509] = x_20510;
    }
    if (memblock_alloc(ctx, &mem_20111, (int64_t) 8, "mem_20111")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t nest_i_20511 = 0; nest_i_20511 < (int64_t) 1; nest_i_20511++) {
        ((int64_t *) mem_20111.mem)[nest_i_20511] = (int64_t) 0;
    }
    if (memblock_alloc(ctx, &mem_20112, (int64_t) 8, "mem_20112")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t nest_i_20512 = 0; nest_i_20512 < (int64_t) 1; nest_i_20512++) {
        ((int64_t *) mem_20112.mem)[nest_i_20512] = (int64_t) 8;
    }
    
    bool loop_cond_t_res_18650 = slt32(0, max_depth_16888);
    
    if (memblock_alloc(ctx, &mem_20113, (int64_t) 0, "mem_20113")) {
        err = 1;
        goto cleanup;
    }
    if (mem_20196_cached_sizze_20617 < (int64_t) 2) {
        err = lexical_realloc(ctx, &mem_20196, &mem_20196_cached_sizze_20617, (int64_t) 2);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    
    int64_t partition_and_deepen_res_18652;
    bool partition_and_deepen_res_18653;
    int32_t partition_and_deepen_res_18654;
    int64_t loop_dz2087U_18659;
    bool loop_while_18660;
    int32_t p_18661;
    
    if (memblock_set(ctx, &mem_param_20116, &mem_20110, "mem_20110") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_20120, &mem_20109, "mem_20109") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_20123, &mem_20111, "mem_20111") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_20126, &mem_20112, "mem_20112") != 0)
        return 1;
    loop_dz2087U_18659 = (int64_t) 1;
    loop_while_18660 = loop_cond_t_res_18650;
    p_18661 = 0;
    while (loop_while_18660) {
        bool loop_nonempty_19245 = slt64((int64_t) 0, loop_dz2087U_18659);
        int32_t newDepth_18697 = add32(1, p_18661);
        int32_t new_i_18698 = mul32(4, p_18661);
        int32_t zm_lhs_18699 = mul32(4, newDepth_18697);
        int32_t new_j_18700 = sub32(zm_lhs_18699, 1);
        int32_t tmp_18701 = add32(2, new_i_18698);
        int64_t i_18702 = sext_i32_i64(new_i_18698);
        int64_t j_18703 = sext_i32_i64(new_j_18700);
        int64_t tmp_18704 = sext_i32_i64(tmp_18701);
        bool step_zzero_18705 = new_i_18698 == tmp_18701;
        bool bounds_invalid_upwards_18706 = slt32(new_j_18700, new_i_18698);
        bool range_invalid_18710 = step_zzero_18705 || bounds_invalid_upwards_18706;
        bool valid_18711 = !range_invalid_18710;
        bool loop_not_taken_19246 = !loop_nonempty_19245;
        bool protect_assert_disj_19247 = valid_18711 || loop_not_taken_19246;
        bool range_valid_c_18712;
        
        if (!protect_assert_disj_19247) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) i_18702, "..", (long long) tmp_18704, "...", (long long) j_18703, " is invalid.", "-> #0  ftHashJoin.fut:65:15-34\n   #1  ftHashJoin.fut:112:20-118:6\n   #2  ftHashJoin.fut:185:6-89\n   #3  ftHashJoin.fut:354:1-363:20\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int32_t distance_upwards_exclusive_18707 = sub32(new_j_18700, new_i_18698);
        int64_t distance_exclusive_18708 = sext_i32_i64(distance_upwards_exclusive_18707);
        int64_t distance_18709 = add64((int64_t) 1, distance_exclusive_18708);
        int64_t num_elems_18713 = sdiv_up64(distance_18709, (int64_t) 2);
        int64_t loopres_18666;
        int64_t loop_dz2081Uz2089U_18672;
        
        if (memblock_set(ctx, &mem_param_20129, &mem_20113, "mem_20113") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_20132, &mem_20113, "mem_20113") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_20135, &mem_param_20116, "mem_param_20116") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_20139, &mem_param_20120, "mem_param_20120") != 0)
            return 1;
        loop_dz2081Uz2089U_18672 = (int64_t) 0;
        for (int64_t i_18671 = 0; i_18671 < loop_dz2087U_18659; i_18671++) {
            int64_t bounds_18677 = ((int64_t *) mem_param_20123.mem)[i_18671];
            int64_t bounds_18678 = ((int64_t *) mem_param_20126.mem)[i_18671];
            int64_t dzlz7bUZLzmZRz20Uboundszi1z20Uboundszi0z7dUzg_18679 = sub64(bounds_18678, bounds_18677);
            bool empty_slice_18680 = dzlz7bUZLzmZRz20Uboundszi1z20Uboundszi0z7dUzg_18679 == (int64_t) 0;
            int64_t m_18681 = sub64(dzlz7bUZLzmZRz20Uboundszi1z20Uboundszi0z7dUzg_18679, (int64_t) 1);
            int64_t i_p_m_t_s_18682 = add64(bounds_18677, m_18681);
            bool zzero_leq_i_p_m_t_s_18683 = sle64((int64_t) 0, i_p_m_t_s_18682);
            bool i_p_m_t_s_leq_w_18684 = slt64(i_p_m_t_s_18682, (int64_t) 8);
            bool zzero_lte_i_18685 = sle64((int64_t) 0, bounds_18677);
            bool i_lte_j_18686 = sle64(bounds_18677, bounds_18678);
            bool y_18687 = i_p_m_t_s_leq_w_18684 && zzero_lte_i_18685;
            bool y_18688 = zzero_leq_i_p_m_t_s_18683 && y_18687;
            bool forwards_ok_18689 = i_lte_j_18686 && y_18688;
            bool ok_or_empty_18690 = empty_slice_18680 || forwards_ok_18689;
            bool index_certs_18691;
            
            if (!ok_or_empty_18690) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) bounds_18677, ":", (long long) bounds_18678, "] out of bounds for array of shape [", (long long) (int64_t) 8, "].", "-> #0  ftHashJoin.fut:145:21-47\n   #1  ftHashJoin.fut:185:6-89\n   #2  ftHashJoin.fut:354:1-363:20\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t bytes_20140 = (int64_t) 8 * dzlz7bUZLzmZRz20Uboundszi1z20Uboundszi0z7dUzg_18679;
            
            if (memblock_alloc(ctx, &mem_20141, bytes_20140, "mem_20141")) {
                err = 1;
                goto cleanup;
            }
            for (int64_t nest_i_20533 = 0; nest_i_20533 < dzlz7bUZLzmZRz20Uboundszi1z20Uboundszi0z7dUzg_18679; nest_i_20533++) {
                ((int64_t *) mem_20141.mem)[nest_i_20533] = (int64_t) 0;
            }
            
            int64_t zm_lhs_18716 = add64((int64_t) 256, dzlz7bUZLzmZRz20Uboundszi1z20Uboundszi0z7dUzg_18679);
            int64_t zs_lhs_18717 = sub64(zm_lhs_18716, (int64_t) 1);
            int64_t m_18723 = sdiv_safe64(zs_lhs_18717, (int64_t) 256);
            bool loop_cond_18724 = slt64((int64_t) 0, m_18723);
            int64_t binop_y_20149 = (int64_t) 2 * bounds_18677;
            int64_t bytes_20174 = (int64_t) 2 * dzlz7bUZLzmZRz20Uboundszi1z20Uboundszi0z7dUzg_18679;
            
            if (mem_20153_cached_sizze_20614 < dzlz7bUZLzmZRz20Uboundszi1z20Uboundszi0z7dUzg_18679) {
                err = lexical_realloc(ctx, &mem_20153, &mem_20153_cached_sizze_20614, dzlz7bUZLzmZRz20Uboundszi1z20Uboundszi0z7dUzg_18679);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            
            int64_t ext_20215;
            int64_t ext_20212;
            int64_t ctx_param_ext_20142;
            int64_t ctx_param_ext_20146;
            
            if (memblock_set(ctx, &mem_param_20145, &mem_param_20139, "mem_param_20139") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_20148, &mem_param_20135, "mem_param_20135") != 0)
                return 1;
            ctx_param_ext_20142 = binop_y_20149;
            ctx_param_ext_20146 = bounds_18677;
            for (int64_t i_18727 = 0; i_18727 < num_elems_18713; i_18727++) {
                int32_t binop_x_18730 = sext_i64_i32(i_18727);
                int32_t binop_x_18731 = mul32(2, binop_x_18730);
                int32_t index_primexp_18732 = add32(new_i_18698, binop_x_18731);
                int32_t zm_lhs_18733 = add32(2, index_primexp_18732);
                int32_t min_arg1_18734 = sub32(zm_lhs_18733, 1);
                int32_t min_res_18735 = smin32(new_j_18700, min_arg1_18734);
                int32_t zp_lhs_18736 = sub32(min_res_18735, index_primexp_18732);
                int32_t ij_bits_18737 = add32(1, zp_lhs_18736);
                int32_t up_to_18738 = shl32(1, ij_bits_18737);
                int64_t tmp_18739 = sext_i32_i64(zp_lhs_18736);
                bool bounds_invalid_upwards_18740 = slt32(zp_lhs_18736, 0);
                bool valid_18742 = !bounds_invalid_upwards_18740;
                bool range_valid_c_18743;
                
                if (!valid_18742) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "...", (long long) tmp_18739, " is invalid.", "-> #0  ftbasics.fut:113:17-26\n   #1  /prelude/functional.fut:9:44-45\n   #2  ftHashJoin.fut:53:87-90\n   #3  ftHashJoin.fut:66:6-70\n   #4  ftHashJoin.fut:112:20-118:6\n   #5  ftHashJoin.fut:185:6-89\n   #6  ftHashJoin.fut:354:1-363:20\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                int64_t up_to_18753 = sext_i32_i64(up_to_18738);
                bool bounds_invalid_upwards_18754 = slt32(up_to_18738, 0);
                bool valid_18755 = !bounds_invalid_upwards_18754;
                bool range_valid_c_18756;
                
                if (!valid_18755) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "..<", (long long) up_to_18753, " is invalid.", "-> #0  ftbasics.fut:137:21-30\n   #1  ftHashJoin.fut:53:87-90\n   #2  ftHashJoin.fut:66:6-70\n   #3  ftHashJoin.fut:112:20-118:6\n   #4  ftHashJoin.fut:185:6-89\n   #5  ftHashJoin.fut:354:1-363:20\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                int64_t distance_18741 = add64((int64_t) 1, tmp_18739);
                
                for (int64_t i_19955 = 0; i_19955 < dzlz7bUZLzmZRz20Uboundszi1z20Uboundszi0z7dUzg_18679; i_19955++) {
                    int8_t defunc_0_f_res_18746;
                    int8_t y_18748 = (int8_t) 0;
                    
                    for (int64_t i_18747 = 0; i_18747 < distance_18741; i_18747++) {
                        int32_t binop_x_18749 = sext_i64_i32(i_18747);
                        int32_t get_bit_arg0_18750 = add32(index_primexp_18732, binop_x_18749);
                        int32_t zm_rhs_19342 = sdiv32(get_bit_arg0_18750, 8);
                        int32_t zm_lhs_19343 = sub32(2, zm_rhs_19342);
                        int32_t whichByte_19344 = sub32(zm_lhs_19343, 1);
                        int64_t whichByte_19346 = sext_i32_i64(whichByte_19344);
                        bool x_19347 = sle64((int64_t) 0, whichByte_19346);
                        bool y_19348 = slt64(whichByte_19346, (int64_t) 2);
                        bool bounds_check_19349 = x_19347 && y_19348;
                        bool index_certs_19350;
                        
                        if (!bounds_check_19349) {
                            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) whichByte_19346, "] out of bounds for array of shape [", (long long) (int64_t) 2, "].", "-> #0  ftHashJoin.fut:27:6-18\n"));
                            err = FUTHARK_PROGRAM_ERROR;
                            goto cleanup;
                        }
                        
                        int32_t whichBit_19345 = smod32(get_bit_arg0_18750, 8);
                        int8_t zbzg_lhs_19351 = ((int8_t *) mem_param_20145.mem)[ctx_param_ext_20142 + (i_19955 * (int64_t) 2 + whichByte_19346)];
                        int8_t unsign_arg0_19352 = zext_i32_i8(whichBit_19345);
                        int8_t unsign_arg0_19353 = ashr8(zbzg_lhs_19351, unsign_arg0_19352);
                        int8_t unsign_arg0_19354 = (int8_t) 1 & unsign_arg0_19353;
                        int32_t to_i32_res_19355 = zext_i8_i32(unsign_arg0_19354);
                        int8_t loopres_18752;
                        
                        if (futrts_set_bit_2464(ctx, &loopres_18752, binop_x_18749, y_18748, to_i32_res_19355) != 0) {
                            err = 1;
                            goto cleanup;
                        }
                        
                        int8_t y_tmp_20541 = loopres_18752;
                        
                        y_18748 = y_tmp_20541;
                    }
                    defunc_0_f_res_18746 = y_18748;
                    ((int8_t *) mem_20153)[i_19955] = defunc_0_f_res_18746;
                }
                
                int64_t defunc_0_radix_sort_multistep_res_18757;
                int64_t offs_18760;
                
                if (memblock_set(ctx, &mem_param_20162, &mem_20141, "mem_20141") != 0)
                    return 1;
                offs_18760 = (int64_t) 0;
                for (int64_t i_18759 = 0; i_18759 < up_to_18753; i_18759++) {
                    int32_t binop_x_18762 = sext_i64_i32(i_18759);
                    int8_t index_primexp_18763 = zext_i32_i8(binop_x_18762);
                    
                    if (memblock_alloc(ctx, &mem_20164, bytes_20140, "mem_20164")) {
                        err = 1;
                        goto cleanup;
                    }
                    
                    int64_t inpacc_19900;
                    int64_t defunc_0_reduce_res_19901;
                    int64_t inpacc_19635;
                    int64_t inpacc_19636;
                    
                    inpacc_19635 = (int64_t) 0;
                    inpacc_19636 = (int64_t) 0;
                    for (int64_t i_19973 = 0; i_19973 < dzlz7bUZLzmZRz20Uboundszi1z20Uboundszi0z7dUzg_18679; i_19973++) {
                        int8_t eta_p_20087 = ((int8_t *) mem_20153)[i_19973];
                        bool lifted_lambda_res_20088 = eta_p_20087 == index_primexp_18763;
                        int64_t bool_res_20089 = btoi_bool_i64(lifted_lambda_res_20088);
                        int64_t eta_p_20102 = ((int64_t *) mem_param_20162.mem)[i_19973];
                        int64_t defunc_0_op_res_20103 = add64(inpacc_19635, bool_res_20089);
                        int64_t zm_lhs_20104 = add64(offs_18760, defunc_0_op_res_20103);
                        int64_t lifted_lambda_res_20105 = sub64(zm_lhs_20104, (int64_t) 1);
                        int64_t defunc_0_f_res_20106 = mul64(bool_res_20089, lifted_lambda_res_20105);
                        int64_t defunc_0_f_res_20107 = add64(eta_p_20102, defunc_0_f_res_20106);
                        int64_t defunc_0_op_res_19684 = add64(inpacc_19636, bool_res_20089);
                        
                        ((int64_t *) mem_20164.mem)[i_19973] = defunc_0_f_res_20107;
                        
                        int64_t inpacc_tmp_20545 = defunc_0_op_res_20103;
                        int64_t inpacc_tmp_20546 = defunc_0_op_res_19684;
                        
                        inpacc_19635 = inpacc_tmp_20545;
                        inpacc_19636 = inpacc_tmp_20546;
                    }
                    inpacc_19900 = inpacc_19635;
                    defunc_0_reduce_res_19901 = inpacc_19636;
                    
                    int64_t tmp_18788 = add64(offs_18760, defunc_0_reduce_res_19901);
                    
                    if (memblock_set(ctx, &mem_param_tmp_20542, &mem_20164, "mem_20164") != 0)
                        return 1;
                    
                    int64_t offs_tmp_20543 = tmp_18788;
                    
                    if (memblock_set(ctx, &mem_param_20162, &mem_param_tmp_20542, "mem_param_tmp_20542") != 0)
                        return 1;
                    offs_18760 = offs_tmp_20543;
                }
                if (memblock_set(ctx, &ext_mem_20173, &mem_param_20162, "mem_param_20162") != 0)
                    return 1;
                defunc_0_radix_sort_multistep_res_18757 = offs_18760;
                if (memblock_alloc(ctx, &mem_20175, bytes_20174, "mem_20175")) {
                    err = 1;
                    goto cleanup;
                }
                lmad_copy_1b(ctx, 2, (uint8_t *) mem_20175.mem, (int64_t) 0, (int64_t []) {(int64_t) 2, (int64_t) 1}, (uint8_t *) mem_param_20145.mem, ctx_param_ext_20142, (int64_t []) {(int64_t) 2, (int64_t) 1}, (int64_t []) {dzlz7bUZLzmZRz20Uboundszi1z20Uboundszi0z7dUzg_18679, (int64_t) 2});
                if (memblock_alloc(ctx, &mem_20177, bytes_20140, "mem_20177")) {
                    err = 1;
                    goto cleanup;
                }
                lmad_copy_8b(ctx, 1, (uint64_t *) mem_20177.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_20148.mem, ctx_param_ext_20146, (int64_t []) {(int64_t) 1}, (int64_t []) {dzlz7bUZLzmZRz20Uboundszi1z20Uboundszi0z7dUzg_18679});
                
                bool partitioned_scatter_res_18795;
                int64_t partitioned_scatter_res_18798;
                bool loop_while_18799;
                int64_t p_18802;
                
                loop_while_18799 = loop_cond_18724;
                p_18802 = (int64_t) 0;
                while (loop_while_18799) {
                    int64_t lower_bound_18803 = mul64((int64_t) 256, p_18802);
                    int64_t min_arg1_18804 = add64((int64_t) 256, lower_bound_18803);
                    int64_t min_res_18805 = smin64(dzlz7bUZLzmZRz20Uboundszi1z20Uboundszi0z7dUzg_18679, min_arg1_18804);
                    int64_t j_m_i_18806 = sub64(min_res_18805, lower_bound_18803);
                    bool empty_slice_18807 = j_m_i_18806 == (int64_t) 0;
                    int64_t m_18808 = sub64(j_m_i_18806, (int64_t) 1);
                    int64_t i_p_m_t_s_18809 = add64(lower_bound_18803, m_18808);
                    bool zzero_leq_i_p_m_t_s_18810 = sle64((int64_t) 0, i_p_m_t_s_18809);
                    bool i_p_m_t_s_leq_w_18811 = slt64(i_p_m_t_s_18809, dzlz7bUZLzmZRz20Uboundszi1z20Uboundszi0z7dUzg_18679);
                    bool zzero_lte_i_18812 = sle64((int64_t) 0, lower_bound_18803);
                    bool i_lte_j_18813 = sle64(lower_bound_18803, min_res_18805);
                    bool y_18814 = i_p_m_t_s_leq_w_18811 && zzero_lte_i_18812;
                    bool y_18815 = zzero_leq_i_p_m_t_s_18810 && y_18814;
                    bool forwards_ok_18816 = i_lte_j_18813 && y_18815;
                    bool ok_or_empty_18817 = empty_slice_18807 || forwards_ok_18816;
                    bool index_certs_18818;
                    
                    if (!ok_or_empty_18817) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) lower_bound_18803, ":", (long long) min_res_18805, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20Uboundszi1z20Uboundszi0z7dUzg_18679, "].", "-> #0  ftbasics.fut:72:25-56\n   #1  ftHashJoin.fut:53:87-90\n   #2  ftHashJoin.fut:66:6-70\n   #3  ftHashJoin.fut:112:20-118:6\n   #4  ftHashJoin.fut:185:6-89\n   #5  ftHashJoin.fut:354:1-363:20\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t bytes_20185 = (int64_t) 2 * j_m_i_18806;
                    int64_t bytes_20187 = (int64_t) 8 * j_m_i_18806;
                    
                    if (mem_20186_cached_sizze_20615 < bytes_20185) {
                        err = lexical_realloc(ctx, &mem_20186, &mem_20186_cached_sizze_20615, bytes_20185);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    lmad_copy_1b(ctx, 2, (uint8_t *) mem_20186, (int64_t) 0, (int64_t []) {(int64_t) 2, (int64_t) 1}, (uint8_t *) mem_20175.mem, (int64_t) 0 + (int64_t) 2 * lower_bound_18803, (int64_t []) {(int64_t) 2, (int64_t) 1}, (int64_t []) {j_m_i_18806, (int64_t) 2});
                    if (mem_20188_cached_sizze_20616 < bytes_20187) {
                        err = lexical_realloc(ctx, &mem_20188, &mem_20188_cached_sizze_20616, bytes_20187);
                        if (err != FUTHARK_SUCCESS)
                            goto cleanup;
                    }
                    lmad_copy_8b(ctx, 1, (uint64_t *) mem_20188, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_20177.mem, (int64_t) 0 + (int64_t) 1 * lower_bound_18803, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_18806});
                    for (int64_t write_iter_19975 = 0; write_iter_19975 < dzlz7bUZLzmZRz20Uboundszi1z20Uboundszi0z7dUzg_18679; write_iter_19975++) {
                        int64_t write_iv_19978 = ((int64_t *) ext_mem_20173.mem)[write_iter_19975];
                        int64_t write_iv_19980 = ((int64_t *) mem_param_20148.mem)[ctx_param_ext_20146 + write_iter_19975];
                        
                        lmad_copy_1b(ctx, 1, (uint8_t *) mem_20196, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint8_t *) mem_param_20145.mem, ctx_param_ext_20142 + write_iter_19975 * (int64_t) 2, (int64_t []) {(int64_t) 1}, (int64_t []) {(int64_t) 2});
                        
                        bool cond_19727 = sle64(lower_bound_18803, write_iv_19978);
                        bool cond_t_res_19728 = slt64(write_iv_19978, min_res_18805);
                        bool x_19729 = cond_19727 && cond_t_res_19728;
                        int64_t lifted_lambda_res_19730;
                        
                        if (x_19729) {
                            int64_t lifted_lambda_res_t_res_19903 = sub64(write_iv_19978, lower_bound_18803);
                            
                            lifted_lambda_res_19730 = lifted_lambda_res_t_res_19903;
                        } else {
                            lifted_lambda_res_19730 = (int64_t) -1;
                        }
                        if ((sle64((int64_t) 0, lifted_lambda_res_19730) && slt64(lifted_lambda_res_19730, j_m_i_18806)) && (sle64((int64_t) 0, (int64_t) 0) && slt64((int64_t) 1, (int64_t) 2))) {
                            lmad_copy_1b(ctx, 1, (uint8_t *) mem_20186, lifted_lambda_res_19730 * (int64_t) 2, (int64_t []) {(int64_t) 1}, (uint8_t *) mem_20196, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {(int64_t) 2});
                        }
                        if (sle64((int64_t) 0, lifted_lambda_res_19730) && slt64(lifted_lambda_res_19730, j_m_i_18806)) {
                            ((int64_t *) mem_20188)[lifted_lambda_res_19730] = write_iv_19980;
                        }
                    }
                    
                    int64_t tmp_18830 = add64((int64_t) 1, p_18802);
                    
                    lmad_copy_1b(ctx, 2, (uint8_t *) mem_20175.mem, (int64_t) 2 * lower_bound_18803, (int64_t []) {(int64_t) 2, (int64_t) 1}, (uint8_t *) mem_20186, (int64_t) 0, (int64_t []) {(int64_t) 2, (int64_t) 1}, (int64_t []) {j_m_i_18806, (int64_t) 2});
                    lmad_copy_8b(ctx, 1, (uint64_t *) mem_20177.mem, lower_bound_18803, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_20188, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_18806});
                    
                    bool loop_cond_18838 = slt64(tmp_18830, m_18723);
                    bool loop_while_tmp_20548 = loop_cond_18838;
                    int64_t p_tmp_20551 = tmp_18830;
                    
                    loop_while_18799 = loop_while_tmp_20548;
                    p_18802 = p_tmp_20551;
                }
                partitioned_scatter_res_18795 = loop_while_18799;
                partitioned_scatter_res_18798 = p_18802;
                if (memblock_unref(ctx, &ext_mem_20173, "ext_mem_20173") != 0)
                    return 1;
                if (memblock_set(ctx, &mem_param_tmp_20534, &mem_20175, "mem_20175") != 0)
                    return 1;
                if (memblock_set(ctx, &mem_param_tmp_20535, &mem_20177, "mem_20177") != 0)
                    return 1;
                
                int64_t ctx_param_ext_tmp_20536 = (int64_t) 0;
                int64_t ctx_param_ext_tmp_20537 = (int64_t) 0;
                
                if (memblock_set(ctx, &mem_param_20145, &mem_param_tmp_20534, "mem_param_tmp_20534") != 0)
                    return 1;
                if (memblock_set(ctx, &mem_param_20148, &mem_param_tmp_20535, "mem_param_tmp_20535") != 0)
                    return 1;
                ctx_param_ext_20142 = ctx_param_ext_tmp_20536;
                ctx_param_ext_20146 = ctx_param_ext_tmp_20537;
            }
            if (memblock_set(ctx, &ext_mem_20217, &mem_param_20145, "mem_param_20145") != 0)
                return 1;
            if (memblock_set(ctx, &ext_mem_20216, &mem_param_20148, "mem_param_20148") != 0)
                return 1;
            ext_20215 = ctx_param_ext_20142;
            ext_20212 = ctx_param_ext_20146;
            if (memblock_unref(ctx, &mem_20141, "mem_20141") != 0)
                return 1;
            if (memblock_alloc(ctx, &mem_20219, bytes_20174, "mem_20219")) {
                err = 1;
                goto cleanup;
            }
            lmad_copy_1b(ctx, 2, (uint8_t *) mem_20219.mem, (int64_t) 0, (int64_t []) {(int64_t) 2, (int64_t) 1}, (uint8_t *) ext_mem_20217.mem, ext_20215, (int64_t []) {(int64_t) 2, (int64_t) 1}, (int64_t []) {dzlz7bUZLzmZRz20Uboundszi1z20Uboundszi0z7dUzg_18679, (int64_t) 2});
            
            int64_t deepen_step_res_18839;
            int64_t deepen_step_res_18840;
            int32_t deepen_step_res_18843;
            int32_t deepen_step_res_18844;
            int32_t deepen_step_res_18845;
            
            if (futrts_getPartitionBounds_8485(ctx, &ext_mem_20222, &ext_mem_20221, &deepen_step_res_18839, &deepen_step_res_18840, &deepen_step_res_18843, &deepen_step_res_18844, &deepen_step_res_18845, mem_20219, dzlz7bUZLzmZRz20Uboundszi1z20Uboundszi0z7dUzg_18679, (int64_t) 2, newDepth_18697, new_i_18698, new_j_18700) != 0) {
                err = 1;
                goto cleanup;
            }
            if (memblock_unref(ctx, &mem_20219, "mem_20219") != 0)
                return 1;
            
            int64_t zl_rhs_18847 = sub64(deepen_step_res_18839, (int64_t) 1);
            int64_t bytes_20223 = (int64_t) 8 * deepen_step_res_18839;
            
            if (mem_20224_cached_sizze_20618 < bytes_20223) {
                err = lexical_realloc(ctx, &mem_20224, &mem_20224_cached_sizze_20618, bytes_20223);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            if (mem_20226_cached_sizze_20619 < bytes_20223) {
                err = lexical_realloc(ctx, &mem_20226, &mem_20226_cached_sizze_20619, bytes_20223);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            if (mem_20228_cached_sizze_20620 < bytes_20223) {
                err = lexical_realloc(ctx, &mem_20228, &mem_20228_cached_sizze_20620, bytes_20223);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            if (mem_20230_cached_sizze_20621 < bytes_20223) {
                err = lexical_realloc(ctx, &mem_20230, &mem_20230_cached_sizze_20621, bytes_20223);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            
            int64_t discard_19997;
            int64_t scanacc_19987 = (int64_t) 0;
            
            for (int64_t i_19992 = 0; i_19992 < deepen_step_res_18839; i_19992++) {
                int64_t tmp_19424 = ((int64_t *) ext_mem_20222.mem)[i_19992];
                bool cond_19425 = slt64(i_19992, zl_rhs_18847);
                int64_t tmp_19426;
                
                if (cond_19425) {
                    int64_t tmp_19907 = add64((int64_t) 1, i_19992);
                    bool x_19908 = sle64((int64_t) 0, tmp_19907);
                    bool y_19909 = slt64(tmp_19907, deepen_step_res_18839);
                    bool bounds_check_19910 = x_19908 && y_19909;
                    bool index_certs_19911;
                    
                    if (!bounds_check_19910) {
                        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_19907, "] out of bounds for array of shape [", (long long) deepen_step_res_18839, "].", "-> #0  ftHashJoin.fut:122:67-81\n   #1  /prelude/functional.fut:9:44-45\n   #2  ftHashJoin.fut:122:8-92\n   #3  ftHashJoin.fut:185:6-89\n   #4  ftHashJoin.fut:354:1-363:20\n"));
                        err = FUTHARK_PROGRAM_ERROR;
                        goto cleanup;
                    }
                    
                    int64_t tmp_t_res_19912 = ((int64_t *) ext_mem_20222.mem)[tmp_19907];
                    
                    tmp_19426 = tmp_t_res_19912;
                } else {
                    tmp_19426 = dzlz7bUZLzmZRz20Uboundszi1z20Uboundszi0z7dUzg_18679;
                }
                
                int64_t zg_lhs_19435 = sub64(tmp_19426, tmp_19424);
                bool lifted_lambda_res_19436 = slt64((int64_t) 2, zg_lhs_19435);
                int64_t defunc_0_f_res_19437 = btoi_bool_i64(lifted_lambda_res_19436);
                int64_t defunc_0_op_res_18874 = add64(defunc_0_f_res_19437, scanacc_19987);
                
                ((int64_t *) mem_20224)[i_19992] = defunc_0_op_res_18874;
                ((int64_t *) mem_20226)[i_19992] = defunc_0_f_res_19437;
                ((int64_t *) mem_20228)[i_19992] = tmp_19424;
                ((int64_t *) mem_20230)[i_19992] = tmp_19426;
                
                int64_t scanacc_tmp_20554 = defunc_0_op_res_18874;
                
                scanacc_19987 = scanacc_tmp_20554;
            }
            discard_19997 = scanacc_19987;
            if (memblock_unref(ctx, &ext_mem_20222, "ext_mem_20222") != 0)
                return 1;
            
            bool cond_18875 = deepen_step_res_18839 == (int64_t) 0;
            bool x_18876 = !cond_18875;
            bool x_18877 = sle64((int64_t) 0, zl_rhs_18847);
            bool y_18878 = slt64(zl_rhs_18847, deepen_step_res_18839);
            bool bounds_check_18879 = x_18877 && y_18878;
            bool protect_assert_disj_18880 = cond_18875 || bounds_check_18879;
            bool index_certs_18881;
            
            if (!protect_assert_disj_18880) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) zl_rhs_18847, "] out of bounds for array of shape [", (long long) deepen_step_res_18839, "].", "-> #0  /prelude/soacs.fut:257:33-47\n   #1  /prelude/functional.fut:9:44-45\n   #2  ftHashJoin.fut:123:8-49\n   #3  ftHashJoin.fut:185:6-89\n   #4  ftHashJoin.fut:354:1-363:20\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t m_f_res_18882;
            
            if (x_18876) {
                int64_t x_19913 = ((int64_t *) mem_20224)[zl_rhs_18847];
                
                m_f_res_18882 = x_19913;
            } else {
                m_f_res_18882 = (int64_t) 0;
            }
            
            int64_t m_18884;
            
            if (cond_18875) {
                m_18884 = (int64_t) 0;
            } else {
                m_18884 = m_f_res_18882;
            }
            
            int64_t m_18894 = sub64(m_18884, (int64_t) 1);
            bool i_p_m_t_s_leq_w_18896 = slt64(m_18894, deepen_step_res_18839);
            bool zzero_leq_i_p_m_t_s_18895 = sle64((int64_t) 0, m_18894);
            bool y_18898 = zzero_leq_i_p_m_t_s_18895 && i_p_m_t_s_leq_w_18896;
            bool i_lte_j_18897 = sle64((int64_t) 0, m_18884);
            bool forwards_ok_18899 = i_lte_j_18897 && y_18898;
            bool eq_x_zz_18891 = (int64_t) 0 == m_f_res_18882;
            bool p_and_eq_x_y_18892 = x_18876 && eq_x_zz_18891;
            bool empty_slice_18893 = cond_18875 || p_and_eq_x_y_18892;
            bool ok_or_empty_18900 = empty_slice_18893 || forwards_ok_18899;
            bool index_certs_18901;
            
            if (!ok_or_empty_18900) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_18884, "] out of bounds for array of shape [", (long long) deepen_step_res_18839, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  ftHashJoin.fut:123:8-49\n   #3  ftHashJoin.fut:185:6-89\n   #4  ftHashJoin.fut:354:1-363:20\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t bytes_20255 = (int64_t) 8 * m_18884;
            int64_t conc_tmp_18923 = loop_dz2081Uz2089U_18672 + m_18884;
            int64_t bytes_20289 = (int64_t) 8 * conc_tmp_18923;
            
            if (mem_20256_cached_sizze_20622 < bytes_20255) {
                err = lexical_realloc(ctx, &mem_20256, &mem_20256_cached_sizze_20622, bytes_20255);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_20256, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_20230, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_18884});
            if (mem_20258_cached_sizze_20623 < bytes_20255) {
                err = lexical_realloc(ctx, &mem_20258, &mem_20258_cached_sizze_20623, bytes_20255);
                if (err != FUTHARK_SUCCESS)
                    goto cleanup;
            }
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_20258, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_20228, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_18884});
            for (int64_t write_iter_19998 = 0; write_iter_19998 < deepen_step_res_18839; write_iter_19998++) {
                int64_t write_iv_20001 = ((int64_t *) mem_20226)[write_iter_19998];
                int64_t write_iv_20002 = ((int64_t *) mem_20224)[write_iter_19998];
                int64_t write_iv_20003 = ((int64_t *) mem_20228)[write_iter_19998];
                int64_t write_iv_20004 = ((int64_t *) mem_20230)[write_iter_19998];
                bool cond_19406 = write_iv_20001 == (int64_t) 1;
                int64_t lifted_lambda_res_19407;
                
                if (cond_19406) {
                    int64_t lifted_lambda_res_t_res_19914 = sub64(write_iv_20002, (int64_t) 1);
                    
                    lifted_lambda_res_19407 = lifted_lambda_res_t_res_19914;
                } else {
                    lifted_lambda_res_19407 = (int64_t) -1;
                }
                if (sle64((int64_t) 0, lifted_lambda_res_19407) && slt64(lifted_lambda_res_19407, m_18884)) {
                    ((int64_t *) mem_20258)[lifted_lambda_res_19407] = write_iv_20003;
                }
                if (sle64((int64_t) 0, lifted_lambda_res_19407) && slt64(lifted_lambda_res_19407, m_18884)) {
                    ((int64_t *) mem_20256)[lifted_lambda_res_19407] = write_iv_20004;
                }
            }
            if (memblock_alloc(ctx, &mem_20271, (int64_t) 16, "mem_20271")) {
                err = 1;
                goto cleanup;
            }
            lmad_copy_1b(ctx, 2, (uint8_t *) mem_20271.mem, (int64_t) 0, (int64_t []) {(int64_t) 2, (int64_t) 1}, (uint8_t *) mem_param_20139.mem, (int64_t) 0, (int64_t []) {(int64_t) 2, (int64_t) 1}, (int64_t []) {(int64_t) 8, (int64_t) 2});
            lmad_copy_1b(ctx, 2, (uint8_t *) mem_20271.mem, (int64_t) 2 * bounds_18677, (int64_t []) {(int64_t) 2, (int64_t) 1}, (uint8_t *) ext_mem_20217.mem, ext_20215, (int64_t []) {(int64_t) 2, (int64_t) 1}, (int64_t []) {dzlz7bUZLzmZRz20Uboundszi1z20Uboundszi0z7dUzg_18679, (int64_t) 2});
            if (memblock_unref(ctx, &ext_mem_20217, "ext_mem_20217") != 0)
                return 1;
            if (memblock_alloc(ctx, &mem_20272, (int64_t) 64, "mem_20272")) {
                err = 1;
                goto cleanup;
            }
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_20272.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_20135.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {(int64_t) 8});
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_20272.mem, bounds_18677, (int64_t []) {(int64_t) 1}, (uint64_t *) ext_mem_20216.mem, ext_20212, (int64_t []) {(int64_t) 1}, (int64_t []) {dzlz7bUZLzmZRz20Uboundszi1z20Uboundszi0z7dUzg_18679});
            if (memblock_unref(ctx, &ext_mem_20216, "ext_mem_20216") != 0)
                return 1;
            if (memblock_alloc(ctx, &mem_20290, bytes_20289, "mem_20290")) {
                err = 1;
                goto cleanup;
            }
            if (memblock_alloc(ctx, &mem_20292, bytes_20289, "mem_20292")) {
                err = 1;
                goto cleanup;
            }
            for (int64_t i_20011 = 0; i_20011 < m_18884; i_20011++) {
                int64_t eta_p_18919 = ((int64_t *) mem_20258)[i_20011];
                int64_t eta_p_18920 = ((int64_t *) mem_20256)[i_20011];
                int64_t tmp_18921 = add64(bounds_18677, eta_p_18919);
                int64_t tmp_18922 = add64(bounds_18677, eta_p_18920);
                
                ((int64_t *) mem_20290.mem)[loop_dz2081Uz2089U_18672 + i_20011] = tmp_18921;
                ((int64_t *) mem_20292.mem)[loop_dz2081Uz2089U_18672 + i_20011] = tmp_18922;
            }
            
            int64_t tmp_offs_20563 = (int64_t) 0;
            
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_20290.mem, tmp_offs_20563, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_20129.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {loop_dz2081Uz2089U_18672});
            tmp_offs_20563 += loop_dz2081Uz2089U_18672;
            if (!(tmp_offs_20563 == loop_dz2081Uz2089U_18672)) {
                lmad_copy_8b(ctx, 1, (uint64_t *) mem_20290.mem, tmp_offs_20563, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_20290.mem, loop_dz2081Uz2089U_18672, (int64_t []) {(int64_t) 1}, (int64_t []) {m_18884});
            }
            tmp_offs_20563 += m_18884;
            
            int64_t tmp_offs_20564 = (int64_t) 0;
            
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_20292.mem, tmp_offs_20564, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_20132.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {loop_dz2081Uz2089U_18672});
            tmp_offs_20564 += loop_dz2081Uz2089U_18672;
            if (!(tmp_offs_20564 == loop_dz2081Uz2089U_18672)) {
                lmad_copy_8b(ctx, 1, (uint64_t *) mem_20292.mem, tmp_offs_20564, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_20292.mem, loop_dz2081Uz2089U_18672, (int64_t []) {(int64_t) 1}, (int64_t []) {m_18884});
            }
            tmp_offs_20564 += m_18884;
            if (memblock_set(ctx, &mem_param_tmp_20524, &mem_20290, "mem_20290") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_tmp_20525, &mem_20292, "mem_20292") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_tmp_20526, &mem_20272, "mem_20272") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_tmp_20527, &mem_20271, "mem_20271") != 0)
                return 1;
            
            int64_t loop_dz2081Uz2089U_tmp_20528 = conc_tmp_18923;
            
            if (memblock_set(ctx, &mem_param_20129, &mem_param_tmp_20524, "mem_param_tmp_20524") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_20132, &mem_param_tmp_20525, "mem_param_tmp_20525") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_20135, &mem_param_tmp_20526, "mem_param_tmp_20526") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_20139, &mem_param_tmp_20527, "mem_param_tmp_20527") != 0)
                return 1;
            loop_dz2081Uz2089U_18672 = loop_dz2081Uz2089U_tmp_20528;
        }
        if (memblock_set(ctx, &ext_mem_20305, &mem_param_20129, "mem_param_20129") != 0)
            return 1;
        if (memblock_set(ctx, &ext_mem_20304, &mem_param_20132, "mem_param_20132") != 0)
            return 1;
        if (memblock_set(ctx, &ext_mem_20303, &mem_param_20135, "mem_param_20135") != 0)
            return 1;
        if (memblock_set(ctx, &ext_mem_20302, &mem_param_20139, "mem_param_20139") != 0)
            return 1;
        loopres_18666 = loop_dz2081Uz2089U_18672;
        
        bool cond_18927 = slt64((int64_t) 0, loopres_18666);
        bool loop_cond_t_res_18928 = slt32(newDepth_18697, max_depth_16888);
        bool x_18929 = cond_18927 && loop_cond_t_res_18928;
        
        if (memblock_set(ctx, &mem_param_tmp_20513, &ext_mem_20303, "ext_mem_20303") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_20514, &ext_mem_20302, "ext_mem_20302") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_20515, &ext_mem_20305, "ext_mem_20305") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_20516, &ext_mem_20304, "ext_mem_20304") != 0)
            return 1;
        
        int64_t loop_dz2087U_tmp_20517 = loopres_18666;
        bool loop_while_tmp_20518 = x_18929;
        int32_t p_tmp_20519 = newDepth_18697;
        
        if (memblock_set(ctx, &mem_param_20116, &mem_param_tmp_20513, "mem_param_tmp_20513") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_20120, &mem_param_tmp_20514, "mem_param_tmp_20514") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_20123, &mem_param_tmp_20515, "mem_param_tmp_20515") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_20126, &mem_param_tmp_20516, "mem_param_tmp_20516") != 0)
            return 1;
        loop_dz2087U_18659 = loop_dz2087U_tmp_20517;
        loop_while_18660 = loop_while_tmp_20518;
        p_18661 = p_tmp_20519;
    }
    if (memblock_set(ctx, &ext_mem_20318, &mem_param_20116, "mem_param_20116") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_20317, &mem_param_20120, "mem_param_20120") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_20316, &mem_param_20123, "mem_param_20123") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_20315, &mem_param_20126, "mem_param_20126") != 0)
        return 1;
    partition_and_deepen_res_18652 = loop_dz2087U_18659;
    partition_and_deepen_res_18653 = loop_while_18660;
    partition_and_deepen_res_18654 = p_18661;
    if (memblock_unref(ctx, &mem_20109, "mem_20109") != 0)
        return 1;
    if (memblock_unref(ctx, &mem_20110, "mem_20110") != 0)
        return 1;
    if (memblock_unref(ctx, &mem_20111, "mem_20111") != 0)
        return 1;
    if (memblock_unref(ctx, &mem_20112, "mem_20112") != 0)
        return 1;
    if (memblock_unref(ctx, &mem_20113, "mem_20113") != 0)
        return 1;
    if (memblock_alloc(ctx, &mem_20319, (int64_t) 16, "mem_20319")) {
        err = 1;
        goto cleanup;
    }
    lmad_copy_1b(ctx, 2, (uint8_t *) mem_20319.mem, (int64_t) 0, (int64_t []) {(int64_t) 2, (int64_t) 1}, (uint8_t *) ext_mem_20317.mem, (int64_t) 0, (int64_t []) {(int64_t) 2, (int64_t) 1}, (int64_t []) {(int64_t) 8, (int64_t) 2});
    
    int64_t calc_partitions_from_partitioned_set_res_18930;
    int64_t calc_partitions_from_partitioned_set_res_18931;
    int32_t calc_partitions_from_partitioned_set_res_18934;
    int32_t calc_partitions_from_partitioned_set_res_18935;
    int32_t calc_partitions_from_partitioned_set_res_18936;
    
    if (futrts_getPartitionBounds_8485(ctx, &ext_mem_20322, &ext_mem_20321, &calc_partitions_from_partitioned_set_res_18930, &calc_partitions_from_partitioned_set_res_18931, &calc_partitions_from_partitioned_set_res_18934, &calc_partitions_from_partitioned_set_res_18935, &calc_partitions_from_partitioned_set_res_18936, mem_20319, (int64_t) 8, (int64_t) 2, 1, 0, 3) != 0) {
        err = 1;
        goto cleanup;
    }
    if (memblock_unref(ctx, &mem_20319, "mem_20319") != 0)
        return 1;
    
    bool cond_18937 = slt32(calc_partitions_from_partitioned_set_res_18934, max_depth_16888);
    int64_t calc_partitions_from_partitioned_set_res_18938;
    int64_t calc_partitions_from_partitioned_set_res_18939;
    bool calc_partitions_from_partitioned_set_res_18940;
    int32_t calc_partitions_from_partitioned_set_res_18943;
    int32_t calc_partitions_from_partitioned_set_res_18944;
    int32_t calc_partitions_from_partitioned_set_res_18945;
    int64_t loop_dz2088U_18946;
    int64_t loop_dz2089U_18947;
    bool loop_while_18948;
    int32_t p_18951;
    int32_t p_18952;
    int32_t p_18953;
    
    if (memblock_set(ctx, &mem_param_20325, &ext_mem_20322, "ext_mem_20322") != 0)
        return 1;
    if (memblock_set(ctx, &mem_param_20328, &ext_mem_20321, "ext_mem_20321") != 0)
        return 1;
    loop_dz2088U_18946 = calc_partitions_from_partitioned_set_res_18930;
    loop_dz2089U_18947 = calc_partitions_from_partitioned_set_res_18931;
    loop_while_18948 = cond_18937;
    p_18951 = calc_partitions_from_partitioned_set_res_18934;
    p_18952 = calc_partitions_from_partitioned_set_res_18935;
    p_18953 = calc_partitions_from_partitioned_set_res_18936;
    while (loop_while_18948) {
        int64_t zl_rhs_18955 = sub64(loop_dz2088U_18946, (int64_t) 1);
        int64_t bytes_20329 = (int64_t) 8 * loop_dz2088U_18946;
        
        if (mem_20330_cached_sizze_20624 < bytes_20329) {
            err = lexical_realloc(ctx, &mem_20330, &mem_20330_cached_sizze_20624, bytes_20329);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_20332_cached_sizze_20625 < bytes_20329) {
            err = lexical_realloc(ctx, &mem_20332, &mem_20332_cached_sizze_20625, bytes_20329);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_20334_cached_sizze_20626 < bytes_20329) {
            err = lexical_realloc(ctx, &mem_20334, &mem_20334_cached_sizze_20626, bytes_20329);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        if (mem_20336_cached_sizze_20627 < bytes_20329) {
            err = lexical_realloc(ctx, &mem_20336, &mem_20336_cached_sizze_20627, bytes_20329);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        
        int64_t discard_20028;
        int64_t scanacc_20018 = (int64_t) 0;
        
        for (int64_t i_20023 = 0; i_20023 < loop_dz2088U_18946; i_20023++) {
            int64_t tmp_19834 = ((int64_t *) mem_param_20325.mem)[i_20023];
            bool cond_19835 = slt64(i_20023, zl_rhs_18955);
            int64_t tmp_19836;
            
            if (cond_19835) {
                int64_t tmp_19923 = add64((int64_t) 1, i_20023);
                bool x_19924 = sle64((int64_t) 0, tmp_19923);
                bool y_19925 = slt64(tmp_19923, loop_dz2088U_18946);
                bool bounds_check_19926 = x_19924 && y_19925;
                bool index_certs_19927;
                
                if (!bounds_check_19926) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_19923, "] out of bounds for array of shape [", (long long) loop_dz2088U_18946, "].", "-> #0  ftHashJoin.fut:200:55-62\n   #1  /prelude/functional.fut:9:44-45\n   #2  ftHashJoin.fut:359:18-78\n   #3  ftHashJoin.fut:354:1-363:20\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                int64_t tmp_t_res_19928 = ((int64_t *) mem_param_20325.mem)[tmp_19923];
                
                tmp_19836 = tmp_t_res_19928;
            } else {
                tmp_19836 = (int64_t) 8;
            }
            
            int64_t zg_lhs_19845 = sub64(tmp_19836, tmp_19834);
            bool lifted_lambda_res_19846 = slt64((int64_t) 2, zg_lhs_19845);
            int64_t defunc_0_f_res_19847 = btoi_bool_i64(lifted_lambda_res_19846);
            int64_t defunc_0_op_res_18982 = add64(defunc_0_f_res_19847, scanacc_20018);
            
            ((int64_t *) mem_20330)[i_20023] = defunc_0_op_res_18982;
            ((int64_t *) mem_20332)[i_20023] = defunc_0_f_res_19847;
            ((int64_t *) mem_20334)[i_20023] = tmp_19834;
            ((int64_t *) mem_20336)[i_20023] = tmp_19836;
            
            int64_t scanacc_tmp_20575 = defunc_0_op_res_18982;
            
            scanacc_20018 = scanacc_tmp_20575;
        }
        discard_20028 = scanacc_20018;
        
        bool cond_18983 = loop_dz2088U_18946 == (int64_t) 0;
        bool x_18984 = !cond_18983;
        bool x_18985 = sle64((int64_t) 0, zl_rhs_18955);
        bool y_18986 = slt64(zl_rhs_18955, loop_dz2088U_18946);
        bool bounds_check_18987 = x_18985 && y_18986;
        bool protect_assert_disj_18988 = cond_18983 || bounds_check_18987;
        bool index_certs_18989;
        
        if (!protect_assert_disj_18988) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) zl_rhs_18955, "] out of bounds for array of shape [", (long long) loop_dz2088U_18946, "].", "-> #0  /prelude/soacs.fut:257:33-47\n   #1  /prelude/functional.fut:9:44-45\n   #2  ftHashJoin.fut:359:18-78\n   #3  ftHashJoin.fut:354:1-363:20\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t m_f_res_18990;
        
        if (x_18984) {
            int64_t x_19929 = ((int64_t *) mem_20330)[zl_rhs_18955];
            
            m_f_res_18990 = x_19929;
        } else {
            m_f_res_18990 = (int64_t) 0;
        }
        
        int64_t m_18992;
        
        if (cond_18983) {
            m_18992 = (int64_t) 0;
        } else {
            m_18992 = m_f_res_18990;
        }
        
        int64_t m_19002 = sub64(m_18992, (int64_t) 1);
        bool i_p_m_t_s_leq_w_19004 = slt64(m_19002, loop_dz2088U_18946);
        bool zzero_leq_i_p_m_t_s_19003 = sle64((int64_t) 0, m_19002);
        bool y_19006 = zzero_leq_i_p_m_t_s_19003 && i_p_m_t_s_leq_w_19004;
        bool i_lte_j_19005 = sle64((int64_t) 0, m_18992);
        bool forwards_ok_19007 = i_lte_j_19005 && y_19006;
        bool eq_x_zz_18999 = (int64_t) 0 == m_f_res_18990;
        bool p_and_eq_x_y_19000 = x_18984 && eq_x_zz_18999;
        bool empty_slice_19001 = cond_18983 || p_and_eq_x_y_19000;
        bool ok_or_empty_19008 = empty_slice_19001 || forwards_ok_19007;
        bool index_certs_19009;
        
        if (!ok_or_empty_19008) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_18992, "] out of bounds for array of shape [", (long long) loop_dz2088U_18946, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n   #2  ftHashJoin.fut:359:18-78\n   #3  ftHashJoin.fut:354:1-363:20\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int64_t bytes_20361 = (int64_t) 8 * m_18992;
        
        if (mem_20362_cached_sizze_20628 < bytes_20361) {
            err = lexical_realloc(ctx, &mem_20362, &mem_20362_cached_sizze_20628, bytes_20361);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        for (int64_t i_20580 = 0; i_20580 < m_18992; i_20580++) {
            int64_t x_20581 = (int64_t) 0 + i_20580 * (int64_t) 1;
            
            ((int64_t *) mem_20362)[i_20580] = x_20581;
        }
        if (mem_20364_cached_sizze_20629 < bytes_20361) {
            err = lexical_realloc(ctx, &mem_20364, &mem_20364_cached_sizze_20629, bytes_20361);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_20364, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_20336, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_18992});
        if (mem_20366_cached_sizze_20630 < bytes_20361) {
            err = lexical_realloc(ctx, &mem_20366, &mem_20366_cached_sizze_20630, bytes_20361);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_20366, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_20334, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_18992});
        for (int64_t write_iter_20029 = 0; write_iter_20029 < loop_dz2088U_18946; write_iter_20029++) {
            int64_t write_iv_20033 = ((int64_t *) mem_20332)[write_iter_20029];
            int64_t write_iv_20034 = ((int64_t *) mem_20330)[write_iter_20029];
            int64_t write_iv_20035 = ((int64_t *) mem_20334)[write_iter_20029];
            int64_t write_iv_20036 = ((int64_t *) mem_20336)[write_iter_20029];
            bool cond_19816 = write_iv_20033 == (int64_t) 1;
            int64_t lifted_lambda_res_19817;
            
            if (cond_19816) {
                int64_t lifted_lambda_res_t_res_19930 = sub64(write_iv_20034, (int64_t) 1);
                
                lifted_lambda_res_19817 = lifted_lambda_res_t_res_19930;
            } else {
                lifted_lambda_res_19817 = (int64_t) -1;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_19817) && slt64(lifted_lambda_res_19817, m_18992)) {
                ((int64_t *) mem_20366)[lifted_lambda_res_19817] = write_iv_20035;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_19817) && slt64(lifted_lambda_res_19817, m_18992)) {
                ((int64_t *) mem_20364)[lifted_lambda_res_19817] = write_iv_20036;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_19817) && slt64(lifted_lambda_res_19817, m_18992)) {
                ((int64_t *) mem_20362)[lifted_lambda_res_19817] = write_iter_20029;
            }
        }
        
        int32_t new_i_19022 = mul32(4, p_18951);
        int32_t zt_rhs_19023 = add32(1, p_18951);
        int32_t zm_lhs_19024 = mul32(4, zt_rhs_19023);
        int32_t new_j_19025 = sub32(zm_lhs_19024, 1);
        int64_t loopres_19026;
        int64_t loopres_19027;
        int32_t loopres_19030;
        int32_t loopres_19031;
        int32_t loopres_19032;
        int64_t loop_dz2084Uz2089U_19034;
        int64_t loop_dz2085Uz2080U_19035;
        int32_t q_19038;
        int32_t q_19039;
        int32_t q_19040;
        
        if (memblock_set(ctx, &mem_param_20387, &mem_param_20325, "mem_param_20325") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_20390, &mem_param_20328, "mem_param_20328") != 0)
            return 1;
        loop_dz2084Uz2089U_19034 = loop_dz2088U_18946;
        loop_dz2085Uz2080U_19035 = loop_dz2089U_18947;
        q_19038 = p_18951;
        q_19039 = p_18952;
        q_19040 = p_18953;
        for (int64_t i_19033 = 0; i_19033 < m_18992; i_19033++) {
            int64_t bounds_19041 = ((int64_t *) mem_20366)[i_19033];
            int64_t bounds_19042 = ((int64_t *) mem_20364)[i_19033];
            int64_t bounds_19043 = ((int64_t *) mem_20362)[i_19033];
            int64_t dzlz7bUZLzmZRz20Uboundszi1z20Uboundszi0z7dUzg_19044 = sub64(bounds_19042, bounds_19041);
            bool empty_slice_19045 = dzlz7bUZLzmZRz20Uboundszi1z20Uboundszi0z7dUzg_19044 == (int64_t) 0;
            int64_t m_19046 = sub64(dzlz7bUZLzmZRz20Uboundszi1z20Uboundszi0z7dUzg_19044, (int64_t) 1);
            int64_t i_p_m_t_s_19047 = add64(bounds_19041, m_19046);
            bool zzero_leq_i_p_m_t_s_19048 = sle64((int64_t) 0, i_p_m_t_s_19047);
            bool i_p_m_t_s_leq_w_19049 = slt64(i_p_m_t_s_19047, (int64_t) 8);
            bool zzero_lte_i_19050 = sle64((int64_t) 0, bounds_19041);
            bool i_lte_j_19051 = sle64(bounds_19041, bounds_19042);
            bool y_19052 = i_p_m_t_s_leq_w_19049 && zzero_lte_i_19050;
            bool y_19053 = zzero_leq_i_p_m_t_s_19048 && y_19052;
            bool forwards_ok_19054 = i_lte_j_19051 && y_19053;
            bool ok_or_empty_19055 = empty_slice_19045 || forwards_ok_19054;
            bool index_certs_19056;
            
            if (!ok_or_empty_19055) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) bounds_19041, ":", (long long) bounds_19042, "] out of bounds for array of shape [", (long long) (int64_t) 8, "].", "-> #0  ftHashJoin.fut:206:21-43\n   #1  ftHashJoin.fut:359:18-78\n   #2  ftHashJoin.fut:354:1-363:20\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t bytes_20391 = (int64_t) 2 * dzlz7bUZLzmZRz20Uboundszi1z20Uboundszi0z7dUzg_19044;
            
            if (memblock_alloc(ctx, &mem_20392, bytes_20391, "mem_20392")) {
                err = 1;
                goto cleanup;
            }
            lmad_copy_1b(ctx, 2, (uint8_t *) mem_20392.mem, (int64_t) 0, (int64_t []) {(int64_t) 2, (int64_t) 1}, (uint8_t *) ext_mem_20317.mem, (int64_t) 0 + (int64_t) 2 * bounds_19041, (int64_t []) {(int64_t) 2, (int64_t) 1}, (int64_t []) {dzlz7bUZLzmZRz20Uboundszi1z20Uboundszi0z7dUzg_19044, (int64_t) 2});
            
            int64_t loopres_19058;
            int64_t loopres_19059;
            int32_t loopres_19062;
            int32_t loopres_19063;
            int32_t loopres_19064;
            
            if (futrts_getPartitionBounds_8485(ctx, &ext_mem_20395, &ext_mem_20394, &loopres_19058, &loopres_19059, &loopres_19062, &loopres_19063, &loopres_19064, mem_20392, dzlz7bUZLzmZRz20Uboundszi1z20Uboundszi0z7dUzg_19044, (int64_t) 2, zt_rhs_19023, new_i_19022, new_j_19025) != 0) {
                err = 1;
                goto cleanup;
            }
            if (memblock_unref(ctx, &mem_20392, "mem_20392") != 0)
                return 1;
            
            bool empty_slice_19065 = bounds_19043 == (int64_t) 0;
            int64_t m_19066 = sub64(bounds_19043, (int64_t) 1);
            bool zzero_leq_i_p_m_t_s_19067 = sle64((int64_t) 0, m_19066);
            bool i_p_m_t_s_leq_w_19068 = slt64(m_19066, loop_dz2084Uz2089U_19034);
            bool i_lte_j_19069 = sle64((int64_t) 0, bounds_19043);
            bool y_19070 = zzero_leq_i_p_m_t_s_19067 && i_p_m_t_s_leq_w_19068;
            bool forwards_ok_19071 = i_lte_j_19069 && y_19070;
            bool ok_or_empty_19072 = empty_slice_19065 || forwards_ok_19071;
            bool index_certs_19073;
            
            if (!ok_or_empty_19072) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) bounds_19043, "] out of bounds for array of shape [", (long long) loop_dz2084Uz2089U_19034, "].", "-> #0  ftHashJoin.fut:215:18-36\n   #1  ftHashJoin.fut:359:18-78\n   #2  ftHashJoin.fut:354:1-363:20\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t conc_tmp_19078 = bounds_19043 + loopres_19058;
            int64_t zpzp_rhs_19079 = add64((int64_t) 1, bounds_19043);
            int64_t j_m_i_19080 = sub64(loop_dz2084Uz2089U_19034, zpzp_rhs_19079);
            bool empty_slice_19081 = j_m_i_19080 == (int64_t) 0;
            int64_t m_19082 = sub64(j_m_i_19080, (int64_t) 1);
            int64_t i_p_m_t_s_19083 = add64(zpzp_rhs_19079, m_19082);
            bool zzero_leq_i_p_m_t_s_19084 = sle64((int64_t) 0, i_p_m_t_s_19083);
            bool i_p_m_t_s_leq_w_19085 = slt64(i_p_m_t_s_19083, loop_dz2084Uz2089U_19034);
            bool zzero_lte_i_19086 = sle64((int64_t) 0, zpzp_rhs_19079);
            bool i_lte_j_19087 = sle64(zpzp_rhs_19079, loop_dz2084Uz2089U_19034);
            bool y_19088 = i_p_m_t_s_leq_w_19085 && zzero_lte_i_19086;
            bool y_19089 = zzero_leq_i_p_m_t_s_19084 && y_19088;
            bool forwards_ok_19090 = i_lte_j_19087 && y_19089;
            bool ok_or_empty_19091 = empty_slice_19081 || forwards_ok_19090;
            bool index_certs_19092;
            
            if (!ok_or_empty_19091) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) zpzp_rhs_19079, ":", (long long) loop_dz2084Uz2089U_19034, "] out of bounds for array of shape [", (long long) loop_dz2084Uz2089U_19034, "].", "-> #0  ftHashJoin.fut:215:90-126\n   #1  ftHashJoin.fut:359:18-78\n   #2  ftHashJoin.fut:354:1-363:20\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t conc_tmp_19094 = conc_tmp_19078 + j_m_i_19080;
            int64_t bytes_20404 = (int64_t) 8 * conc_tmp_19094;
            bool i_p_m_t_s_leq_w_19096 = slt64(m_19066, loop_dz2085Uz2080U_19035);
            bool y_19097 = zzero_leq_i_p_m_t_s_19067 && i_p_m_t_s_leq_w_19096;
            bool ok_or_empty_19098 = empty_slice_19065 || y_19097;
            bool index_certs_19099;
            
            if (!ok_or_empty_19098) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) (int64_t) 0, ":", (long long) bounds_19043, "] out of bounds for array of shape [", (long long) loop_dz2085Uz2080U_19035, "].", "-> #0  ftHashJoin.fut:216:18-36\n   #1  ftHashJoin.fut:359:18-78\n   #2  ftHashJoin.fut:354:1-363:20\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t conc_tmp_19101 = bounds_19043 + loopres_19059;
            int64_t j_m_i_19102 = sub64(loop_dz2085Uz2080U_19035, zpzp_rhs_19079);
            bool empty_slice_19103 = j_m_i_19102 == (int64_t) 0;
            int64_t m_19104 = sub64(j_m_i_19102, (int64_t) 1);
            int64_t i_p_m_t_s_19105 = add64(zpzp_rhs_19079, m_19104);
            bool zzero_leq_i_p_m_t_s_19106 = sle64((int64_t) 0, i_p_m_t_s_19105);
            bool i_p_m_t_s_leq_w_19107 = slt64(i_p_m_t_s_19105, loop_dz2085Uz2080U_19035);
            bool i_lte_j_19108 = sle64(zpzp_rhs_19079, loop_dz2085Uz2080U_19035);
            bool y_19109 = zzero_lte_i_19086 && i_p_m_t_s_leq_w_19107;
            bool y_19110 = zzero_leq_i_p_m_t_s_19106 && y_19109;
            bool forwards_ok_19111 = i_lte_j_19108 && y_19110;
            bool ok_or_empty_19112 = empty_slice_19103 || forwards_ok_19111;
            bool index_certs_19113;
            
            if (!ok_or_empty_19112) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) zpzp_rhs_19079, ":", (long long) loop_dz2085Uz2080U_19035, "] out of bounds for array of shape [", (long long) loop_dz2085Uz2080U_19035, "].", "-> #0  ftHashJoin.fut:216:62-98\n   #1  ftHashJoin.fut:359:18-78\n   #2  ftHashJoin.fut:354:1-363:20\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int64_t conc_tmp_19115 = conc_tmp_19101 + j_m_i_19102;
            int64_t bytes_20406 = (int64_t) 4 * conc_tmp_19115;
            
            if (memblock_alloc(ctx, &mem_20405, bytes_20404, "mem_20405")) {
                err = 1;
                goto cleanup;
            }
            for (int64_t i_20043 = 0; i_20043 < loopres_19058; i_20043++) {
                int64_t eta_p_19076 = ((int64_t *) ext_mem_20395.mem)[i_20043];
                int64_t lifted_lambda_res_19077 = add64(bounds_19041, eta_p_19076);
                
                ((int64_t *) mem_20405.mem)[bounds_19043 + i_20043] = lifted_lambda_res_19077;
            }
            if (memblock_unref(ctx, &ext_mem_20395, "ext_mem_20395") != 0)
                return 1;
            
            int64_t tmp_offs_20595 = (int64_t) 0;
            
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_20405.mem, tmp_offs_20595, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_20387.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {bounds_19043});
            tmp_offs_20595 += bounds_19043;
            if (!(tmp_offs_20595 == bounds_19043)) {
                lmad_copy_8b(ctx, 1, (uint64_t *) mem_20405.mem, tmp_offs_20595, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_20405.mem, bounds_19043, (int64_t []) {(int64_t) 1}, (int64_t []) {loopres_19058});
            }
            tmp_offs_20595 += loopres_19058;
            lmad_copy_8b(ctx, 1, (uint64_t *) mem_20405.mem, tmp_offs_20595, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_param_20387.mem, (int64_t) 0 + (int64_t) 1 * zpzp_rhs_19079, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_19080});
            tmp_offs_20595 += j_m_i_19080;
            if (memblock_alloc(ctx, &mem_20407, bytes_20406, "mem_20407")) {
                err = 1;
                goto cleanup;
            }
            
            int64_t tmp_offs_20596 = (int64_t) 0;
            
            lmad_copy_4b(ctx, 1, (uint32_t *) mem_20407.mem, tmp_offs_20596, (int64_t []) {(int64_t) 1}, (uint32_t *) mem_param_20390.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {bounds_19043});
            tmp_offs_20596 += bounds_19043;
            lmad_copy_4b(ctx, 1, (uint32_t *) mem_20407.mem, tmp_offs_20596, (int64_t []) {(int64_t) 1}, (uint32_t *) ext_mem_20394.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {loopres_19059});
            tmp_offs_20596 += loopres_19059;
            lmad_copy_4b(ctx, 1, (uint32_t *) mem_20407.mem, tmp_offs_20596, (int64_t []) {(int64_t) 1}, (uint32_t *) mem_param_20390.mem, (int64_t) 0 + (int64_t) 1 * zpzp_rhs_19079, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_19102});
            tmp_offs_20596 += j_m_i_19102;
            if (memblock_unref(ctx, &ext_mem_20394, "ext_mem_20394") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_tmp_20585, &mem_20405, "mem_20405") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_tmp_20586, &mem_20407, "mem_20407") != 0)
                return 1;
            
            int64_t loop_dz2084Uz2089U_tmp_20587 = conc_tmp_19094;
            int64_t loop_dz2085Uz2080U_tmp_20588 = conc_tmp_19115;
            int32_t q_tmp_20591 = zt_rhs_19023;
            int32_t q_tmp_20592 = 4;
            int32_t q_tmp_20593 = 2;
            
            if (memblock_set(ctx, &mem_param_20387, &mem_param_tmp_20585, "mem_param_tmp_20585") != 0)
                return 1;
            if (memblock_set(ctx, &mem_param_20390, &mem_param_tmp_20586, "mem_param_tmp_20586") != 0)
                return 1;
            loop_dz2084Uz2089U_19034 = loop_dz2084Uz2089U_tmp_20587;
            loop_dz2085Uz2080U_19035 = loop_dz2085Uz2080U_tmp_20588;
            q_19038 = q_tmp_20591;
            q_19039 = q_tmp_20592;
            q_19040 = q_tmp_20593;
        }
        if (memblock_set(ctx, &ext_mem_20413, &mem_param_20387, "mem_param_20387") != 0)
            return 1;
        if (memblock_set(ctx, &ext_mem_20412, &mem_param_20390, "mem_param_20390") != 0)
            return 1;
        loopres_19026 = loop_dz2084Uz2089U_19034;
        loopres_19027 = loop_dz2085Uz2080U_19035;
        loopres_19030 = q_19038;
        loopres_19031 = q_19039;
        loopres_19032 = q_19040;
        
        bool tmp_19117 = slt64((int64_t) 0, m_18992);
        bool cond_19118 = slt32(loopres_19030, max_depth_16888);
        bool x_19119 = tmp_19117 && cond_19118;
        
        if (memblock_set(ctx, &mem_param_tmp_20565, &ext_mem_20413, "ext_mem_20413") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_tmp_20566, &ext_mem_20412, "ext_mem_20412") != 0)
            return 1;
        
        int64_t loop_dz2088U_tmp_20567 = loopres_19026;
        int64_t loop_dz2089U_tmp_20568 = loopres_19027;
        bool loop_while_tmp_20569 = x_19119;
        int32_t p_tmp_20572 = loopres_19030;
        int32_t p_tmp_20573 = loopres_19031;
        int32_t p_tmp_20574 = loopres_19032;
        
        if (memblock_set(ctx, &mem_param_20325, &mem_param_tmp_20565, "mem_param_tmp_20565") != 0)
            return 1;
        if (memblock_set(ctx, &mem_param_20328, &mem_param_tmp_20566, "mem_param_tmp_20566") != 0)
            return 1;
        loop_dz2088U_18946 = loop_dz2088U_tmp_20567;
        loop_dz2089U_18947 = loop_dz2089U_tmp_20568;
        loop_while_18948 = loop_while_tmp_20569;
        p_18951 = p_tmp_20572;
        p_18952 = p_tmp_20573;
        p_18953 = p_tmp_20574;
    }
    if (memblock_set(ctx, &ext_mem_20419, &mem_param_20325, "mem_param_20325") != 0)
        return 1;
    if (memblock_set(ctx, &ext_mem_20418, &mem_param_20328, "mem_param_20328") != 0)
        return 1;
    calc_partitions_from_partitioned_set_res_18938 = loop_dz2088U_18946;
    calc_partitions_from_partitioned_set_res_18939 = loop_dz2089U_18947;
    calc_partitions_from_partitioned_set_res_18940 = loop_while_18948;
    calc_partitions_from_partitioned_set_res_18943 = p_18951;
    calc_partitions_from_partitioned_set_res_18944 = p_18952;
    calc_partitions_from_partitioned_set_res_18945 = p_18953;
    if (memblock_unref(ctx, &ext_mem_20321, "ext_mem_20321") != 0)
        return 1;
    if (memblock_unref(ctx, &ext_mem_20322, "ext_mem_20322") != 0)
        return 1;
    
    int64_t i32_res_19120 = sext_i32_i64(calc_partitions_from_partitioned_set_res_18944);
    bool nonnegative_19121 = sle64((int64_t) 0, i32_res_19120);
    bool nonzzero_cert_19122;
    
    if (!nonnegative_19121) {
        set_error(ctx, msgprintf("Error: %s\n\nBacktrace:\n%s", "negative exponent", "-> #0  ftHashJoin.fut:255:72-77\n   #1  ftHashJoin.fut:359:7-361:90\n   #2  ftHashJoin.fut:354:1-363:20\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t dzlz7bUZLztztZRz20U2z20Ursz7dUzg_19123 = shl64((int64_t) 1, i32_res_19120);
    int32_t get_radix_arg1_19124 = sub32(calc_partitions_from_partitioned_set_res_18944, 1);
    int64_t tmp_19125 = sext_i32_i64(get_radix_arg1_19124);
    bool bounds_invalid_upwards_19126 = slt32(get_radix_arg1_19124, 0);
    bool valid_19128 = !bounds_invalid_upwards_19126;
    bool range_valid_c_19129;
    
    if (!valid_19128) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 0, "...", (long long) tmp_19125, " is invalid.", "-> #0  ftbasics.fut:113:17-26\n   #1  ftHashJoin.fut:238:63-69\n   #2  /prelude/functional.fut:9:44-45\n   #3  ftHashJoin.fut:359:7-361:90\n   #4  ftHashJoin.fut:354:1-363:20\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t bytes_20420 = (int64_t) 8 * calc_partitions_from_partitioned_set_res_18938;
    int64_t distance_19127 = add64((int64_t) 1, tmp_19125);
    
    if (mem_20421_cached_sizze_20631 < bytes_20420) {
        err = lexical_realloc(ctx, &mem_20421, &mem_20421_cached_sizze_20631, bytes_20420);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    for (int64_t i_20047 = 0; i_20047 < calc_partitions_from_partitioned_set_res_18938; i_20047++) {
        int64_t eta_p_19375 = ((int64_t *) ext_mem_20419.mem)[i_20047];
        bool x_19376 = sle64((int64_t) 0, eta_p_19375);
        bool y_19377 = slt64(eta_p_19375, (int64_t) 8);
        bool bounds_check_19378 = x_19376 && y_19377;
        bool index_certs_19379;
        
        if (!bounds_check_19378) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) eta_p_19375, "] out of bounds for array of shape [", (long long) (int64_t) 8, "].", "-> #0  ftHashJoin.fut:238:63-69\n   #1  /prelude/functional.fut:9:44-45\n   #2  ftHashJoin.fut:359:7-361:90\n   #3  ftHashJoin.fut:354:1-363:20\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        int8_t defunc_0_get_radix_res_19380;
        int8_t y_19382 = (int8_t) 0;
        
        for (int64_t i_19381 = 0; i_19381 < distance_19127; i_19381++) {
            int32_t binop_x_19383 = sext_i64_i32(i_19381);
            int32_t zm_rhs_19384 = sdiv32(binop_x_19383, 8);
            int32_t zm_lhs_19385 = sub32(2, zm_rhs_19384);
            int32_t whichByte_19386 = sub32(zm_lhs_19385, 1);
            int64_t whichByte_19388 = sext_i32_i64(whichByte_19386);
            bool x_19389 = sle64((int64_t) 0, whichByte_19388);
            bool y_19390 = slt64(whichByte_19388, (int64_t) 2);
            bool bounds_check_19391 = x_19389 && y_19390;
            bool index_certs_19392;
            
            if (!bounds_check_19391) {
                set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) whichByte_19388, "] out of bounds for array of shape [", (long long) (int64_t) 2, "].", "-> #0  ftHashJoin.fut:27:6-18\n"));
                err = FUTHARK_PROGRAM_ERROR;
                goto cleanup;
            }
            
            int32_t whichBit_19387 = smod32(binop_x_19383, 8);
            int8_t zbzg_lhs_19393 = ((int8_t *) ext_mem_20317.mem)[eta_p_19375 * (int64_t) 2 + whichByte_19388];
            int8_t unsign_arg0_19394 = zext_i32_i8(whichBit_19387);
            int8_t unsign_arg0_19395 = ashr8(zbzg_lhs_19393, unsign_arg0_19394);
            int8_t unsign_arg0_19396 = (int8_t) 1 & unsign_arg0_19395;
            int32_t to_i32_res_19397 = zext_i8_i32(unsign_arg0_19396);
            int8_t loopres_19398;
            
            if (futrts_set_bit_2464(ctx, &loopres_19398, binop_x_19383, y_19382, to_i32_res_19397) != 0) {
                err = 1;
                goto cleanup;
            }
            
            int8_t y_tmp_20598 = loopres_19398;
            
            y_19382 = y_tmp_20598;
        }
        defunc_0_get_radix_res_19380 = y_19382;
        
        int64_t u8_res_19400 = zext_i8_i64(defunc_0_get_radix_res_19380);
        
        ((int64_t *) mem_20421)[i_20047] = u8_res_19400;
    }
    if (memblock_unref(ctx, &ext_mem_20317, "ext_mem_20317") != 0)
        return 1;
    if (memblock_unref(ctx, &ext_mem_20419, "ext_mem_20419") != 0)
        return 1;
    
    bool cond_19146 = calc_partitions_from_partitioned_set_res_18943 == 1;
    
    if (cond_19146) {
        if (memblock_alloc(ctx, &mem_20442, calc_partitions_from_partitioned_set_res_18938, "mem_20442")) {
            err = 1;
            goto cleanup;
        }
        for (int64_t nest_i_20599 = 0; nest_i_20599 < calc_partitions_from_partitioned_set_res_18938; nest_i_20599++) {
            ((bool *) mem_20442.mem)[nest_i_20599] = 1;
        }
        if (memblock_set(ctx, &ext_mem_20448, &mem_20442, "mem_20442") != 0)
            return 1;
        if (memblock_set(ctx, &ext_mem_20445, &mem_20442, "mem_20442") != 0)
            return 1;
    } else {
        int64_t zeze_rhs_19151 = sub64(calc_partitions_from_partitioned_set_res_18938, (int64_t) 1);
        
        if (memblock_alloc(ctx, &mem_20428, calc_partitions_from_partitioned_set_res_18938, "mem_20428")) {
            err = 1;
            goto cleanup;
        }
        if (memblock_alloc(ctx, &mem_20429, calc_partitions_from_partitioned_set_res_18938, "mem_20429")) {
            err = 1;
            goto cleanup;
        }
        for (int64_t i_20053 = 0; i_20053 < calc_partitions_from_partitioned_set_res_18938; i_20053++) {
            int64_t cur_i_19159 = ((int64_t *) mem_20421)[i_20053];
            bool cond_19160 = i_20053 == (int64_t) 0;
            int64_t pre_i_19161;
            
            if (cond_19160) {
                pre_i_19161 = (int64_t) -1;
            } else {
                int64_t tmp_19162 = sub64(i_20053, (int64_t) 1);
                bool x_19163 = sle64((int64_t) 0, tmp_19162);
                bool y_19164 = slt64(tmp_19162, calc_partitions_from_partitioned_set_res_18938);
                bool bounds_check_19165 = x_19163 && y_19164;
                bool index_certs_19166;
                
                if (!bounds_check_19165) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_19162, "] out of bounds for array of shape [", (long long) calc_partitions_from_partitioned_set_res_18938, "].", "-> #0  ftHashJoin.fut:246:42-74\n   #1  /prelude/functional.fut:9:44-45\n   #2  ftHashJoin.fut:244:10-251:8\n   #3  ftHashJoin.fut:359:7-361:90\n   #4  ftHashJoin.fut:354:1-363:20\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                int64_t pre_i_f_res_19167 = ((int64_t *) mem_20421)[tmp_19162];
                
                pre_i_19161 = pre_i_f_res_19167;
            }
            
            bool cond_19168 = cur_i_19159 == pre_i_19161;
            bool x_19169 = !cond_19168;
            bool cond_19170 = i_20053 == zeze_rhs_19151;
            int64_t pos_i_19171;
            
            if (cond_19170) {
                pos_i_19171 = (int64_t) -1;
            } else {
                int64_t tmp_19172 = add64((int64_t) 1, i_20053);
                bool x_19173 = sle64((int64_t) 0, tmp_19172);
                bool y_19174 = slt64(tmp_19172, calc_partitions_from_partitioned_set_res_18938);
                bool bounds_check_19175 = x_19173 && y_19174;
                bool index_certs_19176;
                
                if (!bounds_check_19175) {
                    set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_19172, "] out of bounds for array of shape [", (long long) calc_partitions_from_partitioned_set_res_18938, "].", "-> #0  ftHashJoin.fut:248:46-78\n   #1  /prelude/functional.fut:9:44-45\n   #2  ftHashJoin.fut:244:10-251:8\n   #3  ftHashJoin.fut:359:7-361:90\n   #4  ftHashJoin.fut:354:1-363:20\n"));
                    err = FUTHARK_PROGRAM_ERROR;
                    goto cleanup;
                }
                
                int64_t pos_i_f_res_19177 = ((int64_t *) mem_20421)[tmp_19172];
                
                pos_i_19171 = pos_i_f_res_19177;
            }
            
            bool cond_19178 = cur_i_19159 == pos_i_19171;
            bool x_19179 = !cond_19178;
            
            ((bool *) mem_20428.mem)[i_20053] = x_19169;
            ((bool *) mem_20429.mem)[i_20053] = x_19179;
        }
        if (memblock_set(ctx, &ext_mem_20448, &mem_20428, "mem_20428") != 0)
            return 1;
        if (memblock_set(ctx, &ext_mem_20445, &mem_20429, "mem_20429") != 0)
            return 1;
    }
    if (mem_20450_cached_sizze_20632 < bytes_20420) {
        err = lexical_realloc(ctx, &mem_20450, &mem_20450_cached_sizze_20632, bytes_20420);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    for (int64_t i_20058 = 0; i_20058 < calc_partitions_from_partitioned_set_res_18938; i_20058++) {
        bool eta_p_19182 = ((bool *) ext_mem_20448.mem)[i_20058];
        int64_t lifted_lambda_res_19184;
        
        if (eta_p_19182) {
            int64_t lifted_lambda_res_t_res_19946 = ((int64_t *) mem_20421)[i_20058];
            
            lifted_lambda_res_19184 = lifted_lambda_res_t_res_19946;
        } else {
            lifted_lambda_res_19184 = (int64_t) -1;
        }
        ((int64_t *) mem_20450)[i_20058] = lifted_lambda_res_19184;
    }
    if (mem_20457_cached_sizze_20633 < dzlz7bUZLztztZRz20U2z20Ursz7dUzg_19123) {
        err = lexical_realloc(ctx, &mem_20457, &mem_20457_cached_sizze_20633, dzlz7bUZLztztZRz20U2z20Ursz7dUzg_19123);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    for (int64_t nest_i_20603 = 0; nest_i_20603 < dzlz7bUZLztztZRz20U2z20Ursz7dUzg_19123; nest_i_20603++) {
        ((bool *) mem_20457)[nest_i_20603] = 1;
    }
    if (mem_20458_cached_sizze_20634 < dzlz7bUZLztztZRz20U2z20Ursz7dUzg_19123) {
        err = lexical_realloc(ctx, &mem_20458, &mem_20458_cached_sizze_20634, dzlz7bUZLztztZRz20U2z20Ursz7dUzg_19123);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    for (int64_t nest_i_20604 = 0; nest_i_20604 < dzlz7bUZLztztZRz20U2z20Ursz7dUzg_19123; nest_i_20604++) {
        ((bool *) mem_20458)[nest_i_20604] = 1;
    }
    
    int64_t zm_lhs_19192 = add64((int64_t) 256, dzlz7bUZLztztZRz20U2z20Ursz7dUzg_19123);
    int64_t zs_lhs_19193 = sub64(zm_lhs_19192, (int64_t) 1);
    int64_t m_19194 = sdiv64(zs_lhs_19193, (int64_t) 256);
    bool loop_cond_19195 = slt64((int64_t) 0, m_19194);
    bool partitioned_scatter_res_19196;
    int64_t partitioned_scatter_res_19199;
    bool loop_while_19200;
    int64_t p_19203;
    
    loop_while_19200 = loop_cond_19195;
    p_19203 = (int64_t) 0;
    while (loop_while_19200) {
        int64_t lower_bound_19204 = mul64((int64_t) 256, p_19203);
        int64_t min_arg1_19205 = add64((int64_t) 256, lower_bound_19204);
        int64_t min_res_19206 = smin64(dzlz7bUZLztztZRz20U2z20Ursz7dUzg_19123, min_arg1_19205);
        int64_t j_m_i_19207 = sub64(min_res_19206, lower_bound_19204);
        bool empty_slice_19208 = j_m_i_19207 == (int64_t) 0;
        int64_t m_19209 = sub64(j_m_i_19207, (int64_t) 1);
        int64_t i_p_m_t_s_19210 = add64(lower_bound_19204, m_19209);
        bool zzero_leq_i_p_m_t_s_19211 = sle64((int64_t) 0, i_p_m_t_s_19210);
        bool i_p_m_t_s_leq_w_19212 = slt64(i_p_m_t_s_19210, dzlz7bUZLztztZRz20U2z20Ursz7dUzg_19123);
        bool zzero_lte_i_19213 = sle64((int64_t) 0, lower_bound_19204);
        bool i_lte_j_19214 = sle64(lower_bound_19204, min_res_19206);
        bool y_19215 = i_p_m_t_s_leq_w_19212 && zzero_lte_i_19213;
        bool y_19216 = zzero_leq_i_p_m_t_s_19211 && y_19215;
        bool forwards_ok_19217 = i_lte_j_19214 && y_19216;
        bool ok_or_empty_19218 = empty_slice_19208 || forwards_ok_19217;
        bool index_certs_19219;
        
        if (!ok_or_empty_19218) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) lower_bound_19204, ":", (long long) min_res_19206, "] out of bounds for array of shape [", (long long) dzlz7bUZLztztZRz20U2z20Ursz7dUzg_19123, "].", "-> #0  ftbasics.fut:72:25-56\n   #1  ftHashJoin.fut:359:7-361:90\n   #2  ftHashJoin.fut:354:1-363:20\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        if (mem_20465_cached_sizze_20635 < j_m_i_19207) {
            err = lexical_realloc(ctx, &mem_20465, &mem_20465_cached_sizze_20635, j_m_i_19207);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_1b(ctx, 1, (uint8_t *) mem_20465, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint8_t *) mem_20457, (int64_t) 0 + (int64_t) 1 * lower_bound_19204, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_19207});
        if (mem_20466_cached_sizze_20636 < j_m_i_19207) {
            err = lexical_realloc(ctx, &mem_20466, &mem_20466_cached_sizze_20636, j_m_i_19207);
            if (err != FUTHARK_SUCCESS)
                goto cleanup;
        }
        lmad_copy_1b(ctx, 1, (uint8_t *) mem_20466, (int64_t) 0, (int64_t []) {(int64_t) 1}, (uint8_t *) mem_20458, (int64_t) 0 + (int64_t) 1 * lower_bound_19204, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_19207});
        for (int64_t write_iter_20060 = 0; write_iter_20060 < calc_partitions_from_partitioned_set_res_18938; write_iter_20060++) {
            int64_t write_iv_20063 = ((int64_t *) mem_20450)[write_iter_20060];
            bool write_iv_20064 = ((bool *) ext_mem_20448.mem)[write_iter_20060];
            bool write_iv_20065 = ((bool *) ext_mem_20445.mem)[write_iter_20060];
            bool cond_19886 = sle64(lower_bound_19204, write_iv_20063);
            bool cond_t_res_19887 = slt64(write_iv_20063, min_res_19206);
            bool x_19888 = cond_19886 && cond_t_res_19887;
            int64_t lifted_lambda_res_19889;
            
            if (x_19888) {
                int64_t lifted_lambda_res_t_res_19947 = sub64(write_iv_20063, lower_bound_19204);
                
                lifted_lambda_res_19889 = lifted_lambda_res_t_res_19947;
            } else {
                lifted_lambda_res_19889 = (int64_t) -1;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_19889) && slt64(lifted_lambda_res_19889, j_m_i_19207)) {
                ((bool *) mem_20465)[lifted_lambda_res_19889] = write_iv_20064;
            }
            if (sle64((int64_t) 0, lifted_lambda_res_19889) && slt64(lifted_lambda_res_19889, j_m_i_19207)) {
                ((bool *) mem_20466)[lifted_lambda_res_19889] = write_iv_20065;
            }
        }
        
        int64_t tmp_19231 = add64((int64_t) 1, p_19203);
        
        lmad_copy_1b(ctx, 1, (uint8_t *) mem_20457, lower_bound_19204, (int64_t []) {(int64_t) 1}, (uint8_t *) mem_20465, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_19207});
        lmad_copy_1b(ctx, 1, (uint8_t *) mem_20458, lower_bound_19204, (int64_t []) {(int64_t) 1}, (uint8_t *) mem_20466, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {j_m_i_19207});
        
        bool loop_cond_19239 = slt64(tmp_19231, m_19194);
        bool loop_while_tmp_20605 = loop_cond_19239;
        int64_t p_tmp_20608 = tmp_19231;
        
        loop_while_19200 = loop_while_tmp_20605;
        p_19203 = p_tmp_20608;
    }
    partitioned_scatter_res_19196 = loop_while_19200;
    partitioned_scatter_res_19199 = p_19203;
    if (memblock_unref(ctx, &ext_mem_20445, "ext_mem_20445") != 0)
        return 1;
    if (memblock_unref(ctx, &ext_mem_20448, "ext_mem_20448") != 0)
        return 1;
    if (memblock_alloc(ctx, &mem_20485, dzlz7bUZLztztZRz20U2z20Ursz7dUzg_19123, "mem_20485")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t i_20070 = 0; i_20070 < dzlz7bUZLztztZRz20U2z20Ursz7dUzg_19123; i_20070++) {
        bool eta_p_19241 = ((bool *) mem_20457)[i_20070];
        bool eta_p_19242 = ((bool *) mem_20458)[i_20070];
        bool x_19243 = eta_p_19241 && eta_p_19242;
        bool lifted_lambda_res_19244 = !x_19243;
        
        ((bool *) mem_20485.mem)[i_20070] = lifted_lambda_res_19244;
    }
    if (memblock_set(ctx, &mem_out_20506, &mem_20485, "mem_20485") != 0)
        return 1;
    prim_out_20507 = dzlz7bUZLztztZRz20U2z20Ursz7dUzg_19123;
    if (memblock_set(ctx, &*mem_out_p_20612, &mem_out_20506, "mem_out_20506") != 0)
        return 1;
    *out_prim_out_20613 = prim_out_20507;
    
  cleanup:
    {
        free(mem_20153);
        free(mem_20186);
        free(mem_20188);
        free(mem_20196);
        free(mem_20224);
        free(mem_20226);
        free(mem_20228);
        free(mem_20230);
        free(mem_20256);
        free(mem_20258);
        free(mem_20330);
        free(mem_20332);
        free(mem_20334);
        free(mem_20336);
        free(mem_20362);
        free(mem_20364);
        free(mem_20366);
        free(mem_20421);
        free(mem_20450);
        free(mem_20457);
        free(mem_20458);
        free(mem_20465);
        free(mem_20466);
        if (memblock_unref(ctx, &mem_20485, "mem_20485") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_20429, "mem_20429") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_20428, "mem_20428") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_20442, "mem_20442") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_20445, "ext_mem_20445") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_20448, "ext_mem_20448") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_20566, "mem_param_tmp_20566") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_20565, "mem_param_tmp_20565") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_20586, "mem_param_tmp_20586") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_20585, "mem_param_tmp_20585") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_20407, "mem_20407") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_20405, "mem_20405") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_20394, "ext_mem_20394") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_20395, "ext_mem_20395") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_20392, "mem_20392") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_20390, "mem_param_20390") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_20387, "mem_param_20387") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_20412, "ext_mem_20412") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_20413, "ext_mem_20413") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_20328, "mem_param_20328") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_20325, "mem_param_20325") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_20418, "ext_mem_20418") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_20419, "ext_mem_20419") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_20321, "ext_mem_20321") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_20322, "ext_mem_20322") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_20319, "mem_20319") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_20516, "mem_param_tmp_20516") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_20515, "mem_param_tmp_20515") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_20514, "mem_param_tmp_20514") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_20513, "mem_param_tmp_20513") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_20527, "mem_param_tmp_20527") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_20526, "mem_param_tmp_20526") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_20525, "mem_param_tmp_20525") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_20524, "mem_param_tmp_20524") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_20292, "mem_20292") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_20290, "mem_20290") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_20272, "mem_20272") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_20271, "mem_20271") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_20221, "ext_mem_20221") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_20222, "ext_mem_20222") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_20219, "mem_20219") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_20535, "mem_param_tmp_20535") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_20534, "mem_param_tmp_20534") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_20177, "mem_20177") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_20175, "mem_20175") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_tmp_20542, "mem_param_tmp_20542") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_20164, "mem_20164") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_20162, "mem_param_20162") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_20173, "ext_mem_20173") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_20148, "mem_param_20148") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_20145, "mem_param_20145") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_20216, "ext_mem_20216") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_20217, "ext_mem_20217") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_20141, "mem_20141") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_20139, "mem_param_20139") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_20135, "mem_param_20135") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_20132, "mem_param_20132") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_20129, "mem_param_20129") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_20302, "ext_mem_20302") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_20303, "ext_mem_20303") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_20304, "ext_mem_20304") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_20305, "ext_mem_20305") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_20126, "mem_param_20126") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_20123, "mem_param_20123") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_20120, "mem_param_20120") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_param_20116, "mem_param_20116") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_20315, "ext_mem_20315") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_20316, "ext_mem_20316") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_20317, "ext_mem_20317") != 0)
            return 1;
        if (memblock_unref(ctx, &ext_mem_20318, "ext_mem_20318") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_20113, "mem_20113") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_20112, "mem_20112") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_20111, "mem_20111") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_20110, "mem_20110") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_20109, "mem_20109") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_20506, "mem_out_20506") != 0)
            return 1;
    }
    return err;
}
FUTHARK_FUN_ATTR int futrts_getPartitionBounds_8485(struct futhark_context *ctx, struct memblock *mem_out_p_20638, struct memblock *mem_out_p_20639, int64_t *out_prim_out_20640, int64_t *out_prim_out_20641, int32_t *out_prim_out_20642, int32_t *out_prim_out_20643, int32_t *out_prim_out_20644, struct memblock pXs_mem_20109, int64_t n_14231, int64_t b_14232, int32_t curDepth_14233, int32_t i_14235, int32_t j_14236)
{
    (void) ctx;
    
    int err = 0;
    int64_t mem_20111_cached_sizze_20645 = 0;
    unsigned char *mem_20111 = NULL;
    int64_t mem_20113_cached_sizze_20646 = 0;
    unsigned char *mem_20113 = NULL;
    int64_t mem_20120_cached_sizze_20647 = 0;
    unsigned char *mem_20120 = NULL;
    int64_t mem_20121_cached_sizze_20648 = 0;
    unsigned char *mem_20121 = NULL;
    int64_t mem_20141_cached_sizze_20649 = 0;
    unsigned char *mem_20141 = NULL;
    struct memblock mem_20152;
    
    mem_20152.references = NULL;
    
    struct memblock mem_20150;
    
    mem_20150.references = NULL;
    
    struct memblock mem_out_20507;
    
    mem_out_20507.references = NULL;
    
    struct memblock mem_out_20506;
    
    mem_out_20506.references = NULL;
    
    int64_t prim_out_20508;
    int64_t prim_out_20509;
    int32_t prim_out_20510;
    int32_t prim_out_20511;
    int32_t prim_out_20512;
    int64_t dzlz7bUZLzmZRz20Unz20U1z7dUzg_14237 = sub64(n_14231, (int64_t) 1);
    bool bounds_invalid_upwards_14241 = slt64(n_14231, (int64_t) 1);
    bool valid_14246 = !bounds_invalid_upwards_14241;
    bool range_valid_c_14247;
    
    if (!valid_14246) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Range ", (long long) (int64_t) 1, "..<", (long long) n_14231, " is invalid.", "-> #0  ftHashJoin.fut:74:23-28\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int32_t zm_rhs_19261 = sdiv32(j_14236, 8);
    int32_t i64_res_19256 = sext_i64_i32(b_14232);
    int32_t zm_lhs_19262 = sub32(i64_res_19256, zm_rhs_19261);
    int32_t i32_arg0_19263 = sub32(zm_lhs_19262, 1);
    int64_t i32_res_19264 = sext_i32_i64(i32_arg0_19263);
    bool y_19289 = slt64(i32_res_19264, b_14232);
    bool x_19288 = sle64((int64_t) 0, i32_res_19264);
    bool bounds_check_19290 = x_19288 && y_19289;
    bool index_certs_19291;
    
    if (!bounds_check_19290) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i32_res_19264, "] out of bounds for array of shape [", (long long) b_14232, "].", "-> #0  ftHashJoin.fut:43:38-54\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int32_t zm_rhs_19257 = sdiv32(i_14235, 8);
    int32_t zm_lhs_19258 = sub32(i64_res_19256, zm_rhs_19257);
    int32_t i32_arg0_19259 = sub32(zm_lhs_19258, 1);
    int64_t i32_res_19260 = sext_i32_i64(i32_arg0_19259);
    bool y_19282 = slt64(i32_res_19260, b_14232);
    bool x_19281 = sle64((int64_t) 0, i32_res_19260);
    bool bounds_check_19283 = x_19281 && y_19282;
    bool index_certs_19284;
    
    if (!bounds_check_19283) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i32_res_19260, "] out of bounds for array of shape [", (long long) b_14232, "].", "-> #0  ftHashJoin.fut:42:48-64\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int32_t lastBit_19266 = smod32(j_14236, 8);
    int32_t zm_lhs_19269 = sub32(8, lastBit_19266);
    int32_t i32_arg0_19270 = sub32(zm_lhs_19269, 1);
    int8_t unsign_arg0_19271 = zext_i32_i8(i32_arg0_19270);
    int8_t unsign_arg0_19272 = lshr8((int8_t) -1, unsign_arg0_19271);
    int32_t firstBit_19265 = smod32(i_14235, 8);
    int8_t unsign_arg0_19267 = zext_i32_i8(firstBit_19265);
    int8_t first_bitMask_19268 = shl8((int8_t) -1, unsign_arg0_19267);
    int64_t bytes_20110 = (int64_t) 8 * dzlz7bUZLzmZRz20Unz20U1z7dUzg_14237;
    
    if (mem_20111_cached_sizze_20645 < bytes_20110) {
        err = lexical_realloc(ctx, &mem_20111, &mem_20111_cached_sizze_20645, bytes_20110);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_20113_cached_sizze_20646 < bytes_20110) {
        err = lexical_realloc(ctx, &mem_20113, &mem_20113_cached_sizze_20646, bytes_20110);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_20120_cached_sizze_20647 < b_14232) {
        err = lexical_realloc(ctx, &mem_20120, &mem_20120_cached_sizze_20647, b_14232);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    if (mem_20121_cached_sizze_20648 < b_14232) {
        err = lexical_realloc(ctx, &mem_20121, &mem_20121_cached_sizze_20648, b_14232);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    
    int64_t discard_19970;
    int64_t scanacc_19964 = (int64_t) 0;
    
    for (int64_t i_19967 = 0; i_19967 < dzlz7bUZLzmZRz20Unz20U1z7dUzg_14237; i_19967++) {
        int64_t index_primexp_20077 = add64((int64_t) 1, i_19967);
        bool x_19444 = sle64((int64_t) 0, index_primexp_20077);
        bool y_19445 = slt64(index_primexp_20077, n_14231);
        bool bounds_check_19446 = x_19444 && y_19445;
        bool index_certs_19447;
        
        if (!bounds_check_19446) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) index_primexp_20077, "] out of bounds for array of shape [", (long long) n_14231, "].", "-> #0  ftHashJoin.fut:75:22-30\n   #1  /prelude/functional.fut:9:44-45\n   #2  ftHashJoin.fut:75:8-44\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        
        bool y_19451 = slt64(i_19967, n_14231);
        bool index_certs_19453;
        
        if (!y_19451) {
            set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) i_19967, "] out of bounds for array of shape [", (long long) n_14231, "].", "-> #0  ftHashJoin.fut:75:32-42\n   #1  /prelude/functional.fut:9:44-45\n   #2  ftHashJoin.fut:75:8-44\n"));
            err = FUTHARK_PROGRAM_ERROR;
            goto cleanup;
        }
        for (int64_t i_19957 = 0; i_19957 < b_14232; i_19957++) {
            int8_t eta_p_19505 = ((int8_t *) pXs_mem_20109.mem)[index_primexp_20077 * b_14232 + i_19957];
            int8_t eta_p_19506 = ((int8_t *) pXs_mem_20109.mem)[i_19967 * b_14232 + i_19957];
            bool cond_19508 = sle64(i_19957, i32_res_19260);
            bool cond_t_res_19509 = sle64(i32_res_19264, i_19957);
            bool x_19510 = cond_19508 && cond_t_res_19509;
            int8_t lifted_lambda_res_19511;
            
            if (x_19510) {
                lifted_lambda_res_19511 = eta_p_19505;
            } else {
                lifted_lambda_res_19511 = (int8_t) 0;
            }
            
            int8_t lifted_lambda_res_19516;
            
            if (x_19510) {
                lifted_lambda_res_19516 = eta_p_19506;
            } else {
                lifted_lambda_res_19516 = (int8_t) 0;
            }
            ((int8_t *) mem_20120)[i_19957] = lifted_lambda_res_19516;
            ((int8_t *) mem_20121)[i_19957] = lifted_lambda_res_19511;
        }
        
        int8_t za_lhs_19464 = ((int8_t *) mem_20121)[i32_res_19260];
        int8_t mod1_x_19465 = first_bitMask_19268 & za_lhs_19464;
        
        ((int8_t *) mem_20121)[i32_res_19260] = mod1_x_19465;
        
        int8_t za_lhs_19467 = ((int8_t *) mem_20121)[i32_res_19264];
        int8_t tmp_19468 = unsign_arg0_19272 & za_lhs_19467;
        
        ((int8_t *) mem_20121)[i32_res_19264] = tmp_19468;
        
        int8_t za_lhs_19477 = ((int8_t *) mem_20120)[i32_res_19260];
        int8_t mod1_x_19478 = first_bitMask_19268 & za_lhs_19477;
        
        ((int8_t *) mem_20120)[i32_res_19260] = mod1_x_19478;
        
        int8_t za_lhs_19480 = ((int8_t *) mem_20120)[i32_res_19264];
        int8_t tmp_19481 = unsign_arg0_19272 & za_lhs_19480;
        
        ((int8_t *) mem_20120)[i32_res_19264] = tmp_19481;
        
        bool defunc_0_reduce_res_19528;
        bool redout_19960 = 0;
        
        for (int64_t i_19961 = 0; i_19961 < b_14232; i_19961++) {
            int8_t eta_p_19498 = ((int8_t *) mem_20121)[i_19961];
            int8_t eta_p_19499 = ((int8_t *) mem_20120)[i_19961];
            bool lifted_lambda_res_19500 = eta_p_19498 == eta_p_19499;
            bool lifted_lambda_res_19501 = !lifted_lambda_res_19500;
            bool defunc_0_op_res_19494 = lifted_lambda_res_19501 || redout_19960;
            bool redout_tmp_20518 = defunc_0_op_res_19494;
            
            redout_19960 = redout_tmp_20518;
        }
        defunc_0_reduce_res_19528 = redout_19960;
        
        int64_t defunc_0_f_res_19495 = btoi_bool_i64(defunc_0_reduce_res_19528);
        int64_t defunc_0_op_res_18078 = add64(defunc_0_f_res_19495, scanacc_19964);
        
        ((int64_t *) mem_20111)[i_19967] = defunc_0_op_res_18078;
        ((int64_t *) mem_20113)[i_19967] = defunc_0_f_res_19495;
        
        int64_t scanacc_tmp_20513 = defunc_0_op_res_18078;
        
        scanacc_19964 = scanacc_tmp_20513;
    }
    discard_19970 = scanacc_19964;
    
    bool cond_18079 = dzlz7bUZLzmZRz20Unz20U1z7dUzg_14237 == (int64_t) 0;
    bool x_18080 = !cond_18079;
    int64_t tmp_18081 = sub64(dzlz7bUZLzmZRz20Unz20U1z7dUzg_14237, (int64_t) 1);
    bool x_18082 = sle64((int64_t) 0, tmp_18081);
    bool y_18083 = slt64(tmp_18081, dzlz7bUZLzmZRz20Unz20U1z7dUzg_14237);
    bool bounds_check_18084 = x_18082 && y_18083;
    bool protect_assert_disj_18085 = cond_18079 || bounds_check_18084;
    bool index_certs_18086;
    
    if (!protect_assert_disj_18085) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [", (long long) tmp_18081, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20Unz20U1z7dUzg_14237, "].", "-> #0  /prelude/soacs.fut:257:33-47\n   #1  /prelude/functional.fut:9:44-45\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t m_f_res_18087;
    
    if (x_18080) {
        int64_t x_19529 = ((int64_t *) mem_20111)[tmp_18081];
        
        m_f_res_18087 = x_19529;
    } else {
        m_f_res_18087 = (int64_t) 0;
    }
    
    int64_t m_18089;
    
    if (cond_18079) {
        m_18089 = (int64_t) 0;
    } else {
        m_18089 = m_f_res_18087;
    }
    
    int64_t m_18099 = sub64(m_18089, (int64_t) 1);
    bool i_p_m_t_s_leq_w_18101 = slt64(m_18099, dzlz7bUZLzmZRz20Unz20U1z7dUzg_14237);
    bool zzero_leq_i_p_m_t_s_18100 = sle64((int64_t) 0, m_18099);
    bool y_18103 = zzero_leq_i_p_m_t_s_18100 && i_p_m_t_s_leq_w_18101;
    bool i_lte_j_18102 = sle64((int64_t) 0, m_18089);
    bool forwards_ok_18104 = i_lte_j_18102 && y_18103;
    bool eq_x_zz_18096 = (int64_t) 0 == m_f_res_18087;
    bool p_and_eq_x_y_18097 = x_18080 && eq_x_zz_18096;
    bool empty_slice_18098 = cond_18079 || p_and_eq_x_y_18097;
    bool ok_or_empty_18105 = empty_slice_18098 || forwards_ok_18104;
    bool index_certs_18106;
    
    if (!ok_or_empty_18105) {
        set_error(ctx, msgprintf("Error: %s%lld%s%lld%s\n\nBacktrace:\n%s", "Index [:", (long long) m_18089, "] out of bounds for array of shape [", (long long) dzlz7bUZLzmZRz20Unz20U1z7dUzg_14237, "].", "-> #0  /prelude/soacs.fut:258:29-35\n   #1  /prelude/functional.fut:9:44-45\n"));
        err = FUTHARK_PROGRAM_ERROR;
        goto cleanup;
    }
    
    int64_t bytes_20140 = (int64_t) 8 * m_18089;
    int64_t conc_tmp_16996 = (int64_t) 1 + m_18089;
    int64_t bytes_20149 = (int64_t) 8 * conc_tmp_16996;
    int64_t bytes_20151 = (int64_t) 4 * conc_tmp_16996;
    
    if (mem_20141_cached_sizze_20649 < bytes_20140) {
        err = lexical_realloc(ctx, &mem_20141, &mem_20141_cached_sizze_20649, bytes_20140);
        if (err != FUTHARK_SUCCESS)
            goto cleanup;
    }
    for (int64_t i_20519 = 0; i_20519 < m_18089; i_20519++) {
        int64_t x_20520 = (int64_t) 1 + i_20519 * (int64_t) 1;
        
        ((int64_t *) mem_20141)[i_20519] = x_20520;
    }
    for (int64_t write_iter_19971 = 0; write_iter_19971 < dzlz7bUZLzmZRz20Unz20U1z7dUzg_14237; write_iter_19971++) {
        int64_t write_iv_19973 = ((int64_t *) mem_20113)[write_iter_19971];
        int64_t write_iv_19974 = ((int64_t *) mem_20111)[write_iter_19971];
        int64_t index_primexp_20073 = add64((int64_t) 1, write_iter_19971);
        bool cond_19378 = write_iv_19973 == (int64_t) 1;
        int64_t lifted_lambda_res_19379;
        
        if (cond_19378) {
            int64_t lifted_lambda_res_t_res_19530 = sub64(write_iv_19974, (int64_t) 1);
            
            lifted_lambda_res_19379 = lifted_lambda_res_t_res_19530;
        } else {
            lifted_lambda_res_19379 = (int64_t) -1;
        }
        if (sle64((int64_t) 0, lifted_lambda_res_19379) && slt64(lifted_lambda_res_19379, m_18089)) {
            ((int64_t *) mem_20141)[lifted_lambda_res_19379] = index_primexp_20073;
        }
    }
    
    int32_t zp_lhs_14371 = sub32(j_14236, i_14235);
    int32_t tmp_14373 = add32(1, zp_lhs_14371);
    
    if (memblock_alloc(ctx, &mem_20150, bytes_20149, "mem_20150")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t nest_i_20522 = 0; nest_i_20522 < (int64_t) 1; nest_i_20522++) {
        ((int64_t *) mem_20150.mem)[nest_i_20522] = (int64_t) 0;
    }
    
    int64_t tmp_offs_20523 = (int64_t) 0;
    
    if (!(tmp_offs_20523 == (int64_t) 0)) {
        lmad_copy_8b(ctx, 1, (uint64_t *) mem_20150.mem, tmp_offs_20523, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_20150.mem, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {(int64_t) 1});
    }
    tmp_offs_20523 += (int64_t) 1;
    lmad_copy_8b(ctx, 1, (uint64_t *) mem_20150.mem, tmp_offs_20523, (int64_t []) {(int64_t) 1}, (uint64_t *) mem_20141, (int64_t) 0, (int64_t []) {(int64_t) 1}, (int64_t []) {m_18089});
    tmp_offs_20523 += m_18089;
    if (memblock_alloc(ctx, &mem_20152, bytes_20151, "mem_20152")) {
        err = 1;
        goto cleanup;
    }
    for (int64_t nest_i_20524 = 0; nest_i_20524 < conc_tmp_16996; nest_i_20524++) {
        ((int32_t *) mem_20152.mem)[nest_i_20524] = curDepth_14233;
    }
    if (memblock_set(ctx, &mem_out_20506, &mem_20150, "mem_20150") != 0)
        return 1;
    if (memblock_set(ctx, &mem_out_20507, &mem_20152, "mem_20152") != 0)
        return 1;
    prim_out_20508 = conc_tmp_16996;
    prim_out_20509 = conc_tmp_16996;
    prim_out_20510 = curDepth_14233;
    prim_out_20511 = tmp_14373;
    prim_out_20512 = i64_res_19256;
    if (memblock_set(ctx, &*mem_out_p_20638, &mem_out_20506, "mem_out_20506") != 0)
        return 1;
    if (memblock_set(ctx, &*mem_out_p_20639, &mem_out_20507, "mem_out_20507") != 0)
        return 1;
    *out_prim_out_20640 = prim_out_20508;
    *out_prim_out_20641 = prim_out_20509;
    *out_prim_out_20642 = prim_out_20510;
    *out_prim_out_20643 = prim_out_20511;
    *out_prim_out_20644 = prim_out_20512;
    
  cleanup:
    {
        free(mem_20111);
        free(mem_20113);
        free(mem_20120);
        free(mem_20121);
        free(mem_20141);
        if (memblock_unref(ctx, &mem_20152, "mem_20152") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_20150, "mem_20150") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_20507, "mem_out_20507") != 0)
            return 1;
        if (memblock_unref(ctx, &mem_out_20506, "mem_out_20506") != 0)
            return 1;
    }
    return err;
}
FUTHARK_FUN_ATTR int futrts_set_bit_2464(struct futhark_context *ctx, int8_t *out_prim_out_20650, int32_t bit_12477, int8_t x_12478, int32_t b_12479)
{
    (void) ctx;
    
    int err = 0;
    int8_t prim_out_20506;
    int32_t i32_arg0_12480 = shl32(1, bit_12477);
    int32_t i32_arg_12482 = ~i32_arg0_12480;
    int8_t unsign_arg0_17001 = zext_i32_i8(i32_arg_12482);
    int8_t unsign_arg0_19254 = x_12478 & unsign_arg0_17001;
    int32_t i32_arg0_12487 = shl32(b_12479, bit_12477);
    int8_t unsign_arg0_17003 = zext_i32_i8(i32_arg0_12487);
    int8_t unsign_arg0_17006 = unsign_arg0_17003 | unsign_arg0_19254;
    
    prim_out_20506 = unsign_arg0_17006;
    *out_prim_out_20650 = prim_out_20506;
    
  cleanup:
    { }
    return err;
}

int futhark_entry_main(struct futhark_context *ctx, struct futhark_bool_1d **out0, const int32_t in0)
{
    int32_t max_depth_16888 = 0;
    int64_t prim_out_20507 = (int64_t) 0;
    int ret = 0;
    
    lock_lock(&ctx->lock);
    
    struct memblock mem_out_20506;
    
    mem_out_20506.references = NULL;
    max_depth_16888 = in0;
    if (ret == 0) {
        ret = futrts_entry_main(ctx, &mem_out_20506, &prim_out_20507, max_depth_16888);
        if (ret == 0) {
            assert((*out0 = (struct futhark_bool_1d *) malloc(sizeof(struct futhark_bool_1d))) != NULL);
            (*out0)->mem = mem_out_20506;
            (*out0)->shape[0] = prim_out_20507;
        }
    }
    lock_unlock(&ctx->lock);
    return ret;
}
  
