use std::process::exit;

mod bf;

fn main() {
  let args: Vec<String> = std::env::args().collect();
  if args.len() <= 1 {
    println!("usage: {} src.bf", args[0]);
    return;
  }
  let fname: &String = &args[1];
  let src_result = std::fs::read(fname);
  if src_result.is_err() {
    println!("Error: {}", src_result.unwrap_err().to_string());
    exit(-1);
  }
  let src = String::from_utf8(src_result.unwrap());
  if src.is_err() {
    println!("Error: {}", src.unwrap_err().to_string());
    exit(-1);
  }
  let codes = bf::compile(src.unwrap().as_str());
  bf::execute(&codes);
}
