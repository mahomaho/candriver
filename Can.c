
#include "Can.h"

/** HW object Can type. Full not supported. */
typedef enum {
  CAN_UNINIT,
  CAN_READY
} Can_DriverState;

  typedef volatile struct  {
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
            struct {
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
                struct {
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

void Can_Init( const Can_ConfigType *Config )
{
  Can_ConfigPtr = Config;
  // initialize each controller
  for(int i = 0; i < CAN_NUM_CONTROLLERS; i++)
  {
    Can_InitController(i, Config->CanConfigSet.CanController[i].CanControllerBaudrateConfig);
    // init mask registers
#if CAN_ENABLE_INDIVIDUAL_MASK
    for(int j = 0; j < CAN_NUM_MSGBOXES; j++){
	  Config->CanConfigSet.CanController[i].CanControllerBaseAddress->RXIMR[j].R = Config->CanConfigSet.CanController[i].CanFilterMask[j];
	}
#else
	Config->CanConfigSet.CanController[i].CanControllerBaseAddress->RXGMASK.R = Config->CanConfigSet.CanController[i].CanGlobalFilterMask;
	Config->CanConfigSet.CanController[i].CanControllerBaseAddress->RX14MASK.R = Config->CanConfigSet.CanController[i].Can14FilterMask;
	Config->CanConfigSet.CanController[i].CanControllerBaseAddress->RX15MASK.R = Config->CanConfigSet.CanController[i].Can15FilterMask;
#endif
  }
  // initialize the hoh's
  for(int i = 0; i < CAN_NUM_HRH; i++) {
	Config->hrh[i].msgBox->id = Config->hrh[i].id;
  }
  for(int i = 0; i < CAN_NUM_HTH; i++) {
	Config->hth[i].msgBox.cfg = 1000;
  }
}

void Can_InitController( uint8 controller, const Can_ControllerBaudrateConfigType *config)
{
  // init controller by setting STOPPED mode
  Can_SetControllerMode(controller, CAN_T_STOP);
  // set timing parameters in CR register
  const struct CR cr = {
    .PSEG1 = config->CanControllerSeg1,
	.PSEG2 = config->CanControllerSeg2,
	.PROPSEG = config->CanControllerPropSeg,
	.RJW = config->CanControllerSyncJumpWidth,
	.PRESDIV = Config->CanConfigSet.CanController[i].CanControllerBaseAddress->CanCpuClock / (
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
  Config->CanConfigSet.CanController[i].CanControllerBaseAddress->CR.B = cr;
}


Can_ReturnType Can_SetControllerMode( uint8 controller, Can_StateTransitionType transition )
{
  // controller mode:
  // STOPPED = freeze mode
  // SLEEP = disabled
  // STARTED = enabled and not in freeze mode
  // UNINIT = STOPPED
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
	mcr.FRZ = 0;
	break;
  case CAN_T_STOP:
	break;
  case CAN_T_SLEEP:
    mcr.MDIS = 1;
	break;
  case CAN_T_WAKEUP:
    break;
  }
  Config->CanConfigSet.CanController[i].CanControllerBaseAddress->MCR.B = mcr;
}

Can_ReturnType Can_Write( Can_HwHandleType hth, Can_PduType *pduInfo )
{
  Can_ReturnType retVal = CAN_OK;
  bool lock = LockSave();
  uint32_t stat = Config->CanConfigSet.CanHardwareObject[hth].msgBox->
  if(stat == 0xC) {
    // msgbox busy. Check prio
	retVal = CAN_BUSY;
#if CAN_HW_TRANSMIT_CANCELLATION
#if CAN_IDENTICAL_ID_CANCELLATION
	if(id >= pduInfo->id) {
#else
	if(id > pduInfo->id) {
#endif
	  // current message has lower prio, cancel transmission
	   = 0xD;
	   if( == 0xD) {
	     // transmission canceled, call callback
	   }
	}
#endif
  } else {
    // msgbox ready for transmission, send message
	if(pduInfo->id & 0x80000000) {
	  // extended id
	  id = pduInfo->id & ~0x80000000;
	} else {
	  id = pduInfo->id << 18;
	}
	memcpy(data, pduInfo->sdu, 8);
	controllerData[msgBox].pduId = pduInfo->id;
	dlc = pduInfo->dlc;
  }
  LockRestore(lock);
  return retVal;
}

void Can_Isr(uint8 controller, uint8 msgBox)
{
  FlexCanT *regs = (FlexCanT*)Config->CanConfigSet.CanController[Controller].CanControllerBaseAddress;
  Config->CanConfigSet.CanController[i].
}
void Can_Isr(flexCanT *controller, MsgBoxT *msgBox

void Can_Arc_Write( Can_HwHandleType hth )
{
	for(int i = 0;
#if CAN_MULTIPLEXED_TRANSMISSION
		i < Can_ConfigPtr->hoh[hth].numMultiplexed;
#else
		i < 1;
#endif
		i++)
	int i = 0;
	{
		if(Can_ConfigPtr->hoh[hth].msgBox[i].reg) // check if msg sent
		{
			// msg sent, call the callback to inform sw
			hthData[hth]
		}
	}
}

void Can_Arc_BusOff( uint8 controller )
{
	if(controlleraddress[controller]->busOffreg)
	{
		// busOff, call callback
	}
}