/* $Id: FileFinder.cc,v 1.3 2002/12/20 09:55:51 thoma Exp $ */
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

FileFinder::FileFinder(gtk_acpi *h,const FileMap_t fm) 
 : hauptfenster(h),FileMap(fm)
{
  init();
  find_filenames();
  check(true);
}

bool FileFinder::check(const bool fix)
{
  bool alles_ok=true;
  for(std::map<heatload::e_find,std::vector<st_file> >::const_iterator i=VFiles.begin();i!=VFiles.end();++i)
   {
     if(!FileMap[i->first].ok)
      {  
         set_dummy_file(FileMap[i->first],i->first);
         std::string s=" Sorry can't open any of the following files:\n";
         for(std::vector<st_file>::const_iterator j=i->second.begin();j!=i->second.end();++j)
           s+= "   "+j->name+'\n';
         s+="\nI've written a dummy ~/.heatloadrc. Please edit the filenames and restart.\n";
         if(fix) manage(new WindowInfo(hauptfenster,s,false));  
         alles_ok=false;     
      }
   }
 if(!alles_ok) hauptfenster->save();
 return alles_ok;
}

void FileFinder::find_filenames()
{
   for(std::map<heatload::e_find,std::vector<st_file> >::const_iterator i=VFiles.begin();i!=VFiles.end();++i)
    {
      for(std::vector<st_file>::const_iterator j=i->second.begin();j!=i->second.end();++j)
       {
         std::ifstream fin(j->name.c_str());
         if(fin.good())
           {
             FileMap[i->first]=*j;
             FileMap[i->first].ok=true;
             break;
          }
      }
   }
}

void FileFinder::init()
{
  std::string B=Bezeichnung(heatload::eAC);
  VFiles[heatload::eAC].push_back(st_file(FileMap[heatload::eAC]));
  VFiles[heatload::eAC].push_back(st_file(B,"/proc/acpi/ac_adapter/AC0/state"));
  VFiles[heatload::eAC].push_back(st_file(B,"/proc/acpi/ac_adapter/ACAD/state"));
  VFiles[heatload::eAC].push_back(st_file(B,"/proc/acpi/ac_adapter/0/status"));

  B=Bezeichnung(heatload::eBat);
  VFiles[heatload::eBat].push_back(st_file(FileMap[heatload::eBat]));
  VFiles[heatload::eBat].push_back(st_file(B,"/proc/acpi/battery/BAT0/state"));
  VFiles[heatload::eBat].push_back(st_file(B,"/proc/acpi/battery/BAT1/state"));
  VFiles[heatload::eBat].push_back(st_file(B,"/proc/acpi/battery/0/status",true));

  B=Bezeichnung(heatload::eBatInfo);
  VFiles[heatload::eBatInfo].push_back(st_file(FileMap[heatload::eBatInfo]));
  VFiles[heatload::eBatInfo].push_back(st_file(B,"/proc/acpi/battery/BAT0/info"));
  VFiles[heatload::eBatInfo].push_back(st_file(B,"/proc/acpi/battery/BAT1/info"));
  VFiles[heatload::eBatInfo].push_back(st_file(B,"/proc/acpi/battery/0/info",true));

  B=Bezeichnung(heatload::eThermal);
  VFiles[heatload::eThermal].push_back(st_file(FileMap[heatload::eThermal]));
  VFiles[heatload::eThermal].push_back(st_file(B,"/proc/acpi/thermal_zone/THRM/temperature"));
  VFiles[heatload::eThermal].push_back(st_file(B,"/proc/acpi/thermal_zone/THM0"));
  VFiles[heatload::eThermal].push_back(st_file(B,"/proc/acpi/thermal/0/status",true));

  B=Bezeichnung(heatload::eCPUthrottling);
  VFiles[heatload::eCPUthrottling].push_back(st_file(FileMap[heatload::eCPUthrottling]));
  VFiles[heatload::eCPUthrottling].push_back(st_file(B,"/proc/acpi/processor/CPU0/throttling"));
  VFiles[heatload::eCPUthrottling].push_back(st_file(B,"/proc/acpi/processor/CPU1/throttling"));
}

std::string FileFinder::looking_for(const heatload::e_find e)
{
  if(e==heatload::eAC) return "/proc/acpi/ac_adapter/*/state";
  if(e==heatload::eBat) return "/proc/acpi/battery/*/state";
  if(e==heatload::eBatInfo) return "/proc/acpi/battery/*/info";
  if(e==heatload::eThermal) return "/proc/acpi/thermal[_zone]/*/[temperature|status]";
  if(e==heatload::eCPUthrottling) return "/proc/acpi/processor/*/throttling";
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
  assert(!" never get here\n");
  abort();
}


void FileFinder::set_dummy_file(st_file &FM,const heatload::e_find e)
{
  FM.bezeichnung=Bezeichnung(e);
  FM.name=looking_for(e);
}
