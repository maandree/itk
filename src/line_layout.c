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
 * 
 * @param  MAJOR     The major size (width if layouted out horizontally)
 * @param  MINOR     The minor size (height if layouted out horizontally)
 * @param  AXIS      The major axis (x if layouted out horizontally)
 * @param  REVERSED  true iff layout out from right to top or bottom up
 */
#define prepare_(this, MAJOR, MINOR, AXIS, REVERSED)			\
  ({									\
    itk_hash_table* prepared = PREPARED_(this) = itk_new_hash_table();	\
    itk_component* container = CONTAINER(this);				\
    long i, n;								\
    if ((n = container->children_count))				\
      {									\
	itk_component** children = container->children;			\
	rectangle_t* buf = BUF_(this) = malloc(n * sizeof(rectangle_t)); \
	dimension_t gap = GAP(this);					\
	dimension_t MINOR = container->size.MINOR;			\
	dimension_t MAJOR = container->size.MAJOR - gap * (n - 1);	\
	position_t AXIS = 0;						\
	for (i = 0; i < n; i++)						\
	  if ((*(children + i))->visible)				\
	    {								\
	      if (((buf + i)->MAJOR = (*(children + i))->minimum_size.MAJOR) < 0) \
		(buf + i)->MAJOR = 0;					\
	      MAJOR -= (buf + i)->MAJOR;				\
	    }								\
	while (MAJOR > 0)						\
	  {								\
	    itk_component** child;					\
	    itk_component** end = children + n;				\
	    long can_grow = 0;						\
	    for (child = children; child != end; child++)		\
	      if ((*(children + i))->visible)				\
		if ((buf + i)->MAJOR < (*child)->preferred_size.MAJOR)	\
		  can_grow++;						\
	    if (can_grow)						\
	      {								\
		dimension_t increment = MAJOR / can_grow, max, now;	\
		if (increment == 0)					\
		  increment = 1;					\
		for (child = children; (child != end) && MAJOR; child++) \
		  if ((*(children + i))->visible)			\
		    if ((now = (buf + i)->MAJOR) < (max = (*child)->preferred_size.MAJOR)) \
		      {							\
			dimension_t soon = now + increment;		\
			(buf + i)->MAJOR = soon < max ? soon : max;	\
			MAJOR -= (buf + i)->MAJOR - now;		\
		      }							\
	      }								\
	  }								\
	for (i = 0; i < n; i++)						\
	  {								\
	    itk_hash_table_put(prepared, *(children + i), buf + i);	\
	    if (((buf + i)->defined = (*(children + i))->visible))	\
	      {								\
		(buf + i)->MINOR = MINOR;				\
		(buf + i)->y = (buf + i)->x = 0;			\
		(buf + i)->AXIS = AXIS;					\
		AXIS += gap + (buf + i)->MAJOR;				\
	      }								\
	  }								\
	if (REVERSED)							\
	  {								\
	    rectangle_t* buf = BUF(this);				\
	    dimension_t MAJOR = container->size.MAJOR;			\
	    for (i = 0; i < n; i++)					\
	      (buf + i)->AXIS = MAJOR - (buf + i)->AXIS - (buf + i)->MAJOR; \
	  }								\
      }									\
  })


/**
 * Prepare the layout manager for locating of multiple components, probably all of them
 */
static void prepare_h(__this__)
{
  prepare_(this, width, height, x, false);
}


/**
 * Prepare the layout manager for locating of multiple components, probably all of them
 */
static void prepare_v(__this__)
{
  prepare_(this, height, width, y, false);
}


/**
 * Prepare the layout manager for locating of multiple components, probably all of them
 */
static void prepare_hr(__this__)
{
  prepare_(this, width, height, x, true);
}


/**
 * Prepare the layout manager for locating of multiple components, probably all of them
 */
