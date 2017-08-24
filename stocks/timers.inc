#if defined _stocks_timers_included
 #endinput
#endif
#define _stocks_timers_included

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

// Internal game timers
enum SimpleSimTimer {
	InvalidTimer = 0
};

// Orange box
methodmap SimpleSimTimer __nullable__ < Handle
{
	public SimpleSimTimer(int entity, int offset) {
		Address addr = Address_Null;
		if (IsValidEdict(entity) && ( addr = GetEntityAddress(entity) ) && offset > -1) {
			addr += view_as<Address>( offset );
		}
		return view_as<SimpleSimTimer>( addr );
	}
	
	public static SimpleSimTimer FromGCOffset(int entity, Handle gc, const char[] key) {
		Address addr = Address_Null;
		if (IsValidEdict(entity) && ( addr = GetEntityAddress(entity) )) {
			int offset = GameConfGetOffset(gc, key);
			if (offset != -1) addr += view_as<Address>( offset );
		}
		return view_as<SimpleSimTimer>( addr );
	}
	
	public static SimpleSimTimer FromEntProp(int entity, PropType type, const char[] prop) {
		Address addr = Address_Null;
		if (IsValidEntity(entity) && ( addr = GetEntityAddress(entity) )) {
			int offset = -1;
			if (type == Prop_Data) {
				offset = FindDataMapInfo(entity, prop);
			}
			else {
				static char netclass[64];
				GetEntityNetClass(entity, netclass, sizeof(netclass));
				offset = FindSendPropInfo(netclass, prop);
			}
			if (offset == -1) return view_as<SimpleSimTimer>( Address_Null );
			addr += view_as<Address>( offset );
		}
		return view_as<SimpleSimTimer>( addr );
	}
	
	public float Get(int index) {
		return view_as<float>( LoadFromAddress(view_as<Address>( this ) + view_as<Address>( index * 4 ), NumberType_Int32) );
	}
	
	public void Set(int index, float value) {
		StoreToAddress(view_as<Address>( this ) + view_as<Address>( index * 4 ), view_as<int>( value ), NumberType_Int32);
	}
	
	property float Next
	{
		public get() {
			return this.Get(1);
		}
		
		public set(float value) {
			this.Set(1, value);
		}
	}
	
	public bool Expired() {
		return GetGameTime() >= this.Next;
	}
	
	public void Delay(float interval) {
		this.Set(1, GetGameTime() + interval);
	}
	
	public void SetTimer(float time) {
		this.Set(1, time);
	}
	
	public void Reset() {
		this.SetTimer(-1.0);
	}
};

// Left 4 Dead
methodmap IntervalTimer < SimpleSimTimer
{
	public IntervalTimer(int entity, int offset) {
		return view_as<IntervalTimer>( new SimpleSimTimer(entity, offset) );
	}
	
	public static IntervalTimer FromGCOffset(int entity, Handle gc, const char[] key) {
		return view_as<IntervalTimer>( SimpleSimTimer.FromGCOffset(entity, gc, key) );
	}
	
	public static IntervalTimer FromEntProp(int entity, PropType type, const char[] prop) {
		return view_as<IntervalTimer>( SimpleSimTimer.FromEntProp(entity, type, prop) );
	}
	
	property float Timestamp
	{
		public get() {
			return view_as<float>( this.Get(1) );//.Get(1);
		}
		
		public set(float value) {
			this.Set(1, value);
		}
	}
	
	public float GetElapsedTime() {
		return GetGameTime() - this.Timestamp;
	}
	
	public bool Expired() {
		return GetGameTime() >= this.Timestamp;
	}
	
	public void Reset() {
		this.Timestamp = -1.0;
	}
};

methodmap CountdownTimer < SimpleSimTimer
{
	public CountdownTimer(int entity, int offset) {
		return view_as<CountdownTimer>( new SimpleSimTimer(entity, offset) );
	}
	
	public static CountdownTimer FromGCOffset(int entity, Handle gc, const char[] key) {
		return view_as<CountdownTimer>( SimpleSimTimer.FromGCOffset(entity, gc, key) );
	}
	
	public static CountdownTimer FromEntProp(int entity, PropType type, const char[] prop) {
		return view_as<CountdownTimer>( SimpleSimTimer.FromEntProp(entity, type, prop) );
	}
	
	property float Duration
	{
		public get() {
			return this.Get(1);
		}
		
		public set(float value) {
			this.Set(1, value);
		}
	}
	
	property float Timestamp
	{
		public get() {
			return this.Get(2);
		}
		
		public set(float value) {
			this.Set(2, value);
		}
	}
	
	public float GetElapsedTime() {
		return ( GetGameTime() - this.Timestamp ) + this.Duration;
	}
	
	public float GetRemainingTime() {
		return this.Timestamp - GetGameTime();
	}
	
	public bool Started() {
		return this.Timestamp >= 0.0;
	}
	
	public bool Expired() {
		return GetGameTime() >= this.Timestamp;
	}
	
	public void Reset() {
		this.Timestamp = -1.0;
	}
};
