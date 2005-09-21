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
#include <iostream>
#include <assert.h>

const std::string Gizmo::Value()
{
  if(!Visible()) return "";
  get_value();
//cout << "V2: "<<i_value<<' '<<s_value<<' '<<einheit<<'\n';
  if(i_value!=invalid) return itos(i_value)+einheit;
  return s_value;
}

void Gizmo::get_value()
{
  std::string sdummy,unit,svalue;
  char cdummy[100];
  int ivalue=invalid;
  std::ifstream fin(getFilenameForValues().c_str());
//cout << "Open: "<<getFilenameForValues()<<'\n';
  if(!fin.good()) {std::cerr<< "Error cannot open '"<<getFilenameForValues()<<"'\n";return;}
  for(int i=1;i<file_value.line_of_interest;++i) fin.getline(cdummy,sizeof(cdummy));  
  for(int i=1;i<file_value.column_of_value;++i)  fin >> sdummy; 
  if(file_value.value_is_int) fin >> ivalue;
  else                        fin >> svalue;
  if(file_value.column_of_unit==file_value.column_of_value+1)
     fin >> unit;
  if     (what==heatload::eCPUthrottling) setValue(static_cast<GizmoThrottling&>(*this).prozent_from_state(svalue));
//  else if(what==heatload::eCPUperformance_cur_scale) setValue(static_cast<GizmoPerformance_cur_scale&>(*this).prozent(svalue));
  else if(!file_value.value_is_int) setValue(svalue);
  else                              setValue(ivalue,unit);

//cout <<'\t'<< sdummy <<'\t'<<ivalue<<'*'<<svalue<<'\t'
// <<unit<<"#"<< file_value.column_of_unit<<'\n';
}



const std::string GizmoThermal::Value()
{
  if(!Visible()) return "";
  get_value();
  if(einheit!="dK")  return itos(i_value)+einheit;
  if (i_value!=invalid) return itos(int((i_value/10.)-273.15))+einheit;
  return "?";
}

const std::string GizmoPerformance_cur_scale::Value()
{ 
  if(!Visible()) return "";
  get_value();
  return  Prozent();
  return "?";
}



