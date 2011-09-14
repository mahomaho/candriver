
// uint16: if only Standard IDs are used
// uint32: if also Extended IDs are used
typedef uint32 Can_IdType;

// uint16: if more than 255 hw handles
// uint8: else
typedef uint16 Can_HwHandleType;

typedef enum {
	CAN_T_START,
	CAN_T_STOP,
	CAN_T_SLEEP,
	CAN_T_WAKEUP
} Can_StateTransitionType;

typedef enum {
	CAN_OK,
	CAN_NOT_OK,
	CAN_BUSY
// 	CAN_WAKEUP,		// Removed in 3.0
} Can_ReturnType;
 
struct Can_PduType {
	// private data for CanIf,just save and use for callback
	PduIdType   swPduHandle;
	// the CAN ID, 29 or 11-bit
	Can_IdType 	id;
	// Length, max 8 bytes
	uint8		length;
	// data ptr
	uint8 		*sdu;
};