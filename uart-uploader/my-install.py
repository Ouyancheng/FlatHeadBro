#!/usr/bin/env python3
from binascii import crc32
import enum
from select import select
import termios
import tty
import serial
import os 
import sys 
import collections
import typing as T 
CandidateTTYUSB = collections.namedtuple(
    "CandidateTTYUSB", 
    ["name", "last_access_time", "last_modify_time", "last_change_time", "birth_time"]
)
def find_ttyusb() -> T.List[CandidateTTYUSB]: 
    """
    Finds the lastly used ttyusb 
    """
    ttyusb_candidate_prefix = [
        "ttyUSB",
        "cu.SLAB_USB"
    ]
    candidate_list = []
    diriter = os.scandir("/dev/")
    for direntry in diriter: 
        devname = direntry.name
        iscandidate = False
        for prefix in ttyusb_candidate_prefix:
            if devname.startswith(prefix):
                iscandidate = True
                break
        if not iscandidate:
            continue
        
        dirstat = direntry.stat()
        candidate_list.append(
            CandidateTTYUSB(devname, 
                dirstat.st_atime, 
                dirstat.st_mtime, 
                dirstat.st_ctime, 
                dirstat.st_birthtime
            )
        )
    return candidate_list

def find_ttyusb_last() -> T.Union[CandidateTTYUSB, None]: 
    candidate_list = find_ttyusb() 
    if len(candidate_list) == 0:
        return None
    selected_candidate = candidate_list[0]
    for candidate in candidate_list: 
        if candidate.last_change_time > selected_candidate.last_change_time: 
            selected_candidate = candidate
    return selected_candidate


"""
enum uart_boot_msgs {
    RISCV64BASE     = 0x40000000,       // where program gets linked.  we could send this.
    BOOT_START      = 0xFFFF0000,
    GET_PROG_INFO   = 0x11112222,       // board sends
    PUT_PROG_INFO   = 0x33334444,       // unix sends
    GET_CODE        = 0x55556666,       // board sends
    PUT_CODE        = 0x77778888,       // unix sends
    BOOT_SUCCESS    = 0x9999AAAA,       // board sends on success
    BOOT_ERROR      = 0xBBBBCCCC,       // board sends on failure.
    PRINT_STRING    = 0xDDDDEEEE,       // board sends to print a string.
    // error codes from the board to unix
    BAD_CODE_ADDR   = 0xdeadbeef,
    BAD_CODE_CKSUM  = 0xfeedface,
};
"""
class uart_boot_msgs(enum.Enum): 
    ARMBASE         = int.to_bytes(0x8000, 4, 'little')
    RISCV64BASE     = int.to_bytes(0x40000000, 4, 'little')
    BOOT_START      = int.to_bytes(0xFFFF0000, 4, 'little')
    GET_PROG_INFO   = int.to_bytes(0x11112222, 4, 'little')
    PUT_PROG_INFO   = int.to_bytes(0x33334444, 4, 'little')
    GET_CODE        = int.to_bytes(0x55556666, 4, 'little')
    PUT_CODE        = int.to_bytes(0x77778888, 4, 'little')
    BOOT_SUCCESS    = int.to_bytes(0x9999AAAA, 4, 'little')
    BOOT_ERROR      = int.to_bytes(0xBBBBCCCC, 4, 'little')
    PRINT_STRING    = int.to_bytes(0xDDDDEEEE, 4, 'little')
    BAD_CODE_ADDR   = int.to_bytes(0xdeadbeef, 4, 'little')
    BAD_CODE_CKSUM  = int.to_bytes(0xfeedface, 4, 'little')



def simple_boot(serial_handler: serial.Serial, raw_binary: bytes):
    while 1: 
        op = serial_handler.read(4)
        # print(uart_boot_msgs.GET_PROG_INFO.value)
        if op != uart_boot_msgs.GET_PROG_INFO.value: 
            print("expected initial GET_PROG_INFO, got <{}>: discarding.".format(op))
            serial_handler.read(1) 
        else:
            break
    sent_crc = int.to_bytes(crc32(raw_binary, 0), 4, 'little') 
    serial_handler.write(uart_boot_msgs.PUT_PROG_INFO.value)
    serial_handler.write(uart_boot_msgs.ARMBASE.value)
    serial_handler.write(int.to_bytes(len(raw_binary), 4, 'little'))
    serial_handler.write(sent_crc)

    while 1: 
        op = serial_handler.read(4)
        if op == uart_boot_msgs.GET_PROG_INFO.value:
            print("discarding GET_PROG_INFO.")
        elif op != uart_boot_msgs.GET_CODE.value:
            print("expected GET_CODE, got <{}>.".format(op))
            exit(1)
        else: 
            break
    
    recv_crc = serial_handler.read(4) 
    if recv_crc != sent_crc:
        print("crc error: recv_crc={}, sent_crc={}".format(recv_crc, sent_crc))
        exit(2)

    serial_handler.write(uart_boot_msgs.PUT_CODE.value)
    serial_handler.write(raw_binary)

    msg = serial_handler.read(4)
    if msg != uart_boot_msgs.BOOT_SUCCESS.value:
        print("expected BOOT_SUCCESS, got <{}>.".format(msg))
        exit(3) 

    print("Done!!!")
    return

def main(argv: T.List[str]) -> int:
    ttyusb = find_ttyusb_last()
    if ttyusb is None: 
        print("No candidate ttyusb is found! Quitting...", file=sys.stderr)
        return -1 
    # print(ttyusb)
    serial_handler = serial.Serial(
        port="/dev/"+ttyusb.name, 
        baudrate=115200, 
        bytesize=serial.EIGHTBITS, 
        parity=serial.PARITY_NONE, 
        stopbits=serial.STOPBITS_ONE
    )
    bin_file = open(argv[1], "rb")
    raw_binary = bin_file.read()
    bin_file.close()
    simple_boot(serial_handler, raw_binary)

    try: 
        tattr = termios.tcgetattr(sys.stdin.fileno())
        tty.setcbreak(sys.stdin.fileno(), termios.TCSANOW)
        serial_fd = serial_handler.fileno()
        stdin_fd = sys.stdin.fileno()
        while 1:
            # if serial_handler.in_waiting > 0: 
            r, _, _ = select([stdin_fd, serial_fd], [], [], 0.01)
            if serial_fd in r:
                b = serial_handler.read_all()
                print(bytes.decode(b, encoding="ascii", errors="backslashreplace"), flush=True, end='') 
            if stdin_fd in r: 
                s = sys.stdin.read(1)
                serial_handler.write(s.encode("ascii"))
    except serial.SerialException as serial_exception:
        print("Serial exception:", serial_exception)
    except Exception as exception: 
        print(exception)
    finally:
        termios.tcsetattr(sys.stdin.fileno(), termios.TCSANOW, tattr)
        exit(0)
    

    

if __name__ == '__main__': 
    main(sys.argv)
