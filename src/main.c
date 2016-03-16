#include "main.h"
Window *main_window;
Layer *main_layer, *root_layer;
uint8_t watch_battery_level = 255;
bool watch_charging=false;

void dirty() {if(main_window) if(window_get_root_layer(main_window)) layer_mark_dirty(window_get_root_layer(main_window));}
bool emulator=false;

// Pixel Mask is from the official Pebble source code:
//   https://github.com/pebble/qemu/blob/master/hw/display/pebble_snowy_display.c#L1048-L1070
//static uint8_t *get_pixel_mask(void) {
  static uint8_t pixel_mask[] = {
    76, 71, 66, 63, 60, 57, 55, 52, 50, 48,
    46, 45, 43, 41, 40, 38, 37, 36, 34, 33,
    32, 31, 29, 28, 27, 26, 25, 24, 23, 22,
    22, 21, 20, 19, 18, 18, 17, 16, 15, 15,
    14, 13, 13, 12, 12, 11, 10, 10,  9,  9,
     8,  8,  7,  7,  7,  6,  6,  5,  5,  5,
     4,  4,  4,  3,  3,  3,  2,  2,  2,  2,
     2,  1,  1,  1,  1,  1,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  };
//  return pixel_mask;
//}
/*
 --------------------------------
   Notes
 --------------------------------
 Total Pixels on Round Display:
   sum(pixel_mask[]) = 1652 (invisible masked pixels per quadrant)
   1652 * 4(corners) = 6608 pixels masked off
   180*180 = 32400 total square pixels
   32400 - 6608 pixels masked = 25792 total visible pixels
   
 Round framebuffer starts at upper-left pixel of square 180x180 screen,
   then 76 pixels are masked before first visible pixel (pixel_mask[0] = 76)
   therefore, first visible pixel starts at: framebuffer + 76

*/
void calculate_arrays_and_log_to_console() {
  #define elements_per_line 10
  int32_t ptr = 0;  // string pointer for concatenation
  int32_t px = 76;  // first visible pixel starts at: framebuffer + 76
  int8_t counter = 0;  // counts number of elements per printout
  char buffer[1000];  // overkill, I know.

  LOG("Calculating Pixel Position - Basalt");
  px = 0; // Aplite and Basalt
  // Calculate for Basalt
    for (uint8_t i=0; i<168; i++) {
      //ptr += snprintf(buffer+ptr, sizeof(buffer)-ptr, " %d,", (int)(px));    // Calculate Left Edge Aplite and Basalt
      ptr += snprintf(buffer+ptr, sizeof(buffer)-ptr, " %d,", (int)(px+10));    // Calculate Middle (left middle pixel) Aplite
    //ptr += snprintf(buffer+ptr, sizeof(buffer)-ptr, " %d,", (int)(px+(144/2)));    // Calculate Middle (left middle pixel) Basalt
    //ptr += snprintf(buffer+ptr, sizeof(buffer)-ptr, " %d,", (int)(px+(144/2)+1));    // Calculate Middle (right middle pixel) Basalt
    counter++;
    if(counter==elements_per_line) {
      counter=0;
      ptr=0;
      LOG("%s", buffer);
    }
    //px = px + 144;  // Basalt
    px = px + 20;  // Aplite
  }
  
  if(counter>0)  // display the remainder of elements
    LOG("%s", buffer);
LOG("Done Calculating");
  /*
  LOG("Calculating Pixel Position");
  px = 0; // Aplite and Basalt
  // px = 76 // Chalk  (first visible pixel starts at: framebuffer + 76)
  for (uint8_t i=0; i<90; i++) {
      ptr += snprintf(buffer+ptr, sizeof(buffer)-ptr, "%d, ", (int)(px));    // Calculate Left Edge Basalt
    //ptr += snprintf(buffer+ptr, sizeof(buffer)-ptr, "%d, ", (int)(px+(144/2)));    // Calculate Middle (left middle pixel) Basalt
    //ptr += snprintf(buffer+ptr, sizeof(buffer)-ptr, "%d, ", (int)(px+(90-pixel_mask[i])));    // Calculate Middle (left middle pixel)
    //ptr += snprintf(buffer+ptr, sizeof(buffer)-ptr, "%d, ", (int)(px+(90-pixel_mask[i])+1));  // Calculate Middle (right middle pixel)
    //ptr += snprintf(buffer+ptr, sizeof(buffer)-ptr, "%d, ", (int)px);                         // Calculate Left Edge
    counter++;
    if(counter==elements_per_line) {
      counter=0;
      ptr=0;
      LOG("%s", buffer);
    }
    px = px + 144;
    //px = px + ((90-pixel_mask[i])*2);
  }

  for (uint8_t i=0; i<90; ++i) {
      ptr += snprintf(buffer+ptr, sizeof(buffer)-ptr, "%d, ", (int)(px));   // Calculate Left Edge Basalt
    //ptr += snprintf(buffer+ptr, sizeof(buffer)-ptr, "%d, ", (int)(px+144/2));   // Calculate Middle (left middle pixel)
    //ptr += snprintf(buffer+ptr, sizeof(buffer)-ptr, "%d, ", (int)(px+(90-pixel_mask[89-i])+1)); // Calculate Middle (right middle pixel)
    //ptr += snprintf(buffer+ptr, sizeof(buffer)-ptr, "%d, ", (int)px);                           // Calculate Left Edge
    counter++;
    if(counter==elements_per_line) {
      counter=0;
      ptr=0;
      LOG("%s", buffer);
    }
    px = px + 144;
    //px = px + ((90-pixel_mask[89-i])*2);
  }
  
  if(counter>0)  // display the remainder of elements
    LOG("%s", buffer);

  LOG("Done Calculating");

  
  // Chalk only:
  LOG("Calculating Width");
  for (uint8_t i=0; i<90; ++i) {
    ptr += snprintf(buffer+ptr, sizeof(buffer)-ptr, "%d, ", (90-pixel_mask[i]));  // Calculate Width
    counter++;
    if(counter==elements_per_line) {
      counter=0;
      ptr=0;
      LOG("%s", buffer);
    }
  }
  
  for (uint8_t i=0; i<90; ++i) {
    ptr += snprintf(buffer+ptr, sizeof(buffer)-ptr, "%d, ", (90-pixel_mask[89-i]));  // Calculate Width
    counter++;
    if(counter==elements_per_line) {
      counter=0;
      ptr=0;
      LOG("%s", buffer);
    }
  }
  
  if(counter>0)
    LOG("%s", buffer);
  
  LOG("Done Calculating");
  */
}



