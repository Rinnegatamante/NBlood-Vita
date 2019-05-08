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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compat.h"
#include "build.h"
#include "mmulti.h"
#include "common_game.h"
#include "actor.h"
#include "blood.h"
#include "db.h"
#include "callback.h"
#include "config.h"
#include "eventq.h"
#include "fx.h"
#include "gameutil.h"
#include "globals.h"
#include "levels.h"
#include "loadsave.h"
#include "player.h"
#include "qav.h"
#include "resource.h"
#include "seq.h"
#include "sfx.h"
#include "sound.h"
#include "trig.h"
#include "triggers.h"
#include "view.h"

void FirePitchfork(int, PLAYER *pPlayer);
void FireSpray(int, PLAYER *pPlayer);
void ThrowCan(int, PLAYER *pPlayer);
void DropCan(int, PLAYER *pPlayer);
void ExplodeCan(int, PLAYER *pPlayer);
void ThrowBundle(int, PLAYER *pPlayer);
void DropBundle(int, PLAYER *pPlayer);
void ExplodeBundle(int, PLAYER *pPlayer);
void ThrowProx(int, PLAYER *pPlayer);
void DropProx(int, PLAYER *pPlayer);
void ThrowRemote(int, PLAYER *pPlayer);
void DropRemote(int, PLAYER *pPlayer);
void FireRemote(int, PLAYER *pPlayer);
void FireShotgun(int nTrigger, PLAYER *pPlayer);
void EjectShell(int, PLAYER *pPlayer);
void FireTommy(int nTrigger, PLAYER *pPlayer);
void FireSpread(int nTrigger, PLAYER *pPlayer);
void AltFireSpread(int nTrigger, PLAYER *pPlayer);
void AltFireSpread2(int nTrigger, PLAYER *pPlayer);
void FireFlare(int nTrigger, PLAYER *pPlayer);
void AltFireFlare(int nTrigger, PLAYER *pPlayer);
void FireVoodoo(int nTrigger, PLAYER *pPlayer);
void AltFireVoodoo(int nTrigger, PLAYER *pPlayer);
void DropVoodoo(int nTrigger, PLAYER *pPlayer);
void FireTesla(int nTrigger, PLAYER *pPlayer);
void AltFireTesla(int nTrigger, PLAYER *pPlayer);
void FireNapalm(int nTrigger, PLAYER *pPlayer);
void FireNapalm2(int nTrigger, PLAYER *pPlayer);
void AltFireNapalm(int nTrigger, PLAYER *pPlayer);
void FireLifeLeech(int nTrigger, PLAYER *pPlayer);
void AltFireLifeLeech(int nTrigger, PLAYER *pPlayer);
void FireBeast(int nTrigger, PLAYER * pPlayer);

typedef void(*QAVTypeCast)(int, void *);

int nClientFirePitchfork = qavRegisterClient((QAVTypeCast)FirePitchfork);
int nClientFireSpray = qavRegisterClient((QAVTypeCast)FireSpray);
int nClientThrowCan = qavRegisterClient((QAVTypeCast)ThrowCan);
int nClientDropCan = qavRegisterClient((QAVTypeCast)DropCan);
int nClientExplodeCan = qavRegisterClient((QAVTypeCast)ExplodeCan);
int nClientThrowBundle = qavRegisterClient((QAVTypeCast)ThrowBundle);
int nClientDropBundle = qavRegisterClient((QAVTypeCast)DropBundle);
int nClientExplodeBundle = qavRegisterClient((QAVTypeCast)ExplodeBundle);
int nClientThrowProx = qavRegisterClient((QAVTypeCast)ThrowProx);
int nClientDropProx = qavRegisterClient((QAVTypeCast)DropProx);
int nClientThrowRemote = qavRegisterClient((QAVTypeCast)ThrowRemote);
int nClientDropRemote = qavRegisterClient((QAVTypeCast)DropRemote);
int nClientFireRemote = qavRegisterClient((QAVTypeCast)FireRemote);
int nClientFireShotgun = qavRegisterClient((QAVTypeCast)FireShotgun);
int nClientEjectShell = qavRegisterClient((QAVTypeCast)EjectShell);
int nClientFireTommy = qavRegisterClient((QAVTypeCast)FireTommy);
int nClientAltFireSpread2 = qavRegisterClient((QAVTypeCast)AltFireSpread2);
int nClientFireSpread = qavRegisterClient((QAVTypeCast)FireSpread);
int nClientAltFireSpread = qavRegisterClient((QAVTypeCast)AltFireSpread);
int nClientFireFlare = qavRegisterClient((QAVTypeCast)FireFlare);
int nClientAltFireFlare = qavRegisterClient((QAVTypeCast)AltFireFlare);
int nClientFireVoodoo = qavRegisterClient((QAVTypeCast)FireVoodoo);
int nClientAltFireVoodoo = qavRegisterClient((QAVTypeCast)AltFireVoodoo);
int nClientFireTesla = qavRegisterClient((QAVTypeCast)FireTesla);
int nClientAltFireTesla = qavRegisterClient((QAVTypeCast)AltFireTesla);
int nClientFireNapalm = qavRegisterClient((QAVTypeCast)FireNapalm);
int nClientFireNapalm2 = qavRegisterClient((QAVTypeCast)FireNapalm2);
int nClientFireLifeLeech = qavRegisterClient((QAVTypeCast)FireLifeLeech);
int nClientFireBeast = qavRegisterClient((QAVTypeCast)FireBeast);
int nClientAltFireLifeLeech = qavRegisterClient((QAVTypeCast)AltFireLifeLeech);
int nClientDropVoodoo = qavRegisterClient((QAVTypeCast)DropVoodoo);
int nClientAltFireNapalm = qavRegisterClient((QAVTypeCast)AltFireNapalm);

#define kQAVEnd 125

QAV *weaponQAV[kQAVEnd];

void QAV::PlaySound(int nSound)
{
    sndStartSample(nSound, -1, -1, 0);
}

void QAV::PlaySound3D(spritetype *pSprite, int nSound, int a3, int a4)
{
    sfxPlay3DSound(pSprite, nSound, a3, a4);
}

char sub_4B1A4(PLAYER *pPlayer)
{
    switch (pPlayer->atbd)
    {
    case 7:
        switch (pPlayer->atc3)
        {
        case 5:
        case 6:
            return 1;
        }
        break;
    case 6:
        switch (pPlayer->atc3)
        {
        case 4:
        case 5:
        case 6:
            return 1;
        }
        break;
    }
    return 0;
}

char BannedUnderwater(int nWeapon)
{
    return nWeapon == 7 || nWeapon == 6;
}

char sub_4B1FC(PLAYER *pPlayer, int a2, int a3, int a4)
{
    if (gInfiniteAmmo)
        return 1;
    if (a3 == -1)
        return 1;
    if (a2 == 12 && pPlayer->atc7 == 11 && pPlayer->atc3 == 11)
        return 1;
    if (a2 == 9 && pPlayer->pXSprite->health > 0)
        return 1;
    return pPlayer->at181[a3] >= a4;
}

char CheckAmmo(PLAYER *pPlayer, int a2, int a3)
{
    if (gInfiniteAmmo)
        return 1;
    if (a2 == -1)
        return 1;
    if (pPlayer->atbd == 12 && pPlayer->atc7 == 11 && pPlayer->atc3 == 11)
        return 1;
    if (pPlayer->atbd == 9 && pPlayer->pXSprite->health >= (a3<<4))
        return 1;
    return pPlayer->at181[a2] >= a3;
}

char sub_4B2C8(PLAYER *pPlayer, int a2, int a3)
{
    if (gInfiniteAmmo)
        return 1;
    if (a2 == -1)
        return 1;
    return pPlayer->at181[a2] >= a3;
}

void SpawnBulletEject(PLAYER *pPlayer, int a2, int a3)
{
    POSTURE *pPosture = &gPosture[pPlayer->at5f][pPlayer->at2f];
    pPlayer->at67 = pPlayer->pSprite->z-pPosture->at24;
    int dz = pPlayer->at6f-(pPlayer->at6f-pPlayer->at67)/2;
    sub_74818(pPlayer->pSprite, dz, a2, a3);
}

void SpawnShellEject(PLAYER *pPlayer, int a2, int a3)
{
    POSTURE *pPosture = &gPosture[pPlayer->at5f][pPlayer->at2f];
    pPlayer->at67 = pPlayer->pSprite->z-pPosture->at24;
    int t = pPlayer->at6f - pPlayer->at67;
    int dz = pPlayer->at6f-t+(t>>2);
    sub_74A18(pPlayer->pSprite, dz, a2, a3);
}

void WeaponInit(void)
{
    for (int i = 0; i < kQAVEnd; i++)
    {
        DICTNODE *hRes = gSysRes.Lookup(i, "QAV");
        if (!hRes)
            ThrowError("Could not load QAV %d\n", i);
        weaponQAV[i] = (QAV*)gSysRes.Lock(hRes);
        weaponQAV[i]->nSprite = -1;
    }
}

void WeaponDraw(PLAYER *pPlayer, int a2, int a3, int a4, int a5)
{
    dassert(pPlayer != NULL);
    if (pPlayer->at26 == -1)
        return;
    QAV *pQAV = weaponQAV[pPlayer->at26];
    int v4;
    if (pPlayer->atbf == 0)
        v4 = gGameClock%pQAV->at10;
    else
        v4 = pQAV->at10-pPlayer->atbf;
    pQAV->x = a3;
    pQAV->y = a4;
    int flags = 2;
    int nInv = powerupCheck(pPlayer, 13);
    if (nInv >= 120*8 || (nInv != 0 && (gGameClock&32)))
    {
        a2 = -128;
        flags |= 1;
    }
    pQAV->Draw(v4, flags, a2, a5);
}

void WeaponPlay(PLAYER *pPlayer)
{
    dassert(pPlayer != NULL);
    if (pPlayer->at26 == -1)
        return;
    QAV *pQAV = weaponQAV[pPlayer->at26];
    pQAV->nSprite = pPlayer->pSprite->index;
    int nTicks = pQAV->at10 - pPlayer->atbf;
    pQAV->Play(nTicks-4, nTicks, pPlayer->at2a, pPlayer);
}

void StartQAV(PLAYER *pPlayer, int nWeaponQAV, int a3 = -1, char a4 = 0)
{
    dassert(nWeaponQAV < kQAVEnd);
    pPlayer->at26 = nWeaponQAV;
    pPlayer->atbf = weaponQAV[nWeaponQAV]->at10;
    pPlayer->at2a = a3;
    pPlayer->at1b1 = a4;
    weaponQAV[nWeaponQAV]->Preload();
    WeaponPlay(pPlayer);
    pPlayer->atbf -= 4;
}

struct WEAPONTRACK
{
    int at0; // x aim speed
    int at4; // y aim speed
    int at8; // angle range
    int atc;
    int at10; // predict
    bool bIsProjectile;
};

