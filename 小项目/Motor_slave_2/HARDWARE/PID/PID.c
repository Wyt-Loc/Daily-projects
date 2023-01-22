#include "PID.h"
#include "sys.h"
#include "stdlib.h"



//  \����Ϊ�м���
#define LimitMax(input, max)   \
    {                          \
        if (input > max)       \
        {                      \
            input = max;       \
        }                      \
        else if (input < -max) \
        {                      \
            input = -max;      \
        }                      \
    }
		
		



/*
*pid	����Ҫ��ʼ����PID�ṹ��ָ��
mode	PID���е�ģʽ������ʽ����λ��ʽPID���˴����Ƕ���һ��ö�ٱ�����������ģʽ
PID[3]	����һ�����飬������Ϊ������������P��I��D�ĳ�ʼֵ
max_out	PID��������޷�����ֹ����������󣬴���һ�����������Ʒ�ΧΪ[-max_out,+max_out]
max_iout	������������޷�����Ϊϵͳ������ʱ��Ŀ�����ϴ��ۼ������������ܴ�Ӱ��ϵͳ�ȶ��ԣ�
���Զ��ۼ��������޷�������һ�����������Ʒ�ΧΪ[-max_iout,+max_iout]
*/


//PID������ʼ��
void PID_init(pid_type_def *pid, uint8_t mode, const float PID[3], float max_out, float max_iout)
{
    if (pid == NULL || PID == NULL)
    {
        return;
    }
    pid->mode = mode;
    pid->Kp = PID[0];
    pid->Ki = PID[1];
    pid->Kd = PID[2];
    pid->max_out = max_out;
    pid->max_iout = max_iout;
    pid->Dbuf[0] = pid->Dbuf[1] = pid->Dbuf[2] = 0.0f;
    pid->error[0] = pid->error[1] = pid->error[2] = pid->Pout = pid->Iout = pid->Dout = pid->out = 0.0f;
}


//PID����
float PID_calc(pid_type_def *pid, float ref, float set)
{
    //�жϴ����PIDָ�벻Ϊ��
    if (pid == NULL)
    {
        return 0.0f;
    }
    //��Ź�ȥ���μ�������ֵ
    pid->error[2] = pid->error[1];
    pid->error[1] = pid->error[0];
    //�趨Ŀ��ֵ�͵�ǰֵ���ṹ���Ա
    pid->set = set;
    pid->fdb = ref;
    //�������µ����ֵ
    pid->error[0] = set - ref;
    //�ж�PID���õ�ģʽ
    if (pid->mode == PID_POSITION)
    {
        //λ��ʽPID
        //������������
        pid->Pout = pid->Kp * pid->error[0];
        //������������
        pid->Iout += pid->Ki * pid->error[0];
        //��Ź�ȥ���μ����΢�����ֵ
        pid->Dbuf[2] = pid->Dbuf[1];
        pid->Dbuf[1] = pid->Dbuf[0];
        //��ǰ����΢���ñ�������ȥ��һ�����������
        pid->Dbuf[0] = (pid->error[0] - pid->error[1]);
        //΢�������
        pid->Dout = pid->Kd * pid->Dbuf[0];
        //�Ի���������޷�
        LimitMax(pid->Iout, pid->max_iout);
        //������������������
        pid->out = pid->Pout + pid->Iout + pid->Dout;
        //������������޷�
        LimitMax(pid->out, pid->max_out);
    }
    else if (pid->mode == PID_DELTA)
    {
        //����ʽPID
        //�Ա���������ϴ����Ĳ�ֵ��Ϊ�����������������
        pid->Pout = pid->Kp * (pid->error[0] - pid->error[1]);
        //�Ա��������Ϊ������������
        pid->Iout = pid->Ki * pid->error[0];
        //����΢���������
        pid->Dbuf[2] = pid->Dbuf[1];
        pid->Dbuf[1] = pid->Dbuf[0];
        //�Ա���������ϴ����Ĳ�ֵ��ȥ�ϴ���������ϴ����Ĳ�ֵ��Ϊ΢���������������
        pid->Dbuf[0] = (pid->error[0] - 2.0f * pid->error[1] + pid->error[2]);
        pid->Dout = pid->Kd * pid->Dbuf[0];
        //����������������Ϊ�����
        pid->out += pid->Pout + pid->Iout + pid->Dout;
        //���������һ�����޷�
        LimitMax(pid->out, pid->max_out);
	}
	return pid->out;
}


void PID_clear(pid_type_def *pid)
{
    if (pid == NULL)
    {
        return;
    }
	//��ǰ�������
    pid->error[0] = pid->error[1] = pid->error[2] = 0.0f;
    //΢��������
    pid->Dbuf[0] = pid->Dbuf[1] = pid->Dbuf[2] = 0.0f;
    //�������
    pid->out = pid->Pout = pid->Iout = pid->Dout = 0.0f;
    //Ŀ��ֵ�͵�ǰֵ����
    pid->fdb = pid->set = 0.0f;
}






