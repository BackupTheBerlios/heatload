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

#include "MeterStruct.hh"

void MeterStruct::add_value(guint8 b,const guint x_size) 
{
/*
    GTimeVal tv;
    glong n;

    g_get_current_time(&tv);

    if (last_time == 0)   n = 1;
    else if (last_time == tv.tv_sec) 
     {
        data_deque.pop_front();
        n = 1;
     } 
   else n = tv.tv_sec - last_time;

   last_time = tv.tv_sec;
        
    for (; n > 0; n --)
*/
        data_deque.push_back(b);

    while (data_deque.size() > x_size)
        data_deque.pop_front();
//cout << x_size<<'\t'<<data_deque.size()<<'\n';
    modified = true;
}


void MeterStruct::draw(Gdk_Drawable &d, Gdk_GC &g) 
{
    gint lx = -1, ly = -1;
    std::deque<guint8> copy = data_deque;

    for (gint x = d.width()-1; x >= 0; x--) 
      {
        if (copy.size() == 0)
            break;

        guint8 b = copy.back();
        gint y = (100-b)*(d.height()-1)/100;

        if (lx == -1)
            d.draw_point(g, x, y);
        else
            d.draw_line(g, lx, ly, x, y);

        lx = x;
        ly = y;

        copy.pop_back();
    }
}
