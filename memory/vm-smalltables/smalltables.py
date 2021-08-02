#!/usr/bin/python3

pageBits = 5
PDE_SIZE = 32 # unrealistic page size (1024 x 32 bytes = 32KB) 
PDBR = 0x6c
PDE_MASK = 0x7c00 # page directory entry
PDE_SHIFT = 10
PTE_MASK = 0x03e0 # page table entry 
PTE_SHIFT = 5
VPN_MASK = PDE_MASK | PTE_MASK
VPN_SHIFT = PTE_SHIFT
OFFSET_MASK = 0x1f

"""
for each virtual address that we want to translate, 
we first perform the following operation
this will tell us which page directory entry to access
i.e. what our virtual page number is
Then we calculate the page directory index
Then check if the address is valid
"""

pagedir = {
        0 : "1b1d05051d0b19001e00121c1909190c0f0b0a1218151700100a061c06050514",
        1 : "0000000000000000000000000000000000000000000000000000000000000000",
        2 : "121b0c06001e04130f0b10021e0f000c17091717071e001a0f0408120819060b",
        3 : "7f7f7f7fcd7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f887f7f7f7f7f7f7f7fb9",
        4 : "0b041004051c13071b131d0e1b150107080507071b0e1b0411001c000c181e00",
        5 : "17131d0a1202111906081507081d1e041b1101121301171902140e070e040a14",
        6 : "0000000000000000000000000000000000000000000000000000000000000000",
        7 : "0000000000000000000000000000000000000000000000000000000000000000",
        8 : "11101a120f10180a11151e151d0c12170a081e0a1e1a06191e08141702190915",
        9 : "0000000000000000000000000000000000000000000000000000000000000000",
        10 : "0000000000000000000000000000000000000000000000000000000000000000",
        11 : "0910141d04011a18170e150c050c18181d1b151016051c16120d13131b11060d",
        12 : "060b16191c05141d01141a0a07120d050e0c110f090b19071100160a0108071d",
        13 : "19100b0e000614140f1d0e091a08121519180b0101161d0a0d16140814090b10",
        14 : "1218140b000d1c0a07040f10020c141d0d0d0e060c140c12191e1b0b00120e07",
        15 : "0000000000000000000000000000000000000000000000000000000000000000",
        16 : "7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7fea7f7f7f",
        17 : "0000000000000000000000000000000000000000000000000000000000000000",
        18 : "7f7f7f7f7f7fab7f7f7f8e7f7f7fdd7f7f7f7f7f7f7f8b7f7f7f7f7f7f7f7f7f",
        19 : "00130001061402011e0d1b060d0b050a1e170b0c081016150e011c0c0c00041a",
        20 : "1a190402020c1d110807030419041a190411001a11170f151c111b0a03000719",
        21 : "0b081b0e1c151e121e050d111e111a130f0c0b09061d101a1b1d070a13090417",
        22 : "1212150f081b0a0e130f1d1d1c1c120f150608010500140418151e0c1c0e0a03",
        23 : "1d0f030b0c0f1e1e1113140f0f091502091b071d1e110102060a03180b07010b",
        24 : "0000000000000000000000000000000000000000000000000000000000000000",
        25 : "03031c031b0e0e0a0c0b110a1907070e1c0016000c170d0d070e070814121c1e",
        26 : "090e1d18081115180d0c170d070e1d040e130e06001513000917131004150e15",
        27 : "0000000000000000000000000000000000000000000000000000000000000000",
        28 : "0f1d0f0a0211070b0b17071d170e1b0b0b04180c0f0e140b1c0d0b0c171e1a0e",
        29 : "17081e031b010710120c030708171c120118090a10071c050c08101113100c13",
        30 : "7f7f7f7f7f847f7f7f7f977fbd7f7ff47f7f7f7f7f7f7f7f7f7f7f7f7f7f9c7f",
        31 : "7f7f7f7f7f7fd07f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f",
        32 : "0000000000000000000000000000000000000000000000000000000000000000",
        33 : "7f7f7f7f7f7f7f7fb57f9d7f7f7f7f7f7f7f7f7f7f7f7f7f7f7ff6b17f7f7f7f",
        34 : "0413050d0c02161518101105060710190b1b16160a031d1a0c1a1b0a0f0a151c",
        35 : "0000000000000000000000000000000000000000000000000000000000000000",
        36 : "1d1313160c0c1400050a07130b1b110c0c150c14010d0804100f11171b0f090e",
        37 : "1e0f0a0d0c100c021e1e05070d15001913081a1409101e01151a150412180c12",
        38 : "0000000000000000000000000000000000000000000000000000000000000000",
        39 : "1b111e171108150e160c0f001601151218081506100a1e1e06110a1e1c121615",
        40 : "0d030b1007190b0709191c1d0017100307080c0e1d01151a0b07060904110700",
        41 : "7f7f7f7f7f7f7f7fe57f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f8d7f7f7f7f7f",
        42 : "03041501111c1015001312110c0b1e01001d050306181d000d030806140a050f",
        43 : "190802041311011e0e0916000d141d171b030d00080b0a0b180519100a11050f",
        44 : "7f7f7f7f7f7fcc7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7fa27f7f7f7f7f7f",
        45 : "7fb27fef7f7f7f7fa4f57f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f",
        46 : "0000000000000000000000000000000000000000000000000000000000000000",
        47 : "070a0f1002090b0c0e0d020613190f0402040b111410110a14160c19171c0e0a",
        48 : "0000000000000000000000000000000000000000000000000000000000000000",
        49 : "1e0a0f0702030d13101003010b1d05080e1c1d00140714171b151a1804011610",
        50 : "161b040706011a0f020d0d181704130f0004140b1d0f15040e1619060c0e0d0e",
        51 : "14000f1a070a1a0511071d180d02090f1c0311151019101d12120d120b110905",
        52 : "0000000000000000000000000000000000000000000000000000000000000000",
        53 : "0f0c18090e121c0f081713071c1e191b09161b150e030d121c1d0e1a08181100",
        54 : "1901050f031b1c090d11081006090d121008070318031607081614160f1a0314",
        55 : "0000000000000000000000000000000000000000000000000000000000000000",
        56 : "0000000000000000000000000000000000000000000000000000000000000000",
        57 : "1c1d1602020b000a001e19021b0606141d03000b00121a05030a1d041d0b0e09",
        58 : "0000000000000000000000000000000000000000000000000000000000000000",
        59 : "0000000000000000000000000000000000000000000000000000000000000000",
        60 : "0000000000000000000000000000000000000000000000000000000000000000",
        61 : "010510020c0a0c031c0e1a1e0a0e150d09161b1c130b1e1302021701000c100d",
        62 : "7f7f7fa87f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f",
        63 : "0612060a1d1b19010407181a12161902021a010601001a0a0404141e0f1b0f11",
        64 : "18121708080d1e161d10111e0518181a1704141c110b1d110c13180700101d15",
        65 : "7f7f7f7f7f7f7f7f7f7f997f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f",
        66 : "7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7fd77f7f",
        67 : "0000000000000000000000000000000000000000000000000000000000000000",
        68 : "121216020f060c0f0a0c16011d120511020f150d09141c1b0b1a03011e171311",
        69 : "190a19020d0a0d190f1e1a03090016001b050c01090c0117160b1902010b1b17",
        70 : "0000000000000000000000000000000000000000000000000000000000000000",
        71 : "7f7f7f7f7f7f7f7f7f7f7f857f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f",
        72 : "180c0018050c0b030a051314000e111b0f02011a181a081402190a1d0e011c13",
        73 : "0000000000000000000000000000000000000000000000000000000000000000",
        74 : "0d0b1e08180d0b011a151b0d14030c06011d0604060b10041e1e040c151b0f1c",
        75 : "1a1c011b00141c0f0c0a1c1c13160a041e14081e120a1b021804030816120d04",
        76 : "0c11150c1b1d1e01191b041d03061d191108070c0013011702000817190f1d03",
        77 : "1c061606001b1a0205071c0b190d0b171308121519141312021d16081513140b",
        78 : "0e02171b1c1a1b1c100c1508191a1b121d110d141e1c1802120f131a07160306",
        79 : "1e1b1516071708030e0a050d1b0d0d1510041c0d180c190c06061d12010c0702",
        80 : "1b081d1c020d170d0f19151d051c1c131d071b171202000007170b18130c1b01",
        81 : "7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7fe27f7f7f7f7f7f7f7f7f7f7f7f7ffa",
        82 : "0000000000000000000000000000000000000000000000000000000000000000",
        83 : "0000000000000000000000000000000000000000000000000000000000000000",
        84 : "7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f947f7f7f7f7fce",
        85 : "7f7f7f7f7f7f7f7f9a7fbf7f7f7f7f7f7f7f7f7faf7f7f7f7f7f7f7f7f7f7f7f",
        86 : "7f7f7f7f7f7f7fc57f7f7f7f7f7f7f7f7f7f7f7fca7f7fee7f7f7f7f7f7f7f7f",
        87 : "1805180d170e1802011c0f1b1d14110602191b18150d09030d111c1d0c031716",
        88 : "7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7fc47f7f7f7f7f7f7f7f7f7f7f7f",
        89 : "0000000000000000000000000000000000000000000000000000000000000000",
        90 : "7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7fc07f7f7f7f7f7f7f7fde7f7f7f7f7f7f",
        91 : "7f7f7f7f7f7f7f7f7f7f7f7fa57f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f",
        92 : "0000000000000000000000000000000000000000000000000000000000000000",
        93 : "0a1a1907001905181505021c12130e0412071816001c01020904070b160c080f",
        94 : "1406190710140713080519110a1200040c1e0f021718181115061619170a1213",
        95 : "0a1d0f1d1e1915040012151d10151406131e0315130b18001b190e030e12070f",
        96 : "7f7f7f7f7f7f7f7f7f7f7f7f7f7fb67f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f",
        97 : "7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7fc87f7f7f7f7fe77f7f7f7f7f7f7f7f7f",
        98 : "15191803171a170e1503170818130f100201001804030b1e1b0919020c111e01",
        99 : "090b1304150b1204140a0e0c0e1509140109170113000e1b0010021a15171400",
        100 : "7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7fa77f7f7f7f7f7f7f7f7f7fe37f7f",
        101 : "0e0a00010b061005061416091a070a16011c020e1601191e0e030203170c1c0d",
        102 : "1d031b0116000d1a0c1c1612050a0c121e080f1c0a13171317061d0512091309",
        103 : "1e171c061012190e180c121a181400050f07021a1d090c19011303081901010c",
        104 : "7f7f7f7f7f7f7f7f7f7f7f7f80aa7f7f7f7f7f7f7f7f7f7f7f7f7f7ff07f7f7f",
        105 : "b37f7f7f7f7f7f7f7f7f7f7f7f937f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f",
        106 : "160a000e1001110a00050310011c1a1d091c1e170814120c090103040e131701",
        107 : "7f7f7f7f7f7f7f7f7f7f7f7f7f7ff17f7f7f7f7f7f7f7f7ff37f7f7f7f7f7f7f",
        108 : "83fee0da7fd47febbe9ed5ade4ac90d692d8c1f89fe1ede9a1e8c7c2a9d1dbff",
        109 : "7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f827f7f7f7f7f7f7f7f7f7f7f7f7f7f",
        110 : "1614041e0c120b010e0401131303110a0b180f1b120e130a03151318031c181c",
        111 : "08000115111d1d1c01171514161b130b10061200040a18160a1301051e080c11",
        112 : "19051e1302161e0c150906160019100303141b081e031a0c02080e181a041014",
        113 : "1d07111b1205071e091a181716181a01050f06100f03020019021d1e170d080c",
        114 : "0000000000000000000000000000000000000000000000000000000000000000",
        115 : "110601040d1406151a170d141e1b0a1505110b0d0d141a0e0417171d0c0e101b",
        116 : "0a130b11150f14171a05060f0f19101b180f190e0a0d0e1401161e0e02060307",
        117 : "1b0a170019111d0b130a18121e000401031c1d0e1d19181705110d1d05051404",
        118 : "1119021a1c05191a1b101206150c00040c1b111c1c02120a0f0e0e03190f130e",
        119 : "0000000000000000000000000000000000000000000000000000000000000000",
        120 : "7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7fcb7f7f7f7f7f7f7f7f7f7f7f7f7f",
        121 : "0000000000000000000000000000000000000000000000000000000000000000",
        122 : "051e0312041b1d18090717090d01040002020d1116040d13020d0b1d010c0c16",
        123 : "0000000000000000000000000000000000000000000000000000000000000000",
        124 : "0000000000000000000000000000000000000000000000000000000000000000",
        125 : "0000000000000000000000000000000000000000000000000000000000000000",
        126 : "7f7f7f7f7f7f7f7f8ce6cf7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f967f7f7f7f7f",
        127 : "7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7fdf7f7f7f7f7f7f7f7f7f7f7f7f957f7f"
    }

