/* $Id: gtk_acpi.hh,v 1.20 2002/12/27 08:27:30 thoma Exp $ */
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
        HeatloadGizmo HG;

        heatload::st_widget show_widget;
        mutable bool show_sudo;
        
        friend class gtk_acpi_glade;
        friend class WindowInfo;

        void init();
        void menu_init();
        void select_throttling(guint i);

        void show_sudo_error();
        void show_run_time_options();

        void show_values();
        gint timeout();

        void set_color(Gtk::Widget &W,const std::string &color);
        void hide_or_show_elements();

        gint on_gtk_acpi_delete_event(GdkEventAny *ev);
        gint on_gtk_acpi_key_press_event(GdkEventKey *ev);
        gint on_eventbox_main_button_press_event(GdkEventButton *event);
        void ende();
   public:
        gtk_acpi(const FileFinder &FF,const heatload::st_widget &show_widget,
                const bool _show_sudo,HeatloadGizmo &HG);
        void set_show_sudo(bool b) const {show_sudo=b;}
        void save() const;
};
#endif
