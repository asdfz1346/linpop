	#ifndef __MYSERVER_H
	#define __MYSERVER_H
	#include<iostream>
	#include<stdio.h>
	#include<stdlib.h>
	#include<pthread.h>
	#include<unistd.h>
	#include<string.h>
	#include<sys/types.h>
	#include<sys/socket.h>
	#include<netinet/in.h>
	#include <arpa/inet.h>  //for htonl() and htons()
	#include <fcntl.h>
	#include <sys/epoll.h>
	#include <signal.h>     //for signal()
	#include <pthread.h>
	#include <semaphore.h>
	#include <errno.h>
	#include <list>
	#include <set>
	#include <time.h>
	#include <sstream>
	#include <deque>
	#include <fstream>
	
	#include <vector>
	#include <iomanip> //for std::setw()/setfill()
	#include <mysql/mysql.h>
	#include <sys/stat.h>
	#include <memory>
	#include<string>
	#include "socket.h"
	#include<mysql/mysql.h>
	#include <json/json.h>

	#define LISTEN_NUM 1
	#define THREAD_NUM 1

	#define MAXSIZE 1000
	#define SND_BUF_SIZE 1024
	#define RCV_BUF_SIZE 56800


	class MyServer{
	public:
		MyServer();
		~MyServer();
		void link_startou(const char*ip,short port);	
		void main_loop();
		std::map<std::string,int> clientmap; // [id] = fd;
		std::map<int,std::string> fdtoidmap; // [fd]= id;
		std::map<int,std::string> ipmap; //[fd] =ip;
		bool addfriendrecvready[1024];
		std::map<int,FILEINFO> 	fileinfomap;
		std::map<int,std::string> fdjsonmap;

	private:
		MyServer(const MyServer& a);//拷贝构造
		MyServer& operator = (const MyServer& a);
		bool server_listen(const char*ip,short port);

		static void* accept_thread_proc(void *args);
		static void* io_thread_proc(void *args);
		static void* worker_thread_proc(void *args);

		pthread_t accept_thread;
		pthread_t io_thread;
		pthread_t worker_thread[THREAD_NUM];

		int listenfd;
		int epollfd;
		int clientfd;


		std::list<int> clientlist;
		std::set<int> clientset;


		pthread_mutex_t accept_mutex 	= PTHREAD_MUTEX_INITIALIZER;
		pthread_mutex_t	clientset_mutex	= PTHREAD_MUTEX_INITIALIZER;
		pthread_mutex_t clientlist_mutex= PTHREAD_MUTEX_INITIALIZER;
		pthread_cond_t 	accept_cond	= PTHREAD_COND_INITIALIZER;
		
		
		bool nstop=true;
	};
	#endif
