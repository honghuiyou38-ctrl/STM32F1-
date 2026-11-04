#include "oled.h"
#include "oledfont.h"  	 
#include "i2c.h"

#define OLED_DATA_CONTROL  0x40U

 #define OLED_ADDR   0x78    // I2C OLED设备地址，0x7A/0x78二选一

//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   
void delay_ms(unsigned int ms)
{
	/* HAL tick remains accurate when the system clock changes.  This function
	 * is also used by the MPU6050 Motion Driver during reset and DMP upload. */
	HAL_Delay(ms);
}


/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
//	IIC_Start();
//	IIC_Send_Byte(0x78);      //Slave address,SA0=0
//	IIC_Recv_Ack();
//	IIC_Send_Byte(0x00);			//write command
//	IIC_Recv_Ack();
//	IIC_Send_Byte(IIC_Command);
//	IIC_Recv_Ack();
//	IIC_Stop();
	uint8_t IIC_Send_cmd[]={0x00,IIC_Command};
	HAL_I2C_Master_Transmit(&hi2c1, 0x78, IIC_Send_cmd, 2, 100);
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
//	IIC_Start();
//	IIC_Send_Byte(0x78);			//D/C#=0; R/W#=0
//	IIC_Recv_Ack();
//	IIC_Send_Byte(0x40);			//write data
//	IIC_Recv_Ack();
//	IIC_Send_Byte(IIC_Data);
//	IIC_Recv_Ack();
//	IIC_Stop();
	uint8_t IIC_Send_data[]={0x40,IIC_Data};
	HAL_I2C_Master_Transmit(&hi2c1, 0x78, IIC_Send_data, 2, 100);
}

void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
	{
    Write_IIC_Data(dat);  
	}
	else
	{
    Write_IIC_Command(dat);		
	}
}

/********************************************
// fill_Picture
********************************************/
void fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WR_Byte(0xb0+m,0);		//page0-page1
		OLED_WR_Byte(0x00,0);		//low column start address
		OLED_WR_Byte(0x10,0);		//high column start address
		for(n=0;n<128;n++)
		{
			OLED_WR_Byte(fill_Data,1);
		}
	}
}


/***********************Delay****************************************/
void Delay_50ms(unsigned int Del_50ms)
{
	unsigned int m;
	for(;Del_50ms>0;Del_50ms--)
		for(m=6245;m>0;m--);
}

void Delay_1ms(unsigned int Del_1ms)
{
	unsigned char j;
	while(Del_1ms--)
	{	
		for(j=0;j<123;j++);
	}
}

//��������
	void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}   	  
//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)
		  OLED_WR_Byte(0,OLED_DATA); 
	} //������ʾ
}
void OLED_On(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)
		 OLED_WR_Byte(1,OLED_DATA); 
	} //������ʾ
}
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	unsigned char c=0,i=0;	
	c=chr-' ';//�õ�ƫ�ƺ��ֵ			
	if(x>Max_Column-1)
	{
		x=0;
		y=y+2;
	}
	if(Char_Size ==16)
	{
		OLED_Set_Pos(x,y);	
		for(i=0;i<8;i++)
		OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
		OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
	}
	else 
	{	
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
		OLED_WR_Byte(F6x8[c][i],OLED_DATA);			
	}
}
//m^n����
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)
	{
	  result*=m;
	}    
	return result;
}				  
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}
			else 
				enshow=1; 		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 
