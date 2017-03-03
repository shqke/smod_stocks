#if defined _stocks_halflife_included
 #endinput
#endif
#define _stocks_halflife_included

#include <bitbuffer>

#include <stocks/entity>
#include <stocks/player>
#include <stocks/property>
#include <halflife>

//
// Screen shake
//
const float MAX_SHAKE_AMPLITUDE = 16.0;

enum ShakeCommand_t
{
	SHAKE_START = 0,		// Starts the screen shake for all players within the radius.
	SHAKE_STOP,				// Stops the screen shake for all players within the radius.
	SHAKE_AMPLITUDE,		// Modifies the amplitude of an active screen shake for all players within the radius.
	SHAKE_FREQUENCY,		// Modifies the frequency of an active screen shake for all players within the radius.
	SHAKE_START_RUMBLEONLY,	// Starts a shake effect that only rumbles the controller, no screen effect.
	SHAKE_START_NORUMBLE,	// Starts a shake that does NOT rumble the controller.
};

static stock void TransmitShakeEvent( int client, float localAmplitude, float frequency, float duration, ShakeCommand_t eCommand )
{
	BfWrite usermsg = view_as< BfWrite >( StartMessageOne( "Shake", client, USERMSG_RELIABLE ) );
	
	usermsg.WriteByte( view_as< char >( eCommand ) ); // shake command (SHAKE_START, STOP, FREQUENCY, AMPLITUDE)
	usermsg.WriteFloat( localAmplitude ); // shake magnitude/amplitude
	usermsg.WriteFloat( frequency ); // shake noise frequency
	usermsg.WriteFloat( duration ); // shake lasts this long
	
	EndMessage();
}

stock void ScreenShake( const float[3] vCenter, float amplitude, float frequency, float duration, float radius, ShakeCommand_t eCommand = SHAKE_START, bool bAirShake = false )
{
	float localAmplitude, flDistance;
	float vPos[3], vDelta[3];

	if ( amplitude > MAX_SHAKE_AMPLITUDE ) {
		amplitude = MAX_SHAKE_AMPLITUDE;
	}
	
	LoopThroughClients( player ) {
		if ( !IsClientInGame( player ) || ( !bAirShake && ( eCommand == SHAKE_START ) && ( ~GetEntityFlags( player ) & FL_ONGROUND ) ) ) {
			continue;
		}
		
		localAmplitude = amplitude;
		if ( radius > 0 ) {
			GetClientAbsOrigin( player, vPos );
			SubtractVectors( vCenter, vPos, vDelta );
			flDistance = GetVectorLength( vDelta );
			if ( flDistance > radius ) {
				// This happens if the player is outside the radius, in which case we should ignore 
				// all commands
				continue;
			}
			
			localAmplitude = amplitude * ( 1.0 - ( flDistance / radius ) );
		}
		
		TransmitShakeEvent( player, localAmplitude, frequency, duration, eCommand );
	}
}

// Entity Dissolve types
enum DissolveType_t
{
	ENTITY_DISSOLVE_NORMAL = 0,
	ENTITY_DISSOLVE_ELECTRICAL,
	ENTITY_DISSOLVE_ELECTRICAL_LIGHT,
	ENTITY_DISSOLVE_CORE,

	// NOTE: Be sure to up the bits if you make more dissolve types
	ENTITY_DISSOLVE_BITS = 3
};

static int m_nDissolveType = -2;
static int m_nMagnitude = -2;
stock void DissolveEntity( int entity, DissolveType_t type = ENTITY_DISSOLVE_NORMAL, int magnitude = 0 )
{
	Assert( IsValidEdict( entity ) );
	
	int dissolver = CreateEntityByName( "env_entity_dissolver" );
	if ( !IsValidEntity( dissolver ) ) {
		return;
	}
	
	DeclarePropOf( dissolver, m_nDissolveType );
	DeclarePropOf( dissolver, m_nMagnitude );
	
	SetEntData( dissolver, m_nDissolveType, view_as< int >( type ), 4, true );
	SetEntData( dissolver, m_nMagnitude, magnitude, 4, true );
	
	SetVariantString( "!activator" );
	AcceptEntityInput( dissolver, "Dissolve", entity );
	KillEntity( dissolver );
}

