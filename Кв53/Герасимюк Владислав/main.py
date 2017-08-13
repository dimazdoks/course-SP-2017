#! /usr/bin/env python
# -*- coding: utf-8 -*-
import re
from enum import Enum, auto
import sys
import re

def read_file():
    f = open('test_asm.asm')
    while True:
        line = f.readline()
        if not line:
            break
        STRING_PROCESS_LEX(line.upper())
        LIST_STRING_SYNX(line.upper())
    f.close()

class TOKEN_TYPE(Enum):
    UNDEFINED = auto()
    USER_ID = auto()
    DIRECTIVE = auto()
    ID_DD_1 = auto()
    ID_DD_2 = auto()
    ID_DD_4 = auto()
    HEX_CONST = auto()
    BIN_CONST = auto()
    DEC_CONST = auto()
    ONE_CHAR = auto()
    MACH_INST = auto()
    ID_16_REG = auto()
    ID_32_REG = auto()
    ID_SEGMENT_REG = auto()
    ID_TYPE_1 = auto()
    ID_TYPE_4 = auto()
    ID_TYPE_DEF = auto()
    ID_8_REG = auto()

class LEXEMA(object):
    def __init__(self, id=None, name=None, len=None, token_type=None):
        self.id = id
        self.name = name
        self.len = len
        self.token_type= token_type

TOKEN__LIST = []

RESERVED_LEXEMS = {
    "SEGMENT": TOKEN_TYPE.DIRECTIVE,
    "END": TOKEN_TYPE.DIRECTIVE,
    "ENDS": TOKEN_TYPE.DIRECTIVE,

    "DB": TOKEN_TYPE.ID_DD_1,
    "DW": TOKEN_TYPE.ID_DD_2,
    "DD": TOKEN_TYPE.ID_DD_4,

    "BYTE": TOKEN_TYPE.ID_TYPE_1,
    "DWORD": TOKEN_TYPE.ID_TYPE_4,

    "PTR": TOKEN_TYPE.ID_TYPE_DEF,

    "CS": TOKEN_TYPE.ID_SEGMENT_REG,
    "DS": TOKEN_TYPE.ID_SEGMENT_REG,
    "SS": TOKEN_TYPE.ID_SEGMENT_REG,
    "ES": TOKEN_TYPE.ID_SEGMENT_REG,
    "FS": TOKEN_TYPE.ID_SEGMENT_REG,
    "GS": TOKEN_TYPE.ID_SEGMENT_REG,

    "EAX": TOKEN_TYPE.ID_32_REG,
    "EBX": TOKEN_TYPE.ID_32_REG,
    "ECX": TOKEN_TYPE.ID_32_REG,
    "EDX": TOKEN_TYPE.ID_32_REG,
    "EBP": TOKEN_TYPE.ID_32_REG,
    "ESI": TOKEN_TYPE.ID_32_REG,
    "EDI": TOKEN_TYPE.ID_32_REG,
    "ESP": TOKEN_TYPE.ID_32_REG,


    "AX": TOKEN_TYPE.ID_16_REG,
    "BX": TOKEN_TYPE.ID_16_REG,
    "CX": TOKEN_TYPE.ID_16_REG,
    "DX": TOKEN_TYPE.ID_16_REG,
    "BP": TOKEN_TYPE.ID_16_REG,
    "SI": TOKEN_TYPE.ID_16_REG,
    "DI": TOKEN_TYPE.ID_16_REG,
    "SP": TOKEN_TYPE.ID_16_REG,

    "AH": TOKEN_TYPE.ID_8_REG,
    "AL": TOKEN_TYPE.ID_8_REG,
    "BH": TOKEN_TYPE.ID_8_REG,
    "BL": TOKEN_TYPE.ID_8_REG,
    "CH": TOKEN_TYPE.ID_8_REG,
    "CL": TOKEN_TYPE.ID_8_REG,
    "DH": TOKEN_TYPE.ID_8_REG,
    "DL": TOKEN_TYPE.ID_8_REG,

    "SCASB": TOKEN_TYPE.MACH_INST,
    "XOR": TOKEN_TYPE.MACH_INST,
    "MOV": TOKEN_TYPE.MACH_INST,
    "TEST": TOKEN_TYPE.MACH_INST,
    "XADD": TOKEN_TYPE.MACH_INST,
    "SBB": TOKEN_TYPE.MACH_INST,
    "NEG": TOKEN_TYPE.MACH_INST,
    "JA": TOKEN_TYPE.MACH_INST
}

