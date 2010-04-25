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
#include "vm.h"

int main(int argc,char* argv[]) {
	//タイトル表示
	puts("---------------------------------------------------");
	puts("Brainfuck C Interpreter ver 1.00(2008/10/09)");
	puts("                               (C)Fairy and Rockets 2008");
	puts("---------------------------------------------------");
	//足りない
	if(argc < 2){
		puts("usage: bfvm [filename]");
		return EXIT_FAILURE;
	}
	//実行
	const char* filename = argv[1];
	puts("sctipt compiling...");
	VM* vm = makeVM(filename);
	puts("script compiling ended.");
	puts("start executing...\n----");
	execVM(vm);
	puts("\n----\nend executing.");
	freeVM(vm);
	return EXIT_SUCCESS;
}
