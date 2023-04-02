

#ifndef _dacaiLCD_H
#define _dacaiLCD_H



#include "sys.h"

#define FIRMWARE_VER 917   /*!< ���ִ˹̼��汾������ʵ��Ļһ�£�ȷ���������ܿ���*/
#define CRC16_ENABLE 0         /*!< �����ҪCRC16У�鹦�ܣ��޸Ĵ˺�Ϊ1(��ʱ��Ҫ��VisualTFT��������CRCУ��)*/
#define CMD_MAX_SIZE   64       /*!<����ָ���С��������Ҫ�������������ô�һЩ*/
#define QUEUE_MAX_SIZE 128   /*!< ָ����ջ�������С��������Ҫ�������������ô�һЩ*/




typedef unsigned char qdata;
typedef unsigned short qsize;



#define NOTIFY_TOUCH_PRESS         0X01  //����������֪ͨ
#define NOTIFY_TOUCH_RELEASE       0X03  //�������ɿ�֪ͨ
#define NOTIFY_WRITE_FLASH_OK      0X0C  //дFLASH�ɹ�
#define NOTIFY_WRITE_FLASH_FAILD   0X0D  //дFLASHʧ��
#define NOTIFY_READ_FLASH_OK       0X0B  //��FLASH�ɹ�
#define NOTIFY_READ_FLASH_FAILD    0X0F  //��FLASHʧ��
#define NOTIFY_MENU                0X14  //�˵��¼�֪ͨ
#define NOTIFY_TIMER               0X43  //��ʱ����ʱ֪ͨ
#define NOTIFY_CONTROL             0XB1  //�ؼ�����֪ͨ
#define NOTIFY_READ_RTC            0XF7  //��ȡRTCʱ��
#define MSG_GET_CURRENT_SCREEN     0X01  //����ID�仯֪ͨ
#define MSG_GET_DATA               0X11  //�ؼ�����֪ͨ



enum CtrlType
{
    kCtrlUnknown=0x0,
    kCtrlButton=0x10,                     //��ť
    kCtrlText,                            //�ı�
    kCtrlProgress,                        //������
    kCtrlSlider,                          //������
    kCtrlMeter,                            //�Ǳ�
    kCtrlDropList,                        //�����б�
    kCtrlAnimation,                       //����
    kCtrlRTC,                             //ʱ����ʾ
    kCtrlGraph,                           //����ͼ�ؼ�
    kCtrlTable,                           //����ؼ�
    kCtrlMenu,                            //�˵��ؼ�
    kCtrlSelector,                        //ѡ��ؼ�
    kCtrlQRCode,                          //��ά��
};

#pragma pack(push)
#pragma pack(1) //���ֽڶ���

typedef struct
{
    u8    cmd_head;                    //֡ͷ

    u8    cmd_type;                    //��������(UPDATE_CONTROL)    
    u8    ctrl_msg;                    //CtrlMsgType-ָʾ��Ϣ������
    u16   screen_id;                   //������Ϣ�Ļ���ID
    u16   control_id;                  //������Ϣ�Ŀؼ�ID
    u8    control_type;                //�ؼ�����

    u8    param[20];                  //�ɱ䳤�Ȳ��������256���ֽ�

    u8  cmd_tail[4];                   //֡β
}CTRL_MSG,*PCTRL_MSG;

#pragma pack(pop)


typedef struct _QUEUE                                             
{                                                                 
    u16 _head;                                                       //����ͷ
    u16 _tail;                                                       //����β
    u8 _data[QUEUE_MAX_SIZE];                                        //�������ݻ�����
}QUEUE; 

 


                                           //ָ�����
extern u32 cmd_state;                                           //����֡β���״̬
extern  qsize cmd_pos;                                              //��ǰָ��ָ��λ��

extern  QUEUE que ;  








extern u16 current_screen_id ;
extern u8  cmd_buffer[CMD_MAX_SIZE]; 


 void queue_reset(void);

/*! 
 * \brief  ����ָ������
 * \detial ���ڽ��յ����ݣ�ͨ���˺�������ָ����� 
 *  \param  _data ָ������
 */
 void queue_push(void);

/*! 
 *  \brief  ��ָ�������ȡ��һ��������ָ��
 *  \param  cmd ָ����ջ�����
 *  \param  buf_len ָ����ջ�������С
 *  \return  ָ��ȣ�0��ʾ������������ָ��
 */
 qsize queue_find_cmd(qdata *cmd,qsize buf_len);















