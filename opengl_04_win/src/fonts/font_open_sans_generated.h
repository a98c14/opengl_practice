#pragma once

#include "opus_base.h"
#include "opus_math.h"
#include "opus_text.h"

global const GlyphAtlasInfo FONT_OPEN_SANS_ATLAS_INFO =
{
	.width = 520,
	.height = 520,
	.size = 64,
	.distance_range = 16,
	.line_height = 1.361816,
	.ascender = 1.068848,
	.underline_y = -0.061035,
	.underline_thickness = 0.024414
};

global const Glyph FONT_OPEN_SANS_GLYPHS[95] =
{
	{ .unicode = 32, .advance = 0.259766, .plane_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 33, .advance = 0.264160, .plane_bounds = { .left = -0.056152, .bottom = -0.142090, .right = 0.318848, .top = 0.842285}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 34, .advance = 0.398438, .plane_bounds = { .left = -0.066406, .bottom = 0.327148, .right = 0.464844, .top = 0.842773}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 35, .advance = 0.645996, .plane_bounds = { .left = -0.106934, .bottom = -0.127441, .right = 0.752441, .top = 0.841309}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 36, .advance = 0.571777, .plane_bounds = { .left = -0.064941, .bottom = -0.188721, .right = 0.638184, .top = 0.889404}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 37, .advance = 0.826660, .plane_bounds = { .left = -0.078857, .bottom = -0.135010, .right = 0.905518, .top = 0.849365}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 38, .advance = 0.728516, .plane_bounds = { .left = -0.072266, .bottom = -0.142334, .right = 0.849609, .top = 0.857666}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 39, .advance = 0.219238, .plane_bounds = { .left = -0.062012, .bottom = 0.327148, .right = 0.281738, .top = 0.842773}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 40, .advance = 0.294922, .plane_bounds = { .left = -0.089844, .bottom = -0.284668, .right = 0.394531, .top = 0.840332}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 41, .advance = 0.294922, .plane_bounds = { .left = -0.099609, .bottom = -0.284668, .right = 0.384766, .top = 0.840332}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 42, .advance = 0.550781, .plane_bounds = { .left = -0.085205, .bottom = 0.186035, .right = 0.633545, .top = 0.889160}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 43, .advance = 0.571777, .plane_bounds = { .left = -0.082275, .bottom = -0.014160, .right = 0.652100, .top = 0.720215}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 44, .advance = 0.258789, .plane_bounds = { .left = -0.090576, .bottom = -0.256348, .right = 0.315674, .top = 0.243652}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 45, .advance = 0.321777, .plane_bounds = { .left = -0.088867, .bottom = 0.104492, .right = 0.411133, .top = 0.432617}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 46, .advance = 0.262695, .plane_bounds = { .left = -0.056152, .bottom = -0.145264, .right = 0.318848, .top = 0.245361}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 47, .advance = 0.366699, .plane_bounds = { .left = -0.121582, .bottom = -0.127441, .right = 0.487793, .top = 0.841309}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 48, .advance = 0.571777, .plane_bounds = { .left = -0.081543, .bottom = -0.142334, .right = 0.652832, .top = 0.857666}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 49, .advance = 0.571777, .plane_bounds = { .left = -0.037109, .bottom = -0.127441, .right = 0.478516, .top = 0.841309}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 50, .advance = 0.571777, .plane_bounds = { .left = -0.076172, .bottom = -0.130127, .right = 0.642578, .top = 0.854248}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 51, .advance = 0.571777, .plane_bounds = { .left = -0.080566, .bottom = -0.135010, .right = 0.638184, .top = 0.849365}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 52, .advance = 0.571777, .plane_bounds = { .left = -0.103516, .bottom = -0.125488, .right = 0.677734, .top = 0.843262}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 53, .advance = 0.571777, .plane_bounds = { .left = -0.062256, .bottom = -0.140137, .right = 0.640869, .top = 0.844238}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 54, .advance = 0.571777, .plane_bounds = { .left = -0.069336, .bottom = -0.135010, .right = 0.649414, .top = 0.849365}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 55, .advance = 0.571777, .plane_bounds = { .left = -0.083740, .bottom = -0.127441, .right = 0.650635, .top = 0.841309}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 56, .advance = 0.571777, .plane_bounds = { .left = -0.082031, .bottom = -0.135010, .right = 0.652344, .top = 0.849365}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 57, .advance = 0.571777, .plane_bounds = { .left = -0.075195, .bottom = -0.135254, .right = 0.643555, .top = 0.849121}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 58, .advance = 0.262695, .plane_bounds = { .left = -0.056152, .bottom = -0.139160, .right = 0.318848, .top = 0.673340}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 59, .advance = 0.262695, .plane_bounds = { .left = -0.093506, .bottom = -0.259277, .right = 0.312744, .top = 0.678223}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 60, .advance = 0.571777, .plane_bounds = { .left = -0.082031, .bottom = -0.012939, .right = 0.652344, .top = 0.737061}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 61, .advance = 0.571777, .plane_bounds = { .left = -0.074219, .bottom = 0.094238, .right = 0.644531, .top = 0.609863}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 62, .advance = 0.571777, .plane_bounds = { .left = -0.082031, .bottom = -0.012939, .right = 0.652344, .top = 0.737061}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 63, .advance = 0.431641, .plane_bounds = { .left = -0.110596, .bottom = -0.144775, .right = 0.530029, .top = 0.855225}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 64, .advance = 0.896484, .plane_bounds = { .left = -0.074951, .bottom = -0.219971, .right = 0.971924, .top = 0.842529}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 65, .advance = 0.632324, .plane_bounds = { .left = -0.129639, .bottom = -0.125977, .right = 0.760986, .top = 0.842773}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 66, .advance = 0.645996, .plane_bounds = { .left = -0.030273, .bottom = -0.127441, .right = 0.719727, .top = 0.841309}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 67, .advance = 0.629883, .plane_bounds = { .left = -0.068359, .bottom = -0.135010, .right = 0.728516, .top = 0.849365}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 68, .advance = 0.725586, .plane_bounds = { .left = -0.032959, .bottom = -0.127441, .right = 0.795166, .top = 0.841309}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 69, .advance = 0.555664, .plane_bounds = { .left = -0.031738, .bottom = -0.127441, .right = 0.624512, .top = 0.841309}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 70, .advance = 0.516113, .plane_bounds = { .left = -0.031738, .bottom = -0.127441, .right = 0.624512, .top = 0.841309}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 71, .advance = 0.727051, .plane_bounds = { .left = -0.065186, .bottom = -0.135010, .right = 0.778564, .top = 0.849365}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 72, .advance = 0.737305, .plane_bounds = { .left = -0.030273, .bottom = -0.127441, .right = 0.766602, .top = 0.841309}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 73, .advance = 0.279297, .plane_bounds = { .left = -0.032715, .bottom = -0.127441, .right = 0.311035, .top = 0.841309}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 74, .advance = 0.268555, .plane_bounds = { .left = -0.209473, .bottom = -0.315186, .right = 0.306152, .top = 0.841064}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 75, .advance = 0.612305, .plane_bounds = { .left = -0.027832, .bottom = -0.127441, .right = 0.737793, .top = 0.841309}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 76, .advance = 0.521973, .plane_bounds = { .left = -0.030518, .bottom = -0.127441, .right = 0.625732, .top = 0.841309}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 77, .advance = 0.899414, .plane_bounds = { .left = -0.034668, .bottom = -0.127441, .right = 0.934082, .top = 0.841309}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 78, .advance = 0.752930, .plane_bounds = { .left = -0.029541, .bottom = -0.127441, .right = 0.782959, .top = 0.841309}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 79, .advance = 0.777832, .plane_bounds = { .left = -0.064209, .bottom = -0.142334, .right = 0.842041, .top = 0.857666}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 80, .advance = 0.601562, .plane_bounds = { .left = -0.027832, .bottom = -0.127441, .right = 0.675293, .top = 0.841309}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 81, .advance = 0.777832, .plane_bounds = { .left = -0.064209, .bottom = -0.300537, .right = 0.842041, .top = 0.855713}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 82, .advance = 0.617188, .plane_bounds = { .left = -0.033691, .bottom = -0.127441, .right = 0.731934, .top = 0.841309}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 83, .advance = 0.548340, .plane_bounds = { .left = -0.075684, .bottom = -0.135010, .right = 0.627441, .top = 0.849365}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 84, .advance = 0.550781, .plane_bounds = { .left = -0.123779, .bottom = -0.127441, .right = 0.673096, .top = 0.841309}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 85, .advance = 0.729004, .plane_bounds = { .left = -0.042236, .bottom = -0.140137, .right = 0.770264, .top = 0.844238}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 86, .advance = 0.596191, .plane_bounds = { .left = -0.131592, .bottom = -0.127441, .right = 0.727783, .top = 0.841309}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 87, .advance = 0.923340, .plane_bounds = { .left = -0.116455, .bottom = -0.127441, .right = 1.039795, .top = 0.841309}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 88, .advance = 0.577637, .plane_bounds = { .left = -0.125488, .bottom = -0.127441, .right = 0.702637, .top = 0.841309}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 89, .advance = 0.559082, .plane_bounds = { .left = -0.126709, .bottom = -0.127441, .right = 0.685791, .top = 0.841309}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 90, .advance = 0.572266, .plane_bounds = { .left = -0.089111, .bottom = -0.127441, .right = 0.660889, .top = 0.841309}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 91, .advance = 0.327148, .plane_bounds = { .left = -0.050537, .bottom = -0.284668, .right = 0.433838, .top = 0.840332}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 92, .advance = 0.366699, .plane_bounds = { .left = -0.121094, .bottom = -0.127441, .right = 0.488281, .top = 0.841309}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 93, .advance = 0.327148, .plane_bounds = { .left = -0.106445, .bottom = -0.284668, .right = 0.377930, .top = 0.840332}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 94, .advance = 0.571777, .plane_bounds = { .left = -0.088867, .bottom = 0.142090, .right = 0.661133, .top = 0.845215}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 95, .advance = 0.437988, .plane_bounds = { .left = -0.132568, .bottom = -0.276123, .right = 0.570557, .top = 0.036377}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 96, .advance = 0.277344, .plane_bounds = { .left = -0.087646, .bottom = 0.475098, .right = 0.365479, .top = 0.896973}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 97, .advance = 0.555664, .plane_bounds = { .left = -0.083740, .bottom = -0.139160, .right = 0.603760, .top = 0.673340}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 98, .advance = 0.611816, .plane_bounds = { .left = -0.046387, .bottom = -0.140625, .right = 0.687988, .top = 0.890625}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 99, .advance = 0.479004, .plane_bounds = { .left = -0.069336, .bottom = -0.138672, .right = 0.571289, .top = 0.673828}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 100, .advance = 0.611816, .plane_bounds = { .left = -0.076416, .bottom = -0.140625, .right = 0.657959, .top = 0.890625}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 101, .advance = 0.561523, .plane_bounds = { .left = -0.076904, .bottom = -0.138672, .right = 0.641846, .top = 0.673828}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 102, .advance = 0.336426, .plane_bounds = { .left = -0.114258, .bottom = -0.125244, .right = 0.510742, .top = 0.890381}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 103, .advance = 0.542969, .plane_bounds = { .left = -0.113770, .bottom = -0.370605, .right = 0.651855, .top = 0.676270}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 104, .advance = 0.613281, .plane_bounds = { .left = -0.042969, .bottom = -0.127930, .right = 0.660156, .top = 0.887695}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 105, .advance = 0.252441, .plane_bounds = { .left = -0.052734, .bottom = -0.132324, .right = 0.306641, .top = 0.867676}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 106, .advance = 0.252441, .plane_bounds = { .left = -0.181641, .bottom = -0.369629, .right = 0.302734, .top = 0.864746}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 107, .advance = 0.525391, .plane_bounds = { .left = -0.042236, .bottom = -0.127930, .right = 0.645264, .top = 0.887695}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 108, .advance = 0.252441, .plane_bounds = { .left = -0.045654, .bottom = -0.127930, .right = 0.298096, .top = 0.887695}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 109, .advance = 0.925781, .plane_bounds = { .left = -0.042725, .bottom = -0.125977, .right = 0.972900, .top = 0.670898}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 110, .advance = 0.613281, .plane_bounds = { .left = -0.042969, .bottom = -0.125977, .right = 0.660156, .top = 0.670898}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 111, .advance = 0.601562, .plane_bounds = { .left = -0.073730, .bottom = -0.138672, .right = 0.676270, .top = 0.673828}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 112, .advance = 0.611816, .plane_bounds = { .left = -0.046387, .bottom = -0.370605, .right = 0.687988, .top = 0.676270}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 113, .advance = 0.611816, .plane_bounds = { .left = -0.076904, .bottom = -0.370605, .right = 0.657471, .top = 0.676270}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 114, .advance = 0.408691, .plane_bounds = { .left = -0.041748, .bottom = -0.125977, .right = 0.520752, .top = 0.670898}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 115, .advance = 0.476562, .plane_bounds = { .left = -0.079346, .bottom = -0.138672, .right = 0.561279, .top = 0.673828}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 116, .advance = 0.356445, .plane_bounds = { .left = -0.114502, .bottom = -0.136230, .right = 0.463623, .top = 0.785645}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 117, .advance = 0.613281, .plane_bounds = { .left = -0.048096, .bottom = -0.135742, .right = 0.655029, .top = 0.661133}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 118, .advance = 0.499512, .plane_bounds = { .left = -0.125244, .bottom = -0.130859, .right = 0.624756, .top = 0.666016}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 119, .advance = 0.774902, .plane_bounds = { .left = -0.120361, .bottom = -0.129883, .right = 0.895264, .top = 0.666992}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 120, .advance = 0.523438, .plane_bounds = { .left = -0.113281, .bottom = -0.130859, .right = 0.636719, .top = 0.666016}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 121, .advance = 0.500977, .plane_bounds = { .left = -0.124023, .bottom = -0.368408, .right = 0.625977, .top = 0.662842}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 122, .advance = 0.468750, .plane_bounds = { .left = -0.086182, .bottom = -0.130859, .right = 0.554443, .top = 0.666016}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 123, .advance = 0.375000, .plane_bounds = { .left = -0.103760, .bottom = -0.284668, .right = 0.474365, .top = 0.840332}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 124, .advance = 0.549316, .plane_bounds = { .left = 0.110107, .bottom = -0.373779, .right = 0.438232, .top = 0.891846}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 125, .advance = 0.375000, .plane_bounds = { .left = -0.098877, .bottom = -0.284668, .right = 0.479248, .top = 0.840332}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}},
	{ .unicode = 126, .advance = 0.571777, .plane_bounds = { .left = -0.082031, .bottom = 0.157227, .right = 0.652344, .top = 0.547852}, .atlas_bounds = { .left = 0.000000, .bottom = 0.000000, .right = 0.000000, .top = 0.000000}}
};

