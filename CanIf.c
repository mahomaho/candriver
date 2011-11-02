
#include "CanIf.h"
#include "CanIf_Cbk.h"
#include "Det.h"
#if defined(USE_DEM)
#include "Dem.h"
#define DEM_REPORTERRORSTATUS(_a, _b) Dem_ReportErrorStatus(_a, _b)
#else
#define DEM_REPORTERRORSTATUS(_a, _b)
#endif
#include <string.h>
///todo implement support for MemMap.h
//#include "MemMap.h"
//#if defined(USE_PDUR)
//#include "PduR_CanIf.h"
//#endif

//#if defined(USE_SCHM)
//#include "SchM_CanIf.h"
//#endif

#if ( CANIF_DEV_ERROR_DETECT == STD_ON )
#include "Det.h"
#define VALIDATE(_exp,_api,_err ) \
        if( !(_exp) ) { \
          Det_ReportError(MODULE_ID_CANIF,0,_api,_err); \
          return E_NOT_OK; \
        }

#define VALIDATE_NO_RV(_exp,_api,_err ) \
        if( !(_exp) ) { \
          Det_ReportError(MODULE_ID_CANIF,0,_api,_err); \
          return; \
        }

#define DET_REPORTERROR(_x,_y,_z,_q) Det_ReportError(_x, _y, _z, _q)
#else
#define VALIDATE(_exp,_api,_err )
#define VALIDATE_NO_RV(_exp,_api,_err )
#define DET_REPORTERROR(_x,_y,_z,_q)
#endif

#if ( CANIF_VERSION_INFO_API == STD_ON )
#define CanIf_GetVersionInfo(_vi) STD_GET_VERSION_INFO(_vi,CANIF)
#endif

typedef struct {
#if CANIF_PUBLIC_SETDYNAMICTXID_API
        Can_IdType dynCanId[CANIF_NUMBER_OF_DYNAMIC_CANTXPDUIDS];
#endif
	struct {
#if CANIF_PUBLIC_TX_BUFFERING
		PduIdType nextInQueue;
		uint8 data[8];
    // dlc set to -1 indicates empty buffer
		uint8 dlc;
#endif
#if CANIF_PUBLIC_READTXPDU_NOTIFY_STATUS_API
		bool txConfirmed;
#endif
	} txLpdu[CANIF_NUM_TX_LPDU_ID];
	struct {
#if CANIF_PUBLIC_READRXPDU_DATA_API
		uint8 data[8];
		uint8 dlc;
#endif
#if CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API
		bool rxInd;
#endif
	} rxLpdu[CANIF_NUM_RX_LPDU_ID];
} CanIf_LPduDataType;

typedef struct {
	
	struct {
    PduIdType nextInQueue; // next tx l-pduid
	}hth[CANIF_NUM_HTHS];
} CanIf_HthDataType;

typedef struct {
	uint8 controllerMode; // CanIf_ControllerModeType
	uint8 pendingControllerMode;  // CanIf_ControllerModeType
	uint8 pduMode;// CanIf_PduGetModeType
#if CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT
	uint8 transmitConfirmedSinceLastStart;
#endif
#if CANIF_PUBLIC_MULTIPLE_DRV_SUPPORT > 1
  uint8 driverUnit;
#endif
} CanIf_CanControllerType;

static CanIf_LPduDataType lPduData;
static CanIf_HthDataType hthData;
static const CanIf_ConfigType* CanIf_ConfigPtr;
static CanIf_CanControllerType controllerData[CANIF_CHANNEL_CNT];

extern const CanIf_DispatchCfgType canIfDispatchCfg;

static inline int LockSave(void) {
	int msr;
	asm volatile("mfmsr %[msr]":[msr] "=r" (msr ) );
	asm volatile("wrteei 0");
	return msr;
}
static inline void LockRestore(int msr) {
  asm volatile ("wrtee %0" : : "r" (msr) );
}

