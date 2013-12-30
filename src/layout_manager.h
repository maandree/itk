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
#ifndef __ITK_LAYOUT_MANAGER_H__
#define __ITK_LAYOUT_MANAGER_H__

#include "itktypes.h"

struct _itk_component;


#define __this__  struct _itk_layout_manager* this

/**
 * Component layout manager
 */
typedef struct _itk_layout_manager
{
  /**
   * Internal use data
   */
  void** data;
  
  
  /**
   * Prepare the layout manager for locating of multiple components, probably all of them
   */
  void (*prepare)(__this__);
  
  /**
   * End of `prepare` requirement
   */
  void (*done)(__this__);
  
  /**
   * Locates the positions of the corners of a component
   * 
   * @param   child  The child, to the component using the layout manager, of interest
   * @return         The rectangle the child is confound in
   */
  rectangle_t (*locate)(__this__, struct _itk_component* child);
  
  /**
   * Calculate the combined advisory minimum size of all components
   * 
   * @return  Advisory minimum size for the container
   */
  size2_t (*minimum_size)(__this__);
  
  /**
   * Calculate the combined preferred size of all components
   * 
   * @return  Rreferred size for the container
   */
  size2_t (*preferred_size)(__this__);
  
  /**
   * Calculate the combined advisory maximum size of all components
   * 
   * @return  Advisory maximum size for the container
   */
  size2_t (*maximum_size)(__this__);
  
} itk_layout_manager;

#undef __this__


#endif

