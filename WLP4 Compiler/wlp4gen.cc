#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <utility>
#include <map>
#include "generator.h"

// the procedure identifier for the current active symbol table
std::string cursymtab;

// contextual sensitive analysis by traversing the parse tree
void traverse(Node * parsetree, int &count,
                std::map<std::string, std::pair<std::vector<std::string>, std::map<std::string, std::pair<std::string, int>>>> &symtab) {
    for (int i = 0; i < parsetree->children.size(); i++) {
        if (parsetree->children[i]->type == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
            // add wain into procedure symbol table
            if (symtab.count("wain") == 0) {
                // need to obtain the type of parameters of wain
                std::string type = parsetree->children[i]->children[3]->children[0]->children[0]->lexeme;
                if (parsetree->children[i]->children[3]->children[0]->children.size() == 2) {
                    // in this case have type INT STAR
                    type = type + parsetree->children[i]->children[3]->children[0]->children[1]->lexeme;
                }
                
                // this pair contains the vector of parameter in the signature of wain
                // and the map of variables declared
                std::pair<std::vector<std::string>, std::map<std::string, std::pair<std::string, int>>> sigsym;
                // push the type of the first parameter into the vector
                (sigsym.first).push_back(type);
                
                type = parsetree->children[i]->children[5]->children[0]->children[0]->lexeme;
                if (parsetree->children[i]->children[5]->children[0]->children.size() == 2) {
                    // in this case have type INT STAR
                    type = type + parsetree->children[i]->children[5]->children[0]->children[1]->lexeme;
                }
                (sigsym.first).push_back(type);
                
                
                // recurse the node to handle dcl, dcls, statements, and expr
                cursymtab = "wain";
                symtab["wain"] = sigsym;
                
                count = 0;
                
                traverse(parsetree->children[i], count, symtab);
            }
            else {
                // a wain main function already exist, report error
                throw SemanError("ERROR: double definition of procedure");
            }
        }
        else if ((parsetree->children[i]->type).substr(0,10) == "procedure ") {
            // must be a declaration of new procedure
            std::string procedure_id = parsetree->children[i]->children[1]->lexeme;
            if (symtab.count(procedure_id) == 0) {
                // this pair contains the vector of parameters for the procedure
                // and the map of variables declared
                std::pair<std::vector<std::string>, std::map<std::string, std::pair<std::string, int>>> sigsym;
                // create an entry for the current procedure in the table of symbol tables
                symtab[procedure_id] = sigsym;
                // active symbol table is one associated with the current procedure
                cursymtab = procedure_id;
                
                // reset the frame pointer
                count = 0;
                
                // traverse to store the parameters, variables, check statements, and returned expr
                traverse(parsetree->children[i], count, symtab);
            }
            else {
                // double declaration of procedure
                throw SemanError("ERROR: double declaration of procedure");
            }
        }
        else if ((parsetree->children[i]->type).substr(0,9) == "paramlist") {
            // the first symbol on the right hand side of a production rule with LHS
            // paramlist must be dcl
            Node * param_dcl = parsetree->children[i]->children[0];
            
            // retreive the parameter information
            std::string type = param_dcl->children[0]->children[0]->lexeme;
            if (param_dcl->children[0]->children.size() == 2) {
                // must be type int *
                type = type + param_dcl->children[0]->children[1]->lexeme;
            }
            // add this type to signature
            symtab[cursymtab].first.push_back(type);
            
            
            traverse(parsetree->children[i], count, symtab);
        }
        else if (parsetree->children[i]->type == "dcl type ID") {
            // add the declared variable into symbol table
            std::string type = parsetree->children[i]->children[0]->children[0]->lexeme;
            if (parsetree->children[i]->children[0]->children.size() == 2) {
                // in this case have type INT STAR
                type = type + parsetree->children[i]->children[0]->children[1]->lexeme;
            }
            std::string id = parsetree->children[i]->children[1]->lexeme;
            if (symtab[cursymtab].second.count(id) == 0) {
                std::pair<std::string, int> new_variable;
                new_variable.first = type;
                new_variable.second = count;
                symtab[cursymtab].second[id] = new_variable;
                count = count - 4;
            }
            else {
                // double declaration report error
                throw SemanError("ERROR: double declaration of variable");
            }
        }
        else if ((parsetree->children[i]->type).substr(0,4) == "expr" || (parsetree->children[i]->type).substr(0,4) == "test" ||
                    (parsetree->children[i]->type).substr(0,4) == "term") {
            // recurse to check variable being used
            traverse(parsetree->children[i],count, symtab);
        }
        else if (parsetree->children[i]->type == "factor ID" || 
                    parsetree->children[i]->type == "lvalue ID") {
            std::string var = parsetree->children[i]->children[0]->lexeme;
            // if variable not declared at this point, report error
            if (symtab[cursymtab].second.count(var) == 0) {
                throw SemanError("ERROR: calling undeclared variable");
            }
        }
        else if ((parsetree->children[i]->type).substr(0, 16) == "factor ID LPAREN") {
            std::string pro = parsetree->children[i]->children[0]->lexeme;
            if (symtab[cursymtab].second.count(pro) == 0) {
                // try to find variable with the same name as the procedure, if doesn't
                // exist, then try to call declared procedure with the same name
                if (symtab.count(pro) == 0) {
                    // if there is no varaible nor procedure with identifier pro,
                    // throw error
                    throw SemanError("ERROR: calling undeclared procedure");
                }
                else if (pro == "wain") {
                    throw SemanError("ERROR: call on wain");
                }
            }
            else {
                // trying to call procedure with variable, error
                throw SemanError("ERROR: variable & procedure");
            }
            traverse(parsetree->children[i], count, symtab);
        }
        else {
            traverse(parsetree->children[i], count, symtab);
        }
    }
}

