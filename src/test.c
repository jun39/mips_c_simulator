#include <mips.h>
// intの最大値を表すヘッダファイルを追加
#include <limits.h>

/* ゲート、プロセッサーごとにテスト。
テストしたい場所のコメントアウトを外してmakeしてください
*/
int main()
{
    
    // test_gate();

    // test_full_adder();

   
    // test_rca(100, 200); 
    // test_rca(100, -200); 
    // test_rca(-100, -200); 
    // test_rca(-100, 200); 
    // 合計４パターンの２つの値の正負の組み合わせをテストしている。



    // あとマイナスのやつも入れる
    // 最大値で最後の繰り上がりがどうなるのか 32ビットで最上位が1同士のたしざん cでintのmax出すライブラリを探す
    // 負の数でやってみる 

   
    // test_alu(); 
    // test_alu32(100, 100);
    // test_alu32(100, 200);
    // test_alu32(450, 201);
    // test_alu32(-100, -200);
    // test_alu32(-200, -100);
    // test_alu32(100, -200);
    // test_alu32(-200, 100);





    // オーバーフローの場合
    // test_alu32(INT_MAX,INT_MAX);
    // test_alu32(INT_MIN,INT_MIN);
    // test_alu32(INT_MAX,INT_MIN);
    // test_alu32(INT_MIN,INT_MAX);




    /* レジスタのテスト*/
    // test_register_file();

    /* 命令・データメモリのテスト */
    // test_inst_memory();
    // test_data_memory();

    test_mips();
    
    return 0;
}
