
/**
 * 'fat_array.h' - fat_array
 *
 * copyright (c) 2016 Cauê Felchar <caue.fcr #at# gmail.com>
 */

#ifndef FAT_ARRAY_H
#define FAT_ARRAY_H

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif
#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif
#ifndef STDINT_H
#define STDINT_H
#include <stdint.h>  //for uint8_t
#endif

#ifndef STDDEF_H
#define STDDEF_H
#include <stddef.h>
#endif

#define EMPTY_ALLOC 8

/* TO-DO:
   - Read rest of sds api and implement applicable and usefull functions
 */

// #define fptr void *

// struct fat_ptr_struct {  /* used for casts when the array itself is not
// needed */
//   size_t len;            /* length of the fat pointer*/
//   size_t alloc;          /* excluding the header */
//   uint8_t flags;
// };

#define fat_toStruct(T, v)                     \
  ((struct f##T##_struct *)(((uint8_t *)(v)) - \
                            offsetof(struct f##T##_struct, vec)))

// #define fat_struct(v) fat_toStruct(at_ptr, v)
// macro wrappers for functions
#define fat_new(T, size) f##T##_new(size)

#define fat_push(T, array, element) f##T##_push((array), (element))

#define fat_pop(T, array) f##T##_pop((array))

#define fat_newfrom(T, init, len) f##T##_newfrom((init), (len))

#define fat_empty(T) f##T##_empty()

#define fat_dup(T, array) f##T##_dup((array))

#define fat_growzero(T, array, len) f##T##_growzero((array), (len))

#define fat_catlen(T, array, base, len) f##T##_catlen((array), (base), (len))

#define fat_range(T, array, start, end) f##T##_range((array), (start), (end))

#define fat_map(T, v, func) f##T##_map((v), (func))

#define fat_fold(T, v, func) f##T##_fold((v), (func))

#define fat_filter(T, v, func) f##T##_filter((v), (func))

#define fat_sort(T, v, cmp) f##T##_sort((v), (cmp))

#define fat_bsearch(T, v, cmp, needle) f##T##_bsearch((v), (cmp), (needle))

#define fat_len(T, v) (fat_toStruct(T, (v))->len)
// static inline size_t fat_len(fptr v) { return fat_struct(v)->len; }

#define fat_avail(T, v) \
  (fat_toStruct(T, (v))->alloc - fat_toStruct(T, (v))->len)

#define fat_setlen(T, v, newlen)          \
  do {                                    \
    fat_toStruct(T, (v))->len = (newlen); \
  } while (0)

#define fat_inclen(T, v, inc)           \
  do {                                  \
    fat_toStruct(T, (v))->len += (inc); \
  } while (0)

#define fat_alloc(T, v) (fat_toStruct(T, (v))->alloc)

#define fat_setalloc(T, v, newlen)          \
  do {                                      \
    fat_toStruct(T, (v))->alloc = (newlen); \
  } while (0)

/* Free's a fat array and makes v NULL. No operation is performed if 'v' is
 * NULL. */
#define fat_free(T, v)                                         \
  do {                                                         \
    ((v) == NULL) ? (void)(NULL) : free(fat_toStruct(T, (v))); \
  } while (0)

#define FCMP(T, a, b) int f##T##_cmp(const T a, const T b)

#define CMP(T) f##T##_cmp

#define makefat(T)                                                             \
                                                                               \
  typedef T *f##T; /*defines a type fType, short for fat type*/                \
                                                                               \
  struct f##T##_struct {                                                       \
    size_t len; /*length of the array*/                                        \
    size_t alloc;                                                              \
    uint8_t flags; /* unused currently */                                      \
    T vec[];       /*0-length array, the whole point of this implementation*/  \
  };                                                                           \
                                                                               \
  static inline size_t f##T##_avail(const f##T v) { return fat_avail(T, v); }  \
  static inline size_t f##T##_len(const f##T v) { return fat_len(T, v); }      \
  static inline void f##T##_setlen(f##T v, const size_t newlen) {              \
    fat_setlen(T, v, newlen);                                                  \
    return;                                                                    \
  }                                                                            \
  static inline void f##T##_inclen(f##T v, const size_t inc) {                 \
    fat_inclen(T, v, inc);                                                     \
    return;                                                                    \
  }                                                                            \
  static inline size_t f##T##_alloc(const f##T v) { return fat_alloc(T, v); }  \
  static inline void f##T##_setalloc(f##T v, size_t newalloc) {                \
    fat_setalloc(T, v, newalloc);                                              \
    return;                                                                    \
  }                                                                            \
  static inline void f##T##_free(const f##T v) {                               \
    fat_free(T, v);                                                            \
    return;                                                                    \
  }                                                                            \
  f##T f##T##_new(const size_t size) {                                         \
    struct f##T##_struct *out = (struct f##T##_struct *)calloc(                \
        1, sizeof(struct f##T##_struct) + sizeof(T) * size);                   \
    if (out == NULL) { /* returns a new fat pointer of the correct type */     \
      fprintf(stderr, "Out of memory!\n");                                     \
      return NULL;                                                             \
    }                                                                          \
    out->alloc = size;                                                         \
    /*out->flags = '6';                                                        \
    printf("size_at_new: %zu,flags_at_new:%c\n",out->alloc,out->flags);        \
     if (DBUG) {                                                               \
       printf("new:\tvec: %p, []: %p\n", (void *)out, (void *)&out->vec[0]);   \
     } */                                                                      \
    return (f##T)out->vec;                                                     \
  }                                                                            \
                                                                               \
  /* Create a new fat pointer array with the content specified by the 'init'   \
   * pointer                                                                   \
   * and 'initlen'.                                                            \
   * If NULL is used for 'init' the array is initialized with zero bytes.   */ \
                                                                               \
  f##T f##T##_newfrom(const T init[], const size_t initlen) {                  \
    f##T out = f##T##_new(((initlen) ? (initlen) : EMPTY_ALLOC));              \
    if (init == NULL) {                                                        \
      return out;                                                              \
    }                                                                          \
    for (size_t i = 0; i < initlen; i++) {                                     \
      out[i] = init[i];                                                        \
    }                                                                          \
    fat_setlen(T, out, initlen);                                               \
    return out;                                                                \
  }                                                                            \
                                                                               \
  /* Create an empty (zero length) fat array. */                               \
                                                                               \
  f##T f##T##_empty(void) { return f##T##_new(EMPTY_ALLOC); }                  \
                                                                               \
  /* Duplicate an fat array. */                                                \
                                                                               \
  f##T f##T##_dup(const f##T v) {                                              \
    size_t len = fat_len(T, v);                                                \
    f##T out = f##T##_new(len);                                                \
    for (size_t i = 0; i < len; i++) {                                         \
      out[i] = v[i];                                                           \
    }                                                                          \
    fat_setlen(T, out, fat_len(T, v));                                         \
    return out;                                                                \
  }                                                                            \
                                                                               \
  /* Grow the fat array to have the specified length. Bytes that were not part \
   * of                                                                        \
   * the original length of the array will be set to zero.                     \
   * if the specified length is smaller than the current length, no operation  \
   * is performed. */                                                          \
                                                                               \
  f##T f##T##_growzero(f##T v, size_t len) {                                   \
    if (fat_len(T, v) >= len) {                                                \
      return v;                                                                \
    }                                                                          \
    f##T out = f##T##_new(len);                                                \
    for (size_t i = 0; i < fat_len(T, v); i++) {                               \
      out[i] = v[i];                                                           \
    }                                                                          \
    fat_setlen(T, out, fat_len(T, v));                                         \
    fat_free(T, v);                                                            \
    return out;                                                                \
  }                                                                            \
                                                                               \
  /* Append the specified array pointed by 't' of 'len' bytes to the  end of   \
   * the specified fat array 'v'.                                              \
   *                                                                           \
   * After the call, the passed fat array is no longer valid and all the       \
   * references must be substituted with the new pointer returned by the call. \
   */                                                                          \
                                                                               \
  f##T f##T##_catlen(f##T v, const T *t, size_t len) {                         \
    if (fat_avail(T, v) < len) {                                               \
      v = f##T##_growzero(v, fat_len(T, v) + len);                             \
    }                                                                          \
    size_t last = fat_len(T, v);                                               \
    for (size_t i = 0; i < len; i++) {                                         \
      v[last + i] = t[i];                                                      \
    }                                                                          \
    fat_inclen(T, v, len);                                                     \
    return v;                                                                  \
  }                                                                            \
                                                                               \
  /* Pushes the value into the end of the array, growing it by a factor of 2   \
   * if needed*/                                                               \
                                                                               \
  f##T f##T##_push(f##T v, T value) {                                          \
    if (fat_avail(T, v) == 0) {                                                \
      v = f##T##_growzero(v, fat_len(T, v) * 2);                               \
    }                                                                          \
    v[fat_len(T, v)] = value;                                                  \
    fat_inclen(T, v, 1);                                                       \
    return v;                                                                  \
  }                                                                            \
                                                                               \
  /* Removes and returns the last value of the array, returns 0 if empty*/     \
                                                                               \
  T f##T##_pop(f##T v) {                                                       \
    if (fat_len(T, v) == 0) {                                                  \
      /* perror("removed element from empty array"); */                        \
      return (T){0};                                                           \
    }                                                                          \
    T out = v[fat_len(T, v) - 1];                                              \
    fat_setlen(T, v, fat_len(T, v) - 1);                                       \
    return out;                                                                \
  }                                                                            \
                                                                               \
  /* Turn the array into a smaller (or equal) array containing only the        \
   * subarray specified by the 'start' and 'end' indexes.                      \
   *                                                                           \
   * start and end can be negative, where -1 means the last character of the   \
   * array, -2 the penultimate character, and so forth.                        \
   *                                                                           \
   * The interval is inclusive, so the start and end characters will be part   \
   * of the resulting array.                                                   \
   *                                                                           \
   * The array is modified in-place.                                           \
   *                                                                           \
   * Example:                                                                  \
   *                                                                           \
   * s = fint_new({1,2,3,4},4);                                                \
   * fint_range(s,1,-1); => {2,3,4}                                            \
   */                                                                          \
                                                                               \
  void f##T##_range(f##T v, int start, int end) {                              \
    size_t len = fat_len(T, v);                                                \
    if (len == 0) return;                                                      \
    if (start < 0) start = len + start;                                        \
    if (end < 0) end = len + end;                                              \
    if (start >= end) {                                                        \
      fat_setlen(T, v, 0);                                                     \
      return;                                                                  \
    }                                                                          \
    start--;                                                                   \
    end++;                                                                     \
    for (int i = 0; i < end - start; i++) {                                    \
      v[i] = v[start + i];                                                     \
    }                                                                          \
    fat_setlen(T, v, end - start);                                             \
  }                                                                            \
                                                                               \
  /* Applies the function func to every element of the array v.                \
   *                                                                           \
   * Example:                                                                  \
   * //at the top level                                                        \
   *                                                                           \
   * int square(int input){return input*input;}                                \
   *                                                                           \
   * // inside a function                                                      \
   * fint_arr = fint_newfrom((int[]){1,2,3,4},4);                              \
   *                                                                           \
   * fint_map(fint_arr,square);    => {1,4,9,16}                               \
   *                                                                           \
   */                                                                          \
                                                                               \
  void f##T##_map(f##T v, T (*func)(T)) {                                      \
    size_t len = fat_len(T, v);                                                \
    for (size_t i = 0; i < len; i++) {                                         \
      v[i] = func(v[i]);                                                       \
    }                                                                          \
    return;                                                                    \
  }                                                                            \
                                                                               \
  /* Applies the function func to every pair of elements from the array.       \
   *                                                                           \
   * Example:                                                                  \
   * //at the top level                                                        \
   *                                                                           \
   * int max(int a, int b){return a>b?a:b;}                                    \
   *                                                                           \
   * // inside a function                                                      \
   * fint_arr = fint_newfrom((int[]){1,2,3,1},4);                              \
   *                                                                           \
   * int out = fint_fold(fint_arr,max);   => 3                                 \
   *                                                                           \
   */                                                                          \
                                                                               \
  T f##T##_fold(f##T v, T (*func)(T, T)) {                                     \
    size_t len = fat_len(T, v);                                                \
    T out = v[0];                                                              \
    for (size_t i = 1; i < len; i++) {                                         \
      out = func(out, v[i]);                                                   \
    }                                                                          \
    return out;                                                                \
  }                                                                            \
                                                                               \
  /* Creates a new fat array from every element which returns true form the    \
   * function                                                                  \
   * func being applied to it                                                  \
   *                                                                           \
   * Example:                                                                  \
   * //at the top level                                                        \
   *                                                                           \
   * int ispair (int a){return a%2==0;}                                        \
   *                                                                           \
   * // inside a function                                                      \
   * fint_arr = fint_newfrom((int[]){1,2,3,4},4);                              \
   *                                                                           \
   * fint out = fint_filter(fint_arr,ispair);   => {2,4}                       \
   *                                                                           \
   */                                                                          \
                                                                               \
  f##T f##T##_filter(f##T v, int (*func)(T)) {                                 \
    f##T out = f##T##_new(8);                                                  \
    size_t len = fat_len(T, v);                                                \
    for (size_t i = 0; i < len; i++) {                                         \
      if (func(v[i])) {                                                        \
        f##T##_push(out, v[i]);                                                \
      }                                                                        \
    }                                                                          \
    return out;                                                                \
  }                                                                            \
  /** shellsort, using Marcin Ciura's gap sequence */                          \
  void f##T##_shsort(T array[], int (*cmp)(T, T), size_t len) {                \
    const int gaps[] = {701, 301, 132, 57, 23, 10, 4, 1};                      \
    int i, j, k;                                                               \
    T tmp;                                                                     \
    for (k = 0; k < 8; k++) {                                                  \
      for (i = gaps[k]; (size_t)i < len; i++) {                                \
        for (j = i; j > gaps[k] && cmp(array[j - gaps[k]], array[j]) > 0;      \
             j -= gaps[k]) {                                                   \
          tmp = array[j];                                                      \
          array[j] = array[j - 1];                                             \
          array[j - 1] = tmp;                                                  \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  }                                                                            \
  /* quicksort with small size optimization */                                 \
  void f##T##_qsort(T array[], int (*cmp)(T, T), int len) {                    \
    if (len < 8) {                                                             \
      f##T##_shsort(array, cmp, len);                                          \
      return;                                                                  \
    }                                                                          \
    /* pivoting */                                                             \
    int p = len / 2, si = 0,                                                   \
        bi = len - 1; /* smaller index and bigger index */                     \
    T tmp;                                                                     \
    while (si < bi) {                                                          \
      for (; cmp(array[si], array[p]) < 0; si++) {                             \
      } /* array[si] < array[p] */                                             \
      for (; cmp(array[bi], array[p]) > 0; bi--) {                             \
      } /* array[bi] > array[p] */                                             \
      tmp = array[si];                                                         \
      array[si] = array[bi];                                                   \
      array[bi] = tmp;                                                         \
      si++;                                                                    \
      bi--;                                                                    \
    }                                                                          \
    f##T##_qsort(array, cmp, p);                                               \
    f##T##_qsort(array + p, cmp, len - p);                                     \
  }                                                                            \
                                                                               \
  /* Sorts the array, it accepts a function which works as                     \
   * strcmp(a,b), returning -1 for a < b, 0 for a == b, 1 a > b or simply a-b. \
   */                                                                          \
                                                                               \
  void f##T##_sort(f##T v, int (*cmp)(T, T)) {                                 \
    if (fat_len(T, v) > 10) {                                                  \
      f##T##_qsort(v, cmp, fat_len(T, v));                                     \
    } else {                                                                   \
      f##T##_shsort(v, cmp, fat_len(T, v));                                    \
    }                                                                          \
  }                                                                            \
                                                                               \
  /* Finds a number in the array, the cmp function should be the same used for \
   * sorting, to ensure it works                                               \
   */                                                                          \
  size_t f##T##_bsearch(f##T v, int (*cmp)(T, T), T needle) {                  \
    long int p = 0, u = fat_len(T, v), m, tmp;                                 \
    do {                                                                       \
      m = p + (u - p) / 2;                                                     \
      tmp = cmp(v[m], needle);                                                 \
      if (!tmp) { /* v[m] == needle */                                         \
        return m;                                                              \
      } else if (tmp < 0) { /* v[m] < needle */                                \
        p = m + 1;                                                             \
      } else { /* v[m] > needle */                                             \
        u = m - 1;                                                             \
      }                                                                        \
    } while (p < u);                                                           \
    return -1;                                                                 \
  }

// #undef EMPTY_ALLOC

#endif /* FAT_ARRAY_H */
