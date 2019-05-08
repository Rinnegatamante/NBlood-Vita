//-------------------------------------------------------------------------
/*
Copyright (C) 2010-2019 EDuke32 developers and contributors
Copyright (C) 2019 Nuke.YKT

This file is part of NBlood.

NBlood is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License version 2
as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
//-------------------------------------------------------------------------
#pragma once
#include "db.h"

void sfxInit(void);
void sfxTerm(void);
void sfxPlay3DSound(int x, int y, int z, int soundId, int nSector);
void sfxPlay3DSound(spritetype *pSprite, int soundId, int a3 = -1, int a4 = 0);
void sfxPlay3DSoundCP(spritetype* pSprite, int soundId, int a3 = -1, int a4 = 0, int pitch = 0);
void sfxKill3DSound(spritetype *pSprite, int a2 = -1, int a3 = -1);
void sfxKillAllSounds(void);
void sfxUpdate3DSounds(void);
void sfxSetReverb(bool toggle);
void sfxSetReverb2(bool toggle);