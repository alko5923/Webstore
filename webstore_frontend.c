# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <ctype.h>
# include <string.h>
# include "utils.h"
# include <time.h>
# include "backend.h"
# include "common.h"
# include "hash_table.h"
# include "linked_list.h"


//------------------------------- MACROS ---------------------------------------

#define Success(v)      (option_t) { .success = true, .value = v };
#define Failure()       (option_t) { .success = false };
#define Successful(o)   (o.success == true)
#define Unsuccessful(o) (o.success == false)


// -------------------------- FUNCTION DEFINITIONS -----------------------------


void add_new_item_to_cart(elem_t chosen_merch, merch_t *merch, cart_t *shopping_cart, ioopm_hash_table_t *ht_names, ioopm_hash_table_t *ht_shelves);
void add_more_quantity_existing_item(elem_t chosen_merch, merch_t *merch, item_c *item, ioopm_hash_table_t *ht_names, ioopm_hash_table_t *ht_shelves);
option_t ask_if_item_in_warehouse(ioopm_hash_table_t *ht_names);
void ioopm_add_to_cart(cart_t *shopping_cart, ioopm_hash_table_t *ht_names, ioopm_hash_table_t *ht_shelves, ioopm_list_t *shopping_carts);
void ioopm_checkout(cart_t *shopping_cart, ioopm_hash_table_t *ht_names, ioopm_hash_table_t *ht_shelves);
void ioopm_remove_from_cart(cart_t *shopping_cart, ioopm_list_t *shopping_carts);


bool ioopm_print_stock(elem_t chosen_key, ioopm_hash_table_t *ht_names, ioopm_hash_table_t *ht_shelves);
void print_cart(cart_t *shopping_cart);
index_counter_t print_20_items(ioopm_list_t *list_of_keys, size_t size_of_list, index_counter_t index_counter);
void print_chosen_item(elem_t key, ioopm_hash_table_t *ht_names);
void print_names(ioopm_list_t *shopping_carts);
void ioopm_show_stock(elem_t chosen_key, ioopm_hash_table_t *ht_names, ioopm_hash_table_t *ht_shelves, ioopm_list_t *shopping_carts);


elem_t ioopm_list_merch (ioopm_hash_table_t *ht_names, ioopm_hash_table_t *ht_shelves, char employee_customer, ioopm_list_t *shopping_carts);
bool ioopm_manipulate_merch(elem_t chosen_key, ioopm_hash_table_t *ht_names, ioopm_hash_table_t *ht_shelves, ioopm_list_t *shopping_carts);


char ioopm_main_menu(void);
void ioopm_customer(ioopm_hash_table_t *ht_names, ioopm_hash_table_t *ht_shelves, ioopm_list_t *shopping_carts);
void ioopm_employee(ioopm_hash_table_t *ht_names, ioopm_hash_table_t *ht_shelves, ioopm_list_t *shopping_carts);


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

struct index_counter
{
    int index_in_list;
    int counter;
};

// ----------------------- CART FUNCTIONS --------------------------------------


void add_new_item_to_cart(elem_t chosen_merch, merch_t *merch, cart_t *shopping_cart, ioopm_hash_table_t *ht_names, ioopm_hash_table_t *ht_shelves)
{
    printf("This is the current stock of the item: \n");
    bool is_not_empty = ioopm_print_stock(chosen_merch, ht_names, ht_shelves);
    if (is_not_empty)
    {
        bool not_done = true;
        int desired_quantity = 0;
        while (not_done)
        {
            desired_quantity = ask_question_int("How much would you like to add?");
            if (desired_quantity <= count_total_quantity_for_merch(merch->locations))
            {
                not_done = false;
            }
        }
        item_c *item1 = item_create(merch->name, merch->price, desired_quantity);
        elem_t item_elem = { .pointer = item1};
        ioopm_linked_list_prepend(shopping_cart->items, item_elem);//add item to cart with desired quantity
    }
    
}


