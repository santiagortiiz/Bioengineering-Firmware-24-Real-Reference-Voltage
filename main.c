/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

uint32 ADC_acumulado = 0;
uint16 ADC = 0;
uint16 Vref_positivo;
uint16 ms = 0;
uint8 contador = 0;

void sensar(void);
void mostrar(void);

CY_ISR_PROTO(cronometro);

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    isr_contador_StartEx(cronometro);
    
    
    ADC_Start();
    LCD_Start();
    Contador_Start();
    Seguidor_Start();

    for(;;)
    {
        if (ms%25 == 0) sensar();
        if (ms%500 == 0) mostrar();
    }
}

void sensar(void){
    contador++;
    ADC_StartConvert();
    ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
    ADC_acumulado += ADC_GetResult16();
    
    if (contador==20){
        contador = 0;
        ADC = ADC_acumulado/20;
        Vref_positivo = 4095*1024/ADC; // Se multiplica *1000 para obtener 3 decimales
        ADC_acumulado = 0;
    }
}

void mostrar(void){
    LCD_Position(0,0);
    LCD_PrintString("ADC = ");
    LCD_PrintNumber(ADC);
    
    LCD_Position(1,0);
    LCD_PrintString("Vref+ = ");
    LCD_PrintNumber(Vref_positivo/1000);
    LCD_PutChar('.');
    LCD_PrintNumber(Vref_positivo%1000);
}

CY_ISR(cronometro){  
    ms++;                                                                    
    if (ms == 1000) ms = 0;
}

/* [] END OF FILE */
