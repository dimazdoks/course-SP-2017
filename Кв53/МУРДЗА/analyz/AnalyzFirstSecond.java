package oksana.analyz;

import java.nio.ByteBuffer;
import java.util.Arrays;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Stack;
import java.util.stream.Collectors;
import oksana.analyz.AnalyzSyntx;
import oksana.units.StrGen;
import oksana.units.StrGenType;
import oksana.units.StrSyntx;
import oksana.units.SyntxUnit;
import oksana.units.SyntxUnitType;

public class AnalyzFirstSecond {

    private final static int ADDRESS_MODE = 32;

    private static class MemInfo {

        private final String value;
        private final String id;
        private final RegInfo reg1;
        private final RegInfo reg2;
        private final SegRegInfo segReg;
        private int size;

        public MemInfo(String value, String id, RegInfo register1, RegInfo register2, SegRegInfo segReg, int size) {
            this.value = value;
            this.id = id;
            this.segReg = segReg;
            this.reg1 = register1;
            this.reg2 = register2;
            this.size = size;
        }

        public static MemInfo parseMem(StrSyntx ss) {

            String value = ss.getLexicalString().getTextString();
            String id = null;
            RegInfo reg1 = null;
            RegInfo reg2 = null;
            SegRegInfo segReg = null;
            int size = -1;

            for (SyntxUnit st : ss.getSyntaxTokens()) {
                switch (st.getType()) {
                    case TYPE:
                        size = st.getLexToken().getValue().toUpperCase().equals("BYTE") ? 1 : 4;
                        break;
                    case ID: {
                        id = st.getLexToken().getValue().toUpperCase();
                        break;
                    }
                    case REG:
                        if (reg1 == null) {
                            reg1 = RegInfo.valueOf(st.getLexToken().getValue().toUpperCase());
                        } else {
                            reg2 = RegInfo.valueOf(st.getLexToken().getValue().toUpperCase());
                        }
                        break;
                    case SEG:
                        segReg = SegRegInfo.valueOf(st.getLexToken().getValue().toUpperCase());
                        break;
                }
            }

            return new MemInfo(value, id, reg1, reg2, segReg, size);
        }

        public String getValue() {
            return value;
        }

        public String getId() {
            return id;
        }

        public RegInfo getReg1() {
            return reg1;
        }

        public RegInfo getReg2() {
            return reg2;
        }

        public SegRegInfo getSegReg() {
            return segReg;
        }

        public int getSize() {
            return size;
        }

        public void setSize(int size) {
            this.size = size;
        }

    }

    private static enum SegRegInfo {
        DS("3E| "),
        CS("2E| "),
        ES("26| "),
        FS("64| "),
        GS("65| "),
        SS("36| ");

        private final String prefix;

        private SegRegInfo(String prefix) {
            this.prefix = prefix;
        }

        public String getPrefix() {
            return prefix;
        }
    }

    private static enum RegInfo {
        AL(8, 0),
        CL(8, 1),
        DL(8, 2),
        BL(8, 3),
        AH(8, 4),
        CH(8, 5),
        DH(8, 6),
        BH(8, 7),
        EAX(32, 0),
        ECX(32, 1),
        EDX(32, 2),
        EBX(32, 3),
        ESP(32, 4),
        EBP(32, 5),
        ESI(32, 6),
        EDI(32, 7);

        private final int size;
        private final int number;

        private RegInfo(int size, int number) {
            this.size = size;
            this.number = number;
        }

        public int getSize() {
            return size;
        }

        public int getNumber() {
            return number;
        }
    }

    private static class ConstInfo {

        private final String value;
        private final Byte[] bytes;
        private final int intValue;
        private final int size;

        public ConstInfo(String value, Byte[] bytes, int intValue, int size) {
            this.value = value;
            this.bytes = bytes;
            this.intValue = intValue;
            this.size = size;
        }

        public static ConstInfo createDummy(String value) {
            return new ConstInfo(value, null, 0, 0);
        }

        public static ConstInfo parseConst(String value) {

            int size, intValue;
            byte[] bytes;

            if (!isString(value)) {
                long longValue = parseLong(value);
                size = getSize(longValue);
                intValue = (int) longValue;
                switch (size) {
                    case 1:
                        bytes = new byte[]{(byte) longValue};
                        break;
                    case 2:
                        bytes = ByteBuffer.allocate(size).putShort((short) longValue).array();
                        break;
                    case 4:
                        bytes = ByteBuffer.allocate(size).putInt((int) longValue).array();
                        break;
                    default:
                        bytes = ByteBuffer.allocate(size).putLong(longValue).array();
                        break;
                }
            } else {
                bytes = value.substring(1, value.length() - 1).getBytes();
                ByteBuffer byteBuff = ByteBuffer.allocate(4).put(bytes, 0, bytes.length >= 4 ? 4 : bytes.length);
                byteBuff.flip();
                byteBuff.limit(4);
                intValue = byteBuff.getInt();
                size = bytes.length;
            }

            Byte[] byteObjs = new Byte[bytes.length];
            for (int i = 0; i < bytes.length; i++) {
                byteObjs[i] = bytes[i];
            }

            return new ConstInfo(value, byteObjs, intValue, size);
        }

