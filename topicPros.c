/*************************************************************************
	> File Name: main.c
	> Mail: xinexporing@163.com
	> Created Time: 2020年09月04日 星期五 13时48分17秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SUBTOPIC_NUM  10
#define MAX_SUBTOPIC_SIZE 8

typedef struct TopicParser{
	int subTopicNum;
	char **subTopicStr;
}TopicParser;

char *getTopicName(char *Lenstring)
{
	/* 获取第一个子字符串 */
	char * tmpLength = Lenstring;
	return strtok(tmpLength, "{");
}

TopicParser * getTopicParser(char *TopicStr, char *arraySubTopic[])
{
	const char * s = "/";
	int  subTopicNum = 0;
	TopicParser * topicParser = (TopicParser *)malloc(sizeof(TopicParser));
	if (NULL == topicParser){
		return NULL;
	}
	/* 获取第一个子字符串 */
	char * token = strtok(TopicStr, s);
	/* 继续获取其他的子字符串 */
	while (token != NULL) {
		printf("%s\n", token);
		arraySubTopic[subTopicNum] = token;
		token = strtok(NULL, s);
		subTopicNum++;
	}
	for (int i = 0; i < subTopicNum; i++)
	{
		printf("%s\t", arraySubTopic[i]);
	}
	topicParser->subTopicNum = subTopicNum;
	topicParser->subTopicStr = arraySubTopic;
	return topicParser;
}

void ParseTopicStr(char *TopicStr)
{
	TopicParser *topicParser01;
	char ** arraySubTopic = (char **)malloc(MAX_SUBTOPIC_NUM * sizeof(char *));
	for (int i = 0; i < MAX_SUBTOPIC_NUM; i++)
	{
		arraySubTopic[i] = (char *)malloc(MAX_SUBTOPIC_SIZE * sizeof(char));
	}

	topicParser01 = getTopicParser(TopicStr, arraySubTopic);
	if (NULL == topicParser01)
	{
		printf("ERROR: TOPIC_PARSER MALLOC with NULL\n");
		return ;
	}
	printf("\n%d %s\n", topicParser01->subTopicNum, topicParser01->subTopicStr[1]);

	free(arraySubTopic); arraySubTopic = NULL;
	free(topicParser01); topicParser01 = NULL;
}
int main()
{
	char str[48] = "S001/Diot/D/0001/Temp";
	char lenstr[48] = "S/V001/todev/sensor{\"temp\",27}";
	ParseTopicStr(str);
	printf("%s\n", getTopicName(lenstr));
	return 0;
}