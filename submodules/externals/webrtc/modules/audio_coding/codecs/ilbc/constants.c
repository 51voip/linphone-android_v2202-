/*
 *  Copyright (c) 2011 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

/******************************************************************

 iLBC Speech Coder ANSI-C Source Code

 constants.c

******************************************************************/

#include "defines.h"
#include "constants.h"

/* HP Filters {b[0] b[1] b[2] -a[1] -a[2]} */

const WebRtc_Word16 WebRtcIlbcfix_kHpInCoefs[5] = {3798, -7596, 3798, 7807, -3733};
const WebRtc_Word16 WebRtcIlbcfix_kHpOutCoefs[5] = {3849, -7699, 3849, 7918, -3833};

/* Window in Q11 to window the energies of the 5 choises (3 for 20ms) in the choise for
   the 80 sample start state
*/
const WebRtc_Word16 WebRtcIlbcfix_kStartSequenceEnrgWin[NSUB_MAX-1]= {
  1638, 1843, 2048, 1843, 1638
};

/* LP Filter coeffs used for downsampling */
const WebRtc_Word16 WebRtcIlbcfix_kLpFiltCoefs[FILTERORDER_DS_PLUS1]= {
  -273, 512, 1297, 1696, 1297, 512, -273
};

/* Constants used in the LPC calculations */

/* Hanning LPC window (in Q15) */
const WebRtc_Word16 WebRtcIlbcfix_kLpcWin[BLOCKL_MAX] = {
  6, 22, 50, 89, 139, 200, 272, 355, 449, 554, 669, 795,
  932, 1079, 1237, 1405, 1583, 1771, 1969, 2177, 2395, 2622, 2858, 3104,
  3359, 3622, 3894, 4175, 4464, 4761, 5066, 5379, 5699, 6026, 6361, 6702,
  7050, 7404, 7764, 8130, 8502, 8879, 9262, 9649, 10040, 10436, 10836, 11240,
  11647, 12058, 12471, 12887, 13306, 13726, 14148, 14572, 14997, 15423, 15850, 16277,
  16704, 17131, 17558, 17983, 18408, 18831, 19252, 19672, 20089, 20504, 20916, 21325,
  21730, 22132, 22530, 22924, 23314, 23698, 24078, 24452, 24821, 25185, 25542, 25893,
  26238, 26575, 26906, 27230, 27547, 27855, 28156, 28450, 28734, 29011, 29279, 29538,
  29788, 30029, 30261, 30483, 30696, 30899, 31092, 31275, 31448, 31611, 31764, 31906,
  32037, 32158, 32268, 32367, 32456, 32533, 32600, 32655, 32700, 32733, 32755, 32767,
  32767, 32755, 32733, 32700, 32655, 32600, 32533, 32456, 32367, 32268, 32158, 32037,
  31906, 31764, 31611, 31448, 31275, 31092, 30899, 30696, 30483, 30261, 30029, 29788,
  29538, 29279, 29011, 28734, 28450, 28156, 27855, 27547, 27230, 26906, 26575, 26238,
  25893, 25542, 25185, 24821, 24452, 24078, 23698, 23314, 22924, 22530, 22132, 21730,
  21325, 20916, 20504, 20089, 19672, 19252, 18831, 18408, 17983, 17558, 17131, 16704,
  16277, 15850, 15423, 14997, 14572, 14148, 13726, 13306, 12887, 12471, 12058, 11647,
  11240, 10836, 10436, 10040, 9649, 9262, 8879, 8502, 8130, 7764, 7404, 7050,
  6702, 6361, 6026, 5699, 5379, 5066, 4761, 4464, 4175, 3894, 3622, 3359,
  3104, 2858, 2622, 2395, 2177, 1969, 1771, 1583, 1405, 1237, 1079, 932,
  795, 669, 554, 449, 355, 272, 200, 139, 89, 50, 22, 6
};