        private static boolean isString(String value) {
            return value.charAt(0) == '\'' || value.charAt(0) == '"';
        }

        private static long parseLong(String value) {
            switch (value.charAt(value.length() - 1)) {
                case 'b':
                case 'B':
                    return Long.parseLong(value.substring(0, value.length() - 1), 2);
                case 'h':
                case 'H':
                    return Long.parseLong(value.substring(0, value.length() - 1), 16);
                default:
                    return Long.parseLong(value);
            }
        }

        private static int getSize(long value) {
            if (value > 0 && value <= 0xFF || value >= Byte.MIN_VALUE && value <= Byte.MAX_VALUE) {
                return 1;
            }
            if (value > 0 && value <= 0xFF_FF || value >= Short.MIN_VALUE && value <= Short.MAX_VALUE) {
                return 2;
            }
            return (value > 0 && value <= 0xFF_FF_FF_FFl) || (value >= Integer.MIN_VALUE && value <= Integer.MAX_VALUE) ? 4 : 8;
        }

        public boolean isString() {
            return isString(value);
        }

        public Byte[] getBytes() {
            return bytes;
        }

        public int getIntValue() {
            return intValue;
        }

        public int getSize() {
            return size;
        }

        public String getValue() {
            return value;
        }

        public String getBytesHex(int size, String joinDelimiter) {

            if (isString()) {
                List<Byte> byteList = Arrays.asList(bytes);
                List<String> byteHexList = byteList.stream().map(b -> String.format("%02X", b)).collect(Collectors.toList());

                if (byteHexList.size() < size) {
                    byteHexList.add(0, "00");
                }

                return String.join(joinDelimiter, byteHexList);
            }

            int val = intValue;
            if (size == 1) {
                val = val & 0xFF;
            } else if (size == 2) {
                val = val & 0xFF_FF;
            }

            return String.format("%0" + (size * 2) + "X", val);
        }

    }

    private static class IdInfo {

        static enum IdType {
            Variable,
            Label,
            Ravno;
        }

        private final String name;
        private final ConstInfo constInfo;
        private final IdType type;
        private final SegInfo segment;
        private final int address;
        private final int size;

        public IdInfo(String id, ConstInfo constInfo, IdType type, SegInfo segment, int address, int size) {
            this.name = id;
            this.constInfo = constInfo;
            this.type = type;
            this.address = address;
            this.size = size;
            this.segment = segment;
        }

        public static IdInfo createLabel(String id, SegInfo segment, int address) {
            return new IdInfo(id, null, IdType.Label, segment, address, 0);
        }

        public static IdInfo createVar(String id, ConstInfo constInfo, SegInfo segment, int address, int size) {
            return new IdInfo(id, constInfo, IdType.Variable, segment, address, size);
        }

        public static IdInfo createRavno(String id, ConstInfo constInfo) {
            return new IdInfo(id, constInfo, IdType.Ravno, null, 0, constInfo.getSize());
        }

        public String getName() {
            return name;
        }

        public ConstInfo getConstInfo() {
            return constInfo;
        }

        public String getValue() {
            return constInfo.getValue();
        }

        public IdType getType() {
            return type;
        }

        public String getTypeName() {
            switch (type) {
                case Ravno:
                    return "EQU";
                case Label:
                    return "LABEL";
                default:
                    if (constInfo.isString() || size == 1) {
                        return "DB";
                    }
                    if (size == 2) {
                        return "DW";
                    }
                    return "DD";
            }
        }

        public SegInfo getSegment() {
            return segment;
        }

        public int getAddress() {
            return address;
        }

        public int getSize() {
            return size;
        }

    }

    private static class SegInfo {

        private final String name;
        private SegRegInfo segReg;
        private int currentAddress;

        public SegInfo(String name) {
            this.name = name;
        }

        public String getName() {
            return name;
        }

        public void setSegReg(SegRegInfo segReg) {
            this.segReg = segReg;
        }

        public SegRegInfo getSegReg() {
            return segReg;
        }

        public int getCurrentAndIncrement(int step) {
            int cur = getCurrentAddress();
            currentAddress += step;
            return cur;
        }

