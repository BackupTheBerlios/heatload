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
 if(show_label)
  {
    label_ac->set_text(ac_adapter.state);
    label_temp->set_text(thermal.temperatur+"° "+thermal.state);
    label_cooling->set_text(thermal.cooling_mode);
    label_bat1->set_text(remaining_time());
    label_load->set_text(itos(cpu_load.load)+"%");
  }
 if(GDA && show_graph)
  {
    GDA->getVM()[0].meter.add_value(cpu_load.load,x_size);
    GDA->getVM()[1].meter.add_value(thermal.temp,x_size);
    GDA->getVM()[2].meter.add_value(battery.prozent()*100,x_size);
    GDA->refresh_pixmap();
  }
}

std::string gtk_acpi::remaining_time()
{
  std::string prozstring = battery.prozent_string();
  double th;
  if(battery.charging)
   {
     if(battery.present_rate_mW==0) return prozstring;
     th=double(battery.max_capacity_mWh-battery.remaining_capacity_mWh)/battery.present_rate_mW;
   }
  else
   {
     th=double(battery.remaining_capacity_mWh)/battery.present_rate_mW;
   }
  int h=int(th);
  int min=int(60*(th-h));
  return prozstring+" "+itos(h)+":"+itos0p(min)+"h";
}

