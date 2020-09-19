/*************************************************************************
	> File Name: main.c
	> Mail: xinexporing@163.com
	> Created Time: 2020年09月03日 星期四 20时48分17秒
 ************************************************************************/

#include "iotJSON.h"

void IOTdataMsg_Parser(char *IOTdataJsonMsg)
{
	cJSON * pSubSensor[4];
	cJSON * pSubGPS[2];
	//initialize two cJSON *object[]
	for (int i = 0; i < 4; i++)
		pSubSensor[i] = cJSON_CreateObject();
	for (int i = 0; i < 2; i++)
		pSubGPS[i] = cJSON_CreateObject();

	//parse one JsonStr Object into pSubSensor & pSubGPS 
	parseJson(IOTdataJsonMsg, pSubSensor, pSubGPS);
	printf("%d %d %d %d %lf %lf\n", pSubSensor[0]->valueint, pSubSensor[1]->valueint, pSubSensor[2]->valueint, pSubSensor[3]->valueint, pSubGPS[0]->valuedouble, pSubGPS[1]->valuedouble);

}

/* Internal constructor. */
char *	getAll_IOTdataJson(void)
{
	//create iotDataMsg object
	IOTdataMsg * iotDataMsg = createAll_IOTData();
	//create and print one JsonStr object
	char *iotJsonMsg = makeJson(iotDataMsg);
	if (NULL == iotJsonMsg)
	{
		printf("ERROR: pJsonRoot Object creating failed\n");//error happend here
		return NULL;
	}
	free(iotDataMsg); iotDataMsg = NULL;
	return iotJsonMsg;
}

IOTdataMsg * createAll_IOTData(void)
{
	IOTdataMsg * iotDataMsg = (IOTdataMsg *)malloc(sizeof(IOTdataMsg));
	if (NULL == iotDataMsg)
		printf("Error: struct \"iotDataMsg\" malloc failed");
	memset(iotDataMsg, 0, sizeof(IOTdataMsg));

	//update IOTdataMsg by creating rand_value
	iotDataMsg->Temperature			= getTemp();
	iotDataMsg->Humidity			= getHumi();
	iotDataMsg->HumanInduction		= "open";
	iotDataMsg->airConditionStatus	= "close";
	getGPS_lat_lng(&(iotDataMsg->GPSInfo.latitude), &(iotDataMsg->GPSInfo.longtitude));

	return iotDataMsg;
}

char * Int2Json_1(char *itemStr, int itemValue)
{
	cJSON * pJsonRoot = cJSON_CreateObject();
	if (NULL == pJsonRoot)
	{
		printf("ERROR: pJsonRoot Object creating failed\n");
		return NULL;
	}
	//cJSON_AddStringToObject(pJsonRoot, "msg", "hello world");
	cJSON_AddNumberToObject(pJsonRoot, itemStr, itemValue);

	char * JsonStr = cJSON_Print(pJsonRoot);
	if (NULL == JsonStr)
	{
		cJSON_Delete(pJsonRoot);
		return NULL;
	}

	cJSON_Delete(pJsonRoot);
	return JsonStr;
}


char * Char2Json_1(char *itemStr, char* itemValue)
{
	cJSON * pJsonRoot = cJSON_CreateObject();
	if (NULL == pJsonRoot)
	{
		printf("ERROR: pJsonRoot Object creating failed\n");
		return NULL;
	}
	//cJSON_AddStringToObject(pJsonRoot, "msg", "hello world");
	cJSON_AddStringToObject(pJsonRoot, itemStr, itemValue);
	
	char * JsonStr = cJSON_Print(pJsonRoot);
	if (NULL == JsonStr)
	{
		cJSON_Delete(pJsonRoot);
		return NULL;
	}

	cJSON_Delete(pJsonRoot);
	return JsonStr;
}

