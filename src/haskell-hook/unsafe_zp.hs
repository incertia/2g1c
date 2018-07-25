-- because we can hook functions as long as the "interfaces" (type signatures)
-- match, as evidenced by hooking Num.(*) with Num.(-), we should be able to
-- compile our own real haskell functions wrapped around an unsafePerformIO to
-- achieve the same result

unsafe_zp :: (Num a) => a -> a -> a
unsafe_zp a b = unsafePerformIO $ do
  -- TODO: the line below will *probably* cause an infinite loop if we hook from
  -- any (+) function
  let c = a + b
  putStrLn $ show a ++ " + " ++ show b ++ " = " ++ show c
  return c
