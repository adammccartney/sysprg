--- conversions.hs: various modules and helpers for numeric conversions
import Numeric (showHex, showIntAtBase)

base = 13884

limit = base + 290

calcRadds vAdds = [x + base | x <- vAdds]

showHexRadds rAdds = [showHex x "" | x <- rAdds]

--- drill calculations
vAddresses1 = [782, 261, 507, 460, 667]

rAdds1 = calcRadds vAddresses1

show rAdds1

showHexRadds rAdds1

filter (< 14174) rAdds1

--- redefine base and limit 
base = 15529

limit = base + 500

calcRadds vAdds = [x + base | x <- vAdds]

vAddresses2 = [57, 86, 855, 753, 685]

rAdds2 = calcRadds vAddresses2

show rAdds2

showHexRadds rAdds2

filter (< limit) rAdds2

--- redefine base and limit 
base = 8916

limit = base + 316

vAddresses3 = [378, 618, 640, 67, 13]

rAdds3 = calcRadds vAddresses3

show rAdds3

showHexRadds rAdds3

filter (< limit) rAdds3
