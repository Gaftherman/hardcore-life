/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
//
// nightvision.cpp
//
// implementation of CHudNightvision class
//

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"

#include <string.h>
#include <stdio.h>

DECLARE_MESSAGE(m_Night, NightBat)
DECLARE_MESSAGE(m_Night, Nightvision)

#define BAT_NAME "sprites/%d_Nightvision.spr"

bool CHudNightvision::Init()
{
	m_fFade = 0;
	m_fOn = false;

	HOOK_MESSAGE(Nightvision);
	HOOK_MESSAGE(NightBat);

	m_iFlags |= HUD_ACTIVE;

	gHUD.AddHudElem(this);

	return true;
}

void CHudNightvision::Reset()
{
	m_fFade = 0;
	m_fOn = false;
}

bool CHudNightvision::VidInit()
{
	int HUD_flash_empty = gHUD.GetSpriteIndex("flash_empty");
	int HUD_flash_full = gHUD.GetSpriteIndex("flash_full");
	int HUD_flash_beam = gHUD.GetSpriteIndex("flash_beam");

	m_hNightvision = LoadSprite("sprites/of_nv_b.spr");

	m_hSprite1 = gHUD.GetSprite(HUD_flash_empty);
	m_hSprite2 = gHUD.GetSprite(HUD_flash_full);
	m_hBeam = gHUD.GetSprite(HUD_flash_beam);
	m_prc1 = &gHUD.GetSpriteRect(HUD_flash_empty);
	m_prc2 = &gHUD.GetSpriteRect(HUD_flash_full);
	m_prcBeam = &gHUD.GetSpriteRect(HUD_flash_beam);
	m_iWidth = m_prc2->right - m_prc2->left;

	return true;
}

bool CHudNightvision::MsgFunc_NightBat(const char* pszName, int iSize, void* pbuf)
{
	BEGIN_READ(pbuf, iSize);
	int x = READ_BYTE();
	m_iBat = x;
	m_flBat = ((float)x) / 100.0;

	return true;
}

bool CHudNightvision::MsgFunc_Nightvision(const char* pszName, int iSize, void* pbuf)
{
	BEGIN_READ(pbuf, iSize);
	m_fOn = READ_BYTE() != 0;
	int x = READ_BYTE();
	m_iBat = x;
	m_flBat = ((float)x) / 100.0;

	return true;
}

bool CHudNightvision::Draw(float flTime)
{
	if ((gHUD.m_iHideHUDDisplay & (HIDEHUD_FLASHLIGHT | HIDEHUD_ALL)) != 0)
		return true;

	if (m_fOn)
	{
		static int lastFrame = 0;

		auto frameIndex = rand() % gEngfuncs.pfnSPR_Frames(m_hNightvision);

		if (frameIndex == lastFrame)
			frameIndex = (frameIndex + 1) % gEngfuncs.pfnSPR_Frames(m_hNightvision);

		lastFrame = frameIndex;

		if (0 != m_hNightvision)
		{
			const auto width = gEngfuncs.pfnSPR_Width(m_hNightvision, 0);
			const auto height = gEngfuncs.pfnSPR_Height(m_hNightvision, 0);

			gEngfuncs.pfnSPR_Set(m_hNightvision, 41, 114, 51);

			Rect drawingRect;

			for (auto x = 0; x < gHUD.m_scrinfo.iWidth; x += width)
			{
				drawingRect.left = 0;
				drawingRect.right = x + width >= gHUD.m_scrinfo.iWidth ? gHUD.m_scrinfo.iWidth - x : width;

				for (auto y = 0; y < gHUD.m_scrinfo.iHeight; y += height)
				{
					drawingRect.top = 0;
					drawingRect.bottom = y + height >= gHUD.m_scrinfo.iHeight ? gHUD.m_scrinfo.iHeight - y : height;

					gEngfuncs.pfnSPR_DrawAdditive(frameIndex, x, y, &drawingRect);
				}
			}
		}
	}

	return true;
}