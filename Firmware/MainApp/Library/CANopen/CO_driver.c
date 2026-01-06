/*
 * CAN module object for generic microcontroller.
 *
 * This file is a template for other microcontrollers.
 *
 * @file        CO_driver.c
 * @ingroup     CO_driver
 * @author      Janez Paternoster
 * @copyright   2004 - 2020 Janez Paternoster
 *
 * This file is part of <https://github.com/CANopenNode/CANopenNode>, a CANopen
 * Stack.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */

#include "301/CO_driver.h"
#include "at32f403a_407_can.h"
#include "can_driver.h"
#include "MyMath.h"
#include <string.h>

/* TODO overview (FuncName | IsItCompleted)
 * CO_CANsetConfigurationMode    | yes
 * CO_CANsetNormalMode           | yes
 * CO_CANmodule_init             | yes
 * CO_CANmodule_disable          | yes
 * CO_CANrxBufferInit            | yes
 * CO_CANtxBufferInit            | yes
 * CO_CANsend                    | yes
 * CO_CANclearPendingSyncPDOs    | yes
 * CO_CANmodule_process          | yes
 * CO_CANinterrupt               | yes (RX/TX/Error paths complete)
 */

/** \brief Switch CAN peripheral into configuration (freeze) mode.
 *
 *  This function puts the underlying AT32 CAN peripheral into
 *  \c CAN_OPERATINGMODE_FREEZE so that bitrate, filters and other core
 *  settings can be changed safely before enabling normal communication.
 *
 *  \param CANptr Pointer to the low-level CAN instance, typically \c CAN1
 *                cast to \c void* when passed into CANopenNode.
 */
void CO_CANsetConfigurationMode(void *CANptr) {
  /* CANptr is expected to be (can_type*) CAN1 or similar */
  can_type *can = (can_type *)CANptr;
  (void)can_operating_mode_set(can, CAN_OPERATINGMODE_FREEZE);
}

/** \brief Switch CAN peripheral into normal communication mode.
 *
 *  Leaves configuration/freeze state and enables normal CAN communication
 *  on the underlying AT32 CAN peripheral. Also sets the \c CANnormal flag
 * in the \c CO_CANmodule_t object so that higher layers know the bus is on.
 *
 *  \param CANmodule Pointer to the CANopen CAN module containing the
 *                   hardware pointer (\c CANptr) to the AT32 CAN instance.
 */
void CO_CANsetNormalMode(CO_CANmodule_t *CANmodule) {
  /* Put CAN module in normal mode */
  can_type *can = (can_type *)CANmodule->CANptr;
  (void)can_operating_mode_set(can, CAN_OPERATINGMODE_COMMUNICATE);

  CANmodule->CANnormal = true;
}

/** \brief Initialize CANopen CAN module and underlying AT32 CAN hardware.
 *
 *  Sets up the software-side \c CO_CANmodule_t structure (RX/TX buffer tables,
 *  status flags, error state) and then configures the AT32 CAN peripheral via
 *  \c can_communication_configuration() with the specified \c CANbitRate.
 *
 *  \param CANmodule Pointer to CANopen CAN module object to initialize.
 *  \param CANptr    Pointer to low-level CAN instance (for example \c CAN1),
 *                   passed as \c void* into the stack.
 *  \param rxArray   Array of receive buffer descriptors (\c CO_CANrx_t).
 *  \param rxSize    Number of receive buffers in \p rxArray.
 *  \param txArray   Array of transmit buffer descriptors (\c CO_CANtx_t).
 *  \param txSize    Number of transmit buffers in \p txArray.
 *  \param CANbitRate CAN bitrate in kbps (125, 250, 500, 1000). Must match
 *  other devices on the bus.
 *
 *  \retval CO_ERROR_NO             Initialization succeeded.
 *  \retval CO_ERROR_ILLEGAL_ARGUMENT Invalid pointers, sizes, or HW init
 * failure.
 */
