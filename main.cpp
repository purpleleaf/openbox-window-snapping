#include <iostream>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <string.h>
#include <chrono>
#include <thread>
extern "C" {
#include <xdo.h>
}

XRRMonitorInfo getMonitor(int x, int y, Display *dis, Window win1)
{
    int monitorCnt;
    XRRMonitorInfo* xMonitors = XRRGetMonitors( dis, win1, true, &monitorCnt );
    for (int currentMonitor = 0; currentMonitor < monitorCnt; currentMonitor++)
    {
        if ((x >= xMonitors[currentMonitor].x) && (x < xMonitors[currentMonitor].x + xMonitors[currentMonitor].width) && (y >= xMonitors[currentMonitor].y) && (y < xMonitors[currentMonitor].y + xMonitors[currentMonitor].height))
        {
            return xMonitors[currentMonitor];
        }
    }
    XRRMonitorInfo nullReturn = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    return nullReturn;
}

void moveWindow(int x, int y, XRRMonitorInfo xMonitor, int edgeMargin, Display* display, xdo_t* xdo, Window activeWindow, int tittleBarHeight, XEvent ev)
{
    int leftX = xMonitor.x;
    int rightX = xMonitor.width / 2 + xMonitor.x;
    int topY = xMonitor.y;
    int bottomY = xMonitor.height / 2 + xMonitor.y;

    int width = xMonitor.width;
    int height = xMonitor.height - tittleBarHeight;
    int widthHalf = xMonitor.width / 2;
    int heightHalf = xMonitor.height / 2 - tittleBarHeight;

    int leftBorder = xMonitor.x + edgeMargin;
    int topBorder = xMonitor.y + edgeMargin;
    int rightBorder = xMonitor.x + xMonitor.width - edgeMargin;
    int bottomBorder = xMonitor.y + xMonitor.height - edgeMargin;



    if (x < leftBorder && y < topBorder) // Top left
    {
        xdo_move_window(xdo, activeWindow, leftX, topY);
        xdo_set_window_size(xdo, activeWindow, widthHalf, heightHalf, 0);
    }
    else if (x > rightBorder && y < topBorder) // Top Right
    {
        xdo_move_window(xdo, activeWindow, rightX, topY);
        xdo_set_window_size(xdo, activeWindow, widthHalf, heightHalf, 0);
    }
    else if (x < leftBorder && y > bottomBorder) // Top Right
    {
        xdo_move_window(xdo, activeWindow, leftX, bottomY);
        xdo_set_window_size(xdo, activeWindow, widthHalf, heightHalf, 0);
    }
    else if (x > rightBorder && y > bottomBorder) // Top Right
    {
        xdo_move_window(xdo, activeWindow, rightX, bottomY);
        xdo_set_window_size(xdo, activeWindow, widthHalf, heightHalf, 0);
    }
    else if (x < leftBorder)
    {
        xdo_move_window(xdo, activeWindow, leftX, topY);
        xdo_set_window_size(xdo, activeWindow, widthHalf, height, 0);
    }
    else if (x > rightBorder)
    {
        xdo_move_window(xdo, activeWindow, rightX, topY);
        xdo_set_window_size(xdo, activeWindow, widthHalf, height, 0);
    }
    else if (y > bottomBorder)
    {
        xdo_move_window(xdo, activeWindow, leftX, bottomY);
        xdo_set_window_size(xdo, activeWindow, width, heightHalf, 0);
    }
    else if (y < topBorder)
    {
        ev.xclient.data.l[0] = 1;
        XSendEvent(display, DefaultRootWindow(display), False, SubstructureRedirectMask, &ev);
    }
    XCloseDisplay(display);
    xdo_free(xdo);
    //XSendEvent(display, DefaultRootWindow(display), False, SubstructureRedirectMask, &ev);
}

int main()
{

    int edgeMargin = 40;
    int tittleBarHeight = 34;

    // Display
    Display *display = XOpenDisplay(NULL);
    int screen = DefaultScreen(display);
    Screen*  s = DefaultScreenOfDisplay(display);
    // Root Window
    Window win1 = RootWindow(display, screen);
    Window win2, win3;
    // Mouse x and y
    int x, y, winx, winy;
    unsigned int mask;
    // Get val of x and y
    XQueryPointer(display, win1, &win2, &win3, &x, &y, &winx, &winy, &mask);

    Window activeWindow;
    xdo_t *xdo = xdo_new(NULL);
    // Get value of activeWindow
    xdo_get_active_window(xdo, &activeWindow);
    XEvent ev;
    memset(&ev, 0, sizeof (ev));
    ev.xclient.window = activeWindow;
    ev.xclient.type = ClientMessage;
    ev.xclient.format = 32;
    ev.xclient.message_type = XInternAtom(display, "_NET_WM_STATE", False);
    ev.xclient.data.l[0] = 0;
    ev.xclient.data.l[1] = XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
    ev.xclient.data.l[2] = XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_VERT", False);
    ev.xclient.data.l[3] = 1;

    XSendEvent(display, DefaultRootWindow(display), False, SubstructureRedirectMask, &ev);
    // Wait until left button is not pressed
    while(mask == 256)
    {
        XQueryPointer(display, win1, &win2, &win3, &x, &y, &winx, &winy, &mask);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    // Return which monitor pointer/cursor is on
    XRRMonitorInfo xMonitor = getMonitor(x, y, display, win1);

    



    if (xMonitor.name)
    {
        moveWindow(x, y, xMonitor, edgeMargin, display, xdo, activeWindow, tittleBarHeight, ev);
    }


}
