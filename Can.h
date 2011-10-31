#ifndef CAN_H_
#define CAN_H_


#include "ComStack_Types.h"
#include "Can_GeneralTypes.h"
#include "Can_Cfg.h"

#define CAN_VENDOR_ID                   1
#define CAN_AR_RELEASE_MAJOR_VERSION    4
#define CAN_AR_RELEASE_MINOR_VERSION    0
#define CAN_AR_RELEASE_REVISION_VERSION 2
#define CAN_MODULE_ID                   MODULE_ID_CAN
#define CAN_SW_MAJOR_VERSION            0
#define CAN_SW_MINOR_VERSION            1
#define CAN_SW_PATCH_VERSION            0
///todo verify versions of other modules



#define CAN_E_PARAM_POINTER     0x01
#define CAN_E_PARAM_HANDLE     0x02
#define CAN_E_PARAM_DLC     	0x03
#define CAN_E_PARAM_CONTROLLER 0x04
// API service used without initialization
#define CAN_E_UNINIT           0x05
// Init transition for current mode
#define CAN_E_TRANSITION       0x06
#define CAN_E_DATALOST         0x07
struct CanController;

typedef const struct
{
	/// Can id for hoh, msb set if extended ID. ID will be overwritten in case of an hth
	Can_IdType canId;
	/// controller idx that this hoh is connected to
	uint8 controller;
	/// msgBox number used within the can controller for this hoh
	uint8 msgBox;
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

typedef struct
{
  /// Default baud rate and timing parameters for controller
  Can_ControllerBaudrateConfigType CanControllerBaudrateConfig;
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

typedef const struct Can_ConfigType
{
  CanController controller[CAN_NUM_CONTROLLERS];
  CanHardwareObject hrh[NUM_OF_HRHS];
  CanHardwareObject hth[NUM_OF_HTHS];
}/*__attribute__((section(".canpostbuild")))*/ Can_ConfigType;

void Can_Init( const Can_ConfigType *Config );
//void Can_DeInit(void);

#if ( CAN_VERSION_INFO_API == STD_ON )
#define Can_GetVersionInfo(_vi) STD_GET_VERSION_INFO(_vi,CAN)
#endif

void Can_InitController( uint8 controller, const Can_ControllerBaudrateConfigType *config);
Can_ReturnType Can_SetControllerMode( uint8 Controller, Can_StateTransitionType transition );
void Can_DisableControllerInterrupts( uint8 controller );
void Can_EnableControllerInterrupts( uint8 controller );
// no wakeup support, always return not ok
static inline Can_ReturnType Can_CheckWakeup( uint8 controller ) {return CAN_NOT_OK;}
Can_ReturnType Can_Write( Can_HwHandleType hth, Can_PduType *pduInfo );

//void Can_Cbk_CheckWakeup( uint8 controller );
/// no wakeup support from this controller
#define Can_MainFunction_Wakeup()
/// function used to poll mode change of CAN controller in case of timeout
void Can_MainFunction_Mode( void );

void Can_Arc_Isr(uint8 controller, uint8 msgBox);
void Can_Arc_IsrL(uint8 controller);
void Can_Arc_IsrH(uint8 controller);
void Can_ErrIsr(uint8 controller);

void Can_Arc_Write( Can_HwHandleType hth ); // called either from Can_MainFunction_Write or isr
void Can_Arc_Read( Can_HwHandleType hrh );  // called either from Can_MainFunction_Read or isr
void Can_Arc_BusOff( uint8 controller );

#endif // CAN_H_

