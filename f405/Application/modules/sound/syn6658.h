
#ifndef __SYN6658_H__
#define __SYN6658_H__


/* Exported macro ------------------------------------------------------------*/

//оƬæ״ָ̬ʾ
#define SYN6658_STATUS_IDLE    0
#define SYN6658_STATUS_BUSY   1

//֡ͷ
#define SYN6658_FRAME_HEAD 0xFD     

//��������Ĳ���, �����ӱ������ֶ�����GBK ����
#define SYN6658_CMD_PARAM   0x01

//�������ڵ����ȼ�
#define SYN6658_MAX_VOLUME_LEVEL    (10)  //syn6658оƬ��������, 0-10, 0Ϊ����

//���ݳ��ȵ�����(��2 �ֽ�, ���ֽ���ǰ)
#define DATA_LEN_HIGH_BYTE_INDEX    (1)
#define DATA_LEN_LOW_BYTE_INDEX    (2)


/* Exported types ------------------------------------------------------------*/

//syn6658 �Ĳ��Ŷ���
#define SYN6658_QUEUE_SIZE	32
typedef struct
{
    uint8_t qBuf[SYN6658_QUEUE_SIZE];	//��һ��Ԫ�أ���ŵ�ַ
    uint8_t head; //ָ������ײ�, ȡ������out
    uint8_t tail; //ָ�����β��, �������in
} SYN6658_QUEUE;


//syn6658�ϳ�����������
//������Ҫ��const char* TTS_list[] ���������һ��
typedef enum
{
    SOUND_MILEBOT_COMPANY = 0x00,   //��ӭʹ�����������������
    SOUND_TIE_BANDAGE,  //��ȷ��ϵ�ð��
    SOUND_START_WALK,    //�뿪ʼ����
    SOUND_START_ASSIST,   //��ʼ����
    SOUND_LOW_BATTERY,  //��ص����ͣ��뼰ʱ���
    SOUND_APP_CONNECTED,//APP������
    SOUND_APP_DISCONNECTED,//APP�ѶϿ�
    SOUND_LIFT_LEFT_LEG,//��̧����
    SOUND_LIFT_RIGHT_LEG,//��̧����
    SOUND_NORMAL_MODE,//����ģʽ
    SOUND_HEMIPLEGIA_LEFT_MODE,//��ƫ̱ģʽ
    SOUND_HEMIPLEGIA_RIGHT_MODE,//��ƫ̱ģʽ
    SOUND_RESISTANCE_MODE,//����ģʽ
    SOUND_ZERO_FORCE_MDOE,//����ģʽ
    SOUND_PASSIVE_MODE,//����ģʽ
    SOUND_TESTING_MODE,//����ģʽ

    SOUND_STAND_UP,//վ����ʾ
    SOUND_SIT_DOWN,//������ʾ
    SOUND_SLIP,//ˤ����ʾ

    SOUND_UPGRADE_SOFTWARE,//�����豸���

	SOUND_ASSESSMENT_MODE,//����ģʽ
	SOUND_KEEP_STAND_UP,//�뱣��վ����
	SOUND_START_ASSESSMENT,//��ʼ�������뿪ʼ����
	SOUND_ASSESSMENT_FINISH,//��������

	SOUND_ASSIST_LEVEL_ONE,//һ������
	SOUND_ASSIST_LEVEL_TWO,//��������
	SOUND_ASSIST_LEVEL_THREE,//��������
	SOUND_ASSIST_LEVEL_FOUR,//�ĵ�����
	SOUND_ASSIST_LEVEL_FIVE,//�嵲����
    
    SOUND_RESIST_LEVEL_ONE,//һ������
	SOUND_RESIST_LEVEL_TWO,//��������
	SOUND_RESIST_LEVEL_THREE,//��������
	SOUND_RESIST_LEVEL_FOUR,//�ĵ�����
	SOUND_RESIST_LEVEL_FIVE,//�嵲����
    SOUND_LIST_NUM
} SOUND_LIST;

typedef enum    //��������
{
    VOICE_PLAY = 0x01,  //����
    VOICE_STOP = 0x02,  //ֹͣ
    VOICE_PAUSE = 0x03,  //��ͣ
    VOICE_RESUME = 0x04,  //�ָ�
    VOICE_STATUS = 0x21,  //оƬ״̬��ѯ
    VOICE_POWER_DOWN = 0x88 //����͹���ģʽ
} SYN6658_CMD;

/*
оƬ���յ���������֡��������λ������1 ���ֽڵ�״̬�ش���
��λ���ɸ�������ش����ж�оƬĿǰ�Ĺ���״̬
*/
typedef enum
{
    SYN6658_INIT = 0x4A,    //��ʼ���ɹ�
    SYN6658_RECEIVE = 0x41,    //�յ���ȷ������֡
    SYN6658_NOT_IDENTIFY = 0x45,    //�յ�����ʶ������֡
    SYN6658_BUSY = 0x4E,    //�յ���״̬��ѯ����֡����оƬ�������ڲ���״̬

    //��һ֡���ݺϳ����Ժ�оƬ�������״̬�ش�0x4F��
    //�����յ���״̬��ѯ����֡����оƬ���ڿ���״̬�ش�0x4F
    SYN6658_IDLE = 0x4F,

} SYN6658_RESPONSE;

typedef enum    //����оƬ�Ĳ���״̬��
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

