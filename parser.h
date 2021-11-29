#include "scanner.h"

class Parser{

  private:
    Scanner* scanner;
    Token* lToken;
    Token* lastToken;
    SymbolTable* globalST;
    SymbolTable* currentST;
    int err; // Flag de erro
    int errSintatic; // Flag para erro sintático
    int recToken; // Flag de troca de token

    void advance();
    int match(int, int *, int);
    void error(int, int, string);
    void initSymbolTable();
    void returnST();

    // Métodos da Gramatica
    void program();
    void mainClass();
    void statement();
    void expression();
    void expressionPredict();
    void classDeclaration();
    void varDeclaration();
    void methodDeclaration();
    void type();
    int isOP();
    int isType();
    int isTypeExtended();

    // Tratamento da parte semântica
    int verVariable(int, int, int);
    int verType();
    
  public:

    Parser(string);
    void run();
};