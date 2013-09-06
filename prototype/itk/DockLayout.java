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
	
	for (final Component child : this.container.children)
	{
	    final Object constraints = child.constraints;
	    if ((constraints != null) && (constraints instanceof String))
	    {
		final String c = (String)constraints;
		if (c.equals(LEFT))
		{
		    int _ = Math.min(w, child.preferredSize.width);
		    this.prepared.put(child, nonzero(new Rectangle(x, y, _, h)));
		    x += _;
		    w -= _;
		    continue;
		}
		if (c.equals(TOP))
		{
		    int _ = Math.min(h, child.preferredSize.height);
		    this.prepared.put(child, nonzero(new Rectangle(x, y, w, _)));
		    y += _;
		    h -= _;
		    continue;
		}
		if (c.equals(RIGHT))
		{
		    int _ = Math.min(w, child.preferredSize.width);
		    this.prepared.put(child, nonzero(new Rectangle(x + w - _, y, _, h)));
		    w -= _;
		    continue;
		}
		if (c.equals(BOTTOM))
		{
		    int _ = Math.min(h, child.preferredSize.height);
		    this.prepared.put(child, nonzero(new Rectangle(x, y + h - _, w, _)));
		    h -= _;
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

