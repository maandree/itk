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
#include "margin_layout.h"
#include "itkmacros.h"

#include <stdlib.h>


#define __this__  itk_layout_manager* this

#define CONTAINER_(layout)  *((void**)(layout->data + 0))
#define MARGINS_(layout)    *((void**)(layout->data + 1))

#define CONTAINER(layout)   ((itk_component*)(CONTAINER_(layout)))
#define MARGINS(layout)     ((dimension_t*)(MARGINS_(layout)))

#define LEFT(layout)        *(MARGINS(layout) + 0)
#define TOP(layout)         *(MARGINS(layout) + 1)
#define RIGHT(layout)       *(MARGINS(layout) + 2)
#define BOTTOM(layout)      *(MARGINS(layout) + 3)


/**
 * Prepare the layout manager for locating of multiple components, probably all of them
 */
static void prepare(__this__)
{
  /* do nothing */
}


/**
 * End of `prepare` requirement
 */
static void done(__this__)
{
  /* do nothing */
}


/**
 * Locates the positions of the corners of a component
 * 
 * @param   child  The child, to the component using the layout manager, of interest
 * @return         The rectangle the child is confound in
 */
static rectangle_t locate(__this__, itk_component* child)
{
  size2_t c = CONTAINER(this)->size;
  position_t x = LEFT(this), y = TOP(this);
  dimension_t w = c.width - RIGHT(this);
  dimension_t h = c.height - BOTTOM(this);
  if ((w | h) < 0)
    {
      w = c.width;
      h = c.height;
      x = y = 0;
    }
  return new_rectangle(x, y, w, h);
}


/**
 * Calculate the combined advisory minimum size of all components
 * 
 * @return  Advisory minimum size for the container
 */
static size2_t minimum_size(__this__)
{ 
  itk_component** children = CONTAINER(this)->children;
  long i, n = CONTAINER(this)->children_count;
  size2_t rc;
  rc.width = rc.height = 0;
  rc.defined = true;
  for (i = 0; i < n; i++)
    {
      if (rc.width < (*(children + i))->minimum_size.width)
	rc.width = (*(children + i))->minimum_size.width;
      if (rc.height < (*(children + i))->minimum_size.height)
	rc.height = (*(children + i))->minimum_size.height;
    }
  return rc;
}


/**
 * Calculate the combined advisory maximum size of all components
 * 
 * @return  Advisory maximum size for the container
 */
static size2_t maximum_size(__this__)
{
  itk_component** children = CONTAINER(this)->children;
  long i, n = CONTAINER(this)->children_count;
  size2_t rc, t;
  rc.width = rc.height = UNBOUNDED;
  rc.defined = true;
  for (i = 0; i < n; i++)
    {
      t = (*(children + i))->maximum_size;
      if (((rc.width < 0) || (rc.width > t.width)) && (t.width >= 0))
	rc.width = t.width;
      if (((rc.height < 0) || (rc.height > t.height)) && (t.height >= 0))
	rc.height = t.height;
    }
  return rc;
}


/**
 * Calculate the combined preferred size of all components
 * 
 * @return  Rreferred size for the container
 */
static size2_t preferred_size(__this__)
{
  itk_component** children = CONTAINER(this)->children;
  long i, n = CONTAINER(this)->children_count;
  size2_t rc;
  rc.width = rc.height = 0;
  rc.defined = true;
  for (i = 0; i < n; i++)
    {
      if (rc.width < (*(children + i))->preferred_size.width)
	rc.width = (*(children + i))->preferred_size.width;
      if (rc.height < (*(children + i))->preferred_size.height)
	rc.height = (*(children + i))->preferred_size.height;
    }
  return rc;
}


/**
 * Destructor
 */
static void free_margin_layout(__this__)
{
  free(MARGINS_(this));
  free(this->data);
  free(this);
}


/**
 * Constructor
 * 
 * @param  container  The container which uses the layout manager
 * @param  left       The size of the left margin
 * @param  top        The size of the top margin
 * @param  right      The size of the right margin
 * @param  bottom     The size of the bottom margin
 */
itk_layout_manager* itk_new_margin_layout(itk_component* container, dimension_t left, dimension_t top, dimension_t right, dimension_t bottom)
{
  itk_layout_manager* rc = malloc(sizeof(itk_layout_manager));
  dimension_t* margins = malloc(4 * sizeof(dimension_t));
  rc->data = malloc(2 * sizeof(void*));
  rc->prepare = prepare;
  rc->done = done;
  rc->locate = locate;
  rc->minimum_size = minimum_size;
  rc->preferred_size = preferred_size;
  rc->maximum_size = maximum_size;
  rc->free = free_margin_layout;
  CONTAINER_(rc) = container;
  MARGINS_(rc) = margins;
  LEFT(rc) = left;
  TOP(rc) = top;
  RIGHT(rc) = right;
  BOTTOM(rc) = bottom;
  return rc;
}

