// Librerias:
#include <WiFi.h>
#include <HttpClient.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

// Constantes iniciales:

#define DELAY 100
#define INICIO 0
#define FIN 9

// Constantes para el wifi:

const char SSID[] = "RoloWiFi";
const char PASS[] = "12345678";
const char* mqtt_server = "test.mosquitto.org";

WiFiClient espClient;
PubSubClient client(espClient);

// Constantes para la matriz:

#define FIL 3
#define COL 3

// Costantes para los pixeles:

#define PIN 19
#define NUM_PIX 9

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIX, PIN, NEO_GRB + NEO_KHZ800);
// Estados:

#define ESTADO_REPOSO 0
#define ESTADO_INICIOV 1
#define ESTADO_INICIO 2
#define ESTADO_ACCIONADO 3
#define ESTADO_TDATOS 4
#define ESTADO_RESPUESTA 5

// Turnos:

#define TUTURNO 1
#define MITURNO 0

// Variables:

int i;
int estado;
int turno;
int cturno;
int vec;
char aux;
int victoria;
int doc;
int selec;
int a;

// Variables vectoriales enteras:

int filas[] = { 36, 39, 34 };
int columnas[] = { 23, 22, 18 };

// Matrizes:
bool matriz[FIL][COL];
bool vector[9];

// Coordenadas:

int ul;
int uc;
int ur;

int cl;
int cc;
int cr;

int dl;
int dc;
int dr;

int espacios;

// Ininiciacion de piexeles:

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
  }
  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);  // Add a unique identifier to the client ID
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("Respuesta");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void ConectarInternet() {

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  setup_wifi();
  pixels.begin();
  ConectarInternet();
  Serial.begin(115200);

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);


  for (int i = 0; i < FIL; i++) {
    pinMode(filas[i], INPUT);
  }
  for (int i = 0; i < COL; i++) {
    pinMode(columnas[i], INPUT);
  }

  Inicial();
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void leerMatriz() {

  vec = 0;

  for (int i = INICIO; i < COL; i++) {

    pinMode(columnas[i], OUTPUT);
    digitalWrite(columnas[i], HIGH);

    for (int j = INICIO; j < FIL; j++) {

      matriz[i][j] = digitalRead(filas[j]);

      vector[vec] = matriz[j][i];
      vec = vec + 1;
    }
    pinMode(columnas[i], INPUT);
  }
}

void MostrarMatriz() {
  Serial.println();
  for (int i = INICIO; i < COL; i++) {
    for (int j = INICIO; j < FIL; j++) {
      Serial.print(matriz[j][i]);
    }
    Serial.println(" ");
  }
}

void Inicial() {

  pixels.setPixelColor(0, pixels.Color(5, 0, 0));
  pixels.setPixelColor(1, pixels.Color(5, 0, 0));
  pixels.setPixelColor(2, pixels.Color(5, 0, 0));
  ul = 2;
  uc = 2;
  ur = 2;

  pixels.setPixelColor(3, pixels.Color(0, 0, 0));
  pixels.setPixelColor(4, pixels.Color(0, 0, 0));
  pixels.setPixelColor(5, pixels.Color(0, 0, 0));
  cl = 0;
  cc = 0;
  cr = 0;

  pixels.setPixelColor(6, pixels.Color(0, 5, 0));
  pixels.setPixelColor(7, pixels.Color(0, 5, 0));
  pixels.setPixelColor(8, pixels.Color(0, 5, 0));
  dl = 1;
  dc = 1;
  dr = 1;

  pixels.show();

  victoria = 0;

  turno = 0;
  cturno = 1;
}

void Hexapawn() {

  switch (turno) {
    case MITURNO:

      Seleccionar();
      Mover();
      Espacios();
      ChequearVictoriaMio();
      break;

    case TUTURNO:
      if (cturno == 2) {
        CondicionalesT2();
        cturno = cturno + 1;
      }
      if (cturno == 4) {
        CondicionalesT4();
        cturno = cturno + 1;
      }
      if (cturno == 6) {
        CondicionalesT6();
        cturno = cturno + 1;
      }

      ChequearVictoriaTuyo();
      Espacios();
      break;
  }
}

