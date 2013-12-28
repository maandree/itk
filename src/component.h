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
#ifndef __ITK_COMPONENT_H__
#define __ITK_COMPONENT_H__

#include "itktypes.h"

struct _itk_graphics;
struct _itk_layout_manager;


#define __this__  struct _itk_component* this

/**
 * The root component class
 */
typedef struct _itk_component
{
  /**
   * The name of the component
   */
  char* name;
  
  /**
   * Whether the component is present
   */
  bool_t visible;
  
  /**
   * The component's background colour
   */
  colour_t background_colour;
  
  /**
   * The component's advisory minimum size
   */
  size2_t minimum_size;
  
  /**
   * The component's preferred size
   */
  size2_t preferred_size;
  
  /**
   * The component's size
   */
  size2_t size;
  
  /**
   * The component's advisory maximum size
   */
  size2_t maximum_size;
  
  /**
   * The components's parent
   */
  struct _itk_component* parent;
  
  /**
   * The number of children the component has
   */
  long children_count;
  
  /**
   * The component's childred
   */
  struct _itk_component** children;
  
  /**
   * Layout constraints
   */
  void* constraints;
  
  /**
   * The component's layout manager
   */
  struct _itk_layout_manager* layout_manager;
  
  /**
   * The number of off-screen buffers the component uses
   */
  int8_t buffer_count;
  
  /**
   * The index of the currently off-screen buffer in use by the component
   */
  int8_t buffer_pointer;
  
  /**
   * The component's off-screen buffers
   */
  void** buffers;
  
  
  /**
   * Locates the positions of the corners of a child
   * 
   * @param   child  The child
   * @return         The rectangle the child is confound in
   */
  rectangle_t (*locate_child)(__this__, struct _itk_component* child);
  
  
  /**
   * Synchronises the graphics
   */
  void (*sync)(__this__);
  
  /**
   * Synchronises the graphics
   * 
   * @param  area  Area to synchronise, `NULL` for everything
   */
  void (*sync_area)(__this__, rectangle_t* area);
  
  /**
   * Synchronises the graphics on a child
   * 
   * @param   child  The child
   * @return         The object with which to paint
   */
  struct _itk_graphics* (*sync_child)(__this__, struct _itk_component* child);
  
  
  /**
   * Repaint the component and its childred
   * 
   * @param  g  The object with which to paint
   */
  void (*paint)(__this__, struct _itk_graphics* g);
  
  /**
   * Repaint the component
   * 
   * @param  g  The object with which to paint
   */
  void (*paint_component)(__this__, struct _itk_graphics* g);
  
  /**
   * Repaint the component's children
   * 
   * @param  g  The object with which to paint
   */
  void (*paint_children)(__this__, struct _itk_graphics* g);
  
  
  /**
   * Add a child component to the component
   * 
   * @param  child  The child
   */
  void (*add_child)(__this__, struct _itk_component* child);
  
  /**
   * Remove a child component from the component
   * 
   * @param  child  The child
   */
  void (*remove_child)(__this__, struct _itk_component* child);
  
  /**
   * Remove a child component from the component
   * 
   * @param  child  The index of the child
   */
  void (*remove_child_by_index)(__this__, long child);
  
} itk_component;

#undef __this__


/**
 * Constructor
 * 
 * @param  name  The name of the component
 */
itk_component* itk_new_component(char* name);

#endif

