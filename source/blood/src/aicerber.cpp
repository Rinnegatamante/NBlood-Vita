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
#include "compat.h"
#include "build.h"
#include "pragmas.h"
#include "mmulti.h"
#include "common_game.h"

#include "actor.h"
#include "ai.h"
#include "aicerber.h"
#include "blood.h"
#include "db.h"
#include "dude.h"
#include "levels.h"
#include "player.h"
#include "seq.h"
#include "sfx.h"
#include "trig.h"

static void BiteSeqCallback(int, int);
static void BurnSeqCallback(int, int);
static void BurnSeqCallback2(int, int);
static void thinkSearch(spritetype *pSprite, XSPRITE *pXSprite);
static void thinkTarget(spritetype *pSprite, XSPRITE *pXSprite);
static void thinkGoto(spritetype *pSprite, XSPRITE *pXSprite);
static void thinkChase(spritetype *pSprite, XSPRITE *pXSprite);

static int nBiteClient = seqRegisterClient(BiteSeqCallback);
static int nBurnClient = seqRegisterClient(BurnSeqCallback);
static int nBurnClient2 = seqRegisterClient(BurnSeqCallback2);

AISTATE cerberusIdle = { kAiStateIdle, 0, -1, 0, NULL, NULL, thinkTarget, NULL };
AISTATE cerberusSearch = { kAiStateSearch, 7, -1, 1800, NULL, aiMoveForward, thinkSearch, &cerberusIdle };
AISTATE cerberusChase = { kAiStateChase, 7, -1, 0, NULL, aiMoveForward, thinkChase, NULL };
AISTATE cerberusRecoil = { kAiStateRecoil, 5, -1, 0, NULL, NULL, NULL, &cerberusSearch };
AISTATE cerberusTeslaRecoil = { kAiStateRecoil, 4, -1, 0, NULL, NULL, NULL, &cerberusSearch };
AISTATE cerberusGoto = { kAiStateMove, 7, -1, 600, NULL, aiMoveForward, thinkGoto, &cerberusIdle };
AISTATE cerberusBite = { kAiStateChase, 6, nBiteClient, 60, NULL, NULL, NULL, &cerberusChase };
AISTATE cerberusBurn = { kAiStateChase, 6, nBurnClient, 60, NULL, NULL, NULL, &cerberusChase };
AISTATE cerberus3Burn = { kAiStateChase, 6, nBurnClient2, 60, NULL, NULL, NULL, &cerberusChase };
AISTATE cerberus2Idle = { kAiStateIdle, 0, -1, 0, NULL, NULL, thinkTarget, NULL };
AISTATE cerberus2Search = { kAiStateSearch, 7, -1, 1800, NULL, aiMoveForward, thinkSearch, &cerberus2Idle };
AISTATE cerberus2Chase = { kAiStateChase, 7, -1, 0, NULL, aiMoveForward, thinkChase, NULL };
AISTATE cerberus2Recoil = { kAiStateRecoil, 5, -1, 0, NULL, NULL, NULL, &cerberus2Search };
AISTATE cerberus2Goto = { kAiStateMove, 7, -1, 600, NULL, aiMoveForward, thinkGoto, &cerberus2Idle };
AISTATE cerberus2Bite = { kAiStateChase, 6, nBiteClient, 60, NULL, NULL, NULL, &cerberus2Chase };
AISTATE cerberus2Burn = { kAiStateChase, 6, nBurnClient, 60, NULL, NULL, NULL, &cerberus2Chase };
AISTATE cerberus4Burn = { kAiStateChase, 6, nBurnClient2, 60, NULL, NULL, NULL, &cerberus2Chase };
AISTATE cerberus139890 = { kAiStateOther, 7, -1, 120, NULL, aiMoveTurn, NULL, &cerberusChase };
AISTATE cerberus1398AC = { kAiStateOther, 7, -1, 120, NULL, aiMoveTurn, NULL, &cerberusChase };

