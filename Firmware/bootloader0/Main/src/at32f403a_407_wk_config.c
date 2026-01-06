/* add user code begin Header */
/**
  **************************************************************************
  * @file     at32f403a_407_wk_config.c
  * @brief    work bench config program
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */
  /* add user code end Header */

#include "at32f403a_407_wk_config.h"

/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */
#include "UartDrv.h"
#include "Consts.h"
/* add user code end private includes */

/* private typedef -----------------------------------------------------------*/
/* add user code begin private typedef */

/* add user code end private typedef */

/* private define ------------------------------------------------------------*/
/* add user code begin private define */

/* add user code end private define */

/* private macro -------------------------------------------------------------*/
/* add user code begin private macro */

/* add user code end private macro */

/* private variables ---------------------------------------------------------*/
/* add user code begin private variables */

/* add user code end private variables */

/* private function prototypes --------------------------------------------*/
/* add user code begin function prototypes */

/* add user code end function prototypes */

/* private user code ---------------------------------------------------------*/
/* add user code begin 0 */

/* add user code end 0 */

/**
  * @brief  system clock config program
  * @note   the system clock is configured as follow:
  *         system clock (sclk)   = hick / 12 * pll_mult
  *         system clock source   = HICK_VALUE
  *         - sclk                = 240000000
  *         - ahbdiv              = 1
  *         - ahbclk              = 240000000
  *         - apb1div             = 2
  *         - apb1clk             = 120000000
  *         - apb2div             = 2
  *         - apb2clk             = 120000000
  *         - pll_mult            = 60
  *         - pll_range           = GT72MHZ (greater than 72 mhz)
  * @param  none
  * @retval none
  */
void wk_system_clock_config(void) {
  /* reset crm */
  crm_reset();

  /* enable lick */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_LICK, TRUE);

  /* wait till lick is ready */
  while (crm_flag_get(CRM_LICK_STABLE_FLAG) != SET) {
  }

  /* enable hick */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_HICK, TRUE);

  /* wait till hick is ready */
  while (crm_flag_get(CRM_HICK_STABLE_FLAG) != SET) {
  }

  /* config pll clock resource */
  crm_pll_config(CRM_PLL_SOURCE_HICK, CRM_PLL_MULT_60, CRM_PLL_OUTPUT_RANGE_GT72MHZ);

  /* enable pll */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);

  /* wait till pll is ready */
  while (crm_flag_get(CRM_PLL_STABLE_FLAG) != SET) {
  }

  /* config ahbclk */
  crm_ahb_div_set(CRM_AHB_DIV_1);

  /* config apb2clk, the maximum frequency of APB2 clock is 120 MHz  */
  crm_apb2_div_set(CRM_APB2_DIV_2);

  /* config apb1clk, the maximum frequency of APB1 clock is 120 MHz  */
  crm_apb1_div_set(CRM_APB1_DIV_2);

  /* enable auto step mode */
  crm_auto_step_mode_enable(TRUE);

  /* select pll as system clock source */
  crm_sysclk_switch(CRM_SCLK_PLL);

  /* wait till pll is used as system clock source */
  while (crm_sysclk_switch_status_get() != CRM_SCLK_PLL) {
  }

  /* disable auto step mode */
  crm_auto_step_mode_enable(FALSE);

  /* update system_core_clock global variable */
  system_core_clock_update();
}

/**
  * @brief  config periph clock
  * @param  none
  * @retval none
  */
void wk_periph_clock_config(void) {
  /* enable dma1 periph clock */
  crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);

  /* enable crc periph clock */
  crm_periph_clock_enable(CRM_CRC_PERIPH_CLOCK, TRUE);

  /* enable iomux periph clock */
  crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);

  /* enable gpioa periph clock */
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

  /* enable gpiob periph clock */
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);

  /* enable gpiod periph clock */
  crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);

  /* enable adc1 periph clock */
  crm_periph_clock_enable(CRM_ADC1_PERIPH_CLOCK, TRUE);

  /* enable spi1 periph clock */
  crm_periph_clock_enable(CRM_SPI1_PERIPH_CLOCK, TRUE);

  /* enable usart1 periph clock */
  crm_periph_clock_enable(CRM_USART1_PERIPH_CLOCK, TRUE);

  /* enable usart6 periph clock */
  crm_periph_clock_enable(CRM_USART6_PERIPH_CLOCK, TRUE);

  /* enable i2c1 periph clock */
  crm_periph_clock_enable(CRM_I2C1_PERIPH_CLOCK, TRUE);

  /* enable bpr periph clock */
  crm_periph_clock_enable(CRM_BPR_PERIPH_CLOCK, TRUE);

  /* enable pwc periph clock */
  crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE);

  /* enable tmr1 periph clock */
  crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, TRUE);
}

