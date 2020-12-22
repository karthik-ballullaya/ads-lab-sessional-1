#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "appconst.h"
#include "tradeq.h"

static Transaction * _create_transaction_(char *name, char * date, float price, uint32_t quantity, TRANSACTION_TYPE tType) {
  Transaction *transaction = (Transaction *) malloc(sizeof(Transaction));
  strcpy(transaction->shareName, name);
  strcpy(transaction->date, date);
  transaction->price = price;
  transaction->quantity = quantity;
  transaction->type = tType;
  transaction->_next_ = NULL;
  return transaction;
}

static Transaction * _queue_dequeue_(TradeQ *q) {
  Transaction *t = q->data[q->front];
  q->front = (q->front + 1) % MAX_TRADE_Q;
  q->length --;
  return t;
}

static uint8_t _queue_enqueue_(TradeQ *q, Transaction *t) {
  if (tradeq_length(q) < MAX_TRADE_Q) {
    q->data[q->rear] = t;
    q->rear = (q->rear + 1) % MAX_TRADE_Q;
    q->length ++;
    return 1;
  }
  return 0;
}

static void _add_transaction_detail_(TransactionDetails *details, Transaction *t) {
  t->_next_ = details->head;
  details->head = t;
  details->length ++;
}

static int8_t _trade_at_(TradeQ *q, char *shareName) {
  for (int8_t i = 0; i < tradeq_length(q); i++) {
    if (q->data[i] && !strcmp(q->data[i]->shareName, shareName)) {
      return i;
    }
  }
  return -1;
}

static void _trade_update_(TradeQ *q, uint8_t loc, Transaction *t_new) {
  Transaction *t_old = q->data[loc];
  t_old->price = ((t_old->price * t_old->quantity) + (t_new->price * t_new->quantity)) / (t_old->quantity + t_new->quantity);
  t_old->quantity += t_new->quantity;
}

TradeQ create_tradeq(void) {
  TradeQ q = { {NULL}, 0, 0, 0 };
  return q;
}

TransactionDetails create_transaction_details(void) {
  TransactionDetails details = { NULL, 0 };
  return details;
}

void purchase_share(TradeQ *q, TransactionDetails *details, char *shareName, char *date, float price, uint32_t quantity) {
  assert(q != NULL && details != NULL);
  Transaction *transaction = _create_transaction_(shareName, date, price, quantity, BUY);
  int8_t isShareAvailable = tradeq_lookup(q, shareName);
  if (isShareAvailable) {
    _trade_update_(q, _trade_at_(q, shareName), transaction);
  } else if (tradeq_length(q) >= MAX_TRADE_Q) {
    Transaction *sale = _queue_dequeue_(q);
    sale->type = (sale->type == BUY) ? SELL : BUY;
    _add_transaction_detail_(details, sale);
    _queue_enqueue_(q, transaction);
  } else {
    _queue_enqueue_(q, transaction);
  }
  _add_transaction_detail_(details, transaction);
}

uint8_t tradeq_lookup(TradeQ *q, char *shareName) {
  assert(q != NULL);
  return (_trade_at_(q, shareName) > 0);
}

uint8_t tradeq_length(TradeQ *q) {
  assert(q != NULL);
  return q->length;
}

void sell_old_share(TradeQ *q, TransactionDetails *details, char *date, float price, uint32_t quantity) {
  assert(q != NULL && details != NULL);
  if (!tradeq_length(q)) return;
  Transaction *transactedData = q->data[q->front];
  Transaction *transaction = _create_transaction_(transactedData->shareName, date, price, quantity, SELL);
  if (transactedData->quantity > quantity) {
    transactedData->quantity -= quantity;
  } else if (q->data[q->front]->quantity == quantity) {
    free(_queue_dequeue_(q));
  } else {
    transaction->type = REJECT;
  }
  _add_transaction_detail_(details, transaction);
}