        public void setCurrentAddress(int addr) {
            currentAddress = addr;
        }

        public int getCurrentAddress() {
            return currentAddress;
        }

        public int getSize() {
            return currentAddress;
        }

    }

    private static class IfElseInfo {

        private final String condition;
        private final boolean ifBody;
        private boolean insideElse;

        public IfElseInfo(String condition) {
            this.condition = condition;
            this.ifBody = ConstInfo.parseLong(condition) != 0;
            this.insideElse = false;
        }

        public String getCondition() {
            return condition;
        }

        public boolean isIfBody() {
            return ifBody;
        }

        public boolean isInsideElse() {
            return insideElse;
        }

        public void setInsideElse(boolean insideElse) {
            this.insideElse = insideElse;
        }

    }
    
    public AnalyzFirstSecond(List<StrSyntx> syntaxStrings) {
        this.syntaxStrings = syntaxStrings;
    }

    private final Map<String, IdInfo> idTable = new LinkedHashMap<>();
    private final Map<String, SegInfo> segTable = new LinkedHashMap<>();
    private final List<StrGen> generetedStrings = new LinkedList<>();
    private final Stack<IfElseInfo> ifBlocks = new Stack<>();
    
    private final List<StrSyntx> syntaxStrings;
    private SegInfo currentSegment;
    private boolean secondPass = false;

    private String formatAddress(int address) {
        return String.format("%0" + (ADDRESS_MODE / 4) + "X", address);
    }

    private String getMemError(MemInfo memInfo) {
        if (memInfo.getSize() == -1) {
            return "type cast not found";
        }
        if (memInfo.getReg1().getSize() != 32 || memInfo.getReg2().getSize() != 32) {
            return "only 32-bit registers allowed";
        }
        return null;
    }

    private StrGen parseIfBegin(StrSyntx ss) {

        IfElseInfo ifInfo;

        String condition = ss.getLexemes()[1].getValue();

        if (ss.getSyntaxTokens()[1].getType() == SyntxUnitType.ID) {
            IdInfo idInfo = idTable.get(condition.toUpperCase());
            if (idInfo == null) {
                return StrGen.createError(ss, "ID not defined (forward definition not allowed)");
            }
            if (idInfo.getType() != IdInfo.IdType.Ravno) {
                return StrGen.createError(ss, "Invalid ID");
            }

            ifInfo = new IfElseInfo(idInfo.getConstInfo().getValue());
        } else {
            ifInfo = new IfElseInfo(condition);
        }

        ifBlocks.push(ifInfo);

        return StrGen.createIfBegin(ss, ifInfo.getCondition(), ifInfo.isIfBody());
    }

    private StrGen parseIfEnd(StrSyntx ss) {

        if (ifBlocks.isEmpty()) {
            return StrGen.createError(ss, "can't find matching IF");
        }

        IfElseInfo ifInfo = ifBlocks.pop();

        return StrGen.createIfEnd(ss, ifInfo.getCondition());
    }

    private StrGen parseIfElse(StrSyntx ss) {

        if (ifBlocks.isEmpty()) {
            return StrGen.createError(ss, "can't find matching IF");
        }

        IfElseInfo ifInfo = ifBlocks.peek();
        ifInfo.setInsideElse(true);

        return StrGen.createIfElse(ss, ifInfo.getCondition(), ifInfo.isIfBody());
    }
    
    private StrGen parseSegmentBegin(StrSyntx ss) {

        if (currentSegment != null) {
            return StrGen.createError(ss, "inside of segment");
        }

        String id = ss.getLexemes()[0].getValue().toUpperCase();

        if (segTable.containsKey(id)) {
            return StrGen.createError(ss, "segment already defined");
        }

        if (segTable.size() == 6) {
            return StrGen.createError(ss, "too many segments");
        }

        currentSegment = new SegInfo(id);
        currentSegment.setSegReg(SegRegInfo.values()[segTable.size()]);

        segTable.put(id, currentSegment);

        return StrGen.createSegBegin(ss);
    }

    private StrGen parseSegmentEnd(StrSyntx ss) {

        if (currentSegment == null) {
            return StrGen.createError(ss, "outside of segment");
        }

        String id = ss.getLexemes()[0].getValue().toUpperCase();

        if (!id.equals(currentSegment.getName())) {
            return StrGen.createError(ss, "wrong segment ID");
        }

        int size = currentSegment.getSize();
        currentSegment = null;

        return StrGen.createSegEnd(ss, size);
    }

