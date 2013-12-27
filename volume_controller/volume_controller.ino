
/*
   This is important, all code for keyboard stuff should be inside a safe block, 
   To enable , connect pin 10 to VCC.
   To disable, connect to GND
   
   pins used:
   2   rotary encoder D ( push button )
   3   rotary encoder A
   4   rotary encoder B
   5   rotary encoder E ( push button )
   GND rotary encoder C 
   10  safty - disable - GND. enable - VCC
 */

const int safety_pin = 10;
const int encoder_pin_a = 3;
const int encoder_pin_b = 4;
const int encoder_push_d = 2;
const int encoder_push_e = 5;

int pos, old_pos;
volatile int encoder_pos = 0; // variables changed within interrupts are volatile
volatile int button_pushed = 0;

void setup() {
    pinMode(encoder_pin_a, INPUT);
    digitalWrite(encoder_pin_a, HIGH);
    pinMode(encoder_pin_b, INPUT);
    digitalWrite(encoder_pin_b, HIGH);
    pinMode(encoder_push_d, INPUT);
    digitalWrite(encoder_push_d, HIGH);
    pinMode(encoder_push_e, INPUT);
    digitalWrite(encoder_push_e, HIGH);
    pinMode( safety_pin, INPUT );
    digitalWrite( safety_pin, LOW );
    attachInterrupt(0, do_encoder, FALLING); // encoder pin on interrupt 1 (pin 3)
    attachInterrupt(1, pushed, FALLING); // encoder pin on interrupt 1 (pin 2)
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
            Remote.decrease();
            Remote.clear();
        }
        old_pos = pos;
    }

    // check and handle button press
    if ( button_pushed )
    {
        button_pushed = 0;
        Remote.mute();
        Remote.clear();
    }
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

// access register directly...
void do_encoder()
{
    if (digitalRead(encoder_pin_a) == digitalRead(encoder_pin_b))
        encoder_pos--;    // count up if both encoder pins are the same
    else
        encoder_pos++;    // count down if pins are different
}

void pushed()
{
    button_pushed = 1;
}

