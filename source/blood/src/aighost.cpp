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
#include "aighost.h"
#include "blood.h"
#include "db.h"
#include "dude.h"
#include "eventq.h"
#include "levels.h"
#include "player.h"
#include "seq.h"
#include "sfx.h"
#include "trig.h"

static void SlashSeqCallback(int, int);
static void ThrowSeqCallback(int, int);
static void BlastSeqCallback(int, int);
static void thinkTarget(spritetype *, XSPRITE *);
static void thinkSearch(spritetype *, XSPRITE *);
static void thinkGoto(spritetype *, XSPRITE *);
static void MoveDodgeUp(spritetype *, XSPRITE *);
static void MoveDodgeDown(spritetype *, XSPRITE *);
static void thinkChase(spritetype *, XSPRITE *);
static void MoveForward(spritetype *, XSPRITE *);
static void MoveSlow(spritetype *, XSPRITE *);
static void MoveSwoop(spritetype *, XSPRITE *);
static void MoveFly(spritetype *, XSPRITE *);

static int nSlashClient = seqRegisterClient(SlashSeqCallback);
static int nThrowClient = seqRegisterClient(ThrowSeqCallback);
static int nBlastClient = seqRegisterClient(BlastSeqCallback);

AISTATE ghostIdle = { kAiStateIdle, 0, -1, 0, NULL, NULL, thinkTarget, NULL };
AISTATE ghostChase = { kAiStateChase, 0, -1, 0, NULL, MoveForward, thinkChase, &ghostIdle };
AISTATE ghostGoto = { kAiStateMove, 0, -1, 600, NULL, MoveForward, thinkGoto, &ghostIdle };
AISTATE ghostSlash = { kAiStateChase, 6, nSlashClient, 120, NULL, NULL, NULL, &ghostChase };
AISTATE ghostThrow = { kAiStateChase, 6, nThrowClient, 120, NULL, NULL, NULL, &ghostChase };
AISTATE ghostBlast = { kAiStateChase, 6, nBlastClient, 120, NULL, MoveSlow, NULL, &ghostChase };
AISTATE ghostRecoil = { kAiStateRecoil, 5, -1, 0, NULL, NULL, NULL, &ghostChase };
AISTATE ghostTeslaRecoil = { kAiStateRecoil, 4, -1, 0, NULL, NULL, NULL, &ghostChase };
AISTATE ghostSearch = { kAiStateSearch, 0, -1, 120, NULL, MoveForward, thinkSearch, &ghostIdle };
AISTATE ghostSwoop = { kAiStateOther, 0, -1, 120, NULL, MoveSwoop, thinkChase, &ghostChase };
AISTATE ghostFly = { kAiStateMove, 0, -1, 0, NULL, MoveFly, thinkChase, &ghostChase };
AISTATE ghostTurn = { kAiStateMove, 0, -1, 120, NULL, aiMoveTurn, NULL, &ghostChase };
AISTATE ghostDodgeUp = { kAiStateMove, 0, -1, 60, NULL, MoveDodgeUp, NULL, &ghostChase };
AISTATE ghostDodgeUpRight = { kAiStateMove, 0, -1, 90, NULL, MoveDodgeUp, NULL, &ghostChase };
AISTATE ghostDodgeUpLeft = { kAiStateMove, 0, -1, 90, NULL, MoveDodgeUp, NULL, &ghostChase };
AISTATE ghostDodgeDown = { kAiStateMove, 0, -1, 120, NULL, MoveDodgeDown, NULL, &ghostChase };
AISTATE ghostDodgeDownRight = { kAiStateMove, 0, -1, 90, NULL, MoveDodgeDown, NULL, &ghostChase };
AISTATE ghostDodgeDownLeft = { kAiStateMove, 0, -1, 90, NULL, MoveDodgeDown, NULL, &ghostChase };

