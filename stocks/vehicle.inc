#if defined _stocks_vehicle_included
 #endinput
#endif
#define _stocks_vehicle_included

#include "entity"

stock Address GetVehicleInterfaceAddress(int vehicle)
{
	if (!IsValidEdict(vehicle)) {
		return Address_Null;
	}
	
	int offset = FindDataMapInfo(vehicle, "m_pServerVehicle");
	if (offset < 0) {
		return Address_Null;
	}
	
	Address vehaddr = GetEntityAddress(vehicle);
	if (vehaddr == Address_Null) {
		return Address_Null;
	}
	
	return view_as<Address>(LoadFromAddress(vehaddr + view_as<Address>(offset), NumberType_Int32));
}

stock void PutPlayerInVehicle(int client, int vehicle)
{
	SetEntPropEnt(client, Prop_Data, "m_hVehicle", vehicle);
	
	for (int i = 0; i < 10; i++) {
		FakeClientCommand(client, "vehicleRole %d", i);
		if (GetParent(client) == vehicle) {
			return;
		}
	}
	
	SetEntPropEnt(client, Prop_Data, "m_hVehicle", -1);
}
