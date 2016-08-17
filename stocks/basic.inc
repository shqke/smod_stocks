#if defined _stocks_basic_included
 #endinput
#endif
#define _stocks_basic_included

#define Clear(%0) Clear_r(view_as<Handle>(%0))
stock void Clear_r(Handle &hndl)
{
	if (hndl == null) {
		return;
	}
	
	CloseHandle(hndl);
	hndl = null;
}
