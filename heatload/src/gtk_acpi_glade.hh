// generated 2002/12/12 14:56:16 CET by thoma@Tiger.(none)
// using glademm V1.1.2a_cvs
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/thoma/Berlios/heatload/heatload.glade
// for gtk 1.2.10 and gtkmm 1.2.10
//
// Please modify the corresponding derived classes in ./src/gtk_acpi.hh and./src/gtk_acpi.cc

#ifndef _GTK_ACPI_GLADE_HH
#  define _GTK_ACPI_GLADE_HH

#include <gtk--/window.h>
#include <gtk--/label.h>
#include <gtk--/frame.h>

class gtk_acpi_glade : public Gtk::Window
{  
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
        class Gtk::Frame *frame_label;
        
        gtk_acpi_glade();
        
        ~gtk_acpi_glade();
private:
        virtual gint on_gtk_acpi_delete_event(GdkEventAny *ev) = 0;
        virtual gint on_gtk_acpi_key_press_event(GdkEventKey *ev) = 0;
};
#endif
