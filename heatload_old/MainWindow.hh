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

#ifndef _MAIN_WINDOW_HH_
#  define _MAIN_WINDOW_HH_

#include "GraphDrawingArea.hh"
#include <gtk/gtkrc.h>


#include <gtk--/main.h>
#include <gtk--/window.h>
#include <gtk--/label.h>
#include <gtk--/box.h>
#include <gtk--/drawingarea.h>
#include <gtk--/button.h>
#include <gtk--/style.h>
#include <gtk--/frame.h>


class MainWindow : public Gtk::Window {

protected:

    Gtk::VBox vbox;
    Gtk::Label label;
    Gtk::Label cpu_label, temperature_label;
    Gtk::Frame frame;
    GraphDrawingArea drawing_area;
    Gtk::HBox hbox, hbox2;
    Gtk::Button refresh_button;
    Gtk::Button quit_button;

public:
    MainWindow(const bool decoration,const bool show_label,const int frame_size
          ,const guint x,const guint y);


    void refresh();
    void quit();
    void update_values(guint8 v1, guint8 v2);

};


#endif
