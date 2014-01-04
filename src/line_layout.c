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
#include "line_layout.h"
#include "hash_table.h"
#include "itkmacros.h"

#include <stdlib.h>


#define __this__  struct _itk_layout_manager* this

#define CONTAINER_(layout)  *((void**)(layout->data) + 0)
#define PREPARED_(layout)   *((void**)(layout->data) + 1)
#define GAP_(layout)        *((void**)(layout->data) + 2)
#define BUF_(layout)        *((void**)(layout->data) + 3)

#define CONTAINER(layout)   ((itk_component*)(CONTAINER_(layout)))
#define PREPARED(layout)    ((itk_hash_table*)(PREPARED_(layout)))
#define GAP(layout)         *((dimension_t*)(GAP_(layout)))
#define BUF(layout)         ((rectangle_t*)(BUF_(layout)))


/**
 * Prepare the layout manager for locating of multiple components, probably all of them
 */
static void prepare_h(__this__)
{
  itk_hash_table* prepared = PREPARED_(this) = itk_new_hash_table();
  itk_component* container = CONTAINER(this);
  long i, n;
  if ((n = container->children_count))
    {
      itk_component** children = container->children;
      rectangle_t* buf = BUF_(this) = malloc(n * sizeof(rectangle_t));
      dimension_t gap = GAP(this);
      dimension_t height = container->size.height;
      dimension_t width = container->size.width - gap * (n - 1);
      position_t x = 0;
      for (i = 0; i < n; i++)
	{
	  if (((buf + i)->width = (*(children + i))->minimum_size.width) < 0)
	    (buf + i)->width = 0;
	  width -= (buf + i)->width;
	}
      while (width > 0)
	{
	  itk_component** child;
	  itk_component** end = children + n;
	  long can_grow = 0;
	  for (child = children; child != end; child++)
	    if ((buf + i)->width < (*child)->preferred_size.width)
	      can_grow++;
	  if (can_grow)
	    {
	      dimension_t increment = width / can_grow, max, now;
	      if (increment == 0)
		increment = 1;
	      for (child = children; (child != end) && width; child++)
		if ((now = (buf + i)->width) < (max = (*child)->preferred_size.width))
		  {
		    dimension_t soon = now + increment;
		    (buf + i)->width = soon < max ? soon : max;
		    width -= (buf + i)->width - now;
		  }
	    }
	}
      for (i = 0; i < n; i++)
	{
	  itk_hash_table_put(prepared, *(children + i), buf + i);
	  (buf + i)->defined = true;
	  (buf + i)->height = height;
	  (buf + i)->y = 0;
	  (buf + i)->x = x;
	  x += gap + (buf + i)->width;
	}
    }
}


/**
 * Prepare the layout manager for locating of multiple components, probably all of them
 */
static void prepare_v(__this__)
{
  itk_hash_table* prepared = PREPARED_(this) = itk_new_hash_table();
  itk_component* container = CONTAINER(this);
  long i, n;
  if ((n = container->children_count))
    {
      itk_component** children = container->children;
      rectangle_t* buf = BUF_(this) = malloc(n * sizeof(rectangle_t));
      dimension_t gap = GAP(this);
      dimension_t height = container->size.height - gap * (n - 1);
      dimension_t width = container->size.width;
      position_t y = 0;
      for (i = 0; i < n; i++)
	{
	  if (((buf + i)->height = (*(children + i))->minimum_size.height) < 0)
	    (buf + i)->height = 0;
	  height -= (buf + i)->height;
	}
      while (height > 0)
	{
	  itk_component** child;
	  itk_component** end = children + n;
	  long can_grow = 0;
	  for (child = children; child != end; child++)
	    if ((buf + i)->height < (*child)->preferred_size.height)
	      can_grow++;
	  if (can_grow)
	    {
	      dimension_t increment = height / can_grow, max, now;
	      if (increment == 0)
		increment = 1;
	      for (child = children; (child != end) && height; child++)
		if ((now = (buf + i)->height) < (max = (*child)->preferred_size.height))
		  {
		    dimension_t soon = now + increment;
		    (buf + i)->height = soon < max ? soon : max;
		    height -= (buf + i)->height - now;
		  }
	    }
	}
      for (i = 0; i < n; i++)
	{
	  itk_hash_table_put(prepared, *(children + i), buf + i);
	  (buf + i)->defined = true;
	  (buf + i)->width = width;
	  (buf + i)->y = y;
	  (buf + i)->x = 0;
	  y += gap + (buf + i)->height;
	}
    }
}


