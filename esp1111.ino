#include "IRremote.h"
#include <ESP8266WiFi.h>                                                // Библиотека для создания Wi-Fi подключения (клиент или точка доступа)
#include <ESP8266WebServer.h>                                           // Библиотека для управления устройством по HTTP (например из браузера)
#include <FS.h>                                                         // Библиотека для работы с файловой системой
#include <ESP8266FtpServer.h>    // Библиотека для работы с SPIFFS по FTP



int espOut = 16; //D0
int espIn = 5; //D1
int timeOut = 1000;
IRrecv irrecv(4);
decode_results results;


const char *ssid = "Remote Control";                                             // Название генерируемой точки доступа
const char *password = "projectService";

ESP8266WebServer HTTP(80);                                              // Определяем объект и порт сервера для работы с HTTP
FtpServer ftpSrv;                                                       // Определяем объект для работы с модулем по FTP (для отладки HTML)

void sendFromIRToStm(long int value) {
  switch (value) {
    case 3877175040:
      sendAction(5);
      break ;
    case 2774204160:
      sendAction(20);
      break ;
    case 2907897600:
      sendAction(10);
      break ;
    case 4144561920:
      sendAction(15);
      break ;
    case 3810328320:
      sendAction(80);
      break ;
    case 3125149440:
      sendAction(35);
      break ;
    case 3108437760:
      sendAction(40);
      break ;
    case 3091726080:
      sendAction(45);
      break ;
    case 3141861120:
      sendAction(50);
      break ;
    case 3208707840:
      sendAction(55);
      break ;
    case 3158572800:
      sendAction(60);
      break ;
    case 4161273600:
      sendAction(65);
      break ;
    case 3927310080:
      sendAction(70);
      break ;
    case 4127850240:
      sendAction(75);
      break ;
    case 3860463360:
      sendAction(30);
      break ;
    case 3910598400:
      sendAction(85);
      break ;
    case 4061003520:
      sendAction(90);
      break ;
    default:
      break ;
  }
}

void setup() {
  pinMode(espOut, OUTPUT); 
  pinMode(espIn, INPUT);
  irrecv.enableIRIn(); 

  Serial.begin(9600);                                                   // Инициализируем вывод данных на серийный порт со скоростью 9600 бод

  WiFi.softAP(ssid, password);                                                    // Создаём точку доступа

  SPIFFS.begin();                                                       // Инициализируем работу с файловой системой
  HTTP.begin();                                                         // Инициализируем Web-сервер
  ftpSrv.begin("relay", "relay");                                       // Поднимаем FTP-сервер для удобства отладки работы HTML (логин: relay, пароль: relay)
  
  Serial.print("\nMy IP to connect via Web-Browser or FTP: ");          // Выводим на монитор серийного порта сообщение о том, что сейчас будем выводить локальный IP
  Serial.println(WiFi.softAPIP());                                      // Выводим локальный IP-адрес ESP8266
  Serial.println("\n");

  // Обработка HTTP-запросов

   HTTP.on("/up", [](){
    HTTP.send(200, "text/plain", sendAction(5));
  });
  HTTP.on("/down", [](){
    HTTP.send(200, "text/plain", sendAction(10));
  });
  HTTP.on("/left", [](){
    HTTP.send(200, "text/plain", sendAction(15));
  });
  HTTP.on("/right", [](){
    HTTP.send(200, "text/plain", sendAction(20));
  });
  HTTP.on("/0", [](){
    HTTP.send(200, "text/plain", sendAction(30));
  });
  HTTP.on("/1", [](){
    HTTP.send(200, "text/plain", sendAction(35));
  });
  HTTP.on("/2", [](){
    HTTP.send(200, "text/plain", sendAction(40));
  });
  HTTP.on("/3", [](){
    HTTP.send(200, "text/plain", sendAction(45));
  });
  HTTP.on("/4", [](){
    HTTP.send(200, "text/plain", sendAction(50));
  });
  HTTP.on("/5", [](){
    HTTP.send(200, "text/plain", sendAction(55));
  });
  HTTP.on("/6", [](){
    HTTP.send(200, "text/plain", sendAction(60));
  });
  HTTP.on("/7", [](){
    HTTP.send(200, "text/plain", sendAction(65));
  });
  HTTP.on("/8", [](){
    HTTP.send(200, "text/plain", sendAction(70));
  });
  HTTP.on("/9", [](){
    HTTP.send(200, "text/plain", sendAction(75));
  });
  HTTP.on("/ok", [](){
    HTTP.send(200, "text/plain", sendAction(80));
  });
  HTTP.on("/r", [](){
    HTTP.send(200, "text/plain", sendAction(85));
  });
  HTTP.on("/d", [](){
    HTTP.send(200, "text/plain", sendAction(90));
  });
  HTTP.on("/O", [](){
    HTTP.send(200, "text/plain", sendAction(95));
  });
  HTTP.on("/V", [](){
    HTTP.send(200, "text/plain", sendAction(100));
  });
  HTTP.on("/I", [](){
    HTTP.send(200, "text/plain", sendAction(105));
  });
  HTTP.on("/P", [](){
    HTTP.send(200, "text/plain", sendAction(25));
  });
  
  HTTP.onNotFound([]() {                                                // Описываем действия при событии "Не найдено"
    if (!handleFileRead(HTTP.uri()))                                      // Если функция handleFileRead (описана ниже) возвращает значение false в ответ на поиск файла в файловой системе
      HTTP.send(404, "text/plain", "Not Found");                        // возвращаем на запрос текстовое сообщение "File isn't found" с кодом 404 (не найдено)
  });
  digitalWrite(espOut, LOW);
}

