#ifndef TRADEQ_H_INCLUDED
#define TRADEQ_H_INCLUDED
#include <stdint.h>
#include "appconst.h"

typedef struct _transaction_ Transaction;

struct _transaction_ {
  char                shareName[MAX_SHARE_NAME_LENGTH];
  char                date[MAX_DATE_LENGTH];
  float               price;
  uint32_t            quantity;
  TRANSACTION_TYPE    type;
  Transaction         *_next_;
};

typedef struct _tradeq_ TradeQ;

struct _tradeq_ {
  Transaction *       data[MAX_TRADE_Q];
  uint8_t             length;
  uint8_t             front;
  uint8_t             rear;
};

typedef struct _transaction_details_ TransactionDetails;

struct _transaction_details_ {
  Transaction *       head;
  uint32_t            length;
};

TradeQ  create_tradeq(void);
TransactionDetails create_transaction_details(void);
void    purchase_share(TradeQ *q, TransactionDetails *details, char *shareName, char *date, float price, uint32_t quantity);
uint8_t tradeq_lookup(TradeQ *q, char *shareName);
uint8_t tradeq_length(TradeQ *q);
void    sell_old_share(TradeQ *q, TransactionDetails *details, char *date, float price, uint32_t quantity);

#endif