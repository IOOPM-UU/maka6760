package org.ioopm.calculator.ast.unary;

import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.Unary;
import org.ioopm.calculator.ast.atom.Constant;

public class Sin extends Unary {

    public Sin(SymbolicExpression exp) {
        super(exp);
    }    

    @Override 
    public String getName() {
        return "sin";
    }

    @Override 
    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression arg = this.getExp().eval(vars); 
        if (arg.isConstant()) {
            return new Constant(Math.sin(arg.getValue()));
        } else {
            return new Sin(arg);
        }
    }
}