/*! 
*  \brief  ��Ϣ��������
*  \param msg ��������Ϣ
*  \param size ��Ϣ����
*/
void ProcessMessage( PCTRL_MSG msg, u16 size );
/*! 
*  \brief  �����л�֪ͨ
*  \details  ��ǰ����ı�ʱ(�����GetScreen)��ִ�д˺���
*  \param screen_id ��ǰ����ID
*/
void NotifyScreen(u16 screen_id);
/*! 
*  \brief  ���������¼���Ӧ
*  \param press 1���´�������3�ɿ�������
*  \param x x����
*  \param y y����
*/
void NotifyTouchXY(u8 press,u16 x,u16 y);

/*! 
*  \brief  ��ť�ؼ�֪ͨ
*  \details  ����ť״̬�ı�(�����GetControlValue)ʱ��ִ�д˺���
*  \param screen_id ����ID
*  \param control_id �ؼ�ID
*  \param state ��ť״̬��0����1����
*/
void NotifyButton(u16 screen_id, u16 control_id, u8 state);
/*! 
*  \brief  �ı��ؼ�֪ͨ
*  \details  ���ı�ͨ�����̸���(�����GetControlValue)ʱ��ִ�д˺���
*  \details  �ı��ؼ����������ַ�����ʽ�·���MCU������ı��ؼ������Ǹ���ֵ��
*  \details  ����Ҫ�ڴ˺����н��·��ַ�������ת�ظ���ֵ��
*  \param screen_id ����ID
*  \param control_id �ؼ�ID
*  \param str �ı��ؼ�����
*/
void NotifyText(u16 screen_id, u16 control_id, u8 *str);
/*!                                                                              
*  \brief  �������ؼ�֪ͨ                                                       
*  \details  ����GetControlValueʱ��ִ�д˺���                                  
*  \param screen_id ����ID                                                      
*  \param control_id �ؼ�ID                                                     
*  \param value ֵ                                                              
*/   
void NotifyProgress(u16 screen_id, u16 control_id, u32 value);
/*!                                                                              
*  \brief  �������ؼ�֪ͨ                                                       
*  \details  ���������ı�(�����GetControlValue)ʱ��ִ�д˺���                  
*  \param screen_id ����ID                                                      
*  \param control_id �ؼ�ID                                                     
*  \param value ֵ                                                              
*/    
void NotifySlider(u16 screen_id, u16 control_id, u32 value);
/*! 
*  \brief  �Ǳ��ؼ�֪ͨ
*  \details  ����GetControlValueʱ��ִ�д˺���
*  \param screen_id ����ID
*  \param control_id �ؼ�ID
*  \param value ֵ
*/
void NotifyMeter(u16 screen_id, u16 control_id, u32 value);
/*! 
*  \brief  �˵��ؼ�֪ͨ
*  \details  ���˵���»��ɿ�ʱ��ִ�д˺���
*  \param screen_id ����ID
*  \param control_id �ؼ�ID
*  \param item �˵�������
*  \param state ��ť״̬��0�ɿ���1����
*/
void NotifyMenu(u16 screen_id, u16 control_id, u8  item, u8  state);

/*! 
*  \brief  ѡ��ؼ�֪ͨ
*  \details  ��ѡ��ؼ��仯ʱ��ִ�д˺���
*  \param screen_id ����ID
*  \param control_id �ؼ�ID
*  \param item ��ǰѡ��
*/
void NotifySelector(u16 screen_id, u16 control_id, u8  item);
/*! 
*  \brief  ��ʱ����ʱ֪ͨ����
*  \param screen_id ����ID
*  \param control_id �ؼ�ID
*/
void NotifyTimer(u16 screen_id, u16 control_id);
/*! 
*  \brief  ��ȡ�û�FLASH״̬����
*  \param status 0ʧ�ܣ�1�ɹ�
*  \param _data ��������
*  \param length ���ݳ���
*/
void NotifyReadFlash(u8 status,u8 *_data,u16 length);

/*! 
*  \brief  д�û�FLASH״̬����
*  \param status 0ʧ�ܣ�1�ɹ�
*/
void NotifyWriteFlash(u8 status);
/*! 
*  \brief  ��ȡRTCʱ�䣬ע�ⷵ�ص���BCD��
*  \param year �꣨BCD��
*  \param month �£�BCD��
*  \param week ���ڣ�BCD��
*  \param day �գ�BCD��
*  \param hour ʱ��BCD��
*  \param minute �֣�BCD��
*  \param second �루BCD��
*/
void NotifyReadRTC(u8 year,u8 month,u8 week,u8 day,u8 hour,u8 minute,u8 second);


void SendStrings(u8 *str);


void SetTextInt32(u16 screen_id,u16 control_id,u32 value,u8 sign,u8 fill_zero);

void SetTextValue(u16 screen_id,u16 control_id,u8 *str);


void AnimationPlayFrame(u16 screen_id,u16 control_id,u8 frame_id);





#endif


