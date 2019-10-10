#include "lvgl/lvgl.h"

#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMG_DISP_MOT_ON_IMG
#define LV_ATTRIBUTE_IMG_DISP_MOT_ON_IMG
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_IMG_DISP_MOT_ON_IMG uint8_t DISP_Mot_On_img_map[] = {
  0x04, 0x04, 0x04, 0xff, 	/*Color of index 0*/
  0x6f, 0x70, 0x6f, 0xff, 	/*Color of index 1*/
  0x3f, 0x3d, 0x3f, 0xff, 	/*Color of index 2*/
  0xad, 0xae, 0xad, 0xff, 	/*Color of index 3*/
  0xc7, 0xc5, 0xc7, 0xff, 	/*Color of index 4*/
  0x1d, 0x1f, 0x1d, 0xff, 	/*Color of index 5*/
  0x59, 0x59, 0x59, 0xff, 	/*Color of index 6*/
  0x94, 0x92, 0x94, 0xff, 	/*Color of index 7*/
  0xb9, 0xba, 0xb9, 0xff, 	/*Color of index 8*/
  0x81, 0x83, 0x81, 0xff, 	/*Color of index 9*/
  0x30, 0x2f, 0x30, 0xff, 	/*Color of index 10*/
  0x68, 0x66, 0x68, 0xff, 	/*Color of index 11*/
  0x0f, 0x10, 0x0f, 0xff, 	/*Color of index 12*/
  0x48, 0x4a, 0x48, 0xff, 	/*Color of index 13*/
  0xcc, 0xce, 0xcc, 0xff, 	/*Color of index 14*/
  0x9d, 0x9f, 0x9d, 0xff, 	/*Color of index 15*/

  0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xe0, 
  0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xe0, 
  0xee, 0xee, 0xe4, 0xd5, 0x23, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xe0, 
  0xee, 0xee, 0xe2, 0xd3, 0x1c, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xe0, 
  0xee, 0xee, 0x80, 0x3e, 0xec, 0x7e, 0xb3, 0x66, 0x3e, 0xee, 0xee, 0xee, 0xe0, 
  0xee, 0xee, 0xf0, 0xee, 0xea, 0x1e, 0x5a, 0x96, 0xae, 0xee, 0xee, 0xee, 0xe0, 
  0xee, 0xee, 0x7c, 0xee, 0xe2, 0xbe, 0x51, 0xe4, 0x04, 0xee, 0xee, 0xee, 0xe0, 
  0xee, 0xee, 0xf0, 0xee, 0xea, 0x1e, 0x5f, 0xee, 0xc3, 0xee, 0xee, 0xee, 0xe0, 
  0xee, 0xee, 0x30, 0x8e, 0xe5, 0x9e, 0x5f, 0xee, 0xc3, 0xee, 0xee, 0xee, 0xe0, 
  0xee, 0xee, 0xe5, 0x1e, 0xf0, 0x4e, 0x5f, 0xee, 0xc3, 0xee, 0xee, 0xee, 0xe0, 
  0xee, 0xee, 0xef, 0xc5, 0xc9, 0xee, 0x5f, 0xee, 0xc3, 0xee, 0xee, 0xee, 0xe0, 
  0xee, 0xee, 0xee, 0xe8, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xe0, 
  0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xe0, 
  0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xe0, 
};

const lv_img_dsc_t DISP_Mot_On_img = {
  .header.always_zero = 0,
  .header.w = 25,
  .header.h = 14,
  .data_size = 246,
  .header.cf = LV_IMG_CF_INDEXED_4BIT,
  .data = DISP_Mot_On_img_map,
};
