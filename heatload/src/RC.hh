/* $Id: RC.hh,v 1.2 2002/12/18 13:29:17 thoma Exp $ */
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

#ifndef _HEAT_RC_H
#define _HEAT_RC_H

#include <Structs.hh>

namespace rc_file{
   void load(heatload::st_show &show_what,heatload::st_color &color,
                  heatload::st_widget &show_widget,bool &read_max_cap,
                  bool &show_sudo);
   void save(const heatload::st_show &show_what,const heatload::st_color &color,
             const heatload::st_widget &show_widget,const bool read_max_cap,
             const bool show_sudo);
}

#endif

