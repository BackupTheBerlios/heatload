/* $Id: RC.hh,v 1.6 2003/03/24 12:27:45 thoma Exp $ */
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

#include "Structs.hh"
#include "FileFinder.hh"

namespace rc_file{

   void load(HeatloadGizmo &HG,
             heatload::st_widget &show_widget,
             bool &show_sudo,FileFinder &FileMap, 
             std::vector<heatload::st_auto> &AutoVec);

}

#endif

