package org.ioopm.calculator.ast.unary;

import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.Unary;
import org.ioopm.calculator.ast.atom.Constant;

public class Negation extends Unary {

    public Negation(SymbolicExpression exp) {
        super(exp);
    }    

    @Override 
    public String getName() {
        return "-";
    }

    @Override 
    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression arg = this.getExp().eval(vars); 
        if (arg.isConstant()) {
            return new Constant(-(arg.getValue()));
        } else {
            return new Negation(arg);
        }
    }
}