static void SlashSeqCallback(int, int nXSprite)
{
    XSPRITE *pXSprite = &xsprite[nXSprite];
    int nSprite = pXSprite->reference;
    spritetype *pSprite = &sprite[nSprite];
    spritetype *pTarget = &sprite[pXSprite->target];
    DUDEINFO *pDudeInfo = &dudeInfo[pSprite->type-kDudeBase];
    DUDEINFO *pDudeInfoT = &dudeInfo[pTarget->type-kDudeBase];
    int height = (pSprite->yrepeat*pDudeInfo->eyeHeight)<<2;
    int height2 = (pTarget->yrepeat*pDudeInfoT->eyeHeight)<<2;
    int dz = height-height2;
    int dx = Cos(pSprite->ang)>>16;
    int dy = Sin(pSprite->ang)>>16;
    sfxPlay3DSound(pSprite, 1406, 0, 0);
    actFireVector(pSprite, 0, 0, dx, dy, dz, VECTOR_TYPE_12);
    int r1 = Random(50);
    int r2 = Random(50);
    actFireVector(pSprite, 0, 0, dx+r2, dy-r1, dz, VECTOR_TYPE_12);
    r1 = Random(50);
    r2 = Random(50);
    actFireVector(pSprite, 0, 0, dx-r2, dy+r1, dz, VECTOR_TYPE_12);
}

static void ThrowSeqCallback(int, int nXSprite)
{
    XSPRITE *pXSprite = &xsprite[nXSprite];
    int nSprite = pXSprite->reference;
    actFireThing(&sprite[nSprite], 0, 0, gDudeSlope[nXSprite]-7500, 421, 0xeeeee);
}

static void BlastSeqCallback(int, int nXSprite)
{
    XSPRITE *pXSprite = &xsprite[nXSprite];
    int nSprite = pXSprite->reference;
    spritetype *pSprite = &sprite[nSprite];
    wrand(); // ???
    spritetype *pTarget = &sprite[pXSprite->target];
    int height = (pSprite->yrepeat*dudeInfo[pSprite->type-kDudeBase].eyeHeight) << 2;
    int dx = pXSprite->targetX-pSprite->x;
    int dy = pXSprite->targetY-pSprite->y;
    int UNUSED(nDist) = approxDist(dx, dy);
    int UNUSED(nAngle) = getangle(dx, dy);
    int x = pSprite->x;
    int y = pSprite->y;
    int z = height;
    TARGETTRACK tt = { 0x10000, 0x10000, 0x100, 0x55, 0x1aaaaa };
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
        if (tt.at10)
        {
            int t = divscale(nDist, tt.at10, 12);
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
            if (klabs(nDeltaAngle) <= tt.at8)
            {
                int tz = pSprite2->z-pSprite->z;
                if (cansee(x, y, z, pSprite->sectnum, x2, y2, z2, pSprite2->sectnum))
                {
                    nClosest = nDist2;
                    aim.dx = Cos(nAngle)>>16;
                    aim.dy = Sin(nAngle)>>16;
                    aim.dz = divscale(tz, nDist, 10);
                    if (tz > -0x333)
                        aim.dz = divscale(tz, nDist, 10);
                    else if (tz < -0x333 && tz > -0xb33)
                        aim.dz = divscale(tz, nDist, 10)+9460;
                    else if (tz < -0xb33 && tz > -0x3000)
                        aim.dz = divscale(tz, nDist, 10)+9460;
                    else if (tz < -0x3000)
                        aim.dz = divscale(tz, nDist, 10)-7500;
                    else
                        aim.dz = divscale(tz, nDist, 10);
                }
                else
                    aim.dz = divscale(tz, nDist, 10);
            }
        }
    }
    if (IsPlayerSprite(pTarget) || !isOriginalDemo()) // By NoOne: allow fire missile in non-player targets if not a demo
    {
        sfxPlay3DSound(pSprite, 489, 0, 0);
        actFireMissile(pSprite, 0, 0, aim.dx, aim.dy, aim.dz, 307);
    }
}

