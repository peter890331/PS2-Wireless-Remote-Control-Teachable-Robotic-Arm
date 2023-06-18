#include <LiquidCrystal_I2C.h>
#include <Wire.h> 
#include <PS2X_lib.h>           //呼叫PS2無線遙控模組的函式庫
PS2X ps2x;                     //建立PS2控制器的類別實體
#include <VarSpeedServo.h>     //呼叫伺服控速函式庫
#include <SPI.h> 
#include <SD.h>
File myFile;

Sd2Card card;
SdVolume volume;
SdFile root;

const int chipSelect = 53;

//宣告程式會使用的變數

int   error = 0;
int   val;
byte  type = 0;
byte  vibrate = 0;
int   angle1 = 96;              //設定馬達預設狀態的角度
int   angle2 = 95;              //設定馬達預設狀態的角度
int   angle3 = 50;              //設定馬達預設狀態的角度
int   angle4 = 94;              //設定馬達預設狀態的角度
int   angle5 = 90;              //設定馬達預設狀態的角度
int   angle6 = 65;              //設定馬達預設狀態的角度
byte  servo1 = 2;               //宣告馬達腳位
byte  servo2 = 3;               //宣告馬達腳位
byte  servo3 = 4;               //宣告馬達腳位
byte  servo4 = 5;               //宣告馬達腳位
byte  servo5 = 6;               //宣告馬達腳位
byte  servo6 = 7;               //宣告馬達腳位
int   a, b;
int   i, j;                        //for迴圈使用的變數
int   SPEED = 20;               //設定除了servo2以外的速度
int   SPEEDservo2 = 5;          //設定servo2專用的速度
int   frequency = 0;            //自動化次數變數
unsigned char   pos[30][3];               //宣告二維陣列
unsigned char   sd[90];
int   k = 0;
int   c = 0;
int   sdf = 0;
bool  kill68 = false;          //debug變數
bool  motionRecord = false;    //預設動作複製不啟用
VarSpeedServo Servo1;          //使用控速函式庫命名伺服馬達
VarSpeedServo Servo2;          //使用控速函式庫命名伺服馬達
VarSpeedServo Servo3;          //使用控速函式庫命名伺服馬達
VarSpeedServo Servo4;          //使用控速函式庫命名伺服馬達
VarSpeedServo Servo5;          //使用控速函式庫命名伺服馬達
VarSpeedServo Servo6;          //使用控速函式庫命名伺服馬達
LiquidCrystal_I2C lcd(0x27, 16, 2);   // 0x27是LCD模組的地址，16表示有十六欄位，2表示總共有2行

