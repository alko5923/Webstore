#include "linked_list.h"
#include "iterator.h"
#include "common.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>


struct link 
{
    elem_t value;
    ioopm_link_t *next;
};


struct list
{
  ioopm_link_t *first;
  ioopm_link_t *last;
  size_t size;
  ioopm_eq_function func;
};

struct iter
{
  ioopm_link_t *current;
  ioopm_list_t *list;
};

ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function func)
{
    ioopm_list_t *list = calloc(1, sizeof(ioopm_list_t));
    list->func = func;
    return list;
}

ioopm_link_t *ioopm_link_new(elem_t value, ioopm_link_t *next)
{
  ioopm_link_t *result = calloc(1, sizeof(ioopm_link_t));

  if (result)
    {
      result->value = value;
      result->next = next;
    }
  return result;
}

void ioopm_linked_list_destroy(ioopm_list_t *list)
{
    ioopm_linked_list_clear(list);
    free(list);
    list = NULL;
}

void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value)
{
    ioopm_link_t *link = ioopm_link_new(value, list->first);
    if (list->first == NULL)
     {
        list->last = link;
     }

  list->first = link;
  list->size = list->size + 1;
}

void ioopm_linked_list_append(ioopm_list_t *list, elem_t value)
{
  if (list->last == NULL)
    { 
      ioopm_linked_list_prepend(list, value);
    }
  else
    {
      ioopm_link_t *link = ioopm_link_new(value, NULL);
      list->last->next = link;
      list->last = link;
      list->size = list->size + 1;
    }
}

ioopm_link_t *ioopm_list_inner_find_previous(ioopm_link_t *link, size_t index)
{
  ioopm_link_t *cursor = link;
  for (size_t i = 0; i < index; ++i)
    {
      if (i + 1 == index)
      {
        return cursor;
      }
      cursor = cursor->next;
    }

  return cursor;
}

size_t ioopm_list_inner_adjust_index(int index, int upper_bound)
{
  if (index <= upper_bound && index >= 0)
  {
    return index;
  }
  else if (upper_bound == 0)
  {
    return 0;
  }
  else 
  {
    index = index % upper_bound;
    return index;
  }
  
}


void ioopm_linked_list_insert(ioopm_list_t *list, int index, elem_t value)
{
  size_t valid_index = ioopm_list_inner_adjust_index(index, list->size);
  
  ioopm_link_t *prev = ioopm_list_inner_find_previous(list->first, (valid_index));
  
  if (valid_index == 0)
  {
    ioopm_linked_list_prepend(list, value);
  }
  else if (valid_index == list->size)
  {
    ioopm_linked_list_append(list, value);
  }
  else
  {
    ioopm_link_t *next_entry = prev->next; 
    prev->next = ioopm_link_new(value, next_entry);
    list->size = list->size + 1;
  }
  
}


elem_t ioopm_linked_list_get(ioopm_list_t *list, int index)
{
  size_t valid_index = ioopm_list_inner_adjust_index(index, list->size);
  ioopm_link_t *link = ioopm_list_inner_find_previous(list->first ,(valid_index));
  if (list->size == 1 || valid_index == 0)
  {
    return link->value;
  }
  else
  {
    return link->next->value;
  }
  
}

bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t value)
{
  ioopm_eq_function func = *(list->func);
  int size = list->size;
  ioopm_link_t *first = list->first;
  for(int i = 0; i < size;i++){
    elem_t value2 = first->value;
    bool result = func(value2, value);
    if (result)
    {
      return true;
    }
    first = first->next;
  }
  return false;
}

elem_t ioopm_linked_list_remove(ioopm_list_t *list, int index)
{
  int size = list->size;
  if (index == 0)
  {
    ioopm_link_t *next = list->first->next;
    elem_t value = list->first->value;
    free(list->first);
    list->first = NULL;
    list->first = next;
    list->size = (list->size - 1);
    return value;
  }
  else if (index == size)
  {
    ioopm_link_t *prev = ioopm_list_inner_find_previous(list->first, index-1);
    elem_t value = list->last->value;
    free(list->last);
    list->last = NULL;
    list->last = prev;
    list->last->next = NULL;
    list->size = (list->size - 1);
    return value;
  }
  else
  {
    ioopm_link_t *prev = ioopm_list_inner_find_previous(list->first, index-1);
    ioopm_link_t *to_remove = prev->next;
    prev->next = prev->next->next;
    elem_t value = to_remove->value;
    free(to_remove);
    to_remove = NULL;
    list->size = (list->size - 1);
    return value;
  }
}

bool ioopm_linked_list_is_empty(ioopm_list_t *list)
{
  if (list->size == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void ioopm_linked_list_clear(ioopm_list_t *list)
{
  size_t size = list->size;
  for(size_t i = 0; i < size; i++)
  {
    
    ioopm_linked_list_remove(list,0);
  }
  list->first = NULL;
  list->last = NULL;
}

size_t ioopm_linked_list_size(ioopm_list_t *list)
{
  return list->size;
}

bool ioopm_linked_list_compare(elem_t a, elem_t b)
{
  if (a.number == b.number)
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool ioopm_linked_list_compare_to_void(int key, elem_t *value, void *extra)
{
  if (value->number == *(int *)extra)
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_char_predicate prop, void *extra){
  ioopm_link_t *link = list->first;
  
  for(size_t i = 0; i < list->size; i++){
    elem_t value = link->value;
    bool a = prop(i, &value, extra);
    if (a == false)
    {
      return false;
    }
    else
    {
      link = link->next;
    }
    
  }
  return true;
}

bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_char_predicate prop, void *extra){
  ioopm_link_t *link = list->first;
  for(size_t i = 0; i < list->size; i++){
    elem_t value = link->value;
    bool a = prop(i, &value, extra);
    if (a == true)
    {
      return true;
    }
    else
    {
      link = link->next;
    }
    
  }
  return false;
}

void ioopm_linked_list_change(int key, elem_t *value, void *extra){
  value->number = *(int*)extra;  
} 

void ioopm_linked_apply_to_all(ioopm_list_t *list, ioopm_apply_char_function fun, void *extra){
  ioopm_link_t *link = list->first;
  for(size_t i = 0; i < list->size; i++){
    fun(i, &(link->value), extra);
    link = link->next;
  }
} 


ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list)
{
  ioopm_list_iterator_t *result = calloc(1, sizeof(ioopm_list_iterator_t));
  result->current = list->first;
  result->list = list;
  return result;
}

bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter){
  return (iter->current->next);
}

elem_t ioopm_iterator_next(ioopm_list_iterator_t *iter)
{
  if (ioopm_iterator_has_next(iter))
  {
    iter->current = iter->current->next;
    return (iter->current->value);
  }
  elem_t value = { .b_ool = false};
  return value;
}

void ioopm_iterator_reset(ioopm_list_iterator_t *iter){
  iter->current = iter->list->first;
}

elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter){
  return (iter->current->value);
}

void ioopm_iterator_destroy(ioopm_list_iterator_t *iter){
  free(iter);
  iter = NULL;
}

elem_t ioopm_iterator_remove(ioopm_list_iterator_t *iter){
  ioopm_link_t *to_remove = iter->current;
  elem_t value = to_remove->value;
  iter->current = to_remove->next; 
  //free(to_remove);
  return value;
}


elem_t ioopm_make_elem(int int_value){
  elem_t value = { .number = int_value};
  return value;
}