// ===============================================================================================================================================
//  Button Functions
// ===============================================================================================================================================
void up_click_handler  (ClickRecognizerRef recognizer, void *context) { //   UP   button
  
}

void sl_click_handler  (ClickRecognizerRef recognizer, void *context) { // SELECT button
  calculate_arrays_and_log_to_console();
}

void dn_click_handler  (ClickRecognizerRef recognizer, void *context) { //  DOWN  button
  
}

void bk_click_handler  (ClickRecognizerRef recognizer, void *context) { //  BACK  button
  window_stack_pop_all(false);
}

void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP,     up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN,   dn_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, sl_click_handler);
  window_single_click_subscribe(BUTTON_ID_BACK,   bk_click_handler);
}
// ===============================================================================================================================================
//  Output of calculate_arrays_and_log_to_console()
// ===============================================================================================================================================
int16_t row_aplite[] = {
     0,   20,   40,   60,   80,  100,  120,  140,
   160,  180,  200,  220,  240,  260,  280,  300,
   320,  340,  360,  380,  400,  420,  440,  460,
   480,  500,  520,  540,  560,  580,  600,  620,
   640,  660,  680,  700,  720,  740,  760,  780,
   800,  820,  840,  860,  880,  900,  920,  940,
   960,  980, 1000, 1020, 1040, 1060, 1080, 1100,
  1120, 1140, 1160, 1180, 1200, 1220, 1240, 1260,
  1280, 1300, 1320, 1340, 1360, 1380, 1400, 1420,
  1440, 1460, 1480, 1500, 1520, 1540, 1560, 1580,
  1600, 1620, 1640, 1660, 1680, 1700, 1720, 1740,
  1760, 1780, 1800, 1820, 1840, 1860, 1880, 1900,
  1920, 1940, 1960, 1980, 2000, 2020, 2040, 2060,
  2080, 2100, 2120, 2140, 2160, 2180, 2200, 2220,
  2240, 2260, 2280, 2300, 2320, 2340, 2360, 2380,
  2400, 2420, 2440, 2460, 2480, 2500, 2520, 2540,
  2560, 2580, 2600, 2620, 2640, 2660, 2680, 2700,
  2720, 2740, 2760, 2780, 2800, 2820, 2840, 2860,
  2880, 2900, 2920, 2940, 2960, 2980, 3000, 3020,
  3040, 3060, 3080, 3100, 3120, 3140, 3160, 3180,
  3200, 3220, 3240, 3260, 3280, 3300, 3320, 3340
};
int16_t *left_center_aplite = row_aplite + (168/2);  // Left Pixel, Vertically Centered

int16_t middle_aplite[] = {
    10,   30,   50,   70,   90,  110,  130,  150,
   170,  190,  210,  230,  250,  270,  290,  310,
   330,  350,  370,  390,  410,  430,  450,  470,
   490,  510,  530,  550,  570,  590,  610,  630,
   650,  670,  690,  710,  730,  750,  770,  790,
   810,  830,  850,  870,  890,  910,  930,  950,
   970,  990, 1010, 1030, 1050, 1070, 1090, 1110,
  1130, 1150, 1170, 1190, 1210, 1230, 1250, 1270,
  1290, 1310, 1330, 1350, 1370, 1390, 1410, 1430,
  1450, 1470, 1490, 1510, 1530, 1550, 1570, 1590,
  1610, 1630, 1650, 1670, 1690, 1710, 1730, 1750,
  1770, 1790, 1810, 1830, 1850, 1870, 1890, 1910,
  1930, 1950, 1970, 1990, 2010, 2030, 2050, 2070,
  2090, 2110, 2130, 2150, 2170, 2190, 2210, 2230,
  2250, 2270, 2290, 2310, 2330, 2350, 2370, 2390,
  2410, 2430, 2450, 2470, 2490, 2510, 2530, 2550,
  2570, 2590, 2610, 2630, 2650, 2670, 2690, 2710,
  2730, 2750, 2770, 2790, 2810, 2830, 2850, 2870,
  2890, 2910, 2930, 2950, 2970, 2990, 3010, 3030,
  3050, 3070, 3090, 3110, 3130, 3150, 3170, 3190,
  3210, 3230, 3250, 3270, 3290, 3310, 3330, 3350
};
int16_t *origin_aplite = middle_aplite + (168/2);  // Pixel Centered Horizontally and Vertically (upper left)

int16_t row_basalt[] = {
      0,   144,   288,   432,   576,   720,   864,  1008,
   1152,  1296,  1440,  1584,  1728,  1872,  2016,  2160,
   2304,  2448,  2592,  2736,  2880,  3024,  3168,  3312,
   3456,  3600,  3744,  3888,  4032,  4176,  4320,  4464,
   4608,  4752,  4896,  5040,  5184,  5328,  5472,  5616,
   5760,  5904,  6048,  6192,  6336,  6480,  6624,  6768,
   6912,  7056,  7200,  7344,  7488,  7632,  7776,  7920,
   8064,  8208,  8352,  8496,  8640,  8784,  8928,  9072,
   9216,  9360,  9504,  9648,  9792,  9936, 10080, 10224,
  10368, 10512, 10656, 10800, 10944, 11088, 11232, 11376,
  11520, 11664, 11808, 11952, 12096, 12240, 12384, 12528,
  12672, 12816, 12960, 13104, 13248, 13392, 13536, 13680,
  13824, 13968, 14112, 14256, 14400, 14544, 14688, 14832,
  14976, 15120, 15264, 15408, 15552, 15696, 15840, 15984,
  16128, 16272, 16416, 16560, 16704, 16848, 16992, 17136,
  17280, 17424, 17568, 17712, 17856, 18000, 18144, 18288,
  18432, 18576, 18720, 18864, 19008, 19152, 19296, 19440,
  19584, 19728, 19872, 20016, 20160, 20304, 20448, 20592,
  20736, 20880, 21024, 21168, 21312, 21456, 21600, 21744,
  21888, 22032, 22176, 22320, 22464, 22608, 22752, 22896,
  23040, 23184, 23328, 23472, 23616, 23760, 23904, 24048
};
int16_t *left_center_basalt = row_basalt + (168/2);  // Left Pixel, Vertically Centered

