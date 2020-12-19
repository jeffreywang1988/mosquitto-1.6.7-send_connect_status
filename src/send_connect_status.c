#include "mosquitto_broker_internal.h"
#include "tls_mosq.h"

#include <ctype.h>
#include <signal.h>
#include <errno.h>
#include <mosquitto_broker.h>
#include <memory_mosq.h>

#include <config.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <assert.h>

#ifdef WITH_WEBSOCKETS
#  include <libwebsockets.h>
#endif

void send__connect_status(struct mosquitto_db *db, struct mosquitto *context, int type, char* reason)
{	
	if(!IS_VALID_POINTER(context->id)) return;
	if(!IS_VALID_POINTER(db->config->topic_notice_prefix)) return;
	if(strncmp(context->id, db->config->topic_notice_prefix, strlen(db->config->topic_notice_prefix))!=0) return; 
	//log__printf(NULL, MOSQ_LOG_DEBUG, "[topic_notice_prefix]:%s,[context->id]:%s", db->config->topic_notice_prefix, context->id);
	char payload[1024] = {0};
	struct	timeval cur_time;
	long int timestamp = 0;
	gettimeofday(&cur_time,NULL);
	timestamp = (cur_time.tv_sec*1000000+cur_time.tv_usec) / 1000;
	if(IS_VALID_POINTER(reason)){
		snprintf(payload, 1024, "{\"id\":\"%s\",\"s\":\"%d\",\"t\":%ld, \"r\":\"%s\"}", context->id, type, timestamp, reason);
	}else{
		snprintf(payload, 1024, "{\"id\":\"%s\",\"s\":\"%d\",\"t\":%ld}", context->id, type, timestamp);
	}
	//log__printf(NULL, MOSQ_LOG_DEBUG, "[send__connect_status]:%s", payload);
	if(ONTICE_TYPE_ONLINE == type && IS_VALID_POINTER(db->config->topic_notice_online)){
		db__messages_easy_queue(db, context, db->config->topic_notice_online, 0, strlen(payload), &payload, 1, 0, NULL);
	}else if(ONTICE_TYPE_OFFLINE == type && IS_VALID_POINTER(db->config->topic_notice_offline)){
		db__messages_easy_queue(db, context, db->config->topic_notice_offline, 0, strlen(payload), &payload, 1, 0, NULL);
	}
}