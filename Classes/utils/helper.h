#ifndef MATH_HELPER_H
#define MATH_HELPER_H

#include "cocos2d.h"
#include <avalon/payment/Manager.h>

namespace helper {

bool paymentAvailableCheck(avalon::payment::Manager* payment);
void showPaymentPendingSpinner(const bool flag);

}; // namespace helper

#endif // MATH_HELPER_H