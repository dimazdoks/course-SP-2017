package oksana.analyz;

import java.util.List;
import java.util.stream.Collectors;
import oksana.units.LexUnit;
import oksana.units.StrLex;
import oksana.units.StrSyntx;
import oksana.units.SyntxUnit;
import oksana.units.SyntxUnitType;

public abstract class AnalyzSyntx {

    private static final String[] VALID_RULES = {
        // Segment
        "ID SEGMENT",
        "ID ENDS",
        // End of file
        "END ID",
        // =
        "ID = CONST",
        // Variables
        "ID DB CONST",
        "ID DW CONST",
        "ID DD CONST",
        // =
        "ID DB ID",
        "ID DW ID",
        "ID DD ID",
        // Label
        "ID :",
        // IF-ELSE-ENDIF
        "IF ID",
        "IF CONST",
        "ELSE",
        "ENDIF",
        // Commands
        "CMPSD",
        //
        "NEG REG",
        //
        "PUSH MEM",
        //
        "ADC REG , REG",
        // 
        "CMP REG , MEM",
        // 
        "AND MEM , REG",
        // 
        "SHR REG , CONST",
        // =
        "SHR REG , ID",
        //
        "OR MEM , CONST",
        // =
        "OR MEM , ID",
        //
        "JBE ID"
    };

    private static SyntxUnit parseDirective(LexUnit lexToken) {
        switch (lexToken.getValue().toUpperCase()) {
            case "SEGMENT":
                return new SyntxUnit(lexToken, SyntxUnitType.SEGMENT);
            case "ENDS":
                return new SyntxUnit(lexToken, SyntxUnitType.ENDS);
            case "END":
                return new SyntxUnit(lexToken, SyntxUnitType.END);
            case "IF":
                return new SyntxUnit(lexToken, SyntxUnitType.IF);
            case "ELSE":
                return new SyntxUnit(lexToken, SyntxUnitType.ELSE);
            case "ENDIF":
                return new SyntxUnit(lexToken, SyntxUnitType.ENDIF);
            case "=":
                return new SyntxUnit(lexToken, SyntxUnitType.RAVNO);
            case "DB":
                return new SyntxUnit(lexToken, SyntxUnitType.DB);
            case "DW":
                return new SyntxUnit(lexToken, SyntxUnitType.DW);
            case "DD":
                return new SyntxUnit(lexToken, SyntxUnitType.DD);
            default: // this case will never raised
                return new SyntxUnit(lexToken, SyntxUnitType.INVALID);
        }
    }

    private static SyntxUnit parseTypeCast(LexUnit lexToken) {
        switch (lexToken.getValue().toUpperCase()) {
            case "BYTE":
            case "DWORD":
                return new SyntxUnit(lexToken, SyntxUnitType.TYPE);
            case "PTR":
                return new SyntxUnit(lexToken, SyntxUnitType.PTR);
            default: // this case will never raised
                return new SyntxUnit(lexToken, SyntxUnitType.INVALID);
        }
    }

    private static SyntxUnit parseCommand(LexUnit lexToken) {
        switch (lexToken.getValue().toUpperCase()) {
            case "CMPSD":
                return new SyntxUnit(lexToken, SyntxUnitType.CMPSD);
            case "NEG":
                return new SyntxUnit(lexToken, SyntxUnitType.NEG);
            case "PUSH":
                return new SyntxUnit(lexToken, SyntxUnitType.PUSH);
            case "ADC":
                return new SyntxUnit(lexToken, SyntxUnitType.ADC);
            case "CMP":
                return new SyntxUnit(lexToken, SyntxUnitType.CMP);
            case "AND":
                return new SyntxUnit(lexToken, SyntxUnitType.AND);
            case "SHR":
                return new SyntxUnit(lexToken, SyntxUnitType.SHR);
            case "OR":
                return new SyntxUnit(lexToken, SyntxUnitType.OR);
            case "JBE":
                return new SyntxUnit(lexToken, SyntxUnitType.JBE);
            default: // this case will never raised
                return new SyntxUnit(lexToken, SyntxUnitType.INVALID);
        }
    }

    private static SyntxUnit parseToken(LexUnit lexToken) {
        switch (lexToken.getType()) {
            case Identifier:
                return new SyntxUnit(lexToken, SyntxUnitType.ID);
            case Constant:
                return new SyntxUnit(lexToken, SyntxUnitType.CONST);
            case Directive:
                return parseDirective(lexToken);
            case Register8:
            case Register32:
                return new SyntxUnit(lexToken, SyntxUnitType.REG);
            case RegisterSegment:
                return new SyntxUnit(lexToken, SyntxUnitType.SEG);
            case OneSymbol:
                return new SyntxUnit(lexToken, SyntxUnitType.SYMBOL);
            case TypeOperator:
                return parseTypeCast(lexToken);
            case Command:
                return parseCommand(lexToken);
            default:
                return new SyntxUnit(lexToken, SyntxUnitType.INVALID);
        }
    }

        public static StrSyntx parseString(LexUnit... lexemes) {
        if (lexemes == null || lexemes.length == 0) {
            return new StrSyntx(new StrLex());
        }
        
        SyntxUnit[] syntaxTokens = new SyntxUnit[lexemes.length];

        for (int i = 0; i < lexemes.length; i++) {
            syntaxTokens[i] = parseToken(lexemes[i]);
        }

        return new StrSyntx(new StrLex(lexemes), syntaxTokens);
    }
    
    public static StrSyntx parseString(StrLex lexString) {

        if (lexString.isEmpty()) {
            return new StrSyntx(lexString);
        }

        LexUnit[] lexemes = lexString.getLexemes();
        SyntxUnit[] syntaxTokens = new SyntxUnit[lexemes.length];

        for (int i = 0; i < lexemes.length; i++) {
            syntaxTokens[i] = parseToken(lexemes[i]);
        }

        return new StrSyntx(lexString, syntaxTokens);
    }

    public static List<StrSyntx> parseStrings(List<StrLex> input) {
        return input.stream().map((line) -> parseString(line)).collect(Collectors.toList());
    }

    public static boolean isValid(StrSyntx syntaxString) {

        if (syntaxString.isEmpty()) {
            return true;
        }

        String syntaxScheme;
        if (syntaxString.getSyntaxString().startsWith("ID : ")) {
            syntaxScheme = syntaxString.getSyntaxString().substring(5);
        } else {
            syntaxScheme = syntaxString.getSyntaxString();
        }

        for (String validScheme : VALID_RULES) {
            if (validScheme.equals(syntaxScheme)) {
                return true;
            }
        }

        return false;
    }
}
