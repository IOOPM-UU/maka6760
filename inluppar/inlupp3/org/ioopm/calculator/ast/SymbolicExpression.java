package org.ioopm.calculator.ast;

public abstract class SymbolicExpression {
    public boolean isConstant() {
        return false;
    }

    public int getPriority() {
        return 100;
    }

    public String getName() {
        throw new RuntimeException("getName() called on expression with no operator");
    }

    public double getValue() {
        throw new RuntimeException("getValue() called on expression that is not a constant");
    }

    public abstract SymbolicExpression eval(Environment vars);

}