/* $Id: FileFinder.hh,v 1.3 2002/12/27 08:27:30 thoma Exp $ */
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


#ifndef _ACPI_FILEFINDER_HH
#define _ACPI_FILEFINDER_HH

#include "Structs.hh"
#include <map>
#include <vector>
class gtk_acpi;

class FileFinder
{
   public:
     struct st_file{std::string bezeichnung;std::string name;bool old_style;bool ok;
            st_file() : old_style(false),ok(false) {}
            st_file(const std::string &b,const std::string &n)
                  : bezeichnung(b),name(n),old_style(false),ok(false){}
            st_file(const std::string &b,const std::string n,const bool o)
                  : bezeichnung(b),name(n),old_style(o),ok(false){}};
     struct st_all{heatload::e_find EF; std::string name;
            st_all(const heatload::e_find e, const std::string n) :EF(e),name(n){}
            };

      typedef std::map<heatload::e_find,st_file> FileMap_t;
   private:
        std::map<heatload::e_find,std::vector<st_file> > VFiles;
        FileMap_t FileMap;

        void init();
        void find_filenames();
        std::string Bezeichnung(const heatload::e_find e);
        std::string looking_for(const heatload::e_find e);
        void set_dummy_file(st_file &FM,const heatload::e_find e);

   public:
        FileFinder();
        void find();

        const std::string &getFileName(heatload::e_find e) {return FileMap[e].name;}    
        bool check(const bool fix);

        const FileMap_t &getFileMap() const {return FileMap;}
        const st_file &getFile(heatload::e_find e) {return FileMap[e];}    

        void NewFileName(heatload::e_find e,const st_file &s)
               {VFiles[e].push_back(s);}
            
};


#endif
