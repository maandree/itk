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
#include "dock_layout.h"
#include "hash_table.h"
#include "itkmacros.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define __this__  struct _itk_layout_manager* this

#define CONTAINER(layout)  *((void**)(layout->data) + 0)
#define PREPARED(layout)   *((void**)(layout->data) + 1)
#define MIN(a, b)          ((a) < (b) ? (a) : (b))
#define MAX(a, b)          ((a) > (b) ? (a) : (b))


/**
 * Move to heap, but undefine if area is zero
 * 
 * @param   x       The position on the horizontal axis
 * @param   y       The position on the vertical axis
 * @param   width   The width
 * @param   height  The height
 * @return          The area, but `NULL` if the width or height is zero
 */
rectangle_t* nonzero(position_t x, position_t y, dimension_t width, dimension_t height)
{
  rectangle_t* rc = malloc(sizeof(rectangle_t));
  rectangle_t area = new_rectangle(x, y, width, height);
  *rc = area;
  if ((area.width <= 0) || (area.height <= 0))
    rc->defined = false;
  return rc;
}


/**
 * Prepare the layout manager for locating of multiple components, probably all of them
 * 
 * @param  mode  0 for normal, 1 for minimum size, 2 for maximum size, 3 for preferred size
 */
static void prepare_(__this__, char mode)
{
  itk_hash_table* prepared = PREPARED(this);
  itk_component* container = CONTAINER(this);
  
  position_t x = 0, y = 0;
  dimension_t w = container->size.width;
  dimension_t h = container->size.height;
  
  long children_count = container->children_count;
  itk_component** children = container->children;
  long children_ptr = 0;
  
  long yeild_left_head   = 0, yeild_left_tail   = 0;
  long yeild_top_head    = 0, yeild_top_tail    = 0;
  long yeild_right_head  = 0, yeild_right_tail  = 0;
  long yeild_bottom_head = 0, yeild_bottom_tail = 0;
  
  itk_component** yeilds       = alloca(4 * children_count * sizeof(itk_component*));
  itk_component** yeild_left   = yeilds      + children_count;
  itk_component** yeild_top    = yeild_left  + children_count;
  itk_component** yeild_right  = yeild_top   + children_count;
  itk_component** yeild_bottom = yeild_right + children_count;
  
  if (mode)
    w = h = (1 << (sizeof(dimension_t) / sizeof(int8_t) - 1)) - 1;
  
#define __EDGE(EDGE, PUT, IS_LOW, FIRST, SECOND, SAME, PERP, PERP_EDGE, AXIS, HORZ) \
  ({									\
    if (strstr(constraints, #EDGE))					\
      {									\
	dimension_t _ = MIN(SAME, HORZ ? child_width : child_height);	\
	dimension_t S = PERP;						\
	position_t P = HORZ ? y : x;					\
	while (yeild_##EDGE##_head && (_ > 0))				\
	  {								\
	    itk_component* yeilded = *(yeild_##EDGE + yeild_##EDGE##_tail++); \
	    if ((r = itk_hash_table_get(prepared, yeilded)))		\
	      if (r->defined)						\
		{							\
		  position_t yx = r->x, yy = r->y;			\
		  dimension_t yw = r->width, yh = r->height;		\
		  if (HORZ)						\
		    r = nonzero(yx + IS_LOW * _, yy, yw - _, yh);	\
		  else							\
		    r = nonzero(yx, yy + IS_LOW * _, yw, yh - _);	\
		  itk_hash_table_put(prepared, yeilded, r);		\
		  S += y##PERP;						\
		  if (strstr(yeilded->constraints, #PERP_EDGE))		\
		    P -= y##PERP;					\
		}							\
	  }								\
	itk_hash_table_put(prepared, child, HORZ ? nonzero(PUT, P, _, S) : nonzero(P, PUT, S, _)); \
	if (IS_LOW)							\
	  AXIS += _;							\
	SAME -= _;							\
	if (r->defined && !strcmp(constraints, #EDGE))			\
	  {								\
	    long n = strstr(constraints, " ") - constraints;		\
	    char* FIRST##_ = alloca((n + 1) * sizeof(char));		\
	    char* SECOND##_ = strstr(strstr(constraints, " ") + 1, " ") + 1; \
	    long i, FIRST, SECOND;					\
	    for (i = 0; i < n; i++)					\
	      *(FIRST##_ + i) = *(constraints + i);			\
	    *(FIRST##_ + n) = 0;					\
	    FIRST = atol(FIRST##_);					\
	    SECOND = atol(SECOND##_);					\
	    for (i = 0; i < FIRST; i++)					\
	      *(yeild_##FIRST + yeild_##FIRST##_head++) = child;	\
	    for (i = 0; i < SECOND; i++)				\
	      *(yeild_##SECOND + yeild_##SECOND##_head++) = child;	\
	  }								\
	continue;							\
      }									\
  })
  
  for (; children_ptr < children_count; children_ptr++)
    {
      itk_component* child = *(children + children_ptr);
      char* constraints = child->constraints;
      rectangle_t* r;
      if (constraints)
	{
	  size2_t child_size =
	    mode == 0 ? child->preferred_size :
	    mode == 1 ? child->minimum_size   :
	    mode == 2 ? child->maximum_size   :
			child->preferred_size;
	  dimension_t child_width  = child_size.width;
	  dimension_t child_height = child_size.height;
	  
	  if ((child_width | child_height) < 0)
	    {
	      itk_hash_table_put(prepared, child, nonzero(x, y, -1, -1));
	      continue;
	    }
	  
	  __EDGE(left, x, 1, bottom, top, w, h, top, x, 1);
	  __EDGE(top, y, 1, left, right, h, w, left, y, 0);
	  __EDGE(right, x + w - _, 0, top, bottom, w, h, top, x, 1);
	  __EDGE(bottom, y + h - _, 0, right, left, h, w, left, y, 0);
	  
	  {
	    long n = strstr(constraints, " ") - constraints;
	    char* x_ = alloca((n + 1) * sizeof(char));
	    char* y_ = constraints + n + 1;
	    if (strstr(y_, " ") == NULL)
	      {
		long i;
		position_t x, y;
		for (i = 0; i < n; i++)
		  *(x_ + i) = *(constraints + i);
		*(x_ + n) = 0;
		x = (position_t)atoll(x_);
		y = (position_t)atoll(y_);
		itk_hash_table_put(prepared, child, nonzero(x, y, child_width, child_height));
		continue;
	      }
	  }
	}
      itk_hash_table_put(prepared, child, nonzero(x, y, w, h)); /* contraint == "center" */
      w = h = 0;
    }
  
  PREPARED(this) = itk_new_hash_table();
}


/**
 * Prepare the layout manager for locating of multiple components, probably all of them
 */
static void prepare(__this__)
{
  prepare_(this, 0);
}


/**
 * End of `prepare` requirement
 */
static void done(__this__)
{
  itk_hash_table* hash_table = PREPARED(this);
  if (hash_table)
    itk_free_hash_table(hash_table, true, false);
  PREPARED(this) = NULL;
}


/**
 * Locates the positions of the corners of a component
 * 
 * @param   child  The child, to the component using the layout manager, of interest
 * @return         The rectangle the child is confound in
 */
static rectangle_t locate(__this__, struct _itk_component* child)
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


#define __CALC_SIZE(MODE, SIZE)					\
  ({								\
    dimension_t cw = 0, ch = 0;					\
    dimension_t lw = 0, th = 0;					\
    dimension_t rw = 0, bh = 0;					\
    dimension_t x = 0, y = 0;					\
    dimension_t w, h;						\
								\
    if (PREPARED(this) == NULL)					\
      this->done(this);						\
    prepare_(this, MODE);					\
    {								\
      itk_hash_table* prepared = PREPARED(this);		\
      itk_component* container = CONTAINER(this);		\
      long children_count = container->children_count;		\
      itk_component** children = container->children;		\
      long children_ptr = 0;					\
								\
      for (; children_ptr < children_count; children_ptr++)	\
	{							\
	  itk_component* child = *(children + children_ptr);	\
	  char* constraints = child->constraints;		\
	  rectangle_t* r;					\
								\
	  if ((r = itk_hash_table_get(PREPARED(this), child)))	\
	    {							\
	      if (strstr(constraints, "left"))			\
		{						\
		  lw += child->SIZE.width;			\
		  y = MAX(y, r->y + child->SIZE.height + bh);	\
		}						\
	      else if (strstr(constraints, "top"))		\
		{						\
		  th += child->SIZE.height;			\
		  x = MAX(x, r->x + child->SIZE.width + rw);	\
		}						\
	      else if (strstr(constraints, "right"))		\
		{						\
		  rw += child->SIZE.width;			\
		  y = MAX(y, r->y + child->SIZE.height + th);	\
		}						\
	      else if (strstr(constraints, "bottom"))		\
		{						\
		  bh += child->SIZE.height;			\
		  x = MAX(x, r->x + child->SIZE.width + lw);	\
		}						\
	      else if (strstr(constraints, "cent"))		\
		{						\
		  cw = MAX(cw, child->SIZE.width);		\
		  ch = MAX(ch, child->SIZE.height);		\
		}						\
	      else						\
		{						\
		  x = MAX(x, r->x + r->width);			\
		  y = MAX(y, r->y + r->height);			\
		}						\
	    }							\
	}							\
    }								\
    this->done(this);						\
								\
    w = lw + cw + rw;						\
    h = th + ch + bh;						\
    rc = new_size2(MAX(x, w), MAX(y, h));			\
  })


/**
 * Calculate the combined advisory minimum size of all components
 * 
 * @return  Advisory minimum size for the container
 */
static size2_t minimum_size(__this__)
{ 
  size2_t rc;
  __CALC_SIZE(1, minimum_size);
  return rc;
}


/**
 * Calculate the combined advisory maximum size of all components
 * 
 * @return  Advisory maximum size for the container
 */
static size2_t maximum_size(__this__)
{
  size2_t rc;
  __CALC_SIZE(1, maximum_size);
  return rc;
}


/**
 * Calculate the combined preferred size of all components
 * 
 * @return  Rreferred size for the container
 */
static size2_t preferred_size(__this__)
{
  size2_t rc;
  __CALC_SIZE(2, preferred_size);
  return rc;
}


/**
 * Constructor
 * 
 * A dock layout can use the following contraints:
 *     • "left"   — Dock to left edge
 *     • "top"    — Dock to top edge
 *     • "right"  — Dock to right edge
 *     • "bottom" — Dock to bottom edge
 *     • "center" — Fill the centre
 *     • "centre" — Fill the centre (perhaps you perfer nouns)
 *     • An output of `itk_dock_layout_yeild`
 *     • "%x %y"  — Absolute position of the component
 * 
 * @param  container  The container which uses the layout manager
 */
itk_layout_manager* itk_new_dock_layout(itk_component* container)
{
  itk_layout_manager* rc = malloc(sizeof(itk_layout_manager));
  rc->data = malloc(2 * sizeof(void*));
  rc->prepare = prepare;
  rc->done = done;
  rc->locate = locate;
  rc->minimum_size = minimum_size;
  rc->preferred_size = preferred_size;
  rc->maximum_size = maximum_size;
  CONTAINER(rc) = container;
  PREPARED(rc) = NULL;
  return rc;
}


/**
 * Destructor
 */
void itk_free_dock_layout(__this__)
{
  itk_hash_table* hash_table = PREPARED(this);
  if (hash_table)
    itk_free_hash_table(hash_table, true, false);
  free(this->data);
  free(this);
}


/**
 * Creates a complexer layout constraint that yeilds for later docked components
 * 
 * @param   anticlockwise  The number of components for which to yeild, that are position at the edge the 90° anticlockwise position
 * @param   edge           The edge to which to dock: "left", "top", "right" or "bottom"
 * @param   clockwise      The number of components for which to yeild, that are position at the edge the 90° clockwise position
 * @return                 The constraint to use, do not forget to free it when it is not in use anymore
 */
char* itk_dock_layout_yeild(long anticlockwise, char* edge, long clockwise)
{
  char* rc;
  long len = 3, x;
  
  /* Calculate length of output to minimise memory usage and support arbitrarly long `long`:s */
  if (anticlockwise == 0)
    len++;
  else
    for (x = 1; x < anticlockwise; x *= 10)
      len++;
  
  if (clockwise == 0)
    len++;
  else
    for (x = 1; x < clockwise; x *= 10)
      len++;
  
  for (x = 0; *(edge + x); x++)
    len++;
  
  /* Create value */
  rc = malloc(len * sizeof(char));
  sprintf(rc, "%li %s %li", anticlockwise, edge, clockwise);
  return rc;
}


/**
 * Creates contraint for a component to be placed at an absolute position
 * 
 * @param   position  The position of the component
 * @return            The constraint to use, do not forget to free it when it is not in use anymore
 */
char* itk_dock_layout_absolute(position2_t position)
{
  char* rc;
  long len = 2, x;
  
  /* Calculate length of output to minimise memory usage */
  if (position.x == 0)
    len++;
  else
    for (x = 1; x < position.x; x *= 10)
      len++;
  
  if (position.y == 0)
    len++;
  else
    for (x = 1; x < position.y; x *= 10)
      len++;
  
  /* Create value */
  rc = malloc(len * sizeof(char));
  sprintf(rc, "%li %li", position.x, position.y);
  return rc;
}

