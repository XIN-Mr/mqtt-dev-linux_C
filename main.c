/*************************************************************************
	> File Name: main.c
	> Mail: xinexporing@163.com
	> Created Time: 2020年09月07日 星期一 13时48分17秒
 ************************************************************************/
#include "mqtt.h"
#include "unistd.h"

int main(int argc, char *argv[])
{
    pthread_t thread_ID;		//定义线程id

    pthread_create(&thread_ID, NULL, &cloud_mqtt_thread, NULL);	//创建一个线程执行mqtt客户端
    pthread_detach(thread_ID);	//设置线程结束收尸

    while (1) {
        char *Msg_Din = Char2Json_3("devLogin", "ping","name","D0001","passwd","123456");
        mqtt_data_write(Msg_Din, strlen(Msg_Din), 0, Din_PubTopic_ID);
        //printf("[D0001]-> %s\n", Msg_Din);
        char *Msg_Diot = getAll_IOTdataJson();						
        mqtt_data_write(Msg_Diot, strlen(Msg_Diot), 0, Diot_PubTopic_ID);
        //printf("[D0001]-> %s\n", Msg_Diot);
        char *Msg_Dchat= Char2Json_1("welcome", "I am Dev0001！");
        mqtt_data_write(Msg_Dchat, strlen(Msg_Dchat), 0, Dchat_PubTopic_ID);
        //printf("[D0001]-> %s\n", Msg_Dchat);
    }
    return 0;
}
