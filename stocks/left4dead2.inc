#if defined _stocks_left4dead2_included
 #endinput
#endif
#define _stocks_left4dead2_included

#include <halflife>
#include <stocks/player>
#include <stocks/halflife>

#define TEAM_SURVIVOR			2
#define	TEAM_INFECTED			3
#define TEAM_SCRIPTEDAI			4 // Left 4 Dead 1 bots

#define GHOSTFLAG_SPAWN_DISABLED	(1 << 0)
#define GHOSTFLAG_WAIT_FOR_SAFE_AREA	(1 << 1)
#define GHOSTFLAG_WAIT_FOR_FINALE	(1 << 2)
#define GHOSTFLAG_WAIT_FOR_TANK		(1 << 3)
#define GHOSTFLAG_SURVIVOR_ESCAPED	(1 << 4)
#define GHOSTFLAG_DIRECTOR_TIMEOUT	(1 << 5)
#define GHOSTFLAG_WAIT_FOR_STAMPEDE	(1 << 6)
#define GHOSTFLAG_IN_SIGHT		(1 << 7)
#define GHOSTFLAG_TOO_CLOSE		(1 << 8)
#define GHOSTFLAG_RESTRICTED_AREA	(1 << 9)
#define GHOSTFLAG_INSIDE_ENTITY		(1 << 10)

enum ZombieClass
{
	Zombie_Common = 0,
	Zombie_Smoker,
	Zombie_Boomer,
	Zombie_Hunter,
	Zombie_Spitter,
	Zombie_Jockey,
	Zombie_Charger,
	Zombie_Witch,
	Zombie_Tank,
	Zombie_Survivor,
};

static int m_zombieClass = -2;
stock ZombieClass GetZombieClass(int client)
{
	Assert(IsClient(client));
	
	DeclareNetPropOf(client, m_zombieClass);
	
	return view_as<ZombieClass>(GetEntData(client, m_zombieClass, 1));
}

#define UPGRADEFLAG_NONE 			0
#define UPGRADEFLAG_INCENDIARY 	0x1
#define UPGRADEFLAG_EXPLOSIVE 		0x2
#define UPGRADEFLAG_LASER			0x4

static int m_upgradeBitVec = -2;
/**
 * Gets a weapon's current upgrade bit
 *
 * @param weapon		weapon index.
 * @return				Current weapon upgrade bit flags.
 * @error				Invalid weapon.
 */
stock int GetWeaponUpgradeBits(int weapon)
{
	Assert(IsValidEdict(weapon));
	
	DeclareNetPropOf(weapon, m_upgradeBitVec);
	
	return GetEntData(weapon, m_upgradeBitVec);
}

/**
 * Sets a weapon's current upgrade bit
 *
 * @param entity		weapon index.
 * @noreturn
 * @error				Invalid weapon.
 */
stock void SetWeaponUpgradeBits(int weapon, int flags)
{
	Assert(IsValidEdict(weapon));
	
	DeclareNetPropOf(weapon, m_upgradeBitVec);
	
	SetEntData(weapon, m_upgradeBitVec, flags, 4, true);
}

static int m_nUpgradedPrimaryAmmoLoaded = -2;
/**
 * Gets a weapon's loaded upgrade ammo
 *
 * @param entity		weapon index.
 * @return				Currently loaded upgrade ammo in weapon
 * @error				Invalid weapon.
 */
stock int GetLoadedUpgradeAmmo(int weapon)
{
	Assert(IsValidEdict(weapon));
	
	DeclareNetPropOf(weapon, m_nUpgradedPrimaryAmmoLoaded);
	
	return GetEntData(weapon, m_nUpgradedPrimaryAmmoLoaded, 1);
}

/**
 * Sets a weapon's loaded upgrade ammo.
 *
 * @param entity		weapon index.
 * @noreturn
 * @error				Invalid weapon.
 */
stock void SetLoadedUpgradeAmmo(int weapon, int amount)
{
	Assert(IsValidEdict(weapon));
	
	DeclareNetPropOf(weapon, m_nUpgradedPrimaryAmmoLoaded);
	
	SetEntData(weapon, m_nUpgradedPrimaryAmmoLoaded, amount, 1, true);
}

enum BackpackItemActionType
{
	UseAction_None = 0,
	UseAction_Healing = 1,
	UseAction_RevivedGetUp = 5,
	UseAction_UseTimedButton = 10,
	UseAction_UsePointScript = 11,
};

static int m_iCurrentUseAction = -2;
stock BackpackItemActionType GetUseAction(int client)
{
	Assert(IsClient(client));
	
	DeclareNetPropOf(client, m_iCurrentUseAction);
	
	return view_as<BackpackItemActionType>(GetEntData(client, m_iCurrentUseAction, 1));
}

stock void SetUseAction(int client, BackpackItemActionType action = UseAction_None)
{
	Assert(IsClient(client));
	
	DeclareNetPropOf(client, m_iCurrentUseAction);
	
	SetEntData(client, m_iCurrentUseAction, view_as<int>(action), 1);
}

static int m_useActionOwner = -2;
stock int GetUseActionOwner(int client)
{
	Assert(IsClient(client));
	
	DeclareNetPropOf(client, m_useActionOwner);
	
	return GetEntDataEnt2(client, m_useActionOwner);
}

stock void SetUseActionOwner(int client, int owner = NULL_ENTITY)
{
	Assert(IsClient(client));
	
	DeclareNetPropOf(client, m_useActionOwner);
	
	return SetEntDataEnt2(client, m_useActionOwner, owner, true);
}

