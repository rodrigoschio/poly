#ifndef dos_h
#define dos_h

enum videomode_t{
	videomode_40x25_8x8,
	videomode_40x25_9x16,
	videomode_80x25_8x8,
	videomode_80x25_8x16,
	videomode_80x25_9x16,
	videomode_80x43_8x8,
	videomode_80x50_8x8,
	videomode_320x200,
	videomode_320x240,
	videomode_320x400,
	videomode_640x200,
	videomode_640x350,
	videomode_640x400,
	videomode_640x480,
	force_size_videomode=0x7ffffff // ensure videomode_t is 32-bit value
};

void setvideomode();
void setdoublebuffer(int enabled);
int screenwidth(void);
int screenheight(void);
unsigned char* screenbuffer(void);
unsigned char* swapbuffers(void);
void waitvbl(void);
void setpal(int index,int r,int g,int b);
void getpal(int index,int* r,int* g,int* b);

int shuttingdown(void);

void clearscreen(void);
int getpixel(int x,int y);
void hline(int x,int y,int len,int color);
void putpixel(int x,int y,int color);

void setdrawtarget(unsigned char* pixels,int width,int height);
void resetdrawtarget(void);

void setcolor(int color);
int getcolor(void);
void line(int x1,int y1,int x2,int y2);
void rectangle(int x,int y,int w,int h);
void bar(int x,int y,int w,int h);
void circle(int x,int y,int r);
void fillcircle(int x,int y,int r);
void ellipse(int x,int y,int rx,int ry);
void fillellipse(int x,int y,int rx,int ry);
void drawpoly(int* points_xy,int count);
void fillpoly(int* points_xy,int count);
void floodfill(int x,int y);
void boundaryfill(int x,int y,int boundary);

void outtextxy(int x,int y,char const* text);
void wraptextxy(int x,int y,char const* text,int width);
void centertextxy(int x,int y,char const* text,int width);

enum keycode_t{
	KEY_INVALID,KEY_LBUTTON,KEY_RBUTTON,KEY_CANCEL,KEY_MBUTTON,KEY_XBUTTON1,KEY_XBUTTON2,KEY_BACK,KEY_TAB,
	KEY_CLEAR,KEY_RETURN,KEY_SHIFT,KEY_CONTROL,KEY_MENU,KEY_PAUSE,KEY_CAPITAL,KEY_KANA,KEY_HANGUL=KEY_KANA,
	KEY_JUNJA,KEY_FINAL,KEY_HANJA,KEY_KANJI=KEY_HANJA,KEY_ESCAPE,KEY_CONVERT,KEY_NONCONVERT,KEY_ACCEPT,
	KEY_MODECHANGE,KEY_SPACE,KEY_PRIOR,KEY_NEXT,KEY_END,KEY_HOME,KEY_LEFT,KEY_UP,KEY_RIGHT,KEY_DOWN,
	KEY_SELECT,KEY_PRINT,KEY_EXEC,KEY_SNAPSHOT,KEY_INSERT,KEY_DELETE,KEY_HELP,KEY_0,KEY_1,KEY_2,KEY_3,KEY_4,
	KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_A,KEY_B,KEY_C,KEY_D,KEY_E,KEY_F,KEY_G,KEY_H,KEY_I,KEY_J,KEY_K,
	KEY_L,KEY_M,KEY_N,KEY_O,KEY_P,KEY_Q,KEY_R,KEY_S,KEY_T,KEY_U,KEY_V,KEY_W,KEY_X,KEY_Y,KEY_Z,KEY_LWIN,
	KEY_RWIN,KEY_APPS,KEY_SLEEP,KEY_NUMPAD0,KEY_NUMPAD1,KEY_NUMPAD2,KEY_NUMPAD3,KEY_NUMPAD4,KEY_NUMPAD5,
	KEY_NUMPAD6,KEY_NUMPAD7,KEY_NUMPAD8,KEY_NUMPAD9,KEY_MULTIPLY,KEY_ADD,KEY_SEPARATOR,KEY_SUBTRACT,KEY_DECIMAL,
	KEY_DIVIDE,KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5,KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,KEY_F11,KEY_F12,
	KEY_F13,KEY_F14,KEY_F15,KEY_F16,KEY_F17,KEY_F18,KEY_F19,KEY_F20,KEY_F21,KEY_F22,KEY_F23,KEY_F24,
	KEY_NUMLOCK,KEY_SCROLL,KEY_LSHIFT,KEY_RSHIFT,KEY_LCONTROL,KEY_RCONTROL,KEY_LMENU,KEY_RMENU,KEY_BROWSER_BACK,
	KEY_BROWSER_FORWARD,KEY_BROWSER_REFRESH,KEY_BROWSER_STOP,KEY_BROWSER_SEARCH,KEY_BROWSER_FAVORITES,
	KEY_BROWSER_HOME,KEY_VOLUME_MUTE,KEY_VOLUME_DOWN,KEY_VOLUME_UP,KEY_MEDIA_NEXT_TRACK,KEY_MEDIA_PREV_TRACK,
	KEY_MEDIA_STOP,KEY_MEDIA_PLAY_PAUSE,KEY_LAUNCH_MAIL,KEY_LAUNCH_MEDIA_SELECT,KEY_LAUNCH_APP1,KEY_LAUNCH_APP2,
	KEY_OEM_1,KEY_OEM_PLUS,KEY_OEM_COMMA,KEY_OEM_MINUS,KEY_OEM_PERIOD,KEY_OEM_2,KEY_OEM_3,KEY_OEM_4,KEY_OEM_5,
	KEY_OEM_6,KEY_OEM_7,KEY_OEM_8,KEY_OEM_102,KEY_PROCESSKEY,KEY_ATTN,KEY_CRSEL,KEY_EXSEL,KEY_EREOF,KEY_PLAY,
	KEY_ZOOM,KEY_NONAME,KEY_PA1,KEY_OEM_CLEAR,
	KEYCOUNT,KEYPADDING=0xFFFFFFFF
};

int keystate(enum keycode_t key);

#define KEY_MODIFIER_RELEASED 0x80000000
enum keycode_t* readkeys(void);
char const* readchars(void);

int mousex(void);
int mousey(void);
int mouserelx(void);
int mouserely(void);

#endif /* dos_h */

#ifdef DOS_IMPLEMENTATION

#ifndef NO_MAIN_DEF
	#ifdef main
		#undef main
	#endif
#endif

#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "app.h"
#include "crtemu_pc.h"
#include "frametimer.h"

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable: 4201)
#endif
#ifdef _WIN32
#pragma warning(pop)
#endif

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable: 4024)
#pragma warning(disable: 4047)
#pragma warning(disable: 4242)
#pragma warning(disable: 4244)
#pragma warning(disable: 4701)
#endif
#ifdef _WIN32
#pragma warning(pop)
#endif

bool app_has_focus(app_t* app);

#include "thread.h"