static void ClearTxBuffers(uint8 controller) {
	// reset all pending tx requests
	for(PduIdType i = 0; i < CANIF_NUM_TX_LPDU_ID; i++) {
		if(CanIf_ConfigPtr->txLpduCfg[i].controller == controller) {
#if CANIF_PUBLIC_READTXPDU_NOTIFY_STATUS_API
      // clear notification status
      lPduData.txLpdu[i].txConfirmed = CANIF_NO_NOTIFICATION;
#endif
#if CANIF_PUBLIC_TX_BUFFERING
			// set nextInQueue to indicate empty queue
			hthData.hth[CanIf_ConfigPtr->txLpduCfg[i].hth].nextInQueue = -1;
  		// set dlc to -1 to indicate empty buffer
			lPduData.txLpdu[i].dlc = -1;
#endif
		}
	}
}

static void ClearRxBuffers(uint8 controller) {
	// reset buffers
	for(PduIdType i = 0; i < CANIF_NUM_RX_LPDU_ID; i++) {
		if(CanIf_ConfigPtr->rxLpduCfg[i].controller == controller) {
#if CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API
      // clear notification status
      lPduData.rxLpdu[i].rxInd = CANIF_NO_NOTIFICATION;
#endif
#if CANIF_PUBLIC_READRXPDU_DATA_API
  		// set dlc to -1 to indicate empty buffer
			lPduData.rxLpdu[i].dlc = -1;
#endif
		}
	}
}

void CanIf_Init(const CanIf_ConfigType* configPtr) {
  // nullptr means use default config
  if(configPtr == 0) {
    configPtr = &CanIf_Config;
  }
#if CANIF_PUBLIC_TX_BUFFERING
  for(int i  = 0; i < CANIF_NUM_TX_LPDU_ID; i++) {
    // set txpdu dlc to -1 to indicate empty tx buffer
    lPduData.txLpdu[i].dlc = -1;
  }
#endif
#if CANIF_PUBLIC_SETDYNAMICTXID_API
  // initialize all dynamic tx ids
  for(int i = 0; i < CANIF_NUMBER_OF_DYNAMIC_CANTXPDUIDS; i++) {
    lPduData.dynCanId[i] = configPtr->txLpduCfg[i].id;
  }
#endif
  // clear all buffers to initialize them
  for(uint8 i = 0; i < CANIF_CHANNEL_CNT; i++) {
    ClearTxBuffers(i);
    ClearRxBuffers(i);
  }
  // initialize all data
  CanIf_ConfigPtr = configPtr;
}


// service id 3
Std_ReturnType CanIf_SetControllerMode(uint8 controllerId, CanIf_ControllerModeType controllerMode) {
	VALIDATE(CanIf_ConfigPtr != 0, 3, CANIF_E_UNINIT);
	VALIDATE(controllerId < CANIF_CHANNEL_CNT, 3, CANIF_E_PARAM_CONTROLLERID);
	Can_StateTransitionType transition;
	// set pending mode to indicate that mode change is ongoing
	controllerData[controllerId].pendingControllerMode = controllerMode;
	switch(controllerMode) {
	case CANIF_CS_STOPPED:
		if(controllerData[controllerId].controllerMode == CANIF_CS_SLEEP) {
			transition = CAN_T_WAKEUP;
		} else {
			transition = CAN_T_STOP;
		}
		break;
	case CANIF_CS_STARTED:
		if(controllerData[controllerId].controllerMode == CANIF_CS_SLEEP) {
			// cannot jump to start from sleep, set wakeup mode
			transition = CAN_T_WAKEUP;
		} else {
			transition = CAN_T_START;
		}
		break;
	case CANIF_CS_SLEEP:
		if(controllerData[controllerId].controllerMode == CANIF_CS_STARTED) {
			// cannot jump to sleep from started, set stop mode
			transition = CAN_T_STOP;
		} else {
			transition = CAN_T_SLEEP;
		}
		break;
	default:
    // general error code for invalid parameters
    DET_REPORTERROR(MODULE_ID_CANIF, 0, 3,CANIF_E_NOK_NOSUPPORT);
    return E_NOT_OK;
	}
	if(Can_SetControllerMode(controllerId, transition) == CAN_NOT_OK) {
		return E_NOT_OK;
	} else {
		return E_OK;
	}
}

