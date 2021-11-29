#include "parser.h"

Parser::Parser(string input) 
{
  currentST = globalST = new SymbolTable();
  initSymbolTable();
  
  scanner = new Scanner(input, globalST);
}

void Parser::advance() 
{
  Token *aux;
  
  if(recToken == 0) {
    lastToken = lToken;
    lToken = scanner->nextToken();
  } 
  else {
    aux = lastToken;
    lastToken = lToken;
    lToken = aux;
    if(recToken == 1)
      recToken = 2;
    else 
      recToken = 0;
  }
}

int Parser::match(int name, int *list, int tam) 
{
  int found = -1; // flag indicando se foi encontrado

  if(tam == 0){ // se tem apenas um token a ser olhado
    while((found == -1 || found == 0) && lToken->name != END_OF_FILE) {
      if(lToken->name == name || lToken->attribute == name){
        found = 1;
        advance();
        return found;
      }
      advance();
      if(found == -1) { // Processo para apontar um erro sintático e continuar procurando
        found = 0;
        error(0, name, " ");
      }
    }
  }
  else { // se há uma lista de tokens a serem conferidos
    // avance até encontrar o token de interesse
    while(found == -1 && lToken->name != END_OF_FILE) {
      for (int i = 0; i < tam; i++) { // procura na lista se o tipo casa com o token
        if(lToken->name == list[i] || lToken->attribute == list[i]){
          found = i;
          i = tam;
        }
      }
      advance();
    }
    return found;
  }
  return -1;
}

//Tabela de símbolos
void Parser::initSymbolTable(){

  Token* token;
  STEntry * st;

  token = new Token(BOOLEAN);
  st = new STEntry(token, "boolean", true);
  globalST->add(st);

  token = new Token(CLASS);
  st = new STEntry(token, "class", true);
  globalST->add(st);

  token = new Token(ELSE);
  st = new STEntry(token, "else", true);
  globalST->add(st);

  token = new Token(EXTENDS);
  st = new STEntry(token, "extends", true);
  globalST->add(st);

  token = new Token(FALSE);
  st = new STEntry(token, "false", true);
  globalST->add(st);

  token = new Token(IF);
  st = new STEntry(token, "if", true);
  globalST->add(st);

  token = new Token(INT);
  st = new STEntry(token, "int", true);
  globalST->add(st);

  token = new Token(LENGTH);
  st = new STEntry(token, "length", true);
  globalST->add(st);

  token = new Token(MAIN);
  st = new STEntry(token, "main", true);
  globalST->add(st);

  token = new Token(NEW);
  st = new STEntry(token, "new", true);
  globalST->add(st);

  token = new Token(PUBLIC);
  st = new STEntry(token, "public", true);
  globalST->add(st);

  token = new Token(RETURN);
  st = new STEntry(token, "return", true);
  globalST->add(st);

  token = new Token(STATIC);
  st = new STEntry(token, "static", true);
  globalST->add(st);

  token = new Token(STRING);
  st = new STEntry(token, "String", true);
  globalST->add(st);

  token = new Token(SYSOUTPRINT);
  st = new STEntry(token, "System.out.println", true);
  globalST->add(st);

  token = new Token(THIS);
  st = new STEntry(token, "this", true);
  globalST->add(st);

  token = new Token(TRUE);
  st = new STEntry(token, "true", true);
  globalST->add(st);

  token = new Token(VOID);
  st = new STEntry(token, "void", true);
  globalST->add(st);

  token = new Token(WHILE);
  st = new STEntry(token, "while", true);
  globalST->add(st);
}

//Iniciar programa
void Parser::run(){ // verificar e alterar

  currentST = new SymbolTable();
  err = 0;
  errSintatic = 0;
  recToken = 0;
  
  advance();
  program();

  cout << "Finished compilation!!" << endl;
  if(err)
    cout << "Status: Problems detected!" << endl;
  else
    cout << "Status: succeed compilation!" << endl;

  returnST();
}

//Retornar escopo
void Parser::returnST(){ // verificar e alterar

  SymbolTable* table = currentST;
  table->clear();
  currentST = currentST->getParent();
  delete table;
}

