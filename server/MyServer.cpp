#include"MyServer.h"

MyServer::MyServer(){}
MyServer::~MyServer(){}

struct ARG {
	MyServer* p;
};

bool sql_alter(std::string S);
MYSQL * sql_conn();
MYSQL_RES *sql_query(std::string S);

void REGISTER(MyServer *pthis,int fd,Json::Value JsonVal);
void GETFRIEND(MyServer *pthis,int fd,Json::Value JsonVal);
void GETGROUP(MyServer *pthis,int fd,Json::Value JsonVal);
void ADDGROUP(MyServer *pthis,int fd,Json::Value JsonVal);
void DELGROUP(MyServer *pthis,int fd,Json::Value JsonVal);
void ADDFRIEND(MyServer *pthis,int fd,Json::Value JsonVal);
void RENAMEGROUP(MyServer *pthis,int fd,Json::Value JsonVal);
void MODIFYPASSWORD(MyServer *pthis,int fd,Json::Value JsonVal);
void DELGROUP(MyServer *pthis,int fd,Json::Value JsonVal);
void MATCHTIPS(MyServer *pthis,int fd,Json::Value JsonVal);
void MOVEFRIEND(MyServer *pthis,int fd,Json::Value JsonVal);
void SEARCHFRIEND(MyServer *pthis,int fd,Json::Value JsonVal);
void LOGIN(MyServer *pthis,int fd,Json::Value JsonVal);
void ADDFRIENDRECVREADY(MyServer *pthis,int fd,Json::Value JsonVal);
void ADDFRIENDSENDREQUEST(MyServer *pthis,int fd,Json::Value JsonVal);
void UPDATEFRIENDSTATUS(MyServer *pthis,int fd,Json::Value JsonVal);
void DELFRIEND(MyServer *pthis,int fd,Json::Value JsonVal);
void UPDATENAME(MyServer *pthis,int fd,Json::Value JsonVal);
void SENDSTRING(MyServer *pthis,int fd,Json::Value JsonVal);
void GETHISTROY(MyServer *pthis,int fd,Json::Value JsonVal);
void SendFile(MyServer *pthis,int fd,Json::Value JsonVal);
template <typename T>
void Debug(T a){
	std::cout << a<<std::endl;
	return ;
}
std::string GetTime(){
	std::string S1 = "select now();";
	MYSQL_RES *result ;
	MYSQL_ROW row;
	result = sql_query(S1);
	row = mysql_fetch_row(result);
	return row[0];
}

bool isJson(std::string c){
	int len = c.length();
	if(len>2&&c[0]=='{'&&c[1]=='"'&&c[len-1]=='}'){
		return true;
	}
	return false;
}

int sendjson(int fd,Json::Value J){
	//Debug(J);
	return send(fd,J.toStyledString().c_str(),strlen(J.toStyledString().c_str()),0);
}

MYSQL * sql_conn(){
	MYSQL *m_conn = mysql_init(NULL);
	MYSQL *m_sql = mysql_real_connect(m_conn,"localhost","root","a123456","linpop",0,NULL,0);
	if(!m_sql){
		printf("connect mysql fail\n");
		return NULL;
	}
	return m_sql;
};
void work(MyServer *pthis,int fd,std::string str){
	Json::Value JsonVal,data,json1;
	//2.在定义一个Json阅读器
	Json::Reader JsonRead;
	JsonRead.parse(str, JsonVal);
	int cmd = JsonVal["Type"].asInt();
	if(cmd == SMT_REGISTER){
		REGISTER(pthis,fd,JsonVal);
	}
	if(cmd == SMT_LOGIN){
		LOGIN(pthis,fd,JsonVal);
	}
	if(cmd == SMT_GETGROUP){
		GETGROUP(pthis,fd,JsonVal);
	}
	if(cmd ==SMT_ADDGROUP){
		ADDGROUP(pthis,fd,JsonVal);
	}
	if(cmd == SMT_ADDFRIEND){
		ADDFRIEND(pthis,fd,JsonVal);
	}
	if(cmd == SMT_MODIFYPASSWORD){
		MODIFYPASSWORD(pthis,fd,JsonVal);
	}
	if(cmd == SMT_DELGROUP){
		DELGROUP(pthis,fd,JsonVal);
	}
	if(cmd ==SMT_RENAMEGROUP){
		RENAMEGROUP(pthis,fd,JsonVal);
	}
	if(cmd == SMT_MATCHTIPS){
		MATCHTIPS(pthis,fd,JsonVal);
	}
	if(cmd == SMT_GETFRIENDLIST){
		GETFRIEND(pthis,fd,JsonVal);
	}
	if(cmd == SMT_MOVEFRIEND){
		MOVEFRIEND(pthis,fd,JsonVal);
	}
	if(cmd == SMT_SEARCHFRIEND){
		SEARCHFRIEND(pthis,fd,JsonVal);
	}
	if(cmd == SMT_ADDFRIENDSENDREQUEST){
		ADDFRIENDSENDREQUEST(pthis,fd,JsonVal);
	}
	if(cmd == SMT_DELFRIEND){
		DELFRIEND(pthis,fd,JsonVal);
	}
	if(cmd == SMT_UPDATEFRIENDSTATUS){
		UPDATEFRIENDSTATUS(pthis,fd,JsonVal);
	}if(cmd ==SMT_UPDATENAME){
		UPDATENAME(pthis,fd,JsonVal);
	}if(cmd==SMT_SENDMESSAGE){
		if(JsonVal["Data"]["MessageType"].asInt() == SCMT_STRING){
			SENDSTRING(pthis,fd,JsonVal);
		}
	}if(cmd == SMT_GETHISTORY){
		GETHISTROY(pthis,fd,JsonVal);
	}if(cmd ==SMT_ADDFRIENDRECVREADY){
		ADDFRIENDRECVREADY(pthis,fd,JsonVal);
	}if(cmd == SFT_RECVFILE){
		SendFile(pthis,fd,JsonVal);
	}
};

