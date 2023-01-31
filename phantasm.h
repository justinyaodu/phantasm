/*
 * MIT License
 *
 * Copyright (c) 2023 Justin Yao Du
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef PHANTASM_H
#define PHANTASM_H

#include <stdint.h>

uint64_t _pasm_registers[16];
#define r0 (&_pasm_registers[0])
#define r1 (&_pasm_registers[1])
#define r2 (&_pasm_registers[2])
#define r3 (&_pasm_registers[3])
#define r4 (&_pasm_registers[4])
#define r5 (&_pasm_registers[5])
#define r6 (&_pasm_registers[6])
#define r7 (&_pasm_registers[7])
#define r8 (&_pasm_registers[8])
#define r9 (&_pasm_registers[9])
#define r10 (&_pasm_registers[10])
#define r11 (&_pasm_registers[11])
#define r12 (&_pasm_registers[12])
#define r13 (&_pasm_registers[13])
#define r14 (&_pasm_registers[14])
#define r15 (&_pasm_registers[15])

uint64_t _pasm_immediate;

uint64_t* _pasm_immediate_value(uint64_t value) {
	_pasm_immediate = value;
	return &_pasm_immediate;
}
#define $(x) (_pasm_immediate_value(x))

uint64_t* _pasm_immediate_ptr(void* ptr) {
	_pasm_immediate = (uint64_t) ptr;
	return &_pasm_immediate;
}
#define $$(x) (_pasm_immediate_ptr(x))

uint64_t* _pasm_dest;
uint64_t _pasm_left;
uint64_t _pasm_right;

typedef void (*_pasm_partial1)(uint64_t*);
typedef _pasm_partial1 (*_pasm_partial2)(uint64_t*);

// add DEST LEFT RIGHT: set DEST to LEFT + RIGHT

void _pasm_add1(uint64_t* right) {
	*_pasm_dest = _pasm_left + *right;
}

_pasm_partial1 _pasm_add2(uint64_t* left) {
	_pasm_left = *left;
	return _pasm_add1;
}

_pasm_partial2 add(uint64_t* dest) {
	_pasm_dest = dest;
	return _pasm_add2;
}

// mov DEST SRC: set DEST to SRC

void _pasm_mov1(uint64_t* src) {
	*_pasm_dest = *src;
}

_pasm_partial1 mov(uint64_t* dest) {
	_pasm_dest = dest;
	return _pasm_mov1;
}

// mul DEST LEFT RIGHT: set DEST to LEFT * RIGHT

void _pasm_mul1(uint64_t* right) {
	*_pasm_dest = _pasm_left * *right;
}

_pasm_partial1 _pasm_mul2(uint64_t* left) {
	_pasm_left = *left;
	return _pasm_mul1;
}

_pasm_partial2 mul(uint64_t* dest) {
	_pasm_dest = dest;
	return _pasm_mul2;
}

// sub DEST LEFT RIGHT: set DEST to LEFT - RIGHT

void _pasm_sub1(uint64_t* right) {
	*_pasm_dest = _pasm_left - *right;
}

_pasm_partial1 _pasm_sub2(uint64_t* left) {
	_pasm_left = *left;
	return _pasm_sub1;
}

_pasm_partial2 sub(uint64_t* dest) {
	_pasm_dest = dest;
	return _pasm_sub2;
}

// jmp LABEL: jump to LABEL

#define jmp	goto

// cmp LEFT RIGHT: use LEFT and RIGHT for the next conditional jump

void _pasm_cmp1(uint64_t* right) {
	_pasm_right = *right;
}

_pasm_partial1 cmp(uint64_t* left) {
	_pasm_left = *left;
	return _pasm_cmp1;
}

// j[condition] LABEL: jump to LABEL if LEFT [condition] RIGHT is true

#define je	if (_pasm_left == _pasm_right) goto
#define jne	if (_pasm_left != _pasm_right) goto
#define jb	if (_pasm_left < _pasm_right) goto
#define ja	if (_pasm_left > _pasm_right) goto
#define jbe	if (_pasm_left <= _pasm_right) goto
#define jae	if (_pasm_left >= _pasm_right) goto
#define jl	if ((int64_t) _pasm_left < (int64_t) _pasm_right) goto
#define jg	if ((int64_t) _pasm_left > (int64_t) _pasm_right) goto
#define jle	if ((int64_t) _pasm_left <= (int64_t) _pasm_right) goto
#define jge	if ((int64_t) _pasm_left >= (int64_t) _pasm_right) goto

#define _pasm_mem(type, base, offset) *((type*) (((char*) base) + offset))

// ldb DEST BASE OFFSET: load an unsigned 8-bit value into DEST from BASE + OFFSET

void _pasm_ldb1(uint64_t* offset) {
	*_pasm_dest = _pasm_mem(uint8_t, _pasm_left, *offset);
}

_pasm_partial1 _pasm_ldb2(uint64_t* base) {
	_pasm_left = *base;
	return _pasm_ldb1;
}

_pasm_partial2 ldb(uint64_t* dest) {
	_pasm_dest = dest;
	return _pasm_ldb2;
}

// stb SRC BASE OFFSET: store an unsigned 8-bit value from SRC into BASE + OFFSET

void _pasm_stb1(uint64_t* offset) {
	_pasm_mem(uint8_t, _pasm_left, *offset) = *_pasm_dest;
}

_pasm_partial1 _pasm_stb2(uint64_t* base) {
	_pasm_left = *base;
	return _pasm_stb1;
}

_pasm_partial2 stb(uint64_t* src) {
	_pasm_dest = src;
	return _pasm_stb2;
}

#endif
