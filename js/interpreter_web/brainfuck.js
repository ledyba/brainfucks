function exec(){
	var machine = new Machine();
	clear_output();
	print("---start executing---");
	machine.process();
	print("\n---end executing---");
}

function char_pt_inc(machine){
	machine.pt++;
}
function char_pt_dec(machine){
	machine.pt--;
}

function char_pt_add(machine){
	if(machine.mem[machine.pt] == undefined){
		machine.mem[machine.pt] = 0;
	}
	machine.mem[machine.pt]++;
	machine.mem[machine.pt] %= 256;
}

function char_pt_minus(machine){
	if(machine.mem[machine.pt] == undefined){
		machine.mem[machine.pt] = 0;
	}
	machine.mem[machine.pt]--;
	if(machine.mem[machine.pt] < 0){
		machine.mem[machine.pt] += 256;
	}
}

function char_out(machine){
	put(String.fromCharCode(machine.mem[machine.pt]));
}

function char_in(machine){
	machine.mem[machine.pt] = get(machine);
}

/*
 * FIXME: ループ系の実装の汚さは異常
 */

function char_loop_in(machine){
	if(machine.mem[machine.pt] != 0 && machine.mem[machine.pt] != undefined){
		machine.loop_stack[machine.loop_stack_idx] = machine.eip;
		machine.loop_stack_idx++;
	}else{//7と6だけサーチする。ここは実装が汚すぎるけど、まあいいや。
		var cnt = 1;
		var dec = machine.opcode[7];
		var inc = machine.opcode[6];
		//もうこの後（コマンドが実行された後）はindexが0になるのは分かっているので、
		//ここでクリアしてしまっても別に良い。
		dec.clear();
		inc.clear();
		//対応する"]"を検索
		for(var i=machine.eip+1;i < machine.src.length && cnt > 0;i++){
			var char = machine.src.charAt(i);
			if(dec.process(char,false)){
				dec.clear();
				cnt--;
			}
			if(inc.process(char,false)){
				inc.clear();
				cnt++;
			}
		}
		machine.eip = i-1;
	}
}

function char_loop_ret(machine){
	machine.loop_stack_idx--;
	if(machine.mem[machine.pt] != 0 && machine.mem[machine.pt] != undefined){
		machine.eip = machine.loop_stack[machine.loop_stack_idx]-machine.opcode[6].code.length;
	}
}

function Machine(){
	this.mem = new Array();
	this.pt = 0;
	this.opcode = [
		new Opcode(this,document.getElementById("char_pt_inc").value,char_pt_inc),
		new Opcode(this,document.getElementById("char_pt_dec").value,char_pt_dec),
		new Opcode(this,document.getElementById("char_pt_add").value,char_pt_add),
		new Opcode(this,document.getElementById("char_pt_minus").value,char_pt_minus),
		new Opcode(this,document.getElementById("char_out").value,char_out),
		new Opcode(this,document.getElementById("char_in").value,char_in),
		new Opcode(this,document.getElementById("char_loop_in").value,char_loop_in),
		new Opcode(this,document.getElementById("char_loop_ret").value,char_loop_ret),
	];
	this.src = document.getElementById("source").value;
	this.loop_stack = new Array();
	this.loop_stack_idx = 0;
	this.eip = 0;
	this.input_index = 0;
	this.input_left = undefined;

	/*以下メソッド定義*/
	this.process = function(){
		for(;this.eip<this.src.length;this.eip++){
			this.process_char(this.src.charAt(this.eip));
		}
	}
	this.process_char = function(char){
		for(var i=0;i<8;i++){
			var op = this.opcode[i];
			if(op.process(char,true)){
				this.all_clear();
				break;
			}
		}
	}
	this.all_clear = function(){
		for(var i=0;i<8;i++){
			this.opcode[i].clear();
		}
	}

}

function Opcode(machine,code,method){
	this.code = code;
	this.method = function(machine){method(machine);};
	this.index = 0;
	this.machine = machine;
	this.process = function(char,exec){
		if(char == this.code.charAt(this.index)){
			this.index++;
			if(this.index >= this.code.length){
				if (exec) {
					this.method(this.machine);
				}
				return true;
			}
		}else{
			this.index = 0;
		}
		return false;
	}
	this.clear = function(){
		this.index = 0;
	}
}



//IO
function clear_output(){
	document.getElementById("output").value = "";
}
function print(char){
	document.getElementById("output").value += char
	document.getElementById("output").value += "\n";
}

function put(char){
	document.getElementById("output").value += char;
}

function get(machine){
	var ret = 0;
	if(machine.input_left == undefined){
		ret = (document.getElementById("input").value).charCodeAt(machine.input_index);
		machine.input_left = ret >> 8;
		ret &= 0xff;
	}else if(machine.input_left == 0){
		machine.input_index++;
		ret = (document.getElementById("input").value).charCodeAt(machine.input_index);
		machine.input_left = ret >> 8;
		ret &= 0xff;
	}else{
		ret = machine.input_left & 0xff;
		machine.input_left >>= 8;
	}
	return ret;
}