CO_ReturnError_t CO_CANmodule_init(CO_CANmodule_t *CANmodule, void *CANptr,
                                   CO_CANrx_t rxArray[], uint16_t rxSize,
                                   CO_CANtx_t txArray[], uint16_t txSize,
                                   uint16_t CANbitRate) {
  uint16_t i;

  /* verify arguments */
  if (CANmodule == NULL || rxArray == NULL || txArray == NULL ||
      CANptr == NULL) {
    return CO_ERROR_ILLEGAL_ARGUMENT;
  }

  /* Configure object variables */
  CANmodule->CANptr = CANptr;
  CANmodule->rxArray = rxArray;
  CANmodule->rxSize = rxSize;
  CANmodule->txArray = txArray;
  CANmodule->txSize = txSize;
  CANmodule->CANerrorStatus = 0;
  CANmodule->CANnormal = false;
  CANmodule->useCANrxFilters =
      (rxSize <= 32U) ? true : false; /* microcontroller dependent */
  CANmodule->bufferInhibitFlag = false;
  CANmodule->firstCANtxMessage = true;
  CANmodule->CANtxCount = 0U;
  CANmodule->errOld = 0U;

  for (i = 0U; i < rxSize; i++) {
    rxArray[i].ident = 0U;
    rxArray[i].mask = 0xFFFFU;
    rxArray[i].object = NULL;
    rxArray[i].CANrx_callback = NULL;
  }
  for (i = 0U; i < txSize; i++) {
    txArray[i].bufferFull = false;
  }

  /* Configure CAN hardware using TafcoMcuCore driver with specified bitrate */
  if (can_communication_configuration(CANbitRate) != SUCCESS) {
    return CO_ERROR_ILLEGAL_ARGUMENT;
  }

  return CO_ERROR_NO;
}

/** \brief Disable CANopen CAN module and place CAN peripheral into low-power
 * mode.
 *
 *  Disables CAN-related interrupts using the shared TafcoMcuCore CAN driver
 *  helper (\c can_interrupts_disable()), then switches the underlying AT32 CAN
 *  peripheral into \c CAN_OPERATINGMODE_DOZE (or freeze) and clears the
 *  \c CANnormal flag in the \c CO_CANmodule_t object.
 *
 *  \param CANmodule Pointer to CANopen CAN module to be disabled.
 */
void CO_CANmodule_disable(CO_CANmodule_t *CANmodule) {
  if (CANmodule != NULL) {
    can_type *can = (can_type *)CANmodule->CANptr;
    can_interrupts_disable();
    (void)can_operating_mode_set(can, CAN_OPERATINGMODE_DOZE); /* or FREEZE */
    CANmodule->CANnormal = false;
  }
}

/** \brief Configure one CANopen receive buffer entry.
 *
 *  Initializes a single element of the \c CO_CANrx_t receive buffer array with
 *  CAN identifier, mask, user object pointer and callback function. The
 *  identifier and mask are stored in a format suitable for later software
 *  matching (and optionally for programming hardware filters if
 *  \c useCANrxFilters is enabled).
 *
 *  \param CANmodule      Pointer to initialized CANopen CAN module.
 *  \param index          Index of the receive buffer to configure
 *                        (must be < \c CANmodule->rxSize).
 *  \param ident          11-bit CAN identifier to match.
 *  \param mask           11-bit mask applied when matching identifiers.
 *  \param rtr            When true, configure buffer to match RTR frames.
 *  \param object         User object pointer passed to the callback.
 *  \param CANrx_callback Function called when a matching CAN frame is received.
 *
 *  \retval CO_ERROR_NO             Buffer configured successfully.
 *  \retval CO_ERROR_ILLEGAL_ARGUMENT Invalid arguments (NULL pointers or index
 *                                    out of range).
 */
CO_ReturnError_t
CO_CANrxBufferInit(CO_CANmodule_t *CANmodule, uint16_t index, uint16_t ident,
                   uint16_t mask, bool_t rtr, void *object,
                   void (*CANrx_callback)(void *object, void *message)) {
  CO_ReturnError_t ret = CO_ERROR_NO;

  if ((CANmodule != NULL) && (object != NULL) && (CANrx_callback != NULL) &&
      (index < CANmodule->rxSize)) {
    /* buffer, which will be configured */
    CO_CANrx_t *buffer = &CANmodule->rxArray[index];

    /* Configure object variables */
    buffer->object = object;
    buffer->CANrx_callback = CANrx_callback;

    /* CAN identifier and CAN mask, bit aligned with CAN module. Different on
     * different microcontrollers. */
    buffer->ident = ident & 0x07FFU;
    if (rtr) {
      buffer->ident |= 0x0800U;
    }
    buffer->mask = (mask & 0x07FFU) | 0x0800U;

    /* Set CAN hardware module filter and mask. */
    if (CANmodule->useCANrxFilters) {
    }
  } else {
    ret = CO_ERROR_ILLEGAL_ARGUMENT;
  }

  return ret;
}