static void BiteSeqCallback(int, int nXSprite)
{
    XSPRITE *pXSprite = &xsprite[nXSprite];
    int nSprite = pXSprite->reference;
    spritetype *pSprite = &sprite[nSprite];
    int dx = Cos(pSprite->ang)>>16;
    int dy = Sin(pSprite->ang)>>16;
    if (!(pSprite->type >= kDudeBase && pSprite->type < kDudeMax))
        return;
    //dassert(pSprite->type >= kDudeBase && pSprite->type < kDudeMax);
    if (!(pXSprite->target >= 0 && pXSprite->target < kMaxSprites))
        return;
    //dassert(pXSprite->target >= 0 && pXSprite->target < kMaxSprites);
    spritetype *pTarget = &sprite[pXSprite->target];
    int dz = pTarget->z-pSprite->z;
    actFireVector(pSprite, 350, -100, dx, dy, dz, VECTOR_TYPE_14);
    actFireVector(pSprite, -350, 0, dx, dy, dz, VECTOR_TYPE_14);
    actFireVector(pSprite, 0, 0, dx, dy, dz, VECTOR_TYPE_14);
}

static void BurnSeqCallback(int, int nXSprite)
{
    XSPRITE *pXSprite = &xsprite[nXSprite];
    int nSprite = pXSprite->reference;
    spritetype *pSprite = &sprite[nSprite];
    DUDEINFO *pDudeInfo = &dudeInfo[pSprite->type - kDudeBase];
    int height = pDudeInfo->eyeHeight*pSprite->yrepeat;
    if (!(pXSprite->target >= 0 && pXSprite->target < kMaxSprites))
        return;
    //dassert(pXSprite->target >= 0 && pXSprite->target < kMaxSprites);
    int x = pSprite->x;
    int y = pSprite->y;
    int z = height; // ???
    TARGETTRACK tt1 = { 0x10000, 0x10000, 0x100, 0x55, 0x1aaaaa };
    Aim aim;
    aim.dx = Cos(pSprite->ang)>>16;
    aim.dy = Sin(pSprite->ang)>>16;
    aim.dz = gDudeSlope[nXSprite];
    int nClosest = 0x7fffffff;
    for (short nSprite2 = headspritestat[6]; nSprite2 >= 0; nSprite2 = nextspritestat[nSprite2])
    {
        spritetype *pSprite2 = &sprite[nSprite2];
        if (pSprite == pSprite2 || !(pSprite2->hitag&8))
            continue;
        int x2 = pSprite2->x;
        int y2 = pSprite2->y;
        int z2 = pSprite2->z;
        int nDist = approxDist(x2-x, y2-y);
        if (nDist == 0 || nDist > 0x2800)
            continue;
        if (tt1.at10)
        {
            int t = divscale(nDist, tt1.at10, 12);
            x2 += (xvel[nSprite2]*t)>>12;
            y2 += (yvel[nSprite2]*t)>>12;
            z2 += (zvel[nSprite2]*t)>>8;
        }
        int tx = x+mulscale30(Cos(pSprite->ang), nDist);
        int ty = y+mulscale30(Sin(pSprite->ang), nDist);
        int tz = z+mulscale(gDudeSlope[nXSprite], nDist, 10);
        int tsr = mulscale(9460, nDist, 10);
        int top, bottom;
        GetSpriteExtents(pSprite2, &top, &bottom);
        if (tz-tsr > bottom || tz+tsr < top)
            continue;
        int dx = (tx-x2)>>4;
        int dy = (ty-y2)>>4;
        int dz = (tz-z2)>>8;
        int nDist2 = ksqrt(dx*dx+dy*dy+dz*dz);
        if (nDist2 < nClosest)
        {
            int nAngle = getangle(x2-x, y2-y);
            int nDeltaAngle = ((nAngle-pSprite->ang+1024)&2047)-1024;
            if (klabs(nDeltaAngle) <= tt1.at8)
            {
                int tz = pSprite2->z-pSprite->z;
                if (cansee(x, y, z, pSprite->sectnum, x2, y2, z2, pSprite2->sectnum))
                {
                    nClosest = nDist2;
                    aim.dx = Cos(nAngle)>>16;
                    aim.dy = Sin(nAngle)>>16;
                    aim.dz = divscale(tz, nDist, 10);
                }
                else
                    aim.dz = tz;
            }
        }
    }
    switch (pSprite->type)
    {
    case 227:
        actFireMissile(pSprite, -350, 0, aim.dx, aim.dy, aim.dz, 313);
        actFireMissile(pSprite, 350, -100, aim.dx, aim.dy, aim.dz, 313);
        break;
    case 228:
        actFireMissile(pSprite, 350, -100, aim.dx, aim.dy, aim.dz, 313);
        break;
    }
}

