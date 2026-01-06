#include "CANopenTask.h"
#include "Tracing.h"

#define log_printf(macropar_message, ...) \
  hal_print_trace(macropar_message, ##__VA_ARGS__)

/* default values for CO_CANopenInit() */
#define NMT_CONTROL \
  (CO_NMT_ERR_ON_ERR_REG | CO_ERR_REG_GENERIC_ERR | CO_ERR_REG_COMMUNICATION)
#define FIRST_HB_TIME 500
#define SDO_SRV_TIMEOUT_TIME 1000
#define SDO_CLI_TIMEOUT_TIME 500
#define SDO_CLI_BLOCK false
#define OD_STATUS_BITS NULL

/* Global variables and objects */
CO_t *CO = NULL; /* CANopen object */
uint8_t LED_red, LED_green;

void CANopenTask(void *parameters) {
  CO_ReturnError_t err;
  CO_NMT_reset_cmd_t reset = CO_RESET_NOT;
  uint32_t heapMemoryUsed;
  void *CANptr = NULL;        /* CAN module address */
  uint8_t pendingNodeId = 0x10; /* read from dip switches or nonvolatile memory,   configurable by LSS slave */
  uint8_t activeNodeId = 0x10; /* Copied from CO_pendingNodeId in the communication reset section */
  uint16_t pendingBitRate = 125; /* 125 kbps to match SKOV devices (configurable
                                    via LSS slave or read from nonvolatile memory) */

/* Storage disabled - OD doesn't have 0x1010/0x1011 objects
#if (CO_CONFIG_STORAGE) & CO_CONFIG_STORAGE_ENABLE
  CO_storage_t storage;
  CO_storage_entry_t storageEntries[] = {
      {.addr = &OD_PERSIST_COMM,
       .len = sizeof(OD_PERSIST_COMM),
       .subIndexOD = 2,
       .attr = CO_storage_cmd | CO_storage_restore,
       .addrNV = NULL}};
  uint8_t storageEntriesCount =
      sizeof(storageEntries) / sizeof(storageEntries[0]);
  uint32_t storageInitError = 0;
#endif
*/

  /* Configure microcontroller. */

  /* Allocate memory */
  CO_config_t *config_ptr = NULL;

  CO = CO_new(config_ptr, &heapMemoryUsed);
  if (CO == NULL) {
    log_printf("Error: Can't allocate memory\n");
    for (;;) {
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
  } else {
    log_printf("Allocated %u bytes for CANopen objects\n", heapMemoryUsed);
  }

/* Storage disabled - OD doesn't have 0x1010/0x1011 objects
#if (CO_CONFIG_STORAGE) & CO_CONFIG_STORAGE_ENABLE
  err = CO_storageBlank_init(
      &storage, CO->CANmodule, OD_ENTRY_H1010_storeParameters,
      OD_ENTRY_H1011_restoreDefaultParameters, storageEntries,
      storageEntriesCount, &storageInitError);

  if (err != CO_ERROR_NO && err != CO_ERROR_DATA_CORRUPT) {
    log_printf("Error: Storage %d\n", storageInitError);
    for (;;) {
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }
#endif
*/

  while (reset != CO_RESET_APP) {
    /* CANopen communication reset - initialize CANopen objects
     * *******************/
    log_printf("CANopenNode - Reset communication...\n");

    /* Wait rt_thread. */
    CO->CANmodule->CANnormal = false;

    /* Enter CAN configuration. */
    CO_CANsetConfigurationMode((void *)&CANptr);
    CO_CANmodule_disable(CO->CANmodule);

    /* initialize CANopen */
    err = CO_CANinit(CO, CANptr, pendingBitRate);
    if (err != CO_ERROR_NO) {
      log_printf("Error: CAN initialization failed: %d\n", err);
      for (;;) {
        vTaskDelay(pdMS_TO_TICKS(1000));
      }
    }

    CO_LSS_address_t lssAddress = {
        .identity = {
            .vendorID = OD_RAM.x1018_identityObject.vendorID,
            .productCode = OD_RAM.x1018_identityObject.productCode,
            .revisionNumber = OD_RAM.x1018_identityObject.revisionNumber,
            .serialNumber = OD_RAM.x1018_identityObject.serialNumber}};
            // .vendorID = OD_PERSIST_COMM.x1018_identity.vendor_ID,
            // .productCode = OD_PERSIST_COMM.x1018_identity.productCode,
            // .revisionNumber = OD_PERSIST_COMM.x1018_identity.revisionNumber,
            // .serialNumber = OD_PERSIST_COMM.x1018_identity.serialNumber}};
    err = CO_LSSinit(CO, &lssAddress, &pendingNodeId, &pendingBitRate);
    if (err != CO_ERROR_NO) {
      log_printf("Error: LSS slave initialization failed: %d\n", err);
      for (;;) {
        vTaskDelay(pdMS_TO_TICKS(1000));
      }
    }

    activeNodeId = pendingNodeId;
    uint32_t errInfo = 0;

    err = CO_CANopenInit(CO,                   /* CANopen object */
                         NULL,                 /* alternate NMT */
                         NULL,                 /* alternate em */
                         OD,                   /* Object dictionary */
                         OD_STATUS_BITS,       /* Optional OD_statusBits */
                         NMT_CONTROL,          /* CO_NMT_control_t */
                         FIRST_HB_TIME,        /* firstHBTime_ms */
                         SDO_SRV_TIMEOUT_TIME, /* SDOserverTimeoutTime_ms */
                         SDO_CLI_TIMEOUT_TIME, /* SDOclientTimeoutTime_ms */
                         SDO_CLI_BLOCK,        /* SDOclientBlockTransfer */
                         activeNodeId, &errInfo);
    if (err != CO_ERROR_NO && err != CO_ERROR_NODE_ID_UNCONFIGURED_LSS) {
      if (err == CO_ERROR_OD_PARAMETERS) {
        log_printf("Error: Object Dictionary entry 0x%X\n", errInfo);
      } else {
        log_printf("Error: CANopen initialization failed: %d\n", err);
      }
      for (;;) {
        vTaskDelay(pdMS_TO_TICKS(1000));
      }
    }

    err = CO_CANopenInitPDO(CO, CO->em, OD, activeNodeId, &errInfo);
    if (err != CO_ERROR_NO && err != CO_ERROR_NODE_ID_UNCONFIGURED_LSS) {
      if (err == CO_ERROR_OD_PARAMETERS) {
        log_printf("Error: Object Dictionary entry 0x%X\n", errInfo);
      } else {
        log_printf("Error: PDO initialization failed: %d\n", err);
      }
      for (;;) {
        vTaskDelay(pdMS_TO_TICKS(1000));
      }
    }

    /* Configure Timer interrupt function for execution every 1 millisecond */

    /* Configure CAN transmit and receive interrupt */

    /* Configure CANopen callbacks, etc */
    if (!CO->nodeIdUnconfigured) {
      /* Storage disabled - OD doesn't have 0x1010/0x1011 objects
#if (CO_CONFIG_STORAGE) & CO_CONFIG_STORAGE_ENABLE
      if (storageInitError != 0) {
        CO_errorReport(CO->em, CO_EM_NON_VOLATILE_MEMORY, CO_EMC_HARDWARE,
                       storageInitError);
      }
#endif
      */
    } else {
      log_printf("CANopenNode - Node-id not initialized\n");
    }

    /* start CAN */
    CO_CANsetNormalMode(CO->CANmodule);

    reset = CO_RESET_NOT;

    log_printf("CANopenNode - Running...\n");

    while (reset == CO_RESET_NOT) {
      /* loop for normal program execution
       * ******************************************/
      /* get time difference since last function call */
      uint32_t timeDifference_us = 500;

      /* CANopen process */
      reset = CO_process(CO, false, timeDifference_us, NULL);
      LED_red = CO_LED_RED(CO->LEDs, CO_LED_CANopen);
      LED_green = CO_LED_GREEN(CO->LEDs, CO_LED_CANopen);

      /* Nonblocking application code may go here. */

      /* Process automatic storage */

      /* optional sleep for short time */
    }
  }

  /* program exit
   * ***************************************************************/
  /* stop threads */

  /* delete objects from memory */
  CO_CANsetConfigurationMode((void *)&CANptr);
  CO_delete(CO);

  log_printf("CANopenNode finished\n");

  /* reset */
  for (;;) {
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}



/* CAN interrupt function executes on received CAN message ********************/
void /* interrupt */
CO_CAN1InterruptHandler(void) {

  /* clear interrupt flag */
}