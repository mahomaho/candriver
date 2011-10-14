
#include "Can_GeneralTypes.h"
#include "Can_Cfg.h"
#include "ComStack_Types.h"

struct CanController;

typedef struct 
{
  uint32 CanFilterMaskValue;
} attribute((section=.canpostbuild)) CanFilterMask;

/** CAN id types. */
typedef enum {
  CAN_ID_TYPE_EXTENDED,
  CAN_ID_TYPE_MIXED,
  CAN_ID_TYPE_STANDARD,
} Can_IdTypeType;

/** CAN HW object types. */
typedef enum {
  CAN_OBJECT_TYPE_NONE,
  CAN_OBJECT_TYPE_RECEIVE,
  CAN_OBJECT_TYPE_TRANSMIT,
} Can_ObjectTypeType;

/** HW object Can type. Full not supported. */
typedef enum {
  CAN_ARC_HANDLE_TYPE_BASIC,
  CAN_ARC_HANDLE_TYPE_FULL
} Can_Arc_HohType;


typedef union
{
	Can_ObjectTypeType CanObjectType; // hth or hrh
	//CanController *CanControllerRef; // ersätt med msgbox ptr?
	CanMsgBox *msgBox; // pointer to msg box
	//Can_HwHandleType CanObjectId; // hoh id, used in rx callback
	struct hrh
	{
	  Can_ObjectTypeType CanObjectType; // hth or hrh
	  //CanController *CanControllerRef; // ersätt med msgbox ptr?
	  CanMsgBox *msgBox; // pointer to msg box
	  Can_HwHandleType CanObjectId; // hoh id, used in rx callback
	  uint32_t CanIdType : 1; // msb set means extended
	  uint32_t : 2;
	  uint32_t CanIdValue : 29; // canId value is the pattern for rx
	  #if CAN_ENABLE_INDIVIDUAL_MASK == STD_ON
	  CanFilterMask CanFilterMask;
	  #endif
	} hrh;
	struct hth
	{
	  Can_ObjectTypeType CanObjectType; // hth or hrh
	  //CanController *CanControllerRef; // ersätt med msgbox ptr?
	  CanMsgBox *msgBox; // pointer to msg box
	  //Can_HwHandleType CanObjectId; // hoh id, used in rx callback
	  #if CAN_MULTIPLEXED_TRANSMISSION
	  uint8 numMultiplexed;
	  #endif
	  #if CAN_ENABLE_MIXED_MODE == STD_OFF
	  bool CanIdType;
	  #endif
	}
} attribute((section=.canpostbuild)) CanHardwareObject;


/*typedef struct
{
  EcucBooleanParamDef CanDevErrorDetection;
  EcucBooleanParamDef CanHardwareCancellation;
  EcucBooleanParamDef CanIdenticalIdCancellation;
  EcucIntegerParamDef CanIndex;
  EcucFunctionNameDef CanLPduReceiveCalloutFunction;
  EcucFloatParamDef CanMainFunctionBusoffPeriod;
  EcucFloatParamDef CanMainFunctionModePeriod;
  {
  EcucFloatParamDef CanMainFunctionReadPeriod;
  CanHardwareObject *CanMainFunctionReadPeriodRef;
  }[];
  EcucFloatParamDef CanMainFunctionWakeupPeriod;
  {
  EcucFloatParamDef CanMainFunctionWritePeriod;
  CanHardwareObject *CanMainFunctionWritePeriodRef;
  }[];
  EcucBooleanParamDef CanMultiplexedTransmission;
  EcucFloatParamDef CanTimeoutDuration;
  EcucBooleanParamDef CanVersionInfoApi;
  OsCounter *CanCounterRef;
  //CanIfPrivateCfg *CanSupportTTCANRef;
} CanGeneral;*/

typedef struct
{
  uint16 CanControllerBaudRate;
  uint8 CanControllerPropSeg;
  uint8 CanControllerSeg1;
  uint8 CanControllerSeg2;
  uint8 CanControllerSyncJumpWidth;
}Can_ControllerBaudrateConfigType;

typedef struct
{
//  EcucEnumerationParamDef CanBusoffProcessing;
//  EcucBooleanParamDef CanControllerActivation;
  EcucIntegerParamDef CanControllerBaseAddress;
//  EcucIntegerParamDef CanControllerId;
//  EcucEnumerationParamDef CanRxProcessing;
//  EcucEnumerationParamDef CanTxProcessing;
//  EcucEnumerationParamDef CanWakeupProcessing;
//  EcucBooleanParamDef CanWakeupSupport;
//  McuClockReferencePoint *CanCpuClockRef;
  uint32 CanCpuClock; // set to oscillator clock freq
//  EcuMWakeupSource *CanWakeupSourceRef;
  Can_ControllerBaudrateConfigType CanControllerBaudrateConfig;
  uint32 txisrmask;
  uint32 rxisrmask;
  #if CAN_NUM_MSGBOXES > 32
  uint32 txisrmaskH;
  uint32 rxisrmaskH;
  #endif
#if CAN_ENABLE_INDIVIDUAL_MASK == STD_OFF
  CanFilterMask CanGlobalFilterMask;
  CanFilterMask Can14FilterMask;
  CanFilterMask Can15FilterMask;
#else
  CanFilterMask CanFilterMask[CAN_NUM_MSGBOXES];
#endif
}CanController;

typedef struct
{
  CanController controller[CAN_NUM_CONTROLLERS];
  CanHardwareObject hoh[NUM_OF_HOHS];
} CanConfigSet;

typedef const struct
{
  CanConfigSet CanConfigSet;
//  CanGeneral CanGeneral;
}attribute((section=.canpostbuild)) Can_ConfigType;

extern Can_ConfigType Can_config;

void Can_Init( const Can_ConfigType *Config );
//void Can_DeInit(void);

#if ( CAN_VERSION_INFO_API == STD_ON )
#define Can_GetVersionInfo(_vi) STD_GET_VERSION_INFO(_vi,CAN)
#endif

void Can_InitController( uint8 controller, const Can_ControllerBaudrateConfigType *config);
Can_ReturnType Can_SetControllerMode( uint8 Controller, Can_StateTransitionType transition );
void Can_DisableControllerInterrupts( uint8 controller );
void Can_EnableControllerInterrupts( uint8 controller );
// Hth - for Flexcan, the hardware message box number... .We don't care


Can_ReturnType Can_Write( Can_HwHandleType hth, Can_PduType *pduInfo );

void Can_Cbk_CheckWakeup( uint8 controller );
void Can_MainFunction_BusOff( void );
void Can_MainFunction_Wakeup( void );
void Can_MainFunction_Mode( void );

void Can_Isr(Can_HwHandleType hoh, msgboxP);
void Can_ErrIsr(uint8 controller);

void Can_Arc_Write( Can_HwHandleType hth ); // called either from Can_MainFunction_Write or isr
void Can_Arc_Read( Can_HwHandleType hrh );  // called either from Can_MainFunction_Read or isr
void Can_Arc_BusOff( uint8 controller );
void Can_Arc_Wakeup( uint8 controller );
