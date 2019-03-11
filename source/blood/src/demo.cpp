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
#include <string.h>
#include "common.h"
#include "common_game.h"
#include "keyboard.h"
#include "control.h"
#include "osd.h"
#include "mmulti.h"
#ifdef WITHKPLIB
#include "kplib.h"
#endif

#include "blood.h"
#include "controls.h"
#include "demo.h"
#include "fire.h"
#include "gamemenu.h"
#include "globals.h"
#include "levels.h"
#include "menu.h"
#include "messages.h"
#include "misc.h"
#include "network.h"
#include "player.h"
#include "screen.h"
#include "view.h"

int nBuild = 0;

void ReadGameOptionsLegacy(GAMEOPTIONS &gameOptions, GAMEOPTIONSLEGACY &gameOptionsLegacy)
{
    gameOptions.nGameType = gameOptionsLegacy.nGameType;
    gameOptions.nDifficulty = gameOptionsLegacy.nDifficulty;
    gameOptions.nEpisode = gameOptionsLegacy.nEpisode;
    gameOptions.nLevel = gameOptionsLegacy.nLevel;
    strcpy(gameOptions.zLevelName, gameOptionsLegacy.zLevelName);
    strcpy(gameOptions.zLevelSong, gameOptionsLegacy.zLevelSong);
    gameOptions.nTrackNumber = gameOptionsLegacy.nTrackNumber;
    strcpy(gameOptions.szSaveGameName, gameOptionsLegacy.szSaveGameName);
    strcpy(gameOptions.szUserGameName, gameOptionsLegacy.szUserGameName);
    gameOptions.nSaveGameSlot = gameOptionsLegacy.nSaveGameSlot;
    gameOptions.picEntry = gameOptionsLegacy.picEntry;
    gameOptions.uMapCRC = gameOptionsLegacy.uMapCRC;
    gameOptions.nMonsterSettings = gameOptionsLegacy.nMonsterSettings;
    gameOptions.uGameFlags = gameOptionsLegacy.uGameFlags;
    gameOptions.uNetGameFlags = gameOptionsLegacy.uNetGameFlags;
    gameOptions.nWeaponSettings = gameOptionsLegacy.nWeaponSettings;
    gameOptions.nItemSettings = gameOptionsLegacy.nItemSettings;
    gameOptions.nRespawnSettings = gameOptionsLegacy.nRespawnSettings;
    gameOptions.nTeamSettings = gameOptionsLegacy.nTeamSettings;
    gameOptions.nMonsterRespawnTime = gameOptionsLegacy.nMonsterRespawnTime;
    gameOptions.nWeaponRespawnTime = gameOptionsLegacy.nWeaponRespawnTime;
    gameOptions.nItemRespawnTime = gameOptionsLegacy.nItemRespawnTime;
    gameOptions.nSpecialRespawnTime = gameOptionsLegacy.nSpecialRespawnTime;
}

CDemo gDemo;

CDemo::CDemo()
{
    nBuild = 4;
    at0 = 0;
    at1 = 0;
    at3 = 0;
    hPFile = -1;
    hRFile = NULL;
    atb = 0;
	pFirstDemo = NULL;
    pCurrentDemo = NULL;
    at59ef = 0;
    at2 = 0;
    memset(&atf, 0, sizeof(atf));
    m_bLegacy = false;
}

CDemo::~CDemo()
{
    at0 = 0;
    at1 = 0;
    at3 = 0;
    atb = 0;
    memset(&atf, 0, sizeof(atf));
    if (hPFile >= 0)
    {
        kclose(hPFile);
        hPFile = -1;
    }
    if (hRFile != NULL)
    {
        fclose(hRFile);
        hRFile = NULL;
    }
	auto pNextDemo = pFirstDemo;
    for (auto pDemo = pFirstDemo; pDemo != NULL; pDemo = pNextDemo)
    {
        pNextDemo = pDemo->pNext;
        delete pDemo;
    }
    pFirstDemo = NULL;
    pCurrentDemo = NULL;
    at59ef = 0;
    m_bLegacy = false;
}

bool CDemo::Create(const char *pzFile)
{
    char buffer[BMAX_PATH];
    char vc = 0;
    if (at0 || at1)
        ThrowError("CDemo::Create called during demo record/playback process.");
    if (!pzFile)
    {
        for (int i = 0; i < 8 && !vc; i++)
        {
            G_ModDirSnprintf(buffer, BMAX_PATH, "%s0%02d.dem", BloodIniPre, i);
            if (access(buffer, F_OK) != -1)
                vc = 1;
        }
        if (vc == 1)
        {
            hRFile = fopen(buffer, "wb");
            if (hRFile == NULL)
                return false;
        }
    }
    else
    {
        G_ModDirSnprintfLite(buffer, BMAX_PATH, pzFile);
        hRFile = fopen(buffer, "wb");
        if (hRFile == NULL)
            return false;
    }
    at0 = 1;
    atb = 0;
    return true;
}

