#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include "parser.h"
using std::string;
using std::istream;
using std::ostream;

int main() {
    std::string productions = R"(start BOF procedures EOF
procedures procedure procedures
procedures main
procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
params
params paramlist
paramlist dcl
paramlist dcl COMMA paramlist
type INT
type INT STAR
dcls
dcls dcls dcl BECOMES NUM SEMI
dcls dcls dcl BECOMES NULL SEMI
dcl type ID
statements
statements statements statement
statement lvalue BECOMES expr SEMI
statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE
statement WHILE LPAREN test RPAREN LBRACE statements RBRACE
statement PRINTLN LPAREN expr RPAREN SEMI
statement DELETE LBRACK RBRACK expr SEMI
test expr EQ expr
test expr NE expr
test expr LT expr
test expr LE expr
test expr GE expr
test expr GT expr
expr term
expr expr PLUS term
expr expr MINUS term
term factor
term term STAR factor
term term SLASH factor
term term PCT factor
factor ID
factor NUM
factor NULL
factor LPAREN expr RPAREN
factor AMP lvalue
factor STAR factor
factor NEW INT LBRACK expr RBRACK
factor ID LPAREN RPAREN
factor ID LPAREN arglist RPAREN
arglist expr
arglist expr COMMA arglist
lvalue ID
lvalue STAR factor
lvalue LPAREN lvalue RPAREN
)";
    std::string parsetable = R"(72 RPAREN reduce 45
