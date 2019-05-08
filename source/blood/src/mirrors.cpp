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
#include "build.h"
#include "compat.h"
#ifdef POLYMER
#include "polymer.h"
#endif
#include "common_game.h"
#include "blood.h"
#include "db.h"
#include "gameutil.h"
#include "loadsave.h"
#include "player.h"
#include "trig.h"
#include "view.h"
#include "warp.h"

int mirrorcnt, mirrorsector, mirrorwall[4];

typedef struct
{
    int at0;
    int at4;
    int at8;
    int atc;
    int at10;
    int at14;
} MIRROR;

MIRROR mirror[16];

#ifdef POLYMER
void PolymerRORCallback(int16_t sectnum, int16_t wallnum, int8_t rorstat, int16_t* msectnum, int32_t* gx, int32_t* gy, int32_t* gz)
{
    UNREFERENCED_PARAMETER(wallnum);
    int nMirror;
    switch (rorstat)
    {
    case 1:
        nMirror = sector[sectnum].ceilingpicnum-4080;
        *msectnum = mirror[nMirror].at4;
        *gx += mirror[nMirror].at8;
        *gy += mirror[nMirror].atc;
        *gz += mirror[nMirror].at10;
        break;
    case 2:
        nMirror = sector[sectnum].floorpicnum-4080;
        *msectnum = mirror[nMirror].at4;
        *gx += mirror[nMirror].at8;
        *gy += mirror[nMirror].atc;
        *gz += mirror[nMirror].at10;
        break;
    }
}
#endif

void InitMirrors(void)
{

#ifdef USE_OPENGL
    r_rortexture = 4080;
    r_rortexturerange = 16;
#ifdef  POLYMER
    polymer_setrorcallback(PolymerRORCallback);
#endif //  POLYMER

#endif
    mirrorcnt = 0;
    tilesiz[504].x = 0;
    tilesiz[504].y = 0;
    tileDelete(504);
    
    for(int i = 0; i < 16; i++)
        tilesiz[4080+i].x = 0, tilesiz[4080+i].y = 0;
    for (int i = numwalls - 1; i >= 0; i--)
    {
        if (mirrorcnt == 16)
            break;
        int nTile = 4080+mirrorcnt;
        if (wall[i].overpicnum == 504)
        {
            if (wall[i].extra > 0 && wall[i].lotag == 501)
            {
                wall[i].overpicnum = nTile;
                mirror[mirrorcnt].at14 = i;
                mirror[mirrorcnt].at0 = 0;
                wall[i].cstat |= 32;
                int tmp = xwall[wall[i].extra].txID;
                int j;
                for (j = numwalls - 1; j >= 0; j--)
                {
                    if (j == i)
                        continue;
                    if (wall[j].extra > 0 && wall[j].lotag == 501)
                    {
                        if (tmp != xwall[wall[j].extra].txID)
                            continue;
                        wall[i].hitag = j;
                        wall[j].hitag = i;
                        mirror[mirrorcnt].at4 = j;
                        break;
                    }
                }
                if (j < 0)
                    ThrowError("wall[%d] has no matching wall link! (data=%d)\n", i, tmp);
                mirrorcnt++;
            }
            continue;
        }
        if (wall[i].picnum == 504)
        {
            mirror[mirrorcnt].at4 = i;
            mirror[mirrorcnt].at14 = i;
            wall[i].picnum = nTile;
            mirror[mirrorcnt].at0 = 0;
            wall[i].cstat |= 32;
            mirrorcnt++;
            continue;
        }
    }
    for (int i = numsectors - 1; i >= 0; i--)
    {
        if (mirrorcnt >= 15)
            break;

        if (sector[i].floorpicnum == 504)
        {
            int nLink = gUpperLink[i];
            if (nLink < 0)
                continue;
            int nLink2 = sprite[nLink].owner & 0xfff;
            int j = sprite[nLink2].sectnum;
            if (sector[j].ceilingpicnum != 504)
                ThrowError("Lower link sector %d doesn't have mirror picnum\n", j);
            mirror[mirrorcnt].at0 = 2;
            mirror[mirrorcnt].at8 = sprite[nLink2].x-sprite[nLink].x;
            mirror[mirrorcnt].atc = sprite[nLink2].y-sprite[nLink].y;
            mirror[mirrorcnt].at10 = sprite[nLink2].z-sprite[nLink].z;
            mirror[mirrorcnt].at14 = i;
            mirror[mirrorcnt].at4 = j;
            sector[i].floorpicnum = 4080+mirrorcnt;
            mirrorcnt++;
            mirror[mirrorcnt].at0 = 1;
            mirror[mirrorcnt].at8 = sprite[nLink].x-sprite[nLink2].x;
            mirror[mirrorcnt].atc = sprite[nLink].y-sprite[nLink2].y;
            mirror[mirrorcnt].at10 = sprite[nLink].z-sprite[nLink2].z;
            mirror[mirrorcnt].at14 = j;
            mirror[mirrorcnt].at4 = i;
            sector[j].ceilingpicnum = 4080+mirrorcnt;
            mirrorcnt++;
        }
    }
    mirrorsector = numsectors;
    for (int i = 0; i < 4; i++)
    {
        mirrorwall[i] = numwalls+i;
        wall[mirrorwall[i]].picnum = 504;
        wall[mirrorwall[i]].overpicnum = 504;
        wall[mirrorwall[i]].cstat = 0;
        wall[mirrorwall[i]].nextsector = -1;
        wall[mirrorwall[i]].nextwall = -1;
        wall[mirrorwall[i]].point2 = numwalls;
    }
    wall[mirrorwall[3]].point2 = mirrorwall[0];
    sector[mirrorsector].ceilingpicnum = 504;
    sector[mirrorsector].floorpicnum = 504;
    sector[mirrorsector].wallptr = mirrorwall[0];
    sector[mirrorsector].wallnum = 4;
}

