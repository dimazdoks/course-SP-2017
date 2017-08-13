package oksana.units;

import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;
import java.util.Objects;

public class StrLex {

    private final LexUnit[] lexemes;
    private final String textString;
    private final String lexemeString;

    public StrLex() {
        this.lexemes = null;
        this.textString = null;
        this.lexemeString = null;
    }

    public StrLex(LexUnit... lexemes) {
        this.lexemes = lexemes;
        List<String> types = new LinkedList<>();
        List<String> texts = new LinkedList<>();

        for (LexUnit lexeme : lexemes) {
            types.add(lexeme.getType().toString());
            texts.add(lexeme.getValue());
        }

        textString = String.join(" ", texts);
        lexemeString = String.join(" ", types);
    }

    public LexUnit[] getLexemes() {
        return lexemes.clone();
    }

    public String getLexemeString() {
        return lexemeString;
    }

    public String getTextString() {
        return textString;
    }

    public boolean isEmpty() {
        return textString == null;
    }

    public String toStringTable() {

        String[] tableHeader = {"№", "Лексема", "Довжина", "Тип лексеми"};
        String tableFormatPattern = "%2s | %10s | %10s | %30s%n";
        StringBuilder resultTable = new StringBuilder();

        resultTable.append("Input: ").append(textString).append('\n');
        resultTable.append(String.format(tableFormatPattern, tableHeader));

        for (int i = 0; i < lexemes.length; i++) {
            resultTable.append(String.format(tableFormatPattern,
                    i + 1,
                    lexemes[i].getValue(),
                    lexemes[i].getValue().length(),
                    lexemes[i].getType().getName()
            ));
        }

        return resultTable.toString();
    }

    @Override
    public String toString() {
        return String.format("[%s]: %s", lexemeString, textString);
    }

    @Override
    public int hashCode() {
        int hash = 7;
        hash = 58 * hash + Arrays.deepHashCode(this.lexemes);
        hash = 58 * hash + Objects.hashCode(this.textString);
        hash = 58 * hash + Objects.hashCode(this.lexemeString);
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
        final StrLex other = (StrLex) obj;
        if (!Objects.equals(this.textString, other.textString)) {
            return false;
        }
        if (!Objects.equals(this.lexemeString, other.lexemeString)) {
            return false;
        }
        return Arrays.deepEquals(this.lexemes, other.lexemes);
    }

}
