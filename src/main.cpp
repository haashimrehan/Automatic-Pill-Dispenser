#include <WiFi.h>
#include <FirebaseESP32.h>
#include <ESP32Servo.h>

#include <Credentials.h>

//2. Define FirebaseESP8266 data object for data sending and receiving
FirebaseData fbdo;

Servo myservo; 
int servoPin = 18;      // GPIO pin used to connect the servo control (digital out)
// Possible ADC pins on the ESP32: 0,2,4,12-15,32-39; 34-39 are recommended for analog input
int ADC_Max = 4096;   

void dispense() {
myservo.write(10);                  // set the servo position according to the scaled value
  delay(2000); 
  myservo.write(180);                  // set the servo position according to the scaled value
  delay(2000);
}
void setup()
{
	// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
    myservo.setPeriodHertz(50);// Standard 50hz servo
  myservo.attach(servoPin, 500, 2400); 

  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  //4. Enable auto reconnect the WiFi when connection lost
  Firebase.reconnectWiFi(true);

  //5. Try to set int data to Firebase
  //The set function returns bool for the status of operation
  //fbdo requires for sending the data
  if(Firebase.setInt(fbdo, "/LED_Status", 1))
  {
    //Success
     Serial.println("Set int data success");

  }else{
    //Failed?, get the error reason from fbdo

    Serial.print("Error in setInt, ");
    Serial.println(fbdo.errorReason());
  }

//int[] schedule = new int[7];
 int schedule[7];
  //6. Try to get int data from Firebase
  //The get function returns bool for the status of operation
  //fbdo requires for receiving the data
  if(Firebase.getString(fbdo, "/Pills/pill1/Schedule"))
  {
    //Success
    Serial.print("Get int data success, int = ");
  
    String sch = fbdo.stringData();
     for (int i = 0; i < 12; i++){
         String t =sch.substring(i-1,i); 
         if (t == ",") continue;
        Serial.println(t);
        schedule[i] = t.toInt();
   // Serial.println(schedule[i]);
     }
     //schedule = sch.split(",");
     //char* command = strtok(sch, ",");
     //String str = "123 456 789";

    Serial.println(fbdo.stringData());

  }else{
    //Failed?, get the error reason from fbdo

    Serial.print("Error in getInt, ");
    Serial.println(fbdo.errorReason());
  }

  /*
  In case where you want to set other data types i.e. bool, float, double and String, you can use setBool, setFloat, setDouble and setString.
  If you want to get data which you known its type at specific node, use getInt, getBool, getFloat, getDouble, getString.
  If you don't know the data type at specific node, use get and check its type.
  The following shows how to get the variant data
  */

 if(Firebase.get(fbdo, "/"))
  {
    //Success
    Serial.print("Get variant data success, type = ");
    Serial.println(fbdo.dataType());

    if(fbdo.dataType() == "int"){
      Serial.print("data = ");
      Serial.println(fbdo.intData());
    }else if(fbdo.dataType() == "bool"){
      if(fbdo.boolData())
        Serial.println("data = true");
      else
        Serial.println("data = false");
    }

  }else{
    //Failed?, get the error reason from fbdo

    Serial.print("Error in get, ");
    Serial.println(fbdo.errorReason());
  }

  /*
  If you want to get the data in realtime instead of using get, see the stream examples.
  If you want to work with JSON, see the FirebaseJson, jsonObject and jsonArray examples.
  If you have questions or found the bugs, feel free to open the issue here https://github.com/mobizt/Firebase-ESP32
  */
dispense();
dispense();

}


void loop()
{//dispense();

}
