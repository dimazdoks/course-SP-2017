package oksana;

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.io.UnsupportedEncodingException;
import java.util.List;
import java.util.stream.Collectors;
import oksana.analyz.AnalyzFirstSecond;
import oksana.units.StrGen;
import oksana.analyz.AnalyzLex;
import oksana.units.StrLex;
import oksana.analyz.AnalyzSyntx;
import oksana.units.StrSyntx;

public class OksanaTranslator {

    private static enum TranslationStates {
        Lexical,
        Syntax,
        FirstPass,
        SecondPass
    }

    private static TranslationStates workMode;
    private static String[] runArguments;
    private static InputStream myInputStream;
    private static PrintStream myOutputStream;

    private static String getRunArgument(int i) {
        return runArguments.length >= (i + 1) ? runArguments[i] : null;
    }

    private static InputStream getInputStream() {

        String inputArgument = getRunArgument(1);

        if (inputArgument == null) {
            return null;
        }

        if ("--test".equals(inputArgument)) {
            return new ByteArrayInputStream(TestFiles.getValidTestFile().getBytes());
        }

        if ("--sin".equals(inputArgument)) {
            return System.in;
        }

        try {
            return new FileInputStream(inputArgument);
        } catch (Exception ex) {
            System.out.println();
            System.out.println(ex.getMessage());
            System.out.println();
            System.exit(1);
            return null;
        }
    }

    private static PrintStream getOutputStream() {
        String outputArgument = getRunArgument(2);

        if (outputArgument == null) {
            return System.out;
        }

        try {
            return new PrintStream(new FileOutputStream(outputArgument));
        } catch (Exception ex) {
            System.out.println();
            System.out.println(ex.getMessage());
            System.out.println();
            System.exit(1);
            return null;
        }
    }

    private static List<String> readLines() {

        return new BufferedReader(new InputStreamReader(myInputStream))
                .lines().collect(Collectors.toList());
    }

    private static void lexicalMode() {
        List<String> inputLines = readLines();

        for (String line : inputLines) {
            StrLex lexemeLine = AnalyzLex.parseString(line);
            if (!lexemeLine.isEmpty()) {
                myOutputStream.println(lexemeLine.toStringTable());
            }
        }
    }

    private static void syntaxMode() {
        List<String> inputLines = readLines();
        List<StrLex> lexLines = AnalyzLex.parseStrings(inputLines);
        
        for (StrLex lexLine : lexLines) {
            StrSyntx syntaxString = AnalyzSyntx.parseString(lexLine);
            myOutputStream.printf("[%b] %s ---> %s%n", AnalyzSyntx.isValid(syntaxString), syntaxString.getSyntaxString(), syntaxString.getLexicalString().getTextString());
        }
    }

    private static void firstPassMode() {
        List<String> inputLines = readLines();
        List<StrLex> lexLines = AnalyzLex.parseStrings(inputLines);
        List<StrSyntx> syntaxLines = AnalyzSyntx.parseStrings(lexLines);

        AnalyzFirstSecond generator = new AnalyzFirstSecond(syntaxLines);
        generator.firstPass();

        myOutputStream.println("   Виконала Оксана Мурдза (КВ-53)");
        
        int counter = 1;
        for (StrGen gLine : generator.getGeneretedStrings()) {
            myOutputStream.printf("%3d %s%n",counter++, gLine);
        }
        myOutputStream.println();
        myOutputStream.println(generator.getEquTable());
        myOutputStream.println(generator.getIdTable());
        myOutputStream.println(generator.getSegTable());        
        myOutputStream.println(generator.getErrors());
    }

    private static void secondPassMode() {
        List<String> inputLines = readLines();
        List<StrLex> lexLines = AnalyzLex.parseStrings(inputLines);
        List<StrSyntx> syntaxLines = AnalyzSyntx.parseStrings(lexLines);

        AnalyzFirstSecond generator = new AnalyzFirstSecond(syntaxLines);
        generator.firstPass();
        generator.secondPass();

        myOutputStream.println("   Виконала Оксана Мурдза (КВ-53)");
        
        int counter = 1;
        for (StrGen gLine : generator.getGeneretedStrings()) {
            myOutputStream.printf("%3d %s%n",counter++, gLine);
        }
        myOutputStream.println();
        myOutputStream.println(generator.getEquTable());
        myOutputStream.println(generator.getIdTable());
        myOutputStream.println(generator.getSegTable());        
        myOutputStream.println(generator.getErrors());
    }

    public static void main(String[] args) throws UnsupportedEncodingException {
        
        //args = new String[]{"-4", "--test"};
        runArguments = args;        
        
        String runMode = getRunArgument(0);

        if (runMode == null) {
            runMode = "invalid";
        }

        switch (runMode) {
            case "-1":
                workMode = TranslationStates.Lexical;
                break;
            case "-2":
                workMode = TranslationStates.Syntax;
                break;
            case "-3":
                workMode = TranslationStates.FirstPass;
                break;
            case "-4":
                workMode = TranslationStates.SecondPass;
                break;
            default:
                System.out.println("\nInvalid mode!!!\n");
                System.exit(1);
                return;
        }
        
        myInputStream = getInputStream();
        myOutputStream = getOutputStream();

        switch (workMode) {
            case Lexical:
                lexicalMode();
                break;
            case Syntax:
                syntaxMode();
                break;
            case FirstPass:
                firstPassMode();
                break;
            case SecondPass:
                secondPassMode();
                break;

        }
    }

}