static void BurnSeqCallback2(int, int nXSprite)
{
    XSPRITE *pXSprite = &xsprite[nXSprite];
    int nSprite = pXSprite->reference;
    spritetype *pSprite = &sprite[nSprite];
    if (!(pXSprite->target >= 0 && pXSprite->target < kMaxSprites))
        return;
    //dassert(pXSprite->target >= 0 && pXSprite->target < kMaxSprites);
    DUDEINFO *pDudeInfo = &dudeInfo[pSprite->type - kDudeBase];
    int height = pDudeInfo->eyeHeight*pSprite->yrepeat;
    //dassert(pXSprite->target >= 0 && pXSprite->target < kMaxSprites);
    int x = pSprite->x;
    int y = pSprite->y;
    int z = height; // ???
    TARGETTRACK tt1 = { 0x10000, 0x10000, 0x100, 0x55, 0x1aaaaa };
    Aim aim;
    int ax, ay, az;
    aim.dx = ax = Cos(pSprite->ang)>>16;
    aim.dy = ay = Sin(pSprite->ang)>>16;
    aim.dz = gDudeSlope[nXSprite];
    az = 0;
    int nClosest = 0x7fffffff;
    for (short nSprite2 = headspritestat[6]; nSprite2 >= 0; nSprite2 = nextspritestat[nSprite2])
    {
        spritetype *pSprite2 = &sprite[nSprite2];
        if (pSprite == pSprite2 || !(pSprite2->hitag&8))
            continue;
        int x2 = pSprite2->x;
        int y2 = pSprite2->y;
        int z2 = pSprite2->z;
        int nDist = approxDist(x2-x, y2-y);
        if (nDist == 0 || nDist > 0x2800)
            continue;
        if (tt1.at10)
        {
            int t = divscale(nDist, tt1.at10, 12);
            x2 += (xvel[nSprite2]*t)>>12;
            y2 += (yvel[nSprite2]*t)>>12;
            z2 += (zvel[nSprite2]*t)>>8;
        }
        int tx = x+mulscale30(Cos(pSprite->ang), nDist);
        int ty = y+mulscale30(Sin(pSprite->ang), nDist);
        int tz = z+mulscale(gDudeSlope[nXSprite], nDist, 10);
        int tsr = mulscale(9460, nDist, 10);
        int top, bottom;
        GetSpriteExtents(pSprite2, &top, &bottom);
        if (tz-tsr > bottom || tz+tsr < top)
            continue;
        int dx = (tx-x2)>>4;
        int dy = (ty-y2)>>4;
        int dz = (tz-z2)>>8;
        int nDist2 = ksqrt(dx*dx+dy*dy+dz*dz);
        if (nDist2 < nClosest)
        {
            int nAngle = getangle(x2-x, y2-y);
            int nDeltaAngle = ((nAngle-pSprite->ang+1024)&2047)-1024;
            if (klabs(nDeltaAngle) <= tt1.at8)
            {
                DUDEINFO *pDudeInfo2 = &dudeInfo[pSprite2->type - kDudeBase];
                int height = (pDudeInfo2->aimHeight*pSprite2->yrepeat)<<2;
                int tz = (z2-height)-z;
                if (cansee(x, y, z, pSprite->sectnum, x2, y2, z2, pSprite2->sectnum))
                {
                    nClosest = nDist2;
                    aim.dx = Cos(nAngle)>>16;
                    aim.dy = Sin(nAngle)>>16;
                    aim.dz = divscale(tz, nDist, 10);
                }
                else
                    aim.dz = tz;
            }
        }
    }
    switch (pSprite->type)
    {
    case 227:
        actFireMissile(pSprite, 350, -100, aim.dx, aim.dy, -aim.dz, 308);
        actFireMissile(pSprite, -350, 0, ax, ay, az, 308);
        break;
    case 228:
        actFireMissile(pSprite, 350, -100, aim.dx, aim.dy, -aim.dz, 308);
        break;
    }
}

static void thinkSearch(spritetype *pSprite, XSPRITE *pXSprite)
{
    aiChooseDirection(pSprite, pXSprite, pXSprite->goalAng);
    aiThinkTarget(pSprite, pXSprite);
}

