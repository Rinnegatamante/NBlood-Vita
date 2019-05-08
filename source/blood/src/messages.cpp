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
#include "mmulti.h"
#include "compat.h"
#include "keyboard.h"
#include "control.h"
#include "function.h"
#include "common_game.h"
#include "blood.h"
#include "config.h"
#include "demo.h"
#include "eventq.h"
#include "globals.h"
#include "levels.h"
#include "loadsave.h"
#include "menu.h"
#include "messages.h"
#include "network.h"
#include "player.h"
#include "view.h"

CPlayerMsg gPlayerMsg;
CCheatMgr gCheatMgr;

void sub_5A928(void)
{
    for (int i = 0; i < NUMGAMEFUNCTIONS-1; i++)
        CONTROL_ClearButton(i);
}

extern uint8_t KeyboardKeys[NUMGAMEFUNCTIONS][2];
void sub_5A944(char key)
{
    for (int i = 0; i < NUMGAMEFUNCTIONS-1; i++)
    {
        char key1, key2;
        key1 = KeyboardKeys[i][0];
        key2 = KeyboardKeys[i][1];
        if (key1 == key || key2 == key)
            CONTROL_ClearButton(i);
    }
}

void SetGodMode(bool god)
{
    playerSetGodMode(gMe, god);
    if (gMe->at31a)
        viewSetMessage("You are immortal.");
    else
        viewSetMessage("You are mortal.");
}

void SetClipMode(bool noclip)
{
    gNoClip = noclip;
    if (gNoClip)
        viewSetMessage("Unclipped movement.");
    else
        viewSetMessage("Normal movement.");
}

void packStuff(PLAYER *pPlayer)
{
    for (int i = 0; i < 5; i++)
        packAddItem(pPlayer, i);
}

void packClear(PLAYER *pPlayer)
{
    pPlayer->at321 = 0;
    for (int i = 0; i < 5; i++)
    {
        pPlayer->packInfo[i].at0 = 0;
        pPlayer->packInfo[i].at1 = 0;
    }
}

void SetAmmo(bool stat)
{
    if (stat)
    {
        for (int i = 0; i < 12; i++)
            gMe->at181[i] = gAmmoInfo[i].at0;
        viewSetMessage("You have full ammo.");
    }
    else
    {
        for (int i = 0; i < 12; i++)
            gMe->at181[i] = 0;
        viewSetMessage("You have no ammo.");
    }
}

void SetWeapons(bool stat)
{
    for (int i = 0; i < 14; i++)
    {
        gMe->atcb[i] = stat;
    }
    SetAmmo(stat);
    if (stat)
        viewSetMessage("You have all weapons.");
    else
        viewSetMessage("You have no weapons.");
}

void SetToys(bool stat)
{
    if (stat)
    {
        packStuff(gMe);
        viewSetMessage("Your inventory is full.");
    }
    else
    {
        packClear(gMe);
        viewSetMessage("Your inventory is empty.");
    }
}

void SetArmor(bool stat)
{
    int nAmount;
    if (stat)
    {
        viewSetMessage("You have full armor.");
        nAmount = 3200;
    }
    else
    {
        viewSetMessage("You have no armor.");
        nAmount = 0;
    }
    for (int i = 0; i < 3; i++)
        gMe->at33e[i] = nAmount;
}

void SetKeys(bool stat)
{
    for (int i = 1; i <= 6; i++)
        gMe->at88[i] = stat;
    if (stat)
        viewSetMessage("You have all keys.");
    else
        viewSetMessage("You have no keys.");
}

void SetInfiniteAmmo(bool stat)
{
    gInfiniteAmmo = stat;
    if (gInfiniteAmmo)
        viewSetMessage("You have infinite ammo.");
    else
        viewSetMessage("You have limited ammo.");
}

void SetMap(bool stat)
{
    gFullMap = stat;
    if (gFullMap)
        viewSetMessage("You have the map.");
    else
        viewSetMessage("You have no map.");
}

void SetWooMode(bool stat)
{
    if (stat)
    {
        if (!powerupCheck(gMe, 17))
            powerupActivate(gMe, 17);
    }
    else
    {
        if (powerupCheck(gMe, 17))
            powerupDeactivate(gMe, 17);
    }
}

