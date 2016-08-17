#if defined _stocks_halflife_included
 #endinput
#endif
#define _stocks_halflife_included

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

stock void DissolveEntity(int entity, DissolveType_t type = ENTITY_DISSOLVE_NORMAL, int magnitude = 0)
{
	int dissolver = CreateEntityByName("env_entity_dissolver");
	if (!IsValidEntity(dissolver)) {
		return;
	}
	
	SetEntProp(dissolver, Prop_Data, "m_nDissolveType", type);
	SetEntProp(dissolver, Prop_Data, "m_nMagnitude", magnitude);
	
	SetVariantString("!activator");
	AcceptEntityInput(dissolver, "Dissolve", entity);
	AcceptEntityInput(dissolver, "Kill");
}
