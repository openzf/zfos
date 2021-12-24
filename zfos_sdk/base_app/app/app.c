#include "app.h"
#include "zfos.h"

#include "led.h"
#include "key.h"
#include "zf_touch.h"
#include "zf_lcd.h"



char wr_buf[100];
char rd_buf[100];


FRESULT fr;
FIL fd;
char filename[] = "1:test.txt";
uint8_t write_dat[] = "Hello,FATFS!\n";
uint16_t write_num = 0;
unsigned int count = 0;
unsigned char read_buf[50] = {0};
static void fs_test(char *filename_t)
{
  int size = 0;
  int fd = open(filename_t, O_CREAT | O_WRONLY);
  if (fd > 0)
  {
    printf("open %s ok\r\n", filename_t);
    sprintf(wr_buf, "test data:%s write123", filename_t);
    size = write(fd, wr_buf, strlen(wr_buf));
    printf("write %s\r\n", wr_buf);
    close(fd);
  }
  fd = open(filename_t, O_EXCL | O_RDONLY);
	memset(rd_buf,50,0);
  size = read(fd, rd_buf, 50);
  printf("read %d: %s\r\n\r\n", size, rd_buf);
  printf("------------\r\n");
  close(fd);
}



void app_init(void)
{

  //touch_init(480, 800, 1);
	
	// 如果不能运行那么就是需要格式化
	fs_test("0:test.txt");
	fs_test("1:test.txt");
	fs_test("2:test.txt");
	fs_test("3:test.txt");


	printf("lcd run\r\n");
	//touch_init(800,480,0);
	
  uint16_t x=0;
	uint8_t mdata[20];
	uint16_t x1 = 0;
	uint16_t y1=0;
  while (1)
  {

		delay_ms(1000);
		int keys =  key_read(0);
		//printf("run: %d\r\n",keys);
		  if (uart_read(0,mdata,1) == 0)
			{
				uart_printf(0,"hello :%c\r\n",mdata[0]);
			}

    led_switch(x);
  //  delay_ms(100);
    led_switch(x);
		
		//touch_get(&x1, &y1);
			// printf("touch %d %d\r\n",x1, y1);
		if(x==0)
		{
			x = 1;
		}else{
			x = 0;
		}
  // touch_get(&x, &y);
		//int x234 = ina219_get_ma();
	//	printf("ma :%d\r\n",x234);
    //lcd_draw_point(x1, y1, RED);
  }
}