void Parser::error(int type, int expected, string msg)
{
  string tokenName[] = {"UNDEF", "ID", "INT_LITERAL", "OP", "SEP", "RESERVED_WORD", "&&","<", ">", "+", "-", "*", "/", "=", "==", "!=", "!", "(",")", "[", "]", "{", "}", ";",".", ",", "boolean", "class", "else", "extends", "false", "if", "int", "length", "main", "new", "public", "return", "static", "String", "System.out.println", "this", "true","void", "while", "EOF", "expression", "int, ID", "int, boolean, ID", "statment", "statement after ID"};

  err = 1; // indicando que houve erro na compilação final
  // Só printa um erro sintático
  if(type == 0 && errSintatic == 0) { // Sintático
    errSintatic = 1;
    cout << "Line "<< scanner->getLine() << ": ";
    cout << "syntatic error near token: " << scanner->printLine() << endl;
    cout << "Expected: " << tokenName[expected] << endl;
  }
  else if(lToken->name != END_OF_FILE) {
    cout << "Line "<< scanner->getLine() << ": ";
    cout << "semantic error: " << msg << endl;
    scanner->printLine();
  }
  
}

// Parte nova
void Parser::program()
{
  mainClass(); // mainClass
  while(lToken->name != END_OF_FILE){
    classDeclaration(); // classDeclaration
  }
  match(END_OF_FILE, NULL, 0); // EOF
}

void Parser::mainClass()
{
  match(CLASS, NULL, 0); // Class
  match(ID, NULL, 0);    // nome_classe
  match(L_BRACES, NULL, 0);// {
  match(PUBLIC, NULL, 0);  // public
  match(STATIC, NULL, 0);  // static
  match(VOID, NULL, 0);    // void
  match(MAIN, NULL, 0);    // main
  match(L_PAREN, NULL, 0); // (
  match(STRING, NULL, 0);  // String
  match(L_BRACKETS, NULL, 0); // [
  match(R_BRACKETS, NULL, 0); // ]
  match(ID, NULL, 0);         // nome_variável
  match(R_PAREN, NULL, 0);    // )
  match(L_BRACES, NULL, 0);   // {
  statement();                // statement
  match(R_BRACES, NULL, 0);   // }
  match(R_BRACES, NULL, 0);   // }
}

void Parser::statement() 
{
  int *list;
  int sentinel = -1;
  list = new int[1];

  do {
    if(lToken->name == L_BRACES || sentinel == 0) { // {
      sentinel = -1;
      advance();
      statement();     // statement
      match(R_BRACES, NULL, 0); // }
    }
    else if(lToken->name == IF || sentinel == 1) { // if
      sentinel = -1;
      advance();
      match(L_PAREN, NULL, 0); // (
      expression();   // expression
      match(R_PAREN, NULL, 0); // )
      statement();    // statement
      match(ELSE, NULL, 0);    // else
      statement();    // statement
    }
    else if(lToken->name == WHILE || sentinel == 2) { // while
      sentinel = -1;
      advance();
      match(L_PAREN, NULL, 0); // (
      expression();   // expression
      match(R_PAREN, NULL, 0); // )
      statement();    // statement
    }
    else if(lToken->name == SYSOUTPRINT || sentinel == 3) { // System.out.println
      sentinel = -1;
      advance();
      match(L_PAREN, NULL, 0); // (
      expression();   // expression
      match(R_PAREN, NULL, 0); // )
      match(SEMICOL, NULL, 0); // ;
    }
    else if(lToken->attribute == ID || sentinel == 4) { // variável
      sentinel = -1;
      advance();
      do{
        if(lToken->name == RECEIVE) { // =
          sentinel = -1;
          advance();
          expression();               // expression
          match(SEMICOL, NULL, 0);             // ;
        }
        else if(lToken->name == L_BRACKETS) { // [
          sentinel = -1;
          advance();
          expression();      // expression
          match(R_BRACKETS, NULL, 0); // ]
          match(RECEIVE, NULL, 0);    // =
          expression();      // expression
          match(SEMICOL, NULL, 0);    // ;
        }
        else { // tratamento de erros
          error(0, STATEMENT_AFTER, " ");
          delete list;
          list = new int[2];
          list[0] = RECEIVE;
          list[1] = L_BRACKETS;
          sentinel = match(-1, list, 2);
        }
      }while(sentinel != -1 && lToken->name != END_OF_FILE); 
    }
    else { // tratamento de erros
      error(0, STATEMENT, " ");
      delete list;
      list = new int[5];
      list[0] = L_BRACES;
      list[1] = IF;
      list[2] = WHILE;
      list[3] = SYSOUTPRINT;
      list[4] = ID;
      sentinel = match(-1, list, 5);
    }
  }while(sentinel != -1 && lToken->name != END_OF_FILE);
  delete list;
}

