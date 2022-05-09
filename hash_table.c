#include "hash_table.h"
#include "linked_list.h"
#include "common.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define no_Buckets 17

typedef struct entry entry_t;

struct entry
{
  elem_t key;     
  elem_t value;  
  entry_t *next; 
};

struct hash_table
{
  entry_t **buckets;
  ioopm_hash_function hash_function;
  ioopm_eq_function eq_key_function;
  ioopm_eq_function eq_value_function;
  ioopm_predicate pred_function;
  int size;
  int no_buckets;
};


bool int_key_eq(elem_t int_1, elem_t int_2){
  return int_1.number == int_2.number;
}

bool str_key_eq(elem_t string_1, elem_t string_2){
  if (strcmp(string_1.str, string_2.str) == 0)
  {
    return true;
  }
  return false;
}

unsigned long string_knr_hash(elem_t value)
{
  const char *str = value.str;
  unsigned long result = 0;
  do
    {
      result = result * 31 + *str;
    }
  while (*++str != '\0');
  return result;
}

unsigned long int_knr_hash(elem_t value){
  unsigned long number = value.number;
  return number;
}


static entry_t *entry_create(elem_t key, elem_t value, entry_t *next)
{
  entry_t *result= calloc(1, sizeof(entry_t));
  entry_t new_entry = { .key = key, .value = value, .next = next};
  *result = new_entry;
  return result;
}


ioopm_hash_table_t *ioopm_hash_table_create_dynamic(ioopm_hash_table_t *ht, ioopm_hash_function hash_function, ioopm_eq_function eq_key_func, ioopm_eq_function eq_value_func, ioopm_predicate pred_func, int buckets){
  if (ht->no_buckets == 0)
  {
    ht->no_buckets = buckets;
    ht->buckets = calloc(ht->no_buckets, sizeof(entry_t));
    ht->size = 0;
    ht->hash_function = hash_function;
    ht->eq_key_function = eq_key_func;
    ht->eq_value_function = eq_value_func;
    ht->pred_function = pred_func;
    return ht;
  }
 
  
  if (ht->size > 0){
    entry_t array_of_keys_values[ht->size];
    ioopm_list_t *list_of_keys = ioopm_hash_table_keys(ht);
    int size = ioopm_linked_list_size(list_of_keys);

    
    for (int i = 0; i < size; i++){
      elem_t key = ioopm_linked_list_get(list_of_keys, i);
      option_t value = ioopm_hash_table_lookup(ht, key);
      
      entry_t gotten_entry = { .key = key, .value = value.value};
      array_of_keys_values[i] = gotten_entry;
    }
    ioopm_hash_table_clear(ht);
    
    ht->no_buckets = ht->no_buckets * 2;
    ht->buckets = realloc(ht->buckets, ht->no_buckets * sizeof(entry_t));
    for (int i = 0; i < ht->no_buckets; i++){
      ht->buckets[i] = NULL;
    }
    ht->size = 0;
    
    for (int i = 0; i < ioopm_linked_list_size(list_of_keys); i++){
      ht = ioopm_hash_table_insert(ht, array_of_keys_values[i].key, array_of_keys_values[i].value);
    }
    ioopm_linked_list_destroy(list_of_keys);
  }
  return ht;
  
}

ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function hash_function, ioopm_eq_function eq_key_func, ioopm_eq_function eq_value_func, ioopm_predicate pred_func)
{
  ioopm_hash_table_t *ht = calloc(1, sizeof(ioopm_hash_table_t));
  ht->no_buckets = 0;
  int buckets = 17;
  return ioopm_hash_table_create_dynamic(ht, hash_function, eq_key_func, eq_value_func, pred_func, buckets);
}


static entry_t *find_entry_for_key(entry_t *entry, elem_t key, ioopm_hash_table_t *ht)
{

  if (entry == NULL)
  {
    return entry;
  }
  else if (ht->eq_key_function(entry->key, key))
  {
    return entry;
  }
  else
  {
    while (entry->next != NULL){
      if (ht->eq_key_function(entry->next->key, key))
      {
        return entry->next;
      }
      else
      {
        entry = entry->next;
      }
      
    }
    return entry;
    
  }
}

static entry_t *find_previous_entry_for_key(entry_t *entry, elem_t key, ioopm_hash_table_t *ht)
{
  if (entry == NULL)
  {
    return entry;
  }
  else if (ht->eq_key_function(entry->key, key))
  {
    return entry;
  }
  else
  {
    while (entry->next != NULL){
      if (ht->eq_key_function(entry->next->key, key))
      {
        return entry;
      }
      else
      {
        entry = entry->next;
      }
      
    }
    return entry;
    
  }
}


size_t ioopm_hash_table_no_entries(ioopm_hash_table_t *ht)
{
  return ht->size;
}



ioopm_hash_table_t *ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{

  size_t size = ht->size;
  if (size > (ht->no_buckets * 0.75)){
    ht = ioopm_hash_table_create_dynamic(ht, ht->hash_function, ht->eq_key_function, ht->eq_value_function, ht->pred_function, ht->no_buckets);
  }
  unsigned long int_key = ht->hash_function(key);
  
  unsigned long hash_bucket = int_key % ht->no_buckets;
  
  
  entry_t *entry = find_entry_for_key(ht->buckets[hash_bucket], key, ht);
  
  
  if (entry == NULL)
  {
    //printf("%ld", hash_bucket);
    entry_t *first_entry = ht->buckets[hash_bucket];
    entry_t *next_entry = entry_create(key, value, first_entry);
    ht->buckets[hash_bucket] = next_entry;

    ht->size = ht->size + 1;
  }
  else if (ht->eq_key_function(entry->key, key))
  {
    entry->value = value;
  }
  else
  {
    entry->next = entry_create(key, value, NULL);
    ht->size = ht->size + 1;
  }
  return ht;
}


