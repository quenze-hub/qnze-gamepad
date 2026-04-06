#include <Wire.h>
#include "Tone.h"
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
#include "font5x7.h"


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define OLED_ADDR 0x3C  // 一般的なSSD1306のアドレス

//const int LEFT_BUTTON_PIN = 5;
//const int RIGHT_BUTTON_PIN = 6;
const int BUZZER_PIN = 9;
const int LEFT_BUTTON_PIN = A0;
const int UP_BUTTON_PIN = A1;
const int DOWN_BUTTON_PIN = A2;
const int RIGHT_BUTTON_PIN = A3;
const int SHOOT_BUTTON_PIN = 2;


/* 効果音（共通） 
#define S_R 1
#define S_A0 2
#define S_B0 3
#define S_C1 4
#define S_D1 5
*/

#define SOUND_PIN1 8
#define SOUND_PIN2 9

Tone tone1;
Tone tone2;


//const char melody[] = { S_D1, S_C1, S_B0, S_A0};
const char melody[] = { NOTE_E1, NOTE_D1, NOTE_C1, NOTE_B0};

// プレイヤー
int playerX = 54;
const int playerY = 56;
const int playerWidth = 8;
const int playerHeight = 5;

// 弾
struct Bullet {
  float x;
  float y;
  bool active;
};

Bullet playerBullet = {0, 0, false};
const int bulletSpeed = 3;
unsigned long lastPlayerShot = 0;
const int playerShootDelay = 50;  // 0.5秒ごとに自動発射

// 敵
const int enemyRows = 3;
const int enemyCols = 8;
const int enemyWidth = 8;
const int enemyHeight = 5;
const int enemySpacing = 4;
struct Enemy {
  float x;
  float y;
  bool alive;
};
Enemy enemies[enemyRows][enemyCols];
float enemyDirection = 1;
float enemySpeed = 0.5;
unsigned long lastEnemyMove = 0;
//const int enemyMoveDelay = 200;
int enemyMoveDelay = 0;
int enemy_pat = 0;


// 敵の弾
const int maxEnemyBullets = 3;
Bullet enemyBullets[maxEnemyBullets];
unsigned long lastEnemyShot = 0;
const int enemyShootDelay = 700;

int score = 0;
bool gameOver = false;
bool gameWin = false;
unsigned long lastButtonPress = 0;
const int buttonDelay = 150;
int alo=60;
unsigned long lastmyMove = 0;
int dflag=0;
int msf=0;
int demo=0;


