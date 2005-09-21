/* $Id: gtk_acpi_menu.cc,v 1.16 2005/09/21 14:12:11 thoma Exp $ */
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

#include "gtk_acpi.hh"
#include <fstream>
#include <iostream> // debug
#include <assert.h>

void gtk_acpi::menu_init() 
{
  menu=new Gtk::Menu();
  Gtk::MenuItem *throttling = manage(new class Gtk::MenuItem("CPU throttling"));
  {
  Gtk::Menu *throttling_menu = manage(new class Gtk::Menu());
  throttling->set_submenu(*throttling_menu);
  for(std::vector<GizmoThrottling::st_state>::const_iterator i=HG.cpu_throttling.getVec().begin();i!=HG.cpu_throttling.getVec().end();++i)
   {Gtk::MenuItem *_t = manage(new class Gtk::MenuItem(i->prozent));
     _t->activate.connect(SigC::bind(SigC::slot(this,&gtk_acpi::select_throttling),i->state,heatload::eCPUthrottling));
     throttling_menu->append(*_t);
   }
  }
#if 0
  Gtk::MenuItem *performance = manage(new class Gtk::MenuItem("CPU performance"));
  {
  Gtk::Menu *performance_menu = manage(new class Gtk::Menu());
  performance->set_submenu(*performance_menu);
  for(std::vector<GizmoThrottling::st_state>::const_iterator i=HG.cpu_performance_cur_scale.getVec().begin();i!=HG.cpu_performance_cur_scale.getVec().end();++i)
   {Gtk::MenuItem *_t = manage(new class Gtk::MenuItem(i->prozent));
     _t->activate.connect(SigC::bind(SigC::slot(this,&gtk_acpi::select_throttling),i->state,heatload::eCPUperformance_cur_scale));
     performance_menu->append(*_t);
   }
  }
#endif
  Gtk::MenuItem *suspend = manage(new class Gtk::MenuItem("Suspend"));
  {
  Gtk::Menu *_menu=manage(new class Gtk::Menu());
  suspend->set_submenu(*_menu);
  Gtk::MenuItem *_suspend_sleep = manage(new class Gtk::MenuItem("Suspend to disk"));
  _suspend_sleep->activate.connect(SigC::bind(SigC::slot(this,&gtk_acpi::suspend_activate),true));
  _menu->append(*_suspend_sleep);
  Gtk::MenuItem *_suspend_awake = manage(new class Gtk::MenuItem("Awake"));
  _suspend_awake->activate.connect(SigC::bind(SigC::slot(this,&gtk_acpi::suspend_activate),false));
  _menu->append(*_suspend_awake);
  }

  Gtk::MenuItem *fan = manage(new class Gtk::MenuItem("Fan"));
  {
  Gtk::Menu *_menu=manage(new class Gtk::Menu());
  fan->set_submenu(*_menu);
  Gtk::MenuItem *_m1 = manage(new class Gtk::MenuItem("On"));
  _m1->activate.connect(SigC::bind(SigC::slot(this,&gtk_acpi::fan_activate),true));
  _menu->append(*_m1);
  Gtk::MenuItem *_m2 = manage(new class Gtk::MenuItem("Off"));
  _m2->activate.connect(SigC::bind(SigC::slot(this,&gtk_acpi::fan_activate),false));
  _menu->append(*_m2);
  }

  Gtk::MenuItem *help = manage(new class Gtk::MenuItem("Help"));
  {
  Gtk::Menu *_menu=manage(new class Gtk::Menu());
  help->set_submenu(*_menu);
  Gtk::MenuItem *_m1 = manage(new class Gtk::MenuItem("Help"));
  _m1->activate.connect(SigC::slot(this,&gtk_acpi::show_run_time_options));
  _menu->append(*_m1);
  Gtk::MenuItem *_m2 = manage(new class Gtk::MenuItem("About"));
  _m2->activate.connect(SigC::slot(this,&gtk_acpi::show_about));
  _menu->append(*_m2);
  }

  Gtk::MenuItem *_close = manage(new class Gtk::MenuItem("Exit"));
  _close->activate.connect(SigC::slot(this,&gtk_acpi::ende));

  menu->append(*throttling);
//  menu->append(*performance);
  if(show_widget.swsusp) menu->append(*suspend);
  menu->append(*fan);
  menu->append(*help);
  menu->append(*_close);
  menu->show_all();
}

