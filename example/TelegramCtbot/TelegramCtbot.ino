/*
https://github.com/shurillu/CTBot
https://randomnerdtutorials.com/telegram-control-esp32-esp8266-nodemcu-outputs/
https://sanusb.blogspot.com/2020/06/plataformas-para-deploy-de-aplicacoes.html
Video: https://www.youtube.com/watch?v=XWMOyjoGw_U
*/
#include "CTBot.h" 
CTBot bot; 

//Parâmetros para conexão com wifi e Telegram
String wifi  = "SanUSB";
String senha  = "sanusblaese19";
String token = "XXXXXXXXXX:aaaaaaaaaaaaaa-bbbbbbbbbbb-xxx-yy"; //TOKEN

//Pinos
int rele = 5; 
int sensor = 5; //mesmo pino de atuação do relé, mas pode ser outro pino digital ou analógico com sensor real para comprovar o acionamento
int valorSensor; //float

void setup(){
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
    Serial.println("INICIANDO...");
    pinMode(sensor, INPUT);
    pinMode(rele, OUTPUT);

    //Conexão com wifi e Telegram
    bot.wifiConnect(wifi, senha);
    bot.setTelegramToken(token);
    delay(1000);
   
    if (bot.testConnection()){
        Serial.println("\nCONEXÃO BEM-SUCEDIDA");
    }
    else
        Serial.println("\nFALHA NA CONEXÃO"); 
}    

void loop(){
  TBMessage msg;
  String usuario;
  //valorSensor = analogRead(sensor);
  valorSensor = digitalRead(sensor);
  
  if (bot.getNewMessage(msg)){
    usuario = msg.sender.username;
    Serial.println(usuario + ": " + msg.text);

    if (msg.sender.id != 111111111){// Your CHAT_ID - t.me/myidbot
      bot.sendMessage(msg.sender.id, "Acesso negado");
    }
    else {
      if (msg.text.equalsIgnoreCase("/start")){
        bot.sendMessage(msg.sender.id, "Olá, " + msg.sender.username + "!");      
        bot.sendMessage(msg.sender.id, "Ligue ou desligue remotamente e confira o status da carga.\n\nUtilize os comandos:\n\t\t\t\t\t/status\n\t\t\t\t\t/on\n\t\t\t\t\t/off\n");
      }
      
      else if (msg.text.equalsIgnoreCase("/status")){
        if (valorSensor > 0){
          Serial.println(valorSensor);
          bot.sendMessage(msg.sender.id, "A carga está ligada");
        }
        else if (valorSensor == 0){
          Serial.println(valorSensor);
          bot.sendMessage(msg.sender.id, "A carga está desligada");
        }
      }
      
      else if (msg.text.equalsIgnoreCase("/on")){
        bot.sendMessage(msg.sender.id, "comando liga...");
        digitalWrite(rele, HIGH);
        delay(500);
      }

      else if (msg.text.equalsIgnoreCase("/off")){
        bot.sendMessage(msg.sender.id, "comando desliga...");
        digitalWrite(rele, LOW);
        delay(500);
      }

      else{
        bot.sendMessage(msg.sender.id, "Comando inválido");
        bot.sendMessage(msg.sender.id, "Utilize os comandos:\n\t\t\t\t\t/on\n\t\t\t\t\t/off\n");
      }
    }
  }
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(500);   
  Serial.println("OK");
}
