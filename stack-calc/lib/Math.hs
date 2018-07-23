module Math
  ( fact
  ) where

fact :: (Enum a, Num a) => a -> a
fact = product . enumFromTo 1
