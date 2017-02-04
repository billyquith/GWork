//
//  GworkUtil.cpp.
//

#include "GworkUtil.h"

#include <fstream>
#include <iostream>
#include <stdarg.h>

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filestream.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/writer.h"

#ifdef _MSC_VER
#   define vsnprintf vsnprintf_s
#   define sscanf sscanf_s
#endif


namespace GwkUtil
{
namespace String
{
    namespace Convert
    {
        BString FromWide(const WString& strIn)
        {
            const BString nstr(strIn.begin(), strIn.end());
            return nstr;
        }

        WString ToWide(const BString& strIn)
        {
            WString temp(strIn.length(), L' ');
            std::copy(strIn.begin(), strIn.end(), temp.begin());
            return temp;
        }

    } // Convert

    namespace Format
    {
        BString Print(const char *fmt, ...)
        {
            va_list s;
            va_start(s, fmt);
            char strOut[1024];
            vsnprintf(strOut, sizeof(strOut), fmt, s);
            va_end(s);

            return strOut;
        }

        WString Print(const wchar_t *fmt, ...)
        {
            va_list s;
            va_start(s, fmt);
            wchar_t strOut[1024];
            vswprintf(strOut, sizeof(strOut), fmt, s);
            va_end(s);

            return strOut;
        }

        BString NiceFloat(float f)
        {
            BString strFloat = Print("%.04f", f);

    //              Util::TrimRight( strFloat, "0" );
    //              Util::TrimRight( strFloat, "." );

            return strFloat;
        }

        BString NiceDouble(double f)
        {
            BString strFloat = Print("%.04f", f);

    //              Util::TrimRight( strFloat, "0" );
    //              Util::TrimRight( strFloat, "." );

            return strFloat;
        }

        BString BinaryToHex(const void* data, unsigned int iLength)
        {
            BString str = "";
            const unsigned char* bytes = (const unsigned char*)data;

            for (unsigned int i = 0; i < iLength; i++)
            {
                str += Print("%02x", bytes[i]);
            }

            return str;
        }

        BString UInt64(unsigned long long iBytes)
        {
            return Print("%llu", iBytes);
        }

        BString Int(int iBytes)
        {
            return Print("%i", iBytes);
        }

    } // Format

    namespace To
    {
        int Int(const BString& str)
        {
            if (str == "")
                return 0;
            return atoi(str.c_str());
        }

        unsigned long long UInt64(const BString& str)   // TODO - use stdint?
        {
            unsigned long long val = 0;
            sscanf(str.c_str(), "%llu", &val);
            return val;
        }

        float Float(const BString& str)
        {
            if (str == "")
                return 0.0f;
            return static_cast<float>(atof(str.c_str()));   // TODO - error handling?
        }

        double Double(const BString& str)
        {
            if (str == "")
                return 0.0;
            return atof(str.c_str());   // TODO - error handling?
        }

        bool Bool(const BString& str)
        {
            if (str.size() == 0)
                return false;
            if (str[0] == 'T' || str[0] == 't' || str[0] == 'y' || str[0] == 'Y')
                return true;
            if (str[0] == 'F' || str[0] == 'f' || str[0] == 'n' || str[0] == 'N')
                return false;
            if (str[0] == '0')
                return false;
            return true;
        }

    } // To

} // String

namespace File
{
bool Read(const BString& strFileName, BString& strOut)
{
    std::ifstream f(strFileName.c_str(), std::ios_base::in);

    if (!f.is_open())
        return false;

    strOut = BString(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
    return true;
}

bool Write(const BString& strFileName, const BString& strOut)
{
    std::ofstream f(strFileName.c_str(), std::ios_base::out);

    if (!f.is_open())
        return false;

    f.write(strOut.c_str(), strOut.length());
    f.close();

    return true;
}

} // File

namespace Data
{
namespace Json
{
    template <typename Writer>
    void WriteValue(Writer& writer, const GwkUtil::Data::Tree& tree)
    {
        if (tree.IsVar<float>())
        {
            writer.Double(tree.Var<float>());
            return;
        }

        if (tree.IsVar<double>())
        {
            writer.Double(tree.Var<double>());
            return;
        }

        if (tree.IsVar<int>())
        {
            writer.Int(tree.Var<int>());
            return;
        }

        if (tree.IsVar<bool>())
        {
            writer.Bool(tree.Var<bool>());
            return;
        }

        writer.String(tree.Value().c_str());
    }

