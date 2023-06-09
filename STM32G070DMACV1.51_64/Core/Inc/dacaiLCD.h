

#ifndef _dacaiLCD_H
#define _dacaiLCD_H



#include "sys.h"

#define FIRMWARE_VER 917   /*!< 保持此固件版本号与真实屏幕一致，确保新增功能可用*/
#define CRC16_ENABLE 0         /*!< 如果需要CRC16校验功能，修改此宏为1(此时需要在VisualTFT工程中配CRC校验)*/
#define CMD_MAX_SIZE   64       /*!<单条指令大小，根据需要调整，尽量设置大一些*/
#define QUEUE_MAX_SIZE 128   /*!< 指令接收缓冲区大小，根据需要调整，尽量设置大一些*/




typedef unsigned char qdata;
typedef unsigned short qsize;



#define NOTIFY_TOUCH_PRESS         0X01  //触摸屏按下通知
#define NOTIFY_TOUCH_RELEASE       0X03  //触摸屏松开通知
#define NOTIFY_WRITE_FLASH_OK      0X0C  //写FLASH成功
#define NOTIFY_WRITE_FLASH_FAILD   0X0D  //写FLASH失败
#define NOTIFY_READ_FLASH_OK       0X0B  //读FLASH成功
#define NOTIFY_READ_FLASH_FAILD    0X0F  //读FLASH失败
#define NOTIFY_MENU                0X14  //菜单事件通知
#define NOTIFY_TIMER               0X43  //定时器超时通知
#define NOTIFY_CONTROL             0XB1  //控件更新通知
#define NOTIFY_READ_RTC            0XF7  //读取RTC时间
#define MSG_GET_CURRENT_SCREEN     0X01  //画面ID变化通知
#define MSG_GET_DATA               0X11  //控件数据通知



enum CtrlType
{
    kCtrlUnknown=0x0,
    kCtrlButton=0x10,                     //按钮
    kCtrlText,                            //文本
    kCtrlProgress,                        //进度条
    kCtrlSlider,                          //滑动条
    kCtrlMeter,                            //仪表
    kCtrlDropList,                        //下拉列表
    kCtrlAnimation,                       //动画
    kCtrlRTC,                             //时间显示
    kCtrlGraph,                           //曲线图控件
    kCtrlTable,                           //表格控件
    kCtrlMenu,                            //菜单控件
    kCtrlSelector,                        //选择控件
    kCtrlQRCode,                          //二维码
};

#pragma pack(push)
#pragma pack(1) //按字节对齐

typedef struct
{
    u8    cmd_head;                    //帧头

    u8    cmd_type;                    //命令类型(UPDATE_CONTROL)    
    u8    ctrl_msg;                    //CtrlMsgType-指示消息的类型
    u16   screen_id;                   //产生消息的画面ID
    u16   control_id;                  //产生消息的控件ID
    u8    control_type;                //控件类型

    u8    param[20];                  //可变长度参数，最多256个字节

    u8  cmd_tail[4];                   //帧尾
}CTRL_MSG,*PCTRL_MSG;

#pragma pack(pop)


typedef struct _QUEUE                                             
{                                                                 
    u16 _head;                                                       //队列头
    u16 _tail;                                                       //队列尾
    u8 _data[QUEUE_MAX_SIZE];                                        //队列数据缓存区
}QUEUE; 

 


                                           //指令队列
extern u32 cmd_state;                                           //队列帧尾检测状态
extern  qsize cmd_pos;                                              //当前指令指针位置

extern  QUEUE que ;  








extern u16 current_screen_id ;
extern u8  cmd_buffer[CMD_MAX_SIZE]; 


 void queue_reset(void);

/*! 
 * \brief  添加指令数据
 * \detial 串口接收的数据，通过此函数放入指令队列 
 *  \param  _data 指令数据
 */
 void queue_push(void);

/*! 
 *  \brief  从指令队列中取出一条完整的指令
 *  \param  cmd 指令接收缓存区
 *  \param  buf_len 指令接收缓存区大小
 *  \return  指令长度，0表示队列中无完整指令
 */
 qsize queue_find_cmd(qdata *cmd,qsize buf_len);















