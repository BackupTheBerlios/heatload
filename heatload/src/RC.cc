/* $Id: RC.cc,v 1.8 2003/01/30 12:22:51 thoma Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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

#include <vector>
#include <map>
#include <string>
#include <unistd.h>
#include <TagStream.h>
#include "gtk_acpi.hh"
#include "Gizmo.hh"
#include "RC.hh"
#include <fstream>

void rc_file::load(HeatloadGizmo &HG,heatload::st_widget &show_widget,
                   bool &show_sudo,FileFinder &FF)
{
  std::vector<std::string> V;
  char currentwd[10240];
  *currentwd=0;
  getcwd(currentwd,sizeof currentwd);
  V.push_back(std::string(currentwd)+"/.heatloadrc");
  V.push_back(std::string(getenv("HOME"))+"/.heatloadrc");
  V.push_back("/etc/heatload.conf");
  for(std::vector<std::string>::const_iterator i=V.begin();i!=V.end();++i)
   {
cout << "looking for "<<*i<<'\n';
     std::ifstream f(i->c_str());
     if (!f.good()) continue;
cout << "FOUND   \n";
     TagStream ts(f);
     const Tag *data=ts.find("heatload-preferences");
     if (!data) {std::cerr << "Wrong format in '"<<*i<<"': 'heatload-preferences' not found\n"; return;}
     const Tag *widget=data->find("Widget");
     if(!widget) {std::cerr << "Wrong format in '"<<*i<<"': 'Widget' not found\n'";}     
     else {
        show_widget.x = widget->getIntAttr("X");
        show_widget.y = widget->getIntAttr("Y");
        show_widget.refresh = widget->getIntAttr("RefreshRate");
        show_widget.decoration = widget->getBoolAttr("Decoration");
        show_widget.label = widget->getBoolAttr("Label");
        show_widget.graph = widget->getBoolAttr("Graph");
        show_widget.swsusp = widget->getBoolAttr("swsusp",true);
      }
     const Tag *show=data->find("Show");
     if(!show) {std::cerr << "Wrong format in '"<<*i<<"': 'Show' not found\n'"; }     
     else {
        HG.ac_adapter.setVisible(show->getBoolAttr("AC",true));
        HG.battery.setVisible(show->getBoolAttr("Battery",true));
        HG.thermal.setVisible(show->getBoolAttr("Thermal",true));
        HG.fan.setVisible(show->getBoolAttr("Fan",true));
        HG.cpu_throttling.setVisible(show->getBoolAttr("CPU-Throttling",true));
        HG.cpu_performance.setVisible(show->getBoolAttr("CPU-Performance",true));
      }           
     const Tag *tcolor=data->find("Color");
     if(!show) {std::cerr << "Wrong format in '"<<*i<<"': 'Show' not found\n'"; }     
     else {
         HG.thermal.setColorLabel(tcolor->getAttr("Thermal_Label"));
         HG.thermal.setColorMeter(tcolor->getAttr("Thermal_Meter"));
         HG.battery.setColorLabel(tcolor->getAttr("Battery_Label"));
         HG.battery.setColorMeter(tcolor->getAttr("Battery_Meter"));
         HG.cpu_load.setColorLabel(tcolor->getAttr("Load_Label"));
         HG.cpu_load.setColorMeter(tcolor->getAttr("Load_Meter"));
      }           
     const Tag *files=data->find("Files");
     if(files)
      FOR_EACH_CONST_TAG_OF(i,*files,"File")
       { 
         FF.NewFileName(heatload::e_find(i->getIntAttr("Id")),
                FileFinder::st_file(i->getAttr("Bezeichnung"),
                                    i->getAttr("Name"),
                                    i->getBoolAttr("OldStyle",false)));
       }
     HG.battery.setReadMaxCap(data->getBoolAttr("ReadMaxCap",false));
     show_sudo=data->getBoolAttr("ShowSudo",true);
     return;
   }
}

void gtk_acpi::save() const
{                  
  std::string file=std::string(getenv("HOME"))+"/.heatloadrc";
  std::ofstream datei(file.c_str());
  if (!datei.good())
   { 
    cerr << "Can't save preferences in "<<file<<'\n';
    return;
   }
  TagStream ts;
  ts.setEncoding("ISO-8859-1");

  Tag &data=ts.push_back(Tag("heatload-preferences"));
  Tag &widget=data.push_back(Tag("Widget")); 
  widget.setIntAttr("X",show_widget.x);
  widget.setIntAttr("Y",show_widget.y);
  widget.setIntAttr("RefreshRate",show_widget.refresh);
  widget.setBoolAttr("Decoration",show_widget.decoration);
  widget.setBoolAttr("Label",show_widget.label);
  widget.setBoolAttr("Graph",show_widget.graph);
  widget.setBoolAttr("swsusp",show_widget.swsusp);

  Tag &show=data.push_back(Tag("Show"));
  show.setBoolAttr("AC",HG.ac_adapter.Visible());
  show.setBoolAttr("Battery",HG.battery.Visible());
  show.setBoolAttr("Thermal",HG.thermal.Visible());
  show.setBoolAttr("Fan",HG.fan.Visible());
  show.setBoolAttr("CPU-Throttling",HG.cpu_throttling.Visible());
  show.setBoolAttr("CPU-Performance",HG.cpu_performance.Visible());

  Tag &tcolor=data.push_back(Tag("Color"));
  tcolor.setAttr("Thermal_Label",HG.thermal.ColorLabel());
  tcolor.setAttr("Thermal_Meter",HG.thermal.ColorMeter());
  tcolor.setAttr("Battery_Label",HG.battery.ColorLabel());
  tcolor.setAttr("Battery_Meter",HG.battery.ColorMeter());
  tcolor.setAttr("Load_Label",HG.cpu_load.ColorLabel());
  tcolor.setAttr("Load_Meter",HG.cpu_load.ColorMeter());

  Tag &files=data.push_back(Tag("Files"));
  for(FileFinder::FileMap_t::const_iterator i=FF.getFileMap().begin();i!=FF.getFileMap().end();++i)
   {
     Tag &f=files.push_back(Tag("File"));
     f.setAttr("Bezeichnung",i->second.bezeichnung);
     f.setIntAttr("Id",i->first);
     f.setAttr("Name",i->second.name);
     if(i->second.old_style)
        f.setBoolAttr("OldStyle",i->second.old_style);
   }

  data.setBoolAttr("ReadMaxCap",HG.battery.ReadMaxCap());
  data.setBoolAttr("ShowSudo",show_sudo);

  ts.write(datei);
}
