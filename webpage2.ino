// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Replace with your network credentials
const char* ssid = "ASUS_X00TD";
const char* password = "chocolaty";

#define DHTPIN 19    // Digital pin connected to the DHT sensor

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readDHTTemperature() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float t = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}

String readDHTHumidity() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(h);
    return String(h);
  }
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
   .clock{
    display: flex;
} 
.clock div{
    margin: 5px;
    position: relative;
    left: 500px;
    bottom: 100px;
    
}
.clock span{
    width: 100px;
    height: 80px;
    background: teal; 
    opacity: 50%;
    color:white;
    display: flex;
    justify-content: center;
    align-items: center;
    font-size: 50px;
    text-shadow: 2px 2px 4px rgba(0,0,0,.3);
}
.clock .text {
    height: 30px;
    font-size: 10px;
    text-transform: uppercase;
    letter-spacing: 2px;
    background: rgb(13, 68, 77);
    opacity: 0.8;
}
.clock #ampm{
    bottom: 0;
    position: absolute;
    width: 60px;
    height: 30px;
    font-size: 20px;
    background: rgb(104, 71, 9);
}
body{
    margin: 0;
    position: relative;
    font-family: sans-serif;
    display: flex;
    flex-direction: column;
    justify-content:center;
    align-items:center;
    height: 100vh;
    background: url(https://images.unsplash.com/photo-1475924156734-496f6cac6ec1?ixlib=rb-1.2.1&ixid=MnwxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8&auto=format&fit=crop&w=2070&q=80);
    background-size: cover;
    overflow: hidden;
}  
    h2 { font-size: 3.0rem; }
    span { font-size: 3.0rem;
      width: 300px;
    height: 80px;
    background: rgb(7, 105, 141); 
    opacity: .5;
    color:white;
    display: flex;
    justify-content: center;
    align-items: center;
    font-size: 25px;
    text-shadow: 2px 2px 4px rgba(0,0,0,.3);
    border-radius: 10px;
   }
   p{
    border: 6px solid rgb(19, 119, 177);
    position: relative;
    top: -50px;
    border-radius: 20px;
    box-shadow: 0 0 50px 0 rgb(23, 130, 168),
                    0 0 50px 0 rgb(19, 133, 182) inset,
                    0 0 50px 0 rgb(10, 80, 151) inset;
   }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
    .button{
    font-size: 25px;
    text-align:center;
    right: 150px;
    bottom: 90px;
    position: absolute;
    width: 200px;
    height: 75px;
    line-height: 1.90;
    justify-content: center;
    text-decoration: none;
    background-color: rgb(138, 26, 26);
    border: 1px solid crimson;
    color: #fff;
    border-radius: 25px;
    padding: 8px 0;
    opacity: 0.7;
    box-shadow:rgb(18, 150, 141);
}
.button:hover{
    box-shadow: 0 0 30px 0 crimson,
                    0 0 30px 0 crimson inset,
                    0 0 30px 0 crimson inset;
    
  }
  </style>
</head>
<body>
  <h2>ESP32 DHT Server</h2>
  <div class="clock">
        <div>
            <span id="hour">00</span>
            <span class="text">Hours</span>
        </div>
        <div>
            <span id="min">00</span>
            <span class="text">Mins</span>
        </div>
        <div>
            <span id="sec">00</span>
            <span class="text">Secs</span>
        </div>
        <div>
            <span id="ampm">AM</span>
        </div>        
    </div>
  <p>
    <i class="fas fa-thermometer-half" style="color:#10108e;"></i> 
    <span class="dht-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%&deg;C</span>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00c4d6;"></i> 
    <span class="dht-labels">Humidity</span>
    <span id="humidity">%HUMIDITY%&percnt;</span>
  </p>
   <button onclick="window.location.href='http://192.168.0.120/';" class="button">Start stream</button>
</body>
<script>
  const hourE1=document.getElementById("hour");
const minuteE1=document.getElementById("min");
const secondE1=document.getElementById("sec");
const ampmE1=document.getElementById("ampm");
setInterval(function ( ){
    let h = new Date().getHours();
    let m = new Date().getMinutes();
    let s = new Date().getSeconds();
    let ampm = "AM";
    if(h>12)
    {
        h=h-12;
        ampm = "PM";
    }
    h = h<10 ? "0" + h: h;
    m = m<10 ? "0" + m: m;
    s = s<10 ? "0" + s: s;
    hourE1.innerText = h;
    minuteE1.innerText = m;
    secondE1.innerText = s;
    ampmE1.innerText = ampm;
    },1000);

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 10000 ) ;
</script>
</html>)rawliteral";

// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return readDHTTemperature();
  }
  else if(var == "HUMIDITY"){
    return readDHTHumidity();
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  dht.begin();
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTHumidity().c_str());
  });

  // Start server
  server.begin();
}
 
void loop(){
  
}
