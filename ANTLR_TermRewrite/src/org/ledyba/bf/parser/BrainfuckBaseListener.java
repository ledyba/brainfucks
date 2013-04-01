// Generated from D:\Dropbox\src\brainfucks\ANTLR_TermRewrite\src\org\ledyba\bf\parser\Brainfuck.g4 by ANTLR 4.0
package org.ledyba.bf.parser;

import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.tree.TerminalNode;
import org.antlr.v4.runtime.tree.ErrorNode;

public class BrainfuckBaseListener implements BrainfuckListener {
	@Override public void enterProg(BrainfuckParser.ProgContext ctx) { }
	@Override public void exitProg(BrainfuckParser.ProgContext ctx) { }

	@Override public void enterLoop(BrainfuckParser.LoopContext ctx) { }
	@Override public void exitLoop(BrainfuckParser.LoopContext ctx) { }

	@Override public void enterOps(BrainfuckParser.OpsContext ctx) { }
	@Override public void exitOps(BrainfuckParser.OpsContext ctx) { }

	@Override public void enterTop(BrainfuckParser.TopContext ctx) { }
	@Override public void exitTop(BrainfuckParser.TopContext ctx) { }

	@Override public void enterEveryRule(ParserRuleContext ctx) { }
	@Override public void exitEveryRule(ParserRuleContext ctx) { }
	@Override public void visitTerminal(TerminalNode node) { }
	@Override public void visitErrorNode(ErrorNode node) { }
}