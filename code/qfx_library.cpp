/***************************************************************************
* Copyright (C) 2012-2016, Crystice Softworks.
* 
* This file is part of QeffectsGL source code.
* 
* QeffectsGL source code is free software; you can redistribute it 
* and/or modify it under the terms of the GNU General Public License 
* as published by the Free Software Foundation; either version 2 of 
* the License, or (at your option) any later version.
* 
* QeffectsGL source code is distributed in the hope that it will be 
* useful, but WITHOUT ANY WARRANTY; without even the implied 
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
* See the GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software 
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
***************************************************************************/
#include "qfx_library.hpp"
#include "qfx_log.hpp"
#include "qfx_settings.hpp"
#include "qfx_opengl.hpp"

//=========================================
// DLL Entry Point
//-----------------------------------------
// Init and shutdown global DLL data
//=========================================

BOOL APIENTRY DllMain( HANDLE, DWORD ul_reason_for_call, LPVOID )
{
    switch (ul_reason_for_call) 
	{
		case DLL_PROCESS_ATTACH:
			QFXLog::Instance();
			QFXSettings::Instance();
			break;
		case DLL_PROCESS_DETACH:
			gl::Shutdown();
			break;
		default:
			break;
    }
    return TRUE;
}