// service id 4
Std_ReturnType CanIf_GetControllerMode(uint8 controllerId, CanIf_ControllerModeType *controllerModePtr) {
	VALIDATE(CanIf_ConfigPtr != 0, 4, CANIF_E_UNINIT);
	VALIDATE(controllerModePtr != 0, 4, CANIF_E_PARAM_POINTER);
	VALIDATE(controllerId < CANIF_CHANNEL_CNT, 4, CANIF_E_PARAM_CONTROLLERID);
	*controllerModePtr = controllerData[controllerId].controllerMode;
	return E_OK;
}

#if CANIF_PUBLIC_TX_BUFFERING
static void AddTxMsgToQueue(PduIdType canTxPduId, const uint8 *sduPtr, uint8 dlc, Can_IdType canId) {
  // write data to buffer
  lPduData.txLpdu[canTxPduId].dlc = dlc;
  memcpy(lPduData.txLpdu[canTxPduId].data, sduPtr, dlc);
  // add txlpdu to transmission queue in prio order
  PduIdType *pduIdPtr = &hthData.hth[CanIf_ConfigPtr->txLpduCfg[canTxPduId].hth].nextInQueue;
  // low id means high prio means high prio
  while(*pduIdPtr != (PduIdType)-1 &&
    ((*pduIdPtr < CANIF_NUMBER_OF_DYNAMIC_CANTXPDUIDS)? lPduData.dynCanId[*pduIdPtr] : CanIf_ConfigPtr->txLpduCfg[*pduIdPtr].id) <= canId) {
    pduIdPtr = &lPduData.txLpdu[canTxPduId].nextInQueue;
  }
  // add pdu tp queue
  lPduData.txLpdu[canTxPduId].nextInQueue = *pduIdPtr;
  *pduIdPtr = canTxPduId;
}
#endif

// serive id 5
Std_ReturnType CanIf_Transmit(PduIdType canTxPduId,	const PduInfoType *pduInfoPtr) {
	VALIDATE(CanIf_ConfigPtr != 0, 5, CANIF_E_UNINIT);
	VALIDATE(pduInfoPtr != 0, 5, CANIF_E_PARAM_POINTER);
	VALIDATE(canTxPduId < CANIF_NUM_TX_LPDU_ID, 5, CANIF_E_INVALID_TXPDUID);
	if(controllerData[CanIf_ConfigPtr->txLpduCfg[canTxPduId].controller].controllerMode != CANIF_CS_STARTED) {
		// channel not started, report to DEM and return
    DEM_REPORTERRORSTATUS(CANIF_E_STOPPED, DEM_EVENT_STATUS_FAILED);
    return E_NOT_OK;
	}
	if(!(controllerData[CanIf_ConfigPtr->txLpduCfg[canTxPduId].controller].pduMode & (CANIF_GET_TX_ONLINE ^ CANIF_GET_OFFLINE_ACTIVE))) {
    // TX is not online, report to DEM and return
    DEM_REPORTERRORSTATUS(CANIF_E_STOPPED, DEM_EVENT_STATUS_FAILED);
    return E_NOT_OK;
  }
	Can_HwHandleType hth = CanIf_ConfigPtr->txLpduCfg[canTxPduId].hth;
  Can_IdType canId = ((canTxPduId < CANIF_NUMBER_OF_DYNAMIC_CANTXPDUIDS)? lPduData.dynCanId[canTxPduId] : CanIf_ConfigPtr->txLpduCfg[canTxPduId].id);
	Can_PduType canPdu = {
		.sdu = pduInfoPtr->SduDataPtr,
		.id = canId,
		.swPduHandle = canTxPduId,
		.length = pduInfoPtr->SduLength
	};
#if CANIF_PUBLIC_TX_BUFFERING
	int lock = LockSave();
	if(lPduData.txLpdu[canTxPduId].dlc != (uint8)-1) {
		// pdu buffer not empty and therefore scheduled to be sent. overwrite data and return
		lPduData.txLpdu[canTxPduId].dlc = pduInfoPtr->SduLength;
		memcpy(lPduData.txLpdu[canTxPduId].data, pduInfoPtr->SduDataPtr, pduInfoPtr->SduLength);
	} else {
    Can_ReturnType canRetVal = Can_Write(hth, &canPdu);
    if(canRetVal == CAN_NOT_OK) {
      // erroneous request, return error
      return E_NOT_OK;
    } else if(canRetVal == CAN_BUSY) {
      // write data to buffer
      AddTxMsgToQueue(canTxPduId, pduInfoPtr->SduDataPtr, pduInfoPtr->SduLength, canId);
    }
  }
	LockRestore(lock);
  // return E_OK since message buffered
	return E_OK;
#else
	Can_ReturnType canRetVal = Can_Write(hth, &canPdu);
  if(canRetVal != CAN_OK) {
    // if cotroller busy, just throw message, cannot do anything since no buffers
    return E_NOT_OK;
  }
  return E_OK;
#endif
}