void CDemo::Write(GINPUT *pPlayerInputs)
{
    dassert(pPlayerInputs != NULL);
    if (!at0)
        return;
    if (atb == 0)
    {
        atf.signature = 0x1a4d4445; // '\x1aMDE';
        atf.nVersion = BYTEVERSION;
        atf.nBuild = nBuild;
        atf.nInputCount = 0;
        atf.nNetPlayers = gNetPlayers;
        atf.nMyConnectIndex = myconnectindex;
        atf.nConnectHead = connecthead;
        memcpy(atf.connectPoints, connectpoint2, sizeof(atf.connectPoints));
        memcpy(&m_gameOptions, &gGameOptions, sizeof(gGameOptions));
        fwrite(&atf, sizeof(DEMOHEADER), 1, hRFile);
        fwrite(&m_gameOptions, sizeof(GAMEOPTIONS), 1, hRFile);
    }
    for (int p = connecthead; p >= 0; p = connectpoint2[p])
    {
        memcpy(&at1aa[atb&1023], &pPlayerInputs[p], sizeof(GINPUT));
        atb++;
        if((atb&(kInputBufferSize-1))==0)
            FlushInput(kInputBufferSize);
    }
}

void CDemo::Close(void)
{
    if (at0)
    {
        if (atb&(kInputBufferSize-1))
            FlushInput(atb&(kInputBufferSize-1));
        atf.nInputCount = atb;
        fseek(hRFile, 0, SEEK_SET);
        fwrite(&atf, sizeof(DEMOHEADER), 1, hRFile);
        fwrite(&m_gameOptions, sizeof(GAMEOPTIONS), 1, hRFile);
    }
    if (hPFile >= 0)
    {
        kclose(hPFile);
        hPFile = -1;
    }
    if (hRFile != NULL)
    {
        fclose(hRFile);
        hRFile = NULL;
    }
    at0 = 0;
    at1 = 0;
}

bool CDemo::SetupPlayback(const char *pzFile)
{
    at0 = 0;
    at1 = 0;
    if (pzFile)
    {
        hPFile = kopen4loadfrommod(pzFile, 0);
        if (hPFile == -1)
            return false;
    }
    else
    {
        if (!pCurrentDemo)
            return false;
        hPFile = kopen4loadfrommod(pCurrentDemo->zName, 0);
        if (hPFile == -1)
            return false;
    }
    kread(hPFile, &atf, sizeof(DEMOHEADER));
    // if (atf.signature != '\x1aMED' && atf.signature != '\x1aMDE')
    if (atf.signature != 0x1a4d4544 && atf.signature != 0x1a4d4445)
        return 0;
    m_bLegacy = atf.signature == 0x1a4d4544;
    if (m_bLegacy)
    {
        GAMEOPTIONSLEGACY gameOptions;
        if (BloodVersion != atf.nVersion)
            return 0;
        kread(hPFile, &gameOptions, sizeof(GAMEOPTIONSLEGACY));
        ReadGameOptionsLegacy(m_gameOptions, gameOptions);
    }
    else
    {
        if (BYTEVERSION != atf.nVersion)
            return 0;
        kread(hPFile, &m_gameOptions, sizeof(GAMEOPTIONS));
    }
    at0 = 0;
    at1 = 1;
    return 1;
}

void CDemo::ProcessKeys(void)
{
    switch (gInputMode)
    {
    case INPUT_MODE_1:
        gGameMenuMgr.Process();
        break;
    case INPUT_MODE_2:
        gPlayerMsg.ProcessKeys();
        break;
    case INPUT_MODE_0:
    {
        char nKey;
        while ((nKey = keyGetScan()) != 0)
        {
	        char UNUSED(alt) = keystatus[0x38] | keystatus[0xb8];
	        char UNUSED(ctrl) = keystatus[0x1d] | keystatus[0x9d];
            switch (nKey)
            {
            case 1:
                if (!CGameMenuMgr::m_bActive)
                {
                    gGameMenuMgr.Push(&menuMain, -1);
                    at2 = 1;
                }
                break;
            case 0x58:
                gViewIndex = connectpoint2[gViewIndex];
                if (gViewIndex == -1)
                    gViewIndex = connecthead;
                gView = &gPlayer[gViewIndex];
                break;
            }
        }
        break;
    default:
        gInputMode = INPUT_MODE_0;
        break;
    }
    }
}