static uint32_t default_palette[256] ={
	0x000000,0xaa0000,0x00aa00,0xaaaa00,0x0000aa,0xaa00aa,0x0055aa,0xaaaaaa,0x555555,0xff5555,0x55ff55,0xffff55,0x5555ff,0xff55ff,0x55ffff,0xffffff,0x000000,0x141414,0x202020,0x2c2c2c,0x383838,0x454545,0x515151,0x616161,0x717171,0x828282,0x929292,0xa2a2a2,0xb6b6b6,0xcbcbcb,0xe3e3e3,0xffffff,0xff0000,0xff0041,0xff007d,0xff00be,0xff00ff,0xbe00ff,0x7d00ff,0x4100ff,0x0000ff,0x0041ff,0x007dff,0x00beff,0x00ffff,0x00ffbe,0x00ff7d,0x00ff41,0x00ff00,0x41ff00,0x7dff00,0xbeff00,0xffff00,0xffbe00,0xff7d00,0xff4100,0xff7d7d,0xff7d9e,0xff7dbe,0xff7ddf,0xff7dff,0xdf7dff,0xbe7dff,0x9e7dff,
	0x7f7dff,0x7d9eff,0x7dbeff,0x7ddfff,0x7dffff,0x7dffdf,0x7dffbe,0x7dff9e,0x7dff7d,0x9eff7d,0xbeff7d,0xdfff7d,0xffff7d,0xffdf7d,0xffbe7d,0xff9e7d,0xffb6b6,0xffb6c7,0xffb6db,0xffb6eb,0xffb6ff,0xebb6ff,0xdbb6ff,0xc7b6ff,0xb6b6ff,0xb6c7ff,0xb6dbff,0xb6ebff,0xb6ffff,0xb6ffeb,0xb6ffdb,0xb6ffc7,0xb6ffb6,0xc7ffb6,0xdbffb6,0xebffb6,0xffffb6,0xffebb6,0xffdbb6,0xffc7b6,0x710000,0x71001c,0x710038,0x710055,0x710071,0x550071,0x380071,0x1c0071,0x000071,0x001c71,0x003871,0x005571,0x007171,0x007155,0x007138,0x00711c,0x007100,0x1c7100,0x387100,0x557100,0x717100,0x715500,0x713800,0x711c00,
	0x713838,0x713845,0x713855,0x713861,0x713871,0x613871,0x553871,0x453871,0x383871,0x384571,0x385571,0x386171,0x387171,0x387161,0x387155,0x387145,0x387138,0x457138,0x557138,0x617138,0x717138,0x716138,0x715538,0x714538,0x715151,0x715159,0x715161,0x715169,0x715171,0x695171,0x615171,0x595171,0x515171,0x515971,0x516171,0x516971,0x517171,0x517169,0x517161,0x517159,0x517151,0x597151,0x617151,0x697151,0x717151,0x716951,0x716151,0x715951,0x410000,0x410010,0x410020,0x410030,0x410041,0x300041,0x200041,0x100041,0x000041,0x001041,0x002041,0x003041,0x004141,0x004130,0x004120,0x004110,
	0x004100,0x104100,0x204100,0x304100,0x414100,0x413000,0x412000,0x411000,0x412020,0x412028,0x412030,0x412038,0x412041,0x382041,0x302041,0x282041,0x202041,0x202841,0x203041,0x203841,0x204141,0x204138,0x204130,0x204128,0x204120,0x284120,0x304120,0x384120,0x414120,0x413820,0x413020,0x412820,0x412c2c,0x412c30,0x412c34,0x412c3c,0x412c41,0x3c2c41,0x342c41,0x302c41,0x2c2c41,0x2c3041,0x2c3441,0x2c3c41,0x2c4141,0x2c413c,0x2c4134,0x2c4130,0x2c412c,0x30412c,0x34412c,0x3c412c,0x41412c,0x413c2c,0x41342c,0x41302c,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,
};

