#include <stdio.h>
#include <mips.h>

// aluの*(ops+2)がtrueのときは入力２(not_b)を選ぶ
void mux(Signal in1, Signal in2, Signal ctl, Signal *out1)
{
    
    Signal inner1[2], inner2[2], inner3[2];

    /* ctl1 != true */
    not_gate(ctl, inner1 + 0);
    inner1[1] = in1;
    andn_gate(inner1, 2, inner3 + 0);
    /* ctl1 == true */
    inner2[0] = ctl;
    inner2[1] = in2;
    andn_gate(inner2, 2, inner3 + 1);

    orn_gate(inner3, 2, out1);
}

// ここがおかしいかも　５個命令があるのに4つしか選択できない
void mux4(Signal in1, Signal in2, Signal in3, Signal in4, Signal ctl1, Signal ctl2, Signal *out1)
{
    Signal inner1[3], inner2[3], inner3[3], inner4[3], inner5[4];
    // inner1 + 0などは配列の要素ごとのポインタ

    /* ctl1 != true && ctl2 != true) */
    not_gate(ctl1, inner1 + 0);
    not_gate(ctl2, inner1 + 1);
    inner1[2] = in1;
    andn_gate(inner1, 3, inner5 + 0);
    // inner1配列は0,1,2までだから合計3要素のngateをする
    /* ctl1 == true && ctl2 != true) */
    inner2[0] = ctl1;
    not_gate(ctl2, inner2 + 1);
    inner2[2] = in2;
    andn_gate(inner2, 3, inner5 + 1);
    /* (ctl1 != true && ctl2 == true) */
    not_gate(ctl1, inner3 + 0);
    inner3[1] = ctl2;
    inner3[2] = in3;
    andn_gate(inner3, 3, inner5 + 2);
    /* (ctl1 == true && ctl2 == true) */
    inner4[0] = ctl1;
    inner4[1] = ctl2;
    inner4[2] = in4;
    andn_gate(inner4, 3, inner5 + 3);
    
    orn_gate(inner5, 4, out1);
}


void alu(Signal *ops, Signal a, Signal b, Signal less, Signal carry_in, Signal *s, Signal *carry_out)
{
    Signal not_b,binvert_b;
    Signal result[4];
    not_gate(b,&not_b);
    mux(b,not_b,*(ops+2),&binvert_b);
    // mux(b,not_b,ops[2],&binvert_b)かも

    
    // AND Gate
    and_gate(a,b,result+0);
    // OR gate
    or_gate(a,b,result+1);
    // FA
    full_adder(a,binvert_b,carry_in,result+2,carry_out);
    // lessをmux4のSignal in4に入れる
    result[3]=less;
    // 最後にMUX4で選択
    mux4(result[0],result[1],result[2],result[3],*(ops+0),*(ops+1),s);
}

// mips.hとtest.cに定義してテストする
void alu_msb(Signal *ops, Signal a, Signal b, Signal less, Signal carry_in, Signal *s, Signal *carry_out, Signal *set,Signal *judge_over)
{
    /* Exercise 5-1 */
    Signal not_b,binvert_b;
    Signal result[4];
    not_gate(b,&not_b);
    mux(b,not_b,*(ops+2),&binvert_b);
    // mux(b,not_b,ops[2],&binvert_b)かも

    
    // AND Gate
    and_gate(a,b,result+0);
    // OR gate
    or_gate(a,b,result+1);
    // setにFAの結果を出力する
    full_adder(a,binvert_b,carry_in,set,carry_out);
    result[2]=*set;
    // lessをmux4のSignal in4に入れる
    result[3]=less;
    // 最後にMUX4で選択
    mux4(result[0],result[1],result[2],result[3],*(ops+0),*(ops+1),s);

    // オーバーフロー判定
    Signal not_binvert,not_a_msb,not_b_msb,not_s_msb;
    Signal inner_over1[4], inner_over2[4], inner_over3[4], inner_over4[4],inner_over5[4];
    not_gate(*(ops+2),&not_binvert);
    not_gate(a,&not_a_msb);
    not_gate(b,&not_b_msb);
    // sはポインタ
    not_gate(*s,&not_s_msb);
    inner_over1[0]=not_binvert;
    inner_over1[1]=not_a_msb;
    inner_over1[2]=not_b_msb;
    inner_over1[3]=*s;

    inner_over2[0]=not_binvert;
    inner_over2[1]=a;
    inner_over2[2]=b;
    inner_over2[3]=not_s_msb;

    inner_over3[0]=*(ops+2);
    inner_over3[1]=not_a_msb;
    inner_over3[2]=b;
    inner_over3[3]=*s;

    inner_over4[0]=*(ops+2);
    inner_over4[1]=a;
    inner_over4[2]=not_b_msb;
    inner_over4[3]=not_s_msb;
    // andn_gateを4回通り、最後に全部をorn_gateしてそれが1なら出力

    // 16この配列にして、for文でinner_over1+(i*4)みたいに配列渡した方がよい可能性

    andn_gate(inner_over1,4,inner_over5+0);
    andn_gate(inner_over2,4,inner_over5+1);
    andn_gate(inner_over3,4,inner_over5+2);
    andn_gate(inner_over4,4,inner_over5+3);

    orn_gate(inner_over5,4,judge_over);
}

// alu32とalu_msbの引数にover_jugdeのsignalを入れて、出力。テスト関数でover_jugdeの値を出力する

