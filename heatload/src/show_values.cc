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
    if(HG.ac_adapter.Visible())   label_ac->set_text(HG.ac_adapter.Value()); 
    if(HG.thermal.Visible())      label_temp->set_text(HG.thermal.Value()); 
    if(HG.fan.Visible())          label_cooling->set_text(HG.fan.Value());
    if(HG.battery.Visible())      label_bat1->set_text(itos(HG.battery.Prozent())+HG.battery.Einheit()+" "+HG.battery.RemainingTime());
    if(HG.cpu_load.Visible())     label_load->set_text(HG.cpu_load.Value());
    if(HG.cpu_throttling.Visible())label_cpu_throttling->set_text(HG.cpu_throttling.Value());
//    if(show_what.cpu_performance) label_cpu_performance->set_text(HG.cpu.performance.value);
  }
 if(GDA && show_widget.graph)
  {
    if(HG.cpu_load.Visible()) GDA->getVM()[0].meter.add_value(HG.cpu_load.IValue(),show_widget.x);
    if(HG.thermal.Visible()) GDA->getVM()[1].meter.add_value(HG.thermal.IValue(),show_widget.x);
    if(HG.battery.Visible()) GDA->getVM()[2].meter.add_value(HG.battery.Prozent(),show_widget.x);
    GDA->refresh_pixmap();
  }
}

