#include <msp430.h>
#include <global.h>

char message[255] = {'1','2','3','l','o',' ','o','s','s','i',' ', '1'};

void test_antenner_deploy(void) {
	Ant_Deploy_Init();
	Start_Ant_Deploy_After(5);
}

void test_tmp101(void) {
	// OBC temp sensor
    volatile uint16_t temp;

	TMP10x_Init(OBCTEMP_ADDR);
    temp = TMP10x_Read(); // C = temp / 0x10
}

void test_eeprom(void) {
	volatile unsigned int i;
    volatile unsigned char x;

	EEPROM_Init(EEPROM_ADDR);

	for(i=0; i < 1;i++ ) {
		  ascii(message[i]);
		  EEPROM_ByteWrite(i, message[i]);
		  EEPROM_AckPolling();
	}

	for(i=0; i < 1;i++ ) {
		  x=EEPROM_RandomRead(i);
		  ascii(x);
	}
}

void test_panneltemp()
{
    volatile uint16_t temp;
    volatile uint8_t x;

    PCA9548A_Init(PCA9548A_ADDR);
    PCA9548A_SetChannel(PCA9548A_CH_PANELTEMP);

 	TMP10x_Init(PANELTEMP_ADDR);
    temp = TMP10x_Read(); // C = temp / 0x10
    temp = temp+1;
    if ((temp/16) > 28)
    	LED_ON();
    else
    	LED_OFF();
}

void test_payload()
{
	PCA9548A_Init(PCA9548A_ADDR);
    PCA9548A_SetChannel(2);

    Payload_Init();
    while(1) {
		Payload_Light_Off();
		_delay();
		Payload_Light_On();
		_delay();
    }
}

void test_payload_with_tmp100()
{
    volatile uint16_t temp;
    volatile uint8_t x;

    PCA9548A_Init(PCA9548A_ADDR);
    PCA9548A_SetChannel(PCA9548A_CH_PANELTEMP);

 	TMP10x_Init(PANELTEMP_ADDR);
    temp = TMP10x_Read(); // C = temp / 0x10
    temp = temp+1;

	PCA9548A_Init(PCA9548A_ADDR);
    PCA9548A_SetChannel(2);

    Payload_Init();

    	if ((temp/16) > 29)
    		Payload_Light_On();
    	else
    		Payload_Light_Off();
		_delay();

}

void test_adc(void)
{
   volatile uint16_t v;

   v = ADC_Get_Voltage_A1(VSOLAR);
   v = v+1;
   v = ADC_Get_Voltage_A1(VBUS);
   v = v+1;
   v = ADC_Get_Voltage_A1(VBAT);
   v = v+1;
   v = ADC_Get_Voltage_A1(VSUPERCAP);
   v = v+1;

   v = ADC_Get_Voltage_A0(SOLAR_CS1);
   v = v+1;
   v = ADC_Get_Voltage_A0(SOLAR_CS2);
   v = v+1;
   v = ADC_Get_Voltage_A0(SOLAR_CS3);
   v = v+1;
   v = ADC_Get_Voltage_A0(SOLAR_CS4);
   v = v+1;
   v = ADC_Get_Voltage_A0(SOLAR_CS5);
   v = v+1;
   v = ADC_Get_Voltage_A0(SUN_IN);
   v = v+1;
}

void test_watchdog(void)
{
	Watchdog_External_Init();
}
