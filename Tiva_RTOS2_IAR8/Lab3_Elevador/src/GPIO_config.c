#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "system_TM4C1294.h" 
#include "tm4c1294ncpdt_pt2.h"
uint32_t up=0;
uint32_t down=0;
uint32_t ctl=0;

// includes da biblioteca driverlib
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/pin_map.h"
#include "utils/uartstdio.h"
#include "system_TM4C1294.h" 

void EnableInterrupts(void);
void DisableInterrupts(void);

#define ONE_SECOND						(((volatile uint32_t )	0x04C4B400))
#define GPIO_PORTD 	    	                                ((volatile uint32_t) 0x00000008)

void GPIO_Init(void)
{
//        int32_t GPIO_ALL=0x00000000;		//ZERA A VARIAVEL PARA INICIALIZA��O
//        
//        GPIO_ALL=GPIO_PORTD;
//          
//        //1a. Ativar o clock para a porta setando o bit correspondente no registrador RCGCGPIO
//	SYSCTL_RCGCGPIO_R |= GPIO_ALL;
//	//1b.   ap�s isso verificar no PRGPIO se a porta est� pronta para uso.
//        while((SYSCTL_PRGPIO_R & (GPIO_ALL) ) != (GPIO_ALL) ){};
//        
//        // 2. Limpar o AMSEL para desabilitar a anal�gica
//	GPIO_PORTD_AHB_AMSEL_R = 0x00;
//        
//        // 3. Limpar PCTL para selecionar o GPIO
//	GPIO_PORTD_AHB_PCTL_R = 0x33;           //selecionando o timer no gpio t0ccp0 t1ccp0 
//        
//        // 4. DIR para 0 se for entrada, 1 se for sa�da
//	GPIO_PORTD_AHB_DIR_R = 0x00;
//        
//        // 5. Limpar os bits AFSEL para 0 para selecionar GPIO sem fun��o alternativa	
//	GPIO_PORTD_AHB_AFSEL_R = 0x03; //Seleciona PD0 e PD1
//        
//        // 6. Setar os bits de DEN para habilitar I/O digital	
//	GPIO_PORTD_AHB_DEN_R = 0x03;   //Bit0 e bit1
//        
//        //receita de bolo
//        RCGCTIMER=0x000000FF;
//	while (0x000000FF!=PRTIMER)
//	{}
//        
  
          // Enable the GPIO Peripheral used by the TIMER.
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD));
  
  // Enable TIMER0
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0));
  
  // Enable TIMER1
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER1));
  
  // Enable TIMER2
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER2));
  
   // Configure GPIO Pins for TIMER mode.
  GPIOPinConfigure(GPIO_PD0_T0CCP0);
  GPIOPinConfigure(GPIO_PD0_T0CCP0);
  GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1);
  
  TimerConfigure(TIMER0_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_TIME));�
� TimerConfigure(TIMER0_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_CAP_TIME));�
� TimerControlEvent(TIMER0_BASE, TIMER_A, TIMER_EVENT_POS_EDGE);�
  TimerControlEvent(TIMER0_BASE, TIMER_B, TIMER_EVENT_NEG_EDGE);
  TimerEnable(TIMER0_BASE, TIMER_BOTH);
  
  TimerLoadSet(TIMER0_BASE, TIMER_A, 0);
  TimerLoadSet(TIMER0_BASE, TIMER_B, 0);
  //TimerLoadSet(uint32_t ui32Base, uint32_t ui32Timer,uint32_t ui32Value); // "Setta" um valor no timer, utilizado para zerar 
  //TimerValueGet(uint32_t ui32Base, uint32_t ui32Timer); // Pega o valor do timer
  
  
//        //1. desabilitando os timers antes das configura��es
//        GPTMCTL_TIMER_0=0x00;
//	GPTMCTL_TIMER_1=0x00;
//	GPTMCTL_TIMER_2=0x00;
//        //2. confiugrando o tamanho do timer
//        GPTMCFG_TIMER_0=0x4;		// SETANDO O CONTADOR PARA 16BITS
//	GPTMCFG_TIMER_1=0x4;
//	GPTMCFG_TIMER_2=0x0;		// SETANDO O CONTADOR PARA 32BITS
//        
//        //3.configurando para edgemode
//        GPTMTAMR_TIMER_0=0x4+0x3+0x8;  //	TnCMR =0x1; TnMR=0x3; TnCDIR=0X1 cont up	
//	GPTMTAMR_TIMER_1=0x4+0x3+0x8;
//	GPTMTAMR_TIMER_2=0x1;		// CONFIGURANDO PARA ONE SHOOT
//        
//        //4.configurando CTL taevent
//        
//        GPTMCTL_TIMER_0=0x00;           //positive edge
//	GPTMCTL_TIMER_1=0x04;           //negative edge
//        
//        //5. no prescaler
//        
//        //6.
//        GPTMTAILR_TIMER_0=0x0000;
//	GPTMTAILR_TIMER_1=0x0000;
//        
//        //7. interrupt
//        GPTMICR_TIMER_0=0x4;            //setando CaEIM
//	GPTMICR_TIMER_1=0x4;            //setando CaEIM
//	GPTMICR_TIMER_2=0x1;
//        
//        GPTMTAILR_TIMER_2=20*ONE_SECOND;
//        GPTMICR_TIMER_2=0x1;
//        GPTMIMR_TIMER_2=0x1;
//        
//        NVIC_PRI8=0x40000000; // TIMER 0 
//        NVIC_PRI9=0x40000040; // TIMER 1 E 2
//        
//        NVIC_EN1=0x000000A8; //timer 0,1 e 2
//        
//        EnableInterrupts();
//}

TimerIntRegister(TIMER0_BASE, TIMER_A, void (*TIMERA_Handler)(void)); // HANDLER
  
TimerIntRegister(TIMER0_BASE, TIMER_B, void (*TIMERB_Handler)(void)); // HANDLER  
  
void TIMERA_Handler(void)
{
up=TimerValueGet(TIMER0_BASE, TIMER_A);
ctl=1;}

void TIMERB_Handler(void)
{
down=TimerValueGet(TIMER0_BASE, TIMER_B);
ctl=1;}

//
//void TIMER2A_Handler(void)
//{GPTMICR_TIMER_2=0x1;}
//
//void ON_TIMER_0(void)
//{GPTMCTL_TIMER_0|=0x1;}
//
//void ON_TIMER_1(void)
//{GPTMCTL_TIMER_1|=0x1;}
//
//void OFF_TIMER_0(void)
//{GPTMCTL_TIMER_0&=0xFFFE;}
//
//void OFF_TIMER_1(void)
//{GPTMCTL_TIMER_1&=0xFFFE;}
}
