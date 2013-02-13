let compose f g x = f (g x);;
type op = PtInc | PtDec | ValInc | ValDec | PutC | GetC | LoopStart of int | LoopEnd of int;;
let string_of_op = function
    | PtInc  -> "PtInc"
    | PtDec  -> "OPtDec"
    | ValInc -> "ValInc"
    | ValDec -> "ValDec"
    | PutC -> "PutC"
    | GetC -> "GetC"
    | LoopStart x -> "LoopStart" ^ (string_of_int x)
    | LoopEnd x -> "LoopEnd" ^ (string_of_int x)
    ;;
type program = op array;;
type memory = char array;;
type machine = {
    prog : program;
    mutable pc : int;
    mutable mem : memory;
    mutable idx : int;
};;
let string_of_machine machine =
"Machine {prog: |" ^
    (Array.fold_left (fun str op -> str ^ "; " ^ (string_of_op op))
    ("len:"^(string_of_int (Array.length machine.prog))) machine.prog ) ^
"| pc: "^ string_of_int(machine.pc) ^"}" ^
" mem: (len: "^ string_of_int (Array.length machine.mem) ^ " idx: " ^ (string_of_int machine.idx) ^ ")"
;;

let makeMachine prg = {prog = prg; pc = 0; mem = (Array.make 3000 (Char.chr 0)); idx=0;};;

let execOp machine = function 
    | PtInc  ->
            machine.idx <- machine.idx+1;
            machine.pc <- machine.pc+1
    | PtDec  ->
            machine.idx <- machine.idx-1;
            machine.pc <- machine.pc+1
    | ValInc ->
            machine.mem.(machine.idx) <- Char.chr ((Char.code machine.mem.(machine.idx))+1);
            machine.pc <- machine.pc+1
    | ValDec ->
            machine.mem.(machine.idx) <- Char.chr ((Char.code machine.mem.(machine.idx))-1);
            machine.pc <- machine.pc+1
    | PutC ->
            output_char stdout machine.mem.(machine.idx);
            machine.pc <- machine.pc+1
    | GetC ->
            machine.mem.(machine.idx) <- ( input_char stdin ) ;
            machine.pc <- machine.pc+1
    | LoopStart x ->
            machine.pc <- if machine.mem.(machine.idx) = (Char.chr 0) then x else (machine.pc+1);
    | LoopEnd x ->
            machine.pc <- if machine.mem.(machine.idx) <> (Char.chr 0) then x else (machine.pc+1);
;;

let runStep machine = execOp machine machine.prog.(machine.pc);;
let isEnd machine = machine.pc >= 0 && machine.pc < Array.length machine.prog;;
let rec runMachine machine =
    if not (isEnd machine)
        then ()
        else
            runStep machine;
            runMachine machine;;

let echo = [|ValInc; LoopStart 7; PtInc;GetC;PutC;PtDec; LoopEnd 2;|];;

(* print source  *)
print_endline (string_of_machine (makeMachine echo));;
runMachine ( makeMachine echo) ;;