static void thinkTarget(spritetype *pSprite, XSPRITE *pXSprite)
{
    dassert(pSprite->type >= kDudeBase && pSprite->type < kDudeMax);
    DUDEINFO *pDudeInfo = &dudeInfo[pSprite->type-kDudeBase];
    DUDEEXTRA_at6_u1 *pDudeExtraE = &gDudeExtra[pSprite->extra].at6.u1;
    if (pDudeExtraE->at8 && pDudeExtraE->at4 < 10)
        pDudeExtraE->at4++;
    else if (pDudeExtraE->at4 >= 10 && pDudeExtraE->at8)
    {
        pXSprite->goalAng += 256;
        POINT3D *pTarget = &baseSprite[pSprite->index];
        aiSetTarget(pXSprite, pTarget->x, pTarget->y, pTarget->z);
        if (pSprite->type == 227)
            aiNewState(pSprite, pXSprite, &cerberus139890);
        else
            aiNewState(pSprite, pXSprite, &cerberus1398AC);
        return;
    }
    if (Chance(pDudeInfo->alertChance))
    {
        for (int p = connecthead; p >= 0; p = connectpoint2[p])
        {
            PLAYER *pPlayer = &gPlayer[p];
            if (pPlayer->pXSprite->health == 0 || powerupCheck(pPlayer, 13) > 0)
                continue;
            int x = pPlayer->pSprite->x;
            int y = pPlayer->pSprite->y;
            int z = pPlayer->pSprite->z;
            int nSector = pPlayer->pSprite->sectnum;
            int dx = x-pSprite->x;
            int dy = y-pSprite->y;
            int nDist = approxDist(dx, dy);
            if (nDist > pDudeInfo->seeDist && nDist > pDudeInfo->hearDist)
                continue;
            if (!cansee(x, y, z, nSector, pSprite->x, pSprite->y, pSprite->z-((pDudeInfo->eyeHeight*pSprite->yrepeat)<<2), pSprite->sectnum))
                continue;
            int nDeltaAngle = ((getangle(dx,dy)+1024-pSprite->ang)&2047)-1024;
            if (nDist < pDudeInfo->seeDist && klabs(nDeltaAngle) <= pDudeInfo->periphery)
            {
                pDudeExtraE->at0 = 0;
                aiSetTarget(pXSprite, pPlayer->at5b);
                aiActivateDude(pSprite, pXSprite);
            }
            else if (nDist < pDudeInfo->hearDist)
            {
                pDudeExtraE->at0 = 0;
                aiSetTarget(pXSprite, x, y, z);
                aiActivateDude(pSprite, pXSprite);
            }
            else
                continue;
            break;
        }
    }
}

static void thinkGoto(spritetype *pSprite, XSPRITE *pXSprite)
{
    dassert(pSprite->type >= kDudeBase && pSprite->type < kDudeMax);
    DUDEINFO *pDudeInfo = &dudeInfo[pSprite->type - kDudeBase];
    int dx = pXSprite->targetX-pSprite->x;
    int dy = pXSprite->targetY-pSprite->y;
    int nAngle = getangle(dx, dy);
    int nDist = approxDist(dx, dy);
    aiChooseDirection(pSprite, pXSprite, nAngle);
    if (nDist < 512 && klabs(pSprite->ang - nAngle) < pDudeInfo->periphery)
    {
        switch (pSprite->type)
        {
        case 227:
            aiNewState(pSprite, pXSprite, &cerberusSearch);
            break;
        case 228:
            aiNewState(pSprite, pXSprite, &cerberus2Search);
            break;
        }
    }
    aiThinkTarget(pSprite, pXSprite);
}

