/* Buzzer frequency finder
 *
 * This sketch is useful in figuring out the frequency at which a
 * buzzer is loudest, in case you can't find a datasheet for it.
 *
 * Connect buzzer between pins 9 and GND.
 *
 * Open up the serial monitor (57600, Newline on [Enter])
 *
 *  - Press [+] + [Enter] to increse frequency
 *  - Press [-] + [Enter] to decrease frequency 
 *  - Press [i] + [Enter] to switch between increments (1, 10, 100, 1000 Hz) 
 *  - Press [Space] + [Enter] to toggle buzzer on/off
 *  - You can also type a frequency in Hz + [Enter]
 */
 
long freq = 1000;
bool buzzing = false;
unsigned short inc = 1000;
long n = 0;

void buzzer_freq(unsigned short freq)
{
  // Set top to PWM_PERIOD
  unsigned short pwm_period = F_CPU / freq;
  ICR1 = pwm_period;

  // Set duty cycle = 50%
  OCR1A = pwm_period / 2;
}

void buzzer_on()
{
  // Start the timer, no prescaler (CS1=1)
  TCCR1B |= _BV(CS10);
}

void buzzer_off()
{
  // Stop the timer (CS1=0)
  TCCR1B &= ~(_BV(CS10) | _BV(CS11));
}

void setup()
{
  Serial.begin(57600);
  pinMode(9, OUTPUT);

  // Top is ICR1 (WGM1=14), non-inverting PWM on pin 9 (COM1A=2) p.135
  TCCR1A = _BV(WGM11) | _BV(COM1A1);
  TCCR1B = _BV(WGM13) | _BV(WGM12);

  buzzer_freq(freq);
}

void loop() {
  char c = Serial.read();
  switch(c) {
    case 'i':
      if (inc == 1000) inc = 100;
      else if (inc == 100) inc = 10;
      else if (inc == 10) inc = 1;
      else if (inc == 1) inc = 1000;
      Serial.print("inc=");
      Serial.println(inc);
      break;
    case '+':
      if (freq + inc <= 65535) {
        freq += inc;
        buzzer_freq(freq);
      }
      Serial.print("f=");
      Serial.println(freq);
      break;
    case '-':
      if (freq - inc >= 300) {
        freq -= inc;
        buzzer_freq(freq);
      }
      Serial.print("f=");
      Serial.println(freq);
      break;
    case ' ':
      buzzing = !buzzing;
      if (buzzing) {
        buzzer_on();
        Serial.println("buzzer on");
      } else {
        buzzer_off();
        Serial.println("buzzer off");
      }
      break;
    case '\n':
      if (n != 0) {
        freq = n;
        n = 0;
        buzzer_freq(freq);
        Serial.print("f=");
        Serial.println(freq);
        break;
      }
  }
  if (c >= '0' && c <= '9') {
    n *= 10;
    n += c - '0';
  }
}