void REGISTER(MyServer *pthis,int fd,Json::Value JsonVal){
	Json::Value tData;
	tData = JsonVal["Data"];
	std::string  id,password,tip,name,S1,groupname,head;
	id = tData["Id"].asString();
	groupname = JsonVal["Data"]["Group"].asString();
	password = tData["Password"].asString();
	tip = tData["Tip"].asString();
	name = tData["Name"].asString();
	head = JsonVal["Data"]["Head"].asString();
	S1 = "INSERT INTO userinfo (id,passwd,name,tip)VALUE('";//asdfz1346','a123456','a','fu');";
	S1 = S1 + id +"','";//a123456','a','fu');"
	S1 = S1 + password +"','";//a','fu');"
	S1 = S1 + name +"','";//fu');
	S1 = S1 + tip + "');";
	Json::Value J;
	J["Type"] = SMT_REGISTER;
	if(sql_alter(S1)){
		S1 = "CREATE TABLE groups_";
		S1 +=id;
		S1+="(groupid int PRIMARY KEY auto_increment,groupname VARCHAR(50));";
		sql_alter(S1);

		J["Data"]["Status"]=SST_REGISTER_SUCCESS;
		S1 = "CREATE TABLE friends_";
		S1 +=id;
		S1+="(id VARCHAR(50) PRIMARY KEY,name VARCHAR(50),groups int);";
		sql_alter(S1);

		S1= "insert into groups_"+id+" (groupname) value ('"+groupname+"');";
		sql_alter(S1);
	
	}else{
		J["Data"]["Status"]=SST_REGISTER_FAILED;
	}
	sendjson(fd,J);

}

void LOGIN(MyServer *pthis,int fd,Json::Value JsonVal){
	Json::Value tData ,J;
	tData = JsonVal["Data"];
	std::string  id,password,S1;
	id = tData["Id"].asString();
	password = tData["Password"].asString();
	S1 = "select * from userinfo where id ='";
	S1 = S1 +id + "';";
	MYSQL_RES *result = sql_query(S1);
	J["Type"] = SMT_LOGIN;
	J["Data"]["Status"]= SST_PASSWORD_ERROR;
	if(result!=NULL){
		MYSQL_ROW row = mysql_fetch_row(result);
		if(row!=NULL){
			std::string pswd(row[1]);
			if(pswd==password){
				if(pthis->clientmap[id]!=0){
					J["Data"]["Status"]= SST_LOGIN_REPEAT;
				}
				else{
					J["Data"]["Status"]= SST_LOGIN_SUCCESS;
					J["Data"]["Name"] 	= row[2];
					J["Data"]["ip"]	 	= pthis->ipmap[fd];	 
					J["Data"]["Head"]	="";
					pthis->clientmap[id]=fd;
					pthis->fdtoidmap[fd]=id;
				}
			}
		}
	}
	sendjson(fd,J);
	
}

void GETGROUP(MyServer *pthis,int fd,Json::Value JsonVal){
	std::string id = JsonVal["Data"]["Id"].asString();
	std::string S1 = "select * from groups_";
	S1 +=id+";";
	MYSQL_RES *result = sql_query(S1);
	MYSQL_ROW row ;
	Json::Value J;
	J["Type"]=SMT_GETGROUP;
	J["Data"]["Status"]=SST_GETGROUP_SUCCESS;
	int i=0;
	if(result!=NULL){
		while(row = mysql_fetch_row(result)){
			J["Data"]["Group"][i] = row[1];
			J["Data"]["GroupIndex"][i] = atoi(row[0]);
			i++;
		}
	}
	sendjson(fd,J);
}

void ADDGROUP(MyServer *pthis,int fd,Json::Value JsonVal){
	std::string id = pthis->fdtoidmap[fd];
	//std::string sourceid = pthis->fdtoidmap[fd];
	//insert into groups_sourceid ('groupname') VALUE (' ');
	std::string groupname =JsonVal["Data"]["GroupName"].asString();
	std::string S1 = "insert into groups_" + id + " (groupname)Value('";
	S1 += groupname+"');";
	Json::Value J;
	J["Type"] = SMT_ADDGROUP;
	J["Data"]["Status"] = SST_ADDGROUP_FAILED;
	if(sql_alter(S1)){
		J["Data"]["Status"] = SST_ADDGROUP_SUCCESS;
		J["Data"]["GroupName"] = groupname;
		S1 = "select max(groupid) from groups_"+id+';';
		MYSQL_RES *result;
		MYSQL_ROW row;
		result = sql_query(S1);
		
		if(result!=NULL){
			row = mysql_fetch_row(result);
			if(row[0]!=NULL)
				J["Data"]["GroupIndex"]=atoi(row[0]);
		} 
	}
	sendjson(fd,J);
	
}

