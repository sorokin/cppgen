#ifndef GRAM_CLASS_H
#define GRAM_CLASS_H

struct context;

void generate_class_key(context& c);
void generate_class_head(context& c);
void generate_member_declarator(context& c);
void generate_member_declarator_list(context& c);
void generate_member_declaration(context& c);
void generate_access_specifier(context& c);
void generate_member_specification(context& c);

#endif // GRAM_CLASS_H
