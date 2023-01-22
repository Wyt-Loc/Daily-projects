#ifndef __PID_H
#define __PID_H	 
#include "sys.h"


typedef struct
{
    //PID����ģʽ
    uint8_t mode;
    //PID ������������
    float Kp;
    float Ki;
    float Kd;

    float max_out;  //PID������
    float max_iout; //PID���������

    float set;	  //PIDĿ��ֵ
    float fdb;	  //PID��ǰֵ

    float out;		//����������
    float Pout;		//���������
    float Iout;		//���������
    float Dout;		//΢�������
    //΢�����������ֵ 0���� 1��һ�� 2���ϴ�
    float Dbuf[3];  
    //������������ֵ 0���� 1��һ�� 2���ϴ�
    float error[3];  

} pid_type_def;





enum PID_MODE
{
    PID_POSITION = 0,
    PID_DELTA
};



void PID_init(pid_type_def *pid, uint8_t mode, const float PID[3], float max_out, float max_iout);
float PID_calc(pid_type_def *pid, float ref, float set);


#endif


