#include <ESP8266HTTPClient.h>
#include <MFRC522.h>
#include <SPI.h>
#include <ESP8266.h>


#define RST_PIN         9  //PINO Q TA A RST
#define SS_PIN          10 //PINO Q TA A SS   

MFRC522 mfrc522(SS_PIN, RST_PIN); // MFRC INSTANCIADO


// Cria uma serial nas portas 2 (RX) e 3 (TX)
SoftwareSerial mySerial(2 , 3);

// Define que o modulo ira utilizar a serial minhaSerial
ESP8266 wifi(mySerial);

// Configuracao ID e senha da rede Wireless
#define SSID        "Vermelho"
#define PASSWORD    "Vermelho@132"

void setup()
{
  // Define que o modulo ira utilizar a serial minhaSerial
  Serial.begin(9600);
  SPI.begin();                                     //USO DA BIBLIOTECA SPI, USADA PARA MOSTRAR OS DADOS NO MONITOR SERIAL
  delay(4000);
  mfrc522.PCD_Init();                              //INICIA O MFRC522
}

void loop()
{
  Serial.print("Inicializando modulo\r\n");
  Serial.print("Versao do firmware: ");
  Serial.println(wifi.getVersion().c_str());
  // Define modo de operacao como STA (station)
  if (wifi.setOprToStation()) {
    Serial.print("Modo STA ok\r\n");

  } else {
    Serial.print("Erro ao definir modo STA !r\n");
  }

  // Conexao a rede especificada em SSID
  if (wifi.joinAP(SSID, PASSWORD)) {
    Serial.print("Conectado com sucesso a rede wireless\r\n");
    Serial.print("IP: ");
    Serial.println(wifi.getLocalIP().c_str());
    
    Serial.println(F("Aproxime cartao:"));           //QUANDO ESSA MENSAGEM APARECER, O SERIAL ESTÁ PRONTO PARA LER O CARTÃO
    HTTPClient http;    //INSTANCIA HTTP
    WiFiClient client;  //INSTANCIA WIFICLIENT

    if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
    }

    if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
    }
    Serial.println("ID tag: ");
  
    String content= "";
    byte letter;
    for(byte i = 0; i < mfrc522.uid.size; i++)
      {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
      content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      content.concat(String(mfrc522.uid.uidByte[i], HEX));
      }
    Serial.println();
    Serial.print("Mensagem: ");
    content.toUpperCase();
    if (content.substring(1) == "YURI")
    {
    Serial.println("usuario verificado");
    //E AQUI ENVIAR REQUISIÇÃO
    http.begin(client, "http://192.168.1.88:8085/hello");      //DESTINO DA REQUISIÇÃO
    http.addHeader("Content-Type", "text/plain");      //ESPECIFICAR CONTENT TYPE
 
    int httpCode = http.POST("Mensagem do ESP8266");   //ENVIA A REQUISIÇÃO
    String payload = http.getString();                 //Get the response payload
 
    Serial.println(httpCode);   //PRINTA HTTP NO SERIAL
    Serial.println(payload);    //Print request response payload
 
    http.end();  //FECHA CONEXÃO

  } else {
    Serial.print("Erro ao conectar rede wireless !!!\r\n");
  }

  Serial.print(" Fim \r\n");
  while (1) {}
}