void ToggleWooMode(void)
{
    SetWooMode(!(powerupCheck(gMe, 17) != 0));
}

void ToggleBoots(void)
{
    if (powerupCheck(gMe, 15))
    {
        viewSetMessage("You have no Jumping Boots.");
        powerupDeactivate(gMe, 15);
    }
    else
    {
        viewSetMessage("You have the Jumping Boots.");
        powerupActivate(gMe, 15);
    }
}

void ToggleInvisibility(void)
{
    if (powerupCheck(gMe, 13))
    {
        viewSetMessage("You are visible.");
        powerupDeactivate(gMe, 13);
    }
    else
    {
        viewSetMessage("You are invisible.");
        powerupActivate(gMe, 13);
    }
}

void ToggleInvulnerability(void)
{
    if (powerupCheck(gMe, 14))
    {
        viewSetMessage("You are vulnerable.");
        powerupDeactivate(gMe, 14);
    }
    else
    {
        viewSetMessage("You are invulnerable.");
        powerupActivate(gMe, 14);
    }
}

void ToggleDelirium(void)
{
    if (powerupCheck(gMe, 28))
    {
        viewSetMessage("You are not delirious.");
        powerupDeactivate(gMe, 28);
    }
    else
    {
        viewSetMessage("You are delirious.");
        powerupActivate(gMe, 28);
    }
}

void LevelWarp(int nEpisode, int nLevel)
{
    levelSetupOptions(nEpisode, nLevel);
    StartLevel(&gGameOptions);
    viewResizeView(gViewSize);
}

void LevelWarpAndRecord(int nEpisode, int nLevel)
{
    char buffer[BMAX_PATH];
    levelSetupOptions(nEpisode, nLevel);
    gGameStarted = false;
    strcpy(buffer, levelGetFilename(nEpisode, nLevel));
    ChangeExtension(buffer, ".DEM");
    gDemo.Create(buffer);
    StartLevel(&gGameOptions);
    viewResizeView(gViewSize);
}

CGameMessageMgr::CGameMessageMgr()
{
    at1 = 1;
    at5 = 0;
    at9 = 0;
    atd = 0;
    at11 = 0;
    at15 = 8;
    at19 = 4;
    at1d = 5;
    at21 = 15;
    at22 = 0;
    at2a = at26 = 0;
}

void CGameMessageMgr::SetState(char state)
{
    if (at0 && !state)
    {
        at0 = 0;
        Clear();
    }
    else if (!at0 && state)
        at0 = 1;
}

void CGameMessageMgr::Add(const char *a1, char a2)
{
    if (a2 && at21)
    {
        messageStruct *pMessage = &at2e[at2a];
        strncpy(pMessage->at4, a1, 80);
        pMessage->at4[80] = 0;
        pMessage->at0 = gFrameClock + at1d*120;
        at2a = (at2a+1)%16;
        at22++;
        if (at22 > at19)
        {
            at26 = (at26+1)%16;
            atd = 0;
            at22 = at19;
            at9 = at15;
        }
    }
}

void CGameMessageMgr::Display(void)
{
    if (at22 && at0 && gInputMode != INPUT_MODE_2)
    {
        int v10 = at22;
        int v18 = at26;
        int vc = ClipHigh(v10*8, 48);
        int v14 = gViewMode == 3 ? gViewX0S : 0;
        int v8 = (gViewMode == 3 ? at5 : 0) + at9;
        for (int i = 0; i < v10; i++)
        {
            messageStruct *pMessage = &at2e[(v18+i)%16];
            if (pMessage->at0 < gFrameClock)
            {
                at26 = (at26+1)%16;
                at22--;
                continue;
            }
            viewDrawText(at11, pMessage->at4, v14, v8, vc, 0, 0, false, 256);
            if (gViewMode == 3)
            {
                int height;
                gMenuTextMgr.GetFontInfo(at11, pMessage->at4, &height, NULL);
                if (v14+height > gViewX1S)
                    viewUpdatePages();
            }
            v8 += at15;
            vc = ClipLow(vc-64/v10, -128);
        }
        if (at9)
        {
            at9 = at15*at9/120;
            atd += gFrameTicks;
        }
    }
}

void CGameMessageMgr::Clear(void)
{
    at26 = at2a = at22 = 0;
}

