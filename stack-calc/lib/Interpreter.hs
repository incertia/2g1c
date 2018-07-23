{-# LANGUAGE FlexibleContexts #-}

module Interpreter
  ( interpret
  ) where

import Control.Monad
  (join)

import Stack
  (MonadStack(..), runStack)
import Token
  (Token(..))

interpret :: [Token a] -> Maybe a
interpret = extract . run
  where interpret' :: MonadStack (Maybe a) m => Token a -> m ()
        interpret' x =
          case x of
               BinOp _ f   -> executeBinary f
               UnaryOp _ f -> executeUnary f
               Numeral a   -> push (Just a)
        executeUnary f = do
          a <- pop
          push (f <$> join a)
        executeBinary f = do
          b <- pop
          a <- pop
          push (f <$> join a <*> join b)
        extract = join . fst
        run = runStack . (>> peek) . sequence_ . fmap interpret'
