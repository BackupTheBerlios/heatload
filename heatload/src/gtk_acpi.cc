// generated 2002/3/18 6:58:50 CET by thoma@Tiger.(none)
// using glademm V0.6.4b_cvs
//
// newer (non customized) versions of this file go to gtk_acpi.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "gtk_acpi.hh"
#include <gtk--/main.h>

gtk_acpi::gtk_acpi(const guint x,const guint y,const guint refresh,
   const bool _show_graph,const bool _show_label,const bool _show_decoration)
: x_size(x),y_size(y),show_graph(_show_graph),show_label(_show_label),
   show_decoration(_show_decoration),GDA(0)
{
  if(show_graph)
   {
     GDA = manage(new GraphDrawingArea(x_size,y_size));
     frame_draw->add(*GDA);
     GDA->show();
   }
  if(show_decoration) label_decoration->set_text("Heatload written by Lennart Poettering (2002)\n"
                  "rewritten and enhanced by Malte Thoma");
  else label_decoration->hide();
  if(!show_label) frame_label->hide();
  else 
   {
     set_color(*label_temp_,"darkred");
     set_color(*label_temp, "darkred");
     set_color(*label_bat1_, "white");
     set_color(*label_bat1,  "white");
     set_color(*label_load_, "darkgreen");
     set_color(*label_load,  "darkgreen");
   }
  get_show();
  Gtk::Main::timeout.connect(slot(this,&gtk_acpi::timeout),refresh);
}

void gtk_acpi::get_show()
{
  get_values();
  show_values();
}

gint gtk_acpi::timeout()
{ 
  get_show();
  return 1;
}


void gtk_acpi::set_color(Gtk::Widget &W,const std::string &color)
{
  GtkRcStyle *rc_style = gtk_rc_style_new ();
  rc_style->fg[GTK_STATE_NORMAL] = Gdk_Color(color);
  rc_style->color_flags[GTK_STATE_NORMAL] = (GtkRcFlags)  ((int) rc_style->color_flags[GTK_STATE_NORMAL] | GTK_RC_FG);
  gtk_widget_modify_style(GTK_WIDGET(W.gtkobj()), rc_style);
  gtk_rc_style_unref(rc_style);
}

 