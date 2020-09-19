/*************************************************************************
	> File Name: main.c
	> Mail: xinexporing@163.com
	> Created Time: 2020年09月03日 星期四 20时48分17秒
 ************************************************************************/
#ifndef 	__IOTJSON_H_
#define 	__IOTJSON_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "cJSON.h"

/*IoT消息结构体*/
typedef struct DataInterface
{
	int Temperature;
	int Humidity;
	bool HumanInduction;
	bool airConditionStatus;
	struct gpsInfo
	{
		double latitude;
		double longtitude;
	}GPSInfo;
}IOTdataMsg;

#define DISTRICT_SPAN_LENTH	6

extern char *makeJson(struct DataInterface * iotDataMsg);
extern void	parseJson(char * pMsg, cJSON * pSubSensor[], cJSON * pSubGPS[]);

extern int	getTemp(void);
extern int	getHumi(void);
extern bool	getBoolX(void);
extern bool	getBoolY(void);
extern void	getGPS_lat_lng(double *lat, double *lng);

extern char *	printDataMessage(struct DataInterface * DataMsg);
extern IOTdataMsg * createAll_IOTData(void);
extern char *	getAll_IOTdataJson(void);

extern char *	Int2Json_1(char *itemStr, int itemValue);
extern char *	Char2Json_1(char *itemStr, char* itemValue);
extern char *	Char2Json_2(char *itemStr1, char* itemValue1,char *itemStr2, char* itemValue2);
extern char *	Char2Json_3(char *itemStr1, char* itemValue1,char *itemStr2, char* itemValue2,char *itemStr3, char* itemValue3);

extern void		IOTdataMsg_Parser(char *IOTdataJsonMsg);
extern char *	Json2Char(char *JsonStr, char *itemStr);
extern int 		Json2Int(char *JsonStr, char *itemStr);

#ifdef __cplusplus
}
#endif

#endif