void ADDFRIEND(MyServer *pthis,int fd,Json::Value JsonVal){
	std::string id =  JsonVal["Data"]["Id"].asString();
	std::string groupindex = std::to_string(JsonVal["Data"]["GroupIndex"].asInt());
	std::string S1= "select * from userinfo where id = '";//id'";
	std::string sourceid = pthis->fdtoidmap[fd];
	int targetfd = pthis->clientmap[id];
	S1+=id+"';";
	MYSQL_RES *result=NULL;
	MYSQL_ROW row;
	Json::Value J;
	J["Type"] = SMT_ADDFRIEND;
	J["Data"]["Status"] = SST_ADDFRIEND_FAILED;
	result = sql_query(S1);
	if(result!=NULL){
		if(row = mysql_fetch_row(result)){
			J["Data"]["Status"] = SST_ADDFRIEND_SUCCESS;
			J["Data"]["Friend"]["Id"] = id;
			J["Data"]["Friend"]["Name"] = row[2];
			J["Data"]["Friend"]["Head"] = "";
			J["Data"]["Friend"]["Ip"] = "";
			J["Data"]["Friend"]["Online"] = 0;
			if(pthis ->ipmap[targetfd]!=""){
				J["Data"]["Ip"]   = pthis ->ipmap[targetfd];
				J["Data"]["Online"] = 1;
			}
			/*update friends_*/
			//insert into friends_zhangsan (id,name,groups) VALUE ('id','name',groupindex);
			S1 = " insert INTO friends_" + sourceid + " (id,name,groups) VALUE ('" + id + "','" + row[2] + "'," + groupindex + ");";
			sql_alter(S1);
		}
	}else{
		Debug("not regieste");
	}
	sendjson(fd,J);

}

void MODIFYPASSWORD(MyServer *pthis,int fd,Json::Value JsonVal){
	std::string id = JsonVal["Data"]["Id"].asString();
	std::string password = JsonVal["Data"]["Password"].asString();
	std::string S1 = "update userinfo SET passwd = '"+password + "' WhERE id='" +id +"';";
	Json::Value J;
	J["Type"] = SMT_MODIFYPASSWORD;
	if(sql_alter(S1)){
		J["Data"]["Status"] =SST_MODIFYPASSWORD_SUCCESS;
	}else{
		J["Data"]["Status"] =SST_MODIFYPASSWORD_FAILED;
	}
	sendjson(fd,J);
}

void RENAMEGROUP(MyServer *pthis,int fd,Json::Value JsonVal){
	std::string id =  pthis->fdtoidmap[fd];
	int groupindex = JsonVal["Data"]["GroupIndex"].asInt();
	std::string groupname = JsonVal["Data"]["GroupName"].asString();
	std::string S1  = "UPDATE groups_"+ id +" SET groupname = '";
	S1+= groupname+"' WHERE groupid = "+std::to_string(groupindex)+';';
	Json::Value J;
	J["Type"] = SMT_RENAMEGROUP;
	if(sql_alter(S1)){
		J["Data"]["Status"] =SST_RENAMEGROUP_SUCCESS;
		J["Data"]["GroupName"] = groupname;
		J["Data"]["GroupIndex"] = groupindex;
	}else
	{
		J["Data"]["Status"] =SST_RENAMEGROUP_FAILED;
	}
	sendjson(fd,J);
}

void MATCHTIPS(MyServer *pthis,int fd,Json::Value JsonVal){
	std::string id = JsonVal["Data"]["Id"].asString();
	std::string S1 = "SELECT * FROM userinfo WHERE id ='";
	S1 += id +"';";
	
	MYSQL_RES *result;
	MYSQL_ROW row;
	Json::Value J;
	result = sql_query(S1);
	J["Type"] = SMT_MATCHTIPS;
	J["Data"]["Id"] = id;
	J["Data"]["Status"] = SST_MATCHTIPS_FAILED;
	if(result!=NULL){
		if(row = mysql_fetch_row(result)){
			if(row[3]==JsonVal["Data"]["Tip"].asString()){
				J["Data"]["Status"] = SST_MATCHTIPS_SUCCESS;
			}
		}
	}
	sendjson(fd,J);
}

void GETFRIEND(MyServer *pthis,int fd,Json::Value JsonVal){
	std::string sourceid = pthis->fdtoidmap[fd];
	std::string targetid ,targetip;
	int targetfd;
	//select friends_2.id,userinfo.name,friends_2.groups from friends_2,userinfo WHERE userinfo.id = friends_2.id;
	std::string tablename = "friends_"+ sourceid ;
	std::string S1 = "select " + tablename +".id,userinfo.name,"+tablename+".groups from "+tablename+",userinfo WHERE userinfo.id="+tablename+".id;";
	MYSQL_RES *result=NULL;
	MYSQL_ROW row;
	result = sql_query(S1);
	Json::Value J;
	J["Type"]=SMT_GETFRIENDLIST;
	J["Data"]["Status"]=SST_GETFRIENDLIST_SUCCESS;
	if(result!=NULL){
		int i=0;
		while(row = mysql_fetch_row(result)){
			int groupid =atoi(row[2]);
			J["Data"]["Group"][groupid]["GroupIndex"]= groupid;
			if(row[0]!=NULL){
				J["Data"]["Group"][groupid]["Friend"][i]["Id"]= row[0];
				targetid = row[0];
			}
			if(row[1]!=NULL){
				J["Data"]["Group"][groupid]["Friend"][i]["Name"]= row[1];
			}
			J["Data"]["Group"][groupid]["Friend"][i]["Head"]= "";
			J["Data"]["Group"][groupid]["Friend"][i]["Ip"]= "";
			J["Data"]["Group"][groupid]["Friend"][i]["Online"]= 0;
			if(pthis->clientmap[targetid]!=0){
				targetfd = pthis->clientmap[targetid];
				targetip = pthis->ipmap[targetfd];
				J["Data"]["Group"][groupid]["Friend"][i]["Ip"]= targetip;
				J["Data"]["Group"][groupid]["Friend"][i]["Online"]= 1;
			}
			i++;
		}
	}
	sendjson(fd,J);
}