/**
 * Prepare the layout manager for locating of multiple components, probably all of them
 */
static void prepare_hr(__this__)
{
  itk_component* container = CONTAINER(this);
  long i, n;
  if ((n = container->children_count))
    {
      rectangle_t* buf = BUF(this);
      dimension_t width = container->size.width;
      for (i = 0; i < n; i++)
	(buf + i)->x = width - (buf + i)->x - (buf + i)->width;
    }
}


/**
 * Prepare the layout manager for locating of multiple components, probably all of them
 */
static void prepare_vr(__this__)
{
  itk_component* container = CONTAINER(this);
  long i, n;
  if ((n = container->children_count))
    {
      rectangle_t* buf = BUF(this);
      dimension_t height = container->size.height;
      for (i = 0; i < n; i++)
	(buf + i)->y = height - (buf + i)->y - (buf + i)->height;
    }
}


/**
 * End of `prepare` requirement
 */
static void done(__this__)
{
  itk_hash_table* hash_table = PREPARED(this);
  if (hash_table)
    itk_free_hash_table(hash_table, false, false);
  PREPARED_(this) = NULL;
  if (BUF(this))
    free(BUF_(this));
  BUF_(this) = NULL;
}


/**
 * Locates the positions of the corners of a component
 * 
 * @param   child  The child, to the component using the layout manager, of interest
 * @return         The rectangle the child is confound in
 */
static rectangle_t locate(__this__, itk_component* child)
{
  rectangle_t* r;
  bool_t p = PREPARED(this) == NULL;
  
  if (p)
    this->prepare(this);
  
  if ((r = itk_hash_table_get(PREPARED(this), child)))
    child->size = new_size2(r->width, r->height);
  
  if (p)
    this->done(this);
  
  if (r)
    return *r;
  else
    {
      rectangle_t rc;
      rc.defined = false;
      return rc;
    }
}


/**
 * Calculate the combined advisory minimum size of all components
 * 
 * @return  Advisory minimum size for the container
 */
static size2_t minimum_size_h(__this__)
{ 
  itk_component** children = CONTAINER(this)->children;
  long i, n = CONTAINER(this)->children_count;
  size2_t rc, t;
  rc.width = rc.height = 0;
  rc.defined = true;
  for (i = 0; i < n; i++)
    {
      t = (*(children + i))->minimum_size;
      if (t.width > 0)
	rc.width += t.width;
      if ((rc.height < t.height) && (t.height > 0))
	rc.height = t.height;
    }
  if (n)
    rc.width += GAP(this) * (n - 1);
  return rc;
}


/**
 * Calculate the combined advisory maximum size of all components
 * 
 * @return  Advisory maximum size for the container
 */
static size2_t maximum_size_h(__this__)
{
  bool_t unbounded = false;
  itk_component** children = CONTAINER(this)->children;
  long i, n = CONTAINER(this)->children_count;
  size2_t rc, t;
  rc.width = 0;
  rc.height = UNBOUNDED;
  rc.defined = true;
  for (i = 0; i < n; i++)
    {
      t = (*(children + i))->maximum_size;
      if (t.width < 0)
	unbounded = true;
      else
	rc.width += t.width;
      if (((rc.height < 0) || (rc.height > t.height)) && (t.height >= 0))
	rc.height = t.height;
    }
  if (n)
    rc.width += GAP(this) * (n - 1);
  else
    if (unbounded)
      rc.width = UNBOUNDED;
  return rc;
}


/**
 * Calculate the combined preferred size of all components
 * 
 * @return  Rreferred size for the container
 */
static size2_t preferred_size_h(__this__)
{
  size2_t min = this->minimum_size(this);
  size2_t max = this->minimum_size(this);
  itk_component** children = CONTAINER(this)->children;
  long i, n = CONTAINER(this)->children_count;
  size2_t rc, t;
  rc.defined = true;
  rc.height = min.height;
  rc.width = 0;
  
  for (i = 0; i < n; i++)
    {
      t = (*(children + i))->preferred_size;
      if (rc.height < t.height)
	rc.height = t.height;
      rc.width += t.width;
    }
  if (n)
    rc.width += GAP(this) * (n - 1);
  
  if ((rc.height > max.height) && (max.height >= 0))
    rc.height = max.height;
  if (rc.width < min.width)
    rc.width = min.width;
  if ((rc.width > max.width) && (max.width >= 0))
    rc.width = max.width;
  return rc;
}