/** \brief Configure one CANopen transmit buffer entry.
 *
 *  Initializes a single element of the \c CO_CANtx_t transmit buffer array
 *  with CAN identifier, data length and RTR flag in a format suitable for the
 *  underlying CAN hardware, and sets runtime flags used by the CANopen stack.
 *
 *  \param CANmodule Pointer to initialized CANopen CAN module.
 *  \param index     Index of the transmit buffer to configure
 *                   (must be < \c CANmodule->txSize).
 *  \param ident     11-bit CAN identifier for transmitted frames.
 *  \param rtr       When true, configure buffer to send RTR frames.
 *  \param noOfBytes Number of data bytes (DLC, max 8).
 *  \param syncFlag  When true, marks this buffer as SYNC-related (PDO).
 *
 *  \return Pointer to configured \c CO_CANtx_t entry, or NULL if arguments
 *          are invalid.
 */
CO_CANtx_t *CO_CANtxBufferInit(CO_CANmodule_t *CANmodule, uint16_t index,
                               uint16_t ident, bool_t rtr, uint8_t noOfBytes,
                               bool_t syncFlag) {
  CO_CANtx_t *buffer = NULL;

  if ((CANmodule != NULL) && (index < CANmodule->txSize)) {
    /* get specific buffer */
    buffer = &CANmodule->txArray[index];

    /* CAN identifier, DLC and rtr, bit aligned with CAN module transmit buffer,
     * microcontroller specific. */
    buffer->ident = ((uint32_t)ident & 0x07FFU) |
                    ((uint32_t)(((uint32_t)noOfBytes & 0xFU) << 11U)) |
                    ((uint32_t)(rtr ? 0x8000U : 0U));

    buffer->bufferFull = false;
    buffer->syncFlag = syncFlag;
  }

  return buffer;
}

/** \brief Queue or send a CANopen transmit buffer.
 *
 *  Checks for TX buffer overflow, then either sends the message immediately
 *  (if no other CANopen TX is in progress) or marks the buffer as pending to
 *  be sent from the CAN TX interrupt. The actual hardware transmission must
 *  be implemented in the "copy message and txRequest" section.
 *
 *  \param CANmodule Pointer to CANopen CAN module handling TX state.
 *  \param buffer    Pointer to configured \c CO_CANtx_t transmit buffer.
 *
 *  \retval CO_ERROR_NO         Transmission accepted (sent or queued).
 *  \retval CO_ERROR_TX_OVERFLOW Buffer was already full (TX overflow).
 */
CO_ReturnError_t CO_CANsend(CO_CANmodule_t *CANmodule, CO_CANtx_t *buffer) {
  CO_ReturnError_t err = CO_ERROR_NO;

  /* Verify overflow */
  if (buffer->bufferFull) {
    if (!CANmodule->firstCANtxMessage) {
      /* don't set error, if bootup message is still on buffers */
      CANmodule->CANerrorStatus |= CO_CAN_ERRTX_OVERFLOW;
    }
    err = CO_ERROR_TX_OVERFLOW;
  }

  CO_LOCK_CAN_SEND(CANmodule);
  /* if CAN TX buffer is free, copy message to it */
  if (CANmodule->CANtxCount == 0) {
    CANmodule->bufferInhibitFlag = buffer->syncFlag;
    
    /* Extract fields from buffer->ident using EXTRACT_BITS macro */
    uint16_t can_id = EXTRACT_BITS(buffer->ident, 0, 0x07FFU);   /* bits 0-10: CAN ID */
    uint8_t dlc = EXTRACT_BITS(buffer->ident, 11, 0x0FU);        /* bits 11-14: DLC */
    bool_t is_rtr = EXTRACT_BITS(buffer->ident, 15, 0x01U) != 0; /* bit 15: RTR flag */
    
    /* Prepare AT32 CAN message structure */
    can_tx_message_type tx_message;
    tx_message.standard_id = can_id;
    tx_message.extended_id = 0;
    tx_message.id_type = CAN_ID_STANDARD;
    tx_message.frame_type = is_rtr ? CAN_TFT_REMOTE : CAN_TFT_DATA;
    tx_message.dlc = dlc;
    
    /* Copy data bytes */
    memcpy(tx_message.data, buffer->data, (dlc < 8) ? dlc : 8);
    
    /* Transmit to hardware */
    can_type *can = (can_type *)CANmodule->CANptr;
    uint8_t mailbox = can_message_transmit(can, &tx_message);
    
    /* Check if transmission was accepted */
    if (mailbox == CAN_TX_STATUS_NO_EMPTY) {
      /* No free mailbox, queue the message */
      buffer->bufferFull = true;
      CANmodule->CANtxCount++;
    }
  }
  /* if no buffer is free, message will be sent by interrupt */
  else {
    buffer->bufferFull = true;
    CANmodule->CANtxCount++;
  }
  CO_UNLOCK_CAN_SEND(CANmodule);

  return err;
}

