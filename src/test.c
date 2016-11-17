
/**
 * 'test.c' - fat_array
 *
 * copyright (c) 2016 Cauê Felchar <caue.fcr #at# gmail.com>
 */

#include <string.h>
#include "fat_array.h"

typedef union {
  float f;
  int i;
  char s[sizeof(float)];
} ut;

typedef struct {
  float f;
  int i;
  char s[sizeof(float)];
} st;

typedef char* str;

MAKEFAT(char)
FCMP(char, a, b) { return (int)a - (int)b; }

MAKEFAT(int)
FCMP(int, a, b) { return a - b; }

MAKEFAT(float)
FCMP(float, a, b) {
#define E 0.0001
  if (a < b - E) {  //
    return -1;
  } else if (a > b - E && a < b + E) {  // a == b
    return 0;
  } else {  // a>b+E
    return 1;
  }
#undef E
}

MAKEFAT(ut)
FCMP(ut, a, b) { return memcmp(&a, &b, sizeof(ut)); }

MAKEFAT(st)
FCMP(st, a, b) { return a.i - b.i; }

MAKEFAT(str)
FCMP(str, a, b) { return strcmp(a, b); }

// fold
int sum(const int a, const int b) { return a + b; }

// map
int square(const int a) { return a * a; }

// filter
int even(const int a) { return a % 2 == 0; }

