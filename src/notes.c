#ifdef These_Are_Just_Notes

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

  LOG("Calculating Pixel Position");
  for (uint8_t i=0; i<90; i++) {
      ptr += snprintf(buffer+ptr, sizeof(buffer)-ptr, "%d, ", (int)(px+(90-pixel_mask[i])));    // Calculate Middle (left middle pixel)
    //ptr += snprintf(buffer+ptr, sizeof(buffer)-ptr, "%d, ", (int)(px+(90-pixel_mask[i])+1));  // Calculate Middle (right middle pixel)
    //ptr += snprintf(buffer+ptr, sizeof(buffer)-ptr, "%d, ", (int)px);                         // Calculate Left Edge
    counter++;
    if(counter==elements_per_line) {
      counter=0;
      ptr=0;
      LOG("%s", buffer);
    }
    px = px + ((90-pixel_mask[i])*2);
  }

  for (uint8_t i=0; i<90; ++i) {
      ptr += snprintf(buffer+ptr, sizeof(buffer)-ptr, "%d, ", (int)(px+(90-pixel_mask[89-i])));   // Calculate Middle (left middle pixel)
    //ptr += snprintf(buffer+ptr, sizeof(buffer)-ptr, "%d, ", (int)(px+(90-pixel_mask[89-i])+1)); // Calculate Middle (right middle pixel)
    //ptr += snprintf(buffer+ptr, sizeof(buffer)-ptr, "%d, ", (int)px);                           // Calculate Left Edge
    counter++;
    if(counter==elements_per_line) {
      counter=0;
      ptr=0;
      LOG("%s", buffer);
    }
    px = px + ((90-pixel_mask[89-i])*2);
  }
  
  if(counter>0)  // display the remainder of elements
    LOG("%s", buffer);

  LOG("Done Calculating");

  
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
int16_t row[] = {  // Offset of Leftmost pixel of row[y].  Note: screen pixel = framebuffer + row[y] + x
     76,   104,   142,   190,   244,   304,   370,   440,   516,   596,
    680,   768,   858,   952,  1050,  1150,  1254,  1360,  1468,  1580, 
   1694,  1810,  1928,  2050,  2174,  2300,  2428,  2558,  2690,  2824, 
   2960,  3096,  3234,  3374,  3516,  3660,  3804,  3950,  4098,  4248, 
   4398,  4550,  4704,  4858,  5014,  5170,  5328,  5488,  5648,  5810, 
   5972,  6136,  6300,  6466,  6632,  6798,  6966,  7134,  7304,  7474, 
   7644,  7816,  7988,  8160,  8334,  8508,  8682,  8858,  9034,  9210, 
   9386,  9562,  9740,  9918, 10096, 10274, 10452, 10632, 10812, 10992, 
  11172, 11352, 11532, 11712, 11892, 12072, 12252, 12432, 12612, 12792, 

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

int16_t middle[] = {  // Middle pixel (left of 2 middle pixels) = row[y]+width[y]
    90,    123,   166,   217,   274,   337,   405,   478,   556,   638, 
    724,   813,   905,  1001,  1100,  1202,  1307,  1414,  1524,  1637, 
   1752,  1869,  1989,  2112,  2237,  2364,  2493,  2624,  2757,  2892, 
   3028,  3165,  3304,  3445,  3588,  3732,  3877,  4024,  4173,  4323, 
   4474,  4627,  4781,  4936,  5092,  5249,  5408,  5568,  5729,  5891, 
   6054,  6218,  6383,  6549,  6715,  6882,  7050,  7219,  7389,  7559, 
   7730,  7902,  8074,  8247,  8421,  8595,  8770,  8946,  9122,  9298, 
   9474,  9651,  9829, 10007, 10185, 10363, 10542, 10722, 10902, 11082, 
  11262, 11442, 11622, 11802, 11982, 12162, 12342, 12522, 12702, 12882, 

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


int16_t middle_right[] = {  // Middle pixel (right of 2 middle pixels) = row[y]+width[y]+1
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

int16_t row_half_width[] = {  // Half of width of row[y].  Double for row width.  Also works for height of column[x].
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


void fill_screen(GContext *ctx, uint8_t color) {
  uint8_t *framebuffer = (uint8_t*)*(uintptr_t*)ctx;  // First pixel on PTR doesn't start til framebuffer + 76 (76 = pixel_mask[0])
  memset(framebuffer, color, IF_APLITE_BASALT_CHALK_ELSE(144*168/8, 144*168, 76 + 25792, 0));
}

// ===============================================================================================================================================
//  Drawing Functions
// ===============================================================================================================================================
void main_layer_update(Layer *me, GContext *ctx) {
  
  fill_screen(ctx, 0b11000011);
  
  //uint8_t *framebuffer = (uint8_t*)*(size_t*)ctx;
  uint8_t *framebuffer = (uint8_t*)*(uintptr_t*)ctx;
  
  // Works to fill circle except left 5 pixels and right 3 pixels
  for (uint8_t y=0; y<180; ++y)
    for (int16_t p = middle[y] - row_half_width[y] + 5; p < middle[y] + row_half_width[y] - 3; ++p)
      framebuffer[p] = 0b11001100;
    
  // Works to fill circle except left 5 pixels and right 3 pixels
  for (uint8_t y=0; y<180; ++y)
    for (int16_t x = 5; x < row_half_width[y]*2-3; ++x)
      framebuffer[row[y]+x] = 0b11001100;

    
  fill_screen(ctx, 0b11111111);
    
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
  
  float average = 2.5;
  APP_LOG(APP_LOG_LEVEL_INFO, "Average of these values is %f\n", average);
  
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

#endif