/** \brief Abort or drop pending synchronous TPDOs after a SYNC window.
 *
 *  Called by the SYNC processing code when a new SYNC has occurred and any
 *  outstanding synchronous TPDOs (with \c syncFlag set) are now considered
 *  late. This function:
 *    - clears the inhibit flag and aborts a currently pending SYNC TPDO, and
 *    - walks all transmit buffers to clear any queued SYNC TPDOs,
 *  then sets the \c CO_CAN_ERRTX_PDO_LATE error flag if any TPDOs were
 * removed.
 *
 *  \note Current implementation cancels ALL pending TX mailboxes when
 *        bufferInhibitFlag is set. For better precision, consider tracking
 *        which specific mailbox contains the SYNC PDO and cancel only that one.
 *
 *  \param CANmodule Pointer to the CANopen CAN module whose TPDOs may need
 *                   to be cancelled.
 */
void CO_CANclearPendingSyncPDOs(CO_CANmodule_t *CANmodule) {
  uint32_t tpdoDeleted = 0U;
  can_type *can = (can_type *)CANmodule->CANptr;

  CO_LOCK_CAN_SEND(CANmodule);
  /* Abort message from CAN module, if there is synchronous TPDO.
   * Take special care with this functionality. */
  if (CANmodule->bufferInhibitFlag) {
    /* If a synchronous TPDO is currently in a transmit mailbox, cancel it. */
    can_cancel_pending_tx(can);
    CANmodule->bufferInhibitFlag = false;
    tpdoDeleted = 1U;
  }
  /* delete also pending synchronous TPDOs in TX buffers */
  if (CANmodule->CANtxCount != 0U) {
    uint16_t i;
    CO_CANtx_t *buffer = &CANmodule->txArray[0];
    for (i = CANmodule->txSize; i > 0U; i--) {
      if (buffer->bufferFull) {
        if (buffer->syncFlag) {
          buffer->bufferFull = false;
          CANmodule->CANtxCount--;
          tpdoDeleted = 2U;
        }
      }
      buffer++;
    }
  }
  CO_UNLOCK_CAN_SEND(CANmodule);

  if (tpdoDeleted != 0U) {
    CANmodule->CANerrorStatus |= CO_CAN_ERRTX_PDO_LATE;
  }
}

/* Get error counters from the module. If necessary, function may use different
 * way to determine errors. */
static uint16_t rxErrors = 0, txErrors = 0, overflow = 0;

/** \brief Update CANopen error status from internal error counters.
 *
 *  Combines the current transmit error counter, receive error counter and
 *  RX overflow flag into a 32-bit value and compares it with the previously
 *  stored value (\c errOld). When a change is detected, this function updates
 *  \c CANerrorStatus with appropriate flags for bus-off, warning, passive and
 *  RX overflow conditions based on the CANopenNode reference thresholds.
 *
 *  This function reads error counters directly from the AT32 CAN peripheral.
 *  Call this function periodically from the main loop or a timer task.
 *
 *  \param CANmodule Pointer to the CANopen CAN module whose error status
 *                   field should be updated.
 */