// 5x8ピクセルのチェック柄データ (1ページ分)
const uint8_t icon[] = { 0x14, 0x0A, 0x0E, 0x0A, 0x14 };
const uint8_t icon4[] = { 0x04, 0x1A, 0x0E, 0x1A, 0x04 };
//const uint8_t icon4[] = { 0xA0, 0x50, 0x70, 0x50, 0xA0 };
const uint8_t icon2[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const uint8_t icon3[] = { 0x00,0x00,0xC0, 0x20, 0x70, 0x20, 0xC0,0x00,0x00, };
const uint8_t tama[] = { 0x00 };

/*
const unsigned char font_7seg[10][5] = {
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // 0
    {0x00, 0x00, 0x7F, 0x00, 0x00}, // 1
    {0x79, 0x49, 0x49, 0x49, 0x4F}, // 2
    {0x49, 0x49, 0x49, 0x49, 0x7F}, // 3
    {0x0F, 0x08, 0x08, 0x08, 0x7F}, // 4
    {0x4F, 0x49, 0x49, 0x49, 0x79}, // 5
    {0x7F, 0x49, 0x49, 0x49, 0x79}, // 6
    {0x01, 0x01, 0x01, 0x01, 0x7F}, // 7
    {0x7F, 0x49, 0x49, 0x49, 0x7F}, // 8
    {0x0F, 0x09, 0x09, 0x09, 0x7F}  // 9
};
*/

const unsigned char font_7seg[10][5] = {
  {0x36, 0x41, 0x41, 0x41, 0x36}, // 0
  {0x00, 0x00, 0x00, 0x00, 0x36}, // 1
  {0x30, 0x49, 0x49, 0x49, 0x06}, // 2
  {0x00, 0x49, 0x49, 0x49, 0x36}, // 3
  {0x06, 0x08, 0x08, 0x08, 0x36}, // 4
  {0x06, 0x49, 0x49, 0x49, 0x30}, // 5
  {0x36, 0x49, 0x49, 0x49, 0x30}, // 6
  {0x00, 0x01, 0x01, 0x01, 0x36}, // 7
  {0x36, 0x49, 0x49, 0x49, 0x36}, // 8
  {0x06, 0x49, 0x49, 0x49, 0x36}  // 9
};


void updateSound(int f) {
  //Beep tetris_sound;
  static int i;
  static unsigned long ti;
  static int rep;
  if (f==0){i=0; return;}
  int t=0;
  int l=0;
  if (millis()<ti){return;}
  l=melody[i];  
  //t=melody[i];  
  //t=pgm_read_byte(melody+i);  
  //if (t==1){l=1;}
  //if (t==2){l=28;}
  //if (t==3){l=30;}
  //if (t==4){l=32;}
  //if (t==5){l=37;}

  //inv_sound.freq=l;
  //t=noteD[i];
  //t=pgm_read_byte(noteD+i);  
  //tetris_sound.ms=1200/t;
  //inv_sound.ms=200;
  if (millis()>ti){ti=millis()+enemyMoveDelay*5;}else{return;} 
  if (l!=1){
    tone2.play(l,enemyMoveDelay+10);
  }
  i++;
  //if (i==40 && rep==0){i=0; rep=1;}
  if (i>=sizeof(melody)){i=0; rep=0;}
}



void initGame() {

  //display.clearDisplay();
  //display.display();
  cls();
  //disp_str(0,5,"GAME START");

    // 敵の初期化
  for (int i = 0; i < enemyRows; i++) {
    for (int j = 0; j < enemyCols; j++) {
      enemies[i][j].x = j * (enemyWidth + enemySpacing) + 8;
      //enemies[i][j].y = i * (enemyHeight + enemySpacing) + 5;
      enemies[i][j].y = -i * (enemyHeight + enemySpacing) + 22;
      enemies[i][j].alive = true;
      teki(enemies[i][j].x,enemies[i][j].y,0);
    }
  }

  // 敵の弾の初期化
  for (int i = 0; i < maxEnemyBullets; i++) {
    enemyBullets[i].active = false;
  }

  playerX = 54;
  playerBullet.active = false;
  //score = 0;
  gameOver = false;
  gameWin = false;
  enemyDirection = 1;
  enemySpeed = 1;
  enemyMoveDelay = 280;
  dflag=0;
  enemy_pat = 0;

}


void setup() {

  Wire.begin();	
  Wire.setClock(400000);	
  tiny_init();

  cls();

  //display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  //display.clearDisplay();
  //display.display();
  //Wire.setClock(400000);	

  pinMode(LEFT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(UP_BUTTON_PIN, INPUT_PULLUP);
  pinMode(DOWN_BUTTON_PIN, INPUT_PULLUP);
  pinMode(SHOOT_BUTTON_PIN, INPUT_PULLUP);


  tone1.begin(SOUND_PIN1);
  tone2.begin(SOUND_PIN2);
  tone1.stop();
  tone2.stop();

  initGame();

  tone1.play( NOTE_C4, 200);  
  tone2.play( NOTE_C5, 200);  
  delay(300);
  tone1.play( NOTE_C4, 200);  
  tone2.play( NOTE_C5, 200);  
  delay(200);
  
  tone1.play(NOTE_D4, 200);  
  tone2.play(NOTE_D5, 200);  
  delay(200);
  tone1.play(NOTE_E4, 500);  
  tone2.play(NOTE_C4, 500);  
  delay(600);
  tone1.stop();
  tone2.stop();
}

void loop() {


  unsigned long currentTime = millis();
  
  //if (gameOver || gameWin) {
  if (demo==1 && (digitalRead(LEFT_BUTTON_PIN) == LOW || digitalRead(RIGHT_BUTTON_PIN) == LOW )){
    gameOver=true;
    demo==0; 
    while( (digitalRead(LEFT_BUTTON_PIN) == LOW || digitalRead(RIGHT_BUTTON_PIN) == LOW ) ){
      delay(50);
    }
  }    
  if (gameOver ) {
    displayEndScreen();
    //while( (digitalRead(LEFT_BUTTON_PIN) == LOW || digitalRead(RIGHT_BUTTON_PIN) == LOW ) ){
    //  delay(50);
    //}
    demo=0;
    // 左ボタンでリスタート
    if (digitalRead(LEFT_BUTTON_PIN) == LOW) {
      delay(50);
      while (digitalRead(LEFT_BUTTON_PIN) != LOW) {
        delay(50);
      }
      initGame(); msf=0; score = 0;
    }
    if (demo==0 && (currentTime - lastmyMove > 10000) ) {initGame(); demo=1; score=0;}
    return;
  }

  if (gameWin) {
    displayEndScreen();
    initGame();
  }




if (currentTime - lastmyMove > alo) {
  lastmyMove=currentTime;
  //dflag=1;
  // プレイヤー操作
  //msf=1;
  //if (digitalRead(LEFT_BUTTON_PIN) == LOW && playerX > 0) {
  if (  (digitalRead(UP_BUTTON_PIN) == LOW) || (digitalRead(SHOOT_BUTTON_PIN) == LOW)  ) {
    if (!playerBullet.active) {msf=1;}   //playerBullet.active = true;
  }
  if (digitalRead(LEFT_BUTTON_PIN) == LOW && digitalRead(RIGHT_BUTTON_PIN) == LOW) {
    //if (digitalRead(UP_BUTTON_PIN) == LOW ) {
    if (!playerBullet.active) {msf=1;}   //playerBullet.active = true;
  }else{ 
    if (digitalRead(LEFT_BUTTON_PIN) == LOW ) {
      if (currentTime - lastButtonPress > 30) {
        if (playerX > 0){
          playerX -= 2;
        }
        lastButtonPress = currentTime;
      }
    }
    if (digitalRead(RIGHT_BUTTON_PIN) == LOW) {
      if (currentTime - lastButtonPress > 30) {
        if (playerX < SCREEN_WIDTH - playerWidth) {
          playerX += 2;
        }
        lastButtonPress = currentTime;
      }
    }
  }
  if (demo==1){msf=1;}
  // 自動発射
  if (msf==1 &&  !playerBullet.active && (currentTime - lastPlayerShot > playerShootDelay) ) {
    playerBullet.x = playerX + playerWidth / 2;
    playerBullet.y = playerY - 2;
    playerBullet.active = true;
    lastPlayerShot = currentTime;
    //inv_sound.freq=4000;
    //inv_sound.ms=5;
    //playBeep(inv_sound);
    tone1.play(5000,8);
    msf=0;

  }

  // プレイヤーの弾の移動
  if (playerBullet.active) {
    undisp_tama( playerBullet.x, playerBullet.y);
    playerBullet.y -= bulletSpeed;
    if (playerBullet.y < 0) {
      playerBullet.active = false;
    }else{
      disp_tama( playerBullet.x, playerBullet.y);
    }
  }
}

  // 敵の移動
  if (currentTime - lastEnemyMove > enemyMoveDelay) {
    bool needMoveDown = false;
    if (dflag++>3){dflag=0;}
    if (enemy_pat++>1){enemy_pat=0;}
  
    // 端に到達したかチェック
    for (int i = 0; i < enemyRows; i++) {
      for (int j = 0; j < enemyCols; j++) {
        if (enemies[i][j].alive) {
          if ((enemies[i][j].x <= 0 && enemyDirection < 0) ||
              (enemies[i][j].x >= SCREEN_WIDTH - enemyWidth && enemyDirection > 0)) {
            needMoveDown = true;
            enemyDirection = -enemyDirection;
            break;
          }
        }
      }
      if (needMoveDown) break;
    }

    // 敵を移動
    for (int i = 0; i < enemyRows; i++) {
      for (int j = 0; j < enemyCols; j++) {
        if (enemies[i][j].alive) {
          teki_cls(enemies[i][j].x,enemies[i][j].y);
          enemies[i][j].x += enemyDirection * enemySpeed;
          if (needMoveDown) {
            enemies[i][j].y += 4;
            // 敵がプレイヤーまで到達したらゲームオーバー
            //if (enemies[i][j].y >= playerY - enemyHeight) {
            if (enemies[i][j].y >= playerY +2) {
              gameOver = true;
            }
          }
          teki(enemies[i][j].x,enemies[i][j].y,enemy_pat);
        }
      }
    }

    lastEnemyMove = currentTime;
  }

  // 敵の発射
  if (currentTime - lastEnemyShot > enemyShootDelay) {
    // ランダムな生きている敵から発射
    int attempts = 0;
    while (attempts < 20) {
      int i = random(enemyRows);
      int j = random(enemyCols);
      if (enemies[i][j].alive) {
        // 空いている弾スロットを探す
        for (int b = 0; b < maxEnemyBullets; b++) {
          if (!enemyBullets[b].active) {
            enemyBullets[b].x = enemies[i][j].x + enemyWidth / 2;
            enemyBullets[b].y = enemies[i][j].y + enemyHeight;
            enemyBullets[b].active = true;
            //tone1.play(50,30);
            break;
          }
        }
        break;
      }
      attempts++;
    }
    lastEnemyShot = currentTime;
  }

  // 敵の弾の移動
  for (int i = 0; i < maxEnemyBullets; i++) {
    if (enemyBullets[i].active) {
      undisp_teki_tama( enemyBullets[i].x, (enemyBullets[i].y));

      //enemyBullets[i].y += 1;
      enemyBullets[i].y += .2;
      if (enemyBullets[i].y > SCREEN_HEIGHT-8) {
        enemyBullets[i].active = false;
      }else{
        disp_teki_tama( enemyBullets[i].x, (enemyBullets[i].y));

      }

      // プレイヤーとの衝突判定
      if (enemyBullets[i].y >= playerY && enemyBullets[i].y <= playerY + playerHeight &&
          enemyBullets[i].x >= playerX && enemyBullets[i].x <= playerX + playerWidth) {
          gameOver = true;
          tone1.play(200,200);//tone(BUZZER_PIN, 200, 200);  // 低い音
          delay(250);
          //tone1.play( 150, 200);  // さらに低い音
          //delay(250);
          tone1.play( 100, 500);  // とても低い音
          delay(600);
          tone1.stop();
      }
    }
  }


  // プレイヤーの弾と敵の衝突判定
  if (playerBullet.active) {
    for (int i = 0; i < enemyRows; i++) {
      for (int j = 0; j < enemyCols; j++) {
        if (enemies[i][j].alive) {
          if (playerBullet.x >= enemies[i][j].x &&
              playerBullet.x <= enemies[i][j].x + enemyWidth &&
              playerBullet.y >= enemies[i][j].y &&
              playerBullet.y <= enemies[i][j].y + enemyHeight) {
            teki_cls(enemies[i][j].x,(enemies[i][j].y));
            enemies[i][j].alive = false;
            playerBullet.active = false;
            undisp_tama( playerBullet.x, playerBullet.y);
            score += 10;
            //inv_sound.freq=100;
            //inv_sound.ms=50;
            //playBeep(inv_sound);
            tone1.play( 50, 100);
            enemyMoveDelay-= 12;
            if (enemyMoveDelay<0){enemyMoveDelay=0;}

          }
        }
      }
    }
  }

  // 全滅チェック
  bool anyEnemyAlive = false;
  for (int i = 0; i < enemyRows; i++) {
    for (int j = 0; j < enemyCols; j++) {
      if (enemies[i][j].alive) {
        //teki(enemies[i][j].x,enemies[i][j].y/8);
        anyEnemyAlive = true;
        break;
      }
    }
    if (anyEnemyAlive) break;
  }
  if (!anyEnemyAlive) {
    gameWin = true;
  }


  // 描画
  // プレイヤーの描画
  drawPlayer(playerX, playerY);
  disp_num(0,0, score);
  updateSound(1);
}

void drawPlayer(int x, int y) {
  updateOLEDRegion(x,y/8,9,icon3);
}

void displayEndScreen() {
  //display.clearDisplay();
  //display.setTextSize(2);
  //display.setTextColor(SSD1306_WHITE);

  if (gameWin) {
    //display.setCursor(20, 15);
    //display.print("YOU WIN!");
    //display.display();
  disp_str(0,5,"YOU WIN!");
  //disp_str(0,6,"LEFT TO START");
  tone1.play( NOTE_C4, 200);  
  tone2.play( NOTE_C5, 200);  
  delay(300);
  tone1.play( NOTE_C4, 200);  
  tone2.play( NOTE_C5, 200);  
  delay(200);
  
  tone1.play(NOTE_D4, 200);  
  tone2.play(NOTE_D5, 200);  
  delay(200);
  tone1.play(NOTE_E4, 500);  
  tone2.play(NOTE_C4, 500);  
  delay(600);
  tone1.stop();
  tone2.stop();

  } else {
    //display.setCursor(10, 15);
    //display.print("GAME OVER");
    //display.setCursor(10, 50);
    //display.setTextSize(1);
    disp_str(0,5,"GAME OVER");
    disp_str(0,6,"LEFT TO START");
    /*
    tone1.play(NOTE_D2, 200);  
    tone2.play(NOTE_D2, 200);  
    delay(200);
    tone1.play(NOTE_C2, 500);  
    tone2.play(NOTE_C2, 500);  
    delay(600);
    tone1.stop();
    tone2.stop();
    */
    //display.print("LEFT to restart");
    //while( (digitalRead(LEFT_BUTTON_PIN) == LOW || digitalRead(RIGHT_BUTTON_PIN) == LOW ) ){
    //while( (digitalRead(LEFT_BUTTON_PIN) == LOW  ) ){
    //  delay(50);
    //}
  }

  //display.setTextSize(1);
  //display.setCursor(30, 35);
  //display.print("Score: ");
  //display.print(score);

  //display.display();
 
}


void cls(){
  for(int y=0;y<9;y++){
    for(int x=0;x<127;x+=8){
      updateOLEDRegion(x, y, 8, icon2);
    }
  }
}

void undisp_teki_tama(int x, int y){
  y=y/8;
  updateOLEDRegion(x, y, 1, icon2);
  updateOLEDRegion(x, y+1, 1, icon2);
}
void disp_teki_tama(int x, int y){
  int c= 0xF0 ;
  uint8_t tama2[] = { 0x00 };
  uint8_t tama3[] = { 0x00 };
  int y2=y % 8;
  y=y/8;
  c=c << y2;
  tama2[0]=c;
  tama3[0]=c>>8;
  updateOLEDRegion(x, y, 1, tama2);
  updateOLEDRegion(x, y+1, 1, tama3);

}


void undisp_tama(int x, int y){
  y=y/8;
  updateOLEDRegion(x, y, 1, icon2);
  updateOLEDRegion(x, y+1, 1, icon2);
}


void disp_tama(int x, int y){
  int c= 0x0F ;
  uint8_t tama2[] = { 0x00 };
  uint8_t tama3[] = { 0x00 };
  int y2=y % 8;
  y=y/8;
  //for(int i=0;i<y2;i++){
  //  if (tama2[0] & 0x80 >0){c=1;} 
  c=c << y2;
    tama2[0]=c;
    tama3[0]=c>>8;
  //   tama3[0]=(tama3[0]+c) << 1;
  //}
  updateOLEDRegion(x, y, 1, tama2);
  updateOLEDRegion(x, y+1, 1, tama3);
}

void teki(int x, int y, int p){
  y=y/8;
  if (p==0){updateOLEDRegion(x, y, 5, icon);}else{updateOLEDRegion(x, y, 5, icon4);}
  //if (p==0){updateOLEDRegion(x, y, 5, icon);}else{updateOLEDRegion(x, y, 5, icon4);}
  //updateOLEDRegion(x, y, 5, icon4);
}

void teki_cls(int x, int y){
  y=y/8;
  updateOLEDRegion(x, y, 5, icon2);
}

void splitDigits(unsigned int num, unsigned char *digits) {
    // 5桁目（万の位）から順に格納する場合
    digits[0] = (num / 10000) % 10;
    digits[1] = (num / 1000) % 10;
    digits[2] = (num / 100) % 10;
    digits[3] = (num / 10) % 10;
    digits[4] = num % 10;
}

void disp_num(int x , int y, int n){
  unsigned char myDigits[5];
  int zero=1;
  splitDigits(n,myDigits);
  for (int i = 0; i < 5; i++) {
    unsigned char n2 = myDigits[i];
    if (zero==1 && n2==0 && i<4){
      //blank();
    }else{
      updateOLEDRegion(x, y, 5, font_7seg[n2]);
      zero=0;
      x+=6;
    }
  }
}

void disp_str(int x , int y, const char* s){
 for (int i = 0; i < strlen(s); i++) {
      updateOLEDRegion(x, y, 5, font5x7[s[i]-32]);
      x+=6;
  }
}

// 部分書き換え関数
// x: 0-127, page: 0-7 (8ピクセル単位), width: 書き込む幅
void updateOLEDRegion(uint8_t x, uint8_t page, uint8_t width, const uint8_t* data) {
    // 1. 書き込み範囲の設定
    Wire.beginTransmission(OLED_ADDR);
    Wire.write(0x00); // コマンドモード
    
    Wire.write(0x21); // カラムアドレス設定
    Wire.write(x);    // 開始カラム
    Wire.write(x + width - 1); // 終了カラム
    
    Wire.write(0x22); // ページアドレス設定
    Wire.write(page); // 開始ページ
    Wire.write(page); // 終了ページ (1ページ分)
    Wire.endTransmission();

    // 2. データの送信
    // I2Cのバッファ制限(32バイト)があるため、分割して送るのが安全です
    for (uint8_t i = 0; i < width; i++) {
        if (i % 16 == 0) { // 16バイトごとにセッションを分ける例
            Wire.endTransmission();
            Wire.beginTransmission(OLED_ADDR);
            Wire.write(0x40); // データモード
        }
        Wire.write(data[i]);
    }
    Wire.endTransmission();
}


void tiny_init(){

  Wire.beginTransmission(OLED_ADDR);
  Wire.write(0x80);
  Wire.write(0xAF);
  Wire.endTransmission();

  Wire.beginTransmission(OLED_ADDR );
  Wire.write(0x00); // 複数バイトコマンドタグ
  Wire.write(0xC8); // [C0]/C8:上下の描画方向
  Wire.write(0xA1); // [A0]/A1:左右の描画方向
  Wire.write(0xA8); Wire.write(0x3F); // 画面の解像度 [3F]:64Line / 1F:32Line
  Wire.write(0xDA); Wire.write(0x12); // [12]:Sequential / 02:Alternative
  Wire.write(0xD3); Wire.write(0x00); // 縦方向のオフセット
  Wire.write(0x40); // 縦方向の描画開始位置
  Wire.write(0xA6); // 表示方法 - A7にすると画面反転
  Wire.write(0x8D); Wire.write(0x14);
  Wire.endTransmission();
}



