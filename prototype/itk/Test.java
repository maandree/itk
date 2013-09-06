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
		    blue.backgroundColour = new Color(0, 0, 188);
		    blue.preferredSize = new Dimension(36, 36);
		    blue.constraints = DockLayout.TOP;
		    
		    final Component red = new Component("Red component");
		    red.backgroundColour = new Color(188, 0, 0);
		    red.preferredSize = new Dimension(16, 16);
		    red.constraints = DockLayout.BOTTOM;
		    
		    final Label label = new Label("Label component");
		    label.text = "Hello world";
		    label.backgroundColour = new Color(0, 188, 0);
		    label.foregroundColour = new Color(255, 67, 255);
		    label.preferredSize = label.calculateSize(16, 16);
		    label.constraints = DockLayout.TOP;
		    
		    final Button button = new Button("Centre component");
		    button.text = "Centre\nCentre";
		    button.backgroundColour = new Color(0, 188, 0);
		    button.foregroundColour = new Color(255, 67, 255);
		    button.constraints = DockLayout.CENTER;
		    
		    final Window window = new Window("Test window");
		    window.layoutManager = new DockLayout(window);
		    window.children.add(blue);
		    window.children.add(red);
		    window.children.add(label);
		    window.children.add(button);
		    blue.parent = window;
		    red.parent = window;
		    label.parent = window;
		    button.parent = window;
		    window.setTitle("ITK test");
		    window.setVisible(true);
		}
	    });
    }
    
}

