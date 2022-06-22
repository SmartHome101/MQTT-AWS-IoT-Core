#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#define Sensor D0


const char* ssid = "SABEK 9090";
const char* password = "77777777";
const char* awsEndpoint = "a3gzgiva70xnia-ats.iot.us-east-2.amazonaws.com";



// certificate.pem.crt
static const char certificatePemCrt[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUPo+cHVN8qOgchtZYw+vLfpysaMEwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIyMDYyMDE0MDkw
OFoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAO+FWw2bUKcad/1PqeKQ
hV8tPTvC/k6gQxA9CSRrlv9kNGxYPtVsZ9g5pCwJMvFOgl0NbTl61jbRoygGEQdI
Mu8NX3NxL44E84Jgl1mEHtEUkjEEO7P8PkBoRU6NLhyKh8d4idEu7zSd5eFaVp9l
BEVVEGusiFo5UxX2u35lzLohhxo26h/6jqropDlZ2y8xw8xdA805tMDoHxF47NDH
7peBOIC6SP8G1OMTwbo2wSy/7ciPBQmQ3Koch7bmtEl/GDnp2tCQHEOcEjwCsW+/
pBmqM+khtzHpKm2AIgEHeWYdXZOLlYuZ8DjzJxotYa+2JUa+0Mnmlvl3FrxhCMH7
Dh0CAwEAAaNgMF4wHwYDVR0jBBgwFoAUpBmJfiShcDkce6iqp3BmSbrfYpgwHQYD
VR0OBBYEFHZ7jersjAT+czDMPhYJD9Av5UNQMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQAJsOg7Oxc0iNHqyFdf9NsObLVy
9ZCIlV2NtWqFXzC8nkf5nw2oVZQHy3E7WgS59l6NQcr475DE1XyZkPxDs5NwAWI2
hAEMErHEq6sJJdLfddwcQhXv94SgNiHhnWqgHvst+GyWAJfEzrCdkYiaBJALU/0N
3AocQAzuvYtnOqLAusplj4RuNJ+259f50Pqvimk1izviWpAWl6BjLaSlG0g2A4xn
e/QCaY8hzZIxPylZ60TXXaRKz1QKwSuTwzjbWWJi0YznIuc0758Tl+LKIvP5AmRc
dZZGSG0m4SsZIhNFK7GvZh9nWlmFUQaaGQ+6LXQz3Mo8GMFLaSmo8oAJmy9c
-----END CERTIFICATE-----
)EOF";

// private.pem.key
static const char privatePemKey[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEA74VbDZtQpxp3/U+p4pCFXy09O8L+TqBDED0JJGuW/2Q0bFg+
1Wxn2DmkLAky8U6CXQ1tOXrWNtGjKAYRB0gy7w1fc3EvjgTzgmCXWYQe0RSSMQQ7
s/w+QGhFTo0uHIqHx3iJ0S7vNJ3l4VpWn2UERVUQa6yIWjlTFfa7fmXMuiGHGjbq
H/qOquikOVnbLzHDzF0DzTm0wOgfEXjs0Mful4E4gLpI/wbU4xPBujbBLL/tyI8F
CZDcqhyHtua0SX8YOena0JAcQ5wSPAKxb7+kGaoz6SG3MekqbYAiAQd5Zh1dk4uV
i5nwOPMnGi1hr7YlRr7QyeaW+XcWvGEIwfsOHQIDAQABAoIBAQCJZIyTZIlCIXu8
sWq4zL/ElbWvRy4LOgDeGrTY0GKAQyGTGJeKYzG6JW5X2DsBklBTQjVqKF0ieuRF
PB2eeLrGx3v+Q2nM5GguXGvU7GWk91DYjx4UI1guJ+HBqlhOBOTX9fvQI0URgJVx
zBkuO4rUnnPZ39EjOAjB65MncQubpC2x5Rcx0hg7jk3wvXPfRi2qujHqIWLMsGiA
FjRhnzldRxyLXseAX9jBldP6pkcBr73SuU5sxTKjWd7H35kAn3+qdE0MSWvwbv0w
yMjvL8FCPwIzVrNQXkgodON+P0ddz5drmNij4Qa1MWFFV8BX7cJmcv2ylzEdF02B
KHSql3wBAoGBAPxEyqlot5Q1Ls+KVT4L2bUAszEganqS9fF8AAhNXCwOmAZkd7pj
F9PmfWW0kQqUHkDjLS+b1ZfFjoSZqKoPdEsdiHzEQJATQhlqidTXYwRS4Fry4TJr
Zdn7LuhSvA5Jxa6UhEAmbwZH4CeqF7nqi1VD4TSam5hvENib5oasFSEdAoGBAPMQ
S3vI1DJPKk+zts7UtE2vO8XGbQKjbwb50rlaqscWTlC7DhTwEOiqf5Azv23orOa2
9YJxtEXO8Z/su9S9wOZHthBg6YdeWF4N0FCWGKVYzwiUkQF7gYiG4C0w1vCU7iva
Kl5G6f/Afh6IfZ2VvkTtRqggCHHA2raMQBhWDxEBAoGBAKQY/5e5pYlEGyIoAxEs
Vj7WqLmxM3JufIvMNJ0x7V4/VAb8faGB6jMOAM4S6jJubUuaO393qDZk7TM2XS/a
g7SFDo7Sfoj57gpociZAwYlDe+vNiE3q7MMiEL5Ra0EEHhJVS74LAjecs6Q5kdt/
RYl+cVVtqP88aEHvD8LU153FAoGAIUs9G7KRpmaDpDE4vDIUlIOJS3j3m6HpZmo3
/B24QNcEWTiWIDYWCti36ne8JAD4Fg4/qOOMTveez0otHGH1l1WBUdMEif9ZWWDJ
BRDaRWPZ0BIRnsNdJedbs7T57DBqQ2V54joSeW7Tl7kYg8psKqLZx9PDdFl3XAPw
D6VSIQECgYA+s5y/blX0i629XgH39fD26pqHyEPiKkUDqPAzAapsljyOjzELRhIQ
0YHX1Bvbjf5eL+oHJmpHBkQ/Gugc2/3s/dRrxfyrwNjjy60P9QNcL3QmSF+Smom/
zLraRIsioEFHPcENYqPAESNFsEvmct3F0pjG98VgPtxLeMLziz5DJA==
-----END RSA PRIVATE KEY-----
)EOF";


