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
#include <gtk--/main.h>
#include <gtk--/window.h>
#include <gtk--/label.h>
#include <gtk--/box.h>
#include <gtk--/drawingarea.h>
#include <gtk--/button.h>
#include <gtk--/style.h>
#include <gtk--/frame.h>
#include <getopt.h>

#include <gdk--/drawable.h>

#include <gtk/gtkrc.h>

#include <gnome--/main.h>
#include <gnome--/messagebox.h>

#include <deque.h>

#include <stdio.h>
#include <unistd.h>

//#define METER_WIDTH 256
//#define METER_HEIGHT 100

class MeterStruct {
    
protected:
    deque<guint8> data_deque;
    glong last_time;
    bool modified;

public:
    MeterStruct() : last_time(0), modified(true) {};

    void add_value(guint8 b,const guint x_size);
    bool is_modified() { return modified; }
    void reset_modified() { modified = false; }

    void draw(Gdk_Drawable &d, Gdk_GC &g);
};

void MeterStruct::add_value(guint8 b,const guint x_size) {
    GTimeVal tv;
    glong n;

    g_get_current_time(&tv);

    if (last_time == 0)
        n = 1;
    else if (last_time == tv.tv_sec) {
        data_deque.pop_front();
        n = 1;
    } else
        n = tv.tv_sec - last_time;

    last_time = tv.tv_sec;
        
    for (; n > 0; n --)
        data_deque.push_back(b);

//    while (data_deque.size() > METER_WIDTH)
    while (data_deque.size() > x_size)
        data_deque.pop_front();

    modified = true;
}

void MeterStruct::draw(Gdk_Drawable &d, Gdk_GC &g) {
    gint lx = -1, ly = -1;
    deque<guint8> copy = data_deque;

    for (gint x = d.width()-1; x >= 0; x--) {
        if (copy.size() == 0)
            break;

        guint8 b = copy.back();
        gint y = (100-b)*(d.height()-1)/100;

        if (lx == -1)
            d.draw_point(g, x, y);
        else
            d.draw_line(g, lx, ly, x, y);

        lx = x;
        ly = y;
        
        copy.pop_back();

    }
}

MeterStruct load_meter;
MeterStruct temperature_meter;

class GraphDrawingArea : public Gtk::DrawingArea {
    
   private:
    guint x_size,y_size;
public:
    GraphDrawingArea(const int x,const int y);

    virtual gint expose_event_impl(GdkEventExpose* event);

    void refresh_pixmap();

protected:
    Gdk_Pixmap pixmap;
    
    Gdk_Color color0;
    Gdk_Color color1;
    Gdk_Color color2;
};

GraphDrawingArea::GraphDrawingArea(const int x,const int y): 
    Gtk::DrawingArea(), x_size(x),y_size(y),
    color0("black"), color1("green"), color2("tomato") {
    set_usize(x_size,y_size);
}

gint GraphDrawingArea::expose_event_impl(GdkEventExpose* event) {

    if (!pixmap.connected())
        refresh_pixmap();
    
    if (pixmap.connected()) {
        Gdk_Window win = get_window();
        Gdk_GC gc = get_style()->get_fg_gc(get_state());
        win.draw_pixmap(gc, pixmap, 0, 0, 0, 0);
    }
        
    return TRUE;
}

void GraphDrawingArea::refresh_pixmap() {

    if (!pixmap.connected() || load_meter.is_modified() || temperature_meter.is_modified()) {
        load_meter.reset_modified();
        temperature_meter.reset_modified();

        if (!pixmap.connected()) {
            pixmap = Gdk_Pixmap(get_window(), x_size, y_size);
            get_window().get_colormap().alloc(color0);
            get_window().get_colormap().alloc(color1);
            get_window().get_colormap().alloc(color2);
        }
        
        Gdk_GC gc(pixmap);

        gc.set_foreground(color0);
        pixmap.draw_rectangle(gc, TRUE, 0, 0, pixmap.width(), pixmap.height());
        
        gc.set_foreground(color1);
        load_meter.draw(pixmap, gc);

        gc.set_foreground(color2);
        temperature_meter.draw(pixmap, gc);
    }
}

    
class MainWindow : public Gtk::Window {

protected:

    Gtk::VBox vbox;
    Gtk::Label label;
    Gtk::Label cpu_label, temperature_label;
    Gtk::Frame frame;
    GraphDrawingArea drawing_area;
    Gtk::HBox hbox, hbox2;
    Gtk::Button refresh_button;
    Gtk::Button quit_button;

public:
    MainWindow(const bool decoration,const bool show_label,const int frame_size
          ,const guint x,const guint y);

