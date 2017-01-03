#ifndef SDK_H_INCLUDED
#define SDK_H_INCLUDED
#include "vector.h"


inline void**& getvtable(void* inst, size_t offset = 0)
{
	return *reinterpret_cast<void***>((size_t)inst + offset);
}
inline const void** getvtable(const void* inst, size_t offset = 0)
{
	return *reinterpret_cast<const void***>((size_t)inst + offset);
}
template< typename Fn >
inline Fn getvfunc(const void* inst, size_t index, size_t offset = 0)
{
	return reinterpret_cast<Fn>(getvtable(inst, offset)[index]);
}

class CBaseEntity
{
public:
};
class CEngineClient
{
public:
	typedef struct player_info_s
		{
	unsigned long long unknown;
	unsigned long long unknown1;
	char			name[128];
	int				userID;
	char			guid[33];
	int32_t			friendsID;
	char			friendsName[128];
	char			_pad[0x28];


	} player_info_t;

    bool GetPlayerInfo(int iIndex, player_info_t *pInfo)
    {
        typedef bool(* oGetPlayerInfo)(void*, int, player_info_t*);
        return getvfunc<oGetPlayerInfo>(this, 8)(this, iIndex, pInfo);
    }

    int GetLocalPlayer(void)
    {
        typedef int(* oGetLocalPlayer)(void*);
        return getvfunc< oGetLocalPlayer >(this, 12)(this);
    }
};

class CEntityList
{
public:
	CBaseEntity* GetClientEntity(int index)
	{
        typedef CBaseEntity* (* oGetClientEntity)(void*, int);
        return getvfunc<oGetClientEntity>(this, 3)(this, index);
	}

};

class CDebugOverlay
{
public:
    bool ScreenPosition(const Vector& vIn, Vector& vOut)
    {
        typedef bool(* oScreenPosition)(void*, const Vector&, Vector&);
        return getvfunc<oScreenPosition>(this, 11)(this, vIn, vOut);
    }
};
class ISurface
{
public:
		void DrawSetColor(int r, int g, int b, int a)
		{
			typedef void(* oDrawSetColor)(void*, int, int, int, int);
			getvfunc<oDrawSetColor>(this, 14)(this, r, g, b, a);
		}
		void DrawFilledRect(int x0, int y0, int x1, int y1)
		{
			typedef void(* oDrawFilledRect)(void*, int, int, int, int);
			getvfunc<oDrawFilledRect>(this, 16)(this, x0, y0, x1, y1);
		}
		void DrawLine(int x0, int y0, int x1, int y1)
		{
			typedef void(* oDrawLine)(void*, int, int, int, int);
			getvfunc<oDrawLine>(this, 19)(this, x0, y0, x1, y1);
		}
		void DrawSetTextFont(unsigned long long font)
		{
			typedef void(* oDrawSetTextFont)(void*, unsigned long long);
			getvfunc<oDrawSetTextFont>(this, 23)(this, font);
		}
		void DrawSetTextColor(int r, int g, int b, int a)
		{
			typedef void(* oDrawSetTextColor)(void*, int, int, int, int);
			getvfunc<oDrawSetTextColor>(this, 24)(this, r, g, b, a);
		}
		void DrawSetTextPos(int x, int y)
		{
			typedef void(* oDrawSetTextPos)(void*, int, int);
			getvfunc<oDrawSetTextPos>(this, 26)(this, x, y);
		}
		void DrawPrintText(const wchar_t *text, int textLen)
		{
			typedef void(* oDrawPrintText)(void*, const wchar_t *, int, int);
			return getvfunc<oDrawPrintText>(this, 28)(this, text, textLen, 0);
		}
		unsigned long long CreateFont()
		{
			typedef unsigned long long(* oCreateFont)(void*);
			return getvfunc<oCreateFont>(this, 71)(this);
		}
		void SetFontGlyphSet(unsigned long long &font, const char *FontName, int tall, int weight, int blur, int scanlines, int flags)
		{
			typedef void(* oSetFontGlyphSet)(void*, unsigned long long, const char*, int, int, int, int, int, int, int);
			getvfunc<oSetFontGlyphSet>(this, 72)(this, font, FontName, tall, weight, blur, scanlines, flags, 0, 0);
		}
		void GetTextSize(unsigned long long font, const wchar_t *text, int &wide, int &tall)
		{
			typedef void(* oGetTextSize)(void*, unsigned long long font, const wchar_t *text, int &wide, int &tall);
			getvfunc<oGetTextSize>(this, 79)(this, font, text, wide, tall);
		}
};

class IPanel
{
public:
	const char *GetName(unsigned long long vguiPanel)
    {
			typedef const char* (* oGetName)(void*, unsigned long long);
			return getvfunc<oGetName>(this, 37)(this, vguiPanel);
    }

};

class HLClient
{
public:

};

#endif // SDK_H_INCLUDED