    private StrGen parseLabel(StrSyntx ss) {

        if (currentSegment == null) {
            return StrGen.createError(ss, "outside of segment");
        }

        String id = ss.getLexemes()[0].getValue().toUpperCase();

        if (idTable.containsKey(id) && !secondPass) {
            return StrGen.createError(ss, "ID already defined");
        }

        IdInfo newLabel = IdInfo.createLabel(id, currentSegment, currentSegment.getCurrentAddress());
        if (!secondPass) {
            idTable.put(id, newLabel);
        }
        return StrGen.createLabel(ss, formatAddress(newLabel.getAddress()), newLabel.getAddress());
    }

    private StrGen parseVarDef(StrSyntx ss) {

        boolean needSecondPass = false;

        if (currentSegment == null) {
            return StrGen.createError(ss, "outside of segment");
        }

        String id = ss.getLexemes()[0].getValue().toUpperCase();

        if (idTable.containsKey(id) && !secondPass) {
            return StrGen.createError(ss, "ID already defined");
        }

        SyntxUnitType type = ss.getSyntaxTokens()[1].getType();
        int sizeLimit = type == SyntxUnitType.DB ? 1 : (type == SyntxUnitType.DW ? 2 : 4);

        String value = ss.getLexemes()[2].getValue();
        ConstInfo constInfo;
        if (ss.getSyntaxTokens()[2].getType() == SyntxUnitType.ID) {
            if (!idTable.containsKey(value.toUpperCase())) {
                needSecondPass = true;
                constInfo = ConstInfo.parseConst("90h");
            } else {
                IdInfo equId = idTable.get(value.toUpperCase());
                if (equId.getType() != IdInfo.IdType.Ravno) {
                    return StrGen.createError(ss, "only EQU IDs allowed");
                }
                constInfo = equId.getConstInfo();
                // if MEM
                if (constInfo.getSize() == 0) {
                    return StrGen.createError(ss, "only CONST allowed");
                }
            }
        } else {
            constInfo = ConstInfo.parseConst(value);
        }

        if (constInfo.isString() && type != SyntxUnitType.DB) {
            return StrGen.createError(ss, "string const only for DB");
        }

        if (!constInfo.isString() && constInfo.getSize() > sizeLimit) {
            return StrGen.createError(ss, "value overflow");
        }

        int size = constInfo.isString() ? constInfo.getSize() : sizeLimit;

        if (!secondPass) {
            IdInfo newVar = IdInfo.createVar(id, constInfo, currentSegment, currentSegment.getCurrentAddress(), sizeLimit);
            idTable.put(id, newVar);
        }

        StrGen varDef = StrGen.createVariable(ss, constInfo.getBytesHex(size, " "), currentSegment.getCurrentAndIncrement(size), size);
        if (needSecondPass) {
            varDef.setType(StrGenType.NeedSecondPass);
            if (secondPass) {
                return StrGen.createError(ss, "ID not defined");
            }
        }
        return varDef;

    }

    private StrGen parseRavnoDef(StrSyntx ss) {

        String id = ss.getLexemes()[0].getValue().toUpperCase();

        if (idTable.containsKey(id) && !secondPass) {
            return StrGen.createError(ss, "ID already defined");
        }

        String value = ss.getLexemes()[2].getValue();

        IdInfo newEqu;
        if (ss.getSyntaxString().endsWith("MEM")) {
            StrSyntx memSs = AnalyzSyntx.parseString(Arrays.copyOfRange(ss.getLexemes(), 2, ss.getLexemes().length));
            newEqu = IdInfo.createRavno(id, ConstInfo.createDummy(memSs.getLexicalString().getTextString()));
        } else {
            newEqu = IdInfo.createRavno(id, ConstInfo.parseConst(value));
        }
        if (!secondPass) {
            idTable.put(id, newEqu);
        }
        return StrGen.createEqu(ss, newEqu.getConstInfo().getValue());
    }