void gtk_acpi::select_throttling(guint i,const heatload::e_find EF)
{
  std::string com = "sudo /usr/sbin/set_throttling "+itos(i)+" "+FF.getFileName(EF);
  int b=system(com.c_str());
  if(!b) show_values();
  else show_sudo_error(heatload::eCPUthrottling);
  reset_auto_tp();
}

void gtk_acpi::fan_activate(const bool turn_on)
{
  std::string com="sudo /usr/sbin/set_throttling " ;
  if(turn_on) com += itos(0); // 0 = on
  else        com += itos(3); // 3 = off
  com += " "+FF.getFileName(heatload::eFan);  
//cout << com<<'\n';
  int b=system(com.c_str());
  if(!b) show_values();
  else   show_sudo_error(heatload::eFan);
}


void gtk_acpi::suspend_activate(const bool turn_to_sleep)
{
  if(!turn_to_sleep) reset_auto_tp();

  std::string com="sudo ";
  if(turn_to_sleep) com += FF.getFileName(heatload::eSuspend_sleep); 
  else              com += FF.getFileName(heatload::eSuspend_awake); 
  int b=system(com.c_str());
  if(!b) show_values();
  else if(turn_to_sleep) ;//show_sudo_error(heatload::eSuspend_sleep);
  else if(!turn_to_sleep)show_sudo_error(heatload::eSuspend_awake);
  else assert(!"never get here");
}



#include "WindowInfo.hh"

void gtk_acpi::show_sudo_error(const heatload::e_find EF) const
{
   std::string what,file;
   std::string alias="HEATLOAD";

   file = "/usr/sbin/set_throttling, "+FF.getFileName(heatload::eSuspend_sleep)
        +=", "+ FF.getFileName(heatload::eSuspend_awake);
   
   std::string s="To use root-privilege commands (like change throttling or "
       " performance or suspend) check the following:\n\n"
       " * is '"+file+"' installed?'\n"
       " * are the permissons set to '-rwx------' (chmod 700) correctly?\n"
       " * is 'sudo' installed ? (apt-get install sudo for debian)\n"
       " * The following two lines must be inserted into '/etc/sudoers':\n"
       "    Cmnd_Alias      "+alias+"="+file+"\n"
       "    ALL     ALL=NOPASSWD: "+alias+" \n"
       " if you still have problems or ideas tell me <thoma@muenster.de>\n";
   manage(new WindowInfo(this,s,true));
}

void gtk_acpi::show_run_time_options()
{
  std::string s = heatload::run_time_options;
//  s+="\n";

  std::string sx;
  for(std::vector<heatload::st_auto>::const_iterator i=AutoVec.begin();i!=AutoVec.end();++i)
   {
     std::string s1 = FileFinder::Bezeichnung(i->EF);
     for(int j=s1.size();j<20;++j) s1+=" ";
     std::string s2 = "("+itos(i->temperature)+")";
     std::string s3; if(i->state!=-1) s3 = " => "+itos((i->state));
     sx += s1+s2+s3+ '\n';
   }
  manage(new WindowInfo(this,heatload::run_time_options+sx,false));
}


gint gtk_acpi::on_eventbox_main_button_press_event(GdkEventButton *event)
{
   if ((event->type == GDK_BUTTON_PRESS) && menu)
   {  menu->popup(event->button,event->time);
      return true;
   }
   return false;
}

void gtk_acpi::show_about() 
{
  show_widget.decoration=true;
  hide_or_show_elements();
}