void add_more_quantity_existing_item(elem_t chosen_merch, merch_t *merch, item_c *item, ioopm_hash_table_t *ht_names, ioopm_hash_table_t *ht_shelves)
{
    printf("This is how much you have in the shopping cart\n");
    //item_c *item1 = is_item_in_cart.value.pointer;
    
    printf("%d\n", item->quantity);
    
    printf("This is the current stock of the item: \n");
    bool is_not_empty = ioopm_print_stock(chosen_merch, ht_names, ht_shelves);
    
    if (is_not_empty)
    {
        bool not_done = true;
        int desired_quantity;
        while (not_done)
        {
            desired_quantity = ask_question_int("How much more would you like to add?");
            if (desired_quantity <= (count_total_quantity_for_merch(merch->locations) - item->quantity))
            {
                not_done = false;
            }
            else
            {
                printf("There is not enough stock in the warehouse, please choose a lower amount \n");
            }
            
        }
        item->quantity = item->quantity + desired_quantity;
    }
    
}


option_t ask_if_item_in_warehouse(ioopm_hash_table_t *ht_names)
{
    bool second_loop = true;
    while (second_loop)
    {
        char *ask_for_item_name = ask_question_string("Write the name of the item to add to cart, please: ");
        elem_t item_name = { .str = ask_for_item_name }; // make name_item into an elem_t
        bool item_exists = ioopm_hash_table_has_key(ht_names, item_name);
        if (item_exists)
        {
            option_t name = { .success = true, .value = item_name };
            return name;
        }
        else
        {
            char choice = ask_question_menu("QqIi", "There is no such item in the warehouse please write an existing [I]tem or press [Q] to go back to the main menu.");
            if (choice == 'Q')
            {
                option_t quit = { .success = false };
                return quit;
            }
        }
    }
    option_t quit = { .success = false };
    return quit;
}


void ioopm_add_to_cart(cart_t *shopping_cart, ioopm_hash_table_t *ht_names, ioopm_hash_table_t *ht_shelves, ioopm_list_t *shopping_carts)
{
    bool first_loop = true;
    
    while (first_loop)
    {
        char first_choice = ask_question_menu("NnLlQq","Would you like to write the [N]ame of the item or [L]ist all items to choose from or [Q]uit?");
        if (first_choice == 'N')
        {
            
            option_t chosen_item_to_add = ask_if_item_in_warehouse(ht_names);
            if (Successful(chosen_item_to_add))
            {
                option_t value_for_key = ioopm_hash_table_lookup(ht_names, chosen_item_to_add.value);
                merch_t *merch = value_for_key.value.pointer;
                
                option_t is_item_in_cart = is_in_cart(merch->name, shopping_cart->items);
                elem_t item_elem = is_item_in_cart.value;
                item_c *item = item_elem.pointer;
                
                if (Successful(is_item_in_cart)) // if item is in cart already
                {
                   add_more_quantity_existing_item(chosen_item_to_add.value, merch, item, ht_names, ht_shelves);
                   first_loop = false;
                }
                else // if item is not in cart already
                {
                    add_new_item_to_cart(chosen_item_to_add.value, merch, shopping_cart, ht_names, ht_shelves);
                    first_loop = false;
                }
            }
        }   
        
        else if (first_choice == 'L')
        {
            elem_t chosen_merch = ioopm_list_merch (ht_names, ht_shelves, 'C', shopping_carts);
            option_t value_for_key = ioopm_hash_table_lookup(ht_names, chosen_merch);
            merch_t *merch = value_for_key.value.pointer;
            char *merch_name = merch->name;
            
            option_t is_item_in_cart = is_in_cart(merch_name, shopping_cart->items);
            elem_t item_elem = is_item_in_cart.value;
            item_c *item = item_elem.pointer;
            
            if (Successful(is_item_in_cart))
            {
                add_more_quantity_existing_item(chosen_merch, merch, item, ht_names, ht_shelves);
                first_loop = false;
            }
            else
            {
                add_new_item_to_cart(chosen_merch, merch, shopping_cart, ht_names, ht_shelves);
                first_loop = false;
            }
        }
        else
        {
            first_loop = false;
        }
    }
}


