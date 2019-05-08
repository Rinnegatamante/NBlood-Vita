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
#include "aicaleb.h"
#include "blood.h"
#include "db.h"
#include "dude.h"
#include "levels.h"
#include "player.h"
#include "seq.h"
#include "sfx.h"
#include "trig.h"

static void SeqAttackCallback(int, int);
static void thinkSearch(spritetype *, XSPRITE *);
static void thinkGoto(spritetype *, XSPRITE *);
static void thinkChase(spritetype *, XSPRITE *);
static void thinkSwimGoto(spritetype *, XSPRITE *);
static void thinkSwimChase(spritetype *, XSPRITE *);
static void sub_65D04(spritetype *, XSPRITE *);
static void sub_65F44(spritetype *, XSPRITE *);
static void sub_661E0(spritetype *, XSPRITE *);

static int nAttackClient = seqRegisterClient(SeqAttackCallback);

AISTATE tinycalebIdle = { kAiStateIdle, 0, -1, 0, NULL, NULL, aiThinkTarget, NULL };
AISTATE tinycalebChase = { kAiStateChase, 6, -1, 0, NULL, aiMoveForward, thinkChase, NULL };
AISTATE tinycalebDodge = { kAiStateMove, 6, -1, 90, NULL, aiMoveDodge, NULL, &tinycalebChase };
AISTATE tinycalebGoto = { kAiStateMove, 6, -1, 600, NULL, aiMoveForward, thinkGoto, &tinycalebIdle };
AISTATE tinycalebAttack = { kAiStateChase, 0, nAttackClient, 120, NULL, NULL, NULL, &tinycalebChase };
AISTATE tinycalebSearch = { kAiStateSearch, 6, -1, 120, NULL, aiMoveForward, thinkSearch, &tinycalebIdle };
AISTATE tinycalebRecoil = { kAiStateRecoil, 5, -1, 0, NULL, NULL, NULL, &tinycalebDodge };
AISTATE tinycalebTeslaRecoil = { kAiStateRecoil, 4, -1, 0, NULL, NULL, NULL, &tinycalebDodge };
AISTATE tinycalebSwimIdle = { kAiStateIdle, 10, -1, 0, NULL, NULL, aiThinkTarget, NULL };
AISTATE tinycalebSwimChase = { kAiStateChase, 8, -1, 0, NULL, sub_65D04, thinkSwimChase, NULL };
AISTATE tinycalebSwimDodge = { kAiStateMove, 8, -1, 90, NULL, aiMoveDodge, NULL, &tinycalebSwimChase };
AISTATE tinycalebSwimGoto = { kAiStateMove, 8, -1, 600, NULL, aiMoveForward, thinkSwimGoto, &tinycalebSwimIdle };
AISTATE tinycalebSwimSearch = { kAiStateSearch, 8, -1, 120, NULL, aiMoveForward, thinkSearch, &tinycalebSwimIdle };
AISTATE tinycalebSwimAttack = { kAiStateChase, 10, nAttackClient, 0, NULL, NULL, NULL, &tinycalebSwimChase };
AISTATE tinycalebSwimRecoil = { kAiStateRecoil, 5, -1, 0, NULL, NULL, NULL, &tinycalebSwimDodge };
AISTATE tinycaleb139660 = { kAiStateOther, 8, -1, 120, NULL, sub_65F44, thinkSwimChase, &tinycalebSwimChase };
AISTATE tinycaleb13967C = { kAiStateOther, 8, -1, 0, NULL, sub_661E0, thinkSwimChase, &tinycalebSwimChase };
AISTATE tinycaleb139698 = { kAiStateOther, 8, -1, 120, NULL, aiMoveTurn, NULL, &tinycalebSwimChase };

static void SeqAttackCallback(int, int nXSprite)
{
    int nSprite = xsprite[nXSprite].reference;
    spritetype *pSprite = &sprite[nSprite];
    int dx = Cos(pSprite->ang)>>16;
    int dy = Sin(pSprite->ang)>>16;
    int dz = gDudeSlope[nXSprite];
    dx += Random2(1500);
    dy += Random2(1500);
    dz += Random2(1500);
    for (int i = 0; i < 2; i++)
    {
        int r1 = Random3(500);
        int r2 = Random3(1000);
        int r3 = Random3(1000);
        actFireVector(pSprite, 0, 0, dx+r3, dy+r2, dz+r1, VECTOR_TYPE_1);
    }
    if (Chance(0x8000))
        sfxPlay3DSound(pSprite, 10000+Random(5), -1, 0);
    if (Chance(0x8000))
        sfxPlay3DSound(pSprite, 1001, -1, 0);
    else
        sfxPlay3DSound(pSprite, 1002, -1, 0);
}

static void thinkSearch(spritetype *pSprite, XSPRITE *pXSprite)
{
    aiChooseDirection(pSprite, pXSprite, pXSprite->goalAng);
    aiThinkTarget(pSprite, pXSprite);
}

static void thinkGoto(spritetype *pSprite, XSPRITE *pXSprite)
{
    dassert(pSprite->type >= kDudeBase && pSprite->type < kDudeMax);
    DUDEINFO *pDudeInfo = &dudeInfo[pSprite->type - kDudeBase];
    XSECTOR *pXSector;
    int nXSector = sector[pSprite->sectnum].extra;
    if (nXSector > 0)
        pXSector = &xsector[nXSector];
    else
        pXSector = NULL;
    int dx = pXSprite->targetX-pSprite->x;
    int dy = pXSprite->targetY-pSprite->y;
    int nAngle = getangle(dx, dy);
    int nDist = approxDist(dx, dy);
    aiChooseDirection(pSprite, pXSprite, nAngle);
    if (nDist < 512 && klabs(pSprite->ang - nAngle) < pDudeInfo->periphery)
    {
        if (pXSector && pXSector->Underwater)
            aiNewState(pSprite, pXSprite, &tinycalebSwimSearch);
        else
            aiNewState(pSprite, pXSprite, &tinycalebSearch);
    }
    aiThinkTarget(pSprite, pXSprite);
}

