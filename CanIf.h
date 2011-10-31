

#include "CanIf_Types.h"
#include "CanIf_Cfg.h"

#define CANIF_VENDOR_ID                   1
#define CANIF_MODULE_ID                   MODULE_ID_CANIF
#define CANIF_AR_RELEASE_MAJOR_VERSION    4
#define CANIF_AR_RELEASE_MINOR_VERSION    0
#define CANIF_AR_RELEASE_REVISION_VERSION 2
#define CANIF_SW_MAJOR_VERSION            0
#define CANIF_SW_MINOR_VERSION            1
#define CANIF_SW_PATCH_VERSION            0

/** @name Error Codes */
///{
#define CANIF_E_PARAM_CANID		            10 
#define CANIF_E_PARAM_DLC			            11  
#define CANIF_E_PARAM_HRH			            12 
#define CANIF_E_PARAM_CHANNEL		          13  
#define CANIF_E_PARAM_CONTROLLER	        14  
#define CANIF_E_PARAM_WAKEUPSOURCE	      15  

#define CANIF_E_PARAM_HTH                 17 
#define CANIF_E_PARAM_LPDU                18
#define CANIF_E_PARAM_CONTROLLER_MODE     19

#define CANIF_E_PARAM_POINTER 			      20
#define CANIF_E_UNINIT 				            30
#define CANIF_E_NOK_NOSUPPORT             40
#define CANIF_TRCV_E_TRCV_NOT_STANDBY	    60
#define CANIF_TRCV_E_TRCV_NOT_NORMAL	    70
#define CANIF_E_INVALID_TXPDUID		        80
#define CANIF_E_INVALID_RXPDUID 		      90
///}

void CanIf_Init(const CanIf_ConfigType* ConfigPtr);

Std_ReturnType CanIf_SetControllerMode(uint8 ControllerId, CanIf_ControllerModeType ControllerMode);

Std_ReturnType CanIf_GetControllerMode(uint8 ControllerId, CanIf_ControllerModeType *ControllerModePtr);

Std_ReturnType CanIf_Transmit(PduIdType CanTxPduId, const PduInfoType *PduInfoPtr);

#if CANIF_CANCEL_TRANSMIT_SUPPORT
Std_ReturnType CanIf_CancelTransmit(PduIdType CanTxPduId);
#endif

#if ( CANIF_PUBLIC_READRXPDU_DATA_API == STD_ON )
Std_ReturnType CanIf_ReadRxPduData(PduIdType CanRxPduId,
		PduInfoType *PduInfoPtr);
#endif

#if ( CANIF_PUBLIC_READTXPDU_NOTIFY_STATUS_API == STD_ON )
CanIf_NotifStatusType CanIf_ReadTxNotifStatus(PduIdType CanTxPduId);
#endif

#if ( CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API == STD_ON )
CanIf_NotifStatusType CanIf_ReadRxNotifStatus(PduIdType CanRxPduId);
#endif

Std_ReturnType CanIf_SetPduMode(uint8 ControllerId, CanIf_PduSetModeType PduModeRequest);
Std_ReturnType CanIf_GetPduMode(uint8 ControllerId, CanIf_PduGetModeType* PduModePtr);
#if ( CANIF_PUBLIC_VERSION_INFO_API == STD_ON )
#define CanIf_GetVersionInfo(_vi) STD_GET_VERSION_INFO(_vi,CANIF)
#endif

#if CANIF_PUBLIC_SETDYNAMICTXID_API
void CanIf_SetDynamicTxId(PduIdType CanTxPduId, Can_IdType CanId);
#endif

Std_ReturnType CanIf_SetTrcvMode( uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode );
Std_ReturnType CanIf_GetTrcvMode( uint8 TransceiverId, CanTrcv_TrcvModeType* TransceiverModePtr );
#if CANIF_TRCV_WAKEUP_SUPPORT
Std_ReturnType CanIf_GetTrcvWakeupReason( uint8 TransceiverId, CanTrcv_TrcvWakeupReasonType* TrcvWuReasonPtr );
Std_ReturnType CanIf_SetTrcvWakeupMode( uint8 TransceiverId, CanTrcv_TrcvWakeupModeType TrcvWakeupMode );
#endif

#if CANIF_CTRL_WAKEUP_SUPPORT || CANIF_TRCV_WAKEUP_SUPPORT
Std_ReturnType CanIf_CheckWakeup( EcuM_WakeupSourceType WakeupSource );
#if CANIF_PUBLIC_WAKEUP_CHECK_VALIDATION_API
Std_ReturnType CanIf_CheckValidation( EcuM_WakeupSourceType WakeupSource );
#endif
#endif

#if CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT
CanIf_NotifStatusType CanIf_GetTxConfirmationState(uint8 ControllerId);
#endif