void CDemo::Playback(void)
{
    CONTROL_BindsEnabled = false;
    ready2send = 0;
    int v4 = 0;
    if (!CGameMenuMgr::m_bActive)
    {
        gGameMenuMgr.Push(&menuMain, -1);
        at2 = 1;
    }
    gNetFifoClock = gGameClock;
    gViewMode = 3;
_DEMOPLAYBACK:
    while (at1 && !gQuitGame)
    {
        while (gGameClock >= gNetFifoClock && !gQuitGame)
        {
            if (!v4)
            {
                viewResizeView(gViewSize);
                viewSetMessage("");
                gNetPlayers = atf.nNetPlayers;
                atb = atf.nInputCount;
                myconnectindex = atf.nMyConnectIndex;
                connecthead = atf.nConnectHead;
                for (int i = 0; i < 8; i++)
                    connectpoint2[i] = atf.connectPoints[i];
                memset(gNetFifoHead, 0, sizeof(gNetFifoHead));
                gNetFifoTail = 0;
                //memcpy(connectpoint2, atf.connectPoints, sizeof(atf.connectPoints));
                memcpy(&gGameOptions, &m_gameOptions, sizeof(GAMEOPTIONS));
                gSkill = gGameOptions.nDifficulty;
                for (int i = 0; i < 8; i++)
                    playerInit(i, 0);
                StartLevel(&gGameOptions);
                for (int i = 0; i < 8; i++)
                {
                    gProfile[i].nAutoAim = 1;
                    gProfile[i].nWeaponSwitch = 1;
                }
            }
            ready2send = 0;
            OSD_DispatchQueued();
            if (!gDemo.at1)
                break;
            ProcessKeys();
            for (int p = connecthead; p >= 0; p = connectpoint2[p])
            {
                if ((v4&1023) == 0)
                {
                    unsigned int nSize = atb-v4;
                    if (nSize > kInputBufferSize)
                        nSize = kInputBufferSize;
                    ReadInput(nSize);
                }
                memcpy(&gFifoInput[gNetFifoHead[p]&255], &at1aa[v4&1023], sizeof(GINPUT));
                gNetFifoHead[p]++;
                v4++;
                if (v4 >= atf.nInputCount)
                {
                    ready2send = 0;
                    if (at59ef != 1)
                    {
                        v4 = 0;
                        Close();
                        NextDemo();
                        gNetFifoClock = gGameClock;
                        goto _DEMOPLAYBACK;
                    }
                    else
                    {
                        int const nOffset = sizeof(DEMOHEADER)+(m_bLegacy ? sizeof(GAMEOPTIONSLEGACY) : sizeof(GAMEOPTIONS));
                        klseek(hPFile, nOffset, SEEK_SET);
                        v4 = 0;
                    }
                }
            }
            gNetFifoClock += 4;
            if (!gQuitGame)
                ProcessFrame();
            ready2send = 0;
        }
        if (viewFPSLimit())
        {
            viewDrawScreen();
            if (gInputMode == INPUT_MODE_1 && CGameMenuMgr::m_bActive)
                gGameMenuMgr.Draw();
        }
        if (TestBitString(gotpic, 2342))
        {
            FireProcess();
            ClearBitString(gotpic, 2342);
        }
    }
    Close();
}

void CDemo::StopPlayback(void)
{
    at1 = 0;
}

void CDemo::LoadDemoInfo(void)
{
	auto pDemo = &pFirstDemo;
    const int opsm = pathsearchmode;
    at59ef = 0;
    pathsearchmode = 0;
    char zFN[BMAX_PATH];
    Bsnprintf(zFN, BMAX_PATH, "%s*.dem", BloodIniPre);
    auto pList = klistpath("/", zFN, CACHE1D_FIND_FILE);
    auto pIterator = pList;
    while (pIterator != NULL)
    {
        int hFile = kopen4loadfrommod(pIterator->name, 0);
        if (hFile == -1)
            ThrowError("Error loading demo file header.");
        kread(hFile, &atf, sizeof(atf));
        kclose(hFile);
        if ((atf.signature == 0x1a4d4544 /* '\x1aMED' */&& atf.nVersion == BloodVersion)
            || (atf.signature == 0x1a4d4445 /* '\x1aMDE' */ && atf.nVersion == BYTEVERSION))
        {
            *pDemo = new DEMOCHAIN;
            (*pDemo)->pNext = NULL;
            Bstrncpy((*pDemo)->zName, pIterator->name, BMAX_PATH);
            at59ef++;
			pDemo = &(*pDemo)->pNext;
        }
        pIterator = pIterator->next;
    }
    klistfree(pList);
    pathsearchmode = opsm;
	pCurrentDemo = pFirstDemo;
}

