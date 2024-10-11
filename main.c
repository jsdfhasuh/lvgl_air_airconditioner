#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include "lvgl/lvgl.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"
#include "lvgl_func.h"
#include "debug.h"
#include "tool.h"
#include "lm75.h"
#include "at24.h"
#include "infreared.h"
#define MIN(a,b) ((a)<(b)?(a):(b))
#define DISP_BUF_SIZE (1024 * 600)
#define EEPROM_PATH "/sys/bus/nvmem/devices/0-00500/nvmem"
#define LM75A_PATH "/dev/lm75a"
#define INFREARED_PATH "/dev/ttymxc5"
#define BYJ48_PATH "/dev/jsdfhasuh_28byj48_0"
#define EEPROM_SIZE 256

int lm75_fd;
int at24_fd;
int infreared_fd;
int byj48_fd;
double before_temperature_buffer = 0.0;
double new_temperature_buffer;
char buffer_str[12];
char infreared_buffer[3];
int set_temperature = 27;
char *temp_ptr;
int byj_status[2]={0};

char eeprom_read_buffer[sizeof(int)] = {'0'};
int i = 0;
int error;
char sub_hex[] = {0x00,0xff,0x07};
char plus_hex[] = {0x00,0xff,0x15};

void renow_data()
{
    i = 0;
    memset(buffer_str,0,sizeof(buffer_str));
    lm75_read(lm75_fd, &new_temperature_buffer);
    if (before_temperature_buffer != new_temperature_buffer)
        {
            sprintf(buffer_str,"%f" ,new_temperature_buffer);
            change_set_text(buffer_str);
        }
    if (set_temperature < new_temperature_buffer)
    {
        //open
        byj_status[0] = 1;
        write(byj48_fd, byj_status, sizeof(byj_status));
        
    }
    else 
    {
        //close
        byj_status[0] = 0 ;
        write(byj48_fd, byj_status, sizeof(byj_status));
    }
}

void infrared_sig_func(int sig)
{
    int result,length;
    printf("begin read infrare data\n");
    infrared_read_raw_data(infreared_buffer, infreared_fd);
    print_raw_data(infreared_buffer);
    print_raw_data(plus_hex);
    length = min(sizeof(infreared_buffer),sizeof(plus_hex));
    result = compare_16(infreared_buffer,plus_hex,length);
    //printf("plus_hex is %d",sizeof(plus_hex));
    //printf("result is %d",result);
    if (!(compare_16(infreared_buffer,plus_hex,length)))
    {
        set_temperature++;
        printf("plus\n");
    }
    if (!(compare_16(infreared_buffer,sub_hex,length)))
    {
        set_temperature--;
        printf("sub \n");
    }
    sprintf(buffer_str,"%d" ,set_temperature);
    change_temprature_set_text(buffer_str);
    temp_ptr = &set_temperature;
    error = write_eeprom(0, sizeof(int), temp_ptr, at24_fd);
}

int main(void)
{
    /*LittlevGL init*/
    lv_init();
    int element;
    char *body;
    /*Linux frame buffer device init*/
    fbdev_init();

    /*A small buffer for LittlevGL to draw the screen's content*/
    static lv_color_t buf[DISP_BUF_SIZE];

    /*Initialize a descriptor for the buffer*/
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);

    /*Initialize and register a display driver*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf   = &disp_buf;
    disp_drv.flush_cb   = fbdev_flush;
    disp_drv.hor_res    = 1024;
    disp_drv.ver_res    = 600;
    lv_disp_drv_register(&disp_drv);

	/* Linux input device init */
    evdev_init();
	
    /* Initialize and register a display input driver */
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);      /*Basic initialization*/

    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = evdev_read;
    lv_indev_t * my_indev = lv_indev_drv_register(&indev_drv); 
    // init all outline divice
    lm75_fd = open(LM75A_PATH, O_RDWR | O_NONBLOCK);
    at24_fd = open(EEPROM_PATH, O_RDWR);
    byj48_fd = open(BYJ48_PATH,O_RDWR| O_NONBLOCK);
    infreared_fd = open_port(INFREARED_PATH, infrared_sig_func);
    error = set_opt(infreared_fd, 9600, 8, 'N', 1);
    //printf("set_temperature is %04x\n set_temperature is %d\n",set_temperature,sizeof(set_temperature));
    error = read_eeprom(0, sizeof(int), eeprom_read_buffer,at24_fd);
    temp_ptr = eeprom_read_buffer;
    set_temperature = *((int*)temp_ptr);
    printf("set_temperature is %d",set_temperature);
    sprintf(buffer_str,"%d" ,set_temperature);
    //print_buffer(eeprom_read_buffer, 4);
        // 创建定时器
    lv_timer_t * timer = lv_timer_create(renow_data, 1000,  NULL);
    /*Create a Demo*/
    lvgl_func();
    change_temprature_set_text(buffer_str);
    /*Handle LitlevGL tasks (tickless mode)*/
    while(1) {
        lv_task_handler();
        usleep(5000);
    }
    close(lm75_fd);
    return 0;
}


/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if(start_ms == 0) {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;
    return time_ms;
}
