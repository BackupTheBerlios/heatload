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


#ifndef _METER_STRUCT_HH_
#  define _METER_STRUCT_HH_

#include <gtk/gtkrc.h>
#include <gtk--/drawingarea.h>
#include <deque.h>

class MeterStruct {
    
protected:
    deque<guint8> data_deque;
    glong last_time;
    bool modified;  

public:
    MeterStruct() : last_time(0), modified(true) {};

    void add_value(guint8 b,const guint x_size);
    bool is_modified() { return modified; }
    void reset_modified() { modified = false; }

    void draw(Gdk_Drawable &d, Gdk_GC &g);
};

#endif
