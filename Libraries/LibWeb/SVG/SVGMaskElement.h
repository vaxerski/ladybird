/*
 * Copyright (c) 2023, Andreas Kling <andreas@ladybird.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <LibWeb/SVG/AttributeParser.h>
#include <LibWeb/SVG/SVGGraphicsElement.h>
#include <LibWeb/SVG/SVGViewport.h>

namespace Web::SVG {

class SVGMaskElement final : public SVGGraphicsElement
    , public SVGViewport {

    WEB_PLATFORM_OBJECT(SVGMaskElement, SVGGraphicsElement);
    GC_DECLARE_ALLOCATOR(SVGMaskElement);

public:
    virtual ~SVGMaskElement() override;

    virtual Optional<ViewBox> view_box() const override
    {
        // maskContentUnits = objectBoundingBox acts like the mask is sized to the bounding box
        // of the target element, with a viewBox of "0 0 1 1".
        if (mask_content_units() == MaskContentUnits::ObjectBoundingBox)
            return ViewBox { 0, 0, 1, 1 };
        return {};
    }

    virtual Optional<PreserveAspectRatio> preserve_aspect_ratio() const override
    {
        // preserveAspectRatio = none (allow mask to be scaled in both x and y to match target size)
        return PreserveAspectRatio { PreserveAspectRatio::Align::None, {} };
    }

    virtual void attribute_changed(FlyString const& name, Optional<String> const& old_value, Optional<String> const& value, Optional<FlyString> const& namespace_) override;

    virtual GC::Ptr<Layout::Node> create_layout_node(GC::Ref<CSS::ComputedProperties>) override;

    CSSPixelRect resolve_masking_area(CSSPixelRect const& mask_target) const;

    MaskContentUnits mask_content_units() const;
    MaskUnits mask_units() const;

private:
    SVGMaskElement(DOM::Document&, DOM::QualifiedName);
    virtual void initialize(JS::Realm&) override;

    Optional<MaskContentUnits> m_mask_content_units = {};
    Optional<MaskUnits> m_mask_units = {};
};

}
