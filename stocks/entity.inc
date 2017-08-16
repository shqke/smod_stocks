#if defined _stocks_entity_included
	#endinput
#endif
#define _stocks_entity_included

#include <sdktools_functions>
#include <sdktools_entinput>
#include <sdktools_entoutput>
#include <sdkhooks>

#include <stocks/assert>
#include <stocks/property>

#define NULL_ENTITY INVALID_ENT_REFERENCE

stock int CreateEntityAt(const char[] classname, const float[3] origin = NULL_VECTOR, const float[3] angles = NULL_VECTOR, int edict = NULL_ENTITY)
{
	static int entity;
	if (edict != NULL_ENTITY && IsValidEdict(edict)) {
		return NULL_ENTITY;
	}
	
	entity = CreateEntityByName(classname, edict);
	
	Assert(IsValidEdict(entity));
	
	TeleportEntity(entity, origin, angles, NULL_VECTOR);
	
	return entity;
}

static int m_hOwnerEntity = -2;
stock void SetOwnerEntity(int entity, int owner)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_hOwnerEntity);
	
	SetEntDataEnt2(entity, m_hOwnerEntity, owner);
}

stock int GetOwnerEntity(int entity)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_hOwnerEntity);
	
	return GetEntDataEnt2(entity, m_hOwnerEntity);
}

static int m_hOwner = -2;
stock void SetOwner(int entity, int owner)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_hOwner);
	
	SetEntDataEnt2(entity, m_hOwner, owner);
}

stock int GetOwner(int entity)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_hOwner);
	
	return GetEntDataEnt2(entity, m_hOwner);
}

static int m_hGroundEntity = -2;
stock int GetGroundEntity(int entity)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_hGroundEntity);
	
	return GetEntDataEnt2(entity, m_hGroundEntity);
}

stock void SetGroundEntity(int entity, int groundent = NULL_ENTITY)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_hGroundEntity);
	
	SetEntDataEnt2(entity, m_hGroundEntity, groundent, true);
	
	SetEntityFlags(entity, (GetEntityFlags(entity) & ~(FL_ONGROUND)) | (view_as<int>( IsValidEdict(groundent) ) << FL_ONGROUND));
}

enum
{
	WL_NotInWater = 0,
	WL_Feet,
	WL_Waist,
	WL_Eyes
};

static int m_nWaterLevel = -2;
stock int GetWaterLevel(int entity)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_nWaterLevel);
	
	return GetEntData(entity, m_nWaterLevel, 1);
}

static int m_iMaxHealth = -2;
stock int GetMaxHealth(int entity)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_iMaxHealth);
	
	return GetEntData(entity, m_iMaxHealth, 4);
}

stock void SetMaxHealth(int entity, int amt)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_iMaxHealth);
	
	SetEntData(entity, m_iMaxHealth, amt, 4, true);
}

static int m_iHealth = -2;
stock int GetHealth(int entity)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_iHealth);
	
	return GetEntData(entity, m_iHealth, 4);
}

#if !defined _smlib_entities_included
	#define	DAMAGE_NO				0
	#define DAMAGE_EVENTS_ONLY		1		// Call damage functions, but don't modify health
	#define	DAMAGE_YES				2
	#define	DAMAGE_AIM				3
#endif

static int m_takedamage = -2;
stock int GetTakeDamage(int entity)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_takedamage);
	
	return GetEntData(entity, m_takedamage, 1);
}

stock void SetTakeDamage(int entity, int value)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_takedamage);
	
	SetEntData(entity, m_takedamage, value, 1);
}

#if !defined _smlib_entities_included
enum
{
	EFL_KILLME	=				(1<<0),	// This entity is marked for death -- This allows the game to actually delete ents at a safe time
	EFL_DORMANT	=				(1<<1),	// Entity is dormant, no updates to client
	EFL_NOCLIP_ACTIVE =			(1<<2),	// Lets us know when the noclip command is active.
	EFL_SETTING_UP_BONES =		(1<<3),	// Set while a model is setting up its bones.
	EFL_KEEP_ON_RECREATE_ENTITIES = (1<<4), // This is a special entity that should not be deleted when we restart entities only

	EFL_HAS_PLAYER_CHILD=		(1<<4),	// One of the child entities is a player.