/* Asymmetric LPC window (in Q15)*/
const WebRtc_Word16 WebRtcIlbcfix_kLpcAsymWin[BLOCKL_MAX] = {
  2, 7, 15, 27, 42, 60, 81, 106, 135, 166, 201, 239,
  280, 325, 373, 424, 478, 536, 597, 661, 728, 798, 872, 949,
  1028, 1111, 1197, 1287, 1379, 1474, 1572, 1674, 1778, 1885, 1995, 2108,
  2224, 2343, 2465, 2589, 2717, 2847, 2980, 3115, 3254, 3395, 3538, 3684,
  3833, 3984, 4138, 4295, 4453, 4615, 4778, 4944, 5112, 5283, 5456, 5631,
  5808, 5987, 6169, 6352, 6538, 6725, 6915, 7106, 7300, 7495, 7692, 7891,
  8091, 8293, 8497, 8702, 8909, 9118, 9328, 9539, 9752, 9966, 10182, 10398,
  10616, 10835, 11055, 11277, 11499, 11722, 11947, 12172, 12398, 12625, 12852, 13080,
  13309, 13539, 13769, 14000, 14231, 14463, 14695, 14927, 15160, 15393, 15626, 15859,
  16092, 16326, 16559, 16792, 17026, 17259, 17492, 17725, 17957, 18189, 18421, 18653,
  18884, 19114, 19344, 19573, 19802, 20030, 20257, 20483, 20709, 20934, 21157, 21380,
  21602, 21823, 22042, 22261, 22478, 22694, 22909, 23123, 23335, 23545, 23755, 23962,
  24168, 24373, 24576, 24777, 24977, 25175, 25371, 25565, 25758, 25948, 26137, 26323,
  26508, 26690, 26871, 27049, 27225, 27399, 27571, 27740, 27907, 28072, 28234, 28394,
  28552, 28707, 28860, 29010, 29157, 29302, 29444, 29584, 29721, 29855, 29987, 30115,
  30241, 30364, 30485, 30602, 30717, 30828, 30937, 31043, 31145, 31245, 31342, 31436,
  31526, 31614, 31699, 31780, 31858, 31933, 32005, 32074, 32140, 32202, 32261, 32317,
  32370, 32420, 32466, 32509, 32549, 32585, 32618, 32648, 32675, 32698, 32718, 32734,
  32748, 32758, 32764, 32767, 32767, 32667, 32365, 31863, 31164, 30274, 29197, 27939,
  26510, 24917, 23170, 21281, 19261, 17121, 14876, 12540, 10126, 7650, 5126, 2571
};

/* Lag window for LPC (Q31) */
const WebRtc_Word32 WebRtcIlbcfix_kLpcLagWin[LPC_FILTERORDER + 1]={
  2147483647,   2144885453,   2137754373,   2125918626,   2109459810,
  2088483140,   2063130336,   2033564590,   1999977009,   1962580174,
  1921610283};

/* WebRtcIlbcfix_kLpcChirpSyntDenum vector in Q15 corresponding
 * floating point vector {1 0.9025 0.9025^2 0.9025^3 ...}
 */
const WebRtc_Word16 WebRtcIlbcfix_kLpcChirpSyntDenum[LPC_FILTERORDER + 1] = {
  32767, 29573, 26690, 24087,
  21739, 19619, 17707, 15980,
  14422, 13016, 11747};

/* WebRtcIlbcfix_kLpcChirpWeightDenum in Q15 corresponding to
 * floating point vector {1 0.4222 0.4222^2... }
 */
const WebRtc_Word16 WebRtcIlbcfix_kLpcChirpWeightDenum[LPC_FILTERORDER + 1] = {
  32767, 13835, 5841, 2466, 1041, 440,
  186, 78,  33,  14,  6};

