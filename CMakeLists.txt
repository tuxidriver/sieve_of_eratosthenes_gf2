##-*-cmake-*-##################################################################
# Copyright 2015 - 2023 Inesonic, LLC
#
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
#  version.
#
#  This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
# details.
#
# You should have received a copy of the GNU General Public License along with
# this program.  If not, see <http://www.gnu.org/licenses/>.
###############################################################################

cmake_minimum_required(VERSION 3.16.3)
project(sieve_of_eratosthenes_gf2 LANGUAGES C)

find_package(Threads REQUIRED)

add_executable(sieve_of_eratosthenes_gf2
               sieve_of_eratosthenes_gf2
               compiler.c
	       gf2.c
	       prime_list.c
)
target_compile_options(sieve_of_eratosthenes_gf2 PRIVATE -mcmodel=medium)
target_link_libraries(sieve_of_eratosthenes_gf2 Threads::Threads)

add_executable(sieve_of_eratosthenes_memory_gf2
               sieve_of_eratosthenes_memory_gf2
               compiler.c
	       gf2.c
	       prime_list.c
)
target_compile_options(sieve_of_eratosthenes_memory_gf2 PRIVATE -mcmodel=medium)
#target_link_libraries(sieve_of_eratosthenes_memory_gf2 Threads::Threads)

add_executable(list_primes_gf2
               list_primes_gf2
               gf2.c
	       prime_list.c
)
target_compile_options(list_primes_gf2 PRIVATE -mcmodel=medium)