	EFL_DIRTY_SHADOWUPDATE =	(1<<5),	// Client only- need shadow manager to update the shadow...
	EFL_NOTIFY =				(1<<6),	// Another entity is watching events on this entity (used by teleport)

	// The default behavior in ShouldTransmit is to not send an entity if it doesn't
	// have a model. Certain entities want to be sent anyway because all the drawing logic
	// is in the client DLL. They can set this flag and the engine will transmit them even
	// if they don't have a model.
	EFL_FORCE_CHECK_TRANSMIT =	(1<<7),

	EFL_BOT_FROZEN =			(1<<8),	// This is set on bots that are frozen.
	EFL_SERVER_ONLY =			(1<<9),	// Non-networked entity.
	EFL_NO_AUTO_EDICT_ATTACH =	(1<<10), // Don't attach the edict; we're doing it explicitly
	
	// Some dirty bits with respect to abs computations
	EFL_DIRTY_ABSTRANSFORM =	(1<<11),
	EFL_DIRTY_ABSVELOCITY =		(1<<12),
	EFL_DIRTY_ABSANGVELOCITY =	(1<<13),
	EFL_DIRTY_SURROUNDING_COLLISION_BOUNDS	= (1<<14),
	EFL_DIRTY_SPATIAL_PARTITION = (1<<15),
//	UNUSED						= (1<<16),

	EFL_IN_SKYBOX =				(1<<17),	// This is set if the entity detects that it's in the skybox.
											// This forces it to pass the "in PVS" for transmission.
	EFL_USE_PARTITION_WHEN_NOT_SOLID = (1<<18),	// Entities with this flag set show up in the partition even when not solid
	EFL_TOUCHING_FLUID =		(1<<19),	// Used to determine if an entity is floating

	// FIXME: Not really sure where I should add this...
	EFL_IS_BEING_LIFTED_BY_BARNACLE = (1<<20),
	EFL_NO_ROTORWASH_PUSH =		(1<<21),		// I shouldn't be pushed by the rotorwash
	EFL_NO_THINK_FUNCTION =		(1<<22),
	EFL_NO_GAME_PHYSICS_SIMULATION = (1<<23),

	EFL_CHECK_UNTOUCH =			(1<<24),
	EFL_DONTBLOCKLOS =			(1<<25),		// I shouldn't block NPC line-of-sight
	EFL_DONTWALKON =			(1<<26),		// NPC;s should not walk on this entity
	EFL_NO_DISSOLVE =			(1<<27),		// These guys shouldn't dissolve
	EFL_NO_MEGAPHYSCANNON_RAGDOLL = (1<<28),	// Mega physcannon can't ragdoll these guys.
	EFL_NO_WATER_VELOCITY_CHANGE  =	(1<<29),	// Don't adjust this entity's velocity when transitioning into water
	EFL_NO_PHYSCANNON_INTERACTION =	(1<<30),	// Physcannon can't pick these up or punt them
	EFL_NO_DAMAGE_FORCES =		(1<<31),	// Doesn't accept forces from physics damage
};
#endif

static int m_iEFlags = -2;
stock int GetEFlags(int entity)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_iEFlags);
	
	return GetEntData(entity, m_iEFlags, 4);
}

stock void SetEFlags(int entity, int iEFlags)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_iEFlags);
	
	SetEntData(entity, m_iEFlags, iEFlags, 4);
}

stock void AddEFlags(int entity, int nEFlagMask)
{
	SetEFlags(entity, GetEFlags(entity) | nEFlagMask);
}

stock void RemoveEFlags(int entity, int nEFlagMask)
{
	SetEFlags(entity, GetEFlags(entity) & ~nEFlagMask);
}

stock bool IsEFlagSet(int entity, int nEFlagMask)
{
	return (GetEFlags(entity) & nEFlagMask);
}