void DELGROUP(MyServer *pthis,int fd,Json::Value JsonVal){
	std::string sourceid = pthis->fdtoidmap[fd];
	int SrcGroupIndex = JsonVal["Data"]["SrcGroupIndex"].asInt();
	int DestGroupIndex = JsonVal["Data"]["DestGroupIndex"].asInt();
	Json::Value J;
	J["Type"] = SMT_DELGROUP;
	J["Data"]["Status"] = SST_DELGROUP_FAILED;
	std::string S1 ="update friends_" + sourceid +" SET groups = "+std::to_string(DestGroupIndex) + " WHERE groups = "+std::to_string(SrcGroupIndex);
	if(sql_alter(S1)){
		J["Data"]["DestGroupIndex"] =JsonVal["Data"]["DestGroupIndex"];
		J["Data"]["SrcGroupIndex"]=JsonVal["Data"]["SrcGroupIndex"];
		S1 = "delete from groups_" + sourceid + " where groupid  = " +std::to_string(SrcGroupIndex) + ";";
		if(sql_alter(S1)){
			J["Data"]["Status"]=SST_DELGROUP_SUCCESS;
		}
	}
	sendjson(fd,J);
}

void MOVEFRIEND(MyServer *pthis,int fd,Json::Value JsonVal){
	std::string sourceid = pthis->fdtoidmap[fd];
	std::string targetid = JsonVal["Data"]["Id"].asString();
	std::string name = JsonVal["Data"]["name"].asString();
	int SrcGroupIndex = JsonVal["Data"]["SrcGroupIndex"].asInt();
	int DestGroupIndex = JsonVal["Data"]["DestGroupIndex"].asInt();
	std::string S1 = "UPDATE friends_"+ sourceid + " SET groups ="+ std::to_string(DestGroupIndex)+ " WHERE id = '"+ targetid +"';";
	Json::Value J;
	J["Type"] = SMT_MOVEFRIEND;
	J["Data"]["Status"] = SST_MOVEFRIEND_FAILED;
	if(sql_alter(S1)){
		J["Data"]["SrcGroupIndex"]  = JsonVal["Data"]["SrcGroupIndex"];
		J["Data"]["DestGroupIndex"]  = JsonVal["Data"]["DestGroupIndex"];
		J["Data"]["Status"] = SST_MOVEFRIEND_SUCCESS;
		J["Data"]["Id"] = targetid;
		J["Data"]["Name"] = name;
	}
	sendjson(fd,J);
}

void SEARCHFRIEND(MyServer *pthis,int fd,Json::Value JsonVal){
	std::string id =  JsonVal["Data"]["Id"].asString();
	std::string S1= "select * from userinfo where id = '";//id'";
	std::string sourceid = pthis->fdtoidmap[fd];
	int targetfd = pthis->clientmap[id];
	S1+=id+"';";
	MYSQL_RES *result=NULL;
	MYSQL_ROW row;
	Json::Value J;
	J["Type"] = SMT_SEARCHFRIEND;
	J["Data"]["Status"] = SST_SEARCHFRIEND_FAILED;
	result = sql_query(S1);
		if(result!=NULL){
		if(row = mysql_fetch_row(result)){
			J["Data"]["Status"] = SST_SEARCHFRIEND_SUCCESS;
			J["Data"]["Friend"]["Id"] = id;
			J["Data"]["Friend"]["Name"] = row[2];
			J["Data"]["Friend"]["Head"] = "";
			J["Data"]["Friend"]["Ip"] = "";
			J["Data"]["Friend"]["Online"] = 0;
			if(pthis->clientmap[id]!=0){
				J["Data"]["Ip"]   = pthis ->ipmap[targetfd];
				J["Data"]["Friend"]["Online"] = 1;
			}
		}
	}
	sendjson(fd,J);
}

void ADDFRIENDRECVREADY(MyServer *pthis,int fd,Json::Value JsonVal){
	pthis->addfriendrecvready[fd] = true;
	//发送离线消息
	std::string id = pthis->fdtoidmap[fd];
	Json::Value J;
	J["Type"] = SMT_RECVMESSAGE;
	J["Data"]["Status"] = SST_RECVMESSAGE_SUCCESS;
	std::string S1 = "SELECT * from histroy WHERE targetid = '"+id +"' AND flag =0";
	MYSQL_RES *result=NULL ;
	MYSQL_ROW row;
	result = sql_query(S1);
	if(result!=NULL){
		int i=0;
		while(row = mysql_fetch_row(result)){
			std::string sourceid = row[0];
			J["Data"]["Message"][i]["Id"]=sourceid;
			J["Data"]["Message"][i]["DateTime"] = row[2];
			J["Data"]["Message"][i]["String"] = row[3];
			J["Data"]["Message"][i]["MessageType"] = SCMT_STRING;
			S1 = "SELECT groups from friends_"+id +" WHERE id = '"+sourceid+"';";
			MYSQL_RES *result2=NULL;
			MYSQL_ROW row2 ;
			result2 = sql_query(S1);
			row2 = mysql_fetch_row(result2);
			J["Data"]["Message"][i]["GroupIndex"] =atoi(row2[0]);
			i++;
		}
		S1 = "UPDATE histroy SET flag = 1 WHERE targetid ='" + id +"';";
		sql_alter(S1);
		sendjson(fd,J);
	}

}

