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
#ifndef __ITK_FLOW_LAYOUT_H__
#define __ITK_FLOW_LAYOUT_H__

#include "layout_manager.h"
#include "component.h"


/**
 * Components are added to the right side of the previous component and are alignmented to the left
 */
#define ALIGNMENT_LEFT  0

/**
 * Components are added to the right side of the previous component but are alignmented to the right
 */
#define ALIGNMENT_RIGHT  1

/**
 * Components are added to the right side of the previous component but are justified
 */
#define ALIGNMENT_JUSTIFY  2



/**
 * Constructor
 * 
 * @param  container  The container which uses the layout manager
 * @param  alignment  The alignment of the components
 * @param  hgap       The size of the horizontal gap between components
 * @param  vgap       The size of the vertical gap between components
 */
itk_layout_manager* itk_new_flow_layout(itk_component* container, int8_t alignment, dimension_t hgap, dimension_t vgap);


#endif

