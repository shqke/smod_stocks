#if defined _stocks_player_included
 #endinput
#endif
#define _stocks_player_included

#include <stocks/assert>
#include <stocks/property>

// unsafe>
#define IsClientEntity(%0) ( %0 >= 1 && %0 <= MaxClients )
#define IsClient(%0) ( IsClientEntity(%0) && IsClientInGame(%0) )
#define IsClientAndAlive(%0) ( IsClient(%0) && IsPlayerAlive(%0) )
#define IsClientAndConnected(%0) ( IsClientEntity(%0) && IsClientConnected(%0) )
#define IsClientAndAuthorized(%0) ( IsClientEntity(%0) && IsClientAuthorized(%0) )
#define IsClientBot(%0) ( IsClient(%0) && IsFakeClient(%0) )
#define IsClientBotAndAlive(%0) ( IsClientBot(%0) && IsPlayerAlive(%0) )

#define LoopThroughPlayers(%0) LoopThroughClients(%0)\
	if ( !IsClientInGame(%0) ) {}\
	else

#define LoopThroughClients(%0) for (int %0 = MaxClients; %0; %0--)

stock int GetCurrentClients()
{
	int count = 0;
	
	LoopThroughClients(client) {
		if (IsClientAndConnected(client)/* && !IsFakeClient(client)*/) {
			count ++;
		}
	}
	
	return count;
}

stock bool IsValidClient( int client )
{
	Assert(IsClientEntity(client));
	
	return IsClientConnected(client);
}

static int m_hViewEntity = -2;
stock int GetClientViewEntity( int client )
{
	Assert(IsClient(client));
	
	DeclarePropOf(client, m_hViewEntity);
	
	return GetEntDataEnt2(client, m_hViewEntity);
}

stock void SetClientViewEntity( int client, int entity )
{
	Assert(IsClient(client));
	
	DeclarePropOf(client, m_hViewEntity);
	
	SetEntDataEnt2(client, m_hViewEntity, entity, true);
}

stock void MoveToPlayer( int client, int target )
{
	Assert(IsClient(client));
	Assert(IsClient(target));
	
	DeclareLocalNetProp(CBasePlayer, m_bDucking);
	DeclareLocalNetProp(CBasePlayer, m_bDucked);
	
	static float origin[3], angles[3], velocity[3];
	GetClientAbsOrigin(target, origin);
	GetClientAbsAngles(target, angles);
	GetEntPropVector(target, Prop_Data, "m_vecVelocity", velocity);
	
	SetEntData(client, m_bDucking, GetEntData(target, m_bDucked, 1), 1);
	
	TeleportEntity(client, origin, angles, velocity);
}

stock void SpawnClient( int client )
{
	Assert(!IsClient(client));
	
	DispatchSpawn(client);
	
	// SetEntPropFloat(client, Prop_Data, "m_flDeathTime", GetGameTime() - 200.0);
}

enum {
	OBS_MODE_NONE = 0,	// not in spectator mode
	OBS_MODE_DEATHCAM,	// special mode for death cam animation
	OBS_MODE_FREEZECAM,	// zooms to a target, and freeze-frames on them
	OBS_MODE_FIXED,		// view from a fixed camera position
	OBS_MODE_IN_EYE,	// follow a player in first person view
	OBS_MODE_CHASE,		// follow a player in third person view
	OBS_MODE_POI,		// PASSTIME point of interest - game objective, big fight, anything interesting; added in the middle of the enum due to tons of hard-coded "<ROAMING" enum compares
	OBS_MODE_ROAMING,	// free roaming

	NUM_OBSERVER_MODES,
};

#define LoopThroughObserversOf(%0,%1) LoopThroughClients(%0)\
	if ( !IsClientInGame(%1) || !IsClientObserver(%1) || GetObserverTarget(%1) != %0) {}\
	else

static int m_hObserverTarget = -2;
stock int GetObserverTarget( int client )
{
	Assert(IsClient(client));
	
	DeclarePropOf(client, m_hObserverTarget);
	
	return GetEntDataEnt2(client, m_hObserverTarget);
}

stock void SetObserverTarget( int client, int target )
{
	Assert(IsClient(client));
	
	DeclarePropOf(client, m_hObserverTarget);
	
	SetEntDataEnt2(client, m_hObserverTarget, target, true);
}

static int m_iObserverMode = -2;
stock int GetObserverMode( int client )
{
	Assert(IsClient(client));
	
	DeclarePropOf(client, m_iObserverMode);
	
	return GetEntData(client, m_iObserverMode, 4);
}

stock int SetObserverMode( int client, int mode = OBS_MODE_NONE )
{
	Assert(IsClient(client));
	
	DeclarePropOf(client, m_iObserverMode);
	
	SetEntData(client, m_iObserverMode, mode, 4, true);
}

static int m_hUseEntity = -2;
stock int GetUseEntity( int client )
{
	Assert(IsClient(client));
	
	DeclarePropOf(client, m_hUseEntity);
	
	return GetEntDataEnt2(client, m_hUseEntity);
}

static int m_flFallVelocity = -2;
stock float GetFallSpeed( int client )
{
	Assert(IsClient(client));
	
	DeclarePropOf(client, m_flFallVelocity);
	
	return GetEntDataFloat(client, m_flFallVelocity);
}

static int m_hMyWeapons = -2;
stock void StripWeapons( int client )
{
	Assert(IsClient(client));
	
	DeclarePropOf(client, m_hMyWeapons);
	
	int size = GetEntPropArraySize(client, Prop_Send, "m_hMyWeapons");
	
	for (int i = 0, item = 0; i < size; i ++) {
		if( (item = GetEntDataEnt2(client, i * 4 + m_hMyWeapons)) == INVALID_ENT_REFERENCE ) {
			continue;
		}
		
		RemovePlayerItem(client, item);
		AcceptEntityInput(item, "Kill");
	}
}

static int m_hActiveWeapon = -2;
stock int GetActiveWeapon( int client )
{
	Assert(IsClient(client));
	
	DeclarePropOf(client, m_hActiveWeapon);
	
	return GetEntDataEnt2(client, m_hActiveWeapon);
}

static int m_vecPunchAngle = -2;
stock void GetViewPunchAngle( int client, float[3] vecResult )
{
	Assert(IsClient(client));
	
	DeclarePropOf(client, m_vecPunchAngle);
	
	GetEntDataVector(client, m_vecPunchAngle, vecResult);
}

stock int ClientFromAuthId(int authId)
{
	static char unkAuth[30], clAuth[30];
	int index = MaxClients + 1;
	
	FormatEx(unkAuth, sizeof(unkAuth), "[U:1:%d]", authId);
	
	while (-- index) {
		if (!IsClientAndAuthorized(index) || IsFakeClient(index)) {
			continue;
		}
		
		if (!GetClientAuthId(index, AuthId_Steam3, clAuth, sizeof(clAuth)) || strcmp(clAuth, unkAuth, false)) {
			continue;
		}
		
		return index;
	}
	
	return INVALID_ENT_REFERENCE;
}