static uint32_t font9x16[] ={
	9,16,11,
	0x00000000,0x00000000,0xfc00ff00,0x00000003,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0xfc00ff00,0x00000003,0x80010000,0xc0000000,0x00000007,0x00000000,0x01f8fc00,0x00000000,0xfc00ff00,0x9f83c3c3,0xc003003f,0x6fe33060,0x180c000c,0x00000030,0x03fd0200,0x06030000,0xfc00ff00,0x99866383,0xe0070c31,0xcdb330f0,0x3c1e0000,0x00000030,0xb36d4a00,0x0f078081,0xfc00ff00,0x9f8662c3,0xf00f0c3f,0x8db331f8,0x7e3f0003,0x00000030,0xfbfd0200,0x1f8781c3,0x0c78ff00,0x81866263,0xf81f6db1,0xcdb33060,0x180c0006,0x00606030,0xfbfd0200,0x3fdce3e3,0x64cce70c,0x818660f2,0xfe7f1e31,0x6de33060,0x180c000c,0x0030c030,0xfb0d7a00,0x3fdce7f3,0xf484c31e,0x8183c19a,0xf81f73b1,0x6d833060,0x180c000c,0x03f9fc30,0xfb9d3200,0x1f9ce3e3,0xf484c31e,0x8181819a,0xf00f1e31,0xcd8331f8,0x183f1fc6,0x0030c030,0xf3fd0200,0x060301c1,0x64cce70c,0x81c7e19a,0xe0076db9,0x8d8000f0,0x181e1fc3,0x006060fc,0xe3fd0200,0x06030080,0x0c78ff00,0xc1e1819b,0xc0030c39,0x0d833060,0x180c1fc6,0x00000078,0x41f8fc00,0x0f078000,0xfc00ff00,0xc0e180f3,0x80010c19,0x6d833000,0x183f1fcc,0x00000030,0x00000000,0x00000000,0xfc00ff00,0xc0000003,0x00000000,0xc0000000,0x00000007,0x00000000,0x00000000,0x00000000,0xfc00ff00,0x00000003,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0xfc00ff00,0x00000003,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0xfc00ff00,0x00000003,0x00000000,0x00000000,0x00000000,0x00000000,
	0x00000000,0x00000000,0x00001800,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x19800000,0x00001800,0x00000060,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x19830000,0x70003e00,0x01830060,0x00000000,0x03c00000,0x301f0f83,0x03f870fe,0x00000000,0x19878000,0xd800631b,0x03018060,0x00000000,0x86600000,0x383198c3,0x03181806,0xf8200000,0x09078003,0xd886431b,0x0600c030,0x00000000,0xcc320000,0x3c301803,0x03000c06,0xf8704800,0x80078003,0x70c6033f,0x8600c000,0x00000c19,0x0c330000,0x36300c03,0x03000c06,0xf070cc03,0x00030001,0xb8603e1b,0x0600c001,0x00000c0f,0x0db18000,0x331e0603,0x0180fc7e,0xf0f9fe03,0x00030001,0xec30601b,0xc600c000,0xfe003f3f,0x0db0c000,0x7f300303,0x00c18cc0,0xe0f8cc03,0x00030000,0xcc18601b,0x0600c000,0x00000c0f,0x0c306000,0x30300183,0x00618cc0,0xe1fc487f,0x80000000,0xcc0c613f,0x8600c000,0x00180c19,0x0c303000,0x303000c3,0x00618cc0,0x41fc0000,0x00030000,0xccc6631b,0x03018000,0x00180000,0x06601860,0x303198c3,0x00618cc6,0x00000000,0x00030000,0xb8c23e1b,0x01830001,0x00180000,0xc3c00860,0x781f1fcf,0x0060f87c,0x00000000,0x00000000,0x00001800,0x00000000,0x000c0000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00001800,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00007c3e,0x00000000,0xfc10001f,0xcfe1f1e0,0x78631e1f,0x60f339e0,0x3f1f18d8,0x01f0fc7c,0x0000c663,0x81800600,0x98383e31,0x8cc36331,0x30633319,0xe06330c0,0x663199dc,0x031998c6,0xc060c663,0x83000300,0x986c6331,0x88c66219,0x30632191,0xe06330c0,0x66319bdf,0x031998c6,0xc060c663,0x060fc180,0x98c66318,0x82c66019,0x30630185,0xe061b0c0,0x66319fdf,0x003198c6,0x0000fc3e,0x0c0000c0,0xf8c67b0c,0x83c66018,0x307f0187,0x6060f0c0,0x3e319edb,0x00e0f8c6,0x0000c063,0x18000060,0x98fe7b0c,0x82c66019,0x30633d85,0x6060f0c0,0x06319cd8,0x0180d8c6,0x0000c063,0x0c0fc0c0,0x98c67b0c,0x80c66019,0x30633181,0x6061b0cc,0x063198d8,0x030198c6,0xc060c063,0x06000180,0x98c63b00,0x88c66219,0x30633181,0x646330cc,0x063198d8,0x031998d6,0xc0606063,0x03000300,0x98c6030c,0x8cc36331,0x30633301,0x666330cc,0x063198d8,0x031998f6,0x60003c3e,0x01800600,0xfcc63e0c,0xcfe1f1e0,0x78632e03,0x67f33878,0x0f1f18d8,0x01f19c7c,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000060,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x000000e0,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x00000000,0x00000000,0x20000000,0x0000c000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x70000000,0x0000c000,0x00000000,0x00000000,0x00000000,0x00000000,0x1b0cc6ff,0x3fd86c36,0xd878001e,0xc0018000,0x00380001,0x00700070,0x1c039803,0x00000000,0x1b0cc6db,0x30d86c36,0x8c600106,0x80000001,0x00300001,0x006000d8,0x18031803,0x00000000,0x1b0cc699,0x18586666,0x00600306,0x80000000,0x00300001,0x00600098,0x18030000,0x00000000,0x1b0cc618,0x0c0cc3c6,0x00600706,0x83c00000,0x7c3c1f07,0x83637018,0x18331c03,0x01f0ecce,0x1b0cc618,0x06078186,0x00600e06,0x86000000,0xc636318d,0x06e1983c,0x181b1803,0x031999fe,0xdb0cc618,0x03030186,0x00601c06,0x87c00000,0xfe330199,0x06619818,0x180f1803,0x031999b6,0xdb0cc618,0x018303c6,0x00603806,0x86600000,0x06330199,0x06619818,0x180f1803,0x031999b6,0xf998c618,0x20c30667,0x00607006,0x86600000,0x06330199,0x06619818,0x181b1803,0x031999b6,0x30f0c618,0x30c30c33,0x00606006,0x86600000,0xc6333199,0x06619818,0x18331803,0x031999b6,0x30607c3c,0x3fc78c33,0x0078401e,0x8dc00000,0x7c6e1f0f,0x8671f03c,0x3c339807,0x01f199b6,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00018000,0x00001980,0x00000000,0x00000000,0x00000000,0x00000000,0x000007f8,0x00000000,0x00019800,0x00001980,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x0000f000,0x00000f00,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0xc00040c0,0x08000700,0x00001800,0x00000000,0x00000080,0x00000000,0x81c18380,0x663c001b,0x8330e060,0x1c000d81,0x033030c6,0x00000000,0x000000c0,0x00000000,0xc30180c0,0x0066000e,0x0001b030,0x36000703,0x00006000,0x00000000,0x000000c0,0x00000000,0x030180c0,0x00430400,0x00000000,0x001e0000,0x00000000,0xf0ecdc3b,0xb0c663f1,0xfcc6c361,0x030180c1,0x66030e00,0xc1e0f0f8,0x3e330783,0x00e0f87c,0x19b86666,0xb0c660c3,0xccc66661,0x0e000070,0x66031b00,0x0301818c,0x63030c06,0x00c18cc6,0x31986666,0xb0c660c0,0x60c63c61,0x030180c0,0x66033180,0xc3e1f1fc,0x7f030f87,0x00c1fcfe,0xe0186666,0xb0c660c0,0x30c6186d,0x030180c0,0x66433180,0x6331980c,0x03330cc6,0x00c00c06,0x80186666,0x998660c1,0x18c63c6d,0x030180c0,0x66663180,0x6331980c,0x031e0cc6,0x00c00c06,0x18186666,0x8f0666c3,0x8cc6667f,0x030180c1,0x663c3f80,0x6331998c,0x63180cc6,0x00c18cc6,0xf03c7c3e,0x060dc381,0xfcfcc333,0x01c18381,0xdc300000,0xc6e370f8,0x3e301b8d,0x01e0f87c,0x00006006,0x00000000,0x00c00000,0x00000000,0x00600000,0x00000000,0x001e0000,0x00000000,0x00006006,0x00000000,0x00600000,0x00000000,0x003e0000,0x00000000,0x00000000,0x00000000,0x0000f00f,0x00000000,0x003e0000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0xe0000000,0x00000180,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x000000dc,0xb18c0c18,0x000000c1,0x300c0004,0xcc600030,0x001c0c18,0x018380fc,0x000c0606,0x00e0f076,0xe000183c,0x1f000060,0x7818630e,0x00063060,0x86360c00,0x00c6c199,0x6e060303,0x01b0d800,0x00203066,0x0d800000,0xcc30001b,0xc7c000c0,0xcc263f18,0x8060c198,0x3b030181,0x01b0d8c6,0xe0700000,0x0cc007f0,0x00000000,0xcc600000,0x78066198,0x0000c0f8,0x00000000,0x00e1f0ce,0xb0d8381c,0x0ccec661,0xcc7c3e1f,0xcc663198,0x300f0198,0x81e0c118,0x3b198f83,0x000000de,0x198c3018,0x9fdb8063,0xccc66331,0xcc663198,0xfe060198,0x0303f199,0x661998c3,0x01f1f8fe,0x198c3018,0x8cdb03e3,0xccc66331,0xcc663198,0x30060198,0x03e0c3d8,0x661998c3,0x000000f6,0xf9fc3018,0x8ccfc063,0xccc66331,0xcc663198,0xfe066198,0x0330c199,0x661998c3,0x000000e6,0x198c3018,0x8cc36063,0xccc66331,0xcc663198,0x30063f18,0x0330c198,0x661998c3,0x000000c6,0x198c3018,0x8cc76663,0xccc66331,0xcc663198,0x30670c18,0x0330c198,0x661998c3,0x000000c6,0x198c783c,0x1cddc7f3,0xb87c3e1f,0x87c7e371,0x303f0c0f,0x86e0c33c,0x66370f87,0x000000c6,0x00000000,0x00000000,0x00000000,0x00060000,0x00000000,0x0000d800,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00030000,0x00000000,0x00007000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x0001e000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x00000000,0x00000000,0xef558800,0x030180c6,0xd800001b,0x86c001b0,0x1800060d,0x00c00030,0x18000000,0x00000030,0xb8aa2200,0x030180c7,0xd800001b,0x86c001b0,0x1800060d,0x00c00030,0x1800000c,0x00030030,0xef558800,0x030180c6,0xd800001b,0x86c001b0,0x1800060d,0x00c00030,0x1800000c,0x00030432,0xb8aa2200,0x030180c7,0xd800001b,0x86c001b0,0x1800060d,0x00c00030,0x18000000,0x00000633,0xef558800,0x030180c6,0xd800001b,0x86c001b0,0x1800060d,0x00c00030,0x9800000c,0x9b030331,0xb8aa220d,0x03e180c7,0xde1f001b,0x86f3f9b0,0x180007cd,0x00c00030,0xc1fcfe0c,0x0d830180,0xef55881b,0x030180c6,0xc018001b,0x860301b0,0x1800060d,0x00c00030,0x61800606,0x06c300c0,0xb8aa2236,0xc3e1f0c7,0xde1f3f9b,0xe7f379b0,0xf80f87cf,0x0fc7ffff,0x31800603,0x0d878660,0xef55881b,0x030180c6,0xd818361b,0x000361b0,0x000c0000,0x00c06000,0x99800663,0x9b078733,0xb8aa220d,0x030180c7,0xd818361b,0x000361b0,0x000c0000,0x00c06000,0xc9800663,0x00078696,0xef558800,0x030180c6,0xd818361b,0x000361b0,0x000c0000,0x00c06000,0x0000003e,0x000307c3,0xb8aa2200,0x030180c7,0xd818361b,0x000361b0,0x000c0000,0x00c06000,0x80000000,0x00000601,0xef558800,0x030180c6,0xd818361b,0x000361b0,0x000c0000,0x00c06000,0xc0000000,0x00000607,0xb8aa2200,0x030180c7,0xd818361b,0x000361b0,0x000c0000,0x00c06000,0x00000000,0x00000000,0xef558800,0x030180c6,0xd818361b,0x000361b0,0x000c0000,0x00c06000,0x00000000,0x00000000,0xb8aa2200,0x030180c7,0xd818361b,0x000361b0,0x000c0000,0x00c06000,
	0x60603000,0x1b0006c3,0xb0006c00,0x0006c0c1,0x00183600,0x01836000,0x00ff8003,0x0fffc01e,0x60603000,0x1b0006c3,0xb0006c00,0x0006c0c1,0x00183600,0x01836000,0x00ff8003,0x0fffc01e,0x60603000,0x1b0006c3,0xb0006c00,0x0006c0c1,0x00183600,0x01836000,0x00ff8003,0x0fffc01e,0x60603000,0x1b0006c3,0xb0006c00,0x0006c0c1,0x00183600,0x01836000,0x00ff8003,0x0fffc01e,0x60603000,0x1b0006c3,0xb0006c00,0x0006c0c1,0x00183600,0x01836000,0x00ff8003,0x0fffc01e,0x67e03000,0xfbff9ec3,0xbfffecff,0x3fe6cfff,0xf1f83600,0x1ff36003,0x00ff8003,0x0fffc01e,0x60603000,0x000180c3,0x00000c00,0x0006c000,0x30183600,0x01836000,0x00ff8003,0x0fffc01e,0x67e3ffff,0xfffd9fcf,0xbfffecf7,0xffffffff,0xf1f8fe7f,0xfffffff3,0xfffffe03,0x0007c01f,0x60603000,0x000d8003,0xb0006c36,0x03000001,0x3000001b,0x018361b0,0xffff8600,0x0007c01f,0x60603000,0x000d8003,0xb0006c36,0x03000001,0x3000001b,0x018361b0,0xffff8600,0x0007c01f,0x60603000,0x000d8003,0xb0006c36,0x03000001,0x3000001b,0x018361b0,0xffff8600,0x0007c01f,0x60603000,0x000d8003,0xb0006c36,0x03000001,0x3000001b,0x018361b0,0xffff8600,0x0007c01f,0x60603000,0x000d8003,0xb0006c36,0x03000001,0x3000001b,0x018361b0,0xffff8600,0x0007c01f,0x60603000,0x000d8003,0xb0006c36,0x03000001,0x3000001b,0x018361b0,0xffff8600,0x0007c01f,0x60603000,0x000d8003,0xb0006c36,0x03000001,0x3000001b,0x018361b0,0xffff8600,0x0007c01f,0x60603000,0x000d8003,0xb0006c36,0x03000001,0x3000001b,0x018361b0,0xffff8600,0x0007c01f,
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000003,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x1c000003,0x07800000,0x01fc3c00,0x00000000,0x70000000,0x000003c0,0x0000000e,0x07000000,0x36000003,0x01800000,0x018c6600,0x000007f0,0xd8387e00,0x18000060,0x00001f03,0x0d818030,0x36000003,0x01800000,0xf98c6600,0x19800633,0x8c6c1837,0x8c0000c1,0x307f3181,0x0d80c060,0x1c000603,0x01800000,0xb00c666e,0x998fc061,0x8cc63c1d,0x8fc7e181,0x30003181,0x018060c0,0x00370603,0x01800000,0xb00c363b,0x198360c1,0x8cc6660c,0x9b6db3e1,0xfc00318f,0x01803180,0x001d8003,0x01800000,0xb00c661b,0x19836181,0xd8fe660c,0x9b6db330,0x307f3181,0x018060c0,0x00001f83,0x01b80030,0xb00cc61b,0x198360c1,0xd8c6660c,0x99edb330,0x30003181,0x6180c060,0x00370003,0x01b06030,0xb00cc61b,0x0f836061,0xd8c63c0c,0x8fc7e330,0x00003181,0x61818030,0x001d8603,0x01b00000,0xb00cc63b,0x01836631,0xd86c180c,0x00c00330,0x007f3183,0x61800000,0x00000603,0x01e00000,0xb00c666e,0x0181c7f1,0xdc387e0c,0x006001e1,0xfe00318e,0xc183f1f9,0x00000001,0x01c00000,0x00000000,0x00c00000,0x00000000,0x00000000,0x00000000,0x01800000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x01800000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x01800000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x01800000,0x00000000,0x00000000,
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00001c1b,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00003636,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00001836,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00f80c36,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00f82636,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00f83e36,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00f80000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00f80000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00f80000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00f80000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
};

