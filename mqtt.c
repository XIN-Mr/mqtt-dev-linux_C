/*************************************************************************
	> File Name:    main.c
	> Mail:         xinexporing@163.com
	> Created Time: 2020年09月07日 星期一 20时48分17秒
 ************************************************************************/
#include "mqtt.h"

typedef struct {
    Network Network;
    Client Client;
    char sub_topic[MQTT_TOPIC_SIZE];		//存放订阅主题
    char pub_topic[4][MQTT_TOPIC_SIZE];		//存放发布主题
    char mqtt_buffer[MQTT_BUF_SIZE];		//发送缓冲区
    char mqtt_read_buffer[MQTT_BUF_SIZE];	//接收缓冲区

    unsigned char willFlag;					
    MQTTPacket_willOptions will;
    char will_topic[MQTT_TOPIC_SIZE];		//存放遗嘱主题

    pMessageArrived_Fun DataArrived_Cb;
}Cloud_MQTT_t;

Cloud_MQTT_t Iot_mqtt;

struct opts_struct {
    char    *clientid;
    int     nodelimiter;
    char    *delimiter;
    enum    QoS qos;
    char    *username;
    char    *password;
    char    *host;
    int     port;
    int     showtopics;
} opts = {
    (char *)"iot-dev", 0, (char *)"\n", QOS0, "admin", "password", (char *)"localhost", 1883, 0
};//初始化连接选项结构体

iot_device_info_t gateway = {
    .iotstatus = IOT_STATUS_LOGIN,
    .AdminID = {"001"},
    .DeviceID = {"0001"}
};//初始化设备信息结构体

void iot_mqtt_init(Cloud_MQTT_t *piot_mqtt) 
{
    memset(piot_mqtt, '\0', sizeof(Cloud_MQTT_t));

    sprintf(piot_mqtt->sub_topic, "D/%s/S/%s/#", gateway.DeviceID, gateway.AdminID);        //将初始化好的订阅主题填到数组中
    printf("subscribe: %s\n", piot_mqtt->sub_topic);

    sprintf(piot_mqtt->pub_topic[0], "S/%s/D/%s/Dreg", gateway.AdminID, gateway.DeviceID);	//将初始化好的发布主题填到数组中
    printf("pub: %s\n", piot_mqtt->pub_topic[0]);
    
    sprintf(piot_mqtt->pub_topic[1], "S/%s/D/%s/Din", gateway.AdminID, gateway.DeviceID);	
    printf("pub: %s\n", piot_mqtt->pub_topic[1]);
    
    sprintf(piot_mqtt->pub_topic[2], "S/%s/D/%s/Diot", gateway.AdminID, gateway.DeviceID);	
    printf("pub: %s\n", piot_mqtt->pub_topic[2]);
   
    sprintf(piot_mqtt->pub_topic[3], "S/%s/D/%s/Dchat", gateway.AdminID, gateway.DeviceID);	
    printf("pub: %s\n", piot_mqtt->pub_topic[3]);

    piot_mqtt->DataArrived_Cb = mqtt_data_rx_cb;		//设置接收到数据回调函数

}
void MQTTMessageArrived_Cb(MessageData* md)
{
    MQTTMessage *message = md->message; 
    MQTTString *topicN = md->topicName;
    Cloud_MQTT_t *piot_mqtt = &Iot_mqtt;

    if (NULL != piot_mqtt->DataArrived_Cb) {
        char topicBuf[MQTT_TOPIC_SIZE];
        char msgBuf[MQTT_BUF_SIZE];
        memcpy(topicBuf, topicN->lenstring.data, topicN->lenstring.len);
        topicBuf[topicN->lenstring.len] = 0;

        memcpy(msgBuf, message->payload, message->payloadlen);
        msgBuf[message->payloadlen] = 0;
        piot_mqtt->DataArrived_Cb((void *)msgBuf, message->payloadlen, (void *)topicBuf);//异步消息体
        
    }
}