static void thinkChase(spritetype *pSprite, XSPRITE *pXSprite)
{
    if (pXSprite->target == -1)
    {
        XSECTOR *pXSector;
        int nXSector = sector[pSprite->sectnum].extra;
        if (nXSector > 0)
            pXSector = &xsector[nXSector];
        else
            pXSector = NULL;
        if (pXSector && pXSector->Underwater)
            aiNewState(pSprite, pXSprite, &tinycalebSwimSearch);
        else
            aiNewState(pSprite, pXSprite, &tinycalebSearch);
        return;
    }
    dassert(pSprite->type >= kDudeBase && pSprite->type < kDudeMax);
    DUDEINFO *pDudeInfo = &dudeInfo[pSprite->type - kDudeBase];
    dassert(pXSprite->target >= 0 && pXSprite->target < kMaxSprites);
    spritetype *pTarget = &sprite[pXSprite->target];
    XSPRITE *pXTarget = &xsprite[pTarget->extra];
    int dx = pTarget->x-pSprite->x;
    int dy = pTarget->y-pSprite->y;
    aiChooseDirection(pSprite, pXSprite, getangle(dx, dy));
    if (pXTarget->health == 0)
    {
        XSECTOR *pXSector;
        int nXSector = sector[pSprite->sectnum].extra;
        if (nXSector > 0)
            pXSector = &xsector[nXSector];
        else
            pXSector = NULL;
        if (pXSector && pXSector->Underwater)
            aiNewState(pSprite, pXSprite, &tinycalebSwimSearch);
        else
        {
            aiPlay3DSound(pSprite, 11000+Random(4), AI_SFX_PRIORITY_1, -1);
            aiNewState(pSprite, pXSprite, &tinycalebSearch);
        }
        return;
    }
    if (IsPlayerSprite(pTarget) && powerupCheck(&gPlayer[pTarget->type-kDudePlayer1], 13) > 0)
    {
        XSECTOR *pXSector;
        int nXSector = sector[pSprite->sectnum].extra;
        if (nXSector > 0)
            pXSector = &xsector[nXSector];
        else
            pXSector = NULL;
        if (pXSector && pXSector->Underwater)
            aiNewState(pSprite, pXSprite, &tinycalebSwimSearch);
        else
            aiNewState(pSprite, pXSprite, &tinycalebSearch);
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
                int nXSprite = sprite[pXSprite->reference].extra;
                gDudeSlope[nXSprite] = divscale(pTarget->z-pSprite->z, nDist, 10);
                if (nDist < 0x599 && klabs(nDeltaAngle) < 28)
                {
                    XSECTOR *pXSector;
                    int nXSector = sector[pSprite->sectnum].extra;
                    if (nXSector > 0)
                        pXSector = &xsector[nXSector];
                    else
                        pXSector = NULL;
                    int hit = HitScan(pSprite, pSprite->z, dx, dy, 0, CLIPMASK1, 0);
                    switch (hit)
                    {
                    case -1:
                        if (pXSector && pXSector->Underwater)
                            aiNewState(pSprite, pXSprite, &tinycalebSwimAttack);
                        else
                            aiNewState(pSprite, pXSprite, &tinycalebAttack);
                        break;
                    case 3:
                        if (pSprite->type != sprite[gHitInfo.hitsprite].type)
                        {
                            if (pXSector && pXSector->Underwater)
                                aiNewState(pSprite, pXSprite, &tinycalebSwimAttack);
                            else
                                aiNewState(pSprite, pXSprite, &tinycalebAttack);
                        }
                        else
                        {
                            if (pXSector && pXSector->Underwater)
                                aiNewState(pSprite, pXSprite, &tinycalebSwimDodge);
                            else
                                aiNewState(pSprite, pXSprite, &tinycalebDodge);
                        }
                        break;
                    default:
                        if (pXSector && pXSector->Underwater)
                            aiNewState(pSprite, pXSprite, &tinycalebSwimAttack);
                        else
                            aiNewState(pSprite, pXSprite, &tinycalebAttack);
                        break;
                    }
                }
            }
            return;
        }
    }

    XSECTOR *pXSector;
    int nXSector = sector[pSprite->sectnum].extra;
    if (nXSector > 0)
        pXSector = &xsector[nXSector];
    else
        pXSector = NULL;
    if (pXSector && pXSector->Underwater)
        aiNewState(pSprite, pXSprite, &tinycalebSwimGoto);
    else
        aiNewState(pSprite, pXSprite, &tinycalebGoto);
    if (Chance(0x2000))
        sfxPlay3DSound(pSprite, 10000 + Random(5), -1, 0);
    pXSprite->target = -1;
}

static void thinkSwimGoto(spritetype *pSprite, XSPRITE *pXSprite)
{
    dassert(pSprite->type >= kDudeBase && pSprite->type < kDudeMax);
    DUDEINFO *pDudeInfo = &dudeInfo[pSprite->type - kDudeBase];
    int dx = pXSprite->targetX-pSprite->x;
    int dy = pXSprite->targetY-pSprite->y;
    int nAngle = getangle(dx, dy);
    int nDist = approxDist(dx, dy);
    aiChooseDirection(pSprite, pXSprite, nAngle);
    if (nDist < 512 && klabs(pSprite->ang - nAngle) < pDudeInfo->periphery)
        aiNewState(pSprite, pXSprite, &tinycalebSwimSearch);
    aiThinkTarget(pSprite, pXSprite);
}

