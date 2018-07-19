// Generated from D:\Dropbox\src\brainfucks\ANTLR_TermRewrite\src\org\ledyba\bf\parser\Brainfuck.g4 by ANTLR 4.0
package org.ledyba.bf.parser;
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.misc.*;
import org.antlr.v4.runtime.tree.*;
import java.util.List;
import java.util.Iterator;
import java.util.ArrayList;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class BrainfuckParser extends Parser {
	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		INPUT=1, OUTPUT=2, INC=3, DEC=4, PINC=5, PDEC=6, LOOP_START=7, LOOP_END=8, 
		WS=9;
	public static final String[] tokenNames = {
		"<INVALID>", "','", "'.'", "'+'", "'-'", "'>'", "'<'", "'['", "']'", "WS"
	};
	public static final int
		RULE_top = 0, RULE_prog = 1, RULE_loop = 2, RULE_ops = 3;
	public static final String[] ruleNames = {
		"top", "prog", "loop", "ops"
	};

	@Override
	public String getGrammarFileName() { return "Brainfuck.g4"; }

	@Override
	public String[] getTokenNames() { return tokenNames; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public ATN getATN() { return _ATN; }

	public BrainfuckParser(TokenStream input) {
		super(input);
		_interp = new ParserATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}
	public static class TopContext extends ParserRuleContext {
		public ProgContext prog() {
			return getRuleContext(ProgContext.class,0);
		}
		public TopContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_top; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof BrainfuckListener ) ((BrainfuckListener)listener).enterTop(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof BrainfuckListener ) ((BrainfuckListener)listener).exitTop(this);
		}
	}

	public final TopContext top() throws RecognitionException {
		TopContext _localctx = new TopContext(_ctx, getState());
		enterRule(_localctx, 0, RULE_top);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(8); prog();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ProgContext extends ParserRuleContext {
		public OpsContext ops(int i) {
			return getRuleContext(OpsContext.class,i);
		}
		public LoopContext loop(int i) {
			return getRuleContext(LoopContext.class,i);
		}
		public List<LoopContext> loop() {
			return getRuleContexts(LoopContext.class);
		}
		public List<OpsContext> ops() {
			return getRuleContexts(OpsContext.class);
		}
		public ProgContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_prog; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof BrainfuckListener ) ((BrainfuckListener)listener).enterProg(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof BrainfuckListener ) ((BrainfuckListener)listener).exitProg(this);
		}
	}

	public final ProgContext prog() throws RecognitionException {
		ProgContext _localctx = new ProgContext(_ctx, getState());
		enterRule(_localctx, 2, RULE_prog);
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(14);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,1,_ctx);
			while ( _alt!=2 && _alt!=-1 ) {
				if ( _alt==1 ) {
					{
					setState(12);
					switch ( getInterpreter().adaptivePredict(_input,0,_ctx) ) {
					case 1:
						{
						setState(10); loop();
						}
						break;

					case 2:
						{
						setState(11); ops();
						}
						break;
					}
					} 
				}
				setState(16);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,1,_ctx);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class LoopContext extends ParserRuleContext {
		public ProgContext prog() {
			return getRuleContext(ProgContext.class,0);
		}
		public TerminalNode LOOP_START() { return getToken(BrainfuckParser.LOOP_START, 0); }
		public TerminalNode LOOP_END() { return getToken(BrainfuckParser.LOOP_END, 0); }
		public LoopContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_loop; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof BrainfuckListener ) ((BrainfuckListener)listener).enterLoop(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof BrainfuckListener ) ((BrainfuckListener)listener).exitLoop(this);
		}
	}

	public final LoopContext loop() throws RecognitionException {
		LoopContext _localctx = new LoopContext(_ctx, getState());
		enterRule(_localctx, 4, RULE_loop);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(17); match(LOOP_START);
			setState(18); prog();
			setState(19); match(LOOP_END);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class OpsContext extends ParserRuleContext {
		public TerminalNode INPUT() { return getToken(BrainfuckParser.INPUT, 0); }
		public TerminalNode DEC() { return getToken(BrainfuckParser.DEC, 0); }
		public TerminalNode INC() { return getToken(BrainfuckParser.INC, 0); }
		public TerminalNode PDEC() { return getToken(BrainfuckParser.PDEC, 0); }
		public TerminalNode LOOP_START() { return getToken(BrainfuckParser.LOOP_START, 0); }
		public TerminalNode OUTPUT() { return getToken(BrainfuckParser.OUTPUT, 0); }
		public TerminalNode LOOP_END() { return getToken(BrainfuckParser.LOOP_END, 0); }
		public TerminalNode PINC() { return getToken(BrainfuckParser.PINC, 0); }
		public OpsContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_ops; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof BrainfuckListener ) ((BrainfuckListener)listener).enterOps(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof BrainfuckListener ) ((BrainfuckListener)listener).exitOps(this);
		}
	}

	public final OpsContext ops() throws RecognitionException {
		OpsContext _localctx = new OpsContext(_ctx, getState());
		enterRule(_localctx, 6, RULE_ops);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(21);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << INPUT) | (1L << OUTPUT) | (1L << INC) | (1L << DEC) | (1L << PINC) | (1L << PDEC) | (1L << LOOP_START) | (1L << LOOP_END))) != 0)) ) {
			_errHandler.recoverInline(this);
			}
			consume();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static final String _serializedATN =
		"\2\3\13\32\4\2\t\2\4\3\t\3\4\4\t\4\4\5\t\5\3\2\3\2\3\3\3\3\7\3\17\n\3"+
		"\f\3\16\3\22\13\3\3\4\3\4\3\4\3\4\3\5\3\5\3\5\2\6\2\4\6\b\2\3\3\3\n\27"+
		"\2\n\3\2\2\2\4\20\3\2\2\2\6\23\3\2\2\2\b\27\3\2\2\2\n\13\5\4\3\2\13\3"+
		"\3\2\2\2\f\17\5\6\4\2\r\17\5\b\5\2\16\f\3\2\2\2\16\r\3\2\2\2\17\22\3\2"+
		"\2\2\20\16\3\2\2\2\20\21\3\2\2\2\21\5\3\2\2\2\22\20\3\2\2\2\23\24\7\t"+
		"\2\2\24\25\5\4\3\2\25\26\7\n\2\2\26\7\3\2\2\2\27\30\t\2\2\2\30\t\3\2\2"+
		"\2\4\16\20";
	public static final ATN _ATN =
		ATNSimulator.deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
	}
}