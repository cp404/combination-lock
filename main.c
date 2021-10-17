#include <reg52.h>					//52头文件
#include "delay.h"

sbit led1=P0^1;   //开关指示灯
sbit led2=P0^2;   //按键指示灯
sbit led3=P0^3;   //成功指示灯

sbit PWM=P1^2;  	//对应舵机的PWM引脚

sbit key1=P3^3;   //按键引脚
sbit key2=P1^4;

unsigned int i;
int count=0;		//全局变量count用于定时器计数

int password_in[7]={0,0,0,0,0,0,0};  //储存输入的密码
int password[7]={1,2,2,2,1,2,2};     //设定的密码数组

void Timer_Init()
{
  TMOD=0X01;      //T0定时方式1
  TH0=0Xfe;
  TL0=0X33;       //计数初值设置为0.5ms
  ET0=1;          //打开定时器0的中断
  TR0=1;          //打开定时器0
  EA=1;           //开总中断
}

void Timer() interrupt 1     
{
  TH0=0Xfe;
  TL0=0X33;       //重新赋计数初值为0.5ms
  count++;
  if(count>=40)		//计数40次，每次0.5ms，总共达到20ms周期后清零，为下一周期开始计数做准备
  {
    count=0;
  }
}

void pwm_out(int x)  //pwm输出函数
{
  if(count<=x)	//进入定时器的时间和高低电平临界值比较
  {
    PWM=1;		//输出PWM波形中的高电平
  }
  else
  {
    PWM=0;    //输出PWM波形中的低电平
  }
}

void main()
{
	P0=0xfe;//开启指示灯亮起
	Timer_Init();
	while(1)
	{
		if(key1==0)
		{
			delay_ms(4);//按键消抖，下面同理
			if(key1==0)
			{
				led1 =0;//按键指示灯亮起
				for(i=0;i<7;i++)//判断密码数组前面的位是否已填上
				{
					if(password_in[i]==0)
					{
						password_in[i]=1;
						i=0;
						break;
					}
				}
				while(!key1);//长按无法结束此次输入
				led1=1;//按键指示灯熄灭
			}
		}
		if(key2==0)
		{
			delay_ms(4);
			if(key2==0)
			{
				led1 =0;
				for(i=0;i<7;i++)
				{
					if(password_in[i]==0)
					{
						password_in[i]=2;
						i=0;
						break;
					}
				}
				while(!key2);//cannot_out2
				led1=1;
			}
		}
		for(i=0;password_in[i]==password[i];i++)//判断密码是否输入正确
		{
			if(i==6)//正确
			{
				led2=0;//开锁成功指示灯亮起
				pwm_out(1);//输出pwm波转动舵机带动门锁完成开锁
			}
		}
	}
}