void ioopm_checkout(cart_t *shopping_cart, ioopm_hash_table_t *ht_names, ioopm_hash_table_t *ht_shelves)
{
    if (cart_is_empty(shopping_cart))
    {
        printf("The cart is empty \n");
    }
    else
    {
        print_cart(shopping_cart);
        ioopm_list_t *cart = shopping_cart->items;
        size_t size_of_cart = ioopm_linked_list_size(cart);
        
        for (int i = 0; i < size_of_cart; i++)
        {
            elem_t item_elem = ioopm_linked_list_get(cart, i);
            item_c *item = item_elem.pointer;
            char *name = item->name;
            int quantity = item->quantity;
            
            elem_t name_elem = { .str = name };
            
            option_t name_lookup = ioopm_hash_table_lookup(ht_names, name_elem);
            merch_t *merch = name_lookup.value.pointer;
            ioopm_list_t *locations = merch->locations;
            size_t amount_of_merch_left = count_total_quantity_for_merch(locations);
            if (amount_of_merch_left < quantity)
            {
                printf("There is not enough stock for this item.\n");
                printf("Stock for item: \n");
                ioopm_print_stock(name_elem, ht_names, ht_shelves);
                
                item->quantity = amount_of_merch_left;
                printf("Updating cart..");
                print_cart(shopping_cart);
            }
            change_quantity_on_locations(quantity, locations, ht_shelves);
            
        }
        float cost_of_cart = calculate_cost(shopping_cart);
        clear_cart(shopping_cart);
        ioopm_linked_list_clear(cart);
        printf("Total cost of purchase: %.2f \n", cost_of_cart);
    }
    
}


void ioopm_remove_from_cart(cart_t *shopping_cart, ioopm_list_t *shopping_carts)
{
    size_t size_of_cart = ioopm_linked_list_size(shopping_cart->items); // check if cart is empty
    if (size_of_cart == 0)
    {
        printf("The cart is empty.\n"); // if empty, then print that cart is empty, return to main menu
    }
    else
    {
        print_cart(shopping_cart); // if cart not empty, then print contents of cart
        bool first_loop = true;
        int item_to_remove;
        while (first_loop)
        {
            item_to_remove = ask_question_int("Please choose an item to remove from cart: "); // choose item to remove from list(cart)
            if (item_to_remove > 0 && item_to_remove <= size_of_cart)
            {
                first_loop = false;
            }
        }
        elem_t item_remove = ioopm_linked_list_get(shopping_cart->items, item_to_remove - 1);
        item_c *item = item_remove.pointer;
        int amount_to_remove;
        bool second_loop = true;
        while (second_loop)
        {
            amount_to_remove = ask_question_int("Please choose amount of chosen item to remove from cart: "); // choose amount of item to remove from list(cart)
            if (amount_to_remove > 0 && amount_to_remove <= item->quantity)
            {
                second_loop = false;
            }
        }
        if (amount_to_remove < item->quantity)
        {
            item->quantity -= amount_to_remove;
        }
        else
        {
            remove_item_from_cart(shopping_cart, item->name);
        }
    }
}

// ----------------------------- PRINT FUNCTIONS -------------------------------

bool ioopm_print_stock(elem_t chosen_key, ioopm_hash_table_t *ht_names, ioopm_hash_table_t *ht_shelves)
{
    option_t item = ioopm_hash_table_lookup(ht_names, chosen_key);
    merch_t *merch = item.value.pointer;
    ioopm_list_t *list_of_locations = merch->locations;
    size_t size_of_list = ioopm_linked_list_size(list_of_locations);
    
    if (size_of_list == 0)
    {
        printf("No stock for this merch.\n");
        return false;
    }
    else
    {
        for (int i = 0; i < size_of_list; i++)
        {
            elem_t value = ioopm_linked_list_get(list_of_locations, i);
            shelf_t *shelf = value.pointer;
            if (shelf->quantity == 0)
            {
                printf("No stock for this merch.\n");
            return false;
            }
            printf("[%s] = %ld \n", shelf->name, shelf->quantity);
        }
        return true;
    }
    
}


