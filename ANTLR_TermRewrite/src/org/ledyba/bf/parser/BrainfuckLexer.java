// Generated from D:\Dropbox\src\brainfucks\ANTLR_TermRewrite\src\org\ledyba\bf\parser\Brainfuck.g4 by ANTLR 4.0
package org.ledyba.bf.parser;
import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.misc.*;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class BrainfuckLexer extends Lexer {
	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		INPUT=1, OUTPUT=2, INC=3, DEC=4, PINC=5, PDEC=6, LOOP_START=7, LOOP_END=8, 
		WS=9;
	public static String[] modeNames = {
		"DEFAULT_MODE"
	};

	public static final String[] tokenNames = {
		"<INVALID>",
		"','", "'.'", "'+'", "'-'", "'>'", "'<'", "'['", "']'", "WS"
	};
	public static final String[] ruleNames = {
		"INPUT", "OUTPUT", "INC", "DEC", "PINC", "PDEC", "LOOP_START", "LOOP_END", 
		"WS"
	};


	public BrainfuckLexer(CharStream input) {
		super(input);
		_interp = new LexerATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@Override
	public String getGrammarFileName() { return "Brainfuck.g4"; }

	@Override
	public String[] getTokenNames() { return tokenNames; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String[] getModeNames() { return modeNames; }

	@Override
	public ATN getATN() { return _ATN; }

	@Override
	public void action(RuleContext _localctx, int ruleIndex, int actionIndex) {
		switch (ruleIndex) {
		case 8: WS_action((RuleContext)_localctx, actionIndex); break;
		}
	}
	private void WS_action(RuleContext _localctx, int actionIndex) {
		switch (actionIndex) {
		case 0: skip();  break;
		}
	}

	public static final String _serializedATN =
		"\2\4\13,\b\1\4\2\t\2\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4"+
		"\t\t\t\4\n\t\n\3\2\3\2\3\3\3\3\3\4\3\4\3\5\3\5\3\6\3\6\3\7\3\7\3\b\3\b"+
		"\3\t\3\t\3\n\6\n\'\n\n\r\n\16\n(\3\n\3\n\2\13\3\3\1\5\4\1\7\5\1\t\6\1"+
		"\13\7\1\r\b\1\17\t\1\21\n\1\23\13\2\3\2\3\5\13\f\17\17\"\",\2\3\3\2\2"+
		"\2\2\5\3\2\2\2\2\7\3\2\2\2\2\t\3\2\2\2\2\13\3\2\2\2\2\r\3\2\2\2\2\17\3"+
		"\2\2\2\2\21\3\2\2\2\2\23\3\2\2\2\3\25\3\2\2\2\5\27\3\2\2\2\7\31\3\2\2"+
		"\2\t\33\3\2\2\2\13\35\3\2\2\2\r\37\3\2\2\2\17!\3\2\2\2\21#\3\2\2\2\23"+
		"&\3\2\2\2\25\26\7.\2\2\26\4\3\2\2\2\27\30\7\60\2\2\30\6\3\2\2\2\31\32"+
		"\7-\2\2\32\b\3\2\2\2\33\34\7/\2\2\34\n\3\2\2\2\35\36\7@\2\2\36\f\3\2\2"+
		"\2\37 \7>\2\2 \16\3\2\2\2!\"\7]\2\2\"\20\3\2\2\2#$\7_\2\2$\22\3\2\2\2"+
		"%\'\t\2\2\2&%\3\2\2\2\'(\3\2\2\2(&\3\2\2\2()\3\2\2\2)*\3\2\2\2*+\b\n\2"+
		"\2+\24\3\2\2\2\4\2(";
	public static final ATN _ATN =
		ATNSimulator.deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
	}
}