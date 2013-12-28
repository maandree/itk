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
#ifndef __ITK_ITKTYPES_H__
#define __ITK_ITKTYPES_H__

#include <inttypes.h>


/**
 * Booleanic data type
 */
typedef char bool_t;


/**
 * Union for raw sRGB colours with an alpha component
 */
typedef union _argb_colour_t
{
  /**
   * The colour as one single value
   */
  uint32_t value;
  
  /**
   * Colour component by name
   */
  struct
  {
    /**
     * The intensity of the blue subpixel of the colour
     */
    uint8_t blue;
    
    /**
     * The intensity of the green subpixel of the colour
     */
    uint8_t green;
    
    /**
     * The intensity of the red subpixel of the colour
     */
    uint8_t red;
    
    /**
     * The opacity of the colour
     */
    uint8_t alpha;
    
  } c;
  
  /**
   * Colour component by index
   */
  uint8_t by_index[4];
  
} argb_colour_t;


/**
 * Structure for colours that can be either raw colours or system colours
 */
typedef struct _colour_t
{
  /**
   * Name of the colour, `NULL` if the colours is a raw colour
   */
  char* system_colour;
  
  /**
   * The colour's sRBG value
   */
  argb_colour_t argb_colour;
  
} colour_t;


/**
 * One-dimensional size type
 */
typedef int32_t dimension_t;


/**
 * Two-dimensional size structure
 */
typedef struct _size2_t
{
  /**
   * The width component of the size
   */
  dimension_t width;
  
  /**
   * The height component of the size
   */
  dimension_t height;
  
} size2_t;


/**
 * One-dimensional position type
 */
typedef int32_t position_t;


/**
 * Rectangle structure
 */
typedef struct _position2_t
{
  /**
   * The position on the horizontal axis
   */
  position_t x;
  
  /**
   * The position on the vertical axis
   */
  position_t y;
  
} position2_t;


/**
 * Rectangle structure
 */
typedef struct _rectangle_t
{
  /**
   * The position on the horizontal axis
   */
  position_t x;
  
  /**
   * The position on the vertical axis
   */
  position_t y;
  
  /**
   * The width component of the size
   */
  dimension_t width;
  
  /**
   * The height component of the size
   */
  dimension_t height;
  
} rectangle_t;


#endif

