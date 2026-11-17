package org.ioopm.calculator.ast;

public abstract class Unary extends SymbolicExpression {
    private SymbolicExpression exp;

    public Unary(SymbolicExpression exp) {
        this.exp = exp;
    }
}