/* LSF quantization Q13 domain */
const WebRtc_Word16 WebRtcIlbcfix_kLsfCb[64 * 3 + 128 * 3 + 128 * 4] = {
  1273,       2238,       3696,
  3199,       5309,       8209,
  3606,       5671,       7829,
  2815,       5262,       8778,
  2608,       4027,       5493,
  1582,       3076,       5945,
  2983,       4181,       5396,
  2437,       4322,       6902,
  1861,       2998,       4613,
  2007,       3250,       5214,
  1388,       2459,       4262,
  2563,       3805,       5269,
  2036,       3522,       5129,
  1935,       4025,       6694,
  2744,       5121,       7338,
  2810,       4248,       5723,
  3054,       5405,       7745,
  1449,       2593,       4763,
  3411,       5128,       6596,
  2484,       4659,       7496,
  1668,       2879,       4818,
  1812,       3072,       5036,
  1638,       2649,       3900,
  2464,       3550,       4644,
  1853,       2900,       4158,
  2458,       4163,       5830,
  2556,       4036,       6254,
  2703,       4432,       6519,
  3062,       4953,       7609,
  1725,       3703,       6187,
  2221,       3877,       5427,
  2339,       3579,       5197,
  2021,       4633,       7037,
  2216,       3328,       4535,
  2961,       4739,       6667,
  2807,       3955,       5099,
  2788,       4501,       6088,
  1642,       2755,       4431,
  3341,       5282,       7333,
  2414,       3726,       5727,
  1582,       2822,       5269,
  2259,       3447,       4905,
  3117,       4986,       7054,
  1825,       3491,       5542,
  3338,       5736,       8627,
  1789,       3090,       5488,
  2566,       3720,       4923,
  2846,       4682,       7161,
  1950,       3321,       5976,
  1834,       3383,       6734,
  3238,       4769,       6094,
  2031,       3978,       5903,
  1877,       4068,       7436,
  2131,       4644,       8296,
  2764,       5010,       8013,
  2194,       3667,       6302,
  2053,       3127,       4342,
  3523,       6595,      10010,
  3134,       4457,       5748,
  3142,       5819,       9414,
  2223,       4334,       6353,
  2022,       3224,       4822,
  2186,       3458,       5544,
  2552,       4757,       6870,
  10905,      12917,      14578,
  9503,      11485,      14485,
  9518,      12494,      14052,
  6222,       7487,       9174,
  7759,       9186,      10506,
  8315,      12755,      14786,
  9609,      11486,      13866,
  8909,      12077,      13643,
  7369,       9054,      11520,
  9408,      12163,      14715,
  6436,       9911,      12843,
  7109,       9556,      11884,
  7557,      10075,      11640,
  6482,       9202,      11547,
  6463,       7914,      10980,
  8611,      10427,      12752,
  7101,       9676,      12606,
  7428,      11252,      13172,
  10197,      12955,      15842,
  7487,      10955,      12613,
  5575,       7858,      13621,
  7268,      11719,      14752,
  7476,      11744,      13795,
  7049,       8686,      11922,
  8234,      11314,      13983,
  6560,      11173,      14984,
  6405,       9211,      12337,
  8222,      12054,      13801,
  8039,      10728,      13255,
  10066,      12733,      14389,
  6016,       7338,      10040,
  6896,       8648,      10234,
  7538,       9170,      12175,
  7327,      12608,      14983,
  10516,      12643,      15223,
  5538,       7644,      12213,
  6728,      12221,      14253,
  7563,       9377,      12948,
  8661,      11023,      13401,
  7280,       8806,      11085,
  7723,       9793,      12333,
  12225,      14648,      16709,
  8768,      13389,      15245,
  10267,      12197,      13812,
  5301,       7078,      11484,
  7100,      10280,      11906,
  8716,      12555,      14183,
  9567,      12464,      15434,
  7832,      12305,      14300,
  7608,      10556,      12121,
  8913,      11311,      12868,
  7414,       9722,      11239,
  8666,      11641,      13250,
  9079,      10752,      12300,
  8024,      11608,      13306,
  10453,      13607,      16449,
  8135,       9573,      10909,
  6375,       7741,      10125,
  10025,      12217,      14874,
  6985,      11063,      14109,
  9296,      13051,      14642,
  8613,      10975,      12542,
  6583,      10414,      13534,
  6191,       9368,      13430,
  5742,       6859,       9260,
  7723,       9813,      13679,
  8137,      11291,      12833,
  6562,       8973,      10641,
  6062,       8462,      11335,
  6928,       8784,      12647,
  7501,       8784,      10031,
  8372,      10045,      12135,
  8191,       9864,      12746,
  5917,       7487,      10979,
  5516,       6848,      10318,
  6819,       9899,      11421,
  7882,      12912,      15670,
  9558,      11230,      12753,
  7752,       9327,      11472,
  8479,       9980,      11358,
  11418,      14072,      16386,
  7968,      10330,      14423,
  8423,      10555,      12162,
  6337,      10306,      14391,
  8850,      10879,      14276,
  6750,      11885,      15710,
  7037,       8328,       9764,
  6914,       9266,      13476,
  9746,      13949,      15519,
  11032,      14444,      16925,
  8032,      10271,      11810,
  10962,      13451,      15833,
  10021,      11667,      13324,
  6273,       8226,      12936,
  8543,      10397,      13496,
  7936,      10302,      12745,
  6769,       8138,      10446,
  6081,       7786,      11719,
  8637,      11795,      14975,
  8790,      10336,      11812,
  7040,       8490,      10771,
  7338,      10381,      13153,
  6598,       7888,       9358,
  6518,       8237,      12030,
  9055,      10763,      12983,
  6490,      10009,      12007,
  9589,      12023,      13632,
  6867,       9447,      10995,
  7930,       9816,      11397,
  10241,      13300,      14939,
  5830,       8670,      12387,
  9870,      11915,      14247,
  9318,      11647,      13272,
  6721,      10836,      12929,
  6543,       8233,       9944,
  8034,      10854,      12394,
  9112,      11787,      14218,
  9302,      11114,      13400,
  9022,      11366,      13816,
  6962,      10461,      12480,
  11288,      13333,      15222,
  7249,       8974,      10547,
  10566,      12336,      14390,
  6697,      11339,      13521,
  11851,      13944,      15826,
  6847,       8381,      11349,
  7509,       9331,      10939,
  8029,       9618,      11909,
  13973,      17644,      19647,      22474,
  14722,      16522,      20035,      22134,
  16305,      18179,      21106,      23048,
  15150,      17948,      21394,      23225,
  13582,      15191,      17687,      22333,
  11778,      15546,      18458,      21753,
  16619,      18410,      20827,      23559,
  14229,      15746,      17907,      22474,
  12465,      15327,      20700,      22831,
  15085,      16799,      20182,      23410,
  13026,      16935,      19890,      22892,
  14310,      16854,      19007,      22944,
  14210,      15897,      18891,      23154,
  14633,      18059,      20132,      22899,
  15246,      17781,      19780,      22640,
  16396,      18904,      20912,      23035,
  14618,      17401,      19510,      21672,
  15473,      17497,      19813,      23439,
  18851,      20736,      22323,      23864,
  15055,      16804,      18530,      20916,
  16490,      18196,      19990,      21939,
  11711,      15223,      21154,      23312,
  13294,      15546,      19393,      21472,
  12956,      16060,      20610,      22417,
  11628,      15843,      19617,      22501,
  14106,      16872,      19839,      22689,
  15655,      18192,      20161,      22452,
  12953,      15244,      20619,      23549,
  15322,      17193,      19926,      21762,
  16873,      18676,      20444,      22359,
  14874,      17871,      20083,      21959,
  11534,      14486,      19194,      21857,
  17766,      19617,      21338,      23178,
  13404,      15284,      19080,      23136,
  15392,      17527,      19470,      21953,
  14462,      16153,      17985,      21192,
  17734,      19750,      21903,      23783,
  16973,      19096,      21675,      23815,
  16597,      18936,      21257,      23461,
  15966,      17865,      20602,      22920,
  15416,      17456,      20301,      22972,
  18335,      20093,      21732,      23497,
  15548,      17217,      20679,      23594,
  15208,      16995,      20816,      22870,
  13890,      18015,      20531,      22468,
  13211,      15377,      19951,      22388,
  12852,      14635,      17978,      22680,
  16002,      17732,      20373,      23544,
  11373,      14134,      19534,      22707,
  17329,      19151,      21241,      23462,
  15612,      17296,      19362,      22850,
  15422,      19104,      21285,      23164,
  13792,      17111,      19349,      21370,
  15352,      17876,      20776,      22667,
  15253,      16961,      18921,      22123,
  14108,      17264,      20294,      23246,
  15785,      17897,      20010,      21822,
  17399,      19147,      20915,      22753,
  13010,      15659,      18127,      20840,
  16826,      19422,      22218,      24084,
  18108,      20641,      22695,      24237,
  18018,      20273,      22268,      23920,
  16057,      17821,      21365,      23665,
  16005,      17901,      19892,      23016,
  13232,      16683,      21107,      23221,
  13280,      16615,      19915,      21829,
  14950,      18575,      20599,      22511,
  16337,      18261,      20277,      23216,
  14306,      16477,      21203,      23158,
  12803,      17498,      20248,      22014,
  14327,      17068,      20160,      22006,
  14402,      17461,      21599,      23688,
  16968,      18834,      20896,      23055,
  15070,      17157,      20451,      22315,
  15419,      17107,      21601,      23946,
  16039,      17639,      19533,      21424,
  16326,      19261,      21745,      23673,
  16489,      18534,      21658,      23782,
  16594,      18471,      20549,      22807,
  18973,      21212,      22890,      24278,
  14264,      18674,      21123,      23071,
  15117,      16841,      19239,      23118,
  13762,      15782,      20478,      23230,
  14111,      15949,      20058,      22354,
  14990,      16738,      21139,      23492,
  13735,      16971,      19026,      22158,
  14676,      17314,      20232,      22807,
  16196,      18146,      20459,      22339,
  14747,      17258,      19315,      22437,
  14973,      17778,      20692,      23367,
  15715,      17472,      20385,      22349,
  15702,      18228,      20829,      23410,
  14428,      16188,      20541,      23630,
  16824,      19394,      21365,      23246,
  13069,      16392,      18900,      21121,
  12047,      16640,      19463,      21689,
  14757,      17433,      19659,      23125,
  15185,      16930,      19900,      22540,
  16026,      17725,      19618,      22399,
  16086,      18643,      21179,      23472,
  15462,      17248,      19102,      21196,
  17368,      20016,      22396,      24096,
  12340,      14475,      19665,      23362,
  13636,      16229,      19462,      22728,
  14096,      16211,      19591,      21635,
  12152,      14867,      19943,      22301,
  14492,      17503,      21002,      22728,
  14834,      16788,      19447,      21411,
  14650,      16433,      19326,      22308,
  14624,      16328,      19659,      23204,
  13888,      16572,      20665,      22488,
  12977,      16102,      18841,      22246,
  15523,      18431,      21757,      23738,
  14095,      16349,      18837,      20947,
  13266,      17809,      21088,      22839,
  15427,      18190,      20270,      23143,
  11859,      16753,      20935,      22486,
  12310,      17667,      21736,      23319,
  14021,      15926,      18702,      22002,
  12286,      15299,      19178,      21126,
  15703,      17491,      21039,      23151,
  12272,      14018,      18213,      22570,
  14817,      16364,      18485,      22598,
  17109,      19683,      21851,      23677,
  12657,      14903,      19039,      22061,
  14713,      16487,      20527,      22814,
  14635,      16726,      18763,      21715,
  15878,      18550,      20718,      22906
};