void print_cart(cart_t *shopping_cart)
{
    printf("\nCart:\n");
    ioopm_list_t *cart = shopping_cart->items;
    size_t size_of_cart = ioopm_linked_list_size(cart);
    for (int i = 0; i < size_of_cart; i++)
    {
        elem_t item_elem = ioopm_linked_list_get(cart, i);
        item_c *item = item_elem.pointer;
        printf("[%d] Name: %s    Quantity: [%d]\n", i+1, item->name, item->quantity);
    }
}


index_counter_t print_20_items(ioopm_list_t *list_of_keys, size_t size_of_list, index_counter_t index_counter)
{
    printf("LIST OF ITEMS: \n");
    
    
    for (index_counter.counter = 0; index_counter.counter < 20 && index_counter.index_in_list < size_of_list; index_counter.counter++)
        {
            elem_t key = ioopm_linked_list_get(list_of_keys, index_counter.index_in_list);
            printf("[%d]: %s \n", index_counter.counter+1, key.str);
            index_counter.index_in_list ++;
        }
    return index_counter;
}


void print_chosen_item(elem_t key, ioopm_hash_table_t *ht_names)
{
    printf("You have chosen the following item: \n");
    option_t item = ioopm_hash_table_lookup(ht_names, key);
    if (Successful(item))
    {
        elem_t item_value = item.value;
        merch_t *item_value_2 = (merch_t*)item_value.pointer;
        printf("Name: %s \nDesc: %s \nPrice: %f\n", item_value_2->name, 
                            item_value_2->desc, item_value_2->price);
    }
}


void print_names(ioopm_list_t *shopping_carts)
{
    size_t size_of_shopping_carts = ioopm_linked_list_size(shopping_carts);
    for (int i = 0; i < size_of_shopping_carts; i++)
    {
        elem_t shopping_cart_elem = ioopm_linked_list_get(shopping_carts, i);
        cart_t *shopping_cart = shopping_cart_elem.pointer;
        char *cart_name = shopping_cart->name;
        printf("[%d]: %s\n", i+1, cart_name);
    }
}


void ioopm_show_stock(elem_t chosen_key, ioopm_hash_table_t *ht_names, ioopm_hash_table_t *ht_shelves, ioopm_list_t *shopping_carts)
{
    printf("This is the stock for this merch: \n");
    ioopm_print_stock(chosen_key, ht_names, ht_shelves); 
    printf("Would you like to replenish stock for this merch? \n");
    char answer = ask_question_menu("YyNn", "[Y]es \n[N]o\n");
    if (answer == 'Y')
    {
        bool not_done = true;
        while (not_done)
        {
            char *location = ask_question_shelf("Where would you like to replenish stock? ");
            elem_t location_elem = { .str = location };
            option_t is_location_in_hashtable = ioopm_hash_table_lookup(ht_shelves, location_elem);
            if ( (is_location_in_hashtable.success && strcmp(is_location_in_hashtable.value.str, chosen_key.str) == 0) || is_location_in_hashtable.success == false)
            {
                int quantity = ask_question_int("By how much? ");
                ioopm_replenish_stock(ht_names, ht_shelves, location, quantity, chosen_key); 
                not_done = false;
            }
            else 
            {
                printf("The location already has stock of another type of merch.\n");
                not_done = true;
            }
        }
    }
    else
    {
        ioopm_manipulate_merch(chosen_key, ht_names, ht_shelves, shopping_carts); 
    }

}

// ------------------------------ MERCH FUNCTIONS ------------------------------


