// generated 2002/3/18 6:58:50 CET by thoma@Tiger.(none)
// using glademm V0.6.4b_cvs
//
// newer (non customized) versions of this file go to gtk_acpi.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _GTK_ACPI_HH
#  include "gtk_acpi_glade.hh"
#  define _GTK_ACPI_HH

#include<string>
#include "GraphDrawingArea.hh"
#include "itos.h"

class gtk_acpi : public gtk_acpi_glade
{  
        guint  x_size,y_size;
        const bool show_graph;
        const bool show_label;
        const bool show_decoration;
        const bool read_max_cap_;
        
        std::string temp_color;
        std::string bat_color;
        std::string load_color;
        int max_cap;
        
        friend class gtk_acpi_glade;
        struct st_ac_adapter{std::string state;};
        struct st_cpu_load{guint load;};
        struct st_thermal{guint temp;std::string temperatur;std::string cooling_mode;std::string state;
               st_thermal() {}
               st_thermal(guint t1,std::string t,std::string c,std::string s)
                  : temp(t1),temperatur(t),cooling_mode(c),state(s){} };
        struct st_battery{bool present;bool charging; int present_rate_mW; 
                          int remaining_capacity_mWh;int max_capacity_mWh;
               st_battery() :present(false),charging(false),present_rate_mW(0),
                         remaining_capacity_mWh(0),max_capacity_mWh(0) {}
               st_battery(bool p,bool c,int pr,int r,int m)
                  : present(p),charging(c),present_rate_mW(pr),
                    remaining_capacity_mWh(r),max_capacity_mWh(m) {}
               const double prozent() const {return double(remaining_capacity_mWh)/max_capacity_mWh;}
               const std::string prozent_string() const {return ::itos(int(prozent()*100))+"%";}
               
                };
              
        st_ac_adapter ac_adapter;
        st_thermal thermal;
        st_battery battery;
        st_cpu_load cpu_load;

        void init();
        void read_max_cap();
        void show_values();
        std::string remaining_time();
        void get_values();
        void get_ac_adapter();        
        void get_thermal();        
        void get_battery();        
        void get_load_value();

        void get_show();
        gint timeout();

        void set_color(Gtk::Widget &W,const std::string &color);

        GraphDrawingArea *GDA;
        gint on_gtk_acpi_delete_event(GdkEventAny *ev);
        void ende();
   public:
        gtk_acpi(const guint x,const guint y,const guint refresh,
                 const bool show_graph,const bool show_label,const bool show_decoration,
                 const bool read_max_cap);

};
#endif
