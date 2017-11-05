#if defined _stocks_vehicle_included
 #endinput
#endif
#define _stocks_vehicle_included

#include <stocks/assert>
#include <stocks/entity>
#include <stocks/player>
#include <stocks/property>

#define IsClientInVehicle(%0) ( GetClientVehicle( %0 ) != NULL_ENTITY )

stock static int m_hVehicle = -2;
stock int GetClientVehicle( int client )
{
	Assert( IsClientAndAlive( client ) );
	
	DeclarePropOf( client, m_hVehicle );
	
	return GetEntDataEnt2( client, m_hVehicle );
}

stock static int m_pServerVehicle = -2;
stock Address GetVehicleInterfaceAddress( int vehicle )
{
	Assert( IsValidEdict( vehicle ) );
	
	DeclarePropOf( vehicle, m_pServerVehicle );
	
	return view_as< Address >( LoadFromAddress( GetEntityAddress( vehicle ) + view_as< Address >( m_pServerVehicle ), NumberType_Int32 ) );
}

stock bool PutPlayerInVehicle(int client, int vehicle)
{
	Assert( IsClientAndAlive( client ) );
	Assert( IsValidEdict( vehicle ) );
	
	DeclarePropOf( client, m_hVehicle );
	
	SetEntDataEnt2( client, m_hVehicle, vehicle, false );
	
	int maxSeats = GetMaxSeats( vehicle );
	for ( int i = 0; i < maxSeats; i++ ) {
		FakeClientCommand( client, "vehicleRole %d", i );
		if ( GetParent( client ) == vehicle ) {
			return true;
		}
	}
	
	SetEntDataEnt2( client, m_hVehicle, NULL_ENTITY, true );
	
	return false;
}

stock static int m_bLocked = -2;
stock bool IsVehicleLocked( int vehicle )
{
	Assert( IsValidEdict( vehicle ) );
	
	DeclarePropOf( vehicle, m_bLocked );
	
	return view_as< bool >( GetEntData( vehicle, m_bLocked, 1 ) );
}

stock void SetVehicleLock( int vehicle, bool bIsLocked = true )
{
	Assert( IsValidEdict( vehicle ) );
	
	DeclarePropOf( vehicle, m_bLocked );
	
	SetEntData( vehicle, m_bLocked, bIsLocked, 1, true );
}

stock int GetVehicleScript( int vehicle, char[] scriptname, int length )
{
	Assert( IsValidEdict( vehicle ) );
	
	return GetEntPropString( vehicle, Prop_Data, "m_vehicleScript", scriptname, length );
}
