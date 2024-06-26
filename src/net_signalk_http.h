#ifndef NET_SIGNALK_HTTP_H
#define NET_SIGNALK_HTTP_H

#ifdef __cplusplus
extern "C"
{
#endif

  char signalKToken[] = "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJkZXZpY2UiOiJNaWNyb2NvbnNvbGUiLCJpYXQiOjE3MDQzNjA1MjYsImV4cCI6MTczNTkxODEyNn0.RHgBCCUZx02Lt0IdzQJx-_3U8dxUrF_a9JVYA3ujbQ8";

  String to_string(float v)
  {
    char buff[20];
    sprintf(buff, "%f", v);
    return String(buff);
  }
  void getVesselInfo()
  {
    String signalk_http_host = preferences.getString(SK_TCP_HOST_PREF);
    int signalk_http_port = preferences.getInt(SK_TCP_PORT_PREF);
    if (signalk_http_host.length() > 0 && signalk_http_port > 0)
    {
      String api = String("http://") += signalk_http_host += String(":") += String(signalk_http_port) += "/signalk/v1/api/";
      String resp = httpGETRequest((api + "vessels/self/design/beam/value/").c_str());
      if (resp.length() > 0)
      {
        shipDataModel.design.beam.m = resp.toFloat();
        shipDataModel.design.beam.age = millis();
      }
      resp = httpGETRequest((api + "vessels/self/design/airHeight/value/").c_str());
      if (resp.length() > 0)
      {
        shipDataModel.design.air_height.m = resp.toFloat();
        shipDataModel.design.air_height.age = millis();
      }
      resp = httpGETRequest((api + "vessels/self/design/draft/value/maximum").c_str());
      if (resp.length() > 0)
      {
        shipDataModel.design.draft.m = resp.toFloat();
        shipDataModel.design.draft.age = millis();
      }
      resp = httpGETRequest((api + "vessels/self/design/length/value/overall").c_str());
      if (resp.length() > 0)
      {
        shipDataModel.design.length.m = resp.toFloat();
        shipDataModel.design.length.age = millis();
      }
      resp = httpGETRequest((api + "vessels/self/name/").c_str());
      if (resp.length() > 0 && resp.indexOf("<") < 0 && resp.indexOf("=") < 0 && resp.indexOf(":") < 0)
      {
        resp.replace("\"", "");
        strncpy(shipDataModel.vessel.name, resp.c_str(), sizeof(shipDataModel.vessel.name) - 1);
      }
      resp = httpGETRequest((api + "vessels/self/mmsi/").c_str());
      if (resp.length() > 0 && resp.indexOf("<") < 0 && resp.indexOf("=") < 0 && resp.indexOf(":") < 0)
      {
        resp.replace("\"", "");
        strncpy(shipDataModel.vessel.mmsi, resp.c_str(), sizeof(shipDataModel.vessel.mmsi) - 1);
      }
      resp = httpGETRequest((api + "vessels/self/navigation/state/value/").c_str());
      if (resp.length() > 0)
      {
        resp.replace("\"", "");
        set_vessel_nav_state(resp);
      }
    }
  }
  void turnSwitch(String name, float value)
  {

    String signalk_http_host = preferences.getString(SK_TCP_HOST_PREF);
    int signalk_http_port = preferences.getInt(SK_TCP_PORT_PREF);

    String api = "http://" + signalk_http_host + String(":") + String(signalk_http_port) + "/signalk/v1/api/vessels/self/" + name;
    String resp = httpPUTRequest(api.c_str(), signalKToken, ("{\"value\":" + to_string(value) + "}").c_str());
    Serial.print("Updating ");
    Serial.println(api);
    Serial.println(resp);
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