void Seleccionar() {
  if (vector[0] && selec != 1 && ul == 1) {
    selec = 1;
  }
  if (vector[1] && selec != 2 && uc == 1) {
    selec = 2;
  }
  if (vector[2] && selec != 3 && ur == 1) {
    selec = 3;
  }
  if (vector[3] && selec != 4 && cl == 1) {
    selec = 4;
  }
  if (vector[4] && selec != 5 && cc == 1) {
    selec = 5;
  }
  if (vector[5] && selec != 6 && cr == 1) {
    selec = 6;
  }
  if (vector[6] && selec != 7 && dl == 1) {
    selec = 7;
  }
  if (vector[7] && selec != 8 && dc == 1) {
    selec = 8;
  }
  if (vector[8] && selec != 9 && dr == 1) {
    selec = 9;
  }
}

void Mover() {
  if (dl == 1 && selec == 7 && vector[3] && cl == 0) {
    dl = 0;
    cl = 1;
    selec = 0;
    turno = 1;
    cturno = cturno + 1;
  }
  if (dl == 1 && selec == 7 && vector[4] && cc == 2) {
    dl = 0;
    cc = 1;
    selec = 0;
    turno = 1;
    cturno = cturno + 1;
  }
  if (dc == 1 && selec == 8 && vector[3] && cl == 2) {
    dc = 0;
    cl = 1;
    selec = 0;
    turno = 1;
    cturno = cturno + 1;
  }
  if (dc == 1 && selec == 8 && vector[4] && cc == 0) {
    dc = 0;
    cc = 1;
    selec = 0;
    turno = 1;
    cturno = cturno + 1;
  }
  if (dc == 1 && selec == 8 && vector[5] && cr == 2) {
    dc = 0;
    cr = 1;
    selec = 0;
    turno = 1;
    cturno = cturno + 1;
  }
  if (dr == 1 && selec == 9 && vector[4] && cc == 2) {
    dr = 0;
    cc = 1;
    selec = 0;
    turno = 1;
    cturno = cturno + 1;
  }
  if (dr == 1 && selec == 9 && vector[5] && cr == 0) {
    dr = 0;
    cr = 1;
    selec = 0;
    turno = 1;
    cturno = cturno + 1;
  }

  if (cl == 1 && selec == 4 && vector[0] && ul == 0) {
    cl = 0;
    ul = 1;
    selec = 0;
    turno = 1;
    cturno = cturno + 1;
  }
  if (cl == 1 && selec == 4 && vector[1] && uc == 2) {
    cl = 0;
    uc = 1;
    selec = 0;
    turno = 1;
    cturno = cturno + 1;
  }
  if (cc == 1 && selec == 5 && vector[0] && ul == 2) {
    cc = 0;
    ul = 1;
    selec = 0;
    turno = 1;
    cturno = cturno + 1;
  }
  if (cc == 1 && selec == 5 && vector[1] && uc == 0) {
    cc = 0;
    uc = 1;
    selec = 0;
    turno = 1;
    cturno = cturno + 1;
  }
  if (cc == 1 && selec == 5 && vector[2] && ur == 2) {
    cc = 0;
    ur = 1;
    selec = 0;
    turno = 1;
    cturno = cturno + 1;
  }
  if (cr == 1 && selec == 6 && vector[1] && uc == 2) {
    cr = 0;
    uc = 1;
    selec = 0;
    turno = 1;
    cturno = cturno + 1;
  }
  if (cr == 1 && selec == 6 && vector[2] && ur == 0) {
    cr = 0;
    ur = 1;
    selec = 0;
    turno = 1;
    cturno = cturno + 1;
  }
}