char *Char2Json_2(char *itemStr1, char* itemValue1,char *itemStr2, char* itemValue2)
{
	cJSON * pJsonRoot = cJSON_CreateObject();
	if (NULL == pJsonRoot)
	{
		printf("ERROR: pJsonRoot Object creating failed\n");
		return NULL;
	}
	//cJSON_AddStringToObject(pJsonRoot, "msg", "hello world");
	cJSON_AddStringToObject(pJsonRoot, itemStr1, itemValue1);
	cJSON_AddStringToObject(pJsonRoot, itemStr2, itemValue2);

	char * JsonStr = cJSON_Print(pJsonRoot);
	if (NULL == JsonStr)
	{
		cJSON_Delete(pJsonRoot);
		return NULL;
	}

	cJSON_Delete(pJsonRoot);
	return JsonStr;
}

char *Char2Json_3(char *itemStr1, char* itemValue1,char *itemStr2, char* itemValue2,char *itemStr3, char* itemValue3)
{
	cJSON * pJsonRoot = cJSON_CreateObject();
	if (NULL == pJsonRoot)
	{
		printf("ERROR: pJsonRoot Object creating failed\n");
		return NULL;
	}
	//cJSON_AddStringToObject(pJsonRoot, "msg", "hello world");
	cJSON_AddStringToObject(pJsonRoot, itemStr1, itemValue1);
	cJSON_AddStringToObject(pJsonRoot, itemStr2, itemValue2);
	cJSON_AddStringToObject(pJsonRoot, itemStr3, itemValue3);

	char * JsonStr = cJSON_Print(pJsonRoot);
	if (NULL == JsonStr)
	{
		cJSON_Delete(pJsonRoot);
		return NULL;
	}

	cJSON_Delete(pJsonRoot);
	return JsonStr;
}

char * makeJson(IOTdataMsg * iotDataMsg)
{
    cJSON * pJsonRoot = cJSON_CreateObject();
    if(NULL == pJsonRoot)
    {
		printf("ERROR: pJsonRoot Object creating failed\n");
        return NULL;
    }
    //cJSON_AddStringToObject(pJsonRoot, "msg", "hello world");
    cJSON_AddNumberToObject(pJsonRoot, "temp",  iotDataMsg->Temperature);
    cJSON_AddNumberToObject(pJsonRoot, "humi",  iotDataMsg->Humidity);
	cJSON_AddBoolToObject(pJsonRoot, "airOpen", iotDataMsg->airConditionStatus);
    cJSON_AddBoolToObject(pJsonRoot, "human",   iotDataMsg->HumanInduction);
    cJSON * pSubJson = cJSON_CreateObject();
    if(NULL == pSubJson)
    {
		cJSON_Delete(pSubJson);
        return NULL;
    }
    cJSON_AddNumberToObject(pSubJson, "lat", iotDataMsg->GPSInfo.latitude);
    cJSON_AddNumberToObject(pSubJson, "lng", iotDataMsg->GPSInfo.longtitude);
    cJSON_AddItemToObject(pJsonRoot, "GPS", pSubJson);

    char * p = cJSON_Print(pJsonRoot);
    if(NULL == p)
    {
        cJSON_Delete(pJsonRoot);
        return NULL;
    }

    cJSON_Delete(pJsonRoot);
    return p;
}

