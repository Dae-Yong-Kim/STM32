/*
 * myLib.c
 *
 *  Created on: Nov 15, 2024
 *      Author: user
 */
#include "main.h"
#include "C:\Users\user\STM32Cube\Repository\STM32Cube_FW_F4_V1.28.1\Drivers\STM32F4xx_HAL_Driver\Inc\stm32f4xx_hal_i2c.h" //If you want to use it universally, You have to include "Absolute Path"
extern UART_HandleTypeDef huart2;
I2C_HandleTypeDef *hi2c = NULL;

// int __io_getchar(void) {}
int __io_putchar(int ch) {
	HAL_UART_Transmit(&huart2, &ch, 1, 10); // size(byte), timeout(ms)
	return ch;
}

void StandBy() {
	while(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin)) {}
}

void ProgramStart(char* str) {
	//printf("\033[2J\033[0;0H");
	cls();
	Cursor(0, 0);
	printf("Program Start - %s\r\n", str);
	printf("Press Blue-button(B1) to Start...\r\n");
	StandBy();
}

void cls() {
	printf("\033[2J");
}

void Cursor(int x, int y) {
	char buf[20];
	printf("\033[%d;%dH", y, x);
	//sprintf(buf, "\033[%d;%dH", y, x);
	//puts(buf);
	//printf("%s", buf);
}

int i2c_init(I2C_HandleTypeDef *p) {
	hi2c = p;
}

int i2c_scan() {
	if(hi2c == NULL) return;
	for(int addr = 0; addr < 128; addr++) {
		if(HAL_I2C_IsDeviceReady(hi2c, addr, 1, 10) == HAL_OK) {
			printf("  %02x", addr);
		} else {
			printf("  . ");
		}
		if((addr + 1) % 16 == 0) {
			printf("\r\n");
		}
	}
}