    private StrGen parseCommand(StrSyntx ss) {

        if (currentSegment == null) {
            return StrGen.createError(ss, "outside of segment");
        }

        SyntxUnitType commandType = ss.getSyntaxTokens()[0].getType();

        String opCode = "";
        int size = 0;
        boolean needSecondPass = false;

        switch (commandType) {
            // CLC
            case CMPSD:
                opCode = "A7";
                size = 1;
                break;
            // NEG REG
            case NEG: {
                RegInfo reg = RegInfo.valueOf(ss.getLexemes()[1].getValue().toUpperCase());
                opCode = reg.getSize() == 8 ? "F6 " : "F7 ";
                opCode += String.format("%02X ", 0xD8 + reg.getNumber());
                size = 2;
                break;
            }
            // PUSH MEM
            case PUSH: {
                StrSyntx memSs = AnalyzSyntx.parseString(Arrays.copyOfRange(ss.getLexemes(), 1, ss.getLexemes().length));
                MemInfo memInfo = MemInfo.parseMem(memSs);

                if (memInfo.getId() != null) {
                    if (!idTable.containsKey(memInfo.getId())) {
                        return StrGen.createError(ss, "ID not defined");
                    }
                    IdInfo idInfo = idTable.get(memInfo.getId());
                    if (idInfo.getType() != IdInfo.IdType.Variable) {
                        return StrGen.createError(ss, "Invalid ID");
                    }
                    if (memInfo.getSize() == -1) {
                        memInfo.setSize(idInfo.getSize());
                    }
                }

                if (memInfo.getSize() != 4) {
                    return StrGen.createError(ss, "Only 32-bit operand allowed");
                }

                if (getMemError(memInfo) != null) {
                    return StrGen.createError(ss, getMemError(memInfo));
                }

                opCode = memInfo.getId() == null ? "FF 34 " : "FF B4 ";
                int base = 0x00 + memInfo.getReg1().getNumber() + 8 * memInfo.getReg2().getNumber();
                opCode += String.format("%02X ", base);
                size = 3;

                if (memInfo.getSegReg() != null && memInfo.getSegReg() != SegRegInfo.DS) {
                    opCode = memInfo.getSegReg().getPrefix() + opCode;
                    size++;
                }

                if (memInfo.getId() != null) {
                    opCode += String.format("%08X", idTable.get(memInfo.getId()).getAddress());
                    size += 4;
                }
                break;
            }
            // ADC REG , REG
            case ADC: {
                RegInfo reg1 = RegInfo.valueOf(ss.getLexemes()[1].getValue().toUpperCase());
                RegInfo reg2 = RegInfo.valueOf(ss.getLexemes()[3].getValue().toUpperCase());

                if (reg1.getSize() != reg2.getSize()) {
                    return StrGen.createError(ss, "argument types not match");
                }

                opCode = reg1.getSize() == 8 ? "12 " : "13 ";
                opCode += String.format("%02X", (0xC0 + 8 * reg1.getNumber() + reg2.getNumber()));
                size = 2;
                break;
            }
            // CMP REG , MEM
            case CMP: {
                RegInfo reg = RegInfo.valueOf(ss.getLexemes()[1].getValue().toUpperCase());
                StrSyntx memSs = AnalyzSyntx.parseString(Arrays.copyOfRange(ss.getLexemes(), 3, ss.getLexemes().length));
                MemInfo memInfo = MemInfo.parseMem(memSs);

                if (memInfo.getId() != null) {
                    if (!idTable.containsKey(memInfo.getId())) {
                        return StrGen.createError(ss, "ID not defined");
                    }
                    IdInfo idInfo = idTable.get(memInfo.getId());
                    if (idInfo.getType() != IdInfo.IdType.Variable) {
                        return StrGen.createError(ss, "Invalid ID");
                    }
                    if (memInfo.getSize() == -1) {
                        memInfo.setSize(idInfo.getSize());
                    }
                }
                
                if (memInfo.getSize() == -1) {
                    memInfo.setSize(reg.getSize() / 8);
                }

                if (getMemError(memInfo) != null) {
                    return StrGen.createError(ss, getMemError(memInfo));
                }
                
                if (reg.getSize() / 8 != memInfo.getSize()) {
                    return StrGen.createError(ss, "argument types not match");
                }

                opCode = memInfo.getSize() == 1 ? "3A " : "3B ";
                int base1 = memInfo.getId() == null ? 0x04 : 0x84;
                int base2 = 0x00 + memInfo.getReg1().getNumber() + 8 * memInfo.getReg2().getNumber();
                opCode += String.format("%02X ", base1 + 8 * reg.getNumber());
                opCode += String.format("%02X ", base2);
                size = 3;
                
                if (memInfo.getSegReg() != null && memInfo.getSegReg() != SegRegInfo.DS) {
                    opCode = memInfo.getSegReg().getPrefix() + opCode;
                    size++;
                }
                
                if (memInfo.getId() != null) {
                    opCode += String.format("%08X", idTable.get(memInfo.getId()).getAddress());
                    size += 4;
                }
                break;
            }
            // AND MEM , REG
            case AND: {
                RegInfo reg = RegInfo.valueOf(ss.getLexemes()[ss.getLexemes().length - 1].getValue().toUpperCase());
                StrSyntx memSs = AnalyzSyntx.parseString(Arrays.copyOfRange(ss.getLexemes(), 1, ss.getLexemes().length - 2));
                MemInfo memInfo = MemInfo.parseMem(memSs);

                if (memInfo.getId() != null) {
                    if (!idTable.containsKey(memInfo.getId())) {
                        return StrGen.createError(ss, "ID not defined");
                    }
                    IdInfo idInfo = idTable.get(memInfo.getId());
                    if (idInfo.getType() != IdInfo.IdType.Variable) {
                        return StrGen.createError(ss, "Invalid ID");
                    }
                    if (memInfo.getSize() == -1) {
                        memInfo.setSize(idInfo.getSize());
                    }
                }
                
                if (memInfo.getSize() == -1) {
                    memInfo.setSize(reg.getSize() / 8);
                }

                if (getMemError(memInfo) != null) {
                    return StrGen.createError(ss, getMemError(memInfo));
                }
                
                if (reg.getSize() / 8 != memInfo.getSize()) {
                    return StrGen.createError(ss, "argument types not match");
                }

                opCode = memInfo.getSize() == 1 ? "20 " : "21 ";
                int base1 = memInfo.getId() == null ? 0x04 : 0x84;
                int base2 = 0x00 + memInfo.getReg1().getNumber() + 8 * memInfo.getReg2().getNumber();
                opCode += String.format("%02X ", base1 + 8 * reg.getNumber());
                opCode += String.format("%02X ", base2);
                size = 3;
                
                if (memInfo.getSegReg() != null && memInfo.getSegReg() != SegRegInfo.DS) {
                    opCode = memInfo.getSegReg().getPrefix() + opCode;
                    size++;
                }
                
                if (memInfo.getId() != null) {
                    opCode += String.format("%08X", idTable.get(memInfo.getId()).getAddress());
                    size += 4;
                }
                break;
            }
            // SHR REG , CONST
            // SHR REG , ID
            case SHR: {
                RegInfo reg = RegInfo.valueOf(ss.getLexemes()[1].getValue().toUpperCase());
                SyntxUnit lastToken = ss.getSyntaxTokens()[ss.getSyntaxTokens().length - 1];
                ConstInfo constInfo = null;
                if (lastToken.getType() == SyntxUnitType.ID) {
                    if (!idTable.containsKey(lastToken.getLexToken().getValue().toUpperCase())) {
                        needSecondPass = true;
                        opCode = "90 90 90";
                        size = 3;
                        break;
                    }
                    IdInfo idInfo = idTable.get(lastToken.getLexToken().getValue().toUpperCase());                    
                    if (idInfo.getType() != IdInfo.IdType.Ravno) {
                        return StrGen.createError(ss, "Invalid ID");
                    }
                    constInfo = idInfo.getConstInfo();
                } else {
                    constInfo = ConstInfo.parseConst(lastToken.getLexToken().getValue());
                }
                
                if (constInfo.getSize() != 1) {
                    return StrGen.createError(ss, "Invalid CONST");
                }
                
                if (constInfo.getIntValue() == 1) {
                    opCode = reg.getSize() == 8 ? "D0 " : "D1 ";
                    opCode += String.format("%02X", 0xE8 + reg.getNumber());
                    size = 2;
                } else {
                    opCode = reg.getSize() == 8 ? "C0 " : "C1 ";
                    opCode += String.format("%02X ", 0xE8 + reg.getNumber());
                    opCode += constInfo.getBytesHex(1, "");
                    size = 3;
                }                
                break;
            }
            // OR MEM , CONST
            // OR MEM , ID
            case OR: {
                SyntxUnit lastToken = ss.getSyntaxTokens()[ss.getSyntaxTokens().length - 1];
                ConstInfo constInfo;
                if (lastToken.getType() == SyntxUnitType.ID) {
                    String id = lastToken.getLexToken().getValue().toUpperCase();
                    if (!idTable.containsKey(id)) {
                        needSecondPass = true;
                        opCode = "90| 90 90 90 90909090 90909090";
                        size = 12;
                        break;
                    }
                    IdInfo idInfo = idTable.get(id);
                    if (idInfo.getType() != IdInfo.IdType.Ravno) {
                        return StrGen.createError(ss, "Invalid ID");
                    }
                    constInfo = idInfo.getConstInfo();
                } else {
                    constInfo = ConstInfo.parseConst(lastToken.getLexToken().getValue());
                }

                StrSyntx memSs = AnalyzSyntx.parseString(Arrays.copyOfRange(ss.getLexemes(), 1, ss.getLexemes().length - 2));                
                MemInfo memInfo = MemInfo.parseMem(memSs);

                if (memInfo.getId() != null) {
                    if (!idTable.containsKey(memInfo.getId())) {
                        return StrGen.createError(ss, "ID not defined");
                    }
                    IdInfo idInfo = idTable.get(memInfo.getId());
                    if (idInfo.getType() != IdInfo.IdType.Variable) {
                        return StrGen.createError(ss, "Invalid ID");
                    }
                    if (memInfo.getSize() == -1) {
                        memInfo.setSize(idInfo.getSize());
                    }
                }

                if (getMemError(memInfo) != null) {
                    return StrGen.createError(ss, getMemError(memInfo));
                }
                
                if (constInfo.getSize() > memInfo.getSize()) {
                    return StrGen.createError(ss, "Invalid CONST");
                }

                int constSize = constInfo.getSize() == 1 ? 1 : memInfo.getSize();
                                
                opCode = memInfo.getSize() == 1 ? "80 " : (constInfo.getSize() == 1 ? "83 " : "81 ");
                int base1 = memInfo.getId() == null ? 0x0C : 0x8C;
                int base2 = 0x00 + memInfo.getReg1().getNumber() + 8 * memInfo.getReg2().getNumber();
                opCode += String.format("%02X ", base1);
                opCode += String.format("%02X ", base2);
                size = 3;
                
                if (memInfo.getId() != null) {
                    opCode += String.format("%08X ", idTable.get(memInfo.getId()).getAddress());
                    size += 4;
                }
                
                if (memInfo.getSegReg() != null && memInfo.getSegReg() != SegRegInfo.DS) {
                    opCode = memInfo.getSegReg().getPrefix() + opCode;
                    size++;
                }
                
                opCode += constInfo.getBytesHex(constSize, "");                
                size += constSize;
                break;
            }
            // JBE ID
            case JBE: {
                String id = ss.getLexemes()[1].getValue().toUpperCase();
                if (!idTable.containsKey(id)) {
                    needSecondPass = true;
                    opCode = "90 90 90909090";
                    size = 6;
                } else {
                    IdInfo label = idTable.get(id);
                    if (!label.getSegment().getName().equalsIgnoreCase(currentSegment.getName())) {
                        return StrGen.createError(ss, "far jumps not allowed");
                    }
                    int addressDifference = currentSegment.getCurrentAddress() + 2 - label.getAddress();
                    if (addressDifference < 127 && addressDifference > -128) {
                        opCode = "76 " + String.format("%02X", -addressDifference & 0xFF);
                        size = 2;
                    } else {
                        opCode = "0F 86 " + String.format("%08X", -addressDifference - 4);
                        size = 6;
                    }
                }               
                break;
            }
        }

        StrGen command = StrGen.createCommand(ss, opCode, currentSegment.getCurrentAndIncrement(size), size);
        if (needSecondPass) {
            command.setType(StrGenType.NeedSecondPass);
            if (secondPass) {
                return StrGen.createError(ss, "ID not defined");
            }
        }
        return command;
    }

