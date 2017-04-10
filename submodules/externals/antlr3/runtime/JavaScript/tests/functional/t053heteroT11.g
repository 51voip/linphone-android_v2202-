grammar t053heteroT11;
options {
    language=JavaScript;
    output=AST;
}
@header {
function V() {
    V.superclass.constructor.apply(this, arguments);
};

org.antlr.lang.extend(V, org.antlr.runtime.tree.CommonTree, {
    toString: function() {
        return this.getText() + "<V>";
    }
});
}
a : 'begin' -> 'begin'<V> ;
ID : 'a'..'z'+ ;
WS : (' '|'\n') {$channel=HIDDEN;} ;