/**
 * Calculate the combined advisory minimum size of all components
 * 
 * @return  Advisory minimum size for the container
 */
static size2_t minimum_size_v(__this__)
{ 
  itk_component** children = CONTAINER(this)->children;
  long i, n = CONTAINER(this)->children_count;
  size2_t rc, t;
  rc.width = rc.height = 0;
  rc.defined = true;
  for (i = 0; i < n; i++)
    {
      t = (*(children + i))->minimum_size;
      if (t.height > 0)
	rc.height += t.height;
      if ((rc.width < t.width) && (t.width > 0))
	rc.width = t.width;
    }
  if (n)
    rc.height += GAP(this) * (n - 1);
  return rc;
}


/**
 * Calculate the combined advisory maximum size of all components
 * 
 * @return  Advisory maximum size for the container
 */
static size2_t maximum_size_v(__this__)
{
  bool_t unbounded = false;
  itk_component** children = CONTAINER(this)->children;
  long i, n = CONTAINER(this)->children_count;
  size2_t rc, t;
  rc.width = UNBOUNDED;
  rc.height = 0;
  rc.defined = true;
  for (i = 0; i < n; i++)
    {
      t = (*(children + i))->maximum_size;
      if (t.height < 0)
	unbounded = true;
      else
	rc.height += t.height;
      if (((rc.width < 0) || (rc.width > t.width)) && (t.width >= 0))
	rc.width = t.width;
    }
  if (n)
    rc.height += GAP(this) * (n - 1);
  else
    if (unbounded)
      rc.height = UNBOUNDED;
  return rc;
}


/**
 * Calculate the combined preferred size of all components
 * 
 * @return  Rreferred size for the container
 */
static size2_t preferred_size_v(__this__)
{
  size2_t min = this->minimum_size(this);
  size2_t max = this->minimum_size(this);
  itk_component** children = CONTAINER(this)->children;
  long i, n = CONTAINER(this)->children_count;
  size2_t rc, t;
  rc.defined = true;
  rc.height = 0;
  rc.width = min.width;
  
  for (i = 0; i < n; i++)
    {
      t = (*(children + i))->preferred_size;
      if (rc.width < t.width)
	rc.width = t.width;
      rc.height += t.height;
    }
  if (n)
    rc.height += GAP(this) * (n - 1);
  
  if ((rc.width > max.width) && (max.width >= 0))
    rc.width = max.width;
  if (rc.height < min.height)
    rc.height = min.height;
  if ((rc.height > max.height) && (max.height >= 0))
    rc.height = max.height;
  return rc;
}


/**
 * Destructor
 */
static void free_line_layout(__this__)
{
  itk_hash_table* hash_table = PREPARED(this);
  if (hash_table)
    itk_free_hash_table(hash_table, false, false);
  free(GAP_(this));
  if (BUF(this))
    free(BUF_(this));
  free(this->data);
  free(this);
}


/**
 * Constructor
 * 
 * @param  container    The container which uses the layout manager
 * @param  orientation  Whether the components are lined up horizontally
 * @param  gap          The size of the gap between components
 */
itk_layout_manager* itk_new_line_layout(itk_component* container, int8_t orientation, dimension_t gap)
{
  itk_layout_manager* rc = malloc(sizeof(itk_layout_manager));
  dimension_t* gap_ = malloc(sizeof(dimension_t));
  bool_t is_horizontal = orientation < 2;
  bool_t is_reversed = orientation & 1;
  rc->data = malloc(4 * sizeof(void*));
  if (is_reversed)
    rc->prepare      = is_horizontal ? prepare_hr : prepare_vr;
  else
    rc->prepare      = is_horizontal ? prepare_h  : prepare_v;
  rc->done           = done;
  rc->locate         = locate;
  rc->minimum_size   = is_horizontal ? minimum_size_h   : minimum_size_v;
  rc->preferred_size = is_horizontal ? preferred_size_h : preferred_size_v;
  rc->maximum_size   = is_horizontal ? maximum_size_h   : maximum_size_v;
  rc->free = free_line_layout;
  CONTAINER_(rc) = container;
  PREPARED_(rc) = NULL;
  GAP_(rc) = gap_;
  GAP(rc) = gap;
  BUF_(rc) = NULL;
  return rc;
}

