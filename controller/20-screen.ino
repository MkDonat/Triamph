/*//screen libs
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
//SPI setup pins
U8G2_SSD1309_128X64_NONAME2_1_4W_SW_SPI u8g2(
  U8G2_R0
  ,  
  18 // SLAVE CLOCK (SCK) or (SCL)
  ,
  23 // SLAVE DATA (SDA)
  ,
  5 // CHEAP SELECT (CS)
  ,  
  2 // REGISTER SELECT (DC)
  ,
  4 // RESET (RES)
);
//Lopaka variables
static const unsigned char image_controller_icon_bits[] = {0x20,0x00,0xfe,0x03,0xfb,0x07,0x71,0x05,0xfb,0x07,0x8f,0x07,0x07,0x07,0x03,0x06};
static const unsigned char image_sending_data_arrow_bits[] = {0x04,0x02,0x7f,0x02,0x04};
static const unsigned char image_boat_icon_bits[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x01,0x00,0x00,0x03,0x00,0x00,0x07,0x00,0x00,0x0f,0x00,0x00,0x01,0x00,0x00,0x01,0x00,0xfe,0xff,0x00,0xfe,0xff,0x00,0xfc,0x7f,0x00,0xf8,0x3f,0x00,0xc0,0x07,0x00};
static const unsigned char image_icoming_data_arrow_bits[] = {0x10,0x20,0x7f,0x20,0x10};

//Global variables

void vTaskScreenUpdate(void *arg){
  u8g2.begin();
  for(;;){
    u8g2.firstPage();
    do{
      u8g2.clearBuffer();
      u8g2.setFontMode(1);
      u8g2.setBitmapMode(1);

      // speed value
      char buffer[32];
      sprintf(
        buffer,
        "%d",
        receivedData.tofSensorData_singleMillimetersValue
      );
      u8g2.setFont(u8g2_font_6x13_tr);
      u8g2.drawStr(60, 30, buffer);

      // triamph battery level
      u8g2.setFont(u8g2_font_6x10_tr);
      u8g2.drawStr(12, 13, "100");

      // circle draw for cap
      u8g2.drawEllipse(23, 40, 17, 16);

      // label % triamph
      u8g2.drawStr(30, 13, "%");

      // controller icon
      u8g2.drawXBM(76, 5, 11, 8, image_controller_icon_bits);

      // label - cap
      u8g2.drawStr(15, 51, "cap");

      // sending data arrow
      if(sending_data){
        u8g2.drawXBM(62, 3, 7, 5, image_sending_data_arrow_bits);
      }

      // boat icon
      u8g2.drawXBM(39, -1, 17, 14, image_boat_icon_bits);

      // label % controller
      u8g2.drawStr(109, 13, "%");

      // degree value
      u8g2.setFont(u8g2_font_6x13_tr);
      u8g2.drawStr(15, 40, "360");

      // icoming data arrow
      if(receiving_data){
        u8g2.drawXBM(62, 8, 7, 5, image_icoming_data_arrow_bits);
      }

      // controller battery level
      u8g2.setFont(u8g2_font_6x10_tr);
      u8g2.drawStr(91, 13, "100");

      // degree icon
      u8g2.drawEllipse(33, 31, 1, 1);

      // horizontal bar
      u8g2.drawLine(13, 43, 33, 43);

      // label - mps
      u8g2.setFont(u8g2_font_6x13_tr);
      u8g2.drawStr(58, 38, "mps");

      // gaz rect
      u8g2.drawFrame(116, 24, 6, 33);

      // label - drive mode
      u8g2.setFont(u8g2_font_5x8_tr);
      u8g2.drawStr(117, 23, "P");

      // label - collecting
      u8g2.setFont(u8g2_font_4x6_tr);
      u8g2.drawStr(47, 50, "collecting");

      u8g2.sendBuffer();
    }
    while(u8g2.nextPage());
    //vTaskDelay(pdMS_TO_TICKS(10));
  }
}
void CreateTasksForScreen(){
  xTaskCreatePinnedToCore(
    vTaskScreenUpdate,"screen refresh"
    ,
     2048
    ,
    NULL // Stack Depth
    ,
    1 //Priority
    ,
    NULL //Task handle
    ,
    CORE_2 // Core on which the task will run
  );
}*/