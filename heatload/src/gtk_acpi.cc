/* $Id: gtk_acpi.cc,v 1.24 2002/12/20 22:12:05 thoma Exp $ */
/*  Copyright (C) 2001 Malte Thoma
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
// newer (non customized) versions of this file go to gtk_acpi.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "gtk_acpi.hh"
#include <gtk--/main.h>
//#include <fstream>
#include "RC.hh"
#include "WindowInfo.hh"

gtk_acpi::gtk_acpi(const FileFinder::FileMap_t &_FileMap,
         const heatload::st_widget &_show_widget,
         const bool _read_max_cap,const bool _show_sudo,
         const heatload::st_show &_show_what,const heatload::st_color &_color)
:
   menu(0),GDA(0),FF(FileFinder(this,_FileMap)),
   show_widget(_show_widget),
   show_what(_show_what),color(_color),show_sudo(_show_sudo),
   battery(GizmoBattery(FF.getFileName(heatload::eBatInfo),_read_max_cap))
{
  init();
}


void gtk_acpi::init()
{
  if(!FF.check(false)) return;

  GDA = manage(new GraphDrawingArea(show_widget.x,show_widget.y,color));
  frame_draw->add(*GDA);

  label_decoration->set_text("Heatload written by Lennart Poettering (2002)\n"
                             "rewritten and enhanced by Malte Thoma");

   set_color(*label_temp_,color.temp_label);
   set_color(*label_temp, color.temp_label);
   set_color(*label_bat1_,color.bat_label);
   set_color(*label_bat1, color.bat_label);
   set_color(*label_load_,color.load_label);
   set_color(*label_load, color.load_label);

   if(show_widget.menu) menu_init();

   hide_or_show_elements();
   get_show();
   Gtk::Main::timeout.connect(slot(this,&gtk_acpi::timeout),show_widget.refresh);
   if(show_sudo) show_sudo_error();
   save();
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

void gtk_acpi::save() const
{
  rc_file::save(show_what,color,show_widget,battery.ReadMaxCap(),show_sudo,FF.getFileMap());
}


gint gtk_acpi::on_gtk_acpi_key_press_event(GdkEventKey *ev)
{
//  if(ev->keyval=='q') ende();
  if     (ev->keyval=='a') show_what.ac=!show_what.ac;
  else if(ev->keyval=='b') show_what.bat=!show_what.bat;
  else if(ev->keyval=='c') battery.toggleUseMaxCap();
  else if(ev->keyval=='d') show_widget.decoration = !show_widget.decoration;
  else if(ev->keyval=='e') show_widget.label = !show_widget.label;
  else if(ev->keyval=='f') show_what.fan=!show_what.fan;
  else if(ev->keyval=='g') show_widget.graph = !show_widget.graph;
  else if(ev->keyval=='l') show_what.load=!show_what.load;
  else if(ev->keyval=='p') show_what.cpu_performance=!show_what.cpu_performance;
  else if(ev->keyval=='r') get_show();
  else if(ev->keyval=='s') show_sudo_error();
  else if(ev->keyval=='t') show_what.temp=!show_what.temp;
  else if(ev->keyval=='u') show_what.cpu_throttling=!show_what.cpu_throttling;
  else if(ev->keyval=='?') show_run_time_options();
  hide_or_show_elements();
  save();
  return false;
}

void gtk_acpi::hide_or_show_elements()
{
  if(show_widget.decoration) label_decoration->show();
  else                       label_decoration->hide();
  if(show_widget.graph)      GDA->show();
  else                       GDA->hide();
  if(show_widget.label)      frame_label->show();
  else                       frame_label->hide();

  if(show_what.ac) { label_ac_->show(); label_ac->show(); }
  else             { label_ac_->hide(); label_ac->hide(); }

  if(show_what.fan) { label_fan_->show(); label_cooling->show(); }
  else              { label_fan_->hide(); label_cooling->hide(); }

  if(show_what.bat) { label_bat1_->show(); label_bat1->show(); }
  else              { label_bat1_->hide(); label_bat1->hide(); }

  if(show_what.load) { label_load_->show(); label_load->show(); }
  else               { label_load_->hide(); label_load->hide(); }

  if(show_what.temp) { label_temp_->show(); label_temp->show(); }
  else               { label_temp_->hide(); label_temp->hide(); }

  if(show_what.cpu_throttling) eventbox_cpu_throttling->show();
  else                         eventbox_cpu_throttling->hide();

  if(show_what.cpu_performance) eventbox_cpu_performance->show();
  else                          eventbox_cpu_performance->hide();
}