void CGameMessageMgr::SetMaxMessages(int nMessages)
{
    at19 = ClipRange(nMessages, 1, 16);
}

void CGameMessageMgr::SetFont(int nFont)
{
    at11 = nFont;
    at15 = gFont[nFont].ySize;
}

void CGameMessageMgr::SetCoordinates(int x, int y)
{
    at1 = ClipRange(x, 0, gViewX1S);
    at5 = ClipRange(y, 0, gViewY1S);
}

void CGameMessageMgr::SetMessageTime(int nTime)
{
    at1d = ClipRange(nTime, 1, 8);
}

void CGameMessageMgr::SetMessageFlags(unsigned int nFlags)
{
    at21 = nFlags&0xf;
}

void CPlayerMsg::Clear(void)
{
    at4[0] = 0;
    at0 = 0;
}

void CPlayerMsg::Term(void)
{
    Clear();
    gInputMode = INPUT_MODE_0;
}

void CPlayerMsg::Draw(void)
{
    char buffer[44];
    strcpy(buffer, at4);
    if (gGameClock & 16)
        strcat(buffer, "_");
    int x = gViewMode == 3 ? gViewX0S : 0;
    int y = gViewMode == 3 ? gViewY0S : 0;
    if (gViewSize >= 1)
        y += tilesiz[2229].y*((gNetPlayers+3)/4);
    viewDrawText(0, buffer, x+1,y+1, -128, 0, 0, false, 256);
    viewUpdatePages();
}

bool CPlayerMsg::AddChar(char ch)
{
    if (at0 < 40)
    {
        at4[at0++] = ch;
        at4[at0] = 0;
        return true;
    }
    return false;
}

void CPlayerMsg::DelChar(void)
{
    if (at0 > 0)
        at4[--at0] = 0;
}

void CPlayerMsg::Set(const char * pzString)
{
    strncpy(at4, pzString, 40);
    at0 = ClipHigh(strlen(pzString), 40);
    at4[at0] = 0;
}

void CPlayerMsg::Send(void)
{
    netBroadcastMessage(myconnectindex, at4);
    viewSetMessage(at4);
    Term();
    keyFlushScans();
}

void CPlayerMsg::ProcessKeys(void)
{
    int key = keyGetScan();
    char ch;
    if (key != 0)
    {
        bool UNUSED(alt) = keystatus[sc_LeftAlt] || keystatus[sc_RightAlt];
        bool ctrl = keystatus[sc_LeftControl] || keystatus[sc_RightControl];
        bool shift = keystatus[sc_LeftShift] || keystatus[sc_RightShift];
        switch (key)
        {
        case sc_Escape:
            Term();
            break;
        case sc_F1:
        case sc_F2:
        case sc_F3:
        case sc_F4:
        case sc_F5:
        case sc_F6:
        case sc_F7:
        case sc_F8:
        case sc_F9:
        case sc_F10:
            CONTROL_ClearButton(gamefunc_See_Chase_View);
            Set(CommbatMacro[key-sc_F1]);
            Send();
            keystatus[key] = 0;
            break;
        case sc_BackSpace:
            if (ctrl)
                Clear();
            else
                DelChar();
            break;
        case sc_Enter:
        case sc_kpad_Enter:
            if (gCheatMgr.Check(at4))
                Term();
            else
                Send();
            break;
        default:
            if (key < 128)
            {
                ch =  shift ? g_keyAsciiTableShift[key] : g_keyAsciiTable[key];
                if (ch)
                    AddChar(ch);
            }
            break;
        }
        sub_5A944(key);
    }
}

