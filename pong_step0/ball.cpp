#include "ball.hpp"

#include <sgfx/primitives.hpp>

#include <algorithm>

using namespace pong;

ball::ball()
{
}

void ball::move_to(sgfx::point position)
{
}

void ball::bind_to(sgfx::rectangle bounds)
{
}

void ball::accelerate(sgfx::vec acceleration)
{
}

void ball::limit_velocity(sgfx::vec max_velocities)
{
}

void ball::set_velocity(sgfx::vec velocity)
{
}

void ball::reflect_x()
{
}

void ball::reflect_y()
{
}

ball::status ball::update_step()
{
}

void ball::draw(sgfx::canvas_view target) const
{
}
