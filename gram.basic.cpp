#include "gram.basic.h"
#include "context.h"
#include "random_utils.h"
#include "gram.dcl.h"

using namespace random_utils;

void generate_translation_unit(context& c)
{
    // declaration-seq[opt]

    if (probably(c.rng, 0.999))
        generate_declaration_seq(c);
}

