#include <stdio.h>
#include <mips.h>

void and_gate(Signal in1, Signal in2, Signal *out1)
{
    *out1 = in1 && in2;
}

void or_gate(Signal in1, Signal in2, Signal *out1)
{
    
    *out1 = in1 || in2;
    // ||はc言語における論理和をとる論理演算子で、in1とin2のどちらかが1の場合にout1ポインタが指す値に、１を代入する。
}

void not_gate(Signal in1, Signal *out1)
{
    *out1 = !in1;
}

void nand_circuit(Signal in1, Signal in2, Signal *out1)
{
    Signal inner;
    and_gate(in1, in2, &inner);
    not_gate(inner, out1);
}

void xor_circuit(Signal in1, Signal in2, Signal *out1)
{
    
    Signal inner1;
    Signal inner2;
    Signal not1;
    Signal not2;
    not_gate(in1, &not1);
    not_gate(in2, &not2);
    and_gate(in1,not2,&inner1);
    and_gate(in2,not1,&inner2);
    or_gate(inner1,inner2,out1);
    // XORサーキットは論理式in1・not(in2) + not(in1)・in2を表している。途中結果のin1・not(in2)とnot(in1)・in2をそれぞれinner1と
    // inner2に保存することで、可読性を高めた。
}

void andn_gate(Signal *in1, int n, Signal *out1)
{
    int i;
    Signal s = true;
    for (i = 0; i < n; ++i) {
        if (in1[i] == false) {
            s = false;
            break;
        }
    }
    *out1 = s;
}

void orn_gate(Signal *in1, int n, Signal *out1)
{
    int i;
    Signal s = false;
    for (i = 0; i < n; ++i) {
        if (in1[i] == true) {
            s = true;
            break;
        }
    }
    *out1 = s;
}

void test_gate()
{
    Signal a, b, out;
    /* AND Gate Test*/
    for (a = 0; a <= 1; ++a) {
        for (b = 0; b <= 1; ++b) {
            and_gate(a, b, &out);
            printf("AND(%d, %d) => %d\n", a, b, out);
        }
    }
    /* NOT Gate Test*/
    for (a = 0; a <= 1; ++a) {
        not_gate(a, &out);
        printf("NOT(%d) => %d\n", a, out);
    }
    /* NAND Circuit Test*/
    for (a = 0; a <= 1; ++a) {
        for (b = 0; b <= 1; ++b) {
            nand_circuit(a, b, &out);
            printf("NAND(%d, %d) => %d\n", a, b, out);
        }
    }

    // OR Circuit Test じぶんでかいたやつ
    for (a = 0; a <= 1; ++a) {
        for (b = 0; b <= 1; ++b) {
            or_gate(a, b, &out);
            printf("OR(%d, %d) => %d\n", a, b, out);
        }
    }
    // XORのテスト
    for (a = 0; a <= 1; ++a) {
        for (b = 0; b <= 1; ++b) {
            xor_circuit(a, b, &out);
            printf("XOR(%d, %d) => %d\n", a, b, out);
        }
    }
}
