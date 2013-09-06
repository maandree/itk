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
 * Component layout manager
 */
public interface LayoutManager
{
    /**
     * Prepare the layout manager for locating of multiple components, probably all of them
     */
    public void prepare();
    
    /**
     * End of {@link #prepare()} requirement
     */
    public void done();
    
    
    /**
     * Locates the positions of the corners of a component
     * 
     * @param   child  The child, to the component using the layout manager, of interest
     * @return         The rectangle the child is confound in
     */
    public Rectangle locate(Component child);
    
}