WEAPONTRACK gWeaponTrack[] = {
    { 0, 0, 0, 0, 0, false },
    { 0x6000, 0x6000, 0x71, 0x55, 0x111111, false },
    { 0x8000, 0x8000, 0x71, 0x55, 0x2aaaaa, true },
    { 0x10000, 0x10000, 0x38, 0x1c, 0, false },
    { 0x6000, 0x8000, 0x38, 0x1c, 0, false },
    { 0x6000, 0x6000, 0x38, 0x1c, 0x2aaaaa, true },
    { 0x6000, 0x6000, 0x71, 0x55, 0, true },
    { 0x6000, 0x6000, 0x71, 0x38, 0, true },
    { 0x8000, 0x10000, 0x71, 0x55, 0x255555, true },
    { 0x10000, 0x10000, 0x71, 0, 0, true },
    { 0x10000, 0x10000, 0xaa, 0, 0, false },
    { 0x6000, 0x6000, 0x71, 0x55, 0, true },
    { 0x6000, 0x6000, 0x71, 0x55, 0, true },
    { 0x6000, 0x6000, 0x71, 0x55, 0, false },
};

void UpdateAimVector(PLAYER * pPlayer)
{
    short nSprite;
    spritetype *pSprite;
    dassert(pPlayer != NULL);
    spritetype *pPSprite = pPlayer->pSprite;
    int x = pPSprite->x;
    int y = pPSprite->y;
    int z = pPlayer->at6f;
    Aim aim;
    aim.dx = Cos(pPSprite->ang)>>16;
    aim.dy = Sin(pPSprite->ang)>>16;
    aim.dz = pPlayer->at83;
    WEAPONTRACK *pWeaponTrack = &gWeaponTrack[pPlayer->atbd];
    int nTarget = -1;
    pPlayer->at1da = 0;
    if (gProfile[pPlayer->at57].nAutoAim == 1 || (gProfile[pPlayer->at57].nAutoAim == 2 && !pWeaponTrack->bIsProjectile) || pPlayer->atbd == 10 || pPlayer->atbd == 9)
    {
        int nClosest = 0x7fffffff;
        for (nSprite = headspritestat[6]; nSprite >= 0; nSprite = nextspritestat[nSprite])
        {
            pSprite = &sprite[nSprite];
            if (pSprite == pPSprite)
                continue;
            if (pSprite->hitag&32)
                continue;
            if (!(pSprite->hitag&8))
                continue;
            int x2 = pSprite->x;
            int y2 = pSprite->y;
            int z2 = pSprite->z;
            int nDist = approxDist(x2-x, y2-y);
            if (nDist == 0 || nDist > 51200)
                continue;
            if (pWeaponTrack->at10)
            {
                int t = divscale(nDist,pWeaponTrack->at10, 12);
                x2 += (xvel[nSprite]*t)>>12;
                y2 += (yvel[nSprite]*t)>>12;
                z2 += (zvel[nSprite]*t)>>8;
            }
            int lx = x + mulscale30(Cos(pPSprite->ang), nDist);
            int ly = y + mulscale30(Sin(pPSprite->ang), nDist);
            int lz = z + mulscale(pPlayer->at83, nDist, 10);
            int zRange = mulscale(9460, nDist, 10);
            int top, bottom;
            GetSpriteExtents(pSprite, &top, &bottom);
            if (lz-zRange>bottom || lz+zRange<top)
                continue;
            int angle = getangle(x2-x,y2-y);
            if (klabs(((angle-pPSprite->ang+1024)&2047)-1024) > pWeaponTrack->at8)
                continue;
            if (pPlayer->at1da < 16 && cansee(x,y,z,pPSprite->sectnum,x2,y2,z2,pSprite->sectnum))
                pPlayer->at1de[pPlayer->at1da++] = nSprite;
            // Inlined?
            int dz = (lz-z2)>>8;
            int dy = (ly-y2)>>4;
            int dx = (lx-x2)>>4;
            int nDist2 = ksqrt(dx*dx+dy*dy+dz*dz);
            if (nDist2 >= nClosest)
                continue;
            DUDEINFO *pDudeInfo = &dudeInfo[pSprite->type-kDudeBase];
            int center = (pSprite->yrepeat*pDudeInfo->aimHeight)<<2;
            int dzCenter = (z2-center)-z;
            if (cansee(x, y, z, pPSprite->sectnum, x2, y2, z2, pSprite->sectnum))
            {
                nClosest = nDist2;
                aim.dx = Cos(angle)>>16;
                aim.dy = Sin(angle)>>16;
                aim.dz = divscale(dzCenter, nDist, 10);
                nTarget = nSprite;
            }
        }
        if (pWeaponTrack->atc > 0)
        {
            for (nSprite = headspritestat[4]; nSprite >= 0; nSprite = nextspritestat[nSprite])
            {
                pSprite = &sprite[nSprite];
                if (!(pSprite->hitag&8))
                    continue;
                int x2 = pSprite->x;
                int y2 = pSprite->y;
                int z2 = pSprite->z;
                int dx = x2-x;
                int dy = y2-y;
                int dz = z2-z;
                int nDist = approxDist(dx, dy);
                if (nDist == 0 || nDist > 51200)
                    continue;
                int lx = x + mulscale30(Cos(pPSprite->ang), nDist);
                int ly = y + mulscale30(Sin(pPSprite->ang), nDist);
                int lz = z + mulscale(pPlayer->at83, nDist, 10);
                int zRange = mulscale10(9460, nDist);
                int top, bottom;
                GetSpriteExtents(pSprite, &top, &bottom);
                if (lz-zRange>bottom || lz+zRange<top)
                    continue;
                int angle = getangle(dx,dy);
                if (klabs(((angle-pPSprite->ang+1024)&2047)-1024) > pWeaponTrack->atc)
                    continue;
                if (pPlayer->at1da < 16 && cansee(x,y,z,pPSprite->sectnum,pSprite->x,pSprite->y,pSprite->z,pSprite->sectnum))
                    pPlayer->at1de[pPlayer->at1da++] = nSprite;
                // Inlined?
                int dz2 = (lz-z2)>>8;
                int dy2 = (ly-y2)>>4;
                int dx2 = (lx-x2)>>4;
                int nDist2 = ksqrt(dx2*dx2+dy2*dy2+dz2*dz2);
                if (nDist2 >= nClosest)
                    continue;
                if (cansee(x, y, z, pPSprite->sectnum, pSprite->x, pSprite->y, pSprite->z, pSprite->sectnum))
                {
                    nClosest = nDist2;
                    aim.dx = Cos(angle)>>16;
                    aim.dy = Sin(angle)>>16;
                    aim.dz = divscale(dz, nDist, 10);
                    nTarget = nSprite;
                }
            }
        }
    }
    Aim aim2;
    aim2 = aim;
    RotateVector((int*)&aim2.dx, (int*)&aim2.dy, -pPSprite->ang);
    aim2.dz -= pPlayer->at83;
    pPlayer->at1ca.dx = interpolate(pPlayer->at1ca.dx, aim2.dx, pWeaponTrack->at0);
    pPlayer->at1ca.dy = interpolate(pPlayer->at1ca.dy, aim2.dy, pWeaponTrack->at0);
    pPlayer->at1ca.dz = interpolate(pPlayer->at1ca.dz, aim2.dz, pWeaponTrack->at4);
    pPlayer->at1be = pPlayer->at1ca;
    RotateVector((int*)&pPlayer->at1be.dx, (int*)&pPlayer->at1be.dy, pPSprite->ang);
    pPlayer->at1be.dz += pPlayer->at83;
    pPlayer->at1d6 = nTarget;
}

struct t_WeaponModes
{
    int at0;
    int at4;
};

t_WeaponModes weaponModes[] = {
    { 0, -1 },
    { 1, -1 },
    { 1, 1 },
    { 1, 2 },
    { 1, 3 },
    { 1, 4 },
    { 1, 5 },
    { 1, 6 },
    { 1, 7 },
    { 1, 8 },
    { 1, 9 },
    { 1, 10 },
    { 1, 11 },
    { 0, -1 },
};

void WeaponRaise(PLAYER *pPlayer)
{
    dassert(pPlayer != NULL);
    int prevWeapon = pPlayer->atbd;
    pPlayer->atbd = pPlayer->atc.newWeapon;
    pPlayer->atc.newWeapon = 0;
    pPlayer->atc7 = weaponModes[pPlayer->atbd].at4;
    switch (pPlayer->atbd)
    {
    case 1: // pitchfork
        pPlayer->atc3 = 0;
        StartQAV(pPlayer, 0, -1, 0);
        break;
    case 7: // spraycan
        if (pPlayer->atc3 == 2)
        {
            pPlayer->atc3 = 3;
            StartQAV(pPlayer, 8, -1, 0);
        }
        else
        {
            pPlayer->atc3 = 0;
            StartQAV(pPlayer, 4, -1, 0);
        }
        break;
    case 6: // dynamite
        if (gInfiniteAmmo || sub_4B2C8(pPlayer, 5, 1))
        {
            pPlayer->atc3 = 3;
            if (prevWeapon == 7)
                StartQAV(pPlayer, 16, -1, 0);
            else
                StartQAV(pPlayer, 18, -1, 0);
        }
        break;
    case 11: // proximity
        if (gInfiniteAmmo || sub_4B2C8(pPlayer, 10, 1))
        {
            pPlayer->atc3 = 7;
            StartQAV(pPlayer, 25, -1, 0);
        }
        break;
    case 12: // remote
        if (gInfiniteAmmo || sub_4B2C8(pPlayer, 11, 1))
        {
            pPlayer->atc3 = 10;
            StartQAV(pPlayer, 31, -1, 0);
        }
        else
        {
            StartQAV(pPlayer, 32, -1, 0);
            pPlayer->atc3 = 11;
        }
        break;
    case 3: // sawed off
        if (powerupCheck(pPlayer, 17))
        {
            if (gInfiniteAmmo || pPlayer->at181[2] >= 4)
                StartQAV(pPlayer, 59, -1, 0);
            else
                StartQAV(pPlayer, 50, -1, 0);
            if (gInfiniteAmmo || pPlayer->at181[2] >= 4)
                pPlayer->atc3 = 7;
            else if (pPlayer->at181[2] > 1)
                pPlayer->atc3 = 3;
            else if (pPlayer->at181[2] > 0)
                pPlayer->atc3 = 2;
            else
                pPlayer->atc3 = 1;
        }
        else
        {
            if (gInfiniteAmmo || pPlayer->at181[2] > 1)
                pPlayer->atc3 = 3;
            else if (pPlayer->at181[2] > 0)
                pPlayer->atc3 = 2;
            else
                pPlayer->atc3 = 1;
            StartQAV(pPlayer, 50, -1, 0);
        }
        break;
    case 4: // tommy gun
        if (powerupCheck(pPlayer, 17) && sub_4B2C8(pPlayer, 3, 2))
        {
            pPlayer->atc3 = 1;
            StartQAV(pPlayer, 69, -1, 0);
        }
        else
        {
            pPlayer->atc3 = 0;
            StartQAV(pPlayer, 64, -1, 0);
        }
        break;
    case 10: // voodoo
        if (gInfiniteAmmo || sub_4B2C8(pPlayer, 9, 1))
        {
            pPlayer->atc3 = 2;
            StartQAV(pPlayer, 100, -1, 0);
        }
        break;
    case 2: // flaregun
        if (powerupCheck(pPlayer, 17) && sub_4B2C8(pPlayer, 1, 2))
        {
            StartQAV(pPlayer, 45, -1, 0);
            pPlayer->atc3 = 3;
        }
        else
        {
            StartQAV(pPlayer, 41, -1, 0);
            pPlayer->atc3 = 2;
        }
        break;
    case 8: // tesla cannon
        if (sub_4B2C8(pPlayer, 7, 1))
        {
            pPlayer->atc3 = 2;
            if (powerupCheck(pPlayer, 17))
                StartQAV(pPlayer, 82, -1, 0);
            else
                StartQAV(pPlayer, 74, -1, 0);
        }
        else
        {
            pPlayer->atc3 = 3;
            StartQAV(pPlayer, 74, -1, 0);
        }
        break;
    case 5: // napalm
        if (powerupCheck(pPlayer, 17))
        {
            StartQAV(pPlayer, 120, -1, 0);
            pPlayer->atc3 = 3;
        }
        else
        {
            StartQAV(pPlayer, 89, -1, 0);
            pPlayer->atc3 = 2;
        }
        break;
    case 9: // life leech
        pPlayer->atc3 = 2;
        StartQAV(pPlayer, 111, -1, 0);
        break;
    case 13: // beast
        pPlayer->atc3 = 2;
        StartQAV(pPlayer, 93, -1, 0);
        break;
    }
}

