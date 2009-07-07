#pragma once

namespace Plum
{
	// Take SDL's typedefs, but make them our own.
	// That way if for some reason, SDL is removed, it's an easy replace here.
	typedef Sint8 s8;
	typedef Sint16 s16;
	typedef Sint32 s32;
	typedef Uint8 u8;
	typedef Uint16 u16;
	typedef Uint32 u32;
}