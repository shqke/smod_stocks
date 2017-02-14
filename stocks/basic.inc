#if defined _stocks_basic_included
 #endinput
#endif
#define _stocks_basic_included

#define GetHandleSerial(%1) ( view_as<int>( %1 ) >> 16 )
#define GetHandleIndex(%1) ( view_as<int>( %1 ) & 0x0000FFFF )

#define MakePair(%1,%2) ( view_as<int>(%1) | ( view_as<int>(%2) << 16 ) )

stock int Abs( int value )
{
	static int mask;
	mask = value >> 31;
	
	return ( value + mask ) ^ mask;
}

// Clamp the value according to min/max inclusive
stock any Clamp( any value, any min, any max )
{
	if ( value < min ) {
		return min;
	}
	
	if ( value > max ) {
		return max;
	}
	
	return value;
}

stock void ClearHandle( any &hndl )
{
	if ( !view_as< Handle >( hndl ) ) {
		return;
	}
	
	CloseHandle( view_as< Handle >( hndl ));
	view_as< Handle >( hndl ) = null;
}