void WeaponLower(PLAYER *pPlayer)
{
    dassert(pPlayer != NULL);
    if (sub_4B1A4(pPlayer))
        return;
    pPlayer->at1ba = 0;
    int prevState = pPlayer->atc3;
    switch (pPlayer->atbd)
    {
    case 1:
        StartQAV(pPlayer, 3, -1, 0);
        break;
    case 7:
        sfxKill3DSound(pPlayer->pSprite, -1, 441);
        switch (prevState)
        {
        case 1:
            StartQAV(pPlayer, 7, -1, 0);
            break;
        case 2:
            pPlayer->atc3 = 1;
            WeaponRaise(pPlayer);
            return;
        case 4:
            pPlayer->atc3 = 1;
            StartQAV(pPlayer, 11, -1, 0);
            pPlayer->atc.newWeapon = 0;
            WeaponLower(pPlayer);
            break;
        case 3:
            if (pPlayer->atc.newWeapon == 6)
            {
                pPlayer->atc3 = 2;
                StartQAV(pPlayer, 11, -1, 0);
                return;
            }
            else if (pPlayer->atc.newWeapon == 7)
            {
                pPlayer->atc3 = 1;
                StartQAV(pPlayer, 11, -1, 0);
                pPlayer->atc.newWeapon = 0;
                WeaponLower(pPlayer);
            }
            else
            {
                pPlayer->atc3 = 1;
                StartQAV(pPlayer, 11, -1, 0);
            }
            break;
        }
        break;
    case 6:
        switch (prevState)
        {
        case 1:
            StartQAV(pPlayer, 7, -1, 0);
            break;
        case 2:
            WeaponRaise(pPlayer);
            break;
        case 3:
            if (pPlayer->atc.newWeapon == 7)
            {
                pPlayer->atc3 = 2;
                StartQAV(pPlayer, 17, -1, 0);
            }
            else
            {
                StartQAV(pPlayer, 19, -1, 0);
            }
            break;
        default:
            break;
        }
        break;
    case 11:
        switch (prevState)
        {
        case 7:
            StartQAV(pPlayer, 26, -1, 0);
            break;
        }
        break;
    case 12:
        switch (prevState)
        {
        case 10:
            StartQAV(pPlayer, 34, -1, 0);
            break;
        case 11:
            StartQAV(pPlayer, 35, -1, 0);
            break;
        }
        break;
    case 3:
        if (powerupCheck(pPlayer, 17))
            StartQAV(pPlayer, 63, -1, 0);
        else
            StartQAV(pPlayer, 58, -1, 0);
        break;
    case 4:
        if (powerupCheck(pPlayer, 17) && pPlayer->atc3 == 1)
            StartQAV(pPlayer, 72, -1, 0);
        else
            StartQAV(pPlayer, 68, -1, 0);
        break;
    case 2:
        if (powerupCheck(pPlayer, 17) && pPlayer->atc3 == 3)
            StartQAV(pPlayer, 49, -1, 0);
        else
            StartQAV(pPlayer, 44, -1, 0);
        break;
    case 10:
        StartQAV(pPlayer, 109, -1, 0);
        break;
    case 8:
        if (sub_4B2C8(pPlayer, 7, 10) && powerupCheck(pPlayer, 17))
            StartQAV(pPlayer, 88, -1, 0);
        else
            StartQAV(pPlayer, 81, -1, 0);
        break;
    case 5:
        if (powerupCheck(pPlayer, 17))
            StartQAV(pPlayer, 124, -1, 0);
        else
            StartQAV(pPlayer, 92, -1, 0);
        break;
    case 9:
        StartQAV(pPlayer, 119, -1, 0);
        break;
    case 13:
        StartQAV(pPlayer, 99, -1, 0);
        break;
    }
    pPlayer->atbd = 0;
    pPlayer->at1b1 = 0;
}

void WeaponUpdateState(PLAYER *pPlayer)
{
    static int lastWeapon = 0;
    static int lastState = 0;
    XSPRITE *pXSprite = pPlayer->pXSprite;
    int va = pPlayer->atbd;
    int vb = pPlayer->atc3;
    if (va != lastWeapon || vb != lastState)
    {
        lastWeapon = va;
        lastState = vb;
    }
    switch (lastWeapon)
    {
    case 1:
        pPlayer->at26 = 1;
        break;
    case 7:
        switch (vb)
        {
        case 0:
            pPlayer->atc3 = 1;
            StartQAV(pPlayer, 5, -1, 0);
            break;
        case 1:
            if (CheckAmmo(pPlayer, 6, 1))
            {
                pPlayer->atc3 = 3;
                StartQAV(pPlayer, 8, -1, 0);
            }
            else
                pPlayer->at26 = 6;
            break;
        case 3:
            pPlayer->at26 = 9;
            break;
        case 4:
            if (CheckAmmo(pPlayer, 6, 1))
            {
                pPlayer->at26 = 9;
                pPlayer->atc3 = 3;
            }
            else
            {
                pPlayer->atc3 = 1;
                StartQAV(pPlayer, 11, -1, 0);
            }
            sfxKill3DSound(pPlayer->pSprite, -1, 441);
            break;
        }
        break;
    case 6:
        switch (vb)
        {
        case 1:
            if (pPlayer->atc7 == 5 && CheckAmmo(pPlayer, 5, 1))
            {
                pPlayer->atc3 = 3;
                StartQAV(pPlayer, 16, -1, 0);
            }
            break;
        case 0:
            pPlayer->atc3 = 1;
            StartQAV(pPlayer, 5, -1, 0);
            break;
        case 2:
            if (pPlayer->at181[5] > 0)
            {
                pPlayer->atc3 = 3;
                StartQAV(pPlayer, 16, -1, 0);
            }
            else
                pPlayer->at26 = 6;
            break;
        case 3:
            pPlayer->at26 = 20;
            break;
        }
        break;
    case 11:
        switch (vb)
        {
        case 7:
            pPlayer->at26 = 27;
            break;
        case 8:
            pPlayer->atc3 = 7;
            StartQAV(pPlayer, 25, -1, 0);
            break;
        }
        break;
    case 12:
        switch (vb)
        {
        case 10:
            pPlayer->at26 = 36;
            break;
        case 11:
            pPlayer->at26 = 37;
            break;
        case 12:
            if (pPlayer->at181[11] > 0)
            {
                pPlayer->atc3 = 10;
                StartQAV(pPlayer, 31, -1, 0);
            }
            else
                pPlayer->atc3 = -1;
            break;
        }
        break;
    case 3:
        switch (vb)
        {
        case 6:
            if (powerupCheck(pPlayer, 17) && (gInfiniteAmmo || CheckAmmo(pPlayer, 2, 4)))
                pPlayer->atc3 = 7;
            else
                pPlayer->atc3 = 1;
            break;
        case 7:
            pPlayer->at26 = 60;
            break;
        case 1:
            if (CheckAmmo(pPlayer, 2, 1))
            {
                sfxPlay3DSound(pPlayer->pSprite, 410, 3, 2);
                StartQAV(pPlayer, 57, nClientEjectShell, 0);
                if (gInfiniteAmmo || pPlayer->at181[2] > 1)
                    pPlayer->atc3 = 3;
                else
                    pPlayer->atc3 = 2;
            }
            else
                pPlayer->at26 = 51;
            break;
        case 2:
            pPlayer->at26 = 52;
            break;
        case 3:
            pPlayer->at26 = 53;
            break;
        }
        break;
    case 4:
        if (powerupCheck(pPlayer, 17) && sub_4B2C8(pPlayer, 3, 2))
        {
            pPlayer->at26 = 70;
            pPlayer->atc3 = 1;
        }
        else
        {
            pPlayer->at26 = 65;
            pPlayer->atc3 = 0;
        }
        break;
    case 2:
        if (powerupCheck(pPlayer, 17))
        {
            if (vb == 3 && sub_4B2C8(pPlayer, 1, 2))
                pPlayer->at26 = 46;
            else
            {
                pPlayer->at26 = 42;
                pPlayer->atc3 = 2;
            }
        }
        else
            pPlayer->at26 = 42;
        break;
    case 10:
        if (pXSprite->height < 256 && klabs(pPlayer->at4f) > 768)
            pPlayer->at26 = 102;
        else
            pPlayer->at26 = 101;
        break;
    case 8:
        switch (vb)
        {
        case 2:
            if (sub_4B2C8(pPlayer, 7, 10) && powerupCheck(pPlayer, 17))
                pPlayer->at26 = 83;
            else
                pPlayer->at26 = 75;
            break;
        case 3:
            pPlayer->at26 = 76;
            break;
        }
        break;
    case 5:
        switch (vb)
        {
        case 3:
            if (powerupCheck(pPlayer, 17) && (gInfiniteAmmo || CheckAmmo(pPlayer,4, 4)))
                pPlayer->at26 = 121;
            else
                pPlayer->at26 = 90;
            break;
        case 2:
            pPlayer->at26 = 90;
            break;
        }
        break;
    case 9:
        switch (vb)
        {
        case 2:
            pPlayer->at26 = 112;
            break;
        }
        break;
    case 13:
        pPlayer->at26 = 94;
        break;
    }
}

void FirePitchfork(int, PLAYER *pPlayer)
{
    Aim *aim = &pPlayer->at1be;
    int r1 = Random2(2000);
    int r2 = Random2(2000);
    int r3 = Random2(2000);
    for (int i = 0; i < 4; i++)
        actFireVector(pPlayer->pSprite, (2*i-3)*40, pPlayer->at6f-pPlayer->pSprite->z, aim->dx+r1, aim->dy+r2, aim->dz+r3, VECTOR_TYPE_0);
}

