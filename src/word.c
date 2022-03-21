#include <mips.h>

void word_set_value(Word *w, int val)
{
    int i;
    for (i = 0; i < 32; ++i) {
        if ((val & 0x1) != 0) {
            w->bit[i] = true;
        }
        else {
            w->bit[i] = false;
        }
        val >>= 1;
    }
}

int word_get_value(Word w)
{
    int i, val = 0;
    for (i = 31; i >= 0; --i) {
        val <<= 1;
        if (w.bit[i]) {
            val++;
        }
    }
    return val;
}

