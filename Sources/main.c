#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tradeq.h"
#include "appconst.h"

void test_tradeq(void) {
  TradeQ q1 = create_tradeq(), *q = &q1;
  TransactionDetails details = create_transaction_details();
  assert(tradeq_length(q) == 0);
  purchase_share(q, &details, "NIFTY 50", "18-12-2020", 13000.15, 20);
  assert(strcmp(details.head->shareName, "NIFTY 50") == 0);
  purchase_share(q, &details, "TCS", "18-12-2020", 2600.50, 100);
  purchase_share(q, &details, "HDFC", "21-12-2020", 1400, 100);
  purchase_share(q, &details, "BURGER KING", "21-12-2020", 150, 100);
  purchase_share(q, &details, "INFY", "22-12-2020", 1100, 100);
  assert(tradeq_length(q) == 5);
  assert(tradeq_lookup(q, "INFY"));
  assert(details.length == 5);
  purchase_share(q, &details, "INFY", "23-12-2020", 900, 100);
  assert(tradeq_length(q) == 5);
  assert(q->data[4]->price == 1000);
  assert(details.length == 6);
  purchase_share(q, &details, "SBIN", "23-12-2020", 250, 100);
  assert(tradeq_length(q) == 5);
  assert(details.length == 8);
  sell_old_share(q, &details, "23-12-2020", 2700.50, 50);
  assert(tradeq_length(q) == 5);
  assert(details.length == 9);
  sell_old_share(q, &details, "24-12-2020", 2800, 100);
  assert(details.length == 10 && details.head->type == REJECT);
  sell_old_share(q, &details, "24-12-2020", 2800, 50);
  assert(tradeq_length(q) == 4);
  assert(details.length == 11);
}

int main(void) {
  test_tradeq();
  return 0;
}