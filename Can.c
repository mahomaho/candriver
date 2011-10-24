#include "Can.h"
#include "CanIf_Cbk.h"
///todo implement support for MemMap.h
//#include "MemMap.h"

#if ( CAN_DEV_ERROR_DETECT == STD_ON )
#include "Det.h"
#define VALIDATE(_exp,_api,_err ) \
        if( !(_exp) ) { \
          Det_ReportError(MODULE_ID_CAN,0,_api,_err); \
          return CAN_NOT_OK; \
        }

#define VALIDATE_NO_RV(_exp,_api,_err ) \
        if( !(_exp) ) { \
          Det_ReportError(MODULE_ID_CAN,0,_api,_err); \
          return; \
        }

#define DET_REPORTERROR(_x,_y,_z,_q) Det_ReportError(_x, _y, _z, _q)
#else
#define VALIDATE(_exp,_api,_err )
#define VALIDATE_NO_RV(_exp,_api,_err )
#define DET_REPORTERROR(_x,_y,_z,_q)
#endif


/** HW object Can type. Full not supported. */
typedef enum {
  CAN_UNINIT,
  CAN_READY
} Can_DriverState;

  typedef volatile struct FlexCan {
        union {
            uint32_t R;
            struct MCR {
                uint32_t MDIS:1;
                uint32_t FRZ:1;
                uint32_t FEN:1;
                uint32_t HALT:1;
                uint32_t NOTRDY:1;
                uint32_t WAKMSK:1;
                uint32_t SOFTRST:1;
                uint32_t FRZACK:1;
                uint32_t SUPV:1;
                uint32_t SLFWAK:1;
                uint32_t WRNEN:1;
                uint32_t LPMACK:1;
                uint32_t WAKSRC:1;
                uint32_t DOZE:1;
                uint32_t SRXDIS:1;
                uint32_t BCC:1;
                  uint32_t:2;
                uint32_t LPRIO_EN:1;
                uint32_t AEN:1;
                  uint32_t:2;
                uint32_t IDAM:2;
                  uint32_t:2;
                uint32_t MAXMB:6;
            } B;
        } MCR;                  /* Module Configuration Register */

        union {
            uint32_t R;
            struct CR {
                uint32_t PRESDIV:8;
                uint32_t RJW:2;
                uint32_t PSEG1:3;
                uint32_t PSEG2:3;
                uint32_t BOFFMSK:1;
                uint32_t ERRMSK:1;
                uint32_t CLKSRC:1;
                uint32_t LPB:1;
                uint32_t TWRNMSK:1;
                uint32_t RWRNMSK:1;
                  uint32_t:2;
                uint32_t SMP:1;
                uint32_t BOFFREC:1;
                uint32_t TSYN:1;
                uint32_t LBUF:1;
                uint32_t LOM:1;
                uint32_t PROPSEG:3;
            } B;
        } CR;                 /* Control Register */

        union {
            uint32_t R;
        } TIMER;                /* Free Running Timer */

        uint32_t FLEXCAN_reserved1;

        union {
            uint32_t R;
            struct {
                uint32_t MI:32;
            } B;
        } RXGMASK;              /* RX Global Mask */

        union {
            uint32_t R;
            struct {
                uint32_t MI:32;
            } B;
        } RX14MASK;             /* RX 14 Mask */

        union {
            uint32_t R;
            struct {
                uint32_t MI:32;
            } B;
        } RX15MASK;             /* RX 15 Mask */

        union {
            uint32_t R;
            struct {
                uint32_t:16;
                uint32_t RXECNT:8;
                uint32_t TXECNT:8;
            } B;
        } ECR;                  /* Error Counter Register */

        union {
            uint32_t R;
            struct ESR{
                uint32_t:14;
                uint32_t TWRNINT:1;
                uint32_t RWRNINT:1;
                uint32_t BIT1ERR:1;
                uint32_t BIT0ERR:1;
                uint32_t ACKERR:1;
                uint32_t CRCERR:1;
                uint32_t FRMERR:1;
                uint32_t STFERR:1;
                uint32_t TXWRN:1;
                uint32_t RXWRN:1;
                uint32_t IDLE:1;
                uint32_t TXRX:1;
                uint32_t FLTCONF:2;
                  uint32_t:1;
                uint32_t BOFFINT:1;
                uint32_t ERRINT:1;
                uint32_t WAKINT:1;
            } B;
        } ESR;                  /* Error and Status Register */

        union {
            uint32_t R;
            struct {
                uint32_t BUF63M:1;
                uint32_t BUF62M:1;
                uint32_t BUF61M:1;
                uint32_t BUF60M:1;
                uint32_t BUF59M:1;
                uint32_t BUF58M:1;
                uint32_t BUF57M:1;
                uint32_t BUF56M:1;
                uint32_t BUF55M:1;
                uint32_t BUF54M:1;
                uint32_t BUF53M:1;
                uint32_t BUF52M:1;
                uint32_t BUF51M:1;
                uint32_t BUF50M:1;
                uint32_t BUF49M:1;
                uint32_t BUF48M:1;
                uint32_t BUF47M:1;
                uint32_t BUF46M:1;
                uint32_t BUF45M:1;
                uint32_t BUF44M:1;
                uint32_t BUF43M:1;
                uint32_t BUF42M:1;
                uint32_t BUF41M:1;
                uint32_t BUF40M:1;
                uint32_t BUF39M:1;
                uint32_t BUF38M:1;
                uint32_t BUF37M:1;
                uint32_t BUF36M:1;
                uint32_t BUF35M:1;
                uint32_t BUF34M:1;
                uint32_t BUF33M:1;
                uint32_t BUF32M:1;
            } B;
        } IMRH;               /* Interruput Masks Register */

        union {
            uint32_t R;
            struct {
                uint32_t BUF31M:1;
                uint32_t BUF30M:1;
                uint32_t BUF29M:1;
                uint32_t BUF28M:1;
                uint32_t BUF27M:1;
                uint32_t BUF26M:1;
                uint32_t BUF25M:1;
                uint32_t BUF24M:1;
                uint32_t BUF23M:1;
                uint32_t BUF22M:1;
                uint32_t BUF21M:1;
                uint32_t BUF20M:1;
                uint32_t BUF19M:1;
                uint32_t BUF18M:1;
                uint32_t BUF17M:1;
                uint32_t BUF16M:1;
                uint32_t BUF15M:1;
                uint32_t BUF14M:1;
                uint32_t BUF13M:1;
                uint32_t BUF12M:1;
                uint32_t BUF11M:1;
                uint32_t BUF10M:1;
                uint32_t BUF09M:1;
                uint32_t BUF08M:1;
                uint32_t BUF07M:1;
                uint32_t BUF06M:1;
                uint32_t BUF05M:1;
                uint32_t BUF04M:1;
                uint32_t BUF03M:1;
                uint32_t BUF02M:1;
                uint32_t BUF01M:1;
                uint32_t BUF00M:1;
            } B;
        } IMRL;               /* Interruput Masks Register */

        union {
            uint32_t R;
            struct {
                uint32_t BUF63I:1;
                uint32_t BUF62I:1;
                uint32_t BUF61I:1;
                uint32_t BUF60I:1;
                uint32_t BUF59I:1;
                uint32_t BUF58I:1;
                uint32_t BUF57I:1;
                uint32_t BUF56I:1;
                uint32_t BUF55I:1;
                uint32_t BUF54I:1;
                uint32_t BUF53I:1;
                uint32_t BUF52I:1;
                uint32_t BUF51I:1;
                uint32_t BUF50I:1;
                uint32_t BUF49I:1;
                uint32_t BUF48I:1;
                uint32_t BUF47I:1;
                uint32_t BUF46I:1;
                uint32_t BUF45I:1;
                uint32_t BUF44I:1;
                uint32_t BUF43I:1;
                uint32_t BUF42I:1;
                uint32_t BUF41I:1;
                uint32_t BUF40I:1;
                uint32_t BUF39I:1;
                uint32_t BUF38I:1;
                uint32_t BUF37I:1;
                uint32_t BUF36I:1;
                uint32_t BUF35I:1;
                uint32_t BUF34I:1;
                uint32_t BUF33I:1;
                uint32_t BUF32I:1;
            } B;
        } IFRH;               /* Interruput Flag Register */

        union {
            uint32_t R;
            struct {
                uint32_t BUF31I:1;
                uint32_t BUF30I:1;
                uint32_t BUF29I:1;
                uint32_t BUF28I:1;
                uint32_t BUF27I:1;
                uint32_t BUF26I:1;
                uint32_t BUF25I:1;
                uint32_t BUF24I:1;
                uint32_t BUF23I:1;
                uint32_t BUF22I:1;
                uint32_t BUF21I:1;
                uint32_t BUF20I:1;
                uint32_t BUF19I:1;
                uint32_t BUF18I:1;
                uint32_t BUF17I:1;
                uint32_t BUF16I:1;
                uint32_t BUF15I:1;
                uint32_t BUF14I:1;
                uint32_t BUF13I:1;
                uint32_t BUF12I:1;
                uint32_t BUF11I:1;
                uint32_t BUF10I:1;
                uint32_t BUF09I:1;
                uint32_t BUF08I:1;
                uint32_t BUF07I:1;
                uint32_t BUF06I:1;
                uint32_t BUF05I:1;
                uint32_t BUF04I:1;
                uint32_t BUF03I:1;
                uint32_t BUF02I:1;
                uint32_t BUF01I:1;
                uint32_t BUF00I:1;
            } B;
        } IFRL;               /* Interruput Flag Register */

        uint32_t FLEXCAN_reserved2[19];

        struct canbuf_t {
            union {
                uint32_t R;
                struct CS{
                    uint32_t:4;
                    uint32_t CODE:4;
                      uint32_t:1;
                    uint32_t SRR:1;
                    uint32_t IDE:1;
                    uint32_t RTR:1;
                    uint32_t LENGTH:4;
                    uint32_t TIMESTAMP:16;
                } B;
            } CS;

            union {
                uint32_t R;
                struct {
                    uint32_t PRIO:3;
                    uint32_t STD_ID:11;
                    uint32_t EXT_ID:18;
                } B;
            } ID;

            union {
                uint8_t  B[8]; /* Data buffer in Bytes (8 bits) */
                uint16_t H[4]; /* Data buffer in Half-words (16 bits) */
                uint32_t W[2]; /* Data buffer in words (32 bits) */
                /* uint32_t R[2];    Data buffer in words (32 bits) */
            } DATA;

        } BUF[64];

        uint32_t FLEXCAN_reserved3[256];

        union {
            uint32_t R;
            struct {
                uint32_t MI:32;
            } B;
        } RXIMR[64];            /* RX Individual Mask Registers */

    } FlexCanT;                          /* end of CTU_tag */


