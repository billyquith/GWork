/*
 *  GWEN
 *  Copyright (c) 2010 Facepunch Studios
 *  See license in Gwen.h
 */


#include "Gwen/Gwen.h"
#include "Gwen/Controls/TextBox.h"
#include "Gwen/Skin.h"
#include "Gwen/Utility.h"
#include "Gwen/Platform.h"


using namespace Gwen;
using namespace Gwen::Controls;

GWEN_CONTROL_CONSTRUCTOR(TextBoxNumeric)
{
    SetText("0");
}

bool TextBoxNumeric::IsTextAllowed(const Gwen::String& str, int iPos)
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
    float temp = Gwen::Utility::Strings::To::Float(GetText());
    return temp;
}

