#if defined _stocks_left4dead2_included
 #endinput
#endif
#define _stocks_left4dead2_included

#include <halflife>

#define TEAM_SURVIVOR			2
#define	TEAM_INFECTED			3
#define TEAM_SCRIPTEDAI			4 // Left 4 Dead 1 bots

enum L4D2_ZombieClass
{
	L4D2Zombie_Invalid = 0,
	L4D2Zombie_Smoker,
	L4D2Zombie_Boomer,
	L4D2Zombie_Hunter,
	L4D2Zombie_Spitter,
	L4D2Zombie_Jockey,
	L4D2Zombie_Charger,
	L4D2Zombie_Unknown1,
	L4D2Zombie_Tank,
	L4D2Zombie_Unknown2
};

static int m_zombieClass = -2;
stock L4D2_ZombieClass L4D2_GetZombieClass(int client)
{
	Assert(IsClient(client));
	
	DeclareNetPropOf(entity, m_zombieClass);
	
	return view_as<L4D2_ZombieClass>( GetEntData(client, m_zombieClass, 1) );
}

#define L4D2_UPGRADEFLAG_NONE 			0
#define L4D2_UPGRADEFLAG_INCENDIARY 	0x1
#define L4D2_UPGRADEFLAG_EXPLOSIVE 		0x2
#define L4D2_UPGRADEFLAG_LASER			0x4

static int m_upgradeBitVec = -2;
/**
 * Gets a weapon's current upgrade bit
 *
 * @param weapon		weapon index.
 * @return				Current weapon upgrade bit flags.
 * @error				Invalid weapon.
 */
stock int L4D2_GetWeaponUpgradeBits(int weapon)
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
stock void L4D2_SetWeaponUpgradeBits(int weapon, int flags)
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
stock int L4D2_GetLoadedUpgradeAmmo(int weapon)
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
stock void L4D2_SetLoadedUpgradeAmmo(int weapon, int amount)
{
	Assert( IsValidEdict( weapon ) );
	
	DeclareNetPropOf(weapon, m_nUpgradedPrimaryAmmoLoaded);
	
	SetEntData(weapon, m_nUpgradedPrimaryAmmoLoaded, amount, 1, true);
}	
