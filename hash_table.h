#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include "linked_list.h"
#include "common.h"
/**
 * @file hash_table.h
 * @author   Jonathan Kjellén, Max Sundström 
 * @date 14 Sep 2019
 * @brief Simple hash table that maps integer keys to string values.
 *
 * Here typically goes a more extensive explanation of what the header
 * defines. Doxygens tags are words preceeded by either a backslash @\
 * or by an at symbol @@.
 *
 * @see http://wrigstad.com/ioopm19/assignments/assignment1.html
 */
 
#define Success(v)      (option_t) { .success = true, .value = v };
#define Failure()       (option_t) { .success = false };
#define Successful(o)   (o.success == true)
#define Unsuccessful(o) (o.success == false)

typedef struct entry entry_t ;
typedef struct hash_table ioopm_hash_table_t ;

typedef bool(*ioopm_predicate)(elem_t key, elem_t value, elem_t extra, ioopm_hash_table_t *ht);
typedef void(*ioopm_apply_function)(elem_t key, elem_t *value, elem_t *extra);
typedef bool(*ioopm_eq_function)(elem_t key1, elem_t key2);
typedef unsigned long(*ioopm_hash_function)(elem_t value);




/// @brief hashes a string
/// @param value string to hash
/// @return the hashed form of value as an unsigned long
unsigned long string_knr_hash(elem_t value);

/// @brief hashes an int to unsigned long
/// @param value integer to hash
/// @return value in the form of an unsigned long
unsigned long int_knr_hash(elem_t value);


/// @brief Update an existing hashtable
/// @param ht hashtable operated upon
/// @param hash_function function to hash keys
/// @param eq_key_func  function to compare keys
/// @param eq_value_func function to compare values
/// @param pred_func  function to check conditions on elem_t's
/// @return Updated version of ht
ioopm_hash_table_t *ioopm_hash_table_resize(ioopm_hash_table_t *ht, ioopm_hash_function hash_function, ioopm_eq_function eq_key_func, ioopm_eq_function eq_value_func, ioopm_predicate pred_func);

/// @brief Create a new hash table
/// @param hash_function function to hash keys
/// @param eq_key_func  function to compare keys
/// @param eq_value_func function to compare values
/// @param pred_func  function to check conditions on elem_t's
/// @return A new empty hash table
ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function hash_function, ioopm_eq_function eq_key_func, ioopm_eq_function eq_value_func, ioopm_predicate pred_func);

/// @brief Delete a hash table and free its memory
/// param ht a hash table to be deleted
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

/// @brief add key => value entry in hash table ht
/// @param ht hash table operated upon
/// @param key key to insert
/// @param value value to insert
/// @return the hash table with the new element inserted
ioopm_hash_table_t *ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value);

/// @brief lookup value for key in hash table ht
/// @param ht hash table operated upon
/// @param key key to lookup
/// @return false if key is not in ht, true + value mapped to key if key is in ht
option_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key);

/// @brief remove any mapping from key to a value
/// @param ht hash table operated upon
/// @param key key to remove
/// @return false if key is not in ht, true + value mapped to by key if key is in ht
option_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key);

/// @brief returns the number of key => value entries in the hash table
/// @param ht hash table operated upon
/// @return the number of key => value entries in the hash table
size_t ioopm_hash_table_no_entries(ioopm_hash_table_t *ht);

/// @brief checks if the hash table is empty
/// @param ht hash table operated upon
/// @return true is size == 0, else false
bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht);


/// @brief clear all the entries in a hash table
/// @param ht hash table operated upon
void ioopm_hash_table_clear(ioopm_hash_table_t *ht);

/// @brief return the keys for all entries in a hash map (in no particular order, but same as ioopm_hash_table_values)
/// @param ht hash table operated upon
/// @return a linked list of keys for hash table ht
ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht);

/// @brief return the values for all entries in a hash map (in no particular order, but same as ioopm_hash_table_keys)
/// @param ht hash table operated upon
/// @return a linked list of values for hash table ht
ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht);

/// @brief check if a hash table has an entry with a given key
/// @param ht hash table operated upon
/// @param key the key sought
/// @return true if key is in ht, else false
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key);

/// @brief check if a hash table has an entry with a given value
/// @param ht hash table operated upon
/// @param value the value sought
/// @return true if value is in ht, else false
bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value);

/// @brief check if a predicate is satisfied by all entries in a hash table
/// @param ht hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
/// @return true if pred is satisfied by all entries in ht, else false
bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, elem_t arg);

/// @brief check if a predicate is satisfied by any entry in a hash table
/// @param ht hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
/// @return true if pred is satisfied by any entry in ht, else false
bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, elem_t arg);

/// @brief apply a function to all entries in a hash table
/// @param ht hash table operated upon
/// @param apply_fun the function to be applied to all elements
/// @param arg extra argument to apply_fun
void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, elem_t *arg);

/// @brief checks if two keys are identical
/// @param key first key to compare
/// @param value_ignored value connected to the key
/// @param x second key to compare
/// @param ht hash table operated upon
/// @return true if key == x, else false
bool key_equiv(elem_t key, elem_t value_ignored, elem_t x, ioopm_hash_table_t *ht);

/// @brief checks if two values are identical
/// @param key_ignored key connected to value
/// @param value first value to compare
/// @param x second value to compare
/// @param ht hash table operated upon
/// @return true if value == x, else false
bool value_equiv(elem_t key_ignored, elem_t value, elem_t x,ioopm_hash_table_t *ht);

/// @brief changes the value to x
/// @param key key connected to value
/// @param value value to be changed
/// @param x value that value will be changed to
void change_value(elem_t key, elem_t *value, elem_t *x);