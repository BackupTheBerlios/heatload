// generated 2002/3/18 6:58:49 CET by thoma@Tiger.(none)
// using glademm V0.6.4b_cvs
//
// newer (non customized) versions of this file go to gtkacpi.cc_new

// This file is for your program, I won't touch it again!

#include <gtk--/main.h>
#include "gtk_acpi.hh"
#include <getopt.h>
#include "FileFinder.hh"
#include "Gizmo.hh"
#include "RC.hh"
#include <config.h>
#include <iostream>

void show_version()
{
   std::cout << "\n This is heatload "<<VERSION<<"\n\n";
   exit(0);
}

void usage(const std::string &name)
{
  std::cerr << "\n  heatload is written by Malte Thoma (thoma@muenster.de),\n"
          "  Gtk::DrawingArea-code by Lennart Poettering,\n"
          "  XML-code by Christof Petig\n\n"
          "  heatload is free software under the terms of the GNU General Public License\n\n"
               "  Usage: "<<name<<" [OPTION]... \n"
               "  [OPTION]... can be a combination of the following items: \n"
               " -?, --help               display this help message and exit\n"
               " -c, --color X           X = 'widget=color' where widget is on of\n"
               "                            battery_label battery_meter \n"
               "                            thermal_label thermal_meter\n"
               "                            load_label    load_meter\n"
               "                          and color is named in '/usr/X11R6/lib/X11/rgb.txt'\n"
               "                          use this Option as often as you need it.\n"
               " -d, --decoration         toggles show/hide decoration\n"
               " -g, --graph              toggles graphical display\n"
               " -h , --hide [OPTION]     OPTION must be one of\n"
               "                            ac battery thermal load fan swsusp\n"
               "                            cpu_throttling cpu_performance\n"
               "                          can be given as often as you need ist\n"
               " -l, --label              toggles numerical thermal and load display\n"
               " -m  , --read_max_capacity  This is a HACK: If you use this\n"
               "                     option the max_cap will be read from\n"
               "                     /proc/acpi/battery/BAT1/info (as it should be).\n"
               "                     If this option is NOT given the max_cap will be 44100 mWh\n"
               "                     this is the max_cap on an Sony-Vaio. Because of a buggy \n"
               "                     BIOS the reading of the info file freezes the keybord on\n"
               "                     this laptop. If you don't have a Sony Vaio it should be\n"
               "                     save (and good idea :-) to use this option.\n\n"
               " -r X, --refresh X        X = refresh rate [2500]\n"
               " -t  , --show_throttling_hint   pop up the throttling hint window\n"
               " -x X, --x_size X         X = x-size of meter [100]\n"
               " -y X, --y_size X         X = y-size of meter [70]\n\n"
               "    run-time-options:\n"
               + heatload::run_time_options+
               " command-line options override the default settings in \n"
               "  './.heatloadrc', '~/.heatloadrc' resp. '/etc/heatload/heatload.conf'\n\n"
               " within the 'Auto'-Tag in '.heatloadrc' auto-throttling and/or \n"
               " auto-performance can be adjusted to prevent overheating\n\n"
               ;
  exit(1);
}

void evaluate(const std::string &s,HeatloadGizmo &HG,heatload::st_widget &show_widget)
{
  if     (s=="ac")      HG.ac_adapter.toggleVisible()  ;
  else if(s=="battery") HG.battery.toggleVisible() ; 
  else if(s=="thermal") HG.thermal.toggleVisible() ; 
  else if(s=="load")    HG.cpu_load.toggleVisible() ; 
  else if(s=="fan")     HG.fan.toggleVisible() ; 
  else if(s=="cpu_throttling")   HG.cpu_throttling.toggleVisible() ; 
  else if(s=="cpu_performance")  HG.cpu_performance.toggleVisible() ; 
  else if(s=="swsusp")  show_widget.swsusp=!show_widget.swsusp;
  else {cerr << s<<" unknown \n";}
}

void change_color(const std::string &s,HeatloadGizmo &HG,const std::string &name)
{
 cout << s<<'\n';
 std::string::size_type st=s.find("=");
 if(st==std::string::npos) {std::cerr << "Wrong color: "<<s<<'\n'; usage(name);}
 std::string c(s,0,st);
 std::string w(s,st+1);
 if     (c=="battery_label") HG.battery.setColorLabel(w);
 else if(c=="battery_meter") HG.battery.setColorMeter(w);
 else if(c=="thermal_label") HG.thermal.setColorLabel(w);
 else if(c=="thermal_meter") HG.thermal.setColorMeter(w);
 else if(c=="load_label")    HG.cpu_load.setColorLabel(w);
 else if(c=="load_meter")    HG.cpu_load.setColorMeter(w);
 else  {std::cerr << "Wrong color: "<<s<<'\n'; usage(name);}
}


 
const static struct option options[]=
{{ "decoration", no_argument,      NULL, 'd' },
 { "label", no_argument,      NULL, 'l' },
 { "graph", no_argument,      NULL, 'g' },
 { "help", no_argument,      NULL, '?' },
 { "read_max_capacity", required_argument,      NULL, 'm' },   
 { "refresh", required_argument,      NULL, 'r' },   
 { "show_throttling_hint",no_argument,      NULL, 't' },
 { "x_size", required_argument,      NULL, 'x' },    
 { "y_size", required_argument,      NULL, 'y' },    
 { "hide", required_argument,      NULL, 'h' },    
 { "color", required_argument,      NULL, 'c' },    
 { "version", no_argument,      NULL, 'v' },    
 { NULL,      0,       NULL, 0 }
};
 

int main(int argc, char **argv)
{  
    int opt;
    bool show_sudo=true;
    heatload::st_widget show_widget;
    FileFinder FF;
    HeatloadGizmo HG;
    std::vector<heatload::st_auto> AutoVec;
    rc_file::load(HG,show_widget,show_sudo,FF,AutoVec);
                                                            
    while ((opt=getopt_long(argc,argv,"c:dlfgh:mr:tvx:y:?",options,NULL))!=EOF)
     {
      switch(opt) {
         case 'd' : show_widget.decoration=!show_widget.decoration; break;
         case 'l' : show_widget.label= !show_widget.label; break;
         case 'g' : show_widget.graph= !show_widget.graph; break;
         case 'm' : HG.battery.setReadMaxCap(true); break;
         case 'r' : show_widget.refresh=atoi(optarg); break;   
         case 't' : show_sudo=true; break;   
         case 'x' : show_widget.x=atoi(optarg); break;
         case 'y' : show_widget.y=atoi(optarg); break;
         case 'h' : evaluate(optarg,HG,show_widget); break;
         case 'c' : change_color(optarg,HG,argv[0]); break;
         case 'v' : show_version(); break;
         default : usage(argv[0]);
       }
     }  
   
   Gtk::Main m(&argc, &argv);
   manage(new class gtk_acpi(FF,show_widget,show_sudo,HG,AutoVec));
   m.run();
   return 0;
}
