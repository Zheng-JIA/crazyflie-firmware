#include <stdint.h>
#include <stdlib.h>
#include "stm32fxxx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "deck.h"
#include "system.h"
#include "log.h"
#include "param.h"

static uint8_t led_mode=3;

static void delayms(unsigned int delay)
{
  vTaskDelay(M2T(delay));
}

static void ledFlashTask(){
	systemWaitStart();
	while(1){
		// adding 1ms delay after switching modes, otherwise the cfclient failed to connect to crazyflie.
		digitalWrite(DECK_GPIO_IO2, led_mode);
		delayms(1);
	}
}

static void viconLedDeckInit(DeckInfo *info)
{
  pinMode(DECK_GPIO_IO2, OUTPUT);
  digitalWrite(DECK_GPIO_IO2, HIGH);
  xTaskCreate(ledFlashTask, "ledFash", 2*configMINIMAL_STACK_SIZE, NULL, 0, NULL);	//test priority
}

static const DeckDriver viconleddeck = {
  .vid = 0xBC,
  .pid = 0x04,
  .name = "viconleddeck",

  .usedPeriph = 0,
  .usedGpio = DECK_USING_IO_2,

  .init = viconLedDeckInit,
};

DECK_DRIVER(viconleddeck);

PARAM_GROUP_START(viconled)
PARAM_ADD(PARAM_UINT8, led_mode, &led_mode)
PARAM_GROUP_STOP(viconled)
