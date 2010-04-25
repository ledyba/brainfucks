Compiled from "Hello.java"
public class Hello extends java.lang.Object{
public Hello();
  Code:
   0:	aload_0
   1:	invokespecial	#1; //Method java/lang/Object."<init>":()V
   4:	return

public static final void main(java.lang.String[]);
  Code:
   0:	bipush	20
   2:	istore_1
   3:	iload_1
   4:	ifle	21
   7:	getstatic	#2; //Field java/lang/System.out:Ljava/io/PrintStream;
   10:	ldc	#3; //String Hello, JVM World!
   12:	invokevirtual	#4; //Method java/io/PrintStream.println:(Ljava/lang/String;)V
   15:	iinc	1, 1
   18:	goto	3
   21:	return

}

