#include <stdint.h>
#include <stm32f10x.h>
#include <stdbool.h>


void delay_us(uint32_t us){
    __asm volatile(
        "push {r0}\r\n"
        "mov R0, %0\r\n"      //val = (9 * us) for 72Mhz
        "_loop:\r\n"    //approx. 8ticks/iteration
	        "cmp R0, #0\r\n" //1
	        "beq _exit\r\n"      		//1 or 1+P (when condition is True)
	        "sub R0, R0, #1\r\n" 	//1
	        "nop\r\n" 				//1 allignment
	        "b _loop\r\n" 			//1+P (pipeline refill) ~4 cycle
        "_exit:\r\n"
	        "pop {r0}\r\n"
        :: "r"(9*us) // For 72 Mhz
    );
}


int main(void) {
    // int i = 0;
    // int mask = 8; // 8 = 0b10000 = 0x8 = (1 << 4)
    // i = i | mask; // i |= mask;


    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; // 0b10000=0x10
    GPIOC->CRH &= ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13); // GPIOC->CRH[23:20]=0000
    GPIOC->CRH |= GPIO_CRH_MODE13_0; // GPIOC->CRH[23:20]=0001

    while(1){
        GPIOC->ODR &= ~GPIO_ODR_ODR13;
        delay_us(1000000);
        GPIOC->ODR |= GPIO_ODR_ODR13;
        delay_us(1000000);
    }




return 0;
}
