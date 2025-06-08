# Pebas Lang - Versão Corrigida

Esta é a versão corrigida do repositório da linguagem Pebas Lang com todas as correções de sintaxe e melhorias estruturais aplicadas.

## Estrutura do Projeto

```
pebas_lang_corrected/
├── CMakeLists.txt              # Configuração principal do CMake
├── README.md                   # Este arquivo
├── include/                    # Headers públicos
│   └── pebas/
│       ├── ast/
│       │   └── ast.h          # Definições da AST
│       ├── lexer/
│       │   └── lexer.h        # Definições do Lexer
│       └── parser/
│           └── parser.h       # Definições do Parser
├── src/                       # Código fonte
│   ├── CMakeLists.txt         # Configuração do CMake para src
│   └── compiler/
│       ├── main.cpp           # Ponto de entrada do compilador Bel-pb
│       ├── ast/
│       │   └── ast.cpp        # Implementação dos métodos AST
│       ├── lexer/
│       │   └── lexer.cpp      # Implementação do Lexer
│       └── parser/
│           └── parser.cpp     # Implementação do Parser
└── examples/
    └── hello_world.pebas      # Exemplo de código Pebas Lang
```

## Principais Correções Aplicadas

### Lexer (lexer.cpp)
- ✅ Corrigido `avdvance()` → `advance()`
- ✅ Corrigido `makeToKen()` → `makeToken()`
- ✅ Corrigido `swift (c)` → `switch (c)`
- ✅ Corrigido `speek()` → `peek()`
- ✅ Corrigido todos os erros de sintaxe e pontuação
- ✅ Implementado mapa completo de palavras-chave
- ✅ Melhorado tratamento de comentários (`#` e `#*...*#`)
- ✅ Adicionado suporte a todos os tipos de tokens

### Parser (parser.cpp)
- ✅ Corrigido `consume(TokenTypeSEMICOLON, ...)` → `consume(TokenType::SEMICOLON, ...)`
- ✅ Corrigido `TokenType::IDENTIFER` → `TokenType::IDENTIFIER`
- ✅ Corrigido `std:vector` → `std::vector`
- ✅ Implementado parsing completo de expressões com precedência
- ✅ Adicionado suporte a estruturas de controle (if, while, for)
- ✅ Implementado tratamento de erros robusto

### AST (ast.h)
- ✅ Estrutura completa com padrão Visitor
- ✅ Todos os tipos de nós necessários
- ✅ Implementação dos métodos `accept()`
- ✅ Suporte a localização de código fonte

## Requisitos

- **macOS**: 10.15+ (recomendado macOS 12+ para Apple Silicon)
- **Compilador**: Clang 12+ ou GCC 9+
- **CMake**: 3.15+
- **C++**: Padrão C++17

## Compilação

### 1. Clonar e Preparar

```bash
# Se ainda não clonou, clone o repositório original
git clone https://github.com/Pebas-Lang/pebas_lang.git
cd pebas_lang

# Substitua os arquivos pelos corrigidos
# (copie os arquivos da pasta pebas_lang_corrected)
```

### 2. Compilar com CMake

```bash
# Criar diretório de build
mkdir build
cd build

# Configurar o projeto
cmake ..

# Compilar
cmake --build .

# Ou usar make diretamente (se disponível)
make
```

### 3. Executar o Compilador

```bash
# Executar com arquivo de exemplo
./src/bel_pb ../examples/hello_world.pebas -v

# Mostrar apenas tokens
./src/bel_pb ../examples/hello_world.pebas -t

# Mostrar apenas AST
./src/bel_pb ../examples/hello_world.pebas -a

# Mostrar ajuda
./src/bel_pb --help
```

## Opções do Compilador Bel-pb

```
Usage: bel_pb [options] <source_file>
Options:
  -h, --help     Show this help message
  -t, --tokens   Print tokens only
  -a, --ast      Print AST only
  -v, --verbose  Verbose output
```

## Exemplo de Uso

### Arquivo de Entrada (hello_world.pebas)

```pebas
# Exemplo simples em Pebas Lang
var x: int = 42;
var name: string = "Pebas Lang";

function add(a: int, b: int) -> int {
    return a + b;
}

function main() -> int {
    var result: int = add(10, 20);
    print("Resultado: ${result}");
    return 0;
}
```

### Saída do Compilador

```bash
$ ./bel_pb hello_world.pebas -v
Reading file: hello_world.pebas
Starting lexical analysis...

=== TOKENS ===
Token: var (Line: 2, Col: 1)
Token: x (Line: 2, Col: 5)
Token: : (Line: 2, Col: 6)
Token: int (Line: 2, Col: 8)
...

Starting syntax analysis...

=== AST ===
Program:
  VarDecl: x : int
    Literal: 42
  VarDecl: name : string
    Literal: "Pebas Lang"
  FunctionDecl: add(a: int, b: int) -> int
    Block:
      Return:
        Binary: +
          Identifier: a
          Identifier: b
...

Compilation completed successfully!
```

## Próximos Passos

1. **Testes**: Implementar testes unitários com Google Test
2. **Análise Semântica**: Adicionar verificação de tipos
3. **Geração de IR**: Implementar representação intermediária
4. **Backend**: Integrar com LLVM para geração de código
5. **Biblioteca Padrão**: Desenvolver funções básicas

## Estrutura de Desenvolvimento Recomendada

### Para Neovim no macOS

1. **Instalar dependências**:
```bash
brew install cmake clang-format
```

2. **Configurar clangd** no Neovim para LSP
3. **Usar clang-format** para formatação automática
4. **Configurar debugging** com LLDB

### Workflow de Desenvolvimento

1. **Fazer mudanças** nos arquivos fonte
2. **Compilar** com `cmake --build build`
3. **Testar** com arquivos de exemplo
4. **Debuggar** se necessário
5. **Commit** das mudanças

## Contribuição

Este projeto segue as melhores práticas de:
- **Robert C. Martin**: Código limpo e arquitetura sólida
- **Bjarne Stroustrup**: Eficiência e expressividade
- **Chris Lattner**: Arquitetura modular (LLVM)
- **Linus Torvalds**: Pragmatismo e qualidade

Para contribuir:
1. Fork o repositório
2. Crie uma branch para sua feature
3. Implemente com testes
4. Faça code review
5. Submit pull request

## Licença

[Definir licença - recomendado MIT ou Apache 2.0]

