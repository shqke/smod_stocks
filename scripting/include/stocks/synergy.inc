#if defined _stocks_game_synergy_included
 #endinput
#endif
#define _stocks_game_synergy_included

#include <stocks/entity>
#include <stocks/player>
#include <stocks/property>

#define SEAT_UNDEFINED -1
#define SEAT_DRIVER 0

stock static int m_hPassengers = -2;
stock static int m_hPlayer = -2;
stock int GetClientOfSeat( int vehicle, int seat )
{
	Assert( IsValidEdict( vehicle ) );
	
	DeclareNetProp( CPropVehicleDriveable, m_hPassengers );
	DeclareNetProp( CPropVehicleDriveable, m_hPlayer );
	
	if ( seat == SEAT_DRIVER ) {
		return GetEntDataEnt2( vehicle, m_hPlayer );
	}
	
	int maxSeats = GetMaxSeats( vehicle );
	if ( seat >= maxSeats ) {
		return NULL_ENTITY;
	}
	
	int client = GetEntDataEnt2( vehicle, m_hPassengers + seat * 4 );
	if ( client > 0 && client <= MaxClients && IsClientInGame( client ) ) {
		return client;
	}
	
	return NULL_ENTITY;
}

// Unable to receive vehicle id from output "PlayerOn" (m_hVehicle and m_pParent is NULL for client) - so we put vehicle id as argument here
stock int GetSeatOfClient( int vehicle, int client )
{
	Assert( IsValidEdict( vehicle ) );
	Assert( IsClientAndAlive( client ) );
	
	DeclareNetProp( CPropVehicleDriveable, m_hPassengers );
	DeclareNetProp( CPropVehicleDriveable, m_hPlayer );
	
	if ( GetEntDataEnt2( vehicle, m_hPlayer ) == client ) {
		return SEAT_DRIVER;
	}
	
	int maxSeats = GetMaxSeats(vehicle);
	for ( int i = 1; i < maxSeats; i++ ) {
		if ( client == GetEntDataEnt2( vehicle, m_hPassengers + i * 4 ) ) {
			return i;
		}
	}
	
	return SEAT_UNDEFINED;
}

stock static int m_iPassengerCount = -2;
stock int GetMaxSeats( int vehicle )
{
	Assert( IsValidEdict( vehicle ) );
	
	DeclareNetProp( CPropVehicleDriveable, m_iPassengerCount );
	
	return GetEntData( vehicle, m_iPassengerCount, 4 );
}

stock bool IsVehicleGotFreeSeat( int vehicle )
{
	int maxSeats = GetMaxSeats( vehicle );
	for ( int i = 0; i < maxSeats; i++ ) {
		if ( GetClientOfSeat( vehicle, i ) == NULL_ENTITY ) {
			return true;
		}
	}
	
	return false;
}