//��ʾһ���ַ��Ŵ�
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
	uint8_t page_buf[128];
	uint8_t lower_buf[128];

	while ((*chr != '\0') && (y < 8U))
	{
		uint8_t max_chars = (uint8_t)((128U - x) / 8U);
		uint8_t count = 0U;
		uint8_t index;
		uint16_t length;

		while ((chr[count] != '\0') && (count < max_chars))
		{
			++count;
		}
		if (count == 0U)
		{
			x = 0U;
			y = (uint8_t)(y + ((Char_Size == 16U) ? 2U : 1U));
			continue;
		}

		length = (uint16_t)count * 8U;
		for (index = 0U; index < count; ++index)
		{
			uint8_t character = chr[index];
			uint8_t column;

			if ((character < ' ') || (character > '~'))
			{
				character = '?';
			}
			character = (uint8_t)(character - ' ');

			if (Char_Size == 16U)
			{
				for (column = 0U; column < 8U; ++column)
				{
					page_buf[(uint16_t)index * 8U + column] =
						F8X16[(uint16_t)character * 16U + column];
					lower_buf[(uint16_t)index * 8U + column] =
						F8X16[(uint16_t)character * 16U + column + 8U];
				}
			}
			else
			{
				for (column = 0U; column < 6U; ++column)
				{
					page_buf[(uint16_t)index * 8U + column] = F6x8[character][column];
				}
				page_buf[(uint16_t)index * 8U + 6U] = 0U;
				page_buf[(uint16_t)index * 8U + 7U] = 0U;
			}
		}

		OLED_Set_Pos(x, y);
		HAL_I2C_Mem_Write(&hi2c1, OLED_ADDR, OLED_DATA_CONTROL,
		                  I2C_MEMADD_SIZE_8BIT, page_buf, length, 100U);
		if ((Char_Size == 16U) && (y < 7U))
		{
			OLED_Set_Pos(x, (uint8_t)(y + 1U));
			HAL_I2C_Mem_Write(&hi2c1, OLED_ADDR, OLED_DATA_CONTROL,
			                  I2C_MEMADD_SIZE_8BIT, lower_buf, length, 100U);
		}

		chr += count;
		x = 0U;
		y = (uint8_t)(y + ((Char_Size == 16U) ? 2U : 1U));
	}
}
//��ʾ����
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
	for(t=0;t<16;t++)
	{
		OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
		adder+=1;
	}	
	OLED_Set_Pos(x,y+1);	
	for(t=0;t<16;t++)
	{	
		OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
		adder+=1;
	}					
}
/****************************/
const uint8_t ReverseTable[256]=
{
0x00,0x80,0x40,0xC0,0x20,0xA0,0x60,0xE0,
0x10,0x90,0x50,0xD0,0x30,0xB0,0x70,0xF0,
0x08,0x88,0x48,0xC8,0x28,0xA8,0x68,0xE8,
0x18,0x98,0x58,0xD8,0x38,0xB8,0x78,0xF8,
0x04,0x84,0x44,0xC4,0x24,0xA4,0x64,0xE4,
0x14,0x94,0x54,0xD4,0x34,0xB4,0x74,0xF4,
0x0C,0x8C,0x4C,0xCC,0x2C,0xAC,0x6C,0xEC,
0x1C,0x9C,0x5C,0xDC,0x3C,0xBC,0x7C,0xFC,
0x02,0x82,0x42,0xC2,0x22,0xA2,0x62,0xE2,
0x12,0x92,0x52,0xD2,0x32,0xB2,0x72,0xF2,
0x0A,0x8A,0x4A,0xCA,0x2A,0xAA,0x6A,0xEA,
0x1A,0x9A,0x5A,0xDA,0x3A,0xBA,0x7A,0xFA,
0x06,0x86,0x46,0xC6,0x26,0xA6,0x66,0xE6,
0x16,0x96,0x56,0xD6,0x36,0xB6,0x76,0xF6,
0x0E,0x8E,0x4E,0xCE,0x2E,0xAE,0x6E,0xEE,
0x1E,0x9E,0x5E,0xDE,0x3E,0xBE,0x7E,0xFE,
0x01,0x81,0x41,0xC1,0x21,0xA1,0x61,0xE1,
0x11,0x91,0x51,0xD1,0x31,0xB1,0x71,0xF1,
0x09,0x89,0x49,0xC9,0x29,0xA9,0x69,0xE9,
0x19,0x99,0x59,0xD9,0x39,0xB9,0x79,0xF9,
0x05,0x85,0x45,0xC5,0x25,0xA5,0x65,0xE5,
0x15,0x95,0x55,0xD5,0x35,0xB5,0x75,0xF5,
0x0D,0x8D,0x4D,0xCD,0x2D,0xAD,0x6D,0xED,
0x1D,0x9D,0x5D,0xDD,0x3D,0xBD,0x7D,0xFD,
0x03,0x83,0x43,0xC3,0x23,0xA3,0x63,0xE3,
0x13,0x93,0x53,0xD3,0x33,0xB3,0x73,0xF3,
0x0B,0x8B,0x4B,0xCB,0x2B,0xAB,0x6B,0xEB,
0x1B,0x9B,0x5B,0xDB,0x3B,0xBB,0x7B,0xFB,
0x07,0x87,0x47,0xC7,0x27,0xA7,0x67,0xE7,
0x17,0x97,0x57,0xD7,0x37,0xB7,0x77,0xF7,
0x0F,0x8F,0x4F,0xCF,0x2F,0xAF,0x6F,0xEF,
0x1F,0x9F,0x5F,0xDF,0x3F,0xBF,0x7F,0xFF
};

#define OLED_ReverseByte(x) ReverseTable[(x)]

void OLED_DrawBMP(unsigned char x0,
                  unsigned char y0,
                  unsigned char x1,
                  unsigned char y1,
                  unsigned char BMP[])
{
    uint8_t page;
    uint8_t x;
    uint8_t w=x1-x0;

    static uint8_t line_buf[128];

    unsigned int index=0;

    uint8_t start_page=y0>>3;
    uint8_t end_page=(y1-1)>>3;

    for(page=start_page;page<=end_page;page++)
    {
        OLED_Set_Pos(x0,page);

        for(x=0;x<w;x++)
        {
            line_buf[x]=ReverseTable[BMP[index++]];
        }

        HAL_I2C_Mem_Write(&hi2c1,
                          OLED_ADDR,
                          OLED_DATA_CONTROL,
                          I2C_MEMADD_SIZE_8BIT,
                          line_buf,
                          w,
                          100);
    }
}

//��ʼ��SSD1306					    
void OLED_Init(void)
{ 	
 
	OLED_WR_Byte(0xAE,OLED_CMD);//--display off
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD); // contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--128   
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
}  






