static const Can_ConfigType *Can_ConfigPtr = 0;
static uint8 CanIsrDisableCnt[CAN_NUM_CONTROLLERS] = 0;
static union {PduIdType id; Can_HwHandleType handle} controllerData[CAN_NUM_MSGBOXES][CAN_NUM_CONTROLLERS];

// id 0
void Can_Init( const Can_ConfigType *config )
{
  VALIDATE_NO_RV(Can_ConfigPtr == 0, 0, CAN_E_TRANSITION);
  VALIDATE_NO_RV(config != 0, 0, CAN_E_PARAM_POINTER);
  Can_ConfigPtr = config;
  // initialize each controller
  for(int i = 0; i < CAN_NUM_CONTROLLERS; i++)
  {
	// init controller and baud rate
    Can_InitController(i, Config->controller[i].CanControllerBaudrateConfig);
    FlexCanT *regs = CAN_CONTROLLER_BASE_ADDRESS[i];
    // init mask registers
#if CAN_ENABLE_INDIVIDUAL_MASK
    for(int j = 0; j < CAN_NUM_MSGBOXES; j++){
	  regs->RXIMR[j].R = Config->controller[i].CanFilterMask[j];
	}
#else
	regs->RXGMASK.R = Config->controller[i].CanGlobalFilterMask;
	regs->RX14MASK.R = Config->controller[i].Can14FilterMask;
	regs->RX15MASK.R = Config->controller[i].Can15FilterMask;
#endif
    // init ISR registers
	regs->IFRL = 0xFFFFFFFF;
	regs->IFRH = 0xFFFFFFFF;
//	regs->IMRL = Config->controller[i].rxisrmaskL | Config->controller[i].txisrmaskL;
  #if CAN_NUM_MSGBOXES > 32
//	regs->IMRH = Config->controller[i].rxisrmaskH | Config->controller[i].txisrmaskH;
  #endif
	
  }
  // initialize the hrh's
  for(int i = 0; i < CAN_NUM_HRH; i++) {
    FlexCanT *regs = CAN_CONTROLLER_BASE_ADDRESS[Config->hrh[i].controller];
	bool IDE = (Config->hrh[i].id & 0x80000000) != 0;
	uint32_t id = IDE? Config->hrh[i].id : Config->hrh[i].id << 18;
	regs->BUF[Config->hrh[i].msgBox].ID.R = id;
	// set CODE to 6 for active RX buffer
	struct CS cs = {.IDE = IDE, .CODE = 0x6};
	regs->BUF[Config->hrh[i].msgBox].CS.R = cs;
	// set handle to hrh handle id to be used in rx callback
	controllerData[Config->hrh[i].msgBox][Config->hrh[i].controller].handle = i;
  }
  // ... and the hth's
  for(int i = 0; i < CAN_NUM_HTH; i++) {
    FlexCanT *regs = CAN_CONTROLLER_BASE_ADDRESS[Config->hth[i].controller];
	bool IDE = (Config->hth[i].id & 0x80000000) != 0;
	uint32_t id = IDE? Config->hth[i].id : Config->hth[i].id << 18;
	regs->BUF[Config->hth[i].msgBox].ID.R = id;
	// set code to 1000 for idle tx buffer
	struct CS cs = {.IDE = IDE, .CODE = 0x8, .SRR = 1};
	regs->BUF[Config->hth[i].msgBox].CS.R = cs;
	// set id to -1 to indicate empty tx buffer
	controllerData[Config->hth[i].msgBox][Config->hth[i].controller].id = -1;
  }
  
}