static void thinkSwimChase(spritetype *pSprite, XSPRITE *pXSprite)
{
    if (pXSprite->target == -1)
    {
        aiNewState(pSprite, pXSprite, &tinycalebSwimGoto);
        return;
    }
    dassert(pSprite->type >= kDudeBase && pSprite->type < kDudeMax);
    DUDEINFO *pDudeInfo = &dudeInfo[pSprite->type - kDudeBase];
    dassert(pXSprite->target >= 0 && pXSprite->target < kMaxSprites);
    spritetype *pTarget = &sprite[pXSprite->target];
    XSPRITE *pXTarget = &xsprite[pTarget->extra];
    int dx = pTarget->x-pSprite->x;
    int dy = pTarget->y-pSprite->y;
    aiChooseDirection(pSprite, pXSprite, getangle(dx, dy));
    if (pXTarget->health == 0)
    {
        aiNewState(pSprite, pXSprite, &tinycalebSwimSearch);
        return;
    }
    if (IsPlayerSprite(pTarget) && powerupCheck(&gPlayer[pTarget->type-kDudePlayer1], 13) > 0)
    {
        aiNewState(pSprite, pXSprite, &tinycalebSwimSearch);
        return;
    }
    int nDist = approxDist(dx, dy);
    if (nDist <= pDudeInfo->seeDist)
    {
        int nDeltaAngle = ((getangle(dx,dy)+1024-pSprite->ang)&2047)-1024;
        int height = pDudeInfo->eyeHeight+pSprite->z;
        int top, bottom;
        GetSpriteExtents(pSprite, &top, &bottom);
        if (cansee(pTarget->x, pTarget->y, pTarget->z, pTarget->sectnum, pSprite->x, pSprite->y, pSprite->z - height, pSprite->sectnum))
        {
            if (nDist < pDudeInfo->seeDist && klabs(nDeltaAngle) <= pDudeInfo->periphery)
            {
                aiSetTarget(pXSprite, pXSprite->target);
                int UNUSED(floorZ) = getflorzofslope(pSprite->sectnum, pSprite->x, pSprite->y);
                if (nDist < 0x400 && klabs(nDeltaAngle) < 85)
                    aiNewState(pSprite, pXSprite, &tinycalebSwimAttack);
                else
                    aiNewState(pSprite, pXSprite, &tinycaleb13967C);
            }
        }
        return;
    }
    aiNewState(pSprite, pXSprite, &tinycalebSwimGoto);
    pXSprite->target = -1;
}

static void sub_65D04(spritetype *pSprite, XSPRITE *pXSprite)
{
    int nSprite = pSprite->index;
    dassert(pSprite->type >= kDudeBase && pSprite->type < kDudeMax);
    DUDEINFO *pDudeInfo = &dudeInfo[pSprite->type - kDudeBase];
    int nAng = ((pXSprite->goalAng+1024-pSprite->ang)&2047)-1024;
    int nTurnRange = (pDudeInfo->angSpeed<<2)>>4;
    pSprite->ang = (pSprite->ang+ClipRange(nAng, -nTurnRange, nTurnRange))&2047;
    int nAccel = pDudeInfo->frontSpeed<<2;
    if (klabs(nAng) > 341)
        return;
    if (pXSprite->target == -1)
        pSprite->ang = (pSprite->ang+256)&2047;
    int dx = pXSprite->targetX-pSprite->x;
    int dy = pXSprite->targetY-pSprite->y;
    int UNUSED(nAngle) = getangle(dx, dy);
    int nDist = approxDist(dx, dy);
    if (Random(64) < 32 && nDist <= 0x400)
        return;
    int nCos = Cos(pSprite->ang);
    int nSin = Sin(pSprite->ang);
    int vx = xvel[nSprite];
    int vy = yvel[nSprite];
    int t1 = dmulscale30(vx, nCos, vy, nSin);
    int t2 = dmulscale30(vx, nSin, -vy, nCos);
    if (pXSprite->target == -1)
        t1 += nAccel;
    else
        t1 += nAccel>>2;
    xvel[nSprite] = dmulscale30(t1, nCos, t2, nSin);
    yvel[nSprite] = dmulscale30(t1, nSin, -t2, nCos);
}