void CDemo::NextDemo(void)
{
    pCurrentDemo = pCurrentDemo->pNext ? pCurrentDemo->pNext : pFirstDemo;
    SetupPlayback(NULL);
}

const int nInputSize = 17;
const int nInputSizeLegacy = 22;

void CDemo::FlushInput(int nCount)
{
    char pBuffer[nInputSize*kInputBufferSize];
    BitWriter bitWriter(pBuffer, sizeof(pBuffer));
    for (int i = 0; i < nCount; i++)
    {
        GINPUT *pInput = &at1aa[i];
        bitWriter.writeBit(pInput->syncFlags.buttonChange);
        bitWriter.writeBit(pInput->syncFlags.keyChange);
        bitWriter.writeBit(pInput->syncFlags.useChange);
        bitWriter.writeBit(pInput->syncFlags.weaponChange);
        bitWriter.writeBit(pInput->syncFlags.mlookChange);
        bitWriter.writeBit(pInput->syncFlags.run);
        bitWriter.write(pInput->forward, 16);
        bitWriter.write(pInput->q16turn, 32);
        bitWriter.write(pInput->strafe, 16);
        bitWriter.writeBit(pInput->buttonFlags.jump);
        bitWriter.writeBit(pInput->buttonFlags.crouch);
        bitWriter.writeBit(pInput->buttonFlags.shoot);
        bitWriter.writeBit(pInput->buttonFlags.shoot2);
        bitWriter.writeBit(pInput->buttonFlags.lookUp);
        bitWriter.writeBit(pInput->buttonFlags.lookDown);
        bitWriter.writeBit(pInput->keyFlags.action);
        bitWriter.writeBit(pInput->keyFlags.jab);
        bitWriter.writeBit(pInput->keyFlags.prevItem);
        bitWriter.writeBit(pInput->keyFlags.nextItem);
        bitWriter.writeBit(pInput->keyFlags.useItem);
        bitWriter.writeBit(pInput->keyFlags.prevWeapon);
        bitWriter.writeBit(pInput->keyFlags.nextWeapon);
        bitWriter.writeBit(pInput->keyFlags.holsterWeapon);
        bitWriter.writeBit(pInput->keyFlags.lookCenter);
        bitWriter.writeBit(pInput->keyFlags.lookLeft);
        bitWriter.writeBit(pInput->keyFlags.lookRight);
        bitWriter.writeBit(pInput->keyFlags.spin180);
        bitWriter.writeBit(pInput->keyFlags.pause);
        bitWriter.writeBit(pInput->keyFlags.quit);
        bitWriter.writeBit(pInput->keyFlags.restart);
        bitWriter.writeBit(pInput->useFlags.useBeastVision);
        bitWriter.writeBit(pInput->useFlags.useCrystalBall);
        bitWriter.writeBit(pInput->useFlags.useJumpBoots);
        bitWriter.writeBit(pInput->useFlags.useMedKit);
        bitWriter.write(pInput->newWeapon, 8);
        bitWriter.write(pInput->q16mlook, 32);
        bitWriter.skipBits(1);
    }
    fwrite(pBuffer, 1, nInputSize*nCount, hRFile);
}

