/*  gtkacpi
 *  Copyright (C) 2002 Malte Thoma
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

#include "gtk_acpi.hh"
#include <cstdio>
#include "itos.h"

void gtk_acpi::show_values()
{
 if(show_widget.label)
  {
    if(show_what.ac)   label_ac->set_text(ac_adapter.Value()); 
    if(show_what.temp) label_temp->set_text(thermal.Value()); 
    if(show_what.fan)  label_cooling->set_text(fan.Value());
    if(show_what.bat)  label_bat1->set_text(itos(battery.Prozent())+battery.Einheit()+" "+battery.RemainingTime());
    if(show_what.load) label_load->set_text(cpu_load.Value());
    if(show_what.cpu_throttling)  label_cpu_throttling->set_text(cpu.throttling.prozent);
    if(show_what.cpu_performance) label_cpu_performance->set_text(cpu.performance.value);
  }
 if(GDA && show_widget.graph)
  {
    if(show_what.load) GDA->getVM()[0].meter.add_value(cpu_load.IValue(),show_widget.x);
    if(show_what.temp) GDA->getVM()[1].meter.add_value(thermal.IValue(),show_widget.x);
    if(show_what.bat) GDA->getVM()[2].meter.add_value(battery.Prozent(),show_widget.x);
    GDA->refresh_pixmap();
  }
}

