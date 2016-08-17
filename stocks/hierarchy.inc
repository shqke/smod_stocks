#if defined _stocks_hierarchy_included
 #endinput
#endif
#define _stocks_hierarchy_included

#include "entity"

#define	MAX_EDICT_BITS				11
#define NUM_ENT_ENTRY_BITS		(MAX_EDICT_BITS + 1)
#define NUM_ENT_ENTRIES			(1 << NUM_ENT_ENTRY_BITS)
#define ENT_ENTRY_MASK			(NUM_ENT_ENTRIES - 1)

stock static int GetEntryIndex(int entity, const char[] prop_name)
{
	return GetEntData(entity, FindDataMapInfo(entity, prop_name)) & ENT_ENTRY_MASK;
}

stock int GetFirstChild(int entity)
{
	int child = GetEntryIndex(entity, "m_hMoveChild");
	if (!IsValidEntity(child)) {
		return -1;
	}
	
	if (child == entity) {
		return -1;
	}
	
	return child;
}

stock static int GetNextPeer(int entity)
{
	int peer = GetEntryIndex(entity, "m_hMovePeer");
	if (!IsValidEntity(peer)) {
		return -1;
	}
	
	if (peer == entity) {
		return -1;
	}
	
	return peer;
}

typedef OnEntityFound = function bool (int entity, any &container, any &data);
stock static bool GetAllChildren_r(int entity, OnEntityFound callback, int &maxiterations, any &container, any &data)
{
	bool result;
	
	while (IsValidEntity(entity)) {
		if (!(maxiterations --)) {
			return true;
		}
		
		Call_StartFunction(null, callback);
		
		Call_PushCell(entity);
		Call_PushCellRef(container);
		Call_PushCellRef(data);
		
		Call_Finish(result);
		if (result) {
			return true;
		}
		
		if (GetAllChildren_r(GetFirstChild(entity), callback, maxiterations, container, data)) {
			return true;
		}
		
		entity = GetNextPeer(entity);
	}
	
	return false;
}

stock void GetAllChildrens(int parent, OnEntityFound callback, any &container = 0, any &data = 0)
{
	int maxiterations = GetMaxEntities() * 2;
	GetAllChildren_r(GetFirstChild(parent), callback, maxiterations, container, data);
}

stock void GetChildrens(int parent, OnEntityFound callback, any &container = 0, any &data = 0)
{
	int maxiterations = GetMaxEntities() * 2;
	bool result;
	int child = GetFirstChild(parent);
	while (IsValidEntity(child)) {
		if (!(maxiterations --)) {
			return;
		}
		
		Call_StartFunction(null, callback);
		
		Call_PushCell(child);
		Call_PushCellRef(container);
		Call_PushCellRef(data);
		
		Call_Finish(result);
		if (result) {
			return;
		}
		
		child = GetNextPeer(child);
	}
}

stock void GetParents(int entity, OnEntityFound callback, any &container = 0, any &data = 0)
{
	bool result;
	int maxiterations = GetMaxEntities() * 2;
	entity = GetParent(entity);
	while (IsValidEntity(entity)) {
		if (!(maxiterations --)) {
			return;
		}
		
		Call_StartFunction(null, callback);
		
		Call_PushCell(entity);
		Call_PushCellRef(container);
		Call_PushCellRef(data);
		
		Call_Finish(result);
		if (result) {
			return;
		}
		
		entity = GetParent(entity);
	}
}

stock int GetNthParent(int entity, int nthdepth = 1)
{
	int maxiterations = GetMaxEntities() * 2;
	int n = 1;
	entity = GetParent(entity);
	while (IsValidEntity(entity)) {
		if (!(maxiterations --)) {
			return -1;
		}
		
		if (n++ == nthdepth) {
			return entity;
		}
		
		entity = GetParent(entity);
	}
	
	return -1;
}