void CDemo::ReadInput(int nCount)
{
    if (m_bLegacy)
    {
        char pBuffer[nInputSizeLegacy*kInputBufferSize];
        kread(hPFile, pBuffer, nInputSizeLegacy*nCount);
        BitReader bitReader(pBuffer, sizeof(pBuffer));
        memset(at1aa, 0, nCount * sizeof(GINPUT));
        for (int i = 0; i < nCount; i++)
        {
            GINPUT *pInput = &at1aa[i];
            pInput->syncFlags.buttonChange = bitReader.readBit();
            pInput->syncFlags.keyChange = bitReader.readBit();
            pInput->syncFlags.useChange = bitReader.readBit();
            pInput->syncFlags.weaponChange = bitReader.readBit();
            pInput->syncFlags.mlookChange = bitReader.readBit();
            pInput->syncFlags.run = bitReader.readBit();
            bitReader.skipBits(26);
            pInput->forward = bitReader.readSigned(8) << 8;
            pInput->q16turn = fix16_from_int(bitReader.readSigned(16) >> 2);
            pInput->strafe = bitReader.readSigned(8) << 8;
            pInput->buttonFlags.jump = bitReader.readBit();
            pInput->buttonFlags.crouch = bitReader.readBit();
            pInput->buttonFlags.shoot = bitReader.readBit();
            pInput->buttonFlags.shoot2 = bitReader.readBit();
            pInput->buttonFlags.lookUp = bitReader.readBit();
            pInput->buttonFlags.lookDown = bitReader.readBit();
            bitReader.skipBits(26);
            pInput->keyFlags.action = bitReader.readBit();
            pInput->keyFlags.jab = bitReader.readBit();
            pInput->keyFlags.prevItem = bitReader.readBit();
            pInput->keyFlags.nextItem = bitReader.readBit();
            pInput->keyFlags.useItem = bitReader.readBit();
            pInput->keyFlags.prevWeapon = bitReader.readBit();
            pInput->keyFlags.nextWeapon = bitReader.readBit();
            pInput->keyFlags.holsterWeapon = bitReader.readBit();
            pInput->keyFlags.lookCenter = bitReader.readBit();
            pInput->keyFlags.lookLeft = bitReader.readBit();
            pInput->keyFlags.lookRight = bitReader.readBit();
            pInput->keyFlags.spin180 = bitReader.readBit();
            pInput->keyFlags.pause = bitReader.readBit();
            pInput->keyFlags.quit = bitReader.readBit();
            pInput->keyFlags.restart = bitReader.readBit();
            bitReader.skipBits(17);
            pInput->useFlags.useBeastVision = bitReader.readBit();
            pInput->useFlags.useCrystalBall = bitReader.readBit();
            pInput->useFlags.useJumpBoots = bitReader.readBit();
            pInput->useFlags.useMedKit = bitReader.readBit();
            bitReader.skipBits(28);
            pInput->newWeapon = bitReader.readUnsigned(8);
            int mlook = bitReader.readSigned(8);
            pInput->q16mlook = fix16_from_int(mlook / 4);
        }
    }
    else
    {
        char pBuffer[nInputSize*kInputBufferSize];
        kread(hPFile, pBuffer, nInputSize*nCount);
        BitReader bitReader(pBuffer, sizeof(pBuffer));
        memset(at1aa, 0, nCount * sizeof(GINPUT));
        for (int i = 0; i < nCount; i++)
        {
            GINPUT *pInput = &at1aa[i];
            pInput->syncFlags.buttonChange = bitReader.readBit();
            pInput->syncFlags.keyChange = bitReader.readBit();
            pInput->syncFlags.useChange = bitReader.readBit();
            pInput->syncFlags.weaponChange = bitReader.readBit();
            pInput->syncFlags.mlookChange = bitReader.readBit();
            pInput->syncFlags.run = bitReader.readBit();
            pInput->forward = bitReader.readSigned(16);
            pInput->q16turn = bitReader.readSigned(32);
            pInput->strafe = bitReader.readSigned(16);
            pInput->buttonFlags.jump = bitReader.readBit();
            pInput->buttonFlags.crouch = bitReader.readBit();
            pInput->buttonFlags.shoot = bitReader.readBit();
            pInput->buttonFlags.shoot2 = bitReader.readBit();
            pInput->buttonFlags.lookUp = bitReader.readBit();
            pInput->buttonFlags.lookDown = bitReader.readBit();
            pInput->keyFlags.action = bitReader.readBit();
            pInput->keyFlags.jab = bitReader.readBit();
            pInput->keyFlags.prevItem = bitReader.readBit();
            pInput->keyFlags.nextItem = bitReader.readBit();
            pInput->keyFlags.useItem = bitReader.readBit();
            pInput->keyFlags.prevWeapon = bitReader.readBit();
            pInput->keyFlags.nextWeapon = bitReader.readBit();
            pInput->keyFlags.holsterWeapon = bitReader.readBit();
            pInput->keyFlags.lookCenter = bitReader.readBit();
            pInput->keyFlags.lookLeft = bitReader.readBit();
            pInput->keyFlags.lookRight = bitReader.readBit();
            pInput->keyFlags.spin180 = bitReader.readBit();
            pInput->keyFlags.pause = bitReader.readBit();
            pInput->keyFlags.quit = bitReader.readBit();
            pInput->keyFlags.restart = bitReader.readBit();
            pInput->useFlags.useBeastVision = bitReader.readBit();
            pInput->useFlags.useCrystalBall = bitReader.readBit();
            pInput->useFlags.useJumpBoots = bitReader.readBit();
            pInput->useFlags.useMedKit = bitReader.readBit();
            pInput->newWeapon = bitReader.readUnsigned(8);
            pInput->q16mlook = bitReader.readSigned(32);
            bitReader.skipBits(1);
        }
    }
}
