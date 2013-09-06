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
     * The component's minimum size
     */
    public Dimension minimumSize = null;
    
    /**
     * The component's size
     */
    public Dimension size = new Dimension(16, 16);
    
    /**
     * The component's maximum size
     */
    public Dimension maximumSize = null;
    
    /**
     * The components's parent
     */
    public Component parent = null;
    
    
    
    /**
     * Repaint the component
     * 
     * @param  g  The object with which to paint
     */
    protected void paint(final Graphics2D g)
    {
	g.setColor(this.backgroundColour);
	g.fillRect(0, 0, this.size.width, this.size.height);
	
	this.printChildren(g);
    }
    
    /**
     * Repaint the component's children
     * 
     * @param  g  The object with which to paint
     */
    protected void printChildren(final Graphics2D g)
    {
    }
    
    
    /**
     * Synchronises the graphics
     */
    public void sync()
    {
	final Graphics2D g;
	if (this.parent != null)
	    if ((g = this.parent.sync(this)) != null)
		this.paint(g);
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
	
	final Rectable rect = this.locateChild(child);
	g.clip(rect);
	g.translate(-rect.x, -rect.y);
	return g;
    }
    
    
    /**
     * Locates the positions of the corners os a child
     * 
     * @param   child  The child
     * @return         The rectangle the child is confound in
     */
    public Rectangle locateChild(final Component child)
    {
	return new Rectangle(0, 0, this.size.width, this.size.height);
    }
    
}

