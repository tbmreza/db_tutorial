#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <string>

typedef enum {
  METACOMMAND_SUCCESS,
  METACOMMAND_UNRECOGNIZED,
} MetaCommandResult;

typedef enum {
  PREPARE_SUCCESS,
  PREPARE_UNRECOGNIZED,
  PREPARE_SYNTAX_ERROR,
} PrepareResult;

typedef enum {
  STATEMENT_INSERT,
  STATEMENT_SELECT,
} StatementType;

#define COL_SIZE_USERNAME 32
#define COL_SIZE_EMAIL 255

typedef struct {
  uint32_t id;
  std::string username;  // char username[COL_SIZE_USERNAME];
  std::string email;     // char email[COL_SIZE_EMAIL];
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

typedef std::vector<std::string> split_vector;

PrepareResult prepare_statement(std::string input, Statement* statement) {
  split_vector words;
  boost::split(words, input, boost::is_any_of(" "));
  if (words.size() < 4) {
    return PREPARE_SYNTAX_ERROR;
  }

  uint32_t id;
  try {
    id = boost::lexical_cast<uint32_t>(words[1]);
  } catch (const boost::bad_lexical_cast& e) {
    std::cerr << e.what() << '\n';
    return PREPARE_SYNTAX_ERROR;
  }

  statement->row_to_insert.id = id;
  statement->row_to_insert.username = words[2];
  statement->row_to_insert.email = words[3];

  if (words[0] == "insert") {
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
      case (PREPARE_SYNTAX_ERROR):
        std::cout << "Syntax error" << std::endl;
        continue;
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