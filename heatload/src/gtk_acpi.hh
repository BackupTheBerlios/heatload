/* $Id: gtk_acpi.hh,v 1.15 2002/12/18 13:29:17 thoma Exp $ */
/*  Copyright (C) 2002 Malte Thoma
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


// generated 2002/3/18 6:58:50 CET by thoma@Tiger.(none)
// using glademm V0.6.4b_cvs
//
// newer (non customized) versions of this file go to gtk_acpi.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _GTK_ACPI_HH
#  include "gtk_acpi_glade.hh"
#  define _GTK_ACPI_HH

#include<string>
#include "GraphDrawingArea.hh"
#include "itos.h"
#include "Structs.hh"
#include <gtk--/menu.h>
#include <map>

class gtk_acpi : public gtk_acpi_glade
{  
        Gtk::Menu *menu;
        heatload::st_widget show_widget;
        const bool read_max_cap_;
        heatload::st_show show_what;
        heatload::st_color color;
        bool use_max_cap,show_sudo;
        int max_cap,last_max_cap;
        
        friend class gtk_acpi_glade;
        friend class WindowInfo;

        enum e_find{eAC,eBat,eThermal,eCPUthrottling};
        struct st_find_filename{std::string name;bool old_style;
               st_find_filename() : old_style(false) {}
               st_find_filename(const std::string n):name(n),old_style(false){}
               st_find_filename(const std::string n,const bool o):name(n),old_style(o){}};
        st_find_filename ac_file;
        st_find_filename bat_file;
        st_find_filename therm_file;
        st_find_filename cpu_thrott_file;
        
        struct st_throttling{std::string tstate;int state;std::string prozent;
               st_throttling() : state(-1){}
               st_throttling(const std::string &t,const int s,const std::string &p)
                  :tstate(t),state(s),prozent(p) {}};
        std::vector<st_throttling> vec_throttling;

        struct st_ac_adapter{std::string state;};
        struct st_cpu{st_throttling throttling;};
        struct st_cpu_load{guint load;};
        struct st_thermal{guint temp;std::string einheit;
                          std::string cooling_mode;
               st_thermal() {}
               st_thermal(guint t1,std::string e,std::string c)
                  : temp(t1),einheit(e),cooling_mode(c){}
               std::string Celsius() const 
                  { if(einheit=="C") return itos(temp)+"C   ";
                    else if(einheit=="dK") return itos(int((temp/10.)-273.15))+"C   ";
                    else return "unknown unit of tempertur, please contact thoma@muenster.de\n";
                   }};
        struct st_battery{bool present;bool charging; int present_rate_mW; 
                          int remaining_capacity_mWh;int max_capacity_mWh;
                          int last_max_capacity_mWh;
               st_battery() :present(false),charging(false),present_rate_mW(0),
                         remaining_capacity_mWh(0),max_capacity_mWh(0),
                         last_max_capacity_mWh(0) {}
               st_battery(const bool p,const bool c,const int pr,
                          const int r,const int m,const int lm)
                  : present(p),charging(c),present_rate_mW(pr),
                    remaining_capacity_mWh(r),max_capacity_mWh(m),
                    last_max_capacity_mWh(lm) {}
               const double prozent() const {return double(remaining_capacity_mWh)/max_capacity_mWh;}
               const double prozent_last() const {return double(remaining_capacity_mWh)/last_max_capacity_mWh;}
               const std::string prozent_string() const {return ::itos(int(prozent()*100))+"%";}
               const std::string prozent_last_string() const {return ::itos(int(prozent_last()*100))+"%";}
                };
              
        st_ac_adapter ac_adapter;
        st_cpu cpu;
        st_thermal thermal;
        st_battery battery;
        st_cpu_load cpu_load;

        void init();
        void menu_init();
        gint on_eventbox_main_button_press_event(GdkEventButton *event);
        void select_throttling(guint i);
        void load_thrott_file();
        st_throttling throttling_from_state(const std::string &s);

        void show_sudo_error();
        void show_run_time_options();
        void find_filenames();
        bool find_filename(const e_find EF,const std::vector<st_find_filename> &F);

        void read_max_cap();
        void show_values();
        std::string remaining_time();
        void get_values();
        void get_ac_adapter();        
        void get_thermal();        
        void get_battery();        
        void get_load_value();
        void get_throttling();

        void get_show();
        gint timeout();

        void set_color(Gtk::Widget &W,const std::string &color);
        void hide_or_show_elements();

        GraphDrawingArea *GDA;
        gint on_gtk_acpi_delete_event(GdkEventAny *ev);
        gint on_gtk_acpi_key_press_event(GdkEventKey *ev);
        void save();
        void ende();
   public:
       gtk_acpi::gtk_acpi(const heatload::st_widget &show_widget,
             const bool _read_max_cap, const bool _show_sudo,
             const heatload::st_show &_show_what,const heatload::st_color &_color);
};
#endif
