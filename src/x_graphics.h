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
#ifndef __ITK_X_GRAPHICS_H__
#define __ITK_X_GRAPHICS_H__

#include "graphics.h"

#include <X11/Xlib.h>


/**
 * Internal use data for X graphics context
 */
typedef struct _itk_x_graphics_data
{
  /**
   * The X display, a connection to the X server
   */
  Display* display;
  
  /**
   * The component that is begin drawn on
   */
  Drawable drawable;
  
  /**
   * The native X graphics context
   */
  GC context;
  
  /**
   * The current clip area
   */
  rectangle_t clip_area;
  
  /**
   * Whether the current arc mode is arc chord
   */
  bool_t chord_mode;
  
} itk_x_graphics_data;


/**
 * Constructor
 * 
 * @param  display   The X display, a connection to the X server
 * @param  screen    The screen the component is located in
 * @param  drawable  The component that is begin drawn on
 */
itk_graphics* itk_new_x_graphics(Display* display, int screen, Drawable drawable);


#endif

