#if defined _stocks_vector_included
 #endinput
#endif
#define _stocks_vector_included

#include <stocks/assert>
#include <stocks/entity>

#define VECTOR_COPY(%1,%2) do { (%1)[0] = (%2)[0]; (%1)[1] = (%2)[1]; (%1)[2] = (%2)[2]; } while( false && _null() )
	
// XYZ macro, for printf type functions - ex printf("%f %f %f",XYZ(myvector));
#define XYZ(%1) (%1)[0], (%1)[1], (%1)[2]

stock bool VectorsEqual( const float[] v1, const float[] v2 )
{
	return ( ( v1[0] == v2[0] ) &&
		     ( v1[1] == v2[1] ) &&
			 ( v1[2] == v2[2] ) );
}

stock float GetDistanceToEntity( const float[3] vec1, int entity )
{
	float vec2[3];
	GetAbsOrigin( entity, vec2 );
	
	return GetVectorDistance( vec1, vec2 );
}

stock float GetDistanceBetweenEntities( int entity1, int entity2 )
{
	float vec1[3], vec2[3];
	GetAbsOrigin( entity1, vec1 );
	GetAbsOrigin( entity2, vec2 );
	
	return GetVectorDistance( vec1, vec2 );
}
