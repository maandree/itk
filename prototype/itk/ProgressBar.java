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
import java.awt.image.BufferedImage;


/**
 * ProgressBar component class
 */
public class ProgressBar extends Component
{
    /**
     * The default foreground colour
     */
    private static final Color DEFAULT_FOREGROUND_COLOUR = Color.BLACK;
    
    /**
     * The default progress colour
     */
    private static final Color DEFAULT_PROGRESS_COLOUR = new Color(0, 0, 188);
    
    /**
     * The default OK colour
     */
    private static final Color DEFAULT_OK_COLOUR = new Color(0, 188, 0);
    
    /**
     * The default warning colour
     */
    private static final Color DEFAULT_WARNING_COLOUR = new Color(188, 188, 0);
    
    /**
     * The default fatal warning colour
     */
    private static final Color DEFAULT_FATAL_COLOUR = new Color(188, 0, 0);
    
    /**
     * Left to right progressing progress bar
     */
    private static final int LEFT_TO_RIGHT = 0;
    
    /**
     * Right to left progressing progress bar
     */
    private static final int RIGHT_TO_LEFT = 1;
    
    /**
     * Top to bottom progressing progress bar
     */
    private static final int TOP_DOWN = 2;
    
    /**
     * Bottom to top progressing progress bar
     */
    private static final int BOTTOM_UP = 3;
    
    /**
     * Prefer to floating progress of integer progress
     */
    private static final int CONTINUOUS = 4;
    
    /**
     * Prefer blocks
     */
    private static final int BLOCKS = 8;
    
    
    
    /**
     * Constructor
     * 
     * @param  name  The name of the component
     */
    public ProgressBar(final String name)
    {
	super(name);
    }
    
    
    
    /**
     * The progress bar's text
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
     * The orientation of each progress
     */
    public int[] orientation = { LEFT_TO_RIGHT };
    
    /**
     * The maxmimum value of each progress
     */
    public int[] maximum = { 100 };
    
    /**
     * The minimum value of each progress.
     * Those with higher than maximum will have an indetermine progress
     */
    public int[] minimum = { 0 };
    
    /**
     * The current value of each progress
     */
    public int[] value = { 0 };
    
    /**
     * The colour of each progress
     */
    public Color[] colour = { DEFAULT_PROGRESS_COLOUR };
    
    /**
     * Preferred number of blocks, assuming blocks are used, negative
     * for block size rather then block count, and zero for automatic
     */
    public int[] blocks = { -8 };
    
    
    
    /**
     * Repaint the component
     * 
     * @param  g  The object with which to paint
     */
    @Override
    protected void paintComponent(final Graphics2D g)
    {
	int x = this.size.width - 1;
	int y = this.size.height - 1;
	
	g.setColor(this.backgroundColour);
	g.fillRect(0, 0, x + 1, y + 1);
	
	g.setColor(this.backgroundColour.darker());
	g.drawLine(0, 0, x, 0);
	g.drawLine(0, 0, 0, y);
	
	g.setColor(this.backgroundColour.brighter());
	g.drawLine(x, y, 1, y);
	g.drawLine(x, y, x, 1);
	
	final int count = Math.min(Math.min(Math.min(this.orientation.length,
						     this.maximum.length),
					    Math.min(this.minimum.length,
						     this.value.length)),
				   Math.min(this.colour.length,
					    this.blocks.length));
	
	/* XXX if there are exactly 2 value, one can be horizontal and one can be vertical */
	for (int i = 0, w = x - 1, h = y - 1; i < count; i++)
	{
	    final int min, max, val, orient, block;
	    min = this.minimum[i];
	    max = this.maximum[i];
	    val = this.value[i];
	    orient = this.orientation[i];
	    block = this.blocks[i];
	    
	    /* XXX support multiple values */
	    if (max > min)
	    {
		g.setColor(this.colour[i]);
		/* XXX support blocks */
		final int v;
		if ((orient & 3) <= RIGHT_TO_LEFT)
		    v = Math.min(w * val / (max - min), w);
		else
		    v = Math.min(h * val / (max - min), h);
		if (v > 0)
		    if ((orient & 3) == LEFT_TO_RIGHT)
			g.fillRect(1, 1, v, h);
		    else if ((orient & 3) == RIGHT_TO_LEFT)
			g.fillRect(1 + w - v, 1, v, h);
		    else if ((orient & 3) == TOP_DOWN)
			g.fillRect(1, 1, w, v);
		    else if ((orient & 3) == BOTTOM_UP)
			g.fillRect(1, 1 + h - v, w, v);
	    }
	    else if (max < min)
	    {
		BufferedImage img = new BufferedImage(w, h, BufferedImage.TYPE_INT_ARGB);
		Graphics2D gg = img.createGraphics();
		gg.setColor(this.colour[i]);
		gg.fillRect(0, 0, 2, 2);
		gg.fillRect(2, 2, 2, 2);
		for (int y_ = 4; y_ < h; y_ <<= 1)
		    gg.drawImage(img, 0, y_, 4, y_ << 1, 0, 0, 4, y_, null);
		for (int x_ = 4; x_ < w; x_ <<= 1)
		    gg.drawImage(img, x_, 0, x_ << 1, h, 0, 0, x_, h, null);
		g.drawImage(img, 1, 1, null);
		gg.dispose();
	    }
	}
	
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
    
}