    private StrGen parseEndFile(StrSyntx ss) {

        if (currentSegment != null) {
            return StrGen.createError(ss, "segment has no ENDS");
        }

        String id = ss.getLexemes()[1].getValue().toUpperCase();

        if (!idTable.containsKey(id)) {
            return StrGen.createError(ss, "ID not defined");
        }

        return StrGen.createEndFile(ss);
    }

    public void firstPass() {

        idTable.clear();
        segTable.clear();
        generetedStrings.clear();
        currentSegment = null;

        for (StrSyntx ss : syntaxStrings) {
            if (ss.isEmpty()) {
                generetedStrings.add(StrGen.createEmpty(ss));
            } else if (!AnalyzSyntx.isValid(ss)) {
                generetedStrings.add(StrGen.createError(ss, "syntax error"));
            } else {

                while (ss.getSyntaxString().startsWith("ID : ") && ss.getSyntaxTokens().length > 2) {
                    StrSyntx labelSs = AnalyzSyntx.parseString(ss.getLexemes()[0], ss.getLexemes()[1]);
                    ss = AnalyzSyntx.parseString(Arrays.copyOfRange(ss.getLexemes(), 2, ss.getLexemes().length));
                    generetedStrings.add(parseLabel(labelSs));
                }

                if (ss.getSyntaxString().startsWith("IF ")) {
                    generetedStrings.add(parseIfBegin(ss));
                    continue;
                }

                if (ss.getSyntaxString().equals("ELSE")) {
                    generetedStrings.add(parseIfElse(ss));
                    continue;
                }

                if (ss.getSyntaxString().equals("ENDIF")) {
                    generetedStrings.add(parseIfEnd(ss));
                    continue;
                }

                if (!ifBlocks.isEmpty()) {
                    IfElseInfo currentIf = ifBlocks.peek();
                    if (currentIf.isIfBody() && currentIf.isInsideElse() || !currentIf.isIfBody() && !currentIf.isInsideElse()) {
                        continue;
                    }
                }
                
                if (ss.getSyntaxString().equals("ID :")) {
                    generetedStrings.add(parseLabel(ss));
                    continue;
                }

                if (ss.getSyntaxString().equals("ID SEGMENT")) {
                    generetedStrings.add(parseSegmentBegin(ss));
                    continue;
                }

                if (ss.getSyntaxString().equals("ID ENDS")) {
                    generetedStrings.add(parseSegmentEnd(ss));
                    continue;
                }

                if (ss.getSyntaxString().startsWith("ID = ")) {
                    generetedStrings.add(parseRavnoDef(ss));
                    continue;
                }

                if (ss.getSyntaxString().matches("ID (DB|DW|DD) (CONST|ID)")) {
                    generetedStrings.add(parseVarDef(ss));
                    continue;
                }

                if (ss.getSyntaxString().equals("END ID")) {
                    generetedStrings.add(parseEndFile(ss));
                    break;
                }

                generetedStrings.add(parseCommand(ss));
            }
        }
    }