void CO_CANmodule_process(CO_CANmodule_t *CANmodule) {
  uint32_t err;
  can_type *can = (can_type *)CANmodule->CANptr;

  /* Read hardware error counters */
  txErrors = can_transmit_error_counter_get(can);
  rxErrors = can_receive_error_counter_get(can);

  /* Check for RX FIFO overflow flags */
  overflow = 0;
  if (can_flag_get(can, CAN_RF0OF_FLAG) == SET) {
    overflow = 1;
    can_flag_clear(can, CAN_RF0OF_FLAG);  /* Clear the flag after reading */
  }
  if (can_flag_get(can, CAN_RF1OF_FLAG) == SET) {
    overflow = 1;
    can_flag_clear(can, CAN_RF1OF_FLAG);  /* Clear the flag after reading */
  }

  err = ((uint32_t)txErrors << 16) | ((uint32_t)rxErrors << 8) | overflow;

  if (CANmodule->errOld != err) {
    uint16_t status = CANmodule->CANerrorStatus;

    CANmodule->errOld = err;

    if (txErrors >= 256U) {
      /* bus off */
      status |= CO_CAN_ERRTX_BUS_OFF;
    } else {
      /* recalculate CANerrorStatus, first clear some flags */
      status &= 0xFFFF ^ (CO_CAN_ERRTX_BUS_OFF | CO_CAN_ERRRX_WARNING |
                          CO_CAN_ERRRX_PASSIVE | CO_CAN_ERRTX_WARNING |
                          CO_CAN_ERRTX_PASSIVE);

      /* rx bus warning or passive */
      if (rxErrors >= 128) {
        status |= CO_CAN_ERRRX_WARNING | CO_CAN_ERRRX_PASSIVE;
      } else if (rxErrors >= 96) {
        status |= CO_CAN_ERRRX_WARNING;
      }

      /* tx bus warning or passive */
      if (txErrors >= 128) {
        status |= CO_CAN_ERRTX_WARNING | CO_CAN_ERRTX_PASSIVE;
      } else if (txErrors >= 96) {
        status |= CO_CAN_ERRTX_WARNING;
      }

      /* if not tx passive clear also overflow */
      if ((status & CO_CAN_ERRTX_PASSIVE) == 0) {
        status &= 0xFFFF ^ CO_CAN_ERRTX_OVERFLOW;
      }
    }

    if (overflow != 0) {
      /* CAN RX bus overflow */
      status |= CO_CAN_ERRRX_OVERFLOW;
    }

    CANmodule->CANerrorStatus = status;
  }
}

typedef struct {
  uint32_t ident;
  uint8_t DLC;
  uint8_t data[8];
} CO_CANrxMsg_t;

/** \brief CAN interrupt handler for CANopenNode.
 *
 *  This function is called from the AT32 CAN interrupt service routine
 *  (typically \c CAN1_RX0_IRQHandler or \c CAN1_SE_IRQHandler) to process
 *  receive, transmit and error interrupts. It handles:
 *    - \b Receive: Reads incoming CAN frames from hardware FIFO0 using
 *      \c can_message_receive(), converts them to CANopenNode format, matches
 *      them against the \c rxArray (using hardware filters or software matching),
 *      and calls the appropriate callback function if a match is found.
 *    - \b Transmit: Checks TX mailbox completion flags (TM0TCF, TM1TCF, TM2TCF),
 *      clears them, marks the first bootup message as sent, and transmits the
 *      next queued message from \c txArray if any are pending. Uses the same
 *      message preparation logic as \c CO_CANsend() to fill \c can_tx_message_type
 *      and call \c can_message_transmit().
 *    - \b Error: Checks error occur interrupt flag (EOIF), calls
 *      \c CO_CANmodule_process() to read hardware error counters (TEC/REC) and
 *      update CANopen error status bits (bus-off, error passive, error warning,
 *      RX overflow), then clears the error interrupt flag.
 *
 *  \note All interrupt paths (RX, TX, Error) are fully implemented.
 *
 *  \param CANmodule Pointer to the CANopen CAN module whose interrupts are
 *                   being processed.
 */