int16_t middle_basalt[] = {
     72,   216,   360,   504,   648,   792,   936,  1080,
   1224,  1368,  1512,  1656,  1800,  1944,  2088,  2232,
   2376,  2520,  2664,  2808,  2952,  3096,  3240,  3384,
   3528,  3672,  3816,  3960,  4104,  4248,  4392,  4536,
   4680,  4824,  4968,  5112,  5256,  5400,  5544,  5688,
   5832,  5976,  6120,  6264,  6408,  6552,  6696,  6840,
   6984,  7128,  7272,  7416,  7560,  7704,  7848,  7992,
   8136,  8280,  8424,  8568,  8712,  8856,  9000,  9144,
   9288,  9432,  9576,  9720,  9864, 10008, 10152, 10296,
  10440, 10584, 10728, 10872, 11016, 11160, 11304, 11448,
  11592, 11736, 11880, 12024, 12168, 12312, 12456, 12600,
  12744, 12888, 13032, 13176, 13320, 13464, 13608, 13752,
  13896, 14040, 14184, 14328, 14472, 14616, 14760, 14904,
  15048, 15192, 15336, 15480, 15624, 15768, 15912, 16056,
  16200, 16344, 16488, 16632, 16776, 16920, 17064, 17208,
  17352, 17496, 17640, 17784, 17928, 18072, 18216, 18360,
  18504, 18648, 18792, 18936, 19080, 19224, 19368, 19512,
  19656, 19800, 19944, 20088, 20232, 20376, 20520, 20664,
  20808, 20952, 21096, 21240, 21384, 21528, 21672, 21816,
  21960, 22104, 22248, 22392, 22536, 22680, 22824, 22968,
  23112, 23256, 23400, 23544, 23688, 23832, 23976, 24120
};
int16_t *origin_basalt = middle_basalt + (168/2);  // Pixel Centered Horizontally and Vertically (upper left)


int16_t row_chalk[] = {  // Offset of Leftmost pixel of row[y].  Note: screen pixel color = *(framebuffer + *(row_chalk + y) + x) or framebuffer[row_chalk[y] + x]
     76,   104,   142,   190,   244,   304,   370,   440,   516,   596,
    680,   768,   858,   952,  1050,  1150,  1254,  1360,  1468,  1580,
   1694,  1810,  1928,  2050,  2174,  2300,  2428,  2558,  2690,  2824,
   2960,  3096,  3234,  3374,  3516,  3660,  3804,  3950,  4098,  4248,
   4398,  4550,  4704,  4858,  5014,  5170,  5328,  5488,  5648,  5810,
   5972,  6136,  6300,  6466,  6632,  6798,  6966,  7134,  7304,  7474,
   7644,  7816,  7988,  8160,  8334,  8508,  8682,  8858,  9034,  9210,
   9386,  9562,  9740,  9918, 10096, 10274, 10452, 10632, 10812, 10992,
  11172, 11352, 11532, 11712, 11892, 12072, 12252, 12432, 12612, 12792,
  //-----------------------------------------------------------------//
  12972, 13152, 13332, 13512, 13692, 13872, 14052, 14232, 14412, 14592,
  14772, 14952, 15132, 15312, 15492, 15670, 15848, 16026, 16204, 16382,
  16558, 16734, 16910, 17086, 17262, 17436, 17610, 17784, 17956, 18128,
  18300, 18470, 18640, 18810, 18978, 19146, 19312, 19478, 19644, 19808,
  19972, 20134, 20296, 20456, 20616, 20774, 20930, 21086, 21240, 21394,
  21546, 21696, 21846, 21994, 22140, 22284, 22428, 22570, 22710, 22848,
  22984, 23120, 23254, 23386, 23516, 23644, 23770, 23894, 24016, 24134,
  24250, 24364, 24476, 24584, 24690, 24794, 24894, 24992, 25086, 25176,
  25264, 25348, 25428, 25504, 25574, 25640, 25700, 25754, 25802, 25840
};
int16_t *left_center_chalk = row_chalk + 90;  // Left Pixel Horizontally, Vertically Centered

