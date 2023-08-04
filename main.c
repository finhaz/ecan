/*
 *     main.c
 *
 *     Created on: 2018-6-5
 *     Author: naiyangui,fin
 *     ��������Ϊ2A������Ϊ8A
 *     �����ŵ�ѹ�⻷����ʱ��ĳɺ�ת���⻷һ��Ϊ5ms
 *     ��λ�Ƕ�11pi/6
 *     ��������Kp=20;Ki=0.0001;�޷�ֵ��155��
 *     ��ѹ�⻷��Kp:0.001;Ki��0.0001��������-6;
 *     ת�ٻ���Kp=0.0025(2000rad/s��ǰ)��Ki=0.00025  �޷�ֵ��=-6
 *          Kp=0.002(3000rad/s��ǰ)��Ki=0.00025   �޷�ֵ��=-6
 *          Kp=0.003(4000rad/s��ǰ)��Ki=0.00025   �޷�ֵ��=-6
 *          Kp=0.004(5000rad/s��ǰ)��Ki=0.00025   �޷�ֵ��=-6
 *          Kp=0.005(7000rad/s��ǰ)��Ki=0.00025   �޷�ֵ��=-6
 *     �ǶȲ�������������һ�����ڽǶȣ���ѹ����2�����ڽǶ�
 *     ͣ���ƶ�������pwm,��ʸ��
 *      �����ŵ�ѹ�⻷��Ϊ1�����ڵ���һ��
 *      ����Ƶ�ʸ�Ϊ10K
 *      ����ת�٣�17000rpm
 *      �����˼���б��ģ��
 *      ����ͣ��ʹ�ð汾(iq<0,����PWM����ʱֱͨ����)
 *      �ж�ʱ������
 *      �޸��л�����
 *      ����оƬ��28279D��Ϊ28335
 */

#include "DSP28x_Project.h"     // DSP28x Headerfile
#include "math.h"
#include "bsp_ecan.h"
#include "Can_message.h"

__interrupt void Ecan0ISR(void);//R  ���պ������ж�
//_iq a,b,c;

/**
 * main.c
 */
void main(void)
{
    InitSysCtrl();

    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();

    InitECanGpio();
    ConfigureEcan();

    EALLOW;
    PieVectTable.ECAN0INTA = &Ecan0ISR;//R-CAN1  ���պ��жϺ���
    EDIS;

    //ʹ��CPU�ж�
    IER |=M_INT9;//SCI�ж�     ��CPU�ж�1~9(���뿪�Ŷ�Ӧ��CPU���жϿ�)
    IER |=M_INT1;//cputime and adc and�ⲿ�ж�
    //ʹ��PIE�ж�
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
    PieCtrlRegs.PIEIER9.bit.INTx5=1;     //R-CAN0  ��������

    //��ȫ���ж�
    EINT;//ʹ��ȫ���жϣ����жϣ���CPU���ģ�
    ERTM;//ʹ��ʵʱ�жϣ�CPU���ģ�

    ///////////////////////////////////////��ʼ������
    while(1)
    {
        Checkdata();
        CanSend();
    }

}


//

__interrupt void Ecan0ISR(void)//R  ���պ������ж�
{
     //DINT;
    if(ECanaRegs.CANRMP.all==0x00010000)//RX get after flag and int   BOX16
    {
        ECanaRegs.CANRMP.all = 0x00010000;//clear GMIF16
        CanRecieve();
    }
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

    //EINT;
}
