/*
 * None of these callback routines HAVE to be implemented but it makes life a lot easier if they are
 * AutoConnect kicks off the whole process and can be recalled when necessary e.g. if the broker disconnects
 * OnConnect is called when a CONNACK was received. Do not assume that the connection was successful - check it
 * OnSubscribe is called when a subscribe request ccompleted successfuilly
 * OnMessage is called when a publish message was received.
 * OnPubAck is called when a publish was isssued with QOS > 0 and completed successfuilly
 * OnDisconnect is called when the TCP connection to the broker disconnected (for whatever reason)
 */
#include <Arduino.h>
#include "A6Services.h"
#include "A6MQTT.h"

#define BROKER_ADDRESS "test.mosquitto.org"  // public broker
#define BROKER_PORT 1883
extern char topic[];
extern char imei[];
extern A6_MQTT MQTT;

/*
 * This function is called once in main setup
 * OnDisconnect below also calls AutoConnect but it is not coumpulsory
 */
void A6_MQTT::AutoConnect()
{
  if (gsm.connectTCPserver(BROKER_ADDRESS,BROKER_PORT))
  {
    Serial.println("TCP up");
    // connect, no userid, password or Will
    MQTT.waitingforConnack = connect(imei, 0, 0, "", "", 1, 0, 0, 0, "", "");
  }
  else
    Serial.println("TCP down");
}

/*
 * This function ic called upon receiving a CONNACK message
 * Note that you should not assume that the connection was successful - check it!
 */
void A6_MQTT::OnConnect(enum eConnectRC rc)
{
  switch (rc)
  {
    case MQTT.CONNECT_RC_ACCEPTED:
      Serial.println("Connected to broker");
      MQTT._PingNextMillis = millis() + (MQTT._KeepAliveTimeOut*1000) - 2000;
      MQTT.subscribe(1234,topic,MQTT.QOS_0);
     break;
    case MQTT.CONNECT_RC_REFUSED_PROTOCOL:
      Serial.println("Protocol error");
      break;
    case MQTT.CONNECT_RC_REFUSED_IDENTIFIER:
      Serial.println("Identity error");
      break;
  }
}

/*
 * Called if the subscribe request completed OK
 */
void A6_MQTT::OnSubscribe(uint16_t pi)
{
  Serial.print("Subscribed to ");
  Serial.println(pi);
}

/*
 * Called when a piblish message is received.
 */
void A6_MQTT::OnMessage(char *topic,char *message,bool dup, bool ret,A6_MQTT::eQOS qos)
{
  if (dup)
    Serial.print("DUP ");
  if (ret)
    Serial.print("RET ");
  Serial.print("QOS ");
  Serial.println(qos);
  Serial.print("Topic: ");Serial.println(topic);
  Serial.print("Message: ");Serial.println(message);
}

/*
 * This function when the the client published a message with QOS > 0 and received confirmation that
 * publish completed OK
 */
void A6_MQTT::OnPubAck(uint16_t messageid)
{
  Serial.print("Packet ");
  Serial.print(messageid,HEX);
  Serial.println(" Acknowledged");
}

/*
 * Called when the client is dsicinnected from the broker
 * My example choosed to remake tre connection
 */
void A6_MQTT::OnDisconnect()
{
  Serial.println("Server disconnected");
  MQTT.AutoConnect();
}

