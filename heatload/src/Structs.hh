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

using namespace std;

namespace heatload{

 static std::string run_time_options="    run-time-options:\n"
                 "    ?        show summery of all run time options\n"
                 "    a        toggle show/hide ac\n"
                 "    b        toggle show/hide battery\n"
                 "    c        toggles between designed- and last-max-capacity\n"
                 "              when calculating the batterys fill-percentage\n"
                 "    d        toggles show/hide decoration\n"
                 "    e        toggles show/hide label\n"
                 "    f        toggle show/hide fan\n"   
                 "    g        toggles show/hide graph\n"
                 "    l        toggle show/hide load\n"  
                 "    p        toggle show/hide cpu-performance\n"  
                 "    r        immediate reload\n"
                 "    s        show sudo hints\n" 
                 "    t        toggle show/hide thermal\n"
                 "    u        toggle show/hide cpu-throttling\n\n"
                 ;


        enum e_find{None=0,eAC='A',eBat='B',eBatInfo='I',eFan='F',
                    eThermal='T',
                    eCPUthrottling='C',eCPUperformance='P',
                    eLoad='L',eSuspend_sleep='S',eSuspend_awake='a'};

   struct st_widget{unsigned int x;unsigned int y;unsigned int refresh;
                    bool decoration;bool label; bool graph; bool menu;
                    bool swsusp;
          st_widget() : x(110),y(65),refresh(2500),decoration(false),
                       label(true),graph(true),menu(true),swsusp(true) {}          
                    };
   struct st_auto{e_find EF; int temperature; int state; mutable bool set;
          st_auto(e_find ef,int t,int s) : EF(ef),temperature(t),state(s),
                                           set(false){}};
}

#endif
