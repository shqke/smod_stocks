#if defined _stocks_assert_included
 #endinput
#endif
#define _stocks_assert_included

stock void _null() {}

// As cheap as possible, no warnings
#define NULL_EXPRESSION do {} while( false && _null() )

#if !defined NDEBUG
	// Following definition will allow to use Assert(EXPRESSION) as a normal function call anywhere, e.g.:
	// Assert(EXPRESSION);
	#define Assert(%0) if ( !( %0 ) ) do {\
			PrintToServer("Assertion failed in expression \"%s\"", #%0);\
			SetFailState("Assertion failed in expression \"%s\"", #%0);\
		} while( false && _null() )

	#define AssertEx(%0) if ( !( %0 ) ) do {\
			PrintToServer("Assertion failed in expression \"%s\"", #%0);\
			ThrowError("Assertion failed in expression \"%s\"", #%0);\
		} while( false && _null() )
#else
	#define Assert(%0) NULL_EXPRESSION
	#define AssertEx(%0) NULL_EXPRESSION
#endif