    template <typename Writer>
    void DoJsonExport(Writer& writer, const GwkUtil::Data::Tree& tree)
    {
        //
        // If none of the children have keys then it's an array
        //
        bool IsArray = true;
        GWKUTIL_FOREACH_CONST(a, tree.Children(), GwkUtil::Data::Tree::List)
        {
            if (!a->Name().empty())
            {
                IsArray = false;
                break;
            }
        }

        if (IsArray)
            writer.StartArray();
        else
            writer.StartObject();

        int iUniqueKey = 1;

        GWKUTIL_FOREACH_CONST(a, tree.Children(), GwkUtil::Data::Tree::List)
        {
            const GwkUtil::BString& strKey = a->Name();

            // If we're not an array then write the key
            if (!IsArray)
            {
                if (strKey.empty())
                {
                    writer.String(GwkUtil::String::Format::Print("_%i_",
                                                                  iUniqueKey).c_str());
                    iUniqueKey++;
                }
                else
                {
                    writer.String(strKey.c_str());
                }
            }


            if (a->HasChildren() || a->IsBranch())
            {
                // If it's a parent we ignore the value and
                // just treat it like an object/array
                DoJsonExport<Writer>(writer, *a);
            }
            else
            {
                WriteValue(writer, *a);
            }
        }

        if (IsArray)
            writer.EndArray();
        else
            writer.EndObject();
    }

    bool Export(const GwkUtil::Data::Tree& tree, GwkUtil::BString& output, bool bPretty)
    {
        rapidjson::StringBuffer f;

        if (bPretty)
        {
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(f);
            writer.SetIndent('\t', 1);
            DoJsonExport<rapidjson::PrettyWriter<rapidjson::StringBuffer> >(writer, tree);
        }
        else
        {
            rapidjson::Writer<rapidjson::StringBuffer> writer(f);
            DoJsonExport<rapidjson::Writer<rapidjson::StringBuffer> >(writer, tree);
        }

        output = f.GetString();
        return true;
    }

    void DoImport(GwkUtil::Data::Tree& tree, rapidjson::Document::ValueType& doc)
    {
        if (doc.IsArray())
        {
            rapidjson::Document::ValueType::ValueIterator it = doc.Begin();
            while (it != doc.End())
            {
                if (it->IsObject() || it->IsArray())
                {
                    GwkUtil::Data::Tree& child = tree.AddChild();
                    DoImport(child, *it);
                }

                if (it->IsString())
                {
                    GwkUtil::Data::Tree& child = tree.AddChild();
                    child.Value(it->GetString());
                }

                if (it->IsBool())
                {
                    GwkUtil::Data::Tree& child = tree.AddChild();
                    child.Var<bool>(it->GetBool());
                }

                if (it->IsInt())
                {
                    GwkUtil::Data::Tree& child = tree.AddChild();
                    child.Var<int>(it->GetInt());
                }

                if (it->IsNumber())
                {
                    GwkUtil::Data::Tree& child = tree.AddChild();
                    child.Var<float>( static_cast<float>(it->GetDouble()) );
                }

                ++it;
            }
        }

        if (doc.IsObject())
        {
            rapidjson::Document::MemberIterator it = doc.MemberBegin();
            while (it != doc.MemberEnd())
            {
                if (it->value.IsObject() || it->value.IsArray())
                {
                    GwkUtil::Data::Tree& child = tree.AddChild(it->name.GetString());
                    DoImport(child, it->value);
                }

                if (it->value.IsString())
                    tree.SetChild(it->name.GetString(), it->value.GetString());
                else if (it->value.IsBool())
                    tree.SetChildVar<bool>(it->name.GetString(), it->value.GetBool());
                else if (it->value.IsInt())
                    tree.SetChildVar<int>(it->name.GetString(), it->value.GetInt());
                else if (it->value.IsNumber())
                    tree.SetChildVar<float>(it->name.GetString(), static_cast<float>( it->value.GetDouble() ));

                ++it;
            }
        }

    }

    bool Import(GwkUtil::Data::Tree& tree, const GwkUtil::BString& input)
    {
        rapidjson::Document doc;

        if (doc.Parse<0>(input.c_str()).HasParseError())
            return false;

        if (doc.IsObject() || doc.IsArray())
            DoImport(tree, doc);

        return true;
    }

}

} // Data

} // GwkUtil
