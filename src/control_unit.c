#include <mips.h>

void control_unit(Signal opcode[6], Signal *register_dst,Signal *alu_src,Signal *mem_to_reg, Signal *register_write, Signal *mem_read, Signal *mem_write, Signal *branch, Signal *aluop1, Signal *aluop0)
{
    
    int i;
    Signal not_opcode[6],lw_opcode[6],sw_opcode[6],beq_opcode[6];
    Signal inner_calc,inner_lw,inner_sw,inner_beq;

    for(i=0;i<6;++i){
        not_gate(opcode[i],&(not_opcode[i]));
    }
    // オペコードをビット反転する。

    andn_gate(not_opcode,6,&inner_calc);
    // 演算のinnerを計算するゲート。ビット反転したオペコードの値をand-Nゲートに通す。

    // lwのinnerを計算するゲートに入れる値lw_opcodeを計算する。
    lw_opcode[0]=opcode[0];
    lw_opcode[1]=opcode[1];
    lw_opcode[2]=not_opcode[2];
    lw_opcode[3]=not_opcode[3];
    lw_opcode[4]=not_opcode[4];
    lw_opcode[5]=opcode[5];
    andn_gate(lw_opcode,6,&inner_lw);

    // swのinnerを計算するゲートに入れる値sw_opcodeを計算する。
    sw_opcode[0]=opcode[0];
    sw_opcode[1]=opcode[1];
    sw_opcode[2]=not_opcode[2];
    sw_opcode[3]=opcode[3];
    sw_opcode[4]=not_opcode[4];
    sw_opcode[5]=opcode[5];
    andn_gate(sw_opcode,6,&inner_sw);

    // beqのinnerを計算するゲートに入れる値beq_opcodeを計算する。
    beq_opcode[0]=not_opcode[0];
    beq_opcode[1]=not_opcode[1];
    beq_opcode[2]=opcode[2];
    beq_opcode[3]=not_opcode[3];
    beq_opcode[4]=not_opcode[4];
    beq_opcode[5]=not_opcode[5];
    andn_gate(beq_opcode,6,&inner_beq);


    *register_dst=inner_calc;
    // 演算命令の場合のinnerの値を書き込みレジスタ番号の位置を制御する信号にセットする

    or_gate(inner_lw,inner_sw,alu_src);
    // inner_lwとinner_swをor演算した値をalu_srcにセットする

    *mem_to_reg=inner_lw;
    // inner_lwの値をmem_to_regにセットする

    or_gate(inner_calc,inner_lw,register_write);
    // inner_calcとinner_lwをor演算した値をレジスタへの書き込みを制御する信号にセットする

    *mem_read=inner_lw;
    // inner_lwの値を,メモリの読み込み制御の信号にセットする。

    *mem_write=inner_sw;
    // inner_swの値を,メモリの書き込み制御の信号にセットする。

    *branch=inner_beq;
    // inner_beqの値を、pcの分岐制御の信号にセットする。


    *aluop1=inner_calc;
    // 演算命令の場合のinnerの値をalu_control_unitで使用するaluOp1にセットする

    *aluop0=inner_beq;
    // inner_beqの値をalu_control_unitで使用するaluOp0にセットする


}

void alu_control_unit(Signal *funct, Signal aluop1, Signal aluop0,Signal *ops)
{
    Signal not_aluop1;
    Signal not_funct2;
    Signal inner1_op2,inner2_op2;
    Signal inner1_op0,inner2_op0;
    // and_gate(aluop1,*(funct+1),ops+2);
    // funct[1]の値とcontrol_unitで求めたaluop1の値のAND演算を行い、ops[2]にその演算結果をセットする

    // 追加課題の場合下のコード
    and_gate(aluop1,*(funct+1),&inner1_op2);
    // *(ops+2)=inner1_op2;
    or_gate(aluop0,inner1_op2,ops+2);
    not_gate(aluop1,&not_aluop1);
    not_gate(*(funct+2),&not_funct2);
    // funct[2]とaluop1をNOTゲートに入力し、それぞれの値をnot_aluop1とnot_funct2にセットする
    or_gate(not_funct2,not_aluop1,ops+1);
    // それらの値を用いてOR演算を行い、その演算結果をops[1]に代入する

    or_gate(*(funct+3),*(funct+0),&inner1_op0);
    and_gate(aluop1,inner1_op0,ops);
    // funct[3]とfunct[0]の値を用いてor演算を行い、その値とaluop1でAND演算を行う。AND演算の結果をops[0]にセットする
    // 以上で求めたops[0~2]の値を使って、ALUの制御を行う。


}
