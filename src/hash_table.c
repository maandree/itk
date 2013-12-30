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
#include "hash_table.h"
#include "itkmacros.h"

#include <stdlib.h>


#define __this__  itk_hash_table* this


/**
 * Calculate the hash of a key and constrain it to the buckets
 * 
 * @param   key  The key to hash
 * @return       A non-negative value less the the table's capacity
 */
static inline long hash(__this__, void* key)
{
  long rc = this->hasher ? this->hasher(key) : (long)key;
  rc %= this->capacity;
  return rc < 0 ? -rc : rc;
}


/**
 * Grow the table
 */
static void rehash(__this__)
{
  itk_hash_entry** old_buckets = this->buckets;
  long old_capacity = this->capacity;
  long i = old_capacity, index;
  itk_hash_entry* bucket;
  itk_hash_entry* destination;
  itk_hash_entry* next;
  
  this->capacity = old_capacity * 2 + 1;
  this->threshold = (long)(this->capacity * this->load_factor);
  this->buckets = calloc(this->capacity, sizeof(itk_hash_entry*));
  
  while (i)
    {
      bucket = *(this->buckets + --i);
      while (bucket)
	{
	  index = hash(this, bucket->key);
	  if ((destination = *(this->buckets + index)))
	    {
	      next = destination->next;
	      while (next)
		{
		  destination = next;
		  next = destination->next;
		}
	      destination->next = bucket;
	    }
	  else
	    *(this->buckets + index) = bucket;
	  
	  next = bucket->next;
	  bucket->next = NULL;
	  bucket = next;
	}
    }
  
  free(old_buckets);
}


/**
 * Constructor
 * 
 * @param  initial_capacity  The initial capacity of the table
 * @param  load_factor       The load factor of the table, i.e. when to grow the table
 */
itk_hash_table* itk_new_hash_table_fine_tuned(long initial_capacity, float load_factor)
{
  itk_hash_table* this = calloc(1, sizeof(itk_hash_table));
  this->capacity = initial_capacity ? initial_capacity : 1;
  this->buckets = calloc(this->capacity, sizeof(itk_hash_entry*));
  this->load_factor = load_factor;
  this->threshold = (long)(this->capacity * load_factor);
  return this;
}


/**
 * Destructor
 * 
 * @param  values  Whether to free all stored values
 * @param  keys    Whether to free all stored keys
 */
void itk_free_hash_table(__this__, bool_t values, bool_t keys)
{
  long i = this->capacity;
  itk_hash_entry* bucket;
  itk_hash_entry* last;
  
  while (i)
    {
      bucket = *(this->buckets + --i);
      while (bucket)
	{
	  if (values)
	    free(bucket->value);
	  if (keys)
	    free(bucket->key);
	  bucket = (last = bucket)->next;
	  free(last);
	}
    }
  
  free(this->buckets);
  free(this);
}


/**
 * Check whether a value is stored in the table
 * 
 * @param   value  The value
 * @return         Whether the value is stored in the table
 */
bool_t itk_hash_table_contains_value(__this__, void* value)
{
  long i = this->capacity;
  itk_hash_entry* bucket;
  
  while (i)
    {
      bucket = *(this->buckets + --i);
      while (bucket)
	{
	  if (bucket->value == value)
	    return true;
	  if (this->value_comparator && this->value_comparator(bucket->value, value))
	    return true;
	  bucket = bucket->next;
	}
    }
  
  return false;
}


/**
 * Check whether a key is used in the table
 * 
 * @param   key  The key
 * @return       Whether the key is used
 */
bool_t itk_hash_table_contains_key(__this__, void* key)
{
  long index = hash(this, key);
  itk_hash_entry* bucket = *(this->buckets + index);
  
  while (bucket)
    {
      if (bucket->key == key)
	return true;
      if (this->key_comparator && this->key_comparator(bucket->key, key))
	return true;
      bucket = bucket->next;
    }
  
  return false;
}


/**
 * Look up a value in the table
 * 
 * @param   key    The key associated with the value
 * @return         The value associated with the key, `NULL` i the key was not used
 */
void* itk_hash_table_get(__this__, void* key)
{
  long index = hash(this, key);
  itk_hash_entry* bucket = *(this->buckets + index);
  
  while (bucket)
    {
      if ((bucket->key == key) || (this->key_comparator && this->key_comparator(bucket->key, key)))
	return bucket->value;
      bucket = bucket->next;
    }
  
  return NULL;
}


/**
 * Add an entry to the table
 * 
 * @param   key    The key of the entry to add
 * @param   value  The value of the entry to add
 * @return         The previous value associated with the key, `NULL` i the key was not used
 */
void* itk_hash_table_put(__this__, void* key, void* value)
{
  long index = hash(this, key);
  itk_hash_entry* bucket = *(this->buckets + index);
  void* rc;
  
  while (bucket)
    if ((bucket->key == key) || (this->key_comparator && this->key_comparator(bucket->key, key)))
      {
	rc = bucket->value;
	bucket->value = value;
	return rc;
      }
    else
      bucket = bucket->next;
  
  if (++(this->size) > this->threshold)
    {
      rehash(this);
      index = hash(this, key);
    }
  
  bucket = malloc(sizeof(itk_hash_entry));
  bucket->next = *(this->buckets + index);
  *(this->buckets + index) = bucket;
  
  return NULL;
}


/**
 * Remove an entry in the table
 * 
 * @param   key  The key of the entry to remove
 * @return       The previous value associated with the key, `NULL` i the key was not used
 */
void* itk_hash_table_remove(__this__, void* key)
{
  long index = hash(this, key);
  itk_hash_entry* bucket = *(this->buckets + index);
  itk_hash_entry* last = NULL;
  void* rc;
  
  while (bucket)
    {
      if ((bucket->key == key) || (this->key_comparator && this->key_comparator(bucket->key, key)))
	{
	  if (last == NULL)
	    *(this->buckets + index) = bucket->next;
	  else
	    last->next = bucket->next;
	  this->size--;
	  rc = bucket->value;
	  free(bucket);
	  return rc;
	}
      last = bucket;
      bucket = bucket->next;
    }
  
  return NULL;
}


/**
 * Remove all entries in the table
 */
void itk_hash_table_clear(__this__)
{
  itk_hash_entry** buf;
  itk_hash_entry* bucket;
  long i, ptr;
  
  if (this->size)
    {
      buf = alloca((this->size + 1) * sizeof(itk_hash_entry*));
      i = this->capacity;
      while (i)
	{
	  bucket = *(this->buckets + --i);
	  ptr = 0;
	  *(buf + ptr++) = bucket;
	  while (bucket)
	    {
	      bucket = bucket->next;
	      *(buf + ptr++) = bucket;
	    }
	  while (ptr)
	    free(*(buf + --ptr));
	  *(this->buckets + i) = NULL;
	}
      this->size = 0;
    }
}

