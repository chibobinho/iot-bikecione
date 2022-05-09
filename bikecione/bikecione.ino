#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h> 

#define RST_PIN         9  //PINO Q TA A RST
#define SS_PIN          10 //PINO Q TA A SS   

MFRC522 mfrc522(SS_PIN, RST_PIN); // MFRC INSTANCIADO

void setup() {
  Serial.begin(9600);                              //COMUNICAÇÃO COM O SERIAL, ALTERAR CONFORME O VALOR
  SPI.begin();                                     //USO DA BIBLIOTECA SPI, USADA PARA MOSTRAR OS DADOS NO MONITOR SERIAL
  delay(4000);
  
  WiFi.begin("Vermelho", "Vermelho@132");          //CONEXÃO WIFI
  while (WiFi.status() != WL_CONNECTED) {          //ESPERA SE CONECTAR AO WIFI
    delay(500);
    Serial.println("Esperando conexão");
  }
  
  delay(4000);
  mfrc522.PCD_Init();                              //INICIA O MFRC522
  delay(4000);
  Serial.println(F("Aproxime cartao:"));           //QUANDO ESSA MENSAGEM APARECER, O SERIAL ESTÁ PRONTO PARA LER O CARTÃO
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {                 //CONFERE CONEXÃO WIFI
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
    }
      else{
      Serial.println("usuario não verificado, acesso negado");
      //E NÃO ENVIAR A REQUISIÇÃO
    }
  } else {
    Serial.println("Erro na conexao WiFI");
  }
}