int16_t middle_chalk[] = {  // Middle pixel (left of 2 middle pixels)   = row_chalk[y] + half_width_chalk[y]
    90,    123,   166,   217,   274,   337,   405,   478,   556,   638,
    724,   813,   905,  1001,  1100,  1202,  1307,  1414,  1524,  1637,
   1752,  1869,  1989,  2112,  2237,  2364,  2493,  2624,  2757,  2892,
   3028,  3165,  3304,  3445,  3588,  3732,  3877,  4024,  4173,  4323,
   4474,  4627,  4781,  4936,  5092,  5249,  5408,  5568,  5729,  5891,
   6054,  6218,  6383,  6549,  6715,  6882,  7050,  7219,  7389,  7559,
   7730,  7902,  8074,  8247,  8421,  8595,  8770,  8946,  9122,  9298,
   9474,  9651,  9829, 10007, 10185, 10363, 10542, 10722, 10902, 11082,
  11262, 11442, 11622, 11802, 11982, 12162, 12342, 12522, 12702, 12882,
  //-----------------------------------------------------------------//
  13062, 13242, 13422, 13602, 13782, 13962, 14142, 14322, 14502, 14682,
  14862, 15042, 15222, 15402, 15581, 15759, 15937, 16115, 16293, 16470,
  16646, 16822, 16998, 17174, 17349, 17523, 17697, 17870, 18042, 18214,
  18385, 18555, 18725, 18894, 19062, 19229, 19395, 19561, 19726, 19890,
  20053, 20215, 20376, 20536, 20695, 20852, 21008, 21163, 21317, 21470,
  21621, 21771, 21920, 22067, 22212, 22356, 22499, 22640, 22779, 22916,
  23052, 23187, 23320, 23451, 23580, 23707, 23832, 23955, 24075, 24192,
  24307, 24420, 24530, 24637, 24742, 24844, 24943, 25039, 25131, 25220,
  25306, 25388, 25466, 25539, 25607, 25670, 25727, 25778, 25821, 25854
};
int16_t *origin_chalk = middle_chalk + 90;  // Pixel Centered Horizontally and Vertically (upper left)

/*
// Middle_right is dumb.
int16_t middle_right_chalk[] = {  // Middle pixel (right of 2 middle pixels) = row[y]+width[y]+1
     91,   124,   167,   218,   275,   338,   406,   479,   557,   639, 
    725,   814,   906,  1002,  1101,  1203,  1308,  1415,  1525,  1638, 
   1753,  1870,  1990,  2113,  2238,  2365,  2494,  2625,  2758,  2893, 
   3029,  3166,  3305,  3446,  3589,  3733,  3878,  4025,  4174,  4324, 
   4475,  4628,  4782,  4937,  5093,  5250,  5409,  5569,  5730,  5892, 
   6055,  6219,  6384,  6550,  6716,  6883,  7051,  7220,  7390,  7560, 
   7731,  7903,  8075,  8248,  8422,  8596,  8771,  8947,  9123,  9299, 
   9475,  9652,  9830, 10008, 10186, 10364, 10543, 10723, 10903, 11083, 
  11263, 11443, 11623, 11803, 11983, 12163, 12343, 12523, 12703, 12883, 

  13063, 13243, 13423, 13603, 13783, 13963, 14143, 14323, 14503, 14683, 
  14863, 15043, 15223, 15403, 15582, 15760, 15938, 16116, 16294, 16471, 
  16647, 16823, 16999, 17175, 17350, 17524, 17698, 17871, 18043, 18215, 
  18386, 18556, 18726, 18895, 19063, 19230, 19396, 19562, 19727, 19891, 
  20054, 20216, 20377, 20537, 20696, 20853, 21009, 21164, 21318, 21471, 
  21622, 21772, 21921, 22068, 22213, 22357, 22500, 22641, 22780, 22917, 
  23053, 23188, 23321, 23452, 23581, 23708, 23833, 23956, 24076, 24193, 
  24308, 24421, 24531, 24638, 24743, 24845, 24944, 25040, 25132, 25221, 
  25307, 25389, 25467, 25540, 25608, 25671, 25728, 25779, 25822, 25855
};
*/

