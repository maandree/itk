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
#ifndef __ITK_STACK_LAYOUT_H__
#define __ITK_STACK_LAYOUT_H__

#include "layout_manager.h"
#include "component.h"


/**
 * stack_layout is a layout manager that puts components on top of eachother
 * filling the entire area of the component. The purpose of stack_layout is
 * to make it simple to to switch change component if viewed when something
 * happens.
 */


/**
 * Constructor
 * 
 * @param  container  The container which uses the layout manager
 */
itk_layout_manager* itk_new_stack_layout(itk_component* container);


#endif

