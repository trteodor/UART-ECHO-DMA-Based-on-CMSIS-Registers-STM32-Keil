#include "main.h"
#include "stdio.h"
#include <string.h>
#include "contable.h"

#define PRIGROUP_16G_0S ((const uint32_t) 0x03)
#define PRIGROUP_8G_2S ((const uint32_t) 0x04)
#define PRIGROUP_4G_4S ((const uint32_t) 0x05)
#define PRIGROUP_2G_8S ((const uint32_t) 0x06)
#define PRIGROUP_0G_16S ((const uint32_t) 0x07)

#define AnimTable
#ifdef AnimTable
char AnimTableTab[500];
int valueTable=100;
#endif




/*
I'am only showing here how to develop more advanced application with terminal 
I Create it only to improve my programming skills ;) 
*/

//License: hmm BeerWare ;) 

#define LED1_SET     GPIOA->GPIO_ODR_ODR1=1;   //funkcja z argumentami ktore ustawia diode
#define LED1_RESET 	 GPIOA->GPIO_ODR_ODR1=0;
volatile uint32_t SoftTimer;


uint8_t rx_BUF[1655]="ASDFGHH";
UART_DMA_Handle_Td TUART2;  
//@@@@

#pragma   arm section code = "RAMCODE" //some tricks
void TIM1_Cnf()   //timer on 500ms
{	
		RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
		TIM1->PSC = 639;
		TIM1->ARR = 1000; //config on 100ms interrupt
	TIM1->DIER |= TIM_DIER_UIE;
		TIM1->CR1 |= TIM_CR1_CEN;	

				NVIC_EnableIRQ(TIM1_UP_IRQn);
	
}
#pragma arm section

int main(void) 
{
	//Frist config the PLL

	NVIC_SetPriorityGrouping(PRIGROUP_4G_4S);

						uint32_t prio;
				 prio = NVIC_EncodePriority(PRIGROUP_4G_4S, 0, 0);
				 NVIC_SetPriority(SysTick_IRQn, prio);
	
	PLL_Config64MHZ();
	//SysTick
	SysTick_Config(64000/8);
	SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk; // SysTickClk/8
	//Enable the GPIOA and GPIOB port
	//	GPIO_Pin_Cfg(GPIOA,Px5,gpio_mode_output_PP_2MHz);		
	
	//Config UART Pins
	GPIO_Pin_Cfg(GPIOA,Px2, gpio_mode_alternate_PP_10MHz);		
	GPIO_Pin_Cfg(GPIOA,Px3, gpio_mode_alternate_PP_10MHz);		
	
	//Config Struct for UART ECHO with DMA
	TUART2.Instance=USART2;
	TUART2.UART_DMA_RX_CHANNEL=6;
	TUART2.UART_DMA_TX_CHANNEL=7;
	TUART2.NbofRecData=0;
	TUART2.ubNbDataToTransmit=0;
	TUART2.ubReceptionComplete=1;  //This must be 1 initialized
	TUART2.ubTransmissionComplete=1; //This must be 1 initialized
	TUART2.NbDataToReceive=1655;
	TUART2.ECHO_Mode=1;
	
		USARTx_DMA_Config(&TUART2);
		TUART_DMA_Trasmit(&TUART2, (uint8_t*)
													"\033[37m"  //Set White Color
													"\e[1;1H\e[2J" //Clear Screen Command! <3 
													"Hello!\n"
													"Change color\n"
													"\033[35m"  //Set Purple
													"New Color! \n"
													"Change Color\n"
													"\033[36m" //Set Cyan
													"New Kolor!\n"
													"\033[33m \n" //Set Yellow
													"Static Table\n"
													" +=========+=========+============+========+=========+========+ \n"
													" |  Ten    | Program |   Dziala   |  Jako  |  Uart   |  ECHO  | \n"
													" +=========+=========+============+========+=========+========+ \n"
													" | Polecam | Rowniez | Generator  | Tabel  | Asii    |        | \n"
													" +---------+---------+------------+--------+---------+--------+ \n"
													" | Bardzo  | Latwo   | Sie        | w nim  | Tworzy  | Tabele | \n"
													" +---------+---------+------------+--------+---------+--------+ \n"
													" \033[37m " //Set White
													"https://ozh.github.io/ascii-tables/ \n"
													"Dynamic Table: \n"
													"\033[36m"
													);
													
											con_table tbl;
											contable_init(&tbl);

											contable_add_column(&tbl, "col a", contable_string, 10); // Add decimal column
											contable_add_column(&tbl, "col b", contable_string, 16);  // Add string column
											contable_add_column(&tbl, "col c", contable_decimal, 12); // Add decimal column

											contable_print(&tbl, "%d%s%d", "ROFL", "Pieknie", 116);
											contable_print(&tbl, "%d%s%d", "WWOW", "SUPER", 15);
											contable_print(&tbl, "%d%s%d", "DZIALA", "string c", 12);
											contable_destroy(&tbl, true);
													
													
													
		TUART_DMA_Trasmit(&TUART2, (uint8_t*)	
													"\033[37m"
													"\nWprowadz Swoja Wiadomosc (Program Dziala Jako Echo)>" );
 
	  TUART_DMA_Receive(&TUART2, rx_BUF, 1655);   //the receiving buffer address must be indicated and listening must be turned on
			/*This program now saves the received string in the receiving buffer, and after
			detecting the end of the transmission by the IDLE flag, returns the received message back to the sender */
			
	  TIM1_Cnf();
		
	 uint32_t zT_LED=0;
																	//napisalem to tylko dla pokazania funkcji debuggera do video... ten kod jest nie poprawny!
																				//brakuje odpowiedniej inicjalizacji pinow wyjsciowych, wgl piny 2i3 wykorzystuje uart...
	 	 GPIOA->ODR &=((uint32_t)(0xffffff00));  //zeruje dolne bity portu wyjsciowego! 		
			GPIOA->ODR=1;
			uint8_t helper=0;
		 
while(1)
 {
		if(  (SysTime-zT_LED) > 500)  //SysTick Handler and the Variable SysTime Definded in RCC_Config (its software timer now)
	{		
		zT_LED=SysTime;	
		if(helper==0)
		{
			GPIOA->ODR =(GPIOA->ODR&(0xffffff00)) | ((GPIOA->ODR&(0x000000FF)) << 1);	
		}
	 if( ((GPIOA->ODR & 0x000000FF) == 0x80) || helper ) //jesli swieci ostatnia dioda
	 {
			if(helper==0)
			{
				helper=1;
			}
			else{
				helper=0;
			GPIOA->ODR &=((uint32_t)(0xffffff00)); //zeruje
				GPIOA->ODR|=(0x00000001);
			}

	 }
	}
 }
}

void TIM1_UP_IRQHandler(void){
 if (TIM1->SR & TIM_SR_UIF){
TIM1->SR = ~TIM_SR_UIF;
//tooglePIN(GPIOA,Psm5);
	 SoftTimer++;
 }
}


