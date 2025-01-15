# STM32
## Test001
	LD2를 flash
	B1으로 껐다 켰다 (EXTERNAL INTTERUPT 사용)
	외부 BUTTON 하나로 FLASH 타이밍 조절

## Test002-Timer
	tim2로 LD2 flash
	tim3로 외부 LED flash
	B1으로 LD2 FLASH 타이밍 조절

## Test003-UART
	ProgramStart 함수 만들어서 library 만들기
	UART로 숫자 입력받아서 출력하기 (PUTTY에 입력하면 출력되도록 설정)

## Test003-UART-DMA
	DMA로 문자열 입력받아서 문자열 출력

## Test004-ADC
	polling 방식 ADC로 컨트롤러 값 putty에 출력

## Test004-ADC-INTR
	interrupt 방식 ADC로 컨트롤러 값 putty에 출력

## Test004-ADC-DMA
	DMA 방식 ADC로 컨트롤러 값 putty에 출력

## Test005-LCD
	LCD에 HELLO(윗 줄) WORLD(아랫 줄) 출력
	LCD library 만들기

## Test005-ADC-LCD
	DMA 방식 ADC로 컨트롤러 값 LCD에 출력

## Test006-uSonic
	초음파 측정기 사용
	timeout까지 만들기