void ADDFRIENDSENDREQUEST(MyServer *pthis,int fd,Json::Value JsonVal){
	std::string sourcename ;
	std::string targetid =  JsonVal["Data"]["Id"].asString();
	int groupindex = JsonVal["Data"]["GroupIndex"].asInt();
	std::string S1= "select * from userinfo where id = '" + targetid+ "';";
	std::string sourceid = pthis->fdtoidmap[fd];
	int targetfd = pthis->clientmap[targetid];
	Json::Value J;
	Json::Value J2;
	MYSQL_RES *result=NULL;
	MYSQL_ROW row;
	result = sql_query(S1);
	if(result!=NULL&&targetid!=sourceid){
		row = mysql_fetch_row(result);
		J["Type"] = SMT_ADDFRIEND;
		J2["Type"] = SMT_ADDFRIEND;
		J["Data"]["GroupIndex"] = groupindex;
		J["Data"]["Friend"]["Id"] = targetid;
		J["Data"]["Friend"]["Name"] = row[2];
		J["Data"]["Friend"]["Head"] = "";
		J["Data"]["Friend"]["Ip"] = "";
		J["Data"]["Friend"]["Online"] = 0;
		S1 = " insert INTO friends_" + sourceid + " (id,name,groups) VALUE ('" + targetid + "','" + row[2] + "'," + std::to_string(groupindex) + ");";
		if(sql_alter(S1))
			J["Data"]["Status"]= SST_ADDFRIEND_SUCCESS;
		else
			J["Data"]["Status"]= SST_ADDFRIEND_FAILED;
		//获取发送方的name
		S1 = "select * from userinfo where id = '" + sourceid+ "';";
		result = sql_query(S1);
		if(result!=NULL){
			row = mysql_fetch_row(result);
			sourcename = row[2];
		}
		S1 = " insert INTO friends_" + targetid + " (id,name,groups) VALUE ('" + sourceid + "','" + sourcename + "',1);";
		
		//如果 添加成功并且对方在线
		if(sql_alter(S1)&&pthis ->ipmap[targetfd]!=""){
				J2["Data"]["Friend"]["Ip"] = pthis ->ipmap[fd];
				J2["Data"]["Friend"]["Id"] = sourceid;
				J2["Data"]["Friend"]["Name"] = sourcename;
				J2["Data"]["GroupIndex"] = 1;
				J2["Data"]["Friend"]["Head"] = "";
				J2["Data"]["Friend"]["Online"] = 1;
				J["Data"]["Friend"]["Ip"] = pthis->ipmap[targetfd];
				J["Data"]["Friend"]["Online"] = 1;
				J2["Data"]["Status"]= SST_ADDFRIEND_SUCCESS;
				sendjson(targetfd,J2);
		}
	}
	sendjson(fd,J);
}

void DELFRIEND(MyServer *pthis,int fd,Json::Value JsonVal){
	int groupindex = JsonVal["Data"]["GroupIndex"].asInt();
	int friendindex = JsonVal["Data"]["FriendIndex"].asInt();
	std::string targetid = JsonVal["Data"]["Id"].asString();
	std::string sourceid = pthis->fdtoidmap[fd];
	Json::Value J;
	Json::Value J2;
	int targetfd =  pthis->clientmap[targetid];
	J["Type"] = SMT_DELFRIEND;
	J["Data"]["Status"] =  SST_DELFRIEND_FAILED;
	//删除发送方的好友
	std::string S1 = "DELETE FROM friends_"+ sourceid+ " Where id = '"+ targetid +"';";
	if(sql_alter(S1)){
		J["Data"]["Status"] =  SST_DELFRIEND_SUCCESS;
		J["Data"]["GroupIndex"] = groupindex;
		J["Data"]["FriendIndex"] = friendindex;
		J["Data"]["IsFriendIndex"] =1;
	}
	//如果对方online 
	if(pthis -> clientmap[targetid]!=0){
		J2["Type"] = SMT_DELFRIEND;
		J2["Data"]["Status"] =  SST_DELFRIEND_FAILED;
		J2["Data"]["IsFriendIndex"] = 0;
		//获取被动删除方的index
		S1 = " SELECT groups FROM friends_"+targetid + " WHERE id = '"+ sourceid +"';";
		MYSQL_RES *result = NULL;
		MYSQL_ROW 	row;
		result = sql_query(S1);
		row = mysql_fetch_row(result);
		int targetgroupindex;
		if(result!=NULL){
			targetgroupindex = atoi(row[0]);
		}
		S1 = "DELETE FROM friends_"+ targetid+ " Where id = '"+ sourceid +"';";
		if(sql_alter(S1)){
			J2["Data"]["Status"] =  SST_DELFRIEND_SUCCESS;
			J2["Data"]["GroupIndex"] = targetgroupindex;
			J2["Data"]["FriendId"] = sourceid;
			J2["Data"]["IsFriendIndex"] =0;
		}
		sendjson(targetfd,J2);
	}
	S1 = "DELETE FROM friends_"+ targetid+ " Where id = '"+ sourceid +"';";
	sql_alter(S1);
	S1 = "DELETE FROM histroy WHERE (sourceid = '"+sourceid+"' AND targetid= '"+targetid +"') OR(sourceid = '";
	S1 += targetid +"' AND targetid = '"+sourceid+"')";
	sendjson(fd,J);
}

