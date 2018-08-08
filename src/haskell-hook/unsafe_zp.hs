{-# LANGUAGE ForeignFunctionInterface #-}

module UnsafeZp where

-- because we can hook functions as long as the "interfaces" (type signatures)
-- match, as evidenced by hooking Num.(*) with Num.(-), we should be able to
-- compile our own real haskell functions wrapped around an unsafePerformIO to
-- achieve the same result

import Foreign.C.Types
import System.IO.Unsafe

foreign import ccall "foreign_add" foreign_add :: CInt -> CInt -> CInt

-- we need a Show instance for a, which causes there to be a different function
-- signature
unsafe_zp :: (Show a, Num a) => a -> a -> a
unsafe_zp a b = unsafePerformIO $ do
  let c = a + b
  putStrLn $ show a ++ " + " ++ show b ++ " = " ++ show c
  return c

-- if we assume things are Integers, then the compiler can automatically
-- generate the correct code
unsafe_zp' :: Integer -> Integer -> Integer
unsafe_zp' a b = unsafePerformIO $ do
  let c = a + b
  putStrLn $ show a ++ " + " ++ show b ++ " = " ++ show c
  return c

-- we may n eed an actual Num instance in the signature
unsafe_zp'' :: Num a => Integer -> Integer -> a
unsafe_zp'' a b = unsafePerformIO $ do
  let c = a + b
  putStrLn $ show a ++ " + " ++ show b ++ " = " ++ show c
  return (fromInteger c)

unsafe_zp''' :: Integer -> Integer -> Integer
unsafe_zp''' a b = unsafePerformIO $ do
  c <- return . toInteger . length $ [1..fromInteger a] ++ [1..fromInteger b]
  return $ c + 1

-- it's hard to convert from Num to CInt (fromInteger :: Num a => Integer -> a)
-- so we assume Integer here
foreign_zp :: Integer -> Integer -> Integer
foreign_zp a b = toInteger $ foreign_add (fromInteger a) (fromInteger b)

safe_weird_zp :: Num a => a -> a -> a
safe_weird_zp a b = a + b

safe_weird_zp' :: Integer -> Integer -> Integer
safe_weird_zp' a b = a + b + 1
