package org.ioopm.calculator.ast.atom;

import org.ioopm.calculator.ast.Atom;
import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.SymbolicExpression;

public class Variable extends Atom {
    private String identifier;

    public Variable(String identifier) {
        this.identifier = identifier;
    }

    public String toString() {
        return this.identifier;
    }

    @Override 
    public int hashCode() {
        return this.identifier.hashCode();
    }
    
    @Override 
    public boolean equals(Object other) {
        if (other instanceof Variable) {
            Variable tmp = (Variable) other;
            return this.identifier.equals(tmp.identifier);
        } else {
            return false;
        }
    }

    @Override 
    public SymbolicExpression eval(Environment vars) {
        if (vars.containsKey(this)) {
            return vars.get(this);
        } else {
            return new Variable(identifier); 
          }
    }
}