//
// Effects
//
enum
{
	EF_BONEMERGE			= 0x001,	// Performs bone merge on client side
	EF_BRIGHTLIGHT 			= 0x002,	// DLIGHT centered at entity origin
	EF_DIMLIGHT 			= 0x004,	// player flashlight
	EF_NOINTERP				= 0x008,	// don't interpolate the next frame
	EF_NOSHADOW				= 0x010,	// Don't cast no shadow
	EF_NODRAW				= 0x020,	// don't draw entity
	EF_NORECEIVESHADOW		= 0x040,	// Don't receive no shadow
	EF_BONEMERGE_FASTCULL	= 0x080,	// For use with EF_BONEMERGE. If this is set, then it places this ent's origin at its
										// parent and uses the parent's bbox + the max extents of the aiment.
										// Otherwise, it sets up the parent's bones every frame to figure out where to place
										// the aiment, which is inefficient because it'll setup the parent's bones even if
										// the parent is not in the PVS.
	EF_ITEM_BLINK			= 0x100,	// blink an item so that the user notices it.
	EF_PARENT_ANIMATES		= 0x200,	// always assume that the parent entity is animating
	EF_MAX_BITS = 10
};

static int m_fEffects = -2;
stock int GetEffects(int entity)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_fEffects);
	
	return GetEntData(entity, m_fEffects, 4);
}

stock void SetEffects(int entity, int nFlags)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_fEffects);
	
	SetEntData(entity, m_fEffects, nFlags, 4, true);
}

stock void AddEffects(int entity, int nEffects)
{
	SetEffects(entity, GetEffects(entity) | nEffects);
}

stock void RemoveEffects(int entity, int nEffects)
{
	SetEffects(entity, GetEffects(entity) & ~nEffects);
}

stock void ClearEffects(int entity)
{
	SetEffects(entity, 0);
}

stock bool IsEffectActive(int entity, int nEffects)
{
	return (GetEffects(entity) & nEffects);
}

static int m_hEffectEntity = -2;
stock void SetEffectEntity(int entity, int pEffectEnt)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_hEffectEntity);
	
	SetEntDataEnt2(entity, m_hEffectEntity, pEffectEnt);
}

stock int GetEffectEntity(int entity)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_hEffectEntity);
	
	return GetEntDataEnt2(entity, m_hEffectEntity);
}

//
// Spawnflags
//
static int m_spawnflags = -2;
stock void SetSpawnflags(int entity, int flags)
{
	Assert(IsValidEntity(entity));
	
	DeclarePropOf(entity, m_spawnflags);
	
	SetEntData(entity, m_spawnflags, flags, 4);
}

stock int GetSpawnflags(int entity)
{
	Assert(IsValidEntity(entity));
	
	DeclarePropOf(entity, m_spawnflags);
	
	return GetEntData(entity, m_spawnflags, 4);
}

stock void AddSpawnFlags(int entity, int nFlags)
{
	SetSpawnflags(GetSpawnflags(entity) | nFlags);
}

stock void RemoveSpawnFlags(int entity, int nFlags)
{
	SetSpawnflags(GetSpawnflags(entity) & ~nFlags);
}

stock void ClearSpawnFlags(int entity)
{
	SetSpawnflags(entity, 0);
}

stock bool HasSpawnFlags(int entity, int nFlags)
{
	return (GetSpawnflags(entity) & nFlags);
}

//
// Solid Type
//
#if !defined __thelpers_entity_included
	#if !defined _smlib_entities_included
enum SolidType_t
{
	SOLID_NONE			= 0,	// no solid model
	SOLID_BSP			= 1,	// a BSP tree
	SOLID_BBOX			= 2,	// an AABB
	SOLID_OBB			= 3,	// an OBB (not implemented yet)
	SOLID_OBB_YAW		= 4,	// an OBB, constrained so that it can only yaw
	SOLID_CUSTOM		= 5,	// Always call into the entity for tests
	SOLID_VPHYSICS		= 6,	// solid vphysics object, get vcollide from the model and collide with that
	SOLID_LAST,
};
	#endif
#endif

static int m_nSolidType = -2;
stock void SetSolid(int entity, SolidType_t type = SOLID_NONE)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_nSolidType);
	
	SetEntData(entity, m_nSolidType, view_as< int >(type), 1, true);
}

stock SolidType_t GetSolid(int entity)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_nSolidType);
	
	return view_as< SolidType_t >(GetEntData(entity, m_nSolidType, 1));
}

//
// Move Collide
//
enum MoveCollide_t
{
	MOVECOLLIDE_DEFAULT = 0,

