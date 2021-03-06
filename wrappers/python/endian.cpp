/*************************************************************************
 * odil - Copyright (C) Universite de Strasbourg
 * Distributed under the terms of the CeCILL-B license, as published by
 * the CEA-CNRS-INRIA. Refer to the LICENSE file or to
 * http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
 * for details.
 ************************************************************************/

#include <pybind11/pybind11.h>

#include <odil/endian.h>

#include "opaque_types.h"
#include "type_casters.h"

void wrap_endian(pybind11::module & m)
{
    using namespace pybind11;
    using namespace odil;

    enum_<ByteOrdering>(m, "ByteOrdering")
        .value("LittleEndian", ByteOrdering::LittleEndian)
        .value("BigEndian", ByteOrdering::BigEndian)
    ;
}
