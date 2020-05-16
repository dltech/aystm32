#ifndef H_AYCONN
#define H_AYCONN

#define AYHZ            2000000
#define COMMUNICATIONHZ AYHZ/512

void initAy(void);
void aySend(uint8_t addr, uint8_t data);
void aySendBlocking(uint8_t addr, uint8_t data);

#endif
