// translatepoint.h
//
// Copyright (C) 2004 Jason Bevins
//
// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or (at
// your option) any later version.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License (COPYING.txt) for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// The developer's email is jlbezigvins@gmzigail.com (for great email, take
// off every 'zig'.)
//

#ifndef NOISE_MODULE_TRANSLATEDOMAIN_H
#define NOISE_MODULE_TRANSLATEDOMAIN_H

#include "modulebase.h"

namespace noise
{

    namespace module
    {

        /// @addtogroup libnoise
        /// @{

        /// @addtogroup modules
        /// @{

        /// @addtogroup transformermodules
        /// @{

        /// Default translation factor applied to the @a x coordinate for the
        /// noise::module::TranslatePoint noise module.
        const double DEFAULT_TRANSLATE_POINT_X = 0.0;

        /// Default translation factor applied to the @a y coordinate for the
        /// noise::module::TranslatePoint noise module.
        const double DEFAULT_TRANSLATE_POINT_Y = 0.0;

        /// Default translation factor applied to the @a z coordinate for the
        /// noise::module::TranslatePoint noise module.
        const double DEFAULT_TRANSLATE_POINT_Z = 0.0;

        /// Noise module that moves the coordinates of the input value before
        /// returning the output value from a source module.
        ///
        /// @image html moduletranslatepoint.png
        ///
        /// The getValue() method moves the ( @a x, @a y, @a z ) coordinates of
        /// the input value by a translation amount before returning the output
        /// value from the source module.  To set the translation amount, call
        /// the SetTranslation() method.  To set the translation amount to
        /// apply to the individual @a x, @a y, or @a z coordinates, call the
        /// SetXTranslation(), SetYTranslation() or SetZTranslation() methods,
        /// respectively.
        ///
        /// This noise module requires one source module.
        class TranslateDomain : public ModuleBase
        {

        public:
            /// Constructor.
            ///
            /// The default translation amount to apply to the @a x coordinate is
            /// set to noise::module::DEFAULT_TRANSLATE_POINT_X.
            ///
            /// The default translation amount to apply to the @a y coordinate is
            /// set to noise::module::DEFAULT_TRANSLATE_POINT_Y.
            ///
            /// The default translation amount to apply to the @a z coordinate is
            /// set to noise::module::DEFAULT_TRANSLATE_POINT_Z.
            TranslateDomain(const noise::ScalarParameter& src);
            TranslateDomain(const noise::ScalarParameter& src, const noise::ScalarParameter& xTranslation, const noise::ScalarParameter& yTranslation, const noise::ScalarParameter& zTranslation);

            /// Sets the translation amount to apply to the @a x coordinate of the
            /// input value.
            ///
            /// @param xTranslation The translation amount to apply to the @a x
            /// coordinate.
            ///
            /// The getValue() method moves the ( @a x, @a y, @a z ) coordinates
            /// of the input value by a translation amount before returning the
            /// output value from the source module
            void SetXTranslation(const noise::ScalarParameter& xTranslation);

            /// Returns the translation amount to apply to the @a x coordinate of
            /// the input value.
            ///
            /// @returns The translation amount to apply to the @a x coordinate.
            const noise::ScalarParameter& GetXTranslation() const;

            /// Sets the translation amount to apply to the @a y coordinate of the
            /// input value.
            ///
            /// @param yTranslation The translation amount to apply to the @a y
            /// coordinate.
            ///
            /// The getValue() method moves the ( @a x, @a y, @a z ) coordinates
            /// of the input value by a translation amount before returning the
            /// output value from the source module
            void SetYTranslation(const noise::ScalarParameter& yTranslation);

            /// Returns the translation amount to apply to the @a y coordinate of
            /// the input value.
            ///
            /// @returns The translation amount to apply to the @a y coordinate.
            const noise::ScalarParameter& GetYTranslation() const;

            /// Sets the translation amount to apply to the @a z coordinate of the
            /// input value.
            ///
            /// @param zTranslation The translation amount to apply to the @a z
            /// coordinate.
            ///
            /// The getValue() method moves the ( @a x, @a y, @a z ) coordinates
            /// of the input value by a translation amount before returning the
            /// output value from the source module
            void SetZTranslation(const noise::ScalarParameter& zTranslation);

            /// Returns the translation amount to apply to the @a z coordinate of
            /// the input value.
            ///
            /// @returns The translation amount to apply to the @a z coordinate.
            const noise::ScalarParameter& GetZTranslation() const;

            /// Sets the translation amounts to apply to the ( @a x, @a y, @a z )
            /// coordinates of the input value.
            ///
            /// @param xTranslation The translation amount to apply to the @a x
            /// coordinate.
            /// @param yTranslation The translation amount to apply to the @a y
            /// coordinate.
            /// @param zTranslation The translation amount to apply to the @a z
            /// coordinate.
            ///
            /// The getValue() method moves the ( @a x, @a y, @a z ) coordinates
            /// of the input value by a translation amount before returning the
            /// output value from the source module
            void SetTranslation(const noise::ScalarParameter& xTranslation, const noise::ScalarParameter& yTranslation, const noise::ScalarParameter& zTranslation);

            virtual double getValue(double x, double y, double z) const override;

        protected:
            noise::ScalarParameter m_source;

            /// Translation amount applied to the @a x coordinate of the input
            /// value.
            noise::ScalarParameter m_xTranslation;

            /// Translation amount applied to the @a y coordinate of the input
            /// value.
            noise::ScalarParameter m_yTranslation;

            /// Translation amount applied to the @a z coordinate of the input
            /// value.
            noise::ScalarParameter m_zTranslation;
        };

        /// @}

        /// @}

        /// @}

    } // namespace module

} // namespace noise

#endif
