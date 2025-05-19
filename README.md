# An Arduino based led clock that reads and manipulates a real-time clock (RTC) DS3231, and also three buttons:
   * add hour
   * add minute
   * switch 12/24 hour mode.

### This arduino project performs the following:

#### 1) Digitally controls 28 independent LEDs;
#### 2) Each clock digit has 7 LEDs;
#### 3) The clock itself has 2 digits on the left, a colon in the middle and 2 digits on the right;
#### 4) The colon blinks once every second;
#### 5) The first button increases the hour. When in 12 hour mode, the hour goes back to 1 right after 12. When in 24 hour mode, the hour going to 0 (zero) after 23;
#### 6) The second button increases the minute. The minute goes to 0 (zero) after 59.
#### 7) The digits shine through a picture, which was printed on regular office paper.
