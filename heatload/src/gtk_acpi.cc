// generated 2002/3/18 6:58:50 CET by thoma@Tiger.(none)
// using glademm V0.6.4b_cvs
//
// newer (non customized) versions of this file go to gtk_acpi.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "gtk_acpi.hh"
#include <gtk--/main.h>
#include <fstream>

gtk_acpi::gtk_acpi(const guint x,const guint y,const guint refresh,
   const bool _show_graph,const bool _show_label,const bool _show_decoration,
   const bool _read_max_cap)
: x_size(x),y_size(y),show_graph(_show_graph),show_label(_show_label),
   show_decoration(_show_decoration),read_max_cap_(_read_max_cap),
   use_max_cap(true),GDA(0)
{
  temp_color="darkred";
  bat_color="white";
  load_color="SeaGreen";
  init();
  read_max_cap();
  get_show();
  Gtk::Main::timeout.connect(slot(this,&gtk_acpi::timeout),refresh);
}


void gtk_acpi::read_max_cap()
{
 #warning HACK 
 if(read_max_cap_)
  {
   ifstream fin("/proc/acpi/battery/0/info");
   if(!fin.good()) {fin.close();
                    fin.open("/proc/acpi/battery/BAT1/info");}
   std::string s1,s2;
   fin >> s1 >> s1;
   fin >> s1 >> s1 >> max_cap;
   fin >> s1 >> s1 >> s1 >> last_max_cap;
  }
 else { max_cap=44100; last_max_cap=41000;}
}


void gtk_acpi::init()
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
     set_color(*label_temp_,temp_color);
     set_color(*label_temp, temp_color);
     set_color(*label_bat1_, bat_color);
     set_color(*label_bat1,  bat_color);
     set_color(*label_load_, load_color);
     set_color(*label_load,  load_color);
   }
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

gint gtk_acpi::on_gtk_acpi_delete_event(GdkEventAny *ev)
{
  ende();  
  return false;
}

void gtk_acpi::ende()
{
  Gtk::Main::instance()->quit(); 
}

gint gtk_acpi::on_gtk_acpi_key_press_event(GdkEventKey *ev)
{
//  if(ev->keyval=='q') ende();
  if(ev->keyval=='c') use_max_cap = !use_max_cap;
  return false;
}


