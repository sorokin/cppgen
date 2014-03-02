#ifndef GRAM_DECL_H
#define GRAM_DECL_H

struct context;

void generate_init_declarator_list(context& c);
void generate_init_declarator(context& c);
void generate_declarator(context& c, bool functions_only);
void generate_ptr_declarator(context& c, bool functions_only);
void generate_noptr_declarator(context& c, bool functions_only, bool allow_declarator_id);
void generate_ptr_operator(context& c);
void generate_declarator_id(context& c);
void generate_parameter_declaration(context& c);
void generate_parameter_declaration_list(context& c);
void generate_parameter_declaration_clause(context& c);
void generate_initializer(context& c);
void generate_initializer_clause(context& c);
void generate_function_definition(context& c);
void generate_function_body(context& c);

#endif // GRAM_DECL_H
