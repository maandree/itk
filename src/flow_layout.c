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
#include "flow_layout.h"
#include "hash_table.h"
#include "itkmacros.h"

#include <stdlib.h>


#define __this__  struct _itk_layout_manager* this

#define CONTAINER_(layout)  *((void**)(layout->data) + 0)
#define PREPARED_(layout)   *((void**)(layout->data) + 1)
#define GAP_(layout)        *((void**)(layout->data) + 2)
#define ALIGN_(layout)      *((void**)(layout->data) + 3)

#define CONTAINER(layout)   ((itk_component*)(CONTAINER_(layout)))
#define PREPARED(layout)    ((itk_hash_table*)(PREPARED_(layout)))
#define HGAP(layout)        *((dimension_t*)(GAP_(layout) + 0))
#define VGAP(layout)        *((dimension_t*)(GAP_(layout) + 1))
#define ALIGN(layout)       *((int8_t*)(ALIGN_(layout)))


/**
 * Prepare the layout manager for locating of multiple components, probably all of them
 */
static void prepare(__this__)
{
  itk_hash_table* prepared = PREPARED_(this) = itk_new_hash_table();
  itk_component* container = CONTAINER(this);
  itk_component** children = container->children;
  long n = container->children_count, line_n = 0;
  itk_component** line_c = alloca(n * sizeof(itk_component*));
  rectangle_t** line_r = alloca(n * sizeof(rectangle_t*));
  int8_t align = ALIGN(this);
  size2_t bounds = container->size;
  dimension_t width = 0, height = 0;
  position_t y = 0;
  dimension_t hgap = HGAP(this), vgap = VGAP(this);
  itk_component* child;
  rectangle_t* r;
  long i, j;
  
  bounds.width += hgap;
  
  for (i = 0; i < n; i++)
    {
    add_components:
      child = *(children + i);
      r = malloc(sizeof(rectangle_t));
      if ((r->defined = child->visible) == false)
	{
	  itk_hash_table_put(prepared, child, r);
	  continue;
	}
      if (width + child->preferred_size.width + hgap <= bounds.width)
	{
	  r->y = y;
	  r->x = (position_t)width;
	  r->height = child->preferred_size.height;
	  r->width = child->preferred_size.width;
	  width += child->preferred_size.width + hgap;
	  itk_hash_table_put(prepared, *(line_c + line_n) = child, *(line_r + line_n) = r);
	  line_n++;
	}
      else
	goto align_components;
    }
  
 align_components:
  if (line_n && (width < bounds.width))
    {
      width = bounds.width - width;
      if (align == ALIGNMENT_RIGHT)
	for (j = 0; j < line_n; j++)
	  (*(line_r + j))->x += width;
      else if (align == ALIGNMENT_JUSTIFY)
	while (width)
	  {
	    long can_grow = 0;
	    dimension_t max;
	    for (j = 0; j < line_n; j++)
	      {
		max = (*(line_c + j))->maximum_size.width;
		if ((max < 0) || ((*(line_r + j))->width < max))
		  can_grow++;
	      }
	    if (can_grow)
	      {
		dimension_t increment = width / can_grow, now;
		if (increment == 0)
		  increment = 1;
		for (j = 0; (j < line_n) && width; j++)
		  {
		    now = (*(line_r + j))->width;
		    max = (*(line_c + j))->maximum_size.width;
		    if ((max < 0) || (now < max))
		      {
			dimension_t soon = now + increment;
			(*(line_r + j))->width = soon < max ? soon : max;
			width -= (*(line_r + j))->width - now;
		      }
		  }
	      }
	    else if (line_n > 1)
	      {
		dimension_t increment = width / (line_n - 1);
		dimension_t offset = 0;
		if (increment == 0)
		  increment = 1;
		for (j = 1; j < line_n; j++)
		  {
		    if (width)
		      {
			offset += increment;
			width -= increment;
		      }
		    (*(line_r + j))->x += offset;
		  }
	      }
	    else
	      (*line_r)->width += width;
	  }
    }
  
  for (j = 0; j < line_n; j++)
    if (height < (*(line_r + j))->height)
      height = (*(line_r + j))->height;
  for (j = 0; j < line_n; j++)
    if (height > (*(line_c + j))->maximum_size.height)
      (*(line_r + j))->height = (*(line_c + j))->maximum_size.height;
    else
      (*(line_r + j))->height = height;
  
  if (i < n)
    {
      y += height + vgap;
      width = height = 0;
      line_n = 0;
      goto add_components;
    }
}


