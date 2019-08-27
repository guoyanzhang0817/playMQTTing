#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"
#define ADDRESS          "tcp://localhost:1883"
#define CLIENTID         "test_sub_and_pub"
#define TOPIC            "test/subscribe/#"
#define TOPIC1           "test/subscribe/aaa"
#define TOPIC2           "test/subscribe/bbb"
#define TOPIC3           "test/subscribe/ccc"
#define TOPIC4           "test/subscribe/ddd"
#define TOPIC5           "test/publish/aaa"
#define TOPIC6           "test/publish/bbb"
#define TOPIC7           "test/publish/ccc"
#define TOPIC8           "test/publish/ddd"
#define PAYLOAD          "Hello World!"
#define QOS              1
#define TIMEOUT          10000L
volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    MQTTClient *p = (MQTTClient)context;
    int i;
    char* payloadptr;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    printf("Message arrived\n");
    printf("topic: %s\n", topicName);
    printf("message: ");
    payloadptr = message->payload;
    for(i=0; i<message->payloadlen; i++)
    {
        putchar(*payloadptr++);
    }
    putchar('\n');
    if (strcmp(topicName, TOPIC1) == 0)
    {
        pubmsg.payload = "huawei1";
        pubmsg.payloadlen = 8;
        pubmsg.qos = 2;
        pubmsg.retained = 0;
        MQTTClient_publishMessage(*p, TOPIC5, &pubmsg, &token);
	MQTTClient_waitForCompletion(*p, token, TIMEOUT);
    }
    if (strcmp(topicName, TOPIC2) == 0)
    {
	pubmsg.payload = "huawei2";
	pubmsg.payloadlen = 8;
	pubmsg.qos = 2;
	pubmsg.retained = 0;
	MQTTClient_publishMessage(*p, TOPIC6, &pubmsg, &token);
	MQTTClient_waitForCompletion(*p, token, TIMEOUT);
    }
    if (strcmp(topicName, TOPIC3) == 0)
    {
	pubmsg.payload = "huawei3";
	pubmsg.payloadlen = 8;
	pubmsg.qos = 2;
	pubmsg.retained = 0;
	MQTTClient_publishMessage(*p, TOPIC7, &pubmsg, &token);
	MQTTClient_waitForCompletion(*p, token, TIMEOUT);
    }
    if (strcmp(topicName, TOPIC4) == 0)
    {
	pubmsg.payload = "huawei4";
	pubmsg.payloadlen = 8;
	pubmsg.qos = 2;
	pubmsg.retained = 0;
	MQTTClient_publishMessage(*p, TOPIC8, &pubmsg, &token);
	MQTTClient_waitForCompletion(*p, token, TIMEOUT);
    }

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}
void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

int main(int argc, char* argv[])
{
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;
    int ch;
    rc = MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    rc += MQTTClient_setCallbacks(client, &client, connlost, msgarrvd, delivered);
    rc += MQTTClient_connect(client, &conn_opts);
    rc += MQTTClient_subscribe(client, TOPIC, QOS);
    if (rc != MQTTCLIENT_SUCCESS)
    {
	printf("failed to subscribe, return error code %d.\n", rc);
	return -1;
    }
    do
    {
        ch = getchar();
    } while(ch!='Q' && ch != 'q');
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}

