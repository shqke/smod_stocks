#if defined _stocks_player_included
 #endinput
#endif
#define _stocks_player_included

#define LoopThroughPlayers(%0) for (int %0 = 1; %0 <= MaxClients + 1; %0++)

stock bool IsValidClient(int client)
{
	if (client > 0 && client <= MaxClients && IsClientConnected(client)) {
		return true;
	}
	
	return false;
}

stock bool IsClientInVehicle(int client)
{
	int vehicle = GetEntPropEnt(client, Prop_Send, "m_hVehicle");
	if (vehicle > 0 && IsValidEntity(vehicle)) {
		return true;
	}
	
	return false;
}

stock int GetClientVehicle(int client)
{
	return GetEntPropEnt(client, Prop_Send, "m_hVehicle");
}

stock void MoveToPlayer(int client, int target) {
	float pos[3], vel[3], ang[3];
	GetClientAbsAngles(target, ang);
	GetEntPropVector(target, Prop_Send, "m_vecOrigin", pos);
	GetEntPropVector(target, Prop_Data, "m_vecVelocity", vel);
	
	SetEntProp(client, Prop_Send, "m_bDucking", GetEntProp(target, Prop_Data, "m_bDucked", 1), 1);
	
	TeleportEntity(client, pos, ang, vel);
}

stock bool IsAdmin(int client)
{
	AdminId admin = GetUserAdmin(client);
	
	return admin.HasFlag(Admin_Generic);
}

stock void SpawnClient(int client)
{
	if(IsPlayerAlive(client)) {
		return;
	}
	
	DispatchSpawn(client);
	
	// SetEntPropFloat(client, Prop_Data, "m_flDeathTime", GetGameTime() - 200.0);
}

stock void StartScene(int client, const char[] scenefile)
{
	char modelname[256], buffer[256];
	GetClientModel(client, modelname, sizeof(modelname));
	strcopy(buffer, sizeof(buffer), scenefile);
	ReplaceString(buffer, sizeof(buffer), "$gender", StrContains(modelname, "female") != -1 ? "female" : "male");
	
	int scene = CreateEntityByName("instanced_scripted_scene");
	SetEntPropString(scene, Prop_Data, "m_iszSceneFile", buffer);
	SetEntPropEnt(scene, Prop_Data, "m_hOwner", client);
	DispatchSpawn(scene);
	
	ActivateEntity(scene);
	AcceptEntityInput(scene, "Start", client, client);
}
