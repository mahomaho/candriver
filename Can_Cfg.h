/// Enable individual mask mode, not supported by MPC5554 and MPC5553, MPC5567?
#define CAN_ENABLE_INDIVIDUAL_MASK    STD_ON
/// set number of used msgboxes, max is 64
#define CAN_NUM_MSGBOXES			  64
/// Enable mixed mode hth's
#define CAN_ENABLE_MIXED_MODE		  STD_ON
/** Enable Development Error Trace. */
#define CAN_DEV_ERROR_DETECT          STD_ON
/** Build version info API. */
#define CAN_VERSION_INFO_API          STD_ON
/** Not supported. */
#define CAN_MULTIPLEXED_TRANSMISSION  STD_ON
/** Not supported. */
#define CAN_WAKEUP_SUPPORT            STD_OFF
/** Not supported. */
#define CAN_HW_TRANSMIT_CANCELLATION   STD_OFF
#define CAN_IDENTICAL_ID_CANCELLATION  STD_OFF
/** Not supported. */
#define CAN_TIMEOUT_DURATION          100

/** Bus off handling. Polling not supported */
#define CAN_BUSOFF_PROCESSING         INTERRUPT  // INTERRUPT/POLLING
/** Rx handling. Polling not supported. */
#define CAN_RX_PROCESSING             INTERRUPT
/** Tx handling. Polling not supported. */
#define CAN_TX_PROCESSING             INTERRUPT
/** Wakeup handling. Polling not supported. */
#define CAN_WAKEUP_PROCESSING         INTERRUPT

void Can_Arc_Write( Can_HwHandleType hth ); // called either from Can_MainFunction_Write or isr
void Can_Arc_Read( Can_HwHandleType hrh );  // called either from Can_MainFunction_Read or isr
void Can_Arc_BusOff( uint8 controller );	// called either from Can_MainFunction_BusOff or isr
void Can_Arc_Wakeup( uint8 controller );// called either from Can_MainFunction_Wakeup or isr

/*// HTH definitions
// Due to effiency: Start with index 0 and don't use any holes in the enumeration
typedef enum {
  CAN_HTH_A_1 = 0,
  CAN_HTH_C_1,
  NUM_OF_HTHS
} Can_Arc_HTHType;

// HRH definitions
// Due to effiency: Start with index 0 and don't use any holes in the enumeration
typedef enum {
  CAN_HRH_A_1 = 0,
  CAN_HRH_C_1,
  NUM_OF_HRHS
} Can_Arc_HRHType;
*/
typedef enum {
	CAN_A,
	CAN_C,
	CAN_D,
	CAN_NUM_CONTROLLERS
}Can_Arc_ControllerIdType;

typedef enum {
	CAN_HTH_A_1 = 0,
	CAN_HTH_C_1,
	CAN_HRH_A_1,
	CAN_HRH_C_1,
	NUM_OF_HOHS
} Can_Arc_HwHandleType;

static inline void Can_MainFunction_Write( void )
{
	// add all hth without isr
}

static inline void Can_MainFunction_Read( void )
{
	// add all hrh without isr
}

static inline void Can_MainFunction_BusOff( void )
{
	// add all controllers without isr
}

static inline void Can_MainFunction_Wakeup( void )
{
	// add all controllers without isr with support for wakeup
}