#if CANIF_CANCEL_TRANSMIT_SUPPORT
// sevice id 0x18
Std_ReturnType CanIf_CancelTransmit(PduIdType CanTxPduId) {
	// dummy function with no functionality needed since it may be callbed by PduR
	VALIDATE(CanTxPduId < CANIF_NUM_TX_LPDU_ID, 0x18, CANIF_E_INVALID_TXPDUID);
  return E_OK;
}
#endif

#if ( CANIF_PUBLIC_READRXPDU_DATA_API == STD_ON )
// service id 6
Std_ReturnType CanIf_ReadRxPduData(PduIdType canRxPduId, PduInfoType *pduInfoPtr) {
	VALIDATE(CanIf_ConfigPtr != 0, 6, CANIF_E_UNINIT);
	VALIDATE(pduInfoPtr != 0, 6, CANIF_E_PARAM_POINTER);
	VALIDATE(canRxPduId < CANIF_NUM_RX_LPDU_ID, 6, CANIF_E_INVALID_RXPDUID);
  // check if buffer empty
  if(lPduData.rxLpdu[canRxPduId].dlc == (uint8)-1) {
    // buffer empty, return error
    // this will always be reached if rx offline and or can is in stoped mode since rx buffer gets cleared when those modes are reached
    return E_NOT_OK;
  }
	// copy data
	int lock = LockSave();
	uint8 dlc = lPduData.rxLpdu[canRxPduId].dlc;
	pduInfoPtr->SduLength = dlc;
	memcpy(pduInfoPtr->SduDataPtr, lPduData.rxLpdu[canRxPduId].data, dlc);
	LockRestore(lock);
	return E_OK;
}
#endif

#if ( CANIF_PUBLIC_READTXPDU_NOTIFY_STATUS_API == STD_ON )
// service id 7
CanIf_NotifStatusType CanIf_ReadTxNotifStatus(PduIdType canTxPduId) {
	VALIDATE_NO_RV(CanIf_ConfigPtr != 0, 7, CANIF_E_UNINIT);
	VALIDATE_NO_RV(canTxPduId < CANIF_NUM_TX_LPDU_ID, 7, CANIF_E_INVALID_TXPDUID);
	CanIf_NotifStatusType retVal = lPduData.txLpdu[canTxPduId].txConfirmed;
	if(retVal != CANIF_NO_NOTIFICATION) {
		lPduData.txLpdu[canTxPduId].txConfirmed = CANIF_NO_NOTIFICATION;
	}
	return retVal;
}
#endif