    public void secondPass() {

        segTable.clear();
        secondPass = true;

        for (StrGen gs : generetedStrings) {

            if (gs.getType() == StrGenType.SegmentBegin) {
                parseSegmentBegin(gs.getSyntaxString());
                continue;
            }

            if (gs.getType() == StrGenType.SegmentEnd) {
                parseSegmentEnd(gs.getSyntaxString());
                continue;
            }

            if (gs.getType() == StrGenType.NeedSecondPass) {
                StrGen newGs;
                // var definition
                if (gs.getSyntaxString().getSyntaxTokens()[0].getType() == SyntxUnitType.ID) {
                    newGs = parseVarDef(gs.getSyntaxString());
                } else { // command
                    newGs = parseCommand(gs.getSyntaxString());
                }
                gs.update(newGs);
                currentSegment.setCurrentAddress(gs.getAddress() + gs.getSize());
                continue;
            }

            if (gs.getSize() > 0 && currentSegment != null) {
                currentSegment.getCurrentAndIncrement(gs.getSize());
            }
        }
    }

    public List<StrGen> getGeneretedStrings() {
        return generetedStrings;
    }

    public String getIdTable() {
        String[] tableHeader = {"Id", "Type", "Address"};
        String tableFormatPattern = "%-6s | %-5s | %-14s%n";
        StringBuilder resultTable = new StringBuilder();

        resultTable.append(String.format(tableFormatPattern, tableHeader));

        for (IdInfo id : idTable.values()) {
            if (id.getType() == IdInfo.IdType.Ravno) {
                continue;
            }
            String name = id.getName();
            String type = id.getTypeName();
            String address = id.getSegment() == null ? "" : id.getSegment().getName() + ":" + formatAddress(id.getAddress());
            resultTable.append(String.format(tableFormatPattern, name, type, address));
        }

        return resultTable.toString();
    }

