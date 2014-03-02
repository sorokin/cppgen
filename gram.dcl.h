#ifndef GRAM_DCL_H
#define GRAM_DCL_H

struct context;

void generate_declaration_seq(context& c);
void generate_declaration(context &c);
void generate_block_declaration(context& c);
void generate_simple_declaration(context& c);
void generate_decl_specifier_seq(context& c);
void generate_class_specifier(context& c);
void generate_type_specifier(context& c);
void generate_trailing_type_specifier(context& c);
void generate_simple_type_specifier(context& c);
void generate_elaborated_type_specifier(context& c);
void generate_enum_specifier(context& c);
void generate_enum_head(context& c);
void generate_enum_key(context& c);
void generate_enumerator_list(context& c);
void generate_enumerator_definition(context& c);
void generate_enumerator(context& c);
void generate_namespace_definition(context& c);
void generate_namespace_body(context& c);

#endif // GRAM_DCL_H
