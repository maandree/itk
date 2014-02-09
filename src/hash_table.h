/**
 * itk — The Impressive Toolkit
 * 
 * Copyright © 2013  Mattias Andrée (maandree@member.fsf.org)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __ITK_HASH_TABLE_H__
#define __ITK_HASH_TABLE_H__

#include "itktypes.h"


/**
 * Hash table entry
 */
typedef struct _itk_hash_entry
{
  /**
   * A key
   */
  void* key;
  
  /**
   * The value associated with the key
   */
  void* value;
  
  /**
   * The truncated hash value of the key
   */
  long hash;
  
  /**
   * The next entry in the bucket
   */
  struct _itk_hash_entry* next;
  
} itk_hash_entry;


/**
 * Value lookup table based on hash value, that do not support `NULL` keys nor `NULL` values
 */
typedef struct _itk_hash_table
{
  /**
   * The table's capacity, i.e. the number of buckets
   */
  long capacity;
  
  /**
   * Entry buckets
   */
  itk_hash_entry** buckets;
  
  /**
   * When, in the ratio of entries comparied to the capacity, to grow the table
   */
  float load_factor;
  
  /**
   * When, in the number of entries, to grow the table
   */
  long threshold;
  
  /**
   * The number of entries stored in the table
   */
  long size;
  
  /**
   * Check whether two values are equal
   * 
   * If this function pointer is `NULL`, the identity is used
   * 
   * @param   value_a  The first value
   * @param   value_b  The second value
   * @return           Whether the values are equals
   */
  bool_t (*value_comparator)(void* value_a, void* value_b);
  
  /**
   * Check whether two keys are equal
   * 
   * If this function pointer is `NULL`, the identity is used
   * 
   * @param   key_a  The first key
   * @param   key_b  The second key
   * @return         Whether the keys are equals
   */
  bool_t (*key_comparator)(void* key_a, void* key_b);
  
  /**
   * Calculate the hash of a key
   * 
   * If this function pointer is `NULL`, the identity hash is used
   * 
   * @param   key  The key
   * @return       The hash of the key
   */
  long (*hasher)(void* key);
  
} itk_hash_table;


#define __this__  itk_hash_table* this

/**
 * Constructor
 * 
 * @param  initial_capacity  The initial capacity of the table
 * @param  load_factor       The load factor of the table, i.e. when to grow the table
 */
itk_hash_table* itk_new_hash_table_fine_tuned(long initial_capacity, float load_factor);

/**
 * Constructor
 * 
 * @param  initial_capacity:long  The initial capacity of the table
 */
#define itk_new_hash_table_tuned(initial_capacity) itk_new_hash_table_fine_tuned(initial_capacity, 0.75)

/**
 * Constructor
 */
#define itk_new_hash_table() itk_new_hash_table_tuned(16)

/**
 * Destructor
 * 
 * @param  values  Whether to free all stored values
 * @param  keys    Whether to free all stored keys
 */
void itk_free_hash_table(__this__, bool_t values, bool_t keys);

/**
 * Check whether a value is stored in the table
 * 
 * @param   value  The value
 * @return         Whether the value is stored in the table
 */
bool_t itk_hash_table_contains_value(__this__, void* value);

/**
 * Check whether a key is used in the table
 * 
 * @param   key  The key
 * @return       Whether the key is used
 */
bool_t itk_hash_table_contains_key(__this__, void* key);

/**
 * Look up a value in the table
 * 
 * @param   key    The key associated with the value
 * @return         The value associated with the key, `NULL` i the key was not used
 */
void* itk_hash_table_get(__this__, void* key);

/**
 * Add an entry to the table
 * 
 * @param   key    The key of the entry to add
 * @param   value  The value of the entry to add
 * @return         The previous value associated with the key, `NULL` i the key was not used
 */
void* itk_hash_table_put(__this__, void* key, void* value);

/**
 * Remove an entry in the table
 * 
 * @param   key  The key of the entry to remove
 * @return       The previous value associated with the key, `NULL` i the key was not used
 */
void* itk_hash_table_remove(__this__, void* key);

/**
 * Remove all entries in the table
 */
void itk_hash_table_clear(__this__);

#undef __this__


#endif

