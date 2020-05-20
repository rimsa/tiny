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
* Identificadores (variáveis) começam com uma letra e podem ser seguidos de
letras, dígitos e underscore (_).
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
resto da divisão (**%**). Expressões artiméticas compostas devem usar
identificadores auxiliares.
* A linguagem possui comentários de uma linha a partir do símbolo tralha (#).

Um exemplo de linguagem é dada a seguir (*somatorio.tiny*).

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
entrar com valores maiores que zero. Ao final, é exibido o valor do somatório.
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
