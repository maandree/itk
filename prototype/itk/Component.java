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
    public Dimension minimumSize;
    
    /**
     * The component's size
     */
    public Dimension size;
    
    /**
     * The component's maximum size
     */
    public Dimension maximumSize;
    
    
    
    /**
     * Repaint the window
     * 
     * @parma  g  The object with which to paint
     */
    protected void paint(final Graphics2D g)
    {
	g.setColor(this.backgroundColour);
	g.fillRect(0, 0, this.size.width, this.size.height);
    }
    
}

