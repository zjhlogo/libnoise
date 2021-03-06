// select.h
//
// Copyright (C) 2003, 2004 Jason Bevins
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

#ifndef NOISE_MODULE_SELECT_H
#define NOISE_MODULE_SELECT_H

#include "modulebase.h"

namespace noise
{
    namespace module
    {
        /// @addtogroup libnoise
        /// @{

        /// @addtogroup modules
        /// @{

        /// @addtogroup selectormodules
        /// @{

        /// Default threshold of the selection for the
        /// noise::module::Select noise module.
        const double DEFAULT_SELECT_THRESHOLD = 1.0;

        /// Default edge-falloff value for the noise::module::Select noise module.
        const double DEFAULT_SELECT_EDGE_FALLOFF = 0.0;

        /// Noise module that outputs the value selected from one of two source
        /// modules chosen by the output value from a control module.
        ///
        /// @image html moduleselect.png
        ///
        /// Unlike most other noise modules, the index value assigned to a source
        /// module determines its role in the selection operation:
        /// - Source module 0 (upper left in the diagram) outputs a value.
        /// - Source module 1 (lower left in the diagram) outputs a value.
        /// - Source module 2 (bottom of the diagram) is known as the <i>control
        ///   module</i>.  The control module determines the value to select.  If
        ///   the output value from the control module is within a range of values
        ///   known as the <i>selection range</i>, this noise module outputs the
        ///   value from the source module with an index value of 1.  Otherwise,
        ///   this noise module outputs the value from the source module with an
        ///   index value of 0.
        ///
        /// To specify the bounds of the selection range, call the SetBounds()
        /// method.
        ///
        /// An application can pass the control module to the SetControlModule()
        /// method instead of the SetSourceModule() method.  This may make the
        /// application code easier to read.
        ///
        /// By default, there is an abrupt transition between the output values
        /// from the two source modules at the selection-range boundary.  To
        /// smooth the transition, pass a non-zero value to the SetEdgeFalloff()
        /// method.  Higher values result in a smoother transition.
        ///
        /// This noise module requires three source modules.
        class Select : public ModuleBase
        {
        public:
            /// Constructor.
            ///
            /// The default threshold of the selection is set to
            /// noise::module::DEFAULT_SELECT_THRESHOLD.
            ///
            /// The default falloff value at the edge transition is set to
            /// noise::module::DEFAULT_SELECT_EDGE_FALLOFF.
            Select(const noise::ScalarParameter& low, const noise::ScalarParameter& high, const noise::ScalarParameter& control);
            Select(const noise::ScalarParameter& low, const noise::ScalarParameter& high, const noise::ScalarParameter& control, const noise::ScalarParameter& threshold, const noise::ScalarParameter& edgeFalloff);

            void setLowModule(const noise::ScalarParameter& low);
            const noise::ScalarParameter& getLowModule() const;

            void setHighModule(const noise::ScalarParameter& high);
            const noise::ScalarParameter& getHighModule() const;

            /// Sets the control module.
            ///
            /// @param controlModule The control module.
            ///
            /// The control module determines the output value to select.  If the
            /// output value from the control module is within a range of values
            /// known as the <i>selection range</i>, the getValue() method outputs
            /// the value from the source module with an index value of 1.
            /// Otherwise, this method outputs the value from the source module
            /// with an index value of 0.
            ///
            /// This method assigns the control module an index value of 2.
            /// Passing the control module to this method produces the same
            /// results as passing the control module to the SetSourceModule()
            /// method while assigning that noise module an index value of 2.
            ///
            /// This control module must exist throughout the lifetime of this
            /// noise module unless another control module replaces that control
            /// module.
            void setControlModule(const noise::ScalarParameter& control);

            /// Returns the control module.
            ///
            /// @returns A reference to the control module.
            ///
            /// @pre A control module has been added to this noise module via a
            /// call to SetSourceModule() or SetControlModule().
            ///
            /// @throw noise::ExceptionNoModule See the preconditions for more
            /// information.
            ///
            /// The control module determines the output value to select.  If the
            /// output value from the control module is within a range of values
            /// known as the <i>selection range</i>, the getValue() method outputs
            /// the value from the source module with an index value of 1.
            /// Otherwise, this method outputs the value from the source module
            /// with an index value of 0.
            const noise::ScalarParameter& getControlModule() const;

            /// Sets the lower and upper bounds of the selection range.
            ///
            /// @param threshold The lower bound.
            ///
            /// If the output value from the control module is less then the
            /// threshold, the getValue() method outputs the value from the
            /// low module. Otherwise, this method outputs the value from the high
            /// module.
            void setThreshold(const noise::ScalarParameter& threshold);

            /// Returns the lower bound of the selection range.
            ///
            /// @returns The lower bound of the selection range.
            ///
            /// If the output value from the control module is within the
            /// selection range, the getValue() method outputs the value from the
            /// source module with an index value of 1.  Otherwise, this method
            /// outputs the value from the source module with an index value of 0.
            const noise::ScalarParameter& getThreshold() const;

            /// Sets the falloff value at the edge transition.
            ///
            /// @param edgeFalloff The falloff value at the edge transition.
            ///
            /// The falloff value is the width of the edge transition at either
            /// edge of the selection range.
            ///
            /// By default, there is an abrupt transition between the values from
            /// the two source modules at the boundaries of the selection range.
            ///
            /// For example, if the selection range is 0.5 to 0.8, and the edge
            /// falloff value is 0.1, then the getValue() method outputs:
            /// - the output value from the source module with an index value of 0
            ///   if the output value from the control module is less than 0.4
            ///   ( = 0.5 - 0.1).
            /// - a linear blend between the two output values from the two source
            ///   modules if the output value from the control module is between
            ///   0.4 ( = 0.5 - 0.1) and 0.6 ( = 0.5 + 0.1).
            /// - the output value from the source module with an index value of 1
            ///   if the output value from the control module is between 0.6
            ///   ( = 0.5 + 0.1) and 0.7 ( = 0.8 - 0.1).
            /// - a linear blend between the output values from the two source
            ///   modules if the output value from the control module is between
            ///   0.7 ( = 0.8 - 0.1 ) and 0.9 ( = 0.8 + 0.1).
            /// - the output value from the source module with an index value of 0
            ///   if the output value from the control module is greater than 0.9
            ///   ( = 0.8 + 0.1).
            void setEdgeFalloff(const noise::ScalarParameter& edgeFalloff);

            /// Returns the falloff value at the edge transition.
            ///
            /// @returns The falloff value at the edge transition.
            ///
            /// The falloff value is the width of the edge transition at either
            /// edge of the selection range.
            ///
            /// By default, there is an abrupt transition between the output
            /// values from the two source modules at the selection-range
            /// boundary.
            const noise::ScalarParameter& getEdgeFalloff() const;

            virtual double getValue(double x, double y, double z) const override;

        protected:
            noise::ScalarParameter m_low;
            noise::ScalarParameter m_high;
            noise::ScalarParameter m_control;

            /// threshold of the selection
            noise::ScalarParameter m_threshold;

            /// Edge-falloff value.
            noise::ScalarParameter m_edgeFalloff;
        };

        /// @}

        /// @}

        /// @}

    } // namespace module

} // namespace noise

#endif