const WebRtc_Word16 WebRtcIlbcfix_kLsfDimCb[LSF_NSPLIT] = {3, 3, 4};
const WebRtc_Word16 WebRtcIlbcfix_kLsfSizeCb[LSF_NSPLIT] = {64,128,128};

const WebRtc_Word16 WebRtcIlbcfix_kLsfMean[LPC_FILTERORDER] = {
  2308,       3652,       5434,       7885,
  10255,      12559,      15160,      17513,
  20328,      22752};

const WebRtc_Word16 WebRtcIlbcfix_kLspMean[LPC_FILTERORDER] = {
  31476, 29565, 25819, 18725, 10276,
  1236, -9049, -17600, -25884, -30618
};

/* Q14 */
const WebRtc_Word16 WebRtcIlbcfix_kLsfWeight20ms[4] = {12288, 8192, 4096, 0};
const WebRtc_Word16 WebRtcIlbcfix_kLsfWeight30ms[6] = {8192, 16384, 10923, 5461, 0, 0};

/*
   cos(x) in Q15
   WebRtcIlbcfix_kCos[i] = cos(pi*i/64.0)
   used in WebRtcIlbcfix_Lsp2Lsf()
*/

const WebRtc_Word16 WebRtcIlbcfix_kCos[64] = {
  32767,  32729,  32610,  32413,  32138,  31786,  31357,  30853,
  30274,  29622,  28899,  28106,  27246,  26320,  25330,  24279,
  23170,  22006,  20788,  19520,  18205,  16846,  15447,  14010,
  12540,  11039,   9512,   7962,   6393,   4808,   3212,   1608,
  0,  -1608,  -3212,  -4808,  -6393,  -7962,  -9512, -11039,
  -12540, -14010, -15447, -16846, -18205, -19520, -20788, -22006,
  -23170, -24279, -25330, -26320, -27246, -28106, -28899, -29622,
  -30274, -30853, -31357, -31786, -32138, -32413, -32610, -32729
};