void MostrarLeds() {
  if (ul == 0) {
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.show();
  }
  if (ul == 1) {
    pixels.setPixelColor(0, pixels.Color(0, 5, 0));
    pixels.show();
  }
  if (ul == 2) {
    pixels.setPixelColor(0, pixels.Color(5, 0, 0));
    pixels.show();
  }
  if (uc == 0) {
    pixels.setPixelColor(1, pixels.Color(0, 0, 0));
    pixels.show();
  }
  if (uc == 1) {
    pixels.setPixelColor(1, pixels.Color(0, 5, 0));
    pixels.show();
  }
  if (uc == 2) {
    pixels.setPixelColor(1, pixels.Color(5, 0, 0));
    pixels.show();
  }
  if (ur == 0) {
    pixels.setPixelColor(2, pixels.Color(0, 0, 0));
    pixels.show();
  }
  if (ur == 1) {
    pixels.setPixelColor(2, pixels.Color(0, 5, 0));
    pixels.show();
  }
  if (ur == 2) {
    pixels.setPixelColor(2, pixels.Color(5, 0, 0));
    pixels.show();
  }
  if (cl == 0) {
    pixels.setPixelColor(5, pixels.Color(0, 0, 0));
    pixels.show();
  }
  if (cl == 1) {
    pixels.setPixelColor(5, pixels.Color(0, 5, 0));
    pixels.show();
  }
  if (cl == 2) {
    pixels.setPixelColor(5, pixels.Color(5, 0, 0));
    pixels.show();
  }
  if (cc == 0) {
    pixels.setPixelColor(4, pixels.Color(0, 0, 0));
    pixels.show();
  }
  if (cc == 1) {
    pixels.setPixelColor(4, pixels.Color(0, 5, 0));
    pixels.show();
  }
  if (cc == 2) {
    pixels.setPixelColor(4, pixels.Color(5, 0, 0));
    pixels.show();
  }
  if (cr == 0) {
    pixels.setPixelColor(3, pixels.Color(0, 0, 0));
    pixels.show();
  }
  if (cr == 1) {
    pixels.setPixelColor(3, pixels.Color(0, 5, 0));
    pixels.show();
  }
  if (cr == 2) {
    pixels.setPixelColor(3, pixels.Color(5, 0, 0));
    pixels.show();
  }
  if (dl == 0) {
    pixels.setPixelColor(6, pixels.Color(0, 0, 0));
    pixels.show();
  }
  if (dl == 1) {
    pixels.setPixelColor(6, pixels.Color(0, 5, 0));
    pixels.show();
  }
  if (dl == 2) {
    pixels.setPixelColor(6, pixels.Color(5, 0, 0));
    pixels.show();
  }
  if (dc == 0) {
    pixels.setPixelColor(7, pixels.Color(0, 0, 0));
    pixels.show();
  }
  if (dc == 1) {
    pixels.setPixelColor(7, pixels.Color(0, 5, 0));
    pixels.show();
  }
  if (dc == 2) {
    pixels.setPixelColor(7, pixels.Color(5, 0, 0));
    pixels.show();
  }
  if (dr == 0) {
    pixels.setPixelColor(8, pixels.Color(0, 0, 0));
    pixels.show();
  }
  if (dr == 1) {
    pixels.setPixelColor(8, pixels.Color(0, 5, 0));
    pixels.show();
  }
  if (dr == 2) {
    pixels.setPixelColor(8, pixels.Color(5, 0, 0));
    pixels.show();
  }
}

void ChequearVictoriaMio() {
  if (ul == 1 || uc == 1 || ur == 1) {
    //111
    //000
    //000
    victoria = 1;
  }
  if (ul == 2 && !uc && ur == 2 && cl == 1 && cc == 2 && cr == 1 && !dl && dc == 1 && !dr && turno) {
    //202
    //121
    //010
    victoria = 1;
  }
  if (!ul && uc == 2 && !ur && !cl && cc == 1 && !cr && !dl && !dc && !dr && turno){
    //020
    //010
    //000
    victoria = 1;
  }
}

