/*
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    KL25Z-Controle.cpp
 * @brief   Application entry point.
 */
#include <inc/Control_ADC.h>
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "arm_math.h" //Operações DSP
#include "Control.h" //API principal para aplicações Controle
#include "Interrupts.h"
#include "Control_DAC.h"
#include "Control_PWM.h"
#include "Matrix.h"
#include <vector>
/* TODO: insert other definitions and declarations here. */

//DEBUG PIN -> PTC9
#define DEBUG_PIN_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_INITPINS_DEBUG_PIN_GPIO, 1U << BOARD_INITPINS_DEBUG_PIN_PIN)

/*
 * @brief   Application entry point.
 */




void CtrlLaw();

int main(void) {


  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    //Control::delay(1000000);


    Control::setSamplingFrequency(100);
    Control::setControlLawHandle(CtrlLaw);
    Control::start();

    //Teste da API pro DAC
    Control::DAC::Init();

    //Teste da API pro ADC
    Control::ADC PTC1(15);
    Control::ADC PTC2(11);

    //Teste da API do PWM
    Control::PWM::setFrequency(10000);
    Control::PWM PWM0(1);

    //Teste da API de MATRIZES
    float A[] = {1,2,3,4};
    Matrix MA(2,2,A);
    Matrix MB(2,2);
    MB.Inverse(MA, MB);
    MB.Print();


    uint32_t adcval1,adcval2;
    float pi = M_1_PI;

    while(1) {
    //	LED_RED_TOGGLE();
    	Control::delay(1000000);
    	adcval1 = PTC1.getConversion();
    	adcval2 = PTC2.getConversion();
    	CONTROLE_PRINT("PTC1 : %d PTC2 : %d \r\n",adcval1,adcval2);
    	CONTROLE_PRINT("pi = %f\r\n",pi);

}

}


int k = 0;
bool Up = true;
bool init = false;
//Escrever aqui lei de controle!
void CtrlLaw(){

//Static para ser construído apenas uma vez
static Control::PWM PWM0(0);

	LED_BLUE_TOGGLE();
	DEBUG_PIN_TOGGLE();
	if(Up){
		k++;
		//PWM1.setDuty(10);
		if(k==101)
			Up = false;

	} else { //Down
		k--;
		if(k==-1)
			Up = true;

	}

	PWM0.setDuty(k);
	if(k>0)
	Control::DAC::SetValue(k*40);

}