static void thinkTarget(spritetype *pSprite, XSPRITE *pXSprite)
{
    if (!(pSprite->type >= kDudeBase && pSprite->type < kDudeMax))
        return;
    //dassert(pSprite->type >= kDudeBase && pSprite->type < kDudeMax);
    DUDEINFO *pDudeInfo = &dudeInfo[pSprite->type-kDudeBase];
    DUDEEXTRA_at6_u1 *pDudeExtraE = &gDudeExtra[pSprite->extra].at6.u1;
    if (pDudeExtraE->at8 && pDudeExtraE->at4 < 10)
        pDudeExtraE->at4++;
    else if (pDudeExtraE->at4 >= 10 && pDudeExtraE->at8)
    {
        pXSprite->goalAng += 256;
        POINT3D *pTarget = &baseSprite[pSprite->index];
        aiSetTarget(pXSprite, pTarget->x, pTarget->y, pTarget->z);
        aiNewState(pSprite, pXSprite, &ghostTurn);
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
                pDudeExtraE->at4 = 0;
                aiSetTarget(pXSprite, pPlayer->at5b);
                aiActivateDude(pSprite, pXSprite);
                return;
            }
            else if (nDist < pDudeInfo->hearDist)
            {
                pDudeExtraE->at4 = 0;
                aiSetTarget(pXSprite, x, y, z);
                aiActivateDude(pSprite, pXSprite);
                return;
            }
        }
    }
}

static void thinkSearch(spritetype *pSprite, XSPRITE *pXSprite)
{
    aiChooseDirection(pSprite, pXSprite, pXSprite->goalAng);
    aiThinkTarget(pSprite, pXSprite);
}

static void thinkGoto(spritetype *pSprite, XSPRITE *pXSprite)
{
    if (!(pSprite->type >= kDudeBase && pSprite->type < kDudeMax))
        return;
    //dassert(pSprite->type >= kDudeBase && pSprite->type < kDudeMax);
    DUDEINFO *pDudeInfo = &dudeInfo[pSprite->type - kDudeBase];
    int dx = pXSprite->targetX-pSprite->x;
    int dy = pXSprite->targetY-pSprite->y;
    int nAngle = getangle(dx, dy);
    int nDist = approxDist(dx, dy);
    aiChooseDirection(pSprite, pXSprite, nAngle);
    if (nDist < 512 && klabs(pSprite->ang - nAngle) < pDudeInfo->periphery)
        aiNewState(pSprite, pXSprite, &ghostSearch);
    aiThinkTarget(pSprite, pXSprite);
}

static void MoveDodgeUp(spritetype *pSprite, XSPRITE *pXSprite)
{
    int nSprite = pSprite->index;
    if (!(pSprite->type >= kDudeBase && pSprite->type < kDudeMax))
        return;
    //dassert(pSprite->type >= kDudeBase && pSprite->type < kDudeMax);
    DUDEINFO *pDudeInfo = &dudeInfo[pSprite->type - kDudeBase];
    int nAng = ((pXSprite->goalAng+1024-pSprite->ang)&2047)-1024;
    int nTurnRange = (pDudeInfo->angSpeed<<2)>>4;
    pSprite->ang = (pSprite->ang+ClipRange(nAng, -nTurnRange, nTurnRange))&2047;
    int nCos = Cos(pSprite->ang);
    int nSin = Sin(pSprite->ang);
    int dx = xvel[nSprite];
    int dy = yvel[nSprite];
    int t1 = dmulscale30(dx, nCos, dy, nSin);
    int t2 = dmulscale30(dx, nSin, -dy, nCos);
    if (pXSprite->dodgeDir > 0)
        t2 += pDudeInfo->sideSpeed;
    else
        t2 -= pDudeInfo->sideSpeed;

    xvel[nSprite] = dmulscale30(t1, nCos, t2, nSin);
    yvel[nSprite] = dmulscale30(t1, nSin, -t2, nCos);
    zvel[nSprite] = -0x1d555;
}

