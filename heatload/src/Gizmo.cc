/*  heatload
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


#include "Gizmo.hh"
#include "itos.h"
#include <fstream>


const std::string Gizmo::Value() const 
{
  if(i_value!=invalid) return itos(i_value)+einheit;
  return s_value;
}

const std::string GizmoThermal::Value() const 
{
    if(einheit!="dK")  return itos(i_value)+einheit;
    if (i_value!=invalid) return itos(int((i_value/10.)-273.15))+einheit;
    return "?";
}

const std::string GizmoBattery::RemainingTime() const
{
  double th;
  switch (zustand) {
     case e_charging: 
          if(present_rate_mW==0) return "";
          else th=double(max_capacity_mWh-remaining_capacity_mWh)/present_rate_mW;
          break;
     case e_discharging: 
          th=double(remaining_capacity_mWh)/present_rate_mW;
          break;
     case e_unknown: return "";
   }
  int h=int(th);
  int min=int(60*(th-h));
  return itos(h)+":"+itos0p(min)+"h";
}

void GizmoBattery::load_info_file(const std::string &filename)
{
 if(read_max_cap)
  {
   std::ifstream fin(filename.c_str());
   std::string s1;
   int mc,lmc;
   fin >> s1 >> s1;
   fin >> s1 >> s1 >> mc;
   fin >> s1 >> s1 >> s1 >> lmc;
   setCapacity(mc,lmc); 
  }
 else { setCapacity(44100,41000);}
}

void GizmoThrottling::load_thrott_file(const std::string &filename)
{
  char c1[100];
//  std::ifstream fin(FF.getFileName(heatload::eCPUthrottling).c_str());
  std::ifstream fin(filename.c_str());
  for(int i=0;i<3;++i) fin.getline(c1,sizeof(c1));  // Kommentarzeilen
  while(fin)
   { fin.getline(c1,sizeof(c1));
     if(!fin.good()) break;
     std::string s1=c1;
     std::string::size_type st1 = s1.find("T");
     std::string::size_type st2 = s1.find_first_of(":");
     std::string::size_type st3 = s1.find_last_of(" "); 
     if(st1==std::string::npos||st2==std::string::npos) 
         {cerr<<"Error while reading "<<filename<<'\n'; abort();}
     std::string st(s1,st1,st2-st1);
     std::string sv(s1,st1+1,st2-st1-1);
     std::string sp(s1,st3+1,std::string::npos);
     vec_state.push_back(st_state(st,atoi(sv.c_str()),sp));
   }
}

const std::string GizmoThrottling::prozent_from_state(const std::string &s)
{
  for(std::vector<GizmoThrottling::st_state>::const_iterator i=vec_state.begin();i!=vec_state.end();++i)
   {
     if(i->tstate==s) return i->prozent;
   }
   cerr << "Warning: throttling not supported,\n if the read throttling-file "
             " looking good\n please contact me <thoma@muenster.de>\n"
             " if your CPU does not support throttling you can "
             "switch it of by pressing 'p'\n\n\n";
   return "";                                     
}

