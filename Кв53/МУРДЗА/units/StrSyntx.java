package oksana.units;

import java.util.Arrays;
import java.util.Objects;
import java.util.stream.Collectors;

public class StrSyntx {

    private final StrLex lexicalString;
    private final SyntxUnit[] syntaxTokens;
    private final String syntaxString;

    public StrSyntx(StrLex lexString) {
        this.lexicalString = lexString;
        this.syntaxTokens = null;
        this.syntaxString = "";
    }

    public StrSyntx(StrLex lexString, SyntxUnit[] syntaxTokens) {
        this.lexicalString = lexString;
        this.syntaxTokens = syntaxTokens;
        String tmpSyntaxString = String.join(" ", Arrays.asList(syntaxTokens)
                .stream().map((token) -> token.getTypeString())
                .collect(Collectors.toList()));

        String[] memSchemes = {
            "TYPE PTR SEG : ID [ REG + REG ]",
            "TYPE PTR SEG : [ REG + REG ]",
            "TYPE PTR ID [ REG + REG ]",
            "TYPE PTR [ REG + REG ]",
            "SEG : ID [ REG + REG ]",
            "SEG : [ REG + REG ]",
            "ID [ REG + REG ]",
            "[ REG + REG ]"
        };

        for (String memScheme : memSchemes) {
            if (tmpSyntaxString.contains(memScheme)) {
                tmpSyntaxString = tmpSyntaxString.replace(memScheme, "MEM");
                break;
            }
        }

        this.syntaxString = tmpSyntaxString;
    }

    public StrLex getLexicalString() {
        return lexicalString;
    }

    public String getSyntaxString() {
        return syntaxString;
    }

    public SyntxUnit[] getSyntaxTokens() {
        return syntaxTokens;
    }

    public LexUnit[] getLexemes() {
        return getLexicalString().getLexemes();
    }
    
    public boolean isEmpty() {
        return lexicalString.isEmpty();
    }

    @Override
    public String toString() {
        return String.format("[%s]: %s", syntaxString, lexicalString.getTextString());
    }

    @Override
    public int hashCode() {
        int hash = 7;
        hash = 17 * hash + Objects.hashCode(this.lexicalString);
        hash = 17 * hash + Objects.hashCode(this.syntaxString);
        return hash;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (obj == null) {
            return false;
        }
        if (getClass() != obj.getClass()) {
            return false;
        }
        final StrSyntx other = (StrSyntx) obj;
        if (!Objects.equals(this.syntaxString, other.syntaxString)) {
            return false;
        }
        return Objects.equals(this.lexicalString, other.lexicalString);
    }

}
