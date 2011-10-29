#ifndef CANIF_CFG_H_
#define CANIF_CFG_H_


/// Enabled version info
#define CANIF_PUBLIC_VERSION_INFO_API             STD_ON
/// Enables the  CanIf_ReadRxPduData function
#define CANIF_PUBLIC_READRXPDU_DATA_API		STD_ON
/// Enables the CanIf_CancelTransmit function
#define CANIF_CANCEL_TRANSMIT_SUPPORT		STD_ON
/// Enables the CanIf_ReadTxNotifStatus function
#define CANIF_PUBLIC_READTXPDU_NOTIFY_STATUS_API	STD_ON
/// Enables the CanIf_ReadRxNotifStatus function
#define CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API	STD_ON
/// Enables the CanIf_GetTxConfirmationState function
#define CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT    STD_ON
/// Enabled the CanIf_CancelTxConfirmation function
#define CANIF_CTRLDRV_TX_CANCELLATION				      STD_ON
/// Enables the dlc check for received frames
#define CANIF_PRIVATE_DLC_CHECK                   STD_ON
/// Selects type of software filtering, only binary search supported
#define CANIF_PRIVATE_SOFTWARE_FILTER_TYPE			  BINARY
/// Datatype for hoh ids, uint8 or uint16
#define CANIF_PUBLIC_HANDLE_TYPE_ENUM				      uint16
/// Enables the tx buffering mechanism
#define CANIF_PUBLIC_TX_BUFFERING					        STD_ON
/// Enables use of multiple can driver units
#define CANIF_PUBLIC_MULTIPLE_DRV_SUPPORT			    STD_OFF // Not supported
/// Enables DET error reports
#define CANIF_DEV_ERROR_DETECT			              STD_OFF
/// Enabled the CanIf_CheckValidation function
#define CANIF_PUBLIC_WAKEUP_CHECK_VALIDATION_API  STD_OFF // not supported
/// Defines if a can controller has wakeup support
#define CANIF_CTRL_WAKEUP_SUPPORT                 STD_OFF // not supported
/// Defines if a can tranceiver has wakeup support
#define CANIF_TRCV_WAKEUP_SUPPORT                 STD_OFF // not supported


typedef enum {
	CANIF_RX_LPDU_ID_CanDB_Message_1,
	CANIF_RX_LPDU_ID_CanDB_Message_2,
	CANIF_NUM_RX_LPDU_ID
} CanIf_Arc_RxLPduIdType;

typedef enum {
	CANIF_TX_LPDU_ID_CanDB_Message_1,
	CANIF_TX_LPDU_ID_CanDB_Message_2,
	CANIF_TX_LPDU_ID_CanDB_Message_3,
	CANIF_NUMBER_OF_DYNAMIC_CANTXPDUIDS
} CanIf_Arc_DynamicTxLPduIdType;

typedef enum {
	CANIF_TX_LPDU_ID_CanDB_Message_4 = CANIF_NUMBER_OF_DYNAMIC_CANTXPDUIDS,
	CANIF_NUM_TX_LPDU_ID
} CanIf_Arc_StaticTxLPduIdType;

typedef enum {
	CANIF_Controller_A,
	CANIF_CHANNEL_CNT
} CanIf_Arc_ChannelIdType;

#endif //CANIF_CFG_H_