	// These ones only work for MOVETYPE_FLY + MOVETYPE_FLYGRAVITY
	MOVECOLLIDE_FLY_BOUNCE,	// bounces, reflects, based on elasticity of surface and object - applies friction (adjust velocity)
	MOVECOLLIDE_FLY_CUSTOM,	// Touch() will modify the velocity however it likes
	MOVECOLLIDE_FLY_SLIDE,  // slides along surfaces (no bounce) - applies friciton (adjusts velocity)

	MOVECOLLIDE_COUNT,		// Number of different movecollides

	// When adding new movecollide types, make sure this is correct
	MOVECOLLIDE_MAX_BITS = 3
};

static int m_MoveCollide = -2;
stock void SetMoveCollide(int entity, MoveCollide_t type = MOVECOLLIDE_DEFAULT)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_MoveCollide);
	
	SetEntData(entity, m_MoveCollide, view_as< int >(type), 1, true);
}

stock MoveCollide_t GetMoveCollide(int entity)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_MoveCollide);
	
	return view_as< MoveCollide_t >(GetEntData(entity, m_MoveCollide, 1));
}

//
// Collision Group
//
#if !defined _smlib_entities_included
enum Collision_Group_t
{
	COLLISION_GROUP_NONE  = 0,
	COLLISION_GROUP_DEBRIS,			// Collides with nothing but world and static stuff
	COLLISION_GROUP_DEBRIS_TRIGGER, // Same as debris, but hits triggers
	COLLISION_GROUP_INTERACTIVE_DEBRIS,	// Collides with everything except other interactive debris or debris
	COLLISION_GROUP_INTERACTIVE,	// Collides with everything except interactive debris or debris
	COLLISION_GROUP_PLAYER,
	COLLISION_GROUP_BREAKABLE_GLASS,
	COLLISION_GROUP_VEHICLE,
	COLLISION_GROUP_PLAYER_MOVEMENT,  // For HL2, same as Collision_Group_Player, for
										// TF2, this filters out other players and CBaseObjects
	COLLISION_GROUP_NPC,			// Generic NPC group
	COLLISION_GROUP_IN_VEHICLE,		// for any entity inside a vehicle
	COLLISION_GROUP_WEAPON,			// for any weapons that need collision detection
	COLLISION_GROUP_VEHICLE_CLIP,	// vehicle clip brush to restrict vehicle movement
	COLLISION_GROUP_PROJECTILE,		// Projectiles!
	COLLISION_GROUP_DOOR_BLOCKER,	// Blocks entities not permitted to get near moving doors
	COLLISION_GROUP_PASSABLE_DOOR,	// Doors that the player shouldn't collide with
	COLLISION_GROUP_DISSOLVING,		// Things that are dissolving are in this group
	COLLISION_GROUP_PUSHAWAY,		// Nonsolid on client and server, pushaway in player code

	COLLISION_GROUP_NPC_ACTOR,		// Used so NPCs in scripts ignore the player.
	COLLISION_GROUP_NPC_SCRIPTED,	// USed for NPCs in scripts that should not collide with each other

	LAST_SHARED_COLLISION_GROUP
};
#endif

static int m_CollisionGroup = -2;
stock void SetCollisionGroup(int entity, Collision_Group_t group = COLLISION_GROUP_NONE)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_CollisionGroup);
	
	SetEntData(entity, m_CollisionGroup, view_as< int >(group), 4, true);
}

stock Collision_Group_t GetCollisionGroup(int entity)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_CollisionGroup);
	
	return view_as< Collision_Group_t >(GetEntData(entity, m_CollisionGroup, 4));
}

//
// Solid Flags
//
#if !defined __thelpers_entity_included
	#if !defined _smlib_entities_included
enum SolidFlags_t
{
	FSOLID_CUSTOMRAYTEST		= 0x0001,	// Ignore solid type + always call into the entity for ray tests
	FSOLID_CUSTOMBOXTEST		= 0x0002,	// Ignore solid type + always call into the entity for swept box tests
	FSOLID_NOT_SOLID			= 0x0004,	// Are we currently not solid?
	FSOLID_TRIGGER				= 0x0008,	// This is something may be collideable but fires touch functions
											// even when it's not collideable (when the FSOLID_NOT_SOLID flag is set)
	FSOLID_NOT_STANDABLE		= 0x0010,	// You can't stand on this
	FSOLID_VOLUME_CONTENTS		= 0x0020,	// Contains volumetric contents (like water)
	FSOLID_FORCE_WORLD_ALIGNED	= 0x0040,	// Forces the collision rep to be world-aligned even if it's SOLID_BSP or SOLID_VPHYSICS
	FSOLID_USE_TRIGGER_BOUNDS	= 0x0080,	// Uses a special trigger bounds separate from the normal OBB
	FSOLID_ROOT_PARENT_ALIGNED	= 0x0100,	// Collisions are defined in root parent's local coordinate space
	FSOLID_TRIGGER_TOUCH_DEBRIS	= 0x0200,	// This trigger will touch debris objects

