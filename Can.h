
#include "Can_GeneralTypes.h"
#include "Can_Cfg.h"
#include "ComStack_Types.h"

struct CanController;

/** CAN HW object types. */
typedef enum {
  CAN_OBJECT_TYPE_NONE,
  CAN_OBJECT_TYPE_RECEIVE,
  CAN_OBJECT_TYPE_TRANSMIT,
} Can_ObjectTypeType;

typedef const struct
{
	/// Can id for hoh, msb set if extended ID. ID will be overwritten in case of an hth
	Can_IdType canId;
	/// type of object, values from Can_ObjectTypeType
	uint8 canObjectType; // hth or hrh
	/// controller idx that this hoh is connected to
	uint8 controller;
	/// msgBox number used within the can controller for this hoh
	uint8 msgBox;
#if CAN_MULTIPLEXED_TRANSMISSION
	/// hth parameter, defines how many msgBoxes used for multiplexed transmission
	/// set to number of msgBoxes starting with \msgBox that will be used for multiplexed transmission. Ignored if hrh
	uint8 numMultiplexed;
#endif
} CanHardwareObject;

typedef const struct
{
  /// baud rate value in kbaud
  uint16 CanControllerBaudRate;
  /// prop seg value for cotroller
  uint8 CanControllerPropSeg;
  /// seg1 value for controller
  uint8 CanControllerSeg1;
  /// seg2 value for controller
  uint8 CanControllerSeg2;
  /// sjw value for controller
  uint8 CanControllerSyncJumpWidth;
}Can_ControllerBaudrateConfigType;

struct FlexCan;
typedef volatile struct FlexCan FlexCanT;

typedef struct
{
  /// Default baud rate and timing parameters for controller
  Can_ControllerBaudrateConfigType CanControllerBaudrateConfig;
  /// Can controller base address, see ref manual for correct value
  FlexCanT *CanControllerBaseAddress;
  /// set to oscillator clock freq
  uint32 CanCpuClock;
  /// set bit to 1 if msgBox used for tx, msbBox 0 - 31
  uint32 txisrmask;
  /// set bit to 1 if msgBox used for rx, msbBox 0 - 31
  uint32 rxisrmask;
  #if CAN_NUM_MSGBOXES > 32
  /// set bit to 1 if msgBox used for tx, msbBox 32 - 63
  uint32 txisrmaskH;
  /// set bit to 1 if msgBox used for rx, msbBox 32 - 63
  uint32 rxisrmaskH;
  #endif
#if CAN_ENABLE_INDIVIDUAL_MASK == STD_OFF
  /// Mask value for all msgBoxes exsept 14 and 15
  Can_IdType CanGlobalFilterMask;
  /// Mask value for msgBox 14
  Can_IdType Can14FilterMask;
  /// Mask value for msgBox 15
  Can_IdType Can15FilterMask;
#else
  /// Mask value for each msgBox
  Can_IdType CanFilterMask[CAN_NUM_MSGBOXES];
#endif
}CanController;

typedef const struct
{
  CanController controller[CAN_NUM_CONTROLLERS];
  CanHardwareObject hoh[NUM_OF_HOHS];
}attribute((section=.canpostbuild)) Can_ConfigType;

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