/*! 
*  \brief  消息处理流程
*  \param msg 待处理消息
*  \param size 消息长度
*/
void ProcessMessage( PCTRL_MSG msg, u16 size );
/*! 
*  \brief  画面切换通知
*  \details  当前画面改变时(或调用GetScreen)，执行此函数
*  \param screen_id 当前画面ID
*/
void NotifyScreen(u16 screen_id);
/*! 
*  \brief  触摸坐标事件响应
*  \param press 1按下触摸屏，3松开触摸屏
*  \param x x坐标
*  \param y y坐标
*/
void NotifyTouchXY(u8 press,u16 x,u16 y);

/*! 
*  \brief  按钮控件通知
*  \details  当按钮状态改变(或调用GetControlValue)时，执行此函数
*  \param screen_id 画面ID
*  \param control_id 控件ID
*  \param state 按钮状态：0弹起，1按下
*/
void NotifyButton(u16 screen_id, u16 control_id, u8 state);
/*! 
*  \brief  文本控件通知
*  \details  当文本通过键盘更新(或调用GetControlValue)时，执行此函数
*  \details  文本控件的内容以字符串形式下发到MCU，如果文本控件内容是浮点值，
*  \details  则需要在此函数中将下发字符串重新转回浮点值。
*  \param screen_id 画面ID
*  \param control_id 控件ID
*  \param str 文本控件内容
*/
void NotifyText(u16 screen_id, u16 control_id, u8 *str);
/*!                                                                              
*  \brief  进度条控件通知                                                       
*  \details  调用GetControlValue时，执行此函数                                  
*  \param screen_id 画面ID                                                      
*  \param control_id 控件ID                                                     
*  \param value 值                                                              
*/   
void NotifyProgress(u16 screen_id, u16 control_id, u32 value);
/*!                                                                              
*  \brief  滑动条控件通知                                                       
*  \details  当滑动条改变(或调用GetControlValue)时，执行此函数                  
*  \param screen_id 画面ID                                                      
*  \param control_id 控件ID                                                     
*  \param value 值                                                              
*/    
void NotifySlider(u16 screen_id, u16 control_id, u32 value);
/*! 
*  \brief  仪表控件通知
*  \details  调用GetControlValue时，执行此函数
*  \param screen_id 画面ID
*  \param control_id 控件ID
*  \param value 值
*/
void NotifyMeter(u16 screen_id, u16 control_id, u32 value);
/*! 
*  \brief  菜单控件通知
*  \details  当菜单项按下或松开时，执行此函数
*  \param screen_id 画面ID
*  \param control_id 控件ID
*  \param item 菜单项索引
*  \param state 按钮状态：0松开，1按下
*/
void NotifyMenu(u16 screen_id, u16 control_id, u8  item, u8  state);

/*! 
*  \brief  选择控件通知
*  \details  当选择控件变化时，执行此函数
*  \param screen_id 画面ID
*  \param control_id 控件ID
*  \param item 当前选项
*/
void NotifySelector(u16 screen_id, u16 control_id, u8  item);
/*! 
*  \brief  定时器超时通知处理
*  \param screen_id 画面ID
*  \param control_id 控件ID
*/
void NotifyTimer(u16 screen_id, u16 control_id);
/*! 
*  \brief  读取用户FLASH状态返回
*  \param status 0失败，1成功
*  \param _data 返回数据
*  \param length 数据长度
*/
void NotifyReadFlash(u8 status,u8 *_data,u16 length);

/*! 
*  \brief  写用户FLASH状态返回
*  \param status 0失败，1成功
*/
void NotifyWriteFlash(u8 status);
/*! 
*  \brief  读取RTC时间，注意返回的是BCD码
*  \param year 年（BCD）
*  \param month 月（BCD）
*  \param week 星期（BCD）
*  \param day 日（BCD）
*  \param hour 时（BCD）
*  \param minute 分（BCD）
*  \param second 秒（BCD）
*/
void NotifyReadRTC(u8 year,u8 month,u8 week,u8 day,u8 hour,u8 minute,u8 second);


void SendStrings(u8 *str);


void SetTextInt32(u16 screen_id,u16 control_id,u32 value,u8 sign,u8 fill_zero);

void SetTextValue(u16 screen_id,u16 control_id,u8 *str);


void AnimationPlayFrame(u16 screen_id,u16 control_id,u8 frame_id);





#endif



