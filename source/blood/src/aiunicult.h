//-------------------------------------------------------------------------
/*
Copyright (C) 2010-2019 EDuke32 developers and contributors
Copyright (C) 2019 Nuke.YKT
Copyright (C) NoOne

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
#include "ai.h"

extern AISTATE GDXGenDudeIdleL;
extern AISTATE GDXGenDudeIdleW;
extern AISTATE GDXGenDudeSearchL;
extern AISTATE GDXGenDudeSearchW;
extern AISTATE GDXGenDudeGotoL;
extern AISTATE GDXGenDudeGotoW;
extern AISTATE GDXGenDudeDodgeL;
extern AISTATE GDXGenDudeDodgeD;
extern AISTATE GDXGenDudeDodgeW;
extern AISTATE GDXGenDudeChaseL;
extern AISTATE GDXGenDudeChaseD;
extern AISTATE GDXGenDudeChaseW;
extern AISTATE GDXGenDudeFireL;
extern AISTATE GDXGenDudeFireD;
extern AISTATE GDXGenDudeFireW;
extern AISTATE GDXGenDudeFire2L;
extern AISTATE GDXGenDudeFire2D;
extern AISTATE GDXGenDudeFire2W;
extern AISTATE GDXGenDudeRecoilL;
extern AISTATE GDXGenDudeRecoilD;
extern AISTATE GDXGenDudeRecoilW;
extern AISTATE GDGenDudeThrow;
extern AISTATE GDGenDudeThrow2;
extern AISTATE GDXGenDudePunch;
extern AISTATE GDXGenDudeRTesla;
extern AISTATE GDXGenDudeProne;
extern AISTATE GDXGenDudeTurn;

XSPRITE* getNextIncarnation(XSPRITE* pXSprite);
void killDudeLeech(spritetype* pLeech);
void removeLeech(spritetype* pLeech, bool delSprite = true);
void removeDudeStuff(spritetype* pSprite);
spritetype* leechIsDropped(spritetype* pSprite);
bool spriteIsUnderwater(spritetype* pSprite, bool oldWay);
bool sfxPlayGDXGenDudeSound(spritetype* pSprite, int mode, int data);
void aiGenDudeMoveForward(spritetype* pSprite, XSPRITE* pXSprite);
int getGenDudeMoveSpeed(spritetype* pSprite, int which, bool mul, bool shift);
bool TargetNearThing(spritetype* pSprite, int thingType);
int checkAttackState(spritetype* pSprite, XSPRITE* pXSprite);