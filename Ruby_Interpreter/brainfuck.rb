module Brainfuck
	class VM
		def initialize(src)
			@Codes = compile(src)
			@Mem = Array.new(65536,0)
			@Ptr = 65536/2;
			@Eip = 0;
		end
		attr_accessor :Ptr,:Eip,:Mem
		def run()
			while @Eip < @Codes.size
				@Codes[@Eip].exec()
				@Eip+=1;
			end
		end
		@private
		def compile(src)
			codes = []
			jump_stack = []
			src.split('').each{|char|
				case char
					when ">"
						codes.push(Brainfuck::OpCodes::PtrInc.new(self))
					when "<"
						codes.push(Brainfuck::OpCodes::PtrDec.new(self))
					when "+"
						codes.push(Brainfuck::OpCodes::Inc.new(self))
					when "-"
						codes.push(Brainfuck::OpCodes::Dec.new(self))
					when "."
						codes.push(Brainfuck::OpCodes::PtrOut.new(self))
					when ","
						codes.push(Brainfuck::OpCodes::PtrIn.new(self))
					when "["
						jump_stack.push(codes.size);
						codes.push(nil);
					when "]"
						idx = jump_stack.pop();
						codes[idx] = Brainfuck::OpCodes::JumpStart.new(self,codes.size)
						codes.push(Brainfuck::OpCodes::JumpEnd.new(self,idx))
					else
				end
			}
			return codes
		end
	end
	#NOP
	module OpCodes
		class NOP
			def initialize(vm)
				@Vm = vm;
			end
			def exec()
			end
		end
		class PtrInc < NOP
			def exec()
				@Vm.Ptr+=1;
			end
		end
		class PtrDec < NOP
			def exec()
				@Vm.Ptr-=1;
			end
		end
		class Inc < NOP
			def exec()
				@Vm.Mem[@Vm.Ptr]+=1;
			end
		end
		class Dec < NOP
			def exec()
				@Vm.Mem[@Vm.Ptr]-=1;
			end
		end
		class PtrOut < NOP
			def exec()
				print @Vm.Mem[@Vm.Ptr].chr;
			end
		end
		class PtrIn < NOP
			def exec()
				@Vm.Mem[@Vm.Ptr] = getc;
			end
		end
		class JumpStart < NOP
			def initialize(vm,jump_addr)
				super(vm)
				@JumpAddr = jump_addr;
			end
			def exec()
				if @Vm.Mem[@Vm.Ptr] == 0
					@Vm.Eip = @JumpAddr;
				end
			end
		end
		class JumpEnd < NOP
			def initialize(vm,jump_addr)
				super(vm)
				@JumpAddr = jump_addr;
			end
			def exec()
				if @Vm.Mem[@Vm.Ptr] != 0
					@Vm.Eip = @JumpAddr;
				end
			end
		end
	end
end

file = open(ARGV[0])
if file == nil
	print "File #{file} is not exist."
	exit();
end
vm = Brainfuck::VM.new(file.read)
file.close;
vm.run()