// service id 2
void Can_InitController( uint8 controller, const Can_ControllerBaudrateConfigType *config)
{
  VALIDATE_NO_RV(Can_ConfigPtr != 0, 2, CAN_E_UNINIT);
  VALIDATE_NO_RV(config != 0, 2, CAN_E_PARAM_POINTER);
  VALIDATE_NO_RV(controller < CAN_NUM_CONTROLLERS, 2, CAN_E_PARAM_CONTROLLER);
  // validate stoped mode
  FlexCanT *regs = CAN_CONTROLLER_BASE_ADDRESS[controller];
  VALIDATE_NO_RV(regs->MCR.FRZ == 1 &&
    regs->MCR.MDIS == 0, 2, CAN_E_TRANSITION);
  // init controller by setting STOPPED mode
  Can_SetControllerMode(controller, CAN_T_STOP);
  // set timing parameters in CR register
  const struct CR cr = {
    .PSEG1 = config->CanControllerSeg1,
	.PSEG2 = config->CanControllerSeg2,
	.PROPSEG = config->CanControllerPropSeg,
	.RJW = config->CanControllerSyncJumpWidth,
	.PRESDIV = *CAN_CPU_CLOCK_REFERENCE / (
		(config->CanControllerSeg1 + config->CanControllerSeg2 + config->CanControllerPropSeg + 4) *
			config->CanControllerBaudRate * 1000
		) - 1,
	// one sample mode, according to ISO11898? Filtering is handled by the tranceiver
	.SMP = 0,
	// set clock source to oscillator, necessary to get enough stable clock source
	.CLKSRC = 0,
	// enable busoff interrupt if processing is interrupt
	.BOFFMSK = (CAN_BUSOFF_PROCESSING == INTERRUPT),
	// disable automatic bussoff recovery, according to requirement BSW01060
	.BOFFREC = 1,
	// do not enable error frame interrupt
	///todo make this configurable and connect interrupt to DEM error report?
	.ERRMSK = 0
  };
  regs->CR.B = cr;
}

