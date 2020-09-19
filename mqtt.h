/*************************************************************************
	> File Name: 	mqtt.h
	> Mail: 		xinexporing@163.com
	> Created Time: 2020年09月05日 星期六 16时40分31秒
 ************************************************************************/

#ifndef NET_PROC_H
#define NET_PROC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "string.h"
#include "unistd.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "fcntl.h"

#include "pthread.h"
#include "MQTTClient.h"
#include "iotJSON.h"

#define MQTT_TOPIC_SIZE     (256)		//订阅和发布主题长度
#define MQTT_BUF_SIZE       (8 * 1024) 	//接收后发送缓冲区大小

#define MQTT_HOST 		"broker.emqx.io"		//ip地址
#define MQTT_PORT 		1883					//端口号
#define MQTT_USER 		"device0001"			//用户名
#define MQTT_PASS 		"device0001"			//密码
#define MQTT_CLIENT_ID 	"2020970001"			//客户端标识

#define Dreg_PubTopic_ID 	0
#define Din_PubTopic_ID 	1
#define Diot_PubTopic_ID 	2
#define Dchat_PubTopic_ID 	3

enum  iot_ctrl_status_t
{
	IOT_STATUS_LOGIN,
	IOT_STATUS_CONNECT,
	IOT_STATUS_DROP,
};

typedef struct{
    enum iot_ctrl_status_t iotstatus;
    char AdminID[5];
    char DeviceID[32];
} iot_device_info_t;
//设备信息结构体

typedef void  (*pMessageArrived_Fun)(void*, int len, void*);

void mqtt_module_init(void);
int mqtt_data_write(char *pbuf, int len, char retain, int topicID);
void mqtt_data_rx_cb(void *pbuf, int len, void *pTopic);
void *cloud_mqtt_thread(void *arg);

#define mDEBUG(fmt, ...)  printf("%s[%s](%d):" fmt,__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif
