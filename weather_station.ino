#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <Sodaq_DS3231.h>
#include <SPI.h>


#include <Fonts\FreeSansBold9pt7b.h>
    
#define TFT_CS 10
#define TFT_RST 9
#define TFT_DC 8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,TFT_DC,TFT_RST);
Adafruit_BME280 bme;

int basetemp,basehum, t,oldt;
double temp;
int oldtemp,oldhum,hum,sezonas,m,oldm,spalva;

int xset = 15,yset = 50;
uint32_t old_ts;
 
String data,val, minut,men,diena,oldval,oldminut,olddiena,metai,oldmetai, dabarMetai;
bool delayTimeChange;
float changeTimer = 0;
uint32_t tempTime;

void setup() {
 rtc.begin();

// DateTime dt(2021,11, 1, 15,57,50, 1);  // pradinis datos/laiko setupas - metai,diena,menuo,val,min,sek,sav d
 //rtc.setDateTime(dt);       //     NEPAMIRST UZKOMENTUOT NUSTACIUS
delayTimeChange = false;
 tft.initR(INITR_BLACKTAB);
 tft.fillScreen (ST7735_LIGHTBLUE);
 tft.setRotation(0);
 
 tft.setFont(&FreeSansBold9pt7b);
  Serial.begin(9600);
 bme.begin(0x76); 
 
 remelis(ST7735_LIGHTGREY);  
 dregsimbolis(xset+100,yset+77);
}