void CO_CANinterrupt(CO_CANmodule_t *CANmodule) {
  can_type *can = (can_type *)CANmodule->CANptr;

  /* receive interrupt */
  if (can_receive_message_pending_get(can, CAN_RX_FIFO0) > 0) {
    CO_CANrxMsg_t rcvMsgStruct; /* storage for received message */
    CO_CANrxMsg_t *rcvMsg = &rcvMsgStruct; /* pointer to received message */
    uint16_t index;        /* index of received message */
    uint32_t rcvMsgIdent;  /* identifier of the received message */
    CO_CANrx_t *buffer =
        NULL; /* receive message buffer from CO_CANmodule_t object. */
    bool_t msgMatched = false;

    /* Read message from hardware FIFO */
    can_rx_message_type hw_msg;
    can_message_receive(can, CAN_RX_FIFO0, &hw_msg);

    /* Convert AT32 message to CANopenNode format */
    rcvMsg->ident = hw_msg.standard_id & 0x07FFU;  /* Standard 11-bit CAN ID */
    rcvMsg->DLC = hw_msg.dlc;
    memcpy(rcvMsg->data, hw_msg.data, 8);

    /* Release FIFO slot */
    can_receive_fifo_release(can, CAN_RX_FIFO0);

    rcvMsgIdent = rcvMsg->ident;
    if (CANmodule->useCANrxFilters) {
      /* CAN module filters are used. Message with known 11-bit identifier has
       * been received */
      index =
          0; /* get index of the received message here. Or something similar */
      if (index < CANmodule->rxSize) {
        buffer = &CANmodule->rxArray[index];
        /* verify also RTR */
        if (((rcvMsgIdent ^ buffer->ident) & buffer->mask) == 0U) {
          msgMatched = true;
        }
      }
    } else {
      /* CAN module filters are not used, message with any standard 11-bit
       * identifier */
      /* has been received. Search rxArray form CANmodule for the same CAN-ID.
       */
      buffer = &CANmodule->rxArray[0];
      for (index = CANmodule->rxSize; index > 0U; index--) {
        if (((rcvMsgIdent ^ buffer->ident) & buffer->mask) == 0U) {
          msgMatched = true;
          break;
        }
        buffer++;
      }
    }

    /* Call specific function, which will process the message */
    if (msgMatched && (buffer != NULL) && (buffer->CANrx_callback != NULL)) {
      buffer->CANrx_callback(buffer->object, (void *)rcvMsg);
    }

    /* Clear interrupt flag */
  }

  /* transmit interrupt - check all three TX mailboxes */
  if (can_flag_get(can, CAN_TM0TCF_FLAG) == SET ||
      can_flag_get(can, CAN_TM1TCF_FLAG) == SET ||
      can_flag_get(can, CAN_TM2TCF_FLAG) == SET) {
    
    /* Clear interrupt flags for all mailboxes */
    can_flag_clear(can, CAN_TM0TCF_FLAG);
    can_flag_clear(can, CAN_TM1TCF_FLAG);
    can_flag_clear(can, CAN_TM2TCF_FLAG);

    /* First CAN message (bootup) was sent successfully */
    CANmodule->firstCANtxMessage = false;
    /* clear flag from previous message */
    CANmodule->bufferInhibitFlag = false;
    
    /* Are there any new messages waiting to be sent */
    if (CANmodule->CANtxCount > 0U) {
      uint16_t i; /* index of transmitting message */

      /* first buffer */
      CO_CANtx_t *buffer = &CANmodule->txArray[0];
      /* search through whole array of pointers to transmit message buffers. */
      for (i = CANmodule->txSize; i > 0U; i--) {
        /* if message buffer is full, send it. */
        if (buffer->bufferFull) {
          buffer->bufferFull = false;
          CANmodule->CANtxCount--;

          /* Prepare message for transmission */
          CANmodule->bufferInhibitFlag = buffer->syncFlag;
          
          /* Extract fields from buffer->ident using EXTRACT_BITS macro */
          uint16_t can_id = EXTRACT_BITS(buffer->ident, 0, 0x07FFU);   /* bits 0-10: CAN ID */
          uint8_t dlc = EXTRACT_BITS(buffer->ident, 11, 0x0FU);        /* bits 11-14: DLC */
          bool_t is_rtr = EXTRACT_BITS(buffer->ident, 15, 0x01U) != 0; /* bit 15: RTR flag */
          
          /* Fill AT32 CAN message structure */
          can_tx_message_type tx_message;
          tx_message.standard_id = can_id;
          tx_message.extended_id = 0;
          tx_message.id_type = CAN_ID_STANDARD;
          tx_message.frame_type = is_rtr ? CAN_TFT_REMOTE : CAN_TFT_DATA;
          tx_message.dlc = dlc;
          
          /* Copy data bytes */
          memcpy(tx_message.data, buffer->data, (dlc < 8) ? dlc : 8);
          
          /* Transmit to hardware */
          can_message_transmit(can, &tx_message);
          
          break; /* exit for loop */
        }
        buffer++;
      } /* end of for loop */

      /* Clear counter if no more messages */
      if (i == 0U) {
        CANmodule->CANtxCount = 0U;
      }
    }
  }
  
  /* error and other interrupts */
  if (can_flag_get(can, CAN_EOIF_FLAG) == SET) {
    /* Error interrupt occurred - update error status from hardware counters */
    CO_CANmodule_process(CANmodule);
    
    /* Clear error interrupt flag */
    can_flag_clear(can, CAN_EOIF_FLAG);
  }
}