def STRING_PROCESS_LEX(line):
    if line != '\n':
         print("{0:^4}".format(line.lstrip()), end='')
         print('{0:=<70s}'.format(""))
         print('%+3s' % "№", '%+21s' % "TOKEN", '%+12s' % "LEN", '%+28s' % "TYPE", '', sep='|')
         print('{0:=<70s}'.format(""))
    lex = ''
    num_len = 0
    for i in line:
        if i == ';':
            i = '\n'
        if i not in [':',',','.','[',']','(',')','<','>','{', '}', '+','-',  '/','*','%','!','?','\\', '=', chr(35), ' ', '\n']:    #['(', '+', ')', ':', ',', '[', ']', '*', '-', ' ', '\n']:
            if i == '\t':
                continue
            else:
                lex += i
        else:
            if i == '\n':
              if lex != '':
                num_len += 1
                LEX_PROCESS(lex, num_len)
              lex = ''
            elif i == ' ':
                if lex != '':
                    num_len += 1
                    LEX_PROCESS(lex, num_len)
                lex = ''
            else:
                if lex != '':
                     num_len += 1
                     LEX_PROCESS(lex, num_len)
                lex = ''
                lex += i
                if lex != '':
                     num_len += 1
                     LEX_PROCESS(lex, num_len)
                lex = ''

def LEX_PROCESS(lex, num_len):
  #IF RESERVED_LEXEM
  for key,val in RESERVED_LEXEMS.items():
      if lex == key:
         # print(num_len, lex, len(lex), val.name)
          TOKEN__LIST.append(LEXEMA(num_len, lex, len(lex), val.name))
          PRINT_TOKEN__LIST()
          return
  #IF IDENTEFICATOR and number
  for i in lex:
    if i in [':',',','.','[',']','(',')','<','>','{', '}', '+','-', '/','*','%','!', ", ",'?','\\', '=', chr(35)]:
        TOKEN__LIST.append(LEXEMA(num_len, lex, len(lex), TOKEN_TYPE.ONE_CHAR.name))
        PRINT_TOKEN__LIST()
        return
    if lex[0].isdigit():
        if i in ['0', '1'] and lex[len(lex) - 1] == 'B':
            TOKEN__LIST.append(LEXEMA(num_len, lex, len(lex), TOKEN_TYPE.BIN_CONST.name))
            PRINT_TOKEN__LIST()
            return
        if '0' <= i <= '9' or lex[len(lex) - 1] == 'D':
            TOKEN__LIST.append(LEXEMA(num_len, lex, len(lex), TOKEN_TYPE.DEC_CONST.name))
            PRINT_TOKEN__LIST()
            return
    if ('0' <= lex[0] <= '9' or lex[0] in ['A','B','C','D','E','F']) and lex[len(lex) - 1] == 'H':
        if 'A' <= i <= 'F' or '0' <= i <= '9':
            lex = '0' + lex
            TOKEN__LIST.append(LEXEMA(num_len, lex, len(lex), TOKEN_TYPE.HEX_CONST.name))
            PRINT_TOKEN__LIST()
            return
    if not i.isalpha() and not i.isdigit() and i not in ["_", "@", "?", "$"]:
        print("ERROR!")
        return
    else:
        continue
  TOKEN__LIST.append(LEXEMA(num_len, lex, len(lex), TOKEN_TYPE.USER_ID.name))
  PRINT_TOKEN__LIST()

def PRINT_TOKEN__LIST():
    print('%+3s' % TOKEN__LIST[len(TOKEN__LIST) - 1].id , '%+21s' % TOKEN__LIST[len(TOKEN__LIST) - 1].name, '%+12s' % TOKEN__LIST[len(TOKEN__LIST) - 1].len, '%+28s' % TOKEN__LIST[len(TOKEN__LIST) - 1].token_type,'',sep ='|')

#SYNTAX ANALIZATOR
def LIST_STRING_SYNX(line):
        if line != '\n':
            print('{0:-<80s}'.format(""))
            print('{0:^16}|{1:^20}|{2:^20}|{3:^20}|'.format("label", "Mnemocode", "operand1", "operand2"))
            print('{0:^16}|{1:^8}|{2:^8}|{3:^8}|{4:^8}|{5:^8}|{6:^8}|'.format("leksema", "leksema_1", "count leks","leksema_1", "count leks", "leksema_1","count leks"))
            print('{0:-<80s}'.format(""))
            lineN = line
            line = line.replace(',', '').split()
            for i in range(len(line)):
                 line[i] = line[i].strip(':')
            STRING_PROCESS(line)
            OFFSET(line, lineN)