// service id 3
Can_ReturnType Can_SetControllerMode( uint8 controller, Can_StateTransitionType transition )
{
  //validate not in CAN_UNINIT mode
  VALIDATE(Can_ConfigPtr != 0, 3, CAN_E_UNINIT);
  VALIDATE(controller < CAN_NUM_CONTROLLERS, 3, CAN_E_PARAM_CONTROLLER);
  FlexCanT *regs = CAN_CONTROLLER_BASE_ADDRESS[controller];
  // controller mode:
  // STOPPED = freeze mode
  // SLEEP = disabled
  // STARTED = enabled and not in freeze mode
  // UNINIT = Can_ConfigPtr == 0
  struct MCR mcr = {
    MDIS = 0, // enable module
    FRZ = 1,  // enable freeze mode
    HALT = 1, // ... and set it
    SRXDIS = 1, // disalbe self reception
    BCC = CAN_ENABLE_INDIVIDUAL_MASK, // control backwards compatibility
    AEN = 1, // enable abort of tx msgs without isr loss
    MAXMB = 63}; // enable 64 message buffers
  switch(transition)
  {
  case CAN_T_START:
    // validate stopped mode
    VALIDATE(regs->MCR.B.FRZ == 1 &&
	  regs->MCR.B.MDIS == 0, 3, CAN_E_TRANSITION);
	// set start mode
	mcr.FRZ = 0;
	// clear eventual pending tx requests, disable interrupts to ensure no simultaneous transmit
	bool lock = LockSave();
	for(int i = 0; i < CAN_NUM_MSGBOXES; i++) {
		if(regs->BUF[i].CS.B.CODE == 0xC) {
			// pending tx request, clear it and set id to -1
			regs->BUF[i].CS.B.CODE = 0x8;
			controllerData[i][controller].id = -1;
		}
	}
	LockRestore(lock);
	// do the mode change
	regs->MCR.B = mcr;
	// no delayed state transition, call mode indication directly
	CanIf_ControllerModeIndication(controller, CANIF_CS_STARTED);
	// enable the controller interrupts
	Can_EnableControllerInterrupts(controller);
    break;
  case CAN_T_STOP:
	// it isalways ok to set stop mode, even in stoped mode
	if(regs->MCR.B.FRZ == 0) {
		// entering stoped mode from started mode, disable interrupts again
		// disable interrupts in stoped mode since no isrs are allowed
	  	Can_DisableControllerInterrupts(controller);
	}
	// set max time for state transition
	int32 timeout = GetCounterValue() + CAN_TIMEOUT_DURATION;
	regs->MCR.B = mcr;
	do {
	  if(regs->MCR.B.) {
	    // state has changed, report to CanIf and return
		CanIf_ControllerModeIndication(controller, CANIF_CS_STOPPED);
		return;
	  }
	} while(GetCounterValue() < timeout);
	// failed to reach stoped mode before timeout, set flag to inform Can_MainFunction_Mode to continue to poll flag
	todo
    break;
  case CAN_T_SLEEP:
    // validate stopped mode
    VALIDATE(regs->MCR.B.FRZ == 1 &&
	  regs->MCR.B.MDIS == 0, 3, CAN_E_TRANSITION);
    mcr.MDIS = 1;
	// set max time for state transition
	int32 timeout = GetCounterValue() + CAN_TIMEOUT_DURATION;
	regs->MCR.B = mcr;
	do {
	  if(regs->MCR.B.) {
	    // state has changed, report to CanIf and return
		CanIf_ControllerModeIndication(controller, CANIF_CS_SLEEP);
		return;
	  }
	} while(GetCounterValue() < timeout);
	// failed to reach sleep mode before timeout, set flag to inform Can_MainFunction_Mode to continue to poll flag
	todo
    break;
  case CAN_T_WAKEUP:
    // validate sleep mode
    VALIDATE(regs->MCR.B.MDIS == 1, 3, CAN_E_TRANSITION);
	regs->MCR.B = mcr;
	// no delayed state transition, call mode indication directly
	CanIf_ControllerModeIndication(controller, CANIF_CS_STOPPED);
    break;
  }
}

