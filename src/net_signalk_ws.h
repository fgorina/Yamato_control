#ifndef NET_SIGNALK_WS_H
#define NET_SIGNALK_WS_H

#ifdef __cplusplus
extern "C"
{
#endif

bool reloadVessel = true;


  void onWsEventsCallback(WebsocketsEvent event, String data)
  {
    if (event == WebsocketsEvent::ConnectionOpened)
    {
        Serial.println("Wss Connnection Opened");
       // wsskClient.lastActivity = millis();
        if(reloadVessel){
          Serial.println("Getting vessel values");
          reloadVessel = false;
          getVesselInfo();
      }
    }
    else if (event == WebsocketsEvent::ConnectionClosed)
    {     
        Serial.print("Wss Connnection Closed: ");     
        CloseReason reason = wsskClient.c.getCloseReason();
        Serial.print("Close Reason:"); Serial.println(reason);
        Serial.println(data);
        wsskClient.lastActivity = 0;
    }
    else if (event == WebsocketsEvent::GotPing)
    {
        Serial.println("Wss Got a Ping!");      
    }
    else if (event == WebsocketsEvent::GotPong)
    {
        Serial.println("Wss Got a Pong!");      
    }
    else
    {
         Serial.println("Received unindentified WebSockets event");     
    }
  }

  void onWsMessageCallback(WebsocketsMessage message)
  {
 
      //Serial.print("Got Message: ");
      //Serial.println(message.data());
      wsskClient.lastActivity = millis();
      bool found = signalk_parse_ws(message.data());
      if(!found){
        Serial.println(message.data());
      }
      
  }
  /*
  void ws_signalk_greet(WiFiClient& client) {
    String dataFeed = client.readStringUntil('\n');
    const char* data = "{\"context\": \"*\",\"subscribe\": [{\"path\": \"*\", \"policy\":\"instant\"}]}";
    client.println(data);
    client.flush();

    Serial.println("Sent Subscribe all");
  }
  */

  void ws_setup_signalk_reconnect(WsNetClient &client, const char * host, int port)
  {
    app.onRepeat(7000, [&client, host, port]()
    {
      
      if (!client.c.available()) {
      
        char buff[100];

        client.lastActivity = millis();
        client.c =  WebsocketsClient();
        client.c.onMessage(onWsMessageCallback);
        client.c.onEvent(onWsEventsCallback);
        sprintf(buff, "ws://%s:%d/signalk/v1/stream", host, port);
        Serial.print("Reconnecting to "); Serial.println(buff);
        if (client.c.connect(String(buff))) {
          Serial.println("Ws conection opened");
          client.lastActivity = millis();
          //signalk_greet(client.c);
          
        }else{
          Serial.println("Cannot connect");
        }
      }else{
        Serial.print(".");
      }
      if (client.lastActivity > 0 && (millis() - client.lastActivity) > 1000000) {
        Serial.println("Closing wss connection");
        client.c.close();
        client.lastActivity = 0;
      } });
  }

  /*
    void ws_signalk_subscribe(NetClient& client) {
      app.onAvailable(client.c, [&client]() {
        while (client.c.connected() && client.c.available() > 160 )  { // Very important for performance and responsiveness
          bool found = signalk_parse(client.c);
          if (found) {
            client.lastActivity = millis();
            break;
          }
        }
      });

    }
  */
  void ws_signalk_begin(WsNetClient &skClient, const char *host, int port)
  {

    skClient.c.onMessage(onWsMessageCallback);
    skClient.c.onEvent(onWsEventsCallback);
    ws_setup_signalk_reconnect(skClient, host, port);
    char buff[100];

    sprintf(buff, "ws://%s:%d/signalk/v1/stream", host, port);
    bool ok = skClient.c.connect(String(buff));
    Serial.print("Connecting to "); Serial.print(buff); Serial.print(" Status ");Serial.println(ok);
    
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
