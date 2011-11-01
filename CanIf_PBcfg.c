#include "CanIf_Types.h"

const CanIf_HrHConfigType canIfHrhCfg0[] = {
  [0].arrayLen = 0,
  [0].pduInfo.lpduId = CANIF_RX_LPDU_ID_CanDB_Message_1
};

const CanIf_ConfigType CanIf_config = {
  .canIfHrhCfg[0] = canIfHrhCfg0
};
