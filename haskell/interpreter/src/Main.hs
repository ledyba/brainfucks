module Main where

import Brainfuck (runMachine, makeMachine, makeProgram, compileProgram)

helloworld :: String
helloworld="+++++++++[>++++++++>+++++++++++>+++++<<<-]>.>++.+++++++..+++.>-.------------.<++++++++.--------.+++.------.--------.>+."

echo :: String
echo = "+[>,.<]"

main::IO()
main = do prg <- return $ makeProgram $ compileProgram echo
          m <- return $ makeMachine prg
          _ <- runMachine m
          putStrLn ""
          return ()
