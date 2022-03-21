#include <stdio.h>
#include <mips.h>

void inst_memory_run(InstMemory *im, Word addr, Word *inst)
{
   
    int addressNum=word_get_value(addr) - INST_MEM_START;
    int offset=addressNum/4;
    word_set_value(inst,im->mem[offset]);
}

void inst_memory_set_inst(InstMemory *im, int addr, int inst)
{
   
    int addressNum=addr - INST_MEM_START;
    // INST_MEM_STARTから何word文高い値をとっているかが、メモリーの配列番号となる。
    // wordごとなので4で割っている。
    int offset=addressNum/4;
    im->mem[offset]=inst;

}

void data_memory_run(DataMemory *dm, Signal mem_write, Signal mem_read, Word addr, Word wdata, Word *rdata)
{
  
    int val, offset;
    val = word_get_value(addr);
    offset = (val - DATA_MEM_START) / 4;
    
    if (mem_read) {
        /* ... */
        // 読み込みのみ
        word_set_value(rdata,dm->mem[offset]);
    }
    else if (mem_write) {
        /* ... */
        // 書き込みの場合はメモリーの値を書き換える
        dm->mem[offset]=word_get_value(wdata);
    }
}

void test_inst_memory()
{
    Word addr, inst;
    InstMemory im;
    int inst_data,tmp;

    int address=0x04000000;

    // 10個の値をInstMemoryにセットして読み出す
    for(int i=0;i<10;++i){
        // セットするアドレスを1wordずつずらしていく
        tmp=address+i*4;
        // 書き込むデータは適当にi*100の値とする。
        inst_data=i*100;
        inst_memory_set_inst(&im, tmp, inst_data);
        printf("InstMemory[0x%x]にinst:%xを書き込みました\n", tmp,inst_data);
    }

    for(int i=0;i<10;++i){
        tmp=address+i*4;
        word_set_value(&addr,tmp);
        inst_memory_run(&im, addr, &inst);
        printf("InstMemory[0x%x]からinst:%xを読み込みました\n", tmp,word_get_value(inst));
    }

}

void test_data_memory()
{
    DataMemory dm;
    Signal mem_write, mem_read;
    Word addr, wdata, rdata;
    
    word_set_value(&rdata, 0);
    mem_write = true;
    mem_read = false;

    word_set_value(&addr, 0x10000004);
    word_set_value(&wdata, 100);
    data_memory_run(&dm, mem_write, mem_read, addr, wdata, &rdata);
    printf("DataMemory[0x%x]にwdata:%dを書き込みました\n", word_get_value(addr),word_get_value(wdata));

    word_set_value(&addr, 0x10000008);
    word_set_value(&wdata, 200);
    data_memory_run(&dm, mem_write, mem_read, addr, wdata, &rdata);
    printf("DataMemory[0x%x]にwdata:%dを書き込みました\n", word_get_value(addr),word_get_value(wdata));

    mem_write = false;
    mem_read = true;
    data_memory_run(&dm, mem_write, mem_read, addr, wdata, &rdata);
    printf("DataMemory[0x%x]からrdata : %dを読み込みました\n", word_get_value(addr),word_get_value(rdata));

    word_set_value(&addr, 0x10000004);
    data_memory_run(&dm, mem_write, mem_read, addr, wdata, &rdata);
    printf("DataMemory[0x%x]からrdata : %dを読み込みました\n", word_get_value(addr),word_get_value(rdata));
    

    word_set_value(&addr, 0x10000004);
    int address=word_get_value(addr);
    mem_write = true;
    mem_read = false;
    // 10個の値をメモリに書き込む
    for(int i=0;i<10;++i){
        int tmp=address+i*4;
        word_set_value(&addr, tmp);
        word_set_value(&wdata, i);
        data_memory_run(&dm, mem_write, mem_read, addr, wdata, &rdata);
        printf("DataMemory[0x%x]にwdata:%dを書き込みました\n", word_get_value(addr),word_get_value(wdata));
    }
    
    // メモリに書き込んだ値を読み出す
    mem_write = false;
    mem_read = true;
    word_set_value(&addr, 0x10000004);
    address=word_get_value(addr);
    for(int i=0;i<10;++i){
        int tmp=address+i*4;
        word_set_value(&addr, tmp);
        data_memory_run(&dm, mem_write, mem_read, addr, wdata, &rdata);
        printf("DataMemory[0x%x]からrdata : %dを読み込みました\n", word_get_value(addr),word_get_value(rdata));
    }

    

}


