#include "gpio.h"
#include "buttonHandler.h"

void Callback_SINGLE_CLICK_btn1Handler(void *btn)
{
  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
}

void Callback_SINGLE_CLICK_btn2Handler(void *btn)
{
  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
}
