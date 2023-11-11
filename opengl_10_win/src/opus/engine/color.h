#pragma once
#include <gfx/base.h>

internal Color
lerp_color(Color a, Color b, float32 t);

global const Color ColorButtonInactive      =  { .r = 0x9e, .g = 0xa8, .b = 0xad, .a =0xff };
global const Color ColorButtonInactiveHover =  { .r = 0x7f, .g = 0x8a, .b = 0x94, .a =0xff };
global const Color ColorButtonActive        =  { .r = 0xeb, .g = 0xf4, .b = 0xf7, .a =0xff };
global const Color ColorButtonActiveHover   =  { .r = 0xeb, .g = 0xf4, .b = 0xf7, .a =0xff };
global const Color ColorButtonFont          =  { .r = 0x19, .g = 0x1c, .b = 0x21, .a =0xff };

global const Color ColorClear        = { .r = 0x0f, .g = 0x17, .b = 0x2a, .a = 0x00 };
global const Color ColorBlack 	     = { .r = 0x00, .g = 0x00, .b = 0x00, .a = 0xff };
global const Color ColorInvisible 	 = { .r = 0x00, .g = 0x00, .b = 0x00, .a = 0x00 };
global const Color ColorWhite 	     = { .r = 0xff, .g = 0xff, .b = 0xff, .a = 0xff };
global const Color ColorRedPastel    = { .r = 0xff, .g = 0x8a, .b = 0x8a, .a = 0xff };
global const Color ColorYellowPastel = { .r = 0xf1, .g = 0xf7, .b = 0xb5, .a = 0xff };
global const Color ColorGreenPastel  = { .r = 168,  .g = 209, .b = 209, .a = 0xff };
global const Color ColorBluePastel   = { .r = 158,  .g = 161, .b = 212, .a = 0xff };

global const Color ColorShadow = { .r = 0,  .g = 0, .b = 0, .a = 173};

global const Color ColorHealthBarFilled = { .r = 201,  .g = 28, .b = 16, .a = 0xff };
global const Color ColorHealthBarEmpty  = { .r = 46,    .g = 7,  .b = 5, .a = 0xff };

global const Color ColorItemPopup      = { .r = 0x12, .g = 0x15, .b = 0x25, .a = 0xff };
global const Color ColorItemPopupInner = { .r = 0x18, .g = 0x22, .b = 0x32, .a = 0xff };

global const Color ColorRed50  = {.r = 0xfe, .g = 0xf2, .b = 0xf2, .a = 0xff};
global const Color ColorRed100 = {.r = 0xfe, .g = 0xe2, .b = 0xe2, .a = 0xff};
global const Color ColorRed200 = {.r = 0xfe, .g = 0xca, .b = 0xca, .a = 0xff};
global const Color ColorRed300 = {.r = 0xfc, .g = 0xa5, .b = 0xa5, .a = 0xff};
global const Color ColorRed400 = {.r = 0xf8, .g = 0x71, .b = 0x71, .a = 0xff};
global const Color ColorRed500 = {.r = 0xef, .g = 0x44, .b = 0x44, .a = 0xff};
global const Color ColorRed600 = {.r = 0xdc, .g = 0x26, .b = 0x26, .a = 0xff};
global const Color ColorRed700 = {.r = 0xb9, .g = 0x1c, .b = 0x1c, .a = 0xff};
global const Color ColorRed800 = {.r = 0x99, .g = 0x1b, .b = 0x1b, .a = 0xff};
global const Color ColorRed900 = {.r = 0x7f, .g = 0x1d, .b = 0x1d, .a = 0xff};

global const Color ColorAmber50  = {.r = 0xff, .g = 0xfb, .b = 0xeb, .a = 0xff};
global const Color ColorAmber100 = {.r = 0xfe, .g = 0xf3, .b = 0xc7, .a = 0xff};
global const Color ColorAmber200 = {.r = 0xfd, .g = 0xe6, .b = 0x8a, .a = 0xff};
global const Color ColorAmber300 = {.r = 0xfc, .g = 0xd3, .b = 0x4d, .a = 0xff};
global const Color ColorAmber400 = {.r = 0xfb, .g = 0xbf, .b = 0x24, .a = 0xff};
global const Color ColorAmber500 = {.r = 0xf5, .g = 0x9e, .b = 0x0b, .a = 0xff};
global const Color ColorAmber600 = {.r = 0xd9, .g = 0x77, .b = 0x06, .a = 0xff};
global const Color ColorAmber700 = {.r = 0xb4, .g = 0x53, .b = 0x09, .a = 0xff};
global const Color ColorAmber800 = {.r = 0x92, .g = 0x40, .b = 0x0e, .a = 0xff};
global const Color ColorAmber900 = {.r = 0x78, .g = 0x35, .b = 0x0f, .a = 0xff};

global const Color ColorYellow50  = { .r = 0xfe, .g = 0xfc, .b = 0xe8, .a = 0xff };
global const Color ColorYellow100 = { .r = 0xfe, .g = 0xf9, .b = 0xc3, .a = 0xff };
global const Color ColorYellow200 = { .r = 0xfe, .g = 0xf0, .b = 0x8a, .a = 0xff };
global const Color ColorYellow300 = { .r = 0xfd, .g = 0xe0, .b = 0x47, .a = 0xff };
global const Color ColorYellow400 = { .r = 0xfa, .g = 0xcc, .b = 0x15, .a = 0xff };
global const Color ColorYellow500 = { .r = 0xea, .g = 0xb3, .b = 0x08, .a = 0xff };
global const Color ColorYellow600 = { .r = 0xca, .g = 0x8a, .b = 0x04, .a = 0xff };
global const Color ColorYellow700 = { .r = 0xa1, .g = 0x62, .b = 0x07, .a = 0xff };
global const Color ColorYellow800 = { .r = 0x85, .g = 0x4d, .b = 0x0e, .a = 0xff };
global const Color ColorYellow900 = { .r = 0x71, .g = 0x3f, .b = 0x12, .a = 0xff };

global const Color ColorWhite100 = { .r = 0xf8, .g = 0xff, .b = 0xff, .a = 0xff };

global const Color ColorSlate50  = { .r = 0xf8, .g = 0xfa, .b = 0xfc, .a = 0xff };
global const Color ColorSlate100 = { .r = 0xf1, .g = 0xf5, .b = 0xf9, .a = 0xff };
global const Color ColorSlate200 = { .r = 0xe2, .g = 0xe8, .b = 0xf0, .a = 0xff };
global const Color ColorSlate300 = { .r = 0xcb, .g = 0xd5, .b = 0xe1, .a = 0xff };
global const Color ColorSlate400 = { .r = 0x94, .g = 0xa3, .b = 0xb8, .a = 0xff };
global const Color ColorSlate500 = { .r = 0x64, .g = 0x74, .b = 0x8b, .a = 0xff };
global const Color ColorSlate600 = { .r = 0x47, .g = 0x55, .b = 0x69, .a = 0xff };
global const Color ColorSlate700 = { .r = 0x33, .g = 0x41, .b = 0x55, .a = 0xff };
global const Color ColorSlate800 = { .r = 0x1e, .g = 0x29, .b = 0x3b, .a = 0xff };
global const Color ColorSlate900 = { .r = 0x0f, .g = 0x17, .b = 0x2a, .a = 0xff };

global const Color ColorSlate300Fade = { .r = 0xcb, .g = 0xd5, .b = 0xe1, .a = 0xaa };