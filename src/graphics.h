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
#ifndef __ITK_GRAPHICS_H__
#define __ITK_GRAPHICS_H__

#include "itktypes.h"


#define __this__  struct _itk_graphics* this

/**
 * Graphics context class
 */
typedef struct _itk_graphics
{
  struct _itk_graphics* (*create)(__this__, rectangle_t area);
  void (*clip)(__this__, rectangle_t area);
  void (*translate)(__this__, position2_t offset);
  void (*fill_rectangle)(__this__, rectangle_t area);
  void (*set_colour)(__this__, colour_t colour);
  void (*free)(__this__);
} itk_graphics;

#undef __this__


#endif

