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

#ifndef   _STRUCTS_HH_
#  define _STRUCTS_HH_

#include <string>

namespace heatload{

   struct st_widget{unsigned int x;unsigned int y;unsigned int refresh;
                    bool decoration;bool label; bool graph;
          st_widget() : x(100),y(50),refresh(2500),decoration(true),
                       label(true),graph(true) {}          
                    };


   struct st_show{bool ac;bool bat; bool temp;bool load; bool fan;
          st_show() : ac(true),bat(true),temp(true),load(true),fan(true){}
//          st_show(const bool a,const bool b,,const bool t,
//                       ,const bool l,,const bool f)
//                   : ac(a),bat(b),temp(t),load(l),fan(f){}
                       };

    struct st_color{std::string temp_label;std::string bat_label;
                        std::string load_label;
                        std::string temp_meter;std::string bat_meter;
                        std::string load_meter;
            st_color() : temp_label("darkred"),bat_label("white"),
                            load_label("SeaGreen"),
                            temp_meter("tomato"),bat_meter("white"),
                            load_meter("green"){}
                        };
}

#endif
