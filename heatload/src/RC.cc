/* $Id: RC.cc,v 1.3 2002/12/18 13:29:17 thoma Exp $ */
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

#include "RC.hh"
#include <vector>
#include <string>
#include <unistd.h>
#include <TagStream.hh>

void rc_file::load(heatload::st_show &show_what,heatload::st_color &color,
                  heatload::st_widget &show_widget,bool &read_max_cap,
                  bool &show_sudo)
{
  std::vector<std::string> V;
  char currentwd[10240];
  *currentwd=0;
  getcwd(currentwd,sizeof currentwd);
  V.push_back(std::string(currentwd)+"/.heatloadrc");
  V.push_back(std::string(getenv("HOME"))+"/.heatloadrc");
  V.push_back("/etc/heatload/heatload.conf");
  for(std::vector<std::string>::const_iterator i=V.begin();i!=V.end();++i)
   {
cout << "looking for "<<*i<<'\n';
     std::ifstream f(i->c_str());
     if (!f.good()) continue;
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
      }
     const Tag *show=data->find("Show");
     if(!show) {std::cerr << "Wrong format in '"<<*i<<"': 'Show' not found\n'"; }     
     else {
        show_what.ac=show->getBoolAttr("AC",true);
        show_what.bat=show->getBoolAttr("Battery",true);
        show_what.temp=show->getBoolAttr("Thermal",true);
        show_what.fan=show->getBoolAttr("Fan",true);
        show_what.cpu_throttling=show->getBoolAttr("CPU",true);
      }           
     const Tag *tcolor=data->find("Color");
     if(!show) {std::cerr << "Wrong format in '"<<*i<<"': 'Show' not found\n'"; }     
     else {
         color.temp_label = tcolor->getAttr("Thermal_Label");
         color.temp_meter = tcolor->getAttr("Thermal_Meter");
         color.bat_label = tcolor->getAttr("Battery_Label");
         color.bat_meter = tcolor->getAttr("Battery_Meter");
         color.load_label = tcolor->getAttr("Load_Label");
         color.load_meter = tcolor->getAttr("Load_Meter");
      }           
     read_max_cap=data->getBoolAttr("ReadMaxCap",false);
     show_sudo=data->getBoolAttr("ShowSudo",true);
     return;
   }
}

void rc_file::save(const heatload::st_show &show_what,const heatload::st_color &color,
                  const heatload::st_widget &show_widget,const bool read_max_cap,
                  const bool show_sudo)
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

  Tag &show=data.push_back(Tag("Show"));
  show.setBoolAttr("AC",show_what.ac);
  show.setBoolAttr("Battery",show_what.bat);
  show.setBoolAttr("Thermal",show_what.temp);
  show.setBoolAttr("Fan",show_what.fan);
  show.setBoolAttr("CPU",show_what.cpu_throttling);

  Tag &tcolor=data.push_back(Tag("Color"));
  tcolor.setAttr("Thermal_Label",color.temp_label);
  tcolor.setAttr("Thermal_Meter",color.temp_meter);
  tcolor.setAttr("Battery_Label",color.bat_label);
  tcolor.setAttr("Battery_Meter",color.bat_meter);
  tcolor.setAttr("Load_Label",color.load_label);
  tcolor.setAttr("Load_Meter",color.load_meter);

  data.setBoolAttr("ReadMaxCap",read_max_cap);
  data.setBoolAttr("ShowSudo",show_sudo);

  ts.write(datei);
}