// service id 4
void Can_DisableControllerInterrupts( uint8 controller ) {
    VALIDATE_NO_RV(Can_ConfigPtr != 0, 4, CAN_E_UNINIT);
    VALIDATE_NO_RV(controller < CAN_NUM_CONTROLLERS, 4, CAN_E_PARAM_CONTROLLER);
    FlexCanT *regs = CAN_CONTROLLER_BASE_ADDRESS[controller];
	bool lock = LockSave();
	if(CanIsrDisableCnt[controller]++ == 0) {
		// disable busOff isr
		regs->CR.BBOFFMSK = 0;
		// err isr aren't enabled
		// disable msgBox isrs
		regs->IMRL = 0;
#if CAN_NUM_MSGBOXES > 32
		regs->IMRH = 0;
#endif
	}
	LockRestore(lock);
}
 
// service id 5
void Can_EnableControllerInterrupts( uint8 controller ) {
    VALIDATE_NO_RV(Can_ConfigPtr != 0, 5, CAN_E_UNINIT);
    VALIDATE_NO_RV(controller < CAN_NUM_CONTROLLERS, 5, CAN_E_PARAM_CONTROLLER);
    FlexCanT *regs = CAN_CONTROLLER_BASE_ADDRESS[controller];
	bool lock = LockSave();
	if(CanIsrDisableCnt[controller] == 0) {
		// isrs are already enabled, do nothing
	} else if(--CanIsrDisableCnt[controller] == 0) {
		// enable busOff isr
		regs->CR.BBOFFMSK = (CAN_BUSOFF_PROCESSING == INTERRUPT);
		// err isr aren't enabled
		// enable msgBox isrs
		regs->IMRL = Can_ConfigPtr->controller[i].rxisrmaskL | Can_ConfigPtr->controller[i].txisrmaskL;
#if CAN_NUM_MSGBOXES > 32
		regs->IMRH = Can_ConfigPtr->controller[i].rxisrmaskH | Can_ConfigPtr->controller[i].txisrmaskH;
#endif
	}
	LockRestore(lock);
}

