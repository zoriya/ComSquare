
# python script to convert tile indexes to correct values in vram depending on the bpp

def getVRAMValue2bpp(nums):
    vramValues = []
    
    
    valHex1 = 0
    valHex2 = 0
    for i in nums:
        valHex1 = 0
        valHex2 = 0
        shift = 7
        for j in i:
            valHex1 += (j & 1) << shift
            valHex2 += ((j & 2) >> 1) << shift
            shift -= 1
        
        vramValues.append((valHex1 << 8) + valHex2)
    
    padding = 4
    for i in vramValues:
        print('{0:0{1}X}'.format(i, 4), end=' ')
    print("")

def getVRAMValue4bpp(nums):
    getVRAMValue2bpp(extract2BppBitPlans(nums, 0))
    getVRAMValue2bpp(extract2BppBitPlans(nums, 1))

def getVRAMValue8bpp(nums):
    getVRAMValue2bpp(extract2BppBitPlans(nums, 0))
    getVRAMValue2bpp(extract2BppBitPlans(nums, 1))
    getVRAMValue2bpp(extract2BppBitPlans(nums, 2))
    getVRAMValue2bpp(extract2BppBitPlans(nums, 3))

def extract2BppBitPlans(nums, index):
    high = []
    for i in nums:
        high.append([(j & (0x3 << (2 * index))) >> (index * 2) for j in i])
    return high
    
result2 = [
        [0, 1, 1, 1, 1, 1, 0, 0],
        [1, 2, 3, 3, 3, 2, 1, 0],
        [1, 2, 3, 3, 3, 2, 1, 0],
        [0, 1, 1, 1, 1, 1, 0, 0],
        [0, 0, 0, 1, 0, 0, 0, 0],
        [1, 1, 0, 1, 0, 1, 1, 0],
        [0, 1, 1, 1, 1, 1, 0, 0],
        [0, 0, 1, 1, 1, 0, 0, 0]
    ]
    
result2 = [
        [0, 7, 7, 7, 7, 7, 0, 0],
        [7, 2, 6, 8, 6, 2, 7, 0],
        [7, 2, 2, 2, 2, 2, 7, 0],
        [0, 7, 7, 7, 7, 7, 0, 0],
        [0, 0, 0, 4, 0, 0, 0, 0],
        [3, 5, 0, 5, 0, 5, 3, 0],
        [0, 3, 4, 5, 4, 3, 0, 0],
        [0, 0, 3, 3, 3, 0, 0, 0],
    ]

result2 = [
		[0x00, 0x22, 0x22, 0x22, 0x23, 0x23, 0x00, 0x00],
		[0x22, 0x11, 0x42, 0x21, 0x41, 0x11, 0x24, 0x00],
		[0x23, 0x11, 0x12, 0x12, 0x12, 0x12, 0x24, 0x00],
		[0x00, 0x24, 0x25, 0x25, 0x25, 0x25, 0x00, 0x00],
		[0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00, 0x00],
		[0x35, 0x32, 0x00, 0x31, 0x00, 0x31, 0x35, 0x00],
		[0x00, 0x34, 0x33, 0x31, 0x33, 0x34, 0x00, 0x00],
		[0x00, 0x00, 0x35, 0x36, 0x36, 0x00, 0x00, 0x00]
    ]

getVRAMValue8bpp(result2)

