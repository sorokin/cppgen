#ifndef GRAM_EXPR_H
#define GRAM_EXPR_H

struct context;

void generate_unary_expression(context& c);
void generate_cast_expression(context& c);
void generate_pm_expression(context& c);
void generate_multiplicative_expression(context& c);
void generate_additive_expression(context& c);
void generate_shift_expression(context& c);
void generate_relational_expression(context& c);
void generate_equality_expression(context& c);
void generate_and_expression(context& c);
void generate_exclusive_or_expression(context& c);
void generate_inclusive_or_expression(context& c);
void generate_logical_and_expression(context& c);
void generate_logical_or_expression(context& c);
void generate_conditional_expression(context& c);
void generate_assignment_expression(context& c);
void generate_assignment_operator(context& c);
void generate_expression(context& c);
void generate_constant_expression(context& c);

#endif // GRAM_EXPR_H
