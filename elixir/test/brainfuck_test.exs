defmodule BrainfuckTest do
  use ExUnit.Case
  @src """
  >+++++++++[<++++++++>-]<.>+++++++[<++++>-]<+.+++++++..+++.[-]>++++++++[<++++>-]<
  .>+++++++++++[<+++++>-]<.>++++++++[<+++>-]<.+++.------.--------.[-]>++++++++[<++
  ++>-]<+.[-]++++++++++.
  """
  test "Hello, world!" do
    res = Brainfuck.exec(@src)
    assert(res == "Hello World!\n")
  end
end
