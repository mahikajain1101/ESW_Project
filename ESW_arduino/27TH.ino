#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>
#include <ThingSpeak.h>

#define MAIN_SSID "OnePlus Nord2 5G"
#define MAIN_PASS "1234567801"

String OM2M_MN = "/~/in-cse/in-name/";
String OM2M_DATA_CONT = "Node-1/Data";
#define INTERVAL 15000L

String CSE_IP = "esw-onem2m.iiit.ac.in";
String CSE_PORT = "443";
String OM2M_ORIGIN = "nbt0aP:2SzUlQ";
String OM2M_AE = "Team-33";

const char * ntpServer = "pool.ntp.org";
float zero_value =0;
String s;
WiFiClient client;
HTTPClient http;

unsigned long myChannelNumber = 1921492;
char* myWriteAPIKey = "2QG9KEOJMC4VWK8W";

long randNumber;
long int prev_millis = 0;
unsigned long epochTime;

const int potPin = 34; 
int potValue = 0;

String data;


unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime( & timeinfo)) {
    //Serial.println("Failed to obtain time");
    return (0);
  }
  time( & now);
  return now;
}

void createCi(String flow_minute)
{
  int i = 0;
  String data;
//  String server = "https://" + String() + cse_ip + ":" + String() + cse_port + String() + "/~/in-cse/in-name/";
  epochTime = getTime();
  String server = "https://" + String() + CSE_IP + ":" + String() + CSE_PORT + String() + OM2M_MN;
  http.begin(server + String() + "Team-33" + "/" + "Node-1/Data" + "/");
  http.addHeader("X-M2M-Origin",OM2M_ORIGIN);
  http.addHeader("Content-Type", "application/json;ty=4");
  http.addHeader("Content-Length", "100");
  data = "[" + String(epochTime) + ", "  + String(flow_minute) + "]";
  String req_data = String() + "{\"m2m:cin\": {"

                    +
                    "\"con\": \"" + data + "\","

                    +
                    "\"lbl\": \"" + "V1.0.0" + "\","

                    +
                    "\"cnf\": \"text\""

                    +
                    "}}";
  int code = http.POST(req_data);
  Serial.println(code);
  if (code == -1)
  {
    Serial.println("UNABLE TO CONNECT TO THE SERVER");
  }
  http.end();
}


void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.begin(MAIN_SSID, MAIN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("#");
  }
  Serial.println("connected");
  configTime(0, 0, ntpServer);
  ThingSpeak.begin(client);
}

void loop() {
  if(zero_value == 0)
  {
    Serial.println("Finding the zero value , please do not connect the device");
    for(int i=0; i<1000; i++)
    {
      float potValue = analogRead(34);
      float voltage=potValue*(3.3/4095.0);
      zero_value += (voltage)*(voltage);
      delay(10);
    }

    zero_value = sqrt(zero_value) / (sqrt(1000));

    Serial.println("Zero value is " + String(zero_value));
    Serial.println("You can now connect the device");
  }

  float temp = 0; 
  float temp1 = 0;

  for(int i=0; i<1000; i++)
  {
    float potValue = analogRead(34);
    float voltage=potValue*(3.3/4095.0);
    temp += (voltage - zero_value)*(voltage- zero_value);
    temp1 += (voltage*voltage);
    delay(10);
  }

    temp = sqrt(temp) / (sqrt(1000));
    temp1 = sqrt(temp1)/sqrt(1000);

    float current = (temp)/0.05;

    Serial.println("Current is " + String(current));


     int n = random(10);
 
 int x = ThingSpeak.writeField(myChannelNumber , 1 , current , myWriteAPIKey );

 if(x == 200)
 {
  Serial.println("Chennel Updated");
 }
 else
 {
  Serial.println("Error code is " + String(x) );
 }
//    String data;
//    String server = "https://" + String() + CSE_IP + ":" + String() + CSE_PORT + String() + OM2M_MN;
//    http.begin(server + String() + OM2M_AE + "/" + String() + OM2M_DATA_CONT + "/");
//    http.addHeader("X-M2M-Origin", OM2M_ORIGIN);
//    http.addHeader("Content-Type", "application/json;ty=4");
//    http.addHeader("Content-Length", "100");
//
//    data = "[" + String(current) + "]"; 
//  String req_data = String() + "{\"m2m:cin\": {"
//
//                    +
//                    "\"con\": \"" + data + "\","
//
//                    +
//                    "\"lbl\": \"" + "V1.0.0" + "\","
//
//                    +
//                    "\"cnf\": \"text\""
//
//                    +
//                    "}}";
//  int code = http.POST(req_data);
//  Serial.println(code);
//     http.end();
     createCi(String(current));
     delay(30000);
}
