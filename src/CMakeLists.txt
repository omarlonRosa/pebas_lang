# Adicionar o executável do compilador Bel-pb
add_executable(bel_pb
    compiler/main.cpp
    compiler/lexer/lexer.cpp
    compiler/parser/parser.cpp
    compiler/ast/ast.cpp
    # Adicionar outros arquivos .cpp conforme são criados
    # compiler/sema/type_checker.cpp
    # compiler/ir_gen/ir_generator.cpp
)

# Especificar diretórios de include
target_include_directories(bel_pb PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Linkar bibliotecas (se necessário no futuro)
# target_link_libraries(bel_pb PRIVATE some_library)

# Configurações específicas do compilador
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(bel_pb PRIVATE -Wno-unused-parameter)
endif()

# Instalar o executável (opcional)
# install(TARGETS bel_pb DESTINATION bin)

# Adicionar target para executar o compilador com um arquivo de exemplo
add_custom_target(run_example
    COMMAND bel_pb ${CMAKE_SOURCE_DIR}/examples/hello_world.pebas -v
    DEPENDS bel_pb
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running Bel-pb compiler with example file"
)

