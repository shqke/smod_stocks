#if defined _stocks_timers_included
 #endinput
#endif
#define _stocks_timers_included

#include <sourcemod>
#include <sdktools>

#define ST_EPS 0.001

stock void ClearTimer(Handle &timer)
{
	if (timer == null) return;
	KillTimer(timer);
	timer = null;
}

stock void RecreateTimer(Handle &timer, float interval, Timer func, any data = 0, int flags = 0)
{
	ClearTimer(timer);
	
	timer = CreateTimer(interval, func, data, flags);
}

static stock Handle __NetworkChange = null;
static stock void NetworkTimerChanged(int entity, int offset)
{
	if (!__NetworkChange) {
		Handle gc = LoadGameConfigFile("stocks.games");
		if (!gc) return;
		
		StartPrepSDKCall(SDKCall_Raw);
		PrepSDKCall_SetVirtual(GameConfGetOffset(gc, "NetworkChange"));
		if (!( __NetworkChange = EndPrepSDKCall() )) return;
		
		delete gc;
	}
	
	SDKCall(__NetworkChange, GetEntityAddress(entity) + view_as<Address>( offset ));
}

// Internal game timers
enum SimpleSimTimer
{
	InvalidTimer = 0,
};

enum
{
	SST_Next = 0,
	ST_Interval = 0,
	IT_Timestamp = 1,
	CT_Duration = 1,
	CT_Timestamp = 2,
};

methodmap SimTimerBase
{
	public SimTimerBase(int offset) {
		return view_as<SimTimerBase>( offset );
	}
	
	public static SimTimerBase FromGCOffset(Handle gc, const char[] key) {
		return view_as<SimTimerBase>( GameConfGetOffset(gc, key) );
	}
	
	public static SimTimerBase FromEntProp(int entity, PropType type, const char[] prop) {
		int offset = -1;
		if (IsValidEntity(entity)) {
			if (type == Prop_Data) {
				offset = FindDataMapInfo(entity, prop);
			}
			else {
				static char netclass[64];
				GetEntityNetClass(entity, netclass, sizeof(netclass));
				offset = FindSendPropInfo(netclass, prop);
			}
		}
		
		return view_as<SimTimerBase>( offset );
	}
	
	public float Get(int entity, int index) {
		return GetEntDataFloat(entity, view_as<int>( this ) + index * 4);
	}
	
	public void Set(int entity, int index, float value, bool changeState = false) {
		if (changeState) NetworkTimerChanged(entity, view_as<int>( this ));
		SetEntDataFloat(entity, view_as<int>( this ) + index * 4, value);
	}
};

// Orange box
methodmap SimpleSimTimer < SimTimerBase
{
	public SimpleSimTimer(int offset) {
		return view_as<SimpleSimTimer>( offset );
	}
	
	public static SimpleSimTimer FromGCOffset(Handle gc, const char[] key) {
		return view_as<SimpleSimTimer>( SimTimerBase.FromGCOffset(gc, key) );
	}
	
	public static SimpleSimTimer FromEntProp(int entity, PropType type, const char[] prop) {
		return view_as<SimpleSimTimer>( SimTimerBase.FromEntProp(entity, type, prop) );
	}
	
	public float GetNext(int entity) {
		return this.Get(entity, SST_Next);
	}
	
	public void SetNext(int entity, float value) {
		return this.Set(entity, SST_Next, value);
	}
	
	public float Now() {
		return GetGameTime();
	}
	
	public void Force(int entity) {
		this.Set(entity, SST_Next, -1.0);
	}
	
	public bool Expired(int entity) {
		return GetGameTime() - this.Get(entity, SST_Next) > -ST_EPS;
	}
	
	public void Delay(int entity, float interval) {
		this.Set(entity, SST_Next, this.Get(entity, SST_Next) + interval);
	}
	
	public float GetRemaining(int entity) {
		float result = this.Get(entity, SST_Next) - GetGameTime();
		if (result < 0.0) return 0.0;
		return result;
	}
};

