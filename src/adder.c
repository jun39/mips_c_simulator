#include <stdio.h>
#include <mips.h>

void full_adder(Signal in1, Signal in2, Signal carry_in, Signal *out1, Signal *carry_out)
{
    
    Signal xor1_2;
    Signal and1_2;
    Signal carryoutInner;
    // sum
    xor_circuit(in1,in2,&xor1_2);
    xor_circuit(xor1_2,carry_in,out1);
    // out1 = in1 XOR in2 XOR carry_inであるのであるのでそれを表した。out1はsumを表す。
    // caryyout
    and_gate(in1,in2,&and1_2);
    and_gate(xor1_2,carry_in,&carryoutInner);
    or_gate(and1_2,carryoutInner,carry_out);
    // carry_out = in1・in2 + (in1 XOR in2)・carry_inであるのでそれを表した。


}

void rca(Word in1, Word in2, Signal carry_in, Word *out1, Signal *carry_out)
{
    
    // in1
// innneroutをwordポインタのbitに代入　for文で回す
// in1とin2はそれぞれ32ビットのバイナリ配列となっており、最下位ビットのときのみ、外部から与えられたcarry_inとの計算が必要となり、
// 計算されたcarry_outが伝播されていくため、in1.bit[0]とin2.bit[0]の場合のみfull_adderの引数にcarry_inを代入した。
// それ以外の場合は、伝播したcarry_outの値をもとに計算するため、carry_outポインタが指し示す値を*carry_outで取得し、計算した結果でcarry_outの値を置き換えている。

    for(int i=0;i<32;++i){
        if(i==0){
            full_adder(in1.bit[i],in2.bit[i],carry_in,&(out1->bit[i]),carry_out);
        }else{
            full_adder(in1.bit[i],in2.bit[i],*carry_out,&(out1->bit[i]),carry_out);
        }
    }
}

void test_full_adder()
{
   
    Signal a, b, c_in,sum,c_out;
    /* AND Gate Test*/
    for (a = 0; a <= 1; ++a) {
        for (b = 0; b <= 1; ++b) {
            for (c_in = 0; c_in <= 1; ++c_in) {
                full_adder(a,b,c_in,&sum,&c_out);
                printf("full_add(%d, %d,%d) => sum(%d) carry_out(%d)\n", a, b,c_in,sum,c_out);
            }
        }
    }
    // テストすべき場合はaとbとcarry_inの値の組み合わせであり、それぞれ0と１をとるので、合計2^3=8通りを計算した。
    // for文で省略している。
}

void test_rca(int val1, int val2)
{
    Word a,b,out1,out2;
    Signal carry_in,carry_out1,carry_out2;
    word_set_value(&a,val1);
    word_set_value(&b,val2);
    // 引数のval1とval2の値を、Word構造体として、0と１の２進法でセットする。



    
    // carry_inが1の場合のテスト
    carry_in=1;
    rca(a,b,carry_in,&out1,&carry_out1);
    int number_a=word_get_value(a);
    int number_b=word_get_value(b);
    int number_out1=word_get_value(out1);


    printf("rca(%d, %d,%d) => sum(%d) carry_out(%d)\n", number_a, number_b,carry_in,number_out1,carry_out1);
    // carry_inが0の場合のテスト
    carry_in=0;
    rca(a,b,carry_in,&out2,&carry_out2);
    number_a=word_get_value(a);
    number_b=word_get_value(b);
    int number_out2=word_get_value(out2);
    printf("rca(%d, %d,%d) => sum(%d) carry_out(%d)\n", number_a, number_b,carry_in,number_out2,carry_out2);

}

