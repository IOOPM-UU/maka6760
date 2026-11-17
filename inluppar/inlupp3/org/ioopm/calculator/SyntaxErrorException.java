package org.ioopm.calculator;

public class SyntaxErrorException extends RuntimeException {
    public SyntaxErrorException() {
        super();
    }
    public SyntaxErrorException(String msg) {
        super(msg);
    }
}
