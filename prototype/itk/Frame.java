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


/**
 * Frame component class
 */
public class Frame extends Component
{
    /**
     * The default foreground colour
     */
    private static final Color DEFAULT_FOREGROUND_COLOUR = Color.BLACK;
    
    
    
    /**
     * Constructor
     * 
     * @param  name  The name of the component
     */
    public Frame(final String name)
    {
	super(name);
	(this.container = new Component("Frame container for " + name)).parent = this;
	(this.label = new Component("Frame label for " + name)).parent = this;
	this.layoutManager = new LayoutManager()
	    {
		public void prepare()
		{
		    /* do nothing */
		}
		public void done()
		{
		    /* do nothing */
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
		
		public Rectangle locate(final Component child)
		{
		    Rectangle rc;
		    if (child == null)
			return null;
		    else if (child == Frame.this.label)
		    {
			child.size = child.preferredSize;
			rc = new Rectangle(7, 0, child.size.width, child.size.height);
		    }
		    else if (child == Frame.this.container)
		    {
			final Rectangle r = this.locate(Frame.this.label);
			final int y = r == null ? 7 : r.height;
			final int w = Frame.this.size.width, h = Frame.this.size.height;
			rc = new Rectangle(7, y, w - 14, h - y - 7);
		    }
		    else
		    {
			if (Frame.this.secondaryLayoutManager != null)
			    return Frame.this.secondaryLayoutManager.locate(child);
			
			int x = 0, y = 0;
			final int w = child.preferredSize.width, h = child.preferredSize.height;
			if ((child.constraints != null) && (child.constraints instanceof Point))
			{
			    x = ((Point)(child.constraints)).x;
			    y = ((Point)(child.constraints)).y;
			}
			
			child.size = child.preferredSize;
			rc = new Rectangle(x, y, w, h);
		    }
		    if (((rc = nonzero(rc)) != null) && (child == Frame.this.container))
			child.size = new Dimension(rc.width, rc.height);
		    return rc;
		}
	    };
    }
    
    
    
    /**
     * The frame's label component
     */
    public Component label;
    
    /**
     * The frame's container for proper childern
     */
    public Component container;
    
    /**
     * The component's foreground colour
     */
    public Color foregroundColour = DEFAULT_FOREGROUND_COLOUR;
    
    /**
     * Layout manager for bad children, those that are not inside {@link #container}
     */
    public LayoutManager secondaryLayoutManager = null;
    
    
    
    /**
     * Repaint the component
     * 
     * @param  g  The object with which to paint
     */
    @Override
    protected void paintComponent(final Graphics2D g)
    {
	final int x = this.size.width - 1;
	final int y = this.size.height - 1;
	
	final Rectangle r = this.locateChild(this.label);
	
	g.setColor(this.backgroundColour);
	g.fillRect(0, 0, x + 1, y + 1);
	
	g.setColor(this.foregroundColour);
	if (r == null)
	    g.drawRect(3, 3, x - 6, y - 6);
	else
	{
	    final int x1 = 3, y1 = r.height >> 1;
	    final int x2 = x - 3, y2 = y - 3;
	    g.drawLine(x1, y1 + 1, x1, y2);
	    g.drawLine(x1 + 1, y2, x2 - 1, y2);
	    g.drawLine(x2, y2, x2, y1 + 1);
	    g.drawLine(x1, y1, r.x - 1, y1);
	    g.drawLine(r.x + r.width, y1, x2, y1);
	}
    }
    
    /**
     * Repaint the component's children
     * 
     * @param  g  The object with which to paint
     */
    @Override
    protected void paintChildren(final Graphics2D g)
    {
	if (this.layoutManager != null)
	    this.layoutManager.prepare();
	try
	{
	    final Component[] children_ = {this.label, this.container};
	    for (final Component child : this.children)
	    {
		final Rectangle rect = this.locateChild(child);
		if (rect != null)
		{
		    final int x = rect.x, width  = rect.width;
		    final int y = rect.y, height = rect.height;
		    child.paint((Graphics2D)(g.create(x, y, width, height)));
		}
	    }
	    for (final Component child : children_)
		if (child != null)
		{
		    final Rectangle rect = this.locateChild(child);
		    if (rect != null)
		    {
			final int x = rect.x, width  = rect.width;
			final int y = rect.y, height = rect.height;
			child.paint((Graphics2D)(g.create(x, y, width, height)));
		    }
		} 
	}
	finally
	{
	    if (this.layoutManager != null)
		this.layoutManager.done();
	}
    }
    
    
    /**
     * Calculate preferred size
     * 
     * @param   x  Extra width
     * @param   y  Extra height
     * @return     The preferred size
     */
    public Dimension calculateSize(final int x, final int y)
    {
	final Dimension rc_ = this.calculateSize();
	return new Dimension(rc_.width + x, rc_.height + y);
    }
    
    /**
     * Calculate preferred size
     * 
     * @return  The preferred size
     */
    public Dimension calculateSize()
    {
	int x = 0, y = 7;
	if (this.label != null)
	{
	    x = this.label.preferredSize.width;
	    y = this.label.preferredSize.height;
	}
	return new Dimension(x + 14, y + 7);
    }
    
}

