
#include "CanIf.h"
#include "CanIf_Cbk.h"
#include "Det.h"
#include "Dem.h"
///todo implement support for MemMap.h
//#include "MemMap.h"
#if defined(USE_PDUR)
#include "PduR_CanIf.h"
#endif

#if defined(USE_SCHM)
#include "SchM_CanIf.h"
#endif

typedef struct {
	dynCanId[CANIF_NUMBER_OF_DYNAMIC_CANTXPDUIDS];
	struct {
		nextInQueue; // ptr or L-PDU id?
//		canid; // slå upp det från lpdu istället?
		uint8 data[8];
		uint8 dlc; // -1 betyder tom buffer
		bool txConfirmed;
	} txLpdu[CANIF_NUM_TX_LPDU_ID];
	struct {
		nextInQueue; // ptr or L-PDU id?
//		canid; // slå upp det från lpdu istället?
		uint8 data[8];
		uint8 dlc;
		bool rxInd;
	} rxLpdu[CANIF_NUM_RX_LPDU_ID];
} CanIf_LPduDataType;

// för buffered tx
typedef struct {
	
	struct {
		nextInQueue; // ptr or L-PDU id?
	}hth[CANIF_NUM_TX_LPDU_ID];
} CanIf_CanDriverUnitType;

typedef struct {
	CanIf_ControllerModeType mode;
	bool transmitConfirmedSinceLastStart;
	
} CanIf_CanControllerType;

static CanIf_LPduDataType lPduData;
static CanIf_CanDriverUnitType driverUnit;
static const CanIf_ConfigType* CanIf_ConfigPtr;
static CanIf_CanControllerType controllerData[];

void CanIf_Init(const CanIf_ConfigType* ConfigPtr);

Std_ReturnType CanIf_SetControllerMode(uint8 ControllerId, CanIf_ControllerModeType ControllerMode) {
	// slå upp controller, sätt mode, sätt lokal flagga om mode
}

Std_ReturnType CanIf_GetControllerMode(uint8 ControllerId, CanIf_ControllerModeType *ControllerModePtr) {
	// returnera lokal flagga
}
		
Std_ReturnType CanIf_Transmit(PduIdType canTxPduId,	const PduInfoType *pduInfoPtr) {
	// verifiera att channel är online
	if(controllerData[CanIf_ConfigPtr->txLpduCfg[canTxPduId].controller].mode != CANIF_CS_STARTED) {
		// channel not started, return
		return ERR_NOT_OK;
	}
	Can_HwHandleType hth = CanIf_ConfigPtr->txLpduCfg[canTxPduId].hth;
	uint8 controller = CanIf_ConfigPtr->txLpduCfg[canTxPduId].controller;
	Can_IdType canId = ((canTxPduId < CANIF_NUM_DYNAMIC_TX_LPDU_ID)? lPduData.dynCanId[canTxPduId] : CanIf_ConfigPtr->txLpduCfg[canTxPduId].id)
	Can_PduType canPdu = {
		.sdu = pduInfoPtr->SduDataPtr,
		.id = canId,
		.swPduHandle = canTxPduId,
		.length = pduInfoPtr->SduLength
	};
#if buffered transmit
	bool lock = LockSave();
	if(lPduData.txLpdu[canTxPduId].dlc == -1) {
		// pdu buffer not empty and therefore scheduled to be sent. overwrite data and return
		lPduData.txLpdu[canTxPduId].dlc = pduInfoPtr->SduLength;
		memcpy(lPduData.txLpdu[canTxPduId].data, pduInfoPtr->SduDataPtr, pduInfoPtr->SduLength);
	} else if(Can_Write(hth, &canPdu) == CAN_BUSY) {
		// write data to buffer
		lPduData.txLpdu[canTxPduId].dlc = pduInfoPtr->SduLength;
		memcpy(lPduData.txLpdu[canTxPduId].data, pduInfoPtr->SduDataPtr, pduInfoPtr->SduLength);
		// add txlpdu to transmission queue in prio order
		PduId *pduIdPtr = &driverUnit.hth[hth].nextInQueue;
		///todo ensure that high id means high prio
		while(*pduIdPtr != -1 &&
			((*pduIdPtr < CANIF_NUM_DYNAMIC_TX_LPDU_ID)? lPduData.dynCanId[*pduIdPtr] : CanIf_ConfigPtr->txLpduCfg[*pduIdPtr].id) >= canId) {
			*pduIdPtr = &lPduData.txLpdu[canTxPduId].nextInQueue;
		}
		// add pdu tp queue
		lPduData.txLpdu[canTxPduId].nextInQueue = *pduIdPtr;
		*pduIdPtr = canTxPduId;
	}
	LockRestore(lock);
#else
	Can_Write(hth, &canPdu);
#endif
	///todo shoud this funtion return E_NOT_OK if not buffered transmission and Can_Write returns not ok?
	return E_OK;
}

#if CANIF_CANCEL_TRANSMIT_SUPPORT
Std_ReturnType CanIf_CancelTransmit(PduIdType CanTxPduId) {
	// not supported by AutoSAR CanDriver API, just return
	///todo kolla vad som skall returneras
	///todo cancel ev buffered data?
	return STD_OK;
}
#endif

#if ( CANIF_READRXPDU_DATA_API == STD_ON )
Std_ReturnType CanIf_ReadRxPduData(PduIdType CanRxPduId, PduInfoType *PduInfoPtr) {
	// return local L-PDU data
}
#endif

#if ( CANIF_READTXPDU_NOTIFY_STATUS_API == STD_ON )
CanIf_NotifStatusType CanIf_ReadTxNotifStatus(PduIdType CanTxPduId) {
	// return tx notif status of L-PDU
}
#endif

