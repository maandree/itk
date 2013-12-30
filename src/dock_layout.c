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


#define __this__  struct _itk_layout_manager* this

#define CONTAINER(layout)  *((void**)(layout->data) + 0)
#define PREPARED(layout)   *((void**)(layout->data) + 1)


/**
 * Prepare the layout manager for locating of multiple components, probably all of them
 */
static void prepare(__this__)
{
}

/**
 * End of `prepare` requirement
 */
static void done(__this__)
{
  itk_hash_table* hash_table = PREPARED(this);
  if (hash_table)
    itk_free_hash_table(hash_table);
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

/**
 * Calculate the combined advisory minimum size of all components
 * 
 * @return  Advisory minimum size for the container
 */
static size2_t minimum_size(__this__)
{
}

/**
 * Calculate the combined preferred size of all components
 * 
 * @return  Rreferred size for the container
 */
static size2_t preferred_size(__this__)
{
}

/**
 * Calculate the combined advisory maximum size of all components
 * 
 * @return  Advisory maximum size for the container
 */
static size2_t maximum_size(__this__)
{
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
    itk_free_hash_table(hash_table);
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

