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

#ifndef   _GIZMO_HH_
#  define _GIZMO_HH_

#include <string>
#include <vector>
#include "itos.h"
#include "Structs.hh"
#include "FileFinder.hh"

class Gizmo{
   private:
      const heatload::e_find what;
   public:
      static const int invalid = -99;   
      struct st_value{int line_of_interest; int column_of_value; 
                      bool value_is_int; int column_of_unit;
             st_value() : line_of_interest(invalid),column_of_value(invalid),
                      value_is_int(false), column_of_unit(invalid) {}
             st_value(int i,int v,int u) : line_of_interest(i),column_of_value(v),
                      value_is_int(true), column_of_unit(u) {}
             st_value(int i,int v) : line_of_interest(i),column_of_value(v),
                      value_is_int(false), column_of_unit(invalid) {}
                      };
   private:
      st_value file_value;
      FileFinder::st_file file_for_values;
   protected:   

      bool visible;
      std::string color_label,color_meter;
      std::string s_value;
      int i_value;
      std::string einheit;
      const std::string &getFilenameForValues() const {return file_for_values.name;}
      const bool getFileOldStyleForValues() const {return file_for_values.old_style;}
public:
      virtual void get_value();

   public:
      Gizmo(const heatload::e_find _what) : what(_what),
                             visible(true), i_value(invalid) {}
      virtual ~Gizmo() {}


      virtual const std::string Value() ;
      int IValue() const {return i_value;}
      const std::string &Einheit() const {return einheit;}
      
      void setFileForValues(const FileFinder::st_file &s,const st_value &fv) 
         { file_for_values=s;file_value=fv;}

      void setValue(const std::string &s) { s_value=s;}
      void setValue(int i,const std::string &s) {i_value=i;einheit=s;}

      bool Visible() const {return visible;}
      void setVisible(const bool b) {visible=b;}
      void toggleVisible() {visible=!visible;}
      
      const std::string& ColorLabel() const {return color_label;}
      const std::string& ColorMeter() const {return color_meter;}
      void setColorLabel(const std::string &c) {color_label=c;}
      void setColorMeter(const std::string &c) {color_meter=c;}

};

class GizmoThermal : public Gizmo
{
    public:
      GizmoThermal(const heatload::e_find what) : Gizmo(what) {}
      const std::string Value() ;
};

class GizmoThrottling : public Gizmo
{
   public:
        struct st_state{std::string tstate;int state;std::string prozent;
               st_state(const std::string &t,const int s,const std::string &p)
                          :tstate(t),state(s),prozent(p) {}
                 };
   private:
        std::vector<st_state> vec_state;

   public:
        GizmoThrottling(const heatload::e_find what): Gizmo(what){};
         
        void load_thrott_file(const std::string &filename);
        const std::vector<st_state>& getVec() const {return vec_state;}
        const std::string prozent_from_state(const std::string &s);
};

class GizmoBattery : public Gizmo
{
   public:
     enum e_zustand{e_charging,e_discharging,e_unknown};
   private:
     bool read_max_cap;
     bool use_max_cap,present;
     e_zustand zustand;
     int present_rate_mW;
     int remaining_capacity_mWh;
     int max_capacity_mWh;
     int last_max_capacity_mWh;
     
     
     void read_info(const std::string &filename);

    public:
      GizmoBattery(const heatload::e_find what) : Gizmo(what),
                       read_max_cap(false),use_max_cap(true),
                       present(false),zustand(e_unknown),present_rate_mW(0),
                       remaining_capacity_mWh(0),max_capacity_mWh(0),
                       last_max_capacity_mWh(0) 
                        {}
      void load_info_file(const std::string &info_filename);
      void get_value();
      const std::string Value() ;

      void setValue(const bool p,const e_zustand z,const int pr,
               const int r)
          { present=p; zustand=z; present_rate_mW=pr,
            remaining_capacity_mWh=r; }
      void setCapacity(const int m,const int lm) {max_capacity_mWh=m;
                                            last_max_capacity_mWh=lm;}
      void setUseMaxCap(const bool b) {use_max_cap=b;}
      void toggleUseMaxCap() {use_max_cap=!use_max_cap;}
      bool UseMaxCap() const {return use_max_cap;}


      const bool ReadMaxCap() const {return read_max_cap;}
      void setReadMaxCap(const bool b) {read_max_cap=b;}
      const double DProzent() const 
         { double       q=     max_capacity_mWh;
           if(!use_max_cap) q=last_max_capacity_mWh;
           return remaining_capacity_mWh/q;
         }
      const int Prozent() const 
         { return int(DProzent()*100);}
      const std::string RemainingTime() const;

};

class GizmoLoad : public Gizmo
{
  public:
      GizmoLoad(const heatload::e_find what): Gizmo(what){};
      void get_value();
};


class HeatloadGizmo
{
   public:
        Gizmo ac_adapter;
        GizmoBattery battery;
        Gizmo fan;
        GizmoThrottling cpu_throttling;
        GizmoThrottling cpu_performance;
        GizmoThermal thermal;
        GizmoLoad cpu_load;

        HeatloadGizmo() : ac_adapter(heatload::eAC),battery(heatload::eBat),
                   fan(heatload::eFan),
                   cpu_throttling(heatload::eCPUthrottling),
                   cpu_performance(heatload::eCPUperformance),
                   thermal(heatload::eThermal),cpu_load(heatload::eLoad)
                     {};
   
};


#endif
