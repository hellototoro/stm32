#include "../../MyApplications/Inc/mainpp.hpp"

#include <led/Inc/led.h>
#include <lcd/Inc/tftlcd.h>
#include <print/Inc/print.h>
#include "../../MyApplications/Inc/bsp_camera.hpp"
#include "mcudev_stm32_f4xx_pro/sram/mcudev_stm32_f4xx_pro_sram.h"
#include <touchgfx/hal/OSWrappers.hpp>

//uint16_t FrameBuffer[240 * 240];	// 카메라로 부터 들어오는 이미지 데이터의 프레임버퍼
//uint32_t RemaindImage = (uint32_t)FrameBuffer + (320 * 200) * 2; // RGB565 2Byte이므로 곱하기 2

bsp_camera camera;
led led1(1);
led led2(2);


/******************sram test start******************/
#define BUFFER_SIZE         ((uint32_t)0x0100)
#define WRITE_READ_ADDR     ((uint32_t)0x0800)

uint16_t aTxBuffer[BUFFER_SIZE];
uint16_t aRxBuffer[BUFFER_SIZE];
uint32_t uwIndex = 0;
__IO uint32_t uwWriteReadStatus = 0;

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

static void Fill_Buffer(uint16_t *pBuffer, uint32_t uwBufferLenght, uint16_t uwOffset)
{
  uint16_t tmpIndex = 0;

  /* Put in global buffer different values */
  for (tmpIndex = 0; tmpIndex < uwBufferLenght; tmpIndex++ )
  {
    pBuffer[tmpIndex] = tmpIndex + uwOffset;
  }
} 

static TestStatus Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;
}

void sram_test(void)
{
  Fill_Buffer(aTxBuffer, BUFFER_SIZE, 0xC20F);
  /* Write data to the SRAM memory */
  BSP_SRAM_WriteData(SRAM_DEVICE_ADDR, aTxBuffer, BUFFER_SIZE);
 /* for (uwIndex = 0; uwIndex < BUFFER_SIZE; uwIndex++)
  {
    *(__IO uint16_t*) (SRAM_BANK_ADDR + WRITE_READ_ADDR + 2*uwIndex) = aTxBuffer[uwIndex];
  }    */
  
  /* Read back data from the SRAM memory */
  BSP_SRAM_ReadData(SRAM_DEVICE_ADDR, aRxBuffer, BUFFER_SIZE);
  /*for (uwIndex = 0; uwIndex < BUFFER_SIZE; uwIndex++)
  {
    aRxBuffer[uwIndex] = *(__IO uint16_t*) (SRAM_BANK_ADDR + WRITE_READ_ADDR + 2*uwIndex);
  } */

  /*##-3- Checking data integrity ############################################*/    
  uwWriteReadStatus = Buffercmp(aTxBuffer, aRxBuffer, BUFFER_SIZE);	

  if (uwWriteReadStatus != PASSED)
  {
    /* KO */
    /* Turn on LED2 */
    led1.on();
  }
  else
  { 
    /* OK */
    /* Turn on LED1 */
    led2.on();
  }

}
/******************sram test end******************/


//#define CAMERA_FRAME_BUFFER               0x64000000
//#define CAMERA_I2C_ADDRESS                    0x60
//#define I2C_TIMEOUT  100 /*<! Value of Timeout when I2C communication fails */
//#define BSP_I2C_SPEED                            100000
//#define I2C_DUTYCYCLE_2                 0x00000000U
//#define EVAL_I2Cx                            I2C2

//CAMERA_DrvTypeDef  *camera_drv;
//extern I2C_HandleTypeDef  *heval_I2c;
extern DCMI_HandleTypeDef *DCMI_Handle;

//uint32_t current_resolution;

void led2_toggle(void)
{
    led2.toggle();
}


void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi)
{
    //LCD_SetWindow(0,0,lcd_dev.width,lcd_dev.height);
    //LCD_SetCursor(0,0);
    //LCD_WriteRAM_Prepare();		//开始写入GRAM
    led1.toggle();
}


void setup(void)
{
    //camera.init(CAMERA_R320x240);
    //ov2640_set_rgb565_mode(CAMERA_I2C_ADDRESS);
    //ov2640_set_output_size(CAMERA_I2C_ADDRESS,320,lcd_dev.height);
    //LCD_SetWindow(0,0,lcd_dev.width,lcd_dev.height);
    //LCD_WriteRAM_Prepare();		//开始写入GRAM

    //camera.continuousStart((uint8_t *)&TFT_LCD->RAM);
    //HAL_DCMI_Start_DMA(DCMI_Handle, DCMI_MODE_CONTINUOUS, (uint32_t)(&TFT_LCD->RAM),  1);
    //sram_test();
}

void loop(void)
{
	touchgfx::OSWrappers::signalVSync();
	led1.toggle();
    //if(image_updata)
    //{
    //	image_updata = 0;
    //	LCD_ShowPicture2(0, 0, 400, 240,(uint16_t *)FrameBuffer);
    //	HAL_DCMI_Resume(DCMI_Handle);
    //}
}