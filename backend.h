#pragma once

#include "common.h"
#include "hash_table.h"
#include "linked_list.h"

typedef struct merch merch_t;
typedef struct shelf shelf_t;
typedef struct item_in_cart item_c;
typedef struct cart cart_t;
typedef struct entry entry_t;
typedef struct index_counter index_counter_t;


/**
 * @file backend.h
 * @author   Jonathan Kjellén, Aljaz Kovac 
 * @date 23 Oct 2020
 * @brief Backend for a simple webstore, as defined by instructions found here: 
 *
 * @see http://wrigstad.com/ioopm/assignments/assignment2.html
 */


/// @brief adds a new merch to the warehouse
/// @param name name of merch
/// @param desc description of merch
/// @param price price of the merch
/// @param ht_names hashtable with names of merch as keys and merch as values
/// @return ht_names
ioopm_hash_table_t *ioopm_add_merch(char *name, char *desc, float price, ioopm_hash_table_t *ht_names);

/// @brief destroys a merch
/// @param merch the merch to be destroyed
void destroy_merch(merch_t *merch);
 
/// @brief destroys all merch in the hashtable
/// @param ht hash table operated upon
void destroy_all_merch(ioopm_hash_table_t *ht);

/// @brief removes a merch from hashtables
/// @param chosen_key the key of the merch to remove
/// @param ht_names hashtable with names of merch as keys and merch as values
/// @param ht_shelves hashtable with names of shelves as keys and names of merch as values
/// @param shopping_carts list of shopping carts
void ioopm_remove_merch(elem_t chosen_key, ioopm_hash_table_t *ht_names, ioopm_hash_table_t *ht_shelves, ioopm_list_t *shopping_carts);

/// @brief edits a merch in hashtables
/// @param struct elem chosen_key
/// @param new_name new name for merch 
/// @param new_desc new description for merch
/// @param new_price new price for merch
/// @param ht_names hashtable with names of merch as keys and merch as values
/// @param ht_shelves hashtable with names of shelves as keys and names of merch as values
/// @param shopping_carts list of shopping carts
void ioopm_edit_merch(elem_t chosen_key, char *new_name, char *new_desc, float new_price, ioopm_hash_table_t *ht_names, ioopm_hash_table_t *ht_shelves, ioopm_list_t *shopping_carts);

/// @brief counts total quantity of a merch on all locations
/// @param locations list of all existing locations
/// @return total quantity for chosen merch
size_t count_total_quantity_for_merch(ioopm_list_t *locations);
 
/// @brief creates a new item
/// @param name name of the item
/// @param price price of the item
/// @param quantity of the item
/// @return the new malloc allocated item which can be freed using free()
item_c *item_create(char *name, float price, int quantity);

/// @brief creates a new cart
/// @param name name of the cart
/// @return the new malloc allocated cart which can be freed using free()
cart_t *ioopm_create_cart(char *name);

/// @brief removes an item from all carts
/// @param shopping_carts a list with all the shopping carts
void remove_item_from_carts(ioopm_list_t *shopping_carts, char *name);


/// @brief removes an item from a cart
/// @param shopping_cart of items in the shopping cart
/// @param name of item to remove from cart
void remove_item_from_cart(cart_t *shopping_cart, char *name);


/// @brief clears a cart
/// @param cart cart to be cleared
void clear_cart(cart_t *cart);

/// @brief destroys a cart
/// @param cart cart to be destroyed
void destroy_cart(cart_t *cart);

/// @brief removes all shopping carts from the list
/// @param shopping_carts a list with all the shopping carts
void remove_all_carts(ioopm_list_t *shopping_carts);

/// @brief checks if a any shopping cart has the old_name and if so replaces it with new_name
/// @param shopping_carts a list with all the shopping carts
/// @param new_name name to replace old name with
/// @param new_price price to replace the old price
/// @param old_name name to search for in shopping carts
void check_shopping_carts(ioopm_list_t *shopping_carts, char* new_name, float new_price, char *old_name);

/// @brief checks if an item is in cart
/// @param name name of item
/// @param cart list of items
/// @return option_t consisting of a bool and an elem_t value
option_t is_in_cart(char *name, ioopm_list_t *cart);

/// @brief checks if cart is empty
/// @param shopping_cart list of items 
/// @return true if cart is empty else false
bool cart_is_empty(cart_t *shopping_cart);

/// @brief calculates the cost of the current cart
/// @param shopping_carts a list with all the shopping carts
/// @return the calculated price
float calculate_cost(cart_t *shopping_cart);

/// @brief checks if name is the name of a cart in the list
/// @param name name of a cart
/// @param shopping_carts a list with all the shopping carts
/// @return shopping cart
cart_t *cart_check(char *name, ioopm_list_t *shopping_carts);

/// @brief replenishes stock for chosen merch
/// @param ht_names hashtable with names of merch as keys and merch as values
/// @param ht_shelves hashtable with names of shelves as keys and names of merch as values
/// @param location name of location where stock should be replenished
/// @param new_quantity amount by which to replenish stock
/// @param chosen_key name of merch to replenish stock for
void ioopm_replenish_stock(ioopm_hash_table_t *ht_names, ioopm_hash_table_t *ht_shelves, char *location, int new_quantity, elem_t chosen_key);

/// @brief changes the quantity on a chosen location
/// @param quantity amount to change the current quantity with
/// @param locations list of all locations for a merch
void change_quantity_on_locations(int quantity, ioopm_list_t *locations, ioopm_hash_table_t *ht_shelves);