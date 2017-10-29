#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "HCTL2020.h"
#include "timer.h"
#include "lcd.h"
#include "control.h"
#include "pwm.h"
#include "led.h"
//#include "key.h"
#include "beep.h"
#include "remote.h"
#include "exti.h"

u8 zhongquan_case;





int main(void)
{
	u8 key = 0;					//����ֵ
	u8 chengxu = 0;				//����ѡ��
	u8 flag=0;
	u8 qiu = 0;				//����
	
//	u8 sanfen_case=0;
	int16_t time = 0;			//��ʱ
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����ϵͳ�ж����ȼ�����2   2λ��ռ���� 2λ��Ӧ����
	zhongquan_case=0;  		
	delay_init(168);  			//��ʼ����ʱ����
	uart_init(9600);	 		//���ڳ�ʼ��Ϊ9600
	initall_PWM();				//��ʼ��PWM������
 	LED_Init();			    	 //LED�˿ڳ�ʼ��
	LCD_Init();					//LCD��ʼ��
	LCD_Show_Title();			//Һ������ʾ���ݳ�ʼ��
	//KEY_Init();					//������ʼ��
	hctl2020_init();			//��ʼ��������
	Remote_Init();				//����ң�س�ʼ��
	charge_init();				//�����翪�س�ʼ��
	shot_init();				//�����ʼ��
	xianwei_init();				//��λ���س�ʼ��
	BEEP_Init();
	hongwai_init();				//���⿪�س�ʼ��
	//EXTIX_Init();

	//TIM_SetCompare2(TIM5,280);	//��HCTL2020�ṩʱ���źţ�PA0
	TIM_SetCompare2(TIM5,7);
	control_init();				//�����˳�ʼ��
	TIM2_Int_Init(100-1,8400-1);//��ʱ��ȡ��������ʱ��0.01f
	//TIM7_Int_Init(20000,8400*5-1); // ѹ��Ͷ�� ������ 120s��ʱ
	EXTIX_Init();	
	

	while(1)
	{
		key = Remote_Scan();
		flag = 0;
		key = 0;
		chengxu = 0;
		//ѡ�����
		while(1)
		{
			LCD_ShowString(30+200,400,200,16,16,"chengxu:");
			key = Remote_Scan();
			//key = KEY_Scan(0);
			switch(key)
			{
				case 0:		//û�а�������
					
					break;
				case KEY_RIGHT:		//��
					LCD_ShowString(30+200,400,200,16,16,"qiu:    ");
					flag = 1;
					break;
				case KEY_DOWN:		//��
					LCD_ShowString(30+200,400,200,16,16,"chengxu-");
					if(chengxu != 0)
						chengxu--;
					break;
				case KEY_LEFT:		//��
					LCD_ShowString(30+200,400,200,16,16,"clear   ");
					chengxu = 0;
					break;
				case KEY_UP:		//��
					LCD_ShowString(30+200,400,200,16,16,"chengxu+");
					chengxu++;
					break;
			}
			
			LCD_ShowNum(30+200+48+8+10,320,chengxu,4,16);
			
			if(flag)
				break;
		}
		
		flag = 0;
		key = 0;
		
		//ѡ����
		while(1)
		{
			key = Remote_Scan();
			//key = KEY_Scan(0);
			switch(key)
			{
				case 0:		//û�а�������
					
					break;
				case KEY_RIGHT:		//��
					LCD_ShowString(30+200,400,200,16,16,"start   ");
					flag = 1;
					break;
				case KEY_DOWN:		//��
					LCD_ShowString(30+200,400,200,16,16,"qiu-");
					if(qiu != 0)
						qiu--;
					break;
				case KEY_LEFT:		//��
					LCD_ShowString(30+200,400,200,16,16,"clear   ");
					qiu = 0;
					break;
				case KEY_UP:		//��
					LCD_ShowString(30+200,400,200,16,16,"qiu+");
					qiu++;
					break;

			}
			
			LCD_ShowNum(30+200+48+8+10,340,qiu,4,16);
			
			if(flag)
				break;
		}
		/*
		flag = 0;
		key = 0;
				while(1)
		{
			key = Remote_Scan();
			switch(key)
			{
				case 0:		//û�а�������
					
					break;
				case KEY_RIGHT:		//��
					LCD_ShowString(30+200,400,200,16,16,"sanfen   ");
					flag = 1;
					break;
				case KEY_DOWN:		//��
					LCD_ShowString(30+200,400,200,16,16,"zhongquan-");
					if(zhongquan_case != 0)
						zhongquan_case--;
					break;
				case KEY_LEFT:		//��
					LCD_ShowString(30+200,400,200,16,16,"clear   ");
					zhongquan_case = 0;
					break;
				case KEY_UP:		//��
					LCD_ShowString(30+200,400,200,16,16,"zhongquan+");
					zhongquan_case++;
					break;
				case KEY_POWER:
					flag = 1;
					chengxu = 99;
					break;
			}
			
			LCD_ShowNum(30+200+48+8+10,360,zhongquan_case,4,16);
			
			if(flag)
				break;
		}
		
		
		flag = 0;
		key = 0;
				while(1)
		{
			key = Remote_Scan();
			switch(key)
			{
				case 0:		//û�а�������
					
					break;
				case KEY_RIGHT:		//��
					LCD_ShowString(30+200,400,200,16,16,"start   ");
					flag = 1;
					break;
				case KEY_DOWN:		//��
					LCD_ShowString(30+200,400,200,16,16,"sanfen-");
					if(sanfen_case != 0)
						sanfen_case--;
					break;
				case KEY_LEFT:		//��
					LCD_ShowString(30+200,400,200,16,16,"clear   ");
					sanfen_case = 0;
					break;
				case KEY_UP:		//��
					LCD_ShowString(30+200,400,200,16,16,"sanfen+");
					sanfen_case++;
					break;
				case KEY_POWER:
					flag = 1;
					chengxu = 99;
					break;
			}
			
			LCD_ShowNum(30+200+48+8+10,380,sanfen_case,4,16);
			
			if(flag)
				break;
		}
		*/
		
		
		
		//��ʱ10s
		//ѭ��100�Σ�delay_ms(40000)
		for(time = 0 ;time <20;time++)
			delay_ms(4000);
		//ÿ�γ���ʼǰ��ʼ��λ����Ϣ
		control_init();						//�����˳�ʼ��
		
		
		switch(chengxu)
		{
			case 0:	    	//���Գ���
				switch(qiu){
					case 0:
						robot_turnOrigin_stage(30);
						//˳ʱ��180��
						break;
					case 1:
						//��е���½�
						jixiebi_down();
						LED0 = !LED0;
						break;
					case 2:
						//��е������
						jixiebi_up();
						LED0 = !LED0;
						break;
					case 3:
						//�������
						get_hongwai();
						LED0 = !LED0;
						break;
					case 4:
						//�������
						while(1)
						{
							time = 0;
							key = Remote_Scan();
							switch(key)
							{
								case KEY_1:
									charge(1);
									LED0 = 0;
									break;
								case KEY_2:
									charge(0);
									LED0 = 1;
									break;
								case KEY_3:
									charge(0);
									delay_ms(30000);
									GPIO_SetBits(GPIOG,GPIO_Pin_7);
									LED1 = 0;
									delay_ms(10000);
									delay_ms(10000);
									delay_ms(10000);
									GPIO_ResetBits(GPIOG,GPIO_Pin_7);
									LED1 = 1;
									break;
								case KEY_4:
									TIM_SetCompare1(TIM9,1950);		//PE6
									TIM_SetCompare2(TIM9,3990);		//PE5
									break;
								case KEY_5:
									TIM_SetCompare1(TIM9,3990);		//PE6
									TIM_SetCompare2(TIM9,3000);		//PE5
									break;
								case KEY_6:
									TIM_SetCompare1(TIM9,3990);		//PE6
									TIM_SetCompare2(TIM9,3990);		//PE5
								case KEY_7:
									time = 1;
									break;
							}
							if(time)
								break;
						}
						break;
					case 5:
						//�Ӿ�����
					//find_ball_sanfen(1);
						find_ball(qiu);
						break;
					case 6:
						//�������
						find_ball_laser();
						break;
					case 7:
						//ֱ������
						//robot_straight_stage(1,1,0);
						set_motor_vx_vy_w_R(0,0,100);//��ʱ��
						
						//set_motor_vx_vy_w(0,100,0);
						control1_W(robot_zqd.pwm[0]);
						control2_W(robot_zqd.pwm[1]);
						control3_W(robot_zqd.pwm[2]);
						//delay_ms(40000);
						//control1_W(0);
						//control2_W(0);
						//control3_W(0);
						
						break;
					case 8:
						//���������
						find_lankuang();
						break;
					case 9:
						remote_control();
						break;
				}		
				
				break;
			case 1:
				//�����һ�غ�
				// --------------- --------------
				//|     |         |       |      |
				//|  -            |         -    |
				//|-              |            - |
				// --------------- --------------
				//             ��     
				charge(1);
				robot_straight_stage(0,4.5f,310);
				//robot_straight_stage(0,5.3f,70);										//��
				//robot_straight_stage(0,5.4f,290);									//�ҳ�
				//robot_straight_ObsAvoidance(0,6.4f,0);
				//robot_turnOrigin_stage(290);
				/*panduan_weizhi();
				if(zhongquan_case==0)
					robot_straight_stage(0,5.4f,0);
				if(zhongquan_case==2)
					robot_straight_stage(robot_zqd.X-0.4f,robot_zqd.Y,90);
				robot_turnOrigin_stage(290);*/
				//delay_ms(30000);
				if(down_shot_up())
					break;
				//delay_ms(30000);
				charge(1);
				/*if(zhongquan_case==0)
				{
					robot_straight_stage(0,4.2f,0);
				}
				else if(zhongquan_case==1)
				{	
					robot_straight_stage(robot_zqd.X+1.32f,robot_zqd.Y-0.5f,45);
				}
				else if(zhongquan_case==2)
				{
					robot_straight_stage(robot_zqd.X-0.4f,robot_zqd.Y,135);
				}
				find_ball_sanfen(qiu);
				//find_ball_zhongquan();			//�״�����
				//find_ball(qiu);						//�Ӿ�����
				//delay_ms(30000);*/
				robot_turnOrigin_stage(0);						//�ҳ�
				find_ball_sanfen(qiu);
				if(robot_zqd.X>0)
					//robot_turnOrigin_stage(90);										//��
					robot_turnOrigin_stage(270);									//�ҳ�
				else
					//robot_turnOrigin_stage(70);									//��
					robot_turnOrigin_stage(290);							//�ҳ�
				//delay_ms(30000);
			
				if(down_shot_up())
					break;
				//delay_ms(30000);
				//robot_straight_stage(0,0,0);		 			//��
				robot_straight_stage(-0.1f,0,0);									//�ҳ�
				break;
			case 2:
				//����ڶ��غ�
				// --------------- --------------
				//|     |         |       |      |
				//|  -            |         -    |
				//|-              |            - |
				// --------------- --------------
				//             ��     
				charge(1);
				robot_straight_stage(0,3,0);
				panduan_weizhi();
				if(zhongquan_case!=0){
					panduan_weizhi2();
				}
				//delay_ms(30000);
				//find_ball_laser();
				find_ball_sanfen(qiu); // 1������ 3������
				//find_ball(qiu);					//�Ӿ�����
				//find_ball_zhongquan();				//��������
			  //delay_ms(30000);
				//robot_turnOrigin_stage(80);											//��
				robot_turnOrigin_stage(290);										//�ҳ�
				//delay_ms(30000);
				if(down_shot_up())
					break;
				//delay_ms(30000);
				charge(1);
				if(zhongquan_case==1)
					robot_turnOrigin_stage(225);
				//robot_straight_stage(-4.5f,2.2f,45);							//��
				//robot_straight_stage(4.5f,2.2f,315);              //�ҳ�
				sanfenpoint(1,zhongquan_case);
				//delay_ms(30000);
				 //��ɫ����
				//find_ball_zhongquan();				//�״�����
				//find_ball(qiu);      //�Ӿ�����
				find_ball_sanfen(qiu);
				//find_ball_zhongquan();
				//delay_ms(30000);
				robot_straight_stage(robot_zqd.X,robot_zqd.Y-1,0);//�˺�һ�� 
				//delay_ms(30000);
				if(down_shot_up())
					break;
				//delay_ms(30000);
				robot_zqd.theta_offset = -0.05f;
				robot_straight_stage(0,robot_zqd.Y,0);
				robot_straight_stage(-0.1f,-0.10f,0);//ԭ�� 0 0 0 
				break;
			case 3:
				//��������غ� 
				// --------------- --------------
				//|     |         |       |      |
				//|  -            |         -    |
				//|-              |            - |
				// --------------- --------------
				//                  ��     
				charge(1);
				set_motor_vx_vy_w(0,400,0);
				control1_W(robot_zqd.pwm[0]);
				control2_W(robot_zqd.pwm[1]);
				control3_W(robot_zqd.pwm[2]);
				delay_ms(40000);
				//robot_straight_stage(-3.9,2,45);								//��
				robot_straight_stage(3.9f,2,315);									//�ҳ�
				//delay_ms(30000);
				find_ball_sanfen(qiu);
				//find_ball_zhongquan();			//�״�����
				//find_ball(qiu);				//�Ӿ�����
				//delay_ms(30000);
				//sanfenpoint(sanfen_case,100);
				robot_straight_stage(robot_zqd.X,robot_zqd.Y-1,0);
				delay_ms(30000);
				if(down_shot_up())
					break;
				//delay_ms(30000);
				charge(1);
				robot_straight_stage(robot_zqd.X,robot_zqd.Y+1.3f,0);
				//robot_straight_stage(-8.5,robot_zqd.Y-2,180);									//��
				robot_straight_stage(8.5,robot_zqd.Y,180);										//�ҳ�  ���޸�
				robot_straight_stage(robot_zqd.X,2,180);		
				//delay_ms(30000);
				find_ball_dixian();			//�״�����							
				//find_ball(qiu);				//�Ӿ�����
				//find_ball_sanfen(qiu);
				//delay_ms(30000);
				//robot_turnOrigin_stage(330);											//��
				//robot_turnOrigin_stage(30);												//�ҳ�
				robot_straight_stage(robot_zqd.X,robot_zqd.Y-0.5f,30);
				//delay_ms(30000);
				if(down_shot_up())
					break;
				//delay_ms(30000);
				robot_zqd.theta_offset = -0.05f;
				robot_straight_stage(0,robot_zqd.Y,0);
				//delay_ms(30000);
				robot_straight_stage(0,0,0);
				break;
			case 4:
				//Ͷ����һ�غ�
				// --------------- --------------
				//|     |         |       |      |
				//|  -            |         -    |
				//|-              |            - |
				// --------------- --------------
				//             ��     
				charge(1);
				//robot_straight_stage(-8,7.5,90);										//��
				robot_straight_stage(8,7,270);										//�ҳ�							
				//delay_ms(30000);
				find_lankuang();
				//delay_ms(30000);
				if(down_shot_up())
					break;
				//delay_ms(30000);
				charge(1);
				robot_straight_stage(0,4.2,0);
				panduan_weizhi();
				if(zhongquan_case==2)
					robot_turnOrigin_stage(135);
//			robot_straight_stage(2.5f,7,90);
//			panduan_weizhifan();
				//delay_ms(30000);
				find_ball_sanfen(qiu);
				//find_ball_zhongquan();			//�״�����
				//find_ball(qiu);							//�Ӿ�����
				//delay_ms(30000);
				//robot_straight_stage(-8,7,90);									//��
				zhongquanpoint(zhongquan_case);		
				robot_straight_stage(8,7,270);									//�ҳ�
				//delay_ms(30000);
				find_lankuang(); 
				//delay_ms(30000);
				if(down_shot_up())
					break;
				//delay_ms(30000);
				//while(1);
				robot_straight_stage(0,2,0);
				//robot_straight_stage(0.1,0,0);										//��
				robot_straight_stage(-0.1,0,0);									//�ҳ�
				break;
			case 5:
				//Ͷ���ڶ��غ�
				// --------------- --------------
				//|     |         |       |      |
				//|  -            |         -    |
				//|-              |            - |
				// --------------- --------------
				//             ��     
				charge(1);
			//set_motor_vx_vy_w(0,400,0);
			//control1_W(robot_zqd.pwm[0]);
			//control2_W(robot_zqd.pwm[1]);
			//control3_W(robot_zqd.pwm[2]);
			//delay_ms(40000);
				robot_straight_stage(0,3.5f,0);
				//robot_straight_ObsAvoidance(0,4.5f,0);
				panduan_weizhi();
				//delay_ms(30000);
				find_ball_sanfen(qiu);
				//find_ball(qiu);					//�Ӿ�����
				//find_ball_zhongquan();				//��������
				//delay_ms(30000);
				//robot_turnOrigin_stage(315);
				if(zhongquan_case==1)
					robot_turnOrigin_stage(225);
				zhongquanpoint(zhongquan_case);
				//delay_ms(30000);
				robot_turnOrigin_stage(270);
				//robot_straight_stage(-8,7,90);											//��	
				//robot_straight_stage(8,7,270);										//�ҳ�
				robot_straight_ObsAvoidance(8,7,270);
				//delay_ms(30000);
				find_lankuang();
				//delay_ms(30000);
				if(down_shot_up())
					break;
				charge(1);
				//delay_ms(30000);
				//robot_straight_stage(-4.5,2.8,45);										//��
				//robot_straight_stage(robot_zqd.X,1,0);
				//robot_straight_stage(4.5,2.2,315);									//�ҳ�
				robot_turnOrigin_stage(135);
				robot_straight_ObsAvoidance(4.5f,2.8f,135);//�ҳ�
				robot_turnOrigin_stage(315);				
				//delay_ms(30000);
				find_ball_sanfen(qiu);
				//delay_ms(30000);
				robot_straight_stage(robot_zqd.X,robot_zqd.Y+0.5f,315);
				//robot_straight_stage(-7,6.5,90);											//��
				robot_straight_stage(8,7,270);										//�ҳ�				
				//delay_ms(30000);
				find_lankuang();
				//delay_ms(30000);
				if(down_shot_up())
					break;
				//delay_ms(30000);
				//while(1);
				robot_straight_stage(robot_zqd.X,2,0);
				robot_straight_stage(0,2,0);
				robot_straight_stage(0,0,0);
				break;
			case 6:
				//Ͷ�������غ�
				// --------------- --------------
				//|     |         |       |      |
				//|  -            |         -    |
				//|-              |            - |
				// --------------- --------------
				//                  ��     
				charge(1);
				set_motor_vx_vy_w(0,400,0);
				control1_W(robot_zqd.pwm[0]);
				control2_W(robot_zqd.pwm[1]);
				control3_W(robot_zqd.pwm[2]);
				//delay_ms(20000);
				//robot_straight_ObsVoidance(-4.0f,2.2f,45);									//��
				//robot_straight_ObsAvoidance(4.0f,2.2f,315);										//�ҳ�
				robot_straight_stage(4,2.2f,315);
				//delay_ms(30000);
				find_ball_sanfen(qiu);					//�Ӿ�����
				//delay_ms(30000);
				//robot_certain_point(8,7,270,robot_zqd.X,robot_zqd.Y+0.5f,0);
				robot_straight_stage(robot_zqd.X,robot_zqd.Y+0.5f,0);
				//robot_straight_stage(8.925f,3.0f,0);   //������������
				//robot_straight_stage(-8,7,90);										//��
				robot_straight_stage(8,7,270);											//�ҳ�
				//delay_ms(30000);
				find_lankuang();
				//delay_ms(30000);
				if(down_shot_up())
					break;
				//delay_ms(30000);
				charge(1);
				//robot_straight_stage(-11,3,180);									//��
				robot_straight_stage(9.5f,3,180);										//�ҳ�
				//delay_ms(30000);
				find_ball_dixian();			//�״�����
				//find_ball_sanfen(qiu);
				//find_ball(qiu);
				//delay_ms(30000);
				//robot_straight_stage(robot_zqd.X,robot_zqd.Y+2,0);
				//robot_straight_stage(-8,7,90);											//��
				robot_straight_stage(8,7,270);												//�ҳ�
				//delay_ms(30000);
				find_lankuang();
				//delay_ms(30000);
				if(down_shot_up())
					break;
				//delay_ms(30000);
				//while(1);
				robot_straight_stage(0,3,0);
				delay_ms(30000);
				robot_straight_stage(0,-0.2,0);
				break;
			case 7:
				//�Ӿ��״����
				find_ball_sanfen(1);
				break;
			case 8:
					robot_straight_stage(2.6f,7,90);
					panduan_weizhifan();
					find_ball_sanfen(qiu);
			
			case 9:
				//�����غϲ��Գ���
				charge(1);
				set_motor_vx_vy_w(0,400,0);
				control1_W(robot_zqd.pwm[0]);
				control2_W(robot_zqd.pwm[1]);
				control3_W(robot_zqd.pwm[2]);
				delay_ms(20000);
				//robot_straight_stage(-3.9,2,45);										//��
				robot_straight_stage(3.9,2,315);										//�ҳ�
				delay_ms(30000);
				//find_ball_zhongquan( );			//�״�����
				//find_ball(qiu);				//�Ӿ�����
				delay_ms(30000);
				robot_straight_stage(robot_zqd.X,robot_zqd.Y-1,0);
				delay_ms(30000);
				//if(down_shot_up())
				//	break;
				delay_ms(30000);
				charge(1);
				//robot_straight_stage(-11,3,180);									//��
				robot_straight_stage(11,3,180);										//�ҳ�
				delay_ms(30000);
				//find_ball_dixian();			//�״�����
				//if(qiu == 1)					//�Ӿ�����
				//	qiu = 2;
				//else
				//	qiu = 1;
				//find_ball(qiu);
				delay_ms(30000);
				//robot_turnOrigin_stage(330);											//��
				robot_turnOrigin_stage(30);												//�ҳ�
				delay_ms(30000);
				//if(down_shot_up())
				//	break;
				delay_ms(30000);
				robot_zqd.theta_offset = -0.05f;
				robot_straight_stage(0,robot_zqd.Y,0);
				delay_ms(30000);
				robot_straight_stage(0,0,0);
				break;
		}

		
		
	}
	
}

