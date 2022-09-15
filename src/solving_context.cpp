#include "solving_context.h"

#include <cassert>

namespace bzla {

/* --- SolvingContext public ----------------------------------------------- */

SolvingContext::SolvingContext() : d_bv_solver(*this) {}

Result
SolvingContext::solve()
{
  d_sat_state = d_bv_solver.check();
  return d_sat_state;
}

void
SolvingContext::assert_formula(const Node& formula)
{
  assert(formula.get_type().is_bool());
  auto [it, inserted] = d_assertions_cache.insert(formula);
  if (inserted)
  {
    d_assertions.push_back(formula);
  }
}

Node
SolvingContext::get_value(const Node& term)
{
  assert(d_sat_state == Result::SAT);

  const Type& type = term.get_type();
  if (type.is_bool() || type.is_bv())
  {
    return d_bv_solver.value(term);
  }

  // TODO: Handle more types.
  assert(false);
  return Node();
}

}  // namespace bzla