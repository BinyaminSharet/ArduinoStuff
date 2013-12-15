
/*
   This is important, all code for keyboard stuff should be inside a safe block, 
   To enable , connect pin 10 to VCC.
   To disable, connect to GND
 */

const int safety_pin = 10;
const int encoder_pin_a = 3;
const int encoder_pin_b = 2;
int pos, old_pos;
volatile int encoder_pos = 0; // variables changed within interrupts are volatile

void setup() {
    pinMode(encoder_pin_a, INPUT);
    digitalWrite(encoder_pin_a, HIGH);
    pinMode(encoder_pin_b, INPUT);
    digitalWrite(encoder_pin_b, HIGH);
    pinMode( safety_pin, INPUT );
    digitalWrite( safety_pin, LOW );
    attachInterrupt(1, do_encoder, FALLING); // encoder pin on interrupt 1 (pin 3)
    Serial.begin( 9600 );
}

int safety()
{
    return digitalRead( safety_pin );
}

void check_encoder()
{
    pos = encoder_pos;

    if( pos != old_pos )
    {
        if ( pos > old_pos )
        {
            Remote.increase();
            Remote.clear();
        }
        else
        {
            Remote.clear();
          Remote.decrease();
        }
        old_pos = pos;
    }
    else
    {
    }

    // check and handle button press
}

void direct_write( uint8_t c)
{
  Keyboard.press_direct( c );
  Keyboard.release_direct( c ); 
}

void safe_loop()
{
    check_encoder( );
    delay( 50 );
}

void loop() {
    if ( safety( ) )
    {
        safe_loop();
    }
}

void do_encoder()
{
    if (digitalRead(encoder_pin_a) == digitalRead(encoder_pin_b))
        encoder_pos++;    // count up if both encoder pins are the same
    else
        encoder_pos--;    // count down if pins are different
}

