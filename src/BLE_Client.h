#ifndef BLE_MYCLIENT_H
#define BLE_MYCLIENT_H



#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#ifdef __cplusplus
extern "C" {
#endif


#define SERVICE_UUID "c7622328-de83-4c4d-957e-b8d51309194b"
#define WINDLASS_CHARACTERISTIC_UUID "f5c63a4c-553a-4b9f-8ece-3ebcd4da2f07"
#define COMMAND_CHARACTERISTIC_UUID "7aa080fe-0ef8-4f0c-987f-74a636dd3a77"

const float penduraChain = 1.0;
const float marginDepth = 1.0;


int scanTime = 5; //In seconds
BLEScan* pBLEScan;
bool molinetFound = false;
bool bleConnected = false;
BLEAdvertisedDevice* molinet;

BLEClient* bleClient;
BLERemoteCharacteristic* commandCharacteristic;
BLERemoteCharacteristic* windlassCharacteristic;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      Serial.println(advertisedDevice.getName().c_str());
        if(advertisedDevice.getName() != ""){
            
            BLEUUID service = advertisedDevice.getServiceUUID();
            if (service.toString() == SERVICE_UUID){
                molinet = new BLEAdvertisedDevice(advertisedDevice);
                molinetFound = true;
                Serial.printf("Found Windlass: %s \n", advertisedDevice.toString().c_str());
            }
        }
      
    }
};

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
    //bleConnected = true;
    Serial.println("onConnect");
  }

  void onDisconnect(BLEClient* pclient) {
    bleConnected = false;
    Serial.println("onDisconnect");
  }
};

static void bleNotifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {

    char cState = pData[0];
    
    windlass_state_e state = windlass_state_e::WLOFF;

    if(cState == 'U'){
        state = windlass_state_e::WLUP;
    }else if (cState == 'D'){
        state = windlass_state_e::WLDOWN;
    }
    char sLength[20];
    memcpy(sLength, (char*)&(pData[1]), length-1);
    sLength[length-1] = 0;

    float rodelength = atof(sLength);

    shipDataModel.navigation.anchor.rode_length.m = rodelength;
    shipDataModel.navigation.anchor.rode_length.age = millis();

    shipDataModel.navigation.anchor.windlass_state.state = state;
    shipDataModel.navigation.anchor.windlass_state.age = millis();


    Serial.print("Notify callback for characteristic ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(" Estat ");
    Serial.print(state);
    Serial.print(" m ");
    Serial.print(rodelength);
    Serial.print(" ");
    Serial.println((char*)pData);
}

void writeWindlassCommand(String command){
    if(commandCharacteristic != nullptr && commandCharacteristic->canWrite()){
        commandCharacteristic->writeValue(command.c_str(), command.length());
    }
}

void windlassDown(){
  writeWindlassCommand("D");
}

void windlassUp(){
  writeWindlassCommand("U");
}

void windlassStop(){
  writeWindlassCommand("S");
}

void windlassGoTo(float l){
  char buffer[10] = "";
  dtostrf(l, 0, 0, &buffer[1]);
  buffer[0]='G';
  if(commandCharacteristic != nullptr && commandCharacteristic->canWrite()){
    Serial.println(buffer);
        commandCharacteristic->writeValue(buffer, strlen(buffer));
    }
}

void fondeixar(){
  float depth = shipDataModel.environment.depth.below_transducer.m;
  float chain = shipDataModel.navigation.anchor.rode_length.m;

  if (chain < (depth + marginDepth)){
    windlassGoTo(depth + marginDepth);
  }
}

void levar(){
  float depth = shipDataModel.environment.depth.below_transducer.m;
  float chain = shipDataModel.navigation.anchor.rode_length.m;
  if (chain > (depth - marginDepth)){
    windlassGoTo(depth - marginDepth);
  }
}

void pendura(){
  windlassGoTo(penduraChain);
}

bool connect_ble(){

    bleClient->connect(molinet);

    Serial.println("Connected to Molinet");
    BLERemoteService* pRemoteService = bleClient->getService(SERVICE_UUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(SERVICE_UUID);
      return false;
    }
    Serial.println(" - Found our service");
    commandCharacteristic = pRemoteService->getCharacteristic(COMMAND_CHARACTERISTIC_UUID);
    if (commandCharacteristic == nullptr) {
      Serial.print("Failed to find characteristic UUID: ");
      Serial.println(COMMAND_CHARACTERISTIC_UUID);
      bleClient->disconnect();
      return false;
    }
    Serial.println(" - Found command characteristic");

    windlassCharacteristic = pRemoteService->getCharacteristic(WINDLASS_CHARACTERISTIC_UUID);
    if (windlassCharacteristic == nullptr) {
      Serial.print("Failed to find characteristic UUID: ");
      Serial.println(WINDLASS_CHARACTERISTIC_UUID);
      bleClient->disconnect();
      return false;
    }
    Serial.println(" - Found windlass characteristic");
    windlassCharacteristic->registerForNotify(bleNotifyCallback);

    const char* buff = windlassCharacteristic->readValue().c_str();
    size_t len = strlen(buff);

    bleNotifyCallback(windlassCharacteristic, (uint8_t*)buff,len, false) ;
    bleConnected = true;
    return true;
}


void setup_ble() {

  if(bleConnected){
     Serial.println("Already connected to  BLE work!");
    return;
  }

  Serial.println("Starting BLE work!");

  BLEDevice::init("");

  Serial.println("Scannig Network");
  
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);
 // wsskClient.c.poll();
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
 // wsskClient.c.poll();
  Serial.println("Scan done!");

  pBLEScan->clearResults(); 

  if (molinetFound){
    Serial.print("I have found ");
    Serial.print(molinet->getName().c_str());
    Serial.print(" at address ");
    Serial.println(molinet->getAddress().toString().c_str());

    bleClient = BLEDevice::createClient();
    bleClient->setClientCallbacks(new MyClientCallback());

    bool done = connect_ble();

  }
}


#ifdef __cplusplus
} /*extern "C"*/
#endif


#endif