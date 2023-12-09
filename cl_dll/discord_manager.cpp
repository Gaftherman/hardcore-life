// ================================================== \\
// Discord RPC implemenatation
// Based on code from the Valve Dev Community wiki
// 
// Jay - 2022
// ================================================== \\

#include "hud.h"
#include "discord_manager.h"
#include <string.h>
#include "discord_rpc.h"
#include <time.h>
#include<string> 

static DiscordRichPresence discordPresence;
extern cl_enginefunc_t gEngfuncs;

// Blank handlers; not required for singleplayer Half-Life
static void HandleDiscordReady(const DiscordUser* connectedUser) {}
static void HandleDiscordDisconnected(int errcode, const char* message) {}
static void HandleDiscordError(int errcode, const char* message) {}
static void HandleDiscordJoin(const char* secret) {}
static void HandleDiscordSpectate(const char* secret) {}
static void HandleDiscordJoinRequest(const DiscordUser* request) {}

// Default logo to use as a fallback
const char* defaultLogo = "logo";

void DiscordMan_Startup(void)
{
	int64_t startTime = time(0);

	DiscordEventHandlers handlers;
	memset(&handlers, 0, sizeof(handlers));

	handlers.ready = HandleDiscordReady;
	handlers.disconnected = HandleDiscordDisconnected;
	handlers.errored = HandleDiscordError;
	handlers.joinGame = HandleDiscordJoin;
	handlers.spectateGame = HandleDiscordSpectate;
	handlers.joinRequest = HandleDiscordJoinRequest;

	Discord_Initialize("1183037788313112576", &handlers, 1, 0);

	memset(&discordPresence, 0, sizeof(discordPresence));

	discordPresence.startTimestamp = startTime;
	discordPresence.largeImageKey = defaultLogo;
	Discord_UpdatePresence(&discordPresence);
}

void DiscordMan_Update(void)
{
	char curArea[64];	// If the CVar is empty, use the map file name
	snprintf(curArea, sizeof(curArea) - 1, strncmp(gEngfuncs.pfnGetCvarString("rpc_chapter"), "", sizeof(curArea)) ? gEngfuncs.pfnGetCvarString("rpc_chapter") : gEngfuncs.pfnGetLevelName());
	char curImage[16];	// If the CVar is empty, use the default logo
	snprintf(curImage, sizeof(curImage) - 1, /*strncmp(gEngfuncs.pfnGetCvarString("rpc_image"), "", sizeof(curImage)) ? gEngfuncs.pfnGetCvarString("rpc_image") : */defaultLogo);

	// This looks stupid, sorry if it is, i'm trying to learn C x[
	std::string m_iszSkill = gEngfuncs.pfnGetCvarString("rpc_skill");

	if( m_iszSkill != "" )
	{
		m_iszSkill = std::string( "Difficulty: " ) + m_iszSkill;
	}

	discordPresence.details = m_iszSkill.c_str();
	discordPresence.state = curArea;
	discordPresence.largeImageKey = curImage;
	discordPresence.button1Label = "Download MOD";
	discordPresence.button1Url = "https://github.com/Mikk155/hardcore-life";
	discordPresence.button2Label = "Download SDK";
	discordPresence.button2Url = "https://github.com/Mikk155/hardcore-life";

	Discord_UpdatePresence(&discordPresence);
}

void DiscordMan_Kill(void)
{
	Discord_Shutdown();
}
