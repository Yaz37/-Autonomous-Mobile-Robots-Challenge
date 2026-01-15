void Custom_Delay(unsigned int count) {

   unsigned int i, j;

    for (i = 0; i < count; i++) {

        for (j = 0; j < 500; j++) {

            asm nop;

        }

    }

}

void PWM(){

  PR2 = 0xFF;

  CCP1CON = 0x0C;

  CCP2CON = 0x0C;

  T2CON = 0x05;

}

void ADC_setup(){

   ADCON0 = 0x41;

   ADCON1 = 0x80;

}

unsigned int ADC_Read(){

   ADCON0 = ADCON0 | 0x04;

   while(ADCON0 & 0x04);

   return ((ADRESH * 256)+ ADRESL);

}



void wheel_s(unsigned char L_S,unsigned char R_S){

  CCPR2L = L_S;

  CCPR1L = R_S;

}



void wait_3s(){

   char i;

   T1CON = 0x31;

   for(i=0; i< 15 ;i++){

   TMR1H= 0x3C;

   TMR1L= 0xB0;

   PIR1 = PIR1 & 0xFE;

   while((PIR1 & 0x01) == 0);

   }

   T1CON = T1CON & 0xFE;

}





void main() {

    TRISA |= 0x01;

    TRISB = 0xFD;

    TRISC &= 0xD8;

    TRISD &= 0xD0;

    PORTD = PORTD & 0xF0;

    ADC_setup();

    OPTION_REG = 0x7F;

    while((PORTB & 0x01) == 0x01);

    wait_3s();

    PWM();

    PORTC = PORTC | 0x20;



    while(1){

        unsigned int light = ADC_Read();
        unsigned char sensors = PORTB & 0x18;




        if(light > 500){

            PORTC |= 0x01;

        } else {

            PORTC &= 0xFE;

        }



        if (((PORTB & 0x20) == 0) && ((PORTB & 0x80) == 0)) {

            wheel_s(0, 0);

            Custom_Delay(150);



            PORTD = (PORTD & 0xF0) | 0x04;

            wheel_s(0, 150);

            Custom_Delay(150);

        }

        else if (((PORTB & 0x20) == 0) && ((PORTB & 0x40) == 0)) {

            wheel_s(0, 0);

            Custom_Delay(150);



            PORTD = (PORTD & 0xF0) | 0x01;

            wheel_s(150, 0);

            Custom_Delay(150);

        }

        else if ((PORTB & 0x20) == 0) {

            wheel_s(0, 0);

            Custom_Delay(150);



            PORTD = (PORTD & 0xF0) | 0x04;

            wheel_s(0, 150);

            Custom_Delay(150);

        }

        else if ((PORTB & 0x80) == 0) {

            PORTD = (PORTD & 0xF0) | 0x04;

            wheel_s(0, 100);

        }

        else if ((PORTB & 0x40) == 0) {

            PORTD = (PORTD & 0xF0) | 0x01;

            wheel_s(0, 100);

        }

        if(sensors == 0x00) {

        PORTD = (PORTD & 0xF0) | 0x05;
        wheel_s(150, 150);
        }

        else if(sensors == 0x08) {
        PORTD = (PORTD & 0xF0) | 0x04;
        
        wheel_s(0, 100);

            }
        else if(sensors == 0x10) {
        PORTD = (PORTD & 0xF0) | 0x01;
        wheel_s(100, 0);

        }
        else {
        PORTD = (PORTD & 0xF0) | 0x05;
        wheel_s(120, 120);
         }

        }

    }

