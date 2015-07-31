#include <dcmtkpp/VR.h>

#include <map>
#include <stdexcept>
#include <string>

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdicent.h>
#include <dcmtk/dcmdata/dcdict.h>
#include <dcmtk/dcmdata/dctagkey.h>

#include "dcmtkpp/Exception.h"
#include "dcmtkpp/Tag.h"

// Anonymous namespace, should not be publicly accessed
namespace
{

#define ADD_TO_MAP(map, vr) map[dcmtkpp::VR::vr] = #vr

std::map<dcmtkpp::VR, std::string>
_build_enum_to_name()
{
    std::map<dcmtkpp::VR, std::string> result;
    ADD_TO_MAP(result, AE);
    ADD_TO_MAP(result, AS);
    ADD_TO_MAP(result, AT);
    ADD_TO_MAP(result, CS);
    ADD_TO_MAP(result, DA);
    ADD_TO_MAP(result, DS);
    ADD_TO_MAP(result, DT);
    ADD_TO_MAP(result, FL);
    ADD_TO_MAP(result, FD);
    ADD_TO_MAP(result, IS);
    ADD_TO_MAP(result, LO);
    ADD_TO_MAP(result, LT);
    ADD_TO_MAP(result, PN);
    ADD_TO_MAP(result, OB);
    ADD_TO_MAP(result, OF);
    ADD_TO_MAP(result, OW);
    ADD_TO_MAP(result, SH);
    ADD_TO_MAP(result, SL);
    ADD_TO_MAP(result, SQ);
    ADD_TO_MAP(result, SS);
    ADD_TO_MAP(result, ST);
    ADD_TO_MAP(result, TM);
    ADD_TO_MAP(result, UC);
    ADD_TO_MAP(result, UI);
    ADD_TO_MAP(result, UL);
    ADD_TO_MAP(result, UN);
    ADD_TO_MAP(result, UR);
    ADD_TO_MAP(result, US);
    ADD_TO_MAP(result, UT);

    return result;
}

#undef ADD_TO_MAP

std::map<std::string, dcmtkpp::VR>
_build_name_to_enum()
{
    std::map<dcmtkpp::VR, std::string> const enum_to_name = _build_enum_to_name();

    std::map<std::string, dcmtkpp::VR> result;
    for(std::map<dcmtkpp::VR, std::string>::const_iterator it = enum_to_name.begin();
        it != enum_to_name.end(); ++it)
    {
        result[it->second] = it->first;
    }

    return result;
}

std::map<dcmtkpp::VR, std::string> const
_enum_to_name = _build_enum_to_name();

std::map<std::string, dcmtkpp::VR> const
_name_to_enum = _build_name_to_enum();

}

namespace dcmtkpp
{

std::string as_string(VR vr)
{
    try
    {
        return _enum_to_name.at(vr);
    }
    catch(std::out_of_range const &)
    {
        throw Exception("Unknown VR");
    }
}

VR as_vr(std::string const vr)
{
    try
    {
        return _name_to_enum.at(vr);
    }
    catch(std::out_of_range const &)
    {
        throw Exception("Unknown VR: "+vr);
    }
}

VR as_vr(Tag const & tag)
{
    DcmTagKey const dcmtk_tag(tag.group, tag.element);
    DcmDictEntry const * entry = dcmDataDict.rdlock().findEntry(dcmtk_tag, NULL);
    dcmDataDict.unlock();

    if(entry == NULL)
    {
        throw Exception("No such element");
    }

    VR const vr(as_vr(std::string(entry->getVR().getValidVRName())));

    return vr;
}

bool is_int(VR vr)
{
    return (
        vr == VR::IS || vr == VR::SL || vr == VR::SS || vr == VR::UL ||
        vr == VR::US);
}

bool is_real(VR vr)
{
    return (vr == VR::DS || vr == VR::FL || vr == VR::FD);
}

bool is_string(VR vr)
{
    return (
        vr == VR::AE || vr == VR::AS || vr == VR::CS || vr == VR::DA ||
        vr == VR::DT || vr == VR::LO || vr == VR::LT || vr == VR::PN ||
        vr == VR::SH || vr == VR::ST || vr == VR::TM || vr == VR::UC ||
        vr == VR::UI || vr == VR::UR || vr == VR::UT);
}

bool is_binary(VR vr)
{
    return (vr == VR::OB || vr == VR::OF || vr == VR::OW || vr == VR::UN);
}


}