struct internals_t{
	thread_mutex_t mutex;
	thread_atomic_int_t exit_flag;
	struct{
		thread_signal_t signal;
		thread_atomic_int_t count;
	} vbl;
	struct{
		enum videomode_t mode;
		int width;
		int height;
		int color;
		uint32_t* font;
		int cellwidth;
		int cellheight;
		bool doublebuffer;
		uint8_t* buffer;
		uint8_t buffer0[1024*1024];
		uint8_t buffer1[1024*1024];
		uint32_t palette[256];
	} screen;
	struct{
		uint8_t* buffer;
		int width;
		int height;
	} draw;
	struct{
		int x;
		int y;
		int fg;
		int bg;
		bool curs;
	} conio;
	struct{
		bool keystate[KEYCOUNT];
		enum keycode_t* keybuffer;
		enum keycode_t keybuffer0[256];
		enum keycode_t keybuffer1[256];
		char* charbuffer;
		char charbuffer0[256];
		char charbuffer1[256];
		int mouse_x;
		int mouse_y;
		int mouse_relx;
		int mouse_rely;
	} input;
}* internals;

static void internals_create(int sound_buffer_size){
	(void)sound_buffer_size;
	internals=(struct internals_t*)malloc(sizeof(struct internals_t));
	memset(internals,0,sizeof(*internals));
	thread_mutex_init(&internals->mutex);
	thread_signal_init(&internals->vbl.signal);
	thread_atomic_int_store(&internals->vbl.count,0);
	internals->screen.mode=videomode_80x25_9x16;
	internals->screen.width=80;
	internals->screen.height=25;
	internals->screen.font=font9x16;
	internals->screen.cellwidth=9;
	internals->screen.cellheight=16;
	internals->screen.buffer=internals->screen.buffer0;
	memcpy(internals->screen.palette,default_palette,1024);
	internals->draw.buffer=internals->screen.buffer;
	internals->draw.width=internals->screen.width;
	internals->draw.height=internals->screen.height;
	internals->screen.color=15;
	internals->conio.fg=7;
	internals->conio.curs=true;
	internals->input.keybuffer=internals->input.keybuffer0;
	internals->input.charbuffer=internals->input.charbuffer0;
}

static void internals_destroy(void){
	thread_signal_term(&internals->vbl.signal);
	thread_mutex_term(&internals->mutex);
	free(internals);
	internals=NULL;
}

int shuttingdown(void){
	return thread_atomic_int_load(&internals->exit_flag);
}

void setvideomode(enum videomode_t mode){
	thread_mutex_lock(&internals->mutex);
	internals->screen.mode=mode;
	memcpy(internals->screen.palette,default_palette,1024);
	internals->conio.curs=true;
	// videomode_320x200
	internals->screen.width=320;
	internals->screen.height=200;
	internals->screen.font=NULL;
	internals->screen.cellwidth=1;
	internals->screen.cellheight=1;
	memset(internals->screen.buffer0,0,internals->screen.width*internals->screen.height*(internals->screen.font ? 2 : 1));
	memset(internals->screen.buffer1,0,internals->screen.width*internals->screen.height*(internals->screen.font ? 2 : 1));
	resetdrawtarget();
	thread_mutex_unlock(&internals->mutex);
};

int screenwidth(void){
	return internals->screen.width;
}

int screenheight(void){
	return internals->screen.height;
}

unsigned char* screenbuffer(void){
	return internals->screen.buffer;
}

void setdoublebuffer(int enabled){
	internals->screen.doublebuffer=(enabled != 0);
}

unsigned char* swapbuffers(void){
	if(internals->screen.doublebuffer){
		thread_mutex_lock(&internals->mutex);
		if(internals->screen.buffer==internals->screen.buffer0){
			if(internals->draw.buffer==internals->screen.buffer){
				internals->draw.buffer=internals->screen.buffer1;
			}
			internals->screen.buffer=internals->screen.buffer1;
		} else{
			if(internals->draw.buffer==internals->screen.buffer){
				internals->draw.buffer=internals->screen.buffer0;
			}
			internals->screen.buffer=internals->screen.buffer0;
		}
		thread_mutex_unlock(&internals->mutex);
	}
	return internals->screen.buffer;
}

void setpal(int index,int r,int g,int b){
	if(index < 0||index >= 256){
		return;
	}
	r=(r & 63)<< 2;
	g=(g & 63)<< 2;
	b=(b & 63)<< 2;
	internals->screen.palette[index]=(b << 16)| (g << 8)| (r);
}

void getpal(int index,int* r,int* g,int* b){
	if(index < 0||index >= 256){
		return;
	}
	uint32_t c=internals->screen.palette[index];
	uint32_t cr=(c)& 0xff;
	uint32_t cg=(c >> 8)& 0xff;
	uint32_t cb=(c >> 16)& 0xff;
	if(r){
		*r=cr >> 2;
	}
	if(g){
		*g=cg >> 2;
	}
	if(b){
		*b=cb >> 2;
	} 
}

int getpixel(int x,int y){
	if(internals->screen.font)return 0;
	if(x >= 0&&y >= 0&&x < internals->draw.width&&y < internals->draw.height){
		return internals->draw.buffer[x+internals->draw.width*y];
	} else{
		return 0;
	}
}

void putpixel(int x,int y,int color){
	if(x >= 0&&y >= 0&&x < internals->draw.width&&y < internals->draw.height){
		internals->draw.buffer[x+internals->draw.width*y]=(uint8_t)color;
	}
}

