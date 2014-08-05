package main

import (
    "flag"
    "fmt"
    "os"
)
const (
        PtInc = iota
        PtDec
        Inc
        Dec
        PChar
        GChar
        JumpStart
        JumpEnd
)
type Inst struct {
	opcode int
	operand int
}

func makeInst(opcode int, operand int) *Inst {
	inst := &Inst{opcode:opcode, operand:operand};
	return inst;
}

func compile(src string) []*Inst {
	var stack []*Inst;
	var list []*Inst;
	for i := 0;i < len(src); i++ {
		switch(src[i]){
			case '>':
				list = append(list, makeInst(PtInc, 0));
				break;
			case '<':
				list = append(list, makeInst(PtDec, 0));
				break;
			case '+':
				list = append(list, makeInst(Inc, 0));
				break;
			case '-':
				list = append(list, makeInst(Dec, 0));
				break;
			case '.':
				list = append(list, makeInst(PChar, 0));
				break;
			case ',':
				list = append(list, makeInst(GChar, 0));
				break;
			case '[':
				inst := makeInst(JumpStart, len(list));
				list = append(list, inst);
				stack = append(stack, inst);
				break;
			case ']':
				sl := len(stack);
				s := len(list);
				before := stack[sl-1];
				stack = stack[:sl-1];
				list = append(list, makeInst(JumpEnd, before.operand));
				before.operand = s;
				break;
			default:
				fmt.Printf("Ignoring: %s\n", src[i]);
				break;
		}
	}
	return list;
}

func run(src []*Inst) {
	var pc = 0;
	var ptr = 0;
	var end = len(src);
	mem := [30000]byte{};
	var b []byte = make([]byte, 1);
	for (pc < end-1) {
		inst := src[pc];
		switch(inst.opcode){
		    case PtInc:
		    	pc++;
		    	ptr++;
		    	break;
		    case PtDec:
		    	pc++;
		    	ptr--;
		    	break;
		    case Inc:
		    	pc++;
		    	mem[ptr]++;
		    	break;
		    case Dec:
		    	pc++;
		    	mem[ptr]--;
		    	break;
		    case PChar:
		    	pc++;
		    	b[0] = mem[ptr];
		    	fmt.Printf("%s", string(b));
		    	break;
		    case GChar:
		    	pc++;
		    	os.Stdin.Read(b)
		    	mem[ptr] = b[0];
		    	break;
		    case JumpStart:
		    	pc++;
		    	if(mem[ptr] == 0){
		    		pc = inst.operand;
				}
		    	break;
		    case JumpEnd:
		    	pc++;
		    	if(mem[ptr] != 0){
		    		pc = inst.operand;
				}
		    	break;
		    default:
				msg := fmt.Sprintf("Unknown opcode: %d\n", inst.opcode);
				fmt.Printf(msg);
				panic(msg);
		}
	}
}

func usage() {
	fmt.Fprintf(os.Stderr, "usage: go run bf.go <sourcefile>\n");
    flag.PrintDefaults();
    os.Exit(2);
}

func main() {
    flag.Usage = usage
    flag.Parse();
    args := flag.Args();
    if(len(args) < 1){
    	usage();
    }

	fmt.Printf("src: %s\n", args[0]);
	inFile,_ := os.Open(args[0]);
	fi,_ := inFile.Stat();
	buff := make([]byte, fi.Size());
	inFile.Read(buff);
	inFile.Close();
	src := compile(string(buff));
	run(src);
}

