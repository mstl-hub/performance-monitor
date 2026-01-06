/* Includes ------------------------------------------------------------------*/
#include "at32f403a_407_wk_config.h"
#include "at32f403a_407_conf_template.h"
#include "wk_system.h"
#include "at32f403a_407_clock.h"
#include "UpTime.h"
#include "UartDrv.h"
#include "mcu_hal.h"
#include "SimpleWD.h"
#include "FlashService.h"
#include "Bootload.h"
#include "CommandHandler.h"
#include "PacketWrapper.h"
#include "Command.h"
#include "ArteryCore/BprDriver/BprDriver.h"
#include "usb_process.h"
#include "BootloadJournal.h"
#include "rtc_module.h"
#include "gpio.h"
#include "PinDefines.h"
#include "ClockCfg.h"
#ifdef LITE_GATEWAY
#include "UartApp.h"
#endif
#include "rtc_and_pwr.h"
#ifdef END_DEVICE_TEMPERATURE_SENSOR
#include "SimpleWD.h"
#endif

extern uint8_t _sboot;