void FireSpray(int, PLAYER *pPlayer)
{
    playerFireMissile(pPlayer, 0, pPlayer->at1be.dx, pPlayer->at1be.dy, pPlayer->at1be.dz, 304);
    UseAmmo(pPlayer, 6, 4);
    if (CheckAmmo(pPlayer, 6, 1))
        sfxPlay3DSound(pPlayer->pSprite, 441, 1, 2);
    else
        sfxKill3DSound(pPlayer->pSprite, -1, 441);
}

void ThrowCan(int, PLAYER *pPlayer)
{
    sfxKill3DSound(pPlayer->pSprite, -1, 441);
    int nSpeed = mulscale16(pPlayer->at1ba, 0x177777)+0x66666;
    sfxPlay3DSound(pPlayer->pSprite, 455, 1, 0);
    spritetype *pSprite = playerFireThing(pPlayer, 0, -9460, 420, nSpeed);
    if (pSprite)
    {
        sfxPlay3DSound(pSprite, 441, 0, 0);
        evPost(pSprite->index, 3, pPlayer->at1b2, COMMAND_ID_1);
        int nXSprite = pSprite->extra;
        XSPRITE *pXSprite = &xsprite[nXSprite];
        pXSprite->Impact = 1;
        UseAmmo(pPlayer, 6, gAmmoItemData[0].at8);
        pPlayer->at1ba = 0;
    }
}

void DropCan(int, PLAYER *pPlayer)
{
    sfxKill3DSound(pPlayer->pSprite, -1, 441);
    spritetype *pSprite = playerFireThing(pPlayer, 0, 0, 420, 0);
    if (pSprite)
    {
        evPost(pSprite->index, 3, pPlayer->at1b2, COMMAND_ID_1);
        UseAmmo(pPlayer, 6, gAmmoItemData[0].at8);
    }
}

void ExplodeCan(int, PLAYER *pPlayer)
{
    sfxKill3DSound(pPlayer->pSprite, -1, 441);
    spritetype *pSprite = playerFireThing(pPlayer, 0, 0, 420, 0);
    evPost(pSprite->index, 3, 0, COMMAND_ID_1);
    UseAmmo(pPlayer, 6, gAmmoItemData[0].at8);
    StartQAV(pPlayer, 15, -1);
    pPlayer->atbd = 0;
    pPlayer->at1ba = 0;
}

void ThrowBundle(int, PLAYER *pPlayer)
{
    sfxKill3DSound(pPlayer->pSprite, 16, -1);
    int nSpeed = mulscale16(pPlayer->at1ba, 0x177777)+0x66666;
    sfxPlay3DSound(pPlayer->pSprite, 455, 1, 0);
    spritetype *pSprite = playerFireThing(pPlayer, 0, -9460, 419, nSpeed);
    int nXSprite = pSprite->extra;
    XSPRITE *pXSprite = &xsprite[nXSprite];
    if (pPlayer->at1b2 < 0)
        pXSprite->Impact = 1;
    else
        evPost(pSprite->index, 3, pPlayer->at1b2, COMMAND_ID_1);
    UseAmmo(pPlayer, 5, 1);
    pPlayer->at1ba = 0;
}

void DropBundle(int, PLAYER *pPlayer)
{
    sfxKill3DSound(pPlayer->pSprite, 16, -1);
    spritetype *pSprite = playerFireThing(pPlayer, 0, 0, 419, 0);
    evPost(pSprite->index, 3, pPlayer->at1b2, COMMAND_ID_1);
    UseAmmo(pPlayer, 5, 1);
}

void ExplodeBundle(int, PLAYER *pPlayer)
{
    sfxKill3DSound(pPlayer->pSprite, 16, -1);
    spritetype *pSprite = playerFireThing(pPlayer, 0, 0, 419, 0);
    evPost(pSprite->index, 3, 0, COMMAND_ID_1);
    UseAmmo(pPlayer, 5, 1);
    StartQAV(pPlayer, 24, -1, 0);
    pPlayer->atbd = 0;
    pPlayer->at1ba = 0;
}

void ThrowProx(int, PLAYER *pPlayer)
{
    int nSpeed = mulscale16(pPlayer->at1ba, 0x177777)+0x66666;
    sfxPlay3DSound(pPlayer->pSprite, 455, 1, 0);
    spritetype *pSprite = playerFireThing(pPlayer, 0, -9460, 401, nSpeed);
    evPost(pSprite->index, 3, 240, COMMAND_ID_1);
    UseAmmo(pPlayer, 10, 1);
    pPlayer->at1ba = 0;
}

void DropProx(int, PLAYER *pPlayer)
{
    spritetype *pSprite = playerFireThing(pPlayer, 0, 0, 401, 0);
    evPost(pSprite->index, 3, 240, COMMAND_ID_1);
    UseAmmo(pPlayer, 10, 1);
}

void ThrowRemote(int, PLAYER *pPlayer)
{
    int nSpeed = mulscale16(pPlayer->at1ba, 0x177777)+0x66666;
    sfxPlay3DSound(pPlayer->pSprite, 455, 1, 0);
    spritetype *pSprite = playerFireThing(pPlayer, 0, -9460, 402, nSpeed);
    int nXSprite = pSprite->extra;
    XSPRITE *pXSprite = &xsprite[nXSprite];
    pXSprite->rxID = 90+(pPlayer->pSprite->type-kDudePlayer1);
    UseAmmo(pPlayer, 11, 1);
    pPlayer->at1ba = 0;
}

void DropRemote(int, PLAYER *pPlayer)
{
    spritetype *pSprite = playerFireThing(pPlayer, 0, 0, 402, 0);
    int nXSprite = pSprite->extra;
    XSPRITE *pXSprite = &xsprite[nXSprite];
    pXSprite->rxID = 90+(pPlayer->pSprite->type-kDudePlayer1);
    UseAmmo(pPlayer, 11, 1);
}

void FireRemote(int, PLAYER *pPlayer)
{
    evSend(0, 0, 90+(pPlayer->pSprite->type-kDudePlayer1), COMMAND_ID_1);
}

#define kMaxShotgunBarrels 4

void FireShotgun(int nTrigger, PLAYER *pPlayer)
{
    dassert(nTrigger > 0 && nTrigger <= kMaxShotgunBarrels);
    if (nTrigger == 1)
    {
        sfxPlay3DSound(pPlayer->pSprite, 411, 2, 0);
        pPlayer->at35e = 30;
        pPlayer->at362 = 20;
    }
    else
    {
        sfxPlay3DSound(pPlayer->pSprite, 412, 2, 0);
        pPlayer->at35e = 50;
        pPlayer->at362 = 40;
    }
    int n = nTrigger<<4;
    for (int i = 0; i < n; i++)
    {
        int r1, r2, r3;
        VECTOR_TYPE nType;
        if (nTrigger == 1)
        {
            r1 = Random3(1500);
            r2 = Random3(1500);
            r3 = Random3(500);
            nType = VECTOR_TYPE_1;
        }
        else
        {
            r1 = Random3(2500);
            r2 = Random3(2500);
            r3 = Random3(1500);
            nType = VECTOR_TYPE_4;
        }
        actFireVector(pPlayer->pSprite, 0, pPlayer->at6f-pPlayer->pSprite->z, pPlayer->at1be.dx+r1, pPlayer->at1be.dy+r2, pPlayer->at1be.dz+r3, nType);
    }
    UseAmmo(pPlayer, pPlayer->atc7, nTrigger);
    pPlayer->at37b = 1;
}

void EjectShell(int, PLAYER *pPlayer)
{
    SpawnShellEject(pPlayer, 25, 35);
    SpawnShellEject(pPlayer, 48, 35);
}

void FireTommy(int nTrigger, PLAYER *pPlayer)
{
    Aim *aim = &pPlayer->at1be;
    sfxPlay3DSound(pPlayer->pSprite, 431, -1, 0);
    switch (nTrigger)
    {
    case 1:
    {
        int r1 = Random3(400);
        int r2 = Random3(1200);
        int r3 = Random3(1200);
        actFireVector(pPlayer->pSprite, 0, pPlayer->at6f-pPlayer->pSprite->z, aim->dx+r3, aim->dy+r2, aim->dz+r1, VECTOR_TYPE_5);
        SpawnBulletEject(pPlayer, -15, -45);
        pPlayer->at362 = 20;
        break;
    }
    case 2:
    {
        int r1 = Random3(400);
        int r2 = Random3(1200);
        int r3 = Random3(1200);
        actFireVector(pPlayer->pSprite, -120, pPlayer->at6f-pPlayer->pSprite->z, aim->dx+r3, aim->dy+r2, aim->dz+r1, VECTOR_TYPE_5);
        SpawnBulletEject(pPlayer, -140, -45);
        r1 = Random3(400);
        r2 = Random3(1200);
        r3 = Random3(1200);
        actFireVector(pPlayer->pSprite, 120, pPlayer->at6f-pPlayer->pSprite->z, aim->dx+r3, aim->dy+r2, aim->dz+r1, VECTOR_TYPE_5);
        SpawnBulletEject(pPlayer, 140, 45);
        pPlayer->at362 = 30;
        break;
    }
    }
    UseAmmo(pPlayer, pPlayer->atc7, nTrigger);
    pPlayer->at37b = 1;
}

#define kMaxSpread 14

void FireSpread(int nTrigger, PLAYER *pPlayer)
{
    dassert(nTrigger > 0 && nTrigger <= kMaxSpread);
    Aim *aim = &pPlayer->at1be;
    int angle = (getangle(aim->dx, aim->dy)+((112*(nTrigger-1))/14-56))&2047;
    int dx = Cos(angle)>>16;
    int dy = Sin(angle)>>16;
    sfxPlay3DSound(pPlayer->pSprite, 431, -1, 0);
    int r1, r2, r3;
    r1 = Random3(300);
    r2 = Random3(600);
    r3 = Random3(600);
    actFireVector(pPlayer->pSprite, 0, pPlayer->at6f-pPlayer->pSprite->z, dx+r3, dy+r2, aim->dz+r1, VECTOR_TYPE_3);
    r1 = Random2(90);
    r2 = Random2(30);
    SpawnBulletEject(pPlayer, r2, r1);
    pPlayer->at362 = 20;
    UseAmmo(pPlayer, pPlayer->atc7, 1);
    pPlayer->at37b = 1;
}

