/*
 * Cppcheck - A tool for static C/C++ code analysis
 * Copyright (C) 2007-2016 Cppcheck team.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


//---------------------------------------------------------------------------
#ifndef checktypeH
#define checktypeH
//---------------------------------------------------------------------------

#include "config.h"
#include "check.h"

/// @addtogroup Checks
/// @{


/** @brief Various small checks */

class CPPCHECKLIB CheckType : public Check {
public:
    /** @brief This constructor is used when registering the CheckClass */
    CheckType() : Check(myName()) {
    }

    /** @brief This constructor is used when running checks. */
    CheckType(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger)
        : Check(myName(), tokenizer, settings, errorLogger) {
    }

    /** @brief Run checks against the normal token list */
    void runChecks(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger) {
        // These are not "simplified" because casts can't be ignored
        CheckType checkType(tokenizer, settings, errorLogger);
        checkType.checkTooBigBitwiseShift();
        checkType.checkIntegerOverflow();
        checkType.checkSignConversion();
        checkType.checkLongCast();
    }

    /** @brief Run checks against the simplified token list */
    void runSimplifiedChecks(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger) {
        (void)tokenizer;
        (void)settings;
        (void)errorLogger;
    }

    /** @brief %Check for bitwise shift with too big right operand */
    void checkTooBigBitwiseShift();

    /** @brief %Check for integer overflow */
    void checkIntegerOverflow();

    /** @brief %Check for dangerous sign conversion */
    void checkSignConversion();

    /** @brief %Check for implicit long cast of int result */
    void checkLongCast();
private:

    // Error messages..
    void tooBigBitwiseShiftError(const Token *tok, int lhsbits, const ValueFlow::Value &rhsbits);
    void integerOverflowError(const Token *tok, const ValueFlow::Value &value);
    void signConversionError(const Token *tok, const bool constvalue);
    void longCastAssignError(const Token *tok);
    void longCastReturnError(const Token *tok);

    void getErrorMessages(ErrorLogger *errorLogger, const Settings *settings) const {
        CheckType c(nullptr, settings, errorLogger);
        c.tooBigBitwiseShiftError(nullptr, 32, ValueFlow::Value(64));
        c.integerOverflowError(nullptr, ValueFlow::Value(1LL<<32));
        c.signConversionError(nullptr, false);
        c.longCastAssignError(nullptr);
        c.longCastReturnError(nullptr);
    }

    static std::string myName() {
        return "Type";
    }

    std::string classInfo() const {
        return "Type checks\n"
               "- bitwise shift by too many bits (only enabled when --platform is used)\n"
               "- signed integer overflow (only enabled when --platform is used)\n"
               "- dangerous sign conversion, when signed value can be negative\n"
               "- possible loss of information when assigning int result to long variable\n"
               "- possible loss of information when returning int result as long return value\n";
    }
};
/// @}
//---------------------------------------------------------------------------
#endif // checktypeH
