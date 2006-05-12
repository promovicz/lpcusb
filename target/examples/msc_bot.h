#include "type.h"

#define BULK_OUT_EP		0x02
#define BULK_IN_EP		0x85

void MSCBotReset(void);
void MSCBotBulkOut(U8 bEP, U8 bEPStatus);
void MSCBotBulkIn(U8 bEP, U8 bEPStatus);

