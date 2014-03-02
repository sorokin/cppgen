#ifndef GRAM_LEX_H
#define GRAM_LEX_H

struct context;

void generate_integer_literal(context& c);
void generate_identifier(context& c);
void generate_float_literal(context& c);

#endif // GRAM_LEX_H
