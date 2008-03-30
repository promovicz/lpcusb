#include "type.h"

BOOL SDInit(void);
BOOL SDReadCSD(U8 *pbCSD);
BOOL SDReadCID(U8 *pbCID);

BOOL SDReadBlock(U8 *pbData, U32 ulBlock);
BOOL SDWriteBlock(U8 *pbData, U32 ulBlock);

