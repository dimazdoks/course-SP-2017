from Lexical import *
class vallues():
    def __init__(self):
        self.name = ""
        self.adr = ""
        self.type = ""
        self.type_in_table = ""
        self.label =""
        self.segm =""
    def clear(self):
        self.name=""
        self.adr = ""
        self.type = ""
        self.type_in_table = ""
        self.label = ""
        self.segm = ""
class FirstPass():
    def __init__(self):
        self.regists = ['EAX', 'EBX', 'ECX', 'EDX', 'ESI', 'EDI', 'ESP', 'EBP', 'AX', 'BX', 'CS', 'DX', 'SI', 'DI', 'SP',
                   'BP', 'AH', 'AL', 'BH', 'BL', 'CH', 'CL', 'DH', 'DL']
        self.alfabet = ['А','Б','В','Г','Д','Й','Ц','У','К','Е','Н','Ш','Щ','З','Х','Ъ','Ф','Ы','П','Р','О',
                        'Л','Ж','Э','Я','Ч','С','М','И','Т','Ь','Б','Ю','й','ц','у','к','е','н','г','ш','щ','з','х','ъ'
            ,'ф','ы','в','а','п','р','о','л','д','ж','э','я','ч','с','м','и','т','ь','б','ю']
        self.location_counter=0
        self.allvall = []
        self.EAX = ["EAX","AX","AL"]
        self.ECX = ["ECX", "CX", "CL"]
        self.EDX = ["EDX", "DX", "DL"]
        self.EBX = ["EBX", "BX", "BL"]
        self.ESP = ["ESP", "SP", "AH"]
        self.EBP = ["EBP", "BP", "CH"]
        self.ESI = ["ESI", "SI", "DH"]
        self.EDI = ["EDI", "DI", "BH"]
        self.REG8 = ["AL","CL","DL","BL","AH","CH", "DH","BH"]
        self.REG16 = ["AX","CX","DX","BX","SP","BP","SI","DI"]
        self.__SEP = ",.:[]\'\""
        self.jumps = []
        self.mysep = " ^"
        self.jumps = []
        self.__comands = ['JE', 'DAA', 'SETA', 'DIV', 'ADD', 'XOR', 'MOV', 'AND', 'CMP']
        self.encoding = {'a': "61", 'b': "62", 'c': '63', 'd': '64', 'e': '65',
                    'f': '66', 'g': '67', 'h': '68', 'i': '69', 'j': "6A",
                    'k': '6B', 'l': '6C', 'm': '6D', 'n': '6E', 'o': '6F',
                    'p': '70', 'q': '71', 'r': '72', 's': '73', 't': '74',
                    'u': '75', 'v': '76', 'w': '77', 'x': '78', 'y': '79',
                    'z': '7A', 'A': '41', 'B': '42', 'C': '43', 'D': '44',
                    'E': '45', 'F': '46', 'G': '47', 'H': '48', 'J': '4A',
                    'K': '4B', 'L': '4C', 'M': '4D', 'N': '4E', 'O': '4F',
                    'P': '50', 'Q': '51', 'R': '52', 'S': '53', 'T': '54',
                    'U': '55', 'V': '56', 'W': '57', 'X': '58', 'Y': '59',
                    'Z': '5A', '!': '21', '.': '2E', ',': '2C', ' ': '20',
                    'а': "D0 B0", 'б':"D0 B1", 'в':"D0 B2", 'г': "D0 B3", 'д': "D0 B4",
                    'е': "D0 B5", 'ё': "D1 91", 'ж': "D0 B6", 'з': "D0 B7", 'и': "D0 B8",
                    'й': "D0 B9", 'к': "D0 BA", 'л': "D0 BB", 'м': "D0 BC", 'н': "D0 BD",
                    'о': "D0 BE", 'п': "D0 BF", 'р': "D1 80", 'с': "D1 81", 'т': "D1 82",
                    'у': "D1 83", 'ф': "D1 84", 'х': "D1 85", 'ц': "D1 86", 'ч': "D1 87",
                    'ш': "D1 88", 'щ': "D1 89", 'ъ': "D1 8A", 'ы': "D1 8B", 'ь': "D1 8C",
                    'э': "D1 8D", 'ю': "D1 8E", 'я': "D1 8F", 'А': "D0 90", 'Б': "D0 91",
                    'В': "D0 92", 'Г': "D0 93", 'Д': "D0 94", 'Е': "D0 95", 'Ё': "D0 96",
                    'Ж': "D0 97", 'З': "D0 98", 'И': "D0 99", 'Й': "D0 9A", 'К': "D0 9B",
                    'Л': "D0 9C", 'М': "D0 9D", 'Н': "D0 9E", 'О': "D0 9F", 'П': "D0 A0",
                    'Р': "D0 A1", 'С': "D0 A2", 'Т': "D0 A2", 'У': "D0 A3", 'Ф': "D0 A4",
                    'Х': "D0 A5", 'Ц': "D0 A6", 'Ч': "D0 A7", 'Ш': "D0 A8", 'Щ': "D0 A9",
                    'Ъ': "D0 AA", 'Ы': "D0 AB", 'Ь': "D0 AC", 'Э': "D0 AD", 'Ю': "D0 AE",
                    'Я': "D0 AF"}
    def program(self, filename,lstname):
        Lex = Lexical()
        Lex.table(filename)
        self.pass_one1(lstname,Lex.lststr,filename)



    def seta_func(self,inf):
        out = ""
        out += "%04X"%(self.location_counter)
        out+="  "

        tmp = 0
        lenlst = len(self.allvall)
        for tmp in range(lenlst):
            out1 = self.allvall[tmp].name
            if(out1.upper()==inf.stroka[1]):
                break
        if(self.allvall[tmp].type.upper == "DW"):
            self.location_counter +=1
            out +="66| "
        out +="0F 97 05 0000"
        out+="%04X"%(self.allvall[tmp].adr)
        out += " R  "
        out+=inf.prt
        out+="\n"
        self.location_counter+=7
        return out

    def parse(self,s,SEPARATORS1):
        s = s[0:] + "^"
        result = []
        current_word = ''
        for char in s:
            if char == ";" :
                break
            if char in SEPARATORS1:
                if current_word: result.append(current_word.upper())

                if char in self.__SEP: result.append(char)
                current_word = ''
            else:
                current_word += char
        return result


    def value_fuct(self,inf,num,filename):
        vall = ""
        i=2
        j = 0
        out =""
        out += ("%04X"%(self.location_counter))
        out+="  "
        filelst = open("LIST.lst", "w", encoding='utf-8')

        if("DD" in inf.stroka):
            if (inf.f_oper[-1] == "H") and (inf.f_oper[0].isdigit()):
                tmp = inf.f_oper[:-1]
                if (len(tmp)<8):
                    tmp1="0"*(8-len(tmp))
                tmp1+=tmp
                for i in range (4) :
                    vall+=tmp1[-2:]
                    tmp1=tmp1[:-2]
                self.location_counter += 4
            elif (inf.f_oper[-1] == "D") and (inf.f_oper[0].isdigit()):
                vall = inf.f_oper[:-1]
                vall = str(hex(int(vall)))
                if (len(vall)<8):
                    vall+="0"*(8-len(vall))

                self.location_counter += 4
            elif (inf.f_oper.isdigit()):
                vall = str(hex(inf.f_oper))
                if (len(vall)<8):
                    vall+="0"*(8-len(vall))

                self.location_counter += 4
            elif (inf.f_oper[-1] == "Q") and (inf.f_oper[0].isdigit()) or (inf.f_oper[-1] == "O") and (inf.f_oper[0].isdigit()) :
                vall = inf.f_oper[:-1]
                vall = str(hex(int(vall,8)))
                if (len(vall) < 8):
                    vall += "0" * (8 - len(vall))

                self.location_counter += 4
            elif (inf.f_oper[-1] == "B") and (inf.f_oper[0].isdigit()):
                vall = inf.f_oper[:-1]
                vall = str(hex(int(vall,2)))
                if (len(vall)<8):
                    vall+="0"*(8-len(vall))

                self.location_counter += 4
            out+=vall[:]
            out+="     "+inf.prt+"\n"


            return out
        elif ("DB" in inf.stroka):
            if (inf.f_oper[-1] == "H") and (inf.f_oper[0].isdigit()):
                vall = inf.f_oper[:-1]
                if (len(vall)<2):
                    vall+="0"*(2-len(vall))

                self.location_counter += 1
            elif (inf.f_oper[-1] == "D") and (inf.f_oper[0].isdigit()):
                vall = inf.f_oper[:-1]
                vall = str(hex(int(vall)))
                if (len(vall)<2):
                    vall+="0"*(2-len(vall))

                self.location_counter += 1
            elif (inf.f_oper.isdigit()):
                vall = str(hex(inf.f_oper))
                if (len(vall)<2):
                    vall+="0"*(2-len(vall))

                self.location_counter += 1
            elif (inf.f_oper[-1] == "Q") and (inf.f_oper[0].isdigit()) or (inf.f_oper[-1] == "O") and (inf.f_oper[0].isdigit()) :
                vall = inf.f_oper[:-1]
                vall = str(hex(int(vall,8)))
                if (len(vall) < 2):
                    vall += "0" * (2 - len(vall))

                self.location_counter+= 1
            elif (inf.f_oper[-1] == "B") and (inf.f_oper[0].isdigit()):
                vall = inf.f_oper[:-1]
                vall = str(hex(int(vall,2)))
                if (len(vall)<2):
                    vall+="0"*(2-len(vall))

                self.location_counter += 1
            else :

                    ret =0
                    bool = 0
                    it = 0
                    for z in  inf.f_oper:
                        if (z=="\'") or (z == "\""):
                            continue
                        out+=self.encoding[z]
                        out+=" "
                        ret+=len(self.encoding[z])//2
                        if(len(self.encoding[z])//2==2):
                            it += 2
                        else:
                            it += 1
                        if (it>=7):
                            if(bool==0):
                                out+="  "+inf.prt + "\n       "
                                bool=1
                            else:  out+="\n       "
                            it=0
                    self.location_counter += ret-2
                    out +="\n"
                    return out
            out += vall[2:].upper()
            out += "     " + inf.prt + "\n"

            return out
        elif ("DW" in inf.stroka):
            if (inf.f_oper[-1] == "H") and (inf.f_oper[0].isdigit()):
                vall = inf.f_oper[:-1]
                if (len(vall)<4):
                    vall+="0"*(4-len(vall))
                    vall = vall[::-1]

                self.location_counter += 2
            elif (inf.f_oper[-1] == "D") and (inf.f_oper[0].isdigit()):
                vall = inf.f_oper[:-1]
                vall = str(hex(int(vall)))
                if (len(vall)<4):
                    vall+="0"*(4-len(vall))

                self.location_counter += 2
            elif (inf.f_oper.isdigit()):
                vall = str(hex(inf.f_oper))
                if (len(vall)<4):
                    vall+="0"*(4-len(vall))

                self.location_counter += 2
            elif (inf.f_oper[-1] == "Q") and (inf.f_oper[0].isdigit()) or (inf.f_oper[-1] == "O") and (inf.f_oper[0].isdigit()) :
                vall = inf.f_oper[:-1]
                vall = str(hex(int(vall,8)))
                if (len(vall) < 4):
                    vall += "0" * (4 - len(vall))

                self.location_counter += 2
            elif (inf.f_oper[-1] == "B") and (inf.f_oper[0].isdigit()):
                vall = inf.f_oper[:-1]
                vall = str(hex(int(vall,2)))
                if (len(vall)<4):
                    vall+="0"*(4-len(vall))
                self.location_counter += 2

            out += vall[:]
            out += "     " + inf.prt + "\n"

            return out

    def div_func(self,inf):
        out = ""
        out += "%04X" % (self.location_counter)
        out += "  "
        if (inf.stroka[1] in self.EAX):
            if(inf.stroka[1]=="AX"):
                out+="66| F7 F0"
                self.location_counter+=1
            elif(inf.stroka[1]=="AL"):
                out += "F6 F0"
            else:  out += "F7 F0"
        elif (inf.stroka[1] in self.EBX):
            if (inf.stroka[1] == "BX"):
                out += "66| F7 F3"
                self.location_counter += 1
            elif (inf.stroka[1] == "BL"):
                out += "F6 F3"
            else:
                out += "F7 F3"
        elif (inf.stroka[1] in self.ECX):
            if (inf.stroka[1] == "CX"):
                out += "66| F7 F1"
                self.location_counter += 1
            elif (inf.stroka[1] == "CL"):
                out += "F6 F1"
            else:
                out += "F7 F1"
        elif (inf.stroka[1] in self.EDX):
            if (inf.stroka[1] == "DX"):
                out += "66| F7 F2"
                self.location_counter += 1
            elif (inf.stroka[1] == "DL"):
                out += "F6 F2"
            else:
                out += "F7 F2"
        elif (inf.stroka[1] in self.EBP):
            if (inf.stroka[1] == "BP"):
                out += "66| F7 F5"
                self.location_counter += 1
            elif (inf.stroka[1] == "CH"):
                out += "F6 F5"
            else:
                out += "F7 F5"
        elif (inf.stroka[1] in self.ESI):
            if (inf.stroka[1] == "SI"):
                out += "66| F7 F6"
                self.location_counter += 1
            elif (inf.stroka[1] == "DH"):
                out += "F6 F6"
            else:
                out += "F7 F6"
        elif (inf.stroka[1] in self.EDI):
            if (inf.stroka[1] == "DI"):
                out += "66| F7 F7"
                self.location_counter += 1
            elif (inf.stroka[1] == "BH"):
                out += "F6 F7"
            else:
                out += "F7 F7"
        elif (inf.stroka[1] in self.ESP):
            if (inf.stroka[1] == "SP"):
                out += "66| F7 F4"
                self.location_counter += 1
            elif (inf.stroka[1] == "AH"):
                out += "F6 F4"
            else:
                out += "F7 F4"
        out +="          "+inf.prt + "\n"
        self.location_counter+=2
        return out

    def add_func(self,inf):
        out = ""
        out += "%04X" % (self.location_counter)
        out += "  "
        if ( inf.f_oper[-4:-1] == "EBP"):
            out += "3E: "
            self.location_counter+=1
        if(not( "["  in  inf.f_oper)) and (not( "]"  in  inf.f_oper)) :
            null = 0
            if (inf.s_oper in self.EAX):
                if (inf.s_oper == "AX"):
                    out += "66| 01 05"
                    self.location_counter += 1
                elif (inf.s_oper == "AL"):
                    out += "00 05"
                else:
                    out += "01 05"
            elif (inf.s_oper in self.ECX):
                if (inf.s_oper == "CX"):
                    out += "66| 01 0D"
                    self.location_counter += 1
                elif (inf.s_oper == "AL"):
                    out += "00 0D"
                else:
                    out += "01 0D"
            elif (inf.s_oper in self.EBX):
                if (inf.s_oper == "BX"):
                    out += "66| 01 1D"
                    self.location_counter += 1
                elif (inf.s_oper == "AL"):
                    out += "00 1D"
                else:
                    out += "01 1D"
            elif (inf.s_oper in self.EDX):
                if (inf.s_oper == "DX"):
                    out += "66| 01 15"
                    self.location_counter += 1
                elif (inf.s_oper == "AL"):
                    out += "00 15"
                else:
                    out += "01 15"
            elif (inf.s_oper in self.EBP):
                if (inf.s_oper == "BP"):
                    out += "66| 01 2D"
                    self.location_counter += 1
                elif (inf.s_oper == "CH"):
                    out += "00 2D"
                else:
                    out += "01 2D"
            elif (inf.s_oper in self.ESI):
                if (inf.s_oper == "SI"):
                    out += "66| 01 35"
                    self.location_counter += 1
                elif (inf.s_oper == "DH"):
                    out += "00 35"
                else:
                    out += "01 35"
            elif (inf.s_oper in self.EDI):
                if (inf.s_oper == "DI"):
                    out += "66| 01 3D"
                    self.location_counter += 1
                elif (inf.s_oper == "BH"):
                    out += "00 3D"
                else:
                    out += "01 3D"
            elif (inf.s_oper in self.ESP):
                if (inf.s_oper == "SP"):
                    out += "66| 01 25"
                    self.location_counter += 1
                elif (inf.s_oper == "AH"):
                    out += "00 25"
                else:
                    out += "01 25"
        else:
            null = 1
            if (inf.s_oper in self.EAX):
                if (inf.f_oper[-4:-1] in self.EAX):
                    if (not(inf.s_oper in self.REG8)):
                        out += "01 80"
                    else:
                        out += "00 80"
                elif (inf.f_oper[-4:-1] in self.ECX):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 81"
                    else:
                        out += "00 81"
                elif (inf.f_oper[-4:-1] in self.EDX):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 82"
                    else:
                        out += "00 82"
                elif (inf.f_oper[-4:-1] in self.EBX):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 83"
                    else:
                        out += "00 83"
                elif (inf.f_oper[-4:-1] in self.ESP):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 84"
                    else:
                        out += "00 84"
                elif (inf.f_oper[-4:-1] in self.EBP):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 85"
                    else:
                        out += "00 85"
                elif (inf.f_oper[-4:-1] in self.ESI):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 86"
                    else:
                        out += "00 86"
                elif (inf.f_oper[-4:-1] in self.EDI):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 87"
                    else:
                        out += "00 87"
            elif (inf.s_oper  in self.ECX):
                if (not (inf.s_oper in self.REG8)):
                    if (inf.s_oper != "AL"):
                        out += "01 88"
                    else:
                        out += "00 88"
                elif (inf.f_oper[-4:-1] in self.ECX):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 89"
                    else:
                        out += "00 89"
                elif (inf.f_oper[-4:-1] in self.EDX):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 8A"
                    else:
                        out += "00 8A"
                elif (inf.f_oper[-4:-1] in self.EBX):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 8B"
                    else:
                        out += "00 8B"
                elif (inf.f_oper[-4:-1] in self.ESP):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 8C"
                    else:
                        out += "00 8C"
                elif (inf.f_oper[-4:-1] in self.EBP):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 8D"
                    else:
                        out += "00 8D"
                elif (inf.f_oper[-4:-1] in self.ESI):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 8E"
                    else:
                        out += "00 8E"
                elif (inf.f_oper[-4:-1] in self.EDI):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 8F"
                    else:
                        out += "00 8F"
            elif (inf.s_oper  in self.EDX):
                if (not (inf.s_oper in self.REG8)):
                    if (inf.s_oper != "AL"):
                        out += "01 90"
                    else:
                        out += "00 90"
                elif (inf.f_oper[-4:-1] in self.ECX):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 91"
                    else:
                        out += "00 91"
                elif (inf.f_oper[-4:-1] in self.EDX):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 92"
                    else:
                        out += "00 92"
                elif (inf.f_oper[-4:-1] in self.EBX):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 93"
                    else:
                        out += "00 93"
                elif (inf.f_oper[-4:-1] in self.ESP):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 94"
                    else:
                        out += "00 94"
                elif (inf.f_oper[-4:-1] in self.EBP):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 95"
                    else:
                        out += "00 95"
                elif (inf.f_oper[-4:-1] in self.ESI):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 96"
                    else:
                        out += "00 96"
                elif (inf.f_oper[-4:-1] in self.EDI):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 97"
                    else:
                        out += "00 97"
            elif (inf.s_oper  in self.EBX):
                if (inf.f_oper[-4:-1] in self.EAX):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 98"
                    else:
                        out += "00 98"
                elif (inf.f_oper[-4:-1] in self.ECX):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 99"
                    else:
                        out += "00 99"
                elif (inf.f_oper[-4:-1] in self.EDX):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 9A"
                    else:
                        out += "00 9A"
                elif (inf.f_oper[-4:-1] in self.EBX):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 9B"
                    else:
                        out += "00 9B"
                elif (inf.f_oper[-4:-1] in self.ESP):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 9C"
                    else:
                        out += "00 9C"
                elif (inf.f_oper[-4:-1] in self.EBP):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 9D"
                    else:
                        out += "00 9D"
                elif (inf.f_oper[-4:-1] in self.ESI):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 9E"
                    else:
                        out += "00 9E"
                elif (inf.f_oper[-4:-1] in self.EDI):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 9F"
                    else:
                        out += "00 9F"
            elif (inf.s_oper  in self.ESP):
                if (inf.f_oper[-4:-1] in self.EAX):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 A0"
                    else:
                        out += "00 A0"
                elif (inf.f_oper[-4:-1] in self.ECX):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 A1"
                    else:
                        out += "00 A1"
                elif (inf.f_oper[-4:-1] in self.EDX):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 A2"
                    else:
                        out += "00 A2"
                elif (inf.f_oper[-4:-1] in self.EBX):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 A3"
                    else:
                        out += "00 A3"
                elif (inf.f_oper[-4:-1] in self.ESP):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 A4"
                    else:
                        out += "00 A4"
                elif (inf.f_oper[-4:-1] in self.EBP):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 A5"
                    else:
                        out += "00 A5"
                elif (inf.f_oper[-4:-1] in self.ESI):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 A6"
                    else:
                        out += "00 A6"
                elif (inf.f_oper[-4:-1] in self.EDI):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 A7"
                    else:
                        out += "00 A7"
            elif (inf.s_oper  in self.EBP):
                if (inf.f_oper[-4:-1] in self.EAX):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 A8"
                    else:
                        out += "00 A8"
                elif (inf.f_oper[-4:-1] in self.ECX):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 A9"
                    else:
                        out += "00 A9"
                elif (inf.f_oper[-4:-1] in self.EDX):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 AA"
                    else:
                        out += "00 AA"
                elif (inf.f_oper[-4:-1] in self.EBX):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 AB"
                    else:
                        out += "00 AB"
                elif (inf.f_oper[-4:-1] in self.ESP):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 AC"
                    else:
                        out += "00 AC"
                elif (inf.f_oper[-4:-1] in self.EBP):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 AD"
                    else:
                        out += "00 AD"
                elif (inf.f_oper[-4:-1] in self.ESI):
                    if (not(inf.s_oper in self.REG8)):
                        out += "01 AE"
                    else:
                        out += "00 AE"
                elif (inf.f_oper[-4:-1] in self.EDI):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 AF"
                    else:
                        out += "00 AF"
            elif (inf.s_oper  in self.ESI):
                if (inf.f_oper[-4:-1] in self.EAX):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 B0"
                    else:
                        out += "00 B0"
                elif (inf.f_oper[-4:-1] in self.ECX):
                    if (not(inf.s_oper in self.REG8)):
                        out += "01 B1"
                    else:
                        out += "00 B1"
                elif (inf.f_oper[-4:-1] in self.EDX):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 B2"
                    else:
                        out += "00 B2"
                elif (inf.f_oper[-4:-1] in self.EBX):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 B3"
                    else:
                        out += "00 B3"
                elif (inf.f_oper[-4:-1] in self.ESP):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 B4"
                    else:
                        out += "00 B4"
                elif (inf.f_oper[-4:-1] in self.EBP):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 B5"
                    else:
                        out += "00 B5"
                elif (inf.f_oper[-4:-1] in self.ESI):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 B6"
                    else:
                        out += "00 B6"
                elif (inf.f_oper[-4:-1] in self.EDI):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 B7"
                    else:
                        out += "00 B7"
            elif (inf.s_oper  in self.EDI):
                if (inf.f_oper[-4:-1] in self.EAX):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 B8"
                    else:
                        out += "00 B8"
                elif (inf.f_oper[-4:-1] in self.ECX):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 B9"
                    else:
                        out += "00 B9"
                elif (inf.f_oper[-4:-1] in self.EDX):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 BA"
                    else:
                        out += "00 BA"
                elif (inf.f_oper[-4:-1] in self.EBX):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 BB"
                    else:
                        out += "00 BB"
                elif (inf.f_oper[-4:-1] in self.ESP):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 BC"
                    else:
                        out += "00 BC"
                elif (inf.f_oper[-4:-1] in self.EBP):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 BD"
                    else:
                        out += "00 BD"
                elif (inf.f_oper[-4:-1] in self.ESI):
                    if (not(inf.s_oper in self.REG8)):
                        out += "01 BE"
                    else:
                        out += "00 BE"
                elif (inf.f_oper[-4:-1] in self.EDI):
                    if (not (inf.s_oper in self.REG8)):
                        out += "01 BF"
                    else:
                        out += "00 BF"
        out +=" 0000"
        tmp = 0
        lenlst = len(self.allvall)
        for tmp in range(lenlst):
            out1 = self.allvall[tmp].name
            name = inf.stroka[1][:]
            if (null) :
                name = name [:-5]
            if (out1.upper() == name):
                break

        out += "%04X" % (self.allvall[tmp].adr)
        out += " R  "
        out += inf.prt
        out += "\n"
        self.location_counter += 6
        return out

    def cmp_func (self,inf):
        out = ""
        out += "%04X" % (self.location_counter)
        out += "  "
        if (inf.f_oper in self.EAX):
            if (inf.s_oper in  self.EAX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B C0"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A C0"
                else:
                    out += "3B C0"
            elif (inf.s_oper in  self.ECX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B C1"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A C1"
                else:
                    out += "3B C1"
            elif (inf.s_oper in  self.EDX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B C2"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A C2"
                else:
                    out += "3B C2"
            elif (inf.s_oper in  self.EBX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B C3"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A C3"
                else:
                    out += "3B C3"
            elif (inf.s_oper in  self.ESP):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B C4"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A C4"
                else:
                    out += "3B C4"
            elif (inf.s_oper in  self.EBP):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B C5"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A C5"
                else:
                    out += "3B C5"
            elif (inf.s_oper in  self.ESI):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B C6"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A C6"
                else:
                    out += "3B C6"
            elif (inf.s_oper in self.EDI):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B C7"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A C7"
                else:
                    out += "3B C7"


        elif (inf.f_oper in self.ECX):
            if (inf.s_oper in self.EAX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B C8"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A C8"
                else:
                    out += "3B C8"
            elif (inf.s_oper in self.ECX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B C9"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A C9"
                else:
                    out += "3B C9"
            elif (inf.s_oper in self.EDX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B CA"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A CA"
                else:
                    out += "3B CA"
            elif (inf.s_oper in self.EBX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B CB"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A CB"
                else:
                    out += "3B CB"
            elif (inf.s_oper in self.ESP):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B CC"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A CC"
                else:
                    out += "3B CC"
            elif (inf.s_oper in self.EBP):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B CD"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A CD"
                else:
                    out += "3B CD"
            elif (inf.s_oper in self.ESI):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B CE"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A CE"
                else:
                    out += "3B CE"
            elif (inf.s_oper in self.EDI):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B CF"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A CF"
                else:
                    out += "3B CF"
        elif (inf.f_oper in self.EDX):
            if (inf.s_oper in self.EAX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B D0"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A D0"
                else:
                    out += "3B D0"
            elif (inf.s_oper in self.ECX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B D1"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A D1"
                else:
                    out += "3B D1"
            elif (inf.s_oper in self.EDX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B D2"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A D2"
                else:
                    out += "3B D2"
            elif (inf.s_oper in self.EBX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B D3"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A D3"
                else:
                    out += "3B D3"
            elif (inf.s_oper in self.ESP):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B D4"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A D4"
                else:
                    out += "3B D4"
            elif (inf.s_oper in self.EBP):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B D5"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A D5"
                else:
                    out += "3B D5"
            elif (inf.s_oper in self.ESI):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B D6"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A D6"
                else:
                    out += "3B D6"
            elif (inf.s_oper in self.EDI):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B D7"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A D7"
                else:
                    out += "3B D7"
        elif (inf.f_oper in self.EBX):
            if (inf.s_oper in self.EAX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B D8"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A D8"
                else:
                    out += "3B D8"
            elif (inf.s_oper in self.ECX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B D9"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A D9"
                else:
                    out += "3B D9"
            elif (inf.s_oper in self.EDX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B DA"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A DA"
                else:
                    out += "3B DA"
            elif (inf.s_oper in self.EBX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B DB"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A DB"
                else:
                    out += "3B DB"
            elif (inf.s_oper in self.ESP):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B DC"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A DC"
                else:
                    out += "3B DC"
            elif (inf.s_oper in self.EBP):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B DD"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A DD"
                else:
                    out += "3B DD"
            elif (inf.s_oper in self.ESI):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B DE"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A DE"
                else:
                    out += "3B DE"
            elif (inf.s_oper in self.EDI):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B DF"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A DF"
                else:
                    out += "3B DF"
        elif (inf.f_oper in self.ESP):
            if (inf.s_oper in self.EAX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B E0"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A E0"
                else:
                    out += "3B E0"
            elif (inf.s_oper in self.ECX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B E1"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A E1"
                else:
                    out += "3B E1"
            elif (inf.s_oper in self.EDX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B E2"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A E2"
                else:
                    out += "3B E2"
            elif (inf.s_oper in self.EBX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B E3"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A E3"
                else:
                    out += "3B E3"
            elif (inf.s_oper in self.ESP):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B E4"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A E4"
                else:
                    out += "3B E4"
            elif (inf.s_oper in self.EBP):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B E5"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A E5"
                else:
                    out += "3B E5"
            elif (inf.s_oper in self.ESI):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B E6"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A E6"
                else:
                    out += "3B E6"
            elif (inf.s_oper in self.EDI):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B E7"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A E7"
                else:
                    out += "3B E7"
        elif (inf.f_oper in self.EBP):
            if (inf.s_oper in self.EAX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B E8"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A E8"
                else:
                    out += "3B E8"
            elif (inf.s_oper in self.ECX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B E9"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A E9"
                else:
                    out += "3B E9"
            elif (inf.s_oper in self.EDX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B EA"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A EA"
                else:
                    out += "3B EA"
            elif (inf.s_oper in self.EBX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B EB"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A EB"
                else:
                    out += "3B EB"
            elif (inf.s_oper in self.ESP):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B EC"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A EC"
                else:
                    out += "3B EC"
            elif (inf.s_oper in self.EBP):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B ED"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A ED"
                else:
                    out += "3B ED"
            elif (inf.s_oper in self.ESI):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B EE"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A EE"
                else:
                    out += "3B EE"
            elif (inf.s_oper in self.EDI):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B EF"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A EF"
                else:
                    out += "3B EF"
        elif (inf.f_oper in self.ESI):
            if (inf.s_oper in self.EAX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B F0"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A F0"
                else:
                    out += "3B F0"
            elif (inf.s_oper in self.ECX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B F1"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A F1"
                else:
                    out += "3B F1"
            elif (inf.s_oper in self.EDX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B F2"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A F2"
                else:
                    out += "3B F2"
            elif (inf.s_oper in self.EBX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B F3"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A F3"
                else:
                    out += "3B F3"
            elif (inf.s_oper in self.ESP):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B F4"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A F4"
                else:
                    out += "3B F4"
            elif (inf.s_oper in self.EBP):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B F5"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A F5"
                else:
                    out += "3B F5"
            elif (inf.s_oper in self.ESI):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B F6"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A F6"
                else:
                    out += "3B F6"
            elif (inf.s_oper in self.EDI):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B F7"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A F7"
                else:
                    out += "3B F7"
        elif (inf.f_oper in self.EDI):
            if (inf.s_oper in self.EAX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B F8"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A F8"
                else:
                    out += "3B F8"
            elif (inf.s_oper in self.ECX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B F9"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A F9"
                else:
                    out += "3B F9"
            elif (inf.s_oper in self.EDX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B FA"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A FA"
                else:
                    out += "3B FA"
            elif (inf.s_oper in self.EBX):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B FB"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A FB"
                else:
                    out += "3B FB"
            elif (inf.s_oper in self.ESP):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B FC"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A FC"
                else:
                    out += "3B FC"
            elif (inf.s_oper in self.EBP):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B FD"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A FD"
                else:
                    out += "3B FD"
            elif (inf.s_oper in self.ESI):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B FE"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A FE"
                else:
                    out += "3B FE"
            elif (inf.s_oper in self.EDI):
                if (inf.f_oper in self.REG16) or (inf.s_oper in self.REG16):
                    out += "66| 3B FF"
                    self.location_counter += 1
                elif (inf.f_oper in self.REG8) or (inf.s_oper in self.REG8):
                    out += "3A FF"
                else:
                    out += "3B FF"

        out += "          " + inf.prt + "\n"
        self.location_counter += 2
        return out

    def xor_func(self,inf):
        out = ""
        out += "%04X" % (self.location_counter)
        out += "  "
        tmp = 0
        lenlst = len(self.allvall)
        for tmp in range(lenlst):
            out1 = self.allvall[tmp].name
            if (out1.upper() == inf.f_oper):
                break
        if(self.allvall[tmp].type.upper()=="DB"):
            out += "80 35 0000"
            self.location_counter+=4
        elif(self.allvall[tmp].type.upper()=="DW"):
            out += "66| 81 35 0000"
            self.location_counter += 5
        else:
            out += " 81 35 0000"
            self.location_counter += 4
        out += "%04X" % (self.allvall[tmp].adr)
        self.location_counter += 2
        out += " R  "
        tmp = 0
        for tmp in range(lenlst):
            out1 = self.allvall[tmp].name
            if (out1.upper() == inf.s_oper):
                break
        out+=self.allvall[tmp].adr
        plus = int((len(self.allvall[tmp].adr))/2)
        out += "    " + inf.prt
        out += "\n"
        self.location_counter += plus
        return out

    def mov_func(self,inf):
        out = ""
        out += "%04X" % (self.location_counter)
        out += "  "
        if (inf.f_oper in self.EAX):
            if(inf.stroka[1]=="AX"):
                out+="66| B8"
                self.location_counter+=1
                rozr = 4
            elif(inf.stroka[1]=="AL"):
                out += "B0"
                rozr = 2
            else:
                out += "B8"
                rozr = 8
        elif (inf.f_oper in self.EBX):
            if (inf.stroka[1] == "BX"):
                out += "66| BB"
                rozr = 4
                self.location_counter += 1
            elif (inf.stroka[1] == "BL"):
                out += "B3"
                rozr = 2
            else:
                out += "BB"
                rozr = 8
        elif (inf.f_oper in self.ECX):
            if (inf.stroka[1] == "CX"):
                out += "66| B9"
                rozr = 4
                self.location_counter += 1
            elif (inf.stroka[1] == "CL"):
                out += "B1"
                rozr = 2
            else:
                out += "B9"
                rozr = 8
        elif (inf.f_oper in self.EDX):
            if (inf.stroka[1] == "DX"):
                out += "66| BA"
                rozr = 4
                self.location_counter += 1
            elif (inf.stroka[1] == "DL"):
                out += "B2"
                rozr = 2
            else:
                out += "BA"
                rozr = 8
        elif (inf.f_oper in self.EBP):
            if (inf.stroka[1] == "BP"):
                out += "66| BD"
                rozr = 4
                self.location_counter += 1
            elif (inf.stroka[1] == "CH"):
                out += "B5"
                rozr = 2
            else:
                out += "BD"
                rozr = 8
        elif (inf.f_oper in self.ESI):
            if (inf.stroka[1] == "SI"):
                out += "66| BE"
                rozr = 4
                self.location_counter += 1
            elif (inf.stroka[1] == "DH"):
                out += "B6"
                rozr = 2
            else:
                out += "B6"
                rozr = 8
        elif (inf.f_oper in self.EDI):
            if (inf.stroka[1] == "DI"):
                out += "66| BF"
                rozr = 4
                self.location_counter += 1
            elif (inf.stroka[1] == "BH"):
                out += "B7"
                rozr = 2
            else:
                out += "B7"
                rozr = 8
        elif (inf.f_oper in self.ESP):
            if (inf.stroka[1] == "SP"):
                out += "66| BC"
                rozr = 4
                self.location_counter += 1
            elif (inf.stroka[1] == "AH"):
                out += "B4"
                rozr = 2
            else:
                out += "B4"
                rozr = 8
        self.location_counter+=1
        tmp = 0
        lenlst = len(self.allvall)
        for tmp in range(lenlst):
            out1 = self.allvall[tmp].name
            if (out1.upper() == inf.s_oper):
                break
        if(len(self.allvall[tmp].adr)>rozr):
            print("Out of range in MOV")
        out+= " "
        temps ="0"*(rozr-len(self.allvall[tmp].adr))
        temps+=self.allvall[tmp].adr
        out += temps + "    " + inf.prt
        out += "\n"
        self.location_counter += int((len(temps))/2)
        return out

    def and_func (self,inf):
        out = ""
        out += "%04X" % (self.location_counter)
        out += "  "
        if ( inf.s_oper[-4:-1] == "EBP"):
            out += "3E: "
            self.location_counter+=1
        if(not( "["  in  inf.s_oper)) and (not( "]"  in  inf.s_oper)) :
            null = 0
            if (inf.f_oper in self.EAX):
                if (inf.f_oper  == "AX"):
                    out += "66| 23 05"
                    self.location_counter += 1
                elif (inf.f_oper  == "AL"):
                    out += "22 05"
                else:
                    out += "23 05"
            elif (inf.f_oper  in self.EBX):
                if (inf.f_oper == "BX"):
                    out += "66| 23 1D"
                    self.location_counter += 1
                elif (inf.f_oper  == "BL"):
                    out += "22 1D"
                else:
                    out += "23 1D"
            elif (inf.f_oper  in self.ECX):
                if (inf.f_oper  == "CX"):
                    out += "66| 23 0D"
                    self.location_counter += 1
                elif (inf.f_oper  == "CL"):
                    out += "22 0D"
                else:
                    out += "23 0D"
            elif (inf.f_oper  in self.EDX):
                if (inf.f_oper == "DX"):
                    out += "66| 23 15"
                    self.location_counter += 1
                elif (inf.f_oper  == "DL"):
                    out += "22 15"
                else:
                    out += "23 15"
            elif (inf.f_oper  in self.EBP):
                if (inf.f_oper  == "BP"):
                    out += "66| 23 2D"
                    self.location_counter += 1
                elif (inf.f_oper  == "CH"):
                    out += "22 2D"
                else:
                    out += "23 2D"
            elif (inf.f_oper  in self.ESI):
                if (inf.f_oper  == "SI"):
                    out += "66| 23 35"
                    self.location_counter += 1
                elif (inf.f_oper  == "DH"):
                    out += "22 35"
                else:
                    out += "23 35"
            elif (inf.f_oper  in self.EDI):
                if (inf.f_oper  == "DI"):
                    out += "66| 23 3D"
                    self.location_counter += 1
                elif (inf.f_oper  == "BH"):
                    out += "22 3D"
                else:
                    out += "23 3D"
            elif (inf.f_oper  in self.ESP):
                if (inf.f_oper  == "SP"):
                    out += "66| 23 25"
                    self.location_counter += 1
                elif (inf.f_oper  == "AH"):
                    out += "22 25"
                else:
                    out += "23 25"
        else:
            null = 1
            if (inf.f_oper in self.EAX):
                if (inf.s_oper[-4:-1] in self.EAX):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 80"
                    else: out += "22 80"
                elif (inf.s_oper[-4:-1] in self.ECX):
                    if (not(inf.f_oper in self.REG8)):
                        out += "23 81"
                    else: out += "22 81"
                elif (inf.s_oper[-4:-1] in self.EDX):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 82"
                    else: out += "22 82"
                elif (inf.s_oper[-4:-1] in self.EBX):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 83"
                    else: out += "22 83"
                elif (inf.s_oper[-4:-1] in self.ESP):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 84"
                    else: out += "22 84"
                elif (inf.s_oper[-4:-1] in self.EBP):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 85"
                    else: out += "22 85"
                elif (inf.s_oper[-4:-1] in self.ESI):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 86"
                    else: out += "22 86"
                elif (inf.s_oper[-4:-1] in self.EDI):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 87"
                    else: out += "22 87"
            elif (inf.f_oper in self.ECX):
                if (inf.s_oper[-4:-1] in self.EAX):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 88"
                    else: out += "22 88"
                elif (inf.s_oper[-4:-1] in self.ECX):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 89"
                    else: out += "22 89"
                elif (inf.s_oper[-4:-1] in self.EDX):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 8A"
                    else: out += "22 8A"
                elif (inf.s_oper[-4:-1] in self.EBX):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 8B"
                    else: out += "22 8B"
                elif (inf.s_oper[-4:-1] in self.ESP):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 8C"
                    else: out += "22 8C"
                elif (inf.s_oper[-4:-1] in self.EBP):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 8D"
                    else: out += "22 8D"
                elif (inf.s_oper[-4:-1] in self.ESI):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 8E"
                    else: out += "22 8E"
                elif (inf.s_oper[-4:-1] in self.EDI):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 8F"
                    else: out += "22 8F"
            elif (inf.f_oper in self.EDX):
                if (inf.s_oper[-4:-1] in self.EAX):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 90"
                    else: out += "22 90"
                elif (inf.s_oper[-4:-1] in self.ECX):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 91"
                    else: out += "22 91"
                elif (inf.s_oper[-4:-1] in self.EDX):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 92"
                    else: out += "22 92"
                elif (inf.s_oper[-4:-1] in self.EBX):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 93"
                    else: out += "22 93"
                elif (inf.s_oper[-4:-1] in self.ESP):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 94"
                    else: out += "22 94"
                elif (inf.s_oper[-4:-1] in self.EBP):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 95"
                    else: out += "22 95"
                elif (inf.s_oper[-4:-1] in self.ESI):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 96"
                    else: out += "22 96"
                elif (inf.s_oper[-4:-1] in self.EDI):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 97"
                    else: out += "22 97"
            elif (inf.f_oper in self.EBX):
                if (inf.s_oper[-4:-1] in self.EAX):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 98"
                    else: out += "22 98"
                elif (inf.s_oper[-4:-1] in self.ECX):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 99"
                    else: out += "22 99"
                elif (inf.s_oper[-4:-1] in self.EDX):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 9A"
                    else: out += "22 9A"
                elif (inf.s_oper[-4:-1] in self.EBX):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 9B"
                    else: out += "22 9B"
                elif (inf.s_oper[-4:-1] in self.ESP):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 9C"
                    else: out += "22 9C"
                elif (inf.s_oper[-4:-1] in self.EBP):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 9D"
                    else: out += "22 9D"
                elif (inf.s_oper[-4:-1] in self.ESI):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 9E"
                    else: out += "22 9E"
                elif (inf.s_oper[-4:-1] in self.EDI):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 9F"
                    else: out += "22 9F"
            elif (inf.f_oper in self.ESP):
                if (inf.s_oper[-4:-1] in self.EAX):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 A0"
                    else: out += "22 A0"
                elif (inf.s_oper[-4:-1] in self.ECX):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 A1"
                    else: out += "22 A1"
                elif (inf.s_oper[-4:-1] in self.EDX):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 A2"
                    else: out += "22 A2"
                elif (inf.s_oper[-4:-1] in self.EBX):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 A3"
                    else: out += "22 A3"
                elif (inf.s_oper[-4:-1] in self.ESP):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 A4"
                    else: out += "22 A4"
                elif (inf.s_oper[-4:-1] in self.EBP):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 A5"
                    else: out += "22 A5"
                elif (inf.s_oper[-4:-1] in self.ESI):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 A6"
                    else: out += "22 A6"
                elif (inf.s_oper[-4:-1] in self.EDI):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 A7"
                    else: out += "22 A7"
            elif (inf.f_oper in self.EBP):
                if (inf.s_oper[-4:-1] in self.EAX):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 A8"
                    else: out += "22 A8"
                elif (inf.s_oper[-4:-1] in self.ECX):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 A9"
                    else: out += "22 A9"
                elif (inf.s_oper[-4:-1] in self.EDX):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 AA"
                    else: out += "22 AA"
                elif (inf.s_oper[-4:-1] in self.EBX):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 AB"
                    else: out += "22 AB"
                elif (inf.s_oper[-4:-1] in self.ESP):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 AC"
                    else: out += "22 AC"
                elif (inf.s_oper[-4:-1] in self.EBP):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 AD"
                    else: out += "22 AD"
                elif (inf.s_oper[-4:-1] in self.ESI):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 AE"
                    else: out += "22 AE"
                elif (inf.s_oper[-4:-1] in self.EDI):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 AF"
                    else: out += "22 AF"
            elif (inf.f_oper in self.ESI):
                if (inf.s_oper[-4:-1] in self.EAX):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 B0"
                    else: out += "22 B0"
                elif (inf.s_oper[-4:-1] in self.ECX):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 B1"
                    else: out += "22 B1"
                elif (inf.s_oper[-4:-1] in self.EDX):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 B2"
                    else: out += "22 B2"
                elif (inf.s_oper[-4:-1] in self.EBX):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 B3"
                    else: out += "22 B3"
                elif (inf.s_oper[-4:-1] in self.ESP):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 B4"
                    else: out += "22 B4"
                elif (inf.s_oper[-4:-1] in self.EBP):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 B5"
                    else: out += "22 B5"
                elif (inf.s_oper[-4:-1] in self.ESI):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 B6"
                    else: out += "22 B6"
                elif (inf.s_oper[-4:-1] in self.EDI):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 B7"
                    else: out += "22 B7"
            elif (inf.f_oper in self.EDI):
                if (inf.s_oper[-4:-1] in self.EAX):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 B8"
                    else: out += "22 B8"
                elif (inf.s_oper[-4:-1] in self.ECX):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 B9"
                    else: out += "22 B9"
                elif (inf.s_oper[-4:-1] in self.EDX):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 BA"
                    else: out += "22 BA"
                elif (inf.s_oper[-4:-1] in self.EBX):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 BB"
                    else: out += "22 BB"
                elif (inf.s_oper[-4:-1] in self.ESP):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 BC"
                    else: out += "22 BC"
                elif (inf.s_oper[-4:-1] in self.EBP):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 BD"
                    else: out += "22 BD"
                elif (inf.s_oper[-4:-1] in self.ESI):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 BE"
                    else: out += "22 BE"
                elif (inf.s_oper[-4:-1] in self.EDI):
                    if (not (inf.f_oper in self.REG8)):
                        out += "23 BF"
                    else: out += "22 BF"

        out += " 0000"
        tmp = 0
        lenlst = len(self.allvall)
        for tmp in range(lenlst):
            out1 = self.allvall[tmp].name
            name = inf.s_oper[:]
            if (null):
                name = name[:-5]
            if (out1.upper() == name):
                break

        out += "%04X" % (self.allvall[tmp].adr)

        out += " R  "
        out += inf.prt
        out += "\n"
        self.location_counter += 6
        return out


    def pass_sec(self):

        lenall = len(self.allvall)
        lenlst = len(self.jumps)
        for i in range(lenlst):
            out = ""
            if ("JE" in self.jumps[i]):
                out = out[:]
                out = self.jumps[i][0][:]
                if (self.jumps[i][1]== "0F84"):
                    out +="  0F 84 0000"
                    for x in range (lenall):
                        if (self.jumps[i][-1][:-1].upper() == self.allvall[x].name.upper()):
                            break
                    out+=("%04X"%(self.allvall[x].adr))
                    out+=" R    "
                    out+= self.jumps[i][2] + "  " + self.jumps[i][3]
                    self.jumps[i] = out [:]
                else:
                    out+="  74 "
                    for x in range (lenall):
                        if (self.jumps[i][-1][:].upper() == self.allvall[x].name.upper()):
                            break
                    out += hex(int("ff",16) - (int(str(self.jumps[i][0][:]), 16) - int(str(self.allvall[x].adr), 16)) - 1)[-2:].upper()
                    out +=  "  "+self.jumps[i][2] + "  " + self.jumps[i][3]
                    self.jumps[i] = out[:]
            elif("JMP" in self.jumps[i] ):
                out = out[:]
                out = self.jumps[i][0][:] + "  EB "
                for p in range(lenall):
                    if (self.jumps[i][-1][:].upper() == self.allvall[p].name.upper()):
                        break
                if (int(hex(self.allvall[p].adr), 16) > int(str(self.jumps[i][0][:]), 16)) :
                    tmp =  hex(int(hex(self.allvall[p].adr), 16) - int(str(self.jumps[i][0][:]), 16)-2)[-2:].upper()
                    if (tmp[-2]!="X"):
                        out +=tmp
                    else: out +="0"+tmp[-1]
                else:
                    out += hex(int("ff", 16) - (int(str(self.jumps[i][0][:]), 16) - int(str(self.allvall[p].adr), 16)) - 1)[-2:].upper()
                out += "  " + self.jumps[i][2] + "  " + self.jumps[i][3]+"  "+self.jumps[i][4]
                self.jumps[i] = out[:]






    def pass_one1(self,filename,lststr,filename2):
        FileAsm = open(filename2, 'r', encoding='utf-8')
        lines = [string.strip() for string in FileAsm]
        FileAsm.close()
        filelst = open ("LIST.lst","w", encoding='utf-8')
        filelst.write("Course Work by Bond Yaroslav\n\n\n")
        self.location_counter=0
        in_segment = 0
        str_print = ""
        wiev = []
        segm = []
        strsegm = ""
        locsegm = []
        allval = []
        allvals =[]
        linetmp = lines[0]
        num_str=0
        num = 0
        Errors = 0
        vallVoc={}
        if (linetmp[0]=="."):
            filelst.write(linetmp+"\n")
            num_str+=1
            num +=1
        lenlst = len(lststr)

        for asminf in range(lenlst) :
            jmp = ""
            jmp = jmp [:]
            num += 1


            if (lststr[asminf].stroka[0] == "END"):
                filelst.write(lststr[asminf].prt + "\n")
                break
            if(lststr[asminf].prt[0]==";"):
                filelst.write(lststr[asminf].prt + "\n")
                num_str += 1
                continue
            if (lststr[asminf].Lexem==1) and (lststr[asminf].stroka[0][-1]==":"):
                ADD = vallues()
                ADD.name = lststr[asminf].stroka[0][0:-1]
                ADD.adr = self.location_counter
                ADD.type_in_table = "L NEAR"
                ADD.segm = SEGM[:]
                self.allvall.append(ADD)
                temp = lststr[asminf].stroka[0]
                temp = temp[0:-1]
                wiev.append(temp)
            if (lststr[asminf].Lexem==1) and (not("SEGMENT" in lststr[asminf].stroka)) and (not("ENDS" in lststr[asminf].stroka)):
                allvals.append(lststr[asminf].stroka[0])
                tmp = []
                tmp.append(lststr[asminf].stroka[0])
                tmp.append(self.location_counter)
                allval.append(tmp)
            if (lststr[asminf].Lexem==1) and (len(lststr[asminf].stroka)==1):
                filelst.write("%04X" % (self.location_counter))
                filelst.write("   " + lststr[asminf].prt + "\n")
                num_str += 1
                continue
            if(lststr[asminf].stroka[0]=="ASSUME"):
                filelst.write("   " + lststr[asminf].prt + "\n")
                num_str += 1
                continue
            elif("SEGMENT" in lststr[asminf].stroka ) and (lststr[asminf].Lexem!=-1):
                strsegm=""
                strsegm = lststr[asminf].stroka[0] + ".................."
                if(lststr[asminf].stroka[-1]=="USE16"):
                    strsegm += "16 Bit  "
                else:
                    strsegm += "32 Bit  "

                in_segment=1
                SEGM = lststr[asminf].stroka[0][:]
                self.location_counter=0
                filelst.write("0000 "+lststr[asminf].prt+"\n")
                num_str += 1
                continue
            elif("ENDS" in lststr[asminf].stroka) and (lststr[asminf].Lexem!=-1):
                strsegm += "  "+str(hex(self.location_counter))+"  PARA  NONE"
                segm.append(strsegm)
                locsegm.append(str(hex(self.location_counter)))
                in_segment = 0
                filelst.write("%04X"%(self.location_counter) )
                filelst.write("  " + lststr[asminf].prt + "\n")
                num_str += 1
                continue
            elif("EQU" in lststr[asminf].stroka):
                filelst.write("=")
                EQU = vallues()
                oper = lststr[asminf].f_oper
                if(oper[-1]=="H") and (oper[0].isdigit()) or (oper.isdigit()) or (oper[-1]=="O") and (oper[0].isdigit())  or (oper[-1]=="Q") and (oper[0].isdigit()) or (oper[-1]=="D") and (oper[0].isdigit()) or (oper[-1] == "B") and (
                oper[0].isdigit()):
                    if(oper[-1]=="H") and (oper[0].isdigit()):
                        oper=oper[0:-1]
                        filelst.write(oper+"  "+lststr[asminf].prt+"\n")
                        EQU.adr = oper
                    elif (oper[0].isdigit()) and(oper[-1]=="D") :
                        oper = oper[0:-1]
                        filelst.write("%04X"%(int(oper)))
                        EQU.adr = ("%X" % (int(oper)))
                        filelst.write("  " + lststr[asminf].prt + "\n")
                    elif (oper[0].isdigit()) and (oper[-1] == "Q") or (oper[-1] == "O"):
                        oper = oper[0:-1]
                        filelst.write("%04X" % (int(str(oper),8)))
                        EQU.adr = ("%X" % (int(str(oper),8)))
                        filelst.write("  " + lststr[asminf].prt + "\n")
                    elif (oper[0].isdigit()) and (oper[-1] == "B"):
                        oper = oper[0:-1]
                        filelst.write("%04X" % (int(str(oper), 2)))
                        EQU.adr = ("%X" % (int(str(oper), 2)))
                        filelst.write("  " + lststr[asminf].prt + "\n")
                    else:
                        oper=oper[0:-1]
                        filelst.write("%04X" %(int(oper)))
                        EQU.adr = ("%X" % (int(oper)))
                        filelst.write("  " + lststr[asminf].prt + "\n")
                    EQU.type_in_table = "NUMBER"
                else:
                    EQU.adr = oper[:]
                    EQU.type_in_table = "TEXT"

                    filelst.write("      "+lststr[asminf].prt + "\n")
                EQU.name=lststr[asminf].stroka[0]
                EQU.segm = SEGM[:]
                EQU.label = "1"
                self.allvall.append(EQU)
                num_str += 1
                continue
            if ("SETA" in lststr[asminf].stroka):
                if (lststr[asminf].s_oper != "") or (len(lststr[asminf].stroka)>2) or (not(lststr[asminf].stroka[1] in allvals)):
                    Errors += 1
                    filelst.write("ERROR : WRONG USING THIS COMAND\n")
                    filelst.write("%04X" % (self.location_counter))
                    filelst.write("  0F 97 05 00000000 U	" + lststr[asminf].prt + "\n")
                    self.location_counter += 7
                    continue
                out = self.seta_func(lststr[asminf])
                filelst.write(out)
                num_str += 1
                continue
            elif ("DAA" in lststr[asminf].stroka) :
                if(len(lststr[asminf].stroka)>1):
                    Errors += 1
                    filelst.write("(" + str(num_str) + ")"+"ERROR : SO MUCH OPERANDS FOR THIS COMAND:\n")


                num_str += 1
                filelst.write("%04X" % (self.location_counter))
                self.location_counter += 1
                filelst.write("  27                   " + lststr[asminf].prt + "\n")
                continue
            elif ("JE" in lststr[asminf].stroka) :
                filelst.write("%04X" % (self.location_counter))
                jmp=("%04X" % (self.location_counter))
                if (lststr[asminf].f_oper in wiev):
                    filelst.write(" 74               " + lststr[asminf].prt + "\n")
                    self.location_counter+=2
                    jmp += " 74  " + lststr[asminf].prt
                else:
                    filelst.write("  0F84                 " + lststr[asminf].prt + "\n")
                    self.location_counter += 6
                    jmp += " 0F84   " + lststr[asminf].prt
                self.jumps.append(self.parse(jmp,self.mysep))
                num_str += 1
                continue
            elif ("DIV" in lststr[asminf].stroka):
                if (lststr[asminf].s_oper != "") or (len(lststr[asminf].stroka)>2) or not(lststr[asminf].f_oper in self.regists):
                    Errors+=1
                    filelst.write("ERROR : WRONG USING COMAND:\n")
                    filelst.write("%04X" % (self.location_counter))
                    filelst.write("  F7 35 00000000 U	"+lststr[asminf].prt+"\n")
                    self.location_counter+=6
                    continue
                out = self.div_func (lststr[asminf])
                filelst.write(out)
                num_str += 1
                continue
            elif ("ADD" in lststr[asminf].stroka):
                if (not (lststr[asminf].s_oper in self.regists)) or (
                    len(lststr[asminf].stroka) > 4)   or (not(lststr[asminf].f_oper in allvals)):

                    if ("[" in lststr[asminf].f_oper):
                        err=""
                        for bykwa in lststr[asminf].f_oper:
                            if (bykwa=="["):
                                break
                            err += bykwa
                        if (not(err in allvals)):
                            Errors += 1
                            filelst.write("ERROR : WRONG USING THIS COMAND:\n")
                            filelst.write("%04X" % (self.location_counter))
                            filelst.write("  01 05 00000000 U	" + lststr[asminf].prt + "\n")
                            self.location_counter += 6
                            continue

                    else:
                        Errors += 1
                        filelst.write("ERROR : WRONG USING THIS COMAND:\n")
                        filelst.write("%04X" % (self.location_counter))
                        filelst.write("  01 05 00000000 U	" + lststr[asminf].prt + "\n")
                        self.location_counter += 6
                        continue
                out = self.add_func(lststr[asminf])
                filelst.write(out)
                num_str += 1
                continue
            elif ("CMP" in lststr[asminf].stroka):
                if (not(lststr[asminf].s_oper in self.regists)) or (not(lststr[asminf].f_oper in self.regists)) or (len(lststr[asminf].stroka)>4) :
                    Errors += 1
                    filelst.write("ERROR : WRONG USING THIS COMAND\n")
                    if (not(lststr[asminf].s_oper in self.regists)):
                        filelst.write("%04X" % (self.location_counter))
                        filelst.write("  3B 1D 00000000 U	" + lststr[asminf].prt + "\n")
                        self.location_counter += 6
                        continue
                    elif(not(lststr[asminf].f_oper in self.regists)):
                        filelst.write("%04X" % (self.location_counter))
                        filelst.write("  39 05 00000000 U	" + lststr[asminf].prt + "\n")
                        self.location_counter += 6
                        continue
                out = self.cmp_func(lststr[asminf])
                filelst.write(out)
                num_str += 1
                continue
            elif ("XOR" in lststr[asminf].stroka):
                if (not (lststr[asminf].f_oper in allvals)) or (len(lststr[asminf].stroka) > 4) or (
                not (lststr[asminf].s_oper in allvals)):
                    Errors += 1
                    filelst.write("ERROR : WRONG USING THIS COMAND:\n")
                    if (not (lststr[asminf].f_oper in allvals)):
                        filelst.write("%04X" % (self.location_counter))
                        filelst.write("  81 35 00000000 U 0000	" + lststr[asminf].prt + "\n")
                        self.location_counter += 10
                        continue

                out = self.xor_func(lststr[asminf])
                filelst.write(out)
                num_str += 1


                continue
            elif ("MOV" in lststr[asminf].stroka):
                if (not(lststr[asminf].f_oper in self.regists)) or (len(lststr[asminf].stroka)>4) or (not(lststr[asminf].s_oper in allvals)):
                    Errors += 1
                    filelst.write("ERROR : WRONG USING THIS COMAND:\n")
                    if (not(lststr[asminf].s_oper in allvals)):
                        filelst.write("%04X" % (self.location_counter))
                        filelst.write("  8B 35 00000000 U	" + lststr[asminf].prt + "\n")
                        self.location_counter += 6
                        continue
                    elif (not(lststr[asminf].f_oper in self.regists)):
                        filelst.write("%04X" % (self.location_counter))
                        filelst.write("  C7 05 00000000 U 0000	" + lststr[asminf].prt + "\n")
                        self.location_counter +=8
                        continue
                out = self.mov_func(lststr[asminf])
                filelst.write(out)
                num_str += 1
                continue
            elif ("AND" in lststr[asminf].stroka):
                if (not (lststr[asminf].f_oper in self.regists)) or (len(lststr[asminf].stroka) > 4) or (not(lststr[asminf].f_oper in allvals)):

                    if (not(lststr[asminf].f_oper in self.regists)):
                        Errors += 1
                        filelst.write("ERROR : WRONG USING THIS COMAND:\n")
                        filelst.write("%04X" % (self.location_counter))
                        filelst.write("  C7 05 00000000 U 0000	" + lststr[asminf].prt + "\n")
                        self.location_counter +=8
                        continue
                    elif (not(lststr[asminf].s_oper in allvals)):
                        if ("[" in lststr[asminf].s_oper):
                            err = ""
                            for bykwa in lststr[asminf].s_oper:
                                if (bykwa == "["):
                                    break
                                err += bykwa
                            if (not (err in allvals)):
                                Errors += 1
                                filelst.write("ERROR : WRONG USING THIS COMAND:\n")
                                filelst.write("%04X" % (self.location_counter))
                                filelst.write("  01 05 00000000 U	" + lststr[asminf].prt + "\n")
                                self.location_counter += 6
                                continue
                        else:
                            Errors += 1
                            filelst.write("ERROR : WRONG USING THIS COMAND:\n")
                            filelst.write("%04X" % (self.location_counter))
                            filelst.write("  23 05 00000000 U	" + lststr[asminf].prt + "\n")
                            self.location_counter += 6
                            continue

                out = self.and_func(lststr[asminf])
                filelst.write(out)
                num_str += 1
                continue
            elif ("SHORT" in lststr[asminf].stroka) :
                filelst.write("%04X" % (self.location_counter))
                filelst.write("  EB               " + lststr[asminf].prt + "\n")
                jmp = ("%04X" % (self.location_counter))
                jmp += "  EB " + lststr[asminf].prt
                self.jumps.append(self.parse(jmp, self.mysep))
                if (lststr[asminf].f_oper in wiev) or ("SHORT" in lststr[asminf].stroka):
                    self.location_counter+=2
                else:
                    self.location_counter += 6
                num_str += 1
                continue
            elif ("DD" in lststr[asminf].stroka) or ("DW" in lststr[asminf].stroka) or ("DB" in lststr[asminf].stroka):
                if (in_segment != 1):
                    Errors += 1
                    filelst.write("ERROR : THIS COMAND NOT IN SEGMENT\n")
                    exit(1)
                ADD = vallues()
                ADD.name = lststr[asminf].stroka[0]
                ADD.adr = self.location_counter
                ADD.segm = SEGM
                ADD.type = lststr[asminf].stroka[1]
                if (ADD.type == "DD"):
                    ADD.type_in_table = "DWORD"
                elif (ADD.type == "DB"):
                    ADD.type_in_table = "BYTE"
                else:
                    ADD.type_in_table = "WORD"
                self.allvall.append(ADD)
                ret = self.value_fuct(lststr[asminf], num_str, filename)
                filelst.write(str(ret))
                num_str += 1
                continue
            elif (not(lststr[asminf].stroka[0] in self.__comands)) :
                Errors+=1
                filelst.write("Error comand :\n")
            filelst.write("%04X" % (self.location_counter))
            filelst.write("  " + lststr[asminf].prt +"\n")


        print("Errors: "+str(Errors) )
        filelst.write("\n\n\n")
        filelst.write(" N a m e         	Size	Length	 Align	Combine Class\n")
        segm.sort()
        for r in segm :
            filelst.write(r+"\n")
        filelst.write("\n\n\n")
        filelst.write("Symbols:\n\n")
        filelst.write("  N a m e         	Type	         Value	     Attr\n")

        outlst = []
        lenall = len(self.allvall)
        for r in range(lenall):
            outstr = ""
            outstr +=self.allvall[r].name+"................"+self.allvall[r].type_in_table+"       "
            if (self.allvall[r].label == ""):
                outstr += ("%04X" % (int(self.allvall[r].adr)))
            elif (self.allvall[r].type_in_table == "TEXT"):
                outstr +=(self.allvall[r].adr)
            else:
                outstr +=("%04X" % (int(self.allvall[r].adr,16)))
            outstr+="         "+self.allvall[r].segm+"\n"
            outlst.append(outstr)
        outlst.sort()
        lenout = len(outlst)
        for l in range(lenout):
            filelst.write(outlst[l])
            if(l+1<lenout):
                if(outlst[l][0]!=outlst[l+1][0]):
                    filelst.write("\n\n")
        filelst.write("\n\n@FILENAME................TEXT  "+filename[:-4].upper())
        filelst.write("\nErrors:"+str(Errors))
        filelst.close()

        self.pass_sec()
        #for u in range(len (self.jumps)):
          #  self.jumps[u][:].lower()
        filelst2 = open (filename,"w", encoding='utf-8')
        filelst1 = open ("LIST.lst","r", encoding='utf-8')
        i=0
        for strout in filelst1:
            if("SHORT" in strout.upper()) or ("JE" in strout.upper()):
                filelst2.write(self.jumps[i]+"\n")
                i+=1
                continue
            filelst2.write(strout)

        filelst1.close()

        filelst2.close()