static void thinkChase(spritetype *pSprite, XSPRITE *pXSprite)
{
    if (pXSprite->target == -1)
    {
        switch (pSprite->type)
        {
        case 227:
            aiNewState(pSprite, pXSprite, &cerberusGoto);
            break;
        case 228:
            aiNewState(pSprite, pXSprite, &cerberus2Goto);
            break;
        }
        return;
    }
    if (!(pSprite->type >= kDudeBase && pSprite->type < kDudeMax))
        return;
    //dassert(pSprite->type >= kDudeBase && pSprite->type < kDudeMax);
    DUDEINFO *pDudeInfo = &dudeInfo[pSprite->type - kDudeBase];
    if (!(pXSprite->target >= 0 && pXSprite->target < kMaxSprites))
        return;
    //dassert(pXSprite->target >= 0 && pXSprite->target < kMaxSprites);
    spritetype *pTarget = &sprite[pXSprite->target];
    XSPRITE *pXTarget = &xsprite[pTarget->extra];
    int dx = pTarget->x-pSprite->x;
    int dy = pTarget->y-pSprite->y;
    aiChooseDirection(pSprite, pXSprite, getangle(dx, dy));
    if (pXTarget->health == 0)
    {
        switch (pSprite->type)
        {
        case 227:
            aiNewState(pSprite, pXSprite, &cerberusSearch);
            break;
        case 228:
            aiNewState(pSprite, pXSprite, &cerberus2Search);
            break;
        }
        return;
    }
    if (IsPlayerSprite(pTarget) && powerupCheck(&gPlayer[pTarget->type-kDudePlayer1], 13) > 0)
    {
        switch (pSprite->type)
        {
        case 227:
            aiNewState(pSprite, pXSprite, &cerberusSearch);
            break;
        case 228:
            aiNewState(pSprite, pXSprite, &cerberus2Search);
            break;
        }
        return;
    }
    int nDist = approxDist(dx, dy);
    if (nDist <= pDudeInfo->seeDist)
    {
        int nDeltaAngle = ((getangle(dx,dy)+1024-pSprite->ang)&2047)-1024;
        int height = (pDudeInfo->eyeHeight*pSprite->yrepeat)<<2;
        if (cansee(pTarget->x, pTarget->y, pTarget->z, pTarget->sectnum, pSprite->x, pSprite->y, pSprite->z - height, pSprite->sectnum))
        {
            if (nDist < pDudeInfo->seeDist && klabs(nDeltaAngle) <= pDudeInfo->periphery)
            {
                aiSetTarget(pXSprite, pXSprite->target);
                if (nDist < 0x1b00 && nDist > 0xd00 && klabs(nDeltaAngle) < 85)
                {
                    switch (pSprite->type)
                    {
                    case 227:
                        aiNewState(pSprite, pXSprite, &cerberusBurn);
                        break;
                    case 228:
                        aiNewState(pSprite, pXSprite, &cerberus2Burn);
                        break;
                    }
                }
                else if (nDist < 0xb00 && nDist > 0x500 && klabs(nDeltaAngle) < 85)
                {
                    switch (pSprite->type)
                    {
                    case 227:
                        aiNewState(pSprite, pXSprite, &cerberus3Burn);
                        break;
                    case 228:
                        aiNewState(pSprite, pXSprite, &cerberus4Burn);
                        break;
                    }
                }
                else if (nDist < 0x200 && klabs(nDeltaAngle) < 85)
                {
                    int hit = HitScan(pSprite, pSprite->z, dx, dy, 0, CLIPMASK1, 0);
                    switch (pSprite->type)
                    {
                    case 227:
                        switch (hit)
                        {
                        case -1:
                            aiNewState(pSprite, pXSprite, &cerberusBite);
                            break;
                        case 3:
                            if (pSprite->type != sprite[gHitInfo.hitsprite].type && sprite[gHitInfo.hitsprite].type != 211)
                                aiNewState(pSprite, pXSprite, &cerberusBite);
                            break;
                        case 0:
                        case 4:
                            break;
                        default:
                            aiNewState(pSprite, pXSprite, &cerberusBite);
                            break;
                        }
                        break;
                    case 228:
                        switch (hit)
                        {
                        case -1:
                            aiNewState(pSprite, pXSprite, &cerberus2Bite);
                            break;
                        case 3:
                            if (pSprite->type != sprite[gHitInfo.hitsprite].type && sprite[gHitInfo.hitsprite].type != 211)
                                aiNewState(pSprite, pXSprite, &cerberus2Bite);
                            break;
                        case 0:
                        case 4:
                            break;
                        default:
                            aiNewState(pSprite, pXSprite, &cerberus2Bite);
                            break;
                        }
                        break;
                    }
                }
                return;
            }
        }
    }

    switch (pSprite->type)
    {
    case 227:
        aiNewState(pSprite, pXSprite, &cerberusGoto);
        break;
    case 228:
        aiNewState(pSprite, pXSprite, &cerberus2Goto);
        break;
    }
    pXSprite->target = -1;
}
