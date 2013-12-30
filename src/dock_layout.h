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
#ifndef __ITK_DOCK_LAYOUT_H__
#define __ITK_DOCK_LAYOUT_H__

#include "layout_manager.h"
#include "component.h"


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
itk_layout_manager* itk_new_dock_layout(itk_component* container);


/**
 * Destructor
 */
void itk_free_dock_layout(itk_layout_manager* this);


/**
 * Creates a complexer layout constraint that yeilds for later docked components
 * 
 * @param   anticlockwise  The number of components for which to yeild, that are position at the edge the 90° anticlockwise position
 * @param   edge           The edge to which to dock: "left", "top", "right" or "bottom"
 * @param   clockwise      The number of components for which to yeild, that are position at the edge the 90° clockwise position
 * @return                 The constraint to use, do not forget to free it when it is not in use anymore
 */
char* itk_dock_layout_yeild(long anticlockwise, char* edge, long clockwise);


/**
 * Creates contraint for a component to be placed at an absolute position
 * 
 * @param   position  The position of the component
 * @return            The constraint to use, do not forget to free it when it is not in use anymore
 */
char* itk_dock_layout_absolute(position2_t position);


#endif

