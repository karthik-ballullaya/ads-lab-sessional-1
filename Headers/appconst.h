#ifndef APPCONST_H_INCLUDED
#define APPCONST_H_INCLUDED

#define MAX_TRADE_Q   5
#define MAX_SHARE_NAME_LENGTH 20
#define MAX_DATE_LENGTH   9

typedef enum _transaction_type TRANSACTION_TYPE;
enum _transaction_type {
  SELL = 1,
  BUY,
  REJECT
};


#endif /* appconst.h */