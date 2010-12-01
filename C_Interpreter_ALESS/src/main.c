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
#include "vm.h"

int main(int argc,char* argv[]) {
	//タイトル表示
	puts("---------------------------------------------------");
	puts("Brainfuck C Interpreter ver 2.00(2010/11/29)");
	puts("                               (C)Fairy and Rockets 2010");
	puts("---------------------------------------------------");
	fflush(stdout);
	//足りない
	if(argc < 2+2){
		puts("usage: bfvm [filename] factor time");
		fflush(stdout);
		return EXIT_FAILURE;
	}
	//実行
	const char* filename = argv[1];
	int factor = atoi(argv[2]);
	int time = atoi(argv[3]);
	puts("sctipt compiling...");
	fflush(stdout);
	AST* ast = AST_new(filename);
	puts("script compiled.");
	fflush(stdout);
#ifdef DEBUG
	VM_show(vm);
#endif
	puts("start executing...\n----");
	fflush(stdout);
	int i,j;
	clock_t start, end;
	for(i=1;i<=factor;i+=128){
		VM* vm = VM_new(ast,i);
		start = clock();
		for(j=0;j<time;j++){
			VM_run(vm);
		}
		end = clock();
		VM_free(vm);
		printf("%d,%d,\n", i,(int)(end-start));
		fflush(stdout);
	}
	puts("\n----\nend executing.");
	AST_free(ast);
	return EXIT_SUCCESS;
}
