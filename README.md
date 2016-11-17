Fat Array
===
An [sds](https://github.com/antirez/sds) inspired dynamic arbitrary typed (growing) array implementation.

### Why?
The goal is to have a one header, simple yet efficient and safe (growing) array implementation, that's usable in pure C. It works by abusing the macro pasting functionality to achieve a type-independent data structure.

Having to re-implement the wheel every time you want to make a simple array with some simple functionality is a burden, it's prone to mistakes and one ends up spending too much time doing so, when the focus should be on writing the application itself.

### Usage
```c
// at the top level
MAKEFAT(my_type)

```
If you want to be able to use the searching and sorting capabilities you should also use the macro FCMP for declaring the comparison function.
```c
FCMP(my_type, a, b){
	return a.key-b.key;
}
// the comparison function should work as strcmp, so if a < b it returns less than 1, if a == b it returns 0, and if a > b it returns more than 1
```

And that's it, you can now use all the defined functions for your type, here's an example.

```c
// inside a function
// it prepends an f before the type name, so you know it's now a fat array, or fat pointer array
fmy_cool_type var = fat_empty(my_type);

for(int i = 0; i < fat_len(my_type, var); i++){
	var = fat_push(my_type, my_variable);
}

```

## Macro Functions
All macro functions take a type as the first input, they start with the "fat\_" prefix and are defined as a convenience, all of them can be used by calling as fMytype\_funcname:

```c
// returns a new fat array with the specified size.
fat_new(T, size)

// puts the element at the end of the array, it tries to grow the array if there is not enough space at the end of the array, so the function returns a pointer, and should be used as arr = fat_push(type, arr, element).
fat_push(T, array, element)

// it returns the last element of the array and decreases it's length, returns (T){0} if the array is empty.
fat_pop(T, array)

// creates and returns a new array from the regular C array init on length len.
fat_newfrom(T, init, len)

// creates a new fat array of length zero, with 8 allocated spaces by default.
fat_empty(T)

// duplicates a fat array, returning the pointer for the new array.
fat_dup(T, array)

// increases allocated space to len, if len is smaller than the currently allocated space it does nothing.
fat_growzero(T, array, len)

// copies the base array of length len to the end of the first fat array.
fat_catlen(T, array, base, len)

// makes the array only contain items from start to end inclusive, can be used with negative numbers, which wrap around.
// fat_range(T, array, 1, -1) does not modify the array, for example.
// fat_range(T, array, 1, -2) removes the last element.
// fat_range(T, array, -3, -1) removes everything but the last 3 elements.
fat_range(T, array, start, end)

// Applies the function func to every element of the array, the function should have it's prototype as: T func_name(T).
fat_map(T, array, func)

// Applies the function func to every pair of elements of the array, the function should have it's prototype as: T func_name(T,T).
fat_fold(T, array, func)

// Applies the function func to every element, if the function return value is different than 0 it then pushes said element into a new fat array.
fat_filter(T, array, func)

// Sorts the array using the cmp function as a test for equality, it's implemented using quicksort and shellsort.
fat_sort(T, array, cmp)

// Searches for the needle element inside of the array, using the cmp function for a test of equality, the array must be sorted before using.
fat_bsearch(T, array, cmp, needle)

// Returns the length of the array.
fat_len(T, array)

// Returns the number of empty spaces in the array.
fat_avail(T, array)

// Set array length to have newlen of length.
fat_setlen(T, array, newlen)

// Increments the length of the array by inc, accepts negative values.
fat_inclen(T, array, inc)

// Returns the number of allocated elements.
fat_alloc(T, array)

// Sets the number of allocated elements to newlen.
fat_setalloc(T, array, newlen)

// Frees the array.
fat_free(T, array)

```