void Parser::expression()
{
  int *list;
  int sentinel = -1;
  list = new int[1];

  do {
    if(lToken->name == INT_LITERAL || sentinel == 0) { // int_literal
      sentinel = -1;
      advance();
      expressionPredict(); // expressionPredict
    }
    else if(lToken->name == TRUE || sentinel == 1) { // true
      sentinel = -1;
      advance();
      expressionPredict(); // expressionPredict
    }
    else if(lToken->name == FALSE || sentinel == 2) { // false
      sentinel = -1;
      advance();
      expressionPredict(); // expressionPredict
    }
    else if(lToken->name == ID || sentinel == 3) { // id
      sentinel = -1;
      advance(); 
      expressionPredict(); // expressionPredict
    }
    else if(lToken->name == THIS || sentinel == 4) { // this
      sentinel = -1;
      advance();
      expressionPredict(); // expressionPredict
    }
    else if(lToken->name == NEW || sentinel == 5) { // new
      sentinel = -1;
      advance();
      do {
        if(lToken->name == INT || sentinel == 0) { // int
          sentinel = -1;
          advance();
          match(L_BRACKETS, NULL, 0); // [
          expression();               // expression
          match(R_BRACKETS, NULL, 0); // ]
        }
        else if(lToken->name == ID || sentinel == 1) { // ID
          sentinel = -1;
          advance();
          match(L_PAREN, NULL, 0); // (
          match(R_PAREN, NULL, 0); // )
        }
        else { // Tratamento de erro
          error(0, EXPRESSION_INT_ID, " ");
          delete list;
          list = new int[2];
          list[0] = INT;
          list[1] = ID;
          sentinel = match(-1, list, 2);
        }
      }while(sentinel != -1 && lToken->name != END_OF_FILE);
    }
    else if(lToken->name == NOT || sentinel == 6) { // !
      sentinel = -1;
      advance();
      expression(); // expression
    }
    else if(lToken->name == L_PAREN || sentinel == 7) { // (
      sentinel = -1;
      advance();
      expression();            // expression
      match(R_PAREN, NULL, 0); // )
      expressionPredict();     // expressionPredict
    }
    else { // Tratamento de erro
      error(0, EXPRESSION, " ");
      delete list;
      list = new int[8];
      list[0] = INT_LITERAL;
      list[1] = TRUE;
      list[2] = FALSE;
      list[3] = ID;
      list[4] = THIS;
      list[5] = NEW;
      list[6] = NOT;
      list[7] = L_PAREN;
      sentinel = match(-1, list, 8);
    }
  }while(sentinel != -1 && lToken->name != END_OF_FILE);
  delete list;
}
// Auxiliador do expression para terminais específicos
void Parser::expressionPredict()
{
  if(isOP()) {    // OP
    advance();
    expression(); // expression
  }
  else if(lToken->name == L_BRACKETS) { // [
    advance();
    expression();               // expression
    match(R_BRACKETS, NULL, 0); // ]
    expressionPredict();
  }
  else if(lToken->name == DOT) {    // .
    advance();
    if(lToken->name == LENGTH) {    // length
      advance();
      expressionPredict();
    }
    else if(lToken->name == ID) {     // ID
      advance();
      match(L_PAREN, NULL, 0);        // (
      if(lToken->name != R_PAREN) {
        expression();                 // expression
        while(lToken->name == COMMA) {//, 
          advance();
          expression();               // expression
        }
      }
      match(R_PAREN, NULL, 0);        // )
      expressionPredict();
    }
  }
}

