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

#ifndef _GRAPH_DRAWING_AREA_HH_
#  define _GRAPH_DRAWING_AREA_HH_

#include <gtk--/drawingarea.h>
#include "MeterStruct.hh"
#include <vector>

class GraphDrawingArea : public Gtk::DrawingArea 
{
   private:
    guint x_size,y_size;

    struct st_meter{std::string name; MeterStruct meter;Gdk_Color color;
           st_meter(const std::string &n,const std::string &c) 
            : name (n),meter(MeterStruct()),color(c) {} };
    std::vector<st_meter> VM;

    bool meter_is_modified();
    void meter_reset_modified();
    void meter_alloc_color();

public:
    GraphDrawingArea(const guint x,const guint y);

    virtual gint expose_event_impl(GdkEventExpose* event);
    void refresh_pixmap();
    std::vector<st_meter>& getVM() {return VM;}

private:
    Gdk_Pixmap pixmap;
    
    Gdk_Color color_background;
};

#endif
