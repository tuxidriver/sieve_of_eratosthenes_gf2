/*-*-c++-*-*************************************************************************************************************
* Copyright (C) 2015 by Paul H. Smith
*
* This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
* License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later
* version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program.  If not, see
* <http://www.gnu.org/licenses/>.
********************************************************************************************************************//**
* \file
* \brief Defines functions to perform math against polynomials in a GF(2) field.
*
* This file defines functions used to perform math functions against prime polynomials in a GF(2) field.
***********************************************************************************************************************/

#ifndef GF2_H
#define GF2_H

#include <stdint.h>
#include "compiler.h"

/*******************************************************************************************************************//**
* \brief Type that is used to represent a polynomial in compact form.
*
* You can use this type to represent a polynomial in compact form.
***********************************************************************************************************************/
typedef uint64_t Gf2Polynomial;

/*******************************************************************************************************************//**
* \brief Function that adds two polynomials in a GF(2) field.
*
* You can use this function to add two polynomials in a GF(2) field.
*
* \param[in] p1 The first polynomial to add.
*
* \param[in] p2 The second polynomial to add.
*
* \return Returns the sum.
***********************************************************************************************************************/
INLINE Gf2Polynomial gf2Add(Gf2Polynomial const p1, Gf2Polynomial const p2) {
    return p1 ^ p2;
}

/***********************************************************************************************************************
* \brief Function that multiplies two polynomials in a GF(2) field.
*
* You can use this function to multiply two polynomials in a GF(2) field.
*
* \param[in] p1 The first polynomial to multiply.
*
* \param[in] p2 The second polynomial to multiply.
*
* \return Returns the product.
***********************************************************************************************************************/
Gf2Polynomial gf2Multiply(Gf2Polynomial const p1, Gf2Polynomial const p2);

/***********************************************************************************************************************
* \brief Function that divides two polynomials in a GF(2) field.
*
* You can use this function to divide one polynomials by another in a GF(2) field.
*
* \param[in] dividend          The dividend to divide.
*
* \param[in] divisor           The divisor to divide the dividend by.  Note that the value must not be 0.
*
* \param[in] optionalRemainder Optional pointer to the remainder.
*
* \returns the quotient of the division.
***********************************************************************************************************************/
Gf2Polynomial gf2Divide(Gf2Polynomial const dividend, Gf2Polynomial const divisor, Gf2Polynomial* optionalRemainder);

/***********************************************************************************************************************
* \brief Function that divides two polynomials in a GF(2) field and calculates just the remainder.
*
* You can use this function to divide one polynomials by another in a GF(2) field.
*
* \param[in] dividend The dividend to divide.
*
* \param[in] divisor  The divisor to divide the dividend by.  Note that the value must not be 0.
*
* \return Returns the remainder.
***********************************************************************************************************************/
Gf2Polynomial gf2Remainder(Gf2Polynomial const dividend, Gf2Polynomial const divisor);

#endif
