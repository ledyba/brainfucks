/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

grammar Brainfuck;
options {
    output=AST;
}

top : prog;

prog : (loop | ops)*;

loop : LOOP_START prog LOOP_END;

ops : INPUT | OUTPUT | INC | DEC | PINC | PDEC | LOOP_START | LOOP_END;

INPUT:',';
OUTPUT:'.';
INC:'+';
DEC:'-';
PINC:'>';
PDEC:'<';

LOOP_START: '[';
LOOP_END: ']';

WS: [ \n\t\r]+ -> skip;
