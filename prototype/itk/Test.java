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
		    final ProgressBar blue = new ProgressBar("Blue component");
		    /* blue.backgroundColour = new Color(0, 0, 188); */
		    blue.preferredSize = new Dimension(36, 36);
		    blue.constraints = DockLayout.TOP;
		    blue.fractionMode = ProgressBar.BLEND;
		    blue.value[0] = 75;
		    
		    final Separator vsep = new Separator("Vertical separator");
		    vsep.orientation = Separator.VERTICAL;
		    vsep.preferredSize = vsep.calculateSize(0, 0);
		    vsep.constraints = DockLayout.TOP;
		    
		    final Component red = new Component("Red component");
		    red.backgroundColour = new Color(188, 0, 0);
		    red.preferredSize = new Dimension(16, 16);
		    red.constraints = DockLayout.yeild(1, DockLayout.BOTTOM, 0);
		    
		    final ToggleButton toggle = new ToggleButton("Toggle button component");
		    toggle.text = "Toggle";
		    toggle.backgroundColour = new Color(188, 188, 0);
		    toggle.foregroundColour = new Color(67, 67, 255);
		    toggle.constraints = DockLayout.yeild(1, DockLayout.LEFT, 0);
		    toggle.preferredSize = toggle.calculateSize(0, 0);
		    toggle.pressed = true;
		    
		    final Separator hsep = new Separator("Horizontal separator");
		    hsep.orientation = Separator.HORIZONTAL;
		    hsep.preferredSize = hsep.calculateSize(0, 0);
		    hsep.constraints = DockLayout.LEFT;
		    
		    final Label label = new Label("Label component");
		    label.text = "Hello world";
		    label.backgroundColour = new Color(0, 188, 0);
		    label.foregroundColour = new Color(255, 67, 255);
		    label.preferredSize = label.calculateSize(8, 8);
		    label.constraints = DockLayout.TOP;
		    
		    final CheckBox cyan = new CheckBox("Cyan component");
		    cyan.boxBackgroundColour = new Color(0, 188, 188);
		    cyan.preferredSize = new Dimension(36, 36);
		    cyan.constraints = DockLayout.RIGHT;
		    cyan.value = null;
		    
		    
		    final Button ired1 = new Button("Inner red 1 button");
		    ired1.backgroundColour = new Color(188, 0, 0);
		    ired1.preferredSize = new Dimension(24, 24);
		    ired1.constraints = DockLayout.yeild(1, DockLayout.TOP, 1);
		    
		    final Button iyellow1 = new Button("Inner yellow 1 button");
		    iyellow1.backgroundColour = new Color(188, 188, 0);
		    iyellow1.preferredSize = new Dimension(24, 24);
		    iyellow1.constraints = DockLayout.RIGHT;
		    
		    final Button igreen1 = new Button("Inner green 1 button");
		    igreen1.backgroundColour = new Color(0, 188, 0);
		    igreen1.preferredSize = new Dimension(24, 24);
		    igreen1.constraints = DockLayout.BOTTOM;
		    
		    final Button iblue1 = new Button("Inner blue 1 button");
		    iblue1.backgroundColour = new Color(0, 0, 188);
		    iblue1.preferredSize = new Dimension(24, 24);
		    iblue1.constraints = DockLayout.LEFT;
		    
		    
		    final Button ired2 = new Button("Inner red 2 button");
		    ired2.backgroundColour = new Color(188, 0, 0);
		    ired2.preferredSize = new Dimension(24, 24);
		    ired2.constraints = DockLayout.TOP;
		    
		    final Button iyellow2 = new Button("Inner yellow 2 button");
		    iyellow2.backgroundColour = new Color(188, 188, 0);
		    iyellow2.preferredSize = new Dimension(24, 24);
		    iyellow2.constraints = DockLayout.yeild(1, DockLayout.RIGHT, 1);
		    
		    final Button igreen2 = new Button("Inner green 2 button");
		    igreen2.backgroundColour = new Color(0, 188, 0);
		    igreen2.preferredSize = new Dimension(24, 24);
		    igreen2.constraints = DockLayout.BOTTOM;
		    
		    final Button iblue2 = new Button("Inner blue 2 button");
		    iblue2.backgroundColour = new Color(0, 0, 188);
		    iblue2.preferredSize = new Dimension(24, 24);
		    iblue2.constraints = DockLayout.LEFT;
		    
		    
		    final Button ired3 = new Button("Inner red 3 button");
		    ired3.backgroundColour = new Color(188, 0, 0);
		    ired3.preferredSize = new Dimension(24, 24);
		    ired3.constraints = DockLayout.TOP;
		    
		    final Button iyellow3 = new Button("Inner yellow 3 button");
		    iyellow3.backgroundColour = new Color(188, 188, 0);
		    iyellow3.preferredSize = new Dimension(24, 24);
		    iyellow3.constraints = DockLayout.RIGHT;
		    
		    final Button igreen3 = new Button("Inner green 3 button");
		    igreen3.backgroundColour = new Color(0, 188, 0);
		    igreen3.preferredSize = new Dimension(24, 24);
		    igreen3.constraints = DockLayout.yeild(1, DockLayout.BOTTOM, 1);
		    
		    final Button iblue3 = new Button("Inner blue 3 button");
		    iblue3.backgroundColour = new Color(0, 0, 188);
		    iblue3.preferredSize = new Dimension(24, 24);
		    iblue3.constraints = DockLayout.LEFT;
		    
		    
		    final Button ired4 = new Button("Inner red 4 button");
		    ired4.backgroundColour = new Color(188, 0, 0);
		    ired4.preferredSize = new Dimension(24, 24);
		    ired4.constraints = DockLayout.TOP;
		    
		    final Button iyellow4 = new Button("Inner yellow 4 button");
		    iyellow4.backgroundColour = new Color(188, 188, 0);
		    iyellow4.preferredSize = new Dimension(24, 24);
		    iyellow4.constraints = DockLayout.RIGHT;
		    
		    final Button igreen4 = new Button("Inner green 4 button");
		    igreen4.backgroundColour = new Color(0, 188, 0);
		    igreen4.preferredSize = new Dimension(24, 24);
		    igreen4.constraints = DockLayout.BOTTOM;
		    
		    final Button iblue4 = new Button("Inner blue 4 button");
		    iblue4.backgroundColour = new Color(0, 0, 188);
		    iblue4.preferredSize = new Dimension(24, 24);
		    iblue4.constraints = DockLayout.yeild(1, DockLayout.LEFT, 1);
		    
		    
		    final Button ired5 = new Button("Inner red 5 button");
		    ired5.backgroundColour = new Color(188, 0, 0);
		    ired5.preferredSize = new Dimension(24, 24);
		    ired5.constraints = DockLayout.yeild(1, DockLayout.TOP, 1);
		    
		    final Button iyellow5 = new Button("Inner yellow 5 button");
		    iyellow5.backgroundColour = new Color(188, 188, 0);
		    iyellow5.preferredSize = new Dimension(24, 24);
		    iyellow5.constraints = DockLayout.RIGHT;
		    
		    final Button igreen5 = new Button("Inner green 5 button");
		    igreen5.backgroundColour = new Color(0, 188, 0);
		    igreen5.preferredSize = new Dimension(24, 24);
		    igreen5.constraints = DockLayout.BOTTOM;
		    
		    final RadioButton iblue5 = new RadioButton("Inner blue 5 radio button");
		    iblue5.boxBackgroundColour = new Color(0, 0, 188);
		    iblue5.preferredSize = new Dimension(24, 24);
		    iblue5.constraints = DockLayout.yeild(1, DockLayout.LEFT, 1);
		    iblue5.value = false;
		    
		    final Button button = new Button("Centre component");
		    button.text = "Centre\nCentre";
		    button.backgroundColour = new Color(0, 192, 0);
		    button.foregroundColour = new Color(255, 63, 255);
		    button.constraints = DockLayout.CENTER;
		    
		    final Button silver = new Button("Silver radioed button");
		    silver.text = "Silver ratioed";
		    silver.preferredSize = Button.silverRatio(silver.calculateSize());
		    
		    final Frame frame = new Frame("Frame component");
		    frame.container.backgroundColour = new Color(188, 188, 188);
		    frame.container.layoutManager = new DockLayout(frame.container);
		    (frame.label = silver).parent = frame;
		    (button.parent = frame.container).children.add(button);
		    
		    
		    final TextField text = new TextField("Text field component");
		    text.text = "Text";
		    text.preferredSize = text.calculateSize();
		    text.constraints = new Point(32, 24);
		    (text.parent = button).children.add(text);
		    
		    
		    final Component inner = new Button("Inner dock component");
		    inner.layoutManager = new DockLayout(inner);
		    inner.backgroundColour = new Color(188, 0, 188);
		    inner.constraints = DockLayout.CENTER;
		    
		    (ired1.parent = inner).children.add(ired1);
		    (iyellow1.parent = inner).children.add(iyellow1);
		    (igreen1.parent = inner).children.add(igreen1);
		    (iblue1.parent = inner).children.add(iblue1);
		    
		    (ired2.parent = inner).children.add(ired2);
		    (iyellow2.parent = inner).children.add(iyellow2);
		    (igreen2.parent = inner).children.add(igreen2);
		    (iblue2.parent = inner).children.add(iblue2);
		    
		    (ired3.parent = inner).children.add(ired3);
		    (iyellow3.parent = inner).children.add(iyellow3);
		    (igreen3.parent = inner).children.add(igreen3);
		    (iblue3.parent = inner).children.add(iblue3);
		    
		    (ired4.parent = inner).children.add(ired4);
		    (iyellow4.parent = inner).children.add(iyellow4);
		    (igreen4.parent = inner).children.add(igreen4);
		    (iblue4.parent = inner).children.add(iblue4);
		    
		    (frame.parent = inner).children.add(frame);
		    
		    
		    final Window window = new Window("Test window");
		    window.layoutManager = new DockLayout(window);
		    window.isDoubleBuffered = true;
		    (blue.parent = window).children.add(blue);
		    (vsep.parent = window).children.add(vsep);
		    (red.parent = window).children.add(red);
		    (toggle.parent = window).children.add(toggle);
		    (hsep.parent = window).children.add(hsep);
		    (label.parent = window).children.add(label);
		    (cyan.parent = window).children.add(cyan);
		    (inner.parent = window).children.add(inner);
		    window.setTitle("ITK test");
		    window.setVisible(true);
		}
	    });
    }
    
}