void TranslateMirrorColors(int nShade, int nPalette)
{
    if (videoGetRenderMode() != REND_CLASSIC)
        return;
    videoBeginDrawing();
    nShade = ClipRange(nShade, 0, 63);
    char *pMap = palookup[nPalette] + (nShade<<8);
    extern intptr_t frameplace;
    char *pFrame = (char*)frameplace;
    unsigned int nPixels = xdim*ydim;
    for (unsigned int i = 0; i < nPixels; i++, pFrame++)
    {
        *pFrame = pMap[*pFrame];
    }
    videoEndDrawing();
}

void sub_5571C(char mode)
{
    for (int i = mirrorcnt-1; i >= 0; i--)
    {
        int nTile = 4080+i;
        if (TestBitString(gotpic, nTile))
        {
            switch (mirror[i].at0)
            {
                case 1:
                    if (mode)
                        sector[mirror[i].at14].ceilingstat |= 1;
                    else
                        sector[mirror[i].at14].ceilingstat &= ~1;
                    break;
                case 2:
                    if (mode)
                        sector[mirror[i].at14].floorstat |= 1;
                    else
                        sector[mirror[i].at14].floorstat &= ~1;
                    break;
            }
        }
    }
}

void sub_557C4(int x, int y, int interpolation)
{
    if (spritesortcnt == 0) return;
    int nViewSprites = spritesortcnt-1;
    for (int nTSprite = nViewSprites; nTSprite >= 0; nTSprite--)
    {
        uspritetype *pTSprite = &tsprite[nTSprite];
        pTSprite->xrepeat = pTSprite->yrepeat = 0;
    }
    for (int i = mirrorcnt-1; i >= 0; i--)
    {
        int nTile = 4080+i;
        if (TestBitString(gotpic, nTile))
        {
            if (mirror[i].at0 == 1 || mirror[i].at0 == 2)
            {
                int nSector = mirror[i].at4;
                int nSector2 = mirror[i].at14;
                for (int nSprite = headspritesect[nSector]; nSprite >= 0; nSprite = nextspritesect[nSprite])
                {
                    spritetype *pSprite = &sprite[nSprite];
                    if (pSprite == gView->pSprite)
                        continue;
                    int top, bottom;
                    GetSpriteExtents(pSprite, &top, &bottom);
                    int zCeil, zFloor;
                    getzsofslope(nSector, pSprite->x, pSprite->y, &zCeil, &zFloor);
                    if (pSprite->statnum == 6 && (top < zCeil || bottom > zFloor))
                    {
                        int j = i;
                        if (mirror[i].at0 == 2)
                            j++;
                        else
                            j--;
                        int dx = mirror[j].at8;
                        int dy = mirror[j].atc;
                        int dz = mirror[j].at10;
                        uspritetype *pTSprite = &tsprite[spritesortcnt];
                        memset(pTSprite, 0, sizeof(uspritetype));
                        pTSprite->type = pSprite->type;
                        pTSprite->index = pSprite->index;
                        pTSprite->sectnum = nSector2;
                        pTSprite->x = pSprite->x+dx;
                        pTSprite->y = pSprite->y+dy;
                        pTSprite->z = pSprite->z+dz;
                        pTSprite->ang = pSprite->ang;
                        pTSprite->picnum = pSprite->picnum;
                        pTSprite->shade = pSprite->shade;
                        pTSprite->pal = pSprite->pal;
                        pTSprite->xrepeat = pSprite->xrepeat;
                        pTSprite->yrepeat = pSprite->yrepeat;
                        pTSprite->xoffset = pSprite->xoffset;
                        pTSprite->yoffset = pSprite->yoffset;
                        pTSprite->cstat = pSprite->cstat;
                        pTSprite->statnum = 0;
                        pTSprite->owner = pSprite->index;
                        pTSprite->extra = pSprite->extra;
                        pTSprite->hitag = pSprite->hitag|0x200;
                        LOCATION *pLocation = &gPrevSpriteLoc[pSprite->index];
                        pTSprite->x = dx+interpolate(pLocation->x, pSprite->x, interpolation);
                        pTSprite->y = dy+interpolate(pLocation->y, pSprite->y, interpolation);
                        pTSprite->z = dz+interpolate(pLocation->z, pSprite->z, interpolation);
                        pTSprite->ang = pLocation->ang+mulscale16(((pSprite->ang-pLocation->ang+1024)&2047)-1024,interpolation);
                        spritesortcnt++;
                    }
                }
            }
        }
    }
    for (int nTSprite = spritesortcnt-1; nTSprite >= nViewSprites; nTSprite--)
    {
        uspritetype *pTSprite = &tsprite[nTSprite];
        int nAnim = 0;
        switch (picanm[pTSprite->picnum].extra&7)
        {
        case 1:
        {
            int dX = x - pTSprite->x;
            int dY = y - pTSprite->y;
            RotateVector(&dX, &dY, 128 - pTSprite->ang);
            nAnim = GetOctant(dX, dY);
            if (nAnim <= 4)
            {
                pTSprite->cstat &= ~4;
            }
            else
            {
                nAnim = 8 - nAnim;
                pTSprite->cstat |= 4;
            }
            break;
        }
        case 2:
        {
            int dX = x - pTSprite->x;
            int dY = y - pTSprite->y;
            RotateVector(&dX, &dY, 128 - pTSprite->ang);
            nAnim = GetOctant(dX, dY);
            break;
        }
        }
        while (nAnim > 0)
        {
            pTSprite->picnum += picanm[pTSprite->picnum].num+1;
            nAnim--;
        }
    }
}

