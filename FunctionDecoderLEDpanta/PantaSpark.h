//--------------------------------------------------------------------------------
// あらかじめ設定された点灯条件を再生するスケッチ
// [PantaSpark.h]
// Copyright (c) 2020 Ayanosuke(Maison de DCC)
// https://desktopstation.net/bb/index.php
//spd
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//--------------------------------------------------------------------------------
#ifndef SeqLight_h
#define SeqLight_h

  enum{
    FastOn = 1,
    SlowOn,
    TriangleWave,
    Random,
    MarsLight,
    FlashLight,
    SinglePulesStrobe,
    DoublePulseStrobe,
    MediumPulseStrobo,
    FluorescentLamp,
    BrokenFluorescentLamp,
  };

//  enum{
//        OFF = 0,
//        ON,
//  };


class SeqLight
{
public:
  SeqLight(unsigned char ch);
  void stateCheck(uint16_t );
  void OnOff(unsigned char sw);
  
private:
  int state;// = ST_IDLE;
  unsigned char port;
  unsigned char mode;
  char updownFlg;

  unsigned char adr = 0;      // アドレス
  
  float pwmRef =0;
  float deltPwm;            // 10msあたりのpwm増加量
  
  int nowPwm;
  int nextPwm;
  int styTime;

  unsigned char (*ptn)[3];
  
  enum{
    ST_IDLE = 0,
    ST_ON,
    ST_OFF,
    ST_FARST,
    ST_SPARKSEL,
    ST_RUN,
    ST_TIMER,
    ST_WAIT,
  };

//FX効果 パンタスパークデータ
//Cmd,Time,Val
//I:初期状態,O:出力,S:スイープ,L:ループ,E:終了

const unsigned char ptn1[10][3]={
                          {'I', 0,  5},
                          {'O', 1,  5},  
                          {'O', 1,  0},
                          {'O', 1,  5},  
                          {'O', 1,  0},
                          {'O', 1,  5},  
                          {'O', 1,  0},
                          {'O', 3,255},
                          {'O', 1,  0},
                          {'E', 0,  0}};

const unsigned char ptn2[10][3]={
                          {'I', 0,  5},
                          {'O', 1,255},
                          {'O', 1,  0},
                          {'O', 1,255},
                          {'O', 1,  0},
                          {'O', 1,255},
                          {'O', 1,  0},
                          {'O', 3,255},
                          {'O', 1,  0},
                          {'E', 0,  0}};
                          
const unsigned char ptn3[8][3]= {
                          {'I', 0,  5},
                          {'O', 1,  5},  
                          {'O', 1,  0},
                          {'O', 1,  5},  
                          {'O', 1,  0},
                          {'O', 1,  5},  
                          {'O', 1,  0},
                          {'E', 0,  0}};

const unsigned char ptn4[6][3]= {
                          {'I', 0,  5},
                          {'O', 1,  5},  
                          {'O', 1,  0},
                          {'O', 1,  5},  
                          {'O', 1,  0},
                          {'E', 0,  0}};
const unsigned char ptn5[4][3]= {
                          {'I', 0,  5},
                          {'O', 3,255},
                          {'O', 1,  0},
                          {'E', 0,  0}};


//panta spark table(速度による点滅間隔のMinとMax
long mtbl[17][2]={       //Min, Max
                          {  0,   0}, // Speed
                          {300, 400}, // 16
                          {200, 350}, // 32
                          {150, 300}, // 48
                          {100, 250}, // 64
                          { 50, 200}, // 80
                          { 30, 150}, // 96
                          { 25, 100}, // 112
                          { 25,  80}, // 128
                          { 10,  50}, // 144
                          {  5,  30}, // 160
                          {  1,  10}, // 176
                          {  0,   0}, // 192
                          {  0,   0}, // 208
                          {  0,   0}, // 224
                          {  0,   0}, // 240
                          {  0,   0}}; // 256


};

#endif
