/* $Id: gtk_acpi.hh,v 1.18 2002/12/20 22:12:05 thoma Exp $ */
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
#include "FileFinder.hh"
#include "Gizmo.hh"

class gtk_acpi : public gtk_acpi_glade
{  
        Gtk::Menu *menu;
        GraphDrawingArea *GDA;
        FileFinder FF;

        heatload::st_widget show_widget;
        heatload::st_show show_what;
        heatload::st_color color;
        mutable bool show_sudo;
        
        friend class gtk_acpi_glade;
        friend class WindowInfo;

        struct st_throttling{std::string tstate;int state;std::string prozent;
               st_throttling() : state(-1),prozent("?") {}
               st_throttling(const std::string &t,const int s,const std::string &p)
                  :tstate(t),state(s),prozent(p) {}};
        std::vector<st_throttling> vec_throttling;

        struct st_performance{std::string tstate;int state;std::string value;
               st_performance() : state(-1),value("?"){}
               st_performance(const std::string &t,const int s,const std::string &p)
                  :tstate(t),state(s),value(p) {}};
        std::vector<st_performance> vec_performance;


        struct st_cpu{st_throttling throttling;st_performance performance;};
        Gizmo ac_adapter;
        st_cpu cpu;
        GizmoThermal thermal;
        Gizmo fan;
        GizmoBattery battery;
        Gizmo cpu_load;

        void init();
        void menu_init();
        gint on_eventbox_main_button_press_event(GdkEventButton *event);
        void select_throttling(guint i);
        void load_thrott_file();
        st_throttling throttling_from_state(const std::string &s);
        void load_performance_file();
        void select_performance(guint i);
        st_performance performance_from_state(const std::string &s);

        void show_sudo_error();
        void show_run_time_options();

        void show_values();
        void get_values();
        void get_ac_adapter();        
        void get_thermal();        
        void get_battery();        
        void get_load_value();
        void get_throttling();
        void get_performance();
        

        void get_show();
        gint timeout();

        void set_color(Gtk::Widget &W,const std::string &color);
        void hide_or_show_elements();

        gint on_gtk_acpi_delete_event(GdkEventAny *ev);
        gint on_gtk_acpi_key_press_event(GdkEventKey *ev);
        void ende();
   public:
       gtk_acpi(const FileFinder::FileMap_t &_FileMap,
             const heatload::st_widget &show_widget,
             const bool _read_max_cap, const bool _show_sudo,
             const heatload::st_show &_show_what,const heatload::st_color &_color);

        void set_show_sudo(bool b) const {show_sudo=b;}
        void save() const;
        void re_init() {init();}
};
#endif
