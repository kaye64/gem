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

%module(package="gem.api") Item

%{
#include <game/item_definition.h>
#include <game/item.h>
%}

%include "pyabc.i"

%include "game/item_definition.h"

%types(item = item_definition) "return &((item_t*)$from)->definition;";
%pythonabc(item, Definition);
%include "game/item.h"
