//
//  GwenUtil.cpp.
//

#include "GwenUtil.h"

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


namespace GwenUtil {
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

            BString BinaryToHex(const void* pData, unsigned int iLength)
            {
                BString str = "";
                const unsigned char* pBytes = (const unsigned char*)pData;

                for (unsigned int i = 0; i < iLength; i++)
                {
                    str += Print("%02x", pBytes[i]);
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

            unsigned long long UInt64(const BString& str)
            {
                unsigned long long val = 0;
                sscanf(str.c_str(), "%llu", &val);
                return val;
            }

            float Float(const BString& str)
            {
                if (str == "")
                    return 0.0f;
                return (float)atof(str.c_str());
            }

            double Double(const BString& str)
            {
                if (str == "")
                    return 0.0f;
                return atof(str.c_str());
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
        // String (generic value)
        template <>
        template <>
        unsigned char Tree::VarID<GwenUtil::BString>() const
        {
            return 1;
        }

        // Float
        template <>
        template <>
        unsigned char Tree::VarID<float>() const
        {
            return 2;
        }

        template <>
        template <>
        GwenUtil::BString Tree::VarToString(float var) const
        {
            return GwenUtil::String::Format::NiceFloat(var);
        }

        template <>
        template <>
        float Tree::StringToVar(const GwenUtil::BString& var) const
        {
            return GwenUtil::String::To::Float(var);
        }

        // int
        template <>
        template <>
        unsigned char Tree::VarID<int>() const
        {
            return 3;

        }

        template <>
        template <>
        GwenUtil::BString Tree::VarToString(int var) const
        {
            return GwenUtil::String::Format::Int(var);
        }

        template <>
        template <>
        int Tree::StringToVar(const GwenUtil::BString& var) const
        {
            return GwenUtil::String::To::Int(var);
        }

        // Bool
        template <>
        template <>
        unsigned char Tree::VarID<bool>() const
        {
            return 4;
        }

        template <>
        template <>
        GwenUtil::BString Tree::VarToString(bool var) const
        {
            return var ? "true" : "false";
        }

        template <>
        template <>
        bool Tree::StringToVar(const GwenUtil::BString& var) const
        {
            return GwenUtil::String::To::Bool(var);
        }

        // Double
        template <>
        template <>
        unsigned char Tree::VarID<double>() const
        {
            return 5;
        }

        template <>
        template <>
        GwenUtil::BString Tree::VarToString(double var) const
        {
            return GwenUtil::String::Format::NiceDouble(var);
        }

        template <>
        template <>
        double Tree::StringToVar(const GwenUtil::BString& var) const
        {
            return GwenUtil::String::To::Double(var);
        }

        namespace Json
        {
            template <typename Writer>
            void WriteValue(Writer& writer, const GwenUtil::Data::Tree& tree)
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
            void DoJsonExport(Writer& writer, const GwenUtil::Data::Tree& tree)
            {
                //
                // If none of the children have keys then it's an array
                //
                bool IsArray = true;
                GWENUTIL_FOREACH_CONST(a, tree.Children(), GwenUtil::Data::Tree::List)
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

                GWENUTIL_FOREACH_CONST(a, tree.Children(), GwenUtil::Data::Tree::List)
                {
                    const GwenUtil::BString& strKey = a->Name();

                    // If we're not an array then write the key
                    if (!IsArray)
                    {
                        if (strKey.empty())
                        {
                            writer.String(GwenUtil::String::Format::Print("_%i_",
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

            bool Export(const GwenUtil::Data::Tree& tree, GwenUtil::BString& output, bool bPretty)
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

            void DoImport(GwenUtil::Data::Tree& tree, rapidjson::Document::ValueType& doc)
            {
                if (doc.IsArray())
                {
                    rapidjson::Document::ValueType::ValueIterator it = doc.Begin();
                    while (it != doc.End())
                    {
                        if (it->IsObject() || it->IsArray())
                        {
                            GwenUtil::Data::Tree& child = tree.AddChild();
                            DoImport(child, *it);
                        }

                        if (it->IsString())
                        {
                            GwenUtil::Data::Tree& child = tree.AddChild();
                            child.Value(it->GetString());
                        }

                        if (it->IsBool())
                        {
                            GwenUtil::Data::Tree& child = tree.AddChild();
                            child.Var<bool>(it->GetBool());
                        }

                        if (it->IsInt())
                        {
                            GwenUtil::Data::Tree& child = tree.AddChild();
                            child.Var<int>(it->GetInt());
                        }

                        if (it->IsNumber())
                        {
                            GwenUtil::Data::Tree& child = tree.AddChild();
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
                            GwenUtil::Data::Tree& child = tree.AddChild(it->name.GetString());
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

            bool Import(GwenUtil::Data::Tree& tree, const GwenUtil::BString& input)
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

} // GwenUtil
