/** @addtogroup CanIf CAN Interface
 *  @{ */

/** @file CanIf_Types.h
 *  Definitions of configuration parameters for CAN Interface.
 */

#ifndef CANIF_TYPES_H_
#define CANIF_TYPES_H_

#include "ComStack_Types.h"
#include "Can_GeneralTypes.h"

typedef enum {
	/** UNINIT mode. Default mode of the CAN driver and all
	 *  CAN controllers connected to one CAN network after
	 *  power on. */
	CANIF_CS_UNINIT = 0,
	                           
	/**  STOPPED mode. At least one of all CAN controllers
	 *   connected to one CAN network are halted and does
	 *   not operate on the bus. */
	CANIF_CS_STOPPED,	                           
	                           
	/** STARTED mode. All CAN controllers connected to
	 *  one CAN network are started by the CAN driver and
	 *  in full-operational mode. */
	CANIF_CS_STARTED,
	
	/** SLEEP mode. At least one of all CAN controllers
	 *  connected to one CAN network are set into the
	 *  SLEEP mode and can be woken up by request of the
	 *  CAN driver or by a network event (must be supported
	 *  by CAN hardware) */
	CANIF_CS_SLEEP
} CanIf_ControllerModeType;

/** Status of the PDU channel group. Current mode of the channel defines its
 *  transmit or receive activity. Communication direction (transmission and/or
 *  reception) of the channel can be controlled separately or together by upper
 *  layers. */
// value is bitcoded: bit 0 = rx online, bit 1 = tx online, bit 2 = tx notification online, byte 1 is mask for affected bits
typedef enum {
	/** Channel shall be set to the offline mode
	 *  => no transmission and reception */
	CANIF_SET_OFFLINE = 0x700,
	
	/** Receive path of the corresponding channel
	 *  shall be disabled */
	CANIF_SET_RX_OFFLINE = 0x100,

	/** Receive path of the corresponding channel
	 *  shall be enabled */
	CANIF_SET_RX_ONLINE = 0x101,
	
	/** Transmit path of the corresponding channel
	 *  shall be disabled */
	CANIF_SET_TX_OFFLINE = 0x600,
	
	/** Transmit path of the corresponding channel
	 *  shall be enabled */
	CANIF_SET_TX_ONLINE = 0x606,
	
	/** Channel shall be set to online mode
	 *  => full operation mode */
	CANIF_SET_ONLINE = 0x707,
	
	/** Transmit path of the corresponding channel
	 *  shall be set to the offline active mode
	 *  => notifications are processed but transmit
	 *  requests are blocked. */
	CANIF_SET_TX_OFFLINE_ACTIVE = 0x604
} CanIf_PduSetModeType;

// value is bitcoded: bit 0 = rx online, bit 1 = tx online, bit 2 = tx notification online
typedef enum {
	/** Channel is in the offline mode ==> no transmission or reception */
  CANIF_GET_OFFLINE = 0,
  /** Receive path of the corresponding channel is enabled and
   *  transmit path is disabled */
  CANIF_GET_RX_ONLINE = 0x1,
  /** Transmit path of the corresponding channel is enabled and
   *  receive path is disabled */
  CANIF_GET_TX_ONLINE = 0x6,
  /** Channel is in the online mode ==> full operation mode */
  CANIF_GET_ONLINE = 0x7,
  /** Transmit path of the corresponding channel is in
   *  the offline mode ==> transmit notifications are processed but
   *  transmit requests are blocked. The receiver path is disabled. */
  CANIF_GET_OFFLINE_ACTIVE = 0x4,
  /** Transmit path of the corresponding channel is in the offline
   *  active mode ==> transmit notifications are processed but transmit
   *  requests are blocked. The receive path is enabled. */
  CANIF_GET_OFFLINE_ACTIVE_RX_ONLINE = 0x5
	
} CanIf_PduGetModeType;

typedef enum {
	/** No transmit or receive event occurred for
	 *  the requested L-PDU. */
	CANIF_NO_NOTIFICATION = 0,	
	/** The requested Rx/Tx CAN L-PDU was
	 *  successfully transmitted or received. */
	CANIF_TX_RX_NOTIFICATION
	
} CanIf_NotifStatusType;

// för buffered tx

typedef struct {
	CanId id;
	dlc;
	uint8 controller;
	hohType hth;
  ///todo add driverUnitId
  ///todo: replace controller hth and driverUnitID with canif hth?
	i_PduId; // att använda i callout functionen, kan vara ipdu, npduid eller valfri pduid type??
	void(*user_TxConfirmation)(PduIdType txPduId);
  ///todo lägg till dem error vid send during stop mode 
} CanIf_TxLPduConfigType;

typedef struct {
	CanId id;
	i_PduId; // att använda i callout functionen, kan vara ipdu, npduid eller valfri pduid type
	dlc; /// min dlc and dlc reported to upper layers. Set to -1 to disable min check
//	uint8 controller;
//	hohType hrhId;
	void(*user_RxIndication)(PduIdType rxPduId, const PduInfoType* pduInfoPtr);
  ///todo lägg till dem error vid CANIF_E_INVALID_DLC
} CanIf_RxLPduConfigType;

typedef struct {
	l_pduIdFirst; // multiple lpdus sharing hrh must have pduid in sequense
	///todo is this ok limitation to cope with postbuild requirements?
	numLpdus; // 0 means fullcan, no filtering
} CanIf_HrHConfigType;

typedef struct {
} CanIf_ControllerConfigType;

typedef struct {
	CanIf_TxLPduConfigType txLpduCfg[CANIF_NUM_TX_LPDU_ID];
	CanIf_RxLPduConfigType rxLpduCfg[CANIF_NUM_RX_LPDU_ID];
	///todo is this really postbuild? Otherwise should this not be here
	CanIf_ControllerConfigType controller[CANIF_CHANNEL_CNT];
} CanIf_ConfigType;

typedef struct {
	void(*user_ControllerModeIndication)(uint8 controllerId, CanIf_ControllerModeType controllerMode);
	void(*user_ControllerBusOff)(uint8 controllerId);
//  void(*user_SetWakeupEvent)(asdf)
//  void(*user_TrcvModeIndication)(asdf)
}CanIf_DispatchCfgType;
#endif /*CANIF_TYPES_H_*/
/** @} */
