module Token
  ( Token(..)
  ) where

data Token a = BinOp String (a -> a -> a)
             | UnaryOp String (a -> a)
             | Numeral a

instance Show a => Show (Token a) where
  show (BinOp op _) = "(" ++ op ++ ")"
  show (UnaryOp op _) = "(" ++ op ++ ")"
  show (Numeral a)  = show a
