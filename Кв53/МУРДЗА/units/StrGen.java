package oksana.units;

import oksana.units.StrSyntx;

public class StrGen {

    private String source;
    private final StrSyntx syntaxString;
    private StrGenType type;
    private String output;
    private final int address;
    private final int size;

    public StrGen(StrSyntx input, StrGenType type, String output, int address, int size) {
        this.syntaxString = input;
        this.source = input.getLexicalString().getTextString();
        this.type = type;
        this.output = output;
        this.address = address;
        this.size = size;
    }

    public static StrGen createEmpty(StrSyntx input) {
        return new StrGen(input, StrGenType.Empty, "", 0, 0);
    }

    public static StrGen createError(StrSyntx input, String errorMsg) {
        return new StrGen(input, StrGenType.Error, errorMsg, 0, 0);
    }

    public static StrGen createLabel(StrSyntx input, String output, int address) {
        return new StrGen(input, StrGenType.LabelDef, ": " + output, address, 0);
    }

    public static StrGen createVariable(StrSyntx input, String value, int address, int size) {
        return new StrGen(input, StrGenType.VarDef, value, address, size);
    }

    public static StrGen createEqu(StrSyntx input, String output) {
        return new StrGen(input, StrGenType.RavnoDef, "= " + output, 0, 0);
    }

    public static StrGen createSegBegin(StrSyntx input) {
        return new StrGen(input, StrGenType.SegmentBegin, "", 0, 0);
    }

    public static StrGen createSegEnd(StrSyntx input, int size) {
        return new StrGen(input, StrGenType.SegmentEnd, "", size, 0);
    }

    public static StrGen createEndFile(StrSyntx input) {
        return new StrGen(input, StrGenType.EndFile, "", 0, 0);
    }

    public static StrGen createCommand(StrSyntx input, String code, int address, int size) {
        return new StrGen(input, StrGenType.VarDef, code, address, size);
    }

    public static StrGen createIfBegin(StrSyntx input, String condition, boolean value) {
        return new StrGen(input, StrGenType.IfBegin, String.format("= if -> (%s):%b", condition, value), 0, 0);
    }

    public static StrGen createIfElse(StrSyntx input, String condition, boolean value) {
        return new StrGen(input, StrGenType.IfBegin, String.format("= else  -> (%s):%b", condition, !value), 0, 0);
    }

    public static StrGen createIfEnd(StrSyntx input, String condition) {
        return new StrGen(input, StrGenType.IfBegin, String.format("= endif -> (%s)", condition), 0, 0);
    }
    
    public void update(StrGen gs) {
        this.output = gs.output;
        this.source = gs.source;
        this.type = gs.type;
        int sizeDiff = this.size - gs.size;
        
        while (sizeDiff > 0) {
            this.output += " 90";
            sizeDiff--;
        }
    }

    public StrSyntx getSyntaxString() {
        return syntaxString;
    }

    public String getSource() {
        return source;
    }

    public void setSource(String source) {
        this.source = source;
    }

    public StrGenType getType() {
        return type;
    }

    public void setType(StrGenType type) {
        this.type = type;
    }

    public int getSize() {
        return size;
    }

    public String getOutput() {
        return output;
    }

    public void setOutput(String output) {
        this.output = output;
    }

    public int getAddress() {
        return address;
    }

    @Override
    public String toString() {
        if (type == StrGenType.Empty) {
            return "";
        }

        if (type == StrGenType.Error) {
            return "*ERROR*: " + output + " (" + source + ")";
        }

        return String.format("%08X %-30s %s", address, output, source);
    }

}