void setdrawtarget(unsigned char* pixels,int width,int height){
	internals->draw.buffer=pixels;
	internals->draw.width=width;
	internals->draw.height=height;
}

void resetdrawtarget(void){
	internals->draw.buffer=internals->screen.buffer;
	internals->draw.width=internals->screen.width;
	internals->draw.height=internals->screen.height;
}

void setcolor(int color){
	if(color >= 0&&color <= 255){
		internals->screen.color=color;
	}
}

int getcolor(void){
	if(internals->screen.font)return 0;
	return internals->screen.color;
}

void waitvbl(void){
	if(thread_atomic_int_load(&internals->exit_flag)== 0){
		int current_vbl_count=thread_atomic_int_load(&internals->vbl.count);
		while(current_vbl_count==thread_atomic_int_load(&internals->vbl.count)){
			thread_signal_wait(&internals->vbl.signal,1000);
		}
	}
}

static void signalvbl(void){
	thread_atomic_int_inc(&internals->vbl.count);
	thread_signal_raise(&internals->vbl.signal);
}

void clearscreen(void){
	memset(internals->screen.buffer,0,internals->screen.width*internals->screen.height*(internals->screen.font ? 2 : 1));
}

void hline(int x,int y,int len,int color){
	if(y < 0||y >= internals->draw.height){
		return;
	}
	if(x < 0){
		len += x; x=0;
	}
	if(x+len > internals->draw.width){
		len=internals->draw.width-x;
	}
	uint8_t* scr=internals->draw.buffer+y*internals->draw.width+x;
	uint8_t* end=scr+len;
	while(scr < end)*scr++=(uint8_t)color;
}

void line(int x1,int y1,int x2,int y2){
	int color=internals->screen.color;
	int dx=x2-x1;
	dx=dx < 0 ? -dx : dx;
	int sx=x1 < x2 ? 1 : -1;
	int dy=y2-y1;
	dy=dy < 0 ? -dy : dy;
	int sy=y1 < y2 ? 1 : -1;
	int err=(dx > dy ? dx : -dy)/ 2;	
	int x=x1;
	int y=y1;
	while(x != x2||y != y2){
		putpixel(x,y,color);		
		int e2=err;
		if(e2 > -dx){
			err -= dy;
			x += sx;
		}
		if(e2 < dy){
			err += dx;
			y += sy;
		}
	}
	putpixel(x,y,color);
}

void rectangle(int x,int y,int w,int h){
	int color=internals->screen.color;
	hline(x,y,w,color);
	hline(x,y+h,w,color);
	line(x,y,x,y+h);
	line(x+w-1,y,x+w-1,y+h);
}

void bar(int x,int y,int w,int h){
	int color=internals->screen.color;
	for(int i=y; i < y+h; ++i){
		hline(x,i,w,color);
	}
}

void circle(int x,int y,int r){
	int color=internals->screen.color;
	int f=1-r;
	int dx=0;
	int dy=-2*r;
	int ix=0;
	int iy=r;
	putpixel(x,y+r,color);
	putpixel(x,y-r,color);
	putpixel(x+r,y,color);
	putpixel(x-r,y,color);
	while(ix < iy){
		if(f >= 0){
			--iy;
			dy += 2;
			f += dy;
		}
		++ix;
		dx += 2;
		f += dx+1;	
		putpixel(x+ix,y+iy,color);
		putpixel(x-ix,y+iy,color);
		putpixel(x+ix,y-iy,color);
		putpixel(x-ix,y-iy,color);
		putpixel(x+iy,y+ix,color);
		putpixel(x-iy,y+ix,color);
		putpixel(x+iy,y-ix,color);
		putpixel(x-iy,y-ix,color);
	}
}

void fillcircle(int x,int y,int r){	
	int color=internals->screen.color;
	int f=1-r;
	int dx=0;
	int dy=-2*r;
	int ix=0;
	int iy=r;
	while(ix <= iy){
		hline(x-iy,y+ix,2*iy,color);
		hline(x-iy,y-ix,2*iy,color);
		if(f >= 0){
			hline(x-ix,y+iy,2*ix,color);
			hline(x-ix,y-iy,2*ix,color);
			--iy;
			dy += 2;
			f += dy;
		}
		++ix;
		dx += 2;
		f += dx+1;	
	}
}
	

void ellipse(int x,int y,int rx,int ry){
	int color=internals->screen.color;
	int asq=rx*rx;
	int bsq=ry*ry;
	putpixel(x,y+ry,color);
	putpixel(x,y-ry,color);
	int wx=0;
	int wy=ry;
	int xa=0;
	int ya=asq*2*ry;
	int thresh=asq/4-asq*ry;
	for(; ;){
		thresh += xa+bsq;
		if(thresh >= 0){
			ya -= asq*2;
			thresh -= ya;
			--wy;
		}
		xa += bsq*2;
		++wx;
		if(xa >= ya){
			break;
		}
		putpixel(x+wx,y-wy,color);
		putpixel(x-wx,y-wy,color);
		putpixel(x+wx,y+wy,color);
		putpixel(x-wx,y+wy,color);
	}
	putpixel(x+rx,y,color);
	putpixel(x-rx,y,color);
	wx=rx;
	wy=0;
	xa=bsq*2*rx;
	ya=0;
	thresh=bsq/4-bsq*rx;
	for(; ;){
		thresh += ya+asq;
		if(thresh >= 0){
			xa -= bsq*2;
			thresh=thresh-xa;
			--wx;
		}
		ya += asq*2;
		++wy;
		if(ya > xa){
			break;
		}
		putpixel(x+wx,y-wy,color);
		putpixel(x-wx,y-wy,color);
		putpixel(x+wx,y+wy,color);
		putpixel(x-wx,y+wy,color);
	}
}


void fillellipse(int x,int y,int rx,int ry){
	int color=internals->screen.color;
	int asq=rx*rx;
	int bsq=ry*ry;
	int wx=0;
	int wy=ry;
	int xa=0;
	int ya=asq*2*ry;
	int thresh=asq/4-asq*ry;
	for(; ;){
		thresh += xa+bsq;
		if(thresh >= 0){
			ya -= asq*2;
			thresh -= ya;
			hline(x-wx,y-wy,wx*2,color);
			hline(x-wx,y+wy,wx*2,color);
			--wy;
		}
		xa += bsq*2;
		++wx;
		if(xa >= ya){
			break;
		}
	}
	hline(x-rx,y,rx*2,color);
	wx=rx;
	wy=0;
	xa=bsq*2*rx;
	ya=0;
	thresh=bsq/4-bsq*rx;
	for(; ;){
		thresh += ya+asq;
		if(thresh >= 0){
			xa -= bsq*2;
			thresh=thresh-xa;
			--wx;
		}
		ya += asq*2;
		++wy;
		if(ya > xa){
			break;
		}
		hline(x-wx,y-wy,wx*2,color);
		hline(x-wx,y+wy,wx*2,color);
	}
}

void drawpoly(int* points_xy,int count){
	for(int i=0; i < count-1; ++i){
		line(points_xy[i*2+0],points_xy[i*2+1],
			points_xy[(i+1)* 2+0],points_xy[(i+1)* 2+1]);
	}
}

void fillpoly(int* points_xy,int count){
	#define MAX_POLYGON_POINTS 256
	static int node_x[MAX_POLYGON_POINTS];

	if(count <= 0||count > MAX_POLYGON_POINTS){
		return;
	}
	int color=internals->screen.color;
	int min_y=points_xy[0+1];
	int max_y=min_y;
	// find extents
	for(int i=1; i < count; ++i){
		if(points_xy[i*2+1] < min_y)min_y=points_xy[i*2+1];
		if(points_xy[i*2+1] > max_y)max_y=points_xy[i*2+1];
	}
	for(int y=min_y; y < max_y; ++y){
		// find intersection points_xy
		int nodes=0;
		int j=count-1;
		for(int i=0; i < count; ++i){
			if((points_xy[i*2+1] <= y&&points_xy[j*2+1] > y)||
				(points_xy[j*2+1] <= y&&points_xy[i*2+1] > y)){
			
				int dx=points_xy[j*2+0]-points_xy[i*2+0];
				int dy=points_xy[j*2+1]-points_xy[i*2+1];
				node_x[nodes++]=points_xy[i*2+0]+((y-points_xy[i*2+1])* dx)/ dy ;
			}
			j=i;
		}
		// sort by x
		int xi=0;
		while(xi < nodes-1){
			if(node_x[xi] > node_x[xi+1]){
				int swap=node_x[xi];
				node_x[xi]=node_x[xi+1];
				node_x[xi+1]=swap;
				if(xi)--xi;
			} else{
				++xi;
			}
		}
		for(int i=0; i < nodes; i += 2){
			hline(node_x[i],y,node_x[i+1]-node_x[i],color);
		}
	}
}

