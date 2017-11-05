#if defined _stocks_buttons_included
 #endinput
#endif
#define _stocks_buttons_included

#include <stocks/assert>
#include <stocks/player>

// unsafe> most of functions are being used with entity "player_speedmod" (HL2MP)
stock static int m_afButtonDisabled = -2;
stock int GetDisabledButtons(int client)
{
	Assert(IsClient(client));
	
	DeclarePropOf(client, m_afButtonDisabled);
	
	return GetEntData(client, m_afButtonDisabled);
}

stock void SetDisabledButtons(int client, int buttons)
{
	Assert(IsClient(client));
	
	DeclarePropOf(client, m_afButtonDisabled);
	
	SetEntData(client, m_afButtonDisabled, buttons);
}

stock void DisableButtons(int client, int buttons)
{
	Assert(IsClient(client));
	
	SetDisabledButtons(client, GetDisabledButtons(client) | buttons);
}

stock void EnableButtons(int client, int buttons)
{
	Assert(IsClient(client));
	
	SetDisabledButtons(client, GetDisabledButtons(client) & ~buttons);
}

stock static int m_afButtonForced = -2;
stock int GetForcedButtons(int client)
{
	Assert(IsClient(client));
	
	DeclarePropOf(client, m_afButtonForced);
	
	return GetEntData(client, m_afButtonForced);
}

stock void SetForcedButtons(int client, int buttons)
{
	Assert(IsClient(client));
	
	DeclarePropOf(client, m_afButtonForced);
	
	SetEntData(client, m_afButtonForced, buttons);
}

stock void ForceButtons(int client, int buttons)
{
	Assert(IsClient(client));
	
	SetForcedButtons(client, GetForcedButtons(client) | buttons);
}

stock void UnforceButtons(int client, int buttons)
{
	Assert(IsClient(client));
	
	SetForcedButtons(client, GetForcedButtons(client) & ~buttons);
}

stock static int m_afButtonPressed = -2;
stock int IsButtonPressed(int client, int buttons)
{
	Assert(IsClient(client));
	
	DeclarePropOf(client, m_afButtonPressed);
	
	return GetEntData(client, m_afButtonPressed) & buttons;
}

stock int UnpressButtons(int client, int buttons)
{
	Assert(IsClient(client));
	
	DeclarePropOf(client, m_afButtonPressed);
	
	SetEntData(client, m_afButtonPressed, GetEntData(client, m_afButtonPressed) & ~buttons);
}

stock int PressButtons(int client, int buttons)
{
	Assert(IsClient(client));
	
	DeclarePropOf(client, m_afButtonPressed);
	
	SetEntData(client, m_afButtonPressed, GetEntData(client, m_afButtonPressed) | buttons);
}

stock static int m_afButtonReleased = -2;
stock int IsButtonReleased(int client, int buttons)
{
	Assert(IsClient(client));
	
	DeclarePropOf(client, m_afButtonReleased);
	
	return GetEntData(client, m_afButtonReleased) & buttons;
}

stock static int m_afButtonLast = -2;
stock int GetLastButtons(int client)
{
	Assert(IsClient(client));
	
	DeclarePropOf(client, m_afButtonLast);
	
	return GetEntData(client, m_afButtonLast);
}
