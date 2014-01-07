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
#ifndef __ITK_LINE_LAYOUT_H__
#define __ITK_LINE_LAYOUT_H__

#include "layout_manager.h"
#include "component.h"


/**
 * line_layout is a simple layout manager to components on a line after each
 * other without ever wrapping. The orientation of the components can be
 * configured to be either left to right, right to left, top down or bottom
 * up, with the alignments top left, top right, top left and bottom left,
 * respectively.
 */


/**
 * Components are added to the right side of the previous component
 */
#define ORIENTATION_LEFT_TO_RIGHT  0

/**
 * Components are added to the left side of the previous component
 */
#define ORIENTATION_RIGHT_TO_LEFT  1

/**
 * Components are added beneath the previous component
 */
#define ORIENTATION_TOP_DOWN       2

/**
 * Components are added on top of the previous component
 */
#define ORIENTATION_BOTTOM_UP      3



/**
 * Constructor
 * 
 * @param  container    The container which uses the layout manager
 * @param  orientation  In which direction are components added
 * @param  gap          The size of the gap between components
 */
itk_layout_manager* itk_new_line_layout(itk_component* container, int8_t orientation, dimension_t gap);


#endif

