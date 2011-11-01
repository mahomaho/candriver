/// Enable individual mask mode, not supported by MPC5554 and MPC5553, MPC5567?
#define CAN_ENABLE_INDIVIDUAL_MASK    STD_ON
/// set number of used msgboxes, max is 64
#define CAN_NUM_MSGBOXES			        64
/** Enable Development Error Trace. */
#define CAN_DEV_ERROR_DETECT          STD_OFF
/** Build version info API. */
#define CAN_VERSION_INFO_API          STD_ON
/// Enable multiplexed transmission, multiple tx msgboxes per hth
#define CAN_MULTIPLEXED_TRANSMISSION  STD_OFF // not supported, fifo must be implemented to support multiplexed transmission, see CAN402
/// Enable cancelation of tx msgs with lower prio
#define CAN_HW_TRANSMIT_CANCELLATION   STD_ON
///  Cancellation also cancels equal prio
#define CAN_IDENTICAL_ID_CANCELLATION  STD_OFF
/// Max busy wait time for CAN state transition [CAN_OS_COUNTER ticks]
#define CAN_TIMEOUT_DURATION          100
/// OS counter used for timeout calculation
#define CAN_OS_COUNTER              0
/// general CAN RX callout function, do not define to disable
//#define CAN_LPDU_RX_CB                CanLPduReceiveCalloutFunction
/// reference clock freq used for can controller, should be set to oscillator clock freq value
#define CAN_CPU_CLOCK_REFERENCE		  8000000

/// base address for the can controller module
void Can_Arc_Write( Can_HwHandleType hth ); // called either from Can_MainFunction_Write or isr
void Can_Arc_Read( Can_HwHandleType hrh );  // called either from Can_MainFunction_Read or isr
void Can_Arc_BusOff( uint8 controller );	// called either from Can_MainFunction_BusOff or isr

typedef enum {
	FlexCan_CAN_A,
	FlexCan_CAN_C,
	FlexCan_CAN_D,
	CAN_NUM_CONTROLLERS
}Can_Arc_ControllerIdType;

typedef enum {
	FlexCan_HTH_A_1 = 0,
	FlexCan_HTH_C_1,
	NUM_OF_HTHS
} Can_Arc_HTHType;


typedef enum {
	FlexCan_HRH_A_1,
	FlexCan_HRH_C_1,
	NUM_OF_HRHS
} Can_Arc_HRHType;

struct Can_ConfigType;
//typedef const struct Can_ConfigType Can_ConfigType;
extern const struct Can_ConfigType Can_config;

struct FlexCan;

/// Can controller base address, see ref manual for correct value
static volatile struct FlexCan* const CAN_CONTROLLER_BASE_ADDRESS[CAN_NUM_CONTROLLERS] = {
        [FlexCan_CAN_A] = (volatile struct FlexCan*)0x12345678,
        [FlexCan_CAN_C] = (volatile struct FlexCan*)0x87654321,
        [FlexCan_CAN_D] = (volatile struct FlexCan*)0x11111111
};
	
void Can_Arc_MainFunction_Write( uint8 controller );
static inline void Can_MainFunction_Write( void )
{
	// add all controllers without isr enabled
  Can_Arc_MainFunction_Write(FlexCan_CAN_C);
}

void Can_Arc_MainFunction_Read( uint8 controller );
static inline void Can_MainFunction_Read( void )
{
	// add all controllers without isr
  Can_Arc_MainFunction_Read(FlexCan_CAN_C);
}

void Can_Arc_BusOff( uint8 controller );
static inline void Can_MainFunction_BusOff( void )
{
	// add all controllers without isr
  Can_Arc_BusOff(FlexCan_CAN_C);
}

