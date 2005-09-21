/* $Id: FileFinder.cc,v 1.12 2005/09/21 14:12:11 thoma Exp $ */
/*  Copyright (C) 2002 Malte Thoma
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


#include "FileFinder.hh"
#include "gtk_acpi.hh"
#include "WindowInfo.hh"
#include <fstream>
#include <iostream> // debug
#include <assert.h>
#include "Gizmo.hh"

FileFinder::FileFinder()
{
  init();
}

void FileFinder::find(const HeatloadGizmo &HG)
{
  find_filenames();
  check(true,HG);
}



bool FileFinder::check(const bool fix,const HeatloadGizmo &HG)
{
  bool alles_ok=true;
  for(std::map<heatload::e_find,std::vector<st_file> >::const_iterator i=VFiles.begin();i!=VFiles.end();++i)
   {
//std::cout<<i->first<<'\t' <<FileMap[i->first].name<<'\t'<< FileMap[i->first].ok<<'\n';
     if(!const_cast<FileMap_t&>(FileMap)[i->first].ok && HG.Visible(i->first))
      {  
         set_dummy_file(FileMap[i->first],i->first);
         std::string s=" Sorry can't open any of the following files:\n";
         for(std::vector<st_file>::const_iterator j=i->second.begin();j!=i->second.end();++j)
           s+= "   "+j->name+'\n';
         s+="\nI've written a dummy ~/.heatloadrc. Please edit the filenames and restart.\n";
         if(fix) manage(new WindowInfo(0,s,false));
         alles_ok=false;     
      }
   }
// if(!alles_ok) hauptfenster->save();
 return alles_ok;
}

void FileFinder::find_filenames()
{
//cout << "FFsize_ "<<FileMap.size()<<'\n';
   for(std::map<heatload::e_find,std::vector<st_file> >::const_iterator i=VFiles.begin();i!=VFiles.end();++i)
    {
      FileMap[i->first]=st_file();
//cout << "\tS="<<i->second.size()<<'\n';
      for(std::vector<st_file>::const_iterator j=i->second.begin();j!=i->second.end();++j)
       {
         std::ifstream fin(j->name.c_str());
//std::cout << "find: "<<i->first<<' '<<j->name<<' '<<fin.good()<<'\n';
         if(fin.good())
           {
             FileMap[i->first]=*j;
             FileMap[i->first].ok=true;
             break;
           }
       }
   }
//cout << "FFsize_ "<<FileMap.size()<<'\n';
}


void FileFinder::init()
{
  std::string B=Bezeichnung(heatload::eAC);
  VFiles[heatload::eAC].push_back(st_file(B,"/proc/acpi/ac_adapter/AC0/state"));
  VFiles[heatload::eAC].push_back(st_file(B,"/proc/acpi/ac_adapter/ACAD/state"));
  VFiles[heatload::eAC].push_back(st_file(B,"/proc/acpi/ac_adapter/0/status"));
  VFiles[heatload::eAC].push_back(st_file(B,"/proc/acpi/ac_adapter/AC/state"));

  B=Bezeichnung(heatload::eBat);
//  VFiles[heatload::eBat].push_back(st_file(B,"/tmp/acpi/battery/1/status"));
  VFiles[heatload::eBat].push_back(st_file(B,"/proc/acpi/battery/BAT0/state"));
  VFiles[heatload::eBat].push_back(st_file(B,"/proc/acpi/battery/BAT1/state"));
  VFiles[heatload::eBat].push_back(st_file(B,"/proc/acpi/battery/0/status",true));
  VFiles[heatload::eBatInfo].push_back(st_file(B,"/proc/acpi/battery/1/status",true));

  B=Bezeichnung(heatload::eBatInfo);
// VFiles[heatload::eBat].push_back(st_file(B,"/tmp/acpi/battery/1/info"));
  VFiles[heatload::eBatInfo].push_back(st_file(B,"/proc/acpi/battery/BAT0/info"));
  VFiles[heatload::eBatInfo].push_back(st_file(B,"/proc/acpi/battery/BAT1/info"));
  VFiles[heatload::eBatInfo].push_back(st_file(B,"/proc/acpi/battery/0/info",true));
  VFiles[heatload::eBatInfo].push_back(st_file(B,"/proc/acpi/battery/1/info",true));

  B=Bezeichnung(heatload::eFan);
  VFiles[heatload::eFan].push_back(st_file(B,"/proc/acpi/fan/FAN/state"));

  B=Bezeichnung(heatload::eThermal);
  VFiles[heatload::eThermal].push_back(st_file(B,"/proc/acpi/thermal_zone/THRM/temperature"));
  VFiles[heatload::eThermal].push_back(st_file(B,"/proc/acpi/thermal_zone/THM0"));
  VFiles[heatload::eThermal].push_back(st_file(B,"/proc/acpi/thermal/0/status",true));

  B=Bezeichnung(heatload::eCPUthrottling);
  VFiles[heatload::eCPUthrottling].push_back(st_file(B,"/proc/acpi/processor/CPU0/throttling"));
  VFiles[heatload::eCPUthrottling].push_back(st_file(B,"/proc/acpi/processor/CPU1/throttling"));

  B=Bezeichnung(heatload::eCPUperformance_cur_scale);
  VFiles[heatload::eCPUperformance_cur_scale].push_back(st_file(B,"/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq"));

  B=Bezeichnung(heatload::eLoad);
  VFiles[heatload::eLoad].push_back(st_file(B,"/proc/stat"));

  B=Bezeichnung(heatload::eSuspend_sleep);
  VFiles[heatload::eSuspend_sleep].push_back(st_file(B,"/usr/sbin/swsusp_sleep"));
  B=Bezeichnung(heatload::eSuspend_awake);
  VFiles[heatload::eSuspend_awake].push_back(st_file(B,"/usr/sbin/swsusp_awake"));
}

std::string FileFinder::looking_for(const heatload::e_find e)
{
  if(e==heatload::eAC) return "/proc/acpi/ac_adapter/___/state";
  if(e==heatload::eFan) return "/proc/acpi/fan/___/state";
  if(e==heatload::eBat) return "/proc/acpi/battery/___/state";
  if(e==heatload::eBatInfo) return "/proc/acpi/battery/___ */info";
  if(e==heatload::eThermal) return "/proc/acpi/thermal[_zone]/___/[temperature|status]";
  if(e==heatload::eCPUthrottling) return "/proc/acpi/processor/___/throttling";
  if(e==heatload::eCPUperformance_cur_scale) return "/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq";
  if(e==heatload::eLoad) return "/proc/stat";
  if(e==heatload::eSuspend_sleep) return "/usr/sbin/swsusp_sleep";
  if(e==heatload::eSuspend_awake) return "/usr/sbin/swsusp_awake";
  assert(!" never get here\n");
  abort();
}

std::string FileFinder::Bezeichnung(const heatload::e_find e)
{
  if(e==heatload::eAC) return  "AC";
  if(e==heatload::eFan) return "Fan";
  if(e==heatload::eBat) return "Battery-State";
  if(e==heatload::eBatInfo) return "Battery-Info";
  if(e==heatload::eThermal) return "Thermal-Zone";
  if(e==heatload::eCPUthrottling) return "CPU-Throttling";
  if(e==heatload::eCPUperformance_cur_scale) return "CPU-Performance_cur_scaling";
  if(e==heatload::eLoad) return "CPU-Load";
  if(e==heatload::eSuspend_sleep) return "Suspend-Sleep";
  if(e==heatload::eSuspend_awake) return "Suspend-Awake";
  assert(!" never get here\n");
  abort();
}


void FileFinder::set_dummy_file(st_file &FM,const heatload::e_find e)
{
  FM.bezeichnung=Bezeichnung(e);
  FM.name=looking_for(e);
}
