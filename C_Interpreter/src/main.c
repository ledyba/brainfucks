/*
 ============================================================================
 Name        : BrainfuckC.c
 Author      : PSI
 Version     :
 Copyright   : (C)ψ（プサイ）の興味関心空間
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ast.h"
#include "middle.h"
#include "vm.h"

#define MODE_AST 1
#define MODE_MIDDLE 2
#define MODE_VM 4
#define MODE_JIT 8

void usage(){
	puts("usage: bfvm [-ast/-middle/-vm/-jit] [filename]");
	fflush(stdout);
	exit(0);
}

int main(int argc,char* argv[]) {
	clock_t start, end;
	//タイトル表示
	puts("---------------------------------------------------");
	puts("Brainfuck C Interpreter ver 2.00(2010/11/29)");
	puts("                               (C)Fairy and Rockets 2010");
	puts("---------------------------------------------------");
	fflush(stdout);
	//足りない
	if(argc < 2){
		usage();
	}
	int i;
	const char* filename = 0;
	int mode = 0;
	int debug = 0;
	for(i=1;i<argc;i++){
		if(strncmp(argv[i],"-ast",4) == 0){
			mode |= MODE_AST;
		}else if(strncmp(argv[i],"-middle",7) == 0){
			mode |= MODE_MIDDLE;
		}else if(strncmp(argv[i],"-vm",3) == 0){
			mode |= MODE_VM;
		}else if(strncmp(argv[i],"-jit",4) == 0){
			mode |= MODE_JIT;
		}else if(strncmp(argv[i],"-debug",6) == 0){
			debug = 1;
		}else if(strncmp(argv[i],"-",1) == 0){
			continue;
		}else{
			filename = argv[i];
			break;
		}
	}
	if(!filename){
		usage();
		return EXIT_SUCCESS;
	}
	printf("target source: %s\n",filename);
	//ASTへの変換
	printf("sctipt converted to AST...");
	fflush(stdout);
	start = clock();
	AST* ast = AST_new(filename);
	end = clock();
	printf("done in %d ms.\n",(int)(end-start));
	fflush(stdout);
	//中間形式への変換
	printf("AST converted to Middle Code...");
	fflush(stdout);
	start = clock();
	Middle* middle = Middle_new(ast);
	end = clock();
	printf("done in %d ms.\n",(int)(end-start));
	fflush(stdout);

	/**
	 * 各モードでの実行を行う
	 */
	if(mode & MODE_AST){
		printf("--------------------\nexecuting in AST mode, preparing...");
		fflush(stdout);
			start = clock();
				ASTMachine* ast_machine = ASTMachine_new(ast);
			end = clock();
		printf("done in %d ms.\n",(int)(end-start));
		fflush(stdout);
		if(debug){
			AST_show(ast);
			fflush(stdout);
		}
		printf("executing...\n--console--\n");
		fflush(stdout);
		start = clock();
			ASTMachine_exec(ast_machine);
		end = clock();
		printf("\n----\nexecuted in %d ms.\n",(int)(end-start));
		fflush(stdout);
		ASTMachine_free(ast_machine);
	}

	if(mode & MODE_MIDDLE){
		printf("--------------------\nexecuting in MiddleCode mode, preparing...");
		fflush(stdout);
			start = clock();
				MiddleMachine* middle_machine = MiddleMachine_new(middle);
			end = clock();
		printf("done in %d ms.\n",(int)(end-start));
		fflush(stdout);
		if(debug){
			Middle_show(middle);
		}
		printf("executing...\n--console--\n");
		fflush(stdout);
		start = clock();
			MiddleMachine_run(middle_machine);
		end = clock();
		printf("\n----\nexecuted in %d ms.\n",(int)(end-start));
		fflush(stdout);
		MiddleMachine_free(middle_machine);
	}

	if(mode & MODE_VM){
		printf("--------------------\nexecuting in VM mode, preparing...");
		fflush(stdout);
			start = clock();
				VM* vm = VM_new(ast);
			end = clock();
		printf("done in %d ms.\n",(int)(end-start));
		fflush(stdout);
		if(debug){
			VM_show(vm);
		}
		printf("executing...\n--console--\n");
		fflush(stdout);
		start = clock();
			VM_exec(vm);
		end = clock();
		printf("\n----\nexecuted in %d ms.\n",(int)(end-start));
		fflush(stdout);
		VM_free(vm);
	}

	//解放
	Middle_free(middle);
	AST_free(ast);
	return EXIT_SUCCESS;
}