void setup() {
	Serial.begin(57600);               //設定鮑率
 
	Servo1.attach(servo1);            //伺服連接數位腳位
	Servo2.attach(servo2);            //伺服連接數位腳位
	Servo3.attach(servo3);            //伺服連接數位腳位
	Servo4.attach(servo4);            //伺服連接數位腳位
	Servo5.attach(servo5);            //伺服連接數位腳位
	Servo6.attach(servo6);            //伺服連接數位腳位
	Serial.print("\nInitializing SD card...");
  pinMode(53,OUTPUT);

if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card is inserted?");
    Serial.println("* Is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
   Serial.println("Wiring is correct and a card is present."); 
  }

	//辨識ps2接收錯誤狀態

	error = ps2x.config_gamepad(13, 11, 10, 12, false, true);
	/* 控制器接腳設置並驗證是否有錯誤:
	 GamePad(時脈腳位, 命令腳位, 選取腳位, 資料腳位, 是否支援類比按鍵, 是否支援震動) */

	if (error == 0) {                 //控制器連接沒有問題
		Serial.println("Found Controller, configured successful.");
	}
	else if (error == 1) {            //找不到控制器
		Serial.println("No controller found, check wiring, see readme.txt to enable debug.");
	}
	else if (error == 2) {            //發現控制器，但不接受命令
		Serial.println("Controller found but not accepting commands.");
	}
	else if (error == 3) {             //控制器拒絕進入類比感測壓力模式，或許是此控制器不支援的緣故
		Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
	}


	//偵測控制器器的類型

	type = ps2x.readType();
	switch (type) {
	case 0:
		Serial.println("Unknown Controller type");        //未知的控制器類型
		break;
	case 1:
		Serial.println("DualShock Controller Found");     //發現DualShock控制器
		break;
	case 2:
		Serial.println("GuitarHero Controller Found");    //發現吉他英雄控制器
		break;
	}
	//LCD初始化

	initial();
  lcd.init();
	lcd.backlight();                //lcd開啟背光
	Initialing();

  

 
}
void loop() {
	if (error == 1) {                       //如果沒發現任何控制器
		return;                               //跳出迴圈
	}
	ps2x.read_gamepad(false, vibrate);      //讀取控制器並且命令震動馬達以"vibrate"變數的速度旋轉
	//kill 68 Start
	if (kill68 == true)
	{
		if (c < 100)
		{
			Initialing();
			int   angle1 = 96;
			int   angle2 = 95;
			int   angle3 = 50;
			int   angle4 = 94;
			int   angle5 = 90;
			int   angle6 = 65;
			k = 0;
			motionRecord = false;
			c++;
		}
		else if (c == 100)
		{
			c = 0;
			kill68 = false;
			int   angle1 = 96;
			int   angle2 = 95;
			int   angle3 = 50;
			int   angle4 = 94;
			int   angle5 = 90;
			int   angle6 = 65;
			k = 0;
			motionRecord = false;
			Serial.println("------------------------------------");
			Serial.println("ALL ANGLE");                              //監控視窗列印字串
			Serial.println("------------------------------------");
			Serial.println("Servo1");                                 //監控視窗列印字串
			Serial.println(angle1);                                   //監控視窗列印角度數值
			Serial.println("Servo2");                                 //監控視窗列印字串
			Serial.println(angle2);                                   //監控視窗列印角度數值
			Serial.println("Servo3");                                 //監控視窗列印字串
			Serial.println(angle3);                                   //監控視窗列印角度數值
			Serial.println("Servo4");                                 //監控視窗列印字串
			Serial.println(angle4);                                   //監控視窗列印角度數值
			Serial.println("Servo5");                                 //監控視窗列印字串
			Serial.println(angle5);                                   //監控視窗列印角度數值
			Serial.println("Gripper");                                //監控視窗列印字串
			Serial.println(angle6);                                   //監控視窗列印角度數值
			Serial.print("k=");
			Serial.println(k);
			Serial.print("motionRecord=");
			Serial.println(motionRecord);
			lcdWelcome();
		}
	}
	//kill 68 End
	//Motion Record Start
	if ((ps2x.ButtonPressed(PSB_PAD_LEFT) == true) && (motionRecord == false))  //當按下左鍵且複製動作未開始時
	{
		motionRecord = true;                                                      //複製動作開始
		k = 0;                                                                    //設定二微陣列初值                                               
		for (a = 0; a < 30; a++)                                                  //二維陣列共30行
		{
			for (b = 0; b < 3; b++)                                                 //二維陣列共3列
			{
				pos[a][b] = 0;                                                        //清除先前二維陣列資料
			}
			Serial.println();
		}
      Serial.println("Start set");                                            //監控視窗列印字串
      lcd.clear();                                                            //LCD清除先前畫面
      lcd.setCursor(0,0);                                                     //設定顯示座標
      lcd.print("Start set");                                                 //LCD顯示字串
	}
	else if ((ps2x.ButtonPressed(PSB_PAD_RIGHT) == true) && (motionRecord == true))     //當按下右鍵且複製動作已開始時
	{   
		motionRecord = false;                                                             //複製動作結束
		pos[k][0] = 0;
		angleWrite(pos[k - 1][0]);                                                        //寫入最後一顆servo數值
		for (a = 0; a < 30; a++)                                                          //二維陣列共30行
		{
			for (b = 0; b < 3; b++)                                                         //二維陣列共3列
			{
				Serial.print(pos[a][b]);                                                      //監控畫面列印所有二維陣列數值
				Serial.print(" ");
			}
			Serial.println();
		}
		k = 0;
    Serial.println("Done set");                              //監控視窗列印字串
    sdf = 1;
    Serial.print(sdf);           
      lcd.clear();                                            //清除先前畫面
      lcd.setCursor(0,0);                                     //設定顯示座標
      lcd.print("Done set");                                  //顯示字串

      if(sdf){
  }
      
	}
	//Motion Record End
	//Servo1 Start
	if ((ps2x.Button(PSB_RED))&&(kill68!=true)) {             //當壓著紅色圓形時動作
		if (motionRecord)                                       //當複製動作開始時
		{
			pos[k][0] = 1;                                        //編排二維陣列數值寫入位置
			if (k == 0)
			{
				pos[k][1] = angle1;                                 //寫入數值
				k++;                                                ////編排二維陣列數值寫入位置
			}
			else if ((pos[k][0] != pos[k - 1][0]) && ((k - 1) >= 0))      //判斷servo是否與上次使用servo重複
			{
				pos[k][1] = angle1;
				angleWrite(pos[k - 1][0]);                                 //覆蓋角度數值
				k++;
			}
		}
		if (ps2x.Analog(PSS_LY) < 120) {      //當左搖桿數值小於120時
			if (angle1 < 180)                     //當servo1角度數值小於180
			{
				angle1 += 2;                         //servo1角度數值+2
			}
			else
			{
				angle1 = 180;                     //servo1角度強制等於180，防止轉超過極限角度
			}
			Serial.println("Servo1");            //監控視窗列印字串
			Serial.println(angle1);              //監控視窗列印角度數值
			Servo1.write(angle1);               //servo1寫入角度數值

			lcd.clear();                        //lcd清除先前畫面
			lcd.setCursor(0, 0);                 //設定第一個字顯示座標
			lcd.print("Servo1");                //lcd顯示字串
			lcd.setCursor(0, 1);                 //設定第二個字顯示座標
			lcd.print(angle1);                  //lcd顯示角度數值

		}
		else if (ps2x.Analog(PSS_LY) > 134) {   //當左搖桿數值大於134時
			if (angle1 > 0)                           //當servo1角度數值大於0
			{
				angle1 -= 2;                             //servo1角度數值-2

				Serial.println("Servo1");                //監控視窗列印字串
				Serial.println(angle1);                  //監控視窗列印角度數值
				Servo1.write(angle1);                   //servo1寫入角度數值

				lcd.clear();                              //lcd清除先前畫面
				lcd.setCursor(0, 0);                       //設定第一個字顯示座標
				lcd.print("Servo1");                      //lcd顯示字串
				lcd.setCursor(0, 1);                       //設定第二個字顯示座標
				lcd.print(angle1);                        //lcd顯示角度數值
			}
			else
			{
				angle1 = 0;                         //servo1角度強制等於0，防止轉超過極限角度

				Serial.println("Servo1");                //監控視窗列印字串
				Serial.println(angle1);                  //監控視窗列印角度數值
				Servo1.write(angle1);                   //servo1寫入角度數值

				lcd.clear();                              //lcd清除先前畫面
				lcd.setCursor(0, 0);                       //設定第一個字顯示座標
				lcd.print("Servo1");                      //lcd顯示字串
				lcd.setCursor(0, 1);                       //設定第二個字顯示座標
				lcd.print(angle1);                        //lcd顯示角度數值
			}


		}
		else {
			Serial.println("Servo1");                //監控視窗列印字串
			Serial.println(angle1);                  //監控視窗列印角度數值
			Servo1.write(angle1);                   //servo1寫入角度數值

			lcd.clear();                              //lcd清除先前畫面
			lcd.setCursor(0, 0);                       //設定第一個字顯示座標
			lcd.print("Servo1");                      //lcd顯示字串
			lcd.setCursor(0, 1);                       //設定第二個字顯示座標
			lcd.print(angle1);                        //lcd顯示角度數值}
		}
	}
	//Servo1 End
	//Servo2 Start
	if ((ps2x.Button(PSB_BLUE))&&(kill68 != true)) {                //當壓著藍色叉叉時動作
		if (motionRecord)
		{
			pos[k][0] = 2;
			if (k == 0)
			{
				pos[k][1] = angle2;
				k++;
			}
			else if ((pos[k][0] != pos[k - 1][0]) && ((k - 1) >= 0))
			{
				pos[k][1] = angle2;
				angleWrite(pos[k - 1][0]);
				k++;
			}
		}
		if (ps2x.Analog(PSS_LY) > 134) {          //當左搖桿數值大於134時
			if (angle2 < 100)                         //當servo2角度數值小於100
			{
				angle2++;                             //servo2角度數值+1

				Serial.println("Servo2");                //監控視窗列印字串
				Serial.println(angle2);                  //監控視窗列印角度數值
				Servo2.write(angle2);                   //servo2寫入角度數值

				lcd.clear();                            //lcd清除先前畫面
				lcd.setCursor(0, 0);                     //設定第一個字顯示座標
				lcd.print("Servo2");                    //lcd顯示字串
				lcd.setCursor(0, 1);                     //設定第二個字顯示座標
				lcd.print(angle2);                      //lcd顯示角度數值
			}
			else
			{
				angle2 = 100;                         //servo2角度強制等於100，防止轉超過極限角度

				Serial.println("Servo2");                //監控視窗列印字串
				Serial.println(angle2);                  //監控視窗列印角度數值
				Servo2.write(angle2);                   //servo2寫入角度數值

				lcd.clear();                            //lcd清除先前畫面
				lcd.setCursor(0, 0);                     //設定第一個字顯示座標
				lcd.print("Servo2");                    //lcd顯示字串
				lcd.setCursor(0, 1);                     //設定第二個字顯示座標
				lcd.print(angle2);                      //lcd顯示角度數值
			}


		}
		else if (ps2x.Analog(PSS_LY) < 120) {   //當左搖桿數值小於120時
			if (angle2 > 50)                          //當servo2角度數值大於50
			{
				angle2--;                             //servo2角度數值-1

				Serial.println("Servo2");                //監控視窗列印字串
				Serial.println(angle2);                  //監控視窗列印角度數值
				Servo2.write(angle2);                   //servo2寫入角度數值

				lcd.clear();                              //lcd清除先前畫面
				lcd.setCursor(0, 0);                       //設定第一個字顯示座標
				lcd.print("Servo2");                      //lcd顯示字串
				lcd.setCursor(0, 1);                       //設定第二個字顯示座標
				lcd.print(angle2);                        //lcd顯示角度數值
			}
			else {
				angle2 = 50;                          //servo2角度強制等於50，防止轉超過極限角度

				Serial.println("Servo2");                //監控視窗列印字串
				Serial.println(angle2);                  //監控視窗列印角度數值
				Servo2.write(angle2);                   //servo2寫入角度數值

				lcd.clear();                              //lcd清除先前畫面
				lcd.setCursor(0, 0);                       //設定第一個字顯示座標
				lcd.print("Servo2");                      //lcd顯示字串
				lcd.setCursor(0, 1);                       //設定第二個字顯示座標
				lcd.print(angle2);                        //lcd顯示角度數值
			}


		}
		else {
			Serial.println("Servo2");                //監控視窗列印字串
			Serial.println(angle2);                  //監控視窗列印角度數值
			Servo2.write(angle2);                   //servo2寫入角度數值

			lcd.clear();                              //lcd清除先前畫面
			lcd.setCursor(0, 0);                       //設定第一個字顯示座標
			lcd.print("Servo2");                      //lcd顯示字串
			lcd.setCursor(0, 1);                       //設定第二個字顯示座標
			lcd.print(angle2);                        //lcd顯示角度數值}
		}
	}
	//Servo2 End
	//Servo3 Start
	if ((ps2x.Button(PSB_PINK)) && (kill68 != true)) {                  //當壓著粉色方形時動作
		if (motionRecord)
		{
			pos[k][0] = 3;
			if (k == 0)
			{
				pos[k][1] = angle3;
				k++;
			}
			else if ((pos[k][0] != pos[k - 1][0]) && ((k - 1) >= 0))
			{
				pos[k][1] = angle3;
				angleWrite(pos[k - 1][0]);
				k++;
			}
		}

		if (ps2x.Analog(PSS_LY) < 120) {            //當左搖桿數值小於120時
			if (angle3 < 100)                           //當servo3角度數值小於100
			{
				angle3 += 2;                               //servo3角度數值+2

				Serial.println("Servo3");                  //監控視窗列印字串
				Serial.println(angle3);                    //監控視窗列印角度數值
				Servo3.write(angle3);                     //servo3寫入角度數值

				lcd.clear();                              //lcd清除先前畫面
				lcd.setCursor(0, 0);                       //設定第一個字顯示座標
				lcd.print("Servo3");                      //lcd顯示字串
				lcd.setCursor(0, 1);                       //設定第二個字顯示座標
				lcd.print(angle3);                        //lcd顯示角度數值
			}
			else
			{
				angle3 = 100;                           //servo3角度強制等於100，防止轉超過極限角度

				Serial.println("Servo3");                  //監控視窗列印字串
				Serial.println(angle3);                    //監控視窗列印角度數值
				Servo3.write(angle3);                     //servo3寫入角度數值

				lcd.clear();                              //lcd清除先前畫面
				lcd.setCursor(0, 0);                       //設定第一個字顯示座標
				lcd.print("Servo3");                      //lcd顯示字串
				lcd.setCursor(0, 1);                       //設定第二個字顯示座標
				lcd.print(angle3);                        //lcd顯示角度數值
			}


		}
		else if (ps2x.Analog(PSS_LY) > 134) {     //當左搖桿數值大於134時
			if (angle3 > 0)                             //當servo3角度數值大於0
			{
				angle3 -= 2;                               //servo3角度數值-2

				Serial.println("Servo3");                 //監控視窗列印字串
				Serial.println(angle3);                   //監控視窗列印角度數值
				Servo3.write(angle3);                     //servo3寫入角度數值

				lcd.clear();                              //lcd清除先前畫面
				lcd.setCursor(0, 0);                       //設定第一個字顯示座標
				lcd.print("Servo3");                      //lcd顯示字串
				lcd.setCursor(0, 1);                       //設定第二個字顯示座標
				lcd.print(angle3);                        //lcd顯示角度數值
			}
			else {
				angle3 = 0;                             //servo3角度強制等於0，防止轉超過極限角度

				Serial.println("Servo3");                 //監控視窗列印字串
				Serial.println(angle3);                   //監控視窗列印角度數值
				Servo3.write(angle3);                     //servo3寫入角度數值

				lcd.clear();                              //lcd清除先前畫面
				lcd.setCursor(0, 0);                       //設定第一個字顯示座標
				lcd.print("Servo3");                      //lcd顯示字串
				lcd.setCursor(0, 1);                       //設定第二個字顯示座標
				lcd.print(angle3);                        //lcd顯示角度數值
			}


		}
		else {
			Serial.println("Servo3");                 //監控視窗列印字串
			Serial.println(angle3);                   //監控視窗列印角度數值
			Servo3.write(angle3);                     //servo3寫入角度數值

			lcd.clear();                              //lcd清除先前畫面
			lcd.setCursor(0, 0);                       //設定第一個字顯示座標
			lcd.print("Servo3");                      //lcd顯示字串
			lcd.setCursor(0, 1);                       //設定第二個字顯示座標
			lcd.print(angle3);                        //lcd顯示角度數值}
		}
	}
	//Servo3 End
	//Servo4 Start
	if ((ps2x.Button(PSB_GREEN)) && (kill68 != true)) {                 //當壓著綠色三角形時動作
		if (motionRecord)
		{
			pos[k][0] = 4;
			if (k == 0)
			{
				pos[k][1] = angle4;
				k++;
			}
			else if ((pos[k][0] != pos[k - 1][0]) && ((k - 1) >= 0))
			{
				pos[k][1] = angle4;
				angleWrite(pos[k - 1][0]);
				k++;
			}
		}
		if (ps2x.Analog(PSS_LY) < 120) {            //當左搖桿數值小於120時
			if (angle4 < 180)                           //當servo4角度數值小於180
			{
				angle4 += 2;                               //servo4角度數值+2
				Serial.println("Servo4");                  //監控視窗列印字串
				Serial.println(angle4);                    //監控視窗列印角度數值
				Servo4.write(angle4);                     //servo4寫入角度數值
				lcd.clear();                              //lcd清除先前畫面
				lcd.setCursor(0, 0);                       //設定第一個字顯示座標
				lcd.print("Servo4");                      //lcd顯示字串
				lcd.setCursor(0, 1);                       //設定第二個字顯示座標
				lcd.print(angle4);                        //lcd顯示角度數值
			}
			else
			{
				angle4 = 180;                           //servo4角度強制等於180，防止轉超過極限角度
				Serial.println("Servo4");                  //監控視窗列印字串
				Serial.println(angle4);                    //監控視窗列印角度數值
				Servo4.write(angle4);                     //servo4寫入角度數值
				lcd.clear();                              //lcd清除先前畫面
				lcd.setCursor(0, 0);                       //設定第一個字顯示座標
				lcd.print("Servo4");                      //lcd顯示字串
				lcd.setCursor(0, 1);                       //設定第二個字顯示座標
				lcd.print(angle4);                        //lcd顯示角度數值
			}
		}
		else if (ps2x.Analog(PSS_LY) > 134) {     //當左搖桿數值大於134時
			if (angle4 > 20)                             //當servo4角度數值大於20
			{
				angle4 -= 2;                               //servo4角度數值-2
				Serial.println("Servo4");                 //監控視窗列印字串
				Serial.println(angle4);                   //監控視窗列印角度數值
				Servo4.write(angle4);                    //servo4寫入角度數值
				lcd.clear();                             //lcd清除先前畫面
				lcd.setCursor(0, 0);                      //設定第一個字顯示座標
				lcd.print("Servo4");                     //lcd顯示字串
				lcd.setCursor(0, 1);                      //設定第二個字顯示座標
				lcd.print(angle4);                       //lcd顯示角度數值
			}
			else {
				angle4 = 20;                             //servo4角度強制等於20，防止轉超過極限角度
				Serial.println("Servo4");                 //監控視窗列印字串
				Serial.println(angle4);                   //監控視窗列印角度數值
				Servo4.write(angle4);                    //servo4寫入角度數值
				lcd.clear();                             //lcd清除先前畫面
				lcd.setCursor(0, 0);                      //設定第一個字顯示座標
				lcd.print("Servo4");                     //lcd顯示字串
				lcd.setCursor(0, 1);                      //設定第二個字顯示座標
				lcd.print(angle4);                       //lcd顯示角度數值
			}
		}
		else {
			Serial.println("Servo4");                 //監控視窗列印字串
			Serial.println(angle4);                   //監控視窗列印角度數值
			Servo4.write(angle4);                    //servo4寫入角度數值
			lcd.clear();                             //lcd清除先前畫面
			lcd.setCursor(0, 0);                      //設定第一個字顯示座標
			lcd.print("Servo4");                     //lcd顯示字串
			lcd.setCursor(0, 1);                      //設定第二個字顯示座標
			lcd.print(angle4);                       //lcd顯示角度數值}
		}
	}
	//Servo4 End
	//Servo5 Start
	if ((ps2x.Button(PSB_R1)) && (kill68 != true)) {                  //當壓著R1時動作
		if (motionRecord)
		{
			pos[k][0] = 5;
			if (k == 0)
			{
				pos[k][1] = angle5;
				k++;
			}
			else if ((pos[k][0] != pos[k - 1][0]) && ((k - 1) >= 0))
			{
				pos[k][1] = angle5;
				angleWrite(pos[k - 1][0]);
				k++;
			}
		}
		if (ps2x.Analog(PSS_LY) < 120) {          //當左搖桿數值小於120時
			if (angle5 < 180)                         //當servo5角度數值小於180
			{
				angle5 += 3;                             //servo5角度數值+3
				Serial.println("Servo5");                //監控視窗列印字串
				Serial.println(angle5);                  //監控視窗列印角度數值
				Servo5.write(angle5);                   //servo5寫入角度數值
				lcd.clear();                            //lcd清除先前畫面
				lcd.setCursor(0, 0);                     //設定第一個字顯示座標
				lcd.print("Servo5");                    //lcd顯示字串
				lcd.setCursor(0, 1);                     //設定第二個字顯示座標
				lcd.print(angle5);                      //lcd顯示角度數值
			}
			else
			{
				angle5 = 180;                         //servo5角度強制等於180，防止轉超過極限角度
				Serial.println("Servo5");                //監控視窗列印字串
				Serial.println(angle5);                  //監控視窗列印角度數值
				Servo5.write(angle5);                   //servo5寫入角度數值
				lcd.clear();                            //lcd清除先前畫面
				lcd.setCursor(0, 0);                     //設定第一個字顯示座標
				lcd.print("Servo5");                    //lcd顯示字串
				lcd.setCursor(0, 1);                     //設定第二個字顯示座標
				lcd.print(angle5);                      //lcd顯示角度數值
			}
		}
		else if (ps2x.Analog(PSS_LY) > 134) {   //當左搖桿數值大於134時
			if (angle5 > 0)                           //當servo5角度數值大於0
			{
				angle5 -= 3;                             //servo5角度數值-3
				Serial.println("Servo5");                //監控視窗列印字串
				Serial.println(angle5);                  //監控視窗列印角度數值
				Servo5.write(angle5);                   //servo5寫入角度數值
				lcd.clear();                            //lcd清除先前畫面
				lcd.setCursor(0, 0);                     //設定第一個字顯示座標
				lcd.print("Servo5");                    //lcd顯示字串
				lcd.setCursor(0, 1);                     //設定第二個字顯示座標
				lcd.print(angle5);                      //lcd顯示角度數值
			}
			else {
				angle5 = 0;                           //servo5角度強制等於0，防止轉超過極限角度
				Serial.println("Servo5");                //監控視窗列印字串
				Serial.println(angle5);                  //監控視窗列印角度數值
				Servo5.write(angle5);                   //servo5寫入角度數值
				lcd.clear();                            //lcd清除先前畫面
				lcd.setCursor(0, 0);                     //設定第一個字顯示座標
				lcd.print("Servo5");                    //lcd顯示字串
				lcd.setCursor(0, 1);                     //設定第二個字顯示座標
				lcd.print(angle5);                      //lcd顯示角度數值
			}
		}
		else {
			Serial.println("Servo5");                //監控視窗列印字串
			Serial.println(angle5);                  //監控視窗列印角度數值
			Servo5.write(angle5);                   //servo5寫入角度數值
			lcd.clear();                            //lcd清除先前畫面
			lcd.setCursor(0, 0);                     //設定第一個字顯示座標
			lcd.print("Servo5");                    //lcd顯示字串
			lcd.setCursor(0, 1);                     //設定第二個字顯示座標
			lcd.print(angle5);                      //lcd顯示角度數值}
		}
	}
	//Servo5 End
	//Gripper
	//Open Start
	if (ps2x.Button(PSB_R2)) {                  //當壓著R2時動作
		if (motionRecord)
		{
			pos[k][0] = 6;
			if (k == 0)
			{
				pos[k][1] = angle6;
				k++;
			}
			else if ((pos[k][0] != pos[k - 1][0]) && ((k - 1) >= 0))
			{
				pos[k][1] = angle6;
				angleWrite(pos[k - 1][0]);
				k++;
			}
		}
		delay(10);
		if (angle6 > 65)                         //當servo5角度數值大於65
		{
			angle6 -= 3;                             //servo6角度數值-3
			Serial.println("Gripper");                //監控視窗列印字串
			Serial.println(angle6);                  //監控視窗列印角度數值
			Servo6.write(angle6);                   //servo6寫入角度數值
			lcd.clear();                            //lcd清除先前畫面
			lcd.setCursor(0, 0);                     //設定第一個字顯示座標
			lcd.print("Gripper");                    //lcd顯示字串
			lcd.setCursor(0, 1);                     //設定第二個字顯示座標
			lcd.print(angle6);                      //lcd顯示角度數值
		}
		else
		{
			angle6 = 65;                         //servo6角度強制等於65，防止轉超過極限角度
			Serial.println("Gripper");                //監控視窗列印字串
			Serial.println(angle6);                  //監控視窗列印角度數值
			Servo6.write(angle6);                   //servo6寫入角度數值
			lcd.clear();                            //lcd清除先前畫面
			lcd.setCursor(0, 0);                     //設定第一個字顯示座標
			lcd.print("Gripper");                    //lcd顯示字串
			lcd.setCursor(0, 1);                     //設定第二個字顯示座標
			lcd.print(angle6);                      //lcd顯示角度數值
		}
	}
	//Open End
	//Close Start
	if (ps2x.Button(PSB_L2)) {//當壓著L2時動作
		if (motionRecord)
		{
			pos[k][0] = 6;
			if (k == 0)
			{
				pos[k][1] = angle6;
				k++;
			}
			else if ((pos[k][0] != pos[k - 1][0]) && ((k - 1) >= 0))
			{
				pos[k][1] = angle6;
				angleWrite(pos[k - 1][0]);
				k++;
			}
		}
		delay(10);
		if (angle6 < 145)                           //當servo6角度數值小於145
		{
			angle6 += 3;                             //servo6角度數值+3

			Serial.println("Gripper");                //監控視窗列印字串
			Serial.println(angle6);                  //監控視窗列印角度數值
			Servo6.write(angle6);                   //servo6寫入角度數值

			lcd.clear();                            //lcd清除先前畫面
			lcd.setCursor(0, 0);                     //設定第一個字顯示座標
			lcd.print("Gripper");                    //lcd顯示字串
			lcd.setCursor(0, 1);                     //設定第二個字顯示座標
			lcd.print(angle6);                      //lcd顯示角度數值
		}
		else {
			angle6 = 145;                           //servo6角度強制等於145，防止轉超過極限角度

			Serial.println("Gripper");                //監控視窗列印字串
			Serial.println(angle6);                  //監控視窗列印角度數值
			Servo6.write(angle6);                   //servo6寫入角度數值

			lcd.clear();                            //lcd清除先前畫面
			lcd.setCursor(0, 0);                     //設定第一個字顯示座標
			lcd.print("Gripper");                    //lcd顯示字串
			lcd.setCursor(0, 1);                     //設定第二個字顯示座標
			lcd.print(angle6);                      //lcd顯示角度數值
		}
		//手把震動
		val = map(angle6, 65, 145, 0, 1000);          //依據servo6角度值調整震動馬達強度
		ps2x.read_gamepad(false, val);                 //讀取控制器並且命令震動馬達以"val"變數的速度旋轉
	}
	//Close End
	//角度檢視
	if (ps2x.Button(PSB_SELECT)) {                                 //當按下select鍵時，顯示所有角度，方便修改準位
		Serial.println("------------------------------------");
		Serial.println("ALL ANGLE");                              //監控視窗列印字串
		Serial.println("------------------------------------");
		Serial.println("Servo1");                                 //監控視窗列印字串
		Serial.println(angle1);                                   //監控視窗列印角度數值
		Serial.println("Servo2");                                 //監控視窗列印字串
		Serial.println(angle2);                                   //監控視窗列印角度數值
		Serial.println("Servo3");                                 //監控視窗列印字串
		Serial.println(angle3);                                   //監控視窗列印角度數值
		Serial.println("Servo4");                                 //監控視窗列印字串
		Serial.println(angle4);                                   //監控視窗列印角度數值
		Serial.println("Servo5");                                 //監控視窗列印字串
		Serial.println(angle5);                                   //監控視窗列印角度數值
		Serial.println("Gripper");                                //監控視窗列印字串
		Serial.println(angle6);                                   //監控視窗列印角度數值
	}
	//初始化
	if (ps2x.Button(PSB_L1)) {                                   //當按下L1鍵時
		initial();
		lcdWelcome();
	}
	delay(50);//遙控馬達時控速用delay
 
	//Auto Motion Simulation Start
	if (ps2x.ButtonPressed(PSB_PAD_UP)) {
      lcd.clear();                                            //lcd清除先前畫面
      lcd.setCursor(0,0);                                     //設定第一個字顯示座標
      lcd.print("Auto set");                                  //lcd顯示字串
      lcd.setCursor(0,1);                                     //設定第一個字顯示座標
      lcd.print("finish");                                    //lcd顯示字串
       delay(1000);
		initial();
    Serial.println("Auto start");                            //監控視窗列印字串
      
      lcd.clear();                                            //lcd清除先前畫面
      lcd.setCursor(0,0);                                     //設定第一個字顯示座標
      lcd.print("Auto start");                                //lcd顯示字串
      lcd.setCursor(0,1);                                     //設定第二個字顯示座標
      lcd.print(j);                                           //lcd顯示已動作次數
      lcd.print("/");                                         //lcd顯示分號
      lcd.print(frequency);                                   //lcd顯示動作設定次數
    for(j=1;j<frequency+1;j++){
       Serial.println("Auto start");                            //監控視窗列印字串
      
      lcd.clear();                                            //lcd清除先前畫面
      lcd.setCursor(0,0);                                     //設定第一個字顯示座標
      lcd.print("Auto start");                                //lcd顯示字串
      lcd.setCursor(0,1);                                     //設定第二個字顯示座標
      lcd.print(j);                                           //lcd顯示已動作次數
      lcd.print("/");                                         //lcd顯示分號
      lcd.print(frequency);                                   //lcd顯示動作設定次數
		for (a = 29; a >= 0; a--)                                 //反向動作
		{
			if (pos[a][0] == 6)
			{

			}
			else {
				servoAutoMotion(pos[a][0], pos[a][1]);                //數值寫入自動化函數
			}
			Serial.print(pos[a][0]);                                //監控畫面列印當前動作Servo
			Serial.print(" ");
			Serial.print(pos[a][1]);                                //監控畫面列印當前目標角度
			Serial.println();
		}
		for (a = 0; a<30; a++)                                   //正向動作
		{
			Serial.print(pos[a][0]);                                //監控畫面列印當前動作Servo
			Serial.print(" ");
			Serial.print(pos[a][2]);                                //監控畫面列印當前目標角度
			Serial.println();
			servoAutoMotion(pos[a][0], pos[a][2]);                 //數值寫入自動化函數
		}
    Serial.println("Auto end");                              //監控視窗列印字串
      
      lcd.clear();                                            //lcd清除先前畫面
      lcd.setCursor(0,0);                                     //設定第一個字顯示座標
      lcd.print("Auto end");                                  //lcd顯示字串
      lcd.setCursor(0,1);                                     //設定第二個字顯示座標
      lcd.print(j);                                           //lcd顯示已動作次數
      lcd.print("/");                                         //lcd顯示分號
      lcd.print(frequency);                                   //lcd顯示動作設定次數
      delay(1000);
    }
		initial();
	}
	//Auto Motion Simulation End
   //自動流程次數設定
    
    if  (ps2x.ButtonPressed(PSB_PAD_DOWN)){                   //當壓著下鍵時動作
      if (ps2x.Analog(PSS_RY) < 120) {                        //當右搖桿數值小於120時
      if (frequency<1000)                                     //當次數小於1000
      {
        frequency++;                                          //次數+1
      }
      else
      {
        frequency = 1000;                                     //次數強制設為1000，防止超過限度
      }
      
      Serial.println("frequency");                            //監控視窗列印字串
      Serial.println(frequency);                              //監控視窗列印數值
      
      lcd.clear();                                           //lcd清除先前畫面
      lcd.setCursor(0,0);                                    //設定第一個字顯示座標
      lcd.print("frequency");                                //lcd顯示字串
      lcd.setCursor(0,1);                                    //設定第二個字顯示座標
      lcd.print(frequency);                                  //lcd顯示數值
      
    }
      else if (ps2x.Analog(PSS_RY) > 134) {                  //當右搖桿數值大於134時
      if (frequency>0)                                       //當次數大於0
      {
        frequency--;                                         //次數-1
      }
      else 
      {
        frequency = 0;                                       //次數強制設為0，防止超過限度
      }
      
      Serial.println("frequency");                            //監控視窗列印字串
      Serial.println(frequency);                              //監控視窗列印數值
      
      lcd.clear();                                           //lcd清除先前畫面
      lcd.setCursor(0,0);                                    //設定第一個字顯示座標
      lcd.print("frequency");                                //lcd顯示字串
      lcd.setCursor(0,1);                                    //設定第二個字顯示座標
      lcd.print(frequency);                                  //lcd顯示數值
    }
    else
    {
      Serial.println("frequency");                            //監控視窗列印字串
      Serial.println(frequency);                              //監控視窗列印數值
      
      lcd.clear();                                            //lcd清除先前畫面
      lcd.setCursor(0,0);                                     //設定第一個字顯示座標
      lcd.print("frequency");                                 //lcd顯示字串
      lcd.setCursor(0,1);                                     //設定第二個字顯示座標
      lcd.print(frequency);                                   //lcd顯示數值
      }
      
      delay(1000);                                            //方便人為操控的緩衝時間
    }
}

