/*  heatload
 *  Copyright (C) 2002 Malte Thoma
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

 static std::string run_time_options="    run-time-options:\n"
                 "    ?        show summery of all run time options\n"
                 "    a        toggle show/hide ac\n\n"
                 "    b        toggle show/hide battery\n"
                 "    c        toggles between designed- and last-max-capacity\n"
                 "              when calculating the batterys fill-percentage\n"
                 "    d        toggles show/hide decoration\n"
                 "    e        toggles show/hide label\n"
                 "    f        toggle show/hide fan\n"   
                 "    g        toggles show/hide graph\n"
                 "    l        toggle show/hide load\n"  
                 "    r        immediate reload\n"
                 "    s        show sudo hints\n" 
                 "    t        toggle show/hide thermal\n"
                 "    u        toggle show/hide cpu throttling\n\n"
                 ;



   struct st_widget{unsigned int x;unsigned int y;unsigned int refresh;
                    bool decoration;bool label; bool graph; bool menu;
          st_widget() : x(100),y(70),refresh(2500),decoration(false),
                       label(true),graph(true),menu(true) {}          
                    };


   struct st_show{bool ac;bool bat; bool temp;bool load; bool fan; bool cpu_throttling;
          st_show() : ac(true),bat(true),temp(true),load(true),fan(true),cpu_throttling(true) {}
//          st_show(const bool a,const bool b,,const bool t,
//                       ,const bool l,,const bool f,const bool c)
//                   : ac(a),bat(b),temp(t),load(l),fan(f),cpu_throttling(c){}
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
