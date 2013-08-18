/**
 *  This file is part of Gem.
 *
 *  Gem is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Gem is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Gem.  If not, see <http://www.gnu.org/licenses/\>.
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#define DEFAULT_BUFFER_SIZE 4096
#define CLIENT_REVISION 317

/* This RSA keypair is public and insecure. It is here for development purposes only. */
#define RSA_MODULUS "136556218291674739898111430431188124027470233924410611154992916496380451385494837854853557705806754337088961796638990314499064316649889602602899420061608809188549202142090401262413669068708760489853080994662587664092535054400054072364518440232031467904581451895140552971397336210123919744191654278662248952533"
#define RSA_PUBLIC_EXPONENT "65537"
#define RSA_PRIVATE_EXPONENT "34284389211761541881433777504535901746311171231399853455975303233767855518210050232140019202760949324236107502661060876066460232634348284499261593568422585131542298140083653918739205482720580931164166929807395075323860751535048520361931691064978536794530332753422111138011371216340021385839886563079659167369"

/* Uncomment the following line to disable RSA block encryption */
//#define RSA_DISABLE

#endif /* _CONFIG_H_ */