#define half_height_chalk half_width_chalk
int16_t half_width_chalk[] = {  // Half of width of row[y].  Double for row width.  Also works for half-height of column[x].
  14, 19, 24, 27, 30, 33, 35, 38, 40, 42,
  44, 45, 47, 49, 50, 52, 53, 54, 56, 57,
  58, 59, 61, 62, 63, 64, 65, 66, 67, 68,
  68, 69, 70, 71, 72, 72, 73, 74, 75, 75,
  76, 77, 77, 78, 78, 79, 80, 80, 81, 81,
  82, 82, 83, 83, 83, 84, 84, 85, 85, 85,
  86, 86, 86, 87, 87, 87, 88, 88, 88, 88,
  88, 89, 89, 89, 89, 89, 90, 90, 90, 90,
  90, 90, 90, 90, 90, 90, 90, 90, 90, 90,
  //----------------------------------//
  90, 90, 90, 90, 90, 90, 90, 90, 90, 90,
  90, 90, 90, 90, 89, 89, 89, 89, 89, 88,
  88, 88, 88, 88, 87, 87, 87, 86, 86, 86,
  85, 85, 85, 84, 84, 83, 83, 83, 82, 82,
  81, 81, 80, 80, 79, 78, 78, 77, 77, 76,
  75, 75, 74, 73, 72, 72, 71, 70, 69, 68,
  68, 67, 66, 65, 64, 63, 62, 61, 59, 58,
  57, 56, 54, 53, 52, 50, 49, 47, 45, 44,
  42, 40, 38, 35, 33, 30, 27, 24, 19, 14
};

#define origin_half_height_chalk origin_half_width_chalk
int16_t *origin_half_width_chalk = half_width_chalk + 90;  // Centered Vertically (or Horizontally if half_height)


#define row         row_chalk
#define row_center  middle_chalk
#define row_radius  half_width_chalk

#define origin      origin_chalk
#define left_center left_center_chalk
#define right_center

  
void fill_screen(GContext *ctx, uint8_t color) {
  uint8_t *framebuffer = (uint8_t*)*(uintptr_t*)ctx;  // First pixel on PTR doesn't start til framebuffer + 76 (76 = pixel_mask[0])
  memset(framebuffer, color, IF_APLITE_BASALT_CHALK_ELSE(144*168/8, 144*168, 76 + 25792, 0));
}