option_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key)
{
  unsigned long int_key = ht->hash_function(key);
  unsigned long hash_bucket = int_key % ht->no_buckets;
  entry_t *tmp = find_entry_for_key(ht->buckets[hash_bucket], key, ht); 
  if (tmp == NULL)
  {
    return Failure();
  }
  else if (ht->eq_key_function(tmp->key, key))
    {
      
      return Success(tmp->value);
    }
  else
    {
      return Failure();
    }
}

void entry_destroy(entry_t *entry)
{
  
  
  free(entry);
  entry = NULL;
}



option_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key)
{
  unsigned long int_key = ht->hash_function(key);
  unsigned long hash_bucket = int_key % ht->no_buckets;
  entry_t *tmp = find_previous_entry_for_key(ht->buckets[hash_bucket], key, ht);
  
  
  if (tmp == NULL)
  {
    option_t result = { .success = false };
    return result;
  }
  else if (ht->eq_key_function(tmp->key, key))
  {
    elem_t value = tmp->value;
    entry_t *next_entry = tmp->next;
    tmp->next = next_entry;   
    entry_destroy(tmp);
    option_t result = { .success = true, .value = value };
    ht->buckets[hash_bucket] = next_entry;
    
    ht->size = ht->size - 1;
    return result;
  }
  else if (ht->eq_key_function(tmp->next->key, key))
  {
    elem_t value = tmp->next->value;
    entry_t *next_entry = tmp->next->next;
    entry_destroy(tmp->next);
    tmp->next = next_entry;
    option_t result = { .success = true, .value = value };
    
    ht->size = ht->size - 1;
    return result;
  }
  else if (ht->eq_key_function(ht->buckets[hash_bucket]->key, tmp->key))
  {
    elem_t value = tmp->value;
    entry_t *next_entry = tmp->next;
    entry_destroy(tmp);
    ht->buckets[hash_bucket] = next_entry;
    option_t result = { .success = true, .value = value };
    ht->size = ht->size - 1;
    return result;
  }
  else
  {
    option_t result = { .success = false };
    return result;
  }
  
  
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
  ioopm_hash_table_clear(ht);     
  free(ht->buckets);
  free(ht);                           
}



bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
  for( int i = 0; i < ht->no_buckets; i++)
  {
    
    entry_t *cursor = ht->buckets[i];
    if(cursor){
      return false;
    }
  }
  return true;
}

void ioopm_hash_table_clear(ioopm_hash_table_t *ht)
{
  for( int i = 0; i < ht->no_buckets; i++){
      
    entry_t *cursor = ht->buckets[i];
    while(cursor){
      entry_t *tmp = cursor;            
      cursor = cursor->next;
      entry_destroy(tmp);            
    }
    cursor = NULL;              
    ht->buckets[i] = cursor;
  }
}


ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht)
{
  ioopm_list_t *list = ioopm_linked_list_create(ioopm_linked_list_compare);
  for( int i = 0; i < ht->no_buckets; i++){
    entry_t *cursor = ht->buckets[i]; 
    
    if (cursor){
      while(cursor->next){
        elem_t key = cursor->key;
        ioopm_linked_list_append(list, key);
        cursor = cursor->next;              
      }
      elem_t key = cursor->key;
      ioopm_linked_list_append(list, key);
      
    }
    
  }
  
  return list;
}



ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht)
{
  ioopm_list_t *list = ioopm_linked_list_create(ioopm_linked_list_compare);
  for( int i = 0; i < ht->no_buckets; i++){
      entry_t *cursor = ht->buckets[i]; 
    if (cursor){
      while(cursor->next){                     
        elem_t value = cursor->value;
        ioopm_linked_list_append(list, value);
        cursor = cursor->next;              
        }
        elem_t value = cursor->value;
        ioopm_linked_list_append(list, value);
    }
  }
  return list;
}


 bool key_equiv(elem_t key, elem_t value_ignored, elem_t x, ioopm_hash_table_t *ht)
{
  return ht->eq_key_function(key, x);
}




bool value_equiv(elem_t key_ignored, elem_t value, elem_t x, ioopm_hash_table_t *ht)
{
  if(ht->eq_value_function(value, x))
  {
    return true;
  }
  return false;
}



void change_value(elem_t key, elem_t *value, elem_t *x)
{
  value->number = x->number;
}


bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred_func, elem_t arg)
{
  for( int i = 0; i < ht->no_buckets; i++){
      entry_t *cursor = ht->buckets[i];
      
    if (cursor){
      while(cursor){                      
        if(pred_func(cursor->key, cursor->value, arg, ht))
        {
          return true;
        }
          cursor = cursor->next;
        }
    }
  }
  return false;
}
  
bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, elem_t arg)
{
  for( int i = 0; i < ht->no_buckets; i++){
      entry_t *cursor = ht->buckets[i]; 
    if (cursor){
      while(cursor){                   
        if(pred(cursor->key, cursor->value, arg, ht))
        {
          cursor = cursor->next;
        }
        else
        {
          return false; 
        }
    }
  }
  }
  return true;
}

void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, elem_t *arg)
{
  for( int i = 0; i < ht->no_buckets; i++){
      entry_t *cursor = ht->buckets[i]; 
    if (cursor){
      while(cursor){                      
        apply_fun(cursor->key, &(cursor->value), arg);
        cursor = cursor->next;
      }
    }
  }
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key)
{
  return ioopm_hash_table_any(ht, ht->pred_function, key);
}

bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value)
{
  return ioopm_hash_table_any(ht, ht->pred_function, value);
}