def STRING_PROCESS(line):
     for LEXEMA in TOKEN__LIST:
        if line[0] == LEXEMA.name and LEXEMA.token_type == "DIRECTIVE":
             print('{0:^16}|{1:^9}|{2:^10}|{3:^9}|{4:^10}|{5:^9}|{6:^10}|'.format('-1', '1', '1', '-1', '0', '-1', '0'))
             return
        if line[0] == LEXEMA.name and LEXEMA.token_type == "USER_ID":
            if len(line) == 1:
                print('{0:^16}|{1:^9}|{2:^10}|{3:^9}|{4:^10}|{5:^9}|{6:^10}|'.format('1', '-1', '0', '-1', '0', '-1','0'))
                return
            for LEXEMA in TOKEN__LIST:
                 if (line[1] == LEXEMA.name) and (LEXEMA.token_type in ["DIRECTIVE","ID_DD_1","ID_DD_2","ID_DD_4"]):
                    if len(line) == 2:
                        print('{0:^16}|{1:^9}|{2:^10}|{3:^9}|{4:^10}|{5:^9}|{6:^10}|'.format('1', '2', '1', '-1', '0', '-1','0'))
                        return
                    else:
                        count = 0
                        for i in line[2]:
                            if i in ['+','(',')','-']:
                                count += 1
                        count1 = re.split(r'[()+-]+', line[2])
                        for i in count1:
                            if i == '':
                                count1.remove(i)
                        print('{0:^16}|{1:^9}|{2:^10}|{3:^9}|{4:^10}|{5:^9}|{6:^10}|'.format('1', '2', '1', '3', len(count1) + count,'-1', '0'))
                        return
        if line[0] == LEXEMA.name and LEXEMA.token_type == "MACH_INST":
            if len(line) == 1:
                print('{0:^16}|{1:^9}|{2:^10}|{3:^9}|{4:^10}|{5:^9}|{6:^10}|'.format('-1', '1', '1', '-1', '0', '-1','0'))
                return
            elif len(line) == 2:
                print('{0:^16}|{1:^9}|{2:^10}|{3:^9}|{4:^10}|{5:^9}|{6:^10}|'.format('-1', '1', '1', '2', '1', '-1','0'))
                return
            for LEXEMA in TOKEN__LIST:


                if (line[1] == LEXEMA.name) and (LEXEMA.token_type in ["ID_32_REG", "ID_16_REG"]):

                    if len(line) == 3:
                        count = 0
                        for i in line[2]:
                            if i in ['+', '(', ')', '-','*','[',']']:
                                count += 1
                        count1 = re.split(r'[(*)+[\]-]+', line[2])
                        for i in count1:
                            if i == '':
                                count1.remove(i)
                        print('{0:^16}|{1:^9}|{2:^10}|{3:^9}|{4:^10}|{5:^9}|{6:^10}|'.format('-1', '1', '1', '2',"1", '4',len(count1) + count))
                        return
                    else:
                        count = 0
                        for i in line[2]:
                            if i in [":",'+', '(', ')', '-','*','[',']']:
                                count += 1
                        count1 = re.split(r'[(*):+[\]-]+', line[2])

                        for i in count1:
                            if i == '':
                                count1.remove(i)

                        count2 = 0
                        for i in line[3]:
                            if i in ['+', '(', ')', '-','*','[',']']:
                                count2 += 1
                        count3 = re.split(r'[(*)+[\]-]+', line[3])

                        for i in count3:
                            if i == '':
                                count3.remove(i)
                        print('{0:^16}|{1:^9}|{2:^10}|{3:^9}|{4:^10}|{5:^9}|{6:^10}|'.format('-1', '1', '1', '2','1', '4',len(count3) + count2 + len(count1) + count + 1))
                        return
                if (line[1] == LEXEMA.name) and (LEXEMA.token_type in ["ID_TYPE_1","ID_TYPE_4"]):
                    count = 0
                    count_L = 0
                    for i in range(1,4):
                        for j in line[i]:
                            if j in [":", '+', '(', ')', '-', '*', '[', ']']:
                                count += 1
                        count1 = re.split(r'[(*):+[\]-]+', line[i])
                        for k in count1:
                            if k == '':
                                count1.remove(k)
                        count_L = count_L + len(count1)
                    print('{0:^16}|{1:^9}|{2:^10}|{3:^9}|{4:^10}|{5:^9}|{6:^10}|'.format('-1', '1', '1', '2',count_L + count,count_L + count + len(line) - 2, '1'))
                    return

offset_val = 0
flag1 = False
flag2 = False