void alu32(Signal *ops, Word a, Word b, Word *s, Signal *zero,Signal *judge_over)
{
    Signal binvert= *(ops+2);
    Signal less,carry_out,set,set_dummy,inner_equal;
    carry_out=false;
    set=false;
    less=false;
    
    // 制御命令がops(1,1,1)であるときslt
    for(int i=0;i<31;++i){
        if(binvert==true&&i==0){
            alu(ops,a.bit[i],b.bit[i],less,binvert,&(s->bit[i]),&carry_out);
        }else{
            alu(ops,a.bit[i],b.bit[i],less,carry_out,&(s->bit[i]),&carry_out);
        }
    }
    // 最後はALU_MSB
    alu_msb(ops,a.bit[31],b.bit[31],less,carry_out,&(s->bit[31]),&carry_out,&set,judge_over);
    // ２回めのALU0
    alu(ops,a.bit[0],b.bit[0],set,binvert,&(s->bit[0]),&carry_out);
    // 出力sを入力としてさらにOR-N Gateを実行
    orn_gate(s,32,&inner_equal);
    // さらにその結果をNOT Gateで反転し、zeroに出力
    not_gate(inner_equal,zero);

}

void test_alu()
{
    
    Signal a, b, carry_in, s, carry_out;
    Signal ops[3];
    
    /* AND */
    ops[0] = false;
    ops[1] = false;
    ops[2] = false;
    for (carry_in = 0; carry_in <= 1; ++carry_in) {
        for (a = 0; a <= 1; ++a) {
            for (b = 0; b <= 1; ++b) {
                alu(ops, a, b, false, carry_in, &s, &carry_out);
                printf("ALU(AND, %x, %x, %d) => (s:%x, carry_out:%d)\n", a, b, carry_in, s, carry_out);
            }
        }
    }
    /* OR */
  
    ops[0] = true;
    ops[1] = false;
    ops[2] = false;
    for (carry_in = 0; carry_in <= 1; ++carry_in) {
        for (a = 0; a <= 1; ++a) {
            for (b = 0; b <= 1; ++b) {
                alu(ops, a, b, false, carry_in, &s, &carry_out);
                printf("ALU(OR, %x, %x, %d) => (s:%x, carry_out:%d)\n", a, b, carry_in, s, carry_out);
            }
        }
    }
    /* ADD */
    
    ops[0] = false;
    ops[1] = true;
    ops[2] = false;
    for (carry_in = 0; carry_in <= 1; ++carry_in) {
        for (a = 0; a <= 1; ++a) {
            for (b = 0; b <= 1; ++b) {
                alu(ops, a, b, false, carry_in, &s, &carry_out);
                printf("ALU(ADD, %d, %d, %d) => (s:%d, carry_out:%d)\n", a, b, carry_in, s, carry_out);
            }
        }
    }
    /* オーバーフロー判定 */
    Signal set=false;
    Signal judge_over=false;

    ops[0] = false;
    ops[1] = true;
    // ここがbinvert
    ops[2] = false;
    for (carry_in = 0; carry_in <= 1; ++carry_in) {
        for (ops[2] = 0; ops[2] <= 1; ++ops[2]) {
            for (a = 0; a <= 1; ++a) {
                for (b = 0; b <= 1; ++b) {
                alu_msb(ops, a, b, false, carry_in, &s, &carry_out,&set,&judge_over);
                printf("ALU_MSB(ADD, a:%d, b:%d, binvert:%d) => (s:%d,carry_in:%d,carryout:%d,judge:%d)\n", a, b, ops[2], s,carry_in,carry_out,judge_over);
                }
            }
        }
    }
    // 積和標準形では式の結果が１となる項のみの和をとればよいので（結果が０の場合は項と積をとった場合に０となるから）、
    // 積和標準形 not(binvert)・not(a)・not(b)・s+not(binvert)・a・b・not(s)+binvert・not(a)・b・s+binvert・a・not(b)・not(s)

}

void test_alu32(int val1, int val2)
{
    
    
    Signal ops[3];
    Word in1, in2, out1;
    Signal is_zero,judge_over;
    word_set_value(&in1, val1);
    word_set_value(&in2, val2);
    judge_over=false;
    is_zero=false;
    
    /* AND */
    ops[0] = false;
    ops[1] = false;
    ops[2] = false;
    alu32(ops, in1, in2, &out1, &is_zero,&judge_over);
    printf("ALU32(AND, %x, %x) => (result:%x)\n", val1, val2, word_get_value(out1));

    /* OR */

    ops[0] = true;
    ops[1] = false;
    ops[2] = false;
    alu32(ops, in1, in2, &out1, &is_zero,&judge_over);
    printf("ALU32(OR, %x, %x) => (result:%x)\n", val1, val2, word_get_value(out1));

    /* ADD */
    ops[0] = false;
    ops[1] = true;
    ops[2] = false;
    alu32(ops, in1, in2, &out1, &is_zero,&judge_over);
    printf("ALU32(ADD, %d, %d) => (result:%d),judge:%d\n", val1, val2, word_get_value(out1),judge_over);

    /* SUB (also check zero?) */
    ops[0] = false;
    ops[1] = true;
    ops[2] = true;
    alu32(ops, in1, in2, &out1, &is_zero,&judge_over);
    printf("ALU32(SUB, %d, %d) => (result:%d, is_zero:%d),judge:%d\n", val1, val2, word_get_value(out1), is_zero,judge_over);
    
    /* SLT */
    ops[0] = true;
    ops[1] = true;
    ops[2] = true;
    alu32(ops, in1, in2, &out1, &is_zero,&judge_over);
    printf("ALU32(SLT, %d, %d) => (a<b:%d)\n", val1, val2,word_get_value(out1));


}



