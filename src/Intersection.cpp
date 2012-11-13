//============================================================================
// Name        : Intersection.cpp
// Author      : Mikkel Kjaer Jesnen
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gdk/gdktypes.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <queue>
#include "Point.h"
#include "line_segment.h"

using namespace std;

//typedef std::priority_queue<line_segment,std::vector<line_segment>, compare_event_point_pointers > line_segment_priority_queue;

static GtkWidget *window = NULL;

/*!
 * \brief Pixmap to scribble area, to store our scribbles
 */
static cairo_surface_t *surface = NULL;

Point point1;
Point point2;

static std::vector<line_segment*> line_segments;
static std::vector<line_segment*> intersects;


static int point_state = 0;

/*!
 * \brief Create a new surface of the appropriate size to store our scribbles
 */
static gboolean scribble_configure_event (GtkWidget*, GdkEventConfigure*, gpointer);

/*!
 * \brief Redraw the screen from the surface
 */
static gboolean scribble_expose_event (GtkWidget*, GdkEventExpose*, gpointer);

/*!
 * \brief Draw a rectangle on the screen
 */
static void draw_rectangle (GtkWidget *widget, gdouble x, gdouble y);

static gboolean scribble_button_press_event (GtkWidget*, GdkEventButton*, gpointer);
static gboolean scribble_motion_notify_event (GtkWidget*, GdkEventMotion*, gpointer);
static void close_window (void);
GtkWidget * do_drawingarea ();

/*!
 * \brief Create a new surface of the appropriate size to store our scribbles
 */
static gboolean scribble_configure_event (GtkWidget *widget, GdkEventConfigure *event, gpointer data)
{
    cairo_t *cr = NULL;

    if (surface)
    {
        cairo_surface_destroy (surface);
    }

    surface = gdk_window_create_similar_surface (widget -> window,
              CAIRO_CONTENT_COLOR,
              widget -> allocation.width,
              widget -> allocation.height);

    /* Initialize the surface to white */
    cr = cairo_create (surface);
    cairo_set_source_rgb (cr, 1, 1, 1);
    cairo_paint (cr);
    cairo_destroy (cr);

    /* We've handled the configure event, no need for further processing. */
    return TRUE;
}

/*!
 * \brief Redraw the screen from the surface
 */
static gboolean scribble_expose_event (GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
    cairo_t *cr = NULL;

    cr = gdk_cairo_create (widget->window);
    cairo_set_source_surface (cr, surface, 0, 0);
    gdk_cairo_rectangle (cr, &event->area);
    cairo_fill (cr);

    cairo_destroy (cr);

    return FALSE;
}

/*!
 * \brief Draw a rectangle on the screen
 */
static void draw_rectangle (GtkWidget *widget, gdouble x, gdouble y)
{
    GdkRectangle update_rect;
    memset(&update_rect, 0, sizeof(GdkRectangle));

    cairo_t *cr = NULL;

    int halfsize = 3;

    update_rect.x = x - halfsize;
    update_rect.y = y - halfsize;
    update_rect.width = halfsize * 2;
    update_rect.height = halfsize * 2;

    /* Paint to the surface, where we store our state */
    cr = cairo_create (surface);
    gdk_cairo_rectangle (cr, &update_rect);
    cairo_fill (cr);
    cairo_destroy (cr);

    /* Now invalidate the affected region of the drawing area. */
    gdk_window_invalidate_rect (widget->window,
                                &update_rect,
                                FALSE);
}

/*!
 * \brief Draw a circle on the screen
 */
static void draw_circle (GtkWidget *widget, gdouble x, gdouble y)
{
    GdkPoint update_point;
    memset(&update_point, 0, sizeof(update_point));

    cairo_t *cr = NULL;

    update_point.x = x;
    update_point.y = y;

    /* Paint to the surface, where we store our state */
    cr = cairo_create (surface);
    cairo_arc(cr, x, y, 200.0, 0, 2 * M_PI );

    cairo_fill (cr);
    cairo_destroy (cr);


    /* Now invalidate the affected region of the drawing area. */
//     gdk_window_invalidate_rectangle (widget->window,
    //                               &update_point,
    //                             FALSE);
}

static gboolean make_line_segment(GtkWidget *widget) {

	double x1 = point1.getX();
    double y1 = point1.getY();

	double x2 = point2.getX();
	double y2 = point2.getY();

	cairo_t *cr;
	cr = gdk_cairo_create(widget->window);

	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_line_width (cr, 0.5);

	cairo_move_to(cr, x1, y1);
	cairo_line_to(cr, x2, y2);

	cairo_stroke(cr);
	cairo_destroy(cr);

	point_state = 0;

	line_segment* segment;

	segment = new line_segment(&point1, &point2);
	line_segments.push_back(segment);

	Point* upper = segment->getupperPoint();
	Point* lower = segment->getLowerPoint();

	printf("upper: %f, %f\n", upper->getX(), upper->getY());
	printf("lower: %f, %f\n", lower->getX(), lower->getY());


	return TRUE;
}

