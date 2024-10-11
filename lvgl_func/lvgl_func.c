

#include <stdio.h>
#include "../lvgl/lvgl.h"
#include "include/lvgl_func.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "lv_dropdown.h"
#include "lv_event.h"
#include "lv_label.h"
#include "lv_obj_pos.h"
#include "lv_obj_style.h"
#include "lv_table.h"
#include "lvgl_func.h"
#include "tool.h"


lv_obj_t * base_screen;
lv_obj_t * control_screen;
lv_obj_t * data_table;
lv_obj_t * dd;
lv_obj_t * log_txt_table;
lv_obj_t *temprature_text_label;
lv_obj_t *temprature_text;
lv_obj_t *temprature_set_text;
lv_obj_t *temprature_set_text_label;

void change_set_text(char * text)
{
    lv_textarea_set_text(temprature_text, text);
}

void change_temprature_set_text(char * text)
{
    lv_textarea_set_text(temprature_set_text, text);
}


void lvgl_func(void)
{
    LV_FONT_DECLARE(SourceHanSan);                         // 声明外部字库
    base_screen = lv_obj_create(NULL);
    lv_scr_load(base_screen);
    lv_obj_set_style_pad_all(lv_scr_act(), 0, 0);
    lv_obj_set_style_outline_width(lv_scr_act(), 0, 0);
    lv_obj_set_size(base_screen, LV_PCT(100), LV_PCT(100));
    lv_refr_now(NULL);
    lv_obj_align(base_screen, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(base_screen, 0, 0);
    lv_obj_set_style_outline_width(base_screen, 0, 0);
    lv_obj_set_style_text_font(base_screen, &SourceHanSan, 0);

    // 创建设置温度标签
    temprature_set_text_label = lv_label_create(base_screen);
    lv_label_set_text(temprature_set_text_label, "设置温度");
    lv_obj_align(temprature_set_text_label, LV_ALIGN_CENTER, 0, -200);


    temprature_set_text = lv_textarea_create(base_screen);
    lv_textarea_set_text(temprature_set_text, "27");
    lv_obj_set_width(temprature_set_text, LV_PCT(50)); // 设置文本框宽度为父对象的一半
    lv_obj_align_to(temprature_set_text, temprature_set_text_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 10); // 将文本框对齐到顶部中间，并向下偏移10像素
    
    
    // 创建温度标签
    temprature_text_label = lv_label_create(base_screen);
    lv_label_set_text(temprature_text_label, "当前温度");
    lv_obj_align(temprature_text_label, LV_ALIGN_CENTER, 0, 40);

    temprature_text = lv_textarea_create(base_screen);
    lv_textarea_set_text(temprature_text, "27");
    lv_obj_set_width(temprature_text, LV_PCT(50)); // 设置文本框宽度为父对象的一半
    lv_obj_align_to(temprature_text, temprature_text_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 10); // 将文本框对齐到顶部中间，并向下偏移10像素

}

