package org.ioopm.calculator.ast;

public abstract class Unary extends SymbolicExpression {
    private SymbolicExpression exp;

    public Unary(SymbolicExpression exp) {
        this.exp = exp;
    }

    public SymbolicExpression getExp() {
        return this.exp;
    }

    public String toString() {
        String expString = this.exp.toString();
        
        if (this.exp.getPriority() < this.getPriority()) {
            expString = "(" + expString + ")";
        }
        
        return this.getName() + " " + expString;
    }

    public boolean equals(Object other) {
        if (other instanceof Unary) {
            Unary tmp = (Unary) other;
            return this.getName().equals(tmp.getName()) && this.exp.equals(tmp.exp);
        } else {
            return false;
        }
    }
}