// service id 6
Can_ReturnType Can_Write( Can_HwHandleType hth, Can_PduType *pduInfo )
{
	//validate not in CAN_UNINIT mode
	VALIDATE(Can_ConfigPtr != 0, 6, CAN_E_UNINIT);
	VALIDATE(hth < NUM_OF_HTHS &&
		Can_ConfigPtr->hth[hth].controller].canObjectType == CAN_OBJECT_TYPE_TRANSMIT, 6, CAN_E_PARAM_HANDLE);
	VALIDATE(pduInfo != 0, 6, CAN_E_PARAM_POINTER);
	VALIDATE(pduInfo->len <= 8, 6, CAN_E_PARAM_DLC);
	bool IDE = (pduInfo->ID & 0x80000000) != 0;
	uint32_t id = ((IDE)? pduInfo->ID : pduInfo->ID << 18);  
	struct CS cs = {
		.CODE = 0xC,
		.SRR = 1, // according to CAN standard
		.IDE = IDE,
		.LENGTH = pduInfo->len};
	FlexCanT *regs = CAN_CONTROLLER_BASE_ADDRESS[Can_ConfigPtr->hth[hth].controller];
#if CAN_HW_TRANSMIT_CANCELLATION
	Can_IdType lowestPrioId = -1;
	loPrioIdx = 0;
