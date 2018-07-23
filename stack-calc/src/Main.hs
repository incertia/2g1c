module Main
  ( main
  ) where

import Control.Monad.IO.Class
  (MonadIO)
import System.Console.Haskeline
  (InputT, runInputT, defaultSettings, getInputLine, outputStr, outputStrLn,
   MonadException)
import System.Posix.Process
  (getProcessID)
import System.IO
  (hSetBuffering, stdout, BufferMode(NoBuffering))
import Text.Parsec
  (parse)

import Interpreter
  (interpret)
import Parser
  (parseCalculation)

loop :: (MonadIO m, MonadException m) => InputT m ()
loop = do
  l <- getInputLine "> "
  case l of
       Nothing -> return ()
       Just l  -> do
         case parse parseCalculation "<stdin>" l of
              Left e   -> outputStrLn (show e)
              Right sm -> case interpret sm of
                               Just v  -> outputStrLn (show v)
                               Nothing -> outputStrLn "<stack machine error>"
         loop

main :: IO ()
main = do
  hSetBuffering stdout NoBuffering
  pid <- getProcessID
  putStrLn "welcome to the postfix stack machine calculator"
  putStrLn $ "the PID for this process is " ++ show pid ++ " for informational purposes"
  runInputT defaultSettings loop
