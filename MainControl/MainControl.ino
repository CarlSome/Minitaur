// 发送/接收控制端，一般用一个端口控制就好了
int RE1 = 4;  // 接收使能端:0为接收状态
int DE1 = 5;  // 发送使能端:1为发送状态 

// 控制指令
char command; // 指令
int angelInt = 0; // 转动角度int值
byte high = 0x00; // 角度的高位字节(目前角度在±360内，只需要2个字节表示)
byte low = 0x00;  // 角度的低位字节
byte checkSum = 0x00;  // 数据校验和
byte positionCommand[14] = {0x3e, 0xa3, 0x01, 0x08, 0xea, 0x00, 0x00, 0x00, 0x00, 0x00,0x00,0x00,0x00, 0x00}; // 位置闭环控制命令:旋转至0度位置
byte stopCommand[5] = {0x3e, 0x80, 0x01, 0x00, 0xBF}; // 关闭电机命令
byte testCommand[10] = {0x3e, 0xa2, 0x01, 0x04, 0xe5, 0x28, 0x23, 0x00, 0x00, 0x4B};  // 测试命令：匀速转动

// 测试指令
byte positionCommand1[14] = {0x3e, 0xa3, 0x01, 0x08, 0xea, 0x28, 0x23, 0x00, 0x00, 0x00,0x00,0x00,0x00, 0x4b}; // 位置闭环控制命令:旋转至90度位置
byte positionCommand2[14] = {0x3e, 0xa3, 0x01, 0x08, 0xea, 0xD0, 0x07, 0x00, 0x00, 0x00,0x00,0x00,0x00, 0xD7}; // 位置闭环控制命令:旋转至20度位置

// 回复信号
byte result[8];

void setup() {
  pinMode(RE1, OUTPUT);
  pinMode(DE1, OUTPUT);
  Serial.begin(115200); // 与电脑通讯的串口
  Serial1.begin(115200); // 与485通讯的串口

  while(Serial.read()>= 0){}//clear serial port
}

void loop() {
  // 将485设置为发送数据模式
  digitalWrite(RE1,HIGH);
  digitalWrite(DE1,HIGH);

  // 指令控制
  while(Serial.available() > 0) {
    command = Serial.read();
    
    if(command == '0') {
      Serial.println("命令：关闭电机");
      Serial1.write(stopCommand, 10);
    }
    else if(command == '1') {
      Serial.println("请输入需要转动的角度(-9000~9000,1234表示12.34度):");

      // 读取int值
      while(Serial.available() == 0){}
      angelInt = Serial.parseInt();

      // 将int转化为byte
      low = byte(angelInt);
      high = byte(angelInt/(16*16));
      // 对负数，在转换为byte都要-1才是正确值
      if(angelInt < 0) {
        high = high - 1;
      }

      // 计算校验和
      checkSum = high + low;
      if(angelInt < 0) {
        unsigned char i = 0;
        for(i=0; i<6; i++) {
          checkSum = checkSum + 0xFF;
        }
      }

      // 合成指令
      positionCommand[5] = low;
      positionCommand[6] = high;
      positionCommand[13] = checkSum;
      unsigned char i = 0;
      if(angelInt >= 0) {
        for(i=7;i<13;i++) {
          positionCommand[i] = 0x00;
        }
      }
      else {
        for(i=7;i<13;i++) {
          positionCommand[i] = 0xFF;
        }
      }

      // 打印指令
      i = 0;
      for(i=0;i<14;i++) {
        Serial.print(positionCommand[i],HEX);
        Serial.print(" ");
      }
      Serial1.println();

      // 发送指令
      Serial1.write(positionCommand, 14);
    }
    else if(command == '2') {
      Serial.println("命令：匀速转动");
      Serial1.write(testCommand, 10);
    }
    else if(command == 't') {
      Serial1.write(positionCommand1, 14);
      delay(500);
      Serial1.write(positionCommand2, 14);
      delay(500);
      Serial1.write(positionCommand1, 14);
      delay(500);
      Serial1.write(positionCommand2, 14);
      delay(500);
      Serial1.write(positionCommand1, 14);
    }
    else if(command == 'p') {
      Serial1.write(positionCommand1, 14);
      delay(500);
      Serial1.write(positionCommand2, 14);
      delay(500);
      Serial1.write(positionCommand1, 14);
      delay(500);
      Serial1.write(positionCommand2, 14);
      delay(500);
      Serial1.write(positionCommand1, 14);
      Serial.println("Msg from Arduino");
    }
  }
  
  while(Serial1.read()>= 0){}
  delay(1000);

  /*
  digitalWrite(RE,LOW);
  digitalWrite(DE,LOW);
  while(Serial1.available() > 0) {
     //Serial.println(Serial1.read(),HEX);
     byte c = Serial1.read();
     Serial.println(c,HEX);
  }
  */
}