#endif
	bool lock = LockSave();
	for(int i = 0; i < (CAN_MULTIPLEXED_TRANSMISSION? Can_ConfigPtr->hth[hth].numMultiplexed : 1); i++) {
		uint8_t msgBox = Can_ConfigPtr->hth[hth].msgBox + i;
		// check if empty
		if(controllerData[msgBox][Can_ConfigPtr->hth[hth].controller].id != -1) {
#if CAN_HW_TRANSMIT_CANCELLATION
			if(regs->BUF[msgBox].ID.R ((CAN_IDENTICAL_ID_CANCELLATION)? <= : < ) lowestPrioId) {
				// cancel ongoing transmission
				lowestPrioId = regs->BUF[msgBox].ID.R;
				loPrioIdx = i;
			}
#endif
		}else {
			controllerData[msgBox][Can_ConfigPtr->hth[hth].controller].id = pduInfo->pduId;
			// fill in the msgBox
			memcpy(regs->BUF[msgBox].DATA.B, pduInfo->sdu, 8);
			regs->BUF[msgBox].ID.R = id;
			regs->BUF[msgBox].CS = cs;
			// restore interrupt when buffer access done
			LockRestore(lock);
			return CAN_OK;
		}
	}
	// 
#if CAN_HW_TRANSMIT_CANCELLATION
	// cancel lowest prio id if prio lower than id
	if(id > lowestPrioId) {
		regs->BUF[loPrioIdx].CS.B.CODE = 0x9;
	}
#endif
	LockRestore(lock);
	return  CAN_BUSY;
}

