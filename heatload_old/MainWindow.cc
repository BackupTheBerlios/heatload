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
                
#include "MainWindow.hh"
#include <stdio.h>
#include <gnome--/main.h>


MainWindow::MainWindow(const bool decoration,const bool show_label,const int frame_size,const guint x,const guint y):

    Gtk::Window(GTK_WINDOW_TOPLEVEL),
    vbox(false, frame_size),
    label("Heatload "VERSION" by Lennart Poettering 2002\nGreen: CPU Load; Red: ACPI CPU Temperature\n(Ranges from 0% to 100%, resp. 0°C  to 100°C)"),
    cpu_label("Load: n/a"),
    temperature_label("Temperature: n/a"),
    drawing_area(x,y),
    hbox(true, frame_size),
    hbox2(true, frame_size),
    refresh_button("Refresh"),
    quit_button("Quit") {

    set_border_width(frame_size);
    set_title("Heatload");

    cpu_label.set_justify(GTK_JUSTIFY_LEFT);
    cpu_label.set_alignment(0, 0);

    temperature_label.set_justify(GTK_JUSTIFY_LEFT);
    temperature_label.set_alignment(0, 0);

    Gdk_Color color("darkgreen");
    GtkRcStyle *rc_style = gtk_rc_style_new ();
    rc_style->fg[GTK_STATE_NORMAL] = color;
    rc_style->color_flags[GTK_STATE_NORMAL] = (GtkRcFlags) ((int) rc_style->color_flags[GTK_STATE_NORMAL] | GTK_RC_FG);

    gtk_widget_modify_style(GTK_WIDGET(cpu_label.gtkobj()), rc_style);
    gtk_rc_style_unref(rc_style);
    
    color = Gdk_Color("darkred");
    rc_style = gtk_rc_style_new ();
    rc_style->fg[GTK_STATE_NORMAL] = color;
    rc_style->color_flags[GTK_STATE_NORMAL] = (GtkRcFlags) ((int) rc_style->color_flags[GTK_STATE_NORMAL] | GTK_RC_FG);

    gtk_widget_modify_style(GTK_WIDGET(temperature_label.gtkobj()), rc_style);
    gtk_rc_style_unref(rc_style);
    
    drawing_area.size(200, 200);
    frame.set_shadow_type(GTK_SHADOW_IN);

    destroy.connect(slot(this, &MainWindow::quit));
    quit_button.clicked.connect(destroy.slot());
    refresh_button.clicked.connect(slot(this, &MainWindow::refresh));

    frame.add(drawing_area);
    
    if(decoration) vbox.pack_start(label, false, true);
    vbox.pack_start(frame, true, true);

    hbox2.pack_start(cpu_label, false, true);
    hbox2.pack_start(temperature_label, false, true);
    

    if(show_label) vbox.pack_start(hbox2, false, true);

    hbox.pack_start(refresh_button, false, true);
    hbox.pack_start(quit_button, false, true);

    if(decoration) vbox.pack_start(hbox, false, true);

    add(vbox);

    show_all();
}

void MainWindow::refresh() {
    drawing_area.refresh_pixmap();
    drawing_area.expose_event(NULL);
}

void MainWindow::quit() {
    Gnome::Main::quit();
}

void MainWindow::update_values(guint8 v1, guint8 v2) {
    char txt[256];
    static int ov1 = -1;
    static int ov2 = -1;

    if (ov1 != v1) {
        snprintf(txt, sizeof(txt), "Load: %u%%", (unsigned) v1);
        cpu_label.set_text(txt);
        ov1 = v1;
    }
            
    if (ov2 != v2) {   
        snprintf(txt, sizeof(txt), "Temperature: %u°C", (unsigned) v2);
        temperature_label.set_text(txt);
        ov2 = v2;
    }
            
}