#if ( CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API == STD_ON )
// service id 8
CanIf_NotifStatusType CanIf_ReadRxNotifStatus(PduIdType canRxPduId) {
	VALIDATE_NO_RV(CanIf_ConfigPtr != 0, 8, CANIF_E_UNINIT);
	VALIDATE_NO_RV(canRxPduId < CANIF_NUM_TX_LPDU_ID, 8, CANIF_E_INVALID_TXPDUID);
	CanIf_NotifStatusType retVal = lPduData.rxLpdu[canRxPduId].rxInd;
	if(retVal != CANIF_NO_NOTIFICATION) {
		lPduData.rxLpdu[canRxPduId].rxInd = CANIF_NO_NOTIFICATION;
	}
	return retVal;
}
#endif

// service id 9
Std_ReturnType CanIf_SetPduMode(uint8 controllerId, CanIf_PduSetModeType pduModeRequest) {
	VALIDATE(CanIf_ConfigPtr != 0, 9, CANIF_E_UNINIT);
	VALIDATE(controllerId < CANIF_CHANNEL_CNT, 9, CANIF_E_PARAM_CONTROLLERID);
	// set controller pdu mode
  uint8 oldMode = controllerData[controllerId].pduMode;
  controllerData[controllerId].pduMode = (controllerData[controllerId].pduMode & ~(pduModeRequest >> 8)) | (pduModeRequest & 0xff);
  // check if tx set offline
  if(0 != (oldMode & CANIF_GET_TX_ONLINE) && 0 == (controllerData[controllerId].pduMode & CANIF_GET_TX_ONLINE)) {
    //clear all tx buffers
    int lock = LockSave();
    ClearTxBuffers(controllerId);
    LockRestore(lock);
  }
  // check if rx set offline
  if(0 != (oldMode & CANIF_GET_RX_ONLINE) && 0 == (controllerData[controllerId].pduMode & CANIF_GET_RX_ONLINE)) {
    //clear all rx buffers
    int lock = LockSave();
    ClearRxBuffers(controllerId);
    LockRestore(lock);
  }
	return E_OK;
}

// service id 10
Std_ReturnType CanIf_GetPduMode(uint8 controllerId, CanIf_PduGetModeType* pduModePtr) {
	VALIDATE(CanIf_ConfigPtr != 0, 10, CANIF_E_UNINIT);
	VALIDATE(pduModePtr != 0, 10, CANIF_E_PARAM_POINTER);
	VALIDATE(controllerId < CANIF_CHANNEL_CNT, 10, CANIF_E_PARAM_CONTROLLERID);
	// return controller pdu mode
	*pduModePtr = controllerData[controllerId].pduMode;
  return E_OK;
}

#if CANIF_PUBLIC_SETDYNAMICTXID_API
// service id 12
void CanIf_SetDynamicTxId(PduIdType canTxPduId, Can_IdType canId) {
	VALIDATE_NO_RV(CanIf_ConfigPtr != 0, 12, CANIF_E_UNINIT);
	VALIDATE_NO_RV((canId & 0x80000000) && canId < 0xA0000000 || canId < 0x800, 12, CANIF_E_PARAM_CANID);
	VALIDATE_NO_RV(canTxPduId < CANIF_NUMBER_OF_DYNAMIC_CANTXPDUIDS, 12, CANIF_E_INVALID_TXPDUID);
	// change the tx id
	lPduData.dynCanId[canTxPduId] = canId;
}
#endif

Std_ReturnType CanIf_SetTrcvMode( uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode ) {
	///todo implement
  return E_NOT_OK;
}
Std_ReturnType CanIf_GetTrcvMode( uint8 TransceiverId, CanTrcv_TrcvModeType* TransceiverModePtr ) {
	///todo implement
  return E_NOT_OK;
}
#if CANIF_TRCV_WAKEUP_SUPPORT
Std_ReturnType CanIf_GetTrcvWakeupReason( uint8 TransceiverId, CanTrcv_TrcvWakeupReasonType* TrcvWuReasonPtr ) {
	///todo implement
  return E_NOT_OK;
}
Std_ReturnType CanIf_SetTrcvWakeupMode( uint8 TransceiverId, CanTrcv_TrcvWakeupModeType TrcvWakeupMode ) {
	///todo implement
  return E_NOT_OK;
}
#endif

