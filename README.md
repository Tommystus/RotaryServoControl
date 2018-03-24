# RotaryServoControl
Use hard-drive based rotary switch to control servo motor on Arduino board.

Circuit based on circuit by Jan Florian Wagner in this [article](http://www.sp2swj.sp-qrp.pl/DDS_DL4JAL/impulsator/OH2GHR_Inexpensive_rotary_encoder.htm).

Basic circuit:

![basic verion](https://github.com/Tommystus/RotaryServoControl/raw/master/Rotary%20Encoder%20using%203%20Phase%20Hard%20Drive%20Motor.png)

Improvements:
* Add bias resistor to ensure output = 1 by default.  The + input of comparator may not have enough bias by default to keep output high and may result in oscillation.
* Add positive feedback resistor to create hysteresis and reduce bounce.  The switch between logic states (high to low to high) is not clean and alway include dozen of bounces each lasting between 2 to 10 microsec.  With 60K positive feedback, only 2 or 3 short bounces are observed.

![Improved version](https://github.com/Tommystus/RotaryServoControl/raw/master/Rotary%20Encoder%20circuit%20-%20Improved.png)

Output Sequences:

![Output](https://github.com/Tommystus/RotaryServoControl/raw/master/Rotary%20Encoder%20Output%20Sequence.png)

Arduino code to use with rotary encoder using state table in in src code that is also used to control a servo.  See this [video](https://www.youtube.com/watch?v=Rl_HCxWeEqw).

