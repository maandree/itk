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

import javax.swing.*;


/**
 * Root component in a GUI, often exclusively controlled by the window manager.
 */
public class Window
{
    /**
     * Constructor
     */
    public Window()
    {
	this.window = new JFrame();
    }
    
    
    
    /**
     * Underlaying window component
     */
    private JFrame window;
    
    
    
    /**
     * Sets the title of the window
     * 
     * @param  title  The new title of the window
     */
    public void setTitle(final String title)
    {
	this.window.setTitle(title);
    }
    
    /**
     * Gets the title of the window
     * 
     * @return  The title of the window
     */
    public String getTitle()
    {
	return this.window.getTitle();
    }
    
    
    /**
     * Sets the visibility of the window
     * 
     * @param  title  The new visibility of the  window
     */
    public void setVisible(final boolean visible)
    {
	this.window.setVisible(visible);
    }
    
    /**
     * Gets the visibility of the window
     * 
     * @return  The visibility of the window
     */
    public boolean isVisible()
    {
	return this.window.isVisible();
    }
    
}

