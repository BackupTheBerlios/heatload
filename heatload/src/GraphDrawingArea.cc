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

GraphDrawingArea::GraphDrawingArea(const guint x,const guint y,
    const HeatloadGizmo &HG): 
    Gtk::DrawingArea(), x_size(x),y_size(y),
    color_background("black")
{
    VM.push_back(st_meter("Load",HG.cpu_load.ColorMeter()));
    VM.push_back(st_meter("Temperature",HG.thermal.ColorMeter()));
    VM.push_back(st_meter("Battery",HG.battery.ColorMeter()));
    set_usize(x_size,y_size);
}

gint GraphDrawingArea::expose_event_impl(GdkEventExpose* event) 
{
    if (!pixmap.connected())  refresh_pixmap();
    
    if (pixmap.connected()) 
     {
        Gdk_Window win = get_window();
        Gdk_GC gc = get_style()->get_fg_gc(get_state());
        win.draw_pixmap(gc, pixmap, 0, 0, 0, 0);
     }
    return TRUE;
}


void GraphDrawingArea::refresh_pixmap() 
{
  if (!pixmap.connected() || meter_is_modified())
   {
      meter_reset_modified();

      if (!pixmap.connected()) 
       {
         pixmap = Gdk_Pixmap(get_window(), x_size, y_size);
         get_window().get_colormap().alloc(color_background);
         meter_alloc_color();
       }
      Gdk_GC gc(pixmap);

      gc.set_foreground(color_background);
      pixmap.draw_rectangle(gc, TRUE, 0, 0, pixmap.width(), pixmap.height());
        
      for(std::vector<st_meter>::iterator i=VM.begin();i!=VM.end();++i)
       {  
         gc.set_foreground(i->color);
         i->meter.draw(pixmap, gc);
       }
     }
  expose_event(NULL);
}


bool GraphDrawingArea::meter_is_modified()
{
  for(std::vector<st_meter>::iterator i=VM.begin();i!=VM.end();++i)
     if(i->meter.is_modified()) return true;
  return false;
}

void GraphDrawingArea::meter_reset_modified()
{
  for(std::vector<st_meter>::iterator i=VM.begin();i!=VM.end();++i)
     i->meter.reset_modified();
}

void GraphDrawingArea::meter_alloc_color()
{
  for(std::vector<st_meter>::iterator i=VM.begin();i!=VM.end();++i)
     get_window().get_colormap().alloc(i->color);
}