void ChequearVictoriaTuyo() {
  if (dr == 2 || dc == 2 || dl == 2) {
    //000
    //000
    //222
    victoria = 2;
  }
  if (ul == 2 && !uc && !ur && cl == 1 && cc == 2 && !cr && !dl && dc == 1 && !dr && !turno) {
    //200
    //120 + MITURNO
    //010
    victoria = 2;
  }
  if (!ul && !uc && ur == 2 && !cl && cc == 2 && cr == 1 && !dl && dc == 1 && !dr && !turno) {
    //002
    //021 + MITURNO
    //010
    victoria = 2;
  }
  if (!ul && uc == 2 && !ur && !cl && cc == 1 && cr == 2 && !dl && !dc && dr == 1 && !turno) {
    //020
    //012 + MITURNO
    //001
    victoria = 2;
  }
  if (!ul && uc == 2 && !ur && cl == 2 && cc == 1 && !cr && dl == 1 && !dc && !dr && !turno) {
    //020
    //210 + MITURNO
    //100
    victoria = 2;
  }
  if (ul == 2 && !uc && !ur && cl == 1 && !cc && cr == 2 && !dl && !dc && dr == 1 && !turno) {
    //200
    //102 + MITURNO
    //001
    victoria = 2;
  }
  if (!ul && !uc && ur == 2 && cl == 2 && !cc && cr == 1 && dl == 1 && !dc && !dr && !turno) {
    //002
    //201 + MITURNO
    //100
    victoria = 2;
  }
  if (!ul && !uc && !ur && cl == 2 && cc == 2 && !cr && !dl && !dc && !dr) {
    //000
    //220
    //000
    victoria = 2;
  }
  if (!ul && !uc && !ur && !cl && cc == 2 && cr == 2 && !dl && !dc && !dr) {
    //000
    //022
    //000
    victoria = 2;
  }
}

void Espacios() {
  espacios = dr + (dc * 10) + (dl * 100) + (cr * 1000) + (cc * 10000) + (cl * 100000) + (ur * 1000000) + (uc * 10000000) + (ul * 100000000);
}

void CondicionalesT2() {
  if (ul == 2 && uc == 2 && ur == 2 && cl == 0 && cc == 1 && cr == 0 && dl == 1 && dc == 0 && dr == 1) {
    // 222
    // 010
    // 101
    a = random(0, 2);
    if (a == 0) {
      ul = 0;
      cl = 2;
      turno = 0;
    }
    if (a == 1) {
      ul = 0;
      cc = 2;
      turno = 0;
    }
  }

  if (ul == 2 && uc == 2 && ur == 2 && cl == 1 && cc == 0 && cr == 0 && dl == 0 && dc == 1 && dr == 1) {
    // 222
    // 100
    // 011
    a = random(0, 3);
    if (a == 0) {
      uc = 0;
      cc = 2;
      turno = 0;
    }
    if (a == 1) {
      uc = 0;
      cl = 2;
      turno = 0;
    }
    if (a == 2) {
      ur = 0;
      cr = 2;
      turno = 0;
    }
  }

  if (ul == 2 && uc == 2 && ur == 2 && cl == 0 && cc == 0 && cr == 1 && dl == 1 && dc == 1 && dr == 0) {
    // 222
    // 001
    // 110
    a = random(0, 3);
    if (a == 0) {
      uc = 0;
      cc = 2;
      turno = 0;
    }
    if (a == 1) {
      uc = 0;
      cr = 2;
      turno = 0;
    }
    if (a == 2) {
      ul = 0;
      cl = 2;
      turno = 0;
    }
  }
}