void UPDATEFRIENDSTATUS(MyServer *pthis,int fd,Json::Value JsonVal){
	std::string targetid = JsonVal["Data"]["Id"].asString();
	int targetfd = pthis->clientmap[targetid];
	//判断是否在线
	Json::Value J;
	std::string S1 = "SELECT name from userinfo WHERE id='"+targetid+"';";
	MYSQL_RES * result;
	MYSQL_ROW row;
	result = sql_query(S1);
	row = mysql_fetch_row(result);
	J["Type"] = SMT_UPDATEFRIENDSTATUS;
	J["Data"]["FriendIndex"]= JsonVal["Data"]["FriendIndex"];
	J["Data"]["GroupIndex"]= JsonVal["Data"]["GroupIndex"];
	J["Data"]["Status"] = SST_UPDATEFRIENDSTATUS_SUCCESS;
	J["Data"]["Name"]  = row[0];
	J["Data"]["Head"]  = "";
	if(pthis->clientmap[targetid]!=0){
		J["Data"]["Ip"] = pthis->ipmap[targetfd];
		J["Data"]["Online"] = 1;
	}else{
		J["Data"]["Ip"] = "";
		J["Data"]["Online"] = 0;
	}
	sendjson(fd,J);
}

void UPDATENAME(MyServer *pthis,int fd,Json::Value JsonVal){
	std::string name = JsonVal["Data"]["Name"].asString();
	std::string sourceid = pthis->fdtoidmap[fd];
	Json::Value J;
	J["Type"] = SMT_UPDATENAME;
	J["Data"]["Status"] = SST_UPDATENAME_FAILED;
	std::string S1 = "UPDATE userinfo SET name = '"+name+"' WHERE id=  '"+sourceid+"';";
	if(sql_alter(S1)){
		J["Data"]["Status"] = SST_UPDATENAME_SUCCESS;
		J["Data"]["Name"] = JsonVal["Data"]["Name"];
	}
	sendjson(fd,J);

}

void SENDSTRING(MyServer *pthis,int fd,Json::Value JsonVal){
	std::string sourceid = pthis->fdtoidmap[fd];
	std::string targetid = JsonVal["Data"]["Id"].asString();
	std::string message =  JsonVal["Data"]["String"].asString();
	int flag = 0;
	int targetfd = pthis->clientmap[targetid];
	int sourcegroupindex = JsonVal["Data"]["GroupIndex"].asInt();
	int friendindex = JsonVal["Data"]["FriendIndex"].asInt();
	//获取对方的index
	std::string S1 = " SELECT groups FROM friends_"+targetid + " WHERE id = '"+ sourceid +"';";
	MYSQL_RES * result;
	MYSQL_ROW row;
	result = sql_query(S1);
	row = mysql_fetch_row(result);
	int targetgroupindex = atoi(row[0]);
	Json::Value J;
	J["Type"] = SMT_SENDMESSAGE;
	J["Data"]["Status"] = SST_SENDMESSAGE_SUCCESS;
	J["Data"]["MessageType"] = SCMT_STRING;
	J["Data"]["Message"]["String"] = JsonVal["Data"]["String"];
	J["Data"]["GroupIndex"] = JsonVal["Data"]["GroupIndex"];
	J["Data"]["FriendIndex"] = JsonVal["Data"]["FriendIndex"];
	J["Data"]["DateTime"] = GetTime();
	///J["Data"][""]
	sendjson(fd,J);
	//如果对方在线
	if(targetfd!=0){
		Json::Value J2;
		int i = 0;
		J2["Type"] = SMT_RECVMESSAGE;
		J2["Data"]["Status"] = SST_RECVMESSAGE_SUCCESS;
		J2["Data"]["MessageType"] = SCMT_STRING;
		J2["Data"]["Message"][i]["String"] = JsonVal["Data"]["String"].asString();
		J2["Data"]["Message"][i]["Id"] = sourceid;
		J2["Data"]["Message"][i]["GroupIndex"] = targetgroupindex;
		J2["Data"]["Message"][i]["DateTime"] = GetTime();
		sendjson(targetfd,J2);
		flag =1;
	}else
	{
		flag = 0;
	}
	S1 = "INSERT INTO histroy (sourceid,targetid,sendtime,message,flag) VALUE ('";
	S1 += sourceid +"','"+targetid+"',now(),'"+message+"',"+std::to_string(flag)+");";
	sql_alter(S1);
}

void GETHISTROY(MyServer *pthis,int fd,Json::Value JsonVal){
	std::string targetid =  JsonVal["Data"]["Id"].asString();
	std::string sourceid = pthis->fdtoidmap[fd];
	Json::Value J ;
	J["Type"] = SMT_GETHISTORY;
	J["Data"]["GroupIndex"] = JsonVal["Data"]["GroupIndex"] ;
	J["Data"]["FriendIndex"] = JsonVal["Data"]["FriendIndex"] ;
	J["Data"]["Status"] = SST_GETHISTORY_FAILED;

	std::string S1 = "SELECT * FROM histroy WHERE (sourceid = '"+sourceid+"' AND targetid = '"+targetid+"')  or(sourceid = '";
	S1 += targetid +"' AND targetid = '"+sourceid +"');";
	MYSQL_RES *result=NULL;
	MYSQL_ROW  row;
	result = sql_query(S1);
	if(result!=NULL){
		int i=0;
		while(row = mysql_fetch_row(result)){
			J["Data"]["Content"][i]["SourceId"] =  row[0];
			J["Data"]["Content"][i]["TargetId"] =  row[1];
			J["Data"]["Content"][i]["DateTime"] =  row[2];
			J["Data"]["Content"][i]["Message"] =  row[3];
			J["Data"]["Content"][i]["MessageType"]=SCMT_STRING;
			i++;
		}
		J["Data"]["Status"] = SST_GETHISTORY_SUCCESS;
	}
	sendjson(fd,J);
}

