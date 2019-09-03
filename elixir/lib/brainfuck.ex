defmodule Brainfuck do
  defmodule Machine do
    defstruct \
      mem: List.duplicate(0, 10000),
      pc: 0,
      ptr: 0,
      out: ""
    @type mem :: list(non_neg_integer)
    @type pc :: non_neg_integer
    @type ptr :: non_neg_integer
    @type out :: String.t
  end

  @spec exec_loop(list(String.t), non_neg_integer(), map(), Machine.t) :: String.t
  defp exec_loop(src, src_len, jumps, m) do
    m = case Enum.at(src, m.pc) do
      ">" -> %{m | pc: m.pc+1, ptr: m.ptr+1}
      "<" -> %{m | pc: m.pc+1, ptr: m.ptr-1}
      "+" -> %{m | pc: m.pc+1, mem: List.replace_at(m.mem, m.ptr, Enum.at(m.mem, m.ptr)+1)}
      "-" -> %{m | pc: m.pc+1, mem: List.replace_at(m.mem, m.ptr, Enum.at(m.mem, m.ptr)-1)}
      "," ->
        dat = IO.getn(1)
        %{m | pc: m.pc+1, mem: List.replace_at(m.mem, m.ptr, hd String.codepoints(dat))}
      "." ->
        chr = << Enum.at(m.mem, m.ptr) :: 8 >>
        out = m.out <> chr
        %{m | pc: m.pc+1, out: out}
      "[" ->
        case Enum.at(m.mem, m.ptr) do
          0 -> %{m | pc: Map.get(jumps, m.pc) + 1}
          _ -> %{m | pc: m.pc + 1}
        end
      "]" ->
        case Enum.at(m.mem, m.ptr) do
          0 -> %{m | pc: m.pc + 1}
          _ -> %{m | pc: Map.get(jumps, m.pc) + 1}
        end
      _ -> %{m | pc: m.pc + 1}
    end
    if m.pc < src_len do
      exec_loop(src, src_len, jumps, m)
    else
      m.out
    end
  end

  @spec construct_jump(non_neg_integer, list(String.t), list(non_neg_integer()), map()) :: map()
  defp construct_jump(idx, src, stack, acc) do
    case src do
      ["[" | left] ->
        construct_jump(idx+1, left, [idx|stack], acc)
      ["]" | left] ->
        [before | next_stack] = stack
        acc = Map.put(acc, idx, before)
        acc = Map.put(acc, before, idx)
        construct_jump(idx+1, left, next_stack, acc)
      [_ | left] ->
        construct_jump(idx+1, left, stack, acc)
      [] -> acc
    end
end

  @spec exec(String.t) :: String.t
  def exec(src_text) do
    src = String.codepoints(src_text)
    exec_loop(src, length(src), construct_jump(0, src, [], %{}), %Machine{})
  end

end
