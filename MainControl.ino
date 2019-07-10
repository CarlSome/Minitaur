// 发送/接收控制端，一般用一个端口控制就好了
int RE1 = 4;
int DE1 = 5;
int RE2 = 6;   // 接收使能端:0为接收状态
int DE2 = 7;   // 发送使能端:1为发送状态

// 控制指令
char command;
byte torqueCommand[10] = {0x3e, 0xa1, 0x01, 0x02, 0xe2, 0x00, 0x01, 0x01}; // 扭矩闭环控制命令:扭矩参数256(比例)
byte speedCommand[10] = {0x3e, 0xa2, 0x01, 0x04, 0xe5, 0x28, 0x23, 0x00, 0x00, 0x4b}; // 速度闭环控制命令:角速度90dps
byte positionCommand[14] = {0x3e, 0xa3, 0x01, 0x08, 0xea, 0x28, 0x23, 0x00, 0x00, 0x00,0x00,0x00,0x00, 0x4b};  // 位置闭环控制命令:旋转90度
byte stopCommand[10] = {0x3e, 0xa2, 0x01, 0x04, 0xe5, 0x00, 0x00, 0x00, 0x00, 0x00};  // 暂停命令:将速度设为0
byte readCommand[5] = {0x3e, 0x90, 0x01, 0x00, 0xcf}; // 读取编码器命令: 

// 回复信号
byte result[8];

void setup() {
  pinMode(RE1, OUTPUT);
  pinMode(DE1, OUTPUT);
  pinMode(RE2, OUTPUT);
  pinMode(DE2, OUTPUT);
  Serial.begin(115200); // 与电脑通讯的串口
  Serial1.begin(115200); // 与485通讯的串口

  while(Serial.read()>= 0){}//clear serial port
}

void loop() {
  // 将485设置为发送数据模式
  digitalWrite(RE1,HIGH);
  digitalWrite(DE1,HIGH);
  digitalWrite(RE2,HIGH);
  digitalWrite(DE2,HIGH);

  // 指令控制
  while(Serial.available() > 0) {
    command = Serial.read();
    if(command == '0') {
      Serial.println("命令：停止");
      Serial1.write(stopCommand, 10);
    }
    else if(command == '1') {
      Serial.println("命令：速度闭环");
      Serial1.write(speedCommand, 10);
    }
    else if(command == '2') {
      Serial.println("命令：位置闭环");
      Serial1.write(positionCommand, 14);
    }
    else if(command == '3') {
      Serial.println("命令：读取编码器");
      Serial1.write(readCommand, 5);
      /*
      digitalWrite(RE,LOW);
      digitalWrite(DE,LOW);
      while(Serial1.available() > 0) {
        Serial.print("读取到：");
        byte c = Serial1.read();
        Serial.println(c,HEX);
      }
      */
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
