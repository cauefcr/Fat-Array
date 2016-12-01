
/**
 * 'fat_array.h' - fat_array
 *
 * copyright (c) 2016 Cauê Felchar <caue.fcr #at# gmail.com>
 */

#ifndef FAT_ARRAY_H
#define FAT_ARRAY_H

#include <stddef.h>
#include <stdint.h>  //for uint8_t
#include <stdio.h>
#include <stdlib.h>

#ifndef FA__EMPTY_ALLOC
#define FA__EMPTY_ALLOC 8
#endif
/* TO-DO:
   - Read rest of sds api and implement applicable and usefull functions
 */

#define fat_toStruct(fa__T, fa__array)                     \
  ((struct f##fa__T##_struct *)(((uint8_t *)(fa__array)) - \
                                offsetof(struct f##fa__T##_struct, vec)))

// macro wrappers for functions
#define fat_new(fa__T, fa__size) f##fa__T##_new(fa__size)

#define fat_push(fa__T, fa__array, fa__element) \
  f##fa__T##_push((fa__array), (fa__element))

#define fat_pop(fa__T, fa__array) f##fa__T##_pop((fa__array))

#define fat_newfrom(fa__T, fa__init, fa__len) \
  f##fa__T##_newfrom((fa__init), (fa__len))

#define fat_empty(fa__T) f##fa__T##_empty()

#define fat_dup(fa__T, fa__array) f##fa__T##_dup((fa__array))

#define fat_growzero(fa__T, fa__array, fa__len) \
  f##fa__T##_growzero((fa__array), (fa__len))

#define fat_catlen(fa__T, fa__array, fa__base, fa__len) \
  f##fa__T##_catlen((fa__array), (fa__base), (fa__len))

#define fat_range(fa__T, fa__array, fa__start, fa__end) \
  f##fa__T##_range((fa__array), (fa__start), (fa__end))

#define fat_map(fa__T, fa__array, fa__func) \
  f##fa__T##_map((fa__array), (fa__func))

#define fat_fold(fa__T, fa__array, fa__func) \
  f##fa__T##_fold((fa__array), (fa__func))

#define fat_filter(fa__T, fa__array, fa__func) \
  f##fa__T##_filter((fa__array), (fa__func))

#define fat_sort(fa__T, fa__array, fa__cmp) \
  f##fa__T##_sort((fa__array), (fa__cmp))

#define fat_bsearch(fa__T, fa__array, fa__cmp, fa__needle) \
  f##fa__T##_bsearch((fa__array), (fa__cmp), (fa__needle))

#define fat_len(fa__T, fa__array) (fat_toStruct(fa__T, (fa__array))->len)

#define fat_avail(fa__T, fa__array)          \
  (fat_toStruct(fa__T, (fa__array))->alloc - \
   fat_toStruct(fa__T, (fa__array))->len)

#define fat_setlen(fa__T, fa__array, fa__newlen)          \
  do {                                                    \
    fat_toStruct(fa__T, (fa__array))->len = (fa__newlen); \
  } while (0)

#define fat_inclen(fa__T, fa__array, fa__inc)           \
  do {                                                  \
    fat_toStruct(fa__T, (fa__array))->len += (fa__inc); \
  } while (0)

#define fat_alloc(fa__T, fa__array) (fat_toStruct(fa__T, (fa__array))->alloc)

#define fat_setalloc(fa__T, fa__array, fa__newlen)          \
  do {                                                      \
    fat_toStruct(fa__T, (fa__array))->alloc = (fa__newlen); \
  } while (0)

/* Free's a fat array and makes array NULL. No operation is performed if
 * 'fa__array' is
 * NULL. */
#define fat_free(fa__T, fa__array)                                  \
  do {                                                              \
    ((fa__array) == NULL) ? (void)(NULL)                            \
                          : free(fat_toStruct(fa__T, (fa__array))); \
  } while (0)

#define FCMP(fa__T, a, b) int f##fa__T##_cmp(const fa__T a, const fa__T b)

#define CMP(fa__T) f##fa__T##_cmp

#define MAKEFAT(fa__T)                                                         \
                                                                               \
  typedef fa__T *f##fa__T; /*defines a type ffa__Type, short for fat type*/    \
                                                                               \
  struct f##fa__T##_struct {                                                   \
    size_t len; /*length of the array*/                                        \
    size_t alloc;                                                              \
    uint8_t flags; /* unused currently */                                      \
    fa__T vec[];   /*0-length array, the whole point of this implementation*/  \
  };                                                                           \
                                                                               \
  static inline size_t f##fa__T##_avail(const f##fa__T v) {                    \
    return fat_avail(fa__T, v);                                                \
  }                                                                            \
  static inline size_t f##fa__T##_len(const f##fa__T v) {                      \
    return fat_len(fa__T, v);                                                  \
  }                                                                            \
  static inline void f##fa__T##_setlen(f##fa__T v, const size_t newlen) {      \
    fat_setlen(fa__T, v, newlen);                                              \
    return;                                                                    \
  }                                                                            \
  static inline void f##fa__T##_inclen(f##fa__T v, const size_t inc) {         \
    fat_inclen(fa__T, v, inc);                                                 \
    return;                                                                    \
  }                                                                            \
  static inline size_t f##fa__T##_alloc(const f##fa__T v) {                    \
    return fat_alloc(fa__T, v);                                                \
  }                                                                            \
  static inline void f##fa__T##_setalloc(f##fa__T v, size_t newalloc) {        \
    fat_setalloc(fa__T, v, newalloc);                                          \
    return;                                                                    \
  }                                                                            \
  static inline void f##fa__T##_free(const f##fa__T v) {                       \
    fat_free(fa__T, v);                                                        \
    return;                                                                    \
  }                                                                            \
  f##fa__T f##fa__T##_new(const size_t size) {                                 \
    struct f##fa__T##_struct *out = (struct f##fa__T##_struct *)calloc(        \
        1, sizeof(struct f##fa__T##_struct) + sizeof(fa__T) * size);           \
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
    return (f##fa__T)out->vec;                                                 \
  }                                                                            \
                                                                               \
  /* Create a new fat pointer array with the content specified by the 'init'   \
   * pointer                                                                   \
   * and 'initlen'.                                                            \
   * If NULL is used for 'init' the array is initialized with zero bytes.   */ \
                                                                               \
  f##fa__T f##fa__T##_newfrom(const fa__T init[], const size_t initlen) {      \
    f##fa__T out = f##fa__T##_new(((initlen) ? (initlen) : FA__EMPTY_ALLOC));  \
    if (init == NULL) {                                                        \
      return out;                                                              \
    }                                                                          \
    for (size_t i = 0; i < initlen; i++) {                                     \
      out[i] = init[i];                                                        \
    }                                                                          \
    fat_setlen(fa__T, out, initlen);                                           \
    return out;                                                                \
  }                                                                            \
                                                                               \
  /* Create an empty (zero length) fat array. */                               \
                                                                               \
  f##fa__T f##fa__T##_empty(void) { return f##fa__T##_new(FA__EMPTY_ALLOC); }  \
                                                                               \
  /* Duplicate an fat array. */                                                \
                                                                               \
  f##fa__T f##fa__T##_dup(const f##fa__T v) {                                  \
    size_t len = fat_len(fa__T, v);                                            \
    f##fa__T out = f##fa__T##_new(len);                                        \
    for (size_t i = 0; i < len; i++) {                                         \
      out[i] = v[i];                                                           \
    }                                                                          \
    fat_setlen(fa__T, out, fat_len(fa__T, v));                                 \
    return out;                                                                \
  }                                                                            \
                                                                               \
  /* Grow the fat array to have the specified length. Bytes that were not part \
   * of                                                                        \
   * the original length of the array will be set to zero.                     \
   * if the specified length is smaller than the current length, no operation  \
   * is performed. */                                                          \
                                                                               \
  f##fa__T f##fa__T##_growzero(f##fa__T v, size_t len) {                       \
    if (fat_len(fa__T, v) >= len) {                                            \
      return v;                                                                \
    }                                                                          \
    f##fa__T out = f##fa__T##_new(len);                                        \
    for (size_t i = 0; i < fat_len(fa__T, v); i++) {                           \
      out[i] = v[i];                                                           \
    }                                                                          \
    fat_setlen(fa__T, out, fat_len(fa__T, v));                                 \
    fat_free(fa__T, v);                                                        \
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
  f##fa__T f##fa__T##_catlen(f##fa__T v, const fa__T *t, size_t len) {         \
    if (fat_avail(fa__T, v) < len) {                                           \
      v = f##fa__T##_growzero(v, fat_len(fa__T, v) + len);                     \
    }                                                                          \
    size_t last = fat_len(fa__T, v);                                           \
    for (size_t i = 0; i < len; i++) {                                         \
      v[last + i] = t[i];                                                      \
    }                                                                          \
    fat_inclen(fa__T, v, len);                                                 \
    return v;                                                                  \
  }                                                                            \
                                                                               \
  /* Pushes the value into the end of the array, growing it by a factor of 2   \
   * if needed*/                                                               \
                                                                               \
  f##fa__T f##fa__T##_push(f##fa__T v, fa__T value) {                          \
    if (fat_avail(fa__T, v) == 0) {                                            \
      v = f##fa__T##_growzero(v, fat_len(fa__T, v) * 2);                       \
    }                                                                          \
    v[fat_len(fa__T, v)] = value;                                              \
    fat_inclen(fa__T, v, 1);                                                   \
    return v;                                                                  \
  }                                                                            \
                                                                               \
  /* Removes and returns the last value of the array, returns 0 if empty*/     \
                                                                               \
  fa__T f##fa__T##_pop(f##fa__T v) {                                           \
    if (fat_len(fa__T, v) == 0) {                                              \
      /* perror("removed element from empty array"); */                        \
      return (fa__T){0};                                                       \
    }                                                                          \
    fa__T out = v[fat_len(fa__T, v) - 1];                                      \
    fat_setlen(fa__T, v, fat_len(fa__T, v) - 1);                               \
    return out;                                                                \
  }                                                                            \
                                                                               \
  /* fa__Turn the array into a smaller (or equal) array containing only the    \
   * subarray specified by the 'start' and 'end' indexes.                      \
   *                                                                           \
   * start and end can be negative, where -1 means the last character of the   \
   * array, -2 the penultimate character, and so forth.                        \
   *                                                                           \
   * fa__The interval is inclusive, so the start and end characters will be    \
   * part                                                                      \
   * of the resulting array.                                                   \
   *                                                                           \
   * fa__The array is modified in-place.                                       \
   *                                                                           \
   * Example:                                                                  \
   *                                                                           \
   * s = fint_new({1,2,3,4},4);                                                \
   * fint_range(s,1,-1); => {2,3,4}                                            \
   */                                                                          \
                                                                               \
  void f##fa__T##_range(f##fa__T v, int start, int end) {                      \
    size_t len = fat_len(fa__T, v);                                            \
    if (len == 0) return;                                                      \
    if (start < 0) start = len + start;                                        \
    if (end < 0) end = len + end;                                              \
    if (start >= end) {                                                        \
      fat_setlen(fa__T, v, 0);                                                 \
      return;                                                                  \
    }                                                                          \
    start--;                                                                   \
    end++;                                                                     \
    for (int i = 0; i < end - start; i++) {                                    \
      v[i] = v[start + i];                                                     \
    }                                                                          \
    fat_setlen(fa__T, v, end - start);                                         \
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
  void f##fa__T##_map(f##fa__T v, fa__T (*func)(fa__T)) {                      \
    size_t len = fat_len(fa__T, v);                                            \
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
  fa__T f##fa__T##_fold(f##fa__T v, fa__T (*func)(fa__T, fa__T)) {             \
    size_t len = fat_len(fa__T, v);                                            \
    fa__T out = v[0];                                                          \
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
  f##fa__T f##fa__T##_filter(f##fa__T v, int (*func)(fa__T)) {                 \
    f##fa__T out = f##fa__T##_new(8);                                          \
    size_t len = fat_len(fa__T, v);                                            \
    for (size_t i = 0; i < len; i++) {                                         \
      if (func(v[i])) {                                                        \
        f##fa__T##_push(out, v[i]);                                            \
      }                                                                        \
    }                                                                          \
    return out;                                                                \
  }                                                                            \
  /** shellsort, using Marcin Ciura's gap sequence */                          \
  void f##fa__T##_shsort(fa__T array[], int (*cmp)(fa__T, fa__T),              \
                         size_t len) {                                         \
    const int gaps[] = {701, 301, 132, 57, 23, 10, 4, 1};                      \
    int i, j, k;                                                               \
    fa__T tmp;                                                                 \
    for (k = 0; k < 8; k++) {                                                  \
      for (i = gaps[k]; (size_t)i < len; i++) {                                \
        for (j = i; j >= gaps[k] && cmp(array[j - gaps[k]], array[j]) > 0;     \
             j -= gaps[k]) {                                                   \
          tmp = array[j];                                                      \
          array[j] = array[j - 1];                                             \
          array[j - 1] = tmp;                                                  \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  }                                                                            \
  /* quicksort with small size optimization */                                 \
  void f##fa__T##_qsort(fa__T array[], int (*cmp)(fa__T, fa__T), int len) {    \
    if (len < 8) {                                                             \
      f##fa__T##_shsort(array, cmp, len);                                      \
      return;                                                                  \
    } /* pivoting */                                                           \
    fa__T pivot = array[len / 2], tmp;                                         \
    int left = 0, right = len - 1;                                             \
    while (left <= right) {                                                    \
      for (; cmp(array[left], pivot) < 0; left++) {                            \
      }                                                                        \
      for (; cmp(array[right], pivot) > 0; right--) {                          \
      }                                                                        \
      if (left >= right) {                                                     \
        break;                                                                 \
      }                                                                        \
      tmp = array[left]; /* swap */                                            \
      array[left] = array[right];                                              \
      array[right] = tmp;                                                      \
      left++;                                                                  \
      right--;                                                                 \
    }                                                                          \
    f##fa__T##_qsort(array, cmp, left);                                        \
    f##fa__T##_qsort(array + left, cmp, len - left);                           \
  }                                                                            \
                                                                               \
  /* Sorts the array, it accepts a function which works as                     \
   * strcmp(a,b), returning -1 for a < b, 0 for a == b, 1 a > b or simply a-b. \
   */                                                                          \
                                                                               \
  void f##fa__T##_sort(f##fa__T v, int (*cmp)(fa__T, fa__T)) {                 \
    if (fat_len(fa__T, v) > 10) {                                              \
      f##fa__T##_qsort(v, cmp, fat_len(fa__T, v));                             \
    } else {                                                                   \
      f##fa__T##_shsort(v, cmp, fat_len(fa__T, v));                            \
    }                                                                          \
  }                                                                            \
                                                                               \
  /* Finds a number in the array, the cmp function should be the same used for \
   * sorting, to ensure it works                                               \
   */                                                                          \
  size_t f##fa__T##_bsearch(f##fa__T v, int (*cmp)(fa__T, fa__T),              \
                            fa__T needle) {                                    \
    long int p = 0, u = fat_len(fa__T, v), m, tmp;                             \
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
    } while (p <= u);                                                          \
    return -1;                                                                 \
  }

#endif /* FAT_ARRAY_H */
