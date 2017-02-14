#if defined _stocks_property_included
	#endinput
#endif
#define _stocks_property_included

#include <stocks/assert>

// Find entity property offset by net/class name
#define DeclareProp(%0,%1) if ( %1 != -2 ) { } else %1 = FindEntProperty( #%0, #%1 )
		
#define DeclareNetProp(%0,%1) if ( %1 != -2 ) { } else %1 = FindEntProperty( #%0, #%1, Prop_Send )
		
#define DeclareLocalProp(%0,%1) static int %1 = -2;\
	if ( %1 != -2 ) { } else %1 = FindEntProperty( #%0, #%1 )

#define DeclareLocalNetProp(%0,%1) static int %1 = -2;\
	if ( %1 != -2 ) { } else %1 = FindEntProperty( #%0, #%1, Prop_Send )

// Find entity property offset by entity ID
#define DeclarePropOf(%0,%1) if ( %1 != -2 ) { } else %1 = FindEntPropertyOf( %0, #%1 )

#define DeclareNetPropOf(%0,%1) if ( %1 != -2 ) { } else %1 = FindEntPropertyOf( %0, #%1, Prop_Send )
		
#define DeclareLocalPropOf(%0,%1) static int %1 = -2;\
	if ( %1 != -2 ) { } else %1 = FindEntPropertyOf( %0, #%1 )

#define DeclareLocalNetPropOf(%0,%1) static int %1 = -2;\
	if ( %1 != -2 ) { } else %1 = FindEntPropertyOf( %0, #%1, Prop_Send )

// Advanced properties
#define sizeofprop(%0) %0__sizeofprop_
#define typeofprop(%0) %0__typeofprop_

#define DeclareLocalPropEx(%0,%1) static int %1 = -2, sizeofprop(%1) = 1;\
	static PropFieldType typeofprop(%1) = PropField_Unsupported;\
	if ( %1 != -2 ) { } else %1 = FindEntPropertyEx( #%0, #%1 )

#define DeclareLocalNetPropEx(%0,%1) static int %1 = -2, sizeofprop(%1) = 1;\
	static PropFieldType typeofprop(%1) = PropField_Unsupported;\
	if ( %1 != -2 ) { } else %1 = FindEntPropertyEx( #%0 , #%1, Prop_Send )

#define DeclareLocalPropOfEx(%0,%1) static int %1 = -2, sizeofprop(%1) = 1;\
	static PropFieldType typeofprop(%1) = PropField_Unsupported;\
	if ( %1 != -2 ) { } else %1 = FindEntPropertyOfEx( %0, #%1 )

#define DeclareLocalNetPropOfEx(%0,%1) static int %1 = -2, sizeofprop(%1) = 1;\
	static PropFieldType typeofprop(%1) = PropField_Unsupported;\
	if ( %1 != -2 ) { } else %1 = FindEntPropertyOfEx( %0, #%1, Prop_Send )

//
// Declaration
//
#include <stocks/__property_sub>

#define PROPERTY_INC_DEF_ADV
#include <stocks/__property_sub>
