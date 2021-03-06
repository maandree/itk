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
 * Label component class
 */
public class Label extends Component
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
    public Label(final String name)
    {
	super(name);
    }
    
    
    
    /**
     * The label's text
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
     * Repaint the component
     * 
     * @param  g  The object with which to paint
     */
    @Override
    protected void paintComponent(final Graphics2D g)
    {
	g.setColor(this.backgroundColour);
	g.fillRect(0, 0, this.size.width, this.size.height);
	
	if (this.text != null)
	{
	    g.setFont(this.font);
	    g.setColor(this.foregroundColour);
	    final FontMetrics metrics = g.getFontMetrics();
	    final String[] lines = this.text.split("\n");
	    int index = 0;
	    for (final String line : lines)
	    {
		int x = (this.size.width - metrics.stringWidth(line)) / 2;
		int y = (this.size.height - metrics.getHeight() * lines.length) / 2;
		g.drawString(line, x, y + metrics.getAscent() + index * metrics.getHeight());
		index++;
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
	/* Dummy calculation! */
	final String[] lines = this.text.split("\n");
	int maxlen = 0;
	for (final String line : lines)
	    if (maxlen < line.length())
		maxlen = line.length();
	return new Dimension(6 * maxlen + 8, 16 * lines.length + 8);
    }
    
    
    /**
     * Make a silver ratioed size at least as big as a specified size
     * 
     * @param   mininum  The minimum dimensions
     * @return           Silver ratio dimensions
     */
    public static Dimension silverRatio(final Dimension minimum)
    {
	final boolean vertical = minimum.height > minimum.width;
	
	final int mx = vertical ? minimum.height : minimum.width;
	final int my = vertical ? minimum.width : minimum.height;
	
	final double mr = mx / my;
	int n = (int)mr;
	
	if ((mr * 2 - n) * (mr * 2 - n) > n * n + 4)
	    n++;
	
	double r, x_;
	int x;
	do
	{
	    r = (n + Math.sqrt(n * n + 4)) / 2;
	    x_ = my * r;
	    x = (int)(x_ + 0.5);
	    n++;
	}
	  while (x < mx);
	
	return new Dimension(vertical ? my : x, vertical ? x : my);
    }
    
}

