# Interpretador para Tiny

Esse projeto contém a implementação de um interpretador para a linguagem Tiny.
Essa é uma linguagem simples, utilizada para exercitar os conceitos necessários
para a disciplina de linguagens de programação do CEFET-MG.
Essa implementação pode ser usada como referência para o desenvolvimento do
primeiro trabalho prático dessa disciplina
(https://dcc.ufmg.br/~rimsa/page/classes/decom009/#homeworks).

## Características da linguagem

Tiny é uma linguagem interpretada de brinquedo com uma sintaxe e semântica
simples.
* A linguagem possui quatro tipos de comandos: comando condicional (**if**),
comando de repetição (**while**), comando de atribuição (**id = expr**) e
comando de saída (**output expr**).
* Esses comandos podem ser combinados para formar blocos de comandos.
Um programa em Tiny começa com a palavra-reservada **program** seguida de um
bloco de comandos.
* Identificadores (variáveis) começam com uma letra ou underscore (_) e podem
ser seguidos de letras, dígitos e underscore (_).
Esses armazenam apenas números inteiros.
* A linguagem permite avaliação de expressões lógicas simples em comandos
condicionais e de repetição.
* As expressões lógicas suportadas são: igual (**==**), diferente (**!=**),
menor (**<**), maior (**>**), menor igual (**<=**), maior igual (**>=**).
Não existe forma de conectar multiplas expressões lógicas com E/OU.
* A linguagem suporta constantes númericas inteiras e leitura de um valor
númerico inteiro do teclado (**read**).
* Expressões artiméticas são suportadas sobre números inteiros: adição (**+**),
subtração (**-**), multiplicação (**&ast;**), divisão (**/**) e
resto da divisão (**%**). Expressões artiméticas compostas devem usar,
necessariamente, identificadores auxiliares.
* A linguagem possui comentários de uma linha a partir do símbolo tralha (#).

Um exemplo de linguagem é dado a seguir (*somatorio.tiny*).

```
# calcula o somatório de números obtidos pela entrada
program
    sum = 0;
    i = read;
    while i > 0 do
        sum = sum + i;
        i = read;
    done;
    output sum;
```

Esse programa produz o somatório de números inteiros enquanto o usuário
entrar com valores maiores que zero.
Ao final, é exibido o valor do somatório.
Por exemplo, para os números 4, 8, 15, 16, 23, 42, o interpretador produz o
somatório 108.

    $ tinyi somatorio.tiny
    4
    8
    15
    16
    23
    42
    0
    108

## Implementação do interpretador

A implementação do interpretador pode ser dividida em três fases:
[analisador léxico](#analisador-léxico),
[analisador sintático](#analisador-sintático),
[interpretador](#interpretador).
Cada uma dessas fases será detalhada a seguir.

### Analisador léxico

O analisador léxico é responsável por separar os *tokens* da linguagem.
*Tokens* são os menores elementos que podem ser formados por um programa.
Para o programa de exemplo, os *tokens* obtidos em ordem são: **program**,
**sum**, **=**, **0**, **;**, **i**, **=**, **read**, **;** e assim por diante.
Note que, nessa linguagem, espaços, novas linhas, tabulações e comentários não
são elementos léxicos, ou seja, não formam *tokens*.

Para consultar a implementação separada do analisador léxico, basta fazer
*checkout* do *branch* **lexico**.

    $ git checkout lexico

#### Lexema

O lexema é uma estrutura que carrega um *token* e o tipo desse *token*.
Opcionalmente, um lexema pode carregar informações adicionais, porém essas não
serão utilizadas no escopo da implementação dessa linguagem.

```Java
public class Lexeme {
    public String token;
    public TokenType type;
}
```

O lexema possui seus membros públicos para facilitar sua utilização pelo
analisador léxico.
O *token* é uma string com o elemento formado, e *type* é o tipo do *token*.
Os tipos possíveis são listados pela enumeração *TokenType* que inclui símbolos
da linguagem (ex.: **+**, **;**, ...), palavras-reservadas (ex.: **if**,
**while**, ...), além de alguns tipos especiais (ex.: *token* inválido,
constante númerica, ...).

```Java
public enum TokenType {
	// Specials
	UNEXPECTED_EOF,
	INVALID_TOKEN,
	END_OF_FILE,

	// Symbols
	SEMICOLON,     // ;
	ASSIGN,        // =

	// Logic operators
	EQUAL,         // ==
	NOT_EQUAL,     // !=
	LOWER,         // <
	LOWER_EQUAL,   // <=
	GREATER,       // >
	GREATER_EQUAL, // >=

	// Arithmetic operators
	ADD,           // +
	SUB,           // -
	MUL,           // *
	DIV,           // /
	MOD,           // %

	// Keywords
	PROGRAM,       // program
	WHILE,         // while
	DO,            // do
	DONE,          // done
	IF,            // if
	THEN,          // then
	ELSE,          // else
	OUTPUT,        // output
	TRUE,          // true
	FALSE,         // false
	READ,          // read
	NOT,           // not

	// Others
	NUMBER,        // number
	VAR            // variable
}
```

Os três primeiros são tipos especiais usados para identificar as seguintes
situações: fim de arquivo inesperado (`UNEXPECTED_EOF`), *token* inválido
(`INVALID_TOKEN`), e fim de arquivo normal/esperado (`END_OF_FILE`).
Já os dois últimos tipos são usados para representar os tipos númericos
(`NUMBER`) e identificadores (`VAR`).
Todos os outros são designados para palavras-reservadas ou símbolos da
linguagem.
Para o programa de exemplo, os lexemas obtidos podem ser vistos na seção de
[resultado](#resultado).

#### Tabela de Símbolos

A tabela de símbolos (`SymbolTable`) é uma estrutura auxiliar utilizada para
facilitar o casamento de um *token* com seu tipo.
A tabela de símbolos é um dicionário que mapeia uma chave (**token**) com seu
valor (**TokenType**).
Essa tabela é pré-populada para todas as palavras-reservadas e símbolos da
linguagem.

| *Token* | *TokenType* |
| ------- | ----------- |
| ";"     | SEMICOLON   |
| "="     | ASSIGN      |
| "=="    | EQUAL       |
| "!="    | NOT_EQUAL   |
| "<"     | LOWER       |
| ...     | ...         |

Note que não é possível preencher essa tabela com todos os números existentes,
nem com todos os possíveis identificadores que possam vir a ser criados por um
programa.
Também não deve ser preenchida com os três tipos especiais (`UNEXPECTED_EOF`,
`INVALID_TOKEN`, `END_OF_FILE`).
Como a linguagem possui escopo global, uma vez populada essa tabela não será
modificada.
Além disso, qualquer consulta a esse mapa cujo *token* não seja
encontrado deve retornar o tipo identificador (`VAR`).

#### Autômato Finito Determinístico

Existem várias estratégias para formação de lexemas. Na implementação desse
interpretador será utilizado um autômato finito determinístico, também conhecido
como máquina de estados, conforme diagrama a seguir.

<p align="center">
  <img src="images/lexico.png?raw=true" width="500" height="300">
</p>

O autômato possui estados (nomeados de 1 a 8) e arestas (rotuladas com
símbolos, caracteres do programa).
Existe um único estado inicial, estado 1 representado pelo estado com uma aresta
entrante sem origem, e dois estados finais, estados 7 e 8 representados pela
oval dupla.
A transição é dada de um estado x (*e<sub>x</sub>*) para um estado y
(*e<sub>y</sub>*) sob um caracter do programa (*'s'*):
*T(e<sub>x</sub>, 's') = e<sub>y</sub>*.
O rótulo `ungetc` é um marcador especial que permite que um símbolo lido seja
devolvido ao buffer para que seja lido novamente posteriormente.
O analisador léxico implementa esse autômato.

```Java
public class LexicalAnalysis {

	private int line;
	private SymbolTable st;
	private PushbackInputStream input;

	...
}
```

O analisador léxico deve abrir o arquivo de entrada que se deseja interpretar.
Deve ser possível *devolver* um caracter para o buffer de leitura.
Em Java pode-se utilizar a classe `PushbackInputStream` com a função `unread`,
já em C/C++ pode-se utilizar o descritor `FILE\*` com a função `fungetc` nativa.
Assim, o analisador léxico deve manter: (1) um apontador para o número da linha
atual (`private int line;`); (2) uma instância com a tabela de símbolos
(`private SymbolTable st;`); e (3) o descritor do arquivo aberto
(`private PushbackInputStream input;`).

```Java
public Lexeme nextToken() {
	Lexeme lex = new Lexeme("", TokenType.END_OF_FILE);

	int state = 1;
	while (state != 7 && state != 8) {
		int c = getc();
		switch (state) {
			case 1:
				...
			case 2:
				...
			case 3:
				...
			case 4:
				...
			case 5:
				...
			case 6:
				...
			default:
				// unreacheable
		}
	}

	if (state == 7)
		lex.type = st.find(lex.token);

	return lex;
}
```

O autômato deve ser implementado na função `nextToken` do analisador léxico.
A cada chamada dessa função ativa-se o autômato que retorna próximo lexema do
programa de entrada.
Deve-se definir o lexema a ser formado inicialmente como vazio
(`Lexeme lex = new Lexeme("", TokenType.END_OF_FILE);`).
O autômato é iniciado no estado 1 (`int state = 1;`).
Enquanto não se atingir os estados finais 7 ou 8
(`while (state != 7 && state != 8) {`) deve-se ler um caracter da entrada
(`int c = getc();`).
Esse caracter pode ser ou não usado na formação do *token*.
Note que o caracter lido é do tipo inteiro (`int`), já que o fim do arquivo
é denotado pelo inteiro `-1`.
Esse inteiro deve ser convertido de volta para caracter quando for concatená-lo
ao *token*.
Os caminhos que levam ao estado 8 devem anotar explicitamente o tipo do *token*
formado, enquanto os caminhos que leval ao estado 7 devem consultar a tabela
de símbolos (`lex.type = st.find(lex.token);`).
A implementação de cada transição depende do estado em que a máquina se
encontra.

```Java
case 4:
	if (c == '=') {
		lex.token += (char) c;
		state = 7;
	} else if (c == -1) {
		lex.type = TokenType.UNEXPECTED_EOF;
		state = 8;
	} else {
		lex.type = TokenType.INVALID_TOKEN;
		state = 8;
	}

	break;
```

Por exemplo, o caminho e<sub>1</sub>->e<sub>4</sub>->e<sub>7</sub> é usado para
formação do *token* **!=**. 
No estado 1, ao ler o símbolo *'!'* deve-se adicioná-lo ao *token* e ir para o
estado 4.
A implementação do estado 4 é dada conforme código acima.
Ao ler o símbolo *'='* (`if (c == '=') {`) deve-se incluí-lo no *token*
(`lex.token += (char) c;`) e ir para o estado 7 (`state = 7;`).
Note que o diagrama não explicita as transições de erro, mas elas devem ser
consideradas na implementação.
O que deve acontecer se o programa ler o símbolo *'!'* e chegar ao fim do
arquivo antes de ler o símbolo *'='* para a formação do *token* **!=**?
Deve-se assinalar que chegou-se a um fim de arquivo inesperado
`lex.type = TokenType.UNEXPECTED_EOF;` e ir para o estado 8 (`state = 8;`).
E o que deve acontecer se vier um caracter qualquer depois do *'!'* que não um
*'='*?
Então deve-se assinalar que foi formado um *token* inválido
(`lex.type = TokenType.INVALID_TOKEN;`) e ir para o estado 8 (`state = 8;`).
Lembre-se que é preciso definir explicitamente o tipo do *token* sempre que se
for para o estado 8.

```Java
case 6:
	if (Character.isDigit(c)) {
		lex.token += (char) c;
		state = 6;
	} else {
		ungetc(c);
		lex.type = TokenType.NUMBER;
		state = 8;
	}

	break;
```

O caminho e<sub>1</sub>->(e<sub>6</sub>)<sup>*</sup>->e<sub>8</sub> é usado
para formação de constantes numéricas.
A implementação do estado 6 é dada conforme código acima.
Enquanto o caracter lido for dígito (`if (Character.isDigit(c)) {`) deve-se
adicioná-lo ao *token* (`lex.token += (char) c;`) e manter
no mesmo estado (`state = 6;`).
Caso contrário (`} else {`), deve-se devolver o caracter para o buffer
(`ungetc(c);`), anotar explicitamente o tipo como numérico
(`lex.type = TokenType.NUMBER;`) e ir para o estado 8 (`state = 8;`).
Considerer que, na expressão `x = 321;`, a constante `321` está sendo lida.
O *token* é formado lendo primeiro o dígito *'3'* no estado 1, e os dígitos
restantes no estado 6.
É preciso ler o caracter *';'* para saber que terminou a formação do número.
Assim, deve-se devolver esse símbolo para o buffer para que esse seja usado na
formação do próximo *token*.

```Java
case 2:
	if (c == '\n') {
		line++;
		state = 1;
	} else if (c == -1) {
		lex.type = TokenType.END_OF_FILE;
		state = 8;
	} else {
		state = 2;
	}

	break;
```

O caminho e<sub>1</sub>->(e<sub>2</sub>)<sup>*</sup>->e<sub>1</sub> é usado
para implementar o comentário de uma linha.
Repare que comentários não formam *tokens*, portanto os caracteres lidos nunca
são concatenados ao *token*.
Caso encontre o caracter *'\n'* (`if (c == '\n') {`), deve-se incrementar
o número da linha (`line++;`) e voltar para o estado 1 (`state = 1;`).
O que deve acontecer se o arquivo terminar no comentário?
Embora não expliciamente definido pelo diagrama, ao chegar no fim do arquivo
(`} else if (c == -1) {`) deve-se considerar que esse é um fim de arquivo
normal/esperado (`lex.type = TokenType.END_OF_FILE;`) e ir para o estado 8
(`state = 8`).
Todos os outros caracteres (`} else {`), que são parte do comentário, devem
ser ignorados e o autônomo mantido no mesmo estado (`state = 2;`).

Para detalhes da implementação dos outros estados, favor consultar o analisador
léxico disponível no código do repositório.

#### Resultado

O resultado obtido pelo analisador léxico é a sequência de lexemas produzidos
pelo programa de entrada. Para o programa de exemplo, obtêm-se os seguintes
lexemas, nessa ordem:

```
("program", TokenType.PROGRAM)
("sum", TokenType.VAR)
("=", TokenType.ASSIGN)
("0", TokenType.NUMBER)
(";", TokenType.SEMICOLON)
("i", TokenType.VAR)
("=", TokenType.ASSIGN)
("read", TokenType.READ)
(";", TokenType.SEMICOLON)
("while", TokenType.WHILE)
("i", TokenType.VAR)
(">", TokenType.GREATER)
("0", TokenType.NUMBER)
("do", TokenType.DO)
("sum", TokenType.VAR)
("=", TokenType.ASSIGN)
("sum", TokenType.VAR)
("+", TokenType.ADD)
("i", TokenType.VAR)
(";", TokenType.SEMICOLON)
("i", TokenType.VAR)
("=", TokenType.ASSIGN)
("read", TokenType.READ)
(";", TokenType.SEMICOLON)
("done", TokenType.DONE)
(";", TokenType.SEMICOLON)
("output", TokenType.OUTPUT)
("sum", TokenType.VAR)
(";", TokenType.SEMICOLON)
("", TokenType.END_OF_FILE)
```

Note que ao final do processo obtém-se o lexema `("", TokenType.END_OF_FILE)`
que é um marcador que o analisador léxico processou o arquivo de entrada
corretamente e chegou-se a um fim de arquivo sem erros léxicos.

### Analisador sintático

O analisador sintático é responsável por verificar se os *tokens* de um programa
se encontram em uma ordem válida.
Para isso, é definida uma gramática com regras de como os *tokens* são
organizados na linguagem.

Para consultar a implementação separada do analisador sintático, basta fazer
*checkout* do *branch* **sintatico**.

    $ git checkout sintatico

#### Gramática

Gramáticas são normalmente expressas no formato EBNF (Extended Backus-Naur
Form), um tipo de gramática livre de contexto.
Nela é possível definir produções opcionais--aquelas separadas por `|` ou entre
`[` e `]`--, repetições de zero ou mais vezes--aquelas entre `{` e `}`--, e
agrupamentos de expressões--aquelas entre `(` e `)`.
A gramática da linguagem Tiny é mostrada a seguir nesse formato:

```
<program>   ::= program <cmdlist>
<cmdlist>   ::= <cmd> { <cmd> }
<cmd>       ::= (<assign> | <output> | <if> | <while>) ;
<assign>    ::= <var> = <intexpr>
<output>    ::= output <intexpr>
<if>        ::= if <boolexpr> then <cmdlist> [ else <cmdlist> ] done
<while>     ::= while <boolexpr> do <cmdlist> done
<boolexpr>  ::= false | true |
                not <boolexpr> |
                <intterm> (== | != | < | > | <= | >=) <intterm>
<intexpr>   ::= [ + | - ] <intterm> [ (+ | - | * | / | %) <intterm> ]
<intterm>   ::= <var> | <const> | read
<var>       ::= id
<const>     ::= number
```

Os nomes entre chaves chevron, `<` e `>`, são símbolos não terminais, ou seja,
regras de produções; já os outros são símbolos terminais, ou símbolos da
linguagem.
A regra de partida é dada pela primeira regra `<program>`.

Essa gramática foi especialmente desenhada como LL(1), um tipo de gramática
que permite a criação de um parser recursivo descendente olhando apenas um
*token* a frente (*1-symbol lookahead*).

#### Parser

O parser depende do analisador léxico (`LexicalAnalysis lex`) para fornecer
os *tokens* para um programa da entrada.
O parser mantém sempre um lexema ativo (`Lexeme current`), ou seja, o *token*
a ser processado.

```Java
public class SyntaticAnalysis {

    private LexicalAnalysis lex;
    private Lexeme current;
	
    public SyntaticAnalysis(LexicalAnalysis lex) {
        this.lex = lex;
        this.current = lex.nextToken();
    }
	
    ...
}
```

O analisador sintático possui um método `advance` que passa o lexema atual
para o próximo (`current = lex.nextToken();`).
Também possui um método `eat` que verifica o casamento do lexema atual com um
tipo de *token* esperado (`if (type == current.type) {`).
Em caso positivo, deve-se avançar para o próximo lexema (`advance();`), caso
contrário deve-se exibir um erro.

```Java
private void advance() {
	current = lex.nextToken();
}

private void eat(TokenType type) {
	if (type == current.type) {
		advance();
	} else {
		showError();
	}
}
```

Existem três tipos de erros sintático para essa linguagem: (1) **lexema
inválido** produzido pelo analisador léxico; (2) **fim de arquivo inesperado**
produzido pelo analisador léxico ou sintático; (3) e **lexema não esperado**
caso o próximo *token* não seja o esperado.
O interpretador exibe uma mensagem de acordo com o tipo de erro com o número
da linha onde ele ocorreu e pode incluir o *token* formado (exceto para fim
de arquivo inesperado).
No caso dessa implementação, o parsing é imediatamente interrompido ao se
encontrar o primeiro erro sintático (`System.exit(1);`).

```Java
private void showError() {
	System.out.printf("%02d: ", lex.getLine());

	switch (current.type) {
		case INVALID_TOKEN:
			System.out.printf("Lexema inválido [%s]\n", current.token);
			break;
		case UNEXPECTED_EOF:
		case END_OF_FILE:
			System.out.printf("Fim de arquivo inesperado\n");
			break;
		default:
			System.out.printf("Lexema não esperado [%s]\n", current.token);
			break;
	}

	System.exit(1);
}
```

O parsing deve implementar um procedimento para cada nome de regra (lado
esquerdo da produção).
É interessante manter a descrição da regra completa como um cabeçalho
comentado antes de cada método.

```Java
// <program>   ::= program <cmdlist>
private void procProgram() { ... }

// <cmdlist>   ::= <cmd> { <cmd> }
private void procCmdList() { ... }

// <cmd>       ::= (<assign> | <output> | <if> | <while>) ;
private void procCmd() { ... }

// <assign>    ::= <var> = <intexpr>
private void procAssign() { ... }

// <output>    ::= output <intexpr>
private void procOutput() { ... }

// <if>        ::= if <boolexpr> then <cmdlist> [ else <cmdlist> ] done
private void procIf() { ... }

// <while>     ::= while <boolexpr> do <cmdlist> done
private void procWhile() { ... }

// <boolexpr>  ::= false | true |
//                 not <boolexpr> |
//                 <intterm> (== | != | < | > | <= | >=) <intterm>
private void procBoolExpr() { ... }

// <intexpr>   ::= [ + | - ] <intterm> [ (+ | - | * | / | %) <intterm> ]
private void procIntExpr() { ... }

// <intterm>   ::= <var> | <const> | read
private void procIntTerm() { ... }

// <var>       ::= id
private void procVar() { ... }

// <const>     ::= number
private void procConst() { ... }
```

Existe um método especial `start` para dar início ao processo de parsing.
Ele tem uma chamada para a regra de partida (`<program>`) e um casamento de
*token* com o fim de arquivo normal/esperado.

```Java
public void start() {
	procProgram();
	matchToken(TokenType.END_OF_FILE);
}
```

Para implementar uma regra deve-se olhar as produções do seu lado direito.
Por exemplo, para a regra `<while>` tem-se `while <boolexpr> do <cmdlist> done`.
Para os símbolos não terminais, entre chaves chevron (ex.: `<boolexpr>`),
deve-se chamar o procedimento respectivo: `procBoolExpr();`.
Para os símbolos terminais (ex.: `while`) deve-se chamar casar o padrão com o
tipo de *token* respectivo: `eat(TokenType.WHILE);`.
Assim, a implementação do regra `<while>` é dada a seguir.

```Java
// <while>     ::= while <boolexpr> do <cmdlist> done
private void procWhile() {
	eat(TokenType.WHILE);
	procBoolExpr();
	eat(TokenType.DO);
	procCmdList();
	eat(TokenType.DONE);
}
```

Quando se tem uma regra com várias opções, separados pelo símbolo `|` na
gramática, deve-se verificar o tipo do lexema atual para verificar como
proceder.
Repare que, nos casos onde existe a produção na esquerda é uma regra, deve-se
olhar nessa regra qual *token* usar--para chamar a regra `<var>`, deve-se olhar
dentro dessa regra qual é primeiro *token* de sua produção, nesse caso `id` cujo
tipo do *token* é `TokenType.VAR`.
O mesmo vale para a regra `<const>`, onde `number` tem como tipo de *token*
`TokenType.NUMBER`.

```Java
// <intterm>   ::= <var> | <const> | read
private void procIntTerm() {
	if (current.type == TokenType.VAR) {
		procVar();
	} else if (current.type == TokenType.NUMBER) {
		procConst();
	} else {
		eat(TokenType.READ);
	}
}
```

Quando se tem uma regra com um trecho opcional, entre `[` e `]`, deve-se usar
a mesma estratégia da implementação anterior.
Note que, se verificado que o *token* é do tipo `else`
(`if (current.type == TokenType.ELSE) {`), basta avançar para o próximo lexema
(`advance();`).

```Java
// <if>        ::= if <boolexpr> then <cmdlist> [ else <cmdlist> ] done
private void procIf() {
	eat(TokenType.IF);
	procBoolExpr();
	eat(TokenType.THEN);
	procCmdList();
	if (current.type == TokenType.ELSE) {
		advance();
		procCmdList();
	}
	eat(TokenType.DONE);
}
```

Por fim, regras que possuem repetições de zero ou mais vezes, entre os símbolos
`{` e `}`, são implementadas como laços.
Repare que, para a regra `<cmdlist>` que possui repetição da regra `<cmd>`,
deve-se olhar quais *tokens* podem atingir essa regra, que nesse caso são:
`TokenType.VAR`, `TokenType.OUTPUT`, `TokenType.IF` e `TokenType.WHILE`.

```Java
// <cmdlist>   ::= <cmd> { <cmd> }
private void procCmdList() {
	procCmd();
	while (current.type == TokenType.VAR ||
			current.type == TokenType.OUTPUT ||
			current.type == TokenType.IF ||
			current.type == TokenType.WHILE) {
		procCmd();
	}
}
```

Para detalhes da implementação das outras regras favor consultar o analisador
sintático disponível no código do repositório.

### Interpretador

O interpretador é responsável por modelar o comportamento de um programa.
Um diagrama de classes é dado na seção [modelo](#modelo) e como ele deve ser
usado é mostrado na seção de [implementação](#implementação).

#### Modelo

O interpretador foi modelado de acordo com o diagrama de classes a seguir.
Esse modelo pode ser divido em duas partes: **comandos** e **expressões**.
Comandos executam uma ação e não produzem uma saída, ex.: comandos de atribução,
condicionais (`if`), de repetição (`while`), etc.
Já as expressões são avaliadas e produzem um resultado, ex.: expressão,
aritmética, expressão relacional, etc.
O modelo separa essas duas partes nos pacotes: `command` e `expr`.
Os comandos devem herdar da classe abstrata `Command` e implementar o método
`void execute()`.
As expressões inteiras devem herdar de `IntExpr` e implementar o método
`int expr()`, enquanto as expressões lógicas devem herdar de `BoolExpr` e
implementar o método `boolean expr()`.
Todas as classes base incluem o número da linha onde suas construções aparecem
no programa fonte.

<p align="center">
  <img src="images/interpretador.png?raw=true" width="936" height="650">
</p>

Cada classe desse modelo deve implementar um comportamento, conforme explicado
nas tabelas a seguir para cada uma das classes base:

- `Command`:
  | *Classe*         | *Comportamento* |
  | ---------------- | --------------- |
  | `BlocksCommand`  | Executar cada um dos comandos da lista do bloco em sequência |
  | `AssignCommand`  | Atribuir o resultado de uma expressão inteira em uma variável |
  | `OutputCommand`  | Mostrar a saída de uma expressão inteira na tela |
  | `IfCommand`      | Executar o bloco *then* se a expressão for verdadeira, caso contrário executar o bloco *else* se esse existir |
  | `WhileCommand`   | Executar o bloco do corpo da repetição enquanto a expressão for verdadeira |

- `BoolExpr`:
  | *Classe*         | *Comportamento* |
  | ---------------- | --------------- |
  | `ConstBoolExpr`  | Obter um valor lógico para s constantes verdadeiro ou falso |
  | `SingleBootExpr` | Comparar dois valores inteiros de acordo com uma operação lógica |
  | `NotBoolExpr`    | Negar uma expressão lógica |

- `IntExpr`:
  | *Classe*         | *Comportamento* |
  | ---------------- | --------------- |
  | `ConstIntExpr`   | Obter o valor de uma constante inteira |
  | `NegIntExpr`     | Negar uma expressão inteira |
  | `ReadIntExpr`    | Ler um valor inteiro dado por um usuário pela entrada de teclado |
  | `BinaryIntExpr`  | Realizar uma operação inteira entre dois valores inteiros |
  | `Variable`       | Armazenar o valor inteiro em uma variável |

#### Implementação

Cada regra da gramática deve instanciar um objeto do modelo dado com os
argumentos necessários para sua execução.
A seguir é detalhado quais classes devem ser retornadas para todas as regras.
As regras que podem retornar mais de um tipo de objeto devem usar a classe
base genérica.
Por exemplo, a regra `<cmd>` pode retornar um comando de atribuição,
condicional, de repetição ou de saída, portanto deve-se usar a classe base
`Command`.

| *Regra*      | *Retorno*       |
| ------------ | --------------- |
| `<program>`  | `BlocksCommand` |
| `<cmdlist>`  | `BlocksCommand` |
| `<cmd>`      | `Command`       |
| `<assign>`   | `AssignCommand` |
| `<output>`   | `OutputCommand` |
| `<if>`       | `IfCommand`     |
| `<while>`    | `WhileCommand`  |
| `<boolexpr>` | `BoolExpr`      |
| `<intexpr>`  | `IntExpr`       |
| `<intterm>`  | `IntExpr`       |
| `<var>`      | `Variable`      |
| `<const>`    | `ConstIntExpr`  |

Considere a implementação da regra `<while>` que chama a regra `<boolexpr>` para
obter a expressão condicional (retorna uma instância de `BoolExpr`) e que
chama a regra `<cmdlist>` com uma lista de comandos (retorna uma instância
de `Command`).
Esses são os dois objetos necessários para a construção do objeto `WhileCommand`
conforme definido no modelo.
Além disso, para construir esse objeto deve-se obter o número da linha onde
ele se encontra no programa fonte.
O número da linha deve ser obtido (`int line = lex.getLine();`) sempre depois do
casamento do primeiro *token* da regra (nesse caso `eat(TokenType.WHILE);`).
Assim, o procedimento completo para essa regra é dado a seguir.

```Java
// <while>     ::= while <boolexpr> do <cmdlist> done
private WhileCommand procWhile() {
    eat(TokenType.WHILE);
    int line = lex.getLine();

    BoolExpr cond = procBoolExpr();
    eat(TokenType.DO);
    Command cmds = procCmdList();
    eat(TokenType.DONE);

    return new WhileCommand(line, cond, cmds);
}
```

A implementação das classe dos modelos devem: (1) inicializar os atribuitos da
classe com os objetos passados pelo construtor; e (2) implementar o método
abstrato de sua classe base de acordo com o comportamento esperado dado de sua
regra.
Por exemplo, a implementação da classe `CommandWhile` é dada a seguir:

```Java
public class WhileCommand extends Command {

    private BoolExpr cond;
    private Command cmds;

    public WhileCommand(int line, BoolExpr cond, Command cmds) {
        super(line);
        this.cond = cond;
        this.cmds = cmds;
    }

    public void execute() {
        while (cond.expr())
            cmds.execute();
    }
}
```

O mesmo vale para a implementação de uma expressão.
Por exemplo para a classe `BinaryIntExpr`:

```Java
public class BinaryIntExpr extends IntExpr {

    private IntExpr left;
    private IntOp op;
    private IntExpr right;

    public BinaryIntExpr(int line, IntExpr left, IntOp op, IntExpr right) {
        super(line);
        this.left = left;
        this.op = op;
        this.right = right;
    }

    public int expr() {
        int v1 = left.expr();
        int v2 = right.expr();

        switch (op) {
            case Add:
                return v1 + v2;
            case Sub:
                return v1 - v2;
            case Mul:
                return v1 * v2;
            case Div:
                return v1 / v2;
            case Mod:
            default:
                return v1 % v2;
        }
    } 
}
```

As regras `<var>` e `<const>` necessitam de atenção especial: elas dependem do
valor do *token* para sua utilização.
Para a regra `<var>` deve-se saber qual o nome da variável referenciada,
enquanto para a regra `<const>` qual o valor da constante inteira utilizada.
Para isso é preciso salvar o *token* em uma variável antes de passar para o
próximo lexema.

```Java
// <var>       ::= id
private Variable procVar() {
    String name = current.token;
    eat(TokenType.VAR);

    return Variable.instance(name);
}
```

Por exemplo, para a implementação da regra `<var>` deve-se obter o nome da
variável (`String name = current.token;`) antes do casamento de padrão que irá
passar para o próximo lexema (`eat(TokenType.VAR);`).
Depois, basta retornar uma instânica dessa variável com esse nome.