/**
 * End of `prepare` requirement
 */
static void done(__this__)
{
  itk_hash_table* hash_table = PREPARED(this);
  if (hash_table)
    itk_free_hash_table(hash_table, true, false);
  PREPARED_(this) = NULL;
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
 * Calculate the combined advisory maximum size of all components
 * 
 * @return  Advisory maximum size for the container
 */
static size2_t maximum_size(__this__)
{
  bool_t unbounded = false;
  itk_component** children = CONTAINER(this)->children;
  long i, n = CONTAINER(this)->children_count, n_ = 0;
  size2_t rc, t;
  rc.width = 0;
  rc.height = UNBOUNDED;
  rc.defined = true;
  for (i = 0; i < n; i++)
    if ((*(children + i))->visible)
      {
	t = (*(children + i))->maximum_size;
	if (t.width < 0)
	  unbounded = true;
	else
	  rc.width += t.width;
	if (((rc.height < 0) || (rc.height > t.height)) && (t.height >= 0))
	  rc.height = t.height;
      }
    else
      n_++;
  if ((n -= n_))
    rc.width += HGAP(this) * (n - 1);
  else
    if (unbounded)
      rc.width = UNBOUNDED;
  return rc;
}


/**
 * Calculate the combined preferred size of all components
 * 
 * @return  Preferred size for the container
 */
static size2_t preferred_size(__this__)
{
  size2_t min = this->minimum_size(this);
  size2_t max = this->minimum_size(this);
  itk_component** children = CONTAINER(this)->children;
  long i, n = CONTAINER(this)->children_count, n_ = 0;
  size2_t rc, t;
  rc.defined = true;
  rc.height = min.height;
  rc.width = 0;
  
  for (i = 0; i < n; i++)
    if ((*(children + i))->visible)
      {
	t = (*(children + i))->preferred_size;
	if (rc.height < t.height)
	  rc.height = t.height;
	rc.width += t.width;
      }
    else
      n_++;
  if ((n -= n_))
    rc.width += HGAP(this) * (n - 1);
  
  if ((rc.height > max.height) && (max.height >= 0))
    rc.height = max.height;
  if (rc.width < min.width)
    rc.width = min.width;
  if ((rc.width > max.width) && (max.width >= 0))
    rc.width = max.width;
  return rc;
}


/**
 * Destructor
 */
static void free_line_layout(__this__)
{
  itk_hash_table* hash_table = PREPARED(this);
  if (hash_table)
    itk_free_hash_table(hash_table, true, false);
  free(GAP_(this));
  free(ALIGN_(this));
  free(this->data);
  free(this);
}


/**
 * Constructor
 * 
 * @param  container  The container which uses the layout manager
 * @param  alignment  The alignment of the components
 * @param  hgap       The size of the horizontal gap between components
 * @param  vgap       The size of the vertical gap between components
 */
itk_layout_manager* itk_new_flow_layout(itk_component* container, int8_t alignment, dimension_t hgap, dimension_t vgap)
{
  itk_layout_manager* rc = malloc(sizeof(itk_layout_manager));
  dimension_t* gap_ = malloc(2 * sizeof(dimension_t));
  int8_t* align_ = malloc(sizeof(int8_t));
  rc->data = malloc(4 * sizeof(void*));
  rc->prepare        = prepare;
  rc->done           = done;
  rc->locate         = locate;
  rc->minimum_size   = preferred_size;
  rc->preferred_size = preferred_size;
  rc->maximum_size   = alignment == ALIGNMENT_JUSTIFY ? maximum_size : preferred_size;
  rc->free = free_line_layout;
  CONTAINER_(rc) = container;
  PREPARED_(rc) = NULL;
  GAP_(rc) = gap_;
  HGAP(rc) = hgap;
  VGAP(rc) = vgap;
  ALIGN_(rc) = align_;
  ALIGN(rc) = alignment;
  return rc;
}