#if CANIF_CTRL_WAKEUP_SUPPORT || CANIF_TRCV_WAKEUP_SUPPORT
Std_ReturnType CanIf_CheckWakeup( EcuM_WakeupSourceType WakeupSource ) {
	///todo implement
}
#if CANIF_PUBLIC_WAKEUP_CHECK_VALIDATION_API
Std_ReturnType CanIf_CheckValidation( EcuM_WakeupSourceType WakeupSource ) {
	///todo implement
}
#endif
#endif

#if CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT
// service id 0x19
CanIf_NotifStatusType CanIf_GetTxConfirmationState(uint8 controllerId) {
	VALIDATE(CanIf_ConfigPtr != 0, 0x19, CANIF_E_UNINIT);
	VALIDATE(controllerId < CANIF_CHANNEL_CNT, 0x19, CANIF_E_PARAM_CONTROLLERID);
	return controllerData[controllerId].transmitConfirmedSinceLastStart;
}
#endif

// service id 19
void CanIf_TxConfirmation(PduIdType canTxPduId) { // L-PDU id
	VALIDATE_NO_RV(CanIf_ConfigPtr != 0, 19, CANIF_E_UNINIT);
	VALIDATE_NO_RV(canTxPduId < CANIF_NUM_TX_LPDU_ID, 19, CANIF_E_PARAM_LPDU);
  // check if transmit confirmation should be processed
  if((controllerData[CanIf_ConfigPtr->txLpduCfg[canTxPduId].controller].pduMode & CANIF_GET_OFFLINE_ACTIVE) == 0) {
    // transmit confirm should not be processed, return
    return;
  }
#if CANIF_PUBLIC_TX_BUFFERING
#if CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT
	controllerData[CanIf_ConfigPtr->txLpduCfg[canTxPduId].controller].transmitConfirmedSinceLastStart = CANIF_TX_RX_NOTIFICATION;
#endif
  Can_HwHandleType hth = CanIf_ConfigPtr->txLpduCfg[canTxPduId].hth;
	int lock = LockSave();
	// send next in queue if present. Check if queue empty:
	PduIdType lpdu = hthData.hth[hth].nextInQueue;
	if(lpdu != (PduIdType)-1) {
		// send next
    Can_IdType canId = ((lpdu < CANIF_NUMBER_OF_DYNAMIC_CANTXPDUIDS)? lPduData.dynCanId[lpdu] : CanIf_ConfigPtr->txLpduCfg[lpdu].id);
		Can_PduType canPdu = {
			.sdu = lPduData.txLpdu[lpdu].data,
			.id = canId,
			.swPduHandle = lpdu,
			.length = lPduData.txLpdu[lpdu].dlc
		};
		// send data to CAN controller, ignore return value since it cannot be busy, send confirm is just called
		Can_Write(hth, &canPdu);
		// set dlc to -1 to indicate empty buffer
		lPduData.txLpdu[lpdu].dlc = -1;
		// update queue head
		hthData.hth[hth].nextInQueue = lPduData.txLpdu[lpdu].nextInQueue;
	}
	LockRestore(lock);
#endif
#if CANIF_PUBLIC_READTXPDU_NOTIFY_STATUS_API
	// set tx ind flag
	lPduData.txLpdu[canTxPduId].txConfirmed = CANIF_TX_RX_NOTIFICATION;
#endif
	// call eventual callback
	if(CanIf_ConfigPtr->txLpduCfg[canTxPduId].user_TxConfirmation) {
		(*CanIf_ConfigPtr->txLpduCfg[canTxPduId].user_TxConfirmation)(CanIf_ConfigPtr->txLpduCfg[canTxPduId].ulPduId);
	}
}

