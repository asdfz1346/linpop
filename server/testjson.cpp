

#include <string>
#include <iostream>
#include <json/json.h>
 
using namespace std;
 


int main()
{
	//这次要做的是解析该字段
	string strJson="{\"name\" : \"Tocy\", \"salary\" : 100, \"msg\" : \"work hard\", \"files\" : [\"1.ts\", \"2.txt\"]}";
	//1.首先定义一个队形的Json对象
	Json::Value JsonVal;
	//2.在定义一个Json阅读器
	Json::Reader JsonRead;
	JsonRead.parse(strJson, JsonVal);
	//3.通过key值访问对应的字段
	
	//解析"name"
	if (JsonVal.isMember("name") && JsonVal["name"].isString())
	{

		//使用时，记得先判断是否是该Json的字段，在判断是否是对应的类型，然后才可以转换成对应的类型
		cout << "JsonVal[name] = " << JsonVal["name"].asString()<< endl;
	}
	//解析"salary"
	if (JsonVal.isMember("salary") && JsonVal["salary"].isInt())
	{
		cout << "JsonVal[salary] = " << JsonVal["salary"].asInt() << endl;
	}
	//解析MSG
	if (JsonVal.isMember("msg") && JsonVal["msg"].isString())
	{
		cout << "JsonVal[msg] = " << JsonVal["msg"].asString() <<  endl;
	}
	//解析file
	if (JsonVal.isMember("files") && JsonVal["files"].isArray())
	{
		Json::Value JsonValChild = JsonVal["files"];
		for (unsigned int i =0 ; i < JsonValChild.size() ; i++)
		{
			cout << "JsonValChild[files] " << i << " is" << JsonValChild[i].asString() << endl;
		}

	}

}
