package org.ioopm.calculator.ast.unary;

import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.Unary;
import org.ioopm.calculator.ast.atom.Constant;


public class Log extends Unary {
    
    public Log(SymbolicExpression exp) {
        super(exp);
    }

    @Override 
    public String getName() {
        return "log";
    }

    @Override 
    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression arg = this.getExp().eval(vars); 
        if (arg.isConstant()) {
            return new Constant(Math.log(arg.getValue()));
        } else {
            return new Log(arg);
        }
    }
}
