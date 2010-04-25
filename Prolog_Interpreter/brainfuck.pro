% メモリを自動で拡張する機能搭載。
/**
	ポインタをインクリメントする。ポインタをptrとすると、C言語の「ptr++;」に相当する。
**/
bf_func('>',Mem,Ptr,Stack,PC,NMem,NPtr,NStack,NPC) :-
	length(Mem,MemLength),Ptr < MemLength - 1,
	NPtr is Ptr + 1,
	NMem = Mem,
	NStack = Stack,
	NPC is PC + 1.

bf_func('>',Mem,Ptr,Stack,PC,NMem,NPtr,NStack,NPC) :-
	length(Mem,MemLength),Ptr == MemLength - 1,
	NPtr is Ptr + 1,
	NMem = [Mem|0],
	NStack = Stack,
	NPC is PC + 1.

/**
	ポインタをデクリメントする。C言語の「ptr--;」に相当。
**/

bf_func('<',Mem,Ptr,Stack,PC,NMem,NPtr,NStack,NPC) :-
	Ptr > 0,
	NMem = Mem,
	NPtr is Ptr - 1,
	NStack = Stack,
	NPC is PC + 1.

bf_func('<',Mem,Ptr,Stack,PC,NMem,NPtr,NStack,NPC) :-
	Ptr == 0,
	NMem = [0|Mem],
	NPtr is 0,
	NStack = Stack,
	NPC is PC + 1.

/**
	リスト操作用関数
**/
insert_at(0, X, Ls, [X | Ls]).
insert_at(N, X, [Y | Ls], [Y | Zs]) :-
    N > 1, N1 is N - 1, insert_at(N1, X, Ls, Zs).
remove_at(0, [X | Ls], Ls).
remove_at(N, [X | Ls], [X | Zs]) :-
    N > 0, N1 is N - 1, remove_at(N1, Xs, Zs).
replace_at(N,X,List,NewList) :- 
	remove_at(N,List,AList),
	insert_at(N,X,AList,NewList).


/**
	ポインタが指す値をインクリメントする。C言語の「(*ptr)++;」に相当。
**/
bf_func('+',Mem,Ptr,Stack,PC,NMem,NPtr,NStack,NPC) :- 
	nth0(Ptr,Mem,Num),
	NewNum is (Num+257) mod 256,
	replace_at(Ptr,NewNum,Mem,NMem),
	NPtr = Ptr,
	NStack = Stack,
	NPC is PC + 1.

/**
	ポインタが指す値をデクリメントする。C言語の「(*ptr)--;」に相当。
**/
bf_func('-',Mem,Ptr,Stack,PC,NMem,NPtr,NStack,NPC) :-
	nth0(Ptr,Mem,Num),
	NewNum is (Num+255) mod 256,
	replace_at(Ptr,NewNum,Mem,NMem),
	NPtr = Ptr,
	NStack = Stack,
	NPC is PC + 1.

% ポインタが指す値を出力する。C言語の「putchar(*ptr);」に相当。
bf_func('.',Mem,Ptr,Stack,PC,NMem,NPtr,NStack,NPC) :-
	nth0(Ptr,Mem,Num),
	put_byte(Num),
	NMem = Mem,
	NPtr = Ptr,
	NStack = Stack,
	NPC is PC + 1.

% 1バイトを入力してポインタが指す値に代入する。C言語の「*ptr=getchar();」に相当。
bf_func(',',Mem,Ptr,Stack,PC,NMem,NPtr,NStack,NPC) :-
	get_byte(NewNum),
	replace_at(Ptr,NewNum,Mem,NMem),
	NPtr = Ptr,
	NStack = Stack,
	NPC is PC + 1.

% ポインタが指す値が0なら、対応する ] の直後までジャンプする。C言語の「while(*ptr){」に相当
bf_func('[',Mem,Ptr,Stack,PC,NMem,NPtr,NStack,NPC) :-
	NMem = Mem,NPtr = Ptr,NStack = Stack,NPC is PC + 1,write('g').

% ポインタが指す値が0でないなら、対応する [ にジャンプする。C言語の「}」に相当。
bf_func(']',Mem,Ptr,Stack,PC,NMem,NPtr,NStack,NPC) :-
	NMem = Mem,NPtr = Ptr,NStack = Stack,NPC is PC + 1,write('h').

/*
bf_repeat(StrList,Mem,Ptr,Stack,PC) :- repeat,
	nth0(PC,StrList,Str),	% PCの文字を取得
	bf_func(Str,Mem,Ptr,Stack,PC,NMem,NPtr,NStack,NPC),	%実際に実行して、
	% 終了判定
	length(StrList,StrListLen),(PC >= StrListLen),!.
*/

/*
	これはスタック消費量がハンパないのでは？？
	でも末尾再帰効いてるから大丈夫？
 */
bf_repeat(StrList,Mem,Ptr,Stack,PC) :-
	%継続条件
	length(StrList,StrListLen),(PC < StrListLen),
	nth0(PC,StrList,Str),	% PCの文字を取得
	bf_func(Str,Mem,Ptr,Stack,PC,NMem,NPtr,NStack,NPC),	%実際に実行して、
	bf_repeat(StrList,NMem,NPtr,NStack,NPC). %末尾再帰。

brainfuck(Str) :- atom_chars(Str,StrList),bf_repeat(StrList,[0],0,[0],0).
