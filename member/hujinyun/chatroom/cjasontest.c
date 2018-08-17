/*
  Copyright (c) 2009 Dave Gamble
 
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
 
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
 
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/
/* 
#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
 
/* Parse text to JSON, then render back to text, and print! */

/*void doit(char *text)
{
	char *out;cJSON *json;
	
	json=cJSON_Parse(text);
	if (!json) {printf("Error before: [%s]\n",cJSON_GetErrorPtr());}
	else
	{
		out=cJSON_Print(json);
		cJSON_Delete(json);
		printf("%s\n",out);
		free(out);
	}
}
void create_objects()
{
	cJSON *root,*fmt,*img,*thm,*fld;char *out, *out1;int i;	/* declare a few. */
 
	/* Here we construct some JSON standards, from the JSON site. */
	
	/* Our "Video" datatype: */
/*	root=cJSON_CreateObject();	
	cJSON_AddItemToObject(root, "name", cJSON_CreateString("Yuwc"));
	cJSON_AddItemToObject(root, "format", fmt=cJSON_CreateObject());
	cJSON_AddStringToObject(fmt,"type",		"rect");
	cJSON_AddNumberToObject(fmt,"width",		1920);
	cJSON_AddNumberToObject(fmt,"height",		1080);
	cJSON_AddFalseToObject (fmt,"interlace");
	cJSON_AddNumberToObject(fmt,"frame rate",	24);
	cJSON *format = cJSON_GetObjectItem(root,"format");   
	char *name = cJSON_GetObjectItem(root,"name")->valuestring;
	printf("name=[%s]\n", name); 
 
	int framerate = cJSON_GetObjectItem(format,"height")->valueint; 
	printf("%d\n", framerate); 
	char *type = cJSON_GetObjectItem(format,"type")->valuestring;
	printf("%s\n", type); 
	out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);	free(out);
}
int main (int argc, const char * argv[])
{
	char text1[]="{\n\"name\": \"Jack (\\\"Bee\\\") Nimble\", \n\"format\": {\"type\":       \"rect\", \n\"width\":      1920, \n\"height\":     1080, \n\"interlace\":  false,\"frame rate\": 24\n}\n}";	
	//string输出Json格式
	doit(text1);
	create_objects();
}
*/
/*===============================================================
*   Copyright (C) 2015 All rights reserved.
*   
*   文件名称：test_cjson.c
*   创 建 者：Gavin (E-mail:gavinlu1015@gmail.com)
*   创建日期：2015年06月1日
*   描    述：C语言JSON简单用例
*
================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"

void create_cjson(char **out)
{
    cJSON *root, *fmt;

    /* 创建一个json */
    root = cJSON_CreateObject();
    /* 定义json root */
    cJSON_AddItemToObject(root, "USR_INFO", fmt = cJSON_CreateObject());
    /* 成员 int型 */
    cJSON_AddNumberToObject(fmt, "usrid", 7);
    /* 成员 string型 */
    cJSON_AddStringToObject(fmt, "E-mail", "gavinlu1990@gamail.com");
    /* 将JSON转换成string型 */
    *out = cJSON_Print(root);
    /* 删除JSON占用内存 */
    cJSON_Delete(root); 
}

void parse_cjson(char *out)
{
    cJSON *out_json, *root, *format;
    char *parse_result;
    char *E_mail;
    int id;

    /* 将string型JSON进行转换 */
    out_json = cJSON_Parse(out);
    if (!out_json) {
        printf("Error before: [%s]\n",cJSON_GetErrorPtr());
        return;
    } else {
        format = cJSON_GetObjectItem(out_json, "USR_INFO");
        /* 获取usrid、E-mail信息 */
        id = cJSON_GetObjectItem(format, "usrid")->valueint;
        E_mail = cJSON_GetObjectItem(format, "E-mail")->valuestring;
    }

    printf("\n解析结果：\n");
    printf("%d\n", id);
    printf("%s\n", E_mail);
    cJSON_Delete(out_json);
}

int main(int argc, const char *argv[])
{
    char *out = NULL;
    /* 构建json */
    create_cjson(&out);
    printf("构成JSON：\n%s\n",out);    
    /* 解析json */
    parse_cjson(out);

    free(out);

    return 0;
}

/**
 * 编译：
 * gcc test_json.c cJSON.c -o test_json -lm
 * （PS：cJSON.c cJSON.h从下载包提取出来）
 */
