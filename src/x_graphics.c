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
#include "x_graphics.h"

#include <stdlib.h>


#define DATA(this)  ((itk_x_graphics_data*)(this->data))


#define __this__  itk_graphics* this


/**
 * Clip the affected area
 * 
 * @param  area  The new only area is affected by usage of this
 *               graphics context. The effective area is the
 *               intersection area and the old clip area.
 */
static void clip(__this__, rectangle_t area)
{
}


/**
 * Translate origin to `offset`
 * 
 * @param  offset  The new position of the old origin
 */
static void translate(__this__, position2_t offset)
{
}


/**
 * Set this graphics context's current drawing colour
 */
static void set_colour(__this__, colour_t colour)
{
}


/**
 * Draw an automatically closed solid polygon
 * 
 * @param  points       Array of points from which the polygon is constructed
 * @param  point_count  The number of elements in `points`
 * @param  shape        The shape of the polygon, this is used to improve performance
 * @param  mode         Whether the points are absolute or relative to the previous one
 */
static void fill_polygon(__this__, position2_t* points, long point_count, int8_t shape, int8_t mode)
{
  XPoint* x_points = alloca(point_count * sizeof(XPoint));
  long i;
  
  for (i = 0; i < point_count; i++)
    {
      (x_points + i)->x = (points + i)->x;
      (x_points + i)->y = (points + i)->y;
    }
  
  XFillPolygon(DATA(this)->display,
	       DATA(this)->drawable,
	       DATA(this)->context,
	       x_points, point_count,
	       shape == ITK_GRAPHICS_SHAPE_CONVEX
	         ? Convex : shape == ITK_GRAPHICS_SHAPE_NONCONVEX
	         ? Nonconvex
	         : Complex,
	       mode == ITK_GRAPHICS_MODE_RELATIVE ? CoordModePrevious : CoordModeOrigin);
}


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
static void fill_arc(__this__, rectangle_t area, float start_angle, float arc_angles)
{
  XFillArc(DATA(this)->display,
	   DATA(this)->drawable,
	   DATA(this)->context,
	   area.x, area.y, area.width, area.height,
	   (int)(start_angle * 64 + 0.5), (int)(arc_angles * 64 + 0.5));
}


/**
 * Draw a polyline, an unclosed polygon
 * 
 * @param  points       Array of points from which the polygline is constructed
 * @param  point_count  The number of elements in `points`, if 1, then a point is drawn
 * @param  mode         Whether the points are absolute or relative to the previous one
 */
static void draw_polyline(__this__, position2_t* points, long point_count, int8_t mode)
{
  if (point_count == 1)
    {
      XDrawPoint(DATA(this)->display,
		 DATA(this)->drawable,
		 DATA(this)->context,
		 points->x, points->y);
    }
  else if (point_count > 1)
    {
      XPoint* x_points = alloca(point_count * sizeof(XPoint));
      long i;
  
      for (i = 0; i < point_count; i++)
	{
	  (x_points + i)->x = (points + i)->x;
	  (x_points + i)->y = (points + i)->y;
	}
      
      XDrawLines(DATA(this)->display,
                 DATA(this)->drawable,
                 DATA(this)->context,
		 x_points, point_count,
		 mode == ITK_GRAPHICS_MODE_RELATIVE ? CoordModePrevious : CoordModeOrigin);
    }
}


/**
 * Draw many line segments
 * 
 * @param  starts  The start point of each line segment
 * @param  ends    The end point of each line segment
 * @param  lines   The number of line segments to draw
 */
static void draw_lines(__this__, position2_t* starts, position2_t* ends, long lines)
{
  XSegment* x_segments = alloca(lines * sizeof(XSegment));
  long i;
  
  for (i = 0; i < lines; i++)
    {
      (x_segments + i)->x1 = (starts + i)->x;
      (x_segments + i)->y1 = (starts + i)->y;
      (x_segments + i)->x2 = (ends + i)->x;
      (x_segments + i)->y2 = (ends + i)->y;
    }
  
  XDrawSegments(DATA(this)->display,
		DATA(this)->drawable,
		DATA(this)->context,
		x_segments, lines);
}


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
static void draw_arc(__this__, rectangle_t area, float start_angle, float arc_angles)
{
  XDrawArc(DATA(this)->display,
	   DATA(this)->drawable,
	   DATA(this)->context,
	   area.x, area.y, area.width, area.height,
	   (int)(start_angle * 64 + 0.5), (int)(arc_angles * 64 + 0.5));
}


static void draw_string(__this__, position2_t point, char* text)
{
}


/**
 * Destructor
 */
static void free_xgc(__this__)
{
}


/**
 * Create a duplicate of this graphics context
 */
static itk_graphics* fork_xgc(__this__)
{
}