static void MoveDodgeDown(spritetype *pSprite, XSPRITE *pXSprite)
{
    int nSprite = pSprite->index;
    if (!(pSprite->type >= kDudeBase && pSprite->type < kDudeMax))
        return;
    //dassert(pSprite->type >= kDudeBase && pSprite->type < kDudeMax);
    DUDEINFO *pDudeInfo = &dudeInfo[pSprite->type - kDudeBase];
    int nAng = ((pXSprite->goalAng+1024-pSprite->ang)&2047)-1024;
    int nTurnRange = (pDudeInfo->angSpeed<<2)>>4;
    pSprite->ang = (pSprite->ang+ClipRange(nAng, -nTurnRange, nTurnRange))&2047;
    if (pXSprite->dodgeDir == 0)
        return;
    int nCos = Cos(pSprite->ang);
    int nSin = Sin(pSprite->ang);
    int dx = xvel[nSprite];
    int dy = yvel[nSprite];
    int t1 = dmulscale30(dx, nCos, dy, nSin);
    int t2 = dmulscale30(dx, nSin, -dy, nCos);
    if (pXSprite->dodgeDir > 0)
        t2 += pDudeInfo->sideSpeed;
    else
        t2 -= pDudeInfo->sideSpeed;

    xvel[nSprite] = dmulscale30(t1, nCos, t2, nSin);
    yvel[nSprite] = dmulscale30(t1, nSin, -t2, nCos);
    zvel[nSprite] = 0x44444;
}

static void thinkChase(spritetype *pSprite, XSPRITE *pXSprite)
{
    if (pXSprite->target == -1)
    {
        aiNewState(pSprite, pXSprite, &ghostGoto);
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
        aiNewState(pSprite, pXSprite, &ghostSearch);
        return;
    }
    if (IsPlayerSprite(pTarget) && powerupCheck(&gPlayer[pTarget->type-kDudePlayer1], 13) > 0)
    {
        aiNewState(pSprite, pXSprite, &ghostSearch);
        return;
    }
    int nDist = approxDist(dx, dy);
    if (nDist <= pDudeInfo->seeDist)
    {
        int nDeltaAngle = ((getangle(dx,dy)+1024-pSprite->ang)&2047)-1024;
        int height = (pDudeInfo->eyeHeight*pSprite->yrepeat)<<2;
        // Should be dudeInfo[pTarget->type-kDudeBase]
        int height2 = (pDudeInfo->eyeHeight*pTarget->yrepeat)<<2;
        int top, bottom;
        GetSpriteExtents(pSprite, &top, &bottom);
        if (cansee(pTarget->x, pTarget->y, pTarget->z, pTarget->sectnum, pSprite->x, pSprite->y, pSprite->z - height, pSprite->sectnum))
        {
            if (nDist < pDudeInfo->seeDist && klabs(nDeltaAngle) <= pDudeInfo->periphery)
            {
                aiSetTarget(pXSprite, pXSprite->target);
                int floorZ = getflorzofslope(pSprite->sectnum, pSprite->x, pSprite->y);
                switch (pSprite->type)
                {
                case 210:
                    if (nDist < 0x2000 && nDist > 0x1000 && klabs(nDeltaAngle) < 85)
                    {
                        int hit = HitScan(pSprite, pSprite->z, dx, dy, 0, CLIPMASK1, 0);
                        switch (hit)
                        {
                        case -1:
                            aiNewState(pSprite, pXSprite, &ghostBlast);
                            break;
                        case 0:
                        case 4:
                            break;
                        case 3:
                            if (pSprite->type != sprite[gHitInfo.hitsprite].type && sprite[gHitInfo.hitsprite].type != 210)
                                aiNewState(pSprite, pXSprite, &ghostBlast);
                            break;
                        default:
                            aiNewState(pSprite, pXSprite, &ghostBlast);
                            break;
                        }
                    }
                    else if (nDist < 0x400 && klabs(nDeltaAngle) < 85)
                    {
                        int hit = HitScan(pSprite, pSprite->z, dx, dy, 0, CLIPMASK1, 0);
                        switch (hit)
                        {
                        case -1:
                            aiNewState(pSprite, pXSprite, &ghostSlash);
                            break;
                        case 0:
                        case 4:
                            break;
                        case 3:
                            if (pSprite->type != sprite[gHitInfo.hitsprite].type && sprite[gHitInfo.hitsprite].type != 210)
                                aiNewState(pSprite, pXSprite, &ghostSlash);
                            break;
                        default:
                            aiNewState(pSprite, pXSprite, &ghostSlash);
                            break;
                        }
                    }
                    else if ((height2-height > 0x2000 || floorZ-bottom > 0x2000) && nDist < 0x1400 && nDist > 0x800)
                    {
                        aiPlay3DSound(pSprite, 1600, AI_SFX_PRIORITY_1, -1);
                        aiNewState(pSprite, pXSprite, &ghostSwoop);
                    }
                    else if ((height2-height < 0x2000 || floorZ-bottom < 0x2000) && klabs(nDeltaAngle) < 85)
                        aiPlay3DSound(pSprite, 1600, AI_SFX_PRIORITY_1, -1);
                    break;
                }
            }
            return;
        }
        else
        {
            aiNewState(pSprite, pXSprite, &ghostFly);
            return;
        }
    }

    aiNewState(pSprite, pXSprite, &ghostGoto);
    pXSprite->target = -1;
}