stock void StartScene( int client, const char[] scenefile )
{
	Assert( IsClient( client ) );
	
	static char modelname[256], buffer[256];
	
	int scene = CreateEntityByName( "instanced_scripted_scene" );
	if ( !IsValidEdict( scene ) ) {
		return;
	}
	
	GetClientModel( client, modelname, sizeof( modelname ) );
	strcopy( buffer, sizeof( buffer ), scenefile );
	ReplaceString( buffer, sizeof( buffer ), "$gender", StrContains( modelname, "female" ) != -1 ? "female" : "male" );
	
	SetEntPropString( scene, Prop_Data, "m_iszSceneFile", buffer );
	SetOwner( scene, client );
	
	DispatchSpawn( scene );
	ActivateEntity( scene );
	
	AcceptEntityInput( scene, "Start", client );
}

static int m_flDeathTime = -2;
stock void SetClientDeathTime( int client, float flTime = 0.0 )
{
	Assert( IsClient( client ) );
	
	DeclarePropOf( client, m_flDeathTime );
	
	SetEntDataFloat( client, m_flDeathTime, flTime, true );
}

#define	SF_ENVEXPLOSION_NODAMAGE	0x00000001 // when set, ENV_EXPLOSION will not actually inflict damage
#define	SF_ENVEXPLOSION_REPEATABLE	0x00000002 // can this entity be refired?
#define SF_ENVEXPLOSION_NOFIREBALL	0x00000004 // don't draw the fireball
#define SF_ENVEXPLOSION_NOSMOKE		0x00000008 // don't draw the smoke
#define SF_ENVEXPLOSION_NODECAL		0x00000010 // don't make a scorch mark
#define SF_ENVEXPLOSION_NOSPARKS	0x00000020 // don't make sparks
#define SF_ENVEXPLOSION_NOSOUND		0x00000040 // don't play explosion sound.
#define SF_ENVEXPLOSION_RND_ORIENT	0x00000080	// randomly oriented sprites
#define SF_ENVEXPLOSION_NOFIREBALLSMOKE 0x0100
#define SF_ENVEXPLOSION_NOPARTICLES 0x00000200
#define SF_ENVEXPLOSION_NODLIGHTS	0x00000400
#define SF_ENVEXPLOSION_NOCLAMPMIN	0x00000800 // don't clamp the minimum size of the fireball sprite
#define SF_ENVEXPLOSION_NOCLAMPMAX	0x00001000 // don't clamp the maximum size of the fireball sprite
#define SF_ENVEXPLOSION_SURFACEONLY	0x00002000 // don't damage the player if he's underwater.
#define SF_ENVEXPLOSION_GENERIC_DAMAGE	0x00004000 // don't do BLAST damage

stock void CreateExplosionEx( const float[3] center, const float[3] angles, int owner,
	int magnitude, int radius = 0, int nSpawnFlags = SF_ENVEXPLOSION_NOSPARKS | SF_ENVEXPLOSION_NODLIGHTS | SF_ENVEXPLOSION_NOSMOKE,
	float flExplosionForce = 0.0, int inflictor = NULL_ENTITY, int iCustomDamageType = -1, int ignoredEntity = NULL_ENTITY, int ignoredClass = 0 )
{
	int explosion;
	if ( ( explosion = CreateEntityByName( "env_explosion" ) ) != -1 )
	{
		SetSpawnflags(explosion, nSpawnFlags);
		SetEntityRenderMode(explosion, RENDER_TRANSADD);
		
		SetEntProp(explosion, Prop_Data, "m_iMagnitude", magnitude);
		SetEntProp(explosion, Prop_Data, "m_iRadiusOverride", radius);
		
		// SetEntProp(explosion, Prop_Data, "m_iTeamNum", GetClientTeam(owner));
		SetEntPropEnt(explosion, Prop_Data, "m_hOwnerEntity", owner);

		DispatchSpawn(explosion);
		TeleportEntity(explosion, center, NULL_VECTOR, NULL_VECTOR);
		
		RequestFrame(TriggerExplosion, explosion);
		
		PrintToServer("m_sFireballSprite = %d", GetEntProp(explosion, Prop_Data, "m_sFireballSprite"));
		
		// SetEntProp(explosion, Prop_Data, "m_sFireballSprite", modelExplode, 2);
		// SetEntProp(explosion, Prop_Data, "m_spriteScale", 1, 4);
	}
	
	// static int explosion;
	// explosion = CreateEntityAt("env_explosion", center, angles);
	// if (!IsValidEdict(explosion)) {
		// return;
	// }
	
	// if (IsValidEdict(owner)) {
		// SetOwnerEntity(explosion, owner);
	// }
	
	// DispatchSpawn(explosion);
	
	// DeclareLocalPropOf(explosion, m_iMagnitude);
	// DeclareLocalPropOf(explosion, m_iRadiusOverride);
	// DeclareLocalPropOf(explosion, m_flDamageForce);
	
	// // SetEntData(explosion, m_iMagnitude, magnitude, 4, true);
	// // if (radius) {
		// // SetEntData(explosion, m_iRadiusOverride, radius, 4, true);
	// // }
	
	// // if (flExplosionForce != 0.0) {
		// // SetEntDataFloat(explosion, m_flDamageForce, flExplosionForce, true);
	// // }
	// SetSpawnflags(explosion, nSpawnFlags);
	
	// DispatchKeyValue(explosion, "iMagnitude", "100");
	// DispatchKeyValue(explosion, "iRadiusOverride", "200");
	// DispatchKeyValue(explosion, "DamageForce", "0.0");
	
	// DispatchSpawn(explosion);
	
	// SetEntityRenderMode(explosion, RENDER_TRANSADD);
	
	// DeclareLocalPropOf(explosion, m_hInflictor);
	// DeclareLocalPropOf(explosion, m_iCustomDamageType);
	// DeclareLocalPropOf(explosion, m_hEntityIgnore);
	// DeclareLocalPropOf(explosion, m_iClassIgnore);
	
	// if (IsValidEdict(inflictor)) {
		// SetEntDataEnt2(explosion, m_hInflictor, inflictor);
	// }
	
	// SetEntData(explosion, m_iCustomDamageType, iCustomDamageType, 4, true);
	
	// if (IsValidEdict(ignoredEntity)) {
		// SetEntDataEntHandle(explosion, m_hEntityIgnore, ignoredEntity);
	// }
	
	// SetEntData(explosion, m_iClassIgnore, ignoredClass, 4, true);
	
	// TeleportEntity(explosion, center, angles, NULL_VECTOR);
	
	// AcceptEntityInput(explosion, "Explode");
	
	// PrintToServer("exploded");
}