	FSOLID_MAX_BITS	= 10
};
	#endif
#endif

static int m_usSolidFlags = -2;
stock void SetSolidFlags(int entity, SolidFlags_t flags)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_usSolidFlags);
	
	SetEntData(entity, m_usSolidFlags, flags, 2, true);
}

stock SolidFlags_t GetSolidFlags(int entity)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_usSolidFlags);
	
	return view_as< SolidFlags_t >(GetEntData(entity, m_usSolidFlags, 4));
}

stock void ClearSolidFlags(int entity)
{
	SetSolidFlags(entity, 0);
}

stock void RemoveSolidFlags(int entity, int flags)
{
	SetSolidFlags(entity, GetSolidFlags(entity) & ~flags);
}

stock void AddSolidFlags(int entity, int flags)
{
	SetSolidFlags(entity, GetSolidFlags(entity) | flags);
}

stock bool IsSolid(int entity)
{
	return (GetSolid(entity) != SOLID_NONE) && (~GetSolidFlags(entity) & FSOLID_NOT_SOLID);
}

stock bool IsBoundsDefinedInEntitySpace(int entity)
{
	Assert(IsValidEdict(entity));
	
	SolidType_t type = GetSolid(entity);
	
	return (!IsSolidFlagSet(entity, FSOLID_FORCE_WORLD_ALIGNED)) &&
		(type != SOLID_BBOX) && (type != SOLID_NONE);
}

static int m_vecOrigin = -2;
stock void GetLocalOrigin(int entity, float vec[ 3 ])
{
	Assert(IsValidEntity(entity));
	
	DeclarePropOf(entity, m_vecOrigin);
	
	GetEntDataVector(entity, m_vecOrigin, vec);
}

public void SetLocalOrigin(int entity, const float vec[ 3 ])
{
	Assert(IsValidEntity(entity));
	
	DeclarePropOf(entity, m_vecOrigin);
	
	SetEntDataVector(entity, m_vecOrigin, vec, true);
}

static int m_angRotation = -2;
public void GetLocalAngles(int entity, float ang[ 3 ])
{
	Assert(IsValidEntity(entity));
	
	DeclarePropOf(entity, m_angRotation);
	
	GetEntDataVector(entity, m_angRotation, ang);
}

public void SetLocalAngles(int entity, const float ang[ 3 ])
{
	Assert(IsValidEntity(entity));
	
	DeclarePropOf(entity, m_angRotation);
	
	SetEntDataVector(entity, m_angRotation, ang, true);
}

static int m_vecAbsOrigin = -2;
public void GetAbsOrigin(int entity, float vec[ 3 ])
{
	Assert(IsValidEntity(entity));
	
	DeclarePropOf(entity, m_vecAbsOrigin);
	
	GetEntDataVector(entity, m_vecAbsOrigin, vec);
}

public void SetAbsOrigin(int entity, const float vec[ 3 ])
{
	Assert(IsValidEntity(entity));
	
	DeclarePropOf(entity, m_vecAbsOrigin);
	
	SetEntDataVector(entity, m_vecAbsOrigin, vec);
}

static int m_angAbsRotation = -2;
public void GetAbsAngles(int entity, float ang[ 3 ])
{
	Assert(IsValidEntity(entity));
	
	DeclarePropOf(entity, m_angAbsRotation);
	
	GetEntDataVector(entity, m_angAbsRotation, ang);
}

public void SetAbsAngles(int entity, const float ang[ 3 ])
{
	Assert(IsValidEntity(entity));
	
	DeclarePropOf(entity, m_vecAbsOrigin);
	
	SetEntDataVector(entity, m_vecAbsOrigin, ang);
}

