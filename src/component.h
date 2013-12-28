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
  itk_component* parent;
  
  /**
   * The number of children the component has
   */
  long children_count;
  
  /**
   * The component's childred
   */
  itk_component* children;
  
  /**
   * Layout constraints
   */
  void* constraints;
  
  /**
   * The component's layout manager
   */
  itk_layout_manager* layout_manager;
  
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
  
} itk_component;


#endif