    public String getEquTable() {
        String[] tableHeader = {"Id", "Value"};
        String tableFormatPattern = "%-6s | %s%n";
        StringBuilder resultTable = new StringBuilder();

        resultTable.append(String.format(tableFormatPattern, tableHeader));

        for (IdInfo id : idTable.values()) {
            if (id.getType() != IdInfo.IdType.Ravno) {
                continue;
            }
            String name = id.getName();
            resultTable.append(String.format(tableFormatPattern, name, id.getConstInfo().getValue()));
        }

        return resultTable.toString();
    }

    public String getSegTable() {
        String[] tableHeader = {"Segment", "Size"};
        String tableFormatPattern = "%-7s | %s%n";
        StringBuilder resultTable = new StringBuilder();

        resultTable.append(String.format(tableFormatPattern, tableHeader));

        for (SegInfo seg : segTable.values()) {
            resultTable.append(String.format(tableFormatPattern, seg.getName(), formatAddress(seg.getSize())));
        }

        return resultTable.toString();
    }

    public String getErrors() {
        String[] tableHeader = {"###", "Error"};
        String tableFormatPattern = "%3s) %s%n";
        StringBuilder resultTable = new StringBuilder();

        resultTable.append(String.format(tableFormatPattern, tableHeader));

        int counter = 1;
        int errors = 0;
        for (StrGen gs : generetedStrings) {
            if (gs.getType() == StrGenType.Error) {
                resultTable.append(String.format(tableFormatPattern, counter, gs.getOutput()));
                errors++;
            }
            counter++;
        }

        if (errors == 0) {
            return "No errors";
        }

        return resultTable.toString();
    }

}