#if ( CANIF_READTXPDU_NOTIFY_STATUS_API == STD_ON )
CanIf_NotifStatusType CanIf_ReadRxNotifStatus(PduIdType CanRxPduId) {
	// return tx notif status of L-PDU
}
#endif

Std_ReturnType CanIf_SetPduMode(uint8 ControllerId, CanIf_PduSetModeType PduModeRequest) {
	// set controller pdu mode
}
Std_ReturnType CanIf_GetPduMode(uint8 ControllerId, CanIf_PduGetModeType* PduModePtr) {
	// return controller pdu mode
}
#if ( CANIF_VERSION_INFO_API == STD_ON )
#define CanIf_GetVersionInfo(_vi) STD_GET_VERSION_INFO(_vi,CANIF)
#endif

#if CANIF_PUBLIC_SETDYNAMICTXID_API
void CanIf_SetDynamicTxId(PduIdType CanTxPduId, Can_IdType CanId) {
	// change the tx id
}
#endif

Std_ReturnType CanIf_SetTrcvMode( uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode ) {
	///todo implement
}
Std_ReturnType CanIf_GetTrcvMode( uint8 TransceiverId, CanTrcv_TrcvModeType* TransceiverModePtr ) {
	///todo implement
}
Std_ReturnType CanIf_GetTrcvWakeupReason( uint8 TransceiverId, CanTrcv_TrcvWakeupReasonType* TrcvWuReasonPtr ) {
	///todo implement
}
Std_ReturnType CanIf_SetTrcvWakeupMode( uint8 TransceiverId, CanTrcv_TrcvWakeupModeType TrcvWakeupMode ) {
	///todo implement
}

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
CanIf_NotifStatusType CanIf_GetTxConfirmationState(uint8 ControllerId) {
	///todo implement
}
#endif


void CanIf_TxConfirmation(PduIdType CanTxPduId) { // L-PDU id
#if buffered transmit
	controllerData[CanIf_ConfigPtr->txLpduCfg[canTxPduId].controller].transmitConfirmedSinceLastStart = 1;
	hth = CanIf_ConfigPtr->txLpduCfg[canTxPduId].controller].hth;
	bool lock = LockSave();
	Can_HwHandleType lpdu = driverUnit.hth[hth].nextInQueue;
	if(lpdu != -1) {
		// send next
		Can_IdType canId = ((lpdu < CANIF_NUM_DYNAMIC_TX_LPDU_ID)? lPduData.dynCanId[lpdu] : CanIf_ConfigPtr->txLpduCfg[lpdu].id)
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
		driverUnit.hth[hth].nextInQueue = lPduData.txLpdu[lpdu].nextInQueue;
	}
	LockRestore(lock);
#endif
	// set tx ind flag
	lPduData.txLpdu[canTxPduId].txConfirmed;
	// call eventual callback
	if(CanIf_ConfigPtr->txLpduCfg[canTxPduId].callback) {
		(*CanIf_ConfigPtr->txLpduCfg[canTxPduId].callback)(CanIf_ConfigPtr->txLpduCfg[canTxPduId].i_PduId);
	}
}

void CanIf_RxIndication(Can_HwHandleType hrh, Can_IdType canId, uint8 canDlc, const uint8* canSduPtr) {
	// function callbed by CAN controller. Different names for different driver units
	CanIf_Arc_RxIndication(hrh, canId, canDlc, canSduPtr, &driverUnit);
}

void CanIf_Arc_RxIndication(Can_HwHandleType hrh, Can_IdType canId, uint8 canDlc, const uint8* canSduPtr, CanIf_CanDriverUnitType *driverUnit) {
	PduIdType firstPduId = 
	int numPdus = 
	if(numPdus == 0) {
		// no filtering, call eventual callback
		if(CanIf_ConfigPtr->rxLpduCfg[firstPduId].callback) {
			(*CanIf_ConfigPtr->rxLpduCfg[firstPduId].callback)(canDlc, canSduPtr, CanIf_ConfigPtr->rxLpduCfg[firstPduId].i_PduId);
		}
	} else {
		while(numPdus > 1) {
			if(CanIf_ConfigPtr->rxLpduCfg[firstPduId + numPdus / 2].id >= canId) {
				firstPduId += numPdus / 2;
				numPdus = numPdus / 2 + numPdus % 2;
			} else {
				numPdus = numPdus / 2;
			}
		}
		if(CanIf_ConfigPtr->rxLpduCfg[firstPduId].id == canId) {
			// expected canId found, call eventual callback
			if(CanIf_ConfigPtr->rxLpduCfg[firstPduId].callback) {
				(*CanIf_ConfigPtr->rxLpduCfg[firstPduId].callback)(canDlc, canSduPtr, CanIf_ConfigPtr->rxLpduCfg[firstPduId].i_PduId);
			}
		}
	}
}

void CanIf_CancelTxConfirmation(const Can_PduType* PduInfoPtr) {
#if buffered transmit
	// om buffer tom:
	// skriv till buffer
	// om hth-kö ej tom: schedulera enligt prio
	// om hth-kö tom: skriv till controller
	  // om busy, lägg på kön
#else
	// do nothing
#endif
}

void CanIf_ControllerBusOff(uint8 Controller) {
	// anropa ev intressenter
}

void CanIf_ControllerModeIndication(uint8 Controller, CanIf_ControllerModeType ControllerMode) {
	// anropa ev intressenter
}

void CanIf_TrcvModeIndication(uint8 Transceiver, CanTrcv_TrcvModeType TransceiverMode) {
}
