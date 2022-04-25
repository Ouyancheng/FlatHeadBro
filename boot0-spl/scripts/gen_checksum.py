import struct
import os 

blocksize       = 0x4000
stamp           = 0x5f0a6C39
checksum_offset = 0x0c
length_offset   = 0x10
def pad_to_roundup(data: bytearray, boundary):
    excess = len(data) % boundary
    if excess:
        data += b'\0' * (boundary - excess)

def main():
    # little endian + unsigned int 
    uint32iter = struct.Struct('<I')
    input_img = open(os.sys.argv[1], 'rb')
    rawbytes = bytearray(input_img.read())
    pad_to_roundup(rawbytes, blocksize)
    uint32iter.pack_into(rawbytes, checksum_offset, stamp)
    uint32iter.pack_into(rawbytes, length_offset, len(rawbytes))
    checksum = 0
    for uint32 in uint32iter.iter_unpack(rawbytes):
        checksum += uint32[0]
    uint32iter.pack_into(rawbytes, checksum_offset, checksum % (2**32))
    output_img = open(os.sys.argv[2], 'wb')
    output_img.write(rawbytes)
    output_img.close()
    input_img.close()

if __name__ == '__main__':
    main()
