package oksana.analyz;

import oksana.units.StrLex;
import oksana.units.LexUnitType;
import oksana.units.LexUnit;
import java.util.List;
import java.util.regex.Pattern;
import java.util.stream.Collectors;

public abstract class AnalyzLex {

    private static LexUnit parseToken(String input) {

        LexUnitType[] types = {
            LexUnitType.OneSymbol,
            LexUnitType.Register8,
            LexUnitType.Register32,
            LexUnitType.Constant,
            LexUnitType.RegisterSegment,
            LexUnitType.Directive,
            LexUnitType.Command,
            LexUnitType.TypeOperator,
            LexUnitType.Identifier
        };

        for (LexUnitType type : types) {
            if (type.isMatch(input)) {
                return new LexUnit(input, type);
            }
        }

        return new LexUnit(input, LexUnitType.Invalid);
    }

    public static StrLex parseString(String input) {
        Pattern seperator = Pattern.compile("[\\s\\t]+");
        String oneSymbolPattern = LexUnitType.OneSymbol.getPattern()
                .pattern().replace('^', '(').replace('$', ')');

        input = input.trim()
                .replaceAll(";.*", "")
                .replaceAll(oneSymbolPattern, " $1 ");

        if (input.length() == 0) {
            return new StrLex();
        }

        String[] subLines = seperator.split(input);
        LexUnit[] lexemes = new LexUnit[subLines.length];

        for (int i = 0; i < lexemes.length; i++) {
            lexemes[i] = parseToken(subLines[i]);
        }
        return new StrLex(lexemes);
    }

    public static List<StrLex> parseStrings(List<String> input) {
        return input.stream().map((line) -> parseString(line)).collect(Collectors.toList());
    }
}
