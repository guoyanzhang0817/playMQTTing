#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"
#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID1    "ExampleClientSub1"
#define CLIENTID2    "ExampleClientSub2"
#define CLIENTID3    "ExampleClientSub3"
#define CLIENTID4    "ExampleClientSub4"
#define TOPIC1       "aaa"
#define TOPIC2       "bbb"
#define TOPIC3       "ccc"
#define TOPIC4       "ddd"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L
volatile MQTTClient_deliveryToken deliveredtoken;



void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}
int msgarrvd(MQTTClient *p, char *topicName, int topicLen, MQTTClient_message *message)
{
    int i;
    char* payloadptr;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: ");
    payloadptr = message->payload;
    for(i=0; i<message->payloadlen; i++)
    {
        putchar(*payloadptr++);
    }
    putchar('\n');
    if (strcmp(topicName, "aaa") == 0)
    {
	printf("1\n");
        pubmsg.payload = "huawei";
        pubmsg.payloadlen = 7;
        pubmsg.qos = 0;
        pubmsg.retained = 0;
        MQTTClient_publishMessage(*p, "hhh", &pubmsg, &token);
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
    MQTTClient client1;
    MQTTClient client2;
    MQTTClient client3;
    MQTTClient client4;
    MQTTClient_connectOptions conn_opts1 = MQTTClient_connectOptions_initializer;
    MQTTClient_connectOptions conn_opts2 = MQTTClient_connectOptions_initializer;
    MQTTClient_connectOptions conn_opts3 = MQTTClient_connectOptions_initializer;
    MQTTClient_connectOptions conn_opts4 = MQTTClient_connectOptions_initializer;
    int rc;
    int ch;
    MQTTClient_create(&client1, ADDRESS, CLIENTID1, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_create(&client2, ADDRESS, CLIENTID2, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_create(&client3, ADDRESS, CLIENTID3, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_create(&client4, ADDRESS, CLIENTID4, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts1.keepAliveInterval = 20;
    conn_opts1.cleansession = 1;
    conn_opts2.keepAliveInterval = 20;
    conn_opts2.cleansession = 1;
    conn_opts3.keepAliveInterval = 20;
    conn_opts3.cleansession = 1;
    conn_opts4.keepAliveInterval = 20;
    conn_opts4.cleansession = 1;
    MQTTClient_setCallbacks(client1, &client1, connlost, msgarrvd, delivered);
    MQTTClient_setCallbacks(client2, NULL, connlost, msgarrvd, delivered);
    MQTTClient_setCallbacks(client3, NULL, connlost, msgarrvd, delivered);
    MQTTClient_setCallbacks(client4, NULL, connlost, msgarrvd, delivered);

    MQTTClient_connect(client1, &conn_opts1);
    MQTTClient_connect(client2, &conn_opts2);
    MQTTClient_connect(client3, &conn_opts3);
    MQTTClient_connect(client4, &conn_opts4);

    MQTTClient_subscribe(client1, TOPIC1, QOS);
    MQTTClient_subscribe(client2, TOPIC2, QOS);
    MQTTClient_subscribe(client3, TOPIC3, QOS);
    MQTTClient_subscribe(client4, TOPIC4, QOS);
    do
    {
        ch = getchar();
    } while(ch!='Q' && ch != 'q');
    MQTTClient_disconnect(client1, 10000);
    MQTTClient_disconnect(client2, 10000);
    MQTTClient_disconnect(client3, 10000);
    MQTTClient_disconnect(client4, 10000);
    MQTTClient_destroy(&client1);
    MQTTClient_destroy(&client2);
    MQTTClient_destroy(&client3);
    MQTTClient_destroy(&client4);
    return rc;
}