int main() {
  /**********************************************************
          testing struct -> fptr and fptr-> struct
  ***********************************************************/

  struct fint_struct* ftest = malloc(sizeof(struct fint_struct));
  if (((uint8_t*)ftest - (uint8_t*)fat_toStruct(int, ftest->vec)) != 0) {
    printf("FAILED: struct: %p, vec: %p, vectostruct: %p,%d\n", (void*)ftest,
           (void*)&ftest->vec[0], (void*)fat_toStruct(int, ftest->vec),
           __LINE__);
    printf(
        "sizeof(struct fint_struct): %zu,sizeof(struct fat_ptr_struct): %zu\n",
        sizeof(struct fint_struct), sizeof(struct fint_struct));
    printf("ftest-fat_toStruct(int,ftest): %ld\n",
           (uint8_t*)ftest -
               ((uint8_t*)ftest->vec - offsetof(struct fint_struct, vec)));
    // printf("ftest+sizeof(fint_struct)-ftest->vec: %zu\n",
    // ftest-(uint8_t)ftest->vec -//+ (uint8_t)sizeof(struct fint_struct) -
    // (uint8_t)offsetof(struct fint_struct,vec) );
  } else {
    printf("PASSED at toStruct\n");
  }
  free(ftest);

  /**********************************************************
              testing new, len, alloc and avail
  ***********************************************************/

  fchar fchar_arr = fat_new(char, 8);
  fint fint_arr = fat_new(int, 8);
  ffloat ffloat_arr = fat_new(float, 8);
  fut fut_arr = fat_new(ut, 8);
  fst fst_arr = fat_new(st, 8);
  fstr fstr_arr = fat_new(str, 8);
  if (fint_len(fint_arr) != 0 || fint_alloc(fint_arr) != 8 ||
      fint_avail(fint_arr) != 8 || ffloat_len(ffloat_arr) != 0 ||
      ffloat_alloc(ffloat_arr) != 8 || ffloat_avail(ffloat_arr) != 8 ||
      fut_len(fut_arr) != 0 || fut_alloc(fut_arr) != 8 ||
      fut_avail(fut_arr) != 8 || fst_len(fst_arr) != 0 ||
      fst_alloc(fst_arr) != 8 || fst_avail(fst_arr) != 8 ||
      fchar_len(fchar_arr) != 0 || fchar_alloc(fchar_arr) != 8 ||
      fchar_avail(fchar_arr) != 8 || fstr_len(fstr_arr) != 0 ||
      fstr_alloc(fstr_arr) != 8 || fstr_avail(fstr_arr) != 8) {
    printf("FAILED at new %d\n", __LINE__);
  } else {
    printf("PASSED at new\n");
  }

  /**********************************************************
                    testing push and grow
  ***********************************************************/

  ut utmp;
  st stmp;
  for (int i = 0; i < 16; i++) {
    fint_arr = fat_push(int, fint_arr, i);
    ffloat_arr = fat_push(float, ffloat_arr, (float)i);
    fchar_arr = fat_push(char, fchar_arr, 'a' + i);
    utmp.i = i;
    fut_arr = fat_push(ut, fut_arr, utmp);
    stmp.i = i;
    stmp.f = (float)i;
    stmp.s[0] = 'a' + i;
    stmp.s[1] = '\0';
    fst_arr = fat_push(st, fst_arr, stmp);
    fstr_arr = fat_push(str, fstr_arr, stmp.s);
  }
  int passed = 1;
  for (int i = 0; i < 16; i++) {
    if (fint_arr[i] != i || fchar_arr[i] != 'a' + i ||
        ffloat_arr[i] != (float)i || fut_arr[i].i != i ||
        (fstr_arr[i][0] != 'a' + 1 && fstr_arr[i][1] != '\0') ||
        (fst_arr[i].i != i && fst_arr[i].f != (float)i &&
         fst_arr[i].s[0] != 'a' + i && fst_arr[i].s[1] != '\0')) {
      passed = 0;
      printf("FAILED at push, %d\n", __LINE__);
      printf("%i: %i,%f,%i,%i,%f,%c\n", i, fint_arr[i], ffloat_arr[i],
             fut_arr[i].i, fst_arr[i].i, fst_arr[i].f, fst_arr[i].s[0]);
    }
  }
  if (passed) {
    printf("PASSED at push\n");
  } else
    passed = 1;

  /**********************************************************
                        testing dup
  ***********************************************************/

  fint duparr = fat_dup(int, fint_arr);
  if (fat_len(int, duparr) != fat_len(int, fint_arr) ||
      fat_alloc(int, duparr) != fat_alloc(int, fint_arr)) {
    passed = 0;
    printf("FAILED at dup, %d\n", __LINE__);
    printf("len: %zu, %zu\n", fat_len(int, duparr), fat_len(int, fint_arr));
    printf("alloc: %zu, %zu\n", fat_alloc(int, duparr),
           fat_alloc(int, fint_arr));
  }
  for (size_t i = 0; i < fat_len(int, fint_arr); i++) {
    if (duparr[i] != fint_arr[i]) {
      passed = 0;
      printf("FAILED at dup, %d\n", __LINE__);
    }
  }
  if (passed) {
    printf("PASSED at dup\n");
  } else
    passed = 1;

  /**********************************************************
                        testing range
  ***********************************************************/

  fat_range(int, duparr, 1, -1);  // should change nothing
  if (fat_len(int, duparr) != 16) {
    printf("FAILED range at %d\n", __LINE__);
    printf("len: %zu\n", fat_len(int, duparr));
    passed = 0;
  }
  for (int i = 0; i < 16; i++) {
    if (duparr[i] != i) {
      printf("FAILED range at %d\n", __LINE__);
      passed = 0;
    }
  }
  fat_range(int, duparr, 2,
            -2);  // should take one of the start, one off the end
  if (fat_len(int, duparr) != 14) {
    printf("FAILED range at %d\n", __LINE__);
    printf("len: %zu\n", fat_len(int, duparr));
    passed = 0;
  }
  for (size_t i = 0; i < fat_len(int, duparr); i++) {
    if (duparr[i] != (int)(i + 1)) {
      printf("FAILED range at %d\n", __LINE__);
      printf("%zu,%i\n", i, duparr[i]);
      passed = 0;
    }
  }
  fat_range(int, duparr, -2, 2);  // return it empty
  if (fat_len(int, duparr) != 0) {
    printf("FAILED range at %d\n", __LINE__);
    printf("len: %zu\n", fat_len(int, duparr));
    passed = 0;
  }
  if (passed) {
    printf("PASSED at range\n");
  } else
    passed = 1;

  /**********************************************************
                        testing pop
  ***********************************************************/

  for (int i = 15; i >= 0; i--) {
    if (fat_pop(int, fint_arr) != i || fat_pop(float, ffloat_arr) != (float)i ||
        (utmp = fat_pop(ut, fut_arr), utmp.i != i) ||
        (stmp = fat_pop(st, fst_arr),
         stmp.i != i || stmp.f != (float)i || stmp.s[0] != 'a' + i)) {
      passed = 0;
      printf("FAILED at pop, %d\n", __LINE__);
      printf("%i: %i,%f,%i,%i,%f,%c\n", i, fint_arr[i], ffloat_arr[i],
             fut_arr[i].i, fst_arr[i].i, fst_arr[i].f, fst_arr[i].s[0]);
    }
  }
  if (passed) {
    printf("PASSED at pop\n");
  } else
    passed = 1;

  /**********************************************************
                        testing newfrom
  ***********************************************************/

  // compound literals!
  fint newfrom2 = fint_newfrom((int[]){0}, 0);
  if (fint_len(newfrom2) != 0 || fint_alloc(newfrom2) < 1) {
    printf("FAILED newfrom, at %d\n", __LINE__);
    printf("len: %zu, alloc:%zu\n", fint_len(newfrom2), fint_alloc(newfrom2));
    passed = 0;
  }
  fint newfrom = fint_newfrom((int[]){1, 2, 3}, 3);
  if (fat_len(int, newfrom) != 3) {
    printf("FAILED newfrom, at %d\n", __LINE__);
    passed = 0;
  }
  for (int i = 0; i < 3; i++) {
    if (newfrom[i] != i + 1) {
      passed = 0;
      printf("FAILED newfrom, at %d\n", __LINE__);
    }
  }
  if (passed) {
    printf("PASSED at newfrom\n");
  } else
    passed = 1;

  /**********************************************************
                        testing map
  ***********************************************************/

  fint map_test = fint_newfrom((int[]){0, 1, 2, 3, 4, 5}, 6);

  fat_map(int, map_test, square);

  for (size_t i = 0; i < fat_len(int, map_test); i++) {
    if (map_test[i] != (int)(i * i)) {
      printf("FAILED at map, at %d\n", __LINE__);
      passed = 0;
    }
  }
  if (passed) {
    printf("PASSED at map\n");
  } else
    passed = 1;

  /**********************************************************
                        testing fold
  ***********************************************************/

  if (fat_fold(int, map_test, sum) != 55) {
    printf("FAILED at fold, at %i\n", __LINE__);
  } else {
    printf("PASSED at fold\n");
  }

  /**********************************************************
                        testing filter
  ***********************************************************/

  fint filter = fat_filter(int, map_test, even);
  if (fat_len(int, filter) != 3) {
    printf("FAILED at filter\n");
  }
  for (size_t i = 0; i < fat_len(int, filter); i++) {
    if (filter[i] % 2 != 0) {
      printf("FAILED at filter\n");
      passed = 0;
    }
  }
  if (passed) {
    printf("PASSED at filter\n");
  } else
    passed = 1;

  /**********************************************************
                          testing sort
  ***********************************************************/

  fint sort_int = fat_empty(int);
  fchar sort_char = fat_empty(char);
  ffloat sort_float = fat_empty(float);
  fut sort_ut = fat_empty(ut);
  fst sort_st = fat_empty(st);
  fstr sort_str = fat_empty(str);
  for (int i = 55; i >= 0; i--) {
    sort_int = fat_push(int, sort_int, i);
    sort_char = fat_push(char, sort_char, i);
    sort_float = fat_push(float, sort_float, (float)i);
    utmp.i = i;
    sort_ut = fat_push(ut, sort_ut, utmp);
    stmp.i = i;
    stmp.f = (float)i;
    memset(stmp.s, '\0', 4);
    stmp.s[0] = 'a' + i;
    sort_st = fat_push(st, sort_st, stmp);
    sort_str = fat_push(str, sort_str, stmp.s);
  }
  fat_sort(int, sort_int, CMP(int));
  fat_sort(char, sort_char, CMP(char));
  fat_sort(float, sort_float, CMP(float));
  fat_sort(ut, sort_ut, CMP(ut));
  fat_sort(st, sort_st, CMP(st));
  fat_sort(str, sort_str, CMP(str));
  for (size_t i = 0; i < fat_len(int, sort_int); i++) {
    if (0 != (CMP(int)(sort_int[i], (int)i)) ||
        0 != (CMP(char)(sort_char[i], (char)i)) ||
        0 != (CMP(float)(sort_float[i], (float)i)) ||
        0 != (utmp.i = i, CMP(ut)(sort_ut[i], utmp)) ||
        0 != (stmp.i = (int)i, stmp.f = (float)i, memset(stmp.s, '\0', 4),
              stmp.s[0] = 'a' + (int)i, CMP(st)(sort_st[i], stmp)) ||
        0 != (CMP(str)(sort_str[i], stmp.s))) {
      printf("{%d, %s ,%f},{%d, %s ,%f}\n", stmp.i, stmp.s, stmp.f,
             sort_st[i].i, sort_st[i].s, sort_st[i].f);
      printf("FAILED at sort, at %d\n", __LINE__);
      passed = 0;
    }
  }
  if (passed) {
    printf("PASSED at sort\n");
  } else
    passed = 1;

  /**********************************************************
                       testing bsearch
  ***********************************************************/
  int tmp;
  if ((tmp = fat_bsearch(int, sort_int, fint_cmp, 0)) != 0) {
    printf("FAILED at bsearch, at %d\n", __LINE__);
    printf("%d", tmp);
    passed = 0;
  }
  if ((tmp = fat_bsearch(int, sort_int, fint_cmp, 55)) != 55) {
    printf("FAILED at bsearch, at %d\n", __LINE__);
    printf("%d", tmp);
    passed = 0;
  }
  if ((tmp = fat_bsearch(int, sort_int, fint_cmp, 666)) != -1) {
    printf("FAILED at bsearch, at %d\n", __LINE__);
    printf("%d", tmp);
    passed = 0;
  }
  if ((tmp = fat_bsearch(int, sort_int, fint_cmp, -666)) != -1) {
    printf("FAILED at bsearch, at %d\n", __LINE__);
    printf("%d", tmp);
    passed = 0;
  }
  if (passed) {
    printf("PASSED at bsearch\n");
  } else
    passed = 1;

  /**********************************************************
                        testing free
  ***********************************************************/

  fat_free(int, NULL);  // no action if pointer is NULL

  fint_free(newfrom2);
  fat_free(int, sort_int);
  fat_free(char, sort_char);
  fat_free(float, sort_float);
  fat_free(ut, sort_ut);
  fat_free(st, sort_st);
  fat_free(str, sort_str);

  fat_free(int, filter);
  fat_free(int, map_test);
  fat_free(int, newfrom);
  fat_free(int, duparr);

  fat_free(char, fchar_arr);
  fat_free(int, fint_arr);
  fat_free(float, ffloat_arr);
  fat_free(ut, fut_arr);
  fat_free(st, fst_arr);
  fat_free(str, fstr_arr);
  return 0;
}