public void TriggerExplosion(int entity)
{
	AcceptEntityInput(entity, "Explode");
	//AcceptEntityInput(entity, "Kill");
}

stock void CreateExplosionB(const float[3] center, const float[3] angles, int owner,
		int magnitude, int radius = 0, bool doDamage = true, float flExplosionForce = 0.0, bool bSurfaceOnly = false, bool bSilent = false, int iCustomDamageType = -1)
{
	static int nFlags;
	nFlags = SF_ENVEXPLOSION_NOSPARKS | SF_ENVEXPLOSION_NODLIGHTS | SF_ENVEXPLOSION_NOSMOKE;
	if (!doDamage) {
		nFlags |= SF_ENVEXPLOSION_NODAMAGE;
	}

	if (bSurfaceOnly) {
		nFlags |= SF_ENVEXPLOSION_SURFACEONLY;
	}

	if (bSilent) {
		nFlags |= SF_ENVEXPLOSION_NOSOUND;
	}
	
	CreateExplosionEx(center, angles, owner, magnitude, radius, nFlags, flExplosionForce, NULL_ENTITY, iCustomDamageType);
}

#if defined __thelpers_entity_included
methodmap CEnvExplosion __nullable__ < CBaseEntity
{
	public CEnvExplosion( int entIndex = NULL_ENTITY )
	{
		return view_as< CEnvExplosion >( entIndex );
	}
	
	public void Explode( float delay = -1.0 )
	{
		if ( delay == -1.0 ) {
			this.AcceptInput( "Explode" );
			
			return;
		}
		
		this.AddOutput( "OnUser1 !self:Explode::%0.4f:1", delay );
		this.AcceptInput( "FireUser1" );
	}
	
	public void SetSprite( const char[] sprite )
	{
		this.SetPropString( Prop_Data, "m_iszFireballSprite", sprite );
	}
	
	property int Magnitude
	{
		public get()
		{
			return this.GetProp( Prop_Data, "m_iMagnitude" );
		}
		
		public set( int iMagnitude )
		{
			this.SetProp( Prop_Data, "m_iMagnitude", iMagnitude );
		}
	}
	
	property int Radius
	{
		public get()
		{
			return this.GetProp( Prop_Data, "m_iRadiusOverride" );
		}
		
		public set( int iRadiusOverride )
		{
			this.SetProp( Prop_Data, "m_iRadiusOverride", iRadiusOverride );
		}
	}
	
	property float DamageForce
	{
		public get()
		{
			return this.GetPropFloat( Prop_Data, "m_flDamageForce" );
		}
		
		public set( float DamageForce )
		{
			this.SetPropFloat( Prop_Data, "m_flDamageForce", DamageForce );
		}
	}
	
	property CBaseEntity Inflictor
	{
		public get()
		{
			return this.GetPropEnt( Prop_Data, "m_hInflictor" );
		}
		
		public set( CBaseEntity Inflictor )
		{
			this.SetPropEnt( Prop_Data, "m_hInflictor", Inflictor );
		}
	}
	
	property CBaseEntity ignoredEntity
	{
		public get()
		{
			return this.GetPropEnt( Prop_Data, "m_hEntityIgnore" );
		}
		
		public set( CBaseEntity ignoredEntity )
		{
			this.SetPropEnt( Prop_Data, "m_hEntityIgnore", ignoredEntity );
		}
	}
	
	property int iClassIgnore
	{
		public get()
		{
			return this.GetProp( Prop_Data, "m_iClassIgnore" );
		}
		
		public set( int iClassIgnore )
		{
			this.SetProp( Prop_Data, "m_iClassIgnore", iClassIgnore );
		}
	}
	
	property int CustomDamageType
	{
		public get()
		{
			return this.GetProp( Prop_Data, "m_iCustomDamageType" );
		}
		
		public set( int iCustomDamageType )
		{
			this.SetProp( Prop_Data, "m_iCustomDamageType", iCustomDamageType );
		}
	}
}
#endif

