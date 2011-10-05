
#define CAN_NUM_HOH 23
//#define CAN_NUM_BAUDRATES 1

#define CAN_A 0
#define CAN_C 1
#define CAN_D 2
#define CAN_NUM_CONTROLLERS 3
//#define CAN_B 3


/** Enable Development Error Trace. */
#define CAN_DEV_ERROR_DETECT          STD_ON
/** Build version info API. */
#define CAN_VERSION_INFO_API          STD_ON
/** Not supported. */
#define CAN_MULTIPLEXED_TRANSMISSION  STD_OFF
/** Not supported. */
#define CAN_WAKEUP_SUPPORT            STD_OFF
/** Not supported. */
#define CAN_HW_TRANSMIT_CANCELLATION   STD_OFF
/** Not supported. */
#define CAN_TIMEOUT_DURATION          100

/** Bus off handling. Polling not supported */
#define CAN_BUSOFF_PROCESSING         INTERRUPT  // INTERRUPT/POLLING
/** Not supported. */
//#define CAN_CONTROLLER_ACTIVATION     OFF
/** Rx handling. Polling not supported. */
#define CAN_RX_PROCESSING             INTERRUPT
/** Tx handling. Polling not supported. */
#define CAN_TX_PROCESSING             INTERRUPT
/** Wakeup handling. Polling not supported. */
#define CAN_WAKEUP_PROCESSING         INTERRUPT
