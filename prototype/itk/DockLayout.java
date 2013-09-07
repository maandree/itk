/**
 * itk – The Impressive Toolkit
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
package itk;

import java.awt.*;
import java.util.*;


/**
 * Docking lomponent layout manager
 */
public class DockLayout implements LayoutManager
{
    /**
     * Dock to left edge
     */
    public static final String LEFT = "left";
    
    /**
     * Dock to top edge
     */
    public static final String TOP = "top";
    
    /**
     * Dock to right edge
     */
    public static final String RIGHT = "right";
    
    /**
     * Dock to bottom edge
     */
    public static final String BOTTOM = "bottom";
    
    /**
     * Fill the centre
     */
    public static final String CENTER = "center";
    
    
    
    /**
     * Constructor
     * 
     * @param  container  The container which uses the layout manager
     */
    public DockLayout(final Component container)
    {
	this.container = container;
    }
    
    
    
    /**
     * The container which uses the layout manager
     */
    public final Component container;
    
    /**
     * The childrens locations
     */
    public HashMap<Component, Rectangle> prepared = null;
    
    
    
    /**
     * Creates a complexer layout constraint that yeilds for later docked components
     * 
     * @param   anticlockwise  The number of components for which to yeild, that are position at the edge the 90° anticlockwise position
     * @param   edge           The edge to which to dock
     * @param   clockwise      The number of components for which to yeild, that are position at the edge the 90° clockwise position
     * @return                 
     */
    public static String yeild(final int anticlockwise, final String edge, final int clockwise)
    {
	return Integer.toString(anticlockwise) + " " + edge + " " + Integer.toString(clockwise);
    }
    
    
    /**
     * Null out if area is zero
     * 
     * @param   area  The area
     * @return        The area, but {@code null} if the width or height is zero
     */
    private Rectangle nonzero(final Rectangle area)
    {
	return ((area.width == 0) || (area.height == 0)) ? null : area;
    }
    
    
    /**
     * Prepare the layout manager for locating of multiple components, probably all of them
     */
    public void prepare()
    {
	this.prepared = new HashMap<Component, Rectangle>();
	int x = 0, y = 0;
	int w = this.container.size.width;
	int h = this.container.size.height;
	
	final ArrayList<Component> yeildLeft   = new ArrayList<Component>();
	final ArrayList<Component> yeildTop    = new ArrayList<Component>();
	final ArrayList<Component> yeildRight  = new ArrayList<Component>();
	final ArrayList<Component> yeildBottom = new ArrayList<Component>();
	
	for (final Component child : this.container.children)
	{
	    final Object constraints = child.constraints;
	    if ((constraints != null) && (constraints instanceof String))
	    {
		final String c = (String)constraints;
		Rectangle r;
		if (c.contains(LEFT))
		{
		    int _ = Math.min(w, child.preferredSize.width), H = h, Y = y;
		    while ((yeildLeft.size() > 0) && (_ > 0))
		    {
			final Component yeilded = yeildLeft.remove(0);
			if ((r = this.prepared.get(yeilded)) != null)
			{
			    int yx = r.x, yy = r.y, yw = r.width, yh = r.height;
			    r = nonzero(new Rectangle(yx + _, yy, yw - _, yh));
			    this.prepared.put(yeilded, r);
			    H += yh;
			    if (yeilded.constraints.toString().contains(TOP))
				Y -= yh;
			}
		    }
		    this.prepared.put(child, r = nonzero(new Rectangle(x, Y, _, H)));
		    x += _;
		    w -= _;
		    if ((c.equals(LEFT) == false) && (r != null))
		    {
			final String[] words = c.split(" ");
			final int bottom = Integer.parseInt(words[0]);
			final int top = Integer.parseInt(words[2]);
			for (_ = 0; _ < bottom; _++)
			    yeildBottom.add(child);
			for (_ = 0; _ < top; _++)
			    yeildTop.add(child);
		    }
		    continue;
		}
		if (c.contains(TOP))
		{
		    int _ = Math.min(h, child.preferredSize.height), W = w, X = x;
		    while ((yeildTop.size() > 0) && (_ > 0))
		    {
			final Component yeilded = yeildTop.remove(0);
			if ((r = this.prepared.get(yeilded)) != null)
			{
			    int yx = r.x, yy = r.y, yw = r.width, yh = r.height;
			    r = nonzero(new Rectangle(yx, yy + _, yw, yh - _));
			    this.prepared.put(yeilded, r);
			    W += yw;
			    if (yeilded.constraints.toString().contains(LEFT))
				X -= yw;
			}
		    }
		    this.prepared.put(child, r = nonzero(new Rectangle(X, y, W, _)));
		    y += _;
		    h -= _;
		    if ((c.equals(TOP) == false) && (r != null))
		    {
			final String[] words = c.split(" ");
			final int left = Integer.parseInt(words[0]);
			final int right = Integer.parseInt(words[2]);
			for (_ = 0; _ < left; _++)
			    yeildLeft.add(child);
			for (_ = 0; _ < right; _++)
			    yeildRight.add(child);
		    }
		    continue;
		}
		if (c.contains(RIGHT))
		{
		    int _ = Math.min(w, child.preferredSize.width), H = h, Y = y;
		    while ((yeildRight.size() > 0) && (_ > 0))
		    {
			final Component yeilded = yeildRight.remove(0);
			if ((r = this.prepared.get(yeilded)) != null)
			{
			    int yx = r.x, yy = r.y, yw = r.width, yh = r.height;
			    r = nonzero(new Rectangle(yx, yy, yw - _, yh));
			    this.prepared.put(yeilded, r);
			    H += yh;
			    if (yeilded.constraints.toString().contains(TOP))
				Y -= yh;
			}
		    }
		    this.prepared.put(child, r = nonzero(new Rectangle(x + w - _, Y, _, H)));
		    w -= _;
		    if ((c.equals(RIGHT) == false) && (r != null))
		    {
			final String[] words = c.split(" ");
			final int top = Integer.parseInt(words[0]);
			final int bottom = Integer.parseInt(words[2]);
			for (_ = 0; _ < top; _++)
			    yeildTop.add(child);
			for (_ = 0; _ < bottom; _++)
			    yeildBottom.add(child);
		    }
		    continue;
		}
		if (c.contains(BOTTOM))
		{
		    int _ = Math.min(h, child.preferredSize.height), W = w, X = x;
		    while ((yeildBottom.size() > 0) && (_ > 0))
		    {
			final Component yeilded = yeildBottom.remove(0);
			if ((r = this.prepared.get(yeilded)) != null)
			{
			    int yx = r.x, yy = r.y, yw = r.width, yh = r.height;
			    r = nonzero(new Rectangle(yx, yy, yw, yh - _));
			    this.prepared.put(yeilded, r);
			    W += yw;
			    if (yeilded.constraints.toString().contains(LEFT))
				X -= yw;
			}
		    }
		    this.prepared.put(child, r = nonzero(new Rectangle(X, y + h - _, W, _)));
		    h -= _;
		    if ((c.equals(BOTTOM) == false) && (r != null))
		    {
			final String[] words = c.split(" ");
			final int right = Integer.parseInt(words[0]);
			final int left = Integer.parseInt(words[2]);
			for (_ = 0; _ < right; _++)
			    yeildRight.add(child);
			for (_ = 0; _ < left; _++)
			    yeildLeft.add(child);
		    }
		    continue;
		}
	    }
	    this.prepared.put(child, nonzero(new Rectangle(x, y, w, h)));
	    w = h = 0;
	}
    }
    
    /**
     * End of {@link #prepare()} requirement
     */
    public void done()
    {
	this.prepared = null;
    }
    
    
    /**
     * Locates the positions of the corners of a component
     * 
     * @param   child  The child, to the component using the layout manager, of interest
     * @return         The rectangle the child is confound in
     */
    public Rectangle locate(final Component child)
    {
	boolean p = this.prepared == null;
	Rectangle r;
	if (p)
	    this.prepare();
	r = this.prepared.get(child);
	child.size = r == null ? child.size : new Dimension(r.width, r.height);
	if (p)
	    this.done();
	return r;
    }
    
}

