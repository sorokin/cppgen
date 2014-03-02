#ifndef GRAM_STMT_H
#define GRAM_STMT_H

struct context;

void generate_statement(context& c);
void generate_labeled_statement(context& c);
void generate_expression_statement(context& c);
void generate_compound_statement(context& c);
void generate_statement_seq(context& c);
void generate_selection_statement(context& c);
void generate_condition(context& c);
void generate_iteration_statement(context& c);
void generate_for_init_statement(context& c);
void generate_for_range_declaration(context& c);
void generate_for_range_initializer(context& c);
void generate_jump_statement(context& c);
void generate_declaration_statement(context& c);

#endif // GRAM_STMT_H
