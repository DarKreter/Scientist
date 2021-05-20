#include <FastLED.h>
#include <ServoTimer2.h>
#include <SimpleSDAudio.h>

const int PraweSerwoPIN = 26, LeweSerwoPIN = 24, SrodkoweSerwoPIN = 25;
ServoTimer2 LeweSerwo, PraweSerwo, SrodkoweSerwo; int MSwhere = 1480;

const int ILOSC = 36, PIN_LEDOW = 41;
CRGB ledziki[ILOSC];

const int DZIALANIE=0, HISTORIA = 2, REJEWSKI  = 3, ROZYCKI  = 5, ZYGALSKI = 6, ZEBATKI = 29;

boolean Check(int);
void ZaswiecLedy(int,int,int);
void RuchSkrzydel();
void MSM(int);

class LAO
{
  short przelicznik, stan = 0;
public:
  LAO(int x) : przelicznik{x}    {}
  void Swiec()
  {
    int x = random(9);
    if(x > stan)
    {
      for(int i=stan;i<=x;i++)
      {
            ledziki[i + stan*przelicznik] = CRGB(255,0,0);
      }
    }
    if(x < stan)
    {
      for(int i=stan;i>x;i--)
      {
            ledziki[i + stan*przelicznik] = CRGB(0,0,0);
      }
    }
    stan = x;
  }
};
LAO a(0),b(1),c(2),d(3);

void setup() {
SdPlay.init(SSDA_MODE_FULLRATE | SSDA_MODE_MONO | SSDA_MODE_AUTOWORKER); 
SdPlay.setFile("tset.afm");

  pinMode(REJEWSKI,INPUT);
  pinMode(ROZYCKI,INPUT);
  pinMode(DZIALANIE,INPUT);
  pinMode(ZYGALSKI,INPUT);
  pinMode(HISTORIA,INPUT);

  pinMode(ZEBATKI,OUTPUT);
  digitalWrite(ZEBATKI,LOW);

  FastLED.addLeds<NEOPIXEL, PIN_LEDOW>(ledziki, ILOSC);
  ZaswiecLedy(0,0,0);

  LeweSerwo.attach(LeweSerwoPIN); 
  PraweSerwo.attach(PraweSerwoPIN);
  SrodkoweSerwo.attach(SrodkoweSerwoPIN);
  LeweSerwo.write(750);
  PraweSerwo.write(2240);
  SrodkoweSerwo.write(MSwhere);
  
  Serial.begin(9600);
}

void loop() {
  if(Check(HISTORIA))
  {
      SdPlay.setFile("historia.afm");
      RuchSkrzydel();
      RuchSkrzydel();
      SdPlay.play();
      while(!SdPlay.isStopped()) {}
  }
  else if(Check(DZIALANIE))
  {
      SdPlay.setFile("dzialanie.afm");
      RuchSkrzydel();
      RuchSkrzydel();
      SdPlay.play();
while(!SdPlay.isStopped()) {}
  }
  else if(Check(ROZYCKI))
  {
      MSM(2250);
      SdPlay.setFile("rozycki.afm");
      RuchSkrzydel();
      RuchSkrzydel();
      SdPlay.play();
      while(!SdPlay.isStopped()) {a.Swiec();b.Swiec();c.Swiec();d.Swiec();delay(10);}
  }
  else if(Check(ZYGALSKI))
  {
      MSM(1480);
      SdPlay.setFile("zygalski.afm");
      RuchSkrzydel();
      RuchSkrzydel();
      SdPlay.play();
      while(!SdPlay.isStopped()) {a.Swiec();b.Swiec();c.Swiec();d.Swiec();delay(10);}
  }
  else if(Check(REJEWSKI))
  {
      MSM(770);
      SdPlay.setFile("rejewski.afm");
      RuchSkrzydel();
      RuchSkrzydel();
      SdPlay.play();
      while(!SdPlay.isStopped()) {a.Swiec();b.Swiec();c.Swiec();d.Swiec();delay(10);}
  }
}

void MSM(int x) //MidleSerwoMove
{
  digitalWrite(ZEBATKI, HIGH);
  delay(1000);
  if(MSwhere > x)
  {
    for(int i=MSwhere;i > x;i--){
      SrodkoweSerwo.write(i);
      delay(1);}
  }
  if(MSwhere < x)
  {
    for(int i=x;i <MSwhere;i++){
      SrodkoweSerwo.write(i);
      delay(1);}
  }
  delay(500);
  digitalWrite(ZEBATKI, LOW);
}

void RuchSkrzydel()
{
    for(int i=2240,j=750;j<=2200;j++,i--)
    {
      LeweSerwo.write(j);
      PraweSerwo.write(i);
      delay(4);
    }
    delay(500);
    for(int i = 750,j = 2200; j >=750;i++,j--)
    {
        LeweSerwo.write(j);
       PraweSerwo.write(i);
       delay(4);
    }
    delay(500);
}

void ZaswiecLedy(int r, int g, int b)
{
      for(int i=0;i<ILOSC;i++)
      {
        ledziki[i] = CRGB(r,g,b);
      }
      FastLED.show();
}

boolean Check(int a)
{
  if(analogRead(a)> 500)
      return true;
  return false;
}
