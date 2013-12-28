#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(void)
{
  Display* display;
  int screen;
  Window root_window;
  Window window;
  XEvent event;
  
  if ((display = XOpenDisplay(NULL)) == NULL)
    {
      fprintf(stderr, "Cannot open display\n");
      exit(1);
    }
  
  screen = DefaultScreen(display);
  root_window = RootWindow(display, screen);
  window = XCreateSimpleWindow(display, root_window, 0, 0, 800, 600, 50,
			       BlackPixel(display, screen),
			       WhitePixel(display, screen));
  XSelectInput(display, window, ExposureMask);
  XMapWindow(display, window);
 
  for (;;)
    {
      XNextEvent(display, &event);
      if (event.type == Expose)
	{
	  XFillRectangle(display, window, DefaultGC(display, screen), 20, 20, 10, 10);
	  XDrawString(display, window, DefaultGC(display, screen), 10, 50, "Hello world!", strlen("Hello world!"));
	}
    }
  XCloseDisplay(display);
  return 0;
}

