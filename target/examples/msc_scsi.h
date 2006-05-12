#include "type.h"

void	SCSIReset(void);
BOOL	SCSIHandleCmd(U8 *pbCDB, int iCDBLen, int *piRspLen, BOOL *pfDevIn);
BOOL	SCSIHandleData(U8 *pbCDB, int iCDBLen, U8 *pbData, U32 dwOffset);
