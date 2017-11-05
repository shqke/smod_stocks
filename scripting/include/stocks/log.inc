#if defined _stocks_log_included
 #endinput
#endif
#define _stocks_log_included

#include <stocks/assert>

#if !defined NLOG
	#define DoLogMessage(%1) LogMessage(%1)
	#define DoLogAction(%1) LogAction(%1)
	#define DoLogError(%1) LogError(%1)
#else
	#define DoLogMessage(%1) NULL_EXPRESSION
	#define DoLogAction(%1) NULL_EXPRESSION
	#define DoLogError(%1) NULL_EXPRESSION
#endif
