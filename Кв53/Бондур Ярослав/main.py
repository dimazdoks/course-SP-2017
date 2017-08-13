
from firstpass import *
asm = FirstPass()
print("Course Work by Yaroslav Bondur\n\n")
print("Sourse file name [.ASM]: ")
filename = input()
if (filename[-4]!='.'):
    filename+=".asm"
print("Listing file name [.LST]: ")
lstname = input()
if (lstname[-4]!='.'):
    lstname+=".lst"
asm.program(filename,lstname)

