//--------------------------------------------------------------------------------
// あらかじめ設定された点灯条件を再生するスケッチ
// [PantaSpark.cpp]
// Copyright (c) 2020 Ayanosuke(Maison de DCC)
// https://desktopstation.net/bb/index.php
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
// SeqLightクラス
//--------------------------------------------------------------------------------
#include <Arduino.h>
#include "PantaSpark.h"

// コンストラクタ
SeqLight::SeqLight( unsigned char ch)
{
  port = ch;
  pinMode(port, OUTPUT);
  state = ST_IDLE;
}  

void SeqLight::OnOff(unsigned char sw)
{
  static unsigned char presw = 0;

  if(presw == sw) // 変化がなかったら抜ける（ずっとstate書き換えられてしまう）
    return;

  if( sw == 0 )
    state = ST_OFF;
  else if( sw == 1)
    state = ST_ON;

  presw = sw;
}

//---------------------------------------------------------------------
// パンタスパークステートマシン
//---------------------------------------------------------------------
void SeqLight::stateCheck(uint16_t SpeedRef){

  int timeUp =0;
  unsigned int asp = 0;
  unsigned char sparkSel=0;
  static int nextSparkWait = 0;

  switch(state){
    case ST_IDLE: // 0
                break;

     case ST_ON:  // 1
                adr = 0;
                timeUp = 0;
                pwmRef = 0;
                analogWrite( port, pwmRef );          
                state = ST_FARST;
                break;
    case ST_OFF:  // 2
                adr = 0;
                timeUp = 0;
                pwmRef = 0;
                analogWrite( port, 0 );          
                state = ST_IDLE;
                break;
 
    case ST_FARST: // 3 スピードによる点灯間隔算出 max 255
                //asp = SpeedRef >> 4; // 
                asp = SpeedRef / 8;
                if(asp == 0){
                  state = ST_FARST;
                  break;
                }
                nextSparkWait = random(mtbl[asp][0], mtbl[asp][1]);
                state = ST_SPARKSEL;
                break;

    case ST_SPARKSEL: // 4
                sparkSel = random(1,5);
                switch(sparkSel){
                  case 1:
                          ptn = ptn1;
                          break;
                  case 2:
                          ptn = ptn2;
                          break;
                  case 3:
                          ptn = ptn3;
                          break;        
                  case 4:
                          ptn = ptn4;
                          break;
                  case 5:
                          ptn = ptn5;
                          break;
                  default:
                          ptn = ptn1;
                          break;
                }
                adr = 0; 
                state = ST_RUN;
                break;
    case ST_RUN:           // 5 コマンド処理
               if( ptn[adr][0]=='I'){ // I:初期化
                  timeUp = ptn[adr][1];
                  pwmRef = ptn[adr][2];
                  analogWrite(port, (unsigned char)pwmRef); // 0〜255            
                  adr++;
                  state = ST_RUN;
                } else if( ptn[adr][0]=='E'){ // E:end
                  state = ST_WAIT;
                } else if( ptn[adr][0]=='O' ){ // O:出力
                  timeUp = ptn[adr][1];
                  pwmRef = ptn[adr][2];
                  state = ST_TIMER;          
                }
                break;

    case ST_TIMER: //6
                  timeUp--;
                  analogWrite(port, (unsigned char)pwmRef); // 0〜255         
 
                  if( timeUp <= 0 ){
                    adr ++;
                  state = ST_RUN;  //次のコマンドへ
                  }
                  break;
    case ST_WAIT:
                  nextSparkWait--;
                 if(nextSparkWait <= 0){
                    state = ST_FARST;
                }
              break;
    default:
                  break;
  }
}
