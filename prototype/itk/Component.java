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
import java.awt.image.BufferedImage;


/**
 * The root component class
 */
public class Component
{
    /**
     * The default background colour
     */
    private static final Color DEFAULT_BACKGROUND_COLOUR = new Color(211, 211, 211);
    
    
    
    /**
     * Constructor
     * 
     * @param  name  The name of the component
     */
    public Component(final String name)
    {
	this.name = name;
    }
    
    
    
    /**
     * The name of the component
     */
    public final String name;
    
    /**
     * The component's background colour
     */
    public Color backgroundColour = DEFAULT_BACKGROUND_COLOUR;
    
    /**
     * The component's advisory minimum size
     */
    public Dimension minimumSize = null;
    
    /**
     * The component's preferred size
     */
    public Dimension preferredSize = new Dimension(16, 16);
    
    /**
     * The component's size
     */
    public Dimension size = this.preferredSize;
    
    /**
     * The component's advisory maximum size
     */
    public Dimension maximumSize = null;
    
    /**
     * The components's parent
     */
    public Component parent = null;
    
    /**
     * The component's childred
     */
    public ArrayList<Component> children = new ArrayList<Component>();
    
    /**
     * Layout constraints
     */
    public Object constraints = null;
    
    /**
     * The component's layout manager
     */
    public LayoutManager layoutManager = null;
    
    /**
     * Buffer for double buffer
     */
    public volatile BufferedImage bufferImage = null;
    
    /**
     * Whether the component is double buffered
     */
    public boolean isDoubleBuffered = false;
    
    
    
    /**
     * Repaint the component and its childred
     * 
     * @param  g  The object with which to paint
     */
    public void paint(final Graphics2D g)
    {
	final int w = this.size.width, h = this.size.height;
	
	final boolean doubleBuffered = this.isDoubleBuffered;
	BufferedImage img = null;
	Graphics2D gg = g;
	if (doubleBuffered)
	{
	    img = bufferImage;
	    if ((img == null) || (img.getWidth() < w) || (img.getHeight() < h)) /* XXX new also of component shrunk considerably */
		this.bufferImage = img = new BufferedImage(w, h, BufferedImage.TYPE_INT_ARGB);
	}
	
	this.paintComponent(gg);
	this.paintChildren(gg);
	
	if (doubleBuffered)
	    g.drawImage(img, 0, 0, null);
    }
    
    /**
     * Repaint the component
     * 
     * @param  g  The object with which to paint
     */
    protected void paintComponent(final Graphics2D g)
    {
	g.setColor(this.backgroundColour);
	g.fillRect(0, 0, this.size.width, this.size.height);
    }
    
    /**
     * Repaint the component's children
     * 
     * @param  g  The object with which to paint
     */
    protected void paintChildren(final Graphics2D g)
    {
	if (this.layoutManager != null)
	    this.layoutManager.prepare();
	try
	{
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
	}
	finally
	{
	    if (this.layoutManager != null)
		this.layoutManager.done();
	}
    }
    
    
    /**
     * Synchronises the graphics
     */
    public void sync()
    {
	this.sync((Rectangle)null);
    }
    
    /**
     * Synchronises the graphics
     * 
     * @param  area  Area to synchronise, {@code null} for everything
     */
    public void sync(final Rectangle area)
    {
	final Graphics2D g;
	if (this.parent != null)
	{
	    if (this.backgroundColour.getAlpha() != 255)
	    {
		final Rectangle inParent = this.parent.locateChild(this);
		if (inParent != null)
		    if (area == null)
			this.parent.sync(inParent);
		    else
		    {
			final int x, y, w, h;
			x = area.x + inParent.x;
			y = area.y + inParent.y;
			w = area.width;
			h = area.height;
			this.parent.sync(new Rectangle(x, y, w, h));
		    }
	    }
	    else
		if ((g = this.parent.sync(this)) != null)
		{
		    if (area != null)
			g.clip(area);
		    this.paint(g);
		}
	}
    }
    
    /**
     * Synchronises the graphics on a child
     * 
     * @param   child  The child
     * @return         The object with which to paint
     */
    protected Graphics2D sync(final Component child)
    {
	if (this.parent == null)
	    return null;
	
	final Graphics2D g = this.parent.sync(this);
	if (g == null)
	    return null;
	
	final Rectangle rect = this.locateChild(child);
	if (rect == null)
	    return null;
	g.clip(rect);
	g.translate(-rect.x, -rect.y);
	return g;
    }
    
    
    /**
     * Locates the positions of the corners of a child
     * 
     * @param   child  The child
     * @return         The rectangle the child is confound in
     */
    public Rectangle locateChild(final Component child)
    {
	if (this.layoutManager != null)
	    return this.layoutManager.locate(child);
	
	int x = 0, y = 0;
	final int w = child.preferredSize.width, h = child.preferredSize.height;
	if ((child.constraints != null) && (child.constraints instanceof Point))
	{
	    x = ((Point)(child.constraints)).x;
	    y = ((Point)(child.constraints)).y;
	}
	
	child.size = child.preferredSize;
	return new Rectangle(x, y, w, h);
    }
    
    
    /**
     * {@inheritDoc}
     */
    @Override
    public String toString()
    {
	return this.name;
    }
    
}

