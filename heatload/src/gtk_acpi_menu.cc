/* $Id: gtk_acpi_menu.cc,v 1.4 2002/12/20 07:53:34 thoma Exp $ */
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
  load_thrott_file();
  menu=new Gtk::Menu();
  Gtk::MenuItem *throttling = manage(new class Gtk::MenuItem("CPU throttling"));
  Gtk::Menu *throttling_menu = manage(new class Gtk::Menu());
  throttling->set_submenu(*throttling_menu);
  for(std::vector<st_throttling>::const_iterator i=vec_throttling.begin();i!=vec_throttling.end();++i)
   {
     Gtk::MenuItem *_t = manage(new class Gtk::MenuItem(i->prozent));
     _t->activate.connect(SigC::bind(SigC::slot(this,&gtk_acpi::select_throttling),i->state));
     throttling_menu->append(*_t);
   }
  Gtk::MenuItem *_close = manage(new class Gtk::MenuItem("Exit"));
  _close->activate.connect(SigC::slot(this,&gtk_acpi::ende));

  menu->append(*throttling);
  menu->append(*_close);
  menu->show_all();
}

void gtk_acpi::load_thrott_file()
{
  char c1[100];
//  std::ifstream fin(cpu_thrott_file.name.c_str());
//  std::ifstream fin(FileMap[heatload::eCPUthrottling].name.c_str());
  std::ifstream fin(FF.getFileName(heatload::eCPUthrottling).c_str());
  for(int i=0;i<3;++i) fin.getline(c1,sizeof(c1));  // Kommentarzeilen
  while(fin)
   { fin.getline(c1,sizeof(c1));
     if(!fin.good()) break;
     std::string s1=c1;
     std::string::size_type st1 = s1.find("T");
     std::string::size_type st2 = s1.find_first_of(":");
     std::string::size_type st3 = s1.find_last_of(" ");
     if(st1==std::string::npos||st2==std::string::npos) 
         {cerr<<"Error while reading "<<FF.getFileName(heatload::eCPUthrottling)<<'\n';abort();}
     std::string st(s1,st1,st2-st1);
     std::string sv(s1,st1+1,st2-st1-1);
     std::string sp(s1,st3+1,std::string::npos);
     vec_throttling.push_back(st_throttling(st,atoi(sv.c_str()),sp));
   }
}


void gtk_acpi::select_throttling(guint i)
{
  std::string com = "sudo /usr/sbin/set_throttling "+itos(i)+" "+FF.getFileName(heatload::eCPUthrottling);
  int b=system(com.c_str());
  if(!b) get_show();
  else show_sudo_error();
}

#include "WindowInfo.hh"
void gtk_acpi::show_sudo_error()
{
   std::string s="To use the throttling adjustment, check the following:\n\n"
       " * is '/usr/sbin/set_throttling' installed?'\n"
       " * are the permissons set to '-rwx------' (chmod 700) correctly?\n"
       " * is 'sudo' installed ? (apt-get install sudo for debian)\n"
       " * The following two lines must be inserted into '/etc/sudoers':\n"
       "    Cmnd_Alias      THROTTLING=/usr/sbin/set_throttling\n"
       "    ALL     ALL=NOPASSWD: THROTTLING \n"
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