CCheatMgr::CHEATINFO CCheatMgr::s_CheatInfo[] = {
    {"NQLGB", kCheat5, 0 }, // MPKFA
    {"DBQJONZBTT", kCheat6, 0 }, // CAPINMYASS
    {"OPDBQJONZBTT", kCheat7, 0 }, // NOCAPINMYASS
    {"J!XBOOB!CF!MJLF!LFWJO", kCheat7, 0 }, // I WANNA BE LIKE KEVIN
    {"JEBIP", kCheat8, 0 }, // IDAHO
    {"NPOUBOB", kCheat31, 0 }, // MONTANA
    {"HSJTXPME", kCheat2, 0 }, // GRISWOLD
    {"FENBSL", kCheat11, 0 }, // EDMARK
    {"UFRVJMB", kCheat20, 0 }, // TEQUILA
    {"CVO[", kCheat32, 0 }, // BUNZ
    {"GVOLZ!TIPFT", kCheat21, 0 }, // FUNKY SHOES
    {"HBUFLFFQFS", kCheat26, 0 },
    {"LFZNBTUFS", kCheat22, 0 },
    {"KPKP", kCheat25, 0 },
    {"TBUDIFM", kCheat3, 0 },
    {"TQPSL", kCheat15, 0 },
    {"POFSJOH", kCheat23, 0 },
    {"NBSJP", kCheat28, 1 },
    {"DBMHPO", kCheat28, 1 },
    {"LFWPSLJBO", kCheat9, 0 },
    {"NDHFF", kCheat10, 0 },
    {"LSVFHFS", kCheat12, 0 },
    {"DIFFTFIFBE", kCheat19, 0 },
    {"DPVTUFBV", kCheat33, 0 },
    {"WPPSIFFT", kCheat24, 0 },
    {"MBSB!DSPGU", kCheat29, 0 },
    {"IPOHLPOH", kCheat30, 0 },
    {"GSBOLFOTUFJO", kCheat18, 0 },
    {"TUFSOP", kCheat13, 0 },
    {"DMBSJDF", kCheat17, 0 },
    {"GPSL!ZPV", kCheat34, 0 },
    {"MJFCFSNBO", kCheat35, 0 },
    {"FWB!HBMMJ", kCheat4, 0 },
    {"SBUF", kCheat27, 0 },
    {"HPPOJFT", kCheat16, 0 },
    {"TQJFMCFSH", kCheat36, 1 },
};

bool CCheatMgr::m_bPlayerCheated = false;

bool CCheatMgr::Check(char *pzString)
{
    char buffer[80];
    strcpy(buffer, pzString);
    Bstrupr(buffer);
    for (size_t i = 0; i < strlen(pzString); i++)
        buffer[i]++;
    for (int i = 0; i < 36; i++)
    {
        int nCheatLen = strlen(s_CheatInfo[i].pzString);
        if (s_CheatInfo[i].flags & 1)
        {
            if (!strncmp(buffer, s_CheatInfo[i].pzString, nCheatLen))
            {
                Process(s_CheatInfo[i].id, buffer+nCheatLen);
                return true;
            }
        }
        if (!strcmp(buffer, s_CheatInfo[i].pzString))
        {
            Process(s_CheatInfo[i].id, NULL);
            return true;
        }
    }
    return false;
}

int parseArgs(char *pzArgs, int *nArg1, int *nArg2)
{
    if (!nArg1 || !nArg2)
        return -1;
    int nLength = strlen(pzArgs);
    for (int i = 0; i < nLength; i++)
        pzArgs[i]--;
    int stat = sscanf(pzArgs, " %d %d", nArg1, nArg2);
    if (stat == 2 && (*nArg1 == 0 || *nArg2 == 0))
        return -1;
    *nArg1 = ClipRange(*nArg1-1, 0, gEpisodeCount-1);
    *nArg2 = ClipRange(*nArg2-1, 0, gEpisodeInfo[*nArg1].nLevels-1);
    return stat;
}

