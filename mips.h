#define INST_MEM_START 0x04000000
#define DATA_MEM_START 0x10000000

typedef enum {
    false,
    true
} Signal;

typedef struct {
    Signal bit[32];
} Word;

typedef struct {
    int val;
} Register;

typedef struct {
    Register r[32];
} RegisterFile;

typedef struct {
    int mem[1024];
} InstMemory;

typedef struct {
    int mem[1024];
} DataMemory;

typedef struct {
    Register pc;
    RegisterFile rf;
    InstMemory im;
    DataMemory dm;
} MIPS;

/* Exercise 5 */
void word_set_value(Word *w, int val);
int word_get_value(Word w);

void and_gate(Signal in1, Signal in2, Signal *out1);
void or_gate(Signal in1, Signal in2, Signal *out1);
void not_gate(Signal in1, Signal *out1);
void nand_circuit(Signal in1, Signal in2, Signal *out1);
void xor_circuit(Signal in1, Signal in2, Signal *out1);
// muxを追加した。
void mux(Signal in1, Signal in2, Signal ctl, Signal *out1);

void full_adder(const Signal in1, const Signal in2, const Signal carry_in, Signal *out1, Signal *carry_out);
void rca(Word in1, Word in2, Signal carry_in, Word *out1, Signal *carry_out);

void andn_gate(Signal *in1, int n, Signal *out1);
void orn_gate(Signal *in1, int n, Signal *out1);

/* Exercise 6 */
void alu(Signal *ops, Signal a, Signal b, Signal less, Signal carry_in, Signal *s, Signal *carry_out);
// 判定のために引数を追加した。
void alu32(Signal *ops, Word a, Word b, Word *s, Signal *zero,Signal *judge_over);

/* Exercise 7 */
void register_run(Register *r, Signal wctl, Word wdata, Word *rdata);
void register_set_value(Register *r, int val);
int register_get_value(Register *r);
void register_file_run(RegisterFile *rf, Signal Register_write, Signal *read1, Signal *read2, Signal *write1, Word wdata, Word *rdata1, Word *rdata2);

void inst_memory_run(InstMemory *im, Word addr, Word *inst);
void inst_memory_set_inst(InstMemory *im, int addr, int inst);
void data_memory_run(DataMemory *dm, Signal mem_write, Signal mem_read, Word addr, Word wdata, Word *rdata);


// void control_unit(Signal opcode[6], Signal *Register_dst, Signal *Register_write, Signal *aluop1);
void control_unit(Signal opcode[6], Signal *register_dst,Signal *alu_src,Signal *mem_to_reg, Signal *register_write, Signal *mem_read, Signal *mem_write, Signal *branch, Signal *aluop1, Signal *aluop0);
void alu_control_unit(Signal *funct, Signal aluop1,Signal aluop0, Signal *ops);

/* Test function */
void test_gate();
void test_full_adder();
void test_rca(int val1, int val2);

void test_alu();
void test_alu32(int val1, int val2);

void test_register_file();
void test_inst_memory();
void test_data_memory();
void test_mips();