void CondicionalesT4() {
  if (ul == 2 && uc == 0 && ur == 2 && cl == 2 && cc == 1 && cr == 0 && dl == 0 && dc == 0 && dr == 1) {
    // 202
    // 210
    // 001
    a = random(0, 4);
    if (a == 0) {
      cl = 0;
      dl = 2;
      turno = 0;
    }
    if (a == 1) {
      ur = 0;
      cl = 2;
      turno = 0;
    }
    if (a == 2) {
      ul = 0;
      cc = 2;
      turno = 0;
    }
    if (a == 3) {
      ur = 0;
      cc = 2;
      turno = 0;
    }
  }

  if (ul == 0 && uc == 2 && ur == 2 && cl == 1 && cc == 2 && cr == 0 && dl == 0 && dc == 0 && dr == 1) {
    // 022
    // 120
    // 001
    a = random(0, 4);
    if (a == 0) {
      uc = 0;
      cl = 2;
      turno = 0;
    }
    if (a == 1) {
      ur = 0;
      cr = 2;
      turno = 0;
    }
    if (a == 2) {
      cc = 0;
      dc = 2;
      turno = 0;
    }
    if (a == 3) {
      cc = 0;
      dr = 2;
      turno = 0;
    }
  }

  if (ul == 2 && uc == 0 && ur == 2 && cl == 1 && cc == 1 && cr == 0 && dl == 0 && dc == 1 && dr == 0) {
    // 202
    // 110
    // 010
    a = random(0, 3);
    if (a == 0) {
      ul = 0;
      cc = 2;
      turno = 0;
    }
    if (a == 1) {
      ur = 0;
      cc = 2;
      turno = 0;
    }
    if (a == 2) {
      ur = 0;
      cr = 2;
      turno = 0;
    }
  }

  if (ul == 2 && uc == 2 && ur == 0 && cl == 1 && cc == 0 && cr == 1 && dl == 0 && dc == 0 && dr == 1) {
    // 220
    // 101
    // 001
    a = random(0, 3);
    if (a == 0) {
      uc = 0;
      cl = 2;
      turno = 0;
    }
    if (a == 1) {
      uc = 0;
      cr = 2;
      turno = 0;
    }
    if (a == 2) {
      uc = 0;
      cc = 2;
      turno = 0;
    }
  }

  if (ul == 0 && uc == 2 && ur == 2 && cl == 0 && cc == 2 && cr == 1 && dl == 1 && dc == 0 && dr == 0) {
    // 022
    // 021
    // 100
    a = random(0, 3);
    if (a == 0) {
      uc = 0;
      cr = 2;
      turno = 0;
    }
    if (a == 1) {
      cc = 0;
      dc = 2;
      turno = 0;
    }
    if (a == 2) {
      cc = 0;
      dl = 2;
      turno = 0;
    }
  }

  if (ul == 0 && uc == 2 && ur == 2 && cl == 2 && cc == 1 && cr == 1 && dl == 1 && dc == 0 && dr == 0) {
    // 022
    // 211
    // 100
    a = random(0, 2);
    if (a == 0) {
      uc = 0;
      cr = 2;
      turno = 0;
    }
    if (a == 1) {
      ur = 0;
      cc = 2;
      turno = 0;
    }
  }

  if (ul == 2 && uc == 0 && ur == 2 && cl == 2 && cc == 0 && cr == 1 && dl == 0 && dc == 1 && dr == 0) {
    // 202
    // 201
    // 010
    a = random(0, 2);
    if (a == 0) {
      cl = 0;
      dl = 2;
      turno = 0;
    }
    if (a == 1) {
      cl = 0;
      dc = 2;
      turno = 0;
    }
  }

  if (ul == 2 && uc == 2 && ur == 0 && cl == 1 && cc == 1 && cr == 2 && dl == 0 && dc == 0 && dr == 1) {
    // 220
    // 112
    // 001
    a = random(0, 2);
    if (a == 0) {
      ul = 0;
      cc = 2;
      turno = 0;
    }
    if (a == 1) {
      uc = 0;
      cl = 2;
      turno = 0;
    }
  }

  if (ul == 0 && uc == 2 && ur == 2 && cl == 0 && cc == 1 && cr == 0 && dl == 0 && dc == 0 && dr == 1) {
    // 022
    // 010
    // 001
    a = random(0, 2);
    if (a == 0) {
      ur = 0;
      cc = 2;
      turno = 0;
    }
    if (a == 1) {
      ur = 0;
      cr = 2;
      turno = 0;
    }
  }

  if (ul == 0 && uc == 2 && ur == 2 && cl == 0 && cc == 1 && cr == 0 && dl == 1 && dc == 0 && dr == 0) {
    // 022
    // 010
    // 100
    a = random(0, 2);
    if (a == 0) {
      ur = 0;
      cc = 2;
      turno = 0;
    }
    if (a == 1) {
      ur = 0;
      cr = 2;
      turno = 0;
    }
  }

  if (ul == 2 && uc == 0 && ur == 2 && cl == 1 && cc == 0 && cr == 0 && dl == 0 && dc == 0 && dr == 1) {
    // 202
    // 100
    // 001
    ur = 0;
    cr = 2;
    turno = 0;
  }

  if (ul == 2 && uc == 0 && ur == 2 && cl == 0 && cc == 1 && cr == 2 && dl == 1 && dc == 0 && dr == 0) {
    // 202
    // 012
    // 100
    a = random(0, 4);
    if (a == 0) {
      cr = 0;
      dr = 2;
      turno = 0;
    }
    if (a == 1) {
      ul = 0;
      cl = 2;
      turno = 0;
    }
    if (a == 2) {
      ur = 0;
      cc = 2;
      turno = 0;
    }
    if (a == 3) {
      ul = 0;
      cc = 2;
      turno = 0;
    }
  }

  if (ul == 2 && uc == 0 && ur == 2 && cl == 0 && cc == 1 && cr == 1 && dl == 0 && dc == 1 && dr == 0) {
    // 202
    // 011
    // 010
    a = random(0, 3);
    if (a == 0) {
      ur = 0;
      cc = 2;
      turno = 0;
    }
    if (a == 1) {
      ul = 0;
      cc = 2;
      turno = 0;
    }
    if (a == 2) {
      ur = 0;
      cr = 2;
      turno = 0;
    }
  }

  if (ul == 0 && uc == 2 && ur == 2 && cl == 1 && cc == 0 && cr == 1 && dl == 1 && dc == 0 && dr == 0) {
    // 022
    // 101
    // 100
    a = random(0, 3);
    if (a == 0) {
      uc = 0;
      cr = 2;
      turno = 0;
    }
    if (a == 1) {
      uc = 0;
      cl = 2;
      turno = 0;
    }
    if (a == 2) {
      uc = 0;
      cc = 2;
      turno = 0;
    }
  }

  if (ul == 2 && uc == 0 && ur == 2 && cl == 1 && cc == 0 && cr == 2 && dl == 0 && dc == 1 && dr == 0) {
    // 202
    // 102
    // 010
    a = random(0, 2);
    if (a == 0) {
      cr = 0;
      dr = 2;
      turno = 0;
    }
    if (a == 1) {
      cr = 0;
      dc = 2;
      turno = 0;
    }
  }

  if (ul == 0 && uc == 2 && ur == 2 && cl == 2 && cc == 1 && cr == 1 && dl == 1 && dc == 0 && dr == 0) {
    // 022
    // 211
    // 100
    a = random(0, 2);
    if (a == 0) {
      ur = 0;
      cc = 2;
      turno = 0;
    }
    if (a == 1) {
      uc = 0;
      cr = 2;
      turno = 0;
    }
  }

  if (ul == 2 && uc == 0 && ur == 2 && cl == 0 && cc == 0 && cr == 1 && dl == 1 && dc == 0 && dr == 0) {
    // 202
    // 001
    // 100
    ul = 0;
    cl = 2;
    turno = 0;
  }
}

