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
 * Radio button component class
 */
public class RadioButton extends Component
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
    public RadioButton(final String name)
    {
	super(name);
    }
    
    
    
    /**
     * The component's foreground colour
     */
    public Color foregroundColour = DEFAULT_FOREGROUND_COLOUR;
    
    /**
       Whether the radio button is selected
     */
    public boolean value = true;
    
    
    
    /**
     * Repaint the component
     * 
     * @param  g  The object with which to paint
     */
    protected void paintComponent(final Graphics2D g)
    {
	int w = (Math.min(Math.min(this.size.width, this.size.height), 13) - 1) | 1;
	int x = (this.size.width - w) / 2;
	int y = (this.size.height - w) / 2;
	
	if (this.value)
	{
	    g.setColor(this.foregroundColour);
	    g.fillPolygon(new int[] { x - 0, x + w / 2, x + w, x + w / 2 },
			  new int[] { y + w / 2, y - 1, y + w / 2, y + w }, 4);
	}
	else
	{
	    g.setColor(this.backgroundColour);
	    g.fillPolygon(new int[] { x - 0, x + w / 2, x + w, x + w / 2 },
			  new int[] { y + w / 2, y - 1, y + w / 2, y + w }, 4);
	    g.setColor(this.foregroundColour);
	    g.drawPolygon(new int[] { x - 0, x + w / 2, x + w - 1, x + w / 2 },
			  new int[] { y + w / 2, y - 0, y + w / 2, y + w - 1}, 4);
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
	return new Dimension(13, 13);
    }
    
}

