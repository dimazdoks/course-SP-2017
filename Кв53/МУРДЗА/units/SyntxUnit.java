package oksana.units;

import java.util.Objects;

public class SyntxUnit {

    private final LexUnit lexicalToken;
    private final SyntxUnitType type;

    public SyntxUnit(LexUnit lexicalToken, SyntxUnitType type) {
        this.lexicalToken = lexicalToken;
        this.type = type;
    }

    public LexUnit getLexToken() {
        return lexicalToken;
    }

    public SyntxUnitType getType() {
        return type;
    }

    public String getTypeString() {
        if (type == SyntxUnitType.SYMBOL) {
            return lexicalToken.getValue();
        } else if (type == SyntxUnitType.RAVNO) {
            return lexicalToken.getValue();
        }
        return type.name();
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
        final SyntxUnit other = (SyntxUnit) obj;
        if (!Objects.equals(this.lexicalToken, other.lexicalToken)) {
            return false;
        }
        if (this.type != other.type) {
            return false;
        }
        return true;
    }

    @Override
    public int hashCode() {
        int hash = 3;
        hash = 23 * hash + Objects.hashCode(this.lexicalToken);
        hash = 23 * hash + Objects.hashCode(this.type);
        return hash;
    }

    @Override
    public String toString() {
        return String.format("[%s]: %s", getTypeString(), lexicalToken.getValue());
    }

}
