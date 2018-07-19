#load "extLib.cma";;
open ExtList;;

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
            machine.idx <- machine.idx + 1;
            machine.pc <- machine.pc+1
    | PtDec  ->
            machine.idx <- machine.idx - 1;
            machine.pc <- machine.pc+1
    | ValInc ->
            machine.mem.(machine.idx) <- Char.chr ((Char.code machine.mem.(machine.idx)) + 1);
            machine.pc <- machine.pc+1
    | ValDec ->
            machine.mem.(machine.idx) <- Char.chr ((Char.code machine.mem.(machine.idx)) - 1);
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


let runStep machine =
(*    print_endline (string_of_machine machine);*)
    execOp machine machine.prog.(machine.pc);;
let isEnd machine = machine.pc >= 0 && machine.pc < Array.length machine.prog;;
let rec runMachine machine =
    if not (isEnd machine)
        then ()
        else begin
            runStep machine;
            runMachine machine
        end
;;

let updateList left idx cnt =
    let (before, after) = (List.split_nth ((List.length left) - idx - 1) left)
    in
        List.concat [before; ((LoopStart cnt)::(List.tl after))];;

let rec compileOp src cnt maxlen lstack ops =
    if (cnt >= maxlen)
        then Array.of_list(List.rev ops)
        else match (String.get src cnt) with
            | '<' -> (compileOp src (cnt+1) maxlen lstack (PtDec :: ops));
            | '>' -> (compileOp src (cnt+1) maxlen lstack (PtInc :: ops));
            | '+' -> (compileOp src (cnt+1) maxlen lstack (ValInc :: ops));
            | '-' -> (compileOp src (cnt+1) maxlen lstack (ValDec :: ops));
            | ',' -> (compileOp src (cnt+1) maxlen lstack (GetC :: ops));
            | '.' -> (compileOp src (cnt+1) maxlen lstack (PutC :: ops));
            | '[' -> (compileOp src (cnt+1) maxlen (cnt::lstack) ((LoopStart ~-1) :: ops));
            | ']' ->
                    let idx = List.hd lstack and left = List.tl lstack
                    in (compileOp src (cnt+1) maxlen left ((LoopEnd (idx+1)) ::(updateList ops idx cnt)));
            | _ -> (compileOp src (cnt+1) maxlen lstack ops);
;;
let compileOp src = compileOp src 0 (String.length src) [] [];;

let echo = compileOp "+[>,.<]";;
    (* [|ValInc; LoopStart 7; PtInc;GetC;PutC;PtDec; LoopEnd 2;|];; *)
let helloworld = compileOp "+++++++++[>++++++++>+++++++++++>+++++<<<-]>.>++.+++++++..+++.>-.------------.<++++++++.--------.+++.------.--------.>+.";;

(* print source  *)
let m = makeMachine helloworld;;
runMachine m ;;
print_endline "";;
