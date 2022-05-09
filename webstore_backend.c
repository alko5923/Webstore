# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <string.h>
#include "backend.h"
#include "linked_list.h"
#include "hash_table.h"
#include "common.h"

// -------------------------- FUNCTION DEFINITIONS -----------------------------

static merch_t *merch_create(char *name, char *desc, float price);
ioopm_hash_table_t *ioopm_add_merch(char *name, char *desc, float price, ioopm_hash_table_t *ht_names);
void destroy_merch(merch_t *merch);
void destroy_all_merch(ioopm_hash_table_t *ht);
void ioopm_remove_merch(elem_t chosen_key, ioopm_hash_table_t *ht_names, ioopm_hash_table_t *ht_shelves, ioopm_list_t *shopping_carts);
void ioopm_edit_merch(elem_t chosen_key, char *new_name, char *new_desc, float new_price, ioopm_hash_table_t *ht_names, ioopm_hash_table_t *ht_shelves, ioopm_list_t *shopping_carts);
size_t count_total_quantity_for_merch(ioopm_list_t *locations);
static void change_key(elem_t old_key, elem_t new_key, ioopm_hash_table_t *ht_names, ioopm_hash_table_t *ht_shelves);

item_c *item_create(char *name, float price, int quantity);
cart_t *ioopm_create_cart(char *name);
void remove_item_from_cart(cart_t *shopping_cart, char *name);
void remove_item_from_carts(ioopm_list_t *shopping_carts, char *name);
void clear_cart(cart_t *cart);
void destroy_cart(cart_t *cart);
void remove_all_carts(ioopm_list_t *shopping_carts);
void check_shopping_carts(ioopm_list_t *shopping_carts, char* new_name, float new_price, char *old_name);
option_t is_in_cart(char *name, ioopm_list_t *cart);
bool cart_is_empty(cart_t *shopping_cart);
float calculate_cost(cart_t *shopping_cart);
cart_t *cart_check(char *name, ioopm_list_t *shopping_carts);

static shelf_t *shelf_create(char *name, size_t quantity);
void ioopm_replenish_stock(ioopm_hash_table_t *ht_names, ioopm_hash_table_t *ht_shelves, char *location, int new_quantity, elem_t chosen_key);
void change_quantity_on_locations(int quantity, ioopm_list_t *locations, ioopm_hash_table_t *ht_shelves);


// ---------------------------- STRUCTS ----------------------------------------

struct merch
{
    char *name;
    char *desc;
    float price;
    ioopm_list_t *locations;
};

struct shelf
{
    char *name;
    size_t quantity;
};


struct cart 
{
    char *name;
    ioopm_list_t *items;
};