void CondicionalesT6() {
  if (ul == 0 && uc == 0 && ur == 2 && cl == 2 && cc == 2 && cr == 1 && dl == 0 && dc == 0 && dr == 0) {
    // 002
    // 221
    // 000
    a = random(0, 2);
    if (a == 0) {
      cl = 0;
      dl = 2;
      turno = 0;
    }
    if (a == 1) {
      cc = 0;
      dc = 2;
      turno = 0;
    }
  }

  if (ul == 2 && uc == 0 && ur == 0 && cl == 1 && cc == 2 && cr == 2 && dl == 0 && dc == 0 && dr == 0) {
    // 200
    // 122
    // 000
    a = random(0, 2);
    if (a == 0) {
      cr = 0;
      dr = 2;
      turno = 0;
    }
    if (a == 1) {
      cc = 0;
      dc = 2;
      turno = 0;
    }
  }

  if (ul == 2 && uc == 0 && ur == 0 && cl == 1 && cc == 1 && cr == 1 && dl == 0 && dc == 0 && dr == 0) {
    // 200
    // 111
    // 000
    ul = 0;
    cc = 2;
    turno = 0;
  }

  if (ul == 0 && uc == 0 && ur == 2 && cl == 1 && cc == 1 && cr == 1 && dl == 0 && dc == 0 && dr == 0) {
    // 002
    // 111
    // 000
    ur = 0;
    cc = 2;
    turno = 0;
  }

  if (ul == 0 && uc == 2 && ur == 0 && cl == 2 && cc == 1 && cr == 1 && dl == 0 && dc == 0 && dr == 0) {
    // 020
    // 211
    // 000
    a = random(0, 2);
    if (a == 0) {
      cl = 0;
      dl = 2;
      turno = 0;
    }
    if (a == 1) {
      uc = 0;
      cr = 2;
      turno = 0;
    }
  }

  if (ul == 0 && uc == 2 && ur == 0 && cl == 1 && cc == 1 && cr == 2 && dl == 0 && dc == 0 && dr == 0) {
    // 020
    // 112
    // 000
    a = random(0, 2);
    if (a == 0) {
      cr = 0;
      dr = 2;
      turno = 0;
    }
    if (a == 1) {
      uc = 0;
      cl = 2;
      turno = 0;
    }
  }

  if (ul == 2 && uc == 0 && ur == 0 && cl == 2 && cc == 2 && cr == 1 && dl == 0 && dc == 0 && dr == 0) {
    // 200
    // 221
    // 000
    a = random(0, 2);
    if (a == 0) {
      cl = 0;
      dl = 2;
      turno = 0;
    }
    if (a == 1) {
      cc = 0;
      dc = 2;
      turno = 0;
    }
  }

  if (ul == 0 && uc == 0 && ur == 2 && cl == 1 && cc == 2 && cr == 2 && dl == 0 && dc == 0 && dr == 0) {
    // 002
    // 122
    // 000
    a = random(0, 2);
    if (a == 0) {
      cr = 0;
      dr = 2;
      turno = 0;
    }
    if (a == 1) {
      cc = 0;
      dc = 2;
      turno = 0;
    }
  }

  if (ul == 0 && uc == 0 && ur == 2 && cl == 2 && cc == 1 && cr == 0 && dl == 0 && dc == 0 && dr == 0) {
    // 002
    // 210
    // 000
    a = random(0, 3);
    if (a == 0) {
      cl = 0;
      dl = 2;
      turno = 0;
    }
    if (a == 1) {
      ur = 0;
      cr = 2;
      turno = 0;
    }
    if (a == 2) {
      ur = 0;
      cc = 2;
      turno = 0;
    }
  }

  if (ul == 0 && uc == 0 && ur == 2 && cl == 2 && cc == 1 && cr == 0 && dl == 0 && dc == 0 && dr == 0) {
    // 200
    // 012
    // 000
    a = random(0, 3);
    if (a == 0) {
      cr = 0;
      dr = 2;
      turno = 0;
    }
    if (a == 1) {
      ul = 0;
      cl = 2;
      turno = 0;
    }
    if (a == 2) {
      ul = 0;
      cc = 2;
      turno = 0;
    }
  }

  if (ul == 0 && uc == 2 && ur == 0 && cl == 1 && cc == 2 && cr == 0 && dl == 0 && dc == 0 && dr == 0) {
    // 020
    // 120
    // 000
    a = random(0, 2);
    if (a == 0) {
      cc = 0;
      dc = 2;
      turno = 0;
    }
    if (a == 1) {
      uc = 0;
      cl = 2;
      turno = 0;
    }
  }

  if (ul == 0 && uc == 2 && ur == 0 && cl == 0 && cc == 2 && cr == 1 && dl == 0 && dc == 0 && dr == 0) {
    // 020
    // 021
    // 000
    a = random(0, 2);
    if (a == 0) {
      cc = 0;
      dc = 2;
      turno = 0;
    }
    if (a == 1) {
      uc = 0;
      cr = 2;
      turno = 0;
    }
  }

  if (ul == 2 && uc == 0 && ur == 0 && cl == 2 && cc == 1 && cr == 0 && dl == 0 && dc == 0 && dr == 0) {
    // 200
    // 210
    // 000
    a = random(0, 2);
    if (a == 0) {
      cl = 0;
      dl = 2;
      turno = 0;
    }
    if (a == 1) {
      ul = 0;
      cc = 2;
      turno = 0;
    }
  }

  if (ul == 0 && uc == 0 && ur == 2 && cl == 0 && cc == 1 && cr == 2 && dl == 0 && dc == 0 && dr == 0) {
    // 002
    // 012
    // 000
    a = random(0, 2);
    if (a == 0) {
      cr = 0;
      dr = 2;
      turno = 0;
    }
    if (a == 1) {
      ur = 0;
      cc = 2;
      turno = 0;
    }
  }
  if (!ul && !uc && ur == 2 && cl == 2 && cc == 1 && cr == 1 && !dl && !dc && !dr){
    //002
    //211
    //000
    a = random(0, 2);
    if (a == 0){
      ur = 0;
      cc = 2;
      turno = 0;
    }
    if (a == 1){
      cl = 0;
      dl = 2;
      turno = 0;
    }
  }
  if (ul == 2 && !uc && !ur && cl == 1 && cc == 1 && cr == 2 && !dl && !dc && !dr){
    //002
    //211
    //000
    a = random(0, 2);
    if (a == 0){
      ul = 0;
      cc = 2;
      turno = 0;
    }
    if (a == 1){
      cr = 0;
      dr = 2;
      turno = 0;
    }
  }
}

void loop() {
  MostrarMatriz();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  leerMatriz();
  MostrarLeds();
  if (!victoria) {
    Hexapawn();
    StaticJsonDocument<200> doc;
    doc["Cords"] = espacios;
    char buffer[256];
    serializeJson(doc, buffer);
    client.publish("Mensaje", buffer);
  }
  MostrarLeds();
  if (victoria == 1) {
    delay(1000);
    for (int i = 0; i < 9; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 10, 5));
      pixels.show();
      delay(250);
    }
    for (int i = 0; i < 9; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      pixels.show();
      delay(250);
    }
    Inicial();
  }

  if (victoria == 2) {
    delay(1000);
    for (int i = 0; i < 9; i++) {
      pixels.setPixelColor(i, pixels.Color(10, 5, 0));
      pixels.show();
      delay(250);
    }
    for (int i = 0; i < 9; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      pixels.show();
      delay(250);
    }
    Inicial();
  }
  delay(100);
}