void loop() {
  HTTP.handleClient();                                                // Обработчик HTTP-событий (отлавливает HTTP-запросы к устройству и обрабатывает их в соответствии с выше описанным алгоритмом)
  ftpSrv.handleFTP();                                                 // Обработчик FTP-соединений
  if(IrReceiver.decode()){
    if(IrReceiver.decodedIRData.decodedRawData != 0){
//      IrReceiver.printIRResultShort(&Serial);
      if(IrReceiver.decodedIRData.decodedRawData == 0xFFFFFFFF){
        Serial.println("...");
      }else{
        Serial.print("command:");
        Serial.println(IrReceiver.decodedIRData.decodedRawData);
        sendFromIRToStm(IrReceiver.decodedIRData.decodedRawData);
      }
    }
    //Enable receiving of the next value
    IrReceiver.resume(); 
  }
}


String sendAction(int actionTime) {
  int timeStart = millis();
  while (!digitalRead(espIn)) {
    
    if (millis() - timeStart > timeOut) {
      Serial.println("error");
      digitalWrite(espOut, LOW);
      return ("error");
    }
    
    digitalWrite(espOut, HIGH);
  }
  
  digitalWrite(espOut, LOW);
  delay(100);
  
  digitalWrite(espOut, HIGH);
  delay(actionTime);
  digitalWrite(espOut, LOW);
  
  Serial.println(actionTime);
  return ((String)actionTime);
}

bool handleFileRead(String path) {                                      // Функция работы с файловой системой
  if (path.endsWith("/")) path += "index.html";                         // Если устройство вызывается по корневому адресу, то должен вызываться файл index.html (добавляем его в конец адреса)
  String contentType = getContentType(path);                            // С помощью функции getContentType (описана ниже) определяем по типу файла (в адресе обращения) какой заголовок необходимо возвращать по его вызову
  if (SPIFFS.exists(path)) {                                            // Если в файловой системе существует файл по адресу обращения
    File file = SPIFFS.open(path, "r");                                 //  Открываем файл для чтения
    size_t sent = HTTP.streamFile(file, contentType);                   //  Выводим содержимое файла по HTTP, указывая заголовок типа содержимого contentType
    file.close();                                                       //  Закрываем файл
    return true;                                                        //  Завершаем выполнение функции, возвращая результатом ее исполнения true (истина)
  }
  return false;                                                         // Завершаем выполнение функции, возвращая результатом ее исполнения false (если не обработалось предыдущее условие)
}

String getContentType(String filename) {                                // Функция, возвращающая необходимый заголовок типа содержимого в зависимости от расширения файла
  if (filename.endsWith(".html")) return "text/html";                   // Если файл заканчивается на ".html", то возвращаем заголовок "text/html" и завершаем выполнение функции
  else if (filename.endsWith(".css")) return "text/css";                // Если файл заканчивается на ".css", то возвращаем заголовок "text/css" и завершаем выполнение функции
  else if (filename.endsWith(".js")) return "application/javascript";   // Если файл заканчивается на ".js", то возвращаем заголовок "application/javascript" и завершаем выполнение функции
  else if (filename.endsWith(".png")) return "image/png";               // Если файл заканчивается на ".png", то возвращаем заголовок "image/png" и завершаем выполнение функции
  else if (filename.endsWith(".jpg")) return "image/jpeg";              // Если файл заканчивается на ".jpg", то возвращаем заголовок "image/jpg" и завершаем выполнение функции
  else if (filename.endsWith(".gif")) return "image/gif";               // Если файл заканчивается на ".gif", то возвращаем заголовок "image/gif" и завершаем выполнение функции
  else if (filename.endsWith(".ico")) return "image/x-icon";            // Если файл заканчивается на ".ico", то возвращаем заголовок "image/x-icon" и завершаем выполнение функции
  return "text/plain";                                                  // Если ни один из типов файла не совпал, то считаем что содержимое файла текстовое, отдаем соответствующий заголовок и завершаем выполнение функции
}
