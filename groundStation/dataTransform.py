def str2bytes(s):
    b = bytes(s,encoding='utf-8')
    return b

def bool2charStar(b):
    if(b):
        c = "True"
    else:
        c = "False"
    return c

def int2bytes(i):
    return i.to_bytes(1,byteorder='little', signed=False)

def bytes2int(b):
    return int.from_bytes(b,byteorder='little',signed=True)