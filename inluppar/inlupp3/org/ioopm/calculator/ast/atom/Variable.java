package org.ioopm.calculator.ast.atom;

import org.ioopm.calculator.ast.Atom;

public class Variable extends Atom {
    private String identifier;

    public Variable(String identifier) {
        this.identifier = identifier;
    }
}
