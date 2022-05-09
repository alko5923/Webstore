#pragma once
#include "common.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct link ioopm_link_t; 
typedef struct list ioopm_list_t;
typedef struct iter ioopm_list_iterator_t;

/// @brief Applies extra to either key or value depending on the situation
typedef void(*ioopm_apply_char_function)(int key, elem_t *value, void *extra);

/// @brief returns either true or false if the pred condition is fulfilled by the specified entries
typedef bool(*ioopm_char_predicate)(int key, elem_t *value, void *extra);

/// Compares two elements and returns true if they are equal
typedef bool(*ioopm_eq_function)(elem_t a, elem_t b);

/// @brief replaces value by extra 
/// @param key key mapped to value
/// @param value value to be changed
/// @param extra value that value will be updated to
void ioopm_linked_list_change(int key, elem_t *value, void *extra);


/// @brief Creates a new empty list
/// @param func function used to compare values
/// @return an empty linked list
ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function func);

/// @brief Create an iterator for a given list
/// @param the list to be iterated over
/// @return an iteration positioned at the start of list
ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list);

/// @brief compares two values
/// @param key key mapped to value
/// @param value first value to compare
/// @param extra second value to compare
/// @return true if extra == value else false
bool ioopm_linked_list_compare_to_void(int key, elem_t *value, void *extra);

/// @brief compares two values
/// @param value_1 first value to compare
/// @param value_2 second value to compare
/// @return true if value_1 == value_2 else false
bool ioopm_linked_list_compare(elem_t value_1, elem_t value_2);

/// @brief Tear down the linked list and return all its memory (but not the memory of the elements)
/// @param list the list to be destroyed
void ioopm_linked_list_destroy(ioopm_list_t *list);

/// @brief Insert at the end of a linked list in O(1) time
/// @param list the linked list that will be appended
/// @param value the value to be appended
void ioopm_linked_list_append(ioopm_list_t *list, elem_t value);

/// @brief Insert at the front of a linked list in O(1) time
/// @param list the linked list that will be prepended
/// @param value the value to be appended
void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value);

/// @brief Insert an element into a linked list in O(n) time.
/// The valid values of index are the range of int's, index is updated
/// to a valid index in the function.
/// @param list the linked list that will be extended
/// @param index the position in the list
/// @param value the value to be appended
void ioopm_linked_list_insert(ioopm_list_t *list, int index, elem_t value);

/// @brief Remove an element from a linked list in O(n) time.
/// The valid values of index are [0,n-1] for a list of n elements,
/// where 0 means the first element and n-1 means the last element.
/// @param list the linked list that will be shortened
/// @param index the position in the list
/// @return the value that was removed
elem_t ioopm_linked_list_remove(ioopm_list_t *list, int index);

/// @brief Retrieve an element from a linked list in O(n) time.
/// The valid values of index are [0,n-1] for a list of n elements,
/// where 0 means the first element and n-1 means the last element.
/// @param list the linked list that will be extended
/// @param index the position in the list
/// @return the value at the given position
elem_t ioopm_linked_list_get(ioopm_list_t *list, int index);

/// @brief Test if an element is in the list
/// @param list the linked list
/// @param value the value sought
/// @return true if element is in the list, else false
bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t value);

/// @brief Lookup the number of elements in the linked list in O(1) time
/// @param list the linked list
/// @return the number of elements in the list
size_t ioopm_linked_list_size(ioopm_list_t *list);

/// @brief Test whether a list is empty or not
/// @param list the linked list
/// @return true if the number of elements int the list is 0, else false
bool ioopm_linked_list_is_empty(ioopm_list_t *list);

/// @brief Remove all elements from a linked list
/// @param list the linked list
void ioopm_linked_list_clear(ioopm_list_t *list);

/// @brief Test if a supplied property holds for all elements in a list.
/// The function returns as soon as the return value can be determined.
/// @param list the linked list
/// @param prop the property to be tested (function pointer)
/// @param extra an additional argument (may be NULL) that will be passed to all internal calls of prop
/// @return true if prop holds for all elements in the list, else false
bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_char_predicate prop, void *extra);

/// @brief Test if a supplied property holds for any element in a list.
/// The function returns as soon as the return value can be determined.
/// @param list the linked list
/// @param prop the property to be tested
/// @param extra an additional argument (may be NULL) that will be passed to all internal calls of prop
/// @return true if prop holds for any elements in the list, else false
bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_char_predicate prop, void *extra);

/// @brief Apply a supplied function to all elements in a list.
/// @param list the linked list
/// @param fun the function to be applied
/// @param extra an additional argument (may be NULL) that will be passed to all internal calls of fun
void ioopm_linked_apply_to_all(ioopm_list_t *list, ioopm_apply_char_function fun, void *extra);



