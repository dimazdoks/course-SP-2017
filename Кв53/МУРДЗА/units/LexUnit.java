package oksana.units;

import java.util.Objects;

public class LexUnit {

    private final String value;
    private final LexUnitType type;

    public LexUnit(String value, LexUnitType type) {
        this.value = value;
        this.type = type;
    }

    public LexUnitType getType() {
        return type;
    }

    public String getValue() {
        return value;
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
        final LexUnit other = (LexUnit) obj;
        if (!Objects.equals(this.value, other.value)) {
            return false;
        }
        return this.type == other.type;
    }

    @Override
    public int hashCode() {
        int hash = 7;
        hash = 59 * hash + Objects.hashCode(this.value);
        hash = 59 * hash + Objects.hashCode(this.type);
        return hash;
    }

    @Override
    public String toString() {
        return String.format("[%s]: %s", type, value);
    }
}
