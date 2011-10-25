#ifndef CAN_GENERAL_TYPES
#define CAN_GENERAL_TYPES
///todo should this file be included here?
#include "CanIf_Cfg.h"

/// this file shall be developed by the AutoSar BSW integrator, see CAN437

// uint16: if only Standard IDs are used
// uint32: if also Extended IDs are used
typedef uint32 Can_IdType;

// uint16: if more than 255 hw handles
// uint8: else
typedef CANIF_PUBLIC_HANDLE_TYPE_ENUM Can_HwHandleType;

typedef enum {
	CAN_T_STOP = 1, // cannot request mode CAN_UNINIT
	CAN_T_START,
	CAN_T_SLEEP,
	CAN_T_WAKEUP
} Can_StateTransitionType;

typedef enum {
	CANIF_CS_UNINIT = 0,
	CANIF_CS_STOPPED
	CANIF_CS_STARTED,
	CANIF_CS_SLEEP,
} CanIf_ControllerModeType;

typedef enum {
	CAN_OK,
	CAN_NOT_OK,
	CAN_BUSY
// 	CAN_WAKEUP,		// Removed in 3.0
} Can_ReturnType;
 
struct Can_PduType {
	// data ptr
	uint8 		*sdu;
	// the CAN ID, 29 or 11-bit
	Can_IdType 	id;
	// private data for CanIf,just save and use for callback
	PduIdType   swPduHandle;
	// Length, max 8 bytes
	uint8		length;
};

typedef enum {
	/** Transceiver mode NORMAL */
  CANTRCV_TRCVMODE_NORMAL = 0,
  /** Transceiver mode STANDBY */
  CANTRCV_TRCVMODE_STANDBY,
  /** Transceiver mode SLEEP */
  CANTRCV_TRCVMODE_SLEEP
} CanTrcv_TrcvModeType ;

typedef enum {
  /** Due to an error wake up reason was not detected.
   *  This value may only be reported when error was
   *  reported to DEM before. */
	CANTRCV_WU_ERROR = 0,
	/** The transceiver does not support any information
	 *  for the wakeup reason. */
	CANTRCV_WU_NOT_SUPPORTED,
	/** The transceiver has detected, that the network has
	 *  caused the wake up of the ECU */
	CANTRCV_WU_BY_BUS,
	/** The transceiver has detected a wake-up event at
	 *  one of the transceiver's pins (not at the CAN bus). */
	CANTRCV_WU_BY_PIN,
	/** The transceiver detected, that the network has woken
	 *  the ECU via a request to NORMAL mode */
	CANTRCV_WU_INTERNALLY,
	/** The transceiver has detected, that the "wake up"
	 *  is due to an ECU reset */
	CANTRCV_WU_RESET,
	/** The transceiver has detected, that the "wake up"
	 *  is due to an ECU reset after power on. */
	CANTRCV_WU_POWER_ON
} CanTrcv_TrcvWakeupReasonType;

typedef enum {
  /** The notification for wakeup events is enabled
   *  on the addressed network. */
	CANTRCV_WUMODE_ENABLE = 0,
	/** The notification for wakeup events is disabled
	 *  on the addressed network. */
	CANTRCV_WUMODE_DISABLE,
	/** A stored wakeup event is cleared on the addressed network */
	CANTRCV_WUMODE_CLEAR
} CanIf_TrcvWakeupModeType;

#endif // CAN_GENERAL_TYPES