static void prepare_vr(__this__)
{
  prepare_(this, height, width, y, true);
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
 * @param   MAJOR  The major size (width if layouted out horizontally)
 * @param   MINOR  The minor size (height if layouted out horizontally)
 * @return         Advisory minimum size for the container
 */
#define minimum_size_(this, MAJOR, MINOR)			\
  ({								\
    itk_component** children = CONTAINER(this)->children;	\
    long i, n = CONTAINER(this)->children_count, n_ = 0;	\
    size2_t rc, t;						\
    rc.width = rc.height = 0;					\
    rc.defined = true;						\
    for (i = 0; i < n; i++)					\
      if ((*(children + i))->visible)				\
	{							\
	  t = (*(children + i))->minimum_size;			\
	  if (t.MAJOR > 0)					\
	    rc.MAJOR += t.MAJOR;				\
	  if ((rc.MINOR < t.MINOR) && (t.MINOR > 0))		\
	    rc.MINOR = t.MINOR;					\
	}							\
      else							\
	n_++;							\
    if ((n -= n_))						\
      rc.MAJOR += GAP(this) * (n - 1);				\
    rc /* return */;						\
  })


/**
 * Calculate the combined advisory maximum size of all components
 * 
 * @param   MAJOR  The major size (width if layouted out horizontally)
 * @param   MINOR  The minor size (height if layouted out horizontally)
 * @return         Advisory maximum size for the container
 */
#define maximum_size_(this, MAJOR, MINOR)				\
  ({									\
    bool_t unbounded = false;						\
    itk_component** children = CONTAINER(this)->children;		\
    long i, n = CONTAINER(this)->children_count, n_ = 0;		\
    size2_t rc, t;							\
    rc.MAJOR = 0;							\
    rc.MINOR = UNBOUNDED;						\
    rc.defined = true;							\
    for (i = 0; i < n; i++)						\
      if ((*(children + i))->visible)					\
	{								\
	  t = (*(children + i))->maximum_size;				\
	  if (t.MAJOR < 0)						\
	    unbounded = true;						\
	  else								\
	    rc.MAJOR += t.MAJOR;					\
	  if (((rc.MINOR < 0) || (rc.MINOR > t.MINOR)) && (t.MINOR >= 0)) \
	    rc.MINOR = t.MINOR;						\
	}								\
      else								\
	n_++;								\
    if ((n -= n_))							\
      rc.MAJOR += GAP(this) * (n - 1);					\
    else								\
      if (unbounded)							\
	rc.MAJOR = UNBOUNDED;						\
    rc; /* return */							\
  })


/**
 * Calculate the combined preferred size of all components
 * 
 * @return  Preferred size for the container
 */
#define preferred_size_(this, MAJOR, MINOR)			\
  ({								\
    size2_t min = this->minimum_size(this);			\
    size2_t max = this->minimum_size(this);			\
    itk_component** children = CONTAINER(this)->children;	\
    long i, n = CONTAINER(this)->children_count, n_ = 0;	\
    size2_t rc, t;						\
    rc.defined = true;						\
    rc.MINOR = min.MINOR;					\
    rc.MAJOR = 0;						\
								\
    for (i = 0; i < n; i++)					\
      if ((*(children + i))->visible)				\
	{							\
	  t = (*(children + i))->preferred_size;		\
	  if (rc.MINOR < t.MINOR)				\
	    rc.MINOR = t.MINOR;					\
	  rc.MAJOR += t.MAJOR;					\
	}							\
      else							\
	n_++;							\
    if ((n -= n_))						\
      rc.MAJOR += GAP(this) * (n - 1);				\
								\
    if ((rc.MINOR > max.MINOR) && (max.MINOR >= 0))		\
      rc.MINOR = max.MINOR;					\
    if (rc.MAJOR < min.MAJOR)					\
      rc.MAJOR = min.MAJOR;					\
    if ((rc.MAJOR > max.MAJOR) && (max.MAJOR >= 0))		\
      rc.MAJOR = max.MAJOR;					\
    rc; /* return */						\
  })


/**
 * Calculate the combined advisory minimum size of all components
 * 
 * @return  Advisory minimum size for the container
 */
static size2_t minimum_size_h(__this__)
{
  return minimum_size_(this, width, height);
}


/**
 * Calculate the combined advisory minimum size of all components
 * 
 * @return  Advisory minimum size for the container
 */
static size2_t minimum_size_v(__this__)
{
  return minimum_size_(this, height, width);
}


/**
 * Calculate the combined advisory maximum size of all components
 * 
 * @return  Advisory maximum size for the container
 */
static size2_t maximum_size_h(__this__)
{
  return maximum_size_(this, width, height);
}


/**
 * Calculate the combined advisory maximum size of all components
 * 
 * @return  Advisory maximum size for the container
 */
static size2_t maximum_size_v(__this__)
{
  return maximum_size_(this, height, width);
}


/**
 * Calculate the combined preferred size of all components
 * 
 * @return  Preferred size for the container
 */
static size2_t preferred_size_h(__this__)
{
  return preferred_size_(this, width, height);
}


/**
 * Calculate the combined preferred size of all components
 * 
 * @return  Preferred size for the container
 */
static size2_t preferred_size_v(__this__)
{
  return preferred_size_(this, height, width);
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