void floodfill(int x,int y){
	#define FILLMAX 10000		/* FILLMAX depth of stack */
	#define FILLPUSH(Y,XL,XR,DY)	/* FILLPUSH new segment on stack */ \
		if(sp < stack+FILLMAX&&Y+(DY)>= 0&&Y+(DY)< internals->draw.height)\
			{ sp->y=Y; sp->xl=XL; sp->xr=XR; sp->dy=DY; ++sp; }
	#define FILLPOP(Y,XL,XR,DY)	/* FILLPOP segment off stack */ \
		{ --sp; Y=sp->y+(DY=sp->dy); XL=sp->xl; XR=sp->xr; }
	int color=internals->screen.color;
	/*
	* Filled horizontal segment of scanline y for xl<=x<=xr.
	* Parent segment was on line y-dy. dy=1 or -1
	*/
	struct segment_t{ int y,xl,xr,dy; };
	int l,x1,x2,dy;
	int ov;	/* old pixel value */
	struct segment_t stack[FILLMAX],*sp=stack;	/* stack of filled segments */
	ov=getpixel(x,y);		/* read pv at seed point */
	if(ov==color||x < 0||x >= internals->draw.width||y < 0||y >= internals->draw.height)return;
	FILLPUSH(y,x,x,1);			/* needed in some cases */
	FILLPUSH(y+1,x,x,-1);		/* seed segment (FILLPOPped 1st)*/
	while(sp > stack){
		/* FILLPOP segment off stack and fill a neighboring scan line */
		FILLPOP(y,x1,x2,dy);
		/*
		* segment of scan line y-dy for x1<=x<=x2 was previously filled,
		* now explore adjacent pixels in scan line y
		*/
		int xs;
		for(x=x1; x >= 0&&getpixel(x,y)== ov; --x)/* nothing */;
		hline(x+1,y,x1-x,color);
		if(x >= x1)goto skip;
		l=x+1;
		if(l < x1)FILLPUSH(y,l,x1-1,-dy);		/* leak on left? */
		x=x1+1;
		do{
			xs=x;
			for(; x < internals->draw.width&&getpixel(x,y)== ov; ++x)/* nothing */;
			hline(xs,y,x-xs,color);	
			FILLPUSH(y,l,x-1,dy);
			if(x > x2+1)FILLPUSH(y,x2+1,x-1,-dy);	/* leak on right? */
		skip:
			for(x++; x <= x2&&getpixel(x,y)!= ov; ++x);
			l=x;
		} while(x <= x2);
	}
	#undef FILLMAX
}

void boundaryfill(int x,int y,int boundary){
	#define FILLMAX 10000		/* FILLMAX depth of stack */
	#define FILLPUSH(Y,XL,XR,DY)	/* FILLPUSH new segment on stack */ \
		if(sp < stack+FILLMAX&&Y+(DY)>= 0&&Y+(DY)< internals->draw.height)\
			{ sp->y=Y; sp->xl=XL; sp->xr=XR; sp->dy=DY; ++sp; }
	#define FILLPOP(Y,XL,XR,DY)	/* FILLPOP segment off stack */ \
		{ --sp; Y=sp->y+(DY=sp->dy); XL=sp->xl; XR=sp->xr; }
	int color=internals->screen.color;
	/*
	* Filled horizontal segment of scanline y for xl<=x<=xr.
	* Parent segment was on line y-dy. dy=1 or -1
	*/
	struct segment_t{ int y,xl,xr,dy; };
	int l,x1,x2,dy;
	int ov;	/* old pixel value */
	struct segment_t stack[FILLMAX],*sp=stack;	/* stack of filled segments */
	ov=boundary;
	if(x < 0||x >= internals->draw.width||y < 0||y >= internals->draw.height)return;
	FILLPUSH(y,x,x,1);			/* needed in some cases */
	FILLPUSH(y+1,x,x,-1);		/* seed segment (FILLPOPped 1st)*/
	while(sp > stack){
		/* FILLPOP segment off stack and fill a neighboring scan line */
		FILLPOP(y,x1,x2,dy);
		/*
		* segment of scan line y-dy for x1<=x<=x2 was previously filled,
		* now explore adjacent pixels in scan line y
		*/
		int xs;
		for(x=x1; x >= 0&&getpixel(x,y)!= ov; --x)/* nothing */;
		hline(x+1,y,x1-x,color);
		if(x >= x1)goto skip;
		l=x+1;
		if(l < x1)FILLPUSH(y,l,x1-1,-dy);		/* leak on left? */
		x=x1+1;
		do{
			xs=x;
			for(; x < internals->draw.width&&getpixel(x,y)!= ov; ++x)/* nothing */;
			hline(xs,y,x-xs,color);	
			FILLPUSH(y,l,x-1,dy);
			if(x > x2+1)FILLPUSH(y,x2+1,x-1,-dy);	/* leak on right? */
		skip:
			for(x++; x <= x2&&getpixel(x,y)== ov; ++x);
			l=x;
		} while(x <= x2);
	}
	#undef FILLMAX
}

int keystate(enum keycode_t key){
	int index=(int)key;
	if(index >= 0&&index < KEYCOUNT){
		return internals->input.keystate[index];
	}
	return false;
}

enum keycode_t* readkeys(void){
	thread_mutex_lock(&internals->mutex);
	memset(internals->input.keybuffer,0,sizeof(internals->input.keybuffer0));
	if(internals->input.keybuffer==internals->input.keybuffer0){
		internals->input.keybuffer=internals->input.keybuffer1;
	} else{
		internals->input.keybuffer=internals->input.keybuffer0;
	}
	thread_mutex_unlock(&internals->mutex);
	return internals->input.keybuffer;
};

char const* readchars(void){
	thread_mutex_lock(&internals->mutex);
	memset(internals->input.charbuffer,0,sizeof(internals->input.charbuffer0));
	if(internals->input.charbuffer==internals->input.charbuffer0){
		internals->input.charbuffer=internals->input.charbuffer1;
	} else{
		internals->input.charbuffer=internals->input.charbuffer0;
	}
	thread_mutex_unlock(&internals->mutex);
	return internals->input.charbuffer;
}

int mousex(void){
	return internals->input.mouse_x;
}

int mousey(void){
	return internals->input.mouse_y;
}

int mouserelx(void){
	return internals->input.mouse_relx;
}

int mouserely(void){
	return internals->input.mouse_rely;
}

struct app_context_t{
	int argc;
	char** argv;
};

struct user_thread_context_t{
	struct app_context_t* app_context;
	int sound_buffer_size;
	thread_signal_t user_thread_initialized;
	thread_atomic_int_t user_thread_finished;
	thread_signal_t app_loop_finished;
	thread_signal_t user_thread_terminated;
};

int dosmain(int argc,char* argv[]);

static int user_thread_proc(void* user_data){
	struct user_thread_context_t* context=(struct user_thread_context_t*)user_data;
		
	internals_create(context->sound_buffer_size);
	thread_signal_raise(&context->user_thread_initialized);
	waitvbl();
	int result=dosmain(context->app_context->argc,context->app_context->argv);
	thread_atomic_int_store(&context->user_thread_finished,1);
	thread_signal_wait(&context->app_loop_finished,5000);
	internals_destroy();
	thread_signal_raise(&context->user_thread_terminated);
	return result;
}