void parseJson(char * pMsg, cJSON * pSubSensor[], cJSON * pSubGPS[])
{
    if(NULL == pMsg)
    {
		printf("ERROR: the str\"pMsg\" argv is NULL\n");
        return;
    }
    cJSON * pJson = cJSON_Parse(pMsg);
	cJSON * pJsonsub = cJSON_Parse(pMsg);
    if(NULL == pJson)
    {
		printf("Error before: [%s]\n", cJSON_GetErrorPtr()); // parse faild, return
		return ;
    }
    // get int from json
	pSubSensor[0] = cJSON_GetObjectItem(pJson, "temp");
	if (NULL == pSubSensor[0])
    {
		printf("Error before: [%s]\n", cJSON_GetErrorPtr()); //get object named "temp" faild
    }
	pSubSensor[1] = cJSON_GetObjectItem(pJson, "humi");
	if (NULL == pSubSensor[1])
    {
		printf("Error before: [%s]\n", cJSON_GetErrorPtr()); //get number from json faild
    }

    // get bool from json
	pSubSensor[2] = cJSON_GetObjectItem(pJson, "airOpen");
	if (NULL == pSubSensor[2])
    {
		printf("Error before: [%s]\n", cJSON_GetErrorPtr());  // get bool from json faild
    }
    // get bool from json
	pSubSensor[3] = cJSON_GetObjectItem(pJson, "human");
	if (NULL == pSubSensor[3])
    {
		printf("Error before: [%s]\n", cJSON_GetErrorPtr());  // get bool from json faild
    }

    // get sub object
	pJsonsub = cJSON_GetObjectItem(pJson, "GPS");
	if (NULL == pJsonsub)
    {
		printf("Error before: [%s]\n", cJSON_GetErrorPtr());  // get sub object faild
    }


	pJson = cJSON_GetObjectItem(pJsonsub, "lat");
	*pSubGPS[0] = *pJson;
	if (NULL == pSubGPS[0])
    {
		printf("Error before: [%s]\n", cJSON_GetErrorPtr());  // get object from subject object faild
    }
	pJsonsub = cJSON_GetObjectItem(pJsonsub, "lng");
	*pSubGPS[1] = *pJsonsub;
	if (NULL == pSubGPS[1])
    {
		printf("Error before: [%s]\n", cJSON_GetErrorPtr());  // get object from subject object faild
    }
	cJSON_Delete(pJson);
} 

char *Json2Char(char *JsonStr, char *itemStr)
{
	cJSON * ItemObject = cJSON_CreateObject();
	if (NULL == JsonStr)
	{
		printf("ERROR: the str\"JsonStr\" argv is NULL\n");
		return NULL;
	}
	cJSON * pJson = cJSON_Parse(JsonStr);
	if (NULL == pJson)
	{
		printf("Error before: [%s]\n", cJSON_GetErrorPtr()); // parse faild, return
		return NULL;
	}
	// get int from json
	ItemObject = cJSON_GetObjectItem(pJson, itemStr);
	char *str = malloc (sizeof(char)*1024);
	memcpy(str, ItemObject->valuestring, (strlen(ItemObject->valuestring) + 1));
	str[(strlen(ItemObject->valuestring) + 1)] = '\0';
	cJSON_Delete(ItemObject);
	return str;
}


int Json2Int(char *JsonStr, char *itemStr)
{
	cJSON * ItemObject = cJSON_CreateObject();
	if (NULL == JsonStr)
	{
		printf("ERROR: the str\"JsonStr\" argv is NULL\n");
		return 0;
	}
	cJSON * pJson = cJSON_Parse(JsonStr);
	if (NULL == pJson)
	{
		printf("Error before: [%s]\n", cJSON_GetErrorPtr()); // parse faild, return
		return 0;
	}
	// get int from json
	ItemObject = cJSON_GetObjectItem(pJson, itemStr);
	int iTemp = ItemObject->valueint;
	cJSON_Delete(ItemObject);
	return iTemp;
}

/* temp---temperature：温度 */
int  getTemp(void)
{
    int randTemp;
    srand((unsigned)time(NULL));
    randTemp = (int)(25 + rand() % 3);
    return randTemp;
}
/* humi---humidity：湿度 */
int getHumi(void)
{
    int randHumi;
    srand((unsigned)time(NULL));
    randHumi = (int)(65 + rand() % 5);
    return randHumi;
}

/* the 1st Get_Rand_Bool */
bool getBoolX(void)
{
    int randInt;
    srand((unsigned)time(NULL));
    randInt = (int)(rand() / 200 + rand() % 3);
    return (bool)((randInt & 1));
}
/* the 2th Get_Rand_Bool */
bool getBoolY(void)
{
    int randInt;
    srand((unsigned)time(NULL));
    randInt = (int)(rand() / 200 + rand() % 3);
    return (bool)(randInt & (1 << 2));
}

/* lat---latitude纬度：,  lng---longtitude：经度*/
void getGPS_lat_lng(double *lat, double *lng)
{
    srand((unsigned)time(NULL));
	*lat = (double)(24 + rand() * DISTRICT_SPAN_LENTH / (double)(RAND_MAX));
	*lng = (double)(110 + rand() * DISTRICT_SPAN_LENTH / (double)(RAND_MAX));
}