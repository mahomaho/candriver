#if 0
const CanIf_DispatchCfgType canIfDispatchCfg = {
}

#if CANIF_PUBLIC_MULTIPLE_DRV_SUPPORT
///todo implement
const CanIf_DriverUnitConfigType driverUnitConfig[] = {
  [0].numHth = ,
  [0].numHrh = ,  
};
#endif

extern const CanIf_HrHConfigType canIfHrhCfg[];

void CanIf_RxIndication(Can_HwHandleType hrh, Can_IdType canId, uint8 canDlc, const uint8* canSduPtr) {
	// function callbed by CAN controller. Different names for different driver units
  ///todo instead of accessing canIfHrhCfg directly it should be accessed via the root pbcfg object. Solve this by passing driverUnit instead of hrh
	CanIf_Arc_RxIndication(hrh, canId, canDlc, canSduPtr, canIfHrhCfg);
}


#endif
