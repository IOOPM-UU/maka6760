package org.ioopm.calculator.ast;

public abstract class Binary extends SymbolicExpression {
    private SymbolicExpression lhs = null;
    private SymbolicExpression rhs = null;
    
    
    public Binary(SymbolicExpression lhs, SymbolicExpression rhs) {
        this.lhs = lhs;
        this.rhs = rhs;
    }
    
    public SymbolicExpression getLhs() {
        return this.lhs;
    }
    
    public SymbolicExpression getRhs() {
        return this.rhs;
    }

    public String toString() {
        String leftstring = this.lhs.toString();
        if(this.lhs.getPriority() < this.getPriority()) {
            leftstring = "(" + leftstring + ")";
        }
        
        String rightString = this.rhs.toString();
        if(this.rhs.getPriority() <= this.getPriority()) {
            rightString = "(" + rightString + ")";
        }
        
        return leftstring + " " + this.getName() + " " + rightString;
    } 

    public boolean equals(Object other) {
        if (other instanceof Binary) {
            Binary tmp = (Binary) other;
            return this.getName().equals(tmp.getName()) && this.lhs.equals(tmp.lhs) && this.rhs.equals(tmp.rhs);
        } else {
            return false;
        }
    }

}