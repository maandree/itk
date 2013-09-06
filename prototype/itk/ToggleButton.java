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
 * Toggle (two state) button component class
 */
public class ToggleButton extends Component
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
    public ToggleButton(final String name)
    {
	super(name);
    }
    
    
    
    /**
     * The button's text
     */
    public String text = null;
    
    /**
     * The component's foreground colour
     */
    public Color foregroundColour = DEFAULT_FOREGROUND_COLOUR;
    
    /**
     * The component's font
     */
    public Font font = null;
    
    /**
     * Wether the button is pressed in
     */
    public boolean pressed = false;
    
    
    
    /**
     * Repaint the component
     * 
     * @param  g  The object with which to paint
     */
    protected void paint(final Graphics2D g)
    {
	int x = this.size.width - 1;
	int y = this.size.height - 1;
	
	g.setColor(this.backgroundColour);
	g.fillRect(0, 0, x + 1, y + 1);
	
	final Color bright = this.backgroundColour.brighter();
	final Color dark = this.backgroundColour.darker();
	
	g.setColor(this.pressed ? dark : bright);
	g.drawLine(0, 0, x, 0);
	g.drawLine(0, 0, 0, y);
	
	g.setColor(this.pressed ? bright : dark);
	g.drawLine(x, y, 1, y);
	g.drawLine(x, y, x, 1);
	
	if (this.text != null)
	{
	    g.setFont(this.font);
	    g.setColor(this.foregroundColour);
	    final FontMetrics metrics = g.getFontMetrics();
	    final String[] lines = this.text.split("\n");
	    int index = 0;
	    for (final String line : lines)
	    {
		x = (this.size.width - metrics.stringWidth(line)) / 2;
		y = (this.size.height - metrics.getHeight() * lines.length) / 2;
		g.drawString(line, x, y + metrics.getAscent() + index * metrics.getHeight());
		index++;
	    }
	}
	
	this.printChildren(g);
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
	/* Dummy calculation! */
	final String[] lines = this.text.split("\n");
	int maxlen = 0;
	for (final String line : lines)
	    if (maxlen < line.length())
		maxlen = line.length();
	return new Dimension(10 * maxlen + 4, 20 * lines.length + 4);
    }
    
}

