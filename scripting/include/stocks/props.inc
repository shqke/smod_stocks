#if defined _stocks_props_included
 #endinput
#endif
#define _stocks_props_included

#include <stocks/entity>
#include <stocks/player>

// This is NOT for CBreakable
stock static int m_hBreaker = -2;
int GetPropBreaker(int entity)
{
	DeclarePropOf(entity, m_hBreaker);
	
	return GetEntDataEnt2(entity, m_hBreaker);
}

void SetPropBreaker(int entity, int breaker)
{
	DeclarePropOf(entity, m_hBreaker);
	
	SetEntDataEnt2(entity, m_hBreaker, breaker);
}

stock static int m_hLastAttacker = -2;
int GetPropLastAttacker(int entity)
{
	DeclarePropOf(entity, m_hLastAttacker);
	
	return GetEntDataEnt2(entity, m_hLastAttacker);
}

void SetPropLastAttacker(int entity, int attacker)
{
	DeclarePropOf(entity, m_hLastAttacker);
	
	SetEntDataEnt2(entity, m_hLastAttacker, attacker);
}

// void SetPhysicsAttacker( CBasePlayer *pEntity, float flTime );

stock static int m_hPhysicsAttacker = -2;
int GetPropPhysicsAttacker(int entity)
{
	DeclarePropOf(entity, m_hPhysicsAttacker);
	
	return GetEntDataEnt2(entity, m_hPhysicsAttacker);
}

void SetPropPhysicsAttacker(int entity, int attacker /* CBasePlayer */)
{
	DeclarePropOf(entity, m_hPhysicsAttacker);
	
	SetEntDataEnt2(entity, m_hPhysicsAttacker, attacker);
}
