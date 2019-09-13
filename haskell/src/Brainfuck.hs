module Brainfuck (
    Program,
    Machine,
    Op(..),
    makeMachine,
    makeProgram,
    compileProgram,
    runMachine
) where

import Control.Monad.State (StateT(..), liftIO)
import Control.Monad (unless)
import Data.Array (Array, (//), (!), bounds, array, listArray)
import Data.Char (chr, ord)

data Op = PtInc | PtDec | ValInc | ValDec | PutC | GetC | LoopStart Int | LoopEnd Int deriving Show
type Program = Array Int Op
type Mem = Array Int Int
data Machine = Machine (Program, Int) (Mem, Int) deriving Show

execOpI :: Op -> Machine -> IO ((), Machine)
execOpI PtInc         (Machine (prg, pc) (mem, idx)) = return ((), Machine (prg, pc + 1) (mem, idx + 1))
execOpI PtDec         (Machine (prg, pc) (mem, idx)) = return ((), Machine (prg, pc + 1) (mem, idx - 1))
execOpI ValInc        (Machine (prg, pc) (mem, idx)) = return ((), Machine (prg, pc + 1) (mem // [(idx, (mem ! idx) + 1)], idx))
execOpI ValDec        (Machine (prg, pc) (mem, idx)) = return ((), Machine (prg, pc + 1) (mem // [(idx, (mem ! idx) - 1)], idx))
execOpI (LoopStart x) (Machine (prg, pc) (mem, idx)) = return ((), Machine (prg, if (mem ! idx) == 0 then x else pc + 1) (mem , idx))
execOpI (LoopEnd   x) (Machine (prg, pc) (mem, idx)) = return ((), Machine (prg, if (mem ! idx) /= 0 then x else pc + 1) (mem , idx))
execOpI PutC          (Machine (prg, pc) (mem, idx)) =
    do putChar (chr (mem ! idx))
       return ((), Machine (prg, pc + 1) (mem , idx))
execOpI GetC          (Machine (prg, pc) (mem, idx)) = do c <- getChar
                                                          return ((), Machine (prg, pc + 1) (mem // [(idx, ord c)], idx))


execOp :: Op -> StateT Machine IO ()
execOp op = StateT $ \t -> execOpI op t

fetchPC :: Machine -> Op
fetchPC (Machine (prg, pc) _) = prg ! pc

fetchPC' :: Machine -> IO (Op, Machine)
fetchPC' m = return (fetchPC m, m)

fetchPC'' :: StateT Machine IO Op
fetchPC'' = StateT $ \t -> liftIO $ fetchPC' t

runStep :: StateT Machine IO ()
runStep = do op <- fetchPC''
             execOp op

isExit :: Machine -> Bool
isExit (Machine (prg, pc) _) = let (start, end) = bounds prg
                             in (start <= pc && pc <= end)

updateLoop :: [Op] -> Int -> Int -> [Op]
updateLoop op idx count = let len = length op
                              (after, before) = splitAt (len-idx-1) op
                          in after ++ LoopStart (count+1) : tail before

compileProgram :: String -> [Op]
compileProgram s = compileProgram' s [] [] 0

compileProgram' :: String -> [Op] -> [Int] -> Int -> [Op]
compileProgram' ('>':xs) acc idxs cnt = compileProgram' xs (PtInc:acc) idxs (cnt+1)
compileProgram' ('<':xs) acc idxs cnt = compileProgram' xs (PtDec:acc) idxs (cnt+1)
compileProgram' ('+':xs) acc idxs cnt = compileProgram' xs (ValInc:acc) idxs (cnt+1)
compileProgram' ('-':xs) acc idxs cnt = compileProgram' xs (ValDec:acc) idxs (cnt+1)
compileProgram' ('.':xs) acc idxs cnt = compileProgram' xs (PutC:acc) idxs (cnt+1)
compileProgram' (',':xs) acc idxs cnt = compileProgram' xs (GetC:acc) idxs (cnt+1)
compileProgram' ('[':xs) acc idxs cnt = compileProgram' xs (LoopStart (-1):acc) (cnt:idxs) (cnt+1)
compileProgram' (']':xs) acc idxs cnt = compileProgram' xs (LoopEnd (1 + head idxs) : updateLoop acc (head idxs) cnt) (tail idxs) (cnt+1)
compileProgram' (_:xs)   acc idxs cnt = compileProgram' xs acc idxs cnt
compileProgram' [] acc _ _ = reverse acc

-- Opのリストからプログラムを作る
makeProgram :: [Op] -> Program
makeProgram lst = listArray (0, length lst - 1) lst

-- プログラムをロードしたマシンを作る
makeMachine :: Program -> Machine
makeMachine prog = Machine (prog, 0) (array (0, 30000) (zip [0..30000] (repeat 0)), 0)

-- プログラムを実行した状態つき計算を組み立てる
runProgram :: StateT Machine IO ()
runProgram =
	do end <- StateT $ \t -> return (isExit t, t)
	   unless (not end) $ do runStep
	                         runProgram

-- プログラムを実行するIOアクションを組み立てる。戻り値は実行した機械そのもの
runMachine :: Machine -> IO Machine
runMachine st = do (_, m) <- runStateT runProgram st
                   return m
