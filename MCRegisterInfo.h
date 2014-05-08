//=== MC/MCRegisterInfo.h - Target Register Description ---------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file describes an abstract interface used to get information about a
// target machines register file.  This information is used for a variety of
// purposed, especially register allocation.
//
//===----------------------------------------------------------------------===//

/* Capstone Disassembly Engine */
/* By Nguyen Anh Quynh <aquynh@gmail.com>, 2013-2014 */

#ifndef CS_LLVM_MC_MCREGISTERINFO_H
#define CS_LLVM_MC_MCREGISTERINFO_H

#include <stdbool.h>
#include <stdint.h>

/// An unsigned integer type large enough to represent all physical registers,
/// but not necessarily virtual registers.
typedef uint16_t MCPhysReg;
typedef MCPhysReg* iterator;

typedef struct MCRegisterClass {
	char *Name;
	iterator RegsBegin;
	uint8_t *RegSet;
	uint16_t RegsSize;
	uint16_t RegSetSize;
	uint16_t ID;
	uint16_t RegSize, Alignment; // Size & Alignment of register in bytes
	int8_t CopyCost;
	bool Allocatable;
} MCRegisterClass;

/// MCRegisterDesc - This record contains information about a particular
/// register.  The SubRegs field is a zero terminated array of registers that
/// are sub-registers of the specific register, e.g. AL, AH are sub-registers
/// of AX. The SuperRegs field is a zero terminated array of registers that are
/// super-registers of the specific register, e.g. RAX, EAX, are
/// super-registers of AX.
///
typedef struct MCRegisterDesc {
	uint32_t Name;      // Printable name for the reg (for debugging)
	uint32_t SubRegs;   // Sub-register set, described above
	uint32_t SuperRegs; // Super-register set, described above

	// Offset into MCRI::SubRegIndices of a list of sub-register indices for each
	// sub-register in SubRegs.
	uint32_t SubRegIndices;

	// RegUnits - Points to the list of register units. The low 4 bits holds the
	// Scale, the high bits hold an offset into DiffLists. See MCRegUnitIterator.
	uint32_t RegUnits;
} MCRegisterDesc;

/// MCRegisterInfo base class - We assume that the target defines a static
/// array of MCRegisterDesc objects that represent all of the machine
/// registers that the target has.  As such, we simply have to track a pointer
/// to this array so that we can turn register number into a register
/// descriptor.
///
/// Note this class is designed to be a base class of TargetRegisterInfo, which
/// is the interface used by codegen. However, specific targets *should never*
/// specialize this class. MCRegisterInfo should only contain getters to access
/// TableGen generated physical register data. It must not be extended with
/// virtual methods.
///
typedef struct MCRegisterInfo {
	MCRegisterDesc *Desc;                 // Pointer to the descriptor array
	unsigned NumRegs;                           // Number of entries in the array
	unsigned RAReg;                             // Return address register
	unsigned PCReg;                             // Program counter register
	MCRegisterClass *Classes;             // Pointer to the regclass array
	unsigned NumClasses;                        // Number of entries in the array
	unsigned NumRegUnits;                       // Number of regunits.
	uint16_t (*RegUnitRoots)[2];          // Pointer to regunit root table.
	MCPhysReg *DiffLists;                 // Pointer to the difflists array
	char *RegStrings;                     // Pointer to the string table.
	uint16_t *SubRegIndices;              // Pointer to the subreg lookup
	// array.
	unsigned NumSubRegIndices;                  // Number of subreg indices.
	uint16_t *RegEncodingTable;           // Pointer to array of register
	// encodings.
} MCRegisterInfo;

void MCRegisterInfo_InitMCRegisterInfo(MCRegisterInfo *RI,
		MCRegisterDesc *D, unsigned NR, unsigned RA,
		unsigned PC,
		MCRegisterClass *C, unsigned NC,
		uint16_t (*RURoots)[2],
		unsigned NRU,
		MCPhysReg *DL,
		char *Strings,
		uint16_t *SubIndices,
		unsigned NumIndices,
		uint16_t *RET);


unsigned MCRegisterInfo_getMatchingSuperReg(MCRegisterInfo *RI, unsigned Reg, unsigned SubIdx, MCRegisterClass *RC);

unsigned MCRegisterInfo_getSubReg(MCRegisterInfo *RI, unsigned Reg, unsigned Idx);

MCRegisterClass* MCRegisterInfo_getRegClass(MCRegisterInfo *RI, unsigned i);

bool MCRegisterClass_contains(MCRegisterClass *c, unsigned Reg);

#endif
