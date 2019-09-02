
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "HTU21D.h"






const char* ssid = "iPhone (Dmitry)";  // Имя вайфай точки доступа
const char* pass = "1234509876"; // Пароль от точки доступа

const char* mqtt_server = "postman.cloudmqtt.com"; // Имя сервера MQTT
const int mqtt_port = 18610; // Порт для подключения к серверу MQTT
const char* mqtt_user = "imhlnxps"; // Логин от сервер
const char* mqtt_pass = "iTH5jj5edeF1"; // Пароль от сервера

#define BUFFER_SIZE 100
HTU21D myHumidity;




// Функция получения данных от сервера

void callback(const MQTT::Publish& pub)
{
	Serial.print(pub.topic());   // выводим в сериал порт название топика
	Serial.print(" => ");
	Serial.println(pub.payload_string()); // выводим в сериал порт значение полученных данных

	String payload = pub.payload_string();



	if (String(pub.topic()) == "gs/uf") // проверяем из какого топика пришли данные
	{
		int uf = payload.toInt(); // преобразуем полученные данные в тип integer
		Sendtemphg();
		Sendgm();
		if (uf == 1) {
			digitalWrite(15, HIGH);
		}
		else {
			digitalWrite(15, LOW);
		}
	}



	if (String(pub.topic()) == "gs/ir")
	{
		int ir = payload.toInt();
		Sendtemphg();
		Sendgm();
		if (ir == 1) {
			digitalWrite(10, HIGH);
		}
		else {
			digitalWrite(10, LOW);
		}
	}



	if (String(pub.topic()) == "gs/pomp")
	{
		int pomp = payload.toInt();
		Sendtemphg();
		Sendgm();
		if (pomp == 1) {
			digitalWrite(12, HIGH);
			delay(2000);
			digitalWrite(12, LOW);
			Sendpomp0();
		}
		else {
			digitalWrite(12, LOW);
		}
	}



	if (String(pub.topic()) == "gs/fan")
	{
		int fan = payload.toInt();
		Sendtemphg();
		Sendgm();
		if (fan == 1) {
			digitalWrite(14, HIGH);
		}
		else {
			digitalWrite(14, LOW);
		}
	}

	if (String(pub.topic()) == "gs/update")
	{
		int date = payload.toInt();
		Rest();
	}



	if (String(pub.topic()) == "gs/uphg")
	{
		int upgh = payload.toInt();
		Sendtemphg();
		Sendgm();
		if (upgh == 1) {
			digitalWrite(13, HIGH);
		}
		else {
			digitalWrite(13, LOW);
		}
	}

	if (String(pub.topic()) == "gs/sett")
	{
		int sett = payload.toInt();
		float temp = myHumidity.readTemperature();
		Sendtemphg();
		Sendgm();
		if (sett >= temp) {
			digitalWrite(10, HIGH);
			Sendir1();

		}
		else {
			digitalWrite(10, LOW);
			Sendir0();

		}

		if (sett < temp) {
			digitalWrite(14, HIGH);
			Sendfan1();
		}
		else {
			digitalWrite(14, LOW);
			Sendfan0();
		}
	}




	if (String(pub.topic()) == "gs/sethg")
	{
		int sethg = payload.toInt();
		float humd = myHumidity.readHumidity();
		Sendtemphg();
		Sendgm();
		if (sethg > humd) {
			digitalWrite(13, HIGH);
			Senduphg1();

		}
		else {
			digitalWrite(13, LOW);
			Senduphg0();

		}

		if (sethg < humd) {
			digitalWrite(14, HIGH);
			Sendfan1();
		}
		else {
			digitalWrite(14, LOW);
			Sendfan0();
		}
	}




	if (String(pub.topic()) == "gs/plant")
	{
		int plant = payload.toInt();
		Sendtemphg();
		Sendgm();
		float humd = myHumidity.readHumidity();
		float temp = myHumidity.readTemperature();
		if (plant == 1) {
			Send1();
			while (plant == 1) {

				Sendtemphg();
				if (myHumidity.readTemperature() < 22) {
					digitalWrite(10, HIGH);
					Sendir1();
				}
				else {
					digitalWrite(10, LOW);
					Sendir0();
				}
				if (myHumidity.readTemperature() > 22) {
					digitalWrite(14, HIGH);
					Sendfan1();
				}
				else {
					digitalWrite(14, LOW);
					Sendfan0();
				}
				if (myHumidity.readHumidity() < 60) {
					digitalWrite(13, HIGH);
					Senduphg1();

				}
				if (myHumidity.readHumidity() > 60) {
					digitalWrite(13, LOW);
					Senduphg0();

				}


				digitalWrite(15, HIGH); // uf on
				Senduf1();
				delay(1000);

			}
		}






		if (plant == 2) {
			Send2();
			while (plant == 2) {

				Sendtemphg();
				if (myHumidity.readTemperature() < 24) {
					digitalWrite(10, HIGH);
					Sendir1();
				}
				else {
					digitalWrite(10, LOW);
					Sendir0();
				}
				if (myHumidity.readTemperature() > 24) {
					digitalWrite(14, HIGH);
					Sendfan1();
				}
				else {
					digitalWrite(14, LOW);
					Sendfan0();
				}
				if (myHumidity.readHumidity() < 85) {
					digitalWrite(13, HIGH);
					Senduphg1();

				}
				if (myHumidity.readHumidity() > 85) {
					digitalWrite(13, LOW);
					Senduphg0();

				}


				digitalWrite(15, HIGH); // uf on
				Senduf1();
				delay(1000);

			}
		}







		if (plant == 3) {
			Send3();
			while (plant == 3) {

				float humd = myHumidity.readHumidity();
				float temp = myHumidity.readTemperature();
				Sendtemphg();

				if (temp < 27) {
					digitalWrite(10, HIGH);
					Sendir1();
				}
				else {
					digitalWrite(10, LOW);
					Sendir0();
				}
				if (temp > 27) {
					digitalWrite(14, HIGH);
					Sendfan1();
				}
				else {
					digitalWrite(14, LOW);
					Sendfan0();
				}
				if (humd < 80) {
					digitalWrite(13, HIGH);
					Senduphg1();

				}
				else {
					digitalWrite(13, LOW);
					Senduphg0();

				}


				digitalWrite(15, HIGH); // uf on
				Senduf1();
				if (pub.topic() == "gs/update") {
					break;
				}
				delay(1000);

			}
		}






		if (plant == 4) {
			Send4();
			while (plant == 4) {
				float humd = myHumidity.readHumidity();
				float temp = myHumidity.readTemperature();
				Sendtemphg();

				if (temp < 23) {
					digitalWrite(10, HIGH);
					Sendir1();
				}
				else {
					digitalWrite(10, LOW);
					Sendir0();
				}
				if (temp > 23) {
					digitalWrite(14, HIGH);
					Sendfan1();
				}
				else {
					digitalWrite(14, LOW);
					Sendfan0();
				}
				if (humd < 75) {
					digitalWrite(13, HIGH);
					Senduphg1();

				}
				else {
					digitalWrite(13, LOW);
					Senduphg0();

				}


				digitalWrite(15, HIGH); // uf on
				Senduf1();
				String payload = pub.payload_string();
				String(pub.topic()) == "gs/plant";
				int plant = payload.toInt();
				delay(1000);

			}
		}


	}

}