struct item_in_cart
{
    char *name;
    float price;
    int quantity;
    
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

struct entry
{
  elem_t key;     
  elem_t value;  
  entry_t *next; 
};


//--------------------------- MERCH FUNCTIONS ----------------------------------


static merch_t *merch_create(char *name, char *desc, float price)
{
  merch_t *result= calloc(1, sizeof(merch_t));
  ioopm_list_t *locations = ioopm_linked_list_create(ioopm_linked_list_compare);
  merch_t new_merch = { .name = name, .desc = desc, .price = price, .locations = locations};
  *result = new_merch;
  return result;
}


ioopm_hash_table_t *ioopm_add_merch(char *name, char *desc, float price, ioopm_hash_table_t *ht_names)
{
    elem_t key = { .str = name};
    bool has_key = ioopm_hash_table_has_key(ht_names, key);
    if (has_key)
    {
        printf("The item is already in the system\n");
        free(name);
        free(desc);
        return ht_names;
    }
    else
    {
        merch_t *new_merch = merch_create(name, desc, price);
        elem_t value = { .pointer = new_merch};
        ht_names = ioopm_hash_table_insert(ht_names, key, value);
        return ht_names;
    }
}

void destroy_merch(merch_t *merch)
{
    size_t size = ioopm_linked_list_size(merch->locations);
    for (int i = 0; i < size; i++)
    {
        elem_t value = ioopm_linked_list_get(merch->locations, i);
        shelf_t *shelf = value.pointer;
        char *name = shelf->name;
        free(name);
        free(shelf);
    }
    ioopm_linked_list_destroy(merch->locations);
    free(merch->name);
    free(merch->desc);
    free(merch);
    
}


void destroy_all_merch(ioopm_hash_table_t *ht)
{
    for(int i = 0; i < ht->no_buckets; i++){
      
    entry_t *cursor = ht->buckets[i]; 
    while(cursor){                      
      entry_t *tmp = cursor;            
      cursor = cursor->next;              
      destroy_merch(tmp->value.pointer);            
    }
  }
}


void ioopm_remove_merch(elem_t chosen_key, ioopm_hash_table_t *ht_names, ioopm_hash_table_t *ht_shelves, ioopm_list_t *shopping_carts)
{
    option_t item = ioopm_hash_table_lookup(ht_names, chosen_key);
    merch_t *merch = item.value.pointer;
    ioopm_list_t *list_of_locations = merch->locations;
    size_t num_of_locations = ioopm_linked_list_size(list_of_locations);
    for (int i = 0; i < num_of_locations; i++)
    {
        elem_t to_remove = ioopm_linked_list_get(list_of_locations, i);
        
        ioopm_hash_table_remove(ht_shelves, to_remove);
    }
    
    remove_item_from_carts(shopping_carts, merch->name);
    ioopm_hash_table_remove(ht_names, chosen_key);
    destroy_merch(merch);
}


void ioopm_edit_merch(elem_t chosen_key, char *new_name, char *new_desc, float new_price, ioopm_hash_table_t *ht_names, ioopm_hash_table_t *ht_shelves, ioopm_list_t *shopping_carts)
{
    option_t item = ioopm_hash_table_lookup(ht_names, chosen_key);
    merch_t *merch = item.value.pointer;
    
    if (strcmp(chosen_key.str, new_name) == 0)
    {
        check_shopping_carts(shopping_carts, new_name, new_price, merch->name);
        free(merch->desc);
        merch->desc = new_desc;
        merch->price = new_price;
    }
    else
    {
        elem_t new_key = { .str = new_name};
        change_key(chosen_key, new_key, ht_names, ht_shelves);
        check_shopping_carts(shopping_carts, new_name, new_price, merch->name);
        free(merch->desc);
        free(merch->name);
        merch->desc = new_desc;
        merch->price = new_price;
        merch->name = new_name;
        ioopm_list_t *list_of_locations = merch->locations;
        size_t size_of_list = ioopm_linked_list_size(list_of_locations);
        for (int i = 0; i < size_of_list; i++)
        {
            elem_t key = ioopm_linked_list_get(list_of_locations, i);
            option_t item_2 = ioopm_hash_table_lookup(ht_shelves, key);
            elem_t value = item_2.value;
            new_name = value.str;
        }

    }
    
}


size_t count_total_quantity_for_merch(ioopm_list_t *locations)
{
    size_t size_of_list = ioopm_linked_list_size(locations);
    size_t total_quantity = 0;
    for (int i = 0; i < size_of_list; i++)
    {
        elem_t location = ioopm_linked_list_get(locations, i);
        shelf_t *shelf = location.pointer;
        size_t quantity = shelf->quantity;
        total_quantity += quantity;
    }
    return total_quantity;
}


static void change_key(elem_t old_key, elem_t new_key, ioopm_hash_table_t *ht_names, ioopm_hash_table_t *ht_shelves)
{
    option_t entry_to_update = ioopm_hash_table_lookup(ht_names, old_key);
    elem_t entry = entry_to_update.value;
    
    ioopm_hash_table_remove(ht_names, old_key);
    
    ioopm_hash_table_insert(ht_names, new_key, entry);
    
}


// -------------------------- CART FUNCTIONS -----------------------------------


item_c *item_create(char *name, float price, int quantity)
{
    item_c *result = calloc(1, sizeof(item_c));
    item_c new_item = { .name = name, .price = price, .quantity = quantity };
    *result = new_item;
    return result;
}


cart_t *ioopm_create_cart(char *name)
{
    cart_t *result = calloc(1, sizeof(cart_t));
    ioopm_list_t *items = ioopm_linked_list_create(ioopm_linked_list_compare);
    cart_t new_cart = { .name = name, .items = items };
    *result = new_cart;
    return result;
}


void remove_item_from_cart(cart_t *shopping_cart, char *name)
{
    size_t amount_of_items = ioopm_linked_list_size(shopping_cart->items);
    for (int i = 0; i < amount_of_items; i++)
    {
        elem_t item_elem = ioopm_linked_list_get(shopping_cart->items, i);
        item_c *item = item_elem.pointer;
        if (strcmp(item->name, name) == 0)
            {
                free(item);
                ioopm_linked_list_remove(shopping_cart->items, i);
            }
    }
}

void remove_item_from_carts(ioopm_list_t *shopping_carts, char *name)
{
    size_t amount_of_carts = ioopm_linked_list_size(shopping_carts);
    for (int i = 0; i < amount_of_carts; i++)
    {
        elem_t cart_elem = ioopm_linked_list_get(shopping_carts, i);
        cart_t *cart = cart_elem.pointer;
        ioopm_list_t *items = cart->items;
        size_t amount_of_items = ioopm_linked_list_size(items);
        for (int j = 0; j < amount_of_items; j++)
        {
            elem_t item_elem = ioopm_linked_list_get(items, i);
            item_c *item = item_elem.pointer;
            if (strcmp(item->name, name) == 0)
            {
                free(item);
                ioopm_linked_list_remove(items, i);
            }
        }
    }
}


void clear_cart(cart_t *cart)
{
    size_t amount_of_items = ioopm_linked_list_size(cart->items);
    for (int i = 0; i < amount_of_items; i++)
    {
        elem_t item_elem = ioopm_linked_list_get(cart->items, i);
        item_c *item = item_elem.pointer;
        free(item);
    }
    ioopm_linked_list_clear(cart->items);
}


void destroy_cart(cart_t *cart)
{
    free(cart->name);
    size_t amount_of_items = ioopm_linked_list_size(cart->items);
    for (int i = 0; i < amount_of_items; i++)
    {
        elem_t item_elem = ioopm_linked_list_get(cart->items, i);
        item_c *item = item_elem.pointer;
        free(item);
    }
    ioopm_linked_list_destroy(cart->items);
    free(cart);
}


void remove_all_carts(ioopm_list_t *shopping_carts)
{
    size_t size = ioopm_linked_list_size(shopping_carts);
    for (int i = 0; i < size; i++)
    {
        elem_t cart_elem = ioopm_linked_list_get(shopping_carts, i);
        cart_t *cart = cart_elem.pointer;
        destroy_cart(cart);
    }
    ioopm_linked_list_destroy(shopping_carts);
}


void check_shopping_carts(ioopm_list_t *shopping_carts, char* new_name, float new_price, char *old_name)
{
    size_t amount_of_carts = ioopm_linked_list_size(shopping_carts);
    for (int i = 0; i < amount_of_carts; i++)
    {
        elem_t cart_elem = ioopm_linked_list_get(shopping_carts, i);
        cart_t *cart = cart_elem.pointer;
        ioopm_list_t *items = cart->items;
        size_t amount_of_items = ioopm_linked_list_size(items);
        for (int j = 0; j < amount_of_items; j++)
        {
            elem_t item_elem = ioopm_linked_list_get(items, i);
            item_c *item = item_elem.pointer;
            if (strcmp(item->name, old_name) == 0)
            {
                item->name = new_name;
                item->price = new_price;
            }
        }
    }
}


option_t is_in_cart(char *name, ioopm_list_t *cart)
{
    size_t size_of_shopping_cart = ioopm_linked_list_size(cart);
    elem_t item1;
    for (int i = 0; i < size_of_shopping_cart; i ++)
    {
        item1 = ioopm_linked_list_get(cart, i);
        item_c *item = item1.pointer;
        char *item_name = item->name;
        if (strcmp(item_name, name) == 0)
        {
            option_t item = { .success = true, .value = item1 };
            return item;
        }
    }
    option_t item = { .success = false };
    return item;
}


bool cart_is_empty(cart_t *shopping_cart)
{
    size_t size_of_cart = ioopm_linked_list_size(shopping_cart->items);
    if (size_of_cart == 0)
    {
        return true;
    }
    return false;
}


float calculate_cost(cart_t *shopping_cart)
{
    size_t size_of_shopping_cart = ioopm_linked_list_size(shopping_cart->items);
    float total_cost = 0;
    
    if (cart_is_empty(shopping_cart) == false)
    {
        for (int i = 0; i < size_of_shopping_cart; i++)
        {
            elem_t item_elem = ioopm_linked_list_get(shopping_cart->items, i);
            item_c *item = item_elem.pointer;
            float price_item = item->price;
            int quantity = item->quantity;
            total_cost = total_cost + (price_item * quantity);
        }
        return total_cost;
    }

    return total_cost;
}


cart_t *cart_check(char *name, ioopm_list_t *shopping_carts)
{
    size_t size_of_shopping_carts = ioopm_linked_list_size(shopping_carts);
    if (size_of_shopping_carts == 0)
    {
        cart_t *shopping_cart = ioopm_create_cart(name);
        elem_t shopping_cart_elem = { .pointer = shopping_cart };
        ioopm_linked_list_prepend(shopping_carts, shopping_cart_elem);
        return shopping_cart;
    }
    else
    {
        elem_t shopping_cart_elem = ioopm_linked_list_get(shopping_carts, 0);
        cart_t *shopping_cart = shopping_cart_elem.pointer;
        char *cart_name = shopping_cart->name;
        
        for (int i = 0; i < size_of_shopping_carts; i++)
        {
            elem_t cart_elem = ioopm_linked_list_get(shopping_carts, i);
            cart_t *cart = cart_elem.pointer;
            if (strcmp(cart->name, name) == 0)
            {
                shopping_cart = cart;
                free(name);
                return shopping_cart;
            }
        }
        if (strcmp(cart_name, name) != 0)
        {
            cart_t *shopping_cart = ioopm_create_cart(name);
            elem_t shopping_cart_elem = { .pointer = shopping_cart };
            ioopm_linked_list_prepend(shopping_carts, shopping_cart_elem);
            return shopping_cart;
        }
        return shopping_cart;
    }
}


// --------------------- SHELF FUNCTIONS ---------------------------------------


static shelf_t *shelf_create(char *name, size_t quantity)
{
    shelf_t *result = calloc(1, sizeof(shelf_t));
    shelf_t new_shelf = { .name = name, .quantity = quantity};
    *result = new_shelf;
    return result;
}


void ioopm_replenish_stock(ioopm_hash_table_t *ht_names, ioopm_hash_table_t *ht_shelves, char *location, int new_quantity, elem_t chosen_key)
{
    elem_t location_elem = { .str = location};
    option_t is_location_in_hashtable = ioopm_hash_table_lookup(ht_shelves, location_elem);
    option_t item = ioopm_hash_table_lookup(ht_names, chosen_key);
    merch_t *merch = item.value.pointer;
    ioopm_list_t *list_of_locations = merch->locations;
    size_t size_of_list = ioopm_linked_list_size(list_of_locations);
    
    if (is_location_in_hashtable.success && strcmp(is_location_in_hashtable.value.str, chosen_key.str) == 0) 
    {
        for (int i = 0; i < size_of_list; i++)
        {
            elem_t value = ioopm_linked_list_get(list_of_locations, i);
            shelf_t *shelf = value.pointer;
            if (strcmp(shelf->name, location) == 0)
            {
                shelf->quantity += new_quantity;
            }
        }
    }
    else if (is_location_in_hashtable.success == false)
    {
        shelf_t *new_shelf = shelf_create(location, new_quantity);
        elem_t shelf = { .pointer = new_shelf };
        ioopm_linked_list_prepend(list_of_locations, shelf);
        elem_t location_elem = { .str = location };
        ioopm_hash_table_insert(ht_shelves, location_elem, chosen_key);
    }
    else
    {
        printf("The location already has stock of another type of merch.\n");
    }
}


void change_quantity_on_locations(int quantity, ioopm_list_t *locations, ioopm_hash_table_t *ht_shelves)
{
    size_t amount_of_locations = ioopm_linked_list_size(locations);
    for (int i = 0; i < amount_of_locations; i++)
    {
        elem_t location = ioopm_linked_list_get(locations, i);
        shelf_t *shelf = location.pointer;
        size_t quantity_on_shelf = shelf->quantity;
        int tmp = quantity_on_shelf - quantity;
        if (tmp < 0)
        {
            quantity -= quantity_on_shelf;
            ioopm_linked_list_remove(locations, i);
            elem_t name = { .str = shelf->name};
            ioopm_hash_table_remove(ht_shelves, name);
            free(shelf->name);
            free(shelf);
        }
        else
        {
            shelf->quantity -= quantity;
            break;
        }
    }
}


