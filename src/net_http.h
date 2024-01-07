#ifndef NET_HTTP_H
#define NET_HTTP_H

#ifdef __cplusplus
extern "C" {
#endif

 
  String httpGETRequest(const char* url) {
    HTTPClient http;
    http.begin(url);
    int httpResponseCode = http.GET();

    String payload = "";
    if (httpResponseCode > 0) {
      payload = http.getString();
    }
    http.end();
    return payload;
  }

  String httpPOSTRequest(const char* url, const char* token, const char* body) {
    HTTPClient http;
    http.begin(url);
    if(strlen(token) != 0){
     http.addHeader("Authorization", token);
    }
  
    int httpResponseCode = http.POST(body);

    String payload = "";
    if (httpResponseCode > 0) {
      payload = http.getString();
    }
    http.end();
    return payload;
  }

  String httpPUTRequest(const char* url, const char* token, const char* body) {
    HTTPClient http;
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    if(strlen(token) != 0){
     http.addHeader("Authorization", token);
    }
    int httpResponseCode = http.PUT(body);

    String payload = "";
    if (httpResponseCode > 0) {
      payload = http.getString();
    }
    http.end();
    return payload;
  }

  

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
