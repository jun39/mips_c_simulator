#include <stdio.h>
#include <mips.h>

void register_run(Register *reg, Signal wctl, Word wdata, Word *rdata)
{
    
    word_set_value(rdata,reg->val);
    if(wctl==1){
        // wdataをintに変換してをregに代入
        reg->val=word_get_value(wdata);
    }
}

void register_set_value(Register *reg, int val)
{
    reg->val = val;
}

int register_get_value(Register *reg)
{
    return reg->val;
}

void decoder5(Signal a[5], Word *b)
{
    int i, val;
    val = 0;
    for (i = 0; i < 5; ++i) {
        if (a[i]) {
            val += (1 << i);
        }
    }
    /* Exercise 6-1 */
    // b->bit[val]の値のみを1にし、それ以外を0にする
    for(i=0;i<32;++i){
        if(i==val){
            b->bit[i]=1;
        }else{
            b->bit[i]=0;
        }
    }
}

void mux32(Word ins[32], Signal ctls[5], Word *out)
{
    /* Exercise 6-1 */
    int selected;
    selected=0;
    for (int i = 0; i < 5; ++i) {
        if (ctls[i]) {
            selected += (1 << i);
        }
    }
    // 0のときb0を１にするword_get_valueして2^0=１のときはb0、２のときはb1、2^2=4のときはb2
    *out=ins[selected];
}

void register_file_run(RegisterFile *rf, Signal register_write, Signal *read1, Signal *read2, Signal *write1, Word wdata, Word *rdata1, Word *rdata2)
{
    /* Exercise 6-1 */
    Word DecodedWrite1,innerRead;
    Word ins[32];
    Signal wctl;
    decoder5(write1,&DecodedWrite1);
    // register_writeとデコーダの値でand積をとる。それが制御信号。for文は0は除く（書き換え不能だから）
    for(int i=0;i<=31;++i){
        if(i==0){
            wctl=0;
            register_run(&(rf->r[i]),wctl,wdata,&innerRead);
            // innerReadには読み取った値がある。
            ins[i]=innerRead;
            // レジスタ0は読み取りのみwctlを強制的にfalseに変える。そのため、常に初期値の0をとる
        }else{
            and_gate(DecodedWrite1.bit[i],register_write,&wctl);
            register_run(&(rf->r[i]),wctl,wdata,&innerRead);
            ins[i]=innerRead;
        }
    }
    // mux2回でread1・2のレジスタの値をrdata1,rdata2にいれる
    mux32(ins,read1,rdata1);
    mux32(ins,read2,rdata2);

}

void test_register_file()
{
    Signal register_write;
    Signal read1[5] = {false, false, false, false, false};
    Signal read2[5] = {false, false, false, false, false};
    Signal write1[5] = {false, false, false, false, false};
    Word wdata, rdata1, rdata2;
    RegisterFile rf;
    // 初期化時にランダムな値が入ってしまったので、0で初期化しました。
    for(int i=0;i<=31;++i){
        rf.r[i].val=0;
    }

    register_write = true;
    // for文で32このレジスタ(zeroレジスタ除く)に書き込む
    // レジスタの番号(int型)
    int number_reg;
    // read1とwrite1をテスト
    for(int i=0;i<=1;++i){
        for(int j=0;j<=1;++j){
            for(int k=0;k<=1;++k){
                for(int l=0;l<=1;++l){
                    for(int m=0;m<=1;++m){
                        read1[0]=i;
                        read1[1]=j;
                        read1[2]=k;
                        read1[3]=l;
                        read1[4]=m;
                        write1[0]=i;
                        write1[1]=j;
                        write1[2]=k;
                        write1[3]=l;
                        write1[4]=m;
                        // レジスタごとに書き込むデータを変える
                        number_reg=1*i + 2*j + 4*k +8*l + 16*m;
                        // 適当な値をレジスタに代入。ここではレジスタ番号+10の値
                        word_set_value(&wdata, number_reg+10);
                        
                        register_file_run(&rf, register_write, read1, read2, write1, wdata, &rdata1, &rdata2);
                        printf("register_number:%d,old data of rdata1: %d\n", number_reg,word_get_value(rdata1));
                        register_file_run(&rf, register_write, read1, read2, write1, wdata, &rdata1, &rdata2);
                        printf("register_number:%d,new data of rdata1: %d\n",number_reg, word_get_value(rdata1));
                        // rdata2もテストするため、初期値0に戻す
                        word_set_value(&wdata, 0);
                        register_file_run(&rf, register_write, read1, read2, write1, wdata, &rdata1, &rdata2);

                    }
                }
            }
        } 
    }


    // read2をテスト
    for(int i=0;i<=1;++i){
        for(int j=0;j<=1;++j){
            for(int k=0;k<=1;++k){
                for(int l=0;l<=1;++l){
                    for(int m=0;m<=1;++m){
                        read2[0]=i;
                        read2[1]=j;
                        read2[2]=k;
                        read2[3]=l;
                        read2[4]=m;
                        write1[0]=i;
                        write1[1]=j;
                        write1[2]=k;
                        write1[3]=l;
                        write1[4]=m;
                        // レジスタごとに書き込むデータを変える
                        number_reg=1*i + 2*j + 4*k +8*l + 16*m;
                        // 適当な値をレジスタに代入。ここではレジスタ番号+10の値
                        word_set_value(&wdata, number_reg+10);
                        
                        register_file_run(&rf, register_write, read1, read2, write1, wdata, &rdata1, &rdata2);
                        printf("register_number:%d,old data of rdata2: %d\n", number_reg,word_get_value(rdata2));
                        register_file_run(&rf, register_write, read1, read2, write1, wdata, &rdata1, &rdata2);
                        printf("register_number:%d,new data of rdata2: %d\n",number_reg, word_get_value(rdata2));
                        
                    }
                }
            }
        } 
    }


}
