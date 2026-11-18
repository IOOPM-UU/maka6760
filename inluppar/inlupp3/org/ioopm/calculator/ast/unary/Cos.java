package org.ioopm.calculator.ast.unary;

import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.Unary;
import org.ioopm.calculator.ast.atom.Constant;
import org.ioopm.calculator.ast.Environment;

public class Cos extends Unary {
    
    public Cos(SymbolicExpression exp) {
        super(exp);
    }

    @Override 
    public String getName() {
        return "cos";
    }

    @Override 
    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression arg = this.getExp().eval(vars); 
        if (arg.isConstant()) {
            return new Constant(Math.cos(arg.getValue()));
        } else {
            return new Cos(arg);
        }
    }
}
