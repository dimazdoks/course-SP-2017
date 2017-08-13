package oksana.units;

import java.util.regex.Pattern;

public enum LexUnitType {
    
        /**
         * <b>Ідентифікатори</b><br>
         * Містять великі і малі букви латинского алфавіту та цифри. <br>
         * Починаються з букви. <br>
         * Великі та малі букви не відрізняються. <br>
         * Довжина ідентифікаторів не більше 6 символів <br>
         */
        Identifier("Ідентифікатор", "^([a-zA-Z][a-zA-Z0-9]*)$"),
        /**
         * <b>Константи</b><br>
         * Десяткові, шістнадцяткові та текстові константи <br>
         */
        Constant("Константа", "^([-+]?)([0-9]+|[a-fA-F0-9]+h|[01]+b|('[^']*'|\"[^\"]*\"))$"),
        /**
         * <b>Директиви</b><br>
         * <i>END</i>, <br>
         * <i>SEGMENT</i> - без операндів, <br>
         * <i>ENDS</i>, программа може мати тільки один сегмент кодів і тільки
         * один сегмент даних <br>
         * <i>EQU</i>, <br>
         * <i>DB, DW, DD</i> з одним операндом - константою (рядкові константи
         * тільки для <i>DB</i>)
         */
        Directive("Директива", "^(END|SEGMENT|ENDS|=|DB|DW|DD|IF|ELSE|ENDIF)$"),
        /**
         * <b>Регістри (8 біт)</b><br>
         * <i>AL, AH, CL, CH, DL, DH, BL, BH</i>
         */
        Register8("Регістр (8 біт)", "^(AL|AH|CL|CH|DL|DH|BL|BH)$"),
        /**
         * <b>Регістри (32 біт)</b><br>
         * <i>EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI</i>
         */
        Register32("Регістр (32 біти)", "^(EAX|ECX|EDX|EBX|ESP|EBP|ESI|EDI)$"),
        /**
         * <b>Регістри (Сегментні)</b><br>
         * <i>SS, CS, DS, ES, FS, GS</i>
         */
        RegisterSegment("Регістр сегменту", "^(SS|CS|DS|ES|FS|GS)$"),
        /**
         * <b>Односимвольні</b><br>
         * <i>[ ] , * :</i>
         */
        OneSymbol("Односимвольна", "^[,+:\\[\\]]$"),
        /**
         * <b>Оператор визначення типу</b><br>
         * <i>ptr byte dword</i>
         */
        TypeOperator("Оператор визначення типу", "^(PTR|BYTE|DWORD)$"),
        /**
         * <b>Мнемокоди команд</b><br>
         * <i>CLC, NOT, IDIV, CMP, LES, MOV, ADD, JNAE</i>
         */
        Command("Мнемокод команди", "^(CMPSD|NEG|PUSH|ADC|CMP|AND|SHR|OR|JBE)$"),
        /**
         * <b>Недопустима лексема</b>
         */
        Invalid("Недопустима лексема", ".*");

        private final String name;
        private final Pattern pattern;

        private LexUnitType(String textValue, String pattern) {
            this.name = textValue;
            this.pattern = Pattern.compile(pattern, Pattern.CASE_INSENSITIVE);
        }

        /**
         * Повертае регулярний вираз відповідний даному типу лексем
         *
         * @return
         */
        public Pattern getPattern() {
            return pattern;
        }

        /**
         * Повертае назву типу лексеми Українською
         *
         * @return
         */
        public String getName() {
            return name;
        }

        /**
         * Перевіряє чи відповідае вхідна стрічка даній лексемі
         *
         * @param input - вхідна стрічка
         * @return
         */
        public boolean isMatch(String input) {
            return getPattern().matcher(input).matches();
        }
    
}
