/* $Id: gtk_acpi.cc,v 1.31 2005/09/21 14:23:13 thoma Exp $ */
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
#include "WindowInfo.hh"

gtk_acpi::gtk_acpi(const FileFinder &_FF,const heatload::st_widget &_show_widget,
                const bool _show_sudo,HeatloadGizmo &_HG,std::vector<heatload::st_auto> &AV)
:
   menu(0),GDA(0),FF(_FF),HG(_HG),AutoVec(AV),
   show_widget(_show_widget),
   show_sudo(_show_sudo)
{
  FF.find(HG);
  HG.battery.load_info_file(FF.getFileName(heatload::eBatInfo));
  HG.cpu_throttling.load_thrott_file(FF.getFileName(heatload::eCPUthrottling));
  HG.cpu_performance_cur_scale.load_max_scale("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq");
  init();
}


void gtk_acpi::init()
{
//  if(!FF.check(false)) return;
  HG.ac_adapter.setFileForValues(FF.getFile(heatload::eAC),Gizmo::st_value(1,2));
  HG.fan.setFileForValues(FF.getFile(heatload::eFan),Gizmo::st_value(1,2));
  HG.thermal.setFileForValues(FF.getFile(heatload::eThermal),Gizmo::st_value(1,2,3));
  HG.battery.setFileForValues(FF.getFile(heatload::eBat),Gizmo::st_value());
  HG.cpu_throttling.setFileForValues(FF.getFile(heatload::eCPUthrottling),Gizmo::st_value(2,3));
  HG.cpu_performance_cur_scale.setFileForValues(FF.getFile(heatload::eCPUperformance_cur_scale),Gizmo::st_value(1,1,Gizmo::invalid));
  HG.cpu_load.setFileForValues(FF.getFile(heatload::eLoad),Gizmo::st_value());

  GDA = manage(new GraphDrawingArea(show_widget.x,show_widget.y,HG));
  frame_draw->add(*GDA);

  label_decoration->set_text("Heatload written by Lennart Poettering (2002)\n"
                             "rewritten and enhanced by Malte Thoma");

   set_color(*label_temp_,HG.thermal.ColorLabel());
   set_color(*label_temp, HG.thermal.ColorLabel());
   set_color(*label_bat1_,HG.battery.ColorLabel());
   set_color(*label_bat1, HG.battery.ColorLabel());
   set_color(*label_load_,HG.cpu_load.ColorLabel());
   set_color(*label_load, HG.cpu_load.ColorLabel());

   if(show_widget.menu) menu_init();

   hide_or_show_elements();
   show_values();
   Gtk::Main::timeout.connect(slot(this,&gtk_acpi::timeout),show_widget.refresh);
   if(show_sudo) show_sudo_error(heatload::eCPUthrottling);
   save();
}

gint gtk_acpi::timeout()
{ 
  show_values();
  test_auto_tp();
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
  if     (ev->keyval=='a') HG.ac_adapter.toggleVisible();
  else if(ev->keyval=='b') HG.battery.toggleVisible();
  else if(ev->keyval=='c') HG.battery.toggleUseMaxCap();
  else if(ev->keyval=='d') show_widget.decoration = !show_widget.decoration;
  else if(ev->keyval=='e') show_widget.label = !show_widget.label;
  else if(ev->keyval=='f') HG.fan.toggleVisible();
  else if(ev->keyval=='g') show_widget.graph = !show_widget.graph;
  else if(ev->keyval=='l') HG.cpu_load.toggleVisible();
  else if(ev->keyval=='p') HG.cpu_performance_cur_scale.toggleVisible();
  else if(ev->keyval=='r') show_values();
  else if(ev->keyval=='s') show_sudo_error(heatload::eCPUthrottling);
  else if(ev->keyval=='t') HG.thermal.toggleVisible();
  else if(ev->keyval=='u') HG.cpu_throttling.toggleVisible();
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

  if(HG.ac_adapter.Visible()) { label_ac_->show(); label_ac->show(); }
  else                     { label_ac_->hide(); label_ac->hide(); }

  if(HG.fan.Visible()) { label_fan_->show(); label_cooling->show(); }
  else              { label_fan_->hide(); label_cooling->hide(); }

  if(HG.battery.Visible()) { label_bat1_->show(); label_bat1->show(); }
  else              { label_bat1_->hide(); label_bat1->hide(); }

  if(HG.cpu_load.Visible()) { label_load_->show(); label_load->show(); }
  else               { label_load_->hide(); label_load->hide(); }

  if(HG.thermal.Visible()) { label_temp_->show(); label_temp->show(); }
  else               { label_temp_->hide(); label_temp->hide(); }

  if(HG.cpu_throttling.Visible()) eventbox_cpu_throttling->show();
  else                         eventbox_cpu_throttling->hide();

  if(HG.cpu_performance_cur_scale.Visible()) eventbox_cpu_performance->show();
  else                          eventbox_cpu_performance->hide();
}