/*
   Derivative in Q19, used to interpolate between the
   WebRtcIlbcfix_kCos[] values to get a more exact y = cos(x)
*/
const WebRtc_Word16 WebRtcIlbcfix_kCosDerivative[64] = {
  -632,  -1893,  -3150,  -4399,  -5638,  -6863,  -8072,  -9261,
  -10428, -11570, -12684, -13767, -14817, -15832, -16808, -17744,
  -18637, -19486, -20287, -21039, -21741, -22390, -22986, -23526,
  -24009, -24435, -24801, -25108, -25354, -25540, -25664, -25726,
  -25726, -25664, -25540, -25354, -25108, -24801, -24435, -24009,
  -23526, -22986, -22390, -21741, -21039, -20287, -19486, -18637,
  -17744, -16808, -15832, -14817, -13767, -12684, -11570, -10428,
  -9261,  -8072,  -6863,  -5638,  -4399,  -3150,  -1893,   -632};

/*
  Table in Q15, used for a2lsf conversion
  WebRtcIlbcfix_kCosGrid[i] = cos((2*pi*i)/(float)(2*COS_GRID_POINTS));
*/

const WebRtc_Word16 WebRtcIlbcfix_kCosGrid[COS_GRID_POINTS + 1] = {
  32760, 32723, 32588, 32364, 32051, 31651, 31164, 30591,
  29935, 29196, 28377, 27481, 26509, 25465, 24351, 23170,
  21926, 20621, 19260, 17846, 16384, 14876, 13327, 11743,
  10125, 8480, 6812, 5126, 3425, 1714, 0, -1714, -3425,
  -5126, -6812, -8480, -10125, -11743, -13327, -14876,
  -16384, -17846, -19260, -20621, -21926, -23170, -24351,
  -25465, -26509, -27481, -28377, -29196, -29935, -30591,
  -31164, -31651, -32051, -32364, -32588, -32723, -32760
};