elem_t ioopm_list_merch (ioopm_hash_table_t *ht_names, ioopm_hash_table_t *ht_shelves, char employee_customer, ioopm_list_t *shopping_carts)
{
    
    bool not_done = true;
    index_counter_t index_counter = { .index_in_list = 0, .counter = 0 };
    while (not_done)
    {
        ioopm_list_t *list_of_keys = ioopm_hash_table_keys(ht_names);
        size_t size_of_list = ioopm_linked_list_size(list_of_keys);
        index_counter = print_20_items(list_of_keys, size_of_list, index_counter);
        if (size_of_list == 0)
        {
            printf("NO ITEMS IN WAREHOUSE\n");
            not_done = false;
        }
        else
        {
            printf("\nWould you like to choose an item? ");
            char answer = ask_question_menu("yYnN", "[Y]es \n[N]o \n");
            if (answer == 'Y')
            {
                if (index_counter.index_in_list > 20)
                {
                    bool notIndex = true;
                    int tmp;
                    int start;
                    while (notIndex)
                    {
                        int chosen_item = ask_question_int("Which item do you want to choose [1-20]: ");
                        tmp = (index_counter.index_in_list - 19);
                        start = index_counter.index_in_list - tmp;
                        tmp = start + chosen_item;
                        if (chosen_item > 0 && chosen_item <= index_counter.counter)
                        {
                            notIndex = false;
                        }
                    }
                    elem_t chosen_key = ioopm_linked_list_get(list_of_keys, tmp);
                    print_chosen_item(chosen_key, ht_names);
                    if (employee_customer == 'E')
                    {
                        not_done = ioopm_manipulate_merch(chosen_key, ht_names, ht_shelves, shopping_carts);
                        index_counter.index_in_list = start + 1;
                    }
                    else
                    {
                        ioopm_linked_list_destroy(list_of_keys);
                        return chosen_key;
                    }
                    
                }
                else
                {
                    bool notIndex = true;
                    int tmp;
                    while (notIndex)
                    {
                        int chosen_item = ask_question_int("Which item do you want to choose [1-20]: ");
                        tmp = chosen_item;
                        if (chosen_item > 0 && chosen_item <= index_counter.counter)
                        {
                            notIndex = false;
                        }
                    }
                    elem_t chosen_key = ioopm_linked_list_get(list_of_keys, tmp - 1);
                    print_chosen_item(chosen_key, ht_names);
                    if (employee_customer == 'E')
                    {
                        not_done = ioopm_manipulate_merch(chosen_key, ht_names, ht_shelves, shopping_carts);
                        index_counter.index_in_list = 0;
                    }
                    else
                    {
                        ioopm_linked_list_destroy(list_of_keys);
                        return chosen_key;
                    }
                }
            }
            else
            {
                if (index_counter.index_in_list == size_of_list)
                {
                    printf("All merch has been printed.\n");
                    not_done = false;
                }
                else
                {
                    printf("Would you like to continue printing items or Quit?: ");
                    char answer = ask_question_menu("CcQq", "[C]ontinue \n[Q]uit \n");
                    if (answer == 'Q')
                    {
                        not_done = false;
                    }
                }
                
            }
        }
        ioopm_linked_list_destroy(list_of_keys);
    }
    elem_t return_bool = { .b_ool = false };
    return return_bool;
}


bool ioopm_manipulate_merch(elem_t chosen_key, ioopm_hash_table_t *ht_names, ioopm_hash_table_t *ht_shelves, ioopm_list_t *shopping_carts)
{
    printf("What would you like to do with this merch?\n");
    char answer = ask_question_menu("EeRrSsPp", 
    "[E]dit merch \n[R]emove merch \n[S]how stock \n[P]revious menu \n");
    if (answer == 'E')
    {
        printf("You chose edit merch\n");
        print_chosen_item(chosen_key, ht_names);
        char *new_name = ask_question_string("New name: ");
        char *new_desc = ask_question_string("New desc: ");
        float new_price = ask_question_float("New price: ");
        ioopm_edit_merch(chosen_key, new_name, new_desc, new_price, ht_names, ht_shelves, shopping_carts);
        return true;
        
    }
    else if (answer == 'R')
    {
        printf("You chose remove merch\n");
        ioopm_remove_merch(chosen_key, ht_names, ht_shelves, shopping_carts); 
        return true;
        
    }
    else if (answer == 'S')
    {
        printf("You chose show stock\n");
        ioopm_show_stock(chosen_key, ht_names, ht_shelves, shopping_carts); 
        return true;
    }
    else 
    {
        return false;
    }
}


