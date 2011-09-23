
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
  CAN_OBJECT_TYPE_RECEIVE,
  CAN_OBJECT_TYPE_TRANSMIT,
} Can_ObjectTypeType;

/** HW object Can type. Full not supported. */
typedef enum {
  CAN_ARC_HANDLE_TYPE_BASIC,
  CAN_ARC_HANDLE_TYPE_FULL
} Can_Arc_HohType;

// HTH definitions
// Due to effiency: Start with index 0 and don't use any holes in the enumeration
/** Transmit object id:s */
typedef enum {
  CAN_HTH_A_1 = 0,
  CAN_HTH_C_1,
  NUM_OF_HTHS
} Can_Arc_HTHType;

// HRH definitions
// Due to effiency: Start with index 0 and don't use any holes in the enumeration
/** Receive object id:s */
typedef enum {
  CAN_HRH_A_1 = 0,
  CAN_HRH_C_1,
  NUM_OF_HRHS
} Can_Arc_HRHType;

typedef struct
{
  Can_Arc_HohType CanHandleType;
  uint32_t CanIdType : 1;
  uint32_t : 2;
  uint32_t CanIdValue : 29; // msb set means extended
  EcucIntegerParamDef CanObjectId;
  EcucEnumerationParamDef CanObjectType;
  CanController *CanControllerRef;
  CanFilterMask *CanFilterMaskRef;
} attribute((section=.canpostbuild)) CanHardwareObject;


typedef struct
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
} CanGeneral;

struct CanControllerBaudrateConfig
{
  EcucIntegerParamDef CanControllerBaudRate;
  EcucIntegerParamDef CanControllerPropSeg;
  EcucIntegerParamDef CanControllerSeg1;
  EcucIntegerParamDef CanControllerSeg2;
  EcucIntegerParamDef CanControllerSyncJumpWidth;
} attribute((section=.canpostbuild));

typedef struct
{
  EcucEnumerationParamDef CanBusoffProcessing;
  EcucBooleanParamDef CanControllerActivation;
  EcucIntegerParamDef CanControllerBaseAddress;
  EcucIntegerParamDef CanControllerId;
  EcucEnumerationParamDef CanRxProcessing;
  EcucEnumerationParamDef CanTxProcessing;
  EcucEnumerationParamDef CanWakeupProcessing;
  EcucBooleanParamDef CanWakeupSupport;
  McuClockReferencePoint *CanCpuClockRef;
  EcuMWakeupSource *CanWakeupSourceRef;
  CanControllerBaudrateConfig *CanControllerBaudrateConfig[];
  CanFilterMask *CanFilterMask[];
}CanController;

typedef struct
{
  CanController CanController[CAN_NUM_CONTROLLERS];
  CanHardwareObject *CanHardwareObject;
} CanConfigSet;

typedef const struct
{
  CanConfigSet CanConfigSet;
  CanGeneral CanGeneral;
}Can_ConfigType;

typedef struct
{
}Can_ControllerBaudrateConfigType;

extern const Can_ConfigType;

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
void Can_MainFunction_Write( void );
void Can_MainFunction_Read( void );
void Can_MainFunction_BusOff( void );
void Can_MainFunction_Wakeup( void );

void Can_Isr(uint8 controller, uint8 msgBox);
void Can_ErrIsr(uint8 controller);
