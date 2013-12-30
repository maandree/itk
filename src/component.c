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
#include "component.h"
#include "layout_manager.h"
#include "graphics.h"
#include "itktypes.h"
#include "itkmacros.h"

#include <stdlib.h>

#define __this__  itk_component* this


/**
 * Locates the positions of the corners of a child
 * 
 * @param   child  The child
 * @return         The rectangle the child is confound in
 */
static rectangle_t locate_child(__this__, struct _itk_component* child)
{
  rectangle_t rc;
  
  if (this->layout_manager)
    return this->layout_manager->locate(this->layout_manager, child);
  
  rc.defined = true;
  rc.x = 0;
  rc.y = 0;
  rc.width = child->preferred_size.width;
  rc.height = child->preferred_size.height;
  if (child->constraints)
    {
      rc.x = ((position2_t*)(child->constraints))->x;
      rc.y = ((position2_t*)(child->constraints))->y;
    }
  
  child->size = child->preferred_size;
  return rc;
}


/**
 * Synchronises the graphics
 * 
 * @param  area  Area to synchronise, `NULL` for everything
 */
static void sync_area(__this__, rectangle_t* area)
{
  itk_graphics* g;
  rectangle_t rect;
  
  if (this->parent == NULL)
    return;
  
  if (this->background_colour.argb_colour.c.alpha != 255)
    {
      rect = this->parent->locate_child(this->parent, this);
      if (rect.defined && ((rect.width | rect.height) > 0))
	{
	  if ((area == NULL) || (area->defined == false) || ((area->width | area->height) < 0))
	    this->parent->sync_area(this->parent, &rect);
	  else
	    {
	      rect.x += area->x;
	      rect.y += area->y;
	      rect.width = area->width;
	      rect.height = area->height;
	      this->parent->sync_area(this->parent, &rect);
	    }
	}
    }
  else if ((g = this->parent->sync_child(this->parent, this)))
    {
      if ((area->defined) && ((area->width | area->height) >= 0))
	g->clip(g, *area);
      this->paint(this, g);
    }
}

/**
 * Synchronises the graphics on a child
 * 
 * @param   child  The child
 * @return         The object with which to paint
 */
static itk_graphics* sync_child(__this__, itk_component* child)
{
  if (this->parent == NULL)
    return NULL;
  
  itk_graphics* g = this->parent->sync_child(this->parent, this);
  if (g == NULL)
    return NULL;
  
  rectangle_t rect = this->locate_child(this, child);
  if ((rect.defined == false) && (rect.width | rect.height) < 0)
    return NULL;
  
  g->clip(g, rect);
  g->translate(g, new_position2(-(rect.x), -(rect.y)));
  return g;
}


/**
 * Synchronises the graphics
 */
static void sync(__this__)
{
  sync_area(this, NULL);
}


/**
 * Repaint the component and its childred
 * 
 * @param  g  The object with which to paint
 */
static void paint(__this__, itk_graphics* g)
{
  /* TODO implement buffer support */
  
  this->paint_component(this, g);
  this->paint_children(this, g);
}

/**
 * Repaint the component
 * 
 * @param  g  The object with which to paint
 */
static void paint_component(__this__, itk_graphics* g)
{
  g->set_colour(g, this->background_colour);
  g->fill_rectangle(g, new_rectangle(0, 0, this->size.width, this->size.height));
}

/**
 * Repaint the component's children
 * 
 * @param  g  The object with which to paint
 */
static void paint_children(__this__, itk_graphics* g)
{
  rectangle_t rect;
  itk_component* child;
  long i = 0, n = this->children_count;
  
  if (this->layout_manager)
    this->layout_manager->prepare(this->layout_manager);
  
  for (; i < n; i++)
    {
      child = *(this->children + i);
      rect = this->locate_child(this, child);
      if (rect.defined && (rect.width | rect.height) > 0)
	{
	  itk_graphics* child_g = g->create(g, rect);
	  child->paint(child, child_g);
	  free(child_g);
	}
    }
  
  if (this->layout_manager)
    this->layout_manager->done(this->layout_manager);
}


/**
 * Add a child component to the component
 * 
 * @param  child  The child
 */
static void add_child(__this__, itk_component* child)
{
  if (this->children_count == 0)
    this->children = malloc(sizeof(itk_component*));
  else if (this->children_count & -(this->children_count) == this->children_count)
    this->children = realloc(this->children, this->children_count * 2 * sizeof(itk_component*));
  *(this->children + this->children_count++) = child;
}

/**
 * Remove a child component from the component
 * 
 * @param  child  The child
 */
static void remove_child(__this__, itk_component* child)
{
  long i;
  for (i = 0;; i++)
    if (*(this->children + i) == child)
      {
	this->remove_child_by_index(this, i);
	break;
      }
}

/**
 * Remove a child component from the component
 * 
 * @param  child  The index of the child
 */
static void remove_child_by_index(__this__, long child)
{
  long i, n = this->children_count -= 1;
  for (i = child; i < n; i++)
    *(this->children + i) = *(this->children + i + 1);
  if (this->children_count == 0)
    {
      free(this->children);
      this->children = NULL;
    }
  else if (this->children_count & -(this->children_count) == this->children_count)
    this->children = realloc(this->children, this->children_count * sizeof(itk_component*));
}


/**
 * Destructor
 */
void free_component(__this__)
{
  free(this);
}

  
/**
 * Destructor that also frees the layout manager and children
 */
void free_everything_component(__this__)
{
  long i = 0, n = this->children_count;
  
  if (this->layout_manager)
    this->layout_manager->free(this->layout_manager);
  
  for (; i < n; i++)
    (*(this->children + i))->free_everything(*(this->children + i));
  
  this->free(this);
}


/**
 * Constructor
 * 
 * @param  name  The name of the component
 */
itk_component* itk_new_component(char* name)
{
  itk_component* rc = calloc(1, sizeof(itk_component));
  rc->name = name;
  rc->visible = 1;
  rc->background_colour = new_colour_by_opaque_value(0xD3D3D3);
  rc->preferred_size = new_size2(16, 16);
  rc->size = new_size2(16, 16);
  rc->maximum_size = new_size2(UNBOUNDED, UNBOUNDED);
  rc->locate_child = locate_child;
  rc->sync = sync;
  rc->sync_area = sync_area;
  rc->sync_child = sync_child;
  rc->paint = paint;
  rc->paint_component = paint_component;
  rc->paint_children = paint_children;
  rc->add_child = add_child;
  rc->remove_child = remove_child;
  rc->remove_child_by_index = remove_child_by_index;
  rc->free = free_component;
  rc->free_everything = free_everything_component;
  return rc;
}

