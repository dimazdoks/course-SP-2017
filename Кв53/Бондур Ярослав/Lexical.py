class asmstr():
    def __init__(self):
        self.stroka = []
        self.Nlexem = 0
        self.Lexem = -1
        self.mnemocd=0
        self.m_lex = 0
        self.Nf_lex = 0
        self.s_oper = ""
        self.Ns_oper = 0
        self.f_oper = ""
        self.Nf_oper = 0
        self.prt=""
class Lexical():
    def __init__(self):

        self.__comands = ['JE', 'DAA', 'SETA', 'DIV', 'ADD', 'XOR', 'MOV', 'AND', 'CMP', 'JMP']
        self.__directivs = ['SEGMENT', 'ENDS', 'ASSUME', 'END']
        self.__operand = ["SHORT"]
        self.__types = ['DB', 'DW', 'DD']
        self.__segment_registr = ['CS', 'DS', 'ES', 'SS', 'FS', 'GS']
        self.__registr32 = ['EAX', 'EBX', 'ECX', 'EDX', 'ESI', 'EDI', 'ESP', 'EBP']
        self.__registr16 = ['AX', 'BX', 'CS', 'DX', 'SI', 'DI', 'SP', 'BP']
        self.__registr8 = ['AH', 'AL', 'BH', 'BL', 'CH', 'CL', 'DH', 'DL']
        self.__equ = ["EQU"]
        self.__coma = [","]
        self.__ASSUME = ["ASSUME"]
        self.__separators = [',', '[', ':', ']']
        self.__SEPARATORS1 = "^,.:+*[]\'\" "
        self.__SEP = ",.:[]\'\""
        self.__quotes = "\'\""
        self.__jmps = ["JMP","JA","JAE","JB","JBE","JC","JE",'JZ','JG',"JGE","JL",'JLE','JNA','JNB','JNBE','JNC','JNE','JNG','JNGE','JNL','JNLE',"JNO","JNP",'JNS',"JNZ","JO","JP","JPE","JPO","JS",'JZ']
        self.__vals =[]
        self.__users=""
        self.user_segm = []
        self.lststr = []
    def parse(self,s,SEPARATORS1):
        s = s[0:] + "^"
        result = []
        current_word = ''
        for char in s:
            if char == ";" :
                break
            if char in SEPARATORS1:
                if current_word: result.append(current_word.upper())
                if char in self.__quotes:
                    current_word = s[s.find("'", 0, len(s)):-1]
                    result.append(current_word)
                    break
                if char in self.__SEP: result.append(char)
                current_word = ''
            else:
                current_word += char
        return result
    def firstpass(self, line, parsestr, firstlex, numlex, secLex, numsec):
        res = ["False", "---", "---"]
        self.printtabl(res,0,line)
        operand2 = 0
        x=[]
        boolean = "False"
        add = proterties = ""
        if(secLex==0):
            operand1=parsestr[firstlex-1:]

        elif (numlex == 1):
            operand1 = parsestr[firstlex-1]
            operand2=parsestr[secLex-1:]
        else:
            operand1 = parsestr[firstlex-1:secLex-2]
            operand2 = parsestr[secLex - 1:]
        res = self.punkt1(operand1,numlex)
        self.printtabl(res, 1, line)
        res.clear()
        res = self.punkt2(operand1,numlex)
        self.printtabl(res, 2, line)
        res.clear()
        res = self.punkt3(operand1,numlex)
        self.printtabl(res, 3, line)
        res.clear()
        res = self.punkt4(parsestr,numlex)
        self.printtabl(res, 4, line)
        res.clear()
        res = self.punkt5(operand1, numlex)
        self.printtabl(res, 5, line)
        res.clear()
        res = self.punkt6(operand1,numlex)
        self.printtabl(res[0], 6, line)
        self.printtabl(res[1], 7, line)
        res.clear()
        res = self.punkt8(operand1, numlex)
        self.printtabl(res, 8, line)
        res.clear()
        res = self.punkt9(operand1, numlex)
        self.printtabl(res, 9, line)
        res.clear()
        res = self.punkt10(operand1, numlex)
        self.printtabl(res, 10, line)
        res.clear()

        if (operand2!=0):
            file = open("first.doc", "a")
            file.write(line + "\n")
            file.write("Table for second operand:\n")
            file.write("-" * 62 + "\n")
            file.write("|  №  |   Operands  |     properties     |   add propertirs  |\n")
            file.write("-" * 62 + "\n")
            file.close()

            res = self.punkt1(operand2, numsec)
            self.printtabl(res, 1, line)
            res.clear()
            res = self.punkt2(operand2, numsec)
            self.printtabl(res, 2, line)
            res.clear()
            res = self.punkt3(operand2, numsec)
            self.printtabl(res, 3, line)
            res.clear()
            res = self.punkt5(operand2, numsec)
            self.printtabl(res, 5, line)
            res.clear()
            res = self.punkt6(operand2, numsec)
            self.printtabl(res[0], 6, line)
            self.printtabl(res[1], 7, line)
            res.clear()
            res = self.punkt8(operand2, numsec)
            self.printtabl(res, 8, line)
            res.clear()
            res = self.punkt9(operand2, numsec)
            self.printtabl(res, 9, line)
            res.clear()
            res = self.punkt10(operand2,numsec)
            self.printtabl(res, 10, line)
            res.clear()
    def table (self,filename):
        FileAsm = open(filename, 'r', encoding='utf-8')
        word = tmp = lexem = string = ""
        x = tmplist = []
        mysep = " ,^\"\'"
        mnemocod = 0
        itt = 0
        lines = [string.strip() for string in FileAsm]
        FileAsm.close()
        EQUvoc = {}
        LexTable = open('table.doc', 'w')
        for string in lines:

            if len(string) == 0 or string[0]== ".":
                continue
            if (string[0]!=';'):
                LexTable.write(string + "\n")
            ParseString = self.parse(string,self.__SEPARATORS1)
            self.lststr.append(asmstr())
            self.lststr[itt].stroka=self.parse(string,mysep)
            self.lststr[itt].prt=string
            LenParseStr = len(ParseString)
            if (len(ParseString)==0):
                continue
            if self.__equ[0] in ParseString:
                if LenParseStr > 3:
                    value = ''.join(ParseString[2:])
                else:
                    value = ParseString[-1]
                const = ParseString[0]
                tmplist.append(const)
                EQUvoc[const] = value
            colFirst = colSecond = numFirst = numSecond = numLex = mnemocod = lextmp = 0
            comas = []
            i = k = 1
            lexem = ""
            LexTable.write("_" * 111 + "\n")
            LexTable.write("|  №/№  | Идентификатор |Размер| Тип                                                                                 |\n")
            for t in range (LenParseStr) :
                if (ParseString[t] in tmplist) and (ParseString[1] != "EQU"):
                    value = ParseString[t]
                    prs = self.parse(EQUvoc[value],self.__SEPARATORS1)
                    if (len(prs) > 1):
                        ParseString.pop(t)
                        for o in range(len(prs)):
                            ParseString.append(prs[o])
                    else:
                        ParseString[t] = EQUvoc[value]
            LenParseStr = len(ParseString)
            for x in range(LenParseStr):
                tmp = ParseString[x]
                if ParseString[x] in self.__comands:
                    my_type = "Команда"
                    mnemocod += 1
                    if mnemocod < 2: numLex = i
                elif ParseString[x] in self.__types:
                    mnemocod += 1
                    if mnemocod < 2:
                        numLex = i
                        # numFirst = i +1
                    if ParseString[x] == "DB":
                        my_type = "Директива типа данных 1"
                    elif ParseString[x] == "DW":
                        my_type = "Директива типа данных 2"
                    else:
                        my_type = "Директива типа данных 3"
                elif ParseString[x] in self.__directivs:
                    my_type = "Директива"
                    mnemocod += 1
                    if mnemocod < 2:
                        numLex = i
                elif ParseString[x] in self.__operand:
                    my_type = "Операнд"
                    if mnemocod < 2:
                        numLex = i
                elif ParseString[x] in self.__equ:
                    mnemocod += 1
                    if mnemocod < 2: numLex = i
                    my_type = "Директива"
                elif ParseString[x] in self.__separators:
                    my_type = "Односимвольная"
                elif ParseString[x] in self.__segment_registr:
                    my_type = "Идентификатор сегментного регистра"
                elif ParseString[x] in self.__registr32:
                    my_type = "32 - разрядный регистр"
                elif ParseString[x] in self.__registr16:
                    my_type = "16 - разрядный регистр"
                elif ParseString[x] in self.__registr8:
                    my_type = "8 разрядный регистр"
                elif (tmp[-1] == 'B') and (tmp[0] == "1") or (tmp[0] == "0"):
                    my_type = "Двоичная константа"
                elif (tmp[-1] == 'Q') or (tmp[-1] == 'O') and (tmp[0:-1].isdigit()):
                    my_type = "Восьмеричная константа"
                elif (tmp[-1] == 'H') and (tmp[0].isdigit()):
                    my_type = "Шестнадцетиричная константа"
                elif (tmp[-1] == "D") and (tmp[0:-1].isdigit()) or (tmp.isdigit()):
                    my_type = "Десятичная константа"
                elif tmp[-1] == "'" or (tmp[-1] == '"'):
                    ParseString[x] = tmp[1:-1]
                    my_type = "Текстовая константа"
                else:
                    my_type = "Идентификатор пользователя или не определен"
                    if(ParseString[1]=="SEGMENT"):
                        if (LenParseStr==2):
                            self.user_segm = [ParseString[0],"32bit"]
                        else:
                            if(ParseString[3]=="USE16"):
                                self.user_segm = [ParseString[0],"16bit"]
                        #self.print_seg(self.user_segm)
                    if not(ParseString in self.__jmps):
                        self.__vals.append(ParseString[x])
                    if lextmp == 0:
                        lexem = '1'
                if (numLex > 0) and (numLex < i):
                    numFirst = numLex + 1
                if (numFirst) and (mnemocod > 1): numFirst += 1
                if (ParseString[x] == ","):
                    colFirst = i - numFirst
                    numSecond = i + 1
                    comas.append(lextmp)
                    if (LenParseStr - numSecond == 0):
                        colSecond = 1
                    else:
                        colSecond = LenParseStr - i
                if (numSecond == 0):
                    colFirst = LenParseStr - numFirst + 1
                    if (colFirst == 0): colFirst = 1
                lextmp += 1
                LexTable.write("|   %-4s" % (i))
                LexTable.write("|%-15s" % (ParseString[x]))
                LexTable.write("|   %-3s" % (str(len(ParseString[x]))))
                LexTable.write("|%-85s" % (my_type))
                LexTable.write("|\n")
                i += 1
            colFirst1 = str(colFirst)
            numSecond1 = str(numSecond)
            colSecond1 = str(colSecond)
            numFirst1 = str(numFirst)
            numLex1 = str(numLex)
            mnemocod1 = str(mnemocod)
            if self.__comands[2] in ParseString:
                numFirst = 0
                colFirst = 0
            if lexem != "1": lexem = "---"
            if mnemocod < 1:  mnemocod1 = "---"
            if numLex == 0: numLex1 = "---"
            if numFirst == 0:
                colFirst = 0
                colFirst1 = "---"
                numFirst1 = "---"
            if colFirst == 0: colFirst1 = "---"
            if numSecond == 0: numSecond1 = "---"
            if colSecond == 0: colSecond1 = "---"
            if (ParseString[0] == "ASSUME") and (len(comas) > 1):
                LexTable.write("*" * 104 + "\n")
                LexTable.write("^" * 141 + "\n")
                LexTable.write("|Label (Name)|         Mnemokod               |")
                while k <= (len(comas) + 1):
                    LexTable.write("Operand №%-22s" % (str(k)))
                    LexTable.write("|")
                    k += 1
                    obj = []
                    colFirst = comas[0] - numFirst + 1
                LexTable.write("\n")
                LexTable.write("|№/№  Lexem  | ")
                LexTable.write("№/№ first Lexem | Number Lexem |" * k + "\n")
                LexTable.write("|%-12s" % (lexem))
                LexTable.write("|%-17s" % (numLex1))
                LexTable.write("|%-14s" % (mnemocod1))
                LexTable.write("|%-17s" % (numFirst1))
                LexTable.write("|%-13s" % (str(colFirst)))

                k = 1
                i = i - 2
                for obj in range(len(comas)):
                    comas[obj] += 1
                    LexTable.write("|%-17s" % (comas[obj] + 1))
                    if len(comas) > k:
                        LexTable.write("|%-13s" % str(comas[obj + 1] - comas[obj]))
                        k += 1
                    else:
                        LexTable.write("|%-13s|" % str(i - comas[obj] + 1))
                LexTable.write("\n\n")
                itt+=1
                continue
            LexTable.write("*" * 111 + "\n")
            LexTable.write("^" * 111 + "\n")
            LexTable.write(
                "|Label (Name)|         Mnemokod               |         First Operand          |         Second Operand       |\n")
            LexTable.write(
                "|№/№ Lexem   | №/№ first Lexem | Number Lexem | №/№ first Lexem | Number Lexem | №/№ first Lexem |Number Lexem|\n")
            LexTable.write("|%-12s" % (lexem))
            LexTable.write("|%-17s" % (numLex1))
            LexTable.write("|%-14s" % (mnemocod1))
            LexTable.write("|%-17s" % (numFirst1))
            LexTable.write("|%-14s" % (colFirst1))
            LexTable.write("|%-17s" % (numSecond1))
            LexTable.write("|%-12s|\n\n" % (colSecond1))
            self.lststr[itt].Nf_oper=numFirst-1
            self.lststr[itt].Ns_oper=numSecond-1
            self.lststr[itt].mnemocd=numLex-1
            if(lexem!="---"):
                self.lststr[itt].Lexem = int(lexem)
            if (numFirst!=0):
                self.lststr[itt].f_oper = self.lststr[itt].stroka[numFirst-1]
            if (numSecond!=0):
                self.lststr[itt].s_oper = self.lststr[itt].stroka[-1]
            itt+=1
            if (numFirst!=0 and not(self.__ASSUME in ParseString)):
                self.firstpass(string,ParseString,numFirst,colFirst,numSecond,colSecond)
        LexTable.close()
    def punkt1(self,operand,numlex):
        result = ["False","---","---"]
        __registr16_ = ['AX','CS','DX','SP']
        __registr32_ = ["EAX","ESP"]
        if(numlex!=1):
            for x in range(len(operand)):
                if operand[x] in self.__registr8 : result[0]="True"
                elif operand[x] in __registr16_ : result[0]="True"
                elif operand[x] in __registr32_ : result[0]="True"
                if(result[0]=="True"):
                    if (operand[x] in self.__registr8):
                        result[1]="8"
                        if("AL" in operand) : result[2]="000"
                        elif("CL" in operand) : result[2]="001"
                        elif ("DL" in operand):  result[2] = "010"
                        elif ("BL" in operand):  result[2] = "011"
                        elif ("AH" in operand): result[2] = "100"
                        elif ("CH" in operand): result[2] = "101"
                        elif ("DH" in operand): result[2] = "110"
                        elif ("BH" in operand): result[2] = "111"
                    elif(operand[x] in __registr16_):
                        result[1]="16"
                        if ("AX" in operand):  result[2] = "000"
                        elif ("CX" in operand): result[2] = "001"
                        elif ("DX" in operand): result[2] = "010"
                        elif ("SP" in operand): result[2] = "100"
                    elif (operand[x] in __registr32_):
                        result[1] = "32"
                        if ("EAX" in operand): result[2] = "000"
                        elif ("ESP" in operand):  result[2] = "100"

                    return result
        else:
            oper = []
            if (len(operand) > 1):
                oper.append(operand)
            else:
                oper = operand
            if oper[0] in self.__registr8:
                result[0] = "True"
            elif oper[0] in __registr16_:
                result[0] = "True"
            elif oper[0] in __registr32_:
                result[0] = "True"
            if (result[0] == "True"):
                if (oper[0] in self.__registr8):
                    result[1] = "8"
                    if ("AL" in oper):
                        result[2] = "000"
                    elif ("CL" in oper):
                        result[2] = "001"
                    elif ("DL" in oper):
                        result[2] = "010"
                    elif ("BL" in oper):
                        result[2] = "011"
                    elif ("AH" in oper):
                        result[2] = "100"
                    elif ("CH" in oper):
                        result[2] = "101"
                    elif ("DH" in oper):
                        result[2] = "110"
                    elif ("BH" in oper):
                        result[2] = "111"
                elif (oper[0] in __registr16_):
                    result[1] = "16"
                    if ("AX" in oper):
                        result[2] = "000"
                    elif ("CX" in oper):
                        result[2] = "001"
                    elif ("DX" in oper):
                        result[2] = "010"
                    elif ("SP" in oper):
                        result[2] = "100"
                elif (oper[0] in __registr32_):
                    result[1] = "32"
                    if ("EAX" in oper):
                        result[2] = "000"
                    elif ("ESP" in oper):
                        result[2] = "100"
                return result
        return result
    def punkt2(self,operand,numlex):
         result = ["False","---","---"]
         if (numlex==1):
             return  result

         for x in range (len(operand)):
            if operand[x] == "ptr":
                result[0]="True"
                result[1]=operand[x-1]
                return result
         return result
    def punkt3(self,operand,numlex):
        result = ["False", "---", "---"]
        if (numlex != 1):
            for x in range(len(operand)):
                if operand[x] in self.__segment_registr:
                    result[0] = "True"
                    if ("CS" in operand):
                        result[1] = "2eh"
                    elif ("SS" in operand):
                         result[1] = "36h"
                    elif ("DS" in operand):
                         result[1] = "3eh"
                    elif ("ES" in operand):
                        result[1] = "26h"
                    elif ("FS" in operand):
                        result[1] = "64h"
                    elif ("GS" in operand):
                         result[1] = "65h"
                    return result
        else:
            if operand in self.__segment_registr:
                result[0] = "True"
                if ("CS" in operand):
                    result[1] = "2eh"
                elif ("SS" in operand):
                    result[1] = "36h"
                elif ("DS" in operand):
                    result[1] = "3eh"
                elif ("ES" in operand):
                    result[1] = "26h"
                elif ("FS" in operand):
                    result[1] = "64h"
                elif ("GS" in operand):
                    result[1] = "65h"
                return result
        return result
    def punkt4(self, parsestr, numlex):
        result = ["False", "---", "---"]
        jmps = ["JMP","JE","JA","JAE","JB","JBE","JC","JE",'JZ','JG',"JGE","JL",'JLE','JNA','JNB','JNBE','JNC','JNE','JNG','JNGE','JNL','JNLE',"JNO","JNP",'JNS',"JNZ","JO","JP","JPE","JPO","JS",'JZ']
        if (numlex == 1):
            if(parsestr[0] in jmps):
                result[0]="True"
                if(len(parsestr)==3) and (parsestr[1]== "SHORT"):
                    result[1]=parsestr[2]
                else: result[1]=parsestr[1]
        return result
    def punkt5(self,operand,numlex):
        result = ["False", "---", "---"]
        if (numlex!=1):
            for x in range (len(operand)) :
                if(operand[x] in self.__vals):
                    result[0]="True"
        else:
            if (operand in self.__vals):
                result[0] = "True"
        return result;
    def punkt6(self,operand,numlex):

        result1 = ["False", "---", "---"]
        result2 = ["False", "---", "---"]
        reg16 = ["BX",'BP',"SI","DI"]
        reg32 = ["EBX","EBP","ESI","EDI"]
        if (numlex!=1):
            for x in range (len(operand)):
                if (operand[x] in reg16) or (operand[x] in reg32):
                    if (result1[0]=="True"):
                        result2[0]=="True"
                    result1[0] = "True"
                    if (operand[x] in reg16):
                        if (operand[x] == "BX"):
                            if (result1[0] == "True") and (result1[1]!="---"):
                                result2[1] = "011"
                                break
                            result1[1] = "011"
                        elif (operand[x] == "BP"):
                            if (result1[0] == "True") and (result1[1]!="---"):
                                result2[1] = "101"
                                break
                            result1[1] = "101"
                        elif (operand[x] == "SI"):
                            if (result1[0] == "True") and (result1[1]!="---"):
                                result2[1] = "110"
                                break
                            result1[1] = "110"
                        elif (operand[x] == "DI"):
                            if (result1[0] == "True") and (result1[1]!="---"):
                                result2[1] = "111"
                                break
                            result1[1] = "111"
                    else:
                        if (operand[x] == "EBX"):
                            if (result1[0] == "True") and (result1[1]!="---"):
                                result2[1] = "011"
                                break
                            result1[1] = "011"
                        elif (operand[x] == "EBP"):
                            if (result1[0] == "True") and (result1[1]!="---"):
                                result2[1] = "101"
                                break
                            result1[1] = "101"
                        elif (operand[x] == "ESI"):
                            if (result1[0] == "True") and (result1[1]!="---"):
                                result2[1] = "110"
                                break
                            result1[1] = "110"
                        elif (operand[x] == "EDI"):
                            if (result1[0] == "True") and (result1[1]!="---"):
                                result2[1] = "111"
                                break
                            result1[1] = "111"
        else:
            oper=[]
            if (len(operand)>1):
                oper.append(operand)
            else:
                oper = operand
            if(oper[0] in reg16) or (oper[0] in reg32):
                result1[0] = "True"
                if(oper[0] in reg16):
                    if (oper[0] == "BX") : result1[1] = "011"
                    elif (oper[0]== "BP") : result1[1]="101"
                    elif (oper[0] == "SI"):  result1[1] = "110"
                    elif (oper[0] == "DI"):    result1[1] = "111"
                else:
                    if (oper[0] == "EBX") : result1[1] = "011"
                    elif (oper[0]== "EBP") : result1[1]="101"
                    elif (oper[0] == "ESI"):  result1[1] = "110"
                    elif (oper[0] == "EDI"):    result1[1] = "111"
        res = [result1, result2]
        return res
    def punkt8(self,operand,numlex):
        result1 = ["False", "---", "---"]
        reg = ["BX", 'BP', "SI", "DI","EBX", "EBP", "ESI", "EDI"]
        if(numlex!=1):
            for x in range (len(operand)):
                if(operand[x] == "*") and (operand[x-1] in reg) and not(x+2>numlex):
                    result1[0]="True"
                    result1[1]=operand[x+1]
                    return result1
        return result1
    def punkt9(self,operand,numlex):
        result = ["False", "---", "---"]
        lst = ["offset","seg"]
        if (numlex != 1):
            for x in range(len(operand)):
                if (operand[x] in lst):
                    result[0] = "True"
                    return result
        else:
            if (operand in lst):
                result[0] = "True"
                return result
        return  result
    def punkt10(self,operand,numlex):
        result1 = ["False", "---", "---"]
        if(numlex!=1):
            for x in range (len(operand)):
                tmp = operand[x]
                if (tmp[-1]=="H") and (tmp[0].isdigit()):
                    result1[0]="True"
                    result1[1]=tmp
                    return result1
                elif (tmp[-1]=="Q") or (tmp[-1]=="O") and not(tmp in self.__vals) and (tmp[0:-1].isdigit()):
                    result1[0] = "True"
                    result1[1] = tmp
                    return result1
                elif (tmp[-1]=="B") and not(tmp in self.__vals) and (tmp[0:-1].isdigit()) :
                    result1[0] = "True"
                    result1[1] = tmp
                    return result1
                elif (tmp[-1]=="D") and not(tmp in self.__vals) and (tmp[0:-1].isdigit()) or (tmp.isdigit()):
                    result1[0] = "True"
                    result1[1] = tmp
                    return result1
        else:
            tmp = operand[0]
            if (tmp[-1]=="H") and (tmp[0].isdigit()):
                result1[0]="True"
                result1[1]=tmp
            elif (tmp[-1]=="Q") or (tmp[-1]=="O") and not(tmp in self.__vals) and (tmp[0:-1].isdigit()):
                result1[0] = "True"
                result1[1] = tmp
            elif (tmp[-1]=="B") and not(tmp in self.__vals) and (tmp[0:-1].isdigit()) :
                result1[0] = "True"
                result1[1] = tmp
            elif (tmp[-1]=="D") and not(tmp in self.__vals) and (tmp[0:-1].isdigit()) or (tmp.isdigit()):
                result1[0] = "True"
                result1[1] = tmp
        return result1
    def printtabl(self,res,i,line):
        file = open("first.doc", "a")
        if (i==0):
            file.write(line + "\n")
            file.write("Table for first operand:\n")
            file.write("-" * 62 + "\n")
            file.write("|  №  |   Operands  |     properties     |   add propertirs  |\n")
            file.write("-" * 62 + "\n")
        else:
            file.write("|%-3s"%(i))
            file.write("  |")
            file.write("%-13s" % (str(res[0])))
            file.write("|%-20s" % (str(res[1])))
            file.write("|%-18s " % (str(res[2])))
            file.write("|\n\n")
        file.close()
