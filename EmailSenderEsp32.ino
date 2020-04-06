#include <DHT.h>
#include "ESP32_MailClient.h"


int pinSenzorAer = 35;
float senzorAer;
String calitateAer;

#define DHTPIN 4 
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
float umiditate;
float temperatura;

// TREBUIE INLOCUIT CU CREDENTIALELE WIFI PERSONALE
const char* ssid = "DIGI-24-75831C";
const char* password = "831E020293";

// Pentru a trimite emailuri de pe gmail trebuie folosit portul 465 ssl si serverul smtp: smtp.gmail.com
// Trebuie bifata casuta "lasa google sa fie folosit de aplicatii nesigure": https://myaccount.google.com/lesssecureapps?pli=1
#define emailSenderAccount    "smartech.iot@gmail.com"    
#define emailSenderPassword   "AAAaaa111222"
#define emailRecipient        "dcpiturlea@gmail.com"
#define smtpServer            "smtp.gmail.com"
#define smtpServerPort        465
#define emailSubject          "ESP32 Test"




void setup(){
  // crerea unui obiect SMTPDATA
SMTPData smtpData;
}
 
  
void loop() {
  dht.begin();
  CitireSenzorAer();
  CitireDHT();
  WIFIConnect();
  SMTPConnect();
  EmailSettings();
  SendEmail();
}



// Callback function to get the Email sending status
void sendCallback(SendStatus info);

// functie sendCallBack
void sendCallback(SendStatus msg) {
  // Afisarea statusului curent
  Serial.println(msg.info());

  // printare mesaj daca s-a trimis
  if (msg.success()) {
    digitalWrite(5, HIGH);
    Serial.println("Done, the email was send");
  }
}



void WIFIConnect(){
  Serial.begin(115200);
  Serial.print("Connecting");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }

  Serial.println();
  Serial.println("WiFi connected.");
  Serial.println();
}



void SMTPConnect(){
  Serial.println("Preparing to send email");
  Serial.println();
  
  // setarea parametrilor obiectului tip SMTPDATA, metoda setarea conectarii
  smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);

  // For library version 1.2.0 and later which STARTTLS protocol was supported,the STARTTLS will be 
  // enabled automatically when port 587 was used, or enable it manually using setSTARTTLS function.
  //smtpData.setSTARTTLS(true);
}




void EmailSettings(){
  / setarea adresei 
  smtpData.setSender("ESP32", emailSenderAccount);

  // setarea prioritatii emailului
  smtpData.setPriority("High");

  // setarea subiectului
  smtpData.setSubject(emailSubject);

  // Set the message with HTML format
  //smtpData.setMessage("<div style=\"color:#2f4468;\"><h1>Hello World!</h1><p>- Sent from ESP32 board</p></div>", true);
  // Set the email message in text format (raw)
  smtpData.setMessage("Calitate Aer: "+calitateAer+" / 500), Temperatura: "+temperatura
  +" *C, Umiditate: "+umiditate+" %", false);

  // adaugare destinatari, poti fi mai multi
  smtpData.addRecipient(emailRecipient);
 // smtpData.addRecipient("sorinbratosin93@yahoo.com");
  //smtpData.addRecipient("cristinafmilitaru@gmail.com");
  //smtpData.addRecipient("ciobanuciprian0000@gmail.com");
    //smtpData.addRecipient("cristian.popa@yahoo.com");
  //smtpData.addRecipient("ivanciu_emanoil@yahoo.com");
  //smtpData.addRecipient("YOUR_OTHER_RECIPIENT_EMAIL_ADDRESS@EXAMPLE.com");

  smtpData.setSendCallback(sendCallback);
}




void SendEmail(){
  
  //Se incearca trimiterea mailului,
  if (!MailClient.sendMail(smtpData))
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());

  //sterge toate datele din email pentru a goli memoria
  smtpData.empty();
}




void CitireSenzorAer(){
  senzorAer=analogRead(pinSenzorAer);
  calitateAer= String(senzorAer);
}

void CitireDHT(){
  umiditate = dht.readHumidity();
  temperatura=dht.readTemperature();
}