WiFiClient wclient;
PubSubClient client(wclient, mqtt_server, mqtt_port);

void setup() {

	//  sensors.begin();
	Serial.begin(115200);
	delay(10);
	myHumidity.begin();

	pinMode(14, OUTPUT);
	pinMode(12, OUTPUT);
	pinMode(13, OUTPUT);
	pinMode(15, OUTPUT);
	pinMode(10, OUTPUT);
}

void loop() {
	// подключаемся к wi-fi
	if (WiFi.status() != WL_CONNECTED) {
		Serial.print("Connecting to ");
		Serial.print(ssid);
		Serial.println("...");
		WiFi.begin(ssid, pass);

		if (WiFi.waitForConnectResult() != WL_CONNECTED)
			return;
		Serial.println("WiFi connected");
	}

	// подключаемся к MQTT серверу
	if (WiFi.status() == WL_CONNECTED) {
		if (!client.connected()) {
			Serial.println("Connecting to MQTT server");
			if (client.connect(MQTT::Connect("arduinoClient2")
				.set_auth(mqtt_user, mqtt_pass))) {
				Serial.println("Connected to MQTT server");
				client.set_callback(callback);
				client.subscribe("gs/uf");
				client.subscribe("gs/ir");
				client.subscribe("gs/pomp");
				client.subscribe("gs/fan");
				client.subscribe("gs/hg");
				client.subscribe("gs/temp");
				client.subscribe("gs/gm");
				client.subscribe("gs/uphg");
				client.subscribe("gs/sett");
				client.subscribe("gs/sethg");
				client.subscribe("gs/plant");
				client.subscribe("gs/update");

			}
			else {
				Serial.println("Could not connect to MQTT server");
			}
		}

		if (client.connected()) {
			client.loop();

		}
	}
}


