#if defined _stocks_entity_included
 #endinput
#endif
#define _stocks_entity_included

#include <sdktools_functions>
#include <sdktools_entinput>

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

// entity effects
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

enum UseType_t
{
	USE_OFF = 0, 
	USE_ON = 1, 
	USE_SET = 2, 
	USE_TOGGLE = 3
};

#define	MAX_EDICT_BITS				11
#define NUM_ENT_ENTRY_BITS		(MAX_EDICT_BITS + 1)
#define NUM_ENT_ENTRIES			(1 << NUM_ENT_ENTRY_BITS)
#define ENT_ENTRY_MASK			(NUM_ENT_ENTRIES - 1)

#define GetNetOffsetEx(%0,%1) %0 = GetNetOffset(%1, #%0)
#define GetOffsetEx(%0,%1) %0 = GetOffset(%1, #%0)

stock int GetNetOffset(int &offset, const char[] netclass, const char[] name)
{
	int offset = FindSendPropInfo(netclass, name);
	if (offset == -1) {
		SetFailState("Unable to retrieve a network property offset \"%s::%s\"", netclass, name);
	}
	
	return offset;
}

stock int GetOffset(int &offset, const char[] classname, const char[] name)
{
	int entity = CreateEntityByName(classname);
	if (entity == -1 || !IsValidEdict(entity)) {
		SetFailState("Unable to retrieve a property offset \"%s::%s\" - wrong classname", classname, name);
		
		return;
	}
	
	int offset = FindDataMapInfo(entity, name);
	RemoveEdict(entity);
	
	if (offset == -1) {
		SetFailState("Unable to retrieve a property offset \"%s::%s\"", classname, name);
	}
	
	return offset;
}

stock void AddFlags(int entity, int flags)
{
	SetEntProp(entity, Prop_Data, "m_iEFlags", GetEntProp(entity, Prop_Data, "m_iEFlags") | flags);
}

stock void RemoveFlags(int entity, int flags)
{
	SetEntProp(entity, Prop_Data, "m_iEFlags", GetEntProp(entity, Prop_Data, "m_iEFlags") & ~flags);
}

stock int GetFlags(int entity)
{
	return GetEntProp(entity, Prop_Data, "m_iEFlags");
}

stock void AddEffects(int entity, int flags)
{
	SetEntProp(entity, Prop_Send, "m_fEffects", GetEntProp(entity, Prop_Send, "m_fEffects") | flags);
}

stock void RemoveEffects(int entity, int flags)
{
	SetEntProp(entity, Prop_Send, "m_fEffects", GetEntProp(entity, Prop_Send, "m_fEffects") & ~flags);
}

stock void SetSolid(int entity, SolidType_t type = SOLID_NONE)
{
	SetEntProp(entity, Prop_Data, "m_nSolidType", type);
}

stock void SetSolidFlags(int entity, int flags = 0)
{
	SetEntProp(entity, Prop_Data, "m_usSolidFlags", flags);
}

stock void GetMins(int entity, float vec[3])
{
	GetEntPropVector(entity, Prop_Data, "m_vecMins", vec);
}

stock void GetMaxs(int entity, float vec[3])
{
	GetEntPropVector(entity, Prop_Data, "m_vecMaxs", vec);
}

stock void SetMins(int entity, float back, float left, float down)
{
	float vec[3];
	vec[0] = back;
	vec[1] = left;
	vec[2] = down;
	SetEntPropVector(entity, Prop_Data, "m_vecMins", vec);
}

stock void SetMaxs(int entity, float fwd, float right, float up)
{
	float vec[3];
	vec[0] = fwd;
	vec[1] = right;
	vec[2] = up;
	SetEntPropVector(entity, Prop_Data, "m_vecMaxs", vec);
}

stock void StopThink(int entity)
{
	SetEntProp(entity, Prop_Data, "m_nNextThinkTick", -1);
	AddFlags(entity, EFL_NO_THINK_FUNCTION);
}

stock bool IsClassedAs(int entity, const char[] classname)
{
	char buffer[128];
	GetEntityClassname(entity, buffer, sizeof(buffer));
	
	return !strcmp(classname, buffer);
}

stock bool IsNamedAs(int entity, const char[] name)
{
	char buffer[128];
	GetTargetname(entity, buffer, sizeof(buffer));
	
	return !strcmp(name, buffer);
}

stock void SetSpawnflags(int entity, int flags)
{
	SetEntProp(entity, Prop_Data, "m_spawnflags", flags);
}

stock int SetTargetname(int entity, const char[] targetname)
{
	return SetEntPropString(entity, Prop_Data, "m_iName", targetname);
}

stock int GetTargetname(int entity, char[] targetname, int length)
{
	return GetEntPropString(entity, Prop_Data, "m_iName", targetname, length);
}

stock int GetParent(int entity)
{
	return GetEntPropEnt(entity, Prop_Data, "m_pParent");
}

stock void SetParent(int entity, int parent)
{
	SetVariantString("!activator");
	AcceptEntityInput(entity, "SetParent", parent);
}

stock void SetParentAttachment(int entity, const char[] attachment_name)
{
	SetVariantString(attachment_name);
	AcceptEntityInput(entity, "SetParentAttachment");
}

stock void ClearParent(int entity)
{
	AcceptEntityInput(entity, "ClearParent");
}

stock void UseEntity(int entity, int user, UseType_t type)
{
	AcceptEntityInput(entity, "Use", user, entity, view_as<int>(type));
}

stock void KillEntity(int entity)
{
	AcceptEntityInput(entity, "Kill");
}

stock void KillHierarchy(int entity)
{
	AcceptEntityInput(entity, "KillHierarchy");
}
