#if defined _stocks_admin_included
 #endinput
#endif
#define _stocks_admin_included

#include <stocks/player>

stock bool IsAdmin( int client )
{
	Assert( IsClientAndConnected( client ) );
	
	return GetUserFlagBits( client ) & ADMFLAG_GENERIC;
}

stock void PrintToAllAdminChat( const char[] message, any ... )
{
	static char buffer[1024];
	
	LoopThroughPlayers( client ) {
		if ( IsFakeClient( client ) || !IsAdmin( client ) ) {
			continue;
		}
		
		SetGlobalTransTarget( client );
		VFormat( buffer, sizeof( buffer ), message, 2 );
		PrintToChat( client, buffer );
	}
}

#if defined _colors_included
stock void CPrintToAllAdminChat( const char[] message, any ... )
{
	static char buffer[1024], buffer2[1024];
	
	LoopThroughPlayers( client ) {
		if ( IsFakeClient( client ) || !IsAdmin( client ) ) {
			continue;
		}
		
		SetGlobalTransTarget( client );
		Format( buffer, sizeof( buffer ), "\x01%s", message );
		VFormat( buffer2, sizeof( buffer2 ), buffer, 2);
		CReplaceColorCodes( buffer2 );
		CSendMessage( client, buffer2 );
	}
}
#endif