vaddr1 = 0x611c
VPN1 = (vaddr1 & VPN_MASK) >> VPN_SHIFT
PDIndex1 = PDE_MASK & VPN1 >> PTE_SHIFT # 31774 & 776 >> 5    # 24
PDAddr1 = PDBR + (PDIndex1 * PDE_SIZE)

# Retrieve the page directory entry PDE
PTIndex = pagedir[PDBR]
page1 = int("0x" + PTIndex[PDIndex1], 16)
PDE1 = pagedir[page1]

# Aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaargh
# This entry is broken up into two two byte values 
PDE1array = [PDE1[i:i+2] for i in range(0, len(PDE1), 2)]
PDEcontents1 = PDE1array[PDIndex1]


pteBits = (vaddr1 & PTE_MASK) >> PTE_SHIFT
pteAddr = (
PTIndex1 = PTE_MASK & VPN1 >> PTE_SHIFT
"""
we would then use an access memory routine to get the PDEAddr
PDE = AccessMemory(PDEAddr1)
then check validity
if valid, begin to access the Page Table Entry
"""

vaddr2 = 0x3da8
VPN2 = (vaddr2 & VPN_MASK) >> VPN_SHIFT
PDIndex2 = (VPN2 & PDE_MASK) >> PDE_SHIFT
PDEAddr2 = PDBR + (PDIndex2 * PDE_SIZE) # decimal 108 in this example 

vaddr3 = 0x17f5
VPN3 = (vaddr3 & VPN_MASK) >> VPN_SHIFT
PDIndex3 = (VPN2 & PDE_MASK) >> PDE_SHIFT
PDEAddr3 = PDBR + (PDIndex3 * PDE_SIZE) # decimal 108 in this example 

vaddr4 = 0x7f6c
VPN4 = (vaddr4 & VPN_MASK) >> VPN_SHIFT
PDIndex4 = (VPN2 & PDE_MASK) >> PDE_SHIFT
PDEAddr4 = PDBR + (PDIndex4 * PDE_SIZE) # decimal 108 in this example 

vaddr5 = 0x0bad
VPN5 = (vaddr5 & VPN_MASK) >> VPN_SHIFT
PDIndex5 = (VPN2 & PDE_MASK) >> PDE_SHIFT
PDEAddr5 = PDBR + (PDIndex5 * PDE_SIZE) # decimal 108 in this example 