static int app_proc(app_t* app,void* user_data){
	struct app_context_t* app_context=(struct app_context_t*)user_data;
 
	app_title(app,app_filename(app));
	bool fullscreen=true;
 
	int modargc=0;
	char* modargv[256];
	for(int i=0; i < app_context->argc; ++i){
		if(strcmp(app_context->argv[i],"--window")== 0){
			fullscreen=false;
		}
		else if(strcmp(app_context->argv[i],"-w")== 0){
			fullscreen=false;
		} else{
			if(modargc >= sizeof(modargv)/ sizeof(*modargv)){
				break;
			}
			modargv[modargc++]=app_context->argv[i];
		}
	}
	app_context->argc=modargc;
	app_context->argv=modargv;
	int pointer_width=0;
	int pointer_height=0;
	int pointer_hotspot_x=0;
	int pointer_hotspot_y=0;
	static APP_U32 pointer_pixels[256*256];
	app_pointer_default(app,&pointer_width,&pointer_height,pointer_pixels,&pointer_hotspot_x,&pointer_hotspot_y);
	app_screenmode(app,fullscreen ? APP_SCREENMODE_FULLSCREEN : APP_SCREENMODE_WINDOW);
	#ifdef DISABLE_SYSTEM_CURSOR
		APP_U32 blank=0;
		app_pointer(app,1,1,&blank,0,0);
	#else
		if(fullscreen){
			APP_U32 blank=0;
			app_pointer(app,1,1,&blank,0,0);
		} else{
			app_pointer(app,pointer_width,pointer_height,pointer_pixels,pointer_hotspot_x,pointer_hotspot_y);
		}
	#endif
	app_displays_t displays=app_displays(app);
	if(displays.count > 0){
		int disp=0;
		for(int i=0; i < displays.count; ++i){
			if(displays.displays[i].x==0&&displays.displays[i].y==0){
				disp=i;
				break;
			}
		}
		int scrwidth=displays.displays[disp].width-80;
		int scrheight=displays.displays[disp].height-80;
		int aspect_width=(int)((scrheight*4.25f)/ 3);
		int aspect_height=(int)((scrwidth*3)/ 4.25f);
		int target_width,target_height;
		if(aspect_height <= scrheight){
			target_width=scrwidth;
			target_height=aspect_height;
		} else{
			target_width=aspect_width;
			target_height=scrheight;
		}
		
		int x=displays.displays[disp].x+(displays.displays[disp].width-target_width)/ 2;
		int y=displays.displays[disp].y+(displays.displays[disp].height-target_height)/ 2;
		int w=target_width;
		int h=target_height;
		app_window_pos(app,x,y);
		app_window_size(app,w,h);
	}
	struct user_thread_context_t user_thread_context;
	user_thread_context.app_context=app_context;
	thread_signal_init(&user_thread_context.user_thread_initialized);
	thread_atomic_int_store(&user_thread_context.user_thread_finished,0);
	thread_signal_init(&user_thread_context.app_loop_finished);
	thread_signal_init(&user_thread_context.user_thread_terminated);
	thread_ptr_t user_thread=thread_create(user_thread_proc,&user_thread_context,
		THREAD_STACK_SIZE_DEFAULT);
	if(!thread_signal_wait(&user_thread_context.user_thread_initialized,5000)){
		thread_signal_term(&user_thread_context.user_thread_initialized);
		thread_signal_term(&user_thread_context.app_loop_finished);
		thread_signal_term(&user_thread_context.user_thread_terminated);
		return EXIT_FAILURE;
	}	
	#ifdef NULL_PLATFORM
		crtemu_pc_t* crt=NULL;
	#else
		crtemu_pc_t* crt=crtemu_pc_create(NULL);
	#endif
	// Create the frametimer instance,and set it to fixed 60hz update. This will ensure we never run faster than that,
	// even if the user have disabled vsync in their graphics card settings.
	frametimer_t* frametimer=frametimer_create(NULL);
	frametimer_lock_rate(frametimer,60);
	signalvbl();
	// Main loop
	static APP_U32 screen_xbgr[sizeof(internals->screen.buffer0)];
	int width=0;
	int height=0;
	int curs_vis=0;
	int curs_x=0;
	int curs_y=0;
	bool keystate[KEYCOUNT] ={ 0 };
	enum keycode_t keys[256] ={ (enum keycode_t)0 };
	char chars[256] ={ 0 };
	APP_U64 crt_time_us=0;
	APP_U64 prev_time=app_time_count(app);	
	while(!thread_atomic_int_load(&user_thread_context.user_thread_finished)){
		app_state_t app_state=app_yield(app);		
		frametimer_update(frametimer);
		int keys_index=0;
		memset(keys,0,sizeof(keys));
		int chars_index=0;
		memset(chars,0,sizeof(chars));
		float relx=0;
		float rely=0;
		app_input_t input=app_input(app);
		for(int i=0; i < input.count; ++i){
			app_input_event_t* event=&input.events[i];
			if(event->type==APP_INPUT_KEY_DOWN){
				int index=(int)event->data.key;
				if(index > 0&&index < KEYCOUNT){
					keystate[index]=true;
					if(keys_index < 255){
						keys[keys_index++]=(enum keycode_t)event->data.key;
					}
				}
				if(event->data.key==APP_KEY_F11){
					fullscreen=!fullscreen;
					app_screenmode(app,fullscreen ? APP_SCREENMODE_FULLSCREEN : APP_SCREENMODE_WINDOW);
					#ifdef DISABLE_SYSTEM_CURSOR
						APP_U32 blank=0;
						app_pointer(app,1,1,&blank,0,0);
					#else
						if(fullscreen){
							APP_U32 blank=0;
							app_pointer(app,1,1,&blank,0,0);
						} else{
							app_pointer(app,pointer_width,pointer_height,pointer_pixels,pointer_hotspot_x,pointer_hotspot_y);
						}
					#endif
				}
			} else if(event->type==APP_INPUT_KEY_UP){
				int index=(int)event->data.key;
				if(index >= 0&&index < KEYCOUNT){
					keystate[index]=false;
					if(keys_index < 255){
						keys[keys_index++]=(enum keycode_t)(((uint32_t)event->data.key)| KEY_MODIFIER_RELEASED);
					}
				}
			} else if(event->type==APP_INPUT_CHAR){
				if(event->data.char_code > 0){
					if(chars_index < 255){
						chars[chars_index++]=event->data.char_code;
					}
				}
			} else if(event->type==APP_INPUT_MOUSE_DELTA){
				relx += event->data.mouse_delta.x;
				rely += event->data.mouse_delta.y;
			}
		}
		internals->input.mouse_relx=(int)relx;
		internals->input.mouse_rely=(int)rely;
		// Check if the close button on the window was clicked (or Alt+F4 was pressed)
		if(app_state==APP_STATE_EXIT_REQUESTED){
			// Signal that we need to force the user thread to exit
			thread_atomic_int_store(&internals->exit_flag,1);
			signalvbl();
			break;
		}
		// Copy data from user thread
		thread_mutex_lock(&internals->mutex);
		width=internals->screen.width;
		height=internals->screen.height;
		uint8_t* internals_screen=internals->screen.buffer;
		uint32_t* font=internals->screen.font;
		if(internals->screen.doublebuffer){
			if(internals->screen.buffer==internals->screen.buffer0){
				internals_screen=internals->screen.buffer1;
			} else{
				internals_screen=internals->screen.buffer0;
			}
		}
		static uint8_t screen[sizeof(internals->screen.buffer0)];
		if(font){
			memcpy(screen,internals_screen,width*height*2);
		} else{
			memcpy(screen,internals_screen,width*height);
		}
		static uint32_t palette[256];
		memcpy(palette,internals->screen.palette,1024);
		bool curs=internals->conio.curs;
		if(internals->conio.x != curs_x||internals->conio.y != curs_y){
			curs_x=internals->conio.x;
			curs_y=internals->conio.y;
			curs_vis=0;
		}
		int mouse_x=app_pointer_x(app);
		int mouse_y=app_pointer_y(app);
		if(crt){
			crtemu_pc_coordinates_window_to_bitmap(crt,width*internals->screen.cellwidth,
				height*internals->screen.cellheight,&mouse_x,&mouse_y);
		}
		internals->input.mouse_x=mouse_x/internals->screen.cellwidth;
		internals->input.mouse_y=mouse_y/internals->screen.cellheight;
		memcpy(internals->input.keystate,keystate,sizeof(internals->input.keystate));
		enum keycode_t* internals_keybuffer;
		if(internals->input.keybuffer==internals->input.keybuffer0){
			internals_keybuffer=internals->input.keybuffer1;
		} else{
			internals_keybuffer=internals->input.keybuffer0;
		}
		enum keycode_t* keyin=keys;
		enum keycode_t* keyout=internals_keybuffer;
		enum keycode_t* keyend=internals_keybuffer+sizeof(internals->input.keybuffer0)/ sizeof(*internals->input.keybuffer0)- 1;
		while(*keyout&&keyout < keyend){
			++keyout;
		}
		while(*keyin){
			if(keyout >= keyend){
				memmove(internals_keybuffer+1,internals_keybuffer,sizeof(internals->input.keybuffer0)/ sizeof(*internals->input.keybuffer0)- 1);
				--keyout;
			}
			*keyout++=*keyin++;						
		}
		*keyout=KEY_INVALID;
		char* internals_charbuffer;
		if(internals->input.charbuffer==internals->input.charbuffer0){
			internals_charbuffer=internals->input.charbuffer1;
		} else{
			internals_charbuffer=internals->input.charbuffer0;
		}
		char* charin=chars;
		char* charout=internals_charbuffer;
		char* charend=internals_charbuffer+sizeof(internals->input.charbuffer0)/ sizeof(*internals->input.charbuffer0)- 1;
		while(*charout&&charout < charend){
			++charout;
		}
		while(*charin){
			if(charout >= charend){
				memmove(internals_charbuffer+1,internals_charbuffer,sizeof(internals->input.charbuffer0)/ sizeof(*internals->input.charbuffer0)- 1);
				--charout;
			}
			*charout++=*charin++;						
		}
		*charout='\0';
		thread_mutex_unlock(&internals->mutex);
		// Signal to the game that the frame is completed,and that we are just starting the next one
		signalvbl();
		// Render screen buffer
		if(font){
			memset(screen_xbgr,0,sizeof(screen_xbgr));
			uint32_t const* data=font;
			int chr_width=*data++;
			int chr_height=*data++;
			int chr_baseline=*data++;
			(void)chr_baseline;
			int chr_mod=256/chr_width;
			for(int y=0; y < height; ++y){
				for(int x=0; x < width; ++x){
					uint8_t c=screen[(x+y*width)* 2+0];
					uint8_t attr=screen[(x+y*width)* 2+1];
					int fg=(attr & 0xf);
					int bg=((attr >> 4)& 0xf);
					int sx=(c % chr_mod)* chr_width;
					int sy=(c/chr_mod)* chr_height;
					int dx=x*chr_width;
					int dy=y*chr_height;
					for(int iy=0; iy < chr_height; ++iy){
						for(int ix=0; ix < chr_width; ++ix){
							int v=(sx+ix)/ 32;
							int u=(sx+ix)- (v*32);
							uint32_t b=data[v+(sy+iy)* 8];
							int xp=dx+ix;
							int yp=dy+iy;
							if(b & (1 << u)){
								screen_xbgr[xp+yp*(width*chr_width)]=palette[fg];
							} else{
								screen_xbgr[xp+yp*(width*chr_width)]=palette[bg];
							}
						}
					}
				}
			}
			++curs_vis;
			if(curs&&curs_x >= 0&&curs_x < width&&curs_y >= 0&&curs_y < height){
				int vis=(curs_vis % 50)< 25;
				if(vis){
					int xp=curs_x*chr_width;
					int yp=curs_y*chr_height;
					int cs=chr_height==16 ? 13 : 7;
					int ce=chr_height==16 ? 15 : 9;
					APP_U32 col=palette[7];
					for(int y=cs; y < ce; ++y){
						for(int x=0; x < chr_width; ++x){
							screen_xbgr[(x+xp)+ (y+yp)* width*chr_width]=col;
						}
					}
				}
			}
			width *= chr_width;
			height *= chr_height;
		} else{
			for(int y=0; y < height; ++y){
				for(int x=0; x < width; ++x){
					screen_xbgr[x+y*width]=palette[screen[x+y*width]];
				}
			}
		}
		if(!app_has_focus(app)){
			continue;
		}
		APP_U64 time=app_time_count(app);
		APP_U64 freq=app_time_freq(app);
		APP_U64 delta_time_us=(time-prev_time)/ ((freq > 1000000 ? freq/1000000 : 1));
		prev_time=time;
		crt_time_us += delta_time_us;
		if(crt){
			#ifndef DISABLE_SCREEN_FRAME
				crtemu_pc_present(crt,crt_time_us,screen_xbgr,width,height,0xffffff,0xff1a1a1a);
			#else
				crtemu_pc_present(crt,crt_time_us,screen_xbgr,width,height,0xffffff,0xff000000);
			#endif
		}
		app_present(app,NULL,1,1,0xffffff,0xff1a1a1a);
	}
	thread_signal_raise(&user_thread_context.app_loop_finished); 
	int user_exit=thread_signal_wait(&user_thread_context.user_thread_terminated,170);
	if(!user_exit){
		for(int i=0; i < 60; ++i){
			APP_U64 time=app_time_count(app);
			APP_U64 delta_time_us=(time-prev_time)/ (app_time_freq(app)/ 1000000);
			prev_time=time;
			crt_time_us += delta_time_us;
			int v=((60-i)* 255)/ 60;
			uint32_t fade=(v << 16)| v << 8 | v;
			if(crt){
				crtemu_pc_present(crt,crt_time_us,screen_xbgr,width,height,fade,0xff1a1a1a);
			}
			app_present(app,NULL,1,1,0xffffff,0xff1a1a1a);
			frametimer_update(frametimer);
		}
		user_exit=thread_signal_wait(&user_thread_context.user_thread_terminated,30);
		if(!user_exit){
			exit(EXIT_FAILURE);
		}
	}
	thread_signal_term(&user_thread_context.user_thread_initialized);
	thread_signal_term(&user_thread_context.app_loop_finished);
	thread_signal_term(&user_thread_context.user_thread_terminated);
	frametimer_destroy(frametimer);
	if(crt){
		crtemu_pc_destroy(crt);
	}
	return thread_join(user_thread);
}

