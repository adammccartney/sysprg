module Main where

import qualified Data.ByteString as B
import qualified Data.ByteString.Lazy as BL

import Data.Bits

import Binary.Bits.MaskBits (makeMaskDyn)
import Data.Word

--- conversions.hs: various modules and helpers for numeric conversions
import Numeric (showHex, showIntAtBase)

{- Constants 
   Address Space size = 128     ( 2 ^ 7 )
   Physical memory size = 512   ( 2 ^ 9 )
-}
--- Q. 1
seg0MASK = 0

seg0SHIFT = 7

offset0MASK = seg0MASK + 20

seg1MASK = 512

seg1SHIFT = 7

offset1MASK = seg1MASK + 20

limit1 = base1 + 20

seg0max = base0 + limit0

seg1max = base1 - limit1

transAddSeg0 vAdd = [vAdd + base]
transAddSeg0 vAdd = [base - vAdd]

-- determine segment
-- TODO: figure out how to define the types in this function
whichSeg vAdd segMASK segSHIFT
  | (vAdd .&. segMASK) >> segSHIFT == 0 = 0
  | (vAdd .&. segMASK) >> segSHIFT == 1 = 1

--- drill calculations
vAddresses1 = [108, 97, 53, 33, 65]

rAdds1 = calcRadds vAddresses1

show rAdds1

showHexRadds rAdds1

filter (< 14174) rAdds1
