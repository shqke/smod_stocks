#if defined _stocks_timers_included
 #endinput
#endif
#define _stocks_timers_included

stock void ClearTimer(Handle &timer)
{
	if (timer == null) {
		return;
	}
	
	KillTimer(timer);
	timer = null;
}

stock void RecreateTimer(Handle &timer, float interval, Timer func, any data = INVALID_HANDLE, int flags = 0)
{
	ClearTimer(timer);
	
	timer = CreateTimer(interval, func, data, flags);
}
