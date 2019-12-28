
#include <M5Stack.h>
#include "18x34.c"
#include "35x67.c"
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>

const char server[] = "api.openweathermap.org";
String nameOfCity = "Vienna,AT";
// How your nameOfCity variable would look like for Lagos on Nigeria
//String nameOfCity = "Lagos,NG";

// Replace the next line with your API Key
String apiKey = "a2307508f02b69dc81ce3a0d4bfafd6f";
WiFiClient client;
String text;
int jsonend = 0;
boolean startJson = false;
int status = WL_IDLE_STATUS;            // the Wifi radio's status

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
String formattedDate;
String dayStamp;
String timeStamp;

const uint8_t*n[] = { // vfd font 18x34
  vfd_18x34_0,vfd_18x34_1,vfd_18x34_2,vfd_18x34_3,vfd_18x34_4,
  vfd_18x34_5,vfd_18x34_6,vfd_18x34_7,vfd_18x34_8,vfd_18x34_9
  };
const uint8_t*m[] = { // vfd font 35x67
  vfd_35x67_0,vfd_35x67_1,vfd_35x67_2,vfd_35x67_3,vfd_35x67_4,
  vfd_35x67_5,vfd_35x67_6,vfd_35x67_7,vfd_35x67_8,vfd_35x67_9
  };


char ssid[]     = "sep-guest";
char password[] = "Everybodylovesserpil1";


void setup() 
{
  M5.begin(true, false, true);
  WiFi.begin(ssid, password);
  Serial.begin(115200);
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setBrightness(128);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setTextColor(ORANGE); 
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  timeClient.begin();
  timeClient.setTimeOffset(3600);


}

void loop() 
{
  M5.update();
 if(M5.BtnA.wasPressed()) { 
    Serial.println("Button Pressed");
     makehttpRequest();

  }

 
  display_date();
  M5.Lcd.setCursor(0, 180);
  M5.Lcd.print("Dogus Ural");
  delay(500);
}


void display_date()
{
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  // The formattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  // We need to extract date and time
  formattedDate = timeClient.getFormattedDate();

  // Extract date
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);  
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
  display_dmy(dayStamp,290,0);
  display_smh(timeStamp,256,100);
}
void display_dmy(String stime, uint16_t xpos, uint16_t ypos)
{
  #define X_SPACER 6
  String year,month,day; 
  year = stime.substring(0,4);
  month = stime.substring(5,7);
  day = stime.substring(8,10);
  bool one_digit = false;

  int dig = year.toInt();
  if(dig < 10)
  {
    one_digit = true;
  }
  else
  {
    one_digit = false;
  }
  while(dig > 0)
  {
      byte digit_byte = dig % 10 ;
      showDigit(digit_byte,xpos,ypos);
      xpos = xpos - 38;
      dig = dig /10 ;
      if(one_digit)
      {
        showDigit(0,xpos,ypos);
        xpos = xpos - 38;
        bool one_digit = false;

      }
  }
  xpos = xpos - X_SPACER;
  M5.Lcd.drawPixel( xpos+35,62, ORANGE);
  M5.Lcd.drawPixel( xpos+36,62, ORANGE);
  M5.Lcd.drawPixel( xpos+36,61, ORANGE);
  M5.Lcd.drawPixel( xpos+35,61, ORANGE);
  xpos = xpos - X_SPACER;
  dig = month.toInt();
  if(dig < 10)
  {
    one_digit = true;
  }
  else
  {
    one_digit = false;
  }
  while(dig > 0)
  {
      byte digit_byte = dig % 10 ;
      showDigit(digit_byte,xpos,ypos);
      xpos = xpos - 38;
      dig = dig /10 ;
      if(one_digit)
      {
        showDigit(0,xpos,ypos);
        xpos = xpos - 38;
        bool one_digit = false;

      }
  }
  
  xpos = xpos - X_SPACER;
  M5.Lcd.drawPixel( xpos+35,62, ORANGE);
  M5.Lcd.drawPixel( xpos+36,62, ORANGE);
  M5.Lcd.drawPixel( xpos+36,61, ORANGE);
  M5.Lcd.drawPixel( xpos+35,61, ORANGE);  
  xpos = xpos - X_SPACER;
  dig = day.toInt();
  if(dig < 10)
  {
    one_digit = true;
  }
  else
  {
    one_digit = false;
  }
  while(dig > 0)
  {
      byte digit_byte = dig % 10 ;
      showDigit(digit_byte,xpos,ypos);
      xpos = xpos - 38;
      dig = dig /10 ;
      if(one_digit)
      {
        showDigit(0,xpos,ypos);
        xpos = xpos - 38;
        bool one_digit = false;

      }
  }
  
}

