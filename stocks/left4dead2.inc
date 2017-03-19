#if defined _stocks_left4dead2_included
 #endinput
#endif
#define _stocks_left4dead2_included

#include <halflife>
#include <stocks/player>

#define TEAM_SURVIVOR			2
#define	TEAM_INFECTED			3
#define TEAM_SCRIPTEDAI			4 // Left 4 Dead 1 bots

enum ZombieClass
{
	Zombie_Invalid = 0,
	Zombie_Smoker,
	Zombie_Boomer,
	Zombie_Hunter,
	Zombie_Spitter,
	Zombie_Jockey,
	Zombie_Charger,
	Zombie_Unknown1,
	Zombie_Tank,
	Zombie_Unknown2
};

static int m_zombieClass = -2;
stock ZombieClass GetZombieClass(int client)
{
	Assert(IsClient(client));
	
	DeclareNetPropOf(client, m_zombieClass);
	
	return view_as<ZombieClass>( GetEntData(client, m_zombieClass, 1) );
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
	Assert( IsValidEdict( weapon ) );
	
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
	Assert( IsValidEdict( weapon ) );
	
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
	Assert( IsValidEdict( weapon ) );
	
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
	Assert( IsValidEdict( weapon ) );
	
	DeclareNetPropOf(weapon, m_nUpgradedPrimaryAmmoLoaded);
	
	SetEntData(weapon, m_nUpgradedPrimaryAmmoLoaded, amount, 1, true);
}

enum BackpackItemActionType
{
	UseAction_None = 0,
	UseAction_Healing = 1,
	UseAction_RevivedGetUp = 5,
	UseAction_UseTimedButton = 0xA,
	UseAction_UsePointScript = 0xB,
};

static int m_iCurrentUseAction = -2;
stock BackpackItemActionType GetUseAction(int client)
{
	Assert(IsClient(client));
	
	DeclareNetPropOf(client, m_iCurrentUseAction);
	
	return view_as<BackpackItemActionType>( GetEntData(client, m_iCurrentUseAction, 1) );
}

stock void SetUseAction(int client, BackpackItemActionType action)
{
	Assert(IsClient(client));
	
	DeclareNetPropOf(client, m_iCurrentUseAction);
	
	SetEntData(client, m_iCurrentUseAction, view_as<int>( action ), 1);
}

static int m_useActionOwner = -2;
stock int GetUseActionOwner( int client )
{
	Assert(IsClient(client));
	
	DeclareNetPropOf(client, m_useActionOwner);
	
	return GetEntDataEnt2(client, m_useActionOwner);
}

stock void SetUseActionOwner( int client, int owner = INVALID_ENT_REFERENCE )
{
	Assert(IsClient(client));
	
	DeclareNetPropOf(client, m_useActionOwner);
	
	SetEntDataEnt2(client, m_useActionOwner, owner, true);
}

static int m_useActionTarget = -2;
stock int GetUseActionTarget( int client )
{
	Assert(IsClient(client));
	
	DeclareNetPropOf(client, m_useActionTarget);
	
	return GetEntDataEnt2(client, m_useActionTarget);
}

stock void SetUseActionTarget( int client, int target = INVALID_ENT_REFERENCE )
{
	Assert(IsClient(client));
	
	DeclareNetPropOf(client, m_useActionTarget);
	
	SetEntDataEnt2(client, m_useActionTarget, target, true);
}

int m_itTimer = -2;
stock bool IsPlayerBiled(int client)
{
	Assert(IsClient(client));
	
	DeclareNetPropOf(client, m_itTimer);
	
	return GetEntDataFloat(player, m_itTimer + 8) != -1.0;
}