void AltFireSpread(int nTrigger, PLAYER *pPlayer)
{
    dassert(nTrigger > 0 && nTrigger <= kMaxSpread);
    Aim *aim = &pPlayer->at1be;
    int angle = (getangle(aim->dx, aim->dy)+((112*(nTrigger-1))/14-56))&2047;
    int dx = Cos(angle)>>16;
    int dy = Sin(angle)>>16;
    sfxPlay3DSound(pPlayer->pSprite, 431, -1, 0);
    int r1, r2, r3;
    r1 = Random3(300);
    r2 = Random3(600);
    r3 = Random3(600);
    actFireVector(pPlayer->pSprite, -120, pPlayer->at6f-pPlayer->pSprite->z, dx+r3, dy+r2, aim->dz+r1, VECTOR_TYPE_3);
    r1 = Random2(45);
    r2 = Random2(120);
    SpawnBulletEject(pPlayer, r2, r1);
    r1 = Random3(300);
    r2 = Random3(600);
    r3 = Random3(600);
    actFireVector(pPlayer->pSprite, 120, pPlayer->at6f-pPlayer->pSprite->z, dx+r3, dy+r2, aim->dz+r1, VECTOR_TYPE_3);
    r1 = Random2(-45);
    r2 = Random2(-120);
    SpawnBulletEject(pPlayer, r2, r1);
    pPlayer->at35e = 20;
    pPlayer->at362 = 30;
    UseAmmo(pPlayer, pPlayer->atc7, 2);
    pPlayer->at37b = 1;
}

void AltFireSpread2(int nTrigger, PLAYER *pPlayer)
{
    dassert(nTrigger > 0 && nTrigger <= kMaxSpread);
    Aim *aim = &pPlayer->at1be;
    int angle = (getangle(aim->dx, aim->dy)+((112*(nTrigger-1))/14-56))&2047;
    int dx = Cos(angle)>>16;
    int dy = Sin(angle)>>16;
    sfxPlay3DSound(pPlayer->pSprite, 431, -1, 0);
    if (powerupCheck(pPlayer, 17) && sub_4B2C8(pPlayer, 3, 2))
    {
        int r1, r2, r3;
        r1 = Random3(300);
        r2 = Random3(600);
        r3 = Random3(600);
        actFireVector(pPlayer->pSprite, -120, pPlayer->at6f-pPlayer->pSprite->z, dx+r3, dy+r2, aim->dz+r1, VECTOR_TYPE_3);
        r1 = Random2(45);
        r2 = Random2(120);
        SpawnBulletEject(pPlayer, r2, r1);
        r1 = Random3(300);
        r2 = Random3(600);
        r3 = Random3(600);
        actFireVector(pPlayer->pSprite, 120, pPlayer->at6f-pPlayer->pSprite->z, dx+r3, dy+r2, aim->dz+r1, VECTOR_TYPE_3);
        r1 = Random2(-45);
        r2 = Random2(-120);
        SpawnBulletEject(pPlayer, r2, r1);
        pPlayer->at35e = 30;
        pPlayer->at362 = 45;
        UseAmmo(pPlayer, pPlayer->atc7, 2);
    }
    else
    {
        int r1, r2, r3;
        r1 = Random3(300);
        r2 = Random3(600);
        r3 = Random3(600);
        actFireVector(pPlayer->pSprite, 0, pPlayer->at6f-pPlayer->pSprite->z, dx+r3, dy+r2, aim->dz+r1, VECTOR_TYPE_3);
        r1 = Random2(90);
        r2 = Random2(30);
        SpawnBulletEject(pPlayer, r2, r1);
        pPlayer->at35e = 20;
        pPlayer->at362 = 30;
        UseAmmo(pPlayer, pPlayer->atc7, 1);
    }
    pPlayer->at37b = 1;
    if (!sub_4B2C8(pPlayer, 3, 1))
    {
        WeaponLower(pPlayer);
        pPlayer->atc3 = -1;
    }
}

void FireFlare(int nTrigger, PLAYER *pPlayer)
{
    spritetype *pSprite = pPlayer->pSprite;
    int offset = 0;
    switch (nTrigger)
    {
    case 2:
        offset = -120;
        break;
    case 3:
        offset = 120;
        break;
    }
    playerFireMissile(pPlayer, offset, pPlayer->at1be.dx, pPlayer->at1be.dy, pPlayer->at1be.dz, 301);
    UseAmmo(pPlayer, 1, 1);
    sfxPlay3DSound(pSprite, 420, 2, 0);
    pPlayer->at362 = 30;
    pPlayer->at37b = 1;
}

void AltFireFlare(int nTrigger, PLAYER *pPlayer)
{
    spritetype *pSprite = pPlayer->pSprite;
    int offset = 0;
    switch (nTrigger)
    {
    case 2:
        offset = -120;
        break;
    case 3:
        offset = 120;
        break;
    }
    playerFireMissile(pPlayer, offset, pPlayer->at1be.dx, pPlayer->at1be.dy, pPlayer->at1be.dz, 303);
    UseAmmo(pPlayer, 1, 8);
    sfxPlay3DSound(pSprite, 420, 2, 0);
    pPlayer->at362 = 45;
    pPlayer->at37b = 1;
}

void FireVoodoo(int nTrigger, PLAYER *pPlayer)
{
    nTrigger--;
    int nSprite = pPlayer->at5b;
    spritetype *pSprite = pPlayer->pSprite;
    if (nTrigger == 4)
    {
        actDamageSprite(nSprite, pSprite, DAMAGE_TYPE_2, 1<<4);
        return;
    }
    dassert(pPlayer->voodooTarget >= 0);
    spritetype *pTarget = &sprite[pPlayer->voodooTarget];
    switch (nTrigger)
    {
    case 0:
    {
        sfxPlay3DSound(pSprite, 460, 2, 0);
        fxSpawnBlood(pTarget, 17<<4);
        int nDamage = actDamageSprite(nSprite, pTarget, DAMAGE_TYPE_5, 17<<4);
        UseAmmo(pPlayer, 9, nDamage/4);
        break;
    }
    case 1:
    {
        sfxPlay3DSound(pSprite, 460, 2, 0);
        fxSpawnBlood(pTarget, 17<<4);
        int nDamage = actDamageSprite(nSprite, pTarget, DAMAGE_TYPE_5, 9<<4);
        if (pTarget->type >= kDudePlayer1 && pTarget->type <= kDudePlayer8)
            WeaponLower(&gPlayer[pTarget->type-kDudePlayer1]);
        UseAmmo(pPlayer, 9, nDamage/4);
        break;
    }
    case 3:
    {
        sfxPlay3DSound(pSprite, 463, 2, 0);
        fxSpawnBlood(pTarget, 17<<4);
        int nDamage = actDamageSprite(nSprite, pTarget, DAMAGE_TYPE_5, 49<<4);
        UseAmmo(pPlayer, 9, nDamage/4);
        break;
    }
    case 2:
    {
        sfxPlay3DSound(pSprite, 460, 2, 0);
        fxSpawnBlood(pTarget, 17<<4);
        int nDamage = actDamageSprite(nSprite, pTarget, DAMAGE_TYPE_5, 11<<4);
        if (pTarget->type >= kDudePlayer1 && pTarget->type <= kDudePlayer8)
        {
            PLAYER *pOtherPlayer = &gPlayer[pTarget->type-kDudePlayer1];
            pOtherPlayer->at36a = 128;
        }
        UseAmmo(pPlayer, 9, nDamage/4);
        break;
    }
    }
}

void AltFireVoodoo(int nTrigger, PLAYER *pPlayer)
{
    if (nTrigger != 2)
        return;
    //int nAmmo = pPlayer->at181[9];
    int nCount = ClipHigh(pPlayer->at181[9], pPlayer->at1da);
    if (nCount > 0)
    {
        int v4 = pPlayer->at181[9] - (pPlayer->at181[9] / nCount)*nCount;
        for (int i = 0; i < pPlayer->at1da; i++)
        {
            int nTarget = pPlayer->at1de[i];
            spritetype *pTarget = &sprite[nTarget];
            if (v4 > 0)
                v4--;
            int nDist = approxDist(pTarget->x-pPlayer->pSprite->x, pTarget->y-pPlayer->pSprite->y);
            if (nDist > 0 && nDist < 51200)
            {
                int vc = pPlayer->at181[9]>>3;
                int v8 = pPlayer->at181[9]<<1;
                int nDamage = (v8+Random2(vc))<<4;
                nDamage = (nDamage*((51200-nDist)+1))/51200;
                nDamage = actDamageSprite(pPlayer->at5b, pTarget, DAMAGE_TYPE_5, nDamage);
                UseAmmo(pPlayer, 9, nDamage);
                if (pTarget->type >= kDudePlayer1 && pTarget->type <= kDudePlayer8)
                {
                    PLAYER *pOtherPlayer = &gPlayer[pTarget->type-kDudePlayer1];
                    if (!pOtherPlayer->at31a || !powerupCheck(pOtherPlayer,14))
                        powerupActivate(pOtherPlayer, 28);
                }
                fxSpawnBlood(pTarget, 0);
            }
        }
    }
    UseAmmo(pPlayer, 9, pPlayer->at181[9]);
    pPlayer->atcb[10] = 0;
    pPlayer->atc3 = -1;
}

void DropVoodoo(int nTrigger, PLAYER *pPlayer)
{
    UNREFERENCED_PARAMETER(nTrigger);
    sfxPlay3DSound(pPlayer->pSprite, 455, 2, 0);
    spritetype *pSprite = playerFireThing(pPlayer, 0, -4730, 432, 0xccccc);
    if (pSprite)
    {
        int nXSprite = pSprite->extra;
        XSPRITE *pXSprite = &xsprite[nXSprite];
        pXSprite->data1 = pPlayer->at181[9];
        evPost(pSprite->index, 3, 90, CALLBACK_ID_21);
        UseAmmo(pPlayer, 6, gAmmoItemData[0].at8);
        UseAmmo(pPlayer, 9, pPlayer->at181[9]);
        pPlayer->atcb[10] = 0;
    }
}

struct TeslaMissile
{
    int at0; // offset
    int at4; // id
    int at8; // ammo use
    int atc; // sound
    int at10; // light
    int at14; // weapon flash
};

void FireTesla(int nTrigger, PLAYER *pPlayer)
{
    TeslaMissile teslaMissile[6] = 
    {
        { 0, 306, 1, 470, 20, 1 },
        { -140, 306, 1, 470, 30, 1 },
        { 140, 306, 1, 470, 30, 1 },
        { 0, 302, 35, 471, 40, 1 },
        { -140, 302, 35, 471, 50, 1 },
        { 140, 302, 35, 471, 50, 1 },
    };
    if (nTrigger > 0 && nTrigger <= 6)
    {
        nTrigger--;
        spritetype *pSprite = pPlayer->pSprite;
        TeslaMissile *pMissile = &teslaMissile[nTrigger];
        if (!sub_4B2C8(pPlayer, 7, pMissile->at8))
        {
            pMissile = &teslaMissile[0];
            if (!sub_4B2C8(pPlayer, 7, pMissile->at8))
            {
                pPlayer->atc3 = -1;
                pPlayer->at26 = 76;
                pPlayer->at37b = 0;
                return;
            }
        }
        playerFireMissile(pPlayer, pMissile->at0, pPlayer->at1be.dx, pPlayer->at1be.dy, pPlayer->at1be.dz, pMissile->at4);
        UseAmmo(pPlayer, 7, pMissile->at8);
        sfxPlay3DSound(pSprite, pMissile->atc, 1, 0);
        pPlayer->at362 = pMissile->at10;
        pPlayer->at37b = pMissile->at14;
    }
}

