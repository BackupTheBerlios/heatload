/*  heatload
 *  Copyright (C) 2002 , written by Lennart Poettering and Malte Thoma
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

#include "MeterStruct.hh"
#include "GraphDrawingArea.hh"
#include "MainWindow.hh"

#include <getopt.h>

#include <gnome--/main.h>
#include <gnome--/messagebox.h>

#include <unistd.h>

MeterStruct load_meter;
MeterStruct temperature_meter;
MeterStruct battery_meter;
MainWindow *window=0;

bool failed = false;

void failure(const string &txt) {
    static Gnome::MessageBox msgbox("Could not read data from /proc file system:\n"+txt, GNOME_MESSAGE_BOX_ERROR);
    static bool running = false;

    if (running)
        return;

    failed = true;
    running = true;

    msgbox.append_button(GNOME_STOCK_BUTTON_OK);
    
    msgbox.run_and_close();
}

guint8 get_load_value() {
    char txt[256];
    char*p;
    float v;
    static int ct[2][4] = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
    static int n = 0;
    int d[4];
    
    static FILE*f = fopen("/proc/stat", "r");

    if (!f) {
        failure("Unable to open kernel statistics file.");
        return 0;
    }
    
    if (!(p = fgets(txt, sizeof(txt), f))) {
        failure("Unable to read from kernel statistics file.");
        fclose(f);
        f = NULL;
        return 0;
    }
    
    int fd = dup(fileno(f));
    fclose(f);
    f = fdopen(fd, "r");
    g_assert(f);
    fseek(f, 0, SEEK_SET);

    if (strlen(p) <= 5)
        return 0;
        
    sscanf(p+5, "%u %u %u %u", &ct[n][0], &ct[n][1], &ct[n][2], &ct[n][3]);

    int t = 0;
    
    for (int i = 0; i < 4; i++)
        t += (d[i] = abs(ct[n][i] - ct[1-n][i]));

    v = (t - d[3])/(float) t;
    
    n = 1-n;

    return (guint8) (v*100);
}

guint8 get_temperature_value() {
    char txt[256];
    char*p;
    int v;
    static FILE*f = NULL;
    static gboolean old_acpi = FALSE;

    if (!f)
        f = fopen("/proc/acpi/thermal_zone/THRM/temperature", "r");

    if (!f) {
        f = fopen("/proc/acpi/thermal/0/status", "r");
        old_acpi = TRUE;
    }

    if (!f) {
        failure("Unable to open ACPI temperature file.");
        return 0;
    }

    if (!(p = fgets(txt, sizeof(txt), f))) {
        failure("Unable to read data from ACPI temperature file.");
        fclose(f);
        f = NULL;
        return 0;
    }
    
    int fd = dup(fileno(f));
    fclose(f);
    f = fdopen(fd, "r");
    g_assert(f);
    fseek(f, 0, SEEK_SET);

    if (!old_acpi) {
        if (strlen(p) > 20)
            v = atoi(p+20);
        else
            v = 0;
    } else {
        if (strlen(p) > 15)
            v = atoi(p+15);
        else
            v = 0;
        v=((v-2732)/10); /* convert from deciKelvin to degrees Celcius */
    }

    if (v > 100) v = 100;
    if (v < 0) v = 0;

    return (guint8) v;
}


guint8 get_battery_value() 
{
#if 0
    char txt[256];
    char*p;
    int v;
    static FILE*f = NULL;
    static gboolean old_acpi = FALSE;

    if (!f)
        f = fopen("/proc/acpi/thermal_zone/THRM/temperature", "r");

    if (!f) {
        f = fopen("/proc/acpi/thermal/0/status", "r");
        old_acpi = TRUE;
    }

    if (!f) {
        failure("Unable to open ACPI temperature file.");
        return 0;
    }

    if (!(p = fgets(txt, sizeof(txt), f))) {
        failure("Unable to read data from ACPI temperature file.");
        fclose(f);
        f = NULL;
        return 0;
    }
    
    int fd = dup(fileno(f));
    fclose(f);
    f = fdopen(fd, "r");
    g_assert(f);
    fseek(f, 0, SEEK_SET);

    if (!old_acpi) {
        if (strlen(p) > 20)
            v = atoi(p+20);
        else
            v = 0;
    } else {
        if (strlen(p) > 15)
            v = atoi(p+15);
        else
            v = 0;
        v=((v-2732)/10); /* convert from deciKelvin to degrees Celcius */
    }

    if (v > 100) v = 100;
    if (v < 0) v = 0;

    return (guint8) v;
#endif
  return 33;
}


void get_values(const guint x_size) 
{
    guint8 v1, v2, v3;

    if (failed)
        return;
    
    load_meter.add_value(v1 = get_load_value(),x_size);
    temperature_meter.add_value(v2 = get_temperature_value(),x_size);
    battery_meter.add_value(v3 = get_battery_value(),x_size);

    if (window) {
        window->update_values(v1, v2);
        window->refresh();
    }
}

gint timer_proc(const guint x_size) 
{
    get_values(x_size);

    if (failed) {
        Gnome::Main::quit();
        return false;
    }

    return true;
}

void usage(const std::string &name)
{
  std::cerr << "\n  heatload is written by Lennart Poettering (until 0.2),\n"
	       "  enhanced (since 0.3) by Malte Thoma (thoma@muenster.de)\n"
	       "  heatload is free software under the terms of the GNU General Public License\n\n"
               "  Usage: "<<name<<" [OPTION]... \n"
               "  [OPTION]... can be a combination of the following items: \n"
               " -d, --no_decoration      no decoration\n"
               " -f X, --frame_size X     X = border-size sourounding boxes [5]\n"
               " -g, --no_graph           no graphical display\n"
               " -h, --help               display this help message and exit\n"
               " -l, --no_label           no numerical display\n"
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
 { "frame_size", required_argument,      NULL, 'f' },
 { "refresh", required_argument,      NULL, 'r' },
 { "x_size", required_argument,      NULL, 'x' },
 { "y_size", required_argument,      NULL, 'y' },
 { NULL,      0,       NULL, 0 }
};
       

int main(int argc, char *argv[]) {
    int opt;
    bool show_decoration=true;
    bool show_label=true;
    bool show_graph=true;
    int frame_size=5;
    guint x=256;
    guint y=100;
    guint refresh=1000;
    while ((opt=getopt_long(argc,argv,"dlfr:x:y:h?",options,NULL))!=EOF)
     {
      switch(opt) {
         case 'd' : show_decoration=false; break;
         case 'l' : show_label= false; break;
         case 'f' : frame_size=atoi(optarg); break;
         case 'g' : show_graph=false; break;
         case 'r' : refresh=atoi(optarg); break;
         case 'x' : x=atoi(optarg); break;
         case 'y' : y=atoi(optarg); break;
         default : usage(argv[0]);
       }
     }

    gchar *c[]={{argv[0]}}; // HACK
    Gnome::Main kit("Heatload" , "0.1", 1, c); // HACK

    get_values(x);

    if (failed)
        return 1;
    
    window = new MainWindow(show_decoration,show_label,frame_size,x,y);

    SigC::Slot0<gint> sl = SigC::bind(SigC::slot(timer_proc),x);
    Gnome::Main::timeout.connect(sl, refresh);

    kit.run();
    delete window;

    return 0;
}