static void Isr(uint8 controller, uint8 msgBox, FlexCanT *regs) {
  struct CS cs;
  do {
	cs = regs->BUF[msgBox].CS.B;
  }while((cs & 0x9) == 0x1); // msg box busy updating with rx data
  switch(cs.CODE) {
  case 0x6:
	// buffer overrun, report to DET and continue without break
	DET_REPORTERROR(MODULE_ID_CAN, 0, 8, CAN_E_DATALOST);
  case 0x2: {
    // message received, send to callback
	Can_IdType id = (cs.IDE)? regs->BUF[msgBox].ID.R + 0x80000000 : regs->BUF[msgBox].ID.B.STD_ID;
#if defined CAN_LPDU_RX_CB
	if(CAN_LPDU_RX_CB(controllerData[msgBox][controller].handle, id, cs.LENGTH, regs->BUF[msgBox].DATA.B) == false) {
		// no further processing of message, return
		return;
	}
#endif
	CanIf_RxIndication(controllerData[msgBox][controller].handle, id, cs.LENGTH, regs->BUF[msgBox].DATA.B);
	// read timer reg to release lock of msg box
	uint32_t timer = regs->TIMER;
	break;
  case 0x8:
    // message transmitted
	CanIf_TxConfirmation(controllerData[msgBox][controller].id);
	// set PDU to -1 to indicate empty msgbox
	controllerData[msgBox][controller].id = -1;
	break;
#if CAN_HW_TRANSMIT_CANCELLATION
  case 0x9:
	// canceled transmission, call callback
	Can_PduType pduInfo = {
	  .ID = (cs.IDE)? regs->BUF[msgBox].ID.R + 0x80000000 : regs->BUF[msgBox].ID.B.STD_ID,
	  .DATA = regs->BUF[msgBox].DATA.B, 
	  .PDUID = controllerData[msgBox][controller].id, 
	  .LEN = cs.LENGTH
	};
	// set PDU to -1 to indicate empty msgbox
	controllerData[msgBox][controller].id = -1;
	CanIf_CancelTxConfirmation(&pduInfo);
	break;
#endif
  default:
    // not suppoted code, report to DET
  }
}

void Can_Arc_Isr(uint8 controller, uint8 msgBox) {
  FlexCanT *regs = CAN_CONTROLLER_BASE_ADDRESS[controller];
  // clear flag before handling of message, necessary to not miss an interrupt when receiving a new frame
  regs->IFRL = 1 << msgBox;
  Isr(controller, msgBox, regs);
}

void Can_Arc_IsrL((uint8 controller) {
  FlexCanT *regs = CAN_CONTROLLER_BASE_ADDRESS[controller];
  uint32_t ifr = regs->IFRL;
  regs->IFRL = ifr;
  int8_t msgBox;
  while((msgBox = 31 - CountLeadingZeroes(ifr)) >= 0) {
    // serve the messageBox
	Isr(controller, msgBox, regs);
  }
}

void Can_Arc_IsrH((uint8 controller) {
  FlexCanT *regs = CAN_CONTROLLER_BASE_ADDRESS[controller];
  uint32_t ifr = regs->IFRH;
  regs->IFRH = ifr;
  int8_t msgBox;
  while((msgBox = 63 - CountLeadingZeroes(ifr)) >= 32) {
    // serve the messageBox
	Isr(controller, msgBox, regs);
  }
}

// called from can_mainfunction_write, service id 1
void Can_Arc_MainFunction_Write( uint8 controller ) {
  VALIDATE_NO_RV(Can_ConfigPtr != 0, 1, CAN_E_UNINIT);
  FlexCanT *regs = CAN_CONTROLLER_BASE_ADDRESS[controller];
  uint32_t ifr = regs->IFRL & Can_ConfigPtr->controller[Controller].txisrmask;
  regs->IFRL = ifr;
  int8_t msgBox;
  while((msgBox = 31 - CountLeadingZeroes(ifr)) >= 0) {
    // serve the messageBox
	Isr(controller, msgBox, regs);
  }
#if CAN_NUM_MSGBOXES > 32
  ifr = regs->IFRH;
  regs->IFRH = ifr & Can_ConfigPtr->controller[Controller].txisrmaskH;
  int8_t msgBox;
  while((msgBox = 63 - CountLeadingZeroes(ifr)) >= 32) {
    // serve the messageBox
	Isr(controller, msgBox, regs);
  }
#endif
}

// called from can_mainfunction_read, service id 8
void Can_Arc_MainFunction_Read( uint8 controller ) {
  VALIDATE_NO_RV(Can_ConfigPtr != 0, 8, CAN_E_UNINIT);
  FlexCanT *regs = CAN_CONTROLLER_BASE_ADDRESS[controller];
  uint32_t ifr = regs->IFRL & Can_ConfigPtr->controller[Controller].rxisrmask;
  regs->IFRL = ifr;
  int8_t msgBox;
  while((msgBox = 31 - CountLeadingZeroes(ifr)) >= 0) {
    // serve the messageBox
	Isr(controller, msgBox, regs);
  }
#if CAN_NUM_MSGBOXES > 32
  ifr = regs->IFRH;
  regs->IFRH = ifr & Can_ConfigPtr->controller[Controller].rxisrmaskH;
  int8_t msgBox;
  while((msgBox = 63 - CountLeadingZeroes(ifr)) >= 32) {
    // serve the messageBox
	Isr(controller, msgBox, regs);
  }
#endif
}

// called from can_mainfunction_BusOff, service id 9
void Can_Arc_BusOff( uint8 controller ) {
  VALIDATE_NO_RV(Can_ConfigPtr != 0, 9, CAN_E_UNINIT);
  FlexCanT *regs = CAN_CONTROLLER_BASE_ADDRESS[controller];
  struct ESR esr = regs->ESR.B
  if(regs->ESR.B.BOFFINT) {
    // busOff, set stopped mode
	Can_SetControllerMode(controller, CAN_T_STOP);
	// clear bus off flag
	struct ESR esr = {.BOFFINT = 1};
	regs->ESR.B = esr;
	// call callback
	CanIf_ControllerBusOff(controller);
  }
}

// service id 12
void Can_MainFunction_Mode(void) {
  VALIDATE_NO_RV(Can_ConfigPtr != 0, 12, CAN_E_UNINIT);	
}

void Can_ErrIsr(uint8 controller) {
	// only busoff error interrupts are enabled, call the busoff handle function
	Can_Arc_BusOff(controller);
}
