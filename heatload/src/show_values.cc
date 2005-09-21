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
#include <iostream> // debug

void gtk_acpi::show_values()
{
 if(show_widget.label)
  {
    label_ac->set_text(HG.ac_adapter.Value());
    label_temp->set_text(HG.thermal.Value());
    label_cooling->set_text(HG.fan.Value());
    label_bat1->set_text(HG.battery.Value());
    label_load->set_text(HG.cpu_load.Value());
    label_cpu_throttling->set_text(HG.cpu_throttling.Value());
    label_cpu_performance->set_text(HG.cpu_performance_cur_scale.Value());
  }
 if(GDA && show_widget.graph)
  {
    if(HG.cpu_load.Visible()) GDA->getVM()[0].meter.add_value(HG.cpu_load.IValue(),show_widget.x);
    if(HG.thermal.Visible())  GDA->getVM()[1].meter.add_value(HG.thermal.IValue(),show_widget.x);
    if(HG.battery.Visible())  GDA->getVM()[2].meter.add_value(HG.battery.Prozent(),show_widget.x);
    if(HG.cpu_performance_cur_scale.Visible())  
                              GDA->getVM()[3].meter.add_value(HG.cpu_performance_cur_scale.IProzent(),show_widget.x);
    GDA->refresh_pixmap();
  }
}

void gtk_acpi::test_auto_tp()
{
   for(std::vector<heatload::st_auto>::const_iterator i=AutoVec.begin();i!=AutoVec.end();++i)
    {
//if(i->EF==heatload::eSuspend_sleep)
//std::cout << i->set<<' '<<i->temperature<<' '<<HG.battery.Prozent()<<'\n';
      if(i->set) continue;
      if(i->EF==heatload::eSuspend_sleep)
       {
        if(HG.battery.Discharging() && i->temperature >= HG.battery.Prozent())
          {
//std::cout << "SUSPEND\n";
            suspend_activate(true);
            i->set=true;
          }
       }
      else if(i->temperature <= HG.thermal.IValue())
       { 
         select_throttling(i->state,i->EF);
         i->set=true;
       }
    }
}

void gtk_acpi::reset_auto_tp()
{
   for(std::vector<heatload::st_auto>::const_iterator i=AutoVec.begin();i!=AutoVec.end();++i)
      i->set = false;   
}