/*
   Derivative of y = acos(x) in Q12
   used in WebRtcIlbcfix_Lsp2Lsf()
*/

const WebRtc_Word16 WebRtcIlbcfix_kAcosDerivative[64] = {
  -26887, -8812, -5323, -3813, -2979, -2444, -2081, -1811,
  -1608, -1450, -1322, -1219, -1132, -1059, -998, -946,
  -901, -861, -827, -797, -772, -750, -730, -713,
  -699, -687, -677, -668, -662, -657, -654, -652,
  -652, -654, -657, -662, -668, -677, -687, -699,
  -713, -730, -750, -772, -797, -827, -861, -901,
  -946, -998, -1059, -1132, -1219, -1322, -1450, -1608,
  -1811, -2081, -2444, -2979, -3813, -5323, -8812, -26887
};


/* Tables for quantization of start state */

/* State quantization tables */
const WebRtc_Word16 WebRtcIlbcfix_kStateSq3[8] = { /* Values in Q13 */
  -30473, -17838, -9257, -2537,
  3639, 10893, 19958, 32636
};

/* This table defines the limits for the selection of the freqg
   less or equal than value 0 => index = 0
   less or equal than value k => index = k
*/
const WebRtc_Word32 WebRtcIlbcfix_kChooseFrgQuant[64] = {
  118, 163, 222, 305, 425, 604,
  851, 1174, 1617, 2222, 3080, 4191,
  5525, 7215, 9193, 11540, 14397, 17604,
  21204, 25209, 29863, 35720, 42531, 50375,
  59162, 68845, 80108, 93754, 110326, 129488,
  150654, 174328, 201962, 233195, 267843, 308239,
  354503, 405988, 464251, 531550, 608652, 697516,
  802526, 928793, 1080145, 1258120, 1481106, 1760881,
  2111111, 2546619, 3078825, 3748642, 4563142, 5573115,
  6887601, 8582108, 10797296, 14014513, 18625760, 25529599,
  37302935, 58819185, 109782723, WEBRTC_SPL_WORD32_MAX
};