    void refresh();
    void quit();
    void update_values(guint8 v1, guint8 v2);
};


MainWindow::MainWindow(const bool decoration,const bool show_label,const int frame_size,const guint x,const guint y):

    Gtk::Window(GTK_WINDOW_TOPLEVEL),
    vbox(false, frame_size),
    label("Heatload "VERSION" by Lennart Poettering 2002\nGreen: CPU Load; Red: ACPI CPU Temperature\n(Ranges from 0% to 100%, resp. 0°C  to 100°C)"),
    cpu_label("Load: n/a"),
    temperature_label("Temperature: n/a"),
    drawing_area(x,y),
    hbox(true, frame_size),
    hbox2(true, frame_size),
    refresh_button("Refresh"),
    quit_button("Quit") {

    set_border_width(frame_size);
    set_title("Heatload");

    cpu_label.set_justify(GTK_JUSTIFY_LEFT);
    cpu_label.set_alignment(0, 0);

    temperature_label.set_justify(GTK_JUSTIFY_LEFT);
    temperature_label.set_alignment(0, 0);

    Gdk_Color color("darkgreen");
    GtkRcStyle *rc_style = gtk_rc_style_new ();
    rc_style->fg[GTK_STATE_NORMAL] = color;
    rc_style->color_flags[GTK_STATE_NORMAL] = (GtkRcFlags) ((int) rc_style->color_flags[GTK_STATE_NORMAL] | GTK_RC_FG);

    gtk_widget_modify_style(GTK_WIDGET(cpu_label.gtkobj()), rc_style);
    gtk_rc_style_unref(rc_style);
    
    color = Gdk_Color("darkred");
    rc_style = gtk_rc_style_new ();
    rc_style->fg[GTK_STATE_NORMAL] = color;
    rc_style->color_flags[GTK_STATE_NORMAL] = (GtkRcFlags) ((int) rc_style->color_flags[GTK_STATE_NORMAL] | GTK_RC_FG);

    gtk_widget_modify_style(GTK_WIDGET(temperature_label.gtkobj()), rc_style);
    gtk_rc_style_unref(rc_style);
    
    drawing_area.size(200, 200);
    frame.set_shadow_type(GTK_SHADOW_IN);

    destroy.connect(slot(this, &MainWindow::quit));
    quit_button.clicked.connect(destroy.slot());
    refresh_button.clicked.connect(slot(this, &MainWindow::refresh));

    frame.add(drawing_area);
    
    if(decoration) vbox.pack_start(label, false, true);
    vbox.pack_start(frame, true, true);

    hbox2.pack_start(cpu_label, false, true);
    hbox2.pack_start(temperature_label, false, true);
    

    if(show_label) vbox.pack_start(hbox2, false, true);

    hbox.pack_start(refresh_button, false, true);
    hbox.pack_start(quit_button, false, true);

    if(decoration) vbox.pack_start(hbox, false, true);

    add(vbox);

    show_all();
}

MainWindow *window = 0;

void MainWindow::refresh() {
    drawing_area.refresh_pixmap();
    drawing_area.expose_event(NULL);
}

void MainWindow::quit() {
    Gnome::Main::quit();
}

void MainWindow::update_values(guint8 v1, guint8 v2) {
    char txt[256];
    static int ov1 = -1;
    static int ov2 = -1;

    if (ov1 != v1) {
        snprintf(txt, sizeof(txt), "Load: %u%%", (unsigned) v1);
        cpu_label.set_text(txt);
        ov1 = v1;
    }
            
    if (ov2 != v2) {   
        snprintf(txt, sizeof(txt), "Temperature: %u°C", (unsigned) v2);
        temperature_label.set_text(txt);
        ov2 = v2;
    }
            
}

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

void get_values(const guint x_size) {
    guint8 v1, v2;

    if (failed)
        return;
    
    load_meter.add_value(v1 = get_load_value(),x_size);
    temperature_meter.add_value(v2 = get_temperature_value(),x_size);

    if (window) {
        window->update_values(v1, v2);
        window->refresh();
    }
}

gint timer_proc(const guint x_size) {
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
         case 'r' : refresh=atoi(optarg); break;
         case 'x' : x=atoi(optarg); break;
         case 'y' : y=atoi(optarg); break;
         default : usage(argv[0]);
       }
     }

    gchar *c[]={{argv[0]}}; // HACK
//    Gnome::Main kit("Heatload" , "0.1", argc, argv);
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