#define APP_IMPLEMENTATION
#ifdef NULL_PLATFORM
	#define APP_NULL
#elif defined(_WIN32)
	#define APP_WINDOWS
#else
	#define APP_SDL
#endif
#define APP_LOG(ctx,level,message)
#include "app.h"

#define CRTEMU_PC_IMPLEMENTATION
#include "crtemu_pc.h"

#define DR_WAV_IMPLEMENTATION

#define FRAMETIMER_IMPLEMENTATION
#include "frametimer.h"

#define MUS_IMPLEMENTATION

#define OPBLIB_IMPLEMENTATION
#pragma warning(push)
#pragma warning(disable: 4189)
#pragma warning(disable: 4204)
#pragma warning(disable: 4244)
#pragma warning(disable: 4296)
#pragma warning(disable: 4388)
#pragma warning(disable: 4457)
#pragma warning(disable: 4706)
#pragma warning(pop)

#define OPL_IMPLEMENTATION
#pragma warning(push)
#pragma warning(disable: 4100)
#pragma warning(disable: 4127)
#pragma warning(disable: 4189)
#pragma warning(disable: 4242)
#pragma warning(disable: 4244)
#pragma warning(disable: 4245)
#pragma warning(pop)

#define THREAD_IMPLEMENTATION
#include "thread.h"

#define TSF_IMPLEMENTATION
#define TSF_POW	pow
#define TSF_POWF	(float)pow
#define TSF_EXPF	(float)exp
#define TSF_LOG	log
#define TSF_TAN	tan
#define TSF_LOG10 log10
#define TSF_SQRT (float)sqrt
#define TSF_SQRTF (float)sqrt
#include <math.h>

#define TML_IMPLEMENTATION
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable: 4201)
#endif
#ifdef _WIN32
#pragma warning(pop)
#endif

bool app_has_focus(app_t* app){
	#ifdef ALWAYS_UPDATE
		return true;
	#else
		#ifndef NULL_PLATFORM
			return app->has_focus;
		#else
			return true;
		#endif
	#endif
}

#include <inttypes.h>

int main(int argc,char** argv){
	(void)argc,(void)argv;
	struct app_context_t app_context;
	app_context.argc=argc;
	app_context.argv=argv;
	return app_run(app_proc,&app_context,NULL,NULL,NULL);
}

#ifdef _WIN32
	// pass-through so the program will build with either /SUBSYSTEM:WINDOWS or /SUBSYSTEM:CONSOLE
	int WINAPI __stdcall WinMain(HINSTANCE a,HINSTANCE b,char* c,int d){
		(void)a,b,c,d;
		return main(__argc,__argv);
	}
#endif

#endif /* DOS_IMPLEMENTATION */

#ifndef NO_MAIN_DEF
	#define main dosmain
#endif
