use std::io::{stdin, stdout, Read, Write};

enum Code {Ptr, Val, In, Out, JumpBeg, JumpEnd}

pub struct Op {
  code: Code,
  value: i16,
}

#[allow(unused_must_use)]
pub fn execute(codes: &Vec<Op>) {
  let mut mem: [u8; 30000] = [0; 30000];
  let mut ptr: usize = 0;
  let mut pc: usize = 0;
  while pc < codes.len() {
    let op = codes.get(pc).unwrap();
    match op.code {
      Code::Ptr => {
        ptr = ((ptr as i16) + op.value) as usize;
        pc+=1;
      }
      Code::Val => {
        mem[ptr] = (mem[ptr] as i16 + op.value) as u8;
        pc+=1;
      }
      Code::In => {
        stdin().read(&mut mem[ptr..ptr+1]);
        pc+=1;
      }
      Code::Out => {
        stdout().write(&mem[ptr..ptr+1]);
        pc+=1;
      }
      Code::JumpBeg => {
        if mem[ptr] == 0 {
          pc = op.value as usize;
        }
        pc+=1;
      }
      Code::JumpEnd => {
        if mem[ptr] != 0 {
          pc = op.value as usize;
        }
        pc+=1;
      }
    }
  }
}

pub fn compile(src: &str) -> Vec<Op> {
  let mut codes: Vec<Op> = Vec::new();
  for c in src.chars() {
    match c {
      '>' => {
        codes.push(Op{
          code: Code::Ptr,
          value: 1,
        });
      }
      '<' => {
        codes.push(Op{
          code: Code::Ptr,
          value: -1,
        });
      }
      '+' => {
        codes.push(Op{
          code: Code::Val,
          value: 1,
        });
      }
      '-' => {
        codes.push(Op{
          code: Code::Val,
          value: -1,
        });
      }
      '[' => {
        codes.push(Op{
          code: Code::JumpBeg,
          value: 0,
        });
      }
      ']' => {
        codes.push(Op{
          code: Code::JumpEnd,
          value: 0,
        });
      }
      ',' => {
        codes.push(Op{
          code: Code::In,
          value: 0,
        });
      }
      '.' => {
        codes.push(Op{
          code: Code::Out,
          value: 0,
        });
      }
      _ => {}
    }
  }
  let mut jumps: Vec<u16> = Vec::new();
  for idx in 0..codes.len() {
    let op : &Op = codes.get(idx).expect("Invalid source code");
    match op.code {
      Code::JumpBeg => {
        jumps.push(idx as u16);
      }
      Code::JumpEnd => {
        let beg = jumps.pop().expect("Invalid source code");
        let end_op : &mut Op = codes.get_mut(idx).expect("Invalid source code");
        end_op.value = beg as i16;

        let beg_op : &mut Op = codes.get_mut(beg as usize).expect("Invalid source code");
        beg_op.value = idx as i16;
      }
      _ => {}
    }
  }
  return codes;
}