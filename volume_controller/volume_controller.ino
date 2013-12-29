/*
   This is important, all code for keyboard stuff should be inside a safe block, 
   To enable , connect pin 10 to VCC.
   To disable, connect to GND

   pins used:
   2   rotary encoder D ( push button )
   3   rotary encoder A
   4   rotary encoder B
   GND rotary encoder E 
   GND rotary encoder C 
   10  safty - disable - GND. enable - VCC
 */

const int safety_pin = 10;
const int encoder_pin_a = 3;
const int encoder_pin_b = 4;
const int encoder_push_d = 2;
const int encoder_push_e = 5;

//#define USE_SERIAL  1
#ifndef USE_SERIAL 
#define USE_REMOTE 1
#endif

int pos, old_pos;
volatile int encoder_pos = 0; // variables changed within interrupts are volatile
volatile int button_pushed = 0;

void setup() {
    // set those pins as input
    pinMode( encoder_pin_a, INPUT );
    pinMode( encoder_pin_b, INPUT );
    pinMode( encoder_push_d, INPUT );
    pinMode( safety_pin, INPUT );

    // use pull-ups for all input pins
    digitalWrite( encoder_pin_a, HIGH );
    digitalWrite( encoder_pin_b, HIGH );
    digitalWrite( encoder_push_d, HIGH );
    digitalWrite( safety_pin, HIGH );

    attachInterrupt( 0, encoder_handler, CHANGE ); // encoder pin on interrupt 1 (pin 3)
    attachInterrupt( 1, pushed, FALLING ); // encoder pin on interrupt 1 (pin 2)

#ifdef USE_SERIAL
    Serial.begin( 9600 );
#endif
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
#ifdef USE_SERIAL
            Keyboard.write( 'u' );
#endif
#ifdef USE_REMOTE
            Remote.increase();
            Remote.clear();
#endif
        }
        else
        {
#ifdef USE_SERIAL
            Keyboard.write( 'd' );
#endif
#ifdef USE_REMOTE
            Remote.decrease();
            Remote.clear();
#endif
        }
        old_pos = pos;
    }

    // check and handle button press
    if ( button_pushed )
    {
        button_pushed = 0;
#ifdef USE_SERIAL
        Keyboard.write( 'p' );  
#endif
#ifdef USE_REMOTE
        Remote.mute();
        Remote.clear();
#endif
    }
}

void safe_loop()
{
    check_encoder( );
    delay( 100 );
}

void loop() {
    if ( safety( ) )
    {
        safe_loop();
    }
}

int n = LOW;
int encoder_pin_a_last = LOW;
void encoder_handler()
{
    n = digitalRead( encoder_pin_a );
    if ( ( encoder_pin_a_last == LOW ) && ( n == HIGH ) )
    {
        if ( digitalRead( encoder_pin_b ) == HIGH )
        {
            encoder_pos++;
        }
        else
        {
            encoder_pos--;
        }
    }
    encoder_pin_a_last = n;
}

void pushed()
{
    button_pushed = 1;
}

