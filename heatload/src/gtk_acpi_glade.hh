// generated 2005/9/21 16:05:17 BST by thoma@terra.(none)
// using glademm V2.0.0.2
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/thoma/Berlios/heatload/heatload.glade
// for gtk 1.2.10 and gtkmm 1.2.10
//
// Please modify the corresponding derived classes in ./src/gtk_acpi.hh and./src/gtk_acpi.cc

#ifndef _GTK_ACPI_GLADE_HH
#  define _GTK_ACPI_GLADE_HH

#include <gtk--/window.h>
#include <gtk--/tooltips.h>
#include <gtk--/label.h>
#include <gtk--/frame.h>
#include <gtk--/eventbox.h>

class gtk_acpi_glade : public Gtk::Window
{  
        
        Gtk::Tooltips _tooltips;
protected:
        class Gtk::Label *label_decoration;
        class Gtk::Frame *frame_draw;
        class Gtk::Label *label_cooling;
        class Gtk::Label *label_ac;
        class Gtk::Label *label_fan_;
        class Gtk::Label *label_ac_;
        class Gtk::Label *label_temp_;
        class Gtk::Label *label_load_;
        class Gtk::Label *label_bat1;
        class Gtk::Label *label_bat1_;
        class Gtk::Label *label_load;
        class Gtk::Label *label_temp;
        class Gtk::Label *label_cpu_throttling;
        class Gtk::EventBox *eventbox_cpu_throttling;
        class Gtk::Label *label_cpu_performance_;
        class Gtk::Label *label_cpu_performance;
        class Gtk::EventBox *eventbox_cpu_performance;
        class Gtk::Frame *frame_label;
        
        gtk_acpi_glade();
        
        ~gtk_acpi_glade();
private:
        virtual gint on_eventbox_main_button_press_event(GdkEventButton *ev) = 0;
        virtual gint on_gtk_acpi_delete_event(GdkEventAny *ev) = 0;
        virtual gint on_gtk_acpi_key_press_event(GdkEventKey *ev) = 0;
};
#endif