void DrawMirrors(int x, int y, int z, fix16_t a, fix16_t horiz)
{
    if (videoGetRenderMode() == REND_POLYMER)
        return;
    for (int i = mirrorcnt - 1; i >= 0; i--)
    {
        int nTile = 4080+i;
        if (TestBitString(gotpic, nTile))
        {
            ClearBitString(gotpic, nTile);
            switch (mirror[i].at0)
            {
            case 0:
            {
                int nWall = mirror[i].at4;
                int nSector = sectorofwall(nWall);
                walltype *pWall = &wall[nWall];
                int nNextWall = pWall->nextwall;
                int nNextSector = pWall->nextsector;
                pWall->nextwall = mirrorwall[0];
                pWall->nextsector = mirrorsector;
                wall[mirrorwall[0]].nextwall = nWall;
                wall[mirrorwall[0]].nextsector = nSector;
                wall[mirrorwall[0]].x = wall[pWall->point2].x;
                wall[mirrorwall[0]].y = wall[pWall->point2].y;
                wall[mirrorwall[1]].x = pWall->x;
                wall[mirrorwall[1]].y = pWall->y;
                wall[mirrorwall[2]].x = wall[mirrorwall[1]].x+(wall[mirrorwall[1]].x-wall[mirrorwall[0]].x)*16;
                wall[mirrorwall[2]].y = wall[mirrorwall[1]].y+(wall[mirrorwall[1]].y-wall[mirrorwall[0]].y)*16;
                wall[mirrorwall[3]].x = wall[mirrorwall[0]].x+(wall[mirrorwall[0]].x-wall[mirrorwall[1]].x)*16;
                wall[mirrorwall[3]].y = wall[mirrorwall[0]].y+(wall[mirrorwall[0]].y-wall[mirrorwall[1]].y)*16;
                sector[mirrorsector].floorz = sector[nSector].floorz;
                sector[mirrorsector].ceilingz = sector[nSector].ceilingz;
                int cx, cy, ca;
                if (pWall->lotag == 501)
                {
                     cx = x - (wall[pWall->hitag].x-wall[pWall->point2].x);
                     cy = y - (wall[pWall->hitag].y-wall[pWall->point2].y);
                     ca = a;
                }
                else
                {
                    //renderPrepareMirror(x,y, fix16_from_int(a),nWall,&cx,&cy,&ca);
                    renderPrepareMirrorOld(x,y,z,a,horiz,nWall,mirrorsector,&cx,&cy,&ca);
                }
                renderDrawRoomsQ16(cx, cy, z, ca,horiz,mirrorsector|MAXSECTORS);
                viewProcessSprites(cx,cy,z);
                renderDrawMasks();
                if (pWall->lotag != 501)
                    renderCompleteMirrorOld();
                if (wall[nWall].pal != 0 || wall[nWall].shade != 0)
                    TranslateMirrorColors(wall[nWall].shade, wall[nWall].pal);
                pWall->nextwall = nNextWall;
                pWall->nextsector = nNextSector;
                return;
            }
            case 1:
            {
#ifdef USE_OPENGL
                r_rorphase = 1;
#endif
                int nSector = mirror[i].at4;
                renderDrawRoomsQ16(x+mirror[i].at8, y+mirror[i].atc, z+mirror[i].at10, a, horiz, nSector|MAXSECTORS);
                viewProcessSprites(x+mirror[i].at8, y+mirror[i].atc, z+mirror[i].at10);
                short fstat = sector[nSector].floorstat;
                sector[nSector].floorstat |= 1;
                renderDrawMasks();
                sector[nSector].floorstat = fstat;
                for (int i = 0; i < 16; i++)
                    ClearBitString(gotpic, 4080+i);
#ifdef USE_OPENGL
                r_rorphase = 0;
#endif
                return;
            }
            case 2:
            {
#ifdef USE_OPENGL
                r_rorphase = 1;
#endif
                int nSector = mirror[i].at4;
                renderDrawRoomsQ16(x+mirror[i].at8, y+mirror[i].atc, z+mirror[i].at10, a, horiz, nSector|MAXSECTORS);
                viewProcessSprites(x+mirror[i].at8, y+mirror[i].atc, z+mirror[i].at10);
                short cstat = sector[nSector].ceilingstat;
                sector[nSector].ceilingstat |= 1;
                renderDrawMasks();
                sector[nSector].ceilingstat = cstat;
                for (int i = 0; i < 16; i++)
                    ClearBitString(gotpic, 4080+i);
#ifdef USE_OPENGL
                r_rorphase = 0;
#endif
                return;
            }
            }
        }
    }
}

