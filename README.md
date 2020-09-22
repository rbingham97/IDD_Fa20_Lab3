# Motors, Power, Paper Prototyping

## Overview
For this assignment, you are going to 

A) [Actuating DC Motors](#part-a-actuating-dc-motors) 

B) [Actuating Servo Motors](#part-b-actuating-servo-motors) 

C) [Integrating Input and Output](#part-c-integrating-input-and-output)

D) [Autonomy!](#part-d-autonomy)

E) [Paper Display](#part-e-paper-display) 

F) [Make it your own](#part-f-make-it-your-own)


## Part A. Actuating DC motors

**Use the [Fade](https://www.arduino.cc/en/tutorial/fade) circuit from Lab 01 (```Examples > 01 Basics > Fade```) and connect the vibration motor's red wire to the pin which normally would be supplying a LED with PWM'd voltage.  The other blue wire from the vibration motor should be connected to ground, to complete the circuit. SECURE THE MOTOR TO KEEP IT FROM SHAKING ITSELF OUT OF THE CIRCUIT!**

Done! The vibration motor slowly pulses between vibrating a lot and not vibrating at all, as expected. 

## Part B. Actuating Servo motors

### Part 1. Connect the Servo to your breadboard

**a. Which color wires correspond to power, ground and signal?**

Commercial servos tend to conform to a standard coloring where red is power, brown is ground, and orange is signal. When wired up like this, our microservo works!

### Part 2. Connect the Servo to your Arduino

**a. Which Arduino pin should the signal line of the servo be attached to?**

The sketch specifies that the code is controlling pin 9. Seems like the Arduino example sketches follow a theme here...

**b. What aspects of the Servo code control angle or speed?**

The sketch implements the desired functionality with two for loops where the code writes a position (in degrees) to the servo, then waits a delay of milliseconds to allow the motor to arrive at the desired position before writing the next angle along the sweep. The range of angles specified in the loop bounds for 'pos' control the sweep angle, and the length of the delay time between position writes determines the speed of the sweeping.

## Part C. Integrating input and output

**Using what you've learned already, write code to control the servo motor circuit.**

I decided to implement the voltage divider circuit option, and scaled the input/output magnitudes such that the motor turns at the same range as the potentiometer does. Thus the potentiometer feels very tactile when controlling the motor - the motor appears to be connected to the potentiometer physically because it always stays at the same angle as the knob. 

## Part D. Autonomy!

**Include a photo/movie of your autonomous device in action.**

The video of my knob/motor device working with the battery is [here](https://youtu.be/FLuTQ4Ajbws). 

## Part E. Paper display

**Make a paper display that uses the servo to show how many times a button on the front has been pressed (or any other thing you can sense or count). Ideally you should design it so that you can slide the breadboard out to work on the circuit or programming, and then slide it back in and reattach a few wires to be back in operation.**

NYI (coming this afternoon!)

**a. Make a video of your paper display in action.**

NYI (coming this afternoon!)

## Part F. Make it your own

**a. Make a video of your final design in action.**

NYI (coming this afternoon!)
