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
#include "blood.h"
// By NoOne: renamed dude struct
struct DUDEINFO {
    short seqStartID; // seq
    short startHealth; // health
    unsigned short mass; // mass
    int at6; // unused?
    unsigned char clipdist; // clipdist
    int eyeHeight;
    int aimHeight; // used by just Cerberus
    int hearDist; // hear radius
    int seeDist; // sight radius
    int periphery; // periphery
    int meleeDist; // unused?
    int fleeHealth; // at which hp level enemy will turn in burning dude
    int hinderDamage; // recoil damage
    int changeTarget; // chance to change target when attacked someone else
    int changeTargetKin; // chance to change target when attacked by same type
    int alertChance;
    char lockOut; // indicates if this dude can trigger something via trigger flags
    int frontSpeed; // acceleration
    int sideSpeed; // dodge
    int backSpeed; // backward speed (unused)
    int angSpeed; // turn speed
    int nGibType[3]; // which gib used when explode dude
    int startDamage[7]; // start damage shift
    int at70[7]; // real damage? Hmm?
    int at8c; // unused ?
    int at90; // unused ?
};

extern DUDEINFO dudeInfo[kDudeMax-kDudeBase];
extern DUDEINFO gPlayerTemplate[2];