/**
  * @brief  nvic config
  * @param  none
  * @retval none
  */
void wk_nvic_config(void) {
  //nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  // NVIC_SetPriority(MemoryManagement_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  // NVIC_SetPriority(BusFault_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  // NVIC_SetPriority(UsageFault_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  // NVIC_SetPriority(SVCall_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  // NVIC_SetPriority(DebugMonitor_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  // NVIC_SetPriority(PendSV_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  // NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 15, 0));
  nvic_irq_enable(TMR1_BRK_TMR9_IRQn, 0, 0);
}

/**
  * @brief  init i2c1 function.
  * @param  none
  * @retval none
  */
  // void wk_i2c1_init(void) {
  //   /* add user code begin i2c1_init 0 */

  //   /* add user code end i2c1_init 0 */

  //   gpio_init_type gpio_init_struct;

  //   gpio_default_para_init(&gpio_init_struct);

  //   /* add user code begin i2c1_init 1 */

  //   /* add user code end i2c1_init 1 */

  //   /* configure the SCL pin */
  //   gpio_init_struct.gpio_out_type = GPIO_OUTPUT_OPEN_DRAIN;
  //   gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  //   gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  //   gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  //   gpio_init_struct.gpio_pins = GPIO_PINS_6;
  //   gpio_init(GPIOB, &gpio_init_struct);

  //   /* configure the SDA pin */
  //   gpio_init_struct.gpio_out_type = GPIO_OUTPUT_OPEN_DRAIN;
  //   gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  //   gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  //   gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  //   gpio_init_struct.gpio_pins = GPIO_PINS_7;
  //   gpio_init(GPIOB, &gpio_init_struct);

  //   i2c_init(I2C1, I2C_FSMODE_DUTY_2_1, 100000);
  //   i2c_own_address1_set(I2C1, I2C_ADDRESS_MODE_7BIT, 0x0);
  //   i2c_ack_enable(I2C1, TRUE);
  //   i2c_clock_stretch_enable(I2C1, TRUE);
  //   i2c_general_call_enable(I2C1, FALSE);

  //   i2c_dma_enable(I2C1, TRUE);

  //   /**
  //    * Users need to configure I2C1 interrupt functions according to the actual application.
  //    * 1. Call the below function to enable the corresponding I2C1 interrupt.
  //    *     --i2c_interrupt_enable(...)
  //    * 2. Add the user's interrupt handler code into the below function in the at32f403a_407_int.c file.
  //    *     --void I2C1_ERR_IRQHandler(void)
  //    */

  //   i2c_enable(I2C1, TRUE);

  //   /* add user code begin i2c1_init 2 */

  //   /* add user code end i2c1_init 2 */
  // }

  /**
    * @brief  init spi1 function
    * @param  none
    * @retval none
    */
void wk_spi1_init(void) {
  /* add user code begin spi1_init 0 */

  /* add user code end spi1_init 0 */

  gpio_init_type gpio_init_struct;
  spi_init_type spi_init_struct;

  // gpio_default_para_init(&gpio_init_struct);
  // spi_default_para_init(&spi_init_struct);

  /* add user code begin spi1_init 1 */

  /* add user code end spi1_init 1 */

  /* configure the SCK pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_5;
  gpio_init_struct.gpio_pull = GPIO_PULL_DOWN;
  gpio_init(GPIOA, &gpio_init_struct);

  /* configure the MISO pin */
  //gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  //gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_6;
  gpio_init_struct.gpio_pull = GPIO_PULL_UP;
  gpio_init(GPIOA, &gpio_init_struct);

  /* configure the MOSI pin */
  //gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  //gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_7;
  gpio_init_struct.gpio_pull = GPIO_PULL_UP;
  gpio_init(GPIOA, &gpio_init_struct);

  // gpio_pin_remap_config(SPI1_GMUX_0001, TRUE);

  /* configure param */
  spi_init_struct.transmission_mode = SPI_TRANSMIT_FULL_DUPLEX;
  spi_init_struct.master_slave_mode = SPI_MODE_MASTER;
  spi_init_struct.frame_bit_num = SPI_FRAME_8BIT;
  spi_init_struct.first_bit_transmission = SPI_FIRST_BIT_MSB;
  spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_64;

  spi_init_struct.clock_polarity = SPI_CLOCK_POLARITY_LOW;
  spi_init_struct.clock_phase = SPI_CLOCK_PHASE_2EDGE;
  spi_init_struct.cs_mode_selection = SPI_CS_HARDWARE_MODE;
  spi_init(SPI1, &spi_init_struct);

  // spi_i2s_dma_receiver_enable(SPI1, TRUE);

  spi_enable(SPI1, TRUE);
  /* add user code begin spi1_init 2 */

  /* add user code end spi1_init 2 */
}

