#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "NETGEAR_Filippo";
const char* password = "Slash19950106";

const char* URL = "http://192.168.0.6";
int sense_Pin = 0; // sensor input at Analog pin A0
int value = 0;
ESP8266WebServer server(80);

//Check if header is present and correct
bool is_authentified() {
  Serial.println("Enter is_authentified");
  if (server.hasHeader("Cookie")) {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
    if (cookie.indexOf("ESPSESSIONID=1") != -1) {
      Serial.println("Authentification Successful");
      return true;
    }
  }
  Serial.println("Authentification Failed");
  return false;
}

//login page, also called for disconnect
void handleLogin() {
  String msg;
  if (server.hasHeader("Cookie")) {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
  }
  if (server.hasArg("DISCONNECT")) {
    Serial.println("Disconnection");
    String header = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=0\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }
  if (server.hasArg("USERNAME") && server.hasArg("PASSWORD")) {
    if (server.arg("USERNAME") == "admin" &&  server.arg("PASSWORD") == "admin" ) {
      String header = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=1\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
      server.sendContent(header);
      Serial.println("Log in Successful");
      return;
    }
    msg = "Wrong username/password! try again.";
    Serial.println("Log in Failed");
  }
  String content = "<!doctype html>";               
  content += "<html lang=\"en\">  <head>";
                   content +=  "<style>body{background: #eee url(http://subtlepatterns.com/patterns/sativa.png);}html,body{    position: relative;    height: 100%;}.login-container{    position: relative;    width: 300px;    margin: 80px auto;    padding: 20px 40px 40px;    text-align: center;    background: #fff;    border: 1px solid #ccc;}#output{    position: absolute;    width: 300px;    top: -75px;    left: 0;    color: #fff;}#output.alert-success{    background: rgb(25, 204, 25);}#output.alert-danger{    background: rgb(228, 105, 105);}.login-container::before,.login-container::after{    content: \"\";    position: absolute;    width: 100%;height: 100%;    top: 3.5px;left: 0;    background: #fff;    z-index: -1;    -webkit-transform: rotateZ(4deg);    -moz-transform: rotateZ(4deg);    -ms-transform: rotateZ(4deg);    border: 1px solid #ccc;}.login-container::after{    top: 5px;    z-index: -2;    -webkit-transform: rotateZ(-2deg);     -moz-transform: rotateZ(-2deg);-ms-transform: rotateZ(-2deg);}.avatar{    width: 100px;height: 100px;    margin: 10px auto 30px;   border-radius: 100%;    border: 2px solid #aaa;    background-size: cover;}.form-box input{    width: 100%;    padding: 10px;    text-align: center;    height:40px;    border: 1px solid #ccc;;    background: #fafafa;    transition:0.2s ease-in-out;}.form-box input:focus{    outline: 0;    background: #eee;}.form-box input[type=\"text\"]{    border-radius: 5px 5px 0 0;    text-transform: lowercase;}.form-box input[type=\"password\"]{    border-radius: 0 0 5px 5px;    border-top: 0;}.form-box button.login{    margin-top:15px;    padding: 10px 20px;}.animated {  -webkit-animation-duration: 1s;  animation-duration: 1s;  -webkit-animation-fill-mode: both;  animation-fill-mode: both;}@-webkit-keyframes fadeInUp {  0% {    opacity: 0;    -webkit-transform: translateY(20px);    transform: translateY(20px);  }  100% {    opacity: 1;    -webkit-transform: translateY(0);    transform: translateY(0);  }}@keyframes fadeInUp {  0% {    opacity: 0;    -webkit-transform: translateY(20px);    -ms-transform: translateY(20px);    transform: translateY(20px);  }  100% {    opacity: 1;    -webkit-transform: translateY(0);    -ms-transform: translateY(0);    transform: translateY(0);  }}.fadeInUp {  -webkit-animation-name: fadeInUp;  animation-name: fadeInUp;}</style>";
                   content +=  "<title>Hello, world!</title>  ";
                   content +=  "<meta charset=\"utf-8\">    ;";
                   content +=  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\"> ";
                   content +=  "<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta.2/css/bootstrap.min.css\" integrity=\"sha384-PsH8R72JQ3SOdhVi3uxftmaW6Vc51MKb0q5P2rRUpPvrszuE4W1povHYgTpBfshb\" crossorigin=\"anonymous\"> ";
                   content +=  "</head> ";
                   content +=  "<body>";
                   content +=  "<div class=\"container\">";
                   content +=  "<div class=\"login-container\">";
                   content +=  "<div id=\"output\"></div>";
                   content +=  "<div class=\"avatar\"></div>";
                   content +=  "<div class=\"form-box\">";
                   content +=  "<form action='/login' method='POST'>To log in, please use : admin/admin";
  content += "User:<input type='text' name='USERNAME' placeholder='user name'><br>";
  content += "Password:<input type='password' name='PASSWORD' placeholder='password'><br>";
  content += "<button class=\"btn btn-info btn-block login\" type=\"submit\">Login</button></form> </div>        </div>    </div>" + msg + "<br>";
  content += "You also can go <a href='/inline'>here</a></body></html>";
  server.send(200, "text/html", content);
}

//root page can be accessed only if authentification is ok
void handleRoot() {
  Serial.println("Enter handleRoot");
  String header;
  if (!is_authentified()) {
    String header = "HTTP/1.1 301 OK\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }
  String content = "<!doctype html>";
                   content += "<html lang=\"en\"> ";
                   content += "<head>";
                   content += "<title>Orto</title>    ";
                   content += "<meta charset=\"utf-8\">    ";
                   content +="<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\">    ";
                   content +="<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta.2/css/bootstrap.min.css\" integrity=\"sha384-PsH8R72JQ3SOdhVi3uxftmaW6Vc51MKb0q5P2rRUpPvrszuE4W1povHYgTpBfshb\" crossorigin=\"anonymous\"> ";
                   content +="</head>  ";
                   content += "<body>";
                   content +="<H2>Hey!</H2><br>";
  

  
  String strval = String(value);
      content += "Soil Moisture:"+strval+" <br>";
  content += "<a href=\"/login?DISCONNECT=YES\">disconnect</a>"
             + strval + "</body>"
             + "</html>";

  server.send(200, "text/html", content);
}

//no need authentification
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


  server.on("/", handleRoot);
  server.on("/login", handleLogin);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works without need of authentification");
  });

  server.onNotFound(handleNotFound);
  //here the list of headers to be recorded
  const char * headerkeys[] = {"User-Agent", "Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
  //ask server to track these headers
  server.collectHeaders(headerkeys, headerkeyssize );
  server.begin();
  Serial.println("HTTP server started");
}
void makerequest()
{
  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
    HTTPClient http;
   http.begin(URL);      //Specify request destination
   http.addHeader("Content-Type", "text/plain");  //Specify content-type header
 
   int httpCode = http.POST("Message from ESP8266");   //Send the request
   String payload = http.getString();                  //Get the response payload
 
   Serial.println(httpCode);   //Print HTTP return code
   Serial.println(payload);    //Print request response payload
 
   http.end();  //Close connection
 
}
}

void loop() {
  server.handleClient();
  Serial.print("MOISTURE LEVEL : ");
   //value= analogRead(sense_Pin);
   value= 19;
   Serial.println(value);
   makerequest();
   
}