class MirrorLoadSave : public LoadSave {
public:
    void Load(void);
    void Save(void);
};

static MirrorLoadSave *myLoadSave;

void MirrorLoadSave::Load(void)
{
    Read(&mirrorcnt,sizeof(mirrorcnt));
    Read(&mirrorsector,sizeof(mirrorsector));
    Read(mirror, sizeof(mirror));
    Read(mirrorwall, sizeof(mirrorwall));
    tilesiz[504].x = 0;
    tilesiz[504].y = 0;

    for (int i = 0; i < 16; i++)
        tilesiz[4080 + i].x = 0, tilesiz[4080 + i].y = 0;
    for (int i = 0; i < 4; i++)
    {
        wall[mirrorwall[i]].picnum = 504;
        wall[mirrorwall[i]].overpicnum = 504;
        wall[mirrorwall[i]].point2 = numwalls;
        wall[mirrorwall[i]].cstat = 0;
        wall[mirrorwall[i]].nextsector = -1;
        wall[mirrorwall[i]].nextwall = -1;
    }
    wall[mirrorwall[3]].point2 = mirrorwall[0];
    sector[mirrorsector].ceilingpicnum = 504;
    sector[mirrorsector].floorpicnum = 504;
    sector[mirrorsector].wallptr = mirrorwall[0];
    sector[mirrorsector].wallnum = 4;
}

void MirrorLoadSave::Save(void)
{
    Write(&mirrorcnt,sizeof(mirrorcnt));
    Write(&mirrorsector,sizeof(mirrorsector));
    Write(mirror, sizeof(mirror));
    Write(mirrorwall, sizeof(mirrorwall));
}

void MirrorLoadSaveConstruct(void)
{
    myLoadSave = new MirrorLoadSave();
}