void CCheatMgr::Process(CCheatMgr::CHEATCODE nCheatCode, char* pzArgs)
{
    dassert(nCheatCode > kCheatNone && nCheatCode < kCheatMax);

    if (gDemo.at0) return;
    if (nCheatCode == kCheat27)
    {
        gShowFps = !gShowFps;
        return;
    }
    if (gGameOptions.nGameType != 0)
        return;
    int nEpisode, nLevel;
    switch (nCheatCode)
    {
    case kCheat36:
        if (parseArgs(pzArgs, &nEpisode, &nLevel) == 2)
            LevelWarpAndRecord(nEpisode, nLevel);
        break;
    case kCheat1:
        SetAmmo(true);
        break;
    case kCheat2:
        SetArmor(true);
        break;
    case kCheat3:
        SetToys(true);
        break;
    case kCheat4:
        SetClipMode(!gNoClip);
        break;
    case kCheat5:
        SetGodMode(!gMe->at31a);
        break;
    case kCheat6:
        SetGodMode(false);
        break;
    case kCheat7:
        SetGodMode(true);
        break;
    case kCheat8:
        SetWeapons(true);
        break;
    case kCheat9:
        actDamageSprite(gMe->at5b, gMe->pSprite, DAMAGE_TYPE_2, 8000);
        viewSetMessage("Kevorkian approves.");
        break;
    case kCheat10:
    {
        if (!gMe->pXSprite->burnTime)
            evPost(gMe->at5b, 3, 0, CALLBACK_ID_0);
        actBurnSprite(actSpriteIdToOwnerId(gMe->at5b), gMe->pXSprite, 2400);
        viewSetMessage("You're fired!");
        break;
    }
    case kCheat11:
        actDamageSprite(gMe->at5b, gMe->pSprite, DAMAGE_TYPE_3, 8000);
        viewSetMessage("Ahhh...those were the days.");
        break;
    case kCheat12:
    {
        actHealDude(gMe->pXSprite, 200, 200);
        gMe->at33e[1] = 200;
        if (!gMe->pXSprite->burnTime)
            evPost(gMe->at5b, 3, 0, CALLBACK_ID_0);
        actBurnSprite(actSpriteIdToOwnerId(gMe->at5b), gMe->pXSprite, 2400);
        viewSetMessage("Flame retardant!");
        break;
    }
    case kCheat13:
        gMe->at36a = 250;
        break;
    case kCheat14:
        gMe->at35a = 360;
        break;
    case kCheat15:
        actHealDude(gMe->pXSprite, 200, 200);
        break;
    case kCheat16:
        SetMap(!gFullMap);
        break;
    case kCheat18:
        gMe->packInfo[0].at1 = 100;
        break;
    case kCheat19:
        gMe->packInfo[1].at1 = 100;
        break;
    case kCheat20:
        ToggleWooMode();
        break;
    case kCheat21:
        ToggleBoots();
        break;
    case kCheat22:
        SetKeys(true);
        break;
    case kCheat23:
        ToggleInvisibility();
        break;
    case kCheat24:
        ToggleInvulnerability();
        break;
    case kCheat25:
        ToggleDelirium();
        break;
    case kCheat27:
        return;
    case kCheat28:
        if (parseArgs(pzArgs, &nEpisode, &nLevel) == 2)
            LevelWarp(nEpisode, nLevel);
        break;
    case kCheat29:
        SetInfiniteAmmo(!gInfiniteAmmo);
        SetWeapons(gInfiniteAmmo);
        break;
    case kCheat30:
        SetWeapons(true);
        SetInfiniteAmmo(true);
        break;
    case kCheat31:
        SetWeapons(true);
        SetToys(true);
        break;
    case kCheat32:
        SetWeapons(true);
        SetWooMode(true);
        break;
    case kCheat33:
        actHealDude(gMe->pXSprite,200,200);
        gMe->packInfo[1].at1 = 100;
        break;
    case kCheat34:
        SetInfiniteAmmo(false);
        SetMap(false);
        SetWeapons(false);
        SetAmmo(false);
        SetArmor(false);
        SetToys(false);
        SetKeys(false);
        SetWooMode(true);
        powerupActivate(gMe, 28);
        gMe->pXSprite->health = 16;
        gMe->atcb[1] = 1;
        gMe->atbd = 0;
        gMe->atbe = 1;
        break;
    default:
        break;
    }
    m_bPlayerCheated = true;
}

void CCheatMgr::sub_5BCF4(void)
{
    m_bPlayerCheated = 0;
    playerSetGodMode(gMe, 0);
    gNoClip = 0;
    packClear(gMe);
    gInfiniteAmmo = 0;
    gFullMap = 0;
}

class MessagesLoadSave : public LoadSave
{
public:
    virtual void Load();
    virtual void Save();
};

void MessagesLoadSave::Load()
{
    Read(&CCheatMgr::m_bPlayerCheated, sizeof(CCheatMgr::m_bPlayerCheated));
}

void MessagesLoadSave::Save()
{
    Write(&CCheatMgr::m_bPlayerCheated, sizeof(CCheatMgr::m_bPlayerCheated));
}

static MessagesLoadSave *myLoadSave;

void MessagesLoadSaveConstruct(void)
{
    myLoadSave = new MessagesLoadSave();
}
