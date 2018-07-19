package org.ledyba.bf;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

import org.antlr.runtime.TokenStream;
import org.antlr.v4.runtime.ANTLRInputStream;
import org.antlr.v4.runtime.CommonTokenStream;
import org.ledyba.bf.parser.BrainfuckLexer;
import org.ledyba.bf.parser.BrainfuckParser;

public class Runner {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		try {
			final FileInputStream is = new FileInputStream("Brainfuck.txt");
			final ANTLRInputStream input = new ANTLRInputStream(is);
			BrainfuckLexer lex = new BrainfuckLexer(input);
			CommonTokenStream tokens = new CommonTokenStream(lex);
			BrainfuckParser parser = new BrainfuckParser(tokens);
			parser.top();
			System.out.println("parsed");
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

}
