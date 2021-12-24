#ifndef __ZF_HAL_CONF_H_
#define __ZF_HAL_CONF_H_

#include "board.h"

#if USE_BOARD_H743_CORE
#include "./STM32_HAL/stm32h743_core/zf_hal_board_conf.h"
#endif

#if USE_BOARD_H750_CORE
#include "./STM32_HAL/stm32h750_core/zf_hal_board_conf.h"
#endif

#endif

