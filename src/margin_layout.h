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
#ifndef __ITK_MARGIN_LAYOUT_H__
#define __ITK_MARGIN_LAYOUT_H__

#include "layout_manager.h"
#include "component.h"


/**
 * margin_layout is a layout manager similar to stack_layout, but slightly
 * more powerful. The side of each margin, left, top, right and bottom, can
 * be configured, independently. The purpose of margin_layout is to make it
 * less obnoxious, and with lower overhead, to create void space between the
 * edges of the container and the component.
 */


/**
 * Constructor
 * 
 * @param  container  The container which uses the layout manager
 * @param  left       The size of the left margin
 * @param  top        The size of the top margin
 * @param  right      The size of the right margin
 * @param  bottom     The size of the bottom margin
 */
itk_layout_manager* itk_new_margin_layout(itk_component* container, dimension_t left, dimension_t top, dimension_t right, dimension_t bottom);


#endif

