#if defined _stocks_vector_included
 #endinput
#endif
#define _stocks_vector_included

stock float GetDistanceToEntity(const float[3] vec1, int entity)
{
	float vec2[3];
	GetEntPropVector(entity, Prop_Data, "m_vecAbsOrigin", vec2);
	
	return GetVectorDistance(vec1, vec2);
}