void angleWrite(int g) {
	switch (g)
	{
	case 1:
		pos[k - 1][2] = angle1;
		break;
	case 2:
		pos[k - 1][2] = angle2;
		break;
	case 3:
		pos[k - 1][2] = angle3;
		break;
	case 4:
		pos[k - 1][2] = angle4;
		break;
	case 5:
		pos[k - 1][2] = angle5;
		break;
	case 6:
		pos[k - 1][2] = angle6;
		break;
	default:
		break;
	}
}
void initial(void) {
	lcd.clear();                                              //lcd清除先前畫面
	lcd.setCursor(0, 0);                                       //設定第一個字顯示座標
	lcd.print("Initial");                                     //lcd顯示字串
	lcd.setCursor(0, 1);                                       //設定第二個字顯示座標
	lcd.print("Start");                                       //lcd顯示字串
	Serial.println("Initial");                                //監控視窗列印字串
	Serial.println("Start");                                  //監控視窗列印字串
	delay(2000);
	angle2 = 95;                                             //servo2角度設定為預設狀態值
	Servo2.write(angle2, SPEEDservo2);                        //以SPEEDservo2的速度轉至angle2角度
	Servo2.wait();                                           //等待servo2動作結束
	Serial.println("Servo2");                                 //監控視窗列印字串
	Serial.println(angle2);                                   //監控視窗列印角度數值
	lcd.clear();                                              //lcd清除先前畫面
	lcd.setCursor(0, 0);                                       //設定第一個字顯示座標
	lcd.print("Servo2");                                      //lcd顯示字串
	lcd.setCursor(0, 1);                                       //設定第二個字顯示座標
	lcd.print(angle2);                                        //lcd顯示角度數值
	delay(1000);                                              //緩衝時間
	angle3 = 50;                                              //servo3角度設定為預設狀態值
	Servo3.write(angle3, SPEED);                               //以SPEED的速度轉至angle3角度
	Servo3.wait();                                            //等待servo3動作結束
	Serial.println("Servo3");                                  //監控視窗列印字串
	Serial.println(angle3);                                    //監控視窗列印角度數值
	lcd.clear();                                              //lcd清除先前畫面
	lcd.setCursor(0, 0);                                       //設定第一個字顯示座標
	lcd.print("Servo3");                                      //lcd顯示字串
	lcd.setCursor(0, 1);                                       //設定第二個字顯示座標
	lcd.print(angle3);                                        //lcd顯示角度數值
	delay(1000);                                              //緩衝時間
	angle4 = 94;                                              //servo4角度設定為預設狀態值
	Servo4.write(angle4, SPEED);                               //以SPEED的速度轉至angle4角度
	Servo4.wait();                                            //等待servo4動作結束
	Serial.println("Servo4");                                  //監控視窗列印字串
	Serial.println(angle4);                                    //監控視窗列印角度數值
	lcd.clear();                                              //lcd清除先前畫面
	lcd.setCursor(0, 0);                                       //設定第一個字顯示座標
	lcd.print("Servo4");                                      //lcd顯示字串
	lcd.setCursor(0, 1);                                       //設定第二個字顯示座標
	lcd.print(angle4);                                        //lcd顯示角度數值
	delay(1000);                                              //緩衝時間
	angle5 = 90;                                              //servo5角度設定為預設狀態值
	Servo5.write(angle5, SPEED);                               //以SPEED的速度轉至angle5角度
	Servo5.wait();                                            //等待servo5動作結束
	Serial.println("Servo5");                                  //監控視窗列印字串
	Serial.println(angle5);                                    //監控視窗列印角度數值
	lcd.clear();                                              //lcd清除先前畫面
	lcd.setCursor(0, 0);                                       //設定第一個字顯示座標
	lcd.print("Servo5");                                      //lcd顯示字串
	lcd.setCursor(0, 1);                                       //設定第二個字顯示座標
	lcd.print(angle5);                                        //lcd顯示角度數值
	delay(1000);                                              //緩衝時間
	angle1 = 96;                                              //servo1角度設定為預設狀態值
	Servo1.write(angle1, SPEED);                               //以SPEED的速度轉至angle1角度
	Servo1.wait();                                            //等待servo1動作結束
	Serial.println("Servo1");                                 //監控視窗列印字串
	Serial.println(angle1);                                   //監控視窗列印角度數值
	lcd.clear();                                             //lcd清除先前畫面
	lcd.setCursor(0, 0);                                      //設定第一個字顯示座標
	lcd.print("Servo1");                                     //lcd顯示字串
	lcd.setCursor(0, 1);                                      //設定第二個字顯示座標
	lcd.print(angle1);                                       //lcd顯示角度數值
	delay(1000);                                             //緩衝時間
	angle6 = 65;                                             //servo6角度設定為預設狀態值
	Servo6.write(angle6, SPEED);                              //以SPEED的速度轉至angle6角度
	Servo6.wait();                                           //等待servo6動作結束
	Serial.println("Gripper");                                //監控視窗列印字串
	Serial.println(angle6);                                   //監控視窗列印角度數值
	lcd.clear();                                             //lcd清除先前畫面
	lcd.setCursor(0, 0);                                      //設定第一個字顯示座標
	lcd.print("Gripper");                                    //lcd顯示字串
	lcd.setCursor(0, 1);                                      //設定第二個字顯示座標
	lcd.print(angle6);                                       //lcd顯示角度數值
	delay(1000);                                             //緩衝時間
	Serial.println("Initial");                                //監控視窗列印字串
	Serial.println("Finish");                                 //監控視窗列印字串
	lcd.clear();                                              //清除先前畫面
	lcd.setCursor(0, 0);                                       //設定第一個字顯示座標
	lcd.print("Initial");                                     //顯示字串
	lcd.setCursor(0, 1);                                       //設定第二個字顯示座標
	lcd.print("Finish");                                      //顯示字串
	delay(1000);
	kill68 = true;
}
void lcdWelcome(void) {
	lcd.clear();                    //lcd清除先前畫面
	lcd.setCursor(0, 0);             //設定第一個字顯示座標
	lcd.print("Hello!");            //lcd顯示字串
	lcd.setCursor(0, 1);             //設定第二個字顯示座標
	lcd.print("Ready to go");       //lcd顯示字串
}
void servoAutoMotion(int servoNO, int servoAngle) {             //自動化函數
	switch (servoNO)                                              //選擇動作Servo
	{
	case 1:
		Servo1.write(servoAngle, SPEED);                            //利用控速函式庫控速旋轉至寫入的角度
		Servo1.wait();                                              //等待旋轉至角度才進行下個動作
		break;                                                      //跳出case
	case 2:
		Servo2.write(servoAngle, SPEEDservo2);                      //利用控速函式庫控速旋轉至寫入的角度
		Servo2.wait();                                              //等待旋轉至角度才進行下個動作
		break;                                                      //跳出case
	case 3:
		Servo3.write(servoAngle, SPEED);                            //利用控速函式庫控速旋轉至寫入的角度
		Servo3.wait();                                              //等待旋轉至角度才進行下個動作
		break;                                                      //跳出case
	case 4:
		Servo4.write(servoAngle, SPEED);                            //利用控速函式庫控速旋轉至寫入的角度
		Servo4.wait();                                              //等待旋轉至角度才進行下個動作
		break;                                                      //跳出case
	case 5:
		Servo5.write(servoAngle, SPEED);                            //利用控速函式庫控速旋轉至寫入的角度
		Servo5.wait();                                              //等待旋轉至角度才進行下個動作
		break;                                                      //跳出case
	case 6:
		Servo6.write(servoAngle,SPEED);                             //利用控速函式庫控速旋轉至寫入的角度
		Servo6.wait();                                              //等待旋轉至角度才進行下個動作
		break;                                                      //跳出case
    default:                                                    //例外情況
		break;                                                      //跳出case
	}

}
void Initialing(void) {
  lcd.clear();                    //lcd清除先前畫面
  lcd.setCursor(0, 0);             //設定第一個字顯示座標
  lcd.print("Initialing");            //lcd顯示字串
  lcd.setCursor(0, 1);             //設定第二個字顯示座標
  lcd.print("Values");       //lcd顯示字串
}

