/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.4
        Device            :  PIC24FJ64GA002
    The generated drivers are tested against the following:
        Compiler          :  XC16 v2.10
        MPLAB 	          :  MPLAB X v6.05
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

        
/*
 * Copyright (c) 2023 Ralph Futa
 * Licensed under the MIT License.
 */
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/tmr1.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/adc1.h"
#include "mcc_generated_files/interrupt_manager.h"
#include <math.h>
#include <stdbool.h> 
#define RAD_TO_DEG (180.0 / M_PI)


int nombre=0;
double angle360=0.0;
bool fixed=false;
double angle_ref=0.0;

// Configuration pour l'afficheur
void DecToBin(int value){      
    if(value==0){ 

            BIT_A_SetLow(); 

            BIT_B_SetLow(); 

            BIT_C_SetLow(); 

            BIT_D_SetLow();       

    }else if(value==1){ 

            BIT_A_SetHigh(); 

            BIT_B_SetLow(); 

            BIT_C_SetLow(); 

            BIT_D_SetLow(); 

    }else if(value==2){ 

            BIT_A_SetLow(); 

            BIT_B_SetHigh(); 

            BIT_C_SetLow(); 

            BIT_D_SetLow(); 

    }else if(value==3){ 

            BIT_A_SetHigh(); 

            BIT_B_SetHigh(); 

            BIT_C_SetLow(); 

            BIT_D_SetLow(); 

    }else if(value==4){ 

            BIT_A_SetLow(); 

            BIT_B_SetLow(); 

            BIT_C_SetHigh(); 

            BIT_D_SetLow(); 

    }else if(value==5){ 

            BIT_A_SetHigh(); 

            BIT_B_SetLow(); 

            BIT_C_SetHigh(); 

            BIT_D_SetLow(); 

    }else if(value==6){ 

            BIT_A_SetLow(); 

            BIT_B_SetHigh(); 

            BIT_C_SetHigh(); 

            BIT_D_SetLow(); 

    }else if(value==7){ 

            BIT_A_SetHigh(); 

            BIT_B_SetHigh(); 

            BIT_C_SetHigh(); 

            BIT_D_SetLow(); 

    }else if(value==8){ 

            BIT_A_SetLow(); 

            BIT_B_SetLow(); 

            BIT_C_SetLow(); 

            BIT_D_SetHigh(); 

    }else if(value==9){  

            BIT_A_SetHigh(); 

            BIT_B_SetLow(); 

            BIT_C_SetLow(); 

            BIT_D_SetHigh(); 

    } 

} 

int timercount=0; 

int AfficherNombre(){ 

    //On divise notre nombre en 4 digits 
     int Digit1 = nombre/1000; 

     int Digit2 = (nombre%1000)/100; 

     int Digit3 = (nombre%100)/10; 

     int Digit4 = (nombre%10); 

     if(timercount==1){ 

         DIGIT_1_SetLow(); 
         DIGIT_2_SetHigh(); 
         DIGIT_3_SetHigh(); 
         DIGIT_4_SetHigh(); 
         DecToBin(Digit1); 

     }else if(timercount==2){ 

         DIGIT_1_SetHigh(); 
         DIGIT_2_SetLow(); 
         DIGIT_3_SetHigh(); 
         DIGIT_4_SetHigh(); 
         DecToBin(Digit2); 

     }else if(timercount==3){ 

         DIGIT_1_SetHigh(); 
         DIGIT_2_SetHigh(); 
         DIGIT_3_SetLow(); 
         DIGIT_4_SetHigh(); 
         DecToBin(Digit3); 

     }else if(timercount==4){ 

         DIGIT_1_SetHigh(); 
         DIGIT_2_SetHigh(); 
         DIGIT_3_SetHigh(); 
         DIGIT_4_SetLow(); 
         DecToBin(Digit4); 
         timercount=0; 
     } 
          
     timercount++;  
     return 0; 
} 

// Configuration du timer
void TimerInterruptHandler() {
    if(!fixed){
        double x = (GetValue()-502.5);
        double y = (GetValue2()-506.0);
    
    double angle = atan2(y, x);

    // Conversion_de_l_angle_en_degr�s
    double degree = angle * RAD_TO_DEG;

    // Assure_que_l_angle_est_dans_l_intervalle_[0, 360]
            angle360 = fmod(degree, 360.0); 

    // Assure_que_l_angle_est_positif
    nombre = (int)angle360-(int)angle_ref;
    
        if ( nombre < 0) {
         nombre += 360.0;
    }
}
    AfficherNombre();
}
    
// Configuration du bouton poussoir
void ButtonInterruptHandler(){
    if(FIX_VALUE_GetValue() == 1){
        fixed = !fixed;
     }
    if(ORIGINE_GetValue() == 1){
        angle_ref=angle360;
    }
}
  
// Valeur obtenue sur l'axe des X de l'ADXL335
void GetValue()
{
    ADC1_ChannelSelect(XOUT);
    ADC1_Enable();
    ADC1_SoftwareTriggerEnable();
    for(int i=0; i<1000; i++)
    {
    }
	ADC1_SoftwareTriggerDisable();
    while(!ADC1_IsConversionComplete(XOUT));
    nombre = ADC1_ConversionResultGet(XOUT);
    ADC1_Disable();
}

// Valeur obtenue sur l'axe des Y de l'ADXL335
void GetValue2()
{
    ADC1_ChannelSelect(YOUT);
    ADC1_Enable();
    ADC1_SoftwareTriggerEnable();
    for(int i=0; i<1000; i++)
    {
    }
	ADC1_SoftwareTriggerDisable();
    while(!ADC1_IsConversionComplete(YOUT));
    nombre = ADC1_ConversionResultGet(YOUT);
    ADC1_Disable();
}

//Fonction main
int main(void)
{
    // initialize the device
 

    // initialize the device 

    SYSTEM_Initialize(); 

    ADC1_Initialize ();
    
    TMR1_Initialize();
    TMR1_Start();

    INTERRUPT_GlobalEnable();
    
    ADC1_Enable();
    ADC1_Disable();
    TMR1_SetInterruptHandler(TimerInterruptHandler);
    CN_SetInterruptHandler(ButtonInterruptHandler);
   
    while (1)
    {
        // Add your application code 
    
    }

    return 1;
}
/**
 End of File
*/

