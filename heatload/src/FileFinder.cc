/* $Id: FileFinder.cc,v 1.5 2002/12/23 07:59:28 thoma Exp $ */
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

FileFinder::FileFinder()
{
  init();
}

void FileFinder::find()
{
  find_filenames();
  check(true);
}



bool FileFinder::check(const bool fix)
{
  bool alles_ok=true;
  for(std::map<heatload::e_find,std::vector<st_file> >::const_iterator i=VFiles.begin();i!=VFiles.end();++i)
   {
//cout<<i->first<<'\t' <<FileMap[i->first].name<<'\t'<< FileMap[i->first].ok<<'\n';
     if(!FileMap[i->first].ok)
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
//cout << "find: "<<i->first<<' '<<j->name<<' '<<fin.good()<<'\n';
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

  B=Bezeichnung(heatload::eBat);
  VFiles[heatload::eBat].push_back(st_file(B,"/proc/acpi/battery/BAT0/state"));
  VFiles[heatload::eBat].push_back(st_file(B,"/proc/acpi/battery/BAT1/state"));
  VFiles[heatload::eBat].push_back(st_file(B,"/proc/acpi/battery/0/status",true));

  B=Bezeichnung(heatload::eBatInfo);
  VFiles[heatload::eBatInfo].push_back(st_file(B,"/proc/acpi/battery/BAT0/info"));
  VFiles[heatload::eBatInfo].push_back(st_file(B,"/proc/acpi/battery/BAT1/info"));
  VFiles[heatload::eBatInfo].push_back(st_file(B,"/proc/acpi/battery/0/info",true));

  B=Bezeichnung(heatload::eThermal);
  VFiles[heatload::eThermal].push_back(st_file(B,"/proc/acpi/thermal_zone/THRM/temperature"));
  VFiles[heatload::eThermal].push_back(st_file(B,"/proc/acpi/thermal_zone/THM0"));
  VFiles[heatload::eThermal].push_back(st_file(B,"/proc/acpi/thermal/0/status",true));

  B=Bezeichnung(heatload::eCPUthrottling);
  VFiles[heatload::eCPUthrottling].push_back(st_file(B,"/proc/acpi/processor/CPU0/throttling"));
  VFiles[heatload::eCPUthrottling].push_back(st_file(B,"/proc/acpi/processor/CPU1/throttling"));

  B=Bezeichnung(heatload::eCPUperformance);
  VFiles[heatload::eCPUperformance].push_back(st_file(B,"/proc/acpi/processor/CPU0/performance"));
  VFiles[heatload::eCPUperformance].push_back(st_file(B,"/proc/acpi/processor/CPU1/performance"));
}

std::string FileFinder::looking_for(const heatload::e_find e)
{
  if(e==heatload::eAC) return "/proc/acpi/ac_adapter/*/state";
  if(e==heatload::eBat) return "/proc/acpi/battery/*/state";
  if(e==heatload::eBatInfo) return "/proc/acpi/battery/*/info";
  if(e==heatload::eThermal) return "/proc/acpi/thermal[_zone]/*/[temperature|status]";
  if(e==heatload::eCPUthrottling) return "/proc/acpi/processor/*/throttling";
  if(e==heatload::eCPUperformance) return "/proc/acpi/processor/*/performance";
  assert(!" never get here\n");
  abort();
}

std::string FileFinder::Bezeichnung(const heatload::e_find e)
{
  if(e==heatload::eAC) return  "AC";
  if(e==heatload::eBat) return "Battery-State";
  if(e==heatload::eBatInfo) return "Battery-Info";
  if(e==heatload::eThermal) return "Thermal-Zone";
  if(e==heatload::eCPUthrottling) return "CPU-Throttling";
  if(e==heatload::eCPUperformance) return "CPU-Performance";
  assert(!" never get here\n");
  abort();
}


void FileFinder::set_dummy_file(st_file &FM,const heatload::e_find e)
{
  FM.bezeichnung=Bezeichnung(e);
  FM.name=looking_for(e);
}
