// generated 2002/3/18 6:58:50 CET by thoma@Tiger.(none)
// using glademm V0.6.4b_cvs
//
// newer (non customized) versions of this file go to gtk_acpi.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "gtk_acpi.hh"
#include <gtk--/main.h>
#include <fstream>
#include "RC.hh"

gtk_acpi::gtk_acpi(const heatload::st_widget &_show_widget,const bool _read_max_cap,
         const heatload::st_show &_show_what,const heatload::st_color &_color)
: show_widget(_show_widget),read_max_cap_(_read_max_cap),
   show_what(_show_what),color(_color),use_max_cap(true),GDA(0)
{
  init();
  hide_or_show_elements();
  read_max_cap();
  get_show();
  Gtk::Main::timeout.connect(slot(this,&gtk_acpi::timeout),show_widget.refresh);
}


void gtk_acpi::read_max_cap()
{
 #warning HACK 
 if(read_max_cap_)
  {
   ifstream fin("/proc/acpi/battery/0/info");
   if(!fin.good()) {
      fin.close();
      fin.open("/proc/acpi/battery/BAT0/info");
      if(!fin.good()) {
         fin.close();
         fin.open("/proc/acpi/battery/BAT1/info"); }
      if(!fin.good()) 
         {cerr << "Sorry can't open 'battery info' in /proc/acpi\n"; exit(1);}
     }
   std::string s1,s2;
   fin >> s1 >> s1;
   fin >> s1 >> s1 >> max_cap;
   fin >> s1 >> s1 >> s1 >> last_max_cap;
  }
 else { max_cap=44100; last_max_cap=41000;}
}


void gtk_acpi::find_filenames()
{
  bool alles_ok=true;
  std::vector<st_find_filename> F;
  F.push_back(st_find_filename("/proc/acpi/ac_adapter/AC0/state"));
  F.push_back(st_find_filename("/proc/acpi/ac_adapter/ACAD/state"));
  F.push_back(st_find_filename("/proc/acpi/ac_adapter/0/status"));
  bool ok=find_filename(eAC,F);
  if(!ok) alles_ok=ok;

  F.clear(); 
  F.push_back(st_find_filename("/proc/acpi/battery/BAT0/state"));
  F.push_back(st_find_filename("/proc/acpi/battery/BAT1/state"));
  F.push_back(st_find_filename("/proc/acpi/battery/0/status",true));
  ok=find_filename(eBat,F);
  if(!ok) alles_ok=ok;

  F.clear(); 
  F.push_back(st_find_filename("/proc/acpi/thermal_zone/THRM/temperature"));
  F.push_back(st_find_filename("/proc/acpi/thermal/0/status",true));
  ok=find_filename(eThermal,F);
  if(!ok) alles_ok=ok;
  if(!alles_ok) abort();
  cout <<"Reading from \n\t"<<ac_file.name<<"\n\t"<<bat_file.name
       <<"\n\t"<<therm_file.name<<'\n';
}

bool gtk_acpi::find_filename(const e_find EF,const std::vector<st_find_filename> &F)
{
  ifstream fin;
  for(std::vector<st_find_filename>::const_iterator i=F.begin();i!=F.end();++i)
   {
     fin.close();
     fin.open(i->name.c_str());
     if(fin.good())
      {
        switch (EF) {
           case eAC:     ac_file=*i   ; return true;
           case eBat:    bat_file=*i  ; return true;
           case eThermal:therm_file=*i; return true;
         }
      }
   }
  // Fehler
  cerr << " Sorry can't open one of the following files:\n";
  for(std::vector<st_find_filename>::const_iterator i=F.begin();i!=F.end();++i)
     cerr << '\t'<<i->name<<'\n';
  cerr << "\n";
  return false;
}


void gtk_acpi::init()
{
  find_filenames();
  if(show_widget.graph)
   {
     GDA = manage(new GraphDrawingArea(show_widget.x,show_widget.y,color));
     frame_draw->add(*GDA);
     GDA->show();
   }
  if(show_widget.decoration) label_decoration->set_text("Heatload written by Lennart Poettering (2002)\n"
                  "rewritten and enhanced by Malte Thoma");
  else label_decoration->hide();
  if(!show_widget.label) frame_label->hide();
  else 
   {
     set_color(*label_temp_,color.temp_label);
     set_color(*label_temp, color.temp_label);
     set_color(*label_bat1_,color.bat_label);
     set_color(*label_bat1, color.bat_label);
     set_color(*label_load_,color.load_label);
     set_color(*label_load, color.load_label);
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
  if     (ev->keyval=='c') use_max_cap = !use_max_cap;
  else if(ev->keyval=='r') get_show();
  else if(ev->keyval=='f') show_what.fan=!show_what.fan;
  else if(ev->keyval=='t') show_what.temp=!show_what.temp;
  else if(ev->keyval=='b') show_what.bat=!show_what.bat;
  else if(ev->keyval=='l') show_what.load=!show_what.load;
  else if(ev->keyval=='a') show_what.ac=!show_what.ac;
  hide_or_show_elements();
//  rc_file::save(show_what,color,show_widget,read_max_cap);
   rc_file::save(show_what,color,show_widget,read_max_cap_);

  return false;
}

void gtk_acpi::hide_or_show_elements()
{
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
}