static void sub_65F44(spritetype *pSprite, XSPRITE *pXSprite)
{
    int nSprite = pSprite->index;
    dassert(pSprite->type >= kDudeBase && pSprite->type < kDudeMax);
    DUDEINFO *pDudeInfo = &dudeInfo[pSprite->type - kDudeBase];
    spritetype *pTarget = &sprite[pXSprite->target];
    int z = pSprite->z + dudeInfo[pSprite->type - kDudeBase].eyeHeight;
    int z2 = pTarget->z + dudeInfo[pTarget->type - kDudeBase].eyeHeight;
    int nAng = ((pXSprite->goalAng+1024-pSprite->ang)&2047)-1024;
    int nTurnRange = (pDudeInfo->angSpeed<<2)>>4;
    pSprite->ang = (pSprite->ang+ClipRange(nAng, -nTurnRange, nTurnRange))&2047;
    int nAccel = pDudeInfo->frontSpeed<<2;
    if (klabs(nAng) > 341)
    {
        pXSprite->goalAng = (pSprite->ang+512)&2047;
        return;
    }
    int dx = pXSprite->targetX-pSprite->x;
    int dy = pXSprite->targetY-pSprite->y;
    int dz = z2 - z;
    int UNUSED(nAngle) = getangle(dx, dy);
    int nDist = approxDist(dx, dy);
    if (Chance(0x600) && nDist <= 0x400)
        return;
    int nCos = Cos(pSprite->ang);
    int nSin = Sin(pSprite->ang);
    int vx = xvel[nSprite];
    int vy = yvel[nSprite];
    int t1 = dmulscale30(vx, nCos, vy, nSin);
    int t2 = dmulscale30(vx, nSin, -vy, nCos);
    t1 += nAccel;
    xvel[nSprite] = dmulscale30(t1, nCos, t2, nSin);
    yvel[nSprite] = dmulscale30(t1, nSin, -t2, nCos);
    zvel[nSprite] = -dz;
}

static void sub_661E0(spritetype *pSprite, XSPRITE *pXSprite)
{
    int nSprite = pSprite->index;
    dassert(pSprite->type >= kDudeBase && pSprite->type < kDudeMax);
    DUDEINFO *pDudeInfo = &dudeInfo[pSprite->type - kDudeBase];
    spritetype *pTarget = &sprite[pXSprite->target];
    int z = pSprite->z + dudeInfo[pSprite->type - kDudeBase].eyeHeight;
    int z2 = pTarget->z + dudeInfo[pTarget->type - kDudeBase].eyeHeight;
    int nAng = ((pXSprite->goalAng+1024-pSprite->ang)&2047)-1024;
    int nTurnRange = (pDudeInfo->angSpeed<<2)>>4;
    pSprite->ang = (pSprite->ang+ClipRange(nAng, -nTurnRange, nTurnRange))&2047;
    int nAccel = pDudeInfo->frontSpeed<<2;
    if (klabs(nAng) > 341)
    {
        pSprite->ang = (pSprite->ang+512)&2047;
        return;
    }
    int dx = pXSprite->targetX-pSprite->x;
    int dy = pXSprite->targetY-pSprite->y;
    int dz = (z2 - z)<<3;
    int UNUSED(nAngle) = getangle(dx, dy);
    int nDist = approxDist(dx, dy);
    if (Chance(0x4000) && nDist <= 0x400)
        return;
    int nCos = Cos(pSprite->ang);
    int nSin = Sin(pSprite->ang);
    int vx = xvel[nSprite];
    int vy = yvel[nSprite];
    int t1 = dmulscale30(vx, nCos, vy, nSin);
    int t2 = dmulscale30(vx, nSin, -vy, nCos);
    t1 += nAccel>>1;
    xvel[nSprite] = dmulscale30(t1, nCos, t2, nSin);
    yvel[nSprite] = dmulscale30(t1, nSin, -t2, nCos);
    zvel[nSprite] = dz;
}
