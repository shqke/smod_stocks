#if defined _stocks_keyhints_included
 #endinput
#endif
#define _stocks_keyhints_included

#include <stocks/assert>

stock void PrintKeyHintText(int client, const char[] fmt, any ...)
{
	Assert(IsClient(client));
	
	static char message[1024];
	VFormat(message, 1024, fmt, 3);
	
	Handle usermsg = StartMessageOne("KeyHintText", client, USERMSG_RELIABLE);
	
	BfWrite bf = UserMessageToBfWrite(usermsg);
	bf.WriteByte(1);
	bf.WriteString(message);
	
	EndMessage();
}

stock void PrintKeyHintTextAll(const char[] fmt, any ...)
{
	static char message[1024];
	VFormat(message, 1024, fmt, 2);
	
	Handle usermsg = StartMessageAll("KeyHintText", USERMSG_RELIABLE);
	
	BfWrite bf = UserMessageToBfWrite(msg);
	bf.WriteByte(1);
	bf.WriteString(message);

	EndMessage();
}

stock void InitKeyHintTimer()
{
	// m_iAltFireHudHintCount++;
	// m_bAltFireHudHintDisplayed = true;
	// m_flHudHintMinDisplayTime = gpGlobals->curtime + MIN_HUDHINT_DISPLAY_TIME;
	
}