#define AMMO_AR 1
#define AMMO_PISTOL 4
#define AMMO_SMG 4
#define AMMO_357 5
#define AMMO_BUCKSHOT 7
#define AMMO_RPGROUND 8

stock bool IsAmmoTypeOf( int weapon, int type )
{
	DeclareLocalNetProp(CBaseCombatWeapon, m_iPrimaryAmmoType);
	
	return GetEntData( weapon, m_iPrimaryAmmoType ) == type;
}

stock bool IsSuitEquipped( int client )
{
	DeclareLocalPropOf( client, m_bWearingSuit );
	
	return view_as< bool >( GetEntData( client, m_bWearingSuit, 1 ) );
}

//
// Gravity
//
stock float GetWorldGravity()
{
	static ConVar sv_gravity;
	if ( !sv_gravity ) {
		sv_gravity = FindConVar( "sv_gravity" );
	}
	
	return sv_gravity.FloatValue;
}

stock float ScaleForGravity( float desiredGravity )
{
	float worldGravity = GetWorldGravity();
	
	return ( worldGravity > 0.0 ? ( desiredGravity / worldGravity ) : 0.0 );
}

//
// RadiusDamage wrapper
//
stock int RadiusDamage()
{
	
}

//
// Hud Element hiding flags
//
#define	HIDEHUD_WEAPONSELECTION		( 1<<0 )	// Hide ammo count & weapon selection
#define	HIDEHUD_FLASHLIGHT			( 1<<1 )
#define	HIDEHUD_ALL					( 1<<2 )
#define HIDEHUD_HEALTH				( 1<<3 )	// Hide health & armor / suit battery
#define HIDEHUD_PLAYERDEAD			( 1<<4 )	// Hide when local player's dead
#define HIDEHUD_NEEDSUIT			( 1<<5 )	// Hide when the local player doesn't have the HEV suit
#define HIDEHUD_MISCSTATUS			( 1<<6 )	// Hide miscellaneous status elements (trains, pickup history, death notices, etc)
#define HIDEHUD_CHAT				( 1<<7 )	// Hide all communication elements (saytext, voice icon, etc)
#define	HIDEHUD_CROSSHAIR			( 1<<8 )	// Hide crosshairs
#define	HIDEHUD_VEHICLE_CROSSHAIR	( 1<<9 )	// Hide vehicle crosshair
#define HIDEHUD_INVEHICLE			( 1<<10 )
#define HIDEHUD_BONUS_PROGRESS		( 1<<11 )	// Hide bonus progress display (for bonus map challenges)

#define HIDEHUD_BITCOUNT			12


static int m_iHideHUD = -2;
stock int GetHUDFlags( int client )
{
	DeclarePropOf( client, m_iHideHUD );
	
	return GetEntData( client, m_iHideHUD, 4 );
}

stock void SetHUDFlags( int client, int iHUDFlags )
{
	DeclarePropOf( client, m_iHideHUD );
	
	SetEntData( client, m_iHideHUD, iHUDFlags, 4 );
}

stock void AddHUDFlags( int client, int iHUDFlagMask )
{
	SetHUDFlags( client, GetEFlags( client ) | iHUDFlagMask );
}

stock void RemoveHUDFlags( int client, int iHUDFlagMask )
{
	SetHUDFlags( client, GetHUDFlags( client ) & ~iHUDFlagMask );
}

stock bool IsHUDFlagSet( int client, int iHUDFlagMask )
{
	return ( GetHUDFlags( client ) & iHUDFlagMask );
}
