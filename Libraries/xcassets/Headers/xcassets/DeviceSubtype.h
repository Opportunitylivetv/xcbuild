/* Copyright 2013-present Facebook. All Rights Reserved. */

#ifndef __xcassets_DeviceSubtype_h
#define __xcassets_DeviceSubtype_h

#include <ext/optional>
#include <string>

namespace xcassets {

/*
 * The subtype of device a launch image is for.
 */
enum class DeviceSubtype {
    Retina4,
    Height667,
    Height736,
};

class DeviceSubtypes {
private:
    DeviceSubtypes();
    ~DeviceSubtypes();

public:
    /*
     * Parse a device subtype from a string.
     */
    static ext::optional<DeviceSubtype> Parse(std::string const &value);

    /*
     * Convert a device subtype to a string.
     */
    static std::string String(DeviceSubtype deviceSubtype);
};

}

#endif // !__xcassets_DeviceSubtype_h
