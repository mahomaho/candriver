#include "CanIf_Types.h"

const CanIf_HrHConfigType canIfHrhCfg0[] = {
  [0].arrayLen = 0,
  [0].pduInfo.lpduId = CANIF_PDU_ID_PDU_RX0
};

const CanIf_ConfigType CanIf_Config = {
  .canIfHrhCfg[0] = canIfHrhCfg0
};
