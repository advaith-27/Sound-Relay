#include <WiFiManager.h>
#include <WiFi.h>
#include <unordered_map>
#include <WiFiUdp.h>
#include <AsyncUDP.h>
#include <Preferences.h>
#include <iostream>
#include "mbedtls/md.h"
#include <arpa/inet.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

IPAddress multicastIP(239, 134, 233,10);
const uint16_t multicastPort = 4444;
int rando;
String name,pass;
char ret[33];
IPAddress ip;
uint16_t udpPort;
Preferences pref;
AsyncUDP udp;
int num = 0;

struct StringHash {
    std::size_t operator()(const String& s) const {
        return std::hash<std::string>{}(s.c_str());
    }
};

std::unordered_map<String, int,StringHash> ipMap;
std::unordered_map<int, String> numMap;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial){};
  // WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG,0);
  // Serial.println("MEeeow");
  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  rando=10000+esp_random()%90000;
  pref.begin("name-pass",false);
  // pref.putString("name","ESPGlove");
  // pref.putString("pass","love");
  name=pref.getString("name","");
  pass=pref.getString("pass","");
  pinMode(2,OUTPUT);
  while(wm.autoConnect(name.c_str())==false){
    digitalWrite(2,HIGH);
    delay(1000);

    digitalWrite(2,LOW);
    delay(1000);
    Serial.print(".");}
  // Begin WiFi
  // Serial.println("sac");
  // WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(2,HIGH);
    delay(1000);

    digitalWrite(2,LOW);
    delay(1000);
    Serial.print(".");
  }

  digitalWrite(2,HIGH);
  
}


void loop() {
  // put your main code here, to run repeatedly:
  if(udp.listen(5555)){
    udp.onPacket([](AsyncUDPPacket p){
      // if(p.length()<1000){
        String dat=(const char*)p.data();
        dat[p.length()] = '\0';
        if(dat.startsWith("Glove Thingi ping")){
          ip=p.remoteIP();
          uint16_t udpPort = 5555;
          char data[100] ;//= "Glove Thingi response Name:";
          sprintf(data,"Glove Thingi response Name:%s Key=%d",name,rando);
          udp.writeTo((uint8_t *)data,strlen(data),ip,udpPort);
        }
        else if(dat.startsWith("Glove Thingi auth") && strcmp(dat.substring(23,55).c_str(),computeMD5FromString(pass.c_str()+String((std::to_string(rando)).c_str())))==0){
          IPAddress ip=p.remoteIP();
          udpPort = 5555;
          char data[100] ;//= "Glove Thingi auth Pass:1ba4bd159b11a825ef4e4b8a4d0a2b72"
          sprintf(data,"Glove Thingi response sucessfully connected Name:%s %d",name);
          udp.writeTo((uint8_t *)data,strlen(data),ip,udpPort);
          rando=10000+esp_random()%90000;
        }
        else if(dat.startsWith("Glove Thingi send")){
          IPAddress ip=p.remoteIP();
          udpPort = 5555;
          char data[100] ;//= "Glove Thingi auth Pass:1ba4bd159b11a825ef4e4b8a4d0a2b72"
          sprintf(data,"Glove Thingi send %d",++num);
          ipMap[ip.toString()]=num;
          numMap[num]=ip.toString();
          udp.writeTo((uint8_t *)data,strlen(data),ip,udpPort);
        }
        else if(dat.startsWith("Glove Thingi list")){
          IPAddress ip=p.remoteIP();
          udpPort = 5555;
          char data[100] ;//= "Glove Thingi auth Pass:1ba4bd159b11a825ef4e4b8a4d0a2b72"
          sprintf(data,"Glove Thingi list %d",num);
          udp.writeTo((uint8_t *)data,strlen(data),ip,udpPort);
        }
        else if(dat.startsWith("Glove Thingi conn ")){
          int n=dat.substring(18).toInt();
          IPAddress ip;
          ip.fromString(numMap[n]);
          udpPort = 5555;
          char data[100] ;//= "Glove Thingi auth Pass:1ba4bd159b11a825ef4e4b8a4d0a2b72"
          sprintf(data,"Glove Thingi conn %s",p.remoteIP().toString());
          udp.writeTo((uint8_t *)data,strlen(data),ip,udpPort);
        }
      // }
      // else if(ipMap.find(p.remoteIP().toString())!=ipMap.end()){
      //     char data[100] ;
      //   sprintf(data,"Meow:%d",ipMap[ip.toString()]);
      //   udp.writeTo((uint8_t *)data,strlen(data),p.remoteIP(),udpPort);
      //   IPAddress ip=p.remoteIP();
      //   // byte data = p.data();
      //   // udp.beginPacket(multicastIP, multicastPort+ipMap[ip]);
      //   udp.writeTo(p.data(), p.length(),multicastIP,multicastPort+ipMap[ip.toString()]);
      //   // udp.endPacket();
      //   // Serial.printf("Sent multicast packet to %s:%u\n", multicastIP, multicastPort+ipMap[ip.toString()]);
      // }
    });
  } 
  
}

char* computeMD5FromString(const String &str){
    std::vector<uint8_t> result(16);
    mbedtls_md_context_t ctx;
    mbedtls_md_type_t md_type = MBEDTLS_MD_MD5;
    mbedtls_md_init(&ctx);
    mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
    mbedtls_md_starts(&ctx);
    mbedtls_md_update(&ctx, (const unsigned char*)str.c_str(), str.length());
    mbedtls_md_finish(&ctx, result.data());
	  mbedtls_md_free(&ctx);
    for (int i = 0; i < 16; i++){
      sprintf(ret+i*2,"%02x",result.data()[i]);
    }
    ret[32]='\0';

    return ret;
}