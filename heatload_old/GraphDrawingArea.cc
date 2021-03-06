/*  heatload
 *  Copyright (C) 2002 , written by Lennart Poettering and Malte Thoma
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "GraphDrawingArea.hh"
#include <gtk--/style.h>
#include "MeterStruct.hh"

extern MeterStruct load_meter;
extern MeterStruct temperature_meter;
extern MeterStruct battery_meter;

GraphDrawingArea::GraphDrawingArea(const int x,const int y): 
    Gtk::DrawingArea(), x_size(x),y_size(y),
    color_background("black"), color_load("green"), color_temp("tomato")
      ,color_bat("blue") {
    set_usize(x_size,y_size);
}

gint GraphDrawingArea::expose_event_impl(GdkEventExpose* event) {

    if (!pixmap.connected())
        refresh_pixmap();
    
    if (pixmap.connected()) {
        Gdk_Window win = get_window();
        Gdk_GC gc = get_style()->get_fg_gc(get_state());
        win.draw_pixmap(gc, pixmap, 0, 0, 0, 0);
    }
        
    return TRUE;
}

void GraphDrawingArea::refresh_pixmap() {

    if (!pixmap.connected() || load_meter.is_modified() 
                            || temperature_meter.is_modified()
                            || battery_meter.is_modified()) {
        load_meter.reset_modified();
        temperature_meter.reset_modified();
        battery_meter.reset_modified();

        if (!pixmap.connected()) {
            pixmap = Gdk_Pixmap(get_window(), x_size, y_size);
            get_window().get_colormap().alloc(color_background);
            get_window().get_colormap().alloc(color_load);
            get_window().get_colormap().alloc(color_temp);
            get_window().get_colormap().alloc(color_bat);
        }
        
        Gdk_GC gc(pixmap);

        gc.set_foreground(color_background);
        pixmap.draw_rectangle(gc, TRUE, 0, 0, pixmap.width(), pixmap.height());
        
        gc.set_foreground(color_load);
        load_meter.draw(pixmap, gc);

        gc.set_foreground(color_temp);
        temperature_meter.draw(pixmap, gc);

        gc.set_foreground(color_bat);
        battery_meter.draw(pixmap, gc);
    }
}
