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
import javax.swing.*;


/**
 * Test class
 */
public class Test
{
    /**
     * Test main class
     * 
     * @param  args  Command line arguments
     */
    public static void main(final String... args)
    {
	SwingUtilities.invokeLater(new Runnable()
	    {
		@Override
		public void run()
		{
		    final Component blue = new Component("Blue component");
		    blue.backgroundColour = Color.BLUE;
		    blue.size = new Dimension(32, 32);
		    
		    final Component red = new Component("Red component");
		    red.backgroundColour = Color.RED;
		    red.constraints = new Point(8, 8);
		    
		    final Window window = new Window("Test window");
		    window.children.add(blue);
		    window.children.add(red);
		    window.setTitle("ITK test");
		    window.setVisible(true);
		}
	    });
    }
    
}