/**
  * @brief  init dma1 channel1 for "usart1_rx"
  * @param  none
  * @retval none
  */
void wk_dma1_channel1_init(void) {
  /* add user code begin dma1_channel1 0 */

  /* add user code end dma1_channel1 0 */

  dma_init_type dma_init_struct;

  dma_reset(DMA1_CHANNEL1);
  dma_default_para_init(&dma_init_struct);
  dma_init_struct.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
  dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_BYTE;
  dma_init_struct.memory_inc_enable = TRUE;
  dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_BYTE;
  dma_init_struct.peripheral_inc_enable = FALSE;
  dma_init_struct.priority = DMA_PRIORITY_LOW;
  dma_init_struct.loop_mode_enable = TRUE;
  dma_init(DMA1_CHANNEL1, &dma_init_struct);

  /* flexible function enable */
  dma_flexible_config(DMA1, FLEX_CHANNEL1, DMA_FLEXIBLE_UART1_RX);
  /* add user code begin dma1_channel1 1 */

  /* add user code end dma1_channel1 1 */
}

/**
  * @brief  init dma1 channel2 for "i2c1_rx"
  * @param  none
  * @retval none
  */
void wk_dma1_channel2_init(void) {
  /* add user code begin dma1_channel2 0 */

  /* add user code end dma1_channel2 0 */

  dma_init_type dma_init_struct;

  dma_reset(DMA1_CHANNEL2);
  dma_default_para_init(&dma_init_struct);
  dma_init_struct.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
  dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_BYTE;
  dma_init_struct.memory_inc_enable = TRUE;
  dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_BYTE;
  dma_init_struct.peripheral_inc_enable = FALSE;
  dma_init_struct.priority = DMA_PRIORITY_LOW;
  dma_init_struct.loop_mode_enable = FALSE;
  dma_init(DMA1_CHANNEL2, &dma_init_struct);

  /* flexible function enable */
  dma_flexible_config(DMA1, FLEX_CHANNEL2, DMA_FLEXIBLE_I2C1_RX);
  /* add user code begin dma1_channel2 1 */

  /* add user code end dma1_channel2 1 */
}

/**
  * @brief  init dma1 channel3 for "spi1_rx"
  * @param  none
  * @retval none
  */
void wk_dma1_channel3_init(void) {
  /* add user code begin dma1_channel3 0 */

  /* add user code end dma1_channel3 0 */

  dma_init_type dma_init_struct;

  dma_reset(DMA1_CHANNEL3);
  dma_default_para_init(&dma_init_struct);
  dma_init_struct.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
  dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_BYTE;
  dma_init_struct.memory_inc_enable = TRUE;
  dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_BYTE;
  dma_init_struct.peripheral_inc_enable = FALSE;
  dma_init_struct.priority = DMA_PRIORITY_LOW;
  dma_init_struct.loop_mode_enable = TRUE;
  dma_init(DMA1_CHANNEL3, &dma_init_struct);

  /* flexible function enable */
  dma_flexible_config(DMA1, FLEX_CHANNEL3, DMA_FLEXIBLE_SPI1_RX);
  /* add user code begin dma1_channel3 1 */

  /* add user code end dma1_channel3 1 */
}

/**
  * @brief  config dma channel transfer parameter
  * @param  dmax_channely: DMAx_CHANNELy
  * @param  peripheral_base_addr: peripheral address.
  * @param  memory_base_addr: memory address.
  * @param  buffer_size: buffer size.
  * @retval none
  */
void wk_dma_channel_config(dma_channel_type* dmax_channely, uint32_t peripheral_base_addr, uint32_t memory_base_addr, uint16_t buffer_size) {
  /* add user code begin dma_channel_config 0 */

  /* add user code end dma_channel_config 0 */

  dmax_channely->dtcnt = buffer_size;
  dmax_channely->paddr = peripheral_base_addr;
  dmax_channely->maddr = memory_base_addr;

  /* add user code begin dma_channel_config 1 */

  /* add user code end dma_channel_config 1 */
}


