#include "object.hpp"
#include <sgfx/primitives.hpp>

#include <algorithm>

using namespace pong;

object::object(
    sgfx::canvas img,
    sgfx::rectangle bounds,
    sgfx::point initialPosition,
    sgfx::vec initialAcceleration)
{
}

void object::accelerate(sgfx::vec acceleration)
{
}

void object::limit_velocity(sgfx::vec max_velocities)
{
}

void object::set_velocity(sgfx::vec velocity)
{
}

void object::reflect_x()
{
}

void object::reflect_y()
{
}

object::status object::update_step()
{
}

void object::draw(sgfx::window& target) const
{
}