void Parser::classDeclaration() {
  
  match(CLASS, NULL, 0); // class
  match(ID, NULL, 0);    // nome_classe
  if(lToken->name == EXTENDS) {// extends
    advance();
    match(ID, NULL, 0); // nome_classe
  }

  match(L_BRACES, NULL, 0);     // {
  while(isType())
    varDeclaration();           // varDecl *
  while(lToken->name == PUBLIC)
    methodDeclaration();        // methodDeclaration *
  match(R_BRACES, NULL, 0);     // }
}

void Parser::varDeclaration() {
  type();         // type
  match(ID, NULL, 0);      // ID
  match(SEMICOL, NULL, 0); // ;
}

void Parser::methodDeclaration() {
  match(PUBLIC, NULL, 0);  // public
  type();         // type
  match(ID, NULL, 0);      // ID
  match(L_PAREN, NULL, 0); // (
  if(isType()) {  
    type();       // type
    match(ID, NULL, 0);    // ID
    while(lToken->name == COMMA) { // ,
      advance();
      type();     // type
      match(ID, NULL, 0);  // ID 
    }
  }
  match(R_PAREN, NULL, 0); // )

  match(L_BRACES, NULL, 0);// { 
  while(isTypeExtended()) {
    varDeclaration();// varDeclaration *
  }

  while(lToken->name != RETURN && lToken->name != END_OF_FILE) { 
    statement();  // statement *
  }
  match(RETURN, NULL, 0);  // return
  expression();   // expression
  match(SEMICOL, NULL, 0); // ;
  match(R_BRACES, NULL, 0);// }
}

void Parser::type() 
{
  int *list;
  int sentinel = -1;
  list = new int[1];
  do {
    if(lToken->name == INT) { // int
      sentinel = -1;
      advance();
      if(lToken->name == L_BRACKETS) { // [
        advance();
        match(R_BRACKETS, NULL, 0); // ]
      }
    }
    else if(lToken->name == BOOLEAN) { // boolean
      sentinel = -1;
      advance();
    }
    else if(lToken->name == ID) { // nome variável
      sentinel = -1;
      advance(); // Verificar tratamento depois
    }
    else { // tratamento de erro
      error(0, TYPE, " ");
      delete list;
      list = new int[3];
      list[0] = INT;
      list[1] = BOOLEAN;
      list[2] = ID;
      match(-1, list, 3);
    }
  }while(sentinel != -1 && lToken->name != END_OF_FILE);
}

// Se for uma operação retorna 1, 0 caso contrário
int Parser::isOP() // && < > == != + - * /
{
  if(lToken->name == AND || lToken->name == LESS_T || lToken->name == GREATER_T || lToken->name == EQUAL || lToken->name == DIF || lToken->name == PLUS || lToken->name == MINUS || lToken->name == TIMES || lToken->name == DIV)
    return 1;
  return 0;
}

// Se for um tipo retorna 1, 0 caso contrário
int Parser::isType()
{
  if(lToken->name == INT || lToken->name == BOOLEAN || lToken->name == ID) // mudar com o id
    return 1;
  return 0;
}

int Parser::isTypeExtended()
{
  if(isType()) {
    advance();
    recToken = 1;
    if(lastToken->name != ID) {
      advance();
      return 1;
    }
    else if(lToken->name == ID) {
      advance();
      return 1;
    }
    advance();
  }
  return 0;
}
int Parser::verType()
{
  if(lastToken->name == INT || lastToken->name == BOOLEAN || lastToken->name == ID || lastToken->name == R_BRACKETS) {
    return 1;
  }
  return 0;
}

int Parser::verVariable(int type, int attribute_return, int check)
{
  STEntry *s;
  s = currentST->get(lToken->lexeme);

  if(!s) {
    if(check)
      return 0;
    if(verType()) {
      if(lastToken->name == R_BRACKETS)
        lToken->attribute = INT;
      else
        lToken->attribute = lastToken->name;
      s = new STEntry(lToken, lToken->lexeme);
      currentST->add(s);
    }
  }
  else {
    error(1, -1, "This variable was already declared!");
  }
}