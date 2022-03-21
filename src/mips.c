#include <stdio.h>
#include <mips.h>

void mips_run(MIPS *m, int inst_num)
{
    Word npaddr, paddr,innerWord_npaddr;
    Word instr,addr;
    Word offset_shift;
    Word innerWord_shift_alu;
    Word data_sign_extention,innerWord_mux_aluSrc,innerWord_mux_memToReg;
    Word wdata, rdata1, rdata2;
    Word rdata_mem;
    Signal register_dst,alu_src, mem_to_reg,register_write, mem_read,mem_write,branch,alu_op1, alu_op0,ops[3];
    Signal zero, zero4,inner_branch_zero;
    Signal inner_mux_regDst[5];
    Word rdata_mem_t4addr;
    int i;
    int j;

    for (i = 0; i < inst_num; ++i) {
        /* Exercise 8-1 */
        Signal judge_over=false;
        // m->imから命令コードを取り出す
        int int_pc=register_get_value(&(m->pc));
        // pcのレジスタ実行
        word_set_value(&paddr,int_pc);
        inst_memory_run(&(m->im),paddr,&instr);
        // 命令メモリから命令読み込み


        control_unit(&(instr.bit[26]),&register_dst,&alu_src,&mem_to_reg,&register_write,&mem_read,&mem_write,&branch,&alu_op1,&alu_op0);

        
        

        data_sign_extention=instr;
        for(j=16;j<32;++j){
            data_sign_extention.bit[j]=instr.bit[15];
        }
        // 符号拡張の回路　offsetを拡張して、Word型に変換する。offsetの最上位ビットの値で32ビットまで拡張。



        alu_control_unit(&(instr.bit[0]),alu_op1,alu_op0,ops);
        // alu_control_unitでの演算

        
        for(j=0;j<5;++j){
            mux(instr.bit[16+j],instr.bit[11+j],register_dst,&(inner_mux_regDst[j]));
        }
        // muxでregister_dstの値に応じて、reg2かreg3のどちらかを出力

        register_file_run(&(m->rf),register_write,&(instr.bit[21]),&(instr.bit[16]),&(inner_mux_regDst[0]),wdata,&rdata1,&rdata2);
        // 指定されたレジスタから、値を読み込む。wdataは適当な値



        for(j=0;j<32;++j){
            mux(rdata2.bit[j],data_sign_extention.bit[j],alu_src,&(innerWord_mux_aluSrc.bit[j]));
        }
        // 符号拡張した値か、rdata2かを選ぶmux回路
        

        alu32(ops,rdata1,innerWord_mux_aluSrc,&addr,&zero,&judge_over);
        // aluでaddrを計算
        data_memory_run(&(m->dm),mem_write,mem_read,addr,rdata2,&rdata_mem);
        // wdataはデータメモリの場合rdata2
        // 指定されたaddrから読み出し・rdata2の書き込みを行う


        for(j=0;j<32;++j){
            mux(addr.bit[j],rdata_mem.bit[j],mem_to_reg,&(innerWord_mux_memToReg.bit[j]));
        }
        // addrかメモリーから読みだしたrdata_memのどちらかかを選ぶ

        
        // 演算結果をwdataに書き込む
        register_file_run(&(m->rf),register_write,&(instr.bit[21]),&(instr.bit[16]),&(inner_mux_regDst[0]),innerWord_mux_memToReg,&rdata1,&rdata2);
        // wdataを指定されたレジスタに書き込む
        // wdataはinnerWord_mux_memToRegとなる。



        /* ... */
        Signal judge_over_pc=false;
        Signal pcadd[3] = {false, true, false};
        Word four;
        // fourが２番めのレジスタの値になればよい
        word_set_value(&four, 4);
         // 処理が終わったので次の命令を読み込むためのため4だけPCの値を増やす。
        // judge_over引数をalu32に追加した。
        alu32(pcadd, paddr, four, &npaddr, &zero4,&judge_over_pc);

       
        // 分岐の回路

        for(j=0;j<32;++j){
            if(j==0||j==1){
                offset_shift.bit[j]=0;
            }else{
                offset_shift.bit[j]=data_sign_extention.bit[j-2];
            }
        }
        
        // シフト計算
        alu32(pcadd, npaddr, offset_shift, &innerWord_shift_alu, &zero4,&judge_over_pc);
        // 分岐の際の加算器、出力はジャンプ先のアドレス

        and_gate(zero,branch,&inner_branch_zero);
        // 分岐の制御信号を求める

        for(j=0;j<32;++j){
            mux(npaddr.bit[j],innerWord_shift_alu.bit[j],inner_branch_zero,&(innerWord_npaddr.bit[j]));
        }
        npaddr =innerWord_npaddr;


        register_run(&(m->pc), true, npaddr, &paddr);
        // 次のアドレスnpaddrがm->pc->valに書き込まれ、paddrの値が元のm->pc->valの値になる。
        printf("16進数表現 $t0 = %x,10進数表現 $t0=%d\n", register_get_value(m->rf.r + 8),register_get_value(m->rf.r + 8));
        // t0レジスタの値（演算結果）を出力
        printf("pcの値%x\n",m->pc.val);


    }

    // swで書き込んだメモリの0x10000004のアドレスにある値を出力する。sw  $t1,0($t4)を行ったので$t1の値0x100が出力される
    word_set_value(&addr, 0x10000004);
    data_memory_run(&(m->dm),false,true,addr,wdata,&rdata_mem_t4addr);
    printf("16進数表現 0x10000004のメモリの値は%x\n",word_get_value(rdata_mem_t4addr));

}

