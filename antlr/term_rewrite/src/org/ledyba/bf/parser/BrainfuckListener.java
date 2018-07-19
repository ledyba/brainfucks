// Generated from D:\Dropbox\src\brainfucks\ANTLR_TermRewrite\src\org\ledyba\bf\parser\Brainfuck.g4 by ANTLR 4.0
package org.ledyba.bf.parser;
import org.antlr.v4.runtime.tree.*;
import org.antlr.v4.runtime.Token;

public interface BrainfuckListener extends ParseTreeListener {
	void enterProg(BrainfuckParser.ProgContext ctx);
	void exitProg(BrainfuckParser.ProgContext ctx);

	void enterLoop(BrainfuckParser.LoopContext ctx);
	void exitLoop(BrainfuckParser.LoopContext ctx);

	void enterOps(BrainfuckParser.OpsContext ctx);
	void exitOps(BrainfuckParser.OpsContext ctx);

	void enterTop(BrainfuckParser.TopContext ctx);
	void exitTop(BrainfuckParser.TopContext ctx);
}