//--------------------------- MENU FUNCTIONS -----------------------------------


char ioopm_main_menu(void)
{
 printf("\nWelcome to our webstore! \nWhat would you like to do today? \n");
 char answer = ask_question_menu(
            "CcEeQq", "[C]ustomer \n[E]mployee \n[Q]uit \n");
 
 return answer;
}


void ioopm_customer(ioopm_hash_table_t *ht_names, ioopm_hash_table_t *ht_shelves, ioopm_list_t *shopping_carts)
{
    print_names(shopping_carts);
    char *name = ask_question_string("What is your name? ");
    cart_t *shopping_cart = cart_check(name, shopping_carts);
    
    
    bool done = false;
    while (done != true)
    {
        char answer = ask_question_menu(
            "AaRrCcLlMm", "[A]dd to cart \n [R]emove from cart \n"
            "[C]heckout \n [L]ist merchandise \n [M]ain menu\n");
    
        if (answer == 'A')
        {
            ioopm_add_to_cart(shopping_cart, ht_names, ht_shelves, shopping_carts); 
            
        }
        else if (answer == 'R')
        {
            ioopm_remove_from_cart(shopping_cart, shopping_carts); 
        }
        else if (answer == 'C')
        {
            ioopm_checkout(shopping_cart, ht_names, ht_shelves); 
            done = true;
        }
        else if (answer == 'L')
        {
            ioopm_list_merch(ht_names, ht_shelves, 'C', shopping_carts); 
        }
        else 
        {
            done = true;
        }
    }
}
    

void ioopm_employee(ioopm_hash_table_t *ht_names, ioopm_hash_table_t *ht_shelves, ioopm_list_t *shopping_carts)
{
    bool not_done = true;
    while (not_done)
    {
        char answer = ask_question_menu(
            "AaLlMm", "[A]dd merch \n"
            "[L]ist merch \n[M]ain menu \n");
        if (answer == 'A')
        {
            printf("You chose add merch\n");
            printf("What merch would you like to add? \n");
            char *name = ask_question_string("Name: ");
            char *desc = ask_question_string("Desc: ");
            float price = ask_question_float("Price: ");
            ht_names = ioopm_add_merch(name, desc, price, ht_names);
        }
        else if (answer == 'L')
        {
            printf("You chose list merch\n");
            
            ioopm_list_merch(ht_names, ht_shelves, 'E', shopping_carts); 
        }
        else 
        {
            not_done = false;
        }    
    }
}


int main(void)
{
    ioopm_hash_table_t *ht_names = ioopm_hash_table_create(string_knr_hash, str_key_eq, int_key_eq, key_equiv);
    ioopm_hash_table_t *ht_shelves = ioopm_hash_table_create(string_knr_hash, str_key_eq, int_key_eq, key_equiv);
    ioopm_list_t *shopping_carts = ioopm_linked_list_create(ioopm_linked_list_compare);
    bool program_quit = false;
    while (program_quit != true)
    {
        char main_menu_choice = ioopm_main_menu();
        if (main_menu_choice == 'E')
        {
            printf("You chose employee\n");
            ioopm_employee(ht_names, ht_shelves, shopping_carts);
        }
        else if (main_menu_choice == 'C')
        {
            printf("You chose customer\n");
            if (ioopm_hash_table_is_empty(ht_names))
            {
                printf("There is nothing in the warehouse, please choose employee first and do some work!\n");
            }
            else
            {
                ioopm_customer(ht_names, ht_shelves, shopping_carts);
            }
            
        }
        else 
        {
            remove_all_carts(shopping_carts);
            destroy_all_merch(ht_names);
            ioopm_hash_table_destroy(ht_names);
            ioopm_hash_table_destroy(ht_shelves);
            program_quit = true;
        }
    }
}

