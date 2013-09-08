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
 * Check box component class
 */
public class CheckBox extends Component
{
    /**
     * The default background colour
     */
    private static final Color DEFAULT_BACKGROUND_COLOUR = new Color(0, 0, 0, 0);
    
    /**
     * The default box background colour
     */
    private static final Color DEFAULT_BOX_BACKGROUND_COLOUR = Color.WHITE;
    
    /**
     * The default foreground colour
     */
    private static final Color DEFAULT_FOREGROUND_COLOUR = Color.BLACK;
    
    
    
    /**
     * Constructor
     * 
     * @param  name  The name of the component
     */
    public CheckBox(final String name)
    {
	super(name);
	this.backgroundColour = DEFAULT_BACKGROUND_COLOUR;
    }
    
    
    
    /**
     * The component's foreground colour
     */
    public Color foregroundColour = DEFAULT_FOREGROUND_COLOUR;
    
    /**
     * The component's box's background colour
     */
    public Color boxBackgroundColour = DEFAULT_BOX_BACKGROUND_COLOUR;
    
    /**
       Whether the check box is checked
     */
    public Boolean value = Boolean.FALSE;
    
    
    
    /**
     * Repaint the component
     * 
     * @param  g  The object with which to paint
     */
    @Override
    protected void paintComponent(final Graphics2D g)
    {
	g.setColor(this.backgroundColour);
	g.fillRect(0, 0, this.size.width, this.size.height);
	
	int w = Math.min(Math.min(this.size.width, this.size.height), 13);
	int x = (this.size.width - w) / 2;
	int y = (this.size.height - w) / 2;
	
	g.setColor(this.foregroundColour);
	if (this.value == null)
	{
	    g.fillRect(x, y, w, w);
	    final int re, gr, bl;
	    re = this.boxBackgroundColour.getRed();
	    gr = this.boxBackgroundColour.getGreen();
	    bl = this.boxBackgroundColour.getBlue();
	    g.setColor(new Color(re, gr, bl, 128));
	    if (w > 3)
		g.fillRect(x + 1, y + 1, w - 2, w - 2);
	    else
		g.fillRect(x, y, w, w);
	}
	else if (this.value.booleanValue())
	    g.fillRect(x, y, w, w);
	else
	{
	    g.drawRect(x, y, w - 1, w - 1);
	    if (w > 3)
	    {
		g.setColor(this.boxBackgroundColour);
		g.fillRect(x + 1, y + 1, w - 2, w - 2);
	    }
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