void mips_init(MIPS *m, int inst[], int inst_num)
{
    int i;
    Word addr,wdata,rdata;
    register_set_value(&(m->pc), INST_MEM_START);
    register_set_value((m->rf.r + 9), 0x100);
    register_set_value((m->rf.r + 10), 0x300);

    word_set_value(&addr, 0x10000004);
    word_set_value(&wdata, 0x200);
    register_set_value((m->rf.r + 12), 0x10000004);
    // $t4に0x200が格納されているメモリのアドレスをセットする。
    data_memory_run(&(m->dm),true,false,addr,wdata,&rdata);
    // 0x10000004が指すメモリに0x200を格納する

    printf("10進数表現 $t1=%d,$t2=%d,$t3=%d\n",register_get_value(m->rf.r + 9),register_get_value(m->rf.r + 10),register_get_value(m->rf.r + 11));
    printf("16進数表現 $t1=%x,$t2=%x,$t3=%x\n",register_get_value(m->rf.r + 9),register_get_value(m->rf.r + 10),register_get_value(m->rf.r + 11));
    // inst[]の命令をmのim(命令メモリ)に格納
    //  INST_MEM_STARTのアドレスから、4ずつ増加させて、順にinst[]のメモリをそのアドレスが指すメモリにセットする。

    
    for (i = 0; i < inst_num; ++i) {
        inst_memory_set_inst(&(m->im), INST_MEM_START + 4 * i, inst[i]);
    }

}

void test_mips()
{
    MIPS m;
    
    // 命令をセット
    // int inst[] = {0x012a4020,
    //               0x012a4022,
    //               0x012a4024,
    //               0x012a4025,
    //               0x012a402a};

    int inst[] = {
                  0x8d880000,
                  0xad890000,
                   0x012a4020,
                   0x012a4022,
                   0x012a4024,
                   0x012a4025,
                   0x012a402a,
                   };
    // 以下の命令を機械語に変換したのが上記のinst
    // lw  $t0,0($t4)
    // sw  $t1,0($t4)
	// add $t0,$t1,$t2
	// sub $t0,$t1,$t2
	// and $t0,$t1,$t2
    // or  $t0,$t1,$t2
    // slt $t0,$t1,$t2
    mips_init(&m, inst, 7);
    mips_run(&m, 7);
}
