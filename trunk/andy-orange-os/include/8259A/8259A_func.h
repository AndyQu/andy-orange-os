#ifndef __8259A__FUNC__
#define __8259A__FUNC__
void enable_disk(void);
void disable_disk(void);

void enable_clock(void);
void disable_clock(void);

void enable_keyboard(void);
void disable_keyboard(void);

void triggerSlave8259(void);
void triggerMaster8259(void);
#endif
