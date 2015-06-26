//============================================================================
// Name        : seqctl.cpp
// Author      : Tim Shearer
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <csignal>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>

using namespace std;

/* name of the POSIX object referencing the queue */
#define MSGQOBJ_NAME    "/termSeq"
/* max length of a message (just for this process) */
#define MAX_MSG_LEN 512

#define TERM_SEQ_PRIO 10

int queue_init(void) {
	unsigned int msgprio = 0;
	time_t currtime;
	struct mq_attr msgq_attr;
	mqd_t msgq_id;

	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = MAX_MSG_LEN;
	attr.mq_curmsgs = 0;

	msgprio=10;

	/* mq_open() for opening an existing queue */
	msgq_id = mq_open(MSGQOBJ_NAME, O_RDWR);\
	if (msgq_id == (mqd_t)-1) {
		perror("In mq_open()");
		printf("creating queue\n");
		msgq_id = mq_open(MSGQOBJ_NAME, O_RDWR | O_CREAT | O_EXCL, S_IRWXU | S_IRWXG, &attr);
	}

	if (msgq_id == (mqd_t)-1) {
		perror("In mq_open()");
		return -1;
	}
/*

	currtime = time(NULL);
	snprintf(msgcontent, MAX_MSG_LEN, "Hello from process %u (at %s).", my_pid, ctime(&currtime));


	msgsz = mq_send(msgq_id, msgcontent, strlen(msgcontent)+1, msgprio);
	if (msgsz == -1) {
		perror("In send()");
		exit(1);
	}
*/

	/* closing the queue        -- mq_close() */
	mq_close(msgq_id);

	return 0;

}

int queue_send(char *buf) {
	mqd_t msgq_id;
	int res;

	printf("message to send is %s\n", buf);

	if(strlen(buf) > MAX_MSG_LEN) {
		printf("Message too big for queue\n");
		return -1;
	}

	/* mq_open() for opening an existing queue */
	msgq_id = mq_open(MSGQOBJ_NAME, O_RDWR);\
	if (msgq_id == (mqd_t)-1) {
		perror("In mq_open()");
		return -1;
	}

	res = mq_send(msgq_id, buf, strlen(buf)+1, TERM_SEQ_PRIO);
	if (res == -1) {
		perror("In send()");
		exit(1);
	}

	return 0;
}

int main(int argc, char **argv)
{
	char sendbuf[512];
	//init message queue
	if (queue_init() < 0)
		exit(1);

	sendbuf[0] = '\0';
	for(int i = 1 ; i < argc ; i++) {
		//TODO validate commands

		//if message looks ok, send it
		strcat(sendbuf, " ");
		strcat(sendbuf, argv[i]);
	}

	queue_send(sendbuf);

	//TODO wait for response message - different queue, timeout

	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}
