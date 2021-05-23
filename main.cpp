#include <iostream>
#include <string>

typedef enum {
  METACOMMAND_SUCCESS,
  METACOMMAND_UNRECOGNIZED,
} MetaCommandResult;

typedef enum {
  PREPARE_SUCCESS,
  PREPARE_UNRECOGNIZED,
} PrepareResult;

typedef enum {
  STATEMENT_INSERT,
  STATEMENT_SELECT,
} StatementType;

#define COL_SIZE_USERNAME 32
#define COL_SIZE_EMAIL 255

typedef struct {
  uint32_t id;
  char username[COL_SIZE_USERNAME];
  char email[COL_SIZE_EMAIL];
} Row;

typedef struct {
  StatementType type;
  Row row_to_insert;
} Statement;

MetaCommandResult do_meta_command(std::string input) {
  if (input == ".exit") {
    exit(EXIT_SUCCESS);
  } else {
    return METACOMMAND_UNRECOGNIZED;
  }
}

PrepareResult prepare_statement(std::string input, Statement* statement) {
  if (input == "insert") {
    statement->type = STATEMENT_INSERT;
    return PREPARE_SUCCESS;
  }
  if (input == "select") {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }
  return PREPARE_UNRECOGNIZED;
}

void execute_statement(Statement* statement) {
  switch (statement->type) {
    case (STATEMENT_INSERT):
      std::cout << "actions for an INSERT..." << std::endl;
      break;
    case (STATEMENT_SELECT):
      std::cout << "actions for a SELECT..." << std::endl;
      break;
  }
}

void print_prompt() { std::cout << "sqlite> "; }

int main() {
  while (true) {
    print_prompt();

    std::string input;
    std::getline(std::cin, input);

    if (input[0] == '.') {
      switch (do_meta_command(input)) {
        case (METACOMMAND_SUCCESS):
          continue;
        case (METACOMMAND_UNRECOGNIZED):
          std::cout << "Error: near \"" << input << "\": unrecognized command" << std::endl;
          continue;
      }
    }

    Statement s;
    switch (prepare_statement(input, &s)) {
      case (PREPARE_UNRECOGNIZED):
        std::cout << "Unrecognized keyword at \"" << input << "\"\n";
        continue;
      case (PREPARE_SUCCESS):
        break;
    }

    execute_statement(&s);
    std::cout << "Executed.\n";
  }
}