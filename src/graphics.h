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


/**
 * Paths may self-intersect
 */
#define ITK_GRAPHICS_SHAPE_COMPLEX 0

/**
 * Non-self-intersecting convex shape
 */
#define ITK_GRAPHICS_SHAPE_CONVEX 1

/**
 * Non-self-intersecting non-convex shape
 */
#define ITK_GRAPHICS_SHAPE_NONCONVEX 2


/**
 * Coordinates are relative to the origin
 */
#define ITK_GRAPHICS_MODE_ABSOLUTE 0

/**
 * Coordinates are relative to the provious coordinate
 * with the first coordinate being relative to the origin
 */
#define ITK_GRAPHICS_MODE_RELATIVE 1



#define __this__  struct _itk_graphics* this

/**
 * Graphics context class
 */
typedef struct _itk_graphics
{
  /**
   * Internal use data for implementations
   */
  void* data;
  
  
  /**
   * Fork the graphics context but clip to a subset of the affected area.
   * The new context will be translate so that the top left corner of the
   * clip area becomes the new origin.
   * 
   * This is equivalent to:
   *     itk_graphics* rc = this->fork(this);
   *     rc->clip(rc, area);
   *     rc->translate(rc, new_position2(-(area.x), -(area.y)));
   *     return rc;
   * 
   * @param   area  The new clip area
   * @return        The new graphics context
   */
  struct _itk_graphics* (*create)(__this__, rectangle_t area);
  
  /**
   * Clip the affected area
   * 
   * @param  area  The new only area is affected by usage of this
   *               graphics context. The effective area is the
   *               intersection area and the old clip area.
   */
  void (*clip)(__this__, rectangle_t area);
  
  /**
   * Translate origin to `offset`
   * 
   * @param  offset  The new position of the old origin
   */
  void (*translate)(__this__, position2_t offset);
  
  
  /**
   * Set this graphics context's current drawing colour
   */
  void (*set_colour)(__this__, colour_t colour);
  
  
  void (*fill_rectangle)(__this__, rectangle_t area);
  void (*fill_rounded_rectangle)(__this__, rectangle_t area, size2_t arc_size);
  
  /**
   * Draw an automatically closed solid polygon
   * 
   * @param  points       Array of points from which the polygon is constructed
   * @param  point_count  The number of elements in `points`
   * @param  shape        The shape of the polygon, this is used to improve performance
   * @param  mode         Whether the points are absolute or relative to the previous one
   */
  void (*fill_polygon)(__this__, position2_t* points, long point_count, int8_t shape, int8_t mode);
  
  /**
   * Draw solid pie slice
   * 
   * @param  area         The rectangle the sliced circles is scribed into
   * @param  start_angle  The start of the arc, the number of degrees, anti-clockwise
   *                      from the three-o'clock position.
   * @param  arc_angles   The number of degrees between the arc start and arc end.
   *                      The magnitude if this value is truncated to 360. If it is
   *                      negative, the arc is drawn clockwise, otherwise it is drawn
   *                      anti-clockwise.
   */
  void (*fill_arc)(__this__, rectangle_t area, float start_angle, float arc_angles);
  void (*fill_oval)(__this__, rectangle_t area);
  
  
  void (*draw_rectangle)(__this__, rectangle_t area);
  void (*draw_rounded_rectangle)(__this__, rectangle_t area, size2_t arc_size);
  void (*draw_polygon)(__this__, position2_t* points, long point_count, int8_t mode);
  
  /**
   * Draw a polyline, an unclosed polygon
   * 
   * @param  points       Array of points from which the polygline is constructed
   * @param  point_count  The number of elements in `points`, if 1, then a point is drawn
   * @param  mode         Whether the points are absolute or relative to the previous one
   */
  void (*draw_polyline)(__this__, position2_t* points, long point_count, int8_t mode);
  void (*draw_line)(__this__, position2_t start, position2_t end);
  
  /**
   * Draw many line segments
   * 
   * @param  starts  The start point of each line segment
   * @param  ends    The end point of each line segment
   * @param  lines   The number of line segments to draw
   */
  void (*draw_lines)(__this__, position2_t* starts, position2_t* ends, long lines);
  
  /**
   * Draw an arc
   * 
   * @param  area         The rectangle the sliced circles is scribed into
   * @param  start_angle  The start of the arc, the number of degrees, anti-clockwise
   *                      from the three-o'clock position.
   * @param  arc_angles   The number of degrees between the arc start and arc end.
   *                      The magnitude if this value is truncated to 360. If it is
   *                      negative, the arc is drawn clockwise, otherwise it is drawn
   *                      anti-clockwise.
   */
  void (*draw_arc)(__this__, rectangle_t area, float start_angle, float arc_angles);
  void (*draw_oval)(__this__, rectangle_t area);
  void (*draw_point)(__this__, position2_t point);
  void (*draw_string)(__this__, position2_t point, char* text);
  
  
  /**
   * Destructor
   */
  void (*free)(__this__);
  
  /**
   * Create a duplicate of this graphics context
   */
  struct _itk_graphics* (*fork)(__this__);
  
} itk_graphics;

/**
 * This function is intended to be used by
 * implementations of this interface. This
 * function will set the functions that
 * can be derived from other functions,
 * this includes:
 *     • create
 *     • fill_rectangle
 *     • fill_rounded_rectangle
 *     • fill_oval
 *     • draw_rectangle
 *     • draw_rounded_rectangle
 *     • draw_polygon
 *     • draw_line
 *     • draw_oval
 *     • draw_point
 * 
 * The graphics context should be zero-initalised
 * because this function will not override
 * defined functions.
 * 
 * For increased performance, the implementer
 * should implement as many of these functions
 * as possible with natively provided functions,
 * and use this function only to implement those
 * that need to use other functions.
 * Derived functions cannot guarantee quality,
 * so some derivable functions may be need to
 * be implemented.
 */
void itk_graphics_derive_methods(__this__);

#undef __this__


#endif

