/*
 * Tc4155.c
 *
 * Created: 2015/11/16 8:44:03
 *  Author: lib
 */ 
#include "Tc4155.h"

void Charger_Init(void)
{
	int i ,reg_buffer[9],temD;
	int NTCOK = 0;
	for (i = 0;i < 7;i++ )
	{
		reg_buffer[i]=Read_Reg(i);
	}
	if (reg_buffer[4] > 63) // USB电源可用  reg4
	{		
		UsartF0_PutString("UsbPower Ok! \r\n");
		Write_Reg(0x00,0x80); //Clear Low battery 0x80:Clear Low batter Current Limit
		Write_Reg(0x01,00); //clear safty timer USB 优先
		//Write_Reg(0x06,0xec); //clear safty timer USB 优先
		if (reg_buffer[5] & 0x01) // 判断NTC
		{
			UsartF0_PutChar( (reg_buffer[5] & 0x01) + 0x30 );          //个位
			NTCOK = 0;
			temD = reg_buffer[5] >> 1;
			if (temD > 102) UsartF0_PutString("NTC is too cold! \r\n");
			else if(19 < temD < 41 ) UsartF0_PutString("NTC is too Warm! \r\n");
			else if (temD < 19) UsartF0_PutString("NTC HOT FAULT! \r\n");
			else 
			{
				UsartF0_PutChar( temD / 1000 + 0x30 );        //千位
				UsartF0_PutChar( temD % 1000 / 100 + 0x30 );  //百位
				UsartF0_PutChar( temD % 100 / 10  + 0x30 );   //十位
				UsartF0_PutChar( temD % 10 + 0x30 );          //个位
			};	
		}
		else 
		{
			NTCOK = 1;	
			UsartF0_PutString("NTC is Ok! \r\n");
		}
		if(NTCOK & (reg_buffer[3] & 0x01)) //判断电池电压
		{
			
			UsartF0_PutString("Low Battery Detection! \r\n");
		}
		else
		{
			//UsartF0_PutChar( (reg_buffer[3] & 0x01) + 0x30 );          //USB状态下无法检测是否有电池
			//UsartF0_PutString("VBAT is above 2.8V! \r\n");
			//reg_buffer[0] |
		}
	}
	else UsartF0_PutString("NO External Power! \r\n");
}
