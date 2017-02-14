#if defined _stocks_hierarchy_included
 #endinput
#endif
#define _stocks_hierarchy_included

//
// In development
//

#include <stocks/entity>
#include <stocks/property>

#define	MAX_EDICT_BITS				11
#define NUM_ENT_ENTRY_BITS		( MAX_EDICT_BITS + 1 )
#define NUM_ENT_ENTRIES			( 1 << NUM_ENT_ENTRY_BITS )
#define ENT_ENTRY_MASK			( NUM_ENT_ENTRIES - 1 )

#if !defined MAX_ENTITIES
	#define MAX_ENTITIES NUM_ENT_ENTRIES
#endif

/**
 * Loops through the children of given parent.
 *
 * @note It's safe to destroy child/parent entities inside the loop body.
 *		 Significantly faster than GetChildrenOf() as it acts like a loop instruction.
 *		 Usage is just as you'd use pseudo foreach statement: LoopThroughChildren(parent, child) { ..loop body.. }
 *		 Make sure that you've not put any spaces after 'child' variable name, e.g.:
 *		 LoopThroughChildren( parent, child ) { .. code .. } // BAD, will not work
 *		 LoopThroughChildren(parent, child) { .. code .. } // GOOD
 *		 The reason for that is to make nested LoopThroughChildren() possible, while Sourcemod compiler (as it gets for 08/16)
 *		 doesn't trim macropreprocessor function variable names.
 *
 * @param parent	Parent entity index.
 * @param child		Entity child index for new found child.
 * @noreturn
 */
#define LoopThroughChildren(%0,%1) for (\
	int %1__child_iteration__ = 0,\
	%1,\
	%1__child__ = GetFirstChild(%0);\
	IsValidEntity( ( %1 = %1__child__ ) ) &&\
	( ( %1__child__ = GetNextPeer( %1__child__ ) ) || true ) &&\
	%1__child_iteration__ ++ < MAX_ENTITIES; )

/**
 * Loops through all the children of given parent and their children and children of their children etc.
 *
 * @note It's safe to destroy child/parent entities inside the loop body.
 *		 Significantly faster than GetAllChildrenOf() as it acts like a loop instruction.
 *		 Usage is just as you'd use pseudo foreach statement: LoopThroughAllChildren(parent, child) { ..loop body.. }
 *		 Make sure that you've not put any spaces after 'child' variable name, e.g.:
 *		 LoopThroughAllChildren( parent, child ) { .. code .. } // BAD, will not work
 *		 LoopThroughAllChildren(parent, child) { .. code .. } // GOOD
 *		 The reason for that is to make nested LoopThroughAllChildren() possible, while Sourcemod compiler (as it gets for 08/16)
 *		 doesn't trim macropreprocessor function variable names.
 *
 * @param parent	Parent entity index.
 * @param child		Entity child index for new found child.
 * @noreturn
 */
#define LoopThroughAllChildren(%0,%1) for (\
	int %1__child_iteration__ = 0,\
	%1,\
	%1__parents__ = view_as< int >( CreateStack() ),\
	%1__child__ = GetFirstChild( %0 );\
	( ( view_as< ArrayStack >( %1__parents__ ) ).Empty != true || IsValidEntity( %1__child__ ) ) &&\
	%1__child_iteration__ < MAX_ENTITIES; ) if ( !IsValidEntity( ( %1 = %1__child__ ) )\
		&& ( ( %1__child__ = ( view_as< ArrayStack >( %1__parents__ ) ).Pop() ) || true )\
		&& ( ( %1__child__ = GetNextPeer( %1__child__ ) ) || true )\
		|| ( ( ++ %1__child_iteration__ ) && false )\
		|| ( ( view_as< ArrayStack >( %1__parents__ ) ).Push(%1__child__) && false )\
		|| ( ( %1__child__ = GetFirstChild( %1__child__ ) ) && false ) ) {}\
	else

static int m_hMoveChild = -2;
stock int GetFirstChild(int entity)
{
	Assert(IsValidEntity(entity));
	
	DeclarePropOf(entity, m_hMoveChild);
	
	return GetEntDataEnt2(entity, m_hMoveChild);
}

static int m_hMovePeer = -2;
stock int GetNextPeer(int entity)
{
	Assert(IsValidEntity(entity));
	
	DeclarePropOf(entity, m_hMovePeer);
	
	return GetEntDataEnt2(entity, m_hMovePeer);
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

stock void GetAllChildrenOf(int parent, OnEntityFound callback, any &container = 0, any &data = 0)
{
	int maxiterations = NUM_ENT_ENTRIES;
	GetAllChildren_r(GetFirstChild(parent), callback, maxiterations, container, data);
}

stock void GetChildrenOf(int parent, OnEntityFound callback, any &container = 0, any &data = 0)
{
	int maxiterations = NUM_ENT_ENTRIES;
	bool result;
	int child = GetFirstChild(parent);
	while (IsValidEntity(child) && maxiterations --) {
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
	int maxiterations = NUM_ENT_ENTRIES;
	entity = GetParent(entity);
	while (IsValidEntity(entity) && maxiterations --) {
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
	int maxiterations = NUM_ENT_ENTRIES;
	int n = 1;
	entity = GetParent(entity);
	while (IsValidEntity(entity) && maxiterations --) {
		if (n++ == nthdepth) {
			return entity;
		}
		
		entity = GetParent(entity);
	}
	
	return NULL_ENTITY;
}

stock int IsParentOf(int parent, int entity)
{
	int maxiterations = NUM_ENT_ENTRIES;
	int depth = 1;
	entity = GetParent(entity);
	while (IsValidEntity(entity) && maxiterations --) {
		if (parent == entity) {
			return depth;
		}
		
		entity = GetParent(entity);
		depth++;
	}
	
	return 0;
}
