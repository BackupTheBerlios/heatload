/* $Id: gtk_acpi_menu.cc,v 1.8 2003/01/05 09:24:24 thoma Exp $ */
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

void gtk_acpi::menu_init() 
{
  menu=new Gtk::Menu();
  Gtk::MenuItem *throttling = manage(new class Gtk::MenuItem("CPU throttling"));
  {
  Gtk::Menu *throttling_menu = manage(new class Gtk::Menu());
  throttling->set_submenu(*throttling_menu);
  for(std::vector<GizmoThrottling::st_state>::const_iterator i=HG.cpu_throttling.getVec().begin();i!=HG.cpu_throttling.getVec().end();++i)
   {Gtk::MenuItem *_t = manage(new class Gtk::MenuItem(i->prozent));
     _t->activate.connect(SigC::bind(SigC::slot(this,&gtk_acpi::select_throttling),i->state));
     throttling_menu->append(*_t);
   }
  }
  Gtk::MenuItem *performance = manage(new class Gtk::MenuItem("CPU performance"));
  {
  Gtk::Menu *performance_menu = manage(new class Gtk::Menu());
  performance->set_submenu(*performance_menu);
  for(std::vector<GizmoThrottling::st_state>::const_iterator i=HG.cpu_performance.getVec().begin();i!=HG.cpu_performance.getVec().end();++i)
   {Gtk::MenuItem *_t = manage(new class Gtk::MenuItem(i->prozent));
     _t->activate.connect(SigC::bind(SigC::slot(this,&gtk_acpi::select_throttling),i->state));
     performance_menu->append(*_t);
   }
  }
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

  Gtk::MenuItem *help = manage(new class Gtk::MenuItem("Help"));
  {
  Gtk::Menu *_menu=manage(new class Gtk::Menu());
  suspend->set_submenu(*_menu);
  Gtk::MenuItem *_help = manage(new class Gtk::MenuItem("Help"));
  _help->activate.connect(SigC::slot(this,&gtk_acpi::show_run_time_options));
  _menu->append(*_help);
  Gtk::MenuItem *_about = manage(new class Gtk::MenuItem("About"));
  _about->activate.connect(SigC::slot(this,&gtk_acpi::show_about));
  _menu->append(*_about);
  }


  Gtk::MenuItem *_close = manage(new class Gtk::MenuItem("Exit"));
  _close->activate.connect(SigC::slot(this,&gtk_acpi::ende));

  menu->append(*throttling);
  menu->append(*performance);
  if(show_widget.swsusp) menu->append(*suspend);
  menu->append(*help);
  menu->append(*_close);
  menu->show_all();
}

void gtk_acpi::select_throttling(guint i)
{
  std::string com = "sudo /usr/sbin/set_throttling "+itos(i)+" "+FF.getFileName(heatload::eCPUthrottling);
  int b=system(com.c_str());
  if(!b) show_values();
  else show_sudo_error(heatload::eCPUthrottling);
}

void gtk_acpi::suspend_activate(const bool turn_to_sleep)
{
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
   manage(new WindowInfo(this,heatload::run_time_options,false));
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