// This is the AWS IoT CA Certificate
static const char caPemCrt[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

BearSSL::X509List client_crt(certificatePemCrt);
BearSSL::PrivateKey client_key(privatePemKey);
BearSSL::X509List rootCert(caPemCrt);

WiFiClientSecure wiFiClient;

void msgReceived(char* topic, byte* payload, unsigned int len);
PubSubClient pubSubClient(awsEndpoint, 8883, msgReceived, wiFiClient);

void setup()
{
  Serial.begin(115200);
  Serial.println();

  //setup used pins
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(D0,INPUT);
  pinMode(D1,OUTPUT);
  pinMode(D2,OUTPUT);
  pinMode(D3,OUTPUT);
  pinMode(D4,OUTPUT);
  pinMode(D5,OUTPUT);
  pinMode(D6,OUTPUT);
  pinMode(D7,OUTPUT);
  pinMode(D8,OUTPUT);
  pinMode(D9,OUTPUT);
  pinMode(D10,OUTPUT);
  

  //WiFi setup
  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  WiFi.waitForConnectResult();
  Serial.print(", WiFi connected, IP address: "); Serial.println(WiFi.localIP());

  // get current time, otherwise certificates are flagged as expired
  setCurrentTime();

  wiFiClient.setClientRSACert(&client_crt, &client_key);
  wiFiClient.setTrustAnchors(&rootCert);
}

unsigned long lastPublish;
int msgCount;

void loop()
{
  pubSubCheckConnect();

// publish data each 10 sec
  if (millis() - lastPublish > 10000)
  {
    char Data[128];
    // allocate the memory for the document
    float var1 =  random(55,77); //fake number range, adjust as you like
    float var2 =  random(77,99);
    sprintf(Data,  "{\"uptime\":%lu,\"temp\":%f,\"humid\":%f}", millis() / 1000, var1, var2);
    pubSubClient.publish("outTopic",Data);
    Serial.print("Published: "); 
    Serial.println(Data);
    lastPublish = millis();
  }
}


// how to handle incoming messages
void msgReceived(char* topic, byte* payload, unsigned int length)
{
  //printing topic name
  Serial.print("Message received on ");
  Serial.println(topic);
  StaticJsonDocument<256> doc;
  deserializeJson(doc, payload, length);

  static const uint8_t NAME2PIN[] = {D0, D1, D2, D3, D4, D5, D6, D7, D8, D9, D10};

  String strPin;
  //topics are in this pattern home/node1/D#
  for (int i =12; i <13; i++) 
    strPin += topic[12];
  uint8_t pin  = NAME2PIN[strPin.toInt()]; 
  Serial.println(pin);
  if (doc["message"] == "ON")
      digitalWrite(pin, HIGH);
  else
      digitalWrite(pin, LOW);
}


void pubSubCheckConnect()
{
  //Connecting to AWS en point
  if (!pubSubClient.connected())
  {
    Serial.print("PubSubClient connecting to: ");
    Serial.print(awsEndpoint);

    while ( ! pubSubClient.connected())
    {
      Serial.print(".");
      pubSubClient.connect("ESP8266_1");
    }

    Serial.println(" connected");

    //the ESP is subscribing to all topics which begin with home/node1/anyThingElse
    pubSubClient.subscribe("home/node1/#");
  }

  pubSubClient.loop();
}

//The main purpose of this function is guarantee that the certificates are not expired.
void setCurrentTime()
{
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: "); Serial.print(asctime(&timeinfo));
}