void AltFireTesla(int nTrigger, PLAYER *pPlayer)
{
    UNREFERENCED_PARAMETER(nTrigger);
    spritetype *pSprite = pPlayer->pSprite;
    playerFireMissile(pPlayer, 0, pPlayer->at1be.dx, pPlayer->at1be.dy, pPlayer->at1be.dz, 302);
    UseAmmo(pPlayer, pPlayer->atc7, 35);
    sfxPlay3DSound(pSprite, 471, 2, 0);
    pPlayer->at362 = 40;
    pPlayer->at37b = 1;
}

void FireNapalm(int nTrigger, PLAYER *pPlayer)
{
    spritetype *pSprite = pPlayer->pSprite;
    int offset = 0;
    switch (nTrigger)
    {
    case 2:
        offset = -50;
        break;
    case 3:
        offset = 50;
        break;
    }
    playerFireMissile(pPlayer, offset, pPlayer->at1be.dx, pPlayer->at1be.dy, pPlayer->at1be.dz, 312);
    sfxPlay3DSound(pSprite, 480, 2, 0);
    UseAmmo(pPlayer, 4, 1);
    pPlayer->at37b = 1;
}

void FireNapalm2(int nTrigger, PLAYER *pPlayer)
{
    UNREFERENCED_PARAMETER(nTrigger);
    spritetype *pSprite = pPlayer->pSprite;
    playerFireMissile(pPlayer, -120, pPlayer->at1be.dx, pPlayer->at1be.dy, pPlayer->at1be.dz, 312);
    playerFireMissile(pPlayer, 120, pPlayer->at1be.dx, pPlayer->at1be.dy, pPlayer->at1be.dz, 312);
    sfxPlay3DSound(pSprite, 480, 2, 0);
    UseAmmo(pPlayer, 4, 2);
    pPlayer->at37b = 1;
}

void AltFireNapalm(int nTrigger, PLAYER *pPlayer)
{
    UNREFERENCED_PARAMETER(nTrigger);
    char UNUSED(bAkimbo) = powerupCheck(pPlayer, 17);
    int nSpeed = mulscale16(0x8000, 0x177777)+0x66666;
    spritetype *pMissile = playerFireThing(pPlayer, 0, -4730, 428, nSpeed);
    if (pMissile)
    {
        XSPRITE *pXSprite = &xsprite[pMissile->extra];
        pXSprite->data4 = ClipHigh(pPlayer->at181[4], 12);
        UseAmmo(pPlayer, 4, pXSprite->data4);
        seqSpawn(22, 3, pMissile->extra, -1);
        actBurnSprite(actSpriteIdToOwnerId(pPlayer->pSprite->index), pXSprite, 600);
        evPost(pMissile->index, 3, 0, CALLBACK_ID_0);
        sfxPlay3DSound(pMissile, 480, 2, 0);
        pPlayer->at362 = 30;
        pPlayer->at37b = 1;
    }
}

void FireLifeLeech(int nTrigger, PLAYER *pPlayer)
{
    if (!CheckAmmo(pPlayer, 8, 1))
        return;
    int r1 = Random2(2000);
    int r2 = Random2(2000);
    int r3 = Random2(1000);
    spritetype *pMissile = playerFireMissile(pPlayer, 0, pPlayer->at1be.dx+r1, pPlayer->at1be.dy+r2, pPlayer->at1be.dz+r3, 315);
    if (pMissile)
    {
        XSPRITE *pXSprite = &xsprite[pMissile->extra];
        pXSprite->target = pPlayer->at1d6;
        pMissile->ang = (nTrigger==2) ? 1024 : 0;
    }
    if (sub_4B2C8(pPlayer, 8, 1))
        UseAmmo(pPlayer, 8, 1);
    else
        actDamageSprite(pPlayer->at5b, pPlayer->pSprite, DAMAGE_TYPE_5, 16);
    pPlayer->at362 = ClipHigh(pPlayer->at362+5, 50);
}

void AltFireLifeLeech(int nTrigger, PLAYER *pPlayer)
{
    UNREFERENCED_PARAMETER(nTrigger);
    sfxPlay3DSound(pPlayer->pSprite, 455, 2, 0);
    spritetype *pMissile = playerFireThing(pPlayer, 0, -4730, 431, 0x19999);
    if (pMissile)
    {
        pMissile->cstat |= 4096;
        XSPRITE *pXSprite = &xsprite[pMissile->extra];
        pXSprite->Push = 1;
        pXSprite->Proximity = 1;
        pXSprite->DudeLockout = 1;
        pXSprite->data4 = ClipHigh(pPlayer->at181[4], 12);
        pXSprite->stateTimer = 1;
        evPost(pMissile->index, 3, 120, CALLBACK_ID_20);
        if (gGameOptions.nGameType <= 1)
        {
            int nAmmo = pPlayer->at181[8];
            if (nAmmo < 25 && pPlayer->pXSprite->health > ((25-nAmmo)<<4))
            {
                actDamageSprite(pPlayer->at5b, pPlayer->pSprite, DAMAGE_TYPE_5, ((25-nAmmo)<<4));
                nAmmo = 25;
            }
            pXSprite->data3 = nAmmo;
            UseAmmo(pPlayer, 8, nAmmo);
        }
        else
        {
            pXSprite->data3 = pPlayer->at181[8];
            pPlayer->at181[8] = 0;
        }
        pPlayer->atcb[9] = 0;
    }
}

void FireBeast(int nTrigger, PLAYER * pPlayer)
{
    UNREFERENCED_PARAMETER(nTrigger);
    int r1 = Random2(2000);
    int r2 = Random2(2000);
    int r3 = Random2(2000);
    actFireVector(pPlayer->pSprite, 0, pPlayer->at6f-pPlayer->pSprite->z, pPlayer->at1be.dx+r1, pPlayer->at1be.dy+r2, pPlayer->at1be.dz+r3, VECTOR_TYPE_9);
}

