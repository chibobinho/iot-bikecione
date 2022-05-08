#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9  //PINO Q TA A RST
#define SS_PIN          10 //PINO Q TA A SS   

MFRC522 mfrc522(SS_PIN, RST_PIN); // MFRC INSTANCIADO

void setup() {
  Serial.begin(9600);                              //COMUNICAÇÃO COM O SERIAL, ALTERAR CONFORME O VALOR
  SPI.begin(),                                     //USO DA BIBLIOTECA SPI, USADA PARA MOSTRAR OS DADOS NO MONITOR SERIAL
  mfrc522.PCD_Init();                              //INICIA O MFRC522
  Serial.println(F("Aproxime cartao:"));           //QUANDO ESSA MENSAGEM APARECER, O SERIAL ESTÁ PRONTO PARA LER O CARTÃO
}

void loop() {
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
    
  }
  else{
    Serial.println("usuario não verificado, acesso negado");
    //E NÃO ENVIAR A REQUISIÇÃO
  }
}