def OFFSET(line, lineN):
    f = open('offset.txt', 'a')
    global  offset_val
    global flag1
    global flag2
    for LEXEMA in TOKEN__LIST:
        if line[0] == LEXEMA.name and LEXEMA.token_type == "DIRECTIVE":
            f.write('{0:<0}'.format(lineN))
            return

        if line[0] == LEXEMA.name and LEXEMA.token_type == "USER_ID":
            if len(line) == 1:
                if line[0] == 'LABEL1':
                    flag1 = True
                if line[0] == 'LABEL2':
                    flag2 = True
                f.write('{0:^0}{1:^9}'.format(lineN,hex(offset_val)))
                return
            for LEXEMA in TOKEN__LIST:
                if (line[1] == LEXEMA.name) and (LEXEMA.token_type in ["DIRECTIVE", "ID_DD_1", "ID_DD_2", "ID_DD_4"]):
                    if len(line) == 2:
                        offset_val = 0
                        if line[0] == "DATA1" and line[1] == "SEGMENT":
                            f.write('{0:>23}{1:^9}'.format(lineN, hex(offset_val)))
                        else:
                            f.write('{0:<0}{1:^9}'.format(lineN, hex(offset_val)))
                            return
                    elif line[1] == 'DD':
                        offset_val = offset_val + 4
                        f.write('{0:<0}{1:^9}'.format(lineN,hex(offset_val)))
                        return
                    elif line[1] == 'DW':
                        offset_val = offset_val + 2
                        f.write('{0:<0}{1:^9}'.format(lineN,hex(offset_val)))
                        return
                    elif line[1] == 'DB':
                        offset_val = offset_val + 1
                        f.write('{0:<0}{1:^9}'.format(lineN,hex(offset_val)))
                        return
        if line[0] == LEXEMA.name and LEXEMA.token_type == "MACH_INST":
            if len(line) == 1:
                offset_val = offset_val + 1
                f.write('{0:<0}{1:^9}'.format(lineN,hex(offset_val)))
                return
            elif len(line) == 2:
                if line[0] == "JA" and flag2 != True:
                    offset_val = offset_val + 6
                    f.write('{0:<0}{1:^9}'.format(lineN,hex(offset_val)))
                    return
                elif line[0] == "JA" and flag1 == True:
                    offset_val = offset_val + 2
                    f.write('{0:<0}{1:^9}'.format(lineN,hex(offset_val)))
                    return
            for LEXEMA in TOKEN__LIST:

                if (line[0] == 'NEG') and (LEXEMA.token_type in ["ID_16_REG"]):
                    offset_val = offset_val + 3
                    f.write('{0:<0}{1:^9}'.format(lineN,hex(offset_val)))
                    return
                elif (line[0] == 'NEG') and (LEXEMA.token_type in ["ID_32_REG"]):
                    offset_val = offset_val + 2
                    f.write('{0:<0}{1:^9}'.format(lineN,hex(offset_val)))
                    return
                if (line[0] == 'SBB'):
                    for LEXEMA in TOKEN__LIST:
                        if (line[1] == LEXEMA.name) and LEXEMA.token_type in ["ID_16_REG"]:
                             offset_val = offset_val + 3
                             f.write('{0:<0}{1:^9}'.format(lineN,hex(offset_val)))
                             return
                        if (line[2] == LEXEMA.name) and LEXEMA.token_type in ["ID_TYPE_DEF"]:
                                f.write('{0:<0}{1:^9}{2:^9}'.format("ERROR!:",lineN,hex(offset_val)))
                                return

                if (line[0] == 'XADD') and (LEXEMA.token_type in ['ID_TYPE_1']):
                    for LEXEMA in TOKEN__LIST:
                        if LEXEMA.token_type in ['HEX_CONST','BIN_CONST','DEC_CONST']:
                             offset_val = offset_val + 4
                             f.write('{0:<0}{1:^9}'.format(lineN,hex(offset_val)))
                             return
                if (line[0] == 'TEST') and (LEXEMA.token_type in ['ID_16_REG']):
                    for LEXEMA in TOKEN__LIST:
                        if LEXEMA.token_type in ['USER_ID','HEX_CONST','BIN_CONST','DEC_CONST']:
                            offset_val = offset_val + 7
                            f.write('{0:<0}{1:^9}'.format(lineN,hex(offset_val)))
                            return
                if line[0] == 'MOV':
                     if (line[1] == LEXEMA.name and LEXEMA.token_type == "ID_16_REG") or (line[2] == LEXEMA.name and LEXEMA.token_type == "ID_SEGMENT_REG"):
                            offset_val = offset_val + 4
                            f.write('{0:<0}{1:^9}'.format(lineN,hex(offset_val)))
                            return
                     if (line[2] != LEXEMA.name and LEXEMA.token_type == "ID_SEGMENT_REG"):
                         offset_val = offset_val + 3
                         f.write('{0:<0}{1:^9}'.format(lineN,hex(offset_val)))
                         return
                if (line[0] == 'XOR') and (LEXEMA.token_type in ['ID_TYPE_4']):
                    for LEXEMA in TOKEN__LIST:
                        if LEXEMA.token_type in ['HEX_CONST','BIN_CONST','DEC_CONST']:
                             offset_val = offset_val + 4
                             f.write('{0:<0}{1:^9}'.format(lineN,hex(offset_val)))
                             return
    f.close()





sys.stdout = open('OUT.txt', 'w')

def main():
    read_file()
    sys.stdout.close()


if __name__ == "__main__":
    main()