methodmap SimTimer < SimpleSimTimer
{
	public SimTimer(int offset) {
		return view_as<SimTimer>( offset );
	}
	
	public static SimTimer FromGCOffset(Handle gc, const char[] key) {
		return view_as<SimTimer>( SimTimerBase.FromGCOffset(gc, key) );
	}
	
	public static SimTimer FromEntProp(int entity, PropType type, const char[] prop) {
		return view_as<SimTimer>( SimTimerBase.FromEntProp(entity, type, prop) );
	}
	
	public void SetInterval(int entity, float interval, bool startExpired = true) {
		this.Set(entity, ST_Interval, interval);
		this.Set(entity, SST_Next, ( startExpired ? -1.0 : GetGameTime() + interval ));
	}
	
	public float GetInterval(int entity) {
		return this.Get(entity, ST_Interval);
	}
	
	public void Reset(int entity, float interval = -1.0) {
		if (interval == -1.0) {
			this.Set(entity, SST_Next, GetGameTime() + this.Get(entity, ST_Interval));
		}
		else {
			this.Set(entity, SST_Next, GetGameTime() + interval);
		}
	}
};

// Left 4 Dead
methodmap IntervalTimer < SimTimerBase
{
	public IntervalTimer(int offset) {
		return view_as<IntervalTimer>( offset );
	}
	
	public static IntervalTimer FromGCOffset(Handle gc, const char[] key) {
		return view_as<IntervalTimer>( SimTimerBase.FromGCOffset(gc, key) );
	}
	
	public static IntervalTimer FromEntProp(int entity, PropType type, const char[] prop) {
		return view_as<IntervalTimer>( SimTimerBase.FromEntProp(entity, type, prop) );
	}
	
	public float GetTimestamp(int entity) {
		return this.Get(entity, IT_Timestamp);
	}
	
	public void SetTimestamp(int entity, float value) {
		this.Set(entity, IT_Timestamp, value, true);
	}
	
	public bool HasStarted(int entity) {
		return this.Get(entity, CT_Timestamp) != -1.0;
	}
	
	public void Start(int entity) {
		this.Set(entity, CT_Timestamp, GetGameTime(), true);
	}
	
	public float GetElapsedTime(int entity) {
		return GetGameTime() - this.Get(entity, CT_Timestamp);
	}
	
	public void Reset(int entity) {
		this.Set(entity, CT_Timestamp, -1.0, true);
	}
};

methodmap CountdownTimer < SimTimerBase
{
	public CountdownTimer(int offset) {
		return view_as<CountdownTimer>( offset );
	}
	
	public static CountdownTimer FromGCOffset(Handle gc, const char[] key) {
		return view_as<CountdownTimer>( SimTimerBase.FromGCOffset(gc, key) );
	}
	
	public static CountdownTimer FromEntProp(int entity, PropType type, const char[] prop) {
		return view_as<CountdownTimer>( SimTimerBase.FromEntProp(entity, type, prop) );
	}
	
	public float GetDuration(int entity) {
		return this.Get(entity, CT_Duration);
	}
	
	public void SetDuration(int entity, float value) {
		return this.Set(entity, CT_Duration, value, true);
	}
	
	public float GetTimestamp(int entity) {
		return this.Get(entity, CT_Timestamp);
	}
	
	public void SetTimestamp(int entity, float value) {
		return this.Set(entity, CT_Timestamp, value, true);
	}
	
	public void Start(int entity, float duration) {
		this.Set(entity, CT_Duration, duration, true);
		this.Set(entity, CT_Timestamp, GetGameTime() + duration, true);
	}
	
	public float GetElapsedTime(int entity) {
		return ( GetGameTime() - this.Get(entity, CT_Timestamp) ) + this.Get(entity, CT_Duration);
	}
	
	public float GetRemainingTime(int entity) {
		return this.Get(entity, CT_Timestamp) - GetGameTime();
	}
	
	public bool HasStarted(int entity) {
		return this.Get(entity, CT_Timestamp) != -1.0;
	}
	
	public bool HasElapsed(int entity) {
		return GetGameTime() >= this.Get(entity, CT_Timestamp);
	}
	
	public void Reset(int entity) {
		this.Set(entity, CT_Timestamp, -1.0, true);
	}
};
