// generated 2002/3/18 6:58:49 CET by thoma@Tiger.(none)
// using glademm V0.6.4b_cvs
//
// newer (non customized) versions of this file go to gtkacpi.cc_new

// This file is for your program, I won't touch it again!

#include <gtk--/main.h>
#include "gtk_acpi.hh"
#include <getopt.h>

void usage(const std::string &name)
{
  std::cerr << "\n  heatload is written by Malte Thoma (thoma@muenster.de),\n"
          "  based upon code of Lennart Poettering\n"
          "  heatload is free software under the terms of the GNU General Public License\n\n"
               "  Usage: "<<name<<" [OPTION]... \n"
               "  [OPTION]... can be a combination of the following items: \n"
               " -c, --color X           X = 'widget=color' where widget is on of\n"
               "                            battery_label battery_meter \n"
               "                            thermal_label thermal_meter\n"
               "                            load_label    load_meter\n"
               "                          and color is named in '/usr/X11R6/lib/X11/rgb.txt'\n"
               "                          use this Option as often as you need it.\n"
               " -d, --no_decoration      no decoration\n"
//               " -f X, --frame_size X     X = border-size sourounding boxes [5]\n"
               " -g, --no_graph           no graphical display\n"
               " -h, --help               display this help message and exit\n"
               " -l, --no_label           no numerical temp. and load display\n"
               " -r X, --refresh X        X = refresh rate [1000]\n"
               " -x X, --x_size X         X = x-size of meter [256]\n"
               " -y X, --y_size X         X = y-size of meter [100]\n\n"
               " -h , --hide [OPTION]     OPTION must be one of\n"
               "                            ac battery thermal load fan\n"
               "                          can be given as often as you need ist\n"
               " -m  , --read_max_capacity  This is a HACK: If you use this\n"
               "                     option the max_cap will be read from\n"
               "                     /proc/acpi/battery/BAT1/info (as it should be).\n"
               "                     If this option is NOT given the max_cap will be 44100 mWh\n"
               "                     this is the max_cap on an Sony-Vaio. Because of a buggy \n"
               "                     BIOS the reading of the info file freezes the keybord on\n"
               "                     this laptop. If you don't have a Sony Vaio it should be\n"
               "                     save to use this option.\n\n"
               "    run-time-options:\n"
               "    c        Toggles between designed- and last-max-capacity\n"
               "              when calculating the batterys fill-percentage\n\n"
               "    r        Immediate reload\n"
               "    f        toggle show/hide fan\n"
               "    t        toggle show/hide thermal\n"
               "    b        toggle show/hide battery\n"
               "    l        toggle show/hide load\n"
               "    a        toggle show/hide ac\n"
               ;
  exit(1);
}

void evaluate(const std::string &s,heatload::st_show &show_what)
{
  if     (s=="ac")      show_what.ac=false  ;
  else if(s=="battery") show_what.bat=false ; 
  else if(s=="thermal") show_what.temp=false ; 
  else if(s=="load")    show_what.load=false ; 
  else if(s=="fan")     show_what.fan=false ; 
  else {cerr << s<<" unknown \n";}
}

void change_color(const std::string &s,heatload::st_color &color,const std::string &name)
{
 cout << s<<'\n';
 std::string::size_type st=s.find("=");
 if(st==std::string::npos) {std::cerr << "Wrong color: "<<s<<'\n'; usage(name);}
 std::string c(s,0,st);
 std::string w(s,st+1);
 if     (c=="battery_label") color.bat_label=w;
 else if(c=="battery_meter") color.bat_meter=w;
 else if(c=="thermal_label") color.temp_label=w;
 else if(c=="thermal_meter") color.temp_meter=w;
 else if(c=="load_label")    color.load_label=w;
 else if(c=="load_meter")    color.load_meter=w;
 else  {std::cerr << "Wrong color: "<<s<<'\n'; usage(name);}
}

 
const static struct option options[]=
{{ "no_decoration", no_argument,      NULL, 'd' },
 { "no_label", no_argument,      NULL, 'l' },
 { "no_graph", no_argument,      NULL, 'g' },
 { "help", no_argument,      NULL, 'h' },
// { "frame_size", required_argument,      NULL, 'f' },
 { "read_max_capacity", required_argument,      NULL, 'm' },   
 { "refresh", required_argument,      NULL, 'r' },   
 { "x_size", required_argument,      NULL, 'x' },    
 { "y_size", required_argument,      NULL, 'y' },    
 { "hide", required_argument,      NULL, 'h' },    
 { "color", required_argument,      NULL, 'c' },    
 { NULL,      0,       NULL, 0 }
};
 
  

int main(int argc, char **argv)
{  
    int opt;
    bool show_decoration=true;
    bool show_label=true;
    bool show_graph=true;
    bool read_max_cap=false;
//    int frame_size=5;
    guint x=100;
    guint y=50;
    guint refresh=2500;
    heatload::st_show show_what;
    heatload::st_color color;
    while ((opt=getopt_long(argc,argv,"c:dlfghmr:x:y:h?",options,NULL))!=EOF)
     {
      switch(opt) {
         case 'd' : show_decoration=false; break;
         case 'l' : show_label= false; break;
//         case 'f' : frame_size=atoi(optarg); break;
         case 'g' : show_graph=false; break;
         case 'm' : read_max_cap=true; break;
         case 'r' : refresh=atoi(optarg); break;   
         case 'x' : x=atoi(optarg); break;
         case 'y' : y=atoi(optarg); break;
         case 'h' : evaluate(optarg,show_what); break;
         case 'c' : change_color(optarg,color,argv[0]); break;
         default : usage(argv[0]);
       }
     }  
   
   Gtk::Main m(&argc, &argv);
   manage(new class gtk_acpi(x,y,refresh,show_graph,show_label,show_decoration,read_max_cap,show_what,color));
   m.run();
   return 0;
}