void TransFile(MyServer *pthis,int fd,char *c,int rcv_num){
	std::string s;
	s.clear();
	std::ofstream OsWrite;
	FILEINFO F;
	
	if(rcv_num==156&&strlen(pthis->fileinfomap[fd].FileName)<1){
		memcpy(&F,c,sizeof F);
		std::string filename(F.FileName);
		filename = "file/"+filename;
		memcpy(F.FileName,filename.c_str(),100);
		pthis->fileinfomap[fd] = F;
		int length = F.FileLen;
		std::string targetid = F.TargetId;
		for(int i=156,j=0;i<rcv_num;i++,j++){
			s[j] = c[i];
		}
		OsWrite.open(pthis->fileinfomap[fd].FileName);
	}else{
		for(int i=0;i<rcv_num;i++){
			s+= c[i];
		}
		OsWrite.open(pthis->fileinfomap[fd].FileName,std::ofstream::app);
	}
	OsWrite<<s;
	OsWrite.close();
  	return;

}

void SendFile(MyServer *pthis,int fd,Json::Value JsonVal){
	FILEINFO F;
	std::string filename = JsonVal["Data"]["FileName"].asString();
	filename = "file/"+ filename;
	memcpy(F.FileName ,JsonVal["Data"]["FileName"].asString().c_str() ,100);

	std::ifstream srcFile;
	srcFile.open(filename,std::ios::binary);
	if(!srcFile){
		return;
	}
	srcFile.seekg(0, std::ios::end); //设置文件指针到文件流的尾部
	F.FileLen = srcFile.tellg(); //读取文件指针的位置
	srcFile.seekg(0, std::ios::beg);
	//发送头信息
	char snd_buf[156];
	memset(snd_buf,0,156);
	memcpy(snd_buf,&F,156);
	send(fd,snd_buf,156,0);
	
	int haveSend = 0;
	int bufferSize = 0;
	char buffer[SND_BUF_SIZE];
	memset(buffer,0,bufferSize);
	int readLen = 0;
	while(haveSend<F.FileLen&&!srcFile.eof()){
		srcFile.read(buffer,SND_BUF_SIZE);
		while(readLen<SND_BUF_SIZE&&haveSend<F.FileLen){
			readLen=send(fd,&buffer[bufferSize],std::min(SND_BUF_SIZE-bufferSize,F.FileLen-haveSend),0);	
			if(readLen<0){
			readLen = 0;
			}
			bufferSize +=readLen;
			haveSend+=readLen;
		}
		readLen = 0;
		bufferSize = 0;
		Debug(haveSend);
		if(haveSend>20000&&haveSend<22000){
			sleep(5);
		}
		if(haveSend>50000&&haveSend<52000){
			sleep(5);
		}

	}
	Debug("");
	Debug(F.FileLen);
	srcFile.close();
}

MYSQL_RES *sql_query(std::string S){
	MYSQL *m_sql = sql_conn();
	MYSQL_RES *ret;
	if(!mysql_query(m_sql,const_cast<char *>(S.c_str()))){
		ret =  mysql_store_result(m_sql);
	}
	mysql_close(m_sql);
	return ret;

}

bool sql_alter(std::string S){
		MYSQL *m_sql = sql_conn();
		bool ret = mysql_query(m_sql, const_cast<char *>(S.c_str()));
		mysql_close(m_sql);
		return !ret;
	}


/*server_listen(),accept,io,work*/
void MyServer::link_startou(const char *ip,short port){
	if(!server_listen(ip,port)){
		printf("	server_listen error\n");	
	}
	struct ARG arg;
	arg.p = this;
	std::cout << "main threadid = "<<pthread_self()<<std::endl;
	pthread_create(&accept_thread,NULL,accept_thread_proc,&arg);
	
	for(int i=0;i<THREAD_NUM;i++){
		pthread_create(&worker_thread[i],NULL,worker_thread_proc,&arg);
	}

}


/*socket ,bind ,listen,epoll_create,epolladd*/
bool MyServer::server_listen(const char *ip,short port){
	//socket
	listenfd = socket(AF_INET,SOCK_STREAM|SOCK_NONBLOCK,0);
	if(listenfd==-1){
		printf("soket error\n");
		return false;
	}
	int on =1;
	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof on);
	setsockopt(listenfd,SOL_SOCKET,SO_REUSEPORT,&on,sizeof on);

	//bind
	struct sockaddr_in serveraddr;
	memset(&serveraddr,0,sizeof serveraddr);
	serveraddr.sin_family= AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(ip);//点分十进制ip转换成一个长整数
	serveraddr.sin_port = htons(port);//转换为网络字节序
	if(bind(listenfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr))==-1){
		printf("bind error\n");
		return false;
	}
	//listen
	if(listen(listenfd,LISTEN_NUM)==-1){//待测试
		printf("listen() error\n");
		return false;
	}
	//epoll
	epollfd = epoll_create(1);

	struct epoll_event e;
	memset(&e,0,sizeof e);
	e.events = EPOLLIN;
	e.data.fd = listenfd;
	if(epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd,&e)==-1){
		printf("epoll_ctl error;\n");
		return false;
	}
	return true;
}