static int m_useActionTarget = -2;
stock int GetUseActionTarget(int client)
{
	Assert(IsClient(client));
	
	DeclareNetPropOf(client, m_useActionTarget);
	
	return GetEntDataEnt2(client, m_useActionTarget);
}

stock void SetUseActionTarget(int client, int target = NULL_ENTITY)
{
	Assert(IsClient(client));
	
	DeclareNetPropOf(client, m_useActionTarget);
	
	return SetEntDataEnt2(client, m_useActionTarget, target, true);
}

static int m_itTimer = -2;
stock bool IsPlayerIT(int client)
{
	Assert(IsClient(client));
	
	DeclareNetPropOf(client, m_itTimer);
	
	return GetEntDataFloat(client, m_itTimer + 8) != -1.0;
}

static int m_tongueOwner = -2;
stock int GetSurvivorSmokerAttacker(int client)
{
	Assert(IsClient(client));
	
	DeclareNetPropOf(client, m_tongueOwner);
	
	return GetEntDataEnt2(client, m_tongueOwner);
}

static int m_pounceAttacker = -2;
stock int GetSurvivorHunterAttacker(int client)
{
	Assert(IsClient(client));
	
	DeclareNetPropOf(client, m_pounceAttacker);
	
	return GetEntDataEnt2(client, m_pounceAttacker);
}

static int m_jockeyAttacker = -2;
stock int GetSurvivorJockeyAttacker(int client)
{
	Assert(IsClient(client));
	
	DeclareNetPropOf(client, m_jockeyAttacker);
	
	return GetEntDataEnt2(client, m_jockeyAttacker);
}

static int m_carryAttacker = -2;
stock int GetSurvivorCarryAttacker(int client)
{
	Assert(IsClient(client));
	
	DeclareNetPropOf(client, m_carryAttacker);
	
	return GetEntDataEnt2(client, m_carryAttacker);
}

static int m_pummelAttacker = -2;
stock int GetSurvivorPummelAttacker(int client)
{
	Assert(IsClient(client));
	
	DeclareNetPropOf(client, m_pummelAttacker);
	
	return GetEntDataEnt2(client, m_pummelAttacker);	
}

stock bool HasSurvivorAnyAttacker(int client)
{
	return (GetSurvivorSmokerAttacker(client) > 0 || GetSurvivorHunterAttacker(client) > 0 || GetSurvivorJockeyAttacker(client) > 0 ||
		GetSurvivorCarryAttacker(client) > 0 || GetSurvivorPummelAttacker(client) > 0);
}

static int m_isGhost = -2;
stock bool IsPlayerGhost(int client)
{
	Assert(IsClient(client));
	
	DeclareNetPropOf(client, m_isGhost);
	
	return !!GetEntData(client, m_isGhost, 1);	
}

static int m_staggerTimer = -2;
stock bool IsPlayerStaggering(int client)
{
	Assert(IsClient(client));
	
	DeclareNetPropOf(client, m_staggerTimer);
	
	return GetEntDataFloat(client, m_staggerTimer + 8) != -1;	
}

static int m_isIncapacitated = -2;
stock bool IsPlayerIncapacitated(int client)
{
	Assert(IsClient(client));
	
	DeclareNetPropOf(client, m_isIncapacitated);
	
	return !!GetEntData(client, m_isIncapacitated, 1);
}

static int m_pendingTankPlayerIndex = -2;
stock int GetPendingTankPlayerIndex()
{
	int entity = FindEntityByClassname(-1, "terror_player_manager");
	if (entity == -1) {
		return entity;
	}
	
	DeclareNetPropOf(entity, m_pendingTankPlayerIndex);
	
	return GetEntData(entity, m_pendingTankPlayerIndex);
}

// m_humanSpectatorEntIndex ?
static int m_humanSpectatorUserID = -2;
stock int GetHumanSpectator(int bot)
{
	Assert(IsClient(bot));
	
	DeclareNetPropOf(bot, m_humanSpectatorUserID);
	
	return GetClientOfUserId(GetEntData(bot, m_humanSpectatorUserID));
}

stock int FindAvailableSurvivorBot()
{
	for (int i = 1; i <= MaxClients; i++) {
		if (IsClientBotAndAlive(i) && GetClientTeam(i) == TEAM_SURVIVOR && !GetHumanSpectator(i)) {
			return i;
		}
	}
	
	return 0;
}

static int m_hasVisibleThreats = -2;
stock bool HasPlayerVisibleThreats(int client) 
{
    Assert(IsClient(client));

    DeclareNetPropOf(client, m_hasVisibleThreats); 

    return !!GetEntData(client, m_hasVisibleThreats, 1);
}

static int m_frustration = -2;
stock int GetPlayerFrustration(int client)
{
	Assert(IsClient(client));
	
	DeclareNetPropOf(client, m_frustration);
	
	return GetEntData(client, m_frustration, 1);
}

static int m_customAbility = -2;
stock int GetPlayerCustomAbility(int client)
{
	Assert(IsClient(client));
	
	DeclareNetPropOf(client, m_customAbility);
	
	return GetEntDataEnt2(client, m_customAbility);
}

static int m_stunTimer = -2;
stock bool IsPlayerStunned(int client)
{
	Assert(IsClient(client));
	
	DeclareNetPropOf(client, m_stunTimer);
	
	return GetEntDataFloat(client, m_stunTimer + 8) != -1;
}

static int m_ghostSpawnState = -2;
stock int GetPlayerGhostSpawnState(int client)
{
	Assert(IsClient(client));
	
	DeclareNetPropOf(client, m_ghostSpawnState);
	
	return GetEntData(client, m_ghostSpawnState);
}
