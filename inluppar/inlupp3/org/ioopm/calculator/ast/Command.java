package org.ioopm.calculator.ast;

public abstract class Command extends SymbolicExpression {
    
    @Override 
    public SymbolicExpression eval(Environment vars) {
        throw new RuntimeException("Commands can not be evaluated");
    }
}