static int m_vecVelocity = -2;
public void GetLocalVelocity(int entity, float vel[ 3 ])
{
	Assert(IsValidEntity(entity));
	
	DeclarePropOf(entity, m_vecVelocity);
	
	GetEntDataVector(entity, m_vecVelocity, vel);
}

public void SetLocalVelocity(int entity, const float vel[ 3 ])
{
	Assert(IsValidEntity(entity));
	
	DeclarePropOf(entity, m_vecVelocity);
	
	SetEntDataVector(entity, m_vecVelocity, vel);
}

static int m_vecAbsVelocity = -2;
public void GetAbsVelocity(int entity, float vel[ 3 ])
{
	Assert(IsValidEntity(entity));
	
	DeclarePropOf(entity, m_vecAbsVelocity);
	
	GetEntDataVector(entity, m_vecAbsVelocity, vel);
}

public void SetAbsVelocity(int entity, const float vel[ 3 ])
{
	Assert(IsValidEntity(entity));
	
	DeclarePropOf(entity, m_vecAbsVelocity);
	
	SetEntDataVector(entity, m_vecAbsVelocity, vel);
}

static int m_vecBaseVelocity = -2;
public void GetBaseVelocity(int entity, float vel[ 3 ])
{
	Assert(IsValidEntity(entity));
	
	DeclarePropOf(entity, m_vecBaseVelocity);
	
	GetEntDataVector(entity, m_vecBaseVelocity, vel);
}

public void SetBaseVelocity(int entity, const float vel[ 3 ])
{
	Assert(IsValidEntity(entity));
	
	DeclarePropOf(entity, m_vecBaseVelocity);
	
	SetEntDataVector(entity, m_vecBaseVelocity, vel);
}

static int m_vecMins = -2;
stock void GetMins(int entity, float vec[3])
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_vecMins);
	
	GetEntDataVector(entity, m_vecMins, vec);
}

static int m_vecMaxs = -2;
stock void GetMaxs(int entity, float vec[3])
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_vecMaxs);
	
	GetEntDataVector(entity, m_vecMaxs, vec);
}

stock void SetMins(int entity, float back, float left, float down)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_vecMins);
	
	static float vec[3];
	vec[0] = back;
	vec[1] = left;
	vec[2] = down;
	SetEntDataVector(entity, m_vecMins, vec, true);
}

stock void SetMaxs(int entity, float fwd, float right, float up)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_vecMaxs);
	
	static float vec[3];
	vec[0] = fwd;
	vec[1] = right;
	vec[2] = up;
	SetEntDataVector(entity, m_vecMaxs, vec, true);
}

stock void SetSize(int entity, const float[3] mins, const float[3] maxs)
{
	for (int i = 0; i < 3; i++) {
		Assert(mins[i] < maxs[i]);
	}
	
	SetMins(entity, mins[0], mins[1], mins[2]);
	SetMaxs(entity, maxs[0], maxs[1], maxs[2]);
	
	AddEFlags(entity, EFL_DIRTY_SURROUNDING_COLLISION_BOUNDS);
}

static int m_iHammerID = -2;
stock int GetHammerID(int entity)
{
	DeclarePropOf(entity, m_iHammerID);
	
	return GetEntData(entity, m_iHammerID, 4);
}

//
// Time, intervals
//
#define TICK_INTERVAL (GetTickInterval())
#if !defined _smlib_general_included
	#define TIME_TO_TICKS(%1)		view_as< int >(0.5 + view_as< float >(%1) / TICK_INTERVAL)
	#define TICKS_TO_TIME(%1)		(TICK_INTERVAL * (%1))
	#define ROUND_TO_TICKS(%1)	(TICK_INTERVAL * TIME_TO_TICKS(%1))
#endif
#define TICK_NEVER_THINK		(-1)

static int m_nNextThinkTick = -2;
stock void SetNextThink(int entity, float flTime = -1.0)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_nNextThinkTick);
	
	if (flTime == -1.0) {
		SetEntData(entity, m_nNextThinkTick, TICK_NEVER_THINK);
		
		return;
	}
	
	SetEntData(entity, m_nNextThinkTick, TIME_TO_TICKS(flTime), 4, true);
}

stock void StopThink(int entity)
{
	SetNextThink(entity, -1.0);
	
	AddEFlags(entity, EFL_NO_THINK_FUNCTION);
}

