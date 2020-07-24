#include <M5Stack.h>
#include <NeoPixelBus.h>
#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <M5Bala.h>

// for wifi
WiFiClient wifiClient;
const char* SSID = "955904991850_Extend";
const char* PASSWORD = "58477850";

// network
const char* BROKER_IP = "192.168.100.93";

// i2c cable
TwoWire *wire;

// command
int command = 0;

// ==================== MQTT =====================
const char *THING_NAME = "M5Stack";
PubSubClient mqttClient;

void callback(char* topic, byte* payload, unsigned int length) {
  String buf_t = String(topic);
  M5.Lcd.println(buf_t);

  payload[length] = '\0';
  String buf_s = String((char*) payload);
  M5.Lcd.println(buf_s);

  if (buf_s == "w")
  {
    command = 1;
  }else if (buf_s == "s"){
    command = 2;
  }else if (buf_s == "a"){
    command = 3;
  }else if (buf_s == "d"){
    command = 4;
  }else{
    command = 0;
  }
  M5.Lcd.printf("cmd:%d\n", command);
}

void connectToBroker() {
  M5.Lcd.println("trying to connect MQTT broker");
  mqttClient.setServer(BROKER_IP, 1883);
  mqttClient.setCallback(callback);
  mqttClient.setClient(wifiClient);
  if (!mqttClient.connect(THING_NAME)) {
    M5.Lcd.println(mqttClient.state());
  }
  mqttClient.subscribe("m5stack/control/#");
}

/*=============================== */
/* servo control */
/*=============================== */
void setMotor(int16_t pwm0, int16_t pwm1){
	// Value range
	int16_t m0 = constrain(pwm0, -255, 255);
	int16_t m1 = constrain(pwm1, -255, 255);

	// Dead zone
	if (((m0 > 0) && (m0 < DEAD_ZONE)) || ((m0 < 0) && (m0 > -DEAD_ZONE))) m0 = 0;
	if (((m1 > 0) && (m1 < DEAD_ZONE)) || ((m1 < 0) && (m1 > -DEAD_ZONE))) m1 = 0;

	// Same value
	static int16_t pre_m0, pre_m1;
	if ((m0 == pre_m0) && (m1 == pre_m1))
		return;
	pre_m0 = m0;
	pre_m1 = m1;

	// Send I2C
	wire->beginTransmission(M5GO_WHEEL_ADDR);
	wire->write(MOTOR_CTRL_ADDR); // Motor ctrl reg addr
	wire->write(((uint8_t*)&m0)[0]);
	wire->write(((uint8_t*)&m0)[1]);
	wire->write(((uint8_t*)&m1)[0]);
	wire->write(((uint8_t*)&m1)[1]);
	wire->endTransmission();
}

/*=============================== */
/* setup */
/*=============================== */
void setup(void) {

	// Power ON Stabilizing...
  delay(500);
  M5.begin();

	// Init I2C
	Wire.begin();
	Wire.setClock(400000UL);  // Set I2C frequency to 400kHz
	delay(500);

  // Serial.begin(115200);

  // wifi connetction
  WiFi.begin(SSID, PASSWORD);
  M5.Lcd.println("Wait for WiFi... ");

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Lcd.println(".");
  }

  M5.Lcd.println("WiFi connected");
  M5.Lcd.println("IP address: ");
  M5.Lcd.println(WiFi.localIP());

  connectToBroker();
  M5.Lcd.println("success to connect MQTT broker");

  wire = &Wire;
  setMotor(0, 0);
}


/*=============================== */
/* main loop */
/*=============================== */
void loop() {
  mqttClient.loop();

  if (command == 1) {
    setMotor(128, 128);
  } else if (command == 2) {
    setMotor(-128, -128);
  } else if (command == 3) {
    setMotor(128, -128);
  } else if (command == 4) {
    setMotor(-128, 128);
  }else{
    setMotor(0, 0);
  }
  Serial.printf("cmd: %d\n", command);

	// M5 Loop
	M5.update();
}
