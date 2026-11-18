package org.ioopm.calculator.ast.atom;

import org.ioopm.calculator.ast.Atom;
import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.SymbolicExpression;

public class Constant extends Atom {
    private double value;

    public Constant(double value) {
        this.value = value;
    }

    @Override 
    public boolean isConstant() {
        return true;
    }

    @Override 
    public double getValue() {
        return this.value;
    }

    public String toString() {
    	return String.valueOf(this.value);
    }

    public boolean equals(Object other) {
        if (other instanceof Constant) {
            Constant tmp = (Constant) other;
            return this.value == tmp.value;
        } else {
            return false;
        }
    }

    @Override 
    public SymbolicExpression eval(Environment vars) {
        return this;
    }
}
