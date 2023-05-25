/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_INTRINSCIS_COLOR_H
#define CYNG_OBJ_INTRINSCIS_COLOR_H

#include <algorithm>
#include <array>
#include <cstdint> // uint8_t, uint16_t
#include <limits>

#if defined(max)
#undef max
#endif

#if defined(min)
#undef min
#endif

#include <boost/assert.hpp>
#include <boost/functional/hash.hpp>

namespace cyng {
    /**
     * A color in the RGB color model is described by indicating how much
     * of each of the red, green, and blue is included. The color is expressed
     * as an RGB triplet (r,g,b), each component of which can vary from zero
     * to a defined maximum value. If all the components are at zero the result
     * is black; if all are at maximum, the result is the brightest representable white.
     * (from wikipedia)
     */
    template <typename T> class color {
        template <typename U> friend bool operator==(color<U> const &lhs, color<U> const &rhs);

        template <typename U> friend bool operator<(color<U> const &lhs, color<U> const &rhs);

      private:
        /**
         * Defines the values for the color index.
         */
        enum channel {
            RED,
            GREEN,
            BLUE,
            OPACITY,
            DIM, //!<	size/dimension of the channel array
        };

        /**
         * Defines the attributes of the
         * HSL (hue, saturation, lightness) model
         */
        enum hsl {
            HUE,
            SATURATION,
            LIGHTNESS, //!<	Luminace
            HSL_DIM,   //!<	size/dimension of the HSL model
        };

      public:
        using channel_type = T;
        using rgb_type = std::array<T, DIM>;
        using hsl_type = std::array<double, 3>; //!<	hue-saturation-lightness

      public:
        color()
            : rgb_() {}

        color(channel_type r, channel_type g, channel_type b, channel_type o = 0)
            : rgb_({r, g, b, o}) //	double-brace required in C+11 but not in C++14
        {}

        color(rgb_type const &rgb)
            : rgb_(rgb) {}

        /**
         * Initialize all channels with the same value. The resulting color is gray.
         */
        color(channel_type c, channel_type o = 0)
            : rgb_({c, c, c, o}) //	double-brace required in C+11 but not in C++14
        {}

        color(color const &other)
            : rgb_(other.rgb_) {}

        /**
         * @return value of the red channel
         */
        channel_type red() const { return rgb_.at(RED); }
        /**
         * @return value of the green channel
         */
        channel_type green() const { return rgb_.at(GREEN); }
        /**
         * @return value of the blue channel
         */
        channel_type blue() const { return rgb_[BLUE]; }
        /**
         * @return opacity value
         */
        channel_type opacity() const { return rgb_.at(OPACITY); }

        /**
         * @return false if the color is gray shaded
         */
        bool colored() const { return red() == green() == blue(); }

        /*
         * @return hue, saturation and lightness in the set [0, 1].
         */
        hsl_type to_hsl() const {
            /*
             * Convert the RGB values to the range 0-1, this can be done by dividing
             * the value by 255 for 8-bit color depth and 65,535 for 16-bit color depth.
             */
            hsl_type hsld{
                (double)red() / std::numeric_limits<T>::max(),
                (double)green() / std::numeric_limits<T>::max(),
                (double)blue() / std::numeric_limits<T>::max()};

            //	find maximum and minimum values
            auto minmax = std::minmax_element(std::begin(hsld), std::end(hsld));

            /*
             * Calculate the lightness value by adding the max and min
             * values and divide by 2.
             * Initialize other values with zero.
             */
            hsl_type hsl{0, 0, (*minmax.first + *minmax.second) / 2.0};

            if (*minmax.first == *minmax.second) {

                hsl[0] = hsl[1] = 0.0; // achromatic

            } else {
                //	delta value
                BOOST_ASSERT(*minmax.second > *minmax.first);
                double d = *minmax.second - *minmax.first;

                //	calculate saturation
                hsl[SATURATION] =
                    (hsl[LIGHTNESS] > 0.5) ? d / (2.0 - *minmax.second - *minmax.first) : d / (*minmax.second + *minmax.first);

                //	calculate hue
                if (*minmax.second == hsld[RED]) {
                    hsl[HUE] = (hsld[GREEN] - hsld[BLUE]) / d + (hsld[GREEN] < hsld[BLUE] ? 6.0 : 0.0);
                } else if (*minmax.second == hsld[GREEN]) {
                    hsl[HUE] = (hsld[BLUE] - hsld[RED]) / d + 2.0;
                } else if (*minmax.second == hsld[BLUE]) {
                    hsl[HUE] = (hsld[RED] - hsld[GREEN]) / d + 4.0;
                }

                //	convert it to [0, 1]
                hsl[HUE] /= 6.0;
            }

            return hsl;
        }

      private:
        rgb_type rgb_;
    };

    //!<	color with 8 bits per channel
    using color_8 = color<std::uint8_t>;

    //!<	color with 16 bits per channel
    using color_16 = color<std::uint16_t>;

    //	comparison
    template <typename T> bool operator==(color<T> const &lhs, color<T> const &rhs) { return lhs.rgb_ == rhs.rgb_; }

    template <typename T> bool operator<(color<T> const &lhs, color<T> const &rhs) { return lhs.rgb_ < rhs.rgb_; }

    //
    //	predefined color names
    //
    namespace names {
        // black, red, green, yellow, blue, magenta, cyan, white,
        template <typename T> auto black() -> color<T> { return color<T>(0, 0, 0, 0); }
        template <typename T> auto white() -> color<T> {
            return color<T>(std::numeric_limits<T>::max(), std::numeric_limits<T>::max(), std::numeric_limits<T>::max(), 0);
        }
        template <typename T> auto red() -> color<T> { return color<T>(std::numeric_limits<T>::max(), 0, 0, 0); }
        template <typename T> auto green() -> color<T> { return color<T>(0, std::numeric_limits<T>::max(), 0, 0); }
        template <typename T> auto blue() -> color<T> { return color<T>(0, 0, std::numeric_limits<T>::max(), 0); }
        template <typename T> auto yellow() -> color<T> {
            return color<T>(std::numeric_limits<T>::max(), std::numeric_limits<T>::max(), 0, 0);
        }
        template <typename T> auto magenta() -> color<T> {
            return color<T>(std::numeric_limits<T>::max(), 0, std::numeric_limits<T>::max(), 0);
        }
        template <typename T> auto cyan() -> color<T> {
            return color<T>(0, std::numeric_limits<T>::max(), std::numeric_limits<T>::max(), 0);
        }
    } // namespace names

} // namespace cyng
#include <functional>

namespace std {

    template <typename T> class hash<cyng::color<T>> {
      public:
        size_t operator()(cyng::color<T> const &v) const noexcept {
            std::size_t seed = 0;
            boost::hash_combine(seed, v.red());
            boost::hash_combine(seed, v.green());
            boost::hash_combine(seed, v.blue());
            boost::hash_combine(seed, v.opacity());
            return seed;
        }
    };

} // namespace std
#endif // CYNG_OBJ_INTRINSCIS_COLOR_H