static gboolean scribble_button_press_event (GtkWidget *widget, GdkEventButton *event, gpointer data)
{

    if (surface == NULL)
    {
        return FALSE; /* Paranoia check, in case we haven't gotten a configure event */
    }

    if (event->button == 1)
    {
    	int x = event->x;
    	int y = event->y;

    	// If the x and y values are identical to the first point, then I rule we are dealing with the same point, and nothing should be done
    	if (point_state == 1 &&
    			point1.getX() == x && point1.getY() == y) {
    		return TRUE;
    	}
        draw_rectangle (widget, x, y);

        if (point_state == 0) {
            point1 = Point(x, y, "point1");
            point_state = 1;
        } else if (point_state == 1) {
        	point2 = Point(x, y, "point2");
        	make_line_segment(widget);
       	}
        //draw_circle (widget, event->x, event->y);
    }

    /* We've handled the event, stop processing */
    return TRUE;
}

static gboolean scribble_motion_notify_event (GtkWidget *widget, GdkEventMotion *event, gpointer data)
{
    int x = 0, y = 0;
    GdkModifierType state = (GdkModifierType)0;

    if (surface == NULL)
    {
        return FALSE; /* paranoia check, in case we haven't gotten a configure event */
    }

    /* This call is very important; it requests the next motion event.
      * If you don't call gdk_window_get_pointer(), you'll only get a single
      * motion event.The reason is that we specified GDK_POINTER_MOTION_HINT_MASK to gtk_widget_set_events()
      * If we hadn't specified that, we could juste use event->x, event->y as the pointer location.
      * But we'd also get deluged in events.
      * By requesting the next event as we handle the current one, we avoid getting a huge number of
      * events faster than we can cope.
      */
    gdk_window_get_pointer (event->window, &x, &y, &state);

    if (state & GDK_BUTTON1_MASK)
    {
        //  draw_rectangle (widget, x, y);
        draw_circle(widget, x, y);
    }

    /* We've handled it, stop processing */
    return TRUE;
}


static void close_window (void)
{
    window = NULL;

    if (surface)
    {
        g_object_unref (surface);
    }

    surface = NULL;
    gtk_main_quit();
}

GtkWidget * do_drawingarea ()
{
    GtkWidget *frame = NULL, *vbox = NULL, *da = NULL, *button = NULL;

    if (!window)
    {
        window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
        //gtk_window_set_screen (GTK_WINDOW (window), gtk_widget_get_screen (do_widget));
        gtk_window_set_title (GTK_WINDOW (window), "Drawing Area");

        g_signal_connect (G_OBJECT(window), "destroy", G_CALLBACK (close_window), NULL);

        gtk_container_set_border_width (GTK_CONTAINER (window), 8);

        vbox = gtk_vbox_new (FALSE, 8);
        gtk_container_set_border_width (GTK_CONTAINER (vbox), 8);
        gtk_container_add (GTK_CONTAINER (window), vbox);

        /*
          * Create the scribble area
          */

        frame = gtk_frame_new (NULL);
        gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);
        gtk_box_pack_start (GTK_BOX (vbox), frame, TRUE, TRUE, 0);

        da = gtk_drawing_area_new();

        /* Set a minimum size */
        gtk_widget_set_size_request (da, 1000, 600);

        gtk_container_add (GTK_CONTAINER (frame), da);

        /* Signals used to handle backing surface */
        g_signal_connect (da, "expose_event", G_CALLBACK (scribble_expose_event), NULL);
        g_signal_connect (da, "configure_event", G_CALLBACK (scribble_configure_event), NULL);

        /* Event signals */
        g_signal_connect (da, "motion-notify-event", G_CALLBACK (scribble_motion_notify_event), NULL);
        g_signal_connect (da, "button-press-event", G_CALLBACK (scribble_button_press_event), NULL);

        // Close button
        button = gtk_button_new_from_stock ("Find intersections");
        g_signal_connect (button, "clicked", gtk_main_quit, NULL);
        gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, TRUE, 0);

        // Button to do drawing
        button = gtk_button_new_from_stock(GTK_STOCK_CLOSE);
        g_signal_connect(button, "clicked", gtk_main_quit, NULL);
        gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, TRUE, 1);

        /* Ask to receive events the drawing area doesn't normally
          * subscribe to
          */
        gtk_widget_set_events (da, gtk_widget_get_events (da)
                               | GDK_LEAVE_NOTIFY_MASK
                               | GDK_BUTTON_PRESS_MASK
                               | GDK_POINTER_MOTION_MASK
                               | GDK_POINTER_MOTION_HINT_MASK);

    }

    if (!gtk_widget_get_visible (window))
    {
        gtk_widget_show_all (window);
    }
    else
    {
        gtk_widget_destroy (window);
    }

    return window;
}

int main(int argc, char** argv) {

	line_segments.empty();

	gtk_init (&argc, &argv);

	do_drawingarea();

	    gtk_main();
	return 0;
}
