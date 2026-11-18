package org.ioopm.calculator.ast.binary;

import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.Binary;
import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.atom.Constant;

public class Subtraction extends Binary {
    public Subtraction(SymbolicExpression lhs, SymbolicExpression rhs) {
        super(lhs, rhs);        
    }  

    @Override 
    public String getName() {
        return "-";
    }

    @Override 
    public int getPriority() {
        return 50;
    }

    @Override
    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression left = this.getLhs().eval(vars);
        SymbolicExpression right = this.getRhs().eval(vars);
        if(left.isConstant() && right.isConstant()) {
            return new Constant(left.getValue() - right.getValue());
        } else {
            return new Subtraction(left, right);
        }
    }
}
