package org.ioopm.calculator.ast.binary;

import org.ioopm.calculator.ast.atom.Variable;
import org.ioopm.calculator.ast.Binary;
import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.SymbolicExpression;

public class Assignment extends Binary {
    public Assignment(SymbolicExpression lhs, SymbolicExpression rhs) {
        super(lhs, rhs);
    }

    @Override 
    public String getName() {
        return "=";
    }

    @Override 
    public int getPriority() {
        return 0;
    }

    @Override 
    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression left = this.getLhs().eval(vars);
        vars.put((Variable)this.getRhs(), left);

        return left;
    }
}