/*create mewclient ,epoll_ctl*/
void *MyServer::accept_thread_proc(void *args){
	MyServer* pthis = ((ARG*)args)->p;
	while(pthis->nstop){
		pthread_mutex_lock(&pthis->accept_mutex);
		pthread_cond_wait(&pthis->accept_cond,&pthis->accept_mutex);//block

		/*accept already,create newclientfd*/
		struct sockaddr_in clientaddr;
		socklen_t addrlen;
		int newfd = accept(pthis->listenfd,(struct sockaddr*)&clientaddr,&addrlen);
		pthread_mutex_unlock(&pthis->accept_mutex);
		if(newfd==-1) continue;
		/*set newfd sndbuf*/
		int nSndBufferLen = 14*1024*1024;
		int nLen          = sizeof(int);
		setsockopt(newfd,SOL_SOCKET,SO_SNDBUF,(char*)&nSndBufferLen,nLen);

		/*set newfd non-blocking;*/
		int oldflag = fcntl(newfd, F_GETFL, 0);
		int newflag = oldflag | O_NONBLOCK;
		if (fcntl(newfd, F_SETFL, newflag) == -1){
			std::cout << "fcntl error, oldflag = " << oldflag << ", newflag = " << newflag << std::endl;
			continue;
		}
		/*insert newfd to the set*/
		pthread_mutex_lock(&pthis->clientset_mutex);
		pthis->clientset.insert(newfd);
		pthread_mutex_unlock(&pthis->clientset_mutex);
		std::cout << "new client connect fd:"<<newfd<<std::endl;
		struct sockaddr_in clientaddr1;
    	memset(&clientaddr1, 0x00, sizeof(clientaddr1));
		/*insert ip to ipmap*/
     	socklen_t nl=sizeof(clientaddr1);
     	getpeername(newfd,(struct sockaddr*)&clientaddr1,&nl);
     	std::string addr=inet_ntoa(clientaddr1.sin_addr); 
		pthis->ipmap[newfd] = addr;
		Debug(addr);
		
		/*add epoll*/
		struct epoll_event e;
		memset (&e,0,sizeof e);
		e.events = EPOLLIN|EPOLLET;
		e.data.fd = newfd;
		if(epoll_ctl(pthis->epollfd,EPOLL_CTL_ADD,newfd,&e)==-1){
			std::cout << "epoll_ctl error,\n"<<std::endl;
		}

	}
	return NULL;
}

void MyServer::main_loop(){	
	while(nstop){
		struct 	epoll_event ev[1024];
		int n = epoll_wait(epollfd,ev,1024,10);
		if(n<=0)
			continue;	
		int m = std::min(n,1024);
		for(int i=0;i<m;i++){
			if(ev[i].data.fd == listenfd)
				pthread_cond_signal(&accept_cond);
			else if(ev[i].events&(EPOLLIN | EPOLLERR)){
				pthread_mutex_lock(&clientset_mutex);
				if(clientset.find(ev[i].data.fd) != clientset.end()){
					pthread_mutex_unlock(&clientset_mutex);
					pthread_mutex_lock(&clientlist_mutex);
					clientlist.push_back(ev[i].data.fd);
					pthread_mutex_unlock(&clientlist_mutex);
				}else
					pthread_mutex_unlock(&clientset_mutex);	
			}
		}
	}
}

void *MyServer::worker_thread_proc(void *args){

	MyServer *pthis =((ARG*)args)->p;
	char rcv_buf[SND_BUF_SIZE];
	char snd_buf[RCV_BUF_SIZE];
	while(pthis->nstop){
		pthread_mutex_lock(&pthis->clientlist_mutex);
		if(!pthis->clientlist.empty()){
			memset(rcv_buf,0,sizeof rcv_buf);
			memset(snd_buf, 0,sizeof snd_buf);
			/*lock clientlist*/

			int fd = pthis->clientlist.front();
			pthis->clientlist.pop_front();
			pthread_mutex_unlock(&pthis->clientlist_mutex);
			
			/*recv msg*/
			int num_rcv=0;
			while((num_rcv=recv(fd,&rcv_buf,sizeof rcv_buf,0))>=0){
				if (num_rcv==0){
					pthread_mutex_lock(&pthis->clientset_mutex);
        			pthis->clientset.erase(fd);
        			pthread_mutex_unlock(&pthis->clientset_mutex);
					std::string id = pthis->fdtoidmap[fd];
					pthis->fdtoidmap[fd]="";
					pthis->clientmap[id]= 0;
					pthis->ipmap[fd] = "";
       				struct epoll_event e;
       				memset (&e,0,sizeof e);
       				e.data.fd = fd;
					e.events=EPOLLIN;
       				epoll_ctl(pthis->epollfd, EPOLL_CTL_DEL, fd,&e);
					pthis->addfriendrecvready[fd] = false;
					memset(pthis->fileinfomap[fd].FileName,0,100);
       				close(fd);
				}
				std::string ctos(rcv_buf);
			if(strlen(pthis->fileinfomap[fd].FileName)<1&&(isJson(ctos)|| pthis->fdtoidmap[fd].length()>0)){
					if(isJson(ctos)){
						work(pthis ,fd, ctos);	
					}else if(ctos[0] == '{')
					{
						pthis->fdjsonmap[fd] =ctos;
					}else{
						std::string s = pthis->fdjsonmap[fd] +=ctos;
						if(isJson(s)){
							work(pthis,fd,s);
							pthis->fdjsonmap[fd].erase();
						}
					}
				}else{
					
					TransFile(pthis,fd,rcv_buf,num_rcv);
				}
				memset(rcv_buf,0,sizeof rcv_buf);
			}
		}
		else {
			pthread_mutex_unlock(&pthis->clientlist_mutex);
			sleep(0.1);
		}
	}
	return NULL;
}	