static void MoveForward(spritetype *pSprite, XSPRITE *pXSprite)
{
    int nSprite = pSprite->index;
    if (!(pSprite->type >= kDudeBase && pSprite->type < kDudeMax))
        return;
    //dassert(pSprite->type >= kDudeBase && pSprite->type < kDudeMax);
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
    if ((unsigned int)Random(64) < 32 && nDist <= 0x400)
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
        t1 += nAccel>>1;
    xvel[nSprite] = dmulscale30(t1, nCos, t2, nSin);
    yvel[nSprite] = dmulscale30(t1, nSin, -t2, nCos);
}

static void MoveSlow(spritetype *pSprite, XSPRITE *pXSprite)
{
    int nSprite = pSprite->index;
    if (!(pSprite->type >= kDudeBase && pSprite->type < kDudeMax))
        return;
    //dassert(pSprite->type >= kDudeBase && pSprite->type < kDudeMax);
    DUDEINFO *pDudeInfo = &dudeInfo[pSprite->type - kDudeBase];
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
    t1 = nAccel>>1;
    t2 >>= 1;
    xvel[nSprite] = dmulscale30(t1, nCos, t2, nSin);
    yvel[nSprite] = dmulscale30(t1, nSin, -t2, nCos);
    switch (pSprite->type)
    {
    case 210:
        zvel[nSprite] = 0x44444;
        break;
    }
}

static void MoveSwoop(spritetype *pSprite, XSPRITE *pXSprite)
{
    int nSprite = pSprite->index;
    if (!(pSprite->type >= kDudeBase && pSprite->type < kDudeMax))
        return;
    //dassert(pSprite->type >= kDudeBase && pSprite->type < kDudeMax);
    DUDEINFO *pDudeInfo = &dudeInfo[pSprite->type - kDudeBase];
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
    t1 += nAccel>>1;
    xvel[nSprite] = dmulscale30(t1, nCos, t2, nSin);
    yvel[nSprite] = dmulscale30(t1, nSin, -t2, nCos);
    switch (pSprite->type)
    {
    case 210:
        zvel[nSprite] = t1;
        break;
    }
}

static void MoveFly(spritetype *pSprite, XSPRITE *pXSprite)
{
    int nSprite = pSprite->index;
    if (!(pSprite->type >= kDudeBase && pSprite->type < kDudeMax))
        return;
    //dassert(pSprite->type >= kDudeBase && pSprite->type < kDudeMax);
    DUDEINFO *pDudeInfo = &dudeInfo[pSprite->type - kDudeBase];
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
    switch (pSprite->type)
    {
    case 210:
        zvel[nSprite] = -t1;
        break;
    }
}
