/* Copyright 2013-present Facebook. All Rights Reserved. */

#include <xcassets/ImageSet.h>
#include <plist/Keys/Unpack.h>
#include <plist/Array.h>
#include <plist/Boolean.h>
#include <plist/String.h>

using xcassets::ImageSet;

bool ImageSet::Image::
parse(plist::Dictionary const *dict)
{
    std::unordered_set<std::string> seen;
    auto unpack = plist::Keys::Unpack("ImageSetImage", dict, &seen);

    auto F   = unpack.cast <plist::String> ("filename");
    auto GFS = unpack.cast <plist::String> ("graphics-feature-set");
    auto I   = unpack.cast <plist::String> ("idiom");
    auto M   = unpack.cast <plist::String> ("memory");
    auto S   = unpack.cast <plist::String> ("scale");
    auto ST  = unpack.cast <plist::String> ("subtype");
    // TODO: screen-width
    auto WC  = unpack.cast <plist::String> ("width-class");
    auto HC  = unpack.cast <plist::String> ("height-class");
    auto U   = unpack.cast <plist::Boolean> ("unassigned");
    auto AI  = unpack.cast <plist::Dictionary> ("alignment-insets");
    auto R   = unpack.cast <plist::Dictionary> ("resizing");

    if (!unpack.complete(true)) {
        fprintf(stderr, "%s", unpack.errorText().c_str());
    }

    if (F != nullptr) {
        _fileName = F->value();
    }

    if (GFS != nullptr) {
        _graphicsFeatureSet = GraphicsFeatureSets::Parse(GFS->value());
    }

    if (I != nullptr) {
        _idiom = Idioms::Parse(I->value());
    }

    if (M != nullptr) {
        _memory = MemoryRequirements::Parse(M->value());
    }

    if (S != nullptr) {
        _scale = Scale::Parse(S->value());
    }

    if (ST != nullptr) {
        _subtype = DeviceSubtypes::Parse(ST->value());
    }

    if (WC != nullptr) {
        _widthClass = SizeClasses::Parse(WC->value());
    }

    if (HC != nullptr) {
        _heightClass = SizeClasses::Parse(HC->value());
    }

    if (U != nullptr) {
        _unassigned = U->value();
    }

    if (AI != nullptr) {
        Insets alignmentInsets;
        if (alignmentInsets.parse(AI)) {
            _alignmentInsets = alignmentInsets;
        }
    }

    if (R != nullptr) {
        Resizing resizing;
        if (resizing.parse(R)) {
            _resizing = resizing;
        }
    }

    return true;
}

bool ImageSet::
parse(plist::Dictionary const *dict, std::unordered_set<std::string> *seen, bool check)
{
    if (!Asset::parse(dict, seen, false)) {
        return false;
    }

    /* Contents is required. */
    if (dict == nullptr) {
        return false;
    }

    auto unpack = plist::Keys::Unpack("ImageSet", dict, seen);

    auto P  = unpack.cast <plist::Dictionary> ("properties");
    auto Is = unpack.cast <plist::Array> ("images");

    if (!unpack.complete(check)) {
        fprintf(stderr, "%s", unpack.errorText().c_str());
    }

    if (P != nullptr) {
        std::unordered_set<std::string> seen;
        auto unpack = plist::Keys::Unpack("Properties", P, &seen);

        auto TRI  = unpack.cast <plist::String> ("template-rendering-intent");
        auto ODRT = unpack.cast <plist::Array> ("on-demand-resource-tags");

        if (!unpack.complete(true)) {
            fprintf(stderr, "%s", unpack.errorText().c_str());
        }

        if (TRI != nullptr) {
            _templateRenderingIntent = TemplateRenderingIntents::Parse(TRI->value());
        }

        if (ODRT != nullptr) {
            _onDemandResourceTags = std::vector<std::string>();
            _onDemandResourceTags->reserve(ODRT->count());

            for (size_t n = 0; n < ODRT->count(); n++) {
                if (auto string = ODRT->value<plist::String>(n)) {
                    _onDemandResourceTags->push_back(string->value());
                }
            }
        }
    }

    if (Is != nullptr) {
        _images = std::vector<Image>();

        for (size_t n = 0; n < Is->count(); ++n) {
            if (auto dict = Is->value<plist::Dictionary>(n)) {
                Image image;
                if (image.parse(dict)) {
                    _images->push_back(image);
                }
            }
        }
    }

    // TODO: confirm no children

    return true;
}


