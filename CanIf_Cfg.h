#ifndef CANIF_CFG_H_
#define CANIF_CFG_H_

/// number of hths referenced by canif
#define CANIF_NUM_HTHS                            NUM_OF_HTHS
/// number of driver units
///todo change this name to something from specificatoin
#define CANIF_NUM_DRIVER_UNITS                    1

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
/// Enables the CanIf_SetDynamicTxId function
#define CANIF_PUBLIC_SETDYNAMICTXID_API            STD_ON
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
  CANIF_PDU_ID_PDU_RX0,
  CANIF_PDU_ID_PDU_functionalRX,
  CANIF_PDU_ID_PDU_UDSphysicalRX,
  CANIF_PDU_ID_PDU_InstrumentCtrl1,
  CANIF_PDU_ID_PDU_InstrumentCtrl2,
  CANIF_PDU_ID_PDU_SERVO1_PDO1_TX,
  CANIF_PDU_ID_PDU_SERVO1_PDO2_TX,
  CANIF_PDU_ID_PDU_InstrumentCtrl3,
  CANIF_PDU_ID_PDU_SERVO1_SDO_TX,
  CANIF_PDU_ID_PDU_SERVO1_BOOT_TX,
  CANIF_PDU_ID_PDU_IMU_GYRO,
  CANIF_PDU_ID_PDU_IMU_ACCELEROMETER,
  CANIF_PDU_ID_PDU_IMU_ANGLES,
	CANIF_NUM_RX_LPDU_ID
} CanIf_Arc_RxLPduIdType;

typedef enum {
	CANIF_TX_LPDU_ID_CanDB_Message_1,
	CANIF_TX_LPDU_ID_CanDB_Message_2,
	CANIF_TX_LPDU_ID_CanDB_Message_3,
	CANIF_NUMBER_OF_DYNAMIC_CANTXPDUIDS
} CanIf_Arc_DynamicTxLPduIdType;

typedef enum {
  CANIF_PDU_ID_PDU_UDSphysicalTX = CANIF_NUMBER_OF_DYNAMIC_CANTXPDUIDS,
  CANIF_PDU_ID_PDU_functionalTX,
  CANIF_PDU_ID_PDU_IcuStatus,
  CANIF_PDU_ID_PDU_Cfg,
  CANIF_PDU_ID_PDU_SensorData,
  CANIF_PDU_ID_PDU_SERVO1_PDO1_RX,
  CANIF_PDU_ID_PDU_SERVO1_PDO2_RX,
  CANIF_PDU_ID_PDU_NM_A_TX,
  CANIF_PDU_ID_PDU_SERVO1_SDO_RX,
  CANIF_PDU_ID_PDU_SERVO1_BOOT_RX,
	CANIF_NUM_TX_LPDU_ID
} CanIf_Arc_StaticTxLPduIdType;

typedef enum {
	CANIF_CONTROLLER_ID_CAN_A,
	CANIF_CONTROLLER_ID_CAN_B,
	CANIF_CHANNEL_CNT
} CanIf_Arc_ChannelIdType;

#endif //CANIF_CFG_H_
