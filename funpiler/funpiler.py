from lark import Lark, Transformer
from llvmlite import ir
import subprocess

# Load grammar
with open("grammar.lark") as f:
    parser = Lark(f.read())

# Transformer: turns parse tree into IR nodes
class CodeGen(Transformer):
    def __init__(self):
        self.module = ir.Module(name="cool")
        self.globals = {}

        self.define_uart_write()


        # Set up main function and entry block here
        func_type = ir.FunctionType(ir.VoidType(), [])
        main_func = ir.Function(self.module, func_type, name="main")
        block = main_func.append_basic_block(name="entry")
        self.builder = ir.IRBuilder(block)

        self.globals = {}

    def start(self, stmts):
        self.builder.ret_void()
        return self.module

    def define_uart_write(self):
        uart_write_ty = ir.FunctionType(ir.VoidType(), [ir.PointerType(ir.IntType(8))])
        self.uart_write_fn = ir.Function(self.module, uart_write_ty, name="uart_write")

        # Address of PL011 UARTDR on QEMU virt
        uart_addr_const = ir.Constant(ir.IntType(64), 0x09000000)

        # === Function blocks ===
        entry_block = self.uart_write_fn.append_basic_block(name="entry")
        loop_block = self.uart_write_fn.append_basic_block(name="loop")
        send_block = self.uart_write_fn.append_basic_block(name="send")
        done_block = self.uart_write_fn.append_basic_block(name="done")

        builder = ir.IRBuilder(entry_block)

        # ptr = arg0
        ptr_arg = self.uart_write_fn.args[0]
        ptr_arg.name = "ptr"

        ptr_var = builder.alloca(ptr_arg.type, name="ch_ptr")
        builder.store(ptr_arg, ptr_var)

        builder.branch(loop_block)

        # === loop block ===
        builder.position_at_start(loop_block)
        cur_ptr = builder.load(ptr_var, name="cur")
        ch = builder.load(cur_ptr, name="ch")
        is_null = builder.icmp_unsigned("==", ch, ir.Constant(ir.IntType(8), 0))
        builder.cbranch(is_null, done_block, send_block)

        # === send block ===
        builder.position_at_start(send_block)
        uart_ptr = builder.inttoptr(uart_addr_const, ir.PointerType(ir.IntType(32)))
        ch_32 = builder.zext(ch, ir.IntType(32))
        builder.store(ch_32, uart_ptr)

        next_ptr = builder.gep(cur_ptr, [ir.Constant(ir.IntType(32), 1)])
        builder.store(next_ptr, ptr_var)
        builder.branch(loop_block)

        # === done block ===
        builder.position_at_start(done_block)
        builder.ret_void()

    def print_stmt(self, items):
        string = items[0][1:-1]  # remove quotes
        c_str = bytearray(string.encode("utf8") + b"\00")
        str_type = ir.ArrayType(ir.IntType(8), len(c_str))

        global_str = ir.GlobalVariable(self.module, str_type, name=f"str{len(self.globals)}")
        global_str.global_constant = True
        global_str.initializer = ir.Constant(str_type, c_str)

        zero = ir.Constant(ir.IntType(32), 0)
        ptr = self.builder.gep(global_str, [zero, zero], inbounds=True)

        self.builder.call(self.uart_write_fn, [ptr])


    def let_stmt(self, items):
        name = str(items[0])
        value = int(items[1])
        self.globals[name] = ir.Constant(ir.IntType(32), value)
        return

# Compile source
with open("sample.cool") as f:
    tree = parser.parse(f.read())

cg = CodeGen()
mod = cg.transform(tree)

with open("sample.ll", "w") as f:
    f.write(str(mod))
