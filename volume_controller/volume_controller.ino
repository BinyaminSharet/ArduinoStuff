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

//#define USE_SERIAL  1
#ifndef USE_SERIAL 
#define USE_REMOTE 1
#endif

//#define USE_DUE

#define safety_pin 10
#define encoder_pin_a 3//51
#define encoder_pin_b 4//49
#define encoder_push_d 2//53

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

#ifdef USE_DUE
    attachInterrupt( encoder_pin_a, encoder_handler, CHANGE ); // encoder pin on interrupt 1 (pin 3)
    attachInterrupt( encoder_push_d, pushed, FALLING ); // encoder pin on interrupt 1 (pin 2)
#else
    attachInterrupt( 0, encoder_handler, CHANGE ); // encoder pin on interrupt 1 (pin 3)
    attachInterrupt( 1, pushed, FALLING ); // encoder pin on interrupt 1 (pin 2)
#endif

#ifdef USE_SERIAL
    Serial.begin( 115200 );
#endif
}

int safety()
{
    return digitalRead( safety_pin );
}

void check_encoder()
{
    int pos;
    pos = encoder_pos;

    if( pos != 0 )
    {
#if USE_SERIAL
      Serial.println( pos );
#endif
        if ( pos > 0 )
        {
          --encoder_pos;
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
          ++encoder_pos;
#ifdef USE_SERIAL
            Keyboard.write( 'd' );
#endif
#ifdef USE_REMOTE
            Remote.decrease();
            Remote.clear();
#endif
        }
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
    delay( 10 );
}

void loop() {
    if ( safety( ) )
    {
        safe_loop();
    }
}

int n = LOW;
int encoder_pin_a_last = LOW;
unsigned long last_millis = 0;

int calc_rotation_speed()
{
    unsigned long time = millis() + 500;
    int delta = time - last_millis;
    if ( delta < 500 )
    {
      return (500 - delta) / 4;
    }
    return 1;
}
void encoder_handler()
{
    int rotation_speed = calc_rotation_speed();
    n = digitalRead( encoder_pin_a );
    rotation_speed = 1;
    if ( ( encoder_pin_a_last == LOW ) && ( n == HIGH ) )
    {
        if ( digitalRead( encoder_pin_b ) == HIGH )
        {
            encoder_pos += rotation_speed;
        }
        else
        {
            encoder_pos -= rotation_speed;
        }
    }
    encoder_pin_a_last = n;
}

void pushed()
{
    button_pushed = 1;
}

