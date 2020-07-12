#include<mysql/mysql.h>
#include<iostream>
#include <stdio.h>
using namespace std;
int main(){
    MYSQL *m_conn =mysql_init(NULL);
    MYSQL* m_sql;
    m_sql = mysql_real_connect(m_conn,"localhost","root","a123456","linpop",0,NULL,0);
    MYSQL_RES *result;
    MYSQL_ROW row;
    string S1 = "show tables;";
    mysql_query(m_sql,const_cast<char *>(S1.c_str()));
    result 	= mysql_store_result(m_sql);
    while(row = mysql_fetch_row(result)){
        if(row[0][0]=='f'||row[0][0]=='g'){
            S1 = "DROP TABLE "+ string(row[0])+';';
            if(!mysql_query(m_sql,const_cast<char *>(S1.c_str()))){
                cout << row[0];
                cout << "  drop success"<<endl;
            }
        }
    }
    S1 = "DELETE FROM userinfo;";
    if(!mysql_query(m_sql,const_cast<char *>(S1.c_str()))){
        cout << "reset userinfo success"<<endl;
    }
    S1 = "DELETE FROM histroy;";
    if(!mysql_query(m_sql,const_cast<char *>(S1.c_str()))){
        cout << "reset histroy success"<<endl;
    }
    return 0;
}
