#include<avr/io.h>

#define SDA_0 PORTF_OUT&=0xfe//PORTF_PIN0CTRL = 0//
#define SDA_1 PORTF_OUT|=0x01//PORTF_PIN0CTRL = 1//
#define SCL_0 PORTF_OUT&=0xfd//PORTF_PIN1CTRL = 0//
#define SCL_1 PORTF_OUT|=0x02//PORTF_PIN1CTRL = 1//

uint16_t Read_Reg(int reg_no);

//void Delay_us(unsigned int number)
//{
	//number *= 4;
	//for(;number>0;number--);
//}

void iic_start()
{
	SDA_1;
	delay_us(4);
	SCL_1;
	delay_us(4);
	SDA_0;
	delay_us(4);
	SCL_0;
	delay_us(4);
	//UsartF0_PutString("START OK \r\n"); 
}

void iic_stop()
{
	SCL_0;
	delay_us(4);
	SDA_0;
	delay_us(4);
	SCL_1;
	delay_us(4);
	SDA_1;
	delay_us(4);
}

void iic_ack()
{
	SDA_0;
	delay_us(4);
	SCL_1;
	delay_us(4);
	SCL_0;
	delay_us(4);
	SDA_1;
	delay_us(4);
}

void iic_NoAck()
{
	SDA_1;
	delay_us(4);
	SCL_1;
	delay_us(4);
	SCL_0;
	delay_us(4);
	SDA_0;
}

unsigned char iic_testack()
{
	unsigned char Error;
	SDA_1;
	delay_us(4);
	PORTF_DIR &= 0xfe;
	SCL_1;
	delay_us(4);
	Error = PORTF_IN & 0x01;
	delay_us(4);
	SCL_0;
	delay_us(4);//
	PORTF_DIR |= 0x01;
	//UsartF0_PutChar( Error%10 + 0x30);        //千位
	//UsartF0_PutChar(0x0D);
	//UsartF0_PutChar(0x0A);  //结尾发送回车换行
	return Error;
}

void iic_write8bit(unsigned char input)
{
	unsigned char temp;
	for(temp=8;temp!=0;temp--)
	{
		if(input&0x80) SDA_1;
		else  SDA_0;
		delay_us(4);
		SCL_1;
		delay_us(4);
		SCL_0;
		delay_us(4);
		input=input<<1;
	}
}

unsigned char iic_read8bit()
{
	unsigned char temp,rbyte=0;
	PORTF_DIR &= 0xfe;
	delay_us(4);//
	for(temp=8;temp!=0;temp--)
	{
		SCL_1;
		delay_us(4);
		if(PORTF_IN&0x01)
		{
			rbyte = rbyte << 1;
			rbyte += 1;
		}
		else  rbyte = rbyte << 1;
		SCL_0;
		delay_us(4);//
	}
	PORTF_DIR |= 0x01;
	return rbyte;
}

uint16_t Read_Reg(int reg_no)
{
	//UsartF0_PutString("IIC DEMO \r\n");
	uint16_t temper;
	int temperh=0,temperl=0,tempdata;
	iic_start();
	iic_write8bit(0x12);       //写模式
	if(!iic_testack())
	{
		//UsartF0_PutString("IIC Write TestAck1 OK\r\n");
		iic_write8bit(reg_no);   //写地址
		if(!iic_testack())
		{
			//UsartF0_PutString("IIC Write TestAck2 OK\r\n");
			//iic_write8bit(0x39);              //写数据
			//if(!iic_testack())
			//{
			////UsartF0_PutString("IIC Write TestAck3 OK\r\n");
			//
			//}
			//else UsartF0_PutString("TestAck3 Failed \r\n");
		}
		else UsartF0_PutString("TestAck2 Failed \r\n");
	}
	else UsartF0_PutString("TestAck1 Failed \r\n");
	iic_stop();
	_delay_ms(100);
	
	//UsartF0_PutString("IIC Read Start \r\n");
	iic_start();
	iic_write8bit(0x13);
	if(!iic_testack())
	{
		//UsartF0_PutString("IIC Read TestAck1 \r\n");

		temper = iic_read8bit();
		UsartF0_PutString("REG");
		UsartF0_PutChar(reg_no + 0x30);
		UsartF0_PutString("=");
		tempdata = temper % 1000 / 100 + 0x30;
		temperh = temper % 100 / 10 + 0x30 ;
		temperl = temper % 10 + 0x30;
		if (tempdata > 48  )
		{
			UsartF0_PutChar( tempdata );   //百位
		}
		else ;
		UsartF0_PutChar( temperh );   //十位
		UsartF0_PutChar( temperl );          //个位
		
		UsartF0_PutString("\r\n");
		//
		//}
	}
	else UsartF0_PutString("IIC Read TestAck1 Failed \r\n");
	iic_NoAck();
	iic_stop();
	//temperh = (temperh<<8) | temperl;
	//temperl >>= 5;
	return temper;
}

void Write_Reg(char reg_add,char reg_data)
{
	iic_start();
	iic_write8bit(0x12);       //写模式
	if(!iic_testack())
	{
		//UsartF0_PutString("IIC Write TestAck1 OK\r\n");
		iic_write8bit(reg_add);   //写地址
		if(!iic_testack())
		{
			//UsartF0_PutString("IIC Write TestAck2 OK\r\n");
			iic_write8bit(reg_data);              //写数据
			if(!iic_testack())
			{
			//UsartF0_PutString("IIC Write TestAck3 OK\r\n");
			
			}
			//else UsartF0_PutString("TestAck3 Failed \r\n");
		}
	}
}