112 paramlist shift 1
85 LPAREN shift 2
40 factor shift 3
3 EQ reduce 47
74 EQ reduce 46
21 INT shift 4
100 EQ reduce 48
106 STAR shift 5
86 STAR shift 5
88 STAR shift 5
95 SEMI shift 6
51 SEMI shift 7
118 NULL shift 8
128 STAR reduce 11
2 NULL shift 8
108 GT shift 9
57 RETURN reduce 16
6 INT reduce 13
7 INT reduce 12
19 factor shift 10
71 factor shift 10
14 term shift 11
44 term shift 11
17 term shift 11
25 term shift 11
13 term shift 11
9 term shift 11
117 BECOMES reduce 14
94 LPAREN reduce 11
100 GT reduce 48
3 GT reduce 47
74 GT reduce 46
85 expr shift 12
108 GE shift 13
74 GE reduce 46
3 GE reduce 47
100 GE reduce 48
128 PRINTLN reduce 11
108 EQ shift 14
2 STAR shift 5
118 STAR shift 5
106 NEW shift 15
86 NEW shift 15
88 NEW shift 15
123 RPAREN shift 16
108 LT shift 17
23 procedure shift 18
70 term shift 11
31 PLUS shift 19
32 PLUS shift 19
33 PLUS shift 19
34 PLUS shift 19
35 PLUS shift 19
36 PLUS shift 19
94 WHILE reduce 11
29 COMMA shift 20
94 dcls shift 21
70 ID shift 22
0 BOF shift 23
5 NULL shift 8
56 LPAREN shift 24
108 LE shift 25
101 COMMA shift 26
121 SEMI shift 27
83 DELETE reduce 18
59 RETURN reduce 15
57 WHILE reduce 16
18 main shift 28
82 DELETE reduce 19
127 STAR reduce 39
102 STAR reduce 40
74 LE reduce 46
30 STAR reduce 36
8 STAR reduce 37
112 dcl shift 29
90 STAR reduce 43
93 STAR reduce 42
97 STAR reduce 38
116 STAR reduce 41
73 NUM shift 30
14 expr shift 31
44 expr shift 32
17 expr shift 33
25 expr shift 34
13 expr shift 35
9 expr shift 36
37 EOF reduce 4
87 RBRACE shift 37
26 INT shift 4
24 INT shift 4
5 STAR shift 5
59 STAR reduce 15
130 DELETE reduce 17
68 DELETE reduce 20
131 DELETE reduce 21
59 WHILE reduce 15
11 LT reduce 28
61 LT reduce 30
60 LT reduce 29
129 LPAREN shift 38
121 PLUS shift 19
46 NEW shift 15
52 NEW shift 15
66 NEW shift 15
4 ID reduce 9
38 LPAREN shift 38
85 STAR shift 5
18 procedures shift 39
85 NUM shift 30
40 NULL shift 8
61 RPAREN reduce 30
60 RPAREN reduce 29
11 RPAREN reduce 28
59 ID reduce 15
59 IF reduce 15
57 RBRACE reduce 16
61 NE reduce 30
60 NE reduce 29
11 NE reduce 28
77 IF reduce 15
76 IF reduce 15
115 STAR shift 40
78 ID reduce 15
78 IF reduce 15
74 RBRACK reduce 46
3 RBRACK reduce 47
100 RBRACK reduce 48
128 LPAREN reduce 11
77 ID reduce 15
76 ID reduce 15
15 INT shift 41
113 STAR shift 40
114 STAR shift 40
20 type shift 42
22 RPAREN reduce 35
24 type shift 42
26 type shift 42
129 WHILE shift 43
108 NE shift 44
11 LE reduce 28
70 factor shift 10
105 NEW shift 15
104 NEW shift 15
80 term shift 11
80 expr shift 12
113 LPAREN shift 38
114 LPAREN shift 38
117 RPAREN reduce 14
115 LPAREN shift 38
61 LE reduce 30
60 LE reduce 29
63 BECOMES shift 45
90 PCT reduce 43
116 PCT reduce 41
30 PCT reduce 36
8 PCT reduce 37
127 PCT reduce 39
102 PCT reduce 40
93 PCT reduce 42
97 PCT reduce 38
47 MINUS reduce 32
48 MINUS reduce 33
49 MINUS reduce 34
10 MINUS reduce 31
10 STAR reduce 31
11 STAR shift 46
61 STAR shift 46
60 STAR shift 46
70 LPAREN shift 2
46 factor shift 47
52 factor shift 48
66 factor shift 49
14 STAR shift 5
44 STAR shift 5
17 STAR shift 5
25 STAR shift 5
13 STAR shift 5
9 STAR shift 5
80 arglist shift 50
12 RPAREN reduce 44
19 NEW shift 15
71 NEW shift 15
45 NUM shift 51
61 GE reduce 30
60 GE reduce 29
11 GE reduce 28
10 RBRACK reduce 31
47 RBRACK reduce 32
48 RBRACK reduce 33
49 RBRACK reduce 34
90 COMMA reduce 43
116 COMMA reduce 41
61 GT reduce 30
60 GT reduce 29
11 SLASH shift 52
11 GT reduce 28
61 SLASH shift 52
60 SLASH shift 52
30 COMMA reduce 36
8 COMMA reduce 37
127 COMMA reduce 39
102 COMMA reduce 40
47 STAR reduce 32
48 STAR reduce 33
49 STAR reduce 34
80 ID shift 22
93 COMMA reduce 42
97 COMMA reduce 38
70 NULL shift 8
59 RBRACE reduce 15
46 AMP shift 53
52 AMP shift 53
66 AMP shift 53
23 main shift 28
94 ID reduce 11
78 RBRACE reduce 15
94 IF reduce 11
94 PRINTLN reduce 11
23 INT shift 54
5 ID shift 22
46 STAR shift 5
52 STAR shift 5
66 STAR shift 5
113 DELETE shift 55
114 DELETE shift 55
116 SLASH reduce 41
129 STAR shift 40
90 SLASH reduce 43
112 RPAREN reduce 5
54 WAIN shift 56
127 SLASH reduce 39
102 SLASH reduce 40
93 SLASH reduce 42
97 SLASH reduce 38
30 SLASH reduce 36
8 SLASH reduce 37
19 STAR shift 5
71 STAR shift 5
118 NUM shift 30
109 statement shift 57
2 NUM shift 30
73 LPAREN shift 2
53 LPAREN shift 38
47 COMMA reduce 32
48 COMMA reduce 33
49 COMMA reduce 34
14 factor shift 10
44 factor shift 10
17 factor shift 10
25 factor shift 10
13 factor shift 10
9 factor shift 10
10 COMMA reduce 31
100 STAR reduce 48
3 STAR reduce 47
74 STAR reduce 46
115 DELETE shift 55
109 PRINTLN shift 58
78 WHILE reduce 15
77 RBRACE reduce 15
76 RBRACE reduce 15
113 statement shift 57
114 statement shift 57
22 PLUS reduce 35
115 statement shift 57
105 AMP shift 53
104 AMP shift 53
77 WHILE reduce 15
76 WHILE reduce 15
128 dcls shift 59
118 term shift 11
2 term shift 11
19 term shift 60
71 term shift 61
128 WHILE reduce 11
19 LPAREN shift 2
71 LPAREN shift 2
128 RETURN reduce 11
57 ID reduce 16
57 IF reduce 16
23 procedures shift 62
2 LPAREN shift 2
118 LPAREN shift 2
21 dcl shift 63
19 NULL shift 8
71 NULL shift 8
78 PRINTLN reduce 15
77 PRINTLN reduce 15
76 PRINTLN reduce 15
2 expr shift 64
118 expr shift 65
11 PCT shift 66
21 ID reduce 15
21 IF reduce 15
6 LPAREN reduce 13
7 LPAREN reduce 12
113 lvalue shift 67
114 lvalue shift 67
14 NULL shift 8
44 NULL shift 8
17 NULL shift 8
25 NULL shift 8
13 NULL shift 8
9 NULL shift 8
118 AMP shift 53
115 lvalue shift 67
2 AMP shift 53
16 SEMI shift 68
80 factor shift 10
94 INT reduce 11
5 NEW shift 15
73 STAR shift 5
57 PRINTLN reduce 16
80 NEW shift 15
74 LT reduce 46
3 LT reduce 47
46 NUM shift 30
52 NUM shift 30
66 NUM shift 30
10 PCT reduce 31
3 LE reduce 47
100 LE reduce 48
85 NEW shift 15
21 WHILE reduce 15
47 PCT reduce 32
48 PCT reduce 33
49 PCT reduce 34
100 LT reduce 48
21 RBRACE reduce 15
22 COMMA reduce 35
129 PRINTLN shift 58
22 SEMI reduce 35
100 NE reduce 48
3 NE reduce 47
74 NE reduce 46
40 AMP shift 53
14 ID shift 22
44 ID shift 22
17 ID shift 22
25 ID shift 22
13 ID shift 22
9 ID shift 22
94 RETURN reduce 11
46 NULL shift 8
52 NULL shift 8
66 NULL shift 8
19 AMP shift 53
71 AMP shift 53
118 ID shift 22
2 ID shift 22
54 ID shift 69
80 NULL shift 8
129 RETURN shift 70
61 PLUS reduce 30
60 PLUS reduce 29
11 PLUS reduce 28
59 DELETE reduce 15
22 PCT reduce 35
31 MINUS shift 71
32 MINUS shift 71
33 MINUS shift 71
34 MINUS shift 71
35 MINUS shift 71
36 MINUS shift 71
105 NUM shift 30
104 NUM shift 30
85 arglist shift 72
22 RBRACK reduce 35
6 DELETE reduce 13
7 DELETE reduce 12
109 RETURN shift 73
88 LPAREN shift 2
12 PLUS shift 19
53 ID shift 74
105 LPAREN shift 2
104 LPAREN shift 2
106 LPAREN shift 2
86 LPAREN shift 2
22 MINUS reduce 35
90 BECOMES reduce 43
93 BECOMES reduce 42
97 BECOMES reduce 38
127 BECOMES reduce 39
102 BECOMES reduce 40
30 BECOMES reduce 36
8 BECOMES reduce 37
70 NUM shift 30
116 BECOMES reduce 41
19 NUM shift 30
71 NUM shift 30
73 expr shift 75
94 STAR reduce 11
126 LBRACE shift 76
125 LBRACE shift 77
100 SLASH reduce 48
3 SLASH reduce 47
96 LBRACE shift 78
74 SLASH reduce 46
55 LBRACK shift 79
40 STAR shift 5
128 INT reduce 11
2 NEW shift 15
22 LPAREN shift 80
127 NE reduce 39
102 NE reduce 40
93 NE reduce 42
97 NE reduce 38
90 NE reduce 43
116 NE reduce 41
116 MINUS reduce 41
38 STAR shift 40
78 RETURN reduce 15
112 INT shift 4
30 NE reduce 36
8 NE reduce 37
30 MINUS reduce 36
8 MINUS reduce 37
118 NEW shift 15
127 MINUS reduce 39
102 MINUS reduce 40
93 MINUS reduce 42
97 MINUS reduce 38
90 MINUS reduce 43
77 RETURN reduce 15
76 RETURN reduce 15
39 EOF reduce 1
6 WHILE reduce 13
7 WHILE reduce 12
28 EOF reduce 2
59 PRINTLN reduce 15
113 RBRACE shift 81
114 RBRACE shift 82
127 RBRACK reduce 39
102 RBRACK reduce 40
93 RBRACK reduce 42
97 RBRACK reduce 38
109 lvalue shift 67
90 RBRACK reduce 43
116 RBRACK reduce 41
115 RBRACE shift 83
30 RBRACK reduce 36
8 RBRACK reduce 37
21 PRINTLN reduce 15
14 LPAREN shift 2
44 LPAREN shift 2
17 LPAREN shift 2
25 LPAREN shift 2
13 LPAREN shift 2
9 LPAREN shift 2
112 params shift 84
12 COMMA shift 85
75 MINUS shift 71
22 SLASH reduce 35
103 RPAREN reduce 8
5 NUM shift 30
67 BECOMES shift 86
11 RBRACK reduce 28
61 COMMA reduce 30
60 COMMA reduce 29
61 RBRACK reduce 30
60 RBRACK reduce 29
11 COMMA reduce 28
74 PCT reduce 46
3 PCT reduce 47
100 PCT reduce 48
75 PLUS shift 19
5 LPAREN shift 2
112 type shift 42
85 ID shift 22
3 BECOMES reduce 47
74 BECOMES reduce 46
100 BECOMES reduce 48
85 term shift 11
116 SEMI reduce 41
64 MINUS shift 71
93 SEMI reduce 42
97 SEMI reduce 38
65 MINUS shift 71
90 SEMI reduce 43
80 LPAREN shift 2
75 SEMI shift 87
130 RBRACE reduce 17
68 RBRACE reduce 20
131 RBRACE reduce 21
14 AMP shift 53
44 AMP shift 53
17 AMP shift 53
25 AMP shift 53
13 AMP shift 53
9 AMP shift 53
82 RBRACE reduce 19
79 RBRACK shift 88
127 SEMI reduce 39
102 SEMI reduce 40
30 SEMI reduce 36
8 SEMI reduce 37
83 RBRACE reduce 18
68 PRINTLN reduce 20
131 PRINTLN reduce 21
130 PRINTLN reduce 17
47 PLUS reduce 32
48 PLUS reduce 33
49 PLUS reduce 34
82 PRINTLN reduce 19
10 PLUS reduce 31
109 WHILE shift 43
83 PRINTLN reduce 18
65 PLUS shift 19
64 PLUS shift 19
77 STAR reduce 15
76 STAR reduce 15
10 SEMI reduce 31
78 STAR reduce 15
73 NULL shift 8
46 LPAREN shift 2
52 LPAREN shift 2
66 LPAREN shift 2
4 STAR shift 89
47 SEMI reduce 32
48 SEMI reduce 33
49 SEMI reduce 34
116 RPAREN reduce 41
40 NUM shift 30
30 RPAREN reduce 36
8 RPAREN reduce 37
127 RPAREN reduce 39
102 RPAREN reduce 40
50 RPAREN shift 90
93 RPAREN reduce 42
97 RPAREN reduce 38
105 NULL shift 8
104 NULL shift 8
90 RPAREN reduce 43
109 DELETE shift 55
18 procedure shift 18
100 PLUS reduce 48
74 PLUS reduce 46
3 PLUS reduce 47
85 factor shift 10
10 NE reduce 31
47 NE reduce 32
48 NE reduce 33
49 NE reduce 34
94 DELETE reduce 11
116 EQ reduce 41
100 COMMA reduce 48
30 GT reduce 36
8 GT reduce 37
3 COMMA reduce 47
93 GT reduce 42
97 GT reduce 38
127 GT reduce 39
102 GT reduce 40
1 RPAREN reduce 6
74 COMMA reduce 46
27 RBRACE shift 91
90 GE reduce 43
93 GE reduce 42
97 GE reduce 38
116 GE reduce 41
83 WHILE reduce 18
30 GE reduce 36
8 GE reduce 37
127 GE reduce 39
102 GE reduce 40
21 LPAREN reduce 15
6 STAR reduce 13
7 STAR reduce 12
82 WHILE reduce 19
68 WHILE reduce 20
131 WHILE reduce 21
130 WHILE reduce 17
61 PCT shift 66
60 PCT shift 66
80 NUM shift 30
20 dcl shift 29
99 RPAREN shift 92
128 ID reduce 11
57 LPAREN reduce 16
80 RPAREN shift 93
92 LBRACE shift 94
14 NUM shift 30
44 NUM shift 30
17 NUM shift 30
25 NUM shift 30
13 NUM shift 30
9 NUM shift 30
90 GT reduce 43
116 GT reduce 41
45 NULL shift 95
128 IF reduce 11
81 ELSE shift 96
57 DELETE reduce 16
64 RPAREN shift 97
73 AMP shift 53
62 EOF shift 98
21 STAR reduce 15
118 factor shift 10
105 factor shift 10
104 factor shift 10
2 factor shift 10
29 RPAREN reduce 7
100 RPAREN reduce 48
105 ID shift 22
104 ID shift 22
82 LPAREN reduce 19
20 INT shift 4
88 factor shift 10
130 LPAREN reduce 17
106 factor shift 10
86 factor shift 10
68 LPAREN reduce 20
131 LPAREN reduce 21
26 dcl shift 99
19 ID shift 22
71 ID shift 22
74 RPAREN reduce 46
111 RPAREN shift 100
3 RPAREN reduce 47
83 LPAREN reduce 18
24 dcl shift 101
5 factor shift 102
20 paramlist shift 103
107 LPAREN shift 104
43 LPAREN shift 105
58 LPAREN shift 106
12 MINUS shift 71
11 MINUS reduce 28
61 MINUS reduce 30
60 MINUS reduce 29
59 INT shift 4
93 LT reduce 42
97 LT reduce 38
90 LT reduce 43
30 LT reduce 36
8 LT reduce 37
127 LT reduce 39
102 LT reduce 40
91 INT reduce 3
116 LT reduce 41
93 LE reduce 42
97 LE reduce 38
108 PLUS shift 19
127 LE reduce 39
102 LE reduce 40
30 LE reduce 36
8 LE reduce 37
129 ID shift 74
116 LE reduce 41
129 IF shift 107
90 LE reduce 43
46 ID shift 22
52 ID shift 22
66 ID shift 22
70 NEW shift 15
88 NULL shift 8
106 NULL shift 8
86 NULL shift 8
10 EQ reduce 31
47 EQ reduce 32
48 EQ reduce 33
49 EQ reduce 34
80 STAR shift 5
6 PRINTLN reduce 13
7 PRINTLN reduce 12
22 LT reduce 35
59 LPAREN reduce 15
59 type shift 42
73 NEW shift 15
22 LE reduce 35
109 STAR shift 40
105 expr shift 108
104 expr shift 108
93 PLUS reduce 42
97 PLUS reduce 38
90 PLUS reduce 43
116 PLUS reduce 41
30 PLUS reduce 36
8 PLUS reduce 37
127 PLUS reduce 39
102 PLUS reduce 40
10 GT reduce 31
47 GT reduce 32
48 GT reduce 33
49 GT reduce 34
109 IF shift 107
109 ID shift 74
80 AMP shift 53
108 MINUS shift 71
10 GE reduce 31
47 GE reduce 32
48 GE reduce 33
49 GE reduce 34
21 statements shift 109
109 LPAREN shift 38
59 dcl shift 63
128 DELETE reduce 11
40 ID shift 22
84 RPAREN shift 110
88 NUM shift 30
106 NUM shift 30
86 NUM shift 30
40 LPAREN shift 2
38 lvalue shift 111
22 BECOMES reduce 35
40 NEW shift 15
69 LPAREN shift 112
77 statements shift 113
76 statements shift 114
73 term shift 11
78 statements shift 115
53 STAR shift 40
57 STAR reduce 16
93 EQ reduce 42
97 EQ reduce 38
90 EQ reduce 43
30 EQ reduce 36
8 EQ reduce 37
127 EQ reduce 39
102 EQ reduce 40
85 AMP shift 53
47 LT reduce 32
48 LT reduce 33
49 LT reduce 34
10 LT reduce 31
117 COMMA reduce 14
18 INT shift 54
83 RETURN reduce 18
10 LE reduce 31
3 MINUS reduce 47
129 statement shift 57
74 MINUS reduce 46
22 NE reduce 35
10 RPAREN reduce 31
100 MINUS reduce 48
47 LE reduce 32
48 LE reduce 33
49 LE reduce 34
106 term shift 11
86 term shift 11
130 RETURN reduce 17
47 RPAREN reduce 32
48 RPAREN reduce 33
49 RPAREN reduce 34
70 AMP shift 53
88 term shift 11
68 RETURN reduce 20
131 RETURN reduce 21
82 RETURN reduce 19
105 term shift 11
104 term shift 11
82 STAR reduce 19
22 STAR reduce 35
83 STAR reduce 18
77 LPAREN reduce 15
76 LPAREN reduce 15
65 RBRACK shift 116
70 STAR shift 5
68 STAR reduce 20
131 STAR reduce 21
78 LPAREN reduce 15
130 STAR reduce 17
129 DELETE shift 55
121 MINUS shift 71
78 DELETE reduce 15
42 ID shift 117
77 DELETE reduce 15
76 DELETE reduce 15
113 ID shift 74
114 ID shift 74
113 IF shift 107
114 IF shift 107
115 ID shift 74
115 IF shift 107
38 ID shift 74
123 PLUS shift 19
124 PLUS shift 19
122 PLUS shift 19
47 SLASH reduce 32
48 SLASH reduce 33
49 SLASH reduce 34
10 SLASH reduce 31
41 LBRACK shift 118
14 NEW shift 15
44 NEW shift 15
17 NEW shift 15
25 NEW shift 15
13 NEW shift 15
9 NEW shift 15
105 STAR shift 5
104 STAR shift 5
105 test shift 119
104 test shift 120
5 AMP shift 53
122 MINUS shift 71
123 MINUS shift 71
124 MINUS shift 71
70 expr shift 121
100 SEMI reduce 48
73 ID shift 22
3 SEMI reduce 47
74 SEMI reduce 46
113 WHILE shift 43
114 WHILE shift 43
88 ID shift 22
106 ID shift 22
86 ID shift 22
22 GE reduce 35
115 WHILE shift 43
88 expr shift 122
106 expr shift 123
86 expr shift 124
120 RPAREN shift 125
119 RPAREN shift 126
22 GT reduce 35
129 lvalue shift 67
11 SEMI reduce 28
6 IF reduce 13
7 IF reduce 12
61 SEMI reduce 30
60 SEMI reduce 29
53 lvalue shift 127
6 ID reduce 13
7 ID reduce 12
88 AMP shift 53
89 ID reduce 10
110 LBRACE shift 128
106 AMP shift 53
86 AMP shift 53
83 IF reduce 18
83 ID reduce 18
82 IF reduce 19
130 IF reduce 17
82 ID reduce 19
68 IF reduce 20
131 IF reduce 21
31 RPAREN reduce 22
32 RPAREN reduce 23
33 RPAREN reduce 24
34 RPAREN reduce 25
35 RPAREN reduce 26
36 RPAREN reduce 27
21 type shift 42
6 RETURN reduce 13
7 RETURN reduce 12
59 statements shift 129
68 ID reduce 20
131 ID reduce 21
130 ID reduce 17
22 EQ reduce 35
11 EQ reduce 28
73 factor shift 10
61 EQ reduce 30
60 EQ reduce 29
85 NULL shift 8
115 PRINTLN shift 58
124 SEMI shift 130
122 SEMI shift 131
21 RETURN reduce 15
113 PRINTLN shift 58
114 PRINTLN shift 58
21 DELETE reduce 15
)";
    try {
        
        // a vector of production rules
        // each rule is represented by a pair of LHS and RHS, where LHS is a single
        // symbol and the RHS is a vector of symbols
        std::vector<Rule> rules;
        
        storeRules(rules, productions);
        
        // a vector of the LR(1) DFA states, each state contains a map of
        // transitions associated with the state
        std::vector<State *> dfa(132);
        
        storeTransit(dfa, parsetable);
        
        // stack for states
        std::stack<State *> states;
        // stack for symbols
        std::stack<Node *> symbols;
        
        // read the sequence of terminals and store them in a vector
        std::string input_kind;
        std::string input_lexeme;
        std::vector<Token> inputs;
        // to facilitate the parsing algorithm augment the inputs vector
        Token begin("BOF", "BOF");
        Token end("EOF", "EOF");
        inputs.push_back(begin);
        while (std::cin >> input_kind) {
            std::cin >> input_lexeme;
            Token curtoken(input_kind, input_lexeme);
            inputs.push_back(curtoken);
        }
        inputs.push_back(end);
        
        // call the parsing function
        parseAlgo(states, symbols, rules, dfa, inputs);
        
        rules.clear();
        rules.shrink_to_fit();
        int dfasize = dfa.size();
        for (int i = 0; i < dfasize; i++) {
            delete dfa[i];
        }
        dfa.clear();
        dfa.shrink_to_fit();
        inputs.clear();
    }
    catch (ParsingFailure &f) {
        std::cerr << f.what() << f.pos() << std::endl;
        return 1;
    }
    return 0;
}