#if defined PROPERTY_INC_DEF_ADV_INCLUDED
	#endinput
#else
	
#if defined PROPERTY_INC_DEF_ADV
	#define PROPERTY_INC_DEF_ADV_INCLUDED
#else

#if !defined Bits2Bytes
	// unsafe>
	#define Bits2Bytes(%0) ( ( ( 1 << ( %0 - 1 ) ) & 0xffff0000 ) ? 4 : RoundToCeil( float( %0 ) / 8 ) )
#endif

#if defined PROPERTY_INC_DEF_ADV
stock int FindEntPropertyEx( const char[] className, const char[] propertyName,
	PropType propType = Prop_Data, PropFieldType &fieldType = PropField_Unsupported, int &propSize = 0 )
#else
stock int FindEntProperty( const char[] className, const char[] propertyName, PropType fieldType = Prop_Data )
#endif
{
	static char fixedClass[128], fixedName[128];
	static int offset, entity;
#if defined PROPERTY_INC_DEF_ADV
	static int num_bits;
#else
	
	strcopy( fixedClass, sizeof( fixedClass ), className );
	TrimString( fixedClass );
	
	strcopy( fixedName, sizeof( fixedName ),  propertyName );
	TrimString( fixedName );
	
	if ( fieldType == Prop_Send ) {
#if defined PROPERTY_INC_DEF_ADV
		offset = FindSendPropInfo( fixedClass, fixedName, fieldType, num_bits );
#else
		offset = FindSendPropInfo( fixedClass, fixedName );
#endif
	} else {
		entity = CreateEntityByName( fixedClass );
		if ( !IsValidEntity( entity ) ) {
			SetFailState( "Unable to retrieve a property offset \"%s\" - wrong classname \"%s\"", fixedName, fixedClass );
		}
		
		AcceptEntityInput( entity, "Kill" );
		
#if defined PROPERTY_INC_DEF_ADV
		offset = FindDataMapInfo( entity, fixedName, fieldType, num_bits );
#else
		offset = FindDataMapInfo( entity, fixedName );
#endif
	}
	
	if ( offset == -1 ) {
		ThrowError( "Unable to look up an offset of%s property \"%s::%s\"", fieldType == Prop_Send ? " networked" : "", fixedClass, fixedName );
	}
#if defined PROPERTY_INC_DEF_ADV
	else {
		propSize = Bits2Bytes( num_bits );
	}
#endif
	
	return offset;
}

#if defined PROPERTY_INC_DEF_ADV
stock int FindEntPropertyOfEx( int entity, const char[] propertyName,
	PropType fieldType = Prop_Data, PropFieldType &fieldType = PropField_Unsupported, int &propSize = 0 )
#else
stock int FindEntPropertyOf( int entity, const char[] propertyName, PropType fieldType = Prop_Data )
#endif
{
	static char fixedClass[128], fixedName[128];
	static int offset;
#if defined PROPERTY_INC_DEF_ADV
	static int num_bits;
#else
	
	strcopy( fixedName, sizeof( fixedName ), propertyName );
	TrimString( fixedName );
	
	if ( fieldType == Prop_Data ) {
#if defined PROPERTY_INC_DEF_ADV
		offset = FindDataMapInfo( entity, fixedName, fieldType, num_bits );
#else
		offset = FindDataMapInfo( entity, fixedName );
#endif
	} else {
		if ( !IsValidEdict( entity ) || !GetEntityNetClass( entity, fixedClass, sizeof( fixedClass ) ) ) {
			SetFailState( "Unable to look up a property offset \"%s\" - entity #%d is not networkable", fixedName, entity );
		}
		
#if defined PROPERTY_INC_DEF_ADV
		offset = FindSendPropInfo( fixedClass, fixedName, fieldType, num_bits );
#else
		offset = FindSendPropInfo( fixedClass, fixedName );
#endif
	}
	
	if ( offset == -1 ) {
		ThrowError( "Unable to look up an offset of%s property \"%s\" (entity #%d)", fieldType == Prop_Send ? " networked" : "", fixedName, entity );
	}
#if defined PROPERTY_INC_DEF_ADV
	else {
		propSize = Bits2Bytes( num_bits );
	}
#endif
	
	return offset;
}
