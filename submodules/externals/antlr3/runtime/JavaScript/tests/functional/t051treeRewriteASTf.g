grammar t051treeRewriteASTf;
options {
    language=JavaScript;
    output=AST;
}
a : ID ;
ID : 'a'..'z'+ ;
INT : '0'..'9'+;
WS : (' '|'\\n') {$channel=HIDDEN;} ;
