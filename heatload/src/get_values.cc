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
#include <fstream>

void gtk_acpi::get_values()
{

 if(show_what.ac)   get_ac_adapter();
 if(show_what.temp || show_what.fan) get_thermal();
 if(show_what.bat)  get_battery();
 if(show_what.load) get_load_value();

}

void gtk_acpi::get_ac_adapter()
{
  std::string s1,s2;
  ifstream fin(ac_file.name.c_str());
  fin >> s1 >>s2;
  ac_adapter.state=s2;
}

void gtk_acpi::get_thermal()
{
  std::string s1,cm,te2;
  guint te;

  ifstream fin(therm_file.name.c_str());
  fin >> s1 >> te  >> te2;

  if(!therm_file.old_style)
   {
     ifstream fin1("/proc/acpi/thermal_zone/THRM/cooling_mode");
     if(fin1.good()) fin1 >> s1>>s1 >>cm;  
//     ifstream fin3("/proc/acpi/thermal_zone/THRM/state");
//     fin3 >> s1 >>st;  
//     if(!fin1.good())
//      {cerr << "Sorry can't open '/proc/acpi/thermal_zone/THRM/cooling_mode'\n"; abort();}
   }
  thermal=st_thermal(te,te2,cm);
}
         
void gtk_acpi::get_battery()
{
  std::string s1;
  std::string present,charging_state;
  std::string spresent_rate;
  int present_rate=0,remaining_cap=0;
  ifstream fin(bat_file.name.c_str());
  fin >> s1 >> present;
  fin >> s1 >> s1;
  if(!bat_file.old_style) 
   { fin >> s1;
     fin >> s1 >> s1 >> charging_state;
   }
  fin >> s1 >> s1 >> spresent_rate; 
  if(spresent_rate!="unknown") 
   { fin >> s1;  
     present_rate=atoi(spresent_rate.c_str());
   }
  fin >> s1 >> s1 >> remaining_cap >> s1;  

  bool bpresent,bcharging_state;
  if(present=="yes") bpresent = true;
  else if (present=="no")  bpresent = false;
  else 
   { static bool show=true;
     if(show) { show=false;
        cerr<<"'Present:' should be 'yes' or 'no' in '/proc/acpi/battery/BAT1/state',\n"
              " it seems to be '"<<present<<"' assuming 'yes'\n";
       }
     bpresent = true;
   }
  if(charging_state=="charging") bcharging_state = true;
  else if (charging_state=="discharging")  bcharging_state = false;
  else 
   { static bool show=true;
     if(show) { show=false;
         cerr<<"'Present Rate:' should be 'charging' or 'discharging' in '/proc/acpi/battery/BAT1/state'\n"
         " but I it is '"<<charging_state<<"' assuming 'charging'\n"
         " if you have a 'charging state' in /proc/acpi/battery/0/status\n"
         " (or anywhere else) please submit a 'cat' of this file to me"
         " <thoma@muenster.de>\n";
        }
     bcharging_state = true;
   }
  battery=st_battery(bpresent,bcharging_state,present_rate,remaining_cap,max_cap,last_max_cap);
}

void failure(const string &txt) 
{         
  cerr << txt<<'\n';
}

#include <stdio.h>
#include <unistd.h>

void gtk_acpi::get_load_value() 
{
    char txt[256];
    char*p; 
    float v;
    static int ct[2][4] = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
    static int n = 0;
    int d[4];

    static FILE*f = fopen("/proc/stat", "r");

    if (!f) {
        failure("Unable to open kernel statistics file.");
        return;
    }

    if (!(p = fgets(txt, sizeof(txt), f))) {
        failure("Unable to read from kernel statistics file.");
        fclose(f);
        f = NULL;
        return;
    }

    int fd = dup(fileno(f));
    fclose(f);
    f = fdopen(fd, "r");
    g_assert(f);
    fseek(f, 0, SEEK_SET);

    if (strlen(p) <= 5)
        return ;
        
    sscanf(p+5, "%u %u %u %u", &ct[n][0], &ct[n][1], &ct[n][2], &ct[n][3]);

    int t = 0;
    
    for (int i = 0; i < 4; i++)
        t += (d[i] = abs(ct[n][i] - ct[1-n][i]));

    v = (t - d[3])/(float) t;
    
    n = 1-n;

   cpu_load.load= (guint8) (v*100);
}