int mqtt_device_connect(Cloud_MQTT_t *piot_mqtt)
{
    int rc = 0, ret = 0;

    NewNetwork(&piot_mqtt->Network);

    rc = ConnectNetwork(&piot_mqtt->Network, MQTT_HOST, (int)MQTT_PORT);	
    if (rc != 0) {
        printf("网络连接失败！\n");
        ret = -101;
        goto __END;
    }
    MQTTClient(&piot_mqtt->Client, &piot_mqtt->Network, 1000, piot_mqtt->mqtt_buffer, MQTT_BUF_SIZE, piot_mqtt->mqtt_read_buffer, MQTT_BUF_SIZE);
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;

    if (piot_mqtt->willFlag) {
        data.willFlag = 1;
        memcpy(&data.will, &piot_mqtt->will, sizeof(MQTTPacket_willOptions));
    } else {
        data.willFlag = 0;
    }
    data.MQTTVersion = 3;
    data.clientID.cstring = MQTT_CLIENT_ID;
    data.username.cstring = MQTT_USER;
    data.password.cstring = MQTT_PASS;
    data.keepAliveInterval = 30;
    data.cleansession = 1;
    
    rc = MQTTConnect(&piot_mqtt->Client, &data);
    if (rc) {
        printf("连接MQTT_Broker[%s]失败！\n", (char *)MQTT_HOST);
        printf("rc = %d\n", rc);
        ret = -102;
        goto __END;
    }
    rc = MQTTSubscribe(&piot_mqtt->Client, piot_mqtt->sub_topic, opts.qos, MQTTMessageArrived_Cb);
    if (rc) {
        printf("订阅服务端主题失败\n");
        ret = -105;
        goto __END;
    }
    gateway.iotstatus = IOT_STATUS_CONNECT;
    printf("订阅服务端主题成功，ret = %d\n", rc);

    //发布注册与登录消息
    char *Msg_Dreg = Char2Json_3("devReg", "ping","name","D0001","passwd","123456");
    mqtt_data_write(Msg_Dreg, strlen(Msg_Dreg), 1, Dreg_PubTopic_ID);					
    //printf("[D0001]-> %s\n", Msg_Dreg);
    
__END:
    return ret;
}

int mqtt_device_disconnect(Cloud_MQTT_t *piot_mqtt)//断开mqtt连接
{
    int ret = 0;

    ret = MQTTDisconnect(&piot_mqtt->Client);
    printf("已断开MQTT_Broker连接, ret = %d\n", ret);

    return ret;
}

void iot_yield(Cloud_MQTT_t *piot_mqtt)
{
    int ret;

    switch (gateway.iotstatus) {
        case IOT_STATUS_LOGIN:
            ret = mqtt_device_connect(piot_mqtt);
            if (ret < 0) {
                printf("MQTT_Broker连接异常, error code %d\n", ret);
                sleep(1);
            }
            break;
        case IOT_STATUS_CONNECT:
            ret = MQTTYield(&piot_mqtt->Client, 100);
            if (ret != SUCCESS) {
                gateway.iotstatus = IOT_STATUS_DROP;
            }
            break;
        case IOT_STATUS_DROP:
            printf("连接MQTT_Broker失败\n");
            mqtt_device_disconnect(piot_mqtt);
            gateway.iotstatus = IOT_STATUS_LOGIN;
            usleep(1000);
            break;
        default: break;
    }
}

int mqtt_will_msg_set(Cloud_MQTT_t *piot_mqtt, char *pbuf, int len)//设置遗嘱函数
{
    memset(piot_mqtt->will_topic, '\0', MQTT_TOPIC_SIZE);
    MQTTPacket_willOptions mqtt_will = MQTTPacket_willOptions_initializer;

    strcpy(piot_mqtt->will_topic, Iot_mqtt.pub_topic[3]);
    memcpy(&Iot_mqtt.will, &mqtt_will, sizeof(MQTTPacket_willOptions));

    Iot_mqtt.willFlag = 1;
    Iot_mqtt.will.retained = 1;
    Iot_mqtt.will.topicName.cstring = (char *)piot_mqtt->will_topic;
    Iot_mqtt.will.message.cstring = (char *)pbuf;
    Iot_mqtt.will.qos = QOS2;
}

void mqtt_data_rx_cb(void *pbuf, int len, void *ptopic) 
{
    printf("[%s]> %s\n",(unsigned char *)ptopic, (unsigned char *)pbuf);	//打印接收到的数据
}

int mqtt_data_write(char *pbuf, int len, char retain, int topicID)
{
    Cloud_MQTT_t *piot_mqtt = &Iot_mqtt; 
    int ret = 0;
    MQTTMessage message;
    char my_topic[128] = {0};

    strcpy(my_topic, piot_mqtt->pub_topic[topicID]);

    message.payload = (void *)pbuf;
    message.payloadlen = len;
    message.dup = 0;
    message.qos = QOS2;
    if (retain) {
        message.retained = 1;
    } else {
        message.retained = 0;
    }

    ret = MQTTPublish(&piot_mqtt->Client, my_topic, &message);	//发布一个主题

    return ret;
}

void *cloud_mqtt_thread(void *arg)
{
    int ret, len; 
    char will_msg[256];
    sprintf(will_msg, "device[%s] will", gateway.DeviceID);	    //初始化遗嘱数据
    
    iot_mqtt_init(&Iot_mqtt);									//初始化主题
    mqtt_will_msg_set(&Iot_mqtt, will_msg, strlen(will_msg));	//设置遗嘱

    ret = mqtt_device_connect(&Iot_mqtt);						//初始化并连接mqtt服务器
    while (ret < 0) {
        printf("ret = %d\r\n", ret);
        sleep(2);
        ret = mqtt_device_connect(&Iot_mqtt);
    }

    while (1)
    {
        iot_yield(&Iot_mqtt);									//维持服务器稳定，断开重连
    }
}