const WebRtc_Word16 WebRtcIlbcfix_kScale[64] = {
  /* Values in Q16 */
  29485, 25003, 21345, 18316, 15578, 13128, 10973, 9310, 7955,
  6762, 5789, 4877, 4255, 3699, 3258, 2904, 2595, 2328,
  2123, 1932, 1785, 1631, 1493, 1370, 1260, 1167, 1083,
  /* Values in Q21 */
  32081, 29611, 27262, 25229, 23432, 21803, 20226, 18883, 17609,
  16408, 15311, 14327, 13390, 12513, 11693, 10919, 10163, 9435,
  8739, 8100, 7424, 6813, 6192, 5648, 5122, 4639, 4207, 3798,
  3404, 3048, 2706, 2348, 2036, 1713, 1393, 1087, 747
};

/*frgq in fixpoint, but already computed like this:
  for(i=0; i<64; i++){
  a = (pow(10,frgq[i])/4.5);
  WebRtcIlbcfix_kFrgQuantMod[i] = round(a);
  }

  Value 0 :36 in Q8
  37:58 in Q5
  59:63 in Q3
*/
const WebRtc_Word16 WebRtcIlbcfix_kFrgQuantMod[64] = {
  /* First 37 values in Q8 */
  569, 671, 786, 916, 1077, 1278,
  1529, 1802, 2109, 2481, 2898, 3440,
  3943, 4535, 5149, 5778, 6464, 7208,
  7904, 8682, 9397, 10285, 11240, 12246,
  13313, 14382, 15492, 16735, 18131, 19693,
  21280, 22912, 24624, 26544, 28432, 30488,
  32720,
  /* 22 values in Q5 */
  4383, 4684, 5012, 5363, 5739, 6146,
  6603, 7113, 7679, 8285, 9040, 9850,
  10838, 11882, 13103, 14467, 15950, 17669,
  19712, 22016, 24800, 28576,
  /* 5 values in Q3 */
  8240, 9792, 12040, 15440, 22472
};

/* Constants for codebook search and creation */

/* Expansion filter to get additional cb section.
 * Q12 and reversed compared to flp
 */
const WebRtc_Word16 WebRtcIlbcfix_kCbFiltersRev[CB_FILTERLEN]={
  -140, 446, -755, 3302, 2922, -590, 343, -138};

/* Weighting coefficients for short lags.
 * [0.2 0.4 0.6 0.8] in Q15 */
const WebRtc_Word16 WebRtcIlbcfix_kAlpha[4]={
  6554, 13107, 19661, 26214};

/* Ranges for search and filters at different subframes */

const WebRtc_Word16 WebRtcIlbcfix_kSearchRange[5][CB_NSTAGES]={
  {58,58,58}, {108,44,44}, {108,108,108}, {108,108,108}, {108,108,108}};

const WebRtc_Word16 WebRtcIlbcfix_kFilterRange[5]={63, 85, 125, 147, 147};

/* Gain Quantization for the codebook gains of the 3 stages */

/* Q14 (one extra value (max WebRtc_Word16) to simplify for the search) */
const WebRtc_Word16 WebRtcIlbcfix_kGainSq3[9]={
  -16384, -10813, -5407, 0, 4096, 8192,
  12288, 16384, 32767};

/* Q14 (one extra value (max WebRtc_Word16) to simplify for the search) */
const WebRtc_Word16 WebRtcIlbcfix_kGainSq4[17]={
  -17203, -14746, -12288, -9830, -7373, -4915,
  -2458, 0, 2458, 4915, 7373, 9830,
  12288, 14746, 17203, 19661, 32767};

/* Q14 (one extra value (max WebRtc_Word16) to simplify for the search) */
const WebRtc_Word16 WebRtcIlbcfix_kGainSq5[33]={
  614,        1229,        1843,        2458,        3072,       3686,
  4301,        4915,        5530,        6144,        6758,        7373,
  7987,        8602,        9216,        9830,       10445,       11059,
  11674,       12288,       12902,       13517,       14131,       14746,
  15360,       15974,       16589,       17203,       17818,       18432,
  19046,       19661,    32767};

/* Q14 gain_sq5Tbl squared in Q14 */
const WebRtc_Word16 WebRtcIlbcfix_kGainSq5Sq[32] = {
  23,   92,    207,  368,  576,  829,
  1129,  1474,   1866,  2304,  2787,  3317,
  3893,  4516,   5184,  5897,  6658,  7464,
  8318,  9216,   10160,  11151,  12187,  13271,
  14400,  15574,   16796,  18062,  19377,  20736,
  22140,  23593
};

const WebRtc_Word16* const WebRtcIlbcfix_kGain[3] =
{WebRtcIlbcfix_kGainSq5, WebRtcIlbcfix_kGainSq4, WebRtcIlbcfix_kGainSq3};


/* Tables for the Enhancer, using upsamling factor 4 (ENH_UPS0 = 4) */

const WebRtc_Word16 WebRtcIlbcfix_kEnhPolyPhaser[ENH_UPS0][ENH_FLO_MULT2_PLUS1]={
  {0,    0,    0, 4096,    0,  0,   0},
  {64, -315, 1181, 3531, -436, 77, -64},
  {97, -509, 2464, 2464, -509, 97, -97},
  {77, -436, 3531, 1181, -315, 64, -77}
};

const WebRtc_Word16 WebRtcIlbcfix_kEnhWt[3] = {
  4800, 16384, 27968 /* Q16 */
};

const WebRtc_Word16 WebRtcIlbcfix_kEnhPlocs[ENH_NBLOCKS_TOT] = {
  160, 480, 800, 1120, 1440, 1760, 2080, 2400  /* Q(-2) */
};

/* PLC table */

const WebRtc_Word16 WebRtcIlbcfix_kPlcPerSqr[6] = { /* Grid points for square of periodiciy in Q15 */
  839, 1343, 2048, 2998, 4247, 5849
};

const WebRtc_Word16 WebRtcIlbcfix_kPlcPitchFact[6] = { /* Value of y=(x^4-0.4)/(0.7-0.4) in grid points in Q15 */
  0, 5462, 10922, 16384, 21846, 27306
};

const WebRtc_Word16 WebRtcIlbcfix_kPlcPfSlope[6] = { /* Slope of y=(x^4-0.4)/(0.7-0.4) in Q11 */
  26667, 18729, 13653, 10258, 7901, 6214
};