// Функция отправки показаний

void Send1() {
	client.publish("gs/sett", String(22));
	client.publish("gs/sethg", String(60));
	delay(10);
}

void Send2() {
	client.publish("gs/sett", String(24));
	client.publish("gs/sethg", String(85));
	delay(10);
}

void Send3() {
	client.publish("gs/sett", String(27));
	client.publish("gs/sethg", String(80));
	delay(10);
}

void Send4() {
	client.publish("gs/sett", String(23));
	client.publish("gs/sethg", String(75));
	delay(10);
}

void Senduf0() {
	client.publish("gs/uf", String(0));
	delay(10);
}
void Senduf1() {
	client.publish("gs/uf", String(1));
	delay(10);
}

void Sendir0() {
	client.publish("gs/ir", String(0));
	delay(10);
}
void Sendir1() {
	client.publish("gs/ir", String(1));
	delay(10);
}

void Sendpomp0() {
	client.publish("gs/pomp", String(0));
	delay(10);
}
void Sendpomp1() {
	client.publish("gs/pomp", String(1));
	delay(10);
}

void Sendfan0() {
	client.publish("gs/fan", String(0));
	delay(10);
}
void Sendfan1() {
	client.publish("gs/fan", String(1));
	delay(10);
}

void Senduphg0() {
	client.publish("gs/uphg", String(0));
	delay(10);
}
void Senduphg1() {
	client.publish("gs/uphg", String(1));
	delay(10);
}

void Rest() {
	Sendtemphg();
	Sendgm();
}


void Sendtemphg() {
	float humd = myHumidity.readHumidity();
	float temp = myHumidity.readTemperature();
	client.publish("gs/temp", String(temp, 1));
	client.publish("gs/hg", String(humd, 1));
	delay(10);
}

void Sendgm() {
	int AirValue = 840;
	int WaterValue = 430;
	int soilMoistureValue = 0;
	soilMoistureValue = analogRead(A0);

	if (soilMoistureValue > 430 && soilMoistureValue < 500)
	{
		Serial.println("Very Wet");
		client.publish("gs/gm", String("Very Wet"));
	}
	else if (soilMoistureValue > 500 && soilMoistureValue < 700)
	{
		Serial.println("Wet");
		client.publish("gs/gm", String("Wet"));
	}
	else if (soilMoistureValue < 850 && soilMoistureValue >700)
	{
		Serial.println("Dry");
		client.publish("gs/gm", String("Dry"));
	}

	delay(10);
}