// ===============================================================================================================================================
//  Drawing Functions
// ===============================================================================================================================================
void main_layer_update(Layer *me, GContext *ctx) {
  // Fill the whole background with Dark Grey
  fill_screen(ctx, 0b11010101);
  
  uint8_t *framebuffer = (uint8_t*)*(uintptr_t*)ctx;  // framebuffer is now an array of uint8_t's. Could also = (uint8_t*)*(size_t*)ctx;
  
  #define OriginDrawing true
  #define NormalDrawing false
  
  #if OriginDrawing  // Origin drawing (where center screen pixel is: x=0 and y=0)
  
  // Horizontal Bar (using Origin)
  for (int8_t y=40; y<50; ++y) {
    // <-- insert vertical bounds checking here, if ya feel like it...
    int8_t halfwidth = *(origin_half_width_chalk + y);          // Formatting Choice 1: Pointer formatting
    for (int8_t x = -halfwidth; x < halfwidth; ++x)             // The entire width
      // <-- insert horizontal bounds checking here, if ya feel like it...
      *(framebuffer + *(origin_chalk + y) + x) = 0b11001100;    // Formatting Choice 1: Pointer formatting
  }
  
  // Vertical Bar (using Origin)
  for (int8_t y=-120; y<100; ++y) {  // Purposly going beyond bounds to test vertical bounds checking
    if(y>=-90 && y<90) {  // vertical bounds checking
      int8_t halfwidth = origin_half_width_chalk[y];          // Formatting Choice 2: Array formatting (looks nicer)

      for (int8_t x = -50; x < 5; ++x)
        if(x>=-halfwidth && x<halfwidth)  // Horizontal Bounds Checking
          framebuffer[origin_chalk[y] + x] = 0b11110000;      // Formatting Choice 2: Array formatting (looks nicer)
    }
  }
  
  // Box (using Origin)
  for (int8_t y=-80; y<-50; ++y) {
    if(y>=-90 && y<90) {  // vertical bounds checking (not actually needed here)
      int8_t halfwidth = *(origin_half_width_chalk + y);
      for (int8_t x = 50; x < 85; ++x)
        if(x>=-halfwidth && x<halfwidth)  // Horizontal Bounds Checking (is needed in this instance)
          *(framebuffer + *(origin_chalk + y) + x) = 0b11110011;
    }
  }
  #endif
  
  #if NormalDrawing  // Normal Coordinate System (upper-left corner is: x=0, y=0)
  
  // Works to fill circle except left 5 pixels and right 3 pixels
  for (uint8_t y=0; y<180; ++y)
    for (int16_t p = middle[y] - row_half_width[y] + 5; p < middle[y] + row_half_width[y] - 3; ++p)
      framebuffer[p] = 0b11001100;
    
  // Works to fill circle except left 5 pixels and right 3 pixels
  for (uint8_t y=0; y<180; ++y)
    for (int16_t x = 5; x < row_half_width[y]*2-3; ++x)
      framebuffer[row[y]+x] = 0b11001100;
    
  // Left pixel
  for (uint8_t y=0; y<180; ++y)
    framebuffer[middle[y] - row_half_width[y]] = 0b11001100;
  
  // Left pixel
  for (uint8_t y=0; y<180; ++y)
    framebuffer[row[y]] = 0b11001100;
    
  // Center (left of 2 center) pixel
  for (uint8_t y=0; y<180; ++y)
    framebuffer[middle[y]] = 0b11110000;

  // Right Pixel
  for (uint8_t y=0; y<180; ++y)
    framebuffer[middle[y] + row_half_width[y] - 1] = 0b11110011;
  #endif
}


// ===============================================================================================================================================
//  Main Pebble Functions
// ===============================================================================================================================================
void main_window_load(Window *window) {
  // Set up window
  window_set_click_config_provider(window, click_config_provider);
  root_layer = window_get_root_layer(window);
  GRect root_frame=layer_get_frame(root_layer);
  
  // Add Main Layer
  main_layer = layer_create(root_frame);
  layer_set_update_proc(main_layer, main_layer_update);
  layer_add_child(root_layer, main_layer);
}

void main_window_unload(Window *window) {

}

void battery_handler(BatteryChargeState charge_state) {
  static int previous_state = 0;  // To stop the friggin CONSTANT messages with no state change!
  watch_battery_level = charge_state.charge_percent;
  watch_charging = charge_state.is_charging;
    
  if (charge_state.is_charging || charge_state.is_plugged) {
    if(previous_state != 1) {
      LOG("External Power Detected: Backlight On");
      previous_state = 1;
      light_enable(true);
    }
  } else {
    if(previous_state != 2) {
      LOG("Battery Power Detected: Backlight Auto");
      if(previous_state!=0)  // Stop turning backlight off at program start
        light_enable(false);
      previous_state = 2;
    }
  }
  dirty();
  LOG("Watch Battery: %03d%% %s", watch_battery_level, watch_charging?"Charging":"Discharging");
}

void init() {
  //-----------------------------------------------------------------
  // Create main Window
  //-----------------------------------------------------------------
  main_window = window_create();
  window_set_window_handlers(main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_set_background_color(main_window, GColorWhite);//IF_BW_COLOR(GColorWhite, GColorRed));
  window_stack_push(main_window, true);

  //-----------------------------------------------------------------
  // Detect if running in an emulator or real watch
  //-----------------------------------------------------------------
  emulator = watch_info_get_model()==WATCH_INFO_MODEL_UNKNOWN;
  if(emulator) {
    light_enable(true);  // Good colors on emulator
    LOG("Emulator Detected: Turning Backlight On");
  } else {
    // Not an emulator, but let battery service control backlight
    battery_state_service_subscribe(battery_handler);
    battery_handler(battery_state_service_peek());
  }
}

void deinit() {
  battery_state_service_unsubscribe();
  window_destroy(main_window);  // Destroy main Window
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}