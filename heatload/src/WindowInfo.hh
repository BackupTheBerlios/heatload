// generated 2002/12/17 15:21:52 CET by thoma@Tiger.(none)
// using glademm V1.1.2a_cvs
//
// newer (non customized) versions of this file go to WindowInfo.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _WINDOWINFO_HH
#  include "WindowInfo_glade.hh"
#  define _WINDOWINFO_HH

class gtk_acpi;

#include "Structs.hh"

class WindowInfo : public WindowInfo_glade
{  
      const gtk_acpi *hauptfenster;
      
      void init(const std::string &s);
      void on_button_close_clicked();
      void on_checkbutton_show_toggled();

   public:
      WindowInfo(const gtk_acpi *h,const std::string &s,const bool show_checkbutton);
};
#endif
