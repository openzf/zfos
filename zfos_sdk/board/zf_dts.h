#ifndef __ZF_DTS_H_
#define __ZF_DTS_H_

#include "board.h"

// 通过在编译器设置宏
#if USE_BOARD_H743_CORE
#include "./STM32_HAL/stm32h743_core/zf_hal_board_dts.h"
#endif


#if USE_BOARD_H750_CORE
#include "./STM32_HAL/stm32h750_core/zf_hal_board_dts.h"
#endif

#endif

