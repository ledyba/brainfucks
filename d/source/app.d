import std.stdio;
import std.file;
import std.string;
import bf;

int main(string[] argv) {
  if (argv.length <= 1) {
    stderr.writefln("Please specify the filename");
  }
  File f = File(argv[1], "r");
  string src = "";
  while(!f.eof()) {
    src ~= strip(f.readln());
  }
  auto parser = new Parser(src);
  auto inst = parser.parse();
  auto machine = new Machine(inst);
  //writeln(inst);
  machine.run();
  return 0;
}