void printSymTab(std::map<std::string, std::pair<std::vector<std::string>, std::map<std::string, std::pair<std::string, int>>>> & symtab) {
    for (auto it = symtab.begin(); it != symtab.end(); it++) {
        std::cerr << it->first << ":";
        for (int i = 0; i < it->second.first.size(); i++) {
            std::cerr << ' ' << it->second.first[i];
        }
        std::cerr << std::endl;
        for (auto var = it->second.second.begin(); var != it->second.second.end(); var++) {
            std::cerr << var->first << ' ' << var->second.first << ' ' << var->second.second << std::endl;
        }
    }
}

// given a node in the parse tree, representing an expression, store the associated
// type of the expression in the data_type component
// originally had a std::stack<std::vector<std::string>> &parameters argument
void typeOf(Node * express,
                    std::map<std::string, std::pair<std::vector<std::string>, std::map<std::string, std::pair<std::string, int>>>> &symtab) {
    if (express->type == "NUM") {
        // must be an integer
        express->data_type = "int";
        //return "int";
    }
    else if (express->type == "NULL") {
        // must be a pointer
        express->data_type = "int*";
        //return "int*";
    }
    else if (express->type == "ID") {
        std::string var = express->lexeme;
        // if variable not declared at this point, report error
        /*if (symtab[cursymtab].second.count(var) == 0) {
            throw SemanError("ERROR: calling undeclared variable");
        }*/
        
        // the type of the ID can be extracted from the
        // symbol table
        express->data_type = symtab[cursymtab].second[var].first;
        //return symtab[cursymtab].second[var];
    }
    else if (express->type == "lvalue LPAREN lvalue RPAREN" ||
                express->type == "factor LPAREN expr RPAREN") {
        // parentheses do not change type
        // determine the data type of the child
        typeOf(express->children[1], symtab);
        // at this point the data_type component of the lvalue child should stores
        // its associated type
        express->data_type = express->children[1]->data_type;
        //return typeOf(express->children[1], parameters, symtab);
    }
    else if (express->type == "factor AMP lvalue") {
        // here attempting to take the address of an int, only possible
        // if lvalue has type int
        typeOf(express->children[1], symtab);
        express->data_type = "int*";
        
        /*if (typeOf(express->children[1], parameters, symtab) == "int") {
            return "int*";
        }
        else {
            // attempting to take the address of non-int
            throw SemanError("ERROR: pointer of non-int");
        }*/
    }
    else if (express->type == "factor STAR factor" || express->type == "lvalue STAR factor") {
        // trying to dereference, only possible if factor is a pointer
        typeOf(express->children[1], symtab);
        express->data_type = "int";
        /*if (typeOf(express->children[1], parameters, symtab) == "int*") {
            return "int";
        }
        else {
            // attempting to dereference non-pointer type
            throw SemanError("ERROR: dereference error");
        }*/
    }
    else if (express->type == "factor NEW INT LBRACK expr RBRACK") {
        // here expr must be an int and we are creating a pointer
        typeOf(express->children[3], symtab);
        express->data_type= "int*";
        /*if (typeOf(express->children[3], parameters, symtab) == "int") {
            return "int*";
        }
        else {
            throw SemanError("ERROR: cannot create pointer for non-int");
        }*/
    }
    else if (express->type == "term term STAR factor" || 
                express->type == "term term SLASH factor" ||
                express->type == "term term PCT factor") {
        // handle multiplication, division, modulo all at once
        typeOf(express->children[0], symtab);
        typeOf(express->children[2], symtab);
        express->data_type = "int";
        // check the two operands are both type int
        /*if (typeOf(express->children[0], parameters, symtab) == "int" && 
                typeOf(express->children[2], parameters, symtab) == "int") {
            return "int";
        }
        else {
            throw SemanError("ERROR: operands type error");
        }*/
    }
    else if (express->type == "expr expr PLUS term") {
        /*std::string first = typeOf(express->children[0], parameters, symtab);
        std::string second = typeOf(express->children[2], parameters, symtab);*/
        typeOf(express->children[0], symtab);
        typeOf(express->children[2], symtab);
        // at this point the expr and term child should store their associated type
        std::string first = express->children[0]->data_type;
        std::string second = express->children[2]->data_type;
        
        // the only invalid addition is addition of type pointers
        if (first == "int*" && second == "int*") {
            //throw SemanError("ERROR: addition of two pointers");
        }
        else {
            // either int* + int or int + int*
            if (first == "int*" || second == "int*") {
                express->data_type = "int*";
                //return "int*";
            }
            else {
                // must be int + int
                express->data_type = "int";
                //return "int";
            }
        }
    }
    else if (express->type == "expr expr MINUS term") {
        /*std::string first = typeOf(express->children[0], parameters, symtab);
        std::string second = typeOf(express->children[2], parameters, symtab);*/
        
        typeOf(express->children[0], symtab);
        typeOf(express->children[2], symtab);
        
        // at this point the expr and term child should store their associated type
        std::string first = express->children[0]->data_type;
        std::string second = express->children[2]->data_type;
        
        // int* - int* = int
        if (first == "int*" && second == "int*") {
            express->data_type = "int";
            //return "int";
        }
        else if (first == "int*") {
            // then term must have type int
            express->data_type = "int*";
            //return "int*";
        }
        else if (second == "int*") {
            // then expr must have type int, this is the only invalid case
            //throw SemanError("ERROR: int - int*");
        }
        else {
            // must be the integer subtraction case
            express->data_type = "int";
            //return "int";
        }
    }
    else if (express->type == "factor ID LPAREN RPAREN" || 
                express->type == "factor ID LPAREN arglist RPAREN") {
        express->data_type = "int";
        /*std::string pro = express->children[0]->lexeme;
        if (symtab[cursymtab].second.count(pro) == 0) {
            // try to find variable with the same name as the procedure, if doesn't
            // exist, then try to call declared procedure with the same name
            if (symtab.count(pro) == 0) {
                // if there is no varaible nor procedure with identifier pro,
                // throw error
                throw SemanError("ERROR: calling undeclared procedure");
            }
            else if (pro == "wain") {
                throw SemanError("ERROR: call on wain");
            }
        }
        else {
            // trying to call procedure with variable, error
            throw SemanError("ERROR: variable & procedure");
        }
        
        if (express->type == "factor ID LPAREN arglist RPAREN") {
            // check arglist match the parameters of the procedure
            if (parameters.empty() != true || parameters.top().empty() != true) {
                // must be the case that calling procedure with procedure call
                std::vector<std::string> newcall;
                parameters.push(newcall);
            }
            typeOf(express->children[2], parameters, symtab);
            
            // at this point parameters vector should store the type of all arguments
            if (parameters.top().size() != symtab[pro].first.size()) {
                // number of aruments given incorrect
                for (int i = 0; i < parameters.top().size(); i++) {
                    std::cout << parameters.top()[i] << std::endl;
                }
                throw SemanError("ERROR: wrong number of arguments");
            }
            for (int i = 0; i < symtab[pro].first.size(); i++) {
                if (parameters.top()[i] != symtab[pro].first[i]) {
                    throw SemanError("ERROR: argument type error");
                }
            }
            
            if (parameters.empty() != true) {
                parameters.top().clear();
                parameters.top().shrink_to_fit();
                parameters.pop();
            }
        }
        else {
            if (symtab[pro].first.empty() != true) {
                throw SemanError("ERROR: no argument provided");
            }
        }*/
        
        // if checked the procedure arguments without error, return int
        //return "int";
    }
    else if (express->type == "factor ID" || express->type == "lvalue ID" ||
                express->type == "factor NUM" || express->type == "factor NULL" ||
                express->type == "term factor" || express->type == "expr term" ||
                express->type == "procedures main") {
        
        typeOf(express->children[0], symtab);
        
        if (express->type != "procedures main") {
            // at this point the first child of the node should already store its associated
            // type
            express->data_type = express->children[0]->data_type;
        }
        
        //return typeOf(express->children[0], parameters, symtab);            
    }
    else if (express->type == "test expr EQ expr" || express->type == "test expr NE expr" ||
                express->type == "test expr LT expr" || express->type == "test expr LE expr" ||
                express->type == "test expr GE expr" || express->type == "test expr GT expr" ||
                express->type == "statement lvalue BECOMES expr SEMI") {
        // want the expr children to store their associated type
        typeOf(express->children[0], symtab);
        typeOf(express->children[2], symtab);
    }
    else if (express->type == "procedures procedure procedures" ||
                express->type == "statements statements statement") {
        typeOf(express->children[0], symtab);
        typeOf(express->children[1], symtab);
    }
    else if (express->type == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
        cursymtab = express->children[1]->lexeme;
        typeOf(express->children[7], symtab);
        typeOf(express->children[9], symtab);
    }
    else if (express->type == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
        cursymtab = "wain";
        typeOf(express->children[9], symtab);
        typeOf(express->children[11], symtab);
    }
    else if (express->type == "statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE") {
        typeOf(express->children[2], symtab);
        typeOf(express->children[5], symtab);
        typeOf(express->children[9], symtab);
    }
    else if (express->type == "statement WHILE LPAREN test RPAREN LBRACE statements RBRACE") {
        typeOf(express->children[2], symtab);
        typeOf(express->children[5], symtab);
    }
    else if (express->type == "statement PRINTLN LPAREN expr RPAREN SEMI") {
        typeOf(express->children[2], symtab);
    }
    else if (express->type == "statement DELETE LBRACK RBRACK expr SEMI") {
        typeOf(express->children[3], symtab);
    }
    else {
        // this node has no expesssions, do nothing
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// lable counters
int control_count = 0;
int control_count2 = 0;
int dele_count = 0;

// outputs the MIPS assembly instruction that loads the value of the variable
// into register 3
void code(Node * variable, 
        std::map<std::string, std::pair<std::vector<std::string>, std::map<std::string, std::pair<std::string, int>>>> &symtab) {
    std::string id = variable->lexeme;
    std::cout << "lw $3, " << symtab[cursymtab].second[id].second << "($29)" << std::endl;
}

// pushes a varialbe stored in register reg onto stack
void pushVar(int reg) {
    std::cout << "sw $" << reg << ", -4($30)" << std::endl;
    std::cout << "sub $30, $30, $4" << std::endl;
}

// pops a variable by loading from top of stack
void popVar(int reg) {
    std::cout << "add $30, $30, $4" << std::endl;
    std::cout << "lw $" << reg << ", -4($30)" << std::endl;
}

// given a lvalue, get the derived variable
std::string getVar(Node * node) {
    if (node->type == "ID") {
        return node->lexeme;
    }
    else {
        if (node->type == "lvalue LPAREN lvalue RPAREN") {
            return getVar(node->children[1]);
        }
        else {
            return getVar(node->children[0]);
        }
    }
}

// generates prologue for wain
void genWainPro(std::map<std::string, std::pair<std::vector<std::string>, std::map<std::string, std::pair<std::string, int>>>> &symtab) {
    // import external procedures
    std::cout << "; begin main prologue" << std::endl;
    std::cout << ".import print" << std::endl;
    std::cout << ".import init" << std::endl;
    std::cout << ".import new" << std::endl;
    std::cout << ".import delete" << std::endl;
    
    // define constant registers
    std::cout << "lis $4" << std::endl;
    std::cout << ".word 4" << std::endl;
    std::cout << "lis $10" << std::endl;
    std::cout << ".word print" << std::endl;
    std::cout << "lis $11" << std::endl;
    std::cout << ".word 1" << std::endl;
    
    // store wain parameters onto stack and set frame pointers
    std::cout << "sub $29 , $30 , $4 ; setup frame pointer" << std::endl;
    std::cout << "sw $1 , 0( $29 ) ; push first parameter" << std::endl;
    std::cout << "sub $30 , $30 , $4 ; update stack pointer" << std::endl;
    std::cout << "sw $2 , -4( $29 ) ; push second parameter" << std::endl;
    std::cout << "sub $30 , $30 , $4 ; update stack pointer" << std::endl;
    
    if (symtab["wain"].first[0] == "int") {
        pushVar(2); 
        // the first parameter of wain is not an array, set $2 to zero
        std::cout << "add $2, $0, $0" << std::endl;
    }
    
    // call init
    pushVar(31);
    std::cout << "lis $5" << std::endl;
    std::cout << ".word init" << std::endl;
    std::cout << "jalr $5" << std::endl;
    popVar(31);
    
    if (symtab["wain"].first[0] == "int") {
        popVar(2);
    }
    
    std::cout << "; end main prologue" << std::endl;
    
    // genExpress(tree, symtab);
}

void genWainEpi() {
    std::cout << "; begin main epilogue" << std::endl;
    std::cout << "add $30 ,$30 ,$4" << std::endl;
    std::cout << "add $30 ,$30 ,$4" << std::endl;
    std::cout << "jr $31" << std::endl;
}

// generates the code for the wlp4 program
void genCode(Node * tree,
                std::map<std::string, std::pair<std::vector<std::string>, std::map<std::string, std::pair<std::string, int>>>> &symtab) {
    if (tree->type == "ID") {
        // if is an ID push load it
        code(tree, symtab);
    }
    else if (tree->type == "NUM") {
        std::cout << "lis $3" << std::endl;
        std::cout << ".word " << tree->lexeme << std::endl;
    }
    else if (tree->type == "start BOF procedures EOF") {
        genCode(tree->children[1], symtab);
    }
    else if (tree->type == "procedures procedure procedures") {
        // note that we want to generate code for wain first, then for other procedures
        // and since for MIPS Assembly programs functions do not need to be defined
        // before called, it doesn't matter the order in which the other procedures are
        // generated
        // we know that from the way the parse tree is built, procedures -> wain must be 
        // the right most and bottom most procedure node, thus whenever the node
        // procedures -> procedure procedures is encountered, want to generate code for the
        // procedures child node before generating code for the procedure child node
        genCode(tree->children[1], symtab);
        genCode(tree->children[0], symtab);
    }
    else if (tree->type == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
        //std::string prev_pro = cursymtab;
        cursymtab = tree->children[1]->lexeme;
        
        // define label for the procedure
        std::cout << "F" << cursymtab << ":" << std::endl;
        
        // note that at this point, the arguments for the procedure should be
        // pushed onto stack already, although in the lecture notes we are
        // provided formulas for calculating offsets, but that means I need to
        // modify the implementation for every part that an offset is calculated
        // this is tedious so I am going with a different approach
        // I defined the offsets in the symbol table building step, the offsets were
        // defined such that the first parameter will have offset 0, second parameter
        // has offset 1, so on until the last parameter n will have offset n-1, and
        // the first local variable will have offset n and so on
        // given this order, we know that the frame pointer will point to the first local variable
        // or first register saved if no local variable, then the offset wrt. to this frame
        // pointer should be original offset + 4 * size of parameter list
        
        if (tree->children[3]->type == "params paramlist") {
            int size = symtab[cursymtab].first.size(); // this is the size of the procedure's parameters list
            
            int update = size * 4;
            
            // update the offset for every variable of the procedure
            for (auto it = symtab[cursymtab].second.begin(); it != symtab[cursymtab].second.end(); it++) {
                it->second.second = it->second.second + update;
            }
        }

        // set frame pointer when the procedure has no argument
        std::cout << "sub $29, $30, $4" << std::endl;
        
        // push local variables onto stack
        genCode(tree->children[6], symtab);
        
        // push registers
        std::cout << "sw $1, -4($30)" << std::endl;
		std::cout << "sub $30, $30, $4" << std::endl;

		std::cout << "sw $2, -4($30)" << std::endl;
		std::cout << "sub $30, $30, $4" << std::endl;

		std::cout << "sw $5, -4($30)" << std::endl;
		std::cout << "sub $30, $30, $4" << std::endl;

		std::cout << "sw $6, -4($30)" << std::endl;
		std::cout << "sub $30, $30, $4" << std::endl;

		std::cout << "sw $7, -4($30)" << std::endl;
		std::cout << "sub $30, $30, $4" << std::endl;
		
		
		// generate code for the procedure
		genCode(tree->children[7], symtab);
		genCode(tree->children[9], symtab);
		
		// restore registers
		std::cout << "lw $7, 0($30)" << std::endl;
		std::cout << "add $30, $30, $4" << std::endl;

		std::cout << "lw $6, 0($30)" << std::endl;
		std::cout << "add $30, $30, $4" << std::endl;

		std::cout << "lw $5, 0($30)" << std::endl;
		std::cout << "add $30, $30, $4" << std::endl;

		std::cout << "lw $2, 0($30)" << std::endl;
		std::cout << "add $30, $30, $4" << std::endl;

		std::cout << "lw $1, 0($30)" << std::endl;
		std::cout << "add $30, $30, $4" << std::endl;
		
		
		std::cout << "add $30, $29, $4" << std::endl;
		
		std::cout << "jr $31" << std::endl;
    }
    else if (tree->type == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
        cursymtab = "wain";
        genWainPro(symtab);
        genCode(tree->children[8], symtab);
        genCode(tree->children[9], symtab);
        genCode(tree->children[11], symtab);
        genWainEpi();
    }
    else if (tree->type == "dcls dcls dcl BECOMES NUM SEMI") {
        genCode(tree->children[0], symtab);
        // need to store the NUM being assigned
        std::cout << "lis $3" << std::endl;
        std::cout << ".word " << tree->children[3]->lexeme << std::endl;
        std::cout << "sw $3, -4($30)" << std::endl;
        std::cout << "sub $30, $30, $4" << std::endl;
    }
    else if (tree->type == "dcls dcls dcl BECOMES NULL SEMI") {
        genCode(tree->children[0], symtab);
        std::cout << "add $3, $11, $0" << std::endl;
        std::cout << "sw $3, -4($30)" << std::endl;
        std::cout << "sub $30, $30, $4" << std::endl;
    }
    else if (tree->type == "statements statements statement") {
        genCode(tree->children[0], symtab);
        genCode(tree->children[1], symtab);
    }
    else if (tree->type == "statement PRINTLN LPAREN expr RPAREN SEMI") {
        genCode(tree->children[2], symtab);
        // println expects output to be print in $1
        std::cout << "add $1, $3, $0" << std::endl;
        // call println, push $31 onto stack
        pushVar(31);
        std::cout << "jalr $10" << std::endl;
        // after calling println, restore $31
        popVar(31);
    }
    else if (tree->type == "statement lvalue BECOMES expr SEMI") {
        // assign value to variable
        genCode(tree->children[2], symtab);
        
        // two cases, lvalue can derive ID or STAR factor
        if (tree->children[0]->type == "lvalue STAR factor") {
            // push RHS onto stack
            pushVar(3);
            // evaluate LHS
            genCode(tree->children[0], symtab);
            popVar(5);
            std::cout << "sw $5, 0($3)" << std::endl;
        }
        else {
            std::string var = getVar(tree->children[0]);
            std::cout << "sw $3, " << symtab[cursymtab].second[var].second << "($29)";
            std::cout << std::endl;
        }
        
    }
    else if (tree->type == "statement DELETE LBRACK RBRACK expr SEMI") {
        dele_count = dele_count + 1;
        genCode(tree->children[3], symtab);
        // if the pointer derived from expr is null, do not delete
        std::cout << "beq $3, $11, skipDelete" << dele_count << std::endl;
        // call delete
        std::cout << "add $1, $3, $0" << std::endl;
        pushVar(31);
        std::cout << "lis $5" << std::endl;
        std::cout << ".word delete" << std::endl;
        std::cout << "jalr $5" << std::endl;
        popVar(31);
        std::cout << "skipDelete" << dele_count << ":" << std::endl;
    }
    else if (tree->type == "statement WHILE LPAREN test RPAREN LBRACE statements RBRACE") {
        // increment control flow counter
        control_count = control_count + 1;
        // loop label
        std::cout << "loop" << control_count << ":" << std::endl;
        genCode(tree->children[2], symtab);
        std::cout << "beq $3, $0, endwhile" << control_count << std::endl;
        
        int count_before = control_count;
        genCode(tree->children[5], symtab);
        std::cout << "beq $0, $0, loop" << count_before << std::endl;
        
        std::cout << "endwhile" << count_before << ":" << std::endl;
    }
    else if (tree->type == "statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE") {
        genCode(tree->children[2], symtab);
        
        control_count2 = control_count2 + 1;
        std::cout << "beq $3, $0, else" << control_count2 << std::endl;
        
        int count_before = control_count2;
        // statement 1
        genCode(tree->children[5], symtab);
        
        std::cout << "beq $0, $0, endif" << count_before << std::endl;
        std::cout << "else" << count_before << ":" << std::endl;
        
        // statement 2
        genCode(tree->children[9], symtab);
        std::cout << "endif" << count_before << ":" << std::endl;
    }
    else if (tree->type == "test expr LT expr" || tree->type == "test expr EQ expr" ||
                tree->type == "test expr NE expr" || tree->type == "test expr LE expr" ||
                tree->type == "test expr GE expr" || tree->type == "test expr GT expr") {
        // get the operands
        genCode(tree->children[0], symtab);
        pushVar(3);
        genCode(tree->children[2], symtab);
        popVar(5);
        
        // evaluate
        if (tree->type == "test expr LT expr") {
            if (tree->children[0]->data_type == "int") {
                std::cout << "slt $3, $5, $3" << std::endl;
            }
            else {
                std::cout << "sltu $3, $5, $3" << std::endl;
            }
        }
        else if (tree->type == "test expr EQ expr") {
            if (tree->children[0]->data_type == "int") {
                std::cout << "slt $6, $3, $5" << std::endl;
                std::cout << "slt $7, $5, $3" << std::endl;
            }
            else {
                std::cout << "sltu $6, $3, $5" << std::endl;
                std::cout << "sltu $7, $5, $3" << std::endl;
            }
            std::cout << "add $3, $6, $7" << std::endl;
            std::cout << "sub $3, $11, $3" << std::endl;
        }
        else if (tree->type == "test expr NE expr") {
            if (tree->children[0]->data_type == "int") {
                std::cout << "slt $6, $3, $5" << std::endl;
                std::cout << "slt $7, $5, $3" << std::endl;
            }
            else {
                std::cout << "sltu $6, $3, $5" << std::endl;
                std::cout << "sltu $7, $5, $3" << std::endl;
            }
            std::cout << "add $3, $6, $7" << std::endl;
        }
        else if (tree->type == "test expr LE expr") {
            // if EQ is true, currently $3 stores 1, $6 stores 0
            if (tree->children[0]->data_type == "int") {
                std::cout << "slt $6, $5, $3" << std::endl;
                std::cout << "slt $7, $3, $5" << std::endl;
            }
            else {
                std::cout << "sltu $6, $5, $3" << std::endl;
                std::cout << "sltu $7, $3, $5" << std::endl;
            }
            
            std::cout << "add $3, $6, $7" << std::endl;
            std::cout << "sub $3, $11, $3" << std::endl;
            // if LT is true, $6 currently stores 1, $3 stores 0
            std::cout << "add $3, $3, $6" << std::endl;
        }
        else if (tree->type == "test expr GE expr") {
            // if EQ is true, currently $3 stores 1, $6 stores 0
            if (tree->children[0]->data_type == "int") {
                std::cout << "slt $6, $3, $5" << std::endl;
                std::cout << "slt $7, $5, $3" << std::endl;
            }
            else {
                std::cout << "sltu $6, $3, $5" << std::endl;
                std::cout << "sltu $7, $5, $3" << std::endl;
            }
           
            std::cout << "add $3, $6, $7" << std::endl;
            std::cout << "sub $3, $11, $3" << std::endl;
            // if GT is true, $6 currently stores 1, $3 stores 0
            std::cout << "add $3, $3, $6" << std::endl;
        }
        else if (tree->type == "test expr GT expr") {
            if (tree->children[0]->data_type == "int") {
                std::cout << "slt $3, $3, $5" << std::endl;
            }
            else {
                std::cout << "sltu $3, $3, $5" << std::endl;
            }
            
        }
    }
    else if (tree->type == "factor ID LPAREN RPAREN") {
        std::cout << "sw $29, -4($30)" << std::endl;
		std::cout << "sub $30, $30, $4" << std::endl;
		
		std::cout << "sw $31, -4($30)" << std::endl;
		std::cout << "sub $30, $30, $4" << std::endl;
		
		// call the procedure
		std::string pro_id = tree->children[0]->lexeme;
		std::cout << "lis $5" << std::endl;
		std::cout << ".word " << "F" << pro_id << std::endl;
		std::cout << "jalr $5" << std::endl;
		
		std::cout << "lw $31, 0($30)" << std::endl;
		std::cout << "add $30, $30, $4" << std::endl;
		
		std::cout << "lw $29, 0($30)" << std::endl;
		std::cout << "add $30, $30, $4" << std::endl;
    }
    else if (tree->type == "factor ID LPAREN arglist RPAREN") {
        std::cout << "sw $29, -4($30)" << std::endl;
		std::cout << "sub $30, $30, $4" << std::endl;
		
		std::cout << "sw $31, -4($30)" << std::endl;
		std::cout << "sub $30, $30, $4" << std::endl;
		
		// push arguments onto stack
		genCode(tree->children[2], symtab);
		
		// call the procedure
		std::string pro_id = tree->children[0]->lexeme;
		std::cout << "lis $5" << std::endl;
		std::cout << ".word " << "F" << pro_id << std::endl;
		std::cout << "jalr $5" << std::endl;
		
		// pop arguments off the stack
		int size = symtab[pro_id].first.size(); // size of the procedure's parameter list
		int pop_num = size * 4;
		std::cout << "lis $5" << std::endl;
		std::cout << ".word " << pop_num << std::endl;
		std::cout << "add $30, $30, $5" << std::endl;
		
		std::cout << "lw $31, 0($30)" << std::endl;
		std::cout << "add $30, $30, $4" << std::endl;
		
		std::cout << "lw $29, 0($30)" << std::endl;
		std::cout << "add $30, $30, $4" << std::endl;
    }
    else if (tree->type == "arglist expr") {
        // determine the associated argument and push onto stack
        genCode(tree->children[0], symtab);
        pushVar(3);
    }
    else if (tree->type == "arglist expr COMMA arglist") {
        // determine the associated argument and push onto stack
        genCode(tree->children[0], symtab);
        pushVar(3);
        // recures on remaining arguments
        genCode(tree->children[2], symtab);
    }
    else if (tree->type == "factor ID" || tree->type == "factor NUM" ||
                tree->type == "expr term" || tree->type == "term factor" ||
                tree->type == "procedures main") {
        genCode(tree->children[0], symtab);            
    }
    else if (tree->type == "factor NULL") {
        std::cout << "add $3, $11, $0" << std::endl;
    }
    else if (tree->type == "factor AMP lvalue") {
        // two cases possible, lvalue can derive ID or STAR factor
        // recurse on lvalue to handle the cases
        genCode(tree->children[1], symtab);
    }
    else if (tree->type == "factor LPAREN expr RPAREN") {
        genCode(tree->children[1], symtab);
    }
    else if (tree->type == "factor STAR factor") {
        genCode(tree->children[1], symtab);
        std::cout << "lw $3, 0($3)" << std::endl;
    }
    else if (tree->type == "factor NEW INT LBRACK expr RBRACK") {
        genCode(tree->children[3], symtab);
        std::cout << "add $1, $3, $0" << std::endl;
        pushVar(31);
        std::cout << "lis $5" << std::endl;
        std::cout << ".word new" <<  std::endl;
        std::cout << "jalr $5" << std::endl;
        popVar(31);
        // check if allocation is successful, if no, set $3 to NULL
        std::cout << "bne $3, $0, 1" << std::endl;
        std::cout << "add $3, $11, $0" << std::endl;
    }
    else if (tree->type == "lvalue ID") {
        // we handled the case that statement -> ... lvalue ... previously, so
        // we know the only case that we will reach here is when generating code
        // for factor AMP lavlue
        std::cout << "lis $3" << std::endl;
        std::string var = getVar(tree);
        int offset = symtab[cursymtab].second[var].second;
        std::cout << ".word " << offset << std::endl;
        std::cout << "add $3, $3, $29" << std::endl;
    }
    else if (tree->type == "lvalue STAR factor") {
        genCode(tree->children[1], symtab);
    }
    else if (tree->type == "expr expr PLUS term" || tree->type == "expr expr MINUS term" ||
                tree->type == "term term STAR factor" || tree->type == "term term SLASH factor" ||
                tree->type == "term term PCT factor") {
        
        // get the two operands
        genCode(tree->children[0], symtab);
        pushVar(3);
        genCode(tree->children[2], symtab);
        popVar(5);
        
        if (tree->type == "expr expr PLUS term") {
            std::string opre1 = tree->children[0]->data_type;
            std::string opre2 = tree->children[2]->data_type;
            
            if (opre1 == "int*" && opre2 == "int") {
                std::cout << "mult $3, $4" << std::endl;
                std::cout << "mflo $3" << std::endl;
            }
            else if (opre1 == "int" && opre2 == "int*") {
                std::cout << "mult $5, $4" << std::endl;
                std::cout << "mflo $5" << std::endl;
            }
            else {
                // do nothing
            }
            std::cout << "add $3, $5, $3" << std::endl;
        }
        else if (tree->type == "expr expr MINUS term") {
            std::string opre1 = tree->children[0]->data_type;
            std::string opre2 = tree->children[2]->data_type;
            
            if (opre1 == "int*" && opre2 == "int") {
                std::cout << "mult $3, $4" << std::endl;
                std::cout << "mflo $3" << std::endl;
                
            }
            
            std::cout << "sub $3, $5, $3" << std::endl;
            
            if (opre1 == "int*" && opre2 == "int*") {
                std::cout << "div $3, $4" << std::endl;
                std::cout << "mflo $3" << std::endl;
            }
            
        }
        else if (tree->type == "term term STAR factor") {
            std::cout << "mult $5, $3" << std::endl;
            std::cout << "mflo $3" << std::endl;
        }
        else if (tree->type == "term term SLASH factor") {
            std::cout << "div $5, $3" << std::endl;
            std::cout << "mflo $3" << std::endl;
        }
        else if (tree->type == "term term PCT factor") {
            std::cout << "div $5, $3" << std::endl;
            // note that since taking the remainder, mfhi
            std::cout << "mfhi $3" << std::endl;
        }
    }
}

void printTree(Node * tree) {
    if (tree->children.empty() == true) {
        std::cout << tree->type << " " << tree->lexeme << " " << tree->data_type << std::endl;
    }
    else {
        std::cout << tree->type << " " << tree->lexeme << " " << tree->data_type << std::endl;
        for (int i = 0; i < tree->children.size(); i++) {
            printTree(tree->children[i]);
        }
    }
}

int main()
{
    std::string line;
    std::getline(std::cin, line);
    
    // create the root node for the parse tree
    std::vector<Node *> child_temp;
    Node * root = new Node(line, "", "", child_temp);
    try {
        // create the parse tree
        pre_to_tree(root, countWord(line) - 1);
        
        std::map<std::string, std::pair<std::vector<std::string>, std::map<std::string, std::pair<std::string, int>>>> tables;
        
        // traverse the parse tree to store information about variable types
        int counter = 0;
        traverse(root, counter, tables);
        
        typeOf(root->children[1], tables);
        
        
        // print the symbol table
        //printSymTab(tables);
        
        //printTree(root);
        
        genCode(root, tables);
        
        delete root;
    }
    
    catch (SemanError &f) {
        std::cerr << f.what() << std::endl;
        delete root;
        return 1;
    }
    
    return 0;
}
