/** @addtogroup CanIf CAN Interface
 *  @{ */

/** @file CanIf_Types.h
 *  Definitions of configuration parameters for CAN Interface.
 */

#ifndef CANIF_TYPES_H_
#define CANIF_TYPES_H_

#include "ComStack_Types.h"
#include "Can_GeneralTypes.h"

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

#if CANIF_PUBLIC_MULTIPLE_DRV_SUPPORT
typedef struct {
  void (*CanIf_TxConfirmation)(PduIdType CanTxPduId); // L-PDU id
  void (*CanIf_RxIndication)(Can_HwHandleType Hrh, Can_IdType CanId, uint8 CanDlc, const uint8* CanSduPtr);
#if CANIF_CTRLDRV_TX_CANCELLATION
  void (*CanIf_CancelTxConfirmation)(const Can_PduType* PduInfoPtr);
#endif
  void (*CanIf_ControllerBusOff)(uint8 Controller);
  void (*CanIf_ControllerModeIndication)(uint8 Controller, CanIf_ControllerModeType ControllerMode);
  uint8 numControllers;
//  Can_HwHandleType numHth;
//  Can_HwHandleType numHrh;
  //uint8 controllerId; // canif controller id
}CanIf_DriverUnitConfigType;
#endif

typedef struct {
  /// upper layer confirmation function, set to null if no confirmation
	void(*user_TxConfirmation)(PduIdType txPduId);
  /// can id used for transmission, msb indicates extended id
  Can_IdType id;
  /// upper layer pdu id passed to callout function
	PduIdType ulPduId;
	///todo what should the dlc be used for in tx lpdu object? transmit length? If so, what to do with non filled bytes? Error code?
  uint8 dlc;
	/// can driver controller id to be used for transmission
  uint8 controller;
  /// can driver hth id to be used for transmission
	Can_HwHandleType hth;
  ///todo add driverUnitId
} CanIf_TxLPduConfigType;

typedef struct {
  /// upper layer indication function, set to null if no rx indication
	void(*user_RxIndication)(PduIdType rxPduId, const PduInfoType* pduInfoPtr);
  /// can id used for reception filtering
  ///todo add support for range reception
  Can_IdType id;
  /// upper layer pdu id passed to callout function
	PduIdType ulPduId;
  /// min dlc and dlc reported to upper layers. Set to -1 to disable dlc check
	uint8 dlc;
	/// can driver controller id from where to receive lpdu
  ///todo is this correct? Can an lpdu only be received from 1 controller?
	uint8 controller;
} CanIf_RxLPduConfigType;

typedef struct {
  union {
    PduIdType lpduId;
    PduIdType *array;
  }pduInfo;
  PduIdType arrayLen; // 0 means no ptr no filtering = fullCan reception
} CanIf_HrHConfigType;

typedef struct {
} CanIf_ControllerConfigType;

typedef struct {
	void(*user_ControllerModeIndication)(uint8 controllerId, CanIf_ControllerModeType controllerMode);
	void(*user_ControllerBusOff)(uint8 controllerId);
//  void(*user_SetWakeupEvent)(asdf)
//  void(*user_TrcvModeIndication)(asdf)
}CanIf_DispatchCfgType;

typedef struct {
	CanIf_TxLPduConfigType txLpduCfg[CANIF_NUM_TX_LPDU_ID];
	CanIf_RxLPduConfigType rxLpduCfg[CANIF_NUM_RX_LPDU_ID];
	///todo is this really postbuild? Otherwise should this not be here
	CanIf_ControllerConfigType controller[CANIF_CHANNEL_CNT];
   const CanIf_HrHConfigType* canIfHrhCfg[CANIF_NUM_DRIVER_UNITS];
} CanIf_ConfigType;

extern const CanIf_ConfigType CanIf_Config;

#endif /*CANIF_TYPES_H_*/
/** @} */
