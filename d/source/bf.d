module bf;

import std.stdio;
import std.container.dlist : DList;
import core.stdc.stdio;

enum Op {
  Inc,
  Dec,
  IncPtr,
  DecPtr,
  PutChar,
  GetChar,
  BegLoop,
  EndLoop,
}

struct Inst {
  public Op op;
  public int operand;
  public this(Op op) {
    this.op = op;
    this.operand = 0;
  }
  public this(Op op, int operand) {
    this.op = op;
    this.operand = operand;
  }
}

class Parser {
  const string src;
  this(string src) {
    this.src = src;
  }
  Inst[] parse() {
    auto stack = DList!int();
    Inst[] list;
    foreach (chr; this.src) {
      switch (chr) {
      case '>':
        list ~= Inst(Op.IncPtr);
        break;
      case '<':
        list ~= Inst(Op.DecPtr);
        break;
      case '+':
        list ~= Inst(Op.Inc);
        break;
      case '-':
        list ~= Inst(Op.Dec);
        break;
      case '.':
        list ~= Inst(Op.PutChar);
        break;
      case ',':
        list ~= Inst(Op.GetChar);
        break;
      case '[':
        stack.insertBack(cast(int)list.length);
        list ~= Inst(Op.BegLoop, 0);
        break;
      case ']': {
        int before = stack.back();
        stack.removeBack();
        list[before].operand = cast(int)list.length;
        list ~= Inst(Op.EndLoop, before);
        break;
      }
      default:
        break;
      }
    }
    return list;
  }
}

class Machine {
  ubyte[8192] mem;
  int ptr;
  const Inst[] program;
  ulong pc;
  this(Inst[] inst) {
    this.program = inst;
    this.ptr = 0;
    this.pc = 0;
  }
  void run() {
    while(this.pc < program.length) {
      auto inst = this.program[this.pc];
      //writeln(this.pc, ": ", inst);
      switch (inst.op) {
      case Op.IncPtr:
        this.ptr++;
        break;
      case Op.DecPtr:
        this.ptr--;
        break;
      case Op.Inc:
        this.mem[this.ptr]++;
        break;
      case Op.Dec:
        this.mem[this.ptr]--;
        break;
      case Op.PutChar:
        putchar(this.mem[this.ptr]);
        break;
      case Op.GetChar: {
        int c = getchar();
        this.mem[this.ptr] = cast(ubyte)c;
        break;
      }
      case Op.BegLoop:
        if(this.mem[this.ptr] == 0) {
          this.pc = inst.operand;
        }
        break;
      case Op.EndLoop:
        if(this.mem[this.ptr] != 0) {
          this.pc = inst.operand;
        }
        break;
      default:
        break;
      }
      this.pc++;
    }
  }
}