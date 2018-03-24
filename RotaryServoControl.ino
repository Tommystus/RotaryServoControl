/*********************************************************************
Servo characterization using rotary encoder to control
	Servo Pin:
	- Black / Brown GND
	- Red / Orange (center) +5v
	- White / Yellow => PWM or Analog pin 9

	Rotary encoder pin:
	- Output A => pin 2
	- Output B => pin 3

********************************************************************/

#include <Servo.h>

const int ENC_A 	= 2;
const int ENC_B 	= 3;
const int SERVO_PIN = 9;

int 		lastSv = 90;	// center start
boolean 	servoAttached = false;
unsigned long 	servoIdleTime = 0;

Servo kp90Servo;

void setup()   {
//	Serial.begin(9600);

	pinMode( ENC_A, INPUT_PULLUP);
	pinMode( ENC_B, INPUT_PULLUP);

	kp90Servo.attach(SERVO_PIN);
	kp90Servo.write(lastSv); // initialize starting position
	delay(200);
	kp90Servo.detach();

//	Serial.println("Starting...");

}


void loop()
{
	int rv = rotEncRead(ENC_A, ENC_B);

	if (rv != 0)
	{
		int currSv = lastSv + rv;
//		Serial.println(currSv);

		if (currSv > 180) currSv = 180;
		if (currSv < 1) currSv = 1;

		if (lastSv != currSv)
		{
			lastSv = currSv;
			if (!servoAttached)
				kp90Servo.attach(SERVO_PIN);
			servoAttached = true;
			servoIdleTime = millis();
			kp90Servo.write(lastSv);
		}
		delay(50);
	}
	else
	{
		// disable attached servo after 800 msec idle time
		if (servoAttached)
		{
			if ((millis() - servoIdleTime) > 800)
			{
				kp90Servo.detach();
				servoAttached = false;
			}
		}
	}
}

static int rotEncRead( int pa, int pb)
{
	unsigned long 	sampleTime = millis();
	int rv = encRead(pa,pb);

	if (rv != 0)
	{
		sampleTime = millis() - sampleTime;
		if (sampleTime < 30)
			sampleTime = 30;
		return rv*(1 + int(90000.0/(sampleTime*sampleTime)));
	}
	return 0;
}

/* Hard drive motor rotary Encoder functions for line Pa,Pb
	11-01-00-10-11 reverse
	11-10-00-01-11 forward
*/
// State table
static uint8_t stb[][4] =
{
//       0 1 2 3
        {8,1,2,8}, //0 start here v=3
        {3,1,8,0}, //1 3*1(-0-2-3 -1
        {4,8,2,0}, //2 3*2(-0-1-3
        {3,1,5,8}, //3 3-1*0(-2-3 -1
        {4,6,2,8}, //4 3-2*0(-1-3
        {3,8,5,7}, //5 3-1-0*2(-3 -1
        {4,6,8,9}, //6 3-2-0*1(-3
};

/* Read encoder state
	return -1, 0, +1 depending on direction of rotation
*/
static int encRead( int pa, int pb)
{
	int8_t cx = 0; 			// initial state 0
	int errCnt = 0;
	while (errCnt < 550)
	{
		uint8_t v = digitalRead(pa) | digitalRead(pb)<<1;
		if (v < 4)
		{
			uint8_t nx = stb[cx][v];

			if (nx > 6)
			{
				if (nx == 8)
				{
//					cx = 0;
					errCnt++;
				}
				else
					return (8 - (int)nx);
			}
			else
				cx = nx;
//			delay(1);
		}
		else
		{
			cx = 0;
			errCnt++;
		}
	}

	return 0;
}



