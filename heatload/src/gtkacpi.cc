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
  std::cerr << "\n  heatload is written by Lennart Poettering (until 0.2),\n"
          "  enhanced (since 0.3) by Malte Thoma (thoma@muenster.de)\n"
          "  heatload is free software under the terms of the GNU General Public License\n\n"
               "  Usage: "<<name<<" [OPTION]... \n"
               "  [OPTION]... can be a combination of the following items: \n"
               " -d, --no_decoration      no decoration\n"
//               " -f X, --frame_size X     X = border-size sourounding boxes [5]\n"
               " -g, --no_graph           no graphical display\n"
               " -h, --help               display this help message and exit\n"
               " -l, --no_label           no numerical temp. and load display\n"
               " -r X, --refresh X        X = refresh rate [1000]\n"
               " -x X, --x_size X         X = x-size of meter [256]\n"
               " -y X, --y_size X         X = y-size of meter [100]\n\n"
               ;
  exit(1);
}

 
const static struct option options[]=
{{ "no_decoration", no_argument,      NULL, 'd' },
 { "no_label", no_argument,      NULL, 'l' },
 { "no_graph", no_argument,      NULL, 'g' },
 { "help", no_argument,      NULL, 'h' },
// { "frame_size", required_argument,      NULL, 'f' },
 { "refresh", required_argument,      NULL, 'r' },   
 { "x_size", required_argument,      NULL, 'x' },    
 { "y_size", required_argument,      NULL, 'y' },    
 { NULL,      0,       NULL, 0 }
};
  

int main(int argc, char **argv)
{  
    int opt;
    bool show_decoration=true;
    bool show_label=true;
    bool show_graph=true;
//    int frame_size=5;
    guint x=100;
    guint y=50;
    guint refresh=1000;
    while ((opt=getopt_long(argc,argv,"dlfgr:x:y:h?",options,NULL))!=EOF)
     {
      switch(opt) {
         case 'd' : show_decoration=false; break;
         case 'l' : show_label= false; break;
//         case 'f' : frame_size=atoi(optarg); break;
         case 'g' : show_graph=false; break;
         case 'r' : refresh=atoi(optarg); break;   
         case 'x' : x=atoi(optarg); break;
         case 'y' : y=atoi(optarg); break;
         default : usage(argv[0]);
       }
     }  
   
   Gtk::Main m(&argc, &argv);
   manage(new class gtk_acpi(x,y,refresh,show_graph,show_label,show_decoration));
   m.run();
   return 0;
}
