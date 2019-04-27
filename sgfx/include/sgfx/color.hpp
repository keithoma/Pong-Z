#ifndef SGFX_COLOR_H
#define SGFX_COLOR_H

#include <cstdint>

namespace sgfx {
namespace color
{
	class rgb_color
	{
		public:
		
		constexpr rgb_color():
			rgb_color(0,0,0)
		{}
		
		constexpr rgb_color(std::uint8_t r, std::uint8_t g, std::uint8_t b):
			values_{r,g,b}
		{};
		
		constexpr auto& red() { return values_[0]; }
		constexpr auto& green() { return values_[1]; }
		constexpr auto& blue() { return values_[2]; }
		
		constexpr const auto& red() const { return values_[0]; }
		constexpr const auto& green() const { return values_[1]; }
		constexpr const auto& blue() const { return values_[2]; }
		
		private:
		std::uint8_t values_[3];
	};
	
	inline constexpr const rgb_color white{255,255,255};
	inline constexpr const rgb_color black{0,0,0};
	inline constexpr const rgb_color red{255,0,0};
	inline constexpr const rgb_color green{0,255,0};
	inline constexpr const rgb_color blue{0,0,255};
	inline constexpr const rgb_color yellow{255,255,0};
	inline constexpr const rgb_color gray{127,127,127};
	inline constexpr const rgb_color cyan{0,255,255};
	inline constexpr const rgb_color magenta{255,0,255};
	
}}

#endif