void loop() { 
tft.setTextSize(0);
tft.setFont(&FreeSansBold9pt7b);
  
DateTime now = rtc.now(); 
uint32_t ts = now.getEpoch();
if (old_ts == 0 || old_ts != ts)old_ts = ts;

minut = String(now.minute());
val = String(now.hour());
men = String(now.month());
diena = String(now.date());
metai = String(now.year());

dabarMetai = now.year();
basetemp = bme.readTemperature()*100;
basehum = bme.readHumidity()*100;

basetemp -= basetemp%1000%100%10;
basehum -= basehum%1000%100;

t = basetemp/100;
temp = basetemp;
hum = basehum;

//Serial.println(delayTimeChange);
//Serial.println(changeTimer);

if(now.dayOfWeek() == 7 && now.month() == 10 && now.date() >= 25 && now.hour() == 4 && !delayTimeChange)
{
   DateTime dt(now.year(),10, now.date(), 3,0,0, 7);  // pradinis datos/laiko setupas - metai,diena,menuo,val,min,sek,sav d
   rtc.setDateTime(dt);     
   delayTimeChange = true;
}
else if(now.dayOfWeek() == 7 && now.month() == 3 && now.date() >= 25 && now.hour() == 3 && !delayTimeChange)
{
    DateTime dt(now.year(),3, now.date(), 4,0,0, 7);  // pradinis datos/laiko setupas - metai,diena,menuo,val,min,sek,sav d
    rtc.setDateTime(dt);  
    delayTimeChange = true;
}
if(delayTimeChange)
{
  changeTimer = millis() - tempTime;
  if(changeTimer >= 4000000)
  {
    delayTimeChange = false;
    changeTimer = 0;
  }
}
else
{
  tempTime = millis();
}

//                                            SIMBOLIAI
if(t < 20) tempsimbolismaz20(xset+100,yset+47);
else if(t < 25 && t >= 20)tempsimbolis20(xset+100,yset+47);
else tempsimbolis25(xset+100,yset+47);

if(t >= 35)
{
  ispejimaskarstis(87,88);
}
/*if(t <= 15)
{
  ispejimassaltis(87,89);
}*/
//                                                temp display
if(temp != oldtemp)
{
oldtemp = temp;
tft.fillRect(xset-5 ,yset+35,40,20,ST7735_LIGHTBLUE);
tft.setCursor(xset,yset+50);
tft.print((temp/100),1);
}

//                                    dregnumo display
if(hum != oldhum)
{
oldhum = hum;
tft.fillRect(xset-5 ,yset+65,40,20,ST7735_LIGHTBLUE);
tft.setCursor(xset,yset+80);
tft.print(hum/100);
}
//                        savaites diena, menuo ir diena display
if(diena != olddiena)
{
olddiena = diena;
tft.fillRect(xset-11,yset-5,119,30,ST7735_LIGHTBLUE);
tft.setTextSize(1);
tft.setFont();
tft.setCursor(xset-9,yset+5);
tft.print(savd(now.dayOfWeek()));

if(now.dayOfWeek() == 6)
{
  varnele(xset-6,yset-10);
  varnele(xset+6,yset-8);
}
if(now.dayOfWeek() == 3)
{
  varnele(xset+12,yset-8);
}

tft.setCursor(xset+77,yset+5);
if(now.month()<10)
{
  tft.print("0" + men);
}
else
{
  tft.print(men);
}

slash(xset+90,yset+5);
tft.setCursor(xset+95,yset+5);
if(now.date()<10)
{
  tft.print("0" + diena);
}
else
{
  tft.print(diena);
}

}
//                                  laiko display
if(minut != oldminut)
{
oldminut = minut;
tft.setTextSize(2);
tft.setFont(&FreeSansBold9pt7b);
tft.fillRect(xset-3,yset-40,40,30,ST7735_LIGHTBLUE);
tft.fillRect(xset+62,yset-40,40,30,ST7735_LIGHTBLUE);

tft.setCursor(xset-4,yset-14);
if(now.hour()<10)
{
  val = "0" + val;
}
tft.print(val);

dvitaskis(xset+49,yset-32);

tft.setCursor(xset+61,yset-14);
if(now.minute()<10)
{
  minut = "0" + minut;
}
tft.print(minut);
}

}
//                               simboliai 
void tempsimbolis25(int x, int y)
{ 
  tft.fillCircle(x,y,3,ST7735_RED);
  tft.fillRect(x-1,y-10,3,10,ST7735_RED);
  tft.drawCircle(x-53,y-9,2,ST7735_WHITE);
  tft.setCursor(x-50,y+3);
  tft.print("C");
}
void tempsimbolis20(int x, int y)
{ 
  tft.fillCircle(x,y,3,ST7735_RED);
  tft.fillRect(x-1,y-10,3,2,ST7735_WHITE);
  tft.fillRect(x-1,y-8,3,8,ST7735_RED);
  tft.drawCircle(x-53,y-9,2,ST7735_WHITE);
  tft.setCursor(x-50,y+3);
  tft.print("C");
}
void tempsimbolismaz20(int x, int y)
{ 
  tft.fillCircle(x,y,3,ST7735_RED);
  tft.fillRect(x-1,y-10,3,5,ST7735_WHITE);
  tft.fillRect(x-1,y-5,3,4,ST7735_RED);
  tft.drawCircle(x-53,y-9,2,ST7735_WHITE);
  tft.setCursor(x-50,y+3);
  tft.print("C");
}
void dregsimbolis(int x, int y)
{ 
  tft.fillCircle(x,y,3,ST7735_BLUE);
  tft.fillTriangle(x,y-6,x-2,y-3,x+2,y-3,ST7735_BLUE);
  tft.setCursor(x-52,y+3);
  tft.print("%");
}
void dvitaskis(int x, int y)
{
  tft.fillCircle(x,y,2,ST7735_WHITE);
  tft.fillCircle(x,y+15,2,ST7735_WHITE);
}
String savd(int a)
{
  switch (a)
  {
    case 1 : return "Pirmadienis";
    case 2 : return "Antradienis";
    case 3 : return "Treciadienis";
    case 4 : return "Ketvirtadienis";
    case 5 : return "Penktadienis";
    case 6 : return "Sestadienis";
    case 7 : return "Sekmadienis";
  }
}
void varnele(int x,int y)
{
tft.drawPixel(x+3,y+12,ST7735_WHITE);
tft.drawPixel(x+1,y+12,ST7735_WHITE);
tft.drawPixel(x+2,y+13,ST7735_WHITE);
}
void slash(int x,int y)
{
  tft.drawLine(x+2,y,x,y+6,ST7735_WHITE);  
}
void ispejimaskarstis(int x, int y)
{
    tft.fillTriangle( x, y,x+4,y,x+2,y+7,ST7735_RED );
    tft.fillCircle(x+2,y+11,1,ST7735_RED);
}
void ispejimassaltis(int x, int y)
{
  tft.drawLine(x + 1 ,y + 2,x + 7,y + 8,ST7735_LIGHTGREY);
  tft.drawLine(x + 1 ,y + 8,x + 7,y + 2,ST7735_LIGHTGREY);
  tft.drawFastVLine(x + 4,y+1,9,ST7735_LIGHTGREY);
  tft.drawFastHLine(x ,y + 5,9,ST7735_LIGHTGREY);
  tft.drawCircle(x + 4,y + 5,3,ST7735_LIGHTGREY);
}
void remelis(int spalva)
{
  tft.fillRect(0,0,4,160,spalva);
  tft.fillRect(124,0,128,160,spalva);
  tft.fillRect(4,0,123,4,spalva);
  tft.fillRect(4,156,123,160,spalva);
}
