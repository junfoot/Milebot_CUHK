
#ifndef __SYN6658_H__
#define __SYN6658_H__


/* Exported macro ------------------------------------------------------------*/

//芯片忙状态指示
#define SYN6658_STATUS_IDLE    0
#define SYN6658_STATUS_BUSY   1

//帧头
#define SYN6658_FRAME_HEAD 0xFD     

//播放命令的参数, 代表不加背景音乐而且用GBK 编码
#define SYN6658_CMD_PARAM   0x01

//音量调节的最大等级
#define SYN6658_MAX_VOLUME_LEVEL    (10)  //syn6658芯片音量设置, 0-10, 0为静音

//数据长度的索引(共2 字节, 高字节在前)
#define DATA_LEN_HIGH_BYTE_INDEX    (1)
#define DATA_LEN_LOW_BYTE_INDEX    (2)


/* Exported types ------------------------------------------------------------*/

//syn6658 的播放队列
#define SYN6658_QUEUE_SIZE	32
typedef struct
{
    uint8_t qBuf[SYN6658_QUEUE_SIZE];	//第一个元素，存放地址
    uint8_t head; //指向队列首部, 取出队列out
    uint8_t tail; //指向队列尾部, 加入队列in
} SYN6658_QUEUE;


//syn6658合成语音的种类
//总数量要和const char* TTS_list[] 数组的数量一致
typedef enum
{
    SOUND_MILEBOT_COMPANY = 0x00,   //欢迎使用迈步外骨骼机器人
    SOUND_TIE_BANDAGE,  //请确认系好绑带
    SOUND_START_WALK,    //请开始行走
    SOUND_START_ASSIST,   //开始助力
    SOUND_LOW_BATTERY,  //电池电量低，请及时充电
    SOUND_APP_CONNECTED,//APP已连接
    SOUND_APP_DISCONNECTED,//APP已断开
    SOUND_LIFT_LEFT_LEG,//请抬左腿
    SOUND_LIFT_RIGHT_LEG,//请抬右腿
    SOUND_NORMAL_MODE,//正常模式
    SOUND_HEMIPLEGIA_LEFT_MODE,//左偏瘫模式
    SOUND_HEMIPLEGIA_RIGHT_MODE,//右偏瘫模式
    SOUND_RESISTANCE_MODE,//阻力模式
    SOUND_ZERO_FORCE_MDOE,//零力模式
    SOUND_PASSIVE_MODE,//被动模式
    SOUND_TESTING_MODE,//测试模式

    SOUND_STAND_UP,//站立提示
    SOUND_SIT_DOWN,//坐下提示
    SOUND_SLIP,//摔倒提示

    SOUND_UPGRADE_SOFTWARE,//升级设备软件

	SOUND_ASSESSMENT_MODE,//评估模式
	SOUND_KEEP_STAND_UP,//请保持站立，
	SOUND_START_ASSESSMENT,//开始评估，请开始行走
	SOUND_ASSESSMENT_FINISH,//评估结束

	SOUND_ASSIST_LEVEL_ONE,//一挡助力
	SOUND_ASSIST_LEVEL_TWO,//二挡助力
	SOUND_ASSIST_LEVEL_THREE,//三挡助力
	SOUND_ASSIST_LEVEL_FOUR,//四挡助力
	SOUND_ASSIST_LEVEL_FIVE,//五挡助力
    
    SOUND_RESIST_LEVEL_ONE,//一挡阻力
	SOUND_RESIST_LEVEL_TWO,//二挡阻力
	SOUND_RESIST_LEVEL_THREE,//三挡阻力
	SOUND_RESIST_LEVEL_FOUR,//四挡阻力
	SOUND_RESIST_LEVEL_FIVE,//五挡阻力
    SOUND_LIST_NUM
} SOUND_LIST;

typedef enum    //控制命令
{
    VOICE_PLAY = 0x01,  //播放
    VOICE_STOP = 0x02,  //停止
    VOICE_PAUSE = 0x03,  //暂停
    VOICE_RESUME = 0x04,  //恢复
    VOICE_STATUS = 0x21,  //芯片状态查询
    VOICE_POWER_DOWN = 0x88 //进入低功耗模式
} SYN6658_CMD;

/*
芯片接收到控制命令帧，会向上位机发送1 个字节的状态回传，
上位机可根据这个回传来判断芯片目前的工作状态
*/
typedef enum
{
    SYN6658_INIT = 0x4A,    //初始化成功
    SYN6658_RECEIVE = 0x41,    //收到正确的命令帧
    SYN6658_NOT_IDENTIFY = 0x45,    //收到不能识别命令帧
    SYN6658_BUSY = 0x4E,    //收到“状态查询命令帧”，芯片处在正在播音状态

    //当一帧数据合成完以后，芯片进入空闲状态回传0x4F；
    //或者收到“状态查询命令帧”，芯片处于空闲状态回传0x4F
    SYN6658_IDLE = 0x4F,

} SYN6658_RESPONSE;

typedef enum    //语言芯片的播放状态机
{
    SOUND_FSM_IDLE = 0,
    SOUND_FSM_PRE_PLAYING_CHECK,
    SOUND_FSM_PLAYING,
    SOUND_FSM_PLAYING_CHECK,
} SOUND_FSM;



/* Exported functions prototypes ---------------------------------------------*/
void sound_task(void *argument);
void user_play_sound(SOUND_LIST list);
void syn6658_queue_init(SYN6658_QUEUE* qu);
void syn6658_set_volume(uint8_t level);
void syn6658_uart_receive_callback(uint8_t value);
uint8_t syn6658_queue_in(SYN6658_QUEUE* qu, uint8_t data);
uint8_t syn6658_queue_out(SYN6658_QUEUE* qu ,uint8_t* data);


#endif

