#if defined _stocks_colors_included
 #endinput
#endif
#define _stocks_colors_included

//
// In development
//

#define DeclareColor(%0) \
	property int %0 {\
		public get() {\
			return (view_as<int>(this) >> %1) & 0xff;\
		}\
	}\
	\
	public Clr Set%0(int %0 = 0) {\
		return view_as<Clr>( (view_as<int>(this) &~ (0xff << %1)) | ((%0 & 0xff) << %1) );\
	}\
	\
	public Clr Add%0(int %0 = 0) {\
		return this.Set%0(this.%0 + %0);\
	}

enum Clr {
	Color_Invalid = 0,
	
	Color_White = 0xffffffff,
	Color_Black = 0xff,
	
	Color_Red = 0xff0000ff,
	Color_Green = 0xff00ff,
	Color_Blue = 0xffff,
	
	Color_Yellow = 0xffff00ff,
	Color_Cyan = 0xffffff,
	Color_Magenta = 0xff00ffff,
	
	Color_Invisible = 0x0,
};

methodmap Clr
{
	public Clr(int red = 0, int green = 0, int blue = 0, int alpha = 255) {
		return view_as<Clr>(((red & 0xff) << 24) | ((green & 0xff) << 16) | ((blue & 0xff) << 8) | (alpha & 0xff));
	}
	
	property int Red {
		public get() {
			return (view_as<int>(this) >> 24) & 0xff;
		}
	}
	
	public Clr SetRed(int Red = 0) {
		return view_as<Clr>( (view_as<int>(this) &~ (0xff << 24)) | ((Red & 0xff) << 24) );
	}
	
	public Clr AddRed(int Red = 0) {
		return this.SetRed(this.Red + Red);
	}
	
	
	property int Green {
		public get() {
			return (view_as<int>(this) >> 16) & 0xff;
		}
	}
	
	public Clr SetGreen(int Green = 0) {
		return view_as<Clr>( (view_as<int>(this) &~ (0xff << 16)) | ((Green & 0xff) << 16) );
	}
	
	public Clr AddGreen(int Green = 0) {
		return this.SetGreen(this.Green + Green);
	}
	
	
	property int Blue {
		public get() {
			return (view_as<int>(this) >> 8) & 0xff;
		}
	}
	
	public Clr SetBlue(int Blue = 0) {
		return view_as<Clr>( (view_as<int>(this) &~ (0xff << 8)) | ((Blue & 0xff) << 8) );
	}
	
	public Clr AddBlue(int Blue = 0) {
		return this.SetBlue(this.Blue + Blue);
	}
	
	
	property int Alpha {
		public get() {
			return view_as<int>(this) & 0xff;
		}
	}
	
	public Clr SetAlpha(int Alpha = 0) {
		return view_as<Clr>( (view_as<int>(this) &~ 0xff) | (Alpha & 0xff) );
	}
	
	public Clr AddAlpha(int Alpha = 0) {
		return this.SetAlpha(this.Alpha + Alpha);
	}
	
	// DeclareColor(Red, 24);
	// DeclareColor(Green, 16);
	// DeclareColor(Blue, 8);
	// DeclareColor(Alpha, 0);
}