static void RxLPduReceived(PduIdType lpdu, Can_IdType canId, uint8 canDlc, const uint8* canSduPtr) {
  // validate pdu mode
  if((controllerData[CanIf_ConfigPtr->rxLpduCfg[lpdu].controller].pduMode & CANIF_GET_RX_ONLINE) == 0) {
    // rx not online, throw message
    return;
  }
	// store in buffer
#if CANIF_PRIVATE_DLC_CHECK
	// check if dlc check is enabled
	if(CanIf_ConfigPtr->rxLpduCfg[lpdu].dlc != (uint8)-1) {
		// dlc check is enabled, verify dlc
		if(canDlc < CanIf_ConfigPtr->rxLpduCfg[lpdu].dlc) {
#if defined(USE_DEM)
			// report error to DEM
      DEM_REPORTERRORSTATUS(CANIF_E_INVALID_DLC, DEM_EVENT_STATUS_FAILED);
#endif
			return;
		}
		canDlc = CanIf_ConfigPtr->rxLpduCfg[lpdu].dlc;
	}
#endif
#if CANIF_PUBLIC_READRXPDU_DATA_API
	int lock = LockSave();
	lPduData.rxLpdu[lpdu].dlc = canDlc;
	memcpy(lPduData.rxLpdu[lpdu].data, canSduPtr, canDlc);
	LockRestore(lock);
#endif
#if CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API
	// set indicator
  lPduData.rxLpdu[lpdu].rxInd = CANIF_TX_RX_NOTIFICATION;
#endif
	// call eventual callback
	if(CanIf_ConfigPtr->rxLpduCfg[lpdu].user_RxIndication) {
    PduInfoType pduInfo = {
      .SduLength = canDlc,
      // cast away const qualifier
      ///todo: should this type really be ptr to data and not to const?
      .SduDataPtr = (uint8*)canSduPtr,
    };
		(*CanIf_ConfigPtr->rxLpduCfg[lpdu].user_RxIndication)(CanIf_ConfigPtr->rxLpduCfg[lpdu].ulPduId, &pduInfo);
  }
}

// called by CanIf_RxIndication with info about correct hrhConfig set for CanDriverUnit
// service id 20
void CanIf_Arc_RxIndication(Can_HwHandleType hrh, Can_IdType canId, uint8 canDlc, const uint8* canSduPtr, uint8 driverUnit) {
	VALIDATE_NO_RV(CanIf_ConfigPtr != 0, 20, CANIF_E_UNINIT);
	VALIDATE_NO_RV(hrh < NUM_OF_HRHS, 20, CANIF_E_PARAM_HRH);
	VALIDATE_NO_RV(canId & 0x80000000 && canId < 0xA0000000 || canId < 0x800, 20, CANIF_E_PARAM_CANID);
	VALIDATE_NO_RV(canDlc <= 8, 20, CANIF_E_PARAM_DLC);
	VALIDATE_NO_RV(canSduPtr != 0, 20, CANIF_E_PARAM_POINTER);
  int numPdus =CanIf_ConfigPtr->canIfHrhCfg[driverUnit][hrh].arrayLen;
	if(numPdus == 0) {
		// no filtering, lpdu id found
    RxLPduReceived(CanIf_ConfigPtr->canIfHrhCfg[driverUnit][hrh].pduInfo.lpduId, canId, canDlc, canSduPtr);
  } else {
    PduIdType *firstPduId = CanIf_ConfigPtr->canIfHrhCfg[driverUnit][hrh].pduInfo.array;
		while(numPdus > 1) {
			if(CanIf_ConfigPtr->rxLpduCfg[firstPduId[numPdus / 2]].id >= canId) {
				firstPduId += numPdus / 2;
				numPdus = numPdus / 2 + numPdus % 2;
			} else {
				numPdus = numPdus / 2;
			}
		}
		if(CanIf_ConfigPtr->rxLpduCfg[*firstPduId].id == canId) {
			// lpdu id found handle message
      RxLPduReceived(*firstPduId, canId, canDlc, canSduPtr);
		}
	}
}