const std::string GizmoBattery::Value()
{
  if(!Visible()) return "";
  get_value();
  return itos(Prozent())+"%"+Einheit()+" "+RemainingTime();
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

void GizmoBattery::get_value()
{
  if(!Visible()) return;
  std::string s1;
  std::string present,charging_state;
  std::string spresent_rate;
  int present_rate=0,remaining_cap=0;
  std::ifstream fin(getFilenameForValues().c_str());
  fin >> s1 >> present;
#if 0  
  fin >> s1 >> s1;
  if(!getFileOldStyleForValues())
   { fin >> s1;
     fin >> s1 >> s1 >> charging_state;
   }
#else
  int count=0;
  while(true)
   {
     if(++count>10) break;
     std::getline(fin,s1);
     if(s1.find_last_of(" ")!=std::string::npos)
       s1=s1.substr(s1.find_last_of(" ")+1,std::string::npos);
     if(s1=="charging" || s1=="charged" || s1=="unknown" || s1=="discharging")
      {
        charging_state=s1;
        break; 
      }
   }
#endif
  fin >> s1 >> s1 >> spresent_rate;
  if(spresent_rate!="unknown") 
   { fin >> s1;  
     present_rate=atoi(spresent_rate.c_str());
   }
  fin >> s1 >> s1 >> remaining_cap >> s1;

  bool bpresent;
  GizmoBattery::e_zustand bcharging_state;
  if(present=="yes") bpresent = true;
  else if (present=="no")  bpresent = false;
  else
   { static bool show=true;
     if(show) { show=false;
        cerr<<"'Present:' should be 'yes' or 'no' in '"<<getFilenameForValues()<<"',\n"
              " it seems to be '"<<present<<"' assuming 'yes'\n";
       }
     bpresent = true;
   }
  if(charging_state=="charging") bcharging_state = GizmoBattery::e_charging;
  else if(charging_state=="charged") bcharging_state = GizmoBattery::e_charging;
  else if(charging_state=="unknown") bcharging_state = GizmoBattery::e_unknown;
  else if (charging_state=="discharging")  bcharging_state = GizmoBattery::e_discharging;
  else
   { static bool show=true;
     if(show) { show=false;
         cerr<<"'Present Rate:' should be 'charging', 'discharging' or 'unknown'\n"
         " in '"<<getFilenameForValues()<<"'\n"
         " but it is '"<<charging_state<<"' assuming 'unknown'\n"
         " if you have a 'charging state' in "<<getFilenameForValues()<<"\n"
         " (or anywhere else) please submit a 'cat' of this file to me"
         " <thoma@muenster.de>\n";
        }
     bcharging_state = GizmoBattery::e_unknown;
   }
 setValue(bpresent,bcharging_state,present_rate,remaining_cap);
}

void delete_leading_space(std::string &s)
{
  while(true)
   {
     std::string::size_type st = s.find_first_of(" ");     
     if(st==std::string::npos || st != 0) return;
     std::string s_(s,1,std::string::npos);
     s=s_;
   }
}

void GizmoPerformance_cur_scale::load_cur_scale(const std::string &filename)
{
  std::ifstream fin(filename.c_str());
  if(!fin.good()) assert(!"never get here: load_cur_scale");

  fin >>  i_value;
}

void GizmoPerformance_cur_scale::load_max_scale(const std::string &filename)
{
  std::ifstream fin(filename.c_str());
  if(!fin.good()) assert(!"never get here: load_max_scale");

  fin >>  max_scale;
}

void GizmoThrottling::load_thrott_file(const std::string &filename)
{
  char c1[100];
  std::ifstream fin(filename.c_str());
  for(int i=0;i<3;++i) fin.getline(c1,sizeof(c1));  // Kommentarzeilen
  while(fin)
   { fin.getline(c1,sizeof(c1));
     if(!fin.good()) break;
     std::string s1=c1;

     std::string C;
//     if     (What()==heatload::eCPUthrottling) C="T";
//     else if(What()==heatload::eCPUperformance_cur_scale)C="P";
//     else assert(!"never get here");
     std::string::size_type st1 = s1.find(C);
     std::string::size_type st2 = s1.find_first_of(":");
     if(st1==std::string::npos||st2==std::string::npos) 
         {cerr<<"Error while reading "<<filename<<'\n'; abort();}
     std::string st(s1,st1,st2-st1);
     std::string sv(s1,st1+1,st2-st1-1);
     std::string sp(s1,st2+1,std::string::npos);
     delete_leading_space(sp);      
     vec_state.push_back(st_state(st,atoi(sv.c_str()),sp));
   }
}

const std::string GizmoThrottling::prozent_from_state(const std::string &s)
{
  for(std::vector<GizmoThrottling::st_state>::const_iterator i=vec_state.begin();i!=vec_state.end();++i)
   {
     if(i->tstate!=s) continue;
     std::string x=i->prozent;
     if(x.find(",")!=std::string::npos) x.erase(x.find(","),std::string::npos);
     return x;
   }
   std::string ss,ss2;
   if     (What()==heatload::eCPUthrottling)  {ss="throttling"; ss2="u";}
//   else if(What()==heatload::eCPUperformance_cur_scale) {ss="performance"; ss2="p";}
   else assert(!"never get here");
   cerr << "Warning: "<<ss<<" not supported,\n if the "<<ss<<"-file "
             " looking good\n please contact me <thoma@muenster.de>\n"
             " if your CPU does not support "<<ss<<" you can "
             "switch it of by pressing '"<<ss2<<"'\n\n\n";
   return "";                                     
}


#include <stdio.h>
#include <unistd.h>


void GizmoLoad::get_value()
{
  if(!Visible()) return;
   
    char txt[256];
    char*p; 
    float v;
    static int ct[2][4] = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
    static int n = 0;
    int d[4];

    static FILE*f = fopen(getFilenameForValues().c_str(), "r");

    if (!f) {
        std::cerr<< "Unable to open kernel statistics file.";
        return;
    }


    if (!(p = fgets(txt, sizeof(txt), f))) {
        std::cerr<<"Unable to read from kernel statistics file.";
        fclose(f);
        f = NULL; 
        return;   
    }

    int fd = dup(fileno(f));
    fclose(f);
    f = fdopen(fd, "r");
    assert(f);
    fseek(f, 0, SEEK_SET);

    if (strlen(p) <= 5)
        return ;
        
    sscanf(p+5, "%u %u %u %u", &ct[n][0], &ct[n][1], &ct[n][2], &ct[n][3]);

    int t = 0;
    
    
    for (int i = 0; i < 4; i++)
        t += (d[i] = abs(ct[n][i] - ct[1-n][i]));

    v = (t - d[3])/(float) t;
    
    n = 1-n;

   setValue((unsigned int) (v*100),"%");
}

bool HeatloadGizmo::Visible(const heatload::e_find EF) const
{
  switch (EF) {
    case heatload::eAC   : return ac_adapter.Visible();
    case heatload::eBat  : return battery.Visible();
    case heatload::eFan  : return fan.Visible();
    case heatload::eThermal  : return thermal.Visible();
    case heatload::eCPUthrottling  : return cpu_throttling.Visible();
    case heatload::eCPUperformance_cur_scale  : return cpu_performance_cur_scale.Visible();
    case heatload::eLoad  : return cpu_load.Visible();
    default: return true;
   }
 assert(!"never ger here\n");
 abort();
}