static int m_flFriction = -2;
stock void SetFriction(int entity, float friction)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_flFriction);
	
	SetEntDataFloat(entity, m_flFriction, friction);
}

stock bool IsClassedAs(int entity, const char[] classname)
{
	static char buffer[128];
	GetEntityClassname(entity, buffer, sizeof(buffer));
	
	return !strcmp(classname, buffer);
}

stock bool IsNamedAs(int entity, const char[] name)
{
	static char buffer[128];
	GetTargetname(entity, buffer, sizeof(buffer));
	
	return !strcmp(name, buffer);
}

stock int GetEntityModel(int entity, char[] modelname, int length)
{
	Assert(IsValidEdict(entity));
	
	return GetEntPropString(entity, Prop_Data, "m_ModelName", modelname, length);
}

stock bool IsModelSet(int entity, const char[] name)
{
	static char buffer[128];
	GetEntityModel(entity, buffer, sizeof(buffer));
	
	return !strcmp(name, buffer);
}

static int m_nModelIndex = -2;
stock int GetModelIndex(int entity)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_nModelIndex);
	
	return GetEntData(entity, m_nModelIndex, 2);
}

stock int SetTargetname(int entity, const char[] targetname)
{
	Assert(IsValidEdict(entity));
	
	return SetEntPropString(entity, Prop_Data, "m_iName", targetname);
}

stock int GetTargetname(int entity, char[] targetname, int length)
{
	Assert(IsValidEdict(entity));
	
	return GetEntPropString(entity, Prop_Data, "m_iName", targetname, length);
}

static int m_pParent = -2;
stock int GetParent(int entity)
{
	Assert(IsValidEdict(entity));
	
	DeclarePropOf(entity, m_pParent);
	
	return GetEntDataEnt2(entity, m_pParent);
}

stock void SetParent(int entity, int parent)
{
	Assert(IsValidEdict(entity));
	
	SetVariantString("!activator");
	AcceptEntityInput(entity, "SetParent", parent);
}

stock void SetParentAttachment(int entity, const char[] attachmentName, bool bMaintainOffset = false)
{
	Assert(IsValidEdict(entity));
	
	SetVariantString(attachmentName);
	AcceptEntityInput(entity, bMaintainOffset ? "SetParentAttachmentMaintainOffset" : "SetParentAttachment");
}

stock void ClearParent(int entity)
{
	Assert(IsValidEdict(entity));
	
	AcceptEntityInput(entity, "ClearParent");
}

stock void UseEntity(int entity, int user = NULL_ENTITY, UseType type = Use_Toggle)
{
	Assert(IsValidEdict(entity));
	
	AcceptEntityInput(entity, "Use", user, entity, view_as< int >(type));
}

stock void EnableEntity(int entity, int activator = NULL_ENTITY)
{
	Assert(IsValidEdict(entity));
	
	AcceptEntityInput(entity, "Enable", activator);
}

stock void DisableEntity(int entity)
{
	Assert(IsValidEdict(entity));
	
	AcceptEntityInput(entity, "Disable");
}

stock void KillEntity(int entity)
{
	Assert(IsValidEdict(entity));
	
	AcceptEntityInput(entity, "Kill");
}

stock void KillHierarchy(int entity)
{
	Assert(IsValidEdict(entity));
	
	AcceptEntityInput(entity, "KillHierarchy");
}

stock void AddOutput(int entity, const char[] fmt, any ...)
{
	Assert(IsValidEdict(entity));
	
	static char buffer[1024];
	VFormat(buffer, sizeof(buffer), fmt, 3);
	
	SetVariantString(buffer);
	AcceptEntityInput(entity, "AddOutput");
}

#define ENTITY_WORLD 0
stock void AddEntityTimer(int entity, EntityOutput callback, float flTime = 0.0)
{
	Assert(IsValidEdict(entity));
	
	HookSingleEntityOutput(entity, "OnUser4", callback, true);
	
	AddOutput(entity, "OnUser3 !self:FireUser4::%0.4f:1", flTime);
	AcceptEntityInput(entity, "FireUser3");
}

// Inline wrapper for most used action with FindEntityByClassname
#define LoopByClassname(%0,%1) for (int %0 = NULL_ENTITY; (%0 = FindEntityByClassname(%0, %1)) != NULL_ENTITY;)
