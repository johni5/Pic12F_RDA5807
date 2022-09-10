#include "includes.h"

void buttonsTick(void) {
    b_ticker++;
    b_ticker2++;
}

void checkButton(button *bs) {
    if (bs->exit) {
        bs->enter = 0;
        bs->exit = 0;
        bs->hold = 0;
    }
    if (!bs->enter && BTN_ON(bs->mask.gpio)) {
        b_ticker = 0;
        bs->exit = 0;
        bs->hold = 0;
        while (b_ticker < 100 && BTN_ON(bs->mask.gpio));
        if (b_ticker >= 100 && BTN_ON(bs->mask.gpio)) bs->enter = 1;
    }
    if (bs->enter && !bs->exit) {
        b_ticker2 = 0;
        while (b_ticker2 < 100 && BTN_OFF(bs->mask.gpio));
        if (b_ticker2 >= 100 && BTN_OFF(bs->mask.gpio)) bs->exit = 1;
        if (!bs->exit && b_ticker > 1500) bs->hold = 1;
    }
}

bit bClick(button *bs) {
    return bs->enter & bs->exit & ~bs->hold;
}

bit bHold(button *bs) {
    return bs->enter & ~bs->exit & bs->hold;
}

bit bPress(button *bs) {
    return bs->enter & ~bs->exit;
}

bit bLongClick(button *bs) {
    return bs->enter & bs->exit & bs->hold;
}