void display_smh(String stime, uint16_t xpos, uint16_t ypos)
{
  String hour,minute,second; 
  hour = timeStamp.substring(0,2);
  minute = timeStamp.substring(3,5);
  second = timeStamp.substring(6,8);
  bool one_digit = false;
  int dig = second.toInt();
  if(dig < 10 && dig > 0)
  {
    one_digit = true;
  }
  else if(dig == 0)
  {
    showDigit(0,xpos,ypos);
    xpos = xpos - 38;
    showDigit(0,xpos,ypos);
    xpos = xpos - 38;
  }
  else
  {
    one_digit = false;
  }
  while(dig > 0)
  {
      byte digit_byte = dig % 10 ;
      showDigit(digit_byte,xpos,ypos);
      xpos = xpos - 38;
      dig = dig /10 ;
      if(one_digit)
      {
        showDigit(0,xpos,ypos);
        xpos = xpos - 38;
        bool one_digit = false;

      }
  }
  xpos = xpos - 10;
  M5.Lcd.drawPixel( xpos+35,133, ORANGE);
  M5.Lcd.drawPixel( xpos+36,133, ORANGE);
  M5.Lcd.drawPixel( xpos+36,132, ORANGE);
  M5.Lcd.drawPixel( xpos+35,132, ORANGE);  
  M5.Lcd.drawPixel( xpos+35,143, ORANGE);
  M5.Lcd.drawPixel( xpos+36,143, ORANGE);
  M5.Lcd.drawPixel( xpos+36,142, ORANGE);
  M5.Lcd.drawPixel( xpos+35,142, ORANGE); 
  xpos = xpos - 10;
  dig = minute.toInt();
  if(dig < 10 && dig > 0)
  {
    one_digit = true;
  }
  else if(dig == 0)
  {
    showDigit(0,xpos,ypos);
    xpos = xpos - 38;
    showDigit(0,xpos,ypos);
    xpos = xpos - 38;
  }
  else
  {
    one_digit = false;
  }
  while(dig > 0)
  {
      byte digit_byte = dig % 10 ;
      showDigit(digit_byte,xpos,ypos);
      xpos = xpos - 38;
      dig = dig /10 ;
      if(one_digit)
      {
        showDigit(0,xpos,ypos);
        xpos = xpos - 38;
        bool one_digit = false;

      }
  }
  
  xpos = xpos - 10;
  M5.Lcd.drawPixel( xpos+35,133, ORANGE);
  M5.Lcd.drawPixel( xpos+36,133, ORANGE);
  M5.Lcd.drawPixel( xpos+36,132, ORANGE);
  M5.Lcd.drawPixel( xpos+35,132, ORANGE);  
  M5.Lcd.drawPixel( xpos+35,143, ORANGE);
  M5.Lcd.drawPixel( xpos+36,143, ORANGE);
  M5.Lcd.drawPixel( xpos+36,142, ORANGE);
  M5.Lcd.drawPixel( xpos+35,142, ORANGE); 
  xpos = xpos - 10;
  dig = hour.toInt();
  if(dig < 10 && dig > 0)
  {
    one_digit = true;
  }
  else if(dig == 0)
  {
    showDigit(0,xpos,ypos);
    xpos = xpos - 38;
    showDigit(0,xpos,ypos);
    xpos = xpos - 38;
  }
  else
  {
    one_digit = false;
  }
  while(dig > 0)
  {
      byte digit_byte = dig % 10 ;
      showDigit(digit_byte,xpos,ypos);
      xpos = xpos - 38;
      dig = dig /10 ;
      if(one_digit)
      {
        showDigit(0,xpos,ypos);
        xpos = xpos - 38;
        bool one_digit = false;

      }
  }
  
}

void showDigit(byte digit, uint16_t xpos, uint16_t ypos)
{
  switch (digit)
  {
    case 0: M5.Lcd.pushImage( xpos,ypos,35,67, (uint16_t *)m[0]); return;
    case 1: M5.Lcd.pushImage( xpos,ypos,35,67, (uint16_t *)m[1]); return;
    case 2: M5.Lcd.pushImage( xpos,ypos,35,67, (uint16_t *)m[2]); return;
    case 3: M5.Lcd.pushImage( xpos,ypos,35,67, (uint16_t *)m[3]); return;
    case 4: M5.Lcd.pushImage( xpos,ypos,35,67, (uint16_t *)m[4]); return;
    case 5: M5.Lcd.pushImage( xpos,ypos,35,67, (uint16_t *)m[5]); return;
    case 6: M5.Lcd.pushImage( xpos,ypos,35,67, (uint16_t *)m[6]); return;
    case 7: M5.Lcd.pushImage( xpos,ypos,35,67, (uint16_t *)m[7]); return;
    case 8: M5.Lcd.pushImage( xpos,ypos,35,67, (uint16_t *)m[8]); return;
    case 9: M5.Lcd.pushImage( xpos,ypos,35,67, (uint16_t *)m[9]); return;
  }
}


void makehttpRequest() {
  // close any connection before send a new request to allow client make connection to server
  client.stop();

  // if there's a successful connection:
  if (client.connect(server, 80)) {
    // Serial.println("connecting...");
    // send the HTTP PUT request:
    client.println("GET /data/2.5/forecast?q=" + nameOfCity + "&APPID=" + apiKey + "&mode=json&units=metric&cnt=2 HTTP/1.1");
    client.println("Host: api.openweathermap.org");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();

    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }

    char c = 0;
    while (client.available()) {
      c = client.read();
      // since json contains equal number of open and close curly brackets, this means we can determine when a json is completely received  by counting
      // the open and close occurences,
      Serial.print(c);
      if (c == '{') {
        startJson = true;         // set startJson true to indicate json message has started
        jsonend++;
      }
      if (c == '}') {
        jsonend--;
      }
      if (startJson == true) {
        text += c;
      }
      // if jsonend = 0 then we have have received equal number of curly braces 
      if (jsonend == 0 && startJson == true) {
        text = "";                // clear text string for the next time
        startJson = false;        // set startJson to false to indicate that a new message has not yet started
      }
    }
  }
  else {
    // if no connction was made:
    Serial.println("connection failed");
    return;
  }
}
