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
 * Separator component class
 */
public class Separator extends Component
{
    /**
     * The default foreground colour
     */
    private static final Color DEFAULT_FOREGROUND_COLOUR = Color.BLACK;
    
    
    /**
     * Vertical sparation, will form a horizontal line with default theme
     */
    public static final int VERTICAL = 0;
    
    /**
     * Horizontal sparation, will form a vertical line with default theme
     */
    public static final int HORIZONTAL = 1;
    
    
    
    /**
     * Constructor
     * 
     * @param  name  The name of the component
     */
    public Separator(final String name)
    {
	super(name);
    }
    
    
    
    /**
     * The component's foreground colour
     */
    public Color foregroundColour = DEFAULT_FOREGROUND_COLOUR;
    
    /**
     * The component's orientation
     */
    public int orientation = VERTICAL;
    
    
    
    /**
     * Repaint the component
     * 
     * @param  g  The object with which to paint
     */
    protected void paintComponent(final Graphics2D g)
    {
	int x = this.size.width;
	int y = this.size.height;
	
	g.setColor(this.foregroundColour);
	if (this.orientation == VERTICAL)
	    g.drawLine(0, y >> 1, x - 1, y >> 1);
	else
	    g.drawLine(x >> 1, 0, x >> 1, y - 1);
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
	return new Dimension(7, 7);
    }
    
}