char gWeaponUpgrade[][13] = {
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    { 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
};

char WeaponUpgrade(PLAYER *pPlayer, char newWeapon)
{
    char weapon = pPlayer->atbd;
    if (!sub_4B1A4(pPlayer) && (gProfile[pPlayer->at57].nWeaponSwitch&1) && (gWeaponUpgrade[pPlayer->atbd][newWeapon] || (gProfile[pPlayer->at57].nWeaponSwitch&2)))
        weapon = newWeapon;
    return weapon;
}

int OrderNext[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 1, 1 };
int OrderPrev[] = { 12, 12, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 1 };

char WeaponFindNext(PLAYER *pPlayer, int *a2, char bDir)
{
    int weapon = pPlayer->atbd;
    do
    {
        if (bDir)
            weapon = OrderNext[weapon];
        else
            weapon = OrderPrev[weapon];
        if (weaponModes[weapon].at0 && pPlayer->atcb[weapon])
        {
            if (weapon == 9)
            {
                if (CheckAmmo(pPlayer, weaponModes[weapon].at4, 1))
                    break;
            }
            else
            {
                if (sub_4B2C8(pPlayer, weaponModes[weapon].at4, 1))
                    break;
            }
        }
    } while (weapon != pPlayer->atbd);
    if (weapon == pPlayer->atbd)
    {
        if (!weaponModes[weapon].at0 || !CheckAmmo(pPlayer, weaponModes[weapon].at4, 1))
            weapon = 1;
    }
    if (a2)
        *a2 = 0;
    return weapon;
}

char WeaponFindLoaded(PLAYER *pPlayer, int *a2)
{
    char v4 = 1;
    int v14 = 0;
    if (weaponModes[pPlayer->atbd].at0 > 1)
    {
        for (int i = 0; i < weaponModes[pPlayer->atbd].at0; i++)
        {
            if (CheckAmmo(pPlayer, weaponModes[pPlayer->atbd].at4, 1))
            {
                v14 = i;
                v4 = pPlayer->atbd;
                break;
            }
        }
    }
    if (v4 == 1)
    {
        int vc = 0;
        for (int i = 0; i < 14; i++)
        {
            int weapon = pPlayer->at111[vc][i];
            if (pPlayer->atcb[weapon])
            {
                for (int j = 0; j < weaponModes[weapon].at0; j++)
                {
                    if (sub_4B1FC(pPlayer, weapon, weaponModes[weapon].at4, 1))
                    {
                        if (a2)
                            *a2 = j;
                        return weapon;
                    }
                }
            }
        }
    }
    else if (a2)
        *a2 = v14;
    return v4;
}

char sub_4F0E0(PLAYER *pPlayer)
{
    switch (pPlayer->atc3)
    {
    case 5:
        if (!pPlayer->atc.buttonFlags.shoot2)
            pPlayer->atc3 = 6;
        return 1;
    case 6:
        if (pPlayer->atc.buttonFlags.shoot2)
        {
            pPlayer->atc3 = 3;
            pPlayer->at1b2 = pPlayer->atbf;
            StartQAV(pPlayer, 13, nClientDropCan, 0);
        }
        else if (pPlayer->atc.buttonFlags.shoot)
        {
            pPlayer->atc3 = 7;
            pPlayer->at1b2 = 0;
            pPlayer->at1b6 = gFrameClock;
        }
        return 1;
    case 7:
    {
        pPlayer->at1ba = ClipHigh(divscale16(gFrameClock-pPlayer->at1b6,240), 65536);
        if (!pPlayer->atc.buttonFlags.shoot)
        {
            if (!pPlayer->at1b2)
                pPlayer->at1b2 = pPlayer->atbf;
            pPlayer->atc3 = 1;
            StartQAV(pPlayer, 14, nClientThrowCan, 0);
        }
        return 1;
    }
    }
    return 0;
}

char sub_4F200(PLAYER *pPlayer)
{
    switch (pPlayer->atc3)
    {
    case 4:
        if (!pPlayer->atc.buttonFlags.shoot2)
            pPlayer->atc3 = 5;
        return 1;
    case 5:
        if (pPlayer->atc.buttonFlags.shoot2)
        {
            pPlayer->atc3 = 1;
            pPlayer->at1b2 = pPlayer->atbf;
            StartQAV(pPlayer, 22, nClientDropBundle, 0);
        }
        else if (pPlayer->atc.buttonFlags.shoot)
        {
            pPlayer->atc3 = 6;
            pPlayer->at1b2 = 0;
            pPlayer->at1b6 = gFrameClock;
        }
        return 1;
    case 6:
    {
        pPlayer->at1ba = ClipHigh(divscale16(gFrameClock-pPlayer->at1b6,240), 65536);
        if (!pPlayer->atc.buttonFlags.shoot)
        {
            if (!pPlayer->at1b2)
                pPlayer->at1b2 = pPlayer->atbf;
            pPlayer->atc3 = 1;
            StartQAV(pPlayer, 23, nClientThrowBundle, 0);
        }
        return 1;
    }
    }
    return 0;
}

char sub_4F320(PLAYER *pPlayer)
{
    switch (pPlayer->atc3)
    {
    case 9:
        pPlayer->at1ba = ClipHigh(divscale16(gFrameClock-pPlayer->at1b6,240), 65536);
        pPlayer->atbf = 0;
        if (!pPlayer->atc.buttonFlags.shoot)
        {
            pPlayer->atc3 = 8;
            StartQAV(pPlayer, 29, nClientThrowProx, 0);
        }
        break;
    }
    return 0;
}

char sub_4F3A0(PLAYER *pPlayer)
{
    switch (pPlayer->atc3)
    {
    case 13:
        pPlayer->at1ba = ClipHigh(divscale16(gFrameClock-pPlayer->at1b6,240), 65536);
        if (!pPlayer->atc.buttonFlags.shoot)
        {
            pPlayer->atc3 = 11;
            StartQAV(pPlayer, 39, nClientThrowRemote, 0);
        }
        break;
    }
    return 0;
}

char sub_4F414(PLAYER *pPlayer)
{
    switch (pPlayer->atc3)
    {
    case 4:
        pPlayer->atc3 = 6;
        StartQAV(pPlayer, 114, nClientFireLifeLeech, 1);
        return 1;
    case 6:
        if (!pPlayer->atc.buttonFlags.shoot2)
        {
            pPlayer->atc3 = 2;
            StartQAV(pPlayer, 118, -1, 0);
            return 1;
        }
        break;
    case 8:
        pPlayer->atc3 = 2;
        StartQAV(pPlayer, 118, -1, 0);
        return 1;
    }
    return 0;
}

char sub_4F484(PLAYER *pPlayer)
{
    switch (pPlayer->atc3)
    {
    case 4:
        pPlayer->atc3 = 5;
        if (sub_4B2C8(pPlayer, 7, 10) && powerupCheck(pPlayer, 17))
            StartQAV(pPlayer, 84, nClientFireTesla, 1);
        else
            StartQAV(pPlayer, 77, nClientFireTesla, 1);
        return 1;
    case 5:
        if (!pPlayer->atc.buttonFlags.shoot)
        {
            pPlayer->atc3 = 2;
            if (sub_4B2C8(pPlayer, 7, 10) && powerupCheck(pPlayer, 17))
                StartQAV(pPlayer, 87, -1, 0);
            else
                StartQAV(pPlayer, 80, -1, 0);
            return 1;
        }
        break;
    case 7:
        pPlayer->atc3 = 2;
        if (sub_4B2C8(pPlayer, 7, 10) && powerupCheck(pPlayer, 17))
            StartQAV(pPlayer, 87, -1, 0);
        else
            StartQAV(pPlayer, 80, -1, 0);
        return 1;
    }
    return 0;
}

void WeaponProcess(PLAYER *pPlayer)
{
    pPlayer->at37b = ClipLow(pPlayer->at37b-1, 0);
    if (pPlayer->pXSprite->health == 0)
    {
        pPlayer->at1b1 = 0;
        sfxKill3DSound(pPlayer->pSprite, 1, -1);
    }
    if (pPlayer->at87 && BannedUnderwater(pPlayer->atbd))
    {
        if (sub_4B1A4(pPlayer))
        {
            if (pPlayer->atbd == 7)
            {
                pPlayer->at1b2 = pPlayer->atbf;
                DropCan(1, pPlayer);
                pPlayer->atc3 = 3;
            }
            else if (pPlayer->atbd == 6)
            {
                pPlayer->at1b2 = pPlayer->atbf;
                DropBundle(1, pPlayer);
                pPlayer->atc3 = 1;
            }
        }
        WeaponLower(pPlayer);
        pPlayer->at1ba = 0;
    }
    WeaponPlay(pPlayer);
    UpdateAimVector(pPlayer);
    pPlayer->atbf -= 4;
    char bShoot = pPlayer->atc.buttonFlags.shoot;
    char bShoot2 = pPlayer->atc.buttonFlags.shoot2;
    if (pPlayer->at1b1 && pPlayer->pXSprite->health > 0)
    {
        if (bShoot && CheckAmmo(pPlayer, pPlayer->atc7, 1))
        {
            while (pPlayer->atbf <= 0)
                pPlayer->atbf += weaponQAV[pPlayer->at26]->at10;
        }
        else
        {
            pPlayer->atbf = 0;
            pPlayer->at1b1 = 0;
        }
        return;
    }
    pPlayer->atbf = ClipLow(pPlayer->atbf, 0);
    switch (pPlayer->atbd)
    {
    case 7:
        if (sub_4F0E0(pPlayer))
            return;
        break;
    case 6:
        if (sub_4F200(pPlayer))
            return;
        break;
    case 11:
        if (sub_4F320(pPlayer))
            return;
        break;
    case 12:
        if (sub_4F3A0(pPlayer))
            return;
        break;
    }
    if (pPlayer->atbf > 0)
        return;
    if (pPlayer->pXSprite->health == 0 || pPlayer->atbd == 0)
        pPlayer->at26 = -1;
    switch (pPlayer->atbd)
    {
    case 9:
        if (sub_4F414(pPlayer))
            return;
        break;
    case 8:
        if (sub_4F484(pPlayer))
            return;
        break;
    }
    if (pPlayer->atbe)
    {
        sfxKill3DSound(pPlayer->pSprite, -1, 441);
        pPlayer->atc3 = 0;
        pPlayer->atc.newWeapon = pPlayer->atbe;
        pPlayer->atbe = 0;
    }
    if (pPlayer->atc.keyFlags.nextWeapon)
    {
        pPlayer->atc.keyFlags.nextWeapon = 0;
        pPlayer->atc3 = 0;
        pPlayer->atbe = 0;
        int t;
        char weapon = WeaponFindNext(pPlayer, &t, 1);
        pPlayer->atd9[weapon] = t;
        if (pPlayer->atbd)
        {
            WeaponLower(pPlayer);
            pPlayer->atbe = weapon;
            return;
        }
        pPlayer->atc.newWeapon = weapon;
    }
    if (pPlayer->atc.keyFlags.prevWeapon)
    {
        pPlayer->atc.keyFlags.prevWeapon = 0;
        pPlayer->atc3 = 0;
        pPlayer->atbe = 0;
        int t;
        char weapon = WeaponFindNext(pPlayer, &t, 0);
        pPlayer->atd9[weapon] = t;
        if (pPlayer->atbd)
        {
            WeaponLower(pPlayer);
            pPlayer->atbe = weapon;
            return;
        }
        pPlayer->atc.newWeapon = weapon;
    }
    if (pPlayer->atc3 == -1)
    {
        pPlayer->atc3 = 0;
        int t;
        char weapon = WeaponFindLoaded(pPlayer, &t);
        pPlayer->atd9[weapon] = t;
        if (pPlayer->atbd)
        {
            WeaponLower(pPlayer);
            pPlayer->atbe = weapon;
            return;
        }
        pPlayer->atc.newWeapon = weapon;
    }
    if (pPlayer->atc.newWeapon)
    {
        if (pPlayer->atc.newWeapon == 6)
        {
            if (pPlayer->atbd == 6)
            {
                if (sub_4B2C8(pPlayer, 10, 1))
                    pPlayer->atc.newWeapon = 11;
                else if (sub_4B2C8(pPlayer, 11, 1))
                    pPlayer->atc.newWeapon = 12;
            }
            else if (pPlayer->atbd == 11)
            {
                if (sub_4B2C8(pPlayer, 11, 1))
                    pPlayer->atc.newWeapon = 12;
                else if (sub_4B2C8(pPlayer, 5, 1) && pPlayer->at87 == 0)
                    pPlayer->atc.newWeapon = 6;
            }
            else if (pPlayer->atbd == 12)
            {
                if (sub_4B2C8(pPlayer, 5, 1) && pPlayer->at87 == 0)
                    pPlayer->atc.newWeapon = 6;
                else if (sub_4B2C8(pPlayer, 10, 1))
                    pPlayer->atc.newWeapon = 11;
            }
            else
            {
                if (sub_4B2C8(pPlayer, 5, 1) && pPlayer->at87 == 0)
                    pPlayer->atc.newWeapon = 6;
                else if (sub_4B2C8(pPlayer, 10, 1))
                    pPlayer->atc.newWeapon = 11;
                else if (sub_4B2C8(pPlayer, 11, 1))
                    pPlayer->atc.newWeapon = 12;
            }
        }
        if (pPlayer->pXSprite->health == 0 || pPlayer->atcb[pPlayer->atc.newWeapon] == 0)
        {
            pPlayer->atc.newWeapon = 0;
            return;
        }
        if (pPlayer->at87 && BannedUnderwater(pPlayer->atc.newWeapon) && !sub_4B1A4(pPlayer))
        {
            pPlayer->atc.newWeapon = 0;
            return;
        }
        int nWeapon = pPlayer->atc.newWeapon;
        int v4c = weaponModes[nWeapon].at0;
        if (!pPlayer->atbd)
        {
            int nAmmoType = weaponModes[nWeapon].at4;
            if (v4c > 1)
            {
                if (CheckAmmo(pPlayer, nAmmoType, 1) || nAmmoType == 11)
                    WeaponRaise(pPlayer);
                pPlayer->atc.newWeapon = 0;
            }
            else
            {
                if (sub_4B1FC(pPlayer, nWeapon, nAmmoType, 1))
                    WeaponRaise(pPlayer);
                else
                {
                    pPlayer->atc3 = 0;
                    int t;
                    char weapon = WeaponFindLoaded(pPlayer, &t);
                    pPlayer->atd9[weapon] = t;
                    if (pPlayer->atbd)
                    {
                        WeaponLower(pPlayer);
                        pPlayer->atbe = weapon;
                        return;
                    }
                    pPlayer->atc.newWeapon = weapon;
                }
            }
            return;
        }
        if (nWeapon == pPlayer->atbd && v4c <= 1)
        {
            pPlayer->atc.newWeapon = 0;
            return;
        }
        int i = 0;
        if (nWeapon == pPlayer->atbd)
            i = 1;
        for (; i <= v4c; i++)
        {
            int v6c = (pPlayer->atd9[nWeapon]+i)%v4c;
            if (sub_4B1FC(pPlayer, nWeapon, weaponModes[nWeapon].at4, 1))
            {
                WeaponLower(pPlayer);
                pPlayer->atd9[nWeapon] = v6c;
                return;
            }
        }
        pPlayer->atc.newWeapon = 0;
        return;
    }
    if (pPlayer->atbd && !CheckAmmo(pPlayer, pPlayer->atc7, 1) && pPlayer->atc7 != 11)
    {
        pPlayer->atc3 = -1;
        return;
    }
    if (bShoot)
    {
        switch (pPlayer->atbd)
        {
        case 1:
            StartQAV(pPlayer, 2, nClientFirePitchfork, 0);
            return;
        case 7:
            switch (pPlayer->atc3)
            {
            case 3:
                pPlayer->atc3 = 4;
                StartQAV(pPlayer, 10, nClientFireSpray, 1);
                return;
            }
            break;
        case 6:
            switch (pPlayer->atc3)
            {
            case 3:
                pPlayer->atc3 = 6;
                pPlayer->at1b2 = -1;
                pPlayer->at1b6 = gFrameClock;
                StartQAV(pPlayer, 21, nClientExplodeBundle, 0);
                return;
            }
            break;
        case 11:
            switch (pPlayer->atc3)
            {
            case 7:
                pPlayer->at26 = 27;
                pPlayer->atc3 = 9;
                pPlayer->at1b6 = gFrameClock;
                return;
            }
            break;
        case 12:
            switch (pPlayer->atc3)
            {
            case 10:
                pPlayer->at26 = 36;
                pPlayer->atc3 = 13;
                pPlayer->at1b6 = gFrameClock;
                return;
            case 11:
                pPlayer->atc3 = 12;
                StartQAV(pPlayer, 40, nClientFireRemote, 0);
                return;
            }
            break;
        case 3:
            switch (pPlayer->atc3)
            {
            case 7:
                pPlayer->atc3 = 6;
                StartQAV(pPlayer, 61, nClientFireShotgun, 0);
                return;
            case 3:
                pPlayer->atc3 = 2;
                StartQAV(pPlayer, 54, nClientFireShotgun, 0);
                return;
            case 2:
                pPlayer->atc3 = 1;
                StartQAV(pPlayer, 55, nClientFireShotgun, 0);
                return;
            }
            break;
        case 4:
            if (powerupCheck(pPlayer, 17) && sub_4B2C8(pPlayer, 3, 2))
                StartQAV(pPlayer, 71, nClientFireTommy, 1);
            else
                StartQAV(pPlayer, 66, nClientFireTommy, 1);
            return;
        case 2:
            if (powerupCheck(pPlayer, 17) && sub_4B2C8(pPlayer, 1, 2))
                StartQAV(pPlayer, 48, nClientFireFlare, 0);
            else
                StartQAV(pPlayer, 43, nClientFireFlare, 0);
            return;
        case 10:
        {
            static int nChance[] = { 0xa000, 0xc000, 0xe000, 0x10000 };
            int nRand = wrand()*2;
            int i;
            for (i = 0; nChance[i] < nRand; i++)
            {
            }
            pPlayer->voodooTarget = pPlayer->at1d6;
            if (pPlayer->voodooTarget == -1 || sprite[pPlayer->voodooTarget].statnum != 6)
                i = 4;
            StartQAV(pPlayer,103+i, nClientFireVoodoo, 0);
            return;
        }
        case 8:
            switch (pPlayer->atc3)
            {
            case 2:
                pPlayer->atc3 = 4;
                if (sub_4B2C8(pPlayer, 7, 10) && powerupCheck(pPlayer, 17))
                    StartQAV(pPlayer, 84, nClientFireTesla, 0);
                else
                    StartQAV(pPlayer, 77, nClientFireTesla, 0);
                return;
            case 5:
                if (sub_4B2C8(pPlayer, 7, 10) && powerupCheck(pPlayer, 17))
                    StartQAV(pPlayer, 84, nClientFireTesla, 0);
                else
                    StartQAV(pPlayer, 77, nClientFireTesla, 0);
                return;
            }
            break;
        case 5:
            if (powerupCheck(pPlayer, 17))
                StartQAV(pPlayer, 122, nClientFireNapalm, 0);
            else
                StartQAV(pPlayer, 91, nClientFireNapalm, 0);
            return;
        case 9:
            sfxPlay3DSound(pPlayer->pSprite, 494, 2, 0);
            StartQAV(pPlayer, 116, nClientFireLifeLeech, 0);
            return;
        case 13:
            StartQAV(pPlayer, 95+Random(4), nClientFireBeast, 0);
            return;
        }
    }
    if (bShoot2)
    {
        switch (pPlayer->atbd)
        {
        case 1:
            StartQAV(pPlayer, 2, nClientFirePitchfork, 0);
            return;
        case 7:
            switch (pPlayer->atc3)
            {
            case 3:
                pPlayer->atc3 = 5;
                StartQAV(pPlayer, 12, nClientExplodeCan, 0);
                return;
            }
            break;
        case 6:
            switch (pPlayer->atc3)
            {
            case 3:
                pPlayer->atc3 = 4;
                StartQAV(pPlayer, 21, nClientExplodeBundle, 0);
                return;
            case 7:
                pPlayer->atc3 = 8;
                StartQAV(pPlayer, 28, nClientDropProx, 0);
                return;
            case 10:
                pPlayer->atc3 = 11;
                StartQAV(pPlayer, 38, nClientDropRemote, 0);
                return;
            case 11:
                if (pPlayer->at181[11] > 0)
                {
                    pPlayer->atc3 = 10;
                    StartQAV(pPlayer, 30, -1, 0);
                }
                return;
            }
            break;
        case 11:
            switch (pPlayer->atc3)
            {
            case 7:
                pPlayer->atc3 = 8;
                StartQAV(pPlayer, 28, nClientDropProx, 0);
                return;
            }
            break;
        case 12:
            switch (pPlayer->atc3)
            {
            case 10:
                pPlayer->atc3 = 11;
                StartQAV(pPlayer, 38, nClientDropRemote, 0);
                return;
            case 11:
                if (pPlayer->at181[11] > 0)
                {
                    pPlayer->atc3 = 10;
                    StartQAV(pPlayer, 30, -1, 0);
                }
                return;
            }
            break;
        case 3:
            switch (pPlayer->atc3)
            {
            case 7:
                pPlayer->atc3 = 6;
                StartQAV(pPlayer, 62, nClientFireShotgun, 0);
                return;
            case 3:
                pPlayer->atc3 = 1;
                StartQAV(pPlayer, 56, nClientFireShotgun, 0);
                return;
            case 2:
                pPlayer->atc3 = 1;
                StartQAV(pPlayer, 55, nClientFireShotgun, 0);
                return;
            }
            break;
        case 4:
            if (powerupCheck(pPlayer, 17) && sub_4B2C8(pPlayer, 3, 2))
                StartQAV(pPlayer, 73, nClientAltFireSpread2, 0);
            else
                StartQAV(pPlayer, 67, nClientAltFireSpread2, 0);
            return;
        case 10:
            sfxPlay3DSound(pPlayer->pSprite, 461, 2, 0);
            StartQAV(pPlayer, 110, nClientAltFireVoodoo, 0);
            return;
#if 0
        case 2:
            if (powerupCheck(pPlayer, 17) && sub_4B2C8(pPlayer, 1, 2))
                StartQAV(pPlayer, 48, nClientFireFlare, 0);
            else
                StartQAV(pPlayer, 43, nClientFireFlare, 0);
            return;
#endif
        case 8:
            if (sub_4B2C8(pPlayer, 7, 35))
            {
                if (sub_4B2C8(pPlayer, 7, 70) && powerupCheck(pPlayer, 17))
                    StartQAV(pPlayer, 85, nClientFireTesla, 0);
                else
                    StartQAV(pPlayer, 78, nClientFireTesla, 0);
            }
            else
            {
                if (sub_4B2C8(pPlayer, 7, 10) && powerupCheck(pPlayer, 17))
                    StartQAV(pPlayer, 84, nClientFireTesla, 0);
                else
                    StartQAV(pPlayer, 77, nClientFireTesla, 0);
            }
            return;
        case 5:
            if (powerupCheck(pPlayer, 17))
                StartQAV(pPlayer, 122, nClientAltFireNapalm, 0);
            else
                StartQAV(pPlayer, 91, nClientAltFireNapalm, 0);
            return;
        case 2:
            if (CheckAmmo(pPlayer, 1, 8))
            {
                if (powerupCheck(pPlayer, 17) && sub_4B2C8(pPlayer, 1, 16))
                    StartQAV(pPlayer, 48, nClientAltFireFlare, 0);
                else
                    StartQAV(pPlayer, 43, nClientAltFireFlare, 0);
            }
            else
            {
                if (powerupCheck(pPlayer, 17) && sub_4B2C8(pPlayer, 1, 2))
                    StartQAV(pPlayer, 48, nClientFireFlare, 0);
                else
                    StartQAV(pPlayer, 43, nClientFireFlare, 0);
            }
            return;
        case 9:
            if (gGameOptions.nGameType <= 1 && !sub_4B2C8(pPlayer, 8, 1) && pPlayer->pXSprite->health < (25 << 4))
            {
                sfxPlay3DSound(pPlayer->pSprite, 494, 2, 0);
                StartQAV(pPlayer, 116, nClientFireLifeLeech, 0);
            }
            else
            {
                StartQAV(pPlayer, 119, -1, 0);
                AltFireLifeLeech(1, pPlayer);
                pPlayer->atc3 = -1;
            }
            return;
        }
    }
    WeaponUpdateState(pPlayer);
}

void sub_51340(spritetype *pMissile, int a2)
{
    char va4[(kMaxSectors+7)>>3];
    int x = pMissile->x;
    int y = pMissile->y;
    int z = pMissile->z;
    int nDist = 300;
    int nSector = pMissile->sectnum;
    int nOwner = actSpriteOwnerToSpriteId(pMissile);
    gAffectedSectors[0] = -1;
    gAffectedXWalls[0] = -1;
    GetClosestSpriteSectors(nSector, x, y, nDist, gAffectedSectors, va4, gAffectedXWalls);
    char v4 = 1;
    int v24 = -1;
    actHitcodeToData(a2, &gHitInfo, &v24, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    if (a2 == 3 && v24 >= 0 && sprite[v24].statnum == 6)
        v4 = 0;
    for (int nSprite = headspritestat[6]; nSprite >= 0; nSprite = nextspritestat[nSprite])
    {
        if (nSprite != nOwner || v4)
        {
            spritetype *pSprite = &sprite[nSprite];
            if (pSprite->hitag&32)
                continue;
            if (TestBitString(va4, pSprite->sectnum) && CheckProximity(pSprite, x, y, z, nSector, nDist))
            {
                int dx = pMissile->x-pSprite->x;
                int dy = pMissile->y-pSprite->y;
                int nDamage = ClipLow((nDist-(ksqrt(dx*dx+dy*dy)>>4)+20)>>1, 10);
                if (nSprite == nOwner)
                    nDamage /= 2;
                actDamageSprite(nOwner, pSprite, DAMAGE_TYPE_6, nDamage<<4);
            }
        }
    }
    for (int nSprite = headspritestat[4]; nSprite >= 0; nSprite = nextspritestat[nSprite])
    {
        spritetype *pSprite = &sprite[nSprite];
        if (pSprite->hitag&32)
            continue;
        if (TestBitString(va4, pSprite->sectnum) && CheckProximity(pSprite, x, y, z, nSector, nDist))
        {
            XSPRITE *pXSprite = &xsprite[pSprite->extra];
            if (!pXSprite->locked)
            {
                int dx = pMissile->x-pSprite->x;
                int dy = pMissile->y-pSprite->y;
                int nDamage = ClipLow(nDist-(ksqrt(dx*dx+dy*dy)>>4)+20, 20);
                actDamageSprite(nOwner, pSprite, DAMAGE_TYPE_6, nDamage<<4);
            }
        }
    }
}

class WeaponLoadSave : public LoadSave
{
public:
    virtual void Load();
    virtual void Save();
};

void WeaponLoadSave::Load()
{
}

void WeaponLoadSave::Save()
{
}

static WeaponLoadSave *myLoadSave;

void WeaponLoadSaveConstruct(void)
{
    myLoadSave = new WeaponLoadSave();
}

