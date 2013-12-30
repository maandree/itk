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
#include "graphics.h"
#include "itkmacros.h"

#include <stdlib.h>


#define __this__  itk_graphics* this


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
static itk_graphics* create(__this__, rectangle_t area)
{
  itk_graphics* rc = this->fork(this);
  rc->clip(rc, area);
  rc->translate(rc, new_position2(-(area.x), -(area.y)));
  return rc;
}


/**
 * Draw a solid rectangle
 * 
 * @param  area  The rectangle to draw
 */
static void fill_rectangle(__this__, rectangle_t area)
{
  position2_t* points = alloca(4 * sizeof(position2_t));
  (points + 0)->x = area.x;
  (points + 0)->y = area.y;
  (points + 1)->x = area.x + area.width - 1;
  (points + 1)->y = area.y;
  (points + 2)->x = area.x + area.width - 1;
  (points + 2)->y = area.y + area.height - 1;
  (points + 3)->x = area.x;
  (points + 3)->y = area.y + area.height - 1;
  this->fill_polygon(this, points, 4, ITK_GRAPHICS_SHAPE_CONVEX, ITK_GRAPHICS_MODE_ABSOLUTE);
}


/**
 * Draw a solid rectangle with rounded corners
 * 
 * @param  area      The rectangle to draw
 * @param  arc_size  The size of the arc at the rounded corners
 */
static void fill_rounded_rectangle(__this__, rectangle_t area, size2_t arc_size)
{
  if ((arc_size.width <= 0) || (arc_size.height <= 0))
    {
      this->fill_rectangle(this, area);
      return;
    }
  
#define arc_rect(X, Y)  new_rectangle(area.x + X, area.y + Y, arc_size.width * 2, arc_size.height * 2)
#define right   (area.width - arc_size.width * 2)
#define bottom  (area.height - arc_size.height * 2)
  
  this->fill_pie(this, arc_rect(0,     0),       90.f, 90.f);
  this->fill_pie(this, arc_rect(0,     bottom), 180.f, 90.f);
  this->fill_pie(this, arc_rect(right, bottom), 270.f, 90.f);
  this->fill_pie(this, arc_rect(right, 0),        0.f, 90.f);
  
  this->fill_rectangle(this, new_rectangle(area.x, area.y + arc_size.height, area.width, bottom));
  this->fill_rectangle(this, new_rectangle(area.x + arc_size.width, area.y, right, arc_size.height));
  this->fill_rectangle(this, new_rectangle(area.x + arc_size.width, area.y + area.height - arc_size.height,
					   right, arc_size.height));
  
#undef bottom
#undef right
#undef arc_rect
}


/**
 * Draw a solid ellipse
 * 
 * @param  area  The rectangle the ellipse is scribed into
 */
static void fill_oval(__this__, rectangle_t area)
{
  this->fill_pie(this, area, 0.f, 360.f);
}


/**
 * Draw a hollow rectangle
 * 
 * @param  area  The rectangle to draw
 */
static void draw_rectangle(__this__, rectangle_t area)
{
  position2_t* points = alloca(4 * sizeof(position2_t));
  (points + 0)->x = area.x;
  (points + 0)->y = area.y;
  (points + 1)->x = area.x + area.width - 1;
  (points + 1)->y = area.y;
  (points + 2)->x = area.x + area.width - 1;
  (points + 2)->y = area.y + area.height - 1;
  (points + 3)->x = area.x;
  (points + 3)->y = area.y + area.height - 1;
  this->draw_polygon(this, points, 4, ITK_GRAPHICS_MODE_ABSOLUTE);
}


/**
 * Draw a hollow rectangle with rounded corners
 * 
 * @param  area      The rectangle to draw
 * @param  arc_size  The size of the arc at the rounded corners
 */
static void draw_rounded_rectangle(__this__, rectangle_t area, size2_t arc_size)
{
  if ((arc_size.width <= 0) || (arc_size.height <= 0))
    {
      this->draw_rectangle(this, area);
      return;
    }
  
#define arc_rect(X, Y)  new_rectangle(area.x + X, area.y + Y, arc_size.width * 2, arc_size.height * 2)
#define right   (area.width - arc_size.width * 2)
#define bottom  (area.height - arc_size.height * 2)
  
  this->draw_arc(this, arc_rect(0,     0),       90.f, 90.f);
  this->draw_arc(this, arc_rect(0,     bottom), 180.f, 90.f);
  this->draw_arc(this, arc_rect(right, bottom), 270.f, 90.f);
  this->draw_arc(this, arc_rect(right, 0),        0.f, 90.f);
  
  this->draw_rectangle(this, new_rectangle(area.x, area.y + arc_size.height, area.width, bottom));
  this->draw_rectangle(this, new_rectangle(area.x + arc_size.width, area.y, right, arc_size.height));
  this->draw_rectangle(this, new_rectangle(area.x + arc_size.width, area.y + area.height - arc_size.height,
					   right, arc_size.height));
  
#undef bottom
#undef right
#undef arc_rect
}


/**
 * Draw an automatically closed hollow polygon
 * 
 * @param  points       Array of points from which the polygon is constructed
 * @param  point_count  The number of elements in `points`
 * @param  shape        The shape of the polygon, this is used to improve performance
 * @param  mode         Whether the points are absolute or relative to the previous one
 */
static void draw_polygon(__this__, position2_t* points, long point_count, int8_t mode)
{
  position2_t* polyline_points = alloca((point_count + 1) * sizeof(position2_t));
  long i;
  for (i = 0; i < point_count; i++)
    {
      (polyline_points + i)->x = (points + i)->x;
      (polyline_points + i)->y = (points + i)->y;
    }
  (polyline_points + point_count)->x = points->x;
  (polyline_points + point_count)->y = points->y;
  this->draw_polyline(this, polyline_points, point_count + 1, mode);
}


/**
 * Draw a single line segment
 * 
 * @param  start  The start point of the line segment
 * @param  end    The end point of the line segment
 */
static void draw_line(__this__, position2_t start, position2_t end)
{
  this->draw_lines(this, &start, &end, 1);
}


/**
 * Draw a hollow ellipse
 * 
 * @param  area  The rectangle the ellipse is scribed into
 */
static void draw_oval(__this__, rectangle_t area)
{
  this->draw_arc(this, area, 0.f, 360.f);
}


/**
 * Draw a single point
 * 
 * @param  point  The position of the point
 */
static void draw_point(__this__, position2_t point)
{
  this->draw_polyline(this, &point, 1, 0);
}


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
void itk_graphics_derive_methods(__this__)
{
#define __(FUNC)  if (this->FUNC == NULL)  this->FUNC = FUNC
  __(create);
  __(fill_rectangle);
  __(fill_rounded_rectangle);
  __(fill_oval);
  __(draw_rectangle);
  __(draw_rounded_rectangle);
  __(draw_polygon);
  __(draw_line);
  __(draw_oval);
  __(draw_point);
#undef __
}

