// generated 2002/12/17 15:21:52 CET by thoma@Tiger.(none)
// using glademm V1.1.2a_cvs
//
// newer (non customized) versions of this file go to WindowInfo.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "WindowInfo.hh"
#include "gtk_acpi.hh"

WindowInfo::WindowInfo(gtk_acpi *h,const std::string &s,const bool show_checkbutton)
 : hauptfenster(h)
{
  if(show_checkbutton) checkbutton_show->show();
  else                 checkbutton_show->hide();
  checkbutton_show->set_active(!hauptfenster->show_sudo);
  get_window().move(200,200);
  label_info->set_text(s);
}

void WindowInfo::on_button_close_clicked()
{
  destroy();
}

void WindowInfo::on_checkbutton_show_toggled()
{
  hauptfenster->show_sudo = !checkbutton_show->get_active();
  hauptfenster->save();
}
