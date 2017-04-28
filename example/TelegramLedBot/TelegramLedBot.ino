#include <ESP8266WiFi.h> //Video: https://www.youtube.com/watch?v=XWMOyjoGw_U
#include <WiFiClientSecure.h>
#include <ESPTelegramBOT.h>

// Initialize Wifi connection to the router
char ssid[] = "xxxxxxxxxxxxx";
char password[] = "yyyyyyyyyyy";

// Initialize Telegram BOT
#define BOTtoken "343986945:AAGuf50736pi2tmkMuWW35DhFmFl3xcp4Uo"  //token of BOT
//search in Telegram the Bot: sanusblaese


TelegramBOT bot(BOTtoken);

int Bot_mtbs = 2000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done
bool Start = false;
int ledStatus = 0;

void setup() {
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
  bot.begin();      // launch Bot functionalities
  pinMode(13, OUTPUT); // initialize digital pin 13 (D7) as an output.
  digitalWrite(13, HIGH);
}

void loop() {
  if (millis() > Bot_lasttime + Bot_mtbs)  { //verifica tempo definido de leitura
   
    bot.getUpdates(bot.message[0][1]);   // launch API GetUpdates up to xxx message (get update_id of last read message)
    
    int conteudoIdAtual = bot.message[0][0].toInt() + 1; //conteudo id_update atual (anterior +1)
   // Serial.println(conteudoIdAtual);    

    for (int i = 1; i < conteudoIdAtual; i++)      {//se tiver conteudo
  
    String update_id = bot.message[i][0];  Serial.print("update_id: "); Serial.println(update_id);   
    String first_name = bot.message[i][1]; Serial.print("first_name: "); Serial.println(first_name);
    String last_name = bot.message[i][2];  Serial.print("last_name: "); Serial.println(last_name);
    String chat_id = bot.message[i][3];    Serial.print("chat_id: "); Serial.println(chat_id);
    String date = bot.message[i][4];       Serial.print("date: "); Serial.println(date);
    String text = bot.message[i][5];       Serial.print("text: "); Serial.println(text);     
   
    if (bot.message[i][5] == "/ledon") {
      ledStatus = 1;
      digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
      bot.sendMessage(chat_id, "Led is ON", "");
    }
    //Escreve a URL https://api.telegram.org/bot343986945:AAGuf50736pi2tmkMuWW35DhFmFl3xcp4Uo/sendMessage?chat_id=309616823&text=Led_is%20ON
    
    if (text == "/ledoff") {
      ledStatus = 0;
      digitalWrite(13, LOW);    // turn the LED off (LOW is the voltage level)
      bot.sendMessage(chat_id, "Led is OFF", "");
    }
    
   if (text == "/status") {
      if(ledStatus){
        bot.sendMessage(chat_id, "Led is ON", ""); 
      } else {
        bot.sendMessage(chat_id, "Led is OFF", "");
      }
    }

    if (text == "/start") {
      String welcome = "Welcome (/start) to SanUSB ESP8266 Telegram Bot library, " + first_name + ". CLick  /ledon : to switch the Led ON.  /ledoff : to switch the Led OFF.  /status : Returns current status of LED.";
      bot.sendMessage(chat_id, welcome, "");
    }
  }
  bot.message[0][0] = "";   //  reset new messages
  //****************************************************************************************  
  
    Bot_lasttime = millis();
  }
}