#if CANIF_CTRLDRV_TX_CANCELLATION
// service id 21
void CanIf_CancelTxConfirmation(const Can_PduType* pduInfoPtr) {
	VALIDATE_NO_RV(CanIf_ConfigPtr != 0, 21, CANIF_E_UNINIT);
	VALIDATE_NO_RV(pduInfoPtr != 0, 21, CANIF_E_PARAM_POINTER);
	VALIDATE_NO_RV(pduInfoPtr->swPduHandle < CANIF_NUM_TX_LPDU_ID, 21, CANIF_E_PARAM_LPDU);
#if CANIF_PUBLIC_TX_BUFFERING
	int lock = LockSave();
	if(lPduData.txLpdu[pduInfoPtr->swPduHandle].dlc != (uint8)-1) {
		// pdu buffer not empty: throw old data and return
	} else {
		// write data to buffer
    AddTxMsgToQueue(pduInfoPtr->swPduHandle,	pduInfoPtr->sdu, pduInfoPtr->length, pduInfoPtr->id);
  }
  Can_HwHandleType hth = CanIf_ConfigPtr->txLpduCfg[pduInfoPtr->swPduHandle].hth;
	Can_HwHandleType lpdu = hthData.hth[hth].nextInQueue;
  // send next
  Can_IdType canId = ((lpdu < CANIF_NUMBER_OF_DYNAMIC_CANTXPDUIDS)? lPduData.dynCanId[lpdu] : CanIf_ConfigPtr->txLpduCfg[lpdu].id);
  Can_PduType canPdu = {
    .sdu = lPduData.txLpdu[lpdu].data,
    .id = canId,
    .swPduHandle = lpdu,
    .length = lPduData.txLpdu[lpdu].dlc
  };
  // send data to CAN controller, ignore return value since it cannot be busy, send confirm is just called
  Can_Write(hth, &canPdu);
  // set dlc to -1 to indicate empty buffer
  lPduData.txLpdu[lpdu].dlc = -1;
  // update queue head
  hthData.hth[hth].nextInQueue = lPduData.txLpdu[lpdu].nextInQueue;
	LockRestore(lock);
#else
	// do nothing
#endif
}
#endif

void CanIf_ControllerBusOff(uint8 controller) {
	// store the new mode
	controllerData[controller].controllerMode = CANIF_CS_STOPPED;
	// update pending mode
	controllerData[controller].pendingControllerMode = CANIF_CS_STOPPED;
	// reset all buffers
  int lock = LockSave();
  ClearTxBuffers(controller);
  ClearRxBuffers(controller);
  LockRestore(lock);
	// call ev callback
	if(canIfDispatchCfg.user_ControllerBusOff) {
		(*canIfDispatchCfg.user_ControllerBusOff)(controller);
  }
}

void CanIf_ControllerModeIndication(uint8 controller, CanIf_ControllerModeType controllerMode) {
	// store the new mode
	controllerData[controller].controllerMode = controllerMode;
	if(controllerMode == CANIF_CS_STOPPED) {
		// stopped mode reached
    // reset all buffers
    int lock = LockSave();
    ClearTxBuffers(controller);
    ClearRxBuffers(controller);
    LockRestore(lock);
	} else if(controllerMode == CANIF_CS_STARTED) {
#if CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT
    // set controller tx indicator
		controllerData[controller].transmitConfirmedSinceLastStart = CANIF_NO_NOTIFICATION;
#endif
	}
	// check if pending mode reached
	if(controllerData[controller].pendingControllerMode != controllerMode) {
		// pending mode not reached. Set pending mode and return
		CanIf_SetControllerMode(controller, controllerData[controller].pendingControllerMode);
	} else {
		// requested mode reached. Call ev callback
		if(canIfDispatchCfg.user_ControllerModeIndication) {
			(*canIfDispatchCfg.user_ControllerModeIndication)(controller, controllerMode);
		}
	}
}

void CanIf_TrcvModeIndication(uint8 transceiver, CanTrcv_TrcvModeType transceiverMode) {
}
