/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */


#include <Gwork/Gwork.h>
#include <Gwork/Controls/TextBox.h>
#include <Gwork/Skin.h>
#include <Gwork/Utility.h>
#include <Gwork/Platform.h>


using namespace Gwk;
using namespace Gwk::Controls;

GWK_CONTROL_CONSTRUCTOR(TextBoxNumeric)
{
    SetText("0");
}

bool TextBoxNumeric::IsTextAllowed(const Gwk::String& str, int iPos)
{
    const String& strString = GetText();

    if (str.length() == 0)
        return true;

    for (size_t i = 0; i < str.length(); i++)
    {
        if (str[i] == '-')
        {
            // Has to be at the start
            if (i != 0 || iPos != 0)
                return false;

            // Can only be one
            if (std::count(strString.begin(), strString.end(), '-') > 0)
                return false;

            continue;
        }

        if (str[i] >= '0' && str[i] <= '9')
            continue;

        if (str[i] == '.')
        {
            // Already a fullstop
            if (std::count(strString.begin(), strString.end(), '.') > 0)
                return false;

            continue;
        }

        return false;
    }

    return true;
}

float TextBoxNumeric::GetFloatFromText()
{
    float temp = Gwk::Utility::Strings::To::Float(GetText());
    return temp;
}

