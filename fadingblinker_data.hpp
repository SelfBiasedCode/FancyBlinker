/* PWM values and constants for FadingBlinker */

#ifndef FADINGBLINKER_DATA_H
#define FADINGBLINKER_DATA_H

static const uint16_t PROGMEM fadingblinker_data[] =
{
//256 timer values
	0,		0,		0,		1,		3,		5,		8,		12,		16,		20,	
	26,		32,		39,		46,		55,		64,		74,		84,		96,		108,
	121,	134,	149,	164,	180,	197,	215,	234,	253,	274,
	295,	317,	340,	364,	389,	414,	441,	468,	497,	526,
	556,	587,	619,	652,	686,	721,	757,	793,	831,	869,
	909,	949,	991,	1033,	1077,	1121,	1167,	1213,	1260,	1308,
	1358,	1408,	1459,	1512,	1565,	1619,	1675,	1731,	1788,	1847,
	1906,	1967,	2028,	2091,	2154,	2219,	2284,	2351,	2419,	2487,
	2557,	2628,	2700,	2773,	2847,	2922,	2998,	3076,	3154,	3233,
	3314,	3395,	3478,	3562,	3647,	3732,	3819,	3908,	3997,	4087,
	4178,	4271,	4364,	4459,	4555,	4652,	4750,	4849,	4949,	5051,
	5153,	5257,	5362,	5468,	5575,	5683,	5792,	5902,	6014,	6127,
	6241,	6356,	6472,	6589,	6707,	6827,	6948,	7070,	7193,	7317,
	7442,	7569,	7697,	7825,	7955,	8087,	8219,	8353,	8487,	8623,
	8760,	8899,	9038,	9179,	9320,	9463,	9608,	9753,	9900,	10047,
	10196,	10346,	10498,	10650,	10804,	10959,	11115,	11273,	11431,	11591,
	11752,	11914,	12077,	12242,	12408,	12575,	12743,	12913,	13084,	13255,
	13429,	13603,	13779,	13956,	14134,	14313,	14494,	14675,	14858,	15043,
	15228,	15415,	15603,	15792,	15983,	16174,	16367,	16562,	16757,	16954,
	17152,	17351,	17551,	17753,	17956,	18160,	18366,	18573,	18781,	18990,
	19201,	19413,	19626,	19840,	20056,	20273,	20491,	20710,	20931,	21153,
	21376,	21601,	21827,	22054,	22282,	22512,	22743,	22975,	23209,	23444,
	23680,	23917,	24156,	24396,	24638,	24880,	25124,	25369,	25616,	25864,
	26113,	26363,	26615,	26868,	27122,	27378,	27635,	27893,	28153,	28414,
	28676,	28940,	29204,	29471,	29738,	30007,	30277,	30548,	30821,	31095,
	31371,	31647,	31925,	32205,	32485,
// Timer TOP value
	32768,
// Off cycles 
	20,
// On cycles 
	20
};

#endif
