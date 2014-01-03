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
#ifndef __ITK_ITKMACROS_H__
#define __ITK_ITKMACROS_H__

#include "itktypes.h"



#ifndef NULL
/**
 * Null pointer
 */
#define NULL ((void*)0)
#endif

/**
 * False booleanic value
 */
#define false 0

/**
 * True booleanic value
 */
#define true 1


/**
 * Get a system colour
 * 
 * @param   colour_name  The name of the colour
 * @return               A colour described by the parameters
 */
static inline colour_t new_colour_by_name(char* colour_name)
{
  colour_t rc;
  rc.defined = true;
  rc.system_colour = colour_name;
  return rc;
}

/**
 * Create a new colour by sRGB value
 * 
 * @param   value  The raw value of the colour, the alpha component is kept
 * @return         A colour described by the parameters
 */
static inline colour_t new_colour_by_value(uint32_t value)
{
  colour_t rc;
  rc.defined = true;
  rc.system_colour = NULL;
  rc.argb_colour.value = value;
  return rc;
}

/**
 * Create a new colour by sRGB value
 * 
 * @param   value  The raw value of the colour, the alpha component is ignored
 * @return         A colour described by the parameters
 */
static inline colour_t new_colour_by_opaque_value(uint32_t value)
{
  colour_t rc = new_colour_by_value(value);
  rc.argb_colour.c.alpha = 255;
  return rc;
}

/**
 * Create a new colour by sRGB value
 * 
 * @param   alpha  The opacity of the colour
 * @param   red    The intensity of the red subpixel of the colour
 * @param   green  The intensity of the green subpixel of the colour
 * @param   blue   The intensity of the blue subpixel of the colour
 * @return         A colour described by the parameters
 */
static inline colour_t new_colour_by_argb(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue)
{
  colour_t rc;
  rc.defined = true;
  rc.system_colour = NULL;
  rc.argb_colour.c.alpha = alpha;
  rc.argb_colour.c.red = red;
  rc.argb_colour.c.green = green;
  rc.argb_colour.c.blue = blue;
  return rc;
}

/**
 * Create a new colour by sRGB value
 * 
 * @param   red    The intensity of the red subpixel of the colour
 * @param   green  The intensity of the green subpixel of the colour
 * @param   blue   The intensity of the blue subpixel of the colour
 * @return         A colour described by the parameters
 */
static inline colour_t new_colour_by_rgba(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
  return new_colour_by_argb(alpha, red, green, blue);
}

/**
 * Create a new colour by sRGB value
 * 
 * @param   red    The intensity of the red subpixel of the colour
 * @param   green  The intensity of the green subpixel of the colour
 * @param   blue   The intensity of the blue subpixel of the colour
 * @param   alpha  The opacity of the colour
 * @return         A colour described by the parameters
 */
static inline colour_t new_colour_by_rgb(uint8_t red, uint8_t green, uint8_t blue)
{
  return new_colour_by_argb(255, red, green, blue);
}


/**
 * A unbounded dimension value
 */
#define UNBOUNDED -1


/**
 * Create an one-dimensional size value
 * 
 * @param   size  The size, can be `UNBOUNDED`
 * @return        A value containing `size`
 */
static inline size1_t new_size1(dimension_t size)
{
  size1_t rc;
  rc.defined = true;
  rc.size = size;
  return rc;
}


/**
 * Create a two-dimensional size value
 * 
 * @param   width   The width, can be `UNBOUNDED`
 * @param   height  The height, can be `UNBOUNDED`
 * @return          A value containing both `width` and `height`
 */
static inline size2_t new_size2(dimension_t width, dimension_t height)
{
  size2_t rc;
  rc.defined = true;
  rc.width = width;
  rc.height = height;
  return rc;
}


/**
 * Create an one-dimensional position value
 * 
 * @param   position  The position
 * @return            A value containing both `position`
 */
static inline position1_t new_position1(position_t position)
{
  position1_t rc;
  rc.defined = true;
  rc.position = position;
  return rc;
}


/**
 * Create a two-dimensional position value
 * 
 * @param   x  The position on the horizontal axis
 * @param   y  The position on the vertical axis
 * @return     A value containing both `x` and `y`
 */
static inline position2_t new_position2(position_t x, position_t y)
{
  position2_t rc;
  rc.defined = true;
  rc.x = x;
  rc.y = y;
  return rc;
}


/**
 * Create a two-dimensional position value
 * 
 * @param   x       The position on the horizontal axis
 * @param   y       The position on the vertical axis
 * @param   width   The width
 * @param   height  The height
 * @return          A value containing the parameters
 */
static inline rectangle_t new_rectangle(position_t x, position_t y, dimension_t width, dimension_t height)
{
  rectangle_t rc;
  rc.defined = true;
  rc.x = x;
  rc.y = y;
  rc.width = width;
  rc.height = height;
  return rc;
}


#endif

