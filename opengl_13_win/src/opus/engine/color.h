#pragma once
#include <gfx/base.h>

internal Color
lerp_color(Color Colora, Color Colorb, float32 t);

global const Color ColorClear    = { .r = 0x0f, .g = 0x17, .b = 0x2a, .a = 0x00 };
global const Color ColorBlack    = { .r = 0x00, .g = 0x00, .b = 0x00, .a = 0xff };
global const Color ColorInvisible  = { .r = 0x00, .g = 0x00, .b = 0x00, .a = 0x00 };
global const Color ColorWhite    = { .r = 0xff, .g = 0xff, .b = 0xff, .a = 0xff };
global const Color ColorRedPastel  = { .r = 0xff, .g = 0x8a, .b = 0x8a, .a = 0xff };
global const Color ColorYellowPastel = { .r = 0xf1, .g = 0xf7, .b = 0xb5, .a = 0xff };
global const Color ColorGreenPastel = { .r = 168, .g = 209, .b = 209, .a = 0xff };
global const Color ColorBluePastel  = { .r = 158, .g = 161, .b = 212, .a = 0xff };
global const Color ColorWhite100 = { .r = 0xf8, .g = 0xff, .b = 0xff, .a = 0xff };
global const Color ColorWhite100A = { .r = 0xf8, .g = 0xff, .b = 0xff, .a = 0x55 };
global const Color ColorSlate50 = { .r = 0xf8, .g = 0xfa, .b = 0xfc, .a = 0xff };
global const Color ColorSlate100 = { .r = 0xf1, .g = 0xf5, .b = 0xf9, .a = 0xff };
global const Color ColorSlate200 = { .r = 0xe2, .g = 0xe8, .b = 0xf0, .a = 0xff };
global const Color ColorSlate300 = { .r = 0xcb, .g = 0xd5, .b = 0xe1, .a = 0xff };
global const Color ColorSlate400 = { .r = 0x94, .g = 0xa3, .b = 0xb8, .a = 0xff };
global const Color ColorSlate500 = { .r = 0x64, .g = 0x74, .b = 0x8b, .a = 0xff };
global const Color ColorSlate600 = { .r = 0x47, .g = 0x55, .b = 0x69, .a = 0xff };
global const Color ColorSlate700 = { .r = 0x33, .g = 0x41, .b = 0x55, .a = 0xff };
global const Color ColorSlate800 = { .r = 0x1e, .g = 0x29, .b = 0x3b, .a = 0xff };
global const Color ColorSlate800_Transparent = { .r = 0x1e, .g = 0x29, .b = 0x3b, .a = 0xbb };
global const Color ColorSlate900 = { .r = 0x0f, .g = 0x17, .b = 0x2a, .a = 0xff };
global const Color ColorRed = { .r= 0x44, .g = 0x33, .b=0x6 , .a = 0xff };
global const Color ColorRed50 = { .r= 0xff, .g = 0xeb, .b=0xee, .a = 0xff };
global const Color ColorRed100 = { .r= 0xff, .g = 0xcd, .b=0xd2, .a = 0xff };
global const Color ColorRed200 = { .r= 0xef, .g = 0x9a, .b=0x9a, .a = 0xff };
global const Color ColorRed300 = { .r= 0xe5, .g = 0x73, .b=0x73, .a = 0xff };
global const Color ColorRed400 = { .r= 0xef, .g = 0x53, .b=0x50, .a = 0xff };
global const Color ColorRed500 = { .r= 0xf4, .g = 0x43, .b=0x36, .a = 0xff };
global const Color ColorRed600 = { .r= 0xe5, .g = 0x39, .b=0x35, .a = 0xff };
global const Color ColorRed700 = { .r= 0xd3, .g = 0x2f, .b=0x2f, .a = 0xff };
global const Color ColorRed800 = { .r= 0xc6, .g = 0x28, .b=0x28, .a = 0xff };
global const Color ColorRed900 = { .r= 0xb7, .g = 0x1c, .b=0x1c, .a = 0xff };
global const Color ColorRed100A = { .r= 0xff, .g = 0x8a, .b=0x80, .a = 0xff };
global const Color ColorRed200A = { .r= 0xff, .g = 0x52, .b=0x52, .a = 0xff };
global const Color ColorRed400A = { .r= 0xff, .g = 0x17, .b=0x44, .a = 0xff };
global const Color ColorRed700A = { .r= 0xd5, .g = 0x00, .b=0x00, .a = 0xff };
global const Color ColorPink = { .r= 0x91, .g = 0xe6, .b=0x3 , .a = 0xff };
global const Color ColorPink50 = { .r= 0xfc, .g = 0xe4, .b=0xec, .a = 0xff };
global const Color ColorPink100 = { .r= 0xf8, .g = 0xbb, .b=0xd0, .a = 0xff };
global const Color ColorPink200 = { .r= 0xf4, .g = 0x8f, .b=0xb1, .a = 0xff };
global const Color ColorPink300 = { .r= 0xf0, .g = 0x62, .b=0x92, .a = 0xff };
global const Color ColorPink400 = { .r= 0xec, .g = 0x40, .b=0x7a, .a = 0xff };
global const Color ColorPink500 = { .r= 0xe9, .g = 0x1e, .b=0x63, .a = 0xff };
global const Color ColorPink600 = { .r= 0xd8, .g = 0x1b, .b=0x60, .a = 0xff };
global const Color ColorPink700 = { .r= 0xc2, .g = 0x18, .b=0x5b, .a = 0xff };
global const Color ColorPink800 = { .r= 0xad, .g = 0x14, .b=0x57, .a = 0xff };
global const Color ColorPink900 = { .r= 0x88, .g = 0x0e, .b=0x4f, .a = 0xff };
global const Color ColorPink100A = { .r= 0xff, .g = 0x80, .b=0xab, .a = 0xff };
global const Color ColorPink200A = { .r= 0xff, .g = 0x40, .b=0x81, .a = 0xff };
global const Color ColorPink400A = { .r= 0xf5, .g = 0x00, .b=0x57, .a = 0xff };
global const Color ColorPink700A = { .r= 0xc5, .g = 0x11, .b=0x62, .a = 0xff };
global const Color ColorPurple = { .r= 0xC2, .g = 0x7b, .b=0x0 , .a = 0xff };
global const Color ColorPurple50 = { .r= 0xf3, .g = 0xe5, .b=0xf5, .a = 0xff };
global const Color ColorPurple100 = { .r= 0xe1, .g = 0xbe, .b=0xe7, .a = 0xff };
global const Color ColorPurple200 = { .r= 0xce, .g = 0x93, .b=0xd8, .a = 0xff };
global const Color ColorPurple300 = { .r= 0xba, .g = 0x68, .b=0xc8, .a = 0xff };
global const Color ColorPurple400 = { .r= 0xab, .g = 0x47, .b=0xbc, .a = 0xff };
global const Color ColorPurple500 = { .r= 0x9c, .g = 0x27, .b=0xb0, .a = 0xff };
global const Color ColorPurple600 = { .r= 0x8e, .g = 0x24, .b=0xaa, .a = 0xff };
global const Color ColorPurple700 = { .r= 0x7b, .g = 0x1f, .b=0xa2, .a = 0xff };
global const Color ColorPurple800 = { .r= 0x6a, .g = 0x1b, .b=0x9a, .a = 0xff };
global const Color ColorPurple900 = { .r= 0x4a, .g = 0x14, .b=0x8c, .a = 0xff };
global const Color ColorPurple100A = { .r= 0xea, .g = 0x80, .b=0xfc, .a = 0xff };
global const Color ColorPurple200A = { .r= 0xe0, .g = 0x40, .b=0xfb, .a = 0xff };
global const Color ColorPurple400A = { .r= 0xd5, .g = 0x00, .b=0xf9, .a = 0xff };
global const Color ColorPurple700A = { .r= 0xaa, .g = 0x00, .b=0xff, .a = 0xff };
global const Color ColorDeepPurple = { .r= 0x67, .g = 0x3a, .b=0xb7, .a = 0xff };
global const Color ColorDeepPurple50 = { .r= 0xed, .g = 0xe7, .b=0xf6, .a = 0xff };
global const Color ColorDeepPurple100 = { .r= 0xd1, .g = 0xc4, .b=0xe9, .a = 0xff };
global const Color ColorDeepPurple200 = { .r= 0xb3, .g = 0x9d, .b=0xdb, .a = 0xff };
global const Color ColorDeepPurple300 = { .r= 0x95, .g = 0x75, .b=0xcd, .a = 0xff };
global const Color ColorDeepPurple400 = { .r= 0x7e, .g = 0x57, .b=0xc2, .a = 0xff };
global const Color ColorDeepPurple500 = { .r= 0x67, .g = 0x3a, .b=0xb7, .a = 0xff };
global const Color ColorDeepPurple600 = { .r= 0x5e, .g = 0x35, .b=0xb1, .a = 0xff };
global const Color ColorDeepPurple700 = { .r= 0x51, .g = 0x2d, .b=0xa8, .a = 0xff };
global const Color ColorDeepPurple800 = { .r= 0x45, .g = 0x27, .b=0xa0, .a = 0xff };
global const Color ColorDeepPurple900 = { .r= 0x31, .g = 0x1b, .b=0x92, .a = 0xff };
global const Color ColorDeepPurple100A = { .r= 0xb3, .g = 0x88, .b=0xff, .a = 0xff };
global const Color ColorDeepPurple200A = { .r= 0x7c, .g = 0x4d, .b=0xff, .a = 0xff };
global const Color ColorDeepPurple400A = { .r= 0x65, .g = 0x1f, .b=0xff, .a = 0xff };
global const Color ColorDeepPurple700A = { .r= 0x62, .g = 0x00, .b=0xea, .a = 0xff };
global const Color ColorIndigo = { .r= 0xF5, .g = 0x1b, .b=0x5 , .a = 0xff };
global const Color ColorIndigo50 = { .r= 0xe8, .g = 0xea, .b=0xf6, .a = 0xff };
global const Color ColorIndigo100 = { .r= 0xc5, .g = 0xca, .b=0xe9, .a = 0xff };
global const Color ColorIndigo200 = { .r= 0x9f, .g = 0xa8, .b=0xda, .a = 0xff };
global const Color ColorIndigo300 = { .r= 0x79, .g = 0x86, .b=0xcb, .a = 0xff };
global const Color ColorIndigo400 = { .r= 0x5c, .g = 0x6b, .b=0xc0, .a = 0xff };
global const Color ColorIndigo500 = { .r= 0x3f, .g = 0x51, .b=0xb5, .a = 0xff };
global const Color ColorIndigo600 = { .r= 0x39, .g = 0x49, .b=0xab, .a = 0xff };
global const Color ColorIndigo700 = { .r= 0x30, .g = 0x3f, .b=0x9f, .a = 0xff };
global const Color ColorIndigo800 = { .r= 0x28, .g = 0x35, .b=0x93, .a = 0xff };
global const Color ColorIndigo900 = { .r= 0x1a, .g = 0x23, .b=0x7e, .a = 0xff };
global const Color ColorIndigo100A = { .r= 0x8c, .g = 0x9e, .b=0xff, .a = 0xff };
global const Color ColorIndigo200A = { .r= 0x53, .g = 0x6d, .b=0xfe, .a = 0xff };
global const Color ColorIndigo400A = { .r= 0x3d, .g = 0x5a, .b=0xfe, .a = 0xff };
global const Color ColorIndigo700A = { .r= 0x30, .g = 0x4f, .b=0xfe, .a = 0xff };
global const Color ColorBlue = { .r= 0x19, .g = 0x6f, .b=0x3 , .a = 0xff };
global const Color ColorBlue50 = { .r= 0xe3, .g = 0xf2, .b=0xfd, .a = 0xff };
global const Color ColorBlue100 = { .r= 0xbb, .g = 0xde, .b=0xfb, .a = 0xff };
global const Color ColorBlue200 = { .r= 0x90, .g = 0xca, .b=0xf9, .a = 0xff };
global const Color ColorBlue300 = { .r= 0x64, .g = 0xb5, .b=0xf6, .a = 0xff };
global const Color ColorBlue400 = { .r= 0x42, .g = 0xa5, .b=0xf5, .a = 0xff };
global const Color ColorBlue500 = { .r= 0x21, .g = 0x96, .b=0xf3, .a = 0xff };
global const Color ColorBlue600 = { .r= 0x1e, .g = 0x88, .b=0xe5, .a = 0xff };
global const Color ColorBlue700 = { .r= 0x19, .g = 0x76, .b=0xd2, .a = 0xff };
global const Color ColorBlue800 = { .r= 0x15, .g = 0x65, .b=0xc0, .a = 0xff };
global const Color ColorBlue900 = { .r= 0x0d, .g = 0x47, .b=0xa1, .a = 0xff };
global const Color ColorBlue100A = { .r= 0x82, .g = 0xb1, .b=0xff, .a = 0xff };
global const Color ColorBlue200A = { .r= 0x44, .g = 0x8a, .b=0xff, .a = 0x55 };
global const Color ColorBlue400A = { .r= 0x29, .g = 0x79, .b=0xff, .a = 0xaa };
global const Color ColorBlue700A = { .r= 0x29, .g = 0x62, .b=0xff, .a = 0xff };
global const Color ColorLightBlue = { .r= 0x03, .g = 0xa9, .b=0xf4, .a = 0xff };
global const Color ColorLightBlue50 = { .r= 0xe1, .g = 0xf5, .b=0xfe, .a = 0xff };
global const Color ColorLightBlue100 = { .r= 0xb3, .g = 0xe5, .b=0xfc, .a = 0xff };
global const Color ColorLightBlue200 = { .r= 0x81, .g = 0xd4, .b=0xfa, .a = 0xff };
global const Color ColorLightBlue300 = { .r= 0x4f, .g = 0xc3, .b=0xf7, .a = 0xff };
global const Color ColorLightBlue400 = { .r= 0x29, .g = 0xb6, .b=0xf6, .a = 0xff };
global const Color ColorLightBlue500 = { .r= 0x03, .g = 0xa9, .b=0xf4, .a = 0xff };
global const Color ColorLightBlue600 = { .r= 0x03, .g = 0x9b, .b=0xe5, .a = 0xff };
global const Color ColorLightBlue700 = { .r= 0x02, .g = 0x88, .b=0xd1, .a = 0xff };
global const Color ColorLightBlue800 = { .r= 0x02, .g = 0x77, .b=0xbd, .a = 0xff };
global const Color ColorLightBlue900 = { .r= 0x01, .g = 0x57, .b=0x9b, .a = 0xff };
global const Color ColorLightBlue100A = { .r= 0x80, .g = 0xd8, .b=0xff, .a = 0xff };
global const Color ColorLightBlue200A = { .r= 0x40, .g = 0xc4, .b=0xff, .a = 0xff };
global const Color ColorLightBlue400A = { .r= 0x00, .g = 0xb0, .b=0xff, .a = 0xff };
global const Color ColorLightBlue700A = { .r= 0x00, .g = 0x91, .b=0xea, .a = 0xff };
global const Color ColorCyan = { .r= 0x0b, .g = 0xcd, .b=0x4 , .a = 0xff };
global const Color ColorCyan50 = { .r= 0xe0, .g = 0xf7, .b=0xfa, .a = 0xff };
global const Color ColorCyan100 = { .r= 0xb2, .g = 0xeb, .b=0xf2, .a = 0xff };
global const Color ColorCyan200 = { .r= 0x80, .g = 0xde, .b=0xea, .a = 0xff };
global const Color ColorCyan300 = { .r= 0x4d, .g = 0xd0, .b=0xe1, .a = 0xff };
global const Color ColorCyan400 = { .r= 0x26, .g = 0xc6, .b=0xda, .a = 0xff };
global const Color ColorCyan500 = { .r= 0x00, .g = 0xbc, .b=0xd4, .a = 0xff };
global const Color ColorCyan600 = { .r= 0x00, .g = 0xac, .b=0xc1, .a = 0xff };
global const Color ColorCyan700 = { .r= 0x00, .g = 0x97, .b=0xa7, .a = 0xff };
global const Color ColorCyan800 = { .r= 0x00, .g = 0x83, .b=0x8f, .a = 0xff };
global const Color ColorCyan900 = { .r= 0x00, .g = 0x60, .b=0x64, .a = 0xff };
global const Color ColorCyan100A = { .r= 0x84, .g = 0xff, .b=0xff, .a = 0xff };
global const Color ColorCyan200A = { .r= 0x18, .g = 0xff, .b=0xff, .a = 0xff };
global const Color ColorCyan400A = { .r= 0x00, .g = 0xe5, .b=0xff, .a = 0xff };
global const Color ColorCyan700A = { .r= 0x00, .g = 0xb8, .b=0xd4, .a = 0xff };
global const Color ColorTeal = { .r= 0x09, .g = 0x68, .b=0x8 , .a = 0xff };
global const Color ColorTeal50 = { .r= 0xe0, .g = 0xf2, .b=0xf1, .a = 0xff };
global const Color ColorTeal100 = { .r= 0xb2, .g = 0xdf, .b=0xdb, .a = 0xff };
global const Color ColorTeal200 = { .r= 0x80, .g = 0xcb, .b=0xc4, .a = 0xff };
global const Color ColorTeal300 = { .r= 0x4d, .g = 0xb6, .b=0xac, .a = 0xff };
global const Color ColorTeal400 = { .r= 0x26, .g = 0xa6, .b=0x9a, .a = 0xff };
global const Color ColorTeal500 = { .r= 0x00, .g = 0x96, .b=0x88, .a = 0xff };
global const Color ColorTeal600 = { .r= 0x00, .g = 0x89, .b=0x7b, .a = 0xff };
global const Color ColorTeal700 = { .r= 0x00, .g = 0x79, .b=0x6b, .a = 0xff };
global const Color ColorTeal800 = { .r= 0x00, .g = 0x69, .b=0x5c, .a = 0xff };
global const Color ColorTeal900 = { .r= 0x00, .g = 0x4d, .b=0x40, .a = 0xff };
global const Color ColorTeal100A = { .r= 0xa7, .g = 0xff, .b=0xeb, .a = 0xff };
global const Color ColorTeal200A = { .r= 0x64, .g = 0xff, .b=0xda, .a = 0xff };
global const Color ColorTeal400A = { .r= 0x1d, .g = 0xe9, .b=0xb6, .a = 0xff };
global const Color ColorTeal700A = { .r= 0x00, .g = 0xbf, .b=0xa5, .a = 0xff };
global const Color ColorGreen = { .r= 0xCa, .g = 0xf5, .b=0x0 , .a = 0xff };
global const Color ColorGreen50 = { .r= 0xe8, .g = 0xf5, .b=0xe9, .a = 0xff };
global const Color ColorGreen100 = { .r= 0xc8, .g = 0xe6, .b=0xc9, .a = 0xff };
global const Color ColorGreen200 = { .r= 0xa5, .g = 0xd6, .b=0xa7, .a = 0xff };
global const Color ColorGreen300 = { .r= 0x81, .g = 0xc7, .b=0x84, .a = 0xff };
global const Color ColorGreen400 = { .r= 0x66, .g = 0xbb, .b=0x6a, .a = 0xff };
global const Color ColorGreen500 = { .r= 0x4c, .g = 0xaf, .b=0x50, .a = 0xff };
global const Color ColorGreen600 = { .r= 0x43, .g = 0xa0, .b=0x47, .a = 0xff };
global const Color ColorGreen700 = { .r= 0x38, .g = 0x8e, .b=0x3c, .a = 0xff };
global const Color ColorGreen800 = { .r= 0x2e, .g = 0x7d, .b=0x32, .a = 0xff };
global const Color ColorGreen900 = { .r= 0x1b, .g = 0x5e, .b=0x20, .a = 0xff };
global const Color ColorGreen100A = { .r= 0xb9, .g = 0xf6, .b=0xca, .a = 0xff };
global const Color ColorGreen200A = { .r= 0x69, .g = 0xf0, .b=0xae, .a = 0xff };
global const Color ColorGreen400A = { .r= 0x00, .g = 0xe6, .b=0x76, .a = 0xff };
global const Color ColorGreen700A = { .r= 0x00, .g = 0xc8, .b=0x53, .a = 0xff };
global const Color ColorLightGreen = { .r= 0x8b, .g = 0xc3, .b=0x4a, .a = 0xff };
global const Color ColorLightGreen50 = { .r= 0xf1, .g = 0xf8, .b=0xe9, .a = 0xff };
global const Color ColorLightGreen100 = { .r= 0xdc, .g = 0xed, .b=0xc8, .a = 0xff };
global const Color ColorLightGreen200 = { .r= 0xc5, .g = 0xe1, .b=0xa5, .a = 0xff };
global const Color ColorLightGreen300 = { .r= 0xae, .g = 0xd5, .b=0x81, .a = 0xff };
global const Color ColorLightGreen400 = { .r= 0x9c, .g = 0xcc, .b=0x65, .a = 0xff };
global const Color ColorLightGreen500 = { .r= 0x8b, .g = 0xc3, .b=0x4a, .a = 0xff };
global const Color ColorLightGreen600 = { .r= 0x7c, .g = 0xb3, .b=0x42, .a = 0xff };
global const Color ColorLightGreen700 = { .r= 0x68, .g = 0x9f, .b=0x38, .a = 0xff };
global const Color ColorLightGreen800 = { .r= 0x55, .g = 0x8b, .b=0x2f, .a = 0xff };
global const Color ColorLightGreen900 = { .r= 0x33, .g = 0x69, .b=0x1e, .a = 0xff };
global const Color ColorLightGreen100A = { .r= 0xcc, .g = 0xff, .b=0x90, .a = 0xff };
global const Color ColorLightGreen200A = { .r= 0xb2, .g = 0xff, .b=0x59, .a = 0xff };
global const Color ColorLightGreen400A = { .r= 0x76, .g = 0xff, .b=0x03, .a = 0xff };
global const Color ColorLightGreen700A = { .r= 0x64, .g = 0xdd, .b=0x17, .a = 0xff };
global const Color ColorLime = { .r= 0xDd, .g = 0xc3, .b=0x9 , .a = 0xff };
global const Color ColorLime50 = { .r= 0xf9, .g = 0xfb, .b=0xe7, .a = 0xff };
global const Color ColorLime100 = { .r= 0xf0, .g = 0xf4, .b=0xc3, .a = 0xff };
global const Color ColorLime200 = { .r= 0xe6, .g = 0xee, .b=0x9c, .a = 0xff };
global const Color ColorLime300 = { .r= 0xdc, .g = 0xe7, .b=0x75, .a = 0xff };
global const Color ColorLime400 = { .r= 0xd4, .g = 0xe1, .b=0x57, .a = 0xff };
global const Color ColorLime500 = { .r= 0xcd, .g = 0xdc, .b=0x39, .a = 0xff };
global const Color ColorLime600 = { .r= 0xc0, .g = 0xca, .b=0x33, .a = 0xff };
global const Color ColorLime700 = { .r= 0xaf, .g = 0xb4, .b=0x2b, .a = 0xff };
global const Color ColorLime800 = { .r= 0x9e, .g = 0x9d, .b=0x24, .a = 0xff };
global const Color ColorLime900 = { .r= 0x82, .g = 0x77, .b=0x17, .a = 0xff };
global const Color ColorLime100A = { .r= 0xf4, .g = 0xff, .b=0x81, .a = 0xff };
global const Color ColorLime200A = { .r= 0xee, .g = 0xff, .b=0x41, .a = 0xff };
global const Color ColorLime400A = { .r= 0xc6, .g = 0xff, .b=0x00, .a = 0xff };
global const Color ColorLime700A = { .r= 0xae, .g = 0xea, .b=0x00, .a = 0xff };
global const Color ColorYellow = { .r= 0xFe, .g = 0xb3, .b=0xb , .a = 0xff };
global const Color ColorYellow50 = { .r= 0xff, .g = 0xfd, .b=0xe7, .a = 0xff };
global const Color ColorYellow100 = { .r= 0xff, .g = 0xf9, .b=0xc4, .a = 0xff };
global const Color ColorYellow200 = { .r= 0xff, .g = 0xf5, .b=0x9d, .a = 0xff };
global const Color ColorYellow300 = { .r= 0xff, .g = 0xf1, .b=0x76, .a = 0xff };
global const Color ColorYellow400 = { .r= 0xff, .g = 0xee, .b=0x58, .a = 0xff };
global const Color ColorYellow500 = { .r= 0xff, .g = 0xeb, .b=0x3b, .a = 0xff };
global const Color ColorYellow600 = { .r= 0xfd, .g = 0xd8, .b=0x35, .a = 0xff };
global const Color ColorYellow700 = { .r= 0xfb, .g = 0xc0, .b=0x2d, .a = 0xff };
global const Color ColorYellow800 = { .r= 0xf9, .g = 0xa8, .b=0x25, .a = 0xff };
global const Color ColorYellow900 = { .r= 0xf5, .g = 0x7f, .b=0x17, .a = 0xff };
global const Color ColorYellow100A = { .r= 0xff, .g = 0xff, .b=0x8d, .a = 0xff };
global const Color ColorYellow200A = { .r= 0xff, .g = 0xff, .b=0x00, .a = 0xff };
global const Color ColorYellow400A = { .r= 0xff, .g = 0xea, .b=0x00, .a = 0xff };
global const Color ColorYellow700A = { .r= 0xff, .g = 0xd6, .b=0x00, .a = 0xff };
global const Color ColorAmber = { .r= 0xFc, .g = 0x10, .b=0x7 , .a = 0xff };
global const Color ColorAmber50 = { .r= 0xff, .g = 0xf8, .b=0xe1, .a = 0xff };
global const Color ColorAmber100 = { .r= 0xff, .g = 0xec, .b=0xb3, .a = 0xff };
global const Color ColorAmber200 = { .r= 0xff, .g = 0xe0, .b=0x82, .a = 0xff };
global const Color ColorAmber300 = { .r= 0xff, .g = 0xd5, .b=0x4f, .a = 0xff };
global const Color ColorAmber400 = { .r= 0xff, .g = 0xca, .b=0x28, .a = 0xff };
global const Color ColorAmber500 = { .r= 0xff, .g = 0xc1, .b=0x07, .a = 0xff };
global const Color ColorAmber600 = { .r= 0xff, .g = 0xb3, .b=0x00, .a = 0xff };
global const Color ColorAmber700 = { .r= 0xff, .g = 0xa0, .b=0x00, .a = 0xff };
global const Color ColorAmber800 = { .r= 0xff, .g = 0x8f, .b=0x00, .a = 0xff };
global const Color ColorAmber900 = { .r= 0xff, .g = 0x6f, .b=0x00, .a = 0xff };
global const Color ColorAmber100A = { .r= 0xff, .g = 0xe5, .b=0x7f, .a = 0xbb };
global const Color ColorAmber200A = { .r= 0xff, .g = 0xd7, .b=0x40, .a = 0x55 };
global const Color ColorAmber400A = { .r= 0xff, .g = 0xc4, .b=0x00, .a = 0xbb };
global const Color ColorAmber700A = { .r= 0xff, .g = 0xab, .b=0x00, .a = 0xbb };
global const Color ColorOrange = { .r= 0xF9, .g = 0x80, .b=0x0 , .a = 0xff };
global const Color ColorOrange50 = { .r= 0xff, .g = 0xf3, .b=0xe0, .a = 0xff };
global const Color ColorOrange100 = { .r= 0xff, .g = 0xe0, .b=0xb2, .a = 0xff };
global const Color ColorOrange200 = { .r= 0xff, .g = 0xcc, .b=0x80, .a = 0xff };
global const Color ColorOrange300 = { .r= 0xff, .g = 0xb7, .b=0x4d, .a = 0xff };
global const Color ColorOrange400 = { .r= 0xff, .g = 0xa7, .b=0x26, .a = 0xff };
global const Color ColorOrange500 = { .r= 0xff, .g = 0x98, .b=0x00, .a = 0xff };
global const Color ColorOrange600 = { .r= 0xfb, .g = 0x8c, .b=0x00, .a = 0xff };
global const Color ColorOrange700 = { .r= 0xf5, .g = 0x7c, .b=0x00, .a = 0xff };
global const Color ColorOrange800 = { .r= 0xef, .g = 0x6c, .b=0x00, .a = 0xff };
global const Color ColorOrange900 = { .r= 0xe6, .g = 0x51, .b=0x00, .a = 0xff };
global const Color ColorOrange100A = { .r= 0xff, .g = 0xd1, .b=0x80, .a = 0xff };
global const Color ColorOrange200A = { .r= 0xff, .g = 0xab, .b=0x40, .a = 0xff };
global const Color ColorOrange400A = { .r= 0xff, .g = 0x91, .b=0x00, .a = 0xff };
global const Color ColorOrange700A = { .r= 0xff, .g = 0x6d, .b=0x00, .a = 0xff };
global const Color ColorDeepOrange = { .r= 0xff, .g = 0x57, .b=0x22, .a = 0xff };
global const Color ColorDeepOrange50 = { .r= 0xfb, .g = 0xe9, .b=0xe7, .a = 0xff };
global const Color ColorDeepOrange100 = { .r= 0xff, .g = 0xcc, .b=0xbc, .a = 0xff };
global const Color ColorDeepOrange200 = { .r= 0xff, .g = 0xab, .b=0x91, .a = 0xff };
global const Color ColorDeepOrange300 = { .r= 0xff, .g = 0x8a, .b=0x65, .a = 0xff };
global const Color ColorDeepOrange400 = { .r= 0xff, .g = 0x70, .b=0x43, .a = 0xff };
global const Color ColorDeepOrange500 = { .r= 0xff, .g = 0x57, .b=0x22, .a = 0xff };
global const Color ColorDeepOrange600 = { .r= 0xf4, .g = 0x51, .b=0x1e, .a = 0xff };
global const Color ColorDeepOrange700 = { .r= 0xe6, .g = 0x4a, .b=0x19, .a = 0xff };
global const Color ColorDeepOrange800 = { .r= 0xd8, .g = 0x43, .b=0x15, .a = 0xff };
global const Color ColorDeepOrange900 = { .r= 0xbf, .g = 0x36, .b=0x0c, .a = 0xff };
global const Color ColorDeepOrange100A = { .r= 0xff, .g = 0x9e, .b=0x80, .a = 0xff };
global const Color ColorDeepOrange200A = { .r= 0xff, .g = 0x6e, .b=0x40, .a = 0xff };
global const Color ColorDeepOrange400A = { .r= 0xff, .g = 0x3d, .b=0x00, .a = 0xff };
global const Color ColorDeepOrange700A = { .r= 0xdd, .g = 0x2c, .b=0x00, .a = 0xff };
global const Color ColorBrown = { .r= 0x95, .g = 0x54, .b=0x8 , .a = 0xff };
global const Color ColorBrown50 = { .r= 0xef, .g = 0xeb, .b=0xe9, .a = 0xff };
global const Color ColorBrown100 = { .r= 0xd7, .g = 0xcc, .b=0xc8, .a = 0xff };
global const Color ColorBrown200 = { .r= 0xbc, .g = 0xaa, .b=0xa4, .a = 0xff };
global const Color ColorBrown300 = { .r= 0xa1, .g = 0x88, .b=0x7f, .a = 0xff };
global const Color ColorBrown400 = { .r= 0x8d, .g = 0x6e, .b=0x63, .a = 0xff };
global const Color ColorBrown500 = { .r= 0x79, .g = 0x55, .b=0x48, .a = 0xff };
global const Color ColorBrown600 = { .r= 0x6d, .g = 0x4c, .b=0x41, .a = 0xff };
global const Color ColorBrown700 = { .r= 0x5d, .g = 0x40, .b=0x37, .a = 0xff };
global const Color ColorBrown800 = { .r= 0x4e, .g = 0x34, .b=0x2e, .a = 0xff };
global const Color ColorBrown900 = { .r= 0x3e, .g = 0x27, .b=0x23, .a = 0xff };
global const Color ColorGrey = { .r= 0xE9, .g = 0xe9, .b=0xe , .a = 0xff };
global const Color ColorGrey50 = { .r= 0xfa, .g = 0xfa, .b=0xfa, .a = 0xff };
global const Color ColorGrey100 = { .r= 0xf5, .g = 0xf5, .b=0xf5, .a = 0xff };
global const Color ColorGrey200 = { .r= 0xee, .g = 0xee, .b=0xee, .a = 0xff };
global const Color ColorGrey300 = { .r= 0xe0, .g = 0xe0, .b=0xe0, .a = 0xff };
global const Color ColorGrey400 = { .r= 0xbd, .g = 0xbd, .b=0xbd, .a = 0xff };
global const Color ColorGrey500 = { .r= 0x9e, .g = 0x9e, .b=0x9e, .a = 0xff };
global const Color ColorGrey600 = { .r= 0x75, .g = 0x75, .b=0x75, .a = 0xff };
global const Color ColorGrey700 = { .r= 0x61, .g = 0x61, .b=0x61, .a = 0xff };
global const Color ColorGrey800 = { .r= 0x42, .g = 0x42, .b=0x42, .a = 0xff };
global const Color ColorGrey900 = { .r= 0x21, .g = 0x21, .b=0x21, .a = 0xff };
global const Color ColorBlueGrey = { .r= 0x60, .g = 0x7d, .b=0x8b, .a = 0xff };
global const Color ColorBlueGrey50 = { .r= 0xec, .g = 0xef, .b=0xf1, .a = 0xff };
global const Color ColorBlueGrey100 = { .r= 0xcf, .g = 0xd8, .b=0xdc, .a = 0xff };
global const Color ColorBlueGrey200 = { .r= 0xb0, .g = 0xbe, .b=0xc5, .a = 0xff };
global const Color ColorBlueGrey300 = { .r= 0x90, .g = 0xa4, .b=0xae, .a = 0xff };
global const Color ColorBlueGrey400 = { .r= 0x78, .g = 0x90, .b=0x9c, .a = 0xff };
global const Color ColorBlueGrey500 = { .r= 0x60, .g = 0x7d, .b=0x8b, .a = 0xff };
global const Color ColorBlueGrey600 = { .r= 0x54, .g = 0x6e, .b=0x7a, .a = 0xff };
global const Color ColorBlueGrey700 = { .r= 0x45, .g = 0x5a, .b=0x64, .a = 0xff };
global const Color ColorBlueGrey800 = { .r= 0x37, .g = 0x47, .b=0x4f, .a = 0xff };
global const Color ColorBlueGrey900 = { .r= 0x26, .g = 0x32, .b=0x38, .a = 0xff };
global const Color ColorSlate300Fade = { .r = 0xcb, .g = 0xd5, .b = 0xe1, .a = 0xaa };

global const Color ColorBlueShadow100 = { .r = 0x0f, .g = 0x0f, .b = 0x1f, .a = 0xcf };